#ifndef _FURNARCHYCORE_OFILTERS_H__
#define _FURNARCHYCORE_OFILTERS_H__

#include "common.h"

namespace ofilters
{
   bool initialize( );
   bool uninitialize( );
   
   bool on_command( const ArgList& arg_list );
   long on_outbound_line( string* line, int from, size_t num );
}

#endif
