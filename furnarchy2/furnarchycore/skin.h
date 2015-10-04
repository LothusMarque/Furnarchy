#ifndef _FURNARCHYCORE_SKIN_H__
#define _FURNARCHYCORE_SKIN_H__

#include "common.h"

namespace skin
{
   bool initialize( );
   bool uninitialize( );

   bool on_command( const ArgList& args );
   void on_tick( unsigned int dt );
   void on_render( HDC dc, unsigned int width, unsigned int height );
   bool on_wndproc( WNDPROCPARAMS* params );
   void on_modstate( unsigned int mod, long state );
}

#endif
