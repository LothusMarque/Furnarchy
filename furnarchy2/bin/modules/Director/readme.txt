Director V1.6
A Furnarchy2 module by Merc.
http://syphor.ne1.net

Director is a module to manage Furnarchy2 scripts in a GUI instead of using the standard text commands.
This module wouldn't have been possible to this degree if Cluracan hadn't added script-related API to Furnarchy. Thanks!
Now with script form support to make scripts just that much easier for the end used. :)

Read below the version history for Module Form specifications!



==Version History==
--Version 1.0--
Initial Release

--Version 1.1--
+-Added "Edit Script..." to available scripts rightclick menu.
+-Now uses Furnarchy 2.3-r0 CL_EVENT_SCRIPT to avoid polling delays and overhead.

--Version 1.5--
+-Beginnings of Module Form system for making scripts easier to use.
+-Added BUTTON, LABEL, and EDIT controls to Module Form.
+-Added Special "CENTER" to control properties.
m-Reversed positions of Available and Running lists. Felt better this way.

--Version 1.5.5--
+-Dform: About script hint in lists.
+-Dform: DROPLIST control type

--Version 1.6--
+-Enable-Disable now shows/hides Director
+-DForm: EDIT Command-on-Enter
+-DForm: EDIT, BUTTON controls now have a "Hint" property.
f-Paste/Enter press issue with help from Cluracan.


==Module Form Specs==
Module Form specifications:
Basically, a module form is a standard INI file, renamed to .dfrm.
An example follows below:

[Main]
Desc=The script does this\nAnd this is a second line.
0=B1
1=somecontrol

[Csomecontrol]
Type=LABEL
Caption=Nomad Control Panel
Location=10,20,20,40
<Special=CENTER>

[CB1]
Type=BUTTON
Caption=Toggle Wandering
Location=28,20,100,20
Command=roam
<Special=CENTER>

Main contains a list of the objects, numbered 0 and up.
Each control is in its own section, prefixed with C.
Type can be LABEL, BUTTON, EDIT, or DROPLIST.
Caption holds the text displayed on the item.
Location is Top, Left, Height, Width, in that order.
Command is what is sent to Furnarchy when a button is pressed or enter is hit while an EDIT is selected.
Stuff in brackets is optional.

DROPLIST's commands/caption are a little different than usual.
Separate commands/captions are split with commas. (Sorry, no escaping yet)
For example:
Command=echo Hello,echo World
Caption=Say Hello,Say World

Special options:
CENTER - centers the control horizontally in the display area.

Command parameters:
Commands use a simple replacement scheme to deal with dynamic
data, for example, from an edit control. Use %#% where the data
should go, # being the number of the control as set in Main.
For example, "online %2%", if the text of control #2 is set to
"Syphor Knight", will resolve to "online Syphor Knight".


