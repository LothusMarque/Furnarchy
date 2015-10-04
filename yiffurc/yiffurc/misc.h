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

#ifndef _YIFFURC_PFX_MISC_H__
#define _YIFFURC_PFX_MISC_H__

#include "pfx_heap.h"
#include <cstring>

namespace yiffurc {

using std::memcpy;

#ifdef __GNUC__
   #define TLS_DATA __thread
#else
   #define TLS_DATA __declspec( thread )
#endif

#define STRLEN( s ) ( (s) ? strlen( s ) : 0 )

/* memcpy that returns pointer to end position. */
inline char* chain_memcpy( char* dst, const void* src, size_t len )
{
   char* const end = dst + len;

   while (dst < end)
   {
      *dst = *((const char*) src);
      ++dst; ++((const char*&) src);
   }

   return end;
}

/* length of a decimal number (as a string). */
inline size_t dec_len( int n )
{
   if (!n)
      return 1;

   size_t len = (n >= 0) ? 0 : 1;

   do
   {
      n /= 10;
      ++len;
   } while ( n );

   return len;
}

/* length of a decimal number (as a string). */
inline size_t dec_len( unsigned int n )
{
   if (!n)
      return 1;

   size_t len = 0;

   do
   {
      n /= 10;
      ++len;
   } while ( n );

   return len;
}

/* converts an integer to a decimal number string
*  and returns the end position. DOES NOT NULL TERMINATE*/
inline char* chain_itoa( char* dst, int n )
{
   if (n < 0)
   {
      *dst = '-';
      ++dst;
      n *= -1;
   }

   int t      = n;
   size_t len = 0;

   do
   {
      t /= 10;
      ++len;
   } while ( t );

   char* p = dst + (len - 1);

   while (p >= dst)
   {
      *(p--) = (n % 10) + '0';
      n /= 10;
   }

   return dst + len;
}

/* converts an unsigned integer to a decimal number string
*  and returns the end position. DOES NOT NULL TERMINATE */
inline char* chain_itoa( char* dst, unsigned int n )
{
   unsigned int t = n;
   size_t len     = 0;

   do
   {
      t /= 10;
      ++len;
   } while ( t );

   char* p = dst + (len - 1);

   while (p >= dst)
   {
      *(p--) = (n % 10) + '0';
      n /= 10;
   }

   return dst + len;
}

/* Duplicates a string as a C-string. */
inline char* strnnew( const char* src, size_t len )
{
   char* str = new char[ len + 1 ];

   const char* const end = src + len;
   char* dst             = str;

   while (src != end)
      *(dst++) = *(src++);

   *dst = '\0';
   return str;
}


/* Converts a decimal string to an integer. */
inline int antoi( const char* a, size_t len )
{
   if (len)
   {
      int sign = 1;

      if (a[ 0 ] == '-')
      {
         sign = -1;
         a++, len--;
      }

      int r = 0;
      for (size_t i = 0; i < len; i++)
      {
         if (a[ i ] >= '0' && a[ i ] <= '9')
            r = r * 10 + (a[ i ] - '0');
         else
            break;
      }

      return sign * r;
   }

   return 0;
}

/* Reverses a sequence of unsigned chars. */
inline char* strnrev( char* dst, const char* src, size_t len )
{
   if (dst == src)
   {
      char* left  = dst;
      char* right = dst + (signed)(len - 1);
      char t;

      while (left < right)
      {
         t = *left;
         *left = *right;
         *right = t;

         left++;
         right--;
      }
   }
   else
   {
      const char* src_p = src + (signed)(len - 1);
      char* dst_p       = dst;

      while (src_p > src)
         *(dst_p++) = *(src_p--);
   }

   return dst;
}

inline int endian_swap32( int i )
{
   return ((i & 0xFF) << 24)  |
      ((i & 0xFF00) << 8)     |
      ((i & 0xFF0000) >> 8)   |
      ((i & 0xFF000000) >> 24);
}

inline short endian_swap16( short s )
{
   return ((s & 0xFF) << 8)  |
      ((s & 0xFF00) >> 8);
}

#ifdef YIFFURC_MSB

inline int lsb_to_machine32( int i ) { return endian_swap32( i ); }
inline int machine_to_lsb32( int i ) { return endian_swap32( i ); }
inline short lsb_to_machine16( short s ) { return endian_swap16( s ); }
inline short machine_to_lsb16( short s ) { return endian_swap16( s ); }

#else

inline int lsb_to_machine32( int i ) { return i; }
inline int machine_to_lsb32( int i ) { return i; }
inline short lsb_to_machine16( short s ) { return s; }
inline short machine_to_lsb16( short s ) { return s; }

#endif

} // namespace yiffurc

#endif
