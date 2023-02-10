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


// Define building child types
#define BUILDING_CHILD_TYPE_GENERIC				0
#define BUILDING_CHILD_TYPE_MINOR_WEAKPOINT		1
#define BUILDING_CHILD_TYPE_MAJOR_WEAKPOINT		2
#define BUILDING_CHILD_TYPE_CAPTUREPOINT		3


/*------------------------
Building Controller Script
--------------------------*/

class dp88_buildingScripts_buildingController : public ScriptImpClass
{
	// Struct to contain data for building children
	struct ChildData
	{
		int objectId;
		int type;
		bool destroyed;

		ChildData* operator= (ChildData* cd )
		{
			objectId = cd->objectId;
			type = cd->type;
			destroyed = false;
		}
	};
	
	void Created( GameObject *obj );
	void Destroyed( GameObject *obj );
	void Killed( GameObject *obj, GameObject *killer );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );

	// Array of data for child objects belonging to this building controller
	int numChildren;
	ChildData* children;
	int findChild ( int objectId );

	// Is the building online?
	bool buildingOnline;

	// String IDs
	unsigned long team0_buildingOfflineStringId;
	unsigned long team1_buildingOfflineStringId;
	unsigned long team0_buildingOnlineStringId;
	unsigned long team1_buildingOnlineStringId;

	// String sound IDs
	unsigned long team0_buildingOfflineSoundId;
	unsigned long team1_buildingOfflineSoundId;
	unsigned long team0_buildingOnlineSoundId;
	unsigned long team1_buildingOnlineSoundId;
};




/*------------------------
Building Repair Node
--------------------------*/

class dp88_buildingScripts_repairNode : public ScriptImpClass
{
	void Created( GameObject *obj );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );

	// ID of the building controller we belong to
	int parentId;
};




/*------------------------
Weakpoint - Destroy Building Script
--------------------------*/

class dp88_buildingScripts_weakpoint_destroyBuilding : public ScriptImpClass
{
	void Created( GameObject *obj );
	void Killed( GameObject *obj, GameObject *killer );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );

	// ID of the building controller we belong to
	int parentId;
};

class dp88_buildingScripts_PT_Script : public ScriptImpClass
{
	void Created( GameObject *obj );
  void Detach ( GameObject *obj );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Timer_Expired ( GameObject* obj, int number );
	void SetDamageAnimation ( GameObject* obj );

	// ID of the building controller we belong to
	int parentId;
	bool dead;
	bool basePowerState;

protected:
  LoopedAnimationController* m_pLoopedAnimCtrl;
public:
  dp88_buildingScripts_PT_Script() : m_pLoopedAnimCtrl(0) {}
};



/*------------------------
Base Class For Generic Minor Weakpoints

All of the minor weakpoints function in basically the same way with different effects applied
to the building they are attached to, so this base class provides the general functionality
and defines two abstract methods which the inheriting classes will implement to apply and
reverse their destroyed effect.
--------------------------*/

class MinorWeakpointScriptClass : public ScriptImpClass
{
	void Created( GameObject *obj );
  void Detach ( GameObject *obj );
	void Damaged ( GameObject *obj, GameObject *damager, float amount );
	void Killed( GameObject *obj, GameObject *killer );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );

	// ID of the building controller we belong to
	int parentId;

	// We use this to remember when we are in the destroyed state, so we know
	// that when we next reach 100% health we need to trigger the repaired event
	bool isDestroyed;

	// Abstract functions
	virtual void applyEffect ( GameObject *obj, GameObject *parent ) = 0;
	virtual void reverseEffect ( GameObject *obj, GameObject *parent ) = 0;
protected:
  LoopedAnimationController* m_pLoopedAnimCtrl;
public:
  MinorWeakpointScriptClass() : m_pLoopedAnimCtrl(0) {}
};




/*------------------------
Weakpoint - Disable Soldier Purchase
--------------------------*/

class dp88_buildingScripts_weakpoint_disableSoldierPurchase : public MinorWeakpointScriptClass
{
	void applyEffect ( GameObject *obj, GameObject *parent );
	void reverseEffect ( GameObject *obj, GameObject *parent );
};




/*------------------------
Weakpoint - Disable Vehicle Purchase
--------------------------*/

class dp88_buildingScripts_weakpoint_disableVehiclePurchase : public MinorWeakpointScriptClass
{
	void applyEffect ( GameObject *obj, GameObject *parent );
	void reverseEffect ( GameObject *obj, GameObject *parent );
};


/*------------------------
Weakpoint - Disable Air Vehicle Purchase
--------------------------*/

class dp88_buildingScripts_weakpoint_disableAirVehiclePurchase : public MinorWeakpointScriptClass
{
	void applyEffect ( GameObject *obj, GameObject *parent );
	void reverseEffect ( GameObject *obj, GameObject *parent );
};

/*------------------------
Weakpoint - Disable Naval Vehicle Purchase
--------------------------*/

class dp88_buildingScripts_weakpoint_disableNavalVehiclePurchase : public MinorWeakpointScriptClass
{
	void applyEffect ( GameObject *obj, GameObject *parent );
	void reverseEffect ( GameObject *obj, GameObject *parent );
};

/*------------------------
Weakpoint - Disable Base Power
--------------------------*/

class dp88_buildingScripts_weakpoint_disableBasePower : public MinorWeakpointScriptClass
{
	void applyEffect ( GameObject *obj, GameObject *parent );
	void reverseEffect ( GameObject *obj, GameObject *parent );
};




/*------------------------
Weakpoint - Disable Radar
--------------------------*/

class dp88_buildingScripts_weakpoint_disableRadar : public MinorWeakpointScriptClass
{
	void applyEffect ( GameObject *obj, GameObject *parent );
	void reverseEffect ( GameObject *obj, GameObject *parent );
};


/*------------------------
Weakpoint - Disable Refinery
--------------------------*/

class dp88_buildingScripts_weakpoint_disableRefinery : public MinorWeakpointScriptClass
{
	void applyEffect ( GameObject *obj, GameObject *parent );
	void reverseEffect ( GameObject *obj, GameObject *parent );
};


/*------------------------
Base Class For Building Functions

All of the building function scripts are derived from this class to add
additional events which are triggered by the building controller.
--------------------------*/

class dp88_BuildingEffectScript : public ScriptImpClass
{
	void Custom ( GameObject *obj, int type, int param, GameObject *sender );


	/* ---------------------------------
	* Virtual stub functions which can be overloaded by events to perform
	* any actions they need to perform
	* ---------------------------------*/

	// Called when the building is taken offline
	virtual void OnBuildingOffline( GameObject* obj ) {};

	// Called when the building is brought back online
	virtual void OnBuildingOnline ( GameObject* obj ) {};

	// Called when the building is captured
	virtual void OnBuildingCaptured ( GameObject* obj, int team ) {};

	// Called by Custom() if it has not handled the message
	virtual void OnCustom ( GameObject *obj, int type, int param, GameObject *sender ) {};
};


// Money trickle function - gives the owning team X credits per second whilst alive
class dp88_buildingScripts_functionMoneyTrickle : public dp88_BuildingEffectScript
{
	void Created ( GameObject* obj );
	void Timer_Expired ( GameObject* obj, int number );
	void OnBuildingCaptured ( GameObject* obj, int team );

private:
	bool isTimerRunning;
};


class dp88_buildingScripts_functionMoneyGrant : public dp88_BuildingEffectScript
{
	void Created ( GameObject* obj );
	void OnBuildingCaptured ( GameObject* obj, int team );
};

// Spawn team zone - spawns a script zone depending on which team currently owns
// the building
class dp88_buildingScripts_functionSpawnTeamZone : public dp88_BuildingEffectScript
{
	void Created ( GameObject* obj );
	void OnBuildingOffline( GameObject* obj );
	void OnBuildingOnline ( GameObject* obj );
	void OnBuildingCaptured ( GameObject* obj, int team );
	void Destroyed ( GameObject* obj );

private:
	// Functions to create and destroy the script zone
	void createZone(GameObject* obj);
	void destroyZone(GameObject* obj);

	// ID of the zone we have created
	int zoneId;
};




/*------------------------
Building Script Zone
--------------------------*/

class dp88_buildingScripts_scriptZone : public ScriptImpClass
{
	void Created( GameObject *obj );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );

	// Functions to create and destroy the script zone
	void createZone(GameObject* obj);
	void destroyZone(GameObject* obj);

	// ID of the building controller we belong to
	int parentId;

	// ID of the zone we have created
	int zoneId;
};




/*------------------------
Building Capture Point
--------------------------*/

class dp88_buildingScripts_capturePoint : public ScriptImpClass
{
	void Created( GameObject *obj );
	void Damaged ( GameObject *obj, GameObject *damager, float amount );
	void Custom( GameObject *obj, int type, int param, GameObject *sender );
	void Timer_Expired( GameObject *obj, int number );

	int parentId;
	int currentTeam;
};



/*------------------------
Spawn Building Part script
--------------------------*/

class dp88_buildingScripts_spawnPart : public dp88_BuildingEffectScript
{
	void Created ( GameObject* obj );
	void Timer_Expired( GameObject *obj, int number );
	void OnBuildingCaptured ( GameObject* obj, int team );
	void Destroyed ( GameObject* obj );

	int partObjectId;
};