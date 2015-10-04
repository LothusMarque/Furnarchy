#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WindowsX.h>
#include <CommCtrl.h>
#include <CommDlg.h>
#include <string>
#include <list>
#include <fstream>
#include <boost/regex.hpp>
#include <ShlObj.h>
#include <ShellAPI.h>
#include "resource.h"
#include "IniFile.h"
#include "tinyxml/tinyxml.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////

enum Dlg_Tab { DLGTAB_GEN = 0, DLGTAB_MOD, DLGTAB_ADV };

const DWORD g_dlg_gen_ids[] = { IDC_GEN_PATH, IDC_GEN_PATHBROWSE, IDC_GEN_PREFIX,
                                IDC_GEN_ALLOWHTML, IDC_GEN_CHECKNEWS, IDC_GEN_SYSTRAY, 
                                IDC_GEN_TIMESTAMP, IDC_GEN_STATIC1, IDC_GEN_STATIC2, 
                                IDC_GEN_STATIC3, IDC_GEN_STATIC4, IDC_GEN_STATIC5,
                                IDC_GEN_SKIN, IDC_GEN_TIMESTAMPSECS };
const DWORD g_dlg_mod_ids[] = { IDC_MOD_STATIC1, IDC_MOD_STATIC2, IDC_MOD_LIST };
const DWORD g_dlg_adv_ids[] = { IDC_ADV_STATIC1, IDC_ADV_SINGLEUSER, IDC_ADV_INITCMDS,
                                IDC_ADV_LOGINCMDS };

//////////////////////////////////////////////////////////////////////////

HINSTANCE g_hinst;
HWND      g_dlg;
bool      g_modlist_dragging;

//////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK dlg_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
void dlg_create( );
void dlg_destroy( );
void dlg_show_tab( Dlg_Tab tab );
bool dlg_fill_fields( );
bool dlg_save_fields( );
string get_user_root( );
string get_cwd( );
template <typename IteratorA, typename IteratorB>
IteratorA find_istr( IteratorA start, const IteratorB end, const string& str );

//////////////////////////////////////////////////////////////////////////

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                      LPSTR lpCmdLine, int nShowCmd )
{
   InitCommonControls( );
   g_hinst = hInstance;

   dlg_create( );

   MSG msg;
   while (GetMessage( &msg, NULL, 0, 0 ))
   {
      if (!IsDialogMessage( g_dlg, &msg ))
      {
         TranslateMessage( &msg );
         DispatchMessage( &msg );
      }
   }

   return 0;
}

void dlg_create( )
{
   dlg_destroy( );

   g_dlg = CreateDialog( g_hinst, MAKEINTRESOURCE( IDD_DIALOG1 ), NULL, dlg_proc );
   ShowWindow( g_dlg, SW_SHOWNORMAL );
}

void dlg_destroy( )
{
   if (g_dlg)
   {
      if (IsWindow( g_dlg ))
         DestroyWindow( g_dlg );
      g_dlg = NULL;
   }
}

void dlg_show_tab( Dlg_Tab tab )
{
   if (g_dlg && IsWindow( g_dlg ))
   {
      HWND ctrl = GetDlgItem( g_dlg, IDC_TABS );
      TabCtrl_SetCurSel( ctrl, tab );

      // General Tab.
      int showcmd = tab == DLGTAB_GEN ? SW_SHOW : SW_HIDE;
      for (unsigned i = 0; i < sizeof( g_dlg_gen_ids ) / sizeof( *g_dlg_gen_ids ); ++i)
         ShowWindow( GetDlgItem( g_dlg, g_dlg_gen_ids[ i ] ), showcmd );

      // Modules Tab.
      showcmd = tab == DLGTAB_MOD ? SW_SHOW : SW_HIDE;
      for (unsigned i = 0; i < sizeof( g_dlg_mod_ids ) / sizeof( *g_dlg_mod_ids ); ++i)
         ShowWindow( GetDlgItem( g_dlg, g_dlg_mod_ids[ i ] ), showcmd );

      // Advanced Tab.
      showcmd = tab == DLGTAB_ADV ? SW_SHOW : SW_HIDE;
      for (unsigned i = 0; i < sizeof( g_dlg_adv_ids ) / sizeof( *g_dlg_adv_ids ); ++i)
         ShowWindow( GetDlgItem( g_dlg, g_dlg_adv_ids[ i ] ), showcmd );         
   }
}

bool dlg_fill_fields( )
{
   if (!g_dlg || !IsWindow( g_dlg ))
      return false;

   string       exe_path = "";
   bool         single_user = false;
   string       cmd_prefix = "@";
   list<string> mods_accepted;
   list<string> mods_masked;
   list<string> mods;
   list<string> skins;
   string       skin = "none";
   bool         allow_raw_html = true;
   bool         check_news = false;
   bool         traymode = false;
   bool         timestamps = false;
   bool         timestampsecs = false;

   /* Load the local options file. */
   {
      TiXmlDocument doc( "options.xml" );
      if (doc.LoadFile( ) && !doc.Error( ))
      {
         const TiXmlElement* xml_root = doc.RootElement( );
         if (xml_root && xml_root->ValueStr( ) == "furnarchyOpts")
         {
            const TiXmlElement* xml_elem = xml_root->FirstChildElement( "singleUserMode" );
            if (xml_elem && xml_elem->GetText( ) && xml_elem->GetText( ) == string( "true" ))
               single_user = true;
            
            xml_elem = xml_root->FirstChildElement( "commandPrefix" );
            if (xml_elem && xml_elem->GetText( ))
               cmd_prefix = xml_elem->GetText( );

            xml_elem = xml_root->FirstChildElement( "modules" );
            if (xml_elem)
            {
               for (xml_elem = xml_elem->FirstChildElement( ); xml_elem; xml_elem = xml_elem->NextSiblingElement( ))
               {
                  if (xml_elem->ValueStr( ) == "accept" && xml_elem->GetText( ))
                     mods_accepted.push_back( xml_elem->GetText( ) );
                  else if (xml_elem->ValueStr( ) == "mask" && xml_elem->GetText( ))
                     mods_masked.push_back( xml_elem->GetText( ) );
               }
            }
         }
      }
   }

   /* Load the per-user options file. */
   if (!single_user)
   {
      TiXmlDocument doc( get_user_root( ) + "\\options.xml" );
      if (doc.LoadFile( ) && !doc.Error( ))
      {
         // Overwrite masked and accepted mods.
         mods_masked.clear( );
         mods_accepted.clear( );

         const TiXmlElement* xml_root = doc.RootElement( );
         if (xml_root && xml_root->ValueStr( ) == "furnarchyOpts")
         {
            // Don't believe the singleUserMode option in this file.

            const TiXmlElement* xml_elem = xml_root->FirstChildElement( "commandPrefix" );
            if (xml_elem && xml_elem->GetText( ))
               cmd_prefix = xml_elem->GetText( );

            xml_elem = xml_root->FirstChildElement( "modules" );
            if (xml_elem)
            {
               for (xml_elem = xml_elem->FirstChildElement( ); xml_elem; xml_elem = xml_elem->NextSiblingElement( ))
               {
                  if (xml_elem->ValueStr( ) == "accept" && xml_elem->GetText( ))
                     mods_accepted.push_back( xml_elem->GetText( ) );
                  else if (xml_elem->ValueStr( ) == "mask" && xml_elem->GetText( ))
                     mods_masked.push_back( xml_elem->GetText( ) );
               }
            }
         }
      }
   }

   /* Load the launcher settings. */
   {
      string path = "launch.ini";
      if (!single_user)
         path = get_user_root( ) + "\\launch.ini";

      IniFile ini( path );
      if (ini.open( ))
         exe_path = ini.read( "ClientExe" );
   }

   /* Load the init source. */
   {
      boost::regex re_rawchat( "^\\s*rawchat\\s+(on|off)\\s*$" );
      boost::regex re_skin( "^\\s*skuse\\s+(.+)\\s*$" );
      // boost::regex re_news( "^\\s*news(\\s+.*)?$" );
      boost::regex re_traymode( "^\\s*traymode\\s+(on|off)\\s*$" );
      boost::regex re_timestamp( "^\\s*timestamp\\s+(on|off)\\s*$" );
	  boost::regex re_timestampsecs( "^\\s*timestampsecs\\s+(on|off)\\s*$" );
      boost::smatch mr;

      string filename = single_user ? "source\\init.txt" : get_user_root( ) + "\\source\\init.txt";

      if (INVALID_FILE_ATTRIBUTES == GetFileAttributes( filename.c_str( ) ))
         filename = "source\\init.txt";

      ifstream f( filename.c_str( ) );
      if (!f.fail( ))
      {
         string line;

         do 
         {
            line.clear( );
            getline( f, line );

            if (line.length( ))
            {
               if (regex_match( line, mr, re_rawchat ))
                  allow_raw_html = (mr.str( 1 ) == "on") ? true : false;
               else if (regex_match( line, mr, re_skin ))
                  skin = mr.str( 1 );
               else if (regex_match( line, mr, re_traymode ))
                  traymode = (mr.str( 1 ) == "on") ? true : false;
               else if (regex_match( line, mr, re_timestamp ))
                  timestamps = (mr.str( 1 ) == "on") ? true : false;
			   else if (regex_match( line, mr, re_timestampsecs ))
                  timestampsecs = (mr.str( 1 ) == "on") ? true : false;
            }
         } while (f.good( ));
      }
   }

   /* Load the login source. */
   {
      boost::regex re_rawchat( "^\\s*rawchat\\s+(on|off)\\s*$" );
      boost::regex re_skin( "^\\s*skuse\\s+(.+)\\s*$" );
      boost::regex re_news( "^\\s*news(\\s+.*)?$" );
      boost::regex re_traymode( "^\\s*traymode\\s+(on|off)\\s*$" );
      boost::regex re_timestamp( "^\\s*timestamp\\s+(on|off)\\s*$" );
	  boost::regex re_timestampsecs( "^\\s*timestampsecs\\s+(on|off)\\s*$" );
      boost::smatch mr;

      string filename = single_user ? "source\\login.txt" : get_user_root( ) + "\\source\\login.txt";

      if (INVALID_FILE_ATTRIBUTES == GetFileAttributes( filename.c_str( ) ))
         filename = "source\\login.txt";

      ifstream f( filename.c_str( ) );
      if (!f.fail( ))
      {
         string line;

         do 
         {
            line.clear( );
            getline( f, line );

            if (line.length( ))
            {
               if (regex_match( line, mr, re_rawchat ))
                  allow_raw_html = (mr.str( 1 ) == "on") ? true : false;
               else if (regex_match( line, mr, re_skin ))
                  skin = mr.str( 1 );
               else if (regex_match( line, mr, re_news ))
                  check_news = true;
               else if (regex_match( line, mr, re_traymode ))
                  traymode = (mr.str( 1 ) == "on") ? true : false;
               else if (regex_match( line, mr, re_timestamp ))
                  timestamps = (mr.str( 1 ) == "on") ? true : false;
			   else if (regex_match( line, mr, re_timestampsecs ))
                  timestampsecs = (mr.str( 1 ) == "on") ? true : false;
            }
         } while (f.good( ));
      }
   }

   /* Look for modules. */
   {
      WIN32_FIND_DATA fd;
      HANDLE f = FindFirstFile( "modules\\*", &fd );

      if (f != INVALID_HANDLE_VALUE)
      {
         do 
         {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && fd.cFileName[ 0 ] != '.')
            {
               mods.push_back( fd.cFileName );
            }

         } while (FindNextFile( f, &fd ));
         FindClose( f );
      }
   }

   /* Merge and validate accepted modules. */
   {
      const list<string> t( mods_accepted );
      mods_accepted.clear( );

      for (list<string>::const_iterator i = t.begin( ); i != t.end( ); ++i)
      {
         // Must be in the modules list and not already accepted.
         if (find_istr( mods.begin( ), mods.end( ), *i ) != mods.end( ) &&
             find_istr( mods_accepted.begin( ), mods_accepted.end( ), *i ) == mods_accepted.end( ))
            mods_accepted.push_back( *i );
      }
      for (list<string>::const_iterator i = mods.begin( ); i != mods.end( ); ++i)
      {
         // Must not be already accepted.
         if (find_istr( mods_accepted.begin( ), mods_accepted.end( ), *i ) == mods_accepted.end( ))
            mods_accepted.push_back( *i );
      }
   }

   /* Validate masked modules. */
   {
      list<string>::iterator i = mods_masked.begin( );
      while (i != mods_masked.end( ))
      {
         if (find_istr( mods.begin( ), mods.end( ), *i ) == mods.end( ))
            mods_masked.erase( i++ );
         else
            ++i;
      }
   }

   /* Look for skins. */
   if (!single_user)
   {
      WIN32_FIND_DATA fd;

      HANDLE f = FindFirstFile( (get_user_root( ) + "\\skins\\*").c_str( ), &fd );

      if (f != INVALID_HANDLE_VALUE)
      {
         do 
         {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && fd.cFileName[ 0 ] != '.')
            {
               skins.push_back( fd.cFileName );
            }

         } while (FindNextFile( f, &fd ));
         FindClose( f );
      }
   }
   /* Look for more skins. */
   {
      WIN32_FIND_DATA fd;

      HANDLE f = FindFirstFile( "skins\\*", &fd );

      if (f != INVALID_HANDLE_VALUE)
      {
         do 
         {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && fd.cFileName[ 0 ] != '.')
            {
               // Avoid duplicates.
               if (find_istr( skins.begin( ), skins.end( ), fd.cFileName ) == skins.end( ))
                  skins.push_back( fd.cFileName );
            }

         } while (FindNextFile( f, &fd ));
         FindClose( f );
      }
   }

   /* Validate the current skin. */
   {
      if (find_istr( skins.begin( ), skins.end( ), skin ) == skins.end( ))
         skin = "none";
   }

   /* Finally, fill in the dialog fields. */
   SetDlgItemText( g_dlg, IDC_GEN_PATH, exe_path.c_str( ) );

   {
      HWND ctrl = GetDlgItem( g_dlg, IDC_GEN_SKIN );
      ComboBox_ResetContent( ctrl );
      ComboBox_AddString( ctrl, "none" );
      ComboBox_SetCurSel( ctrl, 0 );
      for (list<string>::const_iterator i = skins.begin( ); i != skins.end( ); ++i)
      {
         const int n = ComboBox_AddString( ctrl, i->c_str( ) );
         if (_stricmp( i->c_str( ), skin.c_str( ) ) == 0)
            ComboBox_SetCurSel( ctrl, n );
      }      
   }

   SetDlgItemText( g_dlg, IDC_GEN_PREFIX, cmd_prefix.c_str( ) );
   CheckDlgButton( g_dlg, IDC_GEN_ALLOWHTML, allow_raw_html ? BST_CHECKED : BST_UNCHECKED );
   CheckDlgButton( g_dlg, IDC_GEN_CHECKNEWS, check_news ? BST_CHECKED : BST_UNCHECKED );
   CheckDlgButton( g_dlg, IDC_GEN_SYSTRAY, traymode ? BST_CHECKED : BST_UNCHECKED );
   CheckDlgButton( g_dlg, IDC_GEN_TIMESTAMP, timestamps ? BST_CHECKED : BST_UNCHECKED );
   CheckDlgButton( g_dlg, IDC_GEN_TIMESTAMPSECS, timestampsecs ? BST_CHECKED : BST_UNCHECKED );
   {
      HWND ctrl = GetDlgItem( g_dlg, IDC_MOD_LIST );
      
      LVITEM lv;
      memset( &lv, 0, sizeof( lv ) );
      lv.mask      = LVIF_TEXT | LVIF_STATE | LVIF_PARAM;
      lv.iSubItem  = 0;
      lv.iItem     = 0;
      lv.lParam    = 0;

      // Do accepted modules.
      for (list<string>::const_iterator i = mods_accepted.begin( ); 
           i != mods_accepted.end( ); ++i)
      {
         lv.pszText = const_cast<char*>( i->c_str( ) );
         const int n = ListView_InsertItem( ctrl, &lv );
         ++lv.iItem;
         ++lv.lParam;

         if (find_istr( mods_masked.begin( ), mods_masked.end( ), *i ) == mods_masked.end( ) )
         {
            ListView_SetCheckState( ctrl, n, TRUE );
         }
         else
         {
            ListView_SetCheckState( ctrl, n, FALSE );
         }
      }
   }

   CheckDlgButton( g_dlg, IDC_ADV_SINGLEUSER, single_user ? BST_CHECKED : BST_UNCHECKED );

   return true;
}

bool dlg_save_fields( )
{
   if (!g_dlg || !IsWindow( g_dlg ))
      return false;

   string       exe_path = "";
   bool         single_user = false;
   string       cmd_prefix = "@";
   list<string> mods_accepted;
   list<string> mods_masked;
   string       skin = "none";
   bool         allow_raw_html = true;
   bool         check_news = false;
   bool         traymode = false;
   bool         timestamps = false;
   bool         timestampsecs = false;
   /* Read the fields. */

   {
      char sz[ MAX_PATH ];
      GetDlgItemText( g_dlg, IDC_GEN_PATH, sz, MAX_PATH );
      sz[ MAX_PATH - 1 ] = '\0';
      exe_path = sz;
   }

   {
      HWND ctrl = GetDlgItem( g_dlg, IDC_GEN_SKIN );
      char sz[ MAX_PATH ];
      ComboBox_GetLBText( ctrl, ComboBox_GetCurSel( ctrl ), sz );
      sz[ MAX_PATH - 1 ] = '\0';
      skin = sz;
   }

   {
      char sz[ MAX_PATH ];
      GetDlgItemText( g_dlg, IDC_GEN_PREFIX, sz, MAX_PATH );
      sz[ MAX_PATH - 1 ] = '\0';
      cmd_prefix = sz;
   }

   allow_raw_html = IsDlgButtonChecked( g_dlg, IDC_GEN_ALLOWHTML ) == BST_CHECKED;
   check_news     = IsDlgButtonChecked( g_dlg, IDC_GEN_CHECKNEWS ) == BST_CHECKED;
   traymode       = IsDlgButtonChecked( g_dlg, IDC_GEN_SYSTRAY ) == BST_CHECKED;
   timestamps     = IsDlgButtonChecked( g_dlg, IDC_GEN_TIMESTAMP ) == BST_CHECKED;
   timestampsecs  = IsDlgButtonChecked( g_dlg, IDC_GEN_TIMESTAMPSECS ) == BST_CHECKED;
   single_user    = IsDlgButtonChecked( g_dlg, IDC_ADV_SINGLEUSER ) == BST_CHECKED;

   {
      HWND ctrl = GetDlgItem( g_dlg, IDC_MOD_LIST );
      const int n = ListView_GetItemCount( ctrl );

      char sz_name[ MAX_PATH ];

      for (int i = 0; i < n; ++i)
      {
         ListView_GetItemText( ctrl, i, 0, sz_name, MAX_PATH );
         mods_accepted.push_back( sz_name );
         if (!ListView_GetCheckState( ctrl, i ))
            mods_masked.push_back( sz_name );
      }
   }

   /* If in multi-user mode, only update in place the singleUserMode option in the
   *  local options file. */
   if (!single_user)
   {
      TiXmlDocument xml_doc( "options.xml" );
      if (xml_doc.LoadFile( ) && !xml_doc.Error( ) && 
          xml_doc.RootElement( ) && xml_doc.RootElement( )->ValueStr( ) == "furnarchyOpts")
      {
         // Options.xml is OK.  Ready to update in place.

         TiXmlElement* e = xml_doc.RootElement( )->FirstChildElement( "singleUserMode" );
         if (e && e->GetText( ) && string( e->GetText( ) ) == "true")
         {
            // Need to update only the singleUserMode option.
            ofstream fs( "options.xml" );
            fs << "<furnarchyOpts>\n"
               << "\t<singleUserMode>false</singleUserMode>\n";
            e = xml_doc.RootElement( );
            if (e->FirstChildElement( "commandPrefix" ))
               fs << '\t' << *e->FirstChildElement( "commandPrefix" ) << '\n';
            if (e->FirstChildElement( "modules" ))
               fs << '\t' << *e->FirstChildElement( "modules" ) << '\n';
            fs << "</furnarchyOpts>\n";
         }
      }
      // else it's OK to leave the file bad or missing, since singleUserMode = false is the
      // default state.
   }

   /* Write the options.xml file. */
   {
      string path = "options.xml";
      if (!single_user)
         path = get_user_root( ) + "\\options.xml";

      ofstream fs( path.c_str( ) );
      fs << "<furnarchyOpts>\n"
         << "\t<singleUserMode>" << (single_user?"true":"false") << "</singleUserMode>\n"
         << "\t<commandPrefix>" << cmd_prefix << "</commandPrefix>\n"
         << "\t<modules>\n";

      for (list<string>::const_iterator i = mods_accepted.begin( ); i != mods_accepted.end( ); ++i)
         fs << "\t\t<accept>" << *i << "</accept>\n";
      for (list<string>::const_iterator i = mods_masked.begin( ); i != mods_masked.end( ); ++i)
         fs << "\t\t<mask>" << *i << "</mask>\n";

      fs << "\t</modules>\n";
      fs << "</furnarchyOpts>\n";
   }

   /* Write the init.txt file. */
   {
      string path = "source\\init.txt";
      if (!single_user)
         path = get_user_root( ) + "\\source\\init.txt";

      // Read in the whole file and remove any old commands.
      list<string> cmds;
      {
         ifstream fs(  path.c_str( ) );
         string line;
         while (fs.good( ))
         {
            line.clear( );
            getline( fs, line );
            if (line.length( ))
               cmds.push_back( line );
         }
      }

      // Remove old commands.
      {
         boost::regex re_rawchat( "^\\s*rawchat(\\s+.*)?$" );
         boost::regex re_skin( "^\\s*skuse(\\s+.*)?$" );
         boost::regex re_news( "^\\s*news(\\s+.*)?$" );
         boost::regex re_traymode( "^\\s*traymode(\\s+.*)?$" );
         boost::regex re_timestamp( "^\\s*timestamp(\\s+.*)?$" );
		 boost::regex re_timestampsecs( "^\\s*timestampsecs\\s+(on|off)\\s*$" );

         for (list<string>::iterator i = cmds.begin( ); i != cmds.end( );)
         {
            if (boost::regex_match( *i, re_rawchat ) ||
                boost::regex_match( *i, re_skin ) ||
                boost::regex_match( *i, re_news ) ||
                boost::regex_match( *i, re_traymode ) ||
                boost::regex_match( *i, re_timestamp ) ||
				boost::regex_match( *i, re_timestampsecs ))
               cmds.erase( i++ );
            else
               ++i;
         }
      }

      // Rewrite it with new commands added.
      {
         ofstream fs( path.c_str( ) );

         for (list<string>::const_iterator i = cmds.begin( ); i != cmds.end( ); ++i)
            fs << *i << '\n';

         fs << "skuse " << skin << '\n';
         fs << "rawchat " << (allow_raw_html?"on":"off") << '\n';
         fs << "traymode " << (traymode?"on":"off") << '\n';
         fs << "timestamp " << (timestamps?"on":"off") << '\n';
		 fs << "timestampsecs " << (timestampsecs?"on":"off") << '\n';
      }
   }

   /* Write the login.txt file. */
   {
      string path = "source\\login.txt";
      if (!single_user)
         path = get_user_root( ) + "\\source\\login.txt";

      // Read in the whole file and remove any old commands.
      list<string> cmds;
      {
         ifstream fs(  path.c_str( ) );
         string line;
         while (fs.good( ))
         {
            line.clear( );
            getline( fs, line );
            if (line.length( ))
               cmds.push_back( line );
         }
      }

      // Remove old commands.
      {
         boost::regex re_rawchat( "^\\s*rawchat(\\s+.*)?$" );
         boost::regex re_skin( "^\\s*skuse(\\s+.*)?$" );
         boost::regex re_news( "^\\s*news(\\s+.*)?$" );
         boost::regex re_traymode( "^\\s*traymode(\\s+.*)?$" );
         boost::regex re_timestamp( "^\\s*timestamp(\\s+.*)?$" );
		 boost::regex re_timestampsecs( "^\\s*timestampsecs\\s+(on|off)\\s*$" );

         for (list<string>::iterator i = cmds.begin( ); i != cmds.end( );)
         {
            if (boost::regex_match( *i, re_rawchat ) ||
                boost::regex_match( *i, re_skin ) ||
                boost::regex_match( *i, re_news ) ||
                boost::regex_match( *i, re_traymode ) ||
                boost::regex_match( *i, re_timestamp ) ||
				boost::regex_match( *i, re_timestampsecs ))
               cmds.erase( i++ );
            else
               ++i;
         }
      }

      // Rewrite it with new commands added.
      {
         ofstream fs( path.c_str( ) );

         for (list<string>::const_iterator i = cmds.begin( ); i != cmds.end( ); ++i)
            fs << *i << '\n';

         if (check_news)
            fs << "news\n";
      }
   }

   /* Write the launch.ini file. */
   {
      string path = "launch.ini";
      if (!single_user)
         path = get_user_root( ) + "\\launch.ini";

      ofstream fs( path.c_str( ) );
      
      fs << "# Set this value if the launcher cannot find your\n"
         << "# Furcadia.exe file.\n"
         << "ClientExe=";
      if (exe_path.length( ))
         fs << exe_path;
      fs << '\n';
   }

   return true;
}

string get_user_root( )
{
   char sz_path[ MAX_PATH ];
   SHGetFolderPath( NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, 
                    NULL, SHGFP_TYPE_CURRENT, sz_path );
   return string( sz_path ) + "\\Furnarchy2";
}

string get_cwd( )
{
   char sz_path[ MAX_PATH ];
   GetCurrentDirectory( MAX_PATH, sz_path );
   return string( sz_path );
}

template <typename IteratorA, typename IteratorB>
IteratorA find_istr( IteratorA start, const IteratorB end, const string& str )
{
   for (; start != end; ++start)
   {
      if (_stricmp( start->c_str( ), str.c_str( ) ) == 0)
         return start;
   }
   return start;
}

INT_PTR CALLBACK dlg_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
   switch (msg)
   {
   case WM_INITDIALOG:
      {
         g_dlg = wnd;

         // Set up the tab control.
         {
            HWND tabctrl = GetDlgItem( wnd, IDC_TABS );

            TCITEM tci;
            memset( &tci, 0, sizeof( tci ) );
            tci.mask = TCIF_TEXT;
            tci.iImage = -1;

            tci.pszText = "General";
            TabCtrl_InsertItem( tabctrl, 0, &tci );

            tci.pszText = "Modules";
            TabCtrl_InsertItem( tabctrl, 1, &tci );

            tci.pszText = "Advanced";
            TabCtrl_InsertItem( tabctrl, 2, &tci );

            dlg_show_tab( DLGTAB_GEN );
         }

         // Set up the Modules listview control.
         {
            HWND lvctrl = GetDlgItem( wnd, IDC_MOD_LIST );

            ListView_SetExtendedListViewStyleEx( lvctrl, LVS_EX_CHECKBOXES, LVS_EX_CHECKBOXES );

            LVCOLUMN col;
            memset( &col, 0, sizeof( col ) );
            col.mask     = LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
            col.iSubItem = 0;
            col.pszText  = "Name";
            col.cx       = 224;

            ListView_InsertColumn( lvctrl, 0, &col );

            SetWindowPos( lvctrl, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE ); 
         }

         dlg_fill_fields( );
      }
      return FALSE;

   case WM_CLOSE:
      DestroyWindow( wnd );
      return TRUE;

   case WM_DESTROY:
      g_dlg = NULL;
      PostQuitMessage( 0 );
      return TRUE;

   case WM_NOTIFY:
      switch (((NMHDR*) lparam)->idFrom)
      {
      case IDC_TABS:
         if (((NMHDR*) lparam)->code == TCN_SELCHANGE)
         {
            dlg_show_tab( (Dlg_Tab) TabCtrl_GetCurSel( GetDlgItem( wnd, IDC_TABS ) ) );
            return TRUE;
         }
         break;

      case IDC_MOD_LIST:
         if (((NMHDR*) lparam)->code == LVN_BEGINDRAG)
         {
            g_modlist_dragging = true;
            SetFocus( ((NMHDR*) lparam)->hwndFrom );
            SetCapture( wnd );
            return TRUE;
         }
         break;
      }
      break;

   case WM_COMMAND:
      switch (LOWORD( wparam ))
      {
      case IDOK:
         if (HIWORD( wparam ) == BN_CLICKED)
         {
            dlg_save_fields( );
            PostMessage( wnd, WM_CLOSE, 0, 0 );
            return TRUE;
         }
         break;

      case IDCANCEL:
         if (HIWORD( wparam ) == BN_CLICKED)
         {
            PostMessage( wnd, WM_CLOSE, 0, 0 );
            return TRUE;
         }
         break;

      case IDC_GEN_PATHBROWSE:
         if (HIWORD( wparam ) == BN_CLICKED)
         {
            OPENFILENAME ofn;
            memset( &ofn, 0, sizeof( ofn ) );
            ofn.lStructSize  = sizeof( ofn );
            ofn.hwndOwner    = wnd;
            ofn.lpstrFilter  = "*.exe\0*.exe\0*.*\0*.*\0\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFile    = new char[ MAX_PATH ];
            ofn.nMaxFile     = MAX_PATH;
            GetDlgItemText( wnd, IDC_GEN_PATH, ofn.lpstrFile, ofn.nMaxFile );
            ofn.Flags        = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

            if (GetOpenFileName( &ofn ))
               SetDlgItemText( wnd, IDC_GEN_PATH, ofn.lpstrFile );

            delete [] ofn.lpstrFile;

            return TRUE;
         }
         break;
      case IDC_ADV_INITCMDS:
         if (HIWORD( wparam ) == BN_CLICKED)
         {
            string path;
            if (IsDlgButtonChecked( wnd, IDC_ADV_SINGLEUSER ) == BST_UNCHECKED)
            {
               path = get_user_root( ) + "\\source\\init.txt";
               if (GetFileAttributes( path.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
                  path = get_cwd( ) + "\\source\\init.txt";
            }
            else
               path = get_cwd( ) + "\\source\\init.txt";

            ShellExecute( NULL, "open", path.c_str( ), NULL, 
                          path.substr( 0, path.find_last_of( '\\' ) ).c_str( ), SW_SHOWNORMAL );
            return TRUE;
         }
         break;
      case IDC_ADV_LOGINCMDS:
         if (HIWORD( wparam ) == BN_CLICKED)
         {
            string path;
            if (IsDlgButtonChecked( wnd, IDC_ADV_SINGLEUSER ) == BST_UNCHECKED)
            {
               path = get_user_root( ) + "\\source\\login.txt";
               if (GetFileAttributes( path.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
                  path = get_cwd( ) + "\\source\\login.txt";
            }
            else
               path = get_cwd( ) + "\\source\\login.txt";

            ShellExecute( NULL, "open", path.c_str( ), NULL, 
                          path.substr( 0, path.find_last_of( '\\' ) ).c_str( ), SW_SHOWNORMAL );
            return TRUE;
         }
         break;
      }
      break;

   case WM_CAPTURECHANGED:
      {
         if (g_modlist_dragging)
         {
            HWND ctrl = GetDlgItem( wnd, IDC_MOD_LIST );
            POINT pt;
            GetCursorPos( &pt );
            ScreenToClient( ctrl, &pt );

            const int n = ListView_GetItemCount( ctrl );
            int insert_pos = 0;
            RECT r;
            for (int i = 0; i < n; ++i, ++insert_pos)
            {
               ListView_GetItemRect( ctrl, i, &r, LVIR_BOUNDS );
               if (PtInRect( &r, pt ))
                  break;
            }

            if (ListView_GetSelectedCount( ctrl ))
            {
               struct _MoveItem
               {
                  string name;
                  UINT   state;
                  BOOL   checked;
                  _MoveItem( string name, DWORD state, bool checked ) : name( name ), state( state ), checked( checked ) { }
               };

               list<_MoveItem> moves;

               LVITEM item;
               item.mask       = LVIF_STATE | LVIF_TEXT;
               item.stateMask  = -1;
               item.pszText    = new char[ MAX_PATH ];
               item.cchTextMax = MAX_PATH;
               item.iSubItem   = 0;

               /* Move each selected item. */

               for (item.iItem = 0; item.iItem < n; ++item.iItem)
               {
                  if (ListView_GetItem( ctrl, &item ) && item.state & LVIS_SELECTED)
                  {
                     moves.push_back( _MoveItem( item.pszText, item.state, 
                                                 ListView_GetCheckState( ctrl, item.iItem ) ) );
                     ListView_DeleteItem( ctrl, item.iItem );
                     --item.iItem;
                  }
               }

               delete [] item.pszText;
               item.iItem = insert_pos;
               for (list<_MoveItem>::const_iterator i = moves.begin( ); i != moves.end( ); ++i, ++item.iItem)
               {
                  item.pszText = const_cast<char*>( i->name.c_str( ) );
                  item.state   = i->state;
                  ListView_InsertItem( ctrl, &item );
                  ListView_SetCheckState( ctrl, item.iItem, i->checked );
               }
            }

            g_modlist_dragging = false;
         }
      }
      break;

   case WM_LBUTTONUP:
      if (g_modlist_dragging)
      {
         ReleaseCapture( );
         return TRUE;
      }
      break;
   }

   return FALSE;
}

//////////////////////////////////////////////////////////////////////////
