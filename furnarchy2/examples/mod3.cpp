#include "C:\Program Files\Furnarchy2\furn2.h"

using namespace furn2;

/* Define the on_wndproc handler to process windows messages. */
F2EXPORT bool F2CALL on_wndproc( WNDPROCPARAMS* params )
{
   if (params->msg == WM_CHAR)
   {
      if (params->wparam == 's' &&
          !(params->lparam & 0x40000000) &&
          GetKeyState( VK_CONTROL ) & 0x80000000)
      {
         // Ctrl + S pressed!
         // Send out a chat command.
         f2_netoutline( "\"Hello, World!" );
         
         // Tell Furnarchy we handled it.
         params->retval = 0;
         return true; 
      }
   }
   
   return false;
}
