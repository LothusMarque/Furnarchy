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

#include "auth.h"
#include "misc.h"
#include <string>

namespace yiffurc {

/* Used by auth_digest128 */
struct GENS_DATA
{
   unsigned char sum[ 16 ];   // Might make more sense as unsigned int[4]
   unsigned int  unk1;
   unsigned int  unk2;
   unsigned char buf[ 64 ];
};

inline void auth_scramble_swap1( char* p, size_t len )
{
   char* e = p + (signed)(len-1);
   char t;

   while (p < e)
   {
      t = *p;
      *p = *e;
      *e = t;

      p += 2;
      e -= 2;
   }
}

inline void auth_scramble_swap2( char* p, size_t len )
{
   char* e = p + (signed)(len-1);
   char t;

   while (p + 1 < e - 1)
   {
      t = *p;
      *p = *(e);
      *(e) = t;

      t = *(p + 1);
      *(p + 1) = *(e - 1);
      *(e - 1) = t;

      p += 2;
      e -= 2;
   }
}

char* YIFFURC_CALL auth_scramble( char* dst,
                                  const char* src, size_t len )
{
   if (src != dst)
      memcpy( dst, src, len );

   auth_scramble_swap1( dst, len );

   if (len > 1)
   {
      auth_scramble_swap2( dst + 1, len - 1 );
      if (len > 2)
      {
         auth_scramble_swap2( dst + 2, len - 2 );
         if (len > 3)
         {
            auth_scramble_swap1( dst + 3, len - 3 );
            if (len > 4)
            {
               auth_scramble_swap1( dst + 4, len - 4 );
               if (len > 5)
               {
                  auth_scramble_swap2( dst + 5, len - 5 );
                  if (len > 6)
                  {
                     auth_scramble_swap2( dst + 6, len - 6 );
                     if (len > 7)
                     {
                        auth_scramble_swap1( dst + 7, len - 7 );
                        if (len > 8)
                        {
                           auth_scramble_swap2( dst + 8, len - 8 );
                        }
                     }
                  }
               }
            }
         }
      }
   }

   return dst;
}

char* YIFFURC_CALL auth_descramble( char* dst,
                                    const char* src, size_t len )
{
   if (src != dst)
      memcpy( dst, src, len );

   switch (len)
   {
   default:
      auth_scramble_swap2( dst + 8, len - 8 );
   case 8:
      auth_scramble_swap1( dst + 7, len - 7 );
   case 7:
      auth_scramble_swap2( dst + 6, len - 6 );
   case 6:
      auth_scramble_swap2( dst + 5, len - 5 );
   case 5:
      auth_scramble_swap1( dst + 4, len - 4 );
   case 4:
      auth_scramble_swap1( dst + 3, len - 3 );
   case 3:
      auth_scramble_swap2( dst + 2, len - 2 );
   case 2:
      auth_scramble_swap2( dst + 1, len - 1 );
   case 1:
      auth_scramble_swap1( dst, len );
   case 0:
      break;
   }

   return dst;
}

/* 0040FBD0 */
void s_foo_2( unsigned char* in_out, const unsigned char* in )
{
   /* 64-unsigned char digest */
   unsigned int local_16[ 16 ];
   unsigned int local_17 = ((unsigned int*)in_out)[ 0 ];
   unsigned int local_18 = ((unsigned int*)in_out)[ 1 ];
   unsigned int local_20 = ((unsigned int*)in_out)[ 2 ];
   unsigned int local_19 = ((unsigned int*)in_out)[ 3 ];

   memcpy( (unsigned char*) local_16, in, 64 );

   #define _S_INT_MAGIC_1( a, b, c, d, i , x, l, r )  \
              a = ( ( b & d ) | ( (~b) & c ) ) \
                    + local_16[ i ] + a + (unsigned)x; \
              a = ( a << l ) | ( a >> r ); \
              a = a + b

   _S_INT_MAGIC_1( local_17, local_18, local_19, local_20,
                   0, 0xD74AA478, 7, 25 );

   _S_INT_MAGIC_1( local_19, local_17, local_20, local_18,
                   1, 0xE3CEB456, 12, 20 );

   _S_INT_MAGIC_1( local_20, local_19, local_18, local_17,
                   2, 0x2420B0DB, 17, 15 );

   _S_INT_MAGIC_1( local_18, local_20, local_17, local_19,
                   3, 0x212AC5EE, 22, 10 );

   _S_INT_MAGIC_1( local_17, local_18, local_19, local_20,
                   4, 0x325C6CAF, 7, 25 );

   _S_INT_MAGIC_1( local_19, local_17, local_20, local_18,
                   5, 0x4785B22A, 12, 20 );

   _S_INT_MAGIC_1( local_20, local_19, local_18, local_17,
                   6, 0x48346C63, 17, 15 );

   _S_INT_MAGIC_1( local_18, local_20, local_17, local_19,
                   7, 0xF2485501, 22, 10 );

   _S_INT_MAGIC_1( local_17, local_18, local_19, local_20,
                   8, 0x3980C258, 7, 25 );

   _S_INT_MAGIC_1( local_19, local_17, local_20, local_18,
                   9, 0x834412AF, 12, 20 );

   _S_INT_MAGIC_1( local_20, local_19, local_18, local_17,
                   10, 0x3DCF65B1, 17, 15 );

   _S_INT_MAGIC_1( local_18, local_20, local_17, local_19,
                   11, 0x831CD76E, 22, 10 );

   _S_INT_MAGIC_1( local_17, local_18, local_19, local_20,
                   12, 0x6B605552, 7, 25 );

   _S_INT_MAGIC_1( local_19, local_17, local_20, local_18,
                   13, 0xFD937193, 12, 20 );

   _S_INT_MAGIC_1( local_20, local_19, local_18, local_17,
                   14, 0xA669B38E, 17, 15 );

   _S_INT_MAGIC_1( local_18, local_20, local_17, local_19,
                   15, 0x44C43821, 22, 10 );

   #define _S_INT_MAGIC_2( a, b, c, d, i , x, l, r )  \
              a = ( ( b & d ) | ( (~d) & c ) ) \
                    + local_16[ i ] + a + x; \
              a = ( a << l ) | ( a >> r ); \
              a = a + b

   _S_INT_MAGIC_2( local_17, local_18, local_20, local_19,
                   1, 0x341E2562, 5, 0x1B );

   _S_INT_MAGIC_2( local_19, local_17, local_18, local_20,
                   6, 0xC0EAB340, 9, 0x17 );

   _S_INT_MAGIC_2( local_20, local_19, local_17, local_18,
                   11, 0x265B3A51, 0xE, 0x12 );

   _S_INT_MAGIC_2( local_18, local_20, local_19, local_17,
                   0, 0xE9B6C7AA, 0x14, 0xC );

   _S_INT_MAGIC_2( local_17, local_18, local_20, local_19,
                   5, 0xD62F132D, 5, 0x1B );

   _S_INT_MAGIC_2( local_19, local_17, local_18, local_20,
                   10, 0x2541B435, 9, 0x17 );

   _S_INT_MAGIC_2( local_20, local_19, local_17, local_18,
                   15, 0xD8A1E681, 0xE, 0x12 );

   _S_INT_MAGIC_2( local_18, local_20, local_19, local_17,
                   4, 0x37B3FBC8, 0x14, 0xC );

   _S_INT_MAGIC_2( local_17, local_18, local_20, local_19,
                   9, 0x21E1C3E6, 5, 0x1B );

   _S_INT_MAGIC_2( local_19, local_17, local_18, local_20,
                   14, 0xC33707D6, 9, 0x17 );

   _S_INT_MAGIC_2( local_20, local_19, local_17, local_18,
                   3, 0xFBD50D87, 0xE, 0x12 );

   _S_INT_MAGIC_2( local_18, local_20, local_19, local_17,
                   8, 0x455A34ED, 0x14, 0xC );

   _S_INT_MAGIC_2( local_17, local_18, local_20, local_19,
                   13, 0xA9E38905, 5, 0x1B );

   _S_INT_MAGIC_2( local_19, local_17, local_18, local_20,
                   2, 0x8CE3A3F8, 9, 0x17 );

   _S_INT_MAGIC_2( local_20, local_19, local_17, local_18,
                   7, 0x676F82D9, 0xE, 0x12 );

   _S_INT_MAGIC_2( local_18, local_20, local_19, local_17,
                   12, 0xBD234B8A, 0x14, 0xC );

   #define _S_INT_MAGIC_3( a, b, c, d, i , x, l, r )  \
              a = ( ( b ^ c ) ^ d ) \
                  + local_16[ i ] + a + x; \
              a = ( a << l ) | ( a >> r ); \
              a = a + b

   _S_INT_MAGIC_3( local_17, local_18, local_20, local_19,
                   5, 0x32FA3942, 0x4, 0x1C );

   _S_INT_MAGIC_3( local_19, local_17, local_18, local_20,
                   8, 0x87E1F681, 0xB, 0x15 );

   _S_INT_MAGIC_3( local_20, local_19, local_17, local_18,
                   11, 0x6D3D6122, 0x10, 0x10 );

   _S_INT_MAGIC_3( local_18, local_20, local_19, local_17,
                   14, 0xFDE2A80C, 0x17, 0x9 );

   _S_INT_MAGIC_3( local_17, local_18, local_20, local_19,
                   1, 0xA4B4EA14, 0x4, 0x1C );

   _S_INT_MAGIC_3( local_19, local_17, local_18, local_20,
                   4, 0x4A7ECFA9, 0xB, 0x15 );

   _S_INT_MAGIC_3( local_20, local_19, local_17, local_18,
                   7, 0xFB6B46B0, 0x10, 0x10 );

   _S_INT_MAGIC_3( local_18, local_20, local_19, local_17,
                   10, 0xBE7FBC70, 0x17, 0x9 );

   _S_INT_MAGIC_3( local_17, local_18, local_20, local_19,
                   13, 0x289E75C6, 0x4, 0x1C );

   _S_INT_MAGIC_3( local_19, local_17, local_18, local_20,
                   0, 0xE33127FA, 0xB, 0x15 );

   _S_INT_MAGIC_3( local_20, local_19, local_17, local_18,
                   3, 0xD4E73E85, 0x10, 0x10 );

   _S_INT_MAGIC_3( local_18, local_20, local_19, local_17,
                   6, 0x4881D075, 0x17, 0x9 );

   _S_INT_MAGIC_3( local_17, local_18, local_20, local_19,
                   9, 0xD9BED539, 0x4, 0x1C );

   _S_INT_MAGIC_3( local_19, local_17, local_18, local_20,
                   12, 0xE6DB97E5, 0xB, 0x15 );

   _S_INT_MAGIC_3( local_20, local_19, local_17, local_18,
                   15, 0xEFA27CF8, 0x10, 0x10 );

   _S_INT_MAGIC_3( local_18, local_20, local_19, local_17,
                   2, 0xCFA55665, 0x17, 0x9 );

   #define _S_INT_MAGIC_4( a, b, c, d, i , x, l, r )  \
              a = ( ( (~c) | b ) ^ d ) \
                  + local_16[ i ] + a + x; \
              a = ( a << l ) | ( a >> r ); \
              a = a + b

   _S_INT_MAGIC_4( local_17, local_18, local_19, local_20,
                   0, 0x14292244, 0x6, 0x1A );

   _S_INT_MAGIC_4( local_19, local_17, local_20, local_18,
                   7, 0x4A2AFF97, 0x0A, 0x16 );

   _S_INT_MAGIC_4( local_20, local_19, local_18, local_17,
                   14, 0xABE223A7, 0x0F, 0x11 );

   _S_INT_MAGIC_4( local_18, local_20, local_17, local_19,
                   5, 0xFC93A039, 0x15, 0x0B );

   _S_INT_MAGIC_4( local_17, local_18, local_19, local_20,
                   12, 0x25B1663C, 0x6, 0x1A );

   _S_INT_MAGIC_4( local_19, local_17, local_20, local_18,
                   3, 0x8F0CCC92, 0x0A, 0x16 );

   _S_INT_MAGIC_4( local_20, local_19, local_18, local_17,
                   10, 0x4F4FF47D, 0x0F, 0x11 );

   _S_INT_MAGIC_4( local_18, local_20, local_17, local_19,
                   1, 0x85845DD1, 0x15, 0x0B );

   _S_INT_MAGIC_4( local_17, local_18, local_19, local_20,
                   8, 0x61A57E4F, 0x6, 0x1A );

   _S_INT_MAGIC_4( local_19, local_17, local_20, local_18,
                   15, 0x6E2CE6E0, 0x0A, 0x16 );

   _S_INT_MAGIC_4( local_20, local_19, local_18, local_17,
                   6, 0xA0313414, 0x0F, 0x11 );

   _S_INT_MAGIC_4( local_18, local_20, local_17, local_19,
                   13, 0x4E0711A1, 0x15, 0x0B );

   _S_INT_MAGIC_4( local_17, local_18, local_19, local_20,
                   4, 0xF8537E82, 0x6, 0x1A );

   _S_INT_MAGIC_4( local_19, local_17, local_20, local_18,
                   11, 0xBD38F235, 0x0A, 0x16 );

   _S_INT_MAGIC_4( local_20, local_19, local_18, local_17,
                   2, 0x8AD7D2BB, 0x0F, 0x11 );

   _S_INT_MAGIC_4( local_18, local_20, local_17, local_19,
                   9, 0xEB868991, 0x15, 0x0B );

   ((unsigned int*)in_out)[ 0 ] += local_17;
   ((unsigned int*)in_out)[ 1 ] += local_18;
   ((unsigned int*)in_out)[ 2 ] += local_20;
   ((unsigned int*)in_out)[ 3 ] += local_19;
}


/* 0040FA90 */
void s_foo_1( GENS_DATA* data, const unsigned char* in, unsigned int len )
{
   unsigned int local_1;
   unsigned int local_2    = ( data->unk1 >> 3 ) & 0x3F;
   data->unk1  += len * 8;

   if (data->unk1 < ( len << 3 ))
   {
      data->unk2++;
   }

   data->unk2 += ( len >> 29 );

   unsigned int local_3 = 0x40 - local_2;

   if (len >= local_3)
   {
      memcpy( data->buf + local_2, in, local_3 );

      s_foo_2( data->sum, data->buf ); // ?

      for (local_1 = local_3; local_1 + 63 < len; local_1 += 64)
      {
         s_foo_2( data->sum, in + local_1 ); // ?
      }

      local_2 = 0;
   }
   else
      local_1 = 0;

   memcpy( data->buf + local_2, in + local_1, len - local_1 );

}

/* 00410AD0 */
void s_foo_3( unsigned char out[16], GENS_DATA* data )
{
   /* 0x0049FCB0 */
   static const unsigned char s_blank_input[ 64 ] =
   { '\x80', 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0 };

   unsigned int local_1;
   unsigned int local_3[ 2 ];
   unsigned int local_4;
   unsigned int local_5;

   memcpy( (unsigned char*) local_3, &data->unk1, 8 );

   local_1 = ( data->unk1 >> 3 ) & 0x3F;

   if (local_1 < 0x38)
   {
      local_5 = 0x38 - local_1;
   }
   else
   {
      local_5 = 0x78 - local_1;
   }

   local_4 = local_5;

   s_foo_1( data, s_blank_input, local_4 );

   s_foo_1( data, (unsigned char*) local_3, 8 );

   memcpy( out, data, 16 );
}

void YIFFURC_CALL auth_digest128( const char* data, size_t len,
                                  AUTH_DIGEST128* digest )
{
   GENS_DATA s_data;

   ((unsigned int*)s_data.sum)[ 0 ]   = 0x67452301;
   ((unsigned int*)s_data.sum)[ 1 ]   = 0xEFCDAB89;
   ((unsigned int*)s_data.sum)[ 2 ]   = 0x98BADCFE;
   ((unsigned int*)s_data.sum)[ 3 ]   = 0x10325476;

   s_data.unk1 = 0;
   s_data.unk2 = 0;

   s_foo_1( &s_data, (const unsigned char*) data, (unsigned int) len );

   s_foo_3( digest->b, &s_data );
}

const char* YIFFURC_CALL auth_stringclient( const AUTH_MACHINEVARS* vars, bool scramble )
{
   std::string id;

   char t[ 32 ];
   id += "IP=";
   *chain_itoa( t, ((vars->ip4) & 0xFF) ) = '\0';
   id += t;
   id += '.';
   *chain_itoa( t, ((vars->ip4 >> 8) & 0xFF) ) = '\0';
   id += t;
   id += '.';
   *chain_itoa( t, ((vars->ip4 >> 16) & 0xFF) ) = '\0';
   id += t;
   id += '.';
   *chain_itoa( t, ((vars->ip4 >> 24) & 0xFF) ) = '\0';
   id += t;

   for (unsigned int i=0; i < vars->num_drives && i < 4; i++)
   {
      static const char base14syms[ 14 ] =
      {
         '0', '1', '2', '3', '4', '5', '6' , '7', '8', '9',
         'a', 'b', 'c', 'd',
      };

      id += " H=";

      unsigned int n = vars->volume_sn[ i ];
      char* p = t;

      do
      {
         *p = base14syms[ n % 14 ];
         n = n / 14;
         p++;

      } while (n);

      *p = '\0';

      id += strnrev( t, t, strlen( t ) );
   }

   id += " R=";
   *chain_itoa( t, vars->time % 10000 ) = '\0';
   id += t;

   id += " D=";
   *chain_itoa( t, (unsigned int) vars->total_free_clusters >> 8 ) = '\0';
   id += t;
   id += (char) ((vars->num_drives > 4 ? 4 : vars->num_drives) + '0');

   id += " U=";
   *chain_itoa( t, vars->h_e_z + 1859638 ) = '\0';
   id += t;

   id += " M=";
   *chain_itoa( t, vars->screen_width ) = '\0';
   id += t;
   id += 'x';
   *chain_itoa( t, vars->screen_height ) = '\0';
   id += t;

   id += " V=";
   *chain_itoa( t, vars->version.major ) = '\0';
   id += t;
   id += '.';
   *chain_itoa( t, vars->version.minor ) = '\0';
   id += t;

   size_t id_len = id.length( ); // Without P= tag.

   id += " P=168356072050782049568476";

   /* Digest it. */
   AUTH_DIGEST128 digest;
   auth_digest128( (const char*) id.data( ), id.length( ), &digest );

   /* Have all we need now. */
   char* sz_result = pfx_new<char>( id.length( ) + 3 + 32 + 1 );
   char* p = chain_memcpy( chain_memcpy( sz_result, id.c_str( ), id_len ),
                           " S=", 3 );

   /* The S= tag is the digest. */
   for (unsigned int i =0; i < 16; i++, p+=2)
      sprintf( p, "%02x", digest.b[ i ] );

   if (scramble)
   {
      /* Scramble it */
      auth_scramble( (char*) sz_result,
                     (char*) sz_result, strlen( sz_result ) );
   }

   return sz_result;
}

const char* YIFFURC_CALL auth_stringserver( const AUTH_MACHINEVARS* vars, bool scramble )
{
   std::string id;

   char t[ 32 ];
   id += "IP=";
   *chain_itoa( t, ((vars->ip4) & 0xFF) ) = '\0';
   id += t;
   id += '.';
   *chain_itoa( t, ((vars->ip4 >> 8) & 0xFF) ) = '\0';
   id += t;
   id += '.';
   *chain_itoa( t, ((vars->ip4 >> 16) & 0xFF) ) = '\0';
   id += t;
   id += '.';
   *chain_itoa( t, ((vars->ip4 >> 24) & 0xFF) ) = '\0';
   id += t;

   for (unsigned int i=0; i < vars->num_drives && i < 4; i++)
   {
      static const char base14syms[ 14 ] =
      {
         '0', '1', '2', '3', '4', '5', '6' , '7', '8', '9',
         'a', 'b', 'c', 'd',
      };

      id += " H=";

      unsigned int n = vars->volume_sn[ i ];
      char* p = t;

      do
      {
         *p = base14syms[ n % 14 ];
         n = n / 14;
         p++;

      } while (n);

      *p = '\0';

      id += strnrev( t, t, strlen( t ) );
   }

   id += " R=";
   *chain_itoa( t, vars->time % 10000 ) = '\0';
   id += t;

   id += " D=";
   *chain_itoa( t, (unsigned int) vars->total_free_clusters >> 8 ) = '\0';
   id += t;
   id += (char) ((vars->num_drives > 4? 4 : vars->num_drives) + '0');

   id += " U=";
   *chain_itoa( t, vars->h_e_z + 1859638 ) = '\0';
   id += t;

   id += " M=";
   *chain_itoa( t, vars->screen_width ) = '\0';
   id += t;
   id += 'x';
   *chain_itoa( t, vars->screen_height ) = '\0';
   id += t;

   id += " V=";
   *chain_itoa( t, vars->version.major ) = '\0';
   id += t;
   id += '.';
   *chain_itoa( t, vars->version.minor ) = '\0';
   id += t;

   size_t id_len = id.length( ); // Without P= tag.

   id += " P=293968305977398238506023";

   /* Digest it. */
   AUTH_DIGEST128 digest;
   auth_digest128( (const char*) id.data( ), id.length( ), &digest );

   /* Have all we need now. */
   char* sz_result = pfx_new<char>( id.length( ) + 3 + 32 + 1  );
   char* p = chain_memcpy( chain_memcpy( sz_result, id.c_str( ), id_len ),
                           " S=", 3 );

   /* The S= tag is the digest. */
   for (unsigned int i =0; i < 16; i++, p+=2)
      sprintf( p, "%02x", digest.b[ i ] );

   if (scramble)
   {
      /* Scramble it */
      auth_scramble( (char*) sz_result,
                     (char*) sz_result, strlen( sz_result ) );
   }

   return sz_result;
}

} // namespace yiffurc