#ifndef _EGGY_H__
#define _EGGY_H__

//#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _WIN32_WINNT 0x0501

#include "furn2/furn2.h"
#include "yiffurc/yiffurc.h"

#include <WinUser.h>
#include <Windows.h>
#include <WindowsX.h>
#include <commctrl.h>
#include <string>
#include <list>
#include <ctime>
#include <cassert>
#include <sstream>
#include <queue>

namespace eggy
{

#define EGGY_VERSION "1.5-r4"

using namespace std;
using namespace furn2;
using namespace yiffurc;

//////////////////////////////////////////////////////////////////////////

enum On_Contact { CONTACT_ONLINE, CONTACT_DESC, CONTACT_NEARBY, /* CONTACT_LASTSEEN, */
                  CONTACT_BUDDY, CONTACT_NAME };
enum { OPT_AUTOENABLE = 0x1, OPT_AUTOPM = 0x2, OPT_HIDEWHISPERS = 0x4,
       OPT_HIDEERRORS = 0x8, OPT_AUTOLOG = 0x10, OPT_ANNOUNCELOGOFFS = 0x20,
       OPT_SEPARATELOGS = 0x40, OPT_ANNOUNCENEARBY = 0x80, OPT_AUTOOFFLINE = 0x100,
       OPT_BUDDYLISTAOT = 0x200 };

class Contact;
class BuddyList;
class PmSession;
class OfflineMessages;

//////////////////////////////////////////////////////////////////////////

extern HINSTANCE    s_inst;
extern unsigned int s_mod;

//////////////////////////////////////////////////////////////////////////

class Eggy
{
   struct AUTOSUMMON { Contact* contact; unsigned int duration; };
   enum PlayerMenu { PLAYERMENU_WHISPER, PLAYERMENU_SESSIONADD, PLAYERMENU_ADDBUDDY, PLAYERMENU_REMOVEBUDDY };

public:
   /* Factories. */
   static Eggy* singleton_create( HWND parent_wnd );
   static Eggy* singleton( );
   static void  singleton_destroy( );

private:
   Eggy( HWND parent_wnd );
   ~Eggy( );

public:
   void onTick( unsigned int dt );
   bool onINet( Revision rev, const string& line, int from );
   bool onONet( Revision rev, const string& line, int from );
   bool onWndProc( WNDPROCPARAMS* params );
   void onLoggedIn( );
   void onDisconnected( );
   intptr_t onModMsg( const string& msg );
   bool onCommand( int argc, const char* argv[] );
   void onPlayerMenuShow( const string& name );
   void toggleBuddyList( );
   void showOptions( );
   void showAbout( );
   void enable( bool onoff );
   HWND getParentWnd( ) const;
   HICON getIcon( ) const;
   string getLogFile( ) const;
   string getLogFolder( ) const;
   int getOptFlags( ) const;
   OfflineMessages* getOfflineMessages( );
   BuddyList* getBuddyList( );
   void setPmSize( unsigned width, unsigned height );
   bool isEnabled( ) const;
   bool isLoggedIn( ) const;
   bool isMapReady( ) const;
   void createLogDirectory( ) const;
   bool repairAutoLog( );

   /** Create/Retrieve a contact.
   * If the contact already exists, the existing contact will
   * have its reference count incremented and be returned. */
   Contact* contactCreate( const string& name );
   /** Destroys a contact created by contactCreate.
   * Contact should automatically call this when its refcount 
   * reaches zero. */
   void     contactDestroy( Contact* contact );
   /** Broadcast a contact change event. */
   void     onContact( const string& name, On_Contact how );
   /** Create an empty PM session. */
   PmSession* pmCreate( );
   /** Create/Retrieve a PM session.
   * If a PM session exists with \a recipient as a recipient, the
   * session will be returned.  If \a raise is \c true, the session
   * will also be raised. */
   PmSession* pmCreate( const string& recipient, bool raise = false );
   /** Find an existing PM session. */
   PmSession* pmFind( const string& recipient );
   /** Close a PM session created by pmCreate. */
   void       pmDestroy( PmSession* pm );
   /** Sends a summon request and adds the name to the autosummon list. */
   void       autoSummon( const string& name );

   /** Broadcasts a message to all other eggy instances to synchronize
   *   settings. */
   void broadcastSync( ) const;
   /** Posts a sync message to this thread's message queue. */
   void resync( );
   /** Refreshes contact statuses. */
   void refreshContacts( );

private:
   void readOptions( );
   void writeOptions( );

   Contact* contactFind( const string& name );
   POINT calcNextPmPosition( ) const;
   bool onSayFrom( const string& from, const string& msg );
   bool onWhisperTo( const string& to, const string& msg );
   bool onWhisperFrom( const string& from, const string& msg );
   bool onWhisperEcho( const string& to, const string& msg );
   bool onWhisperFail( const string& name );
   bool onWhisperDenied( const string& name );
   bool onOnln( const string& name, bool onoff );
   bool onDescHead( const string& name );
   bool onDescBody( const string& name, const string& body );
   bool onDeclined( const string& name );
   bool onDeclineEcho( const string& name );
   bool onSummonJoinEcho( const string& name );
   bool onSummon( const string& name );
   bool onJoin( const string& name );
   bool onJoined( const string& name );
   bool onSummoned( const string& name );
   bool onMapLoad( );
   bool onMapReady( );

   static INT_PTR CALLBACK opt_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
   static INT_PTR CALLBACK about_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );

private:
   static Eggy*     s_singleton;
   HICON            m_icon;
   HWND             m_parent_wnd;
   bool             m_enabled;
   bool             m_loggedin;
   NET_SESSION      m_net_session;
   UINT             m_sync_msg;
   bool             m_refresh_nearby;
 //  HMENU            m_playermenu;
   
   unsigned int     m_poll_cooldown;
   list<Contact*>   m_contacts;

   BuddyList*       m_buddy_list;
   list<PmSession*> m_pm_sessions;

   list<AUTOSUMMON> m_summons;

   HWND             m_opt_wnd;
   unsigned int     m_poll_rate;
   int              m_opt_flags;
   string           m_log_file;
   unsigned         m_pm_w, m_pm_h;

   HWND             m_about_wnd;

   bool             m_map_loading;

   OfflineMessages* m_offline_messages;

   unsigned int     m_resync_cooldown;
   unsigned int     m_autosave_cooldown;
};

inline Eggy* Eggy::singleton_create( HWND parent_wnd )
{
   if (s_inst && !s_singleton)
         new Eggy( parent_wnd ); // s_singleton initialized in ctor
   else if (s_singleton)
      s_singleton->m_parent_wnd = parent_wnd;
   return s_singleton;
}

inline Eggy* Eggy::singleton( ) {
   return s_singleton;
}

inline void Eggy::singleton_destroy( ) {
   delete s_singleton; // s_singleton nulled in dtor
}

inline HICON Eggy::getIcon( ) const {
   return m_icon;
}

inline HWND Eggy::getParentWnd( ) const {
   return m_parent_wnd;
}

inline int Eggy::getOptFlags( ) const {
   return m_opt_flags;
}

inline bool Eggy::isLoggedIn( ) const {
   return m_loggedin;
}

inline bool Eggy::isMapReady( ) const {
   return !m_map_loading;
}

inline bool Eggy::isEnabled( ) const {
   return m_enabled;
}

inline OfflineMessages* Eggy::getOfflineMessages( ) {
   return m_offline_messages;
}

inline BuddyList* Eggy::getBuddyList( ) {
   return m_buddy_list;
}

//////////////////////////////////////////////////////////////////////////

class Contact
{
private:
   string       m_name;
   string       m_shortname;
   bool         m_online;
   bool         m_nearby;
   string       m_desc;
   time_t       m_lastseen;
   unsigned int m_refcount;
   unsigned int m_freezecount;
   bool         m_buddy;
   unsigned     m_guid;

private:
   Contact( ) { }
public:
   Contact( const string& name );
   ~Contact( ) { }

public:
   void         addRef( ) { ++m_refcount; }
   unsigned int countRef( ) { return m_refcount; }
   void         release( );

   bool          adjustName( const string& name );
   const string& getName( ) const { return m_name; }
   const string& getShortName( ) const { return m_shortname; }
   void          setOnline( bool online );
   bool          getOnline( ) const { return m_online; }
   void          setNearby( bool nearby );
   bool          getNearby( ) const { return m_nearby; }
   void          setDesc( const string& desc );
   const string& getDesc( ) const { return m_desc; }
   void          setLastSeen( time_t t );
   time_t        getLastSeen( ) const { return m_lastseen; }
   void          setBuddy( bool yesno );
   bool          isBuddy( ) const { return m_buddy; }
   void          setGuid( unsigned guid );
   unsigned      getGuid( ) const { return m_guid; }
   void          freeze( ) { ++m_freezecount; }
   void          thaw( ) { --m_freezecount; }
   bool          isFrozen( ) { return (m_freezecount >= m_refcount); }
   string        makeTip( ) const;
};

//////////////////////////////////////////////////////////////////////////

class BuddyList
{
   enum Tree_Item_Type { TREEITEM_BUDDY, TREEITEM_GROUP, 
                         TREEITEM_ADDBUDDY, TREEITEM_ADDGROUP };
   struct BUDDY;
   struct GROUP;

public:
   static void class_init( );
   static void class_uninit( );

public:
   BuddyList( );
   ~BuddyList( );

   HWND getWnd( ) const;
   bool isBuddy( const string& name ) const;
   void show( bool onoff );
   void setAlwaysOnTop( bool onoff );
   bool getAlwaysOnTop( ) const;
   bool isVisible( ) const;
   void onContact( const string& name, On_Contact how );
   void onTick( unsigned int dt );
   void onMapReady( );

   /** Add/Moves a buddy to a group.
   * If the buddy is already in another group, it will be moved.
   * If the buddy is already in the group, nothing will happen.
   * If the group doesn't exist, it will be created, as well.
   * An empty string for a group name implies the unsorted group.
   * \return The BUDDY instance, or NULL if \a name is invalid. */
   BUDDY* addBuddy( const string& name, const string& group = "" );
   /** Removes a buddy from any group. */
   bool   removeBuddy( const string& name );
   /** Adds a group.
   * If the group already exists, nothing will happen.
   * \return The GROUP instance, or NULL if \a name is invalid. */
   GROUP* addGroup( const string& name );
   /** Removes a group and all buddies in it.
   * If \a name is an empty string, the unsorted group will be
   * cleared but cannot be deleted. */
   bool   removeGroup( const string& name );
   /** Freezes/Unfreezes an entire group of contacts. */
   bool   freezeGroup( const string& name, bool freeze );
   /** Returns a list of group names. */
   list<string> getGroups( ) const;

   void readList( );
   void writeList( );

private:

   void messageGroup( const string& name, bool online_only );
   void summonGroup( const string& name );
   void summonBuddy( const string& name );
   void joinBuddy( const string& name );

   static LRESULT CALLBACK wnd_proc( HWND wnd, UINT msg, 
                                     WPARAM wparam, LPARAM lparam );
   static LRESULT CALLBACK treeview_proc( HWND wnd, UINT msg, 
                                          WPARAM wparam, LPARAM lparam );
   static int CALLBACK treeview_cmp( LPARAM a, LPARAM b, LPARAM sort_param );

   void sortTreeItems( );
   void fitWindowElements( );
   /** Searches all groups for a buddy by name. */
   bool findBuddy( const string& name, 
                   BUDDY** buddy_out = NULL, GROUP** group_out = NULL );
   bool findBuddy( const string& name, 
                   const BUDDY** buddy_out = NULL, const GROUP** group_out = NULL ) const;
   /** Fills out a TVITEMEX struct for a group.
   *  \warning The pszText member should be deleted when done. */
   void makeTreeItem( GROUP* in, TVITEMEX* out ) const;
   /** Fills out a TVITEMEX struct for a buddy.
   *  \warning The pszText member should be deleted when done. */
   void makeTreeItem( BUDDY* in, TVITEMEX* out ) const;

private:
   list<GROUP*> m_groups;
   struct
   {
      HWND wnd;
      HWND tree;
   }            m_wnd;
   struct  
   {
      string       drag_buddy;
      string       drop_group;
      unsigned int scroll_ms;
      bool isDragging( ) { return drag_buddy.length( ) != 0; }
   }            m_drag;
   bool m_always_on_top;
};

inline bool BuddyList::isVisible( ) const {
   return IsWindowVisible( m_wnd.wnd ) != FALSE ? true : false;
}

inline HWND BuddyList::getWnd( ) const {
   return m_wnd.wnd;
}

inline bool BuddyList::getAlwaysOnTop( ) const {
   return m_always_on_top;
}

struct BuddyList::BUDDY
{
   const Tree_Item_Type type;
   Contact*             contact;
   HTREEITEM            tree_handle;

   BUDDY( Contact* contact = NULL ) 
      : type( TREEITEM_BUDDY ), contact( contact ), tree_handle( NULL ) { }
};

struct BuddyList::GROUP
{
   const Tree_Item_Type type;
   string               name;
   HTREEITEM            tree_handle;
   bool                 frozen;
   list<BUDDY*>         buddies;

   GROUP( const string& name = "" ) 
      : type( TREEITEM_GROUP ), name( name ), tree_handle( NULL ), frozen( false ) { }
};

//////////////////////////////////////////////////////////////////////////

class PmSession
{
public:
   enum List_Item_Type { LISTITEM_RECIPIENT, LISTITEM_ADDRECIPIENT };
   struct RECIPIENT;

public:
   static void class_init( );
   static void class_uninit( );

private:
   class PmSessionWindow;

public:
   PmSession( int x = 100, int y = 100, unsigned width = 390, unsigned height = 250 );
   ~PmSession( );

   void         raise( );
   bool         isRecipient( const string& name ) const;
   bool         addRecipient( const string& name );
   void         removeRecipient( const string& name );
   bool         detachRecipient( const string& name );
   unsigned int numRecipients( ) const;
   void         showRecipientsPane( bool onoff );
   string       getTitle( ) const;
   // Like getTitle but without the player's own name.
   string       getHalfTitle( ) const;
   HWND         getWnd( ) const;

   void onContact( const string& name, On_Contact how );
   bool onWhisperTo( const string& to, const string& msg );
   bool onWhisperEcho( const string& to, const string& msg );
   bool onWhisperFrom( const string& from, const string& msg );
   bool onWhisperFail( const string& name );
   bool onWhisperDenied( const string& name );
   bool onSummonJoinEcho( const string& name );
   bool onSummon( const string& name );
   bool onJoin( const string& name );
   bool onJoined( const string& name );
   bool onSummoned( const string& name );
   bool onDeclined( const string& name );
   bool onDeclineEcho( const string& name );
   void onLoggedIn( );
   void onDisconnected( );

private:
   static LRESULT CALLBACK wnd_proc( HWND wnd, UINT msg,
                                     WPARAM wparam, LPARAM lparam );
   static LRESULT CALLBACK input_proc( HWND wnd, UINT msg,
                                       WPARAM wparam, LPARAM lparam );
   void refreshElements( );
   void fitWindowElements( );
   void makeListItem( RECIPIENT* in, LVITEM* out ) const;
   void appendChat( const string& name, const string& msg );
   void appendMyChat( const string& name, const string& msg );
   void appendEmote( const string& name, const string& msg );
   void appendMsg( const string& msg, int icon );
   void appendOfflineChat( const string& name, const string& msg );

   RECIPIENT*       findRecipient( const string& name );
   const RECIPIENT* findRecipient( const string& name ) const;

public:
   void doWhisper( const string& msg );
   void doWhisper( const wstring& msg );
   void doJoin( const string& name );
   void doSummon( );
   void doDecline( );
   void doIgnore( );
   void doUnignore( );
   void doSaveLogAs( );
   void doViewLog( );
   void doShareRecips( );
   void doSysMsg( const string& msg );
   void doAddBuddy( const string& group = "" );
   void doRemoveBuddy( );

private:
   void doSummon( const string& name );
   void doIgnore( const string& name );
   void doUnignore( const string& name );
   void doLaunchLink( size_t index );
   void doCopyLink( size_t index );

private:
   list<RECIPIENT*> m_recipients;
   bool             m_recipients_visible;
   time_t           m_last_sent;
   time_t           m_last_received;
   const time_t     m_created;
   bool             m_worth_saving;
   ostringstream    m_log;
   list<string>     m_links;
   HMENU            m_groups_menu;
   list<wstring>    m_input_history;
   struct  
   {
      HWND wnd;
      HWND chat;
      HWND input;
      HWND status;
      HWND ltoolbar;
      HWND rtoolbar;
      HWND send;
      HWND recipients_label;
      HWND recipients;
   }                m_wnd;
};

struct PmSession::RECIPIENT
{
   const List_Item_Type type;
   Contact*             contact;
   // Last whisper sent TO the recipient.
   string               last_sent;
   // Receieved an error since coming online.
   bool                 hopeless;
   RECIPIENT( Contact* contact ) 
      : type( LISTITEM_RECIPIENT ), contact( contact ), hopeless( false )
      { }
};

inline unsigned int PmSession::numRecipients( ) const {
   return (unsigned int) m_recipients.size( );
}

inline HWND PmSession::getWnd( ) const {
   return m_wnd.wnd;
}

//////////////////////////////////////////////////////////////////////////

class OfflineMessages
{
   enum Sort_By { SORT_FROM, SORT_TO, SORT_MSG, SORT_DATE };
   struct OFFLINEMSG
   {
      unsigned id;
      string   from;
      Contact* to;
      string   msg;
      time_t   date;
      OFFLINEMSG( unsigned id = 0, const string& from = "", Contact* to = NULL, const string& msg = "", time_t date = 0 )
         : id( id ), from( from ), to( to ), msg( msg ), date( date ) { if (to) to->addRef( ); }
      OFFLINEMSG( const OFFLINEMSG& r )
         : id( r.id ), from( r.from ), to( r.to ), msg( r.msg ), date( r.date ) { if (to) to->addRef( ); }
      ~OFFLINEMSG( ) { if (to) to->release( ); }
      bool operator ==( const OFFLINEMSG& b ) const
      { return id == b.id; }
      bool operator ==( const unsigned& id ) const
      { return OFFLINEMSG::id == id; }
   };

public:
   static void class_init( );
   static void class_uninit( );

public:
   OfflineMessages( );
   ~OfflineMessages( );

public:
   unsigned add( const string& from, const string& to, const string& msg, time_t date = 0 );
   bool     remove( unsigned id );
   void     show( bool showhide = true );
   void     edit( unsigned id );
   unsigned count( ) const;
   void     clear( );
   bool     load( );
   bool     save( );

   void onContact( const string& name, On_Contact how );

private:
   unsigned nextId( ) const;
   void     syncUi( );
   list<OFFLINEMSG>::iterator find( unsigned id );
   void     beginEdit( unsigned id = -1 );

   static INT_PTR CALLBACK dlg_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
   static INT_PTR CALLBACK edit_dlg_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );
   static int CALLBACK list_sort( LPARAM a, LPARAM b, LPARAM sort_param );

private:
   list<OFFLINEMSG> m_messages;
   HWND             m_wnd;
   HWND             m_edit_wnd;
   HMENU            m_list_menu;
   Sort_By          m_sort;   
};

inline unsigned OfflineMessages::count( ) const
{
   return (unsigned) m_messages.size( );
}

//////////////////////////////////////////////////////////////////////////

inline WNDPROC get_sys_class_proc( const char* name )
{
   WNDCLASSEX wc;
   if (GetClassInfoEx( NULL, name, &wc ))
      return wc.lpfnWndProc;
   assert( false );
   return NULL;
}

static void eggy_init( HINSTANCE inst, unsigned int mod )
{
   if (!s_inst && (s_inst = inst))
   {
      s_mod = mod;
      BuddyList::class_init( );
      PmSession::class_init( );
   }
}

static void eggy_uninit( )
{
   if (s_inst)
   {
      BuddyList::class_uninit( );
      PmSession::class_uninit( );
      s_inst = NULL;
      s_mod  = -1;
   }
}

static string make_shortname( const string& name )
{
   char* buf = new char[ name.length( ) + 1 ];
   furn2::f2_makeshortname( name.c_str( ), buf );
   string shortname = buf;
   delete [] buf;
   return shortname;
}

template <class T_Str>
static T_Str pipes_to_spaces( const T_Str& name )
{
   T_Str r = name;
   for (T_Str::iterator i = r.begin( ); i != r.end( ); ++i)
      if ((char) *i == '|')
         *i = ' ';
   return r;
}

inline bool operator ==( const Contact& a, const string& b )
{
   return a.getShortName( ) == make_shortname( b );
}

static string to_xml_friendly( const string& str )
{
   string xml_str;
   for (size_t i = 0; i < str.length( ); ++i)
   {
      switch (str[ i ])
      {
      case '<':
         xml_str += "&lt;"; break;
      case '>':
         xml_str += "&gt;"; break;
      case '&':
         xml_str += "&amp;"; break;
      default:
         xml_str += str[ i ];
      }
   }
   return xml_str;
}

template <class T_Str>
static string to_path_friendly( const T_Str& str )
{
   /* Only allow [-a-zA-Z0-9 ] characters. */
   T_Str r;
   for (T_Str::const_iterator i = str.begin( ); i != str.end( ); ++i)
   {
      if ((*i >= (T_Str::value_type) (unsigned) 'a' &&
           *i <= (T_Str::value_type) (unsigned) 'z') ||
          (*i >= (T_Str::value_type) (unsigned) 'A' &&
           *i <= (T_Str::value_type) (unsigned) 'Z') ||
          (*i >= (T_Str::value_type) (unsigned) '0' &&
           *i <= (T_Str::value_type) (unsigned) '9') ||
          (*i == (T_Str::value_type) (unsigned) ' ') ||
          (*i == (T_Str::value_type) (unsigned) '-'))
      {
         r.push_back( *i );
      }
      else
      {
         char sz_hex[ 4 ];
         sprintf_s( sz_hex, sizeof( sz_hex ), "%02X", (unsigned char) *i );
         r.append( sz_hex );
      }
   }
   return r;
}

/* This is useless because tinyxml already does this automatically. */
#if 0
static string from_xml_friendly( const string& xml_str )
{
   string str;
   for (size_t i = 0; i < xml_str.length( );)
   {
      if (xml_str[ i ] == '&')
      {
         if (xml_str.compare( i, 4, "&lt;" ) == 0)
         {
            str += '<';
            i += 4;
            continue;
         }
         else if (xml_str.compare( i, 4, "&gt;" ) == 0)
         {
            str += '>';
            i += 4;
            continue;
         }
      }
      str += xml_str[ i++ ];
   }
   return str;
}
#endif

static bool create_folder_path( const string& path )
{
   if (GetFileAttributes( path.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
   {
      /* Folder(s) need to be created. */

      // Break up the different folders in the path.
      queue<string> folders;
      for (string::size_type off = 0; off < path.length( ); ++off )
      {
         const string::size_type end = path.find_first_of( "\\", off );
         if (path.npos == end)
         {
            folders.push( path.substr( off, path.npos ) );
            break;
         }
         folders.push( path.substr( off, end - off ) );
         off = end;
      }

      // Start creating the folder path.
      if (!folders.empty( ))
      {
         string curr_path;
         do
         {
            if (!curr_path.empty( ))
               curr_path += "\\";
            curr_path += folders.front( );
            folders.pop( );

            if (GetFileAttributes( curr_path.c_str( ) ) == INVALID_FILE_ATTRIBUTES)
            {
               // Need to create this folder.
               if (!CreateDirectory( curr_path.c_str( ), NULL ))
               {
                  if (GetLastError( ) != ERROR_ALREADY_EXISTS)
                  {
                     std::cerr << "Unable to create folder path up to " << curr_path << ".\n";
                     return false;
                  }
               }
            }

         } while (!folders.empty( ));
      }
   }
   return true;
}

//////////////////////////////////////////////////////////////////////////

}  // namespace eggy

#endif
