/*	Renegade Scripts.dll
    Dragonade Drop Weapons Game Feature
	Copyright 2012 Whitedragon, Tiberian Technologies

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
#include "engine.h"
#include "engine_DA.h"
#include "da.h"
#include "da_dropweapons.h"
#include "da_settings.h"
#include "da_translation.h"
#include "da_damagelog.h"
#include "WeaponBagClass.h"
#include "WeaponClass.h"

void DADropWeaponsPlayerDataClass::Add_Weapon(WeaponClass *Weapon) {
	if (Locker.ID(Weapon->Get_Definition()) == -1) {
		Locker.Add(Weapon->Get_Definition());
	}
}

void DADropWeaponsPlayerDataClass::Add_Weapon(const WeaponDefinitionClass *Weapon) {
	if (Locker.ID(Weapon) == -1) {
		Locker.Add(Weapon);
	}
}

void DADropWeaponsPlayerDataClass::Remove_Weapon(WeaponClass *Weapon) {
	Locker.DeleteObj(Weapon->Get_Definition());
}

void DADropWeaponsPlayerDataClass::Remove_Weapon(const WeaponDefinitionClass *Weapon) {
	Locker.DeleteObj(Weapon);
}

bool DADropWeaponsPlayerDataClass::Has_Weapon(WeaponClass *Weapon) {
	return (Locker.ID(Weapon->Get_Definition()) != -1);
}

bool DADropWeaponsPlayerDataClass::Has_Weapon(const WeaponDefinitionClass *Weapon) {
	return (Locker.ID(Weapon) != -1);
}

DADropWeaponsBackpackClass::DADropWeaponsBackpackClass() {
	Dropper = 0;
}

DADropWeaponsBackpackClass::DADropWeaponsBackpackClass(SoldierGameObj *Soldier) {
	Dropper = Soldier->Get_Player();
}

void DADropWeaponsBackpackClass::Add_Weapon(const WeaponDefinitionClass *Weapon,int Rounds) {
	WeaponStruct Struct;
	Struct.Weapon = Weapon;
	Struct.Rounds = Rounds;
	if (Weapons.ID(Struct) == -1) {
		Weapons.Add(Struct);
	}
}

void DADropWeaponsBackpackClass::Add_Damager(SoldierGameObj *Soldier) {
	Damagers.Add(Soldier);
}

void DADropWeaponsBackpackClass::Custom(GameObject *obj,int Type,int Param,GameObject *Sender) {
	if (Type == 1000000025 && Is_Player(Sender) && ((SoldierGameObj*)Sender)->Get_Player() != Dropper) {
		for (int i = Damagers.Count()-1;i >= 0;i--) {
			if (!Damagers[i]) {
				Damagers.Delete(i);
			}
		}
		if (!Dropper || !Damagers.Count() || Damagers.ID(Sender) != -1 || ((SoldierGameObj*)Sender)->Get_Player_Type() == Dropper->Get_Player_Type()) {
			WeaponBagClass *Bag = ((SoldierGameObj*)Sender)->Get_Weapon_Bag();
			bool PickedUp = false;
			for (int i = Weapons.Count()-1;i >= 0;i--) {
				WeaponClass *Weapon = Bag->Find_Weapon(Weapons[i].Weapon);
				if (Weapon) {
					if (Weapon->Get_Inventory_Rounds() < Weapon->Get_Definition()->MaxInventoryRounds) {
						Bag->Add_Weapon(Weapons[i].Weapon,Weapons[i].Rounds,true);
						Weapons.Delete(i);
						PickedUp = true;
					}
				}
				else if (Bag->Add_Weapon(Weapons[i].Weapon,Weapons[i].Rounds,true)) {
					Weapons.Delete(i);
					PickedUp = true;
				}
			}
			if (PickedUp) {
				Create_2D_WAV_Sound_Player(Sender,"powerup_ammo.wav");
			}
			if (!Weapons.Count()) {
				obj->Set_Delete_Pending();
			}
		}
	}
}

void DADropWeaponsBackpackClass::Timer_Expired(GameObject *obj,int Number) {
	if (Number == 1) {
		Commands->Create_Object("Spawner Created Special Effect",Commands->Get_Position(obj));
	}
	else if (Number == 2) {
		Commands->Destroy_Object(obj);
	}
	else if (Number == 3) {
		Damagers.Delete_All();
	}
}

const WeaponDefinitionClass *DADropWeaponsGameFeatureClass::Get_Character_Weapon_Drop(SoldierGameObj *Soldier) {
	if (!Soldier) {
		return 0;
	}
	return CharacterWeaponDrops.Get((unsigned int)&Soldier->Get_Definition(),0);
}

const WeaponDefinitionClass *DADropWeaponsGameFeatureClass::Get_Character_Weapon_Drop(const SoldierGameObjDef *Soldier) {
	return CharacterWeaponDrops.Get((unsigned int)Soldier,0);
}

bool DADropWeaponsGameFeatureClass::Is_Droppable(WeaponClass *Weapon) {
	return (Undroppable.ID(Weapon->Get_Definition()) == -1);
}

bool DADropWeaponsGameFeatureClass::Is_Droppable(const WeaponDefinitionClass *Weapon) {
	return (Undroppable.ID(Weapon) == -1);
}

DADropWeaponsBackpackClass *DADropWeaponsGameFeatureClass::Create_Backpack_With_Damagers(SoldierGameObj *Soldier) {
	DADropWeaponsBackpackClass *Backpack = new DADropWeaponsBackpackClass(Soldier);
	PhysicalGameObj *BackpackObj = Create_Object("POW_Backpack",Soldier->Get_Transform());
	BackpackObj->Add_Observer(Backpack);
	if (ExpireTime > 1.0f) {
		Backpack->Start_Timer(1,ExpireTime-1.0f);
		Backpack->Start_Timer(2,ExpireTime);
	}
	if (DamagersOnlyTime >= 0.0f) {
		if (DamagersOnlyTime > 0.0f) {
			Backpack->Start_Timer(3,DamagersOnlyTime);
		}
		DADamageTableStruct DamageTable[128];
		DADamageLog::Compile_Damage_Table_Other_Team(DamageTable,Soldier,Soldier->Get_Player_Type());
		Vector3 Position;
		BackpackObj->Get_Position(&Position);
		for (int i = 1;i < 128;i++) {
			if (DamageTable[i].Player && DamageTable[i].Player->Get_GameObj()) {
				if (DamagersOnlyDistance > 0.0f && Commands->Get_Distance(DamageTable[i].Player->Get_GameObj()->Get_Position(),Position) <= DamagersOnlyDistance) {
					Backpack->Add_Damager(DamageTable[i].Player->Get_GameObj());
				}
			}
		}
	}
	return Backpack;
}

DADropWeaponsBackpackClass *DADropWeaponsGameFeatureClass::Create_Backpack(SoldierGameObj *Soldier) {
	DADropWeaponsBackpackClass *Backpack = new DADropWeaponsBackpackClass(Soldier);
	Create_Object("POW_Backpack",Soldier->Get_Transform())->Add_Observer(Backpack);
	if (ExpireTime > 1.0f) {
		Backpack->Start_Timer(1,ExpireTime-1.0f);
		Backpack->Start_Timer(2,ExpireTime);
	}
	return Backpack;
}

DADropWeaponsBackpackClass *DADropWeaponsGameFeatureClass::Create_Backpack(const Vector3 &Position) {
	DADropWeaponsBackpackClass *Backpack = new DADropWeaponsBackpackClass;
	Create_Object("POW_Backpack",Position)->Add_Observer(Backpack);
	if (ExpireTime > 1.0f) {
		Backpack->Start_Timer(1,ExpireTime-1.0f);
		Backpack->Start_Timer(2,ExpireTime);
	}
	return Backpack;
}

void DADropWeaponsGameFeatureClass::Init() {
	DAPlayerDataManagerClass<DADropWeaponsPlayerDataClass>::Init();
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::ADDWEAPON);
	Register_Event(DAEvent::REMOVEWEAPON);
	Register_Event(DAEvent::CLEARWEAPONS);
	Register_Event(DAEvent::CHANGECHARACTER);
	Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::PLAYER);
	Register_Object_Event(DAObjectEvent::DESTROYED,DAObjectEvent::PLAYER);
	Register_Chat_Command((DAECC)&DADropWeaponsGameFeatureClass::WDrop_Chat_Command,"!wdrop|!drop|!weapdrop|!weapondrop|!dropweap|!dropweapon|!rweapon|!removeweapon|WeapDrop");
}

void DADropWeaponsGameFeatureClass::Settings_Loaded_Event() {
	Undroppable.Delete_All();
	CharacterWeaponDrops.Remove_All();
	INISection *Section = DASettingsManager::Get_Section("Drop_Weapons");
	if (Section) {
		for (int i = 0;i < Section->Count();i++) {
			INIEntry *Entry = Section->Peek_Entry(i);
			DefinitionClass *Def = Find_Named_Definition(Entry->Entry);
			if (Def) {
				if (Def->Get_Class_ID() == CID_Weapon) { //Undroppable weapons
					if (!_stricmp(Entry->Value,"1") || !_stricmp(Entry->Value,"true")) {
						Undroppable.Add((WeaponDefinitionClass*)Def);
					}
				}
				else if (Def->Get_Class_ID() == CID_Soldier) { //Soldier -> Weapon overrides
					const WeaponDefinitionClass *WeaponDef = (WeaponDefinitionClass*)Find_Named_Definition(Entry->Value);
					if (WeaponDef && WeaponDef->Get_Class_ID() == CID_Weapon) {
						CharacterWeaponDrops.Insert((unsigned int)Def,WeaponDef);
					}
				}
			}
		}
	}
	for (SoldierGameObjDef *SoldierDef = (SoldierGameObjDef*)DefinitionMgrClass::Get_First(CID_Soldier);SoldierDef;SoldierDef = (SoldierGameObjDef*)DefinitionMgrClass::Get_Next(SoldierDef,CID_Soldier)) {
		const WeaponDefinitionClass *WeaponDef = (WeaponDefinitionClass*)Find_Definition(SoldierDef->WeaponDefID);
		if (WeaponDef) {
			if (!CharacterWeaponDrops.Exists((unsigned int)SoldierDef)) { //Default Soldier -> Weapon
				CharacterWeaponDrops.Insert((unsigned int)SoldierDef,WeaponDef);
			}
		}
	}
	ExpireTime = DASettingsManager::Get_Float("Drop_Weapons","ExpireTime",20.0f);
	DamagersOnlyTime = DASettingsManager::Get_Float("Drop_Weapons","DamagersOnlyTime",5.0f);
	DamagersOnlyDistance = DASettingsManager::Get_Float("Drop_Weapons","DamagersOnlyDistance",10.0f);
	PowerUpGameObjDef *Backpack = (PowerUpGameObjDef*)Find_Named_Definition("POW_Backpack");
	Backpack->Persistent = true;
	Backpack->GrantWeapon = false;
	Backpack->AlwaysAllowGrant = true;
}

void DADropWeaponsGameFeatureClass::Object_Created_Event(GameObject *obj) {
	DADropWeaponsPlayerDataClass *Player = Get_Player_Data(((SoldierGameObj*)obj)->Get_Player());
	WeaponBagClass *Bag = ((SoldierGameObj*)obj)->Get_Weapon_Bag();
	for (int i = 0;i < Player->Locker.Count();i++) { //Keep weapons when switching characters.
		Bag->Add_Weapon(Player->Locker[i],999);
	}
	//Grant_Refill(obj);
	Get_Player_Data(((SoldierGameObj*)obj)->Get_Player())->CreationTime = The_Game()->FrameCount;
}

void DADropWeaponsGameFeatureClass::Change_Character_Event(SoldierGameObj *Soldier,const SoldierGameObjDef *SoldierDef) {
	Get_Player_Data(Soldier->Get_Player())->CreationTime = The_Game()->FrameCount;
}

void DADropWeaponsGameFeatureClass::Object_Destroyed_Event(GameObject *obj) {
	DADropWeaponsBackpackClass *Backpack = Create_Backpack_With_Damagers((SoldierGameObj*)obj);
	DADropWeaponsPlayerDataClass *Player = Get_Player_Data(((SoldierGameObj*)obj)->Get_Player());
	const WeaponDefinitionClass *MainDrop = Get_Character_Weapon_Drop((SoldierGameObj*)obj);
	WeaponBagClass *Bag = ((SoldierGameObj*)obj)->Get_Weapon_Bag();
	bool MainDropFound = false;
	for (int i = Bag->Get_Count()-1;i >= 1;i--) {
		WeaponClass *Weapon = Bag->Peek_Weapon(i);
		const WeaponDefinitionClass *WeaponDef = Weapon->Get_Definition();
		if (Weapon->Get_Total_Rounds() || (WeaponDef->Style != STYLE_C4 && WeaponDef->Style != STYLE_BEACON)) { //Don't drop empty C4 or beacons.
			if (Player->Has_Weapon(WeaponDef)) {
				Backpack->Add_Weapon(WeaponDef,Weapon->Get_Total_Rounds());
			}
			else if (WeaponDef == MainDrop) {
				Backpack->Add_Weapon(WeaponDef,Weapon->Get_Total_Rounds());
				MainDropFound = true;
			}
		}
	}
	if (MainDrop) {
		if (!MainDropFound) { //Try to find a match by translation ID.
			int IconID = MainDrop->IconNameID;
			for (int i = Bag->Get_Count()-1;i >= 1;i--) {
				WeaponClass *Weapon = Bag->Peek_Weapon(i);
				if (Weapon->Get_Definition()->IconNameID == IconID && (Weapon->Get_Total_Rounds() || (MainDrop->Style != STYLE_C4 && MainDrop->Style != STYLE_BEACON))) {
					Backpack->Add_Weapon(MainDrop,Weapon->Get_Total_Rounds());
					MainDropFound = true;
					break;
				}
			}
		}
		if (!MainDropFound) { //Try to find a match by ammo ID.
			unsigned int AmmoDefID = MainDrop->PrimaryAmmoDefID;
			for (int i = Bag->Get_Count()-1;i >= 1;i--) {
				WeaponClass *Weapon = Bag->Peek_Weapon(i);
				if (Weapon->PrimaryAmmoDefinition->Get_ID() == AmmoDefID && (Weapon->Get_Total_Rounds() || (MainDrop->Style != STYLE_C4 && MainDrop->Style != STYLE_BEACON))) {
					Backpack->Add_Weapon(MainDrop,Weapon->Get_Total_Rounds());
					MainDropFound = true;
					break;
				}
			}
		}
		if (!MainDropFound && MainDrop->Style != STYLE_C4 && MainDrop->Style != STYLE_BEACON) { //Just going to have to drop an empty one then.
			Backpack->Add_Weapon(MainDrop,0);
		}
	}
	if (!Backpack->Get_Weapon_Count()) {
		Backpack->Get_Owner()->Set_Delete_Pending();
	}
	Player->Clear_Weapons();
}

void DADropWeaponsGameFeatureClass::Add_Weapon_Event(SoldierGameObj *Soldier,WeaponClass *Weapon) {
	if (Get_Player_Data(Soldier->Get_Player())->CreationTime != The_Game()->FrameCount && Is_Droppable(Weapon)) { //Don't add undroppable or starting weapons to the locker.
		Get_Player_Data(Soldier->Get_Player())->Add_Weapon(Weapon);
	}
}

void DADropWeaponsGameFeatureClass::Remove_Weapon_Event(SoldierGameObj *Soldier,WeaponClass *Weapon) {
	Get_Player_Data(Soldier->Get_Player())->Remove_Weapon(Weapon);
}

void DADropWeaponsGameFeatureClass::Clear_Weapons_Event(SoldierGameObj *Soldier) {
	Get_Player_Data(Soldier->Get_Player())->Clear_Weapons();
}

bool DADropWeaponsGameFeatureClass::WDrop_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DADropWeaponsPlayerDataClass *PlayerData = Get_Player_Data(Player);
	if (!PlayerData->Get_Weapon_Count() || !Player->Get_GameObj() || Player->Get_GameObj()->Get_Defense_Object()->Get_Health() == 0.0f) {
		DA::Private_Color_Message(Player,WHITE,"You have no weapons to drop.");
	}
	else if (Text.Size()) {
		if (Text[0] == "all") {
			DADropWeaponsBackpackClass *Backpack = Create_Backpack(Player->Get_GameObj());
			WeaponBagClass *Bag = Player->Get_GameObj()->Get_Weapon_Bag();
			for (int i = Bag->Get_Count()-1;i >= 1;i--) {
				WeaponClass *Weapon = Bag->Peek_Weapon(i);
				const WeaponDefinitionClass *WeaponDef = Weapon->Get_Definition();
				if (PlayerData->Has_Weapon(WeaponDef) && (Weapon->Get_Total_Rounds() || (WeaponDef->Style != STYLE_C4 && WeaponDef->Style != STYLE_BEACON))) {
					Backpack->Add_Weapon(WeaponDef,Weapon->Get_Total_Rounds());
					Bag->Remove_Weapon(i);
				}
			}
			if (!Backpack->Get_Weapon_Count()) {
				Backpack->Get_Owner()->Set_Delete_Pending();
				DA::Private_Color_Message(Player,WHITE,"You have no weapons to drop.");
			}
			else {
				DA::Private_Color_Message(Player,WHITE,"You have dropped all your weapons.");
			}
		}
		else {
			WeaponBagClass *Bag = Player->Get_GameObj()->Get_Weapon_Bag();
			for (int i = Bag->Get_Count()-1;i >= 1;i--) {
				WeaponClass *Weapon = Bag->Peek_Weapon(i);
				const WeaponDefinitionClass *WeaponDef = Weapon->Get_Definition();
				const char *Translation = DATranslationManager::Translate(WeaponDef);
				if (PlayerData->Has_Weapon(WeaponDef) && stristr(Translation,Text[0]) && (Weapon->Get_Total_Rounds() || (WeaponDef->Style != STYLE_C4 && WeaponDef->Style != STYLE_BEACON))) { //Only let them drop weapons they've picked up, not started with.
					DA::Private_Color_Message(Player,WHITE,"You have dropped your %s.",Translation);
					DADropWeaponsBackpackClass *Backpack = Create_Backpack(Player->Get_GameObj());
					Backpack->Add_Weapon(WeaponDef,Weapon->Get_Total_Rounds());
					Bag->Remove_Weapon(i);
					return true;
				}
			}
			DA::Private_Color_Message(Player,WHITE,"You cannot drop that weapon.");
		}
	}
	else {
		WeaponBagClass *Bag = Player->Get_GameObj()->Get_Weapon_Bag();
		WeaponClass *Weapon = Bag->Get_Weapon();
		const WeaponDefinitionClass *WeaponDef = Weapon->Get_Definition();
		if (PlayerData->Has_Weapon(WeaponDef) && (Weapon->Get_Total_Rounds() || (WeaponDef->Style != STYLE_C4 && WeaponDef->Style != STYLE_BEACON))) { //Only let them drop weapons they've picked up, not started with.
			DA::Private_Color_Message(Player,WHITE,"You have dropped your %s.",DATranslationManager::Translate(Weapon));
			DADropWeaponsBackpackClass *Backpack = Create_Backpack(Player->Get_GameObj());
			Backpack->Add_Weapon(WeaponDef,Weapon->Get_Total_Rounds());
			Bag->Remove_Weapon(Bag->Get_Index());
			return true;
		}
		DA::Private_Color_Message(Player,WHITE,"You cannot drop that weapon.");
	}
	return true;
}

DA_API Register_Game_Feature(DADropWeaponsGameFeatureClass,"Drop Weapons","EnableDropWeapons",0);
