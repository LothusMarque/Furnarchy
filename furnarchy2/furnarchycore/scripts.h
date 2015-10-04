#ifndef _FURNARCHYCORE_SCRIPTS_H__
#define _FURNARCHYCORE_SCRIPTS_H__

#include "common.h"

namespace scripts
{
   bool initialize( );
   bool uninitialize( );
   
   Script_Error script_enum( vector<unsigned int>* sids );
   Script_Error script_argv( unsigned int sid, const ArgList** args, long* flags = NULL );
   Script_Error script_run( const ArgList& argv, long flags, unsigned int* sid = NULL );
   Script_Error script_kill( unsigned int sid );

   bool on_command( const ArgList& args );
   long on_inbound_line( string* line, int from, size_t num );
   long on_inbound_line_late( string* line, int from, size_t num );
   long on_outbound_line( string* line, int from, size_t num );
   long on_outbound_line_late( string* line, int from, size_t num );
   void on_tick( unsigned int dt );
   void on_connlvl( Connection_Level lvl );
   bool on_wndproc( WNDPROCPARAMS* params );

   void hide_line( long flags );

   //
   void cmd_sckill( const ArgList& args );
}

#endif
