/************************************************************************/
/* Copyright (c) 2006 Cluracan (strangelove@heroinpuppy.com)            */
/*                                                                      */
/* The "MIT" License                                                    */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation       */
/* files (the "Software"), to deal in the Software without restriction, */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* - The above copyright notice and this permission notice shall be     */
/* included in all copies or substantial portions of the Software.      */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND             */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT          */
/* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,         */
/* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        */
/* DEALINGS IN THE SOFTWARE.                                            */
/************************************************************************/

#ifndef _XML_CHARSET_H__
#define _XML_CHARSET_H__

#include <string>

namespace xml_charset
{
   /** Converts unicode to latin-1.
   *  Unicode characters with no latin-1 equivalent will be converted
   *  to an XML entity name. */
   std::string uni_to_latin( const std::wstring& uni );
   /** Converts unicode to ASCII.
   *  Unicode characters with no ASCII equivalent will be converted
   *  to an XML entity name. */
   std::string uni_to_ascii( const std::wstring& uni );
   /** Converts latin-1 to unicode.
   *  XML entity names will be converted to their unicode equivalent. */
   std::wstring latin_to_uni( const std::string& latin );
   /** Converts ASCII to latin-1.
   *  XML entity names will be converted to their latin-1 equivalent. */
   std::string ascii_to_latin( const std::string& ascii );
   /** Converts latin-1 to ASCII.
   *  Latin-1 characters with no ASCII equivalent will be converted
   *  to an XML entity name. */
   std::string latin_to_ascii( const std::string& latin );
};

#endif