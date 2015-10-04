#ifndef _LATIN_1_H_
#define _LATIN_1_H_

#ifndef NULL
   #define NULL 0
#endif

//Note that these functions do not care for null-termination.

class latin_1
{
protected:

   latin_1( );
public:

   // Note: &quot; -> '\"' but not vice versa.
   //       &amp;  -> '&'  but not vice versa.
   // Cannot support &euro; because not part of latin-1.

   //Functions-----------------------------------------------------------//

   //Returns new length.
   static int resolve_names( const char* cp_in, int in_len, 
                             char* p_out = NULL, int out_len = 0 );
   //Returns new length.
   //CAVEAT: cp_in != p_out, or undefined behavior.
   static int expand_symbols( const char* cp_in, int in_len, 
                              char* p_out = NULL, int out_len = 0 );

protected:

   static int find_sorted_name( const char* cp_in, int in_len );

   //Very const static variables----------------------------------------//

protected:

   enum { LONGEST_LATIN_NAME =  8, SHORTEST_LATIN_NAME = 4 };
   static const int         latin_name_lens_special[ 256 ];//No &quot; or &amp;
   static const char* const latin_names_special[ 256 ];    //No &quot; or &amp;
   static const int         latin_name_lens_sorted[ 100 ];
   static const char* const latin_names_sorted[ 100 ];
   static const char        latin_values_sorted[ 100 ];
};

#endif