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

#include "scripts.h"
#include "engine.h"
#include "LoopedAnimationController.h"



/*------------------------
Random weather script
--------------------------*/

/*!
* \brief Weather Randomizer
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_weather
*
* This script can be used to create randomised weather conditions on a map which change periodically
* during the game. The range of weather conditions that are applied can be controlled via the script
* parameters to ensure inappropriate types of weather are not used (such as snow in the desert).
*
* You should only have one instance of this script active at any given time, generally it should be
* attached to a map controller object such as a Daves Arrow preset.
*
* \param update_interval_min
*   The minimum time, in seconds, between weather update checks. Note that unless probability_change
*   is set to 100 an update check does not necessarily cause new weather conditions to occur
* \param update_interval_max
*   The maximum time, in seconds, between weather update checks. Note that unless probability_change
*   is set to 100 an update check does not necessarily cause new weather conditions to occur
* \param probability_change
*   The percentage probability that new weather conditions will be generated when an update check
*   occurs
* \param fog_start_distance_min
*   The minimum starting fog distance that can be applied
* \param fog_start_distance_max
*   The maximum starting fog distance that can be applied
* \param fog_end_distance_min
*   The minimum end of fog distance that can be applied
* \param fog_end_distance_max
*   The maximum end of fog distance that can be applied
* \param precipitation_probability_snow
*   The percentage probability of snow occuring
* \param precipitation_probability_rain
*   The percentage probability of rain occuring
* \param precipitation_probability_volcanic_ash
*   The percentage probability of volcanic ash occuring
* \param precipitation_density_min
*   The minimum precipitation density that can be applied
* \param precipitation_density_max
*   The maximum precipitation density that can be applied
* \param lighting_probability_normal
*   The percentage probability of normal lightning occuring
* \param lighting_probability_war_blitz
*   The percentage probability of a war blitz occuring
* \param lighting_intensity_min
*   The minimum intensity of lightning that can be applied
* \param lighting_intensity_max
*   The maximum intensity of lightning that can be applied
* \param lighting_start_distance_min
*   The minimum starting distance for lightning that can be applied
* \param lighting_start_distance_max
*   The maximum starting distance for lightning that can be applied
* \param lighting_end_distance_min
*   The minimum end distance for lightning that can be applied
* \param lighting_end_distance_max
*   The maximum end distance for lightning that can be applied
* \param lighting_heading_min
*   The minimum lightning heading that can be applied
* \param lighting_heading_max
*   The maximum lightning heading that can be applied
* \param lighting_distribution_min
*   The minimum lightning distribution that can be applied
* \param lighting_distribution_max
*   The maximum lightning distribution that can be applied
* \param wind_heading_min
*   The minimum wind heading that can be applied
* \param wind_heading_max
*   The maximum wind heading that can be applied
* \param wind_speed_min
*   The minimum wind speed that can be applied
* \param wind_speed_max
*   The maximum wind speed that can be applied
* \param wind_variability_min
*   The minimum wind speed variability that can be applied
* \param wind_variability_max
*   The maximum wind speed variability that can be applied
*
* \note
*   The probability of clear conditions (no rain, snow or volcanic ash) is the 100% minus the
*   combined probabilities of rain, snow and volcanic ash that are configured in the script. For
*   example, if precipitation_probability_snow is 5, precipitation_probability_rain is 15 and
*   precipitation_probability_volcanic_ash is 1 the probability of clear conditions will be 100 -
*   5 - 15 - 1 = 79%
*/
class dp88_randomWeather : public ScriptImpClass
{
	void Created ( GameObject* obj );
	void Timer_Expired ( GameObject* obj, int number );
};





/*------------------------
Indirect targetting script
--------------------------*/

class dp88_indirectTargetting : public JFW_Key_Hook_Base
{
	void Created ( GameObject* obj );
	void Destroyed ( GameObject* obj );
	void Custom ( GameObject* obj, int type, int param, GameObject* sender );
	void KeyHook();

	/* General variables. ID of this object, ID of the pilot, current
	targetting level. */
	int objectID, pilotID, * targettingLevel;
};


class dp88_indirectTargetting_Dec : public JFW_Key_Hook_Base
{
	void Created ( GameObject* obj );
	void Custom ( GameObject* obj, int type, int param, GameObject* sender );
	void KeyHook();

	/* General variables. ID of this object, ID of the pilot, current
	targetting level. */
	int objectID, pilotID, * targettingLevel;
};





/*------------------------
Unit sounds script
--------------------------*/

class dp88_unitSounds : public ScriptImpClass
{
	void Created ( GameObject* obj );
	void Damaged( GameObject *obj, GameObject *damager, float amount );
	void Killed ( GameObject *obj, GameObject *killer );
	void Timer_Expired ( GameObject* obj, int number );
	void Custom ( GameObject* obj, int type, int param, GameObject* sender );

	// Enabled states
	bool damagedEnabled;
	bool heavilyDamagedEnabled;
	bool healedEnabled;
	bool fallingDamageEnabled;

	// Counters
	int healAmount;
};



/*------------------------
Unique Infantry Controller
--------------------------*/

class dp88_uniqueInfantry_controller : public JFW_Object_Created_Hook_Base
{
	void Created ( GameObject* obj );
	void ObjectCreateHook ( GameObject* obj );
	void Destroyed ( GameObject* obj );

	char* infantry_preset;
	int team_id;
	int max_count;
	int percent_chance;
};



/*!
* \brief Damage animation script
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This is a generic damage animation script that allows you to choose the health thresholds at which
* different damage states are applied, rather than being tied into the 25%, 50%, 75% etc permitted
* by the damage bones mechanism.
*
* You can create up to six damage states using this script, with both powered and unpowered versions
* of each state, however if you don't require all six states you can leave the extras unconfigured.
*
* The first state is considered to be the 'healthy' state and is used when the object is above the
* first damage boundary. Each subsequent state is activated when the health of the object drops
* below the specified boundary, which is a percentage of total health. So, for instance, if you set
* boundary 1 to 70 and boundary 2 to 30 your object will animate as follows;
*
* Health 71% - 100%: Healthy animation
* Health 31% - 70%: Damage animation 1
* Health 1% - 0%: Damage animation 2
*
* \param animationName
*   Name of the animation containing all the damage state frames
* \param loopAnimation
*   True if the damage animation should be played in a loop, false otherwise
* \param healthy_startFrame
*   First frame number for the healthy state animation
* \param healthy_endFrame
*   Last frame number for the healthy state animation
* \param healthy_startFrame_lowPower
*   First frame number for the healthy state animation when power is offline, or -1 to use the same
*   value as healthy_startFrame
* \param healthy_endFrame_lowPower
*   Last frame number for the healthy state animation when power is offline, or -1 to use the same
*   value as healthy_endFrame
* \param damageBoundary1
*   Damage percent boundary for the first damage state
* \param damageBoundary1_startFrame
*   First frame number for the first damage state animation
* \param damageBoundary1_endFrame
*   Last frame number for the first damage state animation
* \param damageBoundary1_startFrame_lowPower
*   First frame number for the first damage state animation when power is offline, or -1 to use the
*   same value as damageBoundary1_startFrame
* \param damageBoundary1_endFrame_lowPower
*   Last frame number for the first damage state animation when power is offline, or -1 to use the
*   same value as damageBoundary1_endFrame
* \param damageBoundary2
*   Damage percent boundary for the second damage state, or -1 to disable this state
* \param damageBoundary2_startFrame
*   second frame number for the second damage state animation
* \param damageBoundary2_endFrame
*   Last frame number for the second damage state animation
* \param damageBoundary2_startFrame_lowPower
*   second frame number for the second damage state animation when power is offline, or -1 to use
*   the same value as damageBoundary2_startFrame
* \param damageBoundary2_endFrame_lowPower
*   Last frame number for the second damage state animation when power is offline, or -1 to use the
*   same value as damageBoundary2_endFrame
* \param damageBoundary3
*   Damage percent boundary for the third damage state, or -1 to disable this state
* \param damageBoundary3_startFrame
*   third frame number for the third damage state animation
* \param damageBoundary3_endFrame
*   Last frame number for the third damage state animation
* \param damageBoundary3_startFrame_lowPower
*   third frame number for the third damage state animation when power is offline, or -1 to use
*   the same value as damageBoundary3_startFrame
* \param damageBoundary3_endFrame_lowPower
*   Last frame number for the third damage state animation when power is offline, or -1 to use the
*   same value as damageBoundary3_endFrame
* \param damageBoundary4
*   Damage percent boundary for the fourth damage state, or -1 to disable this state
* \param damageBoundary4_startFrame
*   fourth frame number for the fourth damage state animation
* \param damageBoundary4_endFrame
*   Last frame number for the fourth damage state animation
* \param damageBoundary4_startFrame_lowPower
*   fourth frame number for the fourth damage state animation when power is offline, or -1 to use
*   the same value as damageBoundary4_startFrame
* \param damageBoundary4_endFrame_lowPower
*   Last frame number for the fourth damage state animation when power is offline, or -1 to use the
*   same value as damageBoundary4_endFrame
* \param damageBoundary5
*   Damage percent boundary for the fifth damage state, or -1 to disable this state
* \param damageBoundary5_startFrame
*   fifth frame number for the fifth damage state animation
* \param damageBoundary5_endFrame
*   Last frame number for the fifth damage state animation
* \param damageBoundary5_startFrame_lowPower
*   fifth frame number for the fifth damage state animation when power is offline, or -1 to use
*   the same value as damageBoundary5_startFrame
* \param damageBoundary5_endFrame_lowPower
*   Last frame number for the fifth damage state animation when power is offline, or -1 to use the
*   same value as damageBoundary5_endFrame
*/

class dp88_damageAnimation : public ScriptImpClass
{
  void Created ( GameObject* obj );
  void Detach ( GameObject* obj );
  void Damaged( GameObject *obj, GameObject *damager, float amount );
  void Timer_Expired ( GameObject* obj, int number );

  void SetDamageAnimation ( GameObject* obj );

  int currentDamageLevel;
  int damageLevelBoundaries[6];
  int damageLevelStartFrames[6];
  int damageLevelEndFrames[6];
  int damageLevelLowPowerStartFrames[6];
  int damageLevelLowPowerEndFrames[6];
  bool loopAnimation;
  bool basePowerState;
protected:
  LoopedAnimationController* m_pLoopedAnimCtrl;
public:
  dp88_damageAnimation() : m_pLoopedAnimCtrl(0) {}
};



/*------------------------
Object Annoucements script
--------------------------*/

class dp88_objectAnnouncements : public ScriptImpClass
{
	void Created ( GameObject* obj );
	void Damaged ( GameObject *obj, GameObject *damager, float amount );
	void Killed ( GameObject *obj, GameObject *killer );

	time_t lastAnnouncementTime;
	int minimumAnnounceInterval;

	unsigned long team0_underAttackStringId;
	unsigned long team1_underAttackStringId;
	unsigned long team0_destroyedStringId;
	unsigned long team1_destroyedStringId;

	unsigned long team0_underAttackSoundId;
	unsigned long team1_underAttackSoundId;
	unsigned long team0_destroyedSoundId;
	unsigned long team1_destroyedSoundId;
};





/*------------------------
Chrono Tank Script
--------------------------*/

class dp88_chronoTank : public JFW_Key_Hook_Base
{
	void Created ( GameObject* obj );
	void Killed ( GameObject *obj, GameObject *killer );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Timer_Expired ( GameObject* obj, int number );
	void KeyHook();

	void Shift_Out_Begin(GameObject* obj);
	void Shift_Out_Complete(GameObject* obj);
	void Shift_Out_Cleanup(GameObject* obj);
	void Shift_In_Begin(GameObject* obj);
	void Shift_In_Complete(GameObject* obj);
	void Shift_In_Cleanup(GameObject* obj);

	int state;
	int object_id;		// For KeyHook (obj is not provided)
	int driver_id;
	int shifted_vehicle_object_id;

private:
	// Define states
	enum CT_STATES
	{
		CT_STATE_CHARGING,
		CT_STATE_CHARGED,
		CT_STATE_SHIFTING_OUT,
		CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_1,
		CT_STATE_SHIFTED_OUT_PENDING_CLEANUP_2,
		CT_STATE_SHIFTED_OUT,
		CT_STATE_SHIFTING_IN,
		CT_STATE_SHIFTED_IN_PENDING_CLEANUP_1,
		CT_STATE_SHIFTED_IN_PENDING_CLEANUP_2
	};
};



/*------------------------
Spawn Vehicle Part script
--------------------------*/

class dp88_spawnVehiclePart : public ScriptImpClass
{
	void Created ( GameObject* obj );
	void Timer_Expired( GameObject *obj, int number );
	void Killed ( GameObject* obj, GameObject *killer );
	void Destroyed( GameObject* obj );

	int partObjectId;
};



/*------------------------
Custom Points script
--------------------------*/

class dp88_customPoints : public ScriptImpClass
{
	void Created ( GameObject* obj );
	void Damaged ( GameObject *obj, GameObject *damager, float amount );
	void Killed ( GameObject* obj, GameObject *killer );

	float killPoints;
	float damagePoints;
	float repairPoints;
	float killMoney;
	float damageMoney;
	float repairMoney;
};