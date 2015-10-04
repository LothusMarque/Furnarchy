#include "C:\Program Files\Furnarchy2\furn2.h"

using namespace furn2;

bool g_enabled = false;

/* Define the on_module handler to process module events. */
F2EXPORT intptr_t F2CALL on_module( Module_Event e, intptr_t param )
{
   // Keep track of whether we're enabled or not.
   if (e == MOD_EVENT_ENABLE)
      g_enabled = true;
   else if (e == MOD_EVENT_DISABLE)
      g_enabled = false;
   
   return 0;
}

/* Define the on_inet handler to process server to client network lines. */
F2EXPORT void F2CALL on_inet( Revision pass, const char* line, int from, size_t num )
{
   if (!g_enabled) // Don't do anything if we're not enabled.
      return;
   
   if (pass == REV_FINAL)  // Only interested in the end product.
   {
      if (line[ 0 ] == '(')   // Chat buffer line command prefix.
      {
         /* Flash the window 3 times. */
         FLASHWINFO fwi = { sizeof( FLASHWINFO ), f2_getwnd( ), 
                            FLASHW_ALL, 3, 0 };
         FlashWindowEx( &fwi );
      }
   }
}
