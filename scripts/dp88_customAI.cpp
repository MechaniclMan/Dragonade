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

// Include files
#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "dp88_customAI.h"
#include "dp88_custom_timer_defines.h"
#include "VehicleGameObj.h"

/*------------------------
Base class for custom AI's
--------------------------*/
void dp88_customAI::Created( GameObject *obj )
{
  // Note: Init is done in a seperate function because derived classes might want to pass different
  // parameters to loadSettings, thus we cannot assume they will call this function
  loadSettings(obj, true, false);
  Init(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_customAI::Custom ( GameObject* pObj, int message, int param, GameObject* pSender )
{
  if ( message == CUSTOM_AI_DISABLEAI && m_bAiEnabled )
  {
    m_bAiEnabled = false;
    AIStateChanged(pObj, false);
  }

  else if ( message == CUSTOM_AI_ENABLEAI && !m_bAiEnabled )
  {
    m_bAiEnabled = true;
    AIStateChanged(pObj, true);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_customAI::AIStateChanged( GameObject* pObj, bool bEnabled )
{
  if ( bEnabled )
    Commands->Innate_Enable( pObj );
  else
    Commands->Innate_Disable( pObj );
  Commands->Enable_Enemy_Seen( pObj, bEnabled );
}

// -------------------------------------------------------------------------------------------------

void dp88_customAI::Action_Complete( GameObject *obj, int action_id, ActionCompleteReason complete_reason )
{
  if ( action_id == 2 )
  {
    targetID = 0;
    targetPriority = 0.0f;
    Commands->Action_Reset( obj, 100 );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_customAI::Init( GameObject *obj )
{
  // Set default values
  m_bAiEnabled    = true;
  targetID        = 0;
  targetLastSeen  = 0;
  targetPriority  = 0.0f;
  primary_target  = true;

  // Set state
  Commands->Enable_Hibernation( obj, false );
  Commands->Innate_Enable( obj );
  Commands->Enable_Enemy_Seen( obj, true );
  Commands->Enable_Vehicle_Transitions( obj, false );
}

// -------------------------------------------------------------------------------------------------

void dp88_customAI::loadSettings ( GameObject *obj, bool loadSecondaryFireSettings, bool loadBuildingTargetSettings )
{
  // Load base target type priorities
  priority_infantry       = Get_Float_Parameter("Priority_Infantry");
  priority_lightVehicle   = Get_Float_Parameter("Priority_Light_Vehicle");
  priority_heavyVehicle   = Get_Float_Parameter("Priority_Heavy_Vehicle");
  priority_VTOL           = Get_Float_Parameter("Priority_VTOL");
  priority_building       = ( loadBuildingTargetSettings ) ? Get_Float_Parameter("Priority_Building") : 0;


  // Load priority modifiers
  modifier_distance       = Get_Float_Parameter("Modifier_Distance");
  modifier_target_damage  = Get_Float_Parameter("Modifier_Target_Damage");
  modifier_target_value   = Get_Float_Parameter("Modifier_Target_Value");


  // Load primary / secondary fire states for target types
  primary_infantry        = ( loadSecondaryFireSettings ) ? (( Get_Int_Parameter ( "Weapon_Infantry" ) == 2 )     ? false : true ) : true;
  primary_lightVehicle    = ( loadSecondaryFireSettings ) ? (( Get_Int_Parameter("Weapon_Light_Vehicle") == 2)    ? false : true ) : true;
  primary_heavyVehicle    = ( loadSecondaryFireSettings ) ? (( Get_Int_Parameter("Weapon_Heavy_Vehicle") == 2)    ? false : true ) : true;
  primary_VTOL            = ( loadSecondaryFireSettings ) ? (( Get_Int_Parameter ( "Weapon_VTOL" ) == 2 )         ? false : true ) : true;
  primary_building        = ( loadSecondaryFireSettings && loadBuildingTargetSettings ) ? (( Get_Int_Parameter ( "Weapon_Building" ) == 2 ) ? false : true) : true;


  // Get min and max ranges
  primary_minRange        = Get_Int_Parameter ( "Min_Attack_Range" );
  primary_maxRange        = Get_Int_Parameter ( "Max_Attack_Range" );
  secondary_minRange      = ( loadSecondaryFireSettings ) ? Get_Int_Parameter ( "Min_Attack_Range_Secondary" ) : 0;
  secondary_maxRange      = ( loadSecondaryFireSettings ) ? Get_Int_Parameter ( "Max_Attack_Range_Secondary" ) : 0;

  // Load other settings
  m_bCanDetectStealth     = (Get_Int_Parameter("Detects_Stealth")>0);

  // If debugging is enabled open the debug file
  debug = ( Get_Int_Parameter("Debug") == 1 );
  if ( debug )
  {
    StringClass filename(true);
    filename.Format ( "%s_%d_%d.log", Commands->Get_Preset_Name(obj), Commands->Get_ID(obj), (int)time(NULL) );
    debugFile = fopen(filename.Peek_Buffer(),"w");
  }
}

// -------------------------------------------------------------------------------------------------

float dp88_customAI::getDistance ( GameObject *obj1, GameObject *obj2 )
{
  // Gets distance between two objects
  return Commands->Get_Distance ( Commands->Get_Position ( obj1 ), Commands->Get_Position ( obj2 ) );
}

// -------------------------------------------------------------------------------------------------

float dp88_customAI::getPriority( GameObject *obj, GameObject *target )
{
  if ( !obj || !target )
    return 0.0f;

  if ( debug ) fprintf ( debugFile, "Calculating priority of %d (%s)\n", Commands->Get_ID(target), Commands->Get_Preset_Name(target) );

  // Priority is 0 if the unit is dead or not on teams 0 or 1
  if ( ( Commands->Get_Health ( target ) + Commands->Get_Shield_Strength ( target ) ) == 0
    || ( Commands->Get_Player_Type ( target ) != 0 && Commands->Get_Player_Type ( target ) != 1 ) )
  {
    if ( debug ) fprintf ( debugFile, "Target %d is dead or unteamed, ignoring\n", Commands->Get_ID(target));
    return 0.0;
  }

  if ( target->As_VehicleGameObj() && !IsVehicleAIEnabled(target->As_VehicleGameObj()) && IsVehicleEmpty(target->As_VehicleGameObj()) )
  {
    if ( debug ) fprintf ( debugFile, "Target %d is an empty, non-AI vehicle, ignoring\n", Commands->Get_ID(target));
    return 0.0;
  }

  if ( !m_bCanDetectStealth && target->As_SmartGameObj() && target->As_SmartGameObj()->Is_Stealthed() )
  {
    if ( debug ) fprintf ( debugFile, "Target %d is currently stealthed and stealth detection is disabled\n", Commands->Get_ID(target));
    return 0.0;
  }

  float priority = 0.0;


  // Assign base priority
  if ( Get_Vehicle_Mode(target) == VEHICLE_TYPE_FLYING || (target->As_SoldierGameObj() && (Get_Fly_Mode(target)||Find_Script_On_Object(target,"dp88_AR_Paradrop")) ) )
    priority = (float)priority_VTOL;
  else if ( target->As_SoldierGameObj() )
    priority = (float)priority_infantry;
  else if ( target->As_VehicleGameObj() && Is_Script_Attached ( target, "dp88_AI_heavyVehicleMarker" ) )
    priority = (float)priority_heavyVehicle;
  else if ( target->As_VehicleGameObj() )
    priority = (float)priority_lightVehicle;
  else if ( target->As_BuildingGameObj() )
    priority = (float)priority_building;

  // Return if our base priority for this target type is 0
  if ( priority == 0.0f )
  {
    if ( debug ) fprintf ( debugFile, "\tInvalid target type, priority is 0.0\n" );
    return 0.0f;
  }

  if ( debug ) fprintf ( debugFile, "\tBase priority for this unit type is %.2f\n", priority );


  // Apply target value modifier (add X priority for every 10 credits the target
  // cost to purchase - not applicable to buildings)
  if ( !target->As_BuildingGameObj() )
  {
    int cost = Get_Team_Cost( Commands->Get_Preset_Name ( target ), Commands->Get_Player_Type ( target ) );
    priority += ((float)cost/10.0f) * modifier_target_value;

    if ( debug ) fprintf ( debugFile, "\tTarget Value Modifier: Adding %.4f priority ((Value %d /10) * modifier %.2f)\n", ((float)cost/10.0f)*modifier_target_value, cost, modifier_target_value );
  }


  // Apply target damage modifier (add X priority for 10 points of damage the
  // target has already taken from their maximum health/armour)
  float totalHitPts = Commands->Get_Health ( target ) + Commands->Get_Shield_Strength ( target );
  float maxHitPts = Commands->Get_Max_Health ( target ) + Commands->Get_Max_Shield_Strength ( target );
  priority += ((maxHitPts-totalHitPts)/10) * modifier_target_damage;

  if ( debug ) fprintf ( debugFile, "\tTarget Damage Modifier: Adding %.4f priority ((Target Damage Taken %.2f) /10) * modifier %.2f)\n", ((maxHitPts-totalHitPts)/10)*modifier_target_damage, maxHitPts-totalHitPts, modifier_target_damage );


  // Apply target distance modifier (subtract X distance for every 10 units of
  //  distancebetween this object and the target)
  float distance = Commands->Get_Distance ( Commands->Get_Position ( obj ), Commands->Get_Position ( target ) );
  priority -= (distance/10) * modifier_distance;

  if ( debug ) fprintf ( debugFile, "\tDistance Modifier: Subtracting %.4f priority ((Distance to target %.2f) /10) * modifier %.2f)\n", (distance/10)*modifier_distance, distance, modifier_distance );


  // Ensure priority is above 0
  if ( debug ) fprintf ( debugFile, "\tFinal Priority: %.4f\n", priority );
  if ( priority <= 0.0 )
  {
    priority = 0.0001f;
    if ( debug ) fprintf ( debugFile, "\tAdjusting final priority to 0.0001 (must be >0)\n" );
  }


  // Return result
  return priority;
}

// -------------------------------------------------------------------------------------------------

float dp88_customAI::getPriority ( GameObject *obj, int target_id )
{
  // Overloaded function to accept object ID's
  return getPriority ( obj, Commands->Find_Object ( target_id ) );
}

// -------------------------------------------------------------------------------------------------

bool dp88_customAI::getPrimary ( GameObject *target )
{
  if ( target->As_SoldierGameObj() )
    return primary_infantry;
  else if ( Get_Vehicle_Mode ( target ) == VEHICLE_TYPE_FLYING )
    return primary_VTOL;
  else if ( target->As_VehicleGameObj() )
    return (Is_Script_Attached(target,"dp88_AI_heavyVehicleMarker")) ? primary_heavyVehicle : primary_lightVehicle;
  else if ( target->As_BuildingGameObj() )
    return primary_building;
  return true;
}

// -------------------------------------------------------------------------------------------------

bool dp88_customAI::IsVehicleEmpty( VehicleGameObj* vobj )
{
  if ( vobj == NULL )
    return false;
  if ( vobj->Get_Definition().Get_Seat_Count() == 0)
    return false;
  return (vobj->Get_Occupant_Count() == 0);
}

// -------------------------------------------------------------------------------------------------

bool dp88_customAI::IsVehicleAIEnabled( VehicleGameObj* vobj )
{
  if ( vobj == NULL )
    return false;
  return (vobj->Get_Action()->Is_Acting());
}








/*------------------------
Offensive Tank AI
--------------------------*/

void dp88_AI_Tank_Offensive::Created( GameObject *obj )
{
  // Load settings and init
  loadSettings( obj, true, true );
  Init(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_Tank_Offensive::Init( GameObject *obj )
{
  dp88_customAI::Init(obj);

  m_bMovingToTarget = false;
  m_pCurrentObjective = NULL;

  // Start timer which runs for the lifetime of this object - this belongs in Init
  Commands->Start_Timer ( obj, this, 1.0, TIMER_CUSTOMAI_THINK );
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_Tank_Offensive::loadSettings( GameObject *obj, bool loadSecondaryFireSettings, bool loadBuildingTargetSettings )
{
  dp88_customAI::loadSettings(obj, loadSecondaryFireSettings, loadBuildingTargetSettings);

  primary_prefRange = Get_Int_Parameter ( "Preferred_Attack_Range" );
  secondary_prefRange = Get_Int_Parameter ( "Preferred_Attack_Range_Secondary" );
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_Tank_Offensive::Enemy_Seen ( GameObject *obj, GameObject *enemy )
{
  // If we saw a soldier in a vehicle, switch the GameObject to that of the vehicle
  GameObject *o = Get_Vehicle(enemy);
  if ( o )
    enemy = o;

  //fprintf( debugFile, "Seen enemy %s\n", Commands->Get_Preset_Name(enemy) );

  // If this is our current target update it's last seen time and priority
  if ( Commands->Get_ID ( enemy ) == targetID )
  {
    targetLastSeen = (int)time(NULL);
    targetPriority = getPriority(obj, targetID);
    return;
  }

  // Get priority for seen object and return if 0 or lower than current target
  float seenPriority = getPriority( obj, enemy );
  if ( seenPriority == 0.0 || (targetID!=0 && seenPriority <= targetPriority) )
    return;

  //fprintf( debugFile, "Higher priority than existing target... attacking!\n" );

  // Decide if we want to use primary or secondary fire
  bool attackPrimary = getPrimary ( enemy );

  // Get distance to target, and return if they are too close
  float targetDistance = getDistance ( obj, enemy );
  if ( (attackPrimary && targetDistance < primary_minRange) || (!attackPrimary && targetDistance < secondary_minRange) )
    return;

  //fprintf( debugFile, "Range to enemy: %d\n", (int)targetDistance );

  // All good, attack the enemy
  AttackTarget ( obj, enemy );
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_Tank_Offensive::Timer_Expired( GameObject *obj, int number )
{
  // Check target is still alive, still an enemy (eg: stolen vehicles) and in range
  if ( number == TIMER_CUSTOMAI_THINK )
  {
    if ( targetID != 0 )
    {
      //fprintf( debugFile, "Checking target validity...\n" );
      GameObject *target = Commands->Find_Object ( targetID );

      if ( ( Commands->Get_Health ( target ) + Commands->Get_Shield_Strength ( target ) <= 0 )
        || Commands->Get_Player_Type ( target ) == 2
        || Commands->Get_Player_Type ( target ) == Commands->Get_Player_Type ( obj )
        || ( primary_target && getDistance ( obj, target ) > primary_maxRange )
        || ( primary_target && getDistance ( obj, target ) < primary_minRange )
        || ( !primary_target && getDistance ( obj, target ) > secondary_maxRange )
        || ( !primary_target && getDistance ( obj, target ) < secondary_minRange )
        || (int)time(NULL) - targetLastSeen > 3 )	// Not seen target for 3 seconds
      {
        //fprintf( debugFile, "Target invalid, aborting.. \n" );
        targetID = 0;
        m_bMovingToTarget = false;
        Commands->Action_Reset ( obj, 100 );
      }

      // Chase after them if they are running away...
      if ( !m_bMovingToTarget )
      {
        float targetDistance = getDistance ( obj, target );
        int prefDistance = ((primary_target)?primary_prefRange:secondary_prefRange);
        if ( targetDistance > prefDistance )
        {
          ActionParamsStruct params;
          Commands->Get_Action_Params(obj, params);
          params.Set_Movement( target, 1.0f, (float)prefDistance );;
          Commands->Modify_Action(obj,ACTION_ID_ATTACK_ENEMY,params,true,true);
        }
      }
    }


    // Verify current objective (if any) is still valid
    if ( m_pCurrentObjective && !dp88_AI_Objective::IsValidObjective(m_pCurrentObjective) )
    {
      m_pCurrentObjective = NULL;
      m_bMovingToTarget = false;
    }


    if ( targetID == 0 && !m_bMovingToTarget )
    {
      GoToObjective(obj);
    }

    // Timer always runs
    Commands->Start_Timer ( obj, this, 1.0, TIMER_CUSTOMAI_THINK );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_Tank_Offensive::AttackTarget ( GameObject* obj, GameObject* target )
{
  // Clear any existing action
  if (targetID == 0 )
    Commands->Action_Reset(obj,100);

  m_bMovingToTarget = false;
  targetID = Commands->Get_ID(target);
  primary_target = getPrimary(target);
  targetLastSeen = (int)time(NULL);

  // Build a new action struct
  ActionParamsStruct params;
  params.Set_Basic( this, 90, ACTION_ID_ATTACK_ENEMY );
  params.Set_Attack( target, (float)((primary_target)?primary_maxRange:secondary_maxRange), 0.0, true);
  params.AttackCheckBlocked = false;

  float targetDistance = getDistance ( obj, target );
  int prefDistance = ((primary_target)?primary_prefRange:secondary_prefRange);
  if ( targetDistance > prefDistance )
    params.Set_Movement( target, 1.0f, (float)prefDistance );

  Commands->Action_Attack( obj, params );
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_Tank_Offensive::GoToObjective ( GameObject* obj )
{
  // Verify current objective (if any) is still valid
  if ( m_pCurrentObjective && !dp88_AI_Objective::IsValidObjective(m_pCurrentObjective) )
    m_pCurrentObjective = NULL;

  // Obtain a new objective if we don't have one
  if ( m_pCurrentObjective == NULL )
  {
    m_pCurrentObjective = dp88_AI_Objective::GetObjective(obj,dp88_AI_Objective::TYPE_OFFENSIVE);
    if ( !m_pCurrentObjective )
      m_pCurrentObjective = dp88_AI_Objective::GetObjective(obj,dp88_AI_Objective::TYPE_DEFENSIVE);
  }

  if ( m_pCurrentObjective != NULL )
  {
    m_bMovingToTarget = true;

    ActionParamsStruct params;
    params.Set_Basic( this, 80, ACTION_ID_MOVE_TO_OBJECTIVE );
    params.Set_Movement( m_pCurrentObjective->GetGameObject(), 1.0f, (float)m_pCurrentObjective->GetDistance() );
    Commands->Action_Goto ( obj, params );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_Tank_Offensive::Action_Complete ( GameObject *obj, int action_id, ActionCompleteReason reason)
{
  if ( action_id == ACTION_ID_ATTACK_ENEMY )
  {
    if ( reason == MOVEMENT_COMPLETE_ARRIVED )
    {
      AttackTarget(obj, Commands->Find_Object(targetID));
    }
  }

  else if ( action_id == ACTION_ID_MOVE_TO_OBJECTIVE )
  {
    m_bMovingToTarget = false;
  }
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_AI_Tank_Offensive> dp88_AI_Tank_Offensive_Registrant(
  "dp88_AI_Tank_Offensive",
  "Priority_Infantry=1.0:float,"
  "Weapon_Infantry=1:int,"
  "Splash_Infantry=0:int,"
  "Priority_Light_Vehicle=5.0:float,"
  "Weapon_Light_Vehicle=1:int,"
  "Priority_Heavy_Vehicle=7.0:float,"
  "Weapon_Heavy_Vehicle=1:int,"
  "Priority_VTOL=5.0:float,"
  "Weapon_VTOL=1:int,"
  "Priority_Building=12.0:float,"
  "Weapon_Building=1:int,"
  "Max_Attack_Range=150:int,"
  "Min_Attack_Range=0:int,"
  "Preferred_Attack_Range=60:int,"
  "Max_Attack_Range_Secondary=150:int,"
  "Min_Attack_Range_Secondary=0:int,"
  "Preferred_Attack_Range_Secondary=60:int,"
  "Modifier_Distance=0.25:float,"
  "Modifier_Target_Damage=0.1:float,"
  "Modifier_Target_Value=0.05:float,"
  "Debug=0:int,"
  "Detects_Stealth=1:int"
);








/*------------------------
Turret AI
--------------------------*/

void dp88_AI_Turret::Init( GameObject *obj )
{
  dp88_customAI::Init( obj );

  // Start timer which runs for the lifetime of this object
  Commands->Start_Timer ( obj, this, 1.0, TIMER_CUSTOMAI_THINK );
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_Turret::loadSettings(  GameObject *obj, bool loadSecondaryFireSettings, bool loadBuildingTargetSettings )
{
  dp88_customAI::loadSettings(obj, loadSecondaryFireSettings, loadBuildingTargetSettings);

  requiresPower = (Get_Int_Parameter("Requires_Power") > 0 ) ? true: false;
  splashInfantry = (Get_Int_Parameter("Splash_Infantry") > 0 ) ? true: false;
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_Turret::Enemy_Seen ( GameObject *obj, GameObject *enemy )
{
  // If we saw a soldier in a vehicle, switch the GameObject to that of the vehicle
  enemy = ( Get_Vehicle(enemy) ) ? Get_Vehicle(enemy) : enemy;
  if ( debug ) fprintf( debugFile, "Seen an enemy: %d (%s)\n", Commands->Get_ID(enemy), Commands->Get_Preset_Name(enemy) );

	// If this is our current target update last seen time and priority
	if ( Commands->Get_ID ( enemy ) == targetID )
	{
		if ( debug ) fprintf( debugFile, "Enemy is current target, last seen time and priority updated\n" );
		targetLastSeen = (int)time(NULL);
		targetPriority = getPriority(obj, enemy);
		return;
	}

	// If base power is offline and we require power then abort
	if ( !checkPowerState(obj) )
	{
		if ( debug ) fprintf( debugFile, "Unable to attack, no power available..." );
		return;
	}


	// Determine if we want to use primary or secondary fire on this target
	bool attackPrimary = getPrimary ( enemy );
	if ( debug ) fprintf( debugFile, "Using %s fire on this target type\n", (attackPrimary)?"primary":"secondary" );


	// Check if the target is between our min and max firing ranges
	if ( !checkRange(obj, enemy, attackPrimary ) )
	{
		if ( debug ) fprintf( debugFile, "Unable to attack, target out of range\n" );
		return;
	}


	// Determine the priority of the target
	float enemyPriority = getPriority( obj, enemy );
	if ( enemyPriority == 0.0 )
		return;


	// If this is a higher priority than our current target attack it
	if ( targetID == 0 || enemyPriority > targetPriority )
	{
		if ( debug && targetID == 0 ) fprintf ( debugFile, "No current target to compare with, attacking enemy!\n" );
		else if ( debug ) fprintf ( debugFile, "New enemy has a higher priority than current target, attacking enemy\n" );

		targetID = Commands->Get_ID ( enemy );
		targetLastSeen = (int)time(NULL);
		targetPriority = enemyPriority;
		primary_target = attackPrimary;

		if ( splashInfantry && enemy->As_SoldierGameObj() )
		{
			if ( debug ) fprintf ( debugFile, "Using splash damage against this target\n" );
			attackLocation(obj,Commands->Get_Position(enemy),primary_target);
		}
		else
			attackTarget(obj,enemy,primary_target);
	}
	else if ( debug ) fprintf ( debugFile, "Current target has a higher priority than new enemy, continuing to attack current target\n" );
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_Turret::Timer_Expired( GameObject *obj, int number )
{
	// Check target is still alive, still an enemy (eg: stolen vehicles) and in range
	if ( number == TIMER_CUSTOMAI_THINK )
	{
		if ( targetID != 0 )
		{
			GameObject *target = Commands->Find_Object ( targetID );
			if (!target || !checkPowerState(obj)
				|| ( Commands->Get_Health ( target ) + Commands->Get_Shield_Strength ( target ) <= 0 )
				|| !checkTeam ( obj, target )
				|| !checkRange ( obj, target, primary_target )
				|| (int)time(NULL) - targetLastSeen > 3
				|| ( target->As_VehicleGameObj() && !IsVehicleAIEnabled(target->As_VehicleGameObj()) && IsVehicleEmpty(target->As_VehicleGameObj()) ))
			{
				if ( debug ) fprintf ( debugFile, "Target %d no longer valid, ceasing attack.\n", targetID);
				targetID = 0;
				targetPriority = 0.0f;
				targetLastSeen = 0;
				stopAttacking(obj);
			}

			// Otherwise we are OK to continue attacking... if this is an infantry unit
			// and splash targetting is enabled then update targetting position
			else if ( splashInfantry && target->As_SoldierGameObj() )
				attackLocation ( obj, Commands->Get_Position(target), primary_target );
		}

		// Timer always runs
		Commands->Start_Timer ( obj, this, 1.0, TIMER_CUSTOMAI_THINK );
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_Turret::AIStateChanged( GameObject* pObj, bool bEnabled )
{
  dp88_customAI::AIStateChanged(pObj, bEnabled);

  if ( !bEnabled )
    stopAttacking(pObj);
}

// -------------------------------------------------------------------------------------------------

// Returns true if the target is on the enemy team, false otherwise
bool dp88_AI_Turret::checkTeam ( GameObject* obj, GameObject* target )
{
	return !( Commands->Get_Player_Type ( target ) == 2 || Commands->Get_Player_Type ( target ) == Commands->Get_Player_Type ( obj ) );
}

// -------------------------------------------------------------------------------------------------

// Returns true if the target is between our minimum and maximum range for either primary
// or secondary weapons
bool dp88_AI_Turret::checkRange ( GameObject* obj, GameObject* target, bool primary )
{
	// Get distance to target
	float distance = getDistance ( obj, target );
	if ( (primary && distance < primary_minRange) || (!primary && distance < secondary_minRange) )
	{
		if ( debug ) fprintf( debugFile, "Target is too close to attack (distance: %.3f < min range: %d)\n", distance, (primary)?primary_minRange:secondary_minRange );
		return false;
	}
	if ( (primary && distance > primary_maxRange) || (!primary && distance > secondary_maxRange) )
	{
		if ( debug ) fprintf( debugFile, "Target is too far away to attack (distance: %.3f > max range: %d)\n", distance, (primary)?primary_maxRange:secondary_maxRange );
		return false;
	}

	return true;
}

// -------------------------------------------------------------------------------------------------

// Returns true if the base is powered or the defence does not require power
bool dp88_AI_Turret::checkPowerState ( GameObject* obj )
{
	return ( !requiresPower || Is_Base_Powered(Get_Object_Type(obj)) );
}

// -------------------------------------------------------------------------------------------------

// These is called when a valid target has been identified and selected as the highest priority
// target, the turret should begin attacking in this function. Note that this may be called
// while another attack is already in progress, either normal or splash.
void dp88_AI_Turret::attackTarget ( GameObject* obj, GameObject* target, bool primary )
{
	// Reset any current attack (note: specifically use the function from THIS
	// class and not a derived class as we do not know what functionality may
	// be implemented in a derived class)
	dp88_AI_Turret::stopAttacking(obj);

	ActionParamsStruct params;
	params.Set_Basic( this, 100.0f, 8950 );
	params.Set_Attack( target, (primary)?(float)primary_maxRange:(float)secondary_maxRange, 0.0, primary);
	params.AttackCheckBlocked = false;
	Commands->Action_Attack( obj, params );
}

// -------------------------------------------------------------------------------------------------

// This is called instead of attackTarget when we are set to splash infantry instead of shooting
// at them directly. Whilst we are attacking an infantry unit with splash this will be called
// regularly on a timer to update the location of the target. Note that this may be called
// while another attack is already in progress, either normal or splash.
void dp88_AI_Turret::attackLocation ( GameObject* obj, Vector3 location, bool primary )
{
	ActionParamsStruct params;
	Commands->Get_Action_Params(obj,params);
	
	// If we are already using splash on a target then we just want to modify the action (if
	// priority is 0 the action has been completed or reset, so we don't want to use modify)
	if ( params.ActionID == 8951 && params.Priority > 0.0f )
	{
		Commands->Get_Action_Params(obj,params);
		params.Set_Attack( location, (primary)?(float)primary_maxRange:(float)secondary_maxRange, 0.0, primary);
		Commands->Modify_Action(obj,8951,params,false,true);
	}

	// Otherwise setup a new action
	else
	{
		// Reset any current attack (note: specifically use the function from THIS
		// class and not a derived class as we do not know what functionality may
		// be implemented in a derived class)
		dp88_AI_Turret::stopAttacking(obj);

		params.Set_Basic( this, 100.0f, 8951 );
		params.Set_Attack( location, (primary)?(float)primary_maxRange:(float)secondary_maxRange, 0.0, primary);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack( obj, params );
	}
}

// -------------------------------------------------------------------------------------------------

// This is called when the target is no longer valid and the turret should stop attacking, this
// is called to stop both attackTarget and splashLocation attacks.
void dp88_AI_Turret::stopAttacking ( GameObject* obj )
{
  Commands->Action_Reset(obj,101.0f);
}








// -------------------------------------------------------------------------------------------------
//  Popup Turret AI
// -------------------------------------------------------------------------------------------------

void dp88_AI_PopupTurret::Created ( GameObject* pSelf )
{
  m_deploymentState = STATE_UNDEPLOYED;
  m_spotterId = 0;

  // Create our spotter turret
  GameObject* pSpotter = Commands->Create_Object ( Get_Parameter("Spotter_Preset"), Commands->Get_Position(pSelf) );
  if ( pSpotter )
  {
    m_spotterId = Commands->Get_ID(pSpotter);

    // Set the spotter turret to match our configuration, but invisible
    Commands->Set_Model ( pSpotter, Get_Model(pSelf) );
    Commands->Set_Is_Rendered ( pSpotter, false );
    Commands->Enable_Enemy_Seen ( pSpotter, true );
    Commands->Disable_All_Collisions ( pSpotter );
    Commands->Innate_Enable ( pSpotter );
    Set_Skin ( pSpotter, "Blamo" );

    // Attach the spotter script
    char scriptParams[20];
    sprintf_s ( scriptParams, sizeof(scriptParams), "%d", Commands->Get_ID(pSelf) );
    Commands->Attach_Script ( pSpotter, "dp88_AI_PopupTurret_Spotter", scriptParams );

    // 'Deploy' the spotter turret so it can see what we would be able to see whilst deployed
    Commands->Set_Animation_Frame ( pSpotter, Get_Parameter("Deploy_Animation"), Get_Int_Parameter("Deploy_Animation_Frames") );

	// Make sure the turret itself starts popped down
	Commands->Set_Animation_Frame ( pSelf, Get_Parameter("Deploy_Animation"), 0 );
  }

  dp88_AI_Turret::Created(pSelf);
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_PopupTurret::Timer_Expired ( GameObject* pSelf, int number )
{
  // Piggy-back the undeploy timer logic on the existing target check timer since its always running
  if ( number == TIMER_CUSTOMAI_THINK && m_deploymentState == STATE_DEPLOYED )
  {
    // If we have no target and have reached the undeploy timeout then trigger undeployment
    if ( targetID == 0 && time(NULL) > m_undeployTime )
      Undeploy(pSelf);
    
    // Else, if we have an active target, update the undeployment timeout time
    else if ( targetID != 0 )
      m_undeployTime = time(NULL)+Get_Int_Parameter("Deploy_Timeout");
  }

  dp88_AI_Turret::Timer_Expired(pSelf, number);
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_PopupTurret::Custom ( GameObject* pSelf, int type, int param, GameObject* pSender )
{
  if ( m_spotterId == Commands->Get_ID(pSender) )
  {
    // Spotter only ever sends us one type of message... enemy spotted! Note that we don't simply
    // deploy here because it might be a target we can't / shouldn't attack, let Enemy_Seen decide
    // if we actually need to do anything
    GameObject* pEnemy = Commands->Find_Object(param);
    if ( pEnemy )
      Enemy_Seen(pSelf, pEnemy);
  }
  else
    dp88_AI_Turret::Custom(pSelf, type, param, pSender);
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_PopupTurret::Animation_Complete ( GameObject* pSelf, const char* animation_name )
{
  if ( m_deploymentState == STATE_DEPLOYING )
  {
    m_deploymentState = STATE_DEPLOYED;
    m_undeployTime = time(NULL)+Get_Int_Parameter("Deploy_Timeout");

    // Reset targetting parameters, otherwise the next Enemy_Seen call will think we are already attacking
    // an enemy and not trigger the appropriate actions we are depending upon
    targetID = 0;
  }
  else if ( m_deploymentState == STATE_UNDEPLOYING )
    m_deploymentState = STATE_UNDEPLOYED;

  dp88_AI_Turret::Animation_Complete(pSelf, animation_name);
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_PopupTurret::Destroyed ( GameObject* pSelf )
{
  if ( GameObject* pSpotter = Commands->Find_Object(m_spotterId) )
    Commands->Destroy_Object(pSpotter);
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_PopupTurret::attackTarget ( GameObject* pSelf, GameObject* pTarget, bool primary )
{
  if ( m_deploymentState == STATE_DEPLOYED )
    dp88_AI_Turret::attackTarget(pSelf, pTarget, primary);
  else
    Deploy(pSelf); // No need to remember what we are deploying to shoot, enemy seen will be re-triggered
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_PopupTurret::attackLocation ( GameObject* pSelf, Vector3 location, bool primary )
{
  if ( m_deploymentState == STATE_DEPLOYED )
    dp88_AI_Turret::attackLocation(pSelf, location, primary);
  else
    Deploy(pSelf); // No need to remember what we are deploying to shoot, enemy seen will be re-triggered
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_PopupTurret::Deploy ( GameObject* pSelf )
{
  if ( m_deploymentState == STATE_UNDEPLOYED || m_deploymentState == STATE_UNDEPLOYING )
  {
    m_deploymentState = STATE_DEPLOYING;

    Commands->Set_Animation(pSelf,Get_Parameter("Deploy_Animation"),false,0,Get_Animation_Frame(pSelf),(float)Get_Int_Parameter("Deploy_Animation_Frames"),false);

    if (_stricmp(Get_Parameter("Deploy_Sound"),"0"))
      Commands->Create_Sound(Get_Parameter("Deploy_Sound"),Commands->Get_Position(pSelf),pSelf);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_PopupTurret::Undeploy ( GameObject* pSelf )
{
  if ( m_deploymentState == STATE_DEPLOYED )
  {
    m_deploymentState = STATE_UNDEPLOYING;

    Commands->Set_Animation(pSelf,Get_Parameter("Deploy_Animation"),false,0,(float)Get_Int_Parameter("Deploy_Animation_Frames"),0, false);

    if (_stricmp(Get_Parameter("Deploy_Sound"),"0"))
      Commands->Create_Sound(Get_Parameter("Deploy_Sound"),Commands->Get_Position(pSelf),pSelf);
  }
}







// ------------------------------------------------------------------------------------------------
//  Popup Turret Spotter
// ------------------------------------------------------------------------------------------------

void dp88_AI_PopupTurret_Spotter::Enemy_Seen ( GameObject* pSelf, GameObject* pEnemy )
{
  if ( GameObject* pTurret = Commands->Find_Object(Get_Int_Parameter("tId")) )
    Commands->Send_Custom_Event ( pSelf, pTurret, CUSTOM_TURRETAI_ENEMYSEEN, Commands->Get_ID(pEnemy), 0.0f );
}







// -------------------------------------------------------------------------------------------------
//  Charged Turret AI
// -------------------------------------------------------------------------------------------------

void dp88_AI_ChargedTurret::Init( GameObject* pSelf )
{
  dp88_AI_Turret::Init(pSelf);

  m_bIsCharging = false;
  m_bIsDischarging = false;
  m_chargeAnimObjId = NULL;

  // If a seperate charge animation model has been specified then go ahead and spawn it
  if ( strlen ( Get_Parameter("Animation_Model") ) > 0 )
  {
    GameObject* pChargeAnimObj = Commands->Create_Object ( "Invisible_Object", Commands->Get_Position(pSelf) );
    if ( pChargeAnimObj )
    {
      m_chargeAnimObjId = Commands->Get_ID(pChargeAnimObj);
      Commands->Set_Model ( pChargeAnimObj, Get_Parameter("Animation_Model") );
      Commands->Attach_To_Object_Bone ( pChargeAnimObj, pSelf, Get_Parameter("Animation_Model_Bone") );

      // Create the looped animation controller on the charge animation object
      m_pLoopedAnimCtrl = new LoopedAnimationController ( pChargeAnimObj );

      // Attach companion script so we get notified about animation complete events - we need to know
      // about these so we know when the charged animation is complete.
      //
      // \todo Could this be done by simply attaching this script as a listener on the charged
      //       animation object directly and checking the GameObject pointers we get on each
      //       event to check which object they came from? Not sure which is cleaner...
      char scriptParams[20];
      sprintf_s ( scriptParams, sizeof(scriptParams), "%d", Commands->Get_ID(pSelf) );
      Commands->Attach_Script ( pChargeAnimObj, "dp88_AI_ChargedTurret_Animation", scriptParams );
    }
  }

  // If we don't have a charge animation object create the looped animation controller on ourselves
  if ( m_chargeAnimObjId == NULL )
    m_pLoopedAnimCtrl = new LoopedAnimationController ( pSelf );

  m_bPowerState = checkPowerState(pSelf);
  ApplyIdleAnimation(pSelf);
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_ChargedTurret::loadSettings( GameObject* pSelf, bool loadSecondaryFireSettings, bool loadBuildingTargetSettings )
{
  dp88_AI_Turret::loadSettings(pSelf, loadSecondaryFireSettings, loadBuildingTargetSettings);

  // Not currently loading any settings... probably should add this sometime...
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_ChargedTurret::Timer_Expired ( GameObject* pSelf, int number )
{
  // Use the existing custom AI think timer to check for a base power state changes and, if it has
  // changed, update the idle animation (and abort any charge up in progress)
  if ( number == TIMER_CUSTOMAI_THINK && m_bPowerState != checkPowerState(pSelf) )
  {
    m_bPowerState = !m_bPowerState;

    // Charging state must always become false now - either we were unpowered to begin with or
    // we were powered and have now gone offline. Either way, we can't possibly be charging now.
    m_bIsCharging = false;

    // If we were discharging our weapon we need to stop now and trigger a reload so we don't
    // fire a partial charge next time around
    if ( m_bIsDischarging )
    {
      m_bIsDischarging = false;
      stopAttacking(pSelf);
      Set_Current_Bullets(pSelf, 0);
    }

    // Apply the appropriate idle animation
    ApplyIdleAnimation(pSelf);
  }


  // We might as well piggy back the discharged checks on the same timer too whilst we are at it...
  if ( number == TIMER_CUSTOMAI_THINK && m_bIsDischarging )
  {
    // Have we unloaded our entire clip? If so then we can start charging for the next attack
    if ( Get_Current_Bullets(pSelf) == 0 )
    {
      m_bIsDischarging = false;
      stopAttacking(pSelf);   // Don't want to fire again before we are ready!
      if ( targetID != NULL )
        StartCharging(pSelf);
      else
        ApplyIdleAnimation(pSelf);
    }

    // Has our target already died or ran like a coward before we could empty our clip? If so then
    // reset and trigger a reload (so we don't have a partial charge next time round)
    else if ( targetID == NULL )
    {
      m_bIsDischarging = false;
      Set_Current_Bullets(pSelf, 0);
      ApplyIdleAnimation(pSelf);
    }
  }

  dp88_AI_Turret::Timer_Expired(pSelf, number);
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_ChargedTurret::Animation_Complete ( GameObject* pSelf, const char* animation_name )
{
  if (!_stricmp(animation_name,Get_Parameter("Animation"))) //make sure we are running the right animation
  {
    StartDischarging(pSelf);
  }

  dp88_AI_Turret::Animation_Complete(pSelf, animation_name);
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_ChargedTurret::Destroyed ( GameObject* pSelf )
{
  if (m_pLoopedAnimCtrl)
  {
	delete m_pLoopedAnimCtrl;
  }
  
  if ( m_chargeAnimObjId )
  {
    if ( GameObject* pChargeAnimObj = Commands->Find_Object(m_chargeAnimObjId) )
      Commands->Destroy_Object(pChargeAnimObj);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_ChargedTurret::attackTarget ( GameObject* pSelf, GameObject* pTarget, bool primary )
{
  // Changing targets in the middle of discharging? Sure, why not...
  if ( m_bIsDischarging )
    dp88_AI_Turret::attackTarget ( pSelf, pTarget, primary );

  // Otherwise call StartCharging and let it determine what to do...
  StartCharging(pSelf);
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_ChargedTurret::attackLocation ( GameObject* pSelf, Vector3 location, bool primary )
{
  // Changing targets in the middle of discharging? Sure, why not...
  if ( m_bIsDischarging )
    dp88_AI_Turret::attackLocation ( pSelf, location, primary );

  // Otherwise call StartCharging and let it determine what to do...
  StartCharging(pSelf);
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_ChargedTurret::StartCharging ( GameObject* pSelf )
{
  // If we are not charging, not discharging, powered and have a target then we can start a new
  // charging cycle
  if ( !m_bIsCharging && !m_bIsDischarging && checkPowerState(pSelf) && targetID != NULL )
  {
    m_bIsCharging = true;

    // Only play a single iteration of the charge up animation - if we need to keep attacking after
    // the shot is fired this will get called again
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("Animation"), Get_Int_Parameter("Animation_Charge_Start_Frame"), Get_Int_Parameter("Animation_Charge_End_Frame"), 1 );

    if (_stricmp(Get_Parameter("Charge_Sound"),"0"))
      Commands->Create_Sound(Get_Parameter("Charge_Sound"),Commands->Get_Position(pSelf),pSelf);

    // Nothing to do but wait for the charging to complete... hum de dum...
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_ChargedTurret::StartDischarging ( GameObject* pSelf )
{
  // Were we charging up? If so we are now fully charged and can start shooting stuff, if we still
  // have a valid target...
  if ( m_bIsCharging )
  {
    m_bIsCharging = false;
  
    // Got an enemy? Also double check power state whilst we are here, just to be sure
    if ( targetID != NULL && checkPowerState(pSelf) )
    {
      if ( GameObject* pTarget = Commands->Find_Object(targetID) )
      {
        m_bIsDischarging = true;

        // Call into the base class and let it do it's thing
        if ( splashInfantry && pTarget->As_SoldierGameObj() )
          dp88_AI_Turret::attackLocation(pSelf, Commands->Get_Position(pTarget), primary_target);
        else
          dp88_AI_Turret::attackTarget(pSelf, pTarget, primary_target);
      }
    }

    // If we didn't find anything to shoot at then apply the idle animation again
    if ( !m_bIsDischarging )
      ApplyIdleAnimation(pSelf);
  }
}

// -------------------------------------------------------------------------------------------------

GameObject* dp88_AI_ChargedTurret::GetAnimationObject ( GameObject* pSelf )
{
  if ( m_chargeAnimObjId != NULL )
    return Commands->Find_Object(m_chargeAnimObjId);
  return pSelf;
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_ChargedTurret::ApplyIdleAnimation ( GameObject* pSelf )
{
  // Apply the correct animation frames for the uncharged state dependant upon base power conditions,
  // which should already have been updated before calling this function
  if ( m_bPowerState )
    m_pLoopedAnimCtrl->PlayAnimation(Get_Parameter("Animation"),Get_Int_Parameter("Animation_Idle_Start_Frame"),Get_Int_Parameter("Animation_Idle_End_Frame"));
  else
    m_pLoopedAnimCtrl->PlayAnimation(Get_Parameter("Animation"),Get_Int_Parameter("Animation_Unpowered_Start_Frame"),Get_Int_Parameter("Animation_Unpowered_End_Frame"));
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_ChargedTurret::Custom ( GameObject* pSelf, int type, int param, GameObject* pSender )
{
  if ( pSelf != pSender && GetAnimationObject(pSelf) == pSender )
    Animation_Complete(pSelf, Get_Parameter("Animation"));
}









// -------------------------------------------------------------------------------------------------
//  Charged Turret AI - Animation Complete Notifier
// -------------------------------------------------------------------------------------------------

void dp88_AI_ChargedTurret_Animation::Animation_Complete ( GameObject* pSelf, const char* animation_name )
{
  if ( GameObject* pTurret = Commands->Find_Object(Get_Int_Parameter("tId")) )
    Commands->Send_Custom_Event ( pSelf, pTurret, CUSTOM_TURRETAI_ANIMATIONCOMPLETE, 0, 0.0f );
}








// ------------------
// Custom AI Objective
// ------------------

int dp88_AI_Objective::s_nObjectives = 0;
dp88_AI_Objective** dp88_AI_Objective::s_pObjectives = NULL;

// -------------------------------------------------------------------------------------------------

void dp88_AI_Objective::Created ( GameObject* obj )
{
  // Register objective
  dp88_AI_Objective** pOldObjectives = s_pObjectives;
  s_pObjectives = new dp88_AI_Objective*[++s_nObjectives];
  for ( int i = 0; i < s_nObjectives-1; ++i )
    s_pObjectives[i] = pOldObjectives[i];
  s_pObjectives[s_nObjectives-1] = this;

  // Store data
  m_objID = Commands->Get_ID(obj);
  m_priority = Get_Int_Parameter("Priority");
  m_distance = Get_Int_Parameter("Distance");
  m_type = (unsigned char)Get_Int_Parameter("Type");
}

// -------------------------------------------------------------------------------------------------

void dp88_AI_Objective::Detach ( GameObject* obj )
{
  // De-register objective
  dp88_AI_Objective** pOldObjectives = s_pObjectives;
  if ( s_pObjectives != NULL && --s_nObjectives != 0 )
  {
    s_pObjectives = new dp88_AI_Objective*[s_nObjectives];
    for ( int i = 0, j = 0; i < s_nObjectives; ++i, ++j )
    {
      if ( pOldObjectives[j] == this )
        ++j;
      s_pObjectives[i] = pOldObjectives[j];
    }
  }
  delete [] pOldObjectives;
}

// -------------------------------------------------------------------------------------------------

GameObject* dp88_AI_Objective::GetGameObject()
{
  return Commands->Find_Object(m_objID);
}

// -------------------------------------------------------------------------------------------------

bool dp88_AI_Objective::IsSuitable ( GameObject* obj, unsigned char objective_type )
{
  int team = Get_Int_Parameter("Team");
  if ( objective_type != m_type || (team != 2 && Get_Object_Type(obj) != team) )
    return false;

  if ( obj->As_SoldierGameObj() && !Get_Fly_Mode(obj) )
    return ( Get_Int_Parameter("Soldier_Objective") == 1 );

  if (  Get_Vehicle_Mode(obj) == VEHICLE_TYPE_FLYING || (obj->As_SoldierGameObj() && Get_Fly_Mode(obj)) )
    return ( Get_Int_Parameter("Aircraft_Objective") == 1 );

  if ( obj->As_VehicleGameObj() )
    return (Is_Script_Attached ( obj, "dp88_AI_heavyVehicleMarker" ))
      ? ( Get_Int_Parameter("Heavy_Vehicle_Objective") == 1 )
      : ( Get_Int_Parameter("Light_Vehicle_Objective") == 1 );

  return false;
}

// -------------------------------------------------------------------------------------------------

dp88_AI_Objective* dp88_AI_Objective::GetObjective ( GameObject* obj, unsigned char objective_type )
{
  dp88_AI_Objective* result = NULL;
  int top_priority = 0;

  for ( int i = 0; i < s_nObjectives; ++i )
  {
    if ( s_pObjectives[i]->IsSuitable(obj,objective_type)
      && s_pObjectives[i]->GetPriority() >= top_priority
      && s_pObjectives[i]->GetGameObject() != NULL )
    {
      // Always switch to a higher priority objective and allow a random chance to switch to
      // another equal priority objective
      if ( s_pObjectives[i]->m_priority > top_priority || Commands->Get_Random_Int(1,3) == 1 )
      {
        top_priority = s_pObjectives[i]->GetPriority();
        result = s_pObjectives[i];
      }
    }
  }

  return result;
}

// -------------------------------------------------------------------------------------------------

bool dp88_AI_Objective::IsValidObjective(dp88_AI_Objective* pObjective)
{
  for ( int i = 0; i < s_nObjectives; ++i )
  {
    if ( s_pObjectives[i] == pObjective )
      return ( s_pObjectives[i]->GetGameObject() != NULL );
  }

  return false;
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_AI_Objective> dp88_AI_Objective_Registrant(
  "dp88_AI_Objective",
  "Priority=1:int,"
  "Distance=50:int,"
  "Team:int,"
  "Type=1:int,"
  "Soldier_Objective=1:int,"
  "Light_Vehicle_Objective=1:int,"
  "Heavy_Vehicle_Objective=1:int,"
  "Aircraft_Objective=1:int");






// -------------------------------------------------------------------------------------------------
//  Script Registrants
// -------------------------------------------------------------------------------------------------

// Heavy vehicle marker (dummy script)
ScriptRegistrant<dp88_AI_heavyVehicleMarker> dp88_AI_heavyVehicleMarker_Registrant("dp88_AI_heavyVehicleMarker","");

ScriptRegistrant<dp88_AI_Turret> dp88_AI_Turret_Registrant("dp88_AI_Turret","Priority_Infantry=1.0:float,Weapon_Infantry=1:int,Splash_Infantry=0:int,Priority_Light_Vehicle=5.0:float,Weapon_Light_Vehicle=1:int,Priority_Heavy_Vehicle=7.0:float,Weapon_Heavy_Vehicle=1:int,Priority_VTOL=5.0:float,Weapon_VTOL=1:int,Min_Attack_Range=0:int,Max_Attack_Range=150:int,Min_Attack_Range_Secondary=0:int,Max_Attack_Range_Secondary=150:int,Modifier_Distance=0.25:float,Modifier_Target_Damage=0.1:float,Modifier_Target_Value=0.05:float,Requires_Power=0:int,Debug=0:int,Detects_Stealth=1:int");
ScriptRegistrant<dp88_AI_PopupTurret> dp88_AI_PopupTurret_Registrant("dp88_AI_PopupTurret","Priority_Infantry=1.0:float,Weapon_Infantry=1:int,Splash_Infantry=0:int,Priority_Light_Vehicle=5.0:float,Weapon_Light_Vehicle=1:int,Priority_Heavy_Vehicle=7.0:float,Weapon_Heavy_Vehicle=1:int,Priority_VTOL=5.0:float,Weapon_VTOL=1:int,Min_Attack_Range=0:int,Max_Attack_Range=150:int,Min_Attack_Range_Secondary=0:int,Max_Attack_Range_Secondary=150:int,Deploy_Animation:string,Deploy_Animation_Frames:int,Deploy_Sound:string,Deploy_Timeout:int,Spotter_Preset:string,Modifier_Distance=0.25:float,Modifier_Target_Damage=0.1:float,Modifier_Target_Value=0.05:float,Requires_Power=0:int,Debug=0:int,Detects_Stealth=1:int");
ScriptRegistrant<dp88_AI_PopupTurret_Spotter> dp88_AI_PopupTurret_Spotter_Registrant("dp88_AI_PopupTurret_Spotter","tId:int");
ScriptRegistrant<dp88_AI_ChargedTurret> dp88_AI_ChargedTurret_Registrant("dp88_AI_ChargedTurret","Priority_Infantry=1.0:float,Weapon_Infantry=1:int,Splash_Infantry=0:int,Priority_Light_Vehicle=5.0:float,Weapon_Light_Vehicle=1:int,Priority_Heavy_Vehicle=7.0:float,Weapon_Heavy_Vehicle=1:int,Priority_VTOL=5.0:float,Weapon_VTOL=1:int,Min_Attack_Range=0:int,Max_Attack_Range=150:int,Min_Attack_Range_Secondary=0:int,Max_Attack_Range_Secondary=150:int,Animation_Model:string,Animation_Model_Bone:string,Animation:string,Animation_Idle_Start_Frame:int,Animation_Idle_End_Frame:int,Animation_Unpowered_Start_Frame:int,Animation_Unpowered_End_Frame:int,Animation_Charge_Start_Frame:int,Animation_Charge_End_Frame:int,Charge_Sound:string,Modifier_Distance=0.25:float,Modifier_Target_Damage=0.1:float,Modifier_Target_Value=0.05:float,Requires_Power=0:int,Debug=0:int,Detects_Stealth=1:int");
ScriptRegistrant<dp88_AI_ChargedTurret_Animation> dp88_AI_ChargedTurret_Animation_Registrant("dp88_AI_ChargedTurret_Animation","tId:int");
