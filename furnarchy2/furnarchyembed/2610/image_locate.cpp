#include "image_locate.h"
#include <cstring>

using std::size_t;

char* image_locate_header_surplus( void* base, size_t* max_size_out )
{
   if (max_size_out)
      *max_size_out = 0;

   IMAGE_DOS_HEADER* dh = (IMAGE_DOS_HEADER*) base;
   IMAGE_NT_HEADERS* nth = (IMAGE_NT_HEADERS*) ((char*) dh + dh->e_lfanew);

   /* Look for the end of the section headers. */
   IMAGE_SECTION_HEADER* sh = 
      (IMAGE_SECTION_HEADER*) ((char*) nth + sizeof( IMAGE_NT_HEADERS ));
   IMAGE_SECTION_HEADER* sh_end = 
      (IMAGE_SECTION_HEADER*) ((char*) base + (nth->OptionalHeader.SizeOfHeaders));
   
   while (sh < sh_end)
   {
      if (!sh->Name[ 0 ])
         break;   // Found it.

      ++sh;
   }

   if (sh >= sh_end)
      return 0;

   if (max_size_out)
      *max_size_out = nth->OptionalHeader.SizeOfHeaders - ((SIZE_T) sh - (SIZE_T) base);

   return (char*) sh;
}

char* image_locate_code_surplus( void* base, size_t* max_size_out )
{
   if (max_size_out)
      *max_size_out = 0;

   IMAGE_DOS_HEADER* dh = (IMAGE_DOS_HEADER*) base;
   IMAGE_NT_HEADERS* nth = (IMAGE_NT_HEADERS*) ((char*) dh + dh->e_lfanew);
   
   char* const cs_begin = (char*) base + nth->OptionalHeader.BaseOfCode;
   char* cs = cs_begin + nth->OptionalHeader.SizeOfCode;

   /* Scan backwards for opcodes. */
   for (; cs > cs_begin && cs[-1] == 0x00; --cs ) ;
   
   if (cs <= cs_begin)
      return 0;

   /* Need to distance ourselves a bit (biggest jmp instr. is 8 bytes). */
   if (cs + 8 >= cs_begin + nth->OptionalHeader.SizeOfCode)
      return 0;
   
   cs = cs + 8;
   
   if (max_size_out)
      *max_size_out = nth->OptionalHeader.SizeOfCode - 
                      ((size_t) cs - (size_t) cs_begin);

   return cs;
}

char* image_locate_import_thunk( void* base, const char* lib, 
                                 const char* symbol, bool symbol_is_ordinal )
{
   IMAGE_DOS_HEADER* dh = (IMAGE_DOS_HEADER*) base;
   IMAGE_NT_HEADERS* nth = (IMAGE_NT_HEADERS*) ((char*) dh + dh->e_lfanew);

   if (!nth->OptionalHeader.DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress)
      return 0; // No imports.

   IMAGE_IMPORT_DESCRIPTOR* imp_desc = (IMAGE_IMPORT_DESCRIPTOR*)
                                       ((char*) base + nth->OptionalHeader.
                                          DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].
                                             VirtualAddress);
   while (imp_desc->OriginalFirstThunk) // Empty descriptor is sentinel.
   {
      if (imp_desc->Name)
      {
         if (!_stricmp( (char*) base + imp_desc->Name, lib ))
            break; // Found the library.
      }

      ++imp_desc;
   }

   if (!imp_desc->OriginalFirstThunk)
      return 0;   // Couldn't find the library.

   /* Check each import thunk in the library. */
   IMAGE_THUNK_DATA* thunk_orig = (IMAGE_THUNK_DATA*) ((char*) base + imp_desc->OriginalFirstThunk);
   IMAGE_THUNK_DATA* thunk_real = (IMAGE_THUNK_DATA*) ((char*) base + imp_desc->FirstThunk);

   while (thunk_orig->u1.AddressOfData) // Thunk list is terminated by empty thunk.
   {
      if (thunk_orig->u1.Ordinal & IMAGE_ORDINAL_FLAG)
      {
         if (symbol_is_ordinal)
         {
            if ((thunk_orig->u1.Ordinal & (~IMAGE_ORDINAL_FLAG)) == *(DWORD*)&symbol)
               return (char*) thunk_real;   // Found it.
         }
      }
      else if (!symbol_is_ordinal)
      {
         if (!std::strcmp( (char*) base + (thunk_orig->u1.AddressOfData + 2), symbol ))
            return (char*) thunk_real;
      }
      
      ++thunk_orig;
      ++thunk_real;
   }

   return 0;
}

char* image_locate_entry_point( void* base )
{
   IMAGE_DOS_HEADER* dh = (IMAGE_DOS_HEADER*) base;
   IMAGE_NT_HEADERS* nth = (IMAGE_NT_HEADERS*) ((char*) dh + dh->e_lfanew);

   return (char*) base + nth->OptionalHeader.AddressOfEntryPoint;
}

bool image_set_entry_point( void* base, const void* new_entry )
{
   IMAGE_DOS_HEADER* dh = (IMAGE_DOS_HEADER*) base;
   IMAGE_NT_HEADERS* nth = (IMAGE_NT_HEADERS*) ((char*) dh + dh->e_lfanew);

   DWORD old_protect;
   VirtualProtect( &nth->OptionalHeader.AddressOfEntryPoint, 
                   sizeof( DWORD ), PAGE_EXECUTE_READWRITE, &old_protect );

   nth->OptionalHeader.AddressOfEntryPoint = (DWORD) new_entry;

   VirtualProtect( &nth->OptionalHeader.AddressOfEntryPoint, 
                   sizeof( DWORD ), old_protect, &old_protect );

   return true;
}
