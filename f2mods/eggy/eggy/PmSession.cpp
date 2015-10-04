#include "eggy.h"
#include "resource.h"
#include "ChatLexer.h"
#include "PoorTextBox.h"
#include "xml_charset.h"
#include "yiffurc/fsh.hpp"
#include <xutility>
#include <vector>
#include <fstream>
#include <FreeImage.h>
#include <map>

namespace eggy {

//////////////////////////////////////////////////////////////////////////

#define _SAT( i ) (max( (int)0, (int)(i) ))

enum { MAX_CHAT_LINES = 256, INPUT_HISTORY_SIZE = 16 };
enum { IMGL_UNIGNORE = 0, IMGL_JOIN, IMGL_SUMMON, 
       IMGL_DECLINE, IMGL_IGNORE, IMGL_SHOWRECIP, IMGL_SAVE, IMGL_VIEWLOG,
       IMGL_ADDBUDDY, IMGL_RMBUDDY };
enum { IMGL_CONTACT = 0, IMGL_CONTACT_OFF };
enum { DLG_MARGIN = 7, DLG_PADDING = 7, DLG_RECIPPANE_W = 110,
       DLG_MIN_WIDTH = 280, DLG_MIN_HEIGHT = 185 };
enum { ID_PM_JOIN = 0x300, ID_PM_SUMMON, ID_PM_DECLINE, ID_PM_IGNORE, ID_PM_UNIGNORE,
       ID_PM_SAVE, ID_PM_SHOWRECIP, ID_PM_VIEWLOG, ID_PM_ADDBUDDY, ID_PM_RMBUDDY };
enum { CHAT_COLOR_NORMAL = RGB( 0, 0, 0 ), CHAT_COLOR_EMOTE = RGB( 128, 0, 200 ), 
       CHAT_COLOR_LINK = RGB( 0, 0, 255 ), CHAT_COLOR_MSG = RGB( 128, 128, 128 ),
       CHAT_COLOR_NAME = RGB( 200, 0, 0 ), CHAT_COLOR_MYNAME = RGB( 0, 0, 220 ),
       CHAT_COLOR_OFFLINE = RGB( 128, 128, 128 ) };
enum { MSG_ICON_GENERAL = 0, MSG_ICON_ONLINE, MSG_ICON_OFFLINE, 
       MSG_ICON_ERROR, MSG_ICON_NONE };

//////////////////////////////////////////////////////////////////////////

static HIMAGELIST s_imgl_icons        = NULL;
static HIMAGELIST s_imgl_toolbar      = NULL;
static HIMAGELIST s_imgl_toolbaroff   = NULL;
static HIMAGELIST s_imgl_msgicons     = NULL;
static HIMAGELIST sa_imgl_smileyicons[ ChatLexer::NUM_SMILEYS ];
static map<string,HIMAGELIST> sa_img_map; // For img tags.
static HMENU      s_recip_menu        = NULL;
static HMENU      s_link_menu         = NULL;
static HFONT      s_font              = NULL;
static WNDPROC    s_edit_proc         = NULL;

/** Converts a c-string to unicode, replaces pipes with spaces. */
static wchar_t* to_unicode( const char* sz, size_t len = -1 );
/** Converts a c-string to unicode, replaces pipes with spaces,
    and collapses entity names. */
static wchar_t* to_unicode_ex( const char* sz, size_t len = -1 );

/** Loads an fsh into an imagelist array.  Returns the number loaded. */
static unsigned fsh_to_imgls( const string& filename,  unsigned first, unsigned num, HIMAGELIST out[] );

/** Loads a image file as an imagelist. If the file was previously loaded,
    a cached copy will be returned.  Images will be rendered upside-down to mimic client behavior. */
static HIMAGELIST imgl_from_file( const string& filename );

/** Gets the smiley index from the third character of a #Sx string. */
static unsigned to_smiley_index( char c );

inline int ListView_FindFirstItemLParam( HWND lv, LPARAM lparam )
{
   LVFINDINFO lvf;
   lvf.flags = LVFI_PARAM;
   lvf.lParam = lparam;
   return ListView_FindItem( lv, -1, &lvf );
}

//////////////////////////////////////////////////////////////////////////

void PmSession::class_init( )
{
   /* Init common controls. */
   {
      INITCOMMONCONTROLSEX iccex;
      iccex.dwSize = sizeof( iccex );
      iccex.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES;
      InitCommonControlsEx( &iccex );
   }

   /* Init poor text box. */
   PoorTextBox_Init( s_inst );

   /* Register the window class. */
   {
      WNDCLASSEX wc;
      memset( &wc, 0, sizeof( wc ) );
      wc.cbSize        = sizeof( wc );
      // wc.hIcon         = LoadIcon( s_inst, MAKEINTRESOURCE( IDI_ICON1 ) );
      wc.hbrBackground = GetSysColorBrush( COLOR_3DFACE );
      wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
      wc.hInstance     = s_inst;
      wc.lpfnWndProc   = PmSession::wnd_proc;
      wc.lpszClassName = "EggyPmSession";
      wc.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
      RegisterClassEx( &wc );
   }

   /* Get the original edit control proc. */
   s_edit_proc = get_sys_class_proc( WC_EDIT );

   /* Create icons. */
   s_imgl_icons = ImageList_LoadImage( s_inst, MAKEINTRESOURCE( IDB_PMICONS ),
                                       16, 0, CLR_DEFAULT,
                                       IMAGE_BITMAP, LR_CREATEDIBSECTION );
   s_imgl_toolbar = ImageList_LoadImage( s_inst, MAKEINTRESOURCE( IDB_PMTOOLBAR ),
                                         16, 0, CLR_DEFAULT,
                                         IMAGE_BITMAP, LR_CREATEDIBSECTION );
   s_imgl_toolbaroff = ImageList_LoadImage( s_inst, MAKEINTRESOURCE( IDB_PMTOOLBAROFF ),
                                            16, 0, CLR_DEFAULT,
                                            IMAGE_BITMAP, LR_CREATEDIBSECTION );
   s_imgl_msgicons = ImageList_LoadImage( s_inst, MAKEINTRESOURCE( IDB_PMMSGICONS ),
                                          24, 0, CLR_DEFAULT,
                                          IMAGE_BITMAP, LR_CREATEDIBSECTION );

   unsigned fsh_loaded = fsh_to_imgls( string( f2_clroot( ) ) + "\\patches\\default\\smileys.fsh", 0, 16, sa_imgl_smileyicons );
   fsh_loaded += fsh_to_imgls( string( f2_clroot( ) ) + "\\patches\\default\\smileys2.fsh", 0, 38, sa_imgl_smileyicons + 16 );

   assert( fsh_loaded == ChatLexer::NUM_SMILEYS );

   /* Create popup menus. */
   s_recip_menu = LoadMenu( s_inst, MAKEINTRESOURCE( IDR_RECIP ) );
   s_link_menu  = LoadMenu( s_inst, MAKEINTRESOURCE( IDR_LINK ) );

   /* Create fonts. */
   {
      HFONT font_sys = GetStockFont( DEFAULT_GUI_FONT );
      LOGFONT lf;
      GetObject( font_sys, sizeof( LOGFONT ), &lf );

      //lf.lfQuality = 5; // CLEARTYPE_QUALITY; 
      s_font = CreateFontIndirect( &lf );
   }
}

void PmSession::class_uninit( )
{
   DeleteFont( s_font ); s_font = NULL;
   DestroyMenu( s_link_menu ); s_link_menu = NULL;
   DestroyMenu( s_recip_menu ); s_recip_menu = NULL;   
   ImageList_Destroy( s_imgl_icons ); s_imgl_icons = NULL;
   ImageList_Destroy( s_imgl_toolbar ); s_imgl_toolbar = NULL;
   ImageList_Destroy( s_imgl_toolbaroff ); s_imgl_toolbaroff = NULL;
   ImageList_Destroy( s_imgl_msgicons ); s_imgl_msgicons = NULL;
   for (unsigned i = 0; i < ChatLexer::NUM_SMILEYS; ++i)
      if (sa_imgl_smileyicons[ i ])
         ImageList_Destroy( sa_imgl_smileyicons[ i ] ), sa_imgl_smileyicons[ i ] = NULL;
   s_edit_proc = NULL;
   UnregisterClass( "EggyPmSession", s_inst );
   PoorTextBox_Uninit( s_inst );
}

PmSession::PmSession( int x, int y, unsigned width, unsigned height )
   : m_recipients_visible( false ), m_last_sent( 0 ), m_last_received( 0 ),
     m_created( time( NULL ) ), m_worth_saving( false ), m_groups_menu( NULL )
{
   assert( Eggy::singleton( ) );

   const HWND focus_wnd = GetFocus( );

   /* Create the window and controls. */
   m_wnd.wnd = CreateWindowEx( WS_EX_APPWINDOW, "EggyPmSession", "<PmSession>",
                               WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX
                               | WS_MAXIMIZEBOX | WS_SIZEBOX | WS_THICKFRAME,
                               0, 0, 390, 250,
                               NULL, NULL, s_inst, 
                               this );
   m_wnd.status = CreateWindowEx( 0, WC_STATIC, "<Status>",
                                  WS_CHILDWINDOW | WS_VISIBLE
                                  | SS_CENTER | SS_NOPREFIX | SS_ENDELLIPSIS,
                                  0, 0, 245, 11, m_wnd.wnd, NULL, s_inst, NULL );
   m_wnd.input = CreateWindowExW( 0, WC_EDITW, L"",
                                 WS_CHILDWINDOW | WS_BORDER | WS_VSCROLL | WS_VISIBLE
                                 | ES_MULTILINE,
                                 0, 0, 196, 25, m_wnd.wnd, NULL, s_inst, NULL );
   m_wnd.chat = CreateWindowEx( 0, WC_POORTEXTBOX, "",
                                WS_CHILDWINDOW | WS_BORDER | WS_VSCROLL | WS_VISIBLE
                                | PTBS_MULTILINE | PTBS_READONLY,
                                0, 0, 246, 50, m_wnd.wnd, NULL, s_inst, NULL );
   m_wnd.send = CreateWindowEx( 0, WC_BUTTON, "Send",
                                WS_CHILDWINDOW | WS_VISIBLE
                                | BS_PUSHBUTTON | BS_CENTER | BS_DEFPUSHBUTTON,
                                0, 0, 45, 25, m_wnd.wnd, (HMENU) 99, s_inst, NULL );
   m_wnd.ltoolbar = CreateWindowEx( 0, TOOLBARCLASSNAME, "",
                                    WS_CHILDWINDOW | WS_VISIBLE | CCS_NOPARENTALIGN 
                                    | CCS_NORESIZE | CCS_NODIVIDER 
                                    | TBSTYLE_LIST  | TBSTYLE_TOOLTIPS,
                                    0, 0, 246, 12, m_wnd.wnd, NULL, s_inst, NULL );
   m_wnd.rtoolbar = CreateWindowEx( 0, TOOLBARCLASSNAME, "",
                                    WS_CHILDWINDOW | WS_VISIBLE | CCS_NOPARENTALIGN 
                                    | CCS_NORESIZE | CCS_NODIVIDER 
                                    | TBSTYLE_LIST  | TBSTYLE_TOOLTIPS,
                                    0, 0, 246, 12, m_wnd.wnd, NULL, s_inst, NULL );
   m_wnd.recipients = CreateWindowEx( 0, WC_LISTVIEW, "",
                                      WS_CHILDWINDOW | WS_BORDER 
                                      | LVS_REPORT | LVS_NOCOLUMNHEADER | LVS_ALIGNLEFT | LVS_SORTASCENDING | 
                                      LVS_EDITLABELS | LVS_SINGLESEL | LVS_SHAREIMAGELISTS,
                                      0, 0, 72, 94, m_wnd.wnd, NULL, s_inst, NULL );
   m_wnd.recipients_label = CreateWindowEx( 0, WC_STATIC, "Recipients",
                                            WS_CHILDWINDOW | SS_CENTER | SS_NOPREFIX 
                                            | SS_ENDELLIPSIS,
                                            0, 0, 71, 11, m_wnd.wnd, NULL, s_inst, NULL );
   
   m_groups_menu = CreatePopupMenu( );
   
   SetClassLongPtr( m_wnd.wnd, GCLP_HICON, (LONG_PTR) Eggy::singleton( )->getIcon( ) );

   SetWindowFont( m_wnd.chat, s_font, TRUE );
   SetWindowFont( m_wnd.input, s_font, TRUE );
   SetWindowFont( m_wnd.recipients, s_font, TRUE );
   SetWindowFont( m_wnd.send, GetStockFont( DEFAULT_GUI_FONT ), TRUE );
   SetWindowFont( m_wnd.status, GetStockFont( DEFAULT_GUI_FONT ), TRUE );
   SetWindowFont( m_wnd.ltoolbar, GetStockFont( DEFAULT_GUI_FONT ), TRUE );
   SetWindowFont( m_wnd.rtoolbar, GetStockFont( DEFAULT_GUI_FONT ), TRUE );
   SetWindowFont( m_wnd.recipients_label, GetStockFont( DEFAULT_GUI_FONT ), TRUE );
   SetWindowFont( m_wnd.chat, GetStockFont( DEFAULT_GUI_FONT ), TRUE );

   SendMessage( m_wnd.input, EM_SETMARGINS, EC_LEFTMARGIN | EC_RIGHTMARGIN, 4 );
   SendMessage( m_wnd.input, EM_SETLIMITTEXT, 3000, 0 );
   // Subclass the input control.
   SetWindowLongPtrW( m_wnd.input, GWLP_WNDPROC, (LONG_PTR) PmSession::input_proc );
   
   ListView_SetExtendedListViewStyle( m_wnd.recipients, LVS_EX_INFOTIP );
   ListView_SetImageList( m_wnd.recipients, s_imgl_icons, LVSIL_SMALL );
   // Pretty tooltip margins.
   {
      RECT r = { 3, 3, 3, 3 };
      SendMessage( ListView_GetToolTips( m_wnd.recipients ), TTM_SETMARGIN, 0, (LPARAM) &r );
   }
   // Give tooltips a really long autopop.
   SendMessage( ListView_GetToolTips( m_wnd.recipients ), TTM_SETDELAYTIME, TTDT_AUTOPOP, 30000 );
   // Add a single column.
   {
      LVCOLUMN col;
      col.mask     = LVCF_SUBITEM;
      col.iSubItem = 0;
      // Column will be autosized in fitElements.
      ListView_InsertColumn( m_wnd.recipients, 0, &col );
   }

   /* Set up the left toolbar. */
   {
      SendMessage( m_wnd.ltoolbar, TB_SETIMAGELIST, 0, (LPARAM) s_imgl_toolbar );
      SendMessage( m_wnd.ltoolbar, TB_SETDISABLEDIMAGELIST, 0, (LPARAM) s_imgl_toolbaroff );
      SendMessage( m_wnd.ltoolbar, TB_SETBUTTONSIZE, 0, MAKELONG( 16, 16 ) );

      const BYTE style = TBSTYLE_BUTTON;
      TBBUTTON buttons[ 9 ];
      memset( buttons, 0, sizeof( buttons ) );

      buttons[ 0 ].iBitmap = IMGL_JOIN;
      buttons[ 0 ].idCommand = ID_PM_JOIN;
      buttons[ 0 ].fsState = TBSTATE_ENABLED;
      buttons[ 0 ].fsStyle = style;
      buttons[ 0 ].iString = (INT_PTR) 0;

      buttons[ 1 ].iBitmap = IMGL_SUMMON;
      buttons[ 1 ].idCommand = ID_PM_SUMMON;
      buttons[ 1 ].fsState = TBSTATE_ENABLED;
      buttons[ 1 ].fsStyle = style;
      buttons[ 1 ].iString = (INT_PTR) 0;

      buttons[ 2 ].iBitmap = IMGL_DECLINE;
      buttons[ 2 ].idCommand = ID_PM_DECLINE;
      buttons[ 2 ].fsState = TBSTATE_ENABLED;
      buttons[ 2 ].fsStyle = style;
      buttons[ 2 ].iString = (INT_PTR) 0;

      buttons[ 3 ].iBitmap = IMGL_IGNORE;
      buttons[ 3 ].idCommand = ID_PM_IGNORE;
      buttons[ 3 ].fsState = TBSTATE_ENABLED;
      buttons[ 3 ].fsStyle = style;
      buttons[ 3 ].iString = (INT_PTR) 0;

      buttons[ 4 ].iBitmap = IMGL_UNIGNORE;
      buttons[ 4 ].idCommand = ID_PM_UNIGNORE;
      buttons[ 4 ].fsState = TBSTATE_ENABLED;
      buttons[ 4 ].fsStyle = style;
      buttons[ 4 ].iString = (INT_PTR) 0;

      buttons[ 5 ].iBitmap = IMGL_SAVE;
      buttons[ 5 ].idCommand = ID_PM_SAVE;
      buttons[ 5 ].fsState = TBSTATE_ENABLED;
      buttons[ 5 ].fsStyle = style;
      buttons[ 5 ].iString = (INT_PTR) 0;

      buttons[ 6 ].iBitmap = IMGL_VIEWLOG;
      buttons[ 6 ].idCommand = ID_PM_VIEWLOG;
      buttons[ 6 ].fsState = TBSTATE_ENABLED;
      buttons[ 6 ].fsStyle = style;
      buttons[ 6 ].iString = (INT_PTR) 0;

      buttons[ 7 ].iBitmap = IMGL_ADDBUDDY;
      buttons[ 7 ].idCommand = ID_PM_ADDBUDDY;
      buttons[ 7 ].fsState = TBSTATE_ENABLED;
      buttons[ 7 ].fsStyle = style;
      buttons[ 7 ].iString = (INT_PTR) 0;

      buttons[ 8 ].iBitmap = IMGL_RMBUDDY;
      buttons[ 8 ].idCommand = ID_PM_RMBUDDY;
      buttons[ 8 ].fsState = TBSTATE_ENABLED;
      buttons[ 8 ].fsStyle = style;
      buttons[ 8 ].iString = (INT_PTR) 0;

      SendMessage( m_wnd.ltoolbar, TB_ADDBUTTONS, 9, (LPARAM) buttons );
      SendMessage( m_wnd.ltoolbar, TB_AUTOSIZE, 0, 0 );
   }

   /* Set up the right toolbar. */
   {
      SendMessage( m_wnd.rtoolbar, TB_SETIMAGELIST, 0, (LPARAM) s_imgl_toolbar );
      SendMessage( m_wnd.rtoolbar, TB_SETDISABLEDIMAGELIST, 0, (LPARAM) s_imgl_toolbaroff );
      SendMessage( m_wnd.rtoolbar, TB_SETBUTTONSIZE, 0, MAKELONG( 16, 16 ) );

      const BYTE style = TBSTYLE_BUTTON;
      TBBUTTON buttons[ 1 ];
      memset( buttons, 0, sizeof( buttons ) );
      
      buttons[ 0 ].iBitmap = IMGL_SHOWRECIP;
      buttons[ 0 ].idCommand = ID_PM_SHOWRECIP;
      buttons[ 0 ].fsState = TBSTATE_ENABLED;
      buttons[ 0 ].fsStyle = style;
      buttons[ 0 ].iString = (INT_PTR) 0;

      SendMessage( m_wnd.rtoolbar, TB_ADDBUTTONS, 1, (LPARAM) buttons );
      SendMessage( m_wnd.ltoolbar, TB_AUTOSIZE, 0, 0 );
   }
   
   {
      SetWindowPos( m_wnd.wnd, NULL, x, y, width, height, SWP_NOZORDER );

      fitWindowElements( );
      refreshElements( );

      // Restore focus but keep at the top of the z-order.
      SetFocus( focus_wnd );
      SetWindowPos( m_wnd.wnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | 
                                                     SWP_NOACTIVATE | SWP_SHOWWINDOW );
   }
}

PmSession::~PmSession( )
{
   DestroyMenu( m_groups_menu );
   DestroyWindow( m_wnd.wnd );

   // Write the autolog.
   if (m_worth_saving && Eggy::singleton( )->getOptFlags( ) & OPT_AUTOLOG)
   {
      // Make sure the folder exists.
      create_folder_path( Eggy::singleton( )->getLogFolder( ) );

      string path = Eggy::singleton( )->getLogFile( );
      if (m_recipients.size( ) == 1 && Eggy::singleton( )->getOptFlags( ) & OPT_SEPARATELOGS)
         path = Eggy::singleton( )->getLogFolder( ) + "\\" + 
                to_path_friendly( pipes_to_spaces( m_recipients.front( )->contact->getName( ) ) ) + ".txt";

      HANDLE hf = CreateFile( path.c_str( ), GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
      if (hf != INVALID_HANDLE_VALUE)
      {
         char* sz_time = ctime( &m_created );
         sz_time[ strlen( sz_time ) - 1 ] = '\0';
         ostringstream ss;
         ss << "[Session created on " << sz_time << "]\r\n"
            << m_log.str( )
            << "\r\n\r\n";
         
         {
            SetFilePointer( hf, 0, NULL, FILE_END );
            string str = ss.str( );
            DWORD written;
            WriteFile( hf, str.c_str( ), str.length( ), &written, NULL );
         }
         CloseHandle( hf );
      }
      else
      {
         f2_errspeak( "Eggy: Couldn't write session to log." );
      }
   }

   while (!m_recipients.empty( ))
   {
      m_recipients.front( )->contact->release( );
      delete m_recipients.front( );
      m_recipients.pop_front( );
   }
}

bool PmSession::isRecipient( const string& name ) const {
   return findRecipient( name ) != NULL;
}

bool PmSession::addRecipient( const string& name )
{
   if (isRecipient( name ))
      return true;   

   // If the name exists in another PmSession, prompt the user before detaching.
   RECIPIENT* recip = new RECIPIENT( Eggy::singleton( )->contactCreate( name ) );
   if (!recip->contact)
   {
      delete recip;
      return false;
   }

   {
      PmSession* other_session = Eggy::singleton( )->pmFind( name );
      if (other_session)
      {
         // Scary!
         if (MessageBox( m_wnd.wnd, 
                         ("The recipient \"" + name + "\" already exists in another session window."
                         "\nDetach from the other session and move into this one?").c_str( ),
                         "Recipient Exists Elsewhere", 
                         MB_OKCANCEL | MB_ICONQUESTION ) != IDOK)
         {
            recip->contact->release( );
            delete recip;
            return false;
         }

         other_session->removeRecipient( name );
      }
   }
   
   m_recipients.push_back( recip );

   // Update the UI.
   {
      LVITEM item;
      makeListItem( recip, &item );
      ListView_InsertItem( m_wnd.recipients, &item );
      delete [] item.pszText;
   }
   refreshElements( );

   // Mention it.
   appendMsg( "Added <b>" + recip->contact->getName( ) + "</b> to the session.", 
              MSG_ICON_GENERAL );

   return true;
}

void PmSession::removeRecipient( const string& name )
{
   string shortname = make_shortname( name );

   for (list<RECIPIENT*>::iterator i = m_recipients.begin( );
        i != m_recipients.end( ); ++i)
   {
      if ((*i)->contact->getShortName( ) == shortname)
      {
         // Found it.
         RECIPIENT* recip = (*i);

         // Mention it.
         appendMsg( "Removed <b>" + recip->contact->getName( ) + "</b> from the session.", 
                    MSG_ICON_GENERAL );

         // Update the UI.
         ListView_DeleteItem( m_wnd.recipients, 
            ListView_FindFirstItemLParam( m_wnd.recipients, (LPARAM) recip ) );

         // Remove.
         m_recipients.erase( i );
         recip->contact->release( );
         delete recip;

         refreshElements( );

         return;
      }
   }
}

bool PmSession::detachRecipient( const string& name )
{
   if (m_recipients.size( ) <= 1)
      return false;

   RECIPIENT* recip = findRecipient( name );
   if (!recip)
      return false;

   // Don't let the contact expire.
   Contact* contact = recip->contact;
   contact->addRef( );
   removeRecipient( name );

   // Create a new PM session for the contact.
   PmSession* session = Eggy::singleton( )->pmCreate( name );

   contact->release( );

   return session != NULL;
}

PmSession::RECIPIENT* PmSession::findRecipient( const string& name )
{
   const string shortname = make_shortname( name );

   for (list<RECIPIENT*>::iterator i = m_recipients.begin( );
        i != m_recipients.end( ); ++i)
   {
      if ((*i)->contact->getShortName( ) == shortname)
         return (*i);
   }

   return NULL;
}

const PmSession::RECIPIENT* PmSession::findRecipient( const string& name ) const
{
   const string shortname = make_shortname( name );

   for (list<RECIPIENT*>::const_iterator i = m_recipients.begin( );
        i != m_recipients.end( ); ++i)
   {
      if ((*i)->contact->getShortName( ) == shortname)
         return (*i);
   }

   return NULL;
}

void PmSession::showRecipientsPane( bool onoff )
{
   if (m_recipients_visible && !onoff)
   {
      m_recipients_visible = false;

      // Hide the pane and its controls.
      RECT r;
      GetClientRect( m_wnd.wnd, &r );
      r.right = _SAT( r.right - DLG_RECIPPANE_W );
      AdjustWindowRectEx( &r, GetWindowStyle( m_wnd.wnd ), 
                          FALSE, GetWindowExStyle( m_wnd.wnd ) );

      SetWindowPos( m_wnd.wnd, NULL, 0, 0,
                    r.right - r.left, r.bottom - r.top, 
                    SWP_NOMOVE | SWP_NOZORDER );

      ShowWindow( m_wnd.recipients_label, SW_HIDE );
      ShowWindow( m_wnd.recipients, SW_HIDE );
   }
   else if (!m_recipients_visible && onoff)
   {
      m_recipients_visible = true;

      // Show the pane and its controls.
      RECT r;
      GetClientRect( m_wnd.wnd, &r );
      r.right = r.right + DLG_RECIPPANE_W;
      AdjustWindowRectEx( &r, GetWindowStyle( m_wnd.wnd ), 
                          FALSE, GetWindowExStyle( m_wnd.wnd ) );

      SetWindowPos( m_wnd.wnd, NULL, 0, 0,
                    r.right - r.left, r.bottom - r.top, 
                    SWP_NOMOVE | SWP_NOZORDER );

      ShowWindow( m_wnd.recipients_label, SW_SHOW );
      ShowWindow( m_wnd.recipients, SW_SHOW );
   }
}

void PmSession::raise( ) 
{
   ShowWindow( m_wnd.wnd, SW_SHOWNORMAL );

   // Make sure the foreground window isn't fullscreened.
   {
      int fsx = GetSystemMetrics( SM_CXFULLSCREEN ), fsy = GetSystemMetrics( SM_CYFULLSCREEN );
      HWND forewnd = GetForegroundWindow( );
      RECT forewndrect;
      GetWindowRect( forewnd, &forewndrect );
      if (forewndrect.right - forewndrect.left < fsx || forewndrect.bottom - forewndrect.top < fsy)
         // Not fullscreened.
         SetForegroundWindow( m_wnd.wnd );
   }
}

void PmSession::makeListItem( RECIPIENT* in, LVITEM* out ) const
{
   out->mask    = LVIF_IMAGE | LVIF_PARAM | LVIF_TEXT;
   out->lParam  = (LPARAM) in;
   out->iImage  = in->contact->getOnline( ) ? IMGL_CONTACT : IMGL_CONTACT_OFF;
   out->iSubItem = 0;
   out->iItem   = ListView_GetItemCount( m_wnd.recipients );

   const string display_name = xml_charset::ascii_to_latin( pipes_to_spaces( in->contact->getName( ) ) );
   strcpy( out->pszText = new char[ display_name.length( ) + 1 ], display_name.c_str( ) );
}

void PmSession::fitWindowElements( )
{
   SIZE cs;
   {
      RECT cr;
      GetClientRect( m_wnd.wnd, &cr );
      cs.cx = cr.right;
      cs.cy = cr.bottom;

      // Don't count the recipients pane.
      if (m_recipients_visible)
         cs.cx = _SAT( cs.cx - DLG_RECIPPANE_W );
   }

   struct { int x, y, w, h; }
      chat_xy, input_xy, recipients_xy, recipients_label_xy, 
      send_xy, status_xy, ltoolbar_xy, rtoolbar_xy;

   status_xy.x = DLG_PADDING; status_xy.y = DLG_PADDING;
   status_xy.w = _SAT( cs.cx - (2*DLG_PADDING) );
   status_xy.h = 14;

   send_xy.w = 64; send_xy.h = 48;
   send_xy.x = _SAT( cs.cx - DLG_PADDING - send_xy.w ); 
   send_xy.y = _SAT( cs.cy - DLG_PADDING - send_xy.h );

   input_xy.x = DLG_PADDING;
   input_xy.w = _SAT( send_xy.x - DLG_PADDING - input_xy.x );
   input_xy.h = send_xy.h;
   input_xy.y = _SAT( cs.cy - DLG_PADDING - input_xy.h );

   rtoolbar_xy.w = 24; rtoolbar_xy.h = 24;
   rtoolbar_xy.x = _SAT( cs.cx - DLG_PADDING - rtoolbar_xy.w );
   rtoolbar_xy.y = _SAT( input_xy.y - rtoolbar_xy.h - 1 );

   ltoolbar_xy.x = DLG_PADDING;
   ltoolbar_xy.w = _SAT( rtoolbar_xy.x - ltoolbar_xy.x );
   ltoolbar_xy.h = rtoolbar_xy.h;
   ltoolbar_xy.y = _SAT( input_xy.y - ltoolbar_xy.h - 1 );

   chat_xy.x = DLG_PADDING; 
   chat_xy.y = status_xy.y + status_xy.h + DLG_PADDING;
   chat_xy.w = _SAT( cs.cx - (2*DLG_PADDING) );
   chat_xy.h = _SAT( ltoolbar_xy.y - chat_xy.y - 1 ); 

   recipients_label_xy.x = cs.cx; recipients_label_xy.y = DLG_PADDING;
   recipients_label_xy.w = (cs.cx + DLG_RECIPPANE_W) - DLG_PADDING - recipients_label_xy.x;
   recipients_label_xy.h = status_xy.h;

   recipients_xy.x = cs.cx;
   recipients_xy.y = recipients_label_xy.y + recipients_label_xy.h + DLG_PADDING;
   recipients_xy.w = (cs.cx + DLG_RECIPPANE_W) - DLG_PADDING - recipients_label_xy.x;
   recipients_xy.h = _SAT( cs.cy - DLG_PADDING - recipients_xy.y );
   
   SetWindowPos( m_wnd.status, NULL, status_xy.x, status_xy.y,
                 status_xy.w, status_xy.h, SWP_NOZORDER );
   SetWindowPos( m_wnd.chat, NULL, chat_xy.x, chat_xy.y,
                 chat_xy.w, chat_xy.h, SWP_NOZORDER );
   SetWindowPos( m_wnd.ltoolbar, NULL, ltoolbar_xy.x, ltoolbar_xy.y,
                 ltoolbar_xy.w, ltoolbar_xy.h, SWP_NOZORDER );
   SetWindowPos( m_wnd.rtoolbar, NULL, rtoolbar_xy.x, rtoolbar_xy.y,
                 rtoolbar_xy.w, rtoolbar_xy.h, SWP_NOZORDER );
   SetWindowPos( m_wnd.input, NULL, input_xy.x, input_xy.y,
                 input_xy.w, input_xy.h, SWP_NOZORDER );
   SetWindowPos( m_wnd.chat, NULL, chat_xy.x, chat_xy.y,
                 chat_xy.w, chat_xy.h, SWP_NOZORDER );
   SetWindowPos( m_wnd.send, NULL, send_xy.x, send_xy.y,
                 send_xy.w, send_xy.h, SWP_NOZORDER );
   SetWindowPos( m_wnd.recipients_label, NULL, recipients_label_xy.x, recipients_label_xy.y,
                 recipients_label_xy.w, recipients_label_xy.h, SWP_NOZORDER );
   SetWindowPos( m_wnd.recipients, NULL, recipients_xy.x, recipients_xy.y,
                 recipients_xy.w, recipients_xy.h, SWP_NOZORDER );

   // Resize the listview column.
   ListView_SetColumnWidth( m_wnd.recipients, 0, recipients_xy.w );
}

string PmSession::getTitle( ) const
{
   PLAYERINFO pi;
   f2_myplayerinfo( &pi );
   ostringstream ss;

   if (m_recipients.size( ) == 1)
      ss << pipes_to_spaces( m_recipients.front( )->contact->getName( ) )
      << " - " << (!*pi.name ? "MissingNo." : pi.name);
   else
      ss << "(" << m_recipients.size( ) << ") Players" 
      << " - " << (!*pi.name ? "MissingNo." : pi.name);

   return ss.str( );
}

string PmSession::getHalfTitle( ) const
{
   ostringstream ss;

   if (m_recipients.size( ) == 1)
      ss << pipes_to_spaces( m_recipients.front( )->contact->getName( ) );
   else
      ss << "(" << m_recipients.size( ) << ") Players";

   return ss.str( );
}

void PmSession::refreshElements( )
{   
   // Update the window title.
   SetWindowText( m_wnd.wnd, getTitle( ).c_str( ) );

   // Update the status message.
   {
      ostringstream ss;

      if (m_last_received)
      {
         tm* ltime = localtime( &m_last_received );
         if (ltime->tm_hour % 12)
            ss << ltime->tm_hour % 12;
         else
            ss << 12;

         if (ltime->tm_min < 10)
            ss << ":0" << ltime->tm_min;
         else
            ss << ":" << ltime->tm_min;

         ss << (ltime->tm_hour >= 12 ? " PM" : " AM");
      }
       
      ss << " <-> ";

      if (m_last_sent)
      {
         tm* ltime = localtime( &m_last_sent );
         if (ltime->tm_hour % 12)
            ss << ltime->tm_hour % 12;
         else
            ss << 12;

         if (ltime->tm_min < 10)
            ss << ":0" << ltime->tm_min;
         else
            ss << ":" << ltime->tm_min;

         ss << (ltime->tm_hour >= 12 ? " PM" : " AM");
      }

      SetWindowText( m_wnd.status, ss.str( ).c_str( ) );
   }
}

void PmSession::onContact( const string& name, On_Contact how )
{
   RECIPIENT* recip = findRecipient( name );
   if (!recip)
      return;

   if (how == CONTACT_ONLINE || how == CONTACT_NAME)
   {
      // Update the UI.
      LVITEM item;
      makeListItem( recip, &item );
      item.iItem = ListView_FindFirstItemLParam( m_wnd.recipients, (LPARAM) recip );
      ListView_SetItem( m_wnd.recipients, &item );
      delete [] item.pszText;

      if (how == CONTACT_ONLINE)
      {
         // Reset error.
         recip->hopeless = false;
            
         // Mention it.
         if (recip->contact->getOnline( ))
            appendMsg( "<b>" + pipes_to_spaces( name ) + "</b> is online.", MSG_ICON_ONLINE );
         else
            appendMsg( "<b>" + pipes_to_spaces( name ) + "</b> is offline.", MSG_ICON_OFFLINE );
      }
   }
}

void PmSession::onLoggedIn( )
{
   refreshElements( );

   // Mention it.
   appendMsg( "Logged into server.", MSG_ICON_GENERAL );
}

void PmSession::onDisconnected( )
{
   refreshElements( );

   // Mention it.
   appendMsg( "Disconnected from server.", MSG_ICON_GENERAL );
}

bool PmSession::onWhisperTo( const string& to, const string& msg )
{
   /* PM targets are always surrounded in quotes. */
   if (to[ 0 ] == '"' && to[ to.length( ) - 1 ] == '"')
   {
      RECIPIENT* recip = findRecipient( to );
      if (recip)
      {
         recip->last_sent = msg;

         /* Only echo these if there's zero or more than one recipient. */
         if (m_recipients.size( ) != 1)
         {
            /* When everyone in this session is sent the same message, display it. */
            list<RECIPIENT*>::iterator i = m_recipients.begin( );
            for (; i != m_recipients.end( ); ++i)
            {
               if ((*i)->last_sent != msg)
                  break;
            }

            if (i == m_recipients.end( ))
            {
               PLAYERINFO pi;
               f2_myplayerinfo( &pi );

               /* If the message begins with a :, it's an emote. */
               if (msg[ 0 ] == ':')
               {
                  if (isalnum( msg[ 1 ] ))
                     appendEmote( pi.name, " " + msg.substr( 1, msg.npos ) );
                  else
                     appendEmote( pi.name, msg.substr( 1, msg.npos ) );
               }
               else
                  appendMyChat( pi.name, msg );

               // Reset all the last_sents.
               for (i = m_recipients.begin( ); i != m_recipients.end( ); ++i)
                  (*i)->last_sent.clear( );

               m_last_sent = time( NULL );
               refreshElements( );
            }   

            return true;
         }
      }
   }

   return false;
}

bool PmSession::onWhisperEcho( const string& to, const string& msg )
{
   /* Always handle these, but only use them for displays when there's
      only one recipient, otherwise use whispertos. */
   RECIPIENT* recip = findRecipient( to );
   if (recip)
   {
      if (m_recipients.size( ) == 1)
      {
         PLAYERINFO pi;
         f2_myplayerinfo( &pi );

         /* If the whisper starts with the player's name, then it's an emote. */
         if (!_strnicmp( msg.c_str( ), pi.name, strlen( pi.name ) ))
            appendEmote( pi.name, msg.substr( strlen( pi.name ), msg.npos ) );
         else
            appendMyChat( pi.name, msg );

         m_last_sent = time( NULL );
         refreshElements( );

         /* Not really reliable to compare the text of the whisper to what was in the outbox
            since other modules/scripts could modify it, so just cross your fingers and
            clear the outbox. */
         m_recipients.front( )->last_sent.clear( );
      }

      return true;
   }

   return false;
}

/* Vague because this error also occurs for summon/joins. */
bool PmSession::onWhisperFail( const string& name )
{
   /* PM targets are always surrounded in quotes. */
   if (name[ 0 ] == '"' && name[ name.length( ) - 1 ] == '"')
   {
      /* If there's more than one target in this session, only
         display failure messages once. */
      RECIPIENT* recip = findRecipient( name );
      if (recip)
      {
         if (m_recipients.size( ) == 1)
         {
            if (recip->last_sent.length( ))
            {
            
               if (!recip->hopeless)
               {
                  if (Eggy::singleton( )->getOptFlags( ) & OPT_AUTOOFFLINE)
                  {
                     appendMsg( "Delivery Error: <b>" + recip->contact->getName( )
                                + "</b> isn't online. Messages will be delivered when the user is next seen.", MSG_ICON_ERROR );
                  }
                  else
                  {
                     appendMsg( "Delivery Error: <b>" + recip->contact->getName( )
                                + "</b> isn't online.", MSG_ICON_ERROR );
                  }
                  recip->hopeless = true;
               }

               // Show the offline message.
               if (Eggy::singleton( )->getOptFlags( ) & OPT_AUTOOFFLINE)
               {
                  PLAYERINFO pi;
                  f2_myplayerinfo( &pi );

                  if (recip->last_sent[ 0 ] != ':')
                     appendOfflineChat( pi.name, recip->last_sent );
                  else
                     appendOfflineChat( pi.name, pi.name + (" " + recip->last_sent.substr( 1 )) );

                  Eggy::singleton( )->getOfflineMessages( )->add( pi.name, recip->contact->getName( ), recip->last_sent );
               }
            }

            recip->last_sent.clear( );
         }
         else
         {
            assert( m_recipients.size( ) );
            if (!recip->hopeless)
            {
               appendMsg( "Delivery Error: <b>" + recip->contact->getName( )
                  + "</b> isn't online.", MSG_ICON_ERROR );
               recip->hopeless = true;
            }
         }

         return true;
      }
   }

   return false;
}

bool PmSession::onWhisperDenied( const string& name )
{
   /* If from anyone in this session, display it. */
   RECIPIENT* recip = findRecipient( name );
   if (recip)
   {
      appendMsg( "<b>" + name + "</b> isn't accepting whispers.", MSG_ICON_ERROR );
      return true;
   }

   return false;
}

bool PmSession::onWhisperFrom( const string& from, const string& msg )
{
   /* If from anyone in this session, display it. */
   if (isRecipient( from ))
   {
      /* If the whisper starts with the player's name, then it's an emote. */
      if (!msg.compare( 0, from.length( ), from ))
         appendEmote( from, msg.substr( from.length( ), msg.npos ) );
      else
         appendChat( from, msg );

      m_last_received = time( NULL );
      refreshElements( );

      return true;
   }

   return false;
}

bool PmSession::onSummonJoinEcho( const string& name )
{
   /* If to anyone in this session, display it. */
   if (isRecipient( name ))
   {
      appendMsg( "Your request has been sent to <b>" + name + "</b>.", MSG_ICON_GENERAL );
      return true;
   }
   return false;
}

bool PmSession::onJoin( const string& name )
{
   /* If from anyone in this session, display it. */
   if (isRecipient( name ))
   {
      appendMsg( "<b>" + name + "</b> asks to join you.", MSG_ICON_GENERAL );
      return true;
   }
   return false;
}

bool PmSession::onSummon( const string& name )
{
   /* If from anyone in this session, display it. */
   if (isRecipient( name ))
   {
      appendMsg( "<b>" + name + "</b> asks to summon you.", MSG_ICON_GENERAL );
      return true;
   }
   return false;
}

bool PmSession::onJoined( const string& name )
{
   /* If from anyone in this session, display it. */
   if (isRecipient( name ))
   {
      appendMsg( "<b>" + name + "</b> joins you.", MSG_ICON_GENERAL );
      return true;
   }
   return false;
}

bool PmSession::onSummoned( const string& name )
{
   /* If from anyone in this session, display it. */
   if (isRecipient( name ))
   {
      appendMsg( "<b>" + name + "</b> summons you.", MSG_ICON_GENERAL );
      return true;
   }
   return false;
}

bool PmSession::onDeclined( const string& name )
{
   /* If from anyone in this session, display it. */
   if (isRecipient( name ))
   {
      appendMsg( "<b>" + name + "</b> declines your request.", MSG_ICON_GENERAL );
      return true;
   }
   return false;
}

bool PmSession::onDeclineEcho( const string& name )
{
   /* If to anyone in this session, display it. */
   if (isRecipient( name ))
   {
      appendMsg( "You decline <b>" + name + "</b>'s request.", MSG_ICON_GENERAL );
      return true;
   }
   return false;
}

void PmSession::appendChat( const string& name, const string& msg )
{
   // Batch elements for better performance.
   vector<PTBELEM> elems;
   PTBELEM elem;

   // Insert a linebreak.
   if (GetWindowTextLength( m_wnd.chat ))
   {
      elem.type     = PTB_ELEM_TYPE_STR;
      elem.style    = PTB_ELEM_STYLE_NORMAL;
      elem.text     = to_unicode( "\n" ); // Yes, this is a bit silly.
      elem.alt_text = NULL;
      elem.param    = 0;
      elems.push_back( elem );

      // Log.
      m_log << "\r\n";
   }

   /* Treat the name differently from the message. */
   {
      ChatLexer lex( name.c_str( ), name.length( ) );
      while (!lex.eof( ))
      {
         switch (lex.next( ))
         {
         case ChatLexer::TOK_TEXT:
         case ChatLexer::TOK_SMILEY:
         case ChatLexer::TOK_DESCTAG:
            if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
            {
               const unsigned smiley_index = to_smiley_index( lex.tokPtr( )[ 2 ] );

               assert( smiley_index < ChatLexer::NUM_SMILEYS );

               if (sa_imgl_smileyicons[ smiley_index ])
               {
                  elem.type = PTB_ELEM_TYPE_IMG;
                  elem.imglist = sa_imgl_smileyicons[ smiley_index ];
                  elem.img_index   = 0;
                  elem.text        = NULL;
               }
               else
               {
                  elem.type = PTB_ELEM_TYPE_STR;
                  elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
               }
            }
            else
            {
               elem.type  = PTB_ELEM_TYPE_STR;
               elem.text  = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
            }

            elem.style    = PTB_ELEM_STYLE_BOLD;
            elem.color    = CHAT_COLOR_NAME;
            elem.param    = 0;
            if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
               elem.alt_text = to_unicode( lex.tokPtr( ), lex.tokLen( ) );
            else
               elem.alt_text = NULL;
            
            elems.push_back( elem );

            // Log.
            m_log.write( lex.tokPtr( ), lex.tokLen( ) );
         }
      }

      elem.type  = PTB_ELEM_TYPE_STR;
      elem.text  = to_unicode( ": " );
      elem.style    = PTB_ELEM_STYLE_BOLD;
      elem.color    = CHAT_COLOR_NORMAL;
      elem.param    = 0;
      elem.alt_text = NULL;      
      elems.push_back( elem );

      // Log.
      m_log << ": ";
   }
   /* Do the message. */
   {
      ChatLexer lex( msg.c_str( ), msg.length( ) );
      while (!lex.eof( ))
      {
         switch (lex.next( ))
         {
         case ChatLexer::TOK_TEXT:
         case ChatLexer::TOK_SMILEY:
         case ChatLexer::TOK_DESCTAG:
         case ChatLexer::TOK_IMG:
            if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
            {
               const unsigned smiley_index = to_smiley_index( lex.tokPtr( )[ 2 ] );

               assert( smiley_index < ChatLexer::NUM_SMILEYS );

               if (sa_imgl_smileyicons[ smiley_index ])
               {
                  elem.type = PTB_ELEM_TYPE_IMG;
                  elem.imglist = sa_imgl_smileyicons[ smiley_index ];
                  elem.img_index   = 0;
                  elem.text        = NULL;
               }
               else
               {
                  elem.type = PTB_ELEM_TYPE_STR;
                  elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
               }
            }
            else if (lex.tokType( ) == ChatLexer::TOK_IMG)
            {
               // Only allow local files.
               HIMAGELIST imgl = NULL;
               if (lex.tokImgSrc( ).compare( 0, 8, "file:///" ) == 0 && (imgl = imgl_from_file( lex.tokImgSrc( ).substr( 8 ) )))
               {
                  elem.type = PTB_ELEM_TYPE_IMG;
                  elem.imglist = imgl;
                  elem.img_index = 0;
                  elem.text = NULL;
               }
               else
               {
                  elem.type = PTB_ELEM_TYPE_STR;
                  elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
               }
            }
            else
            {
               elem.type  = PTB_ELEM_TYPE_STR;
               elem.text  = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
            }

            if (lex.tokStyle( ) & ChatLexer::STYLE_LINK)
            {
               string href = lex.tokHref( );
               if (_strnicmp( href.c_str( ), "http://", 7 ) &&
                   _strnicmp( href.c_str( ), "https://", 8 ) &&
                   _strnicmp( href.c_str( ), "ftp://", 6 ) &&
                   _strnicmp( href.c_str( ), "furc://", 7 ) &&
                   _strnicmp( href.c_str( ), "name://", 7 ) &&
                   _strnicmp( href.c_str( ), "recip:", 6 ) )
               {
                  href = "http://" + href;
               }
               m_links.push_back( href );

               elem.style    = PTB_ELEM_STYLE_UNDERLINE;
               elem.color    = CHAT_COLOR_LINK;
               elem.param    = m_links.size( ); // 1-based.
               elem.alt_text = to_unicode( href.c_str( ) );
            }
            else
            {
               elem.style    = 0;
               elem.color    = CHAT_COLOR_NORMAL;
               elem.param    = 0;
               if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
                  elem.alt_text = to_unicode( lex.tokPtr( ), lex.tokLen( ) );
               else
                  elem.alt_text = NULL;
            }

            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_BOLD ? PTB_ELEM_STYLE_BOLD : 0;
            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_ITALIC ? PTB_ELEM_STYLE_ITALIC : 0;
            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_UNDERLINE ? PTB_ELEM_STYLE_UNDERLINE : 0;
            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_LINK ? PTB_ELEM_STYLE_LINK : 0;
            
            elems.push_back( elem );

            // Log.
            m_log.write( lex.tokPtr( ), lex.tokLen( ) );
         }
      }
   }

   PoorTextBox_Append( m_wnd.chat, elems.size( ), &elems.front( ) );

   while (!elems.empty( ))
   {
      delete [] elems.back( ).text;
      delete [] elems.back( ).alt_text;
      elems.pop_back( );
   }

   m_worth_saving = true;

   if (GetForegroundWindow( ) != m_wnd.wnd)
      FlashWindow( m_wnd.wnd, TRUE );
}

void PmSession::appendMyChat( const string& name, const string& msg )
{
   // Batch elements for better performance.
   vector<PTBELEM> elems;
   PTBELEM elem;

   // Insert a linebreak.
   if (GetWindowTextLength( m_wnd.chat ))
   {
      elem.type     = PTB_ELEM_TYPE_STR;
      elem.style    = PTB_ELEM_STYLE_NORMAL;
      elem.text     = to_unicode( "\n" ); // Yes, this is a bit silly.
      elem.alt_text = NULL;
      elem.param    = 0;
      elems.push_back( elem );

      // Log.
      m_log << "\r\n";
   }

   /* Treat the name differently from the message. */
   {
      ChatLexer lex( name.c_str( ), name.length( ) );
      while (!lex.eof( ))
      {
         switch (lex.next( ))
         {
         case ChatLexer::TOK_TEXT:
         case ChatLexer::TOK_SMILEY:
         case ChatLexer::TOK_DESCTAG:
            if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
            {
               const unsigned smiley_index = to_smiley_index( lex.tokPtr( )[ 2 ] );

               assert( smiley_index < ChatLexer::NUM_SMILEYS );

               if (sa_imgl_smileyicons[ smiley_index ])
               {
                  elem.type = PTB_ELEM_TYPE_IMG;
                  elem.imglist = sa_imgl_smileyicons[ smiley_index ];
                  elem.img_index   = 0;
                  elem.text        = NULL;
               }
               else
               {
                  elem.type = PTB_ELEM_TYPE_STR;
                  elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
               }
            }
            else
            {
               elem.type  = PTB_ELEM_TYPE_STR;
               elem.text  = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
            }

            elem.style    = PTB_ELEM_STYLE_BOLD;
            elem.color    = CHAT_COLOR_MYNAME;
            elem.param    = 0;
            if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
               elem.alt_text = to_unicode( lex.tokPtr( ), lex.tokLen( ) );
            else
               elem.alt_text = NULL;
            
            elems.push_back( elem );

            // Log.
            m_log.write( lex.tokPtr( ), lex.tokLen( ) );
         }
      }

      elem.type  = PTB_ELEM_TYPE_STR;
      elem.text  = to_unicode( ": " );
      elem.style    = PTB_ELEM_STYLE_BOLD;
      elem.color    = CHAT_COLOR_NORMAL;
      elem.param    = 0;
      elem.alt_text = NULL;      
      elems.push_back( elem );

      // Log.
      m_log << ": ";
   }
   /* Do the message. */
   {
      ChatLexer lex( msg.c_str( ), msg.length( ) );
      while (!lex.eof( ))
      {
         switch (lex.next( ))
         {
         case ChatLexer::TOK_TEXT:
         case ChatLexer::TOK_SMILEY:
         case ChatLexer::TOK_DESCTAG:
         case ChatLexer::TOK_IMG:
            if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
            {
               const unsigned smiley_index = to_smiley_index( lex.tokPtr( )[ 2 ] );

               assert( smiley_index < ChatLexer::NUM_SMILEYS );

               if (sa_imgl_smileyicons[ smiley_index ])
               {
                  elem.type = PTB_ELEM_TYPE_IMG;
                  elem.imglist = sa_imgl_smileyicons[ smiley_index ];
                  elem.img_index   = 0;
                  elem.text        = NULL;
               }
               else
               {
                  elem.type = PTB_ELEM_TYPE_STR;
                  elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
               }
            }
            else if (lex.tokType( ) == ChatLexer::TOK_IMG)
            {
               // Only allow local files.
               HIMAGELIST imgl = NULL;
               if (lex.tokImgSrc( ).compare( 0, 8, "file:///" ) == 0 && (imgl = imgl_from_file( lex.tokImgSrc( ).substr( 8 ) )))
               {
                  elem.type = PTB_ELEM_TYPE_IMG;
                  elem.imglist = imgl;
                  elem.img_index = 0;
                  elem.text = NULL;
               }
               else
               {
                  elem.type = PTB_ELEM_TYPE_STR;
                  elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
               }
            }
            else
            {
               elem.type  = PTB_ELEM_TYPE_STR;
               elem.text  = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
            }

            if (lex.tokStyle( ) & ChatLexer::STYLE_LINK)
            {
               string href = lex.tokHref( );
               if (_strnicmp( href.c_str( ), "http://", 7 ) &&
                   _strnicmp( href.c_str( ), "https://", 8 ) &&
                   _strnicmp( href.c_str( ), "ftp://", 6 ) &&
                   _strnicmp( href.c_str( ), "furc://", 7 ) &&
                   _strnicmp( href.c_str( ), "recip:", 6 ) )
               {
                  href = "http://" + href;
               }
               m_links.push_back( href );

               elem.style    = PTB_ELEM_STYLE_UNDERLINE;
               elem.color    = CHAT_COLOR_LINK;
               elem.param    = m_links.size( ); // 1-based.
               elem.alt_text = to_unicode( href.c_str( ) );
            }
            else
            {
               elem.style    = 0;
               elem.color    = CHAT_COLOR_NORMAL;
               elem.param    = 0;
               if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
                  elem.alt_text = to_unicode( lex.tokPtr( ), lex.tokLen( ) );
               else
                  elem.alt_text = NULL;
            }

            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_BOLD ? PTB_ELEM_STYLE_BOLD : 0;
            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_ITALIC ? PTB_ELEM_STYLE_ITALIC : 0;
            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_UNDERLINE ? PTB_ELEM_STYLE_UNDERLINE : 0;
            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_LINK ? PTB_ELEM_STYLE_LINK : 0;
            
            elems.push_back( elem );

            // Log.
            m_log.write( lex.tokPtr( ), lex.tokLen( ) );
         }
      }
   }

   PoorTextBox_Append( m_wnd.chat, elems.size( ), &elems.front( ) );

   while (!elems.empty( ))
   {
      delete [] elems.back( ).text;
      delete [] elems.back( ).alt_text;
      elems.pop_back( );
   }

   m_worth_saving = true;

   if (GetForegroundWindow( ) != m_wnd.wnd)
      FlashWindow( m_wnd.wnd, TRUE );
}

void PmSession::appendOfflineChat( const string& name, const string& msg )
{
   // Batch elements for better performance.
   vector<PTBELEM> elems;
   PTBELEM elem;

   // Insert a linebreak.
   if (GetWindowTextLength( m_wnd.chat ))
   {
      elem.type     = PTB_ELEM_TYPE_STR;
      elem.style    = PTB_ELEM_STYLE_NORMAL;
      elem.text     = to_unicode( "\n" ); // Yes, this is a bit silly.
      elem.alt_text = NULL;
      elem.param    = 0;
      elems.push_back( elem );

      // Log.
      m_log << "\r\n";
   }

   /* Treat the name differently from the message. */
   {
      ChatLexer lex( name.c_str( ), name.length( ) );
      while (!lex.eof( ))
      {
         switch (lex.next( ))
         {
         case ChatLexer::TOK_TEXT:
         case ChatLexer::TOK_SMILEY:
         case ChatLexer::TOK_DESCTAG:
            if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
            {
               const unsigned smiley_index = to_smiley_index( lex.tokPtr( )[ 2 ] );

               assert( smiley_index < ChatLexer::NUM_SMILEYS );

               if (sa_imgl_smileyicons[ smiley_index ])
               {
                  elem.type = PTB_ELEM_TYPE_IMG;
                  elem.imglist = sa_imgl_smileyicons[ smiley_index ];
                  elem.img_index   = 0;
                  elem.text        = NULL;
               }
               else
               {
                  elem.type = PTB_ELEM_TYPE_STR;
                  elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
               }
            }
            else
            {
               elem.type  = PTB_ELEM_TYPE_STR;
               elem.text  = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
            }

            elem.style    = PTB_ELEM_STYLE_BOLD;
            elem.color    = CHAT_COLOR_MYNAME;
            elem.param    = 0;
            if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
               elem.alt_text = to_unicode( lex.tokPtr( ), lex.tokLen( ) );
            else
               elem.alt_text = NULL;
            
            elems.push_back( elem );

            // Log.
            m_log.write( lex.tokPtr( ), lex.tokLen( ) );
         }
      }

      elem.type  = PTB_ELEM_TYPE_STR;
      elem.text  = to_unicode( ": " );
      elem.style    = PTB_ELEM_STYLE_BOLD;
      elem.color    = CHAT_COLOR_NORMAL;
      elem.param    = 0;
      elem.alt_text = NULL;      
      elems.push_back( elem );

      // Log.
      m_log << ": ";
   }
   /* Do the message. */
   {
      ChatLexer lex( msg.c_str( ), msg.length( ) );
      while (!lex.eof( ))
      {
         switch (lex.next( ))
         {
         case ChatLexer::TOK_TEXT:
         case ChatLexer::TOK_SMILEY:
         case ChatLexer::TOK_DESCTAG:
         case ChatLexer::TOK_IMG:
            if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
            {
               const unsigned smiley_index = to_smiley_index( lex.tokPtr( )[ 2 ] );

               assert( smiley_index < ChatLexer::NUM_SMILEYS );

               if (sa_imgl_smileyicons[ smiley_index ])
               {
                  elem.type = PTB_ELEM_TYPE_IMG;
                  elem.imglist = sa_imgl_smileyicons[ smiley_index ];
                  elem.img_index   = 0;
                  elem.text        = NULL;
               }
               else
               {
                  elem.type = PTB_ELEM_TYPE_STR;
                  elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
               }
            }
            else if (lex.tokType( ) == ChatLexer::TOK_IMG)
            {
               // Only allow local files.
               HIMAGELIST imgl = NULL;
               if (lex.tokImgSrc( ).compare( 0, 8, "file:///" ) == 0 && (imgl = imgl_from_file( lex.tokImgSrc( ).substr( 8 ) )))
               {
                  elem.type = PTB_ELEM_TYPE_IMG;
                  elem.imglist = imgl;
                  elem.img_index = 0;
                  elem.text = NULL;
               }
               else
               {
                  elem.type = PTB_ELEM_TYPE_STR;
                  elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
               }
            }
            else
            {
               elem.type  = PTB_ELEM_TYPE_STR;
               elem.text  = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
            }

            if (lex.tokStyle( ) & ChatLexer::STYLE_LINK)
            {
               string href = lex.tokHref( );
               if (_strnicmp( href.c_str( ), "http://", 7 ) &&
                   _strnicmp( href.c_str( ), "https://", 8 ) &&
                   _strnicmp( href.c_str( ), "ftp://", 6 ) &&
                   _strnicmp( href.c_str( ), "furc://", 7 ) &&
                   _strnicmp( href.c_str( ), "recip:", 6 ))
               {
                  href = "http://" + href;
               }
               m_links.push_back( href );

               elem.style    = PTB_ELEM_STYLE_UNDERLINE;
               elem.color    = CHAT_COLOR_LINK;
               elem.param    = m_links.size( ); // 1-based.
               elem.alt_text = to_unicode( href.c_str( ) );
            }
            else
            {
               elem.style    = 0;
               elem.color    = CHAT_COLOR_OFFLINE;
               elem.param    = 0;
               if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
                  elem.alt_text = to_unicode( lex.tokPtr( ), lex.tokLen( ) );
               else
                  elem.alt_text = NULL;
            }

            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_BOLD ? PTB_ELEM_STYLE_BOLD : 0;
            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_ITALIC ? PTB_ELEM_STYLE_ITALIC : 0;
            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_UNDERLINE ? PTB_ELEM_STYLE_UNDERLINE : 0;
            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_LINK ? PTB_ELEM_STYLE_LINK : 0;
            
            elems.push_back( elem );

            // Log.
            m_log.write( lex.tokPtr( ), lex.tokLen( ) );
         }
      }
   }

   PoorTextBox_Append( m_wnd.chat, elems.size( ), &elems.front( ) );

   while (!elems.empty( ))
   {
      delete [] elems.back( ).text;
      delete [] elems.back( ).alt_text;
      elems.pop_back( );
   }

   m_worth_saving = true;

   if (GetForegroundWindow( ) != m_wnd.wnd)
      FlashWindow( m_wnd.wnd, TRUE );
}

void PmSession::appendEmote( const string& name, const string& msg )
{
   // Batch elements for better performance.
   vector<PTBELEM> elems;
   PTBELEM elem;

   // Insert a linebreak.
   if (GetWindowTextLength( m_wnd.chat ))
   {
      elem.type     = PTB_ELEM_TYPE_STR;
      elem.style    = PTB_ELEM_STYLE_NORMAL;
      elem.text     = to_unicode( "\n" ); // Yes, this is a bit silly.
      elem.alt_text = NULL;
      elem.param    = 0;
      elems.push_back( elem );

      // Log.
      m_log << "\r\n";
   }

   /* Treat the name differently from the message. */
   {
      ChatLexer lex( name.c_str( ), name.length( ) );
      while (!lex.eof( ))
      {
         switch (lex.next( ))
         {
         case ChatLexer::TOK_TEXT:
         case ChatLexer::TOK_SMILEY:
         case ChatLexer::TOK_DESCTAG:
            if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
            {
               const unsigned smiley_index = to_smiley_index( lex.tokPtr( )[ 2 ] );

               assert( smiley_index < ChatLexer::NUM_SMILEYS );

               if (sa_imgl_smileyicons[ smiley_index ])
               {
                  elem.type = PTB_ELEM_TYPE_IMG;
                  elem.imglist = sa_imgl_smileyicons[ smiley_index ];
                  elem.img_index   = 0;
                  elem.text        = NULL;
               }
               else
               {
                  elem.type = PTB_ELEM_TYPE_STR;
                  elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
               }
            }
            else
            {
               elem.type  = PTB_ELEM_TYPE_STR;
               elem.text  = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
            }

            elem.style    = PTB_ELEM_STYLE_BOLD;
            elem.color    = CHAT_COLOR_EMOTE;
            elem.param    = 0;
            if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
               elem.alt_text = to_unicode( lex.tokPtr( ), lex.tokLen( ) );
            else
               elem.alt_text = NULL;
            
            elems.push_back( elem );

            // Log.
            m_log.write( lex.tokPtr( ), lex.tokLen( ) );
         }
      }
   }
   /* Do the message. */
   {
      ChatLexer lex( msg.c_str( ), msg.length( ) );
      while (!lex.eof( ))
      {
         switch (lex.next( ))
         {
         case ChatLexer::TOK_TEXT:
         case ChatLexer::TOK_SMILEY:
         case ChatLexer::TOK_DESCTAG:
         case ChatLexer::TOK_IMG:
            if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
            {
               const unsigned smiley_index = to_smiley_index( lex.tokPtr( )[ 2 ] );

               assert( smiley_index < ChatLexer::NUM_SMILEYS );

               if (sa_imgl_smileyicons[ smiley_index ])
               {
                  elem.type = PTB_ELEM_TYPE_IMG;
                  elem.imglist = sa_imgl_smileyicons[ smiley_index ];
                  elem.img_index   = 0;
                  elem.text        = NULL;
               }
               else
               {
                  elem.type = PTB_ELEM_TYPE_STR;
                  elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
               }
            }
            else if (lex.tokType( ) == ChatLexer::TOK_IMG)
            {
               // Only allow local files.
               HIMAGELIST imgl = NULL;
               if (lex.tokImgSrc( ).compare( 0, 8, "file:///" ) == 0 && (imgl = imgl_from_file( lex.tokImgSrc( ).substr( 8 ) )))
               {
                  elem.type = PTB_ELEM_TYPE_IMG;
                  elem.imglist = imgl;
                  elem.img_index = 0;
                  elem.text = NULL;
               }
               else
               {
                  elem.type = PTB_ELEM_TYPE_STR;
                  elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
               }
            }
            else
            {
               elem.type  = PTB_ELEM_TYPE_STR;
               elem.text  = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
            }

            if (lex.tokStyle( ) & ChatLexer::STYLE_LINK)
            {
               string href = lex.tokHref( );
               if (_strnicmp( href.c_str( ), "http://", 7 ) &&
                   _strnicmp( href.c_str( ), "https://", 8 ) &&
                   _strnicmp( href.c_str( ), "ftp://", 6 ) &&
                   _strnicmp( href.c_str( ), "furc://", 7 ) &&
                   _strnicmp( href.c_str( ), "recip:", 6 ))
               {
                  href = "http://" + href;
               }
               m_links.push_back( href );

               elem.style    = PTB_ELEM_STYLE_UNDERLINE;
               elem.color    = CHAT_COLOR_LINK;
               elem.param    = m_links.size( ); // 1-based.
               elem.alt_text = to_unicode( href.c_str( ) );
            }
            else
            {
               elem.style    = 0;
               elem.color    = CHAT_COLOR_EMOTE;
               elem.param    = 0;
               if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
                  elem.alt_text = to_unicode( lex.tokPtr( ), lex.tokLen( ) );
               else
                  elem.alt_text = NULL;
            }

            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_BOLD ? PTB_ELEM_STYLE_BOLD : 0;
            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_ITALIC ? PTB_ELEM_STYLE_ITALIC : 0;
            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_UNDERLINE ? PTB_ELEM_STYLE_UNDERLINE : 0;
            elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_LINK ? PTB_ELEM_STYLE_LINK : 0;

            elems.push_back( elem );

            // Log.
            m_log.write( lex.tokPtr( ), lex.tokLen( ) );
         }
      }
   }

   PoorTextBox_Append( m_wnd.chat, elems.size( ), &elems.front( ) );

   while (!elems.empty( ))
   {
      delete [] elems.back( ).text;
      delete [] elems.back( ).alt_text;
      elems.pop_back( );
   }

   m_worth_saving = true;

   if (GetForegroundWindow( ) != m_wnd.wnd)
      FlashWindow( m_wnd.wnd, TRUE );
}

void PmSession::appendMsg( const string& msg, int icon )
{
   // Batch elements for better performance.
   vector<PTBELEM> elems;
   PTBELEM elem;

   // Insert a linebreak.
   if (GetWindowTextLength( m_wnd.chat ))
   {
      elem.type     = PTB_ELEM_TYPE_STR;
      elem.style    = PTB_ELEM_STYLE_NORMAL;
      elem.text     = to_unicode( "\n" ); // Yes, this is a bit silly.
      elem.alt_text = NULL;
      elem.param    = 0;
      elems.push_back( elem );

      // Log.
      m_log << "\r\n";
   }

   // Insert an icon.
   if (icon < MSG_ICON_NONE)
   {
      elem.type        = PTB_ELEM_TYPE_IMG;
      elem.style       = PTB_ELEM_STYLE_NORMAL;
      elem.imglist = s_imgl_msgicons;
      elem.img_index   = icon;
      elem.alt_text    = to_unicode( "[f] " );
      elem.text        = NULL;
      elem.param       = 0;
      elems.push_back( elem );

      // Log.
      m_log << "[f] ";
   }

   ChatLexer lex( msg.c_str( ), msg.length( ) );
   while (!lex.eof( ))
   {
      switch (lex.next( ))
      {
      case ChatLexer::TOK_TEXT:
      case ChatLexer::TOK_DESCTAG:
      case ChatLexer::TOK_SMILEY:
      case ChatLexer::TOK_IMG:
         if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
         {
            const unsigned smiley_index = to_smiley_index( lex.tokPtr( )[ 2 ] );

            assert( smiley_index < ChatLexer::NUM_SMILEYS );

            if (sa_imgl_smileyicons[ smiley_index ])
            {
               elem.type = PTB_ELEM_TYPE_IMG;
               elem.imglist = sa_imgl_smileyicons[ smiley_index ];
               elem.img_index   = 0;
               elem.text        = NULL;
            }
            else
            {
               elem.type = PTB_ELEM_TYPE_STR;
               elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
            }
         }
         else if (lex.tokType( ) == ChatLexer::TOK_IMG)
         {
            // Only allow local files.
            HIMAGELIST imgl = NULL;
            if (lex.tokImgSrc( ).compare( 0, 8, "file:///" ) == 0 && (imgl = imgl_from_file( lex.tokImgSrc( ).substr( 8 ) )))
            {
               elem.type = PTB_ELEM_TYPE_IMG;
               elem.imglist = imgl;
               elem.img_index = 0;
               elem.text = NULL;
            }
            else
            {
               elem.type = PTB_ELEM_TYPE_STR;
               elem.text = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
            }
         }
         else
         {
            elem.type  = PTB_ELEM_TYPE_STR;
            elem.text  = to_unicode_ex( lex.tokPtr( ), lex.tokLen( ) );
         }

         if (lex.tokStyle( ) & ChatLexer::STYLE_LINK)
         {
            string href = lex.tokHref( );
            if (_strnicmp( href.c_str( ), "http://", 7 ) &&
                _strnicmp( href.c_str( ), "https://", 8 ) &&
                _strnicmp( href.c_str( ), "ftp://", 6 ) &&
                _strnicmp( href.c_str( ), "furc://", 7 ) &&
                _strnicmp( href.c_str( ), "recip:", 6 ))
            {
               href = "http://" + href;
            }
            m_links.push_back( href );

            elem.style    = PTB_ELEM_STYLE_UNDERLINE;
            elem.color    = CHAT_COLOR_LINK;
            elem.param    = m_links.size( ); // 1-based.
            elem.alt_text = to_unicode( href.c_str( ) );
         }
         else
         {
            elem.style    = 0;
            elem.color    = CHAT_COLOR_MSG;
            elem.param    = 0;
            if (lex.tokType( ) == ChatLexer::TOK_SMILEY)
               elem.alt_text = to_unicode( lex.tokPtr( ), lex.tokLen( ) );
            else
               elem.alt_text = NULL;
         }

         elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_BOLD ? PTB_ELEM_STYLE_BOLD : 0;
         elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_ITALIC ? PTB_ELEM_STYLE_ITALIC : 0;
         elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_UNDERLINE ? PTB_ELEM_STYLE_UNDERLINE : 0;
         elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_LINK ? PTB_ELEM_STYLE_LINK : 0;

         elems.push_back( elem );

         // Log.
         m_log.write( lex.tokPtr( ), lex.tokLen( ) );
      }
   }

   PoorTextBox_Append( m_wnd.chat, elems.size( ), &elems.front( ) );

   while (!elems.empty( ))
   {
      delete [] elems.back( ).text;
      delete [] elems.back( ).alt_text;
      elems.pop_back( );
   }

   if (GetForegroundWindow( ) != m_wnd.wnd)
      FlashWindow( m_wnd.wnd, TRUE );
}

void PmSession::doWhisper( const wstring& msg )
{
   // Escape any non-ascii characters.
   PmSession::doWhisper( xml_charset::uni_to_ascii( msg ) );
}

void PmSession::doWhisper( const string& msg )
{
   if (!Eggy::singleton( )->isLoggedIn( ) || !msg.length( ))
      return;

   // Reset all the last_sents.
   for (list<RECIPIENT*>::iterator i = m_recipients.begin( ); i != m_recipients.end( ); ++i)
      (*i)->last_sent.clear( );

   /* Send the message to all recipients. */
   for (list<RECIPIENT*>::iterator i = m_recipients.begin( );
        i != m_recipients.end( ); ++i)
   {
      // Exact whispers are now gone.
      // Always PM target names with quotes so we can differentiate delivery errors
      // as coming from a PM.
      const char* cmd = c2s_enc_whisper( ("%\"" + (*i)->contact->getShortName( ) + "\"").c_str( ), 
                                          msg.c_str( ) );
      furn2::f2_netout( cmd, strlen( cmd ) );
      yiffurc_release( cmd );
   }
}

void PmSession::doSummon( )
{
   if (!Eggy::singleton( )->isLoggedIn( ))
      return;

   // Auto-summon everyone.
   for (list<RECIPIENT*>::iterator i = m_recipients.begin( );
        i != m_recipients.end( ); ++i)
   {
      Eggy::singleton( )->autoSummon( (*i)->contact->getShortName( ) );
   }
}

void PmSession::doSummon( const string& name )
{
   if (!Eggy::singleton( )->isLoggedIn( ))
      return;

   // Auto-summon the dude.
   Eggy::singleton( )->autoSummon( name );
}

void PmSession::doJoin( const string& name )
{
   if (!Eggy::singleton( )->isLoggedIn( ))
      return;

   const char* cmd = c2s_enc_join( ("%" + make_shortname( name )).c_str( ) );
   f2_netout( cmd, strlen( cmd ) );
   yiffurc_release( cmd );
}

void PmSession::doDecline( )
{
   if (!Eggy::singleton( )->isLoggedIn( ))
      return;

   const char* cmd = c2s_enc_decline( );
   f2_netout( cmd, strlen( cmd ) );
}

void PmSession::doIgnore( const string& name )
{
   f2__ignore_add( make_shortname( name ).c_str( ) );
   appendMsg( "Now ignoring <b>" + name + "</b>.", MSG_ICON_GENERAL );
}

void PmSession::doIgnore( )
{
   for (list<RECIPIENT*>::iterator i = m_recipients.begin( );
        i != m_recipients.end( ); ++i)
   {
      f2__ignore_add( (*i)->contact->getShortName( ).c_str( ) );
      appendMsg( "Now ignoring <b>" + (*i)->contact->getName( ) + "</b>.", MSG_ICON_GENERAL );
   }
}

void PmSession::doUnignore( const string& name )
{
   f2__ignore_rm( make_shortname( name ).c_str( ) );
   appendMsg( "No longer ignoring <b>" + name + "</b>.", MSG_ICON_GENERAL );
}

void PmSession::doUnignore( )
{
   for (list<RECIPIENT*>::iterator i = m_recipients.begin( );
        i != m_recipients.end( ); ++i)
   {
      f2__ignore_rm( (*i)->contact->getShortName( ).c_str( ) );
      appendMsg( "No longer ignoring <b>" + (*i)->contact->getName( ) + "</b>.", MSG_ICON_GENERAL );
   }
}

void PmSession::doSaveLogAs( )
{
   char* filename = new char[ MAX_PATH ];
   filename[ 0 ] = '\0';
   {
      string root = Eggy::singleton( )->getLogFolder( );
      create_folder_path( root );

      OPENFILENAME ofn;
      memset( &ofn, 0, sizeof( ofn ) );
      ofn.lStructSize = sizeof( ofn );
      ofn.hwndOwner = m_wnd.wnd;
      ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0";
      ofn.nFilterIndex = 1;
      ofn.lpstrFile = filename;
      ofn.nMaxFile = MAX_PATH;
      ofn.lpstrInitialDir = root.c_str( );
      ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOREADONLYRETURN | OFN_NOCHANGEDIR;
      ofn.lpstrDefExt = "txt";

      if (!GetSaveFileName( &ofn ))
      {
         delete [] filename;
         return;
      }
   }

   ofstream fs( filename );
   if (fs.fail( ))
   {
      f2_errspeak( "Eggy: Couldn't write session to log." );
   }
   else
   {
      char* sz_time = ctime( &m_created );
      sz_time[ strlen( sz_time ) - 1 ] = '\0';
      fs << "[Session created on " << sz_time << "]\r\n"
         << m_log.str( )
         << "\r\n\r\n";
   }

   delete [] filename;
}

void PmSession::doLaunchLink( size_t index )
{
   // Find the link.
   list<string>::const_iterator i = m_links.begin( );
   for (; index && i != m_links.end( ); ++i, --index) ;

   if (!index && i != m_links.end( ))
   {
      if (!_strnicmp( i->c_str( ), "http://", 7 ) ||
          !_strnicmp( i->c_str( ), "https://", 8 ) ||
          !_strnicmp( i->c_str( ), "ftp://", 6 ))
      {
         // Launch it.
         ShellExecute( NULL, "open", i->c_str( ), NULL, NULL, SW_SHOWNORMAL );
      }
      else if (!_strnicmp( i->c_str( ), "recip:", 6 ))
      {
         // Add recipients.
         PLAYERINFO pi;
         f2_myplayerinfo( &pi );

         size_t beg = 6, end = 6;
         while (beg < i->length( ) && end != i->npos)
         {
            end = i->find( ' ', end + 1 );
            if (beg < end)
            {
               string name = i->substr( beg, end - beg );
               // Don't add ourselves.
               if (_stricmp( pi.name, name.c_str( ) ))
                  addRecipient( name );
            }
            beg = end + 1;
         }

         // Show the pane.
         showRecipientsPane( true );
      }
      else if (!_strnicmp( i->c_str( ), "furc://", 7 ))
      {
         f2_netoutline( ("fdl " + *i).c_str( ), 4 + i->length( ) );
      }
   }
}

void PmSession::doCopyLink( size_t index )
{
   // Find the link.
   list<string>::const_iterator i = m_links.begin( );
   for (; index && i != m_links.end( ); ++i, --index) ;

   if (!index && i != m_links.end( ))
   {
      if (OpenClipboard( m_wnd.wnd ))
      {
         EmptyClipboard( );

         HANDLE hmem = GlobalAlloc( GMEM_MOVEABLE, i->size( ) + 1 );
         char* buf = (char*) GlobalLock( hmem );
         strcpy_s( buf, i->size( ) + 1, i->c_str( ) );
         GlobalUnlock( hmem );

         if (!SetClipboardData( CF_TEXT, hmem ))
            GlobalFree( hmem );

         CloseClipboard( );
      }
   }
}

void PmSession::doShareRecips( )
{
   if (!Eggy::singleton( )->isLoggedIn( ))
      return;

   string msg = "<a href=\"recip:";
   for (list<RECIPIENT*>::const_iterator i = m_recipients.begin( );
        i != m_recipients.end( ); ++i)
   {
      msg += (*i)->contact->getShortName( );
      msg += ' ';
   }
   msg += "\">Click to add recipients.</a>";

   doWhisper( msg );
}

void PmSession::doViewLog( )
{
   string path = Eggy::singleton( )->getLogFile( );

   if (m_recipients.size( ) == 1 && Eggy::singleton( )->getOptFlags( ) & OPT_SEPARATELOGS)
   {
      path = Eggy::singleton( )->getLogFolder( ) + "\\" + 
                to_path_friendly( pipes_to_spaces( m_recipients.front( )->contact->getName( ) ) ) + ".txt";
   }

   if (GetFileAttributes( path.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
      MessageBox( m_wnd.wnd, "No log file exists for this player yet.", "Eggy", MB_OK | MB_ICONINFORMATION );
   else
      ShellExecute( NULL, "open", path.c_str( ), NULL, NULL, SW_SHOWNORMAL );
}

void PmSession::doAddBuddy( const string& group )
{
   BuddyList* bl = Eggy::singleton( )->getBuddyList( );
   if (bl)
   {
      for (list<RECIPIENT*>::const_iterator i = m_recipients.begin( ); i != m_recipients.end( ); ++i)
         bl->addBuddy( (*i)->contact->getName( ), group );
   }
   bl->writeList( );
}

void PmSession::doRemoveBuddy( )
{
   BuddyList* bl = Eggy::singleton( )->getBuddyList( );
   if (bl)
   {
      for (list<RECIPIENT*>::const_iterator i = m_recipients.begin( ); i != m_recipients.end( ); ++i)
         bl->removeBuddy( (*i)->contact->getName( ) );
   }
   bl->writeList( );
}

void PmSession::doSysMsg( const string& msg )
{
   appendMsg( msg, MSG_ICON_GENERAL );
}

LRESULT CALLBACK PmSession::wnd_proc( HWND wnd, UINT msg, 
                                      WPARAM wparam, LPARAM lparam )
{
#define _INST_PTR( ) ((PmSession*) GetWindowLongPtr( wnd, GWLP_USERDATA ))
   
   static const List_Item_Type LPARAM_ADD = LISTITEM_ADDRECIPIENT;

   switch (msg)
   {
   case WM_CREATE:
      {
         CREATESTRUCT* cs = (CREATESTRUCT*) lparam;
         // Store the instance ptr.
         SetWindowLongPtr( wnd, GWLP_USERDATA, (LONG_PTR) cs->lpCreateParams );
      }
      return 0;

   case WM_SETFOCUS:
      // Set keyboard focus to the input box.
      if (wnd == GetForegroundWindow( ))
         SetFocus( _INST_PTR( )->m_wnd.input );
      return 0;

   case furn2::F2M_ISNOTDLGWND:
      return TRUE;

   case WM_GETMINMAXINFO:
      /* This comes before WM_CREATE. */
      if (_INST_PTR( ) && _INST_PTR( )->m_recipients_visible)
         ((MINMAXINFO*) lparam)->ptMinTrackSize.x = DLG_MIN_WIDTH + DLG_RECIPPANE_W;
      else
         ((MINMAXINFO*) lparam)->ptMinTrackSize.x = DLG_MIN_WIDTH;
      ((MINMAXINFO*) lparam)->ptMinTrackSize.y = DLG_MIN_HEIGHT;
      return 0;

   case WM_COMMAND:
      {
         PmSession* inst = _INST_PTR( );

         if ((HWND) lparam == inst->m_wnd.rtoolbar || 
             (HWND) lparam == inst->m_wnd.ltoolbar)
         {
            switch (LOWORD( wparam ))
            {
            case ID_PM_SHOWRECIP:
               inst->showRecipientsPane( !_INST_PTR( )->m_recipients_visible );
               return 0;

            case ID_PM_JOIN:
               {
                  // Join the currently selected recipient.
                  LVITEM item;
                  item.mask     = LVIF_PARAM;
                  item.iItem    = ListView_GetSelectionMark( inst->m_wnd.recipients );
                  item.iSubItem = 0;
                  if (ListView_GetItem( inst->m_wnd.recipients, &item ) &&
                      *(List_Item_Type*) item.lParam == LISTITEM_RECIPIENT)
                     inst->doJoin( ((RECIPIENT*) item.lParam)->contact->getName( ) );
                  else if (!inst->m_recipients.empty( ))
                     inst->doJoin( inst->m_recipients.front( )->contact->getName( ) );
               }
               return 0;

            case ID_PM_SUMMON:
               inst->doSummon( );
               return 0;

            case ID_PM_DECLINE:
               inst->doDecline( );
               return 0;

            case ID_PM_IGNORE:
               inst->doIgnore( );
               return 0;

            case ID_PM_UNIGNORE:
               inst->doUnignore( );
               return 0;

            case ID_PM_SAVE:
               inst->doSaveLogAs( );
               return 0;

            case ID_PM_VIEWLOG:
               inst->doViewLog( );
               return 0;

            case ID_PM_ADDBUDDY:
               {
                  /* Populate the groups submenu. */
                  while (GetMenuItemCount( inst->m_groups_menu ) > 0)
                     DeleteMenu( inst->m_groups_menu, 0, MF_BYPOSITION );
                  list<string> groups = Eggy::singleton( )->getBuddyList( )->getGroups( );
                  // Make sure there's an empty group at the top.
                  {
                     list<string>::iterator i = find( groups.begin( ), groups.end( ), "" );
                     if (i != groups.end( ))
                        groups.erase( i );
                     groups.push_front( "" );
                  }
                  
                  for (list<string>::const_iterator i = groups.begin( ); i != groups.end( ); ++i)
                  {
                     if (i->length( ))
                        AppendMenu( inst->m_groups_menu, MF_STRING, GetMenuItemCount( inst->m_groups_menu ) + 1, i->c_str( ) );
                     else
                        AppendMenu( inst->m_groups_menu, MF_STRING, GetMenuItemCount( inst->m_groups_menu ) + 1, "(No Group)" );
                  }

                  POINT cursor;
                  GetCursorPos( &cursor );

                  unsigned cmd = TrackPopupMenu( inst->m_groups_menu, TPM_NONOTIFY | TPM_RETURNCMD, 
                                                 cursor.x, cursor.y, 0, inst->m_wnd.wnd, NULL );
                  
                  if (cmd < groups.size( ) + 1)
                  {
                     for (list<string>::const_iterator i = groups.begin( ); i != groups.end( ); ++i)
                     {
                        if (cmd == 1)
                        {
                           if (inst->m_recipients.size( ) > 1 && 
                               MessageBox( inst->m_wnd.wnd, "All recipients will be added as buddies. Are you sure?", 
                                           "Add Buddy", MB_YESNO | MB_ICONQUESTION ) == IDNO)
                              break;
                           inst->doAddBuddy( *i );
                           break;
                        }
                        --cmd;
                     }
                  }
                  return 0;
               }

            case ID_PM_RMBUDDY:
               inst->doRemoveBuddy( );
               return 0;
            }
         }
         else if ((HWND) lparam == inst->m_wnd.send)
         {
            switch (HIWORD( wparam ))
            {
            case BN_CLICKED:
               {
                  // Send whatever is in the input buffer.
                  HLOCAL hmem = Edit_GetHandle( inst->m_wnd.input );
                  if (hmem)
                  {
                     const wchar_t* mem = (const wchar_t*) LocalLock( hmem );
                     const unsigned len = wcslen( mem );
                     wchar_t* msg = new wchar_t[ len + 1 ];
                     wcscpy( msg, mem );
                     LocalUnlock( hmem );
                     // Clean up whitespace.
                     for (wchar_t* p = msg; *p; ++p)
                        if ((unsigned short) *p <= (unsigned short) L' ') *p = L' ';
                     SetWindowTextW( inst->m_wnd.input, L"" );
                     inst->doWhisper( msg );

                     // Throw it on the top of the input history.
                     if (!inst->m_input_history.size( ) || inst->m_input_history.front( ) != msg)
                     {
                        inst->m_input_history.push_front( msg );
                        while (inst->m_input_history.size( ) > INPUT_HISTORY_SIZE)
                           inst->m_input_history.pop_back( );
                     }

                     delete [] msg;
                  }
                  return 0;
               }
               return 0;
            }
         }
         else if (HIWORD( wparam ) == 0 && !lparam) // Menu item.
         {
            switch (LOWORD( wparam ))
            {
            case ID_RECIP_SUMMON:
               {
                  // Summon the currently selected recipient.
                  LVITEM item;
                  item.mask     = LVIF_PARAM;
                  item.iItem    = ListView_GetSelectionMark( inst->m_wnd.recipients );
                  item.iSubItem = 0;
                  if (ListView_GetItem( inst->m_wnd.recipients, &item ) &&
                      *(List_Item_Type*) item.lParam == LISTITEM_RECIPIENT)
                     inst->doSummon( ((RECIPIENT*) item.lParam)->contact->getName( ) );
               }
               return 0;

            case ID_RECIP_JOIN:
               {
                  // Join the currently selected recipient.
                  LVITEM item;
                  item.mask     = LVIF_PARAM;
                  item.iItem    = ListView_GetSelectionMark( inst->m_wnd.recipients );
                  item.iSubItem = 0;
                  if (ListView_GetItem( inst->m_wnd.recipients, &item ) &&
                      *(List_Item_Type*) item.lParam == LISTITEM_RECIPIENT)
                     inst->doJoin( ((RECIPIENT*) item.lParam)->contact->getName( ) );
               }
               return 0;

            case ID_RECIP_IGNORE:
               {
                  // Ignore the currently selected recipient.
                  LVITEM item;
                  item.mask     = LVIF_PARAM;
                  item.iItem    = ListView_GetSelectionMark( inst->m_wnd.recipients );
                  item.iSubItem = 0;
                  if (ListView_GetItem( inst->m_wnd.recipients, &item ) &&
                      *(List_Item_Type*) item.lParam == LISTITEM_RECIPIENT)
                     inst->doIgnore( ((RECIPIENT*) item.lParam)->contact->getName( ) );
               }
               return 0;

            case ID_RECIP_UNIGNORE:
               {
                  // Unignore the currently selected recipient.
                  LVITEM item;
                  item.mask     = LVIF_PARAM;
                  item.iItem    = ListView_GetSelectionMark( inst->m_wnd.recipients );
                  item.iSubItem = 0;
                  if (ListView_GetItem( inst->m_wnd.recipients, &item ) &&
                     *(List_Item_Type*) item.lParam == LISTITEM_RECIPIENT)
                     inst->doUnignore( ((RECIPIENT*) item.lParam)->contact->getName( ) );
               }
               break;

            case ID_RECIP_ADD:
               {
                  // Insert a dummy item and edit it.
                  LVITEM item;
                  item.mask = LVIF_PARAM | LVIF_IMAGE;
                  item.iItem = ListView_GetItemCount( inst->m_wnd.recipients );
                  item.iSubItem = 0;
                  item.lParam = (LPARAM) &LPARAM_ADD;
                  item.iImage = IMGL_CONTACT;
                  item.pszText = NULL;
                  ListView_EditLabel( inst->m_wnd.recipients, 
                                      ListView_InsertItem( inst->m_wnd.recipients, &item ) );
               }
               return 0;

            case ID_RECIP_DELETE:
               {
                  LVITEM item;
                  item.mask = LVIF_PARAM;
                  item.iItem = ListView_GetSelectionMark( inst->m_wnd.recipients );
                  item.iSubItem = 0;
                  if (ListView_GetItem( inst->m_wnd.recipients, &item ))
                  {
                     if (*(List_Item_Type*) item.lParam == LISTITEM_RECIPIENT)
                        inst->removeRecipient( ((RECIPIENT*) item.lParam)->contact->getName( ) );
                  }
               }
               return 0;

            case ID_RECIP_DETACH:
               {
                  LVITEM item;
                  item.mask = LVIF_PARAM;
                  item.iItem = ListView_GetSelectionMark( inst->m_wnd.recipients );
                  item.iSubItem = 0;
                  if (ListView_GetItem( inst->m_wnd.recipients, &item ))
                  {
                     if (*(List_Item_Type*) item.lParam == LISTITEM_RECIPIENT)
                        inst->detachRecipient( ((RECIPIENT*) item.lParam)->contact->getName( ) );
                  }
               }
               return 0;
               
            case ID_RECIP_SHARE:
               {
                  inst->doShareRecips( );
               }
               return 0;
            }
         }
      }
      break;

   case WM_NOTIFY:
      {
         PmSession* inst = _INST_PTR( );
         if (((LPNMHDR) lparam)->hwndFrom == inst->m_wnd.recipients)
         {
            switch (((LPNMHDR) lparam)->code)
            {
            case NM_RCLICK:
               {
                  NMITEMACTIVATE* nm = (NMITEMACTIVATE*) lparam;
                  if (nm->iItem != -1)
                  {
                     /* Display the popup. */
                     // Can't detach if only recipient.
                     if (inst->m_recipients.size( ) == 1)
                        EnableMenuItem( GetSubMenu( s_recip_menu, 1 ), ID_RECIP_DETACH, 
                                        MF_GRAYED | MF_BYCOMMAND );
                     else
                        EnableMenuItem( GetSubMenu( s_recip_menu, 1 ), ID_RECIP_DETACH, 
                                        MF_ENABLED | MF_BYCOMMAND );

                     POINT cursor;
                     GetCursorPos( &cursor );
                     TrackPopupMenu( GetSubMenu( s_recip_menu, 1 ), TPM_RIGHTBUTTON, 
                                     cursor.x, cursor.y, 0, wnd, NULL );
                  }
                  else
                  {
                     /* Display the popup. */
                     POINT cursor;
                     GetCursorPos( &cursor );
                     TrackPopupMenu( GetSubMenu( s_recip_menu, 0 ), TPM_RIGHTBUTTON, 
                                     cursor.x, cursor.y, 0, wnd, NULL );
                  }
               }
               return 0;

            case LVN_GETINFOTIP:
               {
                  NMLVGETINFOTIP* nmlv = (NMLVGETINFOTIP*) lparam;
                  // nmlv->lParam is always zero! >(
                  LVITEM item;
                  item.mask = LVIF_PARAM;
                  item.iItem = nmlv->iItem;
                  item.iSubItem = 0;
                  ListView_GetItem( nmlv->hdr.hwndFrom, &item );

                  if (*(List_Item_Type*) item.lParam == LISTITEM_RECIPIENT)
                  {
                     RECIPIENT* recip = (RECIPIENT*) item.lParam;

                     string tip = recip->contact->makeTip( );
                     if (tip.length( ) > (size_t) nmlv->cchTextMax)
                     {
                        assert( nmlv->cchTextMax >= 3 );
                        tip.resize( nmlv->cchTextMax - 3 );
                        tip += "...";
                     }
                     strcpy( nmlv->pszText, tip.c_str( ) );
                  }
               }
               return 0;

            case LVN_BEGINLABELEDIT:
               {
                  // Don't allow in-place editing of non-dummy items.
                  NMLVDISPINFO* nmlv = (NMLVDISPINFO*) lparam;
                  if (*(List_Item_Type*) nmlv->item.lParam == LISTITEM_ADDRECIPIENT)
                     return FALSE;
                  return TRUE;
               }

            case LVN_ENDLABELEDIT:
               {
                  NMLVDISPINFO* nmlv = (NMLVDISPINFO*) lparam;
                  if (nmlv->item.pszText) // Edit successful.
                  {
                     assert( *(List_Item_Type*) nmlv->item.lParam == LISTITEM_ADDRECIPIENT );
                     // Add the new recipient.
                     if (inst->addRecipient( nmlv->item.pszText ))
                     {
                        // Deselect.
                        ListView_SetItemState( nmlv->hdr.hwndFrom, 
                                               ListView_GetSelectionMark( nmlv->hdr.hwndFrom ),
                                               0, LVIS_SELECTED );
                        // Select the newly added item.
                        RECIPIENT* recip = inst->findRecipient( nmlv->item.pszText );
                        if (recip)
                        {
                           int index = ListView_FindFirstItemLParam( nmlv->hdr.hwndFrom, (LPARAM) recip );
                           ListView_SetSelectionMark( nmlv->hdr.hwndFrom, index );
                           ListView_SetItemState( nmlv->hdr.hwndFrom, index, LVIS_SELECTED, LVIS_SELECTED );
                        }
                     }
                  }

                  // Delete the dummy edit label. Hope this is safe within this event.
                  ListView_DeleteItem( nmlv->hdr.hwndFrom, nmlv->item.iItem );
               }
               return FALSE;
            }
         }
         else if (((LPNMHDR) lparam)->hwndFrom == inst->m_wnd.chat)
         {
            switch (((LPNMHDR) lparam)->code)
            {
            case PTBN_CHANGED:
               {
                  /* Trim lines. */
                  if (PoorTextBox_GetLineCount( inst->m_wnd.chat ) > MAX_CHAT_LINES)
                     PoorTextBox_TrimLines( inst->m_wnd.chat, MAX_CHAT_LINES );
               }
               return 0;
            case PTBN_ELEMACTION:
               {
                  NMPTBELEMACTION* nm = (NMPTBELEMACTION*) lparam;
                  if (nm->action == PTB_ELEMACTION_CLICK)
                  {
                     inst->doLaunchLink( nm->param - 1 ); // indices are 1-based.
                     return TRUE;
                  }
                  else if (nm->action == PTB_ELEMACTION_RCLICK)
                  {
                     POINT cursor;
                     GetCursorPos( &cursor );
                     int cmd = TrackPopupMenu( GetSubMenu( s_link_menu, 0 ), 
                                               TPM_RIGHTBUTTON | TPM_RETURNCMD, 
                                               cursor.x, cursor.y, 0, wnd, NULL );
                     if (cmd == ID_LINK_COPYLINK)
                        inst->doCopyLink( nm->param - 1 ); // indices are 1-based.
                     else if (cmd == ID_LINK_LAUNCH)
                        inst->doLaunchLink( nm->param - 1 ); // indices are 1-based.
                     return TRUE;
                  }
               }
               return FALSE;
            }
         }
         else if (((LPNMHDR) lparam)->hwndFrom == inst->m_wnd.ltoolbar ||
                  ((LPNMHDR) lparam)->hwndFrom == inst->m_wnd.rtoolbar)
         {
            switch (((LPNMHDR) lparam)->code)
            {
            case TBN_GETINFOTIP:
               {
                  NMTBGETINFOTIP* tbn = (NMTBGETINFOTIP*) lparam;
                  
                  switch (tbn->iItem)
                  {
                  case ID_PM_DECLINE:
                     strcpy_s( tbn->pszText, tbn->cchTextMax, "Decline Request" );
                     break;
                  case ID_PM_IGNORE:
                     strcpy_s( tbn->pszText, tbn->cchTextMax, "Ignore" );
                     break;
                  case ID_PM_JOIN:
                     strcpy_s( tbn->pszText, tbn->cchTextMax, "Join" );
                     break;
                  case ID_PM_SAVE:
                     strcpy_s( tbn->pszText, tbn->cchTextMax, "Save Log As" );
                     break;
                  case ID_PM_VIEWLOG:
                     strcpy_s( tbn->pszText, tbn->cchTextMax, "View Log" );
                     break;
                  case ID_PM_SHOWRECIP:
                     strcpy_s( tbn->pszText, tbn->cchTextMax, "Toggle Recipients Pane" );
                     break;
                  case ID_PM_SUMMON:
                     strcpy_s( tbn->pszText, tbn->cchTextMax, "Summon" );
                     break;
                  case ID_PM_UNIGNORE:
                     strcpy_s( tbn->pszText, tbn->cchTextMax, "Unignore" );
                     break;
                  case ID_PM_ADDBUDDY:
                     strcpy_s( tbn->pszText, tbn->cchTextMax, "Add Buddy" );
                     break;
                  case ID_PM_RMBUDDY:
                     strcpy_s( tbn->pszText, tbn->cchTextMax, "Remove Buddy" );
                     break;
                  }
               }
               return 0;
            }
         }
      }
      break;

   case WM_SIZE:
      {
         // Remember the size of the PM window.
         if (!IsIconic( wnd ))
         {
            RECT r;
            GetWindowRect( wnd, &r );
            if (r.right - r.left > 0 && r.bottom - r.top > 0)
               Eggy::singleton( )->setPmSize( r.right - r.left, r.bottom - r.top );
            _INST_PTR( )->fitWindowElements( );
         }
         break;
      }

   case WM_CLOSE:
      {
         Eggy::singleton( )->pmDestroy( _INST_PTR( ) );
      }
      return 0;
   }
   
   return DefWindowProc( wnd, msg, wparam, lparam );

#undef _INST_PTR
}

LRESULT CALLBACK PmSession::input_proc( HWND wnd, UINT msg, 
                                        WPARAM wparam, LPARAM lparam )
{
#define _INST_PTR( ) ((PmSession*) GetWindowLongPtrW( GetParent( wnd ), GWLP_USERDATA ))

   switch (msg)
   {
   case WM_CHAR:
      if (wparam == L'\r')
      {
         // Simulate a send button hit.
         SendMessageW( GetParent( wnd ), WM_COMMAND, 
                       MAKEWPARAM( 0, BN_CLICKED ), (LPARAM) _INST_PTR( )->m_wnd.send );
         return 0;
      }
      break;

   case WM_KEYDOWN:
      if (GetKeyState( VK_CONTROL ) & 0x80000000)
      {
         switch (wparam)
         {
         case 0x41: // VK_A
            SendMessageW( wnd, EM_SETSEL, 0, -1 );
            return 0;

         case 0x43: // VK_C
            SendMessageW( wnd, WM_COPY, 0, 0 );
            return 0;

         case 0x56: // VK_V
            SendMessageW( wnd, WM_PASTE, 0, 0 );
            return 0;

         case 0x58: // VK_X
            SendMessageW( wnd, WM_CUT, 0, 0 );
            return 0;

         case 0x5A: // VK_Z
            SendMessageW( wnd, WM_UNDO, 0, 0 );
            return 0;

         case 0x49: // VK_I
            {
               HLOCAL hmem = (HLOCAL) SendMessageW( wnd, EM_GETHANDLE, 0, 0 );
               if (hmem)
               {
                  const wchar_t* mem = (const wchar_t*) LocalLock( hmem );
                  const unsigned len = wcslen( mem );
                  wchar_t* sz_curr_input = new wchar_t[ len + 1 ];
                  wcscpy( sz_curr_input, mem );
                  LocalUnlock( hmem );
                  wstring curr_input( sz_curr_input );
                  delete [] sz_curr_input;

                  PmSession* inst = _INST_PTR( );
                  list<wstring>::iterator i = inst->m_input_history.begin( );
                  for (; i != inst->m_input_history.end( ); ++i)
                  {
                     if (*i == curr_input)
                     {
                        ++i;
                        break;
                     }
                  }
                  if (i == inst->m_input_history.end( ) && inst->m_input_history.size( ))
                     i = inst->m_input_history.begin( );
                  if (i != inst->m_input_history.end( ))
                  {
                     SetWindowTextW( wnd, i->c_str( ) );
                     Edit_SetSel( wnd, 0x7FFF, 0x7FFF );
                  }
               }
            }
            return 0;
         }
      }
      break;
   }

   return CallWindowProcW( s_edit_proc, wnd, msg, wparam, lparam );
}

inline wchar_t* to_unicode( const char* sz, size_t len /* = -1  */ )
{
   if (len == -1)
      len = strlen( sz );

   wchar_t* wsz = new wchar_t[ len + 1 ];
   wsz[ len ] = L'\0';
   for (wchar_t* dst = wsz; len; --len, ++sz, ++dst)
      if (*sz != '|')      
         *dst = *(unsigned char*) sz;
      else
         *dst = L' ';
   return wsz;
}

wchar_t* to_unicode_ex( const char* sz, size_t len /* = -1  */ )
{
   wstring wstr = xml_charset::latin_to_uni( len == -1 ? string( sz ) : string( sz, sz + len ) );
   for (wstring::iterator i = wstr.begin( ); i != wstr.end( ); ++i)
      if (*i == L'|')
         *i = L' ';
   wchar_t* wsz = new wchar_t[ wstr.length( ) + 1 ];
   wcscpy( wsz, wstr.c_str( ) );
   return wsz;
}

unsigned to_smiley_index( char c )
{
   unsigned smiley_index = 0;
   if (c >= 'A' && c <= 'Z')
      smiley_index = c - 'A';
   else if (c >= 'a' && c <= 'y')
      smiley_index = c - 'a' + 26;
   else if (c >= '1' && c <= '3')
      smiley_index = c - '1' + 51;
   return smiley_index;
}

unsigned fsh_to_imgls( const string& filename, unsigned first, unsigned num, HIMAGELIST out[] )
{
   using namespace yiffurc;
   struct FSHBMI
   {
      BITMAPINFOHEADER bmih;
      COLORREF         colors[ 256 ];

      FSHBMI( )
      {
         memset( &bmih, 0, sizeof( bmih ) );
         bmih.biSize     = sizeof( bmih );
         bmih.biBitCount = 8;         
         bmih.biPlanes   = 1;
         bmih.biWidth    = 0;
         bmih.biHeight   = 0;
         fill_default_palette( (PixelB8G8R8X8*) colors );
      }
   };
   
   unsigned num_added = 0;

   FSHBMI* bmi = new FSHBMI;

   FshFile fsh( filename.c_str( ) );
   if (fsh.good( ))
   {
      FshFile::SHAPEINFO si;
      char* shape_bits = new char[ 256 * 256 ];

      for (unsigned i = first; i < first + num && i < fsh.getNumShapes( ); ++i)
      {
         out[ i ] = NULL;

         si = fsh.getShapeInfo( i );
         
         assert( si.width <= 256 && si.height <= 256 );

         if (si.width && si.height)
         {
            const unsigned pad = si.width % 4 == 0 ? 0 : 4 - (si.width % 4);
            const size_t bytes_copied = fsh.copyShapeBitsPadded( i, shape_bits, pad );

            assert( bytes_copied );

            // Convert into an 8-bit DIB.
            bmi->bmih.biWidth  = si.width;
            bmi->bmih.biHeight = si.height;

            char* dib_buf = NULL;
            HBITMAP bm = CreateDIBSection( NULL, (BITMAPINFO*) bmi, DIB_RGB_COLORS, 
                                           (void**) &dib_buf, NULL, 0 );
            assert( bm && dib_buf );

            memcpy( dib_buf, shape_bits, (si.width + pad) * si.height * 1 );

            HIMAGELIST imgl = ImageList_Create( si.width, si.height, 
                                                ILC_COLORDDB | ILC_MASK, 1, 0 );

            assert( imgl );

            if (-1 == ImageList_AddMasked( imgl, bm, bmi->colors[ 0 ] ))
            {
               ImageList_Destroy( imgl );
            }
            else  // Great success!
            {
               out[ i ] = imgl;
               ++num_added;
            }

            DeleteBitmap( bm );
         }
      }

      delete [] shape_bits;
   }

   delete bmi;
   return num_added;
}

HIMAGELIST imgl_from_file( const string& filename )
{
   // Already cached?
   map<string,HIMAGELIST>::const_iterator i = sa_img_map.find( filename );
   if (i != sa_img_map.end( ) && i->second != NULL)
      return i->second;

   /* Try to load the image. */
   HIMAGELIST imgl = NULL;

   FREE_IMAGE_FORMAT file_type = FreeImage_GetFileType( filename.c_str( ) );

   if (file_type == FIF_UNKNOWN)
      return NULL;

   FIBITMAP* fi_bmp = FreeImage_Load( file_type, filename.c_str( ) );

   if (!fi_bmp)
      return NULL;
   
   BITMAPINFO* bmi = FreeImage_GetInfo( fi_bmp );
   // We're mimicking the client, which renders all file:// images upside-down.
   void* bits = NULL;
   bmi->bmiHeader.biHeight *= -1;
   HBITMAP bm = CreateDIBSection( NULL, bmi, DIB_RGB_COLORS, &bits, NULL, 0 );
   bmi->bmiHeader.biHeight *= -1;   
   memcpy( bits, FreeImage_GetBits( fi_bmp ), FreeImage_GetPitch( fi_bmp ) * bmi->bmiHeader.biHeight );

   if (bm)
   {
      int trans_index = FreeImage_GetTransparentIndex( fi_bmp );
      // 8-bit GIFs could have transparency.
      if (bmi->bmiHeader.biBitCount == 8 && trans_index != -1)
      {
         // Create a monochrome mask.
         int mask_stride = (bmi->bmiHeader.biWidth+7)/8;
         while (mask_stride % 2)
            ++mask_stride;
         int src_stride = bmi->bmiHeader.biWidth;
         while (src_stride % 4)
            ++src_stride;

         char* mask_bits = new char[ mask_stride * bmi->bmiHeader.biHeight ];
         memset( mask_bits, 0, mask_stride * bmi->bmiHeader.biHeight );
         for (int y = 0; y < bmi->bmiHeader.biHeight; ++y)
         {
            char* p = mask_bits + (y * mask_stride);
            for (int x = 0; x < bmi->bmiHeader.biWidth; ++x)
            {
               if (x && (x & 0x07) == 0)
                  ++p;
               char bitmask = 0x80 >> (x % 8);
               if (((unsigned char*) bits)[ x + y * src_stride ] == trans_index)
                  (*p) = ((*p) & ~bitmask) | (0xFF & bitmask);
               else
                  (*p) = ((*p) & ~bitmask);
            }
         }

         HBITMAP mask_bm = CreateBitmap( bmi->bmiHeader.biWidth, bmi->bmiHeader.biHeight, 1, 1, mask_bits );
         delete [] mask_bits;

         imgl = ImageList_Create( bmi->bmiHeader.biWidth, bmi->bmiHeader.biHeight, 
                                  ILC_COLOR32 | ILC_MASK, 1, 0 );
         assert( imgl );         

         if (-1 == ImageList_Add( imgl, bm, mask_bm ))
         {
            ImageList_Destroy( imgl );
            imgl = NULL;
         }

         DeleteBitmap( mask_bm );
      }
      else // No transparency.
      {
         imgl = ImageList_Create( bmi->bmiHeader.biWidth, bmi->bmiHeader.biHeight, 
                                  ILC_COLORDDB, 1, 0 );
         assert( imgl );

         if (-1 == ImageList_Add( imgl, bm, NULL ))
         {
            ImageList_Destroy( imgl );
            imgl = NULL;
         }
      }

      DeleteBitmap( bm );
   }

   FreeImage_Unload( fi_bmp );

   // Cache the image.
   if (imgl) 
      sa_img_map[ filename ] = imgl;

   return imgl;
}

//////////////////////////////////////////////////////////////////////////

} // namespace eggy