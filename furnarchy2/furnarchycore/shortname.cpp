#include "shortname.h"
#include "common/xml_charset.h"


namespace shortname 
{

const char g_lut[ 256 ] =
{
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',  
   '8',  '9',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  
   'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',  
   'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  
   'x',  'y',  'z',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  'a',  'b',  'c',  'd',  'e',  'f',  'g',  
   'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',  
   'p',  'q',  'r',  's',  't',  'u',  'v',  'w',  
   'x',  'y',  'z',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  '\0',  
   'a',  'a',  'a',  'a',  'a',  'a',  'a',  'c',  
   'e',  'e',  'e',  'e',  'i',  'i',  'i',  'i',  
   'd',  'n',  'o',  'o',  'o',  'o',  'o',  '\0',  
   'o',  'u',  'u',  'u',  'u',  'y',  '\0',  '\0',  
   'a',  'a',  'a',  'a',  'a',  'a',  'a',  'c',  
   'e',  'e',  'e',  'e',  'i',  'i',  'i',  'i',  
   'o',  'n',  'o',  'o',  'o',  'o',  'o',  '\0',  
   'o',  'u',  'u',  'u',  'u',  'y',  '\0',  'y'
};

std::string make_shortname( const std::string& name )
{
   // In furcadia, the &amp; reference doesn't exist, so escape that.
   std::string _name;
   _name.reserve( name.length( ) );
   for (std::string::size_type i = 0; i < name.length( ); ++i)
   {
      if (!name.compare( i, 5, "&amp;" ))
      {
         _name.append( "&amp;amp;" );
         i += 4;
      }
      else
         _name.push_back( name[ i ] );
   }

   // Expand entity references.
   std::wstring uni = xml_charset::latin_to_uni( _name );

   // Convert and strip characters to make a short name.
   std::string shortname;
   unsigned short c = 0;
   for (std::wstring::const_iterator i = uni.begin( ); i != uni.end( ); ++i)
   {
      c = *i;
      if (c < 256 && g_lut[ c ])
         shortname += g_lut[ c ];
   }

   return shortname;
}

} // namespace shortname