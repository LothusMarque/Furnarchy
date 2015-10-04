#ifndef _NET_SCAN_H__
#define _NET_SCAN_H__

#include <list>
#include <string>
#include <cstdarg>

namespace net_scan
{
   struct FIELD
   {
      enum Field_T
      {
         FIELD_INT,
         FIELD_STR,
         FIELD_CHAR,
      };

      // Ignored by format().
      Field_T      type;
      int          value_int;
      // Also used by FIELD_CHAR
      std::string  value_str;
   };

   bool scan( const std::string& pattern, 
              const std::string& subj,
              std::list<FIELD>& fields );

   std::string format( const std::string& pattern, const std::list<FIELD>& fields );

};

#endif _NET_SCAN_H__