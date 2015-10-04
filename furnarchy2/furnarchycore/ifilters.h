#ifndef _FURNARCHYCORE_IFILTERS_H__
#define _FURNARCHYCORE_IFILTERS_H__

#include "common.h"
#include "core.h"
#include "common/RegexOp.h"
#include <string>
#include <vector>

namespace ifilters
{
   bool initialize( );
   bool uninitialize( );

   bool on_command( const ArgList& arg_list );
   long on_inbound_line( string* line, int from, size_t num );
};

#endif