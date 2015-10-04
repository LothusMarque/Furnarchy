#ifndef _DIRSTACK_H__
#define _DIRSTACK_H__

#include <Windows.h>
#include <string>

/** Automatically restores the initial working directory
*   upon destruction. */
class DirStack
{
private:
   std::vector<std::string> dirs;
public:
   void push( const std::string& dir ) {
      dirs.push_back( dir );
      SetCurrentDirectoryA( dir.c_str( ) );
   }
   void pop( ) {
      if (dirs.size( ) > 1)
      {
         dirs.pop_back( );
         SetCurrentDirectoryA( dirs.back( ).c_str( ) );
      }
   }
   void reset( ) {
      SetCurrentDirectoryA( dirs.front( ).c_str( ) );
      dirs.resize( 1 );
   }
   DirStack( ) { 
      char buf[ MAX_PATH ];
      GetCurrentDirectoryA( MAX_PATH, buf );
      dirs.push_back( buf );
   }
   ~DirStack( ) {
      SetCurrentDirectoryA( dirs.front( ).c_str( ) );
   }
};

#endif