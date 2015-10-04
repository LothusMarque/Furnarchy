#include "curses.h"
#include <cassert>

using namespace std;
using namespace furn2;

//////////////////////////////////////////////////////////////////////////

HINSTANCE       g_inst;
unsigned int    g_mod_index;

//////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain( HINSTANCE inst, DWORD reason, LPVOID reserved )
{
   if (reason == DLL_PROCESS_ATTACH)
   {
      g_inst = inst;
      DisableThreadLibraryCalls( inst );
   }
   return TRUE;
}

//////////////////////////////////////////////////////////////////////////

F2EXPORT intptr_t F2CALL on_module( Module_Event e, intptr_t param )
{
   /* See definition of Module_Event for all module events. */
   switch (e)
   {
   case MOD_EVENT_START:
      g_mod_index = ((const MODULEINFO*) param)->mod;
      assert( !curses::Curses::singleton( ) );
      curses::Curses::singleton_create( g_inst, g_mod_index );
      if (curses::Curses::singleton( )->getOptions( ) & curses::Curses::OPT_AUTOSTART)
         f2_modenable( g_mod_index, true );
      break;

   case MOD_EVENT_STOP:
      curses::Curses::singleton_destroy( );
      break;

   case MOD_EVENT_TOUCH:
      if (curses::Curses::singleton( ))
         curses::Curses::singleton( )->showScreen( );
      break;

   case MOD_EVENT_ENABLE:
      if (curses::Curses::singleton( ))
         curses::Curses::singleton( )->onEnable( true );
      break;

   case MOD_EVENT_DISABLE:
      if (curses::Curses::singleton( ))
         curses::Curses::singleton( )->onEnable( false );
      break;
   }

   return 0;
}

F2EXPORT intptr_t F2CALL on_client( Client_Event e, intptr_t param )
{
   switch (e)
   {
   case CL_EVENT_WNDCREATED:
      if (curses::Curses::singleton( ))
         curses::Curses::singleton( )->setWnd( (HWND) param );
      break;

   case CL_EVENT_SCRLOAD:
      if (curses::Curses::singleton( ))
         curses::Curses::singleton( )->onClientScreen( (Client_Screen) param );
      break;

   case CL_EVENT_SETMAINSOCKET:
      if (curses::Curses::singleton( ))
         curses::Curses::singleton( )->onSetMainSocket( (SOCKET) param );
   }

   return 0;
}

F2EXPORT bool F2CALL on_wndproc( WNDPROCPARAMS* params )
{
   if (curses::Curses::singleton( ))
      return curses::Curses::singleton( )->onWndProc( params );

   return false;
}

F2EXPORT intptr_t F2CALL on_world( World_Event e, intptr_t param )
{
   switch (e)
   {
   case WO_EVENT_CONNECTED:
      if (curses::Curses::singleton( ))
         curses::Curses::singleton( )->onConnected( );
      break;
   case WO_EVENT_LOGGEDIN:
      if (curses::Curses::singleton( ))
         curses::Curses::singleton( )->onLoggedIn( );
      break;
   case WO_EVENT_DISCONNECTED:
      if (curses::Curses::singleton( ))
         curses::Curses::singleton( )->onDisconnected( );
      break;
   }

   return 0;
}

F2EXPORT bool F2CALL on_command( int argc, const char* argv[] )
{
   if (curses::Curses::singleton( ))
      return curses::Curses::singleton( )->onCommand( argc, argv );
   return false;
}

F2EXPORT void F2CALL on_inet( Revision rev, const char* line, int from, size_t num )
{
   if (rev == REV_REVISING)
   {
      if (curses::Curses::singleton( ))
         curses::Curses::singleton( )->onNetIn( line, from, num );
   }
}

F2EXPORT void F2CALL on_onet( Revision rev, const char* line, int from, size_t num )
{
   if (rev == REV_REVISING)
   {
      if (curses::Curses::singleton( ))
         curses::Curses::singleton( )->onNetOut( line, from, num );
   }
}
