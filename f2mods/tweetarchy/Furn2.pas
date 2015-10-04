unit Furn2;
//Converted from Cluracan's original furn2.h by Merc
//With Assistance from Remy Lebeau and Rudy Velthius. Thanks!

//Currently created against F2.3-r0 header

//{$Z+}//For older Delphi Versions
{$MINENUMSIZE 4}
{$ALIGN 4}

{$IFDEF FPC}
//FreePascal Setting for compatibility
{$MODE DELPHI}
{$ENDIF}
//#define F2EXPORT extern "C" __declspec(dllexport)
//cdecl calling spec
interface
uses
 Windows, winsock;

type
 size_t = longword; //Duplication of C definition
 index_t = size_t;


type
 //Network Revision Pass
 Revision = (REV_DRAFT, REV_REVISING, REV_FINAL);
 Connection_Level =(CONNLVL_DISCONNECTED = 0, CONNLVL_CONNECTED = 32,
                           CONNLVL_MOTDEND = 64, CONNLVL_LOGGEDIN = 128);


 {Module events.
   *  These events are received through a module's on_module() event handler.
   *  The event determines the type and meaning of on_module()'s "param"
   *  parameter and return value.}
 Module_Event = (
  MOD_EVENT_START,
  MOD_EVENT_STOP,
  MOD_EVENT_ENABLE,
  MOD_EVENT_DISABLE,
  MOD_EVENT_TOUCH,
  MOD_EVENT_MSG);

 Client_Event = (
  CL_EVENT_WNDCREATED,
  CL_EVENT_SCRLOAD,
  CL_EVENT_SOCKSWAP,
  CL_EVENT_PRERENDER,
  CL_EVENT_RENDER,
  CL_EVENT_POSTRENDER,
  CL_EVENT_TICK,
  CL_EVENT_CHATBOX,
  CL_EVENT_PLAYERMENUSHOW,
  CL_EVENT_SCRIPT);

 World_Event = (
  WO_EVENT_CONNECTED,
  WO_EVENT_DISCONNECTED,
  WO_EVENT_LOGGEDIN,
  WO_EVENT_PLAYERINFO,
  WO_EVENT_MONSTER);

 Client_Screen = (
  SCREEN_TITLE,
  SCREEN_PICKEM,
  SCREEN_BORDER,
  SCREEN_MARBLED);

  Script_Error = (
   //** No error. */
   SCRIPT_OK,
   //** A bad argument was passed. */
   SCRIPT_BADARG,
   //** Script file failed to load/run. */
   SCRIPT_FAILED,
   //** The script ID is bad. */
   SCRIPT_BADSID,
   //** Script is already running. */
   SCRIPT_NOTUNIQUE );

  Script_RunFlags = (
   //** Only run the script if it isn't already running (with the same arguments). */
   SCRIPT_RUN_UNIQUE  = $01,
   //** If the script is already running (with the same arguments), kill it and replace it. */
   SCRIPT_RUN_REPLACE = $02,
   //** The script receives network events later than usual. */
   SCRIPT_RUN_LATE    = $04 );

type
 UUID128 = packed record
  case Integer of
   0: (ints: array[0..3] of Cardinal);
   1: (bytes: array[0..15] of Byte);
   2: (n1: Cardinal;
       s2: Word;
       s3: Word;
       b4: array[0..7] of Byte);
  end;
 PUUID128 = ^UUID128;
//UUID128 Functions
procedure InitUUID128(var id: UUID128; sz: PChar = nil);
procedure AssignUUID128(var id: UUID128; sz: PChar);
procedure CopyUUID128(var id: UUID128; const r: UUID128);
function IsEqualUUID128(const id, r: UUID128): Boolean;
function IsNotEqualUUID128(const id, r: UUID128): boolean;
function IsNotValidUUID128(const id: UUID128) : boolean;

{type COLORCODE = packed record
 case integer of
 0: (a: array [0..12] of char);
 1: (fur: char;
     markings: char;
     hair: char;
     eyes: char;
     badge: char;
     vest: char;
     bracers: char;
     cape: char;
     boots: char;
     trousers: char;
     gender: char;
     species: char;
     digo: char;
    );
 end; }
//functions still need to be converted for COLORCODE
(* COLORCODE( ) {
         memset( a, ' ', sizeof( a ) );
      }
      COLORCODE( const char a[] ) {
         memcpy( COLORCODE::a, a, 13 );
      }
      COLORCODE& operator =( const char a[] ) {
         memcpy( COLORCODE::a, a, 13 );
         return *this;
      }
      void setColors( const char a[] ) {
         memcpy( COLORCODE::a, a, 10 );
      }
      void setSpecies( const char a[] ) {
         memcpy( COLORCODE::a + 10, a, 3 );
      }
      void clear( ) {
         memset( a, ' ', 13 );
      }
      char& operator []( size_t i ) {
         return a[ i ];
      }
      const char& operator []( size_t i ) const {
         return a[ i ];
      } *)

//struct POINT2 { int x, y; };
type
 PPChar = ^PChar;
 PCardinal = ^Cardinal;
 
 POINT2 = record
          x, y : integer;
          end;

 PLAYERINFO = record
              name: pChar;
              pw: pChar;
              mac_id: pChar;
              desc: pChar;
              //colors: COLORCODE;
              colors: pChar;
              guid: cardinal;
              cam: POINT2;
              end;
 PPlayerInfo= ^PLAYERINFO;

 MONSTERINFO = record
               guid: cardinal;
               name: pChar;
               color: pChar;
               pos: POINT2;
               frame: cardinal;
               end;
 PMonsterInfo = ^MONSTERINFO;
 
 MODULEINFO = record
              module: cardinal;  //mod
              uuid: UUID128;
              state: longint;
              name: pChar;
              fancy_name: pChar;
              root: pChar;
              thumb_file: pChar;
              end;
 PModuleInfo = ^MODULEINFO;

 RENDERPARAMS = record
                dc: hdc;
                width, height: cardinal;
                end;
 PRenderParams = ^RENDERPARAMS;

 WNDPROCPARAMS = record
                 wnd: HWND;
                 msg: UINT;
                 wparam: WPARAM;
                 lparam: LPARAM;
                 retval: LRESULT;
                 end;
 PWNDPROCPARAMS = ^WNDPROCPARAMS;

 { /** Version structure. */}
 F2VERSION = record
             major: cardinal;
             minor: cardinal;
             revision: cardinal;
             end;
 PF2VERSION = ^F2VERSION;

const
//enum { MOD_DISABLED = 0x0, MOD_ENABLED = 0x1 };
 MOD_DISABLED = $00;
 MOD_ENABLED = $01;
//enum { FROM_SERVER = 0, FROM_CLIENT = 0, FROM_GENERATED = 1 };
 FROM_SERVER = 0;
 FROM_CLIENT = 0;
 FROM_GENERATED = 1;

{   /** Hooks an API symbol by name or ordinal.
   *  \param[in]     lib         The DLL name.
   *  \param[in]     name        The symbol/function name.
   *  \param[in]     hook        Address to replace the symbol with.
   *  \param[in]   by_ordinal    \c true if \a name is actually of type \c int.
   *  \return The previous symbol address.
   *  \remarks When intercepting API functions, a well-behaved module should
   *           pass the unhandled call to the function returned by f2_hookapi,
   *           because another module might have hooked the same function. To
   *           ensure every module gets its opportunity to handle the same
   *           hooked function, each must agree to forward unhandled calls to
   *           the next.*/}
procedure f2_hookapi(lib, name: pChar; hook: pointer; by_ordinal: bool); cdecl; external 'furn2.dll';
{ /** Retrieve the window handle of the main window.
   *  \remarks This is only valid after the window is created (duh).
   *           To know when the window is created, wait for the
   *           CL_EVENT_WNDCREATED event.
   *  \sa CL_EVENT_WNDCREATED */}
function f2_getwnd: HWND; cdecl; external 'furn2.dll';
{   /** Set the active dialog window.
   *  \param[in]     dlg      The active dialog window.
   *  \remarks If you want the typical dialog processing to be done
   *           for your dialog window, you should pass your window's
   *           handle into this function when your window is
   *           activated via WM_ACTIVATE.  It's also a good idea to
   *           pass NULL to this if your window is being deactivated. */}
procedure f2_setcurdialog(dlg: HWND); cdecl; external 'furn2.dll';
{   /** Send a command line command through furnarchy.
   *  \param[in]     command     The command line command (minus the '@' prefix).
   *  \param[in]     noecho      Whether or not to echo the command in the chat buffer.
   *  \remarks Commands are queued, not processed immediately.
   *  \sa on_command */}
procedure f2_command(command: pchar; noecho: bool = false); cdecl; external 'furn2.dll';
{/** Adds/refreshes a name in the autocomplete list.
   *  \param[in]  name  The name to add or refresh.
   *  \remarks If the name already exists in the list, it is refreshed by
   *           being promoted to the front of the list.
   */}
procedure f2_name_push(name: pChar); cdecl; external 'furn2.dll';
   {/** Completes a name.
   *  \param[in,out]    name  String containing the start of the name, also receives
   *                          the completed name.
   *  \param[in]        size  The size of the \a name buffer, in bytes.
   *  \param[in]        iter  An iterator state value that provides for cycling
   *                          through matching names.
   *  \return The next \a iter state value, or -1 if the name wasn't matched.
   *  \remarks To cycle through all names, start with \a iter as 0 and use
   *           the return value for each successive call.
   */}
function f2_name_complete(name: pchar; size: size_t; iter: integer): integer;  cdecl; external 'furn2.dll';
   {/** Converts a player name into a shortname.
   *  Shortnames are a character's "true" name. All non-alphanumeric characters
   *  are removed and accented characters are converted to their ascii equivalents.
   *  \param[in]     name        Long name.
   *  \param[out]    buf         Buffer to be filled with the shortname. sizeof( buf ) must be >= sizeof( \a name ). */
   F2LINK void F2CALL f2_makeshortname( const char* name, char* buf ); }
 //F2LINK void F2CALL f2_makeshortname( const char* name, char* buf );
procedure f2_makeshortname(name: pchar; buf: pchar); cdecl; external 'furn2.dll';

//Network Functions
   {/** Retrieve the current game connection level.
   *  \return The connection level.
   *  \remarks This refers to the primary, game connection. */ }
function f2_connlvl(): connection_level; cdecl; external 'furn2.dll';
  {/** Sends some bytes to the server.
   *  \param[in]     buf      Bytes to send.
   *  \param[in]     size     Number of bytes to send.
   *  \remarks Bytes are queued, not sent or processed immediately.
   *  \sa on_onet f2_netoutline f2_netoutcount */}
procedure f2_netout(buf: pchar; size: size_t); cdecl; external 'furn2.dll';
   {/** Sends a line to the server.
   *  \param[in]     line     Line to send.
   *  \param[in]     len      Length of the line.
   *  \return The line number.
   *  \remarks Lines are queued, not sent or processed immediately.
   *           The return value can be used to identify the line as it
   *           passes through on_onet.
   *  \sa on_onet f2_netout f2_netoutcount */ }
function f2_netoutline(line : pchar; len: size_t): size_t; cdecl; external 'furn2.dll';
   { /** Returns the next outbound line number to be assigned.
   *  \return The next outbound line number.
   *  \sa on_onet f2_netout f2_netoutline */}
function f2_netoutcount(): size_t; cdecl; external 'furn2.dll';
{   /** Sends some bytes to the client.
   *  \param[in]     buf      Bytes to send.
   *  \param[in]     size     Number of bytes to send.
   *  \remarks Bytes are queued, not sent or processed immediately.
   *  \sa on_inet f2_netinline f2_netincount */}
procedure f2_netin(buf: pchar; size_t: size); cdecl; external 'furn2.dll';
{   /** Sends a line to the client.
   *  \param[in]     line     Line to send.
   *  \param[in]     len      Length of the line.
   *  \return The line number.
   *  \remarks Lines are queued, not sent or processed immediately.
   *           The return value can be used to identify the line as it
   *           passes through on_onet.
   *  \sa on_onet f2_netin f2_netincount */  }
function f2_netinline(line : pchar; len: size_t): size_t; cdecl; external 'furn2.dll';
{   /** Returns the next inbound line number to be assigned.
   *  \return The next inbound line number.
   *  \sa on_onet f2_netin f2_netinline */}
function f2_netincount(): size_t; cdecl; external 'furn2.dll';
{   /** Modifies the current line being revised.
   *  \param[in]     line     A null-terminated line to replace the current line.
   *  \remarks The current line can be modified in-place during the REV_REVISING
   *           pass of the on_inet() or on_onet() event.
   *  \warning Once you revise or cancel a line, the data pointed to by the
   *           \a line string passed into the handler is in an undefined state.
   *           The address it points to may not even be valid anymore.
   *  \sa on_inet on_onet f2_cancelline */}
procedure f2_reviseline(line: pchar); cdecl; external 'furn2.dll';
{   /** Cancels the current line being revised.
   *  \remarks This is equivalent to performing a f2_reviseline( "" ) during
   *           the REV_REVISING pass of the on_inet() or on_onet() event.
   *  \warning Once you revise or cancel a line, the data pointed to by the
   *           \a line string passed into the handler is in an undefined state.
   *           The address it points to may not even be valid anymore.
   *  \sa on_inet on_onet f2_reviseline */}
procedure f2_cancelline(); cdecl; external 'furn2.dll';


//Output Functions
{   /** Prints a line of text to the chat buffer.
   *  \param[in]     line     A null-terminated string to print.
   *  \remarks Furnarchy will strip any non-readable characters
   *           from this, as well as trim it down to one line.
   *  \sa f2_errspeak f2_errlog */}
procedure f2_speak(line : pchar); cdecl; external 'furn2.dll';
{   /** Prints an error line to the chat buffer.
   *  The line is printed to the chat buffer and then appended to
   *  Furnarchy's error log.
   *  \param[in]     line     A null-terminated string to print.
   *  \remarks This is equivalent to doing an f2_speak() followed by an
   *           f2_errlog().
   *  \sa f2_errspeak f2_errlog */}
procedure f2_errspeak(line: pchar); cdecl; external 'furn2.dll';
{   /** Appends some text to the error log.
   *  The error log is shared by all of Furnarchy at $FURNROOT\\core_err.log
   *  \param[in]     line     A null-terminated string to print.
   *  \sa f2_speak f2_errspeak */}
procedure f2_errlog(line: pchar); cdecl; external 'furn2.dll';

//Versioning Functions
{   /** Checks if the furnarchy2 memlocs are valid for this client build.
   *  \return \c true if the memlocs are valid for this client build.
   *  \remarks Furnarchy simply checks if the CRC specified in the memlocs.ini
   *           file is the same as the exe's CRC.  If this fails, any
   *           build-dependent (memloc-dependent) interfaces may be unavailable
   *           or limited.
   *  \sa f2_clcrc */}
function f2_buildok(): bool; cdecl; external 'furn2.dll';
{   /** Retrieves the Furcadia client's CRC32.
   *  \return The standard PKZIP CRC32 of the client's disk image. */ }
function f2_clcrc(): cardinal; cdecl; external 'furn2.dll';
{   /** Retrieves Furnarchy's version.
   *  \param[out] out  Structure to be filled with version information.
   *  \return Furnarchy's version. */}
//function f2_version(): cardinal; cdecl; external 'furn2.dll';
procedure f2_version(version : PF2VERSION); cdecl; external 'furn2.dll';

//Filesystem Functions
{/** Checks whether Furnarchy is running in single user mode.
   *  \return \c true if running in single user mode.
   *  \remarks In Single user mode, the user has indicated his preference
   *           to save all per-user program configuration in the programs folder,
   *           like in pre-Vista mode.
   */    }
function f2_singleusermode: bool; cdecl; external 'furn2.dll';
{   /** Retrieves Furnarchy's root directory.
   *  \return An absolute path to the furnarchy root.
   *  \sa f2_clroot */}
function f2_root(): pchar; cdecl; external 'furn2.dll';
{   /** Retrieves Furnarchy's data directory.
   *  \return An absolute path to the furnarchy data directory.
   *  \remarks This is the folder located in the per-user "Application Data" or "AppData" folder.
   *  \warning If furnarchy is running in single user mode, this directory will be the same as
   *           f2_root.
   *  \sa f2_root f2_docroot */}
function f2_dataroot: pChar; cdecl; external 'furn2.dll';
{   /** Retrieves Furnarchy's document directory.
   *  \return An absolute path to the furnarchy document directory.
   *  \remarks This is the folder located in the per-user "My Documents" or "Documents" folder.
   *  \warning If furnarchy is running in single user mode, this directory will be the same as
   *           f2_root.
   *  \sa f2_root f2_dataroot */}
function f2_docroot: pChar; cdecl; external 'furn2.dll';
{   *  \return An absolute path to the client's root.
   *  \sa f2_root */}
function f2_clroot(): pchar; cdecl; external 'furn2.dll';
{   /** Retrieves the client's data directory.
   *  \return An absolute path to the client's data directory.
   *  \remarks This is the folder located in the computer-specific (non-roaming), 
   *           per-user "Application Data" or "AppData" folder.
   *  \sa f2_clroot f2_cldocroot */}
function f2_cldataroot: pChar; cdecl; external 'furn2.dll';
{   /** Retrieves the client's document directory.
   *  \return An absolute path to the client's document directory.
   *  \remarks This is the folder located in the per-user "My Documents" or "Documents" folder.
   *  \sa f2_clroot f2_cldataroot */ }
function f2_cldocroot: pChar; cdecl; external 'furn2.dll';
{   /** Creates a directory hierarchy.
   *  \return non-zero on success.
   *  \sa f2_clroot f2_cldataroot */     }
//   F2LINK bool F2CALL f2_createdirs( const char* path );
function f2_createdirs(path : pchar): BOOL; cdecl; external 'furn2.dll';

                         
//Module Functions
{   /** Counts the number of loaded modules.
   *  \return The number of modules loaded.
   *  \remarks Because module unloading is unsupported
   *           in F2 at the time of this writing, you can rely on the
   *           return value of f2_modcount to remain the same for the
   *           lifetime of the program. */}
function f2_modcount(): size_t; cdecl; external 'furn2.dll';
{   /** Retrieves a module's root directtory.
   *  \param[in]     mod      The module's index.
   *  \return An absolute path to the module's root directory. */}
function f2_modroot(module : cardinal): pchar; cdecl; external 'furn2.dll';
{   /** Retrieves a module's data directtory.
   *  \param[in]     mod      The module's index.
   *  \return An absolute path to the module's data directory.
   *  \remarks This is located in the per-user "Application Data" or "AppData" directory.
   *           Per-user files a module creates that it doesn't want to expose to the user go here. 
   *  \warning If furnarchy is running in single user mode, this directory will be the same as
   *           f2_modroot.
   *  \sa f2_modroot f2_moddocroot */   }

function f2_moddataroot(module: cardinal): pChar; cdecl; external 'furn2.dll';
{   /** Retrieves a module's documents directtory.
   *  \param[in]     mod      The module's index.
   *  \return An absolute path to the module's documents directory.
   *  \remarks This is located in the per-user "My Documents" or "Documents" directory.
   *           Per-user files a module creates that it wants to expose to the user go here.
   *  \warning If furnarchy is running in single user mode, this directory will be the same as
   *           f2_modroot.
   *  \sa f2_modroot f2_moddataroot */ }
function f2_moddocroot(module: cardinal): pChar; cdecl; external 'furn2.dll';
{   /** Retrieves a module's name.
   *  This is just the module's directory name, as opposed to its
   *  "fancy name."
   *  \param[in]     mod      The module's index.
   *  \return The module's name.
   *  \remarks The module name can be used to reference the module
   *           via '\@mod*' command line commands.
   *  \sa f2_modfancyname */}
function f2_modname(module: cardinal): pchar; cdecl; external 'furn2.dll';
{   /** Retrieves a module's fancy name, as opposed to its "real name."
   *  This is the name set in the module's module.ini file.
   *  \param[in]     mod      The module's index.
   *  \return The module's fancy name.
   *  \remarks The fancy name is used for purely aesthetic reasons.
   *  \sa f2_modname */}
function f2_modfancyname(module: cardinal): pchar; cdecl; external 'furn2.dll';
{   /** Retrieves a module's thumbnail image path.
   *  This is the path specified by the module's module.ini file, and
   *  defaults to 'thumbnail.bmp' if not.
   *  \param[in]     mod      The module's index.
   *  \return An absolute path to the module's thumbnail image.
   *  \remarks The module's thumbnail image should be of a common
   *           image format (jpeg, gif, tga, bmp, png, etc.) and
   *           32x32.  This is typically used by skins. */}
function f2_modthumb(module: cardinal): pchar; cdecl; external 'furn2.dll';
{   /** Retrieves a module's state.
   *  \param[in]     mod      The module's index.
   *  \return A combination of the following flags:
   *          - MOD_DISABLED (mutually exclusive with MOD_ENABLED)
   *          - MOD_ENABLED  (mutually exclusive with MOD_DISABLED)
   *  \remarks Yeah, there's not much variation right now, but that could
   *           change in the future.  You can modify these flags via
   *           the f2_modenable() function.
   *  \sa f2_modenable */}
function f2_modstate(module: cardinal): longword; cdecl; external 'furn2.dll';
{   /** Enables or disables a module.
   *  \param[in]     mod      The module's index.
   *  \param[in]     onoff    \c true to enable a module.
   *  \remarks This will trigger a MOD_EVENT_ENABLE or MOD_EVENT_DISABLE
   *           in the target module, but only if the module isn't
   *           already in that state.
   *           The enabled or disabled state is merely a suggestion for a
   *           module to restrict or unleash its full functionality.
   *           Furnarchy doesn't inhibit the module in any way just because
   *           it's in a disabled state.
   *  \sa f2_modstate */}
procedure f2_modenable(module: cardinal; onoff: bool); cdecl; external 'furn2.dll';
{   /** Looks for a module by UUID.
   *  The UUID is specified in the module's module.ini as an alternate
   *  means of locating it.
   *  \param[in]     uuid     The module's UUID.
   *  \return The module's index number, or -1 if the module wasn't found.
   *  \remarks A module can define a UUID if it intends to expose an
   *           interface to its neighbors (via f2_modmsg or what have you).
   *           The UUID is the preferred method of locating such a module
   *           because the module can easily change its UUID to indicate
   *           new/incompatible interfaces.
   *  \sa f2_modmsg */}
function f2_modfind(uuid: PUUID128): index_t; cdecl; external 'furn2.dll';
{   /** Sends a module a touch event.
   *  \param[in]     mod      The module's index.
   *  \remarks The touch event has historically been a way of asking the
   *           module to expose or display more information. */}
procedure f2_modtouch(module: cardinal); cdecl; external 'furn2.dll';
{   /** Sends a module a message.
   *  Sends a module some module-specific data.
   *  \param[in]      mod     The target module's index.
   *  \param[in,out]  data    Module-specific data.
   *  \return The target module's response, or 0 if the target doesn't exist.
   *  \remarks If f2_modmsg() seems a little vague to you, that's the point.
   *           It only provides a very general method of transporting \a data
   *           between modules.  What the \a data is or means is something
   *           that must be agreed upon between the caller and the target.
   *  \sa f2_modfind */}
function f2_modmsg(module: cardinal; data: pointer): integer; cdecl; external 'furn2.dll';

//World Functions
{   /** Gets information on the (host) player.
   *  \param[out]    out      PLAYERINFO structure to be filled with player data.
   *  \warning The strings returned in \a out have an indefinite lifespan
   *           and should be copied somewhere if you intend to refer
   *           to them at a later time.
   *  \sa PLAYERINFO */}
procedure f2_myplayerinfo(player : PPLAYERINFO); cdecl; external 'furn2.dll';
{  /** Gets information on a monster (avatar).
   *  \param[in]     guid     Monster GUID.
   *  \param[out]    out      MONSTERINFO structure to be filled with player data.
   *  \warning The strings returned in \a out have an indefinite lifespan 
   *           and should be copied somewhere if you intend to refer 
   *           to them at a later time.
   *  \remarks If successful, the guid contained in \a out will be non-zero.
   *  \sa MONSTERINFO */  }
procedure f2_monsterinfo(guid: cardinal; monster: PMonsterInfo); cdecl; external 'furn2.dll';
 {  /** Gets information on a monster (avatar) with a name.
   *  \param[in]     name     Monster name.
   *  \param[out]    out      MONSTERINFO structure to be filled with player data.
   *  \warning The strings returned in \a out have an indefinite lifespan 
   *           and should be copied somewhere if you intend to refer 
   *           to them at a later time.
   *  \remarks The name search is done in a case-sensitive manner.
   *  \remarks If successful, the guid contained in \a out will be non-zero.
   *  \sa MONSTERINFO */ }
procedure f2_monsterinfo_byname(name: pchar; monster: PMonsterInfo; by_shortname: BOOL = false); cdecl; external 'furn2.dll';
 {  /** Retrieves the GUIDs of all existing monsters.
   *  \param[out]    guids    Integer array to be filled with GUIDs. May be NULL.
   *  \param[in]     len      Length of the \a guids array.
   *  \return The number of guids copied to \a guids.
   *  \remarks If \a guids is NULL, nothing is copied and the total number of existing
   *           monsters is returned.
   *  \sa f2_monsterinfo */ }
function f2_monsters_enum(guids: array of cardinal; len: cardinal) : cardinal; cdecl; external 'furn2.dll';
{   /** Cancels the last badge/specitag from being displayed.
   *  \remarks You should call this whenever you cancel chat text that
   *           typically has a badge displayed before it (speech, whispers, etc.),
   *           or if you want to make sure no badge is displayed before the current
   *           text.
   */}
procedure f2_badge_cancel; cdecl; external 'furn2.dll';

//Desctags Functions
{   /** Registers a desctag slot.
   *  The created desctag can be set by f2_desctag_set().
   *  \return A nonzero unique desctag id.
   *  \remarks Desctags slots are handed out order-wise on a first-come
   *           first-serve basis.
   *           You should free the desctag slot with f2_desctag_unreg()
   *           when you no longer need it.
   *  \sa f2_desctag_set f2_desctag_unreg */}
function f2_desctag_reg: integer; cdecl; external 'furn2.dll';
{   /** Unregisters a desctag slot.
   *  \param[in]     tag_id      A unique tag identifier previously returned by
   *                             a f2_desctag_reg() call.
   *  \return \c true if \a tag_id was successfully unregistered.
   *  \remarks This may result in an immediate `desc or `chdesc command
   *           being sent to the server to reflect the restored desc.
   *  \sa f2_desctag_reg f2_desctag_set */}
function f2_desctag_unreg(tag_id: integer): BOOL; cdecl; external 'furn2.dll';
{   /** Sets the contents of a desctag slot.
   *  \param[in]     tag_id      A unique tag identifier previously used in a
   *                             successful f2_desctag_reg() call.
   *  \param[in]     tag         A string to set the slot to.
   *  \remarks \a tag will be trimmed to one line and and cleaned
   *           to be human-readable.
   *           This may result in an immediate `desc or `chdesc command
   *           being sent to the server to reflect the updated desc.
   * \sa f2_desctag_reg f2_desctag_unreg */}
function f2_desctag_set(tag_id: integer; tag: pchar): bool; cdecl; external 'furn2.dll';

//Advanced Functions
  {/** \defgroup adv Advanced Functions.
   *  Advanced functions that depend on code and memory hacks.
   *  With these methods, you can manipulate aspects the client and its
   *  behavior.  Be warned; because these require intimate interaction
   *  with the client, they are guaranteed to cease working if a new
   *  client build is released or memlocs don't match up.  To test for
   *  this condition, see f2_buildok().
   */ }
   {/** Sets internal player information.
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
   */  }
procedure f2__setplayerinfo(name, pw, color, desc: pChar; guid: pointer); cdecl; external 'furn2.dll';
   {/** Replaces the client's main connection socket handle.
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
   *           level could have strange results. */ }
procedure f2__setmainsocket(socket: TSocket); cdecl; external 'furn2.dll';
   {/** Pushes a line of text into the chat buffer.
   *  \param[in]     line     The line of text.
   *  \remarks This should always work, regardless of the client build, in some way. */  }
procedure f2__pushchatbox(line: pchar); cdecl; external 'furn2.dll';
 {  /** Adds an ignore.
   *  \param[in]  name  Name to ignore. If \a name starts with a '@', indicates a channel.
   *  \return true if successful. */ }
procedure f2__ignore_add(name: pchar); cdecl; external 'furn2.dll';
{   /** Removes an ignore.
   *  \param[in]  name  Name to unignore. If \a name starts with a '@', indicates a channel.
   *  \return true if successful. */ }
procedure f2__ignore_rm(name: pchar); cdecl; external 'furn2.dll';
{   /** Tests if a name is ignored.
   *  \param[in]  name  Name to test. If \a name starts with a '@', indicates a channel.
   *  \return true if ignored. */   }
procedure f2__ignore_test(name: pchar); cdecl; external 'furn2.dll';
{/** Retrieves the player shortname associated with the last player menu popup.
   *  \return  A shortname.
   *  \remarks The lifetime of the buffer returned by this function is indefinite, thus
   *           the string should be copied immediately if you intend to reference it
   *           at a later time. }
function f2__playermenu_name():pchar; cdecl; external 'furn2.dll';


{   /** \defgroup api_script Script Functions.
   *  These functions allow the manipulation of scripts.
   */
   /**\{*/
   /** Enumerates the script ids of all running scripts.
   *  \param[out]  sids    int array to fill with script ids.
   *  \param[in]   len     Length of the \a sids array.
   *  \return  The number of sids copied to the \a sids array.
   *  \remarks If \a sids is NULL, nothing is copied and the function
   *           returns the number of running scripts.
   */ }
 // F2LINK unsigned int F2CALL f2_script_enum( unsigned int* sids, unsigned int len );
function f2_script_enum(sids: PCardinal; len : cardinal): cardinal; cdecl; external 'furn2.dll';

{   /** Retrieves the run arguments and flags for a running script.
   *  \param[in]   sid    The script ID.
   *  \param[out]  argv   Array of CONSTANT pointers to fill with string addresses.
   *  \param[in]   len    Length of the \a argv array.
   *  \param[out]  flags  int buffer to be filled with run flags. May be NULL.
   *  \return  The number of pointers copied to the \a argv array.
   *  \remarks If \a argv is NULL, the function copies the flags and returns the number of total arguments.
   */      }
  // F2LINK unsigned int F2CALL f2_script_argv( unsigned int sid, const char** argv, unsigned int len, int* flags );
function f2_script_argv(sid: Cardinal; argv: PPChar; len: Cardinal; var flags: Integer): integer; cdecl; external 'furn2.dll';
{   /** Runs a script.
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
   */        }
//   F2LINK Script_Error F2CALL f2_script_run( const char** argv, unsigned int len, int flags, unsigned int* sid );
function f2_script_run(argv : PPChar; len : cardinal; flags: integer; var sid: longint): Script_Error;  cdecl; external 'furn2.dll';
 {  /** Kills a running script.
   *  \param[in]  sid   The script ID.
   *  \return  An error code.
   *  \sa Script_Error
   */      }
 //  F2LINK Script_Error F2CALL f2_script_kill( unsigned int sid );
function f2_script_kill(sid : cardinal): Script_Error; cdecl; external 'furn2.dll';
//   /**\}*/

{   /** \defgroup api_menu Menu Functions.
   *  These functions allow the manipulation of a player's popup menu.
   */
   /**\{*/
   /** Sets a module's submenu in the player's popup menu.
   *  \param[in]  mod   Module index.
   *  \param[in]  HMENU Submenu.
   *  \return  \c true on success.
   *  \sa f2__playermenu_name
   */}
function f2_playermenu_set( module: longint; menu: HMENU ):bool; cdecl; external 'furn2.dll';
{   /** Unsets a module's submenu in the player's popup menu.
   *  \param[in]   mod  Module index.
   *  \return \c true on success.
   \sa f2__playermenu_name
   */ }
function f2_playermenu_unset(module: longint):bool; cdecl; external 'furn2.dll';


implementation
//UUID128 Functions
procedure InitUUID128(var id: UUID128; sz: PChar = nil);
begin
AssignUUID128(id, sz);
end;

procedure AssignUUID128(var id: UUID128; sz: PChar);
var
 b: Cardinal;
 ch: Char;
 i, j: Integer;
begin
if Assigned(sz) then
 begin
 b := 0;
 ch := sz^;//^sz
 for i := 0 to 15 do
  begin
  b := 0;
  j := 0;
  while (ord(ch) <> 0) and (j < 2) do
   begin
   case ch of
    '0'..'9':
     begin
     Inc(b, (ord(ch) - Ord('0')) shl ((j xor 1) shl 2)); //lsh
     Inc(j);
     end;
    'A'..'F':
     begin
     Inc(b, (ord(ch) - Ord('A') + 10) shl ((j xor 1) shl 2));
     Inc(j);
     end;
    'a'..'f':
     begin
     Inc(b, (ord(ch) - Ord('a') + 10) shl ((j xor 1) shl 2));
     Inc(j);
     end;
   end;
  Inc(sz);
  ch := sz^;//^sz
  end;
 id.bytes[i] := b and $FF;
 end;
end else
 begin
 id.ints[0] := 0;
 id.ints[1] := 0;
 id.ints[2] := 0;
 id.ints[3] := 0;
 end;
end;

procedure CopyUUID128(var id: UUID128; const r: UUID128);
begin
id.ints[0] := r.ints[0];
id.ints[1] := r.ints[1];
id.ints[2] := r.ints[2];
id.ints[3] := r.ints[3];
end;

function IsEqualUUID128(const id, r: UUID128): Boolean;
begin
Result := (id.ints[0] = r.ints[0]) and (id.ints[1] = r.ints[1]) and
(id.ints[2] = r.ints[2]) and (id.ints[3] = r.ints[3]);
end;

function IsNotEqualUUID128(const id, r: UUID128) : boolean;
begin
Result := (id.ints[0] <> r.ints[0]) or (id.ints[1] <> r.ints[1]) or
(id.ints[2] <> r.ints[2]) or (id.ints[3] <> r.ints[3]);
end;

function IsNotValidUUID128(const id: UUID128) : boolean;
begin
Result := (id.ints[0] = 0) and (id.ints[1] = 0) and (id.ints[2] = 0)
and (id.ints[3] = 0);
end;

end.
