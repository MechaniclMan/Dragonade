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
#include "dp88_ar_unitScripts.h"
#include "SoldierGameObj.h"




/*------------------------
Rocketeer Script Functions
--------------------------*/

void dp88_AR_Rocketeer::Created ( GameObject *obj )
{
	objectID = Commands->Get_ID ( obj );
	isFlying = false;
	isDead = false;
	lastDeploy = 0;

	if ( obj->As_SoldierGameObj() && Get_Player_ID ( obj ) >= 0 )
		InstallHook( Get_Parameter("Keyhook"), obj );
	else
		Destroy_Script();
}


void dp88_AR_Rocketeer::Destroyed ( GameObject *obj )
{
	isDead = true;
	RemoveHook();
}


void dp88_AR_Rocketeer::Killed( GameObject *obj, GameObject *killer )
{
	isDead = true;
	RemoveHook();
}


void dp88_AR_Rocketeer::KeyHook()
{
	// Find object
	GameObject* obj = Commands->Find_Object ( objectID );
	if ( !obj || isDead || Get_Vehicle(obj) || ((SoldierGameObj*)obj)->Is_On_Ladder() )
		return;

	if ( (isFlying && time(NULL) - lastDeploy > Get_Int_Parameter("Minimum_Flight_Time")) || 
		!isFlying && time(NULL) - lastDeploy > Get_Int_Parameter("Minimum_Walk_Time"))
	{
		isFlying = !isFlying;
		Toggle_Fly_Mode(obj);
		lastDeploy = time(NULL);
	}
	else
	{
		StringClass message;
		message.Format("You must wait at least %d seconds before %s flying mode", (isFlying) ? Get_Int_Parameter("Minimum_Flight_Time") : Get_Int_Parameter("Minimum_Walk_Time"), (isFlying) ? "deactivating": "activating");
		Send_Message_Player(obj,153,204,25,message);
	}
}


dp88_AR_Rocketeer::~dp88_AR_Rocketeer()
{
	// Turn flying mode off if we buy a new infantry unit...
	if ( isFlying && !isDead )
	{
		GameObject* obj = Commands->Find_Object(objectID);
		if ( obj != NULL )
		{
			Toggle_Fly_Mode(obj);
		}
	}
}




/*------------------------
Mirage Tank Script Functions
--------------------------*/

void dp88_AR_MirageTank::Created ( GameObject *obj )
{
	hidden = false;
	pilotID = 0;
	mirageID = 0;

	// If the game controller does not exist then bail out
	GameObject *gameController = Find_Object_With_Script("dp88_ar_gameController");
	if ( !gameController )
	{
		Console_Output ( "dp88_AR_MirageTank - Unable to find Game Controller, unable to continue. Destroying script...\n" );
		Destroy_Script();
		return;
	}
	else
		gameControllerId = Commands->Get_ID(gameController);
}

void dp88_AR_MirageTank::Damaged ( GameObject* obj, GameObject* damager, float amount )
{
	if ( hidden )
		setHidden ( obj, false );
}

void dp88_AR_MirageTank::Killed ( GameObject* obj, GameObject* killer )
{
	if ( hidden )
		setHidden ( obj, false );
}


void dp88_AR_MirageTank::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
	{
		if ( pilotID == 0 )
		{
			pilotID = Commands->Get_ID(sender);

			// Start timer to check for no movement (and create mirage)
			Commands->Start_Timer ( obj, this, 0.5, TIMER_MIRAGE_CHECKMOVEMENT );
		}
	}

	// Look for vehicle exit
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED )
	{
		if ( Commands->Get_ID(sender) == pilotID )
		{
			pilotID = 0;
			setHidden ( obj, false );
		}
	}
}


void dp88_AR_MirageTank::Timer_Expired( GameObject *obj, int number )
{
	if ( number == TIMER_MIRAGE_CHECKMOVEMENT && pilotID != 0 )
	{
		Vector3 currentPos = Commands->Get_Position ( obj );
		
		// Because stationary vehicles seem to 'drift' without moving we check
		// that the difference is very low to see if it is stationary.
		float diffX = currentPos.X - lastPos.X;		if ( diffX < 0 ) diffX = -diffX;
		float diffY = currentPos.Y - lastPos.Y;		if ( diffY < 0 ) diffY = -diffY;
		float diffZ = currentPos.Z - lastPos.Z;		if ( diffZ < 0 ) diffZ = -diffZ;

		if ( hidden && ( diffX > 0.005 || diffY > 0.005 || diffZ > 0.005 || pilotID == 0 ) )
			setHidden ( obj, false );

		else if ( !hidden && ( diffX < 0.005 && diffY < 0.005 && diffZ < 0.005 ) )
			setHidden ( obj, true );

		// Reset lastPos and restart timer
		if ( pilotID )
			Commands->Start_Timer ( obj, this, 0.5, TIMER_MIRAGE_CHECKMOVEMENT );
		lastPos = Commands->Get_Position ( obj );
	}
}

void dp88_AR_MirageTank::setHidden ( GameObject *obj, bool hide )
{
	if ( hide && !hidden )
	{
		/* Get handle to game controller script */
		GameObject *gameController = Commands->Find_Object(gameControllerId);
		if ( !gameController ) return;
		dp88_AR_GameController *gameControllerScript = (dp88_AR_GameController*)Find_Script_On_Object(gameController, "dp88_ar_gameController");
		if ( !gameControllerScript ) return;


		// Check if there are any model names defined
		if ( gameControllerScript->mirageTank_disguisePresets[0] == 0
			&& gameControllerScript->mirageTank_disguisePresets[1] == 0
			&& gameControllerScript->mirageTank_disguisePresets[2] == 0 )
		{

			Console_Output ( "[%d:%s:%s] No valid Mirage Tank disguises have been set in the GameController, aborting script!", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
			Destroy_Script();
		}
		else
		{
			// Get a disguise number to use
			int disguiseID = -1;
			while ( disguiseID == -1 )
			{
				disguiseID = Commands->Get_Random_Int ( 0, 3 );		// 0 -> 2
				if ( gameControllerScript->mirageTank_disguisePresets[disguiseID] == 0 )
					disguiseID = -1;
			}

			// Create mirage
			Vector3 miragePosition = Commands->Get_Position ( obj );
			miragePosition.Z -= 3;
			GameObject* mirage = Commands->Create_Object ( gameControllerScript->mirageTank_disguisePresets[disguiseID], miragePosition );
			Commands->Disable_All_Collisions ( mirage );
			mirageID = Commands->Get_ID ( mirage );

			// Setup tank variables
			Commands->Enable_Stealth ( obj, true );			// Disable targeting box
			Commands->Set_Is_Rendered ( obj, false );		// Disables rendering
			Commands->Set_Is_Visible ( obj, false );		// Prevents AI seeing tank
			Commands->Enable_Engine ( obj, false );			// Disable engine sounds

			hidden = true;
		}
	}


	else if ( !hide && hidden )
	{
		// Delete mirage
		Commands->Destroy_Object ( Commands->Find_Object ( mirageID ) );
		mirageID = 0;

		// Setup tank variables
		Commands->Enable_Engine ( obj, true );
		Commands->Set_Is_Visible ( obj, true );
		Commands->Set_Is_Rendered ( obj, true );
		Commands->Enable_Stealth ( obj, false );
		
		hidden = false;
	}
}




/*------------------------
IFV Scripts Functions
--------------------------*/

void dp88_AR_IFV::Created ( GameObject *obj )
{
	objectID = Commands->Get_ID ( obj );
	pilotID = NULL;
	isDeployed = false;
	isDeploying = false;
	veterancyLevel = 0;

	deployedAnimFrame = 0;
	deployedRookieWeaponPowerupId = NULL;
	deployedVeteranWeaponPowerupId = NULL;
	deployedEliteWeaponPowerupId = NULL;

	// Save default weapon information
	strcpy_s ( defaultWeapon, sizeof( defaultWeapon ), Get_Current_Weapon ( obj ) );
	strcpy_s ( currentDefaultWeapon, sizeof( currentDefaultWeapon ), Get_Current_Weapon ( obj ) );
	strcpy_s ( currentWeapon, sizeof( currentWeapon ), Get_Current_Weapon ( obj ) );

	// Set default animation frame
	Commands->Set_Animation_Frame ( obj, Get_Parameter ( "Turret_Frames_Animation" ), 1 );
	//Commands->Set_Animation ( obj, Get_Parameter ( "Turret_Frames_Animation" ), false, NULL, 1.0f, 1.0f, false );

	// Grant fake weapon for use during deployment
	//Commands->Give_PowerUp ( obj, "POW_Allied_IFV_None", false );
	nullWeaponGranted = false;
}


void dp88_AR_IFV::Destroyed ( GameObject *obj )
{
}


void dp88_AR_IFV::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
	{
		if ( pilotID == NULL )
		{
			pilotID = Commands->Get_ID(sender);

			InstallHook( Get_Parameter("Keyhook"), sender );

			// Send a custom to the new driver to ask for model and powerup data
			Commands->Send_Custom_Event ( obj, sender, CUSTOM_IFV_REQUESTDRIVERDATA, 0, 0.0f );
		}


		/* If this is our first driver grant the Weapon_IFV_None weapon. Trying to do this here
		because for some reason the IFV can still fire while deploying on the FDS and I can only
		assume it's because the weapon cannot be granted at creation time on the FDS, unlike in
		LAN mode where it works perfectly well... */
		if ( nullWeaponGranted == false )
		{
			nullWeaponGranted = true;
			// Grant fake weapon for use during deployment
			Commands->Give_PowerUp ( obj, "POW_Allied_IFV_None", false );
		}
	}




	// Look for vehicle exit
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED )
	{
		if ( Commands->Get_ID(sender) == pilotID )
		{
			pilotID = NULL;

			RemoveHook();
		}


		// Are we deployed? If so then undeploy
		if ( isDeployed && !isDeploying )
			undeploy();

		/* Otherwise if we are deploying then disable transitions until
		deployment is complete and then undeploy */
		else if ( isDeployed )
			Commands->Enable_Vehicle_Transitions ( obj, false );


		// Reset driver data
		deployedAnimFrame = 0;
		deployedRookieWeaponPowerupId = NULL;
		deployedVeteranWeaponPowerupId = NULL;
		deployedEliteWeaponPowerupId = NULL;
	}




	// Look for a promotion (we will need to store new default weapon
	// if it changed, and possibly restore the deployed weapon)
	else if ( type == CUSTOM_VETERANCY_PROMOTED )
	{
		veterancyLevel = param;

		// If the promotion changed our weapon, then set that as our new default weapon
		if ( Get_Current_Weapon ( obj ) != NULL
			&& strcmp ( Get_Current_Weapon ( obj ), currentWeapon ) != 0 )
		{
			strcpy_s ( currentDefaultWeapon, sizeof(currentDefaultWeapon), Get_Current_Weapon ( obj ) );
			strcpy_s ( currentWeapon, sizeof(currentWeapon), Get_Current_Weapon ( obj ) );
		}


		// If we are deployed then we need to work out which weapon
		// preset we should currently have and set it if necessary
		if ( isDeployed )
		{
			int powerupID = NULL;

			if ( veterancyLevel == 2 && deployedEliteWeaponPowerupId != NULL )
				powerupID = deployedEliteWeaponPowerupId;

			else if ( veterancyLevel >= 1 && deployedVeteranWeaponPowerupId != NULL )
				powerupID = deployedVeteranWeaponPowerupId;

			else
				powerupID = deployedRookieWeaponPowerupId;


			if ( powerupID != NULL )
			{
				Commands->Give_PowerUp ( obj, Get_Definition_Name ( powerupID ), false );
				strcpy_s ( currentWeapon, sizeof(currentWeapon), Get_Powerup_Weapon ( Get_Definition_Name ( powerupID ) ) );
			}
		}


		/* If we are deploying then set weapon to none, otherwise double check
		it is set to the right weapon */
		if ( isDeploying )
			Commands->Select_Weapon ( obj, "Weapon_IFV_None" );
		else
			Commands->Select_Weapon ( obj, currentWeapon );
	}




	// Look for driver responses containing deployment data
	else if ( type == CUSTOM_IFV_DRIVER_ANIMFRAME )
		deployedAnimFrame = param;
	else if ( type == CUSTOM_IFV_DRIVER_ROOKIEPOWID && param > 0 && Is_Valid_Preset_ID ( param ) )
		deployedRookieWeaponPowerupId = param;
	else if ( type == CUSTOM_IFV_DRIVER_VETERANPOWID && param > 0 && Is_Valid_Preset_ID ( param ) )
		deployedVeteranWeaponPowerupId = param;
	else if ( type == CUSTOM_IFV_DRIVER_ELITEPOWID && param > 0 && Is_Valid_Preset_ID ( param ) )
		deployedEliteWeaponPowerupId = param;
}


void dp88_AR_IFV::Timer_Expired( GameObject *obj, int number )
{
	/* Timer to indicate (un)deployment is complete and set correct
	animation frame */
	if ( number == TIMER_IFV_DEPLOYCOMPLETE )
	{
		isDeploying = false;

		if ( isDeployed )
		{
			Commands->Set_Animation_Frame ( obj, Get_Parameter ( "Turret_Frames_Animation" ), deployedAnimFrame );
			//Commands->Set_Animation ( obj, Get_Parameter ( "Turret_Frames_Animation" ), false, NULL, (float)deployedAnimFrame, (float)deployedAnimFrame, false );

			/* Has the pilot got out during deployment? If so then we
			need to undeploy */
			if ( pilotID == NULL )
			{
				undeploy();
				Commands->Enable_Vehicle_Transitions ( obj, true );
			}
		}

		else
		{
			Commands->Set_Animation_Frame ( obj, Get_Parameter ( "Turret_Frames_Animation" ), 1 );
			//Commands->Set_Animation ( obj, Get_Parameter ( "Turret_Frames_Animation" ), false, NULL, 1.0f, 1.0f, false );
		}
		

		// Select new weapon
		Commands->Select_Weapon( obj, currentWeapon );
	}
}


void dp88_AR_IFV::KeyHook()
{
	// Find object
	GameObject* obj = Commands->Find_Object ( objectID );
	if ( !obj )
		return;


	/* Return if no custom model is defined for the driver or if we
	are already in the process of (un)deploying */
	if ( deployedAnimFrame == 0 || isDeploying == true )
		return;

	// Undeploy
	if ( isDeployed )
		undeploy();

	// Deploy
	else
		deploy();
}


// Deploy function
void dp88_AR_IFV::deploy()
{
	// Find object
	GameObject* obj = Commands->Find_Object ( objectID );
	if ( !obj )
		return;
	
	// Pick a weapon powerup to grant
	int powerupID = NULL;

	if ( veterancyLevel == 2 && deployedEliteWeaponPowerupId != NULL )
		powerupID = deployedEliteWeaponPowerupId;

	else if ( veterancyLevel >= 1 && deployedVeteranWeaponPowerupId != NULL )
		powerupID = deployedVeteranWeaponPowerupId;

	else
		powerupID = deployedRookieWeaponPowerupId;


	// Grant any weapon powerups we found and set that as current weapon
	if ( powerupID != NULL )
	{
		Commands->Give_PowerUp ( obj, Get_Definition_Name ( powerupID ), false );
		strcpy_s ( currentWeapon, sizeof(currentWeapon), Get_Powerup_Weapon ( Get_Definition_Name ( powerupID ) ) );
	}


	// Disable weapon
	Commands->Select_Weapon ( obj, "Weapon_IFV_None" );


	// Set deploying anim frame
	Commands->Set_Animation_Frame ( obj, Get_Parameter ( "Turret_Frames_Animation" ), Get_Int_Parameter ( "Switching_Anim_Frame" ) );
	//Commands->Set_Animation ( obj, Get_Parameter ( "Turret_Frames_Animation" ), false, NULL, (float)Get_Int_Parameter ( "Switching_Anim_Frame" ), (float)Get_Int_Parameter ( "Switching_Anim_Frame" ), false );

	// Set flags and start timer to complete deployment
	isDeployed = true;
	isDeploying = true;
	Commands->Start_Timer ( obj, this, (float)Get_Int_Parameter ( "Switch_Time" ), TIMER_IFV_DEPLOYCOMPLETE );
}


// Undeploy function
void dp88_AR_IFV::undeploy()
{
	// Find object
	GameObject* obj = Commands->Find_Object ( objectID );
	if ( !obj )
		return;

	// Disable weapon and set the weapon to use after deploying
	Commands->Select_Weapon ( obj, "Weapon_IFV_None" );
	strcpy_s ( currentWeapon, sizeof(currentWeapon), currentDefaultWeapon );

	// Set deploying anim frame
	Commands->Set_Animation_Frame ( obj, Get_Parameter ( "Turret_Frames_Animation" ), Get_Int_Parameter ( "Switching_Anim_Frame" ) );
	//Commands->Set_Animation ( obj, Get_Parameter ( "Turret_Frames_Animation" ), false, NULL, (float)Get_Int_Parameter ( "Switching_Anim_Frame" ), (float)Get_Int_Parameter ( "Switching_Anim_Frame" ), false );

	// Set flags and start timer to complete deployment
	isDeployed = false;
	isDeploying = true;
	Commands->Start_Timer ( obj, this, (float)Get_Int_Parameter ( "Switch_Time" ), TIMER_IFV_DEPLOYCOMPLETE );
}





void dp88_AR_IFVDriver::Custom ( GameObject* obj, int type, int param, GameObject* sender )
{
	// If we entered an IFV we will recieve this to tell us to register our data
	if ( type == CUSTOM_IFV_REQUESTDRIVERDATA )
	{
		/* Send responses for our animation frame number, rookie weapon powerup ID,
		veteran weapon powerup ID and elite weapon powerup ID */
		Commands->Send_Custom_Event ( obj, sender, CUSTOM_IFV_DRIVER_ANIMFRAME, Get_Int_Parameter ( "IFV_ModelAnimFrame" ), 0.0f );
		
		if ( Is_Valid_Preset ( Get_Parameter ( "IFV_WeaponPowerup_Rookie" ) ) )
			Commands->Send_Custom_Event ( obj, sender, CUSTOM_IFV_DRIVER_ROOKIEPOWID, Get_Definition_ID ( Get_Parameter ( "IFV_WeaponPowerup_Rookie" ) ), 0.0f );
		if ( Is_Valid_Preset ( Get_Parameter ( "IFV_WeaponPowerup_Veteran" ) ) )
			Commands->Send_Custom_Event ( obj, sender, CUSTOM_IFV_DRIVER_VETERANPOWID, Get_Definition_ID ( Get_Parameter ( "IFV_WeaponPowerup_Veteran" ) ), 0.0f );
		if ( Is_Valid_Preset ( Get_Parameter ( "IFV_WeaponPowerup_Elite" ) ) )
			Commands->Send_Custom_Event ( obj, sender, CUSTOM_IFV_DRIVER_ELITEPOWID, Get_Definition_ID ( Get_Parameter ( "IFV_WeaponPowerup_Elite" ) ), 0.0f );
	}
}







/*------------------------
Tesla Coil Script
--------------------------*/

void dp88_AR_Tesla_Coil::Created( GameObject *obj )
{
	// Let the base class handle the main initialization
	dp88_AI_Turret::Init(obj);
	dp88_AI_Turret::loadSettings(obj, false, false);

	// Initialize our charge level to 0
	m_chargeLevel = 0;
	m_isSupercharged = false;

	// Load settings
	m_chargeDuration = Get_Int_Parameter("Charge_Duration");
	m_chargesPowerOn = Get_Int_Parameter("Charges_Power_On");
	m_chargesSupercharge = Get_Int_Parameter("Charges_Supercharge");

	// Grant the supercharged weapon powerup preset and save the name of the default weapon preset
	if ( Is_Valid_Preset(Get_Parameter("Supercharged_Weapon_Powerup_Preset")) )
	{
		m_defaultWeapon = Get_Current_Weapon(obj);
		m_defaultWeaponRange = this->primary_maxRange;

		Commands->Give_PowerUp(obj, Get_Parameter("Supercharged_Weapon_Powerup_Preset"), false);
		m_superchargedWeapon = Get_Powerup_Weapon(Get_Parameter("Supercharged_Weapon_Powerup_Preset"));
		m_superchargedWeaponRange = Get_Int_Parameter("Supercharged_Weapon_Range");
	}
}


void dp88_AR_Tesla_Coil::Damaged ( GameObject *obj, GameObject *damager, float amount )
{
	// If the damager has the script dp88_AR_Tesla_Coil_Charger attached then
	// increase our charge level - unless their last charge time is within 1
	// second of the current time - prevents double charges due to explosion
	// damage
	dp88_AR_Tesla_Coil_Charger* chargerScript = (dp88_AR_Tesla_Coil_Charger *)(Find_Script_On_Object(damager, "dp88_AR_Tesla_Coil_Charger"));
	if ( chargerScript && chargerScript->m_lastChargeTime < time(NULL)-1 )
	{
		// Increase charge level
		m_chargeLevel++;

		// Set chargers last charge time
		chargerScript->m_lastChargeTime = time(NULL);

		// Have we reached a sufficient level to become supercharged?
		if ( !m_isSupercharged && ((Is_Base_Powered(Get_Object_Type(obj)) && m_chargeLevel >= m_chargesSupercharge) || m_chargeLevel >= m_chargesPowerOn + m_chargesSupercharge) )
			setSuperchargedState(obj, true);

		// Start timer to decrement charge level
		Commands->Start_Timer(obj, this, (float)m_chargeDuration, TIMER_TESLACOIL_DECREMENT_CHARGE );
	}
}


void dp88_AR_Tesla_Coil::Timer_Expired ( GameObject* obj, int number )
{
	if (number == TIMER_TESLACOIL_DECREMENT_CHARGE)
	{
		if ( m_chargeLevel > 0 )
			m_chargeLevel--;

		// Have we dropped out of supercharged status?
		if ( m_isSupercharged && !((Is_Base_Powered(Get_Object_Type(obj)) && m_chargeLevel >= m_chargesSupercharge) || m_chargeLevel >= m_chargesPowerOn + m_chargesSupercharge) )
			setSuperchargedState(obj, false);
	}

	// Call base class timer expired
	dp88_AI_Turret::Timer_Expired(obj, number);
}


// Returns true if the base is powered, if the defence does not require power, or
// if the defence has been charged sufficiently to provide base power
bool dp88_AR_Tesla_Coil::checkPowerState ( GameObject* obj )
{
	return ( !requiresPower || Is_Base_Powered(Get_Object_Type(obj)) || m_chargeLevel >= Get_Int_Parameter("Charges_Power_On") );
}


// Set the supercharged state
void dp88_AR_Tesla_Coil::setSuperchargedState(GameObject* obj, bool state)
{
	if (! m_superchargedWeapon.Is_Empty() )
	{
		// If we are setting supercharged to true then switch weapons
		if ( state )
		{
			Commands->Select_Weapon(obj, m_superchargedWeapon);
			primary_maxRange = m_superchargedWeaponRange;
		}
		else
		{
			Commands->Select_Weapon(obj, m_defaultWeapon);
			primary_maxRange = m_defaultWeaponRange;
		}

		m_isSupercharged = state;
	}
}





// Rocketeer
ScriptRegistrant<dp88_AR_Rocketeer> dp88_AR_Rocketeer_Registrant( "dp88_AR_Rocketeer", "Flying_Preset:string,Keyhook=IDeploy:string,Minimum_Flight_Time=5:int,Minimum_Walk_Time=5:int" );

// Mirage Tank
ScriptRegistrant<dp88_AR_MirageTank> dp88_AR_MirageTank_Registrant( "dp88_AR_MirageTank", "" );

// IFV
ScriptRegistrant<dp88_AR_IFV> dp88_AR_IFV_Registrant( "dp88_AR_IFV", "Turret_Frames_Animation=v_all_ifv.v_all_ifv:string,Switch_Time=10:int,Switching_Anim_Frame=2:int,Keyhook=VDeploy:string" );
ScriptRegistrant<dp88_AR_IFVDriver> dp88_AR_IFVDriver_Registrant( "dp88_AR_IFVDriver", "IFV_ModelAnimFrame=1:int,IFV_WeaponPowerup_Rookie=null:string,IFV_WeaponPowerup_Veteran=null:string,IFV_WeaponPowerup_Elite=null:string" );

// Tesla Coil
ScriptRegistrant<dp88_AR_Tesla_Coil> dp88_AR_Tesla_Coil_Registrant("dp88_AR_Tesla_Coil","Priority_Infantry=1.0:float,Splash_Infantry=0:int,Priority_Light_Vehicle=5.0:float,Priority_Heavy_Vehicle=7.0:float,Priority_VTOL=5.0:float,Min_Attack_Range=0:int,Max_Attack_Range=80:int,Modifier_Distance=0.25:float,Modifier_Target_Damage=0.1:float,Modifier_Target_Value=0.05:float,Requires_Power=1:int,Charge_Duration=5:int,Charges_Supercharge=1:int,Charges_Power_On=2:int,Supercharged_Weapon_Powerup_Preset:string,Supercharged_Weapon_Range=100:int,Debug=0:int");
ScriptRegistrant<dp88_AR_Tesla_Coil_Charger> dp88_AR_Tesla_Coil_Charger_Registrant("dp88_AR_Tesla_Coil_Charger","");