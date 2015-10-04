#define WIN32_LEAN_AND_MEAN
#include "../furnarchycore/furn2.h"
#include "FsmlElem.h"
#include <commctrl.h>
#include <SDL.h>
#include <iostream>

using namespace furn2;
using namespace std;

HINSTANCE g_inst;

BOOL APIENTRY DllMain( HMODULE inst, DWORD reason, LPVOID reserved )
{
   if (reason == DLL_PROCESS_ATTACH)
   {
      g_inst = inst;
      DisableThreadLibraryCalls( (HMODULE) inst );

      /* Initialize SDL. */
      if (SDL_Init( SDL_INIT_VIDEO ))
      {
         cerr << "Unable to initialize SDL: " << SDL_GetError( ) << '\n';
      }
   }
   else if (reason == DLL_PROCESS_DETACH)
   {
      SDL_Quit( );
   }
   return TRUE;
}

//////////////////////////////////////////////////////////////////////////

FsmlElemSkin* g_skin = 0;

F2EXPORT bool F2CALL skin_load( const char* name )
{
   assert( g_skin == NULL );

   try
   {
      g_skin = new FsmlElemSkin( g_inst, f2_getwnd( ), name );
      g_skin->addRef( );
   }
   catch (exception& e)
   {
      f2_speak( e.what( ) );
   } 
   
   return g_skin != NULL;
}

F2EXPORT void F2CALL skin_free( )
{
   if (g_skin)
   {
      g_skin->release( );
      g_skin = 0;

      InvalidateRect( f2_getwnd( ), NULL, FALSE );
   }
}

F2EXPORT void F2CALL on_tick( unsigned int dt )
{
   if (g_skin)
      g_skin->onTick( dt );
}

F2EXPORT void F2CALL on_render( HDC dc, unsigned int width, unsigned int height )
{
   if (g_skin)
      g_skin->draw( dc );
}

F2EXPORT bool F2CALL on_wndproc( WNDPROCPARAMS* params )
{
   if (g_skin)
   {
      params->retval = 0;
      return g_skin->onWndProc( params->msg, params->wparam, params->lparam );
   }

   return false;
}

F2EXPORT void F2CALL on_modstate( unsigned int mod, long state )
{
   if (g_skin)
   {
      g_skin->onModState( mod, state );
   }
}
