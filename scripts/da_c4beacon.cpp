/*	Renegade Scripts.dll
    Dragonade C4 and Beacon Manager
	Copyright 2013 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "General.h"
#include "engine.h"
#include "engine_da.h"
#include "da.h"
#include "da_c4beacon.h"
#include "da_chatcommand.h"
#include "da_translation.h"
#include "da_log.h"
#include "da_settings.h"
#include "GameObjManager.h"
#include "weaponmgr.h"

void DAC4BeaconManager::Init() {
	static DAC4BeaconManager Instance;
	Instance.Register_Event(DAEvent::SETTINGSLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::BEACONDEPLOY,INT_MAX);
	Instance.Register_Event(DAEvent::BEACONDETONATE,INT_MAX);
	Instance.Register_Event(DAEvent::C4DETONATE,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPPURCHASEREQUEST,INT_MIN);
	Instance.Register_Object_Event(DAObjectEvent::POKE,DAObjectEvent::C4 | DAObjectEvent::BEACON,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::C4 | DAObjectEvent::BEACON,INT_MAX);
}

void DAC4BeaconManager::Settings_Loaded_Event() {
	Console_InputF("mlimit %d",DASettingsManager::Get_Int("C4Limit",Get_Mine_Limit()));
	if (DASettingsManager::Get_Bool("BlockFakeBeacons",false)) {
		Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::BEACON,INT_MAX);
	}
	else {
		Unregister_Object_Event(DAObjectEvent::CREATED);
	}
}

void DAC4BeaconManager::Object_Created_Event(GameObject *obj) {
	BeaconGameObj *Beacon = (BeaconGameObj*)obj;
	if (Beacon->Get_Owner()) {
		BuildingGameObj *Building = Get_Closest_Building(Beacon->Get_Position(),Beacon->Get_Player_Type()?0:1);
		if (Building) {
			ExplosionDefinitionClass *Explosion = (ExplosionDefinitionClass*)Find_Definition(Beacon->Get_Definition().ExplosionObjDef);
			float DamageRadius = Explosion->DamageRadius*Explosion->DamageRadius;
			float Distance = 0.0f;
			Building->Find_Closest_Poly(Beacon->Get_Position(),&Distance);
			if (Distance > DamageRadius) {
				Set_Bullets(Beacon->Get_Owner(),Beacon->Get_WeaponDef()->Get_Name(),2);
				Beacon->Set_Delete_Pending();
				DA::Page_Player(Beacon->Get_Owner(),"Beacons may only be deployed where they would damage an enemy building.");
			}
		}
	}
}

void DAC4BeaconManager::Beacon_Deploy_Event(BeaconGameObj *Beacon) {
	if (Beacon->Get_Owner()) {
		if (The_Cnc_Game()->BeaconPlacementEndsGame && CollisionMath::Overlap_Test((Beacon->Get_Player_Type()?The_Cnc_Game()->Nod:The_Cnc_Game()->GDI).Get_Beacon_Zone(),Beacon->Get_Position()) == CollisionMath::INSIDE) {
			DA::Team_Player_Message(Beacon->Get_Owner(),"Defend my beacon on the pedestal!");
		}
		else {
			BuildingGameObj *Building = Get_Closest_Building(Beacon->Get_Position(),Beacon->Get_Player_Type()?0:1);
			if (Building) {
				ExplosionDefinitionClass *Explosion = (ExplosionDefinitionClass*)Find_Definition(Beacon->Get_Definition().ExplosionObjDef);
				float DamageRadius = Explosion->DamageRadius*Explosion->DamageRadius;
				float Distance = 0.0f;
				Building->Find_Closest_Poly(Beacon->Get_Position(),&Distance);
				if (Distance <= DamageRadius) {
					DA::Team_Player_Message(Beacon->Get_Owner(),"Defend my beacon at the %s!",DATranslationManager::Translate(Building));
				}
				else {
					DA::Team_Player_Message(Beacon->Get_Owner(),"The beacon is a fake.");
				}
			}
		}
		DALogManager::Write_Log("_BEACON","%ls deployed %s.",((cPlayer*)Beacon->Get_Player_Data())->Get_Name(),a_or_an_Prepend(DATranslationManager::Translate(Beacon)));
	}
}

void DAC4BeaconManager::Beacon_Detonate_Event(BeaconGameObj *Beacon) {
	DALogManager::Write_Log("_BEACON","%s has detonated.",A_Or_An_Prepend(DATranslationManager::Translate(Beacon)));
}

void DAC4BeaconManager::C4_Detonate_Event(C4GameObj *C4) {
	DALogManager::Write_Log("_C4","%ls %s has detonated (Owner: %ls - Attached to: %s)",A_Or_An_Prepend(Get_Wide_Team_Name(C4->Get_Player_Type())),DATranslationManager::Translate(C4),C4->Get_Player_Data()?((cPlayer*)C4->Get_Player_Data())->Get_Name():L"None",C4->Get_Stuck_Object()?DATranslationManager::Translate(C4->Get_Stuck_Object()):"None");
}

void DAC4BeaconManager::Poke_Event(PhysicalGameObj *obj,SoldierGameObj *Poker) {
	if (obj->As_BeaconGameObj()) {
		if (((BeaconGameObj*)obj)->Get_Owner()) {
			DA::Page_Player(Poker,"The owner of this beacon is %ls.",((BeaconGameObj*)obj)->Get_Owner()->Get_Player()->Get_Name());
		}
		else {
			DA::Page_Player(Poker,"This beacon has no owner.");
		}
	}
	else {
		if (((C4GameObj*)obj)->Get_Owner()) {
			DA::Page_Player(Poker,"The owner of this C4 is %ls.",((C4GameObj*)obj)->Get_Owner()->Get_Player()->Get_Name());
		}
		else {
			DA::Page_Player(Poker,"This C4 has no owner.");
		}
	}
}

void DAC4BeaconManager::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	if (((PhysicalGameObj*)Victim)->As_BeaconGameObj()) {
		if (Is_Player(Killer)) {
			DALogManager::Write_Log("_BEACON","%ls disarmed %s.",((SoldierGameObj*)Killer)->Get_Player()->Get_Name(),a_or_an_Prepend(DATranslationManager::Translate(Victim)));
		}
		else {
			DALogManager::Write_Log("_BEACON","%s was disarmed.",A_Or_An_Prepend(DATranslationManager::Translate(Victim)));
		}
	}
	else {
		C4GameObj *C4 = (C4GameObj*)Victim;
		if (Is_Player(Killer)) {
			DALogManager::Write_Log("_C4","%ls disarmed %ls %s (Owner: %ls - Attached to: %s)",((SoldierGameObj*)Killer)->Get_Player()->Get_Name(),a_or_an_Prepend(Get_Wide_Team_Name(C4->Get_Player_Type())),DATranslationManager::Translate(C4),C4->Get_Player_Data()?((cPlayer*)C4->Get_Player_Data())->Get_Name():L"None",C4->Get_Stuck_Object()?DATranslationManager::Translate(C4->Get_Stuck_Object()):"None");
		}
		else {
			DALogManager::Write_Log("_C4","%ls %s was disarmed (Owner: %ls - Attached to: %s)",A_Or_An_Prepend(Get_Wide_Team_Name(C4->Get_Player_Type())),DATranslationManager::Translate(C4),C4->Get_Player_Data()?((cPlayer*)C4->Get_Player_Data())->Get_Name():L"None",C4->Get_Stuck_Object()?DATranslationManager::Translate(C4->Get_Stuck_Object()):"None");
		}
	}
}

//Default powerup purchase handler.
int DAC4BeaconManager::PowerUp_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const PowerUpGameObjDef *Item) {
	//The normal protection against buying a weapon twice doesn't work if the PT data on the server has been changed.
	if (Item->GrantWeapon) {
		WeaponBagClass *Bag = Player->Get_GameObj()->Get_Weapon_Bag();
		for (int i = 1;i < Bag->Get_Count();i++) {
			if (Bag->Peek_Weapon(i)->Get_ID() == Item->GrantWeaponID && Bag->Peek_Weapon(i)->Get_Total_Rounds()) {
				return 4;
			}
		}
	}
	if (Player->Purchase_Item((int)Cost)) {
		Item->Grant(Player->Get_GameObj());
		return 0;
	}
	return 2;
}


#include "da_player.h"
class DAC4ChatCommandClass: public DAChatCommandClass {
	bool Activate(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
		int Remote = 0,Prox = 0;
		int Team = Player->Get_Team();
		for (SLNode<C4GameObj> *x = GameObjManager::C4GameObjList.Head();x;x = x->Next()) {
			if (x->Data()->Get_Player_Type() == Team) {
				if (x->Data()->Get_Ammo_Def()->AmmoType == 3) {
					++Prox;
				}
				else if (x->Data()->Get_Ammo_Def()->AmmoType == 1) {
					++Remote;
				}
			}
		}
		DA::Page_Player(Player,"Remote: %d  - Proximity: %d - Total: %d - Limit: %d",Remote,Prox,Remote+Prox,Get_Mine_Limit());
		return false;
	}
};
Register_Simple_Chat_Command(DAC4ChatCommandClass,"!c4|!mine|!mines|!mlimit|!minelimit|C4Count");