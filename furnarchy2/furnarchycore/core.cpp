#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "core.h"
#include "options.h"
#include "memlocs.h"
#include "client.h"
#include "apihooks.h"
#include "window.h"
#include "net.h"
#include "world.h"
#include "ifilters.h"
#include "ofilters.h"
#include "scripts.h"
#include "modules.h"
#include "skin.h"
#include "news.h"
#include <queue>
#include <fcntl.h>
#include <io.h>
#include <ShlObj.h>

namespace core {

struct F2COMMAND
{
   string cmd;
   bool   noecho;
   F2COMMAND( const string& cmd, bool noecho )
      : cmd( cmd ), noecho( noecho ) {}
};

//////////////////////////////////////////////////////////////////////////

bool             g_initialized  = false;
int              g_silent_count = 0;
HINSTANCE        g_my_instance  = 0;
string           g_client_root;
string           g_client_data_root;
string           g_client_doc_root;
string           g_furn_root;
string           g_furn_data_root;
string           g_furn_doc_root;
unsigned int     g_last_tick    = 0;
unsigned int     g_login_spam_cooldown = 0;
time_t           g_time_started = 0;
Connection_Level g_connlvl = CONNLVL_DISCONNECTED;
Client_Screen    g_screen = SCREEN_TITLE;
queue<F2COMMAND> g_commands;

//////////////////////////////////////////////////////////////////////////

void toggle_console( bool onoff );
bool on_command( const ArgList& args );
void broadcast_commands( );
void on_batch( );

//////////////////////////////////////////////////////////////////////////

bool initialize( HINSTANCE core_instance, unsigned int crc )
{
   if (!g_initialized)
   {
      g_initialized = true;
      g_my_instance = core_instance;

      g_last_tick    = GetTickCount( );
      g_time_started = time( 0 );
      g_connlvl = CONNLVL_DISCONNECTED;

      while (!g_commands.empty( )) g_commands.pop( );

      {
         /* Retrieve client paths. */
         char sz_path[ MAX_PATH + 1 ];
         GetModuleFileName( 0, sz_path, MAX_PATH );
         *strrchr( sz_path, '\\' ) = '\0';
         g_client_root = sz_path;

         SHGetFolderPath( NULL, CSIDL_LOCAL_APPDATA | CSIDL_FLAG_CREATE, 
                          NULL, SHGFP_TYPE_CURRENT, sz_path );
         g_client_data_root = sz_path;
         g_client_data_root += "\\Dragon's Eye Productions\\Furcadia";

         SHGetFolderPath( NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, 
                          NULL, SHGFP_TYPE_CURRENT, sz_path );
         g_client_doc_root = sz_path;
         g_client_doc_root += "\\Furcadia";

         /* Retrieve furnarchy paths. */
         GetModuleFileName( g_my_instance, sz_path, MAX_PATH );
         *strrchr( sz_path, '\\' ) = '\0';
         g_furn_root = sz_path;

         SHGetFolderPath( NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, 
                          NULL, SHGFP_TYPE_CURRENT, sz_path );
         g_furn_data_root = sz_path;
         g_furn_data_root += "\\Furnarchy2";

         SHGetFolderPath( NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, 
                          NULL, SHGFP_TYPE_CURRENT, sz_path );
         g_furn_doc_root = sz_path;
         g_furn_doc_root += "\\Furnarchy2";
      }   

      silence( true );

      options::initialize( );

      if (!options::get_single_user_mode( )) // Multi-user mode.
      {
         /* Make sure the furnarchy skeleton folders in Documents exist. */
         create_folder_path( g_furn_doc_root + "\\filters" );
         create_folder_path( g_furn_doc_root + "\\source" );
         create_folder_path( g_furn_doc_root + "\\skins" );
         create_folder_path( g_furn_doc_root + "\\scripts\\imports" );
         create_folder_path( g_furn_doc_root + "\\scripts\\logs" );
         create_folder_path( g_furn_doc_root + "\\scripts\\dbs" );
         create_folder_path( g_furn_doc_root + "\\module-docs" );

         /* We'll just create the App Data directories as we need them. */
      }
      else  // Single-user mode.
      {
         /* If in single-user mode, make the furnarchy paths all point to the program dir. */
         g_furn_data_root = g_furn_doc_root = g_furn_root;

		 //These need to be created here too, or things get funny. (DBs won't save, etc)
         create_folder_path( g_furn_doc_root + "\\scripts\\logs" );
         create_folder_path( g_furn_doc_root + "\\scripts\\dbs" );
         //create_folder_path( g_furn_doc_root + "\\module-docs" );
      }

      memlocs::initialize( crc );
      client::initialize( );
      apihooks::initialize( );
      window::initialize( );
      net::initialize( );
      world::initialize( );
      ifilters::initialize( );
      ofilters::initialize( );
      scripts::initialize( );
      modules::initialize( );
      skin::initialize( );
      news::initialize( );

      silence( false );

      /* Don't source init until the window has been created.. */
   }

   return true;
}

bool uninitialize( )
{
   if (g_initialized)
   {      
      while (!g_commands.empty( )) g_commands.pop( );

      silence( true );

      news::uninitialize( );
      skin::uninitialize( );
      modules::uninitialize( );
      scripts::uninitialize( );
      ofilters::uninitialize( );
      ifilters::uninitialize( );
      
	  //world::initialize( ); ///!?

      world::uninitialize( );
	  net::uninitialize( );
      window::uninitialize( );
      
	  apihooks::uninitialize( ); //This is cleaner, but is it required?

      client::uninitialize( );
      memlocs::uninitialize( );
      options::uninitialize( );

      silence( false );
      g_initialized = false;
   }

   return true;
}

void log_error( const char* fmt, ... )
{
   /* Prefix the error message. */
   char prefix[ 64 ];
   *prefix = '\0';

   if (window::get_handle( ))
   {
      GetWindowText( window::get_handle( ), prefix, 61 );
      strcat( prefix, "> " );

      size_t len = strlen( prefix );
      for (size_t i = 0; i < len; i++)
         if (prefix[ i ] == '%')
            prefix[ i ] = ' ';
   }

   string new_fmt = prefix;
   new_fmt += fmt;
   new_fmt.push_back( '\n' );

   /* Append to the error log. */
   string path = g_furn_doc_root + "\\errors.log";
   if (GetFileAttributes( g_furn_doc_root.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
      path = g_furn_root + "\\errors.log";

   FILE* log_file = std::fopen( path.c_str( ), "a" );
   if (log_file)
   {
      std::va_list args;
      va_start( args, fmt );

      std::vfprintf( log_file, new_fmt.c_str( ), args );

      va_end( args );
      std::fclose( log_file );
   }
}

void push_chatbox( const string& line )
{
   if (g_screen == SCREEN_MARBLED)
   {  //Memloc method is disabled as of V28b. Causes weird garbage/flip issue with timestamps. Wtf?
      /*if (memlocs::valid( ))
         // Could be a problem if the chatbox doesn't exist yet.
         client::chatbox_push_sysmsg( line );
      else */
         net::finalize_inbound_line( "(" + line );
   }
}

void system_speak( const string& message )
{
   if (g_silent_count <= 0)
   {
      string prefixed = "<b>[<i>f</i>]</b> ";
      prefixed.append( message );
      strip_lines( &prefixed );
      push_chatbox( prefixed );
   }
}

void system_errspeak( const string& message )
{
   if (g_silent_count <= 0)
   {
      string prefixed = "<b>[<i>f</i>]</b> ";
      prefixed.append( message );
      strip_lines( &prefixed );
      push_chatbox( prefixed );
   }

   log_error( "%s", message.c_str( ) );
}

void event_tick( )
{
   unsigned int dt = GetTickCount( ) - g_last_tick;
   g_last_tick += dt;

   // If the system clock gets adjusted, you could get some crazy values here,
   // so keep the max delta to 5 minutes.
   if (dt >= 5 * 60 * 1000)   // 5 mins
      dt = 5 * 60 * 1000;

   scripts::on_tick( dt );
   modules::on_tick( dt );
   skin::on_tick( dt );
   world::on_tick( dt );
   news::on_tick( dt );

   if (g_login_spam_cooldown && g_login_spam_cooldown <= dt)
   {
      // Do login spam.
      if (!memlocs::valid( ))
      {
        /* system_errspeak( "<font color='error'>"
                          "There is no memlocs file that matches this build of the Furcadia client."
                          " Please make sure you have the latest versions of Furnarchy and Furcadia."
                          " Some functionality will be disabled for stability."
                          "</font>" ); */
	     core::log_error( "There is no memlocs file that matches this build of the Furcadia client.", NULL );

      }

      g_login_spam_cooldown = 0;
   }
   else
      g_login_spam_cooldown -= dt;
}

void event_inbound( const string& _line, int from, size_t num )
{
   /* Ignores -> iFilters -> Scripts -> Modules -> World -> Client */

   string line = _line;

   long hide_flags = 0;

   /* Ignores. */
   if (memlocs::valid( ))
   {
      if (client::ignore_line_test( line.c_str( ) ))
      {
         hide_flags = HIDE_ALL;
         world::badge_cancel( );
      }
   }

   /* iFilters. */
   if (!(hide_flags & HIDE_FILTERS))
      hide_flags |= ifilters::on_inbound_line( &line, from, num );

   /* Scripts. */
   if (!(hide_flags & HIDE_SCRIPTS))
      hide_flags |= scripts::on_inbound_line( &line, from, num );

   /* Modules. */
   if (!(hide_flags & HIDE_MODULES))
      hide_flags |= modules::on_inbound_line( &line, from, num );

   /* Scripts (post). */
   if (!(hide_flags & HIDE_SCRIPTS))
      hide_flags |= scripts::on_inbound_line_late( &line, from, num );

   /* World. */
   if (!(hide_flags & HIDE_WORLD))
      hide_flags |= world::on_inbound_line( &line, from, num );

   if (!(hide_flags & HIDE_CLIENT))
      net::finalize_inbound_line( line );
}

void event_outbound( const string& _line, int from, size_t num )
{
   /* oFilters -> Scripts -> Modules -> World -> Client */

   string line = _line;

   /* If memlocs are down, 'client' won't send us
    push_command( )s, so watch for @ commands from Net. */

   if (/*!memlocs::valid( ) && */from == FROM_CLIENT)
   {
      using namespace yiffurc;
      const C2S* c2s = world::parse_outbound( line );

      if (c2s)
      {
         if (c2s->type == C2S_TYPE_SPEECH)
         {
            const char* const msg = ((C2S_SPEECH*) c2s)->message;
            const string& cmdpfx = options::get_command_prefix( );
            if (cmdpfx.compare( 0, cmdpfx.length( ), msg, cmdpfx.length( ) ) == 0)
            {
               // Double prefixed?
               if (cmdpfx.compare( 0, cmdpfx.length( ), msg + cmdpfx.length( ), cmdpfx.length( ) ) == 0)
               {
                  // Double prefix translates into a spoken prefix, not a command.
                  line = "\"";
                  line += (msg + cmdpfx.length( ));
               }
               else
               {
                  push_command( msg + cmdpfx.length( ) );
                  line.clear( );
               }
            }
         }
         yiffurc_release( c2s );
      }
   }

   long hide_flags = 0;
   
   /* oFilters. */
   if (!(hide_flags & HIDE_FILTERS))
      hide_flags |= ofilters::on_outbound_line( &line, from, num );

   /* Scripts. */
   if (!(hide_flags & HIDE_SCRIPTS))
      hide_flags |= scripts::on_outbound_line( &line, from, num );
   
   /* Modules */
   if (!(hide_flags & HIDE_MODULES))
      hide_flags |= modules::on_outbound_line( &line, from, num );

   /* Scripts (post). */
   if (!(hide_flags & HIDE_SCRIPTS))
      hide_flags |= scripts::on_outbound_line_late( &line, from, num );

   /* World. */
   if (!(hide_flags & HIDE_WORLD))
      hide_flags |= world::on_outbound_line( &line, from, num );

   if (!(hide_flags & HIDE_CLIENT))
      net::finalize_outbound_line( line );
}

void push_command( const string& command, bool noecho )
{
   // Just queue the command.
   g_commands.push( F2COMMAND( command, noecho ) );
   batch_add( BATCH_CORE );
}

void broadcast_commands( )
{
   while (!g_commands.empty( ))
   {
      const F2COMMAND cmd = g_commands.front( );   // Queue may get modified.
      g_commands.pop( );

      /* Echo the command. */
      if (!cmd.noecho && g_connlvl >= CONNLVL_LOGGEDIN)
      {
         string echo = options::get_command_prefix( );
         echo += cmd.cmd;
         echo = "<i>" + break_html( break_ent_refs( echo ) ) + "</i>";
         push_chatbox( echo );
      }

      /* Build an arglist. */
      static ArgList arg_list;

      arg_list.clear( );
      make_arg_list( cmd.cmd, &arg_list );

      if (arg_list.size( ))
      {
         /* Send it to the various subsystems. */
         bool handled = false;

         handled |= core::on_command( arg_list );
         handled |= ifilters::on_command( arg_list );
         handled |= ofilters::on_command( arg_list );
         handled |= scripts::on_command( arg_list );
         handled |= client::on_command( arg_list );
         handled |= window::on_command( arg_list );
         handled |= skin::on_command( arg_list );
         handled |= news::on_command( arg_list );
         handled |= world::on_command( arg_list );
         handled |= modules::on_command( arg_list );

         if (!handled)
            system_errspeak( "Error: Invalid command." );
      }
   }
}

void event_screen_load( Client_Screen screen )
{
   g_screen = screen;
   net::on_screen_load( screen );
   modules::on_screen_load( screen );
}

void event_render( HDC dc, unsigned int width, unsigned int height )
{
   modules::on_render( dc, width, height );

   /* Skin always renders last. */
   skin::on_render( dc, width, height );
}

bool event_wndproc( WNDPROCPARAMS* params )
{
   /* Skin. */
   if (skin::on_wndproc( params ))
      return true;

   /* Scripts. */
   if (scripts::on_wndproc( params ))
      return true;

   /* Modules. */
   if (modules::on_wndproc( params ))
      return true;

   return false;
}

void event_window_created( HWND wnd )
{
   /* Source the init file. */
   source( "init" );

   /* Modules. */
   modules::on_window_created( wnd );
}

void event_modstate( unsigned int mod, long state )
{
   skin::on_modstate( mod, state );
}

void event_batch( Batch_Subsystem subsystem )
{
   switch (subsystem)
   {
   case BATCH_CORE:
      core::on_batch( );
      break;
   case BATCH_NET:
      net::on_batch( );
      break;
   }
}

void event_set_main_socket( SOCKET socket )
{
   modules::on_set_main_socket( socket );
}

bool source( const string& name )
{
   // Look for the source file in the per-user location first.
   string source_file = g_furn_doc_root + "\\source\\" + name + ".txt";
   if (GetFileAttributes( source_file.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
      // Fall back to program location.
      source_file = g_furn_root + "\\source\\" + name + ".txt";

   ifstream is( source_file.c_str( ) );

   if (!is.fail( ))
   {
      // silence( true );

      string line;

      while (!is.eof( ) && !is.fail( ))
      {
         getline( is, line );

         if (line.length( ))
            push_command( line, true );
      }

      // silence( false );
      is.close( );
   }
   else
   {
      system_errspeak( "Unable to source file " + source_file );
      return false;
   }

   system_speak( "Successfully sourced file " + source_file );

   return true;
}

HINSTANCE get_core_inst( ) {
   return g_my_instance;
}

const string& get_furn_root( ) {
   return g_furn_root;
}

const string& get_furn_data_root( ) {
   return g_furn_data_root;
}

const string& get_furn_doc_root( ) {
   return g_furn_doc_root;
}

const string& get_client_root( ) {
   return g_client_root;
}

const string& get_client_data_root( ) {
   return g_client_data_root;
}

const string& get_client_doc_root( ) {
   return g_client_doc_root;

}

time_t get_start_time( ) {
   return g_time_started;
}

void silence( bool onoff )
{
   if (onoff)
      ++g_silent_count;
   else
      --g_silent_count;
}

void toggle_console( bool onoff )
{
   if (onoff)
   {
      if (AllocConsole( ))
      {
         freopen( "CONOUT$", "wb", stdout );
         freopen( "CONOUT$", "wb", stderr );
         std::cout.clear( );
         std::cerr.clear( );
      }
      system_speak( "Console opened." );
   }
   else
   {
      fclose( stdout );
      fclose( stderr );

      FreeConsole( );
      system_speak( "Console closed." );
   }
}

bool on_command( const ArgList& args )
{
   /* @source command? */
   if (!_stricmp( args[ 0 ].c_str( ), "source" ))
   {
      if (args.size( ) > 2)
         system_errspeak( "Error: Too many arguments for @source <file>" );
      else if (args.size( ) == 1)
         system_errspeak( "Expected a file to source." );
      else
         source( args[ 1 ] );

      return true;
   }
   /* @console command? */
   else if (!_stricmp( args[ 0 ].c_str( ), "console" ))
   {
      if (args.size( ) > 2)
         system_errspeak( "Error: Too many arguments for @console on|off" );
      else if (args.size( ) == 1)
         system_speak( "Expected either 'on' or 'off'." );
      else
      {
         if (args[ 1 ] == "on")
            toggle_console( true );
         else if (args[ 1 ] == "off")
            toggle_console( false );
         else
            system_errspeak( "Expected either 'on' or 'off'." );
      }

      return true;
   }
   /* @echo command? */
   else if (!_stricmp( args[ 0 ].c_str( ), "echo" ))
   {
      if (args.size( ) > 1)
      {
         string s = args[ 1 ];
         for (size_t i = 2; i < args.size( ); ++i)
         {
            s += " ";
            s += args[ i ];
         }

         system_speak( s );
      }

      return true;
   }
   /* @version command? */
   else if (!_stricmp( args[ 0 ].c_str( ), "version" ))
   {
      system_speak( "Furnarchy " F2VER_SZ );
      return true;
   }


   return false;
}

inline void on_batch( )
{
   broadcast_commands( );
}

void batch_add( Batch_Subsystem subsystem )
{
   window::post_message( window::WM_BATCHMSG, (WPARAM) subsystem, 0 );
}

Connection_Level get_connlvl( )
{
   return g_connlvl;
}

bool create_folder_path( const string& path )
{
   if (GetFileAttributes( path.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
   {
      /* Folder(s) need to be created. */

      // Break up the different folders in the path.
      queue<string> folders;
      for (string::size_type off = 0; off < path.length( ); ++off )
      {
         const string::size_type end = path.find_first_of( "\\", off );
         if (path.npos == end)
         {
            folders.push( path.substr( off, path.npos ) );
            break;
         }
         folders.push( path.substr( off, end - off ) );
         off = end;
      }

      // Start creating the folder path.
      if (!folders.empty( ))
      {
         string curr_path;
         do
         {
            if (!curr_path.empty( ))
               curr_path += "\\";
            curr_path += folders.front( );
            folders.pop( );

            if (GetFileAttributes( curr_path.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
            {
               // Need to create this folder.
               if (!CreateDirectory( curr_path.c_str( ), NULL ))
               {
                  if (GetLastError( ) != ERROR_ALREADY_EXISTS)
                  {
                     log_error( "Unable to create folder path up to %s.", curr_path.c_str( ) );
                     return false;
                  }
               }
            }

         } while (!folders.empty( ));
      }
   }
   return true;
}

void event_connected( )
{
   if (g_connlvl != CONNLVL_CONNECTED)
   {
      g_connlvl = CONNLVL_CONNECTED;

      scripts::on_connlvl( g_connlvl );
      modules::on_connlvl( g_connlvl );
      world::on_connlvl( g_connlvl );
   }
}

void event_disconnected( )
{
   if (g_connlvl != CONNLVL_DISCONNECTED)
   {
      g_connlvl = CONNLVL_DISCONNECTED;

      scripts::on_connlvl( g_connlvl );
      modules::on_connlvl( g_connlvl );
      world::on_connlvl( g_connlvl );
   }
}

void event_loggedin( )
{
   if (g_connlvl != CONNLVL_LOGGEDIN)
   {
      g_connlvl = CONNLVL_LOGGEDIN;

      scripts::on_connlvl( g_connlvl );
      modules::on_connlvl( g_connlvl );
      world::on_connlvl( g_connlvl );

      /* Source the login file. */
      source( "login" );

      // Set the login spam cooldown.
      g_login_spam_cooldown = 2000;
   }
}

void event_motdend( )
{
   if (g_connlvl != CONNLVL_MOTDEND)
   {
      g_connlvl = CONNLVL_MOTDEND;

      scripts::on_connlvl( g_connlvl );
      modules::on_connlvl( g_connlvl );
      world::on_connlvl( g_connlvl );
   }
}

void event_player_info( )
{
   modules::on_player_info( );
}

bool event_chatbox( const string& line )
{
   return modules::on_chatbox( line );
}

void event_monster( unsigned guid )
{
   return modules::on_monster( guid );
}

void event_script( )
{
   return modules::on_script( );
}

} // namespace core