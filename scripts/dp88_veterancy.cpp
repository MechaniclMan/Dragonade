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
#include "dp88_veterancy.h"
#include "dp88_custom_timer_defines.h"
#include "VehicleGameObjDef.h"		// For VEHICLE_TYPE_TURRET
#include "ScriptableGameObj.h"

/******************************
Initialise static pointer lists to null
******************************/

dp88_veterancyUnit* dp88_veterancyUnit::playerData[128] = { NULL };
dp88_veterancyUnit* dp88_veterancyUnit::AIUnitData[256] = { NULL };



/******************************
Event handler functions
******************************/

/* Created event :: Set all data variables to default values and
register with one of the static pointer arrays */
void dp88_veterancyUnit::Created ( GameObject* obj )
{
	objectId = Commands->Get_ID ( obj );

	// Set all values to 0 (default)
	currentLevel = 0;
	infantryVeterancyPoints = 0;
	vehicleVeterancyPoints = 0;
	infantryVeteranRequirement = 0;
	infantryEliteRequirement = 0;
	vehicleVeteranRequirement = 0;
	vehicleEliteRequirement = 0;
	chevronObjId = NULL;
	promotionChevronObjId = NULL;

	rookieWeapon[0] = '\0';
	rookieSkinType[0] = '\0';
	rookieShieldType[0] = '\0';

	hasVeteranWeaponPowerup = (strcmp ( Get_Parameter( "veteran_weaponPowerup" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter( "veteran_weaponPowerup" ) )) ? true : false; 
	hasEliteWeaponPowerup = (strcmp ( Get_Parameter( "elite_weaponPowerup" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter( "elite_weaponPowerup" ) )) ? true : false; 

	pilotId = NULL;
	dead = false;



	// If unit is a soldier...
	if ( obj->As_SoldierGameObj() )
	{
		infantryVeteranRequirement = Get_Int_Parameter ( "veteran_pointsRequired" );
		infantryEliteRequirement = Get_Int_Parameter ( "elite_pointsRequired" );

		// If unit is a player...
		if ( Get_Player_ID ( obj ) >= 0 )
		{
			int playerId = Get_Player_ID ( obj );

			/* Keep 60% of the existing points (will be 0 unless player swapped character) */
			if ( playerData[playerId] != NULL )
			{
				infantryVeterancyPoints = playerData[playerId]->infantryVeterancyPoints * 0.60f;
				vehicleVeterancyPoints = playerData[playerId]->vehicleVeterancyPoints * 0.60f;

				/* Clear old chevrons */
				playerData[playerId]->clearChevrons();

				/* Trigger infantry promotions if applicable */
				if ( infantryVeterancyPoints >= infantryEliteRequirement && infantryEliteRequirement > 0 )
					promoteToElite();
				else if ( infantryVeterancyPoints >= infantryVeteranRequirement && infantryVeteranRequirement > 0 )
					promoteToVeteran();
			}

			/* Set player data array for this player ID to point to this object */
			playerData[playerId] = this;


			// Install a keyhook for getting your current veterancy data
			InstallHook( "ShowVeterancyPoints", obj );
		}


		// Else (if unit is a bot)
		else
		{
			/* Register AI Unit Data in first available position */
			for ( int i = 0; i < 256; i++ )
			{
				if ( AIUnitData[i] == NULL )
				{
					AIUnitData[i] = this;
					break;
				}
			}
		}
	}




	// Else if unit is a vehicle
	else if ( obj->As_VehicleGameObj() )
	{
		vehicleVeteranRequirement = Get_Int_Parameter ( "veteran_pointsRequired" );
		vehicleEliteRequirement = Get_Int_Parameter ( "elite_pointsRequired" );

		/* Register AI Unit Data in first available position (even if we are not
		actually an AI unit, it does not really matter)*/
		for ( int i = 0; i < 256; i++ )
		{
			if ( AIUnitData[i] == NULL )
			{
				AIUnitData[i] = this;
				break;
			}
		}
	}



	// Store default weapon / skin / armour types
	strcpy_s ( rookieSkinType, sizeof( rookieSkinType ), Get_Skin ( obj ) );
	strcpy_s ( rookieShieldType, sizeof( rookieShieldType ), Get_Shield_Type ( obj ) );

	if ( Get_Current_Weapon ( obj ) )
		strcpy_s ( rookieWeapon, sizeof( rookieWeapon ), Get_Current_Weapon ( obj ) );
	else
		strcpy_s ( rookieWeapon, sizeof( rookieWeapon ), "none" );
}


/* Damaged event :: Grant veterancy points to the unit that damaged
us if they are registered as a veterancy unit */
void dp88_veterancyUnit::Damaged( GameObject *obj, GameObject *damager, float amount )
{
	// Bail out if damager == null or damager == obj
	if ( damager == NULL || Commands->Get_ID ( damager ) == Commands->Get_ID ( obj ) )
		return;

	// Calculate our veterancy points value ( add 10% extra per promotion level )
	float pointsValue = (float)Get_Int_Parameter( "pointsValue" ) * (1.0f+(0.1f*currentLevel));

	// Work out our total hit points
	float totalHitPoints = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength ( obj );

	/* If damage was >0 then grant normal points, unless damager is on our team in which case we
	give nothing

	Points given: (pointsValue / total hit points) * damage */
	if ( amount > 0  && Get_Object_Type(damager) != Get_Object_Type(obj) )
		grantVeterancyPoints ( damager, (pointsValue/totalHitPoints)*amount );

	/* Otherwise if damage is < 0 then it is repairs, grant veterancy points if the repairer is on
	the same team (and we are not an empty vehicle).

	Points given: 50% of ((pointsValue / total hit points) * damage repaired) */
	else if ( amount < 0 && Get_Object_Type(damager) == Get_Object_Type(obj)
		&& ( !obj->As_VehicleGameObj() || Get_Vehicle_Driver ( obj ) != NULL || Get_Vehicle_Mode(obj) == VEHICLE_TYPE_TURRET ) )
	{
		grantVeterancyPoints ( damager, ((pointsValue/totalHitPoints)*(amount*-1)) *0.50f );
	}
}


/* Killed event :: Grant veterancy points to the unit that killed
us if they are registered as a veterancy unit and deregister from
static pointer arrays */
void dp88_veterancyUnit::Killed( GameObject *obj, GameObject *killer )
{
	dead = true;

	/* Clear chevrons */
	if ( chevronObjId != NULL && Commands->Find_Object ( chevronObjId ) != NULL )
		Commands->Destroy_Object ( Commands->Find_Object ( chevronObjId ) );
	if ( promotionChevronObjId != NULL && Commands->Find_Object ( promotionChevronObjId ) != NULL )
		Commands->Destroy_Object ( Commands->Find_Object ( promotionChevronObjId ) );

	/* Remove from static pointer arrays */
	if ( Get_Player_ID ( obj ) >= 0 )
		playerData[Get_Player_ID ( obj )] = NULL;
	else
	{
		/* Find this unit and remove it */
		for ( int i = 0; i < 256; i++ )
		{
			if ( AIUnitData[i] == this )
			{
				AIUnitData[i] = NULL;
				break;
			}
		}
	}

	
	// Remove keyhook
		RemoveHook();
}


/* Destroyed event :: Clean up any remaining chevrons and remove from
static pointer arrays if necessary. */
void dp88_veterancyUnit::Destroyed( GameObject *obj )
{
	/* Clear chevrons */
	clearChevrons ();

	/* Remove from static pointer arrays */
	if ( Get_Player_ID ( obj ) >= 0 )
		playerData[Get_Player_ID ( obj )] = NULL;
	else
	{
		/* Find this unit and remove it */
		for ( int i = 0; i < 256; i++ )
		{
			if ( AIUnitData[i] == this )
			{
				AIUnitData[i] = NULL;
				break;
			}
		}
	}

	
	// Remove keyhook
		RemoveHook();
}


/* Custom event :: Process custom messages */
void dp88_veterancyUnit::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle pilot entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED && pilotId == NULL )
	{
		pilotId = Commands->Get_ID ( sender );

		// Get veterancy data for the unit that entered
		dp88_veterancyUnit* vetUnit = getVeterancyData ( sender );
		if ( vetUnit )
		{
			// Reset veterancy to rookie
			demoteToRookie();

			// Apply promotions based upon driver veterancy points
			if ( vetUnit->vehicleVeterancyPoints >= vehicleEliteRequirement && vehicleEliteRequirement > 0 )
				promoteToElite();
			else if ( vetUnit->vehicleVeterancyPoints >= vehicleVeteranRequirement && vehicleVeteranRequirement > 0 )
				promoteToVeteran();

			// Set our drivers vehicle veterancy boundaries
			vetUnit->vehicleVeteranRequirement = vehicleVeteranRequirement;
			vetUnit->vehicleEliteRequirement = vehicleEliteRequirement;
		}
	}




	// Look for vehicle pilot exit
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED && pilotId != NULL && pilotId == Commands->Get_ID ( sender ) )
	{
		pilotId = NULL;

		// Reset veterancy to rookie
		demoteToRookie();

		// Apply promotions based upon our own veterancy points
		if ( vehicleVeterancyPoints >= vehicleEliteRequirement && vehicleEliteRequirement > 0 )
			promoteToElite();
		else if ( vehicleVeterancyPoints >= vehicleVeteranRequirement && vehicleVeteranRequirement > 0 )
			promoteToVeteran();

		// Get veterancy data for the unit that exited
		dp88_veterancyUnit* vetUnit = getVeterancyData ( sender );
		if ( vetUnit )
		{
			// Reset our old drivers vehicle veterancy boundaries
			vetUnit->vehicleVeteranRequirement = 0;
			vetUnit->vehicleEliteRequirement = 0;
		}
	}




	/* Look for internal message to remove old weapon following a promotion
	param = old veterancy level */
	else if ( type == CUSTOM_VETERANCY_REMOVE_OLD_WEAPON )
	{
		/* For whatever reason removing a non-selected weapon changes the
		selected weapon, so store the selected weapon here */
		char currentWeapon[128];
		strcpy_s ( currentWeapon, sizeof(currentWeapon), Get_Current_Weapon(obj) );
		
		if ( param == 1 && hasVeteranWeaponPowerup )
			Remove_Weapon ( obj, Get_Powerup_Weapon ( Get_Parameter ( "veteran_weaponPowerup" ) ) );
		else
			Remove_Weapon ( obj, rookieWeapon );

		/* Reselect current weapon (also triggers the back model of the old
		weapon to be removed so it doesn't crash on the next promotion) */
		Commands->Select_Weapon ( obj, currentWeapon );
	}
}


void dp88_veterancyUnit::Timer_Expired( GameObject *obj, int number )
{
	// Look for timer for clearing promotion chevrons
	if ( number == TIMER_VETERANCY_EXPIRE_PROMOTION_CHEVRON )
	{
		if ( promotionChevronObjId != NULL && Commands->Find_Object ( promotionChevronObjId ) != NULL )
		{
			Commands->Destroy_Object ( Commands->Find_Object ( promotionChevronObjId ) );
			promotionChevronObjId = NULL;
		}
	}
}


void dp88_veterancyUnit::KeyHook()
{
	// Get GameObject
	GameObject* obj = Commands->Find_Object ( objectId );
	if ( obj == NULL || dead )
		return;

	// Catch non player units somehow using a keyhook (LAN mode fucking up maybe?)
	if ( Get_Player_ID( obj ) < 0 )
	{
		Console_Output ( "Error: Unit with no player ID used veterancy data keyhook!" );
		return;
	}


	// Send infantry page
	StringClass message;
	const char *str = Get_Translated_Preset_Name (obj);
	message.Format ( "You currently have %.2f infantry veterancy points. A %s requires %d points for Veteran status and %d points for Elite status.", infantryVeterancyPoints, str, infantryVeteranRequirement, infantryEliteRequirement );
	delete[] str;
	Send_Message_Player( obj,50,255,50,message );


	// Send vehicle page
	if ( Get_Vehicle ( obj ) != NULL )
	{
		const char *str2 = Get_Translated_Preset_Name (Get_Vehicle(obj));
		message.Format ( "You currently have %.2f vehicle veterancy points. A %s requires %d points for Veteran status and %d points for Elite status.", vehicleVeterancyPoints, str2, vehicleVeteranRequirement, vehicleEliteRequirement );
		delete[] str2;
	}
	else
		message.Format ( "You currently have %.2f vehicle veterancy points.", vehicleVeterancyPoints );
	Send_Message_Player( obj,50,255,50,message );
}




/******************************
Chevron functions
******************************/

/* Create chevrons */
void dp88_veterancyUnit::createChevrons()
{
	// Clear any existing chevrons
	clearChevrons ();

	// Get GameObject
	GameObject* obj = Commands->Find_Object ( objectId );
	if ( obj == NULL || dead )
		return;

	if ( obj->As_SoldierGameObj() )
	{
		// Generate name string for the unit promoted chevron
		char promotionChevronPreset[27];
		if ( currentLevel == 2 )			sprintf ( promotionChevronPreset, "chev_inf_elite_promotion" );
		else								sprintf ( promotionChevronPreset, "chev_inf_veteran_promotion" );

		// Create promoted chevron and record it's ID
		GameObject* promotionChevron = Commands->Create_Object_At_Bone( obj, promotionChevronPreset, "Worldbox" );
		Commands->Set_Facing( promotionChevron, Commands->Get_Facing( obj ) );
		promotionChevronObjId = Commands->Get_ID( promotionChevron );
		
		// Start timer to destroy the promotion chevron
		Commands->Start_Timer( obj, this, 2.5f, TIMER_VETERANCY_EXPIRE_PROMOTION_CHEVRON );




		// Generate name string for the unit chevron
		char chevronPreset[17];
		if ( currentLevel == 2 )			sprintf ( chevronPreset, "chev_inf_elite" );
		else								sprintf ( chevronPreset, "chev_inf_veteran" );
	
		// Now create new chevron and record it's ID
		GameObject* chevron = Commands->Create_Object_At_Bone( obj, chevronPreset, "Worldbox" );
		Commands->Set_Facing( chevron, Commands->Get_Facing( obj ) );
		Commands->Attach_To_Object_Bone( chevron, obj, "Worldbox" );
		chevronObjId = Commands->Get_ID( chevron );
	}



	else if ( obj->As_VehicleGameObj() )
	{
		// Generate name string for the unit promoted chevron
		char promotionChevronPreset[27];
		if ( currentLevel == 2 )			sprintf ( promotionChevronPreset, "chev_veh_elite_promotion" );
		else								sprintf ( promotionChevronPreset, "chev_veh_veteran_promotion" );

		// Create promoted chevron and record it's ID
		GameObject* promotionChevron = Commands->Create_Object_At_Bone( obj, promotionChevronPreset, "chevBone" );
		Commands->Set_Facing( promotionChevron, Commands->Get_Facing( obj ) );
		promotionChevronObjId = Commands->Get_ID( promotionChevron );
		
		// Start timer to destroy the promotion chevron
		Commands->Start_Timer( obj, this, 2.5f, TIMER_VETERANCY_EXPIRE_PROMOTION_CHEVRON );




		// Generate name string for the unit chevron
		char chevronPreset[17];
		if ( currentLevel == 2 )			sprintf ( chevronPreset, "chev_veh_elite" );
		else								sprintf ( chevronPreset, "chev_veh_veteran" );
	
		// Now create new chevron and record it's ID
		GameObject* chevron = Commands->Create_Object_At_Bone( obj, chevronPreset, "chevBone" );
		Commands->Set_Facing( chevron, Commands->Get_Facing( obj ) );
		Commands->Attach_To_Object_Bone( chevron, obj, "chevBone" );
		chevronObjId = Commands->Get_ID( chevron );
	}
}


/* Clear chevrons */
void dp88_veterancyUnit::clearChevrons()
{
	if ( chevronObjId != NULL && Commands->Find_Object ( chevronObjId ) != NULL )
	{
		Commands->Destroy_Object ( Commands->Find_Object ( chevronObjId ) );
		chevronObjId = NULL;
	}

	if ( promotionChevronObjId != NULL && Commands->Find_Object ( promotionChevronObjId ) != NULL )
	{
		Commands->Destroy_Object ( Commands->Find_Object ( promotionChevronObjId ) );
		promotionChevronObjId = NULL;
	}
}




/******************************
Veterancy point functions
******************************/

/* Find veterancy unit object */
dp88_veterancyUnit* dp88_veterancyUnit::getVeterancyData( GameObject *obj )
{
	if (!obj)
	{
		return NULL;
	}
	if ( obj->As_SoldierGameObj() && Get_Player_ID ( obj ) != -1 )
		return playerData[Get_Player_ID ( obj )];
	else
	{
		/* Find this unit and return it */
		for ( int i = 0; i < 256; i++ )
		{
			if ( AIUnitData[i] != NULL && AIUnitData[i]->objectId == Commands->Get_ID ( obj ) )
				return AIUnitData[i];
		}
	}

	return NULL;
}


/* Grant veterancy points to specified unit */
void dp88_veterancyUnit::grantVeterancyPoints( GameObject *obj, float points )
{
	dp88_veterancyUnit* vetData = getVeterancyData ( obj );
	if ( vetData != NULL )
		vetData->recieveVeterancyPoints ( points );
}


/* Recieve veterancy points from another unit */
void dp88_veterancyUnit::recieveVeterancyPoints ( float points )
{
	// Get GameObject
	GameObject* obj = Commands->Find_Object ( objectId );
	if ( obj == NULL || dead )
		return;

	// If unit is a soldier and is not driving a vehicle give infantry veterancy points
	if ( obj->As_SoldierGameObj() && Get_Vehicle ( obj ) == NULL )
	{
		infantryVeterancyPoints += points;

		// Check for elite promotion
		if ( currentLevel < 2 && infantryVeterancyPoints >= infantryEliteRequirement && infantryEliteRequirement > 0 )
			promoteToElite();

		// Check for veteran promotion
		else if ( currentLevel < 1 && infantryVeterancyPoints >= infantryVeteranRequirement && infantryVeteranRequirement > 0 )
			promoteToVeteran();
	}


	// If unit is a vehicle or is driving a vehicle give vehicle veterancy points
	if ( obj->As_VehicleGameObj() || ( obj->As_SoldierGameObj() && Get_Vehicle ( obj ) != NULL ) )
	{
		vehicleVeterancyPoints += points;

		// Check for elite promotion
		if ( vehicleVeterancyPoints >= vehicleEliteRequirement && vehicleEliteRequirement > 0 )
		{
			// If unit is a vehicle promote self to elite
			if ( currentLevel < 2 && obj->As_VehicleGameObj() )
				promoteToElite();

			// Otherwise promote the vehicle this unit is driving to elite
			else
			{
				dp88_veterancyUnit* vetUnit = getVeterancyData ( Get_Vehicle ( obj ) );
				if ( vetUnit && vetUnit->currentLevel < 2 )
					vetUnit->promoteToElite();
			}
		}

		// Check for veteran promotion
		else if ( vehicleVeterancyPoints >= vehicleVeteranRequirement && vehicleVeteranRequirement > 0 )
		{
			// If unit is a vehicle promote self to veteran
			if ( currentLevel < 1 && obj->As_VehicleGameObj() )
				promoteToVeteran();

			// Otherwise promote the vehicle this unit is driving to veteran
			else
			{
				dp88_veterancyUnit* vetUnit = getVeterancyData ( Get_Vehicle ( obj ) );
				if ( vetUnit && vetUnit->currentLevel < 1 )
					vetUnit->promoteToVeteran();
			}
		}
	}
}

/* Promotion to veteran function */
void dp88_veterancyUnit::promoteToVeteran()
{
	// Get GameObject
	GameObject* obj = Commands->Find_Object ( objectId );
	if ( obj == NULL || dead )
		return;

	// Set veterancy level
	currentLevel = 1;

	// Create chevrons
	createChevrons ();

	// Send page and promotion sound if unit is a player
	if ( obj->As_SoldierGameObj() && Get_Player_ID ( obj ) >= 0 )
	{
		// Send page
		char consoleInput[128];
		const char* playerName = Get_Player_Name( obj );
		sprintf ( consoleInput, "ppage %d Congratulations %s, you have been promoted to veteran.", Get_Player_ID( obj ), playerName );
		delete [] playerName;
		Console_Input( consoleInput );

		// Play promotion sound
		sprintf ( consoleInput, "sndp %d promotion.wav", Get_Player_ID ( obj ) );
		Console_Input( consoleInput );
	}


	// Send page and promotion sound if unit driver is a player
	else if ( obj->As_VehicleGameObj() && Get_Vehicle_Driver ( obj ) != NULL && Get_Player_ID ( Get_Vehicle_Driver ( obj ) ) >= 0 )
	{
		// Send page
		char consoleInput[128];
		const char* playerName = Get_Player_Name( Get_Vehicle_Driver ( obj ) );
		sprintf ( consoleInput, "ppage %d Congratulations %s, your vehicle has been promoted to veteran.", Get_Player_ID( Get_Vehicle_Driver ( obj ) ), playerName );
		delete [] playerName;
		Console_Input( consoleInput );

		// Play promotion sound
		sprintf ( consoleInput, "sndp %d promotion.wav", Get_Player_ID ( Get_Vehicle_Driver ( obj ) ) );
		Console_Input( consoleInput );
	}




	/* Apply new weapon if applicable */
	if ( hasVeteranWeaponPowerup )
	{
		/* Decide if we need to select this weapon once it is granted (if we
		are using the weapon it replaces then we select it */
		bool selectWeapon = ( strcmp ( Get_Current_Weapon ( obj ), rookieWeapon ) == 0 ) ? true : false;

		/* Grant new weapon */
		Commands->Give_PowerUp( obj, Get_Parameter( "veteran_weaponPowerup" ), true );
		if ( selectWeapon )
			Commands->Select_Weapon ( obj, Get_Powerup_Weapon ( Get_Parameter( "veteran_weaponPowerup" ) ) );

		/* If unit is an infantry unit remove old weapon - immediatly if not
		currently selected or after a 0.1 second delay otherwise */
		if ( obj->As_SoldierGameObj() )
		{
			if ( selectWeapon )
			{
				// Send custom after 0.1 seconds to clean up old weapon
				Commands->Send_Custom_Event ( obj, obj, CUSTOM_VETERANCY_REMOVE_OLD_WEAPON, 0, 0.1f );
			}

			// Remove immediatly
			else
				Remove_Weapon ( obj, rookieWeapon );
		}
	}


	/* Set new skin/armour type if applicable */
	if ( strcmp ( Get_Parameter( "veteran_shieldType" ), "null" ) != 0 )
		Commands->Set_Shield_Type ( obj, Get_Parameter( "veteran_shieldType" ) );

	if ( strcmp ( Get_Parameter( "veteran_skinType" ), "null" ) != 0 )
		Set_Skin( obj, Get_Parameter( "veteran_skinType" ) );



	/* Send custom event for other scripts to watch for (parameter 1 = veteran). Slight
	delay to ensure all scripts are created before it is sent (for when we are insta-
	promoted after buying a new character) */
	Commands->Send_Custom_Event ( obj, obj, CUSTOM_VETERANCY_PROMOTED, 1, 0.1f );
}


/* Promotion to elite function */
void dp88_veterancyUnit::promoteToElite()
{
	// Get GameObject
	GameObject* obj = Commands->Find_Object ( objectId );
	if ( obj == NULL || dead )
		return;

	// Set veterancy level
	int oldLevel = currentLevel;
	currentLevel = 2;

	// Create chevrons
	createChevrons();

	// Send page and promotion sound if unit is a player
	if ( obj->As_SoldierGameObj() && Get_Player_ID ( obj ) >= 0 )
	{
		// Send page
		char consoleInput[128];
		const char* playerName = Get_Player_Name( obj );
		sprintf ( consoleInput, "ppage %d Congratulations %s, you have been promoted to elite.", Get_Player_ID( obj ), playerName );
		delete [] playerName;
		Console_Input( consoleInput );

		// Play promotion sound
		sprintf ( consoleInput, "sndp %d promotion.wav", Get_Player_ID ( obj ) );
		Console_Input( consoleInput );
	}


	// Send page and promotion sound if unit driver is a player
	else if ( obj->As_VehicleGameObj() && Get_Vehicle_Driver ( obj ) != NULL && Get_Player_ID ( Get_Vehicle_Driver ( obj ) ) >= 0 )
	{
		// Send page
		char consoleInput[128];
		const char* playerName = Get_Player_Name( Get_Vehicle_Driver ( obj ) );
		sprintf ( consoleInput, "ppage %d Congratulations %s, your vehicle has been promoted to elite.", Get_Player_ID( Get_Vehicle_Driver ( obj ) ), playerName );
		delete [] playerName;
		Console_Input( consoleInput );

		// Play promotion sound
		sprintf ( consoleInput, "sndp %d promotion.wav", Get_Player_ID ( Get_Vehicle_Driver ( obj ) ) );
		Console_Input( consoleInput );
	}




	/* Work out which new weapon to give them (if there is no elite version then
	give them the veteran version if they were promoted straight from rookie) */
	char weaponPowerup[64] = "null";
	if ( hasEliteWeaponPowerup )
		strcpy_s ( weaponPowerup, sizeof(weaponPowerup), Get_Parameter ( "elite_weaponPowerup" ) );
	else if ( oldLevel == 0 && hasVeteranWeaponPowerup )
		strcpy_s ( weaponPowerup, sizeof(weaponPowerup), Get_Parameter ( "veteran_weaponPowerup" ) );


	/* Apply new weapon if applicable */
	if ( strcmp ( weaponPowerup, "null" ) != 0 && Is_Valid_Preset ( weaponPowerup ) )
	{
		bool selectWeapon = false;

		/* Decide if we need to select this weapon once it is granted (if we
		are using the weapon it replaces then we select it) */
		if ( Get_Current_Weapon(obj) == NULL || 
			( oldLevel == 0 && strcmp ( Get_Current_Weapon ( obj ), rookieWeapon ) == 0 )
			|| ( oldLevel == 1 &&
				( hasVeteranWeaponPowerup && strcmp ( Get_Current_Weapon ( obj ), Get_Powerup_Weapon ( Get_Parameter ( "veteran_weaponPowerup" ) ) ) == 0 )
				|| ( !hasVeteranWeaponPowerup && strcmp ( Get_Current_Weapon ( obj ), rookieWeapon ) == 0 ) ) )
		{
			selectWeapon = true;
		}


		/* Grant new weapon */
		Commands->Give_PowerUp( obj, weaponPowerup, true );
		if ( selectWeapon )
			Commands->Select_Weapon ( obj, Get_Powerup_Weapon ( weaponPowerup ) );

		/* If unit is an infantry unit remove old weapon - immediatly if not
		currently selected or after a 0.1 second delay otherwise */
		if ( obj->As_SoldierGameObj() )
		{
			if ( selectWeapon )
			{
				// Send custom after 0.1 seconds to clean up old weapon
				Commands->Send_Custom_Event ( obj, obj, CUSTOM_VETERANCY_REMOVE_OLD_WEAPON, oldLevel, 0.1f );
			}

			// Remove immediatly
			else
				Remove_Weapon ( obj, (hasVeteranWeaponPowerup) ? Get_Powerup_Weapon ( Get_Parameter ( "veteran_weaponPowerup" ) ) : rookieWeapon );
		}
	}


	/* Work out which new armour to give them (if there is no elite version then
	give them the veteran version if they were promoted straight from rookie) */
	char shieldType[64] = "null";
	if ( strcmp ( Get_Parameter( "elite_shieldType" ), "null" ) != 0 )
		strcpy_s ( shieldType, sizeof(shieldType), Get_Parameter ( "elite_shieldType" ) );
	else if ( oldLevel == 0 && strcmp ( Get_Parameter( "veteran_shieldType" ), "null" ) != 0 )
		strcpy_s ( shieldType, sizeof(shieldType), Get_Parameter ( "veteran_shieldType" ) );

	char skinType[64] = "null";
	if ( strcmp ( Get_Parameter( "elite_skinType" ), "null" ) != 0 )
		strcpy_s ( skinType, sizeof(skinType), Get_Parameter ( "elite_skinType" ) );
	else if ( oldLevel == 0 && strcmp ( Get_Parameter( "veteran_skinType" ), "null" ) != 0 )
		strcpy_s ( skinType, sizeof(skinType), Get_Parameter ( "veteran_skinType" ) );


	/* Set new skin/armour type if applicable */
	if ( strcmp ( shieldType, "null" ) != 0 )
		Commands->Set_Shield_Type ( obj, shieldType );

	if ( strcmp ( skinType, "null" ) != 0 )
		Set_Skin( obj, skinType );



	/* Send custom event for other scripts to watch for (parameter 2 = elite). Slight
	delay to ensure all scripts are created before it is sent (for when we are insta-
	promoted after buying a new character) */
	Commands->Send_Custom_Event ( obj, obj, CUSTOM_VETERANCY_PROMOTED, 2, 0.1f );
}


/* Demote unit to rookie - primaraily used for vehicles, will not work
fully on infantry as their original weapon is gone */
void dp88_veterancyUnit::demoteToRookie()
{
	// Get GameObject
	GameObject* obj = Commands->Find_Object ( objectId );
	if ( obj == NULL || dead )
		return;

	// Set rookie level
	currentLevel = 0;

	// Reset skin & armour types
	Set_Skin ( obj, rookieSkinType );
	Commands->Set_Shield_Type ( obj, rookieShieldType );

	/* Select default weapon (won't work on infantry as the weapon
	was taken off them */
	Commands->Select_Weapon ( obj, rookieWeapon );

	/* Get rid of chevrons */
	clearChevrons();



	/* Send custom event for other scripts to watch for (parameter 0 = rookie) */
	Commands->Send_Custom_Event ( obj, obj, CUSTOM_VETERANCY_PROMOTED, 0, 0 );
}




// Script Registrants
ScriptRegistrant<dp88_veterancyUnit> dp88_veterancyUnit_Registrant (
	"dp88_veterancyUnit",
	"pointsValue=0:int,"
	"veteran_pointsRequired=0:int,"
	"veteran_skinType=null:string,"
	"veteran_shieldType=null:string,"
	"veteran_weaponPowerup=null:string,"
	"elite_pointsRequired=0:int,"
	"elite_skinType=null:string,"
	"elite_shieldType=null:string,"
	"elite_weaponPowerup=null:string"
);








/* Script for a crate which grants veterancy points to the collector */
void dp88_veterancyCrate::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
	if ( type == CUSTOM_EVENT_POWERUP_GRANTED )
	{
		int points = Commands->Get_Random_Int ( Get_Int_Parameter( "minPoints" ), Get_Int_Parameter( "maxPoints" ) );

		// The sender of the custom is the unit which picked up the crate, if they have the
		// veterancy script running then grant them some points
		dp88_veterancyUnit *veterancy_script = (dp88_veterancyUnit*)Find_Script_On_Object(sender, "dp88_veterancyUnit");
		if ( veterancy_script )
		{
			veterancy_script->recieveVeterancyPoints((float)points);

			if ( Get_Player_ID(sender) > 0 )
			{
				StringClass message;
				message.Format("You have been granted %d veterancy points!",points);
				Send_Message_Player(sender,153,204,25,message);
			}
		}
	}
}

ScriptRegistrant<dp88_veterancyCrate> dp88_veterancyCrate_Registrant (
	"dp88_veterancyCrate",
	"minPoints=1:int,"
	"maxPoints=20:int"
);








/* Script to grant veterancy points to whatever it is attached to */
void dp88_veterancyGrantPoints::Created ( GameObject *obj )
{
	// Wait for 1 second before taking action to prevent potential issues with this script being
	// loaded before the dp88_veterancyUnit script is loaded
	Commands->Send_Custom_Event ( obj, obj, CUSTOM_VETERANCY_GRANT_POINTS_DELAY, 0, 1.0f );
}

void dp88_veterancyGrantPoints::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
	if ( type == CUSTOM_VETERANCY_GRANT_POINTS_DELAY )
	{
		int points = Commands->Get_Random_Int ( Get_Int_Parameter( "minPoints" ), Get_Int_Parameter( "maxPoints" ) );

		// Give the unit we are attached to some veterancy points
		dp88_veterancyUnit *veterancy_script = (dp88_veterancyUnit*)Find_Script_On_Object(obj, "dp88_veterancyUnit");
		if ( veterancy_script )
		{
			veterancy_script->recieveVeterancyPoints((float)points);

			if ( Get_Player_ID(obj) > 0 )
			{
				StringClass message;
				message.Format("You have been granted %d veterancy points!",points);
				Send_Message_Player(obj,153,204,25,message);
			}
		}

		Destroy_Script();
	}
}

ScriptRegistrant<dp88_veterancyGrantPoints> dp88_veterancyGrantPoints_Registrant (
	"dp88_veterancyGrantPoints",
	"minPoints=1:int,"
	"maxPoints=20:int"
);








/*------------------------
Veterancy Points Link Script

Maintains the same veterancy points between two objects, for example
the AI warminer and its gun turret
--------------------------*/

void dp88_linkVetPoints::Created ( GameObject* obj )
{
	parentObjID = Get_Int_Parameter ( "parentObjectId" );
	GameObject* parent = Commands->Find_Object(parentObjID);
	if ( !parent )
	{
		Destroy_Script();
		return;
	}

  // Get veterancy script on parent and ourselves
  dp88_veterancyUnit *parentScript = (dp88_veterancyUnit*)Find_Script_On_Object(parent, "dp88_veterancyUnit");
  dp88_veterancyUnit *ourScript = (dp88_veterancyUnit*)Find_Script_On_Object(obj, "dp88_veterancyUnit");
	if ( !parentScript || !ourScript )
	{
		Destroy_Script();
		return;
	}

	// Set initial values for last points values
	//lastInfantryPoints = parentScript->
	//lastVehiclePoints = Commands->Get_Shield_Strength ( parent );

	// Set our max / current health / armour to match our parent
	//Set_Max_Health ( obj, Commands->Get_Max_Health(parent) );
	//Commands->Set_Health ( obj, lastHealth );
	//Set_Max_Shield_Strength ( obj, Commands->Get_Max_Shield_Strength(parent) );
	//Commands->Set_Shield_Strength ( obj, lastArmour );

	// Start timer
	Commands->Start_Timer ( obj, this, 0.25, TIMER_LINKHEALTH );
}

void dp88_linkVetPoints::Timer_Expired ( GameObject* obj, int number )
{
	equalisePoints( obj );

	// Restart timer if still alive
	if ( Commands->Get_Health(obj) > 0 )
		Commands->Start_Timer ( obj, this, 0.5, TIMER_LINKVETPOINTS );
}

void dp88_linkVetPoints::equalisePoints ( GameObject* obj )
{
	//GameObject* other = Commands->Find_Object ( otherObjID );

	// Compute difference in both objects (infantry points)
	//float diff_us = getInfantryVeterancyPoints(obj) - lastInfantryPoints;
	//float diff_other = getInfantryVeterancyPoints(other) - lastInfantryPoints;

	// Grant each object the points from the other object
	//if ( diff_other != 0.0f )
	//	grantInfantryVeterancyPoints ( obj, diff_other );
	//if ( diff_us != 0.0f )
	//	grantInfantryVeterancyPoints ( other, diff_us );

	//lastInfantryPoints += diff_us + diff_other;

	
	
	// Compute difference in both objects (vehicle points)
	//diff_us = getVehicleVeterancyPoints(obj) - lastVehiclePoints;
	//diff_other = getVehicleVeterancyPoints(other) - lastVehiclePoints;

	// Grant each object the points from the other object
	//if ( diff_other != 0.0f )
	//	grantVehicleVeterancyPoints ( obj, diff_other );
	//if ( diff_us != 0.0f )
	//	grantVehicleVeterancyPoints ( other, diff_us );

	//lastVehiclePoints += diff_us + diff_other;
}

ScriptRegistrant<dp88_linkVetPoints> dp88_linkVetPoints_Registrant(
	"dp88_linkVetPoints",
	"parentObjectId=0:int");








/*------------------------
Veterancy - Promotion health/armour boost
--------------------------*/

void dp88_veterancyPromotionHealthArmourIncrease::Created( GameObject *obj )
{
	// Set initial veterancy level
	m_veterancyLevel = 0;
}


void dp88_veterancyPromotionHealthArmourIncrease::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// If custom == CUSTOM_VETERANCY_PROMOTED update veterancyLevel
	if ( type == CUSTOM_VETERANCY_PROMOTED )
	{
		int oldVeterancy = m_veterancyLevel;
		m_veterancyLevel = param;

		int healthChange = 0;
		int armourChange = 0;

		// Handle promotions
		if ( oldVeterancy < m_veterancyLevel )
		{
			// Elite promotion
			if ( m_veterancyLevel == 2 )
			{
				healthChange += Get_Int_Parameter ( "eliteHealthIncrease" );
				armourChange += Get_Int_Parameter ( "eliteArmourIncrease" );
			}

			// Veteran promotion
			if ( oldVeterancy == 0 )	// Must be either veteran OR elite now, so apply veteran bonus
			{
				healthChange += Get_Int_Parameter ( "veteranHealthIncrease" );
				armourChange += Get_Int_Parameter ( "veteranArmourIncrease" );
			}
		}


		// And demotions (specifically vehicles)
		else
		{
			// Elite promotion
			if ( oldVeterancy == 2 )
			{
				healthChange -= Get_Int_Parameter ( "eliteHealthIncrease" );
				armourChange -= Get_Int_Parameter ( "eliteArmourIncrease" );
			}

			if ( m_veterancyLevel == 0 )	// Must have been either veteran OR elite, so remove veteran bonus
			{
				healthChange -= Get_Int_Parameter ( "veteranHealthIncrease" );
				armourChange -= Get_Int_Parameter ( "veteranArmourIncrease" );
			}
		}

		// Calculate new health/armour values - changing the max values might alter the
		// current values so we calculate this first to avoid problems with this
		int newHealth = (int)Commands->Get_Health(obj)+healthChange;
		int newArmour = (int)Commands->Get_Shield_Strength(obj)+armourChange;

		// Apply changes
		Set_Max_Health ( obj, Commands->Get_Max_Health(obj)+healthChange );
		Commands->Set_Health ( obj, (float)newHealth );
		Set_Max_Shield_Strength ( obj, Commands->Get_Max_Shield_Strength(obj)+armourChange );
		Commands->Set_Shield_Strength ( obj, (float)newArmour );
	}
}

ScriptRegistrant<dp88_veterancyPromotionHealthArmourIncrease> dp88_veterancyPromotionHealthArmourIncrease_Registrant(
	"dp88_veterancyPromotionHealthArmourIncrease",
	"veteranHealthIncrease=0:int,"
	"veteranArmourIncrease=0:int,"
	"eliteHealthIncrease=0:int,"
	"eliteArmourIncrease=0:int");