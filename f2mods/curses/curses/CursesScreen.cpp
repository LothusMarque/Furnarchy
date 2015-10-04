#include "curses.h"
#include "SwatchCtrl.h"
#include "yiffurc/fsh.hpp"
#include "yiffurc/colors.hpp"
#include "yiffurc/avatar.h"
#include "resource.h"
#include <CommCtrl.h>
#include <CommDlg.h>
#include <sstream>
#include <fstream>
#include <vector>

namespace curses {

using namespace yiffurc;

#define CONTAINER_CLASS "Curses_Container"

#define LBN_RCLICK 100
#define LBN_RCLICKITEM 101

enum { CTRL_REGISTER = 5000, CTRL_PLAY, CTRL_CLOSE, CTRL_SAVE,
       CTRL_SAVEAS, CTRL_SETTINGS, CTRL_CHARACTERS, CTRL_PORTRAIT,
       CTRL_NAME, CTRL_PASSWORD, CTRL_DESCRIPTION, CTRL_SPECIES,
       CTRL_GENDER, CTRL_CODE, CTRL_COLORS, CTRL_SWATCH, CTRL_PREVIEW,
       CTRL_DIGO, CTRL_FILENAME, CTRL_STATUS };

//////////////////////////////////////////////////////////////////////////

class CursesScreen::FshBitmapInfo
{
public:
   BITMAPINFOHEADER          m_hdr;
   PixelB8G8R8               m_pal[ 256 ];
   AvatarRemaps<PixelB8G8R8> m_remaps;

   void resetPalette( ) 
   {
      fill_default_palette( m_pal );
      m_pal[ 0 ].rgb( GetRValue( GetSysColor( COLOR_WINDOW ) ),
                      GetGValue( GetSysColor( COLOR_WINDOW ) ),
                      GetBValue( GetSysColor( COLOR_WINDOW ) ) );
      m_pal[ 10 ].rgb( 128, 128, 128 );
   }
   void setDims( unsigned w, unsigned h )
   {
      m_hdr.biWidth  = w;
      m_hdr.biHeight = h;
   }
   void remap( const ColorCodePal& code ) {
      m_remaps.remap( code, m_pal, 256 );
   }
   FshBitmapInfo( )
   {
      memset( &m_hdr, 0, sizeof( m_hdr ) );
      m_hdr.biBitCount = 8;
      m_hdr.biPlanes   = 1;
      m_hdr.biSize     = sizeof( m_hdr );
      m_hdr.biWidth    = 0;
      m_hdr.biHeight   = 0;
      m_remaps.applyDefaults( );
      resetPalette( );
   }
};

//////////////////////////////////////////////////////////////////////////

struct FSHSHAPE
{
   unsigned char w, h;
   signed char   off_x, off_y;
   unsigned char* pixels;
   FSHSHAPE( ) : w( 0 ), h( 0 ), off_x( 0 ), off_y( 0 ), pixels( NULL ) { }
   void clear( ) { w = h = off_x = off_y = 0; pixels = NULL; }
   bool empty( ) { return (!w || !h); }
   FSHSHAPE& operator =( const FshFile::SHAPEINFO& r )
   { 
      w = r.width; h = r.height; off_x = r.off_x; off_y = r.off_y;
      return *this;
   }
};

//////////////////////////////////////////////////////////////////////////

static unsigned ComboBox_FindFirst( HWND wnd, LPARAM data );
static unsigned ListBox_FindFirst( HWND wnd, LPARAM data );
static string makefilename( const string& str );
static void InvalidateParent( HWND child, BOOL erase );
static void blt_full( HWND wnd, HDC dst, HDC src );
static LRESULT CALLBACK edit_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
static LRESULT CALLBACK lb_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
static bool operator !=( const LOGININFO& li, const CharacterIni& ini );

//////////////////////////////////////////////////////////////////////////

class CursesScreen::Assets 
{
public:
   class BgAssets
   {
   public:
      HDC     dc;
      HBITMAP dib;

      void reset( )
      {
         if (dc)
         {
            RestoreDC( dc, -1 );
            DeleteDC( dc );
            dc = NULL;
         }
         if (dib)
         {
            DeleteBitmap( dib );
            dib = NULL;
         }
      }

      void load( )
      {
         reset( );

         HDC parent_dc = GetDC( Curses::singleton( )->getWnd( ) );
         dc = CreateCompatibleDC( parent_dc );
         ReleaseDC( Curses::singleton( )->getWnd( ), parent_dc );

         SaveDC( dc );

         string file = f2_modroot( Curses::singleton( )->getModule( ) );
         file += "\\ui\\bg.bmp";
         dib = (HBITMAP) LoadImage( NULL, file.c_str( ), IMAGE_BITMAP, 0, 0, 
                                    LR_LOADFROMFILE | LR_CREATEDIBSECTION );

         if (dib)
            SelectObject( dc, dib );
      }

      BgAssets( )
         : dc( NULL ), dib( NULL ) { }
      ~BgAssets( ) { reset( ); }
   };

   class CtrlAssets
   {
   public:
      int x, y;
      unsigned int w, h;

      CtrlAssets( )
         : x( 0 ), y( 0 ), w( 0 ), h( 0 ) { }

      void reset( )
      {
         x = 0;
         y = 0;
         w = 0;
         h = 0;
      }

      template <class T_StrIter>
      void load( T_StrIter begin, const T_StrIter& end )
      {
         reset( );

         if (begin != end)
         {
            x = atol( begin->c_str( ) );
            ++begin;
         }
         else
            x = 0;

         if (begin != end)
         {
            y = atol( begin->c_str( ) );
            ++begin;
         }
         else
            y = 0;

         if (begin != end)
         {
            w = atol( begin->c_str( ) );
            ++begin;
         }
         else
            w = 0;

         if (begin != end)
         {
            h = atol( begin->c_str( ) );
            ++begin;
         }
         else
            h = 0;
      }
   };

   class ButtonAssets : public CtrlAssets
   {
   public:
      HDC     dc_up;
      HDC     dc_down;
      HBITMAP dib_up;
      HBITMAP dib_down;

      void reset( )
      {
         if (dc_up)
         {
            RestoreDC( dc_up, -1 );
            DeleteDC( dc_up );
            dc_up = NULL;
         }
         if (dc_down)
         {
            RestoreDC( dc_down, -1 );
            DeleteDC( dc_down );
            dc_down = NULL;
         }
         if (dib_up)
         {
            DeleteBitmap( dib_up );
            dib_up = NULL;
         }
         if (dib_down)
         {
            DeleteBitmap( dib_down );
            dib_down = NULL;
         }
      }

      template <class T_StrIter>
      void load( const string& name, T_StrIter begin, const T_StrIter& end ) 
      { 
         CtrlAssets::reset( );
         reset( );

         CtrlAssets::load( begin, end );

         HDC parent_dc = GetDC( Curses::singleton( )->getWnd( ) );
         dc_up = CreateCompatibleDC( parent_dc );
         dc_down = CreateCompatibleDC( parent_dc );
         ReleaseDC( Curses::singleton( )->getWnd( ), parent_dc );

         SaveDC( dc_up );
         SaveDC( dc_down );

         string file = f2_modroot( Curses::singleton( )->getModule( ) );
         file +=  "\\ui\\";
         file += name;
         dib_up = (HBITMAP) LoadImage( NULL, (file + ".bmp").c_str( ), IMAGE_BITMAP, 0, 0, 
                                       LR_LOADFROMFILE | LR_CREATEDIBSECTION );
         if (dib_up)
            SelectObject( dc_up, dib_up );

         dib_down = (HBITMAP) LoadImage( NULL, (file + "_down.bmp").c_str( ), IMAGE_BITMAP, 0, 0, 
                                         LR_LOADFROMFILE | LR_CREATEDIBSECTION );
         if (dib_down)
            SelectObject( dc_down, dib_down );
      }

      ButtonAssets( )
         : dc_up( NULL ), dc_down( NULL ), dib_up( NULL ), dib_down( NULL ) { }
      ~ButtonAssets( ) { reset( ); }      
   };

   class TextAssets : public CtrlAssets
   {
   public:
      HFONT    font;
      string   family;
      unsigned height;
      COLORREF color;

      void reset( )
      {
         if (font)
            DeleteFont( font );
         font = NULL;
         family.clear( );
         height = 0;
         color = RGB( 255, 255, 255 );
      }

      template <class T_StrIter>
      void load( T_StrIter begin, const T_StrIter& end )
      {
         CtrlAssets::reset( );
         reset( );

         CtrlAssets::load( begin, end );

         // Skip past the basic stuff.
         for (unsigned i = 0; begin != end && i < 4; ++i, ++begin) ;

         if (begin != end)
         {
            family = *begin;
            ++begin;
         }

         if (begin != end)
         {
            height = atol( begin->c_str( ) );
            ++begin;
         }
         else
            height = 12;

         if (begin != end)
         {
            color = strtol( begin->c_str( ), NULL, 16 );
            color = ((color & 0xFF0000) >> 16) | (color & 0xFF00) | ((color & 0xFF) << 16);
         }
         else
            color = RGB( 255, 255, 255 );

         // Create the font.
         font = CreateFont( height, (int) ((float) height / 2.5f), 
                            0, 0, FW_BOLD, 
                            FALSE, FALSE, FALSE, 
                            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_MODERN, family.c_str( ) );

         if (!font)
            font = GetStockFont( DEFAULT_GUI_FONT );
      }

      TextAssets( )
         : font( NULL ), height( 0 ), color( RGB( 255, 255, 255 ) ) { }
      ~TextAssets( ) { reset( ); }
   };

   class SwatchAssets : public CtrlAssets
   {
   public:
      typedef AvatarRemaps<PixelR8G8B8X8> remaps_t;
      
      COLORREF* m_swatches[ 10 ];
      unsigned m_counts[ 10 ];

      void reset( )
      {
         for (unsigned i = 0; i < 10; ++i)
         {
            delete [] m_swatches[ i ];
            m_swatches[ i ] = NULL;
            m_counts[ i ] = 0;
         }
      }

      template <class T_StrIter>
      void load( T_StrIter begin, const T_StrIter& end )
      {
         CtrlAssets::reset( );
         reset( );

         CtrlAssets::load( begin, end );
         
         m_swatches[ ColorCodePal::COLOR_FUR ] = new COLORREF[ remaps_t::COUNT_FUR ];
         m_counts[ ColorCodePal::COLOR_FUR ] = remaps_t::COUNT_FUR;
         for (unsigned i = 0; i < remaps_t::COUNT_FUR; ++i)
            m_swatches[ ColorCodePal::COLOR_FUR ][ i ] = *(COLORREF*) &remaps_t::s_default_fur[ i ][ remaps_t::RemapFur::COUNT / 2 ];

         m_swatches[ ColorCodePal::COLOR_MARKINGS ] = new COLORREF[ remaps_t::COUNT_MARKINGS ];
         m_counts[ ColorCodePal::COLOR_MARKINGS ] = remaps_t::COUNT_MARKINGS;
         for (unsigned i = 0; i < remaps_t::COUNT_MARKINGS; ++i)
            m_swatches[ ColorCodePal::COLOR_MARKINGS ][ i ] = *(COLORREF*) &remaps_t::s_default_markings[ i ][ remaps_t::RemapMarkings::COUNT / 2 ];

         m_swatches[ ColorCodePal::COLOR_HAIR ] = new COLORREF[ remaps_t::COUNT_HAIR ];
         m_counts[ ColorCodePal::COLOR_HAIR ] = remaps_t::COUNT_HAIR;
         for (unsigned i = 0; i < remaps_t::COUNT_HAIR; ++i)
            m_swatches[ ColorCodePal::COLOR_HAIR ][ i ] = *(COLORREF*) &remaps_t::s_default_hair[ i ][ remaps_t::RemapHair::COUNT / 2 ];

         m_swatches[ ColorCodePal::COLOR_EYES ] = new COLORREF[ remaps_t::COUNT_EYES ];
         m_counts[ ColorCodePal::COLOR_EYES ] = remaps_t::COUNT_EYES;
         for (unsigned i = 0; i < remaps_t::COUNT_EYES; ++i)
            m_swatches[ ColorCodePal::COLOR_EYES ][ i ] = *(COLORREF*) &remaps_t::s_default_eyes[ i ][ remaps_t::RemapEyes::COUNT / 2 ];

         m_swatches[ ColorCodePal::COLOR_BADGE ] = new COLORREF[ remaps_t::COUNT_BADGE ];
         m_counts[ ColorCodePal::COLOR_BADGE ] = remaps_t::COUNT_BADGE;
         for (unsigned i = 0; i < remaps_t::COUNT_BADGE; ++i)
            m_swatches[ ColorCodePal::COLOR_BADGE ][ i ] = *(COLORREF*) &remaps_t::s_default_badge[ i ][ remaps_t::RemapBadge::COUNT / 2 ];

         m_swatches[ ColorCodePal::COLOR_VEST ] = new COLORREF[ remaps_t::COUNT_VEST ];
         m_counts[ ColorCodePal::COLOR_VEST ] = remaps_t::COUNT_VEST;
         for (unsigned i = 0; i < remaps_t::COUNT_VEST; ++i)
            m_swatches[ ColorCodePal::COLOR_VEST ][ i ] = *(COLORREF*) &remaps_t::s_default_vest[ i ][ remaps_t::RemapVest::COUNT / 2 ];

         m_swatches[ ColorCodePal::COLOR_BRACERS ] = new COLORREF[ remaps_t::COUNT_BRACERS ];
         m_counts[ ColorCodePal::COLOR_BRACERS ] = remaps_t::COUNT_BRACERS;
         for (unsigned i = 0; i < remaps_t::COUNT_BRACERS; ++i)
            m_swatches[ ColorCodePal::COLOR_BRACERS ][ i ] = *(COLORREF*) &remaps_t::s_default_bracers[ i ][ remaps_t::RemapBracers::COUNT / 2 ];

         m_swatches[ ColorCodePal::COLOR_CAPE ] = new COLORREF[ remaps_t::COUNT_CAPE ];
         m_counts[ ColorCodePal::COLOR_CAPE ] = remaps_t::COUNT_CAPE;
         for (unsigned i = 0; i < remaps_t::COUNT_CAPE; ++i)
            m_swatches[ ColorCodePal::COLOR_CAPE ][ i ] = *(COLORREF*) &remaps_t::s_default_cape[ i ][ remaps_t::RemapCape::COUNT / 2 ];

         m_swatches[ ColorCodePal::COLOR_BOOTS ] = new COLORREF[ remaps_t::COUNT_BOOTS ];
         m_counts[ ColorCodePal::COLOR_BOOTS ] = remaps_t::COUNT_BOOTS;
         for (unsigned i = 0; i < remaps_t::COUNT_BOOTS; ++i)
            m_swatches[ ColorCodePal::COLOR_BOOTS ][ i ] = *(COLORREF*) &remaps_t::s_default_boots[ i ][ remaps_t::RemapBoots::COUNT / 2 ];

         m_swatches[ ColorCodePal::COLOR_TROUSERS ] = new COLORREF[ remaps_t::COUNT_TROUSERS ];
         m_counts[ ColorCodePal::COLOR_TROUSERS ] = remaps_t::COUNT_TROUSERS;
         for (unsigned i = 0; i < remaps_t::COUNT_TROUSERS; ++i)
            m_swatches[ ColorCodePal::COLOR_TROUSERS ][ i ] = *(COLORREF*) &remaps_t::s_default_trousers[ i ][ remaps_t::RemapTrousers::COUNT / 2 ];
      }

      SwatchAssets( )
      {
         for (unsigned i = 0; i < 10; ++i)
            m_swatches[ i ] = NULL, m_counts[ i ] = 0;
      }
      ~SwatchAssets( ) { reset( ); }
   };

   class PreviewAssets : public CtrlAssets
   {
   public:

      AvatarRemaps<PixelB8G8R8X8> m_remaps;
      FSHSHAPE m_av_normal[ ColorCodePal::COUNT_SPECIES ];
      FSHSHAPE m_av_classics[ ColorCodePal::COUNT_SPECIES ];
      FSHSHAPE m_av_tris[ ColorCodePal::COUNT_SPECIES ];
      FSHSHAPE m_av_butters[ ColorCodePal::COUNT_SPECIES ];
      FSHSHAPE m_av_bats[ ColorCodePal::COUNT_SPECIES ];
      FSHSHAPE m_av_primes[ ColorCodePal::COUNT_SPECIES ];
      FSHSHAPE m_av_kitter;
      FSHSHAPE m_av_pixie;
      FSHSHAPE m_av_wolven;
      FSHSHAPE m_av_dragon;
      FSHSHAPE m_av_phoenix;
      FSHSHAPE m_av_gryffe;
      FSHSHAPE m_av_eagle;
      FSHSHAPE m_av_kiwi;
      FSHSHAPE m_av_tygard;
      FSHSHAPE m_av_leonen;
      FSHSHAPE m_av_penguin;
      FSHSHAPE m_av_werewolf;
      FSHSHAPE m_av_bat;
      FSHSHAPE m_av_foxen;
      FSHSHAPE m_av_floxen;
      FSHSHAPE m_av_unicorn;
      FSHSHAPE m_av_furlingm;
      FSHSHAPE m_av_furlingf;
      FSHSHAPE m_av_chinchilla;
      FSHSHAPE m_av_toaster;
	  FSHSHAPE m_av_kirin;
	  //FSHSHAPE m_av_kirincloud;
	  FSHSHAPE m_av_noblecaninem;
	  FSHSHAPE m_av_noblecaninef;
	  FSHSHAPE m_av_noblefelinem;
	  FSHSHAPE m_av_noblefelinef;
	  FSHSHAPE m_av_reindeer;
	  FSHSHAPE m_av_catten;
	  FSHSHAPE m_av_owlen;
	  FSHSHAPE m_av_lovebird;

      void reset( )
      {
         m_remaps.applyDefaults( );
         for (unsigned i = 0; i < ColorCodePal::COUNT_SPECIES; ++i)
            delete [] m_av_normal[ i ].pixels, m_av_normal[ i ].clear( );
         for (unsigned i = 0; i < ColorCodePal::COUNT_SPECIES; ++i)
            delete [] m_av_classics[ i ].pixels, m_av_classics[ i ].clear( );
         for (unsigned i = 0; i < ColorCodePal::COUNT_SPECIES; ++i)
            delete [] m_av_tris[ i ].pixels, m_av_tris[ i ].clear( );
         for (unsigned i = 0; i < ColorCodePal::COUNT_SPECIES; ++i)
            delete [] m_av_butters[ i ].pixels, m_av_butters[ i ].clear( );
         for (unsigned i = 0; i < ColorCodePal::COUNT_SPECIES; ++i)
            delete [] m_av_bats[ i ].pixels, m_av_bats[ i ].clear( );
         delete [] m_av_dragon.pixels, m_av_dragon.clear( );
         delete [] m_av_phoenix.pixels, m_av_phoenix.clear( );
         delete [] m_av_gryffe.pixels, m_av_gryffe.clear( );
         delete [] m_av_eagle.pixels, m_av_eagle.clear( );
         delete [] m_av_kiwi.pixels, m_av_kiwi.clear( );
      }

      template <class T_StrIter>
      void load( T_StrIter begin, const T_StrIter& end )
      {
         CtrlAssets::reset( );
         reset( );

         CtrlAssets::load( begin, end );

         const string path = string( f2_clroot( ) ) + "\\patches\\default";

         unsigned char* pix_buf = new unsigned char[ 256 * 256 ];

         #define _READSHAPE( num, dst ) if ((num) < fsh.getNumShapes( ))                        \
                                        {                                                       \
                                           (dst) = fsh.getShapeInfo( (num) );                   \
                                           unsigned pad = (dst).w % 4 == 0 ?                    \
                                                          0 : 4 - ((dst).w % 4);                \
                                           (dst).pixels = new unsigned char[ ((dst).w + pad) * (dst).h ]; \
                                           fsh.copyShapeBitsPadded( (num), (dst).pixels, pad ); \
                                        }
         {
            yiffurc::FshFile fsh( (path + "\\player.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 3, m_av_normal[ ColorCodePal::SPECIES_RODENT ] );
               _READSHAPE( 23, m_av_normal[ ColorCodePal::SPECIES_EQUINE ] );
               _READSHAPE( 43, m_av_normal[ ColorCodePal::SPECIES_FELINE ] );
               _READSHAPE( 63, m_av_normal[ ColorCodePal::SPECIES_CANINE ] );
               _READSHAPE( 83, m_av_normal[ ColorCodePal::SPECIES_MUSTELINE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player2.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_classics[ ColorCodePal::SPECIES_RODENT ] );
               _READSHAPE( 22, m_av_classics[ ColorCodePal::SPECIES_EQUINE ] );
               _READSHAPE( 42, m_av_classics[ ColorCodePal::SPECIES_FELINE ] );
               _READSHAPE( 62, m_av_classics[ ColorCodePal::SPECIES_CANINE ] );
               _READSHAPE( 82, m_av_classics[ ColorCodePal::SPECIES_MUSTELINE ] );
               _READSHAPE( 102, m_av_kiwi );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player3.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_dragon );               
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player4.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_phoenix );               
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player5.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_normal[ ColorCodePal::SPECIES_LAPINE ] );               
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player6.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_classics[ ColorCodePal::SPECIES_LAPINE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player7.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 3, m_av_tris[ ColorCodePal::SPECIES_RODENT ] ); 
               _READSHAPE( 23, m_av_tris[ ColorCodePal::SPECIES_EQUINE ] );
               _READSHAPE( 43, m_av_tris[ ColorCodePal::SPECIES_FELINE ] );
               _READSHAPE( 63, m_av_tris[ ColorCodePal::SPECIES_CANINE ] );
               _READSHAPE( 83, m_av_tris[ ColorCodePal::SPECIES_MUSTELINE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player8.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_tris[ ColorCodePal::SPECIES_LAPINE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player9.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 3, m_av_butters[ ColorCodePal::SPECIES_RODENT ] ); 
               _READSHAPE( 23, m_av_butters[ ColorCodePal::SPECIES_EQUINE ] );
               _READSHAPE( 43, m_av_butters[ ColorCodePal::SPECIES_FELINE ] );
               _READSHAPE( 63, m_av_butters[ ColorCodePal::SPECIES_CANINE ] );
               _READSHAPE( 83, m_av_butters[ ColorCodePal::SPECIES_MUSTELINE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player10.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_butters[ ColorCodePal::SPECIES_LAPINE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player11.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_gryffe );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player12.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_eagle );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player13.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_bats[ ColorCodePal::SPECIES_RODENT ] ); 
               _READSHAPE( 22, m_av_bats[ ColorCodePal::SPECIES_EQUINE ] );
               _READSHAPE( 42, m_av_bats[ ColorCodePal::SPECIES_FELINE ] );
               _READSHAPE( 62, m_av_bats[ ColorCodePal::SPECIES_CANINE ] );
               _READSHAPE( 82, m_av_bats[ ColorCodePal::SPECIES_MUSTELINE ] );
               _READSHAPE( 102, m_av_bats[ ColorCodePal::SPECIES_LAPINE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player14.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_normal[ ColorCodePal::SPECIES_SQUIRINE ] ); 
               _READSHAPE( 22, m_av_classics[ ColorCodePal::SPECIES_SQUIRINE ] );
               _READSHAPE( 42, m_av_tris[ ColorCodePal::SPECIES_SQUIRINE ] );
               _READSHAPE( 62, m_av_butters[ ColorCodePal::SPECIES_SQUIRINE ] );
               _READSHAPE( 82, m_av_bats[ ColorCodePal::SPECIES_SQUIRINE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player15.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_normal[ ColorCodePal::SPECIES_BOVINE ] ); 
               _READSHAPE( 22, m_av_classics[ ColorCodePal::SPECIES_BOVINE ] );
               _READSHAPE( 42, m_av_tris[ ColorCodePal::SPECIES_BOVINE ] );
               _READSHAPE( 62, m_av_butters[ ColorCodePal::SPECIES_BOVINE ] );
               _READSHAPE( 82, m_av_bats[ ColorCodePal::SPECIES_BOVINE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player16.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_primes[ ColorCodePal::SPECIES_RODENT ] ); 
               _READSHAPE( 22, m_av_primes[ ColorCodePal::SPECIES_EQUINE ] );
               _READSHAPE( 42, m_av_primes[ ColorCodePal::SPECIES_FELINE ] );
               _READSHAPE( 62, m_av_primes[ ColorCodePal::SPECIES_CANINE ] );
               _READSHAPE( 82, m_av_primes[ ColorCodePal::SPECIES_MUSTELINE ] );
               _READSHAPE( 102, m_av_primes[ ColorCodePal::SPECIES_LAPINE ] );
               _READSHAPE( 122, m_av_primes[ ColorCodePal::SPECIES_SQUIRINE ] );
               _READSHAPE( 142, m_av_primes[ ColorCodePal::SPECIES_BOVINE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player17.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_pixie );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player18.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_kitter );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player19.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_wolven );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player20.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_normal[ ColorCodePal::SPECIES_URSINE ] ); 
               _READSHAPE( 22, m_av_classics[ ColorCodePal::SPECIES_URSINE ] );
               _READSHAPE( 42, m_av_tris[ ColorCodePal::SPECIES_URSINE ] );
               _READSHAPE( 62, m_av_butters[ ColorCodePal::SPECIES_URSINE ] );
               _READSHAPE( 82, m_av_bats[ ColorCodePal::SPECIES_URSINE ] );
               _READSHAPE( 102, m_av_primes[ ColorCodePal::SPECIES_URSINE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player21.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_tygard );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player22.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_leonen );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player23.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_werewolf );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player24.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_penguin );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player25.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_bat );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player26.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_foxen );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player27.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_floxen );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player28.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_unicorn );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player29.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_furlingm );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player30.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_furlingf );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player31.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_chinchilla );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\player32.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_toaster );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\player33.fsh").c_str( ) );
            if (fsh.good( ))
            {
               //_READSHAPE( 2, m_av_bugge );
			   _READSHAPE( 2, m_av_normal[ ColorCodePal::SPECIES_BUGGE ] );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\player34.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_kirin );
            }
         }
		/* {
            yiffurc::FshFile fsh( (path + "\\player35.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_kirincloud );
            }
         } */
		 {
            yiffurc::FshFile fsh( (path + "\\player36.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_noblecaninem );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\player37.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_noblecaninef );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\player38.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_noblefelinem );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\player39.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_noblefelinef );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\player40.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_reindeer );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\player41.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_catten );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\player42.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_owlen );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\player43.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 2, m_av_lovebird );
            }
         }
         #undef _READSHAPE
      }
   };

   class PortraitAssets : public CtrlAssets
   {
   public:
      FSHSHAPE m_normal[ ColorCodePal::COUNT_SPECIES ][ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_gryffe[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_eagle[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_phoenix[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_dragon[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_kitter[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_wolven[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_tygard[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_leonen[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_werewolf[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_penguin[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_bat[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_kiwi[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_foxen[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_floxen[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_unicorn[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_furling[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_chinchilla[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_toaster[ ColorCodePal::COUNT_GENDER ];
	  //FSHSHAPE m_bugge[ ColorCodePal::COUNT_GENDER ];
      FSHSHAPE m_kirin[ ColorCodePal::COUNT_GENDER ];
	  FSHSHAPE m_noblecanine[ ColorCodePal::COUNT_GENDER ];
	  FSHSHAPE m_noblefeline[ ColorCodePal::COUNT_GENDER ];
	  FSHSHAPE m_reindeer[ ColorCodePal::COUNT_GENDER ];
	  FSHSHAPE m_catten[ ColorCodePal::COUNT_GENDER ];
	  FSHSHAPE m_owlen[ ColorCodePal::COUNT_GENDER ];
	  FSHSHAPE m_lovebird[ ColorCodePal::COUNT_GENDER ];


      void reset( )
      {
         for (unsigned i = 0; i < ColorCodePal::COUNT_SPECIES; ++i)
            for (unsigned j = 0; j < ColorCodePal::COUNT_GENDER; ++j)
               delete [] m_normal[ i ][ j ].pixels, m_normal[ i ][ j ].clear( );
         for (unsigned i = 0; i < ColorCodePal::COUNT_GENDER; ++i)
            delete [] m_gryffe[ i ].pixels, m_gryffe[ i ].clear( );
         for (unsigned i = 0; i < ColorCodePal::COUNT_GENDER; ++i)
            delete [] m_eagle[ i ].pixels, m_eagle[ i ].clear( );
         for (unsigned i = 0; i < ColorCodePal::COUNT_GENDER; ++i)
            delete [] m_phoenix[ i ].pixels, m_phoenix[ i ].clear( );
         for (unsigned i = 0; i < ColorCodePal::COUNT_GENDER; ++i)
            delete [] m_dragon[ i ].pixels, m_dragon[ i ].clear( );
      }

      template <class T_StrIter>
      void load( T_StrIter begin, const T_StrIter& end )
      {
         CtrlAssets::reset( );
         reset( );

         CtrlAssets::load( begin, end );

         const string path = string( f2_clroot( ) ) + "\\patches\\default";
         
         unsigned char* pix_buf = new unsigned char[ 256 * 256 ];

         #define _READSHAPE( num, dst ) if ((num) < fsh.getNumShapes( ))                        \
                                        {                                                       \
                                           (dst) = fsh.getShapeInfo( (num) );                   \
                                           unsigned pad = (dst).w % 4 == 0 ?                    \
                                                          0 : 4 - ((dst).w % 4);                \
                                           (dst).pixels = new unsigned char[ ((dst).w + pad) * (dst).h ]; \
                                           fsh.copyShapeBitsPadded( (num), (dst).pixels, pad ); \
                                        }
         {
            yiffurc::FshFile fsh( (path + "\\portrait.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 15, m_normal[ ColorCodePal::SPECIES_RODENT ][ ColorCodePal::GENDER_MALE ] );
               _READSHAPE( 16, m_normal[ ColorCodePal::SPECIES_EQUINE ][ ColorCodePal::GENDER_MALE ] );
               _READSHAPE( 17, m_normal[ ColorCodePal::SPECIES_FELINE ][ ColorCodePal::GENDER_MALE ] );
               _READSHAPE( 18, m_normal[ ColorCodePal::SPECIES_CANINE ][ ColorCodePal::GENDER_MALE ] );
               _READSHAPE( 19, m_normal[ ColorCodePal::SPECIES_MUSTELINE ][ ColorCodePal::GENDER_MALE ] );

               _READSHAPE( 20, m_normal[ ColorCodePal::SPECIES_RODENT ][ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 21, m_normal[ ColorCodePal::SPECIES_EQUINE ][ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 22, m_normal[ ColorCodePal::SPECIES_FELINE ][ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 23, m_normal[ ColorCodePal::SPECIES_CANINE ][ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 24, m_normal[ ColorCodePal::SPECIES_MUSTELINE ][ ColorCodePal::GENDER_FEMALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt2.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_normal[ ColorCodePal::SPECIES_RODENT ][ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_normal[ ColorCodePal::SPECIES_EQUINE ][ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 2, m_normal[ ColorCodePal::SPECIES_FELINE ][ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 3, m_normal[ ColorCodePal::SPECIES_CANINE ][ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 4, m_normal[ ColorCodePal::SPECIES_MUSTELINE ][ ColorCodePal::GENDER_UNSPEC ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt3.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_dragon[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_dragon[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_dragon[ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt4.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_phoenix[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_phoenix[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_phoenix[ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt5.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_normal[ ColorCodePal::SPECIES_LAPINE ][ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_normal[ ColorCodePal::SPECIES_LAPINE ][ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_normal[ ColorCodePal::SPECIES_LAPINE ][ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt6.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_gryffe[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_gryffe[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_gryffe[ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt7.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_eagle[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_eagle[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_eagle[ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt8.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_normal[ ColorCodePal::SPECIES_SQUIRINE ][ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_normal[ ColorCodePal::SPECIES_SQUIRINE ][ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_normal[ ColorCodePal::SPECIES_SQUIRINE ][ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt9.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_kitter[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_kitter[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_kitter[ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt10.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_normal[ ColorCodePal::SPECIES_BOVINE ][ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_normal[ ColorCodePal::SPECIES_BOVINE ][ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_normal[ ColorCodePal::SPECIES_BOVINE ][ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt11.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_wolven[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_wolven[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_wolven[ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt12.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_normal[ ColorCodePal::SPECIES_URSINE ][ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_normal[ ColorCodePal::SPECIES_URSINE ][ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_normal[ ColorCodePal::SPECIES_URSINE ][ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt13.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_tygard[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_tygard[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_tygard[ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt14.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_leonen[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_leonen[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_leonen[ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt15.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_werewolf[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_werewolf[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_werewolf[ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt16.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_penguin[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_penguin[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_penguin[ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt17.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_bat[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_bat[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_bat[ ColorCodePal::GENDER_MALE ] );
            }
         }

         {
            yiffurc::FshFile fsh( (path + "\\portrt18.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_kiwi[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_kiwi[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_kiwi[ ColorCodePal::GENDER_MALE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\portrt19.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_foxen[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_foxen[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_foxen[ ColorCodePal::GENDER_MALE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\portrt20.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_floxen[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_floxen[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_floxen[ ColorCodePal::GENDER_MALE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\portrt21.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_unicorn[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_unicorn[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_unicorn[ ColorCodePal::GENDER_MALE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\portrt22.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_furling[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_furling[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_furling[ ColorCodePal::GENDER_MALE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\portrt23.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_chinchilla[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_chinchilla[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_chinchilla[ ColorCodePal::GENDER_MALE ] );
            }
         }
         {
            yiffurc::FshFile fsh( (path + "\\portrt24.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_toaster[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_toaster[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_toaster[ ColorCodePal::GENDER_MALE ] );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\portrt25.fsh").c_str( ) );
            if (fsh.good( ))
            {
			 _READSHAPE( 0, m_normal[ ColorCodePal::SPECIES_BUGGE ][ ColorCodePal::GENDER_UNSPEC ] );
			 _READSHAPE( 1, m_normal[ ColorCodePal::SPECIES_BUGGE ][ ColorCodePal::GENDER_FEMALE ] );
			 _READSHAPE( 2, m_normal[ ColorCodePal::SPECIES_BUGGE ][ ColorCodePal::GENDER_MALE ] );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\portrt26.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_kirin[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_kirin[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_kirin[ ColorCodePal::GENDER_MALE ] );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\portrt27.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_noblecanine[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_noblecanine[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_noblecanine[ ColorCodePal::GENDER_MALE ] );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\portrt28.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_noblefeline[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_noblefeline[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_noblefeline[ ColorCodePal::GENDER_MALE ] );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\portrt29.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_reindeer[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_reindeer[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_reindeer[ ColorCodePal::GENDER_MALE ] );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\portrt30.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_catten[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_catten[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_catten[ ColorCodePal::GENDER_MALE ] );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\portrt31.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_owlen[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_owlen[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_owlen[ ColorCodePal::GENDER_MALE ] );
            }
         }
		 {
            yiffurc::FshFile fsh( (path + "\\portrt32.fsh").c_str( ) );
            if (fsh.good( ))
            {
               _READSHAPE( 0, m_lovebird[ ColorCodePal::GENDER_UNSPEC ] );
               _READSHAPE( 1, m_lovebird[ ColorCodePal::GENDER_FEMALE ] );
               _READSHAPE( 2, m_lovebird[ ColorCodePal::GENDER_MALE ] );
            }
         }
         #undef _READSHAPE
      }
   };

   ButtonAssets m_register;
   ButtonAssets m_play;
   ButtonAssets m_close;
   ButtonAssets m_save;
   ButtonAssets m_saveas;
   ButtonAssets m_settings;

   PortraitAssets m_portrait;
   PreviewAssets  m_preview;

   CtrlAssets m_characters;
   CtrlAssets m_name;
   CtrlAssets m_password;
   CtrlAssets m_description;
   CtrlAssets m_species;
   CtrlAssets m_gender;
   CtrlAssets m_code;
   CtrlAssets m_colors;
   SwatchAssets m_swatch;
   CtrlAssets m_digo;
   TextAssets m_filename;
   TextAssets m_status;

   BgAssets m_background;

   Assets( const string& file )
   {
      ifstream is( file.c_str( ) );
      vector<string> args;
      
      if (is.fail( ))
         f2_errspeak( "Curses: Couldn't load UI file." );

      for (string line; is.good( ); )
      {
         getline( is, line );
         if (line_split( line, &args ))
         {
            vector<string>::const_iterator begin = args.begin( );
            ++begin;
            vector<string>::const_iterator end = args.end( );

            if (args.front( ) == "ctlRegister")
               m_register.load( "register", begin, end );
            else if (args.front( ) == "ctlPlay")
               m_play.load( "play", begin, end );
            else if (args.front( ) == "ctlClose")
               m_close.load( "close", begin, end );
            else if (args.front( ) == "ctlSave")
               m_save.load( "save", begin, end );
            else if (args.front( ) == "ctlSaveAs")
               m_saveas.load( "saveas", begin, end );
            else if (args.front( ) == "ctlSettings")
               m_settings.load( "settings", begin, end );

            else if (args.front( ) == "ctlPortrait")
               m_portrait.load( begin, end );
            else if (args.front( ) == "ctlPreview")
               m_preview.load( begin, end );

            else if (args.front( ) == "ctlCharacters")
               m_characters.load( begin, end );
            else if (args.front( ) == "ctlName")
               m_name.load( begin, end );
            else if (args.front( ) == "ctlPassword")
               m_password.load( begin, end );
            else if (args.front( ) == "ctlDescription")
               m_description.load( begin, end );
            else if (args.front( ) == "ctlSpecies")
               m_species.load( begin, end );
            else if (args.front( ) == "ctlGender")
               m_gender.load( begin, end );
            else if (args.front( ) == "ctlCode")
               m_code.load( begin, end );
            else if (args.front( ) == "ctlColors")
               m_colors.load( begin, end );
            else if (args.front( ) == "ctlPalette")
               m_swatch.load( begin, end );
            else if (args.front( ) == "ctlDigo")
               m_digo.load( begin, end );

            else if (args.front( ) == "ctlFileName")
               m_filename.load( begin, end );
            else if (args.front( ) == "ctlStatus")
               m_status.load( begin, end );
         }
      }

      m_background.load( );
   }
};

//////////////////////////////////////////////////////////////////////////

CursesScreen::CursesScreen( const LOGININFO& info )
   : m_connected( false )
{
   assert( Curses::singleton( ) );
   _initOnce( );
   refreshCharacters( );
   fillForm( info );
}

CursesScreen::CursesScreen( )
   : m_connected( false )
{
   assert( Curses::singleton( ) );
   _initOnce( );
   refreshCharacters( );
   if (!selectIniFile( Curses::singleton( )->getDefaultIni( ) ))
      if (m_inis.size( ))
         selectIniFile( m_inis.front( ) );
}

CursesScreen::~CursesScreen( )
{
   // Will destroy child windows.
   DestroyWindow( m_controls.m_background );

   Swatch_Uninit( Curses::singleton( )->getInstance( ) );

   UnregisterClass( CONTAINER_CLASS, Curses::singleton( )->getInstance( ) );

   delete m_assets;

   delete m_fshbmpinfo;

   DestroyMenu( m_menus );
}

void CursesScreen::_initOnce( )
{
   m_menus = LoadMenu( Curses::singleton( )->getInstance( ), MAKEINTRESOURCE( IDR_SCREEN_MENU ) );

   m_fshbmpinfo = new FshBitmapInfo;

   {
      string uifile = f2_modroot( Curses::singleton( )->getModule( ) );
      uifile += "\\ui\\ui.txt";

      m_assets = new Assets( uifile );
   }

   {
      memset( &m_controls, 0, sizeof( m_controls ) );

      HWND parent = Curses::singleton( )->getWnd( );
      HINSTANCE inst = Curses::singleton( )->getInstance( );

      {
         WNDCLASSEX wc;
         memset( &wc, 0, sizeof( wc ) );
         wc.cbSize = sizeof( wc );
         wc.hbrBackground = GetStockBrush( NULL_BRUSH );
         wc.hCursor = LoadCursor( NULL, IDC_ARROW );
         wc.lpszClassName = CONTAINER_CLASS;
         wc.style = CS_PARENTDC | CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
         wc.lpfnWndProc = container_proc;
         wc.hInstance = inst;
         RegisterClassEx( &wc );

         Swatch_Init( inst );
      }
      
      m_controls.m_background = CreateWindowEx( 0, CONTAINER_CLASS, "",
                                                WS_VISIBLE | WS_CHILD,
                                                0, 0, 640, 480,
                                                parent, NULL, inst, this );

      /* Fields. */
      m_controls.m_characters = CreateWindowEx( 0, WC_LISTBOX, "",
                                                WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL |
                                                LBS_HASSTRINGS | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT,
                                                m_assets->m_characters.x, m_assets->m_characters.y, 
                                                m_assets->m_characters.w, m_assets->m_characters.h,
                                                m_controls.m_background, 
                                                (HMENU) CTRL_CHARACTERS, inst, NULL );

      m_controls.m_portrait = CreateWindowEx( 0, WC_STATIC, "",
                                              WS_VISIBLE | WS_CHILD | WS_BORDER |
                                              SS_OWNERDRAW,
                                              m_assets->m_portrait.x, m_assets->m_portrait.y, 
                                              m_assets->m_portrait.w, m_assets->m_portrait.h,
                                              m_controls.m_background, 
                                              (HMENU) CTRL_PORTRAIT, inst, NULL );

      m_controls.m_name = CreateWindowEx( 0, WC_EDIT, "",
                                          WS_VISIBLE | WS_CHILD | WS_BORDER |
                                          ES_LEFT,
                                          m_assets->m_name.x, m_assets->m_name.y, 
                                          m_assets->m_name.w, m_assets->m_name.h,
                                          m_controls.m_background, 
                                          (HMENU) CTRL_NAME, inst, NULL );

      m_controls.m_password = CreateWindowEx( 0, WC_EDIT, "",
                                              WS_VISIBLE | WS_CHILD | WS_BORDER |
                                              ES_LEFT | ES_PASSWORD,
                                              m_assets->m_password.x, m_assets->m_password.y, 
                                              m_assets->m_password.w, m_assets->m_password.h,
                                              m_controls.m_background, 
                                              (HMENU) CTRL_PASSWORD, inst, NULL );

      m_controls.m_species = CreateWindowEx( 0, WC_COMBOBOX, "",
                                             WS_VISIBLE | WS_CHILD | WS_BORDER |
                                             CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_NOINTEGRALHEIGHT |
                                             CBS_SORT,
                                             m_assets->m_species.x, m_assets->m_species.y, 
                                             m_assets->m_species.w, m_assets->m_species.h,
                                             m_controls.m_background, 
                                             (HMENU) CTRL_SPECIES, inst, NULL );

      m_controls.m_gender = CreateWindowEx( 0, WC_COMBOBOX, "",
                                            WS_VISIBLE | WS_CHILD | WS_BORDER |
                                            CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_NOINTEGRALHEIGHT |
                                            CBS_SORT,
                                            m_assets->m_gender.x, m_assets->m_gender.y, 
                                            m_assets->m_gender.w, m_assets->m_gender.h,
                                            m_controls.m_background, 
                                            (HMENU) CTRL_GENDER, inst, NULL );

      m_controls.m_code = CreateWindowEx( 0, WC_EDIT, "",
                                          WS_VISIBLE | WS_CHILD | WS_BORDER |
                                          ES_LEFT,
                                          m_assets->m_code.x, m_assets->m_code.y, 
                                          m_assets->m_code.w, m_assets->m_code.h,
                                          m_controls.m_background, 
                                          (HMENU) CTRL_CODE, inst, NULL );

      m_controls.m_description = CreateWindowEx( 0, WC_EDIT, "",
                                                 WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL |
                                                 ES_LEFT | ES_MULTILINE,
                                                 m_assets->m_description.x, m_assets->m_description.y, 
                                                 m_assets->m_description.w, m_assets->m_description.h,
                                                 m_controls.m_background, 
                                                 (HMENU) CTRL_DESCRIPTION, inst, NULL );

      m_controls.m_colors = CreateWindowEx( 0, WC_COMBOBOX, "",
                                            WS_VISIBLE | WS_CHILD | WS_BORDER |
                                            CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_NOINTEGRALHEIGHT |
                                            CBS_SORT,
                                            m_assets->m_colors.x, m_assets->m_colors.y, 
                                            m_assets->m_colors.w, m_assets->m_colors.h,
                                            m_controls.m_background, 
                                            (HMENU) CTRL_COLORS, inst, NULL );

      m_controls.m_swatch = CreateWindowEx( 0, WC_SWATCH, "",
                                             WS_VISIBLE | WS_CHILD | WS_BORDER,
                                             m_assets->m_swatch.x, m_assets->m_swatch.y, 
                                             m_assets->m_swatch.w, m_assets->m_swatch.h,
                                             m_controls.m_background, 
                                             (HMENU) CTRL_SWATCH, inst, NULL );

      m_controls.m_preview = CreateWindowEx( 0, WC_STATIC, "",
                                             WS_VISIBLE | WS_CHILD | WS_BORDER |
                                             SS_OWNERDRAW,
                                             m_assets->m_preview.x, m_assets->m_preview.y, 
                                             m_assets->m_preview.w, m_assets->m_preview.h,
                                             m_controls.m_background, 
                                             (HMENU) CTRL_PREVIEW, inst, NULL );

      m_controls.m_digo = CreateWindowEx( 0, WC_LISTBOX, "",
                                          WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL |
                                          LBS_SORT | LBS_HASSTRINGS | LBS_NOTIFY | 
                                          LBS_NOINTEGRALHEIGHT,
                                          m_assets->m_digo.x, m_assets->m_digo.y, 
                                          m_assets->m_digo.w, m_assets->m_digo.h,
                                          m_controls.m_background, 
                                          (HMENU) CTRL_DIGO, inst, NULL );

      m_controls.m_filename = CreateWindowEx( WS_EX_TRANSPARENT, WC_STATIC, "",
                                              WS_VISIBLE | WS_CHILD | 
                                              SS_LEFT | SS_NOPREFIX | SS_PATHELLIPSIS,
                                              m_assets->m_filename.x, m_assets->m_filename.y,
                                              m_assets->m_filename.w, m_assets->m_filename.h,
                                              m_controls.m_background,
                                              (HMENU) CTRL_FILENAME, inst, NULL );

      m_controls.m_status = CreateWindowEx( WS_EX_TRANSPARENT, WC_STATIC, "",
                                            WS_VISIBLE | WS_CHILD | 
                                            SS_RIGHT | SS_NOPREFIX | SS_WORDELLIPSIS,
                                            m_assets->m_status.x, m_assets->m_status.y,
                                            m_assets->m_status.w, m_assets->m_status.h,
                                            m_controls.m_background,
                                            (HMENU) CTRL_STATUS, inst, NULL );

      /* Buttons */
      m_controls.m_register = CreateWindowEx( 0, "BUTTON", "Register",
                                              WS_CHILD | BS_OWNERDRAW | WS_VISIBLE, 
                                              m_assets->m_register.x, m_assets->m_register.y, 
                                              m_assets->m_register.w, m_assets->m_register.h,
                                              m_controls.m_background, 
                                              (HMENU) CTRL_REGISTER, inst, NULL );
      m_controls.m_close    = CreateWindowEx( 0, "BUTTON", "Close",
                                              WS_CHILD | BS_OWNERDRAW | WS_VISIBLE, 
                                              m_assets->m_close.x, m_assets->m_close.y, 
                                              m_assets->m_close.w, m_assets->m_close.h,
                                              m_controls.m_background, 
                                              (HMENU) CTRL_CLOSE, inst, NULL );
      m_controls.m_play     = CreateWindowEx( 0, "BUTTON", "Play",
                                              WS_CHILD | BS_OWNERDRAW | WS_VISIBLE, 
                                              m_assets->m_play.x, m_assets->m_play.y, 
                                              m_assets->m_play.w, m_assets->m_play.h,
                                              m_controls.m_background, 
                                              (HMENU) CTRL_PLAY, inst, NULL );
      m_controls.m_save     = CreateWindowEx( 0, "BUTTON", "Save",
                                              WS_CHILD | BS_OWNERDRAW | WS_VISIBLE, 
                                              m_assets->m_save.x, m_assets->m_save.y, 
                                              m_assets->m_save.w, m_assets->m_save.h,
                                              m_controls.m_background, 
                                              (HMENU) CTRL_SAVE, inst, NULL );
      m_controls.m_saveas   = CreateWindowEx( 0, "BUTTON", "Save As",
                                              WS_CHILD | BS_OWNERDRAW | WS_VISIBLE, 
                                              m_assets->m_saveas.x, m_assets->m_saveas.y, 
                                              m_assets->m_saveas.w, m_assets->m_saveas.h,
                                              m_controls.m_background, 
                                              (HMENU) CTRL_SAVEAS, inst, NULL );
      m_controls.m_settings = CreateWindowEx( 0, "BUTTON", "Settings",
                                              WS_CHILD | BS_OWNERDRAW | WS_VISIBLE, 
                                              m_assets->m_settings.x, m_assets->m_settings.y, 
                                              m_assets->m_settings.w, m_assets->m_settings.h,
                                              m_controls.m_background, 
                                              (HMENU) CTRL_SETTINGS, inst, NULL );
   }

   /* Set up controls. */
   {
      SetWindowFont( m_controls.m_characters, GetStockFont( DEFAULT_GUI_FONT ), FALSE );
      SetWindowFont( m_controls.m_name, GetStockFont( DEFAULT_GUI_FONT ), FALSE );
      SetWindowFont( m_controls.m_password, GetStockFont( DEFAULT_GUI_FONT ), FALSE );
      SetWindowFont( m_controls.m_species, GetStockFont( DEFAULT_GUI_FONT ), FALSE );
      SetWindowFont( m_controls.m_gender, GetStockFont( DEFAULT_GUI_FONT ), FALSE );
      SetWindowFont( m_controls.m_code, GetStockFont( DEFAULT_GUI_FONT ), FALSE );
      SetWindowFont( m_controls.m_description, GetStockFont( DEFAULT_GUI_FONT ), FALSE );
      SetWindowFont( m_controls.m_colors, GetStockFont( DEFAULT_GUI_FONT ), FALSE );
      SetWindowFont( m_controls.m_digo, GetStockFont( DEFAULT_GUI_FONT ), FALSE );
      SetWindowFont( m_controls.m_filename, m_assets->m_filename.font, FALSE );
      SetWindowFont( m_controls.m_status, m_assets->m_status.font, FALSE );

      SetWindowLongPtr( m_controls.m_description, GWLP_USERDATA, 
         SetWindowLongPtr( m_controls.m_description, GWLP_WNDPROC, 
                           (LONG_PTR) edit_proc ) );

      SetWindowLongPtr( m_controls.m_name, GWLP_USERDATA, 
         SetWindowLongPtr( m_controls.m_name, GWLP_WNDPROC, 
                           (LONG_PTR) edit_proc ) );

      SetWindowLongPtr( m_controls.m_code, GWLP_USERDATA, 
         SetWindowLongPtr( m_controls.m_code, GWLP_WNDPROC, 
                           (LONG_PTR) edit_proc ) );

      SetWindowLongPtr( m_controls.m_password, GWLP_USERDATA, 
         SetWindowLongPtr( m_controls.m_password, GWLP_WNDPROC, 
                           (LONG_PTR) edit_proc ) );

      SendMessage( m_controls.m_description, EM_SETLIMITTEXT, 768, 0 );
      SendMessage( m_controls.m_code, EM_SETLIMITTEXT, 14, 0 );

      {
         const char* const strings[] = 
         {
            "Fur", "Markings", "Hair", "Eyes", "Badge",
            "Vest", "Bracers", "Cape", "Boots", "Trousers"
         };

         for (unsigned i = 0; i < sizeof( strings ) / sizeof( *strings ); ++i)
         {
            int n = SendMessage( m_controls.m_colors, CB_ADDSTRING, 0, (LPARAM) strings[ i ] );
            SendMessage( m_controls.m_colors, CB_SETITEMDATA, n, i );
         }
         SendMessage( m_controls.m_colors, CB_SETCURSEL, 0, 0 );
      }

      {
         Swatch_SetSwatchSize( m_controls.m_swatch, 13 );
         const unsigned i = 
            SendMessage( m_controls.m_colors, CB_GETITEMDATA, 
                         SendMessage( m_controls.m_colors, CB_GETCURSEL, 0, 0 ), 0 );
         assert( i < 10 );
         Swatch_Fill( m_controls.m_swatch, 
                      m_assets->m_swatch.m_counts[ i ],
                      m_assets->m_swatch.m_swatches[ i ] );
      }

      {
         const char* const strings[] = 
         {
            "Rodent", "Equine", "Feline", "Canine", 
            "Musteline", "Lapine", "Squirine", "Bovine",
            "Ursine", "Bugge"
         };

         for (unsigned i = 0; i < sizeof( strings ) / sizeof( *strings ); ++i)
         {
            int n = SendMessage( m_controls.m_species, CB_ADDSTRING, 0, (LPARAM) strings[ i ] );
            SendMessage( m_controls.m_species, CB_SETITEMDATA, n, i );
         }
         SendMessage( m_controls.m_species, CB_SETCURSEL, 0, 0 );
      }

      {
         const char* const strings[] = 
         {
            "Female", "Male", "Unspecified",
         };

         for (unsigned i = 0; i < sizeof( strings ) / sizeof( *strings ); ++i)
         {
            int n = SendMessage( m_controls.m_gender, CB_ADDSTRING, 0, (LPARAM) strings[ i ] );
            SendMessage( m_controls.m_gender, CB_SETITEMDATA, n, i );
         }
         SendMessage( m_controls.m_gender, CB_SETCURSEL, 0, 0 );
      }

      {
         const char* const strings[] = 
         {
            "(None)",
            "Classic wings", "Tri-wings", "Butterfly Wings", "Bat Wings", "Prime Wings",
            "Dragon", "Phoenix", "Gryffe", "Eagle", "Kiwi", "Kitterwing", "Kitterwing Pixie",
            "Wolven", "Tygard", "Leonen", "Werewolf", "Penguin", "Bat", "Foxen", "Floxen",
            "Unicorn", "Furling", "Chinchilla", "Toaster", "Kirin", "Noble Canine",
			"Noble Feline", "Reindeer", "Catten", "Owlen", "Lovebird"
         };

         for (unsigned i = 0; i < sizeof( strings ) / sizeof( *strings ); ++i)
         {
            int n = SendMessage( m_controls.m_digo, LB_ADDSTRING, 0, (LPARAM) strings[ i ] );
            SendMessage( m_controls.m_digo, LB_SETITEMDATA, n, i );
         }
         SendMessage( m_controls.m_digo, LB_SETCURSEL, 0, 0 );
      }

      // Subclass the characters listbox.
      SetWindowLongPtr( m_controls.m_characters, GWLP_USERDATA, 
                        (LONG_PTR) GetWindowLongPtr( m_controls.m_characters, GWLP_WNDPROC ) );
      SetWindowLongPtr( m_controls.m_characters, GWLP_WNDPROC, (LONG_PTR) lb_proc );
   }
}

void CursesScreen::setConnected( bool connected )
{
   const char* const sz = ((m_connected = connected) ? 
                           "Status: Connected" : "Status: Disconnected");
   InvalidateParent( m_controls.m_status, FALSE );
   SetWindowText( m_controls.m_status, sz );
   
}

void CursesScreen::syncColorCode( )
{
   // Color code field.
   {
      const unsigned caret = HIWORD( Edit_GetSel( m_controls.m_code ) );
      SetWindowText( m_controls.m_code, ("t" + m_colorcode.str( )).c_str( ) );
      Edit_SetSel( m_controls.m_code, caret, caret );
   }

   // Update the swatches.
   {
      assert( SendMessage( m_controls.m_colors, CB_GETCURSEL, 0, 0 ) != -1 );
      unsigned i = SendMessage( m_controls.m_colors, CB_GETITEMDATA, 
                                SendMessage( m_controls.m_colors, CB_GETCURSEL, 0, 0 ), 0 );
      assert( i < ColorCodePal::COLORS_LEN );
      Swatch_SetSel( m_controls.m_swatch, m_colorcode.getColor( i ) );
   }

   assert( ComboBox_FindFirst( m_controls.m_species, m_colorcode.getGsd( m_colorcode.GSD_SPECIES ) ) != -1 );
   SendMessage( m_controls.m_species, CB_SETCURSEL, 
                ComboBox_FindFirst( m_controls.m_species, m_colorcode.getGsd( m_colorcode.GSD_SPECIES ) ), 0 );

   assert( ComboBox_FindFirst( m_controls.m_gender, m_colorcode.getGsd( m_colorcode.GSD_GENDER ) ) != -1 );
   SendMessage( m_controls.m_gender, CB_SETCURSEL, 
                ComboBox_FindFirst( m_controls.m_gender, m_colorcode.getGsd( m_colorcode.GSD_GENDER ) ), 0 );

   InvalidateRect( m_controls.m_preview, NULL, TRUE );
   InvalidateRect( m_controls.m_portrait, NULL, TRUE );
}

void CursesScreen::refreshCharacters( )
{
   string sel_file;

   if (ListBox_GetCurSel( m_controls.m_characters ) != -1)
   {
      assert( ListBox_GetItemData( m_controls.m_characters, 
               ListBox_GetCurSel( m_controls.m_characters ) ) );

      sel_file = *(string*) ListBox_GetItemData( m_controls.m_characters, 
                               ListBox_GetCurSel( m_controls.m_characters ) );
   }

   m_inis.clear( );
   ListBox_ResetContent( m_controls.m_characters );

   WIN32_FIND_DATA wfd;
   const list<string>& paths = Curses::singleton( )->getIniPaths( );
   const string def_ini = Curses::singleton( )->getDefaultIni( );

   for (list<string>::const_iterator i = paths.begin( ); i != paths.end( ); ++i)
   {
      HANDLE h_find = FindFirstFile( (*i + "\\*.ini").c_str( ), &wfd );
      if (h_find != INVALID_HANDLE_VALUE)
      {
         do 
         {
            string file = (*i + "\\") + wfd.cFileName;

            if (GetFileAttributes( file.c_str( ) ) & FILE_ATTRIBUTE_DIRECTORY)
               continue;

            CharacterIni ini( file );
            if (ini.read( ))
            {
               string name = unpipen( ini.getName( ) );
               if (_stricmp( ini.getFile( ).c_str( ), def_ini.c_str( ) ) == 0)
                  name += " (Default)";

               m_inis.push_back( ini.getFile( ) );
               ListBox_SetItemData( m_controls.m_characters, 
                  ListBox_AddString( m_controls.m_characters, name.c_str( ) ),
                                     &m_inis.back( ) );
            }              
            
         } while (FindNextFile( h_find, &wfd ));
         FindClose( h_find );
      }
   }

   {
      // Re-select the selected file.
      list<string>::const_iterator i = find( m_inis.begin( ), m_inis.end( ), sel_file );
      if (i != m_inis.end( ))
      {
         assert( ListBox_FindFirst( m_controls.m_characters, (LPARAM) &*i ) != -1 );
         ListBox_SetCurSel( m_controls.m_characters, 
            ListBox_FindFirst( m_controls.m_characters, (LPARAM) &*i ) );
      }
   }
}

bool CursesScreen::selectIniFile( const string& name )
{
   bool success = false;

   LOGININFO li;
   getLoginInfo( &li );
   CharacterIni ini( name );
   if (ini.read( ))
   {
      li.name = ini.getName( );
      li.password = ini.getPassword( );
      li.description = ini.getDescription( );
      li.color.clear( );
      if (!ini.getColorCode( ).empty( ))
         li.color = ini.getColorCode( ).substr( 1 );
      success = true;
   }

   li.filename = name;
   fillForm( li );
   return success;
}

void CursesScreen::fillForm( const LOGININFO& info )
{
   m_curr_ini = info.filename;
   m_colorcode = info.color;
   
   SetWindowText( m_controls.m_name, unpipen( info.name ).c_str( ) );
   // Don't ignore passwords?
   if (Curses::singleton( )->getOptions( ) & Curses::OPT_REMEMBERPW)
      SetWindowText( m_controls.m_password, info.password.c_str( ) );
   SetWindowText( m_controls.m_description, info.description.c_str( ) );

   InvalidateParent( m_controls.m_filename, FALSE );
   if (m_curr_ini.length( ))
   {
      if (isUnsaved( ))
         SetWindowText( m_controls.m_filename, (m_curr_ini + "*").c_str( ) );
      else
         SetWindowText( m_controls.m_filename, m_curr_ini.c_str( ) );
   }
   else
      SetWindowText( m_controls.m_filename, "New INI File*" );

   syncColorCode( );

   // Select the file if it's in the listbox.
   ListBox_SetCurSel( m_controls.m_characters, -1 );
   for (list<string>::const_iterator i = m_inis.begin( );
        i != m_inis.end( ); ++i)
   {
      if (!_stricmp( i->c_str( ), m_curr_ini.c_str( ) ))
      {
         const unsigned sel = ListBox_FindFirst( m_controls.m_characters, (LPARAM) &*i );
         if (sel != -1)            
            ListBox_SetCurSel( m_controls.m_characters, sel );
         break;
      }
   }
}

void CursesScreen::getLoginInfo( LOGININFO* out ) const
{
   {
      char sz[ 768 ];
      GetWindowText( m_controls.m_name, sz, sizeof( sz ) / sizeof( *sz ) );
      out->name = pipen( sz );
      GetWindowText( m_controls.m_password, sz, sizeof( sz ) / sizeof( *sz ) );
      out->password = sz;
      GetWindowText( m_controls.m_description, sz, sizeof( sz ) / sizeof( *sz ) );
      out->description = sz;
   }

   assert( ListBox_GetCurSel( m_controls.m_digo ) != -1 );
   out->digo = (Digo_Type) 
      ListBox_GetItemData( m_controls.m_digo, ListBox_GetCurSel( m_controls.m_digo ) );

   out->color = m_colorcode;
   out->filename = m_curr_ini;
}

void CursesScreen::getCharIni( CharacterIni* ini )
{
   LOGININFO li;
   getLoginInfo( &li );
   ini->setFile( li.filename );
   ini->setColorCode( "t" + li.color.str( ) );
   ini->setDescription( li.description );
   ini->setLastLogin( 0 );
   ini->setName( li.name );
   ini->setNumLogins( 0 );
   ini->setPassword( li.password );
   {
      string species( 3, '0' );
      species[ 0 ] = li.color.getGsd( li.color.GSD_GENDER ) + '0';
      species[ 1 ] = li.color.getGsd( li.color.GSD_SPECIES ) + '0';
      species[ 2 ] = li.color.getGsd( li.color.GSD_DIGO ) + '0';
      ini->setSpecies( species );
   }

   // Pull the login stuff from disk.
   if (!m_curr_ini.empty( ))
   {
      CharacterIni disk( m_curr_ini );
      if (disk.read( ))
      {
         ini->setNumLogins( disk.getNumLogins( ) );
         ini->setLastLogin( disk.getLastLogin( ) );
      }
   }
}

bool CursesScreen::isUnsaved( ) const
{
   if (m_curr_ini.length( ))
   {
      CharacterIni ini( m_curr_ini );
      if (ini.read( ))
      {
         LOGININFO li;
         getLoginInfo( &li );

         // Ignore passwords?
         if (!(Curses::singleton( )->getOptions( ) & Curses::OPT_REMEMBERPW))
         {
            li.password.clear( );
            ini.setPassword( "" );
         }

         return (!li.isEmpty( ) && li != ini);
      }
   }
   return true;
}

bool CursesScreen::promptSave( )
{
   if (isUnsaved( ))
   {
      int r = MessageBox( Curses::singleton( )->getWnd( ), "Save changes to the current file?",
                          "Curses", MB_YESNOCANCEL | MB_ICONQUESTION );
      switch (r)
      {
      case IDYES:
         {
            save( );
         }
      case IDNO:
         break;
      case IDCANCEL:
         return false;
      }
   }

   return true;
}

bool CursesScreen::save( )
{
   if (m_curr_ini.empty( ))
      return saveAs( );

   CharacterIni ini;
   getCharIni( &ini );
   // Ignore passwords?
   if (!(Curses::singleton( )->getOptions( ) & Curses::OPT_REMEMBERPW))
      ini.setPassword( "" );
   ini.write( );
   refreshCharacters( );
   markUnsaved( false );

   return true;
}

bool CursesScreen::saveAs( )
{
   if (m_last_save_dir.empty( ))
   {
      if (Curses::singleton( )->getIniPaths( ).empty( ))
         m_last_save_dir = string( f2_cldocroot( ) ) + "\\Furcadia Characters";
      else
         m_last_save_dir = Curses::singleton( )->getIniPaths( ).front( );
   }

   OPENFILENAME ofn;
   memset( &ofn, 0, sizeof( ofn ) );
   ofn.lStructSize = sizeof( ofn );
   ofn.hwndOwner = Curses::singleton( )->getWnd( );
   ofn.lpstrFilter = "Character INI Files (*.ini)\0*.ini\0All Files (*.*)\0*.*\0\0";
   ofn.nFilterIndex = 1;
   ofn.lpstrFile = new char[ MAX_PATH ];
   ofn.nMaxFile = MAX_PATH;
   ofn.lpstrInitialDir = NULL;
   ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN | OFN_NOCHANGEDIR;
   ofn.lpstrDefExt = "ini";

   {
      LOGININFO li;
      getLoginInfo( &li );
      strcpy( ofn.lpstrFile, (m_last_save_dir + "\\" + makefilename( li.name ) + ".ini").c_str( ) );
   }

   if (GetSaveFileName( &ofn ))
   {
      assert( strlen( ofn.lpstrFile ) );

      m_curr_ini = ofn.lpstrFile;
      delete [] ofn.lpstrFile;

      CharacterIni ini;
      getCharIni( &ini );
      // Ignore passwords?
      if (!(Curses::singleton( )->getOptions( ) & Curses::OPT_REMEMBERPW))
         ini.setPassword( "" );
      ini.write( );

      m_last_save_dir.assign( m_curr_ini, 0, m_curr_ini.find_last_of( "\\" ) );

      refreshCharacters( );

      // Select the new INI.
      LOGININFO li;
      getLoginInfo( &li );
      fillForm( li );
      markUnsaved( false );

      return true;
   }

   delete [] ofn.lpstrFile;
   return false;
}

void CursesScreen::newChar( )
{
   // Use an empty login info.
   LOGININFO li;
   fillForm( li );
}

void CursesScreen::deleteChar( )
{
   if (!m_curr_ini.empty( ))
   {
      if (IDOK == MessageBox( Curses::singleton( )->getWnd( ), ("Really delete \"" + m_curr_ini + "\"?").c_str( ),
                              "Confirm Delete", MB_OKCANCEL |MB_ICONQUESTION ))
      {
         const int cur_sel = ListBox_GetCurSel( m_controls.m_characters );

         DeleteFile( m_curr_ini.c_str( ) );
         refreshCharacters( );

         if (ListBox_GetCount( m_controls.m_characters ) > cur_sel)
         {
            assert( ListBox_GetItemData( m_controls.m_characters, cur_sel ) );  
            selectIniFile( *(string*) ListBox_GetItemData( m_controls.m_characters, cur_sel ) );
         }
         else
            newChar( );         
      }
   }
}

void CursesScreen::markUnsaved( bool unsaved )
{
   InvalidateParent( m_controls.m_filename, FALSE );
   if (unsaved)
   {
      if (m_curr_ini.length( ))
         SetWindowText( m_controls.m_filename, (m_curr_ini + "*").c_str( ) );
      else
         SetWindowText( m_controls.m_filename, "New INI File*" );
   }
   else
   {
      SetWindowText( m_controls.m_filename, m_curr_ini.c_str( ) );
   }
}

bool CursesScreen::onWndProc( WNDPROCPARAMS* params )
{
   switch (params->msg)
   {
   case WM_SIZE:
      // Resize the container.
      SetWindowPos( m_controls.m_background, NULL, 0, 0, 
                    LOWORD( params->lparam ), HIWORD( params->lparam ), 
                    SWP_NOMOVE | SWP_NOZORDER );
      return false;
   }

   return false;
}

void CursesScreen::setCloseable( bool onoff )
{
   if (onoff)
   {
      EnableWindow( m_controls.m_close, TRUE );
      ShowWindow( m_controls.m_close, SW_SHOWNORMAL );
   }
   else
   {
      EnableWindow( m_controls.m_close, FALSE );
      ShowWindow( m_controls.m_close, SW_HIDE );
   }
}

LRESULT CALLBACK CursesScreen::container_proc( HWND wnd, UINT msg, 
                                               WPARAM wparam, LPARAM lparam )
{
#define _INST_PTR( ) ((CursesScreen*) GetWindowLongPtr( wnd, GWLP_USERDATA ))
   switch (msg)
   {
   case WM_CREATE:
      SetWindowLongPtr( wnd, GWLP_USERDATA, 
                        (LONG_PTR) ((CREATESTRUCT*) lparam)->lpCreateParams );
      return 0;

   case WM_SIZE:
      {
         // Resize child controls.
         CursesScreen* inst = _INST_PTR( );
         const float x_scale = LOWORD( lparam ) / 640.0f;
         const float y_scale = HIWORD( lparam ) / 480.0f;

         #define _RESIZECTRL( wnd, asset ) SetWindowPos( wnd, NULL,                 \
                                                         (int) (asset.x * x_scale), \
                                                         (int) (asset.y * y_scale), \
                                                         (int) (asset.w * x_scale), \
                                                         (int) (asset.h * y_scale), \
                                                         SWP_NOZORDER )

         if (inst->m_controls.m_register)
            _RESIZECTRL( inst->m_controls.m_register, inst->m_assets->m_register );
         if (inst->m_controls.m_play)
            _RESIZECTRL( inst->m_controls.m_play, inst->m_assets->m_play );
         if (inst->m_controls.m_close)
            _RESIZECTRL( inst->m_controls.m_close, inst->m_assets->m_close );
         if (inst->m_controls.m_save)
            _RESIZECTRL( inst->m_controls.m_save, inst->m_assets->m_save );
         if (inst->m_controls.m_saveas)
            _RESIZECTRL( inst->m_controls.m_saveas, inst->m_assets->m_saveas );
         if (inst->m_controls.m_settings)
            _RESIZECTRL( inst->m_controls.m_settings, inst->m_assets->m_settings );
         if (inst->m_controls.m_characters)
            _RESIZECTRL( inst->m_controls.m_characters, inst->m_assets->m_characters );
         if (inst->m_controls.m_portrait)
            _RESIZECTRL( inst->m_controls.m_portrait, inst->m_assets->m_portrait );
         if (inst->m_controls.m_name)
            _RESIZECTRL( inst->m_controls.m_name, inst->m_assets->m_name );
         if (inst->m_controls.m_password)
            _RESIZECTRL( inst->m_controls.m_password, inst->m_assets->m_password );
         if (inst->m_controls.m_description)
            _RESIZECTRL( inst->m_controls.m_description, inst->m_assets->m_description );
         if (inst->m_controls.m_species)
            _RESIZECTRL( inst->m_controls.m_species, inst->m_assets->m_species );
         if (inst->m_controls.m_gender)
            _RESIZECTRL( inst->m_controls.m_gender, inst->m_assets->m_gender );
         if (inst->m_controls.m_code)
            _RESIZECTRL( inst->m_controls.m_code, inst->m_assets->m_code );
         if (inst->m_controls.m_colors)
            _RESIZECTRL( inst->m_controls.m_colors, inst->m_assets->m_colors );
         if (inst->m_controls.m_swatch)
            _RESIZECTRL( inst->m_controls.m_swatch, inst->m_assets->m_swatch );
         if (inst->m_controls.m_preview)
            _RESIZECTRL( inst->m_controls.m_preview, inst->m_assets->m_preview );
         if (inst->m_controls.m_digo)
            _RESIZECTRL( inst->m_controls.m_digo, inst->m_assets->m_digo );
         if (inst->m_controls.m_filename)
            _RESIZECTRL( inst->m_controls.m_filename, inst->m_assets->m_filename );
         if (inst->m_controls.m_status)
            _RESIZECTRL( inst->m_controls.m_status, inst->m_assets->m_status );
      }
      return 0;

   case WM_PAINT:
      {
         PAINTSTRUCT ps;
         HDC dc = BeginPaint( wnd, &ps );

         CursesScreen* inst = _INST_PTR( );

         blt_full( wnd, dc, inst->m_assets->m_background.dc );

         EndPaint( wnd, &ps );
         return 0;
      }

   case WM_CTLCOLORSTATIC:
      {
         switch (GetDlgCtrlID( (HWND) lparam ))
         {
         case CTRL_FILENAME:
            SetTextColor( (HDC) wparam, _INST_PTR( )->m_assets->m_filename.color );
            SetBkMode( (HDC) wparam, TRANSPARENT );
            return (LRESULT) GetStockBrush( NULL_BRUSH );
         case CTRL_STATUS:
            SetTextColor( (HDC) wparam, _INST_PTR( )->m_assets->m_status.color );
            SetBkMode( (HDC) wparam, TRANSPARENT );
            return (LRESULT) GetStockBrush( NULL_BRUSH );
         }
      }
      break;

   case WM_DRAWITEM:
      {
         CursesScreen* inst = _INST_PTR( );
         DRAWITEMSTRUCT* di = (DRAWITEMSTRUCT*) lparam;
         if (di->itemAction != ODA_DRAWENTIRE)
            break;

#define DRAWBUTTON( asset ) if (SendMessage( di->hwndItem, BM_GETSTATE, 0, 0 ) & BST_PUSHED)     \
                              blt_full( di->hwndItem, di->hDC, asset.dc_down );                  \
                            else                                                                 \
                              blt_full( di->hwndItem, di->hDC, asset.dc_up );                    
         switch (wparam)
         {
         case CTRL_REGISTER:
            DRAWBUTTON( inst->m_assets->m_register );
            return TRUE;

         case CTRL_PLAY:
            DRAWBUTTON( inst->m_assets->m_play );
            return TRUE;

         case CTRL_CLOSE:
            DRAWBUTTON( inst->m_assets->m_close );
            return TRUE;

         case CTRL_SAVE:
            DRAWBUTTON( inst->m_assets->m_save );
            return TRUE;

         case CTRL_SAVEAS:
            DRAWBUTTON( inst->m_assets->m_saveas );
            return TRUE;

         case CTRL_SETTINGS:
            DRAWBUTTON( inst->m_assets->m_settings );
            return TRUE;

         case CTRL_PREVIEW:
            {
               const FSHSHAPE* shape;

               switch (ListBox_GetItemData( inst->m_controls.m_digo, 
                                            ListBox_GetCurSel( inst->m_controls.m_digo ) ))
               {
               case DIGO_BATS:
                  shape = &inst->m_assets->m_preview.m_av_bats[ inst->m_colorcode.getGsd( ColorCodePal::GSD_SPECIES ) ];
                  break;
               case DIGO_BUTTERS:
                  shape = &inst->m_assets->m_preview.m_av_butters[ inst->m_colorcode.getGsd( ColorCodePal::GSD_SPECIES ) ];
                  break;
               case DIGO_CLASSICS:
                  shape = &inst->m_assets->m_preview.m_av_classics[ inst->m_colorcode.getGsd( ColorCodePal::GSD_SPECIES ) ];
                  break;
               case DIGO_DRAGON:
                  shape = &inst->m_assets->m_preview.m_av_dragon;
                  break;
               case DIGO_EAGLE:
                  shape = &inst->m_assets->m_preview.m_av_eagle;
                  break;
               case DIGO_GRYFFE:
                  shape = &inst->m_assets->m_preview.m_av_gryffe;
                  break;
               case DIGO_KIWI:
                  shape = &inst->m_assets->m_preview.m_av_kiwi;
                  break;
               case DIGO_PHOENIX:
                  shape = &inst->m_assets->m_preview.m_av_phoenix;
                  break;
               case DIGO_TRIS:
                  shape = &inst->m_assets->m_preview.m_av_tris[ inst->m_colorcode.getGsd( ColorCodePal::GSD_SPECIES ) ];
                  break;
               case DIGO_PRIMES:
                  shape = &inst->m_assets->m_preview.m_av_primes[ inst->m_colorcode.getGsd( ColorCodePal::GSD_SPECIES ) ];
                  break;
               case DIGO_WOLVEN:
                  shape = &inst->m_assets->m_preview.m_av_wolven;
                  break;
               case DIGO_KITTER:
                  shape = &inst->m_assets->m_preview.m_av_kitter;
                  break;
               case DIGO_PIXIE:
                  shape = &inst->m_assets->m_preview.m_av_pixie;
                  break;
               case DIGO_TYGARD:
                  shape = &inst->m_assets->m_preview.m_av_tygard;
                  break;
               case DIGO_LEONEN:
                  shape = &inst->m_assets->m_preview.m_av_leonen;
                  break;
               case DIGO_WEREWOLF:
                  shape = &inst->m_assets->m_preview.m_av_werewolf;
                  break;
               case DIGO_PENGUIN:
                  shape = &inst->m_assets->m_preview.m_av_penguin;
                  break;
               case DIGO_BAT:
                  shape = &inst->m_assets->m_preview.m_av_bat;
                  break;
               case DIGO_FOXEN:
                  shape = &inst->m_assets->m_preview.m_av_foxen;
                  break;
               case DIGO_FLOXEN:
                  shape = &inst->m_assets->m_preview.m_av_floxen;
                  break;
               case DIGO_UNICORN:
                  shape = &inst->m_assets->m_preview.m_av_unicorn;
                  break;
               case DIGO_FURLING:
                  switch (inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ))
                  {
                  default:
                  case ColorCodePal::GENDER_MALE:
                     shape = &inst->m_assets->m_preview.m_av_furlingm;
                     break;
                  case ColorCodePal::GENDER_FEMALE:
                     shape = &inst->m_assets->m_preview.m_av_furlingf;
                     break;
                  }
                  break;
               case DIGO_CHINCHILLA:
                  shape = &inst->m_assets->m_preview.m_av_chinchilla;
                  break;
               case DIGO_TOASTER:
                  shape = &inst->m_assets->m_preview.m_av_toaster;
                  break;
			   case DIGO_KIRIN:
				   shape = &inst->m_assets->m_preview.m_av_kirin;
                  break;	   
			  /* case DIGO_KIRINCLOUD:
				   shape = &inst->m_assets->m_preview.m_av_kirincloud;
                  break; */
			   case DIGO_NOBLECANINE:
			      switch (inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ))
                  {
                  default:
                  case ColorCodePal::GENDER_MALE:
                     shape = &inst->m_assets->m_preview.m_av_noblecaninem;
                     break;
                  case ColorCodePal::GENDER_FEMALE:
                     shape = &inst->m_assets->m_preview.m_av_noblecaninef;
                     break;
                  } 
                  break;
			   case DIGO_NOBLEFELINE:
				  switch (inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ))
                  {
                  default:
                  case ColorCodePal::GENDER_MALE:
                     shape = &inst->m_assets->m_preview.m_av_noblefelinem;
                     break;
                  case ColorCodePal::GENDER_FEMALE:
                     shape = &inst->m_assets->m_preview.m_av_noblefelinef;
                     break;
                  }
                  break;
			   case DIGO_REINDEER:
				   shape = &inst->m_assets->m_preview.m_av_reindeer;
                  break;
			   case DIGO_CATTEN:
				   shape = &inst->m_assets->m_preview.m_av_catten;
                  break;
			   case DIGO_OWLEN:
				   shape = &inst->m_assets->m_preview.m_av_owlen;
                  break;					   
			   case DIGO_LOVEBIRD:
				   shape = &inst->m_assets->m_preview.m_av_lovebird;
                  break;	   
               case DIGO_NONE:
               default:
                  shape = &inst->m_assets->m_preview.m_av_normal[ inst->m_colorcode.getGsd( ColorCodePal::GSD_SPECIES ) ];
               }

               if (shape && shape->pixels)
               {
                  inst->m_fshbmpinfo->remap( inst->m_colorcode );
                  inst->m_fshbmpinfo->setDims( shape->w, shape->h );

                  RECT client_rect;
                  GetClientRect( di->hwndItem, &client_rect );

                  FillRect( di->hDC, &client_rect, GetSysColorBrush( COLOR_WINDOW ) );


                  StretchDIBits( di->hDC, 
                                 client_rect.right / 2 - shape->w / 2, 
                                 client_rect.bottom / 2 - shape->h / 2, 
                                 shape->w, shape->h,
                                 0, 0, shape->w, shape->h, 
                                 shape->pixels,
                                 (BITMAPINFO*) inst->m_fshbmpinfo, DIB_RGB_COLORS, SRCCOPY );
               }
               return TRUE;
            }

         case CTRL_PORTRAIT:
            {
               const FSHSHAPE* shape;

               switch (ListBox_GetItemData( inst->m_controls.m_digo, 
                                            ListBox_GetCurSel( inst->m_controls.m_digo ) ))
               {
               case DIGO_DRAGON:
                  shape = &inst->m_assets->m_portrait.m_dragon[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_EAGLE:
                  shape = &inst->m_assets->m_portrait.m_eagle[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_GRYFFE:
                  shape = &inst->m_assets->m_portrait.m_gryffe[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_PHOENIX:
                  shape = &inst->m_assets->m_portrait.m_phoenix[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_PIXIE:
               case DIGO_KITTER:
                  shape = &inst->m_assets->m_portrait.m_kitter[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_WOLVEN:
                  shape = &inst->m_assets->m_portrait.m_wolven[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_KIWI:
                  shape = &inst->m_assets->m_portrait.m_kiwi[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_TYGARD:
                  shape = &inst->m_assets->m_portrait.m_tygard[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_LEONEN:
                  shape = &inst->m_assets->m_portrait.m_leonen[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_WEREWOLF:
                  shape = &inst->m_assets->m_portrait.m_werewolf[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_PENGUIN:
                  shape = &inst->m_assets->m_portrait.m_penguin[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_BAT:
                  shape = &inst->m_assets->m_portrait.m_bat[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_FOXEN:
                  shape = &inst->m_assets->m_portrait.m_foxen[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_FLOXEN:
                  shape = &inst->m_assets->m_portrait.m_floxen[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_UNICORN:
                  shape = &inst->m_assets->m_portrait.m_unicorn[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_FURLING:
                  shape = &inst->m_assets->m_portrait.m_furling[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_CHINCHILLA:
                  shape = &inst->m_assets->m_portrait.m_chinchilla[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_TOASTER:
                  shape = &inst->m_assets->m_portrait.m_toaster[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
			   case DIGO_KIRIN: 
				   shape = &inst->m_assets->m_portrait.m_kirin[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;	   
			   case DIGO_NOBLECANINE:
			      shape = &inst->m_assets->m_portrait.m_noblecanine[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
			   case DIGO_NOBLEFELINE:
				  shape = &inst->m_assets->m_portrait.m_noblefeline[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
			   case DIGO_REINDEER:
				   shape = &inst->m_assets->m_portrait.m_reindeer[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
			   case DIGO_CATTEN:
				   shape = &inst->m_assets->m_portrait.m_catten[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
			   case DIGO_OWLEN:
				   shape = &inst->m_assets->m_portrait.m_owlen[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;					   
			   case DIGO_LOVEBIRD:
				   shape = &inst->m_assets->m_portrait.m_lovebird[ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
                  break;
               case DIGO_PRIMES:
               case DIGO_BATS:
               case DIGO_BUTTERS:
               case DIGO_CLASSICS:
               case DIGO_TRIS:
               case DIGO_NONE:
               default:
                  shape = &inst->m_assets->m_portrait.m_normal
                     [ inst->m_colorcode.getGsd( ColorCodePal::GSD_SPECIES ) ][ inst->m_colorcode.getGsd( ColorCodePal::GSD_GENDER ) ];
               }

               if (shape && shape->pixels)
               {
                  inst->m_fshbmpinfo->remap( inst->m_colorcode );
                  inst->m_fshbmpinfo->setDims( shape->w, shape->h );

                  RECT client_rect;
                  GetClientRect( di->hwndItem, &client_rect );
                  StretchDIBits( di->hDC, 
                                 0, 0, client_rect.right, client_rect.bottom,
                                 0, 0, shape->w, shape->h, 
                                 shape->pixels,
                                 (BITMAPINFO*) inst->m_fshbmpinfo, DIB_RGB_COLORS, SRCCOPY );
               }
               return TRUE;
            }
         }
      }
      break;

   case WM_COMMAND:
      {
         switch (LOWORD( wparam ))
         {
         case CTRL_COLORS:
            if (HIWORD( wparam ) == CBN_SELCHANGE)
            {
               CursesScreen* inst = _INST_PTR( );
               const unsigned i = 
                  SendMessage( inst->m_controls.m_colors, CB_GETITEMDATA, 
                               SendMessage( inst->m_controls.m_colors, CB_GETCURSEL, 0, 0 ), 0 );
               assert( i < 10 );
               Swatch_Fill( inst->m_controls.m_swatch, 
                            inst->m_assets->m_swatch.m_counts[ i ],
                            inst->m_assets->m_swatch.m_swatches[ i ] );
               inst->syncColorCode( );
               return 0;
            }
            break;
         case CTRL_SPECIES:
            if (HIWORD( wparam ) == CBN_SELCHANGE)
            {
               CursesScreen* inst = _INST_PTR( );
               const unsigned i = 
                  SendMessage( inst->m_controls.m_species, CB_GETITEMDATA, 
                               SendMessage( inst->m_controls.m_species, CB_GETCURSEL, 0, 0 ), 0 );
               inst->m_colorcode.setGsd( ColorCodePal::GSD_SPECIES, i );
               inst->syncColorCode( );
               inst->markUnsaved( true );
               return 0;
            }
            break;
         case CTRL_GENDER:
            if (HIWORD( wparam ) == CBN_SELCHANGE)
            {
               CursesScreen* inst = _INST_PTR( );
               const unsigned i = 
                  SendMessage( inst->m_controls.m_gender, CB_GETITEMDATA, 
                               SendMessage( inst->m_controls.m_gender, CB_GETCURSEL, 0, 0 ), 0 );
               inst->m_colorcode.setGsd( ColorCodePal::GSD_GENDER, i );
               inst->syncColorCode( );
               inst->markUnsaved( true );
               return 0;
            }
            break;

         case CTRL_CODE:
            if (HIWORD( wparam ) == EN_CHANGE)
            {
               CursesScreen* inst = _INST_PTR( );
               char sz_code[ 15 ];
               GetWindowText( inst->m_controls.m_code, sz_code, 15 );
               if (!*sz_code || inst->m_colorcode != ColorCodePal( sz_code + 1 ))
               {
                  inst->m_colorcode = sz_code + 1;
                  inst->syncColorCode( );
                  inst->markUnsaved( true );
               }
            }
            break;

         case CTRL_NAME:
         case CTRL_PASSWORD:
         case CTRL_DESCRIPTION:
            if (HIWORD( wparam ) == EN_CHANGE)
            {
               _INST_PTR( )->markUnsaved( true );
            }
            break;

         case CTRL_DIGO:
            if (HIWORD( wparam ) == LBN_SELCHANGE)
            {
               _INST_PTR( )->syncColorCode( );
            }
            break;

         case CTRL_CHARACTERS:
            if (HIWORD( wparam ) == LBN_SELCHANGE)
            {
               CursesScreen* inst = _INST_PTR( );

               if (inst->promptSave( ))
               {
                  const int sel = ListBox_GetCurSel( inst->m_controls.m_characters );
                  if (sel != -1)
                  {
                     assert( ListBox_GetItemData( inst->m_controls.m_characters, sel ) );
                     const string* filename = (const string*) 
                        ListBox_GetItemData( inst->m_controls.m_characters, sel );

                     if (_stricmp( filename->c_str( ), inst->m_curr_ini.c_str( ) ))
                        inst->selectIniFile( *filename );
                  }
               }
               else // Prompt cancelled.
               {
                  // Restore selection by re-filling the form.
                  LOGININFO li;
                  inst->getLoginInfo( &li );
                  inst->fillForm( li );
               }
            }
            else if (HIWORD( wparam ) == LBN_RCLICKITEM)
            {
               CursesScreen* inst = _INST_PTR( );

               // On an item, so enable some menu options.
               HMENU popup = GetSubMenu( inst->m_menus, 0 );
               EnableMenuItem( popup, ID_CHARS_MAKEDEFAULT, MF_ENABLED );
               EnableMenuItem( popup, ID_CHARS_DELETE, MF_ENABLED );

               POINT pt;
               GetCursorPos( &pt );
               TrackPopupMenu( popup,
                               TPM_RIGHTBUTTON, pt.x, pt.y, 0,
                               wnd, NULL );
            }
            else if (HIWORD( wparam ) == LBN_RCLICK)
            {
               CursesScreen* inst = _INST_PTR( );

               // Not on an item, so disable some menu options.
               HMENU popup = GetSubMenu( inst->m_menus, 0 );
               EnableMenuItem( popup, ID_CHARS_MAKEDEFAULT, MF_DISABLED | MF_GRAYED );
               EnableMenuItem( popup, ID_CHARS_DELETE, MF_DISABLED | MF_GRAYED );

               POINT pt;
               GetCursorPos( &pt );
               TrackPopupMenu( popup,
                               TPM_RIGHTBUTTON, pt.x, pt.y, 0,
                               wnd, NULL );

            }
            else if (HIWORD( wparam ) == LBN_DBLCLK)
            {
               Curses::singleton( )->screenPlay( );
            }
            break;

         case CTRL_SAVE:
            if (HIWORD( wparam ) == BN_CLICKED)
            {
               _INST_PTR( )->save( );
            }
            break;

         case CTRL_SAVEAS:
            if (HIWORD( wparam ) == BN_CLICKED)
            {
               _INST_PTR( )->saveAs( );
            }
            break;

         case CTRL_SETTINGS:
            if (HIWORD( wparam ) == BN_CLICKED)
            {
               Curses::singleton( )->showOptions( );
            }
            break;            

         case CTRL_CLOSE:
            if (HIWORD( wparam ) == BN_CLICKED)
            {
               Curses::singleton( )->screenClose( );
            }
            break;

         case CTRL_PLAY:
            if (HIWORD( wparam ) == BN_CLICKED)
            {
               Curses::singleton( )->screenPlay( );
            }
            break;

         case CTRL_REGISTER:
            if (HIWORD( wparam ) == BN_CLICKED)
            {
               Curses::singleton( )->screenRegister( );
            }
            break;

         case ID_CHARS_MAKEDEFAULT:
            if (!HIWORD( wparam ) && !lparam)
            {
               CursesScreen* inst = _INST_PTR( );

               if (!inst->m_curr_ini.empty( ))
               {
                  Curses::singleton( )->setDefaultIni( inst->m_curr_ini );
                  inst->refreshCharacters( );
               }
            }
            break;
         case ID_CHARS_NEW:
            if (!HIWORD( wparam ) && !lparam)
            {
               _INST_PTR( )->newChar( );
            }
            break;
         case ID_CHARS_DELETE:
            if (!HIWORD( wparam ) && !lparam)
            {
               _INST_PTR( )->deleteChar( );
            }
            break;
         case ID_CHARS_REFRESH:
            if (!HIWORD( wparam ) && !lparam)
            {
               _INST_PTR( )->refreshCharacters( );
            }
            break;

         }
      }
      break;

   case WM_NOTIFY:
      {
         switch (((LPNMHDR) lparam)->idFrom)
         {
         case CTRL_SWATCH:
            if (((LPNMHDR) lparam)->code == SWN_SELCHANGE)
            {
               CursesScreen* inst = _INST_PTR( );

               assert( SendMessage( inst->m_controls.m_colors, CB_GETCURSEL, 0, 0 ) != -1 );

               unsigned swatch = 
                  SendMessage( inst->m_controls.m_colors, CB_GETITEMDATA, 
                               SendMessage( inst->m_controls.m_colors, CB_GETCURSEL, 0, 0 ), 0 );
               assert( swatch < 10 );

               const unsigned sel = ((NMSWSELCHANGE*) lparam)->sel;
               assert( sel != -1 );

               inst->m_colorcode.setColor( swatch, sel );
               inst->syncColorCode( );
               inst->markUnsaved( true );
               return 0;
            }
         }
      }
      break;
   }

   return DefWindowProc( wnd, msg, wparam, lparam );
#undef _INST_PTR
}

//////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK lb_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
   const WNDPROC old_proc = (WNDPROC) GetWindowLongPtr( wnd, GWLP_USERDATA );

   switch (msg)
   {
   case WM_RBUTTONDOWN:
      {
         const LRESULT hit = CallWindowProc( old_proc, wnd, LB_ITEMFROMPOINT, 0, lparam );
         if (!HIWORD( hit ))  // Inside the client area.
         {
            if (LOWORD( hit ) != CallWindowProc( old_proc, wnd, LB_GETCURSEL, 0, 0 ))
            {
               CallWindowProc( old_proc, wnd, LB_SETCURSEL, LOWORD( hit ), 0 );
               SendMessage( GetParent( wnd ), WM_COMMAND, 
                            MAKELONG( (LONG) GetMenu( wnd ), LBN_SELCHANGE ), (LPARAM) wnd );
            }
            else
               CallWindowProc( old_proc, wnd, LB_SETCURSEL, LOWORD( hit ), 0 );               
         }
         return 0;
      }

   case WM_RBUTTONUP:
      {
         const LRESULT hit = CallWindowProc( old_proc, wnd, LB_ITEMFROMPOINT, 0, lparam );
         if (!HIWORD( hit ))  // Inside an item.
         {
            SendMessage( GetParent( wnd ), WM_COMMAND, 
                         MAKELONG( (LONG) GetMenu( wnd ), LBN_RCLICKITEM ), (LPARAM) wnd );
         }
         else
            SendMessage( GetParent( wnd ), WM_COMMAND, 
                         MAKELONG( (LONG) GetMenu( wnd ), LBN_RCLICK ), (LPARAM) wnd );
         return 0;
      }
   }

   return CallWindowProc( old_proc,
                          wnd, msg, wparam, lparam );
}

LRESULT CALLBACK edit_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
   switch (msg)
   {
   case WM_KEYDOWN:
      if (GetKeyState( VK_CONTROL ) & 0x80000000)
      {
         switch (wparam)
         {
         case 0x56: // V
            PostMessage( wnd, WM_PASTE, 0, 0 );
            break;
         case 0x58: // X
            PostMessage( wnd, WM_CUT, 0, 0 );
            break;
         case 0x5A: // Z
            PostMessage( wnd, WM_UNDO, 0, 0 );
            break;
         case 0x43: // C
            PostMessage( wnd, WM_COPY, 0, 0 );
            break;
         case 0x41: // A
            PostMessage( wnd, EM_SETSEL, 0, -1 );
            break;
         }
      }
      break;
   }
   return CallWindowProc( (WNDPROC) GetWindowLongPtr( wnd, GWLP_USERDATA ),
                          wnd, msg, wparam, lparam );
}

//////////////////////////////////////////////////////////////////////////

void blt_full( HWND wnd, HDC dst, HDC src )
{
   if (dst && src)
   {
      HBITMAP src_bmp = (HBITMAP) GetCurrentObject( src, OBJ_BITMAP );
      BITMAP src_bm;
      GetObject( src_bmp, sizeof( BITMAP ), &src_bm );

      RECT dst_rect;
      GetClientRect( wnd, &dst_rect );

      StretchBlt( dst, 0, 0, dst_rect.right, dst_rect.bottom,
                  src, 0, 0, src_bm.bmWidth, src_bm.bmHeight, SRCCOPY );
   }
}

unsigned ComboBox_FindFirst( HWND wnd, LPARAM data )
{
   const unsigned count = ComboBox_GetCount( wnd );
   for (unsigned i = 0; i < count; ++i)
   {
      if (data == (LPARAM) ComboBox_GetItemData( wnd, i ))
         return i;
   }

   return -1;
}

unsigned ListBox_FindFirst( HWND wnd, LPARAM data )
{
   const unsigned count = ListBox_GetCount( wnd );
   for (unsigned i = 0; i < count; ++i)
   {
      if (data == (LPARAM) ListBox_GetItemData( wnd, i ))
         return i;
   }

   return -1;
}

void InvalidateParent( HWND child, BOOL erase )
{
   const HWND parent = GetParent( child );
   assert( parent );

   RECT r;
   GetWindowRect( child, &r );
   POINT origin = { r.left, r.top };
   ScreenToClient( parent, &origin );
   r.right  = origin.x + (r.right - r.left);
   r.bottom = origin.y + (r.bottom - r.top);
   r.left   = origin.x;
   r.top    = origin.y;

   InvalidateRect( parent, &r, erase );
}

bool operator !=( const LOGININFO& li, const CharacterIni& ini )
{
   return (li.name != ini.getName( ) || li.password != ini.getPassword( ) ||
           li.description != ini.getDescription( ) ||
           li.color != ColorCodePal( ini.getColorCode( ).substr( 1 ) ));
}

string makefilename( const string& str )
{
   string result( str );
   for (string::iterator i = result.begin( ); i != result.end( ); ++i)
   {
      switch (*i)
      {
      case '|':
         *i = ' ';
         break;
      case '\\':
      case '/':
      case '<':
      case '>':
      case ' ':
      case '?':
      case '*':
      case ':':
         *i = '_';
      }
   }
   return result;
}

//////////////////////////////////////////////////////////////////////////

} // namespace curses
