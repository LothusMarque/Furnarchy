/************************************************************************/
/* Copyright (c) 2007 Cluracan (strangelove@heroinpuppy.com)            */
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

#if !defined( _YIFFURC_UTIL_HPP__ ) && defined( __cplusplus )
#define _YIFFURC_UTIL_HPP__

#include "common.h"

#include <vector>
#include <string>
#include <algorithm>
#ifdef _MSC_VER
#include <xutility>
#endif
#include <cassert>

/** \addtogroup cpp
* \{ */
/** \addtogroup util Miscellaneous Utilities
* \{ */

namespace yiffurc {

/** colorcode base class. */
class ColorCode
{
public:
   enum Encoding_Type { ENC_PAL, ENC_MIXED, ENC_RGB };

protected:
   ColorCode( Encoding_Type encoding ) : m_encoding( encoding ) { }

protected:
   Encoding_Type m_encoding;

public:
   Encoding_Type getEncoding( ) { return m_encoding; }
   virtual std::string str( ) const = 0;
};


/** Paletted colorcode class. */
class ColorCodePal : public ColorCode
{
public:
   enum
   {
      COLORS_LEN = 10, GSD_LEN = 3
   };
   enum 
   { 
      COLOR_FUR = 0, COLOR_MARKINGS, COLOR_HAIR, COLOR_EYES, COLOR_BADGE, COLOR_VEST,
      COLOR_BRACERS, COLOR_CAPE, COLOR_BOOTS, COLOR_TROUSERS
   };
   enum
   {
      GSD_GENDER = 0, GSD_SPECIES, GSD_DIGO
   };
   enum
   {
      SPECIES_RODENT = 0, SPECIES_EQUINE, SPECIES_FELINE, SPECIES_CANINE, 
      SPECIES_MUSTELINE, SPECIES_LAPINE, SPECIES_SQUIRINE, SPECIES_BOVINE, SPECIES_URSINE,
	  SPECIES_BUGGE,
      COUNT_SPECIES
   };
   enum
   {
      GENDER_FEMALE = 0, GENDER_MALE, GENDER_UNSPEC, COUNT_GENDER
   };
   enum 
   { 
      COUNT_FUR = 25, COUNT_MARKINGS = 25, COUNT_HAIR = 45,
      COUNT_EYES = 30, COUNT_BADGE = 40, COUNT_VEST = 30,
      COUNT_BRACERS = 30, COUNT_CAPE = 30, COUNT_BOOTS = 30,
      COUNT_TROUSERS = 30, COUNT_DIGO = 1
   };

protected:

   unsigned      m_colors[ COLORS_LEN ];
   unsigned      m_gsd[ GSD_LEN ];

private:

   void checkRanges( )
   {
      // Make sure everything is within range.
      m_colors[ COLOR_FUR ] %= COUNT_FUR;
      m_colors[ COLOR_MARKINGS ] %= COUNT_MARKINGS;
      m_colors[ COLOR_HAIR ] %= COUNT_HAIR;
      m_colors[ COLOR_EYES ] %= COUNT_EYES;
      m_colors[ COLOR_BADGE ] %= COUNT_BADGE;
      m_colors[ COLOR_VEST ] %= COUNT_VEST;
      m_colors[ COLOR_BRACERS ] %= COUNT_BRACERS;
      m_colors[ COLOR_CAPE ] %= COUNT_CAPE;
      m_colors[ COLOR_BOOTS ] %= COUNT_BOOTS;
      m_colors[ COLOR_TROUSERS ] %= COUNT_TROUSERS;
      m_gsd[ GSD_GENDER ] %= COUNT_GENDER;
      m_gsd[ GSD_SPECIES ] %= COUNT_SPECIES;
      m_gsd[ GSD_DIGO ] %= COUNT_DIGO;
   }

public:
   void setColor( unsigned index, unsigned c )
   {
      assert( index < COLORS_LEN );
      switch (index)
      {
      case COLOR_FUR:
         m_colors[ index ] = c % COUNT_FUR; break;
      case COLOR_MARKINGS:
         m_colors[ index ] = c % COUNT_MARKINGS; break;
      case COLOR_HAIR:
         m_colors[ index ] = c % COUNT_HAIR; break;
      case COLOR_EYES:
         m_colors[ index ] = c % COUNT_EYES; break;
      case COLOR_BADGE:
         m_colors[ index ] = c % COUNT_BADGE; break;
      case COLOR_VEST:
         m_colors[ index ] = c % COUNT_VEST; break;
      case COLOR_BRACERS:
         m_colors[ index ] = c % COUNT_BRACERS; break;
      case COLOR_CAPE:
         m_colors[ index ] = c % COUNT_CAPE; break;
      case COLOR_BOOTS:
         m_colors[ index ] = c % COUNT_BOOTS; break;
      case COLOR_TROUSERS:
         m_colors[ index ] = c % COUNT_TROUSERS; break;
      }
   }
   const unsigned getColor( unsigned index ) const 
   {
      assert( index < COLORS_LEN );
      return m_colors[ index ];
   }

   void setGsd( unsigned index, unsigned c )
   {
      assert( index < GSD_LEN );
      switch (index)
      {
      case GSD_GENDER:
         m_gsd[ index ] = c % COUNT_GENDER; break;
      case GSD_SPECIES:
         m_gsd[ index ] = c % COUNT_SPECIES; break;
      case GSD_DIGO:
         m_gsd[ index ] = c % COUNT_DIGO; break;
      }
   }
   const unsigned getGsd( unsigned index ) const 
   {
      assert( index < GSD_LEN );
      return m_gsd[ index ];
   }

   ColorCodePal& operator =( const ColorCodePal& code ) 
   {
      std::copy( code.m_colors, code.m_colors + COLORS_LEN, m_colors );
      std::copy( code.m_gsd, code.m_gsd + GSD_LEN, m_gsd );
      return *this;
   }

   ColorCodePal& operator =( const std::string& code )
   {
      using namespace std; // To get over windows.h #defining min max
      size_t count = min( (string::size_type) COLORS_LEN, code.length( ) );
      for (size_t i = 0; i < count; ++i)
         m_colors[ i ] = ftoi1_220( code[ i ] );

      if (code.length( ) > COLORS_LEN)
      {
         count = min( (string::size_type) GSD_LEN, code.length( ) - COLORS_LEN );
         for (size_t i = 0; i < count; ++i)
            m_gsd[ i ] = ftoi1_220( code[ COLORS_LEN + i ] );
      }

      checkRanges( );
      return *this;
   }

   ColorCodePal& operator =( const char* code )
   {
      for (std::size_t i = 0; i < COLORS_LEN && *code; ++code, ++i)
         m_colors[ i ] = ftoi1_220( *code );

      for (std::size_t i = 0; i < GSD_LEN && *code; ++code, ++i)
         m_gsd[ i ] = ftoi1_220( *code );

      checkRanges( );
      return *this;
   }

   template <typename T_iter>
   ColorCodePal& assign( T_iter first, const T_iter& last )
   {
      for (std::size_t i = 0; i < COLORS_LEN && first != last; ++first, ++i)
         m_colors[ i ] = ftoi1_220( *first );

      for (std::size_t i = 0; i < GSD_LEN && first != last; ++first, ++i)
         m_gsd[ i ] = ftoi1_220( *first );

      checkRanges( );
      return *this;
   }

   template <typename T_iter>
   ColorCodePal& assignColor( T_iter first, const T_iter& last )
   {
      for (std::size_t i = 0; i < COLORS_LEN && first != last; ++first, ++i)
         m_colors[ i ] = ftoi1_220( *first );

      checkRanges( );
      return *this;
   }

   template <typename T_iter>
   ColorCodePal& assignGsd( T_iter first, const T_iter& last )
   {
      for (std::size_t i = 0; i < GSD_LEN && first != last; ++first, ++i)
         m_gsd[ i ] = ftoi1_220( *first );

      checkRanges( );
      return *this;
   }

   virtual std::string str( ) const 
   {
      std::string s( COLORS_LEN + GSD_LEN, '#' );
      for (std::size_t i = 0; i < COLORS_LEN; ++i)
         s[ i ] = (std::string::value_type) itof1_220( m_colors[ i ] );
      for (std::size_t i = 0; i < GSD_LEN; ++i)
         s[ i + COLORS_LEN ] = (std::string::value_type) itof1_220( m_gsd[ i ] );
      return s;
   }

   void clear( ) 
   {
      std::fill( m_colors, m_colors + COLORS_LEN, 0 );
      std::fill( m_gsd, m_gsd + GSD_LEN, 0 );
   }

   bool operator ==( const ColorCodePal& code ) const 
   {
      return std::equal( m_colors, m_colors + COLORS_LEN, code.m_colors )
             && std::equal( m_gsd, m_gsd + GSD_LEN, code.m_gsd );
   }

   bool operator !=( const ColorCodePal& code ) const 
   {
      return !(std::equal( m_colors, m_colors + COLORS_LEN, code.m_colors )
               && std::equal( m_gsd, m_gsd + GSD_LEN, code.m_gsd ));
   }

   ColorCodePal( ) : ColorCode( ENC_PAL )
   { 
      clear( );
   }
   explicit ColorCodePal( const std::string& code ) : ColorCode( ENC_PAL )
   {
      clear( );
      *this = code;
   }
   explicit ColorCodePal( const char* code ) : ColorCode( ENC_PAL )
   {
      clear( );
      *this = code;
   }
   ColorCodePal( const ColorCodePal& code ) : ColorCode( ENC_PAL ) 
   { 
      std::copy( code.m_colors, code.m_colors + COLORS_LEN, m_colors );
      std::copy( code.m_gsd, code.m_gsd + GSD_LEN, m_gsd );
   }
};

// To be implemented.
class ColorCodeMixed : public ColorCode
{
public:
   struct ColorVal
   {
      bool rgb_encoded;
      union 
      {
         struct 
         {
            unsigned char r;
            unsigned char g;
            unsigned char b;
         };
         unsigned pal;
      };
   };

protected:
   ColorCodeMixed( ) : ColorCode( ENC_MIXED ) { }
};

// To be implementend.
class ColorCodeRgb : public ColorCode
{
public:
   struct ColorVal
   {
      unsigned char r;
      unsigned char g;
      unsigned char b;
   };
protected:
   ColorCodeRgb( ) : ColorCode( ENC_RGB ) { }
};

} // namespace yiffurc

/** \} util */
/** \} cpp */

#endif