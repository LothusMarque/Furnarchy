#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <WindowsX.h>
#include "PoorTextBox.h"

#include <list>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <xutility>

using namespace std;

//////////////////////////////////////////////////////////////////////////

enum { PTB_FONT_NORMAL = 0x00, PTB_FONT_BOLD = 0x01, 
       PTB_FONT_ITALIC = 0x02, PTB_FONT_UNDERLINE = 0x04,
       PTB_NUM_FONTS = 8 };

enum { MAX_ELEM_LEN = 256, IMG_PADDING = 2, LINE_PADDING = 1, LINE_MARGIN = 5 };
enum { PTB_FONT_QUALITY = 5 };
enum { DIRTY_DC = 0x1, DIRTY_LINES = 0x2, DIRTY_VISIBLE_LINES = 0x4, DIRTY_CARET = 0x8 };
enum { AREA_NOWHERE = 0x0, AREA_ABOVE = 0x1, AREA_BELOW = 0x2, 
       AREA_LEFT = 0x4, AREA_RIGHT = 0x8 };
enum { SCROLL_TIMER = 0x421F, SCROLL_RESOLUTION = 100 };
enum { ID_EDIT_CUT = 0x1000, ID_EDIT_COPY, ID_EDIT_PASTE, 
       ID_EDIT_CLEAR,  ID_EDIT_SELECT_ALL };

#define STYLEFONT( style ) ((style & 0x0000FF00) >> 8)

//////////////////////////////////////////////////////////////////////////

class PtbInst
{
   struct DIMS { int w, h; };

   struct ELEM
   {
      Ptb_Elem_Type type;
      size_t        len;

      ELEM( ) : type( PTB_ELEM_TYPE_EMPTY ), len( 0 ) {  }
      virtual ~ELEM( ) {}
      virtual bool check( ) const { return len == 0; }
   };

   struct ELEMBRK : public ELEM
   {
      ELEMBRK( ) { type = PTB_ELEM_TYPE_BREAK; len = 1; }
      virtual bool check( ) const { return len == 1; }
   };

   struct ELEMSTR : public ELEM
   {
      wstring       text;
      wstring       alt_text;
      int           style;
      COLORREF      color;
      LPARAM        param;

      ELEMSTR( ) : style( PTB_ELEM_STYLE_NORMAL ), param( 0 ) {
         type = PTB_ELEM_TYPE_STR;
         len  = 0;
      }
      ELEMSTR( const ELEMSTR& r ) : text( r.text ), alt_text( r.alt_text ),
         style( r.style ), param( r.param ) { type = r.type; len = r.len; }
      virtual bool check( ) const { return len == text.length( ); }
   };

   struct ELEMIMG : public ELEM
   {
      HIMAGELIST imglist;
      int        index;
      wstring    alt_text;
      int        style;
      LPARAM     param;

      ELEMIMG( ) : imglist( NULL ), index( 0 ), 
                   style( PTB_ELEM_STYLE_NORMAL ), param( 0 )
      {
         type = PTB_ELEM_TYPE_IMG;
         len = 1;
      }
      virtual bool check( ) const { return len == 1; }
   };

   class ElemIter
   {
   public:
      list<ELEM*>*          m_list;
      list<ELEM*>::iterator m_elem;
      size_t                m_off;

   private:
      ElemIter( ) { }
   public:
      ElemIter( list<ELEM*>* p_list ) : m_list( p_list ), m_off( 0 )
      { m_elem = m_list->begin( ); }
      ElemIter( list<ELEM*>* p_list, list<ELEM*>::iterator elem, size_t off = 0 )
         : m_list( p_list ), m_elem( elem ), m_off( off ) { }
      ElemIter( list<ELEM*>* p_list, size_t chars );
      
      ELEM* elem( ) { return *m_elem; }
      ELEMSTR* elemStr( ) { return (ELEMSTR*) *m_elem; }
      ELEMIMG* elemImg( ) { return (ELEMIMG*) *m_elem; }
      ELEMBRK* elemBrk( ) { return (ELEMBRK*) *m_elem; }

      bool operator == ( const ElemIter& iter ) const {
         return (iter.m_list == m_list && iter.m_elem == m_elem && iter.m_off == m_off);
      }
      bool operator != ( const ElemIter& iter ) const {
         return (iter.m_list != m_list || iter.m_elem != m_elem || iter.m_off != m_off);
      }
      bool operator !( ) const { return (m_list->end( ) == m_elem); }

      bool isFirst( ) const 
      { return (m_list->begin( ) == m_elem && m_off == 0); }

      ElemIter& next( )
      {
         if (m_elem != m_list->end( )) { ++m_elem; m_off = 0; }
         return *this; 
      }
      ElemIter& prev( )
      {
         if (m_list->size( ) && m_elem != m_list->begin( )) { --m_elem; }
         m_off = 0;
         return *this;
      }
      ElemIter& nextChars( size_t len )
      {
         list<ELEM*>::iterator end = m_list->end( );

         // Advance.
         while (len && m_elem != end)
         {
            if (m_off + len < (*m_elem)->len)
            {
               m_off += len;
               break;
            }
            
            len -= (*m_elem)->len - m_off;
            ++m_elem;
            m_off = 0;
         }

         return *this;
      }
      ElemIter& prevChars( size_t len )
      {
         list<ELEM*>::iterator begin = m_list->begin( );

         // Advance.
         while (len && (m_elem != begin || m_off))
         {
            if (m_off)
            {
               assert( m_off < (*m_elem)->len );
               size_t n = min( len, m_off );
               m_off -= n;
               len -= n;
            }
            else
            {
               --m_elem;
               
               if ((*m_elem)->len)
               {
                  m_off = (*m_elem)->len - 1;
                  len -= 1;
               }
               else
                  m_off = 0;
            }
         }

         return *this;
      }
      ElemIter& nextChar( ) { return nextChars( 1 ); }
      ElemIter& prevChar( ) { return prevChars( 1 ); }
      ElemIter& fromCharOffset( size_t offset )
      {
         if (offset != -1 && m_list->size( ))
         {
            m_elem = m_list->begin( );
            m_off  = 0;
            return nextChars( offset );
         }
         m_elem = m_list->end( );
         m_off = 0;
         return *this;
      }
      size_t toCharOffset( ) const
      {
         size_t offset = 0;
         list<ELEM*>::const_iterator i = m_list->begin( );
         list<ELEM*>::const_iterator end = m_list->end( );
         for (; i != end; ++i)
         {
            if (i == m_elem)
            {
               assert( m_off < (*i)->len );
               offset += m_off;
               break;
            }
            
            offset += (*i)->len;
         }

         return offset;
      }
   };

   struct LINE
   {
      ElemIter iter;
      size_t   off;
      size_t   len;
      // Whether the line is a continued in the next line.
      bool     is_cont;
      RECT     rect;
      // Distances from rect.left to the right of each character.
      int*     dx;

      LINE( ElemIter& iter, size_t off, size_t len, 
            bool is_cont, const RECT& rect, int* dx = NULL ) 
         : iter( iter ), off( off ), len( len ), is_cont( is_cont ), 
           rect( rect ), dx( dx ) { }
   };

public:
   
   PtbInst( HWND wnd );
   ~PtbInst( );

   void eraseSel( );
   void erase( size_t off, size_t len );
   void eraseAll( );
   void replaceSel( size_t count, const PTBELEM* elems );
   void insert( size_t off, size_t count, const PTBELEM* elems );
   void fixSel( );
   bool checkSel( );
   void compact( );
   // Requires the text be compacted.
   void buildLines( );
   // Requires the text be compacted.
   void paint( HDC dc );
   void setSel( size_t begin, size_t end );
   void updateCaret( );
   /** Selects the word the caret is in. */
   void selectWord( );
   void trimLines( size_t max_lines );

   void ensureVisible( size_t off );
   void setBkColor( COLORREF color );
   void setFont( HFONT font );
   void setLimit( size_t limit );
   
   void     toString( size_t off, size_t len, wstring* out, bool crlf = false, bool alts = false );
   size_t   toStringLength( size_t off, size_t len, bool crlf = false );
   ElemIter elemFromPoint( const POINT& pt );
   size_t   offsetFromPoint( const POINT& pt );
   long     areaFromPoint( const POINT& pt );
   size_t   lineFromOffset( size_t off );
   size_t   findHome( size_t off );
   size_t   findEnd( size_t off );
   void     showTip( const wstring& tip, int x, int y );
   void     hideTip( );
   
   /** Compute the dimensions of a series of characters.
   *   Requires the text be compacted.
   *   \return \c true if all \a len characters were used. */
   bool charsSize( const ElemIter& first, const size_t len, DIMS* size );
   /** Calculate how many characters can fit in a width.
   *   Requires the text be compacted. */
   size_t charsFit( const ElemIter& first, const size_t len, 
                    const int max_w, DIMS* size );
   /** Computes the individual offsets for each character in a series from the beginning (0).
   *   Requires the text be compacted.
   *   \return \c true if all \a len characters were used. */
   bool charsDx( const ElemIter& first, size_t len, int* dx );

   static LRESULT CALLBACK wnd_proc( HWND wnd, UINT msg, 
                                     WPARAM wparam, LPARAM lparam );

private:

   HWND        m_wnd;
   HMENU       m_menu;
   HWND        m_tips;
   HDC         m_dc;
   // The last "good" client size (in case the window gets minimized).
   SIZE        m_clientsize;
   HFONT       m_fonts[ PTB_NUM_FONTS ];
   TEXTMETRIC  m_textmetric;
   HBRUSH      m_bgbrush;
   // Default color for character input/pastes/etc.
   COLORREF    m_defaultcolor;
   // Default style for character input/pastes/etc.
   int         m_defaultstyle;
   // Default param for character input/pastes/etc.
   LPARAM      m_defaultparam;

   list<ELEM*>  m_elems;
   vector<LINE> m_lines;
   size_t       m_limit;
   size_t       m_length;

   // First visible line.
   size_t       m_top_line;
   // Actually, the line after the last visible line.
   size_t       m_bottom_line;
   POINT        m_scroll;
   DIMS         m_content_size;

   bool m_dragging;
   long m_dirty;

   // Previous cursor position, needed because
   // stupid TrackMouseEvent likes to send a WM_MOUSEMOVE along
   // with WM_MOUSEHOVER.
   POINT        m_cursor;
   
   struct  
   {
      // Character offsets.
      size_t begin, end;
   }           m_selection;
};

inline PtbInst::ElemIter::ElemIter( list<ELEM*>* p_list, size_t chars ) 
: m_list( p_list ) { fromCharOffset( chars ); }

//////////////////////////////////////////////////////////////////////////

static BOOL(WINAPI *_GetTextExtentExPointW)( HDC hdc, LPCWSTR lpszStr, int cchString, 
                                             int nMaxExtent, LPINT lpnFit, LPINT alpDx, 
                                             LPSIZE lpSize );

//////////////////////////////////////////////////////////////////////////

static HANDLE to_global_unicode_text( const wstring& str );
static HANDLE to_global_text( const wstring& str );
static wchar_t* crlf_to_lf( wchar_t* sz );
static BOOL WINAPI GetTextExtentExPointW98( HDC hdc, LPCWSTR lpszStr, int cchString, 
                                            int nMaxExtent, LPINT lpnFit, LPINT alpDx, 
                                            LPSIZE lpSize );
static void send_ptb_changed( HWND ptb );

//////////////////////////////////////////////////////////////////////////

static bool g_clipboard_unicode_available = true;

//////////////////////////////////////////////////////////////////////////

void PoorTextBox_Init( HINSTANCE inst )
{
   /* Check if CF_UNICODETEXT is available as a clipboard format. */
   OpenClipboard( NULL );
   g_clipboard_unicode_available = false;
   for (UINT format = 0; ERROR_SUCCESS != (format = EnumClipboardFormats( format ));)
   {
      if (CF_UNICODETEXT == format)
      {
         g_clipboard_unicode_available = true;
         break;
      }
   }
   CloseClipboard( );

   /* Register the window class. */
   {
      WNDCLASSEX wc;
      memset( &wc, 0, sizeof( wc ) );
      wc.cbSize        = sizeof( wc );
      wc.hCursor       = NULL; //LoadCursor( NULL, IDC_IBEAM );
      wc.hbrBackground = GetSysColorBrush( COLOR_WINDOW );
      wc.hInstance     = inst;
      wc.style         = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC | CS_DBLCLKS;
      wc.cbWndExtra    = sizeof( void* );
      wc.lpfnWndProc   = PtbInst::wnd_proc;
      wc.lpszClassName = "PoorTextBox";
      RegisterClassEx( &wc );
   }

   /* If windows 98, point _GetTextExtentExPointW to GetTextExtentExPointW98 */
   {
      OSVERSIONINFO ver;
      ver.dwOSVersionInfoSize = sizeof( ver );
      GetVersionEx( &ver );
      if (ver.dwMajorVersion == 4)
         (void*&)_GetTextExtentExPointW = (void*) GetTextExtentExPointW98;
      else
         (void*&)_GetTextExtentExPointW = (void*) GetTextExtentExPointW;
   }
}

void PoorTextBox_Uninit( HINSTANCE inst )
{
   UnregisterClass( "PoorTextBox", inst );
}

//////////////////////////////////////////////////////////////////////////

PtbInst::PtbInst( HWND wnd )
   : m_wnd( wnd ), m_dirty( DIRTY_DC )
{   
   m_clientsize.cx = m_clientsize.cy = 0;
   m_selection.begin = 0;
   m_selection.end   = 0;
   m_top_line = m_bottom_line = 0;
   m_scroll.x = m_scroll.y = 0;
   m_dragging = false;
   m_limit = PTB_DEFAULT_TEXTLIMIT;
   m_length = 0;
   m_defaultcolor = GetSysColor( COLOR_WINDOWTEXT );
   m_defaultstyle = PTB_ELEM_STYLE_NORMAL;
   m_defaultparam = 0;
   m_content_size.w = m_content_size.h = 0;
   m_cursor.x = m_cursor.y = 0;

   m_bgbrush = CreateSolidBrush( GetSysColor( COLOR_WINDOW ) );

   /* Create the back buffer DC. */
   {
      HDC parent_dc = GetDC( GetParent( wnd ) );
      SaveDC( m_dc = CreateCompatibleDC( parent_dc ) );

      SelectObject( m_dc, 
         CreateCompatibleBitmap( parent_dc, max( (LONG) 1, m_clientsize.cx ), max( (LONG) 1, m_clientsize.cy ) ) );
   }

   /* Create the menu. */
   {
      m_menu = CreatePopupMenu( );
      AppendMenu( m_menu, MF_STRING | MF_ENABLED, ID_EDIT_CUT, "Cut" );
      AppendMenu( m_menu, MF_STRING | MF_ENABLED, ID_EDIT_COPY, "Copy" );
      AppendMenu( m_menu, MF_STRING | MF_ENABLED, ID_EDIT_PASTE, "Paste" );
      AppendMenu( m_menu, MF_STRING | MF_ENABLED, ID_EDIT_CLEAR, "Delete" );
      AppendMenu( m_menu, MF_SEPARATOR, 0, NULL );
      AppendMenu( m_menu, MF_STRING | MF_ENABLED, ID_EDIT_SELECT_ALL, "Select All" );
   }

   /* Create the tooltips control. */
   {
      m_tips = CreateWindowEx( WS_EX_TOPMOST, TOOLTIPS_CLASS, 0,
                               WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               m_wnd, // Putting an hParent crashes my win98 vm?
                               0, 0, 0 );

      SetWindowPos( m_tips, HWND_TOPMOST, 0, 0, 0, 0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE );
      SendMessage( m_tips, TTM_SETDELAYTIME, TTDT_INITIAL, 0 );
      SendMessage( m_tips, TTM_SETDELAYTIME, TTDT_RESHOW, 0 );
      SendMessage( m_tips, TTM_ACTIVATE, TRUE, 0 );

      /* Add a single tracking tooltip. */
      TOOLINFOW ti;
      ti.cbSize      = sizeof( ti );
      ti.uFlags      = TTF_TRACK | TTF_ABSOLUTE;
      ti.hinst       = 0;
      ti.hwnd        = m_wnd;
      ti.lParam      = 0;
      ti.uId         = 0x598406B;
      ti.lpszText    = L"";
      ti.rect.left   = 0;
      ti.rect.right  = 0;
      ti.rect.top    = 0;
      ti.rect.bottom = 0;
      SendMessage( m_tips, TTM_ADDTOOLW, 0, (LPARAM) &ti );
   }

   /* Use default fonts. */
   {
      HFONT font_sys = GetStockFont( DEFAULT_GUI_FONT );
      LOGFONT lf;
      GetObject( font_sys, sizeof( LOGFONT ), &lf );
      // lf.lfQuality = PTB_FONT_QUALITY;
      
      for (int i = 0; i < PTB_NUM_FONTS; ++i)
      {
         lf.lfWeight    = i & PTB_FONT_BOLD ? FW_BOLD : FW_NORMAL;
         lf.lfItalic    = i & PTB_FONT_ITALIC ? TRUE : FALSE;
         lf.lfUnderline = i & PTB_FONT_UNDERLINE ? TRUE : FALSE;
         m_fonts[ i ] = CreateFontIndirect( &lf );
      }

      SelectObject( m_dc, m_fonts[ PTB_FONT_NORMAL ] );
      GetTextMetrics( m_dc, &m_textmetric );
   }
}

PtbInst::~PtbInst( )
{
   while (!m_lines.empty( ))
   {
      delete [] m_lines.back( ).dx;
      m_lines.pop_back( );
   }

   while (!m_elems.empty( ))
   {
      delete m_elems.front( );
      m_elems.pop_front( );
   }

   for (int i = 0; i < PTB_NUM_FONTS; ++i)
      DeleteFont( m_fonts[ i ] ); 
   
   DestroyWindow( m_tips );
   DestroyMenu( m_menu );

   HBITMAP bm = (HBITMAP) GetCurrentObject( m_dc, OBJ_BITMAP );
   RestoreDC( m_dc, -1 );
   DeleteDC( m_dc );
   DeleteBitmap( bm );

   DeleteBrush( m_bgbrush );
}

inline void PtbInst::setBkColor( COLORREF color )
{
   DeleteBrush( m_bgbrush );
   m_bgbrush = CreateSolidBrush( color );
   m_dirty |= DIRTY_DC;
}

void PtbInst::setFont( HFONT font )
{
   if (!font)
      font = GetStockFont( DEFAULT_GUI_FONT );

   LOGFONT lf;
   GetObject( font, sizeof( LOGFONT ), &lf );
   // lf.lfQuality = PTB_FONT_QUALITY;

   for (int i = 0; i < PTB_NUM_FONTS; ++i)
   {
      lf.lfWeight    = i & PTB_FONT_BOLD ? FW_BOLD : FW_NORMAL;
      lf.lfItalic    = i & PTB_FONT_ITALIC ? TRUE : FALSE;
      lf.lfUnderline = i & PTB_FONT_UNDERLINE ? TRUE : FALSE;
      m_fonts[ i ] = CreateFontIndirect( &lf );
   }

   SelectObject( m_dc, m_fonts[ PTB_FONT_NORMAL ] );
   GetTextMetrics( m_dc, &m_textmetric );

   m_dirty |= DIRTY_LINES;
}

inline void PtbInst::setLimit( size_t limit )
{
   if (m_length > limit)
   {
      // Trim off characters from the end.
      erase( limit, -1 );
   }
   m_limit = limit;
   assert( m_length <= limit );
}

void PtbInst::setSel( size_t begin, size_t end )
{
   struct { size_t begin, end; } old = { m_selection.begin, m_selection.end };

   /* Clip the selection offsets. */
   m_selection.begin = min( begin, m_length );
   m_selection.end   = min( end, m_length );

   if (old.begin != m_selection.begin || old.end != m_selection.end)
      m_dirty |= DIRTY_DC;
}

void PtbInst::eraseSel( )
{
   size_t off, len;
   if (m_selection.begin < m_selection.end)
   {
      off = m_selection.begin;
      len = m_selection.end - m_selection.begin;
   }
   else
   {
      off = m_selection.end;
      len = m_selection.begin - m_selection.end;
   }

   erase( off, len );
}

void PtbInst::erase( size_t off, size_t len )
{
   if (!len)
      return;

   ElemIter i( &m_elems, off );
   size_t erase_len = 0;
   for (; erase_len < len && !!i; i.next( ))
   {
      if (i.elem( )->type == PTB_ELEM_TYPE_STR)
      {
         ELEMSTR* elem = (ELEMSTR*) i.elem( );
         size_t n = min( len - erase_len, elem->len - i.m_off );

         // snip snip.
         wstring rh = elem->text.substr( i.m_off + n, wstring::npos );
         elem->text.resize( i.m_off );
         elem->text.append( rh );
         elem->len = elem->text.length( );

         erase_len += n;
      }
      else  // Treat everything else as atomic.
      {
         erase_len += i.elem( )->len;
         i.elem( )->len = 0;
      }

      // Mark zero-length elements as empty.
      if (i.elem( )->len == 0)
         i.elem( )->type = PTB_ELEM_TYPE_EMPTY;
   }

   // Update selection/caret if it was inside or beyond the range.
   if (m_selection.begin > off)
      m_selection.begin -= min( m_selection.begin - off, erase_len );
   if (m_selection.end > off)
      m_selection.end -= min( m_selection.end - off, erase_len );

   assert( m_length >= erase_len );
   m_length -= erase_len;

   m_dirty |= DIRTY_LINES;
}

void PtbInst::eraseAll( )
{
   while (!m_elems.empty( ))
   {
      delete m_elems.front( );
      m_elems.pop_front( );
   }

   m_length = 0;
   m_selection.begin = m_selection.end = 0;
   m_dirty |= DIRTY_LINES;
}

void PtbInst::replaceSel( size_t count, const PTBELEM* elems )
{
   /* Erase the selection. */
   eraseSel( );
   
   /* Insert the items at the caret pos. */
   insert( m_selection.end, count, elems );
}

void PtbInst::insert( size_t off, size_t count, const PTBELEM* elems )
{
   if (!count || m_length >= m_limit)
      return;
       
   const LONG style = GetWindowLong( m_wnd, GWL_STYLE );

   ElemIter caret( &m_elems, off );
   /* If in the middle of an element, split it up. */
   if (caret.m_off)
   {
      // This should only occur with strings.
      assert( caret.elem( )->type == PTB_ELEM_TYPE_STR );

      ELEMSTR* right = (ELEMSTR*) caret.elem( );
      ELEMSTR* left  = new ELEMSTR( );
      left->param    = right->param;
      left->style    = right->style;
      left->color    = right->color;
      left->alt_text = right->alt_text;
      left->text.assign( right->text, 0, caret.m_off );
      left->len = left->text.length( );
      right->text = right->text.substr( caret.m_off, wstring::npos );
      right->len = right->text.length( );

      m_elems.insert( caret.m_elem, left );
      caret.m_off = 0;
   }

   size_t old_length = m_length;
   for (; m_length < m_limit && count > 0; --count, ++elems)
   {
      if (style & PTBS_MULTILINE && elems->type == PTB_ELEM_TYPE_STR)
      {
         /* Convert LF characters into break elements. */
         const wchar_t* sz = elems->text;
         for (const wchar_t* f; 
              m_length < m_limit && (f = wcschr( sz, L'\n' )); 
              sz = f + 1)
         {
            size_t len = min( m_limit - m_length, ((size_t) f - (size_t) sz) >> 1 );
            if (len)
            {
               ELEMSTR* str = new ELEMSTR( );
               str->text.assign( sz, len );
               str->len = str->text.length( );
               str->param = elems->param;
               str->style = elems->style;
               str->color = elems->color;
               if (elems->alt_text) str->alt_text = elems->alt_text;
               
               m_elems.insert( caret.m_elem, str );
               m_length += str->len;
            }
            if (m_length < m_limit)
            {
               ELEMBRK* brk = new ELEMBRK( );
               m_elems.insert( caret.m_elem, brk );
               m_length += 1;
            }
         }

         size_t len = min( m_limit - m_length, wcslen( sz ) );
         if (len)
         {
            ELEMSTR* str = new ELEMSTR( );
            str->text.assign( sz, len );
            str->len = str->text.length( );
            str->param = elems->param;
            str->style = elems->style;
            str->color = elems->color;
            if (elems->alt_text) str->alt_text = elems->alt_text;

            m_elems.insert( caret.m_elem, str );
            m_length += str->len;
         }
      }
      else // Not multiline or not dealing with a string.
      {
         if (elems->type == PTB_ELEM_TYPE_STR)
         {
            size_t len = min( m_limit - m_length, wcslen( elems->text ) );
            if (len)
            {
               ELEMSTR* str = new ELEMSTR( );
               str->text.assign( elems->text, len );
               str->len = str->text.length( );
               str->param = elems->param;
               str->style = elems->style;
               str->color = elems->color;
               if (elems->alt_text) str->alt_text = elems->alt_text;

               m_elems.insert( caret.m_elem, str );
               m_length += str->len;
            }
         }
         else if (elems->type == PTB_ELEM_TYPE_IMG)
         {
            // Should fit or we wouldn't be in here.
            ELEMIMG* img = new ELEMIMG( );
            img->imglist = elems->imglist;
            img->index   = elems->img_index;
            img->style   = elems->style;
            img->param   = elems->param;
            if (elems->alt_text) img->alt_text = elems->alt_text;

            m_elems.insert( caret.m_elem, img );
            m_length += 1;
         }
         else if (style & PTBS_MULTILINE && elems->type == PTB_ELEM_TYPE_BREAK)
         {
            ELEMBRK* brk = new ELEMBRK( );
            m_elems.insert( caret.m_elem, brk );
            m_length += 1;
         }
      }
   }

   // Update selection/caret if it was inside or beyond the range.
   if (m_selection.begin >= off)
      m_selection.begin += m_length - old_length;
   if (m_selection.end >= off)
      m_selection.end += m_length - old_length;

   m_dirty |= DIRTY_LINES;
}

void PtbInst::compact( )
{
   /* Merge adjacent elements with compatible properties,
      split up elements that are too big, remove empty elements,
      recreate elements whose strings have gotten too big. */
   
   list<ELEM*>::iterator i = m_elems.begin( );
   while (i != m_elems.end( ))
   {      
      // Delete empty elements.
      if ((*i)->type == PTB_ELEM_TYPE_EMPTY || (*i)->len == 0)
      {
         delete *i;
         m_elems.erase( i++ );
         continue;
      }

      if ((*i)->type == PTB_ELEM_TYPE_STR)
      {
         ELEMSTR* left = (ELEMSTR*) *i;

         // Split up too big strings.
         if (left->len > MAX_ELEM_LEN)
         {            
            ELEMSTR* right = new ELEMSTR( );
            right->param    = left->param;
            right->style    = left->style;
            right->color    = left->color;
            right->alt_text = left->alt_text;
            right->text.assign( left->text, MAX_ELEM_LEN, wstring::npos );
            right->len = right->text.length( );
 
            /* Rebuild the left element because the wstring probably has
              tons of wasted space reserved now. */
            ELEMSTR* new_left = new ELEMSTR( );
            new_left->param    = left->param;
            new_left->style    = left->style;
            new_left->color    = left->color;
            new_left->alt_text = left->alt_text;
            new_left->text.assign( left->text, 0, MAX_ELEM_LEN );
            new_left->len = new_left->text.length( );
            
            m_elems.insert( i, new_left );
            m_elems.insert( i, right );
            m_elems.erase( i-- );
            delete left;
            continue;
         }
         // Merge adjacent strings.
         if (left->text.length( ) < MAX_ELEM_LEN)
         {
            if (++i != m_elems.end( ) && (*i)->type == PTB_ELEM_TYPE_STR)
            {
               ELEMSTR* right = (ELEMSTR*) *i;
               // Compatible?
               if (left->param == right->param && left->style == right->style
                   && left->alt_text == right->alt_text && left->color == right->color)
               {
                  // Compact.
                  size_t len = min( right->text.length( ), 
                                    MAX_ELEM_LEN - left->text.length( ) );
                  left->text.append( right->text, 0, len );
                  left->len = left->text.length( );

                  right->text = right->text.substr( len, wstring::npos );
                  right->len = right->text.length( );
                  // right gets compacted next iteration.
               }
            }
            continue;
         }
         // Otherwise the element is already maxed out.
      }

      // Anything else slips by unmolested.
      ++i;
   }
   
   // Compact shouldn't affect the selection, but it can invalidate
   // the lines' iterators.
   m_dirty |= DIRTY_LINES;
}

bool PtbInst::charsSize( const ElemIter& first, const size_t len, DIMS* size )
{
   const int ch_pad = GetTextCharacterExtra( m_dc );

   size->w = size->h = 0;
   size_t fit = 0;
   for (ElemIter i = first; !!i && fit < len; i.next( ))
   {
      // Pad before the start if not the first.
      if (fit)
         size->w += ch_pad;

      if (i.elem( )->type == PTB_ELEM_TYPE_STR)
      {
         ELEMSTR* str = (ELEMSTR*) i.elem( );

         SelectObject( m_dc, m_fonts[ STYLEFONT( str->style ) ] );

         SIZE str_size = { 0, 0 };
         const int str_fit = min( str->len - i.m_off, len - fit );
         assert( str_fit > 0 );
         GetTextExtentPointW( m_dc, str->text.c_str( ) + i.m_off, 
                              str_fit, &str_size );
         size->w += str_size.cx;
         size->h = max( size->h, (int) str_size.cy );

         fit += str_fit;
      }
      else if (i.elem( )->type == PTB_ELEM_TYPE_IMG)
      {
         ELEMIMG* img = (ELEMIMG*) i.elem( );

         DIMS img_size = { 0, 0 };
         ImageList_GetIconSize( img->imglist, &img_size.w, &img_size.h );
         
         size->w += img_size.w;
         size->h = max( size->h, img_size.h );

         fit += 1;
      }
      else
      {
         assert( i.elem( )->type == PTB_ELEM_TYPE_BREAK );
         fit += 1;
      }
   }

   return fit == len;
}

size_t PtbInst::charsFit( const ElemIter& first, const size_t len, 
                          const int max_w, DIMS* size )
{
   const int ch_pad = GetTextCharacterExtra( m_dc );

   size->w = size->h = 0;
   size_t fit = 0;
   for (ElemIter i = first; !!i && fit < len && size->w < max_w; i.next( ))
   {
      if (i.elem( )->type == PTB_ELEM_TYPE_STR)
      {
         ELEMSTR* str = (ELEMSTR*) i.elem( );

         SelectObject( m_dc, m_fonts[ STYLEFONT( str->style ) ] );
         
         // Pad before the start if not the first.
         const int _max_w = fit ? max( max_w - ch_pad, 0 ) : max_w;
         const int str_fit_full = min( str->len - i.m_off, len - fit );
         assert( str_fit_full > 0 );

         SIZE str_size = { 0, 0 };
         int str_fit = 0;         
         _GetTextExtentExPointW( m_dc, str->text.c_str( ) + i.m_off, 
                                 str_fit_full, _max_w - size->w, &str_fit, 
                                 NULL, &str_size );
         if (!str_fit)
            break;

         GetTextExtentPointW( m_dc, str->text.c_str( ) + i.m_off,
                              str_fit, &str_size );
         
         assert( size->w + str_size.cx <= max_w );
         
         size->w += str_size.cx;
         size->h = max( size->h, (int) str_size.cy );

         fit += str_fit;

         // Bail now if the whole string didn't fit.
         if (str_fit != str_fit_full)
            break;
      }
      else if (i.elem( )->type == PTB_ELEM_TYPE_IMG)
      {
         ELEMIMG* img = (ELEMIMG*) i.elem( );

         DIMS img_size = { 0, 0 };
         ImageList_GetIconSize( img->imglist, &img_size.w, &img_size.h );

         // Pad before the start if not the first.
         if (fit)
            img_size.w += ch_pad;
         
         if (size->w + (int) img_size.w > max_w)
            break;
         
         size->w += img_size.w;
         size->h = max( size->h, img_size.h );

         fit += 1;
      }
      else
      {
         assert( i.elem( )->type == PTB_ELEM_TYPE_BREAK );
         fit += 1;
      }
   }

   return fit;
}

bool PtbInst::charsDx( const ElemIter& first, const size_t len, int* dx )
{
   const int ch_pad = GetTextCharacterExtra( m_dc );
   int x = 0;
   size_t fit = 0;
   for (ElemIter i = first; !!i && fit < len; i.next( ))
   {
      //if (i.elem( )->type == PTB_ELEM_TYPE_BREAK)
         /* Stop at breaks. */
         //break;

      if (i.elem( )->type == PTB_ELEM_TYPE_STR)
      {
         ELEMSTR* str = (ELEMSTR*) i.elem( );

         // Pad before the start if not the first.
         if (fit) x += ch_pad;

         SelectObject( m_dc, m_fonts[ STYLEFONT( str->style ) ] );

         SIZE str_size = { 0, 0 };
         const int str_fit = min( str->len - i.m_off, len - fit );
         assert( str_fit > 0 );
         _GetTextExtentExPointW( m_dc, str->text.c_str( ) + i.m_off, 
                                 str_fit, 0, NULL, dx, &str_size );

         // Offset the offsets.
         for (int i = 0; i < str_fit; ++i)
            dx[ i ] += x;

         // New origin.
         x = dx[ str_fit - 1 ];
         dx += str_fit;
         fit += str_fit;
      }
      else if (i.elem( )->type == PTB_ELEM_TYPE_IMG)
      {
         ELEMIMG* img = (ELEMIMG*) i.elem( );

         // Pad before the start if not the first.
         if (len) x += ch_pad;

         DIMS img_size = { 0, 0 };
         ImageList_GetIconSize( img->imglist, &img_size.w, &img_size.h );

         // Offset the offset.
         dx[ 0 ] = x + img_size.w;

         // New origin.
         x = dx[ 0 ];
         dx += 1;
         fit += 1;
      }
      else
      {
         assert( i.elem( )->type == PTB_ELEM_TYPE_BREAK );
         fit += 1;
      }
   }

   return fit == len;
}

void PtbInst::buildLines( )
{
   if (!m_clientsize.cx || !m_clientsize.cy)
      return;

   if (m_dirty & DIRTY_LINES) // Lines need updating.
   {
      m_dirty |= DIRTY_VISIBLE_LINES | DIRTY_DC;

      /* Clear out previous lines. */
      for (size_t i = m_top_line; i < m_bottom_line; ++i)
      {
         delete [] m_lines[ i ].dx;
         m_lines[ i ].dx = NULL;
      }
      m_lines.clear( );
      m_top_line = m_bottom_line = 0;

      const int max_w  = max( 0, (int) m_clientsize.cx - LINE_MARGIN * 2 );
      const int ch_pad = GetTextCharacterExtra( m_dc );
      DIMS content_size = { max_w, 0 };

      /* Break up the text into lines. */
      for (ElemIter i( &m_elems ); true; )
      {
         DIMS     line_size = { 0, m_textmetric.tmHeight };
         size_t   line_len  = 0;
         bool     broken = false;
         
         /* Fill up this line. */
         for (ElemIter j = i; !!j && line_size.w < max_w; )
         {
            if (j.elem( )->type == PTB_ELEM_TYPE_STR)
            {
               if (iswspace( j.elemStr( )->text[ j.m_off ] ))
               {
                  /* Add whitespace one by one. */
                  DIMS space_size = { 0, 0 };
                  charsSize( j, 1, &space_size );
                  // Pad.
                  if (line_len) space_size.w += ch_pad;
                  // Won't fit?
                  if (line_size.w + space_size.w > max_w) break;

                  line_size.w += space_size.w;
                  line_size.h = max( line_size.h, space_size.h );
                  line_len += 1;
                  j.nextChar( );
               }
               else
               {
                  /* Collect the entire word. */
                  const ElemIter word = j;
                  size_t word_len     = 0;
                  do 
                  {
                     ++word_len;
                     j.nextChar( );

                  } while (!!j && j.elem( )->type == PTB_ELEM_TYPE_STR
                           && !iswspace( j.elemStr( )->text[ j.m_off ] ));

                  DIMS word_size = { 0, 0 };
                  charsSize( word, word_len, &word_size );

                  // Too big for any line?
                  if (word_size.w > max_w)
                  {
                     // Just treat the word like whitespace.
                     word_len = charsFit( word, word_len, max_w - line_size.w, 
                                          &word_size );
                     
                     line_size.w += word_size.w;
                     line_size.h = max( line_size.h, word_size.h );
                     line_len += word_len;
                     break; // Remainder of word will be picked up by next line.
                  }

                  // Pad.
                  if (line_len) word_size.w += ch_pad;
                  // Won't fit?
                  if (line_size.w + word_size.w > max_w) break;

                  line_size.w += word_size.w;
                  line_size.h = max( line_size.h, word_size.h );
                  line_len += word_len;
               }
            }
            else if (j.elem( )->type == PTB_ELEM_TYPE_IMG)
            {
               /* Add images one by one. */
               DIMS img_size = { 0, 0 };
               charsSize( j, 1, &img_size );
               // Pad.
               if (line_len) img_size.w += ch_pad;
               // Won't fit?
               if (line_size.w + img_size.w > max_w) break;

               line_size.w += img_size.w;
               line_size.h = max( line_size.h, img_size.h );
               line_len += 1;
               j.nextChar( );
            }
            else
            {
               assert( j.elem( )->type == PTB_ELEM_TYPE_BREAK );
               broken = true;
               break;
            }
         } // for (ElemIter j = i; !!j && line_size.w < max_w; )

         /* Make sure at least one character is on a line. */
         if (!!i && line_len == 0 && !broken)
         {
            if (i.elem( )->type == PTB_ELEM_TYPE_STR)
            {
               DIMS ch_size = { 0, 0 };
               charsSize( i, 1, &ch_size );
               
               line_size.w += ch_size.w;
               line_size.h = max( line_size.h, ch_size.h );
               line_len += 1;
            }
            else if (i.elem( )->type == PTB_ELEM_TYPE_IMG)
            {
               DIMS img_size = { 0, 0 };
               charsSize( i, 1, &img_size );

               line_size.w += img_size.w;
               line_size.h = max( line_size.h, img_size.h );
               line_len += 1;
            }
         }

         // Just record the dimensons in the rect for now.
         RECT line_rect = { 0, 0, line_size.w, line_size.h };
         m_lines.push_back( LINE( i, i.toCharOffset( ), line_len, !broken, line_rect ) );
         i.nextChars( line_len );

         if (!i) // End of elements.
         {
            m_lines.back( ).is_cont = false;
            break;
         }

         // Breaks aren't counted, so advance one more if the line ends in one.
         if (broken)
            i.next( );

         // Keep track of content width.
         content_size.w = max( content_size.w, line_size.w );

      } // for (ElemIter i( &m_elems ); !!i; )

      const LONG wstyle = GetWindowLong( m_wnd, GWL_STYLE );
      content_size.w += LINE_MARGIN * 2;

      /* Pad and justify the lines */
      for (vector<LINE>::iterator i = m_lines.begin( );
           i != m_lines.end( ); ++i)
      {
         if (wstyle & PTBS_CENTER)
         {
            i->rect.left   = (content_size.w - i->rect.right) / 2;  // Margins already factored in.
            i->rect.top    = content_size.h;
            i->rect.right  = i->rect.left + i->rect.right;
            i->rect.bottom = i->rect.top + i->rect.bottom;
         }
         else if (wstyle & PTBS_RIGHT)
         {
            i->rect.left   = content_size.w - LINE_MARGIN - i->rect.right;
            i->rect.top    = content_size.h;
            i->rect.right  = i->rect.left + i->rect.right;
            i->rect.bottom = i->rect.top + i->rect.bottom;
         }
         else
         { 
            i->rect.left   = LINE_MARGIN;
            i->rect.top    = content_size.h;
            i->rect.right  = i->rect.left + i->rect.right;
            i->rect.bottom = i->rect.top + i->rect.bottom;
         }

         content_size.h = i->rect.bottom + LINE_PADDING;
      }

      // Remember the content size.
      m_content_size = content_size;

      /* Adjust horizontal scrollbar. */
      {
         SCROLLINFO old_scroll;
         old_scroll.cbSize = sizeof( old_scroll );
         old_scroll.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
         GetScrollInfo( m_wnd, SB_HORZ, &old_scroll );

         SCROLLINFO scroll;
         scroll.cbSize = sizeof( scroll );
         scroll.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
         scroll.nMin = 0;
         scroll.nMax = content_size.w;
         scroll.nPage = m_clientsize.cx + 1;
         // If the scrollbar is at the end of the track, keep it there. 
         if (old_scroll.nPos >= old_scroll.nMax - max( (int) old_scroll.nPage, (int) 0 ))
            scroll.nPos = max( scroll.nMax - (int) scroll.nPage, (int) 0 );
         // If the range is shrinking, compensate for the change to keep the same line visible.
         else if (old_scroll.nMax > scroll.nMax)
            scroll.nPos = max( old_scroll.nPos - (int) (old_scroll.nMax - scroll.nMax), (int) 0 );
         else
            scroll.nPos = old_scroll.nPos;
         SetScrollInfo( m_wnd, SB_HORZ, &scroll, TRUE );

         GetScrollInfo( m_wnd, SB_HORZ, &scroll );
         m_scroll.x = scroll.nPos;
      }
      /* Adjust vertical scrollbar. */
      {
         SCROLLINFO old_scroll;
         old_scroll.cbSize = sizeof( old_scroll );
         old_scroll.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
         GetScrollInfo( m_wnd, SB_VERT, &old_scroll );

         SCROLLINFO scroll;
         scroll.cbSize = sizeof( scroll );
         scroll.fMask = SIF_RANGE | SIF_POS | SIF_PAGE;
         scroll.nMin = 0;
         scroll.nMax = content_size.h;
         scroll.nPage = m_clientsize.cy + 1;
         // If the scrollbar is at the end of the track, keep it there. 
         if (old_scroll.nPos >= old_scroll.nMax - max( (int) old_scroll.nPage, (int) 0 ))
            scroll.nPos = max( scroll.nMax - (int) scroll.nPage, (int) 0 );
         // If the range is shrinking, compensate for the change to keep the same line visible.
         else if (old_scroll.nMax > scroll.nMax)
            scroll.nPos = max( old_scroll.nPos - (int) (old_scroll.nMax - scroll.nMax), (int) 0 );
         else
            scroll.nPos = old_scroll.nPos;
         SetScrollInfo( m_wnd, SB_VERT, &scroll, TRUE );

         GetScrollInfo( m_wnd, SB_VERT, &scroll );
         m_scroll.y = scroll.nPos;
      }

      m_dirty &= (~DIRTY_LINES);
   }

   if (m_dirty & DIRTY_VISIBLE_LINES) // Visible lines need updating.
   {
      m_dirty |= DIRTY_DC;
      
      /* Find the first visible line. */
      size_t top_line = 0;
      if (m_lines.size( ))
      {
         // Find any visible line.
         size_t left = 0, right = m_lines.size( ) - 1, mid = 0;
         while (left != right)
         {
            mid = left + ((right - left) >> 1);

            if (m_lines[ mid ].rect.bottom - m_scroll.y <= 0)
               left = mid + 1;
            else if (m_lines[ mid ].rect.top - m_scroll.y > m_clientsize.cy)
               right = mid;
            else
               left = right = mid;
         }

         if (m_lines[ left ].rect.bottom - m_scroll.y > 0 &&
             m_lines[ left ].rect.top - m_scroll.y <= m_clientsize.cy)
             // Find the first.
            for (top_line = left; 
                 m_lines[ top_line ].rect.top - m_scroll.y > 0; --top_line) ;
         else
            top_line = m_lines.size( );
      }

      /* Free dx arrays from lines that are no longer visible. */
      for (size_t i = m_top_line; i < m_bottom_line; ++i)
      {
         if (m_lines[ i ].rect.bottom - m_scroll.y <= 0 ||
             m_lines[ i ].rect.top - m_scroll.y > m_clientsize.cy)
         {
            delete [] m_lines[ i ].dx;
            m_lines[ i ].dx = NULL;
         }
      }

      /* Build the visible lines, using existing ones if possible. */
      m_top_line = m_bottom_line = top_line;      
      for (size_t i = top_line; i < m_lines.size( ); ++i, ++m_bottom_line)
      {
         if (m_lines[ i ].rect.top - m_scroll.y < m_clientsize.cy)
         {
            if (!m_lines[ i ].dx)
            {
               int* dx = new int[ m_lines[ i ].len ];
               bool r = charsDx( m_lines[ i ].iter, m_lines[ i ].len, dx );
               assert( r );

               // Leave character offsets as relative from origin.
               m_lines[ i ].dx = dx;
            }
         }
         else
            break;
      }

      m_dirty &= (~DIRTY_VISIBLE_LINES);
   }
}

void PtbInst::paint( HDC dc )
{
   if ((m_clientsize.cx && m_clientsize.cy) && m_dirty & DIRTY_DC)
   {
      /* Resize the DC if necessary. */
      BITMAP bm;
      GetObject( GetCurrentObject( m_dc, OBJ_BITMAP ), sizeof( BITMAP ), &bm );
      // Don't let it go to zero.
      if (bm.bmWidth != m_clientsize.cx || bm.bmHeight != m_clientsize.cy)
      {
         
         DeleteObject( SelectObject( m_dc, 
                                     CreateCompatibleBitmap( m_dc, max( (LONG) 1, m_clientsize.cx ), 
                                                             max( (LONG) 1, m_clientsize.cy ) ) ) );
      }

      /* Clear the background. */
      {
         RECT r = { 0, 0, m_clientsize.cx, m_clientsize.cy };
         FillRect( m_dc, &r, m_bgbrush );
      }

      const size_t sel_beg = m_selection.begin < m_selection.end ? 
                             m_selection.begin : m_selection.end;
      const size_t sel_end = m_selection.begin < m_selection.end ? 
                             m_selection.end : m_selection.begin;
      const COLORREF color_hilite = GetSysColor( COLOR_HIGHLIGHTTEXT );

      SetBkColor( m_dc, GetSysColor( COLOR_HIGHLIGHT ) );
      SetTextAlign( m_dc, TA_BOTTOM | TA_LEFT );

      /* Paint each visible line. */
      for (size_t i = m_top_line; i < m_bottom_line; ++i)
      {
         if (m_lines[ i ].len == 0)
            continue;

         assert( !!m_lines[ i ].iter );
         assert( m_lines[ i ].dx );

         const LINE& line = m_lines[ i ];
         ElemIter e = m_lines[ i ].iter;
         size_t j = 0;

         while (j < line.len)
         {
            if (e.elem( )->type == PTB_ELEM_TYPE_STR)
            {
               // Clip to the end of the element.
               size_t len = min( e.elemStr( )->len - e.m_off, line.len - j );
               // Clip to the end of the selection.
               if (sel_end > line.off + j)
                  len = min( len, sel_end - (line.off + j) );
               // Clip to the start of the selection.
               if (sel_beg > line.off + j)
                  len = min( len, sel_beg - (line.off + j) );

               if (sel_beg <= line.off + j && sel_end > line.off + j)
               {
                  /* Inside a selection. */
                  // Set selected styles.
                  SetBkMode( m_dc, OPAQUE );
                  SetTextColor( m_dc, color_hilite );
               }
               else
               {
                  // set unselected drawing styles.
                  SetBkMode( m_dc, TRANSPARENT );
                  SetTextColor( m_dc, e.elemStr( )->color );
               }

               // Set the font.
               SelectObject( m_dc, m_fonts[ STYLEFONT( e.elemStr( )->style ) ] );
               // Draw.
               const int x = j ? line.rect.left + line.dx[ j - 1 ] - m_scroll.x : 
                                 line.rect.left - m_scroll.x;
               ExtTextOutW( m_dc, x, 
                            line.rect.bottom - m_scroll.y, 
                            0, NULL, e.elemStr( )->text.c_str( ) + e.m_off, 
                            len, NULL ); // Can't use line.dx here because it
                                         // wants an array of separators between
                                         // successive characters >(

               e.nextChars( len );
               j += len;
            }
            else if (e.elem( )->type == PTB_ELEM_TYPE_IMG)
            {
               // Clip to the end of the element.
               size_t len = min( e.elemImg( )->len, line.len );

               UINT style = ILD_NORMAL;
               if (line.off + j >= sel_beg && line.off + j < sel_end)
               {
                  /* Inside a selection. */
                  // Clip to the end of the line.
                  len = min( sel_end - (line.off + j), len );
                  // Set selected styles.
                  style |= ILD_SELECTED;
               }

               // Need to make sure the image is centered vertically.
               int w = 0, h = 0; 
               ImageList_GetIconSize( e.elemImg( )->imglist, &w, &h );

               // Draw.
               const int x = j ? line.rect.left + line.dx[ j - 1 ] - m_scroll.x : 
                                 line.rect.left - m_scroll.x;
               ImageList_Draw( e.elemImg( )->imglist, e.elemImg( )->index, m_dc, 
                               x, 
                               line.rect.top - m_scroll.y + (line.rect.bottom - line.rect.top - h) / 2,
                               style );
               
               e.nextChars( len );
               j += len;
            }
            else
            {
               // assert( false );
               e.next( );
            }

         } // while (j < line.len)

      } // for (size_t i = m_top_line; i < m_bottom_line; ++i)

      m_dirty &= (~DIRTY_DC);
   } // if (m_dirty & DIRTY_DC)

   /* Blt the DC to the front buffer. */
   BitBlt( dc, 0, 0, m_clientsize.cx, m_clientsize.cy, m_dc, 0, 0, SRCCOPY );

   /* Draw the caret. */
}

void PtbInst::updateCaret( )
{
   /* Update the system caret position if we have focus. */
   if (GetFocus( ) == m_wnd)
   {
      /* Find the line that the caret is on. */
      size_t i = lineFromOffset( m_selection.end );

      // Need to clip the caret ourself.
      RECT caret = { 0, 0, 0, 0 };

      if (i < m_lines.size( ))
      {
         if (m_selection.end <= m_lines[ i ].off)
         {
            // Place at the start of the line.
            caret.left   = m_lines[ i ].rect.left;
            caret.top    = m_lines[ i ].rect.top;
            caret.right  = caret.left + GetSystemMetrics( SM_CXBORDER );
            caret.bottom = caret.top + m_lines[ i ].rect.bottom - m_lines[ i ].rect.top;
         }
         else
         {
            if (m_lines[ i ].dx)
            {
               // Place before the character.
               caret.left   = m_lines[ i ].rect.left + 
                              m_lines[ i ].dx[ m_selection.end - m_lines[ i ].off - 1 ];
               caret.top    = m_lines[ i ].rect.top;
               caret.right  = caret.left + GetSystemMetrics( SM_CXBORDER );
               caret.bottom = caret.top + m_lines[ i ].rect.bottom - m_lines[ i ].rect.top;
            }
            else
            {
               // Line isn't even visible, so it won't matter.
               caret.left   = 0;
               caret.top    = 0;
               caret.right  = 0;
               caret.bottom = 0;
            }
         }
      }
      else  // Wasn't on any line.
      {
         /* Stick it in at the end. */
         assert( m_lines.size( ) );

         i = m_lines.size( ) - 1;
         caret.left   = m_lines[ i ].rect.right;
         caret.top    = m_lines[ i ].rect.top;
         caret.right  = caret.left + GetSystemMetrics( SM_CXBORDER );
         caret.bottom = caret.top + m_lines[ i ].rect.bottom - m_lines[ i ].rect.top;
      }

      /* Clip the caret. */
      caret.left   -= m_scroll.x;
      caret.right  -= m_scroll.x;
      caret.top    -= m_scroll.y;
      caret.bottom -= m_scroll.y;

      RECT client_r;
      GetClientRect( m_wnd, &client_r );
      
      IntersectRect( &caret, &caret, &client_r );

      if (!(caret.right - caret.left) || !(caret.bottom - caret.top))
      {
         // Not visible.
         HideCaret( m_wnd );
      }
      else
      {
         CreateCaret( m_wnd, (HBITMAP) 0, 
                      caret.right - caret.left, caret.bottom - caret.top );
         SetCaretPos( caret.left, caret.top );
         ShowCaret( m_wnd );
      }
   }
}

PtbInst::ElemIter PtbInst::elemFromPoint( const POINT& _pt )
{
   /* The point must be both within the x and y extents of a line. */
   POINT pt = { _pt.x + m_scroll.x, _pt.y + m_scroll.y };

   for (size_t i = m_top_line; i < m_bottom_line; ++i)
   {
      if (m_lines[ i ].rect.bottom > pt.y &&
          m_lines[ i ].rect.top <= pt.y)
      {
         if (m_lines[ i ].rect.left <= pt.x &&
             m_lines[ i ].rect.right > pt.x)
         {
            assert( m_lines[ i ].dx );

            pt.x -= m_lines[ i ].rect.left;
            for (size_t j = 0; j < m_lines[ i ].len; ++j)
            {
               if (pt.x < m_lines[ i ].dx[ j ])
               {
                  ElemIter iter = m_lines[ i ].iter;
                  iter.nextChars( j );
                  return iter;
               }
            }
         }
         break;
      }
   }

   return ElemIter( &m_elems, -1 );
}

size_t PtbInst::offsetFromPoint( const POINT& _pt )
{
   /* The point must be within the y extents of a line. */
   POINT pt = { _pt.x + m_scroll.x, _pt.y + m_scroll.y };

   for (size_t i = m_top_line; i < m_bottom_line; ++i)
   {
      if (m_lines[ i ].rect.bottom > pt.y
          /* && m_lines[ i ].rect.top <= pt.y */ )
      {
         if (m_lines[ i ].rect.left <= pt.x)
         {
            if (m_lines[ i ].rect.right > pt.x) // Totally inside the line.
            {
               assert( m_lines[ i ].dx );

               pt.x -= m_lines[ i ].rect.left;
               // Use half-widths.
               int prev = 0;
               for (size_t j = 0; j < m_lines[ i ].len; ++j)
               {
                  if (pt.x < prev + ((m_lines[ i ].dx[ j ] - prev) >> 1))
                  {
                     return m_lines[ i ].off + j;
                  }
                  prev = m_lines[ i ].dx[ j ];
               }

               return m_lines[ i ].off + m_lines[ i ].len;
            }
            else // To the right of the line.
            {
               return m_lines[ i ].off + m_lines[ i ].len;
            }
         }
         else // To the left of the line.
         {
            return m_lines[ i ].off;
         }

         break;
      }
   }

   return -1;
}

void PtbInst::selectWord( )
{
   ElemIter i( &m_elems, m_selection.end );
   
   size_t begin = m_selection.end, end = m_selection.end;

   // The caret is at the end or on a break, move it back one so it's actually
   // on an element.
   if (!i || i.elem( )->type == PTB_ELEM_TYPE_BREAK)
   {
      --begin, --end;
      if (!i.prevChar( ))
         return;
   }

   if (i.elem( )->type == PTB_ELEM_TYPE_STR)
   {
      /* Look left and right for the end of the word. */
      if (iswspace( i.elemStr( )->text[ i.m_off ] ))
      {
         // Select whitespace.
         ElemIter left = i;
         for (ElemIter j = i; 
             j.elem( )->type == PTB_ELEM_TYPE_STR; j.prevChar( ))
         {
            if (!iswspace( j.elemStr( )->text[ j.m_off ] ))
               break;

            left = j;
            if (j.isFirst( ))
               break;
         }

         ElemIter right = i;
         for (ElemIter j = i; 
              !!j && j.elem( )->type == PTB_ELEM_TYPE_STR && 
              iswspace( j.elemStr( )->text[ j.m_off ] ); j.nextChar( ))
         {
            right = j;
         }
         right.nextChar( );

         begin = left.toCharOffset( );
         end   = right.toCharOffset( );
      }
      else
      {
         // Select non-whitespace.
         ElemIter left = i;
         for (ElemIter j = i; 
            j.elem( )->type == PTB_ELEM_TYPE_STR; j.prevChar( ))
         {
            if (iswspace( j.elemStr( )->text[ j.m_off ] ))
               break;

            left = j;
            if (j.isFirst( ))
               break;
         }

         ElemIter right = i;
         for (ElemIter j = i; 
              !!j && j.elem( )->type == PTB_ELEM_TYPE_STR && 
              !iswspace( j.elemStr( )->text[ j.m_off ] ); j.nextChar( ))
         {
            right = j;
         }
         right.nextChar( );

         begin = left.toCharOffset( );
         end   = right.toCharOffset( );
      }
   }
   else if (i.elem( )->type == PTB_ELEM_TYPE_IMG)
   {
      // Images are atomic.
      end   = begin + 1;
   }

   setSel( begin, end );
}

long PtbInst::areaFromPoint( const POINT& pt )
{
   /* The point must be within the y extents of a line. */
   
   long flags = 0;
   
   if (pt.x < 0)
      flags |= AREA_LEFT;
   else if (pt.x >= m_clientsize.cx)
      flags |= AREA_RIGHT;
   if (pt.y < 0)
      flags |= AREA_ABOVE;
   else if (pt.y >= m_clientsize.cy)
      flags |= AREA_BELOW;

   return flags;
}

void PtbInst::toString( size_t off, size_t len, wstring* out, 
                        bool crlf /* = false */, bool alts /* = false */ )
{
   out->clear( );

   const wchar_t* const sz_break = crlf ? L"\r\n" : L"\n";

   for (ElemIter i( &m_elems, off ); !!i && len; i.next( ))
   {
      if (i.elem( )->type == PTB_ELEM_TYPE_STR)
      {
         size_t n = min( len, i.elemStr( )->len - i.m_off );
         out->append( i.elemStr( )->text, i.m_off, n );
         len -= n;
      }
      else if (i.elem( )->type == PTB_ELEM_TYPE_IMG)
      {
         if (alts && i.elemImg( )->alt_text.length( ))
         {
            // Use alternate text for images.
            out->append( i.elemImg( )->alt_text );
         }
         else
         {
            out->append( L" " );
         }
         len -= i.elem( )->len;
      }
      else if (i.elem( )->type == PTB_ELEM_TYPE_BREAK)
      {
         out->append( sz_break );
         len -= i.elem( )->len;
      }
   }
}

size_t PtbInst::toStringLength( size_t off, size_t len, bool crlf /* = false */ )
{
   size_t str_len = 0;
   const size_t break_len = crlf ? 2 : 1;

   for (ElemIter i( &m_elems, off ); !!i && len; i.next( ))
   {
      if (i.elem( )->type == PTB_ELEM_TYPE_STR)
      {
         size_t n = min( len, i.elemStr( )->len - i.m_off );
         len -= n;
         str_len += n;
      }
      else if (i.elem( )->type == PTB_ELEM_TYPE_IMG)
      {
         len -= i.elem( )->len;
         str_len += i.elem( )->len;
      }
      else if (i.elem( )->type == PTB_ELEM_TYPE_BREAK)
      {
         len -= i.elem( )->len;
         str_len += break_len;
      }
   }

   return str_len;
}

inline size_t PtbInst::lineFromOffset( size_t off )
{
   for (size_t i = 0; i < m_lines.size( ); ++i)
   {
      if (off <= m_lines[ i ].off + m_lines[ i ].len)
      {
         // Bias the end caret to the next line down if the next line
         // is a continuation.
         if (m_lines[ i ].is_cont &&
             off == m_lines[ i ].off + m_lines[ i ].len)
            ++i;
         return i;
      }
   }

   return m_lines.size( ) - 1;
}

void PtbInst::ensureVisible( size_t off )
{
   size_t line_n = lineFromOffset( off );
   if (line_n == -1)
      return;

   const LINE& line = m_lines[ line_n ];

   POINT scroll_new = m_scroll;

   if (line.rect.bottom - m_scroll.y > m_clientsize.cy)
   {
      // Scroll down.
      scroll_new.y += (line.rect.bottom - m_scroll.y) - m_clientsize.cy;
   }
   else if (line.rect.top - m_scroll.y < 0)
   {
      // Scroll up.
      scroll_new.y -= 0 - (line.rect.top - m_scroll.y);
   }

   if (line.rect.right - m_scroll.x > m_clientsize.cx)
   {
      // Scroll right.
      scroll_new.x += (line.rect.right - m_scroll.x) - m_clientsize.cx;
   }
   else if (line.rect.left - m_scroll.x < 0)
   {
      // Scroll left.
      scroll_new.x -= 0 - (line.rect.left - m_scroll.x);
   }

   if (scroll_new.x == m_scroll.x && scroll_new.y == m_scroll.y)
      return;

   SCROLLINFO si;
   si.cbSize = sizeof( si );
   si.fMask = SIF_POS;
   si.nPos  = scroll_new.y;
   SetScrollInfo( m_wnd, SB_VERT, &si, TRUE );

   si.fMask = SIF_ALL;
   GetScrollInfo( m_wnd, SB_VERT, &si );
   m_scroll.y = si.nPos;
   
   si.fMask = SIF_POS;
   si.nPos  = scroll_new.x;
   SetScrollInfo( m_wnd, SB_HORZ, &si, TRUE );

   si.fMask = SIF_ALL;
   GetScrollInfo( m_wnd, SB_HORZ, &si );
   m_scroll.x = si.nPos;

   m_dirty |= DIRTY_VISIBLE_LINES;
}

size_t PtbInst::findHome( size_t off )
{
   size_t line = lineFromOffset( off );
   if (line == -1)
      return 0;

   /* Find the paragraph start. */
   for (; line; --line)
   {
      if (!m_lines[ line - 1 ].is_cont)
         break;
   }

   return m_lines[ line ].off;
}

size_t PtbInst::findEnd( size_t off )
{
   size_t line = lineFromOffset( off );
   if (line == -1)
      return -1;

   /* Find the paragraph end. */
   for (; line + 1 < m_lines.size( ); ++line)
   {
      if (!m_lines[ line ].is_cont)
         break;
   }

   return m_lines[ line ].off + m_lines[ line ].len;
}

void PtbInst::trimLines( size_t max_lines )
{
   if (max_lines >= m_lines.size( ))
      return;
   
   if (!max_lines)
      eraseAll( );
   else
   {
      /* Trim off the top. */
      size_t new_top = m_lines.size( ) - max_lines;
      erase( 0, m_lines[ new_top ].off );
   }
}

void PtbInst::showTip( const wstring& tip, int x, int y )
{
   TOOLINFOW ti;
   ti.cbSize      = sizeof( ti );
   ti.uFlags      = TTF_TRACK | TTF_ABSOLUTE | TTF_TRANSPARENT;
   ti.hinst       = 0;
   ti.hwnd        = m_wnd;
   ti.lParam      = 0;
   ti.uId         = 0x598406B;
   ti.lpszText    = _wcsdup( tip.c_str( ) );
   ti.rect.left   = 0;
   ti.rect.right  = 0;
   ti.rect.top    = 0;
   ti.rect.bottom = 0;

   SendMessage( m_tips, TTM_SETTOOLINFOW, 0, (LPARAM) &ti );

   free( ti.lpszText );

   POINT scr_pt = { x, y };
   ClientToScreen( m_wnd, &scr_pt );

   SendMessage( m_tips, TTM_TRACKPOSITION, 0, MAKELONG( scr_pt.x, scr_pt.y ) );
   SendMessage( m_tips, TTM_TRACKACTIVATE, TRUE, (LPARAM) &ti );
}

void PtbInst::hideTip( )
{
   SendMessage( m_tips, TTM_TRACKACTIVATE, FALSE, 0 );
}

LRESULT CALLBACK PtbInst::wnd_proc( HWND wnd, UINT msg, 
                                    WPARAM wparam, LPARAM lparam )
{
#define _INST_PTR( ) ((PtbInst*) GetWindowLongPtr( wnd, 0 ))

   switch (msg)
   {
   case WM_CREATE:
      /* Set the instance ptr. */
      SetWindowLongPtr( wnd, 0, (LONG_PTR) new PtbInst( wnd ) );
      return 0;

   case WM_DESTROY:
      /* Delete the instance ptr. */
      delete _INST_PTR( );
      SetWindowLongPtr( wnd, 0, NULL );
      break;

   case WM_ERASEBKGND:
      {
         return 0;
      }

   case WM_PAINT:
      {
         PAINTSTRUCT ps;
         _INST_PTR( )->paint( BeginPaint( wnd, &ps ) );
         EndPaint( wnd, &ps );
         return 0;
      }

   case WM_SIZE:
      {
         PtbInst* inst = _INST_PTR( );

         // Only remember non-zero sizes.
         if (LOWORD( lparam ) && HIWORD( lparam ))
         {
            inst->m_clientsize.cx = LOWORD( lparam );
            inst->m_clientsize.cy = HIWORD( lparam );
            inst->m_dirty |= DIRTY_LINES;
            inst->buildLines( );
            inst->updateCaret( );
            InvalidateRect( wnd, NULL, FALSE );
         }
      }
      break;

   case WM_VSCROLL:
   case WM_HSCROLL:
      {
         PtbInst* inst = _INST_PTR( );

         SCROLLINFO scroll;
         scroll.cbSize = sizeof( scroll );
         scroll.fMask = SIF_ALL;
         GetScrollInfo( wnd, msg == WM_VSCROLL ? SB_VERT : SB_HORZ, &scroll );

         switch (LOWORD( wparam ))
         {
         case SB_TOP:
         //case SB_LEFT
            scroll.nPos = scroll.nMin;
            break;

         case SB_BOTTOM:
         //case SB_RIGHT:
            scroll.nPos = scroll.nMax;
            break;

         case SB_LINEUP:
         //case SB_LINELEFT:
            scroll.nPos -= inst->m_textmetric.tmHeight;
            break;

         case SB_LINEDOWN:
         //case SB_LINERIGHT:
            scroll.nPos += inst->m_textmetric.tmHeight;
            break;

         case SB_PAGEUP:
         //case SB_PAGELEFT:
            scroll.nPos -= scroll.nPage;
            break;

         case SB_PAGEDOWN:
         //case SB_PAGERIGHT:
            scroll.nPos += scroll.nPage;
            break;

         case SB_THUMBTRACK:
            scroll.nPos = scroll.nTrackPos;
            break;
         }

         SetScrollInfo( wnd, msg == WM_VSCROLL ? SB_VERT : SB_HORZ, &scroll, TRUE );
         GetScrollInfo( wnd, msg == WM_VSCROLL ? SB_VERT : SB_HORZ, &scroll );
         if (msg == WM_VSCROLL)
            inst->m_scroll.y = scroll.nPos;
         else
            inst->m_scroll.x = scroll.nPos;

         inst->m_dirty |= DIRTY_VISIBLE_LINES;
         inst->buildLines( );
         inst->updateCaret( );
         InvalidateRect( wnd, NULL, FALSE );
         return 0;
      }
      break;

   case WM_SETFOCUS:
      {
         _INST_PTR( )->updateCaret( );
         return 0;
      }
      
   case WM_KILLFOCUS:
      {
         DestroyCaret( );
         return 0;
      }

   //////////////////////////////////////////////////////////////////////////

   case WM_MOUSEHOVER:
      {
         PtbInst* inst = _INST_PTR( );
         POINT pt = { GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) };
         ElemIter elem = inst->elemFromPoint( pt );

         if (!!elem)
         {
            /* Display the alt text. */
            if (elem.elem( )->type == PTB_ELEM_TYPE_STR && 
                elem.elemStr( )->alt_text.length( ))
            {
               inst->showTip( elem.elemStr( )->alt_text, pt.x + 16, pt.y + 8 );
            }
            else if (elem.elem( )->type == PTB_ELEM_TYPE_IMG && 
                     elem.elemImg( )->alt_text.length( ))
            {
               inst->showTip( elem.elemImg( )->alt_text, pt.x + 16, pt.y + 8 );
            }
         }

         /* Ask for leave events. */
         {
            TRACKMOUSEEVENT track;
            track.cbSize = sizeof( track );
            track.dwFlags = TME_LEAVE;
            track.hwndTrack = wnd;
            TrackMouseEvent( &track );
         }
      }
      return 0;

   case WM_MOUSELEAVE:
      _INST_PTR( )->hideTip( );
      return 0;

   case WM_LBUTTONDBLCLK:
      {
         POINT pt = { GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) };
         PtbInst* inst = _INST_PTR( );
         inst->hideTip( );

         if (GetFocus( ) != wnd)
            SetFocus( wnd );

         size_t off = inst->offsetFromPoint( pt );
         inst->setSel( off, off );
         inst->selectWord( );
         _INST_PTR( )->updateCaret( );
         InvalidateRect( wnd, NULL, FALSE );

         return 0;
      }

   case WM_LBUTTONDOWN:
      {
         POINT pt = { GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) };
         PtbInst* inst = _INST_PTR( );
         inst->hideTip( );
         
         ElemIter e = inst->elemFromPoint( pt );
         if (!!e && 
             ((e.elem( )->type == PTB_ELEM_TYPE_IMG && e.elemImg( )->style & PTB_ELEM_STYLE_LINK) ||
             (e.elem( )->type == PTB_ELEM_TYPE_STR && e.elemStr( )->style & PTB_ELEM_STYLE_LINK)))
         {
            /* Send a notification. */
            NMPTBELEMACTION nmtv;
            nmtv.hdr.code     = PTBN_ELEMACTION;
            nmtv.hdr.hwndFrom = wnd;
            nmtv.hdr.idFrom   = (UINT_PTR) GetMenu( wnd );
            nmtv.action       = PTB_ELEMACTION_CLICK;
            if (e.elem( )->type == PTB_ELEM_TYPE_STR)
               nmtv.param        = e.elemStr( )->param;
            else
               nmtv.param        = e.elemImg( )->param;
            nmtv.offset       = e.toCharOffset( );

            /* If the return value is nonzero, stop here. */

            if (SendMessage( GetParent( wnd ), WM_NOTIFY, 
                             (WPARAM) nmtv.hdr.idFrom, (LPARAM) &nmtv ))
            {
               return 0;
            }
         }
         
         if (GetFocus( ) != wnd)
            SetFocus( wnd );

         size_t off = inst->offsetFromPoint( pt );
         inst->setSel( off, off );
         inst->updateCaret( );

         /* Start dragging. */
         inst->m_dragging = true;
         SetCapture( wnd );
         SetTimer( wnd, SCROLL_TIMER, SCROLL_RESOLUTION, NULL );
         
         InvalidateRect( wnd, NULL, FALSE );

         return 0;
      }
   
   case WM_RBUTTONDOWN:
      {
         POINT pt = { GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) };
         PtbInst* inst = _INST_PTR( );
         inst->hideTip( );
         
         ElemIter e = inst->elemFromPoint( pt );
         if (!!e && 
            ((e.elem( )->type == PTB_ELEM_TYPE_IMG && e.elemImg( )->style & PTB_ELEM_STYLE_LINK) ||
            (e.elem( )->type == PTB_ELEM_TYPE_STR && e.elemStr( )->style & PTB_ELEM_STYLE_LINK)))
         {
            /* Send a notification. */
            NMPTBELEMACTION nmtv;
            nmtv.hdr.code     = PTBN_ELEMACTION;
            nmtv.hdr.hwndFrom = wnd;
            nmtv.hdr.idFrom   = (UINT_PTR) GetMenu( wnd );
            nmtv.action       = PTB_ELEMACTION_RCLICK;
            if (e.elem( )->type == PTB_ELEM_TYPE_STR)
               nmtv.param        = e.elemStr( )->param;
            else
               nmtv.param        = e.elemImg( )->param;
            nmtv.offset       = e.toCharOffset( );

            /* If the return value is nonzero, stop here. */

            if (SendMessage( GetParent( wnd ), WM_NOTIFY, 
                             (WPARAM) nmtv.hdr.idFrom, (LPARAM) &nmtv ))
            {
               return 0;
            }
         }

         /* Show the menu. */
         {
            if (inst->m_selection.begin != inst->m_selection.end)
            {
               EnableMenuItem( inst->m_menu, ID_EDIT_COPY, MF_ENABLED | MF_BYCOMMAND );
               EnableMenuItem( inst->m_menu, ID_EDIT_CUT, MF_ENABLED | MF_BYCOMMAND );
               EnableMenuItem( inst->m_menu, ID_EDIT_CLEAR, MF_ENABLED | MF_BYCOMMAND );
            }
            else
            {
               EnableMenuItem( inst->m_menu, ID_EDIT_COPY, MF_GRAYED | MF_BYCOMMAND );
               EnableMenuItem( inst->m_menu, ID_EDIT_CUT, MF_GRAYED | MF_BYCOMMAND );
               EnableMenuItem( inst->m_menu, ID_EDIT_CLEAR, MF_GRAYED | MF_BYCOMMAND );
            }

            if (GetWindowStyle( wnd ) & PTBS_READONLY)
            {
               EnableMenuItem( inst->m_menu, ID_EDIT_CUT, MF_GRAYED | MF_BYCOMMAND );
               EnableMenuItem( inst->m_menu, ID_EDIT_CLEAR, MF_GRAYED | MF_BYCOMMAND );
               EnableMenuItem( inst->m_menu, ID_EDIT_PASTE, MF_GRAYED | MF_BYCOMMAND );
            }
            else
            {
               EnableMenuItem( inst->m_menu, ID_EDIT_PASTE, MF_ENABLED | MF_BYCOMMAND );
            }               

            POINT cursor;
            GetCursorPos( &cursor );
            TrackPopupMenu( inst->m_menu, TPM_RIGHTBUTTON, cursor.x, cursor.y, 0,
                            wnd, NULL );
         }

         return 0;
      }

   case WM_LBUTTONUP:
      {
         PtbInst* inst = _INST_PTR( );

         /* Stop dragging. */
         if (inst->m_dragging)
            ReleaseCapture( );
         return 0;
      }

   case WM_MOUSEMOVE:
      {
         POINT pt = { GET_X_LPARAM( lparam ), GET_Y_LPARAM( lparam ) };
         PtbInst* inst = _INST_PTR( );

         if (pt.x == inst->m_cursor.x && pt.y == inst->m_cursor.y)
            return 0;

         inst->m_cursor = pt;
         inst->hideTip( );

         if (inst->m_dragging)
         {
            /* Update the selection. */
            inst->setSel( inst->m_selection.begin, inst->offsetFromPoint( pt ) );
            inst->updateCaret( );
            InvalidateRect( wnd, NULL, FALSE );
         }
         
         /* Ask for hover events. */
         {
            TRACKMOUSEEVENT track;
            track.cbSize = sizeof( track );
            track.dwFlags = TME_HOVER;
            track.hwndTrack = wnd;
            track.dwHoverTime = HOVER_DEFAULT;
            TrackMouseEvent( &track );
         }

         return 0;
      }

   case WM_SETCURSOR:
      if ((HWND) wparam == wnd && LOWORD( lparam ) == HTCLIENT)
      {
         /* Change the cursor when over links. */
         POINT pt;
         GetCursorPos( &pt );
         ScreenToClient( wnd, &pt );

         ElemIter elem = _INST_PTR( )->elemFromPoint( pt );
         if (!!elem)
         {
            if ((elem.elem( )->type == PTB_ELEM_TYPE_STR && 
                 elem.elemStr( )->style & PTB_ELEM_STYLE_LINK) ||
                (elem.elem( )->type == PTB_ELEM_TYPE_IMG && 
                 elem.elemImg( )->style & PTB_ELEM_STYLE_LINK))
            {
               SetCursor( LoadCursor( NULL, IDC_HAND ) );
               return FALSE;
            }
         }

         SetCursor( LoadCursor( NULL, IDC_IBEAM ) );
         return FALSE;
      }
      SetCursor( LoadCursor( NULL, IDC_ARROW ) );
      return FALSE;

   case WM_TIMER:
      if (wparam == SCROLL_TIMER)
      {
         PtbInst* inst = _INST_PTR( );
         if (inst->m_dragging)
         {
            POINT pt;
            GetCursorPos( &pt );
            ScreenToClient( wnd, &pt );

            long area = inst->areaFromPoint( pt );

            if (area & AREA_ABOVE)
            {
               const int n = min( 4, (int) (0 - pt.y) / 24 + 1 );
               for (int i = 0; i < n; ++i)
                  PostMessage( wnd, WM_VSCROLL, SB_LINEUP, 0 );
            }
            else if (area & AREA_BELOW)
            {
               const int n = min( 4, (int) (pt.y - inst->m_clientsize.cy) / 24 + 1 );
               for (int i = 0; i < n; ++i)
                  PostMessage( wnd, WM_VSCROLL, SB_LINEDOWN, 0 );
            }
            if (area & AREA_LEFT)
            {
               const int n = min( 4, (int) (0 - pt.x) / 24 + 1 );
               for (int i = 0; i < n; ++i)
                  PostMessage( wnd, WM_HSCROLL, SB_LINELEFT, 0 );
            }
            else if (area & AREA_RIGHT)
            {
               const int n = min( 4, (int) (pt.x - inst->m_clientsize.cx) / 24 + 1 );
               for (int i = 0; i < n; ++i)
                  PostMessage( wnd, WM_HSCROLL, SB_LINERIGHT, 0 );
            }

            inst->setSel( inst->m_selection.begin, inst->offsetFromPoint( pt ) );
         }

         return 0;
      }

   case WM_CAPTURECHANGED:
      {
         /* Stop dragging. */
         _INST_PTR( )->m_dragging = false;
         KillTimer( wnd, SCROLL_TIMER );
         return 0;
      }

   case WM_MOUSEWHEEL:
      if ((short) HIWORD( wparam ) > 0)
         PostMessage( wnd, WM_VSCROLL, SB_LINEUP, 0 );
      else
         PostMessage( wnd, WM_VSCROLL, SB_LINEDOWN, 0 );
      return 0;         

   //////////////////////////////////////////////////////////////////////////

   case WM_COPY:
      {
         PtbInst* inst = _INST_PTR( );
         wstring str;
         inst->toString( min( inst->m_selection.begin, inst->m_selection.end ),
                         inst->m_selection.begin > inst->m_selection.end ?
                         inst->m_selection.begin - inst->m_selection.end :
                         inst->m_selection.end - inst->m_selection.begin,
                         &str, true, true );

         if (OpenClipboard( wnd ))
         {
            EmptyClipboard( );

            if (g_clipboard_unicode_available)
            {
               HANDLE hmem = to_global_unicode_text( str );
               if (!SetClipboardData( CF_UNICODETEXT, hmem ))
                  GlobalFree( hmem );
            }
            else
            {
               HANDLE hmem = to_global_text( str );
               if (!SetClipboardData( CF_TEXT, hmem ))
                  GlobalFree( hmem );
            }

            CloseClipboard( );
         }
      }
      return 0;

   case WM_PASTE:
      {
         PtbInst* inst = _INST_PTR( );

         HANDLE hmem = NULL;
         OpenClipboard( wnd );
         /* Try to get clipboard data in unicode. */
         if (hmem = GetClipboardData( CF_UNICODETEXT ))
         {
            PTBELEM elem;
            elem.type     = PTB_ELEM_TYPE_STR;
            elem.style    = inst->m_defaultstyle;
            elem.color    = inst->m_defaultcolor;
            elem.alt_text = NULL;
            elem.param    = inst->m_defaultparam;
            elem.text     = crlf_to_lf( _wcsdup( (const wchar_t*) GlobalLock( hmem ) ) );
            GlobalUnlock( hmem );
            inst->replaceSel( 1, &elem );

            free( const_cast<wchar_t*>( elem.text ) );

            inst->compact( );
            inst->buildLines( );
            inst->ensureVisible( inst->m_selection.end );
            inst->buildLines( );
            inst->updateCaret( );
            InvalidateRect( wnd, NULL, FALSE );
            send_ptb_changed( wnd );
         }
         else if (hmem = GetClipboardData( CF_TEXT )) // ANSI
         {
            const char* sz = (const char*) GlobalLock( hmem );

            /* Convert to wide char. */
            size_t buf_size = (strlen( sz ) + 1) * sizeof( wchar_t );
            wchar_t* buf = (wchar_t*) new char[ buf_size ];
            MultiByteToWideChar( 28591, // Latin-1
                                 0, sz, -1, buf, buf_size );
            GlobalUnlock( hmem );

            PTBELEM elem;
            elem.type     = PTB_ELEM_TYPE_STR;
            elem.style    = inst->m_defaultstyle;
            elem.color    = inst->m_defaultcolor;
            elem.alt_text = NULL;
            elem.param    = inst->m_defaultparam;
            elem.text     = crlf_to_lf( buf );
            inst->replaceSel( 1, &elem );
            
            delete [] buf;

            inst->compact( );
            inst->buildLines( );
            inst->ensureVisible( inst->m_selection.end );
            inst->buildLines( );
            inst->updateCaret( );
            InvalidateRect( wnd, NULL, FALSE );
            send_ptb_changed( wnd );
         }
         CloseClipboard( );
      }
      return 0;

   case WM_CUT:
      /* Just do a copy and a clear. */
      SendMessage( wnd, WM_COPY, 0, 0 );
      SendMessage( wnd, WM_CLEAR, 0, 0 );
      return 0;

   case WM_CLEAR:
      {
         PtbInst* inst = _INST_PTR( );
         inst->eraseSel( );
         inst->compact( );
         inst->buildLines( );
         inst->ensureVisible( inst->m_selection.end );
         inst->buildLines( );
         inst->updateCaret( );
         InvalidateRect( wnd, NULL, FALSE );
         send_ptb_changed( wnd );
      }
      return 0;

   //////////////////////////////////////////////////////////////////////////

   case WM_COMMAND:
      if (!HIWORD( wparam )) // Menu Item.
      {
         PtbInst* inst = _INST_PTR( );

         switch (LOWORD( wparam ))
         {
         case ID_EDIT_COPY:
            SendMessage( wnd, WM_COPY, 0, 0 );
            break;

         case ID_EDIT_CLEAR:
            if (GetWindowStyle( wnd ) & PTBS_READONLY)
               break;
            SendMessage( wnd, WM_CLEAR, 0, 0 );
            break;

         case ID_EDIT_CUT:
            if (GetWindowStyle( wnd ) & PTBS_READONLY)
               break;
            SendMessage( wnd, WM_CUT, 0, 0 );
            break;

         case ID_EDIT_PASTE:
            if (GetWindowStyle( wnd ) & PTBS_READONLY)
               break;
            SendMessage( wnd, WM_PASTE, 0, 0 );
            break;

         case ID_EDIT_SELECT_ALL:
            inst->setSel( 0, -1 );
            inst->updateCaret( );
            InvalidateRect( wnd, NULL, FALSE );
            break;
         }
         return 0;
      }

   //////////////////////////////////////////////////////////////////////////

   case WM_KEYDOWN:
      {
         PtbInst* inst = _INST_PTR( );

         // CTRL+ commands?
         if (wparam != VK_CONTROL && 
             LOWORD( lparam ) == 1 && GetKeyState( VK_CONTROL ) & 0x80000000)
         {
            // Notify the parent.
            NMPTBCTRLKEY nm;
            nm.hdr.code     = PTBN_CTRLKEY;
            nm.hdr.hwndFrom = wnd;
            nm.hdr.idFrom   = (UINT_PTR) GetMenu( wnd );
            nm.vk_code      = wparam;
            if (SendMessage( GetParent( wnd ), WM_NOTIFY, 0, (LPARAM) &nm ))
            {
               // Abort.
               return 0;
            }

            switch (wparam)
            {
            case 0x41: // VK_A
               SendMessage( wnd, WM_COMMAND, ID_EDIT_SELECT_ALL, 0 );
               break;

            case 0x43: // VK_C   
               SendMessage( wnd, WM_COPY, 0, 0 );
               break;

            case 0x56: // VK_V
               if (!(GetWindowStyle( wnd ) & PTBS_READONLY))
                  SendMessage( wnd, WM_PASTE, 0, 0 );
               break;

            case 0x58: // VK_X
               if (!(GetWindowStyle( wnd ) & PTBS_READONLY))
                  SendMessage( wnd, WM_CUT, 0, 0 );
               break;

            case VK_HOME:
               {
                  if (GetKeyState( VK_SHIFT ) & 0x80000000)
                     inst->setSel( inst->m_selection.begin, 0 );
                  else
                     inst->setSel( 0, 0 );

                  inst->ensureVisible( 0 );
                  inst->buildLines( );
                  inst->updateCaret( );
                  InvalidateRect( wnd, NULL, FALSE );
               }
               break;

            case VK_END:
               {
                  if (GetKeyState( VK_SHIFT ) & 0x80000000)
                     inst->setSel( inst->m_selection.begin, -1 );
                  else
                     inst->setSel( -1, -1 );

                  inst->ensureVisible( -1 );
                  inst->buildLines( );
                  inst->updateCaret( );
                  InvalidateRect( wnd, NULL, FALSE );
               }
               break;
            }
            return 0;
         } // if (LOWORD( lparam ) == 1 && GetKeyState( VK_CONTROL ) & 0x80000000)

         switch (wparam)
         {
         case VK_LEFT:
            if (inst->m_selection.end)
            {
               // Move back one character.
               // This assumes there's possibly only a single linebreak between lines.
               size_t off = inst->m_selection.end - 1;
               if (GetKeyState( VK_SHIFT ) & 0x80000000)
                  inst->setSel( inst->m_selection.begin, off );
               else
                  inst->setSel( off, off );

               inst->ensureVisible( off );
               inst->buildLines( );
               inst->updateCaret( );
               InvalidateRect( wnd, NULL, FALSE );
            }
            break;

         case VK_RIGHT:
            {
               // Move back one character.
               // This assumes there's possibly only a single linebreak between lines.
               size_t off = inst->m_selection.end + 1;
               if (GetKeyState( VK_SHIFT ) & 0x80000000)
                  inst->setSel( inst->m_selection.begin, off );
               else
                  inst->setSel( off, off );

               inst->ensureVisible( off );
               inst->buildLines( );
               inst->updateCaret( );
               InvalidateRect( wnd, NULL, FALSE );
            }
            break;

         case VK_UP:
            {
               // Move up one line.
               size_t line = inst->lineFromOffset( inst->m_selection.end );
               if (line != -1 && line > 0)
               {
                  // Try to stay in the same column.
                  size_t col = inst->m_selection.end - inst->m_lines[ line ].off;
                  size_t off = inst->m_lines[ line - 1 ].off + 
                               min( col, inst->m_lines[ line - 1 ].len );

                  if (GetKeyState( VK_SHIFT ) & 0x80000000)
                     inst->setSel( inst->m_selection.begin, off );
                  else
                     inst->setSel( off, off );

                  inst->ensureVisible( off );
                  inst->buildLines( );
                  inst->updateCaret( );
                  InvalidateRect( wnd, NULL, FALSE );
               }
            }
            break;

         case VK_DOWN:
            {
               // Move down one line.
               size_t line = inst->lineFromOffset( inst->m_selection.end );
               if (line + 1 < inst->m_lines.size( ))
               {
                  // Try to stay in the same column.
                  size_t col = inst->m_selection.end - inst->m_lines[ line ].off;
                  size_t off = inst->m_lines[ line + 1 ].off + 
                               min( col, inst->m_lines[ line + 1 ].len );

                  if (GetKeyState( VK_SHIFT ) & 0x80000000)
                     inst->setSel( inst->m_selection.begin, off );
                  else
                     inst->setSel( off, off );

                  inst->ensureVisible( off );
                  inst->buildLines( );
                  inst->updateCaret( );
                  InvalidateRect( wnd, NULL, FALSE );
               }
            }
            break;

         case VK_PRIOR:
            {
               // Move up a page.
               size_t line = inst->lineFromOffset( inst->m_selection.end );
               if (line != -1 && line > 0)
               {
                  size_t next = line - min( line, inst->m_bottom_line - inst->m_top_line );

                  // Try to stay in the same column.
                  size_t col = inst->m_selection.end - inst->m_lines[ line ].off;
                  size_t off = inst->m_lines[ next ].off + 
                               min( col, inst->m_lines[ next ].len );

                  if (GetKeyState( VK_SHIFT ) & 0x80000000)
                     inst->setSel( inst->m_selection.begin, off );
                  else
                     inst->setSel( off, off );

                  inst->ensureVisible( off );
                  inst->buildLines( );
                  inst->updateCaret( );
                  InvalidateRect( wnd, NULL, FALSE );
               }
            }
            break;

         case VK_NEXT:
            {
               // Move down a page.
               size_t line = inst->lineFromOffset( inst->m_selection.end );
               if (line < inst->m_lines.size( ))
               {
                  size_t next = min( inst->m_lines.size( ) - 1, 
                                     line + inst->m_bottom_line - inst->m_top_line );
                  
                  // Try to stay in the same column.
                  size_t col = inst->m_selection.end - inst->m_lines[ line ].off;
                  size_t off = inst->m_lines[ next ].off + 
                     min( col, inst->m_lines[ next ].len );

                  if (GetKeyState( VK_SHIFT ) & 0x80000000)
                     inst->setSel( inst->m_selection.begin, off );
                  else
                     inst->setSel( off, off );

                  inst->ensureVisible( off );
                  inst->buildLines( );
                  inst->updateCaret( );
                  InvalidateRect( wnd, NULL, FALSE );
               }
            }
            break;

         case VK_HOME:
            {
               size_t off = inst->findHome( inst->m_selection.end );

               if (GetKeyState( VK_SHIFT ) & 0x80000000)
                  inst->setSel( inst->m_selection.begin, off );
               else
                  inst->setSel( off, off );

               inst->ensureVisible( off );
               inst->buildLines( );
               inst->updateCaret( );
               InvalidateRect( wnd, NULL, FALSE );
            }
            break;

         case VK_END:
            {
               size_t off = inst->findEnd( inst->m_selection.end );

               if (GetKeyState( VK_SHIFT ) & 0x80000000)
                  inst->setSel( inst->m_selection.begin, off );
               else
                  inst->setSel( off, off );

               inst->ensureVisible( off );
               inst->buildLines( );
               inst->updateCaret( );
               InvalidateRect( wnd, NULL, FALSE );
            }
            break;

         case VK_ESCAPE:
            {
               // Notify the parent.
               NMHDR hdr;
               hdr.code     = PTBN_ESCAPE;
               hdr.hwndFrom = wnd;
               hdr.idFrom   = (UINT_PTR) GetMenu( wnd );
               if (!SendMessage( GetParent( wnd ), WM_NOTIFY, 0, (LPARAM) &hdr ))
               {
                  // Unselect.
                  inst->setSel( inst->m_selection.end, inst->m_selection.end );
                  inst->updateCaret( );
                  InvalidateRect( wnd, NULL, FALSE );
               }
            }
            break;

         case VK_BACK:
            if (GetWindowStyle( wnd ) & PTBS_READONLY)
               break;

            if (inst->m_selection.end != inst->m_selection.begin)
            {
               // Clear the selection.
               inst->eraseSel( );
               inst->compact( );
               inst->buildLines( );
               inst->ensureVisible( inst->m_selection.end );
               inst->buildLines( );
               inst->updateCaret( );
               InvalidateRect( wnd, NULL, FALSE );
               send_ptb_changed( wnd );
            }
            else if (inst->m_selection.end)
            {
               // Erase the previous character.
               inst->erase( inst->m_selection.end - 1, 1 );
               inst->compact( );
               inst->buildLines( );
               inst->ensureVisible( inst->m_selection.end );
               inst->buildLines( );
               inst->updateCaret( );
               InvalidateRect( wnd, NULL, FALSE );
               send_ptb_changed( wnd );
            }
            break;

         case VK_DELETE:
            if (GetWindowStyle( wnd ) & PTBS_READONLY)
               break;

            if (inst->m_selection.end != inst->m_selection.begin)
            {
               // Clear the selection.
               inst->eraseSel( );
               inst->compact( );
               inst->buildLines( );
               inst->ensureVisible( inst->m_selection.end );
               inst->buildLines( );
               inst->updateCaret( );
               InvalidateRect( wnd, NULL, FALSE );
               send_ptb_changed( wnd );
            }
            else
            {
               // Erase the current character.
               inst->erase( inst->m_selection.end, 1 );
               inst->compact( );
               inst->buildLines( );
               inst->ensureVisible( inst->m_selection.end );
               inst->buildLines( );
               inst->updateCaret( );
               InvalidateRect( wnd, NULL, FALSE );
               send_ptb_changed( wnd );
            }
            break;

         case VK_RETURN:
            {
               // Notify the parent.
               NMHDR hdr;
               hdr.code     = PTBN_RETURN;
               hdr.hwndFrom = wnd;
               hdr.idFrom   = (UINT_PTR) GetMenu( wnd );
               if (!SendMessage( GetParent( wnd ), WM_NOTIFY, 0, (LPARAM) &hdr ))
               {
                  if (GetWindowStyle( wnd ) & PTBS_READONLY)
                     break;

                  // Insert a line break.
                  PTBELEM e;
                  e.type     = PTB_ELEM_TYPE_BREAK;
                  e.style    = inst->m_defaultstyle;
                  e.color    = inst->m_defaultcolor;
                  e.alt_text = NULL;
                  e.param    = inst->m_defaultparam;
                  e.text     = L"\n";
                  inst->replaceSel( 1, &e );

                  inst->compact( );
                  inst->buildLines( );
                  inst->ensureVisible( inst->m_selection.end );
                  inst->buildLines( );
                  inst->updateCaret( );
                  InvalidateRect( wnd, NULL, FALSE );
                  send_ptb_changed( wnd );
               }
            }
            break;

         case VK_TAB:
            {
               // Notify the parent.
               NMHDR hdr;
               hdr.code     = PTBN_TAB;
               hdr.hwndFrom = wnd;
               hdr.idFrom   = (UINT_PTR) GetMenu( wnd );
               SendMessage( GetParent( wnd ), WM_NOTIFY, 0, (LPARAM) &hdr );
            }
            break;
         }
      }
      return 0;

   case WM_CHAR:
      {
         if (GetWindowStyle( wnd ) & PTBS_READONLY)
            return 0;

         // WM_CHARs are UTF-16 encoded.
         // If the character has a surrogate word, ignore it, since it won't fit.
         if (HIWORD( wparam ))
            return 0;

         PtbInst* inst = _INST_PTR( );
         if ((unsigned) wparam >= 0x20 && wparam != 0x7F)
         {
            // Insert the character.
            wchar_t sz[ 2 ] = { *(wchar_t*) &wparam, L'\0' };
            PTBELEM e;
            e.type     = PTB_ELEM_TYPE_STR;
            e.style    = inst->m_defaultstyle;
            e.color    = inst->m_defaultcolor;
            e.alt_text = NULL;
            e.param    = inst->m_defaultparam;
            e.text     = sz;
            inst->replaceSel( 1, &e );

            inst->compact( );
            inst->buildLines( );
            inst->ensureVisible( inst->m_selection.end );
            inst->buildLines( );
            inst->updateCaret( );
            InvalidateRect( wnd, NULL, FALSE );
            send_ptb_changed( wnd );
         }            
      }
      return 0;

   //////////////////////////////////////////////////////////////////////////

   case PTBM_REPLACESEL:
      {
         PtbInst* inst = _INST_PTR( );
         inst->replaceSel( (size_t) wparam, (const PTBELEM*) lparam );
         inst->compact( );
         inst->buildLines( );
         inst->ensureVisible( inst->m_selection.end );
         inst->buildLines( );
         inst->updateCaret( );
         InvalidateRect( wnd, NULL, FALSE );
         send_ptb_changed( wnd );
      }
      return 0;

   case PTBM_APPEND:
      {
         PtbInst* inst = _INST_PTR( );

         inst->insert( -1, (size_t) wparam, (const PTBELEM*) lparam );
         inst->compact( );
         inst->buildLines( );
         inst->updateCaret( );
         InvalidateRect( wnd, NULL, FALSE );
         send_ptb_changed( wnd );
      }
      return 0;

   case PTBM_SETSEL:
      {
         PtbInst* inst = _INST_PTR( );
         inst->setSel( (size_t) wparam, (size_t) lparam );
         inst->updateCaret( );
         send_ptb_changed( wnd );
         return (LRESULT) (inst->m_selection.end - inst->m_selection.begin);
      }

   case PTBM_GETSEL:
      {
         PtbInst* inst = _INST_PTR( );
         *(size_t*) wparam = inst->m_selection.begin;
         *(size_t*) lparam = inst->m_selection.end;
      }
      return 0;

   case PTBM_ENSUREVISIBLE:
      {
         PtbInst* inst = _INST_PTR( );
         inst->ensureVisible( (size_t) wparam );
         inst->buildLines( );
         inst->updateCaret( );
         InvalidateRect( wnd, NULL, FALSE );
      }
      return 0;

   case PTBM_GETLINECOUNT:
      return _INST_PTR( )->m_lines.size( );
      
   case PTBM_TRIMLINES:
      {
         PtbInst* inst = _INST_PTR( );
         // Trim.
         inst->trimLines( (size_t) wparam );
         inst->compact( );
         inst->buildLines( );
         inst->updateCaret( );
         InvalidateRect( wnd, NULL, FALSE );
         send_ptb_changed( wnd );
         return inst->m_lines.size( );
      }

   case PTBM_SETBKGNDCOLOR:
      {
         PtbInst* inst = _INST_PTR( );
         inst->setBkColor( (COLORREF) wparam );
         InvalidateRect( wnd, NULL, FALSE );
      }
      return 0;

   case PTBM_GETTEXT:
      {
         PtbInst* inst = _INST_PTR( );
         PTBGETTEXT* gettext = (PTBGETTEXT*) lparam;
         if (gettext->text_max)
         {
            size_t off, len;
            if (gettext->end > gettext->begin)
               off = gettext->begin, len = gettext->end - gettext->begin;
            else 
               off = gettext->end, len = gettext->begin - gettext->end;

            len = min( len, gettext->text_max - 1 );
            wstring out;
            inst->toString( off, len, &out, 
                           !!(gettext->flags & PTBGETTEXT_CRLF),
                           !!(gettext->flags & PTB_GETTEXT_IMGALT) );

            assert( out.length( ) < gettext->text_max );

            wcscpy( gettext->text, out.c_str( ) );
            return out.length( );
         }
      }
      return 0;

   case PTBM_SETTEXTLIMIT:
      {
         PtbInst* inst = _INST_PTR( );
         inst->setLimit( (size_t) wparam );
         inst->compact( );
         inst->buildLines( );
         inst->updateCaret( );
         InvalidateRect( wnd, NULL, FALSE );
         send_ptb_changed( wnd );
      }
      return 0;

   case PTBM_GETTEXTLIMIT:
      return _INST_PTR( )->m_limit;

   case PTBM_FLUSH:
      {
         PtbInst* inst = _INST_PTR( );
         inst->eraseAll( );
         inst->compact( );
         inst->buildLines( );
         inst->updateCaret( );
         InvalidateRect( wnd, NULL, FALSE );
      }
      return 0;

   case PTBM_GETDEFAULTCOLOR:
      return _INST_PTR( )->m_defaultcolor;

   case PTBM_SETDEFAULTCOLOR:
      _INST_PTR( )->m_defaultcolor = (COLORREF) wparam;
      return 0;

   case PTBM_GETDEFAULTSTYLE:
      return _INST_PTR( )->m_defaultstyle;

   case PTBM_SETDEFAULTSTYLE:
      _INST_PTR( )->m_defaultstyle = (long) wparam;
      return 0;

   case PTBM_GETDEFAULTPARAM:
      return _INST_PTR( )->m_defaultparam;

   case PTBM_SETDEFAULTPARAM:
      _INST_PTR( )->m_defaultparam = lparam;
      return 0;
   
   case PTBM_GETCONTENTSIZE:
      {
         PtbInst* inst = _INST_PTR( );
         ((SIZE*) lparam)->cx = inst->m_content_size.w;
         ((SIZE*) lparam)->cy = inst->m_content_size.h;
      }
      return 0;

   case WM_GETTEXT:
      if (wparam)
      {
         PtbInst* inst = _INST_PTR( );
         wstring str;
         inst->toString( 0, (size_t) wparam, &str, true );

         return WideCharToMultiByte( 28591, // Latin-1
                                     0, str.c_str( ), -1, (char*) lparam,
                                     (int) wparam, NULL, NULL ) - 1;
         return str.length( );
      }

   case WM_GETTEXTLENGTH:
      return _INST_PTR( )->toStringLength( 0, -1, true );

   case PTBM_ISEMPTY:
      return _INST_PTR( )->m_length == 0;

   case WM_SETTEXT:
      {
         PtbInst* inst = _INST_PTR( );
         const char* sz = (const char*) lparam;

         /* Convert to wide char. */
         size_t buf_size = (strlen( sz ) + 1) * sizeof( wchar_t );
         wchar_t* buf = (wchar_t*) new char[ buf_size ];
         MultiByteToWideChar( 28591, // Latin-1
                              0, sz, -1, buf, buf_size );

         PTBELEM elem;
         elem.type     = PTB_ELEM_TYPE_STR;
         elem.style    = inst->m_defaultstyle;
         elem.alt_text = NULL;
         elem.param    = inst->m_defaultparam;
         elem.color    = inst->m_defaultcolor;
         elem.text     = crlf_to_lf( buf );
         inst->eraseAll( );
         inst->insert( 0, 1, &elem );
         
         delete [] buf;

         inst->compact( );
         inst->buildLines( );
         inst->updateCaret( );
         InvalidateRect( wnd, NULL, FALSE );

         send_ptb_changed( wnd );
         return TRUE;
      }

   case WM_SETFONT:
      {
         PtbInst* inst = _INST_PTR( );
         inst->setFont( (HFONT) wparam );
         inst->buildLines( );
         inst->updateCaret( );
         if (lparam)
            InvalidateRect( wnd, NULL, FALSE );
      }
      return 0;

   case WM_GETFONT:
      return (LRESULT) _INST_PTR( )->m_fonts[ PTB_FONT_NORMAL ];

   }

   return DefWindowProc( wnd, msg, wparam, lparam );
}

HANDLE to_global_text( const wstring& str )
{
   size_t mem_size = str.length( ) + 1;
   HANDLE hmem = GlobalAlloc( GMEM_MOVEABLE, mem_size );
   char* buf = (char*) GlobalLock( hmem );
   WideCharToMultiByte( 28591, // Latin-1
                        0, str.c_str( ), -1, buf,
                        mem_size, NULL, NULL );
   GlobalUnlock( hmem );
   return hmem;
}

HANDLE to_global_unicode_text( const wstring& str )
{
   size_t mem_size = (str.length( ) + 1) * sizeof( wchar_t );
   HANDLE hmem = GlobalAlloc( GMEM_MOVEABLE, mem_size );
   wchar_t* buf = (wchar_t*) GlobalLock( hmem );
   memcpy( buf, str.c_str( ), mem_size );
   GlobalUnlock( hmem );
   return hmem;
}

wchar_t* crlf_to_lf( wchar_t* sz )
{
   wchar_t *src = sz, *dst = sz;
   for (; *src; ++src, ++dst)
   {
      if (src[ 0 ] == L'\r' && src[ 1 ] == L'\n')
         dst[ 0 ] = '\n', ++src;
      else
         dst[ 0 ] = src[ 0 ];
   }
   *dst = L'\0';

   return sz;
}

/* Windows 98 only has partial support for unicode.
*  see http://msdn2.microsoft.com/en-us/library/ms813063.aspx */
BOOL WINAPI GetTextExtentExPointW98( HDC hdc, LPCWSTR lpszStr, 
                                     int cchString, int nMaxExtent, 
                                     LPINT lpnFit, LPINT alpDx, 
                                     LPSIZE lpSize )
{
   //return GetTextExtentExPointW( hdc, lpszStr, cchString, nMaxExtent, lpnFit, alpDx, lpSize );
   TEXTMETRIC metric;
   GetTextMetrics( hdc, &metric );

   const int extra = GetTextCharacterExtra( hdc );

   int pos = 0;
   int fit = 0;

   if (alpDx)
   {
      for (int i = 0; i < cchString; ++lpszStr, ++i, ++alpDx, pos += extra)
      {
         int w = 0;
         GetCharWidthW( hdc, *lpszStr, *lpszStr, &w );
         w -= metric.tmOverhang;
         pos += w;
         *alpDx = pos;

         if (pos < nMaxExtent)
            ++fit;
      }
   }
   else
   {
      for (int i = 0; i < cchString; ++lpszStr, ++i, pos += extra)
      {
         int w = 0;
         GetCharWidthW( hdc, *lpszStr, *lpszStr, &w );
         w -= metric.tmOverhang;
         pos += w;

         if (pos < nMaxExtent)
            ++fit;
      }
   }

   if (cchString == 1)
      pos -= extra;

   if (lpnFit)
      *lpnFit = fit;

   lpSize->cx = pos;
   lpSize->cy = metric.tmHeight;
   return TRUE;
}

void send_ptb_changed( HWND ptb )
{
   NMHDR hdr;
   hdr.code     = PTBN_CHANGED;
   hdr.hwndFrom = ptb;
   hdr.idFrom   = (UINT_PTR) GetMenu( ptb );
   SendMessage( GetParent( ptb ), WM_NOTIFY, 0, (LPARAM) &hdr );
}