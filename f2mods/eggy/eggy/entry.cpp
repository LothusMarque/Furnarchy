#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "eggy.h"

using namespace furn2;
using eggy::Eggy;

//////////////////////////////////////////////////////////////////////////

HINSTANCE    g_inst;
unsigned int g_mod;

//////////////////////////////////////////////////////////////////////////

BOOL APIENTRY DllMain( HINSTANCE inst, DWORD reason, LPVOID reserved )
{
   if (reason == DLL_PROCESS_ATTACH)
   {
      g_inst = inst;
      DisableThreadLibraryCalls( (HMODULE) inst );
   }
   return TRUE;
}

F2EXPORT intptr_t F2CALL on_client( Client_Event e, void* param )
{
   switch (e)
   {
   case CL_EVENT_WNDCREATED:
      assert( !Eggy::singleton( ) );
      Eggy::singleton_create( (HWND) param );
      break;
   case CL_EVENT_TICK:
      if (Eggy::singleton( ))
         Eggy::singleton( )->onTick( (unsigned int) param );
      break;
/*   case CL_EVENT_PLAYERMENUSHOW:
      if (Eggy::singleton( ))
         Eggy::singleton( )->onPlayerMenuShow( (const char*) param );
      break; */
   }
   return 0;
}

F2EXPORT intptr_t F2CALL on_module( Module_Event e, void* param )
{
   switch (e)
   {
   case MOD_EVENT_START:
      g_mod = ((MODULEINFO*) param)->mod;
      eggy::eggy_init( g_inst, g_mod );
      break;

   case MOD_EVENT_STOP:
      Eggy::singleton_destroy( );
      eggy::eggy_uninit( );
      break;

   case MOD_EVENT_ENABLE:
      if (Eggy::singleton( ))
         Eggy::singleton( )->enable( true );
      break;

   case MOD_EVENT_DISABLE:
      if (Eggy::singleton( ))
         Eggy::singleton( )->enable( false );
      break;

   case MOD_EVENT_TOUCH:
      if (Eggy::singleton( ))
         Eggy::singleton( )->toggleBuddyList( );
      break;

   case MOD_EVENT_MSG:
      if (Eggy::singleton( ))
      {
         if (param)
            return Eggy::singleton( )->onModMsg( (const char*) param );
      }      
      break;

   default:
      break;
   }
   return 0;
}

F2EXPORT intptr_t F2CALL on_world( World_Event e, void* param )
{
   if (Eggy::singleton( ))
   {
      if (e == WO_EVENT_LOGGEDIN)
      {
         Eggy::singleton( )->onLoggedIn( );
      }
      else if (e == WO_EVENT_DISCONNECTED)
      {
         Eggy::singleton( )->onDisconnected( );
      }
   }

   return 0;
}

F2EXPORT void F2CALL on_inet( Revision pass, const char* line, int from, size_t num )
{
   if (Eggy::singleton( ))
   {
      if (Eggy::singleton( )->onINet( pass, line, from ))
         f2_hideline( HIDE_CLIENT );
   }
}

F2EXPORT void F2CALL on_onet( Revision pass, const char* line, int from, size_t num )
{
   if (Eggy::singleton( ))
   {
      if (Eggy::singleton( )->onONet( pass, line, from ))
         f2_hideline( HIDE_CLIENT );
   }
}

F2EXPORT bool F2CALL on_wndproc( WNDPROCPARAMS* params )
{
   if (Eggy::singleton( ))
      return Eggy::singleton( )->onWndProc( params );
   return false;
}

F2EXPORT bool F2CALL on_command( int argc, const char* argv[] )
{
   if (Eggy::singleton( ))
      return Eggy::singleton( )->onCommand( argc, argv );
   return false;
}