#define WIN32_LEAN_AND_MEAN
#include "furn2.h"
#include <cstring>
#include <string>
#include <fstream>

#define WINAMPEENY_VERSION "2.1-r1"
// Poll every 3 seconds.
#define POLL_DELAY 3000
#define DEFAULT_TAG   "[#SM] Now Playing \"<i>%song</i>\""
#define DEFAULT_BCAST ":plays <i>%song</i> #SM"

using namespace furn2;
using namespace std;

//////////////////////////////////////////////////////////////////////////

/** Says the currently playing song to everyone. */
void broadcast_song( );
/** Updates the current song. */
void poll( );
/** Tags the player's desc with the song. */
void tag_desc( );
/** Untags the player's desc with the song. */
void untag_desc( );
/** Performs % tag replacement on a string. */
string replace_tags( const string& str );
/** Loads the strings.txt file. */
void load_strings( );

//////////////////////////////////////////////////////////////////////////

unsigned int g_my_mod;
string       g_my_root;
bool         g_enabled;
wstring      g_curr_song;
string       g_tag_fmt       = DEFAULT_TAG;
string       g_bcast_fmt     = DEFAULT_BCAST;
unsigned int g_poll_cooldown = POLL_DELAY;
int          g_tag_id;

/************************************************************************/
/* Function definitions.                                                */
/************************************************************************/

string replace_tags( const string& str )
{
   wstring w;
   for (size_t i = 0; i < str.length( ); ++i)
   {
      if (str[ i ] == '%' && !str.compare( i, 5, "%song" ))
      {
         w += g_curr_song;
         i += 4;
      }
      else
         w += (wchar_t) (unsigned short) str[ i ];
   }

   char* sz = new char[ w.length( ) + 1 ];
   WideCharToMultiByte( 28591 /* Latin 1 */, 0, w.c_str( ), -1,
                        sz, (int) w.length( ) + 1, NULL, NULL );
   string r( sz );
   delete [] sz;
   return r;
}

void tag_desc( )
{
   if (g_curr_song.length( ) && g_tag_fmt.length( ))
   {
      f2_desctag_set( g_tag_id, replace_tags( g_tag_fmt ).c_str( ) );
   }
}

void untag_desc( )
{
   f2_desctag_set( g_tag_id, "" );
}

inline void broadcast_song( )
{
   if (g_curr_song.length( ) && g_bcast_fmt.length( ))
   {
      string cmd;
      if (g_bcast_fmt[ 0 ] == ':') // If it begins with a ':', use an emote.
      {
         cmd = ":";
         cmd += replace_tags( g_bcast_fmt.substr( 1 ) );
      }
      else
      {
         cmd = "\"";
         cmd += replace_tags( g_bcast_fmt );
      }
      f2_netoutline( cmd.c_str( ), cmd.length( ) );
   }
}

void poll( )
{
   wstring song;
   {
      HWND wnd = FindWindow( "Winamp v1.x", NULL );

      if (wnd)
      {
         /* The current song is just in the window title. */
         {
            wchar_t wsz_title[ 256 ];
            GetWindowTextW( wnd, wsz_title, 256 );
            wsz_title[ 255 ] = L'\0';
            song = wsz_title;
         }

         /* Strip the " - Winamp" suffix. */
         size_t pos = song.rfind( L" - Winamp" );
         if (pos != song.npos)
            song.resize( pos );

         /* Strip .mp3 suffixes. */
         pos = song.rfind( L".mp3" );
         if (pos != song.npos)
            song.resize( pos );

         /* Strip playlist numbers. */
         for (pos = 0; isdigit( song.c_str( )[ pos ] ); ++pos) ;
         if (song.compare( pos, 2, L". " ) == 0)
            song.erase( 0, pos + 2 );
      }
   }

   if (song != g_curr_song)
   {
      g_curr_song = song;
      tag_desc( );
   }
}

void load_strings( )
{
   // If the strings file in the document root is missing, resort to the one in the
   // module directory.
   ifstream fs( (string( f2_moddocroot( g_my_mod ) ) +  "\\strings.txt").c_str( ) );
   if (!fs.good( ))
   {
      fs.clear( );
      fs.open( (string( f2_modroot( g_my_mod ) ) + "\\strings.txt" ).c_str( ) );
   }
   if (fs.good( ))
   {
      g_tag_fmt.clear( );
      g_bcast_fmt.clear( );

      // First line is the desc tag.
      getline( fs, g_tag_fmt );
      // Second line is the broadcast.
      getline( fs, g_bcast_fmt );
   }
}

/************************************************************************/
/* Furn2 entry points.                                                  */
/************************************************************************/

F2EXPORT intptr_t F2CALL on_module( Module_Event e, void* params )
{
   switch (e)
   {
   case MOD_EVENT_START:
      {
         const MODULEINFO* info = (const MODULEINFO*) params;
         g_my_mod  = info->mod;
         g_my_root = info->root;

         // Load strings.
         load_strings( );

         // Register for a desctag slot.
         g_tag_id = f2_desctag_reg( );
      }
      break;

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
      f2_speak( "Winampeeny " WINAMPEENY_VERSION " - A less annoying song broadcaster." );
      f2_speak( "Just turn on the module and Winampeeny will display "
                "the current winamp song in your desc." );
      f2_speak( "Type '@song' to say the current song out loud." );
      f2_speak( "Edit strings.txt in the winampeeny document folder to customize strings." );
      break;

   case MOD_EVENT_MSG:
      if (!strcmp( (const char*) params, "song" ))
         return (intptr_t) g_curr_song.c_str( );
      return 0;
   }

   return 0;
}

F2EXPORT intptr_t F2CALL on_client( Client_Event e, void* params )
{
   if (e == CL_EVENT_TICK && g_enabled)
   {
      const unsigned int dt = (unsigned int) (intptr_t) params;

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
         broadcast_song( );
      else
         f2_errspeak( "Winampeeny isn't enabled!" );
      return true;   // Handled.
   }

   return false;
}
