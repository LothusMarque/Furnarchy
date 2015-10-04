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
</members>
</doc>