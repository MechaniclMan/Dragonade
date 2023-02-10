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


/* Script to handle veterancy on any type of object, either player or AI controlled */
class dp88_veterancyUnit : public JFW_Key_Hook_Base
{
  public:
    void Created ( GameObject *obj );
    void Damaged( GameObject *obj, GameObject *damager, float amount );
    void Killed ( GameObject *obj, GameObject *killer );
    void Destroyed ( GameObject *obj );
    void Custom ( GameObject* obj, int type, int param, GameObject* sender );
    void Timer_Expired( GameObject *obj, int number );
    void KeyHook();

    // Recieve veterancy points
    void recieveVeterancyPoints ( float points );

  private:
    /****************
    Variables
    *****************/

    // Our data
    int objectId;
    int currentLevel;
    float infantryVeterancyPoints, vehicleVeterancyPoints;
    int infantryVeteranRequirement, infantryEliteRequirement;
    int vehicleVeteranRequirement, vehicleEliteRequirement;
    int chevronObjId, promotionChevronObjId;

    /* Flags to mark whether we have upgraded weapons for veteran or rookie levels (saves lots of
    string comparisions and preset validation later on) */
    bool hasVeteranWeaponPowerup, hasEliteWeaponPowerup;

    /* Original values for weapons / skin / armour type (req. for vehicles only) */
    char rookieWeapon[128], rookieSkinType[128], rookieShieldType[128];

    // ID for vehicle pilot (vehicles only)
    int pilotId;

    // Marker to indicate if we are dead or not, sometimes the promotion
    // functions can get called after the unit is dead, so we need a quick
    // and easy way to check this and abort promotion if we are dead
    bool dead;


    // Static arrays of pointers to all veterancy units
    static dp88_veterancyUnit* playerData[128];
    static dp88_veterancyUnit* AIUnitData[256];

    /****************
    Functions
    *****************/

    /*! Grant x veterancy points to the specified unit */
    void grantVeterancyPoints ( GameObject* obj, float points );

    /*! Get a pointer to the dp88_veterancyUnit class object for the specified GameObject */
    dp88_veterancyUnit* getVeterancyData ( GameObject* obj );

    /*! Handle promotion events */
    void promoteToVeteran();
    void promoteToElite();

    /*! Handle demotion event - Mainly for vehicles when their drivers get out */
    void demoteToRookie();

    /*! Chevron scripts */
    void createChevrons();
    void clearChevrons();
};

// -------------------------------------------------------------------------------------------------

/* Script for a crate which grants veterancy points to the collector */
class dp88_veterancyCrate : public ScriptImpClass
{
  void Custom ( GameObject *obj, int type, int param, GameObject *sender );
};

// -------------------------------------------------------------------------------------------------

/* Script to grant veterancy points to whatever it is attached to */
class dp88_veterancyGrantPoints : public ScriptImpClass
{
  void Created ( GameObject *obj );
  void Custom ( GameObject *obj, int type, int param, GameObject *sender );
};

// -------------------------------------------------------------------------------------------------

// Script to link veterancy points of two objects together
class dp88_linkVetPoints : public ScriptImpClass
{
  void Created ( GameObject *obj );
  void Timer_Expired ( GameObject *obj, int number );
  void equalisePoints( GameObject* obj );

  int parentObjID;
  float lastInfantryPoints;
  float lastVehiclePoints;
};

// -------------------------------------------------------------------------------------------------

// Script to grant extra health / armour upon promotion
class dp88_veterancyPromotionHealthArmourIncrease : public ScriptImpClass
{
  void Created( GameObject *obj );
  void Custom( GameObject *obj, int type, int param, GameObject *sender );

private:
  int m_veterancyLevel;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Veterancy - Regeneration
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This is a companion script for dp88_veterancyUnit which grants health regeneration to a unit based
* upon it's current promotion level (rookie, veteran, elite). It can optionally also repair their
* armour, but only once their health has been fully restored and at the same speed.
*
* \param rookie_regenAmount
*   Number of hitpoints to heal, per second, whilst this unit is at rookie level
* \param rookie_repairArmour
*   Whether to repair armour as well as health, whilst this unit is at rookie level. 1 to enable
*   armour repairs, 0 to disable and only restore health
* \param veteran_regenAmount
*   Number of hitpoints to heal, per second, whilst this unit is at veteran level
* \param veteran_repairArmour
*   Whether to repair armour as well as health, whilst this unit is at veteran level. 1 to enable
*   armour repairs, 0 to disable and only restore health
* \param elite_regenAmount
*   Number of hitpoints to heal, per second, whilst this unit is at elite level
* \param elite_repairArmour
*   Whether to repair armour as well as health, whilst this unit is at elite level. 1 to enable
*   armour repairs, 0 to disable and only restore health
*/
class dp88_veterancyRegeneration : public ScriptImpClass
{
  void Created( GameObject *obj );
  void Timer_Expired( GameObject *obj, int number );
  void Custom( GameObject *obj, int type, int param, GameObject *sender );

private:
  int veterancyLevel;

  int m_regenAmount;
  bool m_bRepairArmour;
};

// -------------------------------------------------------------------------------------------------

/*!
* \brief Veterancy - Grant Weapon
* \author Daniel Paul (danpaul88@yahoo.co.uk)
*
* This is a companion script to dp88_veterancyUnit which grants an additional weapon to a unit upon
* spawning and replaces the weapon with a (presumably) more powerful version when the unit gets
* promoted. Alternatively it can grant an additional weapon to a unit when it reaches veteran or
* elite level, without granting anything to a rookie unit.
*
* Similar to the m00_GrantPowerup_Created stock script, with the additional veterancy logic added.
*
* \note
*   Whilst this script is designed to be used for granting additional weapons there's no technical
*   reason it can't also be used to grant other powerup types. If non-weapon powerups are granted
*   their effects will not be removed when the next veterancy level is reached.
*
* \param powerupPreset
*   The name of a powerup preset to grant to the unit when it is spawned at rookie level
* \param powerupPreset_veteran
*   The name of a powerup preset to grant to the unit when it is promoted to veteran level
* \param powerupPreset_elite
*   The name of a powerup preset to grant to the unit when it is promoted to elite level
*/
class dp88_veterancyGrantPowerup : public ScriptImpClass
{
  char weaponName[64];
  char oldWeapon[64];

  void Created( GameObject *obj );
  void Custom( GameObject *obj, int type, int param, GameObject *sender );

private:
  int veterancyLevel;
};