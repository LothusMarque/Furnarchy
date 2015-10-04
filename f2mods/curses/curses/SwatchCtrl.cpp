#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _WIN32_WINNT 0x0500
#include "SwatchCtrl.h"
#include <WindowsX.h>

#include <xutility>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;

class SwatchCtrl
{
   enum { MARGIN_SIZE = 2 };
public:
   static void class_init( HINSTANCE inst );
   static void class_uninit( HINSTANCE inst );

   SwatchCtrl( HWND wnd );
   ~SwatchCtrl( );

   void paint( HDC dc ) const;
   SIZE calcDims( ) const;
   SIZE calcSize( ) const;
   void setSwatchSize( unsigned int size );
   void setBgColor( COLORREF color );
   void fill( unsigned int num, const COLORREF* colors );
   unsigned int getSel( ) const;
   void setSel( unsigned int sel );
   unsigned int getCount( ) const;
   COLORREF getColor( unsigned int index ) const;
   void updateScrollInfo( );
   void resize( );
   unsigned int itemFromPt( int x, int y ) const;

protected:
   static LRESULT CALLBACK wnd_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );

private:
   HWND             m_wnd;
   vector<COLORREF> m_colors;
   COLORREF         m_bgcolor;
   unsigned int     m_sel;
   unsigned int     m_swatch_size;
   HDC              m_backdc;
};

void SwatchCtrl::class_init( HINSTANCE inst )
{
   WNDCLASSEX wc;
   memset( &wc, 0, sizeof( wc ) );
   wc.cbSize = sizeof( wc );
   wc.hCursor = LoadCursor( NULL, IDC_ARROW );
   wc.lpfnWndProc = wnd_proc;
   wc.lpszClassName = WC_SWATCH;
   wc.hInstance = inst;
   wc.hbrBackground = GetStockBrush( NULL_BRUSH );
   wc.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;
   wc.cbWndExtra = sizeof( SwatchCtrl* );

   RegisterClassEx( &wc );
}

void SwatchCtrl::class_uninit( HINSTANCE inst )
{
   UnregisterClass( WC_SWATCH, inst );
}

SwatchCtrl::SwatchCtrl( HWND wnd )
   : m_wnd( wnd ), m_sel( -1 ), m_swatch_size( 10 ), 
     m_bgcolor( GetSysColor( COLOR_WINDOW ) )
{
   {
      HDC parent_dc = GetDC( GetParent( wnd ) );
      m_backdc = CreateCompatibleDC( parent_dc );
      ReleaseDC( GetParent( wnd ), parent_dc );
   }

   SaveDC( m_backdc );
   resize( );
}

SwatchCtrl::~SwatchCtrl( )
{
   HBITMAP bm = (HBITMAP) GetCurrentObject( m_backdc, OBJ_BITMAP );
   RestoreDC( m_backdc, -1 );
   if (bm != (HBITMAP) GetCurrentObject( m_backdc, OBJ_BITMAP ))
      DeleteBitmap( bm );

   DeleteDC( m_backdc );
}

void SwatchCtrl::resize( )
{
   updateScrollInfo( );

   RECT client_rect;
   GetClientRect( m_wnd, &client_rect );

   HBITMAP bm = (HBITMAP) GetCurrentObject( m_backdc, OBJ_BITMAP );
   BITMAP bm_info;
   GetObject( bm, sizeof( BITMAP ), &bm_info );
   if (bm_info.bmWidth != client_rect.right || bm_info.bmHeight != client_rect.bottom)
   {
      RestoreDC( m_backdc, -1 );
      if (bm != (HBITMAP) GetCurrentObject( m_backdc, OBJ_BITMAP ))
         DeleteBitmap( bm );
      SaveDC( m_backdc );
      HDC parent_dc = GetDC( GetParent( m_wnd ) );
      bm = CreateCompatibleBitmap( parent_dc, client_rect.right, client_rect.bottom );
      ReleaseDC( GetParent( m_wnd ), parent_dc );
      if (bm)
         SelectObject( m_backdc, bm );
   }
}

SIZE SwatchCtrl::calcDims( ) const
{
   RECT r;
   GetClientRect( m_wnd, &r );

   const unsigned swatch_size = m_swatch_size + (m_swatch_size / 10 + 1) * 2;
   const unsigned max_cols = (unsigned) r.right <= swatch_size + (MARGIN_SIZE * 2) ? 
                              1 : (r.right - (MARGIN_SIZE * 2)) / swatch_size;

   SIZE dims;
   dims.cx = m_colors.size( ) >= max_cols ? max_cols : m_colors.size( );
   dims.cy = m_colors.size( ) ? 1 + (m_colors.size( ) - 1) / max_cols : 0;

   if (!dims.cx || !dims.cy)
      dims.cx = 0, dims.cy = 0;

   return dims;
}

SIZE SwatchCtrl::calcSize( ) const
{
   SIZE dims = calcDims( );

   const unsigned swatch_size = m_swatch_size + (m_swatch_size / 10 + 1) * 2;
   dims.cx = dims.cx * swatch_size + MARGIN_SIZE * 2;
   dims.cy = dims.cy * swatch_size + MARGIN_SIZE * 2;
   
   return dims;
}

inline void SwatchCtrl::setSwatchSize( unsigned int size )
{
   m_swatch_size = size;
   updateScrollInfo( );
}

inline void SwatchCtrl::setBgColor( COLORREF color ) {
   m_bgcolor = color;
}

inline void SwatchCtrl::fill( unsigned int num, const COLORREF* colors )
{
   m_colors.resize( num );
   copy( colors, colors + num, m_colors.begin( ) );
   
   if (m_sel >= num)
      m_sel = -1;

   updateScrollInfo( );
}

inline unsigned int SwatchCtrl::getSel( ) const {
   return m_sel;
}

inline void SwatchCtrl::setSel( unsigned int sel ) 
{
   if (sel >= m_colors.size( ))
      sel = -1;

   NMSWSELCHANGE nm;
   nm.hdr.code     = SWN_SELCHANGE;
   nm.hdr.hwndFrom = m_wnd;
   nm.hdr.idFrom   = (UINT_PTR) GetMenu( m_wnd );
   nm.sel   = sel;
   nm.color = getColor( sel );

   if (SendMessage( GetParent( m_wnd ), WM_NOTIFY, nm.hdr.idFrom, (LPARAM) &nm ))
      return;

   m_sel = sel;
}

inline unsigned int SwatchCtrl::getCount( ) const {
   return (unsigned int) m_colors.size( );
}

inline COLORREF SwatchCtrl::getColor( unsigned int index ) const {
   if (index < (unsigned int) m_colors.size( ))
      return m_colors[ index ];
   return (COLORREF) -1;
}

void SwatchCtrl::updateScrollInfo( )
{
   RECT client_rect;
   GetClientRect( m_wnd, &client_rect );

   SIZE content_size = calcSize( );

   // Set scrollbars.
   if (content_size.cx > client_rect.right)
      ShowScrollBar( m_wnd, SB_HORZ, TRUE );
   else
      ShowScrollBar( m_wnd, SB_HORZ, FALSE );

   if (content_size.cy > client_rect.bottom)
      ShowScrollBar( m_wnd, SB_VERT, TRUE );
   else
      ShowScrollBar( m_wnd, SB_VERT, FALSE );

   // Get new client and content sizes.
   GetClientRect( m_wnd, &client_rect );
   content_size = calcSize( );

   {
      SCROLLINFO si;
      memset( &si, 0, sizeof( si ) );
      si.cbSize = sizeof( si );
      si.fMask  = SIF_PAGE | SIF_RANGE;
      si.nPage  = client_rect.right;
      si.nMin   = 0;
      si.nMax   = content_size.cx;
      SetScrollInfo( m_wnd, SB_HORZ, &si, TRUE );

      si.nPage  = client_rect.bottom;
      si.nMin   = 0;
      si.nMax   = content_size.cy;
      SetScrollInfo( m_wnd, SB_VERT, &si, TRUE );
   }
}

void SwatchCtrl::paint( HDC dc ) const
{
   int x_start, y_start;
   {
      SIZE scroll;
      SCROLLINFO si;
      memset( &si, 0, sizeof( si ) );
      si.cbSize = sizeof( si );
      si.fMask  = SIF_POS;
      GetScrollInfo( m_wnd, SB_HORZ, &si );
      scroll.cx = si.nPos;
      GetScrollInfo( m_wnd, SB_VERT, &si );
      scroll.cy = si.nPos;

      SIZE content_size = calcSize( );
      RECT client_rect;
      GetClientRect( m_wnd, &client_rect );

      if (client_rect.right > content_size.cx)
         x_start = (client_rect.right - content_size.cx) / 2 - scroll.cx;
      else
         x_start = -scroll.cx;

      if (client_rect.bottom > content_size.cy)
         y_start = (client_rect.bottom - content_size.cy) / 2 - scroll.cy;
      else
         y_start = -scroll.cy;
   }

   {
      const SIZE dims = calcDims( );
      const unsigned swatch_size = m_swatch_size + (m_swatch_size / 10 + 1) * 2;
      const unsigned border_size = m_swatch_size / 10 + 1;
      HBRUSH sel_brush = CreateSolidBrush( RGB( (~GetRValue( m_bgcolor )) & 0xFF, 
                                                (~GetGValue( m_bgcolor )) & 0xFF,
                                                (~GetBValue( m_bgcolor )) & 0xFF ) );

      unsigned int i = 0;
      int y = y_start + MARGIN_SIZE;
      for (unsigned row = 0; row < (unsigned) dims.cy; ++row, y += swatch_size)
      {
         int x = x_start + MARGIN_SIZE;
         for (unsigned col = 0; 
              col < (unsigned) dims.cx && i < (unsigned int) m_colors.size( ); 
              ++col, ++i, x += swatch_size)
         {
            if (m_sel == i)
            {
               // Selected.
               RECT r = { x, y, x + swatch_size, y + swatch_size };
               FillRect( dc, &r, sel_brush );
            }

            HBRUSH swatch_brush = CreateSolidBrush( m_colors[ i ] );
            RECT r = { x + border_size, y + border_size, 
                       x + border_size + m_swatch_size,
                       y + border_size + m_swatch_size };
            FillRect( dc, &r, swatch_brush );
            DeleteBrush( swatch_brush );
         }
      }

      DeleteBrush( sel_brush );
   }
}

unsigned int SwatchCtrl::itemFromPt( int x, int y ) const
{
   int x_start, y_start;
   SIZE content_size = calcSize( );
   {
      SIZE scroll;
      SCROLLINFO si;
      memset( &si, 0, sizeof( si ) );
      si.cbSize = sizeof( si );
      si.fMask  = SIF_POS;
      GetScrollInfo( m_wnd, SB_HORZ, &si );
      scroll.cx = si.nPos;
      GetScrollInfo( m_wnd, SB_VERT, &si );
      scroll.cy = si.nPos;

      RECT client_rect;
      GetClientRect( m_wnd, &client_rect );

      if (client_rect.right > content_size.cx)
         x_start = (client_rect.right - content_size.cx) / 2 - scroll.cx;
      else
         x_start = -scroll.cx;

      if (client_rect.bottom > content_size.cy)
         y_start = (client_rect.bottom - content_size.cy) / 2 - scroll.cy;
      else
         y_start = -scroll.cy;
   }

   x_start += MARGIN_SIZE;
   y_start += MARGIN_SIZE;
   content_size.cx -= MARGIN_SIZE * 2;
   content_size.cy -= MARGIN_SIZE * 2;

   const unsigned swatch_size = m_swatch_size + (m_swatch_size / 10 + 1) * 2;

   if ((x >= x_start && x < x_start + content_size.cx) &&
       (y >= y_start && y < y_start + content_size.cy))
   {
      const SIZE dims = calcDims( );
      unsigned int index = (y - y_start) / swatch_size * dims.cx + 
                           (x - x_start) / swatch_size;
      if (index < m_colors.size( ))
         return index;
   }

   return -1;
}

LRESULT CALLBACK SwatchCtrl::wnd_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
#define _INST( ) ((SwatchCtrl*) GetWindowLongPtr( wnd, 0 ))
   switch (msg)
   {
   case WM_CREATE:
      /* Create an instance. */
      SetWindowLongPtr( wnd, 0, (LONG_PTR) new SwatchCtrl( wnd ) );
      return 0;

   case WM_DESTROY:
      /* Destroy the instance. */
      delete _INST( );
      SetWindowLongPtr( wnd, 0, (LONG_PTR) NULL );
      return 0;

   case WM_SIZE:
      {
         SwatchCtrl* inst = _INST( );
         if (inst)
         {
            inst->resize( );
         }
      }
      return 0;

   case WM_VSCROLL:
   case WM_HSCROLL:
      {
         SwatchCtrl* inst = _INST( );
         assert( inst );

         SCROLLINFO si;
         si.cbSize = sizeof( si );
         si.fMask = SIF_ALL;
         GetScrollInfo( wnd, msg == WM_VSCROLL ? SB_VERT : SB_HORZ, &si );

         switch (LOWORD( wparam ))
         {
         case SB_TOP:
            //case SB_LEFT
            si.nPos = si.nMin;
            break;

         case SB_BOTTOM:
            //case SB_RIGHT:
            si.nPos = si.nMax;
            break;

         case SB_LINEUP:
            //case SB_LINELEFT:
            si.nPos -= inst->m_swatch_size + (inst->m_swatch_size / 10 + 1) * 2;
            break;

         case SB_LINEDOWN:
            //case SB_LINERIGHT:
            si.nPos += inst->m_swatch_size + (inst->m_swatch_size / 10 + 1) * 2;
            break;

         case SB_PAGEUP:
            //case SB_PAGELEFT:
            si.nPos -= si.nPage;
            break;

         case SB_PAGEDOWN:
            //case SB_PAGERIGHT:
            si.nPos += si.nPage;
            break;

         case SB_THUMBTRACK:
            si.nPos = si.nTrackPos;
            break;
         }

         SetScrollInfo( wnd, msg == WM_VSCROLL ? SB_VERT : SB_HORZ, &si, TRUE );
         InvalidateRect( wnd, NULL, TRUE );
      }
      return 0;

   case WM_MOUSEWHEEL:
      {
         int rot = (short) HIWORD( wparam );
         if (rot > 0)
         {
            for (int i = rot / WHEEL_DELTA; i; --i)
               PostMessage( wnd, WM_VSCROLL, MAKELONG( SB_LINEUP, 0 ), NULL );
         }
         else
         {
            for (int i = rot / WHEEL_DELTA; i; ++i)
               PostMessage( wnd, WM_VSCROLL, MAKELONG( SB_LINEDOWN, 0 ), NULL );
         }
      }
      return 0;

      /*
   case WM_NCLBUTTONDOWN:
   case WM_NCRBUTTONDOWN:
      if (GetFocus( ) != wnd)
         SetFocus( wnd );
      break; */

   case WM_LBUTTONDOWN:
   case WM_RBUTTONDOWN:
      {
         SwatchCtrl* inst = _INST( );
         assert( inst );

         if (GetFocus( ) != wnd)
            SetFocus( wnd );

         unsigned int item = inst->itemFromPt( GET_X_LPARAM( lparam ), 
                                               GET_Y_LPARAM( lparam ) );
         if (item != -1)
            inst->setSel( item );
         InvalidateRect( wnd, NULL, TRUE );
      }
      return 0;

   case WM_PAINT:
      {
         SwatchCtrl* inst = _INST( );
         assert( inst );

         PAINTSTRUCT ps;
         HDC dc = BeginPaint( wnd, &ps );

         if (ps.fErase)
         {
            HBRUSH bgbrush = CreateSolidBrush( inst->m_bgcolor );
            FillRect( inst->m_backdc, &ps.rcPaint, bgbrush );
            DeleteBrush( bgbrush );
         }

         inst->paint( inst->m_backdc );

         BitBlt( dc, ps.rcPaint.left, ps.rcPaint.top, 
                 ps.rcPaint.right - ps.rcPaint.left, 
                 ps.rcPaint.bottom - ps.rcPaint.top,
                 inst->m_backdc, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY );
         
         EndPaint( wnd, &ps );
      }
      return 0;

   case WM_ERASEBKGND:
      // Let it get erased in paint.
      return 0;

   case SWM_FILL:
      {
         SwatchCtrl* inst = _INST( );
         assert( inst );
         inst->fill( (unsigned int) wparam, (const COLORREF*) lparam );
         InvalidateRect( wnd, NULL, TRUE );
      }
      return 0;

   case SWM_SETSWATCHSIZE:
      {
         SwatchCtrl* inst = _INST( );
         assert( inst );
         inst->setSwatchSize( wparam );
         InvalidateRect( wnd, NULL, TRUE );
      }
      return 0;

   case SWM_GETCOUNT:
      {
         SwatchCtrl* inst = _INST( );
         assert( inst );
         return inst->getCount( );
      }

   case SWM_GETSEL:
      {
         SwatchCtrl* inst = _INST( );
         assert( inst );
         return inst->getSel( );
      }

   case SWM_SETSEL:
      {
         SwatchCtrl* inst = _INST( );
         assert( inst );
         inst->m_sel = wparam;
         if (inst->m_sel >= inst->m_colors.size( ))
            inst->m_sel = -1;
         InvalidateRect( wnd, NULL, TRUE );
         return 0;
      }

   case SWM_GETCOLOR:
      {
         SwatchCtrl* inst = _INST( );
         assert( inst );
         return inst->getColor( wparam );
      }

   case SWM_SETBKGNDCOLOR:
      {
         SwatchCtrl* inst = _INST( );
         assert( inst );
         inst->setBgColor( wparam );
         return 0;
      }
   }

   return DefWindowProc( wnd, msg, wparam, lparam );
#undef _INST
}

//////////////////////////////////////////////////////////////////////////

void Swatch_Init( HINSTANCE inst )
{
   SwatchCtrl::class_init( inst );
}

void Swatch_Uninit( HINSTANCE inst )
{
   SwatchCtrl::class_uninit( inst );
}