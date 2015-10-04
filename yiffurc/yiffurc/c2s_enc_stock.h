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

#ifndef _YIFFURC_PFX_C2S_ENC_STOCK_H__
#define _YIFFURC_PFX_C2S_ENC_STOCK_H__

#include "pfx_heap.h"
#include "misc.h"
#include <string.h>

namespace yiffurc {

/* Stock encoded client-to-server commands. */
struct C2S_ENC_STOCK
{
   char* sz_cookiesaccept;
   char* sz_cookiesreject;
   char* sz_eatcookie;
   char* sz_munchcookie;
   char* sz_shouttoggle;
   char* sz_decline;
   char* sz_pickup;
   char* sz_goalleg;
   char* sz_goback;
   char* sz_govinca;
   char* sz_info;
   char* sz_onlineprx;
   char* sz_use;
   char* sz_downloadfinished;
   char* sz_whichinfo;
   char* sz_whoinfo;
   char* sz_dragonbreath;
   char* sz_gryffeeagle;
   char* sz_phoenixflame;
   char* sz_portraitchange;
   char* sz_wings;
   char* sz_turnleft;
   char* sz_turnright;
   char* sz_sitstandlie;
   char* sz_liedown;
   char* sz_movene;
   char* sz_movenw;
   char* sz_movesw;
   char* sz_movese;
   char* sz_sitdown;
   char* sz_standup;
   char* sz_showdreamurl;
   char* sz_keepalive;
   char* sz_quit;
   char* sz_uploadrequest;
   char* sz_time;
   char* sz_whisperson;
   char* sz_whispersoff;
   char* sz_cookieson;
   char* sz_cookiesoff;
   char* sz_dreampadsall;
   char* sz_dreampadsowner;
   char* sz_dreampadsshared;
   char* sz_shieldsummon;
   char* sz_shieldurl;
   char* sz_unloaddream;
   char* sz_unloadalldreams;
   char* sz_dreamuploadsall;
   char* sz_dreamuploadsowner;
   char* sz_dreamuploadsshared;
   char* sz_dreamuploadssponsors;
   char* sz_joinbeekchannel;
   char* sz_leavebeekchannel;
   char* sz_listbeekcommands;
   char* sz_listf;
   char* sz_joinguardchannel;
   char* sz_ondutyinfo;
   char* sz_listrequests;
   char* sz_gowelcomemap;
   char* sz_dragon;
   char* sz_phoenix;

   C2S_ENC_STOCK( )
   {
      #define _STOCK_SZ( s ) ( strcpy( pfx_new<char>( strlen( s "\n" ) + 1, true ), s "\n" ) )

      sz_cookiesaccept = _STOCK_SZ( "\"cookies-accept" );
      sz_cookiesreject = _STOCK_SZ( "\"cookies-reject" );
      sz_eatcookie = _STOCK_SZ( "\"eat-cookie" );
      sz_munchcookie = _STOCK_SZ( "\"munch-cookie" );
      sz_shouttoggle = _STOCK_SZ( "-" );
      sz_decline = _STOCK_SZ( "decline" );
      sz_pickup = _STOCK_SZ( "get" );
      sz_goalleg = _STOCK_SZ( "goalleg" );
      sz_goback = _STOCK_SZ( "goback" );
      sz_govinca = _STOCK_SZ( "gostart" );
      sz_info = _STOCK_SZ( "info" );
      sz_onlineprx = _STOCK_SZ( "onlnprx" );
      sz_use = _STOCK_SZ( "use" );
      sz_downloadfinished = _STOCK_SZ( "vascodagama" );
      sz_whichinfo = _STOCK_SZ( "which" );
      sz_whoinfo = _STOCK_SZ( "who" );
      sz_dragonbreath = _STOCK_SZ( "breath" );
      sz_gryffeeagle = _STOCK_SZ( "eagle" );
      sz_phoenixflame = _STOCK_SZ( "flame" );
      sz_portraitchange = _STOCK_SZ( "portrchng" );
      sz_wings = _STOCK_SZ( "wings" );
      sz_turnleft = _STOCK_SZ( "<" );
      sz_turnright = _STOCK_SZ( ">" );
      sz_sitstandlie = _STOCK_SZ( "lie" );
      sz_liedown = _STOCK_SZ( "liedown" );
      sz_movene = _STOCK_SZ( "m 9" );
      sz_movenw = _STOCK_SZ( "m 7" );
      sz_movesw = _STOCK_SZ( "m 1" );
      sz_movese = _STOCK_SZ( "m 3" );
      sz_sitdown = _STOCK_SZ( "sit" );
      sz_standup = _STOCK_SZ( "stand" );
      sz_showdreamurl = _STOCK_SZ( "\"dreamurl" );
      sz_keepalive = _STOCK_SZ( "iamhere" );
      sz_quit = _STOCK_SZ( "quit" );
      sz_uploadrequest = _STOCK_SZ( "rgate" );
      sz_time = _STOCK_SZ( "\"time" );
      sz_whisperson = _STOCK_SZ( "\"whisperson" );
      sz_whispersoff = _STOCK_SZ( "\"whispersoff" );
      sz_cookieson = _STOCK_SZ( "\"cookies-on" );
      sz_cookiesoff = _STOCK_SZ( "\"cookies-off" );
      sz_dreampadsall = _STOCK_SZ( "\"pads-all" );
      sz_dreampadsowner = _STOCK_SZ( "\"pads-owner" );
      sz_dreampadsshared = _STOCK_SZ( "\"pads-shared" );
      sz_shieldsummon = _STOCK_SZ( "shield" );
      sz_shieldurl = _STOCK_SZ( "shieldurl" );
      sz_unloaddream = _STOCK_SZ( "unload" );
      sz_unloadalldreams = _STOCK_SZ( "unload all" );
      sz_dreamuploadsall = _STOCK_SZ( "\"uploads-all" );
      sz_dreamuploadsowner = _STOCK_SZ( "\"uploads-owner" );
      sz_dreamuploadsshared = _STOCK_SZ( "\"uploads-shared" );
      sz_dreamuploadssponsors = _STOCK_SZ( "\"uploads-ss" );
      sz_joinbeekchannel = _STOCK_SZ( "bb" );
      sz_leavebeekchannel = _STOCK_SZ( "leave" );
      sz_listbeekcommands = _STOCK_SZ( "\"list" );
      sz_listf = _STOCK_SZ( "\"listf" );
      sz_joinguardchannel = _STOCK_SZ( "ob" );
      sz_ondutyinfo = _STOCK_SZ( "onduty" );
      sz_listrequests = _STOCK_SZ( "que" );
      sz_gowelcomemap = _STOCK_SZ( "wmap" );
      sz_dragon = _STOCK_SZ( "dragon" );
      sz_phoenix = _STOCK_SZ( "phoenix" );

      #undef _STOCK_SZ
   }
   ~C2S_ENC_STOCK( )
   {
      pfx_free( sz_cookiesaccept );
      pfx_free( sz_cookiesreject );
      pfx_free( sz_eatcookie );
      pfx_free( sz_munchcookie );
      pfx_free( sz_shouttoggle );
      pfx_free( sz_decline );
      pfx_free( sz_pickup );
      pfx_free( sz_goalleg );
      pfx_free( sz_goback );
      pfx_free( sz_govinca );
      pfx_free( sz_info );
      pfx_free( sz_onlineprx );
      pfx_free( sz_use );
      pfx_free( sz_downloadfinished );
      pfx_free( sz_whichinfo );
      pfx_free( sz_whoinfo );
      pfx_free( sz_dragonbreath );
      pfx_free( sz_gryffeeagle );
      pfx_free( sz_phoenixflame );
      pfx_free( sz_portraitchange );
      pfx_free( sz_wings );
      pfx_free( sz_turnleft );
      pfx_free( sz_turnright );
      pfx_free( sz_sitstandlie );
      pfx_free( sz_liedown );
      pfx_free( sz_movene );
      pfx_free( sz_movenw );
      pfx_free( sz_movesw );
      pfx_free( sz_movese );
      pfx_free( sz_sitdown );
      pfx_free( sz_standup );
      pfx_free( sz_showdreamurl );
      pfx_free( sz_keepalive );
      pfx_free( sz_quit );
      pfx_free( sz_uploadrequest );
      pfx_free( sz_time );
      pfx_free( sz_whisperson );
      pfx_free( sz_whispersoff );
      pfx_free( sz_cookieson );
      pfx_free( sz_cookiesoff );
      pfx_free( sz_dreampadsall );
      pfx_free( sz_dreampadsowner );
      pfx_free( sz_dreampadsshared );
      pfx_free( sz_shieldsummon );
      pfx_free( sz_shieldurl );
      pfx_free( sz_unloaddream );
      pfx_free( sz_unloadalldreams );
      pfx_free( sz_dreamuploadsall );
      pfx_free( sz_dreamuploadsowner );
      pfx_free( sz_dreamuploadsshared );
      pfx_free( sz_dreamuploadssponsors );
      pfx_free( sz_joinbeekchannel );
      pfx_free( sz_leavebeekchannel );
      pfx_free( sz_listbeekcommands );
      pfx_free( sz_listf );
      pfx_free( sz_joinguardchannel );
      pfx_free( sz_ondutyinfo );
      pfx_free( sz_listrequests );
      pfx_free( sz_gowelcomemap );
      pfx_free( sz_dragon );
      pfx_free( sz_phoenix );      
   }
};

extern const C2S_ENC_STOCK c2s_enc_stock;

} // namespace yiffurc

#endif // #ifndef _YIFFURC_PFX_C2S_ENC_STOCK_H__
