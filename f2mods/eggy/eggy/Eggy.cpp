#include "eggy.h"
#include "furn2/furn2.h"
#include "yiffurc/yiffurc.h"
#include "resource.h"
#include "tinyxml/tinyxml.h"
#include "resource.h"
#include <stdexcept>
#include <fstream>

namespace eggy {

using namespace yiffurc;

//////////////////////////////////////////////////////////////////////////

enum { POLL_RATE_DEFAULT = 1000, 
       AUTOSUMMON_DURATION = 300000 /* 5 minutes */,
       PLAYERMENU_SESSIONID_START = 3493,
       PLAYERMENU_GROUPID_START = 3593,
       MAP_BEGIN_COOLDOWN = 5000,
       AUTOSAVE_COOLDOWN = 3600000 /* 10 minutes */,
     };

//////////////////////////////////////////////////////////////////////////

HINSTANCE    s_inst            = NULL;
unsigned int s_mod             = -1;
Eggy*        Eggy::s_singleton = NULL;

//////////////////////////////////////////////////////////////////////////

Eggy::Eggy( HWND parent_wnd ) 
   : m_icon( NULL ), m_parent_wnd( parent_wnd ),
     m_buddy_list( NULL ), 
     m_loggedin( false ), m_poll_cooldown( 0 ), m_enabled( false ),
     m_opt_wnd( NULL ), m_opt_flags( 0 ),
     m_pm_w( 390 ), m_pm_h( 250 ),
     m_poll_rate( POLL_RATE_DEFAULT ),
     m_sync_msg( 0 ), m_refresh_nearby( false ), 
     m_about_wnd( NULL ), m_map_loading( false ), m_offline_messages( NULL ),
     m_resync_cooldown( 0 ), m_autosave_cooldown( AUTOSAVE_COOLDOWN )
{
   assert( s_inst );

   s_singleton = this;

   m_icon = LoadIcon( s_inst, MAKEINTRESOURCE( IDI_ICON1 ) );
   memset( &m_net_session, 0, sizeof( m_net_session ) );
   
   m_sync_msg = RegisterWindowMessage( "Eggy Synchronize" );
// m_playermenu = LoadMenu( s_inst, MAKEINTRESOURCE( IDR_PLAYERMENU ) );
//   f2_playermenu_set( s_mod, GetSubMenu( m_playermenu, 0 ) );

   // Create the eggy player menu.

   /* Initialize options to defaults. */
   {
      m_opt_flags = /* OPT_AUTOENABLE | */ OPT_AUTOLOG | OPT_AUTOPM |
                    OPT_HIDEERRORS;
      m_poll_rate = POLL_RATE_DEFAULT;
      m_log_file  = "autolog.txt";
   }

   readOptions( );

   // Make the log folder.
   create_folder_path( getLogFolder( ) );

   m_buddy_list = new BuddyList( );
   m_buddy_list->readList( );

   m_offline_messages = new OfflineMessages( );
   m_offline_messages->load( );

   if (m_opt_flags & OPT_AUTOENABLE)
      f2_modenable( s_mod, true );
}

Eggy::~Eggy( )
{
   // Write on exit.
   m_offline_messages->save( );
   m_buddy_list->writeList( );
   writeOptions( );
   broadcastSync( );

   while (!m_pm_sessions.empty( ))
   {
      delete m_pm_sessions.front( );
      m_pm_sessions.pop_front( );
   }

   delete m_buddy_list;
   delete m_offline_messages;

   for (list<AUTOSUMMON>::iterator i = m_summons.begin( ); i != m_summons.end( ); ++i)
      i->contact->release( );
   m_summons.clear( );

   assert( m_contacts.empty( ) );

   f2_playermenu_unset( s_mod );
//   DestroyMenu( m_playermenu );
   DestroyIcon( m_icon );

   s_singleton = NULL;
}

Contact* Eggy::contactFind( const string& name )
{
   const string shortname = make_shortname( name );
   for (list<Contact*>::iterator i = m_contacts.begin( ); 
        i != m_contacts.end( ); ++i)
   {
      if ((*i)->getShortName( ) == shortname)
         return (*i);
   }

   return NULL;
}

Contact* Eggy::contactCreate( const string& name )
{
   if (!make_shortname( name ).length( ))
      return NULL;

   /* Return an existing contact, if possible. */
   Contact* contact = contactFind( name );
   if (contact)
   {
      contact->addRef( );
      return contact;
   }

   /* Create a new one and add to the back of the pool. */
   contact = new Contact( name );
   m_contacts.push_front( contact ); // New contacts get checked sooner
   contact->addRef( );  // This is for the caller, the pool doesn't count.
   refreshContacts( ); // So a test is made soon for the visibility of the contact.
   return contact;
}

void Eggy::contactDestroy( Contact* contact )
{
   assert( contact->countRef( ) == 0 );

   for (list<Contact*>::iterator i = m_contacts.begin( ); 
        i != m_contacts.end( ); ++i)
   {
      if ((*i) == contact)
      {
         // Remove from pool.
         m_contacts.erase( i );
         // Deallocate.
         delete contact;
         return;
      }
   }

   assert( false );
}

void Eggy::toggleBuddyList( ) {
   m_buddy_list->show( !m_buddy_list->isVisible( ) );
}

void Eggy::enable( bool onoff ) {
   m_enabled = onoff;
}

bool Eggy::repairAutoLog( )
{
   string log_file = getLogFile( );
   char sz_tmp_file[ MAX_PATH ];
   if (!GetTempFileName( getLogFolder( ).c_str( ), "autolog_repair", 0, sz_tmp_file ))
      return false;

   ofstream os( sz_tmp_file );
   if (os.fail( ))
      return false;

   ifstream is( log_file.c_str( ), ifstream::binary );
   if (is.fail( ))
      return false;
   
   string line;
   while (is.good( ))
   {
      getline( is, line );
      os << line << std::endl;
   }

   is.close( );
   os.close( );

   return 0 != MoveFileEx( sz_tmp_file, log_file.c_str( ), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH );
}

POINT Eggy::calcNextPmPosition( ) const
{
   POINT pos = { 100, 100 };
   if (!m_pm_sessions.size( ))
   {
      // Center within the parent window.
      WINDOWPLACEMENT wndpl;
      wndpl.length = sizeof( wndpl );
      if (GetWindowPlacement( m_parent_wnd, &wndpl ))
      {
         pos.x = wndpl.rcNormalPosition.left + ((wndpl.rcNormalPosition.right - wndpl.rcNormalPosition.left) - m_pm_w) / 2;
         pos.y = wndpl.rcNormalPosition.top + ((wndpl.rcNormalPosition.bottom - wndpl.rcNormalPosition.top) - m_pm_h) / 2;
      }
   }
   // If another PM window exists, cascade the new one.
   else
   {
      WINDOWPLACEMENT wndpl;
      wndpl.length = sizeof( wndpl );
      if (GetWindowPlacement( m_pm_sessions.back( )->getWnd( ), &wndpl ))
      {
         pos.x = wndpl.rcNormalPosition.left + 20;
         pos.y = wndpl.rcNormalPosition.top + 20;
      }
   }

   return pos;
}

PmSession* Eggy::pmCreate( )
{
   POINT pos = calcNextPmPosition( );
   PmSession* pm = new PmSession( pos.x, pos.y, m_pm_w, m_pm_h );
   // Insert after the last PM session window.
   if (m_pm_sessions.size( ))
      SetWindowPos( pm->getWnd( ), m_pm_sessions.back( )->getWnd( ), 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE );
   m_pm_sessions.push_back( pm );
   return pm;
}

PmSession* Eggy::pmCreate( const string& recipient, bool raise /* = false */ )
{
   PmSession* pm = pmFind( recipient );
   if (!pm)
   {
      /* Create a new one. */
      POINT pos = calcNextPmPosition( );
      pm = new PmSession( pos.x, pos.y, m_pm_w, m_pm_h );
      // Insert after the last PM session window.
      if (m_pm_sessions.size( ))
         SetWindowPos( pm->getWnd( ), m_pm_sessions.back( )->getWnd( ), 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE );
      m_pm_sessions.push_back( pm );
      pm->addRecipient( recipient );
   }

   if (raise)
      pm->raise( );

   return pm;   
}

PmSession* Eggy::pmFind( const string& recipient )
{
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      if ((*i)->isRecipient( recipient ))
         // Found it.
         return (*i);
   }

   return NULL;
}

void Eggy::pmDestroy( PmSession* pm )
{
   /* Look for the session. */
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      if (pm == (*i))
      {
         m_pm_sessions.erase( i );
         delete pm;
         return;
      }
   }

   assert( false );
}

void Eggy::autoSummon( const string& name )
{
   const string shortname = make_shortname( name );

   /* Send a summon. */
   {
      const char* cmd = c2s_enc_summon( ("%" + shortname).c_str( ) );
      f2_netout( cmd, strlen( cmd ) );
      yiffurc_release( cmd );
   }

   /* Check if the name is already on the list. */
   for (list<AUTOSUMMON>::iterator i = m_summons.begin( );
        i != m_summons.end( ); ++i)
   {
      if (i->contact->getShortName( ) == shortname)
      {
         // Just renew the lease.
         i->duration = AUTOSUMMON_DURATION;
         return;
      }
   }

   /* Add to the auto-summon list. */
   AUTOSUMMON summon = { contactCreate( name ), AUTOSUMMON_DURATION };
   if (summon.contact)
   {
      // Store them frozen.
      summon.contact->freeze( );
      m_summons.push_back( summon );
   }
}

void Eggy::showOptions( )
{
   if (!IsWindow( m_opt_wnd ))
   {
      m_opt_wnd = CreateDialog( s_inst, MAKEINTRESOURCE( IDD_OPTIONS ), 
                                     m_buddy_list->getWnd( ), opt_proc );

      // Center inside the parent client area.
      RECT parent_rect;
      GetWindowRect( m_buddy_list->getWnd( ), &parent_rect );
      parent_rect.right = parent_rect.right - parent_rect.left;
      parent_rect.bottom = parent_rect.bottom - parent_rect.top;
      RECT dlg_rect;
      GetWindowRect( m_opt_wnd, &dlg_rect );
      dlg_rect.right = dlg_rect.right - dlg_rect.left;
      dlg_rect.bottom = dlg_rect.bottom - dlg_rect.top;
      POINT pt = { 
                    parent_rect.left + (parent_rect.right - dlg_rect.right) / 2,
                    parent_rect.top + (parent_rect.bottom - dlg_rect.bottom) / 2
                 };
      SetWindowPos( m_opt_wnd, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
   }

   // Show.
   ShowWindow( m_opt_wnd, SW_SHOWNORMAL );
   SetForegroundWindow( m_opt_wnd );
}

void Eggy::showAbout( )
{
   if (!IsWindow( m_about_wnd ))
   {
      m_about_wnd = CreateDialog( s_inst, MAKEINTRESOURCE( IDD_ABOUT ), 
                                  m_buddy_list->getWnd( ), about_proc );

      // Center inside the parent client area.
      RECT parent_rect;
      GetWindowRect( m_buddy_list->getWnd( ), &parent_rect );
      parent_rect.right = parent_rect.right - parent_rect.left;
      parent_rect.bottom = parent_rect.bottom - parent_rect.top;
      RECT dlg_rect;
      GetWindowRect( m_about_wnd, &dlg_rect );
      dlg_rect.right = dlg_rect.right - dlg_rect.left;
      dlg_rect.bottom = dlg_rect.bottom - dlg_rect.top;
      POINT pt = { 
                    parent_rect.left + (parent_rect.right - dlg_rect.right) / 2,
                    parent_rect.top + (parent_rect.bottom - dlg_rect.bottom) / 2
                 };
      SetWindowPos( m_about_wnd, NULL, pt.x, pt.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER );
   }

   // Show.
   ShowWindow( m_about_wnd, SW_SHOWNORMAL );
   SetForegroundWindow( m_about_wnd );
}

void Eggy::onContact( const string& name, On_Contact how )
{
   m_buddy_list->onContact( name, how );

   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      (*i)->onContact( name, how );
   }

   m_offline_messages->onContact( name, how );
}

void Eggy::onTick( unsigned int dt )
{
   /* Perform contact polling. */
   if (m_enabled && m_loggedin)
   {
      m_poll_cooldown += dt;

      if (m_poll_cooldown >= m_poll_rate)
      {
         const size_t num_contacts = m_contacts.size( );
         /* Ping one unfrozen contact. */
         for (size_t i = 0; i < num_contacts; ++i)
         {
            // Next up is always at the front of the list.
            Contact* contact = m_contacts.front( );

            // Rotate to the back of the list.
            m_contacts.pop_front( );
            m_contacts.push_back( contact );

            if (!contact->isFrozen( ))
            {
               // Poll this one.
               const char* cmd = 
                  yiffurc::c2s_enc_onlinecheck( contact->getShortName( ).c_str( ) );
               furn2::f2_netout( cmd, strlen( cmd ) );
               yiffurc::yiffurc_release( cmd );
               break;
            }
         }

         m_poll_cooldown = 0;
      }
   }

   /* Prune auto-summons. */
   for (list<AUTOSUMMON>::iterator i = m_summons.begin( );
        i != m_summons.end( ); )
   {
      if (i->duration <= dt)
      {
         i->contact->thaw( );
         i->contact->release( );
         m_summons.erase( i++ );
      }
      else
      {
         i->duration -= dt;
         ++i;
      }
   }

   /* Countdown to resync. */
   if (m_resync_cooldown)
   {
      if (m_resync_cooldown <= dt)
      {
         m_resync_cooldown = 0;
         PostMessage( m_parent_wnd, m_sync_msg, 0, 0 );
      }
      else
         m_resync_cooldown -= dt;
   }

   /* Countdown to autosave. */
   if (m_autosave_cooldown <= dt)
   {
      m_autosave_cooldown = AUTOSAVE_COOLDOWN;

      m_offline_messages->save( );
      m_buddy_list->writeList( );
      writeOptions( );
      broadcastSync( );
   }
   else
      m_autosave_cooldown -= dt;

   /* Refresh nearby contacts. */
   if (m_refresh_nearby)
   {
      /* Check if contacts are on the same map. */
      MONSTERINFO mi;
      for (list<Contact*>::iterator i = m_contacts.begin( ); i != m_contacts.end( ); )
      {         
         (*i)->addRef( );
         f2_monsterinfo_byname( (*i)->getShortName( ).c_str( ), &mi, true );
         if (mi.guid)
         {
            (*i)->setGuid( mi.guid );
            (*i)->setNearby( true );
         }
         else
            (*i)->setNearby( false );
         (*i++)->release( );
      }
      m_refresh_nearby = false;
   }

   /* Pass it on. */
   m_buddy_list->onTick( dt );
}

void Eggy::onLoggedIn( )
{
   m_loggedin = true;
   m_net_session.level = NET_LEVEL_LOGGEDIN;

   /* Pass it on. */
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      (*i)->onLoggedIn( );
   }   
}

void Eggy::onDisconnected( )
{
   m_loggedin = false;
   // Reset session info.
   memset( &m_net_session, 0, sizeof( m_net_session ) );

#if 0
   /* Mark all the contacts as offline. */
   for (list<Contact*>::iterator i = m_contacts.begin( ); 
        i != m_contacts.end( ); ++i)
   {
     (*i)->setOnline( false );
   }
#endif

   /* Pass it on. */
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      (*i)->onDisconnected( );
   }
}

bool Eggy::onWhisperFrom( const string& from, 
                          const string& msg ) 
{
   if (m_enabled && m_opt_flags & OPT_AUTOPM)
      pmCreate( from );

   Contact* f = contactFind( from );
   if (f)
   {
      f->adjustName( from );
      f->setOnline( true );
   }

   /* Pass it on. */
   bool handled = false;
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      handled |= (*i)->onWhisperFrom( from, msg );
   }

   if (m_opt_flags & OPT_HIDEWHISPERS)
   {
      // Push the name because F2 won't see it.
      f2_name_push( from.c_str( ) );
      return handled;
   }
   return false;
}

bool Eggy::onWhisperEcho( const string& to, 
                          const string& msg )
{
   if (m_enabled && m_opt_flags & OPT_AUTOPM)
      pmCreate( to );

   Contact* f = contactFind( to );
   if (f)
   {
      f->adjustName( to );
      f->setOnline( true );
   }

   /* Pass it on. */
   bool handled = false;
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
      i != m_pm_sessions.end( ); ++i)
   {
      handled |= (*i)->onWhisperEcho( to, msg );
   }

   if (m_opt_flags & OPT_HIDEWHISPERS)
   {
      // Push the name because F2 won't see it.
      f2_name_push( to.c_str( ) );
      return handled;
   }
   return false;
}

bool Eggy::onWhisperTo( const string& to, const string& msg )
{
   /*
   if (m_enabled && m_opt_flags & OPT_AUTOPM)
      pmCreate( to );
   */

   /* Pass it on. */
   bool handled = false;
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
      i != m_pm_sessions.end( ); ++i)
   {
      handled |= (*i)->onWhisperTo( to, msg );
   }

   return false;
}

bool Eggy::onWhisperFail( const string& name )
{
   Contact* f = contactFind( name );
   if (f) f->setOnline( false );

   /* Pass it on. */
   bool handled = false;
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      handled |= (*i)->onWhisperFail( name );
   }

   return m_opt_flags & OPT_HIDEERRORS ? handled : false;
}

bool Eggy::onWhisperDenied( const string& name )
{
   Contact* f = contactFind( name );
   if (f)
   {
      f->adjustName( name );
      f->setOnline( true );
   }

   /* Pass it on. */
   bool handled = false;
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
      i != m_pm_sessions.end( ); ++i)
   {
      handled |= (*i)->onWhisperDenied( name );
   }

   return m_opt_flags & OPT_HIDEERRORS ? handled : false;
}

bool Eggy::onDeclined( const string& name )
{
   Contact* f = contactFind( name );
   if (f)
   {
      f->adjustName( name );
      f->setOnline( true );
   }

   /* Pass it on. */
   bool handled = false;
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      handled |= (*i)->onDeclined( name );
   }

   return false;
}

bool Eggy::onDeclineEcho( const string& name )
{
   // Unsure if this can only happen if the other guy is online.

   /* Pass it on. */
   bool handled = false;
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      handled |= (*i)->onDeclineEcho( name );
   }

   return false;
}

bool Eggy::onSummonJoinEcho( const string& name )
{
   Contact* f = contactFind( name );
   if (f)
   {
      f->adjustName( name );
      f->setOnline( true );
   }

   /* Pass it on. */
   bool handled = false;
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      handled |= (*i)->onSummonJoinEcho( name );
   }

   return false;
}

bool Eggy::onSummon( const string& name )
{
   if (m_enabled && m_opt_flags & OPT_AUTOPM)
      pmCreate( name );

   Contact* f = contactFind( name );
   if (f)
   {
      f->adjustName( name );
      f->setOnline( true );
   }

   /* Pass it on. */
   bool handled = false;
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      handled |= (*i)->onSummon( name );
   }

   return false;
}

bool Eggy::onJoin( const string& name )
{
   if (m_enabled && m_opt_flags & OPT_AUTOPM)
      pmCreate( name );

   Contact* f = contactFind( name );
   if (f)
   {
      f->adjustName( name );
      f->setOnline( true );
   }

   const string shortname = make_shortname( name );

   /* If on the autosummon list, just summon and don't pass it on. */
   for (list<AUTOSUMMON>::iterator i = m_summons.begin( );
        i != m_summons.end( ); ++i)
   {
      if (i->contact->getShortName( ) == shortname)
      {
         const char* cmd = c2s_enc_summon( ("%" + shortname).c_str( ) );
         f2_netout( cmd, strlen( cmd ) );
         yiffurc_release( cmd );
         return false;
      }
   }

   /* Pass it on. */
   bool handled = false;
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      handled |= (*i)->onJoin( name );
   }

   return false;
}

bool Eggy::onJoined( const string& name )
{
   Contact* f = contactFind( name );
   if (f)
   {
      f->adjustName( name );
      f->setOnline( true );
   }

   const string shortname = make_shortname( name );

   /* Remove from autosummon list. */
   for (list<AUTOSUMMON>::iterator i = m_summons.begin( );
        i != m_summons.end( ); ++i)
   {
      if (i->contact->getShortName( ) == shortname)
      {
         i->contact->thaw( );
         i->contact->release( );
         m_summons.erase( i );
         break;
      }
   }

   /* Pass it on. */
   bool handled = false;
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      handled |= (*i)->onJoined( name );
   }

   return false;
}

bool Eggy::onSummoned( const string& name )
{
   Contact* f = contactFind( name );
   if (f)
   {
      f->adjustName( name );
      f->setOnline( true );
   }

   /* Pass it on. */
   bool handled = false;
   for (list<PmSession*>::iterator i = m_pm_sessions.begin( );
        i != m_pm_sessions.end( ); ++i)
   {
      handled |= (*i)->onSummoned( name );
   }

   return false;
}

bool Eggy::onDescHead( const string& name )
{
   Contact* f = contactFind( name );
   if (f)
   {
      f->adjustName( name );
      f->setOnline( true );
   }

   return false;
}

bool Eggy::onDescBody( const string& name, const string& body )
{
   // name is actually a shortname.
   Contact* f = contactFind( name );
   if (f)
   {
      f->setDesc( body );
   }

   return false;
}

bool Eggy::onOnln( const string& name, bool onoff )
{
   Contact* f = contactFind( name );
   if (f)
   {
      if (onoff)  // Only when they're online is the name expanded.
         f->adjustName( name );
      f->setOnline( onoff );
   }

   return false;
}

bool Eggy::onSayFrom( const string& from, const string& msg )
{
   Contact* f = contactFind( from );
   if (f)
   {
      f->adjustName( from );
      f->setOnline( true );
   }

   return false;
}

bool Eggy::onMapLoad( )
{
   // Set all contacts as not nearby.
   for (list<Contact*>::iterator i = m_contacts.begin( ); i != m_contacts.end( );)
      (*i++)->setNearby( false );

   m_map_loading = true;
   return false;
}

bool Eggy::onMapReady( )
{
   m_map_loading = false;
   m_buddy_list->onMapReady( );
   return false;
}

bool Eggy::onINet( Revision rev, const string& line, int from )
{
   if (!m_loggedin)
      return false;

   bool cancel = false;

   if (rev == REV_FINAL)
   {
      const S2C* cmd = s2c_parse( line.c_str( ), line.length( ), &m_net_session );
      if (cmd)
      {
         switch (cmd->type)
         {
         case S2C_TYPE_SPEECH:
            cancel = onSayFrom( ((S2C_SPEECH*)cmd)->from, ((S2C_SPEECH*)cmd)->message );
            break;
         case S2C_TYPE_WHISPER:
            cancel = onWhisperFrom( ((S2C_WHISPER*)cmd)->from, ((S2C_WHISPER*)cmd)->message );
            break;
         case S2C_TYPE_YOUWHISPER:
            cancel = onWhisperEcho( ((S2C_YOUWHISPER*)cmd)->to, ((S2C_YOUWHISPER*)cmd)->message );
            break;
         case S2C_TYPE_WHISPERFAILEXACT:
            cancel = onWhisperFail( ((S2C_WHISPERFAILEXACT*)cmd)->player );
            break;
         case S2C_TYPE_WHISPERDENIED:
            cancel = onWhisperDenied( ((S2C_WHISPERDENIED*)cmd)->player );
            break;
         case S2C_TYPE_DESCHEAD:
            cancel = onDescHead( ((S2C_DESCHEAD*)cmd)->player );
            break;
         case S2C_TYPE_ONLINERESULT:
            cancel = onOnln( ((S2C_ONLINERESULT*)cmd)->player, ((S2C_ONLINERESULT*)cmd)->online );
            break;
         case S2C_TYPE_DECLINED:
            cancel = onDeclined( ((S2C_DECLINED*)cmd)->player );
            break;
         case S2C_TYPE_DECLINEECHO:
            cancel = onDeclineEcho( ((S2C_DECLINEECHO*)cmd)->player );
            break;
         case S2C_TYPE_SUMMONJOINECHO:
            cancel = onSummonJoinEcho( ((S2C_SUMMONJOINECHO*)cmd)->player );
            break;
         case S2C_TYPE_JOIN:
            cancel = onJoin( ((S2C_JOIN*)cmd)->from );
            break;
         case S2C_TYPE_SUMMON:
            cancel = onSummon( ((S2C_SUMMON*)cmd)->from );
            break;
         case S2C_TYPE_JOINED:
            cancel = onJoined( ((S2C_JOINED*)cmd)->joiner );
            break;
         case S2C_TYPE_SUMMONED:
            cancel = onSummoned( ((S2C_SUMMONED*)cmd)->summoner );
            break;
         case S2C_TYPE_DESCBODY:
            cancel = onDescBody( ((S2C_DESCBODY*)cmd)->player_short, ((S2C_DESCBODY*)cmd)->body );
            break;
         case S2C_TYPE_DESCBODY2:
            cancel = onDescBody( ((S2C_DESCBODY2*)cmd)->player_short, ((S2C_DESCBODY2*)cmd)->body );
            break;
         case S2C_TYPE_USEMAPFILE:
         case S2C_TYPE_FETCHDREAM:
            onMapLoad( );
            break;
         case S2C_TYPE_CAMSET:
            if (m_map_loading)
               onMapReady( );
            break;
         case S2C_TYPE_AVCREATE:
            {
               Contact* c = contactFind( ((S2C_AVCREATE*)cmd)->name );
               if (c)
               {
                  c->addRef( );
                  c->setGuid( ((S2C_AVCREATE*)cmd)->uid );
                  c->setNearby( true );
                  c->release( );
               }
            }
            break;
         case S2C_TYPE_AVDESTROY:
            {
               for (list<Contact*>::iterator i = m_contacts.begin( ); i != m_contacts.end( );)
               {
                  if ((*i)->getGuid( ) && (*i)->getGuid( ) == ((S2C_AVDESTROY*)cmd)->uid)
                     (*i++)->setNearby( false );
                  else
                     ++i;
               }
            }
            break;
         }

         yiffurc_release( cmd );
      }
   }

   if (cancel)
      f2_badge_cancel( );

   return cancel;
}

bool Eggy::onONet( Revision rev, const string& line, int from )
{
   if (!m_loggedin)
      return false;

   bool cancel = false;

   if (rev == REV_FINAL)
   {
      const C2S* cmd = c2s_parse( line.c_str( ), line.length( ), &m_net_session );
      if (cmd)
      {
         switch (cmd->type)
         {
         case C2S_TYPE_WHISPER:
            cancel = onWhisperTo( ((C2S_WHISPER*)cmd)->player, ((C2S_WHISPER*)cmd)->message );
            break;
            /*
         case C2S_TYPE_DOWNLOADFINISHED:
            onMapLoad( );
            break;*/
         }

         yiffurc_release( cmd );
      }
   }

   return cancel;
}

bool Eggy::onWndProc( WNDPROCPARAMS* params )
{
   if (params->msg == m_sync_msg && (HWND) params->wparam != params->wnd)
   {
      readOptions( );
      m_buddy_list->readList( );
      m_offline_messages->load( );
      params->retval = 0;
      return true;
   }
/*   // Handle menu commands.
   else if (params->msg == WM_COMMAND && params->lparam == NULL && HIWORD( params->wparam ) == 0)
   {
      const int id = LOWORD( params->wparam );
      switch (id)
      {
      case ID_PLAYER_MESSAGE:
         if (*f2__playermenu_name( ))
            pmCreate( f2__playermenu_name( ), true );
         return true;
      default:
         {
            const HMENU playermenu = GetSubMenu( m_playermenu, 0 );
            HMENU menu = GetSubMenu( playermenu, 2 );
            // Add to group option?
            if (id >= PLAYERMENU_GROUPID_START && id < PLAYERMENU_GROUPID_START + GetMenuItemCount( menu ))
            {
               char sz_group[ 512 ];
               sz_group[ 0 ] = '\0';
               if (id != PLAYERMENU_GROUPID_START)
               {
                  GetMenuString( menu, id - PLAYERMENU_GROUPID_START, sz_group, 512, MF_BYPOSITION );
                  sz_group[ 511 ] = '\0';
               }
               if (*f2__playermenu_name( ))
                  m_buddy_list->addBuddy( f2__playermenu_name( ), sz_group );
               return true;
            }

            menu = GetSubMenu( playermenu, 1 );
            // Add to session option?
            if (id >= PLAYERMENU_SESSIONID_START && id < PLAYERMENU_SESSIONID_START + GetMenuItemCount( menu ))
            {
               MENUITEMINFO mii;
               memset( &mii, 0, sizeof( mii ) );
               mii.cbSize = sizeof( mii );
               mii.fMask = MIIM_DATA;
               GetMenuItemInfo( menu, id - PLAYERMENU_SESSIONID_START, TRUE, &mii );
               for (list<PmSession*>::iterator i = m_pm_sessions.begin( ); i != m_pm_sessions.end( ); ++i)
               {
                  if ((uintptr_t) mii.dwItemData == (uintptr_t) *i)
                  {
                     if (*f2__playermenu_name( ))
                        (*i)->addRecipient( f2__playermenu_name( ) );
                  }
               }
               return true;
            }
         }
      }
   } */
   return false;
}

int Eggy::onModMsg( const string& msg )
{
   istringstream ss( msg );
   if (!ss.good( ))
      return 0;

   string cmd;
   ss >> cmd;

   if (cmd == "pm")
   {
      list<string> recips;
      string name;
      while (ss.good( ))
      {
         ss >> name;
         if (name.length( ))
            recips.push_back( name );
      }

      // If there's just one, create/raise the PM the player is in.
      // If there's more than one, create a new PM and add them in.
      PmSession* session = NULL;
      if (recips.size( ) == 1)
      {
         session = pmCreate( recips.front( ), true );
      }
      else if (recips.size( ) > 1)
      {
         session = pmCreate( );
         while (!recips.empty( ))
         {
            session->addRecipient( recips.front( ) );
            recips.pop_front( );
         }

         // Show the recipients pane.
         session->showRecipientsPane( true );
      }

      return session ? session->numRecipients( ) : 0;
   }
   else if (cmd ==  "add")
   {
      string buddy;
      ss >> buddy;
      string group;
      getline( ss, group );
      {
         // Trim leading whitespace.
         size_t off = group.find_first_not_of( " \t" );
         if (off != group.npos)
            group = group.substr( off, group.npos );
         else
            group.clear( );
      }

      if (buddy.length( ))
      {
         if (m_buddy_list->addBuddy( buddy, group ))
         {
            m_buddy_list->writeList( );
            broadcastSync( );
            return true;
         }
      }
      return false;
   }
   else if (cmd == "isbud")
   {
      string buddy;
      ss >> buddy;

      Contact* contact = contactFind( buddy );
      if (contact && contact->isBuddy( ))
         return true;
      return false;
   }
   else if (cmd == "stat")
   {
      string buddy;
      ss >> buddy;

      Contact* contact = contactFind( buddy );
      if (contact)
         return contact->getOnline( );
      return false;
   }

   return 0;
}

bool Eggy::onCommand( int argc, const char* argv[] )
{
   if (argc < 1)
      return false;

   if (argc == 1 && !_stricmp( argv[ 0 ], "who" )) {
	   //@who command - lists people on your buddylist who are currently marked as online.
	std::string tmpstr = "People Online: ";
	bool first = true;
	for (list<Contact*>::iterator i = m_contacts.begin( ); 
        i != m_contacts.end( ); ++i)
     {
		 if ((*i)->getOnline()) {
			 if (!first) { tmpstr += ", "; }
			 tmpstr += "<name shortname='" + (*i)->getShortName() + "'>" + (*i)->getName() +"</name>";
			 first = false;
		 }
     }
   if (first) { tmpstr += "<i>None</i>"; }
   f2_speak(tmpstr.c_str());
   return true;
   }

   if (argc >= 1 && !_stricmp( argv[ 0 ], "eggy" ))
   {
      if (argc == 1)
      {
         m_buddy_list->show( !m_buddy_list->isVisible( ) );
         return true;
      }

      if (argc >= 2 && !_stricmp( argv[ 1 ], "repair" ))
      {
         if (argc == 3 && !_stricmp( argv[ 2 ], "autolog" ))
         {
            if (repairAutoLog( ))
               f2_speak( "Autolog file repaired." );
            else
               f2_errspeak( "Failed to repair autolog file!" );
            return true;
         }

         f2_speak( "Syntax is: eggy repair autolog" );
         return true;
      }
      return false;
   }

   return false;
}

void Eggy::onPlayerMenuShow( const string& name )
{ /*
   HMENU playermenu = GetSubMenu( m_playermenu, 0 );

   // Enable or disable the Add to Buddies option.  
   if (m_buddy_list->isBuddy( name ))
      EnableMenuItem( playermenu, 2, MF_GRAYED | MF_BYPOSITION );
   else
      EnableMenuItem( playermenu, 2, MF_ENABLED | MF_BYPOSITION );

   // Populate the Add to Buddies submenu. 
   if (!m_buddy_list->isBuddy( name ))
   {
      HMENU menu = GetSubMenu( playermenu, 2 );

      while (GetMenuItemCount( menu ) > 0)
         DeleteMenu( menu, 0, MF_BYPOSITION );

      int curr_id = PLAYERMENU_GROUPID_START;
      AppendMenu( menu, MF_STRING, curr_id++, "(No Group)" );
      const list<string> groups = m_buddy_list->getGroups( );
      for (list<string>::const_iterator i = groups.begin( ); i != groups.end( ); ++i)
      {
         if (i->length( ))
         AppendMenu( menu, MF_STRING, curr_id++, i->c_str( ) );
      }
   }

   // Enable or disable the Add to Session option. 
   if (m_pm_sessions.size( ))
      EnableMenuItem( playermenu, 1, MF_ENABLED | MF_BYPOSITION );
   else
      EnableMenuItem( playermenu, 1, MF_GRAYED | MF_BYPOSITION );

   // Populate the sessions submenu.
   if (m_pm_sessions.size( ))
   {
      HMENU menu = GetSubMenu( playermenu, 1 );

      while (GetMenuItemCount( menu ) > 0)
         DeleteMenu( menu, 0, MF_BYPOSITION );

      MENUITEMINFO mii;
      memset( &mii, 0, sizeof( mii ) );
      mii.cbSize = sizeof( mii );

      int curr_id = PLAYERMENU_SESSIONID_START;
      for (list<PmSession*>::const_iterator i = m_pm_sessions.begin( );
           i != m_pm_sessions.end( ); ++i)
      {
         AppendMenu( menu, MF_STRING, curr_id++, (*i)->getHalfTitle( ).c_str( ) );
         mii.fMask = MIIM_DATA;
         mii.dwItemData = (DWORD) (uintptr_t) *i;
         SetMenuItemInfo( menu, GetMenuItemCount( menu ) - 1, TRUE, &mii );
      }
   } */
}

void Eggy::readOptions( )
{
   // Prefer the document folder one if it exists.
   string filename = string( f2_moddocroot( s_mod ) ) + "\\options.xml";

   if (INVALID_FILE_ATTRIBUTES == GetFileAttributes( filename.c_str( ) ))
      filename = string( f2_modroot( s_mod ) ) + "\\options.xml";

   TiXmlDocument doc( filename );
   if (!doc.LoadFile( ) || doc.Error( ))
   {
      // ErrorId 2 is "Failed to open file."
      if (doc.ErrorId( ) == 2 && INVALID_FILE_ATTRIBUTES != GetFileAttributes( filename.c_str( ) ))
      {
         // File exists but is locked? Try again.
         resync( );
      }
      else
         f2_errlog( "Eggy: options.xml file is damaged." );

      return;
   }

   const TiXmlElement* root = doc.RootElement( );
   if (!root || root->ValueStr( ) != "options")
   {
      f2_errspeak( "Eggy: options.xml file is malformed." );
      return;
   }

   /* Initialize options to defaults. */
   {
      m_opt_flags = OPT_AUTOENABLE | OPT_BUDDYLISTAOT | OPT_AUTOLOG | OPT_AUTOPM | OPT_HIDEERRORS | OPT_ANNOUNCENEARBY | OPT_AUTOOFFLINE;
      m_poll_rate = POLL_RATE_DEFAULT;
      m_log_file  = "autolog.txt";
   }

   const TiXmlElement* opt = root->FirstChildElement( "autoEnable" );
   if (opt && opt->GetText( ) && !strcmp( opt->GetText( ), "false" ) )
      m_opt_flags &= ~OPT_AUTOENABLE;
   else
      m_opt_flags |= OPT_AUTOENABLE;      

   opt = root->FirstChildElement( "buddyListAlwaysOnTop" );
   if (opt && opt->GetText( ) && !strcmp( opt->GetText( ), "false" ) )
      m_opt_flags &= ~OPT_BUDDYLISTAOT;
   else
      m_opt_flags |= OPT_BUDDYLISTAOT;      

   opt = root->FirstChildElement( "announceLogOffs" );
   if (opt && opt->GetText( ) && !strcmp( opt->GetText( ), "true" ) )
      m_opt_flags |= OPT_ANNOUNCELOGOFFS;
   else
      m_opt_flags &= ~OPT_ANNOUNCELOGOFFS;   

   opt = root->FirstChildElement( "announceNearby" );
   if (opt && opt->GetText( ) && !strcmp( opt->GetText( ), "false" ) )
      m_opt_flags &= ~OPT_ANNOUNCENEARBY;
   else
      m_opt_flags |= OPT_ANNOUNCENEARBY;

   opt = root->FirstChildElement( "autoOffline" );
   if (opt && opt->GetText( ) && !strcmp( opt->GetText( ), "false" ) )
      m_opt_flags &= ~OPT_AUTOOFFLINE;
   else
      m_opt_flags |= OPT_AUTOOFFLINE;

   opt = root->FirstChildElement( "autoPm" );
   if (opt && opt->GetText( ) && !strcmp( opt->GetText( ), "false" ) )
      m_opt_flags &= ~OPT_AUTOPM;
   else
      m_opt_flags |= OPT_AUTOPM;

   opt = root->FirstChildElement( "hideWhispers" );
   if (opt && opt->GetText( ) && !strcmp( opt->GetText( ), "true" ) )
      m_opt_flags |= OPT_HIDEWHISPERS;
   else
      m_opt_flags &= ~OPT_HIDEWHISPERS;

   opt = root->FirstChildElement( "hideErrors" );
   if (opt && opt->GetText( ) && !strcmp( opt->GetText( ), "true" ) )
      m_opt_flags |= OPT_HIDEERRORS;
   else
      m_opt_flags &= ~OPT_HIDEERRORS;

   opt = root->FirstChildElement( "autoLog" );
   if (opt && opt->GetText( ) && !strcmp( opt->GetText( ), "false" ) )
      m_opt_flags &= ~OPT_AUTOLOG;
   else
      m_opt_flags |= OPT_AUTOLOG;

   opt = root->FirstChildElement( "separateLogs" );
   if (opt && opt->GetText( ) && !strcmp( opt->GetText( ), "true" ) )
      m_opt_flags |= OPT_SEPARATELOGS;
   else
      m_opt_flags &= ~OPT_SEPARATELOGS;

   opt = root->FirstChildElement( "pollRate" );
   if (opt && opt->GetText( ))
      m_poll_rate = (int) (atof( opt->GetText( ) ) * 1000.0f);

   opt = root->FirstChildElement( "logFile" );
   if (opt && opt->GetText( ))
      m_log_file = opt->GetText( );

   if (m_poll_rate <= 0)
      m_poll_rate = POLL_RATE_DEFAULT;

   if (!m_log_file.length( ))
      m_log_file = "autolog.txt";

   if (m_buddy_list)
      m_buddy_list->setAlwaysOnTop( (m_opt_flags & OPT_BUDDYLISTAOT) != 0 );
}

string Eggy::getLogFile( ) const
{
   // Expand the log file if it's not an absolute path.
   if (m_log_file.find( ":\\" ) == m_log_file.npos 
       && m_log_file.find( "\\\\" ) != 0)
   {
      return string( f2_moddocroot( s_mod ) ) + "\\logs\\" + m_log_file;
   }

   return m_log_file;
}

string Eggy::getLogFolder( ) const
{
   // Expand the log folder if it's not an absolute path.
   if (m_log_file.find( ":\\" ) == m_log_file.npos 
       && m_log_file.find( "\\\\" ) != 0)
   {
      return string( f2_moddocroot( s_mod ) ) + "\\logs";
   }

   return m_log_file.substr( 0, m_log_file.rfind( '\\' ) );
}

void Eggy::setPmSize( unsigned width, unsigned height )
{
   m_pm_w = width;
   m_pm_h = height;
}

void Eggy::writeOptions( )
{
   HANDLE hf = CreateFile( (string( f2_moddocroot( s_mod ) ) + "\\options.xml").c_str( ), GENERIC_WRITE, 0, NULL,
                           CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
   if (hf != INVALID_HANDLE_VALUE)
   {
   
      ostringstream ss;
      ss.flags( ios_base::showpoint );
      ss.precision( 2 );

      ss << "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\r\n"
         << "<options>\r\n"
         << "\t<autoEnable>" << (m_opt_flags & OPT_AUTOENABLE ? "true" : "false") << "</autoEnable>\r\n"
         << "\t<buddyListAlwaysOnTop>" << (m_opt_flags & OPT_BUDDYLISTAOT ? "true" : "false") << "</buddyListAlwaysOnTop>\r\n"
         << "\t<pollRate>" << (m_poll_rate / 1000.0f) << "</pollRate>\r\n"
         << "\t<announceNearby>" << (m_opt_flags & OPT_ANNOUNCENEARBY ? "true" : "false") << "</announceNearby>\r\n"
         << "\t<announceLogOffs>" << (m_opt_flags & OPT_ANNOUNCELOGOFFS ? "true" : "false") << "</announceLogOffs>\r\n"
         << "\t<autoOffline>" << (m_opt_flags & OPT_AUTOOFFLINE ? "true" : "false") << "</autoOffline>\r\n"
         << "\t<autoPm>" << (m_opt_flags & OPT_AUTOPM ? "true" : "false") << "</autoPm>\r\n"
         << "\t<hideWhispers>" << (m_opt_flags & OPT_HIDEWHISPERS ? "true" : "false") << "</hideWhispers>\r\n"
         << "\t<hideErrors>" << (m_opt_flags & OPT_HIDEERRORS ? "true" : "false") << "</hideErrors>\r\n"
         << "\t<autoLog>" << (m_opt_flags & OPT_AUTOLOG ? "true" : "false") << "</autoLog>\r\n"
         << "\t<separateLogs>" << (m_opt_flags & OPT_SEPARATELOGS ? "true" : "false") << "</separateLogs>\r\n"
         << "\t<logFile>" << m_log_file << "</logFile>\r\n"
         << "</options>\r\n";

      {
         string str = ss.str( );
         DWORD written;
         WriteFile( hf, str.c_str( ), str.length( ), &written, NULL );
      }
      CloseHandle( hf );
   }
}

void Eggy::broadcastSync( ) const
{
   DWORD recips = BSM_APPLICATIONS;
   BroadcastSystemMessage( BSF_POSTMESSAGE | BSF_IGNORECURRENTTASK, 
                           &recips, m_sync_msg, (WPARAM) m_parent_wnd, 0 );
}

void Eggy::resync( ) {
   m_resync_cooldown = 500;
}

void Eggy::refreshContacts( ) {
   m_refresh_nearby = true;
}

INT_PTR CALLBACK Eggy::opt_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
   switch (msg)
   {
   case WM_INITDIALOG:
      {
         Eggy* inst = Eggy::singleton( );
         // Fill in the fields.
         CheckDlgButton( wnd, IDC_AUTOENABLE, 
                         inst->m_opt_flags & OPT_AUTOENABLE ? BST_CHECKED : BST_UNCHECKED );

         {
            ostringstream ss;
            ss.flags( ios_base::showpoint );
            ss.precision( 2 );
            ss << inst->m_poll_rate / 1000.0f;
            SetDlgItemText( wnd, IDC_POLLRATE, ss.str( ).c_str( ) );
         }

         CheckDlgButton( wnd, IDC_BUDDYLISTAOT,
                         inst->m_opt_flags & OPT_BUDDYLISTAOT ? BST_CHECKED : BST_UNCHECKED );
         CheckDlgButton( wnd, IDC_ANNOUNCELOGOFFS, 
                         inst->m_opt_flags & OPT_ANNOUNCELOGOFFS ? BST_CHECKED : BST_UNCHECKED );
         CheckDlgButton( wnd, IDC_ANNOUNCENEARBY, 
                         inst->m_opt_flags & OPT_ANNOUNCENEARBY ? BST_CHECKED : BST_UNCHECKED );

         CheckDlgButton( wnd, IDC_AUTOPM, 
                         inst->m_opt_flags & OPT_AUTOPM ? BST_CHECKED : BST_UNCHECKED );

         CheckDlgButton( wnd, IDC_HIDEWHISPERS, 
                         inst->m_opt_flags & OPT_HIDEWHISPERS ? BST_CHECKED : BST_UNCHECKED );

         CheckDlgButton( wnd, IDC_HIDEERRORS, 
                         inst->m_opt_flags & OPT_HIDEERRORS ? BST_CHECKED : BST_UNCHECKED );

         CheckDlgButton( wnd, IDC_AUTOLOG, 
                         inst->m_opt_flags & OPT_AUTOLOG ? BST_CHECKED : BST_UNCHECKED );

         CheckDlgButton( wnd, IDC_LOGSEPARATE, 
                         inst->m_opt_flags & OPT_SEPARATELOGS ? BST_CHECKED : BST_UNCHECKED );

         CheckDlgButton( wnd, IDC_AUTOOFFLINE, 
                         inst->m_opt_flags & OPT_AUTOOFFLINE ? BST_CHECKED : BST_UNCHECKED );

         EnableWindow( GetDlgItem( wnd, IDC_AUTOLOGFILE ),
                       inst->m_opt_flags & OPT_AUTOLOG ? TRUE : FALSE );
         EnableWindow( GetDlgItem( wnd, IDC_AUTOLOGBROWSE ),
                       inst->m_opt_flags & OPT_AUTOLOG ? TRUE : FALSE );
         EnableWindow( GetDlgItem( wnd, IDC_AUTOLOGSTATIC ),
                       inst->m_opt_flags & OPT_AUTOLOG ? TRUE : FALSE );

         SetDlgItemText( wnd, IDC_AUTOLOGFILE, inst->m_log_file.c_str( ) );
      }
      return TRUE;

   case WM_COMMAND:
      {
         switch (LOWORD( wparam ))
         {
         case IDC_AUTOLOG:
            if (HIWORD( wparam ) == BN_CLICKED)
            {
               if (IsDlgButtonChecked( wnd, IDC_AUTOLOG ))
               {
                  EnableWindow( GetDlgItem( wnd, IDC_AUTOLOGFILE ), TRUE );
                  EnableWindow( GetDlgItem( wnd, IDC_AUTOLOGBROWSE ), TRUE );
                  EnableWindow( GetDlgItem( wnd, IDC_AUTOLOGSTATIC ), TRUE );
               }
               else
               {
                  EnableWindow( GetDlgItem( wnd, IDC_AUTOLOGFILE ), FALSE );
                  EnableWindow( GetDlgItem( wnd, IDC_AUTOLOGBROWSE ), FALSE );
                  EnableWindow( GetDlgItem( wnd, IDC_AUTOLOGSTATIC ), FALSE );
               }
               return TRUE;
            }
            break;

         case IDC_AUTOLOGBROWSE:
            if (HIWORD( wparam ) == BN_CLICKED)
            {
               char* filename = new char[ MAX_PATH ];
               filename[ 0 ] = '\0';
               {
                  const string root = Eggy::singleton( )->getLogFolder( );

                  OPENFILENAME ofn;
                  memset( &ofn, 0, sizeof( ofn ) );
                  ofn.lStructSize = sizeof( ofn );
                  ofn.hwndOwner = wnd;
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
                     return TRUE;
                  }
               }

               SetDlgItemText( wnd, IDC_AUTOLOGFILE, filename );
               delete [] filename;

               return TRUE;
            }
            break;

         case IDOK:
            if (HIWORD( wparam ) == BN_CLICKED)
            {
               Eggy* inst = Eggy::singleton( );
               inst->m_opt_flags = 0;
               inst->m_opt_flags |= IsDlgButtonChecked( wnd, IDC_AUTOENABLE ) ? OPT_AUTOENABLE : 0;
               inst->m_opt_flags |= IsDlgButtonChecked( wnd, IDC_BUDDYLISTAOT ) ? OPT_BUDDYLISTAOT : 0;
               inst->m_opt_flags |= IsDlgButtonChecked( wnd, IDC_AUTOPM ) ? OPT_AUTOPM : 0;
               inst->m_opt_flags |= IsDlgButtonChecked( wnd, IDC_HIDEWHISPERS ) ? OPT_HIDEWHISPERS : 0;
               inst->m_opt_flags |= IsDlgButtonChecked( wnd, IDC_HIDEERRORS ) ? OPT_HIDEERRORS : 0;
               inst->m_opt_flags |= IsDlgButtonChecked( wnd, IDC_AUTOLOG ) ? OPT_AUTOLOG : 0;
               inst->m_opt_flags |= IsDlgButtonChecked( wnd, IDC_LOGSEPARATE ) ? OPT_SEPARATELOGS : 0;
               inst->m_opt_flags |= IsDlgButtonChecked( wnd, IDC_ANNOUNCELOGOFFS ) ? OPT_ANNOUNCELOGOFFS : 0;
               inst->m_opt_flags |= IsDlgButtonChecked( wnd, IDC_ANNOUNCENEARBY ) ? OPT_ANNOUNCENEARBY : 0;
               inst->m_opt_flags |= IsDlgButtonChecked( wnd, IDC_AUTOOFFLINE ) ? OPT_AUTOOFFLINE : 0;

               char buf[ MAX_PATH ];
               GetDlgItemText( wnd, IDC_POLLRATE, buf, MAX_PATH );
               inst->m_poll_rate = (int) (atof( buf ) * 1000.0f);

               GetDlgItemText( wnd, IDC_AUTOLOGFILE, buf, MAX_PATH );
               inst->m_log_file = buf;
               
               if (inst->m_poll_rate <= 0)
                  inst->m_poll_rate = POLL_RATE_DEFAULT;

               if (!inst->m_log_file.length( ))
                  inst->m_log_file = "autolog.txt";

               if (inst->m_buddy_list)
                  inst->m_buddy_list->setAlwaysOnTop( (inst->m_opt_flags & OPT_BUDDYLISTAOT) != 0 );

               inst->writeOptions( );
               inst->broadcastSync( );

               inst->m_opt_wnd = NULL;
               DestroyWindow( wnd );
               return TRUE;
            }
            break;

         case IDCANCEL:
            if (HIWORD( wparam ) == BN_CLICKED)
            {
               Eggy::singleton( )->m_opt_wnd = NULL;
               DestroyWindow( wnd );
               return TRUE;
            }
            break;
         }
      }
      break;

   case WM_CLOSE:
      Eggy::singleton( )->m_opt_wnd = NULL;
      DestroyWindow( wnd );
      return TRUE;
   }

   return FALSE;
}

INT_PTR CALLBACK Eggy::about_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam )
{
   switch (msg)
   {
   case WM_INITDIALOG:
      {
         SetDlgItemText( wnd, IDC_ABOUT_TEXT, 
                         "Eggy " EGGY_VERSION "\r\n"
                         "A buddy list and instant messenger module for Furnarchy 2.\r\n\r\n"
                         "Written by Cluracan\r\n"
                         "(strangelove@heroinpuppy.com)\r\n"
                         "www.heroinpuppy.com/forums" );
      }
      return TRUE;

   case WM_COMMAND:
      {
         switch (LOWORD( wparam ))
         {
         case IDOK:
            if (HIWORD( wparam ) == BN_CLICKED)
            {
               PostMessage( wnd, WM_CLOSE, 0, 0 );
               return TRUE;
            }
            break;
         }
      }
      break;

   case WM_CLOSE:
      Eggy::singleton( )->m_about_wnd = NULL;
      DestroyWindow( wnd );
      return TRUE;

   }

   return FALSE;
}

//////////////////////////////////////////////////////////////////////////

} // namespace eggy