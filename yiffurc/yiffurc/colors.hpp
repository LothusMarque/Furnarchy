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

#if !defined( _YIFFURC_COLORS_HPP__ ) && defined( __cplusplus )
#define _YIFFURC_COLORS_HPP__

// for std::copy
#ifndef _SCL_SECURE_NO_WARNINGS
   #define _SCL_SECURE_NO_WARNINGS 1
#endif

#include <algorithm>
#include "util.hpp"

/** \addtogroup cpp
* \{ */
/** \addtogroup colors Palettes and Colors
* \{ */

#ifndef DEFAULT_PIXEL
   #ifdef _WIN32
      #define DEFAULT_PIXEL PixelB8G8R8
   #else
      #define DEFAULT_PIXEL PixelR8G8B8
   #endif
#endif

namespace yiffurc {

//////////////////////////////////////////////////////////////////////////

#pragma pack( push, 1 )

/** Windows Bitmap Pixel.
* Traits/data type for a windows (BGR-order) 24-bit pixel.
* If you want to define your own pixel type for use with the other
* classes here, you should implement all the methods in this type. */
struct PixelB8G8R8
{
   unsigned m_b : 8;
   unsigned m_g : 8;
   unsigned m_r : 8;

   PixelB8G8R8( unsigned char r,
                unsigned char g,
                unsigned char b ) : m_b( b ), m_g( g ), m_r( r ) { }
   PixelB8G8R8( ) { }
   unsigned char r( ) const { return m_r; }
   unsigned char g( ) const { return m_g; }
   unsigned char b( ) const { return m_b; }
   void rgb( unsigned char r, unsigned char g, unsigned char b )
   { m_b = b; m_g = g; m_r = r; }
   template <typename T_SrcPixel>
   PixelB8G8R8& operator =( const T_SrcPixel& src )
   { m_b = src.b( ); m_g = src.g( ); m_r = src.r( ); return *this; }
};

/** Traditional, 24-bit RGB pixel.
* Traits/data type for a regular (RGB-order) 24-bit pixel.
* If you want to define your own pixel type for use with the other
* classes here, you should implement all the methods in this type. */
struct PixelR8G8B8
{
   unsigned m_r : 8;
   unsigned m_g : 8;
   unsigned m_b : 8;

   PixelR8G8B8( unsigned char r,
                unsigned char g,
                unsigned char b ) : m_r( r ), m_g( g ), m_b( b ) { }
   PixelR8G8B8( ) { }
   unsigned char r( ) const { return m_r; }
   unsigned char g( ) const { return m_g; }
   unsigned char b( ) const { return m_b; }
   void rgb( unsigned char r, unsigned char g, unsigned char b )
   { m_r = r; m_g = g; m_b = b; }
   template <typename T_SrcPixel>
   PixelR8G8B8& operator =( const T_SrcPixel& src )
   { m_r = src.r( ); m_g = src.g( ); m_b = src.b( ); return *this; }
};

/** 24-bit RGB pixel aligned to 32-bits.
* Traits/data type for a regular (RGB-order), 32-bit aligned 24-bit pixel.
* If you want to define your own pixel type for use with the other
* classes here, you should implement all the methods in this type. */
struct PixelR8G8B8X8
{
   unsigned m_r : 8;
   unsigned m_g : 8;
   unsigned m_b : 8;
   unsigned m_x : 8;

   PixelR8G8B8X8( unsigned char r,
                  unsigned char g,
                  unsigned char b ) : m_r( r ), m_g( g ), m_b( b ), m_x( 0 ) { }
   PixelR8G8B8X8( ) { }
   unsigned char r( ) const { return m_r; }
   unsigned char g( ) const { return m_g; }
   unsigned char b( ) const { return m_b; }
   void rgb( unsigned char r, unsigned char g, unsigned char b )
   { m_r = r; m_g = g; m_b = b; }
   template <typename T_SrcPixel>
   PixelR8G8B8X8& operator =( const T_SrcPixel& src )
   { m_r = src.r( ); m_g = src.g( ); m_b = src.b( ); return *this; }
   PixelR8G8B8X8& operator =( const PixelR8G8B8X8& src )
   { *(int*)this = *(const int*)&src; return *this; }
};

/** 24-bit BGR (windows bitmap) pixel aligned to 32-bits.
* Traits/data type for a regular (RGB-order), 32-bit aligned 24-bit pixel.
* If you want to define your own pixel type for use with the other
* classes here, you should implement all the methods in this type. */
struct PixelB8G8R8X8
{
   unsigned m_b : 8;
   unsigned m_g : 8;
   unsigned m_r : 8;
   unsigned m_x : 8;

   PixelB8G8R8X8( unsigned char r,
                  unsigned char g,
                  unsigned char b ) : m_b( b ), m_g( g ), m_r( r ), m_x( 0 ) { }
   PixelB8G8R8X8( ) { }
   unsigned char r( ) const { return m_r; }
   unsigned char g( ) const { return m_g; }
   unsigned char b( ) const { return m_b; }
   void rgb( unsigned char r, unsigned char g, unsigned char b )
   { m_r = r; m_g = g; m_b = b; }
   template <typename T_SrcPixel>
   PixelB8G8R8X8& operator =( const T_SrcPixel& src )
   { m_r = src.r( ); m_g = src.g( ); m_b = src.b( ); return *this; }
   PixelB8G8R8X8& operator =( const PixelB8G8R8X8& src )
   { *(int*)this = *(const int*)&src; return *this; }
};

#pragma pack( pop )

//////////////////////////////////////////////////////////////////////////

/** Fill a palette with the default furcadia colors.
*  This assumes the destination palette has 256 entries.
*  \param[out] pal  Palette (pixel array) to fill.
*  \param[in] off   Index to begin filling with default colors. Range: [0,256)
*  \param[in] count Length of fill. Range: [0,256 - off]
*  \return \a pal */
template <typename T_Pixel>
T_Pixel* fill_default_palette( T_Pixel* pal, unsigned off = 0, unsigned count = 256 )
{
   static const T_Pixel s_default[ 256 ] =
   {
      T_Pixel( 199, 144, 186 ), T_Pixel( 128, 0  , 0   ), T_Pixel( 0  , 128, 0   ), T_Pixel( 128, 128, 0   ),
      T_Pixel( 0  , 0  , 128 ), T_Pixel( 128, 0  , 128 ), T_Pixel( 0  , 128, 128 ), T_Pixel( 192, 192, 192 ),
      T_Pixel( 192, 220, 192 ), T_Pixel( 166, 202, 240 ), T_Pixel( 255, 55 , 55  ), T_Pixel( 227, 39 , 39  ),
      T_Pixel( 199, 23 , 23  ), T_Pixel( 171, 11 , 11  ), T_Pixel( 143, 7  , 7   ), T_Pixel( 99 , 0  , 0   ),
      T_Pixel( 231, 211, 215 ), T_Pixel( 211, 163, 175 ), T_Pixel( 191, 123, 135 ), T_Pixel( 171, 91 , 103 ),
      T_Pixel( 151, 59 , 75  ), T_Pixel( 131, 35 , 47  ), T_Pixel( 115, 15 , 27  ), T_Pixel( 95 , 0  , 11  ),
      T_Pixel( 171, 111, 79  ), T_Pixel( 163, 107, 79  ), T_Pixel( 155, 103, 79  ), T_Pixel( 147, 95 , 79  ),
      T_Pixel( 143, 95 , 79  ), T_Pixel( 135, 91 , 75  ), T_Pixel( 127, 87 , 75  ), T_Pixel( 123, 83 , 75  ),
      T_Pixel( 255, 227, 207 ), T_Pixel( 247, 195, 155 ), T_Pixel( 243, 163, 107 ), T_Pixel( 235, 135, 63  ),
      T_Pixel( 211, 111, 39  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 163, 71 , 7   ), T_Pixel( 139, 55 , 0   ),
      T_Pixel( 219, 183, 163 ), T_Pixel( 211, 171, 147 ), T_Pixel( 207, 163, 135 ), T_Pixel( 203, 151, 123 ),
      T_Pixel( 195, 143, 111 ), T_Pixel( 191, 135, 99  ), T_Pixel( 187, 127, 87  ), T_Pixel( 183, 119, 79  ),
      T_Pixel( 255, 251, 235 ), T_Pixel( 251, 235, 183 ), T_Pixel( 247, 223, 135 ), T_Pixel( 243, 215, 91  ),
      T_Pixel( 219, 187, 71  ), T_Pixel( 199, 159, 51  ), T_Pixel( 175, 135, 39  ), T_Pixel( 155, 111, 27  ),
      T_Pixel( 175, 151, 79  ), T_Pixel( 159, 139, 63  ), T_Pixel( 147, 127, 51  ), T_Pixel( 135, 115, 39  ),
      T_Pixel( 119, 107, 27  ), T_Pixel( 107, 95 , 19  ), T_Pixel( 95 , 83 , 11  ), T_Pixel( 83 , 75 , 7   ),
      T_Pixel( 163, 223, 131 ), T_Pixel( 143, 203, 107 ), T_Pixel( 123, 183, 83  ), T_Pixel( 107, 163, 63  ),
      T_Pixel( 95 , 143, 47  ), T_Pixel( 79 , 123, 31  ), T_Pixel( 67 , 103, 19  ), T_Pixel( 55 , 83 , 11  ),
      T_Pixel( 131, 195, 115 ), T_Pixel( 111, 175, 91  ), T_Pixel( 95 , 155, 67  ), T_Pixel( 79 , 139, 47  ),
      T_Pixel( 63 , 119, 31  ), T_Pixel( 51 , 99 , 19  ), T_Pixel( 43 , 83 , 11  ), T_Pixel( 39 , 71 , 7   ),
      T_Pixel( 235, 243, 255 ), T_Pixel( 183, 203, 231 ), T_Pixel( 143, 171, 207 ), T_Pixel( 103, 139, 187 ),
      T_Pixel( 71 , 111, 163 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 23 , 67 , 119 ), T_Pixel( 7  , 51 , 99  ),
      T_Pixel( 231, 235, 255 ), T_Pixel( 219, 223, 247 ), T_Pixel( 199, 203, 231 ), T_Pixel( 179, 187, 219 ),
      T_Pixel( 163, 171, 207 ), T_Pixel( 143, 151, 187 ), T_Pixel( 123, 131, 167 ), T_Pixel( 103, 115, 151 ),
      T_Pixel( 251, 211, 255 ), T_Pixel( 235, 187, 235 ), T_Pixel( 215, 163, 207 ), T_Pixel( 195, 143, 183 ),
      T_Pixel( 179, 123, 159 ), T_Pixel( 159, 103, 135 ), T_Pixel( 139, 87 , 111 ), T_Pixel( 123, 71 , 91  ),
      T_Pixel( 239, 231, 243 ), T_Pixel( 231, 219, 235 ), T_Pixel( 207, 191, 215 ), T_Pixel( 183, 163, 195 ),
      T_Pixel( 163, 135, 175 ), T_Pixel( 143, 111, 155 ), T_Pixel( 123, 91 , 135 ), T_Pixel( 103, 71 , 115 ),
      T_Pixel( 235, 235, 255 ), T_Pixel( 203, 199, 239 ), T_Pixel( 175, 171, 223 ), T_Pixel( 151, 143, 207 ),
      T_Pixel( 135, 115, 191 ), T_Pixel( 115, 95 , 175 ), T_Pixel( 103, 71 , 159 ), T_Pixel( 91 , 55 , 147 ),
      T_Pixel( 255, 255, 255 ), T_Pixel( 235, 243, 231 ), T_Pixel( 219, 231, 211 ), T_Pixel( 203, 219, 191 ),
      T_Pixel( 187, 207, 171 ), T_Pixel( 171, 195, 155 ), T_Pixel( 155, 183, 139 ), T_Pixel( 143, 175, 123 ),
      T_Pixel( 255, 175, 195 ), T_Pixel( 239, 127, 159 ), T_Pixel( 227, 79 , 127 ), T_Pixel( 211, 39 , 103 ),
      T_Pixel( 199, 7  , 83  ), T_Pixel( 171, 7  , 71  ), T_Pixel( 143, 11 , 63  ), T_Pixel( 111, 7  , 47  ),
      T_Pixel( 255, 251, 175 ), T_Pixel( 243, 239, 159 ), T_Pixel( 227, 231, 147 ), T_Pixel( 211, 219, 135 ),
      T_Pixel( 199, 207, 127 ), T_Pixel( 183, 195, 115 ), T_Pixel( 167, 183, 103 ), T_Pixel( 155, 171, 95  ),
      T_Pixel( 247, 231, 231 ), T_Pixel( 235, 207, 207 ), T_Pixel( 219, 187, 187 ), T_Pixel( 207, 167, 167 ),
      T_Pixel( 195, 151, 151 ), T_Pixel( 183, 135, 135 ), T_Pixel( 171, 119, 119 ), T_Pixel( 159, 103, 103 ),
      T_Pixel( 147, 87 , 87  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 123, 63 , 63  ), T_Pixel( 111, 51 , 51  ),
      T_Pixel( 99 , 43 , 43  ), T_Pixel( 87 , 31 , 31  ), T_Pixel( 75 , 23 , 23  ), T_Pixel( 63 , 19 , 19  ),
      T_Pixel( 243, 239, 247 ), T_Pixel( 227, 219, 235 ), T_Pixel( 211, 199, 223 ), T_Pixel( 195, 179, 215 ),
      T_Pixel( 179, 163, 203 ), T_Pixel( 167, 147, 195 ), T_Pixel( 151, 131, 183 ), T_Pixel( 135, 115, 171 ),
      T_Pixel( 123, 103, 163 ), T_Pixel( 111, 91 , 151 ), T_Pixel( 99 , 79 , 143 ), T_Pixel( 83 , 67 , 131 ),
      T_Pixel( 71 , 55 , 119 ), T_Pixel( 63 , 47 , 111 ), T_Pixel( 51 , 39 , 99  ), T_Pixel( 43 , 31 , 91  ),
      T_Pixel( 255, 251, 223 ), T_Pixel( 239, 235, 203 ), T_Pixel( 227, 223, 187 ), T_Pixel( 215, 207, 171 ),
      T_Pixel( 203, 195, 155 ), T_Pixel( 191, 179, 139 ), T_Pixel( 179, 167, 127 ), T_Pixel( 167, 155, 115 ),
      T_Pixel( 155, 143, 99  ), T_Pixel( 139, 127, 87  ), T_Pixel( 127, 115, 75  ), T_Pixel( 115, 103, 67  ),
      T_Pixel( 103, 91 , 55  ), T_Pixel( 91 , 79 , 47  ), T_Pixel( 79 , 67 , 39  ), T_Pixel( 67 , 55 , 31  ),
      T_Pixel( 247, 247, 247 ), T_Pixel( 231, 227, 227 ), T_Pixel( 219, 211, 211 ), T_Pixel( 203, 195, 195 ),
      T_Pixel( 191, 179, 179 ), T_Pixel( 175, 167, 167 ), T_Pixel( 163, 151, 151 ), T_Pixel( 147, 135, 135 ),
      T_Pixel( 135, 123, 123 ), T_Pixel( 119, 107, 107 ), T_Pixel( 107, 95 , 95  ), T_Pixel( 91 , 79 , 79  ),
      T_Pixel( 79 , 67 , 67  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 51 , 43 , 43  ), T_Pixel( 23 , 19 , 19  ),
      T_Pixel( 255, 255, 255 ), T_Pixel( 243, 239, 235 ), T_Pixel( 231, 223, 219 ), T_Pixel( 219, 211, 207 ),
      T_Pixel( 207, 195, 191 ), T_Pixel( 195, 183, 179 ), T_Pixel( 183, 171, 167 ), T_Pixel( 171, 159, 155 ),
      T_Pixel( 159, 147, 139 ), T_Pixel( 147, 135, 127 ), T_Pixel( 135, 123, 115 ), T_Pixel( 123, 111, 103 ),
      T_Pixel( 115, 103, 95  ), T_Pixel( 103, 91 , 83  ), T_Pixel( 91 , 79 , 71  ), T_Pixel( 83 , 71 , 63  ),
      T_Pixel( 235, 215, 243 ), T_Pixel( 207, 167, 223 ), T_Pixel( 183, 127, 203 ), T_Pixel( 163, 91 , 183 ),
      T_Pixel( 147, 63 , 167 ), T_Pixel( 127, 35 , 147 ), T_Pixel( 111, 15 , 127 ), T_Pixel( 95 , 0  , 111 ),
      T_Pixel( 139, 191, 243 ), T_Pixel( 119, 147, 247 ), T_Pixel( 103, 99 , 255 ), T_Pixel( 75 , 71 , 227 ),
      T_Pixel( 47 , 47 , 199 ), T_Pixel( 27 , 27 , 175 ), T_Pixel( 11 , 11 , 147 ), T_Pixel( 0  , 0  , 123 ),
      T_Pixel( 255, 95 , 27  ), T_Pixel( 255, 239, 7   ), T_Pixel( 151, 215, 11  ), T_Pixel( 0  , 135, 71  ),
      T_Pixel( 0  , 135, 139 ), T_Pixel( 19 , 95 , 255 ), T_Pixel( 255, 251, 240 ), T_Pixel( 160, 160, 164 ),
      T_Pixel( 128, 128, 128 ), T_Pixel( 255, 0  , 0   ), T_Pixel( 0  , 255, 0   ), T_Pixel( 255, 255, 0   ),
      T_Pixel( 0  , 0  , 255 ), T_Pixel( 255, 0  , 255 ), T_Pixel( 0  , 255, 255 ), T_Pixel( 255, 255, 255 ),
   };

   if (off < 256)
   {
      if (off + count > 256)
         count = 256 - off;

      std::copy( s_default + off, s_default + (off + count), pal + off );
   }

   return pal;
}

//////////////////////////////////////////////////////////////////////////

/** A generic palette remap template.
* \a T_index is the first palette entry to remap.
* \a T_count is the number of entries to remap.
* \a T_Pixel is the native pixel traits/data type to use. */
template <unsigned T_index, unsigned T_count, typename T_Pixel = DEFAULT_PIXEL>
struct Remap
{
   enum { INDEX = T_index, COUNT = T_count };
   typedef T_Pixel PixelT;

   ////////////////////////////////////////////////////////

   PixelT m_remaps[ COUNT ];

   ////////////////////////////////////////////////////////
   template <typename T_SrcPixel>
   explicit Remap( const T_SrcPixel remaps[COUNT] )
   {
      set( remaps );
   }

   Remap( ) { }

   template <typename T_SrcPixel>
   void set( const T_SrcPixel remaps[COUNT] )
   {
      for (unsigned i = 0; i < COUNT; ++i, ++remaps)
         m_remaps[ i ] = *remaps;
   }

   /** Performs the remap on the palette.
   *  \param[out] pal   The palette (array of pixels) to remap.
   *  \param[in]  count The number of entries in the palette.
   *  \return \a pal
   *  If \a pal is not in the same format as the native remaps, it
   *  will be converted, so long as the \c T_DstPixel type is well-formed. */
   template <typename T_DstPixel>
   T_DstPixel* remap( T_DstPixel* pal, unsigned count = 256 ) const
   {
      if (INDEX < count)
      {
         const unsigned n = INDEX + COUNT <= count ? COUNT : count - INDEX;
         std::copy( m_remaps, m_remaps + n, pal + INDEX );
      }
      return pal;
   }

#ifndef ALWAYS_ASSIGN_PIXELS
   T_Pixel* remap( T_Pixel* pal, unsigned count = 256 ) const
   {
      if (INDEX < count)
      {
         const unsigned n = INDEX + COUNT <= count ? COUNT : count - INDEX;
         std::memcpy( pal + INDEX, m_remaps, n * sizeof( PixelT ) );
      }
      return pal;
   }
#endif

};


#define _REMAPDEF( T, index, count )                                           \
template <typename T_Pixel = DEFAULT_PIXEL>                                    \
struct T: Remap<index,count,T_Pixel>                                           \
{                                                                              \
   enum { INDEX = index, COUNT = count };                                      \
   typedef T_Pixel PixelT;                                                     \
   typedef Remap<index,count,T_Pixel> ParentT;                                 \
                                                                               \
   template <typename T_SrcPixel>                                              \
   T( const T_SrcPixel* remaps ) : ParentT( remaps ) { }                       \
   T( ) { }                                                                    \
};

_REMAPDEF( RemapFur,      199, 8 );
_REMAPDEF( RemapMarkings, 136, 8 );
_REMAPDEF( RemapHair,     128, 8 );
_REMAPDEF( RemapEyes,     50,  1 );
_REMAPDEF( RemapBadge,    11,  1 );
_REMAPDEF( RemapVest,     72,  8 );
_REMAPDEF( RemapBracers,  80,  8 );
_REMAPDEF( RemapCape,     32,  8 );
_REMAPDEF( RemapBoots,    16,  8 );
_REMAPDEF( RemapTrousers, 224, 8 );

#undef _REMAPDEF

//////////////////////////////////////////////////////////////////////////

/** Embodies a complete set of palette remaps.
*  \a T_Pixel is the native pixel traits/data type to use. */
template <typename T_Pixel = DEFAULT_PIXEL>
struct AvatarRemaps
{
   typedef T_Pixel PixelT;
   typedef RemapFur<PixelT> RemapFur;
   typedef RemapMarkings<PixelT> RemapMarkings;
   typedef RemapHair<PixelT> RemapHair;
   typedef RemapEyes<PixelT> RemapEyes;
   typedef RemapBadge<PixelT> RemapBadge;
   typedef RemapVest<PixelT> RemapVest;
   typedef RemapBracers<PixelT> RemapBracers;
   typedef RemapCape<PixelT> RemapCape;
   typedef RemapBoots<PixelT> RemapBoots;
   typedef RemapTrousers<PixelT> RemapTrousers;

   enum { COUNT_FUR = 25, COUNT_MARKINGS = 25, COUNT_HAIR = 45,
          COUNT_EYES = 30, COUNT_BADGE = 40, COUNT_VEST = 30,
          COUNT_BRACERS = 30, COUNT_CAPE = 30, COUNT_BOOTS = 30,
          COUNT_TROUSERS = 30 };

   RemapFur      m_fur[ COUNT_FUR ];
   RemapMarkings m_markings[ COUNT_MARKINGS ];
   RemapHair     m_hair[ COUNT_HAIR ];
   RemapEyes     m_eyes[ COUNT_EYES ];
   RemapBadge    m_badge[ COUNT_BADGE ];
   RemapVest     m_vest[ COUNT_VEST ];
   RemapBracers  m_bracers[ COUNT_BRACERS ];
   RemapCape     m_cape[ COUNT_CAPE ];
   RemapBoots    m_boots[ COUNT_BOOTS ];
   RemapTrousers m_trousers[ COUNT_TROUSERS ];

   static const PixelT s_default_fur[ COUNT_FUR ][ RemapFur::COUNT ];
   static const PixelT s_default_markings[ COUNT_MARKINGS ][ RemapMarkings::COUNT ];
   static const PixelT s_default_hair[ COUNT_HAIR ][ RemapHair::COUNT ];
   static const PixelT s_default_eyes[ COUNT_EYES ][ RemapEyes::COUNT ];
   static const PixelT s_default_badge[ COUNT_BADGE ][ RemapBadge::COUNT ];
   static const PixelT s_default_vest[ COUNT_VEST ][ RemapVest::COUNT ];
   static const PixelT s_default_bracers[ COUNT_BRACERS ][ RemapBracers::COUNT ];
   static const PixelT s_default_cape[ COUNT_CAPE ][ RemapCape::COUNT ];
   static const PixelT s_default_boots[ COUNT_BOOTS ][ RemapBoots::COUNT ];
   static const PixelT s_default_trousers[ COUNT_TROUSERS ][ RemapTrousers::COUNT ];

   /** Apply the default remap colors to the remaps.
   * By default, the remaps just contain garbage after this class is instantiated. */
   void applyDefaults( )
   {
      for (unsigned i = 0; i < COUNT_FUR; ++i)
         m_fur[ i ].set( s_default_fur[ i ] );

      for (unsigned i = 0; i < COUNT_MARKINGS; ++i)
         m_markings[ i ].set( s_default_markings[ i ] );

      for (unsigned i = 0; i < COUNT_HAIR; ++i)
         m_hair[ i ].set( s_default_hair[ i ] );

      for (unsigned i = 0; i < COUNT_EYES; ++i)
         m_eyes[ i ].set( s_default_eyes[ i ] );

      for (unsigned i = 0; i < COUNT_BADGE; ++i)
         m_badge[ i ].set( s_default_badge[ i ] );

      for (unsigned i = 0; i < COUNT_VEST; ++i)
         m_vest[ i ].set( s_default_vest[ i ] );

      for (unsigned i = 0; i < COUNT_BRACERS; ++i)
         m_bracers[ i ].set( s_default_bracers[ i ] );

      for (unsigned i = 0; i < COUNT_CAPE; ++i)
         m_cape[ i ].set( s_default_cape[ i ] );

      for (unsigned i = 0; i < COUNT_BOOTS; ++i)
         m_boots[ i ].set( s_default_boots[ i ] );

      for (unsigned i = 0; i < COUNT_TROUSERS; ++i)
         m_trousers[ i ].set( s_default_trousers[ i ] );
   }

   /** Remap a palette according to a player's color code.
   * \a param[in] colors The color code.
   * \a param[out] pal   The palette (array of pixels) to remap.
   * \a param[in] count  The number of entries in the palette.
   * The destination pixel format need not match the native
   * (AvatarRemaps::PixelT) pixel format.  A conversion will be
   * properly made if \c T_DstPixel type is well-formed. */
   template <typename T_DstPixel>
   void remap( const ColorCodePal& colors, T_DstPixel* pal, unsigned count = 256 ) const
   {
      m_fur[ colors.getColor( colors.COLOR_FUR ) ].remap( pal, count );
      m_markings[ colors.getColor( colors.COLOR_MARKINGS ) ].remap( pal, count );
      m_hair[ colors.getColor( colors.COLOR_HAIR ) ].remap( pal, count );
      m_eyes[ colors.getColor( colors.COLOR_EYES ) ].remap( pal, count );
      m_badge[ colors.getColor( colors.COLOR_BADGE ) ].remap( pal, count );
      m_vest[ colors.getColor( colors.COLOR_VEST ) ].remap( pal, count );
      m_bracers[ colors.getColor( colors.COLOR_BRACERS ) ].remap( pal, count );
      m_cape[ colors.getColor( colors.COLOR_CAPE ) ].remap( pal, count );
      m_boots[ colors.getColor( colors.COLOR_BOOTS ) ].remap( pal, count );
      m_trousers[ colors.getColor( colors.COLOR_TROUSERS ) ].remap( pal, count );
   }

   AvatarRemaps( ) { }
};

template <typename T_Pixel>
const T_Pixel AvatarRemaps<T_Pixel>::s_default_fur[ AvatarRemaps<T_Pixel>::COUNT_FUR ][ RemapFur::COUNT ] =
{
   {  T_Pixel( 147, 135, 135 ), T_Pixel( 135, 123, 123 ), T_Pixel( 119, 107, 107 ), T_Pixel( 107, 95 , 95  ), T_Pixel( 91 , 79 , 79  ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 51 , 43 , 43  ) },
   {  T_Pixel( 219, 223, 247 ), T_Pixel( 199, 203, 231 ), T_Pixel( 179, 187, 219 ), T_Pixel( 163, 171, 207 ), T_Pixel( 143, 151, 187 ), T_Pixel( 123, 131, 167 ), T_Pixel( 103, 115, 151 ), T_Pixel( 51 , 39 , 99  ) },
   {  T_Pixel( 79 , 67 , 67  ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 51 , 43 , 43  ), T_Pixel( 51 , 43 , 43  ), T_Pixel( 23 , 19 , 19  ), T_Pixel( 23 , 19 , 19  ) },
   {  T_Pixel( 159, 147, 139 ), T_Pixel( 147, 135, 127 ), T_Pixel( 135, 123, 115 ), T_Pixel( 123, 111, 103 ), T_Pixel( 115, 103, 95  ), T_Pixel( 103, 91 , 83  ), T_Pixel( 91 , 79 , 71  ), T_Pixel( 83 , 71 , 63  ) },
   {  T_Pixel( 155, 143, 99  ), T_Pixel( 139, 127, 87  ), T_Pixel( 127, 115, 75  ), T_Pixel( 115, 103, 67  ), T_Pixel( 103, 91 , 55  ), T_Pixel( 91 , 79 , 47  ), T_Pixel( 79 , 67 , 39  ), T_Pixel( 67 , 55 , 31  ) },
   {  T_Pixel( 147, 87 , 87  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 123, 63 , 63  ), T_Pixel( 111, 51 , 51  ), T_Pixel( 99 , 43 , 43  ), T_Pixel( 87 , 31 , 31  ), T_Pixel( 75 , 23 , 23  ), T_Pixel( 63 , 19 , 19  ) },
   {  T_Pixel( 239, 231, 243 ), T_Pixel( 231, 219, 235 ), T_Pixel( 207, 191, 215 ), T_Pixel( 183, 163, 195 ), T_Pixel( 163, 135, 175 ), T_Pixel( 143, 111, 155 ), T_Pixel( 123, 91 , 135 ), T_Pixel( 103, 71 , 115 ) },
   {  T_Pixel( 251, 211, 255 ), T_Pixel( 235, 187, 235 ), T_Pixel( 215, 163, 207 ), T_Pixel( 195, 143, 183 ), T_Pixel( 179, 123, 159 ), T_Pixel( 159, 103, 135 ), T_Pixel( 139, 87 , 111 ), T_Pixel( 123, 71 , 91  ) },
   {  T_Pixel( 255, 227, 207 ), T_Pixel( 247, 195, 155 ), T_Pixel( 243, 163, 107 ), T_Pixel( 235, 135, 63  ), T_Pixel( 211, 111, 39  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 163, 71 , 7   ), T_Pixel( 139, 55 , 0   ) },
   {  T_Pixel( 231, 211, 215 ), T_Pixel( 211, 163, 175 ), T_Pixel( 191, 123, 135 ), T_Pixel( 171, 91 , 103 ), T_Pixel( 151, 59 , 75  ), T_Pixel( 131, 35 , 47  ), T_Pixel( 115, 15 , 27  ), T_Pixel( 95 , 0  , 11  ) },
   {  T_Pixel( 219, 183, 163 ), T_Pixel( 207, 163, 135 ), T_Pixel( 195, 143, 111 ), T_Pixel( 183, 119, 79  ), T_Pixel( 163, 107, 79  ), T_Pixel( 143, 95 , 79  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 147, 87 , 87  ) },
   {  T_Pixel( 219, 187, 187 ), T_Pixel( 207, 167, 167 ), T_Pixel( 183, 135, 135 ), T_Pixel( 171, 119, 119 ), T_Pixel( 159, 103, 103 ), T_Pixel( 147, 87 , 87  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 111, 51 , 51  ) },
   {  T_Pixel( 247, 195, 155 ), T_Pixel( 243, 163, 107 ), T_Pixel( 235, 135, 63  ), T_Pixel( 211, 111, 39  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 199, 23 , 23  ), T_Pixel( 163, 71 , 7   ), T_Pixel( 151, 59 , 75  ) },
   {  T_Pixel( 239, 235, 203 ), T_Pixel( 227, 223, 187 ), T_Pixel( 215, 207, 171 ), T_Pixel( 203, 195, 155 ), T_Pixel( 191, 179, 139 ), T_Pixel( 179, 167, 127 ), T_Pixel( 195, 151, 151 ), T_Pixel( 171, 119, 119 ) },
   {  T_Pixel( 255, 251, 235 ), T_Pixel( 251, 235, 183 ), T_Pixel( 247, 223, 135 ), T_Pixel( 243, 215, 91  ), T_Pixel( 219, 187, 71  ), T_Pixel( 199, 159, 51  ), T_Pixel( 175, 135, 39  ), T_Pixel( 155, 111, 27  ) },
   {  T_Pixel( 131, 195, 115 ), T_Pixel( 111, 175, 91  ), T_Pixel( 95 , 155, 67  ), T_Pixel( 79 , 139, 47  ), T_Pixel( 63 , 119, 31  ), T_Pixel( 51 , 99 , 19  ), T_Pixel( 43 , 83 , 11  ), T_Pixel( 39 , 71 , 7   ) },
   {  T_Pixel( 255, 175, 195 ), T_Pixel( 239, 127, 159 ), T_Pixel( 227, 79 , 127 ), T_Pixel( 211, 39 , 103 ), T_Pixel( 199, 7  , 83  ), T_Pixel( 171, 7  , 71  ), T_Pixel( 143, 11 , 63  ), T_Pixel( 111, 7  , 47  ) },
   {  T_Pixel( 247, 231, 231 ), T_Pixel( 235, 207, 207 ), T_Pixel( 219, 187, 187 ), T_Pixel( 195, 151, 151 ), T_Pixel( 171, 119, 119 ), T_Pixel( 147, 87 , 87  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 123, 63 , 63  ) },
   {  T_Pixel( 235, 243, 255 ), T_Pixel( 183, 203, 231 ), T_Pixel( 143, 171, 207 ), T_Pixel( 103, 139, 187 ), T_Pixel( 71 , 111, 163 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 23 , 67 , 119 ), T_Pixel( 7  , 51 , 99  ) },
   {  T_Pixel( 235, 215, 243 ), T_Pixel( 207, 167, 223 ), T_Pixel( 183, 127, 203 ), T_Pixel( 163, 91 , 183 ), T_Pixel( 147, 63 , 167 ), T_Pixel( 127, 35 , 147 ), T_Pixel( 111, 15 , 127 ), T_Pixel( 95 , 0  , 111 ) },
   {  T_Pixel( 255, 55 , 55  ), T_Pixel( 255, 55 , 55  ), T_Pixel( 227, 39 , 39  ), T_Pixel( 227, 39 , 39  ), T_Pixel( 199, 23 , 23  ), T_Pixel( 171, 11 , 11  ), T_Pixel( 143, 7  , 7   ), T_Pixel( 99 , 0  , 0   ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 247, 247, 247 ), T_Pixel( 243, 239, 235 ), T_Pixel( 231, 227, 227 ), T_Pixel( 219, 211, 211 ), T_Pixel( 195, 183, 179 ), T_Pixel( 159, 147, 139 ), T_Pixel( 123, 111, 103 ) },
   {  T_Pixel( 211, 199, 223 ), T_Pixel( 179, 163, 203 ), T_Pixel( 151, 131, 183 ), T_Pixel( 123, 103, 163 ), T_Pixel( 99 , 79 , 143 ), T_Pixel( 83 , 67 , 131 ), T_Pixel( 63 , 47 , 111 ), T_Pixel( 43 , 31 , 91  ) },
   {  T_Pixel( 203, 195, 195 ), T_Pixel( 191, 179, 179 ), T_Pixel( 175, 167, 167 ), T_Pixel( 163, 151, 151 ), T_Pixel( 147, 135, 135 ), T_Pixel( 135, 123, 123 ), T_Pixel( 119, 107, 107 ), T_Pixel( 107, 95 , 95  ) },
   {  T_Pixel( 139, 191, 243 ), T_Pixel( 119, 147, 247 ), T_Pixel( 103, 99 , 255 ), T_Pixel( 75 , 71 , 227 ), T_Pixel( 47 , 47 , 199 ), T_Pixel( 27 , 27 , 175 ), T_Pixel( 11 , 11 , 147 ), T_Pixel( 0  , 0  , 123 ) },
};

template <typename T_Pixel>
const T_Pixel AvatarRemaps<T_Pixel>::s_default_markings[ AvatarRemaps<T_Pixel>::COUNT_MARKINGS ][ RemapMarkings::COUNT ] =
{
   {  T_Pixel( 147, 135, 135 ), T_Pixel( 135, 123, 123 ), T_Pixel( 119, 107, 107 ), T_Pixel( 107, 95 , 95  ), T_Pixel( 91 , 79 , 79  ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 51 , 43 , 43  ) },
   {  T_Pixel( 219, 223, 247 ), T_Pixel( 199, 203, 231 ), T_Pixel( 179, 187, 219 ), T_Pixel( 163, 171, 207 ), T_Pixel( 143, 151, 187 ), T_Pixel( 123, 131, 167 ), T_Pixel( 103, 115, 151 ), T_Pixel( 51 , 39 , 99  ) },
   {  T_Pixel( 79 , 67 , 67  ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 51 , 43 , 43  ), T_Pixel( 51 , 43 , 43  ), T_Pixel( 23 , 19 , 19  ), T_Pixel( 23 , 19 , 19  ) },
   {  T_Pixel( 159, 147, 139 ), T_Pixel( 147, 135, 127 ), T_Pixel( 135, 123, 115 ), T_Pixel( 123, 111, 103 ), T_Pixel( 115, 103, 95  ), T_Pixel( 103, 91 , 83  ), T_Pixel( 91 , 79 , 71  ), T_Pixel( 83 , 71 , 63  ) },
   {  T_Pixel( 155, 143, 99  ), T_Pixel( 139, 127, 87  ), T_Pixel( 127, 115, 75  ), T_Pixel( 115, 103, 67  ), T_Pixel( 103, 91 , 55  ), T_Pixel( 91 , 79 , 47  ), T_Pixel( 79 , 67 , 39  ), T_Pixel( 67 , 55 , 31  ) },
   {  T_Pixel( 147, 87 , 87  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 123, 63 , 63  ), T_Pixel( 111, 51 , 51  ), T_Pixel( 99 , 43 , 43  ), T_Pixel( 87 , 31 , 31  ), T_Pixel( 75 , 23 , 23  ), T_Pixel( 63 , 19 , 19  ) },
   {  T_Pixel( 239, 231, 243 ), T_Pixel( 231, 219, 235 ), T_Pixel( 207, 191, 215 ), T_Pixel( 183, 163, 195 ), T_Pixel( 163, 135, 175 ), T_Pixel( 143, 111, 155 ), T_Pixel( 123, 91 , 135 ), T_Pixel( 103, 71 , 115 ) },
   {  T_Pixel( 251, 211, 255 ), T_Pixel( 235, 187, 235 ), T_Pixel( 215, 163, 207 ), T_Pixel( 195, 143, 183 ), T_Pixel( 179, 123, 159 ), T_Pixel( 159, 103, 135 ), T_Pixel( 139, 87 , 111 ), T_Pixel( 123, 71 , 91  ) },
   {  T_Pixel( 255, 227, 207 ), T_Pixel( 247, 195, 155 ), T_Pixel( 243, 163, 107 ), T_Pixel( 235, 135, 63  ), T_Pixel( 211, 111, 39  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 163, 71 , 7   ), T_Pixel( 139, 55 , 0   ) },
   {  T_Pixel( 231, 211, 215 ), T_Pixel( 211, 163, 175 ), T_Pixel( 191, 123, 135 ), T_Pixel( 171, 91 , 103 ), T_Pixel( 151, 59 , 75  ), T_Pixel( 131, 35 , 47  ), T_Pixel( 115, 15 , 27  ), T_Pixel( 95 , 0  , 11  ) },
   {  T_Pixel( 219, 183, 163 ), T_Pixel( 207, 163, 135 ), T_Pixel( 195, 143, 111 ), T_Pixel( 183, 119, 79  ), T_Pixel( 163, 107, 79  ), T_Pixel( 143, 95 , 79  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 147, 87 , 87  ) },
   {  T_Pixel( 219, 187, 187 ), T_Pixel( 207, 167, 167 ), T_Pixel( 183, 135, 135 ), T_Pixel( 171, 119, 119 ), T_Pixel( 159, 103, 103 ), T_Pixel( 147, 87 , 87  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 111, 51 , 51  ) },
   {  T_Pixel( 247, 195, 155 ), T_Pixel( 243, 163, 107 ), T_Pixel( 235, 135, 63  ), T_Pixel( 211, 111, 39  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 199, 23 , 23  ), T_Pixel( 163, 71 , 7   ), T_Pixel( 151, 59 , 75  ) },
   {  T_Pixel( 239, 235, 203 ), T_Pixel( 227, 223, 187 ), T_Pixel( 215, 207, 171 ), T_Pixel( 203, 195, 155 ), T_Pixel( 191, 179, 139 ), T_Pixel( 179, 167, 127 ), T_Pixel( 195, 151, 151 ), T_Pixel( 171, 119, 119 ) },
   {  T_Pixel( 255, 251, 235 ), T_Pixel( 251, 235, 183 ), T_Pixel( 247, 223, 135 ), T_Pixel( 243, 215, 91  ), T_Pixel( 219, 187, 71  ), T_Pixel( 199, 159, 51  ), T_Pixel( 175, 135, 39  ), T_Pixel( 155, 111, 27  ) },
   {  T_Pixel( 131, 195, 115 ), T_Pixel( 111, 175, 91  ), T_Pixel( 95 , 155, 67  ), T_Pixel( 79 , 139, 47  ), T_Pixel( 63 , 119, 31  ), T_Pixel( 51 , 99 , 19  ), T_Pixel( 43 , 83 , 11  ), T_Pixel( 39 , 71 , 7   ) },
   {  T_Pixel( 255, 175, 195 ), T_Pixel( 239, 127, 159 ), T_Pixel( 227, 79 , 127 ), T_Pixel( 211, 39 , 103 ), T_Pixel( 199, 7  , 83  ), T_Pixel( 171, 7  , 71  ), T_Pixel( 143, 11 , 63  ), T_Pixel( 111, 7  , 47  ) },
   {  T_Pixel( 247, 231, 231 ), T_Pixel( 235, 207, 207 ), T_Pixel( 219, 187, 187 ), T_Pixel( 195, 151, 151 ), T_Pixel( 171, 119, 119 ), T_Pixel( 147, 87 , 87  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 123, 63 , 63  ) },
   {  T_Pixel( 235, 243, 255 ), T_Pixel( 183, 203, 231 ), T_Pixel( 143, 171, 207 ), T_Pixel( 103, 139, 187 ), T_Pixel( 71 , 111, 163 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 23 , 67 , 119 ), T_Pixel( 7  , 51 , 99  ) },
   {  T_Pixel( 235, 215, 243 ), T_Pixel( 207, 167, 223 ), T_Pixel( 183, 127, 203 ), T_Pixel( 163, 91 , 183 ), T_Pixel( 147, 63 , 167 ), T_Pixel( 127, 35 , 147 ), T_Pixel( 111, 15 , 127 ), T_Pixel( 95 , 0  , 111 ) },
   {  T_Pixel( 255, 55 , 55  ), T_Pixel( 255, 55 , 55  ), T_Pixel( 227, 39 , 39  ), T_Pixel( 227, 39 , 39  ), T_Pixel( 199, 23 , 23  ), T_Pixel( 171, 11 , 11  ), T_Pixel( 143, 7  , 7   ), T_Pixel( 99 , 0  , 0   ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 247, 247, 247 ), T_Pixel( 243, 239, 235 ), T_Pixel( 231, 227, 227 ), T_Pixel( 219, 211, 211 ), T_Pixel( 195, 183, 179 ), T_Pixel( 159, 147, 139 ), T_Pixel( 123, 111, 103 ) },
   {  T_Pixel( 211, 199, 223 ), T_Pixel( 179, 163, 203 ), T_Pixel( 151, 131, 183 ), T_Pixel( 123, 103, 163 ), T_Pixel( 99 , 79 , 143 ), T_Pixel( 83 , 67 , 131 ), T_Pixel( 63 , 47 , 111 ), T_Pixel( 43 , 31 , 91  ) },
   {  T_Pixel( 203, 195, 195 ), T_Pixel( 191, 179, 179 ), T_Pixel( 175, 167, 167 ), T_Pixel( 163, 151, 151 ), T_Pixel( 147, 135, 135 ), T_Pixel( 135, 123, 123 ), T_Pixel( 119, 107, 107 ), T_Pixel( 107, 95 , 95  ) },
   {  T_Pixel( 139, 191, 243 ), T_Pixel( 119, 147, 247 ), T_Pixel( 103, 99 , 255 ), T_Pixel( 75 , 71 , 227 ), T_Pixel( 47 , 47 , 199 ), T_Pixel( 27 , 27 , 175 ), T_Pixel( 11 , 11 , 147 ), T_Pixel( 0  , 0  , 123 ) },
};

template <typename T_Pixel>
const T_Pixel AvatarRemaps<T_Pixel>::s_default_hair[ AvatarRemaps<T_Pixel>::COUNT_HAIR ][ RemapHair::COUNT ] =
{
   {  T_Pixel( 199, 23 , 23  ), T_Pixel( 171, 11 , 11  ), T_Pixel( 143, 7  , 7   ), T_Pixel( 99 , 0  , 0   ), T_Pixel( 63 , 19 , 19  ), T_Pixel( 63 , 19 , 19  ), T_Pixel( 63 , 19 , 19  ), T_Pixel( 63 , 19 , 19  ) },
   {  T_Pixel( 255, 55 , 55  ), T_Pixel( 227, 39 , 39  ), T_Pixel( 199, 23 , 23  ), T_Pixel( 171, 11 , 11  ), T_Pixel( 143, 7  , 7   ), T_Pixel( 143, 7  , 7   ), T_Pixel( 143, 7  , 7   ), T_Pixel( 143, 7  , 7   ) },
   {  T_Pixel( 255, 55 , 55  ), T_Pixel( 255, 55 , 55  ), T_Pixel( 255, 55 , 55  ), T_Pixel( 227, 39 , 39  ), T_Pixel( 199, 23 , 23  ), T_Pixel( 199, 23 , 23  ), T_Pixel( 199, 23 , 23  ), T_Pixel( 199, 23 , 23  ) },
   {  T_Pixel( 243, 163, 107 ), T_Pixel( 235, 135, 63  ), T_Pixel( 255, 95 , 27  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 163, 71 , 7   ), T_Pixel( 163, 71 , 7   ), T_Pixel( 163, 71 , 7   ), T_Pixel( 163, 71 , 7   ) },
   {  T_Pixel( 171, 7  , 71  ), T_Pixel( 143, 11 , 63  ), T_Pixel( 111, 7  , 47  ), T_Pixel( 75 , 23 , 23  ), T_Pixel( 63 , 19 , 19  ), T_Pixel( 63 , 19 , 19  ), T_Pixel( 63 , 19 , 19  ), T_Pixel( 63 , 19 , 19  ) },
   {  T_Pixel( 187, 91 , 23  ), T_Pixel( 163, 71 , 7   ), T_Pixel( 139, 55 , 0   ), T_Pixel( 111, 51 , 51  ), T_Pixel( 99 , 43 , 43  ), T_Pixel( 99 , 43 , 43  ), T_Pixel( 99 , 43 , 43  ), T_Pixel( 99 , 43 , 43  ) },
   {  T_Pixel( 235, 135, 63  ), T_Pixel( 211, 111, 39  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 163, 71 , 7   ), T_Pixel( 139, 55 , 0   ), T_Pixel( 139, 55 , 0   ), T_Pixel( 139, 55 , 0   ), T_Pixel( 139, 55 , 0   ) },
   {  T_Pixel( 247, 195, 155 ), T_Pixel( 243, 163, 107 ), T_Pixel( 235, 135, 63  ), T_Pixel( 211, 111, 39  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 187, 91 , 23  ) },
   {  T_Pixel( 171, 111, 79  ), T_Pixel( 155, 103, 79  ), T_Pixel( 147, 95 , 79  ), T_Pixel( 123, 63 , 63  ), T_Pixel( 99 , 43 , 43  ), T_Pixel( 99 , 43 , 43  ), T_Pixel( 99 , 43 , 43  ), T_Pixel( 99 , 43 , 43  ) },
   {  T_Pixel( 171, 91 , 103 ), T_Pixel( 151, 59 , 75  ), T_Pixel( 131, 35 , 47  ), T_Pixel( 115, 15 , 27  ), T_Pixel( 95 , 0  , 11  ), T_Pixel( 95 , 0  , 11  ), T_Pixel( 95 , 0  , 11  ), T_Pixel( 95 , 0  , 11  ) },
   {  T_Pixel( 199, 159, 51  ), T_Pixel( 175, 135, 39  ), T_Pixel( 155, 111, 27  ), T_Pixel( 107, 95 , 19  ), T_Pixel( 95 , 83 , 11  ), T_Pixel( 95 , 83 , 11  ), T_Pixel( 95 , 83 , 11  ), T_Pixel( 95 , 83 , 11  ) },
   {  T_Pixel( 251, 235, 183 ), T_Pixel( 247, 223, 135 ), T_Pixel( 243, 215, 91  ), T_Pixel( 219, 187, 71  ), T_Pixel( 199, 159, 51  ), T_Pixel( 199, 159, 51  ), T_Pixel( 199, 159, 51  ), T_Pixel( 199, 159, 51  ) },
   {  T_Pixel( 255, 251, 235 ), T_Pixel( 251, 235, 183 ), T_Pixel( 251, 235, 183 ), T_Pixel( 247, 223, 135 ), T_Pixel( 219, 187, 71  ), T_Pixel( 219, 187, 71  ), T_Pixel( 219, 187, 71  ), T_Pixel( 219, 187, 71  ) },
   {  T_Pixel( 255, 251, 235 ), T_Pixel( 251, 235, 183 ), T_Pixel( 255, 239, 7   ), T_Pixel( 219, 187, 71  ), T_Pixel( 199, 159, 51  ), T_Pixel( 199, 159, 51  ), T_Pixel( 199, 159, 51  ), T_Pixel( 199, 159, 51  ) },
   {  T_Pixel( 183, 195, 115 ), T_Pixel( 175, 151, 79  ), T_Pixel( 175, 151, 79  ), T_Pixel( 159, 139, 63  ), T_Pixel( 147, 127, 51  ), T_Pixel( 147, 127, 51  ), T_Pixel( 147, 127, 51  ), T_Pixel( 147, 127, 51  ) },
   {  T_Pixel( 95 , 143, 47  ), T_Pixel( 79 , 123, 31  ), T_Pixel( 67 , 103, 19  ), T_Pixel( 55 , 83 , 11  ), T_Pixel( 39 , 71 , 7   ), T_Pixel( 39 , 71 , 7   ), T_Pixel( 39 , 71 , 7   ), T_Pixel( 39 , 71 , 7   ) },
   {  T_Pixel( 111, 175, 91  ), T_Pixel( 95 , 155, 67  ), T_Pixel( 79 , 139, 47  ), T_Pixel( 63 , 119, 31  ), T_Pixel( 51 , 99 , 19  ), T_Pixel( 51 , 99 , 19  ), T_Pixel( 51 , 99 , 19  ), T_Pixel( 51 , 99 , 19  ) },
   {  T_Pixel( 163, 223, 131 ), T_Pixel( 131, 195, 115 ), T_Pixel( 111, 175, 91  ), T_Pixel( 95 , 155, 67  ), T_Pixel( 79 , 139, 47  ), T_Pixel( 79 , 139, 47  ), T_Pixel( 79 , 139, 47  ), T_Pixel( 79 , 139, 47  ) },
   {  T_Pixel( 163, 223, 131 ), T_Pixel( 163, 223, 131 ), T_Pixel( 151, 215, 11  ), T_Pixel( 123, 183, 83  ), T_Pixel( 107, 163, 63  ), T_Pixel( 107, 163, 63  ), T_Pixel( 107, 163, 63  ), T_Pixel( 107, 163, 63  ) },
   {  T_Pixel( 0  , 135, 139 ), T_Pixel( 0  , 135, 139 ), T_Pixel( 0  , 135, 139 ), T_Pixel( 71 , 111, 163 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 43 , 87 , 143 ) },
   {  T_Pixel( 71 , 111, 163 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 23 , 67 , 119 ), T_Pixel( 7  , 51 , 99  ), T_Pixel( 43 , 31 , 91  ), T_Pixel( 43 , 31 , 91  ), T_Pixel( 43 , 31 , 91  ), T_Pixel( 43 , 31 , 91  ) },
   {  T_Pixel( 103, 139, 187 ), T_Pixel( 71 , 111, 163 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 23 , 67 , 119 ), T_Pixel( 7  , 51 , 99  ), T_Pixel( 7  , 51 , 99  ), T_Pixel( 7  , 51 , 99  ), T_Pixel( 7  , 51 , 99  ) },
   {  T_Pixel( 183, 203, 231 ), T_Pixel( 143, 171, 207 ), T_Pixel( 103, 139, 187 ), T_Pixel( 71 , 111, 163 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 43 , 87 , 143 ) },
   {  T_Pixel( 103, 99 , 255 ), T_Pixel( 19 , 95 , 255 ), T_Pixel( 19 , 95 , 255 ), T_Pixel( 47 , 47 , 199 ), T_Pixel( 27 , 27 , 175 ), T_Pixel( 27 , 27 , 175 ), T_Pixel( 27 , 27 , 175 ), T_Pixel( 27 , 27 , 175 ) },
   {  T_Pixel( 47 , 47 , 199 ), T_Pixel( 27 , 27 , 175 ), T_Pixel( 11 , 11 , 147 ), T_Pixel( 0  , 0  , 123 ), T_Pixel( 0  , 0  , 123 ), T_Pixel( 0  , 0  , 123 ), T_Pixel( 0  , 0  , 123 ), T_Pixel( 0  , 0  , 123 ) },
   {  T_Pixel( 147, 63 , 167 ), T_Pixel( 127, 35 , 147 ), T_Pixel( 111, 15 , 127 ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 51 , 43 , 43  ), T_Pixel( 51 , 43 , 43  ), T_Pixel( 51 , 43 , 43  ), T_Pixel( 51 , 43 , 43  ) },
   {  T_Pixel( 183, 127, 203 ), T_Pixel( 163, 91 , 183 ), T_Pixel( 147, 63 , 167 ), T_Pixel( 127, 35 , 147 ), T_Pixel( 111, 15 , 127 ), T_Pixel( 111, 15 , 127 ), T_Pixel( 111, 15 , 127 ), T_Pixel( 111, 15 , 127 ) },
   {  T_Pixel( 235, 215, 243 ), T_Pixel( 207, 167, 223 ), T_Pixel( 183, 127, 203 ), T_Pixel( 163, 91 , 183 ), T_Pixel( 147, 63 , 167 ), T_Pixel( 147, 63 , 167 ), T_Pixel( 147, 63 , 167 ), T_Pixel( 147, 63 , 167 ) },
   {  T_Pixel( 243, 239, 159 ), T_Pixel( 151, 215, 11  ), T_Pixel( 151, 215, 11  ), T_Pixel( 107, 163, 63  ), T_Pixel( 79 , 123, 31  ), T_Pixel( 79 , 123, 31  ), T_Pixel( 79 , 123, 31  ), T_Pixel( 79 , 123, 31  ) },
   {  T_Pixel( 75 , 71 , 227 ), T_Pixel( 47 , 47 , 199 ), T_Pixel( 27 , 27 , 175 ), T_Pixel( 11 , 11 , 147 ), T_Pixel( 0  , 0  , 123 ), T_Pixel( 0  , 0  , 123 ), T_Pixel( 0  , 0  , 123 ), T_Pixel( 0  , 0  , 123 ) },
   {  T_Pixel( 211, 39 , 103 ), T_Pixel( 199, 7  , 83  ), T_Pixel( 171, 7  , 71  ), T_Pixel( 143, 11 , 63  ), T_Pixel( 111, 7  , 47  ), T_Pixel( 111, 7  , 47  ), T_Pixel( 111, 7  , 47  ), T_Pixel( 111, 7  , 47  ) },
   {  T_Pixel( 239, 127, 159 ), T_Pixel( 227, 79 , 127 ), T_Pixel( 211, 39 , 103 ), T_Pixel( 199, 7  , 83  ), T_Pixel( 171, 7  , 71  ), T_Pixel( 171, 7  , 71  ), T_Pixel( 171, 7  , 71  ), T_Pixel( 171, 7  , 71  ) },
   {  T_Pixel( 255, 227, 207 ), T_Pixel( 255, 175, 195 ), T_Pixel( 239, 127, 159 ), T_Pixel( 227, 79 , 127 ), T_Pixel( 211, 39 , 103 ), T_Pixel( 211, 39 , 103 ), T_Pixel( 211, 39 , 103 ), T_Pixel( 211, 39 , 103 ) },
   {  T_Pixel( 235, 187, 235 ), T_Pixel( 215, 163, 207 ), T_Pixel( 195, 143, 183 ), T_Pixel( 179, 123, 159 ), T_Pixel( 159, 103, 135 ), T_Pixel( 159, 103, 135 ), T_Pixel( 159, 103, 135 ), T_Pixel( 159, 103, 135 ) },
   {  T_Pixel( 103, 99 , 255 ), T_Pixel( 103, 99 , 255 ), T_Pixel( 75 , 71 , 227 ), T_Pixel( 47 , 47 , 199 ), T_Pixel( 47 , 47 , 199 ), T_Pixel( 47 , 47 , 199 ), T_Pixel( 47 , 47 , 199 ), T_Pixel( 47 , 47 , 199 ) },
   {  T_Pixel( 143, 111, 155 ), T_Pixel( 123, 91 , 135 ), T_Pixel( 103, 71 , 115 ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 63 , 55 , 55  ) },
   {  T_Pixel( 163, 135, 175 ), T_Pixel( 143, 111, 155 ), T_Pixel( 123, 91 , 135 ), T_Pixel( 103, 71 , 115 ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 79 , 67 , 67  ) },
   {  T_Pixel( 183, 163, 195 ), T_Pixel( 163, 135, 175 ), T_Pixel( 143, 111, 155 ), T_Pixel( 123, 91 , 135 ), T_Pixel( 103, 71 , 115 ), T_Pixel( 103, 71 , 115 ), T_Pixel( 103, 71 , 115 ), T_Pixel( 103, 71 , 115 ) },
   {  T_Pixel( 203, 199, 239 ), T_Pixel( 175, 171, 223 ), T_Pixel( 151, 143, 207 ), T_Pixel( 135, 115, 191 ), T_Pixel( 103, 71 , 159 ), T_Pixel( 103, 71 , 159 ), T_Pixel( 103, 71 , 159 ), T_Pixel( 103, 71 , 159 ) },
   {  T_Pixel( 63 , 47 , 111 ), T_Pixel( 51 , 39 , 99  ), T_Pixel( 43 , 31 , 91  ), T_Pixel( 43 , 31 , 91  ), T_Pixel( 23 , 19 , 19  ), T_Pixel( 23 , 19 , 19  ), T_Pixel( 23 , 19 , 19  ), T_Pixel( 23 , 19 , 19  ) },
   {  T_Pixel( 135, 123, 123 ), T_Pixel( 107, 95 , 95  ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 23 , 19 , 19  ), T_Pixel( 23 , 19 , 19  ), T_Pixel( 23 , 19 , 19  ), T_Pixel( 23 , 19 , 19  ) },
   {  T_Pixel( 203, 195, 195 ), T_Pixel( 175, 167, 167 ), T_Pixel( 163, 151, 151 ), T_Pixel( 135, 123, 123 ), T_Pixel( 107, 95 , 95  ), T_Pixel( 107, 95 , 95  ), T_Pixel( 107, 95 , 95  ), T_Pixel( 107, 95 , 95  ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 243, 239, 235 ), T_Pixel( 219, 211, 211 ), T_Pixel( 159, 147, 139 ), T_Pixel( 123, 111, 103 ), T_Pixel( 123, 111, 103 ), T_Pixel( 123, 111, 103 ), T_Pixel( 123, 111, 103 ) },
   {  T_Pixel( 247, 231, 231 ), T_Pixel( 219, 187, 187 ), T_Pixel( 171, 119, 119 ), T_Pixel( 135, 75 , 75  ), T_Pixel( 123, 63 , 63  ), T_Pixel( 123, 63 , 63  ), T_Pixel( 123, 63 , 63  ), T_Pixel( 123, 63 , 63  ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 219, 231, 211 ), T_Pixel( 187, 207, 171 ), T_Pixel( 155, 183, 139 ), T_Pixel( 143, 175, 123 ), T_Pixel( 143, 175, 123 ), T_Pixel( 143, 175, 123 ), T_Pixel( 143, 175, 123 ) },
};

template <typename T_Pixel>
const T_Pixel AvatarRemaps<T_Pixel>::s_default_eyes[ AvatarRemaps<T_Pixel>::COUNT_EYES ][ RemapEyes::COUNT ] =
{
   {  T_Pixel( 95 , 0  , 11  ) },
   {  T_Pixel( 187, 91 , 23  ) },
   {  T_Pixel( 247, 223, 135 ) },
   {  T_Pixel( 219, 187, 71  ) },
   {  T_Pixel( 107, 163, 63  ) },
   {  T_Pixel( 103, 139, 187 ) },
   {  T_Pixel( 23 , 67 , 119 ) },
   {  T_Pixel( 123, 91 , 135 ) },
   {  T_Pixel( 139, 127, 87  ) },
   {  T_Pixel( 103, 99 , 255 ) },
   {  T_Pixel( 47 , 47 , 199 ) },
   {  T_Pixel( 11 , 11 , 147 ) },
   {  T_Pixel( 0  , 135, 71  ) },
   {  T_Pixel( 0  , 135, 139 ) },
   {  T_Pixel( 19 , 95 , 255 ) },
   {  T_Pixel( 171, 119, 119 ) },
   {  T_Pixel( 135, 75 , 75  ) },
   {  T_Pixel( 99 , 43 , 43  ) },
   {  T_Pixel( 23 , 19 , 19  ) },
   {  T_Pixel( 239, 127, 159 ) },
   {  T_Pixel( 255, 55 , 55  ) },
   {  T_Pixel( 63 , 19 , 19  ) },
   {  T_Pixel( 199, 203, 231 ) },
   {  T_Pixel( 111, 15 , 127 ) },
   {  T_Pixel( 255, 95 , 27  ) },
   {  T_Pixel( 255, 239, 7   ) },
   {  T_Pixel( 151, 215, 11  ) },
   {  T_Pixel( 107, 95 , 95  ) },
   {  T_Pixel( 143, 175, 123 ) },
   {  T_Pixel( 71 , 55 , 119 ) },
};

template <typename T_Pixel>
const T_Pixel AvatarRemaps<T_Pixel>::s_default_badge[ AvatarRemaps<T_Pixel>::COUNT_BADGE ][ RemapBadge::COUNT ] =
{
   {  T_Pixel( 143, 7  , 7   ) },
   {  T_Pixel( 199, 23 , 23  ) },
   {  T_Pixel( 255, 55 , 55  ) },
   {  T_Pixel( 255, 95 , 27  ) },
   {  T_Pixel( 111, 7  , 47  ) },
   {  T_Pixel( 139, 55 , 0   ) },
   {  T_Pixel( 187, 91 , 23  ) },
   {  T_Pixel( 235, 135, 63  ) },
   {  T_Pixel( 147, 95 , 79  ) },
   {  T_Pixel( 131, 35 , 47  ) },
   {  T_Pixel( 155, 111, 27  ) },
   {  T_Pixel( 243, 215, 91  ) },
   {  T_Pixel( 251, 235, 183 ) },
   {  T_Pixel( 255, 239, 7   ) },
   {  T_Pixel( 175, 151, 79  ) },
   {  T_Pixel( 67 , 103, 19  ) },
   {  T_Pixel( 79 , 139, 47  ) },
   {  T_Pixel( 111, 175, 91  ) },
   {  T_Pixel( 151, 215, 11  ) },
   {  T_Pixel( 0  , 135, 139 ) },
   {  T_Pixel( 23 , 67 , 119 ) },
   {  T_Pixel( 43 , 87 , 143 ) },
   {  T_Pixel( 103, 139, 187 ) },
   {  T_Pixel( 19 , 95 , 255 ) },
   {  T_Pixel( 11 , 11 , 147 ) },
   {  T_Pixel( 111, 15 , 127 ) },
   {  T_Pixel( 147, 63 , 167 ) },
   {  T_Pixel( 183, 127, 203 ) },
   {  T_Pixel( 151, 215, 11  ) },
   {  T_Pixel( 27 , 27 , 175 ) },
   {  T_Pixel( 171, 7  , 71  ) },
   {  T_Pixel( 211, 39 , 103 ) },
   {  T_Pixel( 239, 127, 159 ) },
   {  T_Pixel( 195, 143, 183 ) },
   {  T_Pixel( 75 , 71 , 227 ) },
   {  T_Pixel( 103, 71 , 115 ) },
   {  T_Pixel( 123, 91 , 135 ) },
   {  T_Pixel( 143, 111, 155 ) },
   {  T_Pixel( 151, 143, 207 ) },
   {  T_Pixel( 43 , 31 , 91  ) },
};

template <typename T_Pixel>
const T_Pixel AvatarRemaps<T_Pixel>::s_default_vest[ AvatarRemaps<T_Pixel>::COUNT_VEST ][ RemapVest::COUNT ] =
{
   {  T_Pixel( 255, 55 , 55  ), T_Pixel( 255, 55 , 55  ), T_Pixel( 227, 39 , 39  ), T_Pixel( 199, 23 , 23  ), T_Pixel( 171, 11 , 11  ), T_Pixel( 143, 7  , 7   ), T_Pixel( 99 , 0  , 0   ), T_Pixel( 99 , 0  , 0   ) },
   {  T_Pixel( 231, 211, 215 ), T_Pixel( 211, 163, 175 ), T_Pixel( 191, 123, 135 ), T_Pixel( 171, 91 , 103 ), T_Pixel( 151, 59 , 75  ), T_Pixel( 131, 35 , 47  ), T_Pixel( 115, 15 , 27  ), T_Pixel( 95 , 0  , 11  ) },
   {  T_Pixel( 171, 111, 79  ), T_Pixel( 163, 107, 79  ), T_Pixel( 155, 103, 79  ), T_Pixel( 147, 95 , 79  ), T_Pixel( 143, 95 , 79  ), T_Pixel( 135, 91 , 75  ), T_Pixel( 127, 87 , 75  ), T_Pixel( 123, 83 , 75  ) },
   {  T_Pixel( 255, 227, 207 ), T_Pixel( 247, 195, 155 ), T_Pixel( 243, 163, 107 ), T_Pixel( 235, 135, 63  ), T_Pixel( 211, 111, 39  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 163, 71 , 7   ), T_Pixel( 139, 55 , 0   ) },
   {  T_Pixel( 219, 183, 163 ), T_Pixel( 211, 171, 147 ), T_Pixel( 207, 163, 135 ), T_Pixel( 203, 151, 123 ), T_Pixel( 195, 143, 111 ), T_Pixel( 191, 135, 99  ), T_Pixel( 187, 127, 87  ), T_Pixel( 183, 119, 79  ) },
   {  T_Pixel( 255, 251, 235 ), T_Pixel( 251, 235, 183 ), T_Pixel( 247, 223, 135 ), T_Pixel( 243, 215, 91  ), T_Pixel( 219, 187, 71  ), T_Pixel( 199, 159, 51  ), T_Pixel( 175, 135, 39  ), T_Pixel( 155, 111, 27  ) },
   {  T_Pixel( 175, 151, 79  ), T_Pixel( 159, 139, 63  ), T_Pixel( 147, 127, 51  ), T_Pixel( 135, 115, 39  ), T_Pixel( 119, 107, 27  ), T_Pixel( 107, 95 , 19  ), T_Pixel( 95 , 83 , 11  ), T_Pixel( 83 , 75 , 7   ) },
   {  T_Pixel( 163, 223, 131 ), T_Pixel( 143, 203, 107 ), T_Pixel( 123, 183, 83  ), T_Pixel( 107, 163, 63  ), T_Pixel( 95 , 143, 47  ), T_Pixel( 79 , 123, 31  ), T_Pixel( 67 , 103, 19  ), T_Pixel( 55 , 83 , 11  ) },
   {  T_Pixel( 131, 195, 115 ), T_Pixel( 111, 175, 91  ), T_Pixel( 95 , 155, 67  ), T_Pixel( 79 , 139, 47  ), T_Pixel( 63 , 119, 31  ), T_Pixel( 51 , 99 , 19  ), T_Pixel( 43 , 83 , 11  ), T_Pixel( 39 , 71 , 7   ) },
   {  T_Pixel( 235, 243, 255 ), T_Pixel( 183, 203, 231 ), T_Pixel( 143, 171, 207 ), T_Pixel( 103, 139, 187 ), T_Pixel( 71 , 111, 163 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 23 , 67 , 119 ), T_Pixel( 7  , 51 , 99  ) },
   {  T_Pixel( 231, 235, 255 ), T_Pixel( 219, 223, 247 ), T_Pixel( 199, 203, 231 ), T_Pixel( 179, 187, 219 ), T_Pixel( 163, 171, 207 ), T_Pixel( 143, 151, 187 ), T_Pixel( 123, 131, 167 ), T_Pixel( 103, 115, 151 ) },
   {  T_Pixel( 251, 211, 255 ), T_Pixel( 235, 187, 235 ), T_Pixel( 215, 163, 207 ), T_Pixel( 195, 143, 183 ), T_Pixel( 179, 123, 159 ), T_Pixel( 159, 103, 135 ), T_Pixel( 139, 87 , 111 ), T_Pixel( 123, 71 , 91  ) },
   {  T_Pixel( 239, 231, 243 ), T_Pixel( 231, 219, 235 ), T_Pixel( 207, 191, 215 ), T_Pixel( 183, 163, 195 ), T_Pixel( 163, 135, 175 ), T_Pixel( 143, 111, 155 ), T_Pixel( 123, 91 , 135 ), T_Pixel( 103, 71 , 115 ) },
   {  T_Pixel( 235, 235, 255 ), T_Pixel( 203, 199, 239 ), T_Pixel( 175, 171, 223 ), T_Pixel( 151, 143, 207 ), T_Pixel( 135, 115, 191 ), T_Pixel( 115, 95 , 175 ), T_Pixel( 103, 71 , 159 ), T_Pixel( 91 , 55 , 147 ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 235, 243, 231 ), T_Pixel( 219, 231, 211 ), T_Pixel( 203, 219, 191 ), T_Pixel( 187, 207, 171 ), T_Pixel( 171, 195, 155 ), T_Pixel( 155, 183, 139 ), T_Pixel( 143, 175, 123 ) },
   {  T_Pixel( 255, 175, 195 ), T_Pixel( 239, 127, 159 ), T_Pixel( 227, 79 , 127 ), T_Pixel( 211, 39 , 103 ), T_Pixel( 199, 7  , 83  ), T_Pixel( 171, 7  , 71  ), T_Pixel( 143, 11 , 63  ), T_Pixel( 111, 7  , 47  ) },
   {  T_Pixel( 255, 251, 175 ), T_Pixel( 243, 239, 159 ), T_Pixel( 227, 231, 147 ), T_Pixel( 211, 219, 135 ), T_Pixel( 199, 207, 127 ), T_Pixel( 183, 195, 115 ), T_Pixel( 167, 183, 103 ), T_Pixel( 155, 171, 95  ) },
   {  T_Pixel( 247, 231, 231 ), T_Pixel( 235, 207, 207 ), T_Pixel( 219, 187, 187 ), T_Pixel( 207, 167, 167 ), T_Pixel( 195, 151, 151 ), T_Pixel( 183, 135, 135 ), T_Pixel( 171, 119, 119 ), T_Pixel( 159, 103, 103 ) },
   {  T_Pixel( 147, 87 , 87  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 123, 63 , 63  ), T_Pixel( 111, 51 , 51  ), T_Pixel( 99 , 43 , 43  ), T_Pixel( 87 , 31 , 31  ), T_Pixel( 75 , 23 , 23  ), T_Pixel( 63 , 19 , 19  ) },
   {  T_Pixel( 243, 239, 247 ), T_Pixel( 227, 219, 235 ), T_Pixel( 211, 199, 223 ), T_Pixel( 195, 179, 215 ), T_Pixel( 179, 163, 203 ), T_Pixel( 167, 147, 195 ), T_Pixel( 151, 131, 183 ), T_Pixel( 135, 115, 171 ) },
   {  T_Pixel( 123, 103, 163 ), T_Pixel( 111, 91 , 151 ), T_Pixel( 99 , 79 , 143 ), T_Pixel( 83 , 67 , 131 ), T_Pixel( 71 , 55 , 119 ), T_Pixel( 63 , 47 , 111 ), T_Pixel( 51 , 39 , 99  ), T_Pixel( 43 , 31 , 91  ) },
   {  T_Pixel( 255, 251, 223 ), T_Pixel( 239, 235, 203 ), T_Pixel( 227, 223, 187 ), T_Pixel( 215, 207, 171 ), T_Pixel( 203, 195, 155 ), T_Pixel( 191, 179, 139 ), T_Pixel( 179, 167, 127 ), T_Pixel( 167, 155, 115 ) },
   {  T_Pixel( 155, 143, 99  ), T_Pixel( 139, 127, 87  ), T_Pixel( 127, 115, 75  ), T_Pixel( 115, 103, 67  ), T_Pixel( 103, 91 , 55  ), T_Pixel( 91 , 79 , 47  ), T_Pixel( 79 , 67 , 39  ), T_Pixel( 67 , 55 , 31  ) },
   {  T_Pixel( 247, 247, 247 ), T_Pixel( 231, 227, 227 ), T_Pixel( 219, 211, 211 ), T_Pixel( 203, 195, 195 ), T_Pixel( 191, 179, 179 ), T_Pixel( 175, 167, 167 ), T_Pixel( 163, 151, 151 ), T_Pixel( 147, 135, 135 ) },
   {  T_Pixel( 135, 123, 123 ), T_Pixel( 119, 107, 107 ), T_Pixel( 107, 95 , 95  ), T_Pixel( 91 , 79 , 79  ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 51 , 43 , 43  ), T_Pixel( 23 , 19 , 19  ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 243, 239, 235 ), T_Pixel( 231, 223, 219 ), T_Pixel( 219, 211, 207 ), T_Pixel( 207, 195, 191 ), T_Pixel( 195, 183, 179 ), T_Pixel( 183, 171, 167 ), T_Pixel( 171, 159, 155 ) },
   {  T_Pixel( 159, 147, 139 ), T_Pixel( 147, 135, 127 ), T_Pixel( 135, 123, 115 ), T_Pixel( 123, 111, 103 ), T_Pixel( 115, 103, 95  ), T_Pixel( 103, 91 , 83  ), T_Pixel( 91 , 79 , 71  ), T_Pixel( 83 , 71 , 63  ) },
   {  T_Pixel( 235, 215, 243 ), T_Pixel( 207, 167, 223 ), T_Pixel( 183, 127, 203 ), T_Pixel( 163, 91 , 183 ), T_Pixel( 147, 63 , 167 ), T_Pixel( 127, 35 , 147 ), T_Pixel( 111, 15 , 127 ), T_Pixel( 95 , 0  , 111 ) },
   {  T_Pixel( 103, 99 , 255 ), T_Pixel( 103, 99 , 255 ), T_Pixel( 75 , 71 , 227 ), T_Pixel( 47 , 47 , 199 ), T_Pixel( 27 , 27 , 175 ), T_Pixel( 11 , 11 , 147 ), T_Pixel( 0  , 0  , 123 ), T_Pixel( 0  , 0  , 123 ) },
   {  T_Pixel( 231, 235, 255 ), T_Pixel( 231, 235, 255 ), T_Pixel( 219, 223, 247 ), T_Pixel( 199, 203, 231 ), T_Pixel( 199, 203, 231 ), T_Pixel( 179, 187, 219 ), T_Pixel( 179, 187, 219 ), T_Pixel( 163, 171, 207 ) },
};

template <typename T_Pixel>
const T_Pixel AvatarRemaps<T_Pixel>::s_default_bracers[ AvatarRemaps<T_Pixel>::COUNT_BRACERS ][ RemapBracers::COUNT ] =
{
   {  T_Pixel( 255, 55 , 55  ), T_Pixel( 255, 55 , 55  ), T_Pixel( 227, 39 , 39  ), T_Pixel( 199, 23 , 23  ), T_Pixel( 171, 11 , 11  ), T_Pixel( 143, 7  , 7   ), T_Pixel( 99 , 0  , 0   ), T_Pixel( 99 , 0  , 0   ) },
   {  T_Pixel( 231, 211, 215 ), T_Pixel( 211, 163, 175 ), T_Pixel( 191, 123, 135 ), T_Pixel( 171, 91 , 103 ), T_Pixel( 151, 59 , 75  ), T_Pixel( 131, 35 , 47  ), T_Pixel( 115, 15 , 27  ), T_Pixel( 95 , 0  , 11  ) },
   {  T_Pixel( 171, 111, 79  ), T_Pixel( 163, 107, 79  ), T_Pixel( 155, 103, 79  ), T_Pixel( 147, 95 , 79  ), T_Pixel( 143, 95 , 79  ), T_Pixel( 135, 91 , 75  ), T_Pixel( 127, 87 , 75  ), T_Pixel( 123, 83 , 75  ) },
   {  T_Pixel( 255, 227, 207 ), T_Pixel( 247, 195, 155 ), T_Pixel( 243, 163, 107 ), T_Pixel( 235, 135, 63  ), T_Pixel( 211, 111, 39  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 163, 71 , 7   ), T_Pixel( 139, 55 , 0   ) },
   {  T_Pixel( 219, 183, 163 ), T_Pixel( 211, 171, 147 ), T_Pixel( 207, 163, 135 ), T_Pixel( 203, 151, 123 ), T_Pixel( 195, 143, 111 ), T_Pixel( 191, 135, 99  ), T_Pixel( 187, 127, 87  ), T_Pixel( 183, 119, 79  ) },
   {  T_Pixel( 255, 251, 235 ), T_Pixel( 251, 235, 183 ), T_Pixel( 247, 223, 135 ), T_Pixel( 243, 215, 91  ), T_Pixel( 219, 187, 71  ), T_Pixel( 199, 159, 51  ), T_Pixel( 175, 135, 39  ), T_Pixel( 155, 111, 27  ) },
   {  T_Pixel( 175, 151, 79  ), T_Pixel( 159, 139, 63  ), T_Pixel( 147, 127, 51  ), T_Pixel( 135, 115, 39  ), T_Pixel( 119, 107, 27  ), T_Pixel( 107, 95 , 19  ), T_Pixel( 95 , 83 , 11  ), T_Pixel( 83 , 75 , 7   ) },
   {  T_Pixel( 163, 223, 131 ), T_Pixel( 143, 203, 107 ), T_Pixel( 123, 183, 83  ), T_Pixel( 107, 163, 63  ), T_Pixel( 95 , 143, 47  ), T_Pixel( 79 , 123, 31  ), T_Pixel( 67 , 103, 19  ), T_Pixel( 55 , 83 , 11  ) },
   {  T_Pixel( 131, 195, 115 ), T_Pixel( 111, 175, 91  ), T_Pixel( 95 , 155, 67  ), T_Pixel( 79 , 139, 47  ), T_Pixel( 63 , 119, 31  ), T_Pixel( 51 , 99 , 19  ), T_Pixel( 43 , 83 , 11  ), T_Pixel( 39 , 71 , 7   ) },
   {  T_Pixel( 235, 243, 255 ), T_Pixel( 183, 203, 231 ), T_Pixel( 143, 171, 207 ), T_Pixel( 103, 139, 187 ), T_Pixel( 71 , 111, 163 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 23 , 67 , 119 ), T_Pixel( 7  , 51 , 99  ) },
   {  T_Pixel( 231, 235, 255 ), T_Pixel( 219, 223, 247 ), T_Pixel( 199, 203, 231 ), T_Pixel( 179, 187, 219 ), T_Pixel( 163, 171, 207 ), T_Pixel( 143, 151, 187 ), T_Pixel( 123, 131, 167 ), T_Pixel( 103, 115, 151 ) },
   {  T_Pixel( 251, 211, 255 ), T_Pixel( 235, 187, 235 ), T_Pixel( 215, 163, 207 ), T_Pixel( 195, 143, 183 ), T_Pixel( 179, 123, 159 ), T_Pixel( 159, 103, 135 ), T_Pixel( 139, 87 , 111 ), T_Pixel( 123, 71 , 91  ) },
   {  T_Pixel( 239, 231, 243 ), T_Pixel( 231, 219, 235 ), T_Pixel( 207, 191, 215 ), T_Pixel( 183, 163, 195 ), T_Pixel( 163, 135, 175 ), T_Pixel( 143, 111, 155 ), T_Pixel( 123, 91 , 135 ), T_Pixel( 103, 71 , 115 ) },
   {  T_Pixel( 235, 235, 255 ), T_Pixel( 203, 199, 239 ), T_Pixel( 175, 171, 223 ), T_Pixel( 151, 143, 207 ), T_Pixel( 135, 115, 191 ), T_Pixel( 115, 95 , 175 ), T_Pixel( 103, 71 , 159 ), T_Pixel( 91 , 55 , 147 ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 235, 243, 231 ), T_Pixel( 219, 231, 211 ), T_Pixel( 203, 219, 191 ), T_Pixel( 187, 207, 171 ), T_Pixel( 171, 195, 155 ), T_Pixel( 155, 183, 139 ), T_Pixel( 143, 175, 123 ) },
   {  T_Pixel( 255, 175, 195 ), T_Pixel( 239, 127, 159 ), T_Pixel( 227, 79 , 127 ), T_Pixel( 211, 39 , 103 ), T_Pixel( 199, 7  , 83  ), T_Pixel( 171, 7  , 71  ), T_Pixel( 143, 11 , 63  ), T_Pixel( 111, 7  , 47  ) },
   {  T_Pixel( 255, 251, 175 ), T_Pixel( 243, 239, 159 ), T_Pixel( 227, 231, 147 ), T_Pixel( 211, 219, 135 ), T_Pixel( 199, 207, 127 ), T_Pixel( 183, 195, 115 ), T_Pixel( 167, 183, 103 ), T_Pixel( 155, 171, 95  ) },
   {  T_Pixel( 247, 231, 231 ), T_Pixel( 235, 207, 207 ), T_Pixel( 219, 187, 187 ), T_Pixel( 207, 167, 167 ), T_Pixel( 195, 151, 151 ), T_Pixel( 183, 135, 135 ), T_Pixel( 171, 119, 119 ), T_Pixel( 159, 103, 103 ) },
   {  T_Pixel( 147, 87 , 87  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 123, 63 , 63  ), T_Pixel( 111, 51 , 51  ), T_Pixel( 99 , 43 , 43  ), T_Pixel( 87 , 31 , 31  ), T_Pixel( 75 , 23 , 23  ), T_Pixel( 63 , 19 , 19  ) },
   {  T_Pixel( 243, 239, 247 ), T_Pixel( 227, 219, 235 ), T_Pixel( 211, 199, 223 ), T_Pixel( 195, 179, 215 ), T_Pixel( 179, 163, 203 ), T_Pixel( 167, 147, 195 ), T_Pixel( 151, 131, 183 ), T_Pixel( 135, 115, 171 ) },
   {  T_Pixel( 123, 103, 163 ), T_Pixel( 111, 91 , 151 ), T_Pixel( 99 , 79 , 143 ), T_Pixel( 83 , 67 , 131 ), T_Pixel( 71 , 55 , 119 ), T_Pixel( 63 , 47 , 111 ), T_Pixel( 51 , 39 , 99  ), T_Pixel( 43 , 31 , 91  ) },
   {  T_Pixel( 255, 251, 223 ), T_Pixel( 239, 235, 203 ), T_Pixel( 227, 223, 187 ), T_Pixel( 215, 207, 171 ), T_Pixel( 203, 195, 155 ), T_Pixel( 191, 179, 139 ), T_Pixel( 179, 167, 127 ), T_Pixel( 167, 155, 115 ) },
   {  T_Pixel( 155, 143, 99  ), T_Pixel( 139, 127, 87  ), T_Pixel( 127, 115, 75  ), T_Pixel( 115, 103, 67  ), T_Pixel( 103, 91 , 55  ), T_Pixel( 91 , 79 , 47  ), T_Pixel( 79 , 67 , 39  ), T_Pixel( 67 , 55 , 31  ) },
   {  T_Pixel( 247, 247, 247 ), T_Pixel( 231, 227, 227 ), T_Pixel( 219, 211, 211 ), T_Pixel( 203, 195, 195 ), T_Pixel( 191, 179, 179 ), T_Pixel( 175, 167, 167 ), T_Pixel( 163, 151, 151 ), T_Pixel( 147, 135, 135 ) },
   {  T_Pixel( 135, 123, 123 ), T_Pixel( 119, 107, 107 ), T_Pixel( 107, 95 , 95  ), T_Pixel( 91 , 79 , 79  ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 51 , 43 , 43  ), T_Pixel( 23 , 19 , 19  ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 243, 239, 235 ), T_Pixel( 231, 223, 219 ), T_Pixel( 219, 211, 207 ), T_Pixel( 207, 195, 191 ), T_Pixel( 195, 183, 179 ), T_Pixel( 183, 171, 167 ), T_Pixel( 171, 159, 155 ) },
   {  T_Pixel( 159, 147, 139 ), T_Pixel( 147, 135, 127 ), T_Pixel( 135, 123, 115 ), T_Pixel( 123, 111, 103 ), T_Pixel( 115, 103, 95  ), T_Pixel( 103, 91 , 83  ), T_Pixel( 91 , 79 , 71  ), T_Pixel( 83 , 71 , 63  ) },
   {  T_Pixel( 235, 215, 243 ), T_Pixel( 207, 167, 223 ), T_Pixel( 183, 127, 203 ), T_Pixel( 163, 91 , 183 ), T_Pixel( 147, 63 , 167 ), T_Pixel( 127, 35 , 147 ), T_Pixel( 111, 15 , 127 ), T_Pixel( 95 , 0  , 111 ) },
   {  T_Pixel( 103, 99 , 255 ), T_Pixel( 103, 99 , 255 ), T_Pixel( 75 , 71 , 227 ), T_Pixel( 47 , 47 , 199 ), T_Pixel( 27 , 27 , 175 ), T_Pixel( 11 , 11 , 147 ), T_Pixel( 0  , 0  , 123 ), T_Pixel( 0  , 0  , 123 ) },
   {  T_Pixel( 231, 235, 255 ), T_Pixel( 231, 235, 255 ), T_Pixel( 219, 223, 247 ), T_Pixel( 199, 203, 231 ), T_Pixel( 199, 203, 231 ), T_Pixel( 179, 187, 219 ), T_Pixel( 179, 187, 219 ), T_Pixel( 163, 171, 207 ) },
};

template <typename T_Pixel>
const T_Pixel AvatarRemaps<T_Pixel>::s_default_cape[ AvatarRemaps<T_Pixel>::COUNT_CAPE ][ RemapCape::COUNT ] =
{
   {  T_Pixel( 255, 55 , 55  ), T_Pixel( 255, 55 , 55  ), T_Pixel( 227, 39 , 39  ), T_Pixel( 199, 23 , 23  ), T_Pixel( 171, 11 , 11  ), T_Pixel( 143, 7  , 7   ), T_Pixel( 99 , 0  , 0   ), T_Pixel( 99 , 0  , 0   ) },
   {  T_Pixel( 231, 211, 215 ), T_Pixel( 211, 163, 175 ), T_Pixel( 191, 123, 135 ), T_Pixel( 171, 91 , 103 ), T_Pixel( 151, 59 , 75  ), T_Pixel( 131, 35 , 47  ), T_Pixel( 115, 15 , 27  ), T_Pixel( 95 , 0  , 11  ) },
   {  T_Pixel( 171, 111, 79  ), T_Pixel( 163, 107, 79  ), T_Pixel( 155, 103, 79  ), T_Pixel( 147, 95 , 79  ), T_Pixel( 143, 95 , 79  ), T_Pixel( 135, 91 , 75  ), T_Pixel( 127, 87 , 75  ), T_Pixel( 123, 83 , 75  ) },
   {  T_Pixel( 255, 227, 207 ), T_Pixel( 247, 195, 155 ), T_Pixel( 243, 163, 107 ), T_Pixel( 235, 135, 63  ), T_Pixel( 211, 111, 39  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 163, 71 , 7   ), T_Pixel( 139, 55 , 0   ) },
   {  T_Pixel( 219, 183, 163 ), T_Pixel( 211, 171, 147 ), T_Pixel( 207, 163, 135 ), T_Pixel( 203, 151, 123 ), T_Pixel( 195, 143, 111 ), T_Pixel( 191, 135, 99  ), T_Pixel( 187, 127, 87  ), T_Pixel( 183, 119, 79  ) },
   {  T_Pixel( 255, 251, 235 ), T_Pixel( 251, 235, 183 ), T_Pixel( 247, 223, 135 ), T_Pixel( 243, 215, 91  ), T_Pixel( 219, 187, 71  ), T_Pixel( 199, 159, 51  ), T_Pixel( 175, 135, 39  ), T_Pixel( 155, 111, 27  ) },
   {  T_Pixel( 175, 151, 79  ), T_Pixel( 159, 139, 63  ), T_Pixel( 147, 127, 51  ), T_Pixel( 135, 115, 39  ), T_Pixel( 119, 107, 27  ), T_Pixel( 107, 95 , 19  ), T_Pixel( 95 , 83 , 11  ), T_Pixel( 83 , 75 , 7   ) },
   {  T_Pixel( 163, 223, 131 ), T_Pixel( 143, 203, 107 ), T_Pixel( 123, 183, 83  ), T_Pixel( 107, 163, 63  ), T_Pixel( 95 , 143, 47  ), T_Pixel( 79 , 123, 31  ), T_Pixel( 67 , 103, 19  ), T_Pixel( 55 , 83 , 11  ) },
   {  T_Pixel( 131, 195, 115 ), T_Pixel( 111, 175, 91  ), T_Pixel( 95 , 155, 67  ), T_Pixel( 79 , 139, 47  ), T_Pixel( 63 , 119, 31  ), T_Pixel( 51 , 99 , 19  ), T_Pixel( 43 , 83 , 11  ), T_Pixel( 39 , 71 , 7   ) },
   {  T_Pixel( 235, 243, 255 ), T_Pixel( 183, 203, 231 ), T_Pixel( 143, 171, 207 ), T_Pixel( 103, 139, 187 ), T_Pixel( 71 , 111, 163 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 23 , 67 , 119 ), T_Pixel( 7  , 51 , 99  ) },
   {  T_Pixel( 231, 235, 255 ), T_Pixel( 219, 223, 247 ), T_Pixel( 199, 203, 231 ), T_Pixel( 179, 187, 219 ), T_Pixel( 163, 171, 207 ), T_Pixel( 143, 151, 187 ), T_Pixel( 123, 131, 167 ), T_Pixel( 103, 115, 151 ) },
   {  T_Pixel( 251, 211, 255 ), T_Pixel( 235, 187, 235 ), T_Pixel( 215, 163, 207 ), T_Pixel( 195, 143, 183 ), T_Pixel( 179, 123, 159 ), T_Pixel( 159, 103, 135 ), T_Pixel( 139, 87 , 111 ), T_Pixel( 123, 71 , 91  ) },
   {  T_Pixel( 239, 231, 243 ), T_Pixel( 231, 219, 235 ), T_Pixel( 207, 191, 215 ), T_Pixel( 183, 163, 195 ), T_Pixel( 163, 135, 175 ), T_Pixel( 143, 111, 155 ), T_Pixel( 123, 91 , 135 ), T_Pixel( 103, 71 , 115 ) },
   {  T_Pixel( 235, 235, 255 ), T_Pixel( 203, 199, 239 ), T_Pixel( 175, 171, 223 ), T_Pixel( 151, 143, 207 ), T_Pixel( 135, 115, 191 ), T_Pixel( 115, 95 , 175 ), T_Pixel( 103, 71 , 159 ), T_Pixel( 91 , 55 , 147 ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 235, 243, 231 ), T_Pixel( 219, 231, 211 ), T_Pixel( 203, 219, 191 ), T_Pixel( 187, 207, 171 ), T_Pixel( 171, 195, 155 ), T_Pixel( 155, 183, 139 ), T_Pixel( 143, 175, 123 ) },
   {  T_Pixel( 255, 175, 195 ), T_Pixel( 239, 127, 159 ), T_Pixel( 227, 79 , 127 ), T_Pixel( 211, 39 , 103 ), T_Pixel( 199, 7  , 83  ), T_Pixel( 171, 7  , 71  ), T_Pixel( 143, 11 , 63  ), T_Pixel( 111, 7  , 47  ) },
   {  T_Pixel( 255, 251, 175 ), T_Pixel( 243, 239, 159 ), T_Pixel( 227, 231, 147 ), T_Pixel( 211, 219, 135 ), T_Pixel( 199, 207, 127 ), T_Pixel( 183, 195, 115 ), T_Pixel( 167, 183, 103 ), T_Pixel( 155, 171, 95  ) },
   {  T_Pixel( 247, 231, 231 ), T_Pixel( 235, 207, 207 ), T_Pixel( 219, 187, 187 ), T_Pixel( 207, 167, 167 ), T_Pixel( 195, 151, 151 ), T_Pixel( 183, 135, 135 ), T_Pixel( 171, 119, 119 ), T_Pixel( 159, 103, 103 ) },
   {  T_Pixel( 147, 87 , 87  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 123, 63 , 63  ), T_Pixel( 111, 51 , 51  ), T_Pixel( 99 , 43 , 43  ), T_Pixel( 87 , 31 , 31  ), T_Pixel( 75 , 23 , 23  ), T_Pixel( 63 , 19 , 19  ) },
   {  T_Pixel( 243, 239, 247 ), T_Pixel( 227, 219, 235 ), T_Pixel( 211, 199, 223 ), T_Pixel( 195, 179, 215 ), T_Pixel( 179, 163, 203 ), T_Pixel( 167, 147, 195 ), T_Pixel( 151, 131, 183 ), T_Pixel( 135, 115, 171 ) },
   {  T_Pixel( 123, 103, 163 ), T_Pixel( 111, 91 , 151 ), T_Pixel( 99 , 79 , 143 ), T_Pixel( 83 , 67 , 131 ), T_Pixel( 71 , 55 , 119 ), T_Pixel( 63 , 47 , 111 ), T_Pixel( 51 , 39 , 99  ), T_Pixel( 43 , 31 , 91  ) },
   {  T_Pixel( 255, 251, 223 ), T_Pixel( 239, 235, 203 ), T_Pixel( 227, 223, 187 ), T_Pixel( 215, 207, 171 ), T_Pixel( 203, 195, 155 ), T_Pixel( 191, 179, 139 ), T_Pixel( 179, 167, 127 ), T_Pixel( 167, 155, 115 ) },
   {  T_Pixel( 155, 143, 99  ), T_Pixel( 139, 127, 87  ), T_Pixel( 127, 115, 75  ), T_Pixel( 115, 103, 67  ), T_Pixel( 103, 91 , 55  ), T_Pixel( 91 , 79 , 47  ), T_Pixel( 79 , 67 , 39  ), T_Pixel( 67 , 55 , 31  ) },
   {  T_Pixel( 247, 247, 247 ), T_Pixel( 231, 227, 227 ), T_Pixel( 219, 211, 211 ), T_Pixel( 203, 195, 195 ), T_Pixel( 191, 179, 179 ), T_Pixel( 175, 167, 167 ), T_Pixel( 163, 151, 151 ), T_Pixel( 147, 135, 135 ) },
   {  T_Pixel( 135, 123, 123 ), T_Pixel( 119, 107, 107 ), T_Pixel( 107, 95 , 95  ), T_Pixel( 91 , 79 , 79  ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 51 , 43 , 43  ), T_Pixel( 23 , 19 , 19  ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 243, 239, 235 ), T_Pixel( 231, 223, 219 ), T_Pixel( 219, 211, 207 ), T_Pixel( 207, 195, 191 ), T_Pixel( 195, 183, 179 ), T_Pixel( 183, 171, 167 ), T_Pixel( 171, 159, 155 ) },
   {  T_Pixel( 159, 147, 139 ), T_Pixel( 147, 135, 127 ), T_Pixel( 135, 123, 115 ), T_Pixel( 123, 111, 103 ), T_Pixel( 115, 103, 95  ), T_Pixel( 103, 91 , 83  ), T_Pixel( 91 , 79 , 71  ), T_Pixel( 83 , 71 , 63  ) },
   {  T_Pixel( 235, 215, 243 ), T_Pixel( 207, 167, 223 ), T_Pixel( 183, 127, 203 ), T_Pixel( 163, 91 , 183 ), T_Pixel( 147, 63 , 167 ), T_Pixel( 127, 35 , 147 ), T_Pixel( 111, 15 , 127 ), T_Pixel( 95 , 0  , 111 ) },
   {  T_Pixel( 103, 99 , 255 ), T_Pixel( 103, 99 , 255 ), T_Pixel( 75 , 71 , 227 ), T_Pixel( 47 , 47 , 199 ), T_Pixel( 27 , 27 , 175 ), T_Pixel( 11 , 11 , 147 ), T_Pixel( 0  , 0  , 123 ), T_Pixel( 0  , 0  , 123 ) },
   {  T_Pixel( 231, 235, 255 ), T_Pixel( 231, 235, 255 ), T_Pixel( 219, 223, 247 ), T_Pixel( 199, 203, 231 ), T_Pixel( 199, 203, 231 ), T_Pixel( 179, 187, 219 ), T_Pixel( 179, 187, 219 ), T_Pixel( 163, 171, 207 ) },
};

template <typename T_Pixel>
const T_Pixel AvatarRemaps<T_Pixel>::s_default_boots[ AvatarRemaps<T_Pixel>::COUNT_BOOTS ][ RemapBoots::COUNT ] =
{
   {  T_Pixel( 255, 55 , 55  ), T_Pixel( 255, 55 , 55  ), T_Pixel( 227, 39 , 39  ), T_Pixel( 199, 23 , 23  ), T_Pixel( 171, 11 , 11  ), T_Pixel( 143, 7  , 7   ), T_Pixel( 99 , 0  , 0   ), T_Pixel( 99 , 0  , 0   ) },
   {  T_Pixel( 231, 211, 215 ), T_Pixel( 211, 163, 175 ), T_Pixel( 191, 123, 135 ), T_Pixel( 171, 91 , 103 ), T_Pixel( 151, 59 , 75  ), T_Pixel( 131, 35 , 47  ), T_Pixel( 115, 15 , 27  ), T_Pixel( 95 , 0  , 11  ) },
   {  T_Pixel( 171, 111, 79  ), T_Pixel( 163, 107, 79  ), T_Pixel( 155, 103, 79  ), T_Pixel( 147, 95 , 79  ), T_Pixel( 143, 95 , 79  ), T_Pixel( 135, 91 , 75  ), T_Pixel( 127, 87 , 75  ), T_Pixel( 123, 83 , 75  ) },
   {  T_Pixel( 255, 227, 207 ), T_Pixel( 247, 195, 155 ), T_Pixel( 243, 163, 107 ), T_Pixel( 235, 135, 63  ), T_Pixel( 211, 111, 39  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 163, 71 , 7   ), T_Pixel( 139, 55 , 0   ) },
   {  T_Pixel( 219, 183, 163 ), T_Pixel( 211, 171, 147 ), T_Pixel( 207, 163, 135 ), T_Pixel( 203, 151, 123 ), T_Pixel( 195, 143, 111 ), T_Pixel( 191, 135, 99  ), T_Pixel( 187, 127, 87  ), T_Pixel( 183, 119, 79  ) },
   {  T_Pixel( 255, 251, 235 ), T_Pixel( 251, 235, 183 ), T_Pixel( 247, 223, 135 ), T_Pixel( 243, 215, 91  ), T_Pixel( 219, 187, 71  ), T_Pixel( 199, 159, 51  ), T_Pixel( 175, 135, 39  ), T_Pixel( 155, 111, 27  ) },
   {  T_Pixel( 175, 151, 79  ), T_Pixel( 159, 139, 63  ), T_Pixel( 147, 127, 51  ), T_Pixel( 135, 115, 39  ), T_Pixel( 119, 107, 27  ), T_Pixel( 107, 95 , 19  ), T_Pixel( 95 , 83 , 11  ), T_Pixel( 83 , 75 , 7   ) },
   {  T_Pixel( 163, 223, 131 ), T_Pixel( 143, 203, 107 ), T_Pixel( 123, 183, 83  ), T_Pixel( 107, 163, 63  ), T_Pixel( 95 , 143, 47  ), T_Pixel( 79 , 123, 31  ), T_Pixel( 67 , 103, 19  ), T_Pixel( 55 , 83 , 11  ) },
   {  T_Pixel( 131, 195, 115 ), T_Pixel( 111, 175, 91  ), T_Pixel( 95 , 155, 67  ), T_Pixel( 79 , 139, 47  ), T_Pixel( 63 , 119, 31  ), T_Pixel( 51 , 99 , 19  ), T_Pixel( 43 , 83 , 11  ), T_Pixel( 39 , 71 , 7   ) },
   {  T_Pixel( 235, 243, 255 ), T_Pixel( 183, 203, 231 ), T_Pixel( 143, 171, 207 ), T_Pixel( 103, 139, 187 ), T_Pixel( 71 , 111, 163 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 23 , 67 , 119 ), T_Pixel( 7  , 51 , 99  ) },
   {  T_Pixel( 231, 235, 255 ), T_Pixel( 219, 223, 247 ), T_Pixel( 199, 203, 231 ), T_Pixel( 179, 187, 219 ), T_Pixel( 163, 171, 207 ), T_Pixel( 143, 151, 187 ), T_Pixel( 123, 131, 167 ), T_Pixel( 103, 115, 151 ) },
   {  T_Pixel( 251, 211, 255 ), T_Pixel( 235, 187, 235 ), T_Pixel( 215, 163, 207 ), T_Pixel( 195, 143, 183 ), T_Pixel( 179, 123, 159 ), T_Pixel( 159, 103, 135 ), T_Pixel( 139, 87 , 111 ), T_Pixel( 123, 71 , 91  ) },
   {  T_Pixel( 239, 231, 243 ), T_Pixel( 231, 219, 235 ), T_Pixel( 207, 191, 215 ), T_Pixel( 183, 163, 195 ), T_Pixel( 163, 135, 175 ), T_Pixel( 143, 111, 155 ), T_Pixel( 123, 91 , 135 ), T_Pixel( 103, 71 , 115 ) },
   {  T_Pixel( 235, 235, 255 ), T_Pixel( 203, 199, 239 ), T_Pixel( 175, 171, 223 ), T_Pixel( 151, 143, 207 ), T_Pixel( 135, 115, 191 ), T_Pixel( 115, 95 , 175 ), T_Pixel( 103, 71 , 159 ), T_Pixel( 91 , 55 , 147 ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 235, 243, 231 ), T_Pixel( 219, 231, 211 ), T_Pixel( 203, 219, 191 ), T_Pixel( 187, 207, 171 ), T_Pixel( 171, 195, 155 ), T_Pixel( 155, 183, 139 ), T_Pixel( 143, 175, 123 ) },
   {  T_Pixel( 255, 175, 195 ), T_Pixel( 239, 127, 159 ), T_Pixel( 227, 79 , 127 ), T_Pixel( 211, 39 , 103 ), T_Pixel( 199, 7  , 83  ), T_Pixel( 171, 7  , 71  ), T_Pixel( 143, 11 , 63  ), T_Pixel( 111, 7  , 47  ) },
   {  T_Pixel( 255, 251, 175 ), T_Pixel( 243, 239, 159 ), T_Pixel( 227, 231, 147 ), T_Pixel( 211, 219, 135 ), T_Pixel( 199, 207, 127 ), T_Pixel( 183, 195, 115 ), T_Pixel( 167, 183, 103 ), T_Pixel( 155, 171, 95  ) },
   {  T_Pixel( 247, 231, 231 ), T_Pixel( 235, 207, 207 ), T_Pixel( 219, 187, 187 ), T_Pixel( 207, 167, 167 ), T_Pixel( 195, 151, 151 ), T_Pixel( 183, 135, 135 ), T_Pixel( 171, 119, 119 ), T_Pixel( 159, 103, 103 ) },
   {  T_Pixel( 147, 87 , 87  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 123, 63 , 63  ), T_Pixel( 111, 51 , 51  ), T_Pixel( 99 , 43 , 43  ), T_Pixel( 87 , 31 , 31  ), T_Pixel( 75 , 23 , 23  ), T_Pixel( 63 , 19 , 19  ) },
   {  T_Pixel( 243, 239, 247 ), T_Pixel( 227, 219, 235 ), T_Pixel( 211, 199, 223 ), T_Pixel( 195, 179, 215 ), T_Pixel( 179, 163, 203 ), T_Pixel( 167, 147, 195 ), T_Pixel( 151, 131, 183 ), T_Pixel( 135, 115, 171 ) },
   {  T_Pixel( 123, 103, 163 ), T_Pixel( 111, 91 , 151 ), T_Pixel( 99 , 79 , 143 ), T_Pixel( 83 , 67 , 131 ), T_Pixel( 71 , 55 , 119 ), T_Pixel( 63 , 47 , 111 ), T_Pixel( 51 , 39 , 99  ), T_Pixel( 43 , 31 , 91  ) },
   {  T_Pixel( 255, 251, 223 ), T_Pixel( 239, 235, 203 ), T_Pixel( 227, 223, 187 ), T_Pixel( 215, 207, 171 ), T_Pixel( 203, 195, 155 ), T_Pixel( 191, 179, 139 ), T_Pixel( 179, 167, 127 ), T_Pixel( 167, 155, 115 ) },
   {  T_Pixel( 155, 143, 99  ), T_Pixel( 139, 127, 87  ), T_Pixel( 127, 115, 75  ), T_Pixel( 115, 103, 67  ), T_Pixel( 103, 91 , 55  ), T_Pixel( 91 , 79 , 47  ), T_Pixel( 79 , 67 , 39  ), T_Pixel( 67 , 55 , 31  ) },
   {  T_Pixel( 247, 247, 247 ), T_Pixel( 231, 227, 227 ), T_Pixel( 219, 211, 211 ), T_Pixel( 203, 195, 195 ), T_Pixel( 191, 179, 179 ), T_Pixel( 175, 167, 167 ), T_Pixel( 163, 151, 151 ), T_Pixel( 147, 135, 135 ) },
   {  T_Pixel( 135, 123, 123 ), T_Pixel( 119, 107, 107 ), T_Pixel( 107, 95 , 95  ), T_Pixel( 91 , 79 , 79  ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 51 , 43 , 43  ), T_Pixel( 23 , 19 , 19  ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 243, 239, 235 ), T_Pixel( 231, 223, 219 ), T_Pixel( 219, 211, 207 ), T_Pixel( 207, 195, 191 ), T_Pixel( 195, 183, 179 ), T_Pixel( 183, 171, 167 ), T_Pixel( 171, 159, 155 ) },
   {  T_Pixel( 159, 147, 139 ), T_Pixel( 147, 135, 127 ), T_Pixel( 135, 123, 115 ), T_Pixel( 123, 111, 103 ), T_Pixel( 115, 103, 95  ), T_Pixel( 103, 91 , 83  ), T_Pixel( 91 , 79 , 71  ), T_Pixel( 83 , 71 , 63  ) },
   {  T_Pixel( 235, 215, 243 ), T_Pixel( 207, 167, 223 ), T_Pixel( 183, 127, 203 ), T_Pixel( 163, 91 , 183 ), T_Pixel( 147, 63 , 167 ), T_Pixel( 127, 35 , 147 ), T_Pixel( 111, 15 , 127 ), T_Pixel( 95 , 0  , 111 ) },
   {  T_Pixel( 103, 99 , 255 ), T_Pixel( 103, 99 , 255 ), T_Pixel( 75 , 71 , 227 ), T_Pixel( 47 , 47 , 199 ), T_Pixel( 27 , 27 , 175 ), T_Pixel( 11 , 11 , 147 ), T_Pixel( 0  , 0  , 123 ), T_Pixel( 0  , 0  , 123 ) },
   {  T_Pixel( 231, 235, 255 ), T_Pixel( 231, 235, 255 ), T_Pixel( 219, 223, 247 ), T_Pixel( 199, 203, 231 ), T_Pixel( 199, 203, 231 ), T_Pixel( 179, 187, 219 ), T_Pixel( 179, 187, 219 ), T_Pixel( 163, 171, 207 ) },
};

template <typename T_Pixel>
const T_Pixel AvatarRemaps<T_Pixel>::s_default_trousers[ AvatarRemaps<T_Pixel>::COUNT_TROUSERS ][ RemapTrousers::COUNT ] =
{
   {  T_Pixel( 255, 55 , 55  ), T_Pixel( 255, 55 , 55  ), T_Pixel( 227, 39 , 39  ), T_Pixel( 199, 23 , 23  ), T_Pixel( 171, 11 , 11  ), T_Pixel( 143, 7  , 7   ), T_Pixel( 99 , 0  , 0   ), T_Pixel( 99 , 0  , 0   ) },
   {  T_Pixel( 231, 211, 215 ), T_Pixel( 211, 163, 175 ), T_Pixel( 191, 123, 135 ), T_Pixel( 171, 91 , 103 ), T_Pixel( 151, 59 , 75  ), T_Pixel( 131, 35 , 47  ), T_Pixel( 115, 15 , 27  ), T_Pixel( 95 , 0  , 11  ) },
   {  T_Pixel( 171, 111, 79  ), T_Pixel( 163, 107, 79  ), T_Pixel( 155, 103, 79  ), T_Pixel( 147, 95 , 79  ), T_Pixel( 143, 95 , 79  ), T_Pixel( 135, 91 , 75  ), T_Pixel( 127, 87 , 75  ), T_Pixel( 123, 83 , 75  ) },
   {  T_Pixel( 255, 227, 207 ), T_Pixel( 247, 195, 155 ), T_Pixel( 243, 163, 107 ), T_Pixel( 235, 135, 63  ), T_Pixel( 211, 111, 39  ), T_Pixel( 187, 91 , 23  ), T_Pixel( 163, 71 , 7   ), T_Pixel( 139, 55 , 0   ) },
   {  T_Pixel( 219, 183, 163 ), T_Pixel( 211, 171, 147 ), T_Pixel( 207, 163, 135 ), T_Pixel( 203, 151, 123 ), T_Pixel( 195, 143, 111 ), T_Pixel( 191, 135, 99  ), T_Pixel( 187, 127, 87  ), T_Pixel( 183, 119, 79  ) },
   {  T_Pixel( 255, 251, 235 ), T_Pixel( 251, 235, 183 ), T_Pixel( 247, 223, 135 ), T_Pixel( 243, 215, 91  ), T_Pixel( 219, 187, 71  ), T_Pixel( 199, 159, 51  ), T_Pixel( 175, 135, 39  ), T_Pixel( 155, 111, 27  ) },
   {  T_Pixel( 175, 151, 79  ), T_Pixel( 159, 139, 63  ), T_Pixel( 147, 127, 51  ), T_Pixel( 135, 115, 39  ), T_Pixel( 119, 107, 27  ), T_Pixel( 107, 95 , 19  ), T_Pixel( 95 , 83 , 11  ), T_Pixel( 83 , 75 , 7   ) },
   {  T_Pixel( 163, 223, 131 ), T_Pixel( 143, 203, 107 ), T_Pixel( 123, 183, 83  ), T_Pixel( 107, 163, 63  ), T_Pixel( 95 , 143, 47  ), T_Pixel( 79 , 123, 31  ), T_Pixel( 67 , 103, 19  ), T_Pixel( 55 , 83 , 11  ) },
   {  T_Pixel( 131, 195, 115 ), T_Pixel( 111, 175, 91  ), T_Pixel( 95 , 155, 67  ), T_Pixel( 79 , 139, 47  ), T_Pixel( 63 , 119, 31  ), T_Pixel( 51 , 99 , 19  ), T_Pixel( 43 , 83 , 11  ), T_Pixel( 39 , 71 , 7   ) },
   {  T_Pixel( 235, 243, 255 ), T_Pixel( 183, 203, 231 ), T_Pixel( 143, 171, 207 ), T_Pixel( 103, 139, 187 ), T_Pixel( 71 , 111, 163 ), T_Pixel( 43 , 87 , 143 ), T_Pixel( 23 , 67 , 119 ), T_Pixel( 7  , 51 , 99  ) },
   {  T_Pixel( 231, 235, 255 ), T_Pixel( 219, 223, 247 ), T_Pixel( 199, 203, 231 ), T_Pixel( 179, 187, 219 ), T_Pixel( 163, 171, 207 ), T_Pixel( 143, 151, 187 ), T_Pixel( 123, 131, 167 ), T_Pixel( 103, 115, 151 ) },
   {  T_Pixel( 251, 211, 255 ), T_Pixel( 235, 187, 235 ), T_Pixel( 215, 163, 207 ), T_Pixel( 195, 143, 183 ), T_Pixel( 179, 123, 159 ), T_Pixel( 159, 103, 135 ), T_Pixel( 139, 87 , 111 ), T_Pixel( 123, 71 , 91  ) },
   {  T_Pixel( 239, 231, 243 ), T_Pixel( 231, 219, 235 ), T_Pixel( 207, 191, 215 ), T_Pixel( 183, 163, 195 ), T_Pixel( 163, 135, 175 ), T_Pixel( 143, 111, 155 ), T_Pixel( 123, 91 , 135 ), T_Pixel( 103, 71 , 115 ) },
   {  T_Pixel( 235, 235, 255 ), T_Pixel( 203, 199, 239 ), T_Pixel( 175, 171, 223 ), T_Pixel( 151, 143, 207 ), T_Pixel( 135, 115, 191 ), T_Pixel( 115, 95 , 175 ), T_Pixel( 103, 71 , 159 ), T_Pixel( 91 , 55 , 147 ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 235, 243, 231 ), T_Pixel( 219, 231, 211 ), T_Pixel( 203, 219, 191 ), T_Pixel( 187, 207, 171 ), T_Pixel( 171, 195, 155 ), T_Pixel( 155, 183, 139 ), T_Pixel( 143, 175, 123 ) },
   {  T_Pixel( 255, 175, 195 ), T_Pixel( 239, 127, 159 ), T_Pixel( 227, 79 , 127 ), T_Pixel( 211, 39 , 103 ), T_Pixel( 199, 7  , 83  ), T_Pixel( 171, 7  , 71  ), T_Pixel( 143, 11 , 63  ), T_Pixel( 111, 7  , 47  ) },
   {  T_Pixel( 255, 251, 175 ), T_Pixel( 243, 239, 159 ), T_Pixel( 227, 231, 147 ), T_Pixel( 211, 219, 135 ), T_Pixel( 199, 207, 127 ), T_Pixel( 183, 195, 115 ), T_Pixel( 167, 183, 103 ), T_Pixel( 155, 171, 95  ) },
   {  T_Pixel( 247, 231, 231 ), T_Pixel( 235, 207, 207 ), T_Pixel( 219, 187, 187 ), T_Pixel( 207, 167, 167 ), T_Pixel( 195, 151, 151 ), T_Pixel( 183, 135, 135 ), T_Pixel( 171, 119, 119 ), T_Pixel( 159, 103, 103 ) },
   {  T_Pixel( 147, 87 , 87  ), T_Pixel( 135, 75 , 75  ), T_Pixel( 123, 63 , 63  ), T_Pixel( 111, 51 , 51  ), T_Pixel( 99 , 43 , 43  ), T_Pixel( 87 , 31 , 31  ), T_Pixel( 75 , 23 , 23  ), T_Pixel( 63 , 19 , 19  ) },
   {  T_Pixel( 243, 239, 247 ), T_Pixel( 227, 219, 235 ), T_Pixel( 211, 199, 223 ), T_Pixel( 195, 179, 215 ), T_Pixel( 179, 163, 203 ), T_Pixel( 167, 147, 195 ), T_Pixel( 151, 131, 183 ), T_Pixel( 135, 115, 171 ) },
   {  T_Pixel( 123, 103, 163 ), T_Pixel( 111, 91 , 151 ), T_Pixel( 99 , 79 , 143 ), T_Pixel( 83 , 67 , 131 ), T_Pixel( 71 , 55 , 119 ), T_Pixel( 63 , 47 , 111 ), T_Pixel( 51 , 39 , 99  ), T_Pixel( 43 , 31 , 91  ) },
   {  T_Pixel( 255, 251, 223 ), T_Pixel( 239, 235, 203 ), T_Pixel( 227, 223, 187 ), T_Pixel( 215, 207, 171 ), T_Pixel( 203, 195, 155 ), T_Pixel( 191, 179, 139 ), T_Pixel( 179, 167, 127 ), T_Pixel( 167, 155, 115 ) },
   {  T_Pixel( 155, 143, 99  ), T_Pixel( 139, 127, 87  ), T_Pixel( 127, 115, 75  ), T_Pixel( 115, 103, 67  ), T_Pixel( 103, 91 , 55  ), T_Pixel( 91 , 79 , 47  ), T_Pixel( 79 , 67 , 39  ), T_Pixel( 67 , 55 , 31  ) },
   {  T_Pixel( 247, 247, 247 ), T_Pixel( 231, 227, 227 ), T_Pixel( 219, 211, 211 ), T_Pixel( 203, 195, 195 ), T_Pixel( 191, 179, 179 ), T_Pixel( 175, 167, 167 ), T_Pixel( 163, 151, 151 ), T_Pixel( 147, 135, 135 ) },
   {  T_Pixel( 135, 123, 123 ), T_Pixel( 119, 107, 107 ), T_Pixel( 107, 95 , 95  ), T_Pixel( 91 , 79 , 79  ), T_Pixel( 79 , 67 , 67  ), T_Pixel( 63 , 55 , 55  ), T_Pixel( 51 , 43 , 43  ), T_Pixel( 23 , 19 , 19  ) },
   {  T_Pixel( 255, 255, 255 ), T_Pixel( 243, 239, 235 ), T_Pixel( 231, 223, 219 ), T_Pixel( 219, 211, 207 ), T_Pixel( 207, 195, 191 ), T_Pixel( 195, 183, 179 ), T_Pixel( 183, 171, 167 ), T_Pixel( 171, 159, 155 ) },
   {  T_Pixel( 159, 147, 139 ), T_Pixel( 147, 135, 127 ), T_Pixel( 135, 123, 115 ), T_Pixel( 123, 111, 103 ), T_Pixel( 115, 103, 95  ), T_Pixel( 103, 91 , 83  ), T_Pixel( 91 , 79 , 71  ), T_Pixel( 83 , 71 , 63  ) },
   {  T_Pixel( 235, 215, 243 ), T_Pixel( 207, 167, 223 ), T_Pixel( 183, 127, 203 ), T_Pixel( 163, 91 , 183 ), T_Pixel( 147, 63 , 167 ), T_Pixel( 127, 35 , 147 ), T_Pixel( 111, 15 , 127 ), T_Pixel( 95 , 0  , 111 ) },
   {  T_Pixel( 103, 99 , 255 ), T_Pixel( 103, 99 , 255 ), T_Pixel( 75 , 71 , 227 ), T_Pixel( 47 , 47 , 199 ), T_Pixel( 27 , 27 , 175 ), T_Pixel( 11 , 11 , 147 ), T_Pixel( 0  , 0  , 123 ), T_Pixel( 0  , 0  , 123 ) },
   {  T_Pixel( 231, 235, 255 ), T_Pixel( 231, 235, 255 ), T_Pixel( 219, 223, 247 ), T_Pixel( 199, 203, 231 ), T_Pixel( 199, 203, 231 ), T_Pixel( 179, 187, 219 ), T_Pixel( 179, 187, 219 ), T_Pixel( 163, 171, 207 ) },
};

} // namespace yiffurc

/** \} colors */
/** \} cpp */

#endif
