#include "eggy.h"
#include "ChatLexer.h"

namespace eggy {

static string make_desc( const string& desc );

//////////////////////////////////////////////////////////////////////////

Contact::Contact( const string& name ) 
   : m_name( name ), m_refcount( 0 ), 
     m_freezecount( 0 ), m_online( false ), m_nearby( false ), m_lastseen( 0 ),
     m_buddy( false ), m_guid( 0 )
{ 
   assert( Eggy::singleton( ) );

   /* Replace pipes with spaces. */
   for (size_t i = 0; i < m_name.length( ); ++i)
      if (m_name[ i ] == '|')
         m_name[ i ] = ' ';

   /* Create the shortname. */
   m_shortname = make_shortname( m_name );
}

void Contact::release( )
{
   if (m_refcount-- == 1)
      Eggy::singleton( )->contactDestroy( this );
}

bool Contact::adjustName( const string& name )
{
   /* Make sure that the adjustment doesn't alter the shortname. */
   if (!(*this == name))
      return false;

   if (m_name != name)
   {
      m_name = name;

      // Generate an event.
      addRef( );
      Eggy::singleton( )->onContact( m_name, CONTACT_NAME );
      release( );
   }

   return true;
}

void Contact::setLastSeen( time_t t )
{
   if (m_lastseen != t)
   {
      m_lastseen = t;
      // Generate an event.
      // Eggy::singleton( )->onContact( m_name, CONTACT_LASTSEEN );
   }
}

void Contact::setGuid( unsigned guid )
{
   if (m_guid != guid)
   {
      m_guid = guid;
   }
}

void Contact::setDesc( const string& desc )
{
   addRef( );
   if (m_desc != desc)
   {
      m_desc = make_desc( desc );
      // Generate an event.
      Eggy::singleton( )->onContact( m_name, CONTACT_DESC );
   }
   release( );
}

void Contact::setOnline( bool online )
{
   addRef( );
   if (m_online != online)
   {
      m_online = online;
      if (!online)
         setNearby( false );
      // Generate an event.
      Eggy::singleton( )->onContact( m_name, CONTACT_ONLINE );
   }

   if (online)
      m_lastseen = time( NULL );
   release( );
}

void Contact::setNearby( bool nearby )
{
   addRef( );
   if (m_nearby != nearby)
   {
      m_nearby = nearby;
      if (nearby)
         setOnline( true );
      // Generate an event.
      Eggy::singleton( )->onContact( m_name, CONTACT_NEARBY );
   }
   release( );
}

void Contact::setBuddy(bool yesno )
{
   addRef( );
   if (m_buddy != yesno)
   {
      m_buddy = yesno;
      // Generate an event.
      Eggy::singleton( )->onContact( m_name, CONTACT_BUDDY );
   }
   release( );
}

string Contact::makeTip( ) const
{
   ostringstream ss;
   ss << "Name: " << pipes_to_spaces( m_name );

   if (m_lastseen)
   {
      time_t t = time( NULL );
      if (t - m_lastseen > 86400)
         ss << "\nLast seen " << (t - m_lastseen) / 86400 << " days ago.";
      else if (t - m_lastseen > 3600)
         ss << "\nLast seen " << (t - m_lastseen) / 3600 << " hours ago.";
      else if (t - m_lastseen > 300)
         ss << "\nLast seen " << (t - m_lastseen) / 60 << " minutes ago.";
   }

   if (m_desc.length( ))
   {
      ss << "\nDescription:\n" << m_desc;
   }

   return ss.str( );
}

//////////////////////////////////////////////////////////////////////////

string make_desc( const string& desc )
{
   // Strip smileys and tags, condense whitespace, break up
   // words that are too long.
   string out;
   ChatLexer lex( desc.c_str( ), desc.length( ) );
   while (!lex.eof( ))
   {
      if (lex.next( ) == lex.TOK_TEXT)
      {
         const char* c_end = lex.tokPtr( ) + lex.tokLen( );
         for (const char* c = lex.tokPtr( ); c < c_end;)
         {
            if (isspace( (unsigned char) *c ))
            {
               // Condense spaces.
               out += ' ';
               for (; c < c_end && isspace( (unsigned char) *c ); ++c) ;
            }
            else
            {
               // Eat img tags.
               if (_strnicmp( "<img ", c, 5 ) == 0)
               {
                  const char* p = c;
                  for (; p < c_end && *p != '>'; ++p) ;
                  if (!*p)
                     out += '<';
                  else
                     c = p + 1;
               }
               // Break up long words.
               for (size_t len = 0; len < 32 && c < c_end && !isspace( (unsigned char) *c ); ++c)
                  out += *c;
               if (c < c_end && !isspace( (unsigned char) *c ))
                  out += '-';
            }
         }
      }
   }

   return out;
}

} // namespace eggy