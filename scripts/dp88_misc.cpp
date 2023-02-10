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
#include "general.h"
#include "dp88_misc.h"
#include "dp88_custom_timer_defines.h"
#include "definition.h"
#include "VehicleGameObj.h"
#include "SoldierGameObj.h"
#include "SoldierGameObjDef.h"


/*------------------------
Random weather script
--------------------------*/

void dp88_randomWeather::Created ( GameObject* obj )
{
	// Start timer
	float updateInterval = (float)Commands->Get_Random_Int ( Get_Int_Parameter ( "update_interval_min" ), Get_Int_Parameter ( "update_interval_max" )+1 );
	Commands->Start_Timer ( obj, this, updateInterval, TIMER_RANDOMWEATHER );
}


void dp88_randomWeather::Timer_Expired ( GameObject* obj, int number )
{
	// Decide if we should change the weather at all
	if ( Commands->Get_Random_Int ( 0,101 ) <= Get_Int_Parameter ( "probability_change" ) )
	{
		// Time to change the weather!


		/********************
		Fog section (start / end / time)
		********************/
		float fog_start = (float)Commands->Get_Random_Int ( Get_Int_Parameter ( "fog_start_distance_min" ), Get_Int_Parameter ( "fog_start_distance_max" )+1 );
		float fog_end = (float)Commands->Get_Random_Int ( Get_Int_Parameter ( "fog_end_distance_min" ), Get_Int_Parameter ( "fog_end_distance_max" )+1 );
		
		// Check start <= end
		if ( fog_start > fog_end )
			fog_end = fog_start;

		Commands->Set_Fog_Range ( fog_start, fog_end, 2.0f );






		/********************
		Precipitation section (density / time / unknown)
		********************/
		int precipitation_type = Commands->Get_Random_Int ( 1, 101 );		// 1 to 100
		float precipitation_density = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "precipitation_density_min" ) *100.0f), (int)((Get_Float_Parameter ( "precipitation_density_max" ) *100.0f)+1.0f) ) / 100.0f );

		
		if ( precipitation_density > 0.0f && Get_Int_Parameter ( "precipitation_probability_snow" ) >= precipitation_type )
		{
			// Set snow
			Commands->Set_Snow(precipitation_density, 2.0f, true);
			Commands->Set_Rain(0.0f, 2.0f, true);
			Commands->Set_Ash(0.0f, 2.0f, true);
		}
		else if ( precipitation_density > 0.0f && Get_Int_Parameter ( "precipitation_probability_snow" ) + Get_Int_Parameter ( "precipitation_probability_rain" ) >= precipitation_type )
		{
			// Set rain
			Commands->Set_Snow(0.0f, 2.0f, true);
			Commands->Set_Rain(precipitation_density, 2.0f, true);
			Commands->Set_Ash(0.0f, 2.0f, true);
		}
		else if ( precipitation_density > 0.0f && Get_Int_Parameter ( "precipitation_probability_snow" ) + Get_Int_Parameter ( "precipitation_probability_rain" )+ Get_Int_Parameter ( "precipitation_probability_volcanic_ash" ) >= precipitation_type )
		{
			// Set volcanic ash
			Commands->Set_Snow(0.0f, 2.0f, true);
			Commands->Set_Rain(0.0f, 2.0f, true);
			Commands->Set_Ash(precipitation_density, 2.0f, true);
		}
		else
		{
			// Set none
			Commands->Set_Rain(0.0f, 2.0f, true);
			Commands->Set_Snow(0.0f, 2.0f, true);
			Commands->Set_Ash(0.0f, 2.0f, true);
		}






		/********************
		Lightning section (intensity / start / end / heading / distribution / time)
		********************/
		int lightning_type = Commands->Get_Random_Int ( 1, 101 );		// 1 to 100
		float lightning_intensity = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "lighting_intensity_min" ) *100.0f), (int)((Get_Float_Parameter ( "lighting_intensity_max" ) *100.0f)+1.0f) ) / 100.0f );
		float lightning_start = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "lighting_start_distance_min" ) *100.0f), (int)((Get_Float_Parameter ( "lighting_start_distance_max" ) *100.0f)+1.0f) ) / 100.0f );
		float lightning_end = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "lighting_end_distance_min" ) *100.0f), (int)((Get_Float_Parameter ( "lighting_end_distance_max" ) *100.0f)+1.0f) ) / 100.0f );
		float lightning_heading = (float)Commands->Get_Random_Int ( Get_Int_Parameter ( "lighting_heading_min" ), Get_Int_Parameter ( "lighting_heading_max" ) +1 );
		float lightning_distribution = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "lighting_distribution_min" ) *100.0f), (int)((Get_Float_Parameter ( "lighting_distribution_max" ) *100.0f)+1.0f) ) / 100.0f );

		// Check ranges of variables
		if ( lightning_intensity > 1.0f )			lightning_intensity = 1.0f;
		if ( lightning_start > 1.0f )				lightning_start = 1.0f;
		if ( lightning_end > 1.0f )					lightning_end = 1.0f;
		if ( lightning_distribution > 1.0f )		lightning_distribution = 1.0f;

		// Check start <= end
		if ( lightning_start > lightning_end )
			lightning_end = lightning_start;


		if ( Get_Int_Parameter ( "lighting_probability_normal" ) >= lightning_type )
		{
			// Set lightning normal
			Commands->Set_Lightning ( lightning_intensity, lightning_start, lightning_end, lightning_heading, lightning_distribution, 2.0f );
			Commands->Set_War_Blitz ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f );
		}

		else if ( Get_Int_Parameter ( "lighting_probability_normal" ) + Get_Int_Parameter ( "lighting_probability_war_blitz" ) >= lightning_type )
		{
			// Set lightning war blitz
			Commands->Set_Lightning ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f );
			Commands->Set_War_Blitz ( lightning_intensity, lightning_start, lightning_end, lightning_heading, lightning_distribution, 2.0f );
		}

		else
		{
			// Set none
			Commands->Set_Lightning ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f );
			Commands->Set_War_Blitz ( 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f );
		}
		





		/********************
		Wind section (heading / strength / variability / time)
		********************/
		float wind_heading = (float)Commands->Get_Random_Int ( Get_Int_Parameter ( "wind_heading_min" ), Get_Int_Parameter ( "wind_heading_max" ) +1 );
		float wind_strength = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "wind_speed_min" ) *100.0f), (int)((Get_Float_Parameter ( "wind_speed_max" ) *100.0f)+1.0f) ) / 100.0f );
		float wind_variability = (float)(Commands->Get_Random_Int ( (int)(Get_Float_Parameter ( "wind_variability_min" ) *100.0f), (int)((Get_Float_Parameter ( "wind_variability_max" ) *100.0f)+1.0f) ) / 100.0f );

		// Check ranges of variables
		if ( wind_variability > 1.0f )				wind_variability = 1.0f;
		
		Commands->Set_Wind ( wind_heading, wind_strength, wind_variability, 2.0f );
	}

	// Restart timer
	float updateInterval = (float)Commands->Get_Random_Int ( Get_Int_Parameter ( "update_interval_min" ), Get_Int_Parameter ( "update_interval_max" )+1 );
	Commands->Start_Timer ( obj, this, updateInterval, TIMER_RANDOMWEATHER );
}

ScriptRegistrant<dp88_randomWeather> dp88_randomWeather_Registrant(
	"dp88_randomWeather",
	"update_interval_min=20:int,"
	"update_interval_max=50:int,"
	"probability_change=20:int,"
	"fog_start_distance_min=20:int,"
	"fog_start_distance_max=40:int,"
	"fog_end_distance_min=80:int,"
	"fog_end_distance_max=100:int,"
	"precipitation_probability_snow=0:int,"
	"precipitation_probability_rain=70:int,"
	"precipitation_probability_volcanic_ash=0:int,"
	"precipitation_density_min=0.0:float,"
	"precipitation_density_max=20.0:float,"
	"lighting_probability_normal=20:int,"
	"lighting_probability_war_blitz=5:int,"
	"lighting_intensity_min=0.2:float,"
	"lighting_intensity_max=0.8:float,"
	"lighting_start_distance_min=0.2:float,"
	"lighting_start_distance_max=0.8:float,"
	"lighting_end_distance_min=0.4:float,"
	"lighting_end_distance_max=1.0:float,"
	"lighting_heading_min=0:int,"
	"lighting_heading_max=359:int,"
	"lighting_distribution_min=0.1:float,"
	"lighting_distribution_max=1.0:float,"
	"wind_heading_min=0:int,"
	"wind_heading_max=359:int,"
	"wind_speed_min=0.0:float,"
	"wind_speed_max=8.0:float,"
	"wind_variability_min=0.0:float,"
	"wind_variability_max=1.0:float"
);










/*------------------------
Indirect Targetting Scripts
--------------------------*/

void dp88_indirectTargetting::Created ( GameObject* obj )
{
	objectID = Commands->Get_ID ( obj );
	pilotID = NULL;
	targettingLevel = new int(0);

	Console_Output ( "TL1: %p\n", targettingLevel );

	Commands->Send_Custom_Event (obj,obj,CUSTOM_INSTALL_IT_DECREMENT,0,0.0f);
}


void dp88_indirectTargetting::Destroyed ( GameObject* obj )
{
	delete targettingLevel;
}


void dp88_indirectTargetting::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
	{
		if ( pilotID == NULL )
		{
			pilotID = Commands->Get_ID(sender);

			InstallHook( "TargettingLevelInc", sender );
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
	}


	// Look for install decrement script
	else if ( type == CUSTOM_INSTALL_IT_DECREMENT )
	{
		char* script_parameters = new char[strlen(Get_Parameter ( "targettingAnimation" ))+193];
		sprintf ( script_parameters, "%s,%d,%d,%d,%p", Get_Parameter ( "targettingAnimation" ), Get_Int_Parameter ( "targettingLevels"), Get_Int_Parameter ( "maxFrame" ), Get_Int_Parameter ( "maxFrame" ), &targettingLevel );
		Console_Output ( "script params: %s\n", script_parameters );
		Commands->Attach_Script ( obj, "dp88_indirectTargetting_Dec", script_parameters );
		delete [] script_parameters;
	}
}


void dp88_indirectTargetting::KeyHook()
{
	// Find object
	GameObject* obj = Commands->Find_Object ( objectID );
	if ( !obj )
		return;


	if ( (*targettingLevel)++ <= Get_Int_Parameter ( "targettingLevels" ) )
	{
		float framesPerLevel = ((float)Get_Int_Parameter ( "maxFrame" ) - (float)Get_Int_Parameter ( "minFrame" )) / (float)Get_Int_Parameter ( "targettingLevels" );
		float targetFrame = framesPerLevel * (float)(*targettingLevel);

		Console_Output ( "framesPerLevel: %.4f\n", framesPerLevel );
		Console_Output ( "targetFrame: %.4f\n", targetFrame );

		Commands->Set_Animation ( obj, Get_Parameter ( "targettingAnimation" ), false, NULL, Get_Animation_Frame ( obj ), targetFrame, false );
	}
	else
		(*targettingLevel) = Get_Int_Parameter ( "targettingLevels" );
}





void dp88_indirectTargetting_Dec::Created ( GameObject* obj )
{
	objectID = Commands->Get_ID ( obj );
	pilotID = NULL;
	targettingLevel = (int*)Get_Int_Parameter ( "TL_ADDR" );

	Console_Output ( "TL2: %p\n", targettingLevel );

	// Catch errors
	if ( targettingLevel == NULL )
	{
		Console_Output ( "ERROR: dp88_indirectTargetting_Dec should never be used on it's own, use dp88_indirectTargetting instead.\n" );
		Destroy_Script();
	}
}


void dp88_indirectTargetting_Dec::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED )
	{
		if ( pilotID == NULL )
		{
			pilotID = Commands->Get_ID(sender);

			InstallHook( "TargettingLevelDec", sender );
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
	}
}


void dp88_indirectTargetting_Dec::KeyHook()
{
	// Find object
	GameObject* obj = Commands->Find_Object ( objectID );
	if ( !obj )
		return;


	(*targettingLevel)--;
	float targetFrame = (float)((*targettingLevel)/2);
	Commands->Set_Animation ( obj, Get_Parameter ( "targettingAnimation" ), false, NULL, 1.0f, targetFrame, false );
}


ScriptRegistrant<dp88_indirectTargetting> dp88_indirectTargetting_Registrant(
	"dp88_indirectTargetting",
	"targettingAnimation=obj.obj:string,"
	"targettingLevels=100:int,"
	"minFrame=1:int,"
	"maxFrame=100:int"
);

ScriptRegistrant<dp88_indirectTargetting_Dec> dp88_indirectTargetting_Dec_Registrant(
	"dp88_indirectTargetting_Dec",
	"targettingAnimation=obj.obj:string,"
	"targettingLevels=100:int,"
	"minFrame=1:int,"
	"maxFrame=100:int,"
	"TL_ADDR=0:int"
);










/*------------------------
Unit sounds script
--------------------------*/

void dp88_unitSounds::Created ( GameObject* obj )
{
	Update_Network_Object(obj);
	healAmount = 0;
	if ( strcmp ( Get_Parameter ( "createdSound" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter ( "createdSound" ) ) )
		Commands->Create_3D_Sound_At_Bone( Get_Parameter ( "createdSound" ), obj, "origin" );


	if ( strcmp ( Get_Parameter ( "damagedSound" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter ( "damagedSound" ) ) )
		damagedEnabled = true;
	else
		damagedEnabled = false;


	if ( strcmp ( Get_Parameter ( "heavilyDamagedSound" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter ( "heavilyDamagedSound" ) ) )
		heavilyDamagedEnabled = true;
	else
		heavilyDamagedEnabled = false;


	if ( strcmp ( Get_Parameter ( "healedSound" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter ( "healedSound" ) ) )
		healedEnabled = true;
	else
		healedEnabled = false;


	if ( strcmp ( Get_Parameter ( "fallingDamageSound" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter ( "fallingDamageSound" ) ) )
		fallingDamageEnabled = true;
	else
		fallingDamageEnabled = false;
}

void dp88_unitSounds::Damaged( GameObject *obj, GameObject *damager, float amount )
{
	// Bail out if damager == null or damager == obj
	if ( damager == NULL || Commands->Get_ID ( damager ) == Commands->Get_ID ( obj ) )
		return;

	if ( amount > 0 && heavilyDamagedEnabled && amount > Get_Int_Parameter ( "heavilyDamagedSoundMinDamageAmount" ) )
	{
		if ( Commands->Get_Random_Int ( 0,101 ) <= Get_Int_Parameter ( "heavilyDamagedSoundProbability" ) )
		{
			heavilyDamagedEnabled = false;
			Commands->Create_3D_Sound_At_Bone( Get_Parameter ( "heavilyDamagedSound" ), obj, "origin" );
			Commands->Start_Timer ( obj, this, (float)Get_Int_Parameter( "heavilyDamagedSoundMinInterval" ), 2 );
		}
	}


	else if ( amount > 0 && damagedEnabled )
	{
		if ( Commands->Get_Random_Int ( 0,101 ) <= Get_Int_Parameter ( "damagedSoundProbability" ) )
		{
			damagedEnabled = false;
			Commands->Create_3D_Sound_At_Bone( Get_Parameter ( "damagedSound" ), obj, "origin" );
			Commands->Start_Timer ( obj, this, (float)Get_Int_Parameter( "damagedSoundMinInterval" ), 1 );
		}
	}


	else if ( amount < 0 && healedEnabled )
	{
		healAmount += (int)-amount;
		
		// Send delayed custom to decrement heal amount
		Commands->Send_Custom_Event ( obj, obj, CUSTOM_UNITSOUNDS_HEALDECREMENT, (int)-amount, (float)Get_Int_Parameter ( "healedSoundCheckTime" ) );

		if ( healAmount >= Get_Int_Parameter ( "healedSoundMinHealAmount" ) )
		{
			healedEnabled = false;
			Commands->Create_3D_Sound_At_Bone( Get_Parameter ( "healedSound" ), obj, "origin" );
			Commands->Start_Timer ( obj, this, (float)Get_Int_Parameter( "healedSoundMinInterval" ), 3 );
		}
	}
}

void dp88_unitSounds::Killed ( GameObject *obj, GameObject *killer )
{
	if ( strcmp ( Get_Parameter ( "killedSound" ), "null" ) != 0 && Is_Valid_Preset ( Get_Parameter ( "killedSound" ) ) )
		Commands->Create_3D_Sound_At_Bone( Get_Parameter ( "killedSound" ), obj, "origin" );
}

void dp88_unitSounds::Custom ( GameObject* obj, int type, int param, GameObject* sender )
{
	if ( type == CUSTOM_EVENT_FALLING_DAMAGE && fallingDamageEnabled )
	{
		fallingDamageEnabled = false;
		Commands->Create_3D_Sound_At_Bone( Get_Parameter ( "fallingDamageSound" ), obj, "origin" );
		Commands->Start_Timer ( obj, this, (float)Get_Int_Parameter( "fallingDamageSoundMinInterval" ), 4 );
	}

	else if ( type == CUSTOM_UNITSOUNDS_HEALDECREMENT )
		healAmount -= param;
}

void dp88_unitSounds::Timer_Expired ( GameObject* obj, int number )
{
	if ( number == 1 )
		damagedEnabled = true;
	else if ( number == 2 )
		heavilyDamagedEnabled = true;
	else if ( number == 3 )
		healedEnabled = true;
	else if ( number == 4 )
		fallingDamageEnabled = true;
}


ScriptRegistrant<dp88_unitSounds> dp88_unitSounds_Registrant(
	"dp88_unitSounds",
	"createdSound=null:string,"
	"damagedSound=null:string,"
	"damagedSoundProbability=15:int,"
	"damagedSoundMinInterval=5:int,"
	"heavilyDamagedSound=null:string,"
	"heavilyDamagedSoundMinDamageAmount=40:int,"
	"heavilyDamagedSoundProbability=15:int,"
	"heavilyDamagedSoundMinInterval=5:int,"
	"healedSound=null:string,"
	"healedSoundMinHealAmount=45:int,"
	"healedSoundCheckTime=15:int,"
	"healedSoundMinInterval=30:int,"
	"fallingDamageSound=null:string,"
	"fallingDamageSoundMinInterval=5:int,"
	"killedSound=null:string"
);










/*------------------------
Unique Infantry Controller
--------------------------*/

void dp88_uniqueInfantry_controller::Created ( GameObject* obj )
{
	infantry_preset = new char[strlen(Get_Parameter("infantry_preset"))+1];
	strcpy ( infantry_preset, Get_Parameter("infantry_preset") );
	team_id = Get_Int_Parameter("team_id");
	max_count = Get_Int_Parameter("max_count");
	percent_chance = Get_Int_Parameter("percent_chance");

	// Die if infantry_preset is not valid
	if ( !Is_Valid_Preset(infantry_preset) )
	{
		Console_Output("dp88_uniqueInfantry_controller: %s is not a valid infantry preset! Destroying script...\n", infantry_preset );
		Destroy_Script();
	}
	else
	{
		hookid = 0;
		InstallHook();
	}
}

void dp88_uniqueInfantry_controller::Destroyed ( GameObject* obj )
{
	if ( infantry_preset != NULL )
		delete [] infantry_preset;
}

void dp88_uniqueInfantry_controller::ObjectCreateHook ( GameObject *obj )
{
		if ( Commands->Is_A_Star(obj)
		&& Get_Object_Type(obj) == team_id
		&& Get_Object_Count(team_id,infantry_preset) < max_count
		&& Commands->Get_Random_Int(0,100) < percent_chance )
	{
		Change_Character(obj,infantry_preset);
	}
}


ScriptRegistrant<dp88_uniqueInfantry_controller> dp88_uniqueInfantry_controller_Registrant(
	"dp88_uniqueInfantry_controller",
	"infantry_preset:string,"
	"team_id:int,"
	"max_count=1:int,"
	"percent_chance=20:int"
);










// -------------------------------------------------------------------------------------------------
// Damage animation script
// -------------------------------------------------------------------------------------------------

void dp88_damageAnimation::Created ( GameObject* obj )
{
  loopAnimation = (Get_Int_Parameter ( "loopAnimation" ) == 1);
  m_pLoopedAnimCtrl = new LoopedAnimationController(obj);   // Can also do single-iteration loops :) 

  /* Load settings */
  damageLevelBoundaries[0] = 100;
  damageLevelStartFrames[0] = Get_Int_Parameter("healthy_startFrame");
  damageLevelEndFrames[0] = Get_Int_Parameter("healthy_endFrame");
  damageLevelLowPowerStartFrames[0] = Get_Int_Parameter("healthy_startFrame_lowPower");
  damageLevelLowPowerEndFrames[0] = Get_Int_Parameter("healthy_endFrame_lowPower");

  damageLevelBoundaries[1] = Get_Int_Parameter("damageBoundary1");
  damageLevelStartFrames[1] = Get_Int_Parameter("damageBoundary1_startFrame");
  damageLevelEndFrames[1] = Get_Int_Parameter("damageBoundary1_endFrame");
  damageLevelLowPowerStartFrames[1] = Get_Int_Parameter("damageBoundary1_startFrame_lowPower");
  damageLevelLowPowerEndFrames[1] = Get_Int_Parameter("damageBoundary1_endFrame_lowPower");

  damageLevelBoundaries[2] = Get_Int_Parameter("damageBoundary2");
  damageLevelStartFrames[2] = Get_Int_Parameter("damageBoundary2_startFrame");
  damageLevelEndFrames[2] = Get_Int_Parameter("damageBoundary2_endFrame");
  damageLevelLowPowerStartFrames[2] = Get_Int_Parameter("damageBoundary2_startFrame_lowPower");
  damageLevelLowPowerEndFrames[2] = Get_Int_Parameter("damageBoundary2_endFrame_lowPower");

  damageLevelBoundaries[3] = Get_Int_Parameter("damageBoundary3");
  damageLevelStartFrames[3] = Get_Int_Parameter("damageBoundary3_startFrame");
  damageLevelEndFrames[3] = Get_Int_Parameter("damageBoundary3_endFrame");
  damageLevelLowPowerStartFrames[3] = Get_Int_Parameter("damageBoundary3_startFrame_lowPower");
  damageLevelLowPowerEndFrames[3] = Get_Int_Parameter("damageBoundary3_endFrame_lowPower");

  damageLevelBoundaries[4] = Get_Int_Parameter("damageBoundary4");
  damageLevelStartFrames[4] = Get_Int_Parameter("damageBoundary4_startFrame");
  damageLevelEndFrames[4] = Get_Int_Parameter("damageBoundary4_endFrame");
  damageLevelLowPowerStartFrames[4] = Get_Int_Parameter("damageBoundary4_startFrame_lowPower");
  damageLevelLowPowerEndFrames[4] = Get_Int_Parameter("damageBoundary4_endFrame_lowPower");

  damageLevelBoundaries[5] = Get_Int_Parameter("damageBoundary5");
  damageLevelStartFrames[5] = Get_Int_Parameter("damageBoundary5_startFrame");
  damageLevelEndFrames[5] = Get_Int_Parameter("damageBoundary5_endFrame");
  damageLevelLowPowerStartFrames[5] = Get_Int_Parameter("damageBoundary5_startFrame_lowPower");
  damageLevelLowPowerEndFrames[5] = Get_Int_Parameter("damageBoundary5_endFrame_lowPower");

  /* Set default level */
  currentDamageLevel = 0;
  basePowerState = Is_Base_Powered(Get_Object_Type(obj));
  SetDamageAnimation(obj);

	/* If any of the damage levels have a different set of frames for the low power state
	then start the timer to check power state regularly */
	for ( int i = 0; i < 6; i++ )
	{
		if ( damageLevelBoundaries[i] >= 0 && damageLevelLowPowerStartFrames[i] >= 0
			&& (damageLevelLowPowerStartFrames[i] != damageLevelStartFrames[i]
				|| damageLevelLowPowerEndFrames[i] != damageLevelEndFrames[i])
			)
		{
			Commands->Start_Timer(obj, this, 1.0f, TIMER_CHECK_BASE_POWER_STATE );
			break;
		}
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_damageAnimation::Detach ( GameObject* obj )
{
	if (m_pLoopedAnimCtrl)
	{
		delete m_pLoopedAnimCtrl;
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_damageAnimation::Damaged( GameObject *obj, GameObject *damager, float amount )
{
	if ( amount >= 0 )
	{
		while ( currentDamageLevel < 5 && damageLevelBoundaries[currentDamageLevel+1] >= 0
			&& ((Commands->Get_Health(obj)/Commands->Get_Max_Health(obj))*100.0f) < damageLevelBoundaries[currentDamageLevel+1] )
		{
			currentDamageLevel++;
			basePowerState = Is_Base_Powered(Get_Object_Type(obj));
			SetDamageAnimation(obj);
		}
	}

	// Repairs make the damage level go backwards
	if ( amount <= 0 )
	{
		while ( currentDamageLevel > 0
			&& ((Commands->Get_Health(obj)/Commands->Get_Max_Health(obj))*100.0f) > damageLevelBoundaries[currentDamageLevel] )
		{
			currentDamageLevel--;
			basePowerState = Is_Base_Powered(Get_Object_Type(obj));
			SetDamageAnimation(obj);
		}
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_damageAnimation::Timer_Expired ( GameObject* obj, int number )
{
	if ( number == TIMER_CHECK_BASE_POWER_STATE )
	{
		if ( Is_Base_Powered(Get_Object_Type(obj)) != basePowerState )
		{
			basePowerState = !basePowerState;
			SetDamageAnimation(obj);
		}

		// Restart timer
		Commands->Start_Timer(obj, this, 1.0f, TIMER_CHECK_BASE_POWER_STATE );
	}
}

// -------------------------------------------------------------------------------------------------

void dp88_damageAnimation::SetDamageAnimation ( GameObject* obj )
{
  if ( basePowerState || damageLevelLowPowerStartFrames[currentDamageLevel] < 0 )
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), damageLevelStartFrames[currentDamageLevel], damageLevelEndFrames[currentDamageLevel], (loopAnimation)?0:1 );
  else
    m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("animationName"), damageLevelLowPowerStartFrames[currentDamageLevel], damageLevelLowPowerEndFrames[currentDamageLevel], (loopAnimation)?0:1 );
}

// -------------------------------------------------------------------------------------------------

ScriptRegistrant<dp88_damageAnimation> dp88_damageAnimation_Registrant(
  "dp88_damageAnimation",
  "animationName=object.object:string,"
  "loopAnimation=1:int,"
  "healthy_startFrame=0:int,"
  "healthy_endFrame=0:int,"
  "healthy_startFrame_lowPower=-1:int,"
  "healthy_endFrame_lowPower=-1:int,"
  "damageBoundary1=-1:int,"
  "damageBoundary1_startFrame=0:int,"
  "damageBoundary1_endFrame=0:int,"
  "damageBoundary1_startFrame_lowPower=-1:int,"
  "damageBoundary1_endFrame_lowPower=-1:int,"
  "damageBoundary2=-1:int,"
  "damageBoundary2_startFrame=0:int,"
  "damageBoundary2_endFrame=0:int,"
  "damageBoundary2_startFrame_lowPower=-1:int,"
  "damageBoundary2_endFrame_lowPower=-1:int,"
  "damageBoundary3=-1:int,"
  "damageBoundary3_startFrame=0:int,"
  "damageBoundary3_endFrame=0:int,"
  "damageBoundary3_startFrame_lowPower=-1:int,"
  "damageBoundary3_endFrame_lowPower=-1:int,"
  "damageBoundary4=-1:int,"
  "damageBoundary4_startFrame=0:int,"
  "damageBoundary4_endFrame=0:int,"
  "damageBoundary4_startFrame_lowPower=-1:int,"
  "damageBoundary4_endFrame_lowPower=-1:int,"
  "damageBoundary5=-1:int,"
  "damageBoundary5_startFrame=0:int,"
  "damageBoundary5_endFrame=0:int,"
  "damageBoundary5_startFrame_lowPower=-1:int,"
  "damageBoundary5_endFrame_lowPower=-1:int"
);

// -------------------------------------------------------------------------------------------------










/*------------------------
Object Annoucements script
--------------------------*/

void dp88_objectAnnouncements::Created ( GameObject* obj )
{
	lastAnnouncementTime = 0;
	minimumAnnounceInterval = Get_Int_Parameter("minimumAnnounceInterval");

	/* By default we set all sound IDs to 0 */
	team0_underAttackSoundId = 0;
	team1_underAttackSoundId = 0;
	team0_destroyedSoundId = 0;
	team1_destroyedSoundId = 0;

	/* Get string IDs and for each one determine if it has a valid sound
	attached to it */
	unsigned long soundId;
	team0_underAttackStringId = (Is_Valid_String_ID(Get_Int_Parameter("team0_underAttackStringId"))) ? Get_Int_Parameter("team0_underAttackStringId") : 0;
	if ( team0_underAttackStringId && (soundId = Get_String_Sound_ID(team0_underAttackStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		team0_underAttackSoundId = soundId;

	team1_underAttackStringId = (Is_Valid_String_ID(Get_Int_Parameter("team1_underAttackStringId"))) ? Get_Int_Parameter("team1_underAttackStringId") : 0;
	if ( team1_underAttackStringId && (soundId = Get_String_Sound_ID(team1_underAttackStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		team1_underAttackSoundId = soundId;

	team0_destroyedStringId = (Is_Valid_String_ID(Get_Int_Parameter("team0_destroyedStringId"))) ? Get_Int_Parameter("team0_destroyedStringId") : 0;
	if ( team0_destroyedStringId && (soundId = Get_String_Sound_ID(team0_destroyedStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		team0_destroyedSoundId = soundId;

	team1_destroyedStringId = (Is_Valid_String_ID(Get_Int_Parameter("team1_destroyedStringId"))) ? Get_Int_Parameter("team1_destroyedStringId") : 0;
	if ( team1_destroyedStringId && (soundId = Get_String_Sound_ID(team1_destroyedStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		team1_destroyedSoundId = soundId;
}


void dp88_objectAnnouncements::Damaged( GameObject *obj, GameObject *damager, float amount )
{
	/* If damage has been done and it is more than minimumAnnounceInterval seconds since the last
	announcement was made then trigger the under attack announcement */
	if ( amount > 0 && time(NULL) - lastAnnouncementTime > minimumAnnounceInterval )
	{
		lastAnnouncementTime = time(NULL);

		/* Send messages to teams */
		if ( team0_underAttackStringId )
		{
			const char *str = Get_Translated_String(team0_underAttackStringId);
			Send_Message_Team(0,255,255,255,str);
			delete[] str;
			if ( team0_underAttackSoundId )
				Create_2D_Sound_Team(Get_Definition_Name(team0_underAttackSoundId),0);
		}

		if ( team1_underAttackStringId )
		{
			const char *str = Get_Translated_String(team1_underAttackStringId);
			Send_Message_Team(1,255,255,255,str);
			delete[] str;
			if ( team1_underAttackSoundId )
				Create_2D_Sound_Team(Get_Definition_Name(team1_underAttackSoundId),1);
		}
	}
}


void dp88_objectAnnouncements::Killed ( GameObject *obj, GameObject *killer )
{
	/* Send messages to teams */
	if ( team0_destroyedStringId )
	{
		const char *str = Get_Translated_String(team0_destroyedStringId);
		Send_Message_Team(0,255,255,255,str);
		delete[] str;
		if ( team0_destroyedSoundId )
			Create_2D_Sound_Team(Get_Definition_Name(team0_destroyedSoundId),0);
	}
		
	if ( team1_destroyedStringId )
	{
		const char *str = Get_Translated_String(team1_destroyedStringId);
		Send_Message_Team(1,255,255,255,str);
		delete[] str;
		if ( team1_destroyedSoundId )
			Create_2D_Sound_Team(Get_Definition_Name(team1_destroyedSoundId),1);
	}
}


ScriptRegistrant<dp88_objectAnnouncements> dp88_objectAnnouncements_Registrant(
	"dp88_objectAnnouncements",
	"minimumAnnounceInterval=20:int,"
	"team0_underAttackStringId=0:int,"
	"team1_underAttackStringId=0:int,"
	"team0_destroyedStringId=0:int,"
	"team1_destroyedStringId=0:int,"
);










/*------------------------
Chrono Tank Script
--------------------------*/

void dp88_chronoTank::Created ( GameObject* obj )
{
	state = CT_STATE_CHARGED;
	object_id = Commands->Get_ID(obj);
	driver_id = NULL;
	shifted_vehicle_object_id = NULL;

	// Can only be used on vehicles
	if ( !obj->As_VehicleGameObj() )
	{
		Console_Output ( "[%d:%s:%s] Critical Error: This script can only be used on vehicles! Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
		Destroy_Script();
	}

	// Check the shifted vehicle preset is valid
	if ( !Is_Valid_Preset(Get_Parameter("shifted_vehicle_preset")) )
	{
		Console_Output ( "[%d:%s:%s] Critical Error: A vehicle preset with the name '%s' was not found. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("shifted_vehicle_preset") );
		Destroy_Script();
	}
}


void dp88_chronoTank::Killed ( GameObject *obj, GameObject *killer )
{
	if ( shifted_vehicle_object_id != NULL )
	{
		Commands->Destroy_Object(Commands->Find_Object(shifted_vehicle_object_id));
		shifted_vehicle_object_id = NULL;
	}
}


void dp88_chronoTank::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Look for vehicle entry
	if ( type == CUSTOM_EVENT_VEHICLE_ENTERED && driver_id == NULL )
	{
		driver_id = Commands->Get_ID(sender);
		InstallHook( Get_Parameter("shift_keyhook"), sender );
	}


	// Look for vehicle exit (ignore when leaving due to chronoshift)
	else if ( type == CUSTOM_EVENT_VEHICLE_EXITED && Commands->Get_ID(sender) == driver_id && state != CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_1 )
	{
		driver_id = NULL;
		RemoveHook();
	}
}


void dp88_chronoTank::Timer_Expired( GameObject *obj, int number )
{
	// Timer to advance to the next stage of the chronoshift process
	if ( number == TIMER_CHRONOTANK_ADVANCE_STATE )
	{
		// Chronoshift device has recharged, set state and send message to driver
		if ( state == CT_STATE_CHARGING )
		{
			state = CT_STATE_CHARGED;

			if ( driver_id != NULL )
				Send_Message_Player(Commands->Find_Object(driver_id),255,255,255,"The chronoshift device is now fully charged and ready for use");
		}


		// Shift out sequence has completed
		else if ( state == CT_STATE_SHIFTING_OUT )
			Shift_Out_Complete(obj);


		// Shift out sequence is ready for cleanup
		else if ( state == CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_1 || state == CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_2 )
			Shift_Out_Cleanup(obj);


		// Chronoshift time has expired
		else if ( state == CT_STATE_SHIFTED_OUT )
			Shift_In_Begin(obj);


		// Shift in sequence has completed
		else if ( state == CT_STATE_SHIFTING_IN )
			Shift_In_Complete(obj);


		// Shift in sequence is ready for cleanup
		else if ( state == CT_STATE_SHIFTED_IN_PENDING_CLEANUP_1 || state == CT_STATE_SHIFTED_IN_PENDING_CLEANUP_2 )
			Shift_In_Cleanup(obj);
	}


	// Timer to apply random interference actions
	else if ( number == TIMER_CHRONOTANK_RANDOM_INTERFERENCE && shifted_vehicle_object_id != NULL && state == CT_STATE_SHIFTED_OUT )
	{
		// Get shifted vehicle
		GameObject* shifted_vehicle = Commands->Find_Object(shifted_vehicle_object_id);


		// Alter heading
		if ( Get_Int_Parameter("shift_heading_fluctuation_degrees_max") > 0 )
		{
			float heading = Commands->Get_Facing (shifted_vehicle);
			Console_Output ( "Heading was %.2f\n", heading );
			heading += Commands->Get_Random_Int(Get_Int_Parameter("shift_heading_fluctuation_degrees_max")*-1,Get_Int_Parameter("shift_heading_fluctuation_degrees_max"));
			heading = (heading<0) ? heading+360 : ((heading>360) ? heading-360 : heading);
			Console_Output ( "Heading is now %.2f\n", heading );
			Commands->Set_Facing(shifted_vehicle, heading);
		}


		// Alter velocity - move x% of velocity from a random axis to another random axis
		float velocity_fluctuation_percent_max = Get_Float_Parameter("shift_velocity_fluctuation_percent_max");
		if (velocity_fluctuation_percent_max > 0 )
		{
			Vector3 velocity = Get_Velocity (shifted_vehicle);
			Console_Output ( "Velocity was %.2f, %.2f, %.2f\n", velocity.X, velocity.Y, velocity.Z );

			float avgVelocity = (abs(velocity.X) + abs(velocity.Y) + abs(velocity.Z))/3.0f;
			float percentChange = Commands->Get_Random(0.0f,velocity_fluctuation_percent_max);
			float velocityChange = avgVelocity * (percentChange/100.0f);

			// Pick a random axis to apply the changes to
			int subAxis = Commands->Get_Random_Int(1,3);
			int plusAxis = Commands->Get_Random_Int(1,3);
			while (plusAxis == subAxis)
				plusAxis = Commands->Get_Random_Int(1,3);
			Console_Output ( "Subtracting %.2f%% velocity (%.2f) from axis %d\n", percentChange, velocityChange*-1.0f, subAxis );
			Console_Output ( "Adding %.2f%% velocity (%.2f) to axis %d\n", percentChange, velocityChange, plusAxis );

			// Apply the velocity changes
			if ( plusAxis == 1 )
				velocity.X -= velocityChange;
			else if ( plusAxis == 2 )
				velocity.Y -= velocityChange;
			else if ( plusAxis == 3 )
				velocity.Z -= velocityChange;
			
			if ( subAxis == 1 )
				velocity.X += velocityChange;
			else if ( subAxis == 2 )
				velocity.Y += velocityChange;
			else if ( subAxis == 3 )
				velocity.Z += velocityChange;

			// Set new velocity
			Console_Output ( "Velocity is now %.2f, %.2f, %.2f\n", velocity.X, velocity.Y, velocity.Z );
			Set_Velocity(shifted_vehicle, velocity);
		}

		// Start timer again for the next set of interference actions
		Commands->Start_Timer(obj,this,1.00f,TIMER_CHRONOTANK_RANDOM_INTERFERENCE);
	}
}


void dp88_chronoTank::KeyHook()
{
	GameObject* obj = Commands->Find_Object(object_id);
	if ( state == CT_STATE_CHARGED )
		Shift_Out_Begin(obj);
	else if ( state == CT_STATE_CHARGING )
		Send_Message_Player(Commands->Find_Object(driver_id),255,255,255,"The chronoshift device has not finished recharging yet");
}


void dp88_chronoTank::Shift_Out_Begin(GameObject* obj)
{
	if ( state != CT_STATE_CHARGED )
		return;

	// Set new state
	state = CT_STATE_SHIFTING_OUT;

	// Lock the vehicle in it's current location and prevent the driver getting out
	obj->As_VehicleGameObj()->Set_Immovable(true);
	Commands->Enable_Vehicle_Transitions(obj,false);

	// Create the shifted out vehicle in a safe location (pre-creation reduces the transition delay)
	GameObject* shifted_vehicle = Commands->Create_Object ( Get_Parameter("shifted_vehicle_preset"), Vector3(0.0f,0.0f,500.0f) );
	shifted_vehicle_object_id = Commands->Get_ID(shifted_vehicle);

	// Start timer to advance to next stage (shift out complete)
	Commands->Start_Timer(obj,this,Get_Float_Parameter("shift_out_time"),TIMER_CHRONOTANK_ADVANCE_STATE);

	/* Create shift out effect if the preset provided is valid */
	if ( Is_Valid_Preset(Get_Parameter("shift_out_effect_preset")) )
	{
		// Create effect object
		GameObject* effect_object = Commands->Create_Object ( Get_Parameter("shift_out_effect_preset"), Commands->Get_Position(obj) );

		// Attach script to clean up effect
		StringClass params(true);
		params.Format ( "%.2f,%d", Get_Float_Parameter("shift_out_effect_time"), 983142 );
		Commands->Attach_Script ( effect_object, "JFW_Destroy_Self_Timer", params.Peek_Buffer() );
	}
}


void dp88_chronoTank::Shift_Out_Complete(GameObject* obj)
{
	// Enable vehicle transitions to allow the driver out
	Commands->Enable_Vehicle_Transitions(obj,true);

	// Transition the driver out of this vehicle
	Soldier_Transition_Vehicle(Commands->Find_Object(driver_id));

	// Set state and start timer to advance to next state (cleanup step 1) on next engine tick
	state = CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_1;
	Commands->Start_Timer(obj,this,5.2f,TIMER_CHRONOTANK_ADVANCE_STATE);
}


void dp88_chronoTank::Shift_Out_Cleanup(GameObject* obj)
{
	// Cleanup step 1
	if ( state == CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_1 )
	{
		// Get shifted vehicle
		GameObject* shifted_vehicle = Commands->Find_Object(shifted_vehicle_object_id);

		// Make the shifted vehicle invisible to base defences
		Commands->Set_Is_Visible(shifted_vehicle,false);

		// Move the shifted vehicle to this location
		Set_Transform(shifted_vehicle, Get_Transform(obj) );

		// Move the driver to where the shifted vehicle is
		Commands->Set_Position(Commands->Find_Object(driver_id),Commands->Get_Position(obj));

		// Move this vehicle to a safe place out of harms way and make it invisible
		Commands->Set_Position(obj,Vector3(0.0f,0.0f,500.0f));
		Commands->Set_Is_Rendered ( obj, false );

		// Set state and start timer to advance to next state (cleanup step 2) on next engine tick
		state = CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_2;
		Commands->Start_Timer(obj,this,5.4f,TIMER_CHRONOTANK_ADVANCE_STATE);
	}

	// Cleanup step 2
	else
	{
		// Get shifted vehicle
		GameObject* shifted_vehicle = Commands->Find_Object(shifted_vehicle_object_id);

		// Move the driver to where the shifted vehicle is (make sure they don't walk off...)
		Commands->Set_Position(Commands->Find_Object(driver_id),Commands->Get_Position(shifted_vehicle));

		// Transition the driver into the shifted vehicle
		ActionParamsStruct params;
		params.Set_Basic((long)0,100,39);
		GameObject* driver = Commands->Find_Object(driver_id);
		if ( driver )
			Commands->Action_Enter_Exit(driver,params);

		// Set state and start timer to advance to next stage (shift in)
		state = CT_STATE_SHIFTED_OUT;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("shift_duration"),TIMER_CHRONOTANK_ADVANCE_STATE);

		// Begin timer for random interference actions
		Commands->Start_Timer(obj,this,1.5f,TIMER_CHRONOTANK_RANDOM_INTERFERENCE);
	}
}


void dp88_chronoTank::Shift_In_Begin(GameObject* obj)
{
	// Set new state
	state = CT_STATE_SHIFTING_IN;

	// Get shifted vehicle
	GameObject* shifted_vehicle = Commands->Find_Object(shifted_vehicle_object_id);

	// Lock the shifted vehicle in position
	shifted_vehicle->As_VehicleGameObj()->Set_Immovable(true);

	// Start timer to advance to next stage (shift in complete)
	Commands->Start_Timer(obj,this,Get_Float_Parameter("shift_in_time"),TIMER_CHRONOTANK_ADVANCE_STATE);

	/* Create shift in effect if the preset provided is valid */
	if ( Is_Valid_Preset(Get_Parameter("shift_in_effect_preset")) )
	{
		// Create effect object
		GameObject* effect_object = Commands->Create_Object ( Get_Parameter("shift_in_effect_preset"), Commands->Get_Position(shifted_vehicle) );

		// Attach script to clean up effect
		StringClass params(true);
		params.Format ( "%.2f,%d", Get_Float_Parameter("shift_in_effect_time"), 983142 );
		Commands->Attach_Script ( effect_object, "JFW_Destroy_Self_Timer", params.Peek_Buffer() );
	}
}


void dp88_chronoTank::Shift_In_Complete(GameObject* obj)
{
	// Get shifted vehicle
	GameObject* shifted_vehicle = Commands->Find_Object(shifted_vehicle_object_id);

	// Move this vehicle to the location of the shifted vehicle
	Set_Transform(obj, Get_Transform(shifted_vehicle) );

	// Make this vehicle visible again
	Commands->Set_Is_Rendered(obj, true);

	// Destroy the shifted vehicle
	Commands->Destroy_Object(Commands->Find_Object(shifted_vehicle_object_id));
	shifted_vehicle_object_id = NULL;

	// Set state
	state = CT_STATE_SHIFTED_IN_PENDING_CLEANUP_1;

	// Start timer to advance to next state (cleanup) on next engine tick
	Commands->Start_Timer(obj,this,5.01f,TIMER_CHRONOTANK_ADVANCE_STATE);
}


void dp88_chronoTank::Shift_In_Cleanup(GameObject* obj)
{
	// Cleanup step 1
	if ( state == CT_STATE_SHIFTED_IN_PENDING_CLEANUP_1 )
	{
		// Move driver to location of this vehicle
		Commands->Set_Position(Commands->Find_Object(driver_id),Commands->Get_Position(obj));

		// Set state and start timer to advance to next state (cleanup step 2) on next engine tick
		state = CT_STATE_SHIFTED_IN_PENDING_CLEANUP_2;
		Commands->Start_Timer(obj,this,5.4f,TIMER_CHRONOTANK_ADVANCE_STATE);
	}

	// Cleanup step 2
	else
	{
		// Transition the driver into this vehicle
		ActionParamsStruct params;
		params.Set_Basic((long)0,100,39);
		GameObject* driver = Commands->Find_Object(driver_id);
		if ( driver )
			Commands->Action_Enter_Exit(driver,params);

		// Allow this vehicle to be mobile again
		obj->As_VehicleGameObj()->Set_Immovable(false);

		// Set state and start timer to advance to next stage (charged)
		state = CT_STATE_CHARGING;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("recharge_time"),TIMER_CHRONOTANK_ADVANCE_STATE);
	}
}


ScriptRegistrant<dp88_chronoTank> dp88_chronoTank_Registrant(
	"dp88_chronoTank",
	"shift_keyhook:string,"
	"shift_duration:int,"
	"recharge_time:int,"
	"shifted_vehicle_preset:string,"
	"shift_out_time:float,"
	"shift_out_effect_preset:string,"
	"shift_out_effect_time:float,"
	"shift_in_time:float,"
	"shift_in_effect_preset:string,"
	"shift_in_effect_time:float,"
	"shift_heading_fluctuation_degrees_max=10:int,"
	"shift_velocity_fluctuation_percent_max=10:float"
);










/*------------------------
Spawn Vehicle Part script
--------------------------*/

void dp88_spawnVehiclePart::Created ( GameObject* obj )
{
	partObjectId = NULL;

	/* Start a timer to spawn the vehicle part. We can't do this immediatly because
  it seems to cause issues with double initializations of scripts... */
	Commands->Start_Timer ( obj, this, 0.01f, TIMER_SPAWNVEHICLEPART );
}


void dp88_spawnVehiclePart::Timer_Expired( GameObject *obj, int number )
{
	/* Part spawn timer */
	if ( number == TIMER_SPAWNVEHICLEPART && partObjectId == NULL )
	{
		/* Spawn the vehicle part and get it's ID */
		GameObject* part = Commands->Create_Object_At_Bone ( obj, Get_Parameter("Vehicle_Part_Preset"), Get_Parameter("Spawn_At_Bone") );
		Commands->Attach_To_Object_Bone ( part, obj, Get_Parameter ( "Spawn_At_Bone" ) );
		partObjectId = Commands->Get_ID(part);

		/* Unless this is a decorative part start the timer again to attach the health and
		veterancy link scripts. We don't do this immediately as it causes a lovely crash... */
		if ( Get_Int_Parameter("IsDecorative") == 0 )
		  Commands->Start_Timer ( obj, this, 0.01f, TIMER_SPAWNVEHICLEPART );
	}


	/* Link health & veterancy script attach timer */
	else if ( number == TIMER_SPAWNVEHICLEPART )
	{
		GameObject* part = Commands->Find_Object(partObjectId);
		if ( part )
		{
			char hostIdString[12];
			sprintf ( hostIdString, "%d", Commands->Get_ID(obj) );
			Attach_Script_Once ( part, "dp88_linkHealth", hostIdString );
			Attach_Script_Once ( part, "dp88_linkVetPoints", hostIdString );
		}
	}
}


void dp88_spawnVehiclePart::Killed ( GameObject* obj, GameObject *killer )
{
	if ( partObjectId != NULL && Commands->Find_Object(partObjectId) )
		Commands->Destroy_Object ( Commands->Find_Object (partObjectId) );
	partObjectId = NULL;
}


void dp88_spawnVehiclePart::Destroyed ( GameObject* obj )
{
	if ( partObjectId != NULL && Commands->Find_Object(partObjectId) )
		Commands->Destroy_Object ( Commands->Find_Object (partObjectId) );
	partObjectId = NULL;
}


ScriptRegistrant<dp88_spawnVehiclePart> dp88_spawnVehiclePart_Registrant(
	"dp88_spawnVehiclePart",
	"Vehicle_Part_Preset:string,"
	"Spawn_At_Bone:string,"
	"IsDecorative=0:int"
);










/*------------------------
Custom Points script
--------------------------*/

void dp88_customPoints::Created ( GameObject* obj )
{
	// Preload values since converting strings to floats every time we take
	// damage is very inefficient
	killPoints		= Get_Float_Parameter("killPoints");
	damagePoints	= Get_Float_Parameter("damagePoints");
	repairPoints	= Get_Float_Parameter("repairPoints");
	killMoney		= Get_Float_Parameter("killMoney");
	damageMoney		= Get_Float_Parameter("damageMoney");
	repairMoney		= Get_Float_Parameter("repairMoney");
}


void dp88_customPoints::Damaged ( GameObject *obj, GameObject *damager, float amount )
{
	// Abort if there is no damager, or the unit damaged itself, or the damager is not
	// a star
	if ( !damager || damager == obj || !Commands->Is_A_Star(damager) )
		return;

	/* Damaged or repaired? Then convert amount into it's absolute value for
	calculating points granted */
	bool bRepair = (amount < 0);
	amount = abs(amount);
	float points = ((bRepair) ? repairPoints : damagePoints) * amount;
	float money = ((bRepair) ? repairMoney : damageMoney) * amount;

	// Is this player an APB spy?
	bool bSpy = Is_Spy(damager);

	// Repaired other team or damaged own team? That would be negative points and no
	// money for you then... unless your spy, then the logic is inverted...
	if ( bSpy ^ ((Get_Object_Type(obj) == Get_Object_Type(damager)) != bRepair) )
	{
		points *= -1.0f;
		money = 0.0f;
	}

	// Give the damager their points and money
	Commands->Give_Points(damager, points, false);
	Commands->Give_Money(damager, money, false);
}


void dp88_customPoints::Killed ( GameObject* obj, GameObject *killer )
{
	// Abort if there is no killer, or the unit killed itself, or the killer is not
	// a star
	if ( !killer || killer == obj || !Commands->Is_A_Star(killer) )
		return;

	// Killed own teammate? That would be negative points and no cash for you then...
	if ( Get_Object_Type(obj) == Get_Object_Type(killer) )
	{
		Commands->Give_Points(killer, killPoints*-1.0f, false);
	}

	else
	{
		// Give the killer their points and money
		Commands->Give_Points(killer, killPoints, false);
		Commands->Give_Money(killer, killMoney, false);
	}
}


ScriptRegistrant<dp88_customPoints> dp88_customPoints_Registrant(
	"dp88_customPoints",
	"killPoints:float,"
	"damagePoints:float,"
	"repairPoints:float,"
	"killMoney:float,"
	"damageMoney:float,"
	"repairMoney:float"
);