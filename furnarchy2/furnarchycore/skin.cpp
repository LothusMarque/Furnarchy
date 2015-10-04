#include "skin.h"
#include "window.h"
#include "core.h"

namespace skin {

//////////////////////////////////////////////////////////////////////////

struct SKINHOST
{
   typedef bool(CDECL *PFN_SKIN_LOAD)( const char* name );
   typedef void(CDECL *PFN_SKIN_FREE)( );
   typedef void(CDECL *PFN_ON_TICK)( unsigned int dt );
   typedef void(CDECL *PFN_ON_RENDER)( HDC dc, unsigned int width, unsigned int height );
   typedef bool(CDECL *PFN_ON_WNDPROC)( WNDPROCPARAMS* params );
   typedef bool(CDECL *PFN_ON_MODSTATE)( unsigned int mod, long state );

   HMODULE lib;
   bool    loaded;
   PFN_SKIN_LOAD   skin_load;
   PFN_SKIN_FREE   skin_free;
   PFN_ON_TICK     on_tick;
   PFN_ON_RENDER   on_render;
   PFN_ON_WNDPROC  on_wndproc;
   PFN_ON_MODSTATE on_modstate;
};

//////////////////////////////////////////////////////////////////////////

void cmd_skuse( const ArgList& args );

bool      g_initialized = false;
SKINHOST* g_host        = 0;

bool initialize( )
{
   if (!g_initialized)
   {
      g_initialized = true;

      /* Load the host. */
      string lib_file = core::get_furn_root( );
      lib_file += "\\skin.dll";

      HMODULE lib = LoadLibrary( lib_file.c_str( ) );
      if (!lib)
      {
         ostringstream ss;
         ss << "Couldn't load skin library '" << lib_file << "' ("
            << GetLastError( ) << ").";
         core::system_errspeak( ss.str( ) );
         return true;
      }

      /* Load the host symbols. */
      SKINHOST::PFN_SKIN_LOAD  sym_skin_load  = 
         (SKINHOST::PFN_SKIN_LOAD) GetProcAddress( lib, "skin_load" );
      SKINHOST::PFN_SKIN_FREE  sym_skin_free  = 
         (SKINHOST::PFN_SKIN_FREE) GetProcAddress( lib, "skin_free" );
      SKINHOST::PFN_ON_TICK    sym_on_tick    = 
         (SKINHOST::PFN_ON_TICK) GetProcAddress( lib, "on_tick" );
      SKINHOST::PFN_ON_RENDER  sym_on_render  = 
         (SKINHOST::PFN_ON_RENDER) GetProcAddress( lib, "on_render" );
      SKINHOST::PFN_ON_WNDPROC sym_on_wndproc = 
         (SKINHOST::PFN_ON_WNDPROC) GetProcAddress( lib, "on_wndproc" );
      SKINHOST::PFN_ON_MODSTATE sym_on_modstate =
         (SKINHOST::PFN_ON_MODSTATE) GetProcAddress( lib, "on_modstate" );      

      if (!sym_skin_load || !sym_skin_free 
          || !sym_on_tick || !sym_on_render || !sym_on_wndproc
          || !sym_on_modstate)
      {
         core::system_errspeak( "Skin.dll is missing required symbols." );
         FreeLibrary( lib );
         return true;
      }

      /* Otherwise, it's well-formed. */
      g_host = new SKINHOST;
      g_host->lib         = lib;
      g_host->loaded      = false;
      g_host->skin_load   = sym_skin_load;
      g_host->skin_free   = sym_skin_free;
      g_host->on_tick     = sym_on_tick;
      g_host->on_render   = sym_on_render;
      g_host->on_wndproc  = sym_on_wndproc;
      g_host->on_modstate = sym_on_modstate;
   }

   return true;
}

bool uninitialize( )
{
   if (g_initialized)
   {
      g_initialized = false;

      if (g_host)
      {
         if (g_host->loaded)
            g_host->skin_free( );
         FreeLibrary( g_host->lib );
         delete g_host;
         g_host = 0;
      }
   }

   return true;
}

bool on_command( const ArgList& args )
{
   string arg = args[ 0 ];
   to_lower( &arg );
   bool handled = false;

   if (arg == "skuse")
   {
      cmd_skuse( args );
      handled = true;
   }

   return handled;
}

void on_tick( unsigned int dt )
{
   if (g_host && g_host->loaded)
      g_host->on_tick( dt );
}

void on_render( HDC dc, unsigned int width, unsigned int height )
{
   if (g_host && g_host->loaded)
      g_host->on_render( dc, width, height );
}

bool on_wndproc( WNDPROCPARAMS* params )
{
   if (g_host && g_host->loaded)
      return g_host->on_wndproc( params );
   return false;
}

void on_modstate( unsigned int mod, long state )
{
   if (g_host && g_host->loaded)
      g_host->on_modstate( mod, state );
}

void cmd_skuse( const ArgList& args )
{
   if (args.size( ) < 2)
   {
      core::system_errspeak( "Error: Expected a skin name." );
      return;
   }

   if (!g_host)
   {
      core::system_errspeak( "Error: Skin host isn't loaded." );
      return;
   }

   if (g_host->loaded)
   {
      g_host->skin_free( );
      g_host->loaded = false;
   }

   if (args[ 1 ] == "none")
   {
      core::system_speak( "Skin unloaded." );
      return;
   }

   if (!window::get_handle( ))
   {
      core::system_errspeak( "Error: Can't load a skin until the window "
                             "has been created." );
      return;
   }

   g_host->loaded = g_host->skin_load( args[ 1 ].c_str( ) );

   if (g_host->loaded)
      core::system_speak( "Skin loaded." );
   else
      core::system_errspeak( "Skin failed to load." );
}

} // namespace skin