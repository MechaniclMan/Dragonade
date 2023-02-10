Dragonade 1.3
Created by Whitedragon and Black-Cell.net

Special thanks to:
Vloktboky, original auther of DA and creator of server-side modding in general.
Jonwil, creating scripts.dll and tt.dll.
Ma1kel, input and ideas on various things.
ExEric3, providing hosting and test servers.
...and everyone that supported BC over the years.


--------------------------------------------------------------------------------------------------------------------------------------------------------------


Installation:

1. Download and install the latest server version of scripts.dll from http://www.tiberiantechnologies.org/downloads. 
   Note that DA was designed to work with Scripts 4.0. It will not work with earlier versions and may or may not work with later versions.
2. Copy the contents of the server folder included in this zip to the server folder of your FDS, overwriting any existing files.
3. Configure da.ini, da_crates.ini, and the relevant gamemode.ini to your liking.


--------------------------------------------------------------------------------------------------------------------------------------------------------------


Update History:

Version 1.3:
  Additions:
   - Crates have been added. See da_crates.ini.
   - The settings manager has been overhauled and can now support multiple settings files beyond just da.ini and gamemode.ini. This is used to load
     da_crates.ini, but can just as easily be used in your own plugins.
   - Added setting "EnableStockKillMessages".
   - Added settings "DisableKillCounter", "DisableTeamKillCounter", "DisableDeathCounter", and "DisableTeamDeathCounter".
   - Added some crate and settings examples to the example plugin.
   - Added better protection against sending long messages which could cause clients to crash.

  Changes:
   - Switched to a (hopefully) better random number generator.
   - The Vehicle Queue will now gray out the vehicle menu for a player when their vehicle is building.

  Bug Fixes:
   - Fixed a bug where vehicle shells could sometimes not be seen by players that joined the game after the shell was created.
   - Settings and section names are no longer case sensitive.
   - Points Distribution will no longer give points for killing neutral vehicles that never had a player in them.
   - Fixed team swapping not working correctly if "SwapChance" was set to anything besides 50%.
   - Fixed a rare crash on map end.
   - Players will no longer get a death when switching teams.
   - Fixed an exploit in stock Renegade where a player could avoid a death by leaving the game after they died but before they respawned.


Version 1.2:
  Additions:
   - Added setting "ForceTT". This can be used in three ways:
     0: Allow non-TT clients to join the server.
     1: Allow non-TT clients to join the server and use server damage extrapolation for them. This means they will be unable to use any kind of damage modifying cheat, but their game will appear laggier.
     2: Allow only TT clients to join the server.
   - New Console functions to toggle server damage extrapolation for a player.
   - Added setting "TTRevision". This should be set to the latest publicly released TT revision to ensure all TT players are running that revision.
   - Added setting "DropCommandExpireTime" to the Loot Game Feature.
   - Parachutes now have sounds.
   - Headshot kill messages will now work when the killer is AI controlled.
   - Added "!unstuck" chat command that will attempt to move a player if they are stuck.
   - Added version of the "!vlimit" chat command that works when the Vehicle Queue is disabled.

  Changes:
   - Team remixing should now be more random.
   - Team rebalancing has been improved.
   - All beacon log messages now include the owner of the beacon.
   - Headshot kill messages will now be more accurate when the killer is a non-TT client.
   - The functions relating to fixing stuck objects have been improved.
   - Updated some out of date scripts files.

  Bug Fixes:
   - Fixed a bug in the partial nick matching used in various commands.
   - GameSpy should now display the correct next map.
   - Fixed some bugs in the chat command system.
   - Fixed page sound not playing for the player sending the page.
   - Beacon disarm log messages now trigger when the beacon is disarmed by means other than damage.
   - Fixed possible crash in RTC Game Feature when a player is on the neutral team.
   - Fixed some bugs in the Squad System.


Version 1.1:
  Additions:
   - The Drop Weapons Game Feature is now the Loot Game Feature and includes support for dropping backpacks, DNA powerups, and generic powerups(health/armor/weapons).
     The odds for what type of powerup will drop, and many other things, can be configured in the settings file.
   - Backpacks, and the newly added DNA poweups, will now have an icon above them that shows what weapons or character they contain.
   - The Parachutes Game Feature has been added. If enabled parachutes will automatically deploy when a player exits a flying vehicle above a certain height.
   - Messages have been added for when a player steals an enemy vehicle.
   - Setting "BlockFakeBeacons" has been added and will prevent players from deploying beacons that would not damage an enemy building.
   - Settings "EnablePlayerKillMessages", "EnableVehicleKillMessages", "EnableBuildingKillMessages", and "EnableBotKillMessages" have been added to the Advanced Kill Messages Game Feature.
   - Support for the vote keyboard shortcuts has been added.
   - Keyboard shortcut "VehKick" has been added to trigger "!vkick all". Clients will need to add and bind this key in the extended options menu to use it.
   - New console function "teammessage"/"thmsg" to send a host message to a specific team.
   - Some new documentation and examples have been added to the example plugin.
   - Added macros for simpler registering of global chat commands and key hooks.
   - Added macro to register console functions.
   - Events PowerUp_Grant, PowerUp_Grant_Request, Add_Weapon_Request, Add_Weapon, Remove_Weapon, and Clear_Weapons have been added to DAGameObjObserverClass.
   - Chat commands that are triggered by key hooks will now be passed TEXT_MESSAGE_KEYHOOK as the chat type.
  
  Changes:
   - All player based events will now be passed the player's cPlayer instead of their SoldierGameObj.
   - Vehicle shells will no longer spawn from AI controlled vehicles.
   - The _VEHKILL log messages have been split into _VEHKILL(player controllable vehicles), _HARVKILL(harvesters), _BUILDINGKILL(turrets/guard towers), and _BOTKILL(any other AI controlled vehicle).
   - The team messages generated by the Extra Radio Commands Game Feature and beacon deployments will no longer show up on IRC.

  Bug Fixes:
   - Fixed some memory leaks.
   - "CanRepairBuildings", "IsFriendlyFirePermitted", and "BeaconPlacementEndsGame" now work correctly when used in da.ini or gamemode.ini.
   - Fixed some color messages not working for players with scripts 2.6-3.4.
   - Fixed not being able to compile in debug mode.
   - Black Hand Rocket Soldiers in Domination mode will now work correctly with the Loot Game Feature.


Version 1.0.1:
  Bug Fixes:
   - Added missing cPlayer exports that would cause some plugins to not load correctly.
   - Fixed crash that occurred on certain fan maps with Tiberium Silos.
   - Fixed one of the examples in the example plugin not working.


Version 1.0:
   - Initial release


