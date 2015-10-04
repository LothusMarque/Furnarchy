#include "apihooks.h"
#include "core.h"
#include "window.h"
#include "net.h"
#include "client.h"
#include <process.h>
#include <string.h>
#include <string>

#define HOOK_API( module, name, hook, by_ordinal )   \
        (apihooks::hook_api( (module), (void*) (name), (void*) (hook), (by_ordinal) ))

namespace apihooks {

bool g_initialized = false;

namespace hooks
{
   int STDCALL connect( SOCKET s, const sockaddr* name, int namelen );
   int STDCALL send( SOCKET s, const char* buf, int len, int flags );
   int STDCALL recv( SOCKET s, char* buf, int len, int flags );
   int STDCALL closesocket( SOCKET s );
   HWND STDCALL CreateWindowExA( DWORD ex_style, LPCTSTR sz_class,
                                 LPCTSTR sz_window, DWORD style,
                                 int x, int y, int width, int height,
                                 HWND parent, HMENU menu,
                                 HINSTANCE inst, LPVOID param );
   BOOL STDCALL DestroyWindow( HWND hwnd );
   BOOL STDCALL GetMessageA( LPMSG lpMsg, HWND hWnd, 
                             UINT wMsgFilterMin, UINT wMsgFilterMax );
   LRESULT STDCALL DispatchMessageA( const MSG* lpMsg );
   INT_PTR STDCALL DialogBoxParamA( HINSTANCE hInstance, LPCSTR lpTemplateName, 
                                    HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam );
   HMENU STDCALL LoadMenuA( HINSTANCE hInstance, LPCTSTR lpMenuName );
   HANDLE STDCALL CreateFileA( LPCTSTR lpFileName, DWORD dwDesiredAccess,
                               DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                               DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
                               HANDLE hTemplateFile );
   int STDCALL SetDIBitsToDevice( HDC hdc, 
                                  int xDest, int yDest, DWORD w, DWORD h, 
                                  int xSrc, int ySrc,
                                  UINT StartScan, UINT cLines,
                                  CONST VOID * lpBits, CONST BITMAPINFO* lpbmi, 
                                  UINT fuColorUse );
   int STDCALL TranslateAcceleratorA( HWND hWnd, HACCEL hAccTable, LPMSG lpMsg );
   UINT_PTR STDCALL SetTimer( HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc );
};

bool initialize( )
{
   if (!g_initialized)
   {
      g_initialized = true;
      
      HOOK_API( "wsock32.dll", 4, hooks::connect, true );
      HOOK_API( "wsock32.dll", 16, hooks::recv, true );
      HOOK_API( "wsock32.dll", 19, hooks::send, true );
      HOOK_API( "wsock32.dll", 3, hooks::closesocket, true );
      HOOK_API( "user32.dll", "CreateWindowExA", hooks::CreateWindowExA, false );
      HOOK_API( "user32.dll", "DestroyWindow", hooks::DestroyWindow, false );
      HOOK_API( "user32.dll", "GetMessageA", hooks::GetMessageA, false );
      HOOK_API( "user32.dll", "DispatchMessageA", hooks::DispatchMessageA, false );
      HOOK_API( "user32.dll", "TranslateAcceleratorA", hooks::TranslateAcceleratorA, false );
      HOOK_API( "user32.dll", "DialogBoxParamA", hooks::DialogBoxParamA, false );
      HOOK_API( "user32.dll", "LoadMenuA", hooks::LoadMenuA, false );
      HOOK_API( "user32.dll", "SetTimer", hooks::SetTimer, false );
      HOOK_API( "gdi32.dll", "SetDIBitsToDevice", hooks::SetDIBitsToDevice, false );
      HOOK_API( "kernel32.dll", "CreateFileA", hooks::CreateFileA, false );
   }

   return true;
}

bool uninitialize( )
{
   if (g_initialized)
   {
      g_initialized = false;

      HOOK_API( "wsock32.dll", 4, ::connect, true );
      HOOK_API( "wsock32.dll", 16, ::recv, true );
      HOOK_API( "wsock32.dll", 19, ::send, true );
      HOOK_API( "wsock32.dll", 3, ::closesocket, true );
      HOOK_API( "user32.dll", "CreateWindowExA", ::CreateWindowExA, false );
      HOOK_API( "user32.dll", "DestroyWindow", ::DestroyWindow, false );
      HOOK_API( "user32.dll", "GetMessageA", ::GetMessageA, false );
      HOOK_API( "user32.dll", "DispatchMessageA", ::DispatchMessageA, false );
      HOOK_API( "user32.dll", "DialogBoxParamA", ::DialogBoxParamA, false );
      HOOK_API( "user32.dll", "LoadMenuA", ::LoadMenuA, false );
      HOOK_API( "user32.dll", "SetTimer", ::SetTimer, false );
      HOOK_API( "user32.dll", "TranslateAcceleratorA", ::TranslateAcceleratorA, false );
      HOOK_API( "gdi32.dll", "SetDIBitsToDevice", ::SetDIBitsToDevice, false );
      HOOK_API( "kernel32.dll", "CreateFileA", ::CreateFileA, false );
   }

   return true;
}

void* hook_api( const char* module, const void* name, void* hook_proc,
                bool name_is_ordinal /* = false  */ )
{
   PIMAGE_DOS_HEADER dh = (PIMAGE_DOS_HEADER) GetModuleHandle( 0 );

   // Seek to PE header.
   PIMAGE_NT_HEADERS nth = (PIMAGE_NT_HEADERS)( ((size_t) dh) + dh->e_lfanew );

   // Make sure there are imports.
   size_t imports_vaddr = nth->OptionalHeader.
                           DataDirectory[ IMAGE_DIRECTORY_ENTRY_IMPORT ].VirtualAddress;

   if (!imports_vaddr)
      return 0;

   // Seek to first import descriptor.
   PIMAGE_IMPORT_DESCRIPTOR desc = (PIMAGE_IMPORT_DESCRIPTOR) ( (size_t) dh + imports_vaddr );

   // Find the proper module.
   while (desc->Name)
   {
      // Name is just another vaddr, though.
      if (!_stricmp( (const char*) ( (size_t) dh + desc->Name ), module ))
         break; // Found the correct module.

      ++desc;
   }

   if (!desc->Name)
      return 0;
   

   /* Go through the thunks looking for the right function. */

   PIMAGE_THUNK_DATA original_thunk = (PIMAGE_THUNK_DATA) 
                                       ( (size_t) dh + desc->OriginalFirstThunk );
   PIMAGE_THUNK_DATA real_thunk = (PIMAGE_THUNK_DATA) 
                                    ( (size_t) dh + desc->FirstThunk );
   
   for (; original_thunk->u1.Function ; original_thunk++, real_thunk++)
   {
      if (name_is_ordinal) // Going by ordinal.
      {
         if (original_thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG &&
             (original_thunk->u1.Ordinal & (~IMAGE_ORDINAL_FLAG)) == (DWORD) name)
         {
            // Found it.
            break;
         }
      }
      else
      {
         if (!(original_thunk->u1.Ordinal & IMAGE_ORDINAL_FLAG) &&
             !strcmp( (const char*) ( (size_t) dh + original_thunk->u1.AddressOfData + 2 ),
                      (const char*) name ))
         {
            // Found it.
            break;
         }
      }
   }

   if (!original_thunk->u1.Function)
      return 0;
      
   // Need to change write permissions to the page the IAT resides in.
   MEMORY_BASIC_INFORMATION mbi;
   VirtualQuery( real_thunk, &mbi, sizeof( mbi ) );

   VirtualProtect( mbi.BaseAddress, mbi.RegionSize, 
                   PAGE_READWRITE, &mbi.Protect);

   void* old_entry = (void*) (real_thunk->u1.Function);

   // Replace with hook.
   real_thunk->u1.Function = (DWORD) hook_proc;

   // Reset page permissions.
   VirtualProtect( mbi.BaseAddress, mbi.RegionSize, 
                   mbi.Protect, &mbi.Protect );

   return old_entry;
}

int STDCALL hooks::connect( SOCKET s, const sockaddr* name, int namelen )
{
   return net::on_client_connect( s, (sockaddr_in*) name );
}

int STDCALL hooks::send( SOCKET s, const char* buf, int len, int flags )
{
   return net::on_client_send( s, buf, len );
}

int STDCALL hooks::recv( SOCKET s, char* buf, int len, int flags )
{
	return net::on_client_recv( s, buf, len );
}

int STDCALL hooks::closesocket( SOCKET s )
{
   return net::on_client_closesocket( s );
}

HWND STDCALL hooks::CreateWindowExA( DWORD ex_style, LPCTSTR sz_class, 
                                     LPCTSTR sz_window, DWORD style, 
                                     int x, int y, int width, int height, 
                                     HWND parent, HMENU menu, HINSTANCE inst, 
                                     LPVOID param )
{
   return window::on_client_CreateWindowEx( ex_style, sz_class, sz_window,
                                            style, x, y, width, height, parent,
                                            menu, inst, param );
}


BOOL STDCALL hooks::DestroyWindow( HWND hwnd )
{
   return window::on_client_DestroyWindow( hwnd );
}

BOOL STDCALL hooks::GetMessageA( LPMSG lpMsg, HWND hWnd, 
                                 UINT wMsgFilterMin, UINT wMsgFilterMax )
{
   if (!window::on_client_GetMessage( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax )) 
   {
      core::uninitialize( );
      return FALSE;
   }
   return TRUE;
}

LRESULT STDCALL hooks::DispatchMessageA( const MSG* lpMsg )
{
   // Make unicode.
   if (lpMsg)
      return DispatchMessageW( lpMsg );
   return 0;
}

INT_PTR STDCALL hooks::DialogBoxParamA( HINSTANCE hInstance, LPCSTR lpTemplateName, 
                                        HWND hWndParent, DLGPROC lpDialogFunc, 
                                        LPARAM dwInitParam )
{
   // If the client launches with -pick and the user cancels, the client
   // just lets WinMain return.
   if (*(WORD*)&lpTemplateName == 10002) // The ID of the dialog resource.
   {
      INT_PTR r = ::DialogBoxParamA( hInstance, lpTemplateName, hWndParent, 
                                     lpDialogFunc, dwInitParam );
      if (r == 0 || r == IDCANCEL)
         core::uninitialize( );
      return r;
   }

   return ::DialogBoxParamA( hInstance, lpTemplateName, hWndParent, 
                             lpDialogFunc, dwInitParam );
}

int STDCALL hooks::TranslateAcceleratorA( HWND hWnd, HACCEL hAccTable, LPMSG lpMsg )
{
   /* I suspect the client is calling this for any window, not just its main window,
      so it ends up stealing accelerator strokes from child controls. */   
   return ::TranslateAcceleratorA( lpMsg->hwnd, hAccTable, lpMsg );
}

int STDCALL hooks::SetDIBitsToDevice( HDC hdc, int xDest, int yDest, 
                                      DWORD w, DWORD h, 
                                      int xSrc, int ySrc, 
                                      UINT StartScan, UINT cLines, 
                                      CONST VOID * lpBits, CONST BITMAPINFO* lpbmi, 
                                      UINT fuColorUse )
{
   /* The client calls StretchDIBits exactly once, when it blts the
      DIB of the screen to the window. */
   return window::on_client_SetDIBitsToDevice( hdc, xDest, yDest, w, h,
                                               xSrc, ySrc, StartScan, cLines,
                                               lpBits, lpbmi, fuColorUse );
}

HANDLE STDCALL hooks::CreateFileA( LPCTSTR lpFileName, DWORD dwDesiredAccess,
                                   DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                   DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes,
                                   HANDLE hTemplateFile )
{
   if (lpFileName)
   {
      const char* file = lpFileName;
      const char* p = strrchr( file, '\\' );
      if (p)
         file = p + 1;

      if (!_stricmp( file, "title261.pcx" ))
         core::event_screen_load( SCREEN_TITLE );
      else if (!_stricmp( file, "border1.pcx" ))
         core::event_screen_load( SCREEN_BORDER );
      else if (!_stricmp( file, "pickem13.pcx" ))
         core::event_screen_load( SCREEN_PICKEM );
      else if (!_stricmp( file, "marbled.pcx" ))
         core::event_screen_load( SCREEN_MARBLED );
   }

   return ::CreateFileA( lpFileName, dwDesiredAccess, dwShareMode,
                         lpSecurityAttributes, dwCreationDisposition,
                         dwFlagsAndAttributes, hTemplateFile );
}

HMENU STDCALL hooks::LoadMenuA( HINSTANCE hInstance, LPCTSTR lpMenuName )
{
   return client::on_client_LoadMenu( hInstance, lpMenuName );
}

UINT_PTR STDCALL hooks::SetTimer( HWND hWnd, UINT_PTR nIDEvent, UINT uElapse, TIMERPROC lpTimerFunc )
{
   return window::on_client_SetTimer( hWnd, nIDEvent, uElapse, lpTimerFunc );
}

} // namespace apihooks