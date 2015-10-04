#include "RegexOp.h"
/* Requires pcre.lib */
#include "../pcre/pcre.h"
#include <sstream>

RegexOp::RegexOp( Type type, const std::string& re, const std::string& action,
                  long manips )
{
   m_type           = type;
   m_action         = action;
   m_compiled       = 0;
   m_compiled_extra = 0;
   m_manips         = manips;
   m_num_caps       = 0;

   if (!compile_re( re ))
      m_type = TYPE_BAD;
}

RegexOp::~RegexOp( )
{
   pcre_free( m_compiled );
   pcre_free( m_compiled_extra );
}

bool RegexOp::compile_re( const std::string& re )
{
   int err_off;
   const char* sz_err;
   int opts = 0;
   opts |= (m_manips & MANIP_INSENSITIVE) ? PCRE_CASELESS : 0;
   opts |= (m_manips & MANIP_UNGREEDY) ? PCRE_UNGREEDY : 0;

   m_compiled = pcre_compile( re.c_str( ), opts, &sz_err, &err_off, 0 );

   if (!m_compiled)
   {
      std::ostringstream ss;
      ss << "Regex compile error (" << err_off << "): ";
      ss << sz_err;
      m_error = ss.str( );
      return false;
   }

   m_compiled_extra = pcre_study( (pcre*) m_compiled, 0, &sz_err );

   pcre_fullinfo( (pcre*) m_compiled, (pcre_extra*) m_compiled_extra,
                  PCRE_INFO_CAPTURECOUNT, &m_num_caps );

   if (m_num_caps > MAX_CAPTURES)
      m_num_caps = MAX_CAPTURES;

   return true;
}

bool RegexOp::execute( const char* src, std::size_t len, std::string* result )
{
   if (!ok( ) || !m_compiled)
      return 0;

   int pcre_result = 0;

   if (m_manips & MANIP_INVERT)
   {
      pcre_result = pcre_exec( (pcre*) m_compiled, (pcre_extra*) m_compiled_extra, 
                               src, len, 0, PCRE_NOTEMPTY, 
                               m_pcre_buf, ( MAX_CAPTURES + 1 ) * 3 );

      if (pcre_result == PCRE_ERROR_NOMATCH)
      {
         // No match matches the entire line.
         m_caps[ 0 ].start = 0;
         m_caps[ 0 ].end = len;
         
         if (m_type == TYPE_SUBSTITUTE && result)
            run_action( src, result );

         return true;
      }

      return false;
   }

   int off      = 0;
   bool matched = false;
   memset( m_caps, -1, sizeof( m_caps ) );
   
   do
   {
      pcre_result = pcre_exec( (pcre*) m_compiled, (pcre_extra*) m_compiled_extra, 
                               src, len, off, PCRE_NOTEMPTY, 
                               m_pcre_buf, ( MAX_CAPTURES + 1 ) * 3 );
      
      if (pcre_result < 0)
         break;

      matched = true;

      if (m_type == TYPE_SUBSTITUTE && result)
      {
         result->append( src + off, m_caps[ 0 ].start - off );
         run_action( src, result );
      }
      
      off = m_caps[ 0 ].end;

   } while (m_manips & MANIP_GLOBAL);

   if (m_type == TYPE_SUBSTITUTE && result && matched)
      result->append( src + off, len - off );

   return matched;
}

void RegexOp::run_action( const char* src, std::string* out ) const
{
   const char* c = m_action.c_str( );
   bool esc      = false;
   std::size_t cap;

   while (*c)
   {
      if (!esc)
      {
         if (*c == '\\')
            esc = true;
         else
            out->push_back( *c );
      }
      else
      {
         switch (*c)
         {
         case '0': case '1': case '2': case '3': case '4':
         case '5': case '6': case '7': case '8': case '9':
            {
               cap = *c - '0';
               if (cap <= m_num_caps)
               {
                  out->append( src + m_caps[ cap ].start, 
                               m_caps[ cap ].end - m_caps[ cap ].start );
               }
            }
            break;

#ifdef REGEXOP_ALLOW_ESCAPED_ASCII
         case 'r': 
            out ->push_back( '\r' ); break;
         case 'n':
            out->push_back( '\n' ); break;
         case 't':
            out->push_back( '\t' ); break;
#endif

         default:
            out->push_back( *c ); break;
         }

         esc = false;
      }

      ++c;
   }
}

static const std::string REOP_ERR_MALFORMED( "Definition is malformed." );
static const std::string REOP_ERR_BADPREFIX( "Bad prefix on definition. "
                                             "Only \'s\' and \'m\' are supported." );
static const std::string REOP_ERR_BADMANIP( "Bad manipulator on definition. "
                                            "Only \'i\', \'g\', \'u\', and \'v\' "
                                            "are supported." );

RegexOp* RegexOp::from_str( const std::string& s, const std::string** err /* = 0  */ )
{
   if (s.length( ) < 3) // smallest is m//
   {
      *err = &REOP_ERR_MALFORMED;
      return 0;
   }

   const char* c = s.c_str( );

   Type t;
   switch (*(c++))
   {
   case 'm':
      t = TYPE_MATCH;      break;   
   case 's':
      t = TYPE_SUBSTITUTE; break;
   default:
      *err = &REOP_ERR_BADPREFIX;
      return 0;
   }

   if (!*c)
   {
      *err = &REOP_ERR_MALFORMED;
      return 0;
   }

   const char delim = *(c++);

   std::string re;
   bool esc = false;

   while (*c)
   {
      // Leave everything but delims escaped.
      if (!esc)
      {
         if (*c == delim)
            break;

         if (*c == '\\')
         {
            esc = true;
            ++c;
            continue;
         }
      }
      else
      {
         if (*c != delim)
            re.push_back( '\\' );
         esc = false;
      }

      re.push_back( *(c++) );
   }

   if (!*(c++))
   {
      *err = &REOP_ERR_MALFORMED;
      return 0;
   }

   std::string action;
   if (t == TYPE_SUBSTITUTE)
   {
      esc = false;

      while (*c)
      {
         // Leave everything but delims escaped.
         if (!esc)
         {
            if (*c == delim)
               break;

            if (*c == '\\')
            {
               esc = true;
               ++c;
               continue;
            }
         }
         else
         {
            if (*c != delim)
               action.push_back( '\\' );
            esc = false;
         }

         action.push_back( *(c++) );
      }

      if (!*(c++))
      {
         *err = &REOP_ERR_MALFORMED;
         return 0;
      }
   }

   long manips = 0;

   while (*c)
   {
      switch (*(c++))
      {
      case 'i':
         manips |= MANIP_INSENSITIVE;     break;
      case 'g':
         manips |= MANIP_GLOBAL;          break;
      case 'u':
         manips |= MANIP_UNGREEDY;        break;
      case 'v':
         manips |= MANIP_INVERT;          break;
      default:
         *err = &REOP_ERR_BADMANIP;
         return 0;
      }
   }

   return new RegexOp( t, re, action, manips );
}
