#ifndef _CURSES_H__
#define _CURSES_H__

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _WIN32_WINNT 0x0501
#include "furn2/furn2.h"
#include "yiffurc/util.hpp"
#include "CharacterIni.h"

#include <Windows.h>
#include <WindowsX.h>
#include <WinSock.h>
#include <string>
#include <list>
#include <cassert>
#include <sstream>

namespace curses {

using namespace furn2;
using namespace std;

//////////////////////////////////////////////////////////////////////////

class Curses;
class CursesScreen;

enum Digo_Type 
{ 
   DIGO_NONE = 0,
   DIGO_CLASSICS, DIGO_TRIS, DIGO_BUTTERS, DIGO_BATS, DIGO_PRIMES,
   DIGO_DRAGON, DIGO_PHOENIX, DIGO_GRYFFE, DIGO_EAGLE, DIGO_KIWI,
   DIGO_KITTER, DIGO_PIXIE, DIGO_WOLVEN, DIGO_TYGARD, DIGO_LEONEN,
   DIGO_WEREWOLF, DIGO_PENGUIN, DIGO_BAT, DIGO_FOXEN, DIGO_FLOXEN,
   DIGO_UNICORN, DIGO_FURLING, DIGO_CHINCHILLA, DIGO_TOASTER,
   
   DIGO_KIRIN, DIGO_NOBLECANINE, DIGO_NOBLEFELINE,
   DIGO_REINDEER, DIGO_CATTEN, DIGO_OWLEN, DIGO_LOVEBIRD
};

//////////////////////////////////////////////////////////////////////////

struct LOGININFO
{
   string                name;
   string                password;
   string                description;
   yiffurc::ColorCodePal color;
   Digo_Type             digo;
   string                filename;
   LOGININFO( ) : digo( DIGO_NONE ) { }
   bool isEmpty( ) const
   {
      return name.empty( ) && password.empty( ) && description.empty( ) &&
             color == yiffurc::ColorCodePal( ) && filename.empty( );
   }
};

//////////////////////////////////////////////////////////////////////////

class Curses
{
   class Job;
   class JobDefaultLogin;
   class JobLogin;
   class JobReLogin;
   class JobRegister;
   class JobReRegister;

public:
   enum { OPT_AUTOSTART = 0x1, OPT_REMEMBERPW = 0x2 };

   static Curses* singleton_create( HINSTANCE inst, unsigned int mod );
   static Curses* singleton( );
   static bool singleton_destroy( );

private:
   Curses( HINSTANCE inst, unsigned int mod );
   Curses( ) { }
   ~Curses( );

public:
   void                setWnd( HWND wnd );
   HWND                getWnd( ) const;
   HINSTANCE           getInstance( ) const;
   unsigned int        getModule( ) const;
   const list<string>& getIniPaths( ) const;
   long                getOptions( ) const;

   void onConnected( );
   void onClientScreen( Client_Screen screen );
   bool onCommand( int argc, const char* argv[] );
   void onEnable( bool onoff );
   void onLoggedIn( );
   void onDisconnected( );
   void onNetIn( const string& line, int from, size_t num );
   void onNetOut( const string& line, int from, size_t num );
   bool onWndProc( WNDPROCPARAMS* params );
   void onSetMainSocket( SOCKET socket );
   
   void screenPlay( );
   void screenClose( );
   void screenRegister( );

   void showScreen( );
   void showOptions( );
   void terminateJob( );

   const string& getDefaultIni( ) const;
   void          setDefaultIni( const string& ini );
   const string& getSettingsFile( ) const;

private:
   static INT_PTR CALLBACK options_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
   static HANDLE CALLBACK CreateFileA( LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, 
                                       LPSECURITY_ATTRIBUTES lpSecurityAttributes, 
                                       DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, 
                                       HANDLE hTemplateFile );
   static BOOL CALLBACK DeleteFileA( LPCSTR lpFileName );
   void readOptions( );
   void writeOptions( );

private:
   HWND          m_wnd;
   HINSTANCE     m_inst;
   bool          m_logged_in;
   unsigned int  m_module;
   bool          m_enabled;
   SOCKET        m_main_socket;
   CursesScreen* m_screen;
   LOGININFO*    m_login_info;
   Job*          m_job;
   string        m_localdir;
   list<string>  m_connect_buf;
   list<string>  m_ini_paths;
   string        m_last_email;
   long          m_options;
   HWND          m_options_dlg;
   string        m_default_ini;
   string        m_settings_file;

   // Hook chains must be static.
   static void* s_old_createfilea;
   static void* s_old_deletefilea;

   static Curses* s_singleton;
};

inline Curses* Curses::singleton( ) { return s_singleton; }
inline Curses* Curses::singleton_create( HINSTANCE inst, unsigned int mod )
{
   if (!s_singleton)
      new Curses( inst, mod ); // s_singleton initialized in ctor.
   return s_singleton;
}
inline bool Curses::singleton_destroy( ) 
{ 
   delete s_singleton; // s_singleton uninited in dtor. 
   return s_singleton == NULL;
}
inline HWND Curses::getWnd( ) const { return m_wnd; }
inline HINSTANCE Curses::getInstance( ) const { return m_inst; }
inline unsigned int Curses::getModule( ) const { return m_module; }
inline const list<string>& Curses::getIniPaths( ) const { return m_ini_paths; }
inline long Curses::getOptions( ) const { return m_options; }
inline const string& Curses::getDefaultIni( ) const { return m_default_ini; }
inline const string& Curses::getSettingsFile( ) const { return m_settings_file; }

//////////////////////////////////////////////////////////////////////////

class CursesScreen
{
   class Assets;
   class FshBitmapInfo;
   struct CONTROLS
   {
      HWND m_background;
      HWND m_register;
      HWND m_play;
      HWND m_close;
      HWND m_save;
      HWND m_saveas;
      HWND m_settings;
      HWND m_characters;
      HWND m_portrait;
      HWND m_name;
      HWND m_password;
      HWND m_description;
      HWND m_species;
      HWND m_gender;
      HWND m_code;
      HWND m_colors;
      HWND m_swatch;
      HWND m_preview;
      HWND m_digo;
      HWND m_filename;
      HWND m_status;
   };

public:
   CursesScreen( );
   CursesScreen( const LOGININFO& info );
   ~CursesScreen( );

public:
   bool onWndProc( WNDPROCPARAMS* params );
   void onRender( );
   void setConnected( bool connected );
   void refreshCharacters( );
   void fillForm( const LOGININFO& info );
   void getLoginInfo( LOGININFO* out ) const;
   void selectDefaultCharacter( );
   void setCloseable( bool closeable );

private:
   void getCharIni( CharacterIni* ini );
   bool promptSave( );
   void newChar( );
   void deleteChar( );
   bool save( );
   bool saveAs( );
   bool selectIniFile( const string& name );
   void syncColorCode( );
   void markUnsaved( bool unsaved );
   bool isUnsaved( ) const;
   void _initOnce( );
   static LRESULT CALLBACK container_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
   
private:
   CONTROLS              m_controls;
   Assets*               m_assets;
   FshBitmapInfo*        m_fshbmpinfo;
   yiffurc::ColorCodePal m_colorcode;
   list<string>          m_inis;
   string                m_curr_ini;
   bool                  m_connected;
   string                m_last_save_dir;
   HMENU                 m_menus;
};

//////////////////////////////////////////////////////////////////////////

template <class T_Vec>
size_t line_split( const string& line, T_Vec* out )
{
   out->clear( );
   istringstream ss( line );
   string arg;
   for (unsigned ch; ss.good( ) && (ch = ss.peek( )) != EOF; )
   {
      if (!iswspace( ch ))
      {
         if (ch == '"') // '"'-terminated.
         {
            ss.ignore( );
            getline( ss, arg, '"' );
         }
         else  // whitespace-terminated.
         {
            ss >> arg;
         }

         out->push_back( arg );
      }
      else
         ss.ignore( );
   }

   return out->size( );
}

inline string pipen( const string& str )
{
   string result( str );
   for (string::iterator i = result.begin( ); i != result.end( ); ++i)
   {
      if (*i == ' ')
         *i = '|';
   }
   return result;
}

inline string unpipen( const string& str )
{
   string result( str );
   for (string::iterator i = result.begin( ); i != result.end( ); ++i)
   {
      if (*i == '|')
         *i = ' ';
   }
   return result;
}

static int path_cmp( const string& file_a, const string& file_b )
{
   char sz_full[ MAX_PATH ];
   GetFullPathName( file_a.c_str( ), MAX_PATH, sz_full, NULL );
   sz_full[ MAX_PATH - 1 ] = '\0';
   const string full_a = sz_full;
   GetFullPathName( file_b.c_str( ), MAX_PATH, sz_full, NULL );
   sz_full[ MAX_PATH - 1 ] = '\0';
   const string full_b = sz_full;

   return stricmp( full_a.c_str( ), full_b.c_str( ) );
}

//////////////////////////////////////////////////////////////////////////

} // namespace curses

#endif