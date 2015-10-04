#ifndef _FURNARCHYCORE_NEWS_H__
#define _FURNARCHYCORE_NEWS_H__

#include "common.h"

namespace news 
{
   bool initialize( );
   bool uninitialize( );

   bool on_command( const ArgList& args );
   void on_tick( unsigned int dt );
}

#endif