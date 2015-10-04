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
static XFER_S2C_RE g_re;

const XFER_S2C* YIFFURC_CALL xfer_s2c_parse_peek( const char* line, int len,
                                               const XFER_NET_SESSION* session )
{
   XFER_NET_SESSION tmp = *session;
   return xfer_s2c_parse( line, len, &tmp );
}

const XFER_S2C* YIFFURC_CALL xfer_s2c_parse( const char* line, int len,
                                          XFER_NET_SESSION* session )
{
   ++session->s2c_state.line_count;

   if (!len)
      return 0;

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
      /* Connected but not logged in. */
   case XFER_NET_LEVEL_CONNECTED:

      switch (*line)
      {
      case '1':
         if (REGXEC( g_re.GREETING ))
         {
            XFER_S2C_GREETING* proto = pfx_new_obj<XFER_S2C_GREETING>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         break;

      case '2':
         if (REGXEC( g_re.USEROK ))
         {
            XFER_S2C_USEROK* proto = pfx_new_obj<XFER_S2C_USEROK>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         break;

      case '3':
         if (REGXEC( g_re.LOGGEDIN ))
         {
            session->level = XFER_NET_LEVEL_LOGGEDIN;

            XFER_S2C_LOGGEDIN* proto = pfx_new_obj<XFER_S2C_LOGGEDIN>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         break;

      case '4':
         if (REGXEC( g_re.DOWNLOADINFO ))
         {
            session->level = XFER_NET_LEVEL_SSEND;

            XFER_S2C_DOWNLOADINFO* proto = pfx_new_obj<XFER_S2C_DOWNLOADINFO>( );
            proto->file_size  = antoi( SUBSTR( 1 ), SUBLEN( 1 ) );
            proto->unk1       = antoi( SUBSTR( 2 ), SUBLEN( 2 ) );
            proto->chunk_size = antoi( SUBSTR( 3 ), SUBLEN( 3 ) );
            proto->unk2       = antoi( SUBSTR( 4 ), SUBLEN( 4 ) );

            session->s2c_state.chunks_left = (int) (proto->file_size /
                                                     proto->chunk_size);
            if (proto->file_size % proto->chunk_size)
               session->s2c_state.chunks_left++;

            return (XFER_S2C*) proto;
         }
         if (REGXEC( g_re.FILENOTFOUND ))
         {
            XFER_S2C_FILENOTFOUND* proto = pfx_new_obj<XFER_S2C_FILENOTFOUND>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         break;

      case '9':
         if (REGXEC( g_re.LOGGEDOUT ))
         {
            // session->level = XFER_NET_LEVEL_CONNECTED;

            XFER_S2C_LOGGEDOUT* proto = pfx_new_obj<XFER_S2C_LOGGEDOUT>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         if (REGXEC( g_re.INACTIVITY ))
         {
            // session->level = XFER_NET_LEVEL_CONNECTED;

            XFER_S2C_INACTIVITY* proto = pfx_new_obj<XFER_S2C_INACTIVITY>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         break;

      default:
         break;
      }
      break;

   case XFER_NET_LEVEL_LOGGEDIN:

      switch (*line)
      {
      case '9':
         if (REGXEC( g_re.UFPSET ))
         {
            XFER_S2C_UFPSET* proto = pfx_new_obj<XFER_S2C_UFPSET>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         if (REGXEC( g_re.LOGGEDOUT ))
         {
            session->level = XFER_NET_LEVEL_CONNECTED;

            XFER_S2C_LOGGEDOUT* proto = pfx_new_obj<XFER_S2C_LOGGEDOUT>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         if (REGXEC( g_re.INACTIVITY ))
         {
            session->level = XFER_NET_LEVEL_CONNECTED;

            XFER_S2C_INACTIVITY* proto = pfx_new_obj<XFER_S2C_INACTIVITY>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         break;

      case '5':
         if (REGXEC( g_re.UPLOADOK ))
         {
            session->level = XFER_NET_LEVEL_CSEND;

            XFER_S2C_UPLOADOK* proto = pfx_new_obj<XFER_S2C_UPLOADOK>( );
            proto->unk1    = antoi( SUBSTR( 1 ), SUBLEN( 1 ) );
            proto->message = SUBNEW( 2 );
            return (XFER_S2C*) proto;
         }
         break;

      case '4':
         if (REGXEC( g_re.DOWNLOADINFO ))
         {
            session->level = XFER_NET_LEVEL_SSEND;

            XFER_S2C_DOWNLOADINFO* proto = pfx_new_obj<XFER_S2C_DOWNLOADINFO>( );
            proto->file_size  = antoi( SUBSTR( 1 ), SUBLEN( 1 ) );
            proto->unk1       = antoi( SUBSTR( 2 ), SUBLEN( 2 ) );
            proto->chunk_size = antoi( SUBSTR( 3 ), SUBLEN( 3 ) );
            proto->unk2       = antoi( SUBSTR( 4 ), SUBLEN( 4 ) );

            session->s2c_state.chunks_left = (int) (proto->file_size /
                                          proto->chunk_size);
            if (proto->file_size % proto->chunk_size)
               session->s2c_state.chunks_left++;

            return (XFER_S2C*) proto;
         }
         break;

      default:
         break;
      }
      break;

   /* Uploading a file. */
   case XFER_NET_LEVEL_CSEND:

      switch (*line)
      {
      case '9':
         if (REGXEC( g_re.LOGGEDOUT ))
         {
            session->level = XFER_NET_LEVEL_CONNECTED;

            XFER_S2C_LOGGEDOUT* proto = pfx_new_obj<XFER_S2C_LOGGEDOUT>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         if (REGXEC( g_re.INACTIVITY ))
         {
            session->level = XFER_NET_LEVEL_CONNECTED;

            XFER_S2C_INACTIVITY* proto = pfx_new_obj<XFER_S2C_INACTIVITY>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         break;

      case '5':
         if (REGXEC( g_re.CHUNKOK ))
         {
            XFER_S2C_CHUNKOK* proto = pfx_new_obj<XFER_S2C_CHUNKOK>( );
            proto->chunk_id = antoi( SUBSTR( 1 ), SUBLEN( 1 ) );
            return (XFER_S2C*) proto;
         }
         if (REGXEC( g_re.UPLOADDONE ))
         {
            XFER_S2C_UPLOADDONE* proto = pfx_new_obj<XFER_S2C_UPLOADDONE>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         break;

      default:
         break;
      }
      break;

      /* Downloading a file. */
   case XFER_NET_LEVEL_SSEND:

      switch (*line)
      {
      case '9':
         if (REGXEC( g_re.LOGGEDOUT ))
         {
            session->level = XFER_NET_LEVEL_CONNECTED;

            XFER_S2C_LOGGEDOUT* proto = pfx_new_obj<XFER_S2C_LOGGEDOUT>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         if (REGXEC( g_re.INACTIVITY ))
         {
            session->level = XFER_NET_LEVEL_CONNECTED;

            XFER_S2C_INACTIVITY* proto = pfx_new_obj<XFER_S2C_INACTIVITY>( );
            proto->message = SUBNEW( 1 );
            return (XFER_S2C*) proto;
         }
         break;

      case 'S':
         if (len >= 10 && line[ 1 ] == 'C')
         {
            session->s2c_state.chunks_left--;

            if (session->s2c_state.chunks_left == 0)
            {
               session->level = XFER_NET_LEVEL_LOGGEDIN;
            }

            XFER_S2C_CHUNKHEAD* proto = pfx_new_obj<XFER_S2C_CHUNKHEAD>( );
            proto->chunk_id    = lsb_to_machine32( *(int*)(line + 2) );
            proto->chunk_crc32 = lsb_to_machine32( *(int*)(line + 2) );
            return (XFER_S2C*) proto;
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