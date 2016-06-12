#ifndef _FURNARCHYCORE_NET_H__
#define _FURNARCHYCORE_NET_H__

#include "common.h"
#include <winsock.h>

enum { FROM_SERVER = 0, FROM_CLIENT = 0, FROM_OTHER = 1 };

namespace net
{
   using namespace std;
   extern const char *FURC_WEBSITE_IP;

   bool          initialize( );
   bool          uninitialize( );

   /** Pushes bytes for processing. */
   bool          push_outbound( const char* buf, size_t size, int from = FROM_OTHER );
   bool          push_inbound( const char* buf, size_t size, int from = FROM_OTHER );

   /** Pushes a line for processing. */
   size_t        push_outbound_line( const char* line, size_t len = -1, int from = FROM_OTHER );
   size_t        push_inbound_line( const char* line, size_t len = -1, int from = FROM_OTHER );

   /** Finalized lines.
   * These won't get broadcasted. */
   void          finalize_inbound_line( const string& line );
   void          finalize_outbound_line( const string& line );

   /** Replaces the current main socket.
   *  Won't raise an event. */
   bool          set_main_socket( SOCKET s );

   int           on_client_recv( SOCKET s, char* buf, size_t max );
   int           on_client_send( SOCKET s, const char* buf, size_t size );
   int           on_client_connect( SOCKET s, const sockaddr_in* addr );
   int           on_client_closesocket( SOCKET s );
   void          on_wsa_event( int fd, int err );
   void          on_screen_load( Client_Screen screen );
   void          on_batch( );

   size_t get_inbound_linecount( );
   size_t get_outbound_linecount( );
};

#endif