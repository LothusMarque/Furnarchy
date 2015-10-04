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
#include <list>
#include <string>
#include <cstdarg>
#ifdef _MSC_VER
   #include <xutility>
#endif
#include <cassert>

namespace yiffurc {

int YIFFURC_CALL net_scan( const char* pattern, 
                           const char* subj, int subj_len,
                           ... )
{
   assert( pattern );

   if (subj_len == -1)
   {
      assert( subj );
      subj_len = strlen( subj );
   }

   assert( subj );

   va_list vargs;
   va_start( vargs, subj_len );
   int subj_pos = 0;

   while (subj_pos < subj_len && pattern[ 0 ])
   {
      // Format specifier?
      if (pattern[ 0 ] == '%')
      {
         ++pattern;

         // Read in the length field.
         unsigned field_len = 1;
         if (pattern[ 0 ] >= '0' && pattern[ 0 ] <= '9')
         {
            for (field_len = 0; pattern[ 0 ] >= '0' && pattern[ 0 ] <= '9'; ++pattern)
               field_len = field_len * 10 + (pattern[ 0 ] - '0');
         }
         else if (pattern[ 0 ] == '*') // Infinite length.
            field_len = -1;

         // Read in field.
         switch (pattern[ 0 ])
         {
         case '%': // Literal '%'
            {
               field_len = 1;

               if (subj[ subj_pos ] != '%')
                  return false;

               pattern  += 1;
               break;
            }

         case 'c': // One character.
            {
               const bool is_cstr = (field_len == -1);

               if ((unsigned) (subj_len - subj_pos) < field_len)
               {
                  if (field_len == -1)
                     field_len = subj_len - subj_pos;
                  else
                     return false;
               }

               char* buf = va_arg( vargs, char* );
               memcpy( buf, subj + subj_pos, field_len );
               if (is_cstr)
                  buf[ field_len ] = '\0';

               pattern  += 1;
               break;
            }

         case 's': // Byte-string.
            {
               if ((unsigned) subj[ subj_pos ] < 35)
                  return false;

               field_len = 1 + ftoi1_220( subj[ subj_pos ] );
               if ((unsigned) (subj_len - subj_pos) < field_len)
                  return false;

               char* buf = va_arg( vargs, char* );
               memcpy( buf, subj + subj_pos + 1, field_len - 1 );
               buf[ field_len - 1 ] = '\0';

               pattern += 1;
               break;
            }

         case 'k': // Old-style color code.
            {
               field_len = 10;

               if ((unsigned) (subj_len - subj_pos) < field_len)
                  return false;

               for (unsigned i = 0; i < field_len; ++i)
                  if ((unsigned) subj[ subj_pos + i ] < 0x20 || (unsigned) subj[ subj_pos + i ] > 0x7E)
                     return false;

               char* buf = va_arg( vargs, char* );
               memcpy( buf, subj + subj_pos, field_len );
               buf[ field_len ] = '\0';

               pattern += 1;
               break;
            }

         case 'K': // New-style color code.
            {
               field_len = get_color_len( subj + subj_pos, subj_len - subj_pos );
               
               for (unsigned i = 0; i < field_len; ++i)
                  if ((unsigned) subj[ subj_pos + i ] < 35)
                     return false;

               char* buf = va_arg( vargs, char* );
               memcpy( buf, subj + subj_pos, field_len );
               buf[ field_len ] = '\0';

               pattern += 1;
               break;
            }

         case 'n': // Base-95 digit.
            {
               if ((unsigned) (subj_len - subj_pos) < field_len)
                  return false;

               for (unsigned i = 0; i < field_len; ++i)
                  if ((unsigned) subj[ subj_pos + i ] < 0x20 || (unsigned) subj[ subj_pos + i ] > 0x7E)
                     return false;

               unsigned n = 0;
               for (unsigned i = 0; i < field_len; ++i)
                  n = (n * 95) + ((unsigned) subj[ subj_pos + i ] - (unsigned) ' ');

               int* buf = va_arg( vargs, int* );
               *buf = n;

               pattern += 1;
               break;
            }

         case 'N': // Base-220 digit.
            {
               if ((unsigned) (subj_len - subj_pos) < field_len)
                  return false;

               for (unsigned i = 0; i < field_len; ++i)
                  if ((unsigned) subj[ subj_pos + i ] < 35)
                     return false;

               unsigned n = 0;
               for (unsigned mul = 1, i = 0; i < field_len; mul *= 220, ++i)
                  n += ((unsigned char) subj[ subj_pos + i ] - (unsigned char) '#') * mul;
               
               int* buf = va_arg( vargs, int* );
               *buf = n;

               pattern += 1;
               break;
            }

         default: // Unknown.  Assume literal '%'
            {
               field_len = 1;

               if (subj[ subj_pos ] != '%')
                  return false;
               // Don't advance the pattern.
               break;
            }
         }

         subj_pos += field_len;
      }
      else // Literal.
      {
         if (subj[ subj_pos++ ] != (pattern++)[ 0 ])
            return false;
      }
   }

   va_end( vargs );

   // If exhausted both the pattern and suibject, scan was a success.
   return (subj_pos == subj_len && !*pattern);
}


} // namespace yiffurc