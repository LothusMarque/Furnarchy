#ifndef _FURNARCHYCORE_CLIENT_H__
#define _FURNARCHYCORE_CLIENT_H__

#include "common.h"
#include <WinSock.h>

namespace client
{
   bool initialize( );
   bool uninitialize( );
   
   bool  on_command( const ArgList& args );
   HMENU on_client_LoadMenu( HINSTANCE inst, LPCTSTR resource );

   HMENU         playermenu_menu( );
   const string& playermenu_name( );
   void  chatbox_push_sysmsg( const string& line );

   /** Replaces the current main socket.
   *  Automatically calls net::set_main_socket() and
   *  raises core::event_set_main_socket(). */
   void set_main_socket( SOCKET s );
   /** Changes client player information.
   *  All parameters are optional.
   *  Automatically calls world::set_player_info(). */
   void set_player_info( const char* name, const char* pw, 
                         const char* colors, const char* desc,
                         const unsigned int* guid );
   
   bool ignore_add( const char* name );
   bool ignore_rm( const char* name );
   bool ignore_test( const char* name );
   bool ignore_line_test( const char* line );
};

#endif
