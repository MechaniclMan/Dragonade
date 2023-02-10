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
#include "scripts.h"
#include "obelfix.h"
#include "engine.h"


void Nod_Obelisk_CnC::Created(GameObject* ObeliskObj) {
	if (Commands->Get_Building_Power(ObeliskObj)) {
		// Get weapon position
		Vector3 WeaponPos = Commands->Get_Position(ObeliskObj);
		WeaponPos.Z += 45;

		// Create the Obelisk weapon
		GameObject* WeaponObj = Commands->Create_Object("Nod_Obelisk", WeaponPos);
		if (WeaponObj) {
			WeaponID = Commands->Get_ID(WeaponObj);
			Commands->Attach_Script(WeaponObj, "Obelisk_Weapon_CnC", "");
		}
	}
}

void Nod_Obelisk_CnC::Killed(GameObject* ObeliskObj, GameObject* Killer) {
	// Kill the weapon too
	GameObject* WeaponObj = Commands->Find_Object(WeaponID);
	if (WeaponObj != 0) {
		Commands->Destroy_Object(WeaponObj);
	}
}

void Nod_Obelisk_CnC::Custom(GameObject* ObeliskObj, int type, int Param, GameObject* Sender) {
	if (type == CUSTOM_EVENT_BUILDING_POWER_CHANGED) {
		if (Param != 0) {
			GameObject* WeaponObj = Commands->Find_Object(WeaponID);
			if (!WeaponObj) {
				// Get weapon position
				Vector3 WeaponPos = Commands->Get_Position(ObeliskObj);
				WeaponPos.Z += 45;

				// Create the Obelisk weapon
				WeaponObj = Commands->Create_Object("Nod_Obelisk", WeaponPos);
				if (WeaponObj) {
					WeaponID = Commands->Get_ID(WeaponObj);
					Commands->Attach_Script(WeaponObj, "Obelisk_Weapon_CnC", "");
				}
			}
		} else {
			// Kill the weapon object
			GameObject* WeaponObj = Commands->Find_Object(WeaponID);
			if (WeaponObj) {
				Commands->Destroy_Object(WeaponObj);
			}
		}
	}
}


void Obelisk_Weapon_CnC::Created(GameObject* WeaponObj) {
	// Some settings
	Commands->Set_Is_Rendered(WeaponObj, false); // It's not visible
	Commands->Set_Player_Type(WeaponObj, 0); // We're a Nod Obelisk, GDI will not own an Obelisk
	Commands->Enable_Enemy_Seen(WeaponObj, true); // We want to get notified when we see an enemy
	Commands->Enable_Hibernation(WeaponObj, false); // Not controlled, or whatever?
	Commands->Innate_Enable(WeaponObj); // Dunno :)
	
	Firing = false;
	Charged = false;
	EnemyID = NULL;
	EffectID = NULL;
}

void Obelisk_Weapon_CnC::Destroyed(GameObject* WeaponObj) {
	StopFiring(WeaponObj);
}

bool Obelisk_Weapon_CnC::IsValidEnemy(GameObject* WeaponObj, GameObject* EnemyObj) {
	if (!EnemyObj) return false;
	// TODO: Make switch for obby kills neutral?
	// NEUTRAL:  if (Commands->Get_Player_Type(EnemyObj) == Commands->Get_Player_Type(WeaponObj)) return false;
	// GDI-ONLY: if (Commands->Get_Player_Type(EnemyObj) == 1) return false;
	if (Commands->Get_Player_Type(EnemyObj) != 1) return false;
	if (Commands->Get_Health(EnemyObj) <= 0) return false;
	if (!Commands->Is_Object_Visible(WeaponObj, EnemyObj)) return false;

	Vector3 WeaponObjPos = Commands->Get_Position(WeaponObj);
	Vector3 WeaponObjPosXY = WeaponObjPos;
	WeaponObjPosXY.Z = 0;

	Vector3 EnemyObjPos = Commands->Get_Position(EnemyObj);
	Vector3 EnemyObjPosXY = EnemyObjPos;
	EnemyObjPosXY.Z = 0;

	float Distance = Commands->Get_Distance(WeaponObjPos, EnemyObjPos);
	float DistanceXY = Commands->Get_Distance(WeaponObjPosXY, EnemyObjPosXY);
	
	return DistanceXY > 15 && Distance < 150;
}

void Obelisk_Weapon_CnC::StartFiring(GameObject* WeaponObj) {
	// Start effect
	StartEffect(WeaponObj);
	
	// Start charging
	Commands->Start_Timer(WeaponObj, this, 2, 1);
	Firing = true;
}

void Obelisk_Weapon_CnC::StopFiring(GameObject* WeaponObj) {
	// Stop effect
	StopEffect(WeaponObj);
	
	// Stop firing
	Firing = false;
	Charged = false;
	EnemyID = NULL;
	EffectID = NULL;

	StopFireAt(WeaponObj);
}

void Obelisk_Weapon_CnC::StartEffect(GameObject* WeaponObj) {
	Commands->Create_Sound("Obelisk_Warm_Up", Commands->Get_Position(WeaponObj), WeaponObj);

	GameObject* EffectObj = Commands->Create_Object("Obelisk Effect", Commands->Get_Position(WeaponObj));
	if (EffectObj) {
		EffectID = Commands->Get_ID(EffectObj);
	}
}
void Obelisk_Weapon_CnC::StopEffect(GameObject* WeaponObj) {
	GameObject* EffectObj = Commands->Find_Object(EffectID);
	if (EffectObj) {
		Commands->Destroy_Object(EffectObj);
	}
}

void Obelisk_Weapon_CnC::FireAt(GameObject* WeaponObj, GameObject* EnemyObj)
{
	ActionParamsStruct AttackParams;
	AttackParams.Set_Basic(this, 100, 0);
	AttackParams.Set_Attack(EnemyObj, 150.f, 0, true);
	Commands->Action_Attack(WeaponObj, AttackParams);

	Commands->Start_Timer(WeaponObj, this, 1, 3);
}

void Obelisk_Weapon_CnC::StopFireAt(GameObject* WeaponObj)
{
	Commands->Action_Reset(WeaponObj, 100);
}

void Obelisk_Weapon_CnC::Timer_Expired(GameObject* WeaponObj, int Number) {
	if (Number == 1) {
		// Charged; Set variable and try to fire
		Charged = true;

		GameObject* EnemyObj = Commands->Find_Object(EnemyID);
		if (IsValidEnemy(WeaponObj, EnemyObj)) {
			// Fire at the enemy
			FireAt(WeaponObj, EnemyObj);

			// Check effect in 4 seconds
			Commands->Start_Timer(WeaponObj, this, 4, 1);
		} else {
			// Forget it
			StopFiring(WeaponObj);
		}
	} else if (Number == 2) {
		// Restart the effect
		StopEffect(WeaponObj);
		
		if (IsValidEnemy(WeaponObj, Commands->Find_Object(EnemyID))) {
			// Restart the effect
			StartEffect(WeaponObj);

			// Check again in 3.5 seconds
			Commands->Start_Timer(WeaponObj, this, 4, 1);
		} else {
			// Forget it
			StopFiring(WeaponObj);
		}
	} else if (Number == 3)
	{
		StopFireAt(WeaponObj);
	}
}

void Obelisk_Weapon_CnC::Enemy_Seen(GameObject* WeaponObj, GameObject* EnemyObj) {
	// Check for an living target which is in range
	if (!IsValidEnemy(WeaponObj, EnemyObj)) {
		return;
	}

	// If the previous enemy has gone, set the enemy to the currently detected enemy
	if (!IsValidEnemy(WeaponObj, Commands->Find_Object(EnemyID))) {
		EnemyID = Commands->Get_ID(EnemyObj);
	}

	if (Firing) {
		if (Charged) {
			// Only change the target if the previous target has gone
			if (!IsValidEnemy(WeaponObj, Commands->Find_Object(EnemyID))) {
				FireAt(WeaponObj, EnemyObj);
			}
		}
	} else {
		StartFiring(WeaponObj);
	}
}
void Obelisk_Weapon_CnC::Register_Auto_Save_Variables() {
	Auto_Save_Variable(&EnemyID, 4, 1);
	Auto_Save_Variable(&EffectID, 4, 1);
	Auto_Save_Variable(&Firing, 1, 3);
	Auto_Save_Variable(&Charged, 1, 4);
}

ScriptRegistrant<Nod_Obelisk_CnC> M00_Nod_Obelisk_CnC_Registrant("M00_Nod_Obelisk_CnC", "");
ScriptRegistrant<Nod_Obelisk_CnC> Nod_Obelisk_CnC_Registrant("Nod_Obelisk_CnC", "");
ScriptRegistrant<Obelisk_Weapon_CnC> Obelisk_Weapon_CnC_Registrant("Obelisk_Weapon_CnC", "");

void Nod_Obelisk_CnC_Ground::Created(GameObject* ObeliskObj) {
	if (Commands->Get_Building_Power(ObeliskObj)) {
		// Get weapon position
		Vector3 WeaponPos = Commands->Get_Position(ObeliskObj);
		WeaponPos.Z += 45;

		// Create the Obelisk weapon
		GameObject* WeaponObj = Commands->Create_Object("Nod_Obelisk", WeaponPos);
		if (WeaponObj) {
			WeaponID = Commands->Get_ID(WeaponObj);
			Commands->Attach_Script(WeaponObj, "Obelisk_Weapon_CnC_Ground", "");
		}
	}
}

void Nod_Obelisk_CnC_Ground::Killed(GameObject* ObeliskObj, GameObject* Killer) {
	// Kill the weapon too
	GameObject* WeaponObj = Commands->Find_Object(WeaponID);
	if (WeaponObj != 0) {
		Commands->Destroy_Object(WeaponObj);
	}
}

void Nod_Obelisk_CnC_Ground::Custom(GameObject* ObeliskObj, int type, int Param, GameObject* Sender) {
	if (type == CUSTOM_EVENT_BUILDING_POWER_CHANGED) {
		if (Param != 0) {
			GameObject* WeaponObj = Commands->Find_Object(WeaponID);
			if (!WeaponObj) {
				// Get weapon position
				Vector3 WeaponPos = Commands->Get_Position(ObeliskObj);
				WeaponPos.Z += 45;

				// Create the Obelisk weapon
				WeaponObj = Commands->Create_Object("Nod_Obelisk", WeaponPos);
				if (WeaponObj) {
					WeaponID = Commands->Get_ID(WeaponObj);
					Commands->Attach_Script(WeaponObj, "Obelisk_Weapon_CnC_Ground", "");
				}
			}
		} else {
			// Kill the weapon object
			GameObject* WeaponObj = Commands->Find_Object(WeaponID);
			if (WeaponObj) {
				Commands->Destroy_Object(WeaponObj);
			}
		}
	}
}


void Obelisk_Weapon_CnC_Ground::Created(GameObject* WeaponObj) {
	// Some settings
	Commands->Set_Is_Rendered(WeaponObj, false); // It's not visible
	Commands->Set_Player_Type(WeaponObj, 0); // We're a Nod Obelisk, GDI will not own an Obelisk
	Commands->Enable_Enemy_Seen(WeaponObj, true); // We want to get notified when we see an enemy
	Commands->Enable_Hibernation(WeaponObj, false); // Not controlled, or whatever?
	Commands->Innate_Enable(WeaponObj); // Dunno :)
	
	Firing = false;
	Charged = false;
	EnemyID = NULL;
	EffectID = NULL;
}

void Obelisk_Weapon_CnC_Ground::Destroyed(GameObject* WeaponObj) {
	StopFiring(WeaponObj);
}

bool Obelisk_Weapon_CnC_Ground::IsValidEnemy(GameObject* WeaponObj, GameObject* EnemyObj) {
	if (!EnemyObj) return false;
	// TODO: Make switch for obby kills neutral?
	// NEUTRAL:  if (Commands->Get_Player_Type(EnemyObj) == Commands->Get_Player_Type(WeaponObj)) return false;
	// GDI-ONLY: if (Commands->Get_Player_Type(EnemyObj) == 1) return false;
	if (Commands->Get_Player_Type(EnemyObj) != 1) return false;
	if (Commands->Get_Health(EnemyObj) <= 0) return false;
	if (!Commands->Is_Object_Visible(WeaponObj, EnemyObj)) return false;

	Vector3 WeaponObjPos = Commands->Get_Position(WeaponObj);
	Vector3 WeaponObjPosXY = WeaponObjPos;
	WeaponObjPosXY.Z = 0;

	Vector3 EnemyObjPos = Commands->Get_Position(EnemyObj);
	Vector3 EnemyObjPosXY = EnemyObjPos;
	EnemyObjPosXY.Z = 0;

	float Distance = Commands->Get_Distance(WeaponObjPos, EnemyObjPos);
	float DistanceXY = Commands->Get_Distance(WeaponObjPosXY, EnemyObjPosXY);
	
	return DistanceXY > 15 && Distance < 150;
}

void Obelisk_Weapon_CnC_Ground::StartFiring(GameObject* WeaponObj) {
	// Start effect
	StartEffect(WeaponObj);
	
	// Start charging
	Commands->Start_Timer(WeaponObj, this, 2, 1);
	Firing = true;
}

void Obelisk_Weapon_CnC_Ground::StopFiring(GameObject* WeaponObj) {
	// Stop effect
	StopEffect(WeaponObj);
	
	// Stop firing
	Firing = false;
	Charged = false;
	EnemyID = NULL;
	EffectID = NULL;

	StopFireAt(WeaponObj);
}

void Obelisk_Weapon_CnC_Ground::StartEffect(GameObject* WeaponObj) {
	Commands->Create_Sound("Obelisk_Warm_Up", Commands->Get_Position(WeaponObj), WeaponObj);

	GameObject* EffectObj = Commands->Create_Object("Obelisk Effect", Commands->Get_Position(WeaponObj));
	if (EffectObj) {
		EffectID = Commands->Get_ID(EffectObj);
	}
}
void Obelisk_Weapon_CnC_Ground::StopEffect(GameObject* WeaponObj) {
	GameObject* EffectObj = Commands->Find_Object(EffectID);
	if (EffectObj) {
		Commands->Destroy_Object(EffectObj);
	}
}

void Obelisk_Weapon_CnC_Ground::FireAt(GameObject* WeaponObj, GameObject* EnemyObj)
{
	ActionParamsStruct AttackParams;
	AttackParams.Set_Basic(this, 100, 0);
	AttackParams.Set_Attack(EnemyObj, 150.f, 0, true);
	Commands->Action_Attack(WeaponObj, AttackParams);

	Commands->Start_Timer(WeaponObj, this, 1, 3);
}

void Obelisk_Weapon_CnC_Ground::StopFireAt(GameObject* WeaponObj)
{
	Commands->Action_Reset(WeaponObj, 100);
}

void Obelisk_Weapon_CnC_Ground::Timer_Expired(GameObject* WeaponObj, int Number) {
	if (Number == 1) {
		// Charged; Set variable and try to fire
		Charged = true;

		GameObject* EnemyObj = Commands->Find_Object(EnemyID);
		if (IsValidEnemy(WeaponObj, EnemyObj)) {
			// Fire at the enemy
			FireAt(WeaponObj, EnemyObj);

			// Check effect in 4 seconds
			Commands->Start_Timer(WeaponObj, this, 4, 1);
		} else {
			// Forget it
			StopFiring(WeaponObj);
		}
	} else if (Number == 2) {
		// Restart the effect
		StopEffect(WeaponObj);
		
		if (IsValidEnemy(WeaponObj, Commands->Find_Object(EnemyID))) {
			// Restart the effect
			StartEffect(WeaponObj);

			// Check again in 3.5 seconds
			Commands->Start_Timer(WeaponObj, this, 4, 1);
		} else {
			// Forget it
			StopFiring(WeaponObj);
		}
	} else if (Number == 3)
	{
		StopFireAt(WeaponObj);
	}
}

void Obelisk_Weapon_CnC_Ground::Enemy_Seen(GameObject* WeaponObj, GameObject* EnemyObj) {
	if (Is_VTOL(EnemyObj))
	{
		return;
	}
	// Check for an living target which is in range
	if (!IsValidEnemy(WeaponObj, EnemyObj)) {
		return;
	}

	// If the previous enemy has gone, set the enemy to the currently detected enemy
	if (!IsValidEnemy(WeaponObj, Commands->Find_Object(EnemyID))) {
		EnemyID = Commands->Get_ID(EnemyObj);
	}

	if (Firing) {
		if (Charged) {
			// Only change the target if the previous target has gone
			if (!IsValidEnemy(WeaponObj, Commands->Find_Object(EnemyID))) {
				FireAt(WeaponObj, EnemyObj);
			}
		}
	} else {
		StartFiring(WeaponObj);
	}
}
void Obelisk_Weapon_CnC_Ground::Register_Auto_Save_Variables() {
	Auto_Save_Variable(&EnemyID, 4, 1);
	Auto_Save_Variable(&EffectID, 4, 1);
	Auto_Save_Variable(&Firing, 1, 3);
	Auto_Save_Variable(&Charged, 1, 4);
}

ScriptRegistrant<Nod_Obelisk_CnC_Ground> Nod_Obelisk_CnC_Ground_Registrant("Nod_Obelisk_CnC_Ground", "");
ScriptRegistrant<Obelisk_Weapon_CnC_Ground> Obelisk_Weapon_CnC_Ground_Registrant("Obelisk_Weapon_CnC_Ground", "");
