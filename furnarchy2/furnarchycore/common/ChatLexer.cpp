#include "ChatLexer.h"
#include <string>
#include <list>
#include <stdexcept>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_chset.hpp>

using namespace std;

//////////////////////////////////////////////////////////////////////////

inline bool is_smiley( char c )
{
   return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'y') || (c >= '1' && c <= '3');
}

static int strncasecmp( const char* a, size_t m, 
                        const char* b, size_t n )
{
   if (m < n)
      return -1;
   if (m > n)
      return 1;
   
   for (; m; --m, ++a, ++b)
   {
      register char _a = *a;
      register const char _b = *b;
      
      if (_a != _b)
      {
         if (_a >= 'A' && _a <= 'Z')
         {
            _a ^= 0x20;
            if (_a < _b)
               return -1;
            else if (_a > _b)
               return 1;
         }
         else if ((unsigned) _a < (unsigned) _b)
            return -1;
         else if ((unsigned) _a > (unsigned) _b)
            return 1;
      }
   }
   
   return 0;
}

//////////////////////////////////////////////////////////////////////////

struct ChatLexer::Tag
{
   enum Tag_Type { TAG_BAD, TAG_START, TAG_END, TAG_EMPTY };
   struct ATTR 
   { 
      string name, value;
      ATTR( const string& name, const string& value ) : name( name ), value( value ) {}
   };
   
   string                    name;
   Tag_Type                  type;
   list<ATTR>                attrs;
   boost::spirit::classic::rule<>     start_r;
   boost::spirit::classic::subrule<0> tag_r;
   boost::spirit::classic::subrule<1> name_r;
   boost::spirit::classic::subrule<2> att_r;

   /* Actors for spirit parser. */
   struct ActorAttrName
   {
      mutable Tag& self;
      ActorAttrName( Tag& self ) : self( self ) {}
      void operator()( const char* beg, const char* end ) const
      {
         self.attrs.push_back( ATTR( string( beg, end ), "" ) );
      }
   };
   struct ActorAttrVal
   {
      mutable Tag& self;
      ActorAttrVal( Tag& self ) : self( self ) {}
      void operator()( const char* beg, const char* end ) const
      {
         self.attrs.back( ).value.assign( beg, end );
      }
   };
   struct ActorTagName
   {
      mutable Tag& self;
      ActorTagName( Tag& self ) : self( self ) {}
      void operator()( const char* beg, const char* end ) const
      {
         self.name.assign( beg, end );
      }
   };
   template <Tag_Type T>
   struct ActorTagType
   {
      mutable Tag& self;
      ActorTagType( Tag& self ) : self( self ) {}
      void operator()( const char* beg, const char* end ) const
      {
         self.type = T;
      }
   };

   /* Tag methods. */

   Tag( ) : type( TAG_BAD ) 
   { 
	   using namespace boost::spirit::classic;

      /* Build parser rules. */
      start_r = 
      (
         tag_r = '<' 
                 >> (
                     (name_r[ ActorTagName( *this ) ] >> *(space_p >> !att_r))[ ActorTagType<TAG_START>( *this ) ]
                     | ('/' >> name_r[ ActorTagName( *this ) ] >> *(space_p >> !att_r))[ ActorTagType<TAG_END>( *this ) ]
                     | (name_r[ ActorTagName( *this ) ] >> *(space_p >> !att_r) >> '/')[ ActorTagType<TAG_EMPTY>( *this ) ]
                    )
                 >> '>',

         name_r = (alpha_p | '_' | ':' ) 
                  >> *(alnum_p | '.' | '-' | '_' | ':'),

         att_r = name_r[ ActorAttrName( *this ) ]
                 >> !( '=' 
                       >> ( ('"' >> (*~ch_p( '"' ))[ ActorAttrVal( *this ) ]  >> '"') 
                            | ('\'' >> (*~ch_p( '\'' ))[ ActorAttrVal( *this ) ] >> '\'') ) 
                     )
      );
   }
   
   size_t parse( const char* beg, const char* end )
   {
	   using namespace boost::spirit::classic;
      clear( );
      
      parse_info<const char*> pi = boost::spirit::classic::parse( beg, end, start_r );
      if (!pi.hit)
      {
         type = TAG_BAD;
         return 0;
      }
      return pi.length;
   }
   bool isAttr( const char* name ) const
   {
      const size_t name_len = strlen( name );
      for (list<ATTR>::const_iterator i = attrs.begin( );
           i != attrs.end( ); ++i)
      {
         if (!strncasecmp( name, name_len, 
                           i->name.c_str( ), i->name.length( ) ))
            return true;
      }
      return false;
   }
   const string& getAttrVal( const char* name ) const
   {
      const size_t name_len = strlen( name );
      for (list<ATTR>::const_iterator i = attrs.begin( );
           i != attrs.end( ); ++i)
      {
         if (!strncasecmp( name, name_len, 
                           i->name.c_str( ), i->name.length( ) ))
            return i->value;
      }
      throw runtime_error( "Invalid attribute." );
   }
   void clear( )
   {
      name.clear( );
      type = TAG_BAD;
      attrs.clear( );
   }
   bool isName( const char* name ) const
   {
      return _stricmp( Tag::name.c_str( ), name ) == 0;
   }
};

struct ChatLexer::UrlGrammar
{
   boost::spirit::classic::rule<> start_r;

   UrlGrammar( )
   {
	   using namespace boost::spirit::classic;

      start_r = (str_p( "http://" ) | "www." | "furc://" | "https://" | "ftp://")
         >> +(~chset_p( "<>()^*\t " ));
   }
   size_t parse( const char* beg, const char* end )
   {
	   using namespace boost::spirit::classic;
      
	   parse_info<const char*> pi = boost::spirit::classic::parse( beg, end, start_r );
      if (pi.hit)
         return pi.length;
      return 0;
   }
};

//////////////////////////////////////////////////////////////////////////

ChatLexer::ChatLexer( const char* buf, size_t len )
   : m_buf( buf ), m_len( len )
{
   m_off = 0;
   
   m_bold      = 0;
   m_italic    = 0;
   m_underline = 0;
   
   m_last_token.type  = TOK_NONE;
   m_last_token.off   = 0;
   m_last_token.len   = 0;
   m_last_token.style = 0;

   m_tag = new Tag;
   m_urlgrammar = new UrlGrammar;
}

ChatLexer::~ChatLexer( )
{
   delete m_tag;
   delete m_urlgrammar;
}

ChatLexer::Tok_Type ChatLexer::next( )
{
   m_last_token.type = TOK_EOF;
   m_last_token.len  = 0;

   while (m_off < m_len)
   {
      // Tag?
      if (m_buf[ m_off ] == '<')
      {
         size_t n = m_tag->parse( m_buf + m_off, m_buf + m_len );
         if (n && m_tag->type != Tag::TAG_BAD)
         {
            assert( m_off + n <= m_len );

            if (m_tag->isName( "a" ))
            {
               if (m_tag->type == Tag::TAG_START)
               {
                  if (m_tag->isAttr( "href" ))
                     m_href.push( m_tag->getAttrVal( "href" ) );
               }
               else if (m_tag->type == Tag::TAG_END)
               {
                  if (!m_href.empty( ))
                     m_href.pop( );
               }

               m_off += n;
               if (m_last_token.len)
                  break;
               continue;
            }
            else if (m_tag->isName( "b" ))
            {
               if (m_tag->type == Tag::TAG_START)
                  ++m_bold;
               else if (m_tag->type == Tag::TAG_END)
                  m_bold ? --m_bold : m_bold;

               m_off += n;
               if (m_last_token.len)
                  break;
               continue;
            }
            else if (m_tag->isName( "i" ))
            {
               if (m_tag->type == Tag::TAG_START)
                  ++m_italic;
               else if (m_tag->type == Tag::TAG_END)
                  m_italic ? --m_italic : m_italic;
               
               m_off += n;
               if (m_last_token.len)
                  break;
               continue;
            }
            else if (m_tag->isName( "u" ))
            {
               if (m_tag->type == Tag::TAG_START)
                  ++m_underline;
               else if (m_tag->type == Tag::TAG_END)
                  m_underline ? --m_underline : m_underline;

               m_off += n;
               if (m_last_token.len)
                  break;
               continue;
            }
         }
      }
      // Icon?
      else if (m_buf[ m_off ] == '#')
      {
         if (m_off + 2 < m_len && 
             (m_buf[ m_off + 1 ] == 'S' && 
              is_smiley( m_buf[ m_off + 2 ] )) ||
             (m_buf[ m_off + 1 ] == 'C' &&
              (unsigned) (m_buf[ m_off + 2 ] - DESCTAGS_FIRST) < NUM_DESCTAGS))
         {
            if (m_last_token.len)
               break;

            m_last_token.type = m_buf[ m_off + 1 ] == 'S' ? TOK_SMILEY : TOK_DESCTAG;
            m_last_token.off  = m_off;
            m_last_token.len  = 3;
            m_last_token.href  = m_href.empty( ) ? "" : m_href.top( );
            m_last_token.style = (m_bold ? STYLE_BOLD : 0) | 
                                 (m_italic ? STYLE_ITALIC : 0) |
                                 (m_underline ? STYLE_UNDERLINE : 0) |
                                 (m_href.empty( ) ? 0 : STYLE_LINK);
            m_off += 3;
            break;
         }
      }
      // URL?
      else if (m_buf[ m_off ] == 'h' || m_buf[ m_off ] == 'w' || m_buf[ m_off ] == 'f')
      {
         size_t n = m_urlgrammar->parse( m_buf + m_off, m_buf + m_len );
         if (n)
         {
            if (m_last_token.len)
               break; // Let it get picked up next time around.

            m_last_token.type = TOK_TEXT;
            m_last_token.off  = m_off;
            m_last_token.len  = n;
            m_last_token.href.assign( m_buf + m_off, m_buf + m_off + n );
            m_last_token.style = (m_bold ? STYLE_BOLD : 0) | 
                                 (m_italic ? STYLE_ITALIC : 0) |
                                 (m_underline ? STYLE_UNDERLINE : 0) |
                                 STYLE_LINK;
            m_off += n;
            break;
         }
      }
      
      // Text.
      if (!m_last_token.len)
      {
         m_last_token.type = TOK_TEXT;
         m_last_token.off  = m_off;
         m_last_token.href  = m_href.empty( ) ? "" : m_href.top( );
         m_last_token.style = (m_bold ? STYLE_BOLD : 0) | 
                              (m_italic ? STYLE_ITALIC : 0) |
                              (m_underline ? STYLE_UNDERLINE : 0) |
                              (m_href.empty( ) ? 0 : STYLE_LINK);
      }
      ++m_last_token.len;
      ++m_off;
   }
   
   return m_last_token.type;
}