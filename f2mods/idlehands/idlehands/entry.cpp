#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _WIN32_WINNT 0x0501
#include "util.h"
#include "furn2/furn2.h"
#include "yiffurc/net.h"
#include "tinyxml/tinyxml.h"
#include "resource.h"
#include <WindowsX.h>
#include <commctrl.h>
#include <cassert>
#include <string>
#include <sstream>
#include <list>
#include <xutility>
#include <queue>
#include <fstream>

namespace idlehands {

using namespace furn2;
using namespace std;

//////////////////////////////////////////////////////////////////////////

#define IDLEHANDS_VERSION "1.2-r5"

enum { OPT_BLINVERT = 0x1, OPT_AUTOENABLE = 0x8, OPT_REPORTTIMEAWAY = 0x10 };

const unsigned int KEEPALIVE_RATE       = 600000; // ms
const unsigned int UPDATE_RATE          = 60000;  // ms
const time_t       DEFAULT_AUTOIDLE     = 300;    // s
const unsigned int REPLY_RATE           = 900000; // ms
const char* const  DEFAULT_IDLEMSG      = "Auto-Idle";
const char* const  DEFAULT_AWAYTAG      = "[AFF %hh,%mm]";
const char* const  DEFAULT_AWAYMSG      = "Sorry, %player, I'm currently away!";
const int          DEFAULT_OPTS         = /* OPT_AUTOENABLE | */ OPT_REPORTTIMEAWAY;
   
//////////////////////////////////////////////////////////////////////////

namespace dlg {

   namespace _ctrl {
      
      const int tag[] = { IDC_TAG_0, IDC_TAG_1,
                          IDC_TAG_TAG };
      const int whisper[] = { IDC_WHISPER_0, IDC_WHISPER_1, IDC_WHISPER_2,
                              IDC_WHISPER_WHISPER };
      const int idling[] = { IDC_IDLING_0, IDC_IDLING_1, IDC_IDLING_2,
                             IDC_IDLING_3, IDC_IDLING_AUTOIDLE, IDC_IDLING_REPORT };
      const int blacklist[] = { IDC_BLACKLIST_0, IDC_BLACKLIST_1, IDC_BLACKLIST_INVERT,
                                IDC_BLACKLIST_BLACKLIST };
      const int commands[] = { IDC_COMMANDS_0, IDC_COMMANDS_1, IDC_COMMANDS_2,
                               IDC_COMMANDS_ENTER, IDC_COMMANDS_EXIT };
      const int module[] = { IDC_MODULE_0, IDC_MODULE_AUTOENABLE };
      const int about[] = { IDC_ABOUT_0, IDC_ABOUT_TEXT };

   } // namespace ctrl

   enum { SCREEN_IDLING = 0, SCREEN_WHISPER, SCREEN_TAG, SCREEN_BLACKLIST,
          SCREEN_COMMANDS, SCREEN_MODULE, SCREEN_ABOUT, NUM_SCREENS };

   const int* const screens[ NUM_SCREENS ] = 
   { _ctrl::idling, _ctrl::tag, _ctrl::whisper, _ctrl::blacklist, 
     _ctrl::commands, _ctrl::module, _ctrl::about };

   const char* const screen_names[ NUM_SCREENS ] = 
   { "Idling", "Desc Tag", "Whisper", "Blacklist", "Commands", "Module", "About" };

   const size_t screen_counts[ NUM_SCREENS ] =
   { sizeof( _ctrl::idling ) / sizeof( *_ctrl::idling ),
     sizeof( _ctrl::tag ) / sizeof( *_ctrl::tag ),
     sizeof( _ctrl::whisper ) / sizeof( *_ctrl::whisper ),
     sizeof( _ctrl::blacklist ) / sizeof( *_ctrl::blacklist ),
     sizeof( _ctrl::commands ) / sizeof( *_ctrl::commands ),
     sizeof( _ctrl::module ) / sizeof( *_ctrl::module ),
     sizeof( _ctrl::about ) / sizeof( *_ctrl::about ) };

} // namespace dlg

//////////////////////////////////////////////////////////////////////////

struct RecentWhisper
{
   // Short-name of the recent whisperer.
   string       from_short;
   // time to live
   unsigned int ttl;

   RecentWhisper( const string& from_short, unsigned int ttl = REPLY_RATE )
      : from_short( from_short ), ttl( ttl ) { }
};

struct Profile
{
   string name;
   string awaytag;
   string awaymsg;
};

//////////////////////////////////////////////////////////////////////////

HINSTANCE            g_inst;
unsigned int         g_module;
bool                 g_enabled;
bool                 g_loggedin;
UINT                 g_sync_msg;
HHOOK                g_getmsghook;
HMENU                g_blacklist_menu;
yiffurc::NET_SESSION g_session;
bool                 g_away;
int                  g_desctag;
int                  g_locked;

HWND                g_dlg;
list<RecentWhisper> g_whispers;
unsigned int        g_keepalive_countdown;
unsigned int        g_update_countdown;
time_t              g_last_action;
time_t              g_login_time;
time_t              g_autoidle     = DEFAULT_AUTOIDLE;
string              g_awaytag      = DEFAULT_AWAYTAG;
string              g_awaymsg      = DEFAULT_AWAYMSG;
string              g_idlemsg      = DEFAULT_IDLEMSG;
int                 g_opts         = DEFAULT_OPTS;
list<string>        g_blacklist;
list<string>        g_cmds_enter;
list<string>        g_cmds_exit;
Profile             g_profile;

//////////////////////////////////////////////////////////////////////////

void     set_away( bool onoff );
void     update_desctag( );
void     on_activity( );
intptr_t on_modmsg( const string& msg );
void     keep_alive( );
void     read_opts( );
void     write_opts( );
void     show_screen( );
string   make_profile_filename( const string& profile );
INT_PTR CALLBACK dlg_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
LRESULT CALLBACK getmsg_proc( int code, WPARAM wParam, LPARAM lParam );

//////////////////////////////////////////////////////////////////////////

F2EXPORT intptr_t F2CALL on_module( Module_Event e, void* param )
{
   /* See definition of Module_Event for all module events. */
   switch (e)
   {
   case MOD_EVENT_START:
      g_module = ((const MODULEINFO*) param)->mod;
      g_sync_msg = RegisterWindowMessage( "IdleHandsSyncMsg" );
      g_blacklist_menu = LoadMenu( g_inst, MAKEINTRESOURCE( IDR_BLACKLIST ) );
      g_cmds_enter.push_back( "echo 'Entering away mode.'" );
      g_cmds_exit.push_back( "echo 'Exiting away mode.'" );
      g_last_action = time( NULL );

      // Set up a windows hook to monitor keystrokes.
      g_getmsghook = SetWindowsHookEx( WH_GETMESSAGE, getmsg_proc, 
                                       NULL, GetCurrentThreadId( ) );

      read_opts( );
      if (g_opts & OPT_AUTOENABLE)
         f2_modenable( g_module, true );

      break;

   case MOD_EVENT_STOP:
      set_away( false );

      UnhookWindowsHookEx( g_getmsghook );

      if (IsWindow( g_dlg )) {
         DestroyWindow( g_dlg ); g_dlg = NULL;
      }

      DestroyMenu( g_blacklist_menu ); g_blacklist_menu = NULL;

      break;

   case MOD_EVENT_TOUCH:
      if (!IsWindow( g_dlg ))
      {
         g_dlg = CreateDialog( g_inst, MAKEINTRESOURCE( IDD_DIALOG1 ), 
                               f2_getwnd( ), dlg_proc );
      }
      ShowWindow( g_dlg, SW_SHOWNORMAL );
      SetForegroundWindow( g_dlg );

      break;

   case MOD_EVENT_ENABLE:
      g_enabled = true;
      // Show the desctag.
      if (g_away)
         update_desctag( );
      break;

   case MOD_EVENT_DISABLE:
      g_enabled = false;
      // Just hide the desctag.
      if (g_away && g_desctag)
      {
         f2_desctag_unreg( g_desctag );
         g_desctag = 0;
      }
      break;

   case MOD_EVENT_MSG:
      return on_modmsg( (const char*) param );
   }

   return 0;
}

F2EXPORT bool F2CALL on_wndproc( WNDPROCPARAMS* params )
{
   /* Only interested in the synchronize message. */
   if (params->msg == g_sync_msg && (HWND) params->wparam != f2_getwnd( ))
   {
      read_opts( );
      params->retval = 0;
      return true;
   }
   return false;
}

F2EXPORT intptr_t F2CALL on_client( Client_Event e, void* param )
{
   if (e == CL_EVENT_TICK)
   {
      const unsigned int dt = (unsigned int) param;

      // Prune recent whispers.
      for (list<RecentWhisper>::iterator i = g_whispers.begin( );
           i != g_whispers.end( ); )
      {
         if (dt >= i->ttl)
            g_whispers.erase( i++ );
         else
         {
            i->ttl -= dt;
            ++i;
         }
      }

      if (g_enabled)
      {
         // Send a keepalive.
         if (dt >= g_keepalive_countdown)
         {
            g_keepalive_countdown = KEEPALIVE_RATE;
            const char* enc = yiffurc::c2s_enc_keepalive( );
            f2_netoutline( enc, strlen( enc ) );
         }
         else
            g_keepalive_countdown -= dt;

         if (g_away)
         {
            // Update the desctag.
            if (dt >= g_update_countdown)
            {
               g_update_countdown = UPDATE_RATE;
               update_desctag( );
            }
            else
               g_update_countdown -= dt;
         }
         // Go away if idle long enough.
         else if (!g_locked && g_autoidle)
         {
            if (g_last_action && time( NULL ) - g_last_action >= g_autoidle)
            {
			   g_idlemsg = DEFAULT_IDLEMSG;
               set_away( true );
            }
         }
      }
   }

   return 0;
}

F2EXPORT intptr_t F2CALL on_world( World_Event e, void* param )
{
   switch (e)
   {
   case WO_EVENT_LOGGEDIN:
      g_login_time = time( NULL );
      g_loggedin = true;
      g_session.level = yiffurc::NET_LEVEL_LOGGEDIN;
      break;   
   case WO_EVENT_DISCONNECTED:
      g_loggedin = false;
      break;
   case WO_EVENT_CONNECTED:
      memset( &g_session, 0, sizeof( g_session ) );
      break;
   case WO_EVENT_PLAYERINFO:
      {
         PLAYERINFO pi;
         f2_myplayerinfo( &pi );
         if (pi.name != g_profile.name)
         {
            // Switch profiles.
            g_profile.name = pi.name;
            read_opts( );
         }
      }
      break;
   }

   return 0;
}

F2EXPORT bool F2CALL on_command( int argc, const char* argv[] )
{
   if (argc > 0)
   {
       if (!_stricmp( argv[ 0 ], "afk" )) //Takes an AFK message/reason, no time.
      {
		// This command is immune to locks.
         if (!g_away || argc > 1)
         {
            g_last_action = time( NULL );          
			//g_last_action = time( NULL ) - util::to_time( argv + 1, argc - 1 );
			int i;
			string temp="";
			for (i=1;i<argc;++i) {
				if (i != 1) temp.append(" ");
				temp.append(argv[i]);
				}
			g_idlemsg = temp;
			if (g_idlemsg == "") g_idlemsg = DEFAULT_IDLEMSG;
            set_away( true );
            update_desctag( );
         }
         else
         {
            g_last_action = time( NULL );
            set_away( false );
         }
	     return true;
	   }
	   else if (!_stricmp( argv[ 0 ], "aff" ))
      {
         // This command is immune to locks.

         if (!g_away || argc > 1)
         {
			g_idlemsg = DEFAULT_IDLEMSG;
            g_last_action = time( NULL ) - util::to_time( argv + 1, argc - 1 );
            set_away( true );
            update_desctag( );
         }
         else
         {
            g_last_action = time( NULL );
            set_away( false );
         }
         return true;
      }
      else if (!_stricmp( argv[ 0 ], "afflock" ))
      {
         ++g_locked;

		 ostringstream tmp; 
         tmp << "Locked. (" << g_locked << ")";           

		 f2_speak(tmp.str().c_str());
         return true;
      }
      else if (!_stricmp( argv[ 0 ], "affunlock" ))
      {
         ostringstream tmp;   

		 if (g_locked > 0)
            --g_locked;

		 if (g_locked > 0) {
		 tmp << "Lock decreased. (" << g_locked << ")";
		 } else {
			 tmp << "Unlocked.";
			}

		 f2_speak(tmp.str().c_str());
         return true;
      }
   }
   return false;
}

F2EXPORT void F2CALL on_inet( Revision rev, const char* line, int from, size_t num )
{
   using namespace yiffurc;

   if (rev != REV_DRAFT)
      return;

   const S2C* cmd = s2c_parse( line, strlen( line ), &g_session );
   if (cmd)
   {
      if (cmd->type == S2C_TYPE_WHISPER)
      {
         const string from_long  = ((S2C_WHISPER*) cmd)->from;
         const string from_short = ((S2C_WHISPER*) cmd)->from_short;

         if (g_enabled && g_profile.awaymsg.length( ))
         {
            // Check if the player is blacklisted.
            bool blacklisted = g_opts & OPT_BLINVERT ? true : false;
            for (list<string>::const_iterator i = g_blacklist.begin( );
                 i != g_blacklist.end( ); ++i)
            {
               if (!_stricmp( util::make_short_name( *i ).c_str( ), from_short.c_str( ) ))
               {
                  blacklisted = !blacklisted;
                  break;
               }
            }

            // Send an autoreply.
            if (blacklisted || g_away)
            {
               // Make sure we haven't already sent a reply to this person recently.
               bool replied = false;
               for (list<RecentWhisper>::const_iterator i = g_whispers.begin( );
                    i != g_whispers.end( ); ++i)
               {
                  if (i->from_short == from_short)
                  {
                     replied = true;
                     break;
                  }
               }

               if (!replied)
               {
                  // Remember him.
                  g_whispers.push_back( RecentWhisper( from_short ) );

                  // Send a reply.
                  const char* enc = 
                     c2s_enc_whisper( ("%" + from_short).c_str( ), 
					 util::tag_substitute( g_profile.awaymsg, time( NULL ) - g_last_action, from_long, g_idlemsg.c_str() ).c_str( ));

                  f2_netoutline( enc, strlen( enc ) );

                  yiffurc_release( enc );
               }
            }
         } // if (g_enabled && g_profile.awaymsg.length( ))
         else if (cmd->type == S2C_TYPE_YOUWHISPER)
         {
            if (g_away)
            {
               string to_short = ((S2C_YOUWHISPER*) cmd)->to_short;

               // Reset the TTL on the recent whisper.
               for (list<RecentWhisper>::iterator i = g_whispers.begin( );
                  i != g_whispers.end( ); ++i)
               {
                  if (i->from_short == to_short)
                  {
                     i->ttl = REPLY_RATE;
                     break;
                  }
               }
            }
         }
      }  // if (cmd)

      yiffurc_release( cmd );
   }
}

void set_away( bool onoff )
{
   if (onoff)
   {
      // Only enter away mode when enabled.
      if (!g_away && g_enabled)
      {
         g_away = true;
         update_desctag( );

         // Run enter commands.
         for (list<string>::const_iterator i = g_cmds_enter.begin( );
              i != g_cmds_enter.end( ); ++i)
         {
            f2_command( i->c_str( ), true );
         }
      }
   }
   else
   {
      // Always alow exiting of away mode.
      if (g_away)
      {
         g_away = false;
         if (g_desctag)
         {
            f2_desctag_unreg( g_desctag );
            g_desctag = 0;
         }
         g_whispers.clear( );

         // Report the time we were away.
         if (g_opts & OPT_REPORTTIMEAWAY)
            f2_speak( util::tag_substitute( "You were away for %d days, %h hours, %m minutes, and %s seconds.", 
                                            time( NULL ) - g_last_action ).c_str( ) );

         // Run exit commands.
         for (list<string>::const_iterator i = g_cmds_exit.begin( );
              i != g_cmds_exit.end( ); ++i)
         {
            f2_command( i->c_str( ), true );
         }
      }
   }
}

void on_activity( )
{
   if (!g_locked)
   {
      set_away( false );
      g_last_action = time( NULL );
   }
}

void update_desctag( )
{
   if (g_away && g_enabled && g_profile.awaytag.length( ))
   {
      if (!g_desctag)
         g_desctag = f2_desctag_reg( );
      f2_desctag_set( g_desctag, 
		  util::tag_substitute( g_profile.awaytag, time( NULL ) - g_last_action, "", g_idlemsg.c_str() ).c_str( ) );
   }
}

void write_opts( )
{
   const string root = f2_moddocroot( g_module );

   /* options.xml */
   {
      string path = root + "\\options.xml";
         
      HANDLE hf = CreateFile( path.c_str( ), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
      if (hf == INVALID_HANDLE_VALUE)
         f2_errspeak( "Idle Hands: Couldn't write options file." );
      else
      {
         ostringstream ss;
         ss << "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\r\n";
         ss << "<options>\r\n";
         ss << "\t" << "<autoEnable>" << (g_opts & OPT_AUTOENABLE ? "true" : "false") << "</autoEnable>\r\n";
         ss << "\t" << "<reportTimeAway>" << (g_opts & OPT_REPORTTIMEAWAY ? "true" : "false") << "</reportTimeAway>\r\n";
         ss << "\t" << "<blInvert>" << (g_opts & OPT_BLINVERT ? "true" : "false") << "</blInvert>\r\n";
         ss << "\t" << "<autoIdle>" << g_autoidle << "</autoIdle>\r\n";
         ss << "\t" << "<awayTag>" << util::to_xml_friendly( g_awaytag ) << "</awayTag>\r\n";
         ss << "\t" << "<awayMsg>" << util::to_xml_friendly( g_awaymsg ) << "</awayMsg>\r\n";
         ss << "</options>\r\n";
         
         {
            string str = ss.str( );
            DWORD written;
            WriteFile( hf, str.c_str( ), str.length( ), &written, NULL );
         }
         CloseHandle( hf );
      }
   }

   /* profile */
   if (!g_profile.name.empty( ))
   {
      CreateDirectory( (root + "\\profiles").c_str( ), NULL );

      string path = root + "\\profiles\\" + make_profile_filename( g_profile.name );
      HANDLE hf = CreateFile( path.c_str( ), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
      if (hf == INVALID_HANDLE_VALUE)
         f2_errspeak( "Idle Hands: Couldn't write profile file." );
      else
      {
         ostringstream ss;
         ss << "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\r\n";
         ss << "<options>\r\n";
         ss << "\t" << "<awayTag>" << util::to_xml_friendly( g_profile.awaytag ) << "</awayTag>\r\n";
         ss << "\t" << "<awayMsg>" << util::to_xml_friendly( g_profile.awaymsg ) << "</awayMsg>\r\n";
         ss << "</options>\r\n";

         {
            string str = ss.str( );
            DWORD written;
            WriteFile( hf, str.c_str( ), str.length( ), &written, NULL );
         }
         CloseHandle( hf );
      }

   }

   /* blacklist.txt */
   {
      string path = root + "\\blacklist.txt";
      HANDLE hf = CreateFile( path.c_str( ), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
      if (hf == INVALID_HANDLE_VALUE)
         f2_errspeak( "Idle Hands: Couldn't write blacklist file." );
      else
      {
         ostringstream ss;
         for (list<string>::const_iterator i = g_blacklist.begin( );
              i != g_blacklist.end( ); ++i)
            ss << *i << "\r\n";
         {
            string str = ss.str( );
            DWORD written;
            WriteFile( hf, str.c_str( ), str.length( ), &written, NULL );
         }
         CloseHandle( hf );
      }
   }

   /* enter.txt */
   {
      string path = root + "\\enter.txt";
   
      HANDLE hf = CreateFile( path.c_str( ), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
      if (hf == INVALID_HANDLE_VALUE)
         f2_errspeak( "Idle Hands: Couldn't write enter commands." );
      else
      {
         ostringstream ss;
         for (list<string>::const_iterator i = g_cmds_enter.begin( );
            i != g_cmds_enter.end( ); ++i)
            ss << *i << "\r\n";
         {
            string str = ss.str( );
            DWORD written;
            WriteFile( hf, str.c_str( ), str.length( ), &written, NULL );
         }
         CloseHandle( hf );
      }
   }

   /* exit.txt */
   {
      string path = root + "\\exit.txt";
      HANDLE hf = CreateFile( path.c_str( ), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
      if (hf == INVALID_HANDLE_VALUE)
         f2_errspeak( "Idle Hands: Couldn't write exit commands." );
      else
      {
         ostringstream ss;
         for (list<string>::const_iterator i = g_cmds_exit.begin( );
            i != g_cmds_exit.end( ); ++i)
            ss << *i << "\r\n";
         {
            string str = ss.str( );
            DWORD written;
            WriteFile( hf, str.c_str( ), str.length( ), &written, NULL );
         }
         CloseHandle( hf );
      }
   }

   // Synchronize all instances.
   {
      DWORD recips = BSM_APPLICATIONS;
      BroadcastSystemMessage( BSF_POSTMESSAGE | BSF_IGNORECURRENTTASK , 
                              &recips, g_sync_msg, (WPARAM) f2_getwnd( ), 0 );
   }
}

void read_opts( )
{
   const string root = f2_moddocroot( g_module );

   /* options.xml */
   TiXmlDocument doc( root + "\\options.xml" );
   if (!doc.LoadFile( ) || doc.Error( ))
   {
      if (!doc.Error( ) && INVALID_FILE_ATTRIBUTES != GetFileAttributes( (root + "\\options.xml").c_str( ) ))
      {
         // Try again.
         PostMessage( f2_getwnd( ), g_sync_msg, 0, 0 );
         return;
         //f2_errlog( "Idle Hands: options.xml file is damaged or inaccessible." );
      }
   }
   else
   {
      /* Initialize to defaults. */
      g_autoidle = DEFAULT_AUTOIDLE;
      g_awaytag  = DEFAULT_AWAYTAG;
      g_awaymsg  = DEFAULT_AWAYMSG;
      g_opts     = DEFAULT_OPTS;

      const TiXmlElement* e_root = doc.RootElement( );
      if (e_root && e_root->ValueStr( ) == "options")
      {
         const TiXmlElement* e;

         if ((e = e_root->FirstChildElement( "autoIdle" )) && e->GetText( ))
            g_autoidle = atol( e->GetText( ) );

         if ((e = e_root->FirstChildElement( "reportTimeAway" )) && 
             e->GetText( ) && strcmp( e->GetText( ), "false" ))
            g_opts |= OPT_REPORTTIMEAWAY;
         else
            g_opts &= ~OPT_REPORTTIMEAWAY;

         if ((e = e_root->FirstChildElement( "autoEnable" )) && 
             e->GetText( ) && strcmp( e->GetText( ), "false" ))
            g_opts |= OPT_AUTOENABLE;
         else
            g_opts &= ~OPT_AUTOENABLE;

         if ((e = e_root->FirstChildElement( "blInvert" )) && 
             e->GetText( ) && strcmp( e->GetText( ), "false" ))
            g_opts |= OPT_BLINVERT;
         else
            g_opts &= ~OPT_BLINVERT;
         
         // This can be empty.
         if ((e = e_root->FirstChildElement( "awayTag" )))
         {
            if (e->GetText( ))
               g_awaytag = e->GetText( );
            else
               g_awaytag.clear( );
         }

         // This can be empty.
         if ((e = e_root->FirstChildElement( "awayMsg" )))
         {
            if (e->GetText( ))
               g_awaymsg = e->GetText( );
            else
               g_awaymsg.clear( );
         }
      } // if (e_root && e_root->ValueStr( ) == "options")
   }

   g_profile.awaymsg = g_awaymsg;
   g_profile.awaytag = g_awaytag;

   /* Profile overrides. */
   if (!g_profile.name.empty( ))
   {
      if (doc.LoadFile( root + "\\profiles\\" + make_profile_filename( g_profile.name ) ) && !doc.Error( ))
      {
         const TiXmlElement* e_root = doc.RootElement( );
         if (e_root && e_root->ValueStr( ) == "options")
         {
            const TiXmlElement* e;

            // This can be empty.
            if ((e = e_root->FirstChildElement( "awayTag" )))
            {
               if (e->GetText( ))
                  g_profile.awaytag = e->GetText( );
               else
                  g_profile.awaytag.clear( );
            }

            // This can be empty.
            if ((e = e_root->FirstChildElement( "awayMsg" )))
            {
               if (e->GetText( ))
                  g_profile.awaymsg = e->GetText( );
               else
                  g_profile.awaymsg.clear( );
            }            
         }
      }
   }

   /* blacklist.txt */
   string line;
   ifstream fs( (root + "\\blacklist.txt").c_str( ) );
   if (fs.fail( ))
   {
      if (INVALID_FILE_ATTRIBUTES != GetFileAttributes( (root + "\\blacklist.txt").c_str( ) ))
         f2_errspeak( "Idle Hands: blacklist.txt is inaccessible." );
   }
   else
   {
      g_blacklist.clear( );

      while (fs.good( ))
      {
         getline( fs, line );
         if (line.length( ))
            g_blacklist.push_back( util::to_name( line ) );
      }

      fs.close( );
   }

   /* enter.txt */
   fs.clear( );
   fs.open( (root + "\\enter.txt").c_str( ) );
   if (fs.fail( ))
   {
      if (INVALID_FILE_ATTRIBUTES != GetFileAttributes( (root + "\\enter.txt").c_str( ) ))
         f2_errspeak( "Idle Hands: enter.txt is inaccessible." );
   }
   else
   {
      g_cmds_enter.clear( );

      while (fs.good( ))
      {
         getline( fs, line );
         if (line.length( ))
         {
            // Remove @ prefixes.
            if (line.find( "@" ) == 0) 
               line = line.substr( 1, line.npos );
            if (line.length( ))
               g_cmds_enter.push_back( line );
         }
      }

      fs.close( );
   }

   /* exit.txt */
   fs.clear( );
   fs.open( (root + "\\exit.txt").c_str( ) );
   if (fs.fail( ))
   {
      if (INVALID_FILE_ATTRIBUTES != GetFileAttributes( (root + "\\exit.txt").c_str( ) ))
         f2_errspeak( "Idle Hands: exit.txt is inaccessible." );
   }
   else
   {
      g_cmds_exit.clear( );

      while (fs.good( ))
      {
         getline( fs, line );
         if (line.length( ))
         {
            // Remove @ prefixes.
            if (line.find( "@" ) == 0) 
               line = line.substr( 1, line.npos );
            if (line.length( ))
               g_cmds_exit.push_back( line );
         }
      }

      fs.close( );
   }
}

void show_screen( )
{
   assert( IsWindow( g_dlg ) );

   /* Display the current screen. */
   size_t curr_screen = (size_t) SendDlgItemMessage( g_dlg, IDC_SCREENS, LB_GETCURSEL, 0, 0 );
   curr_screen = (curr_screen >= dlg::NUM_SCREENS ? 0 : curr_screen);
   for (size_t i = 0; i < dlg::NUM_SCREENS; ++i)
   {
      if (i != curr_screen)
      {
         for (size_t j = 0; j < dlg::screen_counts[ i ]; ++j)
            ShowWindow( GetDlgItem( g_dlg, dlg::screens[ i ][ j ] ), SW_HIDE );
      }
      else
      {
         for (size_t j = 0; j < dlg::screen_counts[ i ]; ++j)
            ShowWindow( GetDlgItem( g_dlg, dlg::screens[ i ][ j ] ), SW_SHOWNA );
      }
   }
}

string make_profile_filename( const string& name )
{
   char* sz_buf = new char[ name.length( ) + 1 ];
   furn2::f2_makeshortname( name.c_str( ), sz_buf );
   string r( sz_buf );
   r += ".xml";
   delete [] sz_buf;
   return r;
}

int on_modmsg( const string& msg )
{
   if (msg == "state")
      return g_away;
   else if (!msg.compare( 0, 8, "awaymsg " ))
   {
      g_profile.awaymsg.assign( msg, 8, msg.length( ) - 8 );
      write_opts( );
      return true;
   }
   else if (!msg.compare( 0, 8, "awaytag " ))
   {
      g_profile.awaytag.assign( msg, 8, msg.length( ) - 8 );
      write_opts( );
      return true;
   }

   return 0;
}

LRESULT CALLBACK getmsg_proc( int code, WPARAM wparam, LPARAM lparam )
{
   if (wparam == PM_REMOVE)
   {
      const MSG* msg = (const MSG*) lparam;
      if (msg->message == WM_KEYDOWN)
      {
         on_activity( );
      }
   }

   return CallNextHookEx( g_getmsghook, code, wparam, lparam );
}

INT_PTR CALLBACK dlg_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
   switch (msg)
   {
   case WM_INITDIALOG:
      {
         g_dlg = wnd;

         /* Center inside the parent window. */
         {
            HWND parent = GetParent( wnd );
            RECT parent_rect;
            GetClientRect( parent, &parent_rect );
            RECT dlg_rect;
            GetWindowRect( wnd, &dlg_rect );
            dlg_rect.right = dlg_rect.right - dlg_rect.left;
            dlg_rect.bottom = dlg_rect.bottom - dlg_rect.top;
            POINT pt = { 
                         pt.x = (parent_rect.right - dlg_rect.right) / 2,
                         pt.y = (parent_rect.bottom - dlg_rect.bottom) / 2
                       };
            ClientToScreen( parent, &pt );
            SetWindowPos( wnd, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
         }

         /* Populate the controls. */

         // Version.
         SetDlgItemText( wnd, IDC_VERSION, "Idle Hands " IDLEHANDS_VERSION );

         // Flags.
         SetDlgItemInt( wnd, IDC_IDLING_AUTOIDLE, (UINT) g_autoidle, FALSE );
         CheckDlgButton( wnd, IDC_IDLING_REPORT, g_opts & OPT_REPORTTIMEAWAY ? BST_CHECKED : BST_UNCHECKED );
         CheckDlgButton( wnd, IDC_BLACKLIST_INVERT, g_opts & OPT_BLINVERT ? BST_CHECKED : BST_UNCHECKED );
         CheckDlgButton( wnd, IDC_MODULE_AUTOENABLE, g_opts & OPT_AUTOENABLE ? BST_CHECKED : BST_UNCHECKED );

         // Tag & whisper.
         SetDlgItemText( wnd, IDC_TAG_TAG, g_profile.awaytag.c_str( ) );
         SetDlgItemText( wnd, IDC_WHISPER_WHISPER, g_profile.awaymsg.c_str( ) );

         // Blacklist.
         {
            HWND lv = GetDlgItem( wnd, IDC_BLACKLIST_BLACKLIST );
            // Add a single column to the control.
            {
               LVCOLUMN col;
               col.mask     = LVCF_SUBITEM;
               col.iSubItem = 0;
               ListView_InsertColumn( lv, 0, &col );
               RECT r;
               GetClientRect( lv, &r );
               ListView_SetColumnWidth( lv, 0, r.right );
            }

            LVITEM item;
            item.mask = LVIF_TEXT;
            item.iSubItem = 0;
            item.iItem = 0;
            for (list<string>::const_iterator i = g_blacklist.begin( );
                 i != g_blacklist.end( ); ++i)
            {
               item.pszText = const_cast<char*>( i->c_str( ) );
               ListView_InsertItem( lv, &item );
            }
         }
         {
            string text;
            for (list<string>::const_iterator i = g_cmds_enter.begin( );
                 i != g_cmds_enter.end( ); ++i)
            {
              if (text.length( ))
                 text += "\r\n";
               text += *i;
            }
            SetDlgItemText( wnd, IDC_COMMANDS_ENTER, text.c_str( ) );

            text.clear( );
            for (list<string>::const_iterator i = g_cmds_exit.begin( );
               i != g_cmds_exit.end( ); ++i)
            {
               if (text.length( ))
                  text += "\r\n";
               text += *i;
            }
            SetDlgItemText( wnd, IDC_COMMANDS_EXIT, text.c_str( ) );
         }
         {
            ifstream fs( (string( f2_modroot( g_module ) ) +  "\\about.txt").c_str( ),
                         ios::binary );
            fs.unsetf( ios_base::skipws );
            if (fs.good( ))
            {
               string str;
               str.assign( istream_iterator<char>( fs ), istream_iterator<char>( ) );
               SetDlgItemText( wnd, IDC_ABOUT_TEXT, str.c_str( ) );
               fs.close( );
            }

            GetDlgItem( wnd, IDC_ABOUT_TEXT );
         }

         /* Populate the screens list. */
         for (size_t i = 0; i < dlg::NUM_SCREENS; ++i)
            SendDlgItemMessage( wnd, IDC_SCREENS, LB_ADDSTRING, 
                                0, (LPARAM) dlg::screen_names[ i ] );
         /* Select the first item. */
         SendDlgItemMessage( wnd, IDC_SCREENS, LB_SETCURSEL, 0, 0 );
         show_screen( );
      }
      return TRUE;

   case WM_ACTIVATE:
      {
         // Set the current dialog when active to get fancy
         // default message processing.
         if (wparam != WA_INACTIVE)
            f2_setcurdialog( wnd );
      }
      break;

   case WM_CTLCOLORSTATIC:
      // I like white on static edit controls.
      if ((HWND) lparam == GetDlgItem( wnd, IDC_ABOUT_TEXT ))
      {
         SetBkMode( (HDC) wparam, TRANSPARENT );
         return (INT_PTR) GetSysColorBrush( COLOR_WINDOW );
      }
      break;

   case WM_COMMAND:
      if (lparam)
      {
         switch (LOWORD( wparam ))
         {
         case IDC_SCREENS:
            if (HIWORD( wparam ) == LBN_SELCHANGE)
            {
               show_screen( );
               return TRUE;
            }
            break;

         case IDOK:
            if (HIWORD( wparam ) == BN_CLICKED)
            {
               /* Retrieve options. */

               /* Various flags. */
               g_autoidle = GetDlgItemInt( wnd, IDC_IDLING_AUTOIDLE, NULL, FALSE );
               g_opts = 0;
               g_opts |= IsDlgButtonChecked( wnd, IDC_IDLING_REPORT ) ? OPT_REPORTTIMEAWAY : 0;
               g_opts |= IsDlgButtonChecked( wnd, IDC_MODULE_AUTOENABLE ) ? OPT_AUTOENABLE : 0;
               g_opts |= IsDlgButtonChecked( wnd, IDC_BLACKLIST_INVERT ) ? OPT_BLINVERT : 0;

               /* Away tag. */
               {
                  const size_t size = GetWindowTextLength( GetDlgItem( wnd, IDC_TAG_TAG ) ) + 1;
                  char* buf = new char[ size ];
                  GetDlgItemText( wnd, IDC_TAG_TAG, buf, size );
                  g_profile.awaytag = util::to_readable( buf );
                  delete [] buf;
               }

               /* Away whisper. */
               {
                  const size_t size = GetWindowTextLength( GetDlgItem( wnd, IDC_WHISPER_WHISPER ) ) + 1;
                  char* buf = new char[ size ];
                  GetDlgItemText( wnd, IDC_WHISPER_WHISPER, buf, size );
                  g_profile.awaymsg = util::to_readable( buf );
                  delete [] buf;
               }

               /* Blacklist. */
               {
                  g_blacklist.clear( );
                  HWND lv = GetDlgItem( wnd, IDC_BLACKLIST_BLACKLIST );
                  const int n = ListView_GetItemCount( lv );
                  char* buf = new char[ 64 ];
                  for (int i = 0; i < n; ++i)
                  {
                     ListView_GetItemText( lv, i, 0, buf, 64 );
                     if (*buf)
                        g_blacklist.push_back( util::to_name( buf ) );
                  }
                  delete [] buf;
               }

               /* Enter commands. */
               {
                  g_cmds_enter.clear( );

                  size_t size = GetWindowTextLength( GetDlgItem( wnd, IDC_COMMANDS_ENTER ) ) + 1;
                  char* buf = new char[ size ];
                  GetDlgItemText( wnd, IDC_COMMANDS_ENTER, buf, size );
                  if (*buf)
                  {
                     istringstream ss( buf );
                     string line;
                     while (ss.good( ))
                     {
                        getline( ss, line, '\r' );
                        ss.ignore( 1 );
                        if (line.length( ))
                        {
                           // Strip '@' prefixes.
                           if (line[ 0 ] == '@')
                              g_cmds_enter.push_back( line.substr( 1, line.npos ) );
                           else
                           g_cmds_enter.push_back( line );
                        }
                     }
                  }
                  delete [] buf;
               }

               /* Exit commands. */
               {
                  g_cmds_exit.clear( );

                  size_t size = GetWindowTextLength( GetDlgItem( wnd, IDC_COMMANDS_EXIT ) ) + 1;
                  char* buf = new char[ size ];
                  GetDlgItemText( wnd, IDC_COMMANDS_EXIT, buf, size );
                  if (*buf)
                  {
                     istringstream ss( buf );
                     string line;
                     while (ss.good( ))
                     {
                        getline( ss, line, '\r' );
                        ss.ignore( 1 );
                        if (line.length( ))
                        {
                           // Strip '@' prefixes.
                           if (line[ 0 ] == '@')
                              g_cmds_exit.push_back( line.substr( 1, line.npos ) );
                           else
                              g_cmds_exit.push_back( line );
                        }
                     }
                  }
                  delete [] buf;
               }

               write_opts( );

               DestroyWindow( wnd );
               return TRUE;
            }
            break;

         case IDCANCEL:
            if (HIWORD( wparam ) == BN_CLICKED)
            {
               DestroyWindow( wnd );
               return TRUE;
            }
            break;
         }
      } // if (lparam)
      else if (HIWORD( wparam ) == 0)
      {
         switch (LOWORD( wparam ))
         {
         case ID_BLACKLIST_REMOVE:
            {
               HWND lv = GetDlgItem( wnd, IDC_BLACKLIST_BLACKLIST );
               
               while (ListView_GetSelectedCount( lv ))
               {
                  const int n = ListView_GetItemCount( lv );
                  for (int i = 0; i < n; ++i)
                  {
                     if (ListView_GetItemState( lv, i, LVIS_SELECTED ))
                     {
                        ListView_DeleteItem( lv, i );
                        break;
                     }
                  }
               }
            }
            return TRUE;

         case ID_BLACKLIST_ADD:
            {
               HWND lv = GetDlgItem( wnd, IDC_BLACKLIST_BLACKLIST );

               LVITEM item;
               item.mask     = LVIF_TEXT;
               item.iItem    = 0;
               item.iSubItem = 0;
               item.pszText  = "";

               ListView_EditLabel( lv, ListView_InsertItem( lv, &item ) );               
            }
            return TRUE;

         case ID_BLACKLIST_CLEAR:
            {
               HWND lv = GetDlgItem( wnd, IDC_BLACKLIST_BLACKLIST );

               ListView_DeleteAllItems( lv );
            }
            return TRUE;
         }
      }
      break;

   case WM_NOTIFY:
      if (((NMHDR*) lparam)->idFrom == IDC_BLACKLIST_BLACKLIST)
      {
         HWND lv = GetDlgItem( wnd, IDC_BLACKLIST_BLACKLIST );

         switch (((NMHDR*)lparam)->code)
         {
         case NM_RCLICK:
            {
               HMENU menu = GetSubMenu( g_blacklist_menu, 0 );
               if (ListView_GetSelectedCount( lv ))
               {
                  EnableMenuItem( menu, ID_BLACKLIST_REMOVE, MF_ENABLED );
                  EnableMenuItem( menu, ID_BLACKLIST_CLEAR, MF_ENABLED );
               }
               else
               {
                  EnableMenuItem( menu, ID_BLACKLIST_REMOVE, MF_GRAYED );
                  if (ListView_GetItemCount( lv ))
                     EnableMenuItem( menu, ID_BLACKLIST_CLEAR, MF_ENABLED );
                  else
                     EnableMenuItem( menu, ID_BLACKLIST_CLEAR, MF_GRAYED );
               }

               POINT cursor;
               GetCursorPos( &cursor );
               TrackPopupMenu( menu, TPM_RIGHTBUTTON, cursor.x, cursor.y, 0,
                               wnd, NULL );

               SetWindowLongPtr( wnd, DWLP_MSGRESULT, 0 );
               return TRUE;
            }
            break;

         case LVN_ENDLABELEDIT:
            {
               NMLVDISPINFO* nm = (NMLVDISPINFO*) lparam;
               if (nm->item.pszText && *nm->item.pszText)
               {
                  strcpy_s( nm->item.pszText, nm->item.cchTextMax,
                            util::to_name( nm->item.pszText ).c_str( ) );
                  SetWindowLongPtr( wnd, DWLP_MSGRESULT, TRUE );
                  return TRUE;
               }
               ListView_DeleteItem( lv, nm->item.iItem );
               SetWindowLongPtr( wnd, DWLP_MSGRESULT, FALSE );
               return TRUE;
            }
            break;
         }
      }
      break;

   case WM_DESTROY:
      g_dlg = NULL;
      break;
   }

   return FALSE;
}

} // namespace idlehands

BOOL WINAPI DllMain( HINSTANCE inst, DWORD reason, LPVOID _reserved )
{
   if (reason == DLL_PROCESS_ATTACH)
   {
      idlehands::g_inst = inst;
      DisableThreadLibraryCalls( inst );
   }
   return TRUE;
}