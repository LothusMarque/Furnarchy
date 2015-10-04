#ifndef _FSML_COMMON_H__
#define _FSML_COMMON_H__

#include <string>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <utility>
#include <cassert>
#include "common/ref_ptr.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

typedef unsigned int uint;
typedef unsigned long ulong;

using std::string;
using std::time_t;
using std::size_t;
using std::memcpy;
using std::memset;
using std::strlen;
using std::strcmp;
using std::strcpy;
using std::vector;
using std::exception;
using std::logic_error;
using std::runtime_error;

struct FSML_POS
{
   int x, y;

   inline FSML_POS( int x = 0, int y = 0 ) : x( x ), y( y ) { }
   inline bool operator ==( const FSML_POS& r ) const
   {
      return (x == r.x && y == r.y);
   }
   inline bool operator !=( const FSML_POS& r ) const
   {
      return (x != r.x || y != r.y);
   }

};

struct FSML_SIZE
{
   unsigned int w, h;
   
   inline FSML_SIZE( unsigned int w = 0, unsigned int h = 0 ) : w( w ), h( h ) { }
};

struct FSML_RECT
{
   int x, y;
   unsigned int w, h;

   inline FSML_RECT( int x = 0, int y = 0, 
                     unsigned int w = 0, unsigned int h = 0 )
                     : x( x ), y( y ), w( w ), h( h ) { }
};

struct FSML_COLOR
{
   unsigned char r;
   unsigned char g;
   unsigned char b;
   unsigned char a;

   inline FSML_COLOR( unsigned char r = 255, unsigned char g = 255, 
                      unsigned char b = 255, unsigned char a = 255 )
                      : r( r ), g( g ), b( b ), a( a ) { }
};

/** Automatically restores the initial working directory
*   upon destruction. */
class DirStack
{
private:
   std::vector<std::string> dirs;
public:
   void push( const std::string& dir ) {
      dirs.push_back( dir );
      SetCurrentDirectory( dir.c_str( ) );
   }
   void pop( ) {
      if (dirs.size( ) > 1)
      {
         dirs.pop_back( );
         SetCurrentDirectory( dirs.back( ).c_str( ) );
      }
   }
   void reset( ) {
      SetCurrentDirectory( dirs.front( ).c_str( ) );
      dirs.resize( 1 );
   }
   DirStack( ) { 
      char buf[ MAX_PATH ];
      GetCurrentDirectory( MAX_PATH, buf );
      dirs.push_back( buf );
   }
   ~DirStack( ) {
      SetCurrentDirectory( dirs.front( ).c_str( ) );
   }
};

inline int fsml_strtoi( const string& str )
{
   return (int) std::atol( str.c_str( ) );
}

inline float fsml_strtof( const string& str )
{
   return (float) std::atof( str.c_str( ) );
}

inline string& fsml_itostr( string* str, int i )
{
   char sz[ 32 ];
   sz[ 0 ] = '0'; sz[ 1 ] = '\0';
   std::sprintf( sz, "%i", i );
   str->assign( sz );
   return *str;
}

inline string& fsml_ftostr( string* str, float f )
{
   char sz[ 32 ];
   sz[ 0 ] = '0'; sz[ 1 ] = '.'; sz[ 2 ] = '0'; sz[ 3 ] ='\0';
   std::sprintf( sz, "%f", f );
   str->assign( sz );
   return *str;
}

static bool fsml_rect_intersect( FSML_RECT* out,
                                 const FSML_RECT& a, const FSML_RECT& b )
{
   if (a.x > b.x)
   {
      if (a.x + (signed) a.w < b.x + (signed) b.w)
      {
         out->w = a.x + a.w - a.x;
         out->x = a.x;
      }
      else
      {
         out->w = b.x + (signed) b.w > a.x ? b.x + b.w - a.x : 0;
         out->x = a.x;
      }
   }
   else
   {
      if (a.x + (signed) a.w < b.x + (signed) b.w)
      {
         out->w = a.x + (signed) a.w > b.x ? a.x + a.w - b.x : 0;
         out->x = b.x;
      }
      else
      {
         out->w = b.x + b.w - b.x;
         out->x = b.x;
      }
   }

   if (a.y > b.y)
   {
      if (a.y + (signed) a.h < b.y + (signed) b.h)
      {
         out->h = a.y + a.h - a.y;
         out->y = a.y;
      }
      else
      {
         out->h = b.y + (signed) b.h > a.y ? b.y + b.h - a.y : 0;
         out->y = a.y;
      }
   }
   else
   {
      if (a.y + (signed) a.h < b.y + (signed) b.h)
      {
         out->h = a.y + (signed) a.h > b.y ? a.y + a.h - b.y : 0;
         out->y = b.y;
      }
      else
      {
         out->h = b.y + b.h - b.y;
         out->y = b.y;
      }
   }
   
   return (out->w && out->h);
}

static void fsml_rect_union( FSML_RECT* out, 
                             const FSML_RECT& a, const FSML_RECT& b )
{
   // Don't combine with a 0-dimension rect.
   if (!a.w || !a.h)
   {
      *out = b;
      return;
   }
   if (!b.w || !b.h)
   {
      *out = a;
      return;
   }

   if (a.x < b.x)
   {
      if (a.x + a.w > b.x + b.w)
      {
         out->w = ( a.x + a.w ) - a.x;
         out->x = a.x;
      }
      else
      {
         out->w = ( b.x + b.w ) - a.x;
         out->x = a.x;
      }
   }
   else
   {
      if (a.x + a.w > b.x + b.w)
      {
         out->w = ( a.x + a.w ) - b.x;
         out->x = b.x;
      }
      else
      {
         out->w = ( b.x + b.w ) - b.x;
         out->x = b.x;
      }
   }

   if (a.y < b.y)
   {
      if (a.y + a.h > b.y + b.h)
      {
         out->h = ( a.y + a.h ) - a.y;
         out->y = a.y;
      }
      else
      {
         out->h = ( b.y + b.h ) - a.y;
         out->y = a.y;
      }
   }
   else
   {
      if (a.y + a.h > b.y + b.h)
      {
         out->h = ( a.y + a.h ) - b.y;
         out->y = b.y;
      }
      else
      {
         out->h = ( b.y + b.h ) - b.y;
         out->y = b.y;
      }
   }
}

inline bool fsml_pt_in_rect( const FSML_POS& pt, const FSML_RECT& r )
{
   return ( (pt.x >= r.x && pt.x < r.x + (signed) r.w) && 
            (pt.y >= r.y && pt.y < r.y + (signed) r.h) );
}

inline bool fsml_is_full_path( const string& path )
{
   if (path.find_first_of( "\\\\" ) == 0 
       || path.find_first_of( ":\\" ) == 1)
      return true;

   return false;
}

#endif