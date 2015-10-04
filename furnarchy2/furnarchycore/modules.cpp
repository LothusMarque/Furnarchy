#include "modules.h"
#include "core.h"
#include "net.h"
#include "options.h"
#include "client.h"
#include "common/IniFile.h"
#include <vector>
#include <stack>
#include <sstream>
#include <algorithm>
#include <ShlObj.h>

namespace modules {

using std::stack;
using furn2::Revision;
using furn2::MODULEINFO;
using furn2::RENDERPARAMS;
using furn2::Module_Event;
using furn2::Client_Event;
using furn2::World_Event;

using furn2::REV_DRAFT; using furn2::REV_REVISING; using furn2::REV_FINAL;
using furn2::MOD_EVENT_START; using furn2::MOD_EVENT_STOP; using furn2::MOD_EVENT_ENABLE;
using furn2::MOD_EVENT_DISABLE; using furn2::MOD_EVENT_TOUCH; using furn2::MOD_EVENT_MSG;

using furn2::CL_EVENT_WNDCREATED; using furn2::CL_EVENT_SCRLOAD; using furn2::CL_EVENT_SETMAINSOCKET;
using furn2::CL_EVENT_PRERENDER; using furn2::CL_EVENT_RENDER; using furn2::CL_EVENT_POSTRENDER;
using furn2::CL_EVENT_TICK; using furn2::CL_EVENT_CHATBOX; using furn2::CL_EVENT_PLAYERMENUSHOW;
using furn2::CL_EVENT_SCRIPT;

using furn2::WO_EVENT_CONNECTED; using furn2::WO_EVENT_DISCONNECTED; using furn2::WO_EVENT_LOGGEDIN;
using furn2::WO_EVENT_PLAYERINFO; using furn2::WO_EVENT_MONSTER;

using furn2::MOD_DISABLED; using furn2::MOD_ENABLED;

//////////////////////////////////////////////////////////////////////////

struct ADAPTER
{
   typedef bool(CDECL *PFN_CREATE)( unsigned int mod, const char* root, long flags );
   typedef bool(CDECL *PFN_DESTROY)( unsigned int mod );
   typedef void(CDECL *PFN_ON_INET)( unsigned int mod, Revision rev, 
                                     const char* line, int from, size_t num );
   typedef void(CDECL *PFN_ON_ONET)( unsigned int mod, Revision rev, 
                                     const char* line, int from, size_t num );
   typedef intptr_t(CDECL *PFN_ON_MODULE)( unsigned int mod, Module_Event e, intptr_t param );
   typedef intptr_t(CDECL *PFN_ON_CLIENT)( unsigned int mod, Client_Event e, intptr_t param );
   typedef bool(CDECL *PFN_ON_WNDPROC)( unsigned int mod, WNDPROCPARAMS* params );
   typedef intptr_t(CDECL *PFN_ON_WORLD)( unsigned int mod, World_Event e, intptr_t data );
   typedef bool(CDECL *PFN_ON_COMMAND)( unsigned int mod, int argc, char* argv[] );
   
   HMODULE        lib;
   string         name;
   PFN_CREATE     create;
   PFN_DESTROY    destroy;
   PFN_ON_INET    on_inet;
   PFN_ON_ONET    on_onet;
   PFN_ON_MODULE  on_module;
   PFN_ON_CLIENT  on_client;
   PFN_ON_WNDPROC on_wndproc;
   PFN_ON_WORLD   on_world;
   PFN_ON_COMMAND on_command;

   ADAPTER( ) : create( 0 ), destroy( 0 ), on_inet( 0 ), on_onet( 0 ),
                on_module( 0 ), on_client( 0 ), on_wndproc( 0 ), on_command( 0 ), 
                on_world( 0 ) { }
};

//////////////////////////////////////////////////////////////////////////

struct MODULE
{
   ADAPTER*      adapter;
   unsigned int  mod;
   UUID128       uuid;
   long          state;
   string        name;
   string        fancy_name;
   string        root;
   string        data_root;
   string        doc_root;
   string        thumb_file;
   HMENU         menu;
};

//////////////////////////////////////////////////////////////////////////

bool             g_initialized = false;
vector<ADAPTER*> g_adapters;
vector<MODULE*>  g_modules;
string*          g_curr_line;
long             g_curr_hide;
Revision         g_curr_revision;

//////////////////////////////////////////////////////////////////////////

bool     adapter_exists( const string& name );
bool     load_adapter( const string& name );
ADAPTER* find_adapter( const string& name );
void     free_adapters( );
bool     load_modules( );
MODULE*  construct_module( const string& dir );
void     free_modules( );
void     start_modules( );
void     stop_modules( );
void     cmd_modls( const ArgList& args );
void     cmd_modtouch( const ArgList& args );
void     cmd_modon( const ArgList& args );
void     cmd_modoff( const ArgList& args );
unsigned int  find_module( const string& name );

bool initialize( )
{
   if (!g_initialized)
   {
      g_initialized = true;
      g_curr_line = NULL;
      g_curr_revision = REV_DRAFT;

      load_modules( );
      start_modules( );
   }

   return true;
}

bool uninitialize( )
{
   if (g_initialized)
   {
      g_initialized = false;
      g_curr_line = NULL;
      g_curr_revision = REV_DRAFT;

      stop_modules( );
      free_modules( );
      free_adapters( );
   }

   return true;
}

bool adapter_exists( const string& name )
{
   ostringstream ss;
   ss << core::get_furn_root( ) << "\\adapters\\" << name << ".dll";
   DWORD atts = GetFileAttributes( ss.str( ).c_str( ) );
   return atts != INVALID_FILE_ATTRIBUTES;
}

bool load_adapter( const string& name )
{
   // Make sure it isn't already loaded.
   if (find_adapter( name ))
      return true;

   ostringstream ss;
   ss << core::get_furn_root( ) << "\\adapters\\" << name << ".dll";
   string filename = ss.str( );

   HMODULE h_lib = NULL;
   {
      DirStack cd;
      cd.push( core::get_furn_root( ) + "\\adapters" );
      h_lib = LoadLibrary( filename.c_str( ) );
   }

   if (!h_lib)
   {
      ostringstream ss;
      ss << "Couldn't load adapter '" << name
         << "' (ErrCode: " << GetLastError( ) << ")";
      core::system_errspeak( ss.str( ) );
      return false;
   }

   ADAPTER* adapter = new ADAPTER;
   adapter->lib  = h_lib;
   adapter->name = name;

   adapter->create  = (ADAPTER::PFN_CREATE) 
                        GetProcAddress( h_lib, "create" );
   adapter->destroy = (ADAPTER::PFN_DESTROY) 
                        GetProcAddress( h_lib, "destroy" );
   adapter->on_inet = (ADAPTER::PFN_ON_INET)
                        GetProcAddress( h_lib, "on_inet" );
   adapter->on_onet = (ADAPTER::PFN_ON_ONET)
                        GetProcAddress( h_lib, "on_onet" );
   adapter->on_client = (ADAPTER::PFN_ON_CLIENT)
                        GetProcAddress( h_lib, "on_client" );
   adapter->on_wndproc = (ADAPTER::PFN_ON_WNDPROC)
                        GetProcAddress( h_lib, "on_wndproc" );
   adapter->on_module = (ADAPTER::PFN_ON_MODULE)
                        GetProcAddress( h_lib, "on_module" );
   adapter->on_world = (ADAPTER::PFN_ON_WORLD)
                        GetProcAddress( h_lib, "on_world" );
   adapter->on_command = (ADAPTER::PFN_ON_COMMAND)
                        GetProcAddress( h_lib, "on_command" );

   if (!adapter->create || !adapter->destroy || !adapter->on_inet
       || !adapter->on_client || !adapter->on_wndproc 
       || !adapter->on_module || !adapter->on_command
       || !adapter->on_world)
   {
      ostringstream ss;
      ss << "Adapter '" << name << "' is missing required exports.";
      core::system_errspeak( ss.str( ) );
      delete adapter;
      FreeLibrary( h_lib );
      return false;
   }

   g_adapters.push_back( adapter );
   return true;
}

ADAPTER* find_adapter( const string& name )
{
   for (size_t i = 0; i < g_adapters.size( ); ++i)
   {
      if (!_stricmp( g_adapters[ i ]->name.c_str( ), name.c_str( ) ))
         return g_adapters[ i ];
   }
   return NULL;
}

void free_adapters( )
{
   for (size_t i = 0; i < g_adapters.size( ); i++)
      delete g_adapters[ i ];
   g_adapters.clear( );
}

bool load_modules( )
{
   const list<string>& masked  = options::get_modules_masked( );
   const list<string>& accepted = options::get_modules_accepted( );

   // If a wildcard character is masked, don't load any modules.
   if (find( masked.begin( ), masked.end( ), string( "*" ) ) != masked.end( ))
   {
      core::log_error( "All modules prevented from loading by wildcard mask." );
      return true;
   }

   /* Keep track of modules we've loaded so far. */
   vector<string> loaded;
   
   /* Load up the accepted modules first. */
   for (list<string>::const_iterator i = accepted.begin( ); i != accepted.end( ); ++i)
   {      
      /* Make sure it hasn't already been loaded. */
      {
         vector<string>::const_iterator j = loaded.begin( );
         for (; j != loaded.end( ); ++j)
            if (!_stricmp( i->c_str( ), j->c_str( ) ))
               break;

         if (j != loaded.end( ))
            continue; // Already loaded.
      }

      /* Make sure it isn't masked. */
      {
         list<string>::const_iterator j = masked.begin( );
         for (; j != masked.end( ); ++j)
            if (!_stricmp( i->c_str( ), j->c_str( ) ))
               break;

         if (j != masked.end( ))
            continue; // Masked.
      }

      /* OK, create it. */
      MODULE* module = construct_module( *i );
      if (module)
      {
         if (module->adapter->create( g_modules.size( ), module->root.c_str( ), 0 ))
         {
            loaded.push_back( module->name );
            g_modules.push_back( module );
         }
         else
            delete module;
      }
   }

   /* Load up any other modules you can find. */

   string globstr = core::get_furn_root( ) + "\\modules\\*";

   WIN32_FIND_DATA find_data;   
   HANDLE h_find = FindFirstFile( globstr.c_str( ), &find_data );

   if (h_find != INVALID_HANDLE_VALUE)
   {
      do
      {
         if (find_data.cFileName[ 0 ] != '.')
         {
            string dir = find_data.cFileName;

            /* Make sure it hasn't already been loaded. */
            {
               vector<string>::const_iterator j = loaded.begin( );
               for (; j != loaded.end( ); ++j)
                  if (!_stricmp( dir.c_str( ), j->c_str( ) ))
                     break;

               if (j != loaded.end( ))
                  continue; // Already loaded.
            }

            /* Make sure it isn't masked. */
            {
               list<string>::const_iterator j = masked.begin( );
               for (; j != masked.end( ); ++j)
                  if (!_stricmp( dir.c_str( ), j->c_str( ) ))
                     break;

               if (j != masked.end( ))
                  continue; // Masked.
            }

            /* OK, create it. */
            MODULE* module = construct_module( dir );
            if (module)
            {
               if (module->adapter->create( g_modules.size( ), module->root.c_str( ), 0 ))
               {
                  loaded.push_back( module->name );
                  g_modules.push_back( module );
               }
               else
                  delete module;
            }
         }
      } while (FindNextFile( h_find, &find_data ));

      FindClose( h_find );
   }

   return true;
}

MODULE* construct_module( const string& dir )
{
   string file = core::get_furn_root( );
   file += "\\modules\\";
   file += dir;
   
   DWORD file_atts = GetFileAttributes( file.c_str( ) );
   if (file_atts != INVALID_FILE_ATTRIBUTES && file_atts & FILE_ATTRIBUTE_DIRECTORY)
   {
      MODULE* module = new MODULE;
      module->name  = dir;
      module->mod   = g_modules.size( );   // Next available unsigned int.
      module->state = MOD_DISABLED;
      module->menu  = CreatePopupMenu( );

      /* Get folder paths. */
      module->root  = file;   // 'file' is just the program directory right now.
      {
         if (options::get_single_user_mode( ))
         {
            module->data_root = core::get_furn_root( ) + "\\modules\\" + dir;
            module->doc_root = core::get_furn_doc_root( ) + "\\modules\\" + dir;
         }
         else
         {
            module->data_root = core::get_furn_data_root( ) + "\\module-data\\" + dir;
            module->doc_root = core::get_furn_doc_root( ) + "\\module-docs\\" + dir;
         }         
      }
      
      /* Load up the module.ini. */
      file += "\\module.ini";

      string adapter_name;

      IniFile module_ini( file );
      if (module_ini.open( ))
      {
         module->fancy_name = module_ini.read( "name" );
         module->uuid       = module_ini.read( "uuid" ).c_str( );
         module->thumb_file = module_ini.read( "thumb" );
         adapter_name       = module_ini.read( "adapter" );
      }

      if (!module->fancy_name.length( ))  // Default to dir name.
         module->fancy_name = module->name;

      if (!module->thumb_file.length( )) // Default to 'thumbnail.bmp'
         module->thumb_file = "thumbnail.bmp";

      if (!adapter_name.length( )) // Default to 'native'
         adapter_name = "native"; 

      /* Make sure the adapter exists. */
      load_adapter( adapter_name );
      if (!(module->adapter = find_adapter( adapter_name )))
      {
         ostringstream ss;
         ss << "The adapter '" << adapter_name << "' for module '"
            << module->name << "' does not exist or isn't loaded.";

         core::system_errspeak( ss.str( ) );
         delete module;
      }
      else
      {
         // Looks good.

         // Make sure the data and documents folders for the module exist.
         if (!core::create_folder_path( module->data_root ))
            core::log_error( "Failed to create data folder for module '%s'\n", module->name.c_str( ) );
         if (!core::create_folder_path( module->doc_root ))
            core::log_error( "Failed to create documents folder for module '%s'\n", module->name.c_str( ) );

         return module;
      }
   }

   return 0;
}

void start_modules( )
{
   MODULEINFO info;
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];

      info.mod        = module->mod;
      info.uuid       = module->uuid;
      info.state      = module->state;
      info.name       = module->name.c_str( );
      info.fancy_name = module->fancy_name.c_str( );
      info.root       = module->root.c_str( );
      info.thumb_file = module->thumb_file.c_str( );

      module->adapter->on_module( module->mod, MOD_EVENT_START, (intptr_t) &info );
   }
}

void stop_modules( )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      module->adapter->on_module( module->mod, MOD_EVENT_STOP, 0 );
   }
}

void free_modules( )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
      delete g_modules[ i ];
   
   g_modules.clear( );
}

long on_inbound_line( string* line, int from, size_t num )
{
   g_curr_line = line;
   g_curr_hide = 0;
   
   g_curr_revision = REV_DRAFT;
   for (vector<MODULE*>::iterator i = g_modules.begin( ); !(g_curr_hide & HIDE_MODULES) && i != g_modules.end( ); ++i)
   {
      (*i)->adapter->on_inet( (*i)->mod, g_curr_revision, g_curr_line->c_str( ), from, num );
   }

   g_curr_revision = REV_REVISING;
   for (vector<MODULE*>::iterator i = g_modules.begin( ); !(g_curr_hide & HIDE_MODULES) && i != g_modules.end( ); ++i)
   {
      (*i)->adapter->on_inet( (*i)->mod, g_curr_revision, g_curr_line->c_str( ), from, num );
   }

   g_curr_revision = REV_FINAL;
   for (vector<MODULE*>::iterator i = g_modules.begin( ); !(g_curr_hide & HIDE_MODULES) && i != g_modules.end( ); ++i)
   {
      (*i)->adapter->on_inet( (*i)->mod, g_curr_revision, g_curr_line->c_str( ), from, num );
   }

   g_curr_line = NULL;
   return g_curr_hide;
}

long on_outbound_line( string* line, int from, size_t num )
{   
   g_curr_line = line;
   g_curr_hide = 0;

   g_curr_revision = REV_DRAFT;
   for (vector<MODULE*>::iterator i = g_modules.begin( ); !(g_curr_hide & HIDE_MODULES) && i != g_modules.end( ); ++i)
   {
      (*i)->adapter->on_onet( (*i)->mod, g_curr_revision, g_curr_line->c_str( ), from, num );
   }

   g_curr_revision = REV_REVISING;
   for (vector<MODULE*>::iterator i = g_modules.begin( ); !(g_curr_hide & HIDE_MODULES) && i != g_modules.end( ); ++i)
   {
      (*i)->adapter->on_onet( (*i)->mod, g_curr_revision, g_curr_line->c_str( ), from, num );
   }

   g_curr_revision = REV_FINAL;
   for (vector<MODULE*>::iterator i = g_modules.begin( ); !(g_curr_hide & HIDE_MODULES) && i != g_modules.end( ); ++i)
   {
      (*i)->adapter->on_onet( (*i)->mod, g_curr_revision, g_curr_line->c_str( ), from, num );
   }

   g_curr_line = NULL;
   return g_curr_hide;
}

bool on_command( const ArgList& args )
{
   string arg = args[ 0 ];
   to_lower( &arg );
   bool handled = false;

   if (arg == "modls")
   {
      cmd_modls( args );
      handled = true;
   }
   else if (arg == "modtouch")
   {
      cmd_modtouch( args );
      handled = true;
   }
   else if (arg == "modon")
   {
      cmd_modon( args );
      handled = true;
   }
   else if (arg == "modoff")
   {
      cmd_modoff( args );
      handled = true;
   }

   /* Make into c-style args. */
   char** argv = new char*[ args.size( ) ];

   for (size_t i = 0; i < args.size( ); i++)
      argv[ i ] = _strdup( args[ i ].c_str( ) );
   
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      handled |= module->adapter->on_command( module->mod, args.size( ), argv );
   }

   for (size_t i = 0; i < args.size( ); i++)
      free( argv[ i ] );

   delete [] argv;

   return handled;
}

void on_connlvl( Connection_Level lvl )
{
   World_Event e;

   switch (lvl)
   {
   default:
      return;
   case CONNLVL_CONNECTED:
      e = WO_EVENT_CONNECTED; break;
   case CONNLVL_DISCONNECTED:
      e = WO_EVENT_DISCONNECTED; break;
   case CONNLVL_LOGGEDIN:
      e = WO_EVENT_LOGGEDIN; break;      
   }

   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      module->adapter->on_world( module->mod, e, 0 );
   }
}

void on_monster( unsigned guid )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      module->adapter->on_world( module->mod, WO_EVENT_MONSTER, (intptr_t) guid );
   }
}

void on_window_created( HWND wnd )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      module->adapter->on_client( module->mod, CL_EVENT_WNDCREATED, (intptr_t) wnd );
   }   
}

void on_screen_load( Client_Screen screen )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      module->adapter->on_client( module->mod, CL_EVENT_SCRLOAD, (intptr_t) screen );
   }
}   

void on_tick( unsigned int dt )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      module->adapter->on_client( module->mod, CL_EVENT_TICK, (intptr_t) dt );
   }
}

void on_render( HDC dc, unsigned int width, unsigned int height )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      RENDERPARAMS param = { dc, width, height };
      module->adapter->on_client( module->mod, CL_EVENT_PRERENDER, (intptr_t) &param );
   }

   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      RENDERPARAMS param = { dc, width, height };
      module->adapter->on_client( module->mod, CL_EVENT_RENDER, (intptr_t) &param );
   }

   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      RENDERPARAMS param = { dc, width, height };
      module->adapter->on_client( module->mod, CL_EVENT_POSTRENDER, (intptr_t) &param );
   }
}

bool on_wndproc( WNDPROCPARAMS* params )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      if (module->adapter->on_wndproc( module->mod, params ))
         return true;
   }

   return false;
}

void on_set_main_socket( SOCKET socket )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      module->adapter->on_client( module->mod, CL_EVENT_SETMAINSOCKET, (intptr_t) socket );
   }
}

void on_player_info( )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      module->adapter->on_world( module->mod, WO_EVENT_PLAYERINFO, NULL );
   }
}

bool on_chatbox( const string& line )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      if (module->adapter->on_client( module->mod, CL_EVENT_CHATBOX, (intptr_t) line.c_str( ) ))
         return true;
   }

   return false;
}

void playermenu_populate( )
{
   HMENU menu = client::playermenu_menu( );
   if (menu)
   {
      assert( GetMenuItemCount( menu ) > 0 );

      MENUITEMINFO mii;
      memset( &mii, 0, sizeof( mii ) );
      mii.cbSize = sizeof( mii );

      /* Remove all the stuff we added. */
      {
         unsigned n = GetMenuItemCount( menu );
         mii.fMask = MIIM_DATA;
         for (unsigned i = 0; i < n; ++i)
         {
            GetMenuItemInfo( menu, i, TRUE, &mii );
            if (mii.dwItemData == 1337)
            {
               RemoveMenu( menu, i, MF_BYPOSITION );
               --n; --i;
            }
         }
      }      

      mii.fMask      = MIIM_FTYPE | MIIM_STRING | MIIM_SUBMENU | MIIM_STRING | MIIM_DATA;
      mii.fType      = MFT_STRING;
      mii.dwItemData = 1337;
      bool added = false;

      for (vector<MODULE*>::const_iterator i = g_modules.begin( ); i != g_modules.end( ); ++i)
      {
         if ((*i)->menu && GetMenuItemCount( (*i)->menu ) > 0)
         {
            mii.dwTypeData = const_cast<char*>( (*i)->fancy_name.c_str( ) );
            mii.hSubMenu = (*i)->menu;
            InsertMenuItem( menu, GetMenuItemCount( menu ) - 1, TRUE, &mii );
            added = true;
         }
      }

      if (added)
      {
         /* Insert a separator before the shortname. */
         mii.fMask  = MIIM_TYPE | MIIM_DATA;
         mii.fType  = MFT_SEPARATOR;
         InsertMenuItem( menu, GetMenuItemCount( menu ) - 1, TRUE, &mii );
      }
   }
}

void on_playermenu_created( )
{
   HMENU menu = client::playermenu_menu( );
   assert( menu );

   // Populate with module menus.
   playermenu_populate( );
}

void on_playermenu_show( const string& name )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      module->adapter->on_client( module->mod, CL_EVENT_PLAYERMENUSHOW, (intptr_t) name.c_str( ) );
   }
}

void on_script( )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      module->adapter->on_client( module->mod, CL_EVENT_SCRIPT, (intptr_t) NULL );
   }
}

WORD playermenu_find_unique_id( )
{
   WORD id = 1024; // Client menu IDs are well below this number.
   for (vector<MODULE*>::const_iterator i = g_modules.begin( ); i != g_modules.end( ); ++i)
   {
      const int count = GetMenuItemCount( (*i)->menu );
      for (int j = 0; j < count; ++j)
      {
         if ((WORD) GetMenuItemID( (*i)->menu, j ) == id)
            id = (WORD) GetMenuItemID( (*i)->menu, j ) + 1;
      }
   }
   return id;
}

bool playermenu_set( unsigned int mod, HMENU menu )
{   
   if (mod < g_modules.size( ))
   {
      g_modules[ mod ]->menu = menu;
      playermenu_populate( );
      return true;
   }
   return false;
}

bool playermenu_unset( unsigned int mod )
{
   if (mod < g_modules.size( ))
   {
      g_modules[ mod ]->menu = NULL;
      playermenu_populate( );
      return true;
   }
   return false;
}

void enable_module( unsigned int mod, bool onoff )
{
   if (mod < g_modules.size( ))
   {
      MODULE* module = g_modules[ mod ];

      if (onoff)
      {
         if (!(get_module_state( mod ) & MOD_ENABLED))
         {
            module->state = MOD_ENABLED;
            module->adapter->on_module( mod, MOD_EVENT_ENABLE, 0 );
            core::event_modstate( mod, module->state );
         }
      }
      else
      {
         if (get_module_state( mod ) & MOD_ENABLED)
         {
            module->state = MOD_DISABLED;
            module->adapter->on_module( mod, MOD_EVENT_DISABLE, 0 );
            core::event_modstate( mod, module->state );
         }
      }
   }
}

void touch_module( unsigned int mod )
{
   if (mod < g_modules.size( ))
   {
      MODULE* module = g_modules[ mod ];

      module->adapter->on_module( mod, MOD_EVENT_TOUCH, 0 );
   }
}

intptr_t msg_module( unsigned int mod, void* data )
{
   if (mod < g_modules.size( ))
   {
      MODULE* module = g_modules[ mod ];

      return module->adapter->on_module( mod, MOD_EVENT_MSG, (intptr_t) data );
   }

   return 0;
}

void revise_line( const char* line )
{
   if (g_curr_line)
   {
      switch (g_curr_revision)
      {
      case REV_DRAFT:
         // Modules may only cancel a line.
         if (!line || !*line)
            g_curr_line->clear( );
         break;
         
      case REV_REVISING:
         // Modules may completely modify a line.
         if (line)
            g_curr_line->assign( line );
         break;

      case REV_FINAL:
         // Modules may only cancel a line.
         if (!line || !*line)
            g_curr_line->clear( );
         break;
      }
   }
}

void hide_line( long flags )
{
   // Meaningless unless in the middle of an outbound/inbound event.
   g_curr_hide |= flags;
}

unsigned long get_hide_flags( )
{
   return g_curr_hide;
}

size_t get_num_modules( )
{
   return g_modules.size( );
}

long get_module_state( unsigned int mod )
{
   return g_modules[ mod ]->state;
}

const string& get_module_name( unsigned int mod )
{
   return g_modules[ mod ]->name;
}

const string& get_module_fancy_name( unsigned int mod )
{
   return g_modules[ mod ]->fancy_name;
}

const string& get_module_root( unsigned int mod )
{
   return g_modules[ mod ]->root;
}

const string& get_module_data_root( unsigned int mod )
{
   return g_modules[ mod ]->data_root;
}

const string& get_module_doc_root( unsigned int mod )
{
   return g_modules[ mod ]->doc_root;
}

const string& get_module_thumb( unsigned int mod )
{
   return g_modules[ mod ]->thumb_file;
}

unsigned int find_module( const UUID128& uuid )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      if (g_modules[ i ]->uuid == uuid)
         return g_modules[ i ]->mod;
   }

   return -1;
}

unsigned int find_module( const string& name )
{
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      if (!_stricmp( g_modules[ i ]->name.c_str( ), name.c_str( ) ))
         return g_modules[ i ]->mod;
   }

   return -1;
}   

void cmd_modls( const ArgList& args )
{
   if (args.size( ) > 1)
   {
      core::system_errspeak( "Too many arguments." );
      return;
   }

   if (g_modules.size( ) == 0)
   {
      core::system_speak( "No modules are loaded." );
      return;
   }

   ostringstream ss;
   string uuid;
   for (size_t i = 0; i < g_modules.size( ); i++)
   {
      MODULE* module = g_modules[ i ];
      ss.str( "" );
      ss << i + 1 << ". ";

      ss << "name=\"" << module->name << "\" ";
      ss << "enabled=" << ( (module->state & MOD_ENABLED) ? "yes" : "no" ) << ' ';
      ss << "adapter=\"" << module->adapter->name << "\" ";
      ss << "fancy_name=\"" << module->fancy_name << "\"";
      if (!!module->uuid)
         ss << " uuid=" << uuid_to_str( module->uuid, &uuid );

      core::system_speak( ss.str( ) );
   }
}

void cmd_modon( const ArgList& args )
{
   if (args.size( ) < 2)
   {
      core::system_errspeak( "Expected a module name." );
      return;
   }

   size_t i_arg = 1;
   string arg = (args.size( ) > i_arg++) ? args[ i_arg - 1 ] : "";
   
   while (arg.length( ))
   {
      unsigned int mod = find_module( arg );

      if (mod != -1)
      {
         core::system_speak( "Enabling " + arg );
         enable_module( mod, true );
      }
      else
      {
         core::system_errspeak( "Can't find module " + arg );
      }

      arg = (args.size( ) > i_arg++) ? args[ i_arg - 1 ] : "";
   }
}

void cmd_modoff( const ArgList& args )
{
   if (args.size( ) < 2)
   {
      core::system_errspeak( "Expected a module name." );
      return;
   }

   size_t i_arg = 1;
   string arg = (args.size( ) > i_arg++) ? args[ i_arg - 1 ] : "";

   while (arg.length( ))
   {
      unsigned int mod = find_module( arg );

      if (mod != -1)
      {
         core::system_speak( "Disabling " + arg );
         enable_module( mod, false );
      }
      else
      {
         core::system_errspeak( "Can't find module " + arg );
      }

      arg = (args.size( ) > i_arg++) ? args[ i_arg - 1 ] : "";
   }
}

void cmd_modtouch( const ArgList& args )
{
   if (args.size( ) < 2)
   {
      core::system_errspeak( "Expected a module name." );
      return;
   }

   size_t i_arg = 1;
   string arg = (args.size( ) > i_arg++) ? args[ i_arg - 1 ] : "";

   while (arg.length( ))
   {
      unsigned int mod = find_module( arg );

      if (mod != -1)
      {
         core::system_speak( "Touching " + arg );
         touch_module( mod );
      }
      else
      {
         core::system_errspeak( "Can't find module " + arg );
      }

      arg = (args.size( ) > i_arg++) ? args[ i_arg - 1 ] : "";
   }
}

} // namespace modules