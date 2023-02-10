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

#include "dp88_ar.h"

/*------------------------
Rocketeer Script
--------------------------*/

class dp88_AR_Rocketeer : public JFW_Key_Hook_Base
{
	void Created( GameObject *obj );
	void Destroyed ( GameObject *obj );
	void Killed( GameObject *obj, GameObject *killer );
	void KeyHook();

	
public:
	/* Constructor - necessary to set variables used by destructor (Otherwise LE crashes) */
	dp88_AR_Rocketeer()
	{
		isFlying = false;
		isDead = false;
	}
private:
	/* Destructor - Technically scripts shouldn't have one but it seems to be the best
	way to handle events that need to trigger when you buy a new character to replace
	the current one... This is only a temporary fix until we implement the new rocketeer
	mechanism anyway. */
	~dp88_AR_Rocketeer();

	bool isFlying;
	bool isDead;		// If isDead the destructor knows we shouldn't try to disable flying mode
	int objectID;		// ID of infantry unit
	time_t lastDeploy;	// Timestamp of last deploy keyhook event (prevent constant spamming)
};




/*------------------------
Mirage Tank Script
--------------------------*/

class dp88_AR_MirageTank : public ScriptImpClass
{
	void Created( GameObject *obj );
	void Damaged( GameObject *obj, GameObject *damager, float amount );
	void Killed( GameObject *obj, GameObject *killer );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Timer_Expired( GameObject *obj, int number );

	void setHidden ( GameObject *obj, bool hidden );

	int pilotID;
	bool hidden;
	Vector3 lastPos;
	int gameControllerId;

	// ID of the mirage disguising us
	int mirageID;
};




/*------------------------
IFV Scripts
--------------------------*/

class dp88_AR_IFV : public JFW_Key_Hook_Base
{
	void Created ( GameObject* obj );
	void Destroyed ( GameObject *obj );
	void Custom ( GameObject* obj, int type, int param, GameObject* sender );
	void Timer_Expired ( GameObject* obj, int number );
	void KeyHook();

	/* General variables. ID of this object, ID of the pilot, current
	veterancy level. */
	int objectID, pilotID, veterancyLevel;

	/* Flags to indicate deployment status and if it's currently deploying */
	bool isDeployed, isDeploying;

	int deployedAnimFrame;					// Animation frame for deployed mode
	int deployedRookieWeaponPowerupId;		// Powerup ID for rookie deployed weapon
	int deployedVeteranWeaponPowerupId;		// Powerup ID for veteran deployed weapon
	int deployedEliteWeaponPowerupId;		// Powerup ID for elite deployed weapon


	char defaultWeapon[64];				// Original default model
	char currentDefaultWeapon[64];		// Current default model
	char currentWeapon[64];				// Current model

	

	// Deploy / undeploy functions
	void deploy();
	void undeploy();

	bool nullWeaponGranted; // test to see if this works around stupid FDS bug
};


// IFV driver script
class dp88_AR_IFVDriver : public ScriptImpClass
{
	void Custom ( GameObject* obj, int type, int param, GameObject* sender );
};





/*------------------------
Tesla Coil Script
--------------------------*/

class dp88_AR_Tesla_Coil : public dp88_AI_Turret
{
	/* -----
	Events
	----- */

	void Created ( GameObject *obj );
	void Damaged ( GameObject *obj, GameObject *damager, float amount );
	void Timer_Expired ( GameObject *obj, int number );

	// Overload the checkPowerState function to return true when sufficient
	// charge level exists to provide power
	virtual bool checkPowerState(GameObject* obj);

private:
	/* -----
	Functions
	----- */

	void setSuperchargedState(GameObject* obj, bool state);

	/* -----
	Variables
	----- */

	int m_chargeLevel;
	bool m_isSupercharged;
	StringClass m_defaultWeapon;
	int m_defaultWeaponRange;
	StringClass m_superchargedWeapon;
	int m_superchargedWeaponRange;

	// Settings
	int m_chargeDuration;
	int m_chargesPowerOn;
	int m_chargesSupercharge;
};


// Dummy script to mark a unit as being able to charge a tesla coil
class dp88_AR_Tesla_Coil_Charger : public ScriptImpClass
{
public:
	dp88_AR_Tesla_Coil_Charger() { m_lastChargeTime = 0; };

	// Used by dp88_AR_Tesla_Coil to track last charge times - used to
	// prevent double charges due to explosion damage
	time_t m_lastChargeTime;
};