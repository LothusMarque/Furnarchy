Tricorder V1.6.6
A Furnarchy2 module ©2007-2012 Mercenary Enclave Productions.

Tricorder is the evolution of Dream Locator, which in itself is a
re-emergence of the classic modules Gangsta Lovin' and its fork G-Unit.
It is not a port of Gangsta Lovin' or G-Unit, rather it's been (almost) entirely
rewritten from scratch.
However, the Compass drawing and positioning code was converted directly from
Gangsta Lovin's source code, so original credit goes to Cluracan for this.

Features:
* List of dreams in the current map
* List people in the current map
* List delta objects and floors
* Compass allowing you to walk to a selected dream, object, or floor
* Favorite Dreams list

Known Issues:
* MASSIVE numbers of Delta objects/floors can cause a temporary client stall until all of them are processed.
* Delta object/floor changes are not displayed (Until next map entry) for DS-caused effects.

Commands:
<NONE>


---Version History---
==1.6.6==
Added: Auto-synchronization between all running copies for favorites and main configuration.
Fixed: Compass colors didn't change on apply.

==1.6.5==
Fixed: Fixed population display after 029 update.
Fixed: Regex issues, seemingly related to my library. Changed parsing fixed it.

==1.6.4==
Added: AFK display in people list.
Modified: Updated compiler version.

==1.6.3==
Added: Delta Objects/Floors enable-on-startup checkboxes in Config.
Added: Expiration system, attempting to avoid stuck names.
Modified: Delta Objects/Floors default to off before user interaction. This is because of the stall possibility.
Fixed: DreamURL Builder stripping numbers. Whoops!
Fixed: Possible List Index out of Bounds error/crash.

==1.6.2==
Fixed: Minor breakage with the 027 update - AFK timer data on colorcodes.

==1.6.1==
Added: Help->Support... menuitem.
Added: Enter key now works like a click on lists.
Added: Rightclick menu to People list.
Added: Rightclick->Whisper... for People list.
Modified: Status text strings, slightly.

==1.6==
Added: Sort-by-column on Dreams, Objects, and Floors lists.
Added: Parameter column in Dream list. (Don't know what it does yet, though)
Modified: Adjusted regex and handling of new dream parameter as of 3-12-2009.
Modified: Basic sort system.
Modified: Minor internal tweaking.
Fixed: Forgot to change icon on Favorites window. :P (Thanks Blizz!)

==1.5==
NOW KNOWN AS TRICORDER!
Added: Delta Objects
Added: Delta Floors
Added: People list!
Added: Ability to temporarily turn off scanning of Delta Objects and/or Floors.
Added: Much error checking
Modified: Internal list changes. Hopefully faster under stress, but still slow.
Modified: Now uses Module Doc folder for configuration.
Modified: Widened default window size to accommodate all the tabs.
Fixed: Coordinate display with compass to show dreamed-coords. (Doubled X)
Fixed: DreamURL generation on unusual names/captions.

==1.0==
Initial release

