#include "window.h"
#include "core.h"
#include "net.h"
#include "modules.h"
#include <CommCtrl.h>
#include <WinSock.h>
#include <shellapi.h>
#include <cassert>
#include <cmath>

namespace window {

bool      g_initialized       = false;
bool      g_traymode          = false;
HMENU     g_traymenu          = NULL;
UINT      g_taskbarcreate_msg = 0;
HWND      g_wnd               = NULL;
HDC       g_backdc            = NULL;
HBITMAP   g_backoldbmp        = NULL;
HWND      g_current_dlg       = NULL;
TIMERPROC g_timerproc104      = NULL;
LRESULT(CALLBACK *g_old_proc)( HWND, UINT, WPARAM, LPARAM ) = 0;

void tray_remove( );
bool tray_create( );
void tray_hide( );
LRESULT window_proc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam );

bool initialize( )
{
   if (!g_initialized)
   {
      g_initialized = true;
      g_traymode    = false;
      g_wnd         = NULL;
      g_backdc      = NULL;
      g_backoldbmp  = NULL;
      g_current_dlg = NULL;

      /* Create a menu for the tray icon. */
      g_traymenu = CreatePopupMenu( );
      AppendMenu( g_traymenu, MF_STRING, TRAYCMD_RESTORE, "&Restore" );
      AppendMenu( g_traymenu, MF_STRING, TRAYCMD_EXIT, "&Exit" );

      g_taskbarcreate_msg = RegisterWindowMessage( "TaskbarCreated" );
   }

   return true;
}

bool uninitialize( )
{
   if (g_initialized)
   {
      g_initialized = false;

      tray_remove( );
      g_traymenu = NULL;

      DestroyMenu( g_traymenu );

      if (g_wnd && g_old_proc)
      {
         /* Disable double clicks. */
         SetClassLong( g_wnd, GCL_STYLE, GetClassLong( g_wnd, GCL_STYLE ) & (~CS_DBLCLKS) );
         /* Unclip children. */
         SetWindowLong( g_wnd, GWL_STYLE, GetWindowLong( g_wnd, GWL_STYLE ) & (~WS_CLIPCHILDREN) );
         /* Reset windowproc. */
         SetWindowLongPtr( g_wnd, GWL_WNDPROC, (LONG_PTR) g_old_proc );
         g_old_proc = 0;

         DeleteObject( SelectObject( g_backdc, g_backoldbmp ) );
         DeleteDC( g_backdc );
         g_backdc     = NULL;
         g_backoldbmp = NULL;
      }

      g_wnd = NULL;      
      g_traymode = false;
      g_taskbarcreate_msg = 0;
      g_current_dlg = NULL;
   }

   return true;
}

HWND get_handle( )
{
   return g_wnd;
}

void post_message( UINT msg, WPARAM wparam, LPARAM lparam )
{
   if (g_wnd)
   {
      PostMessage( g_wnd, msg, wparam, lparam );
   }
}

HWND on_client_CreateWindowEx (DWORD ex_style, LPCTSTR sz_class, 
                               LPCTSTR sz_window, DWORD style, 
                               int x, int y, int width, int height, 
                               HWND parent, HMENU menu, 
                               HINSTANCE inst, LPVOID param )
{
   if (!g_wnd && !strcmp( sz_class, "Furcadia@Client@DragonsEyeProductionsInc" ))
   {
      // No main window found yet.
      // Assume this is it.
      g_wnd = CreateWindowEx( ex_style, sz_class, sz_window, style | WS_CLIPCHILDREN,
                              x, y, width, height, parent, menu,
                              inst, param );

      if (g_wnd)
      {
         /* Enable double clicks. */
         SetClassLong( g_wnd, GCL_STYLE, 
                       GetClassLong( g_wnd, GCL_STYLE ) | CS_DBLCLKS );

         /* Redirect windowproc. */
         g_old_proc = (LRESULT(CALLBACK *)( HWND, UINT, WPARAM, LPARAM )) 
                      SetWindowLongPtr( g_wnd, GWL_WNDPROC, (LONG_PTR) window_proc );

         /* Create the timer. */
         SetTimer( g_wnd, TIMER_ID, 75, 0 );

         HDC dc = GetDC( g_wnd );

         /* Create the back buffer as a top-down 24-bit DIB. */
         BITMAPINFO bmi;
         memset( &bmi, 0, sizeof( bmi ) );
         bmi.bmiHeader.biBitCount = 32;
         bmi.bmiHeader.biPlanes   = 1;
         bmi.bmiHeader.biSize     = sizeof( bmi.bmiHeader );
         bmi.bmiHeader.biWidth    = 640;
         bmi.bmiHeader.biHeight   = -480;
         HBITMAP dib = CreateDIBSection( 0, &bmi, DIB_RGB_COLORS, 0, 0, 0 );
         g_backdc = CreateCompatibleDC( dc );
         g_backoldbmp = (HBITMAP) SelectObject( g_backdc, dib );

         ReleaseDC( g_wnd, dc );

         core::event_window_created( g_wnd );
      }

      return g_wnd;
   }

   // The client creates a tooltip control when it wants to immediately show
   // a tooltip.
   if (!strcmp( sz_class, TOOLTIPS_CLASS ))
   {
      // Make sure the cursor is INSIDE the client area before creating a tooltip.
      RECT client_r = { 0, 0, 0, 0 };
      GetClientRect( g_wnd, &client_r );
      {
         POINT origin = { client_r.left, client_r.top };
         ClientToScreen( g_wnd, &origin );
         client_r.right = origin.x + client_r.right - client_r.left;
         client_r.bottom = origin.y + client_r.bottom - client_r.top;
         client_r.left = origin.x;
         client_r.top = origin.y;
      }

      POINT cursor;
      GetCursorPos( &cursor );

      if (!PtInRect( &client_r, cursor ))
      {
         // Not inside, so cancel it.
         return NULL;
      }

      // Set the parent so WINE doesn't treat it like an overlapped window.
      return CreateWindowEx( ex_style, sz_class, sz_window, style,
                             x, y, width, height, g_wnd, menu,
                             inst, param );
   }


   return CreateWindowEx( ex_style, sz_class, sz_window, style,
                          x, y, width, height, parent, menu,
                          inst, param );
}

BOOL on_client_DestroyWindow( HWND wnd )
{
   BOOL r = DestroyWindow( wnd );

   if (r && g_wnd == wnd)
   {
      g_wnd = NULL;
      g_old_proc = NULL;

      DeleteObject( SelectObject( g_backdc, g_backoldbmp ) );
      DeleteDC( g_backdc );
      g_backdc     = NULL;
      g_backoldbmp = NULL;
   }

   return r;
}

int on_client_SetDIBitsToDevice( HDC hdc, int xDest, int yDest, 
                                 DWORD w, DWORD h, int xSrc, int ySrc, 
                                 UINT StartScan, UINT cLines, 
                                 CONST VOID * lpBits, CONST BITMAPINFO* lpbmi, 
                                 UINT fuColorUse )
{
   /* The client updates various portions of the window with this function.
      It no longer paints the entire contents of the window internally and
      does one giant blt. */

   assert( StartScan == 0 );  // Furc seems to always start at 0 even when resized.

   const_cast<BITMAPINFO*>(lpbmi)->bmiHeader.biCompression = BI_RGB;
   RECT r;
   GetClientRect( g_wnd, &r );

   if (cLines == r.bottom)
   {
      SetStretchBltMode( g_backdc, COLORONCOLOR );
      SetStretchBltMode( hdc, COLORONCOLOR );

      // Blt to the backdc.
      StretchDIBits( g_backdc, 0, 0, 640, 480, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, lpBits, lpbmi, 
                     DIB_RGB_COLORS, SRCCOPY );

      // Render on top of the client image.
      core::event_render( g_backdc, 640, 480 );
      
      float x_scale = 640.0f / lpbmi->bmiHeader.biWidth;
      float y_scale = 480.0f / lpbmi->bmiHeader.biHeight;
      StretchBlt( hdc, 0, 0, lpbmi->bmiHeader.biWidth, lpbmi->bmiHeader.biHeight, g_backdc, 0, 0, 640, 480, SRCCOPY );
      //StretchBlt( hdc, xDest, yDest, w, h, g_backdc, ceilf( xDest * x_scale ), ceilf( yDest * y_scale ), ceilf( w * x_scale ), ceilf( h * y_scale ), SRCCOPY );
      return h;
   }
   return SetDIBitsToDevice( hdc, xDest, yDest, w, h, xSrc, ySrc, StartScan, cLines,
                             lpBits, lpbmi, fuColorUse );
}

BOOL on_client_GetMessage( LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax )
{
   // Make sure to do dialog processing.
   while (::GetMessageW( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax )) // Use the unicode version.
   {
      // Perform dialog window processing on the active window.
      HWND active_wnd = GetActiveWindow( );
      if (active_wnd != g_wnd && 
          !SendMessage( active_wnd, furn2::F2M_ISNOTDLGWND, 0, 0 ) && IsDialogMessageW( active_wnd, lpMsg ))
          continue;
      return TRUE;
   }

   return FALSE;
}

UINT_PTR on_client_SetTimer( HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc )
{
   if (nIDEvent == 104) // Animation timer.
      g_timerproc104 = lpTimerFunc;
   return SetTimer( hWnd, nIDEvent, uElapse, lpTimerFunc );
}

LRESULT window_proc( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
   
   WNDPROCPARAMS wndprocparams = { hwnd, msg, wparam, lparam, 0 };
   if (core::event_wndproc( &wndprocparams ))
      return wndprocparams.retval;

   msg    = wndprocparams.msg;
   wparam = wndprocparams.wparam;
   lparam = wndprocparams.lparam;

   switch (msg)
   {
      /* The client likes to return successful hits on non-client area
         cursor positions, so take the NC messages away from it. */
   case WM_NCHITTEST:
   case WM_NCLBUTTONDOWN:
   case WM_NCLBUTTONUP:
   case WM_LBUTTONDBLCLK:
   case WM_NCRBUTTONDOWN:
   case WM_NCRBUTTONUP:
   case WM_NCRBUTTONDBLCLK:
   case WM_NCMOUSEMOVE:
   case WM_NCMOUSEHOVER:
   case WM_NCMOUSELEAVE:
      {
         return DefWindowProc( hwnd, msg, wparam, lparam );
      }
      break;

      /* The client depends on the current cursor to know what to do
         on a mouseclick, etc., and sets the cursor in WM_SETCURSOR.
         However, this is a bit inconveniencing to modules that draw
         widgets, which probably just block WM_MOUSEMOVES.  So take 
         away WM_SETCURSOR from the client, but send them along with
         WM_MOUSEMOVEs. */
      
   case WM_SETCURSOR:
      if (LOWORD( lparam ) == HTCLIENT)
         return FALSE;
   case WM_MOUSEMOVE:
      g_old_proc( hwnd, WM_SETCURSOR, 
                  (WPARAM) hwnd, HTCLIENT | (WM_MOUSEMOVE << 16) );
      break;
      

   case WM_WSAMSG:
      {
         net::on_wsa_event( WSAGETSELECTEVENT( lparam ), 
                            WSAGETSELECTERROR( lparam ) );
      }
      break;

   case WM_TIMER:
      {
         if (wparam == TIMER_ID)
         {
            core::event_tick( );
            return 0;
         }
      }
      break;

   case WM_CHAR:
      /* Modify the scan code produced by numpad keydowns to look
       * like regular numbers. */
      if (wparam >= '0' && wparam <= '9')
         lparam = (lparam & 0xFF00FFFF) | ( (wparam - '0' + 1) << 16 );
      break;

   case WM_KEYDOWN:
   case WM_KEYUP:
   case WM_SYSKEYDOWN:
   case WM_SYSKEYUP:
      // Convert VK_NUMPAD0-VK_NUMPAD9
      // to VK_0-VK_9
      if(wparam > 0x5F && wparam < 0x6A)
         wparam = wparam - 0x60 + 0x30;
      break;

   case WM_SYSCOMMAND:
      if (SC_MINIMIZE == wparam)
      {
         if (g_traymode)
         {
            if (tray_create( ))
            {
               tray_hide( );
               return true;
            }
         }
      }
      break;

   case WM_SHOWWINDOW:
      if (wparam) // Being shown.
         tray_remove( );
      break;

   case WM_SIZE:
      if (wparam == SIZE_RESTORED) // Full screen and being shown.
         tray_remove( );
      break;

   case WM_TRAYMSG:
      {
         if (lparam == WM_LBUTTONUP)
         {
            ShowWindow( g_wnd, SW_RESTORE );
            SetForegroundWindow( g_wnd );
         }
         else if (lparam == WM_RBUTTONUP)
         {
            SetForegroundWindow( g_wnd );
            POINT pos;
            GetCursorPos( &pos );
            TrackPopupMenu( g_traymenu, 0, pos.x, pos.y, 0, g_wnd, 0 );
         }
         return 0;
      }

   case WM_COMMAND:
      if (!HIWORD( wparam ))
      {
         if (LOWORD( wparam ) == TRAYCMD_RESTORE)
         {
            ShowWindow( g_wnd, SW_RESTORE );
            SetForegroundWindow( g_wnd );
         }
         else if (LOWORD( wparam ) == TRAYCMD_EXIT)
            PostQuitMessage( 0 );
      }
      break;

   case WM_BATCHMSG:
      core::event_batch( (Batch_Subsystem) wparam );
      return 0;

   case WM_ACTIVATE:
      // Don't let furc handle this, because it forgets to steal focus.
      return DefWindowProc( hwnd, msg, wparam, lparam );
   
   default:

      if (msg == g_taskbarcreate_msg)
      {
         /* Taskbar was recreated, so need to recreate systray icons. */
         if (g_traymode && !IsWindowVisible( g_wnd ))
            tray_create( );
      }
      break;
   }

   return g_old_proc( hwnd, msg, wparam, lparam );   
}

BOOL CALLBACK find_systray( HWND wnd, LPARAM lparam )
{
   char class_name[ 64 ]; class_name[ 63 ] = '\0';
   GetClassName( wnd, class_name, 63 );

   if (!strcmp( class_name, "TrayNotifyWnd" ))
   {
      // Found it.
      *((HWND*) lparam) = wnd;
      return FALSE;
   }

   return TRUE;
}

void tray_hide( )
{   
   /* Animate a rectangle to indicate minimizing. */
   
   RECT wnd_rect;
   GetWindowRect( g_wnd, &wnd_rect );
   HWND tray_wnd = 0;
   EnumChildWindows( FindWindow(  "Shell_TrayWnd", 0  ), find_systray, (LPARAM) &tray_wnd );

   if (tray_wnd)
   {
      RECT tray_rect = { 0, 0, 0, 0 };
      GetWindowRect( tray_wnd, &tray_rect );
      DrawAnimatedRects( g_wnd, IDANI_CAPTION, &wnd_rect, &tray_rect );
   }

   ShowWindow( g_wnd, SW_HIDE );
}

bool tray_create( )
{
   NOTIFYICONDATA nid;
   memset( &nid, 0, sizeof( nid ) );
   nid.cbSize           = sizeof( nid );
   nid.hWnd             = g_wnd;
   nid.uID              = TRAYICON_ID;
   nid.hIcon            = (HICON) GetClassLong( g_wnd, GCL_HICON );
   nid.uCallbackMessage = WM_TRAYMSG;
   nid.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
   GetWindowText( g_wnd, nid.szTip, 63 );

   if (!Shell_NotifyIcon( NIM_ADD, &nid ))
   {
      core::system_errspeak( "Unable to create a tray icon." );
      return false;
   }

   return true;
}

void tray_remove( )
{
   NOTIFYICONDATA nid;
   memset( &nid, 0, sizeof( nid ) );
   nid.cbSize           = sizeof( nid );
   nid.hWnd             = g_wnd;
   nid.uID              = TRAYICON_ID;

   Shell_NotifyIcon( NIM_DELETE, &nid );
}

bool on_command( const ArgList& args )
{
   string arg = args[ 0 ];
   to_lower( &arg );

   if (arg == "traymode")
   {
      if (args.size( ) == 2)
      {
         arg = args[ 1 ];
         to_lower( &arg );

         if (arg == "on")
         {
            g_traymode = true;
            core::system_speak( "Tray mode enabled." );
            return true;
         }
         else if (arg == "off")
         {
            g_traymode = false;
            core::system_speak( "Tray mode disabled." );
            return true;
         }
      }

      core::system_speak( "Usage: @traymode <'on'|'off'>" );
      return true;
   }
   else if (arg == "tickrate")
   {
      if (args.size( ) == 2)
      {
         arg = args[ 1 ];
         to_lower( &arg );

         string::const_iterator i = arg.begin( );
         for (; i != arg.end( ); ++i)
            if (!isdigit( *i ))
               break;
         if (i == arg.end( ))
         {
            unsigned rate = strtoul( arg.c_str( ), NULL, 10 );
            if (rate <= 100)
            {
               float factor = 1.0f - rate / 100.0f;
               SetTimer( g_wnd, 1, (unsigned) (33.0f + factor * 500.0f), NULL );
               SetTimer( g_wnd, 4, (unsigned) (20.0f + factor * 500.0f), NULL );
               if (g_timerproc104)
                  SetTimer( g_wnd, 104, (unsigned) (20.0f + factor * 500.0f), g_timerproc104 );

               std::ostringstream ss;
               ss << "Client tick-rate set to " << rate << "%.";
               core::system_speak( ss.str( ) );
               return true;
            }
         }
      }
      core::system_speak( "Usage: @tickrate <0-100>" );
      return true;
   }

   return false;
}

void set_current_dialog( HWND dlg ) {
   g_current_dlg = dlg;
}

} // namespace window