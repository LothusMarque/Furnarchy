#include "common.h"

namespace yiffurc
{

YIFFURC_LINK unsigned YIFFURC_CALL get_color_len( const char* color, const unsigned len )
{
   if (!len)
      return 0;
   
   unsigned _len = 1;

   switch (*color)
   {
   case 't':
      _len += len - _len < 10 ? len - _len : 10;
      break;

   case 'u':
      // RGB and paletted values are mixed.
      if (_len < len)
      {
         // Get the encoding bitmasks.
         const unsigned char mask_a = ftoi1_220( color[ 1 ] );
         ++_len;
         if (_len < len)
         {
            const unsigned char mask_b = ftoi1_220( color[ 2 ] );
            ++_len;

            for (unsigned i = 0; _len < len && i < 6; ++i)
               if ((mask_a >> i) & 0x1) // RGB value?
                  _len += 3;
               else
                  ++_len;   // Paletted.

            for (unsigned i = 0; _len < len && i < 4; ++i)
               if ((mask_b >> i) & 0x1) // RGB value?
                  _len += 3;
               else
                  ++_len;   // Paletted.
         }
      }
      break;

   case 'v':
      // Every 3 bytes are 2 RGB values.
      _len += len - _len < 5 ? len - _len : 5;
      break;

   default:
      _len = 0;
      break;
   }

   return _len;
}

} // namespace yiffurc