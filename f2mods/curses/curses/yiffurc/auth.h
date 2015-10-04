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

#ifndef _YIFFURC_AUTH_H__
#define _YIFFURC_AUTH_H__

#include "common.h"

/** \addtogroup auth Client/Server Authentication
*  An API for validating the authenticity of the client and server during login.
* \{ */

#if defined( __cplusplus )
namespace yiffurc {
#endif

/** Scrambles a sequence of bytes.
*  \param[out]    dst   Buffer to hold scrambled unsigned chars.
*  \param[in]     src   Source buffer to scramble.
*  \param[in]     len   Size of buffers.
*  \returns dst pointer
*/
YIFFURC_LINK char* YIFFURC_CALL   auth_scramble( char* dst, const char* src, int len );
/** Unscrambles a sequence of bytes.
*  \param[out]    dst   Buffer to hold unscrambled unsigned chars.
*  \param[in]     src   Source buffer to unscramble.
*  \param[in]     len   Size of buffers.
*  \returns dst pointer
*/
YIFFURC_LINK char* YIFFURC_CALL   auth_descramble( char* dst, const char* src, int len );

/** A 128-bit digest. */
union AUTH_DIGEST128
{
   int           i[ 4 ];
   unsigned char b[ 16 ];
};

/** Generates a 128-bit digest of a sequence of unsigned chars.
*  \param[in]     data     Bytes to digest.
*  \param[in]     len      Number of unsigned chars to digest.
*  \param[out]    digest   128-bit digest.
*/
YIFFURC_LINK void YIFFURC_CALL    auth_digest128( const char* data, int len,
                                                  union AUTH_DIGEST128* digest );

/** Client machine variables. */
struct AUTH_MACHINEVARS
{
   /** IP=, the host machine's public IP address, in network order. */
   int               ip4;
   /** Number of fixed drives. */
   unsigned int      num_drives;
   /** H= Serial numbers of each major drive (C:\ D:\ E:\ A:\) */
   unsigned int      volume_sn[ 4 ];
   /** R= Current time. */
   unsigned int      time;
   /** D= Summed free clusters of all the drives. */
   int            total_free_clusters;
   /** U= Should be some fixed hardware number. */
   unsigned int      h_e_z;
   /** M= */
   unsigned int      screen_width;
   /** M= */
   unsigned int      screen_height;
   /* V= (always 22.4) */
   struct MAJORMINOR version;
};

/** Generate a machine ID string from the client.
*  \param[in]     vars  Client machine variables.
*  \param[in]  scramble Set to \c false to return the string unscrambled.
*  \returns A machine ID string that should be
*           freed with yiffurc_release() when no longer needed. */
YIFFURC_LINK const char* YIFFURC_CALL auth_stringclient( const struct AUTH_MACHINEVARS* vars,
                                                         bool scramble );

/** Generate a machine ID string from the server.
*  \param[in]     vars  Client machine variables.
*  \param[in]  scramble Set to \c false to return the string unscrambled.
*  \returns A machine ID string that should be
*           freed with yiffurc_release() when no longer needed. */
YIFFURC_LINK const char* YIFFURC_CALL auth_stringserver( const struct AUTH_MACHINEVARS* vars,
                                                         bool scramble );

/** Extracts client machine variables from a scrambled machine ID string.
*  \param[in]     str   Scrambled machine ID string.
*  \param[out]    vars  Extracted machine variables.
*  \retval true Extraction succeeded. */
// YIFFURC_LINK bool YIFFURC_CALL auth_stringvars( const char* str, AUTH_MACHINEVARS* vars );

#if defined( __cplusplus )
} // namespace yiffurc
#endif

/** \} auth */

#endif
