#ifndef _POORTEXTBOX_H__
#define _POORTEXTBOX_H__

#include <Windows.h>
#include <CommCtrl.h>

//////////////////////////////////////////////////////////////////////////

/** Window Messages. */
enum 
{ 
   PTBM_SETSEL = WM_USER + 0x20,
   PTBM_GETSEL, 
   PTBM_REPLACESEL,
   PTBM_SETTEXTLIMIT,
   PTBM_GETTEXTLIMIT,
   PTBM_GETLINECOUNT,
   PTBM_TRIMLINES,
   PTBM_ENSUREVISIBLE,
   PTBM_SETBKGNDCOLOR,
   PTBM_APPEND,
   PTBM_GETTEXT,
   PTBM_FLUSH,
   PTBM_SETDEFAULTCOLOR,
   PTBM_GETDEFAULTCOLOR,
   PTBM_SETDEFAULTSTYLE,
   PTBM_GETDEFAULTSTYLE,
   PTBM_SETDEFAULTPARAM,
   PTBM_GETDEFAULTPARAM,
   PTBM_GETCONTENTSIZE,
   PTBM_ISEMPTY,
};

/** Notifications. */
enum
{
   PTBN_SELCHANGED = 0x0100, // The start of the edit control notifications.
   PTBN_CHANGED,
   PTBN_ELEMACTION,
   PTBN_RETURN,
   PTBN_TAB,
   PTBN_ESCAPE,
   PTBN_CTRLKEY,
};

/** Styles. */
enum
{
   PTBS_LEFT       = 0x0000,   // The start of edit control styles.
   PTBS_RIGHT      = 0x0001,
   PTBS_CENTER     = 0x0002,
   PTBS_READONLY   = 0x0004,
   PTBS_MULTILINE  = 0x0008,
};

/** PTBGETTEXT flag values. */
enum
{
   PTBGETTEXT_CRLF    = 0x01,
   PTB_GETTEXT_IMGALT = 0x02,
};

#ifdef UNICODE
   #define WC_POORTEXTBOX L"PoorTextBox"
#else 
   #define WC_POORTEXTBOX "PoorTextBox"
#endif

#define PTB_NOIMAGE -1
#define PTB_DEFAULT_TEXTLIMIT 0xFFFF

enum { PTB_CARET_FIRST = 0, PTB_CARET_LAST = -1 };

enum Ptb_Elem_Type { PTB_ELEM_TYPE_EMPTY = 0, PTB_ELEM_TYPE_STR, PTB_ELEM_TYPE_IMG, 
                     PTB_ELEM_TYPE_BREAK };

/** Element styles. */
enum 
{ 
   PTB_ELEM_STYLE_NORMAL    = 0x0000,
   PTB_ELEM_STYLE_BOLD      = 0x0100, 
   PTB_ELEM_STYLE_ITALIC    = 0x0200, 
   PTB_ELEM_STYLE_UNDERLINE = 0x0400,
   PTB_ELEM_STYLE_LINK      = 0x00010000,
};

/** NMPTBELEMACTION actions. */
enum Ptb_Elem_Action
{
   PTB_ELEMACTION_CLICK, PTB_ELEMACTION_RCLICK,
};

//////////////////////////////////////////////////////////////////////////

struct PTBELEM
{
   Ptb_Elem_Type  type;
   int            style;
   LPARAM         param;
   COLORREF       color;
   HIMAGELIST     imglist;
   int            img_index;
   const wchar_t* text;
   const wchar_t* alt_text;
};

struct PTBGETTEXT
{
   size_t   begin;
   size_t   end;
   wchar_t* text;
   size_t   text_max;
   int      flags;
};

struct NMPTBELEMACTION
{
   NMHDR           hdr;
   Ptb_Elem_Action action;
   LPARAM          param;
   int             offset;
};

struct NMPTBCTRLKEY
{
   NMHDR hdr;
   int   vk_code;
};

//////////////////////////////////////////////////////////////////////////

/** Sets the current selection.
*  If \a beg == \a end, nothing is selected. -1 may be used to specify the
*  end of the contents.
*  \return The number of characters selected. */
inline size_t PoorTextBox_SetSel( HWND ptb, size_t beg, size_t end ) {
   return (size_t) SendMessage( ptb, PTBM_SETSEL, (WPARAM) beg, (WPARAM) end );
}

/** Retrieve the current selection. */
inline void PoorTextBox_GetSel( HWND ptb, size_t* beg, size_t* end ) {
   SendMessage( ptb, PTBM_GETSEL, (WPARAM) beg, (LPARAM) end );
}

/** Replace the current selection.
*  The caller is responsible for the lifetime of imagelists that are passed in. */
inline void PoorTextBox_ReplaceSel( HWND ptb, size_t count, PTBELEM* elems ) {
   SendMessage( ptb, PTBM_REPLACESEL, (WPARAM) count, (LPARAM) elems );
}

/** Sets the text limit. */
inline void PoorTextBox_SetTextLimit( HWND ptb, size_t limit ) {
   SendMessage( ptb, PTBM_SETTEXTLIMIT, (WPARAM) limit, 0 );
}

/** Retrieves the text limit. */
inline size_t PoorTextBox_GetTextLimit( HWND ptb ) {
   return (size_t) SendMessage( ptb, PTBM_GETTEXTLIMIT, 0, 0 );
}

/** Retrieves the number of lines. */
inline size_t PoorTextBox_GetLineCount( HWND ptb ) {
   return (size_t) SendMessage( ptb, PTBM_GETLINECOUNT, 0, 0 );
}

/** Ensures the text box only has \a max lines lines.
*  Lines are trimmed from the top.
*  \return The number of lines left. */
inline size_t PoorTextBox_TrimLines( HWND ptb, size_t max_lines ) {
   return (size_t) SendMessage( ptb, PTBM_TRIMLINES, (WPARAM) max_lines, 0 );
}

/** Ensures a position is visible. */
inline void PoorTextBox_EnsureVisible( HWND ptb, size_t pos ) {
   SendMessage( ptb, PTBM_ENSUREVISIBLE, pos, 0 );
}

/** Sets the background color for the control. */
inline void PoorTextBox_SetBkgndColor( HWND ptb, COLORREF color ) {
   SendMessage( ptb, PTBM_SETBKGNDCOLOR, (WPARAM) color, 0 );
}

/** Appends to the end of the contents. 
*  The caller is responsible for the lifetime of imagelists that are passed in. */
inline void PoorTextBox_Append( HWND ptb, size_t count, PTBELEM* elems ) {
   SendMessage( ptb, PTBM_APPEND, (WPARAM) count, (LPARAM) elems );
}

/** Retrieves a range of text.
*  \return The length of the copied string. */
inline size_t PoorTextBox_GetText( HWND ptb, PTBGETTEXT* gettext ) {
   return (size_t) SendMessage( ptb, PTBM_GETTEXT, 0, (LPARAM) gettext );
}

/** Empties the entire contents of the text box. */
inline void PoorTextBox_Flush( HWND ptb ) {
   SendMessage( ptb, PTBM_FLUSH, 0, 0 );
}

/** Sets the default color for the text box. */
inline void PoorTextBox_SetDefaultColor( HWND ptb, COLORREF color ) {
   SendMessage( ptb, PTBM_SETDEFAULTCOLOR, (WPARAM) color, 0 );
}

/** Retrieves the default style for text box. */
inline COLORREF PoorTextBox_GetDefaultColor( HWND ptb ) {
   return (COLORREF) SendMessage( ptb, PTBM_GETDEFAULTCOLOR, 0, 0 );
}

/** Sets the default style for the text box. */
inline void PoorTextBox_SetDefaultStyle( HWND ptb, int style ) {
   SendMessage( ptb, PTBM_SETDEFAULTSTYLE, (WPARAM) style, 0 );
}

/** Retrieves the default style for text box. */
inline int PoorTextBox_GetDefaultStyle( HWND ptb ) {
   return (int) SendMessage( ptb, PTBM_GETDEFAULTSTYLE, 0, 0 );
}

/** Sets the default lparam for the text box. */
inline void PoorTextBox_SetDefaultParam( HWND ptb, LPARAM param ) {
   SendMessage( ptb, PTBM_SETDEFAULTPARAM, 0, param );
}

/** Retrieves the default lparam for the text box. */
inline LPARAM PoorTextBox_GetDefaultParam( HWND ptb ) {
   return (LPARAM) SendMessage( ptb, PTBM_GETDEFAULTPARAM, 0, 0 );
}

/** Retrieves the size of the contents of the text box. */
inline void PoorTextBox_GetContentSize( HWND ptb, SIZE* out ) {
   SendMessage( ptb, PTBM_GETCONTENTSIZE, 0, (LPARAM) out );
}

/** Checks whether or not the text box is empty.
*  This is faster than asking for a length. */
inline bool PoorTextBox_IsEmpty( HWND ptb ) {
   return !!SendMessage( ptb, PTBM_ISEMPTY, 0, 0 );
}

//////////////////////////////////////////////////////////////////////////

/** Initialize the window class.
*   Should only be called once before a call to PoorTextBox_Uninit.
*   Not thread-safe. */
extern void PoorTextBox_Init( HINSTANCE inst );
/** Uninitialize the window class.
*   Not thread-safe. */
extern void PoorTextBox_Uninit( HINSTANCE inst );

#endif
