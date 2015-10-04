#include "C:\Program Files\Furnarchy2\furn2.h"
#include <string>

using namespace furn2;
using namespace std;

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

/* Define the on_onet handler to process client to server network lines. */
F2EXPORT void F2CALL on_onet( Revision pass, const char* line, int from, size_t num )
{
   if (!g_enabled) // Don't do anything if we're not enabled.
      return;
   
   if (pass == REV_REVISING)  // Can only modify the line during this pass.
   {
      if (line[ 0 ] == '"')   // Speech command prefix.
      {
         /* Reverse the chat string. */
         string reversed = "\"";
         
         if (*(++line)) {
            const char* end = line + ( strlen( line ) - 1 );
            while (end != line) reversed.push_back( *(end--) );
         }
         
         // Submit the modified line.
         f2_reviseline( reversed.c_str( ) );
      }
   }
}
