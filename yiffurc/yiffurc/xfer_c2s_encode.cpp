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
#include "xfer_c2s_enc_stock.h"

namespace yiffurc {

const XFER_C2S_ENC_STOCK xfer_c2s_enc_stock;

const char* YIFFURC_CALL xfer_c2s_enc_atomicfetch( const char* player,
                                                const char* pass,
                                                const char* package,
                                                size_t start_off,
                                                unsigned int send_ahead,
                                                size_t chunk_size )
{
   size_t player_len     = STRLEN( player );
   size_t pass_len       = STRLEN( pass );
   size_t package_len    = STRLEN( package );
   size_t start_off_len  = dec_len( (unsigned int) start_off );
   size_t send_ahead_len = dec_len( send_ahead );
   size_t chunk_size_len = dec_len( (unsigned int) chunk_size );

   char* out = pfx_new<char>( 3 + player_len + 1 + pass_len +
                              1 + package_len + 1 + start_off_len +
                              1 + send_ahead_len + 1 + chunk_size_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "RC ", 3 );
   out = chain_memcpy( out, player, player_len );
   *out = ' ';  out++;
   out = chain_memcpy( out, pass, pass_len );
   *out = ' ';  out++;
   out = chain_memcpy( out, package, package_len );
   *out = ' ';  out++;
   out = chain_itoa( out, (unsigned int) start_off );
   *out = ' ';  out++;
   out = chain_itoa( out, send_ahead );
   *out = ' ';  out++;
   out = chain_itoa( out, (unsigned int) chunk_size );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const unsigned char* YIFFURC_CALL xfer_c2s_enc_chunkhead( unsigned int chunk_id, unsigned int chunk_crc32 )
{
   chunk_id    = machine_to_lsb32( chunk_id );
   chunk_crc32 = machine_to_lsb32( chunk_crc32 );

   unsigned char* out = (unsigned char*) pfx_new<char>( 2 + sizeof( unsigned int ) + sizeof( unsigned int ) );
   const unsigned char* const ret = out;

   *out = 'S'; out++;
   *out = 'C'; out++;
   *(unsigned int*)out = chunk_id; out += sizeof( unsigned int );
   *(unsigned int*)out = chunk_crc32; out += sizeof( unsigned int );

   return ret;
}

const char* YIFFURC_CALL xfer_c2s_enc_password( const char* pass )
{
   size_t pass_len = STRLEN( pass );

   char* out = pfx_new<char>( 5 + pass_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "PASS ", 5 );
   out = chain_memcpy( out, pass, pass_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL xfer_c2s_enc_setr( int unk )
{
   size_t unk_len = dec_len( unk );

   char* out = pfx_new<char>( 5 + unk_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "SETR ", 5 );
   out = chain_itoa( out, unk );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL xfer_c2s_enc_upload2pkg( unsigned int dream_id, int unk )
{
   size_t dream_id_len = dec_len( dream_id );
   size_t unk_len      = dec_len( unk );

   char* out = pfx_new<char>( 8 + dream_id_len + 1 + unk_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "SNCSEND ", 8 );
   out = chain_itoa( out, dream_id );
   *out = ' '; out++;
   out = chain_itoa( out, unk );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL xfer_c2s_enc_uploadinfo( const char* filename,
                                               size_t filesize )
{
   size_t filename_len = STRLEN( filename );
   size_t filesize_len = dec_len( (unsigned int) filesize );

   char* out = pfx_new<char>( 5 + filename_len + 1 + filesize_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "SEND ", 5 );
   out = chain_memcpy( out, filename, filename_len );
   *out = ' '; out++;
   out = chain_itoa( out, (unsigned int) filesize );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL xfer_c2s_enc_user( const char* player )
{
   size_t player_len = STRLEN( player );

   char* out = pfx_new<char>( 5 + player_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "USER ", 5 );
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL xfer_c2s_enc_bye( )
{
   return xfer_c2s_enc_stock.sz_bye;
}

} // namespace yiffurc