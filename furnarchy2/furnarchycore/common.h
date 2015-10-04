#ifndef _FURNARCHYCORE_COMMON_H__
#define _FURNARCHYCORE_COMMON_H__

#include <cstddef>
#include <utility>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <cstring>
#include <cstdarg>
#include <sstream>
#include <ctime>
#include <iostream>
#include <cassert>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include "furn2.h"

#undef CDECL   // F-U, windef.h

#ifdef _MSC_VER
   #define STDCALL __stdcall
   #define CDECL __cdecl
   #define FASTCALL __fastcall
#else
   NOT SUPPORTED :(
#endif

/************************************************************************/
/* Common types, usings, etc.                                           */
/************************************************************************/

using std::string;
using std::time_t;
using std::size_t;
//using std::intptr_t;  // no?
using std::vector;
using std::list;
using std::ostringstream;
using std::istringstream;
using std::ifstream;
using std::ofstream;
using std::memcpy;
using std::memset;
using std::strlen;
using std::strcmp;
// using std::stricmp;

using furn2::POINT2;
using furn2::WNDPROCPARAMS;
using furn2::Client_Screen;
using furn2::UUID128;
using furn2::Connection_Level;
using furn2::Script_Error;
using furn2::Script_RunFlags;

using furn2::CONNLVL_DISCONNECTED; using furn2::CONNLVL_CONNECTED; 
using furn2::CONNLVL_MOTDEND; using furn2::CONNLVL_LOGGEDIN;

using furn2::SCREEN_BORDER; using furn2::SCREEN_MARBLED; using furn2::SCREEN_PICKEM;
using furn2::SCREEN_TITLE;

using furn2::SCRIPT_BADARG; using furn2::SCRIPT_BADSID; using furn2::SCRIPT_FAILED;
using furn2::SCRIPT_NOTUNIQUE; using furn2::SCRIPT_OK;

using furn2::SCRIPT_RUN_LATE; using furn2::SCRIPT_RUN_REPLACE; using furn2::SCRIPT_RUN_UNIQUE;

using furn2::MONSTER_HASPROFILE;

using furn2::HIDE_ALL; using furn2::HIDE_CLIENT; using furn2::HIDE_FILTERS;
using furn2::HIDE_MODULES; using furn2::HIDE_SCRIPTS; using furn2::HIDE_WORLD;

typedef vector<string> ArgList;

// Contrast with furn2::PLAYERINFO
struct PLAYERINFO
{
   string       name;
   string       pw;
   string       mac_id;
   string       desc;
   string       colors;
   unsigned int guid;
   POINT2       cam;

   void clear( )
   {
      name.clear( );
      pw.clear( );
      mac_id.clear( );
      desc.clear( );
      colors.clear( );
      guid = 0;
      cam.x = cam.y = 0;
   }
};

// Contrast with furn2::MONSTERINFO
struct MONSTERINFO
{
   unsigned guid;
   string   name;
   string   color;
   POINT2   pos;
   unsigned frame;
   unsigned flags;
   bool     visible;
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


/************************************************************************/
/*  Common Functions                                                    */
/************************************************************************/

static string& uuid_to_str( const UUID128& uuid, string* buf )
{
   static const char xdigits[] = { '0', '1', '2', '3', '4', '5', '6', '7',
                                   '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

   const unsigned char* b = uuid.bytes;
   buf->assign( "{" );

   for (size_t i = 0; i < 4; ++i, ++b)
   {
      buf->push_back( xdigits[ *b >> 4 ] );
      buf->push_back( xdigits[ *b & 0x0F ] );
   }

   buf->push_back( '-' );

   for (size_t i = 0; i < 2; ++i, ++b)
   {
      buf->push_back( xdigits[ *b >> 4 ] );
      buf->push_back( xdigits[ *b & 0x0F ] );
   }

   buf->push_back( '-' );

   for (size_t i = 0; i < 2; ++i, ++b)
   {
      buf->push_back( xdigits[ *b >> 4 ] );
      buf->push_back( xdigits[ *b & 0x0F ] );
   }

   buf->push_back( '-' );

   for (size_t i = 0; i < 2; ++i, ++b)
   {
      buf->push_back( xdigits[ *b >> 4 ] );
      buf->push_back( xdigits[ *b & 0x0F ] );
   }

   buf->push_back( '-' );

   for (size_t i = 0; i < 6; ++i, ++b)
   {
      buf->push_back( xdigits[ *b >> 4 ] );
      buf->push_back( xdigits[ *b & 0x0F ] );
   }

   buf->push_back( '}' );

   return *buf;
}

static size_t tok_line_args( const string& line, size_t off, string* out )
{
   out->clear( );
   
   const char* s = line.c_str( ) + off;
   char c;

   while ((c = *s) && c == ' ') ++s;

   if (!c) return line.length( );

   if (c == '\"' || c == '\'' || c == '`')   // quote-delimited
   {
      const char ch_delim = *(s++);
      bool escaping = false;

      while ((c = *s))
      {
         ++s;
         if (!escaping)
         {
            if (c == ch_delim)
               break;
            else if (c == '\\')
               escaping = true;
            else
               out->push_back( c );
         }
         else
         {
            escaping = false;

            if (c != ch_delim)
               out->push_back( '\\' );

            out->push_back( c );
         }
      }
   }
   else
   {
      bool escaping = false;

      while ((c = *s))
      {
         ++s;
         if (!escaping)
         {
            if (c == ' ')
               break;
            else if (c == '\\')
               escaping = true;
            else
               out->push_back( c );
         }
         else
         {
            escaping = false;

            if (c != ' ')
               out->push_back( '\\' );

            out->push_back( c );
         }
      }
   }

   return (size_t) s - (size_t) line.c_str( );
}

static size_t make_arg_list( const string& args, ArgList* arg_list )
{
   string arg;
   arg_list->clear( );
   size_t arg_off = 0;

   while (true)
   {
      arg_off = tok_line_args( args, arg_off, &arg );

      if (!arg.length( ))
         break;

      arg_list->push_back( arg );
   }

   return arg_list->size( );
}

inline string& to_lower( string* s )
{
   const char* c = s->c_str( );

   size_t off = 0;
   while (*c)
   {
      if (*c >= 'A' && *c <= 'Z')
         s->operator []( off ) = *c + 0x20;
      ++c;
      ++off;
   }

   return *s;
}

inline string& to_upper( string* s )
{
   const char* c = s->c_str( );

   size_t off = 0;
   while (*c)
   {
      if (*c >= 'a' && *c <= 'z')
         s->operator []( off ) = *c - 0x20;
      ++c;
      ++off;
   }

   return *s;
}

inline string break_html( const string& s )
{
   string r;
   for (string::const_iterator i = s.begin( ); i != s.end( ); ++i)
   {
      switch (*i)
      {
      case '<':
         r += "&lt;";
         break;
      case '>':
         r += "&gt;";
         break;
      default:
         r += *i;
      }
   }
   return r;
}

inline string break_ent_refs( const string& s )
{
   string r;
   for (string::const_iterator i = s.begin( ); i != s.end( ); ++i)
   {
      switch (*i)
      {
      case '&':
         r += "&#38;"; 
         break;
      default:
         r += *i;
      }
   }
   return r;
}

inline bool to_size_t( const string& s, size_t* o )
{
   const char* c = s.c_str( );

   size_t t = 0;
   while (*c && *c >= '0' && *c <= '9')
   {
      t = t * 10 + (*c - '0');
      ++c;
   }

   *o = t;

   if (!*c)
      return true;

   return false;
}

static bool to_size_t_pair( const string& s, std::pair<size_t,size_t>* o )
{
   const char* c = s.c_str( );

   size_t t = 0;
   while (*c && *c != '-' && *c >= '0' && *c <= '9')
   {
      t = t * 10 + (*c - '0');
      ++c;
   }

   o->first = t;

   if (*c != '-')
      return false;

   ++c;

   t = 0;
   while (*c && *c != '-' && *c >= '0' && *c <= '9')
   {
      t = t * 10 + (*c - '0');
      ++c;
   }

   o->second = t;

   if (!*c)
      return true;

   return false;
}

static string& escape_char( string* str, const char ch )
{
   string t;

   bool escaping = false;
   for (const char* sz = str->c_str( ); *sz; ++sz)
   {
      if (!escaping)
      {
         if (*sz == '\\')
         {
            escaping = true;
            t += '\\';
         }
         else if (*sz == ch)
         {
            t += '\\';
            t += ch;
         }
         else
            t += *sz;
      }
      else // escaping
      {
         escaping = false;
         t += *sz;
      }
   }
   return *str = t;
}

static string& pipes2spaces( string* str )
{
   for (size_t i = 0; i < str->length( ); i++)
      if (str->operator []( i ) == '|')
         str->operator []( i ) = ' ';

   return *str;
}

static string& spaces2pipes( string* str )
{
   for (size_t i = 0; i < str->length( ); i++)
      if (str->operator []( i ) == ' ')
         str->operator []( i ) = '|';

   return *str;
}

inline string& strip_lines( string* str )
{
   /* Convert CR and LF characters to spaces. */
   for (string::iterator i = str->begin( ); i != str->end( ); ++i)
      if (*i == '\n' || *i == '\r')
         *i = ' ';

   return *str;
}

/** Removes case, whitespace, and pipes.
*   Maybe one day, if you wish hard enough, this will resolve latin-1 
*   to its ascii equivalents.
*   \warning name should != out */
static string& reduce_player_name( const string& name, string* out,
                                   size_t max = -1 )
{
   out->clear( );
   const char* s = name.c_str( );
   char c;
   size_t len = 0;
   while ((c = *(s++)) && len < max)
   {
      switch (c)
      {
      case ' ':
      case '|':
         break;
      case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
      case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
      case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
      case 'V': case 'W': case 'X': case 'Y': case 'Z':
         out->push_back( c + 0x20 );
         ++len;
         break;
      default:         
         out->push_back( c );
         ++len;
         break;
      }
   }

   return *out;
}

/** Resolves {n} {{n}} and {{{n}}} patterns. */
string& resolve_braces( string* s );

static size_t read_file_lines( const char* file, vector<string>* lines,
                               bool trim = true )
{
   lines->clear( );

   std::ifstream fs( file );

   if (!fs.fail( ))
   {
      string line;

      while (!fs.eof( ))
      {
         getline( fs, line );
         
         if (trim)
         {
            size_t n = line.find_last_not_of( " \t" );
            if (n != line.npos)
               line.resize( n + 1 );

            n = line.find_first_not_of( " \t" );
            if (n != line.npos)
               line.erase( 0, n );
         }

         if (line.length( ))
            lines->push_back( line );
      }
      
      fs.close( );
   }

   return lines->size( );   
}

inline int stricmp( const string& a, const string& b )
{
   return _stricmp( a.c_str( ), b.c_str( ) );
}

#endif