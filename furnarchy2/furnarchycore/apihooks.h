#ifndef _FURNARCHYCORE_APIHOOKS_H__
#define _FURNARCHYCORE_APIHOOKS_H__

#include "common.h"

namespace apihooks
{
   bool initialize( );
   bool uninitialize( );

   /** Replaces an IAT function with a hook function.
   *  \param   module          The name of the DLL the function to hook resides in.
   *  \param   name            The null-terminated name of the function to hook.
   *  \param   hook_proc       The new function entry point.
   *  \param   name_is_ordinal \c true if \c name is an ordinal number, not a pointer.
   *  \return The previous function entry point.
   */
   void* hook_api( const char* module, const void* name,
                   void* hook_proc, bool name_is_ordinal = false );
};

#endif

