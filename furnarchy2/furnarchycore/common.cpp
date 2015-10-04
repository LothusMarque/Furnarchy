#include "common.h"
#include "yiffurc/common.h"

string& resolve_braces( string* s )
{
   const string t = *s;
   const char* p = t.c_str( );
   char c;
   s->clear( );

   while ((c = *p))
   {
      if (c == '{')  // Start of a tag?
      {
         if (p[ 1 ] == '{')
         {
            if (p[ 2 ] == '{')   // 3-digit fnum.
            {
               const char* p2 = strchr( p + 3, '}' );
               if (p2 && p2[ 1 ] == '}' && p2[ 2 ] == '}')
               {
                  if (string( p + 3, p2 ).find_first_not_of( "0123456789" ) == string::npos)
                  {
                     char t[ 3 ];
                     yiffurc::itof3( atol( p + 3 ), t );
                     s->append( t, 3 );
                     p = p2 + 3;
                     continue;
                  }
               }
            }
            else  // 2-digit fnum.
            {
               const char* p2 = strchr( p + 2, '}' );
               if (p2 && p2[ 1 ] == '}')
               {
                  if (string( p + 2, p2 ).find_first_not_of( "0123456789" ) == string::npos)
                  {
                     char t[ 2 ];
                     yiffurc::itof2( atol( p + 2 ), t );
                     s->append( t, 2 );
                     p = p2 + 2;
                     continue;
                  }
               }
            }
         }
         else  
         {
            const char* p2 = strchr( p + 1, '}' );
            if (p2)
            {
               if ((size_t)p2 - (size_t)p == 2 && p[ 1 ] == 'n')
               {
                  // Linebreak.
                  *s += '\n';
                  p = p2 + 1;
                  continue;
               }
               else  // 1-digit fnum.
               {
                  if (string( p + 1, p2 ).find_first_not_of( "0123456789" ) == string::npos)
                  {
                     s->push_back( yiffurc::itof1( atol( p + 1 ) ) );
                     p = p2 + 1;
                     continue;
                  }
               }
            }
         }
      } // if (c == '{')

      /* base 220 tags. */
      if (c == '[')  // Start of a tag?
      {
         if (p[ 1 ] == '[')
         {
            if (p[ 2 ] == '[') 
            {
               if (p[ 3 ] == '[')   // 4-digit fnum.
               {
                  const char* p2 = strchr( p + 4, ']' );
                  if (p2 && p2[ 1 ] == ']' && p2[ 2 ] == ']' && p2[ 3 ] ==']')
                  {
                     if (string( p + 4, p2 ).find_first_not_of( "0123456789" ) == string::npos)
                     {
                        char t[ 4 ];
                        yiffurc::itof4_220( atol( p + 4 ), t );
                        s->append( t, 4 );
                        p = p2 + 4;
                        continue;
                     }
                  }
               }
               else // 3-digit fnum.
               {
                  const char* p2 = strchr( p + 3, ']' );
                  if (p2 && p2[ 1 ] == ']' && p2[ 2 ] == ']')
                  {
                     if (string( p + 3, p2 ).find_first_not_of( "0123456789" ) == string::npos)
                     {
                        char t[ 3 ];
                        yiffurc::itof3_220( atol( p + 3 ), t );
                        s->append( t, 3 );
                        p = p2 + 3;
                        continue;
                     }
                  }
               }
            }
            else  // 2-digit fnum.
            {
               const char* p2 = strchr( p + 2, ']' );
               if (p2 && p2[ 1 ] == ']')
               {
                  if (string( p + 2, p2 ).find_first_not_of( "0123456789" ) == string::npos)
                  {
                     char t[ 2 ];
                     yiffurc::itof2_220( atol( p + 3 ), t );
                     s->append( t, 2 );
                     p = p2 + 2;
                     continue;
                  }
               }
            }
         }
         else  
         {
            const char* p2 = strchr( p + 1, ']' );
            if (p2)
            {
               if ((size_t)p2 - (size_t)p == 2 && p[ 1 ] == 'n')
               {
                  // Linebreak.
                  *s += '\n';
                  p = p2 + 1;
                  continue;
               }
               else  // 1-digit fnum.
               {
                  if (string( p + 1, p2 ).find_first_not_of( "0123456789" ) == string::npos)
                  {
                     s->push_back( yiffurc::itof1_220( atol( p + 1 ) ) );
                     p = p2 + 1;
                     continue;
                  }
               }
            }
         }
      } // if (c == '{')


      *s += *(p++);
   } // while ((c = *p))

   return *s;
}