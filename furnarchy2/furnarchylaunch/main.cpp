#include "common/crc32.h"
#include "common/x86pp.h"
#include "process_locate.h"
#include "common/IniFile.h"
#include "tinyxml/tinyxml.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ShlObj.h>
#include <cstring>
#include <string>
#include <fstream>

using namespace std;

//////////////////////////////////////////////////////////////////////////

bool launch_client_process( );
void kill_client_process( );
bool patch_client_process( );
bool resume_client_process( );
void cleanup( );
bool locate_client_exe( );
string find_config_file( );
string get_user_root( );

//////////////////////////////////////////////////////////////////////////

string              g_client_exe;
unsigned int        g_client_crc;
PROCESS_INFORMATION g_process_info;
IniFile             g_ini( find_config_file( ) );

//////////////////////////////////////////////////////////////////////////

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, int nShowCmd  )
{
   g_ini.open( false );

   if (launch_client_process( ))
   {
      if (patch_client_process( ))
      {
         if (!resume_client_process( ))
         {
            kill_client_process( );
            cleanup( );
            return 1;
         }
      }
      else
      {
         kill_client_process( );
         cleanup( );
         return 1;
      }

      cleanup( );
   }
   else
      return 1;

   return 0;
}

bool launch_client_process( )
{
   if (!locate_client_exe( ))
      return false;

   /* Client directory. */
   string client_root;
   client_root.assign( g_client_exe, 0, g_client_exe.find_last_of( '\\' ) );   

   /* Prefix the PATH env var with our root folder. */
   char sz_my_root[ MAX_PATH ];
   GetModuleFileName( GetModuleHandle( 0 ), sz_my_root, 512 );
   *strrchr( sz_my_root, '\\' ) = '\0';
   size_t my_root_len = strlen( sz_my_root );

   size_t env_path_len = GetEnvironmentVariable( "PATH", 0, 0 );

   char* sz_env_path = new char[ my_root_len + 1 + env_path_len + 1 ];
   strcpy( sz_env_path, sz_my_root );
   strcat( sz_env_path, ";" );
   GetEnvironmentVariable( "PATH", sz_env_path + strlen( sz_env_path ), 
                           env_path_len + 1 );
   SetEnvironmentVariable( "PATH", sz_env_path );

   /* Command line. */
   string cmd_line = "\"";
   cmd_line += g_client_exe;
   cmd_line += "\" ";

   /* Pass our arguments on verbatim. */
   const char* my_cmd_line = GetCommandLine( );

   // Skip past our path.
   if (my_cmd_line[ 0 ] == '\"') 
      my_cmd_line = strchr( my_cmd_line + 1, '"' );
   else
      my_cmd_line = strchr( my_cmd_line, ' ' );

   if (!my_cmd_line)
      my_cmd_line = "";
   else
      ++my_cmd_line;

   cmd_line += my_cmd_line;
   
   /* Fire up the client in a suspended state and set to inherit our
      environment variables. */

   STARTUPINFO startup_info;
   memset( &startup_info, 0, sizeof( startup_info ) );
   startup_info.cb = sizeof( startup_info );
   
   // Idiot createprocess modifies the command line param.
   char* cmd_line_cpy = _strdup( cmd_line.c_str( ) );

   if (!CreateProcess( g_client_exe.c_str( ), cmd_line_cpy,
                       0, 0, FALSE, CREATE_NEW_CONSOLE | CREATE_SUSPENDED,
                       0, client_root.c_str( ),
                       &startup_info, &g_process_info ))
   {
      MessageBox( 0, "Couldn't launch Furcadia.exe!", "CreateProcess failed.",
                  MB_OK | MB_ICONSTOP );
      free( cmd_line_cpy );
      return false;
   }

   free( cmd_line_cpy );
   return true;
}

void cleanup( )
{
   if (g_process_info.hProcess)
      CloseHandle( g_process_info.hProcess );
   if (g_process_info.hThread)
      CloseHandle( g_process_info.hThread );
}

bool resume_client_process( )
{
   if (-1 == ResumeThread( g_process_info.hThread ))
      return false;
   return true;
}

void kill_client_process( )
{
   TerminateProcess( g_process_info.hProcess, 0 );
}

bool patch_client_process( )
{
   /* Build the string buffer. */
   static char strbuf[ 512 ];

   GetModuleFileName( GetModuleHandle( 0 ), strbuf, 512 );
   *strrchr( strbuf, '\\' ) = '\0';
   strcat( strbuf, "\\furn2.dll" );

   size_t core_path_len = strlen( strbuf );
  
   strcpy( strbuf + (core_path_len + 1), "go" );

   size_t core_entry_name_len = strlen( "go" );

   const size_t strbuf_size = core_path_len + 1 + core_entry_name_len + 1;

   /* Stick the string buffer in the client's PE headers. */
   size_t max_size = 0;
   char* v_strs = process_locate_header_surplus( g_process_info.hProcess, &max_size );

   if (!v_strs || strbuf_size >= max_size)
   {
      MessageBox( 0, "Unable to inject strings into client.", 
                  "Client patching failed.", MB_OK | MB_ICONSTOP );
      return false;
   }

   DWORD old_protect;
   VirtualProtectEx( g_process_info.hProcess, v_strs, strbuf_size, 
                     PAGE_READWRITE, &old_protect );
                     
   SIZE_T bytes_written = 0;
   if (!WriteProcessMemory( g_process_info.hProcess, v_strs, 
                            strbuf, strbuf_size, &bytes_written )
       || bytes_written != strbuf_size)
   {
      MessageBox( 0, "Unable to inject strings into client.", 
                  "Client patching failed.", MB_OK | MB_ICONSTOP );
      return false;
   }

   VirtualProtectEx( g_process_info.hProcess, v_strs, strbuf_size,
                     old_protect, &old_protect );

   /* Determine where the furnarchy loader will be injected. */
   char* v_loader = process_locate_code_surplus( g_process_info.hProcess, &max_size );
   
   if (!v_loader)
   {
      MessageBox( 0, "Unable to inject loader into client.", 
                  "Client patching failed.", MB_OK | MB_ICONSTOP );
      return false;
   }

   /* Build the furnarchy loader. */
   static char loader[ 256 ];
   char* ops = loader;

   using namespace x86pp;

   // All as virtual addresses.
   imm32 core_dll_path_sz = (imm32) v_strs;
   imm32 core_dll_entry_sz = (imm32) (v_strs + (core_path_len + 1));
   mem32 loadlibrary_thunk = (mem32) 
      process_locate_import_thunk( g_process_info.hProcess, "kernel32.dll",
                                   "LoadLibraryA", false );
   mem32 getprocaddr_thunk = (mem32)
      process_locate_import_thunk( g_process_info.hProcess, "kernel32.dll",
                                   "GetProcAddress", false );

   CONTEXT thread_context;
   thread_context.ContextFlags = CONTEXT_CONTROL;
   GetThreadContext( g_process_info.hThread, &thread_context );

   #define ASM_EIP( ) ( v_loader + ((size_t) ops - (size_t) loader) )
   
   /* Kernel32 jumpback relies on current GP reg values. */
   ops += asm_pushd( ops );
   ops += asm_push( ops, core_dll_path_sz );
   ops += asm_call( ops, loadlibrary_thunk, 0 );
   ops += asm_test( ops, Registers::EAX, Registers::EAX );
   ops += asm_push( ops, core_dll_entry_sz );
   ops += asm_push( ops, Registers::EAX );
   ops += asm_call( ops, getprocaddr_thunk, 0 );
   ops += asm_push( ops, (imm32) g_client_crc );
   ops += asm_call( ops, Registers::EAX );
   ops += asm_add( ops, Registers::ESP, 4 );
   ops += asm_popd( ops );
   ops += asm_jmp( ops, (rel32) thread_context.Eip - (rel32) ASM_EIP( ) );

   size_t loader_size = (size_t) ops - (size_t) loader;

   /* Write the loader. */
   VirtualProtectEx( g_process_info.hProcess, v_loader, loader_size, 
                     PAGE_READWRITE, &old_protect );
   
   if (!WriteProcessMemory( g_process_info.hProcess, v_loader, 
                            loader, loader_size, &bytes_written )
       || bytes_written != loader_size)
   {
      MessageBox( 0, "Unable to inject loader into client.", 
                  "Client patching failed.", MB_OK | MB_ICONSTOP );
      return false;
   }

   VirtualProtectEx( g_process_info.hProcess, v_loader, loader_size,
                     old_protect, &old_protect );

   /* Move EIP to the loader routine. */
   thread_context.Eip = (DWORD) v_loader;
   SetThreadContext( g_process_info.hThread, &thread_context );

   return true;
}

bool locate_client_exe( )
{
   /* If the path is specified in launch.ini, check there first. */
   g_client_exe = g_ini.read( "ClientExe" );

   if (!g_client_exe.empty( ) && 
       INVALID_FILE_ATTRIBUTES != GetFileAttributes( g_client_exe.c_str( ) ))
   {
      g_client_crc = crc32_stream( ifstream( g_client_exe.c_str( ), ios::binary ) );
      return true;
   }

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
            g_client_exe = file;
            g_client_crc = crc32_stream( ifstream( g_client_exe.c_str( ), ios::binary ) );
            RegCloseKey( key );
            return true;
         }
      }

      RegCloseKey( key );
   }

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
            g_client_exe = file;
            g_client_crc = crc32_stream( ifstream( g_client_exe.c_str( ), ios::binary ) );
            return true;
         }

         file.clear( );
         file.push_back( letter );
         file.append( ":\\Furcadia\\Furcadia.exe" );
         if (INVALID_FILE_ATTRIBUTES != GetFileAttributes( file.c_str( ) ))
         {
            g_client_exe = file;
            g_client_crc = crc32_stream( ifstream( g_client_exe.c_str( ), ios::binary ) );
            return true;
         }
      }

      drives = drives >> 1;
   }
   
   MessageBox( 0, "Unable to find Furcadia.exe!  "
                  "Check that the launch.ini file is correct.",
                  "Client executable couldn't be found.", MB_OK | MB_ICONSTOP );

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