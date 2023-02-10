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

#ifndef INCLUDE_DADROPWEAPONS
#define INCLUDE_DADROPWEAPONS

#include "da_event.h"
#include "da_gamefeature.h"
#include "da_gameobj.h"
#include "da_player.h"
#include "HashTemplateClass.h"

class DA_API DADropWeaponsPlayerDataClass : public DAPlayerDataClass {
public:
	void Init() {
		CreationTime = 0;
	}
	void Clear_Level() {
		Clear_Weapons();
	}
	void Add_Weapon(WeaponClass *Weapon);
	void Add_Weapon(const WeaponDefinitionClass *Weapon);
	void Remove_Weapon(WeaponClass *Weapon);
	void Remove_Weapon(const WeaponDefinitionClass *Weapon);
	bool Has_Weapon(WeaponClass *Weapon);
	bool Has_Weapon(const WeaponDefinitionClass *Weapon);
	void Clear_Weapons() {
		Locker.Delete_All();
	}
	inline int Get_Weapon_Count() {
		return Locker.Count();
	}
	DynamicVectorClass<const WeaponDefinitionClass*> Locker;
	unsigned long CreationTime;
};

class DA_API DADropWeaponsBackpackClass : public DAGameObjObserverClass {
public:
	DADropWeaponsBackpackClass();
	DADropWeaponsBackpackClass(SoldierGameObj *Soldier);
	void Add_Weapon(const WeaponDefinitionClass *Weapon,int Rounds);
	void Add_Damager(SoldierGameObj *Soldier);
	inline int Get_Weapon_Count() {
		return Weapons.Count();
	}

private:
	struct WeaponStruct {
		bool operator==(const WeaponStruct &That) {
			return Weapon == That.Weapon;
		}
		bool operator!=(const WeaponStruct &That) {
			return Weapon != That.Weapon;
		}
		const WeaponDefinitionClass *Weapon;
		int Rounds;
	};
	virtual void Custom(GameObject *obj,int Type,int Param,GameObject *Sender);
	virtual void Timer_Expired(GameObject *obj,int Number);
	virtual const char *Get_Name() { 
		return "DADropWeaponsBackpackClass"; 
	}
	
	DynamicVectorClass<WeaponStruct> Weapons;
	DynamicVectorClass<ReferencerClass> Damagers;
	cPlayer *Dropper;
};

class DA_API DADropWeaponsGameFeatureClass : public DAEventClass, public DAGameFeatureClass, public DAPlayerDataManagerClass<DADropWeaponsPlayerDataClass> {
public:
	const WeaponDefinitionClass *Get_Character_Weapon_Drop(SoldierGameObj *Soldier);
	const WeaponDefinitionClass *Get_Character_Weapon_Drop(const SoldierGameObjDef *Soldier);
	bool Is_Droppable(WeaponClass *Weapon);
	bool Is_Droppable(const WeaponDefinitionClass *Weapon);
	DADropWeaponsBackpackClass *Create_Backpack_With_Damagers(SoldierGameObj *Soldier);
	DADropWeaponsBackpackClass *Create_Backpack(SoldierGameObj *Soldier);
	DADropWeaponsBackpackClass *Create_Backpack(const Vector3 &Position);

private:
	virtual void Init();
	virtual void Settings_Loaded_Event();
	virtual void Object_Created_Event(GameObject *obj);
	virtual void Change_Character_Event(SoldierGameObj *Soldier,const SoldierGameObjDef *SoldierDef);
	virtual void Object_Destroyed_Event(GameObject *obj);
	virtual void Add_Weapon_Event(SoldierGameObj *Soldier,WeaponClass *Weapon);
	virtual void Remove_Weapon_Event(SoldierGameObj *Soldier,WeaponClass *Weapon);
	virtual void Clear_Weapons_Event(SoldierGameObj *Soldier);

	bool WDrop_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);

	DynamicVectorClass<const WeaponDefinitionClass*> Undroppable;
	HashTemplateClass<unsigned int,const WeaponDefinitionClass*> CharacterWeaponDrops; //SoldierGameObjDef*,WeaponDefinitionClass*
	float ExpireTime;
	float DamagersOnlyTime;
	float DamagersOnlyDistance;
};

extern DA_API DAGameFeatureRegistrant<DADropWeaponsGameFeatureClass> DADropWeaponsGameFeatureClassRegistrant;
#define DADropWeaponsGameFeature ((DADropWeaponsGameFeatureClass*)DADropWeaponsGameFeatureClassRegistrant.Get_Instance())

#endif