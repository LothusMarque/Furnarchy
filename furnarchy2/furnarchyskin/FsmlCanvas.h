#ifndef _FSML_CANVAS_H__
#define _FSML_CANVAS_H__

#include "fsml_common.h"
#include "FsmlImage.h"
#include <stack>

class FsmlCanvas
{
private:
   struct VIEWPORT
   {
      FSML_POS  origin;
      FSML_RECT clip;
   };

   typedef std::stack<VIEWPORT> VpStack;

private:
   FsmlCanvas( ) { }
public:
   FsmlCanvas( FsmlImage* image );
   virtual ~FsmlCanvas( ) { }

public:
   const FSML_SIZE& getSize( ) const;

   void pushViewport( const FSML_RECT* vp = 0 );
   void popViewport( unsigned int num_unwind = 1 );
   bool drawImage( const FsmlImage& img, const FSML_POS& src_pos,
                   const FSML_POS& dst_pos, const FSML_SIZE* size = 0 );
   bool fillRect( const FSML_COLOR& c, 
                  const FSML_RECT* r = 0 );

private:
   FsmlImagePtr m_image;
   VpStack      m_vp_stack;
};

inline FsmlCanvas::FsmlCanvas( FsmlImage* image ) : m_image( image )
{
   VIEWPORT new_vp = { FSML_POS( 0, 0 ), 
                       FSML_RECT( 0, 0, image->getSize( ).w, image->getSize( ).h ) };
   m_vp_stack.push( new_vp );
}

inline const FSML_SIZE& FsmlCanvas::getSize( ) const {
   return m_image->getSize( );
}

#endif
