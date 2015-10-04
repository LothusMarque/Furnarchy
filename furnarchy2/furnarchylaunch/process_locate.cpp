#include "process_locate.h"
#include <cstring>

using std::size_t;

/** Copies a process' PE headers. */
bool process_copy_pe_header( HANDLE process, void* base, IMAGE_DOS_HEADER* dh, 
                             IMAGE_NT_HEADERS* nth )
{
   SIZE_T bytes_read;

   if (!ReadProcessMemory( process, base, dh, sizeof( IMAGE_DOS_HEADER ), &bytes_read )
       || bytes_read != sizeof( IMAGE_DOS_HEADER ))
      return false;

   ((char*&)base) += dh->e_lfanew;

   if (!ReadProcessMemory( process, base, nth, sizeof( IMAGE_NT_HEADERS ), &bytes_read )
       || bytes_read != sizeof( IMAGE_NT_HEADERS ))
      return false;

   return true;
}

/** Looks for unused bytes typically at the end the PE header. */
char* process_locate_header_surplus( HANDLE process, size_t* max_size_out )
{
   if (max_size_out)
      *max_size_out = 0;

   // We should have the same module bases.
   char* const mod_base = (char*) GetModuleHandle( 0 );

   IMAGE_DOS_HEADER dh;
   IMAGE_NT_HEADERS nth;
   if (!process_copy_pe_header( process, mod_base, &dh, &nth ))
      return 0;

   /* Look for the end of the section headers. */
   char* vaddr = mod_base + (dh.e_lfanew + sizeof( nth ));
   const char* const vaddr_end = mod_base + (nth.OptionalHeader.SizeOfHeaders);

   SIZE_T bytes_read;
   IMAGE_SECTION_HEADER sh;

   while (vaddr < vaddr_end)
   {
      if (!ReadProcessMemory( process, vaddr, &sh, sizeof( sh ), &bytes_read )
          || bytes_read != sizeof( sh ))
         return 0;

      vaddr = vaddr + sizeof( IMAGE_SECTION_HEADER );

      if (!sh.Name[ 0 ])
         break;   // Found it.
   }

   if (vaddr >= vaddr_end)
      return 0;

   if (max_size_out)
      *max_size_out = nth.OptionalHeader.SizeOfHeaders - ((SIZE_T) vaddr - (SIZE_T) mod_base);

   return vaddr;
}

/** Looks for unused bytes typically at the end of the code section. */
char* process_locate_code_surplus( HANDLE process, size_t* max_size_out )
{
   if (max_size_out)
      *max_size_out = 0;

   // We should have the same module bases.
   char* const mod_base = (char*) GetModuleHandle( 0 );

   IMAGE_DOS_HEADER dh;
   IMAGE_NT_HEADERS nth;
   if (!process_copy_pe_header( process, mod_base, &dh, &nth ))
      return false;
   
   char* const vaddr_begin = mod_base + (nth.OptionalHeader.BaseOfCode);
   char* vaddr = vaddr_begin + nth.OptionalHeader.SizeOfCode;

   /* Read in a page at a time. */
   const SIZE_T page_size = nth.OptionalHeader.SectionAlignment;
   char* page = new char[ page_size ];
   SIZE_T bytes_read;

   while (vaddr > vaddr_begin)
   {
      if (!ReadProcessMemory( process, vaddr - page_size, page, page_size, &bytes_read )
          || bytes_read != page_size)
      {
         delete [] page;
         return 0;
      }

      /* Scan backwards for opcodes. */
      SIZE_T i = 0;
      for (; i < page_size && page[ page_size - i -1 ] == 0x00; ++i, --vaddr);

      if (i < page_size)
         break;
   }

   delete [] page;

   if (vaddr <= vaddr_begin)
      return 0;

   /* Need to distance ourselves a bit (biggest jmp instr. is 8 bytes). */
   if (vaddr + 8 >= vaddr_begin + nth.OptionalHeader.SizeOfCode)
      return 0;
   
   vaddr = vaddr + (8);

   if (max_size_out)
      *max_size_out = nth.OptionalHeader.SizeOfCode - 
                      ((SIZE_T) vaddr - (SIZE_T) vaddr_begin);

   return vaddr;
}

/** Looks for an import thunk (the real one). */
char* process_locate_import_thunk( HANDLE process, const char* lib,
                                   const char* symbol, bool symbol_is_ordinal )
{
   // We should have the same module bases.
   char* const mod_base = (char*) GetModuleHandle( 0 );

   IMAGE_DOS_HEADER dh;
   IMAGE_NT_HEADERS nth;
   if (!process_copy_pe_header( process, mod_base, &dh, &nth ))
      return 0;

   if (!nth.OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress)
      return 0; // No imports.

   char* p_desc = mod_base + nth.OptionalHeader.
                        DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].
                           VirtualAddress;

   SIZE_T bytes_read;
   IMAGE_IMPORT_DESCRIPTOR imp_desc;
   char sz_name[ 64 ];

   while (true)
   {
      if (!ReadProcessMemory( process, p_desc, &imp_desc, sizeof( imp_desc ), &bytes_read )
          || bytes_read != sizeof( imp_desc ))
          return 0;

      if (!imp_desc.OriginalFirstThunk)   // Empty descriptor indicates end of list.
         break;

      if (imp_desc.Name)
      {
         /* Fetch the library name. */
         char c;
         for (SIZE_T i = 0; i < 64; i++)
         {
            if (!ReadProcessMemory( process, mod_base + (imp_desc.Name + i), 
                                    &c, 1, &bytes_read ) || bytes_read != 1)
                return 0;

            sz_name[ i ] = c;
            if (c == '\0') break;
         }
         sz_name[ 63 ] = '\0';

         if (!_stricmp( lib, sz_name ))
            break;   // Found the library.
      }

      p_desc = p_desc + sizeof( IMAGE_IMPORT_DESCRIPTOR );
   }

   if (!imp_desc.OriginalFirstThunk)
      return 0;   // Couldn't find the library.

   /* Check each import thunk in the library. */
   char* p_orig = mod_base + (imp_desc.OriginalFirstThunk);
   char* p_bound = mod_base + (imp_desc.FirstThunk);
   IMAGE_THUNK_DATA orig;

   while (true)
   {
      if (!ReadProcessMemory( process, p_orig, &orig, sizeof( orig ), &bytes_read )
          || bytes_read != sizeof( orig ))
          return 0;

      if (!orig.u1.AddressOfData) // Thunk list is terminated by empty thunk.
         break;

      if (orig.u1.Ordinal & IMAGE_ORDINAL_FLAG)
      {
         if (symbol_is_ordinal)
         {
            if ((orig.u1.Ordinal & (~IMAGE_ORDINAL_FLAG)) == *(DWORD*)&symbol)
               return p_bound;   // Found it.
         }
      }
      else if (!symbol_is_ordinal)
      {
         /* Fetch the symbol name. */
         char c;
         for (SIZE_T i = 0; i < 64; i++)
         {
            if (!ReadProcessMemory( process, mod_base + (orig.u1.AddressOfData + 2 + i), 
                                    &c, 1, &bytes_read ) || bytes_read != 1)
               return 0;

            sz_name[ i ] = c;
            if (c == '\0') break;
         }
         sz_name[ 63 ] = '\0';

         if (!std::strcmp( symbol, sz_name ))
            return p_bound;   // Found it.
      }

      p_orig = p_orig + sizeof( IMAGE_THUNK_DATA );
      p_bound = p_bound + sizeof( IMAGE_THUNK_DATA );
   }

   return 0;
}

/** Looks for the process entry point. */
char* process_locate_entry_point( HANDLE process )
{
   // We should have the same module bases.
   char* const mod_base = (char*) GetModuleHandle( 0 );

   IMAGE_DOS_HEADER dh;
   IMAGE_NT_HEADERS nth;
   if (!process_copy_pe_header( process, mod_base, &dh, &nth ))
      return 0;

   return mod_base + nth.OptionalHeader.AddressOfEntryPoint;
}