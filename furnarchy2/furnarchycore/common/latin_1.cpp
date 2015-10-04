#include <cstddef>
#include "latin_1.h"

int latin_1::expand_symbols( const char* cp_in, int in_len, 
                             char* p_out /*=0*/, int out_len /*=0*/ )
{
   if (p_out == NULL || out_len == 0)      //Just calculate length.
   {
      out_len = 0;
      for (int i=0; i < in_len; i++, cp_in++)
         out_len += latin_1::latin_name_lens_special[ (unsigned char)*cp_in ];
      return out_len;
   }

   //Expand.

   int                  curr_len = 0;
   int                  latin_end;
   const char*          cp_latin_name;
   unsigned char        c;
   const char*          cp_end = cp_in + in_len;

   while (curr_len <= out_len && cp_end != cp_in)
   {
      c = *cp_in++;
      cp_latin_name = latin_1::latin_names_special[ c ];
      latin_end = curr_len + latin_1::latin_name_lens_special[ c ];

      if (latin_end <= out_len)
      {
         for (; curr_len < latin_end; curr_len++, p_out++, cp_latin_name++)
            *p_out = *cp_latin_name;
      }
      else 
         break;    //Means you won't see fragmented names.
   }

   return curr_len;
}

int latin_1::resolve_names( const char* cp_in, int in_len,
                            char* p_out /*=0*/, int out_len /*=0*/ )
{   
   if (p_out == NULL || out_len == 0)      //Just calculate length.
   {
      out_len = 0;
      int         j;
      const char* cp_sc;
      int         src_len;

      for (int i=0; i < in_len; i++, cp_in++, out_len++)
      {
         //Check if it could be a named latin1 character.
         if (*cp_in == '&' && (in_len - i) >= latin_1::SHORTEST_LATIN_NAME)
         {
            cp_sc = cp_in;
            for (j=i; j < in_len && *cp_sc != ';'; j++, cp_sc++);

            if (*cp_sc == ';')
            {
               src_len = (std::size_t)cp_sc - (std::size_t)cp_in + 1;

               if (src_len <= latin_1::LONGEST_LATIN_NAME)
               {
                  if (-1 != latin_1::find_sorted_name( cp_in, src_len ))
                  {
                     --src_len;
                     i += src_len;
                     cp_in += src_len;
                  }
               }
            }
         }
      }

      return out_len;
   }
   //Resolve.

   int         j;
   const char* cp_sc;
   int         src_len;
   int         name_index;
   char*       p_out_end = p_out + out_len + 1;
   char*       p_out_pos = p_out;

   for (int i=0; i < in_len && p_out_pos != p_out_end; i++, cp_in++, p_out_pos++)
   {
      if (*cp_in == '&' && (in_len - i) >= latin_1::SHORTEST_LATIN_NAME)
      {
         cp_sc = cp_in;
         for (j=i; j < in_len && *cp_sc != ';'; j++, cp_sc++);

         if (*cp_sc == ';')
         {
            src_len = (std::size_t)cp_sc - (std::size_t)cp_in + 1;

            if (src_len <= latin_1::LONGEST_LATIN_NAME)
            {
               if (-1 != (name_index = latin_1::find_sorted_name( cp_in, src_len )))
               {
                  --src_len;
                  i += src_len;
                  cp_in += src_len;
                  *p_out_pos = latin_1::latin_values_sorted[ name_index ];
               }
               else
                  *p_out_pos = *cp_in;
            }
            else
               *p_out_pos = *cp_in;
         }
         else
            *p_out_pos = *cp_in;
      }
      else
         *p_out_pos = *cp_in;
   }

   return (std::size_t)p_out_pos - (std::size_t)p_out;
}

int latin_1::find_sorted_name( const char* cp_in, int in_len )
{
   int         mid, begin, end;
   const char* cp_name;
   int         name_len;
   int         j;
   int         cmp_res = -1;

   static const int _NUM_SORTED_NAMES = sizeof( latin_1::latin_names_sorted ) / 
                                        sizeof( *latin_1::latin_names_sorted );
   mid = _NUM_SORTED_NAMES >> 1;
   begin = 0;  end = _NUM_SORTED_NAMES;

   while (end - begin)
   {
      cp_name = latin_1::latin_names_sorted[ mid ];
      name_len = latin_1::latin_name_lens_sorted[ mid ];

      if (in_len < name_len)
      {
         end = mid;
         mid = begin + ((end - begin) >> 1);
      }
      else if (in_len > name_len)
      {
         begin = mid + 1;
         mid = begin + ((end - begin) >> 1);
      }
      else
      {
         for (j=1; j < in_len && !(cmp_res = cp_in[j] - cp_name[j]); j++); //j=1 -> skip '&'

         if (cmp_res < 0)
         {
            end = mid;
            mid = begin + ((end - begin) >> 1);
         }
         else if (cmp_res > 0)
         {
            begin = mid + 1;
            mid = begin + ((end - begin) >> 1);
         }
         else  //you win!
            break;
      }
   }

   if (end - begin)
      return mid;

   return -1;
}

//No &quot; or &amp;
const int      latin_1::latin_name_lens_special[256] = //256
{
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 4, 1, 
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
//	  1,
      6, 7, 6, 7, 8, 5, 8, 6, 5, 6, 6, 7, 5, 5, 5, 6, 
      5, 8, 6, 6, 7, 7, 6, 8, 7, 6, 6, 7, 8, 8, 8, 8, 
      8, 8, 7, 8, 6, 7, 7, 8, 8, 8, 7, 6, 8, 8, 7, 6, 
      5, 8, 8, 8, 7, 8, 6, 7, 8, 8, 8, 7, 6, 8, 7, 7, 
      8, 8, 7, 8, 6, 7, 7, 8, 8, 8, 7, 6, 8, 8, 7, 6, 
      5, 8, 8, 8, 7, 8, 6, 8, 8, 8, 8, 7, 6, 8, 7, 6, 
//	  6,
};

//No &quot; or &amp;
const char* const latin_1::latin_names_special[256] =
{
   "\x0", "\x1", "\x2", "\x3", "\x4", "\x5", "\x6", "\x7", 
      "\x8", "\x9", "\xa", "\xb", "\xc", "\xd", "\xe", "\xf", 
      "\x10", "\x11", "\x12", "\x13", "\x14", "\x15", "\x16", "\x17", 
      "\x18", "\x19", "\x1a", "\x1b", "\x1c", "\x1d", "\x1e", "\x1f", 
      "\x20", "\x21", "\x22", "\x23", "\x24", "\x25", "\x26", "\x27", 
      "\x28", "\x29", "\x2a", "\x2b", "\x2c", "\x2d", "\x2e", "\x2f", 
      "\x30", "\x31", "\x32", "\x33", "\x34", "\x35", "\x36", "\x37", 
      "\x38", "\x39", "\x3a", "\x3b", "&lt;", "\x3d", "&gt;", "\x3f", 
      "\x40", "\x41", "\x42", "\x43", "\x44", "\x45", "\x46", "\x47", 
      "\x48", "\x49", "\x4a", "\x4b", "\x4c", "\x4d", "\x4e", "\x4f", 
      "\x50", "\x51", "\x52", "\x53", "\x54", "\x55", "\x56", "\x57", 
      "\x58", "\x59", "\x5a", "\x5b", "\x5c", "\x5d", "\x5e", "\x5f", 
      "\x60", "\x61", "\x62", "\x63", "\x64", "\x65", "\x66", "\x67", 
      "\x68", "\x69", "\x6a", "\x6b", "\x6c", "\x6d", "\x6e", "\x6f", 
      "\x70", "\x71", "\x72", "\x73", "\x74", "\x75", "\x76", "\x77", 
      "\x78", "\x79", "\x7a", "\x7b", "\x7c", "\x7d", "\x7e", "\x7f", 
      "\x80", "\x81", "\x82", "\x83", "\x84", "\x85", "\x86", "\x87", 
      "\x88", "\x89", "\x8a", "\x8b", "\x8c", "\x8d", "\x8e", "\x8f", 
      "\x90", "\x91", "\x92", "\x93", "\x94", "\x95", "\x96", "\x97", 
      "\x98", "\x99", "\x9a", "\x9b", "\x9c", "\x9d", "\x9e", "\x9f",
	 // "\x94", //Furcadia mapping mistake patch
      "&nbsp;", "&iexcl;", "&cent;", "&pound;", "&curren;", "&yen;", "&brvbar;", "&sect;", 
      "&uml;", "&copy;", "&ordf;", "&laquo;", "&not;", "&shy;", "&reg;", "&macr;", 
      "&deg;", "&plusmn;", "&sup2;", "&sup3;", "&acute;", "&micro;", "&para;", "&middot;", 
      "&cedil;", "&sup1;", "&ordm;", "&raquo;", "&frac14;", "&frac12;", "&frac34;", "&iquest;", 
      "&Agrave;", "&Aacute;", "&Acirc;", "&Atilde;", "&Auml;", "&Aring;", "&AElig;", "&Ccedil;", 
      "&Egrave;", "&Eacute;", "&Ecirc;", "&Euml;", "&Igrave;", "&Iacute;", "&Icirc;", "&Iuml;", 
      "&ETH;", "&Ntilde;", "&Ograve;", "&Oacute;", "&Ocirc;", "&Otilde;", "&Ouml;", "&times;", 
      "&Oslash;", "&Ugrave;", "&Uacute;", "&Ucirc;", "&Uuml;", "&Yacute;", "&THORN;", "&szlig;", 
      "&agrave;", "&aacute;", "&acirc;", "&atilde;", "&auml;", "&aring;", "&aelig;", "&ccedil;", 
      "&egrave;", "&eacute;", "&ecirc;", "&euml;", "&igrave;", "&iacute;", "&icirc;", "&iuml;", 
      "&eth;", "&ntilde;", "&ograve;", "&oacute;", "&ocirc;", "&otilde;", "&ouml;", "&divide;", 
      "&oslash;", "&ugrave;", "&uacute;", "&ucirc;", "&uuml;", "&yacute;", "&thorn;", "&yuml;",
	 // "&oric;", //Furcadia mapping mistake patch
};

const int latin_1::latin_name_lens_sorted[100] = //100
{
   4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 
      6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,// 6,
      6, 6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
      7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 
      8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
      8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
      8, 8, 8, 8, 
};

const char* const latin_1::latin_names_sorted[100] = //100
{
   "&gt;", "&lt;", "&ETH;", "&amp;", "&deg;", "&eth;", "&not;", "&reg;", 
      "&shy;", "&uml;", "&yen;", "&Auml;", "&Euml;", "&Iuml;", "&Ouml;", "&Uuml;", 
      "&auml;", "&cent;", "&copy;", "&euml;", "&iuml;", "&macr;", "&nbsp;", "&oirc;",/*24*/ "&ordf;", 
      "&ordm;", "&ouml;", "&para;", "&quot;", "&sect;", "&sup1;", "&sup2;", "&sup3;", 
      "&uuml;", "&yuml;", "&AElig;", "&Acirc;", "&Aring;", "&Ecirc;", "&Icirc;", "&Ocirc;", 
      "&THORN;", "&Ucirc;", "&acirc;", "&acute;", "&aelig;", "&aring;", "&cedil;", "&ecirc;", 
      "&icirc;", "&iexcl;", "&laquo;", "&micro;", /*"&ocirc;" 54,*/ "&pound;", "&raquo;", "&szlig;", 
      "&thorn;", "&times;", "&ucirc;", "&Aacute;", "&Agrave;", "&Atilde;", "&Ccedil;", "&Eacute;", 
      "&Egrave;", "&Iacute;", "&Igrave;", "&Ntilde;", "&Oacute;", "&Ograve;", "&Oslash;", "&Otilde;", 
      "&Uacute;", "&Ugrave;", "&Yacute;", "&aacute;", "&agrave;", "&atilde;", "&brvbar;", "&ccedil;", 
      "&curren;", "&divide;", "&eacute;", "&egrave;", "&frac12;", "&frac14;", "&frac34;", "&iacute;", 
      "&igrave;", "&iquest;", "&middot;", "&ntilde;", "&oacute;", "&ograve;", "&oslash;", "&otilde;", 
      "&plusmn;", "&uacute;", "&ugrave;", "&yacute;", 
};

const char latin_1::latin_values_sorted[100] = //100  (24) (54)
{
   62, 60, -48, 38, -80, -16, -84, -82, -83, -88, -91, -60, -53, -49, -42, -36, 
      -28, -94, -87, -21, -17, -81, -96/*, -12 24*/, -86, -70, -10, -74, 34, -89, -71, -78, -77, 
      -4, -1, -58, -62, -59, -54, -50, -44, -34, -37, -30, -76, -26, -27, -72, -22, 
      -18, -95, -85, -75, -12/*54*/, -93, -69, -33, -2, -41, -5, -63, -64, -61, -57, -55, 
      -56, -51, -52, -47, -45, -46, -40, -43, -38, -39, -35, -31, -32, -29, -90, -25, 
      -92, -9, -23, -24, -67, -68, -66, -19, -20, -65, -73, -15, -13, -14, -8, -11, 
      -79, -6, -7, -3, 
};

