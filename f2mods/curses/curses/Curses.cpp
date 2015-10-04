#include "curses.h"
#include "common/IniFile.h"
#include "common/NetBuffer.h"
#include "yiffurc/avatar.h"
#include "yiffurc/net.h"
#include "yiffurc/auth.h"
#include "tinyxml/tinyxml.h"
#include "resource.h"
#include <vector>
#include <fstream>
#include <ShlObj.h>

namespace curses {

Curses* Curses::s_singleton = NULL;
void* Curses::s_old_createfilea = NULL;
void* Curses::s_old_deletefilea = NULL;

//////////////////////////////////////////////////////////////////////////

enum { DEFAULT_OPTIONS = Curses::OPT_AUTOSTART | Curses::OPT_REMEMBERPW };
enum { LBN_RCLICK = 100, LBN_RCLICKITEM };

//////////////////////////////////////////////////////////////////////////

struct OptionsDlgData
{
   Curses* curses;
   HMENU   popup_menu;
};

//////////////////////////////////////////////////////////////////////////

static LRESULT CALLBACK lb_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
static string get_default_ini( const string& settings_file );

//////////////////////////////////////////////////////////////////////////

class Curses::Job
{
public:
   virtual bool onWndProc( WNDPROCPARAMS* params ) = 0;
   virtual void onLoggedIn( ) = 0;
   virtual void onDisconnected( ) = 0;
   virtual void onNetIn( const string& line, int from, size_t num )  = 0;
   virtual void onNetOut( const string& line, int from, size_t num ) = 0;

   virtual ~Job( ) { }
};

class Curses::JobDefaultLogin : public Curses::Job
{
public:
   JobDefaultLogin( );
   ~JobDefaultLogin( );
   virtual bool onWndProc( WNDPROCPARAMS* params );
   virtual void onLoggedIn( );
   virtual void onDisconnected( );
   virtual void onNetIn( const string& line, int from, size_t num );
   virtual void onNetOut( const string& line, int from, size_t num );
private:
};

class Curses::JobLogin : public Curses::Job
{
public:
   JobLogin( );
   ~JobLogin( );
   virtual bool onWndProc( WNDPROCPARAMS* params );
   virtual void onLoggedIn( );
   virtual void onDisconnected( );
   virtual void onNetIn( const string& line, int from, size_t num );
   virtual void onNetOut( const string& line, int from, size_t num );
private:
   LOGININFO            m_login;
   yiffurc::NET_SESSION m_session;
   vector<size_t>       m_lines_allowed;
};

class Curses::JobReLogin : public Curses::Job
{
   enum { WM_WSAMSG = WM_USER };
public:
   JobReLogin( );
   ~JobReLogin( );
   virtual bool onWndProc( WNDPROCPARAMS* params );
   virtual void onLoggedIn( );
   virtual void onDisconnected( );
   virtual void onNetIn( const string& line, int from, size_t num );
   virtual void onNetOut( const string& line, int from, size_t num );
private:
   static INT_PTR CALLBACK dlg_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
   void onSocketRead( );
   void printStatus( const string& line );
   void tryReplaceMainSocket( );
private:
   LOGININFO            m_login;
   yiffurc::NET_SESSION m_session;
   SOCKET               m_socket;
   string               m_linebuf;
   bool                 m_close_hidden;
   HWND                 m_dlg;
};

class Curses::JobRegister : public Curses::Job
{
public:
   JobRegister( const string& email );
   ~JobRegister( );
   virtual bool onWndProc( WNDPROCPARAMS* params );
   virtual void onLoggedIn( );
   virtual void onDisconnected( );
   virtual void onNetIn( const string& line, int from, size_t num );
   virtual void onNetOut( const string& line, int from, size_t num );
private:
   static INT_PTR CALLBACK dlg_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
   static INT_PTR CALLBACK email_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
   void create( );
   void printStatus( const string& line );
private:
   LOGININFO            m_login;
   string               m_email;
   yiffurc::NET_SESSION m_session;
   HWND                 m_dlg_email;
   HWND                 m_dlg;
};

class Curses::JobReRegister : public Curses::Job
{
public:
   JobReRegister( const string& email );
   ~JobReRegister( );
   virtual bool onWndProc( WNDPROCPARAMS* params );
   virtual void onLoggedIn( );
   virtual void onDisconnected( );
   virtual void onNetIn( const string& line, int from, size_t num );
   virtual void onNetOut( const string& line, int from, size_t num );
private:
   static INT_PTR CALLBACK dlg_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
private:
   LOGININFO            m_login;
   yiffurc::NET_SESSION m_session;
   SOCKET               m_socket;
   string               m_linebuf;
   HWND                 m_dlg_email;
   HWND                 m_dlg;
};

//////////////////////////////////////////////////////////////////////////

Curses::Curses( HINSTANCE inst, unsigned int mod )
   : m_wnd( NULL ), m_inst( inst ), m_logged_in( false ), 
     m_module( mod ), m_enabled( false ), m_main_socket( INVALID_SOCKET ), m_screen( NULL ), 
     m_login_info( NULL ), m_job( NULL ), m_options( DEFAULT_OPTIONS ),
     m_options_dlg( NULL )
{
   s_singleton = this;

   CoInitializeEx( NULL, COINIT_APARTMENTTHREADED );
   readOptions( );

   // If the localdir.ini file exists, set the localdir member.
   {
      string path = string( f2_clroot( ) ) + "\\localdir.ini";
      HANDLE f = ::CreateFile( path.c_str( ), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0,NULL );
      if (f != INVALID_HANDLE_VALUE)
      {
         DWORD filesize = GetFileSize( f, NULL );
         char* buf = new char[ filesize + 1 ];
         ReadFile( f, buf, filesize, &filesize, NULL );
         buf[ filesize ] = '\0';
         DWORD atts = GetFileAttributes( buf );
         if (atts != INVALID_FILE_ATTRIBUTES && atts & FILE_ATTRIBUTE_DIRECTORY)
            m_localdir = buf;
         else
            m_localdir = f2_clroot( );
      }
   }

   // Find the settings file.
   // If the localdir is set, use that settings.ini
   if (m_localdir.length( ))
      m_settings_file = m_localdir + "\\settings.ini";
   else
   {
      // Try the per-user settings.ini file.
      m_settings_file = string( f2_cldataroot( ) ) + "\\settings.ini";
      if (GetFileAttributes( m_settings_file.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
         // Try the program root.
         m_settings_file = string( f2_clroot( ) ) + "\\settings.ini";
   }

   // Get the default INI.
   m_default_ini = get_default_ini( m_settings_file );

   s_old_createfilea = f2_hookapi( "kernel32.dll", "CreateFileA", CreateFileA, false );
   s_old_deletefilea = f2_hookapi( "kernel32.dll", "DeleteFileA", DeleteFileA, false );
}

Curses::~Curses( )
{
   if (m_options_dlg)
      DestroyWindow( m_options_dlg );

   terminateJob( );
   delete m_login_info;
   delete m_screen;

   CoUninitialize( );
   s_singleton = NULL;
}

void Curses::setWnd( HWND wnd )
{
   if (m_screen)
   {
      screenClose( );
      m_wnd = wnd;
      showScreen( );
   }
   else
      m_wnd = wnd;
}

void Curses::onClientScreen( Client_Screen screen )
{

}

bool Curses::onCommand( int argc, const char* argv[] )
{
   if (argc >= 1 && !_stricmp( argv[ 0 ], "curses" ))
   {
      showScreen( );
      return true;
   }
   return false;
}

void Curses::onEnable( bool onoff )
{
   m_enabled = onoff;
}

bool Curses::onWndProc( WNDPROCPARAMS* params )
{
   // Pass it on.
   if (m_screen)
   {
      if (m_screen->onWndProc( params ))
         return true;
   }
   if (m_job)
   {
      if (m_job->onWndProc( params ))
         return true;
   }
      
   return false;
}

void Curses::onConnected( )
{

}

void Curses::onLoggedIn( )
{
   m_logged_in = true;

   if (m_job)
      m_job->onLoggedIn( );

   m_connect_buf.clear( );
}

void Curses::onDisconnected( )
{
   m_logged_in = false;

   if (m_job)
      m_job->onDisconnected( );

   m_connect_buf.clear( );

   if (m_screen)
      m_screen->setConnected( false );
}

void Curses::onNetIn( const string& line, int from, size_t num )
{
   if (m_job)
      m_job->onNetIn( line, from, num );
}

void Curses::onNetOut( const string& line, int from, size_t num )
{
   /* These days, the client will just start trying to log in right away
   *  after connection at the title screen.  Only when it logs in successfully
   *  does it switch to marbled. */

   // If not logged in and this is a login attempt, show the screen.
   if (m_enabled && !m_screen && !m_logged_in && !m_job && 
       !line.compare( 0, 7, "connect" ) && from == FROM_CLIENT)
   {

      if (!m_login_info)
         m_screen = new CursesScreen( );
      else
         m_screen = new CursesScreen( *m_login_info );
      m_screen->setConnected( true );
      m_screen->setCloseable( false );
   }

   // If the screen is being displayed and there's no job, just buffer
   // anything being sent by the client.
   if (m_screen && !m_job)
   {
      if (from == FROM_CLIENT)
      {
         // Ignore subsequent login attempts because the client just keeps sending
         // them if it isn't getting a response.
         if (!line.compare( 0, 7, "connect" ) && 
             m_connect_buf.end( ) != find( m_connect_buf.begin( ), m_connect_buf.end( ), line ))
         {
            // Ignored.
         }
         else
         {
            m_connect_buf.push_back( line );
         }
         f2_cancelline( );
      }
   }
   else if (m_job)
      m_job->onNetOut( line, from, num );
}

void Curses::onSetMainSocket( SOCKET socket )
{
   m_main_socket = socket;
}

void Curses::screenPlay( )
{
   assert( m_screen );

   terminateJob( );

   if (!m_login_info)
      m_login_info = new LOGININFO;
   m_screen->getLoginInfo( m_login_info );

   switch (furn2::f2_connlvl( ))
   {
   case CONNLVL_CONNECTED:
   case CONNLVL_MOTDEND:
      delete m_screen;
      m_screen = NULL;
      m_job = new JobLogin( );
      break;
   case CONNLVL_LOGGEDIN:
      m_job = new JobReLogin( );
   }
}

void Curses::screenClose( )
{
   assert( m_screen );

   delete m_screen;
   m_screen = NULL;

   terminateJob( );

   switch (furn2::f2_connlvl( ))
   {
   case CONNLVL_CONNECTED:
   case CONNLVL_MOTDEND:
      // Create the default job that does nothing.
      m_job = new JobDefaultLogin( );
      // Send out all buffered lines from the client.
      while (!m_connect_buf.empty( ))
      {
         f2_netoutline( m_connect_buf.front( ).c_str( ), m_connect_buf.front( ).length( ) );
         m_connect_buf.pop_front( );
      }
      break;
   }
}

void Curses::screenRegister( )
{
   assert( m_screen );

   terminateJob( );

   if (!m_login_info)
      m_login_info = new LOGININFO;
   m_screen->getLoginInfo( m_login_info );


   if (!m_logged_in)
      m_job = new JobRegister( m_last_email );
   else
   {
      MessageBox( m_wnd, "Character registration can currently only be performed before logging in.",
                  "A lazy programmer is me.", MB_OK | MB_ICONINFORMATION );
   }
   /*
   else
      m_job = new JobReRegister( );
      */
}

void Curses::showOptions( )
{
   if (!m_options_dlg || !IsWindow( m_options_dlg ))
   {
      m_options_dlg = CreateDialogParam( m_inst, MAKEINTRESOURCE( IDD_OPTIONS ), m_wnd,
                                         options_proc, (LPARAM) NULL );
   }
   else
      SetForegroundWindow( m_options_dlg );
}

void Curses::showScreen( )
{
   if (m_wnd && IsWindow( m_wnd ) && !m_screen)
   {
      if (!m_login_info)
         m_screen = new CursesScreen( );
      else
         m_screen = new CursesScreen( *m_login_info );
      m_screen->setConnected( true );
   }
   else
      f2_errspeak( "Curses screen already being displayed." );
}

void Curses::terminateJob( )
{
   Job* j = m_job;
   m_job = NULL;
   delete j;
}

void Curses::readOptions( )
{
   // Set up defaults.
   m_options = OPT_AUTOSTART | OPT_REMEMBERPW;
   m_ini_paths.clear( );

   m_ini_paths.push_back( f2_clroot( ) );
   m_ini_paths.push_back( string( f2_cldocroot( ) ) + "\\Furcadia Characters" );

   //  Prefer the per-user location.

   string file = string( f2_moddocroot( m_module ) ) + "//options.xml";

   if (GetFileAttributes( file.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
   {
      file = string( f2_modroot( m_module ) ) + "//options.xml";
      if (GetFileAttributes( file.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
         return;
   }

   TiXmlDocument doc( file );
   if ((!doc.LoadFile( ) || doc.Error( )))
   {
      f2_errspeak( "Curses: Failed to load options file." );
      return;
   }

   const TiXmlElement* xml_root = doc.RootElement( );

   if (!xml_root || xml_root->ValueStr( ) != "options")
   {
      f2_errspeak( "Curses: Options file is malformed." );
      return;
   }

   m_options = OPT_AUTOSTART | OPT_REMEMBERPW;

   {
      const TiXmlElement* xml_autostart = xml_root->FirstChildElement( "autoStart" );
      if (xml_autostart)
      {
         if (xml_autostart->GetText( ) && _stricmp( xml_autostart->GetText( ), "true" ))
            m_options &= ~OPT_AUTOSTART;
      }
   }

   {
      const TiXmlElement* xml_rememberpw = xml_root->FirstChildElement( "rememberPw" );
      if (xml_rememberpw)
      {
         if (xml_rememberpw->GetText( ) && _stricmp( xml_rememberpw->GetText( ), "true" ))
            m_options &= ~OPT_REMEMBERPW;
      }
   }   

   m_ini_paths.clear( );

   {
      const TiXmlElement* xml_folders = xml_root->FirstChildElement( "charFolders" );
      if (xml_folders)
      {
         for (const TiXmlElement* folder = xml_folders->FirstChildElement( "folder" );
              folder; folder = folder->NextSiblingElement( "folder" ))
         {
            if (folder->GetText( ) && 
                m_ini_paths.end( ) == find( m_ini_paths.begin( ), m_ini_paths.end( ), string( folder->GetText( ) ) ))
               m_ini_paths.push_back( folder->GetText( ) );
         }
      }
   }
}

void Curses::writeOptions( )
{
   string file = string( f2_moddocroot( m_module ) ) + "\\options.xml";

   ofstream fs( file.c_str( ) );

   fs << "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n"
      << "<options>\n"
      << "\t<autoStart>" << (m_options & OPT_AUTOSTART ? "true" : "false") << "</autoStart>\n"
      << "\t<rememberPw>" << (m_options & OPT_REMEMBERPW ? "true" : "false") << "</rememberPw>\n";

   fs << "\t<charFolders>\n";
   for (list<string>::const_iterator i = m_ini_paths.begin( ); i != m_ini_paths.end( ); ++i)
      fs << "\t\t<folder>" << *i << "</folder>\n"; // Beware paths with &!

   fs << "\t</charFolders>\n";
   
   fs << "</options>";
}

void Curses::setDefaultIni( const string& ini )
{
   IniFile furc_settings( m_settings_file );
   if (furc_settings.open( ))
   {
      furc_settings.write( "DefaultINI", ini, "general" );
      furc_settings.close( );
      m_default_ini = get_default_ini( m_settings_file );
   }
}

INT_PTR CALLBACK Curses::options_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
#define _DATA_PTR( ) ((OptionsDlgData*) GetWindowLongPtr( wnd, GWLP_USERDATA ))

   switch (msg)
   {
   case WM_INITDIALOG:
      {
         OptionsDlgData* data = new OptionsDlgData;
         data->curses = Curses::singleton( );
         data->popup_menu = LoadMenu( data->curses->m_inst, MAKEINTRESOURCE( IDR_OPTIONS ) );
         SetWindowLongPtr( wnd, GWLP_USERDATA, (LONG_PTR) data );

         CheckDlgButton( wnd, IDC_AUTOENABLE, 
                         data->curses->m_options & OPT_AUTOSTART ? BST_CHECKED : BST_UNCHECKED );
         CheckDlgButton( wnd, IDC_REMEMBERPW, 
                         data->curses->m_options & OPT_REMEMBERPW ? BST_CHECKED : BST_UNCHECKED );

         HWND ctrl_folders = GetDlgItem( wnd, IDC_FOLDERS );

         // Fill up the folders listbox.
         for (list<string>::const_iterator i = data->curses->m_ini_paths.begin( );
              i != data->curses->m_ini_paths.end( ); ++i)
            ListBox_AddString( ctrl_folders, i->c_str( ) );

         // Subclass the folders listbox for right-clicks.
         {
            SetWindowLongPtr( ctrl_folders, GWLP_USERDATA, (LONG_PTR)
                              GetWindowLongPtr( ctrl_folders, GWLP_WNDPROC ) );
            SetWindowLongPtr( GetDlgItem( wnd, IDC_FOLDERS ), GWLP_WNDPROC, (LONG_PTR) lb_proc );
         }

         // Center and display.
         RECT parent_rect, wnd_rect;
         GetWindowRect( GetParent( wnd ), &parent_rect );
         GetWindowRect( wnd, &wnd_rect );

         SetWindowPos( wnd, NULL, 
                       parent_rect.left + 
                        (parent_rect.right - parent_rect.left) / 2 - 
                        (wnd_rect.right - wnd_rect.left) / 2,
                       parent_rect.top + 
                        (parent_rect.bottom - parent_rect.top) / 2 -
                        (wnd_rect.bottom - wnd_rect.top ) / 2,
                       0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );

         return TRUE;
      }

   case WM_COMMAND:
      if (HIWORD( wparam ) == 0 && !lparam)
      {
         // Menu item.
         switch (LOWORD( wparam ))
         {
         case ID_FOLDERS_ADD:
            {
               // Prompt for new folder to add.
               BROWSEINFO bi;
               memset( &bi, 0, sizeof( bi ) );
               bi.hwndOwner      = wnd;
               bi.lpszTitle      = "Add Characters Search Folder";
               bi.pszDisplayName = NULL;
               bi.ulFlags        = BIF_USENEWUI;
               LPITEMIDLIST pidl = SHBrowseForFolder( &bi );

               if (pidl)
               {
                  // Add it to the listbox.
                  char* sz_path = new char[ MAX_PATH ];
                  SHGetPathFromIDList( pidl, sz_path );

                  ListBox_AddString( GetDlgItem( wnd, IDC_FOLDERS ), sz_path );
                  
                  delete [] sz_path;

                  IMalloc* shmalloc = NULL;
                  SHGetMalloc( &shmalloc );
                  assert( shmalloc );
                  shmalloc->Free( pidl );
               }

               return TRUE;
            }

         case ID_FOLDERS_REMOVE:
            {
               // Delete the entry.
               HWND ctrl_folders = GetDlgItem( wnd, IDC_FOLDERS );
               ListBox_DeleteString( ctrl_folders, ListBox_GetCurSel( ctrl_folders ) );

               return TRUE;
            }
         }
      }

      // Say it's a control.
      switch (LOWORD( wparam ))
      {
      case IDOK:
         if (HIWORD( wparam ) == BN_CLICKED)
         {
            OptionsDlgData* data = _DATA_PTR( );

            // Get option flags.
            {
               long options = 0;
               options |= IsDlgButtonChecked( wnd, IDC_AUTOENABLE ) ? OPT_AUTOSTART : 0;
               options |= IsDlgButtonChecked( wnd, IDC_REMEMBERPW ) ? OPT_REMEMBERPW : 0;
               data->curses->m_options = options;
            }
            
            // Get INI folders.
            {
               data->curses->m_ini_paths.clear( );

               HWND ctrl_folders = GetDlgItem( wnd, IDC_FOLDERS );
               char sz_path[ MAX_PATH ];
               const unsigned count = ListBox_GetCount( ctrl_folders );
               for (unsigned i = 0; i < count; ++i)
               {
                  ListBox_GetText( ctrl_folders, i, sz_path );
                  // Avoid duplicates.
                  if (data->curses->m_ini_paths.end( ) == 
                      find( data->curses->m_ini_paths.begin( ), data->curses->m_ini_paths.end( ), string( sz_path ) ))
                  {
                     data->curses->m_ini_paths.push_back( sz_path );
                  }
               }
            }

            // Save.
            data->curses->writeOptions( );

            DestroyWindow( wnd );
            return TRUE;
         }

      case IDCANCEL:
         if (HIWORD( wparam ) == BN_CLICKED)
         {
            DestroyWindow( wnd );
            return TRUE;
         }

      case IDC_FOLDERS:
         if (HIWORD( wparam ) == LBN_RCLICK)
         {
            // Display the add/remove popup menu.
            OptionsDlgData* data = _DATA_PTR( );

            HMENU popup = GetSubMenu( data->popup_menu, 0 );
            EnableMenuItem( popup, ID_FOLDERS_REMOVE, MF_DISABLED | MF_GRAYED );

            POINT cursor;
            GetCursorPos( &cursor );
            TrackPopupMenu( popup, TPM_RIGHTBUTTON, cursor.x, cursor.y,
               0, wnd, NULL );
            return TRUE;
         }
         else if (HIWORD( wparam ) == LBN_RCLICKITEM )
         {
            // Display the add/remove popup menu.
            OptionsDlgData* data = _DATA_PTR( );

            HMENU popup = GetSubMenu( data->popup_menu, 0 );
            EnableMenuItem( popup, ID_FOLDERS_REMOVE, MF_ENABLED );

            POINT cursor;
            GetCursorPos( &cursor );
            TrackPopupMenu( popup, TPM_RIGHTBUTTON, cursor.x, cursor.y,
                            0, wnd, NULL );
            return TRUE;
         }
      }
      break;

   case WM_DESTROY:
      {
         OptionsDlgData* data = _DATA_PTR( );
         data->curses->m_options_dlg = NULL;
         delete data;
      }
      break;

   case WM_ACTIVATE:
      if (LOWORD( wparam ) != WA_INACTIVE)
         f2_setcurdialog( wnd );
   }

   return FALSE;
#undef _DATA_PTR
}

//////////////////////////////////////////////////////////////////////////

Curses::JobDefaultLogin::JobDefaultLogin( )
{

}

Curses::JobDefaultLogin::~JobDefaultLogin( )
{

}

void Curses::JobDefaultLogin::onDisconnected( )
{
   Curses::singleton( )->terminateJob( );
}

void Curses::JobDefaultLogin::onLoggedIn( )
{
   Curses::singleton( )->terminateJob( );
}

void Curses::JobDefaultLogin::onNetIn( const string& line, int from, size_t num )
{
   // I do nothing. :)
}

void Curses::JobDefaultLogin::onNetOut( const string& line, int from, size_t num )
{
   // I do nothing. :)
}

bool Curses::JobDefaultLogin::onWndProc( WNDPROCPARAMS* params )
{
   return false;
}

//////////////////////////////////////////////////////////////////////////

Curses::JobLogin::JobLogin( )
   : m_login( *Curses::singleton( )->m_login_info )
{
   memset( &m_session, 0, sizeof( m_session ) );
   m_session.level = yiffurc::NET_LEVEL_MOTDEND;
}

Curses::JobLogin::~JobLogin( )
{

}

void Curses::JobLogin::onDisconnected( )
{
   Curses::singleton( )->terminateJob( );
}

void Curses::JobLogin::onLoggedIn( )
{
   f2__setplayerinfo( m_login.name.c_str( ), m_login.password.c_str( ),
                      ("t" + m_login.color.str( )).c_str( ), m_login.description.c_str( ), NULL );
   SetWindowText( Curses::singleton( )->m_wnd, ("Furcadia - " + unpipen( m_login.name )).c_str( ) );

   Curses::singleton( )->terminateJob( );
}

void Curses::JobLogin::onNetIn( const string& line, int from, size_t num )
{

}

void Curses::JobLogin::onNetOut( const string& line, int from, size_t num )
{
   using namespace yiffurc;
   const C2S* c2s = c2s_parse( line.c_str( ), line.length( ), &m_session );

   if (c2s)
   {
      switch (c2s->type)
      {
      case C2S_TYPE_LOGIN:
         {
            // Piggy-back on the client's connect attempt and steal its mac id.

            const C2S_LOGIN* const c2s_login = (C2S_LOGIN*) c2s;
            const char* cmd = c2s_enc_login( m_login.name.c_str( ), 
                                             m_login.password.c_str( ), 
                                             c2s_login->mac_id );
            f2_reviseline( cmd );
            yiffurc_release( cmd );

            const string cc = "t" + m_login.color.str( );
            cmd = c2s_enc_setcolor( cc.c_str( ), cc.c_str( ) + ColorCodePal::COLORS_LEN + 1 );

            m_lines_allowed.push_back( f2_netoutline( cmd, strlen( cmd ) ) );
            yiffurc_release( cmd );

            cmd = c2s_enc_setdesc( m_login.description.c_str( ) );
            m_lines_allowed.push_back( f2_netoutline( cmd, strlen( cmd ) ) );
            yiffurc_release( cmd );
         }
         break;

      case C2S_TYPE_SETCOLOR:
         {
            // Don't let the client or anyone send their own color code.
            if (find( m_lines_allowed.begin( ), m_lines_allowed.end( ), num ) == m_lines_allowed.end( ))
               f2_cancelline( );
         }
         break;

      case C2S_TYPE_SETDESC:
         {
            // Don't let the client or anyone send their own desc.
            if (find( m_lines_allowed.begin( ), m_lines_allowed.end( ), num ) == m_lines_allowed.end( ))
               f2_cancelline( );
         }
         break;
      }
      yiffurc_release( c2s );
   }
}

bool Curses::JobLogin::onWndProc( WNDPROCPARAMS* params )
{
   return false;
}

//////////////////////////////////////////////////////////////////////////

Curses::JobReLogin::JobReLogin( )
   : m_login( *Curses::singleton( )->m_login_info ), m_socket( INVALID_SOCKET ),
     m_close_hidden( false )
{
   memset( &m_session, 0, sizeof( m_session ) );

   m_dlg = CreateDialogParam( Curses::singleton( )->m_inst, MAKEINTRESOURCE( IDD_JOB_DLG ),
                              Curses::singleton( )->m_wnd, dlg_proc, (LPARAM) this );

   m_socket = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
   assert( m_socket != INVALID_SOCKET );

   WSAAsyncSelect( m_socket, m_dlg, WM_WSAMSG, FD_READ | FD_CONNECT | FD_CLOSE );

   printStatus( "Creating secondary connection to game server." );

   sockaddr_in addr;
   int addr_size = sizeof( addr );
   getpeername( Curses::singleton( )->m_main_socket, (sockaddr*) &addr, &addr_size );
   connect( m_socket, (sockaddr*) &addr, sizeof( addr ) );

   printStatus( string( "Connecting to game server " ) + 
                inet_ntoa( addr.sin_addr ) + string( "..." ) );
}

Curses::JobReLogin::~JobReLogin( )
{
   shutdown( m_socket, -1 );
   closesocket( m_socket );
   if (m_dlg)
      DestroyWindow( m_dlg );

   // If we have an outstanding close event, let the client know now.
   if (m_close_hidden)
      PostMessage( Curses::singleton( )->m_wnd, WM_USER, (WPARAM) Curses::singleton( )->m_main_socket, 
                   MAKELPARAM( FD_CLOSE, 0 ) );      
}

void Curses::JobReLogin::onDisconnected( )
{
   shutdown( m_socket, -1 );
   closesocket( m_socket );
   m_socket = INVALID_SOCKET;

   printStatus( "Primary game connection lost." );
   SetDlgItemText( m_dlg, IDOK, "Close" );
}

void Curses::JobReLogin::onLoggedIn( )
{

}

void Curses::JobReLogin::onNetIn( const string& line, int from, size_t num )
{

}

void Curses::JobReLogin::onNetOut( const string& line, int from, size_t num )
{

}

bool Curses::JobReLogin::onWndProc( WNDPROCPARAMS* params )
{
   if (params->msg == WM_USER && 
       Curses::singleton( )->m_main_socket == (SOCKET) params->wparam) // client's WSA message.
   {
      using namespace yiffurc;

      // If disconnected and 
      // we've received the MOTD on the secondary connection,
      // hide the event and proceed with login.
      if (WSAGETSELECTEVENT( params->lparam ) == FD_CLOSE &&
          m_session.level == NET_LEVEL_MOTDEND)
      {
         printStatus( "Primary connection closed." );
         printStatus( "Logging in..." );

         PLAYERINFO pi;
         f2_myplayerinfo( &pi );

         const char* cmd = c2s_enc_login( m_login.name.c_str( ), 
                                          m_login.password.c_str( ),
                                          pi.mac_id );
         send( m_socket, cmd, strlen( cmd ), 0 );
         yiffurc_release( cmd );

         m_close_hidden = true;
         params->retval = 0;
         return true;
      }
   }
   return false;
}

void Curses::JobReLogin::onSocketRead( )
{
   int recv_len;
   char ch;
   while (1 == (recv_len = recv( m_socket, &ch, 1, 0 )))
   {
      if (ch != '\n')
         m_linebuf += ch;
      else
      {
         using namespace yiffurc;
         const S2C* s2c = s2c_parse( m_linebuf.c_str( ), m_linebuf.length( ), &m_session );

         if (s2c)
         {
            switch (s2c->type)
            {
            case S2C_TYPE_DRAGONROAR:
               {
                  printStatus( "Received MOTD." );
                  printStatus( "Closing primary connection..." );
                  // Secretly send a quit for a graceful exit
                  // and hide the FD_CLOSE event.
                  send( Curses::singleton( )->m_main_socket, "quit\n", 5, 0 );
               }
               break;

             case S2C_TYPE_PASS:
               {
                  printStatus( "Logged in." );
                  printStatus( "Sending colors..." );

                  const string cc = "t" + m_login.color.str( );
                  const char* cmd = c2s_enc_setcolor( cc.c_str( ), cc.c_str( ) + ColorCodePal::COLORS_LEN + 1 );

                  send( m_socket, cmd, strlen( cmd ), 0 );
                  yiffurc_release( cmd );

                  printStatus( "Sending description..." );

                  cmd = c2s_enc_setdesc( m_login.description.c_str( ) );
                  send( m_socket, cmd, strlen( cmd ), 0 );
                  yiffurc_release( cmd );

                  printStatus( "Swapping sockets..." );

                  // Swap sockets.
                  SOCKET old_socket = Curses::singleton( )->m_main_socket;
                  f2__setmainsocket( m_socket );
                  m_socket = old_socket;  // It'll get cleaned up in the dtor.
                  m_close_hidden = false;

                  // Set new player info.
                  f2__setplayerinfo( m_login.name.c_str( ), m_login.password.c_str( ),
                                     ("t" + m_login.color.str( )).c_str( ), m_login.description.c_str( ), NULL );
                  SetWindowText( Curses::singleton( )->m_wnd, ("Furcadia - " + unpipen( m_login.name )).c_str( ) );

                  SetDlgItemText( m_dlg, IDOK, "Close" );
                  printStatus( "Done." );

                  yiffurc_release( s2c );

                  m_linebuf.clear( );

                  Curses::singleton( )->screenClose( ); // This'll kill this object.
                  return;
               }
               break;

             case S2C_TYPE_MSGBOX:
               // The server sends this when a login fails, then disconnects.
               printStatus( ((const S2C_MSGBOX*) s2c)->msg );
               break;
            } // switch (s2c->type)

            yiffurc_release( s2c );
         } // if (s2c)

         m_linebuf.clear( );
      }
   }
}

void Curses::JobReLogin::printStatus( const string& line )
{
   HWND ctrl = GetDlgItem( m_dlg, IDC_JOB_STATUS );

   Edit_SetSel( ctrl, -1, -1 );
   Edit_ReplaceSel( ctrl, (line + "\r\n").c_str( ) );
}

INT_PTR CALLBACK Curses::JobReLogin::dlg_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
#define _INST_PTR( ) ((JobReLogin*) GetWindowLongPtr( wnd, GWLP_USERDATA ))

   switch (msg)
   {
   case WM_INITDIALOG:
      {
         SetWindowLongPtr( wnd, GWLP_USERDATA, (LONG_PTR) lparam );

         SetWindowText( wnd, "Curses - Secondary Log In" );
         SetDlgItemText( wnd, IDOK, "Cancel" );

         // Center and display.
         RECT parent_rect, wnd_rect;
         GetWindowRect( GetParent( wnd ), &parent_rect );
         GetWindowRect( wnd, &wnd_rect );

         SetWindowPos( wnd, NULL, 
                       parent_rect.left + 
                        (parent_rect.right - parent_rect.left) / 2 - 
                        (wnd_rect.right - wnd_rect.left) / 2,
                       parent_rect.top + 
                        (parent_rect.bottom - parent_rect.top) / 2 -
                        (wnd_rect.bottom - wnd_rect.top ) / 2,
                       0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );
         return TRUE;
      }

   case WM_WSAMSG:
      {
         JobReLogin* inst = _INST_PTR( );

         // If we've already swapped the sockets, just forward the message to
         // the new owner.
         if ((SOCKET) wparam != inst->m_socket)
            return SendMessage( Curses::singleton( )->m_wnd, WM_USER, wparam, lparam );

         switch (WSAGETSELECTEVENT( lparam ))
         {
         case FD_CONNECT:
            switch (WSAGETSELECTERROR( lparam ))
            {
            case WSAECONNREFUSED:
               inst->printStatus( "Connection refused." );
               break;
            case WSAENETUNREACH:
            case WSAEHOSTUNREACH:
               inst->printStatus( "Host unreachable." );
               break;
            }

            if (WSAGETSELECTERROR( lparam ))
            {
               inst->printStatus( "Couldn't connect to remote host." );
               SetDlgItemText( wnd, IDOK, "Close" );
            }
            else
               inst->printStatus( "Connected to game server." );
            break;

         case FD_READ:
            if (WSAGETSELECTERROR( lparam ))
            {
               inst->printStatus( "Connection reset." );
               SetDlgItemText( wnd, IDOK, "Close" );
               shutdown( inst->m_socket, -1 );
            }
            else
               inst->onSocketRead( );
            break;
         } // switch (WSAGETSELECTEVENT( lparam ))
      } // case WM_WSAMSG:
      return TRUE;

   case WM_COMMAND:
      switch (LOWORD( wparam ))
      {
      case IDOK:
         if (HIWORD( wparam ) == BN_CLICKED)
         {
            DestroyWindow( wnd );
            return TRUE;
         }
      }
      break;

   case WM_DESTROY:
      _INST_PTR( )->m_dlg = NULL;
      Curses::singleton( )->terminateJob( );
      break;

   case WM_ACTIVATE:
      if (LOWORD( wparam ) != WA_INACTIVE)
         f2_setcurdialog( wnd );
   }

   return FALSE;
#undef _INST_PTR
}

//////////////////////////////////////////////////////////////////////////

Curses::JobRegister::JobRegister( const string& email )
   : m_login( *Curses::singleton( )->m_login_info ), m_email( email ),
     m_dlg_email( NULL ), m_dlg( NULL )
{
   memset( &m_session, 0, sizeof( m_session ) );
   m_session.level = yiffurc::NET_LEVEL_MOTDEND;

   // Prompt for email.
   m_dlg_email = CreateDialogParam( Curses::singleton( )->m_inst, MAKEINTRESOURCE( IDD_JOB_EMAIL ),
                                    Curses::singleton( )->m_wnd, email_proc, (LPARAM) this );
}

Curses::JobRegister::~JobRegister( )
{
   if (m_dlg_email)
      DestroyWindow( m_dlg_email );

   if (m_dlg)
      DestroyWindow( m_dlg );
}

void Curses::JobRegister::onDisconnected( )
{
   printStatus( "Primary game connection lost." );
   SetDlgItemText( m_dlg, IDOK, "Close" );
}

void Curses::JobRegister::onLoggedIn( )
{

}

void Curses::JobRegister::onNetIn( const string& line, int from, size_t num )
{
   using namespace yiffurc;
   const S2C* s2c = s2c_parse( line.c_str( ), line.length( ), &m_session );

   if (s2c)
   {
      switch (s2c->type)
      {
      case S2C_TYPE_CREATEFAIL:
         printStatus( "Failed to create character." );
         printStatus( string( "Server responded: " ) + ((S2C_CREATEFAIL*) s2c)->reason );
         SetDlgItemText( m_dlg, IDOK, "Close" );
         break;

      case S2C_TYPE_PASS:
         printStatus( "Successfully created character." );
         Curses::singleton( )->m_last_email = m_email; // Remember the last email.
         SetDlgItemText( m_dlg, IDOK, "Close" );
         break;
      }
      yiffurc_release( s2c );
   }
}

void Curses::JobRegister::onNetOut( const string& line, int from, size_t num )
{
   // Just update the session state.
   using namespace yiffurc;
   const C2S* c2s = c2s_parse( line.c_str( ), line.length( ), &m_session );
   if (c2s)
   {
      switch (c2s->type)
      {
      case C2S_TYPE_LOGIN:
         // Block the login attempts.
         f2_cancelline( );
      }
      yiffurc_release( c2s );
   }
}

bool Curses::JobRegister::onWndProc( WNDPROCPARAMS* params )
{
   return false;
}

void Curses::JobRegister::printStatus( const string& line )
{
   if (m_dlg)
   {
      HWND ctrl = GetDlgItem( m_dlg, IDC_JOB_STATUS );

      Edit_SetSel( ctrl, -1, -1 );
      Edit_ReplaceSel( ctrl, (line + "\r\n").c_str( ) );
   }
}

void Curses::JobRegister::create( )
{
   assert( !m_email.empty( ) );
   assert( !m_dlg );

   m_dlg = CreateDialogParam( Curses::singleton( )->m_inst, MAKEINTRESOURCE( IDD_JOB_DLG ),
                              Curses::singleton( )->m_wnd, dlg_proc, (LPARAM) this );

   printStatus( "Registering character..." );

   const char* cmd = yiffurc::c2s_enc_create( m_login.name.c_str( ), m_login.password.c_str( ),
                                              m_email.c_str( ) );

   f2_netoutline( cmd, strlen( cmd ) );
}

INT_PTR CALLBACK Curses::JobRegister::email_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
#define _INST_PTR( ) ((JobRegister*) GetWindowLongPtr( wnd, GWLP_USERDATA ))

   switch (msg)
   {
   case WM_INITDIALOG:
      {
         SetWindowLongPtr( wnd, GWLP_USERDATA, (LONG_PTR) lparam );

         SetWindowText( wnd, "Curses - Secondary Log In" );
         SetDlgItemText( wnd, IDC_EMAIL, ((JobRegister*) lparam)->m_email.c_str( ) );

         // Center and display.
         RECT parent_rect, wnd_rect;
         GetWindowRect( GetParent( wnd ), &parent_rect );
         GetWindowRect( wnd, &wnd_rect );

         SetWindowPos( wnd, NULL, 
                       parent_rect.left + 
                        (parent_rect.right - parent_rect.left) / 2 - 
                        (wnd_rect.right - wnd_rect.left) / 2,
                       parent_rect.top + 
                        (parent_rect.bottom - parent_rect.top) / 2 -
                        (wnd_rect.bottom - wnd_rect.top ) / 2,
                       0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );

         SetFocus( GetDlgItem( wnd, IDC_EMAIL ) );
         return FALSE;
      }

   case WM_COMMAND:
      switch (LOWORD( wparam ))
      {
      case IDOK:
         if (HIWORD( wparam ) == BN_CLICKED)
         {
            JobRegister* inst = _INST_PTR( );

            {
               char buf[ 256 ];
               GetDlgItemText( wnd, IDC_EMAIL, buf, 256 );
               inst->m_email = buf;
            }

            if (!inst->m_email.empty( ))
            {
               DestroyWindow( wnd );
               inst->create( );
            }
            return TRUE;
         }

      case IDCANCEL:
         if (HIWORD( wparam ) == BN_CLICKED)
         {
            Curses::singleton( )->terminateJob( );
            return TRUE;
         }
      }
      break;

   case WM_SYSCOMMAND:
      if (wparam == SC_CLOSE)
         Curses::singleton( )->terminateJob( );
      break;

   case WM_DESTROY:
      _INST_PTR( )->m_dlg = NULL;
      break;
      
   case WM_ACTIVATE:
      if (LOWORD( wparam ) != WA_INACTIVE)
         f2_setcurdialog( wnd );
      break;
   }

   return FALSE;
#undef  _INST_PTR
}

INT_PTR CALLBACK Curses::JobRegister::dlg_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
#define _INST_PTR( ) ((JobRegister*) GetWindowLongPtr( wnd, GWLP_USERDATA ))

   switch (msg)
   {
   case WM_INITDIALOG:
      {
         SetWindowLongPtr( wnd, GWLP_USERDATA, (LONG_PTR) lparam );

         SetWindowText( wnd, "Curses - Registering Character" );
         SetDlgItemText( wnd, IDOK, "Cancel" );

         // Center and display.
         RECT parent_rect, wnd_rect;
         GetWindowRect( GetParent( wnd ), &parent_rect );
         GetWindowRect( wnd, &wnd_rect );

         SetWindowPos( wnd, NULL, 
                       parent_rect.left + 
                        (parent_rect.right - parent_rect.left) / 2 - 
                        (wnd_rect.right - wnd_rect.left) / 2,
                       parent_rect.top + 
                        (parent_rect.bottom - parent_rect.top) / 2 -
                        (wnd_rect.bottom - wnd_rect.top ) / 2,
                       0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW );
         return TRUE;
      }

   case WM_COMMAND:
      switch (LOWORD( wparam ))
      {
      case IDOK:
         if (HIWORD( wparam ) == BN_CLICKED)
         {
            DestroyWindow( wnd );
            return TRUE;
         }
      }
      break;

   case WM_DESTROY:
      _INST_PTR( )->m_dlg = NULL;
      Curses::singleton( )->terminateJob( );
      break;

   case WM_ACTIVATE:
      if (LOWORD( wparam ) != WA_INACTIVE)
         f2_setcurdialog( wnd );
   }

   return FALSE;
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
         if (!HIWORD( hit ))  // Inside the client area.
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

HANDLE CALLBACK Curses::CreateFileA( LPCSTR lpFileName, DWORD dwDesiredAccess, 
                                     DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
                                     DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, 
                                     HANDLE hTemplateFile )
{
   assert( s_old_createfilea );

   if (Curses::singleton( ))
   {
      const string filename( lpFileName );

      if (filename.rfind( ".ini" ) == filename.length( ) - 4)
      {
         // The client will open the file once to read it before logging in and again after logging in to write to it.
         CharacterIni char_ini( filename );
         // Is it a character INI file?
         if (char_ini.read( ) || filename == "furcadia.ini" || !path_cmp( Curses::singleton( )->m_default_ini, filename )) 
         {
            if (!Curses::singleton( )->m_logged_in && Curses::singleton( )->m_options & OPT_AUTOSTART) // Reading.
            {
               // Always ensure a name and password exist.
               if (!char_ini.getName( ).length( ) || !char_ini.getPassword( ).length( ))
               {
                  // Return a temp version.
                  char sz_temp_path[ MAX_PATH ];
                  GetTempPath( MAX_PATH, sz_temp_path );
                  ostringstream ss;
                  ss << sz_temp_path << "curses-furcadia-ini-" << GetCurrentProcessId( ) << "-" << GetCurrentThreadId( ) << ".ini";            
                  char_ini.clear( );
                  char_ini.setFile( ss.str( ) );
                  char_ini.setName( "NULL" );
                  char_ini.setPassword( "NULL" );
                  char_ini.write( );
                  
                  return ::CreateFile( char_ini.getFile( ).c_str( ), GENERIC_READ, dwShareMode, lpSecurityAttributes,
                                       OPEN_EXISTING, dwFlagsAndAttributes, hTemplateFile );
               }
            }
            // Don't allow writing or creating new files.
            return ::CreateFile( lpFileName, GENERIC_READ, dwShareMode, lpSecurityAttributes,
                                 OPEN_ALWAYS, dwFlagsAndAttributes, hTemplateFile );
         }
      }
   }

   return ((HANDLE (CALLBACK *)(LPCSTR,DWORD,DWORD,
            LPSECURITY_ATTRIBUTES,DWORD,
            DWORD,HANDLE)) s_old_createfilea)( lpFileName, dwDesiredAccess,
                                               dwShareMode, lpSecurityAttributes,
                                               dwCreationDisposition, dwFlagsAndAttributes,
                                               hTemplateFile );
}

BOOL CALLBACK Curses::DeleteFileA( LPCSTR lpFileName )
{
   assert( s_old_deletefilea );

   if (Curses::singleton( ))
   {
      const string filename( lpFileName );

      // No deleting character files.
      if (filename.rfind( ".ini" ) == filename.length( ) - 4 )
      {
         CharacterIni char_ini( filename );
         // Is it a character INI file?
         if (char_ini.read( ) || filename == "furcadia.ini" || !path_cmp( Curses::singleton( )->m_default_ini, filename )) 
         {
            return TRUE;
         }
      }
   }
   return ((BOOL (CALLBACK *)(LPCSTR)) s_old_deletefilea)( lpFileName );
}

string get_default_ini( const string& settings_file )
{  
   string def_ini;

   // Check the command line first.
   {
      vector<string> cmds;
      line_split( GetCommandLine( ), &cmds );
      assert( !cmds.empty( ) );

      vector<string>::const_iterator i = cmds.begin( );
      ++i;
      for (; i != cmds.end( ); ++i)
      {
         assert( !i->empty( ) );
         if (*i == "-followurl" || *i == "-followfile" || *i == "-defaultpath")
         {
            if (++i == cmds.end( ))
               break;
         }
         else if (i->at( 0 ) != '-')
         {
            // Call it the default INI file.
            def_ini = *i;
            break;
         }
      }
   }
   
   if (def_ini.empty( ))    // Try the settings file.
   {
      IniFile furc_settings( settings_file );
      furc_settings.open( );
      def_ini = furc_settings.read( "DefaultINI", "general" );
   }

   if (def_ini.empty( ))   // Use the default.
      def_ini = "furcadia.ini";

   // Make absolute.
   if (def_ini.find( ":\\" ) == def_ini.npos &&
       def_ini.compare( 0, 2, "\\" ))
   {
      // Check in the per-user characters folder first.
      string fname = def_ini;
      def_ini = string( f2_cldocroot( ) ) + "\\Furcadia Characters\\" + fname;
      if (GetFileAttributes( def_ini.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
      {
         // Doesn't exist there, so say it's in the furc directory.
         def_ini = string( f2_clroot( ) ) + "\\" + fname;
      }
   }

   return def_ini;
}

} // namespace curses