#include "C:\Program Files\Furnarchy2\furn2.h"

using namespace furn2;

/* Define the on_world handler to receive world events. */
F2EXPORT intptr_t F2CALL on_world( World_Event e, intptr_t param )
{
   if (e == WO_EVENT_LOGGEDIN)   // Player has logged in!
      // Print a message to the chat buffer.
      f2_speak( "Hello, World!" );
   
   return 0;
}
