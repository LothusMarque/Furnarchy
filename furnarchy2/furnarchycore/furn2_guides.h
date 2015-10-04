/** \addtogroup guides Guides */
/**\{*/

/** \defgroup quickstart Writing C++ Modules - Quickstart Examples
*  Learn to write modules like a pro!
*
*  Writing Furnarchy 2 modules is a structurally much simpler departure 
*  from Furnarchy 3.
*  The Furnarchy API has been greatly condensed and simplified,
*  and the scaffolding required for well-formed modules is now almost nil.
*
*  \section qs_project_setup Project Setup
*  Furnarchy native modules are DLLs, so you should start by creating a 
*  Win32 DLL project.
*
*  To make use of the Furnarchy API, you need to include the \c furn2.h header
*  and link against the \c furn2.lib import library.  Both these files come
*  bundled with every Furnarchy 2 installation.
*  
*  For C++ projects, the furnarchy 2 API will be contained in the \c furn2 
*  namespace.
*
*  \section qs_event_handlers Event Handlers
*  Furnarchy modules are entirely event-based.  Thus, all you do will ultimately 
*  be a reaction to an event coming through one of the event handlers:
*     - \ref on_module
*     - \ref on_client
*     - \ref on_wndproc
*     - \ref on_world
*     - \ref on_command
*     - \ref on_inet
*     - \ref on_onet
*
*  To use event handlers, you just need to define one. Furnarchy will
*  automatically discover it when it loads your module and call it
*  whenever an appropriate event comes up.
*
*  \section qs_example1 Example: "Hello, World!"
*
*  As a simple example, here's a module that prints "Hello, World!" to the
*  chat buffer when the player logs in:
*  \include mod1.cpp
*
*  \section qs_example2 Example: "Hello, World!" (Part Deux)
*
*  To illustrate sending network data and command line processing, 
*  here's a module that makes the player say "Hello, World!" to everyone 
*  nearby when the player types "@sayhello":
*  \include mod2.cpp
*
*  \section qs_example3 Example: "Hello, World!" (Part Trois)
*
*  To illustrate handling windows messages, here's a module that
*  makes the player say "Hello, World!" to everyone nearby when the
*  player presses CTRL+S:
*  \include mod3.cpp
*
*  \section qs_example4 Example: Chat Alert
*  
*  To illustrate processing inbound network data, here's a module that
*  flashes the Furcadia window whenever something is about to be posted
*  to the chat buffer:
*  \include mod4.cpp
*
*  \section qs_example5 Example: Reverse Speech
*
*  To illustrate manipulating outbound network data, here's a module that
*  reverses normal chat coming from the player:
*  \include mod5.cpp
*
*  \section qs_more Where To Go From Here
*
*  To get the most out of Furnarchy, you should read more on
*  the \ref event_handlers "event handlers", browse the \ref api 
*  "API Functions" and look at the other \ref guides "guides".
*  The forums at http://www.heroinpuppy.com/forums are also an excellent
*  place to go for help with module making.
*/

/** \defgroup module_ini The module.ini File
*  You may notice some modules have a module.ini file in their
*  directories.  This file is used to give Furnarchy extra
*  information about your module. \n
*  It's made up of a series of 'Name=Value' fields.  \n
*
*  Furnarchy currently looks at the following fields:
*  
*  - \c name \n
*     Specifies the module's "fancy name."  The fancy name is the name
*     skins will use for your module in its graphical elements.  
*     The default value is your module's directory name.
*  - \c adapter \n
*     The adapter a module uses.  Adapters are a kind of
*     runtime environment for modules, to allow Furnarchy to potentially
*     load modules of varying platforms (.NET assemblies, python scripts, 
*     etc.).  For C++ modules, this should be set to 'native'.  The
*     default value is 'native'.
*  - \c thumb \n
*     Specifies a module icon for the skin to use.  You should specify
*     the relative path to a 32x32 image.  The type can be any common
*     image type (PNG, TGA, BMP, JPG, GIF, etc.) 
*     The default value is 'thumbnail.bmp'.
*  - \c uuid \n
*     UUIDs are the preferred lookup method when a module wants to
*     communicate with another module. UUIDs are specified in Microsoft
*     GUID registry form (e.g., {3F945C7E-270F-4eb8-AF15-D2ABC72AECC0}).
*     The default value is a zero UUID.
*/

/** \defgroup net_handling Handling Network Events
*  \brief How network data is sent to modules.
*  \n\n
*  Commands sent between the client and server on the main game connection
*  come in the form of (somewhat) human-readable lines.
*  For example, a chat command from client to server, in C-string notation,
*  would be <tt>"\"blah blah blah\\n"</tt> and <tt>"m 9\n"</tt> to move northeast once.
*
*  (There are various resources on the web that catalogue Furcadia commands.
*  For example, http://icerealm.quickfox.org/data/FTR/?)
*
*  When furnarchy intercepts a line from the client or server, it's sent
*  to each modules' respective handler (on_onet() or on_inet(), depending
*  on the direction).  It's passed as a simple, null-terminated string
*  with the trailing linefeed character discarded.
*
*  \section net_handling_passes Passes
*  A line is sent to all the modules in 3 passes, giving you three 
*  opportunities to process the line.  Each pass has different restrictions
*  on what can be done with the line, which will likely dictate which
*  pass of a particular message you choose to handle.  The value of the
*  \a rev parameter of the on_inet() and on_onet() handlers indicates
*  the current pass.
*
*  The passes occur in the following order:
*
*  \par The \c REV_DRAFT Pass
*  The line is first sent to all modules during the \c REV_DRAFT pass.
*  The line cannot be modified during this pass, but it can be cancelled
*  via f2_hideline().
*  This is an opportunity to see a line before modules have modified it.
*
*  \par The \c REV_REVISING Pass
*  The line is sent a second time to all modules for the \c REV_REVISING pass.
*  The line may be modified via f2_reviseline() or cancelled via f2_hideline().
*
*  \par The \c REV_FINAL Pass
*  The line is sent one final time to all modules for the \c REV_FINAL pass.
*  The line cannot be modified during this pass, but it can be cancelled
*  via f2_hideline().
*  This is an opportunity to see the line after modules have modified it.
*  For client->server lines, this what will actually be sent to the server,
*  and for server->client lines, this is what will actually be received
*  by the client.
*
*  \section net_handling_parsing Parsing Network Lines
*  Unlike Furnarchy 3, Furnarchy 2 doesn't parse network lines for you.
*  This decision was made to un-clutter the API, as well as give you
*  more freedom to mangle entire lines as you see fit.
*  Parsing commands isn't always trivial, though, and to help you out,
*  Furnarchy has an unofficial companion library, YIFFurc 
*  (http://www.heroinpuppy.com/index.php?cat=yiffurc&page=yiffurc).  
*  YIFFurc is capable of decoding and encoding Furcadia network 
*  traffic with little hassle, among other things.
*/

/**\}*/
   
/** \defgroup event_handlers Event Handlers
*  To receive event notifications from Furnarchy, simply define
*  any of the following event handlers anywhere in your project.
*/
/**\{*/

/** The module events handler.
*  Various events that relate to the module come through here.
*  The type and meaning of \a param depends on the value of \a e.
*  \param[in]        e     The module event.
*  \param[in,out]  param   Event-specific parameter.
*  \return Depends on the message.
*  \sa mod_events */
F2EXPORT intptr_t F2CALL on_module( Module_Event e, intptr_t param );

/** The client event handler.
*  Various events that relate to the client come through here.  
*  The type and meaning of \a param depends on the value of \a e.
*  \param[in]        e     The client event.
*  \param[in,out]  param   Event-specific parameter.
*  \return Depends on the message.
*  \sa cl_events */
F2EXPORT intptr_t F2CALL on_client( Client_Event e, intptr_t param );

/** The windows message events handler.
*  About Everything that goes through the client's WindowProcedure 
*  goes through here.
*  \param[in]        params*    Message parameters.
*  \return Return \c true to indicate you've handled the message
*          and to use the return value in \c params->retval.
*          Otherwise, the message will continue on to the other
*          modules in the pipeline (and eventually to the client).
*  \sa furn2::WNDPROCPARAMS */
F2EXPORT bool F2CALL on_wndproc( WNDPROCPARAMS* params );

/** The world events handler.
*  World event messages come through here.  The type and meaning of 
*  \a param depends on the value of \a e.
*  \param[in]        e     Message parameters.
*  \param[in,out]  param   Event-specific parameter.
*  \return Depends on the message.
*  \sa wo_events */
F2EXPORT intptr_t F2CALL on_world( World_Event e, intptr_t param );

/** The Furnarchy command events handler.
*  These are the '@' prefixed commands, but without the '@' prefix.
*  \param[in]    argc    Number of strings in \a argv.
*  \param[in]    argv    Array of command line parameters.
*  \return  Return \c true to tell Furnarchy you've handled the message.
*           Return \c false otherwise.
*  \remarks Commands are sent to all modules/scripts/etc.
*           The command line parameters are tokenized by whitespace,
*           unless enclosed in backticks, apostrophes, or double quotes.
*  \sa f2_command() */
F2EXPORT bool F2CALL on_command( int argc, char* argv[] );

/** The Server-to-Client network line handler.
*  Allows for inspecting or manipulating of inbound network data.
*  \param[in]    pass   The revision pass.
*  \param[in]    line   Null-terminated server-to-client line.
*  \param[in]    from   Where the line originated from.  \a from will be set to
*                       \c FROM_SERVER if the line originated from the server (i.e.,
*                       wasn't genereted by an f2_netinline() or equivalent).
*  \param[in]    num    The line number. Every network line that passes through
*                       furnarchy gets tagged with an identifying number.
*  \remarks Network lines are sent to modules in 3 passes.  For more
*           information, check the section on 
*           \ref net_handling "Handling Network Events". 
*  \sa net_handling f2_netin() f2_netinline() */
F2EXPORT void F2CALL on_inet( Revision pass, const char* line, int from, size_t num );

/** The Client-to-Server network line handler.
*  Allows for inspecting or manipulating of outbound network data.
*  \param[in]    pass   The revision pass.
*  \param[in]    line   Null-terminated client-to-server line.
*  \param[in]    from   Where the line originated from.  \a from will be set to
*                       \c FROM_CLIENT if the line originated from the client (i.e.,
*                       wasn't genereted by an f2_netoutline() or equivalent).
*  \param[in]    num    The line number. Every network line that passes through
*                       furnarchy gets tagged with an identifying number.
*  \remarks Network lines are sent to modules in 3 passes.  For more
*           information, check the section on 
*           \ref net_handling "Handling Network Events". 
*  \sa net_handling f2_netout() f2_netoutline() */
F2EXPORT void F2CALL on_onet( Revision pass, const char* line, int from, size_t num );

/**\}*/
/**\}*/

/** \mainpage Furnarchy Module Development Documentation
*  For categorized documentation, see the modules page. */
