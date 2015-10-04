#ifndef _FURNARCHYCORE_MODULES_H__
#define _FURNARCHYCORE_MODULES_H__

#include "common.h"
#include <WinSock.h>

struct UUID128;

namespace modules
{      
   bool initialize( );
   bool uninitialize( );

   unsigned int  find_module( const UUID128& uuid );
   size_t        get_num_modules( );
   long          get_module_state( unsigned int mod );
   const string& get_module_name( unsigned int mod );
   const string& get_module_fancy_name( unsigned int mod );
   const string& get_module_root( unsigned int mod );
   const string& get_module_data_root( unsigned int mod );
   const string& get_module_doc_root( unsigned int mod );
   const string& get_module_thumb( unsigned int mod );
   
   void enable_module( unsigned int mod, bool onoff );
   void touch_module( unsigned int mod );
   intptr_t msg_module( unsigned int mod, void* data );
   bool playermenu_set( unsigned int mod, HMENU menu );
   bool playermenu_unset( unsigned int mod );

   long on_outbound_line( string* line, int from, size_t num );
   long on_inbound_line( string* line, int from, size_t num );
   bool on_command( const ArgList& args );
   void on_connlvl( Connection_Level lvl );
   void on_monster( unsigned guid );
   void on_window_created( HWND wnd );
   void on_screen_load( Client_Screen screen );
   void on_tick( unsigned int dt );
   void on_render( HDC dc, unsigned int width, unsigned int height );
   bool on_wndproc( WNDPROCPARAMS* params );
   void on_set_main_socket( SOCKET socket );
   void on_player_info( );
   bool on_chatbox( const string& line );
   void on_playermenu_created( );
   void on_playermenu_show( const string& name );
   void on_script( );

   void revise_line( const char* line );
   void hide_line( long flags );
   unsigned long get_hide_flags( );
};

#endif
