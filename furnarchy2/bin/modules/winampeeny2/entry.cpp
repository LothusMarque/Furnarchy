#define WIN32_LEAN_AND_MEAN
#include "furn2.h"
#include <cstring>
#include <string>

// Poll every 3 seconds.
#define POLL_DELAY 3000

using namespace furn2;
using namespace std;

//////////////////////////////////////////////////////////////////////////

/** Says the currently playing song to everyone. */
void say_song( );
/** Updates the current song. */
void poll( );
/** Tags the player's desc with the song. */
void tag_desc( );
/** Untags the player's desc with the song. */
void untag_desc( );

//////////////////////////////////////////////////////////////////////////

index_t      g_my_mod;
string       g_my_root;
bool         g_enabled;
string       g_curr_song;
unsigned int g_poll_cooldown = POLL_DELAY;
int          g_tag_id;

/************************************************************************/
/* Function definitions.                                                */
/************************************************************************/

void tag_desc( )
{
   if (g_curr_song.length( ))
   {
      string tag = "[#SM] Now playing: \"<i>";
      tag += g_curr_song;
      tag += "</i>\"";

      f2_desctag_set( g_tag_id, tag.c_str( ) );
   }
}

void untag_desc( )
{
   f2_desctag_set( g_tag_id, "" );
}

inline void say_song( )
{
   if (g_curr_song.length( ))
   {
      string say_cmd = "\"<i>";
      say_cmd += g_curr_song;
      say_cmd += "</i> #SM\n";
      f2_netout( say_cmd.c_str( ), say_cmd.length( ) );
   }
}

void poll( )
{
   string song;
   {
      HWND wnd = FindWindow( "Winamp v1.x", NULL );

      if (wnd)
      {
         /* The current song is just in the window title. */
         {
            char sz_title[ 256 ];
            GetWindowText( wnd, sz_title, 256 );
            sz_title[ 256 ] = '\0';
            song = sz_title;
         }

         /* Strip the " - Winamp" suffix. */
         size_t pos = song.rfind( " - Winamp" );
         if (pos != song.npos)
            song.resize( pos );

         /* Strip .mp3 suffixes. */
         pos = song.rfind( ".mp3" );
         if (pos != song.npos)
            song.resize( pos );

         /* Strip playlist numbers. */
         for (pos = 0; isdigit( song.c_str( )[ pos ] ); ++pos) ;
         if (song.compare( pos, 2, ". " ) == 0)
            song.erase( 0, pos + 2 );
      }
   }

   if (song != g_curr_song)
   {
      g_curr_song = song;
      tag_desc( );
   }
}

/************************************************************************/
/* Furn2 entry points.                                                  */
/************************************************************************/

F2EXPORT int F2CALL on_module( Module_Event e, void* params )
{
   switch (e)
   {
   case MOD_EVENT_START:
      {
         const MODULEINFO* info = (const MODULEINFO*) params;
         g_my_mod  = info->mod;
         g_my_root = info->root;

         // Register for a desctag slot.
         g_tag_id = f2_desctag_reg( );
         break;
      }

   case MOD_EVENT_ENABLE:
      g_enabled = true;
      g_poll_cooldown = 0; // Guarantee a poll at next tick.
      tag_desc( );
      break;

   case MOD_EVENT_DISABLE:
      g_enabled = false;
      untag_desc( );
      break;

   case MOD_EVENT_TOUCH:
      f2_speak( "Winampeeny 2.00.0 - A less annoying song broadcaster." );
      f2_speak( "Just turn on the module and Winampeeny will display "
                "the current winamp song in your desc." );
      f2_speak( "Type '@song' to say the current song out loud." );
      break;
   }

   return 0;
}

F2EXPORT int F2CALL on_client( Client_Event e, void* params )
{
   if (e == CL_EVENT_TICK && g_enabled)
   {
      const unsigned int dt = (unsigned int) params;

      if (g_poll_cooldown <= dt)
      {
         poll( );
         g_poll_cooldown = POLL_DELAY;
      }
      else
         g_poll_cooldown -= dt;
   }

   return 0;
}

F2EXPORT bool F2CALL on_command( int argc, char* argv[] )
{
   // assert( argc > 0 );
   if (_stricmp( argv[ 0 ], "song" ) == 0)
   {
      if (g_enabled)
         say_song( );
      else
         f2_errspeak( "Winampeeny isn't enabled!" );
      return true;   // Handled.
   }

   return false;
}
