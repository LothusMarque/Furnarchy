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
#include "net_regex.h"
#include "misc.h"

namespace yiffurc {

// Instantiate regexes on load.
static C2S_RE g_re;

const C2S* YIFFURC_CALL c2s_parse_peek( const char* line, int len,
                                     const NET_SESSION* session )
{
   NET_SESSION tmp = *session;
   return c2s_parse( line, len, &tmp );
}

const C2S* YIFFURC_CALL c2s_parse( const char* line, int len,
                                NET_SESSION* session )
{
   ++session->c2s_state.line_count;

   // Substring offsets.
   SUBS_T subs;

   // Nice, helpful macros.
   #define REGXEC( re ) (0 <= pcre_exec( re, NULL, line, (int) len, 0, 0, subs.ints, SUBS_T::MAX * 3 ))

   #define SUBCPY( dst, pair ) (memcpy( (dst), line + subs.sub[ (pair) ].so, \
                                        subs.sub[ (pair) ].eo - subs.sub[ (pair) ].so ))

   #define SUBOFF( pair ) (subs.sub[ (pair) ].so)

   #define SUBSTR( pair ) (line + subs.sub[ (pair) ].so)

   #define SUBLEN( pair ) (subs.sub[ (pair) ].eo - subs.sub[ (pair) ].so)

   #define SUBNEW( pair ) (strnnew( line + subs.sub[ (pair) ].so, \
                                   subs.sub[ (pair) ].eo - subs.sub[ (pair) ].so ))

   switch (session->level)
   {
      /* Just connected, not past the MOTD, yet. */
   case NET_LEVEL_CONNECTED:
      /* Past the MOTD. */
   case NET_LEVEL_MOTDEND:
      /* The client generally takes for granted that a login will work and
      starts sending stuff right away. */
   case NET_LEVEL_LOGGEDIN:

      /* Sort by first letter. */
      switch (line[ 0 ])
      {
      case 'c':
         if (session->level == NET_LEVEL_MOTDEND && REGXEC( g_re.CREATEPLAYER ))
         {
            ++session->create_count;

            C2S_CREATEPLAYER* proto = pfx_new_obj<C2S_CREATEPLAYER>( );
            proto->player     = SUBNEW( 1 );
            proto->password   = SUBNEW( 2 );
            proto->email      = SUBNEW( 3 );
            return (C2S*) proto;
         }
         if (session->level == NET_LEVEL_MOTDEND && REGXEC( g_re.LOGIN ))
         {
            C2S_LOGIN* proto = pfx_new_obj<C2S_LOGIN>( );
            proto->player     = SUBNEW( 1 );
            proto->password   = SUBNEW( 2 );
            proto->mac_id     = SUBNEW( 3 );
            return (C2S*) proto;
         }

         if (REGXEC( g_re.SETCOLOR ))
         {
            C2S_SETCOLOR* proto = pfx_new_obj<C2S_SETCOLOR>( );
            const int color_len = get_color_len( SUBSTR( 1 ), SUBLEN( 1 ) );
            proto->colors = strnnew( SUBSTR( 1 ), color_len );
            if (SUBOFF( 1 ) + color_len < len)
            {
               memcpy( proto->gsd, SUBSTR( 1 ) + color_len, 3 );
               return (C2S*) proto;
            }
            delete proto;
            break;
         }
         if (REGXEC( g_re.CHANGECOLOR ))
         {
            C2S_CHANGECOLOR* proto = pfx_new_obj<C2S_CHANGECOLOR>( );
            const int color_len = get_color_len( SUBSTR( 1 ), SUBLEN( 1 ) );
            proto->colors = strnnew( SUBSTR( 1 ), color_len );
            if (SUBOFF( 1 ) + color_len < len)
            {
               memcpy( proto->gsd, SUBSTR( 1 ) + color_len, 3 );
               return (C2S*) proto;
            }
            delete proto;
            break;
         }
         if (REGXEC( g_re.CHANGEDESC ))
         {
            C2S_CHANGEDESC* proto = pfx_new_obj<C2S_CHANGEDESC>( );
            proto->desc = SUBNEW( 1 );
            return (C2S*) proto;
         }
         break;

      case 'i':
         if (REGXEC( g_re.KEEPALIVE ))
         {
            C2S_KEEPALIVE* proto = pfx_new_obj<C2S_KEEPALIVE>( );
            return (C2S*) proto;
         }
         break;

      case '"':
         if (REGXEC( g_re.LOOKPLAYER ))
         {
            C2S_LOOKPLAYER* proto = pfx_new_obj<C2S_LOOKPLAYER>( );
            proto->exact  = SUBLEN( 1 ) > 0;
            proto->player = SUBNEW( 2 );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.SPEECH ))
         {
            C2S_SPEECH* proto = pfx_new_obj<C2S_SPEECH>( );
            proto->message = SUBNEW( 1 );
            return (C2S*) proto;
         }
         break;

      case ':':
         if (REGXEC( g_re.EMOTE ))
         {
            C2S_EMOTE* proto = pfx_new_obj<C2S_EMOTE>( );
            proto->message = SUBNEW( 1 );
            return (C2S*) proto;
         }
         break;

      case '-':
         if (REGXEC( g_re.SHOUTTOGGLE ))
         {
            C2S_SHOUTTOGGLE* proto = pfx_new_obj<C2S_SHOUTTOGGLE>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.SHOUT ))
         {
            C2S_SHOUT* proto = pfx_new_obj<C2S_SHOUT>( );
            proto->message = SUBNEW( 1 );
            return (C2S*) proto;
         }
         break;

      case 'W':
         if (REGXEC( g_re.WINVER ))
         {
            C2S_WINVER* proto = pfx_new_obj<C2S_WINVER>( );
            proto->windows.major = antoi( SUBSTR( 1 ), SUBLEN( 1 ) );
            proto->windows.minor = antoi( SUBSTR( 2 ), SUBLEN( 2 ) );
            proto->build         = antoi( SUBSTR( 3 ), SUBLEN( 3 ) );
            proto->directx.major = antoi( SUBSTR( 4 ), SUBLEN( 4 ) );
            proto->directx.minor = antoi( SUBSTR( 5 ), SUBLEN( 5 ) );
            return (C2S*) proto;
         }
         break;

      case 'b':
         if (REGXEC( g_re.DRAGONBREATH ))
         {
            C2S_DRAGONBREATH* proto = pfx_new_obj<C2S_DRAGONBREATH>( );
            return (C2S*) proto;
         }
         break;

      case 'd':
         if (REGXEC( g_re.SETDESC ))
         {
            C2S_SETDESC* proto = pfx_new_obj<C2S_SETDESC>( );
            proto->desc = SUBNEW( 1 );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.DRAGON ))
         {
            C2S_DRAGON* proto = pfx_new_obj<C2S_DRAGON>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.DECLINE ))
         {
            C2S_DECLINE* proto = pfx_new_obj<C2S_DECLINE>( );
            return (C2S*) proto;
         }
         break;

      case 'e':
         if (REGXEC( g_re.EAGLE ))
         {
            C2S_EAGLE* proto = pfx_new_obj<C2S_EAGLE>( );
            return (C2S*) proto;
         }
         break;

      case 'f':
         if (REGXEC( g_re.PHOENIXFLAME ))
         {
            C2S_PHOENIXFLAME* proto = pfx_new_obj<C2S_PHOENIXFLAME>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.FDL ))
         {
            C2S_FDL* proto = pfx_new_obj<C2S_FDL>( );
            proto->url = SUBNEW( 1 );
            return (C2S*) proto;
         }
         break;

      case 'g':
         if (REGXEC( g_re.GOALLEG ))
         {
            C2S_GOALLEG* proto = pfx_new_obj<C2S_GOALLEG>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.GOBACK ))
         {
            C2S_GOBACK* proto = pfx_new_obj<C2S_GOBACK>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.GOVINCA ))
         {
            C2S_GOVINCA* proto = pfx_new_obj<C2S_GOVINCA>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.GOMAP ))
         {
            C2S_GOMAP* proto = pfx_new_obj<C2S_GOMAP>( );
            proto->map_num = (*SUBSTR( 1 )) - ' ';
            return (C2S*) proto;
         }
         if (REGXEC( g_re.PICKUP ))
         {
            C2S_PICKUP* proto = pfx_new_obj<C2S_PICKUP>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.GRYFFE ))
         {
            C2S_GRYFFE* proto = pfx_new_obj<C2S_GRYFFE>( );
            return (C2S*) proto;
         }
         break;

      case 'j':
         if (REGXEC( g_re.JOIN ))
         {
            C2S_JOIN* proto = pfx_new_obj<C2S_JOIN>( );
            proto->exact  = SUBLEN( 1 ) > 0;
            proto->player = SUBNEW( 2 );
            return (C2S*) proto;
         }
         break;

      case 'l':
         if (REGXEC( g_re.SITSTANDLIE ))
         {
            C2S_SITSTANDLIE* proto = pfx_new_obj<C2S_SITSTANDLIE>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.LIEDOWN ))
         {
            C2S_LIEDOWN* proto = pfx_new_obj<C2S_LIEDOWN>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.LOOKPOS ))
         {
            C2S_LOOKPOS* proto = pfx_new_obj<C2S_LOOKPOS>( );
            proto->x = ftoi2( SUBSTR( 1 ) );
            proto->y = ftoi2( SUBSTR( 2 ) );
            return (C2S*) proto;
         }
         break;

      case 'm':
         if (REGXEC( g_re.MOVE ))
         {
            C2S_MOVE* proto = pfx_new_obj<C2S_MOVE>( );
            proto->direction = (Move_Dir) (*SUBSTR( 1 )); // See Move_Dir why
            return (C2S*) proto;
         }
         break;

      case 'o':
         if (REGXEC( g_re.ONLINECHECK ))
         {
            C2S_ONLINECHECK* proto = pfx_new_obj<C2S_ONLINECHECK>( );
            proto->player = SUBNEW( 1 );
            return (C2S*) proto;
         }
         break;

      case 'p':
         if (REGXEC( g_re.PHOENIX ))
         {
            C2S_PHOENIX* proto = pfx_new_obj<C2S_PHOENIX>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.PONG ))
         {
            C2S_PONG* proto = pfx_new_obj<C2S_PONG>( );
            proto->value = antoi( SUBSTR( 1 ), SUBLEN( 1 ) );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.PORTRAITCHANGE ))
         {
            C2S_PORTRAITCHANGE* proto = pfx_new_obj<C2S_PORTRAITCHANGE>( );
            return (C2S*) proto;
         }
         break;

      case 'q':
         if (REGXEC( g_re.QUIT ))
         {
            C2S_QUIT* proto = pfx_new_obj<C2S_QUIT>( );
            return (C2S*) proto;
         }
         break;

      case 's':
         if (REGXEC( g_re.SUMMON ))
         {
            C2S_SUMMON* proto = pfx_new_obj<C2S_SUMMON>( );
            proto->exact  = SUBLEN( 1 ) > 0;
            proto->player = SUBNEW( 2 );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.STANDUP ))
         {
            C2S_STANDUP* proto = pfx_new_obj<C2S_STANDUP>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.SITDOWN ))
         {
            C2S_SITDOWN* proto = pfx_new_obj<C2S_SITDOWN>( );
            return (C2S*) proto;
         }
         break;

      case 'u':
         if (REGXEC( g_re.USE ))
         {
            C2S_USE* proto = pfx_new_obj<C2S_USE>( );
            return (C2S*) proto;
         }
         break;

      case 'v':
         if (REGXEC( g_re.DOWNLOADFINISHED ))
         {
            C2S_DOWNLOADFINISHED* proto = pfx_new_obj<C2S_DOWNLOADFINISHED>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.VERSION ))
         {
            C2S_VERSION* proto = pfx_new_obj<C2S_VERSION>( );
            proto->client.major = antoi( SUBSTR( 1 ), SUBLEN( 1 ) );
            proto->client.minor = antoi( SUBSTR( 2 ), SUBLEN( 2 ) );
            return (C2S*) proto;
         }
         break;

      case 'w':
         if (REGXEC( g_re.WHISPER ))
         {
            C2S_WHISPER* proto = pfx_new_obj<C2S_WHISPER>( );
            proto->exact   = SUBLEN( 1 ) > 0;
            proto->player  = SUBNEW( 2 );
            proto->message = SUBNEW( 3 );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.WINGS ))
         {
            C2S_WINGS* proto = pfx_new_obj<C2S_WINGS>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.WHOINFO ))
         {
            C2S_WHOINFO* proto = pfx_new_obj<C2S_WHOINFO>( );
            return (C2S*) proto;
         }
         if (REGXEC( g_re.GOWELCOMEMAP ))
         {
            C2S_GOWELCOMEMAP* proto = pfx_new_obj<C2S_GOWELCOMEMAP>( );
            return (C2S*) proto;
         }
         break;

      case '<':
         if (REGXEC( g_re.TURNLEFT ))
         {
            C2S_TURNLEFT* proto = pfx_new_obj<C2S_TURNLEFT>( );
            return (C2S*) proto;
         }
         break;

      case '>':
         if (REGXEC( g_re.TURNRIGHT ))
         {
            C2S_TURNRIGHT* proto = pfx_new_obj<C2S_TURNRIGHT>( );
            return (C2S*) proto;
         }
         break;

      default:
         break;
      }
      break;

   default:
      break;
   }

   return 0;
}

} // namespace yiffurc