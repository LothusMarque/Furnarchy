#include "furn2.h"
#include <string>
#include <sstream>

using namespace std;
using namespace furn2;

unsigned int g_mod_index;
bool         g_enabled;

/************************************************************************/
/* Furnarchy2 entry points.                                             */
/* Feel free to leave out the ones you don't need.                      */
/* They're just all defined here for completness.                       */
/************************************************************************/

F2EXPORT intptr_t F2CALL on_module( Module_Event e, void* param )
{
   /* See definition of Module_Event for all module events. */
   switch (e)
   {
   case MOD_EVENT_START:
      g_mod_index = ((const MODULEINFO*) param)->mod;
      break;

   case MOD_EVENT_STOP:
      // module is unloading.
      break;

   case MOD_EVENT_TOUCH:
      f2_speak( "Dummy module 1.00.0 - Sits here and takes up space." );
      break;

   case MOD_EVENT_ENABLE:
      g_enabled = true;
      break;

   case MOD_EVENT_DISABLE:
      g_enabled = false;
      break;
   }

   return 0;
}

F2EXPORT intptr_t F2CALL on_client( Client_Event e, void* param )
{
   /* See definition of Client_Event for all client events. */
   return 0;
}

F2EXPORT bool F2CALL on_wndproc( WNDPROCPARAMS* params )
{
   /* All the regular WM messages come through here. */
   /* Return 'true' to tell furnarchy you handled it. */

   return false;
}

F2EXPORT intptr_t F2CALL on_world( World_Event e, void* param )
{
   /* See definition of World_Event for all world events. */
   return 0;
}

F2EXPORT bool F2CALL on_command( int argc, char* argv[] )
{
   /* @... commands come through here.  Return true if you handled it. */
   return false;
}

F2EXPORT void F2CALL on_inet( Revision rev, const char* line, int from, size_t num )
{
   /* Inbound network lines come through here.
   *  'line' is always null-terminated.
   *  'from' is FROM_SERVER if the line was originally from the server
   *     (not generated).
   *  'num' is the line number.
   *  Use f2_reviseline() to edit the line when rev == REV_REVISING. */
}

F2EXPORT void F2CALL on_onet( Revision rev, const char* line, int from, size_t num )
{
   /* Outbound network lines come through here.
   *  'line' is always null-terminated.
   *  'from' is FROM_CLIENT if the line was originally from the client
   *     (not generated).
   *  'num' is the line number.
   *  Use f2_reviseline() to edit the line when rev == REV_REVISING. */
}
