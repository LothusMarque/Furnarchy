#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include "ScriptTerm.h"
#include "common/PoorTextBox.h"
#include "common/xml_charset.h"
#include "common/ChatLexer.h"
#include "ScriptInst.h"
#include "yiffurc/fsh.hpp"
#include "yiffurc/colors.hpp"
#include "core.h"
#include "net.h"
#include "resource.h"
#include <WindowsX.h>
#include <ShellAPI.h>
#include <algorithm>
#include <sstream>

using namespace std;

//////////////////////////////////////////////////////////////////////////

enum { STERM_DEF_W = 384, STERM_DEF_H = 200, STERM_INPUT_H = 8, STERM_OUTPUT_H = 24,
       STERM_MIN_H = STERM_INPUT_H + STERM_OUTPUT_H, STERM_MIN_W = 128 };
enum { IDC_STERM_INPUT = 11333, IDC_STERM_OUTPUT };
enum { MAX_INPUT_LEN = 2048, MAX_OUTPUT_LINES = 156, MAX_OUTPUT_LINES_THRESHOLD = 64,
       MAX_LINKS = MAX_OUTPUT_LINES * 2, TRIM_COOLDOWN = 5 };
enum { ID_LINKMENU_COPY = 0x3030, ID_LINKMENU_LAUNCH };
enum Uri_Type { URI_HTTP, URI_FDL, URI_F2CMD, URI_CMD };

//////////////////////////////////////////////////////////////////////////

static unsigned fsh_to_imgls( const string& filename,  unsigned num, HIMAGELIST out[] );
static wchar_t* wcsdup( const char* sz );
static int term_to_ptb_style( const int style );
static Uri_Type guess_uri_type( const string& uri );

//////////////////////////////////////////////////////////////////////////

HINSTANCE ScriptTerm::s_hinstance = NULL;
unsigned ScriptTerm::s_init_count = 0;
HIMAGELIST ScriptTerm::s_smileys[ ScriptTerm::NUM_SMILIES ];
HMENU ScriptTerm::s_link_menu = NULL;
const ScriptTerm::StyleColor ScriptTerm::s_default_types[ ScriptTerm::NUM_TEXT_TYPES ] =
{
   /* Output */ StyleColor( STYLE_NORMAL, RGB( 0, 0, 0 ) ),
   /* Link */ StyleColor( STYLE_UNDERLINE, RGB( 0, 0, 225 ) ), 
   /* Success */ StyleColor( STYLE_NORMAL, RGB( 0, 225, 0 ) ),
   /* Warning */ StyleColor( STYLE_NORMAL, RGB( 225, 32, 32 ) ),
   /* Error */ StyleColor( STYLE_BOLD, RGB( 225, 0, 0 ) ),
   /* Channel */ StyleColor( STYLE_NORMAL, RGB( 128, 72, 128 ) ),
   /* Whisper */ StyleColor( STYLE_NORMAL, RGB( 128, 72, 100 ) ),
   /* Emote */ StyleColor( STYLE_NORMAL, RGB( 225, 0, 225 ) ),
   /* Emit */ StyleColor( STYLE_NORMAL, RGB( 225, 128, 25 ) ),
   /* My speech */ StyleColor( STYLE_NORMAL, RGB( 0, 0, 0 ) ),
   /* Broadcast */ StyleColor( STYLE_NORMAL, RGB( 182, 182, 182 ) ),
   /* Roll */ StyleColor( STYLE_NORMAL, RGB( 0, 0, 0 ) ),
   /* Shout */ StyleColor( STYLE_NORMAL, RGB( 0, 0, 0 ) ),
   /* Trade */ StyleColor( STYLE_NORMAL, RGB( 64, 64, 225 ) ),
   /* Dragonspeak */ StyleColor( STYLE_NORMAL, RGB( 0, 0, 0 ) ),
   /* Notify */ StyleColor( STYLE_NORMAL, RGB( 64, 64, 225 ) ),
   /* Query */ StyleColor( STYLE_NORMAL, RGB( 64, 64, 225 ) ),
   /* Highlight */ StyleColor( STYLE_BOLD, RGB( 0, 0, 225 ) ),
   /* Log */ StyleColor( STYLE_NORMAL, RGB( 0, 0, 0 ) ),
   /* History */ StyleColor( STYLE_NORMAL, RGB( 0, 0, 0 ) ),
   /* FurcLink */ StyleColor( STYLE_UNDERLINE, RGB( 0, 0, 225 ) ),
   /* Command Link */ StyleColor( STYLE_UNDERLINE, RGB( 0, 0, 225 ) ),
   /* Name */ StyleColor( STYLE_BOLD, RGB( 225, 0, 225 ) ),
   /* Input */ StyleColor( STYLE_NORMAL, RGB( 0, 0, 0 ) ),
   /* F2CmdLink */ StyleColor( STYLE_UNDERLINE, RGB( 0, 0, 225 ) ),
};

//////////////////////////////////////////////////////////////////////////

void ScriptTerm::init( HINSTANCE inst )
{
   assert( !s_init_count || inst == s_hinstance );

   if (s_init_count++ == 0)
   {
      s_hinstance = inst;

      // Load smileys.
      {
         unsigned loaded = fsh_to_imgls( core::get_client_root( ) + "\\patches\\default\\smileys.fsh",
                                         16, s_smileys );
         loaded += fsh_to_imgls( core::get_client_root( ) + "\\patches\\default\\smileys2.fsh",
                                 38, s_smileys + 16 );
                                 
         if (loaded != NUM_SMILIES)
         {
            ostringstream ss;
            ss << "Loaded " << loaded << "/" << NUM_SMILIES << " smileys.";
            core::system_errspeak( ss.str( ) );
         }
      }

      PoorTextBox_Init( s_hinstance );

      // Create link popup menu.
      s_link_menu = CreatePopupMenu( );
      AppendMenu( s_link_menu, MF_ENABLED | MF_STRING, ID_LINKMENU_COPY, "Copy URL" );
      AppendMenu( s_link_menu, MF_ENABLED | MF_STRING, ID_LINKMENU_LAUNCH, "Launch" );

      // Register window class.
      WNDCLASSEX wc;
      memset( &wc, 0, sizeof( wc ) );
      wc.cbSize        = sizeof( wc );
      wc.cbWndExtra    = sizeof( ScriptTerm* );
      wc.hbrBackground = GetStockBrush( COLOR_WINDOW );
      wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
      wc.hIcon         = LoadIcon( s_hinstance, MAKEINTRESOURCE( IDI_ICON_TERM ) );
      wc.hInstance     = s_hinstance;
      wc.lpfnWndProc   = wnd_proc;
      wc.lpszClassName = "ScriptTerm";
      wc.style         = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

      RegisterClassEx( &wc );
   }
}

void ScriptTerm::uninit( )
{
   if (s_init_count && --s_init_count == 0)
   {
      UnregisterClass( "ScriptTerm", s_hinstance );

      //Free the link popup menu.
      DestroyMenu( s_link_menu ); s_link_menu = NULL;

      PoorTextBox_Uninit( s_hinstance );

      // Free smileys.
      for (unsigned i = 0; i < NUM_SMILIES; ++i)
      {
         if (s_smileys[ i ])
         {
            ImageList_Destroy( s_smileys[ i ] );
            s_smileys[ i ] = NULL;
         }
      }

      s_hinstance = NULL;
   }
}

//////////////////////////////////////////////////////////////////////////

ScriptTerm::ScriptTerm( ScriptInst* owner /* = NULL */, HWND parent_wnd /* = NULL  */ )
   : m_owner( owner ), m_user_data( 0 ), m_parent_wnd( parent_wnd ), m_wnd( NULL ),
     m_title( "Script-Term" ), 
     m_font( NULL ), m_font_height( 0 ), m_font_width( 0 ), 
     m_input_bgcolor( RGB( 222, 212, 236 ) ), m_output_bgcolor( RGB( 239, 234, 217 ) ),
     m_last_trimmed( 0 )
{
   {
      RECT r = { 0, 0, STERM_DEF_W, STERM_DEF_H };
      AdjustWindowRectEx( &r, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW );
      m_wnd = CreateWindowEx( WS_EX_APPWINDOW, "ScriptTerm", m_title.c_str( ), WS_OVERLAPPEDWINDOW, 
                              CW_USEDEFAULT, CW_USEDEFAULT, r.right - r.left, r.bottom - r.top,
                              // Not actually its parent, teehee.
                              NULL, NULL, s_hinstance, (LPVOID) this );
      
      assert( m_wnd );
   }

   // Set up default attributes.
   {
      copy( s_default_types, s_default_types + NUM_TEXT_TYPES, m_types );
      {
         m_font = GetStockFont( DEFAULT_GUI_FONT );
         LOGFONT lf;
         GetObjectA( m_font, sizeof( lf ), &lf );
         m_font_name    = lf.lfFaceName;
         m_font_height  = abs( lf.lfHeight );
         m_font_width   = lf.lfWidth;
         m_font         = CreateFontIndirect( &lf );
      }
   }

   // Create child controls.
   {
      HWND ctrl = CreateWindowEx( 0, WC_POORTEXTBOX, NULL, WS_CHILD | WS_VISIBLE | PTBS_MULTILINE | PTBS_READONLY, 
                                  CW_USEDEFAULT, CW_USEDEFAULT, 1, 1, 
                                  m_wnd, (HMENU) IDC_STERM_OUTPUT, s_hinstance, NULL );
      PoorTextBox_SetBkgndColor( ctrl, m_output_bgcolor );
      PoorTextBox_SetDefaultColor( ctrl, m_types[ TEXT_OUTPUT ].color );
      PoorTextBox_SetDefaultStyle( ctrl, term_to_ptb_style( m_types[ TEXT_OUTPUT ].style ) );
      SetWindowFont( ctrl, m_font, FALSE );
      
      ctrl = CreateWindowEx( 0, WC_POORTEXTBOX, NULL, WS_CHILD | WS_VISIBLE | PTBS_MULTILINE, 
                             CW_USEDEFAULT, CW_USEDEFAULT, 1, 1, 
                             m_wnd, (HMENU) IDC_STERM_INPUT, s_hinstance, NULL );
      PoorTextBox_SetBkgndColor( ctrl, m_input_bgcolor );
      PoorTextBox_SetDefaultColor( ctrl, m_types[ TEXT_INPUT ].color );
      PoorTextBox_SetDefaultStyle( ctrl, term_to_ptb_style( m_types[ TEXT_INPUT ].style ) );
      PoorTextBox_SetTextLimit( ctrl, MAX_INPUT_LEN );
      SetWindowFont( ctrl, m_font, FALSE );
   }

   // Fit inside parent.
   fitControls( );

   // Center in parent window.
   {
      RECT parent_r;
      if (!IsIconic(m_parent_wnd)) { //Make sure the window isn't minimized - silly things happen if it is.
	  GetWindowRect( m_parent_wnd, &parent_r ); 
      SetWindowPos( m_wnd, NULL,
                    parent_r.left + ((parent_r.right - parent_r.left) - STERM_DEF_W) / 2,
                    parent_r.top + ((parent_r.bottom - parent_r.top) - STERM_DEF_H) / 2,
                    STERM_DEF_W, STERM_DEF_H, SWP_NOZORDER );
	  }
   }

   // Show.
   ShowWindow( m_wnd, SW_SHOWNOACTIVATE );
}

ScriptTerm::~ScriptTerm( )
{
   destroy( );
   DeleteFont( m_font );
}

void ScriptTerm::destroy( )
{
   if (m_wnd)
      DestroyWindow( m_wnd );
}

void ScriptTerm::clear( )
{
   if (m_wnd)
   {
      PoorTextBox_Flush( GetDlgItem( m_wnd, IDC_STERM_OUTPUT ) );
   }
}

void ScriptTerm::write( const string& line, const int* style /* = NULL */, const COLORREF* color /* = NULL  */ )
{
   if (!m_wnd)
      return;

   HWND ctrl_output = GetDlgItem( m_wnd, IDC_STERM_OUTPUT );

   // Buffer elements to send it all in one go, for efficiency.
   vector<PTBELEM> elems;

   // Go down to the next line.
   if (!PoorTextBox_IsEmpty( ctrl_output ))
   {
      PTBELEM elem;
      memset( &elem, 0, sizeof( elem ) );
      elem.type = PTB_ELEM_TYPE_BREAK;
      elems.push_back( elem );
   }

   {
      // No interpolation needs to be done.
      PTBELEM elem;
      memset( &elem, 0, sizeof( elem ) );
      elem.type  = PTB_ELEM_TYPE_STR;
      elem.text  = wcsdup( line.c_str( ) );
      elem.style = style ? term_to_ptb_style( *style ) : term_to_ptb_style( m_types[ TEXT_OUTPUT ].style );
      elem.color = color ? *color : m_types[ TEXT_OUTPUT ].color;
      elems.push_back( elem );
   }

   // Send all the buffered appends.
   if (!elems.empty( ))
      PoorTextBox_Append( ctrl_output, elems.size( ), &elems.front( ) );

   // Clean up.
   for (vector<PTBELEM>::iterator i = elems.begin( ); i != elems.end( ); ++i)
   {
      free( const_cast<wchar_t*>( i->alt_text ) );
      free( const_cast<wchar_t*>( i->text ) );
   }

   trimOutput( );
}

void ScriptTerm::print( const string& line, const int* style /* = NULL */, const COLORREF* color /* = NULL  */ )
{
   if (!m_wnd)
      return;

   HWND ctrl_output = GetDlgItem( m_wnd, IDC_STERM_OUTPUT );

   // Buffer elements to send it all in one go, for efficiency.
   vector<PTBELEM> elems;

   // Go down to the next line.
   if (!PoorTextBox_IsEmpty( ctrl_output ))
   {
      PTBELEM elem;
      memset( &elem, 0, sizeof( elem ) );
      elem.type = PTB_ELEM_TYPE_BREAK;
      elems.push_back( elem );
   }

   wstring wstr;

   for (ChatLexer lex( line.c_str( ), line.length( ) ); !lex.eof( ); )
   {
      switch (lex.next( ))
      {
      case ChatLexer::TOK_TEXT:
      case ChatLexer::TOK_DESCTAG:
         {
            PTBELEM elem;
            memset( &elem, 0, sizeof( elem ) );
            elem.type = PTB_ELEM_TYPE_STR;

            if (lex.tokStyle( ) & ChatLexer::STYLE_LINK) // Link.
            {
               // Set the target as alt text.
               elem.alt_text = wcsdup( lex.tokHref( ).c_str( ) );

               // Set the link's param.
               elem.param = addLink( lex.tokHref( ) );
               
               switch (guess_uri_type( lex.tokHref( ) ))
               {
               case URI_HTTP:
                  elem.style = style ? term_to_ptb_style( *style ) : term_to_ptb_style( m_types[ TEXT_LINK ].style );
                  elem.color = color ? *color : m_types[ TEXT_LINK].color;
                  break;
               case URI_FDL:
                  elem.style = style ? term_to_ptb_style( *style ) : term_to_ptb_style( m_types[ TEXT_FURCLINK ].style );
                  elem.color = color ? *color : m_types[ TEXT_FURCLINK ].color;
                  break;
               case URI_CMD:
                  elem.style = style ? term_to_ptb_style( *style ) : term_to_ptb_style( m_types[ TEXT_CMDLINK ].style );
                  elem.color = color ? *color : m_types[ TEXT_CMDLINK ].color;
                  break;
               case URI_F2CMD:
                  elem.style = style ? term_to_ptb_style( *style ) : term_to_ptb_style( m_types[ TEXT_F2CMDLINK ].style );
                  elem.color = color ? *color : m_types[ TEXT_F2CMDLINK ].color;
                  break;
               }

               elem.style |= PTB_ELEM_STYLE_LINK;
            }
            else // Regular text.
            {
               elem.style = style ? term_to_ptb_style( *style ) : term_to_ptb_style( m_types[ TEXT_OUTPUT ].style );
               elem.color = color ? *color : m_types[ TEXT_OUTPUT ].color;
            }

            if (!style)
            {
               elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_BOLD ? PTB_ELEM_STYLE_BOLD : 0;
               elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_ITALIC ? PTB_ELEM_STYLE_ITALIC : 0;
               elem.style |= lex.tokStyle( ) & ChatLexer::STYLE_UNDERLINE ? PTB_ELEM_STYLE_UNDERLINE : 0;
            }

            elem.text = wcsdup( xml_charset::latin_to_uni( string( lex.tokPtr( ), lex.tokLen( ) ) ).c_str( ) );

            elems.push_back( elem );
         }
         break;

      case ChatLexer::TOK_SMILEY:
         {
            const string smiley_code( lex.tokPtr( ), lex.tokLen( ) );
            unsigned smiley_index = 0;
            if (smiley_code[ 2 ] >= 'A' && smiley_code[ 2 ] <= 'Z')
               smiley_index = smiley_code[ 2 ] - 'A';
            else if (smiley_code[ 2 ] >= 'a' && smiley_code[ 2 ] <= 'y')
               smiley_index = smiley_code[ 2 ] - 'a' + 26;
            else if (smiley_code[ 2 ] >= '1' && smiley_code[ 2 ] <= '3')
               smiley_index = smiley_code[ 2 ] - '1' + 51;

            assert( smiley_index < NUM_SMILIES );

            if (s_smileys[ smiley_index ])
            {
               PTBELEM elem;
               memset( &elem, 0, sizeof( elem ) );
               elem.type = PTB_ELEM_TYPE_IMG;

               if (lex.tokStyle( ) & ChatLexer::STYLE_LINK) // Linked smiley.
               {
                  // Set the target as alt text.
                  elem.alt_text = wcsdup( lex.tokHref( ).c_str( ) );

                  // Set the link's param.
                  elem.param = addLink( lex.tokHref( ) );

                  // Always use the link color.
                  elem.style = PTB_ELEM_STYLE_LINK;
               }
               else // Regular smiley.
                  // Set the smiley code as alt text.
                  elem.alt_text = wcsdup( smiley_code.c_str( ) );

               assert( smiley_code.compare( 0, 2, "#S" ) == 0 );

               elem.imglist = s_smileys[ smiley_index ];
               elem.img_index = 0;

               elems.push_back( elem );
            }
         }
         break;
      }
   }

   // Send all the buffered appends.
   if (!elems.empty( ))
      PoorTextBox_Append( ctrl_output, elems.size( ), &elems.front( ) );

   // Clean up.
   for (vector<PTBELEM>::iterator i = elems.begin( ); i != elems.end( ); ++i)
   {
      free( const_cast<wchar_t*>( i->alt_text ) );
      free( const_cast<wchar_t*>( i->text ) );
   }

   trimOutput( );
}

void ScriptTerm::fitControls( )
{
   if (!m_wnd)
      return;

   HWND ctrl_output = GetDlgItem( m_wnd, IDC_STERM_OUTPUT );
   HWND ctrl_input  = GetDlgItem( m_wnd, IDC_STERM_INPUT );

   SIZE client_size;
   {
      RECT client_r;
      GetClientRect( m_wnd, &client_r );
      client_size.cx = client_r.right - client_r.left;
      client_size.cy = client_r.bottom - client_r.top;
   }

   SIZE input_size;
   PoorTextBox_GetContentSize( ctrl_input, &input_size );
   input_size.cx = client_size.cx;
   input_size.cy = min( (LONG) client_size.cy - STERM_OUTPUT_H, input_size.cy );
   input_size.cy = max( (LONG) STERM_INPUT_H, input_size.cy );

   SIZE output_size = { client_size.cx, client_size.cy - input_size.cy };
      
   // Avoid redundant transformations.
   POINT origin = { 0, 0 };
   ClientToScreen( m_wnd, &origin );
   RECT r;
   
   GetWindowRect( ctrl_input, &r );
   if (r.left != origin.x + 0 || r.top != origin.y + output_size.cy ||
       r.right - r.left != input_size.cx || r.bottom - r.top != input_size.cy)
   {
      SetWindowPos( ctrl_input, NULL, 
                    0, output_size.cy, 
                    input_size.cx, input_size.cy, 
                    SWP_NOZORDER );
   }

   GetWindowRect( ctrl_output, &r );
   if (r.left != origin.x + 0 || r.top != origin.y + 0 ||
       r.right - r.left != output_size.cx || r.bottom - r.top != output_size.cy)
   {
      SetWindowPos( ctrl_output, NULL, 
                    0, 0, 
                    output_size.cx, output_size.cy, 
                    SWP_NOZORDER );
   }
}

string ScriptTerm::getInputString( ) const
{
   string str;

   if (m_wnd)
   {
      PTBGETTEXT gettext;
      gettext.begin    = 0;
      gettext.end      = -1;
      gettext.text     = new wchar_t[ 2049 ];
      gettext.text_max = 2049;
      gettext.flags    = PTB_GETTEXT_IMGALT;

      if (PoorTextBox_GetText( GetDlgItem( m_wnd, IDC_STERM_INPUT ), &gettext ))
      {
         str = xml_charset::uni_to_latin( gettext.text );
         // Replace linebreaks with spaces.
         for (string::iterator i = str.begin( ); i != str.end( ); ++i)
         {
            if (*i == '\n')
               *i = ' ';
         }
      }

      delete [] gettext.text;
   }

   return str;
}

void ScriptTerm::handleInput( )
{
   string str = getInputString( );
   if (m_owner && !str.empty( ))
      m_owner->onTermInput( this, str );

   pushHistory( str );
   clearInput( );
}

void ScriptTerm::clearInput( )
{
   m_history_working_str.clear( );

   if (m_wnd)
      PoorTextBox_Flush( GetDlgItem( m_wnd, IDC_STERM_INPUT ) );

   fitControls( );
}

void ScriptTerm::trimOutput( )
{
   if (m_wnd)
   {
      // Only trim if the last trim was long enough ago.
      time_t curr_time = time( NULL );
      if (curr_time - m_last_trimmed >= TRIM_COOLDOWN)
      {
         m_last_trimmed = curr_time;
         HWND ctrl_output =  GetDlgItem( m_wnd, IDC_STERM_OUTPUT );
         // Only trim if it's a certain amount over.
         if (PoorTextBox_GetLineCount( ctrl_output ) > MAX_OUTPUT_LINES + MAX_OUTPUT_LINES_THRESHOLD)
            PoorTextBox_TrimLines( ctrl_output, MAX_OUTPUT_LINES );
      }
   }
}

void ScriptTerm::cycleHistory( )
{
   if (!m_wnd || m_history.empty( ))
      return;

   HWND ctrl_input = GetDlgItem( m_wnd, IDC_STERM_INPUT );

   string str = getInputString( );
   list<string>::const_iterator i = find( m_history.begin( ), m_history.end( ), str );

   // If the string is in history, start cycling from its position.
   if (i != m_history.end( ))
   {
      if (++i != m_history.end( ))
         SetWindowText( ctrl_input, i->c_str( ) );
      else // Hit the back.
      {
         // Restore the working string.
         SetWindowText( ctrl_input, m_history_working_str.c_str( ) );
      }
   }
   else
   {
      // Not in history, so replace the working string with this one and start
      // cycling from the start.
      assert( MAX_HISTORY > 1 );
      m_history_working_str = str;
      SetWindowText( ctrl_input, m_history.front( ).c_str( ) );
   }

   PoorTextBox_EnsureVisible( ctrl_input, -1 );
}

void ScriptTerm::pushHistory( const string& str )
{
   // If the string is already in history, just rotate that string to the front.
   list<string>::iterator i = find( m_history.begin( ), m_history.end( ), str );
   
   if (i != m_history.end( ))
   {
      if (i != m_history.begin( )) // Not already at the front.
      {
         m_history.erase( i );
         m_history.push_front( str );
      }
   }
   else
   {
      // Otherwise, free up some room and push it onto the back.

      assert( MAX_HISTORY > 0 );

      while (m_history.size( ) + 1 > MAX_HISTORY)
         m_history.pop_back( );

      m_history.push_front( str );
   }
}

int ScriptTerm::addLink( const string& url )
{
   // The last link should have the highest id.
   Link link = { m_links.empty( ) ? 0 : m_links.back( ).id + 1, url };
   
   assert( !findLink( link.id ) );

   m_links.push_back( link );

   while (m_links.size( ) > MAX_LINKS)
      m_links.pop_front( );

   return link.id;
}

const string* ScriptTerm::findLink( int id ) const
{
   for (list<Link>::const_iterator i = m_links.begin( );
        i != m_links.end( ); ++i)
   {
      if (i->id == id)
         return &i->url;
   }
   return NULL;
}

void ScriptTerm::launchUrl( const string& url )
{
   switch (guess_uri_type( url ))
   {
   case URI_F2CMD:
      core::push_command( url.substr( 5 ), true ); // Clicked f2 commands are always noecho.
      break;
   case URI_CMD:
      net::push_outbound_line(  url.substr( 6 ).c_str( ) );
      break;
   case URI_FDL:
      net::push_outbound_line( ("fdl " + url).c_str( ) );
      break;
   case URI_HTTP:
   default:
      // Make sure it starts with http://
      if (_strnicmp( url.c_str( ), "http://", 7 ))
         ShellExecute( NULL, "open", ("http://" + url).c_str( ), NULL, NULL, SW_SHOWNORMAL );
      else
         ShellExecute( NULL, "open", url.c_str( ), NULL, NULL, SW_SHOWNORMAL );
      break;
   }
}

void ScriptTerm::copyUrl( const string& url )
{
   HGLOBAL hmem = GlobalAlloc( GMEM_MOVEABLE, url.length( ) + 1 );
   char* buf = (char*) GlobalLock( hmem );
   strcpy( buf, url.c_str( ) );
   GlobalUnlock( hmem );

   OpenClipboard( m_wnd );
   EmptyClipboard( );

   SetClipboardData( CF_TEXT, hmem );

   CloseClipboard( );
}

void ScriptTerm::setTitle( const string& title )
{
   m_title = title;
   if (m_wnd)
      SetWindowText( m_wnd, m_title.c_str( ) );
}

void ScriptTerm::setFont( const string& name, unsigned height /* = 12 */, unsigned width /* = 0  */ )
{
   m_font_name   = name;
   m_font_height = height;
   m_font_width  = width;

   if (m_font)
      DeleteFont( m_font );

   LOGFONT lf;
   GetObjectA( GetStockFont( DEFAULT_GUI_FONT ), sizeof( lf ), &lf );
   strncpy( lf.lfFaceName, m_font_name.c_str( ), 32 );
   lf.lfHeight = 0 - m_font_height; // Yes, negative.
   lf.lfWidth  = m_font_width;
   m_font = CreateFontIndirect( &lf );

   if (m_font && m_wnd)
   {
      SetWindowFont( GetDlgItem( m_wnd, IDC_STERM_INPUT ), m_font, TRUE );
      SetWindowFont( GetDlgItem( m_wnd, IDC_STERM_OUTPUT ), m_font, TRUE );

      fitControls( );
   }
}

void ScriptTerm::setTextType( Text_Type type, const StyleColor& sc )
{
   m_types[ type ] = sc;

   if (type == TEXT_INPUT)
   {
      HWND ctrl = GetDlgItem( m_wnd, IDC_STERM_INPUT );
      PoorTextBox_SetDefaultColor( ctrl, sc.color );
      PoorTextBox_SetDefaultStyle( ctrl, term_to_ptb_style( sc.style ) );
   }
   else if (type == TEXT_OUTPUT)
   {
      HWND ctrl = GetDlgItem( m_wnd, IDC_STERM_OUTPUT );
      PoorTextBox_SetDefaultColor( ctrl, sc.color );
      PoorTextBox_SetDefaultStyle( ctrl, term_to_ptb_style( sc.style ) );
   }
}

void ScriptTerm::setBgColors( COLORREF input, COLORREF output )
{
   m_input_bgcolor  = input;
   m_output_bgcolor = output;

   if (m_wnd)
   {
      PoorTextBox_SetBkgndColor( GetDlgItem( m_wnd, IDC_STERM_INPUT ), m_input_bgcolor );
      PoorTextBox_SetBkgndColor( GetDlgItem( m_wnd, IDC_STERM_OUTPUT ), m_output_bgcolor );
   }
}

LRESULT CALLBACK ScriptTerm::wnd_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
#define _INST_PTR( ) ((ScriptTerm*) GetWindowLongPtr( wnd, 0 ))

   switch (msg)
   {
   case WM_CREATE:
      assert( ((CREATESTRUCT*) lparam)->lpCreateParams );
      SetWindowLongPtr( wnd, 0, (LONG_PTR) ((CREATESTRUCT*) lparam)->lpCreateParams );
      return 0;

   case WM_GETMINMAXINFO:
      {
         MINMAXINFO* mmi = (MINMAXINFO*) lparam;
         mmi->ptMinTrackSize.x = STERM_MIN_W;
         mmi->ptMinTrackSize.y = STERM_MIN_H;
      }
      return 0;

   case WM_SIZE:
      {
         ScriptTerm* inst = _INST_PTR( );
         if (inst && inst->m_wnd)
            inst->fitControls( );
      }
      break;

   case WM_DESTROY:
      {
         ScriptTerm* inst = _INST_PTR( );
         if (inst->m_wnd)
         {
            inst->m_wnd = NULL;
            if (inst->m_owner)
               inst->m_owner->onTermDestroy( inst );
         }
      }
      break;

   case WM_SETFOCUS:
      {
         // Give the input field focus.
         SetFocus( GetDlgItem( wnd, IDC_STERM_INPUT ) );
      }
      break;

   case furn2::F2M_ISNOTDLGWND:
         return TRUE;

   case WM_NOTIFY:
      switch (((NMHDR*) lparam)->idFrom)
      {
      case IDC_STERM_INPUT:
         switch (((NMHDR*) lparam)->code)
         {
         case PTBN_CHANGED:
            _INST_PTR( )->fitControls( );
            return 0;

         case PTBN_RETURN:
            _INST_PTR( )->handleInput( );
            return 1;

         case PTBN_ESCAPE:
            _INST_PTR( )->clearInput( );
            return 1;

         case PTBN_CTRLKEY:
            if (((NMPTBCTRLKEY*) lparam)->vk_code == 0x49) // I
            {
               _INST_PTR( )->cycleHistory( );
               return 1;
            }
            break;
         }
         return 0;
         
      case IDC_STERM_OUTPUT:
         switch (((NMHDR*) lparam)->code)
         {
         case PTBN_TAB:
            // Change focus to the input on tab.
            SetFocus( GetDlgItem( wnd, IDC_STERM_INPUT ) );
            return 1;

         case PTBN_ELEMACTION:
            {
               NMPTBELEMACTION* nm = (NMPTBELEMACTION*) lparam;

               if (nm->action == PTB_ELEMACTION_CLICK) // Link clicked.
               {
                  ScriptTerm* inst = _INST_PTR( );
                  const string* url = inst->findLink( (int) nm->param );
                  if (url)
                  {
                     inst->launchUrl( *url );
                     return 1;
                  }
               }
               else if (nm->action == PTB_ELEMACTION_RCLICK) // Link right-clicked.
               {
                  ScriptTerm* inst = _INST_PTR( );
                  const string* url = inst->findLink( (int) nm->param );
                  if (url)
                  {
                     // Show the popup menu.
                     POINT cursor;
                     GetCursorPos( &cursor );
                     // Has to be synchronous so we know what link was clicked.
                     int cmd = TrackPopupMenu( ScriptTerm::s_link_menu, TPM_RIGHTBUTTON | TPM_RETURNCMD, 
                                               cursor.x, cursor.y, 0, wnd, NULL );

                     if (cmd == ID_LINKMENU_COPY)
                        inst->copyUrl( *url );
                     else if (cmd == ID_LINKMENU_LAUNCH)
                        inst->launchUrl( *url );                        

                     return 1;
                  }
               }
            }
            break;
         }
         return 0;
      }
      break;
   }

   return DefWindowProc( wnd, msg, wparam, lparam );
#undef _INST_PTR
}

//////////////////////////////////////////////////////////////////////////

unsigned fsh_to_imgls( const string& filename, unsigned num, HIMAGELIST out[] )
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

      for (unsigned i = 0; i < num && i < fsh.getNumShapes( ); ++i)
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
                                                ILC_COLOR24 | ILC_MASK, 1, 0 );

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

wchar_t* wcsdup( const char* sz )
{
   const size_t len = strlen( sz ) + 1;
   wchar_t* wc = (wchar_t*) malloc( len * sizeof( wchar_t ) );
   for (wchar_t* dst = wc; *sz; ++sz, ++dst)
      *dst = (unsigned short) *(unsigned char*) sz;
   wc[ len - 1 ] = L'\0';
   return wc;
}

int term_to_ptb_style( const int style )
{
   int ptb_style = PTB_ELEM_STYLE_NORMAL;
   ptb_style |= style & ScriptTerm::STYLE_BOLD ? PTB_ELEM_STYLE_BOLD : 0;
   ptb_style |= style & ScriptTerm::STYLE_ITALIC ? PTB_ELEM_STYLE_ITALIC : 0;
   ptb_style |= style & ScriptTerm::STYLE_UNDERLINE ? PTB_ELEM_STYLE_UNDERLINE : 0;
   return ptb_style;
}

Uri_Type guess_uri_type( const string& uri )
{
   if (!uri.compare( 0, 5, "f2://", 5 ))
      return URI_F2CMD;
   else if (!uri.compare( 0, 6, "cmd://", 6 ))
      return URI_CMD;
   else if (!uri.compare( 0, 7, "furc://", 8 ))
      return URI_FDL;
   
   return URI_HTTP;
}