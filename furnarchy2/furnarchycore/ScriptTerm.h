#ifndef _SCRIPT_TERM_H__
#define _SCRIPT_TERM_H__

#include "common.h"
#include "common/PoorTextBox.h"
#include <CommCtrl.h>

class ScriptInst;

class ScriptTerm
{
private:
   struct Link
   {
      int    id;
      string url;
   };

public:
   struct StyleColor
   {
      int      style;
      COLORREF color;
      StyleColor( int style = 0, COLORREF color = 0 ) : style( style ), color( color ) {}
   };

public:
   enum { STYLE_NORMAL = 0x00, STYLE_BOLD = 0x01, 
          STYLE_ITALIC = 0x02, STYLE_UNDERLINE = 0x04 };
   enum { MAX_HISTORY = 32 };
   enum { NUM_SMILIES = 54 };

   enum Text_Type { TEXT_OUTPUT = 0, TEXT_LINK, TEXT_SUCCESS, TEXT_WARNING, TEXT_ERROR,
                    TEXT_CHANNEL, TEXT_WHISPER, TEXT_EMOTE, TEXT_EMIT, TEXT_MYSPEECH,
                    TEXT_BCAST, TEXT_ROLL, TEXT_SHOUT, TEXT_TRADE, TEXT_DRAGONSPEAK,
                    TEXT_NOTIFY, TEXT_QUERY, TEXT_HIGHLIGHT, TEXT_LOG, TEXT_HISTORY,
                    TEXT_FURCLINK, TEXT_CMDLINK, TEXT_NAME, TEXT_INPUT, TEXT_F2CMDLINK, 
                    NUM_TEXT_TYPES };

   static void init( HINSTANCE inst );
   static void uninit( );

public:
   ScriptTerm( ScriptInst* owner = NULL, HWND parent_wnd = NULL );
   ~ScriptTerm( );

public:
   void flash( );
   void raise( );
   void clear( );
   /** Writes raw text to terminal output
   *  If either \a style or \a color are non-NULL, they will override the default. */
   void write( const string& line, const int* style = NULL, const COLORREF* color = NULL );
   /** Writes text to terminal output
   *  Markup, entity names, smilies, and XML elementsare automatically interpolated.
   *  If either \a style or \a color are non-NULL, they will override the default. */
   void print( const string& line, const int* style = NULL, const COLORREF* color = NULL );

   void destroy( );
   bool isDestroyed( ) const;

   void      setUserData( uintptr_t data );
   uintptr_t getUserData( ) const;
   void setTitle( const string& title );
   const string& getTitle( ) const;
   void setFont( const string& name, unsigned height = 12, unsigned width = 0 );
   const string& getFont( unsigned* height = NULL, unsigned* width = NULL ) const;
   void setTextType( Text_Type type, const StyleColor& sc );
   StyleColor getTextType( Text_Type type ) const;
   void setBgColors( COLORREF input, COLORREF output );
   void getBgColors( COLORREF* input = NULL, COLORREF* output = NULL ) const;

private:
   void fitControls( );
   string getInputString( ) const;
   void handleInput( );
   void clearInput( );
   void cycleHistory( );
   void pushHistory( const string& str );
   int  addLink( const string& url );
   const string* findLink( int id ) const;
   void launchUrl( const string& url );
   void copyUrl( const string& url );
   void trimOutput( );

   static LRESULT CALLBACK wnd_proc( HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam );

private:
   static HINSTANCE        s_hinstance;
   static unsigned         s_init_count;
   static HIMAGELIST       s_smileys[ NUM_SMILIES ];
   static HMENU            s_link_menu;
   static const StyleColor s_default_types[ NUM_TEXT_TYPES ];

   ScriptInst* m_owner;
   int         m_user_data;
   HWND        m_parent_wnd;
   HWND        m_wnd;

   string      m_title;
   HFONT       m_font;
   string      m_font_name;
   unsigned    m_font_height;
   unsigned    m_font_width;
   COLORREF    m_input_bgcolor;
   COLORREF    m_output_bgcolor;
   StyleColor  m_types[ NUM_TEXT_TYPES ];

   list<string> m_history;
   string       m_history_working_str;

   list<Link>   m_links;

   time_t       m_last_trimmed;
};

//////////////////////////////////////////////////////////////////////////

inline bool ScriptTerm::isDestroyed( ) const { return m_wnd == NULL; }

inline void ScriptTerm::setUserData( uintptr_t data ) { m_user_data = data; }
inline uintptr_t ScriptTerm::getUserData( ) const { return m_user_data; }

inline const string& ScriptTerm::getTitle( ) const { return m_title; }

inline const string& ScriptTerm::getFont( unsigned* height /* = NULL */, unsigned* width /* = NULL */ ) const
{
   if (height) *height = m_font_height;
   if (width) *width = m_font_width;
   return m_font_name;
}

inline ScriptTerm::StyleColor ScriptTerm::getTextType( Text_Type type ) const
{
   return m_types[ type ];
}

inline void ScriptTerm::getBgColors( COLORREF* input /* = NULL */, COLORREF* output /* = NULL  */ ) const
{
   if (input) *input = m_input_bgcolor;
   if (output) *output = m_output_bgcolor;
}

#endif