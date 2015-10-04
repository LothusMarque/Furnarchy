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

#ifndef _YIFFURC_NET_H__
#define _YIFFURC_NET_H__

#include "common.h"

#if defined( __cplusplus )
namespace yiffurc {
#endif

/** \defgroup net Network
* \{ */

/** Network Connection Levels.
* \sa NET_SESSION */
enum Net_Level
{
   NET_LEVEL_CONNECTED = 0,
   NET_LEVEL_MOTDEND   = 1,
   NET_LEVEL_LOGGEDIN  = 64,
};

/** Transfer (fileserver) network connection levels.
* \sa XFER_NET_SESSION */
enum Xfer_Net_Level
{
   XFER_NET_LEVEL_CONNECTED = 0x00,
   XFER_NET_LEVEL_LOGGEDIN = 0x01,
   XFER_NET_LEVEL_CSEND = 0x03,
   XFER_NET_LEVEL_SSEND = 0x05,
};

/** Network session object.
* Maintains state information used in network parsing.
* \sa c2s_parse s2c_parse */
struct NET_SESSION
{
   /** Network Level.
   *  \sa Net_Level
   */
   enum Net_Level level;

   /** Number of outstanding character create requests from the client.
   *  Only valid when level ==  NET_LEVEL_MOTEND */
   unsigned int create_count;

   /** State information for server -> client parsing. */
   struct
   {
      /** Number of lines seen. */
      unsigned int line_count;
      /** Reserved. */
      unsigned int _r0;
      /** Reserved. */
      unsigned int _r1;
   }         s2c_state;

   /** State information for client -> server parsing. */
   struct
   {
      /** Number of lines seen. */
      unsigned int line_count;
      /** Reserved. */
      unsigned int _r0;
      /** Reserved. */
      unsigned int _r1;
   }         c2s_state;
};

/** \defgroup s2c Server-to-Client Commands (and Regular Expressions)
*  \par How to read regular expressions contained in this document.
*     Commands are shown as Perl-compatible regular expressions,
*     with a few added named classes designed for clarity:
*        - \b [:95int1:] A one-digit base-95 number: <tt>[\\x20-\\x7E]</tt>
*        - \b [:95int2:] A two-digit base-95 number: <tt>[\\x20-\\x7E]{2}</tt>
*        - \b [:95int3:] A three-digit base-95 number: <tt>[\\x20-\\x7E]{3}</tt>
*        - \b [:name:]  A username: <tt>[\\x21-\\x3B\=\\x3F-\\x7E]+</tt>
*        - \b [:220int1:] A one-digit base-220 number: <tt>[\\x23-\\xFF]</tt>
*        - \b [:220int2:] A two-digit base-220 number: <tt>[\\x23-\\xFF]{2}</tt>
*        - \b [:220int3:] A three-digit base-220 number: <tt>[\\x23-\\xFF]{3}</tt>
*        - \b [:220int4:] A four-digit base-220 number: <tt>[\\x23-\\xFF]{4}</tt>
*        - \b [:bstr:] A string whose length is specified by a leading base-220 digit: <tt>[:220int1:].*</tt>
*        - \b [:colors:] Color code: <tt>(t|u[:220int1:]{2}|v)[:220int1:]{10,30}</tt>
*        - \b [:gsd:] 3 base-220 digits indicating gender, species, and digo: <tt>[:220int1:]{3}</tt>
*
*     \note All regexes are implicitly sandwiched between ^ and $ anchors.
*  \{ */

/** An unknown server command.
*  wtf am I looking at?
* \sa S2C */
const int S2C_TYPE_UNKNOWN        = 0;

/** A message of the day line.
*  \par regex:
*  \code (.*) \endcode
*  \param $1 A single line of MOTD text.
*
*  Certain text cannot be displayed on a MOTD
*  line on their own, as they have other
*  meanings.
*
*  \sa S2C_MOTD */
const int S2C_TYPE_MOTD          = 1;

/** Instructs the client to play welcome.wav.
*  \par regex:
*  \code Dragonroar \endcode
*  This also signals the end of the MOTD.
*  \note In some scenarios, this may never be sent, or sent too late (after already logging in the player).
*  \sa S2C_DRAGONROAR */
const int S2C_TYPE_DRAGONROAR    = 2;

/** The protocol version.
*  \par regex:
*  \code V0013 \endcode
*
*  I think this has always been 'V0013' since
*  as far as I can remember.
*
*  \note Now extinct. :)
*
*  \sa S2C_VER */
const int S2C_TYPE_VER           = 3;

/** Signals the end of MOTD text.
*  \par regex:
*  \code END \endcode
*  \note Now extinct. :)  S2C_TYPE_DRAGONROAR now signals the end of the MOTD.
*  \sa S2C_MOTDEND */
const int S2C_TYPE_MOTDEND       = 4;

/** Response machine ID string.
*  \par regex:
*  \code \\PW(.+) \endcode
*  \param $1   Scrambled machine ID response.
*
*  This machine ID string is identical to the string
*  sent by the client, except the S= (checksum)
*  component will be re-seeded to produce a
*  different value the client expects.
*
*  \sa S2C_MACID, auth *
*/
const int S2C_TYPE_MACID         = 5;

/** Generic "succeeded," response.
*  \par regex:
*  \code &[&]+ \endcode
*
*  This is the server's response to both successful
*  alt creation and a successful login attempt.
*  The meaning of this message depends on the network level.
*
*  \sa S2C_PASS */
const int S2C_TYPE_PASS          = 6;

/** Alt creation failed.
*  \par regex:
*  \code N\{([^}]*)}(.*) \endcode
*  \param $1 Error category; either "NAME", "PASS", or "EMAIL".
*  \param $2 Human-readable reason.
*  \sa S2C_CREATEFAIL */
const int S2C_TYPE_CREATEFAIL    = 7;

/** Login attempt failed. (Deprecated)
*  \par regex:
*  \code Invalid name or password\. \endcode
*
*  This response is given even if the name and
*  password are correct, but there are
*  other reasons why the server won't let you play (banned).
*
*  \note No longer sent by server. Now you receive a
*        S2C_TYPE_MSGBOX message and just get disconnected.
*
*  \sa S2C_LOGINFAIL, S2C_TYPE_MSGBOX */
const int S2C_TYPE_LOGINFAIL     = 8;

/** Center the camera on a position.
*  \par regex:
*  \code @([:95int2:])([:95int2:]) \endcode
*  \param $1   x-coordinate.
*  \param $2   y-coordinate.
*  \sa S2C_CAMSET */
const int S2C_TYPE_CAMSET        = 9;

/** Tween the camera between two positions.
*  \par regex:
*  \code @([:95int2:])([:95int2:])([:95int2:])([:95int2:]) \endcode
*  \param $1   x-coordinate (dest).
*  \param $2   y-coordinate (dest).
*  \param $3   x-coordinate (src).
*  \param $4   y-coordinate (src).
*  \sa S2C_CAMTWEEN */
const int S2C_TYPE_CAMTWEEN      = 10;

/** Sets objects at positions.
*  \par regex:
*  \code >(?:([:220int2:])([:220int2:])([:220int2:]))+ \endcode
*  \param $1   x-coordinate.
*  \param $2   y-coordinate.
*  \param $3   object ID.
*
*  Normally the server sends individual commands
*  per object, but when a user first enters a map,
*  it may glob them all together into a big objset
*  command, where each number triplet pertains to
*  a different object.
*
*  \sa S2C_OBJSET */
const int S2C_TYPE_OBJSET        = 11;

/** Creates a user avatar at a position.
*  \par regex:
*  \code <([:220int4:])([:220int2:])([:220int2:])([:220int2:])([:bstr:])([:colors:])([:220int1:]) \endcode
*  \param $1   User ID.
*  \param $2   x-coordinate.
*  \param $3   y-coordinate.
*  \param $4   Avatar frame.
*  \param $5   Character name.
*  \param $6   Colors.
*  \param %7   Flags.
*  \sa S2C_AVCREATE, frame2av, AVFRAME */
const int S2C_TYPE_AVCREATE         = 12;

/** Animate an avatar between two positions.
*  \par regex:
*  \code /([:220int4:])([:220int2:])([:220int2:])([:220int2:]) \endcode
*  \param $1   User ID.
*  \param $2   x-coordinate.
*  \param $3   y-coordinate.
*  \param $4   End frame.
*  \sa S2C_AVTWEEN, frame2av, AVFRAME */
const int S2C_TYPE_AVTWEEN       = 13;

/** Destroy an avatar.
*  \par regex:
*  \code \)([:220int4:]) \endcode
*  \param $1   User ID.
*  \sa S2C_AVDESTROY */
const int S2C_TYPE_AVDESTROY       = 14;

/** Create dreampad text (deprecated).
*  \par regex:
*  \code ]s([:95int2:])([:95int2:])([:name:]) \endcode
*  \param $1   x-coordinate.
*  \param $2   y-coordinate.
*  \param $3   Dream name.
*
*  Since the introduction of dream captions,
*  this command has been superceded by S2C_TYPE_DREAMPOS2.\n
*
*  \remarks Is this command still in use?  Should I just leave it here for
*           posterity anyway?
*  \sa S2C_DREAMPOS, S2C_TYPE_DREAMPOS2 */
const int S2C_TYPE_DREAMPOS      = 15;

/** Create dreampad text with caption.
*  \par regex:
*  \code ]s([:95int2:])([:95int2:])1([:name:])(?: (.+))? \endcode
*  \param $1   x-coordinate.
*  \param $2   y-coordinate.
*  \param $3   Dream name.
*  \param $4   Dream caption.
*  \sa S2C_DREAMPOS2 */
const int S2C_TYPE_DREAMPOS2     = 16;

/** Clear the dreampad text at a position.
*  \par regex:
*  \code ]t([:95int2:])([:95int2:]) \endcode
*  \param $1   x-coordinate.
*  \param $2   y-coordinate.
*  \sa S2C_DREAMCLEAR */
const int S2C_TYPE_DREAMCLEAR    = 17;

/** Sets a contiguous range of DS variable values.
*  \par regex:
*  \code 0([:95int2:])(([:95int3:])|!mL([:95int3:])([:95int3:]))+ \endcode
*  \param $1   First variable to set.
*  \param $2   Value to set individual variable.
*  \param $3   Length of flood fill, minus 1.
*  \param $4   Flood fill value.
*
*  This is a rather cryptic command that can
*  either set individual variable values
*  or "flood fill" a single value across a range.\n\n
*  than furc digits), if you wanted to set variable 100
*  to the value 32, the variable 101 to the value 64,
*  the variables 102 - 104 (inclusive) to the value 96,
*  and the variable 105 to the value 128, this is what
*  the server would send (spaces added for readability):\n\n
*  \code
*  0 100 32 64 !mL 2 96 128
*  \endcode
*  \n
*  To further illustrate the format of this command,
*  here is the reverse-engineered parsing code from the official
*  client:
*  \include 0_from_server.cpp
*
*  \sa S2C_VARSET */
const int S2C_TYPE_VARSET        = 18;

/** Sets floor tiles at positions.
*  \par regex:
*  \code 1(?:([:95int2:])([:95int2:])([:95int2:]))+ \endcode
*  \param $1   x-coordinate.
*  \param $2   y-coordinate.
*  \param $3   floor id.
*
*  Normally the server sends individual commands
*  per floor, but when a user first enters a map,
*  it may glob them all together into a big floorset
*  command, where each number triplet pertains to
*  a different object.
*
*  \sa S2C_FLOORSET */
const int S2C_TYPE_FLOORSET      = 19;

/** Buffers a badge for the next chat command.
*  \par regex:
*  \code ]-#B([:95int1:]) \endcode
*  \param $1   Badge frame.
*  \sa S2C_BADGE */
const int S2C_TYPE_BADGE         = 20;

/** The result of an online query.
*  \par regex:
*  \code ]%(0|1)([:name:]) \endcode
*  \param $1   Online status (0 for offline).
*  \param $2   User name.
*  \sa S2C_ONLINERESULT */
const int S2C_TYPE_ONLINERESULT  = 21;

/** Sets an object at the player's feet.
*  \par regex:
*  \code %([:95int2:]) \endcode
*  \param $1   Object.
*  \sa S2C_FOOTOBJ */
const int S2C_TYPE_FOOTOBJ       = 22;

/** Sets the object in the player's inventory.
*  \par regex:
*  \code \^([:95int2:]) \endcode
*  \param $1   Object.
*  \sa S2C_INVOBJ */
const int S2C_TYPE_INVOBJ        = 23;

/** Sets the current portrait and name.
*  \par regex:
*  \code ]f([:colors:])([:gsd:])([:name:]) \endcode
*  \param $1   Colors.
*  \param $2   Gender-species-Digo.
*  \param $3   Player name.
*  \sa S2C_PORTRAIT */
const int S2C_TYPE_PORTRAIT      = 24;

/** Instructs the client to fetch a portrait by name/ID.
*  \par regex:
*  \code ]&([0-9]+) (.+) \endcode
*  \param $1   pid (portrait ID).
*  \param $2   Portrait name.
*  \sa S2C_FETCHPID */
const int S2C_TYPE_FETCHPID      = 25;

/** Instructs the client to fetch a dream by name/ID.
*  \par regex:
*  \code ]q (.+) ([0-9]+) \endcode
*  \param $1   Dream name.
*  \param $2   Dream ID.
*  \sa S2C_FETCHDREAM */
const int S2C_TYPE_FETCHDREAM    = 26;

/** Instructs the client to load a dream by name/ID.
*  \par regex:
*  \code ]r (.+) ([0-9]+) \endcode
*  \param $1   Dream name.
*  \param $2   Dream ID.
*  \note Appears to be deprecated after december 28 reset. Just rely on FETCHDREAM now.
*  \sa S2C_USEDREAM */
const int S2C_TYPE_USEDREAM      = 27;

/** Instructs the client to use a background image by file name.
*  \par regex:
*  \code ]cc(.+) \endcode
*  \param $1   File name.
*  \sa S2C_USEBGFILE */
const int S2C_TYPE_USEBGFILE     = 28;

/** Instructs the client to play a song.
*  \par regex:
*  \code ]j([:95int2:]) \endcode
*  \param $1   Song number.
*  \sa S2C_PLAYSONG */
const int S2C_TYPE_PLAYSONG      = 29;

/** Instructs the client to play a sound.
*  \par regex:
*  \code !([:95int2:]) \endcode
*  \param $1   Sound number.
*  \sa S2C_PLAYSOUND */
const int S2C_TYPE_PLAYSOUND     = 30;

/** Regular player speech.
*  \par regex:
*  \code \(<name shortname='([^']*)'>([:name:])</name>: (.*) \endcode
*  \param $1   Player short name.
*  \param $2   Player name.
*  \param $3   Message.
*  \sa S2C_SPEECH */
const int S2C_TYPE_SPEECH        = 31;

/** Incoming whisper.
*  \par regex:
*  \code \(<font color='whisper'>\[ <name shortname='([^']*)' src='whisper-from'>([:name:])</name> whispers, "(.*)" to you\. ]</font> \endcode
*  \param $1   Player short name.
*  \param $2   Player.
*  \param $3   Message.
*  \sa S2C_WHISPER */
const int S2C_TYPE_WHISPER       = 32;

/** Incoming shout.
*  \par regex:
*  \code \(<font color='shout'>\{S} <name shortname='([^']*)'>([:name:])</name> shouts: (.*)</font> \endcode
*  \param $1   Player short name.
*  \param $2   Player.
*  \param $3   Message.
*  \sa S2C_SHOUT */
const int S2C_TYPE_SHOUT         = 33;

/** Incoming emote/action.
*  \par regex:
*  \code \(<font color='emote'><name shortname='([^']*)'>([:name:])</name>(.*)</font> \endcode
*  \param $1   Player short name.
*  \param $2   Player.
*  \param $3   Action.
*  \sa S2C_EMOTE */
const int S2C_TYPE_EMOTE         = 34;

/** Emit message.
*  \par regex:
*  \code \(<font color='emit'><img src='fsh://system.fsh:91' alt='@emit' /><channel name='@emit' /> (.*)</font> \endcode
*  \param $1   Message.
*  \sa S2C_EMIT */
const int S2C_TYPE_EMIT          = 35;

/** "Quiet" roll results.
*  \par regex:
*  \code \(<font color='roll'><img src='fsh://system.fsh:101' alt='@roll' /><channel name='@roll' /> <name shortname='([^']*)'>([:name:])</name> rolls ([0-9]*)d([0-9]+) (?:(.*) )?& gets ([0-9]+)\.</font> \endcode
*  \param $1   Player short name.
*  \param $2   Player.
*  \param $3   Number of dice.
*  \param $4   Number of sides.
*  \param $5   Message.
*  \param $6   Sum.
*  \sa S2C_ROLL */
const int S2C_TYPE_ROLL          = 36;

/** "Verbose" roll results.
*  \par regex:
*  \code \(<font color='roll'><img src='fsh://system.fsh:101' alt='@roll' /><channel name='@roll' /> <name shortname='([^']*)'>([:name:])</name> rolls ([0-9]*)d([0-9]+)(?: (.*))?:( \([0-9]+\))+ = ([0-9]+)\.</font> \endcode
*  \param $1   Player short name.
*  \param $2   Player.
*  \param $3   Number of dice.
*  \param $4   Number of sides.
*  \param $5   Message.
*  \param $6   Per-dice roll value.
*  \param $7   Sum.
*  \sa S2C_VERBOSEROLL */
const int S2C_TYPE_VERBOSEROLL   = 37;

/** The "You see..." part of the desc.
*  \par regex:
*  \code \(\(You see <name shortname='([^']*)'>([:name:])</name>\.\) \endcode
*  \param $1   Player short name.
*  \param $2   Player.
*  \sa S2C_DESCHEAD */
const int S2C_TYPE_DESCHEAD      = 38;

/** A regular player's description.
*  \par regex:
*  \code \(<desc shortname='([^']*)' />&gt; (.*) \endcode
*  \param $1   Player short name.
*  \param $2   Desc body.
*  \sa S2C_DESCBODY */
const int S2C_TYPE_DESCBODY      = 39;

/** Uptime line of the who info.
*  \par regex:
*  \code \(<img src='fsh://system.fsh:86'  /> ([0-9]+) total players online. Uptime(?: ([0-9]+) days)? ([0-9]+):([0-9]+) \endcode
*  \param $1   Players online.
*  \param $2   Days up.
*  \param $3   Hours up.
*  \param $4   Minutes up.
*  \sa S2C_WHOUPTIME */
const int S2C_TYPE_WHOUPTIME     = 40;

/** Max player count of the who info.
*  \par regex:
*  \code \(<img src='fsh://system.fsh:86'  /> Max players this session: 4005 \(\* Max players this session: ([0-9]+) \endcode
*  \param $1   Max players this session (duh).
*  \sa S2C_WHOMAX */
const int S2C_TYPE_WHOMAX        = 43;

/** Daily max player count of the who info.
*  \par regex:
*  \code \(<img src='fsh://system.fsh:86'  /> Max players today: ([0-9]+) \endcode
*  \param $1   Max players today (duh).
*  \sa S2C_WHODAILYMAX */
const int S2C_TYPE_WHODAILYMAX   = 44;

/** Cookies have finished baking.
*  \par regex:
*  \code \(<img src='fsh://system.fsh:90' alt='@cookie' /><channel name='@cookie' /> Your cookies are ready\. \endcode
*  \sa S2C_COOKIESREADY */
const int S2C_TYPE_COOKIESREADY  = 45;

/** Received a cookie.
*  \par regex:
*  \code \(<img src='fsh://system.fsh:90' alt='@cookie' /><channel name='@cookie' /> ([:name:]) just gave you a cookie!(?: It has a note attached which reads: (.+))? \endcode
*  \param $1   Player that sent the cookie.
*  \param $2   Attached note.
*  \sa S2C_COOKIE */
const int S2C_TYPE_COOKIE        = 46;

/** An 'Item:' line from user info.
*  \par regex:
*  \code \(Item: (.+)\ \(([0-9]+) active, ([0-9]+) inactive\) \endcode
*  \param $1   Item name.
*  \param $2   Quantity active.
*  \param $3   Quantity inactive.
*  \sa S2C_INFOITEM */
const int S2C_TYPE_INFOITEM      = 47;

/** The 'Player Info:' line from user info.
*  \par regex:
*  \code \(Player info: ([:name:]) \((.+)\) \endcode
*  \param $1   Player name.
*  \param $2   Player email.
*  \sa S2C_INFOPLAYER */
const int S2C_TYPE_INFOPLAYER    = 48;

/** Join request.
*  \par regex:
*  \code \(<font color='query'><name shortname='([^']*)'>([:name:])</name> requests permission to join your company. To accept the request, <a href='command://summon'>click here</a> or type `summon and press &lt;enter&gt;.</font> \endcode
*  \param $1   Player short name.
*  \param $2   Player name.
*  \sa S2C_JOIN */
const int S2C_TYPE_JOIN        = 49;

/** Summon request.
*  \par regex:
*  \code \(<font color='query'><name shortname='([^']*)'>([:name:])</name> asks you to join his or her company in <b>(.+)</b>. To accept the request, <a href='command://join'>click here</a> or type `join and press &lt;enter&gt;.</font> \endcode
*  \param $1   Player short name.
*  \param $2   Player name.
*  \param $3   Destination.
*  \sa S2C_SUMMON */
const int S2C_TYPE_SUMMON          = 50;

/** Whisper recipient's name couldn't be partially matched.
*  \par regex:
*  \code \(<font color='error'>Sorry, there are no furres around right now with a name starting with ([:name:])!  -- Beekin the Help Dragon</font> \endcode
*  \param $1   Partial name.
*  \sa S2C_WHISPERFAIL */
const int S2C_TYPE_WHISPERFAIL   = 51;

/** Summon/Join target's name couldn't be partially matched.
*  Alias of S2C_TYPE_WHISPERFAIL.
*  \sa S2C_SUMMONJOINFAIL */
const int S2C_TYPE_SUMMONJOINFAIL = 51; // S2C_TYPE_WHISPERFAIL;

/** Echo of what you whispered.
*  \par regex:
*  \code \(<font color='whisper'>\[ You whisper "(.*)" to <name shortname='([^']*)' forced src='whisper-to'>([:name:])</name>\. ]</font> \endcode
*  \param $1   Message.
*  \param $2   Recipient's shortname.
*  \param $3   Recipient.
*  \sa S2C_YOUWHISPER */
const int S2C_TYPE_YOUWHISPER    = 52;

/** A generic chat message.
*  \par regex:
*  \code \((.*) \endcode
*  \param $1   Message.
*  \sa S2C_CHAT */
const int S2C_TYPE_CHAT          = 53;

/** Opens a display list.
*  \par regex:
*  \code ~ \endcode
*
*  The client should buffer all following graphical
*  commands (avatar/camera tweens, etc.) rather
*  than rendering the changes immediately.\n\n
*  Display lists are a necessary mechanism to
*  make events appear to coincide, regardless
*  of lag.  For example, it would be pretty jarring
*  for your avatar to move ahead of your camera for a second
*  until the camera tween command came in.
*
*  \sa S2C_DISPLISTOPEN, S2C_TYPE_DISPLISTCLOSE */
const int S2C_TYPE_DISPLISTOPEN  = 54;

/** Closes a display list.
*  \par regex:
*  \code = \endcode
*
*  The client should immediately render all the buffered
*  graphical commands accumulated since the display list
*  was opened to make them appear to occur simultaneously.
*
*  \sa S2C_DISPLISTCLOSE, S2C_TYPE_DISPLISTOPEN */
const int S2C_TYPE_DISPLISTCLOSE = 55;

/** Change the client's internal color code.
*  \par regex:
*  \code ]}([:colors:])([:gsd:]) \endcode
*  \param $1   New colors.
*  \param $2   New gender-species-digo.
*
*  The client keeps track of what it thinks the player's color and
*  speci codes are, and in the cases of silver sponsors, the server
*  must inform the client of when changes have occured.  The server
*  will also send this as you enter each map to make sure the client
*  is on the same page.
*
*  \sa S2C_SYNCCOLORCODE */
const int S2C_TYPE_SYNCCOLORCODE = 56;

/** Echo of what you shouted.
*  \par regex:
*  \code \(<font color='shout'>You shout, "(.*)"</font> \endcode
*  \param $1   Shout message.
*  \sa S2C_YOUSHOUT */
const int S2C_TYPE_YOUSHOUT      = 57;

/** Echo of what you said.
*  \par regex:
*  \code \(<font color='myspeech'>You say, "(.*)"</font> \endcode
*  \param $1   Speech message.
*  \sa S2C_YOUSAY */
const int S2C_TYPE_YOUSAY        = 58;

/** DS Trigger(s) induced by you.
*  \par regex:
*  \code 6([:95int2:])([:95int2:])([:95int2:])([:95int2:])(?:([:95int2:])([:95int2:])([:95int2:]))+ \endcode
*  \param $1   x1.
*  \param $2   y1.
*  \param $3   x2.
*  \param $4   y2.
*  \param $5   DS line.
*  \param $6   DS param.
*  \param $7   DS param.
*
*  Lines refer to a stripped DS script, where non-DS and empty lines
*  are removed.
*  \remarks I hope someone more familiar with DS can help me with a
*           better detailed description for this.
*
*  \sa S2C_TRIGGERSELF, S2C_TYPE_TRIGGER */
const int S2C_TYPE_TRIGGERSELF   = 59;

/** DS Trigger(s) induced by someone else or the server.
*  \par regex:
*  \code 7([:95int2:])([:95int2:])([:95int2:])([:95int2:])(?:([:95int2:])([:95int2:])([:95int2:]))+ \endcode
*  \param $1   x1.
*  \param $2   y1.
*  \param $3   x2.
*  \param $4   y2.
*  \param $5   DS line.
*  \param $6   DS param.
*  \param $7   DS param.
*
*  Lines refer to a stripped DS script, where non-DS and empty lines
*  are removed.
*  \remarks I hope someone more familiar with DS can help me with a
*           better detailed description for this.
*
*  \sa S2C_TRIGGER, S2C_TYPE_TRIGGERSELF */
const int S2C_TYPE_TRIGGER       = 60;

/** Request for the client version.
*  \par regex:
*  \code ]w \endcode
*  \sa S2C_VERREQ */
const int S2C_TYPE_VERREQ        = 61;

/** Ping.
*  \par regex:
*  \code ]marco ([0-9]+) \endcode
*  \param $1 Value that should be ponged back.
*  \sa S2C_PING */
const int S2C_TYPE_PING          = 62;

/** Buffers a specitag for the next chat command.
*  \par regex:
*  \code ]-#A([:colors:])([:gsd:]) \endcode
*  \param $1   Colors.
*  \param $2   Gender-Species-Digo.
*  \sa S2C_SPECITAG */
const int S2C_TYPE_SPECITAG      = 63;

/** Whisper recipient's name couldn't be fully matched.
*  \par regex:
*  \code \(<font color='error'>Sorry, there's no furre around right now with an exact name ([:name:])! To find similar names, try typing the name without the '%' at the beginning of the name\. -- Beekin the Help Dragon</font> \endcode
*  \param $1   Exact name.
*  \sa S2C_WHISPERFAILEXACT */
const int S2C_TYPE_WHISPERFAILEXACT = 64;

/** Summon/Join target's name couldn't be fully matched.
*  alias of S2C_TYPE_WHISPERFAILEXACT.
*  \sa S2C_SUMMONJOINFAILEXACT */
const int S2C_TYPE_SUMMONJOINFAILEXACT = 64; // S2C_TYPE_WHISPERFAILEXACT;

/** Whisper recipient's name is a partial match for more than one player.
*  \par regex:
*  \code \(<font color='error'>More than one furre's name begins that way.  You need to type more of the name, or maybe all of their name, so I can tell exactly who you mean!  -- Beekin the Help Dragon</font> \endcode
*  \sa S2C_WHISPERFAILMORE */
const int S2C_TYPE_WHISPERFAILMORE  = 65;

/** Summon target's name is a partial match for more than one player.
*  alias of S2C_TYPE_WHISPERFAILMORE
*  \sa S2C_SUMMONJOINFAILMORE */
const int S2C_TYPE_SUMMONJOINFAILMORE = 65; // S2C_TYPE_WHISPERFAILMORE;

/** A beekin's/staff's description.
*  \par regex:
*  \code \(#B([:95int1:])<desc shortname='([^']*)' /> (.*) \endcode
*  \param $1   Badge number.
*  \param $2   Player short name.
*  \param $3   Desc body.
*  \sa S2C_DESCBODY2 */
const int S2C_TYPE_DESCBODY2      = 66;

/** Instructs the client to use a map file by filename.
*  \par regex:
*  \code ;(.+) \endcode
*  \param $1   map filename (in root furcadia folder).
*  \sa S2C_USEMAPFILE */
const int S2C_TYPE_USEMAPFILE     = 67;

/** Whisper recipient has whispers disabled (SS).
*  \par regex:
*  \code \(<font color='error'>Sorry, but <name shortname='([^']*)'>([:name:])</name> is not accepting whispers right now\.</font> \endcode
*  \param $1   Whisper recipient short name.
*  \param $2   Whisper recipient.
*  \sa S2C_WHISPERDENIED */
const int S2C_TYPE_WHISPERDENIED  = 68;

/** A summon/join request echo.
*  \par regex:
*  \code \(<font color='success'>Your request has been sent to <name shortname='([^']*)'>([:name:])</name>\.</font> \endcode
*  \param $1   Summon/join target's short name.
*  \param $2   Summon/join target.
*  \sa S2C_SUMMONJOINECHO */
const int S2C_TYPE_SUMMONJOINECHO = 69;

/** A summon/join request was declined.
*  \par regex:
*  \code \(<font color='warning'>Player <name shortname='([^']*)'>([:name:])</name> declined your request\.</font> \endcode
*  \param $1   Summon/join target's short name.
*  \param $2   Summon/join target.
*  \sa S2C_DECLINED */
const int S2C_TYPE_DECLINED     = 70;

/** A summon/join request decline echo.
*  \par regex:
*  \code \(<font color='warning'>You decline the request from <name shortname='([^']*)'>([:name:])</name>\.</font> \endcode
*  \param $1   Summoner/joiner's short name.
*  \param $2   Summoner/joiner.
*  \sa S2C_DECLINEECHO */
const int S2C_TYPE_DECLINEECHO  = 71;

/** Decline echo with no requests.
*  \par regex:
*  \code \(<font color='error'>There are no requests to decline\.</font> \endcode
*  \sa S2C_DECLINEFAIL */
const int S2C_TYPE_DECLINEFAIL  = 72;

/** Join request accepted.
*  \par regex:
*  \code \(<font color='success'><name shortname='([^']*)'>([:name:])</name> summons you\.</font> \endcode
*  \param $1   Summoner's short name.
*  \param $2   Summoner.
*  \sa S2C_SUMMONED */
const int S2C_TYPE_SUMMONED = 73;

/** Summon request accepted.
*  \par regex:
*  \code \(<font color='success'><name shortname='([^']*)'>([:name:])</name> joins you\.</font> \endcode
*  \param $1   Joiner's short name.
*  \param $2   Joiner.
*  \sa S2C_JOINED */
const int S2C_TYPE_JOINED = 74;

/** Display a messagebox.
*  \par regex:
*  \code ]#([^ ]*) ([0-9]+) (.*) \endcode
*  \param $1   Unknown.
*  \param $2   Win32 MessageBox() flags.
*  \param $3   Message.
*  \sa S2C_MSGBOX */
const int S2C_TYPE_MSGBOX = 75;

/** Show an avatar with at the specified position and frame.
*  \par regex:
*  \code A([:220int4:])([:220int2:])([:220int2:])([:220int2:]) \endcode
*  \param $1   User ID.
*  \param $2   x-coordinate.
*  \param $3   y-coordinate.
*  \param $4   Frame.
*  \sa S2C_AVSHOW, frame2av, AVFRAME */
const int S2C_TYPE_AVSHOW       = 76;

/** Update an avatar's colors and frame.
*  \par regex:
*  \code B([:220int4:])([:220int2:])([:colors:]) \endcode
*  \param $1   User ID.
*  \param $2   Frame.
*  \param $3   Colors.
*  \sa S2C_AVUPDATECOLORS, frame2av, AVFRAME */
const int S2C_TYPE_AVUPDATECOLORS    = 77;

/** Hide an avatar at the specified position.
*  \par regex:
*  \code C([:220int4:])([:220int2:])([:220int2:]) \endcode
*  \param $1   User ID.
*  \param $2   x-coordinate.
*  \param $3   y-coordinate.
*  \sa S2C_AVHIDE, frame2av, AVFRAME */
const int S2C_TYPE_AVHIDE       = 78;

/** Instruct the client to print the player list and dream population.
*  \par regex:
*  \code ]3(0|1) ([:name:]) ([:name:]) \endcode
*  \param $1   '1' if the dream doesn't allow player listing, '0' otherwise.
*  \param $2   Name of dream.
*  \param $3   Caption of dream.
*  \sa S2C_PRINTPLAYERS */
const int S2C_TYPE_PRINTPLAYERS  = 79;

/** Show a player's avatar at the specified position and frame.
*   Does not move your camera if the avatar is yours. Usually seen
*   when DS moves your avatar.
*  \par regex:
*  \code D([:220int4:])([:220int2:])([:220int2:])([:220int2:])(.+) \endcode
*  \param $1   User ID.
*  \param $2   x-coordinate.
*  \param $3   y-coordinate.
*  \param $4   Frame.
*  \param $5   Unknown. Looks like 7-8 base-220 digits.
*  \sa S2C_AVSHOW2, frame2av, AVFRAME */
const int S2C_TYPE_AVSHOW2       = 80;

/** Emit triggered by DS.
*  \par regex:
*  \code \(<font color='dragonspeak'><img src='fsh://system.fsh:91' alt='@emit' /><channel name='@emit' /> (.*)</font> \endcode
*  \param $1   Message.
*  \sa S2C_DSEMIT */
const int S2C_TYPE_DSEMIT        = 81;

/** Base parsed client protocol structure. */
struct S2C
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
};

/** A parsed MOTD line.
*  \sa S2C_TYPE_MOTD. */
struct S2C_MOTD
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** A line of the MOTD. */
   char* line;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_MOTD, };
   S2C_MOTD( ) : type( CLSID )
   {

   }
   ~S2C_MOTD( )
   {
      delete [] line;
   }
#endif
};

/** A parsed Dragonroar command.
*  \sa S2C_TYPE_DRAGONROAR. */
struct S2C_DRAGONROAR
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_DRAGONROAR, };
   S2C_DRAGONROAR( ) : type( CLSID )
   {

   }
   ~S2C_DRAGONROAR( )
   {
   }
#endif
};

/** A parsed MOTD end command.
*  \sa S2C_TYPE_MOTDEND. */
struct S2C_MOTDEND
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_MOTDEND, };
   S2C_MOTDEND( ) : type( CLSID )
   {

   }
   ~S2C_MOTDEND( )
   {
   }
#endif
};

/** A parsed protocol version command.
*  \sa S2C_TYPE_VER. */
struct S2C_VER
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_VER, };
   S2C_VER( ) : type( CLSID )
   {

   }
   ~S2C_VER( )
   {
   }
#endif
};

/** A parsed login/create succeeded reply.
*  \sa S2C_TYPE_PASS. */
struct S2C_PASS
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_PASS, };
   S2C_PASS( ) : type( CLSID )
   {

   }
   ~S2C_PASS( )
   {

   }
#endif
};

/** A parsed create failed reply.
*  \sa S2C_TYPE_CREATEFAIL. */
struct S2C_CREATEFAIL
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

   /** Human readable failure code. */
   char* reason;

   /** Error category.
   *  Typically either "NAME", "PASS", or "EMAIL" */
   char* category;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_CREATEFAIL, };
   S2C_CREATEFAIL( ) : type( CLSID )
   {

   }
   ~S2C_CREATEFAIL( )
   {
      delete [] reason;
      delete [] category;
   }
#endif
};

/** A parsed machine ID response.
*  \sa S2C_TYPE_MACID. */
struct S2C_MACID
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** The scrambled machine ID response. */
   char* mac_id;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_MACID, };
   S2C_MACID( ) : type( CLSID )
   {

   }
   ~S2C_MACID( )
   {
      delete [] mac_id;
   }
#endif
};

/** A parsed login fail response.
*  \sa S2C_TYPE_LOGINFAIL. */
struct S2C_LOGINFAIL
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Human readable failure code. */
   char* reason;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_LOGINFAIL, };
   S2C_LOGINFAIL( ) : type( CLSID )
   {

   }
   ~S2C_LOGINFAIL( )
   {
      delete [] reason;
   }
#endif
};

/** A parsed camera set command.
*  \sa S2C_TYPE_CAMSET. */
struct S2C_CAMSET
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** New coordinates. */
   unsigned int  x;
   /** New coordinates. */
   unsigned int  y;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_CAMSET, };
   S2C_CAMSET( ) : type( CLSID )
   {

   }
   ~S2C_CAMSET( )
   {
   }
#endif
};

/** A parsed camera tween command.
*  \sa S2C_TYPE_CAMTWEEN. */
struct S2C_CAMTWEEN
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** New coordinates. */
   unsigned int  x;
   /** New coordinates. */
   unsigned int  y;
   /** Old coordinates. */
   unsigned int  x_old;
   /** Old coordinates. */
   unsigned int  y_old;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_CAMTWEEN, };
   S2C_CAMTWEEN( ) : type( CLSID )
   {

   }
   ~S2C_CAMTWEEN( )
   {
   }
#endif
};

/** A parsed series of object set commands.
*  \sa S2C_TYPE_OBJSET. */
struct S2C_OBJSET
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** How many objects to set. */
   unsigned int count;
   /** Per-object info. */
   struct IDXY*    objects;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_OBJSET, };
   S2C_OBJSET( ) : type( CLSID )
   {

   }
   ~S2C_OBJSET( )
   {
      delete [] objects;
   }
#endif
};

/** A parsed avatar create command.
*  \sa S2C_TYPE_AVCREATE. */
struct S2C_AVCREATE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** User ID. */
   unsigned int  uid;
   /** Position. */
   unsigned int  x;
   /** Position. */
   unsigned int  y;
   /** The avatar frame. */
   unsigned int  frame;
   /** Name. */
   char*         name;
   /** Color code. */
   char*         colors;
   /** Flags. */
   unsigned int  flags;
#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_AVCREATE, };
   S2C_AVCREATE( ) : type( CLSID )
   {
   }
   ~S2C_AVCREATE( )
   {
      delete [] name;
      delete [] colors;
   }
#endif
};

/** A parsed avatar tween command.
*  \sa S2C_TYPE_AVTWEEN. */
struct S2C_AVTWEEN
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** User ID. */
   unsigned int  uid;
   /** Position. */
   unsigned int  x;
   /** Position. */
   unsigned int  y;
   /** End frame. */
   unsigned int  frame;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_AVTWEEN, };
   S2C_AVTWEEN( ) : type( CLSID )
   {

   }
   ~S2C_AVTWEEN( )
   {
   }
#endif
};

/** A parsed avatar destroy command.
*  \sa S2C_TYPE_AVDESTROY. */
struct S2C_AVDESTROY
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** User ID. */
   unsigned int  uid;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_AVDESTROY, };
   S2C_AVDESTROY( ) : type( CLSID )
   {

   }
   ~S2C_AVDESTROY( )
   {
   }
#endif
};

/** A parsed dream text command.
*  \sa S2C_TYPE_DREAMPOS. */
struct S2C_DREAMPOS
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Dream text. */
   char* name;
   /** Position. */
   unsigned int        x;
   /** Position. */
   unsigned int        y;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_DREAMPOS, };
   S2C_DREAMPOS( ) : type( CLSID )
   {

   }
   ~S2C_DREAMPOS( )
   {
      delete [] name;
   }
#endif
};

/** A parsed dream text and caption command.
*  \sa S2C_TYPE_DREAMPOS2. */
struct S2C_DREAMPOS2
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Dream text. */
   char* name;
   /** Dream caption. */
   char* caption;
   /** Position. */
   unsigned int        x;
   /** Position. */
   unsigned int        y;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_DREAMPOS2, };
   S2C_DREAMPOS2( ) : type( CLSID )
   {

   }
   ~S2C_DREAMPOS2( )
   {
      delete [] name;
      delete [] caption;
   }
#endif
};

/** A parsed dream text clear command.
*  \sa S2C_TYPE_DREAMCLEAR. */
struct S2C_DREAMCLEAR
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Position. */
   unsigned int  x;
   /** Position. */
   unsigned int  y;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_DREAMCLEAR, };
   S2C_DREAMCLEAR( ) : type( CLSID )
   {

   }
   ~S2C_DREAMCLEAR( )
   {
   }
#endif
};

/** A parsed series of variable set commands.
*   \sa S2C_TYPE_VARSET. */
struct S2C_VARSET
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Number of ranges to set. */
   unsigned int     count;
   /** Ranges of variables to set. */
   struct VARTRIP
   {
      /** First variable to set. */
      unsigned int  first;
      /** How many variables to set. */
      unsigned int  count;
      /** Value to set the range to. */
      unsigned int  value;
   }        *set;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_VARSET, };
   S2C_VARSET( ) : type( CLSID )
   {

   }
   ~S2C_VARSET( )
   {
      delete [] set;
   }
#endif
};

/** A parsed series of floor set commands.
*   \sa S2C_TYPE_FLOORSET. */
struct S2C_FLOORSET
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** How many floors to set. */
   unsigned int count;
   /** Per-floor info. */
   struct IDXY* floors;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_FLOORSET, };
   S2C_FLOORSET( ) : type( CLSID )
   {

   }
   ~S2C_FLOORSET( )
   {
      delete [] floors;
   }
#endif
};

/** A badge command.
*   \sa S2C_TYPE_BADGE. */
struct S2C_BADGE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Badge frame (for beekins, or the like). */
   unsigned int badge;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_BADGE, };
   S2C_BADGE( ) : type( CLSID )
   {

   }
   ~S2C_BADGE( )
   {
   }
#endif
};

/** A specitag command.
*   \sa S2C_TYPE_SPECITAG. */
struct S2C_SPECITAG
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int     type;
   /** Specitag colors. */
   char* colors;
   /** Specitag species. */
   char  gsd[ 3 ];

   char* url;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_SPECITAG, };
   S2C_SPECITAG( ) : type( CLSID )
   {

   }
   ~S2C_SPECITAG( )
   {
      delete [] colors;
	  delete [] url;
   }
#endif
};

/** A parsed online check reply.
*   \sa S2C_TYPE_ONLINERESULT. */
struct S2C_ONLINERESULT
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player name */
   char*     player;
   /** Online status. */
   bool      online;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_ONLINERESULT, };
   S2C_ONLINERESULT( ) : type( CLSID )
   {

   }
   ~S2C_ONLINERESULT( )
   {
      delete [] player;
   }
#endif
};

/** A parsed display portrait command.
*   \sa S2C_TYPE_PORTRAIT. */
struct S2C_PORTRAIT
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int     type;
   /** Player name. */
   char*         player;
   /** Colors. */
   char*         colors;
   /** GSD. */
   char          gsd[ 3 ];

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_PORTRAIT, };
   S2C_PORTRAIT( ) : type( CLSID )
   {

   }
   ~S2C_PORTRAIT( )
   {
      delete [] player;
      delete [] colors;
   }
#endif
};

/** A parsed display object at feet command.
*   \sa S2C_TYPE_FOOTOBJ.
*/
struct S2C_FOOTOBJ
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Object ID (frame). */
   unsigned int  obj;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_FOOTOBJ, };
   S2C_FOOTOBJ( ) : type( CLSID )
   {

   }
   ~S2C_FOOTOBJ( )
   {
   }
#endif
};

/** A parsed display object in inventory command.
*   \sa S2C_TYPE_INVOBJ. */
struct S2C_INVOBJ
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Object ID (frame). */
   unsigned int obj;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_INVOBJ, };
   S2C_INVOBJ( ) : type( CLSID )
   {

   }
   ~S2C_INVOBJ( )
   {
   }
#endif
};

/** A parsed fetch portrait command.
*   \sa S2C_TYPE_FETCHPID. */
struct S2C_FETCHPID
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Fileserver portrait name. */
   char*        name;
   /** Fileserver portrait id. */
   unsigned int pid;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_FETCHPID, };
   S2C_FETCHPID( ) : type( CLSID )
   {

   }
   ~S2C_FETCHPID( )
   {
      delete [] name;
   }
#endif
};

/** A parsed fetch dream command.
*   \sa S2C_TYPE_FETCHDREAM. */
struct S2C_FETCHDREAM
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Fileserver dream name. */
   char*        name;
   /** Fileserver dream id. */
   unsigned int did;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_FETCHDREAM, };
   S2C_FETCHDREAM( ) : type( CLSID )
   {

   }
   ~S2C_FETCHDREAM( )
   {
      delete [] name;
   }
#endif
};

/** A parsed use dream command.
*   \sa S2C_TYPE_USEDREAM. */
struct S2C_USEDREAM
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Fileserver dream name. */
   char*        name;
   /** Fileserver dream id. */
   unsigned int did;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_USEDREAM, };
   S2C_USEDREAM( ) : type( CLSID )
   {

   }
   ~S2C_USEDREAM( )
   {
      delete [] name;
   }
#endif
};

/** A parsed use background file command.
*   \sa S2C_TYPE_USEBGFILE. */
struct S2C_USEBGFILE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Background filename. */
   char*     file;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_USEBGFILE, };
   S2C_USEBGFILE( ) : type( CLSID )
   {

   }
   ~S2C_USEBGFILE( )
   {
      delete [] file;
   }
#endif
};

/** A parsed use map file command.
*   \sa S2C_TYPE_USEMAPFILE. */
struct S2C_USEMAPFILE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Map filename. */
   char*     file;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_USEMAPFILE, };
   S2C_USEMAPFILE( ) : type( CLSID )
   {

   }
   ~S2C_USEMAPFILE( )
   {
      delete [] file;
   }
#endif
};

/** A parsed play song command.
*   \sa S2C_TYPE_PLAYSONG. */
struct S2C_PLAYSONG
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Song number. */
   unsigned int song;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_PLAYSONG, };
   S2C_PLAYSONG( ) : type( CLSID )
   {

   }
   ~S2C_PLAYSONG( )
   {
   }
#endif
};

/** A parsed play sound command.
*   \sa S2C_TYPE_PLAYSOUND. */
struct S2C_PLAYSOUND
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Sound number. */
   unsigned int sound;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_PLAYSOUND, };
   S2C_PLAYSOUND( ) : type( CLSID )
   {

   }
   ~S2C_PLAYSOUND( )
   {
   }
#endif
};

/** A parsed player speech command.
*   \sa S2C_TYPE_SPEECH. */
struct S2C_SPEECH
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player short name. */
   char*     from_short;
   /** Player name. */
   char*     from;
   /** Message. */
   char*     message;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_SPEECH, };
   S2C_SPEECH( ) : type( CLSID )
   {

   }
   ~S2C_SPEECH( )
   {
      delete [] from_short;
      delete [] from;
      delete [] message;
   }
#endif
};

/** A parsed player whisper command.
*   \sa S2C_TYPE_WHISPER. */
struct S2C_WHISPER
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player short name. */
   char* from_short;
   /** Player name. */
   char* from;
   /** Message. */
   char* message;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_WHISPER, };
   S2C_WHISPER( ) : type( CLSID )
   {

   }
   ~S2C_WHISPER( )
   {
      delete [] from_short;
      delete [] from;
      delete [] message;
   }
#endif
};

/** A parsed player shout command.
*   \sa S2C_TYPE_SHOUT. */
struct S2C_SHOUT
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player short name. */
   char*     from_short;
   /** Player name. */
   char*     from;
   /** Message. */
   char*     message;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_SHOUT, };
   S2C_SHOUT( ) : type( CLSID )
   {

   }
   ~S2C_SHOUT( )
   {
      delete [] from_short;
      delete [] from;
      delete [] message;
   }
#endif
};

/** A parsed player emote command.
*   \sa S2C_TYPE_EMOTE. */
struct S2C_EMOTE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player short name. */
   char*     from_short;
   /** Player name. */
   char*     from;
   /** Message/emote. */
   char*     message;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_EMOTE, };
   S2C_EMOTE( ) : type( CLSID )
   {

   }
   ~S2C_EMOTE( )
   {
      delete [] from_short;
      delete [] from;
      delete [] message;
   }
#endif
};

/** A parsed emit command.
*  \sa S2C_TYPE_EMIT */
struct S2C_EMIT
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Emit message. */
   char*     message;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_EMIT, };
   S2C_EMIT( ) : type( CLSID )
   {

   }
   ~S2C_EMIT( )
   {
      delete [] message;
   }
#endif
};

/** A parsed "quiet" roll reply.
*   \sa S2C_TYPE_ROLL. */
struct S2C_ROLL
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Player short name. */
   char*        player_short;
   /** Player name. */
   char*        player;
   /** Message. */
   char*        message;
   /** Number of dice. */
   unsigned int dice;
   /** Number of sides. */
   unsigned int sides;
   /** Sum of the rolls. */
   unsigned int sum;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_ROLL, };
   S2C_ROLL( ) : type( CLSID )
   {

   }
   ~S2C_ROLL( )
   {
      delete [] player;
      delete [] message;
   }
#endif
};

/** A parsed ROLL reply.
*   \sa S2C_TYPE_VERBOSEROLL. */
struct S2C_VERBOSEROLL
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int     type;
   /** Player short name. */
   char*        player_short;
   /** Player name. */
   char*         player;
   /** Message. */
   char*         message;
   /** Number of dice. */
   unsigned int  dice;
   /** Number of sides. */
   unsigned int  sides;
   /** Individual roll values. */
   unsigned int* values;
   /** Sum of all values. */
   unsigned int  sum;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_VERBOSEROLL, };
   S2C_VERBOSEROLL( ) : type( CLSID )
   {

   }
   ~S2C_VERBOSEROLL( )
   {
      delete [] player;
      delete [] message;
      delete [] values;
   }
#endif
};

/** A parsed desc head reply.
*   \sa S2C_TYPE_DESCHEAD. */
struct S2C_DESCHEAD
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player short name. */
   char* player_short;
   /** Player name. */
   char* player;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_DESCHEAD, };
   S2C_DESCHEAD( ) : type( CLSID )
   {

   }
   ~S2C_DESCHEAD( )
   {
      delete [] player_short;
      delete [] player;
   }
#endif
};

/** A parsed desc body reply.
*   \sa S2C_TYPE_DESCBODY. */
struct S2C_DESCBODY
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player short name. */
   char* player_short;
   /** Desc body. */
   char*     body;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_DESCBODY, };
   S2C_DESCBODY( ) : type( CLSID )
   {

   }
   ~S2C_DESCBODY( )
   {
      delete [] player_short;
      delete [] body;
   }
#endif
};

/** A parsed desc body reply.
*   \sa S2C_TYPE_DESCBODY2. */
struct S2C_DESCBODY2
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Player badge. */
   unsigned int badge;
   /** Player short name. */
   char*        player_short;
   /** Desc body. */
   char*        body;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_DESCBODY2, };
   S2C_DESCBODY2( ) : type( CLSID )
   {

   }
   ~S2C_DESCBODY2( )
   {
      delete [] player_short;
      delete [] body;
   }
#endif
};

/** A parsed line of the who info.
*   \sa S2C_TYPE_WHODAILYMAX. */
struct S2C_WHODAILYMAX
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Maximum number of players today. */
   unsigned int daily_max;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_WHODAILYMAX, };
   S2C_WHODAILYMAX( ) : type( CLSID )
   {

   }
   ~S2C_WHODAILYMAX( )
   {
   }
#endif
};

/** A parsed line of the who info.
*   \sa S2C_TYPE_WHOMAX. */
struct S2C_WHOMAX
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Maximum number of players this session. */
   unsigned int session_max;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_WHOMAX, };
   S2C_WHOMAX( ) : type( CLSID )
   {

   }
   ~S2C_WHOMAX( )
   {
   }
#endif
};

/** A parsed line of the who info.
*   \sa S2C_TYPE_WHOUPTIME. */
struct S2C_WHOUPTIME
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Number of players online. */
   unsigned int players_online;
   /** Days up. */
   unsigned int days_up;
   /** Hours up. */
   unsigned int hours_up;
   /** Minutes up. */
   unsigned int minutes_up;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_WHOUPTIME, };
   S2C_WHOUPTIME( ) : type( CLSID )
   {

   }
   ~S2C_WHOUPTIME( )
   {
   }
#endif
};

/** A parsed received cookie notification.
*   \sa S2C_TYPE_COOKIE. */
struct S2C_COOKIE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player name. */
   char*     from;
   /** Optional note. */
   char*     note;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_COOKIE, };
   S2C_COOKIE( ) : type( CLSID )
   {

   }
   ~S2C_COOKIE( )
   {
      delete [] from;
      delete [] note;
   }
#endif
};

/** A parsed cookies ready notification.
*   \sa S2C_TYPE_COOKIESREADY. */
struct S2C_COOKIESREADY
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_COOKIESREADY, };
   S2C_COOKIESREADY( ) : type( CLSID )
   {

   }
   ~S2C_COOKIESREADY( )
   {
   }
#endif
};

/** A parsed line of the player info.
*   \sa S2C_TYPE_INFOITEM. */
struct S2C_INFOITEM
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Name of the user item. */
   char*        item_name;
   /** Number of the items active. */
   unsigned int num_active;
   /** Number of the items inactive. */
   unsigned int num_inactive;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_INFOITEM, };
   S2C_INFOITEM( ) : type( CLSID )
   {

   }
   ~S2C_INFOITEM( )
   {
      delete [] item_name;
   }
#endif
};

/** A parsed line of the player info.
*   \sa S2C_TYPE_INFOPLAYER. */
struct S2C_INFOPLAYER
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player name. */
   char*     player;
   /** Player email. */
   char*     email;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_INFOPLAYER, };
   S2C_INFOPLAYER( ) : type( CLSID )
   {

   }
   ~S2C_INFOPLAYER( )
   {
      delete [] player;
      delete [] email;
   }
#endif
};

/** A parsed summon request.
*   \sa S2C_TYPE_SUMMON. */
struct S2C_SUMMON
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player short name. */
   char*     from_short;
   /** Player name. */
   char*     from;
   /** Destination. */
   char*     dest;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_SUMMON, };
   S2C_SUMMON( ) : type( CLSID )
   {

   }
   ~S2C_SUMMON( )
   {
      delete [] from_short;
      delete [] from;
      delete [] dest;
   }
#endif
};

/** A parsed join request.
*   \sa S2C_TYPE_JOIN. */
struct S2C_JOIN
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player short name. */
   char*     from_short;
   /** Player name. */
   char*     from;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_JOIN, };
   S2C_JOIN( ) : type( CLSID )
   {

   }
   ~S2C_JOIN( )
   {
      delete [] from_short;
      delete [] from;
   }
#endif
};

/** A parsed whisper fail response.
*   \sa S2C_TYPE_WHISPERFAIL. */
struct S2C_WHISPERFAIL
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Partial player name. */
   char*     player;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_WHISPERFAIL, };
   S2C_WHISPERFAIL( ) : type( CLSID )
   {

   }
   ~S2C_WHISPERFAIL( )
   {
      delete [] player;
   }
#endif
};

/** A parsed summon fail response.
*   \sa S2C_TYPE_SUMMONJOINFAIL. */
#define S2C_SUMMONJOINFAIL S2C_WHISPERFAIL

/** A parsed whisper fail response.
*   \sa S2C_TYPE_WHISPERFAILEXACT. */
struct S2C_WHISPERFAILEXACT
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Exact player name. */
   char*     player;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_WHISPERFAILEXACT, };
   S2C_WHISPERFAILEXACT( ) : type( CLSID )
   {

   }
   ~S2C_WHISPERFAILEXACT( )
   {
      delete [] player;
   }
#endif
};

/** A parsed summon fail response.
*   \sa S2C_TYPE_SUMMONJOINFAIL. */
#define S2C_SUMMONJOINFAILEXACT S2C_WHISPERFAILEXACT

/** A parsed whisper fail response.
*   \sa S2C_TYPE_WHISPERFAILMORE. */
struct S2C_WHISPERFAILMORE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_WHISPERFAILMORE, };
   S2C_WHISPERFAILMORE( ) : type( CLSID )
   {

   }
   ~S2C_WHISPERFAILMORE( )
   {
   }
#endif
};

/** A parsed summon fail response.
*   \sa S2C_TYPE_SUMMONJOINFAILMORE. */
#define S2C_SUMMONJOINFAILMORE S2C_WHISPERFAILMORE

/** A parsed whisper echo.
*   \sa S2C_TYPE_YOUWHISPER. */
struct S2C_YOUWHISPER
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Whisper recipient's short name. */
   char*     to_short;
   /** Whisper recipient. */
   char*     to;
   /** Whisper message. */
   char*     message;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_YOUWHISPER, };
   S2C_YOUWHISPER( ) : type( CLSID )
   {

   }
   ~S2C_YOUWHISPER( )
   {
      delete [] to_short;
      delete [] to;
      delete [] message;
   }
#endif
};

/** A parsed generic chat message command.
*   \sa S2C_TYPE_CHAT. */
struct S2C_CHAT
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Chat message. */
   char*     message;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_CHAT, };
   S2C_CHAT( ) : type( CLSID )
   {

   }
   ~S2C_CHAT( )
   {
      delete [] message;
   }
#endif
};

/** A parsed display list open command.
*   \sa S2C_TYPE_DISPLISTOPEN. */
struct S2C_DISPLISTOPEN
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_DISPLISTOPEN, };
   S2C_DISPLISTOPEN( ) : type( CLSID )
   {

   }
   ~S2C_DISPLISTOPEN( )
   {
   }
#endif
};

/** A parsed display list close command.
*   \sa S2C_TYPE_DISPLISTCLOSE. */
struct S2C_DISPLISTCLOSE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_DISPLISTCLOSE, };
   S2C_DISPLISTCLOSE( ) : type( CLSID )
   {

   }
   ~S2C_DISPLISTCLOSE( )
   {
   }
#endif
};

/** A parsed synchronize color code command.
*   \sa S2C_TYPE_SYNCCOLORCODE. */
struct S2C_SYNCCOLORCODE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int     type;
   /** colors */
   char*         colors;
   /** GSD */
   char          gsd[ 3 ];

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_SYNCCOLORCODE, };
   S2C_SYNCCOLORCODE( ) : type( CLSID )
   {

   }
   ~S2C_SYNCCOLORCODE( )
   {
      delete [] colors;
   }
#endif
};

/** A parsed shout echo.
*   \sa S2C_TYPE_YOUSHOUT. */
struct S2C_YOUSHOUT
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Shout message. */
   char*     message;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_YOUSHOUT, };
   S2C_YOUSHOUT( ) : type( CLSID )
   {

   }
   ~S2C_YOUSHOUT( )
   {
      delete [] message;
   }
#endif
};

/** A parsed speech echo.
*   \sa S2C_TYPE_YOUSAY. */
struct S2C_YOUSAY
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Speech message. */
   char*     message;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_YOUSAY, };
   S2C_YOUSAY( ) : type( CLSID )
   {

   }
   ~S2C_YOUSAY( )
   {
      delete [] message;
   }
#endif
};

/** A parsed trigger.
*   \sa S2C_TYPE_TRIGGER. */
struct S2C_TRIGGER
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** DS-dependent value. */
   unsigned int x1;
   /** DS-dependent value. */
   unsigned int y1;
   /** DS-dependent value. */
   unsigned int x2;
   /** DS-dependent value. */
   unsigned int y2;

   /** Number of triggers. */
   unsigned int count;

   struct DSXY* triggers;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_TRIGGER, };
   S2C_TRIGGER( ) : type( CLSID )
   {

   }
   ~S2C_TRIGGER( )
   {
      delete [] triggers;
   }
#endif
};

/** A parsed self-induced trigger.
*   \sa S2C_TYPE_TRIGGERSELF. */
struct S2C_TRIGGERSELF
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** DS-dependent value. */
   unsigned int x1;
   /** DS-dependent value. */
   unsigned int y1;
   /** DS-dependent value. */
   unsigned int x2;
   /** DS-dependent value. */
   unsigned int y2;

   /** Number of triggers. */
   unsigned int count;

   struct DSXY* triggers;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_TRIGGERSELF, };
   S2C_TRIGGERSELF( ) : type( CLSID )
   {

   }
   ~S2C_TRIGGERSELF( )
   {
      delete [] triggers;
   }
#endif
};

/** A parsed client version request.
*   \sa S2C_TYPE_VERREQ. */
struct S2C_VERREQ
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_VERREQ, };
   S2C_VERREQ( ) : type( CLSID )
   {

   }
   ~S2C_VERREQ( )
   {
   }
#endif
};

/** A parsed ping request.
*   \sa S2C_TYPE_PING. */
struct S2C_PING
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** The value to pong back. */
   int       value;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_PING, };
   S2C_PING( ) : type( CLSID )
   {

   }
   ~S2C_PING( )
   {
   }
#endif
};

/** A parsed whisper denial.
*   \sa S2C_TYPE_WHISPERDENIED. */
struct S2C_WHISPERDENIED
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player short name. */
   char*     player_short;
   /** Player name. */
   char*     player;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_WHISPERDENIED, };
   S2C_WHISPERDENIED( ) : type( CLSID )
   {

   }
   ~S2C_WHISPERDENIED( )
   {
      delete [] player_short;
      delete [] player;
   }
#endif
};

/** A parsed summon/join request echo.
*   \sa S2C_TYPE_SUMMONJOINECHO. */
struct S2C_SUMMONJOINECHO
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Target player short name. */
   char*     player_short;
   /** Target player name. */
   char*     player;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_SUMMONJOINECHO, };
   S2C_SUMMONJOINECHO( ) : type( CLSID )
   {

   }
   ~S2C_SUMMONJOINECHO( )
   {
      delete [] player_short;
      delete [] player;
   }
#endif
};

/** A parsed summon/join request declined.
*   \sa S2C_TYPE_DECLINED. */
struct S2C_DECLINED
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Target player short name. */
   char*     player_short;
   /** Target player name. */
   char*     player;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_DECLINED, };
   S2C_DECLINED( ) : type( CLSID )
   {

   }
   ~S2C_DECLINED( )
   {
      delete [] player_short;
      delete [] player;
   }
#endif
};

/** A parsed summon/join request decline echo.
*   \sa S2C_TYPE_DECLINEECHO. */
struct S2C_DECLINEECHO
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Summoner/joiner's short name. */
   char*     player_short;
   /** Summoner/joiner. */
   char*     player;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_DECLINEECHO, };
   S2C_DECLINEECHO( ) : type( CLSID )
   {

   }
   ~S2C_DECLINEECHO( )
   {
      delete [] player_short;
      delete [] player;
   }
#endif
};

/** A parsed decline echo with no requests.
*   \sa S2C_TYPE_DECLINEFAIL. */
struct S2C_DECLINEFAIL
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_DECLINEFAIL, };
   S2C_DECLINEFAIL( ) : type( CLSID )
   {

   }
   ~S2C_DECLINEFAIL( )
   {

   }
#endif
};

/** A parsed summon request accepted.
*   \sa S2C_TYPE_JOINED. */
struct S2C_JOINED
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Player joining you's short name. */
   const char* joiner_short;
   /** Player joining you. */
   const char* joiner;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_JOINED, };
   S2C_JOINED( ) : type( CLSID )
   {

   }
   ~S2C_JOINED( )
   {
      delete [] joiner_short;
      delete [] joiner;
   }
#endif
};

/** A parsed join request accepted.
*   \sa S2C_TYPE_SUMMONED. */
struct S2C_SUMMONED
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Player summoning you's short name. */
   const char* summoner_short;
   /** Player summoning you. */
   const char* summoner;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_SUMMONED, };
   S2C_SUMMONED( ) : type( CLSID )
   {

   }
   ~S2C_SUMMONED( )
   {
      delete [] summoner_short;
      delete [] summoner;
   }
#endif
};

/** A parsed display message box.
*   \sa S2C_TYPE_MSGBOX. */
struct S2C_MSGBOX
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Unknown portion of message. */
   const char* unk;
   /** Win32 MessageBox() flags. */
   int         flags;
   /** Message. */
   const char* msg;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_MSGBOX, };
   S2C_MSGBOX( ) : type( CLSID )
   {

   }
   ~S2C_MSGBOX( )
   {
      delete [] unk;
      delete [] msg;
   }
#endif
};

/** A parsed avatar show.
*   \sa S2C_TYPE_AVSHOW. */
struct S2C_AVSHOW
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Avatar UID. */
   unsigned int uid;
   /** Position. */
   unsigned int x;
   /** Position. */
   unsigned int y;
   /** Frame. */
   unsigned int frame;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_AVSHOW, };
   S2C_AVSHOW( ) : type( CLSID )
   {

   }
   ~S2C_AVSHOW( )
   {

   }
#endif
};

/** A parsed avatar update colors.
*   \sa S2C_TYPE_AVUPDATECOLORS. */
struct S2C_AVUPDATECOLORS
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Avatar UID. */
   unsigned int uid;
   /** Frame. */
   unsigned int frame;
   /** Colors. */
   const char*  colors;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_AVUPDATECOLORS, };
   S2C_AVUPDATECOLORS( ) : type( CLSID )
   {

   }
   ~S2C_AVUPDATECOLORS( )
   {
      delete [] colors;
   }
#endif
};

/** A parsed hide avatar.
*   \sa S2C_TYPE_AVHIDE. */
struct S2C_AVHIDE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Avatar UID. */
   unsigned int uid;
   /** Position. */
   unsigned int x;
   /** Position. */
   unsigned int y;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_AVHIDE, };
   S2C_AVHIDE( ) : type( CLSID )
   {

   }
   ~S2C_AVHIDE( )
   {

   }
#endif
};

/** A parsed hide avatar.
*   \sa S2C_TYPE_PRINTPLAYERS. */
struct S2C_PRINTPLAYERS
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Whether the player list should be shown. */
   bool        listable;
   /** Name of dream. */
   const char* name;
   /** Caption of dream. */
   const char* caption;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_PRINTPLAYERS, };
   S2C_PRINTPLAYERS( ) : type( CLSID )
   {

   }
   ~S2C_PRINTPLAYERS( )
   {
      delete [] name;
      delete [] caption;
   }
#endif
};

/** A parsed avatar show (2).
*   \sa S2C_TYPE_AVSHOW2 */
struct S2C_AVSHOW2
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Avatar UID. */
   unsigned int uid;
   /** Position. */
   unsigned int x;
   /** Position. */
   unsigned int y;
   /** Frame. */
   unsigned int frame;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_AVSHOW2, };
   S2C_AVSHOW2( ) : type( CLSID )
   {

   }
   ~S2C_AVSHOW2( )
   {

   }
#endif
};

/** A parsed DS emit.
*   \sa S2C_TYPE_DSEMIT */
struct S2C_DSEMIT
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Message. */
   const char* message;

#ifdef BUILD_YIFFURC
   enum { CLSID = S2C_TYPE_DSEMIT, };
   S2C_DSEMIT( ) : type( CLSID )
   {

   }
   ~S2C_DSEMIT( )
   {
      delete [] message;
   }
#endif
};

/** Parses a server -> client line.
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
*  \sa S2C, NET_SESSION, yiffurc_release( ), s2c_parse_peek( ) */
YIFFURC_LINK const struct S2C* YIFFURC_CALL s2c_parse( const char* line, int len,
                                                       struct NET_SESSION* session );

/** Parses a server -> client line without updating session state info.
*  \param[in]     line     The raw command line.
*  \param[in]     len      Length of the command line.
*  \param[in]   session    Session state data.
*  \returns A S2C structure that should
*           be freed with yiffurc_release( ) when no longer
*           needed, or \c 0 on error.
*  This is identical to s2c_parse( ) but \a session is considered const,
*  so lines parsed with this function won't count towards the network state.
*  \sa S2C, NET_SESSION, yiffurc_release( ), s2c_parse( ) */
YIFFURC_LINK const struct S2C* YIFFURC_CALL s2c_parse_peek( const char* line, int len,
                                                            const struct NET_SESSION* session );

/** \addtogroup s2c_encode Server-to-Client Encoding
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
* \{ */

/** Destroys an avatar.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_avdestroy( unsigned int uid );
/** Creates an avatar.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).
*  \sa AVFRAME, av2frame, frame2av */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_avcreate( unsigned int uid, 
                                                        unsigned int x, unsigned int y,
                                                        unsigned int frame,
                                                        const char* name,
                                                        const char* color,
                                                        unsigned int flags );
/** Tweens an avatar to a new position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).
*  \sa AVFRAME, av2frame, frame2av */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_avtween( unsigned int uid,
                                                       unsigned int x_dst, unsigned int y_dst,
                                                       unsigned int end_frame );
/** Update an avatar's colors and frame.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).
*  \sa AVFRAME, av2frame, frame2av */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_avupdatecolors( unsigned int uid, 
                                                              unsigned int frame,
                                                              const char* color );
/** Show an avatar at a position at a frame.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).
*  \sa AVFRAME, av2frame, frame2av */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_avshow( unsigned int uid, 
                                                      unsigned int x, unsigned int y,
                                                      unsigned int frame );

/** Hide an avatar at a position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).
*  \sa AVFRAME, av2frame, frame2av */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_avhide( unsigned int uid, 
                                                      unsigned int x, unsigned int y );

/** Sets badge data for the next chat message.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_badge( unsigned int badge );
/** Sets the camera position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_camset( unsigned int x, unsigned int y );
/** Tweens the camera position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_camtween( unsigned int x_dst, unsigned int y_dst,
                                                        unsigned int x_src, unsigned int y_src );
/** Shows a generic chat message.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_chat( const char* message );
/** Fail on a player create request.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_createfail( const char* error );
/** Sets specitag data for the next chat message.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_specitag( const char* color,
                                                        const char* gsd );
/** Clear a dream name at a position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_dreamclear( unsigned int x, unsigned int y );
/** Create a dream name at a position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_dreampos( unsigned int x, unsigned int y,
                                                        const char* name );
/** Create a dream name and caption at a position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_dreampos2( unsigned int x, unsigned int y,
                                                         const char* name,
                                                         const char* caption );
/** Instruct the client to fetch a dream from the fileserver.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_fetchdream( const char* name, unsigned int id );
/** Instruct the client to fetch a portrait from the fileserver.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_fetchpid( const char* name, unsigned int id );
/** Set the floor at a position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_floorset( unsigned int floor, unsigned int x, unsigned int y );
/** Set the floors at several positions.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_floorsets( unsigned int count, const struct IDXY* floors );
/** Set the object at the player's feet.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_footobj( unsigned int obj );
/** Sets the object in the player's inventory.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_invobj( unsigned int obj );
/** Reply to a machine ID with a modified, scrambled machine ID.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_macid( const char* mac_id );
/** Display a line of the MOTD.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_motd( const char* motd );
/** Set the object at a position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_objset( unsigned int obj, unsigned int x, unsigned int y );
/** Set the objects at several positions.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_objsets( unsigned int count, const struct IDXY* objects );
/** Reply to an online check.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).
*/
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_onlineresult( const char* player, bool online );
/** Play a song.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_playsong( unsigned int song );
/** Play a sound.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_playsound( unsigned int sound );
/** Set the current portrait.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_portrait( const char* player,
                                                        const char color[ 10 ],
                                                        const char species[ 3 ] );
/** Change the client's internal color code.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_synccolorcode( const char* color,
                                                             const char* species );
/** Trigger DS lines.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_trigger( unsigned int x1, unsigned int y1,
                                                       unsigned int x2, unsigned int y2,
                                                       unsigned int count,
                                                       const struct DSXY* lines );
/** Self-trigger DS lines.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_triggerself( unsigned int x1, unsigned int y1,
                                                           unsigned int x2, unsigned int y2,
                                                           unsigned int count,
                                                           const struct DSXY* lines );
/** Tell the client to use a specific background file.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_usebgfile( const char* file );
/** Load a dream by name and id.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_usedream( const char* name,
                                                        unsigned int id );
/** Tell the client to use a specific map file.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_usemapfile( const char* file );
/** Set a contiguous range of DS variables.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_varset( unsigned int first, unsigned int count,
                                                      int* values, int stride );
/** Display a messagebox.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_msgbox( int msgbox_flags,
                                                      const char* message );

/* Stock encodes. */

/** Close the display list (render buffered artifacts).
*  \returns Stock encoded command string.
*/
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_displaylistclose( );
/** Open the display list (buffer artifacts).
*  \returns Stock encoded command string.
*/
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_displaylistopen( );
/** Play welcome.wav.
*  \returns Stock encoded command string.
*/
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_dragonroar( );
/** Fail a login attempt.
*  \warning Deprecated. See S2C_TYPE_LOGINFAIL for details.
*  \returns Stock encoded command string.
*/
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_loginfail( );
/** Signal the end of the MOTD.
*  \returns Stock encoded command string.
*/
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_motdend( );
/** Respond with a generic create/login success code.
*  \returns Stock encoded command string.
*/
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_pass( );
/** Specify protocol version.
*  \returns Stock encoded command string.
*/
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_ver( );
/** Request client version.
*  \returns Stock encoded command string.
*/
YIFFURC_LINK const char* YIFFURC_CALL s2c_enc_verreq( );

/** \} s2c_encode */
/** \} s2c */

/** \addtogroup c2s Client-to-Server Commands (and Regular Expressions)
*  \par How to read regular expressions contained in this document.
*     Commands are shown as Perl-compatible regular expressions,
*     with a few added named classes designed for clarity:
*        - \b [:95int1:] A one-digit base-95 number: <tt>[\\x20-\\x7E]</tt>
*        - \b [:95int2:] A two-digit base-95 number: <tt>[\\x20-\\x7E]{2}</tt>
*        - \b [:95int3:] A three-digit base-95 number: <tt>[\\x20-\\x7E]{3}</tt>
*        - \b [:name:]  A username: <tt>[\\x21-\\x3B\=\\x3F-\\x7E]+</tt>
*        - \b [:220int1:] A one-digit base-220 number: <tt>[\\x23-\\xFF]</tt>
*        - \b [:220int2:] A two-digit base-220 number: <tt>[\\x23-\\xFF]{2}</tt>
*        - \b [:220int3:] A three-digit base-220 number: <tt>[\\x23-\\xFF]{3}</tt>
*        - \b [:220int4:] A four-digit base-220 number: <tt>[\\x23-\\xFF]{4}</tt>
*        - \b [:bstr:] A string whose length is specified by a leading base-220 digit: <tt>[:220int1:].*</tt>
*        - \b [:colors:] Color code: <tt>(t|u[:220int1:]{2}|v)[:220int1:]{10,30}</tt>
*        - \b [:gsd:] 3 base-220 digits indicating gender, species, and digo: <tt>[:220int1:]{3}</tt>
*
*     \note All regexes are implicitly sandwiched between ^ and $ anchors.
*  \{ */

/** An unknown client command.
*  wtf am I looking at?
* \sa C2S */
const int C2S_TYPE_UNKNOWN                 = 1024 + 0;

/** A keep-alive command before logging in.
*  \par regex:
*  \code iamhere \endcode
*  Sent during the character creation screen to keep the connection active.
*  \sa C2S_KEEPALIVE */
const int C2S_TYPE_KEEPALIVE              = 1024 + 1;

/** Create a new player account.
*  \par regex:
*  \code create ([:name:]) ([^ ]+) ([^ ]+) N Y \endcode
*  \param   $1    Player name.
*  \param   $2    Password.
*  \param   $3    Email.
*
*  Registers a new account with the server.  Note
*  that the player cannot be used until a valid
*  machine ID is sent on login.
*
*  \sa C2S_CREATEPLAYER, auth */
const int C2S_TYPE_CREATEPLAYER           = 1024 + 2;

/** Log in.
*  \par regex:
*  \code connect ([:name:]) ([^ ]+) (?:PW(.+))? \endcode
*  \param   $1    Player name.
*  \param   $2    Password.
*  \param   $3    Machine ID.
*
*  The machine ID is a scrambled computer-identification
*  string and checksum.  It is not necessary to
*  log in with, except for the first time after
*  a character is created.
*
*  \sa C2S_LOGIN, auth */
const int C2S_TYPE_LOGIN                  = 1024 + 3;

/** Graceful disconnect.
*  \par regex:
*  \code quit \endcode
*  \sa C2S_QUIT */
const int C2S_TYPE_QUIT                   = 1024 + 4;

/** Set colors and species on login.
*  \par regex:
*  \code color ([:colors:])([:gsd:]) \endcode
*  \param $1   New colors.
*  \param $2   New gender-species-digo.
*  \sa C2S_SETCOLOR */
const int C2S_TYPE_SETCOLOR               = 1024 + 5;

/** Set colors and species while playing.
*  \par regex:
*  \code chcol ([:colors:])([:gsd:]) \endcode
*  \param $1   New colors.
*  \param $2   New gender-species-digo.
*  \sa C2S_CHANGECOLOR */
const int C2S_TYPE_CHANGECOLOR            = 1024 + 6;

/** Set the desc on login or while playing. (SS)
*  \par regex:
*  \code desc (.*) \endcode
*  \param $1   Description body.
*  \sa C2S_SETDESC */
const int C2S_TYPE_SETDESC                = 1024 + 7;

/** Change the desc on login or while playing.
*  \par regex:
*  \code chdesc (.*) \endcode
*  \param $1   Description body.
*  \sa C2S_CHANGEDESC */
const int C2S_TYPE_CHANGEDESC             = 1024 + 8;

/** Ping reply.
*  \par regex:
*  \code polon ([0-9]+) \endcode
*  \param $1 ping value to be echoed.
*  \sa C2S_PONG */
const int C2S_TYPE_PONG                   = 1024 + 9;

/** Client Version.
*  \par regex:
*  \code version ([0-9]+).([0-9]+) \endcode
*  \param $1   Major client version.
*  \param $2   Minor client version.
*  \sa C2S_VERSION */
const int C2S_TYPE_VERSION                = 1024 + 10;

/** Windows versions.
*  \par regex:
*  \code Winver ([0-9]+).([0-9]+) b([0-9]+) d([0-9]+).([0-9]+) \endcode
*  \param $1   Major Windows version.
*  \param $2   Minor Windows version.
*  \param $3   Windows build.
*  \param $4   Major DirectX version.
*  \param $5   Minor DirectX version.
*  \sa C2S_WINVER */
const int C2S_TYPE_WINVER                 = 1024 + 11;

/** Dream done downloading.
*  \par regex:
*  \code vascodagama \endcode
*  \sa C2S_DOWNLOADFINISHED */
const int C2S_TYPE_DOWNLOADFINISHED       = 1024 + 12;

/** Player move.
*  \par regex:
*  \code m ([1379]) \endcode
*  \param $1 Movement direction (SW/SE/NE/NW).
*  \sa C2S_MOVE */
const int C2S_TYPE_MOVE                   = 1024 + 13;

/** Player sit.
*  \par regex:
*  \code sit \endcode
*  \sa C2S_SITDOWN */
const int C2S_TYPE_SITDOWN                = 1024 + 14;

/** Player stand.
*  \par regex:
*  \code stand \endcode
*  \sa C2S_STANDUP */
const int C2S_TYPE_STANDUP                = 1024 + 15;

/** Player lie.
*  \par regex:
*  \code liedown \endcode
*  \sa C2S_LIEDOWN */
const int C2S_TYPE_LIEDOWN                = 1024 + 16;

/** Cycle through sit/stand/lie positions.
*  \par regex:
*  \code lie \endcode
*  \sa C2S_SITSTANDLIE */
const int C2S_TYPE_SITSTANDLIE            = 1024 + 17;

/** Player summon.
*  \par regex:
*  \code summon(?: (%)?([:name:]))? \endcode
*  \param $1   Valid if the name is exact.
*  \param $2   Player to summon.
*  \sa C2S_SUMMON */
const int C2S_TYPE_SUMMON                 = 1024 + 18;

/** Player join.
*  \par regex:
*  \code join(?: (%)?([:name:]))? \endcode
*  \param $1   Valid if the name is exact.
*  \param $2   Player to join.
*  \sa C2S_JOIN */
const int C2S_TYPE_JOIN                   = 1024 + 19;

/** Join/Summon request declined.
*  \par regex:
*  \code decline \endcode
*  \sa C2S_DECLINE */
const int C2S_TYPE_DECLINE                = 1024 + 20;

/** Teleport to a (main) map by number.
*  \par regex:
*  \code gomap ([:95int1:]) \endcode
*  \param $1   Map number.
*  \sa C2S_GOMAP */
const int C2S_TYPE_GOMAP                  = 1024 + 21;

/** Return to the previous map.
*  \par regex:
*  \code goback \endcode
*  \sa C2S_GOBACK */
const int C2S_TYPE_GOBACK                 = 1024 + 22;

/** Teleport to Allegria Island.
*  \par regex:
*  \code goalleg \endcode
*  \sa C2S_GOALLEG */
const int C2S_TYPE_GOALLEG                = 1024 + 23;

/** Teleport to the Vinca.
*  \par regex:
*  \code gostart \endcode
*  \sa C2S_GOVINCA */
const int C2S_TYPE_GOVINCA                = 1024 + 24;

/** Teleport to the Welcome map.
*  \par regex:
*  \code wmap \endcode
*  \sa C2S_GOWELCOMEMAP */
const int C2S_TYPE_GOWELCOMEMAP           = 1024 + 25;

/** Normal speech.
*  \par regex:
*  \code "(.+) \endcode
*  \param $1   Message.
*  \sa C2S_SPEECH */
const int C2S_TYPE_SPEECH                 = 1024 + 26;

/** Shout.
*  \par regex:
*  \code -(.+) \endcode
*  \param $1   Message.
*  \sa C2S_SHOUT */
const int C2S_TYPE_SHOUT                  = 1024 + 27;

/** Shout Toggle.
*  \par regex:
*  \code - \endcode
*  \sa C2S_SHOUTTOGGLE */
const int C2S_TYPE_SHOUTTOGGLE            = 1024 + 28;

/** Whisper.
*  \par regex:
*  \code wh (%)?([:name:]) (.+) \endcode
*  \param $1   Valid if the name is exact.
*  \param $2   Recipient.
*  \param $3   Message.
*  \sa C2S_WHISPER */
const int C2S_TYPE_WHISPER                = 1024 + 29;

/** Cycle wings.
*  \par regex:
*  \code wings \endcode
*  \sa C2S_WINGS */
const int C2S_TYPE_WINGS                  = 1024 + 30;

/** Toggle dragon.
*  \par regex:
*  \code dragon \endcode
*  \sa C2S_DRAGON */
const int C2S_TYPE_DRAGON                 = 1024 + 31;

/** Toggle phoenix.
*  \par regex:
*  \code phoenix \endcode
*  \sa C2S_PHOENIX */
const int C2S_TYPE_PHOENIX                = 1024 + 32;

/** Cycle through portraits.
*  \par regex:
*  \code portrchng \endcode
*  \sa C2S_PORTRAITCHANGE */
const int C2S_TYPE_PORTRAITCHANGE         = 1024 + 33;

/** Toggle eagle.
*  \par regex:
*  \code eagle \endcode
*  \sa C2S_EAGLE */
const int C2S_TYPE_EAGLE                  = 1024 + 34;

/** Perform dragon breath.
*  \par regex:
*  \code breath \endcode
*  \sa C2S_DRAGONBREATH */
const int C2S_TYPE_DRAGONBREATH           = 1024 + 35;

/** Perform phoenix flame.
*  \par regex:
*  \code flame \endcode
*  \sa C2S_PHOENIXFLAME */
const int C2S_TYPE_PHOENIXFLAME           = 1024 + 36;

/** Look at the player at a position.
*  \par regex:
*  \code l ([:95int2:])([:95int2:]) \endcode
*  \param $1   x-coordinate
*  \param $2   y-coordinate
*  \sa C2S_LOOKPOS */
const int C2S_TYPE_LOOKPOS                = 1024 + 37;

/** Get who list.
*  \par regex:
*  \code who \endcode
*  \sa C2S_WHOINFO */
const int C2S_TYPE_WHOINFO                = 1024 + 38;

/** Pick up item at feet.
*  \par regex:
*  \code get \endcode
*  \sa C2S_PICKUP */
const int C2S_TYPE_PICKUP                 = 1024 + 39;

/** Use inventory item.
*  \par regex:
*  \code use \endcode
*  \sa C2S_USE */
const int C2S_TYPE_USE                    = 1024 + 40;

/** Follow Dream Link.
*  \par regex:
*  \code fdl (furc://.+) \endcode
*  \sa C2S_FDL */
const int C2S_TYPE_FDL                    = 1024 + 41;

/** Look at a player by name.
*  \par regex:
*  \code "(?:l|look) (%)?([:name:]) \endcode
*  \param $1   Valid if the name is exact.
*  \param $2   Subject.
*  \sa C2S_LOOKPLAYER */
const int C2S_TYPE_LOOKPLAYER             = 1024 + 42;

/** Toggle gryffe.
*  \par regex:
*  \code gryffe \endcode
*  \sa C2S_GRYFFE */
const int C2S_TYPE_GRYFFE                 = 1024 + 44;

/** Online check.
*  \par regex:
*  \code onln ([:name:]) \endcode
*  \param $1   Player whose online status to check.
*  \sa C2S_ONLINECHECK */
const int C2S_TYPE_ONLINECHECK            = 1024 + 45;

/** Emote.
*  \par regex:
*  \code :(.+) \endcode
*  \param $1   Emote text.
*  \sa C2S_EMOTE */
const int C2S_TYPE_EMOTE                  = 1024 + 46;

/** Rotate left.
*  \par regex:
*  \code < \endcode
*  \sa C2S_TURNLEFT */
const int C2S_TYPE_TURNLEFT               = 1024 + 47;

/** Rotate right.
*  \par regex:
*  \code > \endcode
*  \sa C2S_TURNRIGHT */
const int C2S_TYPE_TURNRIGHT              = 1024 + 48;

/** Base parsed server protocol structure. */
struct C2S
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
};

/** A parsed keep-alive.
*  \sa C2S_TYPE_KEEPALIVE. */
struct C2S_KEEPALIVE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_KEEPALIVE, };
   C2S_KEEPALIVE( ) : type( CLSID )
   {

   }
   ~C2S_KEEPALIVE( )
   {
   }
#endif
};

/** A parsed player create.
*  \sa C2S_TYPE_CREATEPLAYER. */
struct C2S_CREATEPLAYER
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player name */
   char*     player;
   /** Player password. */
   char*     password;
   /** Player email. */
   char*     email;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_CREATEPLAYER, };
   C2S_CREATEPLAYER( ) : type( CLSID )
   {

   }
   ~C2S_CREATEPLAYER( )
   {
      delete [] player;
      delete [] password;
      delete [] email;
   }
#endif
};

/** A parsed login attempt.
*  \sa C2S_TYPE_LOGIN. */
struct C2S_LOGIN
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player name */
   char*     player;
   /** Player password. */
   char*     password;
   /** Scrambled machine ID (optional). */
   char*     mac_id;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_LOGIN, };
   C2S_LOGIN( ) : type( CLSID )
   {

   }
   ~C2S_LOGIN( )
   {
      delete [] player;
      delete [] password;
      delete [] mac_id;
   }
#endif
};

/** A parsed graceful disconnect.
*  \sa C2S_TYPE_QUIT. */
struct C2S_QUIT
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_QUIT, };
   C2S_QUIT( ) : type( CLSID )
   {

   }
   ~C2S_QUIT( )
   {
   }
#endif
};

/** A parsed color set.
*  \sa C2S_TYPE_SETCOLOR. */
struct C2S_SETCOLOR
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int     type;
   /** Colors */
   char*         colors;
   /** GSD. */
   char          gsd[ 3 ];

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_SETCOLOR, };
   C2S_SETCOLOR( ) : type( CLSID )
   {

   }
   ~C2S_SETCOLOR( )
   {
      delete [] colors;
   }
#endif
};

/** A parsed color change.
*  \sa C2S_TYPE_CHANGECOLOR. */
struct C2S_CHANGECOLOR
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int     type;
   /** Colors */
   char*         colors;
   /** GSD. */
   char          gsd[ 3 ];

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_CHANGECOLOR, };
   C2S_CHANGECOLOR( ) : type( CLSID )
   {

   }
   ~C2S_CHANGECOLOR( )
   {
      delete [] colors;
   }
#endif
};

/** A parsed set description.
*  \sa C2S_TYPE_SETDESC. */
struct C2S_SETDESC
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Description. */
   char*     desc;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_SETDESC, };
   C2S_SETDESC( ) : type( CLSID )
   {

   }
   ~C2S_SETDESC( )
   {
      delete [] desc;
   }
#endif
};

/** A parsed change description.
*  \sa C2S_TYPE_CHANGEDESC. */
struct C2S_CHANGEDESC
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Description. */
   char*     desc;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_CHANGEDESC, };
   C2S_CHANGEDESC( ) : type( CLSID )
   {

   }
   ~C2S_CHANGEDESC( )
   {
      delete [] desc;
   }
#endif
};

/** A parsed ping reply.
*  \sa C2S_TYPE_PONG. */
struct C2S_PONG
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Echoed ping value. */
   int       value;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_PONG, };
   C2S_PONG( ) : type( CLSID )
   {

   }
   ~C2S_PONG( )
   {
   }
#endif
};

/** A parsed client version.
*  \sa C2S_TYPE_VERSION. */
struct C2S_VERSION
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int         type;
   /** Client version. */
   struct MAJORMINOR client;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_VERSION, };
   C2S_VERSION( ) : type( CLSID )
   {

   }
   ~C2S_VERSION( )
   {
   }
#endif
};

/** A parsed windows version.
*  \sa C2S_TYPE_WINVER. */
struct C2S_WINVER
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int         type;
   /** Windows version. */
   struct MAJORMINOR windows;
   /** Windows build. */
   unsigned int      build;
   /** DirectX version. */
   struct MAJORMINOR directx;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_WINVER, };
   C2S_WINVER( ) : type( CLSID )
   {

   }
   ~C2S_WINVER( )
   {
   }
#endif
};

/** A parsed dream done downloading
*  \sa C2S_TYPE_DOWNLOADFINISHED. */
struct C2S_DOWNLOADFINISHED
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_DOWNLOADFINISHED, };
   C2S_DOWNLOADFINISHED( ) : type( CLSID )
   {

   }
   ~C2S_DOWNLOADFINISHED( )
   {
   }
#endif
};

/** A parsed player move.
*  \sa C2S_TYPE_MOVE. */
struct C2S_MOVE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int     type;
   /** Direction. */
   enum Move_Dir direction;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_MOVE, };
   C2S_MOVE( ) : type( CLSID )
   {

   }
   ~C2S_MOVE( )
   {
   }
#endif
};

/** A parsed player sit.
*  \sa C2S_TYPE_SITDOWN. */
struct C2S_SITDOWN
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_SITDOWN, };
   C2S_SITDOWN( ) : type( CLSID )
   {

   }
   ~C2S_SITDOWN( )
   {
   }
#endif
};

/** A parsed player stand.
*  \sa C2S_TYPE_STANDUP. */
struct C2S_STANDUP
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_STANDUP, };
   C2S_STANDUP( ) : type( CLSID )
   {

   }
   ~C2S_STANDUP( )
   {
   }
#endif
};

/** A parsed player lie.
*  \sa C2S_TYPE_LIEDOWN. */
struct C2S_LIEDOWN
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_LIEDOWN, };
   C2S_LIEDOWN( ) : type( CLSID )
   {

   }
   ~C2S_LIEDOWN( )
   {
   }
#endif
};

/** A parsed player sit/stand/lie cycle.
*  \sa C2S_TYPE_SITSTANDLIE. */
struct C2S_SITSTANDLIE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_SITSTANDLIE, };
   C2S_SITSTANDLIE( ) : type( CLSID )
   {

   }
   ~C2S_SITSTANDLIE( )
   {
   }
#endif
};

/** A parsed summon.
*  \sa C2S_TYPE_SUMMON. */
struct C2S_SUMMON
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player to summon. */
   char*     player;
   /** \c true if \a player is an exact name. */
   bool      exact;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_SUMMON, };
   C2S_SUMMON( ) : type( CLSID )
   {

   }
   ~C2S_SUMMON( )
   {
      delete [] player;
   }
#endif
};

/** A parsed join.
*  \sa C2S_TYPE_JOIN. */
struct C2S_JOIN
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player to join. */
   char*     player;
   /** \c true if \a player is an exact name. */
   bool      exact;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_JOIN, };
   C2S_JOIN( ) : type( CLSID )
   {

   }
   ~C2S_JOIN( )
   {
      delete [] player;
   }
#endif
};

/** A parsed summon/join decline.
*  \sa C2S_TYPE_DECLINE. */
struct C2S_DECLINE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_DECLINE, };
   C2S_DECLINE( ) : type( CLSID )
   {

   }
   ~C2S_DECLINE( )
   {
   }
#endif
};

/** A parsed gomap.
*  \sa C2S_TYPE_GOMAP */
struct C2S_GOMAP
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Map number. */
   unsigned int map_num;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_GOMAP, };
   C2S_GOMAP( ) : type( CLSID )
   {

   }
   ~C2S_GOMAP( )
   {
   }
#endif
};

/** A parsed goback.
*  \sa C2S_TYPE_GOBACK */
struct C2S_GOBACK
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_GOBACK, };
   C2S_GOBACK( ) : type( CLSID )
   {

   }
   ~C2S_GOBACK( )
   {
   }
#endif
};

/** A parsed allegria island teleport.
*  \sa C2S_TYPE_GOALLEG */
struct C2S_GOALLEG
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_GOALLEG, };
   C2S_GOALLEG( ) : type( CLSID )
   {

   }
   ~C2S_GOALLEG( )
   {
   }
#endif
};

/** A parsed vinca teleport.
*  \sa C2S_TYPE_GOVINCA */
struct C2S_GOVINCA
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_GOVINCA, };
   C2S_GOVINCA( ) : type( CLSID )
   {

   }
   ~C2S_GOVINCA( )
   {
   }
#endif
};

/** A parsed welcome map teleport.
*  \sa C2S_TYPE_GOWELCOMEMAP */
struct C2S_GOWELCOMEMAP
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_GOWELCOMEMAP, };
   C2S_GOWELCOMEMAP( ) : type( CLSID )
   {

   }
   ~C2S_GOWELCOMEMAP( )
   {
   }
#endif
};

/** A parsed speech.
*  \sa C2S_TYPE_SPEECH */
struct C2S_SPEECH
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Message. */
   char*     message;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_SPEECH, };
   C2S_SPEECH( ) : type( CLSID )
   {

   }
   ~C2S_SPEECH( )
   {
      delete [] message;
   }
#endif
};

/** A parsed shout.
*  \sa C2S_TYPE_SHOUT */
struct C2S_SHOUT
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Message. */
   char*     message;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_SHOUT, };
   C2S_SHOUT( ) : type( CLSID )
   {

   }
   ~C2S_SHOUT( )
   {
      delete [] message;
   }
#endif
};

/** A parsed shout toggle.
*  \sa C2S_TYPE_SHOUTTOGGLE */
struct C2S_SHOUTTOGGLE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_SHOUTTOGGLE, };
   C2S_SHOUTTOGGLE( ) : type( CLSID )
   {

   }
   ~C2S_SHOUTTOGGLE( )
   {
   }
#endif
};

/** A parsed whisper.
*  \sa C2S_TYPE_WHISPER */
struct C2S_WHISPER
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player. */
   char*     player;
   /** Message. */
   char*     message;
   /** \c true if \a player is an exact name. */
   bool      exact;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_WHISPER, };
   C2S_WHISPER( ) : type( CLSID )
   {

   }
   ~C2S_WHISPER( )
   {
      delete [] player;
      delete [] message;
   }
#endif
};

/** A parsed cycle wings.
*  \sa C2S_TYPE_WINGS */
struct C2S_WINGS
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_WINGS, };
   C2S_WINGS( ) : type( CLSID )
   {

   }
   ~C2S_WINGS( )
   {
   }
#endif
};

/** A parsed toggle dragon.
*  \sa C2S_TYPE_WINGS */
struct C2S_DRAGON
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_DRAGON, };
   C2S_DRAGON( ) : type( CLSID )
   {

   }
   ~C2S_DRAGON( )
   {
   }
#endif
};

/** A parsed toggle phoenix.
*  \sa C2S_TYPE_PHOENIX */
struct C2S_PHOENIX
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_PHOENIX, };
   C2S_PHOENIX( ) : type( CLSID )
   {

   }
   ~C2S_PHOENIX( )
   {
   }
#endif
};

/** A parsed cycle portraits.
*  \sa C2S_TYPE_PORTRAITCHANGE */
struct C2S_PORTRAITCHANGE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_PORTRAITCHANGE, };
   C2S_PORTRAITCHANGE( ) : type( CLSID )
   {

   }
   ~C2S_PORTRAITCHANGE( )
   {
   }
#endif
};

/** A parsed eagle toggle.
*  \sa C2S_TYPE_EAGLE */
struct C2S_EAGLE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_EAGLE, };
   C2S_EAGLE( ) : type( CLSID )
   {

   }
   ~C2S_EAGLE( )
   {
   }
#endif
};

/** A parsed gryffe toggle.
*  \sa C2S_TYPE_GRYFFE */
struct C2S_GRYFFE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_GRYFFE, };
   C2S_GRYFFE( ) : type( CLSID )
   {

   }
   ~C2S_GRYFFE( )
   {
   }
#endif
};

/** A parsed dragon breath.
*  \sa C2S_TYPE_DRAGONBREATH */
struct C2S_DRAGONBREATH
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_DRAGONBREATH, };
   C2S_DRAGONBREATH( ) : type( CLSID )
   {

   }
   ~C2S_DRAGONBREATH( )
   {
   }
#endif
};

/** A parsed cycle wings.
*  \sa C2S_TYPE_PHOENIXFLAME */
struct C2S_PHOENIXFLAME
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_PHOENIXFLAME, };
   C2S_PHOENIXFLAME( ) : type( CLSID )
   {

   }
   ~C2S_PHOENIXFLAME( )
   {
   }
#endif
};

/** A parsed lookat by position.
*  \sa C2S_TYPE_LOOKPOS */
struct C2S_LOOKPOS
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Look position. */
   unsigned int x;
   /** Look position. */
   unsigned int y;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_LOOKPOS, };
   C2S_LOOKPOS( ) : type( CLSID )
   {

   }
   ~C2S_LOOKPOS( )
   {
   }
#endif
};

/** A parsed who info request.
*  \sa C2S_TYPE_WHOINFO */
struct C2S_WHOINFO
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_WHOINFO, };
   C2S_WHOINFO( ) : type( CLSID )
   {

   }
   ~C2S_WHOINFO( )
   {
   }
#endif
};

/** A parsed pickup object.
*  \sa C2S_TYPE_PICKUP */
struct C2S_PICKUP
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_PICKUP, };
   C2S_PICKUP( ) : type( CLSID )
   {

   }
   ~C2S_PICKUP( )
   {
   }
#endif
};

/** A parsed use object.
*  \sa C2S_TYPE_USE */
struct C2S_USE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_USE, };
   C2S_USE( ) : type( CLSID )
   {

   }
   ~C2S_USE( )
   {
   }
#endif
};

/** A parsed follow dream link.
*  \sa C2S_TYPE_FDL */
struct C2S_FDL
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Dream URL */
   char*     url;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_FDL, };
   C2S_FDL( ) : type( CLSID )
   {

   }
   ~C2S_FDL( )
   {
      delete [] url;
   }
#endif
};

/** A parsed lookat by name.
*  \sa C2S_TYPE_LOOKPLAYER */
struct C2S_LOOKPLAYER
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player. */
   char*     player;
   /** \c true if \a player is an exact name. */
   bool      exact;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_LOOKPLAYER, };
   C2S_LOOKPLAYER( ) : type( CLSID )
   {

   }
   ~C2S_LOOKPLAYER( )
   {
      delete [] player;
   }
#endif
};

/** A parsed online check.
*  \sa C2S_TYPE_ONLINECHECK */
struct C2S_ONLINECHECK
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Player. */
   char*     player;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_ONLINECHECK, };
   C2S_ONLINECHECK( ) : type( CLSID )
   {

   }
   ~C2S_ONLINECHECK( )
   {
      delete [] player;
   }
#endif
};

/** A parsed emote.
*  \sa C2S_TYPE_EMOTE */
struct C2S_EMOTE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** Emote message. */
   char*     message;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_EMOTE };
   C2S_EMOTE( ) : type( CLSID )
   {

   }
   ~C2S_EMOTE( )
   {
      delete [] message;
   }
#endif
};

/** A parsed rotate left.
*  \sa C2S_TYPE_TURNLEFT */
struct C2S_TURNLEFT
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_TURNLEFT };
   C2S_TURNLEFT( ) : type( CLSID )
   {

   }
   ~C2S_TURNLEFT( )
   {

   }
#endif
};

/** A parsed rotate right.
*  \sa C2S_TYPE_TURNRIGHT */
struct C2S_TURNRIGHT
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;

#ifdef BUILD_YIFFURC
   enum { CLSID = C2S_TYPE_TURNRIGHT };
   C2S_TURNRIGHT( ) : type( CLSID )
   {

   }
   ~C2S_TURNRIGHT( )
   {

   }
#endif
};

/** Parses a client -> server line.
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
*/
YIFFURC_LINK const struct C2S* YIFFURC_CALL c2s_parse( const char* line, int len,
                                                       struct NET_SESSION* session );

/** Parses a client -> server line without updating session state info.
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
*/
YIFFURC_LINK const struct C2S* YIFFURC_CALL c2s_parse_peek( const char* line, int len,
                                                            const struct NET_SESSION* session );

/** \addtogroup c2s_encode Client-to-Server Encoding
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
*  \{ */

/** Registers a player account.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_create( const char* player, const char* pass,
                                                      const char* email );
/** Logs in with a player account and optional machine ID.
*  \a macid should already be scrambled.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ).
*  \sa auth */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_login( const char* player, const char* pass,
                                                     const char* macid );
/** Sends out regular speech.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_speech( const char* message );
/** Sends out a shout.
*  A null \a shout parameter will toggle shouts on and off.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_shout( const char* shout );
/** Sends out a whisper.
*  The \a to parameter will match any player name beginning
*  the same way.  You can prepend \a to with a % character to make
*  the name exact.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_whisper( const char* to, const char* message );
/** Sends out an emote.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_emote( const char* action );
/** Follows a furc (dream) url.
*  Furc urls have the form <tt>furc://dreamname:caption</tt>
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_fdl( const char* furl );
/** Teleport to a server map.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_gomap( unsigned int map_num );
/** Join a player.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_join( const char* player );
/** Summon a player.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_summon( const char* player );
/** Look at the player at a specific position.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_lookpos( unsigned int x, unsigned int y );
/** Check if a player is online or not.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_onlinecheck( const char* player );
/** Change your player colors (SS).
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_changecolor( const char* color,
                                                           const char* gsd );
/** Set your player colors after login.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_setcolor( const char* color,
                                                        const char* gsd );
/** Change your player description.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_changedesc( const char* desc );
/** Set your player desc.
*  Essentially a synonym for c2s_enc_changedesc( ).
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_setdesc( const char* desc );
/** Reply to a ping (]marco) from the server.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_pong( int num );
/** Tell the server your client version.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_version( struct MAJORMINOR client );
/** Tell the server your windows version, build, and directx version.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_winver( struct MAJORMINOR windows, unsigned int build,
                                                      struct MAJORMINOR directx );
/** Register a dream you uploaded.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_regdream( unsigned int dream_id );
/** Send out a dream emit.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_emit( const char* message );
/** Send out a dream emit to the whole map.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_emitloud( const char* message );
/** Eject a player from the dream.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_eject( const char* player );
/** Share dream control with a player.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_share( const char* player );
/** Unshare dream control with a player.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_unshare( const char* player );
/** Give a player an active cookie with optional message.
*  You can prepend \a player with a % character to make the name exact.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_givecookie( const char* player,
                                                          const char* message );
/** Give a player an inactive cookie with optional message.
*  You can prepend \a player with a % character to make the name exact.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_makecookie( const char* player,
                                                          const char* message );
/** Looks at a player by name.
*  You can prepend \a player with a % character to make the name exact.
*  \returns Encoded command string that should be freed
*           with yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_lookplayer( const char* player );

/* Stock encodes. */

/** Begin accepting cookies.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_cookiesaccept( );
/** Begin rejecting cookies.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_cookiesreject( );
/** Eat an active cookie.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_eatcookie( );
/** Eat an active cookie loudly.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_munchcookie( );
/** Toggle hearing shouts.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_shouttoggle( );
/** Decline a summon or join request.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_decline( );
/** Pick up the object at your feet.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_pickup( );
/** Go to allegria island.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_goalleg( );
/** Return to the last map visited.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_goback( );
/** Go to the vinca.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_govinca( );
/** Get player info.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_info( );
/** Tell the server to ignore non-online-check commands.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_onlineprx( );
/** Use the object in your inventory.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_use( );
/** Inform the server you're done downloading the dream.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_downloadfinished( );
/** Get server process info.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_whichinfo( );
/** Get who info.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_whoinfo( );
/** Make dragon breath.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_dragonbreath( );
/** Turn into an eagle.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_gryffeeagle( );
/** Make phoenix flames.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_phoenixflame( );
/** Cycle through portraits.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_portraitchange( );
/** Cycle through wings.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_wings( );
/** Rotate the player left.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_turnleft( );
/** Rotate the player right.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_turnright( );
/** Cycle through sitting, standing, and lying positions.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_sitstandlie( );
/** Lay the player down.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_liedown( );
/** Move one space in a direction.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_move( enum Move_Dir direction );
/** Sit down.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_sitdown( );
/** Stand up.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_standup( );
/** Get the furc (dream) url for the current dream.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_showdreamurl( );
/** Tell the server to keep your connection alive before logging in.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_keepalive( );
/** Graceful disconnect.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_quit( );
/** Request to upload a dream.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_uploadrequest( );
/** Get the current server time (FST).
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_time( );
/** Toggle on whispers. (SS)
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_whisperson( );
/** Toggle off whispers. (SS)
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_whispersoff( );
/** Enable cookies for the current dream.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_cookieson( );
/** Disable cookies for the current dream.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_cookiesoff( );
/** Allow dreampad uploads to everyone in the dream.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_dreampadsall( );
/** Allow dreampad uploads to only the owner in the dream.
*  \returns Stock encoded command string.
*/
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_dreampadsowner( );
/** Allow dreampad uploads to only those with shared control in the dream.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_dreampadsshared( );
/** Toggle making summoned furres appear at start point in the dream.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_shieldsummon( );
/** Toggle letting players access the dream through dream urls.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_shieldurl( );
/** Unload an uploaded dream.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_unloaddream( );
/** Unload all uploaded dreams.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_unloadalldreams( );
/** Allow everyone to upload dreams anywhere.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_dreamuploadsall( );
/** Allow only the owner to upload dreams anywhere.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_dreamuploadsowner( );
/** Allow only those with share control to upload dreams anywhere.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_dreamuploadsshared( );
/** Allow only silver sponsors to upload dreams anywhere.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_dreamuploadssponsors( );
/** Join the beekin channel.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_joinbeekchannel( );
/** Leave the beekin channel. (B)
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_leavebeekchannel( );
/** List available beeking commands. (B)
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_listbeekcommands( );
/** List help requests and on-duty beekins. (B)
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_listf( );
/** Join the guardian channel. (B)
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_joinguardchannel( );
/** Show the number of on-duty beekins. (B)
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_ondutyinfo( );
/** List outstanding help requests. (B)
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_listrequests( );
/** Go to the welcome map.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_gowelcomemap( );
/** Toggle dragon form.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_dragon( );
/** Toggle phoenix form.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_phoenix( );
/** Cycles portraits.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_portraits( );
/** Perform phoenix flames.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL c2s_enc_phoenixflames( );

/** \} c2s_encode */
/** \} c2s */

/** \addtogroup xfer File Transfer Connections
* \{ */

/** Transfer network session information.
* Holds state information used in file transfer parsing.
* \sa xfer_s2c_parse */
struct XFER_NET_SESSION
{
   /** Network Level.
   *  \sa Xfer_Net_Level */
   enum Xfer_Net_Level level;

   /** State information for Xfer server -> client parsing. */
   struct
   {
      /** Number of lines seen. */
      unsigned int line_count;
      /** Number of chunks left to receive when downloading. */
      unsigned int chunks_left;
      /** Reserved. */
      unsigned int _r0;
      /** Reserved. */
      unsigned int _r1;
   }              s2c_state;
};

/** \addtogroup xfer_s2c Server-to-Client Commands (and Regular Expressions)
*  \par How to read regular expressions contained in this document.
*     Commands are shown as Perl-compatible regular expressions,
*     with a few added named classes designed for clarity:
*        - \b [:95int1:] A one-digit base-95 number: <tt>[\\x20-\\x7E]</tt>
*        - \b [:95int2:] A two-digit base-95 number: <tt>[\\x20-\\x7E]{2}</tt>
*        - \b [:95int3:] A three-digit base-95 number: <tt>[\\x20-\\x7E]{3}</tt>
*        - \b [:name:]  A username: <tt>[\\x21-\\x3B\=\\x3F-\\x7E]+</tt>
*        - \b [:220int1:] A one-digit base-220 number: <tt>[\\x23-\\xFF]</tt>
*        - \b [:220int2:] A two-digit base-220 number: <tt>[\\x23-\\xFF]{2}</tt>
*        - \b [:220int3:] A three-digit base-220 number: <tt>[\\x23-\\xFF]{3}</tt>
*        - \b [:220int4:] A four-digit base-220 number: <tt>[\\x23-\\xFF]{4}</tt>
*        - \b [:bstr:] A string whose length is specified by a leading base-220 digit: <tt>[:220int1:].*</tt>
*        - \b [:colors:] Color code: <tt>(t|u[:220int1:]{2}|v)[:220int1:]{10,30}</tt>
*        - \b [:gsd:] 3 base-220 digits indicating gender, species, and digo: <tt>[:220int1:]{3}</tt>
*
*     \note All regexes are implicitly sandwiched between ^ and $ anchors.
*  \{ */
/** An unknown file server command.
*  wtf am I looking at?
* \sa XFER_S2C */
const int XFER_S2C_TYPE_UNKNOWN = 2048 + 0;

/** File server greeting.
*  \par regex:
*  \code 10(?: (.+))? \endcode
*  \param   $1    Human-readable message.
*  \sa XFER_S2C_GREETING */
const int XFER_S2C_TYPE_GREETING = 2048 + 1;

/** User-name ACK.
*  \par regex:
*  \code 20(?: (.+))? \endcode
*  \param   $1    Human-readable message.
*  \sa XFER_S2C_USEROK */
const int XFER_S2C_TYPE_USEROK = 2048 + 2;

/** Logged in, proceed with actions.
*  \par regex:
*  \code 30(?: (.+))? \endcode
*  \param   $1    Human-readable message.
*  \sa XFER_S2C_LOGGEDIN */
const int XFER_S2C_TYPE_LOGGEDIN = 2048 + 3;

/** SETR response.
*  \par regex:
*  \code 90(?: (.+))? \endcode
*  \param   $1    Human-readable message.
*  \sa XFER_S2C_UFPSET */
const int XFER_S2C_TYPE_UFPSET = 2048 + 4;

/** Ready to receive upload.
*  \par regex:
*  \code 50 (d+)(?: (.+))? \endcode
*  \param   $1    Unknown.
*  \param   $2    Human-readable message.
*  \sa XFER_S2C_UPLOADOK */
const int XFER_S2C_TYPE_UPLOADOK = 2048 + 5;

/** Upload chunk ACK.
*  \par regex:
*  \code 55 (d+) \endcode
*  \param   $1    Chunk id.
*  \sa XFER_S2C_CHUNKOK */
const int XFER_S2C_TYPE_CHUNKOK = 2048 + 6;

/** All chunks received (stop uploading).
*  \par regex:
*  \code 59(?: (.+))? \endcode
*  \param   $1    Human-readable message.
*  \sa XFER_S2C_UPLOADDONE */
const int XFER_S2C_TYPE_UPLOADDONE = 2048 + 7;

/** Logged out.
*  \par regex:
*  \code 99(?: (.+))? \endcode
*  \param   $1    Human-readable message.
*  \sa XFER_S2C_LOGGEDOUT */
const int XFER_S2C_TYPE_LOGGEDOUT = 2048 + 8;

/** Disconnected for inactivity.
*  \par regex:
*  \code 98(?: (.+))? \endcode
*  \param   $1    Human-readable message.
*  \sa XFER_S2C_INACTIVITY */
const int XFER_S2C_TYPE_INACTIVITY = 2048 + 9;

/** Download information before receiving data.
*  \par regex:
*  \code 44 (.+) (.+) (.+) (.+) \endcode
*  \param   $1    File size.
*  \param   $2    Unknown.
*  \param   $3    Chunk size.
*  \param   $4    Unknown.
*  \sa XFER_S2C_DOWNLOADINFO */
const int XFER_S2C_TYPE_DOWNLOADINFO = 2048 + 10;

/** File not found.
*  \par regex:
*  \code 41(?: (.+))? \endcode
*  \param   $1    Human readable message.
*  \sa XFER_S2C_FILENOTFOUND */
const int XFER_S2C_TYPE_FILENOTFOUND = 2048 + 11;

/** Download chunk header.
*  The characters "SC" followed by two ints.
*  \param   int1    Chunk ID.
*  \param   int2    Chunk CRC32.
*  What follows after this is the chunk data.
*  \sa XFER_S2C_CHUNKHEAD */
const int XFER_S2C_TYPE_CHUNKHEAD = 2048 + 12;

/** Base parsed transfer client protocol structure. */
struct XFER_S2C
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
};

/** A parsed greeting.
*  \sa XFER_S2C_TYPE_GREETING. */
struct XFER_S2C_GREETING
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Human-readable message. */
   const char* message;

#ifdef BUILD_YIFFURC
   enum { CLSID = XFER_S2C_TYPE_GREETING, };
   XFER_S2C_GREETING( ) : type( CLSID )
   {

   }
   ~XFER_S2C_GREETING( )
   {
      delete [] message;
   }
#endif
};

/** A parsed user-name ACK.
*  \sa XFER_S2C_TYPE_USEROK. */
struct XFER_S2C_USEROK
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Human-readable message. */
   const char* message;

#ifdef BUILD_YIFFURC
   enum { CLSID = XFER_S2C_TYPE_USEROK, };
   XFER_S2C_USEROK( ) : type( CLSID )
   {

   }
   ~XFER_S2C_USEROK( )
   {
      delete [] message;
   }
#endif
};

/** A parsed logged in.
*  \sa XFER_S2C_TYPE_LOGGEDIN. */
struct XFER_S2C_LOGGEDIN
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Human-readable message. */
   const char* message;

#ifdef BUILD_YIFFURC
   enum { CLSID = XFER_S2C_TYPE_LOGGEDIN, };
   XFER_S2C_LOGGEDIN( ) : type( CLSID )
   {

   }
   ~XFER_S2C_LOGGEDIN( )
   {
      delete [] message;
   }
#endif
};

/** A parsed SETR response.
*  \sa XFER_S2C_TYPE_UFPSET. */
struct XFER_S2C_UFPSET
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Human-readable message. */
   const char* message;

#ifdef BUILD_YIFFURC
   enum { CLSID = XFER_S2C_TYPE_UFPSET, };
   XFER_S2C_UFPSET( ) : type( CLSID )
   {

   }
   ~XFER_S2C_UFPSET( )
   {
      delete [] message;
   }
#endif
};

/** A parsed ready to upload.
*  \sa XFER_S2C_TYPE_UPLOADOK. */
struct XFER_S2C_UPLOADOK
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Unknown. */
   int         unk1;
   /** Human-readable message. */
   const char* message;

#ifdef BUILD_YIFFURC
   enum { CLSID = XFER_S2C_TYPE_UPLOADOK, };
   XFER_S2C_UPLOADOK( ) : type( CLSID )
   {

   }
   ~XFER_S2C_UPLOADOK( )
   {
      delete [] message;
   }
#endif
};

/** A parsed upload chunk ACK.
*  \sa XFER_S2C_TYPE_CHUNKOK. */
struct XFER_S2C_CHUNKOK
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Chunks id. */
   unsigned int chunk_id;

#ifdef BUILD_YIFFURC
   enum { CLSID = XFER_S2C_TYPE_CHUNKOK, };
   XFER_S2C_CHUNKOK( ) : type( CLSID )
   {

   }
   ~XFER_S2C_CHUNKOK( )
   {
   }
#endif
};

/** A parsed upload finished.
*  \sa XFER_S2C_TYPE_UPLOADDONE. */
struct XFER_S2C_UPLOADDONE
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Human-readable message. */
   const char* message;

#ifdef BUILD_YIFFURC
   enum { CLSID = XFER_S2C_TYPE_UPLOADDONE, };
   XFER_S2C_UPLOADDONE( ) : type( CLSID )
   {

   }
   ~XFER_S2C_UPLOADDONE( )
   {
      delete [] message;
   }
#endif
};

/** A parsed logged out.
*  \sa XFER_S2C_TYPE_LOGGEDOUT. */
struct XFER_S2C_LOGGEDOUT
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Human-readable message. */
   const char* message;

#ifdef BUILD_YIFFURC
   enum { CLSID = XFER_S2C_TYPE_LOGGEDOUT, };
   XFER_S2C_LOGGEDOUT( ) : type( CLSID )
   {

   }
   ~XFER_S2C_LOGGEDOUT( )
   {
      delete [] message;
   }
#endif
};

/** A parsed ready to upload.
*  \sa XFER_S2C_TYPE_DOWNLOADINFO. */
struct XFER_S2C_DOWNLOADINFO
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int type;
   /** File size. */
   int   file_size;
   /** Unknown. */
   int      unk1;
   /** Chunk size. */
   int   chunk_size;
   /** Unknown. */
   int      unk2;

#ifdef BUILD_YIFFURC
   enum { CLSID = XFER_S2C_TYPE_DOWNLOADINFO, };
   XFER_S2C_DOWNLOADINFO( ) : type( CLSID )
   {

   }
   ~XFER_S2C_DOWNLOADINFO( )
   {
   }
#endif
};

/** A parsed chunk header.
*  \sa XFER_S2C_TYPE_CHUNKHEAD. */
struct XFER_S2C_CHUNKHEAD
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int    type;
   /** Chunk ID. */
   unsigned int chunk_id;
   /** Chunk CRC32. */
   unsigned int chunk_crc32;

#ifdef BUILD_YIFFURC
   enum { CLSID = XFER_S2C_TYPE_CHUNKHEAD, };
   XFER_S2C_CHUNKHEAD( ) : type( CLSID )
   {

   }
   ~XFER_S2C_CHUNKHEAD( )
   {
   }
#endif
};

/** A parsed inactivity disconnect.
*  \sa XFER_S2C_TYPE_INACTIVITY. */
struct XFER_S2C_INACTIVITY
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Humean-readable message. */
   const char* message;

#ifdef BUILD_YIFFURC
   enum { CLSID = XFER_S2C_TYPE_INACTIVITY, };
   XFER_S2C_INACTIVITY( ) : type( CLSID )
   {

   }
   ~XFER_S2C_INACTIVITY( )
   {
      delete [] message;
   }
#endif
};

/** A parsed file not found.
*  \sa XFER_S2C_TYPE_FILENOTFOUND. */
struct XFER_S2C_FILENOTFOUND
{
   /** The protocol type id.
   *  The value of this member reveals the true
   *  protocol structure. */
   const int   type;
   /** Humean-readable message. */
   const char* message;

#ifdef BUILD_YIFFURC
   enum { CLSID = XFER_S2C_TYPE_FILENOTFOUND, };
   XFER_S2C_FILENOTFOUND( ) : type( CLSID )
   {

   }
   ~XFER_S2C_FILENOTFOUND( )
   {
      delete [] message;
   }
#endif
};

/** Parses a transfer server -> client line.
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
*  \sa XFER_S2C, XFER_NET_SESSION, yiffurc_release( ), xfer_s2c_parse_peek( ) */
YIFFURC_LINK const struct XFER_S2C* YIFFURC_CALL xfer_s2c_parse( const char* line, int len,
                                                                 struct XFER_NET_SESSION* session );

/** Parses a transfer server -> client line. without updating session state info.
*  \param[in]     line     The raw command line.
*  \param[in]     len      Length of the command line.
*  \param[in]   session    Session state data.
*  \returns A XFER_S2C structure that should
*           be freed with yiffurc_release( ) when no longer
*           needed, or \c 0 on error.
*
*  This is identical to xfer_s2c_parse( ) but \a session is considered const,
*  so lines parsed with this function won't count towards the network state.
*  \sa XFER_S2C, XFER_NET_SESSION, yiffurc_release( ), xfer_s2c_parse( )  */
YIFFURC_LINK const struct XFER_S2C* YIFFURC_CALL xfer_s2c_parse_peek( const char* line, int len,
                                                                      const struct XFER_NET_SESSION* session );


/** \addtogroup xfer_c2s_encode Client-to-Server Encoding
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
* \{ */
/** Set user name.
*  \returns Encoded command string that should be freed
*           with xfer_yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL xfer_c2s_enc_user( const char* player );

/** Set password.
*  \returns Encoded command string that should be freed
*           with xfer_yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL xfer_c2s_enc_password( const char* pass );

/** SETR command.
*  \param   unk   Normally 0.
*  \returns Encoded command string that should be freed
*           with xfer_yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL xfer_c2s_enc_setr( int unk );

/** Tell the server about the file being uploaded.
*  \returns Encoded command string that should be freed
*           with xfer_yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL xfer_c2s_enc_uploadinfo( const char* filename,
                                                               int filesize );

/** Make the upload part of a (dream?) package.
*  \param   unk   Normally 0.
*  \returns Encoded command string that should be freed
*           with xfer_yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL xfer_c2s_enc_upload2pkg( unsigned int dream_id,
                                                               int unk );

/** Send information about chunk data.
*  \returns Encoded 10-unsigned char sequence that should be freed
*           with xfer_yiffurc_release( ). */
YIFFURC_LINK const unsigned char* YIFFURC_CALL xfer_c2s_enc_chunkhead( unsigned int chunk_id,
                                                                       unsigned int chunk_crc32 );

/** Atomic login and request to download a package.
*  \returns Encoded command string that should be freed
*           with xfer_yiffurc_release( ). */
YIFFURC_LINK const char* YIFFURC_CALL xfer_c2s_enc_atomicfetch( const char* player,
                                                                const char* pass,
                                                                const char* package,
                                                                int start_off,
                                                                unsigned int   send_ahead,
                                                                int chunk_size );

/* Stock encodes. */

/** Log out of file server.
*  \returns Stock encoded command string. */
YIFFURC_LINK const char* YIFFURC_CALL xfer_c2s_enc_bye( );

/** \} xfer_c2s_encode */
/** \} xfer */

/** \addtogroup scanning Simple Command Scanning
\{ */

/** Extracts common fields from a raw network string.
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
*/
YIFFURC_LINK int YIFFURC_CALL net_scan( const char* pattern, 
                                        const char* subj, int subj_len, ... );
/** \} scanning */

/** \} net */

#if defined( __cplusplus )
} // namespace yiffurc
#endif

#endif
 