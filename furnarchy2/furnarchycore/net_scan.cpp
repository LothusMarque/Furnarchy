#include "common.h"
#include "net_scan.h"
#include "yiffurc/common.h"

namespace net_scan {

bool scan( const char* pattern, 
           const char* subj, int subj_len,
           list<FIELD>& fields )
{
   assert( pattern );

   if (subj_len == -1)
   {
      assert( subj );
      subj_len = strlen( subj );
   }

   assert( subj );

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
         {
            field_len = -1;
            ++pattern;
         }

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

               FIELD f;
               f.type      = FIELD::FIELD_STR;
               f.value_str.assign( subj + subj_pos, field_len );
               fields.push_back( f );

               pattern  += 1;
               break;
            }

         case 's': // Byte-string.
            {
               if ((unsigned) subj[ subj_pos ] < 35)
                  return false;

               field_len = 1 + yiffurc::ftoi1_220( subj[ subj_pos ] );
               if ((unsigned) (subj_len - subj_pos) < field_len)
                  return false;

               FIELD f;
               f.type      = FIELD::FIELD_STR;
               f.value_str.assign( subj + subj_pos + 1, field_len - 1 );
               fields.push_back( f );

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

               FIELD f;
               f.type      = FIELD::FIELD_STR;
               f.value_str.assign( subj + subj_pos, field_len );
               fields.push_back( f );

               pattern += 1;
               break;
            }

         case 'K': // New-style color code.
            {
               field_len = yiffurc::get_color_len( subj + subj_pos, subj_len - subj_pos );

               for (unsigned i = 0; i < field_len; ++i)
                  if ((unsigned) subj[ subj_pos + i ] < 35)
                     return false;

               FIELD f;
               f.type      = FIELD::FIELD_STR;
               f.value_str.assign( subj + subj_pos, field_len );
               fields.push_back( f );

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

               FIELD f;
               f.type      = FIELD::FIELD_INT;
               f.value_int = n;
               fields.push_back( f );

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

               FIELD f;
               f.type      = FIELD::FIELD_INT;
               f.value_int = n;
               fields.push_back( f );

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

   // If exhausted both the pattern and suibject, scan was a success.
   return (subj_pos == subj_len && !*pattern);
}

} // namespace net_scan