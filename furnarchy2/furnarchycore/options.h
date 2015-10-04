#ifndef _FURNARCHYCORE_OPTIONS_H__
#define _FURNARCHYCORE_OPTIONS_H__

#include "common.h"

namespace options
{
   bool initialize( );
   bool uninitialize( );

   const list<string>& get_modules_masked( );
   const list<string>& get_modules_accepted( );
   bool                get_single_user_mode( );
   const string&       get_command_prefix( );
};

#endif 