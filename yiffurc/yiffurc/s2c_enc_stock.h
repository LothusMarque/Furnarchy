/************************************************************************/
/* Copyright (c) 2006 Cluracan (strangelove@heroinpuppy.com)            */
/*                                                                      */
/* The "MIT" License                                                    */
/*                                                                      */
/* Permission is hereby granted, free of charge, to any person          */
/* obtaining a copy of this software and associated documentation       */
/* files (the "Software"), to deal in the Software without restriction, */
/* including without limitation the rights to use, copy, modify, merge, */
/* publish, distribute, sublicense, and/or sell copies of the Software, */
/* and to permit persons to whom the Software is furnished to do so,    */
/* subject to the following conditions:                                 */
/*                                                                      */
/* - The above copyright notice and this permission notice shall be     */
/* included in all copies or substantial portions of the Software.      */
/*                                                                      */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,      */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES      */
/* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND             */
/* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT          */
/* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,         */
/* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        */
/* DEALINGS IN THE SOFTWARE.                                            */
/************************************************************************/

#ifndef _YIFFURC_PFX_S2C_ENC_STOCK_H__
#define _YIFFURC_PFX_S2C_ENC_STOCK_H__

#include "pfx_heap.h"
#include "misc.h"
#include <string.h>

namespace yiffurc {

struct S2C_ENC_STOCK
{
   char* sz_cookiesready;
   char* sz_displaylistclose;
   char* sz_displaylistopen;
   char* sz_dragonroar;
   char* sz_loginfail;
   char* sz_motdend;
   char* sz_pass;
   char* sz_ver;
   char* sz_verreq;
   char* sz_whisperfailmore;

   S2C_ENC_STOCK( )
   {
      #define _STOCK_SZ( s ) ( strcpy( pfx_new<char>( strlen( s "\n" ) + 1, true ), s "\n" ) )

      sz_cookiesready = _STOCK_SZ( "([%] Your cookies are ready." );
      sz_displaylistclose = _STOCK_SZ( "=" );
      sz_displaylistopen = _STOCK_SZ( "~" );
      sz_dragonroar = _STOCK_SZ( "Dragonroar" );
      sz_loginfail = _STOCK_SZ( "Invalid name or password." );
      sz_motdend = _STOCK_SZ( "END" );
      sz_pass = _STOCK_SZ( "&&&&&&&&&&&&&" );
      sz_ver = _STOCK_SZ( "V0013" );
      sz_verreq = _STOCK_SZ( "]w" );
      sz_whisperfailmore = _STOCK_SZ( "(* More than one furre\'s name begins that way.  You need to type more of the name, or maybe all of their name, so I can tell exactly who you mean!  -- Beekin the Help Dragon" );

      #undef _STOCK_SZ
   }
   ~S2C_ENC_STOCK( )
   {
      pfx_free( sz_cookiesready );
      pfx_free( sz_displaylistclose );
      pfx_free( sz_displaylistopen );
      pfx_free( sz_dragonroar );
      pfx_free( sz_loginfail );
      pfx_free( sz_motdend );
      pfx_free( sz_pass );
      pfx_free( sz_ver );
      pfx_free( sz_verreq );
      pfx_free( sz_whisperfailmore );
   }
};

extern const S2C_ENC_STOCK s2c_enc_stock;

} // namespace yiffurc



#endif // #ifndef _YIFFURC_PFX_S2C_ENC_STOCK_H__
