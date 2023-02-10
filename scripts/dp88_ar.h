/*	Renegade Scripts.dll
	Copyright 2011 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#pragma once

#include "dp88_customAI.h"
#include "dp88_custom_timer_defines.h"


/*------------------------
Game Controller
  Game controller for AR maps, set's up map variables and manages several events
--------------------------*/

class dp88_AR_GameController : public ScriptImpClass
{
	// Events
	void Created( GameObject *obj );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Timer_Expired( GameObject *obj, int number );
	void Destroyed( GameObject *obj );

public:
	dp88_AR_GameController();
	~dp88_AR_GameController();

	bool created;

	short team0_countryID;		// Country ID for Soviets
	short team1_countryID;		// Country ID for Allies
	
	// Terror Drone console waiting for response
	int TD_Waiting_Console;

	// Mirage Tank
	char* mirageTank_disguisePresets[3];
};




// Class for vehicles (redundant, keeping it temporaraily for terror drone code)
class dp88_AR_Vehicle : public ScriptImpClass
{
	// Variables
	int pilotID;						// Pilots GameObject ID
	int attackingTerrorDroneID;			// ID of any Terror Drones attacking this vehicle
	bool dead;							// Set to true when we die

	// Events
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Created(GameObject *obj);
	void Timer_Expired( GameObject *obj,int number );
	void Killed(GameObject *obj, GameObject *killer);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
	void Animation_Complete ( GameObject *obj, const char *animation_name );
};




/*!
* A companion script for dp88_veterancyUnit which grants units health and/or armour
* regeneration dependant upon their promotion level (rookie, veteran, elite)
*/
class dp88_AR_Veterancy_HealthArmourRegen : public ScriptImpClass
{
	void Created( GameObject *obj );
	void Timer_Expired( GameObject *obj, int number );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );

	private:
		int veterancyLevel;
};




/*!
* A companion script for dp88_veterancyUnit which grants units a weapon upon creation
* which can subsequently be upgraded to a more powerful weapon when the player gets
* promoted
*/
class dp88_AR_grantDefaultWeapon : public ScriptImpClass {

	char weaponName[64];
	char oldWeapon[64];

	void Created( GameObject *obj );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Timer_Expired( GameObject *obj, int number );
};




/*------------------------
Deployable Infantry script
  Allows an infantry unit to 'deploy', which involves switching to
  a more powerful weapon and creating a defensive structure of some
  kind at the players location.
--------------------------*/

class dp88_AR_Deployable_Infantry : public JFW_Key_Hook_Base
{
	// Events
	void Created( GameObject *obj );
	void Killed( GameObject *obj, GameObject *killer );
	void Destroyed ( GameObject* obj );
	void KeyHook();
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Timer_Expired( GameObject *obj,int number );		// TEMPORARY

	// Ugly hack to clean up when buying a new character
	void Detach(GameObject *obj);

	// Get armour type to set for the given veterancy level
	const char* GetArmourType ( int vetLevel );

	// Get weapon powerup to grant for the given veterancy level
	 const char* GetWeaponPowerup ( int vetLevel );


	// Variables
	int objectID;
	bool deployed;
	char* undeployedWeapon;
	int deployedObjectId;

	int cannotDeployStringId;
	int cannotDeploySoundId;
	time_t lastDeploy;	// Timestamp of last deploy keyhook event (prevent constant spamming)

	/* Current veterancy level */
	int currentVetLevel;

	/* Flags to mark which veterancy levels have deployed weapon powerups */
	bool hasRookieWeaponPowerup, hasVeteranWeaponPowerup, hasEliteWeaponPowerup;

	/* Original values for Skin & Armour types */
	char undeployedSkinType[128], undeployedArmourType[128];
};




/*------------------------
Chrono Legionnaire
--------------------------*/

class dp88_AR_CLEG : public ScriptImpClass {
	bool phasing;
	bool phasingBack;
	int zPosDropObj;
	short timeRemaining;
	int dropObjID;

	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
	void Killed(GameObject *obj, GameObject *killer);
	void Timer_Expired( GameObject *obj,int number );
};

class dp88_AR_CLEG_target : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
	void Killed(GameObject *obj, GameObject *killer);
	void Timer_Expired( GameObject *obj,int number );

	int currentResistance;
	int lastResistanceCheck;
	int effectObjectId;
};




// -------------------------------------------------------------------------------------------------

/*!
* \brief AR Miner Script
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This class contains the basic logic for the ore miners used in Apocalypse Rising and supports both
* AI and player controlled miners. A miner can collect ore from any ore fields defined by a script
* zone with the dp88_AR_Ore_Field_Zone script attached to it, which determines the value of each ore
* 'unit' in that field, thus implicitly supporting gem fields with the same scripts.
*
* The miner can hold up to a configurable number of ore units, each of which takes a configurable
* amount of time to mine from the ore field. Mining is achieved by remaining inside an ore field
* whilst ore units are 'collected'.
*
* Miners can deposit ore at any time as long as they have at least one unit mined, with the total
* value of the deposit being based on the number of units collected and the value of each unit. Ore
* deposits are achieved by entering a script zone with the dp88_AR_Ore_Deposit_Zone attached to it
* with the team parameter matching that of the miner.
*
* When the AI is enabled the miner will use the pathfind grid to locate the nearest ore field and
* drive to it to begin mining. Once it has collected a full load of ore it will locate the nearest
* ore deposit zone and drive there to unload it's ore. This will be repeated ad-infinitum until the
* miner is destroyed. If you have problems with the miner getting stuck on terrain or other objects
* you should place pathfind blockers around the problematic areas and re-generate pathfind data to
* make it route around those locations.
*
* \param enableAI
*   Specifies that this miner should use its AI to mine autonomously. 1 to enable AI, 0 otherwise
* \param loadLevels
*   Specifies the total number of ore units this miner can hold at any time
* \param orePerLoadLevel
*   Amount of ore mined in each unit, this is multiplied by the oreValue parameter in the script
*   attached to the ore field to calculate the credit value of each ore unit
* \param timePerLoadLevel
*   The amount of time, in seconds, it takes to mine one ore unit from an ore field
* \param unloadtime
*   The amount of time, in seconds, it takes to unload all ore units at an ore deposit zone
* \param aiStartDelay
*   If the miner AI is enabled this specifies the initial delay before starting the first AI action,
*   which is required to avoid the original Renegade Harvester AI overriding this one.
* \param dockAnimation
*   An optional animation to be played when depositing ore at a refinery
* \param dockSound
*   An optional sound effect to be played when depositing ore at a refinery
* \param miningAnimation
*   An optional animation to be played in a loop whilst mining in an ore field
* \param miningSound
*   An optional sound effect to be played each time the ore load level increases
* \param idleAnimation
*   An optional animation to be played in a loop when the miner is not mining or docked
*
* \note
*   Because this script is designed to act as a base class for both types of AR miner it is not
*   named after either teams miner and does not appear as an available script in LevelEdit. However,
*   because there is no additional logic required for the Soviet War Miner there isn't actually a
*   derived script for that at this time, thus the dp88_AR_War_Miner script in LevelEdit is really
*   an instance of this base class script.
*/

class dp88_AR_Miner : public JFW_Key_Hook_Base	// Inherit from keyhook base for chrono miner
{
protected:
	int oreFieldRand;		// Used to prevent glitching by entering/exiting an ore field rapidly. 0 = not in ore field
	int oreFieldValue;
	int oreLoadLevel;
	int oreValue;

	bool useAI;				// Is AI enabled?
	bool animating;         // Used to identify if the harvest animation is running so it doesn't start it a second time
	int aiState;			// Current AI state

public:
	// Default event handlers for the AI miners
	virtual void Created ( GameObject *obj );
	virtual void Custom ( GameObject *obj, int type, int param, GameObject *sender );
	virtual void Action_Complete( GameObject *obj, int action_id, ActionCompleteReason complete_reason );
	virtual	void KeyHook() {};

protected:
	// These functions implement standard drive to ore field, harvest some ore,
	// return to refinery, dock and unload ore functionlity for an AI miner. They
	// can be overloaded to provide custom functionality in derived classes.
	virtual void DriveToOreField ( GameObject *obj );
	virtual void EnteredOreField ( GameObject *obj, GameObject* oreField );
	virtual void ExitedOreField ( GameObject *obj, GameObject* oreField );
	virtual void ReturnToRefinery ( GameObject *obj );
	virtual void DockAtRefinery ( GameObject *obj );
	virtual void DockedAtRefinery ( GameObject *obj );
	virtual void UndockedFromRefinery ( GameObject *obj );

	// AI states for the miner
	enum MINER_AISTATES
	{
		MINER_AISTATE_IDLE,
		MINER_AISTATE_DRIVE_TO_ORE,
		MINER_AISTATE_COLLECTING_ORE,
		MINER_AISTATE_RETURN_TO_REFINERY,
		MINER_AISTATE_DOCK_AT_REFINERY,
		MINER_AISTATE_UNLOADING_ORE
	};

	enum MINER_ACTIONIDS
	{
		MINER_ACTIONID_DRIVE_TO_ORE,
		MINER_ACTIONID_RETURN_TO_REFINERY,
		MINER_ACTIONID_DOCK_AT_REFINERY
	};
};


/*------------------------
Chrono Miner
--------------------------*/

class dp88_AR_Chrono_Miner : public dp88_AR_Miner
{
	int objectId;			// ID of the object we are running on, since Keyhook() does not include the GameObject...
	int driverId;			// ID of our current driver, or NULL

	void Created ( GameObject *obj );
	void Damaged ( GameObject *obj, GameObject *damager, float amount );
	void Custom ( GameObject *obj, int type, int param, GameObject *sender );
	void KeyHook();

private:
	void ReturnToRefinery ( GameObject *obj );
	bool Start_Chronoshift( GameObject *obj );
	void Do_Chronoshift( GameObject *obj, int target_zone_id );
	void Complete_Chronoshift( GameObject *obj );

	enum CMINER_AISTATES
	{
		CMINER_AISTATE_CHRONOSHIFTING = MINER_AISTATE_UNLOADING_ORE+1
	};
};


class dp88_AR_Chrono_Miner_Chronozone : public ScriptImpClass
{
	void Created( GameObject *obj );

public:
	// Set by dp88_AR_Chrono_Miner when it is shifting to this zone, used
	// to prevent multiple miners trying to shift to the same place
	int chronominer_id;
};


class dp88_AR_Ore_Field_Zone : public ScriptImpClass {
	void Entered( GameObject *obj, GameObject *enterer );
	void Exited ( GameObject *obj, GameObject *exiter );
};


class dp88_AR_Ore_Deposit_Zone : public ScriptImpClass {
	void Entered( GameObject *obj, GameObject *enterer );
};




/*------------------------
VTOL Landing Zone Animation
--------------------------*/

class dp88_Aircraft_LandingZone : public ScriptImpClass
{
	void Entered( GameObject *obj, GameObject *enterer );
	void Exited ( GameObject *obj, GameObject *exiter );
};

class dp88_Aircraft_LandingZone_Aircraft : public ScriptImpClass
{
	void Created ( GameObject *obj );
	void Custom ( GameObject *obj, int type, int param, GameObject *sender );

private:
	int driverID;
	unsigned int landingZoneCount;  // Since we may be in more than one at a time
};



/*------------------------
Terror Drone Script
--------------------------*/

/*class dp88_AR_TerrorDrone : public ScriptImpClass {
	void Created( GameObject *obj );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Killed( GameObject *obj, GameObject *killer );

	int targetID;
	int pilotID;
	int consoleID;
	char defaultModel[32];
};*/




/*!
* \brief Remote Control Vehicle Console
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This script is attached to consoles to allow players to take remote control of a drone vehicle
* which is spawned by the console on demand. The vehicle preset which is spawned must have the
* dp88_RemoteControlVehicle script attached with a matching remoteControlID parameter to work
* correctly
*
* \param remoteControlID
*   Shared ID used by the console and drones to enable them to be linked correctly - each vehicle
*   preset should use a unique ID, but multiple consoles can share that vehicles ID
* \param vehiclePreset
*   Name of the vehicle preset to spawn as the drone
* \param cost
*   Cost of purchasing a drone when non exists
* \param team
*   ID of the team permitted to use this console (0: Nod, 1: GDI, 2: Both)
* \param chargeTime
*   Minimum time, in seconds, for the console to charge up before the purchase of a drone is
*   permitted and to recharge between drone purchases. Set to 0 to disable charging.
* \param accessDeniedSound
*   2D sound to play for a player of the wrong team who tries to use the console
* \param consoleOfflineSound
*   2D sound to play for a player trying to use a disabled console
* \param consoleChargingSound
*   2D sound to play for a player trying to use a console which is charging
* \param insufficientFundsSound
*   2D sound to play for a player trying to purchase a drone with insufficient credits
* \param connectionEstablishedSound
*   2D sound to play for a player when they successful take control of a drone
* \param animationName
*   Name of the animation for the state frames
* \param animFrameAvailable1
*   First frame of the animation to play when the console is available (drone ready to purchase)
* \param animFrameAvailable2
*   Last frame of the animation to play when the console is available (drone ready to purchase)
* \param animFrameCharging1
*   First frame of the animation to play when the console is charging
* \param animFrameCharging2
*   Last frame of the animation to play when the console is charging
* \param animFrameActive1
*   First frame of the animation to play when the console is in use
* \param animFrameActive2
*   Last frame of the animation to play when the console is in use
* \param animFrameIdle1
*   First frame of the animation to play when the console is available (drone idle in field)
* \param animFrameIdle2
*   Last frame of the animation to play when the console is available (drone idle in field)
* \param animFrameDisabled1
*   First frame of the animation to play when the console is disabled
* \param animFrameDisabled2
*   Last frame of the animation to play when the console is disabled
*/
class dp88_RemoteControlConsole : public ScriptImpClass
{
  void Created( GameObject *obj );
  void Detach ( GameObject* obj );
  void Poked ( GameObject *obj, GameObject *poker );
  void Custom( GameObject *obj, int type, int param, GameObject *sender );
  void Timer_Expired( GameObject *obj, int number );
  void SetEnabled ( GameObject* obj, bool state );

  int vehicleID;
  int pilotID;
  int m_pilotDummyID;
  Vector3 pilotDummyPos;
  bool m_bEnabled;
  int m_nChargeTime;
  void CreateDummy ( GameObject* pilot, Vector3 position, float facing );
  void DestroyDummy();

  void HandleDriverExit ( GameObject* obj, GameObject* pilot, GameObject* vehicle );

protected:
  LoopedAnimationController* m_pLoopedAnimCtrl;
public:
  dp88_RemoteControlConsole() : m_pLoopedAnimCtrl(0) {}
};



/*------------------------
Remote Control Vehicle script
--------------------------*/

class dp88_RemoteControlVehicle : public ScriptImpClass
{
	void Created( GameObject *obj );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Killed( GameObject *obj, GameObject *killer );

	int consoleID;
	int pilotID;
};



/*------------------------
Demo Truck Scripts
--------------------------*/

class dp88_AR_DemoTruck : public ScriptImpClass {

	// Events
	void Created( GameObject *obj );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Killed( GameObject *obj, GameObject *killer );
	void Damaged ( GameObject *obj, GameObject *damager, float amount );

	// Custom functions
	void dp88_AR_DemoTruck::Detonate( GameObject *obj );

	// Variables
	int pilotID;
	bool canDetonate;
};



/*------------------------
Paradrop Console Script
--------------------------*/

class dp88_AR_paradrop_Console : public ScriptImpClass {
	void Created( GameObject *obj );
	void Poked ( GameObject *obj, GameObject *poker );

	int last_triggered;
};



/*------------------------
Paradrop Script
--------------------------*/

class dp88_AR_paradrop : public ScriptImpClass {
	void Created( GameObject *obj );
	void Damaged( GameObject *obj, GameObject* damager, float amount );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );

	bool hit_ground;
};





// -------------------------------------------------------------------------------------------------

/*!
* \brief AR Prism Tower script
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This script implements the prism tower logic for Apocalypse Rising on top of the generic charged
* turret implementation provided by dp88_AI_ChargedTurret.
*
* \param Priority_Infantry
*   Base targetting priority for infantry targets, or 0 to ignore infantry
* \param Splash_Infantry
*   Determines if we should try to damage infantry with splash instead of hitting them directly.
*   This is useful for slow / inaccurate weapons which do splash damage: 1 to enable, 0 to disable
* \param Priority_Light_Vehicle
*   Base targetting priority for light vehicle targets, or 0 to ignore light vehicles
* \param Priority_Heavy_Vehicle
*   Base targetting priority for heavy vehicle targets, or 0 to ignore heavy vehicles
* \param Priority_VTOL
*   Base targetting priority for flying targets, or 0 to ignore flying targets
* \param Min_Attack_Range
*   Minimum distance at which the turret can engage enemies when using primary fire
* \param Max_Attack_Range
*   Maximum distance at which the turret can engage enemies when using primary fire
* \param Animation_Model
*   Optional name of a W3D model file to spawn and attach to the turret upon which the charge
*   animations defined in this script will be applied. This allows the turret itself to be running
*   a different set of animations, such as damage states via dp88_damageAnimation
* \param Animation_Model_Bone
*   If you specify an Animation_Model above then you must also specify the a bone on the parent
*   to which that model should be attached. This allows the animation model to be moved by the
*   animation set executing on the turret.
* \param Animation
*   Name of the animation which contains the frames for the various charge states
* \param Animation_Idle_Start_Frame
*   First frame number for the idle animation, which will loop continually whilst the turret is
*   powered and not currently attacking a target
* \param Animation_Idle_End_Frame
*   Final frame number for the idle animation
* \param Animation_Unpowered_Start_Frame
*   First frame number for the unpowered animation, which will loop continually whilst the turret
*   is in the unpowered state. This is not used if Requires_Power is disabled (see below).
* \param Animation_Unpowered_End_Frame
*   Final frame number for the unpowered animation
* \param Animation_Charge_Start_Frame
*   First frame number for the charging animation, which will be played once each time the turret
*   reloads its weapon. Once this animation completes the turret will fire again, therefore the
*   length of this animation must be greater than the weapon reload time.
* \param Animation_Charge_End_Frame
*   Final frame number for the charging animation
* \param Charge_Sound
*   Optional sound effect to be played each time the turret starts charging up
* \param Modifier_Distance
*   Priority modification to apply based on distance to target. Higher values will favour targets
*   which are closer to the turret, good for less accurate weapons
* \param Modifier_Target_Damage
*   Priority modification to apply based on damage a target has already sustained. Higher values
*   will favour targets which have already been damaged in combat, picking them off first.
* \param Modifier_Target_Value
*   Priority modification to apply based on the value of the target. Higher values will favour
*   targets with a higher purchase cost, good for hard hitting weapons.
* \param Requires_Power
*   Specify whether this turret requires base power to operate: 1 to require power, 0 to ignore
* \param Debug
*   Specify whether to produce a debug logfile about the turrets targetting decisions, this is
*   useful for fine tuning your base priorities and modifiers: 1 to enable, 0 to disable.
*
* \todo
*   Decide on a mechanism to ensure each firing sequence fires one bullet per assisting tower,
*   regardless of chain length. Can possibly overload dp88_AI_ChargedTurret::StartDischarging() to
*   introduce a 0.1 second delay for each level of assisting tower depth to ensure the charging
*   shot arrives before it fires its own attacking (or chain charging) shot.
*/
class dp88_AR_Prism_Tower : public dp88_AI_ChargedTurret
{
  /* -----
  Static variables
  ----- */

  static const int MAX_TOWERS = 20;
  static int prismTowerCount;
  static int prismTowerIds[MAX_TOWERS];
  static dp88_AR_Prism_Tower* prismTowerScripts[MAX_TOWERS];

  /* -----
  Static functions
  ----- */
  static void registerTower(int towerId, dp88_AR_Prism_Tower* script);
  static void deregisterTower(int towerId);
  static void clearTowerMap(int idx);
  static void calculateTowerMap();
  static bool calculateTowerMapPathSearch(int* sortedConnections, int numConnections, int tower1, int tower2);

  /* -----
  Variables
  ----- */

  // Is our current target a tower we are charging?
  bool isAssistingTower;

  // Towers adjacent to us in the tower map
  int adjacentTowerCount;
  int* adjacentTowers;


  /* -----
  Events
  ----- */

  void Created ( GameObject *obj );
  void Damaged ( GameObject *obj, GameObject *damager, float amount );
  void Killed ( GameObject *obj, GameObject *killer );
  void Destroyed ( GameObject *obj );
  void Custom ( GameObject *obj, int type, int param, GameObject *sender );
  void Timer_Expired ( GameObject *obj, int number );

  /* -----
  Functions
  ----- */

  // Override priority calculation with our own taking into account
  // the fact that the target might be a tower we are charging
  virtual float getPriority( GameObject *obj, GameObject *target );

  /*!
  * Overrides the team check mechanism to return true if the specified target is the tower we are
  * currently charging, otherwise the base class target validation logic will cancel the 'attack'
  */
  virtual bool checkTeam( GameObject *obj, GameObject *target );

  // Customised attack functionality
  virtual void attackTarget ( GameObject* obj, GameObject* target, bool primary );
  virtual void attackLocation ( GameObject* obj, Vector3 location, bool primary );
  virtual void stopAttacking ( GameObject* obj );

  /* Functions to start and stop charging of another tower */
  void StartAssisting(GameObject* obj, GameObject* tower, float priority);
  void StopAssisting(GameObject* obj);

  /*!
  * Send assistance requests to all neighbouring prism towers, except the one we are currently
  * assisting, if any
  */
  void SendAssistanceRequests(GameObject* obj);

  /*!
  * Send end assistance notifications to all neighnouring prism towers
  */
  void SendEndAssistanceNotifications(GameObject* obj);
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Object health linking script
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This is a simple script that links together the health and armour states of two different objects,
* keeping them in constant sync. This is useful when an ingame entity is made up of two seperate
* game objects, such as the AI controlled War Miner in Apocalypse Rising which spawns a seperate
* gun turret to allow the logic for the turret and the miner to execute on seperate objects and
* avoid conflicts between the attack and move-to actions.
*
* The script works on a parent/child basis, whereby the object this script is attached to is the
* child and the parent is the object whose ID is specified in the script parameters. Upon creation
* the script will inherit the maximum and current values of the parents health and armour states,
* however it will maintain it's own skin and armour types, thus allowing for a child object to be
* either more or less resistant to damage than it's parent, allowing the creation of strong or weak
* points in the overall entities defences.
*
* This script is usually attached to dynamically created objects by other scripts and it's unlikely
* to be attached to anything directly via LevelEdit.
*
* \param parentObjectId
*   ID of the parent game object with which this objects health and armour will be linked
*
* \note
*   You can link multiple child objects to a single parent object to create complex entities which
*   consist of many parts, but you should always ensure the same object is designated as the parent
*   to avoid conflicts between the scripts.
*/
class dp88_linkHealth : public ScriptImpClass
{
  void Created ( GameObject *obj );
  void Timer_Expired ( GameObject *obj, int number );
  void Killed ( GameObject *obj, GameObject *killer );
  void equaliseHealth( GameObject* obj );

  int parentObjID;
  float lastHealth;
  float lastArmour;
};

// -------------------------------------------------------------------------------------------------






/*
void printDebug ( const char debugline[], int objID )
{
	FILE *f = fopen("dp88_debug.txt","at");
	fprintf(f,"[%d:%s] %s\n",objID,Commands->Get_Preset_Name(Commands->Find_Object(objID)),debugline);
	fclose(f);
}
*/