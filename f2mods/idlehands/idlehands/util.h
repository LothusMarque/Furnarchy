#ifndef _IDLEHANDS_UTIL_H__
#define _IDLEHANDS_UTIL_H__

#include <string>
#include <sstream>
#include <ctime>

namespace idlehands {
   namespace util {

      using namespace std;

      static string to_xml_friendly( const string& str )
      {
         string xml_str;
         for (size_t i = 0; i < str.length( ); ++i)
         {
            switch (str[ i ])
            {
            case '<':
               xml_str += "&lt;"; break;
            case '>':
               xml_str += "&gt;"; break;
            case '&':
               xml_str += "&amp;"; break;
            default:
               xml_str += str[ i ];
            }
         }
         return xml_str;
      }

      static string to_readable( const string& str )
      {
         string readable_str;
         for (size_t i = 0; i < str.length( ); ++i)
            readable_str += (unsigned) str[ i ] < ' ' ? ' ' : str[ i ];
         return readable_str;
      }

      static string tag_substitute( const string& str, time_t dt, 
                                    const string& player = "",
									const string& idlereason="")
      {
         ostringstream out;
         for (size_t i = 0; i < str.length( ); ++i)
         {
            if (str[ i ] == '%')
            {
               if (!str.compare( i, 2, "%d" ))
               { out << (dt / 60 / 60 / 24); ++i; }
               else if (!str.compare( i, 2, "%h" ))
               { out << (dt / 60 / 60) % 24; ++i; }
               else if (!str.compare( i, 2, "%H" ))
               { out << (dt / 60 / 60); ++i; }
               else if (!str.compare( i, 2, "%m" ))
               { out << (dt / 60) % 60; ++i; }
               else if (!str.compare( i, 2, "%M" ))
               { out << dt / 60; ++i; }
               else if (!str.compare( i, 2, "%s" ))
               { out << dt % 60; ++i; }
               else if (!str.compare( i, 2, "%S" ))
               { out << dt; ++i; }
               else if (!str.compare( i, 7, "%player" ))
               { out << player; i += 6; }
               else if (!str.compare( i, 8, "%idlemsg" )) //Idle Message!
               { out << idlereason; i += 7; }
			   else
                  out << '%';
            }
            else
               out << str[ i ];
         }
         return out.str( );
      }

      static string to_name( const string& str )
      {
         string name;
         for (size_t i = 0; i < str.length( ); ++i)
         {
            if (str[ i ] == ' ')
               name += '|';
            else
               name += str[ i ];
         }
         return name;
      }
      
      static string make_short_name( const string& name )
      {
         string short_name;
         for (size_t i = 0; i < name.length( ); ++i)
         {
            if (name[ i ] != ' ' && name[ i ] != '|')
               short_name += name[ i ];
         }
         return short_name;
      }

      static time_t to_time( const char* strs[], size_t count )
      {
         if (count > 4)
            count = 4;
         time_t r = 0;
         switch (count)
         {
         case 4:
            r += atol( strs[ 3 ] );
            r += atol( strs[ 2 ] ) * 60;
            r += atol( strs[ 1 ] ) * 60 * 60;
            r += atol( strs[ 0 ] ) * 60 * 60 * 24;
            break;
         case 3:
            r += atol( strs[ 2 ] );
            r += atol( strs[ 1 ] ) * 60;
            r += atol( strs[ 0 ] ) * 60 * 60;
            break;
         case 2:
            r += atol( strs[ 1 ] );
            r += atol( strs[ 0 ] ) * 60;
            break;
         case 1:
            r += atol( strs[ 0 ] );
            break;
         }
         return r;
      }

   } // namespace util
} // namespace idlehands

#endif