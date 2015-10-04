#include "FsmlImage.h"
#include <FreeImage.h>
#include <SDL.h>
#include <iostream>

using namespace std;

inline SDL_Rect to_SDL_Rect( const FSML_RECT& r ) {
   SDL_Rect _r = { r.x, r.y, r.w, r.h };
   return _r;
}

FsmlImage::FsmlImage( unsigned int w, unsigned int h )
   : m_surface( 0 )
{
   m_surface = SDL_CreateRGBSurface( SDL_HWSURFACE | SDL_SRCALPHA, w, h, 32,
                                     0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 );

   if (!m_surface)   // Try software.
      m_surface = SDL_CreateRGBSurface( SDL_SWSURFACE | SDL_SRCALPHA, w, h, 32,
                                        0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 );

   if (m_surface)
   {
      m_size.w = w;
      m_size.h = h;
   }
   else
      cerr << "Unable to create an empty SDL surface ("
           << w << 'x' << h << ").\n";
}

FsmlImage::FsmlImage( void* bits, unsigned int w, unsigned int h, 
                      unsigned int bpp, unsigned int pitch )
   : m_surface( 0 )
{
   switch (bpp)
   {
   case 32:
      m_surface = SDL_CreateRGBSurfaceFrom( bits, w, h, bpp, pitch,
                     0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 );
      break;
   case 24:
      m_surface = SDL_CreateRGBSurfaceFrom( bits, w, h, bpp, pitch,
                     0x000000FF, 0x0000FF00, 0x00FF0000, 0 );
      break;
   default:
      cerr << "Only 32 and 24 bit FsmlImages can be created.\n";
      return;
   }
   
   if (m_surface)
   {
      m_size.w = w;
      m_size.h = h;
   }
   else
      cerr << "Unable to create an indirect SDL surface ("
      << w << 'x' << h << '@' << bpp << ").\n";
}

FsmlImage::~FsmlImage( )
{
   if (m_surface)
      SDL_FreeSurface( m_surface );
}

bool FsmlImage::fillRect( const FSML_COLOR& c, const FSML_RECT& r )
{
   if (!m_surface)
      return false;

   return !SDL_FillRect( m_surface, &to_SDL_Rect( r ), *(Uint32*)&c );
}

/* NEVAR FORGET: dst = ( src.a * (src - dst) + (dst << 8) ) >> 8; */

bool FsmlImage::blt( FsmlImage& dst, const FSML_POS& dst_pos, 
                     const FSML_POS& src_pos, const FSML_SIZE& size ) const
{
   if (!m_surface || !dst.m_surface)
      return false;

   SDL_Rect rect_dst = { dst_pos.x, dst_pos.y, size.w, size.h };
   SDL_Rect rect_src = { src_pos.x, src_pos.y, size.w, size.h };
   return !SDL_BlitSurface( m_surface, &rect_src, dst.m_surface, &rect_dst );
}

FsmlImage* FsmlImage::load_from_file( const string& file )
{
   FREE_IMAGE_FORMAT file_type = FreeImage_GetFileType( file.c_str( ) );

   if (file_type == FIF_UNKNOWN)
      return 0;

   FIBITMAP* fi_bmp = FreeImage_Load( file_type, file.c_str( ) );

   if (!fi_bmp)
      return 0;

   FIBITMAP* fi_bmp32 = FreeImage_ConvertTo32Bits( fi_bmp );

   FreeImage_Unload( fi_bmp );

   if (!fi_bmp32)
      return 0;

   unsigned int width  = FreeImage_GetWidth( fi_bmp32 );
   unsigned int height = FreeImage_GetHeight( fi_bmp32 );
   size_t pitch        = FreeImage_GetPitch( fi_bmp32 );

   FsmlImage* img = new FsmlImage( width, height );  
   if (!width || !height || !img->m_surface)
   {
      delete img;
      FreeImage_Unload( fi_bmp32 );
      return 0;
   }

   /* Flip the image right-side-up while copying. */

   const char* src_px = (const char*) FreeImage_GetScanLine( fi_bmp32, height - 1 );
   SDL_LockSurface( img->m_surface );
   FSML_COLOR* dst_px = (FSML_COLOR*) img->m_surface->pixels;

   while (height)
   {
      const char* const scan_end = src_px + pitch;
      while (src_px != scan_end)
      {
         dst_px->r = src_px[ FI_RGBA_BLUE ];
         dst_px->g = src_px[ FI_RGBA_GREEN ];
         dst_px->b = src_px[ FI_RGBA_RED ];
         dst_px->a = src_px[ FI_RGBA_ALPHA ];
         src_px += 4;
         ++dst_px;
      }

      src_px -= 2 * pitch;

      --height;
   }

   SDL_UnlockSurface( img->m_surface );
   FreeImage_Unload( fi_bmp32 );

   return img;
}