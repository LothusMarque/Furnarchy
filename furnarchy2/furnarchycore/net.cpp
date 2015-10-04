#include "net.h"
#include "core.h"
#include "window.h"
#include "world.h"
#include "common/NetBuffer.h"
#include "common/StringQueue.h"

namespace net {

enum Socks_State { SOCKS4_CONNREQUEST, SOCKS5_GREETING, 
                   SOCKS5_PROXYSELECT, SOCKS5_CONNREQUEST, SOCKS_DONE };
enum { SELECT_WAIT_MS = 500, SEND_CHUNK_SIZE = 256, MAX_OLINE_LEN = 1024*3 };
enum Net_State { NETSTATE_DISCONNECTED, NETSTATE_CONNECTING, 
                 NETSTATE_NEGOTIATING, NETSTATE_CONNECTED };

struct NETLINE 
{ 
   int from; string line; 
   NETLINE( int from, string line ) : from( from ), line( line ) {} 
};

//////////////////////////////////////////////////////////////////////////

bool                 g_initialized = false;
SOCKET               g_socket;
Net_State            g_state;
Socks_State          g_proxy_state;

NetBuffer            g_in_server;
NetBuffer            g_in;
list<NETLINE>        g_in_lines;
size_t               g_num_in_lines;
NetBuffer            g_in_final;

NetBuffer            g_out_client;
NetBuffer            g_out;
list<NETLINE>        g_out_lines;
size_t               g_num_out_lines;
NetBuffer            g_out_final;

//////////////////////////////////////////////////////////////////////////

void flush_bufs( );
void reset_connection( );
void set_state( Net_State state );
int  send_all( );

//////////////////////////////////////////////////////////////////////////

bool initialize( )
{
   if (!g_initialized)
   {
      g_initialized = true;
      reset_connection( );
   }

   return true;
}

bool uninitialize( )
{
   if (g_initialized)
   {
      g_initialized = false;
      reset_connection( );
   }

   return true;
}

void reset_connection( )
{
   flush_bufs( );
   g_socket      = INVALID_SOCKET;
   g_state       = NETSTATE_DISCONNECTED;
   g_proxy_state = SOCKS_DONE;
}

void flush_bufs( )
{
   g_in_server.clear( );
   g_in.clear( );
   g_in_lines.clear( );
   g_num_in_lines = 0;
   g_in_final.clear( );

   g_out_client.clear( );
   g_out.clear( );
   g_out_lines.clear( );
   g_num_out_lines = 0;
   g_out_final.clear( );
}

bool push_inbound( const char* buf, size_t size, int from /* = FROM_OTHER */ )
{
   if (g_state >= NETSTATE_CONNECTED)
   {
      bool jobs = false;

      if (from == FROM_SERVER)
      {
         g_in_server.pushBytes( buf, size );

         while (g_in_server.popNextLine( ))
         {
            g_in_lines.push_back( NETLINE( FROM_SERVER, g_in_server.getLastLine( ) ) );
            jobs = true;
         }
      }
      else
      {
         g_in.pushBytes( buf, size );

         while (g_in.popNextLine( ))
         {
            g_in_lines.push_back( NETLINE( FROM_OTHER, g_in.getLastLine( ) ) );
            jobs = true;
         }
      }

      if (jobs)
         core::batch_add( BATCH_NET );
      return true;
   }

   return false;
}

size_t push_inbound_line( const char* line, size_t len, int from /* = FROM_OTHER */ )
{
   if (len == -1)
      len = strlen( line );

   if (g_state >= NETSTATE_CONNECTED)
   {
      /* Clip to one line. */
      string _line( line, len );
      for (size_t i = 0; i < _line.length( ); ++i)
      {
         if (_line[ i ] == '\n' || _line[ i ] == '\r')
         {
            _line.resize( i );
            break;
         }
      }

      g_in_lines.push_back( NETLINE( from == FROM_SERVER ? FROM_SERVER : FROM_OTHER, _line ) );

      core::batch_add( BATCH_NET );

      return g_num_in_lines + g_in_lines.size( ) - 1;
   }

   return g_num_in_lines + g_in_lines.size( );
}

bool push_outbound( const char* buf, size_t size, int from /* = FROM_OTHER */ )
{
   if (g_state >= NETSTATE_CONNECTED)
   {
      bool jobs = false;

      if (from == FROM_SERVER)
      {
         g_out_client.pushBytes( buf, size );

         while (g_out_client.popNextLine( ))
         {
            g_out_lines.push_back( NETLINE( FROM_CLIENT, g_out_client.getLastLine( ) ) );
            jobs = true;
         }
      }
      else
      {
         g_out.pushBytes( buf, size );

         while (g_out.popNextLine( ))
         {
            g_out_lines.push_back( NETLINE( FROM_OTHER, g_out.getLastLine( ) ) );
            jobs = true;
         }
      }

      if (jobs)
         core::batch_add( BATCH_NET );
      return true;
   }

   return false;
}

size_t push_outbound_line( const char* line, size_t len /* = -1 */, int from /* = FROM_OTHER */ )
{
   if (len == -1)
      len = strlen( line );

   if (g_state >= NETSTATE_CONNECTED)
   {
      /* Clip to one line. */
      string _line( line, len );
      for (size_t i = 0; i < _line.length( ); ++i)
      {
         if (_line[ i ] == '\n' || _line[ i ] == '\r')
         {
            _line.resize( i );
            break;
         }
      }

      g_out_lines.push_back( NETLINE( from == FROM_CLIENT ? FROM_CLIENT: FROM_OTHER, _line ) );

      core::batch_add( BATCH_NET );

      return g_num_out_lines + g_out_lines.size( ) - 1;
   }

   return g_num_out_lines + g_out_lines.size( );
}

bool set_main_socket( SOCKET s )
{ 
   /* Sockets must be in the same state. */
   if (s != g_socket)
   {
      // Restore the old one to non-blocking mode.
      {
         WSAAsyncSelect( g_socket, window::get_handle( ), window::WM_WSAMSG, 0 );
         u_long arg = 0;
         ioctlsocket( g_socket, FIONBIO, &arg );
      }

      g_in_server.clear( );

      g_socket = s;

      // Disable nagling.
      {
         BOOL opt = TRUE;
         setsockopt( g_socket, IPPROTO_TCP, TCP_NODELAY, (const char*) &opt, sizeof( opt ) );
      }

      // Select.
      WSAAsyncSelect( g_socket, window::get_handle( ), window::WM_WSAMSG, FD_READ | FD_CONNECT | FD_CLOSE );

      // Post a read message.
      window::post_message( window::WM_WSAMSG, *(WPARAM*)&g_socket, 
                            MAKELPARAM( FD_READ, 0 ) );
   }

   return true;
}

int on_client_recv( SOCKET s, char* buf, size_t max )
{
   if (s != g_socket)
      return recv( s, buf, (int) max, 0 );

   /* Dealing with main socket. */

   if (g_state == NETSTATE_NEGOTIATING)
   {
      /* Let SOCKS stuff pass through unmolested. */
      if (g_proxy_state != SOCKS_DONE)
      {
         // This assumes it all comes in at once.
         switch (g_proxy_state)
         {
         case SOCKS4_CONNREQUEST:
            g_proxy_state = SOCKS_DONE;
            break;
         case SOCKS5_GREETING:
            g_proxy_state = SOCKS5_PROXYSELECT;
            break;
         case SOCKS5_CONNREQUEST:
            g_proxy_state = SOCKS_DONE;
            break;
         }
         return recv( s, buf, (int) max, 0 );
      }

      // If a recv comes in before a send, the proxy type
      // won't be set, and it also means there is no proxy.
      set_state( NETSTATE_CONNECTED );
   }

   static char recv_buf[ 512 ];  // Ah, the singleton life.

   /* Fetch all the data. */
   int r = 0;
   do 
   {
      r = recv( s, recv_buf, 512, 0 );

      if (r > 0)
         push_inbound( recv_buf, r, FROM_SERVER );

   } while (r == 512);

   /* Send what's finalized to the client. */
   if (r = (int) g_in_final.popBytes( max ))
   {
      const string& str = g_in_final.getLastLine( );
      memcpy( buf, str.c_str( ), str.size( ) );
      return r;
   }
 
   
   // Nothing to send to the client.
   r = SOCKET_ERROR;
   WSASetLastError( WSAEWOULDBLOCK );
   return r;
}

int on_client_send( SOCKET s, const char* buf, size_t size )
{
   if (s != g_socket)
   {
      // Probably the fileserver connection.
      // As a dirty hack for keeping the player's name/pw in sync
      // with fserv logins, watch out for the RC, USER, and PASS commands and edit accordingly.
      if (size > 3 && !strncmp( buf, "RC ", 3 ))
      {
         // Edit the first part.
         string rc = "RC ";
         rc += world::get_player( ).name;
         rc += " ";
         rc += world::get_player( ).pw;
         // Find the payload.
         const char* payload = buf;
         for (unsigned num_spaces = 0; (payload = strchr( payload + 1, ' ' )) && num_spaces < 2; num_spaces++) ;
         if (payload)
         {
            rc += payload;
            return send( s, rc.c_str( ), (int) rc.length( ), 0 );
         }
      }
      if (size > 5 && !strncmp( buf, "USER ", 5 ))
      {
         // Edit the name.
         string cmd = "USER ";
         cmd += world::get_player( ).name;
         cmd += "\n";
         return send( s, cmd.c_str( ), (int) cmd.length( ), 0 );
      }
      else if (size > 5 && !strncmp( buf, "PASS ", 5))
      {
         const string cmd = "PASS " + world::get_player( ).pw + "\n";
         return send( s, cmd.c_str( ), (int) cmd.length( ), 0 );
      }
      return send( s, buf, (int) size, 0 );
   }

   /* Dealing with main socket. */

   if (size && g_state == NETSTATE_NEGOTIATING)
   {
      /* Let SOCKS stuff pass through unmolested. */
      switch (g_proxy_state)
      {
      // This assumes it all is sent at once.
      case SOCKS_DONE:  // Also the start value for the proxy SM.
         /* Hackily detect the proxy type. */
         if (buf[ 0 ] == 0x04)
         {
            g_proxy_state = SOCKS4_CONNREQUEST;
            return send( s, buf, (int) size, 0 );
         }
         else if (buf[ 0 ] == 0x05)
         {
            g_proxy_state = SOCKS5_GREETING;
            return send( s, buf, (int) size, 0 );
         }
         break;
      case SOCKS5_PROXYSELECT:
         {
            g_proxy_state = SOCKS5_CONNREQUEST;
            return send( s, buf, (int) size, 0 );
         }
      }
      // Oops, not a proxy connection.
      set_state( NETSTATE_CONNECTED );
   }

   /* Queue it up. */
   push_outbound( buf, size, FROM_CLIENT );

   /* Just tell the client it was all sent. */
   return (int) size;
}

int on_client_connect( SOCKET s, const sockaddr_in* addr )
{
	//OutputDebugString("General connection attempted: ");
	//OutputDebugString(inet_ntoa(addr->sin_addr));
   // Just take the first connection that isn't to the news server.
   if (g_socket == INVALID_SOCKET 
       && addr->sin_addr.S_un.S_addr != inet_addr( "72.232.1.171" ))   // Hacky. 72.233.19.231
   {
	   //OutputDebugString("Server connection attempted: ");
	   //OutputDebugString(inet_ntoa(addr->sin_addr));
      // This is our main connection.
      g_socket = s;

      // Disable nagling.
      {
         BOOL opt = TRUE;
         setsockopt( g_socket, IPPROTO_TCP, TCP_NODELAY, (const char*) &opt, sizeof( opt ) );
      }

      // Send an event.
      core::event_set_main_socket( g_socket );

      set_state( NETSTATE_CONNECTING );
   }

   return connect( s, (sockaddr*) addr, sizeof( sockaddr_in ) );
}

int on_client_closesocket( SOCKET s )
{
   if (g_socket == s)
   {
      set_state( NETSTATE_DISCONNECTED );
      reset_connection( );      
   }

   return closesocket( s );
}

void on_wsa_event( int fd, int err )
{
   if (fd == FD_CONNECT)
   {
      if (err)
      {
         set_state( NETSTATE_DISCONNECTED );
         reset_connection( );
      }
      else
      {
         /* Could have just connected to the proxy. */
         set_state( NETSTATE_NEGOTIATING );
         // set_state( NETSTATE_CONNECTED );
      }
   }
   else if (fd == FD_CLOSE)
   {
      set_state( NETSTATE_DISCONNECTED );
      reset_connection( );
   }
}

void on_screen_load( Client_Screen screen )
{
   if (g_state == NETSTATE_NEGOTIATING)
   {
      /* Cheap and dirty way to know we've gotten through negotiations. */
      if (screen == SCREEN_BORDER || screen == SCREEN_PICKEM)
         set_state( NETSTATE_CONNECTED );
   }
}

int send_all( )
{
   int sent = 0;
   while (g_out_final.getSize( ))
   {
      static char buf[ 256 ];
      size_t len = g_out_final.peekBytes( buf, 256 );
      g_out_final.popBytes( len );

      for (const char* data = buf; len; )
      {
         int r = send( g_socket, data, 
                      (len > SEND_CHUNK_SIZE) ? SEND_CHUNK_SIZE : (int) len,
                      0 );

         if (r == SOCKET_ERROR)
         {
            int e = WSAGetLastError( );

            if (e == WSAEWOULDBLOCK)
            {
               fd_set w_set;
               FD_ZERO( &w_set );
               FD_SET( g_socket, &w_set );
               fd_set e_set;
               FD_ZERO( &e_set );
               FD_SET( g_socket, &e_set );
               timeval tv;
               tv.tv_sec   = SELECT_WAIT_MS / 1000;
               tv.tv_usec  = (SELECT_WAIT_MS % 1000) * 1000000;

               if (0 >= select( (int) g_socket + 1, 0, &w_set, &e_set, &tv ))
                  return SOCKET_ERROR;

               if (!FD_ISSET( g_socket, &w_set ))
                  return SOCKET_ERROR;
            }
            else
               return SOCKET_ERROR;
         }
         else
         {
            data += r;
            len  -= r;
            sent += r;
         }
      }
   }

   return sent;
}

void on_batch( )
{
   while (!g_in_lines.empty( ))
   {
      core::event_inbound( g_in_lines.front( ).line, 
                           g_in_lines.front( ).from, 
                           g_num_in_lines );
      g_in_lines.pop_front( );
      ++g_num_in_lines;
   }
   while (!g_out_lines.empty( ))
   {
         core::event_outbound( g_out_lines.front( ).line, 
                               g_out_lines.front( ).from, 
                               g_num_out_lines );
      g_out_lines.pop_front( );
      ++g_num_out_lines;
   }
}

void finalize_inbound_line( const string& line )
{
   if (g_state >= NETSTATE_CONNECTED)
   {
      size_t len = 0;

      // Clip to one line.
      for (string::const_iterator i = line.begin( ); i != line.end( ); ++i, ++len)
         if (*i == '\n' || *i == '\r')
            break;

      // Can't discard empty lines because they're significant to the MOTD.

      g_in_final.pushBytes( line.c_str( ), len );
      g_in_final.pushBytes( "\n", 1 );

      /* Let the client know something's waiting. */
      window::post_message( window::WM_WSAMSG, *(WPARAM*)&g_socket, 
                            MAKELPARAM( FD_READ, 0 ) );
   }
}

void finalize_outbound_line( const string& line )
{   
   if (g_state >= NETSTATE_CONNECTED)
   {
      size_t len = 0;

      // Clip to one line.
      for (string::const_iterator i = line.begin( ); i != line.end( ); ++i, ++len)
         if (*i == '\n' || *i == '\r')
            break;

      if (len) // Discard empty lines.
      {
         /* Server will disconnect if line is too big. */
         if (len > MAX_OLINE_LEN)
         {
            core::system_errspeak( "Warning: An outbound line was truncated because"
                                   " it was too long!" );
            len = MAX_OLINE_LEN;
         }
         
         g_out_final.pushBytes( line.c_str( ), len );
         g_out_final.pushBytes( "\n", 1 );
   
         // Send everything right away.
         send_all( );
      }
   }
}

void set_state( Net_State state )
{
   if (g_state != state)
   {
      g_state = state;

      /* Inform the core. */
      if (state == NETSTATE_CONNECTED)
         core::event_connected( );
      else if (state == NETSTATE_DISCONNECTED)
         core::event_disconnected( );
   }
}

size_t get_inbound_linecount( )
{
   // Return the next line number to be assigned.
   return g_num_in_lines + g_in_lines.size( );
}

size_t get_outbound_linecount( )
{
   // Return the next line number to be assigned.
   return g_num_out_lines + g_out_lines.size( );
}

}  // namespace net