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

#ifndef _YIFFURC_COMMON_H__
#define _YIFFURC_COMMON_H__

#ifdef BUILD_YIFFURC
   #ifndef YIFFURC_STATIC  // DSO/DLL
      #ifndef _WIN32
         #define YIFFURC_LINK extern "C" __attribute__((visibility( "default" )))
      #else
         #define YIFFURC_LINK extern "C" __declspec( dllexport )
      #endif
   #endif
#else
   #ifndef YIFFURC_STATIC // DSO/DLL
      #ifdef _WIN32
         #ifdef __cplusplus
            #define YIFFURC_LINK extern "C" __declspec( dllimport )
         #else
            #define YIFFURC_LINK __declspec( dllimport )
         #endif
      #endif
   #endif
#endif

#ifndef YIFFURC_LINK
   #ifdef __cplusplus
      #define YIFFURC_LINK extern "C"
   #else
      #define YIFFURC_LINK extern
   #endif
#endif

#ifdef _WIN32
   #ifdef __GNUC__
      #define YIFFURC_CALL __attribute__((cdecl))
   #else
      #define YIFFURC_CALL __cdecl
   #endif
#else
   #define YIFFURC_CALL
#endif

#ifndef __cplusplus
   #include <stddef.h>
   #include <stdbool.h>
#else
   #include <cstddef>
#endif


#if defined( __cplusplus )
namespace yiffurc {
#endif

/** Defines a major and minor revision. */
struct MAJORMINOR
{
   unsigned short major;
   unsigned short minor;
};

/** An object/floor/wall ID and a position pair. */
struct IDXY
{
   unsigned int id;
   unsigned int x;
   unsigned int y;
};

/** A DS line and parameter pair. */
struct DSXY
{
   unsigned int line;
   unsigned int x;
   unsigned int y;
};

/** Player movement directions.
*  \todo In the wrong place? */
enum Move_Dir
{
   MOVEDIR_SW = '1',
   MOVEDIR_SE = '3',
   MOVEDIR_NW = '7',
   MOVEDIR_NE = '9',
};

/** Some main map numbers.
* add to ' ' to make the correct gomap.
*  \sa c2s_enc_gomap */
enum Go_Map
{
   GOMAP_VINCA = 0,
   GOMAP_GOLDWYN = 1,
   GOMAP_GRASS = 2 ,
   GOMAP_ALLEGRIA = 3,
   GOMAP_CHALLENGE = 4,
   GOMAP_FURRABIA = 5,
   GOMAP_MEOVANNI = 6,
   GOMAP_HAVEN = 7,
   GOMAP_IMAGINARIUM = 8,
   GOMAP_THERIOPOLOIS = 9,
   GOMAP_ACROPOLIS = 10,
   GOMAP_UPDATE = 11,
   GOMAP_PILLOWZ = 14,
   GOMAP_DUSK2DAWN = 17,
};

/** Converts a 3-digit base-95 furcadian number to unsigned int.
*  \note
*     ftoi3( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-95 Furcadian number (at least \c char[3])
*/
inline   unsigned int      ftoi3( const char* f )
{
   return ( (unsigned) f[ 0 ] * 9025 +
      (unsigned) f[ 1 ] * 95 +
      (unsigned) f[ 2 ] ) - 291935;
}
/** Converts a 2-digit base-95 furcadian number to an integer.
*  \note
*     ftoi2( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-95 Furcadian number (at least \c char[2])
*/
inline   unsigned int      ftoi2( const char* f )
{
   return ( (unsigned) f[ 0 ] * 95 + (unsigned) f[ 1 ] ) - 3072;
}
/** Converts a 1-digit base-95 furcadian number to an integer.
*  \param   f  Base-95 Furcadian digit.
*/
inline   unsigned int      ftoi1( char f )
{
   return ((unsigned) f - 32) % 95;
}
/** Converts an integer to a 3-digit base-95 furcadian number.
*  \note
*     itof3( ) won't null-terminate \c f.
*  \param[in]     i     integer number
*  \param[out]    f     Base-95 Furcadian number (at least \c char[3])
*/
inline   void              itof3( unsigned int i, char* f )
{
   f[ 0 ] = ( i / 9025 ) + 32;
   f[ 1 ] = ( i / 95 )   + 32;
   f[ 2 ] = ( i % 95 )   + 32;
}
/** Converts an integer to a 2-digit base-95 furcadian number.
*  \note
*     itof2( ) won't null-terminate \c f.
*  \param[in]     i           integer number
*  \param[out]    f           Base-95 Furcadian number (at least \c char[2])
*/
inline   void              itof2( unsigned int i, char* f )
{
   f[ 0 ] = ( i / 95 ) + 32;
   f[ 1 ] = ( i % 95 ) + 32;
}
/** Converts an integer to a 1-digit base-95 furcadian number.
*  param    i     integer number.
*  \returns Base-95 Furcadian digit.
*/
inline   char              itof1( unsigned int i )
{
   return (i % 95) + 32;
}

/** Converts a 4-digit base-220 furcadian number to unsigned int.
*  \note
*     ftoi4_220( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-220 Furcadian number (at least \c char[4])
*/
inline   unsigned int      ftoi4_220( const char* f )
{
   return ( (unsigned char) f[ 0 ] +
            (unsigned char) f[ 1 ] * 220 +
            (unsigned char) f[ 2 ] * 220 * 220 +
            (unsigned char) f[ 3 ] * 220 * 220 * 220 ) - (35 + 35*220 + 35*220*220 + 35*220*220*220);
}
/** Converts a 3-digit base-220 furcadian number to unsigned int.
*  \note
*     ftoi3_220( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-220 Furcadian number (at least \c char[3])
*/
inline   unsigned int      ftoi3_220( const char* f )
{
   return ( (unsigned char) f[ 0 ] +
            (unsigned char) f[ 1 ] * 220 +
            (unsigned char) f[ 2 ] * 220 * 220 ) - (35 + 35*220 + 35*220*220);
}
/** Converts a 2-digit base-220 furcadian number to an integer.
*  \note
*     ftoi2_220( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-220 Furcadian number (at least \c char[2])
*/
inline   unsigned int      ftoi2_220( const char* f )
{
   return ( (unsigned char) f[ 0 ] + (unsigned char) f[ 1 ] * 220 ) - (35 + 35*220);
}
/** Converts a 1-digit base-220 furcadian number to an integer.
*  \param   f  Base-220 Furcadian digit.
*/
inline   unsigned int      ftoi1_220( char f )
{
   return (unsigned char) f - 35;
}
/** Converts an integer to a 4-digit base-220 furcadian number.
*  \note
*     itof4_220( ) won't null-terminate \c f.
*  \param[in]     i     integer number
*  \param[out]    f     Base-220 Furcadian number (at least \c char[4])
*/
inline   void              itof4_220( unsigned int i, char* f )
{
   f[ 0 ] = ( i % 220 ) + 35;
   f[ 1 ] = ( i / 220 ) + 35;
   f[ 2 ] = ( i / (220*220) ) + 35;
   f[ 3 ] = ( i / (220*220*220) ) + 35;
}
/** Converts an integer to a 3-digit base-220 furcadian number.
*  \note
*     itof3_220( ) won't null-terminate \c f.
*  \param[in]     i     integer number
*  \param[out]    f     Base-220 Furcadian number (at least \c char[3])
*/
inline   void              itof3_220( unsigned int i, char* f )
{
   f[ 0 ] = ( i % 220 ) + 35;
   f[ 1 ] = ( i / 220 ) + 35;
   f[ 2 ] = ( i / (220*220) ) + 35;
}
/** Converts an integer to a 2-digit base-220 furcadian number.
*  \note
*     itof2_220( ) won't null-terminate \c f.
*  \param[in]     i           integer number
*  \param[out]    f           Base-220 Furcadian number (at least \c char[2])
*/
inline   void              itof2_220( unsigned int i, char* f )
{
   f[ 0 ] = ( i % 220 ) + 35;
   f[ 1 ] = ( i / 220 ) + 35;
}
/** Converts an integer to a 1-digit base-220 furcadian number.
*  param    i     integer number.
*  \returns Base-220 Furcadian digit.
*/
inline   char              itof1_220( unsigned int i )
{
   return (i % 220) + 35;
}

/** Compute the length of a color string. 
*  \remarks This doesn't include the gender-species-digo bytes that
*           sometimes follow the color string. Those are always 3 bytes
*           long.
*/
YIFFURC_LINK unsigned YIFFURC_CALL get_color_len( const char* color, const unsigned len );

/** \addtogroup memory Memory Management
*  \{ */
/** Releases any dynamically allocated object returned by a YIFFurc API function. */
YIFFURC_LINK void YIFFURC_CALL   yiffurc_release( const void* p );
/** \} */

#if defined( __cplusplus )
} // namespace yiffurc
#endif

#endif
