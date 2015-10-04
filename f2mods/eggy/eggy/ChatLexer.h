#ifndef _CHATLEXER_H__
#define _CHATLEXER_H__

#include <stack>
#include <string>

class ChatLexer
{
   struct Tag;
   struct UrlGrammar;

public:
   enum { NUM_SMILEYS = 54, NUM_DESCTAGS = 101,
          SMILEYS_FIRST = 'A', DESCTAGS_FIRST = ' ' };
   enum Tok_Type { TOK_NONE = 0, TOK_TEXT, TOK_SMILEY, TOK_DESCTAG, TOK_IMG, TOK_EOF = -1 };
   enum { STYLE_BOLD = 0x1, STYLE_ITALIC = 0x2,
          STYLE_UNDERLINE = 0x4, STYLE_LINK = 0x8 };
   
   ChatLexer( const char* buf, std::size_t len );
   ~ChatLexer( );
   
   const char* getBuf( ) const;
   bool        eof( ) const;
   
   Tok_Type           next( );
   Tok_Type           tokType( ) const;
   size_t             tokOff( ) const;
   const char*        tokPtr( ) const;
   size_t             tokLen( ) const;
   int                tokStyle( ) const;
   const std::string& tokHref( ) const;
   const std::string& tokColor( ) const;
   const std::string& tokImgSrc( ) const;
   
private:
   ChatLexer( );
   
private:   
   const char* const m_buf;
   const std::size_t m_len;
   std::size_t       m_off;
   Tag*              m_tag;
   UrlGrammar*       m_urlgrammar;

   int                     m_bold;
   int                     m_italic;
   int                     m_underline;
   std::stack<std::string> m_href;
   std::stack<std::string> m_color;

   struct
   {
      Tok_Type    type;
      std::size_t off;
      std::size_t len;
      int         style;
      std::string href;
      std::string color;
      // Used by TOK_IMG
      std::string src;
   } m_last_token;
};

inline const char* ChatLexer::getBuf( ) const {
   return m_buf;
}

inline bool ChatLexer::eof( ) const {
   return m_off == m_len;
}

inline ChatLexer::Tok_Type ChatLexer::tokType( ) const {
   return m_last_token.type;
};

inline size_t ChatLexer::tokOff( ) const {
   return m_last_token.off;
}

inline const char* ChatLexer::tokPtr( ) const {
   return m_buf + m_last_token.off;
}

inline size_t ChatLexer::tokLen( ) const {
   return m_last_token.len;
}

inline int ChatLexer::tokStyle( ) const {
   return m_last_token.style;
}

inline const std::string& ChatLexer::tokHref( ) const {
   return m_last_token.href;
}

inline const std::string& ChatLexer::tokImgSrc( ) const {
   return m_last_token.src;
}

inline const std::string& ChatLexer::tokColor( ) const {
   return m_last_token.color;
}

#endif // _CHATLEXER_H__