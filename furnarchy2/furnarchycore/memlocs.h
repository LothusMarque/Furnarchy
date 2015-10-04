#ifndef _FURNARCHYCORE_MEMLOCS_H__
#define _FURNARCHYCORE_MEMLOCS_H__

#include "common.h"

class memlocs
{
public:
   enum LocType { CHATBOX_PUSH_FN = 0, CHATBOX_PUSH_SYSMSG_FN,
                  CHATBOX_RENDER_PIPE_JMP, 
				  SHOWPLAYERMENU_FN,
                 /* INPUTBOX_GETSZ_CALL, INPUTBOX_GETSZ_FN,
                  INPUTBOX_PROCESS_CALL, INPUTBOX_PROCESS_FN,
                  INPUTBOX_PUSH_HISTORY_FN,
                  RENDER_WORLD_FN, 
                  RENDER_FLOOR_CALL, RENDER_FLOOR_FN,
                  RENDER_WALL_CALL, RENDER_WALL_FN,
                  RENDER_OBJECT_CALL, RENDER_OBJECT_FN,
                  RENDER_AVATAR_CALL, RENDER_AVATAR_FN,
                  SUPPRESS_RENDERING,
                  NETOBJ_P, NETOBJ_SOCKET_OFFSET,
                  PLAYER_NAME_BUF, PLAYER_PASSWORD_BUF, 
                  PLAYER_DESC_BUF, PLAYER_COLORS_BUF, 
                  PLAYER_GUID,
                  IGNORE_ADD_FN, IGNORE_RM_FN, IGNORE_TEST_FN,
                  IGNORE_LINE_TEST_FN,
                  LOGFILE_OBJ, FCLOSE_FN,
                  MAP_PALETTE_FN, BUTTONS_PALETTE, */
                  LOC_MAX  };

   static bool initialize( unsigned int crc );
   static bool uninitialize( );

   static bool valid( );
   static unsigned int get_client_crc( );
   static void* get( LocType i );
   static const string& get_date( );

private:
   static bool load_memlocs( );

private:
   static bool         g_initialized;
   static unsigned int g_client_crc;
   static string       g_date;
   static unsigned int g_locs_crc;
   static void*        g_locs[ LOC_MAX ];
};

inline bool memlocs::valid( ) {
   return (g_client_crc && g_client_crc == g_locs_crc);
}
inline unsigned int memlocs::get_client_crc( ) {
   return g_client_crc;
}
inline void* memlocs::get( LocType i ) {
   return g_locs[ i ];
}
inline const string& memlocs::get_date( ) {
   return g_date;
}

#endif
