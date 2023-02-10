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
#include "dp88_ar.h"
#include "Definition.h"
#include "PurchaseSettingsDefClass.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"
#include "VehicleGameObjDef.h"
#include "MoveablePhysClass.h"
#include "SList.h"
#include "OffenseObjectClass.h"
#include "WeaponBagClass.h"

/*------------------------
Game Controller
--------------------------*/

dp88_AR_GameController::dp88_AR_GameController()
{
	created = false;

	team0_countryID = 0;
	team1_countryID = 0;

	TD_Waiting_Console = 0;

	mirageTank_disguisePresets[0] = 0;
	mirageTank_disguisePresets[1] = 0;
	mirageTank_disguisePresets[2] = 0;
}

dp88_AR_GameController::~dp88_AR_GameController()
{}

void dp88_AR_GameController::Created( GameObject *obj )
{
	//Console_Output ( "Created dp88_AR_GameController\n" );
	created = true;
	
	// Check we have not disabled all soviet countries
	if ( Get_Int_Parameter ( "enableCountry_Russia" ) == 0
		&& Get_Int_Parameter ( "enableCountry_Cuba" ) == 0
		&& Get_Int_Parameter ( "enableCountry_Iraq" ) == 0
		&& Get_Int_Parameter ( "enableCountry_Libya" ) == 0 )
	{
		Console_Input ( "msg GAME CONTROLLER ERROR: All Soviet countries are disabled, defaulting to Russia!" );
		team0_countryID = 1;
	}

	// Check we have not disabled all allied countries
	if ( Get_Int_Parameter ( "enableCountry_America" ) == 0
		&& Get_Int_Parameter ( "enableCountry_France" ) == 0
		&& Get_Int_Parameter ( "enableCountry_Germany" ) == 0
		&& Get_Int_Parameter ( "enableCountry_GreatBritain" ) == 0
		&& Get_Int_Parameter ( "enableCountry_Korea" ) == 0 )
	{
		Console_Input ( "msg GAME CONTROLLER ERROR: All Allied countries are disabled, defaulting to America!" );
		team1_countryID = 1;
	}
	
	// Loop around until we get a non-disabled country ID for Soviets
	while ( team0_countryID == 0 )		
	{
		team0_countryID = (short)Commands->Get_Random_Int ( 1, 5 ); // Soviet, 1 to 4

		if ( team0_countryID == 1 && Get_Int_Parameter ( "enableCountry_Russia" ) == 0 )
			team0_countryID = 0;
		
		else if ( team0_countryID == 2 && Get_Int_Parameter ( "enableCountry_Cuba" ) == 0 )
			team0_countryID = 0;
		
		else if ( team0_countryID == 3 && Get_Int_Parameter ( "enableCountry_Iraq" ) == 0 )
			team0_countryID = 0;
		
		else if ( team0_countryID == 4 && Get_Int_Parameter ( "enableCountry_Libya" ) == 0 )
			team0_countryID = 0;
	}

	// Loop around until we get a non-disabled country ID for Allies
	while ( team1_countryID == 0 )		
	{
		team1_countryID = (short)Commands->Get_Random_Int ( 1, 5 ); // Allies, 1 to 5

		if ( team1_countryID == 1 && Get_Int_Parameter ( "enableCountry_America" ) == 0 )
			team1_countryID = 0;
		
		else if ( team1_countryID == 2 && Get_Int_Parameter ( "enableCountry_France" ) == 0 )
			team1_countryID = 0;
		
		else if ( team1_countryID == 3 && Get_Int_Parameter ( "enableCountry_Germany" ) == 0 )
			team1_countryID = 0;
		
		else if ( team1_countryID == 4 && Get_Int_Parameter ( "enableCountry_GreatBritain" ) == 0 )
			team1_countryID = 0;

		else if ( team1_countryID == 5 && Get_Int_Parameter ( "enableCountry_Korea" ) == 0 )
			team1_countryID = 0;
	}

	// Output Soviet country name and cmsg the team what their special ability is
	StringClass countriesMessage ( "The Soviets are playing as ", true );
	StringClass sovietCountryMessage ( true );
	StringClass alliedCountryMessage ( true );
	switch ( team0_countryID )
	{
		case 1:
			countriesMessage += "Russia";
			sovietCountryMessage = "As the Russians you can use the powerful Tesla Tank to wipe out those allied scum!";
			break;
		case 2:
			countriesMessage += "Cuba";
			sovietCountryMessage = "As the Cubans you can use the insane but deadly Terrorist to bomb your enemies!";
			break;
		case 3:
			countriesMessage += "Iraq";
			sovietCountryMessage = "As the Iraqis you can use the deadly Desolator to irradiate the allied scum!";
			break;
		case 4:
			countriesMessage += "Libya";
			sovietCountryMessage = "As the Libiyans you can use the deadly Demolition Truck to destroy enemy forces!";
			break;
	}


	// Output Allied country name and cmsg the team what their special ability is
	countriesMessage += " and the Allies are playing as ";
	switch ( team1_countryID )
	{
		case 1:
			countriesMessage += "America";
			alliedCountryMessage = "As the Americans you can use paradrops to surprise the Soviet forces!";
			break;
		case 2:
			countriesMessage += "France";
			alliedCountryMessage = "As the French your base is well defended by the powerful Grand Cannon";
			break;
		case 3:
			countriesMessage += "Germany";
			alliedCountryMessage = "As the Germans you can use the Tank Destroyer to easily wipe out Soviet tanks.";
			break;
		case 4:
			countriesMessage += "Great Britain";
			alliedCountryMessage = "As the British you can use snipers to pick off enemy infantry from distance";
			break;
		case 5:
			countriesMessage += "Korea";
			alliedCountryMessage = "As the Koreans you can use Black Eagles to rain death from the skies!";
			break;
	}

	// Send public countries message and team country messages
	Send_Message ( 255,255,255, countriesMessage.Peek_Buffer() );
	Send_Message_Team( 0, 255,69,0, sovietCountryMessage.Peek_Buffer() );
	Send_Message_Team( 1, 30,114,255, alliedCountryMessage.Peek_Buffer() );


	// Get mirage tank disguise preset names
	const char* mtDisguise = Get_Parameter( "MirageTank_disguisePreset_1" );
	if ( strcmp ( mtDisguise, "null" ) != 0 && Is_Valid_Preset ( mtDisguise ) )
	{
		mirageTank_disguisePresets[0] = new char[strlen(mtDisguise)+1];
		strcpy ( mirageTank_disguisePresets[0], mtDisguise );
	}

	mtDisguise = Get_Parameter( "MirageTank_disguisePreset_2" );
	if ( strcmp ( mtDisguise, "null" ) != 0 && Is_Valid_Preset ( mtDisguise ) )
	{
		mirageTank_disguisePresets[1] = new char[strlen(mtDisguise)+1];
		strcpy ( mirageTank_disguisePresets[1], mtDisguise );
	}

	mtDisguise = Get_Parameter( "MirageTank_disguisePreset_3" );
	if ( strcmp ( mtDisguise, "null" ) != 0 && Is_Valid_Preset ( mtDisguise ) )
	{
		mirageTank_disguisePresets[2] = new char[strlen(mtDisguise)+1];
		strcpy ( mirageTank_disguisePresets[2], mtDisguise );
	}
}


void dp88_AR_GameController::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
}


void dp88_AR_GameController::Timer_Expired( GameObject *obj, int number )
{
}


void dp88_AR_GameController::Destroyed ( GameObject* obj )
{
	// Clean up memory for Mirage Tank disguises
	if ( mirageTank_disguisePresets[0] != NULL )
	{
		delete [] mirageTank_disguisePresets[0];
		mirageTank_disguisePresets[0] = NULL;
	}
	if ( mirageTank_disguisePresets[1] != NULL )
	{
		delete [] mirageTank_disguisePresets[1];
		mirageTank_disguisePresets[1] = NULL;
	}
	if ( mirageTank_disguisePresets[2] != NULL )
	{
		delete [] mirageTank_disguisePresets[2];
		mirageTank_disguisePresets[2] = NULL;
	}
}




/*------------------------
Default script for all vehicles in AR
--------------------------*/

void dp88_AR_Vehicle::Created(GameObject *obj)
{
	//Console_Output ( "Created dp88_AR_Vehicle\n" );
	pilotID = 0;
	attackingTerrorDroneID = 0;
	
	dead = false;
}


void dp88_AR_Vehicle::Killed( GameObject *obj, GameObject *killer )
{
	/*dead = true;


	// If we have a pilot make sure they know they are no longer a pilot
	if ( pilotID != 0 )
	{
		Commands->Send_Custom_Event( obj,Commands->Find_Object(pilotID),CUSTOM_PILOTED_VEHICLE_ID,0,0 );
		pilotID = 0;
	}

	// If a Terror Drone is attacking us let them know we died...
	if ( attackingTerrorDroneID != 0 )
	{
		Commands->Send_Custom_Event( obj,Commands->Find_Object(attackingTerrorDroneID),CUSTOM_TD_TARGET_DIED,0,0 );
		attackingTerrorDroneID = 0;
	}*/
}



void dp88_AR_Vehicle::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
	{
		if ( pilotID == 0 )
		{
			pilotID = Commands->Get_ID(sender);

			// Send a custom to the driver to make their scripts work properly in
			// vehicle mode, and give them our ID. They will report their veterancy level
			// in return, if nessicary.
			Commands->Send_Custom_Event( obj, sender, CUSTOM_PILOTED_VEHICLE_ID, Commands->Get_ID(obj), 0 );
		}
	}


	// Look for vehicle exit
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED )
	{
		if ( Commands->Get_ID(sender) == pilotID )
		{
			pilotID = 0;

			// Send a custom to the driver to let them know they got out
			Commands->Send_Custom_Event( obj, sender, CUSTOM_PILOTED_VEHICLE_ID, 0, 0 );
		}
	}
	
	
	// Look for a message from the attacking terror drone telling us it died
	/*else if ( type == CUSTOM_TD_TD_DIED )
	{
		attackingTerrorDroneID = 0;
	}*/
}


void dp88_AR_Vehicle::Timer_Expired( GameObject *obj, int number )
{
	// Terror Drone damage timer
	/*if ( number == TIMER_TD_DO_DAMAGE )
	{
		if ( attackingTerrorDroneID != 0 && !dead )
		{
			Commands->Apply_Damage( obj, 40.0, "Steel", Get_Vehicle_Driver ( Commands->Find_Object( attackingTerrorDroneID ) ) );
			Commands->Start_Timer( obj, this, 1.0f, TIMER_TD_DO_DAMAGE );
		}
	}*/
}


void dp88_AR_Vehicle::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	// If damage is repairing, and we are being attacked by a TD, kill the TD
	/*if ( amount < 0 && attackingTerrorDroneID != 0 )
		Commands->Send_Custom_Event( obj, Commands->Find_Object(attackingTerrorDroneID), CUSTOM_TD_TARGET_REPAIRED, 1, 0 );

	
	// If the damage is from the Terror Drone gun set the attacker and start anim
	// Can only be attacked by one TD at a time, and only by opposite team
	const static char *TD_Comparison = "Terror Drone";
	if ( strstr( Commands->Get_Preset_Name( Get_Vehicle(damager) ), TD_Comparison ) != NULL
		&& ( Get_Object_Type(damager) != Get_Object_Type(obj) )
		&& attackingTerrorDroneID == 0
		&& !dead )
	{
		attackingTerrorDroneID = Commands->Get_ID( Get_Vehicle(damager) );
		Commands->Send_Custom_Event( obj, Get_Vehicle(damager), CUSTOM_TD_TARGET_ID, 1, 0 );
		Commands->Set_Animation( obj, Get_Parameter("TD_attack_animName"), false, 0, Get_Float_Parameter("TD_attack_firstFrame"), Get_Float_Parameter("TD_attack_lastFrame"), false );
		Commands->Start_Timer( obj, this, 0.1f, TIMER_TD_DO_DAMAGE );
	}*/
}


void dp88_AR_Vehicle::Animation_Complete ( GameObject *obj, const char *animation_name )
{
	// Restart TD Attack Anim
	/*if ( attackingTerrorDroneID != 0 )
		Commands->Set_Animation( obj, Get_Parameter("TD_attack_animName"), false, 0, Get_Float_Parameter("TD_attack_firstFrame"), Get_Float_Parameter("TD_attack_lastFrame"), false );
		*/
}




/*------------------------
Script for health / armour regeneration based on veterancy
--------------------------*/

void dp88_AR_Veterancy_HealthArmourRegen::Created( GameObject *obj )
{
	// Set initial veterancy level
	veterancyLevel = 0;
	
	// Start timer
	Commands->Start_Timer ( obj, this, 1.0f, TIMER_HEALTHARMOURREGENTICK );
}


void dp88_AR_Veterancy_HealthArmourRegen::Timer_Expired( GameObject *obj, int number )
{
	// Only process this if we are not dead...
	if ( Commands->Get_Health ( obj ) > 0.0f )
	{
		// Check if unit is below full health
		if ( Commands->Get_Health ( obj ) < Commands->Get_Max_Health ( obj ) )
		{
			// Work out how much health we can regenerate
			float healthRegenAmount = (float)(( veterancyLevel == 0 ) ? Get_Int_Parameter ( "rookie_healthRegenAmount" ) : ( veterancyLevel == 1 ) ? Get_Int_Parameter ( "veteran_healthRegenAmount" ) : Get_Int_Parameter ( "elite_healthRegenAmount" ));
		
			// If current health + regen amount > max health set to max health
			if ( Commands->Get_Health ( obj ) + healthRegenAmount >= Commands->Get_Max_Health ( obj ) )
				Commands->Set_Health ( obj, Commands->Get_Max_Health ( obj ) );

			// Otherwise add on health regen amount
			else
				Commands->Set_Health ( obj, Commands->Get_Health( obj ) + healthRegenAmount );
		}

		// Check if unit is below full armour
		if ( Commands->Get_Shield_Strength ( obj ) < Commands->Get_Max_Shield_Strength ( obj ) )
		{
			// Work out how much armour we can regenerate
			float armourRegenAmount = (float)(( veterancyLevel == 0 ) ? Get_Int_Parameter ( "rookie_armourRegenAmount" ) : ( veterancyLevel == 1 ) ? Get_Int_Parameter ( "veteran_armourRegenAmount" ) : Get_Int_Parameter ( "elite_armourRegenAmount" ));

			// If current armour + regen amount > max armour set to max armour
			if ( Commands->Get_Shield_Strength ( obj ) + armourRegenAmount >= Commands->Get_Max_Shield_Strength ( obj ) )
				Commands->Set_Shield_Strength ( obj, Commands->Get_Max_Shield_Strength ( obj ) );

			// Otherwise add on armour regen amount
			else
				Commands->Set_Shield_Strength ( obj, Commands->Get_Shield_Strength( obj ) + armourRegenAmount );
		}

		// Start timer
		Commands->Start_Timer ( obj, this, 1.0f, TIMER_HEALTHARMOURREGENTICK );
	}
}


void dp88_AR_Veterancy_HealthArmourRegen::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// If custom == CUSTOM_VETERANCY_PROMOTED update veterancyLevel
	if ( type == CUSTOM_VETERANCY_PROMOTED )
		veterancyLevel = param;
}






/*------------------------
Grant Default Weapon Scripts
Replaces m00_GrantPowerup_Created
--------------------------*/

void dp88_AR_grantDefaultWeapon::Created( GameObject *obj )
{
	//Console_Output ( "Created dp88_AR_grantDefaultWeapon\n" );
	strcpy_s ( weaponName, sizeof(weaponName), Get_Powerup_Weapon ( Get_Parameter ( "powerupPreset" ) ) );
	strcpy_s ( oldWeapon, sizeof(oldWeapon), "null" );
	Commands->Give_PowerUp ( obj, Get_Parameter ( "powerupPreset" ), true );
}

void dp88_AR_grantDefaultWeapon::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	if ( type == CUSTOM_VETERANCY_PROMOTED )
	{
		// Work out if we need to grant them a new weapon
		char weaponPowerup[64] = "null";
		switch ( param )
		{
			case 1:
				if ( strcmp ( Get_Parameter( "powerupPreset_veteran" ), "null" ) != 0 )
					strcpy_s ( weaponPowerup, sizeof(weaponPowerup), Get_Parameter ( "powerupPreset_veteran" ) );
				break;
			case 2:
				if ( strcmp ( Get_Parameter( "powerupPreset_elite" ), "null" ) != 0 )
					strcpy_s ( weaponPowerup, sizeof(weaponPowerup), Get_Parameter ( "powerupPreset_elite" ) );
				else if ( strcmp ( Get_Parameter( "powerupPreset_veteran" ), "null" ) != 0 )
					strcpy_s ( weaponPowerup, sizeof(weaponPowerup), Get_Parameter ( "powerupPreset_veteran" ) );
				break;
		}

		// If we found a powerup to give them then grant it
		if ( strcmp ( weaponPowerup, "null" ) != 0 )
		{
			if ( Is_Valid_Preset ( weaponPowerup ) )
			{
				// Give new weapon, and select if if they were using that weapon
				if ( strcmp ( Get_Current_Weapon ( obj ), weaponName ) == 0 )
				{
					Commands->Give_PowerUp( obj, weaponPowerup, true );
					strcpy_s ( oldWeapon, sizeof(oldWeapon), weaponName );
					Commands->Start_Timer ( obj, this, 0.1f, TIMER_REMOVE_OLDWEAPON );
					Commands->Select_Weapon ( obj, Get_Powerup_Weapon(weaponPowerup) );
				}
				else
				{
					char curWep[128];
					strcpy_s ( curWep, sizeof(curWep), Get_Current_Weapon ( obj ) );
					Commands->Give_PowerUp( obj, weaponPowerup, true );
					Remove_Weapon ( obj, weaponName );
					Commands->Select_Weapon ( obj, curWep );
				}

				// Set this weapon as their default for future reference
				strcpy_s ( weaponName, sizeof(weaponName), Get_Powerup_Weapon(weaponPowerup) );
			}
		}
	}
}

void dp88_AR_grantDefaultWeapon::Timer_Expired ( GameObject *obj, int number )
{
	if ( number == TIMER_REMOVE_OLDWEAPON && strcmp ( oldWeapon, "null" ) != 0 )
	{
		char curWep[128];
		strcpy_s ( curWep, sizeof(curWep), Get_Current_Weapon ( obj ) );
		if ( strcmp ( curWep, oldWeapon ) == 0 )
		{
			Commands->Select_Weapon ( obj, weaponName );
			Commands->Start_Timer ( obj, this, 0.1f, TIMER_REMOVE_OLDWEAPON );
		}
		else
		{
			Remove_Weapon ( obj, oldWeapon );
			Commands->Select_Weapon ( obj, curWep );
			strcpy_s ( oldWeapon, sizeof(oldWeapon), "null" );
		}
	}
}









/*------------------------
Deployable Infantry scripts
--------------------------*/
void dp88_AR_Deployable_Infantry::Created( GameObject *obj )
{
	//Console_Output ( "Created dp88_AR_Deployable_Infantry\n" );
	objectID = Commands->Get_ID ( obj );
	deployed = false;
	undeployedWeapon = NULL;
	deployedObjectId = NULL;
	lastDeploy = 0;
	currentVetLevel = 0;
	
	// Check deployedObjectPreset is a valid preset name
	if ( !Is_Valid_Preset(Get_Parameter ("deployedObjectPreset")) )
	{
		Send_Message(255,255,255,"dp88_AR_Deployable_Infantry :: Script aborting due to invalid deployedObjectPreset");
		Destroy_Script();
		return;
	}
	
	
	// Validate the weapon powerups
	hasRookieWeaponPowerup = (strcmp ( Get_Parameter( "deployedWeaponPowerup" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter( "deployedWeaponPowerup" ) )) ? true : false; 
	hasVeteranWeaponPowerup = (strcmp ( Get_Parameter( "deployedWeaponPowerup_veteran" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter( "deployedWeaponPowerup_veteran" ) )) ? true : false; 
	hasEliteWeaponPowerup = (strcmp ( Get_Parameter( "deployedWeaponPowerup_elite" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter( "deployedWeaponPowerup_elite" ) )) ? true : false; 
	
	// Store undeployed skin & armour types
	strcpy_s ( undeployedSkinType, sizeof( undeployedSkinType ), Get_Skin ( obj ) );
	strcpy_s ( undeployedArmourType, sizeof( undeployedArmourType ), Get_Shield_Type ( obj ) );


	// Get cannot deploy string and sound IDs
	int soundId = 0;
	cannotDeployStringId = (Is_Valid_String_ID(Get_Int_Parameter("cannotDeployStringId"))) ? Get_Int_Parameter("cannotDeployStringId") : 0;
	if ( cannotDeployStringId && (soundId = Get_String_Sound_ID(cannotDeployStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		cannotDeploySoundId = soundId;


	// Install keyhook
	if ( obj->As_SoldierGameObj() && Get_Player_ID ( obj ) >= 0 )
		InstallHook( Get_Parameter("deployKeyhook"), obj );
}


void dp88_AR_Deployable_Infantry::KeyHook()
{
	// Find object
	GameObject* obj = Commands->Find_Object ( objectID );
	if ( !obj || Get_Vehicle(obj) || obj->As_SoldierGameObj()->Is_On_Ladder() || obj->As_SoldierGameObj()->Is_In_Elevator() )
		return;

	if ( deployed )
	{
		// If deployed for less than undeployTime seconds dont allow undeploy
		if ( time(NULL) - lastDeploy < Get_Float_Parameter("undeployTime") )
		{
			StringClass str;
			str.Format("You cannot undeploy yet, you must wait at least %f seconds after deploying to undeploy.",Get_Float_Parameter("undeployTime"));
			Send_Message_Player(obj,153,204,25,str);
			return;
		}

		// Destroy the defensive structure
		if ( deployedObjectId != NULL )
			Commands->Destroy_Object ( Commands->Find_Object ( deployedObjectId ) );

		deployed = false;
		lastDeploy = time(NULL);

		// Remove deployed weapon
		const char* powerup = GetWeaponPowerup(currentVetLevel);
		if ( powerup )
			Remove_Weapon ( obj, Get_Powerup_Weapon ( powerup ) );
	}
	else
	{
		// If deployed for less than deployTime seconds dont allow deploy
		if ( time(NULL) - lastDeploy < Get_Float_Parameter("deployTime") )
		{
			StringClass str;
			str.Format("You cannot deploy yet, you must wait at least %f seconds after undeploying to deploy again.",Get_Float_Parameter("deployTime"));
			Send_Message_Player(obj,153,204,25,str);
			return;
		}

		// Get velocity of deployer
		Vector3 velocity = Get_Velocity(obj);

		// Find closest other armed object
		GameObject* closest = Get_Closest_Armed_Object_To_Object(obj);

		// Check if we can deploy
		if ( fabs(velocity.X) > 0.001 || fabs(velocity.Y) > 0.001 || fabs(velocity.Z) > 0.001
			|| (closest && Commands->Get_Distance(Commands->Get_Position(obj), Commands->Get_Position(closest)) < Get_Float_Parameter("deployedObjectSpaceRequired") ) )
		{
			const char *str = Get_Translated_String(cannotDeployStringId);
			Send_Message_Player(obj,153,204,25,str);
			delete[] str;
			if ( cannotDeploySoundId )
				Create_2D_Sound_Player(obj, Get_Definition_Name(cannotDeploySoundId));
		}
		else
		{
			// Create the defensive structure
			GameObject* deployedObject = Commands->Create_Object ( Get_Parameter ( "deployedObjectPreset" ), Commands->Get_Position ( obj ) );
			deployedObjectId = Commands->Get_ID ( deployedObject );

			// Grant deployed weapon and select it for use
			const char* powerup = GetWeaponPowerup(currentVetLevel);
			if ( powerup )
			{
				Commands->Give_PowerUp( obj, powerup, true );
				Commands->Select_Weapon ( obj, Get_Powerup_Weapon ( powerup ) );
			}

			// Set armour & skin types
			const char* armourType = GetArmourType(currentVetLevel);
			if ( armourType )
			{
				Commands->Set_Shield_Type(obj, armourType);
				Set_Skin(obj, armourType);
			}

			deployed = true;
			lastDeploy = time(NULL);

			// TEMP - Start timer to check position
			Commands->Start_Timer(obj,this,2.0f,154785);
		}
	}
}


void dp88_AR_Deployable_Infantry::Killed( GameObject *obj, GameObject *killer )
{
	RemoveHook();
	if ( deployedObjectId != NULL )
		Commands->Destroy_Object ( Commands->Find_Object ( deployedObjectId ) );
	deployedObjectId = NULL;
}


void dp88_AR_Deployable_Infantry::Destroyed( GameObject *obj )
{
	if ( deployedObjectId != NULL )
		Commands->Destroy_Object ( Commands->Find_Object ( deployedObjectId ) );
	deployedObjectId = NULL;
}


void dp88_AR_Deployable_Infantry::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
	// We have been promoted, apply new weapon and/or armour types
	if ( sender == obj && type == CUSTOM_VETERANCY_PROMOTED )
	{
		// Remember old vet level
		int oldVetLevel = currentVetLevel;
		
		// Save new level
		currentVetLevel = param;
		
		/* Save new defaults for skin and armour type, unless we are deployed and
		they match the values we set for the old vet level, which implies they
		didn't recieve a new armour type on promotion */
		if ( !deployed || strcmp(GetArmourType(oldVetLevel),Get_Skin(obj)) != 0 )
			strcpy_s ( undeployedSkinType, sizeof( undeployedSkinType ), Get_Skin ( obj ) );
		if ( !deployed || strcmp(GetArmourType(oldVetLevel),Get_Shield_Type(obj)) != 0 )
			strcpy_s ( undeployedArmourType, sizeof( undeployedArmourType ), Get_Shield_Type ( obj ) );
		
		// Set new armour / shield types and weapon if deployed
		if ( deployed )
		{
			const char* armourType = GetArmourType(currentVetLevel);
			if ( armourType )
			{
				Commands->Set_Shield_Type(obj, armourType);
				Set_Skin(obj, armourType);
			}


			// Grant new weapon if necessary
			bool grantWeapon = ((currentVetLevel == 2 && hasEliteWeaponPowerup )||( oldVetLevel == 0 && hasVeteranWeaponPowerup)) ? true : false;
			if ( grantWeapon )
			{
				// Do we need to remove an old deployed weapon?
				if ( (oldVetLevel == 1 && hasVeteranWeaponPowerup) || hasRookieWeaponPowerup )
				{
					// Remove old weapon
					const char* oldWeaponPowerup = GetWeaponPowerup(oldVetLevel);
					Remove_Weapon ( obj, Get_Powerup_Weapon ( oldWeaponPowerup ) );
				}
				// Grant and select new weapon
				const char* powerup = GetWeaponPowerup(currentVetLevel);
				Commands->Give_PowerUp( obj, powerup, true );
				Commands->Select_Weapon ( obj, Get_Powerup_Weapon ( powerup ) );
			}
		}
	}
}


// TEMPORARY - CHECK IF GI MOVES AWAY FROM SANDBAGS
void dp88_AR_Deployable_Infantry::Timer_Expired ( GameObject* obj, int number )
{
	if ( deployed && number == 154785 )
	{
		GameObject* deployedObject = Commands->Find_Object(deployedObjectId);
		float distance = Commands->Get_Distance(Commands->Get_Position(deployedObject),Commands->Get_Position(obj));

		// If more than 3m away, undeploy
		if ( deployedObject != NULL && distance > 1.5f )
		{
			// Destroy the defensive structure
			if ( deployedObjectId != NULL )
				Commands->Destroy_Object ( Commands->Find_Object ( deployedObjectId ) );

			deployed = false;
			lastDeploy = time(NULL);

			// Remove deployed weapon
			if ( Is_Valid_Preset ( Get_Parameter ( "deployedWeaponPowerup" ) ) )
				Remove_Weapon ( obj, Get_Powerup_Weapon ( Get_Parameter ( "deployedWeaponPowerup" ) ) );

			// Remove script to punish abusers
			StringClass message;
			message.Format ("Deployment abuse detected, disabling deploy script... (distance from deployment: %.2fm)", distance);
			Send_Message_Player(obj,255,50,50,message);
			RemoveHook();
			Destroy_Script();
		}


		// Otherwise restart timer
		else
			Commands->Start_Timer(obj,this,2.0f,154785);
	}
}


// Ugly hack to clean up when player purchases new character
void dp88_AR_Deployable_Infantry::Detach(GameObject *obj)
{
	if ( Exe != 4 )	// Don't call destroyed if called from LE, it will crash!
		Destroyed(obj);
	JFW_Key_Hook_Base::Detach(obj);
}


// Get armour type to set for the given veterancy level
const char* dp88_AR_Deployable_Infantry::GetArmourType ( int vetLevel )
{
	if ( vetLevel == 2 && strcmp ( Get_Parameter( "deployedArmourType_elite" ), "null" ) != 0 )
		return Get_Parameter ( "deployedArmourType_elite" );
	else if ( vetLevel >= 1 && strcmp ( Get_Parameter( "deployedArmourType_veteran" ), "null" ) != 0 )
		return Get_Parameter ( "deployedArmourType_veteran" );
	else if ( strcmp ( Get_Parameter( "deployedArmourType" ), "null" ) != 0 )
		return Get_Parameter ( "deployedArmourType" );
	return NULL;
}


// Get weapon powerup to grant for the given veterancy level
const char* dp88_AR_Deployable_Infantry::GetWeaponPowerup ( int vetLevel )
{
	if ( vetLevel == 2 && hasEliteWeaponPowerup )
		return Get_Parameter("deployedWeaponPowerup_elite");
	else if ( vetLevel >= 1 && hasVeteranWeaponPowerup )
		return Get_Parameter("deployedWeaponPowerup_veteran");
	else if ( hasRookieWeaponPowerup )
		return Get_Parameter("deployedWeaponPowerup");
	return NULL;
}




/*------------------------
Chrono Legionnaire Scripts
--------------------------*/

void dp88_AR_CLEG::Created(GameObject *obj)
{
	//Console_Output ( "Created dp88_AR_CLEG\n" );
	phasing = false;
	phasingBack = false;
	timeRemaining = 180;
	zPosDropObj = 0;
	dropObjID = 0;
}

void dp88_AR_CLEG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if ( Commands->Get_ID ( damager ) == Commands->Get_ID ( obj ) && amount == 0.0 )
	{
		Vector3 position = Commands->Get_Position ( obj );

		// If damage = 0 and damager is ourselves then phase to the top plane, or phase
		// back if we are already there.
		if ( !phasing )
		{
			position.Z = 90;
			Commands->Set_Position ( obj, position );
			timeRemaining = 180;

			// Start the clock ticking
			Commands->Start_Timer ( obj, this, 1.0f, TIMER_CLEG_PHASEDEATH );
			phasing = true;
		}
		else if ( dropObjID == 0 )
		{
			position.Z -= 5.0;
			// Time to phase back down again, create the object and start the timer
			// to wait for it to hit the floor
			phasingBack = true;
			GameObject *dropper;
			dropper = Commands->Create_Object( "CLEG_DropObj", position );
			dropObjID = Commands->Get_ID(dropper);
			zPosDropObj = 0;
			Commands->Start_Timer ( obj, this, 0.2f, TIMER_CLEG_CHECKDROPOBJ );
			Send_Message_Player( obj, 153, 204, 25, "Phasing in...." );
		}
	}
}

void dp88_AR_CLEG::Killed(GameObject *obj, GameObject *killer)
{
	if ( dropObjID != 0 )
		Commands->Destroy_Object( Commands->Find_Object( dropObjID ) );
}

void dp88_AR_CLEG::Timer_Expired( GameObject *obj, int number )
{
	if ( number == TIMER_CLEG_CHECKDROPOBJ && dropObjID != 0 )
	{
		Vector3 position = Commands->Get_Position( Commands->Find_Object( dropObjID ) );
		
		if ( (int)zPosDropObj == (int)position.Z )
		{
			Commands->Destroy_Object( Commands->Find_Object( dropObjID ) );
			position.Z += 1;
			Commands->Set_Position( obj, position );
			phasing = false;
			phasingBack = false;
			dropObjID = 0;
			zPosDropObj = 0;
		}
		else
		{
			zPosDropObj = (int)position.Z;
			Commands->Start_Timer( obj, this, 0.2f, TIMER_CLEG_CHECKDROPOBJ );
		}
	}

	else if ( number == TIMER_CLEG_PHASEDEATH )
	{
		if ( phasing == true )
		{
			Commands->Start_Timer ( obj, this, 1.0f, TIMER_CLEG_PHASEDEATH );
			timeRemaining -= 1;
		}

		if ( timeRemaining == 0 && phasing == true && phasingBack != true )
		{
			char deathMSG[128];
			sprintf ( deathMSG, "You have been phased out too long and have died..." );
			Send_Message_Player( obj, 153, 204, 25, deathMSG );
			
			Commands->Apply_Damage( obj, 500.0, "Death", obj );
		}
		else if ( phasing == true && timeRemaining % 20 == 0 || timeRemaining == 10 || timeRemaining == 5 )
		{
			char deathTimer[128];
			sprintf ( deathTimer, "WARNING: You have %d seconds to phase back before you die...", timeRemaining );
			Send_Message_Player( obj, 153, 204, 25, deathTimer );
		}
	}
}

void dp88_AR_CLEG_target::Created(GameObject *obj)
{
	currentResistance = Get_Int_Parameter("resistance")*10;
	lastResistanceCheck = 0;
	effectObjectId = 0;
}

void dp88_AR_CLEG_target::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	const static char *Comparison = "CLEG_Weapon";
	if ( damager
		&& Get_Current_Weapon(damager)
		&& strstr( Get_Current_Weapon(damager), Comparison ) != NULL
		&& ( !Get_Vehicle(damager) )								// Check they are not in a vehicle, as current_weapon will not show this
		&& ( Get_Object_Type(damager) != Get_Object_Type(obj) )		// Check they are not on the same team
		&& currentResistance > 0 )									// Check if we have any CLEG resistance remaining
	{
		// If this is the first damage we have taken in this CLEG attack setup
		// all variables
		if ( lastResistanceCheck == 0 )
		{
			lastResistanceCheck = currentResistance;

			// Start timer to see if we can be released
			Commands->Start_Timer( obj, this, 1.0f, TIMER_CLEG_CHECKRELEASETARGET );

			// Create instance of CLEG effect preset (if applicable)
			const char* effectPreset = Get_Parameter("clegEffectPreset");
			if ( Is_Valid_Preset(effectPreset) )
			{
				GameObject *effectObject = Commands->Create_Object ( effectPreset, Commands->Get_Position(obj) );
				if ( effectObject )
					effectObjectId = Commands->Get_ID ( effectObject );
			}

			// Disable controls for target
			Commands->Control_Enable( obj, false );
		}

		// Reduce remaining resistance
		currentResistance--;
		if ( currentResistance == 0 )
		{
			Commands->Apply_Damage( obj, 5000.0, "Death", damager );

			// Make target 'vanish'
			Commands->Set_Model( obj, "null" );
		}
	}
}

void dp88_AR_CLEG_target::Killed(GameObject *obj, GameObject *killer)
{
	/* Destroy effect object if it exists */
	if ( effectObjectId != 0 )
	{
		GameObject *effectObject = Commands->Find_Object(effectObjectId);
		if ( effectObject )
			Commands->Destroy_Object(effectObject);
		effectObjectId = 0;
	}
}

void dp88_AR_CLEG_target::Timer_Expired( GameObject *obj, int number )
{
	if ( number == TIMER_CLEG_CHECKRELEASETARGET )
	{
		/* If resistance since last check has not changed then CLEG has stopped attacking
		us so free the unit, reset its resistance and destroy any effect object */
		if ( lastResistanceCheck == currentResistance )
		{
			// Reset resistance
			currentResistance = Get_Int_Parameter("resistance")*10;
			lastResistanceCheck = 0;
			
			// Re-enable controls
			Commands->Control_Enable( obj, true );

			/* Destroy effect object if it exists */
			if ( effectObjectId != 0 )
			{
				GameObject *effectObject = Commands->Find_Object(effectObjectId);
				if ( effectObject )
					Commands->Destroy_Object(effectObject);
				effectObjectId = 0;
			}
		}

		/* Otherwise we are still being attacked, update the last check variable and restart
		the timer... */
		else if ( currentResistance > 0 )
		{
			// If we are still being targetted by CLEG(s) start the timer again
			lastResistanceCheck = currentResistance;
			Commands->Start_Timer( obj, this, 1.0f, TIMER_CLEG_CHECKRELEASETARGET );
		}
	}
}




/*------------------------
AR Miner Script - Base Class

This base class is used by both dp88_AR_Chrono_Miner and dp88_AR_War_Miner
and implements common functionality used by both scripts
--------------------------*/

void dp88_AR_Miner::Created ( GameObject *obj )
{
	oreFieldRand = 0;
	oreFieldValue = 0;
	oreLoadLevel = 0;
	oreValue = 0;
	animating = false;

	useAI = ( Get_Int_Parameter ( "enableAI" ) == 1 ) ? true : false;
	aiState = MINER_AISTATE_IDLE;

	/* If this miner is AI controlled then send the ore unload complete event in 10
	seconds, which will cause the AI to set off towards an ore field. Note that the
	delay is required to prevent the normal harvester AI taking over */
	if ( useAI )
		Commands->Send_Custom_Event( obj, obj, CUSTOM_MINER_UNLOAD_ORE_COMPLETE, 1, (float)Get_Int_Parameter("aiStartDelay") );
	if (strlen(Get_Parameter("idleAnimation")) > 0)
		Commands->Set_Animation(obj,Get_Parameter("idleAnimation"),true,0,0,-1,false);
}


void dp88_AR_Miner::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
	// Message from an ore field notifying us that we have entered it. Start
	// collecting ore and call EnteredOreField()
	if ( type == CUSTOM_MINER_ENTERED_ORE_FIELD && oreLoadLevel < Get_Int_Parameter("loadLevels") )
	{
		// Store the value of the ore field and generate a random integer to identify
		// this trip into the ore field - prevents glitching the timers by entering
		// and exiting constantly
		oreFieldValue = param;
		oreFieldRand = Commands->Get_Random_Int(2,10240);

		// Set AI state
		if ( useAI )
			aiState = MINER_AISTATE_COLLECTING_ORE;

		// Send message to increase ore load and, if a player vehicle, inform the driver
		// that we have started collecting ore
		Commands->Send_Custom_Event ( obj, obj, CUSTOM_MINER_INCREASE_ORE_LOAD, oreFieldRand, Get_Float_Parameter("timePerLoadLevel") );
		if ( Get_Vehicle_Driver(obj) != NULL )
			Send_Message_Player ( Get_Vehicle_Driver(obj), 153, 204, 25, "Collecting ore..." );

		// Call EnteredOreField()
		EnteredOreField(obj, sender);
	}


	// Message from an ore field notifying us that we have left it. Stop
	// collecting ore and call ExitedOreField()
	else if ( type == CUSTOM_MINER_EXITED_ORE_FIELD )
	{
		// Reset ore field parameters
		oreFieldValue = 0;
		oreFieldRand = 0;

		// Call ExitedOreField()
		ExitedOreField(obj, sender);
	}


  // Message to ourselves to increase our ore load
  else if ( type == CUSTOM_MINER_INCREASE_ORE_LOAD && param == oreFieldRand )
  {
    // Unless we are already full increase our load level and the value of our load
    if ( oreLoadLevel < Get_Int_Parameter("loadLevels") )
    {
      // Play the mining sound
      if (strlen(Get_Parameter("miningSound")) > 0)
        Commands->Create_Sound(Get_Parameter("miningSound"),Commands->Get_Position(obj),obj);

      oreLoadLevel++;
      oreValue += oreFieldValue*Get_Int_Parameter("orePerLoadLevel");
    }

    // If we are still not full start send a delayed custom to increase it again
    if ( oreLoadLevel < Get_Int_Parameter("loadLevels") )
      Commands->Send_Custom_Event ( obj, obj, CUSTOM_MINER_INCREASE_ORE_LOAD, oreFieldRand, Get_Float_Parameter("timePerLoadLevel") );


    // Otherwise we are full of ore...
    else
    {
      // Stop the mining animation
      animating = false;
      obj->As_PhysicalGameObj()->Clear_Animation();
      if (strlen(Get_Parameter("idleAnimation")) > 0)
         Commands->Set_Animation(obj,Get_Parameter("idleAnimation"),true,0,0,-1,false);
      // If using the AI start driving to the refinery
      if ( useAI )
      {
        aiState = MINER_AISTATE_RETURN_TO_REFINERY;
        ReturnToRefinery(obj);
      }

      // Or, if we are a player driven miner, tell the driver we are full
      else if ( Get_Vehicle_Driver(obj) != NULL )
        Send_Message_Player ( Get_Vehicle_Driver(obj), 153, 204, 25, "Fully loaded with ore, dock at the Refinery to smelt the ore into credits" );
    }
  }


	// Message from the ore dump zone notifying us that we have entered it. If
	// we have ore to unload then immobilize the vehicle and begin unloading it
	else if ( type == CUSTOM_MINER_ENTERED_DUMP_ZONE && oreLoadLevel > 0 )
	{
		// Inform driver we are unloading
		if ( Get_Vehicle_Driver(obj) != NULL )
			Send_Message_Player ( Get_Vehicle_Driver(obj), 153, 204, 25, "Unloading ore, please stand by..." );

		// Set AI state
		if ( useAI )
			aiState = MINER_AISTATE_UNLOADING_ORE;

		// Send a timed event to notify us when the ore unload is completed and
		// call the DockedAtRefinery() event
		Commands->Send_Custom_Event ( obj, obj, CUSTOM_MINER_UNLOAD_ORE_COMPLETE, 0, Get_Float_Parameter("unloadTime") );
		DockedAtRefinery(obj);
	}


	// Message to ourselves to indicate ore unloading is complete, grant money
	// to the team and set off to collect some more ore
	if ( type == CUSTOM_MINER_UNLOAD_ORE_COMPLETE )
	{
		// Inform the driver that we have finished unloading
		if ( Get_Vehicle_Driver(obj) != NULL )
		{
			StringClass message;
			message.Format ("Ore unloaded successfully, you have earned %d credits for each player and %d points for yourself", oreValue, oreValue/10 );
			Send_Message_Player ( Get_Vehicle_Driver(obj), 153, 204, 25, message );
			Commands->Give_Points(Get_Vehicle_Driver(obj),(float)oreValue/10.0f,false);
		}

		// Grant money to team and reset ore load level
		Commands->Give_Money ( obj, (float)oreValue, true );
		oreLoadLevel = 0;
		oreValue = 0;

		// Call UndockedFromRefinery
		UndockedFromRefinery(obj);
	}
}


void dp88_AR_Miner::Action_Complete ( GameObject *obj, int action_id, ActionCompleteReason complete_reason )
{
	// If the completed action was RETURN_TO_REFINERY then set the AI state to
	// docking and call DockAtRefinery();
	if ( action_id == MINER_ACTIONID_RETURN_TO_REFINERY )
	{
		//Console_Output ( "dp88_AR_Miner: Arrived at refinery... start docking\n" );
		aiState = MINER_AISTATE_DOCK_AT_REFINERY;
		DockAtRefinery(obj);
	}
}


void dp88_AR_Miner::DriveToOreField ( GameObject *obj )
{
	/* Find and drive to an ore field */
	GameObject* zone = Find_Closest_Object_With_Script("dp88_AR_Ore_Field_Zone", Commands->Get_Position(obj));
	if ( zone != NULL )
	{
		Vector3 position = Commands->Get_Position(zone);
		//Console_Output ( "dp88_AR_Miner: Driving to location: %.2f, %.2f, %.2f\n", position.X, position.Y, position.Z );

		/* Setup parameters and get going! */
		ActionParamsStruct params;
		params.Set_Basic( this, 100.0f, MINER_ACTIONID_DRIVE_TO_ORE );
		params.Set_Movement ( position, 1.0f, 1.0f );
		params.MovePathfind = true;
		params.ShutdownEngineOnArrival = true;
		params.AttackActive = false;
		Commands->Action_Goto( obj, params );
	}
}


void dp88_AR_Miner::EnteredOreField ( GameObject *obj, GameObject* oreField )
{
  // If we are using AI then reset the action now that we have arrived
  if ( useAI )
    Commands->Action_Reset ( obj, 101.0f );

  // Start the mining animation if it is not already running
  if ((strlen(Get_Parameter("miningAnimation")) > 0) && !animating)
  {
    animating = true;
    Commands->Set_Animation(obj,Get_Parameter("miningAnimation"),true,0,0,-1,false);
  }
}


void dp88_AR_Miner::ExitedOreField ( GameObject *obj, GameObject* oreField )
{
  // Stop the mining animation
  animating = false;
  obj->As_PhysicalGameObj()->Clear_Animation();
  if (strlen(Get_Parameter("idleAnimation")) > 0)
    Commands->Set_Animation(obj,Get_Parameter("idleAnimation"),true,0,0,-1,false);

  // If this is an AI miner and our state is still collecting ore then we were probably shoved out
  // of the ore field by some bully in a vehicle so drive back in to finish mining
  if ( useAI && aiState == MINER_AISTATE_COLLECTING_ORE )
    DriveToOreField(obj);
}


void dp88_AR_Miner::ReturnToRefinery ( GameObject *obj )
{
	/* Find and drive to the refinery */
	GameObject *refinery = Find_Refinery(Commands->Get_Player_Type(obj));
	if ( refinery != NULL )
	{
		GameObject* zone = Find_Closest_Object_With_Script("dp88_AR_Ore_Deposit_Zone", Commands->Get_Position(refinery));
		if ( zone != NULL )
		{
			Vector3 position = Commands->Get_Position(zone);
			//Console_Output ( "dp88_AR_Miner: Driving to location: %.2f, %.2f, %.2f\n", position.X, position.Y, position.Z );

			/* Setup parameters and get going! */
			ActionParamsStruct params;
			params.Set_Basic( this, 100.0f, MINER_ACTIONID_RETURN_TO_REFINERY );
			params.Set_Movement ( position, 1.0f, 25.0f );
			params.MovePathfind = true;
			params.ShutdownEngineOnArrival = true;
			params.AttackActive = false;
			Commands->Action_Goto( obj, params );
		}
	}
}


void dp88_AR_Miner::DockAtRefinery ( GameObject *obj )
{
	// Reset current action
	Commands->Action_Reset(obj, 101.0f);

	/* Find and drive to the refinery unloading area */
	GameObject* zone = Find_Closest_Object_With_Script("dp88_AR_Ore_Deposit_Zone", Commands->Get_Position(obj));
	if ( zone != NULL )
	{
		Vector3 position = Commands->Get_Position(zone);
		//Console_Output ( "dp88_AR_Miner: Docking at location: %.2f, %.2f, %.2f\n", position.X, position.Y, position.Z );

		/* Setup parameters and get going! */
		ActionParamsStruct params;
		params.Set_Basic( this, 100.0f, 3 );
		params.Set_Movement ( position, 1.0f, 1.0f );
		params.MoveBackup = true;
		params.ShutdownEngineOnArrival = true;
		params.MovePathfind = true;
		Commands->Action_Goto(obj, params);
	}
}


void dp88_AR_Miner::DockedAtRefinery ( GameObject *obj )
{
	// Start dock animation and sound
	if (strlen(Get_Parameter("dockAnimation")) > 0)
		Commands->Set_Animation(obj,Get_Parameter("dockAnimation"),false,0,0,-1,false);
	if (strlen(Get_Parameter("dockSound")) > 0)
		Commands->Create_Sound(Get_Parameter("dockSound"),Commands->Get_Position(obj),obj);

	// If we are using AI then reset the action now that we have arrived
	if (useAI)
		Commands->Action_Reset ( obj, 101.0f );

	// Immobilize the vehicle and disable engine sounds
	if ( obj->As_VehicleGameObj() )
	{
		obj->As_VehicleGameObj()->Set_Immovable(true);
		Commands->Enable_Engine ( obj, false );
	}
}


void dp88_AR_Miner::UndockedFromRefinery ( GameObject *obj )
{
	// Un-immobilize the vehicle and enable engine sounds
	if ( obj->As_VehicleGameObj() )
	{
		obj->As_VehicleGameObj()->Set_Immovable(false);
		Commands->Enable_Engine ( obj, true );
	}

	// If using the AI then set the AI state and start driving to the ore field
	if ( useAI )
	{
		aiState = MINER_AISTATE_DRIVE_TO_ORE;
		DriveToOreField(obj);
	}
	obj->As_PhysicalGameObj()->Clear_Animation();
	if (strlen(Get_Parameter("idleAnimation")) > 0)
		Commands->Set_Animation(obj,Get_Parameter("idleAnimation"),true,0,0,-1,false);
}




/*------------------------
Chrono Miner Scripts (AI and non-AI)
--------------------------*/

void dp88_AR_Chrono_Miner::Created( GameObject *obj )
{
	dp88_AR_Miner::Created(obj);
	objectId = Commands->Get_ID(obj);
	driverId = NULL;
}


void dp88_AR_Chrono_Miner::Damaged( GameObject *obj, GameObject *damager, float amount )
{
	// If AI miner health drops below the emergency chronoshift health threshold and we are driving to
	// the ore field or collecting ore then begin an emergency chronoshift
	if ( useAI && Commands->Get_Health(obj) < (Commands->Get_Max_Health(obj)*(Get_Float_Parameter("emergencyChronoshiftHealthThreshold")/100.0f))
		&& (aiState == MINER_AISTATE_COLLECTING_ORE && oreLoadLevel > 0) )
	{
		// Attempt to start a chronoshift - if it fails don't bother
		// with anything else, driving away won't help...
		Start_Chronoshift(obj);
	}
}

	
void dp88_AR_Chrono_Miner::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED && driverId == NULL )
	{
		driverId = Commands->Get_ID(sender);
		InstallHook( Get_Parameter("chronoshiftKeyhook"), sender );
	}


	// Look for vehicle exit
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED && Commands->Get_ID(sender) == driverId )
	{
		driverId = NULL;
		RemoveHook();
	}
	
	
	// AI miner failed to chronoshift back to the refinery, if we are still more than 150m
	// from the target then try again
	else if ( type == CUSTOM_CHRONO_MINER_RETRY_CHRONOSHIFT && aiState == MINER_AISTATE_RETURN_TO_REFINERY )
	{
		GameObject *refinery = Find_Refinery(Commands->Get_Player_Type(obj));
		if ( refinery != NULL && Commands->Get_Distance(Commands->Get_Position(refinery),Commands->Get_Position(obj)) > 150.0f )
		{
			// Try to chronoshift and, on failure, set timer to try again in 5 seconds
			if ( !Start_Chronoshift(obj) )
				Commands->Send_Custom_Event(obj,obj,CUSTOM_CHRONO_MINER_RETRY_CHRONOSHIFT,0,5.0f);
		}
	}
	
	
	// Time to perform a chronoshift!
	else if ( type == CUSTOM_CHRONO_MINER_DO_CHRONOSHIFT )
	{
		Do_Chronoshift(obj,param);
	}


	// Otherwise pass the message on to the base class
	else
		dp88_AR_Miner::Custom ( obj, type, param, sender );
}


void dp88_AR_Chrono_Miner::KeyHook()
{
	// Find object
	GameObject* obj = Commands->Find_Object ( objectId );
	if ( !obj )
		return;

	if ( aiState != CMINER_AISTATE_CHRONOSHIFTING )
	{
		if ( !Start_Chronoshift(obj) )
			Send_Message_Player ( Get_Vehicle_Driver(obj), 153, 204, 25, "Unable to chronoshift, all target zones are unavailable..." );
	}
}


void dp88_AR_Chrono_Miner::ReturnToRefinery ( GameObject *obj )
{
	// Attempt to chronoshift to the refinery
	if ( !Start_Chronoshift(obj) )
	{
		// Can't chronoshift... drive there instead!
		dp88_AR_Miner::ReturnToRefinery(obj);
		Send_Message_Team ( Get_Object_Type(obj), 153, 204, 25, "The AI Chronominer was unable to chronoshift, please clear the area around the refinery" );

		// Send a delayed custom to retry the chronoshift in 5 seconds
		Commands->Send_Custom_Event(obj,obj,CUSTOM_CHRONO_MINER_RETRY_CHRONOSHIFT,0,5.0f);
	}
}


bool dp88_AR_Chrono_Miner::Start_Chronoshift( GameObject *obj )
{
	// If we are currently chronoshifting then bail out
	if ( aiState == CMINER_AISTATE_CHRONOSHIFTING )
		return false;

	/* Find a zone to chronoshift in to */
	GameObject *refinery = Find_Refinery(Commands->Get_Player_Type(obj));
	if ( refinery != NULL )
	{
		// Check the refinery is not dead - if it is then the target script zones will
		// all be gone, so we have nowhere to go...
		if ( Commands->Get_Health(refinery) == 0 )
			return false;

		SList<GameObject> chronoZones;
		Find_All_Objects_With_Script ( "dp88_AR_Chrono_Miner_Chronozone", chronoZones );

		GameObject* zone = NULL;
		dp88_AR_Chrono_Miner_Chronozone *chronozone_script = NULL;

		// Define the maximum distance we will shift from the refinery - this prevents us
		// from going to the enemy refinery
		const float maxDist = 50.0f;

		// Search for the closest zone which we can chronoshift to
		Vector3 pos = Commands->Get_Position(refinery);
		while ( zone == NULL && !chronoZones.Is_Empty() )
		{
			float closestdist = 0.0f;

			// Find the closest zone
			SLNode<GameObject> *x = chronoZones.Head();
			while (x)
			{
				GameObject *o = x->Data();
				x = x->Next();

				float dist = Commands->Get_Distance(Commands->Get_Position(o),pos);
				if ( dist > maxDist )
					chronoZones.Remove(o);

				else if (zone == NULL || dist < closestdist)
				{
					closestdist = dist;
					zone = o;
				}
			}

			if (!zone)
				break;

			// OK, got the closest... can we chronoshift there?

			// Get physical game object and moveable phys class references (used to check if we can move to the zone)
			MoveablePhysClass* mphys = ( obj->As_PhysicalGameObj() ) ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;

			// Can we move to this position without getting stuck?
			if ( !mphys->Can_Teleport( Matrix3D( Commands->Get_Position(zone)) ) )
			{
				// Nope, we can't... eliminate this zone from the list of possibilities and
				// loop around for another try
				chronoZones.Remove(zone);
				zone = NULL;
			}

			// Is this zone in use for another chronoshift operation? If so then we cannot use it
			chronozone_script = (dp88_AR_Chrono_Miner_Chronozone*)Find_Script_On_Object(zone, "dp88_AR_Chrono_Miner_Chronozone");
			if ( !chronozone_script || (chronozone_script->chronominer_id != NULL
										&& Commands->Find_Object(chronozone_script->chronominer_id)
										&& Commands->Get_Health(Commands->Find_Object(chronozone_script->chronominer_id)) > 0) )
			{
				// Yes, it's in use... eliminate this zone from the list of possibilities and
				// loop around for another try
				chronoZones.Remove(zone);
				chronozone_script = NULL;
				zone = NULL;
			}
		}


		// If we failed to find a suitable target zone bail out
		if ( !zone )
			return false;

		// Set the chronominer_id of the script zone to our own ID to prevent other
		// miners trying to use it
		chronozone_script->chronominer_id = Commands->Get_ID(obj);

		// NB: We use the AI state flag to determine if we are currently in the middle of
		// a chronoshift for player driven miners too
		aiState = CMINER_AISTATE_CHRONOSHIFTING;
		
		// Immobilise the vehicle
		if ( obj->As_VehicleGameObj() )
			obj->As_VehicleGameObj()->Set_Immovable(true);

		// Send a delayed custom to perform the chronoshift
		Commands->Send_Custom_Event ( obj, obj, CUSTOM_CHRONO_MINER_DO_CHRONOSHIFT, Commands->Get_ID(zone), Get_Float_Parameter("chronoshift_time") );


		/* If we have an out effect preset to spawn then spawn it at the origin */
		if ( Is_Valid_Preset(Get_Parameter("chronoshift_out_effect_preset")) )
		{
			// Create effect object
			GameObject* effectObject = Commands->Create_Object ( Get_Parameter("chronoshift_out_effect_preset"), Commands->Get_Position(obj) );

			// Attach script to clean up effect
			StringClass params(true);
			params.Format ( "%.2f,%d", Get_Float_Parameter("chronoshift_out_effect_time"), 983142 );
			Commands->Attach_Script ( effectObject, "JFW_Destroy_Self_Timer", params.Peek_Buffer() );
		}


		/* If we have an in effect preset to spawn then spawn it at the destination */
		if ( Is_Valid_Preset(Get_Parameter("chronoshift_in_effect_preset")) )
		{
			// Create effect object
			GameObject* effectObject = Commands->Create_Object ( Get_Parameter("chronoshift_in_effect_preset"), Commands->Get_Position ( zone ) );
			
			// Attach script to clean up effect
			StringClass params(true);
			params.Format ( "%.2f,%d", Get_Float_Parameter("chronoshift_in_effect_time"), 983142 );
			Commands->Attach_Script ( effectObject, "JFW_Destroy_Self_Timer", params.Peek_Buffer() );
		}

		return true;
	}

	return false;
}


void dp88_AR_Chrono_Miner::Do_Chronoshift( GameObject *obj, int target_zone_id )
{
	// Unimmobilise the vehicle
	if ( obj->As_VehicleGameObj() )
		obj->As_VehicleGameObj()->Set_Immovable(false);

	/* Get the target chronoshift zone */
	GameObject *zone = Commands->Find_Object(target_zone_id);
	if ( zone != NULL )
	{
		// Get a reference to the chronozone script and check if the chronominer_id
		// matches ours. If so zero it and continue, otherwise bail out...
		dp88_AR_Chrono_Miner_Chronozone *chronozone_script = (dp88_AR_Chrono_Miner_Chronozone*)Find_Script_On_Object(zone, "dp88_AR_Chrono_Miner_Chronozone");
		if ( !chronozone_script || chronozone_script->chronominer_id != Commands->Get_ID(obj) )
			return;
		
		chronozone_script->chronominer_id = 0;
		
		// NB: We use the AI state flag to determine if we are currently in the middle
		// of a chronoshift for player driven miners too
		aiState = MINER_AISTATE_IDLE;

		// Chronoshift to position of zone (resetting rotation in the process)
		Set_Transform(obj, Matrix3D(Commands->Get_Position(zone)) );
	}


	/* If using AI start driving to refinery now */
	if ( useAI )
	{
		if ( oreLoadLevel > 0 )
		{
			aiState = MINER_AISTATE_RETURN_TO_REFINERY;
			dp88_AR_Miner::ReturnToRefinery(obj);
		}

		// No ore collected... guess we must have chronoshifted away from an attack
		// so lets set off towards the ore field again... what a waste of time!
		else
		{
			aiState = MINER_AISTATE_DRIVE_TO_ORE;
			dp88_AR_Miner::DriveToOreField(obj);
		}
	}
}




/*------------------------
Chronoshift Zone Controller
--------------------------*/

void dp88_AR_Chrono_Miner_Chronozone::Created ( GameObject *obj )
{
	// If the game controller does not exist then bail out
	GameObject *gameController = Find_Object_With_Script("dp88_ar_gameController");
	if ( !gameController )
	{
		Console_Output ( "dp88_AR_Chrono_Miner_Chronozone - Unable to find Game Controller, unable to continue. Destroying script...\n" );
		Destroy_Script();
		return;
	}

	chronominer_id = NULL;
}





/*------------------------
Ore Field Zone Controller
--------------------------*/

void dp88_AR_Ore_Field_Zone::Entered( GameObject *obj, GameObject *enterer )
{
	Commands->Send_Custom_Event( obj, enterer, CUSTOM_MINER_ENTERED_ORE_FIELD, Get_Int_Parameter("oreValue"), 0 );
}

void dp88_AR_Ore_Field_Zone::Exited ( GameObject *obj, GameObject *exiter )
{
	Commands->Send_Custom_Event( obj, exiter, CUSTOM_MINER_EXITED_ORE_FIELD, 0, 0 );
}




/*------------------------
Ore Deposit Zone Controller
--------------------------*/

void dp88_AR_Ore_Deposit_Zone::Entered( GameObject *obj, GameObject *enterer )
{
	if ( Get_Object_Type(enterer) == Get_Int_Parameter( "teamID" ) )
		Commands->Send_Custom_Event( obj, enterer, CUSTOM_MINER_ENTERED_DUMP_ZONE, Get_Int_Parameter( "teamID" ), 0 );
}




/*------------------------
Aircraft Landing Zone Scripts
--------------------------*/

// Landing Zone
void dp88_Aircraft_LandingZone::Entered( GameObject *obj, GameObject *enterer )
{
	Commands->Send_Custom_Event( obj, enterer, CUSTOM_ENTERED_VTOL_LAND_ZONE, 1, 0 );
}

void dp88_Aircraft_LandingZone::Exited( GameObject *obj, GameObject *exiter )
{
	Commands->Send_Custom_Event( obj, exiter, CUSTOM_EXITED_VTOL_LAND_ZONE, 1, 0 );
}


// Landing Zone - Aircraft
void dp88_Aircraft_LandingZone_Aircraft::Created ( GameObject *obj )
{
	driverID = 0;
	landingZoneCount = 0;
}

void dp88_Aircraft_LandingZone_Aircraft::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
	if ( type == CUSTOM_ENTERED_VTOL_LAND_ZONE )
	{
		landingZoneCount++;
		
		// Play landing animation if this is the first zone we have entered
		if ( landingZoneCount == 1 )
			Commands->Set_Animation( obj,Get_Parameter("landing_anim_name"), false, 0, Get_Float_Parameter("landing_anim_first_frame"), Get_Float_Parameter("landing_anim_last_frame"), false );
	}

	else if ( type == CUSTOM_EXITED_VTOL_LAND_ZONE )
	{
		landingZoneCount--;
		
		// Play take off animation if this is the last zone we were in (landing anim in reverse...)
		if ( landingZoneCount == 0 )
			Commands->Set_Animation( obj, Get_Parameter("landing_anim_name"), false, 0, Get_Float_Parameter("landing_anim_last_frame"), Get_Float_Parameter("landing_anim_first_frame"), true );
	}

	else if ( type == CUSTOM_EVENT_VEHICLE_ENTERED && driverID == NULL )
		driverID = Commands->Get_ID(sender);

	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED && Commands->Get_ID(sender) == driverID )
	{
		driverID = NULL;

		// If the driver exited outside of a landing zone then kablooey!
		if ( Get_Int_Parameter("require_landing_zone") >= 1 && landingZoneCount == 0 )
		{
			Commands->Apply_Damage(sender, 10000.00f, "Death", obj );
			Commands->Apply_Damage(obj, 10000.00f, "Death", obj );
		}
	}
}




/*------------------------
Terror Drone Script
--------------------------*/

/*void dp88_AR_TerrorDrone::Created ( GameObject *obj )
{
	//Console_Output ( "Created dp88_AR_TerrorDrone\n" );
	pilotID = 0;
	targetID = 0;
	consoleID = 0;

	// Report our creation to the game controller to pass on to the console
	Commands->Send_Custom_Event ( obj, Find_Object_With_Script("dp88_AR_GameController"), CUSTOM_TD_REPORTCREATION, Commands->Get_ID( obj ), 1 );

	// Remember default model
	strcpy_s ( defaultModel, sizeof(defaultModel), Get_Model ( obj ) );
}

void dp88_AR_TerrorDrone::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
	{
		if ( pilotID == 0 )
			pilotID = Commands->Get_ID(sender);
	}

	// Look for vehicle exit
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED )
	{
		if ( Commands->Get_ID(sender) == pilotID )
		{
			pilotID = 0;

			// Report driver exit to our console
			Commands->Send_Custom_Event( obj, Commands->Find_Object( consoleID ), CUSTOM_TD_DRIVEREXIT, 1, 0 );
		}
	}

	// Look for console reporting
	else if ( type == CUSTOM_TD_CONSOLEID && consoleID == 0 )
	{
		consoleID = Commands->Get_ID ( sender );
	}

	// A target reporting we have successfully latched on to them
	else if ( type == CUSTOM_TD_TARGET_ID )
	{
		targetID = Commands->Get_ID( sender );
		Commands->Set_Model ( obj, "dumbox" );
		Commands->Set_Position ( obj, Commands->Get_Position ( sender ) );
		Commands->Attach_To_Object_Bone ( obj, sender, "worldbox" );
		Commands->Disable_All_Collisions ( obj );
	}

	// A target reporting they have died
	else if ( type == CUSTOM_TD_TARGET_DIED )
	{
		targetID = 0;
		Vector3 newPosition = Commands->Get_Position ( obj );
		newPosition.Z += 0.5;
		Commands->Set_Position ( obj, newPosition );
		Commands->Enable_Collisions ( obj );
		Commands->Set_Model ( obj, defaultModel );
	}

	// A target reporting they have been repaired (kills terror drone)
	else if ( type == CUSTOM_TD_TARGET_REPAIRED )
	{
		Commands->Apply_Damage ( obj, 5000.00, "Death", sender );
	}
}


void dp88_AR_TerrorDrone::Killed ( GameObject *obj, GameObject *killer )
{
	if ( targetID )
		Commands->Send_Custom_Event( obj, Commands->Find_Object( targetID ), CUSTOM_TD_TD_DIED, 1, 0 );
	targetID = 0;

	// Report death to our console
	Commands->Send_Custom_Event( obj, Commands->Find_Object( consoleID ), CUSTOM_TD_TD_DIED, 1, 0 );
}*/




// -------------------------------------------------------------------------------------------------
// Remote Control Console Script
// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::Created ( GameObject *obj )
{
  //Console_Output ( "[%d:%s:%s] Created - remoteControlID %d\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Int_Parameter("remoteControlID") );

  vehicleID         = NULL;
  pilotID           = NULL;
  m_pilotDummyID    = NULL;
  m_bEnabled        = true;
  m_nChargeTime     = Get_Int_Parameter ( "chargeTime" );

  m_pLoopedAnimCtrl = new LoopedAnimationController(obj);

  // Start charge tick timer if necessary and set initial animation frames
  if ( m_nChargeTime > 0 )
  {
    Commands->Start_Timer( obj, this, 1.0f, TIMER_REMOTECONTROL_CHARGETICK);
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), Get_Int_Parameter("animFrameCharging1"), Get_Int_Parameter("animFrameCharging2") );
  }
  else
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), Get_Int_Parameter("animFrameAvailable1"), Get_Int_Parameter("animFrameAvailable2") );

  Commands->Enable_HUD_Pokable_Indicator(obj, (m_nChargeTime == 0));
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::Detach ( GameObject* obj )
{
	if (m_pLoopedAnimCtrl)
	{
		delete m_pLoopedAnimCtrl;
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::Poked ( GameObject *obj, GameObject *poker )
{
	//Console_Output ( "[%d:%s:%s] Poked\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );

	// Check the team of the poker
	int team = Get_Int_Parameter("team");
	if ( team != 2 && Get_Object_Type(poker) != team )
	{
		//Console_Output ( "[%d:%s:%s] Console access denied\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		// Play access denied sound
		Create_2D_Sound_Player(poker, Get_Parameter("accessDeniedSound") );
		return;
	}

	// Check if the terminal is offline
	if ( !m_bEnabled )
	{
		//Console_Output ( "[%d:%s:%s] Console is offline\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		// Play console offline sound
		Create_2D_Sound_Player(poker, Get_Parameter("consoleOfflineSound") );
		return;
	}

	// Check if the terminal is charging
	if ( m_nChargeTime > 0 )
	{
		//Console_Output ( "[%d:%s:%s] Console still charging\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		// Play console charging sound
		Create_2D_Sound_Player(poker, Get_Parameter("consoleChargingSound") );
		return;
	}

	// If no vehicle currently exists then create one
	if ( pilotID == NULL && vehicleID == NULL )
	{
		//Console_Output ( "[%d:%s:%s] BUYING\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		// Try to purchase the unit, will fail if we don't have enough money
		int cost = Get_Int_Parameter ( "cost" );
		if ( !Purchase_Item(poker, cost ) )
		{
			//Console_Output ( "[%d:%s:%s] NO NOOLAH\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
			// Play insufficient funds denied sound
			Create_2D_Sound_Player(poker, Get_Parameter("insufficientFundsSound") );
			return;
		}

		// OK, we have paid for the unit, lets proceed... first glue the driver in place...
		pilotID = Commands->Get_ID( poker );
		Commands->Control_Enable ( poker, false );

		// Disable pokeable indicator
		Commands->Enable_HUD_Pokable_Indicator(obj, false);

		// ...Then create the vehicle...
		//Console_Output ( "[%d:%s:%s] Creating remote control vehicle %s\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter ( "vehiclePreset" ) );
		if (!Create_Vehicle( Get_Parameter ( "vehiclePreset" ), 0.0, Commands->Find_Object( pilotID ), Get_Object_Type( Commands->Find_Object(pilotID) ) ))
		{
			Timer_Expired(obj,TIMER_REMOTECONTROL_TIMEOUT);
		}
		else
		{
			// And wait for confirmation... start a timer in the event that we don't ever get this...
			Commands->Start_Timer ( obj, this, 5.0, TIMER_REMOTECONTROL_TIMEOUT );
		}
	}

	// OK, a vehicle already exists, does it have a pilot? If not then take control of it
	else if ( pilotID == NULL )
	{
		//Console_Output ( "[%d:%s:%s] ENTER EXISTING\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		// Enable transitions and wait until they are active to put driver in
		GameObject* vehicle = Commands->Find_Object(vehicleID);
		if ( vehicle )
		{
			// Stop the pilot from walking away in the meantime
			pilotID = Commands->Get_ID( poker );
			Commands->Control_Enable ( poker, false );

			// Disable pokeable indicator
			Commands->Enable_HUD_Pokable_Indicator(obj, false);

			Commands->Enable_Vehicle_Transitions( vehicle, true );
			Commands->Start_Timer ( obj, this, 0.5, TIMER_REMOTECONTROL_DRIVERENTER );
		}
		else
			vehicleID = NULL;	// This should never actually happen... but just in case...
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	//Console_Output ( "[%d:%s:%s] Custom %d - %d\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), type, param );
	if ( type == CUSTOM_REMOTECONTROL_CREATED && param == Get_Int_Parameter("remoteControlID") && vehicleID == NULL && pilotID != NULL )
	{
		//Console_Output ( "[%d:%s:%s] Sending console ID to unit\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );

		// Inform the remote control unit that this is it's console (we do this to prevent multiple
		// consoles trying to control the same unit if two are created close together)
		Commands->Send_Custom_Event ( obj, sender, CUSTOM_REMOTECONTROL_CONSOLEID, 0, 0.0 );

		// Wait for the remote control unit to accept this as it's console - it will only accept the
		// first response it recieves, so we may not hear back from it...
	}

	else if ( type == CUSTOM_REMOTECONTROL_CONSOLEACK )
	{
		//Console_Output ( "[%d:%s:%s] Recieved ACK from unit, establishing control\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );

		// The remote control unit has accepted us as it's control console
		vehicleID = Commands->Get_ID(sender);
		
		// Put our pilot in the drivers seat
		Commands->Enable_Vehicle_Transitions( sender, true );
		Commands->Start_Timer ( obj, this, 0.5, TIMER_REMOTECONTROL_DRIVERENTER );
	}

	else if ( type == CUSTOM_REMOTECONTROL_DRIVEREXIT )
	{
    DestroyDummy();
    HandleDriverExit ( obj, Commands->Find_Object(pilotID), sender );
    pilotID = NULL;
	}

	else if ( type == CUSTOM_REMOTECONTROL_DESTROYED )
	{
		//Console_Output ( "[%d:%s:%s] Remote control unit was destroyed\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );

		vehicleID = NULL;

    // Make driver invisible and uncollidable so they can't be killed before they are teleported home
    if ( GameObject* pilot = Commands->Find_Object(pilotID) )
    {
      Commands->Set_Is_Rendered ( pilot, false );
      Commands->Disable_All_Collisions ( pilot );
      Update_Network_Object(pilot);
    }

		// Start timer to return the pilot (if we do it immediately the game will crash...)
		if ( pilotID != NULL )
			Commands->Start_Timer ( obj, this, 0.5, TIMER_REMOTECONTROL_DRIVEREXIT );

		// Reset charge time
		m_nChargeTime	= Get_Int_Parameter ( "chargeTime" );

		// Start charge tick timer if necessary and set initial animation frames
		if ( m_bEnabled )
		{
			if ( m_nChargeTime > 0 )
			{
				Commands->Start_Timer( obj, this, 1.0f, TIMER_REMOTECONTROL_CHARGETICK);
        m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), Get_Int_Parameter("animFrameCharging1"), Get_Int_Parameter("animFrameCharging2") );
			}
			else
        m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), Get_Int_Parameter("animFrameAvailable1"), Get_Int_Parameter("animFrameAvailable2") );
		}

		Commands->Enable_HUD_Pokable_Indicator(obj, (m_bEnabled && m_nChargeTime == 0));
	}

	else if ( type == CUSTOM_REMOTECONTROL_DISABLED )
	{
		SetEnabled(obj, false);
	}

	else if ( type == CUSTOM_REMOTECONTROL_ENABLED )
	{
		SetEnabled(obj, true);
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::Timer_Expired ( GameObject *obj, int number )
{
	if ( number == TIMER_REMOTECONTROL_TIMEOUT && vehicleID == 0 )
	{
		//Console_Output ( "[%d:%s:%s] Purchase timed out, refunding money and freeing pilot\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );

		GameObject* pilot = Commands->Find_Object(pilotID);
		if ( pilot )
		{
			Commands->Control_Enable ( pilot, true );
			Commands->Give_Money( pilot, (float)(Get_Int_Parameter ( "cost" )), 0 );
		}
		pilotID = NULL;

		// Enable pokeable indicator
		Commands->Enable_HUD_Pokable_Indicator(obj, (m_bEnabled && m_nChargeTime == 0));
	}


	else if ( number == TIMER_REMOTECONTROL_DRIVERENTER )
	{
		//Console_Output ( "[%d:%s:%s] Transitioning driver into vehicle\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		GameObject* vehicle = Commands->Find_Object(vehicleID);
		GameObject* pilot = Commands->Find_Object(pilotID);
		if ( pilot )
		{
			// Create pilot dummy...
			CreateDummy ( pilot, Commands->Get_Position(pilot), Commands->Get_Facing(pilot) );

			Commands->Control_Enable ( pilot, true );
			Commands->Set_Position ( pilot, Commands->Get_Position ( vehicle ) );
			Soldier_Transition_Vehicle ( pilot );

			// Play control established sound
			Create_2D_Sound_Player(pilot, Get_Parameter("connectionEstablishedSound") );

			// Set animation frame
      m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), Get_Int_Parameter("animFrameActive1"), Get_Int_Parameter("animFrameActive2") );
		}
		else	// This should never actually happen... but just in case...
		{
			Commands->Enable_Vehicle_Transitions( vehicle, false );
			pilotID = NULL;

			// Enable pokeable indicator
			Commands->Enable_HUD_Pokable_Indicator(obj, (m_bEnabled && m_nChargeTime == 0));
		}
	}


	// This can be called for the vehicle being destroyed OR the console becoming disabled
	else if ( number == TIMER_REMOTECONTROL_DRIVEREXIT )
	{
		DestroyDummy();
		HandleDriverExit ( obj, Commands->Find_Object(pilotID), Commands->Find_Object(vehicleID) );
		pilotID = NULL;
	}


	// Count down tick for charge time
	else if ( number == TIMER_REMOTECONTROL_CHARGETICK )
	{
		if ( m_bEnabled )
		{
			m_nChargeTime--;
			if ( m_nChargeTime > 0 )
				Commands->Start_Timer( obj, this, 1.0f, TIMER_REMOTECONTROL_CHARGETICK);
			else
			{
        m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), Get_Int_Parameter("animFrameAvailable1"), Get_Int_Parameter("animFrameAvailable2") );
        Commands->Enable_HUD_Pokable_Indicator(obj, true);
			}
		}
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::SetEnabled ( GameObject* obj, bool state )
{
	if ( state == m_bEnabled )
		return;

	m_bEnabled = state;

	// If we are no longer enabled evict the driver
	if ( !m_bEnabled && pilotID != NULL )
	{
		GameObject* pilot = Commands->Find_Object(pilotID);
		if ( pilot )
		{
      Soldier_Transition_Vehicle ( pilot );
      Create_2D_Sound_Player(pilot, Get_Parameter("consoleOfflineSound") );

      // Make driver invisible and uncollidable so they can't be killed before they are teleported home
      Commands->Set_Is_Rendered ( pilot, false );
      Commands->Disable_All_Collisions ( pilot );
      Update_Network_Object(pilot);

      // Can't do this instantly or the game crashes. Yay!
      Commands->Start_Timer ( obj, this, 0.5, TIMER_REMOTECONTROL_DRIVEREXIT );
		}
	}

	// Set pokeable indicator state
	Commands->Enable_HUD_Pokable_Indicator(obj, (m_bEnabled && m_nChargeTime == 0));

  // Update the animation frame state
  if ( !m_bEnabled )
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), Get_Int_Parameter("animFrameDisabled1"), Get_Int_Parameter("animFrameDisabled2") );

  else if ( m_nChargeTime > 0 )
  {
    Commands->Start_Timer( obj, this, 1.0f, TIMER_REMOTECONTROL_CHARGETICK);
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), Get_Int_Parameter("animFrameCharging1"), Get_Int_Parameter("animFrameCharging2") );
  }

  else
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), Get_Int_Parameter("animFrameAvailable1"), Get_Int_Parameter("animFrameAvailable2") );
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::CreateDummy ( GameObject* pilot, Vector3 position, float facing )
{
	GameObject* dummy = Commands->Create_Object ( "Invisible_Object_2", position );
	if ( dummy )
	{
		m_pilotDummyID = Commands->Get_ID(dummy);
		Commands->Set_Facing(dummy, facing);
		pilotDummyPos = position;	// Used to put the pilot back in his original location

		// Clone health/armour
		Set_Skin(dummy, Get_Skin(pilot) );
		Commands->Set_Shield_Type(dummy, Get_Shield_Type(pilot) );
		Set_Max_Health(dummy, Commands->Get_Max_Health(pilot) );
		Set_Max_Shield_Strength(dummy, Commands->Get_Max_Shield_Strength(pilot) );
		Commands->Set_Health(dummy, Commands->Get_Health(pilot) );
		Commands->Set_Shield_Strength(dummy, Commands->Get_Shield_Strength(pilot) );

		// Link dummy and pilot health and armour
		char pilotIdString[12];
		sprintf ( pilotIdString, "%d", Commands->Get_ID(pilot) );
		Attach_Script_Once ( dummy, "dp88_linkHealth", pilotIdString );

		// Clone player model onto dummy
		Commands->Set_Model ( dummy, Get_Model(pilot) );
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::DestroyDummy()
{
	if ( m_pilotDummyID != NULL )
	{
		GameObject* dummy = Commands->Find_Object(m_pilotDummyID);
		m_pilotDummyID = NULL;

		// Destroy the dummy object rather than killing it - this prevents dp88_linkHealth from also
		// killing the pilot
		if ( dummy )
			Commands->Destroy_Object(dummy);
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlConsole::HandleDriverExit ( GameObject* obj, GameObject* pilot, GameObject* vehicle )
{
  if ( pilot )
  {
    Commands->Set_Position ( pilot, pilotDummyPos );

    // Make driver visible and collidable again
    Commands->Set_Is_Rendered ( pilot, true );
    Commands->Enable_Collisions ( pilot );
    Update_Network_Object(pilot);
  }

	// If the vehicle is still alive set it to its idle state
	if ( vehicle )
	{
		// Set team to correct setting (so it gives points for being damaged)
		Set_Object_Type ( vehicle, Get_Int_Parameter("team") );

		// Disable transitions on remote vehicle
		Commands->Enable_Vehicle_Transitions( vehicle, false );
	}

	// Enable pokeable indicator
	Commands->Enable_HUD_Pokable_Indicator(obj, (m_bEnabled && m_nChargeTime == 0));

  // Set animation frame
  if ( !m_bEnabled )
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), Get_Int_Parameter("animFrameDisabled1"), Get_Int_Parameter("animFrameDisabled2") );

  else if ( vehicle )
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), Get_Int_Parameter("animFrameIdle2"), Get_Int_Parameter("animFrameActive2") );

  else if ( m_nChargeTime > 0 )
  {
    Commands->Start_Timer( obj, this, 1.0f, TIMER_REMOTECONTROL_CHARGETICK);
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), Get_Int_Parameter("animFrameCharging1"), Get_Int_Parameter("animFrameCharging2") );
  }

  else
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), Get_Int_Parameter("animFrameAvailable1"), Get_Int_Parameter("animFrameAvailable2") );
}




// -------------------------------------------------------------------------------------------------
// Remote Control Vehicle Script
// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlVehicle::Created ( GameObject *obj )
{
	//Console_Output ( "[%d:%s:%s] Created - remoteControlID %d\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Int_Parameter("remoteControlID") );

	consoleID = NULL;
	pilotID = NULL;

	Commands->Enable_Vehicle_Transitions(obj, false);

	// Notify all consoles of our creation and disable transitions by default
	Send_Custom_Event_To_Objects_With_Script(obj, "dp88_RemoteControlConsole", CUSTOM_REMOTECONTROL_CREATED, Get_Int_Parameter("remoteControlID"), 0 );
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlVehicle::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
	{
		if ( pilotID == 0 )
			pilotID = Commands->Get_ID(sender);
	}

	// Look for vehicle exit
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED )
	{
		if ( Commands->Get_ID(sender) == pilotID )
		{
			pilotID = 0;

			// Report driver exit to our console
			if ( consoleID != NULL )
				Commands->Send_Custom_Event( obj, Commands->Find_Object( consoleID ), CUSTOM_REMOTECONTROL_DRIVEREXIT, 1, 0 );
		}
	}

	// Look for console reporting and, if we don't already have a console, send an ACK
	// to accept them as our controller
	else if ( type == CUSTOM_REMOTECONTROL_CONSOLEID && consoleID == 0 )
	{
		//Console_Output ( "[%d:%s:%s] Got console, sending ACK\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		consoleID = Commands->Get_ID ( sender );
		Commands->Send_Custom_Event(obj, sender, CUSTOM_REMOTECONTROL_CONSOLEACK, 1 ,0);
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_RemoteControlVehicle::Killed ( GameObject* obj, GameObject* killer )
{
	if ( consoleID != NULL )
		Commands->Send_Custom_Event( obj, Commands->Find_Object( consoleID ), CUSTOM_REMOTECONTROL_DESTROYED, 1, 0 );
}




/*------------------------
Demo Truck Scripts
--------------------------*/

void dp88_AR_DemoTruck::Created( GameObject *obj )
{
	//Console_Output ( "Created dp88_AR_DemoTruck\n" );
	pilotID = 0;
	canDetonate = true;
}

void dp88_AR_DemoTruck::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
	{
		if ( pilotID == 0 )
			pilotID = Commands->Get_ID(sender);
	}

	// Look for vehicle exit
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED )
	{
		if ( Commands->Get_ID(sender) == pilotID && canDetonate )
			pilotID = 0;
	}
}

void dp88_AR_DemoTruck::Killed( GameObject *obj, GameObject *killer )
{
	Detonate( obj );
}

void dp88_AR_DemoTruck::Damaged ( GameObject *obj, GameObject *damager, float amount )
{
	// If we damaged ourselves thats the trigger to detonate
	if ( Commands->Get_ID ( damager ) == Commands->Get_ID ( Get_Vehicle_Driver ( obj ) ) && amount == 0.0 )
		Detonate( obj );
}

void dp88_AR_DemoTruck::Detonate( GameObject *obj )
{
	if ( canDetonate )
	{
		canDetonate = false;

		// Create the explosion
		if ( pilotID != 0 )
		{
			Commands->Create_Explosion ( Get_Parameter ( "explosionPreset" ), 
				Commands->Get_Position ( Commands->Find_Object( pilotID ) ), 
				Commands->Find_Object( pilotID )
			);

			Ranged_Scale_Damage_To_Buildings( 
				Get_Float_Parameter ( "buildingDamageStrength" ), 
				Get_Parameter ( "buildingDamageWarhead" ), 
				Commands->Get_Position( Commands->Find_Object( pilotID ) ), 
				Get_Float_Parameter ( "buildingDamageRange" ), 
				Commands->Find_Object( pilotID )
			);
		}
		
		else
		{
			Commands->Create_Explosion ( Get_Parameter ( "explosionPreset" ), 
				Commands->Get_Position ( obj ), 
				obj
			);


			Ranged_Scale_Damage_To_Buildings( 
				Get_Float_Parameter ( "buildingDamageStrength" ), 
				Get_Parameter ( "buildingDamageWarhead" ), 
				Commands->Get_Position( obj ), 
				Get_Float_Parameter ( "buildingDamageRange" ), 
				obj
			);
		}

		// Create cinematic if nessicary
		if ( strcmp ( Get_Parameter( "cinematicPreset" ), "null" ) != 0 )
			Commands->Create_Object ( Get_Parameter ( "cinematicPreset" ), Commands->Get_Position ( obj ) );

		// Kill driver and truck - can't kill driver instantly as truck protects them
		Commands->Apply_Damage ( obj, 5000.00, "Death", obj );
		if ( pilotID != 0 )
			Commands->Attach_Script( Commands->Find_Object( pilotID ),"RA_DriverDeath", "0" );
	}
}



/*------------------------
Paradrop Scripts
--------------------------*/

void dp88_AR_paradrop_Console::Created ( GameObject *obj )
{
	//Console_Output ( "Created dp88_AR_paradrop_Console\n" );
	last_triggered = 0;
}

void dp88_AR_paradrop_Console::Poked ( GameObject *obj, GameObject *poker )
{
	// Deny use to members of the wrong team (unless team == 2 (ie: anyone))
	if ( Get_Int_Parameter("team") != 2 && Get_Object_Type(poker) != Get_Int_Parameter("team"))
	{
		Send_Message_Player(obj, 153, 204, 25, "Invalid Security Clearance\n");
		return;
	}

	if ( time(NULL)-last_triggered < 180 )
	{
		Send_Message_Player(obj, 153, 204, 25, "This can only be used once every 3 minutes\n");
		return;
	}

	last_triggered = (int)time(NULL);

	// TEMP
	Vector3 pos;
	pos.X = -40.0f;
	pos.Y = 110.0f;
	pos.Z = 40.0f;

	// Change poker into paradrop unit and teleport to position
	Commands->Set_Position(poker,pos);
	Change_Character(poker,"Allied_GI_Paradrop");
	
	// Spawn 3 bots to go with me :)
	for ( int i = 0; i < 3; i++ )
	{
		pos.X += 2;
		pos.Y += 2;
		Commands->Create_Object ( "Allied_GI_Paradrop", pos );
	}
}



void dp88_AR_paradrop::Created( GameObject *obj )
{
	//Console_Output ( "Created dp88_AR_paradrop\n" );
	hit_ground = false;
}

void dp88_AR_paradrop::Damaged( GameObject *obj, GameObject* damager, float amount )
{
	if ( hit_ground )
	{
		/* Spawn new infantry preset */
		if ( Is_Valid_Preset(Get_Parameter("infantry_preset")) )
			Change_Character(obj, Get_Parameter("infantry_preset") );
	}
}

void dp88_AR_paradrop::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	if ( type == CUSTOM_EVENT_FALLING_DAMAGE )
		hit_ground = true;
}







/*------------------------
Prism Tower AI
--------------------------*/

// Static variable initialisation
int dp88_AR_Prism_Tower::prismTowerCount = 0;
int dp88_AR_Prism_Tower::prismTowerIds[MAX_TOWERS] = { 0 };
dp88_AR_Prism_Tower* dp88_AR_Prism_Tower::prismTowerScripts[MAX_TOWERS] = { NULL };

// Static function - register tower
void dp88_AR_Prism_Tower::registerTower(int towerId, dp88_AR_Prism_Tower* script)
{
	// Find an empty slot for this tower
	for ( int i = 0; i < MAX_TOWERS; ++i )
	{
		if ( prismTowerIds[i] == 0 )
		{
			prismTowerIds[i] = towerId;
			prismTowerScripts[i] = script;

			// Recalculate tower map
			calculateTowerMap();
			break;
		}
	}
}

// Static function - deregister tower
void dp88_AR_Prism_Tower::deregisterTower ( int towerId )
{
	// Find this tower in the list and remove it from the array
	for ( int i = 0; i < MAX_TOWERS; ++i )
	{
		if ( prismTowerIds[i] == towerId )
		{
			prismTowerIds[i] = 0;
			clearTowerMap(i);
			prismTowerScripts[i] = NULL;

			// Recalculate tower map
			calculateTowerMap();
			break;
		}
	}
}

// Static function - clear tower map for a given index
void dp88_AR_Prism_Tower::clearTowerMap ( int idx )
{
	if ( idx < MAX_TOWERS )
	{
		prismTowerScripts[idx]->adjacentTowerCount = 0;
		if ( prismTowerScripts[idx]->adjacentTowers != NULL )
		{
			delete [] prismTowerScripts[idx]->adjacentTowers;
			prismTowerScripts[idx]->adjacentTowers = NULL;
		}
	}
}

// Static function - calculate tower map
void dp88_AR_Prism_Tower::calculateTowerMap()
{
	// Count number of registered towers and reset their adjacent tower indexes
	int numTowers = 0;
	for ( int i = 0; i < MAX_TOWERS; ++i )
	{
		if ( prismTowerIds[i] > 0 )
		{
			numTowers++;
			clearTowerMap(i);
		}
	}


	// Build array of all possible connections between registered towers - maximum
	// number of nodes is n(n-1)/2 (IE: Complete graph), but unless the towers are
	// very close together we will probably use less than this
	int maxConnections = (int)(numTowers*((numTowers-1.0f)/2.0f));
	int* towerConnections = new int[maxConnections*3];
	memset(towerConnections,0,sizeof(int)*maxConnections*3);


	// Populate array
	int connectionCount = 0;
	for ( int i = 0; i < MAX_TOWERS; ++i )
	{
		if ( prismTowerIds[i] > 0 )
		{
			GameObject* t1 = Commands->Find_Object(prismTowerIds[i]);
			for ( int j = i+1; j < MAX_TOWERS; ++j )
			{
				if ( prismTowerIds[j] > 0 )
				{
					GameObject* t2 = Commands->Find_Object(prismTowerIds[j]);
					
					// If these towers are within firing range of each other then add
					// them to the connections list
					int distance = (int)Commands->Get_Distance(Commands->Get_Position(t1),Commands->Get_Position(t2));
					if ( distance < ((dp88_AR_Prism_Tower*)prismTowerScripts[i])->primary_maxRange )
					{
						towerConnections[connectionCount*3] = i;
						towerConnections[(connectionCount*3)+1] = j;
						towerConnections[(connectionCount*3)+2] = distance;
						connectionCount++;
					}
				}
			}
		}
	}


	// Now sort the entries in descending order and add a fourth field to
	// allow the connections to be marked as 'bad' during the reverse
	// delete algorithm
	int* sortedTowerConnections = new int[connectionCount*4];
	memset(sortedTowerConnections,0,sizeof(int)*connectionCount*4);


	// Copy connections to the new array one by one, starting with the largest
	// value and ending with the smallest... yes, this is probably a horribly
	// inefficient way of doing this but it only occasionally and it works...
	for ( int i = 0; i < connectionCount; ++i )
	{
		int maxDist = 0;
		int maxDistIndex = -1;
		for ( int j = 0; j < connectionCount; ++j )
		{
			if ( towerConnections[j*3] != -1 && towerConnections[(j*3)+2] > maxDist )
			{
				maxDist = towerConnections[(j*3)+2];
				maxDistIndex = j;
			}
		}

		// Copy highest index and set it's first tower ID to 0 so we skip past
		// it in subsequent iterations
		if ( maxDistIndex != -1 )
		{
			sortedTowerConnections[i*4] = towerConnections[maxDistIndex*3];
			sortedTowerConnections[(i*4)+1] = towerConnections[(maxDistIndex*3)+1];
			sortedTowerConnections[(i*4)+2] = towerConnections[(maxDistIndex*3)+2];
			sortedTowerConnections[(i*4)+3] = 0;	// Not a 'bad' connection

			// Remove the copied item from the sort
			towerConnections[maxDistIndex*3] = -1;
		}
	}

	// Get rid of tower connections
	delete [] towerConnections;


	// OK, we have a sorted list of connections, now iterate through it and
	// for each connection see if there is another path between the towers it
	// links. If there is then get rid of this connection
	for ( int i = 0; i < connectionCount; ++i )
	{
		// Mark the connection as bad
		sortedTowerConnections[(i*4)+3] = 1;

		// Check to see if another path exists between these towers - if it
		// does not then this path is required for the minimum spanning tree
		// so restore it as a 'good' path
		if ( !calculateTowerMapPathSearch(sortedTowerConnections,connectionCount,sortedTowerConnections[i*4],sortedTowerConnections[(i*4)+1]) )
			sortedTowerConnections[(i*4)+3] = 0;
	}


	// OK, we now have a minimum spanning tree... update each tower with
	// a list of their neighbours
	for ( int i = 0; i < connectionCount; ++i )
	{
		// Ignore 'bad' connections
		if ( sortedTowerConnections[(i*4)+3] != 1 )
		{
			int tower1idx = sortedTowerConnections[i*4];
			int tower1id = prismTowerIds[tower1idx];
			dp88_AR_Prism_Tower* tower1script = prismTowerScripts[tower1idx];
			int tower2idx = sortedTowerConnections[(i*4)+1];
			int tower2id = prismTowerIds[tower2idx];
			dp88_AR_Prism_Tower* tower2script = prismTowerScripts[tower2idx];


			// Add tower2 to tower1's adjacent towers list
			int* oldAdjacentTowers = tower1script->adjacentTowers;
			tower1script->adjacentTowers = new int[++tower1script->adjacentTowerCount];

			// Copy existing data
			for ( int j = 0; j < tower1script->adjacentTowerCount-1; ++j )
				tower1script->adjacentTowers[j] = oldAdjacentTowers[j];

			// Delete old array
			if ( oldAdjacentTowers != NULL )
				delete [] oldAdjacentTowers;

			// Add new adjacent tower
			tower1script->adjacentTowers[tower1script->adjacentTowerCount-1] = tower2id;




			// Add tower1 to tower2's adjacent towers list
			oldAdjacentTowers = tower2script->adjacentTowers;
			tower2script->adjacentTowers = new int[++tower2script->adjacentTowerCount];

			// Copy existing data
			for ( int j = 0; j < tower2script->adjacentTowerCount-1; ++j )
				tower2script->adjacentTowers[j] = oldAdjacentTowers[j];

			// Delete old array
			if ( oldAdjacentTowers != NULL )
				delete [] oldAdjacentTowers;

			// Add new adjacent tower
			tower2script->adjacentTowers[tower2script->adjacentTowerCount-1] = tower1id;
		}
	}

	// Get rid of sorted tower connections
	delete [] sortedTowerConnections;
}


bool dp88_AR_Prism_Tower::calculateTowerMapPathSearch(int* sortedConnections, int numConnections, int tower1, int tower2)
{
	// Try to find a path between these towers
	for ( int i = 0; i < numConnections; ++i )
	{
		// Ignore 'bad' connections
		if ( sortedConnections[(i*4)+3] != 1 )
		{
			// If this is a connection between the towers we are searching for then
			// return success
			if ( sortedConnections[i*4] == tower1 && sortedConnections[(i*4)+1] == tower2
				|| sortedConnections[i*4] == tower2 && sortedConnections[(i*4)+1] == tower1 )
				return true;

			// If one of the towers in this connection matches tower 1 then look for
			// a path between the other tower in the connection and tower 2. We do this
			// by temporaraily marking the connection as 'bad' before recursing, otherwise
			// we cause an infinite recursion, which is bad!
			if ( sortedConnections[i*4] == tower1 )
			{
				sortedConnections[(i*4)+3] = 1;
				bool result = calculateTowerMapPathSearch(sortedConnections,numConnections,tower2,sortedConnections[(i*4)+1]);
				sortedConnections[(i*4)+3] = 0;
				return result;
			}
			if ( sortedConnections[(i*4)+1] == tower1 )
			{
				sortedConnections[(i*4)+3] = 1;
				bool result = calculateTowerMapPathSearch(sortedConnections,numConnections,tower2,sortedConnections[i*4]);
				sortedConnections[(i*4)+3] = 0;
				return result;
			}

			// Ditto for tower 2
			if ( sortedConnections[i*4] == tower2 )
			{
				sortedConnections[(i*4)+3] = 1;
				bool result = calculateTowerMapPathSearch(sortedConnections,numConnections,tower1,sortedConnections[(i*4)+1]);
				sortedConnections[(i*4)+3] = 0;
				return result;
			}
			if ( sortedConnections[(i*4)+1] == tower2 )
			{
				sortedConnections[(i*4)+3] = 1;
				bool result = calculateTowerMapPathSearch(sortedConnections,numConnections,tower1,sortedConnections[i*4]);
				sortedConnections[(i*4)+3] = 0;
				return result;
			}
		}
	}

	// Nothing found...
	return false;
}








void dp88_AR_Prism_Tower::Created( GameObject *obj )
{
  dp88_AI_ChargedTurret::Created(obj);

  // Initialise member variables
  isAssistingTower = false;
  adjacentTowerCount = 0;
  adjacentTowers = NULL;

  // Register the tower
  registerTower(Commands->Get_ID(obj),this);
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::Damaged ( GameObject *obj, GameObject *damager, float amount )
{
  // If the damager is a prism tower then we have been charged, add 1 to ammo count
  if ( Is_Script_Attached ( damager, "dp88_AR_Prism_Tower" ) )
  {
    /* Increase ammo count by 1 */
    if ( Get_Current_Weapon(obj) != NULL )
      Set_Current_Bullets(obj,Get_Current_Bullets(obj)+1);
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::Killed ( GameObject *obj, GameObject *killer )
{
  // Notify adjacent towers to stop assisting us if they were
  for ( int i = 0; i < adjacentTowerCount; i++ )
  {
    if (Commands->Find_Object(adjacentTowers[i]) != obj)
    {
      Commands->Send_Custom_Event(obj,Commands->Find_Object(adjacentTowers[i]), CUSTOM_PRISMTOWER_STOP_CHARGING,0,0.0f);
    }
  }

  // Deregister this tower
  deregisterTower(Commands->Get_ID(obj));
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::Destroyed ( GameObject *obj )
{
  // Deregister this tower
  deregisterTower(Commands->Get_ID(obj));
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
  // If we recieve a stop charging message from the tower we are currently
  // charging then we should stop all actions
  if ( type == CUSTOM_PRISMTOWER_STOP_CHARGING && isAssistingTower && Commands->Get_ID(sender) == targetID )
    StopAssisting(obj);


  // If we recieve a request charging message from another tower then we
  // should check to see if their target has higher priority than our
  // current target (if any) and, if so, start charging them
  else if ( type == CUSTOM_PRISMTOWER_REQUEST_CHARGING )
  {
    // Are we idle? If so then there's no reason not to simply start charging immediatly
    if ( targetID == 0 )
      StartAssisting(obj, sender, (float)param);

    // Is this request from the tower we are currently charging? If so then update the last seen
    // time and priority and forward the assistance request to any other adjacent towers
    else if ( isAssistingTower && targetID == Commands->Get_ID(sender) )
    {
      targetLastSeen = (int)time(NULL);
      targetPriority = (float)param;
      SendAssistanceRequests(obj);
      return;
    }

    // Is the priority of the request greater than the priority of our current target (either
    // another tower or an enemy)? If so then stop what we are doing and charge the requester
    else if ( param > targetPriority )
      StartAssisting(obj, sender, (float)param );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::Timer_Expired ( GameObject *obj, int number )
{
  // Piggy back our assistance polling and charge refilling on the existing think timer
  if ( number == TIMER_CUSTOMAI_THINK )
  {
    /* Send out assistance requests to ensure other towers don't time out on the 'last seen' check
    *
    * \todo
    *   Can this screw up the timings between charging towers and the attacking tower? Need to think
    *   this through sometime and possibly do some experimentation...
    */
    if ( targetID )
      SendAssistanceRequests(obj);

    /* Refill a single unit of charge if depleted */
    if ( Get_Current_Clip_Bullets(obj) == 0 )
      Set_Current_Clip_Bullets(obj,1);

    /* If current bullets > 1 and no enemy seen recently then additional charge is lost */
    if ( Get_Current_Bullets(obj) > 1 && targetID == 0 )
      Set_Current_Bullets(obj,1);
  }


  // Pass the timer onto the base class to be handled
  dp88_AI_ChargedTurret::Timer_Expired(obj, number);
}

// -------------------------------------------------------------------------------------------------

float dp88_AR_Prism_Tower::getPriority( GameObject *obj, GameObject *target )
{
  // If the target is the tower we are currently charging then return
  // the priority of that charging sequence
  if ( Commands->Get_ID(target) == targetID && isAssistingTower )
    return targetPriority;

  // Otherwise run the normal priority calculation
  return dp88_AI_ChargedTurret::getPriority(obj, target);
}

// -------------------------------------------------------------------------------------------------

bool dp88_AR_Prism_Tower::checkTeam( GameObject *obj, GameObject *target )
{
  // Return true for the tower we are charging, even though it is on the same team as us
  if ( Commands->Get_ID(target) == targetID && isAssistingTower )
    return true;

  // Otherwise run the normal check team function
  return dp88_AI_ChargedTurret::checkTeam(obj, target);
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::attackTarget ( GameObject* obj, GameObject* target, bool primary )	
{
  // Call base classes attack routine
  dp88_AI_ChargedTurret::attackTarget(obj, target, primary);

  // Send notifications for assistance
  SendAssistanceRequests(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::attackLocation ( GameObject* obj, Vector3 location, bool primary )
{
  // Call base classes attack routine
  dp88_AI_ChargedTurret::attackLocation(obj, location, primary);

  // Send notifications for assistance
  SendAssistanceRequests(obj);
}

// -------------------------------------------------------------------------------------------------


void dp88_AR_Prism_Tower::stopAttacking ( GameObject* obj )
{
  // Was the target a tower we were charging? If so this means they must have timed out on the last
  // seen and are therefore probably dead...
  if ( isAssistingTower )
    StopAssisting(obj);

  // Otherwise we were attacking a normal target, send out end assistance notifications to all
  // adjacent towers and call base class
  else
  {
    SendEndAssistanceNotifications(obj);
    dp88_AI_ChargedTurret::stopAttacking(obj);
  }
}

// -------------------------------------------------------------------------------------------------

/* Start charging the specified tower */
void dp88_AR_Prism_Tower::StartAssisting(GameObject* obj, GameObject* tower, float priority)
{
  // Set our new target ID and priority
  targetID = Commands->Get_ID(tower);
  targetPriority = priority;
  targetLastSeen = (int)time(NULL);
  isAssistingTower = true;

  // Work out attack position
  Vector3 chargePos = Commands->Get_Bone_Position(tower,"turret");
  chargePos.Z += 1;

  // Start charging the tower - this will also trigger assistance notifications to be sent out
  attackLocation ( obj, chargePos, true );
}

// -------------------------------------------------------------------------------------------------

/* Stop charging the specified tower */
void dp88_AR_Prism_Tower::StopAssisting(GameObject* obj)
{
  if ( isAssistingTower )
  {
    targetID = NULL;
    targetPriority = 0;
    isAssistingTower = false;
    Commands->Action_Reset(obj, 101.0f);
  }

  // Send end assistance notifications
  SendEndAssistanceNotifications(obj);
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::SendAssistanceRequests(GameObject* obj)
{
  // Send out assistance requests to all adjacent towers except the one we are charging, if any
  for ( int i = 0; i < adjacentTowerCount; ++i )
  {
    if ( adjacentTowers[i] != targetID && Commands->Find_Object(adjacentTowers[i]) != obj)
      Commands->Send_Custom_Event(obj, Commands->Find_Object(adjacentTowers[i]), CUSTOM_PRISMTOWER_REQUEST_CHARGING, (int)targetPriority, 0.0f );
  }
}

// -------------------------------------------------------------------------------------------------

void dp88_AR_Prism_Tower::SendEndAssistanceNotifications(GameObject* obj)
{
  // Send out end assistance notifications to all adjacent towers
  for ( int i = 0; i < adjacentTowerCount; ++i )
  {
    if (Commands->Find_Object(adjacentTowers[i]) != obj)
    {
      Commands->Send_Custom_Event(obj, Commands->Find_Object(adjacentTowers[i]), CUSTOM_PRISMTOWER_STOP_CHARGING, 0, 0.0f );
    }
  }
}

// -------------------------------------------------------------------------------------------------




/*------------------------
Health Link Script

Maintains the same health & armour between two objects, for example
the AI warminer and its gun turret
--------------------------*/

void dp88_linkHealth::Created ( GameObject* obj )
{
	parentObjID = Get_Int_Parameter ( "parentObjectId" );
	GameObject* parent = Commands->Find_Object(parentObjID);
	if ( !parent )
	{
		Destroy_Script();
		return;
	}

	// Set initial values for lastHealth / lastArmour
	lastHealth = Commands->Get_Health ( parent );
	lastArmour = Commands->Get_Shield_Strength ( parent );

	// Set our max / current health / armour to match our parent
	Set_Max_Health ( obj, Commands->Get_Max_Health(parent) );
	Commands->Set_Health ( obj, lastHealth );
	Set_Max_Shield_Strength ( obj, Commands->Get_Max_Shield_Strength(parent) );
	Commands->Set_Shield_Strength ( obj, lastArmour );

	// Start timer
	Commands->Start_Timer ( obj, this, 0.25, TIMER_LINKHEALTH );
}

void dp88_linkHealth::Timer_Expired ( GameObject* obj, int number )
{
	equaliseHealth( obj );

	// Restart timer if still alive
	if ( lastHealth + lastArmour > 0.0f )
		Commands->Start_Timer ( obj, this, 0.25, TIMER_LINKHEALTH );
}

void dp88_linkHealth::Killed ( GameObject* obj, GameObject *killer )
{
	// Destroy parent object if its still alive
	GameObject* parent = Commands->Find_Object ( parentObjID );
	if ( parent )
	{
    // If the parent is a soldier in a vehicle we can't simply kill them because things go horribly wrong. Instead
    // eject them from a vehicle and attach JFW_Timer_Destroy_Object to kill them as soon as possible.
    if ( parent->As_SoldierGameObj() && parent->As_SoldierGameObj()->Is_In_Vehicle() )
    {
      Soldier_Transition_Vehicle ( parent );
      Commands->Attach_Script ( parent, "JFW_Timer_Destroy_Object", "1.0,547859,5000.0,Death" );
      Destroy_Script();   // Prevent equaliseHealth() getting called and trying to kill the driver
    }
		else
		    Commands->Apply_Damage ( parent, 5000.00, "None", NULL );
	}
}

void dp88_linkHealth::equaliseHealth ( GameObject* obj )
{
	GameObject* parent = Commands->Find_Object ( parentObjID );
	if ( !parent )
	{
		Commands->Apply_Damage ( obj, 5000.00, "Death", obj );
		return;
	}

	/* Work out difference in our health and armour since last check */
	float difference = ((Commands->Get_Health(obj)-lastHealth)+(Commands->Get_Shield_Strength(obj)-lastArmour));

	/* Apply damage to parent using 'None' warhead (this should be 1-1 ratio against all skin and
  shield types. Special case scenario: if the target is a soldier in a vehicle we need to use a
  special function to override the check that prevents damage to soldiers in a vehicle */
  if ( parent->As_SoldierGameObj() && parent->As_SoldierGameObj()->Is_In_Vehicle() )
  {
    OffenseObjectClass offense(difference*-1.0f, ArmorWarheadManager::Get_Warhead_Type("None"), NULL);
    parent->As_SoldierGameObj()->Apply_Damage_IgnoreVehicleCheck ( offense, 1, -1 );
  }
  else
    Commands->Apply_Damage ( parent, difference*-1.0f, "None", NULL );

	/* Update our health/armour from the parents new health/armour */
	Commands->Set_Health ( obj, Commands->Get_Health(parent) );
	Commands->Set_Shield_Strength ( obj, Commands->Get_Shield_Strength(parent) );

	/* Set lastHealth / lastArmour */
	lastHealth = Commands->Get_Health(obj);
	lastArmour = Commands->Get_Shield_Strength(obj);

	/* Are we dead? */
	if ( lastHealth <= 0.0f )
	{
		Commands->Set_Health ( obj, 1.0f );
		Commands->Apply_Damage ( obj, 5000.00f, "Death", obj );
		Commands->Set_Health ( parent, 1.0f );
		Commands->Apply_Damage ( parent, 5000.00f, "Death", NULL );
	}

	/* Otherwise apply 0 damage to ourselves to trigger any on-damage events
	that need to run, such as dp88_damageAnimation() */
	Commands->Apply_Damage ( obj, 0.0, "None", obj );
}






/*------------------------
Script Registrants
--------------------------*/

// Game Controller
ScriptRegistrant<dp88_AR_GameController> dp88_AR_GameController_Registrant( "dp88_AR_GameController", "enableCountry_Russia=1:int,enableCountry_Cuba=1:int,enableCountry_Iraq=1:int,enableCountry_Libya=1:int,enableCountry_America=1:int,enableCountry_France=1:int,enableCountry_Germany=1:int,enableCountry_GreatBritain=1:int,enableCountry_Korea=1:int,MirageTank_disguisePreset_1=mt_tree:string,MirageTank_disguisePreset_2=null:string,MirageTank_disguisePreset_3=null:string,warminer_refToOre=0:int,warminer_oreToRef=0:int,warminer_unloadOre=0:int,chronominer_refToOre=0:int,chronominer_unloadOre=0:int" );

// Unit scripts
//ScriptRegistrant<dp88_AR_Vehicle> dp88_AR_Vehicle_Registrant( "dp88_AR_Vehicle", "TD_attack_animName=modelfile.animfile:string,TD_attack_firstFrame=0.0:float,TD_attack_lastFrame=30.0:float,CLEG_Resistance=10:int" );


// Replacement for m00_GrantPowerup_Created
ScriptRegistrant<dp88_AR_grantDefaultWeapon> dp88_AR_grantDefaultWeapon_Registrant("dp88_AR_grantDefaultWeapon","powerupPreset=presetname:string,powerupPreset_veteran=null:string,powerupPreset_elite=null:string");

// Veterancy stuff
ScriptRegistrant<dp88_AR_Veterancy_HealthArmourRegen> dp88_AR_Veterancy_HealthArmourRegen_Registrant("dp88_AR_Veterancy_HealthArmourRegen","rookie_healthRegenAmount=0:int,rookie_armourRegenAmount=0:int,veteran_healthRegenAmount=1:int,veteran_armourRegenAmount=0:int,elite_healthRegenAmount=3:int,elite_armourRegenAmount=2:int");

// Deployable Infantry
ScriptRegistrant<dp88_AR_Deployable_Infantry> dp88_AR_Deployable_Infantry_Registrant("dp88_AR_Deployable_Infantry","deployedObjectPreset=null:string,deployedObjectSpaceRequired=6:float,deployAnimation=obj.obj:string,deployTime=4:float,undeployAnimation=obj.obj:string,undeployTime=4:float,deployedWeaponPowerup=null:string,deployedWeaponPowerup_veteran=null:string,deployedWeaponPowerup_elite=null:string,cannotDeployStringId=0:int,deployKeyhook=IDeploy:string,deployedArmourType=null:string,deployedArmourType_veteran=null:string,deployedArmourType_elite=null:string");

// CLEG
ScriptRegistrant<dp88_AR_CLEG> dp88_AR_CLEG_Registrant("dp88_AR_CLEG","");

// CLEG resistance script
ScriptRegistrant<dp88_AR_CLEG_target> dp88_AR_CLEG_target_Registrant( "dp88_AR_CLEG_target", "resistance=20:int,clegEffectPreset=null:string" );

// Ore Miners
ScriptRegistrant<dp88_AR_Miner> dp88_AR_Miner_Registrant("dp88_AR_War_Miner","enableAI=1:int,loadLevels=10:int,orePerLoadLevel=100:int,timePerLoadLevel=2.0:float,unloadTime=8.0:float,aiStartDelay=10:int,dockAnimation:string,dockSound:string,miningAnimation:String,miningSound:string,idleAnimation:string");
ScriptRegistrant<dp88_AR_Chrono_Miner> dp88_AR_Chrono_Miner_Registrant("dp88_AR_Chrono_Miner","enableAI=1:int,chronoshift_time=2.5:float,chronoshift_out_effect_preset:string,chronoshift_out_effect_time:float,chronoshift_in_effect_preset:string,chronoshift_in_effect_time:float,chronoshiftKeyhook=VDeploy:string,loadLevels=10:int,orePerLoadLevel=50:int,timePerLoadLevel=1.00:float,unloadTime=4.0:float,emergencyChronoshiftHealthThreshold=30.0:float,aiStartDelay=10:int,dockAnimation:string,dockSound:string,miningAnimation:String,miningSound:string,idleAnimation:string");
ScriptRegistrant<dp88_AR_Chrono_Miner_Chronozone> dp88_AR_Chrono_Miner_Chronozone_Registrant("dp88_AR_Chrono_Miner_Chronozone","");
ScriptRegistrant<dp88_AR_Ore_Field_Zone> dp88_AR_Ore_Field_Zone_Registrant("dp88_AR_Ore_Field_Zone","oreValue=1:int");
ScriptRegistrant<dp88_AR_Ore_Deposit_Zone> dp88_AR_Ore_Deposit_Zone_Registrant("dp88_AR_Ore_Deposit_Zone","teamID=0:int");


// Aircraft Landing Zone
ScriptRegistrant<dp88_Aircraft_LandingZone> dp88_Aircraft_LandingZone_Registrant("dp88_Aircraft_LandingZone","");
ScriptRegistrant<dp88_Aircraft_LandingZone_Aircraft> dp88_Aircraft_LandingZone_Aircraft_Registrant("dp88_Aircraft_LandingZone_Aircraft","landing_anim_name=modelfile.animfile:string,landing_anim_first_frame=0.0:float,landing_anim_last_frame=25.0:float,require_landing_zone=1:int");


// Terror Drone
/*ScriptRegistrant<dp88_AR_TerrorDrone> dp88_AR_TerrorDrone_Registrant("dp88_AR_TerrorDrone","");*/

// Remote Control Scripts
ScriptRegistrant<dp88_RemoteControlConsole> dp88_RemoteControlConsole_Registrant("dp88_RemoteControlConsole","remoteControlID:int,vehiclePreset:string,cost:int,team:int,chargeTime=0:int,accessDeniedSound:string,consoleOfflineSound:string,consoleChargingSound:string,insufficientFundsSound:string,connectionEstablishedSound:string,animationName:string,animFrameAvailable1=0:int,animFrameAvailable2=0:int,animFrameCharging1=0:int,animFrameCharging2=0:int,animFrameActive1=0:int,animFrameActive2=0:int,animFrameIdle1=0:int,animFrameIdle2=0:int,animFrameDisabled1=0:int,animFrameDisabled2=0:int");
ScriptRegistrant<dp88_RemoteControlVehicle> dp88_RemoteControlVehicle_Registrant("dp88_RemoteControlVehicle","remoteControlID:int");


// Demo Truck
ScriptRegistrant<dp88_AR_DemoTruck> dp88_AR_DemoTruck_Registrant("dp88_AR_DemoTruck","explosionPreset=Explosion_NukeBeacon:string,cinematicPreset=null:string,buildingDamageRange=50.0:float,buildingDamageWarhead=Nuke:string,buildingDamageStrength=2200.00:float");


// Paradrop scripts
ScriptRegistrant<dp88_AR_paradrop_Console> dp88_AR_paradrop_Console_Registrant("dp88_AR_paradrop_Console","team=1:int");
ScriptRegistrant<dp88_AR_paradrop> dp88_AR_paradrop_Registrant("dp88_AR_paradrop","infantry_preset:string");

// Prism Tower script
ScriptRegistrant<dp88_AR_Prism_Tower> dp88_AR_Prism_Tower_Registrant("dp88_AR_Prism_Tower","Priority_Infantry=1.0:float,Splash_Infantry=0:int,Priority_Light_Vehicle=5.0:float,Priority_Heavy_Vehicle=7.0:float,Priority_VTOL=0.0:float,Min_Attack_Range=0:int,Max_Attack_Range=150:int,Animation_Model:string,Animation_Model_Bone:string,Animation:string,Animation_Idle_Start_Frame:int,Animation_Idle_End_Frame:int,Animation_Unpowered_Start_Frame:int,Animation_Unpowered_End_Frame:int,Animation_Charge_Start_Frame:int,Animation_Charge_End_Frame:int,Charge_Sound:string,Modifier_Distance=0.25:float,Modifier_Target_Damage=0.1:float,Modifier_Target_Value=0.25:float,Requires_Power=1:int,Debug=0:int");

// Health link script
ScriptRegistrant<dp88_linkHealth> dp88_linkHealth_Registrant("dp88_linkHealth","parentObjectId=0:int");
