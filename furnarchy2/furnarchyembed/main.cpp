#include "common/crc32.h"
#include "common/x86pp.h"
#include "common/IniFile.h"
#include "tinyxml/tinyxml.h"
#include "image_locate.h"
#include <iostream>
#include <string>
#include <cstring>
#include <ShlObj.h>

using namespace std;

//////////////////////////////////////////////////////////////////////////

bool locate_client_exe( );
bool load_client_image( );
bool patch_client_image( );
bool write_client_image( );
void cleanup( );
string find_config_file( );
string get_user_root( );

//////////////////////////////////////////////////////////////////////////

string       g_client_path;
void*        g_client_image;
size_t       g_client_image_size;
unsigned int g_client_crc;
IniFile      g_ini( find_config_file( ) );

//////////////////////////////////////////////////////////////////////////

int main( int argc, char* argv[] )
{
   if (argc > 1)
   {
      if (!_stricmp( "--help", argv[ 1 ] ) || !_stricmp( "-help", argv[ 1 ] ))
      {
         cout << "Usage: embed.exe [path to Furcadia.exe]\n"
                 "This will create a file called 'Furcadiaf2.exe' in the\n"
                 "current directory.  This new executable is patched to\n"
                 "automatically load furnarchy2 when run.\n"
                 "If no path is specified, embed.exe will look in furnarchy's\n"
                 "launch.ini file then look for furcadia.exe itself.\n\n"
              << "Warning: The path of furnarchy2 is hardcoded into the\n"
                 "created executable, so the executable will stop working if\n"
                 "you decide to relocate the furnarchy2 files.\n"
                 "Also, embed.exe relies on its own path (not the current path)\n"
                 "to determine the furnarchy2 directory, so don't move embed.exe.\n";
         system( "pause" );
         return EXIT_SUCCESS;
      }

      g_client_path = argv[ 1 ];
   }
   else
   {   
      if (!locate_client_exe( ))
      {
         cleanup( );
         system( "pause" );
         return EXIT_FAILURE;
      }
   }

   if (!load_client_image( ))
   {
      cleanup( );
      system( "pause" );
      return EXIT_FAILURE;
   }

   if (!patch_client_image( ))
   {
      cleanup( );
      system( "pause" );
      return EXIT_FAILURE;
   }

   if (!write_client_image( ))
   {
      cleanup( );
      system( "pause" );
      return EXIT_FAILURE;
   }

   cleanup( );
   system( "pause" );
   return EXIT_SUCCESS;
}

bool patch_client_image( )
{
   cout << "Patching the client image.\n";

   /* Build the string buffer. */
   /* Stick the string buffer in the client's PE headers. */
   size_t max_size = 0;
   char* strbuf = (char*) image_locate_header_surplus( g_client_image, &max_size );
   char* p = strbuf;

   if (!strbuf)
   {
      cerr << "Couldn't fit strings into the client headers!\n";
      return false;
   }

   GetModuleFileName( GetModuleHandle( 0 ), p, MAX_PATH );
   *strrchr( p, '\\' ) = '\0';
   strcat( p, "\\furn2.dll" );
   const char* sz_core_path = p;
   p += strlen( p ) + 1;

   strcpy( p, "go" );
   const char* sz_core_entry = p;
   p += strlen( p ) + 1;

   strcpy( p, "KERNEL32.DLL" );
   const char* sz_kernel32 = p;
   p += strlen( p ) + 1;

   strcpy( p, "GetEnvironmentVariableA" );
   const char* sz_getenv = p;
   p += strlen( p ) + 1;

   GetModuleFileName( GetModuleHandle( 0 ), p, MAX_PATH );
   *strrchr( p, '\\' ) = '\0';
   strcat( p, ";" );
   const char* sz_furn_dir = p;
   p += strlen( p ) + 1;

   strcpy( p, "PATH" );
   const char* sz_path = p;
   p += strlen( p ) + 1;

   const size_t strbuf_size = (size_t) p - (size_t) strbuf;   

   if (strbuf_size >= max_size)
   {
      cerr << "Couldn't fit strings into the client headers!\n";
      return false;
   }

   cout << "Embedded strings.\n";

   /* Find a place to stick the load routine. */
   char* loader = (char*) image_locate_code_surplus( g_client_image, &max_size );

   if (!loader)
   {
      cerr << "Couldn't fit load routine into the code section! (Scanning Phase)\n";
      return false;
   }

   /* Generate the load routine. */

   using namespace x86pp;

   intptr_t base = (intptr_t) GetModuleHandle( 0 );
   #define VIRT( p ) (image_addr_to_rva( g_client_image, p ) + base)

   // All virtual.
   const imm32 vsz_core_path = (imm32) VIRT( sz_core_path );
   const imm32 vsz_core_entry = (imm32) VIRT( sz_core_entry );
   const imm32 vsz_kernel32 = (imm32) VIRT( sz_kernel32 );
   const imm32 vsz_getenv = (imm32) VIRT( sz_getenv );
   const imm32 vsz_furn_dir = (imm32) VIRT( sz_furn_dir );
   const imm32 vsz_path = (imm32) VIRT( sz_path );

   const mem32 v_loadlibrary_thunk = (mem32) 
      VIRT( image_locate_import_thunk( g_client_image, "kernel32.dll",
                                       "LoadLibraryA", false ) );
   const mem32 v_freelibrary_thunk = (mem32) 
      VIRT( image_locate_import_thunk( g_client_image, "kernel32.dll",
                                       "FreeLibrary", false ) );
   const mem32 v_getprocaddr_thunk = (mem32)
      VIRT( image_locate_import_thunk( g_client_image, "kernel32.dll",
                                       "GetProcAddress", false ) );
   const mem32 v_setenv_thunk = (mem32)
      VIRT( image_locate_import_thunk( g_client_image, "kernel32.dll",
                                       "SetEnvironmentVariableA", false ) );

   const char* v_orig_entry_pt = (char*) VIRT( image_locate_entry_point( g_client_image ) );
   const char* v_loader = (char*) VIRT( loader );

   #define ASM_EIP( ) ( (size_t) v_loader + ((size_t) ops - (size_t) loader) )
   
   char* ops = loader;

   assert( image_rva_to_addr( g_client_image, image_addr_to_rva( g_client_image, image_locate_entry_point( g_client_image ) ) ) == image_locate_entry_point( g_client_image ) );

   /* All this just to set the proper PATH env variable :`( */
   ops += asm_push( ops, vsz_kernel32 );
   ops += asm_call( ops, v_loadlibrary_thunk, 0 );
   ops += asm_push( ops, Registers::EAX );
   ops += asm_push( ops, vsz_getenv );
   ops += asm_push( ops, Registers::EAX );
   ops += asm_call( ops, v_getprocaddr_thunk, 0 );
   ops += asm_sub( ops, Registers::ESP, (imm32) 512 );
   ops += asm_push( ops, (imm32) (512 - strlen( sz_furn_dir )) );
   ops += asm_mov( ops, Registers::ECX, Registers::ESP );
   ops += asm_add( ops, Registers::ECX, (imm32) (4 + strlen( sz_furn_dir )) );
   ops += asm_push( ops, Registers::ECX );
   ops += asm_push( ops, vsz_path );
   ops += asm_call( ops, Registers::EAX ); // GetEnvironmentVariableA
   ops += asm_mov( ops, Registers::EDI, Registers::ESP );
   ops += asm_mov( ops, Registers::ESI, vsz_furn_dir );
   ops += asm_mov( ops, Registers::ECX, (imm32) strlen( sz_furn_dir ) );
   ops += asm_rep_movsb( ops );
   ops += asm_push( ops, Registers::ESP );
   ops += asm_push( ops, vsz_path );
   ops += asm_call( ops, v_setenv_thunk, 0 );
   ops += asm_add( ops, Registers::ESP, 512 );
   ops += asm_call( ops, v_freelibrary_thunk, 0 );

   /* Load and init furnarchy. */
   ops += asm_push( ops, vsz_core_path );
   ops += asm_call( ops, v_loadlibrary_thunk, 0 );
   ops += asm_test( ops, Registers::EAX, Registers::EAX );
   ops += asm_push( ops, vsz_core_entry );
   ops += asm_push( ops, Registers::EAX );
   ops += asm_call( ops, v_getprocaddr_thunk, 0 );
   ops += asm_push( ops, (imm32) g_client_crc );
   ops += asm_call( ops, Registers::EAX );
   ops += asm_add( ops, Registers::ESP, 4 );
   ops += asm_jmp( ops, (rel32) v_orig_entry_pt - (rel32) ASM_EIP( ) );

   const size_t loader_size = (size_t) ops - (size_t) loader;

   if (loader_size >= max_size)
   {
      cerr << "Couldn't fit load routine into the code section!\n Needed " << loader_size << ", had " << max_size <<".\n";
      return false;
   }

   cout << "Embedded load routine.\n";

   image_set_entry_point( g_client_image, loader );

   cout << "Changed entry point.\n";

   cout << "Patched the client image!\n";
   return true;
}

void cleanup( )
{
   delete [] g_client_image;
   g_client_image = 0;
   g_client_image_size = 0;
   g_client_path.clear( );
   g_client_crc = 0;
}

bool load_client_image( )
{
   cout << "Reading the client exe at '" << g_client_path << "'...\n";

   /* Read in the exe. */
   HANDLE fh = CreateFile( g_client_path.c_str( ), GENERIC_READ, 
                           FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );

   if (fh == INVALID_HANDLE_VALUE)
   {
      cerr << "Failed to open '" << g_client_path << "' for reading.\n";
      return false;
   }
   
   g_client_image_size = (size_t) GetFileSize( fh, 0 );
   g_client_image      = new char[ g_client_image_size ];
   {
      DWORD bytes_read = 0;
      if (!ReadFile( fh, g_client_image, (DWORD) g_client_image_size, &bytes_read, 0 )
          || bytes_read != g_client_image_size)
      {
         cerr << "Failed to read in the entire executable at '"
              << g_client_path << "'\n";
         CloseHandle( fh );
         return false;
      }
   }

   CloseHandle( fh );

   /* CRC the exe. */
   g_client_crc = crc32_bytes( g_client_image, g_client_image_size );

   cout << "Read the client exe!\n";
   return true;
}

bool write_client_image( )
{
   cout << "Creating Furcadiaf2.exe...\n";

   HANDLE fh = CreateFile( "Furcadiaf2.exe", GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
                           FILE_ATTRIBUTE_NORMAL, 0 );

   if (fh == INVALID_HANDLE_VALUE)
   {
      cerr << "Unable to open 'Furcadiaf2.exe' for writing.\n";
      return false;
   }

   {
      DWORD bytes_written = 0;
      if (!WriteFile( fh, g_client_image, (DWORD) g_client_image_size, &bytes_written, 0 )
          || bytes_written != g_client_image_size)
      {
         cerr << "Failed to write the entire executable 'Furcadiaf2.exe'\n";
         CloseHandle( fh );
         return false;
      }
   }

   CloseHandle( fh );

   cout << "Created Furcadiaf2.exe!\n";
   return true;
}

bool locate_client_exe( )
{
   cout << "Looking for the client exe...\n";
   cout << "Checking furnarchy2's launch.ini file...\n";

   g_ini.open( );
   g_client_path = g_ini.read( "ClientExe" );
   g_ini.close( );

   if (g_client_path.length( ) && 
       INVALID_FILE_ATTRIBUTES != GetFileAttributes( g_client_path.c_str( ) ))
   {
      cout << "Found it!\n";
      return true;
   }

   cout << "Checking Furcadia's registry keys...\n";
   string file;

   /* Check the registry keys. */
   HKEY key;
   if (ERROR_SUCCESS == RegOpenKeyEx( HKEY_LOCAL_MACHINE, 
                                      "Software\\Dragon\'s Eye Productions"
                                      "\\Furcadia\\Programs", 0,
                                      KEY_READ, &key ))
   {
      char reg_value[ MAX_PATH + 1 ];  reg_value[ MAX_PATH ] = '\0';
      DWORD size = MAX_PATH;
      DWORD type = REG_SZ;

      if (ERROR_SUCCESS == RegQueryValueEx( key, "Path", 0, &type, (BYTE*) reg_value, &size ))
      {
         file = reg_value; // Has trailing backslash.
         file += "Furcadia.exe";
         if (INVALID_FILE_ATTRIBUTES != GetFileAttributes( file.c_str( ) ))
         {
            g_client_path = file;
            RegCloseKey( key );
            cout << "Found it!\n";
            return true;
         }
      }

      RegCloseKey( key );
   }

   cout << "Just flat-out guessing now...\n";

   /* Check the usual suspects. */
   DWORD drives = GetLogicalDrives( );

   for (char letter = 'A'; drives; letter++)
   {
      if (drives & 0x1)
      {
         file.clear( );
         file.push_back( letter );
         file.append( ":\\Program Files\\Furcadia\\Furcadia.exe" );
         if (INVALID_FILE_ATTRIBUTES != GetFileAttributes( file.c_str( ) ))
         {
            g_client_path = file;
            cout << "Found it!\n";
            return true;
         }

         file.clear( );
         file.push_back( letter );
         file.append( ":\\Furcadia\\Furcadia.exe" );
         if (INVALID_FILE_ATTRIBUTES != GetFileAttributes( file.c_str( ) ))
         {
            g_client_path = file;
            cout << "Found it!\n";
            return true;
         }
      }

      drives = drives >> 1;
   }

   cerr << "Couldn't find Furcadia.exe anywhere.\n"
           "Check that the launch.ini file is correct.\n";
   return false;
}


string find_config_file( )
{
   // Check the single-user mode option.
   bool single_user = false;
   {
      TiXmlDocument xml_doc( "options.xml" );
      if (xml_doc.LoadFile( ) && !xml_doc.Error( ) &&
          xml_doc.RootElement( ) && xml_doc.RootElement( )->ValueStr( ) == "furnarchyOpts")
      {
         TiXmlElement* xml_e = xml_doc.RootElement( )->FirstChildElement( "singleUserMode" );
         if (xml_e && xml_e->GetText( ) == string( "true" ))
            single_user = true;
      }
   }

   string path = "launch.ini";
   if (!single_user)
   {
      path = get_user_root( ) + "\\launch.ini";
      if (INVALID_FILE_ATTRIBUTES == GetFileAttributes( path.c_str( ) ))
         path = "launch.ini";
   }

   return path;
}

string get_user_root( )
{
   char sz_path[ MAX_PATH ];
   SHGetFolderPath( NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, 
                    NULL, SHGFP_TYPE_CURRENT, sz_path );
   return string( sz_path ) + "\\Furnarchy2";
}