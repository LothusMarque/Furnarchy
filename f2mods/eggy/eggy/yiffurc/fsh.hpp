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

#if !defined( _YIFFURC_FSH_HPP__ ) && defined( __cplusplus )
#define _YIFFURC_FSH_HPP__

#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>

/** \addtogroup cpp
* \{ */
/** \addtogroup fsh FSH Files
* \{ */

namespace yiffurc {

/** Class to pull shapes from an FSH file stream.
*  define BIG_ENDIAN before including this to do endian correction. */
class FshStream
{
   enum { SHAPE_INFO_SIZE = 4 };

public:
   /** Shape information. */
   struct SHAPEINFO
   {
      /** Width (in pixels) */
      unsigned char width;
      /** Height (in pixels) */
      unsigned char height;
      /** Rendering x offset */
      signed char off_x;
      /** Rendering y offset */
      signed char off_y;
   };

protected:
   FshStream( );
public:
   /** Creates an FshStream with an istream object.
   * The istream object will not be closed when the FshStream object is
   * destroyed. */
   FshStream( std::istream* fs );
   virtual ~FshStream( );

   /** Check whether the stream is good for reading. */
   bool             good( ) const;
   /** Retrieve information on a shape. */
   SHAPEINFO        getShapeInfo( std::size_t shape );
   /** Copy shape image data.
   * \a buf will be filled with 8-bit palette indices.
   * \a buf should be at least <tt>shapeinfo.width * shapeinfo.height</tt> bytes. */
   std::size_t      copyShapeBits( std::size_t shape, void* buf );
   /** Copy shape image data.
   * \a buf will be filled with 8-bit palette indices.
   * \a pad bytes at the end of each row will be left untouched.
   * \a buf should be at least <tt>(shapeinfo.width + \a pad) * shapeinfo.height</tt> bytes. */
   std::size_t      copyShapeBitsPadded( std::size_t shape, void* buf, std::size_t pad );
   /** Returns the number of shapes in the FSH stream. */
   std::size_t      getNumShapes( ) const;

protected:
   void init( );

protected:
   template <typename T>
   static T endian_swap16( const T& );

protected:
   std::istream*               m_fs;
   bool                        m_fail;
   std::size_t                 m_num_shapes;
   std::vector<std::streampos> m_shapes_pos;
};

template <typename T>
inline T FshStream::endian_swap16( const T& t16 )
{
#ifdef BIG_ENDIAN
   return ((t16 & 0xFF) << 8) | ((t16 & 0xFF00) >> 8);
#else
   return t16;
#endif
}

inline FshStream::FshStream( )
: m_fs( NULL ), m_fail( true ), m_num_shapes( 0 ) { }

inline FshStream::FshStream( std::istream* fs ) : m_fs( fs ) {
   init( );
}

inline FshStream::~FshStream( ) { }

inline void FshStream::init( )
{
   using namespace std;

   assert( offsetof( SHAPEINFO, off_y ) + 1 == SHAPE_INFO_SIZE );

   m_fs->clear( );
   m_fs->seekg( 0 );

   m_num_shapes = 0;
   m_shapes_pos.clear( );

   if (m_fs->good( ))
   {
      unsigned short num_shapes;
      m_fs->read( (char*) &num_shapes, sizeof( unsigned short ) );
      if (m_fs->gcount( ) != sizeof( unsigned short ))
      {
         m_fail = true; return;
      }
      m_num_shapes = endian_swap16( num_shapes );
   }
   else
   {
      m_fail = true; return;
   }

   {
      unsigned short shape_size;
      streampos last_chunk = m_fs->tellg( ) +
         (streampos) (m_num_shapes * sizeof( unsigned short ));

      for (size_t i = 0; m_fs->good( ) && i < m_num_shapes; ++i)
      {
         m_fs->read( (char*) &shape_size, sizeof( unsigned short ) );
         if (m_fs->gcount( ) != sizeof( unsigned short ))
            break;

         m_shapes_pos.push_back( last_chunk );
         last_chunk += shape_size;
      }
   }

   if (m_shapes_pos.size( ) < m_num_shapes)
   {
      m_fail = true;
   }
}

inline bool FshStream::good( ) const {
   return m_fs->good( );
}

inline std::size_t FshStream::getNumShapes( ) const {
   return m_num_shapes;
}

inline FshStream::SHAPEINFO FshStream::getShapeInfo( std::size_t shape )
{
   using namespace std;

   assert( shape < m_shapes_pos.size( ) );

   m_fs->clear( );
   m_fs->seekg( m_shapes_pos[ shape ], ios_base::beg );
   SHAPEINFO info;
   m_fs->read( (char*) &info, sizeof( info ) );
   if ((unsigned) m_fs->gcount( ) < sizeof( info ))
      m_fail = true;
   return info;
}

inline std::size_t FshStream::copyShapeBits( std::size_t shape, void* buf )
{
   using namespace std;

   assert( shape < m_shapes_pos.size( ) );

   const SHAPEINFO info  = getShapeInfo( shape );
   const streamsize size = info.width * info.height;

   assert( m_fs->tellg( ) == m_shapes_pos[ shape ] + (streampos) sizeof( SHAPEINFO ) );

   m_fs->read( (char*) buf, size );
   if (m_fs->gcount( ) < size)
      m_fail = true;
   return m_fs->gcount( );
}

inline std::size_t FshStream::copyShapeBitsPadded( std::size_t shape, void* buf, std::size_t pad )
{
   using namespace std;

   assert( shape < m_shapes_pos.size( ) );

   SHAPEINFO info            = getShapeInfo( shape );
   const streamsize row_size = info.width * 1;
   const streamsize pitch    = info.width * 1 + pad;

   assert( m_fs->tellg( ) == m_shapes_pos[ shape ] + (streampos) sizeof( SHAPEINFO ) );

   std::size_t bytes_read = 0;
   for (; info.height; --info.height, (char*&) buf += pitch)
   {
      m_fs->read( (char*) buf, row_size );
      if (m_fs->gcount( ) < row_size)
         m_fail = true;
      bytes_read += m_fs->gcount( );
   }
   return bytes_read;
}

//////////////////////////////////////////////////////////////////////////

/** Descendant of FshStream that reads shapes specifically from a file name. */
class FshFile : public FshStream
{
protected:
   FshFile( );
public:
   FshFile( const char* filename );
   ~FshFile( );

protected:
   std::ifstream m_file;
};

inline FshFile::FshFile( ) { }

inline FshFile::FshFile( const char* filename )
: m_file( filename, std::ios_base::binary )
{
   m_fs = &m_file;
   init( );
}

inline FshFile::~FshFile( ) { }

} // namespace yiffurc

/** \} fsh */
/** \} cpp */

#endif
