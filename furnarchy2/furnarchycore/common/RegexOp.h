#ifndef _REGEXOP_H__
#define _REGEXOP_H__

#include <string>

class RegexOp
{
public:
   enum { MAX_CAPTURES = 9 };
   enum Type { TYPE_BAD, TYPE_SUBSTITUTE, TYPE_MATCH };
   enum { MANIP_INSENSITIVE = 0x1, MANIP_GLOBAL = 0x2, 
          MANIP_UNGREEDY = 0x4, MANIP_INVERT = 0x8 };
private:
   struct CAP
   {
      unsigned int start;
      unsigned int end;
   };

private:
   RegexOp( ) { };
public:
   static RegexOp* from_str( const std::string& s, const std::string** err = 0 );
   RegexOp( Type type, const std::string& re, const std::string& action, 
            long manips );
   ~RegexOp( );

   inline const std::string& getLastError( ) const;
   inline Type               getType( ) const;
   inline bool               ok( ) const;
   inline unsigned int       getNumSubstrs( ) const;
   inline bool               getSubstr( const char* src,
                                        unsigned int index, 
                                        std::string* out ) const;

   /** Runs the operation on a string.
   * The result parameter will be ignored unless the type is OP_SUBSTITUTE.
   * If used, the result parameter gets appended to, not overwritten.
   * \return true on match. */
   bool execute( const char* src, std::size_t len, std::string* result = 0 );

private:
   bool compile_re( const std::string& re );
   void run_action( const char* src, std::string* out ) const;

private:

   union
   {
      /** Captured substrings.
      * If execute succeeded, this will contain all the
      * captured substrings starting from index 1 to MAX_CAPTURES.
      * Index 0 is a capture of the entire matched substring. */
      CAP m_caps[ MAX_CAPTURES + 1 ];
      int m_pcre_buf[ ( MAX_CAPTURES + 1 ) * 3 ];
   };

private:
   Type         m_type;
   std::string  m_action;
   long         m_manips;
   /** Secretly a pcre* */
   void*        m_compiled;
   /** Secretly a pcre_extra* */
   void*        m_compiled_extra;
   unsigned int m_num_caps;
   std::string  m_error;
};

const std::string& RegexOp::getLastError( ) const
{
   return m_error;
}

RegexOp::Type RegexOp::getType( ) const
{
   return m_type;
}

bool RegexOp::ok( ) const
{
   return (m_type != TYPE_BAD);
}

bool RegexOp::getSubstr( const char* src,
                         unsigned int index, 
                         std::string* out ) const
{
   if (!ok( ) || index > m_num_caps)
      return false;

   out->assign( src + m_caps[ index ].start, 
                m_caps[ index ].end - m_caps[ index ].start );

   return true;
}
unsigned int RegexOp::getNumSubstrs( ) const
{
   return m_num_caps;
}


#endif