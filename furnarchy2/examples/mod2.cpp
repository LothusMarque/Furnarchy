#include "C:\Program Files\Furnarchy2\furn2.h"
#include <cstring>

using namespace furn2;
using namespace std;

/* Define the on_command handler to receive commmand line input. */
F2EXPORT bool F2CALL on_command( int argc, char* argv[] )
{
   // If the user typed "@sayhello"
   if (argc >= 1 && strcmp( argv[ 0 ], "sayhello" ) == 0)
   {
      // Send out a chat command.
      f2_netoutline( "\"Hello, World!" );
      
      return true;   // Tell Furnarchy that we handled it.
   }
   
   return false;
}
