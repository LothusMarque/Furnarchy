#ifndef _FSML_IMAGE_H__
#define _FSML_IMAGE_H__

#include "fsml_common.h"

struct SDL_Surface;

class FsmlImage : public RefCount
{
private:
   FsmlImage( ) { };
   ~FsmlImage( );
public:
   /** Creates a 32-bit image. */
   FsmlImage( unsigned int w, unsigned int h );
   /* Creates an image from a bitmap.  
   *  The bits aren't copied, so don't free bits before freeing
   *  the created image. */
   FsmlImage( void* bits, unsigned int w, unsigned int h, 
              unsigned int bpp, unsigned int pitch );

public:
   static FsmlImage* load_from_file( const string& file );

public:
   void              setClipRect( const FSML_RECT* clip = 0 );
   const FSML_RECT&  getClipRect( ) const;
   bool              blt( FsmlImage& dst, const FSML_POS& dst_pos, 
                          const FSML_POS& src_pos,
                          const FSML_SIZE& size ) const;
   bool              fillRect( const FSML_COLOR& color, 
                               const FSML_RECT& dst_rect );
   const FSML_SIZE&  getSize( ) const;

private:
   FSML_SIZE    m_size;
   SDL_Surface* m_surface;
};

typedef ref_ptr<FsmlImage> FsmlImagePtr;
typedef c_ref_ptr<FsmlImage> FsmlImageCPtr;

inline const FSML_SIZE& FsmlImage::getSize( ) const {
   return m_size;
}

#endif