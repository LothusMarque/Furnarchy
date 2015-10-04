#ifndef _SWATCHCTRL_H__
#define _SWATCHCTRL_H__

#include <Windows.h>

#define WC_SWATCH "Swatch"

enum
{
   SWM_FILL = WM_USER,
   SWM_SETSWATCHSIZE,
   SWM_GETCOUNT,
   SWM_GETSEL,
   SWM_SETSEL,
   SWM_GETCOLOR,
   SWM_SETBKGNDCOLOR,
};

enum
{
   SWN_SELCHANGE = 1,
};

struct NMSWSELCHANGE
{
   NMHDR hdr;
   unsigned int sel;
   COLORREF color;
};

extern void Swatch_Init( HINSTANCE inst );
extern void Swatch_Uninit( HINSTANCE inst );

inline void Swatch_SetSwatchSize( HWND sw, unsigned int size ) {
   SendMessage( sw, SWM_SETSWATCHSIZE, size, 0 );
}

inline void Swatch_Fill( HWND sw, unsigned int num, const COLORREF* colors ) {
   SendMessage( sw, SWM_FILL, (WPARAM) num, (LPARAM) colors );
}

inline unsigned int Swatch_GetCount( HWND sw ) {
   return (unsigned int) SendMessage( sw, SWM_GETCOUNT, 0, 0 );
}

inline unsigned int Swatch_GetSel( HWND sw ) {
   return (unsigned int) SendMessage( sw, SWM_GETSEL, 0, 0 );
}

inline void Swatch_SetSel( HWND sw, unsigned int sel ) {
   SendMessage( sw, SWM_SETSEL, sel, 0 );
}

inline COLORREF Swatch_GetColor( HWND sw, unsigned int index ) {
   return (COLORREF) SendMessage( sw, SWM_GETCOLOR, index, 0 );
}

inline void Swatch_SetBkgndColor( HWND sw, COLORREF color ) {
   SendMessage( sw, SWM_SETBKGNDCOLOR, (WPARAM) color, 0 );
}

#endif