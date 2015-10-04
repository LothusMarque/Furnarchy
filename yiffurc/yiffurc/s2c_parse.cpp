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
#include <list>
#include <string>
#ifdef _MSC_VER
#include <xutility>
#endif
#include <cassert>
#include "windows.h"

namespace yiffurc {

using std::min;

// Instantiate regexes on load.
static S2C_RE g_re;

const S2C* YIFFURC_CALL s2c_parse_peek( const char* line, int len,
                                     const NET_SESSION* session )
{
   NET_SESSION tmp = *session;
   return s2c_parse( line, len, &tmp );
}

const S2C* YIFFURC_CALL s2c_parse( const char* line, int len,
                                   NET_SESSION* session )
{
   ++session->s2c_state.line_count;

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
      /* Past the MOTD. Confused? Dragonroar may not always be sent to terminate the motd. :( */
   case NET_LEVEL_MOTDEND:

      /* Sort by first letter. */
      switch (line[ 0 ])
      {
      case 'D':
         if (REGXEC( g_re.DRAGONROAR ))
         {
            session->level = NET_LEVEL_MOTDEND;

            S2C_DRAGONROAR* proto = pfx_new_obj<S2C_DRAGONROAR>( );
            return (S2C*) proto;
         }
         break;
      case '&':
         if (REGXEC( g_re.PASS ))
         {
            if (session->create_count) // Create response.
               --session->create_count;
            else // Login response.
               session->level = NET_LEVEL_LOGGEDIN;

            S2C_PASS* proto = pfx_new_obj<S2C_PASS>( );
            return (S2C*) proto;
         }
         break;

      case 'N':
         if (session->create_count && REGXEC( g_re.CREATEFAIL ))
         {
            --session->create_count;

            S2C_CREATEFAIL* proto = pfx_new_obj<S2C_CREATEFAIL>( );
            proto->category = SUBNEW( 1 );
            proto->reason = SUBNEW( 2 );
            return (S2C*) proto;
         }
         break;

      case '\\':
         if (!session->create_count && REGXEC( g_re.MACID ))
         {
            S2C_MACID* proto = pfx_new_obj<S2C_MACID>( );
            proto->mac_id = SUBNEW( 1 );
            return (S2C*) proto;
         }
         break;
      }

      if (REGXEC( g_re.MOTD ))
      {
         S2C_MOTD* proto = pfx_new_obj<S2C_MOTD>( );
         proto->line = SUBNEW( 1 );
         return (S2C*) proto;
      }

      break;

      /* Logged in. */
   case NET_LEVEL_LOGGEDIN:

      /* Sort by first letter. */
      switch (line[ 0 ])
      {
      case ';':
         if (REGXEC( g_re.USEMAPFILE ))
         {
            S2C_USEMAPFILE* proto = pfx_new_obj<S2C_USEMAPFILE>( );
            proto->file    = SUBNEW( 1 );
            return (S2C*) proto;
         }
         break;

      case '@':
         if (REGXEC( g_re.CAMTWEEN ))
         {
            S2C_CAMTWEEN* proto = pfx_new_obj<S2C_CAMTWEEN>( );
            proto->x       = ftoi2( SUBSTR( 1 ) );
            proto->y       = ftoi2( SUBSTR( 2 ) );
            proto->x_old   = ftoi2( SUBSTR( 3 ) );
            proto->y_old   = ftoi2( SUBSTR( 4 ) );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.CAMSET ))
         {
            S2C_CAMSET* proto = pfx_new_obj<S2C_CAMSET>( );
            proto->x       = ftoi2( SUBSTR( 1 ) );
            proto->y       = ftoi2( SUBSTR( 2 ) );
            return (S2C*) proto;
         }
         break;

      case '>':
         if (REGXEC( g_re.OBJSET ))
         {
			 try{
            // Still gotta parse this one by hand. :(
            S2C_OBJSET* proto = pfx_new_obj<S2C_OBJSET>( );
            proto->count   = SUBLEN( 0 );
            proto->count   -= 1;
            proto->count   /= 6;

            IDXY* objects = new IDXY[ proto->count ];

            for (unsigned int i = 0; i < proto->count; i++)
            {
			//	if (
               objects[ i ].x  = ftoi2_220( line + (1 + 6*i + 0) );
               objects[ i ].y  = ftoi2_220( line + (1 + 6*i + 2) );
               objects[ i ].id = ftoi2_220( line + (1 + 6*i + 4) );
            }

            proto->objects = objects;
            return (S2C*) proto;
			 } catch (char *str ) {
			 OutputDebugString("Explosion in OBJSET\n");
			 OutputDebugString(str); 
			 }
         }
         break;

      case '<':
         if (REGXEC( g_re.AVCREATE ))
         {
            S2C_AVCREATE* proto = pfx_new_obj<S2C_AVCREATE>( );
            proto->uid     = ftoi4_220( SUBSTR( 1 ) );
            proto->x       = ftoi2_220( SUBSTR( 2 ) );
            proto->y       = ftoi2_220( SUBSTR( 3 ) );
            proto->frame   = ftoi2_220( SUBSTR( 4 ) );
            
			//unsigned name_len = min( ftoi1_220( *SUBSTR( 5 ) ), (unsigned) (SUBLEN( 5 ) - 1) );
            //proto->name    = strnnew( SUBSTR( 5 ) + 1, name_len );
			proto->name = strnnew( SUBSTR ( 6 ), SUBLEN( 6 ) );
			// subtract 1 from the colors length because of the trailing flags.
            //proto->colors  = strnnew( SUBSTR( 5 ) + 1 + name_len, len - (SUBOFF( 5 ) + 1 + name_len) - 1 );
            proto->colors = strnnew( SUBSTR ( 7 ), SUBLEN( 7 ) );
			
			proto->flags   =  ftoi1_220( SUBSTR(8)[0] );// 0;//ftoi1_220( line[ len - 1 ] );
            
            return (S2C*) proto;
         }
         break;

      case '/':
         if (REGXEC( g_re.AVTWEEN ))
         {
            S2C_AVTWEEN* proto = pfx_new_obj<S2C_AVTWEEN>( );
            proto->uid     = ftoi4_220( SUBSTR( 1 ) );
            proto->x       = ftoi2_220( SUBSTR( 2 ) );
            proto->y       = ftoi2_220( SUBSTR( 3 ) );
            proto->frame   = ftoi2_220( SUBSTR( 4 ) );
            return (S2C*) proto;
         }
         break;

      case ')':
         if (REGXEC( g_re.AVDESTROY ))
         {
            S2C_AVDESTROY* proto = pfx_new_obj<S2C_AVDESTROY>( );
            proto->uid     = ftoi4_220( SUBSTR( 1 ) );
            return (S2C*) proto;
         }
         break;

      case 'A':
         if (REGXEC( g_re.AVSHOW ))
         {
            S2C_AVSHOW* proto = pfx_new_obj<S2C_AVSHOW>( );
            proto->uid     = ftoi4_220( SUBSTR( 1 ) );
            proto->x       = ftoi2_220( SUBSTR( 2 ) );
            proto->y       = ftoi2_220( SUBSTR( 3 ) );
            proto->frame   = ftoi2_220( SUBSTR( 4 ) );
            return (S2C*) proto;
         }
         break;

      case 'B':
         if (REGXEC( g_re.AVUPDATECOLORS ))
         {
            S2C_AVUPDATECOLORS* proto = pfx_new_obj<S2C_AVUPDATECOLORS>( );
            proto->uid     = ftoi4_220( SUBSTR( 1 ) );
            proto->frame   = ftoi2_220( SUBSTR( 2 ) );
            proto->colors  = SUBNEW( 3 );
            return (S2C*) proto;
         }
         break;

      case 'C':
         if (REGXEC( g_re.AVHIDE ))
         {
            S2C_AVHIDE* proto = pfx_new_obj<S2C_AVHIDE>( );
            proto->uid     = ftoi4_220( SUBSTR( 1 ) );
            proto->x       = ftoi2_220( SUBSTR( 2 ) );
            proto->y       = ftoi2_220( SUBSTR( 3 ) );
            return (S2C*) proto;
         }
         break;

      case 'D':
         if (REGXEC( g_re.AVSHOW2 ))
         {
            S2C_AVSHOW2* proto = pfx_new_obj<S2C_AVSHOW2>( );
            proto->uid     = ftoi4_220( SUBSTR( 1 ) );
            proto->x       = ftoi2_220( SUBSTR( 2 ) );
            proto->y       = ftoi2_220( SUBSTR( 3 ) );
            proto->frame   = ftoi2_220( SUBSTR( 4 ) );
            return (S2C*) proto;
         }
         break;

      case '0':
         if (REGXEC( g_re.VARSET ))
         {
            S2C_VARSET* proto = pfx_new_obj<S2C_VARSET>( );

            size_t match_len  = SUBLEN( 0 );
            const char* c     = SUBSTR( 0 ) + 3;
            proto->count      = 0;

            for (size_t i = 3; i < match_len; proto->count++)
            {
               int v = ftoi3( c );

               if (v == 0x4000)
                  i += 9, c+= 9;
               else
                  i += 3, c+= 3;
            }

            S2C_VARSET::VARTRIP* trips = new S2C_VARSET::VARTRIP[ proto->count ];

            c = SUBSTR( 0 ) + 1;
            int curr_v  = ftoi2( c );
            c += 2;

            for (unsigned int i = 0; i < proto->count; i++)
            {
               trips[ i ].first = curr_v;

               int v = ftoi3( c );
               c += 3;

               if (v == 0x4000)
               {
                  trips[ i ].count = ftoi3( c ) + 1;
                  c += 3;
                  trips[ i ].value = ftoi3( c );
                  c += 3;
               }
               else
               {
                  trips[ i ].count = 1;
                  trips[ i ].value = v;
               }
            }

            proto->set = trips;
            return (S2C*) proto;
         }
         break;

      case '1':
         if (REGXEC( g_re.FLOORSET ))
         {
            // Still gotta parse this one by hand. :(
            S2C_FLOORSET* proto = pfx_new_obj<S2C_FLOORSET>( );
            proto->count   = SUBLEN( 0 );
            proto->count   -= 1;
            proto->count   /= 6;

            IDXY* floors = new IDXY[ proto->count ];

            for (unsigned int i = 0; i < proto->count; i++)
            {
               floors[ i ].x  = ftoi2( line + (1 + 6*i + 0) );
               floors[ i ].y  = ftoi2( line + (1 + 6*i + 2) );
               floors[ i ].id = ftoi2( line + (1 + 6*i + 4) );
            }

            proto->floors = floors;
            return (S2C*) proto;
         }
         break;

      case '%':
         if (REGXEC( g_re.FOOTOBJ ))
         {
            S2C_FOOTOBJ* proto = pfx_new_obj<S2C_FOOTOBJ>( );
            proto->obj     = ftoi2( SUBSTR( 1 ) );
            return (S2C*) proto;
         }
         break;

      case '^':
         if (REGXEC( g_re.INVOBJ ))
         {
            S2C_INVOBJ* proto = pfx_new_obj<S2C_INVOBJ>( );
            proto->obj     = ftoi2( SUBSTR( 1 ) );
            return (S2C*) proto;
         }
         break;

      case '!':
         if (REGXEC( g_re.PLAYSOUND ))
         {
            S2C_PLAYSOUND* proto = pfx_new_obj<S2C_PLAYSOUND>( );
            proto->sound   = ftoi2( SUBSTR( 1 ) );
            return (S2C*) proto;
         }
         break;

      case '~':
         if (REGXEC( g_re.DISPLISTOPEN ))
         {
            S2C_DISPLISTOPEN* proto =
               pfx_new_obj<S2C_DISPLISTOPEN>( );
            return (S2C*) proto;
         }
         break;

      case '=':
         if (REGXEC( g_re.DISPLISTCLOSE ))
         {
            S2C_DISPLISTCLOSE* proto =
               pfx_new_obj<S2C_DISPLISTCLOSE>( );
            return (S2C*) proto;
         }
         break;

      case '6':
         if (REGXEC( g_re.TRIGGERSELF ))
         {
            // Still gotta parse this one by hand. :(
            S2C_TRIGGERSELF* proto = pfx_new_obj<S2C_TRIGGERSELF>( );
            proto->x1      = ftoi2( SUBSTR( 1 ) );
            proto->y1      = ftoi2( SUBSTR( 2 ) );
            proto->x2      = ftoi2( SUBSTR( 3 ) );
            proto->y2      = ftoi2( SUBSTR( 4 ) );

            proto->count   = SUBLEN( 0 );
            proto->count   -= 9;
            proto->count   /= 6;

            DSXY* trgtrips = new DSXY[ proto->count ];

            for (unsigned int i = 0; i < proto->count; i++)
            {
               trgtrips[ i ].line = ftoi2( line + (1 + 6*i + 0) );
               trgtrips[ i ].x    = ftoi2( line + (1 + 6*i + 2) );
               trgtrips[ i ].y    = ftoi2( line + (1 + 6*i + 4) );
            }

            proto->triggers = trgtrips;
            return (S2C*) proto;
         }
         break;

      case '7':
         if (REGXEC( g_re.TRIGGER ))
         {
            // Still gotta parse this one by hand. :(
            S2C_TRIGGER* proto = pfx_new_obj<S2C_TRIGGER>( );
            proto->x1      = ftoi2( SUBSTR( 1 ) );
            proto->y1      = ftoi2( SUBSTR( 2 ) );
            proto->x2      = ftoi2( SUBSTR( 3 ) );
            proto->y2      = ftoi2( SUBSTR( 4 ) );

            proto->count   = SUBLEN( 0 );
            proto->count   -= 9;
            proto->count   /= 6;

            DSXY* trgtrips = new DSXY[ proto->count ];

            for (unsigned int i = 0; i < proto->count; i++)
            {
               trgtrips[ i ].line = ftoi2( line + (1 + 6*i + 0) );
               trgtrips[ i ].x    = ftoi2( line + (1 + 6*i + 2) );
               trgtrips[ i ].y    = ftoi2( line + (1 + 6*i + 4) );
            }

            proto->triggers = trgtrips;
            return (S2C*) proto;
         }
         break;

      case ']':

         if (len < 2)
            break;

         switch (line[ 1 ])
         {
         case 'w':
            if (REGXEC( g_re.VERREQ ))
            {
               S2C_VERREQ* proto = pfx_new_obj<S2C_VERREQ>( );
               return (S2C*) proto;
            }
            break;

         case 's':
            if (REGXEC( g_re.DREAMPOS2 ))
            {
               S2C_DREAMPOS2* proto = pfx_new_obj<S2C_DREAMPOS2>( );
               proto->x       = ftoi2( SUBSTR( 1 ) );
               proto->y       = ftoi2( SUBSTR( 2 ) );
               proto->name    = SUBNEW( 3 );
               proto->caption = SUBNEW( 4 );
               return (S2C*) proto;
            }
            if (REGXEC( g_re.DREAMPOS ))
            {
               S2C_DREAMPOS* proto = pfx_new_obj<S2C_DREAMPOS>( );
               proto->x       = ftoi2( SUBSTR( 1 ) );
               proto->y       = ftoi2( SUBSTR( 2 ) );
               proto->name    = SUBNEW( 3 );
               return (S2C*) proto;
            }
            break;

         case 't':
            if (REGXEC( g_re.DREAMCLEAR ))
            {
               S2C_DREAMCLEAR* proto = pfx_new_obj<S2C_DREAMCLEAR>( );
               proto->x       = ftoi2( SUBSTR( 1 ) );
               proto->y       = ftoi2( SUBSTR( 2 ) );
               return (S2C*) proto;
            }
            break;

		//Custom specitag
		 case 'P':
			 if (REGXEC(g_re.CUSTSPECITAG)) 
			 {
			   S2C_SPECITAG* proto = pfx_new_obj<S2C_SPECITAG>( );

               const int color_len = get_color_len( SUBSTR( 2 ), SUBLEN( 2 ) );
               proto->colors = strnnew( SUBSTR( 1 ), color_len );
			   proto->url = strnnew( SUBSTR(1), SUBLEN(1) );
               if (SUBOFF( 2 ) + color_len < len)
               {
                  memcpy( proto->gsd, SUBSTR( 2 ) + color_len, 3 );
                  return (S2C*) proto;
               }
               delete proto;
               break;
			 }
			 break;
		//Specitag/badges
         case '-':
            if (REGXEC( g_re.SPECITAG ))
            {
               S2C_SPECITAG* proto = pfx_new_obj<S2C_SPECITAG>( );

               const int color_len = get_color_len( SUBSTR( 1 ), SUBLEN( 1 ) );
               proto->colors = strnnew( SUBSTR( 1 ), color_len );
			   proto->url = NULL;
               if (SUBOFF( 1 ) + color_len < len)
               {
                  memcpy( proto->gsd, SUBSTR( 1 ) + color_len, 3 );
                  return (S2C*) proto;
               }
               delete proto;
               break;
            }
            if (REGXEC( g_re.BADGE ))
            {
               S2C_BADGE* proto = pfx_new_obj<S2C_BADGE>( );
               proto->badge = SUBSTR( 1 )[ 0 ] - ' ';
               return (S2C*) proto;
            }
            break;

         case '%':
            if (REGXEC( g_re.ONLINERESULT ))
            {
               S2C_ONLINERESULT* proto = pfx_new_obj<S2C_ONLINERESULT>( );
               proto->online = (SUBSTR( 1 )[ 0 ] - '0') ? true : false;
               proto->player = SUBNEW( 2 );
               return (S2C*) proto;
            }
            break;

         case 'f':
            if (REGXEC( g_re.PORTRAIT ))
            {
               S2C_PORTRAIT* proto = pfx_new_obj<S2C_PORTRAIT>( );
               int pos = 2;
               const int color_len = get_color_len( line + pos, SUBLEN( 1 ) );
               proto->colors = strnnew( line + pos, color_len );
               pos += color_len;
               if (pos + 3 < len)
               {
                  memcpy( proto->gsd, line + pos, 3 );
                  pos += 3;
                  if (pos < len)
                  {
                     proto->player = strnnew( line + pos, min( (unsigned) (len - pos), ftoi1_220( line[ pos ] ) ) );
                     return (S2C*) proto;
                  }
               }
               delete proto;               
            }
            break;

         case '&':
            if (REGXEC( g_re.FETCHPID ))
            {
               S2C_FETCHPID* proto = pfx_new_obj<S2C_FETCHPID>( );
               proto->pid     = antoi( SUBSTR( 1 ), SUBLEN( 1 ) );
               proto->name    = SUBNEW( 2 );
               return (S2C*) proto;
            }
            break;

         case 'q':
            if (REGXEC( g_re.FETCHDREAM ))
            {
               S2C_FETCHDREAM* proto = pfx_new_obj<S2C_FETCHDREAM>( );
               proto->name    = SUBNEW( 1 );
               proto->did     = antoi( SUBSTR( 2 ), SUBLEN( 2 ) );
               return (S2C*) proto;
            }
            break;

         case 'r':
            if (REGXEC( g_re.USEDREAM ))
            {
               S2C_USEDREAM* proto = pfx_new_obj<S2C_USEDREAM>( );
               proto->name    = SUBNEW( 1 );
               proto->did     = antoi( SUBSTR( 2 ), SUBLEN( 2 ) );
               return (S2C*) proto;
            }
            break;

         case 'c':
            if (REGXEC( g_re.USEBGFILE ))
            {
               S2C_USEBGFILE* proto = pfx_new_obj<S2C_USEBGFILE>( );
               proto->file    = SUBNEW( 1 );
               return (S2C*) proto;
            }
            break;

         case 'j':
            if (REGXEC( g_re.PLAYSONG ))
            {
               S2C_PLAYSONG* proto = pfx_new_obj<S2C_PLAYSONG>( );
               proto->song    = ftoi2( SUBSTR( 1 ) );
               return (S2C*) proto;
            }
            break;

         case '}':
            if (REGXEC( g_re.SYNCCOLORCODE ))
            {
               S2C_SYNCCOLORCODE* proto = pfx_new_obj<S2C_SYNCCOLORCODE>( );
               
               const int color_len = get_color_len( SUBSTR( 1 ), SUBLEN( 1 ) );
               proto->colors = strnnew( SUBSTR( 1 ), color_len );
               if (SUBOFF( 1 ) + color_len < len)
               {
                  memcpy( proto->gsd, SUBSTR( 1 ) + color_len, 3 );
                  return (S2C*) proto;
               }
               delete proto;
            }
            break;

         case 'm':
            if (REGXEC( g_re.PING ))
            {
               S2C_PING* proto = pfx_new_obj<S2C_PING>( );
               proto->value = antoi( SUBSTR( 1 ), SUBLEN( 1 ) );
               return (S2C*) proto;
            }
            break;

         case '#':
            if (REGXEC( g_re.MSGBOX ))
            {
               S2C_MSGBOX* proto = pfx_new_obj<S2C_MSGBOX>( );
               proto->unk   = SUBNEW( 1 );
               proto->flags = antoi( SUBSTR( 2 ), SUBLEN( 2 ) );
               proto->msg   = SUBNEW( 3 );
               return (S2C*) proto;
            }
            break;

         case '3':
            if (REGXEC( g_re.PRINTPLAYERS ))
            {
               S2C_PRINTPLAYERS* proto = pfx_new_obj<S2C_PRINTPLAYERS>( );
               proto->listable = SUBSTR( 1 )[ 0 ] == '1' ? false : true;
               proto->name     = SUBNEW( 2 );
               proto->caption  = SUBNEW( 3 );
               return (S2C*) proto;
            }

         default:
            break;
         }
         break;

   case '(':
      
         if (REGXEC( g_re.WHISPER ))
         {
            S2C_WHISPER* proto = pfx_new_obj<S2C_WHISPER>( );
            proto->from_short = SUBNEW( 1 );
            proto->from       = SUBNEW( 2 );
            proto->message    = SUBNEW( 3 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.SHOUT ))
         {
            S2C_SHOUT* proto = pfx_new_obj<S2C_SHOUT>( );
            proto->from_short = SUBNEW( 1 );
            proto->from       = SUBNEW( 2 );
            proto->message    = SUBNEW( 3 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.SPEECH ))
         {
            S2C_SPEECH* proto = pfx_new_obj<S2C_SPEECH>( );
            proto->from_short = SUBNEW( 1 );
            proto->from       = SUBNEW( 2 );
            proto->message    = SUBNEW( 3 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.EMOTE ))
         {
            S2C_EMOTE* proto = pfx_new_obj<S2C_EMOTE>( );
            proto->from_short = SUBNEW( 1 );
            proto->from       = SUBNEW( 2 );
            proto->message    = SUBNEW( 3 );
            return (S2C*) proto;
         }

         if (REGXEC( g_re.DESCBODY ))
         {
            S2C_DESCBODY* proto = pfx_new_obj<S2C_DESCBODY>( );
            proto->player_short = SUBNEW( 1 );
            proto->body         = SUBNEW( 2 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.DESCBODY2 ))
         {
            S2C_DESCBODY2* proto = pfx_new_obj<S2C_DESCBODY2>( );
            proto->badge = *SUBSTR( 1 ) - 32;
            proto->player_short = SUBNEW( 2 );
            proto->body         = SUBNEW( 3 );
            return (S2C*) proto;
         }

         if (REGXEC( g_re.YOUSAY ))
         {
            S2C_YOUSAY* proto = pfx_new_obj<S2C_YOUSAY>( );
            proto->message = SUBNEW( 1 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.YOUWHISPER ))
         {
            S2C_YOUWHISPER* proto = pfx_new_obj<S2C_YOUWHISPER>( );
            proto->message = SUBNEW( 1 );
            proto->to_short = SUBNEW( 2 );
            proto->to       = SUBNEW( 3 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.YOUSHOUT ))
         {
            S2C_YOUSHOUT* proto = pfx_new_obj<S2C_YOUSHOUT>( );
            proto->message = SUBNEW( 1 );
            return (S2C*) proto;
         }

         if (REGXEC( g_re.EMIT ))
         {
            S2C_EMIT* proto = pfx_new_obj<S2C_EMIT>( );
            proto->message = SUBNEW( 1 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.DSEMIT ))
         {
            S2C_DSEMIT* proto = pfx_new_obj<S2C_DSEMIT>( );
            proto->message = SUBNEW( 1 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.ROLL ))
         {
            S2C_ROLL* proto = pfx_new_obj<S2C_ROLL>( );
            proto->player_short = SUBNEW( 1 );
            proto->player       = SUBNEW( 2 );
            if (SUBLEN( 3 ))
               proto->dice    = antoi( SUBSTR( 3 ), SUBLEN( 3 ) );
            else
               proto->dice    = 1;
            proto->sides   = antoi( SUBSTR( 4 ), SUBLEN( 4 ) );
            proto->message = SUBNEW( 5 );
            proto->sum     = antoi( SUBSTR( 6 ), SUBLEN( 6 ) );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.VERBOSEROLL ))
         {
            S2C_VERBOSEROLL* proto = pfx_new_obj<S2C_VERBOSEROLL>( );
            proto->player_short = SUBNEW( 1 );
            proto->player       = SUBNEW( 2 );
            if (SUBLEN( 3 ))
               proto->dice    = antoi( SUBSTR( 3 ), SUBLEN( 3 ) );
            else
               proto->dice    = 1;
            proto->sides   = antoi( SUBSTR( 4 ), SUBLEN( 4 ) );
            proto->message = SUBNEW( 5 );
            proto->sum     = antoi( SUBSTR( 7 ), SUBLEN( 7 ) );
            proto->values  = new unsigned int[ proto->dice ];

            size_t off = SUBOFF( 4 ) + SUBLEN( 4 )+ 3;
            if (SUBLEN( 5 ))
               off += SUBLEN( 5 ) + 1;

            for (unsigned int i = 0; i < proto->dice; i++)
            {
               proto->values[ i ] = antoi( line + off, len - off );
               while (line[ off ] != ')')
                  off++;
               off += 3;
            }
            return (S2C*) proto;
         }
         if (REGXEC( g_re.WHOUPTIME ))
         {
            S2C_WHOUPTIME* proto = pfx_new_obj<S2C_WHOUPTIME>( );
            proto->players_online = antoi( SUBSTR( 1 ),
                                           SUBLEN( 1 ) );
            proto->days_up        = antoi( SUBSTR( 2 ),
                                           SUBLEN( 2 ) );
            proto->hours_up       = antoi( SUBSTR( 3 ),
                                           SUBLEN( 3 ) );
            proto->minutes_up     = antoi( SUBSTR( 4 ),
                                           SUBLEN( 4 ) );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.WHOMAX ))
         {
            S2C_WHOMAX* proto = pfx_new_obj<S2C_WHOMAX>( );
            proto->session_max = antoi( SUBSTR( 1 ),
                                        SUBLEN( 1 ) );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.WHODAILYMAX ))
         {
            S2C_WHODAILYMAX* proto = pfx_new_obj<S2C_WHODAILYMAX>( );
            proto->daily_max = antoi( SUBSTR( 1 ),
                                      SUBLEN( 1 ) );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.COOKIESREADY ))
         {
            S2C_COOKIESREADY* proto = pfx_new_obj<S2C_COOKIESREADY>( );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.COOKIE ))
         {
            S2C_COOKIE* proto = pfx_new_obj<S2C_COOKIE>( );
            proto->from = SUBNEW( 1 );
            proto->note = SUBNEW( 2 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.SUMMON ))
         {
            S2C_SUMMON* proto = pfx_new_obj<S2C_SUMMON>( );
            proto->from_short = SUBNEW( 1 );
            proto->from       = SUBNEW( 2 );
            proto->dest       = SUBNEW( 3 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.JOIN ))
         {
            S2C_JOIN* proto = pfx_new_obj<S2C_JOIN>( );
            proto->from_short = SUBNEW( 1 );
            proto->from       = SUBNEW( 2 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.SUMMONJOINECHO ))
         {
            S2C_SUMMONJOINECHO* proto = pfx_new_obj<S2C_SUMMONJOINECHO>( );
            proto->player_short = SUBNEW( 1 );
            proto->player = SUBNEW( 2 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.SUMMONED ))
         {
            S2C_SUMMONED* proto = pfx_new_obj<S2C_SUMMONED>( );
            proto->summoner_short = SUBNEW( 1 );
            proto->summoner       = SUBNEW( 2 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.JOINED ))
         {
            S2C_JOINED* proto = pfx_new_obj<S2C_JOINED>( );
            proto->joiner_short = SUBNEW( 1 );
            proto->joiner       = SUBNEW( 2 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.DECLINED ))
         {
            S2C_DECLINED* proto = pfx_new_obj<S2C_DECLINED>( );
            proto->player_short = SUBNEW( 1 );
            proto->player       = SUBNEW( 2 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.DECLINEECHO ))
         {
            S2C_DECLINEECHO* proto = pfx_new_obj<S2C_DECLINEECHO>( );
            proto->player_short = SUBNEW( 1 );
            proto->player       = SUBNEW( 2 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.DECLINEFAIL ))
         {
            S2C_DECLINEFAIL* proto = pfx_new_obj<S2C_DECLINEFAIL>( );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.WHISPERFAIL ))
         {
            S2C_WHISPERFAIL* proto = pfx_new_obj<S2C_WHISPERFAIL>( );
            proto->player = SUBNEW( 1 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.WHISPERFAILEXACT ))
         {
            S2C_WHISPERFAILEXACT* proto = pfx_new_obj<S2C_WHISPERFAILEXACT>( );
            proto->player = SUBNEW( 1 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.WHISPERFAILMORE ))
         {
            S2C_WHISPERFAILMORE* proto = pfx_new_obj<S2C_WHISPERFAILMORE>( );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.WHISPERDENIED ))
         {
            S2C_WHISPERDENIED* proto = pfx_new_obj<S2C_WHISPERDENIED>( );
            proto->player_short = SUBNEW( 1 );
            proto->player       = SUBNEW( 2 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.INFOITEM ))
         {
            S2C_INFOITEM* proto = pfx_new_obj<S2C_INFOITEM>( );
            proto->item_name    = SUBNEW( 1 );
            proto->num_active   = antoi( SUBSTR( 2 ), SUBLEN( 2 ) );
            proto->num_inactive = antoi( SUBSTR( 3 ), SUBLEN( 3 ) );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.INFOPLAYER ))
         {
            S2C_INFOPLAYER* proto = pfx_new_obj<S2C_INFOPLAYER>( );
            proto->player       = SUBNEW( 1 );
            proto->email        = SUBNEW( 2 );
            return (S2C*) proto;
         }
         if (REGXEC( g_re.DESCHEAD ))
         {
            S2C_DESCHEAD* proto = pfx_new_obj<S2C_DESCHEAD>( );
            proto->player_short = SUBNEW( 1 );
            proto->player       = SUBNEW( 2 );
            return (S2C*) proto;
         }

         /* Chat catch-all */
         if (REGXEC( g_re.CHAT ))
         {
            S2C_CHAT* proto = pfx_new_obj<S2C_CHAT>( );
            proto->message = SUBNEW( 1 );
            return (S2C*) proto;
         }
         break;
      }
      break;

   default:
      break;
   }

   return 0;
}

} // namespace yiffurc