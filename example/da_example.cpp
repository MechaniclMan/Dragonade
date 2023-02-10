/*	Renegade Scripts.dll
    Dragonade Example Plugin
	Copyright 2012 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "engine_da.h"
#include "da.h"
#include "da_example.h"
#include "da_settings.h"
#include "da_translation.h"
#include "da_vehicle.h"
#include "GameObjManager.h"

/*
The following are examples on how to use some of the core sytems in DA:

Event System
Game Mode System
Game Feature System
Player Observers
Player Data Manager
Improved GameObject Observers
Game Mode Framework

These systems are designed to allow features to be decentralized. No longer do you have to manually initialize everything from one place,
nor do you need a central settings struct that holds all settings. Instead game features and game modes are registered using a registrant
and automatically loaded when enabled, and automatically unloaded when disabled.

Note that DA has no plugin interface. Instead you just use the Event, Game Mode, and Game Feature Systems to implement things 
in the exact same way DA does.
*/


/*
DA's core is divided into various manager classes. Each one handling a different type of object or aspect of the game. 
Generally, managers implement the following types of things:
-Essential functionality that is used to implement other features.
-Non-essential functionality that is centralized so there aren't multiple implementions of it in various game features.
-Settings too small, or that would not be appropriate, to make a game feature out of.
-Log messages.
-Default events.
-Console commands.


DAEventManager: da_event.cpp/h
-Handles event system.
-Installs hooks.
-Interacted with through the DAEventClass, not directly.

DASettingsManager: da_settings.cpp/h
-Loads settings files.
-Provides various functions to retrieve settings.
-Force settings reload by calling Reload.

DAGameObjManager: da_gameobj.cpp/h
-Handles calling the extra events added in DAGameObjManager.
-Not interacted with directly.

DAPlayerManager: da_player.cpp/h
-Creates and manages each player's DAPlayerClass.
-Default connection request and suicide events.
-Implements DisallowedNickCharacters and DisallowedNicks settings.
-Writes purchase log messages.
-Not interacted with directly.
DAPlayerClass: da_player.cpp/h
-Handles player observers.
-Handles player data.
-Implements c4 locking, beacon locking, muting, and tags.
-Holds player data used by the managers.
-Call cPlayer::Get_DA_Player to get that player's DAPlayerClass.

DAGameManager: da_game.cpp/h
-Selects game mode
-Loads/unloads game features
-Loads da.ini version of the svrcfg_cnc.ini settings.
-Writes gameover log message.
-Functions provided to get current game mode or a list of game features.

DALogManager: da_log.cpp/h
-Handles TCP log connection to bots.
-Provides functions Write_Log and Write_GameLog to send log messages.

DATranslationManager: da_translate.cpp/h
-Loads translation DB from da.ini
-Provides functions Translate, Translate_With_Team_Name, Translate_Weapon, and Translate_Soldier to translate object names.

DADamageLog: da_damage.cpp/h
-Records all damage done to an object in the last 30 seconds.
-Includes functions to turn this damage log into a damage table, get the highest damager, get total damage done by a team, etc.

DATeamManager: da_team.cpp/h
-Handles team remixing, rebalancing, and swapping.
-Implements RebalanceTime, RemixFrequency, SwapChance, EnableRebalance, EnableFreeTeamChanging, and ForceTeam settings.
-Default team change request event.

DAVehicleManager: da_vehicle.cpp/h
-Loads VehicleLimit setting.
-Implements per-player and per-team vehicle limits(NYI).
-Checks vehicle limit and vehicle factory status for vehicle purchases.
-Default vehicle purchase event.
-Functions Get_Team, Get_Last_Driver, and Get_Last_Player provided to get extra information about a vehicle.
-Writes vehicle kill log messages.

DASoldierManager: da_soldier.cpp/h
-Loads spawn character settings.
-Default character purchase event.
-Writes player and bot kill log messages.
-Not interacted with directly.

DAC4BeaconManager: da_c4beacon.cpp/h
-Handles all beacon and C4 log messages.
-Sends beacon deploy message to team.
-Implements per-player and per-team C4 limits(NYI).
-Not interacted with directly.

DABuildingManager: da_building.cpp/h
-Plays building sounds(NYI)
-Writes building kill log messages(NYI)
-Not interacted with directly.

DAChatCommandManager: da_chatcommand.cpp/h
-Holds global and event chat commands.
-Parses chat and calls chat commands.
-Interacted with through DAEventClass, DAPlayerObserverClass, and DAChatCommandRegistrant.

DAPluginManager: da_plugin.cpp/h
-Loads plugins.
-Not interacted with directly.

DADisableListManager: da_disable.cpp/h
-Implements various disable list settings.
-Functions Is_Preset_Disabled and Is_Preset_Disabled_For_Character provided to check disable list.
*/


/*
At the heart of DA is the Event System. The DAEventClass includes over 60 different events divided into a few different types.
Core classes, managers, game features, and game modes all make use of this class to implement their functionality.

Event Type:
1. DAEvent. These are events that can happen either to players, or the game in general. They are registered with Register_Event.
2. DAObjectEvent. These can involve multiple types of objects. The second parameter determines what type of objects will trigger the event.
   Registered with Register_Object_Event.

Event Format:
1. Returns bool/int. These allow you to block or somehow affect the event. If suffixed with "Request" then it will be paired with a similarly named
event that triggers after the event is completed. Examples of this would be the chat, purchase, and damage events.
2. Void. These trigger after the event is complete and cannot be blocked or modified. Examples of this are the player leave and level loaded events.

DAEventClasses have timer functionality and can have chat commands or key hooks directly bound to them. See below for examples of this.
See da_event.h for the full list of events.
*/


/*
Each game mode has its own settings file in which all settings can be defined. This allows you to enable infinite ammo for only the Domination game mode, for exmaple.
The current game mode is controlled by the "GameMode" setting in da.ini. This can of course be set per map.
The following is our example game mode. It's a sort of vehicles only game mode.
*/
void DAExampleGameModeClass::Init() {
	//Here we register the events we want.

	//DAEvents are events that can happen either to players, or the game in general.
	Register_Event(DAEvent::CHARACTERPURCHASEREQUEST);
	Register_Event(DAEvent::POWERUPGRANTREQUEST);
	Register_Event(DAEvent::ADDWEAPONREQUEST);


	//DAObjectEvents can involve multiple types of objects. The second parameter determines what type of objects will trigger the event.

	//DAMAGERECEIVEDREQUEST means the object type applies to the object being damaged. DAMAGEDEALTREQUEST would make it apply to the object doing the damage, instead.
	Register_Object_Event(DAObjectEvent::DAMAGERECEIVEDREQUEST,DAObjectEvent::BUILDING); //This event will only trigger when buildings are damaged.

	Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::VEHICLE); //This event will only trigger when vehicles are created.
}

int DAExampleGameModeClass::Character_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const SoldierGameObjDef *Item) {
	return 4; //Since this is supposed to be vehicles only block all character purchases. 4 is "Item not in stock."
}

int DAExampleGameModeClass::PowerUp_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const PowerUpGameObjDef *Item) {
	return 4; //Since this is supposed to be vehicles only block all powerup purchases. 4 is "Item not in stock."
}

bool DAExampleGameModeClass::Add_Weapon_Request_Event(SoldierGameObj *Soldier,const WeaponDefinitionClass *Weapon) {
	return false; //Block players from having any weapons.
}

bool DAExampleGameModeClass::Damage_Request_Event(DamageableGameObj *Victim,OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone) {
	Offense->Scale_Damage(0.5f); //Since there's no way to repair in this game mode we will half all damage to buildings.
	return true;
}

void DAExampleGameModeClass::Object_Created_Event(GameObject *obj) {
	Start_Timer(1,0.1f,false,obj->Get_ID()); //Start a timer with the ID of the gameobject as the data.
}

void DAExampleGameModeClass::Timer_Expired(int Number,unsigned int Data) {
	VehicleGameObj *Vehicle = (VehicleGameObj*)GameObjManager::Find_SmartGameObj(Data); //Turn the ID back into a gameobject.
	if (Vehicle && Vehicle->Get_Lock_Owner()) { //Just in case.
		Vehicle->Add_Occupant((SoldierGameObj*)Vehicle->Get_Lock_Owner(),0); //Force the purchaser of the vehicle into it.
	}
}

/*
Here's our game mode registrant. This registers the game mode with the game mode manager and defines its name.
The first parameter is the "short name." This is used for the GameMode setting in da.ini, the name of the game mode specific settings file,
the game mode setting on GameSpy, and appended to the server title if that is enabled.
The second parameter is the "long name." This is used in any messages involving this game mode. Such as the "Running in %s mode." message.
The third optional parameter is the "required setting." This is for if the game mode requires certain data to work on a map, such as flag locations 
in CTF. For that you might put "GDIFlag_X." Game modes such as sniping or infantry only inheritly work on all maps and don't need to make use of this setting.
*/
Register_Game_Mode(DAExampleGameModeClass,"Example","Super Fun Example",0);
/*
Register_Game_Mode is a macro that produces the following code:
DAGameModeRegistrant<DAExampleGameModeClass> DAExampleGameModeClassRegistrant("Example","Super Fun Example",0);

It's possible to overwrite existing game modes by registering your game mode with the same name.
The following replaces the Domination game mode with our example game mode.
Register_Game_Mode(DAExampleGameModeClass,"Domination","Domination",0);
*/




/*
Each game feature is controlled by a setting defined in its registrant. On level load all game features are checked and automatically 
loaded/unloaded as necessary.
If your game feature needs to store settings they should be included in its class definition, not global variables.
*/
void DAExampleGameFeatueClass::Init() {
	//Initialize the player data manager. From this point all data management will be done automatically.
	DAPlayerDataManagerClass<DAExamplePlayerDataClass>::Init();

	//Register our events.
	Register_Event(DAEvent::REFILL,INT_MAX); //The second parameter here is the priority. We want to make sure these trigger and block the event before anything else.
	Register_Event(DAEvent::TEAMCHANGEREQUEST,INT_MAX);
	Register_Object_Event(DAObjectEvent::DAMAGEDEALTREQUEST,DAObjectEvent::PLAYER); //Only triggers when a player is damaging an object.

	//Chat commands can be bound directly to a DAEventClass. This allows chat commands to be automatically loaded and unloaded along with game modes and game features.
	//The somewhat ugly format is a limitation of the C++ language.
	Register_Chat_Command((DAECC)&DAExampleGameFeatueClass::Example1_Chat_Command,"!example1",0,DAAccessLevel::NONE,DAChatType::TEAM | DAChatType::PRIVATE);
	Register_Chat_Command((DAECC)&DAExampleGameFeatueClass::Example2_Chat_Command,"!example2"); //Short version with defaults.

	//DAEventClass includes a timer system. Timers can have multiple data parameters and can be set to repeat.
	Start_Timer(1,5.0f,true); //Triggers every 5 seconds repeating.


	//The settings manager automatically loads settings from both da.ini and the game mode specific settings file.
	//Game mode map specific > game mode general > da.ini map specific > da.ini general
	NodDamageScale = DASettingsManager::Get_Float("NodDamageScale",1.1f); //By default Nod takes 10% more damage.
	GDIDamageScale = DASettingsManager::Get_Float("GDIDamageScale",0.8f); //By default GDI takes 20% less damage.
}

bool DAExampleGameFeatueClass::Team_Change_Request_Event(cPlayer *Player) {
	return false; //Block team change requests.
}

bool DAExampleGameFeatueClass::Damage_Request_Event(DamageableGameObj *Victim,OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone) {
	if (Victim->Get_Player_Type() == 0) {
		Offense->Scale_Damage(NodDamageScale); //Scale damage based on loaded setting.
	}
	else if (Victim->Get_Player_Type() == 1) {
		Offense->Scale_Damage(GDIDamageScale);
	}
	return true;
}

void DAExampleGameFeatueClass::Timer_Expired(int Number,unsigned int Data) {
	Give_Credits_Team(0,100.0f); //All players get 100 credits every 5 seconds.
	Give_Credits_Team(1,100.0f);
}

bool DAExampleGameFeatueClass::Example1_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	//Allow players to buy the following player observer.
	if (!Player->Get_DA_Player()->Find_Observer("DAPlayerObserverExampleClass")) { //Don't want players to have multiple of this.
		if (Player->Purchase_Item(1000)) {
			Player->Get_DA_Player()->Add_Observer(new DAPlayerObserverExampleClass);
		}
	}
	return true;
}

bool DAExampleGameFeatueClass::Example2_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	//Allow players to buy the following gameobject observer.
	VehicleGameObj *Vehicle = Player->Get_GameObj()->Get_Vehicle();
	if (Vehicle) {
		if (!Get_Player_Data(Player)->Purchased) { //Limit one per customer.
			if (Player->Purchase_Item(2000)) {
				Vehicle->Add_Observer(new DAExampleObserverClass);
				Get_Player_Data(Player)->Purchased = true;
			}
		}
	}
	return false;
}

/*
This is the game feature registrant. It registers the game feature with the game feature manager.
The first parameter is the name. It's used in the loaded message and listed in GameSpy if you have that enabled. You can leave this blank if you don't want the game feature to be listed on GameSpy or have a loaded message.
The second parameter is the setting name. That's what you use in the settings file to enable or disabled the game feature. You can leave this blank to have the game feature always be enabled.
The third optional parameter is the "required data." This works the same way as in the game mode and defines some data that has to exist for this
game feature to work. One example would be "Crate1_X", spawn locations for crate spawns. If this data is not defined the game feature will 
be automatically unloaded for that map.
*/
Register_Game_Feature(DAExampleGameFeatueClass,"Example Game Feature","EnableExample",0);
/*
Register_Game_Feature is a macro that produces the following code:
DAGameFeatureRegistrant<DAExampleGameFeatueClass> DAExampleGameFeatueClassRegistrant("Example Game Feature","EnableExample",0);

It's possible to overwrite existing game features by registering your game feature with the same name.
The following replaces the Taunts game feature with our example game feature
Register_Game_Feature(DAExampleGameFeatueClass,"Taunts","Taunts",0);
*/











/*
Player observers are similar to gameobject observers except that they attach to a player, not their gameobject. This means they will persist
through death. They can also be set to persist between multiple maps or the player leaving the game.
You do not need to register events. All events are automatically called.
Player observers have a higher priority than DAEventClasses, meaning that all events are triggered first for a player observer and then for the event
classes. This allows player observers to temporarily overwrite events, such as key hooks.
See da_player.h for the full list of events.
*/

void DAPlayerObserverExampleClass::Init() {
	//As you can see key hooks can be listed as one of the triggers for a chat command.
	//An example of the above note on priority. This will overwrite the vehicle ownership system's use of the key hook "VehStatus" for this player.
	Register_Chat_Command((DAPOCC)&DAPlayerObserverExampleClass::Refill_Chat_Command,"!refill|!r|VehStatus");

	//This makes the observer persist through the player leaving and rejoining the game.
	//You can also use PERSISTMAP to make it persist through multiple maps.
	Add_Flag(DAPlayerFlags::PERSISTLEAVE);
}

bool DAPlayerObserverExampleClass::Refill_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType) {
	Console_InputF("msg %s called\n",__FUNCTION__);
	Grant_Refill(Get_GameObj());
	return false;
}

int DAPlayerObserverExampleClass::Character_Purchase_Request(float &Cost,const SoldierGameObjDef *Item) {
	Cost *= 0.9f; //10% discount on characters.
	return -1;
}

int DAPlayerObserverExampleClass::Vehicle_Purchase_Request(float &Cost,const VehicleGameObjDef *Item) {
	Cost *= 0.8f; //20% discount on vehicles.
	return -1;
}

int DAPlayerObserverExampleClass::PowerUp_Purchase_Request(float &Cost,const PowerUpGameObjDef *Item) {
	Cost *= 0.7f; //30% discount on powerups.
	return -1;
}

bool DAPlayerObserverExampleClass::Vehicle_Entry_Request(VehicleGameObj *Vehicle,int &Seat) {
	//This player can only enter the passenger seat of vehicles. If it's a one seater then the player is prohibited from entering.
	if (Seat == 0) {
		Seat = Find_Empty_Vehicle_Seat(Vehicle,false);
	}
	return true;
}

void DAPlayerObserverExampleClass::Vehicle_Enter(VehicleGameObj *Vehicle,int Seat) {
	//This player increases the health of any vehicle they enter by 10%
	Vehicle->Get_Defense_Object()->Set_Shield_Strength_Max(Vehicle->Get_Defense_Object()->Get_Shield_Strength_Max()*1.1f);
	Vehicle->Get_Defense_Object()->Set_Shield_Strength(Vehicle->Get_Defense_Object()->Get_Shield_Strength_Max());
}

void DAPlayerObserverExampleClass::Vehicle_Exit(VehicleGameObj *Vehicle,int Seat) {
	//Remove the increase when they exit.
	Vehicle->Get_Defense_Object()->Set_Shield_Strength_Max(Vehicle->Get_Definition().Get_DefenseObjectDef().ShieldStrengthMax);
}

bool DAPlayerObserverExampleClass::C4_Detonate_Request(C4GameObj *C4) {
	return false; //Don't trigger proximity C4.
}

//This is called each time the player receives a new gameobject.
void DAPlayerObserverExampleClass::Created() {
	//Another 10% health increase.
	Get_GameObj()->Get_Defense_Object()->Set_Health_Max(Get_GameObj()->Get_Defense_Object()->Get_Health_Max()*1.1f);
	Get_GameObj()->Get_Defense_Object()->Set_Health(Get_GameObj()->Get_Defense_Object()->Get_Health_Max());
}

//This is called when the player damages another object.
bool DAPlayerObserverExampleClass::Damage_Dealt_Request(DamageableGameObj *Victim,OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone) {
	if (Type == DADamageType::SPLASH) {
		return false; //This player deals no splash damage.
	}
	return true;
}

//This is called when the player is damaged.
bool DAPlayerObserverExampleClass::Damage_Received_Request(OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone) {
	if (Type == DADamageType::SQUISH || Type == DADamageType::FALL) {
		return false; //This player cannot be squished and takes no fall damage.
	}
	return true;
}

//This is called when the player kills another object.
void DAPlayerObserverExampleClass::Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	//Anything this player kills explodes.
	Commands->Create_Explosion("Explosion_Shell_Artillery",Commands->Get_Position(Victim),Get_GameObj());
}

//This is called when the player is killed.
void DAPlayerObserverExampleClass::Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	//Anyone who kills this player gets a refill.
	Grant_Refill(Killer);
}









/*
GameObjObserverClass is the base of ScriptImpClass. It provides the various events, such as Custom and Destroyed, while the ScriptImpClass provides
the script parameters and means to create the observer. Scripts were never really intended for what we've used them for, tough. They were intended
to be attached to objects in leveledit, not by other scripts or code. This makes them somewhat hard to work with if you need to create them directly
or pass complex parameters. For this reason scripts are no longer used anywhere in DA. In most cases a DAEventClass is used to implement a feature
that would have previously been implemented through a script attached to every object.

However, there are still a few places where script functionality is useful, such as crate effects or the vehicle ownership system. 
For this we have DAGameObjObserverClass.
It's an improved version of GameObjObserverClass that can be attached to an object similarly to ScriptImpClass. 
It includes extra events, better timer functionality, and an easier method of attaching to objects.

The following is used to attach a DAGameObjObserverClass:

obj->Add_Observer(new DAGameObjObserverClass);

See da_gameobj.h for the full list of events.
*/

//This replaces the Created and Attach events. Note that the gameobject is not passed to the events added by DA since you can get it with Get_Owner().
void DAExampleObserverClass::Init() {
	Start_Timer(1,5.0f); //Timers are created like this instead of with Commands->Start_Timer.
	Stop_Timer(1); //We also have this now.
}

//Use the destructor in place of Detach.
DAExampleObserverClass::~DAExampleObserverClass() {
}

//Same as stock event.
void DAExampleObserverClass::Destroyed(GameObject *obj) {
}

//Same as stock event.
void DAExampleObserverClass::Timer_Expired(GameObject *obj,int Number) {
}

//This allows you to block players from getting in the vehicle the observer is attached to, or change their seat.
bool DAExampleObserverClass::Vehicle_Entry_Request(SoldierGameObj *Soldier,int &Seat) {
	if (Soldier->Get_Player_Type() != DAVehicleManager::Get_Team(Get_Owner())) { //Prevent the enemy from stealing this vehicle.
		return false;
	}
	return true;
}

//This allows you to block or change the damage done to the object the observer is attached to.
bool DAExampleObserverClass::Damage_Received_Request(OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone) {
	Offense->Scale_Damage(0.9f); //%10 damage reduction.
	return true;
}

//Called when the object the observer is attached to kills another object.
void DAExampleObserverClass::Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
}




//These are provided for legacy support, but there isn't much reason to use them.
extern "C" {

_declspec(dllexport) void Plugin_Init() {
}

_declspec(dllexport) void Plugin_Shutdown() {
}

}