#ifndef _CLI_UTIL_H__
#define _CLI_UTIL_H__

#include <string>
#include <vector>
#include <vcclr.h>
#include "common/xml_charset.h"

private ref class CliUtil abstract
{
internal:
   static std::string cliStrToAscii( System::String^ clrstr )
   {
      using namespace System;
      using namespace System::Runtime::InteropServices;

      cli::pin_ptr<const wchar_t> pinned = PtrToStringChars( clrstr );
      const wchar_t* p = pinned;
      std::wstring wstr( p, p + clrstr->Length );
      pinned = nullptr;
      return xml_charset::uni_to_ascii( wstr );
   }

   static std::string cliStrToMultiByteStr( System::String^ clrstr )
   {
      using namespace System;
      using namespace System::Runtime::InteropServices;

      cli::pin_ptr<const wchar_t> pinned = PtrToStringChars( clrstr );
      const wchar_t* p = pinned;
      std::string str( clrstr->Length, '\0' );
      for (std::string::iterator i = str.begin( ); i != str.end( ); ++i, ++p)
         *i = (unsigned char) *(unsigned short*) p;
      pinned = nullptr;
      return str;
   }

   static cli::array<System::Byte>^ charsToCliBytes( const char* bytes, std::size_t len )
   {
      using namespace System;
      array<Byte>^ r = gcnew array<Byte>( len );
      if (len)
      {
         interior_ptr<Byte> p = &(r[ 0 ]);
         for (const char *i = bytes, *end = bytes + len; i != end; ++i, ++p)
            *p = *i;
      }
      return r;
   }

   static std::vector<char> cliBytesToChars( cli::array<System::Byte>^ bytes )
   {
      using namespace System;
      std::vector<char> r( bytes->Length, '\0' );
      if (r.size( ))
      {
         interior_ptr<Byte> p = &(bytes[ 0 ]);
         for (std::vector<char>::iterator i = r.begin( ); i != r.end( ); ++i, ++p)
            *i = *p;
      }
      return r;
   }

   // Doesn't include the terminating null byte.
   static cli::array<System::Byte>^ multiByteStrToCliBytes( const std::string& str )
   {
      using namespace System;
      array<Byte>^ r = gcnew array<Byte>( str.length( ) );
      if (str.length( ))
      {
         interior_ptr<Byte> p = &(r[ 0 ]);
         for (std::string::const_iterator i = str.begin( ); i != str.end( ); ++i, ++p)
            *p = *i;
      }
      return r;
   }

   // Adds a terminating null byte.
   static std::string CliBytesToMultiByteStr( cli::array<System::Byte>^ bytes )
   {
      using namespace System;
      std::string r( bytes->Length, '\0' );
      if (r.length( ))
      {
         interior_ptr<Byte> p = &(bytes[ 0 ]);
         for (std::string::iterator i = r.begin( ); i != r.end( ); ++i , ++p)
            *i = *p;
      }
      return r;
   }

   ref class Safe abstract
   {
   internal:
      static std::string cliStrToAscii( System::String^ clrstr )
      {
         using namespace System;
         using namespace System::Runtime::InteropServices;

         wchar_t* p = (wchar_t*) Marshal::StringToHGlobalUni( clrstr ).ToPointer( );
         std::wstring wstr( p, p + clrstr->Length );
         Marshal::FreeHGlobal( (IntPtr) p );
         return xml_charset::uni_to_ascii( wstr );
      }

      static std::string cliStrToMultiByte( System::String^ clrstr )
      {
         using namespace System;
         using namespace System::Runtime::InteropServices;

         wchar_t* p = (wchar_t*) Marshal::StringToHGlobalUni( clrstr ).ToPointer( );
         std::string str( clrstr->Length, '\0' );
         for (std::string::iterator i = str.begin( ); i != str.end( ); ++i, ++p)
            *i = (unsigned char) *(unsigned short*) p;
         Marshal::FreeHGlobal( (IntPtr) p );
         return str;
      }
   };
};

#endif