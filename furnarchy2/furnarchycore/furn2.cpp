#define F2LINK extern "C" __declspec(dllexport)
#include "furn2.h"
#include "apihooks.h"
#include "net.h"
#include "core.h"
#include "modules.h"
#include "world.h"
#include "memlocs.h"
#include "window.h"
#include "client.h"
#include "options.h"
#include "shortname.h"
#include "scripts.h"

namespace furn2 {

   // Deprecated structure used by f2_myplayer()
   struct _PLAYERINFO1
   {
      const char*  name;
      const char*  pw;
      const char*  mac_id;
      const char*  desc;
      char   colors[ 13 ];
      POINT2 cam;
   };

   F2LINK void* F2CALL f2_hookapi( const char* lib, const char* name,
      void* hook, bool by_ordinal ) {
      return apihooks::hook_api( lib, name, hook, by_ordinal );
   }

   F2LINK void F2CALL f2_netout( const char* buf, std::size_t size ) {
      net::push_outbound( buf, size );
   }

   F2LINK void F2CALL f2_netin( const char* buf, std::size_t size ) {
      net::push_inbound( buf, size );
   }

   F2LINK void F2CALL f2_reviseline( const char* line ) {
      modules::revise_line( line );
   }

   F2LINK void F2CALL f2_cancelline( ) {
      modules::hide_line( furn2::HIDE_ALL );
   }

   F2LINK void F2CALL f2_hideline( int flags ) {
      modules::hide_line( (unsigned long) flags );
   }

   F2LINK int F2CALL f2_hideflags( ) {
      return (int) modules::get_hide_flags( );
   }

   F2LINK void F2CALL f2_speak( const char* line ) {
      core::system_speak( line );
   }

   F2LINK void F2CALL f2_errspeak( const char* line ) {
      core::system_errspeak( line );
   }

   F2LINK void F2CALL f2_errlog( const char* line ) {
      core::log_error( line );
   }

   F2LINK void F2CALL f2_command( const char* command, bool noecho ) {
      core::push_command( command, noecho );
   }

   F2LINK bool F2CALL f2_buildok( ) {
      return memlocs::valid( );
   }

   F2LINK unsigned int F2CALL f2_clcrc( ) {
      return memlocs::get_client_crc( );
   }

   F2LINK void F2CALL f2_version( F2VERSION* out )
   {
      out->major    = F2VER_MAJOR;
      out->minor    = F2VER_MINOR;
      out->revision = F2VER_REV;
   }

   F2LINK bool F2CALL f2_singleusermode( ) {
      return options::get_single_user_mode( );
   }

   F2LINK const char* F2CALL f2_root( ) {
      return core::get_furn_root( ).c_str( );
   }

   F2LINK const char* F2CALL f2_dataroot( ) {
      return core::get_furn_data_root( ).c_str( );
   }

   F2LINK const char* F2CALL f2_docroot( ) {
      return core::get_furn_doc_root( ).c_str( );
   }

   F2LINK const char* F2CALL f2_clroot( ) {
      return core::get_client_root( ).c_str( );
   }

   F2LINK const char* F2CALL f2_cldataroot( ) {
      return core::get_client_data_root( ).c_str( );
   }

   F2LINK const char* F2CALL f2_cldocroot( ) {
      return core::get_client_doc_root( ).c_str( );
   }

   F2LINK std::size_t F2CALL f2_modcount( ) {
      return modules::get_num_modules( );
   }

   F2LINK const char* F2CALL f2_modroot( unsigned int mod )
   {
      if (mod < modules::get_num_modules( ))
         return modules::get_module_root( mod ).c_str( );
      return NULL;
   }

   F2LINK const char* F2CALL f2_moddataroot( unsigned int mod )
   {
      if (mod < modules::get_num_modules( ))
         return modules::get_module_data_root( mod ).c_str( );
      return NULL;
   }

   F2LINK const char* F2CALL f2_moddocroot( unsigned int mod )
   {
      if (mod < modules::get_num_modules( ))
         return modules::get_module_doc_root( mod ).c_str( );
      return NULL;
   }

   F2LINK const char* F2CALL f2_modname( unsigned int mod )
   {
      if (mod < modules::get_num_modules( ))
         return modules::get_module_name( mod ).c_str( );
      return NULL;
   }

   F2LINK const char* F2CALL f2_modfancyname( unsigned int mod )
   {
      if (mod < modules::get_num_modules( ))
         return modules::get_module_fancy_name( mod ).c_str( );
      return NULL;
   }

   F2LINK const char* F2CALL f2_modthumb( unsigned int mod )
   {
      if (mod < modules::get_num_modules( ))
         return modules::get_module_thumb( mod ).c_str( );
      return NULL;
   }

   F2LINK unsigned long F2CALL f2_modstate( unsigned int mod ) {
      if (mod < modules::get_num_modules( ))
         return modules::get_module_state( mod );
      return 0;
   }

   F2LINK void F2CALL f2_modenable( unsigned int mod, bool onoff ) {
      if (mod < modules::get_num_modules( ))
         modules::enable_module( mod, onoff );
   }

   F2LINK unsigned int F2CALL f2_modfind( const UUID128* uuid ) {
      return modules::find_module( *(const ::UUID128*) uuid );
   }

   F2LINK void F2CALL f2_modtouch( unsigned int mod ) {
      if (mod < modules::get_num_modules( ))
         modules::touch_module( mod );
   }

   F2LINK int F2CALL f2_modmsg( unsigned int mod, void* data ) {
      if (mod < modules::get_num_modules( ))
         return modules::msg_module( mod, data );
      return 0;
   }

   //F2LINK void F2CALL f2_myplayer( _PLAYERINFO1* out )
   //{
   //   const ::PLAYERINFO& player = world::get_player( );
   //   out->name   = player.name.c_str( );
   //   out->pw     = player.pw.c_str( );
   //   out->mac_id = player.mac_id.c_str( );
   //   // Colors no longer work for this.
   //   memset( out->colors, 0, sizeof( out->colors ) );
   //   out->desc   = player.desc.c_str( );
   //   out->cam    = (const POINT2&) player.cam;
   //}

   F2LINK void F2CALL f2_myplayerinfo( PLAYERINFO* out )
   {
      const ::PLAYERINFO& player = world::get_player( );
      out->name   = player.name.c_str( );
      out->pw     = player.pw.c_str( );
      out->mac_id = player.mac_id.c_str( );
      out->colors = player.colors.c_str( );
      out->desc   = player.desc.c_str( );
      out->guid   = player.guid;
      out->cam    = (const POINT2&) player.cam;
   }

   F2LINK void F2CALL f2_monsterinfo( unsigned int guid, MONSTERINFO* out )
   {
      const ::MONSTERINFO* monster = world::monster_find( guid );
      if (monster)
      {
         out->guid  = monster->guid;
         out->frame = monster->frame;
         out->pos   = monster->pos;
         out->name  = monster->name.c_str( );
         out->color = monster->color.c_str( );
         out->flags = monster->flags;
      }
      else
         out->guid = 0;
   }

   F2LINK void F2CALL f2_monsterinfo_byname( const char* name, MONSTERINFO* out, bool by_shortname /* = false */ )
   {
      const ::MONSTERINFO* monster = world::monster_find( name, by_shortname );
      if (monster)
      {
         out->guid  = monster->guid;
         out->frame = monster->frame;
         out->pos   = monster->pos;
         out->name  = monster->name.c_str( );
         out->color = monster->color.c_str( );
         out->flags = monster->flags;
      }
      else
         out->guid = 0;
   }

   F2LINK unsigned int F2CALL f2_monsters_enum( unsigned int* guids, unsigned int len )
   {
      vector<unsigned> vguids;
      world::monster_enum( &vguids );

      if (guids)
      {
         vector<unsigned>::size_type i = 0;
         for (; len && i < vguids.size( ); --len, ++i, ++guids)
            *guids = vguids[ i ];
         return (unsigned int) i;
      }
      return (unsigned int) vguids.size( );
   }

   F2LINK bool F2CALL f2_monstervisible( unsigned int guid )
   {
      const ::MONSTERINFO* monster = world::monster_find( guid );
      if (monster)
      {
         return monster->visible;
      }
      return false;
   }
      

   F2LINK HWND F2CALL f2_getwnd( )
   {
      return window::get_handle( );
   }

   F2LINK int F2CALL f2_desctag_reg( ) {
      return world::desctag_reg( );
   }

   F2LINK bool F2CALL f2_desctag_unreg( int tag_id ) {
      return world::desctag_unreg( tag_id );
   }

   F2LINK bool F2CALL f2_desctag_set( int tag_id, const char* tag ) {
      return world::desctag_set( tag_id, tag );
   }

   F2LINK int F2CALL f2_name_complete( char* name, std::size_t size, int iter )
   {
      string _name = name;
      iter = world::name_complete( &_name, iter );
      strcpy_s( name, size, _name.c_str( ) );
      return iter;
   }

   F2LINK void F2CALL f2_name_push( const char* name ) {
      world::name_push( name );
   }

   F2LINK Connection_Level F2CALL f2_connlvl( ) {
      return core::get_connlvl( );
   }

   F2LINK size_t F2CALL f2_netoutline( const char* line, std::size_t len ) {
      return net::push_outbound_line( line, len );
   }

   F2LINK size_t F2CALL f2_netinline( const char* line, std::size_t len ) {
      return net::push_inbound_line( line, len );
   }

   F2LINK size_t F2CALL f2_netoutcount( ) {
      return net::get_outbound_linecount( );
   }

   F2LINK size_t F2CALL f2_netincount( ) {
      return net::get_inbound_linecount( );
   }   

   F2LINK void F2CALL f2_badge_cancel( ) {
      world::badge_cancel( );
   }

   F2LINK void F2CALL f2_setcurdialog( HWND dlg ) {
      window::set_current_dialog( dlg );
   }

   F2LINK void F2CALL f2__setplayerinfo( const char* name, const char* pw, 
                                         const char* color, const char* desc,
                                         const unsigned int* guid )
   {
      client::set_player_info( name, pw, color, desc, guid );
   }

   F2LINK void F2CALL f2__setmainsocket( SOCKET socket )
   {
      client::set_main_socket( socket );
   }

   F2LINK void F2CALL f2__pushchatbox( const char* line )
   {
      core::push_chatbox( line );
   }

   F2LINK bool F2CALL f2__ignore_add( const char* name )
   {
      return client::ignore_add( name );
   }

   F2LINK bool F2CALL f2__ignore_rm( const char* name )
   {
      return client::ignore_rm( name );
   }

   F2LINK bool F2CALL f2__ignore_test( const char* name )
   {
      return client::ignore_test( name );
   }

   F2LINK void F2CALL f2_makeshortname( const char* name, char* buf )
   {
      string shortname = shortname::make_shortname( name );
      strcpy( buf, shortname.c_str( ) );
   }

   F2LINK bool F2CALL f2_createdirs( const char* path )
   {
      return core::create_folder_path( path );
   }

   F2LINK unsigned int F2CALL f2_script_enum( unsigned int* sids, unsigned int len )
   {
      vector<unsigned int> vsids;
      scripts::script_enum( &vsids );
      if (sids)
      {
         size_t i = 0;
         for (; i < len && i < vsids.size( ); ++i)
            sids[ i ] = vsids[ i ];
         return i;
      }
      return vsids.size( );
   }

   F2LINK unsigned int F2CALL f2_script_argv( unsigned int sid, const char** argv, unsigned int len, int* flags )
   {
      const ArgList* arglist;
      long lflags = 0;
      if (scripts::script_argv( sid, &arglist, &lflags ) == SCRIPT_OK)
      {
         if (flags)
            *flags = (unsigned int) lflags;

         assert( arglist );

         if (argv)
         {
            size_t i = 0;
            for (; i < len && i < arglist->size( ); ++i)
               argv[ i ] = (*arglist)[ i ].c_str( );
            return i;
         }
         return arglist->size( );
      }
      return 0;
   }

   F2LINK Script_Error F2CALL f2_script_run( const char** argv, unsigned int len, int flags, unsigned int* sid )
   {
      ArgList arglist;
      for (unsigned int i = 0; i < len; ++i)
         arglist.push_back( argv[ i ] );
      return scripts::script_run( arglist, (unsigned long) flags );
   }

   F2LINK Script_Error F2CALL f2_script_kill( unsigned int sid )
   {
      return scripts::script_kill( sid );
   }

   F2LINK bool F2CALL f2_playermenu_set( unsigned int mod, HMENU menu )
   {
      return modules::playermenu_set( mod, menu );
   }

   F2LINK bool F2CALL f2_playermenu_unset( unsigned int mod )
   {
      return modules::playermenu_unset( mod );
   }

   F2LINK const char* F2CALL f2__playermenu_name( )
   {
      return client::playermenu_name( ).c_str( );
   }

} // namespace furn2