/*	Renegade Scripts.dll
    Dragonade Disable List
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
#include "da_disable.h"
#include "da_settings.h"
#include "BaseControllerClass.h"
#include "RefineryGameObj.h"
#include "GameObjManager.h"
#include "PurchaseSettingsDefClass.h"
#include "PowerupGameObjDef.h"

DynamicVectorClass<unsigned int> DADisableListManager::DisableList;
HashTemplateClass<unsigned int,DynamicVectorClass<unsigned int>> DADisableListManager::CharacterDisableList;

void DADisableListManager::Init() {
	static DADisableListManager Instance;
	Instance.Register_Event(DAEvent::LEVELLOADED,INT_MAX-1);
	Instance.Register_Event(DAEvent::CHARACTERPURCHASEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEPURCHASEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPPURCHASEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPGRANTREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::ADDWEAPONREQUEST,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::ALL,INT_MAX);
}

bool DADisableListManager::Is_Preset_Disabled(GameObject *obj) {
	if (obj) {
		return (DisableList.ID(obj->Get_Definition().Get_ID()) != -1);
	}
	return false;
}

bool DADisableListManager::Is_Preset_Disabled(const char *Preset) {
	return (DisableList.ID(Get_Definition_ID(Preset)) != -1);
}

bool DADisableListManager::Is_Preset_Disabled(const DefinitionClass *Def) {
	if (!Def) {
		return false;
	}
	return (DisableList.ID(Def->Get_ID()) != -1);
}

bool DADisableListManager::Is_Preset_Disabled(unsigned int DefID) {
	return (DisableList.ID(DefID) != -1);
}

bool DADisableListManager::Is_Preset_Disabled_For_Character(GameObject *obj,const DefinitionClass *Def) {
	DynamicVectorClass<unsigned int> *Disable = CharacterDisableList.Get((unsigned int)&obj->Get_Definition());
	if (Disable) {
		return (Disable->ID(Def->Get_ID()) != -1);
	}
	return false;
}

bool DADisableListManager::Is_Preset_Disabled_For_Character(GameObject *obj,const char *Preset) {
	const DefinitionClass *Def = Find_Named_Definition(Preset);
	if (!Def) {
		return false;
	}
	DynamicVectorClass<unsigned int> *Disable = CharacterDisableList.Get((unsigned int)&obj->Get_Definition());
	if (Disable) {
		return (Disable->ID(Def->Get_ID()) != -1);
	}
	return false;
}

bool DADisableListManager::Is_Preset_Disabled_For_Character(GameObject *obj,unsigned int DefID) {
	DynamicVectorClass<unsigned int> *Disable = CharacterDisableList.Get((unsigned int)&obj->Get_Definition());
	if (Disable) {
		return (Disable->ID(DefID) != -1);
	}
	return false;
}

//This uses Level_Loaded instead of Settings_Loaded because buildings can't be destroyed mid-game.
void DADisableListManager::Level_Loaded_Event() {
	DisableList.Delete_All();
	CharacterDisableList.Remove_All();
	INISection *Section = DASettingsManager::Get_Section("Disable_List");
	if (Section) {
		for (int i = 0;i < Section->Count();i++) {
			INIEntry *Entry = Section->Peek_Entry(i);
			if (!_stricmp(Entry->Value,"1") || !_stricmp(Entry->Value,"true")) {
				if (!_stricmp(Entry->Entry,"Buildings")) {
					BaseControllerClass *Base = BaseControllerClass::Find_Base(0);
					for (int i = Base->Get_Building_List().Count()-1;i >= 0;i--) {
						Base->Get_Building_List()[i]->Set_Delete_Pending();
						Base->Get_Building_List()[i]->Clear_Object_Dirty_Bits();
						Base->Get_Building_List().Delete(i);
					}
					Base = BaseControllerClass::Find_Base(1);
					for (int i = Base->Get_Building_List().Count()-1;i >= 0;i--) {
						Base->Get_Building_List()[i]->Set_Delete_Pending();
						Base->Get_Building_List()[i]->Clear_Object_Dirty_Bits();
						Base->Get_Building_List().Delete(i);
					}
				}
				else if (!_stricmp(Entry->Entry,"BaseDefenses")) {
					Disable_Building_Type(0,BuildingConstants::TYPE_BASE_DEFENSE);
					Disable_Building_Type(1,BuildingConstants::TYPE_BASE_DEFENSE);
					for (SLNode<VehicleGameObj> *z = GameObjManager::VehicleGameObjList.Head();z;z = z->Next()) {
						if (z->Data()->Is_Turret()) {
							z->Data()->Set_Delete_Pending();
							z->Data()->Clear_Object_Dirty_Bits();
						}
					}
				}
				else if (!_stricmp(Entry->Entry,"PowerPlants")) {
					Disable_Building_Type(0,BuildingConstants::TYPE_POWER_PLANT);
					Disable_Building_Type(1,BuildingConstants::TYPE_POWER_PLANT);
				}
				else if (!_stricmp(Entry->Entry,"Refineries")) {
					Disable_Building_Type(0,BuildingConstants::TYPE_REFINERY);
					Disable_Building_Type(1,BuildingConstants::TYPE_REFINERY);
				}
				else if (!_stricmp(Entry->Entry,"SoldierFactories")) {
					Disable_Building_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY);
					Disable_Building_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY);
				}
				else if (!_stricmp(Entry->Entry,"VehicleFactories")) {
					Disable_Building_Type(0,BuildingConstants::TYPE_VEHICLE_FACTORY);
					Disable_Building_Type(1,BuildingConstants::TYPE_VEHICLE_FACTORY);
				}
				else if (!_stricmp(Entry->Entry,"Helipads")) {
					Disable_Building_Type(0,BuildingConstants::TYPE_HELIPAD);
					Disable_Building_Type(1,BuildingConstants::TYPE_HELIPAD);
				}
				else if (!_stricmp(Entry->Entry,"NavalFactories")) {
					Disable_Building_Type(0,BuildingConstants::TYPE_NAVAL_FACTORY);
					Disable_Building_Type(1,BuildingConstants::TYPE_NAVAL_FACTORY);
				}
				else if (!_stricmp(Entry->Entry,"RepairPads")) {
					Disable_Building_Type(0,BuildingConstants::TYPE_REPAIR_BAY);
					Disable_Building_Type(1,BuildingConstants::TYPE_REPAIR_BAY);
				}
				else if (!_stricmp(Entry->Entry,"CommCenters")) {
					Disable_Building_Type(0,BuildingConstants::TYPE_COM_CENTER);
					Disable_Building_Type(1,BuildingConstants::TYPE_COM_CENTER);
				}
				else if (!_stricmp(Entry->Entry,"ConYards")) {
					Disable_Building_Type(0,BuildingConstants::TYPE_CONYARD);
					Disable_Building_Type(1,BuildingConstants::TYPE_CONYARD);
				}
				else if (!_stricmp(Entry->Entry,"TechCenters")) {
					Disable_Building_Type(0,BuildingConstants::TYPE_TECH_CENTER);
					Disable_Building_Type(1,BuildingConstants::TYPE_TECH_CENTER);
				}
				else if (!_stricmp(Entry->Entry,"Beacons")) {
					for (PowerUpGameObjDef *PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_First(CID_PowerUp);PowerUpDef;PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_Next(PowerUpDef,CID_PowerUp)) {
						if (PowerUpDef->GrantWeapon) {
							WeaponDefinitionClass *WeapDef = (WeaponDefinitionClass*)Find_Definition(PowerUpDef->GrantWeaponID);
							if (WeapDef && WeapDef->Style == STYLE_BEACON) {
								Disable_Preset_By_Name(0,PowerUpDef->Get_Name(),true);
								Disable_Preset_By_Name(1,PowerUpDef->Get_Name(),true);
								DisableList.Add(PowerUpDef->Get_ID());
								DisableList.Add(PowerUpDef->GrantWeaponID);
							}
						}
					}
				}
				else if (!_stricmp(Entry->Entry,"Vehicles")) {
					for (VehicleGameObjDef *VehicleDef = (VehicleGameObjDef*)DefinitionMgrClass::Get_First(CID_Vehicle);VehicleDef;VehicleDef = (VehicleGameObjDef*)DefinitionMgrClass::Get_Next(VehicleDef,CID_Vehicle)) {
						if (VehicleDef->Get_Type() != VEHICLE_TYPE_TURRET && VehicleDef->Get_ID() != (unsigned int)Get_Harvester_Preset_ID(0) && VehicleDef->Get_ID() != (unsigned int)Get_Harvester_Preset_ID(1)) {
							Disable_Preset_By_Name(0,VehicleDef->Get_Name(),true);
							Disable_Preset_By_Name(1,VehicleDef->Get_Name(),true);
							DisableList.Add(VehicleDef->Get_ID());
						}
					}
					Set_Vehicle_Limit(0);
				}
				else if (!_stricmp(Entry->Entry,"Harvesters")) {
					RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(0);
					if (Ref) {
						Ref->Block_Harvester_Spawn();
					}
					Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(1);
					if (Ref) {
						Ref->Block_Harvester_Spawn();
					}
				}
				else { //Preset Name
					for (DefinitionClass *Def = DefinitionMgrClass::Get_First();Def;Def = DefinitionMgrClass::Get_Next(Def)) {
						if (!_stricmp(Def->Get_Name(),Entry->Entry)) {
							Disable_Preset_By_Name(0,Def->Get_Name(),true);
							Disable_Preset_By_Name(1,Def->Get_Name(),true);
							DisableList.Add(Def->Get_ID());
						}
					}
				}
			}
		}
	}

	for (DefinitionClass *Def = DefinitionMgrClass::Get_First(CID_Soldier);Def;Def = DefinitionMgrClass::Get_Next(Def,CID_Soldier)) {
		Section = DASettingsManager::Get_Section(StringFormat("%s_Disable_List",Def->Get_Name()));
		if (Section) {
			DynamicVectorClass<unsigned int> Disable;
			for (int i = 0;i < Section->Count();i++) {
				INIEntry *Entry = Section->Peek_Entry(i);
				if (!_stricmp(Entry->Value,"1") || !_stricmp(Entry->Value,"true")) {
					const DefinitionClass *WeaponDef = Find_Named_Definition(Entry->Entry);
					if (WeaponDef && (WeaponDef->Get_Class_ID() == CID_Weapon || WeaponDef->Get_Class_ID() == CID_PowerUp)) {
						Disable.Add(WeaponDef->Get_ID());
					}
				}
			}
			if (Disable.Count()) {
				CharacterDisableList.Insert((unsigned int)Def,Disable);
			}	
		}
	}
	
	if (DASettingsManager::Get_Bool("Disable_List","Refills",false)) {
		Register_Event(DAEvent::REFILL,INT_MAX);
	}
	else {
		Unregister_Event(DAEvent::REFILL);
	}
}

void DADisableListManager::Object_Created_Event(GameObject *obj) {
	if (Is_Preset_Disabled(obj)) {
		obj->Set_Delete_Pending();
		obj->Clear_Object_Dirty_Bits();
	}
	else if (Is_Powerup(obj)) {
		if (Is_Preset_Disabled(((const PowerUpGameObjDef&)obj->Get_Definition()).GrantWeaponID)) {
			obj->Set_Delete_Pending();
			obj->Clear_Object_Dirty_Bits();
		}
	}
}

int DADisableListManager::Character_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const SoldierGameObjDef *Item) {
	if (Is_Preset_Disabled(Item)) {
		return 4;
	}
	return -1;
}

int DADisableListManager::Vehicle_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const VehicleGameObjDef *Item) {
	if (Is_Preset_Disabled(Item)) {
		return 4;
	}
	return -1;
}

int DADisableListManager::PowerUp_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const PowerUpGameObjDef *Item) {	
	if (Is_Preset_Disabled(Item) || Is_Preset_Disabled(Item->GrantWeaponID) || Is_Preset_Disabled_For_Character(Purchaser,Item) || Is_Preset_Disabled_For_Character(Purchaser,Item->GrantWeaponID)) {
		return 4;
	}
	return -1;
}

bool DADisableListManager::PowerUp_Grant_Request_Event(SoldierGameObj *Grantee,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	if (Is_Preset_Disabled(PowerUp) || Is_Preset_Disabled(PowerUp->GrantWeaponID) || Is_Preset_Disabled_For_Character(Grantee,PowerUp) || Is_Preset_Disabled_For_Character(Grantee,PowerUp->GrantWeaponID)) {
		return false;
	}
	return true;
}

bool DADisableListManager::Add_Weapon_Request_Event(SoldierGameObj *Soldier,const WeaponDefinitionClass *Weapon) {
	if (Is_Preset_Disabled(Weapon) || Is_Preset_Disabled_For_Character(Soldier,Weapon)) {
		return false;
	}
	return true;
}

bool DADisableListManager::Refill_Event(SoldierGameObj *Purchaser) {
	return false;
}