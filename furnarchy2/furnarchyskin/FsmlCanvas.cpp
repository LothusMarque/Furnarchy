#include "FsmlCanvas.h"

inline int _sat( int n )
{
   if (n >= 0)
      return n;
   return 0;
}

void FsmlCanvas::pushViewport( const FSML_RECT* vp /* = 0 */ )
{
   const VIEWPORT& curr_vp = m_vp_stack.top( );
   VIEWPORT new_vp;
   if (!vp)
      new_vp = curr_vp;
   else
   {
      new_vp.origin.x = curr_vp.origin.x + vp->x;
      new_vp.origin.y = curr_vp.origin.y + vp->y;
      new_vp.clip.x = new_vp.origin.x;
      new_vp.clip.y = new_vp.origin.y;
      new_vp.clip.w = vp->w;
      new_vp.clip.h = vp->h;

      fsml_rect_intersect( &new_vp.clip, new_vp.clip, curr_vp.clip );
   }

   m_vp_stack.push( new_vp );
}

void FsmlCanvas::popViewport( unsigned int num_unwind /* = 1 */ )
{
   while (num_unwind && m_vp_stack.size( ) > 1)
   {
      m_vp_stack.pop( );
      --num_unwind;
   }
}

bool FsmlCanvas::drawImage( const FsmlImage& img, const FSML_POS& src_pos, 
                            const FSML_POS& dst_pos, const FSML_SIZE* size /* = 0  */ )
{
   /* Clip, clip, clip. */
   const VIEWPORT& vp = m_vp_stack.top( );
   FSML_POS  clp_src_pos = src_pos;
   FSML_POS  clp_dst_pos( dst_pos.x + vp.origin.x, dst_pos.y + vp.origin.y );
   FSML_SIZE clp_size = size ? *size : img.getSize( );
   {
      const FSML_SIZE& src_size = img.getSize( );
      
      if (clp_src_pos.x < 0)
      {
         clp_size.w = _sat( clp_size.w + clp_src_pos.x );
         clp_dst_pos.x -= clp_src_pos.x;
         clp_src_pos.x = 0;
      }
      if (clp_dst_pos.x < vp.clip.x)
      {
         const int dx = vp.clip.x - clp_dst_pos.x;
         clp_size.w = _sat( clp_size.w - dx );
         clp_src_pos.x += dx;
         clp_dst_pos.x = vp.clip.x;
      }
      if (clp_src_pos.x + clp_size.w > src_size.w)
      {
         const int dx = (clp_src_pos.x + clp_size.w) - src_size.w;
         clp_size.w = _sat( clp_size.w - dx );
      }
      if (clp_dst_pos.x + clp_size.w > vp.clip.x + vp.clip.w)
      {
         const int dx = (clp_dst_pos.x + clp_size.w) - (vp.clip.x + vp.clip.w);
         clp_size.w = _sat( clp_size.w - dx );
      }

      if (clp_src_pos.y < 0)
      {
         clp_size.h = _sat( clp_size.h + clp_src_pos.y );
         clp_dst_pos.y -= clp_src_pos.y;
         clp_src_pos.y = 0;
      }
      if (clp_dst_pos.y < vp.clip.y)
      {
         const int dy = vp.clip.y - clp_dst_pos.y;
         clp_size.h = _sat( clp_size.h - dy );
         clp_src_pos.y += dy;
         clp_dst_pos.y = vp.clip.y;
      }
      if (clp_src_pos.y + clp_size.h > src_size.h)
      {
         const int dy = (clp_src_pos.y + clp_size.h) - src_size.h;
         clp_size.h = _sat( clp_size.h - dy );
      }
      if (clp_dst_pos.y + clp_size.h > vp.clip.y + vp.clip.h)
      {
         const int dy = (clp_dst_pos.y + clp_size.h) - (vp.clip.y + vp.clip.h);
         clp_size.h = _sat( clp_size.h - dy );
      }
   }

   if (!clp_size.w  || !clp_size.h)
      return true;

   return img.blt( *m_image, clp_dst_pos, clp_src_pos, clp_size );
}

bool FsmlCanvas::fillRect( const FSML_COLOR& c, const FSML_RECT* r )
{
   /* Clip, clip, clip. */
   const VIEWPORT& vp = m_vp_stack.top( );
   FSML_RECT clp_r;
   if (!r)
      clp_r = vp.clip;
   else
   {
      clp_r.x = vp.origin.x + r->x;
      clp_r.y = vp.origin.y + r->y;
      clp_r.w = r->w;
      clp_r.h = r->h;

      fsml_rect_intersect( &clp_r, clp_r, vp.clip );
   }

   if (!clp_r.w || !clp_r.h)
      return true;

   return m_image->fillRect( c, clp_r );
}
