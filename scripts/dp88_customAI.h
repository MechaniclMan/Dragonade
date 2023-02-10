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

#include "LoopedAnimationController.h"


/*------------------------
Base class for custom AI's
--------------------------*/


/*!
* \brief danpaul88's Custom AI Core
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This class contains the core logic used in custom AI scripts, such as the target prioritisation
* mechanism. It is not an actual script in it's own right and cannot be used in LevelEdit directly.
*/
class dp88_customAI : public ScriptImpClass
{
public:

	/* -----
	Constructor / Destructor (cleanup debug file if applicable
	----- */

	dp88_customAI() { debugFile = NULL; };
	~dp88_customAI() { if ( debugFile != NULL ) { fclose(debugFile); } };




	/* -----
	Variables
	----- */

	// Priority and weapon choice for each target type
	float priority_infantry;			bool primary_infantry;
	float priority_lightVehicle;		bool primary_lightVehicle;
	float priority_heavyVehicle;		bool primary_heavyVehicle;
	float priority_VTOL;				bool primary_VTOL;
	float priority_building;			bool primary_building;

	// Priority modifiers
	float modifier_distance, modifier_target_damage, modifier_target_value;

	// Attack ranges
	int primary_minRange, primary_maxRange;
	int secondary_minRange, secondary_maxRange;

	// Current target state
	int targetID;
	int targetLastSeen;
	float targetPriority;
	bool primary_target;

	// Debug state
	bool debug;
	FILE* debugFile;




	/* -----
	Events
	----- */

	virtual void Created( GameObject *obj );
	virtual void Action_Complete( GameObject *obj, int action_id, ActionCompleteReason complete_reason  );
	//virtual void Enemy_Seen ( GameObject *obj, GameObject *enemy );
	//virtual void Timer_Expired( GameObject *obj, int number );


	/* ----
	Functions
	----- */
	
	virtual void Init( GameObject *obj );
	virtual void loadSettings( GameObject *obj, bool loadSecondaryFireSettings = true, bool loadBuildingTargetSettings = false, bool oldSettingNames = false );
	
	float getDistance ( GameObject *obj1, GameObject *obj2 );
	virtual float getPriority( GameObject *obj, GameObject *target );
	virtual float getPriority( GameObject *obj, int target_id );
	bool getPrimary ( GameObject *target );

	/* Utility functions for both priority calculations and AI scripts to utilise */
	virtual bool IsVehicleEmpty( VehicleGameObj* vobj );
	virtual bool IsVehicleAIEnabled( VehicleGameObj* vobj );
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief danpaul88's Custom AI Heavy Vehicle Marker
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This is a dummy script with no functionality which can be attached to an object to indicate all of
* my custom AI scripts should treat this as a heavy vehicle. Any vehicle without this script will be
* considered to be a light vehicle.
*/
class dp88_AI_heavyVehicleMarker : public ScriptImpClass{};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Offensive Tank AI
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This is a somewhat experimental AI for vehicles which can track targets over a predefined distance
* and attack them. This script is not yet suitable for use in maps or mods and is subject to change.
*/
class dp88_tankAI_Offensive : public dp88_customAI
{
	/* ----
	Variables
	----- */

	// Preferred attack ranges
	int primary_prefRange, secondary_prefRange;
	bool movingToTarget;

	// Other
	int retreatDamageAmount;
	FILE *debugFile;
	
	// Events
	void Created( GameObject *obj );
	void Enemy_Seen ( GameObject *obj, GameObject *enemy );
	void Timer_Expired( GameObject *obj, int number );
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Turret AI
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_basedefences
*
* A custom designed turret AI code designed to allow maximum flexibility in implementation without
* needing multiple versions of effectively the same code to target different enemies (ie: VTOL vs.
* non-VTOL).
*
* The turret AI uses a priority based system to 'intelligently' pick targets based upon a range of
* criteria such as the type of target, how far away it is, if it's already taken damage and how
* valuable it is. All of these criteria can be fine tuned in the script parameters to give you a
* high degree of control over which of several targets a turret will choose to shoot at. This also
* has the effect of making multiple, closely placed, turrets with the same targetting parameters
* concentrate their fire on a single target for maximum impact.
*
* \param Priority_Infantry
*   Base targetting priority for infantry targets, or 0 to ignore infantry
* \param Weapon_Infantry
*   Weapon to use against infantry targets: 0 for primary fire, 1 for secondary fire
* \param Splash_Infantry
*   Determines if we should try to damage infantry with splash instead of hitting them directly.
*   This is useful for slow / inaccurate weapons which do splash damage: 1 to enable, 0 to disable
* \param Priority_Light_Vehicle
*   Base targetting priority for light vehicle targets, or 0 to ignore light vehicles
* \param Weapon_Light_Vehicle
*   Weapon to use against light vehicle targets: 0 for primary fire, 1 for secondary fire
* \param Priority_Heavy_Vehicle
*   Base targetting priority for heavy vehicle targets, or 0 to ignore heavy vehicles
* \param Weapon_Heavy_Vehicle
*   Weapon to use against heavy vehicle targets: 0 for primary fire, 1 for secondary fire
* \param Priority_VTOL
*   Base targetting priority for flying targets, or 0 to ignore flying targets
* \param Weapon_VTOL
*   Weapon to use against flying targets: 0 for primary fire, 1 for secondary fire
* \param Min_Attack_Range
*   Minimum distance at which the turret can engage enemies when using primary fire
* \param Max_Attack_Range
*   Maximum distance at which the turret can engage enemies when using primary fire
* \param Min_Attack_Range_Secondary
*   Minimum distance at which the turret can engage enemies when using secondary fire
* \param Max_Attack_Range_Secondary
*   Maximum distance at which the turret can engage enemies when using secondary fire
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
* \note
*   Vehicles are always classified as light vehicles unless they have the dp88_AI_heavyVehicleMarker
*   script attached to them.
*
* \warning
*   Never leave the Debug parameter enabled when releasing your mod, it will clog up everyones
*   machines with useless text files everywhere... people usually don't like that :D
*/
class dp88_AI_Turret : public dp88_customAI
{
public:
	// Events
	virtual void Created( GameObject *obj );
	virtual void Enemy_Seen ( GameObject *obj, GameObject *enemy );
	virtual void Timer_Expired( GameObject *obj, int number );

protected:
	bool requiresPower, splashInfantry;

	virtual void Init( GameObject *obj );
	virtual void loadSettings( GameObject *obj, bool loadSecondaryFireSettings = true, bool loadBuildingTargetSettings = false );

	// These can be overloaded from their default functionality as required, allowing the default
	// enemy seen procedure to be reused even when the behaviour of these checks has been altered
	virtual bool checkTeam ( GameObject* obj, GameObject* target );
	virtual bool checkRange ( GameObject* obj, GameObject* target, bool primary );
	virtual bool checkPowerState( GameObject* obj );




	/* These functions are used to initiate and control the AI actions and can be overloaded from
	their defaults if required to provide custom functionality, such as delaying an attack whilst
	waiting for a chargeup / popup animation to occur. */

	// These is called when a valid target has been identified and selected as the highest priority
	// target, the turret should begin attacking in this function. Note that this may be called
	// while another attack is already in progress, either normal or splash.
	virtual void attackTarget ( GameObject* obj, GameObject* target, bool primary );

	// This is called instead of attackTarget when we are set to splash infantry instead of shooting
	// at them directly. Whilst we are attacking an infantry unit with splash this will be called
	// regularly on a timer to update the location of the target. Note that this may be called
	// while another attack is already in progress, either normal or splash.
	virtual void attackLocation ( GameObject* obj, Vector3 location, bool primary );

	// This is called when the target is no longer valid and the turret should stop attacking, this
	// is called to stop both attackTarget and splashLocation attacks.
	virtual void stopAttacking ( GameObject* obj );
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Popup Turret AI
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_basedefences
*
* An overloaded version of my custom turret AI incorporating popup/deploy logic. This script requires
* two LevelEdit presets to work correctly, which are as follows;
* 
* <b>Turret Preset</b>
* As with any turret preset, this has all the weapons, health, armour, etc etc and should have an
* instance of this script attached to it with the relevant parameters configured (see below).
*
* <b>Spotter Preset</b>
* This is a dummy preset used to 'see' enemies whilst the real turret is undeployed. This should be
* setup as a turret but only the sight range parameter needs to be configured properly. It's model
* can be set to any valid model (or possibly left blank?) and it should NOT have ANY scripts. Note
* this preset can be shared by multiple turret presets as long as the sight range is appropriate.
*
* \param Priority_Infantry
*   Base targetting priority for infantry targets, or 0 to ignore infantry
* \param Weapon_Infantry
*   Weapon to use against infantry targets: 0 for primary fire, 1 for secondary fire
* \param Splash_Infantry
*   Determines if we should try to damage infantry with splash instead of hitting them directly.
*   This is useful for slow / inaccurate weapons which do splash damage: 1 to enable, 0 to disable
* \param Priority_Light_Vehicle
*   Base targetting priority for light vehicle targets, or 0 to ignore light vehicles
* \param Weapon_Light_Vehicle
*   Weapon to use against light vehicle targets: 0 for primary fire, 1 for secondary fire
* \param Priority_Heavy_Vehicle
*   Base targetting priority for heavy vehicle targets, or 0 to ignore heavy vehicles
* \param Weapon_Heavy_Vehicle
*   Weapon to use against heavy vehicle targets: 0 for primary fire, 1 for secondary fire
* \param Priority_VTOL
*   Base targetting priority for flying targets, or 0 to ignore flying targets
* \param Weapon_VTOL
*   Weapon to use against flying targets: 0 for primary fire, 1 for secondary fire
* \param Min_Attack_Range
*   Minimum distance at which the turret can engage enemies when using primary fire
* \param Max_Attack_Range
*   Maximum distance at which the turret can engage enemies when using primary fire
* \param Min_Attack_Range_Secondary
*   Minimum distance at which the turret can engage enemies when using secondary fire
* \param Max_Attack_Range_Secondary
*   Maximum distance at which the turret can engage enemies when using secondary fire
* \param Deploy_Animation
*   Name of the animation to play when the turret deploys - this is played in reverse for undeploy
* \param Deploy_Animation_Frames
*   Number of animation frames in the deployment animation, which should start at frame 0
* \param Deploy_Sound
*   An optional sound effect to be played whilst the deployment occurs
* \param Deploy_Timeout
*   Time the turret will wait after it last saw an enemy before it undeploys, in seconds
* \param Spotter_Preset
*   Name of your Spotter Preset in LevelEdit
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
* \note
*   Vehicles are always classified as light vehicles unless they have the dp88_AI_heavyVehicleMarker
*   script attached to them.
*
* \warning
*   Never leave the Debug parameter enabled when releasing your mod, it will clog up everyones
*   machines with useless text files everywhere... people usually don't like that :D
*/
class dp88_AI_PopupTurret : public dp88_AI_Turret
{
public:
  // Game events
  virtual void Created ( GameObject* pSelf );
  virtual void Timer_Expired ( GameObject* pSelf, int number );
  virtual void Custom ( GameObject* pSelf, int type, int param, GameObject* pSender );
  virtual void Animation_Complete ( GameObject* pSelf, const char* animation_name );
  virtual void Destroyed( GameObject* pSelf );


protected:
  static const unsigned char    STATE_UNDEPLOYED  = 0x00;
  static const unsigned char    STATE_DEPLOYING   = 0x01;
  static const unsigned char    STATE_DEPLOYED    = 0x02;
  static const unsigned char    STATE_UNDEPLOYING = 0x03;

  /*! Current deployment state */
  unsigned char m_deploymentState;

  /*! Time at which we will undeploy unless we see a target in the meantime */
  time_t m_undeployTime;

  /*! ID of the spotter turret we have created for ourselves */
  int m_spotterId;


  /*! Overloaded form of dp88_AI_Turret::attackTarget to check deployment state prior to initiating attack */
  virtual void attackTarget ( GameObject* pSelf, GameObject* pTarget, bool primary );

  /*! Overloaded form of dp88_AI_Turret::attackLocation to check deployment state prior to initiating attack */
  virtual void attackLocation ( GameObject* pSelf, Vector3 location, bool primary );


  /*! Function to initiate the deployment sequence if it is not already in process, this is called
  when we want to start shooting things */
  virtual void Deploy ( GameObject* pSelf );

  /*! Function to initiate the undeployment sequence if it is not already in process, this is called
  when we are bored because we haven't had anything to shoot at for a while...*/
  virtual void Undeploy ( GameObject* pSelf );
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Popup Turret Spotter
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* A simple companion script for dp88_AI_PopupTurret which is attached to an invisible clone of the
* turret to allow it to 'see' enemies when whilst the muzzle is underground. Simply fires off a
* custom event to the parent turret each time it sees an enemy with the ID of that enemy.
*/
class dp88_AI_PopupTurret_Spotter : public ScriptImpClass
{
  void Enemy_Seen ( GameObject* pSelf, GameObject* pEnemy );
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Charged Turret AI
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_basedefences
*
* An overloaded version of my custom turret AI incorporating charge up logic, which causes an
* animation sequence to be played prior to each clip fired by the turret. For maximum flexibility
* the animation can be played on either the turrets own model or on a seperate preset which is
* attached by this script to a specified bone on the turret model. This allows damage and charge
* up animations to be used on a turret simultaneously.
*
* One notable feature is the ability to fire more than one shot each time the turret charges up,
* allowing for greater flexibility in usage. When the turret is fully charged it will fire at the
* enemy until it runs out of bullets and needs to reload, at which point it will restart the charge
* up animation.
*
* \note
*   The weapon reload time <b>must</b> be less than the duration of the charging animation
*
* \warning
*   Currently there is a design limitation in the script which requires that the weapon reload time
*   must be at least 1.1 seconds in duration to ensure correct detection of the discharged state. If
*   this is problematic in future the limitation can be removed. If your reload time is less than 1
*   second the turret could empty two or more complete clips of ammo before charging again.
*
* \param Priority_Infantry
*   Base targetting priority for infantry targets, or 0 to ignore infantry
* \param Weapon_Infantry
*   Weapon to use against infantry targets: 0 for primary fire, 1 for secondary fire
* \param Splash_Infantry
*   Determines if we should try to damage infantry with splash instead of hitting them directly.
*   This is useful for slow / inaccurate weapons which do splash damage: 1 to enable, 0 to disable
* \param Priority_Light_Vehicle
*   Base targetting priority for light vehicle targets, or 0 to ignore light vehicles
* \param Weapon_Light_Vehicle
*   Weapon to use against light vehicle targets: 0 for primary fire, 1 for secondary fire
* \param Priority_Heavy_Vehicle
*   Base targetting priority for heavy vehicle targets, or 0 to ignore heavy vehicles
* \param Weapon_Heavy_Vehicle
*   Weapon to use against heavy vehicle targets: 0 for primary fire, 1 for secondary fire
* \param Priority_VTOL
*   Base targetting priority for flying targets, or 0 to ignore flying targets
* \param Weapon_VTOL
*   Weapon to use against flying targets: 0 for primary fire, 1 for secondary fire
* \param Min_Attack_Range
*   Minimum distance at which the turret can engage enemies when using primary fire
* \param Max_Attack_Range
*   Maximum distance at which the turret can engage enemies when using primary fire
* \param Min_Attack_Range_Secondary
*   Minimum distance at which the turret can engage enemies when using secondary fire
* \param Max_Attack_Range_Secondary
*   Maximum distance at which the turret can engage enemies when using secondary fire
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
* \note
*   Vehicles are always classified as light vehicles unless they have the dp88_AI_heavyVehicleMarker
*   script attached to them.
*
* \warning
*   Never leave the Debug parameter enabled when releasing your mod, it will clog up everyones
*   machines with useless text files everywhere... people usually don't like that :D
*/
class dp88_AI_ChargedTurret : public dp88_AI_Turret
{
public:
  // Game events
  virtual void Created ( GameObject* pSelf );
  virtual void Timer_Expired ( GameObject* pSelf, int number );
  virtual void Custom ( GameObject* pSelf, int type, int param, GameObject* pSender );
  virtual void Animation_Complete ( GameObject* pSelf, const char* animation_name );
  virtual void Destroyed( GameObject* pSelf );


protected:
  /*! Are we currently charging ready for an attack? */
  bool m_bIsCharging;

  /*! Are we currently discharging (ie: attacking an enemy)? */
  bool m_bIsDischarging;

  /*! ID of the charge animation object we have created for ourselves */
  int m_chargeAnimObjId;

  /*! Power state last time it was checked - if this changes we need to update our animation */
  bool m_bPowerState;

  /*! A looped animation controller for the idle and low power state animations */
  LoopedAnimationController* m_pLoopedAnimCtrl;


  /*! Overloaded form of dp88_AI_Turret::attackTarget which initiates turret charging (if it is not
  * currently in progress) rather than actually attacking anything */
  virtual void attackTarget ( GameObject* pSelf, GameObject* pTarget, bool primary );

  /*! Overloaded form of dp88_AI_Turret::attackLocation which initiates turret charging (if it is
  * not currently in progress) rather than actually attacking anything */
  virtual void attackLocation ( GameObject* pSelf, Vector3 location, bool primary );


  /*! This function is called to initiate the charge up process, if it is not already in progress */
  virtual void StartCharging ( GameObject* pSelf );

  /*! This function is called when the turret is fully charged and is ready to fire at something */
  virtual void StartDischarging ( GameObject* pSelf );


  /*! Internal utility function to get a pointer to the object upon which the charge animation will
  * be applied - this will either be the turret itself or the seperate charge animation object if
  * one was specified in the script parameters */
  virtual GameObject* GetAnimationObject ( GameObject* pSelf );

  /*! Called when the turret is idle or unpowered to apply the correct animation frames */
  virtual void ApplyIdleAnimation ( GameObject* pSelf );
};

/*!
* \brief Chargeup Animation Detector
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* A script to allow detection of animations if the "separate object" feature is used
*/
class dp88_AI_ChargedTurret_Animation : public ScriptImpClass
{
  // Game events
  virtual void Animation_Complete ( GameObject* pSelf, const char* animation_name );
};

// -------------------------------------------------------------------------------------------------