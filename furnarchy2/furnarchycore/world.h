#ifndef _FURNARCHYCORE_WORLD_H__
#define _FURNARCHYCORE_WORLD_H__

#include "common.h"
#include "yiffurc/yiffurc.h"

/** Does world bookkeeping, speech/specitag buffering, and name completion. */
namespace world
{
   using namespace std;

   enum { MAX_RECENT_NAMES = 32 };

   bool initialize( );
   bool uninitialize( );

   const PLAYERINFO&  get_player( );
   void               name_push( const string& name );
   // Failure when return value == iter
   int                name_complete( string* name, int iter = 0 );
   int                desctag_reg( );
   bool               desctag_unreg( int tag_id );
   bool               desctag_set( int tag_id, const char* tag );
   void               badge_cancel( );
   const MONSTERINFO* monster_find( unsigned guid );
   const MONSTERINFO* monster_find( const string& name, bool by_shortname = false );
   void               monster_update( unsigned guid, const char* name, const char* colors,
                                      const unsigned* frame, const POINT2* pos,
                                      bool* visible, unsigned flags, unsigned flags_mask );
   void               monster_remove( unsigned guid );
   void               monster_enum( vector<unsigned>* guids );

   /** Sets player info.
   *  All parameters are optional.
   *  Automatically raises a core::event_player_info */
   void              set_player_info( const char* name, const char* pw, 
                                      const char* colors, const char* desc,
                                      const unsigned int* guid, const POINT2* cam );
   
   void on_tick( unsigned int dt );
   long on_outbound_line( string* line, int from, size_t num );
   long on_inbound_line( string* line, int from, size_t num );
   void on_connlvl( Connection_Level lvl );
   bool on_command( const ArgList& args );

   const yiffurc::S2C* parse_inbound( const string& line );
   const yiffurc::C2S* parse_outbound( const string& line );
};

#endif
