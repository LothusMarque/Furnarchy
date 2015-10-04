#ifndef _FURNARCHYCORE_WINHOOKS_H__
#define _FURNARCHYCORE_WINHOOKS_H__

#include "common.h"

namespace window
{
   enum { WM_WSAMSG = WM_USER, TIMER_ID = 0xFA2, TRAYICON_ID = 0xFA2,
          WM_TRAYMSG = WM_USER + 0x40F2, WM_BATCHMSG = WM_USER + 0x094C,
          TRAYCMD_EXIT = 0xFABA, TRAYCMD_RESTORE = 0xFEEB };

   bool initialize( );
   bool uninitialize( );

   HWND get_handle( );
   void post_message( UINT msg, WPARAM wparam, LPARAM lparam );
   void set_current_dialog( HWND dlg );

   bool on_command( const ArgList& args );
   HWND on_client_CreateWindowEx( DWORD ex_style, LPCTSTR sz_class, 
                                  LPCTSTR sz_window, DWORD style, 
                                  int x, int y, int width, int height, 
                                  HWND parent, HMENU menu, HINSTANCE inst, 
                                  LPVOID param );
   BOOL on_client_DestroyWindow( HWND wnd );
   int  on_client_SetDIBitsToDevice( HDC hdc, 
                                     int xDest, int yDest, DWORD w, DWORD h, 
                                     int xSrc, int ySrc,
                                     UINT StartScan, UINT cLines,
                                     CONST VOID * lpBits, CONST BITMAPINFO* lpbmi, 
                                     UINT fuColorUse );
   BOOL on_client_GetMessage( LPMSG lpMsg, HWND hWnd, 
                              UINT wMsgFilterMin, UINT wMsgFilterMax );
   UINT_PTR on_client_SetTimer( HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc );

}
   
#endif
