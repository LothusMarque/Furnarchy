#include "world.h"
#include "core.h"
#include "net.h"
#include "client.h"
#include "common/xml_charset.h"
#include "shortname.h"
#include <cmath>
#include <queue>

namespace world {

//////////////////////////////////////////////////////////////////////////

enum { MASS_DEFAULT = 90, MASS_SPEECH = 1000, 
       MASS_CRITICAL = 1600, MASS_DECAYPS = 800 };

struct DESCTAG
{
   int    id;
   string tag;
   DESCTAG( ) : id( 0 ) { }
   DESCTAG( int id ) : id( id ) { }
};

//////////////////////////////////////////////////////////////////////////

bool           g_initialized = false;
PLAYERINFO     g_player;
list<string>   g_recent_names;
list<DESCTAG>  g_desc_tags;
string         g_last_badge;
queue<string>  g_speech;
unsigned int   g_mass;
bool           g_limiting;
yiffurc::NET_SESSION g_session;
list<MONSTERINFO>    g_monsters;

//////////////////////////////////////////////////////////////////////////

string make_desc( );

//////////////////////////////////////////////////////////////////////////

bool initialize( )
{
 if (!g_initialized)
 {
    g_initialized = true;
    g_player.clear( );
    g_monsters.clear( );
    g_recent_names.clear( );
    g_desc_tags.clear( );
    g_last_badge.clear( );
    while (!g_speech.empty( )) g_speech.pop( );
    g_mass = 0;
    g_limiting = true;
    memset( &g_session, 0, sizeof( g_session ) );
 }
 return true;
}

bool uninitialize( )
{
 if (g_initialized)
 {
    g_initialized = false;
    g_player.clear( );
    g_monsters.clear( );
    g_recent_names.clear( );
    g_desc_tags.clear( );
    g_last_badge.clear( );
    while (!g_speech.empty( )) g_speech.pop( );
    g_mass = 0;
    g_limiting = true;
    memset( &g_session, 0, sizeof( g_session ) );
 }
 return true;
}

long on_inbound_line( string* line, int from, size_t num )
{
   using namespace yiffurc;

   long hide_flags = 0;

   const S2C* parsed = s2c_parse( line->c_str( ), line->length( ), &g_session );
   if (!parsed)
       return hide_flags;

   switch (parsed->type)
   {
   case S2C_TYPE_LOGINFAIL:
      g_player.clear( );
      core::event_player_info( );
      break;

   case S2C_TYPE_CAMSET:
      g_player.cam.x = ((S2C_CAMSET*) parsed)->x;
      g_player.cam.y = ((S2C_CAMSET*) parsed)->y;
      core::event_player_info( );
      break;

   case S2C_TYPE_CAMTWEEN:
      g_player.cam.x = ((S2C_CAMTWEEN*) parsed)->x;
      g_player.cam.y = ((S2C_CAMTWEEN*) parsed)->y;
      core::event_player_info( );
      break;       

   case S2C_TYPE_SYNCCOLORCODE:
      {
         string colors = ((S2C_SYNCCOLORCODE*) parsed)->colors;
         colors.append( ((S2C_SYNCCOLORCODE*) parsed)->gsd, 3 );
         if (colors != g_player.colors)
         {
            g_player.colors = colors;
            client::set_player_info( NULL, NULL, colors.c_str( ), NULL, NULL );
         }
      }
      break;

   case S2C_TYPE_AVCREATE:
      {
         S2C_AVCREATE* avcreate = (S2C_AVCREATE*) parsed;

         if (monster_find( avcreate->uid )) // Already exists.
         {
            POINT2 pos = { avcreate->x, avcreate->y };
            unsigned flags = avcreate->flags;
            monster_update( avcreate->uid, avcreate->name, avcreate->colors, &avcreate->frame, &pos, NULL, flags, -1 );
         }
         else // Create a monster.
         {
            MONSTERINFO monst;
            monst.guid  = avcreate->uid;
            monst.name  = avcreate->name;
            monst.frame = avcreate->frame;
            monst.color = avcreate->colors;
            monst.pos.x = avcreate->x;
            monst.pos.y = avcreate->y;
            monst.flags = avcreate->flags;
            monst.visible = false;
            g_monsters.push_back( monst );

            core::event_monster( monst.guid );
         }

         // If it has the same name as the player and we don't have a guid yet, use it.
         if (!g_player.guid && g_player.name == avcreate->name)
            g_player.guid = avcreate->uid;
      }
      break;

   case S2C_TYPE_AVTWEEN:
      {
         S2C_AVTWEEN* avtween = (S2C_AVTWEEN*) parsed;

         if (monster_find( avtween->uid ))
         {
            POINT2 pos = { avtween->x, avtween->y };
            monster_update( avtween->uid, NULL, NULL, &avtween->frame, &pos, NULL, 0, 0 );
         }
      }
      break;

   case S2C_TYPE_AVUPDATECOLORS:
      {
         S2C_AVUPDATECOLORS* avupdatecolors = (S2C_AVUPDATECOLORS*) parsed;

         if (monster_find( avupdatecolors->uid ))
         {
            monster_update( avupdatecolors->uid, NULL, avupdatecolors->colors, &avupdatecolors->frame, NULL, NULL, 0, 0 );
         }
      }
      break;

   case S2C_TYPE_AVSHOW:
      {
         S2C_AVSHOW* avshow = (S2C_AVSHOW*) parsed;

         if (monster_find( avshow->uid ))
         {
            POINT2 pos = { avshow->x, avshow->y };
            bool visible = true;
            monster_update( avshow->uid, NULL, NULL, &avshow->frame, &pos, &visible, 0, 0 );
         }
      }
      break;

   case S2C_TYPE_AVSHOW2:
      {
         S2C_AVSHOW2* avshow = (S2C_AVSHOW2*) parsed;

         if (monster_find( avshow->uid ))
         {
            POINT2 pos = { avshow->x, avshow->y };
            bool visible = true;
            monster_update( avshow->uid, NULL, NULL, &avshow->frame, &pos, &visible, 0, 0 );
         }
      }
      break;

   case S2C_TYPE_AVHIDE:
      {
         S2C_AVHIDE* avhide = (S2C_AVHIDE*) parsed;

         if (monster_find( avhide->uid ))
         {
            POINT2 pos = { avhide->x, avhide->y };
            bool visible = false;
            monster_update( avhide->uid, NULL, NULL, NULL, &pos, &visible, 0, 0 );
         }
      }
      break;

   case S2C_TYPE_AVDESTROY:
      {
         S2C_AVDESTROY* avdestroy = (S2C_AVDESTROY*) parsed;
         monster_remove( avdestroy->uid );
      }
      break;

   case S2C_TYPE_FETCHDREAM:
   case S2C_TYPE_USEMAPFILE:
      {
		 //Not being nuked so it throws monster change events for each one.
         while (!g_monsters.empty( ))
            monster_remove( g_monsters.front( ).guid ); 
		//Temp. setting to nuke for testing.
		//g_monsters.clear();
      }
      break;

   case S2C_TYPE_SPECITAG:
   case S2C_TYPE_BADGE:
      // Buffer badges until some kind of chatter comes in to claim it.
      g_last_badge = *line;
      hide_flags |= HIDE_CLIENT;
      break;

   case S2C_TYPE_DESCHEAD:
      name_push( ((S2C_DESCHEAD*) parsed)->player );
      break;

   case S2C_TYPE_YOUSAY:
   case S2C_TYPE_CHAT:
      // Claim badge.
      net::finalize_inbound_line( g_last_badge ); g_last_badge.clear( );
      break;

   case S2C_TYPE_SPEECH:
      // Claim badge.
      net::finalize_inbound_line( g_last_badge ); g_last_badge.clear( );
      name_push( ((S2C_SPEECH*) parsed)->from );
      break;

   case S2C_TYPE_EMOTE:
      // Claim badge.
      net::finalize_inbound_line( g_last_badge ); g_last_badge.clear( );
      {         
         string name = ((S2C_EMOTE*) parsed)->from;
         /* Strip common suffixes. */
         if (name.length( ) >= 2 && !name.compare( name.length( ) - 2, 2, "'s" ))
            name.resize( name.length( ) - 2 );
         else if (name.length( ) >= 1)
         {
            char ch_last = tolower( name[ name.length( ) - 1 ] );

            if (!((ch_last >= 'a' && ch_last <= 'z') || 
                (ch_last >= '0' && ch_last <= '9') || ch_last == ';'))
               name.resize( name.length( ) - 1 );
         }

         name_push( name );
      }
      break;

   case S2C_TYPE_WHISPER:
      // Claim badge.
      net::finalize_inbound_line( g_last_badge ); g_last_badge.clear( );
      name_push( ((S2C_WHISPER*) parsed)->from );
      break;

   case S2C_TYPE_SHOUT:
      // Claim badge.
      net::finalize_inbound_line( g_last_badge ); g_last_badge.clear( );
      name_push( ((S2C_SHOUT*) parsed)->from );
      break;

   case S2C_TYPE_JOIN:
      name_push( ((S2C_JOIN*) parsed)->from );
      break;

   case S2C_TYPE_SUMMON:
      name_push( ((S2C_SUMMON*) parsed)->from );
      break;
   }

   yiffurc_release( parsed );

   // Update network level changes.
   switch (g_session.level)
   {
   case NET_LEVEL_MOTDEND:
      if (core::get_connlvl( ) < CONNLVL_MOTDEND)
         core::event_motdend( );
      break;

   case NET_LEVEL_LOGGEDIN:
      if (core::get_connlvl( ) < CONNLVL_LOGGEDIN)
         core::event_loggedin( );
      break;
   }


   return hide_flags;
}

long on_outbound_line( string* line, int from, size_t num )
{
   using namespace yiffurc;

   // Increase mass.
   if (line->length( ))
      g_mass += MASS_DEFAULT;

   long hide_flags = 0;

   const C2S* parsed = c2s_parse( line->c_str( ), line->length( ), &g_session );
   if (!parsed)
      return hide_flags;

   switch (parsed->type)
   {
   case C2S_TYPE_LOGIN:
      g_player.name   = ((C2S_LOGIN*) parsed)->player;
      g_player.pw     = ((C2S_LOGIN*) parsed)->password;
      g_player.mac_id = ((C2S_LOGIN*) parsed)->mac_id;
      core::event_player_info( );
      break;

   case C2S_TYPE_SETDESC:
      g_player.desc = ((C2S_SETDESC*) parsed)->desc;

      /* Modify outgoing descs with the desc tags appended. */
      /* Also fix non-ascii characters. */
      {
         const char* cmd = c2s_enc_setdesc( make_desc( ).c_str( ) );
         line->assign( cmd, strlen( cmd ) - 1 );
         yiffurc_release( cmd );
      }
      core::event_player_info( );
      break;

   case C2S_TYPE_CHANGEDESC:
      g_player.desc = ((C2S_CHANGEDESC*) parsed)->desc;

      /* Modify outgoing descs with the desc tags appended. */
      /* Also fix non-ascii characters. */
      {
         const char* cmd = c2s_enc_changedesc( make_desc( ).c_str( ) );
         line->assign( cmd, strlen( cmd ) - 1 );
         yiffurc_release( cmd );
      }
      core::event_player_info( );
      break;

   case C2S_TYPE_SPEECH:
   case C2S_TYPE_SHOUT:
   case C2S_TYPE_EMOTE:
      {
         const char* cmd = NULL;
      
         /* Fix non-ascii characters. */
         switch (parsed->type)
         {
         case C2S_TYPE_SPEECH:
            cmd = c2s_enc_speech( xml_charset::latin_to_ascii( ((C2S_SPEECH*) parsed)->message ).c_str( ) );
            break;
         case C2S_TYPE_SHOUT:
            cmd = c2s_enc_shout( xml_charset::latin_to_ascii( ((C2S_SHOUT*) parsed)->message ).c_str( ) );
            break;
         case C2S_TYPE_EMOTE:
            cmd = c2s_enc_emote( xml_charset::latin_to_ascii( ((C2S_EMOTE*) parsed)->message ).c_str( ) );
            break;
         }

         /* Buffer chatter. */
         g_speech.push( cmd );
         yiffurc_release( cmd );
         g_mass -= MASS_DEFAULT;
         hide_flags |= HIDE_CLIENT;
         // Try to send what we can now.
         on_tick( 0 );
      }
      break;

   case C2S_TYPE_WHISPER:
      {
         /* Fix non-ascii characters. */
         const char* cmd = NULL;
         if (((C2S_WHISPER*) parsed)->exact)
            cmd = c2s_enc_whisper( ("%" + string( ((C2S_WHISPER*) parsed)->player )).c_str( ),  
                                   xml_charset::latin_to_ascii( ((C2S_WHISPER*) parsed)->message ).c_str( ) );
         else
            cmd = c2s_enc_whisper( ((C2S_WHISPER*) parsed)->player, 
                                   xml_charset::latin_to_ascii( ((C2S_WHISPER*) parsed)->message ).c_str( ) );

         line->assign( cmd, strlen( cmd ) - 1 );
         yiffurc_release( cmd );
      }
      break;
   }

   yiffurc_release( parsed );

   // Update network level changes.
   switch (g_session.level)
   {
   case NET_LEVEL_MOTDEND:
      if (core::get_connlvl( ) < CONNLVL_MOTDEND)
         core::event_motdend( );
      break;

   case NET_LEVEL_LOGGEDIN:
      if (core::get_connlvl( ) < CONNLVL_LOGGEDIN)
         core::event_loggedin( );
      break;
   }

   return hide_flags;
}

bool on_command( const ArgList& args )
{
   if (args.size( ) && !_strnicmp( args[ 0 ].c_str( ), "speedlimit", 10 ))
   {
      if (args.size( ) > 1)
      {
         if (!_strnicmp( args[ 1 ].c_str( ), "on", 2 ))
         {
            g_limiting = true;
            core::system_speak( "Outbound speed limit enabled." );
         }
         else if (!_strnicmp( args[ 1 ].c_str( ), "off", 3 ))
         {
            g_limiting = false;
            core::system_speak( "Outbound speed limit disabled." );
         }
      }
      core::system_errspeak( "Usage: speedlimit on|off" );
      return true;
   }
   return false;
}

void name_push( const string& name )
{
   if (name.length( ))
   {
      // See if it's already in the list.
      list<string>::iterator i = g_recent_names.begin( );
      for (; i != g_recent_names.end( ); ++i)
      {
         if (!_stricmp( i->c_str( ), name.c_str( ) ))
            break;
      }

      if (i == g_recent_names.end( )) // Not in the list.
      {
         if (g_recent_names.size( ) >= MAX_RECENT_NAMES)
         {
            // List is full, so pop off the oldest chatter.
            g_recent_names.pop_back( );
         }
      }
      else // In the list already.
      {
         if (i == g_recent_names.begin( ))
         {
            // Already in the front so just update the case.
            *i = name;
            return;
         }

         // Erase.
         g_recent_names.erase( i );
      }

      // Add to the front.
      g_recent_names.push_front( name );
   }
}

int name_complete( string* name, int iter /* = 0 */ )
{
   const size_t _min = (size_t) iter <= g_recent_names.size( ) ? iter : 0;
   size_t pos = 0;
   for (list<string>::const_iterator i = g_recent_names.begin( );
        i != g_recent_names.end( ); ++i, ++pos)
   {
      if (pos >= _min)
      {
         if (!_strnicmp( name->c_str( ), i->c_str( ), name->length( ) ))
         {
            name->assign( *i );
            return pos + 1;
         }
      }
   }

   return -1;
}

const PLAYERINFO& get_player( ) {
   return g_player;
}

int desctag_reg( )
{
   while (true)
   {
      int tag_id = rand( ) + 1;

      list<DESCTAG>::iterator i = g_desc_tags.begin( );
      for (; i != g_desc_tags.end( ); ++i)
      {
         if (i->id == tag_id) // Already registered.
            break;
      }

      if (i == g_desc_tags.end( ))
      {
         g_desc_tags.push_back( DESCTAG( tag_id ) );
         return tag_id;
      }
   }

   return 0;
}

bool desctag_unreg( int tag_id )
{
   using namespace yiffurc;

   for (list<DESCTAG>::iterator i = g_desc_tags.begin( );
        i != g_desc_tags.end( ); ++i)
   {
      if (i->id == tag_id)
      {
         size_t tag_len = i->tag.length( );
         g_desc_tags.erase( i );

         if (tag_len)
         {
            // Update immediately.
            const char* cmd = c2s_enc_setdesc( make_desc( ).c_str( ) );
            net::finalize_outbound_line( string( cmd, strlen( cmd ) - 1 ) );
            yiffurc_release( cmd );
         }
         return true;
      }
   }
   return false;
}

bool desctag_set( int tag_id, const char* tag )
{
   using namespace yiffurc;

   for (list<DESCTAG>::iterator i = g_desc_tags.begin( );
        i != g_desc_tags.end( ); ++i)
   {
      if (i->id == tag_id)
      {
         string str_tag = tag ? tag : "";
         strip_lines( &str_tag );

         if (i->tag != str_tag)
         {
            i->tag = str_tag;

            // Update immediately.
            const char* cmd = c2s_enc_setdesc( make_desc( ).c_str( ) );
            net::finalize_outbound_line( string( cmd, strlen( cmd ) - 1 ) );
            yiffurc_release( cmd );
         }
         return true;
      }
   }
   return false;
}

string make_desc( )
{
   string desc = g_player.desc;
   for (list<DESCTAG>::const_iterator i = g_desc_tags.begin( );
        i != g_desc_tags.end( ); ++i)
   {
      if (i->tag.length( ))
      {
         desc += " ";
         desc += i->tag;
      }
   }

   return xml_charset::latin_to_ascii( desc );
}

void on_tick( unsigned int dt )
{
   /* Send buffered speech. */
   unsigned int decay = (unsigned int) ceil( dt * MASS_DECAYPS / 1000.0f );

   if (decay > g_mass)
      g_mass = 0;
   else
      g_mass -= decay;

   if (!g_limiting)
   {
      /* just send everything right away */
      while (!g_speech.empty( ))
      {
         net::finalize_outbound_line( g_speech.front( ) );
         g_speech.pop( );
      }
   }
   else
   {
      /* Only send a speech line if the mass will be under the limit. */
      while (!g_speech.empty( ) && g_mass + MASS_SPEECH <= MASS_CRITICAL)
      {
         g_mass += MASS_SPEECH;
         net::finalize_outbound_line( g_speech.front( ) );
         g_speech.pop( );
      }
   }
}

void on_connlvl( Connection_Level lvl )
{
   using namespace yiffurc;

   switch (lvl)
   {
   case CONNLVL_CONNECTED:
      memset( &g_session, 0, sizeof( g_session ) );
      g_session.level = NET_LEVEL_CONNECTED;
      break;
   case CONNLVL_LOGGEDIN: // We probably sent this.
      g_session.level = NET_LEVEL_LOGGEDIN;
      break;
   case CONNLVL_MOTDEND: // We probably sent this.
      g_session.level = NET_LEVEL_MOTDEND;
      break;
   case CONNLVL_DISCONNECTED:
      g_player.clear( );
      g_recent_names.clear( );
      // g_desc_tags.clear( );
      g_last_badge.clear( );
      g_monsters.clear( );
      while (!g_speech.empty( )) g_speech.pop( );
      g_mass = 0;
      memset( &g_session, 0, sizeof( g_session ) );
      break;
   }
}

const yiffurc::S2C* parse_inbound( const string& line )
{
   using namespace yiffurc;
   if (core::get_connlvl( ) >= CONNLVL_CONNECTED)
      return s2c_parse_peek( line.c_str( ), line.length( ), &g_session );
   return NULL;
}

const yiffurc::C2S* parse_outbound( const string& line )
{
   using namespace yiffurc;
   if (core::get_connlvl( ) >= CONNLVL_CONNECTED)
      return c2s_parse_peek( line.c_str( ), line.length( ), &g_session );
   return NULL;
}

void badge_cancel( ) {
   g_last_badge.clear( );
}

void set_player_info( const char* name, const char* pw, 
                      const char* colors, const char* desc,
                      const unsigned int* guid, const POINT2* cam )
{
   if (name && g_player.name != name)
      g_player.name = name;
   if (pw && g_player.pw != pw)
      g_player.pw = pw;
   if (colors && g_player.colors != colors)
      g_player.colors = colors;
   if (desc && g_player.desc != desc)
   {
      g_player.desc = desc;

      if (core::get_connlvl( ) >= CONNLVL_DISCONNECTED)
      {
         // Update immediately.
         const char* cmd = yiffurc::c2s_enc_setdesc( make_desc( ).c_str( ) );
         net::finalize_outbound_line( string( cmd, strlen( cmd ) - 1 ) );
         yiffurc::yiffurc_release( cmd );
      }
   }
   if (guid && g_player.guid != *guid)
      g_player.guid = *guid;
   if (cam && g_player.cam != *cam)
      g_player.cam = *cam;

   core::event_player_info( );
}

const MONSTERINFO* monster_find( const string& name, bool by_shortname /* = false */ )
{
   if (by_shortname)
   {
      string _name = shortname::make_shortname( name );
      for (list<MONSTERINFO>::const_iterator i = g_monsters.begin( );
         i != g_monsters.end( ); ++i)
      {
         if (shortname::make_shortname( i->name ) == _name)
            // Found it.
            return &*i;
      }
   }
   else
   {
      for (list<MONSTERINFO>::const_iterator i = g_monsters.begin( );
           i != g_monsters.end( ); ++i)
      {
         if (i->name == name)
            // Found it.
            return &*i;
      }
   }
   return NULL;
}

const MONSTERINFO* monster_find( unsigned guid )
{
   for (list<MONSTERINFO>::const_iterator i = g_monsters.begin( );
        i != g_monsters.end( ); ++i)
   {
      if (i->guid == guid)
         // Found it.
         return &*i;
   }
   return NULL;
}

void monster_remove( unsigned guid ) 
{
   for (list<MONSTERINFO>::iterator i = g_monsters.begin( );
        i != g_monsters.end( ); ++i)
   {
      if (i->guid == guid)
      {
         // Remove it.
         g_monsters.erase( i );
         core::event_monster( guid );
         return;
      }
   }
}

void monster_update( unsigned guid, const char* name, const char* colors, 
                     const unsigned* frame, const POINT2* pos, 
                     bool* visible, unsigned flags, unsigned flags_mask )
{
   for (list<MONSTERINFO>::iterator i = g_monsters.begin( );
        i != g_monsters.end( ); ++i)
   {
      if (i->guid == guid)
      {
         bool changed = false;

         // Update in place.
         if (name && i->name != name)
            i->name = name, changed = true;
         if (colors && i->color != colors)
            i->color = colors, changed = true;
         if (frame && i->frame != *frame)
            i->frame = *frame, changed = true;
         if (pos && i->pos != *pos)
            i->pos = *pos, changed = true;
         if (visible && i->visible != *visible)
            i->visible = *visible, changed = true;
         if ((i->flags & flags_mask) != (flags & flags_mask))
            i->flags = (i->flags & ~flags_mask) | (flags & flags_mask), changed = true;

         if (changed)
            core::event_monster( guid );
         return;
      }
   }
}

void monster_enum( vector<unsigned>* guids )
{
   guids->clear( );
   for (list<MONSTERINFO>::const_iterator i = g_monsters.begin( );
        i != g_monsters.end( ); ++i)
   {
      guids->push_back( i->guid );
   }
}

} // namespace world
