/************************************************************************/
/* Copyright (c) 2006 Cluracan (strangelove@heroinpuppy.com)            */
/*                                                                      */
/* The "MIT" License                                                    */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation       */
/* files (the "Software"), to deal in the Software without restriction, */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* - The above copyright notice and this permission notice shall be     */
/* included in all copies or substantial portions of the Software.      */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND             */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT          */
/* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,         */
/* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        */
/* DEALINGS IN THE SOFTWARE.                                            */
/************************************************************************/

#include "net.h"
#include "pfx_heap.h"
#include "misc.h"
#include "c2s_enc_stock.h"

namespace yiffurc {

const C2S_ENC_STOCK c2s_enc_stock;

const char* YIFFURC_CALL c2s_enc_changecolor( const char* color,
                                              const char* gsd )
{
   const size_t color_len = STRLEN( color );
   const size_t gsd_len   = STRLEN( gsd );

   char* out = pfx_new<char>( 6 + color_len + gsd_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "chcol ", 6 );
   out = chain_memcpy( out, color, color_len );
   out = chain_memcpy( out, gsd, gsd_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_changedesc( const char* desc )
{
   size_t desc_len = STRLEN( desc );

   char* out = pfx_new<char>( 7 + desc_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "chdesc ", 7 );
   out = chain_memcpy( out, desc, desc_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_create( const char* player, const char* pass,
                                         const char* email )
{
   size_t player_len = STRLEN( player );
   size_t pass_len   = STRLEN( pass );
   size_t email_len  = STRLEN( email );

   char* out = pfx_new<char>( 7 + player_len + 1
                              + pass_len + 1 + email_len + 4 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "create ", 7 );
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, " ", 1 );
   out = chain_memcpy( out, pass, pass_len );
   out = chain_memcpy( out, " ", 1 );
   out = chain_memcpy( out, email, email_len );
   out = chain_memcpy( out, " N Y\n", 6 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_emote( const char* action )
{
   size_t action_len = STRLEN( action );

   char* out = pfx_new<char>( 1 + action_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, ":", 1 );
   out = chain_memcpy( out, action, action_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_fdl( const char* furl )
{
   size_t furl_len = STRLEN( furl );

   char* out = pfx_new<char>( 4 + furl_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "fdl ", 4 );
   out = chain_memcpy( out, furl, furl_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_gomap( unsigned int map_num )
{
   char* out = pfx_new<char>( 6 + 1 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "gomap ", 6 );
   *out = (map_num % 95) + 32;
   ++out;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_join( const char* player )
{
   size_t player_len = STRLEN( player );

   char* out = pfx_new<char>( 5 + player_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "join ", 5 );
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_login( const char* player, const char* pass,
                            const char* macid /* = 0 */ )
{
   size_t player_len = STRLEN( player );
   size_t pass_len   = STRLEN( pass );
   size_t macid_len  = STRLEN( macid );

   if (macid_len)
   {
      char* out = pfx_new<char>( 8 + player_len + 1
                                 + pass_len + 3 + macid_len + 2 );
      const char* const ret = out;

      out = chain_memcpy( out, "connect ", 8 );
      out = chain_memcpy( out, player, player_len );
      out = chain_memcpy( out, " ", 1 );
      out = chain_memcpy( out, pass, pass_len );
      out = chain_memcpy( out, " PW", 3 );
      out = chain_memcpy( out, macid, macid_len );
      out = chain_memcpy( out, "\n", 2 );

      return ret;
   }

   char* out = pfx_new<char>( 8 + player_len + 1 + pass_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "connect ", 8 );
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, " ", 1 );
   out = chain_memcpy( out, pass, pass_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_lookpos( unsigned int x, unsigned int y )
{
   char* out = pfx_new<char>( 2 + 2 + 2 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "l ", 2 );
   itof2( x, out );  out += 2;
   itof2( y, out );  out += 2;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_onlinecheck( const char* player )
{
   size_t player_len = STRLEN( player );

   char* out = pfx_new<char>( 5 + player_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "onln ", 5 );
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_pong( int num )
{
   size_t num_len = dec_len( num );

   char* out = pfx_new<char>( 6 + num_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "polon ", 6 );
   out = chain_itoa( out, num );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_regdream( unsigned int dream_id )
{
   size_t did_len = dec_len( dream_id );

   char* out = pfx_new<char>( 7 + did_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "tdgate ", 7 );
   out = chain_itoa( out, dream_id );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_setcolor( const char* color,
                                           const char* gsd )
{
   const size_t color_len = STRLEN( color );
   const size_t gsd_len   = STRLEN( gsd );

   char* out = pfx_new<char>( 6 + color_len + gsd_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "color ", 6 );
   out = chain_memcpy( out, color, color_len );
   out = chain_memcpy( out, gsd, gsd_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_setdesc( const char* desc )
{
   size_t desc_len = STRLEN( desc );

   char* out = pfx_new<char>( 5 + desc_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "desc ", 5 );
   out = chain_memcpy( out, desc, desc_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_shout( const char* shout /* = 0 */ )
{
   size_t shout_len = STRLEN( shout );

   char* out = pfx_new<char>( 1 + shout_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "-", 1 );
   out = chain_memcpy( out, shout, shout_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_speech( const char* message )
{
   size_t message_len = STRLEN( message );

   char* out = pfx_new<char>( 1 + message_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "\"", 1 );
   out = chain_memcpy( out, message, message_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_summon( const char* player )
{
   size_t player_len = STRLEN( player );

   char* out = pfx_new<char>( 7 + player_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "summon ", 7 );
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_version( MAJORMINOR client )
{
   size_t major_len  = dec_len( client.major );
   size_t minor_len  = dec_len( client.minor );

   char* out = pfx_new<char>( 8 + major_len + 1 + minor_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "version ", 8 );
   out = chain_itoa( out, client.major );
   out = chain_memcpy( out, ".", 1 );
   out = chain_itoa( out, client.minor );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_whisper( const char* to, const char* message )
{
   size_t to_len        = STRLEN( to );
   size_t message_len   = STRLEN( message );

   char* out = pfx_new<char>( 3 + to_len + 1 + message_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "wh ", 3 );
   out = chain_memcpy( out, to, to_len );
   out = chain_memcpy( out, " ", 1 );
   out = chain_memcpy( out, message, message_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_winver( MAJORMINOR windows, unsigned int build,
                             MAJORMINOR directx )
{
   size_t windows_major_len = dec_len( windows.major );
   size_t windows_minor_len = dec_len( windows.minor );
   size_t build_len         = dec_len( build );
   size_t directx_major_len = dec_len( directx.major );
   size_t directx_minor_len = dec_len( directx.minor );

   char* out = pfx_new<char>( 8 + windows_major_len + 1
                              + windows_minor_len + 2 + build_len
                              + 2 + directx_major_len + 1
                              + directx_minor_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "Winver v", 8 );
   out = chain_itoa( out, windows.major );
   out = chain_memcpy( out, ".", 1 );
   out = chain_itoa( out, windows.minor );
   out = chain_memcpy( out, " b", 2 );
   out = chain_itoa( out, (int) build_len );
   out = chain_memcpy( out, " d", 2 );
   out = chain_itoa( out, directx.major );
   out = chain_memcpy( out, ".", 1 );
   out = chain_itoa( out, directx.minor );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_emit( const char* message )
{
   size_t message_len =  STRLEN( message );

   char* out = pfx_new<char>( 6 + message_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "\"emit ", 6 );
   out = chain_memcpy( out, message, message_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_emitloud( const char* message )
{
   size_t message_len = STRLEN( message );

   char* out = pfx_new<char>( 10 + message_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "\"emitloud ", 10 );
   out = chain_memcpy( out, message, message_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_eject( const char* player )
{
   size_t player_len = STRLEN( player );

   char* out = pfx_new<char>( 7 + player_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "\"eject ", 7 );
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_share( const char* player )
{
   size_t player_len = STRLEN( player );

   char* out = pfx_new<char>( 7 + player_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "\"share ", 7 );
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_unshare( const char* player )
{
   size_t player_len = STRLEN( player );

   char*  out = pfx_new<char>( 9 + player_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "\"unshare ", 9 );
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_givecookie( const char* player,
                                const char* message /* = 0 */ )
{
   size_t player_len = STRLEN( player );
   size_t message_len = STRLEN( message );

   if (message_len)
   {
      char* out = pfx_new<char>( 13 + player_len + 1 + message_len + 2 );
      const char* const ret = out;

      out = chain_memcpy( out, "\"give-cookie ", 13 );
      out = chain_memcpy( out, player, player_len );
      out = chain_memcpy( out, " ", 1 );
      out = chain_memcpy( out, message, message_len );
      out = chain_memcpy( out, "\n", 2 );

      return ret;
   }

   char* out = pfx_new<char>( 13 + player_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "\"give-cookie ", 13 );
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_makecookie( const char* player,
                                const char* message /* = 0 */ )
{
   size_t player_len = STRLEN( player );
   size_t message_len = STRLEN( message );

   if (message_len)
   {
      char* out = pfx_new<char>( 13 + player_len + 1 + message_len + 2 );
      const char* const ret = out;

      out = chain_memcpy( out, "\"make-cookie ", 13 );
      out = chain_memcpy( out, player, player_len );
      out = chain_memcpy( out, " ", 1 );
      out = chain_memcpy( out, message, message_len );
      out = chain_memcpy( out, "\n", 2 );

      return ret;
   }

   char* out = pfx_new<char>( 13 + player_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "\"make-cookie ", 13 );
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_lookplayer( const char* player )
{
   size_t player_len = STRLEN( player );

   char* out = pfx_new<char>( 3 + player_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "\"l ", 3 );
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL c2s_enc_cookiesaccept( )
{
   return c2s_enc_stock.sz_cookiesaccept;
}

const char* YIFFURC_CALL c2s_enc_cookiesreject( )
{
   return c2s_enc_stock.sz_cookiesreject;
}

const char* YIFFURC_CALL c2s_enc_eatcookie( )
{
   return c2s_enc_stock.sz_eatcookie;
}

const char* YIFFURC_CALL c2s_enc_munchcookie( )
{
   return c2s_enc_stock.sz_munchcookie;
}

const char* YIFFURC_CALL c2s_enc_shouttoggle( )
{
   return c2s_enc_stock.sz_shouttoggle;
}

const char* YIFFURC_CALL c2s_enc_decline( )
{
   return c2s_enc_stock.sz_decline;
}

const char* YIFFURC_CALL c2s_enc_pickup( )
{
   return c2s_enc_stock.sz_pickup;
}

const char* YIFFURC_CALL c2s_enc_goalleg( )
{
   return c2s_enc_stock.sz_goalleg;
}

const char* YIFFURC_CALL c2s_enc_goback( )
{
   return c2s_enc_stock.sz_goback;
}

const char* YIFFURC_CALL c2s_enc_govinca( )
{
   return c2s_enc_stock.sz_govinca;
}

const char* YIFFURC_CALL c2s_enc_info( )
{
   return c2s_enc_stock.sz_info;
}

const char* YIFFURC_CALL c2s_enc_onlineprx( )
{
   return c2s_enc_stock.sz_onlineprx;
}

const char* YIFFURC_CALL c2s_enc_use( )
{
   return c2s_enc_stock.sz_use;
}

const char* YIFFURC_CALL c2s_enc_downloadfinished( )
{
   return c2s_enc_stock.sz_downloadfinished;
}

const char* YIFFURC_CALL c2s_enc_whichinfo( )
{
   return c2s_enc_stock.sz_whichinfo;
}

const char* YIFFURC_CALL c2s_enc_whoinfo( )
{
   return c2s_enc_stock.sz_whoinfo;
}

const char* YIFFURC_CALL c2s_enc_dragonbreath( )
{
   return c2s_enc_stock.sz_dragonbreath;
}

const char* YIFFURC_CALL c2s_enc_gryffeeagle( )
{
   return c2s_enc_stock.sz_gryffeeagle;
}

const char* YIFFURC_CALL c2s_enc_phoenixflame( )
{
   return c2s_enc_stock.sz_phoenixflame;
}

const char* YIFFURC_CALL c2s_enc_portraitchange( )
{
   return c2s_enc_stock.sz_portraitchange;
}

const char* YIFFURC_CALL c2s_enc_wings( )
{
   return c2s_enc_stock.sz_wings;
}

const char* YIFFURC_CALL c2s_enc_turnleft( )
{
   return c2s_enc_stock.sz_turnleft;
}

const char* YIFFURC_CALL c2s_enc_turnright( )
{
   return c2s_enc_stock.sz_turnright;
}

const char* YIFFURC_CALL c2s_enc_sitstandlie( )
{
   return c2s_enc_stock.sz_sitstandlie;
}

const char* YIFFURC_CALL c2s_enc_liedown( )
{
   return c2s_enc_stock.sz_liedown;
}

const char* YIFFURC_CALL c2s_enc_move( Move_Dir direction )
{
   switch (direction)
   {
   case MOVEDIR_NE:
      return c2s_enc_stock.sz_movene;
   case MOVEDIR_NW:
      return c2s_enc_stock.sz_movenw;
   case MOVEDIR_SE:
      return c2s_enc_stock.sz_movese;
   case MOVEDIR_SW:
      return c2s_enc_stock.sz_movesw;
   default:
      break;
   }

   return 0;
}

const char* YIFFURC_CALL c2s_enc_sitdown( )
{
   return c2s_enc_stock.sz_sitdown;
}

const char* YIFFURC_CALL c2s_enc_standup( )
{
   return c2s_enc_stock.sz_standup;
}

const char* YIFFURC_CALL c2s_enc_showdreamurl( )
{
   return c2s_enc_stock.sz_showdreamurl;
}

const char* YIFFURC_CALL c2s_enc_keepalive( )
{
   return c2s_enc_stock.sz_keepalive;
}

const char* YIFFURC_CALL c2s_enc_quit( )
{
   return c2s_enc_stock.sz_quit;
}

const char* YIFFURC_CALL c2s_enc_uploadrequest( )
{
   return c2s_enc_stock.sz_uploadrequest;
}

const char* YIFFURC_CALL c2s_enc_time( )
{
   return c2s_enc_stock.sz_time;
}

const char* YIFFURC_CALL c2s_enc_whisperson( )
{
   return c2s_enc_stock.sz_whisperson;
}

const char* YIFFURC_CALL c2s_enc_whispersoff( )
{
   return c2s_enc_stock.sz_whispersoff;
}

const char* YIFFURC_CALL c2s_enc_cookieson( )
{
   return c2s_enc_stock.sz_cookieson;
}

const char* YIFFURC_CALL c2s_enc_cookiesoff( )
{
   return c2s_enc_stock.sz_cookiesoff;
}

const char* YIFFURC_CALL c2s_enc_dreampadsall( )
{
   return c2s_enc_stock.sz_dreampadsall;
}

const char* YIFFURC_CALL c2s_enc_dreampadsowner( )
{
   return c2s_enc_stock.sz_dreampadsowner;
}

const char* YIFFURC_CALL c2s_enc_dreampadsshared( )
{
   return c2s_enc_stock.sz_dreampadsshared;
}

const char* YIFFURC_CALL c2s_enc_shieldsummon( )
{
   return c2s_enc_stock.sz_shieldsummon;
}

const char* YIFFURC_CALL c2s_enc_shieldurl( )
{
   return c2s_enc_stock.sz_shieldurl;
}

const char* YIFFURC_CALL c2s_enc_unloaddream( )
{
   return c2s_enc_stock.sz_unloaddream;
}

const char* YIFFURC_CALL c2s_enc_unloadalldreams( )
{
   return c2s_enc_stock.sz_unloadalldreams;
}

const char* YIFFURC_CALL c2s_enc_dreamuploadsall( )
{
   return c2s_enc_stock.sz_dreamuploadsall;
}

const char* YIFFURC_CALL c2s_enc_dreamuploadsowner( )
{
   return c2s_enc_stock.sz_dreamuploadsowner;
}

const char* YIFFURC_CALL c2s_enc_dreamuploadsshared( )
{
   return c2s_enc_stock.sz_dreamuploadsshared;
}

const char* YIFFURC_CALL c2s_enc_dreamuploadssponsors( )
{
   return c2s_enc_stock.sz_dreamuploadssponsors;
}

const char* YIFFURC_CALL c2s_enc_joinbeekchannel( )
{
   return c2s_enc_stock.sz_joinbeekchannel;
}

const char* YIFFURC_CALL c2s_enc_leavebeekchannel( )
{
   return c2s_enc_stock.sz_leavebeekchannel;
}

const char* YIFFURC_CALL c2s_enc_listbeekcommands( )
{
   return c2s_enc_stock.sz_listbeekcommands;
}

const char* YIFFURC_CALL c2s_enc_listf( )
{
   return c2s_enc_stock.sz_listf;
}

const char* YIFFURC_CALL c2s_enc_joinguardchannel( )
{
   return c2s_enc_stock.sz_joinguardchannel;
}

const char* YIFFURC_CALL c2s_enc_ondutyinfo( )
{
   return c2s_enc_stock.sz_ondutyinfo;
}

const char* YIFFURC_CALL c2s_enc_listrequests( )
{
   return c2s_enc_stock.sz_listrequests;
}

const char* YIFFURC_CALL c2s_enc_gowelcomemap( )
{
   return c2s_enc_stock.sz_gowelcomemap;
}

const char* YIFFURC_CALL c2s_enc_dragon( )
{
   return c2s_enc_stock.sz_dragon;
}

const char* YIFFURC_CALL c2s_enc_phoenix( )
{
   return c2s_enc_stock.sz_phoenix;
}

} // namespace yiffurc