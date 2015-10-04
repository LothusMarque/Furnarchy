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

#include "yiffurc.h"
#include "pfx_heap.h"

namespace yiffurc {

size_t pfx_count = 0;

void pfx_free( const void* p )
{
   if (!p)
      return;

   const PFX* pfx = (PFX*) ( (const char*) p - PFX::padded_size( ) );

   if (pfx->info.flags & PFX::DESTRUCT)
   {
      switch (pfx->info.clsid)
      {
      default:
         break;
      case S2C_MOTD::CLSID:
         ((const S2C_MOTD*) p)->~S2C_MOTD( ); break;
      case S2C_DRAGONROAR::CLSID:
         ((const S2C_DRAGONROAR*) p)->~S2C_DRAGONROAR( ); break;
      case S2C_MOTDEND::CLSID:
         ((const S2C_MOTDEND*) p)->~S2C_MOTDEND( ); break;
      case S2C_VER::CLSID:
         ((const S2C_VER*) p)->~S2C_VER( ); break;
      case S2C_PASS::CLSID:
         ((const S2C_PASS*) p)->~S2C_PASS( ); break;
      case S2C_CREATEFAIL::CLSID:
         ((const S2C_CREATEFAIL*) p)->~S2C_CREATEFAIL( ); break;
      case S2C_MACID::CLSID:
         ((const S2C_MACID*) p)->~S2C_MACID( ); break;
      case S2C_LOGINFAIL::CLSID:
         ((const S2C_LOGINFAIL*) p)->~S2C_LOGINFAIL( ); break;
      case S2C_CAMSET::CLSID:
         ((const S2C_CAMSET*) p)->~S2C_CAMSET( ); break;
      case S2C_CAMTWEEN::CLSID:
         ((const S2C_CAMTWEEN*) p)->~S2C_CAMTWEEN( ); break;
      case S2C_OBJSET::CLSID:
         ((const S2C_OBJSET*) p)->~S2C_OBJSET( ); break;
      case S2C_AVCREATE::CLSID:
         ((const S2C_AVCREATE*) p)->~S2C_AVCREATE( ); break;
      case S2C_AVTWEEN::CLSID:
         ((const S2C_AVTWEEN*) p)->~S2C_AVTWEEN( ); break;
      case S2C_AVDESTROY::CLSID:
         ((const S2C_AVDESTROY*) p)->~S2C_AVDESTROY( ); break;
      case S2C_DREAMPOS::CLSID:
         ((const S2C_DREAMPOS*) p)->~S2C_DREAMPOS( ); break;
      case S2C_DREAMPOS2::CLSID:
         ((const S2C_DREAMPOS2*) p)->~S2C_DREAMPOS2( ); break;
      case S2C_DREAMCLEAR::CLSID:
         ((const S2C_DREAMCLEAR*) p)->~S2C_DREAMCLEAR( ); break;
      case S2C_VARSET::CLSID:
         ((const S2C_VARSET*) p)->~S2C_VARSET( ); break;
      case S2C_FLOORSET::CLSID:
         ((const S2C_FLOORSET*) p)->~S2C_FLOORSET( ); break;
      case S2C_BADGE::CLSID:
         ((const S2C_BADGE*) p)->~S2C_BADGE( ); break;
      case S2C_SPECITAG::CLSID:
         ((const S2C_SPECITAG*) p)->~S2C_SPECITAG( ); break;
      case S2C_ONLINERESULT::CLSID:
         ((const S2C_ONLINERESULT*) p)->~S2C_ONLINERESULT( ); break;
      case S2C_PORTRAIT::CLSID:
         ((const S2C_PORTRAIT*) p)->~S2C_PORTRAIT( ); break;
      case S2C_FOOTOBJ::CLSID:
         ((const S2C_FOOTOBJ*) p)->~S2C_FOOTOBJ( ); break;
      case S2C_INVOBJ::CLSID:
         ((const S2C_INVOBJ*) p)->~S2C_INVOBJ( ); break;
      case S2C_FETCHPID::CLSID:
         ((const S2C_FETCHPID*) p)->~S2C_FETCHPID( ); break;
      case S2C_FETCHDREAM::CLSID:
         ((const S2C_FETCHDREAM*) p)->~S2C_FETCHDREAM( ); break;
      case S2C_USEDREAM::CLSID:
         ((const S2C_USEDREAM*) p)->~S2C_USEDREAM( ); break;
      case S2C_USEBGFILE::CLSID:
         ((const S2C_USEBGFILE*) p)->~S2C_USEBGFILE( ); break;
      case S2C_USEMAPFILE::CLSID:
         ((const S2C_USEMAPFILE*) p)->~S2C_USEMAPFILE( ); break;
      case S2C_PLAYSONG::CLSID:
         ((const S2C_PLAYSONG*) p)->~S2C_PLAYSONG( ); break;
      case S2C_PLAYSOUND::CLSID:
         ((const S2C_PLAYSOUND*) p)->~S2C_PLAYSOUND( ); break;
      case S2C_SPEECH::CLSID:
         ((const S2C_SPEECH*) p)->~S2C_SPEECH( ); break;
      case S2C_WHISPER::CLSID:
         ((const S2C_WHISPER*) p)->~S2C_WHISPER( ); break;
      case S2C_SHOUT::CLSID:
         ((const S2C_SHOUT*) p)->~S2C_SHOUT( ); break;
      case S2C_EMOTE::CLSID:
         ((const S2C_EMOTE*) p)->~S2C_EMOTE( ); break;
      case S2C_EMIT::CLSID:
         ((const S2C_EMIT*) p)->~S2C_EMIT( ); break;
      case S2C_ROLL::CLSID:
         ((const S2C_ROLL*) p)->~S2C_ROLL( ); break;
      case S2C_VERBOSEROLL::CLSID:
         ((const S2C_VERBOSEROLL*) p)->~S2C_VERBOSEROLL( ); break;
      case S2C_DESCHEAD::CLSID:
         ((const S2C_DESCHEAD*) p)->~S2C_DESCHEAD( ); break;
      case S2C_DESCBODY::CLSID:
         ((const S2C_DESCBODY*) p)->~S2C_DESCBODY( ); break;
      case S2C_DESCBODY2::CLSID:
         ((const S2C_DESCBODY2*) p)->~S2C_DESCBODY2( ); break;
      case S2C_WHODAILYMAX::CLSID:
         ((const S2C_WHODAILYMAX*) p)->~S2C_WHODAILYMAX( ); break;
      case S2C_WHOMAX::CLSID:
         ((const S2C_WHOMAX*) p)->~S2C_WHOMAX( ); break;
      case S2C_WHOUPTIME::CLSID:
         ((const S2C_WHOUPTIME*) p)->~S2C_WHOUPTIME( ); break;
      case S2C_COOKIESREADY::CLSID:
         ((const S2C_COOKIESREADY*) p)->~S2C_COOKIESREADY( ); break;
      case S2C_COOKIE::CLSID:
         ((const S2C_COOKIE*) p)->~S2C_COOKIE( ); break;
      case S2C_INFOITEM::CLSID:
         ((const S2C_INFOITEM*) p)->~S2C_INFOITEM( ); break;
      case S2C_INFOPLAYER::CLSID:
         ((const S2C_INFOPLAYER*) p)->~S2C_INFOPLAYER( ); break;
      case S2C_SUMMON::CLSID:
         ((const S2C_SUMMON*) p)->~S2C_SUMMON( ); break;
      case S2C_JOIN::CLSID:
         ((const S2C_JOIN*) p)->~S2C_JOIN( ); break;
      case S2C_WHISPERFAIL::CLSID:
         ((const S2C_WHISPERFAIL*) p)->~S2C_WHISPERFAIL( ); break;
      case S2C_WHISPERFAILEXACT::CLSID:
         ((const S2C_WHISPERFAILEXACT*) p)->~S2C_WHISPERFAILEXACT( ); break;
      case S2C_WHISPERFAILMORE::CLSID:
         ((const S2C_WHISPERFAILMORE*) p)->~S2C_WHISPERFAILMORE( ); break;
      case S2C_YOUWHISPER::CLSID:
         ((const S2C_YOUWHISPER*) p)->~S2C_YOUWHISPER( ); break;
      case S2C_CHAT::CLSID:
         ((const S2C_CHAT*) p)->~S2C_CHAT( ); break;
      case S2C_DISPLISTOPEN::CLSID:
         ((const S2C_DISPLISTOPEN*) p)->~S2C_DISPLISTOPEN( ); break;
      case S2C_DISPLISTCLOSE::CLSID:
         ((const S2C_DISPLISTCLOSE*) p)->~S2C_DISPLISTCLOSE( ); break;
      case S2C_SYNCCOLORCODE::CLSID:
         ((const S2C_SYNCCOLORCODE*) p)->~S2C_SYNCCOLORCODE( ); break;
      case S2C_YOUSHOUT::CLSID:
         ((const S2C_YOUSHOUT*) p)->~S2C_YOUSHOUT( ); break;
      case S2C_YOUSAY::CLSID:
         ((const S2C_YOUSAY*) p)->~S2C_YOUSAY( ); break;
      case S2C_TRIGGER::CLSID:
         ((const S2C_TRIGGER*) p)->~S2C_TRIGGER( ); break;
      case S2C_TRIGGERSELF::CLSID:
         ((const S2C_TRIGGERSELF*) p)->~S2C_TRIGGERSELF( ); break;
      case S2C_VERREQ::CLSID:
         ((const S2C_VERREQ*) p)->~S2C_VERREQ( ); break;
      case S2C_PING::CLSID:
         ((const S2C_PING*) p)->~S2C_PING( ); break;
      case S2C_WHISPERDENIED::CLSID:
         ((const S2C_WHISPERDENIED*) p)->~S2C_WHISPERDENIED( ); break;
      case S2C_SUMMONJOINECHO::CLSID:
         ((const S2C_SUMMONJOINECHO*) p)->~S2C_SUMMONJOINECHO( ); break;
      case S2C_DECLINED::CLSID:
         ((const S2C_DECLINED*) p)->~S2C_DECLINED( ); break;
      case S2C_DECLINEECHO::CLSID:
         ((const S2C_DECLINEECHO*) p)->~S2C_DECLINEECHO( ); break;
      case S2C_DECLINEFAIL::CLSID:
         ((const S2C_DECLINEFAIL*) p)->~S2C_DECLINEFAIL( ); break;
      case S2C_MSGBOX::CLSID:
         ((const S2C_MSGBOX*) p)->~S2C_MSGBOX( ); break;
      case S2C_AVSHOW::CLSID:
         ((const S2C_AVSHOW*) p)->~S2C_AVSHOW( ); break;
      case S2C_AVUPDATECOLORS::CLSID:
         ((const S2C_AVUPDATECOLORS*) p)->~S2C_AVUPDATECOLORS( ); break;
      case S2C_AVHIDE::CLSID:
         ((const S2C_AVHIDE*) p)->~S2C_AVHIDE( ); break;
      case S2C_PRINTPLAYERS::CLSID:
         ((const S2C_PRINTPLAYERS*) p)->~S2C_PRINTPLAYERS( ); break;
      case S2C_AVSHOW2::CLSID:
         ((const S2C_AVSHOW2*) p)->~S2C_AVSHOW2( ); break;
      case S2C_DSEMIT::CLSID:
         ((const S2C_DSEMIT*) p)->~S2C_DSEMIT( ); break;

      case C2S_KEEPALIVE::CLSID:
         ((const C2S_KEEPALIVE*) p)->~C2S_KEEPALIVE( ); break;
      case C2S_CREATEPLAYER::CLSID:
         ((const C2S_CREATEPLAYER*) p)->~C2S_CREATEPLAYER( ); break;
      case C2S_LOGIN::CLSID:
         ((const C2S_LOGIN*) p)->~C2S_LOGIN( ); break;
      case C2S_QUIT::CLSID:
         ((const C2S_QUIT*) p)->~C2S_QUIT( ); break;
      case C2S_SETCOLOR::CLSID:
         ((const C2S_SETCOLOR*) p)->~C2S_SETCOLOR( ); break;
      case C2S_CHANGECOLOR::CLSID:
         ((const C2S_CHANGECOLOR*) p)->~C2S_CHANGECOLOR( ); break;
      case C2S_SETDESC::CLSID:
         ((const C2S_SETDESC*) p)->~C2S_SETDESC( ); break;
      case C2S_CHANGEDESC::CLSID:
         ((const C2S_CHANGEDESC*) p)->~C2S_CHANGEDESC( ); break;
      case C2S_PONG::CLSID:
         ((const C2S_PONG*) p)->~C2S_PONG( ); break;
      case C2S_VERSION::CLSID:
         ((const C2S_VERSION*) p)->~C2S_VERSION( ); break;
      case C2S_WINVER::CLSID:
         ((const C2S_WINVER*) p)->~C2S_WINVER( ); break;
      case C2S_DOWNLOADFINISHED::CLSID:
         ((const C2S_DOWNLOADFINISHED*) p)->~C2S_DOWNLOADFINISHED( ); break;
      case C2S_MOVE::CLSID:
         ((const C2S_MOVE*) p)->~C2S_MOVE( ); break;
      case C2S_SITDOWN::CLSID:
         ((const C2S_SITDOWN*) p)->~C2S_SITDOWN( ); break;
      case C2S_STANDUP::CLSID:
         ((const C2S_STANDUP*) p)->~C2S_STANDUP( ); break;
      case C2S_LIEDOWN::CLSID:
         ((const C2S_LIEDOWN*) p)->~C2S_LIEDOWN( ); break;
      case C2S_SITSTANDLIE::CLSID:
         ((const C2S_SITSTANDLIE*) p)->~C2S_SITSTANDLIE( ); break;
      case C2S_SUMMON::CLSID:
         ((const C2S_SUMMON*) p)->~C2S_SUMMON( ); break;
      case C2S_JOIN::CLSID:
         ((const C2S_JOIN*) p)->~C2S_JOIN( ); break;
      case C2S_DECLINE::CLSID:
         ((const C2S_DECLINE*) p)->~C2S_DECLINE( ); break;
      case C2S_GOMAP::CLSID:
         ((const C2S_GOMAP*) p)->~C2S_GOMAP( ); break;
      case C2S_GOBACK::CLSID:
         ((const C2S_GOBACK*) p)->~C2S_GOBACK( ); break;
      case C2S_GOALLEG::CLSID:
         ((const C2S_GOALLEG*) p)->~C2S_GOALLEG( ); break;
      case C2S_GOVINCA::CLSID:
         ((const C2S_GOVINCA*) p)->~C2S_GOVINCA( ); break;
      case C2S_GOWELCOMEMAP::CLSID:
         ((const C2S_GOWELCOMEMAP*) p)->~C2S_GOWELCOMEMAP( ); break;
      case C2S_SPEECH::CLSID:
         ((const C2S_SPEECH*) p)->~C2S_SPEECH( ); break;
      case C2S_SHOUT::CLSID:
         ((const C2S_SHOUT*) p)->~C2S_SHOUT( ); break;
      case C2S_SHOUTTOGGLE::CLSID:
         ((const C2S_SHOUTTOGGLE*) p)->~C2S_SHOUTTOGGLE( ); break;
      case C2S_WHISPER::CLSID:
         ((const C2S_WHISPER*) p)->~C2S_WHISPER( ); break;
      case C2S_WINGS::CLSID:
         ((const C2S_WINGS*) p)->~C2S_WINGS( ); break;
      case C2S_DRAGON::CLSID:
         ((const C2S_DRAGON*) p)->~C2S_DRAGON( ); break;
      case C2S_PHOENIX::CLSID:
         ((const C2S_PHOENIX*) p)->~C2S_PHOENIX( ); break;
      case C2S_PORTRAITCHANGE::CLSID:
         ((const C2S_PORTRAITCHANGE*) p)->~C2S_PORTRAITCHANGE( ); break;
      case C2S_EAGLE::CLSID:
         ((const C2S_EAGLE*) p)->~C2S_EAGLE( ); break;
      case C2S_GRYFFE::CLSID:
         ((const C2S_GRYFFE*) p)->~C2S_GRYFFE( ); break;
      case C2S_DRAGONBREATH::CLSID:
         ((const C2S_DRAGONBREATH*) p)->~C2S_DRAGONBREATH( ); break;
      case C2S_PHOENIXFLAME::CLSID:
         ((const C2S_PHOENIXFLAME*) p)->~C2S_PHOENIXFLAME( ); break;
      case C2S_LOOKPOS::CLSID:
         ((const C2S_LOOKPOS*) p)->~C2S_LOOKPOS( ); break;
      case C2S_WHOINFO::CLSID:
         ((const C2S_WHOINFO*) p)->~C2S_WHOINFO( ); break;
      case C2S_PICKUP::CLSID:
         ((const C2S_PICKUP*) p)->~C2S_PICKUP( ); break;
      case C2S_USE::CLSID:
         ((const C2S_USE*) p)->~C2S_USE( ); break;
      case C2S_FDL::CLSID:
         ((const C2S_FDL*) p)->~C2S_FDL( ); break;
      case C2S_LOOKPLAYER::CLSID:
         ((const C2S_LOOKPLAYER*) p)->~C2S_LOOKPLAYER( ); break;
      case C2S_ONLINECHECK::CLSID:
         ((const C2S_ONLINECHECK*) p)->~C2S_ONLINECHECK( ); break;
      case C2S_EMOTE::CLSID:
         ((const C2S_EMOTE*) p)->~C2S_EMOTE( ); break;
      case C2S_TURNLEFT::CLSID:
         ((const C2S_TURNLEFT*) p)->~C2S_TURNLEFT( ); break;
      case C2S_TURNRIGHT::CLSID:
         ((const C2S_TURNRIGHT*) p)->~C2S_TURNRIGHT( ); break;

      case XFER_S2C_GREETING::CLSID:
         ((const XFER_S2C_GREETING*) p)->~XFER_S2C_GREETING( ); break;
      case XFER_S2C_USEROK::CLSID:
         ((const XFER_S2C_USEROK*) p)->~XFER_S2C_USEROK( ); break;
      case XFER_S2C_LOGGEDIN::CLSID:
         ((const XFER_S2C_LOGGEDIN*) p)->~XFER_S2C_LOGGEDIN( ); break;
      case XFER_S2C_UFPSET::CLSID:
         ((const XFER_S2C_UFPSET*) p)->~XFER_S2C_UFPSET( ); break;
      case XFER_S2C_UPLOADOK::CLSID:
         ((const XFER_S2C_UPLOADOK*) p)->~XFER_S2C_UPLOADOK( ); break;
      case XFER_S2C_CHUNKOK::CLSID:
         ((const XFER_S2C_CHUNKOK*) p)->~XFER_S2C_CHUNKOK( ); break;
      case XFER_S2C_UPLOADDONE::CLSID:
         ((const XFER_S2C_UPLOADDONE*) p)->~XFER_S2C_UPLOADDONE( ); break;
      case XFER_S2C_LOGGEDOUT::CLSID:
         ((const XFER_S2C_LOGGEDOUT*) p)->~XFER_S2C_LOGGEDOUT( ); break;
      case XFER_S2C_DOWNLOADINFO::CLSID:
         ((const XFER_S2C_DOWNLOADINFO*) p)->~XFER_S2C_DOWNLOADINFO( ); break;
      case XFER_S2C_CHUNKHEAD::CLSID:
         ((const XFER_S2C_CHUNKHEAD*) p)->~XFER_S2C_CHUNKHEAD( ); break;
      case XFER_S2C_INACTIVITY::CLSID:
         ((const XFER_S2C_INACTIVITY*) p)->~XFER_S2C_INACTIVITY( ); break;
      case XFER_S2C_FILENOTFOUND::CLSID:
         ((const XFER_S2C_FILENOTFOUND*) p)->~XFER_S2C_FILENOTFOUND( ); break;
      }
   }

   ::operator delete( const_cast<void*>( (const void*) pfx ) );
   pfx_count--;
}

void YIFFURC_CALL yiffurc_release( const void* p )
{
   pfx_release( p );
}

} // namespace yiffurc