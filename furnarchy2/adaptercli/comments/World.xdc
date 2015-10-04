<?xml version="1.0"?><doc>
<members>
<member name="T:furn2.Revision" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="54">
The network revision pass.
   * \sa on_inet on_onet net_handling 
</member>
<member name="F:MOD_EVENT_START" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="66">
The module is being started for the first time.
      *  \param[in]     param       A const MODULEINFO*
      *  \return Return 0 always.
      *  \sa MODULEINFO 
</member>
<member name="F:MOD_EVENT_STOP" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="71">
The module is being shut down.
      *  \param[in]     param       Not used.
      *  \return Return 0 always. 
</member>
<member name="F:MOD_EVENT_ENABLE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="75">
The module has been enabled.
      *  \param[in]     param       Not used.
      *  \return Return 0 always.
      *  \sa f2_modenable() 
</member>
<member name="F:MOD_EVENT_DISABLE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="80">
The module has been disabled.
      *  \param[in]     param       Not used.
      *  \return Return 0 always.
      *  \sa f2_modenable() 
</member>
<member name="F:MOD_EVENT_TOUCH" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="85">
The module has been touched.
      *  \param[in]     param       Not used.
      *  \return Return 0 always.
      *  \sa f2_modtouch() 
</member>
<member name="F:MOD_EVENT_MSG" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="90">
An arbitrary communication/transport event.
      *  \param[in]     param       Meaning is arbitrary.
      *  \return Meaning is arbitrary.
      *  \remarks It's up to modules communicating with each other
      *           via MOD_EVENT_MSG to agree on what the parameters
      *           and return value mean.
      *  \sa f2_modmsg() 
</member>
<member name="T:furn2.Module_Event" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="58">
\defgroup mod_events Module Events
   *  Module events.
   *  These events are received through a module's on_module() event handler.
   *  The event determines the type and meaning of on_module()'s \a param 
   *  parameter and return value.
   *  \sa on_module 
\{
</member>
<member name="F:CL_EVENT_WNDCREATED" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="109">
The main client window was created.
      *  \param[in]     param    HWND of the created window.
      *  \return Return 0 always.
      *  \remarks After this message is received, f2_getwnd() will also
      *           return the window handle.
      *  \sa f2_getwnd() 
</member>
<member name="F:CL_EVENT_SCRLOAD" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="116">
The client is about to change screens.
      *  \param[in]     param    A Client_Screen indicating the new screen.
      *  \return Return 0 always.
      *  \remarks Furnarchy monitors file access to determine when screen
      *           changes occur.
      *  \sa Client_Screen 
</member>
<member name="F:CL_EVENT_SETMAINSOCKET" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="123">
The main connection socket is being set.
      *  \param[in]     param    The new SOCKET handle.
      *  \return Return 0 always.
      *  \remarks The socket is set when the client initially attempts to
      *           establish a connection to the game server.  Modules may
      *           also trigger this event when they call f2__setmainsocket().
      *  \sa f2__setmainsocket() 
</member>
<member name="F:CL_EVENT_PRERENDER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="131">
Furnarchy is rendering its lowest z-order elements.
      *  \param[in]     param    const RENDERPARAMS*
      *  \return Return 0 always.
      *  \remarks Furnarchy guarantees the device context provided in \a param
      *           will be in the format of a 24-bit top-down DIB section, for
      *           compatibility with SDL and easier direct manipulation.  Be
      *           sure to call GdiFlush() if you wish to read/modify the DIB's
      *           bits directly.
      *  \sa RENDERPARAMS, CL_EVENT_RENDER, CL_EVENT_POSTRENDER 
</member>
<member name="F:CL_EVENT_RENDER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="141">
Furnarchy is rendering its medium/normal z-order elements.
      *  \param[in]     param    const RENDERPARAMS*
      *  \return Return 0 always.
      *  \remarks Furnarchy guarantees the device context provided in \a param
      *           will be in the format of a 24-bit top-down DIB section, for
      *           compatibility with SDL and easier direct manipulation.  Be
      *           sure to call GdiFlush() if you wish to read/modify the DIB's
      *           bits directly.
      *  \sa RENDERPARAMS, CL_EVENT_PRERENDER, CL_EVENT_POSTRENDER 
</member>
<member name="F:CL_EVENT_POSTRENDER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="151">
Furnarchy is rendering its highest z-order elements.
      *  \param[in]     param    const RENDERPARAMS*
      *  \return Return 0 always.
      *  \remarks Furnarchy guarantees the device context provided in \a param
      *           will be in the format of a 24-bit top-down DIB section, for
      *           compatibility with SDL and easier direct manipulation.  Be
      *           sure to call GdiFlush() if you wish to read/modify the DIB's
      *           bits directly.
      *  \sa RENDERPARAMS, CL_EVENT_PRERENDER, CL_EVENT_RENDER 
</member>
<member name="F:CL_EVENT_TICK" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="161">
A regular Furnarchy tick event occurred.
      *  \param[in]     param    unsigned int, the number of milliseconds 
      *                          since the last tick event.
      *  \return Return 0 always.
      *  \remarks This gives modules an opportunity to perform timed events.
      *           CL_EVENT_TICK has a resolution of about 160ms. 
</member>
<member name="F:CL_EVENT_CHATBOX" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="168">
A line is being pushed onto the chat buffer.
      *  \param[in]     param    const char*
      *  \return Return non-zero to prevent the line from being pushed.
      *  \remarks This event depends on the client build and memlocs. 
</member>
<member name="F:CL_EVENT_PLAYERMENUSHOW" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="173">
The player popup menu is being shown.
      *  \param[in]     param    A const char*, the name of the player.
      *  \remarks This event is build-dependent. You'll only receive it
      *           if f2_buildok() returns true. 
</member>
<member name="F:CL_EVENT_SCRIPT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="178">
The running scripts have changed.
      *  \param[in]     param    Not used.
      *  \remarks
      *  This is triggered whenever a script is run or exited/killed. 
</member>
<member name="T:furn2.Client_Event" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="99">
\}
\defgroup cl_events Client Events
   *  Furcadia client events.
   *  These events are received through a module's on_client() event handler.
   *  The event determines the type and meaning of on_client()'s \a param 
   *  parameter and return value.
   *  \sa on_client 
\{
</member>
<member name="F:WO_EVENT_CONNECTED" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="194">
The main connection to the server has been established.
      *  \param  param    Not used.
      *  \return Return 0 always. 
</member>
<member name="F:WO_EVENT_DISCONNECTED" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="198">
The main connection to the server has been terminated or lost.
      *  \param param    Not used.
      *  \return Return 0 always. 
</member>
<member name="F:WO_EVENT_LOGGEDIN" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="202">
The player has successfully logged in.
      *  \param param    Not used.
      *  \return Return 0 always.
      *  \remarks After this event, you can use f2_myplayerinfo() to 
      *           retrieve information on the player, although some 
      *           of it (namely the colors and desc) may
      *           still be in a state of flux at this point. 
</member>
<member name="F:WO_EVENT_PLAYERINFO" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="210">
Player information has changed.
      *  \param param   Not used.
      *  \return Return 0 always.
      *  \remarks You can use f2_myplayerinfo() to retrieve the
      *           new information. 
</member>
<member name="F:WO_EVENT_MONSTER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="216">
Monster (avatar) updated.
      *  \param[in] param   GUID of the updated monster.
      *  \return Return 0 always.
      *  \remarks You can use f2_monsterinfo() to retrieve information
      *           on the updated monster. If f2_monsterinfo() returns NULL,
      *           this event occured in response to the monster being cleared. 
</member>
<member name="T:furn2.World_Event" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="184">
\}
\defgroup wo_events World Events
   *  Game world events.
   *  Events concerning the world are recieved through a module's on_world() event
   *  handler.  The event determines the type and meaning of on_world()'s \a param
   *  parameter and return value.
   *  \sa on_world 
\{
</member>
<member name="F:SCREEN_TITLE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="234">
The title, splash, or connecting screen. 
</member>
<member name="F:SCREEN_PICKEM" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="236">
The character creation screen. 
</member>
<member name="F:SCREEN_BORDER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="238">
The message of the day screen. 
</member>
<member name="F:SCREEN_MARBLED" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="240">
The regular world display screen. 
</member>
<member name="T:furn2.Client_Screen" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="231">
Different client screens.
   *  \sa on_client CL_EVENT_SCRLOAD 
</member>
<member name="F:SCRIPT_OK" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="247">
No error. 
</member>
<member name="F:SCRIPT_BADARG" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="249">
A bad argument was passed. 
</member>
<member name="F:SCRIPT_FAILED" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="251">
Script file failed to load/run. 
</member>
<member name="F:SCRIPT_BADSID" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="253">
The script ID is bad. 
</member>
<member name="F:SCRIPT_NOTUNIQUE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="255">
Script is already running. 
</member>
<member name="T:furn2.Script_Error" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="244">
Error codes returned by script functions.
   *  \sa api_script 
</member>
<member name="F:SCRIPT_RUN_UNIQUE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="262">
Only run the script if it isn't already running (with the same arguments). 
</member>
<member name="F:SCRIPT_RUN_REPLACE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="264">
If the script is already running (with the same arguments), kill it and replace it. 
</member>
<member name="F:SCRIPT_RUN_LATE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="266">
The script receives network events later than usual. 
</member>
<member name="T:furn2.Script_RunFlags" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="259">
Run flags for scripts.
   *  \sa api_script 
</member>
<member name="F:HIDE_FILTERS" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="273">
Prevent further filters from seeing the line. 
</member>
<member name="F:HIDE_SCRIPTS" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="275">
Prevent further scripts from seeing the line. 
</member>
<member name="F:HIDE_MODULES" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="277">
Prevent further modules from seeing the line. 
</member>
<member name="F:HIDE_WORLD" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="279">
Prevent furnarchy from seeing the line. 
</member>
<member name="F:HIDE_CLIENT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="281">
Prevent the client from seeing the line. 
</member>
<member name="F:HIDE_ALL" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="283">
Prevent anyone from seeing the line. 
</member>
<member name="T:furn2.Hide_Flags" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="270">
Network hide flags.
   *  \sa f2_hideline() 
</member>
<member name="T:furn2.UUID128" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="287">
A little class to represent UUIDs.
   *  \sa f2_modfind() 
</member>
<member name="T:furn2.POINT2" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="375">
Basic x,y coordinate structure. 
</member>
<member name="T:furn2.F2VERSION" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="386">
Version structure. 
</member>
<member name="T:furn2.PLAYERINFO" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="394">
Describes the player.
   *  \sa f2_myplayerinfo() 
</member>
<member name="F:furn2.PLAYERINFO.name" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="398">
Player name. 
</member>
<member name="F:furn2.PLAYERINFO.pw" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="400">
Player password. 
</member>
<member name="F:furn2.PLAYERINFO.mac_id" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="402">
Player machine ID (scrambled). 
</member>
<member name="F:furn2.PLAYERINFO.desc" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="404">
Player's description (logical). 
</member>
<member name="F:furn2.PLAYERINFO.colors" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="406">
Player's color code. 
</member>
<member name="F:furn2.PLAYERINFO.guid" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="408">
Player's GUID. 
</member>
<member name="F:furn2.PLAYERINFO.cam" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="410">
Player's position/camera position. 
</member>
<member name="T:furn2.MONSTERINFO" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="414">
Describes a monster.
   *  \sa f2_myplayerinfo() 
</member>
<member name="F:furn2.MONSTERINFO.guid" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="418">
Monster GUID. 
</member>
<member name="F:furn2.MONSTERINFO.name" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="420">
Monster name. 
</member>
<member name="F:furn2.MONSTERINFO.color" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="422">
Monster color string. 
</member>
<member name="F:furn2.MONSTERINFO.pos" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="424">
Monster position. 
</member>
<member name="F:furn2.MONSTERINFO.frame" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="426">
Monster frame. 
</member>
<member name="F:furn2.MONSTERINFO.flags" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="428">
Flags.
      *  May be a combination of the following flags:
      *     - \c MONSTER_HASPROFILE The monster has a profile page. 
</member>
<member name="T:furn2.MODULEINFO" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="434">
Describes a loaded module.
   *  \sa on_module MOD_EVENT_START 
</member>
<member name="F:furn2.MODULEINFO.mod" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="438">
Module index number. 
</member>
<member name="F:furn2.MODULEINFO.uuid" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="440">
Module UUID (optional). 
</member>
<member name="F:furn2.MODULEINFO.state" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="442">
Module state flags.
      *  This may be a combination of the following flags:
      *     - MOD_DISABLED  (mutually exclusive with MOD_ENABLED).
      *     - MOD_ENABLED   (mutually exclusive with MOD_DISABLED). 
</member>
<member name="F:furn2.MODULEINFO.name" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="447">
Module's directory name. 
</member>
<member name="F:furn2.MODULEINFO.fancy_name" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="449">
Module's fancy name (optional). 
</member>
<member name="F:furn2.MODULEINFO.root" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="451">
Absolute path to the module's root directory. 
</member>
<member name="F:furn2.MODULEINFO.thumb_file" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="453">
Absolute path to the module's thumbnail image (optional). 
</member>
<member name="T:furn2.RENDERPARAMS" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="457">
Describes a render target.
   *  \sa on_client CL_EVENT_RENDER CL_EVENT_PRERENDER CL_EVENT_POSTRENDER 
</member>
<member name="F:furn2.RENDERPARAMS.dc" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="461">
Device context of the target surface.
      *  The bitmap contained in the DC is actually a 24-bit
      *  top-down DIB which you can manipulate manually if you
      *  choose to. 
</member>
<member name="F:furn2.RENDERPARAMS.width" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="466">
Width of the surface (should be 640 always). 
</member>
<member name="F:furn2.RENDERPARAMS.height" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="468">
Width of the surface (should be 480 always). 
</member>
<member name="T:furn2.WNDPROCPARAMS" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="472">
Describes a windows message.
   *  These are the parameters typically passed to the furcadia window's
   *  window procedure.  For more on (WM_) windows messages, see
   *  the Windows Platform SDK documentation at 
   *  http://msdn.microsoft.com/library/en-us/winui/winui/windowsuserinterface/windowing/windows.asp?
   *  \sa on_wndproc 
</member>
<member name="F:furn2.WNDPROCPARAMS.wnd" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="480">
The window handle. 
</member>
<member name="F:furn2.WNDPROCPARAMS.msg" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="482">
The window procedure message. 
</member>
<member name="F:furn2.WNDPROCPARAMS.wparam" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="484">
The message wparam. 
</member>
<member name="F:furn2.WNDPROCPARAMS.lparam" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="486">
The message lparam. 
</member>
<member name="F:furn2.WNDPROCPARAMS.retval" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="488">
The value to return if this message is handled by a module. 
</member>
<member name="M:furn2.f2_hookapi(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.Void*,System.Boolean)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="507">
\defgroup api API Functions 
   * bam boom 
\{
\defgroup api_misc Miscellaneous Functions.
   *  This and that. 
\{
Hooks an API symbol by name or ordinal.
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
   *           the next.
</member>
<member name="M:furn2.f2_getwnd" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="527">
Retrieve the window handle of the main window.
   *  \remarks This is only valid after the window is created (duh).
   *           To know when the window is created, wait for the 
   *           CL_EVENT_WNDCREATED event.
   *  \sa CL_EVENT_WNDCREATED 
</member>
<member name="M:furn2.f2_setcurdialog(HWND__*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="533">
Set the active dialog window.
   *  \param[in]     dlg      The active dialog window.
   *  \remarks This command no longer does anything. All windows are automatically
   *           granted dialog processing by Furnarchy unless they explicitly opt-out by
   *           responding to the F2M_ISNOTDLGWND message.
   *  \sa F2M_ISNOTDLGWND 
</member>
<member name="M:furn2.f2_command(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.Boolean)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="540">
Send a command line command through furnarchy.
   *  \param[in]     command     The command line command (minus the '@' prefix).
   *  \param[in]     noecho      Whether or not to echo the command in the chat buffer.
   *  \remarks Commands are queued, not processed immediately.
   *  \sa on_command 
</member>
<member name="M:furn2.f2_name_push(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="546">
Adds/refreshes a name in the autocomplete list.
   *  \param[in]  name  The name to add or refresh.
   *  \remarks If the name already exists in the list, it is refreshed by
   *           being promoted to the front of the list.

</member>
<member name="M:furn2.f2_name_complete(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*,System.UInt32,System.Int32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="552">
Completes a name.
   *  \param[in,out]    name  String containing the start of the name, also receives
   *                          the completed name.
   *  \param[in]        size  The size of the \a name buffer, in bytes.
   *  \param[in]        iter  An iterator state value that provides for cycling
   *                          through matching names.
   *  \return The next \a iter state value, or -1 if the name wasn't matched.
   *  \remarks To cycle through all names, start with \a iter as 0 and use
   *           the return value for each successive call.

</member>
<member name="M:furn2.f2_makeshortname(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="563">
Converts a player name into a shortname.
   *  Shortnames are a character's "true" name. All non-alphanumeric characters
   *  are removed and accented characters are converted to their ascii equivalents.
   *  \param[in]     name        Long name.
   *  \param[out]    buf         Buffer to be filled with the shortname. sizeof( buf ) must be &gt;= sizeof( \a name ). 
</member>
<member name="M:furn2.f2_connlvl" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="571">
\}
\defgroup api_net Network Functions.
   *  Facilitates sending and receiving of network data and 
   *  manipulating network lines being processed. 
\{
Retrieve the current game connection level.
   *  \return The connection level.
   *  \remarks This refers to the primary, game connection. 
</member>
<member name="M:furn2.f2_netout(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="581">
Sends some bytes to the server.
   *  \param[in]     buf      Bytes to send.
   *  \param[in]     size     Number of bytes to send.
   *  \remarks Bytes are queued, not sent or processed immediately.
   *  \sa on_onet f2_netoutline() f2_netoutcount() 
</member>
<member name="M:furn2.f2_netoutline(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="587">
Sends a line to the server.
   *  \param[in]     line     Line to send.
   *  \param[in]     len      Length of the line.
   *  \return The line number.
   *  \remarks Lines are queued, not sent or processed immediately.
   *           The return value can be used to identify the line as it
   *           passes through on_onet.
   *  \sa on_onet f2_netout() f2_netoutcount() 
</member>
<member name="M:furn2.f2_netoutcount" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="596">
Returns the next outbound line number to be assigned.
   *  \return The next outbound line number.
   *  \sa on_onet f2_netout() f2_netoutline() 
</member>
<member name="M:furn2.f2_netin(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="600">
Sends some bytes to the client.
   *  \param[in]     buf      Bytes to send.
   *  \param[in]     size     Number of bytes to send.
   *  \remarks Bytes are queued, not sent or processed immediately.
   *  \sa on_inet f2_netinline() f2_netincount() 
</member>
<member name="M:furn2.f2_netinline(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="606">
Sends a line to the client.
   *  \param[in]     line     Line to send.
   *  \param[in]     len      Length of the line.
   *  \return The line number.
   *  \remarks Lines are queued, not sent or processed immediately.
   *           The return value can be used to identify the line as it
   *           passes through on_onet.
   *  \sa on_onet f2_netin() f2_netincount() 
</member>
<member name="M:furn2.f2_netincount" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="615">
Returns the next inbound line number to be assigned.
   *  \return The next inbound line number.
   *  \sa on_onet f2_netin() f2_netinline() 
</member>
<member name="M:furn2.f2_reviseline(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="619">
Modifies the current line being revised.
   *  \param[in]     line     A null-terminated line to replace the current line.
   *  \remarks The current line can be modified in-place during the REV_REVISING
   *           pass of the on_inet() or on_onet() event.
   *  \warning Once you revise or cancel a line, the data pointed to by the
   *           \a line string passed into the handler is in an undefined state.
   *           The address it points to may not even be valid anymore.
   *  \sa on_inet on_onet f2_cancelline() 
</member>
<member name="M:furn2.f2_cancelline" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="628">
Cancels the current line.
   *  \deprecated This function is only included for legacy modules and has been replaced by
   *              the more robust f2_hideline(). In fact, this function is essentially synonymous with
   *              <tt>f2_hideline( HIDE_ALL )</tt>.
   *  \sa on_inet on_onet f2_reviseline() f2_hideline() 
</member>
<member name="M:furn2.f2_hideline(System.Int32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="634">
Prevents the current line from being processed by further handlers/listeners in specific subsystems.
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
   *  \sa Hide_Flags on_inet on_onet f2_reviseline() 
</member>
<member name="M:furn2.f2_hideflags" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="652">
Retrieves the hide flags for the current line.
   *  \return The hide flags for the current line.
   *  \sa Hide_Flags f2_hideline()

</member>
<member name="M:furn2.f2_speak(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="657">
\}
\defgroup api_out Output Functions.
   *  Facilitates printing to the chat buffer and error log. 
\{
Prints a line of text to the chat buffer.
   *  \param[in]     line     A null-terminated string to print.
   *  \remarks Furnarchy will strip any non-readable characters
   *           from this, as well as trim it down to one line.
   *  \sa f2_errspeak() f2_errlog() 
</member>
<member name="M:furn2.f2_errspeak(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="668">
Prints an error line to the chat buffer.
   *  The line is printed to the chat buffer and then appended to
   *  Furnarchy's error log.
   *  \param[in]     line     A null-terminated string to print.
   *  \remarks This is equivalent to doing an f2_speak() followed by an 
   *           f2_errlog().
   *  \sa f2_errspeak() f2_errlog() 
</member>
<member name="M:furn2.f2_errlog(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="676">
Appends some text to the error log.
   *  The error log is shared by all of Furnarchy at $FURNROOT\\core_err.log
   *  \param[in]     line     A null-terminated string to print.
   *  \sa f2_speak() f2_errspeak() 
</member>
<member name="M:furn2.f2_buildok" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="681">
\}
\defgroup api_version Versioning Functions.
   *  Client CRCing, build checking, and Furnarchy versioning. 
\{
Checks if the furnarchy2 memlocs are valid for this client build.
   *  \return \c true if the memlocs are valid for this client build.
   *  \remarks Furnarchy simply checks if the CRC specified in the memlocs.ini
   *           file is the same as the exe's CRC.  If this fails, any
   *           build-dependent (memloc-dependent) interfaces may be unavailable
   *           or limited.
   *  \sa f2_clcrc(), adv 
</member>
<member name="M:furn2.f2_clcrc" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="694">
Retrieves the Furcadia client's CRC32.
   *  \return The standard PKZIP CRC32 of the client's disk image. 
</member>
<member name="M:furn2.f2_version(furn2.F2VERSION*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="697">
Retrieves Furnarchy's version.
   *  \param[out] out  Structure to be filled with version information.
   *  \return Furnarchy's version. 
</member>
<member name="M:furn2.f2_singleusermode" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="701">
\}
\defgroup api_fs Filesystem Functions.
   *  Get directory paths. 
\{
Checks whether Furnarchy is running in single user mode.
   *  \return \c true if running in single user mode.
   *  \remarks In Single user mode, the user has indicated his preference
   *           to save all per-user program configuration in the programs folder,
   *           like in pre-Vista mode.

</member>
<member name="M:furn2.f2_root" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="713">
Retrieves Furnarchy's program directory.
   *  \return An absolute path to the furnarchy program directory.
   *  \sa f2_dataroot() f2_docroot() 
</member>
<member name="M:furn2.f2_dataroot" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="717">
Retrieves Furnarchy's data directory.
   *  \return An absolute path to the furnarchy data directory.
   *  \remarks This is the folder located in the per-user "Application Data" or "AppData" folder.
   *  \warning If furnarchy is running in single user mode, this directory will be the same as
   *           f2_root().
   *  \sa f2_root() f2_docroot() 
</member>
<member name="M:furn2.f2_docroot" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="724">
Retrieves Furnarchy's document directory.
   *  \return An absolute path to the furnarchy document directory.
   *  \remarks This is the folder located in the per-user "My Documents" or "Documents" folder.
   *  \warning If furnarchy is running in single user mode, this directory will be the same as
   *           f2_root().
   *  \sa f2_root() f2_dataroot() 
</member>
<member name="M:furn2.f2_clroot" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="731">
Retrieves the client's program directory.
   *  \return An absolute path to the client's program directory.
   *  \sa f2_cldocroot() f2_cldataroot() 
</member>
<member name="M:furn2.f2_cldataroot" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="735">
Retrieves the client's data directory.
   *  \return An absolute path to the client's data directory.
   *  \remarks This is the folder located in the computer-specific (non-roaming), 
   *           per-user "Application Data" or "AppData" folder.
   *  \sa f2_clroot() f2_cldocroot() 
</member>
<member name="M:furn2.f2_cldocroot" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="741">
Retrieves the client's document directory.
   *  \return An absolute path to the client's document directory.
   *  \remarks This is the folder located in the per-user "My Documents" or "Documents" folder.
   *  \sa f2_clroot() f2_cldataroot() 
</member>
<member name="M:furn2.f2_createdirs(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="746">
Creates a directory hierarchy.
   *  \return non-zero on success.
   *  \sa f2_clroot() f2_cldataroot() 
</member>
<member name="M:furn2.f2_modcount" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="750">
\}
\defgroup api_mod Module Functions.
   *  Facilitates module manipulation and communication.
   *  Many of these require a module index as a parameter.
   *  To retrieve your own module index, copy it from the \c MOD_EVENT_START
   *  event in on_module(). 
\{
Counts the number of loaded modules.
   *  \return The number of modules loaded.
   *  \remarks Because module unloading is unsupported
   *           in F2 at the time of this writing, you can rely on the 
   *           return value of f2_modcount() to remain the same for the
   *           lifetime of the program. 
</member>
<member name="M:furn2.f2_modroot(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="765">
Retrieves a module's program directory.
   *  \param[in]     mod      The module's index.
   *  \return An absolute path to the module's program directory.
   *  \sa f2_moddataroot() f2_moddocroot() 
</member>
<member name="M:furn2.f2_moddataroot(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="770">
Retrieves a module's data directtory.
   *  \param[in]     mod      The module's index.
   *  \return An absolute path to the module's data directory.
   *  \remarks This is located in the per-user "Application Data" or "AppData" directory.
   *           Per-user files a module creates that it doesn't want to expose to the user go here. 
   *  \warning If furnarchy is running in single user mode, this directory will be the same as
   *           f2_modroot().
   *  \sa f2_modroot() f2_moddocroot() 
</member>
<member name="M:furn2.f2_moddocroot(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="779">
Retrieves a module's documents directtory.
   *  \param[in]     mod      The module's index.
   *  \return An absolute path to the module's documents directory.
   *  \remarks This is located in the per-user "My Documents" or "Documents" directory.
   *           Per-user files a module creates that it wants to expose to the user go here.
   *  \warning If furnarchy is running in single user mode, this directory will be the same as
   *           f2_modroot().
   *  \sa f2_modroot() f2_moddataroot() 
</member>
<member name="M:furn2.f2_modname(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="788">
Retrieves a module's name.
   *  This is just the module's directory name, as opposed to its
   *  "fancy name."
   *  \param[in]     mod      The module's index.
   *  \return The module's name.
   *  \remarks The module name can be used to reference the module
   *           via '\@mod*' command line commands.
   *  \sa f2_modfancyname() 
</member>
<member name="M:furn2.f2_modfancyname(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="797">
Retrieves a module's fancy name, as opposed to its "real name."
   *  This is the name set in the module's module.ini file.
   *  \param[in]     mod      The module's index.
   *  \return The module's fancy name.
   *  \remarks The fancy name is used for purely aesthetic reasons.
   *  \sa f2_modname() 
</member>
<member name="M:furn2.f2_modthumb(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="804">
Retrieves a module's thumbnail image path.
   *  This is the path specified by the module's module.ini file, and
   *  defaults to 'thumbnail.bmp' if not.
   *  \param[in]     mod      The module's index.
   *  \return An absolute path to the module's thumbnail image.
   *  \remarks The module's thumbnail image should be of a common
   *           image format (jpeg, gif, tga, bmp, png, etc.) and 
   *           32x32.  This is typically used by skins. 
</member>
<member name="M:furn2.f2_modstate(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="813">
Retrieves a module's state.
   *  \param[in]     mod      The module's index.
   *  \return A combination of the following flags:
   *          - MOD_DISABLED (mutually exclusive with MOD_ENABLED)
   *          - MOD_ENABLED  (mutually exclusive with MOD_DISABLED)
   *  \remarks Yeah, there's not much variation right now, but that could
   *           change in the future.  You can modify these flags via
   *           the f2_modenable() function.
   *  \sa f2_modenable() 
</member>
<member name="M:furn2.f2_modenable(System.UInt32,System.Boolean)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="823">
Enables or disables a module.
   *  \param[in]     mod      The module's index.
   *  \param[in]     onoff    \c true to enable a module.
   *  \remarks This will trigger a MOD_EVENT_ENABLE or MOD_EVENT_DISABLE 
   *           in the target module, but only if the module isn't
   *           already in that state.
   *           The enabled or disabled state is merely a suggestion for a
   *           module to restrict or unleash its full functionality.
   *           Furnarchy doesn't inhibit the module in any way just because
   *           it's in a disabled state.
   *  \sa f2_modstate() 
</member>
<member name="M:furn2.f2_modfind(furn2.UUID128!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="835">
Looks for a module by UUID.
   *  The UUID is specified in the module's module.ini as an alternate
   *  means of locating it.
   *  \param[in]     uuid     The module's UUID.
   *  \return The module's index number, or -1 if the module wasn't found.
   *  \remarks A module can define a UUID if it intends to expose an
   *           interface to its neighbors (via f2_modmsg() or what have you).  
   *           The UUID is the preferred method of locating such a module 
   *           because the module can easily change its UUID to indicate 
   *           new/incompatible interfaces.
   *  \sa f2_modmsg() 
</member>
<member name="M:furn2.f2_modtouch(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="847">
Sends a module a touch event.
   *  \param[in]     mod      The module's index.
   *  \remarks The touch event has historically been a way of asking the
   *           module to expose or display more information. 
</member>
<member name="M:furn2.f2_modmsg(System.UInt32,System.Void*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="852">
Sends a module a message.
   *  Sends a module some module-specific data.
   *  \param[in]      mod     The target module's index.
   *  \param[in,out]  data    Module-specific data.
   *  \return The target module's response, or 0 if the target doesn't exist.
   *  \remarks If f2_modmsg() seems a little vague to you, that's the point.
   *           It only provides a very general method of transporting \a data
   *           between modules.  What the \a data is or means is something
   *           that must be agreed upon between the caller and the target.
   *  \sa f2_modfind() 
</member>
<member name="M:furn2.f2_myplayerinfo(furn2.PLAYERINFO*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="863">
\}
\defgroup api_world World Functions.
   *  Query and manipulate the game world. 
\{
Gets information on the (host) player.
   *  \param[out]    out      PLAYERINFO structure to be filled with player data.
   *  \warning The strings returned in \a out have an indefinite lifespan 
   *           and should be copied somewhere if you intend to refer 
   *           to them at a later time.
   *  \sa PLAYERINFO 
</member>
<member name="M:furn2.f2_monsterinfo(System.UInt32,furn2.MONSTERINFO*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="875">
Gets information on a monster (avatar).
   *  \param[in]     guid     Monster GUID.
   *  \param[out]    out      MONSTERINFO structure to be filled with player data.
   *  \warning The strings returned in \a out have an indefinite lifespan 
   *           and should be copied somewhere if you intend to refer 
   *           to them at a later time.
   *  \remarks If successful, the guid contained in \a out will be non-zero.
   *  \sa MONSTERINFO 
</member>
<member name="M:furn2.f2_monsterinfo_byname(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,furn2.MONSTERINFO*,System.Boolean)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="884">
Gets information on a monster (avatar) with a name.
   *  \param[in]     name     Monster name.
   *  \param[out]    out      MONSTERINFO structure to be filled with player data.
   *  \param[in]     by_shortname     true if searching by shortname.
   *  \warning The strings returned in \a out have an indefinite lifespan 
   *           and should be copied somewhere if you intend to refer 
   *           to them at a later time.
   *  \remarks The name search is done in a case-sensitive manner.
   *  \remarks If successful, the guid contained in \a out will be non-zero.
   *  \sa MONSTERINFO 
</member>
<member name="M:furn2.f2_monsters_enum(System.UInt32*,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="895">
Retrieves the GUIDs of all existing monsters.
   *  \param[out]    guids    Integer array to be filled with GUIDs. May be NULL.
   *  \param[in]     len      Length of the \a guids array.
   *  \return The number of guids copied to \a guids.
   *  \remarks If \a guids is NULL, nothing is copied and the total number of existing
   *           monsters is returned.
   *  \sa f2_monsterinfo() 
</member>
<member name="M:furn2.f2_monstervisible(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="903">
Check if a monster is visible.
   *  \param[in]    guid    GUID of the monster to check.
   *  \return \c true if the monster is visible. \c false if not visible or doesn't exist.
   *  \remarks A monster is visible if an "avatar show" server command has been received.
   *           An "avatar hide" server command will mark the monster invisible.
   *  \sa f2_monsterinfo() 
</member>
<member name="M:furn2.f2_badge_cancel" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="910">
Cancels the last badge/specitag from being displayed.
   *  \remarks You should call this whenever you cancel chat text that
   *           typically has a badge displayed before it (speech, whispers, etc.),
   *           or if you want to make sure no badge is displayed before the current
   *           text.

</member>
<member name="M:furn2.f2_desctag_reg" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="918">
\defgroup desctags Desctags Functions.
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
   *  f2_desctag_unreg() should be called when the tag is no longer needed. 
\{
Registers a desctag slot.
   *  The created desctag can be set by f2_desctag_set().
   *  \return A nonzero unique desctag id.
   *  \remarks Desctags slots are handed out order-wise on a first-come 
   *           first-serve basis.
   *           You should free the desctag slot with f2_desctag_unreg()
   *           when you no longer need it.
   *  \sa f2_desctag_set() f2_desctag_unreg() 
</member>
<member name="M:furn2.f2_desctag_unreg(System.Int32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="949">
Unregisters a desctag slot.
   *  \param[in]     tag_id      A unique tag identifier previously returned by
   *                             a f2_desctag_reg() call.
   *  \return \c true if \a tag_id was successfully unregistered.
   *  \remarks This may result in an immediate `desc or `chdesc command
   *           being sent to the server to reflect the restored desc.
   *  \sa f2_desctag_reg() f2_desctag_set() 
</member>
<member name="M:furn2.f2_desctag_set(System.Int32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="957">
Sets the contents of a desctag slot.
   *  \param[in]     tag_id      A unique tag identifier previously used in a
   *                             successful f2_desctag_reg() call.
   *  \param[in]     tag         A string to set the slot to.
   *  \remarks \a tag will be trimmed to one line and and cleaned
   *           to be human-readable.
   *           This may result in an immediate `desc or `chdesc command
   *           being sent to the server to reflect the updated desc.
   * \sa f2_desctag_reg() f2_desctag_unreg() 
</member>
<member name="M:furn2.f2__setplayerinfo(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.UInt32!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="967">
\}
\}
\defgroup adv Advanced Functions.
   *  Advanced functions that depend on code and memory hacks.
   *  With these methods, you can manipulate aspects the client and its
   *  behavior.  Be warned; because these require intimate interaction
   *  with the client, they are guaranteed to cease working if a new
   *  client build is released or memlocs don't match up.  To test for
   *  this condition, see f2_buildok().

\{
Sets internal player information.
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

</member>
<member name="M:furn2.f2__setmainsocket(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="996">
Replaces the client's main connection socket handle.
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
   *           level could have strange results. 
</member>
<member name="M:furn2.f2__pushchatbox(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="1011">
Pushes a line of text into the chat buffer.
   *  \param[in]     line     The line of text.
   *  \remarks This should always work, regardless of the client build, in some way. 
</member>
<member name="M:furn2.f2__ignore_add(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="1015">
Adds an ignore.
   *  \param[in]  name  Name to ignore. If \a name starts with a '@', indicates a channel.
   *  \return true if successful. 
</member>
<member name="M:furn2.f2__ignore_rm(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="1019">
Removes an ignore.
   *  \param[in]  name  Name to unignore. If \a name starts with a '@', indicates a channel.
   *  \return true if successful. 
</member>
<member name="M:furn2.f2__ignore_test(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="1023">
Tests if a name is ignored.
   *  \param[in]  name  Name to test. If \a name starts with a '@', indicates a channel.
   *  \return true if ignored. 
</member>
<member name="M:furn2.f2__playermenu_name" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="1027">
Retrieves the player shortname associated with the last player menu popup.
   *  \return  A shortname.
   *  \remarks The lifetime of the buffer returned by this function is indefinite, thus
   *           the string should be copied immediately if you intend to reference it
   *           at a later time.
   *  \sa api_menu

</member>
<member name="M:furn2.f2_script_enum(System.UInt32*,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="1035">
\}
\defgroup api_script Script Functions.
   *  These functions allow the manipulation of scripts.

\{
Enumerates the script ids of all running scripts.
   *  \param[out]  sids    int array to fill with script ids.
   *  \param[in]   len     Length of the \a sids array.
   *  \return  The number of sids copied to the \a sids array.
   *  \remarks If \a sids is NULL, nothing is copied and the function
   *           returns the number of running scripts.

</member>
<member name="M:furn2.f2_script_argv(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst**,System.UInt32,System.Int32*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="1049">
Retrieves the run arguments and flags for a running script.
   *  \param[in]   sid    The script ID.
   *  \param[out]  argv   Array of CONSTANT pointers to fill with string addresses.
   *  \param[in]   len    Length of the \a argv array.
   *  \param[out]  flags  int buffer to be filled with run flags. May be NULL.
   *  \return  The number of pointers copied to the \a argv array.
   *  \remarks If \a argv is NULL, the function copies the flags and returns the number of total arguments.

</member>
<member name="M:furn2.f2_script_run(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst**,System.UInt32,System.Int32,System.UInt32*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="1058">
Runs a script.
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

</member>
<member name="M:furn2.f2_script_kill(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="1074">
Kills a running script.
   *  \param[in]  sid   The script ID.
   *  \return  An error code.
   *  \sa Script_Error

</member>
<member name="M:furn2.f2_playermenu_set(System.UInt32,HMENU__*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="1080">
\}
\defgroup api_menu Menu Functions.
   *  These functions allow the manipulation of a player's popup menu.

\{
Sets a module's submenu in the player's popup menu.
   *  \param[in]  mod   Module index.
   *  \param[in]  menu Submenu.
   *  \return  \c true on success.
   *  \sa f2__playermenu_name()

</member>
<member name="M:furn2.f2_playermenu_unset(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\furnarchycore\furn2.h" line="1093">
Unsets a module's submenu in the player's popup menu.
   *  \param[in]   mod  Module index.
   *  \return \c true on success.
   \sa f2__playermenu_name()

</member>
<member name="M:xml_charset.uni_to_latin(std.basic_string&lt;System.Char,std.char_traits{System.Char},std.allocator&lt;System.Char&gt;&gt;!System.Runtime.CompilerServices.IsConst*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\common\xml_charset.h" line="34">
Converts unicode to latin-1.
   *  Unicode characters with no latin-1 equivalent will be converted
   *  to an XML entity name. 
</member>
<member name="M:xml_charset.uni_to_ascii(std.basic_string&lt;System.Char,std.char_traits{System.Char},std.allocator&lt;System.Char&gt;&gt;!System.Runtime.CompilerServices.IsConst*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\common\xml_charset.h" line="38">
Converts unicode to ASCII.
   *  Unicode characters with no ASCII equivalent will be converted
   *  to an XML entity name. 
</member>
<member name="M:xml_charset.latin_to_uni(std.basic_string&lt;System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte,std.char_traits{System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte},std.allocator&lt;System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte&gt;&gt;!System.Runtime.CompilerServices.IsConst*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\common\xml_charset.h" line="42">
Converts latin-1 to unicode.
   *  XML entity names will be converted to their unicode equivalent. 
</member>
<member name="M:xml_charset.ascii_to_latin(std.basic_string&lt;System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte,std.char_traits{System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte},std.allocator&lt;System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte&gt;&gt;!System.Runtime.CompilerServices.IsConst*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\common\xml_charset.h" line="45">
Converts ASCII to latin-1.
   *  XML entity names will be converted to their latin-1 equivalent. 
</member>
<member name="M:xml_charset.latin_to_ascii(std.basic_string&lt;System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte,std.char_traits{System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte},std.allocator&lt;System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte&gt;&gt;!System.Runtime.CompilerServices.IsConst*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\common\xml_charset.h" line="48">
Converts latin-1 to ASCII.
   *  Latin-1 characters with no ASCII equivalent will be converted
   *  to an XML entity name. 
</member>
<member name="T:Furnarchy.IFurnarchyModule" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\furnarchy.h" line="14">
<summary>Empty interface a module's primary class should implement.</summary>
<remarks>
  <para>
     The module host uses reflection to find the first public class that implements
     this interface then constructs it, passing the <see cref="!:ModuleHost"/> instance.
  </para>
</remarks>
<example>
  Defining a CLI module's primary class (C#).
  <code>
     public class MyModule : Furnarchy.IFurnarchyModule
     {
        ModuleHost m_host;
        // Constructor must have this signature.
        public MyModule( ModuleHost host )
        {
           m_host = host;
        }
     }
  </code>
</example>
</member>
<member name="T:Furnarchy.Move_Direction" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\furnarchy.h" line="37">
<summary> Map movement directions. </summary>
</member>
<member name="T:Furnarchy.Avatar_Class" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\furnarchy.h" line="45">
<summary> Avatar types. </summary>
</member>
<member name="T:Furnarchy.Avatar_Pose" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\furnarchy.h" line="123">
<summary> Avatar poses. </summary>
</member>
<member name="T:Furnarchy.Monster_Flags" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\furnarchy.h" line="133">
<summary> Monster (avatar) creation flags. </summary>
</member>
<member name="T:Furnarchy.F2Version" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\furnarchy.h" line="143">
<summary> Furnarchy version structure. </summary>
</member>
<member name="T:Furnarchy.Module_State" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="5">
<summary> Module state flags. </summary>
</member>
<member name="T:Furnarchy.TickEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="16">
<summary> Arguments for <see cref="!:TickDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.CommandEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="22">
<summary> Arguments for <see cref="!:CommandDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.BroadcastValueEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="29">
<summary> Arguments for <see cref="!:BroadcastValueDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.EnabledDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="44">
<summary> A module event delegate. </summary>
<seealso cref="!:ModuleHost::EnabledEvent"/>
</member>
<member name="T:Furnarchy.DisabledDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="47">
<summary> A module event delegate. </summary>
<seealso cref="!:ModuleHost::DisabledEvent"/>
</member>
<member name="T:Furnarchy.TouchedDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="50">
<summary> A module event delegate. </summary>
<seealso cref="!:ModuleHost::TouchedEvent"/>
</member>
<member name="T:Furnarchy.TickDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="53">
<summary> A module event delegate. </summary>
<seealso cref="!:ModuleHost::TickEvent"/>
</member>
<member name="T:Furnarchy.CommandDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="56">
<summary> A module event delegate. </summary>
<seealso cref="!:ModuleHost::CommandEvent"/>
</member>
<member name="T:Furnarchy.BroadcastValueDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="59">
<summary> A module event delegate. </summary>
<seealso cref="!:ModuleHost::BroadcastValueEvent"/>
</member>
<member name="T:Furnarchy.ModuleHost" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="65">
<summary>Class providing common services for CLI modules.</summary>
<remarks>
  <para>
     An instance of this class will be created for each module that
     defines a public class that implements the IFurnarchyModule interface.
  </para>
  <para>
     Please be wary of some features that depend on memlocs that will
     operate with limited (if any) functionality if Furnarchy does not
     support the running client build.
  </para>
</remarks>
</member>
<member name="M:Furnarchy.ModuleHost.postCommand(System.String,System.Boolean)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="95">
<summary> Posts a @-style command to Furnarchy. </summary>
<param name="command">
  Command line. Each argument is space-delimited. To preserve whitespace, you
  must escape it or surround each argument with quotes, apostrophes, or backticks.
  Non-ascii characters will be XML-escaped.
</param>
<param name="no_echo">
  Whether or not to echo the command in the chat buffer.
</param>
<remarks> 
  After the command is processed, a <see cref="E:Furnarchy.ModuleHost.CommandEvent"/> will
  be raised.
</remarks>
</member>
<member name="M:Furnarchy.ModuleHost.makeShortname(System.String)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="114">
<summary> Convert a user name into its shortname equivalent. </summary>
<param name="name"> The name to transform. Non-ascii characters will be XML-escaped. </param>
<returns> The shortname version of <c>name</c>. </returns>
<remarks> Shortnames are stripped down versions of names and how the server sees names. </remarks>
</member>
<member name="M:Furnarchy.ModuleHost.broadcastValue(System.IntPtr)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="128">
<summary> Broadcast a value to all (other) running instances of this module. </summary>
<param name="value"> Value to broadcast. </param>
<remarks>
  This will asynchronously raise a <see cref="E:Furnarchy.ModuleHost.BroadcastValueEvent"/> in all running
  instances of this module.
</remarks>
<seealso cref="E:Furnarchy.ModuleHost.BroadcastValueEvent"/>
</member>
<member name="M:Furnarchy.ModuleHost.speak(System.String)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="141">
<summary> Echoes a string in the chat buffer. </summary>
<param name="line"> String to echo. Non-ascii characters will be XML-escaped. </param>
<seealso cref="M:Furnarchy.ModuleHost.speakError(System.String)"/>
</member>
<member name="M:Furnarchy.ModuleHost.speakError(System.String)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="150">
<summary> Echoes a string in the chat buffer and adds it to the error log. </summary>
<param name="line"> String to echo. Non-ascii characters will be XML-escaped. </param>
<seealso cref="M:Furnarchy.ModuleHost.speak(System.String)"/>
<seealso cref="M:Furnarchy.ModuleHost.logError(System.String)"/>
</member>
<member name="M:Furnarchy.ModuleHost.logError(System.String)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="160">
<summary> Logs an error to the error log. </summary>
<param name="line"> String to append to the error log. Non-ascii characters will be XML-escaped. </param>
<seealso cref="M:Furnarchy.ModuleHost.speakError(System.String)"/>
</member>
<member name="M:Furnarchy.ModuleHost.touch" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="169">
<summary> "Touches" this module. </summary>
<remarks>
  This is equivalent to when the user right-clicks on your module's icon.
  A <see cref="E:Furnarchy.ModuleHost.TouchedEvent"/> will be immediately raised.
</remarks>
</member>
<member name="P:Furnarchy.ModuleHost.ModulesCount" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="179">
<summary> Number of modules loaded. </summary>
<remarks> This is a read-only attribute. </remarks>
<returns> The number of modules loaded/running. </returns>
</member>
<member name="P:Furnarchy.ModuleHost.Id" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="186">
<summary> This module's module ID. </summary>
<remarks> This is a read-only property. </remarks>
<returns> Your module's ID. </returns>
</member>
<member name="P:Furnarchy.ModuleHost.State" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="193">
<summary> This module's state. </summary>
<remarks>
  You can assign a new state to this property to update your module's state to Furnarchy.
  Enabling or disabling a module will immediately raise an <see cref="E:Furnarchy.ModuleHost.EnabledEvent"/> or <see cref="E:Furnarchy.ModuleHost.DisabledEvent"/>.
</remarks>
<seealso cref="T:Furnarchy.Module_State"/>
<returns> The state of this module (see <see cref="T:Furnarchy.Module_State"/>). </returns>
</member>
<member name="P:Furnarchy.ModuleHost.Name" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="217">
<summary> This module's name. </summary>
<remarks> The module name is simply the name of its program folder. </remarks>
<returns> This module's name. </returns>
</member>
<member name="P:Furnarchy.ModuleHost.FancyName" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="224">
<summary> This module's fancy name. </summary>
<remarks> This is the name specified in the module's module.ini file. </remarks>
<returns> This module's fancy name. </returns>
</member>
<member name="P:Furnarchy.ModuleHost.ProxyVersion" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="231">
<summary> The running version of Furnarchy. </summary>
<remarks> This is a read-only property. </remarks>
<seealso cref="T:Furnarchy.F2Version"/>
</member>
<member name="P:Furnarchy.ModuleHost.SingleUserMode" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="241">
<summary> Whether Furnarchy is running in single-user-mode. </summary>
<remarks> 
  <para>
     Single-user-mode means Furnarchy has reverted to the old, pre-Vista folder
     layout, where all files are contained within the Furnarchy Program Files folder.
  </para>
  <para>
     This is a read-only attribute.
  </para>
</remarks>
</member>
<member name="P:Furnarchy.ModuleHost.ProxyProgramFolder" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="255">
<summary> Furnarchy's program root folder. </summary>
</member>
<member name="P:Furnarchy.ModuleHost.ProxyDataFolder" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="260">
<summary> Furnarchy's application data folder. </summary>
<remarks> In single-user mode, this is equivalent to <see cref="P:Furnarchy.ModuleHost.ProxyProgramFolder"/>. </remarks>
</member>
<member name="P:Furnarchy.ModuleHost.ProxyDocumentFolder" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="266">
<summary> Furnarchy's document folder. </summary>
<remarks> In single-user mode, this is equivalent to <see cref="P:Furnarchy.ModuleHost.ProxyProgramFolder"/>. </remarks>
</member>
<member name="P:Furnarchy.ModuleHost.ClientFolder" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="272">
<summary> Furcadia's program root folder. </summary>
</member>
<member name="P:Furnarchy.ModuleHost.ClientDataFolder" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="277">
<summary> Furcadia's application data folder. </summary>
</member>
<member name="P:Furnarchy.ModuleHost.ClientDocumentFolder" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="282">
<summary> Furcadia's document folder. </summary>
</member>
<member name="P:Furnarchy.ModuleHost.ProgramFolder" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="287">
<summary> The program folder of this module. </summary>
<remarks> The program folder is the module where the module binary resides. </remarks>
<returns> A folder path. </returns>
</member>
<member name="P:Furnarchy.ModuleHost.DataFolder" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="294">
<summary> The application data folder of this module. </summary>
<remarks> 
  <para> 
     The application data folder of the module is where the module should store
     files that the user should not easily access.
  </para>
  <para>
     In single-user mode, this is equivalent to <see cref="P:Furnarchy.ModuleHost.ProgramFolder"/>.
  </para>
</remarks>
<returns> A folder path. </returns>
</member>
<member name="P:Furnarchy.ModuleHost.DocumentFolder" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="309">
<summary> The document folder of this module. </summary>
<remarks> 
  <para> 
     The document folder of the module is where the module should store
     files easily accessible to the user.
  </para>
  <para>
     In single-user mode, this is equivalent to <see cref="P:Furnarchy.ModuleHost.ProgramFolder"/>.
  </para>
</remarks>
<returns> A folder path. </returns>
</member>
<member name="P:Furnarchy.ModuleHost.Thumbnail" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="324">
<summary> This module's thumbnail file. </summary>
<returns> The path to a module's thumbnail file. </returns>
</member>
<member name="P:Furnarchy.ModuleHost.Enabled" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="330">
<summary> Whether this module is enabled. </summary>
<seealso cref="P:Furnarchy.ModuleHost.State"/>
</member>
<member name="E:Furnarchy.ModuleHost.EnabledEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="344">
<summary> The module has been enabled. </summary>
<remarks> 
  This can occur if the user left-clicks on your module's icon or the 
  <see cref="P:Furnarchy.ModuleHost.State"/> property has been assigned a new value.
</remarks>
<seealso cref="P:Furnarchy.ModuleHost.State"/>
</member>
<member name="E:Furnarchy.ModuleHost.DisabledEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="351">
<summary> The module has been disabled. </summary>
<remarks> 
  This can occur if the user left-clicks on your module's icon or the 
  <see cref="P:Furnarchy.ModuleHost.State"/> property has been assigned a new value.
</remarks>
<seealso cref="P:Furnarchy.ModuleHost.State"/>
</member>
<member name="E:Furnarchy.ModuleHost.TouchedEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="358">
<summary> The module has been touched. </summary>
<remarks> 
  This can occur when the user right-clicks on your module's icon
  or a call to <see cref="M:Furnarchy.ModuleHost.touch"/> is made. 
</remarks>
</member>
<member name="E:Furnarchy.ModuleHost.TickEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="364">
<summary> A module heartbeat has occured. </summary>
<param name="dt"> Time (in milliseconds) since last tick event. </param>
<remarks>
 This has a resolution of about 75ms, but will vary slightly
 from machine to machine.
</remarks>
</member>
<member name="E:Furnarchy.ModuleHost.CommandEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="371">
<summary> A '@' command needs to be processed. </summary>
<param name="args"> Structure containing the command arguments. </param>
<remarks>
 If you handle the command, you should set the <c>handled</c> field
 in <c>args</c> to <c>true</c>, otherwise the user will get an error
 about an unrecognized command.
</remarks>
</member>
<member name="E:Furnarchy.ModuleHost.BroadcastValueEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="379">
<summary> A value is being broadcasted to all instances of this module. </summary>
<seealso cref="M:Furnarchy.ModuleHost.broadcastValue(System.IntPtr)"/>
</member>
<member name="F:Furnarchy.ModuleHost.Client" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="386">
<summary> The client subsystem. See <see cref="T:Furnarchy.ClientSubsystem"/>. </summary>
</member>
<member name="F:Furnarchy.ModuleHost.Net" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="388">
<summary> The network subsystem. See <see cref="T:Furnarchy.NetSubsystem"/>. </summary>
</member>
<member name="F:Furnarchy.ModuleHost.Scripts" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="390">
<summary> The scripts subsystem. See <see cref="T:Furnarchy.ScriptsSubsystem"/>. </summary>
</member>
<member name="F:Furnarchy.ModuleHost.World" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\modulehost.h" line="392">
<summary> The world subsystem. See <see cref="T:Furnarchy.WorldSubsystem"/>. </summary>
</member>
<member name="T:Furnarchy.WndProcEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="11">
<summary> For use with <see cref="!:Client::onWndProc"/>. </summary>
</member>
<member name="T:Furnarchy.WindowCreatedEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="38">
<summary> Arguments for <see cref="!:WindowCreatedDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.PlayerMenuShowingEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="44">
<summary> Arguments for <see cref="!:PlayerMenuShowingDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.WindowMessageEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="50">
<summary> Arguments for <see cref="!:WindowMessageDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.DrawClientEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="61">
<summary> Arguments for <see cref="!:DrawClientDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.WindowCreatedDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="72">
<summary> A client event delegate. </summary>
<seealso cref="!:ClientSubsystem::WindowCreatedEvent"/>
</member>
<member name="T:Furnarchy.PlayerMenuShowingDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="75">
<summary> A client event delegate. </summary>
<seealso cref="!:ClientSubsystem::PlayerMenuShowingEvent"/>
</member>
<member name="T:Furnarchy.WindowMessageDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="78">
<summary> A client event delegate. </summary>
<seealso cref="!:ClientSubsystem::WindowMessageEvent"/>
</member>
<member name="T:Furnarchy.DrawClientDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="81">
<summary> A client event delegate. </summary>
<seealso cref="!:ClientSubsystem::DrawClientEvent"/>
</member>
<member name="T:Furnarchy.ClientSubsystem" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="87">
<summary> Encapsulates client hacks and properties. </summary>
</member>
<member name="M:Furnarchy.ClientSubsystem.cancelBadge" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="104">
<summary> Cancels the buffered chat badge. </summary>
<remarks>
 Cancels the badge to display with the next line of text.
</remarks>
</member>
<member name="M:Furnarchy.ClientSubsystem.attachPlayerMenu(System.Windows.Forms.MenuItem)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="109">
<summary> Attaches a menu to the player right-click menu. </summary>
<param name="menu"> MenuItem to attach. </param>
<remarks>
 <para>
  The player menu is the popup menu that appears when a name or avatar is right-clicked.
  A submenu will be created for each module that attaches a valid MenuItem.
 </para>
</remarks>
</member>
<member name="M:Furnarchy.ClientSubsystem.detachPlayerMenu" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="118">
<summary> Detaches a module's player right-click menu. </summary>
<remarks>
  The player menu is the popup menu that appears when a name or avatar is right-clicked.
</remarks>
</member>
<member name="M:Furnarchy.ClientSubsystem.pushChatBox(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="123">
<summary> Pushes a line to the chat buffer. </summary>
<param name="line"> Line to push. </param>
<remarks>
  This relies on client memlocs. If the build is unsupported, the line will be
  pushed by injecting a <c>'('</c> network line.
</remarks>
</member>
<member name="M:Furnarchy.ClientSubsystem.ignoreAdd(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="130">
<summary> Ignore a name or channel. </summary>
<param name="name"> Name or channel to ignore. </param>
<remarks>
  <para>
     To ignore a channel, prefix the channel name with a '@' character.
  </para>
  <para>
     This relies on client memlocs and will do nothing if the current build is unsupported.
  </para>
</remarks>
</member>
<member name="M:Furnarchy.ClientSubsystem.ignoreRemove(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="141">
<summary> Unignore a name or channel. </summary>
<param name="name"> Name or channel to unignore. </param>
<remarks>
  <para>
     To unignore a channel, prefix the channel name with a '@' character.
  </para>
  <para>
     This relies on client memlocs and will do nothing if the current build is unsupported.
  </para>
</remarks>
</member>
<member name="M:Furnarchy.ClientSubsystem.ignoreTest(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="152">
<summary> Tests if a name or channel is ignored. </summary>
<param name="name"> Name or channel to test. </param>
<remarks>
  <para>
     To test a channel, prefix the channel name with a '@' character.
  </para>
  <para>
     This relies on client memlocs and will do nothing if the current build is unsupported.
  </para>
</remarks>
<returns> <c>true</c> if the name or channel is ignored. </returns>
</member>
<member name="M:Furnarchy.ClientSubsystem.throwBuildOk" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="165">
<summary> Throws an exception if <see cref="P:Furnarchy.ClientSubsystem.BuildOk"/> is false. </summary>
</member>
<member name="P:Furnarchy.ClientSubsystem.BuildOk" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="173">
<summary> Whether or not the client build is supported by Furnarchy. </summary>
<remarks>
  When the client build is not supported, many operations that depend on
  memlocs or hacks will be crippled or disabled entirely.
</remarks>
</member>
<member name="P:Furnarchy.ClientSubsystem.ClientCrc" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="185">
<summary> The client program's CRC32. </summary>
</member>
<member name="P:Furnarchy.ClientSubsystem.ClientHwnd" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="193">
<summary> The HWND of the client's main window. </summary>
</member>
<member name="P:Furnarchy.ClientSubsystem.ClientWindow" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="202">
<summary> ClientHwnd wrapped in a IWin32Window (for use with <see cref="!:Form.Show"/>). </summary>
</member>
<member name="P:Furnarchy.ClientSubsystem.PlayermenuName" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="210">
<summary> The name of the player whose player menu (right-click menu) is being displayed for. </summary>
<remarks> 
  This depends on memlocs and will return an empty string if the client build is unsupported. 
</remarks>
</member>
<member name="E:Furnarchy.ClientSubsystem.PlayerMenuShowingEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="222">
<summary> The player menu popup is about to be displayed. </summary>
<param name="name"> The name of the player being clicked on. </param>
</member>
<member name="E:Furnarchy.ClientSubsystem.WindowCreatedEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="225">
<summary> The main window has been created. </summary>
<param name="hwnd"> The handle (HWND) if the window. </param>
</member>
<member name="E:Furnarchy.ClientSubsystem.WindowMessageEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="228">
<summary> A window message is coming through the client's message pump. </summary>
<param name="msg"> Properties of the message, which you may modify. </param>
<remarks>
 To indicate you've handled the message and wish to prevent the client or other 
 modules from processing it, set the <c>handled</c> field to <c>true</c>.
</remarks>
</member>
<member name="E:Furnarchy.ClientSubsystem.DrawClientEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\client.h" line="235">
<summary> The client-area is being drawn. </summary>
<param name="graphics"> A Graphics object for the client-area. </param>
</member>
<member name="T:yiffurc.MAJORMINOR" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="80">
Defines a major and minor revision. 
</member>
<member name="T:yiffurc.IDXY" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="87">
An object/floor/wall ID and a position pair. 
</member>
<member name="T:yiffurc.DSXY" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="95">
A DS line and parameter pair. 
</member>
<member name="T:yiffurc.Move_Dir" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="103">
Player movement directions.
*  \todo In the wrong place? 
</member>
<member name="T:yiffurc.Go_Map" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="113">
Some main map numbers.
* add to ' ' to make the correct gomap.
*  \sa c2s_enc_gomap 
</member>
<member name="M:yiffurc.ftoi3(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="134">
Converts a 3-digit base-95 furcadian number to unsigned int.
*  \note
*     ftoi3( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-95 Furcadian number (at least \c char[3])

</member>
<member name="M:yiffurc.ftoi2(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="146">
Converts a 2-digit base-95 furcadian number to an integer.
*  \note
*     ftoi2( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-95 Furcadian number (at least \c char[2])

</member>
<member name="M:yiffurc.ftoi1(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="156">
Converts a 1-digit base-95 furcadian number to an integer.
*  \param   f  Base-95 Furcadian digit.

</member>
<member name="M:yiffurc.itof3(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="163">
Converts an integer to a 3-digit base-95 furcadian number.
*  \note
*     itof3( ) won't null-terminate \c f.
*  \param[in]     i     integer number
*  \param[out]    f     Base-95 Furcadian number (at least \c char[3])

</member>
<member name="M:yiffurc.itof2(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="175">
Converts an integer to a 2-digit base-95 furcadian number.
*  \note
*     itof2( ) won't null-terminate \c f.
*  \param[in]     i           integer number
*  \param[out]    f           Base-95 Furcadian number (at least \c char[2])

</member>
<member name="M:yiffurc.itof1(System.UInt32)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="186">
Converts an integer to a 1-digit base-95 furcadian number.
*  param    i     integer number.
*  \returns Base-95 Furcadian digit.

</member>
<member name="M:yiffurc.ftoi4_220(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="195">
Converts a 4-digit base-220 furcadian number to unsigned int.
*  \note
*     ftoi4_220( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-220 Furcadian number (at least \c char[4])

</member>
<member name="M:yiffurc.ftoi3_220(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="208">
Converts a 3-digit base-220 furcadian number to unsigned int.
*  \note
*     ftoi3_220( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-220 Furcadian number (at least \c char[3])

</member>
<member name="M:yiffurc.ftoi2_220(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="220">
Converts a 2-digit base-220 furcadian number to an integer.
*  \note
*     ftoi2_220( ) doesn't care about null-termination.  It just
*     looks at the first two indices of \c f.
*  \param   f     Base-220 Furcadian number (at least \c char[2])

</member>
<member name="M:yiffurc.ftoi1_220(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="230">
Converts a 1-digit base-220 furcadian number to an integer.
*  \param   f  Base-220 Furcadian digit.

</member>
<member name="M:yiffurc.itof4_220(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="237">
Converts an integer to a 4-digit base-220 furcadian number.
*  \note
*     itof4_220( ) won't null-terminate \c f.
*  \param[in]     i     integer number
*  \param[out]    f     Base-220 Furcadian number (at least \c char[4])

</member>
<member name="M:yiffurc.itof3_220(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="250">
Converts an integer to a 3-digit base-220 furcadian number.
*  \note
*     itof3_220( ) won't null-terminate \c f.
*  \param[in]     i     integer number
*  \param[out]    f     Base-220 Furcadian number (at least \c char[3])

</member>
<member name="M:yiffurc.itof2_220(System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="262">
Converts an integer to a 2-digit base-220 furcadian number.
*  \note
*     itof2_220( ) won't null-terminate \c f.
*  \param[in]     i           integer number
*  \param[out]    f           Base-220 Furcadian number (at least \c char[2])

</member>
<member name="M:yiffurc.itof1_220(System.UInt32)" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="273">
Converts an integer to a 1-digit base-220 furcadian number.
*  param    i     integer number.
*  \returns Base-220 Furcadian digit.

</member>
<member name="M:yiffurc.get_color_len(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.UInt32!System.Runtime.CompilerServices.IsConst)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="282">
Compute the length of a color string. 
*  \remarks This doesn't include the gender-species-digo bytes that
*           sometimes follow the color string. Those are always 3 bytes
*           long.

</member>
<member name="M:yiffurc.yiffurc_release(System.Void!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\common.h" line="289">
\addtogroup memory Memory Management
*  \{ 
Releases any dynamically allocated object returned by a YIFFurc API function. 
</member>
<member name="T:yiffurc.Net_Level" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="36">
\defgroup net Network
* \{ 
Network Connection Levels.
* \sa NET_SESSION 
</member>
<member name="T:yiffurc.Xfer_Net_Level" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="48">
Transfer (fileserver) network connection levels.
* \sa XFER_NET_SESSION 
</member>
<member name="T:yiffurc.NET_SESSION" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="58">
Network session object.
* Maintains state information used in network parsing.
* \sa c2s_parse s2c_parse 
</member>
<member name="T:yiffurc.Net_Level" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="63">
Network Level.
   *  \sa Net_Level

</member>
<member name="F:yiffurc.NET_SESSION.create_count" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="68">
Number of outstanding character create requests from the client.
   *  Only valid when level ==  NET_LEVEL_MOTEND 
</member>
<member name="T:yiffurc.S2C" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="888">
Base parsed client protocol structure. 
</member>
<member name="F:yiffurc.S2C.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="891">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.S2C_MOTD" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="897">
A parsed MOTD line.
*  \sa S2C_TYPE_MOTD. 
</member>
<member name="F:yiffurc.S2C_MOTD.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="901">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_MOTD.line" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="905">
A line of the MOTD. 
</member>
<member name="T:yiffurc.S2C_DRAGONROAR" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="921">
A parsed Dragonroar command.
*  \sa S2C_TYPE_DRAGONROAR. 
</member>
<member name="F:yiffurc.S2C_DRAGONROAR.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="925">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.S2C_MOTDEND" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="942">
A parsed MOTD end command.
*  \sa S2C_TYPE_MOTDEND. 
</member>
<member name="F:yiffurc.S2C_MOTDEND.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="946">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.S2C_VER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="963">
A parsed protocol version command.
*  \sa S2C_TYPE_VER. 
</member>
<member name="F:yiffurc.S2C_VER.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="967">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.S2C_PASS" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="984">
A parsed login/create succeeded reply.
*  \sa S2C_TYPE_PASS. 
</member>
<member name="F:yiffurc.S2C_PASS.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="988">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.S2C_CREATEFAIL" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1006">
A parsed create failed reply.
*  \sa S2C_TYPE_CREATEFAIL. 
</member>
<member name="F:yiffurc.S2C_CREATEFAIL.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1010">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_CREATEFAIL.reason" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1015">
Human readable failure code. 
</member>
<member name="F:yiffurc.S2C_CREATEFAIL.category" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1018">
Error category.
   *  Typically either "NAME", "PASS", or "EMAIL" 
</member>
<member name="T:yiffurc.S2C_MACID" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1036">
A parsed machine ID response.
*  \sa S2C_TYPE_MACID. 
</member>
<member name="F:yiffurc.S2C_MACID.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1040">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_MACID.mac_id" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1044">
The scrambled machine ID response. 
</member>
<member name="T:yiffurc.S2C_LOGINFAIL" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1060">
A parsed login fail response.
*  \sa S2C_TYPE_LOGINFAIL. 
</member>
<member name="F:yiffurc.S2C_LOGINFAIL.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1064">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_LOGINFAIL.reason" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1068">
Human readable failure code. 
</member>
<member name="T:yiffurc.S2C_CAMSET" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1084">
A parsed camera set command.
*  \sa S2C_TYPE_CAMSET. 
</member>
<member name="F:yiffurc.S2C_CAMSET.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1088">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_CAMSET.x" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1092">
New coordinates. 
</member>
<member name="F:yiffurc.S2C_CAMSET.y" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1094">
New coordinates. 
</member>
<member name="T:yiffurc.S2C_CAMTWEEN" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1109">
A parsed camera tween command.
*  \sa S2C_TYPE_CAMTWEEN. 
</member>
<member name="F:yiffurc.S2C_CAMTWEEN.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1113">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_CAMTWEEN.x" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1117">
New coordinates. 
</member>
<member name="F:yiffurc.S2C_CAMTWEEN.y" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1119">
New coordinates. 
</member>
<member name="F:yiffurc.S2C_CAMTWEEN.x_old" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1121">
Old coordinates. 
</member>
<member name="F:yiffurc.S2C_CAMTWEEN.y_old" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1123">
Old coordinates. 
</member>
<member name="T:yiffurc.S2C_OBJSET" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1138">
A parsed series of object set commands.
*  \sa S2C_TYPE_OBJSET. 
</member>
<member name="F:yiffurc.S2C_OBJSET.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1142">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_OBJSET.count" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1146">
How many objects to set. 
</member>
<member name="T:yiffurc.IDXY" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1148">
Per-object info. 
</member>
<member name="T:yiffurc.S2C_AVCREATE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1164">
A parsed avatar create command.
*  \sa S2C_TYPE_AVCREATE. 
</member>
<member name="F:yiffurc.S2C_AVCREATE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1168">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_AVCREATE.uid" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1172">
User ID. 
</member>
<member name="F:yiffurc.S2C_AVCREATE.x" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1174">
Position. 
</member>
<member name="F:yiffurc.S2C_AVCREATE.y" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1176">
Position. 
</member>
<member name="F:yiffurc.S2C_AVCREATE.frame" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1178">
The avatar frame. 
</member>
<member name="F:yiffurc.S2C_AVCREATE.name" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1180">
Name. 
</member>
<member name="F:yiffurc.S2C_AVCREATE.colors" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1182">
Color code. 
</member>
<member name="F:yiffurc.S2C_AVCREATE.flags" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1184">
Flags. 
</member>
<member name="T:yiffurc.S2C_AVTWEEN" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1199">
A parsed avatar tween command.
*  \sa S2C_TYPE_AVTWEEN. 
</member>
<member name="F:yiffurc.S2C_AVTWEEN.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1203">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_AVTWEEN.uid" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1207">
User ID. 
</member>
<member name="F:yiffurc.S2C_AVTWEEN.x" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1209">
Position. 
</member>
<member name="F:yiffurc.S2C_AVTWEEN.y" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1211">
Position. 
</member>
<member name="F:yiffurc.S2C_AVTWEEN.frame" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1213">
End frame. 
</member>
<member name="T:yiffurc.S2C_AVDESTROY" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1228">
A parsed avatar destroy command.
*  \sa S2C_TYPE_AVDESTROY. 
</member>
<member name="F:yiffurc.S2C_AVDESTROY.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1232">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_AVDESTROY.uid" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1236">
User ID. 
</member>
<member name="T:yiffurc.S2C_DREAMPOS" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1251">
A parsed dream text command.
*  \sa S2C_TYPE_DREAMPOS. 
</member>
<member name="F:yiffurc.S2C_DREAMPOS.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1255">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_DREAMPOS.name" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1259">
Dream text. 
</member>
<member name="F:yiffurc.S2C_DREAMPOS.x" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1261">
Position. 
</member>
<member name="F:yiffurc.S2C_DREAMPOS.y" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1263">
Position. 
</member>
<member name="T:yiffurc.S2C_DREAMPOS2" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1279">
A parsed dream text and caption command.
*  \sa S2C_TYPE_DREAMPOS2. 
</member>
<member name="F:yiffurc.S2C_DREAMPOS2.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1283">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_DREAMPOS2.name" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1287">
Dream text. 
</member>
<member name="F:yiffurc.S2C_DREAMPOS2.caption" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1289">
Dream caption. 
</member>
<member name="F:yiffurc.S2C_DREAMPOS2.x" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1291">
Position. 
</member>
<member name="F:yiffurc.S2C_DREAMPOS2.y" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1293">
Position. 
</member>
<member name="T:yiffurc.S2C_DREAMCLEAR" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1310">
A parsed dream text clear command.
*  \sa S2C_TYPE_DREAMCLEAR. 
</member>
<member name="F:yiffurc.S2C_DREAMCLEAR.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1314">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_DREAMCLEAR.x" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1318">
Position. 
</member>
<member name="F:yiffurc.S2C_DREAMCLEAR.y" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1320">
Position. 
</member>
<member name="T:yiffurc.S2C_VARSET" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1335">
A parsed series of variable set commands.
*   \sa S2C_TYPE_VARSET. 
</member>
<member name="F:yiffurc.S2C_VARSET.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1339">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_VARSET.count" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1343">
Number of ranges to set. 
</member>
<member name="T:yiffurc.S2C_VARSET.VARTRIP" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1345">
Ranges of variables to set. 
</member>
<member name="F:yiffurc.S2C_VARSET.VARTRIP.first" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1348">
First variable to set. 
</member>
<member name="F:yiffurc.S2C_VARSET.VARTRIP.count" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1350">
How many variables to set. 
</member>
<member name="F:yiffurc.S2C_VARSET.VARTRIP.value" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1352">
Value to set the range to. 
</member>
<member name="T:yiffurc.S2C_FLOORSET" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1369">
A parsed series of floor set commands.
*   \sa S2C_TYPE_FLOORSET. 
</member>
<member name="F:yiffurc.S2C_FLOORSET.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1373">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_FLOORSET.count" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1377">
How many floors to set. 
</member>
<member name="T:yiffurc.IDXY" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1379">
Per-floor info. 
</member>
<member name="T:yiffurc.S2C_BADGE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1395">
A badge command.
*   \sa S2C_TYPE_BADGE. 
</member>
<member name="F:yiffurc.S2C_BADGE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1399">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_BADGE.badge" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1403">
Badge frame (for beekins, or the like). 
</member>
<member name="T:yiffurc.S2C_SPECITAG" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1418">
A specitag command.
*   \sa S2C_TYPE_SPECITAG. 
</member>
<member name="F:yiffurc.S2C_SPECITAG.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1422">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_SPECITAG.colors" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1426">
Specitag colors. 
</member>
<member name="F:yiffurc.S2C_SPECITAG.gsd" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1428">
Specitag species. 
</member>
<member name="T:yiffurc.S2C_ONLINERESULT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1444">
A parsed online check reply.
*   \sa S2C_TYPE_ONLINERESULT. 
</member>
<member name="F:yiffurc.S2C_ONLINERESULT.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1448">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_ONLINERESULT.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1452">
Player name 
</member>
<member name="F:yiffurc.S2C_ONLINERESULT.online" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1454">
Online status. 
</member>
<member name="T:yiffurc.S2C_PORTRAIT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1470">
A parsed display portrait command.
*   \sa S2C_TYPE_PORTRAIT. 
</member>
<member name="F:yiffurc.S2C_PORTRAIT.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1474">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_PORTRAIT.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1478">
Player name. 
</member>
<member name="F:yiffurc.S2C_PORTRAIT.colors" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1480">
Colors. 
</member>
<member name="F:yiffurc.S2C_PORTRAIT.gsd" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1482">
GSD. 
</member>
<member name="T:yiffurc.S2C_FOOTOBJ" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1499">
A parsed display object at feet command.
*   \sa S2C_TYPE_FOOTOBJ.

</member>
<member name="F:yiffurc.S2C_FOOTOBJ.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1504">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_FOOTOBJ.obj" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1508">
Object ID (frame). 
</member>
<member name="T:yiffurc.S2C_INVOBJ" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1523">
A parsed display object in inventory command.
*   \sa S2C_TYPE_INVOBJ. 
</member>
<member name="F:yiffurc.S2C_INVOBJ.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1527">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_INVOBJ.obj" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1531">
Object ID (frame). 
</member>
<member name="T:yiffurc.S2C_FETCHPID" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1546">
A parsed fetch portrait command.
*   \sa S2C_TYPE_FETCHPID. 
</member>
<member name="F:yiffurc.S2C_FETCHPID.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1550">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_FETCHPID.name" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1554">
Fileserver portrait name. 
</member>
<member name="F:yiffurc.S2C_FETCHPID.pid" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1556">
Fileserver portrait id. 
</member>
<member name="T:yiffurc.S2C_FETCHDREAM" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1572">
A parsed fetch dream command.
*   \sa S2C_TYPE_FETCHDREAM. 
</member>
<member name="F:yiffurc.S2C_FETCHDREAM.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1576">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_FETCHDREAM.name" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1580">
Fileserver dream name. 
</member>
<member name="F:yiffurc.S2C_FETCHDREAM.did" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1582">
Fileserver dream id. 
</member>
<member name="T:yiffurc.S2C_USEDREAM" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1598">
A parsed use dream command.
*   \sa S2C_TYPE_USEDREAM. 
</member>
<member name="F:yiffurc.S2C_USEDREAM.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1602">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_USEDREAM.name" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1606">
Fileserver dream name. 
</member>
<member name="F:yiffurc.S2C_USEDREAM.did" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1608">
Fileserver dream id. 
</member>
<member name="T:yiffurc.S2C_USEBGFILE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1624">
A parsed use background file command.
*   \sa S2C_TYPE_USEBGFILE. 
</member>
<member name="F:yiffurc.S2C_USEBGFILE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1628">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_USEBGFILE.file" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1632">
Background filename. 
</member>
<member name="T:yiffurc.S2C_USEMAPFILE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1648">
A parsed use map file command.
*   \sa S2C_TYPE_USEMAPFILE. 
</member>
<member name="F:yiffurc.S2C_USEMAPFILE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1652">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_USEMAPFILE.file" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1656">
Map filename. 
</member>
<member name="T:yiffurc.S2C_PLAYSONG" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1672">
A parsed play song command.
*   \sa S2C_TYPE_PLAYSONG. 
</member>
<member name="F:yiffurc.S2C_PLAYSONG.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1676">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_PLAYSONG.song" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1680">
Song number. 
</member>
<member name="T:yiffurc.S2C_PLAYSOUND" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1695">
A parsed play sound command.
*   \sa S2C_TYPE_PLAYSOUND. 
</member>
<member name="F:yiffurc.S2C_PLAYSOUND.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1699">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_PLAYSOUND.sound" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1703">
Sound number. 
</member>
<member name="T:yiffurc.S2C_SPEECH" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1718">
A parsed player speech command.
*   \sa S2C_TYPE_SPEECH. 
</member>
<member name="F:yiffurc.S2C_SPEECH.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1722">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_SPEECH.from_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1726">
Player short name. 
</member>
<member name="F:yiffurc.S2C_SPEECH.from" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1728">
Player name. 
</member>
<member name="F:yiffurc.S2C_SPEECH.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1730">
Message. 
</member>
<member name="T:yiffurc.S2C_WHISPER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1748">
A parsed player whisper command.
*   \sa S2C_TYPE_WHISPER. 
</member>
<member name="F:yiffurc.S2C_WHISPER.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1752">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_WHISPER.from_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1756">
Player short name. 
</member>
<member name="F:yiffurc.S2C_WHISPER.from" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1758">
Player name. 
</member>
<member name="F:yiffurc.S2C_WHISPER.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1760">
Message. 
</member>
<member name="T:yiffurc.S2C_SHOUT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1778">
A parsed player shout command.
*   \sa S2C_TYPE_SHOUT. 
</member>
<member name="F:yiffurc.S2C_SHOUT.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1782">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_SHOUT.from_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1786">
Player short name. 
</member>
<member name="F:yiffurc.S2C_SHOUT.from" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1788">
Player name. 
</member>
<member name="F:yiffurc.S2C_SHOUT.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1790">
Message. 
</member>
<member name="T:yiffurc.S2C_EMOTE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1808">
A parsed player emote command.
*   \sa S2C_TYPE_EMOTE. 
</member>
<member name="F:yiffurc.S2C_EMOTE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1812">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_EMOTE.from_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1816">
Player short name. 
</member>
<member name="F:yiffurc.S2C_EMOTE.from" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1818">
Player name. 
</member>
<member name="F:yiffurc.S2C_EMOTE.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1820">
Message/emote. 
</member>
<member name="T:yiffurc.S2C_EMIT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1838">
A parsed emit command.
*  \sa S2C_TYPE_EMIT 
</member>
<member name="F:yiffurc.S2C_EMIT.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1842">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_EMIT.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1846">
Emit message. 
</member>
<member name="T:yiffurc.S2C_ROLL" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1862">
A parsed "quiet" roll reply.
*   \sa S2C_TYPE_ROLL. 
</member>
<member name="F:yiffurc.S2C_ROLL.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1866">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_ROLL.player_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1870">
Player short name. 
</member>
<member name="F:yiffurc.S2C_ROLL.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1872">
Player name. 
</member>
<member name="F:yiffurc.S2C_ROLL.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1874">
Message. 
</member>
<member name="F:yiffurc.S2C_ROLL.dice" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1876">
Number of dice. 
</member>
<member name="F:yiffurc.S2C_ROLL.sides" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1878">
Number of sides. 
</member>
<member name="F:yiffurc.S2C_ROLL.sum" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1880">
Sum of the rolls. 
</member>
<member name="T:yiffurc.S2C_VERBOSEROLL" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1897">
A parsed ROLL reply.
*   \sa S2C_TYPE_VERBOSEROLL. 
</member>
<member name="F:yiffurc.S2C_VERBOSEROLL.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1901">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_VERBOSEROLL.player_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1905">
Player short name. 
</member>
<member name="F:yiffurc.S2C_VERBOSEROLL.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1907">
Player name. 
</member>
<member name="F:yiffurc.S2C_VERBOSEROLL.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1909">
Message. 
</member>
<member name="F:yiffurc.S2C_VERBOSEROLL.dice" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1911">
Number of dice. 
</member>
<member name="F:yiffurc.S2C_VERBOSEROLL.sides" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1913">
Number of sides. 
</member>
<member name="F:yiffurc.S2C_VERBOSEROLL.values" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1915">
Individual roll values. 
</member>
<member name="F:yiffurc.S2C_VERBOSEROLL.sum" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1917">
Sum of all values. 
</member>
<member name="T:yiffurc.S2C_DESCHEAD" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1935">
A parsed desc head reply.
*   \sa S2C_TYPE_DESCHEAD. 
</member>
<member name="F:yiffurc.S2C_DESCHEAD.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1939">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_DESCHEAD.player_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1943">
Player short name. 
</member>
<member name="F:yiffurc.S2C_DESCHEAD.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1945">
Player name. 
</member>
<member name="T:yiffurc.S2C_DESCBODY" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1962">
A parsed desc body reply.
*   \sa S2C_TYPE_DESCBODY. 
</member>
<member name="F:yiffurc.S2C_DESCBODY.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1966">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_DESCBODY.player_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1970">
Player short name. 
</member>
<member name="F:yiffurc.S2C_DESCBODY.body" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1972">
Desc body. 
</member>
<member name="T:yiffurc.S2C_DESCBODY2" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1989">
A parsed desc body reply.
*   \sa S2C_TYPE_DESCBODY2. 
</member>
<member name="F:yiffurc.S2C_DESCBODY2.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1993">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_DESCBODY2.badge" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1997">
Player badge. 
</member>
<member name="F:yiffurc.S2C_DESCBODY2.player_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="1999">
Player short name. 
</member>
<member name="F:yiffurc.S2C_DESCBODY2.body" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2001">
Desc body. 
</member>
<member name="T:yiffurc.S2C_WHODAILYMAX" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2018">
A parsed line of the who info.
*   \sa S2C_TYPE_WHODAILYMAX. 
</member>
<member name="F:yiffurc.S2C_WHODAILYMAX.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2022">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_WHODAILYMAX.daily_max" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2026">
Maximum number of players today. 
</member>
<member name="T:yiffurc.S2C_WHOMAX" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2041">
A parsed line of the who info.
*   \sa S2C_TYPE_WHOMAX. 
</member>
<member name="F:yiffurc.S2C_WHOMAX.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2045">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_WHOMAX.session_max" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2049">
Maximum number of players this session. 
</member>
<member name="T:yiffurc.S2C_WHOUPTIME" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2064">
A parsed line of the who info.
*   \sa S2C_TYPE_WHOUPTIME. 
</member>
<member name="F:yiffurc.S2C_WHOUPTIME.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2068">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_WHOUPTIME.players_online" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2072">
Number of players online. 
</member>
<member name="F:yiffurc.S2C_WHOUPTIME.days_up" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2074">
Days up. 
</member>
<member name="F:yiffurc.S2C_WHOUPTIME.hours_up" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2076">
Hours up. 
</member>
<member name="F:yiffurc.S2C_WHOUPTIME.minutes_up" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2078">
Minutes up. 
</member>
<member name="T:yiffurc.S2C_COOKIE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2093">
A parsed received cookie notification.
*   \sa S2C_TYPE_COOKIE. 
</member>
<member name="F:yiffurc.S2C_COOKIE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2097">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_COOKIE.from" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2101">
Player name. 
</member>
<member name="F:yiffurc.S2C_COOKIE.note" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2103">
Optional note. 
</member>
<member name="T:yiffurc.S2C_COOKIESREADY" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2120">
A parsed cookies ready notification.
*   \sa S2C_TYPE_COOKIESREADY. 
</member>
<member name="F:yiffurc.S2C_COOKIESREADY.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2124">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.S2C_INFOITEM" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2141">
A parsed line of the player info.
*   \sa S2C_TYPE_INFOITEM. 
</member>
<member name="F:yiffurc.S2C_INFOITEM.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2145">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_INFOITEM.item_name" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2149">
Name of the user item. 
</member>
<member name="F:yiffurc.S2C_INFOITEM.num_active" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2151">
Number of the items active. 
</member>
<member name="F:yiffurc.S2C_INFOITEM.num_inactive" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2153">
Number of the items inactive. 
</member>
<member name="T:yiffurc.S2C_INFOPLAYER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2169">
A parsed line of the player info.
*   \sa S2C_TYPE_INFOPLAYER. 
</member>
<member name="F:yiffurc.S2C_INFOPLAYER.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2173">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_INFOPLAYER.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2177">
Player name. 
</member>
<member name="F:yiffurc.S2C_INFOPLAYER.email" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2179">
Player email. 
</member>
<member name="T:yiffurc.S2C_SUMMON" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2196">
A parsed summon request.
*   \sa S2C_TYPE_SUMMON. 
</member>
<member name="F:yiffurc.S2C_SUMMON.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2200">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_SUMMON.from_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2204">
Player short name. 
</member>
<member name="F:yiffurc.S2C_SUMMON.from" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2206">
Player name. 
</member>
<member name="F:yiffurc.S2C_SUMMON.dest" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2208">
Destination. 
</member>
<member name="T:yiffurc.S2C_JOIN" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2226">
A parsed join request.
*   \sa S2C_TYPE_JOIN. 
</member>
<member name="F:yiffurc.S2C_JOIN.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2230">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_JOIN.from_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2234">
Player short name. 
</member>
<member name="F:yiffurc.S2C_JOIN.from" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2236">
Player name. 
</member>
<member name="T:yiffurc.S2C_WHISPERFAIL" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2253">
A parsed whisper fail response.
*   \sa S2C_TYPE_WHISPERFAIL. 
</member>
<member name="F:yiffurc.S2C_WHISPERFAIL.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2257">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_WHISPERFAIL.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2261">
Partial player name. 
</member>
<member name="T:yiffurc.S2C_WHISPERFAILEXACT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2277">
A parsed summon fail response.
*   \sa S2C_TYPE_SUMMONJOINFAIL. 
A parsed whisper fail response.
*   \sa S2C_TYPE_WHISPERFAILEXACT. 
</member>
<member name="F:yiffurc.S2C_WHISPERFAILEXACT.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2285">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_WHISPERFAILEXACT.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2289">
Exact player name. 
</member>
<member name="T:yiffurc.S2C_WHISPERFAILMORE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2305">
A parsed summon fail response.
*   \sa S2C_TYPE_SUMMONJOINFAIL. 
A parsed whisper fail response.
*   \sa S2C_TYPE_WHISPERFAILMORE. 
</member>
<member name="F:yiffurc.S2C_WHISPERFAILMORE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2313">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.S2C_YOUWHISPER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2330">
A parsed summon fail response.
*   \sa S2C_TYPE_SUMMONJOINFAILMORE. 
A parsed whisper echo.
*   \sa S2C_TYPE_YOUWHISPER. 
</member>
<member name="F:yiffurc.S2C_YOUWHISPER.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2338">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_YOUWHISPER.to_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2342">
Whisper recipient's short name. 
</member>
<member name="F:yiffurc.S2C_YOUWHISPER.to" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2344">
Whisper recipient. 
</member>
<member name="F:yiffurc.S2C_YOUWHISPER.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2346">
Whisper message. 
</member>
<member name="T:yiffurc.S2C_CHAT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2364">
A parsed generic chat message command.
*   \sa S2C_TYPE_CHAT. 
</member>
<member name="F:yiffurc.S2C_CHAT.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2368">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_CHAT.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2372">
Chat message. 
</member>
<member name="T:yiffurc.S2C_DISPLISTOPEN" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2388">
A parsed display list open command.
*   \sa S2C_TYPE_DISPLISTOPEN. 
</member>
<member name="F:yiffurc.S2C_DISPLISTOPEN.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2392">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.S2C_DISPLISTCLOSE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2409">
A parsed display list close command.
*   \sa S2C_TYPE_DISPLISTCLOSE. 
</member>
<member name="F:yiffurc.S2C_DISPLISTCLOSE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2413">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.S2C_SYNCCOLORCODE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2430">
A parsed synchronize color code command.
*   \sa S2C_TYPE_SYNCCOLORCODE. 
</member>
<member name="F:yiffurc.S2C_SYNCCOLORCODE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2434">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_SYNCCOLORCODE.colors" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2438">
colors 
</member>
<member name="F:yiffurc.S2C_SYNCCOLORCODE.gsd" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2440">
GSD 
</member>
<member name="T:yiffurc.S2C_YOUSHOUT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2456">
A parsed shout echo.
*   \sa S2C_TYPE_YOUSHOUT. 
</member>
<member name="F:yiffurc.S2C_YOUSHOUT.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2460">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_YOUSHOUT.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2464">
Shout message. 
</member>
<member name="T:yiffurc.S2C_YOUSAY" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2480">
A parsed speech echo.
*   \sa S2C_TYPE_YOUSAY. 
</member>
<member name="F:yiffurc.S2C_YOUSAY.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2484">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_YOUSAY.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2488">
Speech message. 
</member>
<member name="T:yiffurc.S2C_TRIGGER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2504">
A parsed trigger.
*   \sa S2C_TYPE_TRIGGER. 
</member>
<member name="F:yiffurc.S2C_TRIGGER.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2508">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_TRIGGER.x1" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2512">
DS-dependent value. 
</member>
<member name="F:yiffurc.S2C_TRIGGER.y1" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2514">
DS-dependent value. 
</member>
<member name="F:yiffurc.S2C_TRIGGER.x2" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2516">
DS-dependent value. 
</member>
<member name="F:yiffurc.S2C_TRIGGER.y2" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2518">
DS-dependent value. 
</member>
<member name="F:yiffurc.S2C_TRIGGER.count" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2521">
Number of triggers. 
</member>
<member name="T:yiffurc.S2C_TRIGGERSELF" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2539">
A parsed self-induced trigger.
*   \sa S2C_TYPE_TRIGGERSELF. 
</member>
<member name="F:yiffurc.S2C_TRIGGERSELF.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2543">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_TRIGGERSELF.x1" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2547">
DS-dependent value. 
</member>
<member name="F:yiffurc.S2C_TRIGGERSELF.y1" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2549">
DS-dependent value. 
</member>
<member name="F:yiffurc.S2C_TRIGGERSELF.x2" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2551">
DS-dependent value. 
</member>
<member name="F:yiffurc.S2C_TRIGGERSELF.y2" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2553">
DS-dependent value. 
</member>
<member name="F:yiffurc.S2C_TRIGGERSELF.count" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2556">
Number of triggers. 
</member>
<member name="T:yiffurc.S2C_VERREQ" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2574">
A parsed client version request.
*   \sa S2C_TYPE_VERREQ. 
</member>
<member name="F:yiffurc.S2C_VERREQ.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2578">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.S2C_PING" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2595">
A parsed ping request.
*   \sa S2C_TYPE_PING. 
</member>
<member name="F:yiffurc.S2C_PING.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2599">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_PING.value" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2603">
The value to pong back. 
</member>
<member name="T:yiffurc.S2C_WHISPERDENIED" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2618">
A parsed whisper denial.
*   \sa S2C_TYPE_WHISPERDENIED. 
</member>
<member name="F:yiffurc.S2C_WHISPERDENIED.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2622">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_WHISPERDENIED.player_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2626">
Player short name. 
</member>
<member name="F:yiffurc.S2C_WHISPERDENIED.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2628">
Player name. 
</member>
<member name="T:yiffurc.S2C_SUMMONJOINECHO" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2645">
A parsed summon/join request echo.
*   \sa S2C_TYPE_SUMMONJOINECHO. 
</member>
<member name="F:yiffurc.S2C_SUMMONJOINECHO.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2649">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_SUMMONJOINECHO.player_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2653">
Target player short name. 
</member>
<member name="F:yiffurc.S2C_SUMMONJOINECHO.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2655">
Target player name. 
</member>
<member name="T:yiffurc.S2C_DECLINED" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2672">
A parsed summon/join request declined.
*   \sa S2C_TYPE_DECLINED. 
</member>
<member name="F:yiffurc.S2C_DECLINED.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2676">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_DECLINED.player_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2680">
Target player short name. 
</member>
<member name="F:yiffurc.S2C_DECLINED.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2682">
Target player name. 
</member>
<member name="T:yiffurc.S2C_DECLINEECHO" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2699">
A parsed summon/join request decline echo.
*   \sa S2C_TYPE_DECLINEECHO. 
</member>
<member name="F:yiffurc.S2C_DECLINEECHO.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2703">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_DECLINEECHO.player_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2707">
Summoner/joiner's short name. 
</member>
<member name="F:yiffurc.S2C_DECLINEECHO.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2709">
Summoner/joiner. 
</member>
<member name="T:yiffurc.S2C_DECLINEFAIL" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2726">
A parsed decline echo with no requests.
*   \sa S2C_TYPE_DECLINEFAIL. 
</member>
<member name="F:yiffurc.S2C_DECLINEFAIL.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2730">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.S2C_JOINED" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2748">
A parsed summon request accepted.
*   \sa S2C_TYPE_JOINED. 
</member>
<member name="F:yiffurc.S2C_JOINED.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2752">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_JOINED.joiner_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2756">
Player joining you's short name. 
</member>
<member name="F:yiffurc.S2C_JOINED.joiner" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2758">
Player joining you. 
</member>
<member name="T:yiffurc.S2C_SUMMONED" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2775">
A parsed join request accepted.
*   \sa S2C_TYPE_SUMMONED. 
</member>
<member name="F:yiffurc.S2C_SUMMONED.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2779">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_SUMMONED.summoner_short" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2783">
Player summoning you's short name. 
</member>
<member name="F:yiffurc.S2C_SUMMONED.summoner" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2785">
Player summoning you. 
</member>
<member name="T:yiffurc.S2C_MSGBOX" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2802">
A parsed display message box.
*   \sa S2C_TYPE_MSGBOX. 
</member>
<member name="F:yiffurc.S2C_MSGBOX.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2806">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_MSGBOX.unk" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2810">
Unknown portion of message. 
</member>
<member name="F:yiffurc.S2C_MSGBOX.flags" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2812">
Win32 MessageBox() flags. 
</member>
<member name="F:yiffurc.S2C_MSGBOX.msg" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2814">
Message. 
</member>
<member name="T:yiffurc.S2C_AVSHOW" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2831">
A parsed avatar show.
*   \sa S2C_TYPE_AVSHOW. 
</member>
<member name="F:yiffurc.S2C_AVSHOW.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2835">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_AVSHOW.uid" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2839">
Avatar UID. 
</member>
<member name="F:yiffurc.S2C_AVSHOW.x" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2841">
Position. 
</member>
<member name="F:yiffurc.S2C_AVSHOW.y" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2843">
Position. 
</member>
<member name="F:yiffurc.S2C_AVSHOW.frame" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2845">
Frame. 
</member>
<member name="T:yiffurc.S2C_AVUPDATECOLORS" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2861">
A parsed avatar update colors.
*   \sa S2C_TYPE_AVUPDATECOLORS. 
</member>
<member name="F:yiffurc.S2C_AVUPDATECOLORS.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2865">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_AVUPDATECOLORS.uid" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2869">
Avatar UID. 
</member>
<member name="F:yiffurc.S2C_AVUPDATECOLORS.frame" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2871">
Frame. 
</member>
<member name="F:yiffurc.S2C_AVUPDATECOLORS.colors" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2873">
Colors. 
</member>
<member name="T:yiffurc.S2C_AVHIDE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2889">
A parsed hide avatar.
*   \sa S2C_TYPE_AVHIDE. 
</member>
<member name="F:yiffurc.S2C_AVHIDE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2893">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_AVHIDE.uid" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2897">
Avatar UID. 
</member>
<member name="F:yiffurc.S2C_AVHIDE.x" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2899">
Position. 
</member>
<member name="F:yiffurc.S2C_AVHIDE.y" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2901">
Position. 
</member>
<member name="T:yiffurc.S2C_PRINTPLAYERS" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2917">
A parsed hide avatar.
*   \sa S2C_TYPE_PRINTPLAYERS. 
</member>
<member name="F:yiffurc.S2C_PRINTPLAYERS.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2921">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_PRINTPLAYERS.listable" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2925">
Whether the player list should be shown. 
</member>
<member name="F:yiffurc.S2C_PRINTPLAYERS.name" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2927">
Name of dream. 
</member>
<member name="F:yiffurc.S2C_PRINTPLAYERS.caption" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2929">
Caption of dream. 
</member>
<member name="T:yiffurc.S2C_AVSHOW2" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2946">
A parsed avatar show (2).
*   \sa S2C_TYPE_AVSHOW2 
</member>
<member name="F:yiffurc.S2C_AVSHOW2.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2950">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_AVSHOW2.uid" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2954">
Avatar UID. 
</member>
<member name="F:yiffurc.S2C_AVSHOW2.x" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2956">
Position. 
</member>
<member name="F:yiffurc.S2C_AVSHOW2.y" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2958">
Position. 
</member>
<member name="F:yiffurc.S2C_AVSHOW2.frame" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2960">
Frame. 
</member>
<member name="T:yiffurc.S2C_DSEMIT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2976">
A parsed DS emit.
*   \sa S2C_TYPE_DSEMIT 
</member>
<member name="F:yiffurc.S2C_DSEMIT.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2980">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.S2C_DSEMIT.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="2984">
Message. 
</member>
<member name="T:yiffurc.S2C" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3000">
Parses a server -&gt; client line.
*  \param[in]     line     The raw command line.
*  \param[in]     len      Length of the command line.
*  \param[in,out] session  Session state data.
*  \returns A S2C structure that should
*           be freed with yiffurc_release( ) when no longer
*           needed, or \c 0 on error.
*
*  s2c_parse( ) relies on the \c level and \c s2c_state members of
*  the \a session structure to provide contextual information when
*  decoding \a line.  Both these members will also be modified by the
*  function.  You can check the value of \c level after the call to
*  see if the game connection has advanced.
*
*  If the connection is lost, you should memset all fields of \a session
*  to zero before using it again.
*
*  You should check the \c type member of the returned S2C object
*  then cast it to the appropriate structure for use.
*
*  \sa S2C, NET_SESSION, yiffurc_release( ), s2c_parse_peek( ) 
</member>
<member name="T:yiffurc.S2C" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3024">
Parses a server -&gt; client line without updating session state info.
*  \param[in]     line     The raw command line.
*  \param[in]     len      Length of the command line.
*  \param[in]   session    Session state data.
*  \returns A S2C structure that should
*           be freed with yiffurc_release( ) when no longer
*           needed, or \c 0 on error.
*  This is identical to s2c_parse( ) but \a session is considered const,
*  so lines parsed with this function won't count towards the network state.
*  \sa S2C, NET_SESSION, yiffurc_release( ), s2c_parse( ) 
</member>
<member name="M:yiffurc.s2c_enc_avdestroy(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3037">
\addtogroup s2c_encode Server-to-Client Encoding
*  Encode commands for the client.
*  \c s2c_enc_* functions return compiled client commands
*  terminated by "\n\0", so they may
*  be treated as normal C-strings.
*
*  Strings returned by the \c s2c_enc_* functions should
*  be released by yiffurc_release( ) when no longer needed.
*  \note \c s2c_enc_* functions that return "stock" commands
*        (commands that don't take any parameters) need
*        not be released, although releasing them will
*        do no harm.
* \{ 
Destroys an avatar.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_avcreate(System.UInt32,System.UInt32,System.UInt32,System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3055">
Creates an avatar.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).
*  \sa AVFRAME, av2frame, frame2av 
</member>
<member name="M:yiffurc.s2c_enc_avtween(System.UInt32,System.UInt32,System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3065">
Tweens an avatar to a new position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).
*  \sa AVFRAME, av2frame, frame2av 
</member>
<member name="M:yiffurc.s2c_enc_avupdatecolors(System.UInt32,System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3072">
Update an avatar's colors and frame.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).
*  \sa AVFRAME, av2frame, frame2av 
</member>
<member name="M:yiffurc.s2c_enc_avshow(System.UInt32,System.UInt32,System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3079">
Show an avatar at a position at a frame.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).
*  \sa AVFRAME, av2frame, frame2av 
</member>
<member name="M:yiffurc.s2c_enc_avhide(System.UInt32,System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3087">
Hide an avatar at a position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).
*  \sa AVFRAME, av2frame, frame2av 
</member>
<member name="M:yiffurc.s2c_enc_badge(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3094">
Sets badge data for the next chat message.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_camset(System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3098">
Sets the camera position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_camtween(System.UInt32,System.UInt32,System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3102">
Tweens the camera position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_chat(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3107">
Shows a generic chat message.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_createfail(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3111">
Fail on a player create request.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_specitag(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3115">
Sets specitag data for the next chat message.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_dreamclear(System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3120">
Clear a dream name at a position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_dreampos(System.UInt32,System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3124">
Create a dream name at a position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_dreampos2(System.UInt32,System.UInt32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3129">
Create a dream name and caption at a position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_fetchdream(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3135">
Instruct the client to fetch a dream from the fileserver.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_fetchpid(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3139">
Instruct the client to fetch a portrait from the fileserver.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_floorset(System.UInt32,System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3143">
Set the floor at a position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_floorsets(System.UInt32,yiffurc.IDXY!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3147">
Set the floors at several positions.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_footobj(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3151">
Set the object at the player's feet.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_invobj(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3155">
Sets the object in the player's inventory.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_macid(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3159">
Reply to a machine ID with a modified, scrambled machine ID.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_motd(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3163">
Display a line of the MOTD.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_objset(System.UInt32,System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3167">
Set the object at a position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_objsets(System.UInt32,yiffurc.IDXY!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3171">
Set the objects at several positions.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_onlineresult(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.Boolean)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3175">
Reply to an online check.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).

</member>
<member name="M:yiffurc.s2c_enc_playsong(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3180">
Play a song.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_playsound(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3184">
Play a sound.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_portrait(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3188">
Set the current portrait.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_synccolorcode(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3194">
Change the client's internal color code.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_trigger(System.UInt32,System.UInt32,System.UInt32,System.UInt32,System.UInt32,yiffurc.DSXY!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3199">
Trigger DS lines.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_triggerself(System.UInt32,System.UInt32,System.UInt32,System.UInt32,System.UInt32,yiffurc.DSXY!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3206">
Self-trigger DS lines.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_usebgfile(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3213">
Tell the client to use a specific background file.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_usedream(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3217">
Load a dream by name and id.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_usemapfile(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3222">
Tell the client to use a specific map file.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_varset(System.UInt32,System.UInt32,System.Int32*,System.Int32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3226">
Set a contiguous range of DS variables.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_msgbox(System.Int32,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3231">
Display a messagebox.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.s2c_enc_displaylistclose" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3239">
Close the display list (render buffered artifacts).
*  \returns Stock encoded command string.

</member>
<member name="M:yiffurc.s2c_enc_displaylistopen" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3243">
Open the display list (buffer artifacts).
*  \returns Stock encoded command string.

</member>
<member name="M:yiffurc.s2c_enc_dragonroar" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3247">
Play welcome.wav.
*  \returns Stock encoded command string.

</member>
<member name="M:yiffurc.s2c_enc_loginfail" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3251">
Fail a login attempt.
*  \warning Deprecated. See S2C_TYPE_LOGINFAIL for details.
*  \returns Stock encoded command string.

</member>
<member name="M:yiffurc.s2c_enc_motdend" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3256">
Signal the end of the MOTD.
*  \returns Stock encoded command string.

</member>
<member name="M:yiffurc.s2c_enc_pass" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3260">
Respond with a generic create/login success code.
*  \returns Stock encoded command string.

</member>
<member name="M:yiffurc.s2c_enc_ver" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3264">
Specify protocol version.
*  \returns Stock encoded command string.

</member>
<member name="M:yiffurc.s2c_enc_verreq" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3268">
Request client version.
*  \returns Stock encoded command string.

</member>
<member name="T:yiffurc.C2S" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3631">
Base parsed server protocol structure. 
</member>
<member name="F:yiffurc.C2S.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3634">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_KEEPALIVE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3640">
A parsed keep-alive.
*  \sa C2S_TYPE_KEEPALIVE. 
</member>
<member name="F:yiffurc.C2S_KEEPALIVE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3644">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_CREATEPLAYER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3661">
A parsed player create.
*  \sa C2S_TYPE_CREATEPLAYER. 
</member>
<member name="F:yiffurc.C2S_CREATEPLAYER.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3665">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_CREATEPLAYER.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3669">
Player name 
</member>
<member name="F:yiffurc.C2S_CREATEPLAYER.password" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3671">
Player password. 
</member>
<member name="F:yiffurc.C2S_CREATEPLAYER.email" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3673">
Player email. 
</member>
<member name="T:yiffurc.C2S_LOGIN" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3691">
A parsed login attempt.
*  \sa C2S_TYPE_LOGIN. 
</member>
<member name="F:yiffurc.C2S_LOGIN.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3695">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_LOGIN.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3699">
Player name 
</member>
<member name="F:yiffurc.C2S_LOGIN.password" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3701">
Player password. 
</member>
<member name="F:yiffurc.C2S_LOGIN.mac_id" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3703">
Scrambled machine ID (optional). 
</member>
<member name="T:yiffurc.C2S_QUIT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3721">
A parsed graceful disconnect.
*  \sa C2S_TYPE_QUIT. 
</member>
<member name="F:yiffurc.C2S_QUIT.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3725">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_SETCOLOR" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3742">
A parsed color set.
*  \sa C2S_TYPE_SETCOLOR. 
</member>
<member name="F:yiffurc.C2S_SETCOLOR.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3746">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_SETCOLOR.colors" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3750">
Colors 
</member>
<member name="F:yiffurc.C2S_SETCOLOR.gsd" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3752">
GSD. 
</member>
<member name="T:yiffurc.C2S_CHANGECOLOR" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3768">
A parsed color change.
*  \sa C2S_TYPE_CHANGECOLOR. 
</member>
<member name="F:yiffurc.C2S_CHANGECOLOR.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3772">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_CHANGECOLOR.colors" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3776">
Colors 
</member>
<member name="F:yiffurc.C2S_CHANGECOLOR.gsd" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3778">
GSD. 
</member>
<member name="T:yiffurc.C2S_SETDESC" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3794">
A parsed set description.
*  \sa C2S_TYPE_SETDESC. 
</member>
<member name="F:yiffurc.C2S_SETDESC.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3798">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_SETDESC.desc" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3802">
Description. 
</member>
<member name="T:yiffurc.C2S_CHANGEDESC" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3818">
A parsed change description.
*  \sa C2S_TYPE_CHANGEDESC. 
</member>
<member name="F:yiffurc.C2S_CHANGEDESC.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3822">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_CHANGEDESC.desc" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3826">
Description. 
</member>
<member name="T:yiffurc.C2S_PONG" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3842">
A parsed ping reply.
*  \sa C2S_TYPE_PONG. 
</member>
<member name="F:yiffurc.C2S_PONG.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3846">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_PONG.value" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3850">
Echoed ping value. 
</member>
<member name="T:yiffurc.C2S_VERSION" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3865">
A parsed client version.
*  \sa C2S_TYPE_VERSION. 
</member>
<member name="F:yiffurc.C2S_VERSION.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3869">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.MAJORMINOR" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3873">
Client version. 
</member>
<member name="T:yiffurc.C2S_WINVER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3888">
A parsed windows version.
*  \sa C2S_TYPE_WINVER. 
</member>
<member name="F:yiffurc.C2S_WINVER.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3892">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.MAJORMINOR" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3896">
Windows version. 
</member>
<member name="F:yiffurc.C2S_WINVER.build" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3898">
Windows build. 
</member>
<member name="T:yiffurc.MAJORMINOR" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3900">
DirectX version. 
</member>
<member name="T:yiffurc.C2S_DOWNLOADFINISHED" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3915">
A parsed dream done downloading
*  \sa C2S_TYPE_DOWNLOADFINISHED. 
</member>
<member name="F:yiffurc.C2S_DOWNLOADFINISHED.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3919">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_MOVE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3936">
A parsed player move.
*  \sa C2S_TYPE_MOVE. 
</member>
<member name="F:yiffurc.C2S_MOVE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3940">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.Move_Dir" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3944">
Direction. 
</member>
<member name="T:yiffurc.C2S_SITDOWN" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3959">
A parsed player sit.
*  \sa C2S_TYPE_SITDOWN. 
</member>
<member name="F:yiffurc.C2S_SITDOWN.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3963">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_STANDUP" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3980">
A parsed player stand.
*  \sa C2S_TYPE_STANDUP. 
</member>
<member name="F:yiffurc.C2S_STANDUP.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="3984">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_LIEDOWN" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4001">
A parsed player lie.
*  \sa C2S_TYPE_LIEDOWN. 
</member>
<member name="F:yiffurc.C2S_LIEDOWN.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4005">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_SITSTANDLIE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4022">
A parsed player sit/stand/lie cycle.
*  \sa C2S_TYPE_SITSTANDLIE. 
</member>
<member name="F:yiffurc.C2S_SITSTANDLIE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4026">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_SUMMON" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4043">
A parsed summon.
*  \sa C2S_TYPE_SUMMON. 
</member>
<member name="F:yiffurc.C2S_SUMMON.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4047">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_SUMMON.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4051">
Player to summon. 
</member>
<member name="F:yiffurc.C2S_SUMMON.exact" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4053">
\c true if \a player is an exact name. 
</member>
<member name="T:yiffurc.C2S_JOIN" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4069">
A parsed join.
*  \sa C2S_TYPE_JOIN. 
</member>
<member name="F:yiffurc.C2S_JOIN.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4073">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_JOIN.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4077">
Player to join. 
</member>
<member name="F:yiffurc.C2S_JOIN.exact" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4079">
\c true if \a player is an exact name. 
</member>
<member name="T:yiffurc.C2S_DECLINE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4095">
A parsed summon/join decline.
*  \sa C2S_TYPE_DECLINE. 
</member>
<member name="F:yiffurc.C2S_DECLINE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4099">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_GOMAP" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4116">
A parsed gomap.
*  \sa C2S_TYPE_GOMAP 
</member>
<member name="F:yiffurc.C2S_GOMAP.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4120">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_GOMAP.map_num" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4124">
Map number. 
</member>
<member name="T:yiffurc.C2S_GOBACK" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4139">
A parsed goback.
*  \sa C2S_TYPE_GOBACK 
</member>
<member name="F:yiffurc.C2S_GOBACK.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4143">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_GOALLEG" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4160">
A parsed allegria island teleport.
*  \sa C2S_TYPE_GOALLEG 
</member>
<member name="F:yiffurc.C2S_GOALLEG.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4164">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_GOVINCA" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4181">
A parsed vinca teleport.
*  \sa C2S_TYPE_GOVINCA 
</member>
<member name="F:yiffurc.C2S_GOVINCA.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4185">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_GOWELCOMEMAP" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4202">
A parsed welcome map teleport.
*  \sa C2S_TYPE_GOWELCOMEMAP 
</member>
<member name="F:yiffurc.C2S_GOWELCOMEMAP.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4206">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_SPEECH" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4223">
A parsed speech.
*  \sa C2S_TYPE_SPEECH 
</member>
<member name="F:yiffurc.C2S_SPEECH.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4227">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_SPEECH.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4231">
Message. 
</member>
<member name="T:yiffurc.C2S_SHOUT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4247">
A parsed shout.
*  \sa C2S_TYPE_SHOUT 
</member>
<member name="F:yiffurc.C2S_SHOUT.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4251">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_SHOUT.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4255">
Message. 
</member>
<member name="T:yiffurc.C2S_SHOUTTOGGLE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4271">
A parsed shout toggle.
*  \sa C2S_TYPE_SHOUTTOGGLE 
</member>
<member name="F:yiffurc.C2S_SHOUTTOGGLE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4275">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_WHISPER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4292">
A parsed whisper.
*  \sa C2S_TYPE_WHISPER 
</member>
<member name="F:yiffurc.C2S_WHISPER.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4296">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_WHISPER.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4300">
Player. 
</member>
<member name="F:yiffurc.C2S_WHISPER.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4302">
Message. 
</member>
<member name="F:yiffurc.C2S_WHISPER.exact" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4304">
\c true if \a player is an exact name. 
</member>
<member name="T:yiffurc.C2S_WINGS" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4321">
A parsed cycle wings.
*  \sa C2S_TYPE_WINGS 
</member>
<member name="F:yiffurc.C2S_WINGS.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4325">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_DRAGON" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4342">
A parsed toggle dragon.
*  \sa C2S_TYPE_WINGS 
</member>
<member name="F:yiffurc.C2S_DRAGON.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4346">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_PHOENIX" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4363">
A parsed toggle phoenix.
*  \sa C2S_TYPE_PHOENIX 
</member>
<member name="F:yiffurc.C2S_PHOENIX.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4367">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_PORTRAITCHANGE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4384">
A parsed cycle portraits.
*  \sa C2S_TYPE_PORTRAITCHANGE 
</member>
<member name="F:yiffurc.C2S_PORTRAITCHANGE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4388">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_EAGLE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4405">
A parsed eagle toggle.
*  \sa C2S_TYPE_EAGLE 
</member>
<member name="F:yiffurc.C2S_EAGLE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4409">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_GRYFFE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4426">
A parsed gryffe toggle.
*  \sa C2S_TYPE_GRYFFE 
</member>
<member name="F:yiffurc.C2S_GRYFFE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4430">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_DRAGONBREATH" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4447">
A parsed dragon breath.
*  \sa C2S_TYPE_DRAGONBREATH 
</member>
<member name="F:yiffurc.C2S_DRAGONBREATH.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4451">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_PHOENIXFLAME" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4468">
A parsed cycle wings.
*  \sa C2S_TYPE_PHOENIXFLAME 
</member>
<member name="F:yiffurc.C2S_PHOENIXFLAME.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4472">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_LOOKPOS" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4489">
A parsed lookat by position.
*  \sa C2S_TYPE_LOOKPOS 
</member>
<member name="F:yiffurc.C2S_LOOKPOS.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4493">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_LOOKPOS.x" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4497">
Look position. 
</member>
<member name="F:yiffurc.C2S_LOOKPOS.y" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4499">
Look position. 
</member>
<member name="T:yiffurc.C2S_WHOINFO" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4514">
A parsed who info request.
*  \sa C2S_TYPE_WHOINFO 
</member>
<member name="F:yiffurc.C2S_WHOINFO.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4518">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_PICKUP" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4535">
A parsed pickup object.
*  \sa C2S_TYPE_PICKUP 
</member>
<member name="F:yiffurc.C2S_PICKUP.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4539">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_USE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4556">
A parsed use object.
*  \sa C2S_TYPE_USE 
</member>
<member name="F:yiffurc.C2S_USE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4560">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_FDL" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4577">
A parsed follow dream link.
*  \sa C2S_TYPE_FDL 
</member>
<member name="F:yiffurc.C2S_FDL.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4581">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_FDL.url" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4585">
Dream URL 
</member>
<member name="T:yiffurc.C2S_LOOKPLAYER" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4601">
A parsed lookat by name.
*  \sa C2S_TYPE_LOOKPLAYER 
</member>
<member name="F:yiffurc.C2S_LOOKPLAYER.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4605">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_LOOKPLAYER.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4609">
Player. 
</member>
<member name="F:yiffurc.C2S_LOOKPLAYER.exact" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4611">
\c true if \a player is an exact name. 
</member>
<member name="T:yiffurc.C2S_ONLINECHECK" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4627">
A parsed online check.
*  \sa C2S_TYPE_ONLINECHECK 
</member>
<member name="F:yiffurc.C2S_ONLINECHECK.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4631">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_ONLINECHECK.player" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4635">
Player. 
</member>
<member name="T:yiffurc.C2S_EMOTE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4651">
A parsed emote.
*  \sa C2S_TYPE_EMOTE 
</member>
<member name="F:yiffurc.C2S_EMOTE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4655">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.C2S_EMOTE.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4659">
Emote message. 
</member>
<member name="T:yiffurc.C2S_TURNLEFT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4675">
A parsed rotate left.
*  \sa C2S_TYPE_TURNLEFT 
</member>
<member name="F:yiffurc.C2S_TURNLEFT.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4679">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S_TURNRIGHT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4697">
A parsed rotate right.
*  \sa C2S_TYPE_TURNRIGHT 
</member>
<member name="F:yiffurc.C2S_TURNRIGHT.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4701">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.C2S" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4719">
Parses a client -&gt; server line.
*  \param[in]     line     The raw command line.
*  \param[in]     len      Length of the command line.
*  \param[in,out] session  Session state data.
*  \returns A C2S structure that should
*           be freed with yiffurc_release( ) when no longer
*           needed, or \c 0 on error.
*
*  c2s_parse( ) relies on the \c level and \c c2s_state members of
*  the \a session structure to provide contextual information when
*  decoding \a line.  Both these members will also be modified by the
*  function.  You can check the value of \c level after the call to
*  see if the game connection has advanced.
*  If the function decodes a login attempt while \c level is
*  \c NET_LEVEL_CONNECTED or \c NET_LEVEL_MOTEND, it will set
*  \c level to a new value of \c NET_LEVEL_LOGGINGIN.
*
*  If the connection is lost, you should memset all fields of \a session
*  to zero before using it again.
*
*  You should check the \c type member of the returned C2S object
*  then cast it to the appropriate structure for use.
*
*  \sa C2S, NET_SESSION, yiffurc_release( ), c2s_parse_peek( )

</member>
<member name="T:yiffurc.C2S" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4747">
Parses a client -&gt; server line without updating session state info.
*  \param[in]     line     The raw command line.
*  \param[in]     len      Length of the command line.
*  \param[in]   session    Session state data.
*  \returns A C2S structure that should
*           be freed with yiffurc_release( ) when no longer
*           needed, or \c 0 on error.
*
*  This is identical to c2s_parse( ) but \a session is considered const,
*  so lines parsed with this function won't count towards the network state.
*  \sa C2S, NET_SESSION, yiffurc_release( ), c2s_parse( )

</member>
<member name="M:yiffurc.c2s_enc_create(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4762">
\addtogroup c2s_encode Client-to-Server Encoding
*  Encode commands for the server.
*  \c c2s_enc_* functions return compiled client commands
*  terminated by "\n\0", so they may
*  be treated as normal C-strings.
*
*  Strings returned by the \c c2s_enc_* functions should
*  be released by yiffurc_release( ) when no longer needed.
*  \note \c c2s_enc_* functions that return "stock" commands
*        (commands that don't take any parameters) need
*        not be released, although releasing them will
*        do no harm.
*  \{ 
Registers a player account.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_login(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4781">
Logs in with a player account and optional machine ID.
*  \a macid should already be scrambled.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).
*  \sa auth 
</member>
<member name="M:yiffurc.c2s_enc_speech(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4788">
Sends out regular speech.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_shout(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4792">
Sends out a shout.
*  A null \a shout parameter will toggle shouts on and off.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_whisper(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4797">
Sends out a whisper.
*  The \a to parameter will match any player name beginning
*  the same way.  You can prepend \a to with a % character to make
*  the name exact.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_emote(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4804">
Sends out an emote.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_fdl(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4808">
Follows a furc (dream) url.
*  Furc urls have the form <tt>furc://dreamname:caption</tt>
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_gomap(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4813">
Teleport to a server map.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_join(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4817">
Join a player.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_summon(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4821">
Summon a player.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_lookpos(System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4825">
Look at the player at a specific position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_onlinecheck(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4829">
Check if a player is online or not.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_changecolor(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4833">
Change your player colors (SS).
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_setcolor(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4838">
Set your player colors after login.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_changedesc(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4843">
Change your player description.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_setdesc(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4847">
Set your player desc.
*  Essentially a synonym for c2s_enc_changedesc( ).
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_pong(System.Int32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4852">
Reply to a ping (]marco) from the server.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_version(yiffurc.MAJORMINOR)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4856">
Tell the server your client version.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_winver(yiffurc.MAJORMINOR,System.UInt32,yiffurc.MAJORMINOR)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4860">
Tell the server your windows version, build, and directx version.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_regdream(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4865">
Register a dream you uploaded.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_emit(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4869">
Send out a dream emit.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_emitloud(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4873">
Send out a dream emit to the whole map.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_eject(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4877">
Eject a player from the dream.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_share(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4881">
Share dream control with a player.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_unshare(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4885">
Unshare dream control with a player.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_givecookie(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4889">
Give a player an active cookie with optional message.
*  You can prepend \a player with a % character to make the name exact.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_makecookie(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4895">
Give a player an inactive cookie with optional message.
*  You can prepend \a player with a % character to make the name exact.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_lookplayer(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4901">
Looks at a player by name.
*  You can prepend \a player with a % character to make the name exact.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). 
</member>
<member name="M:yiffurc.c2s_enc_cookiesaccept" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4909">
Begin accepting cookies.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_cookiesreject" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4912">
Begin rejecting cookies.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_eatcookie" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4915">
Eat an active cookie.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_munchcookie" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4918">
Eat an active cookie loudly.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_shouttoggle" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4921">
Toggle hearing shouts.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_decline" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4924">
Decline a summon or join request.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_pickup" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4927">
Pick up the object at your feet.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_goalleg" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4930">
Go to allegria island.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_goback" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4933">
Return to the last map visited.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_govinca" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4936">
Go to the vinca.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_info" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4939">
Get player info.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_onlineprx" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4942">
Tell the server to ignore non-online-check commands.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_use" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4945">
Use the object in your inventory.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_downloadfinished" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4948">
Inform the server you're done downloading the dream.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_whichinfo" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4951">
Get server process info.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_whoinfo" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4954">
Get who info.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_dragonbreath" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4957">
Make dragon breath.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_gryffeeagle" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4960">
Turn into an eagle.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_phoenixflame" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4963">
Make phoenix flames.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_portraitchange" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4966">
Cycle through portraits.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_wings" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4969">
Cycle through wings.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_turnleft" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4972">
Rotate the player left.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_turnright" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4975">
Rotate the player right.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_sitstandlie" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4978">
Cycle through sitting, standing, and lying positions.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_liedown" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4981">
Lay the player down.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_move(yiffurc.Move_Dir)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4984">
Move one space in a direction.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_sitdown" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4987">
Sit down.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_standup" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4990">
Stand up.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_showdreamurl" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4993">
Get the furc (dream) url for the current dream.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_keepalive" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4996">
Tell the server to keep your connection alive before logging in.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_quit" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="4999">
Graceful disconnect.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_uploadrequest" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5002">
Request to upload a dream.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_time" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5005">
Get the current server time (FST).
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_whisperson" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5008">
Toggle on whispers. (SS)
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_whispersoff" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5011">
Toggle off whispers. (SS)
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_cookieson" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5014">
Enable cookies for the current dream.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_cookiesoff" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5017">
Disable cookies for the current dream.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_dreampadsall" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5020">
Allow dreampad uploads to everyone in the dream.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_dreampadsowner" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5023">
Allow dreampad uploads to only the owner in the dream.
*  \returns Stock encoded command string.

</member>
<member name="M:yiffurc.c2s_enc_dreampadsshared" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5027">
Allow dreampad uploads to only those with shared control in the dream.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_shieldsummon" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5030">
Toggle making summoned furres appear at start point in the dream.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_shieldurl" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5033">
Toggle letting players access the dream through dream urls.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_unloaddream" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5036">
Unload an uploaded dream.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_unloadalldreams" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5039">
Unload all uploaded dreams.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_dreamuploadsall" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5042">
Allow everyone to upload dreams anywhere.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_dreamuploadsowner" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5045">
Allow only the owner to upload dreams anywhere.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_dreamuploadsshared" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5048">
Allow only those with share control to upload dreams anywhere.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_dreamuploadssponsors" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5051">
Allow only silver sponsors to upload dreams anywhere.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_joinbeekchannel" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5054">
Join the beekin channel.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_leavebeekchannel" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5057">
Leave the beekin channel. (B)
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_listbeekcommands" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5060">
List available beeking commands. (B)
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_listf" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5063">
List help requests and on-duty beekins. (B)
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_joinguardchannel" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5066">
Join the guardian channel. (B)
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_ondutyinfo" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5069">
Show the number of on-duty beekins. (B)
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_listrequests" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5072">
List outstanding help requests. (B)
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_gowelcomemap" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5075">
Go to the welcome map.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_dragon" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5078">
Toggle dragon form.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_phoenix" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5081">
Toggle phoenix form.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_portraits" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5084">
Cycles portraits.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.c2s_enc_phoenixflames" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5087">
Perform phoenix flames.
*  \returns Stock encoded command string. 
</member>
<member name="T:yiffurc.XFER_NET_SESSION" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5091">
\} c2s_encode 
\} c2s 
\addtogroup xfer File Transfer Connections
* \{ 
Transfer network session information.
* Holds state information used in file transfer parsing.
* \sa xfer_s2c_parse 
</member>
<member name="T:yiffurc.Xfer_Net_Level" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5102">
Network Level.
   *  \sa Xfer_Net_Level 
</member>
<member name="T:yiffurc.XFER_S2C" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5232">
Base parsed transfer client protocol structure. 
</member>
<member name="F:yiffurc.XFER_S2C.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5235">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="T:yiffurc.XFER_S2C_GREETING" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5241">
A parsed greeting.
*  \sa XFER_S2C_TYPE_GREETING. 
</member>
<member name="F:yiffurc.XFER_S2C_GREETING.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5245">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.XFER_S2C_GREETING.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5249">
Human-readable message. 
</member>
<member name="T:yiffurc.XFER_S2C_USEROK" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5265">
A parsed user-name ACK.
*  \sa XFER_S2C_TYPE_USEROK. 
</member>
<member name="F:yiffurc.XFER_S2C_USEROK.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5269">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.XFER_S2C_USEROK.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5273">
Human-readable message. 
</member>
<member name="T:yiffurc.XFER_S2C_LOGGEDIN" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5289">
A parsed logged in.
*  \sa XFER_S2C_TYPE_LOGGEDIN. 
</member>
<member name="F:yiffurc.XFER_S2C_LOGGEDIN.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5293">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.XFER_S2C_LOGGEDIN.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5297">
Human-readable message. 
</member>
<member name="T:yiffurc.XFER_S2C_UFPSET" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5313">
A parsed SETR response.
*  \sa XFER_S2C_TYPE_UFPSET. 
</member>
<member name="F:yiffurc.XFER_S2C_UFPSET.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5317">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.XFER_S2C_UFPSET.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5321">
Human-readable message. 
</member>
<member name="T:yiffurc.XFER_S2C_UPLOADOK" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5337">
A parsed ready to upload.
*  \sa XFER_S2C_TYPE_UPLOADOK. 
</member>
<member name="F:yiffurc.XFER_S2C_UPLOADOK.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5341">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.XFER_S2C_UPLOADOK.unk1" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5345">
Unknown. 
</member>
<member name="F:yiffurc.XFER_S2C_UPLOADOK.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5347">
Human-readable message. 
</member>
<member name="T:yiffurc.XFER_S2C_CHUNKOK" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5363">
A parsed upload chunk ACK.
*  \sa XFER_S2C_TYPE_CHUNKOK. 
</member>
<member name="F:yiffurc.XFER_S2C_CHUNKOK.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5367">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.XFER_S2C_CHUNKOK.chunk_id" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5371">
Chunks id. 
</member>
<member name="T:yiffurc.XFER_S2C_UPLOADDONE" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5386">
A parsed upload finished.
*  \sa XFER_S2C_TYPE_UPLOADDONE. 
</member>
<member name="F:yiffurc.XFER_S2C_UPLOADDONE.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5390">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.XFER_S2C_UPLOADDONE.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5394">
Human-readable message. 
</member>
<member name="T:yiffurc.XFER_S2C_LOGGEDOUT" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5410">
A parsed logged out.
*  \sa XFER_S2C_TYPE_LOGGEDOUT. 
</member>
<member name="F:yiffurc.XFER_S2C_LOGGEDOUT.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5414">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.XFER_S2C_LOGGEDOUT.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5418">
Human-readable message. 
</member>
<member name="T:yiffurc.XFER_S2C_DOWNLOADINFO" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5434">
A parsed ready to upload.
*  \sa XFER_S2C_TYPE_DOWNLOADINFO. 
</member>
<member name="F:yiffurc.XFER_S2C_DOWNLOADINFO.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5438">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.XFER_S2C_DOWNLOADINFO.file_size" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5442">
File size. 
</member>
<member name="F:yiffurc.XFER_S2C_DOWNLOADINFO.unk1" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5444">
Unknown. 
</member>
<member name="F:yiffurc.XFER_S2C_DOWNLOADINFO.chunk_size" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5446">
Chunk size. 
</member>
<member name="F:yiffurc.XFER_S2C_DOWNLOADINFO.unk2" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5448">
Unknown. 
</member>
<member name="T:yiffurc.XFER_S2C_CHUNKHEAD" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5463">
A parsed chunk header.
*  \sa XFER_S2C_TYPE_CHUNKHEAD. 
</member>
<member name="F:yiffurc.XFER_S2C_CHUNKHEAD.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5467">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.XFER_S2C_CHUNKHEAD.chunk_id" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5471">
Chunk ID. 
</member>
<member name="F:yiffurc.XFER_S2C_CHUNKHEAD.chunk_crc32" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5473">
Chunk CRC32. 
</member>
<member name="T:yiffurc.XFER_S2C_INACTIVITY" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5488">
A parsed inactivity disconnect.
*  \sa XFER_S2C_TYPE_INACTIVITY. 
</member>
<member name="F:yiffurc.XFER_S2C_INACTIVITY.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5492">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.XFER_S2C_INACTIVITY.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5496">
Humean-readable message. 
</member>
<member name="T:yiffurc.XFER_S2C_FILENOTFOUND" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5512">
A parsed file not found.
*  \sa XFER_S2C_TYPE_FILENOTFOUND. 
</member>
<member name="F:yiffurc.XFER_S2C_FILENOTFOUND.type" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5516">
The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. 
</member>
<member name="F:yiffurc.XFER_S2C_FILENOTFOUND.message" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5520">
Humean-readable message. 
</member>
<member name="T:yiffurc.XFER_S2C" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5536">
Parses a transfer server -&gt; client line.
*  \param[in]     line     The raw command line.
*  \param[in]     len      Length of the line.
*  \param[in,out] session  Session state data.
*  \returns A XFER_S2C structure that should
*           be freed with yiffurc_release( ) when no longer
*           needed, or \c 0 on error.
*
*  xfer_s2c_parse( ) relies on the \c level and \c s2c_state members of
*  the \a session structure to provide contextual information when
*  decoding \a line.  Both these members will also be modified by the
*  function.  You can check the value of \c level after the call to
*  see if the file server connection has advanced.
*
*  If the connection is lost, you should memset all fields of \a session
*  to zero before using it again.
*
*  You should check the \c type member of the returned XFER_S2C object
*  then cast it to the appropriate structure for use.
*
*  \sa XFER_S2C, XFER_NET_SESSION, yiffurc_release( ), xfer_s2c_parse_peek( ) 
</member>
<member name="T:yiffurc.XFER_S2C" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5560">
Parses a transfer server -&gt; client line. without updating session state info.
*  \param[in]     line     The raw command line.
*  \param[in]     len      Length of the command line.
*  \param[in]   session    Session state data.
*  \returns A XFER_S2C structure that should
*           be freed with yiffurc_release( ) when no longer
*           needed, or \c 0 on error.
*
*  This is identical to xfer_s2c_parse( ) but \a session is considered const,
*  so lines parsed with this function won't count towards the network state.
*  \sa XFER_S2C, XFER_NET_SESSION, yiffurc_release( ), xfer_s2c_parse( )  
</member>
<member name="M:yiffurc.xfer_c2s_enc_user(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5575">
\addtogroup xfer_c2s_encode Client-to-Server Encoding
*  Encode commands for the file server.
*  \c xfer_c2s_enc_* functions (with the exception of
*  xfer_c2s_enc_chunkhead( )) return compiled client commands
*  terminated by a "\n\0", so they may
*  be treated as normal C-strings.
*
*  Strings returned by the \c xfer_c2s_enc_* functions should
*  be released by xfer_yiffurc_release( ) when no longer needed.
*  \note \c xfer_c2s_enc_* functions that return "stock" commands
*        (commands that don't take any parameters) need
*        not be released, although releasing them will
*        do no harm.
* \{ 
Set user name.
*  \returns Encoded command string that should be freed
*           with xfer_yiffurc_release( ). 
</member>
<member name="M:yiffurc.xfer_c2s_enc_password(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5594">
Set password.
*  \returns Encoded command string that should be freed
*           with xfer_yiffurc_release( ). 
</member>
<member name="M:yiffurc.xfer_c2s_enc_setr(System.Int32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5599">
SETR command.
*  \param   unk   Normally 0.
*  \returns Encoded command string that should be freed
*           with xfer_yiffurc_release( ). 
</member>
<member name="M:yiffurc.xfer_c2s_enc_uploadinfo(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.Int32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5605">
Tell the server about the file being uploaded.
*  \returns Encoded command string that should be freed
*           with xfer_yiffurc_release( ). 
</member>
<member name="M:yiffurc.xfer_c2s_enc_upload2pkg(System.UInt32,System.Int32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5611">
Make the upload part of a (dream?) package.
*  \param   unk   Normally 0.
*  \returns Encoded command string that should be freed
*           with xfer_yiffurc_release( ). 
</member>
<member name="M:yiffurc.xfer_c2s_enc_chunkhead(System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5618">
Send information about chunk data.
*  \returns Encoded 10-unsigned char sequence that should be freed
*           with xfer_yiffurc_release( ). 
</member>
<member name="M:yiffurc.xfer_c2s_enc_atomicfetch(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.Int32,System.UInt32,System.Int32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5624">
Atomic login and request to download a package.
*  \returns Encoded command string that should be freed
*           with xfer_yiffurc_release( ). 
</member>
<member name="M:yiffurc.xfer_c2s_enc_bye" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5636">
Log out of file server.
*  \returns Stock encoded command string. 
</member>
<member name="M:yiffurc.net_scan(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.Int32,BTEllipsis)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\net.h" line="5640">
\} xfer_c2s_encode 
\} xfer 
\addtogroup scanning Simple Command Scanning
\{ 
Extracts common fields from a raw network string.
*  net_scan performs a function similar to \c scanf, albeit naively.
*  \a pattern is a string consisting of literal characters and 
*  format specifiers. As in \c scanf, format specifiers start with a '%'
*  character followed by optional digits restricting the length of the field
*  then a character indicating the type.
*  \code %{length}[type] \endcode
*  The following types are recognized, with certain ones accepting length specifiers:
*           - \c %c One character. Outputs to a char buffer.
*              - \c %3c 3 characters. Outputs to a char buffer.
*              - \c %*c One or more characters (matches to the end of the string). Outputs to a C-string.
*           - \c %s A furcadia byte-string (a base-220 digit followed by a string). Outputs to a C-string.
*           - \c %k An old-style, base-95 color code (not including the 3 tailing GSD bytes). Outputs to a C-string.
*           - \c %K base-220 color code (not including the 3 tailing GSD bytes). Outputs to a C-string.
*           - \c %n A single-digit base-95 number. Outputs to an int buffer.
*              - \c %3n A 3-digit base-95 number.
*           - \c %N A single-digit base-220 number. Outputs to an int buffer.
*              - \c %3N A 3-digit base-220 number.
*           - \c %% Literal '%' character. No output buffer expected.
*  \param[in]  pattern  The pattern string (must be null-terminated).
*  \param[in]  subj     The string to scan.
*  \param[in]  subj_len Length of the subject string, or -1 if null-terminated.
*  \param[out] ...      Pointers to locations to store fields. The type of variable expected
*                       depends on the format specifier.
*  \returns Nonzero on success.

</member>
<member name="T:yiffurc.Av_Class" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\avatar.h" line="39">
Avatar classes.
* These are also the frame numbers of the first frame in
* their series.
* Note that 'BW' = bat wings and 'FW' = butterfly wings. 
</member>
<member name="T:yiffurc.Av_Pose" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\avatar.h" line="124">
Avatar poses. 
</member>
<member name="T:yiffurc.Av_Dir" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\avatar.h" line="134">
Avatar directions (direction they face). 
</member>
<member name="T:yiffurc.AVFRAME" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\avatar.h" line="143">
Describes an avatar frame.
* \sa av2frame, frame2av 
</member>
<member name="T:yiffurc.Av_Class" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\avatar.h" line="147">
The class of avatar (e.g., equine, dragon, lapine with classic wings). 
</member>
<member name="T:yiffurc.Av_Pose" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\avatar.h" line="149">
The pose (e.g., standing, walking, sitting). 
</member>
<member name="T:yiffurc.Av_Dir" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\avatar.h" line="151">
The direction it faces (e.g., SW, SE, NW). 
</member>
<member name="M:yiffurc.frame2av(System.UInt32,yiffurc.AVFRAME*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\avatar.h" line="155">
Extracts avatar information from a frame number.
* \param[in]    frame   Avatar frame number.
* \param[out]    out    Extracted avatar information.
* \return \c false if the frame was out of range.
* \sa AVFRAME, S2C_TYPE_AVCREATE, S2C_TYPE_AVTWEEN 
</member>
<member name="M:yiffurc.av2frame(yiffurc.AVFRAME!System.Runtime.CompilerServices.IsConst*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\avatar.h" line="162">
Converts avatar information into a frame number.
* \param[in]    av      Avatar information.
* \return The frame number.
* \sa AVFRAME, s2c_enc_avset, s2c_enc_avtween 
</member>
<member name="M:yiffurc.auth_scramble(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.Int32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="40">
Scrambles a sequence of bytes.
*  \param[out]    dst   Buffer to hold scrambled unsigned chars.
*  \param[in]     src   Source buffer to scramble.
*  \param[in]     len   Size of buffers.
*  \returns dst pointer

</member>
<member name="M:yiffurc.auth_descramble(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte*,System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.Int32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="47">
Unscrambles a sequence of bytes.
*  \param[out]    dst   Buffer to hold unscrambled unsigned chars.
*  \param[in]     src   Source buffer to unscramble.
*  \param[in]     len   Size of buffers.
*  \returns dst pointer

</member>
<member name="T:yiffurc.AUTH_DIGEST128" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="55">
A 128-bit digest. 
</member>
<member name="M:yiffurc.auth_digest128(System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte!System.Runtime.CompilerServices.IsConst*,System.Int32,yiffurc.AUTH_DIGEST128*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="62">
Generates a 128-bit digest of a sequence of unsigned chars.
*  \param[in]     data     Bytes to digest.
*  \param[in]     len      Number of unsigned chars to digest.
*  \param[out]    digest   128-bit digest.

</member>
<member name="T:yiffurc.AUTH_MACHINEVARS" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="70">
Client machine variables. 
</member>
<member name="F:yiffurc.AUTH_MACHINEVARS.ip4" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="73">
IP=, the host machine's public IP address, in network order. 
</member>
<member name="F:yiffurc.AUTH_MACHINEVARS.num_drives" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="75">
Number of fixed drives. 
</member>
<member name="F:yiffurc.AUTH_MACHINEVARS.volume_sn" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="77">
H= Serial numbers of each major drive (C:\ D:\ E:\ A:\) 
</member>
<member name="F:yiffurc.AUTH_MACHINEVARS.time" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="79">
R= Current time. 
</member>
<member name="F:yiffurc.AUTH_MACHINEVARS.total_free_clusters" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="81">
D= Summed free clusters of all the drives. 
</member>
<member name="F:yiffurc.AUTH_MACHINEVARS.h_e_z" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="83">
U= Should be some fixed hardware number. 
</member>
<member name="F:yiffurc.AUTH_MACHINEVARS.screen_width" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="85">
M= 
</member>
<member name="F:yiffurc.AUTH_MACHINEVARS.screen_height" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="87">
M= 
</member>
<member name="M:yiffurc.auth_stringclient(yiffurc.AUTH_MACHINEVARS!System.Runtime.CompilerServices.IsConst*,System.Boolean)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="93">
Generate a machine ID string from the client.
*  \param[in]     vars  Client machine variables.
*  \param[in]  scramble Set to \c false to return the string unscrambled.
*  \returns A machine ID string that should be
*           freed with yiffurc_release() when no longer needed. 
</member>
<member name="M:yiffurc.auth_stringserver(yiffurc.AUTH_MACHINEVARS!System.Runtime.CompilerServices.IsConst*,System.Boolean)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\auth.h" line="101">
Generate a machine ID string from the server.
*  \param[in]     vars  Client machine variables.
*  \param[in]  scramble Set to \c false to return the string unscrambled.
*  \returns A machine ID string that should be
*           freed with yiffurc_release() when no longer needed. 
</member>
<member name="T:yiffurc.FshStream" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\fsh.hpp" line="42">
Class to pull shapes from an FSH file stream.
*  define BIG_ENDIAN before including this to do endian correction. 
</member>
<member name="T:yiffurc.FshStream.SHAPEINFO" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\fsh.hpp" line="49">
Shape information. 
</member>
<member name="F:yiffurc.FshStream.SHAPEINFO.width" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\fsh.hpp" line="52">
Width (in pixels) 
</member>
<member name="F:yiffurc.FshStream.SHAPEINFO.height" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\fsh.hpp" line="54">
Height (in pixels) 
</member>
<member name="F:yiffurc.FshStream.SHAPEINFO.off_x" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\fsh.hpp" line="56">
Rendering x offset 
</member>
<member name="F:yiffurc.FshStream.SHAPEINFO.off_y" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\fsh.hpp" line="58">
Rendering y offset 
</member>
<member name="M:yiffurc.FshStream.#ctor(std.basic_istream&lt;System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte,std.char_traits{System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte}&gt;*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\fsh.hpp" line="65">
Creates an FshStream with an istream object.
   * The istream object will not be closed when the FshStream object is
   * destroyed. 
</member>
<member name="M:yiffurc.FshStream.good" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\fsh.hpp" line="71">
Check whether the stream is good for reading. 
</member>
<member name="M:yiffurc.FshStream.getShapeInfo(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\fsh.hpp" line="73">
Retrieve information on a shape. 
</member>
<member name="M:yiffurc.FshStream.copyShapeBits(System.UInt32,System.Void*)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\fsh.hpp" line="75">
Copy shape image data.
   * \a buf will be filled with 8-bit palette indices.
   * \a buf should be at least <tt>shapeinfo.width * shapeinfo.height</tt> bytes. 
</member>
<member name="M:yiffurc.FshStream.copyShapeBitsPadded(System.UInt32,System.Void*,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\fsh.hpp" line="79">
Copy shape image data.
   * \a buf will be filled with 8-bit palette indices.
   * \a pad bytes at the end of each row will be left untouched.
   * \a buf should be at least <tt>(shapeinfo.width + \a pad) * shapeinfo.height</tt> bytes. 
</member>
<member name="M:yiffurc.FshStream.getNumShapes" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\fsh.hpp" line="84">
Returns the number of shapes in the FSH stream. 
</member>
<member name="T:yiffurc.FshFile" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\fsh.hpp" line="234">
Descendant of FshStream that reads shapes specifically from a file name. 
</member>
<member name="T:yiffurc.ColorCode" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\util.hpp" line="47">
colorcode base class. 
</member>
<member name="T:yiffurc.ColorCodePal" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\util.hpp" line="65">
Paletted colorcode class. 
</member>
<member name="T:yiffurc.PixelB8G8R8" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\colors.hpp" line="57">
Windows Bitmap Pixel.
* Traits/data type for a windows (BGR-order) 24-bit pixel.
* If you want to define your own pixel type for use with the other
* classes here, you should implement all the methods in this type. 
</member>
<member name="T:yiffurc.PixelR8G8B8" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\colors.hpp" line="81">
Traditional, 24-bit RGB pixel.
* Traits/data type for a regular (RGB-order) 24-bit pixel.
* If you want to define your own pixel type for use with the other
* classes here, you should implement all the methods in this type. 
</member>
<member name="T:yiffurc.PixelR8G8B8X8" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\colors.hpp" line="105">
24-bit RGB pixel aligned to 32-bits.
* Traits/data type for a regular (RGB-order), 32-bit aligned 24-bit pixel.
* If you want to define your own pixel type for use with the other
* classes here, you should implement all the methods in this type. 
</member>
<member name="T:yiffurc.PixelB8G8R8X8" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\yiffurc\colors.hpp" line="132">
24-bit BGR (windows bitmap) pixel aligned to 32-bits.
* Traits/data type for a regular (RGB-order), 32-bit aligned 24-bit pixel.
* If you want to define your own pixel type for use with the other
* classes here, you should implement all the methods in this type. 
</member>
<member name="T:Furnarchy.Connection_Level" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="8">
<summary> Various connection states the client can be in. </summary>
</member>
<member name="T:Furnarchy.Hide_Flags" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="15">
<summary> Network hide flags. </summary>
<remarks>
  Network lines processed during the
  <see cref="!:NetSubsystem::InboundRawEvent"/> or <see cref="!:NetSubsystem::OutboundRawEvent"/> events
  may be hidden from subsequent modules or subsystems by setting these flags.
</remarks>
</member>
<member name="T:Furnarchy.ObjectSet" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="32">
<summary> Describes an object and its map position. </summary>
</member>
<member name="T:Furnarchy.FloorSet" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="40">
<summary> Describes a floor tile and its map position. </summary>
</member>
<member name="T:Furnarchy.DsVarSet" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="48">
<summary> Describes a range of DS variables to set to a value. </summary>
</member>
<member name="T:Furnarchy.DsTriggerLine" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="56">
<summary> Describes a triggered DS line and two parameters. </summary>
</member>
<member name="T:Furnarchy.InboundEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="67">
<summary> For use with <see cref="!:NetSubsystem::onInbound"/> </summary>.
</member>
<member name="T:Furnarchy.OutboundEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="75">
<summary> For use with <see cref="!:NetSubsystem::onOutbound"/> </summary>.
</member>
<member name="T:Furnarchy.InboundRawEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="84">
<summary> Arguments for <see cref="!:InboundRawDelegate"/>. </summary>
</member>
<member name="F:Furnarchy.InboundRawEventArgs.line" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="88">
<summary> The line in its raw form. </summary>
<remarks>
  Modifying this line will, in turn, modify the line for subsequent modules or subsystems.
</remarks>
</member>
<member name="F:Furnarchy.InboundRawEventArgs.line_id" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="93">
<summary> An increasing number that uniquely identifies this inbound line. </summary>
</member>
<member name="F:Furnarchy.InboundRawEventArgs.line_hide_flags" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="95">
<summary> Flags that may be set to hide the line from subsequent modules or subsystems. </summary>
<remarks>
  This is any combination of the <see cref="T:Furnarchy.Hide_Flags"/> enumeration.
</remarks>
</member>
<member name="T:Furnarchy.InboundAvatarMoveEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="101">
<summary> Arguments for <see cref="!:InboundAvatarMoveDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundAvatarCreateEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="110">
<summary> Arguments for <see cref="!:InboundAvatarCreateDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundAvatarDestroyEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="122">
<summary> Arguments for <see cref="!:InboundAvatarDestroyDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundAvatarHideEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="128">
<summary> Arguments for <see cref="!:InboundAvatarHideDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundAvatarShowEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="136">
<summary> Arguments for <see cref="!:InboundAvatarShowDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundAvatarShow2EventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="145">
<summary> Arguments for <see cref="!:InboundAvatarShow2Delegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundAvatarUpdateEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="154">
<summary> Arguments for <see cref="!:InboundAvatarUpdateDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundCameraMoveEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="162">
<summary> Arguments for <see cref="!:InboundCameraMoveDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundCameraSetEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="171">
<summary> Arguments for <see cref="!:InboundCameraSetDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundChatEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="178">
<summary> Arguments for <see cref="!:InboundChatDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundChatBadgeEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="186">
<summary> Arguments for <see cref="!:InboundChatBadgeDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundChatEchoEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="192">
<summary> Arguments for <see cref="!:InboundChatEchoDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundChatSpeciTagEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="198">
<summary> Arguments for <see cref="!:InboundChatSpeciTagDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDeclinedEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="204">
<summary> Arguments for <see cref="!:InboundDeclinedDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDeclineEchoEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="211">
<summary> Arguments for <see cref="!:InboundDeclineEchoDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDescBodyEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="218">
<summary> Arguments for <see cref="!:InboundDescBodyDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDescHeadEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="225">
<summary> Arguments for <see cref="!:InboundDescHeadDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDreamPadCreateEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="232">
<summary> Arguments for <see cref="!:InboundDreamPadCreateDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDreamPadDestroyEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="241">
<summary> Arguments for <see cref="!:InboundDreamPadDestroyDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDsEmitEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="248">
<summary> Arguments for <see cref="!:InboundDsEmitDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDsTriggerEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="254">
<summary> Arguments for <see cref="!:InboundDsTriggerDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDsTriggerSelfEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="264">
<summary> Arguments for <see cref="!:InboundDsTriggerSelfDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDsVarSetEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="274">
<summary> Arguments for <see cref="!:InboundDsVarSetDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundEmitEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="280">
<summary> Arguments for <see cref="!:InboundEmitDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundEmoteEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="286">
<summary> Arguments for <see cref="!:InboundEmoteDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundFeetObjectSetEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="294">
<summary> Arguments for <see cref="!:InboundFeetObjectSetDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundFetchDreamEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="300">
<summary> Arguments for <see cref="!:InboundFetchDreamDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundFetchPortraitEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="307">
<summary> Arguments for <see cref="!:InboundFetchPortraitDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundFloorSetEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="313">
<summary> Arguments for <see cref="!:InboundFloorSetDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundInventoryObjectSetEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="319">
<summary> Arguments for <see cref="!:InboundInventoryObjectSetDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundJoinedEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="325">
<summary> Arguments for <see cref="!:InboundJoinedDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundJoinRequestEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="332">
<summary> Arguments for <see cref="!:InboundJoinRequestDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundLoginFailEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="339">
<summary> Arguments for <see cref="!:InboundLoginFailDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundNameLookupFailEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="345">
<summary> Arguments for <see cref="!:InboundNameLookupFailDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundObjectSetEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="351">
<summary> Arguments for <see cref="!:InboundObjectSetDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundOnlineCheckEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="357">
<summary> Arguments for <see cref="!:InboundOnlineCheckDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundPlaySongEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="364">
<summary> Arguments for <see cref="!:InboundPlaySongDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundPlaySoundEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="370">
<summary> Arguments for <see cref="!:InboundPlaySoundDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundPortraitSetEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="376">
<summary> Arguments for <see cref="!:InboundPortraitSetDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundRawChatEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="383">
<summary> Arguments for <see cref="!:InboundRawChatDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundRegisterFailEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="389">
<summary> Arguments for <see cref="!:InboundRegisterFailDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundRequestEchoEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="395">
<summary> Arguments for <see cref="!:InboundRequestEchoDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundRollEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="402">
<summary> Arguments for <see cref="!:InboundRollDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundShoutEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="413">
<summary> Arguments for <see cref="!:InboundShoutDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundShoutEchoEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="421">
<summary> Arguments for <see cref="!:InboundShoutEchoDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundSummonedEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="427">
<summary> Arguments for <see cref="!:InboundSummonedDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundSummonRequestEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="434">
<summary> Arguments for <see cref="!:InboundSummonRequestDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundSyncColorCodeEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="442">
<summary> Arguments for <see cref="!:InboundSyncColorCodeDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundUseBgFileEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="448">
<summary> Arguments for <see cref="!:InboundUseBgFileDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundWhisperEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="454">
<summary> Arguments for <see cref="!:InboundWhisperDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundWhisperDeniedEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="462">
<summary> Arguments for <see cref="!:InboundWhisperDeniedDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundWhisperEchoEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="469">
<summary> Arguments for <see cref="!:InboundWhisperEchoDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundRawEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="478">
<summary> Arguments for <see cref="!:OutboundRawDelegate"/>. </summary>
</member>
<member name="F:Furnarchy.OutboundRawEventArgs.line" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="482">
<summary> The line in its raw form. </summary>
<remarks>
  Modifying this line will, in turn, modify the line for subsequent modules or subsystems.
</remarks>
</member>
<member name="F:Furnarchy.OutboundRawEventArgs.line_id" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="487">
<summary> An increasing number that uniquely identifies this outbound line. </summary>
</member>
<member name="F:Furnarchy.OutboundRawEventArgs.line_hide_flags" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="489">
<summary> Flags that may be set to hide the line from subsequent modules or subsystems. </summary>
<remarks>
  This is any combination of the <see cref="T:Furnarchy.Hide_Flags"/> enumeration.
</remarks>
</member>
<member name="T:Furnarchy.OutboundChangeColorEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="495">
<summary> Arguments for <see cref="!:OutboundChangeColorDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundChangeDescEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="501">
<summary> Arguments for <see cref="!:OutboundChangeDescDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundChatEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="507">
<summary> Arguments for <see cref="!:OutboundChatDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundEmoteEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="513">
<summary> Arguments for <see cref="!:OutboundEmoteDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundFollowDreamLinkEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="519">
<summary> Arguments for <see cref="!:OutboundFollowDreamLinkDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundGoMapEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="525">
<summary> Arguments for <see cref="!:OutboundGoMapDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundJoinEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="531">
<summary> Arguments for <see cref="!:OutboundJoinDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundLoginEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="537">
<summary> Arguments for <see cref="!:OutboundLoginDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundLookPosEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="545">
<summary> Arguments for <see cref="!:OutboundLookPosDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundMoveEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="551">
<summary> Arguments for <see cref="!:OutboundMoveDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundOnlineCheckEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="557">
<summary> Arguments for <see cref="!:OutboundOnlineCheckDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundSetColorEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="563">
<summary> Arguments for <see cref="!:OutboundSetColorDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundSetDescEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="569">
<summary> Arguments for <see cref="!:OutboundSetDescDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundShoutEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="575">
<summary> Arguments for <see cref="!:OutboundShoutDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundSummonEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="581">
<summary> Arguments for <see cref="!:OutboundSummonDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundWhisperEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="587">
<summary> Arguments for <see cref="!:OutboundWhisperDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.InboundAvatarMoveDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="597">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundAvatarCreateDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="599">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundAvatarDestroyDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="601">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundAvatarHideDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="603">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundAvatarShowDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="605">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundAvatarShow2Delegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="607">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundAvatarUpdateDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="609">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundCameraMoveDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="611">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundCameraSetDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="613">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundChatDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="615">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundChatBadgeDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="617">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundChatEchoDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="619">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundChatSpeciTagDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="621">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDeclinedDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="623">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDeclineEchoDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="625">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDeclineFailDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="627">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDescBodyDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="629">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDescHeadDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="631">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDragonRoarDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="633">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDreamPadCreateDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="635">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDreamPadDestroyDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="637">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDsEmitDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="639">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDsTriggerDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="641">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDsTriggerSelfDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="643">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundDsVarSetDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="645">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundEmitDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="647">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundEmoteDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="649">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundFeetObjectSetDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="651">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundFetchDreamDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="653">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundFetchPortraitDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="655">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundFloorSetDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="657">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundInventoryObjectSetDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="659">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundJoinedDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="661">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundJoinRequestDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="663">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundLoginFailDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="665">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundNameLookupAmbiguousDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="667">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundNameLookupFailDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="669">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundObjectSetDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="671">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundOnlineCheckDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="673">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundPlaySongDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="675">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundPlaySoundDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="677">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundPortraitSetDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="679">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundRawChatDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="681">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundRawDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="683">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundRegisterFailDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="685">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundRequestEchoDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="687">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundRollDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="689">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundShoutDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="691">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundShoutEchoDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="693">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundSuccessDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="695">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundSummonedDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="697">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundSummonRequestDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="699">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundSuppressRenderingDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="701">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundSyncColorCodeDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="703">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundUseBgFileDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="705">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundUnsuppressRenderingDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="707">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundWhisperDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="709">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundWhisperDeniedDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="711">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.InboundWhisperEchoDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="713">
<summary> A parsed inbound network event delegate. See <see cref="!:NetSubsystem::InboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundChangeColorDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="716">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundChangeDescDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="718">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundChatDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="720">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundDeclineDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="722">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundDreamReadyDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="724">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundEmoteDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="726">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundFollowDreamLinkDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="728">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundGoBackDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="730">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundGoMapDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="732">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundJoinDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="734">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundLayDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="736">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundLoginDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="738">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundLookPosDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="740">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundMoveDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="742">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundOnlineCheckDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="744">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundPickUpDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="746">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundPortraitChangeDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="748">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundQuitDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="750">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundRawDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="752">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundSetColorDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="754">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundSetDescDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="756">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundShoutDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="758">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundShoutToggleDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="760">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundSitDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="762">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundSitStandLayDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="764">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundStandDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="766">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundSummonDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="768">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundTurnLeftDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="770">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundTurnRightDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="772">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundUseItemDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="774">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.OutboundWhisperDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="776">
<summary> A parsed outbound network event delegate. See <see cref="!:NetSubsystem::OutboundRawEvent"/>. </summary>
</member>
<member name="T:Furnarchy.ConnectedDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="779">
<summary> A network event delegate. </summary>
<seealso cref="!:NetSubsystem::ConnectedEvent"/>
</member>
<member name="T:Furnarchy.DisconnectedDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="782">
<summary> A network event delegate. </summary>
<seealso cref="!:NetSubsystem::DisconnectedEvent"/>
</member>
<member name="T:Furnarchy.LoggedInDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="785">
<summary> A network event delegate. </summary>
<seealso cref="!:NetSubsystem::LoggedInEvent"/>
</member>
<member name="T:Furnarchy.NetSubsystem" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="791">
<summary> Encapsulates network operations and sDelegate. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeAvatarFrameNumber(Furnarchy.Avatar_Class,Furnarchy.Avatar_Pose,Furnarchy.Move_Direction)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="808">
<summary> Generate an avatar's frame number. </summary>
<param name="av"> The type of avatar (see <see cref="T:Furnarchy.Avatar_Class"/>). </param>
<param name="pose"> The avatar's pose (see <see cref="T:Furnarchy.Avatar_Pose"/>). </param>
<param name="direction"> The direction the avatar is facing. </param>
<remarks>
  Whenever an avatar is spawned, moved, or updated, the server
  sends a frame number (or two!) which maps to an FSH shape for that avatar.
</remarks>
<returns> The frame number. </returns>
</member>
<member name="M:Furnarchy.NetSubsystem.decodeAvatarFrameNumber(System.UInt32,Furnarchy.Avatar_Class@,Furnarchy.Avatar_Pose@,Furnarchy.Move_Direction@)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="820">
<summary> Decode an avatar's frame number. </summary>
<param name="frame"> The avatar frame number. </param>
<param name="av"> (out) The type of avatar (see <see cref="T:Furnarchy.Avatar_Class"/>). </param>
<param name="pose"> (out) The avatar's pose (see <see cref="T:Furnarchy.Avatar_Pose"/>). </param>
<param name="direction"> (out) The direction the avatar is facing. </param>
<remarks>
  Whenever an avatar is spawned, moved, or updated, the server
  sends a frame number (or two!) which maps to an FSH shape for that avatar.
</remarks>
<returns> true if the frame number was successfully decoded. </returns>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeBase220Number(System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="834">
<summary> Encode an integer into base-220. </summary>
<param name="n"> The integer. </param>
<param name="len"> How many digits wide the base-220 number should be. </param>
<remarks>
  Many of the newer sever-to-client messages now use base-220 to encode
  integers, rather than the old base-95.
</remarks>
<returns> The base-220 number as a byte array. </returns>
</member>
<member name="M:Furnarchy.NetSubsystem.decodeBase220Number(System.Byte[])" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="843">
<summary> Decode a base-220 number into an integer. </summary>
<param name="base220"> The base-220 number string. </param>
<remarks>
  Many of the newer sever-to-client messages now use base-220 to encode
  integers, rather than the old base-95.
</remarks>
<returns> The integer value of the base-220 number. </returns>
</member>
<member name="M:Furnarchy.NetSubsystem.decodeBase220Number(System.Byte[],System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="851">
<summary> Decode a base-220 number into an integer. </summary>
<param name="base220"> The base-220 number string. </param>
<param name="offset"> Zero-based index of the start of the base-220 number within the array. </param>
<param name="len"> Length of the base-220 number within the array. </param>
<remarks>
  Many of the newer sever-to-client messages now use base-220 to encode
  integers, rather than the old base-95.
</remarks>
<returns> The integer value of the base-220 number. </returns>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeBase95Number(System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="861">
<summary> Encode an integer into base-95. </summary>
<param name="n"> The integer. </param>
<param name="len"> How many digits wide the base-95 number should be. </param>
<remarks>
  Many of the older sever-to-client messages use base-95 to encode
  integers.
</remarks>
<returns> The base-95 number as a byte array. </returns>
</member>
<member name="M:Furnarchy.NetSubsystem.decodeBase95Number(System.Byte[])" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="870">
<summary> Decode a base-95 number into an integer. </summary>
<param name="base95"> The base-95 number string. </param>
<remarks>
  Many of the older sever-to-client messages use base-95 to encode
  integers.
</remarks>
<returns> The integer value of the base-95 number. </returns>
</member>
<member name="M:Furnarchy.NetSubsystem.decodeBase95Number(System.Byte[],System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="878">
<summary> Decode a base-95 number into an integer. </summary>
<param name="base95"> The base-95 number string. </param>
<param name="offset"> Zero-based index of the start of the base-220 number within the array. </param>
<param name="len"> Length of the base-220 number within the array. </param>
<remarks>
  Many of the older sever-to-client messages use base-95 to encode
  integers.
</remarks>
<returns> The integer value of the base-95 number. </returns>
</member>
<member name="M:Furnarchy.NetSubsystem.scan(System.String,System.Byte[])" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="888">
<summary> Match and extract information from a network string. </summary>
<param name="pattern"> String defining format and data types to extract. </param>
<param name="line"> A network line. </param>
<remarks>
  <para>
     This is similar to the scanf family of functions in C.
     <c>pattern</c> will contain a mix of literals and format specifiers.
     If <c>line</c> matches the format defined by <c>pattern</c>,
     an array of boxed Objects are returned containing all the scanned elements
     in order from left-to-right.
  </para>
  <para>
     Format specifiers have the structure <c>%[length]type</c>, where
     <c>length</c> is the number of symbols (or digits) of that type to extract
     and <c>type</c> is the type of information to extract.  The <c>length</c>
     specifier may be omitted in some cases, such as with byte strings and
     colors.
  </para>
  <list type="bullet">
     <listheader>
        <term>Available format specifiers.</term>
     </listheader>
     <item>
        <term><c>%c</c></term>
        <description>
           Any one (8-bit) character. May be augmented with a length 
           such as <c>%3c</c> to read in 3 any characters or <c>%*c</c> to read in 
           all characters that follow. Returns as a byte array. 
        </description>
     </item>
     <item>
        <term><c>%s</c></term>
        <description>
           A furcadia byte-string (A base-220 digit followed by a string). 
           Returns as a string.
        </description>
     </item>
     <item>
        <term><c>%k</c></term>
        <description>
           An old-style furcadia color-code (10-characters long, not including the 
           3 trailing GSD bytes). Returns as a byte array. 
        </description>
     </item>
     <item>
        <term><c>%K</c></term>
        <description>
           A new-style furcadia color-code (variable-length, not including the 3 
           trailing GSD bytes). 
           Returns as a byte array.
        </description>
     </item>
     <item>
        <term><c>%n</c></term>
        <description>
           A single-digit base-95 number. May be augmented with a length 
           such as <c>%2n</c> to read in a 2-digit base-95 number. Returns as an integer. 
        </description>
     </item>
     <item>
        <term><c>%N</c></term>
        <description>
           A single-digit base-220 number. May be augmented with a 
           length such as <c>%2N</c> to read in a 2-digit base-220 number. Returns as an integer. 
        </description>
     </item>
     <item>
        <term><c>%%</c></term>
        <description>
           A literal <c>%</c> character. Isn't captured 
           so it doesn't count towards the overall length of the returned array. 
        </description>
     </item>
  </list>
</remarks>
<example>Extracting name and color data from a portrait string (C#).
   <code>
     using System;
     
     public class MyModule : Furnarchy.IFurnarchyModule
     {
       private readonly Furnarchy.ModuleHost m_host;
     
       public MyModule( Furnarchy.ModuleHost host )
       {
           m_host = host;
           m_host.Net.InboundRawEvent += new Furnarchy.InboundRawDelegate( onInboundRaw );
       }
           
       void onInboundRaw( Object sender, Furnarchy.InboundRawEventArgs e )
       {
           // Match the characters "]f", then capture a color-code string, 
           // 3 more characters (the gender, species and digo bytes),
           // and all remaining characters (the name).
           object[] scanned = m_host.Net.scan( "]f%K%3c%*c", e.line );
           if (scanned != null)
           {
               // It was a portrait string!
               m_host.speak( String.Format( "Player {2}'s full color code is {0}{1}.",
                                            System.Text.Encoding.ASCII.GetString( (byte[]) scanned[0] ), 
                                            System.Text.Encoding.ASCII.GetString( (byte[]) scanned[1] ),
                                            System.Text.Encoding.ASCII.GetString( (byte[]) scanned[2] ) ) );
           }
        }
     }
   </code>
</example>
<returns> An Object array containing boxed versions of each element scanned. </returns>
</member>
<member name="M:Furnarchy.NetSubsystem.format(System.String,System.Object[])" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="997">
<summary> Formats a network string. </summary>
<param name="pattern"> String defining format and data types to encode. </param>
<param name="fields"> Data to be encoded (passed in the same order as defined by <c>pattern</c>). </param>
<remarks>
  <para>
     This is similar to the sprintf family of functions in C.
     <c>pattern</c> will contain a mix of literals and format specifiers.
     The returned string will interpolate <c>fields</c> over <c>pattern</c>
     using format specifiers to determine how to encode each field.
     The returned byte array is suitable for passing into <see cref="M:Furnarchy.NetSubsystem.inject(System.Byte[])"/> or <see cref="M:Furnarchy.NetSubsystem.send(System.Byte[])"/>.
  </para>
  <para>
     Format specifiers have the structure <c>%[length]type</c>, where
     <c>length</c> is the number of symbols (or digits) of that type to extract
     and <c>type</c> is the type of information to extract.  The <c>length</c>
     specifier may be omitted in some cases, such as when strings or byte arrays are
     expected.
     Note that many of the format specifiers are not completely analagous to
     those used by <see cref="M:Furnarchy.NetSubsystem.scan(System.String,System.Byte[])"/>.
  </para>
  <list type="bullet">
     <listheader>
        <term>Available format specifiers.</term>
     </listheader>
     <item>
        <term><c>%c</c></term>
        <description>
           One or more (8-bit) characters. Expects a byte array or string.
        </description>
     </item>
     <item>
        <term><c>%s</c></term>
        <description>
           A furcadia byte-string (A base-220 digit followed by a string). 
           Expects a string.
        </description>
     </item>
     <item>
        <term><c>%n</c></term>
        <description>
           A single-digit base-95 number. May be augmented with a length 
           such as <c>%2n</c> to read in a 2-digit base-95 number. Expects an integer. 
        </description>
     </item>
     <item>
        <term><c>%N</c></term>
        <description>
           A single-digit base-220 number. May be augmented with a 
           length such as <c>%2N</c> to read in a 2-digit base-220 number. Expects an integer. 
        </description>
     </item>
     <item>
        <term><c>%%</c></term>
        <description>
           A literal <c>%</c> character. Isn't captured 
           so it doesn't count towards the overall length of the returned array. 
        </description>
     </item>
  </list>
</remarks>
<example>Modifying a portrait string (C#).
   <code>
     using System;
     
     public class MyModule : Furnarchy.IFurnarchyModule
     {
       private readonly Furnarchy.ModuleHost m_host;
     
       public MyModule( Furnarchy.ModuleHost host )
       {
           m_host = host;
           m_host.Net.InboundRawEvent += new Furnarchy.InboundPortraitSetDelegate( onInboundPortraitSet );
       }
           
       void onInboundPortraitSet( Object sender, Furnarchy.InboundRawEventArgs e )
       {
           // Modify the line in-place so that all portrait names are set to "Zoidberg".
           e.line = m_host.Net.format( "]f%c%c", e.colors, "Zoidberg" );
        }
     }
   </code>
</example>
<returns> A byte array suitable for passing into <see cref="M:Furnarchy.NetSubsystem.inject(System.Byte[])"/> or <see cref="M:Furnarchy.NetSubsystem.send(System.Byte[])"/>. </returns>
</member>
<member name="M:Furnarchy.NetSubsystem.inject(System.Byte[])" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1081">
<summary> Inject an inbound line. </summary>
<remarks>
  The subsequent inbound line will not raise a network event in
  the calling module.
</remarks>
<returns> The line's id. </returns>
</member>
<member name="M:Furnarchy.NetSubsystem.send(System.Byte[])" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1088">
<summary> Send an outbound line. </summary>
<remarks>
  The subsequent outbound line will not raise a network event in
  the calling module.
</remarks>
<returns> The line's id. </returns>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundChangeColor(System.Byte[])" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1096">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundChangeDesc(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1098">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundChat(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1100">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundDecline" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1102">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundDreamReady" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1104">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundEmote(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1106">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundFollowDreamLink(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1108">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundGoBack" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1110">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundGoMap(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1112">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundJoin(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1114">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundLay" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1116">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundLogin(System.String,System.String,System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1118">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundLookPos(System.UInt32,System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1120">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundMove(Furnarchy.Move_Direction)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1122">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundOnlineCheck(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1124">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundPickUp" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1126">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundPortraitChange" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1128">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundQuit" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1130">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundSetColor(System.Byte[])" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1132">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundSetDesc(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1134">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundShout(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1136">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundShoutToggle" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1138">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundSit" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1140">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundSitStandLay" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1142">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundStand" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1144">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundSummon(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1146">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundTurnLeft" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1148">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundTurnRight" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1150">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundUseItem" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1152">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="M:Furnarchy.NetSubsystem.encodeOutboundWhisper(System.String,System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1154">
<summary> Encode an outbound (client-to-server) network line. </summary>
</member>
<member name="P:Furnarchy.NetSubsystem.ConnectionLevel" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1157">
<summary> The state of the game's primary connection. </summary>
<remarks>
  The connection level can be one of <see cref="T:Furnarchy.Connection_Level"/>.
</remarks>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundAvatarMoveEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1166">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundAvatarCreateEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1168">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundAvatarDestroyEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1170">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundAvatarHideEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1172">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundAvatarShowEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1174">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundAvatarShow2Event" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1176">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundAvatarUpdateEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1178">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundCameraMoveEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1180">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundCameraSetEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1182">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundChatEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1184">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundChatBadgeEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1186">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundChatEchoEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1188">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundChatSpeciTagEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1190">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundDeclinedEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1192">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundDeclineEchoEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1194">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundDeclineFailEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1196">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundDescBodyEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1198">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundDescHeadEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1200">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundDragonRoarEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1202">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundDreamPadCreateEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1204">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundDreamPadDestroyEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1206">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundDsEmitEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1208">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundDsTriggerEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1210">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundDsTriggerSelfEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1212">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundDsVarSetEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1214">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundEmitEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1216">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundEmoteEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1218">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundFeetObjectSetEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1220">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundFetchDreamEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1222">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundFetchPortraitEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1224">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundFloorSetEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1226">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundInventoryObjectSetEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1228">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundJoinedEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1230">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundJoinRequestEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1232">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundLoginFailEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1234">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundNameLookupAmbiguousEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1236">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundNameLookupFailEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1238">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundObjectSetEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1240">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundOnlineCheckEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1242">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundPlaySongEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1244">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundPlaySoundEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1246">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundPortraitSetEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1248">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundRawChatEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1250">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundRawEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1252">
<summary> A raw, unparsed inbound network event. </summary>
<remarks>
  <para>
     Before a line received from the server is given to the client,
     Furnarchy sends it to filters, scripts, then modules.
     When the module host receives the line, it triggers an <c>InboundRawEvent</c> event
     then a more specific event, if one exists.
  </para>
  <para>
     For example, if the client receives the line <c>"@!4 C"</c>, an <c>InboundRawEvent</c>
     event is raised first, then (assuming the <c>line</c> field of the event object wasn't modified)
     an <see cref="E:Furnarchy.NetSubsystem.InboundCameraSetEvent"/> event.
  </para>
  <para>
     At any time during the handling of these events, you may cancel/hide the line
     by setting the <c>line_hide_flags</c> field of the passed <see cref="T:Furnarchy.InboundRawEventArgs"/> structure
     and you may edit it (in its raw form) by assigning a new String to the <c>line</c> field.
  </para>
</remarks>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundRegisterFailEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1272">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundRequestEchoEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1274">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundRollEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1276">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundShoutEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1278">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundShoutEchoEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1280">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundSuccessEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1282">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundSummonedEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1284">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundSummonRequestEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1286">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundSuppressRenderingEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1288">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundSyncColorCodeEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1290">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundUseBgFileEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1292">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundUnsuppressRenderingEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1294">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundWhisperEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1296">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundWhisperDeniedEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1298">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.InboundWhisperEchoEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1300">
<summary> A parsed inbound network event. See <see cref="E:Furnarchy.NetSubsystem.InboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundChangeColorEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1303">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundChangeDescEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1305">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundChatEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1307">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundDeclineEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1309">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundDreamReadyEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1311">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundEmoteEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1313">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundFollowDreamLinkEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1315">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundGoBackEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1317">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundGoMapEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1319">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundJoinEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1321">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundLayEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1323">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundLoginEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1325">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundLookPosEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1327">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundMoveEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1329">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundOnlineCheckEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1331">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundPickUpEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1333">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundPortraitChangeEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1335">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundQuitEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1337">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundRawEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1339">
<summary> A raw, unparsed outbound network event. </summary>
<remarks>
  <para>
     Before a line sent from the client to the server,
     Furnarchy sends it to filters, scripts, then modules.
     When the module host receives the line, it triggers an <c>OutboundRawEvent</c> event
     then a more specific event, if one exists.
  </para>
  <para>
     For example, if the client receives the line <c>"m 3"</c>, an <c>OutboundRawEvent</c>
     event is raised first, then (assuming the <c>line</c> field of the event object wasn't modified) 
     an <see cref="E:Furnarchy.NetSubsystem.OutboundMoveEvent"/> event.
  </para>
  <para>
     At any time during the handling of these events, you may cancel/hide the line
     by setting the <c>line_hide_flags</c> field of the passed <see cref="T:Furnarchy.InboundRawEventArgs"/> structure
     and you may edit it (in its raw form) by assigning a new String to the <c>line</c> field.
  </para>
</remarks>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundSetColorEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1359">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundSetDescEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1361">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundShoutEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1363">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundShoutToggleEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1365">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundSitEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1367">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundSitStandLayEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1369">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundStandEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1371">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundSummonEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1373">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundTurnLeftEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1375">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundTurnRightEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1377">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundUseItemEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1379">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.OutboundWhisperEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1381">
<summary> A parsed outbound network event. See <see cref="E:Furnarchy.NetSubsystem.OutboundRawEvent"/>. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.ConnectedEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1384">
<summary> The primary game server connection has been established. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.LoggedInEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1386">
<summary> The player has logged in. </summary>
</member>
<member name="E:Furnarchy.NetSubsystem.DisconnectedEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\net.h" line="1388">
<summary> The primary game server connection was lost. </summary>
</member>
<member name="T:Furnarchy.Script_Flags" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\scripts.h" line="8">
<summary> Script run flags. </summary>
</member>
<member name="T:Furnarchy.Script_Error" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\scripts.h" line="17">
<summary> Script run errors. </summary>
</member>
<member name="T:Furnarchy.ScriptInstance" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\scripts.h" line="26">
<summary> Information on a script instance. </summary>
</member>
<member name="T:Furnarchy.ScriptRanEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\scripts.h" line="35">
<summary> Arguments for <see cref="!:ScriptRanDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.ScriptKilledEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\scripts.h" line="41">
<summary> Arguments for <see cref="!:ScriptKilledDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.ScriptRanDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\scripts.h" line="50">
<summary> A script event delegate. </summary>
<seealso cref="!:ScriptsSubsystem::ScriptRanEvent"/>
</member>
<member name="T:Furnarchy.ScriptKilledDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\scripts.h" line="53">
<summary> A script event delegate. </summary>
<seealso cref="!:ScriptsSubsystem::ScriptKilledEvent"/>
</member>
<member name="T:Furnarchy.ScriptsSubsystem" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\scripts.h" line="59">
<summary> Exposes control over the scripting subsystem. </summary>
</member>
<member name="M:Furnarchy.ScriptsSubsystem.getRunningScripts" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\scripts.h" line="72">
<summary> Retrieve an array of all running scripts. </summary>
<remarks> 
 To maintain an up-to-date list of running scripts, one should
 make a call to <c>getRunningScripts</c> whenever running scripts have been updated.
</remarks>
<returns>
 An array of <see cref="T:Furnarchy.ScriptInstance"/> structures with information on each running script.
</returns>
<seealso cref="E:Furnarchy.ScriptsSubsystem.ScriptRanEvent"/>
<seealso cref="E:Furnarchy.ScriptsSubsystem.ScriptKilledEvent"/>
</member>
<member name="M:Furnarchy.ScriptsSubsystem.run(Furnarchy.ScriptInstance@)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\scripts.h" line="83">
<summary> Run a script. </summary>
<param name="instance"> A <see cref="T:Furnarchy.ScriptInstance"/> structure that contains the script arguments to run. </param>
<remarks>
  If successful, the <c>id</c> member of <c>instance</c> will be filled with the ID of the running script.
</remarks>
<returns> A <see cref="T:Furnarchy.Script_Error"/> enumeration indicating success or failure. </returns>
</member>
<member name="M:Furnarchy.ScriptsSubsystem.kill(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\scripts.h" line="90">
<summary> Kill a running script. </summary>
<param name="id"> The ID of the running script. </param>
<returns> A <see cref="T:Furnarchy.Script_Error"/> enumeration indicating success or failure. </returns>
</member>
<member name="E:Furnarchy.ScriptsSubsystem.ScriptRanEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\scripts.h" line="95">
<summary> A script has been run. </summary>
</member>
<member name="E:Furnarchy.ScriptsSubsystem.ScriptKilledEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\scripts.h" line="97">
<summary> A script has been killed. </summary>
</member>
<member name="T:Furnarchy.PlayerInfo" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="6">
<summary> Information on the current player. </summary>
</member>
<member name="T:Furnarchy.MonsterInfo" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="19">
<summary> Information on a monster (avatar). </summary>
</member>
<member name="T:Furnarchy.MonsterCreatedEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="33">
<summary> Arguments for <see cref="!:MonsterCreatedDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.MonsterDestroyedEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="39">
<summary> Arguments for <see cref="!:MonsterDestroyedDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.MonsterUpdatedEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="45">
<summary> Arguments for <see cref="!:MonsterUpdatedDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.CurrentPlayerUpdatedEventArgs" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="51">
<summary> Arguments for <see cref="!:CurrentPlayerUpdatedDelegate"/>. </summary>
</member>
<member name="T:Furnarchy.MonsterCreatedDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="60">
<summary> A monster event delegate. </summary>
<seealso cref="!:WorldSubsystem::MonsterCreatedEvent"/>
</member>
<member name="T:Furnarchy.MonsterDestroyedDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="63">
<summary> A monster event delegate. </summary>
<seealso cref="!:WorldSubsystem::MonsterDestroyedEvent"/>
</member>
<member name="T:Furnarchy.MonsterUpdatedDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="66">
<summary> A monster event delegate. </summary>
<seealso cref="!:WorldSubsystem::MonsterUpdatedEvent"/>
</member>
<member name="T:Furnarchy.CurrentPlayerUpdatedDelegate" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="69">
<summary> A world event delegate. </summary>
<seealso cref="!:WorldSubsystem::CurrentPlayerUpdatedEvent"/>
</member>
<member name="T:Furnarchy.DescTag" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="75">
<summary> An instance of a desctag. </summary>
<seealso cref="!:WorldSubsystem::createDescTag"/>
</member>
<member name="P:Furnarchy.DescTag.body" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="83">
<summary> The body of the desctag. </summary>
<remarks>
  Writing to this property will automatically
  update to the player's description with the server.
</remarks>
</member>
<member name="T:Furnarchy.WorldSubsystem" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="106">
<summary> Encapsulates knowledge of and interactions with the game world. </summary>
</member>
<member name="M:Furnarchy.WorldSubsystem.createShortName(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="121">
<summary> Create a shortname. </summary>
<param name="name"> A longname </param>
<remarks>
  To create a shortname, all spaces and non-ascii characters are stripped.
  Some latin-1 characters that resemble regular ASCII characters are converted to their
  ASCII counterparts (e.g., <c>ë</c> to <c>e</c>).
</remarks>
<returns> The shortname equivalent of <c>name</c>. </returns>
</member>
<member name="M:Furnarchy.WorldSubsystem.createDescTag" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="130">
<summary> Create a desctag. </summary>
<remarks>
  Desctags are logical suffixes to the player's description.
  They help keep modules and scripts that wish to append text to
  the player's description from stepping on each other's toes.
</remarks>
<returns> A <see cref="T:Furnarchy.DescTag"/> instance. </returns>
<seealso cref="T:Furnarchy.DescTag"/>
</member>
<member name="M:Furnarchy.WorldSubsystem.getMonsterInfo(System.UInt32)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="139">
<summary> Retrieve information on a monster by its GUID. </summary>
<param name="guid"> 32-bit GUID of the monster. </param>
<remarks> 
 The <c>guid</c> member of the returned <see cref="T:Furnarchy.MonsterInfo"/>
 will be zero if the monster could not be found.
</remarks>
<returns>
A <see cref="T:Furnarchy.MonsterInfo"/> structure with information on the monster.
</returns>
</member>
<member name="M:Furnarchy.WorldSubsystem.getMonsterInfo(System.String)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="149">
<summary> Retrieve information on a monster by its name. </summary>
<param name="name"> Name of the monster (this will be converted to a shortname automatically). </param>
<remarks> 
 The <c>guid</c> member of the returned <see cref="T:Furnarchy.MonsterInfo"/>
 will be zero if the monster could not be found.
</remarks>
<returns>
 A <see cref="T:Furnarchy.MonsterInfo"/> structure with information on the monster.
</returns>
</member>
<member name="M:Furnarchy.WorldSubsystem.getMonsters" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="159">
<summary> Retrieve an array of all existing monsters. </summary>
<remarks> 
 To maintain an up-to-date list of existing monsters, one should
 make a call to <c>getMonsters</c> whenever monsters have been updated.
</remarks>
<returns>
 An array of <see cref="T:Furnarchy.MonsterInfo"/> structures with information on each existing monster.
</returns>
<seealso cref="E:Furnarchy.WorldSubsystem.MonsterCreatedEvent"/>
<seealso cref="E:Furnarchy.WorldSubsystem.MonsterDestroyedEvent"/>
<seealso cref="E:Furnarchy.WorldSubsystem.MonsterUpdatedEvent"/>
</member>
<member name="P:Furnarchy.WorldSubsystem.CurrentPlayer" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="172">
<summary> Information on the current player. </summary>
<remarks>
 <para>
    You may assign a new <see cref="T:Furnarchy.PlayerInfo"/> value to
    update Furnarchy and the client if you've made logical
    changes to their values (by, say for example, swapping the connection).
 </para>
 <para>
    The client update relies entirely on memlocs.
 </para>
</remarks>
</member>
<member name="E:Furnarchy.WorldSubsystem.MonsterCreatedEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="213">
<summary> A monster was created. </summary>
</member>
<member name="E:Furnarchy.WorldSubsystem.MonsterDestroyedEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="215">
<summary> A monster was destroyed. </summary>
</member>
<member name="E:Furnarchy.WorldSubsystem.MonsterUpdatedEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="217">
<summary> A monster was updated. </summary>
</member>
<member name="E:Furnarchy.WorldSubsystem.CurrentPlayerUpdatedEvent" decl="false" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\world.h" line="219">
<summary> The player's <see cref="T:Furnarchy.PlayerInfo"/> has changed. </summary>
</member>
<member name="M:shortname.make_shortname(std.basic_string&lt;System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte,std.char_traits{System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte},std.allocator&lt;System.SByte!System.Runtime.CompilerServices.IsSignUnspecifiedByte&gt;&gt;!System.Runtime.CompilerServices.IsConst*!System.Runtime.CompilerServices.IsImplicitlyDereferenced)" decl="true" source="e:\svn\furnarchy2\trunk\furnarchy2\adaptercli\shortname.h" line="34">
Convert a name to a shortname.
   * The name may contain xml entity references. 
</member>
</members>
</doc>