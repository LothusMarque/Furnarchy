/************************************************************************/
/* offend.cpp - A YIFFurc Library example. (Public Domain)              */
/* A console app that logs a character into Furcadia, teleports to      */
/* Meovanni, goes to the Wisdom Tree, then offends Jag with depth of    */
/* feeling.                                                             */
/************************************************************************/

#include "yiffurc.h"   // Modify this to wherever yiffurc.h is located.

#ifdef _WIN32
   #define WIN32_LEAN_AND_MEAN
   #include <winsock.h> // Depends on ws2_32.lib
   #include <conio.h>
#else // POSIX
   #include <netinet/in.h>
   #include <arpa/inet.h>
   #include <sys/select.h>
   #include <netdb.h>
   #include <errno.h>
   #include <unistd.h>
   #include <termios.h>
#endif

#include <iostream>
#include <cstring>
#include <string>
#include <ctime>
#include <errno.h>
#include <cassert>

using namespace std;
using namespace yiffurc;

//////////////////////////////////////////////////////////////////////////

enum { CONNECT_RETRIES = 10 };

//////////////////////////////////////////////////////////////////////////

bool        g_game_connected;
string      g_linebuf;
NET_SESSION g_session;
int         g_sockfd = -1;

//////////////////////////////////////////////////////////////////////////

bool   game_connect( const string& server, unsigned short port );
bool   game_disconnect( );
bool   game_run( const string& name = "", const string& pw = "" );
void   send_line( const string& line );
bool   recv_line( );
size_t _getline( string* out, bool mask  = false );

int main( int argc, char* argv[] )
{
   #ifdef _WIN32
   // Windows requires initialization of winsock.
   {
      WSADATA data;
      WSAStartup( 0x0202, &data );
   }
   #endif
   
   if (game_connect( "lightbringer.furcadia.com", 80 ))
   {
      game_run( );
      game_disconnect( );
   }
   
   #ifdef _WIN32
   WSACleanup( );
   #endif
   
   return EXIT_SUCCESS;
}

bool game_connect( const string& server, unsigned short port )
{
   /* Create the socket. */
   g_sockfd = socket( PF_INET, SOCK_STREAM, 0 );
   
   /* Resolve the server address. */
   sockaddr_in dst;
   memset( &dst, 0, sizeof( dst ) );
   dst.sin_port   = htons( port );
   dst.sin_family = AF_INET;
   {
      const hostent* ent = gethostbyname( server.c_str( ) );
      if (!ent)
      {
         cerr << "Failed to resolve server name: " << server << '\n';
         return false;
      }
      #ifdef _WIN32
      dst.sin_addr.S_un.S_addr = *((int**) ent->h_addr_list)[ 0 ];
      #else // POSIX
      dst.sin_addr.s_addr = *((int**) ent->h_addr_list)[ 0 ];
      #endif
   }
   
   /* Connect. */
   for (int retry = 0; retry < CONNECT_RETRIES; ++retry)
   {
      cout << "Connecting to " << server << " on port " << port << "...\n";
      int err = connect( g_sockfd, (sockaddr*) &dst, sizeof( dst ) );
      
      if (err < 0)
      {
         cout << "Couldn't connect to " << server << " on port " << port
              << ". " << strerror( errno ) << '\n';
         cout << "Sleeping for 10 seconds and retrying...\n";
         
         timeval tv = { 10, 0 };
         select( 0, 0, 0, 0, &tv );         
      }
      else
      {
         g_game_connected = true;
         cout << "Connected to " << server << " on port " << port << ".\n";
         return true;
      }
   }

   cout << "Max retries (" << CONNECT_RETRIES << ") reached.  Giving up.\n";
   return false;
}

bool game_disconnect( )
{
   if (g_sockfd != -1)
   {
      // c2s_enc_quit returns a stock string, so no need to release it.
      send_line( c2s_enc_quit( ) );
      
      #ifdef _WIN32
      closesocket( g_sockfd );
      #else
      close( g_sockfd );
      #endif
      
      g_sockfd = -1;
   }
   
   if (g_game_connected)
   {
      cout << "Disconnected.\n";
      g_game_connected = false;
   }
   
   memset( &g_session, 0, sizeof( g_session ) );
   
   return true;
}

bool game_run( const string& name, const string& pw )
{
   /* Wait for the MOTD to pass. */
   cout << "Receiving MOTD...\n";
   while (g_session.level < NET_LEVEL_MOTDEND)
   {
      if (!recv_line( ))
         return false;
      // Just to keep the network level updated.
      yiffurc_release( s2c_parse( g_linebuf.c_str( ), 
                                  g_linebuf.length( ), &g_session ) );
   }
   cout << "Received MOTD.\n";
   
   /* Prompt for name and password. */
   string _name = name, _pw = pw;
   
   while (!_name.length( ))
   {
      cout << "Player: ";
      _getline( &_name );
   }
   
   while (!_pw.length( ))
   {
      cout << "Password: ";
      _getline( &_pw, true );
   }
   
   /* Log in. */
   cout << "Logging in as " << _name << "...\n";
   {
      // These are largely phony values, as retrieving the proper values of
      // each field is mostly a platform-dependent thing.
      // Check the authentication documentation for what they should be.
      AUTH_MACHINEVARS mac_vars;
      mac_vars.h_e_z               = 5817123;
      mac_vars.num_drives          = 2;
      mac_vars.volume_sn[ 0 ]      = 0xDEADBEEF;
      mac_vars.volume_sn[ 1 ]      = 0x1337FEA7;
      mac_vars.total_free_clusters = 0x777483;
      mac_vars.time                = time( 0 );
      mac_vars.screen_width        = 1280;
      mac_vars.screen_height       = 960;
      mac_vars.version.major       = 22;
      mac_vars.version.minor       = 4;
      mac_vars.ip4                 = inet_addr( "192.168.0.1" );
      
      const char* sz_macid = auth_stringclient( &mac_vars, true );
      const char* sz_login = c2s_enc_login( _name.c_str( ), _pw.c_str( ),
                                            sz_macid );
      yiffurc_release( sz_macid );
      
      send_line( sz_login );
      yiffurc_release( sz_login );
   }
   
   /* Wait for success. */
   cout << "Waiting for login response...\n";
   while (g_session.level < NET_LEVEL_LOGGEDIN)
   {
      if (!recv_line( )) // The server will disconnect us if the login is wrong.
         return false;
      
      const S2C* s2c = s2c_parse( g_linebuf.c_str( ), g_linebuf.length( ), 
                                  &g_session );
      if (s2c)
         yiffurc_release( s2c );
   }
   
   cout << "Logged in!\n";
   
   /* Send color and desc data. */
   {
      const char* sz_cmd = c2s_enc_setcolor( "!!G5G 8 8 ", "!#!" );
      send_line( sz_cmd );
      yiffurc_release( sz_cmd );
      
      sz_cmd = c2s_enc_setdesc( "JAG EATS" );
      send_line( sz_cmd );
      yiffurc_release( sz_cmd );
   }
   
   cout << "Waiting for map...\n";

   // Wait for the map to load.
   while (true)
   {
      if (!recv_line( ))
         return false;
      
      const S2C* s2c = s2c_parse( g_linebuf.c_str( ), g_linebuf.length( ),
                                  &g_session );
      
      if (s2c && (s2c->type == S2C_TYPE_USEDREAM || 
                  s2c->type == S2C_TYPE_USEMAPFILE))
      {
         yiffurc_release( s2c );
         break;
      }
      
      yiffurc_release( s2c );
   }
   
   cout << "Going to Meovanni...\n";
   
   /* Go to Meovanni. */
   {
      const char* sz_cmd = c2s_enc_gomap( GOMAP_MEOVANNI );
      send_line( sz_cmd );
      yiffurc_release( sz_cmd );
   }
   
   // Wait for the map to load.
   while (true)
   {
      if (!recv_line( ))
         return false;
      
      const S2C* s2c = s2c_parse( g_linebuf.c_str( ), g_linebuf.length( ),
                                  &g_session );
      
      if (s2c && (s2c->type == S2C_TYPE_USEDREAM || 
                  s2c->type == S2C_TYPE_USEMAPFILE))
      {
         // In case this is a custom dream, tell the server the download
         // finished so it'll let us move around.
         send_line( c2s_enc_downloadfinished( ) );
         yiffurc_release( s2c );
         break;
      }
      
      yiffurc_release( s2c );
   }
   
   cout << "In Meovanni.\n";
   cout << "Going to the Wisdom Tree...\n";
   sleep( 3 );
   
   /* Exploit the map's 'use' DS to get to the wisdom tree. */
   {
      // c2s_enc_use( ) returns a stock string so there's no need to 
      // release it.
      send_line( c2s_enc_use( ) );
      
      // Wait for the emit text.
      while (true)
      {
         if (!recv_line( ))
            return false;
         
         const S2C* s2c = s2c_parse( g_linebuf.c_str( ), g_linebuf.length( ),
                                     &g_session );
         
         if (s2c && s2c->type == S2C_TYPE_DSEMIT)
         {
            cout << "Received emit text: " << ((const S2C_DSEMIT*) s2c)->message << '\n';
            if (strcmp( ((const S2C_DSEMIT*) s2c)->message, "Wisdom Tree" ) == 0)
            {
               // We've arrived.
               yiffurc_release( s2c );
               break;
            }
            else
               send_line( c2s_enc_use( ) );
         }
         
         yiffurc_release( s2c );
      }
   }
   
   cout << "At the Wisdom Tree.\n";
   cout << "Offending Jag...\n";
   
   // Say something true to hurt Jag's feelings.
   {
      const char* sz_cmd = c2s_enc_speech( "Kylie Minogue has no talent." );
      send_line( sz_cmd );
      yiffurc_release( sz_cmd );
   }
   
   cout << "Jag has been offended.\n";
   
   return true;
}

void send_line( const string& line )
{
   if (!line.empty( ))
   {
      // Should be a singular line.
      assert( line.find_first_of( "\n" ) == line.length( ) - 1 );

      // Parse it first to keep the session object up-to-date.
      const C2S* c2s = c2s_parse( line.c_str( ), line.length( ), &g_session );
      if (c2s)
         yiffurc_release( c2s );

      assert( g_sockfd != -1 );

      // If not linefeed terminated, terminate it.
      if (line.find_first_of( "\n" ) == line.npos)
         send( g_sockfd, (line + "\n").c_str( ), line.length( ) + 1, 0 );
      else
         send( g_sockfd, line.c_str( ), line.length( ), 0 );
   }
}

size_t _getline( string* out, bool mask )
{
   #ifdef _WIN32
   for (int ch = 0; ch = _getch( );)
   {
      if (ch == '\n' || ch == '\r')
      {
         putchar( '\n' );
         break;
      }
      else if (ch == '\x08') // Backspace
      {
         if (out->length( ))
         {
            putchar( '\x08' );
            putchar( ' ' );
            putchar( '\x08' );
            out->erase( out->length( ) - 1 );
         }
      }
      else
      {
         if (mask)
            putchar( '*' );
         else
            putchar( ch );
         out->push_back( ch );
      }
   }
   #else
   termios ios_new, ios_old;
   tcgetattr( STDIN_FILENO, &ios_old );
   ios_new = ios_old;
   ios_new.c_lflag &= ~( ICANON | ECHO );
   tcsetattr( STDIN_FILENO, TCSANOW, &ios_new );
   
   for (int ch = 0; (ch = getchar( ));)
   {
      if (ch == '\n')
      {
         putchar( '\n' );
         break;
      }
      else if (ch == '\x7F') // Backspace
      {
         if (out->length( ))
         {
            putchar( ch );
            out->erase( out->length( ) - 1 );
         }
      }
      else
      {
         if (mask)
            putchar( '*' );
         else
            putchar( ch );
         out->push_back( ch );
      }    
   }
   
   tcsetattr( STDIN_FILENO, TCSANOW, &ios_old );
   #endif
   
   return out->length( );
}

bool recv_line( )
{
   if (g_sockfd == -1 || !g_game_connected)
      return false;
   
   g_linebuf.clear( );
   
   char ch;
   int r;
   while ((r = recv( g_sockfd, &ch, 1, 0 )) == 1)
   {      
      if ((unsigned) ch < (unsigned) ' ')
         return true;
      g_linebuf.push_back( ch );
   }
   
   if (r == 0)
   {
      cout << "The server closed the connection.\n";
      g_game_connected = false;
      return false;
   }
   
   cout << "Recv error. " << strerror( errno ) << '\n';
   return false;
}
