#ifndef _NET_SCAN_H__
#define _NET_SCAN_H__

#include <list>
#include <string>

namespace net_scan
{
   struct FIELD
   {
      enum Field_T
      {
         FIELD_INT,
         FIELD_STR,
      };

      Field_T      type;
      int          value_int;
      std::string  value_str;
   };

   bool scan( const char* pattern, 
              const char* subj, int subj_len,
              list<FIELD>& fields );
};

#endif _NET_SCAN_H__