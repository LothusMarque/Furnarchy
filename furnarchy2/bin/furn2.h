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

#ifndef _FURNARCHY_PART_DEUX_H__
#define _FURNARCHY_PART_DEUX_H__

#include <cstddef>
#include <cstring>
#include <algorithm>
#include <windows.h>
#include <winsock.h>

namespace furn2
{   
   /** \defgroup furnarchy2 Furnarchy 2
   * bam boom */
   /**\{ */

   /** Message sent to windows to check if they aren't a dialog window.
   *  \param wparam Not used.
   *  \param lparam Not used.
   *  \return A nonzero value if the window does not want dialog window processing.
   *  \remarks Furnarchy will automatically call IsDialogMessage() for the
   *           active window to perform standard dialog box processing unless
   *           the window responds to this message with a non-zero return value.
   *           Dialog processing lets the system handle things such as navigating
   *           around TAB groups and control accelerators.
   */
   const UINT F2M_ISNOTDLGWND = WM_USER + 0xFA20;

   /** The network revision pass.
   * \sa on_inet on_onet net_handling */
   enum Revision { REV_DRAFT, REV_REVISING, REV_FINAL };
   
   /** \defgroup mod_events Module Events
   *  Module events.
   *  These events are received through a module's on_module() event handler.
   *  The event determines the type and meaning of on_module()'s \a param 
   *  parameter and return value.
   *  \sa on_module */
   /**\{*/
   enum Module_Event { 
      /** The module is being started for the first time.
      *  \param[in]     param       A const MODULEINFO*
      *  \return Return 0 always.
      *  \sa MODULEINFO */
      MOD_EVENT_START,
      /** The module is being shut down.
      *  \param[in]     param       Not used.
      *  \return Return 0 always. */
      MOD_EVENT_STOP, 
      /** The module has been enabled.
      *  \param[in]     param       Not used.
      *  \return Return 0 always.
      *  \sa f2_modenable() */
      MOD_EVENT_ENABLE,
      /** The module has been disabled.
      *  \param[in]     param       Not used.
      *  \return Return 0 always.
      *  \sa f2_modenable() */
      MOD_EVENT_DISABLE, 
      /** The module has been touched.
      *  \param[in]     param       Not used.
      *  \return Return 0 always.
      *  \sa f2_modtouch() */
      MOD_EVENT_TOUCH,
      /** An arbitrary communication/transport event.
      *  \param[in]     param       Meaning is arbitrary.
      *  \return Meaning is arbitrary.
      *  \remarks It's up to modules communicating with each other
      *           via MOD_EVENT_MSG to agree on what the parameters
      *           and return value mean.
      *  \sa f2_modmsg() */
      MOD_EVENT_MSG 
   };
   /**\}*/

   /** \defgroup cl_events Client Events
   *  Furcadia client events.
   *  These events are received through a module's on_client() event handler.
   *  The event determines the type and meaning of on_client()'s \a param 
   *  parameter and return value.
   *  \sa on_client */
   /**\{*/
   enum Client_Event { 
      /** The main client window was created.
      *  \param[in]     param    HWND of the created window.
      *  \return Return 0 always.
      *  \remarks After this message is received, f2_getwnd() will also
      *           return the window handle.
      *  \sa f2_getwnd() */
      CL_EVENT_WNDCREATED, 
      /** The client is about to change screens.
      *  \param[in]     param    A Client_Screen indicating the new screen.
      *  \return Return 0 always.
      *  \remarks Furnarchy monitors file access to determine when screen
      *           changes occur.
      *  \sa Client_Screen */
      CL_EVENT_SCRLOAD, 
      /** The main connection socket is being set.
      *  \param[in]     param    The new SOCKET handle.
      *  \return Return 0 always.
      *  \remarks The socket is set when the client initially attempts to
      *           establish a connection to the game server.  Modules may
      *           also trigger this event when they call f2__setmainsocket().
      *  \sa f2__setmainsocket() */
      CL_EVENT_SETMAINSOCKET,
      /** Furnarchy is rendering its lowest z-order elements.
      *  \param[in]     param    const RENDERPARAMS*
      *  \return Return 0 always.
      *  \remarks Furnarchy guarantees the device context provided in \a param
      *           will be in the format of a 24-bit top-down DIB section, for
      *           compatibility with SDL and easier direct manipulation.  Be
      *           sure to call GdiFlush() if you wish to read/modify the DIB's
      *           bits directly.
      *  \sa RENDERPARAMS, CL_EVENT_RENDER, CL_EVENT_POSTRENDER */
      CL_EVENT_PRERENDER, 
      /** Furnarchy is rendering its medium/normal z-order elements.
      *  \param[in]     param    const RENDERPARAMS*
      *  \return Return 0 always.
      *  \remarks Furnarchy guarantees the device context provided in \a param
      *           will be in the format of a 24-bit top-down DIB section, for
      *           compatibility with SDL and easier direct manipulation.  Be
      *           sure to call GdiFlush() if you wish to read/modify the DIB's
      *           bits directly.
      *  \sa RENDERPARAMS, CL_EVENT_PRERENDER, CL_EVENT_POSTRENDER */
      CL_EVENT_RENDER, 
      /** Furnarchy is rendering its highest z-order elements.
      *  \param[in]     param    const RENDERPARAMS*
      *  \return Return 0 always.
      *  \remarks Furnarchy guarantees the device context provided in \a param
      *           will be in the format of a 24-bit top-down DIB section, for
      *           compatibility with SDL and easier direct manipulation.  Be
      *           sure to call GdiFlush() if you wish to read/modify the DIB's
      *           bits directly.
      *  \sa RENDERPARAMS, CL_EVENT_PRERENDER, CL_EVENT_RENDER */
      CL_EVENT_POSTRENDER,
      /** A regular Furnarchy tick event occurred.
      *  \param[in]     param    unsigned int, the number of milliseconds 
      *                          since the last tick event.
      *  \return Return 0 always.
      *  \remarks This gives modules an opportunity to perform timed events.
      *           CL_EVENT_TICK has a resolution of about 160ms. */
      CL_EVENT_TICK,
      /** A line is being pushed onto the chat buffer.
      *  \param[in]     param    const char*
      *  \return Return non-zero to prevent the line from being pushed.
      *  \remarks This event depends on the client build and memlocs. */
      CL_EVENT_CHATBOX,
      /** The player popup menu is being shown.
      *  \param[in]     param    A const char*, the name of the player.
      *  \remarks This event is build-dependent. You'll only receive it
      *           if f2_buildok() returns true. */
      CL_EVENT_PLAYERMENUSHOW,
      /** The running scripts have changed.
      *  \param[in]     param    Not used.
      *  \remarks
      *  This is triggered whenever a script is run or exited/killed. */
      CL_EVENT_SCRIPT,
   };
   /**\}*/

   /** \defgroup wo_events World Events
   *  Game world events.
   *  Events concerning the world are recieved through a module's on_world() event
   *  handler.  The event determines the type and meaning of on_world()'s \a param
   *  parameter and return value.
   *  \sa on_world */
   /**\{*/
   enum World_Event { 
      /** The main connection to the server has been established.
      *  \param  param    Not used.
      *  \return Return 0 always. */
      WO_EVENT_CONNECTED, 
      /** The main connection to the server has been terminated or lost.
      *  \param param    Not used.
      *  \return Return 0 always. */
      WO_EVENT_DISCONNECTED, 
      /** The player has successfully logged in.
      *  \param param    Not used.
      *  \return Return 0 always.
      *  \remarks After this event, you can use f2_myplayerinfo() to 
      *           retrieve information on the player, although some 
      *           of it (namely the colors and desc) may
      *           still be in a state of flux at this point. */      
      WO_EVENT_LOGGEDIN,
      /** Player information has changed.
      *  \param param   Not used.
      *  \return Return 0 always.
      *  \remarks You can use f2_myplayerinfo() to retrieve the
      *           new information. */
      WO_EVENT_PLAYERINFO,
      /** Monster (avatar) updated.
      *  \param[in] param   GUID of the updated monster.
      *  \return Return 0 always.
      *  \remarks You can use f2_monsterinfo() to retrieve information
      *           on the updated monster. If f2_monsterinfo() returns NULL,
      *           this event occured in response to the monster being cleared. */
      WO_EVENT_MONSTER,
   };
   /**\}*/
   
   enum { MOD_DISABLED = 0x0, MOD_ENABLED = 0x1 };
   enum { MONSTER_HASPROFILE = 0x1, MONSTER_UNK1 = 0x2, MONSTER_UNK2 = 0x4 };
   enum { FROM_SERVER = 0, FROM_CLIENT = 0, FROM_GENERATED = 1 };
   enum Connection_Level { CONNLVL_DISCONNECTED = 0, CONNLVL_CONNECTED = 32,
                           CONNLVL_MOTDEND = 64, CONNLVL_LOGGEDIN = 128 };
   /** Different client screens.
   *  \sa on_client CL_EVENT_SCRLOAD */
   enum Client_Screen { 
      /** The title, splash, or connecting screen. */
      SCREEN_TITLE, 
      /** The character creation screen. */
      SCREEN_PICKEM, 
      /** The message of the day screen. */
      SCREEN_BORDER, 
      /** The regular world display screen. */
      SCREEN_MARBLED
   };

   /** Error codes returned by script functions.
   *  \sa api_script */
   enum Script_Error { 
      /** No error. */
      SCRIPT_OK, 
      /** A bad argument was passed. */
      SCRIPT_BADARG,
      /** Script file failed to load/run. */
      SCRIPT_FAILED, 
      /** The script ID is bad. */
      SCRIPT_BADSID, 
      /** Script is already running. */
      SCRIPT_NOTUNIQUE,
   };

   /** Run flags for scripts.
   *  \sa api_script */
   enum Script_RunFlags { 
      /** Only run the script if it isn't already running (with the same arguments). */
      SCRIPT_RUN_UNIQUE  = 0x1,
      /** If the script is already running (with the same arguments), kill it and replace it. */
      SCRIPT_RUN_REPLACE = 0x2,
      /** The script receives network events later than usual. */
      SCRIPT_RUN_LATE    = 0x4,
   };

   /** Network hide flags.
   *  \sa f2_hideline() */
   enum Hide_Flags {
      /** Prevent further filters from seeing the line. */
      HIDE_FILTERS = 0x1,
      /** Prevent further scripts from seeing the line. */
      HIDE_SCRIPTS = 0x2,
      /** Prevent further modules from seeing the line. */
      HIDE_MODULES = 0x4,
      /** Prevent furnarchy from seeing the line. */
      HIDE_WORLD = 0x8,
      /** Prevent the client from seeing the line. */
      HIDE_CLIENT = 0x10,
      /** Prevent anyone from seeing the line. */
      HIDE_ALL = 0x1F,
   };

   /** A little class to represent UUIDs.
   *  \sa f2_modfind() */
   struct UUID128
   {
      union
      {
         unsigned int  ints[ 4 ];
         unsigned char bytes[ 16 ];
         struct 
         {
            unsigned int   n1;
            unsigned short s2;
            unsigned short s3;
            unsigned char  b4[ 8 ];
         }  m;
      };

      UUID128( const char* sz = 0 ) {
         operator =( sz );      
      }

      UUID128& operator =( const char* sz )
      {
         if (sz)
         {
            unsigned int b = 0;
            char ch = *sz;

            for (size_t i = 0; i < 16; ++i)
            {
               b = 0;

               for (size_t j = 0; ch && j < 2; )
               {
                  switch (ch)
                  {
                  case '0': case '1': case '2': case '3': case '4': case '5':
                  case '6': case '7': case '8': case '9': 
                     b += (ch - '0') << ( (j++ ^ 0x1) << 2 );
                     break;
                  case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                     b += (ch - 'A' + 10) << ( (j++ ^ 0x1) << 2 );
                     break;
                  case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                     b += (ch - 'a' + 10) << ( (j++ ^ 0x1) << 2 );
                     break;
                  default:
                     break;
                  }
                  ch = *(++sz);
               }

               bytes[ i ] = b & 0xFF;
            }
         }
         else
         {
            ints[ 0 ] = ints[ 1 ] = ints[ 2 ] = ints[ 3 ] = 0;
         }

         return (*this);
      }

      UUID128& operator =( const UUID128& r )
      {
         ints[ 0 ] = r.ints[ 0 ];
         ints[ 1 ] = r.ints[ 1 ];
         ints[ 2 ] = r.ints[ 2 ];
         ints[ 3 ] = r.ints[ 3 ];
         return (*this);
      }

      bool operator ==( const UUID128& r ) {
         return (ints[ 0 ] == r.ints[ 0 ] && ints[ 1 ] == r.ints[ 1 ] &&
            ints[ 2 ] == r.ints[ 2 ] && ints[ 3 ] == r.ints[ 3 ]);
      }

      bool operator !=( const UUID128& r ) {
         return (ints[ 0 ] != r.ints[ 0 ] || ints[ 1 ] != r.ints[ 1 ] ||
            ints[ 2 ] != r.ints[ 2 ] || ints[ 3 ] != r.ints[ 3 ]);
      }

      bool operator !( )
      {
         return (!ints[ 0 ] && !ints[ 1 ] && !ints[ 2 ] && !ints[ 4 ]);
      }
   };

   /** Basic x,y coordinate structure. */
   struct POINT2
   {
      int x, y;

      bool operator !=( const POINT2& r )
      { return (x != r.x || y != r.y); }
      bool operator ==( const POINT2& r )
      { return (x == r.x && y == r.y); }
   };

   /** Version structure. */
   struct F2VERSION
   {
      unsigned int major;
      unsigned int minor;
      unsigned int revision;
   };

   /** Describes the player.
   *  \sa f2_myplayerinfo() */
   struct PLAYERINFO
   {
      /** Player name. */
      const char*  name;
      /** Player password. */
      const char*  pw;
      /** Player machine ID (scrambled). */
      const char*  mac_id;
      /** Player's description (logical). */
      const char*  desc;
      /** Player's color code. */
      const char*  colors;
      /** Player's GUID. */
      unsigned int guid;
      /** Player's position/camera position. */
      POINT2       cam;
   };

   /** Describes a monster.
   *  \sa f2_myplayerinfo() */
   struct MONSTERINFO
   {
      /** Monster GUID. */
      unsigned    guid;
      /** Monster name. */
      const char* name;
      /** Monster color string. */
      const char* color;
      /** Monster position. */
      POINT2      pos;
      /** Monster frame. */
      unsigned    frame;
      /** Flags.
      *  May be a combination of the following flags:
      *     - \c MONSTER_HASPROFILE The monster has a profile page. */
      unsigned    flags;
   };

   /** Describes a loaded module.
   *  \sa on_module MOD_EVENT_START */
   struct MODULEINFO
   {
      /** Module index number. */
      unsigned int mod;
      /** Module UUID (optional). */
      UUID128      uuid;
      /** Module state flags.
      *  This may be a combination of the following flags:
      *     - MOD_DISABLED  (mutually exclusive with MOD_ENABLED).
      *     - MOD_ENABLED   (mutually exclusive with MOD_DISABLED). */
      long         state;
      /** Module's directory name. */
      const char*  name;
      /** Module's fancy name (optional). */
      const char*  fancy_name;
      /** Absolute path to the module's root directory. */
      const char*  root;
      /** Absolute path to the module's thumbnail image (optional). */
      const char*  thumb_file;
   };

   /** Describes a render target.
   *  \sa on_client CL_EVENT_RENDER CL_EVENT_PRERENDER CL_EVENT_POSTRENDER */
   struct RENDERPARAMS
   {
      /** Device context of the target surface.
      *  The bitmap contained in the DC is actually a 24-bit
      *  top-down DIB which you can manipulate manually if you
      *  choose to. */
      HDC          dc;
      /** Width of the surface (should be 640 always). */
      unsigned int width;
      /** Width of the surface (should be 480 always). */
      unsigned int height;
   };

   /** Describes a windows message.
   *  These are the parameters typically passed to the furcadia window's
   *  window procedure.  For more on (WM_) windows messages, see
   *  the Windows Platform SDK documentation at 
   *  http://msdn.microsoft.com/library/en-us/winui/winui/windowsuserinterface/windowing/windows.asp?
   *  \sa on_wndproc */
   struct WNDPROCPARAMS
   {
      /** The window handle. */
      HWND    wnd;
      /** The window procedure message. */
      UINT    msg;
      /** The message wparam. */
      WPARAM  wparam;
      /** The message lparam. */
      LPARAM  lparam;
      /** The value to return if this message is handled by a module. */
      LRESULT retval;
   };

   //////////////////////////////////////////////////////////////////////////


   #ifndef F2LINK
      #define F2LINK extern "C"
   #endif

   #define F2EXPORT extern "C" __declspec(dllexport)

   #ifdef _MSC_VER
      #define F2CALL __cdecl
   #else
      #define F2CALL __attribute__((cdecl))
   #endif
   
   /** \defgroup api API Functions 
   * bam boom */
   /**\{*/
   /** \defgroup api_misc Miscellaneous Functions.
   *  This and that. */
   /**\{*/
   /** Hooks an API symbol by name or ordinal.
   *  \param[in]     lib         The DLL name.
   *  \param[in]     name        The symbol/function name.
   *  \param[in]     hook        Address to replace the symbol with.
   *  \param[in]   by_ordinal    \c true if \a name is actually of type \c int.
   *  \return The previous symbol address.
   *  \remarks When intercepting API functions, a well-behaved module should
   *           pass the unhandled call to the function returned by f2_hookapi(),
   *           because another module might have hooked the same function. To
   *           ensure every module gets its opportunity to handle the same
   *           hooked function, each must agree to forward unhandled calls to
   *           the next.*/
   F2LINK void* F2CALL f2_hookapi( const char* lib, const char* name,
                                   void* hook, bool by_ordinal );
   /** Retrieve the window handle of the main window.
   *  \remarks This is only valid after the window is created (duh).
   *           To know when the window is created, wait for the 
   *           CL_EVENT_WNDCREATED event.
   *  \sa CL_EVENT_WNDCREATED */
   F2LINK HWND F2CALL f2_getwnd( );
   /** Set the active dialog window.
   *  \param[in]     dlg      The active dialog window.
   *  \remarks This command no longer does anything. All windows are automatically
   *           granted dialog processing by Furnarchy unless they explicitly opt-out by
   *           responding to the F2M_ISNOTDLGWND message.
   *  \sa F2M_ISNOTDLGWND */
   F2LINK void F2CALL f2_setcurdialog( HWND dlg );
   /** Send a command line command through furnarchy.
   *  \param[in]     command     The command line command (minus the '@' prefix).
   *  \param[in]     noecho      Whether or not to echo the command in the chat buffer.
   *  \remarks Commands are queued, not processed immediately.
   *  \sa on_command */
   F2LINK void F2CALL f2_command( const char* command, bool noecho = false );
   /** Adds/refreshes a name in the autocomplete list.
   *  \param[in]  name  The name to add or refresh.
   *  \remarks If the name already exists in the list, it is refreshed by
   *           being promoted to the front of the list.
   */
   F2LINK void F2CALL f2_name_push( const char* name );
   /** Completes a name.
   *  \param[in,out]    name  String containing the start of the name, also receives
   *                          the completed name.
   *  \param[in]        size  The size of the \a name buffer, in bytes.
   *  \param[in]        iter  An iterator state value that provides for cycling
   *                          through matching names.
   *  \return The next \a iter state value, or -1 if the name wasn't matched.
   *  \remarks To cycle through all names, start with \a iter as 0 and use
   *           the return value for each successive call.
   */
   F2LINK int F2CALL f2_name_complete( char* name, std::size_t size, int iter );
   /** Converts a player name into a shortname.
   *  Shortnames are a character's "true" name. All non-alphanumeric characters
   *  are removed and accented characters are converted to their ascii equivalents.
   *  \param[in]     name        Long name.
   *  \param[out]    buf         Buffer to be filled with the shortname. sizeof( buf ) must be >= sizeof( \a name ). */
   F2LINK void F2CALL f2_makeshortname( const char* name, char* buf );


   /**\}*/   
   
   /** \defgroup api_net Network Functions.
   *  Facilitates sending and receiving of network data and 
   *  manipulating network lines being processed. */
   /**\{*/
   /** Retrieve the current game connection level.
   *  \return The connection level.
   *  \remarks This refers to the primary, game connection. */
   F2LINK Connection_Level F2CALL f2_connlvl( );
   /** Sends some bytes to the server.
   *  \param[in]     buf      Bytes to send.
   *  \param[in]     size     Number of bytes to send.
   *  \remarks Bytes are queued, not sent or processed immediately.
   *  \sa on_onet f2_netoutline() f2_netoutcount() */
   F2LINK void F2CALL f2_netout( const char* buf, std::size_t size );
   /** Sends a line to the server.
   *  \param[in]     line     Line to send.
   *  \param[in]     len      Length of the line.
   *  \return The line number.
   *  \remarks Lines are queued, not sent or processed immediately.
   *           The return value can be used to identify the line as it
   *           passes through on_onet.
   *  \sa on_onet f2_netout() f2_netoutcount() */
   F2LINK size_t F2CALL f2_netoutline( const char* line, std::size_t len );
   /** Returns the next outbound line number to be assigned.
   *  \return The next outbound line number.
   *  \sa on_onet f2_netout() f2_netoutline() */
   F2LINK size_t F2CALL f2_netoutcount( );
   /** Sends some bytes to the client.
   *  \param[in]     buf      Bytes to send.
   *  \param[in]     size     Number of bytes to send.
   *  \remarks Bytes are queued, not sent or processed immediately.
   *  \sa on_inet f2_netinline() f2_netincount() */
   F2LINK void F2CALL f2_netin( const char* buf, std::size_t size );
   /** Sends a line to the client.
   *  \param[in]     line     Line to send.
   *  \param[in]     len      Length of the line.
   *  \return The line number.
   *  \remarks Lines are queued, not sent or processed immediately.
   *           The return value can be used to identify the line as it
   *           passes through on_onet.
   *  \sa on_onet f2_netin() f2_netincount() */
   F2LINK size_t F2CALL f2_netinline( const char* line, std::size_t len );
   /** Returns the next inbound line number to be assigned.
   *  \return The next inbound line number.
   *  \sa on_onet f2_netin() f2_netinline() */
   F2LINK size_t F2CALL f2_netincount( );
   /** Modifies the current line being revised.
   *  \param[in]     line     A null-terminated line to replace the current line.
   *  \remarks The current line can be modified in-place during the REV_REVISING
   *           pass of the on_inet() or on_onet() event.
   *  \warning Once you revise or cancel a line, the data pointed to by the
   *           \a line string passed into the handler is in an undefined state.
   *           The address it points to may not even be valid anymore.
   *  \sa on_inet on_onet f2_cancelline() */
   F2LINK void F2CALL f2_reviseline( const char* line );
   /** Cancels the current line.
   *  \deprecated This function is only included for legacy modules and has been replaced by
   *              the more robust f2_hideline(). In fact, this function is essentially synonymous with
   *              <tt>f2_hideline( HIDE_ALL )</tt>.
   *  \sa on_inet on_onet f2_reviseline() f2_hideline() */
   F2LINK void F2CALL f2_cancelline( );
   /** Prevents the current line from being processed by further handlers/listeners in specific subsystems.
   *  \param[in]     hide_flags  A combination of the flags defined in the Hide_Flags enum below.
   *  In contrast to f2_cancelline(), this function does not modify the
   *  line, and allows you to control (by combining flags) which subsystems along 
   *  the chain will not receive the current line. Like f2_cancelline(), this 
   *  function may be called during any pass of on_inet or on_onet.
   *  To mimic f2_cancelline() behavior, simply use the HIDE_ALL flag.
   *  \remarks Currently, modules cannot hide a line from filters or (non-late) scripts, as
   *           the module subsystem is further down the chain.
   *           The full chain in which subsystems receive inbound and outbound lines is:
   *              - Filters
   *              - Scripts
   *              - Modules
   *              - Scripts (ran with --late option)
   *              - World
   *              - Client
   *  \sa Hide_Flags on_inet on_onet f2_reviseline() */
   F2LINK void F2CALL f2_hideline( int hide_flags );
   /** Retrieves the hide flags for the current line.
   *  \return The hide flags for the current line.
   *  \sa Hide_Flags f2_hideline()
   */
   F2LINK int F2CALL f2_hideflags( );
   /**\}*/
   
   /** \defgroup api_out Output Functions.
   *  Facilitates printing to the chat buffer and error log. */
   /**\{*/
   /** Prints a line of text to the chat buffer.
   *  \param[in]     line     A null-terminated string to print.
   *  \remarks Furnarchy will strip any non-readable characters
   *           from this, as well as trim it down to one line.
   *  \sa f2_errspeak() f2_errlog() */
   F2LINK void F2CALL f2_speak( const char* line );
   /** Prints an error line to the chat buffer.
   *  The line is printed to the chat buffer and then appended to
   *  Furnarchy's error log.
   *  \param[in]     line     A null-terminated string to print.
   *  \remarks This is equivalent to doing an f2_speak() followed by an 
   *           f2_errlog().
   *  \sa f2_errspeak() f2_errlog() */
   F2LINK void F2CALL f2_errspeak( const char* line );
   /** Appends some text to the error log.
   *  The error log is shared by all of Furnarchy at $FURNROOT\\core_err.log
   *  \param[in]     line     A null-terminated string to print.
   *  \sa f2_speak() f2_errspeak() */
   F2LINK void F2CALL f2_errlog( const char* line );
   /**\}*/
   
   /** \defgroup api_version Versioning Functions.
   *  Client CRCing, build checking, and Furnarchy versioning. */
   /**\{*/
   /** Checks if the furnarchy2 memlocs are valid for this client build.
   *  \return \c true if the memlocs are valid for this client build.
   *  \remarks Furnarchy simply checks if the CRC specified in the memlocs.ini
   *           file is the same as the exe's CRC.  If this fails, any
   *           build-dependent (memloc-dependent) interfaces may be unavailable
   *           or limited.
   *  \sa f2_clcrc(), adv */
   F2LINK bool F2CALL f2_buildok( );
   /** Retrieves the Furcadia client's CRC32.
   *  \return The standard PKZIP CRC32 of the client's disk image. */
   F2LINK unsigned int F2CALL f2_clcrc( );
   /** Retrieves Furnarchy's version.
   *  \param[out] out  Structure to be filled with version information.
   *  \return Furnarchy's version. */
   F2LINK void F2CALL f2_version( F2VERSION* out );
   /**\}*/
   
   /** \defgroup api_fs Filesystem Functions.
   *  Get directory paths. */
   /**\{*/
   /** Checks whether Furnarchy is running in single user mode.
   *  \return \c true if running in single user mode.
   *  \remarks In Single user mode, the user has indicated his preference
   *           to save all per-user program configuration in the programs folder,
   *           like in pre-Vista mode.
   */
   F2LINK bool F2CALL f2_singleusermode( );
   /** Retrieves Furnarchy's program directory.
   *  \return An absolute path to the furnarchy program directory.
   *  \sa f2_dataroot() f2_docroot() */
   F2LINK const char* F2CALL f2_root( );
   /** Retrieves Furnarchy's data directory.
   *  \return An absolute path to the furnarchy data directory.
   *  \remarks This is the folder located in the per-user "Application Data" or "AppData" folder.
   *  \warning If furnarchy is running in single user mode, this directory will be the same as
   *           f2_root().
   *  \sa f2_root() f2_docroot() */
   F2LINK const char* F2CALL f2_dataroot( );
   /** Retrieves Furnarchy's document directory.
   *  \return An absolute path to the furnarchy document directory.
   *  \remarks This is the folder located in the per-user "My Documents" or "Documents" folder.
   *  \warning If furnarchy is running in single user mode, this directory will be the same as
   *           f2_root().
   *  \sa f2_root() f2_dataroot() */
   F2LINK const char* F2CALL f2_docroot( );
   /** Retrieves the client's program directory.
   *  \return An absolute path to the client's program directory.
   *  \sa f2_cldocroot() f2_cldataroot() */
   F2LINK const char* F2CALL f2_clroot( );
   /** Retrieves the client's data directory.
   *  \return An absolute path to the client's data directory.
   *  \remarks This is the folder located in the computer-specific (non-roaming), 
   *           per-user "Application Data" or "AppData" folder.
   *  \sa f2_clroot() f2_cldocroot() */
   F2LINK const char* F2CALL f2_cldataroot( );
   /** Retrieves the client's document directory.
   *  \return An absolute path to the client's document directory.
   *  \remarks This is the folder located in the per-user "My Documents" or "Documents" folder.
   *  \sa f2_clroot() f2_cldataroot() */
   F2LINK const char* F2CALL f2_cldocroot( );
   /** Creates a directory hierarchy.
   *  \return non-zero on success.
   *  \sa f2_clroot() f2_cldataroot() */
   F2LINK bool F2CALL f2_createdirs( const char* path );
   /**\}*/
   
   /** \defgroup api_mod Module Functions.
   *  Facilitates module manipulation and communication.
   *  Many of these require a module index as a parameter.
   *  To retrieve your own module index, copy it from the \c MOD_EVENT_START
   *  event in on_module(). */
   /**\{*/
   /** Counts the number of loaded modules.
   *  \return The number of modules loaded.
   *  \remarks Because module unloading is unsupported
   *           in F2 at the time of this writing, you can rely on the 
   *           return value of f2_modcount() to remain the same for the
   *           lifetime of the program. */
   F2LINK std::size_t F2CALL f2_modcount( );
   /** Retrieves a module's program directory.
   *  \param[in]     mod      The module's index.
   *  \return An absolute path to the module's program directory.
   *  \sa f2_moddataroot() f2_moddocroot() */
   F2LINK const char* F2CALL f2_modroot( unsigned int mod );
   /** Retrieves a module's data directtory.
   *  \param[in]     mod      The module's index.
   *  \return An absolute path to the module's data directory.
   *  \remarks This is located in the per-user "Application Data" or "AppData" directory.
   *           Per-user files a module creates that it doesn't want to expose to the user go here. 
   *  \warning If furnarchy is running in single user mode, this directory will be the same as
   *           f2_modroot().
   *  \sa f2_modroot() f2_moddocroot() */
   F2LINK const char* F2CALL f2_moddataroot( unsigned int mod );
   /** Retrieves a module's documents directtory.
   *  \param[in]     mod      The module's index.
   *  \return An absolute path to the module's documents directory.
   *  \remarks This is located in the per-user "My Documents" or "Documents" directory.
   *           Per-user files a module creates that it wants to expose to the user go here.
   *  \warning If furnarchy is running in single user mode, this directory will be the same as
   *           f2_modroot().
   *  \sa f2_modroot() f2_moddataroot() */
   F2LINK const char* F2CALL f2_moddocroot( unsigned int mod );
   /** Retrieves a module's name.
   *  This is just the module's directory name, as opposed to its
   *  "fancy name."
   *  \param[in]     mod      The module's index.
   *  \return The module's name.
   *  \remarks The module name can be used to reference the module
   *           via '\@mod*' command line commands.
   *  \sa f2_modfancyname() */
   F2LINK const char* F2CALL f2_modname( unsigned int mod );
   /** Retrieves a module's fancy name, as opposed to its "real name."
   *  This is the name set in the module's module.ini file.
   *  \param[in]     mod      The module's index.
   *  \return The module's fancy name.
   *  \remarks The fancy name is used for purely aesthetic reasons.
   *  \sa f2_modname() */
   F2LINK const char* F2CALL f2_modfancyname( unsigned int mod );
   /** Retrieves a module's thumbnail image path.
   *  This is the path specified by the module's module.ini file, and
   *  defaults to 'thumbnail.bmp' if not.
   *  \param[in]     mod      The module's index.
   *  \return An absolute path to the module's thumbnail image.
   *  \remarks The module's thumbnail image should be of a common
   *           image format (jpeg, gif, tga, bmp, png, etc.) and 
   *           32x32.  This is typically used by skins. */
   F2LINK const char* F2CALL f2_modthumb( unsigned int mod );
   /** Retrieves a module's state.
   *  \param[in]     mod      The module's index.
   *  \return A combination of the following flags:
   *          - MOD_DISABLED (mutually exclusive with MOD_ENABLED)
   *          - MOD_ENABLED  (mutually exclusive with MOD_DISABLED)
   *  \remarks Yeah, there's not much variation right now, but that could
   *           change in the future.  You can modify these flags via
   *           the f2_modenable() function.
   *  \sa f2_modenable() */
   F2LINK unsigned long F2CALL f2_modstate( unsigned int mod );
   /** Enables or disables a module.
   *  \param[in]     mod      The module's index.
   *  \param[in]     onoff    \c true to enable a module.
   *  \remarks This will trigger a MOD_EVENT_ENABLE or MOD_EVENT_DISABLE 
   *           in the target module, but only if the module isn't
   *           already in that state.
   *           The enabled or disabled state is merely a suggestion for a
   *           module to restrict or unleash its full functionality.
   *           Furnarchy doesn't inhibit the module in any way just because
   *           it's in a disabled state.
   *  \sa f2_modstate() */
   F2LINK void F2CALL f2_modenable( unsigned int mod, bool onoff );
   /** Looks for a module by UUID.
   *  The UUID is specified in the module's module.ini as an alternate
   *  means of locating it.
   *  \param[in]     uuid     The module's UUID.
   *  \return The module's index number, or -1 if the module wasn't found.
   *  \remarks A module can define a UUID if it intends to expose an
   *           interface to its neighbors (via f2_modmsg() or what have you).  
   *           The UUID is the preferred method of locating such a module 
   *           because the module can easily change its UUID to indicate 
   *           new/incompatible interfaces.
   *  \sa f2_modmsg() */
   F2LINK unsigned int F2CALL f2_modfind( const UUID128* uuid );
   /** Sends a module a touch event.
   *  \param[in]     mod      The module's index.
   *  \remarks The touch event has historically been a way of asking the
   *           module to expose or display more information. */
   F2LINK void F2CALL f2_modtouch( unsigned int mod );
   /** Sends a module a message.
   *  Sends a module some module-specific data.
   *  \param[in]      mod     The target module's index.
   *  \param[in,out]  data    Module-specific data.
   *  \return The target module's response, or 0 if the target doesn't exist.
   *  \remarks If f2_modmsg() seems a little vague to you, that's the point.
   *           It only provides a very general method of transporting \a data
   *           between modules.  What the \a data is or means is something
   *           that must be agreed upon between the caller and the target.
   *  \sa f2_modfind() */
   F2LINK int F2CALL f2_modmsg( unsigned int mod, void* data );
   /**\}*/
   
   /** \defgroup api_world World Functions.
   *  Query and manipulate the game world. */
   /**\{*/
   /** Gets information on the (host) player.
   *  \param[out]    out      PLAYERINFO structure to be filled with player data.
   *  \warning The strings returned in \a out have an indefinite lifespan 
   *           and should be copied somewhere if you intend to refer 
   *           to them at a later time.
   *  \sa PLAYERINFO */
   F2LINK void F2CALL f2_myplayerinfo( PLAYERINFO* out );
   /** Gets information on a monster (avatar).
   *  \param[in]     guid     Monster GUID.
   *  \param[out]    out      MONSTERINFO structure to be filled with player data.
   *  \warning The strings returned in \a out have an indefinite lifespan 
   *           and should be copied somewhere if you intend to refer 
   *           to them at a later time.
   *  \remarks If successful, the guid contained in \a out will be non-zero.
   *  \sa MONSTERINFO */
   F2LINK void F2CALL f2_monsterinfo( unsigned int guid, MONSTERINFO* out );
   /** Gets information on a monster (avatar) with a name.
   *  \param[in]     name     Monster name.
   *  \param[out]    out      MONSTERINFO structure to be filled with player data.
   *  \param[in]     by_shortname     true if searching by shortname.
   *  \warning The strings returned in \a out have an indefinite lifespan 
   *           and should be copied somewhere if you intend to refer 
   *           to them at a later time.
   *  \remarks The name search is done in a case-sensitive manner.
   *  \remarks If successful, the guid contained in \a out will be non-zero.
   *  \sa MONSTERINFO */
   F2LINK void F2CALL f2_monsterinfo_byname( const char* name, MONSTERINFO* out, bool by_shortname = false );
   /** Retrieves the GUIDs of all existing monsters.
   *  \param[out]    guids    Integer array to be filled with GUIDs. May be NULL.
   *  \param[in]     len      Length of the \a guids array.
   *  \return The number of guids copied to \a guids.
   *  \remarks If \a guids is NULL, nothing is copied and the total number of existing
   *           monsters is returned.
   *  \sa f2_monsterinfo() */
   F2LINK unsigned int F2CALL f2_monsters_enum( unsigned int* guids, unsigned int len );
   /** Check if a monster is visible.
   *  \param[in]    guid    GUID of the monster to check.
   *  \return \c true if the monster is visible. \c false if not visible or doesn't exist.
   *  \remarks A monster is visible if an "avatar show" server command has been received.
   *           An "avatar hide" server command will mark the monster invisible.
   *  \sa f2_monsterinfo() */
   F2LINK bool F2CALL f2_monstervisible( unsigned int guid );
   /** Cancels the last badge/specitag from being displayed.
   *  \remarks You should call this whenever you cancel chat text that
   *           typically has a badge displayed before it (speech, whispers, etc.),
   *           or if you want to make sure no badge is displayed before the current
   *           text.
   */
   F2LINK void F2CALL f2_badge_cancel( );

   /** \defgroup desctags Desctags Functions.
   *  Manipulate description tags.
   *  Oftentimes modules or scripts will want to append some data to the end of
   *  the player's description.  However, if you have multiple modules or scripts
   *  trying to tag/untag the player's desc at the same time, you usually
   *  end up with a mess as each module presents what -it- thinks was the original
   *  description.
   *
   *  To solve this problem, Furnarchy makes a logical separation between the
   *  player's description and any tags scripts or modules want to append
   *  to it.  When the client sends a `desc or `chdesc command, 
   *  then Furnarchy appends the tags to the description.
   *
   *  By agreeing to go through Furnarchy for this sort of stuff, modules can
   *  tag the desc with whatever they'd like without stepping on each other's
   *  toes, and there's no need to track/restore the original description
   *  as Furnarchy will handle it all for you.
   *
   *  To use desctags, modules have to register for a desctag slot with
   *  f2_desctag_reg(), then use f2_desctag_set() to set its contents.
   *  f2_desctag_unreg() should be called when the tag is no longer needed. */
   /**\{*/
   /** Registers a desctag slot.
   *  The created desctag can be set by f2_desctag_set().
   *  \return A nonzero unique desctag id.
   *  \remarks Desctags slots are handed out order-wise on a first-come 
   *           first-serve basis.
   *           You should free the desctag slot with f2_desctag_unreg()
   *           when you no longer need it.
   *  \sa f2_desctag_set() f2_desctag_unreg() */
   F2LINK int F2CALL f2_desctag_reg( );
   /** Unregisters a desctag slot.
   *  \param[in]     tag_id      A unique tag identifier previously returned by
   *                             a f2_desctag_reg() call.
   *  \return \c true if \a tag_id was successfully unregistered.
   *  \remarks This may result in an immediate `desc or `chdesc command
   *           being sent to the server to reflect the restored desc.
   *  \sa f2_desctag_reg() f2_desctag_set() */
   F2LINK bool F2CALL f2_desctag_unreg( int tag_id );
   /** Sets the contents of a desctag slot.
   *  \param[in]     tag_id      A unique tag identifier previously used in a
   *                             successful f2_desctag_reg() call.
   *  \param[in]     tag         A string to set the slot to.
   *  \remarks \a tag will be trimmed to one line and and cleaned
   *           to be human-readable.
   *           This may result in an immediate `desc or `chdesc command
   *           being sent to the server to reflect the updated desc.
   * \sa f2_desctag_reg() f2_desctag_unreg() */
   F2LINK bool F2CALL f2_desctag_set( int tag_id, const char* tag );
   /**\}*/
   /**\}*/

   /** \defgroup adv Advanced Functions.
   *  Advanced functions that depend on code and memory hacks.
   *  With these methods, you can manipulate aspects the client and its
   *  behavior.  Be warned; because these require intimate interaction
   *  with the client, they are guaranteed to cease working if a new
   *  client build is released or memlocs don't match up.  To test for
   *  this condition, see f2_buildok().
   */
   /**\{*/
   /** Sets internal player information.
   *  \param[in]     name     Player name (optional if NULL).
   *  \param[in]     pw       Player password (optional if NULL).
   *  \param[in]     color    Player (complete, 13 byte) color code (optional if NULL).
   *  \param[in]     desc     Player description (optional if NULL).
   *  \param[in]     guid     Player GUID (optional if NULL).
   *  \remarks When the client changes maps, the client restores the
   *           portrait area to show the current player, based on these
   *           values.  The name and password may also be used in
   *           opening fileserver connections.\n\n
   *           Passing a parameter as NULL will leave it unchanged in the client.\n\n
   *           Changing these values will not affect the player information
   *           stored by furnarchy and returned by the world functions.
   */
   F2LINK void F2CALL f2__setplayerinfo( const char* name, const char* pw,
                                         const char* color, const char* desc,
                                         const unsigned int* guid );
   /** Replaces the client's main connection socket handle.
   *  \param[in]     socket   Replacement socket handle.
   *  \remarks The old socket will be restored to blocking IO mode, but not
   *           shutdown or closed. Be sure to properly deallocate the old
   *           socket yourself. \n\n
   *           The new socket will be immediately WSAAsyncSelected into
   *           asynchronous mode for the client, and an FD_READ event
   *           will be posted to the message queue.
   *  \warning Care must be taken to make sure the sockets are in the same
   *           connection level.  That is, they're both waiting for the MOTD
   *           text to arrive, have received the MOTD text, or are logged in.
   *           Replacing the current socket with one in a different connection
   *           level could have strange results. */
   F2LINK void F2CALL f2__setmainsocket( SOCKET socket );

   /** Pushes a line of text into the chat buffer.
   *  \param[in]     line     The line of text.
   *  \remarks This should always work, regardless of the client build, in some way. */
   F2LINK void F2CALL f2__pushchatbox( const char* line );
   /** Adds an ignore.
   *  \param[in]  name  Name to ignore. If \a name starts with a '@', indicates a channel.
   *  \return true if successful. */
   F2LINK bool F2CALL f2__ignore_add( const char* name );
   /** Removes an ignore.
   *  \param[in]  name  Name to unignore. If \a name starts with a '@', indicates a channel.
   *  \return true if successful. */
   F2LINK bool F2CALL f2__ignore_rm( const char* name );
   /** Tests if a name is ignored.
   *  \param[in]  name  Name to test. If \a name starts with a '@', indicates a channel.
   *  \return true if ignored. */
   F2LINK bool F2CALL f2__ignore_test( const char* name );
   /** Retrieves the player shortname associated with the last player menu popup.
   *  \return  A shortname.
   *  \remarks The lifetime of the buffer returned by this function is indefinite, thus
   *           the string should be copied immediately if you intend to reference it
   *           at a later time.
   *  \sa api_menu
   */
   F2LINK const char* F2CALL f2__playermenu_name( );
   /**\}*/

   /** \defgroup api_script Script Functions.
   *  These functions allow the manipulation of scripts.
   */
   /**\{*/
   /** Enumerates the script ids of all running scripts.
   *  \param[out]  sids    int array to fill with script ids.
   *  \param[in]   len     Length of the \a sids array.
   *  \return  The number of sids copied to the \a sids array.
   *  \remarks If \a sids is NULL, nothing is copied and the function
   *           returns the number of running scripts.
   */
   F2LINK unsigned int F2CALL f2_script_enum( unsigned int* sids, unsigned int len );
   /** Retrieves the run arguments and flags for a running script.
   *  \param[in]   sid    The script ID.
   *  \param[out]  argv   Array of CONSTANT pointers to fill with string addresses.
   *  \param[in]   len    Length of the \a argv array.
   *  \param[out]  flags  int buffer to be filled with run flags. May be NULL.
   *  \return  The number of pointers copied to the \a argv array.
   *  \remarks If \a argv is NULL, the function copies the flags and returns the number of total arguments.
   */
   F2LINK unsigned int F2CALL f2_script_argv( unsigned int sid, const char** argv, unsigned int len, int* flags );
   /** Runs a script.
   *  \param[in]  argv   Run arguments.
   *  \param[in]  len    Length of the \a argv array.
   *  \param[in]  flags  Run flags.
   *  \param[out] sid    int buffer to be filled with the created script's script id. May be NULL.
   *  \return  An error code.
   *  \remarks The first string in the \a argv array is the script's name.
   *           All other strings are arguments passed to the script when it executes.
   *           If the SCRIPTRUN_UNIQUE flag is set and a copy of the script with the
   *           same arguments is already running, a SCRIPT_NOTUNIQUE error will be returned.
   *  \remarks Note that you won't always get a SCRIPT_FAILED error when the script fails
   *           because scripts are only loaded and executed immediately if the player is in
   *           a logged in state. All other times, they are suspended.
   *  \sa Script_Error Script_RunFlags
   */
   F2LINK Script_Error F2CALL f2_script_run( const char** argv, unsigned int len, int flags, unsigned int* sid );
   /** Kills a running script.
   *  \param[in]  sid   The script ID.
   *  \return  An error code.
   *  \sa Script_Error
   */
   F2LINK Script_Error F2CALL f2_script_kill( unsigned int sid );
   /**\}*/

   /** \defgroup api_menu Menu Functions.
   *  These functions allow the manipulation of a player's popup menu.
   */
   /**\{*/
   /** Sets a module's submenu in the player's popup menu.
   *  \param[in]  mod   Module index.
   *  \param[in]  menu Submenu.
   *  \return  \c true on success.
   *  \sa f2__playermenu_name()
   */
   F2LINK bool F2CALL f2_playermenu_set( unsigned int mod, HMENU menu );
   /** Unsets a module's submenu in the player's popup menu.
   *  \param[in]   mod  Module index.
   *  \return \c true on success.
   \sa f2__playermenu_name()
   */
   F2LINK bool F2CALL f2_playermenu_unset( unsigned int mod );
   /**\}*/

   /**\}*/

   /**\}*/

} // namespace furn2

#endif
