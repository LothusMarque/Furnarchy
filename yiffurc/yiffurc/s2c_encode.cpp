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

#include "net.h"
#include "pfx_heap.h"
#include "misc.h"
#include "s2c_enc_stock.h"
#include <string>

namespace yiffurc {
using namespace std;

const S2C_ENC_STOCK s2c_enc_stock;

const char* YIFFURC_CALL s2c_enc_avdestroy( unsigned int uid )
{
   char* out = pfx_new<char>( 1 + 3 + 1 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, ")", 1 );
   itof3_220( uid, out );  out += 3;
   *out = '&'; out += 1;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_avcreate( unsigned int uid, 
                                           unsigned int x, unsigned int y, 
                                           unsigned int frame, 
                                           const char* name, const char* color,
                                           unsigned int flags )
{
   const size_t name_len  = STRLEN( name );
   const size_t color_len = STRLEN( color );

   char* out = pfx_new<char>( 1 + 3 + 1 + 2 + 2 + 2 + 1 + name_len + color_len + 1 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "<", 1 );
   itof3_220( uid, out );  out += 3;
   *out = '&'; out += 1;
   itof2_220( x, out ); out += 2;
   itof2_220( y, out ); out += 2;
   itof2_220( frame, out ); out += 2;
   *out = itof1_220( name_len ); out += 1;
   out = chain_memcpy( out, name, name_len );
   out = chain_memcpy( out, color, color_len );
   *out = itof1_220( flags ); out += 1;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_avtween( unsigned int uid, 
                                          unsigned int x_dst, unsigned int y_dst, 
                                          unsigned int end_frame )
{
   char* out = pfx_new<char>( 1 + 3 + 1 + 2 + 2 + 2 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "/", 1 );
   itof3_220( uid, out );  out += 3;
   *out = '&'; out += 1;
   itof2_220( x_dst, out );  out += 2;
   itof2_220( y_dst, out );  out += 2;
   itof2_220( end_frame, out ); out += 2;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_avupdatecolors( unsigned int uid, 
                                                 unsigned int frame, const char* color )
{
   const size_t color_len = STRLEN( color );

   char* out = pfx_new<char>( 1 + 3 + 1 + 2 + color_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "B", 1 );
   itof3_220( uid, out );  out += 3;
   *out = '&'; out += 1;
   itof2_220( frame, out );  out += 2;
   out = chain_memcpy( out, color, color_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_avshow( unsigned int uid, 
                                         unsigned int x, unsigned int y, 
                                         unsigned int frame )
{
   char* out = pfx_new<char>( 1 + 3 + 1 + 2 + 2 + 2 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "A", 1 );
   itof3_220( uid, out );  out += 3;
   *out = '&'; out += 1;
   itof2_220( x, out );  out += 2;
   itof2_220( y, out );  out += 2;
   itof2_220( frame, out );  out += 2;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_avhide( unsigned int uid,
                                         unsigned int x, unsigned int y )
{
   char* out = pfx_new<char>( 1 + 3 + 1 + 2 + 2 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "C", 1 );
   itof3_220( uid, out );  out += 3;
   *out = '&'; out += 1;
   itof2_220( x, out );  out += 2;
   itof2_220( y, out );  out += 2;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_badge( unsigned int badge )
{
   char* out = pfx_new<char>( 4 + 1 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]-#B", 4 );
   *(out++) = (badge % 95) + ' ';
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_camset( unsigned int x, unsigned int y )
{
   char* out = pfx_new<char>( 1 + 2 + 2 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "@", 1 );
   itof2( x, out );  out += 2;
   itof2( y, out );  out += 2;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}


const char* YIFFURC_CALL s2c_enc_camtween( unsigned int x_dst, unsigned int y_dst,
                                        unsigned int x_src, unsigned int y_src )
{
   char* out = pfx_new<char>( 1 + 2 + 2 + 2 + 2 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "@", 1 );
   itof2( x_dst, out );  out += 2;
   itof2( y_dst, out );  out += 2;
   itof2( x_src, out );  out += 2;
   itof2( y_src, out );  out += 2;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_chat( const char* message )
{
   size_t message_len = STRLEN( message );

   char* out = pfx_new<char>( 1 + message_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "(", 1 );
   out = chain_memcpy( out, message, message_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_createfail( const char* error )
{
   size_t error_len = STRLEN( error );

   char* out = pfx_new<char>( 7 + error_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "N{NAME}", 1 );
   out = chain_memcpy( out, error, error_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_specitag( const char* color,
                                           const char* gsd )
{
   const size_t color_len = STRLEN( color );
   const size_t gsd_len   = STRLEN( gsd );

   char* out = pfx_new<char>( 4 + color_len + gsd_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]-#A", 4 );
   out = chain_memcpy( out, color, color_len );
   out = chain_memcpy( out, gsd, gsd_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_dreamclear( unsigned int x, unsigned int y )
{
   char* out = pfx_new<char>( 2 + 2 + 2 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]t", 2 );
   itof2( x, out );  out += 2;
   itof2( y, out );  out += 2;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_dreampos( unsigned int x, unsigned int y,
                                        const char* name )
{
   size_t name_len = STRLEN( name );

   char* out = pfx_new<char>( 2 + 2 + 2 + name_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]s", 2 );
   itof2( x, out );  out += 2;
   itof2( y, out );  out += 2;
   out = chain_memcpy( out, name, name_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_dreampos2( unsigned int x, unsigned int y,
                                         const char* name,
                                         const char* caption )
{
   size_t name_len = STRLEN( name );
   size_t caption_len = STRLEN( caption );

   if (caption_len)
   {
      char* out = pfx_new<char>( 2 + 2 + 2 + 1 + name_len + 1 + caption_len + 2 );
      const char* const ret = out;

      out = chain_memcpy( out, "]s", 2 );
      itof2( x, out );  out += 2;
      itof2( y, out );  out += 2;
      out = chain_memcpy( out, "1", 1 );
      out = chain_memcpy( out, name, name_len );
      out = chain_memcpy( out, " ", 1 );
      out = chain_memcpy( out, caption, caption_len );
      out = chain_memcpy( out, "\n", 2 );

      return ret;
   }

   char* out = pfx_new<char>( 2 + 2 + 2 + 1 + name_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]s", 2 );
   itof2( x, out );  out += 2;
   itof2( y, out );  out += 2;
   out = chain_memcpy( out, "1", 1 );
   out = chain_memcpy( out, name, name_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_fetchdream( const char* name, unsigned int id )
{
   size_t name_len STRLEN( name );
   size_t id_len = dec_len( id );

   char* out = pfx_new<char>( 3 + name_len + 1 + id_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]q ", 3 );
   out = chain_memcpy( out, name, name_len );
   out = chain_memcpy( out, " ", 1 );
   out = chain_itoa( out, id );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_fetchpid( const char* name, unsigned int id )
{
   size_t name_len STRLEN( name );
   size_t id_len = dec_len( id );

   char* out = pfx_new<char>( 2 + id_len + 1 + name_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]&", 2 );
   out = chain_itoa( out, id );
   out = chain_memcpy( out, " ", 1 );
   out = chain_memcpy( out, name, name_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_floorset( unsigned int floor, unsigned int x, unsigned int y )
{
   char* out = pfx_new<char>( 1 + 2 + 2 + 2 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "1", 1 );
   itof2( x, out ); out += 2;
   itof2( y, out ); out += 2;
   itof2( floor, out ); out += 2;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_floorsets( unsigned int count, const IDXY* floors )
{
   char* out = pfx_new<char>( 1 + count * (2 + 2 + 2) + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "1", 1 );

   for (unsigned int i = 0; i < count; i++)
   {
      itof2( floors[ i ].x, out ); out += 2;
      itof2( floors[ i ].y, out ); out += 2;
      itof2( floors[ i ].id, out ); out += 2;
   }

   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_footobj( unsigned int obj )
{
   char* out = pfx_new<char>( 1 + 2 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "%", 1 );
   itof2( obj, out ); out += 2;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_invobj( unsigned int obj )
{
   char* out = pfx_new<char>( 1 + 2 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "^", 1 );
   itof2( obj, out ); out += 2;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_macid( const char* mac_id )
{
   size_t mac_id_len = STRLEN( mac_id );

   char* out = pfx_new<char>( 3 + mac_id_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "\\PW", 3 );
   out = chain_memcpy( out, mac_id, mac_id_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_motd( const char* motd )
{
   size_t motd_len = STRLEN( motd );

   char* out = pfx_new<char>( motd_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, motd, motd_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_objset( unsigned int obj, unsigned int x, unsigned int y )
{
   char* out = pfx_new<char>( 1 + 2 + 2 + 2 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, ">", 1 );
   itof2_220( x, out ); out += 2;
   itof2_220( y, out ); out += 2;
   itof2_220( obj, out ); out += 2;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_objsets( unsigned int count, const IDXY* objects )
{
   char* out = pfx_new<char>( 1 + count * (2 + 2 + 2) + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, ">", 1 );

   for (unsigned int i = 0; i < count; i++)
   {
      itof2_220( objects[ i ].x, out ); out += 2;
      itof2_220( objects[ i ].y, out ); out += 2;
      itof2_220( objects[ i ].id, out ); out += 2;
   }

   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_onlineresult( const char* player, bool online )
{
   size_t player_len = STRLEN( player );

   char* out = pfx_new<char>( 2 + 1 + player_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]%", 2 );
   *out = (online) ? '1' : '0'; out++;
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_playsong( unsigned int song )
{
   char* out = pfx_new<char>( 2 + 2 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]j", 2 );
   itof2( song, out ); out += 2;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_playsound( unsigned int sound )
{
   char* out = pfx_new<char>( 1 + 2 + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "!", 1 );
   itof2( sound, out ); out += 2;
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_portrait( const char* player,
                                        const char color[ 10 ],
                                        const char species[ 3 ] )
{
   size_t player_len = STRLEN( player );

   char* out = pfx_new<char>( 2 + 10 + 3 + player_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]f", 2 );
   out = chain_memcpy( out, color, 10 );
   out = chain_memcpy( out, species, 3 );
   out = chain_memcpy( out, player, player_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_synccolorcode( const char* color,
                                                const char* gsd )
{
   const size_t color_len = STRLEN( color );
   const size_t gsd_len   = STRLEN( gsd );

   char* out = pfx_new<char>( 2 + color_len + gsd_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]}", 2 );
   out = chain_memcpy( out, color, color_len );
   out = chain_memcpy( out, gsd, gsd_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_trigger( unsigned int x1, unsigned int y1,
                                       unsigned int x2, unsigned int y2,
                                       unsigned int count,
                                       const DSXY* lines )
{
   char* out = pfx_new<char>( 1 + 2 + 2 + 2 + 2 + count * ( 2 + 2 + 2 ) + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "7", 1 );
   itof2( x1, out ); out += 2;
   itof2( y1, out ); out += 2;
   itof2( x2, out ); out += 2;
   itof2( y2, out ); out += 2;

   for (unsigned int i = 0; i < count; i++)
   {
      itof2( lines[ i ].line, out ); out += 2;
      itof2( lines[ i ].x, out ); out += 2;
      itof2( lines[ i ].y, out ); out += 2;
   }

   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_triggerself( unsigned int x1, unsigned int y1,
                                           unsigned int x2, unsigned int y2,
                                           unsigned int count,
                                           const DSXY* lines )
{
   char* out = pfx_new<char>( 1 + 2 + 2 + 2 + 2 + count * ( 2 + 2 + 2 ) + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "6", 1 );
   itof2( x1, out ); out += 2;
   itof2( y1, out ); out += 2;
   itof2( x2, out ); out += 2;
   itof2( y2, out ); out += 2;

   for (unsigned int i = 0; i < count; i++)
   {
      itof2( lines[ i ].line, out ); out += 2;
      itof2( lines[ i ].x, out ); out += 2;
      itof2( lines[ i ].y, out ); out += 2;
   }

   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_usebgfile( const char* file )
{
   size_t file_len = STRLEN( file );

   char* out = pfx_new<char>( 3 + file_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]cc", 3 );
   out = chain_memcpy( out, file, file_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_usedream( const char* name, unsigned int id )
{
   size_t name_len = STRLEN( name );
   size_t id_len   = dec_len( id );

   char* out = pfx_new<char>( 3 + name_len + 1 + id_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]r ", 3 );
   out = chain_memcpy( out, name, name_len );
   out = chain_memcpy( out, " ", 1 );
   out = chain_itoa( out, id );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_usemapfile( const char* file )
{
   size_t file_len = STRLEN( file );

   char* out = pfx_new<char>( 1 + file_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, ";", 1 );
   out = chain_memcpy( out, file, file_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_varset( unsigned int first, unsigned int count,
                                      int* values, size_t stride )
{
   if (!count)
      return 0;

   string str_t;
   char t[ 3 ];

   itof2( first, t );
   str_t.append( t, 2 );

   int prev_value = *values;
   unsigned int run = 1;

   values = (int*)( ((char*) values) + stride * (first + 1));

   for (unsigned int i = 0; i < count; i++)
   {
      if (prev_value != *values)
      {
         if (run > 1 || prev_value == 0x4000)
         {
            str_t.append( "!mL", 3 );
            itof3( run, t - 1 );
            str_t.append( t, 3 );
            itof3( prev_value, t );
            str_t.append( t, 3 );
         }
         else
         {
            itof3( prev_value, t );
            str_t.append( t, 3 );
         }

         run = 1;
      }
      else
         run++;

      prev_value = *values;
      values = (int*)( ((char*) values) + stride );
   }

   // One more time.
   if (run > 1 || prev_value == 0x4000)
   {
      str_t.append( "!mL", 3 );
      itof3( run, t - 1 );
      str_t.append( t, 3 );
      itof3( prev_value, t );
      str_t.append( t, 3 );
   }
   else
   {
      itof3( prev_value, t );
      str_t.append( t, 3 );
   }

   char* out = pfx_new<char>( 1 + str_t.length( ) + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "0", 1 );
   out = chain_memcpy( out, str_t.c_str( ), str_t.length( ) );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_msgbox( int msgbox_flags,
                                         const char* message )
{
   const size_t flags_len   = dec_len( (unsigned) msgbox_flags );
   const size_t message_len = STRLEN( message );

   char* out = pfx_new<char>( 7 + flags_len + 1 + message_len + 2 );
   const char* const ret = out;

   out = chain_memcpy( out, "]#xxxx ", 7 );
   out = chain_itoa( out, (unsigned) msgbox_flags );
   out = chain_memcpy( out, " ", 1 );
   out = chain_memcpy( out, message, message_len );
   out = chain_memcpy( out, "\n", 2 );

   return ret;
}

const char* YIFFURC_CALL s2c_enc_displaylistclose( )
{
   return s2c_enc_stock.sz_displaylistclose;
}

const char* YIFFURC_CALL s2c_enc_displaylistopen( )
{
   return s2c_enc_stock.sz_displaylistopen;
}

const char* YIFFURC_CALL s2c_enc_dragonroar( )
{
   return s2c_enc_stock.sz_dragonroar;
}

const char* YIFFURC_CALL s2c_enc_loginfail( )
{
   return s2c_enc_stock.sz_loginfail;
}

const char* YIFFURC_CALL s2c_enc_motdend( )
{
   return s2c_enc_stock.sz_motdend;
}

const char* YIFFURC_CALL s2c_enc_pass( )
{
   return s2c_enc_stock.sz_pass;
}

const char* YIFFURC_CALL s2c_enc_ver( )
{
   return s2c_enc_stock.sz_ver;
}

const char* YIFFURC_CALL s2c_enc_verreq( )
{
   return s2c_enc_stock.sz_verreq;
}

} // namespace yiffurc