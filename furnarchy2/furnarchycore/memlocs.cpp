#include "memlocs.h"
#include "core.h"
#include "common/IniFile.h"

//////////////////////////////////////////////////////////////////////////

bool         memlocs::g_initialized = false;
string       memlocs::g_date;
unsigned int memlocs::g_client_crc  = 0;
unsigned int memlocs::g_locs_crc    = 0;
void*        memlocs::g_locs[ memlocs::LOC_MAX ];

//////////////////////////////////////////////////////////////////////////

bool memlocs::initialize( unsigned int crc )
{
   if (!g_initialized)
   {
      g_initialized = true;
      g_client_crc  = crc;
      g_locs_crc    = 0;

      for (size_t i = 0; i < memlocs::LOC_MAX; i++)
         g_locs[ i ] = 0;

      load_memlocs( );

      if (g_locs_crc != g_client_crc)
         core::system_errspeak( "The client CRC does not match the memlocs CRC." );
   }
   return true;
}

bool memlocs::uninitialize( )
{
   if (g_initialized)
   {
      g_initialized = false;
   }
   return true;
}

bool memlocs::load_memlocs( )
{
   g_locs_crc = 0;
   /* Go through the ini files in the memlocs folder looking for a memloc file
      matching this client's CRC. */

   WIN32_FIND_DATA wfd;
   memset( &wfd, 0, sizeof( wfd ) );
   HANDLE hfind = FindFirstFile( (core::get_furn_root( ) + "\\memlocs\\*.ini").c_str( ), &wfd );
   if (hfind != INVALID_HANDLE_VALUE)
   {
      do
      {
         string memlocs_file = core::get_furn_root( ) + "\\memlocs\\" + wfd.cFileName;

         IniFile ini( memlocs_file );
         if (!ini.open( false ))
         {
            core::log_error( "Couldn't load memlocs file (%s).", memlocs_file.c_str( ) );
         }
         else
         { 
            unsigned int locs_crc = strtoul( ini.read( "CRC32" ).c_str( ), 0, 16 );
            if (g_client_crc == locs_crc) // Use this file
            {
               g_locs_crc = locs_crc;

               #define _READLOC( s ) g_locs[ s ] = (void*) strtoul( ini.read( #s ).c_str( ), 0, 16 )
               
               _READLOC( CHATBOX_PUSH_FN );
               _READLOC( CHATBOX_PUSH_SYSMSG_FN );
               _READLOC( CHATBOX_RENDER_PIPE_JMP );
			   _READLOC( SHOWPLAYERMENU_FN );
              /* _READLOC( INPUTBOX_GETSZ_CALL );
               _READLOC( INPUTBOX_GETSZ_FN );
               _READLOC( INPUTBOX_PROCESS_CALL );
               _READLOC( INPUTBOX_PROCESS_FN );
               _READLOC( INPUTBOX_PUSH_HISTORY_FN );
               _READLOC( RENDER_WORLD_FN );
               _READLOC( RENDER_FLOOR_CALL );
               _READLOC( RENDER_FLOOR_FN );
               _READLOC( RENDER_WALL_CALL );
               _READLOC( RENDER_WALL_FN );
               _READLOC( RENDER_OBJECT_CALL );
               _READLOC( RENDER_OBJECT_FN );
               _READLOC( RENDER_AVATAR_CALL );
               _READLOC( RENDER_AVATAR_FN );
               _READLOC( SUPPRESS_RENDERING );
               _READLOC( NETOBJ_P );
               _READLOC( NETOBJ_SOCKET_OFFSET );
               _READLOC( PLAYER_NAME_BUF );
               _READLOC( PLAYER_PASSWORD_BUF );
               _READLOC( PLAYER_DESC_BUF );
               _READLOC( PLAYER_COLORS_BUF );
               _READLOC( PLAYER_GUID );
               _READLOC( IGNORE_ADD_FN );
               _READLOC( IGNORE_RM_FN );
               _READLOC( IGNORE_TEST_FN );
               _READLOC( IGNORE_LINE_TEST_FN );
               _READLOC( LOGFILE_OBJ );
               _READLOC( FCLOSE_FN );
               _READLOC( MAP_PALETTE_FN );
               _READLOC( BUTTONS_PALETTE ); */

               g_date = ini.read( "DATE" );
               return true;
            }
         }

      } while (FindNextFile( hfind, &wfd ));
      FindClose( hfind );
   }
   return false;
}
