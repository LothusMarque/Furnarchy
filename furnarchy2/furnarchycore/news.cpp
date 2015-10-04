#include "news.h"
#include "core.h"
#include "tinyxml/tinyxml.h"
#include <curl/curl.h>
#include <process.h>
#include <ios>
#include <sstream>

namespace news {

//URL news xml file is stored. Needs to be a completely valid URL path.
#define NEWSURL     "http://proxy.entwinedstudios.com/furnarchy_cl_news.php" 
#define CANCELEVENT "furn2::news::cancel_event"

//////////////////////////////////////////////////////////////////////////

bool   g_initialized;
CURLM* g_curlm;
CURL*  g_curl_news;

//////////////////////////////////////////////////////////////////////////

bool   fetch_start( );
void   fetch( );
void   fetch_cancel( );
size_t fetch_fwrite( const void* buf, size_t size, size_t count, FILE* stream );
void   process_news( );

//////////////////////////////////////////////////////////////////////////

bool initialize( )
{
   if (!g_initialized)
   {
      curl_global_init( CURL_GLOBAL_WIN32 );
      g_initialized = true;
   }
   return true;
}

bool uninitialize( )
{
   if (g_initialized)
   {
      fetch_cancel( );
      curl_global_cleanup( );
      g_initialized = false;
   }
   return true;
}

void on_tick( unsigned int dt )
{
   if (g_curlm)
      fetch( );
}

bool on_command( const ArgList& args )
{
   if (!_stricmp( args[ 0 ].c_str( ), "news" ))
   {
      fetch_start( );
      return true;
   }

   return false;
}

bool fetch_start( )
{
   if (g_curlm)
      // Already fetching.
      return true;


   // Clear the news file.
   {
      ofstream ss( (core::get_furn_doc_root( ) + "\\news.xml").c_str( ), std::ios_base::trunc );
   }

   g_curlm     = curl_multi_init( );
   g_curl_news = curl_easy_init( );
   curl_easy_setopt( g_curl_news, CURLOPT_URL, NEWSURL );
   curl_easy_setopt( g_curl_news, CURLOPT_USERAGENT, "Furnarchy2" );
   curl_easy_setopt( g_curl_news, CURLOPT_WRITEFUNCTION, fetch_fwrite );

   curl_multi_add_handle( g_curlm, g_curl_news );

   int running = 0;
   while (curl_multi_perform( g_curlm, &running ) == CURLM_CALL_MULTI_PERFORM) ;
   return true;
}

void fetch_cancel( )
{
   if (g_curlm)
   {
      curl_multi_remove_handle( g_curlm, g_curl_news );
      curl_easy_cleanup( g_curl_news );
      curl_multi_cleanup( g_curlm );
      g_curlm = NULL;
      g_curl_news = NULL;
   }
}

void fetch( )
{
   if (g_curlm)
   {
      // Check for data.
      fd_set fd_r, fd_w, fd_e;
      FD_ZERO( &fd_r );
      FD_ZERO( &fd_w );
      FD_ZERO( &fd_e );
      int max_fd = 0;
      curl_multi_fdset( g_curlm, &fd_r, &fd_w, &fd_e, &max_fd );

      TIMEVAL tv = { 0, 0 };
      int select_code = select( max_fd + 1, &fd_r, &fd_w, &fd_e, &tv );
      if (select_code > 0)
      {
         CURLMcode code;
         do
         {
            int running;
            code = curl_multi_perform( g_curlm, &running );

            if (running == 0)
            {
               // All done.
               CURLMsg* msg;
               int msgs_left;
               while (msg = curl_multi_info_read( g_curlm, &msgs_left ))
                  if (msg->msg == CURLMSG_DONE)
                     break;
               
               assert( msg && msg->easy_handle == g_curl_news );

               int response_code = 0;
               curl_easy_getinfo( g_curl_news, CURLINFO_RESPONSE_CODE, &response_code );

               if (msg->data.result != CURLE_OK || response_code != 200)
               {
                  core::system_errspeak( "Couldn't fetch news." );
               }
               else
               {
                  process_news( );
               }

               fetch_cancel( );
               break;
            }
         } while (code == CURLM_CALL_MULTI_PERFORM);
      }
      else if (select_code < 0)
      {
         fetch_cancel( );
      }
   }
}

size_t fetch_fwrite( const void* buf, size_t size, size_t count, FILE* stream )
{
   ofstream fs( (core::get_furn_doc_root( ) + "\\news.xml").c_str( ), std::ios_base::app | std::ios_base::binary );
   if (!fs.fail( ))
      if (fs.good( ))
         fs.write( (const char*) buf, size * count );
   fs.close( );
   return count;
}

void process_news( )
{
   TiXmlDocument doc( core::get_furn_doc_root( ) + "\\news.xml" );
   if (doc.LoadFile( ) && !doc.Error( ))
   {
      const TiXmlElement* e_root = doc.RootElement( );
      if (e_root->ValueStr( ) == "news")
      {
         const TiXmlElement* e = e_root->FirstChildElement( "version" );
         if (e && e->GetText( ))
         {
            unsigned major = 0, minor = 0, revision = 0;
            sscanf( e->GetText( ), "%u.%u-r%u", &major, &minor, &revision );
            bool old = false;
            if (major > F2VER_MAJOR)
               old = true;
            if (major == F2VER_MAJOR && minor > F2VER_MINOR)
               old = true;
            if (major == F2VER_MAJOR && minor == F2VER_MINOR && revision > F2VER_REV )
               old = true;
            if (old)
            {
               ostringstream ss;
               ss << "<font color='error'><b>A newer version of Furnarchy ("
                     "<a href='http://proxy.entwinedstudios.com/'>"
                  << major << "." << minor << "-r" << revision << "</a>) is now available!</font>";
               core::system_speak( ss.str( ) );
            }
         }

         string date;
         e = e_root->FirstChildElement( "date" );
         if (e && e->GetText( ))
            date = e->GetText( );

         e = e_root->FirstChildElement( "body" );
         if (e && e->GetText( ))
         {
            ostringstream ss;
			ss << "<b>news</b>: <i>" << date << "</i> - <font color='success'>" << e->GetText( ) << "</font>";
            core::system_speak( ss.str( ) );
         }
      }
   }
}

} // namespace news