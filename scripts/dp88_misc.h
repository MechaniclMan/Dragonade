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



/*!
* \brief Custom Unit Points & Credits
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts
*
* This script provides finer control over the points and credits awarded for damaging or repairing
* an object, as opposed to the values in an LevelEdit preset which award equal amounts of score and
* credits.
*
* \note
*   You should set the points value of the object to 0 in the LevelEdit preset settings or both
*   values will be applied
*
* \param killPoints
*   Number of points to award for killing the object
* \param damagePoints
*   Number of points to award per hitpoint of damage done to the object
* \param repairPoints
*   Number of points to award per hitpoint repaired on object
* \param killMoney
*   Amount of credits to award for killing the object
* \param damageMoney
*   Amount of credits to award per hitpoint of damage done to the object
* \param repairMoney
*   Amount of credits to award per hitpoint repaired on object
*/
class dp88_customPoints : public ScriptImpClass
{
protected:
  void Created ( GameObject* obj );
  void Damaged ( GameObject *obj, GameObject *damager, float amount );
  void Killed ( GameObject* obj, GameObject *killer );

  void GivePointsAndMoney ( GameObject* obj, float points, float money );

  /*! \name Cached Script Parameters */
  /*! @{ */
  float m_killPoints;
  float m_damagePoints;
  float m_repairPoints;
  float m_killMoney;
  float m_damageMoney;
  float m_repairMoney;
  /*! @} */
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Conquest Mode - Controller
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_conquest
*
* This script controls conquest mode and must be placed on exactly one object on any map where you
* want to enable conquest mode, such as a Daves Arrow. It stores the points totals for each team
* and continually sends messages to the server with the current scores.
*
* The controller allows you to choose from one of two scoring systems, which basically work the same
* way but in different directions. The default scoring system is points accumulation mode, where
* each team starts with zero points and the points they earn are added to their total. The first
* team to reach the target score wins the game.
*
* Alternatively you can enable points deducation mode, where each team starts with a set number of
* points and the points they score are removed from the other teams total. The first team to have
* their score reduced to zero loses the game.
*
* \pre
*   Requires the base destruction victory mode to be enabled in the server configuration otherwise
*   it will be unable to end the game when a team achieves their victory condition
*
* \param ScoringMode
*    Specifies which scoring system to use. 0 = default, 1 = deduction mode
* \param TargetScoreTeam0
*    This is the number of points team 0 (Nod / Soviet) need to win the game or, if deduction mode
*    is enabled, the number of points team 0 starts with
* \param TargetScoreTeam1
*    This is the number of points team 1 (GDI / Allies) need to win the game or, if deduction mode
*    is enabled, the number of points team 1 starts with
* \param IntroString
*    The name of an entry in the strings database containing a message to be displayed to players at
*    the start of the map which can be used to explain how to win. It will also be sent to players
*    when they join mid game. See the note below for a list of macros you can use in this string
* \param ScoreInterval
*    Specifies how often, in seconds, the controller should send a message to the server with the
*    current scores. Set this to 0 if you do not want the scores to be shown.
* \param ScoreString
*    The name of strings database entry containing the message to be sent to the server with the
*    current scores. If this is blank a generic message will be shown. See the note below for a list
*    of macros you can use in this string
* \param ScoreStringTeam0
*    Optional. The name of a strings database entry containing an alternative score string to be
*    used if team 0 are currently winning. If this is blank the value of ScoreString will be used
*    instead. See the note below for a list of macros you can use in this string
* \param ScoreStringTeam1
*    Optional. The name of a strings database entry containing an alternative score string to be
*    used if team 1 are currently winning. If this is blank the value of ScoreString will be used
*    instead. See the note below for a list of macros you can use in this string
*
* \note
*   The following macros can be used in strings database entries;\n
*   \n
*   <table>
*    <tr><th>Macro</th>                     <th>Value</th></tr>
*    <tr><td>%%TEAM0%</td>                  <td>Translated name of team 0</td></tr>
*    <tr><td>%%TEAM1%</td>                  <td>Translated name of team 1</td></tr>
*    <tr><td>%%WINNINGTEAM%</td>            <td>Translated name of winning team, or 'Neither' if the points are level</td></tr>
*    <tr><td>%%LOSINGTEAM%</td>             <td>Translated name of losing team, or 'Neither' if the points are level</td></tr>
*    <tr><td>%%TEAM0TARGET%</td>            <td>Target score for team 0 (or starting score in deduction mode)</td></tr>
*    <tr><td>%%TEAM1TARGET%</td>            <td>Target score for team 1 (or starting score in deduction mode)</td></tr>
*    <tr><td>%%TEAM0SCORE%</td>             <td>Current score for team 0</td></tr>
*    <tr><td>%%TEAM1SCORE%</td>             <td>Current score for team 1</td></tr>
*    <tr><td>%%TEAM0SCORENEEDED%</td>       <td>Points required before team 0 will win</td></tr>
*    <tr><td>%%TEAM1SCORENEEDED%</td>       <td>Points required before team 1 will win</td></tr>
*   </table>
*/
class dp88_conquestController : public ScriptImpClass
{
  void Created ( GameObject* pSelf );
  void Timer_Expired ( GameObject* pSelf, int number );
  void Custom ( GameObject* pSelf, int type, int param, GameObject* pSender );

protected:
  /*!
  * \name Scoring Modes
  * Scoring mode definitions, see the class description for an explaination of these modes
  */
  ///@{
  static const unsigned char MODE_DEFAULT   = 0;  /*!< Default, points accumulation mode */
  static const unsigned char MODE_DEDUCTION = 1;  /*!< Points deduction mode */
  ///@}

  unsigned char m_scoringMode;  /*!< The scoring mode being used */
  int m_points[2];              /*!< Current points total for each team */
  int m_targets[2];             /*!< Target for each team, or starting points in deduction mode */

};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Conquest Mode - Control Zone
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_conquest
*
* Attach this script to an object to create a conquest control zone around or near that object. A
* control zone can be taken over by a team if one or more players from that team are standing in the
* zone with no enemy players. A zone can grant conquest points to a team upon it's initial capture
* and continually for as long as they control the zone. Requires a controller object to be placed
* on the map with the dp88_conquestController script attached to it before this script can work.
*
* If the zone is currently neutral it takes CaptureTime seconds for one player to take control of
* the zone for their team. If the zone is currently owned by the enemy team it takes CaptureTime
* seconds for one player to make the zone neutral, after which they can take control of the zone as
* normal.
*
* Additional units can contribute to the capturing process to reduce the time required to take
* control of the zone, up to MultiCaptureCap. Any additional players in the zone beyond the maximum
* cap will not provide any additional boost to the capture speed. Note that the cap is the total
* number of units, including the first unit to enter the zone, so a cap of 1 will disable the
* ability to boost capture speeds.
*
* Each additional player within the cap, beyond the first player, will reduce the capture time by
* (CaptureTime*MultiCaptureMultiplier) seconds, thus with a multipler of 0.5 each additional player
* will reduce the capture time by half of the original time.
*
* If AllowMajorityCapture is enabled the zone can be captured whilst there are enemy players in the
* zone by the team which has the most players in the zone. If combined with a MultiCaptureCap >1
* each enemy player in the zone nullifies the effect of one of the majority teams players, reducing
* the capture speed boost. Thus, with 5 players on the majority team and 3 players on the minority
* team in the zone the capture speed will be 50% of the original time, rather than the 20% it would
* be if there were no enemy team players in the zone (assuming a MultiCaptureMultiplier of 1.0).
*
* An animation can be played on the object upon which the script is attached to indicate the current
* state of the control zone, which consists of a single frame for each of the three fixed states
* (team0 controlled, neutral, team1 controlled) and optionally a series of intermediate transition
* frames which will be used to indicate how close the zone is to being captured or returned to its
* neutral state. You could attach other animated objects to the animation using bones if a continual
* animation is required, such as a waving flag that moves up and down a flagpole during capture.
*
* If the zone is partially captured and there is no longer anybody in the zone it will slowly revert
* to its previous state, losing 1 second of capture progress per second.
*
* \param ZoneSize
*   The size of the capture zone to be created
* \param ZoneOffset
*   Optional. The offset from the object location the zone should be centered upon
* \param ZonePreset
*   The preset to use for the capture zone, this does not require any scripts to be attached
* \param CapturePoints
*   The number of points to give to a team when they take control of the zone from neutral. If the
*   control point spawns as non-neutral no points will be given to the initial team
* \param CapturePointsFirstTeamOnly
*   Determines if CapturePoints are every time a team takes control of a zone or if they are only
*   given the first time the zone is taken over from neutral. 1 to enable, 0 to disable
* \param TickPoints
*   Conquest points to give to the team which currently owns the zone every 'TickInterval' seconds
* \param TickInterval
*   Tick interval, in seconds, at which to give conquest points to the zones current owner
* \param CaptureTime
*   Time, in seconds, required to take control of the zone from neutral or restore the zone to the
*   neutral state from enemy control
* \param MultiCaptureCap
*   The maximum number of units that can contribute to speeding up the capture process. If set to 1
*   or 0 multi capture will be disabled. See the class description for more information
* \param MultiCaptureMultiplier
*   The multipler to be applied to the capture speed by each additional unit, up to the specified
*   cap, where 1.0 gives a 100% boost to the capture speed and 0.5 gives a 50% boost. See the class
*   description for more information 
* \param AllowMajorityCapture
*   Allow a zone to be captured by the team with more players in the zone, even if enemy players are
*   also in the zone. See the class description for more information. 1 to enable, 0 to disable
* \param CaptureAnim
*   Optional. The name of an animation to play on the object to indicate capture progress. This
*   animation should be in the following order: (team0 controlled) -> (team0<->neutral transition)
*   -> (neutral) -> (neutral<->team1 transition) -> (team1 controlled). The controlled and neutral
*   states should be a single frame. The transitions are optional, but if present must contain the
*   same number of frames
* \param CaptureAnimTransitionFrames
*   The number of transition frames between the controlled and neutral states in the CaptureAnim, or
*   zero if there are no transition frames
* \param CapturingString
*   Optional. The name of a strings database entry containing a message to be displayed to the team
*   which currently controls the zone when the enemy team are capturing it. See the note below for a
*   list of macros you can use in this string
* \param NeutralString
*   Optional. The name of a strings database entry containing a message to be displayed to all
*   players when a capture zone becomes neutral. See the note below for a list of macros you can use
*   in this string
* \param CapturedString
*   Optional. The name of a strings database entry containing a message to be displayed to all
*   players when a team takes control of a capture zone. See the note below for a list of macros you
*   can use in this string
* \param AIObjectivePriority
*   Optional. When used in conjunction with danpaul88's custom AI scripts this allows this zone to
*   be marked as an AI objective for both teams, either an offensive objective (for teams that do
*   not currently control it) or a defensive objective (for the team that does control the zone).
*   The objective distance will be set to three quarters of the zones smallest dimension and will
*   be suitable for infantry, light and heavy vehicles. See \ref dp88_AI_Objective for more info.
*
* \note
*   The following macros can be used in strings database entries;\n
*   \n
*   <table>
*    <tr><th>Macro</th>                     <th>Value</th></tr>
*    <tr><td>%%OWNINGTEAM%</td>             <td>Translated name of the team which controls (or controlled) the zone</td></tr>
*    <tr><td>%%OTHERTEAM%</td>              <td>Translated name of the other team</td></tr>
*    <tr><td>%%CAPTUREPOINTS%</td>          <td>The value of CapturePoints</td></tr>
*    <tr><td>%%TICKPOINTS%</td>             <td>The value of TickPoints</td></tr>
*    <tr><td>%%TICKINTERVAL%</td>           <td>The value of TickInterval</td></tr>
*   </table>
*/
class dp88_conquestControlZone : public ScriptImpClass
{
public:
  void Created( GameObject* pObj );
  void Timer_Expired( GameObject* pObj, int number );

protected:
  void IncrementCaptureProgress( GameObject* pObj, int team, int nPlayers );
  void UpdateAnimationFrame( GameObject* pObj );

  int m_controllerID;       /*!< ID of the GameObject with the controller script attached */
  int m_zoneID;             /*!< ID of the control zone GameObject */
  float m_captureState;     /*!< Current capture progress. 0 = neutral, negative = team0, positive = team1 */

  bool m_bTickRunning;      /*!< True if the tick timer is currently running, false otherwise */

  /*! \name Cached Script Parameters */
  /*! @{ */
  int m_captureTime;
  int m_multiCaptureCap;
  float m_multiCaptureMultiplier;
  bool m_bAllowMajorityCapture;
  int m_nAnimTransitionFrames;
  /*! @} */
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Radar Invisibility
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_radar
*
* Attach this script to an object to make it invisible to the radar - when the script is detached it
* will restore the original radar state. If this script is attached to a vehicle it will also make
* passengers radar-invisible until they leave the vehicle again.
*/
class dp88_radarInvisibility : public ScriptImpClass
{
public:
  /*! Constructor to prevent crashes in LE due to uninitialised variables */
  dp88_radarInvisibility() : m_pPassengerIds(), m_pPassengerRadarModes(0) {};

protected:
  void Created ( GameObject* pObj );
  void Custom ( GameObject* pObj, int type, int param, GameObject* pSender );
  void Detach ( GameObject* pObj );

  void HidePassengerBlip ( class VehicleGameObj* pVehicle, class SoldierGameObj* pSoldier );
  void RestorePassengerBlip ( class SoldierGameObj* pSoldier );

  int m_originalRadarMode;      //!< The original radar mode of this object
  int* m_pPassengerIds;         //!< For vehicles, the ID of each passenger
  int* m_pPassengerRadarModes;  //!< For vehicles, the original radar mode of each passenger
  int m_nSeats;                 //!< For vehicles, cache the number of seats (for Detach() to use)
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Turret Rotation Sound Effect
* \author Daniel Paul (danpaul88@yahoo.co.uk)
* \ingroup scripts_sound
*
* This script plays a 3d sound at a vehicles turret bone when that bone is being rotated and stops
* the sound when the rotation stops. The sound will be looped whilst the turret is being rotated.
*
* \note
*   This script uses the difference between the vehicle rotation and the turret bone rotation to
*   determine if the turret is rotating. This means simply aiming in one direction and spinning on
*   the spot will cause the sound to be played, since the turret is rotating relative to the vehicle
*
* \param Sound_Preset
*   The name of a 3D sound preset to be played whilst the turret is rotating
* \param Min_Differential_Rad
*   The minimum difference in the turret rotation, in radians, to be considered as "rotating", this
*   helps to filter out tiny movements caused by driving along uneven terrain.
*/
class dp88_turretSound : public ScriptImpClass
{
protected:
  void Created ( GameObject* pObj );
  void Timer_Expired ( GameObject* pObj, int number );
  void Custom ( GameObject* pObj, int type, int param, GameObject* pSender );

  float Get_Turret_Facing ( class RenderObjClass* pRenderObj );
  void Play_Sound ( GameObject* pObj );
  void Stop_Sound ( GameObject* pObj );

  float m_lastFacing;
  int m_nSoundId;

  /*! \name Cached Script Parameters */
  /*! @{ */
  float m_nMinDifferential;
  /*! @} */
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Yet Another Teleport Script
* \author Daniel Paul
* \ingroup scripts
*
* The teleport script to end all teleport scripts, this is designed to cover almost any scenario for
* teleporting units to a location or another object. It can be attached to either a script zone, a
* pokeable object or a powerup for maximum flexibility.
*
* \param ObjectID
*   The ID of an object to teleport to, or 0 to teleport to a location instead. If a target object
*   with the specified ID is not found the script will not teleport the unit.
* \param Bone
*   The bone on the object to teleport to, or 0 to teleport to the objects origin. Not applicable
*   if ObjectID is set to 0.
* \param Offset
*   If teleporting to an object (or one of it's bones), apply this offset to the object location.
*   Otherwise this is used as the absolute position to teleport to (ie: an offset from 0,0,0)
* \param Teleport_Infantry
*   Set this to 1 if infantry should be teleported by this script, 0 if it should ignore infantry
* \param Teleport_Vehicles
*   Set this to 1 if vehicles should be teleported by this script, 0 if it should ignore vehicles
*
* \note
*   If you only want to teleport players and not AI controlled units use a script zone and tick the
*   <i>CheckStarsOnly</i> box in LevelEdit.
*/
class dp88_teleport : public ScriptImpClass
{
protected:
  void Created ( GameObject* pObj );
  void Entered ( GameObject* pObj, GameObject* pEnterer );
  void Custom ( GameObject* pObj, int type, int param, GameObject* pSender );
  void Poked ( GameObject* pObj, GameObject* pPoker );

  /*! \name Cached Script Parameters */
  /*! @{ */
  int m_nObjectID;
  bool m_bUseBone;
  Vector3 m_offset;

  bool m_bTeleportInfantry;
  bool m_bTeleportVehicles;
  /*! @} */
};