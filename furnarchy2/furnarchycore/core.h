#ifndef _FURNARCHYCORE_CORE_H__
#define _FURNARCHYCORE_CORE_H__

#include "common.h"
#include <WinSock.h>

#define F2VER_MAJOR 2
#define F2VER_MINOR 6
#define F2VER_REV   13
#define F2VER_SZ    "2.6-r13"

enum Batch_Subsystem { BATCH_NET, BATCH_CORE };

namespace core
{
   using namespace std;

   bool initialize( HINSTANCE core_instance, unsigned int crc32 );
   bool uninitialize( );

   HINSTANCE     get_core_inst( );
   const string& get_furn_root( );
   const string& get_furn_doc_root( );
   const string& get_furn_data_root( );
   const string& get_profile_root( );
   const string& get_client_root( );
   const string& get_client_doc_root( );
   const string& get_client_data_root( );
   time_t        get_start_time( );
   Connection_Level get_connlvl( );

   /* Creates a folder and all its intermediate folders. */
   bool          create_folder_path( const string& folder );

   void          log_error( const char* fmt, ... );
   void          system_speak( const string& message );
   void          system_errspeak( const string& message );
   void          batch_add( Batch_Subsystem subsystem );
   void          silence( bool onoff );
   bool          source( const string& name );
   void          push_command( const string& command, bool noecho = false );   
   void          push_chatbox( const string& line );

   void          event_tick( );
   void          event_inbound( const string& line, int from, size_t num );
   void          event_outbound( const string& line, int from, size_t num );
   void          event_screen_load( Client_Screen screen );
   void          event_render( HDC dc, unsigned int width, unsigned int height );
   bool          event_wndproc( WNDPROCPARAMS* params );
   void          event_window_created( HWND wnd );
   void          event_modstate( unsigned int mod, long state );
   void          event_batch( Batch_Subsystem subsystem );
   void          event_connected( );
   void          event_disconnected( );
   void          event_loggedin( );
   void          event_motdend( );
   void          event_set_main_socket( SOCKET socket );
   void          event_player_info( );
   void          event_monster( unsigned guid );
   bool          event_chatbox( const string& line );
   void          event_script( );
};

#endif
