/*	Renegade Scripts.dll
    Dragonade Infantry Only Crates
	Copyright 2013 Whitedragon, Tiberian Technologies

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
#include "engine_da.h"
#include "da.h"
#include "da_crate.h"
#include "da_event.h"
#include "da_player.h"
#include "da_gameobj.h"
#include "da_translation.h"
#include "da_vehicle.h"
#include "da_settings.h"
#include "da_disable.h"
#include "PurchaseSettingsDefClass.h"
#include "TeamPurchaseSettingsDefClass.h"
#include "GameObjManager.h"

/********************************************************************************************************************************/

class DARandomCharacterCrateClass : public DACrateClass {
	virtual void Settings_Loaded() {
		DACrateClass::Settings_Loaded();

		//Characters
		Characters.Delete_All();
		for (int Team = 0;Team < 2;Team++) { //Add all purchasable characters into vector.
			for (int Type = 0;Type < 7;Type++) {
				PurchaseSettingsDefClass *PT = PurchaseSettingsDefClass::Find_Definition((PurchaseSettingsDefClass::TYPE)Type,(PurchaseSettingsDefClass::TEAM)Team);
				if (PT) {
					for (int i = 0;i < 10;i++) {
						SoldierGameObjDef *Def = (SoldierGameObjDef*)Find_Definition(PT->Get_Definition(i));
						if (Def && Def->Get_Class_ID() == CID_Soldier && !stristr(Def->Get_Name(),"Mutant")) {
							DynamicVectorClass<const SoldierGameObjDef*> Chars;
							Chars.Add(Def);
							if (PT->Get_Alt_Definition(i,0)) {
								Chars.Add((SoldierGameObjDef*)Find_Definition(PT->Get_Alt_Definition(i,0)));
							}
							if (PT->Get_Alt_Definition(i,1)) {
								Chars.Add((SoldierGameObjDef*)Find_Definition(PT->Get_Alt_Definition(i,1)));
							}
							if (PT->Get_Alt_Definition(i,2)) {
								Chars.Add((SoldierGameObjDef*)Find_Definition(PT->Get_Alt_Definition(i,2)));
							}
							Characters.Add(Chars);
						}
					}
				}
			}
		}
	}

	virtual bool Can_Activate(cPlayer *Player) {
		return Characters.Count() > 1;
	}

	virtual void Activate(cPlayer *Player) {
		Reselect:
		int Rand = Get_Random_Int(0,Characters.Count()); //Get random character.
		DynamicVectorClass<const SoldierGameObjDef*> &Chars = Characters[Rand];
		if (Chars.ID(&Player->Get_GameObj()->Get_Definition()) != -1) { //Don't pick the character they already are.
			goto Reselect;
		}
		Rand = Get_Random_Int(0,Chars.Count()); //Get random skin of that character.
		const SoldierGameObjDef *Character = Chars[Rand];
		Player->Get_GameObj()->Re_Init(*Character);
		Player->Get_GameObj()->Post_Re_Init();
		DA::Page_Player(Player,"You have been transformed into %s by the Random Character Crate.",a_or_an_Prepend(DATranslationManager::Translate(Character)));
	}
	DynamicVectorClass<DynamicVectorClass<const SoldierGameObjDef*>> Characters;
};

Register_Crate(DARandomCharacterCrateClass,"Random Character",DACrateType::INFANTRY);

/********************************************************************************************************************************/

class DARandomVehicleCrateClass : public DACrateClass {
	virtual void Settings_Loaded() {
		DACrateClass::Settings_Loaded();

		//Vehicles
		Vehicles.Delete_All();
		if (Find_Vehicle_Factory(0) && Find_Vehicle_Factory(1)) {
			for (int Team = 0;Team < 2;Team++) { //Add all purchasable vehicles into vector.
				for (int Type = 0;Type < 7;Type++) {
					PurchaseSettingsDefClass *PT = PurchaseSettingsDefClass::Find_Definition((PurchaseSettingsDefClass::TYPE)Type,(PurchaseSettingsDefClass::TEAM)Team);
					if (PT) {
						for (int i = 0;i < 10;i++) {
							VehicleGameObjDef *Def = (VehicleGameObjDef*)Find_Definition(PT->Get_Definition(i));
							if (Def && Def->Get_Class_ID() == CID_Vehicle && (Def->Get_Type() != VEHICLE_TYPE_FLYING || Is_Map_Flying())) {
								Vehicles.Add(Def);
							}
						}
					}
				}
			}
			VehicleGameObjDef *Def = (VehicleGameObjDef*)Find_Named_Definition("CnC_Nod_Recon_Bike"); //Special case for Recon Bike.
			if (Def) {
				Def->WeaponDefID = Get_Definition_ID("Weapon_StealthTank_Player");
				DefenseObjectDefClass &Defense = const_cast<DefenseObjectDefClass&>(Def->Get_DefenseObjectDef());
				Defense.Skin = ArmorWarheadManager::Get_Armor_Type("CNCVehicleHeavy");
				Defense.ShieldType = ArmorWarheadManager::Get_Armor_Type("CNCVehicleHeavy");
				Defense.ShieldStrengthMax = 150;
				Defense.ShieldStrength = 150;
				Vehicles.Add(Def);
			}
			Def = (VehicleGameObjDef*)Find_Named_Definition("Nod_SSM_Launcher_Player"); //Special case for SSM.
			if (Def) {
				Def->WeaponDefID = 0;
				DefenseObjectDefClass &Defense = const_cast<DefenseObjectDefClass&>(Def->Get_DefenseObjectDef());
				Defense.Skin = ArmorWarheadManager::Get_Armor_Type("CNCVehicleHeavy");
				Defense.ShieldType = ArmorWarheadManager::Get_Armor_Type("CNCVehicleHeavy");
				Defense.ShieldStrengthMax = 125;
				Defense.ShieldStrength = 125;
				Vehicles.Add(Def);
			}
		}
		
		//Drop point
		DASettingsManager::Get_Vector3(Position[0],"Nod_Vehicle_Drop",Vector3());
		DASettingsManager::Get_Vector3(Position[1],"GDI_Vehicle_Drop",Vector3());
		Facing[0] = DASettingsManager::Get_Float("Nod_Vehicle_Drop_Facing",0.0f);
		Facing[1] = DASettingsManager::Get_Float("GDI_Vehicle_Drop_Facing",0.0f);
	}

	virtual bool Can_Activate(cPlayer *Player) { //No drop points or vehicles on this map.
		return (Vehicles.Count() && (Position[0].X || Position[0].Y || Position[0].Z) && (Position[1].X || Position[1].Y || Position[1].Z));
	}
	
	virtual void Activate(cPlayer *Player) {
		int Rand = Get_Random_Int(0,Vehicles.Count()); //Get random vehicle.
		const VehicleGameObjDef *Def = Vehicles[Rand];
		VehicleGameObj *Vehicle = (VehicleGameObj*)Create_Object(Def,Vector3());
		Vehicle->Lock_Vehicle(Player->Get_GameObj(),44.0f);
		DAVehicleManager::Air_Drop_Vehicle(Player->Get_Team(),Vehicle,Position[Player->Get_Team()],Facing[Player->Get_Team()]);
		Player->Get_GameObj()->Set_Position(Position[Player->Get_Team()]);
		Fix_Stuck_Objects(Position[Player->Get_Team()],10.0f,15.0f);
		DA::Host_Message("Looks like %ls just got a random vehicle. Go them!",Get_Wide_Team_Name(Player->Get_Team()));
		DA::Page_Player(Player,"You have received %s from the Random Vehicle Crate. It will be dropped at your position momentarily.",a_or_an_Prepend(DATranslationManager::Translate(Def)));
	}
	DynamicVectorClass<const VehicleGameObjDef*> Vehicles;
	Vector3 Position[2];
	float Facing[2];
};

Register_Crate(DARandomVehicleCrateClass,"Random Vehicle",DACrateType::INFANTRY);

/********************************************************************************************************************************/

class DAArmsDealerCrateClass : public DACrateClass {
	virtual void Settings_Loaded() {
		DACrateClass::Settings_Loaded();

		//Weapons
		Weapons.Delete_All();
		for (int Team = 0;Team < 2;Team++) { //Add all main weapons to vector.
			for (int Type = 0;Type < 7;Type++) {
				PurchaseSettingsDefClass *PT = PurchaseSettingsDefClass::Find_Definition((PurchaseSettingsDefClass::TYPE)Type,(PurchaseSettingsDefClass::TEAM)Team);
				if (PT) {
					for (int i = 0;i < 10;i++) {
						SoldierGameObjDef *Soldier = (SoldierGameObjDef*)Find_Definition(PT->Get_Definition(i));
						if (Soldier && Soldier->Get_Class_ID() == CID_Soldier) {
							WeaponDefinitionClass *Weapon = (WeaponDefinitionClass*)Find_Definition(Soldier->WeaponDefID);
							if (Weapon) {
								Weapons.Add(Weapon);
							}
						}
					}
				}
			}
		}
	}
	
	virtual bool Can_Activate(cPlayer *Player) { //Don't trigger if stealth.
		return (Weapons.Count() && !Player->Get_GameObj()->Is_Stealth_Enabled());
	}

	virtual void Activate(cPlayer *Player) {
		WeaponBagClass *Bag = Player->Get_GameObj()->Get_Weapon_Bag();
		for (int i = 0;i < Weapons.Count();i++) {
			Bag->Add_Weapon(Weapons[i],999);
		}
		DA::Page_Player(Player,"You have been given all weapons by the Arms Dealer Crate.");
	}
	DynamicVectorClass<WeaponDefinitionClass*> Weapons;
};

Register_Crate(DAArmsDealerCrateClass,"Arms Dealer",DACrateType::INFANTRY);

/********************************************************************************************************************************/

class DADemolitionKitCrateClass : public DACrateClass {
	virtual bool Can_Activate(cPlayer *Player) { //Don't trigger if stealth.
		return !Player->Get_GameObj()->Is_Stealth_Enabled();
	}

	virtual void Activate(cPlayer *Player) {
		WeaponBagClass *Bag = Player->Get_GameObj()->Get_Weapon_Bag();
		Bag->Add_Weapon("Weapon_MineProximity_Player",999);
		Bag->Add_Weapon("CnC_Weapon_MineRemote_Player",999);
		Bag->Add_Weapon("CnC_Weapon_MineRemote_Player_2Max",999);
		Bag->Add_Weapon("CnC_Weapon_MineTimed_Player",999);
		Bag->Add_Weapon("CnC_Weapon_MineTimed_Player_2Max",999);
		DA::Page_Player(Player,"You have received 3 Timed C4, 3 Remote C4, and 6 Proximity C4 from the Demolition Kit Crate. Use them wisely.");
	}
};

Register_Crate(DADemolitionKitCrateClass,"Demolition Kit",DACrateType::INFANTRY);

/********************************************************************************************************************************/

/*class DABeaconCrateClass : public DACrateClass {
	virtual bool Can_Activate(cPlayer *Player) { //Don't trigger if no beacon is available.
		TeamPurchaseSettingsDefClass *PT = TeamPurchaseSettingsDefClass::Get_Definition((TeamPurchaseSettingsDefClass::TEAM)PTTEAM(Player->Get_Team()));
		return PT->Get_Beacon_Definition();
	}

	virtual void Activate(cPlayer *Player) {
		TeamPurchaseSettingsDefClass *PT = TeamPurchaseSettingsDefClass::Get_Definition((TeamPurchaseSettingsDefClass::TEAM)PTTEAM(Player->Get_Team()));
		WeaponDefinitionClass *Beacon = (WeaponDefinitionClass*)Find_Definition(PT->Get_Beacon_Definition());
		Player->Get_GameObj()->Get_Weapon_Bag()->Add_Weapon(Beacon,1);
		DA::Page_Player(Player,"You have received %s from the Beacon Crate.",a_or_an_Prepend(DATranslationManager::Translate(Beacon)));
	}
};

Register_Crate(DABeaconCrateClass,"Beacon",DACrateType::INFANTRY);*/

/********************************************************************************************************************************/

class DAAdrenalineCrateObserverClass : public DAGameObjObserverClass {
	virtual const char *Get_Name() { 
		return "DAAdrenalineCrateObserverClass"; 
	}

	virtual void Init() {
		((SoldierGameObj*)Get_Owner())->Set_Max_Speed(((SoldierGameObj*)Get_Owner())->Get_Max_Speed()*2);
		Start_Timer(1,Get_Random_Float(60.0f,90.0f)); //End
	}

	virtual void Timer_Expired(GameObject *obj,int Number) {
		((SoldierGameObj*)Get_Owner())->Set_Max_Speed(((SoldierGameObj*)Get_Owner())->Get_Max_Speed()/2);
		Set_Delete_Pending();
	}
};

class DAAdrenalineCrateClass : public DACrateClass {
	virtual bool Can_Activate(cPlayer *Player) { //Prevent crate from triggering if already in effect.
		return !Player->Get_GameObj()->Has_Observer("DAAdrenalineCrateObserverClass");
	}

	virtual void Activate(cPlayer *Player) {
		Player->Get_GameObj()->Add_Observer(new DAAdrenalineCrateObserverClass);
		DA::Page_Player(Player,"The Adrenaline Crate has temporarily doubled your run speed.");
	}
};

Register_Crate(DAAdrenalineCrateClass,"Adrenaline",DACrateType::INFANTRY);

/********************************************************************************************************************************/

/*class DAButterFingersCrateClass : public DACrateClass {
	virtual void Activate(cPlayer *Player) {
		Commands->Clear_Weapons(Player->Get_GameObj());
		DA::Page_Player(Player,"You just picked up the Butter Fingers Crate. You have dropped all of your weapons.");
	}
};

Register_Crate(DAButterFingersCrateClass,"Butter Fingers",DACrateType::INFANTRY);*/

/********************************************************************************************************************************/

class DAStealthCrateClass : public DACrateClass, public DAEventClass {
	virtual bool Can_Activate(cPlayer *Player) { //Don't trigger if already stealth.
		return !Player->Get_GameObj()->Is_Stealth_Enabled();
	}
	
	virtual void Activate(cPlayer *Player) {
		Commands->Enable_Stealth(Player->Get_GameObj(),true);
		DA::Page_Player(Player,"You have been equipped with a stealth suit by the Stealth Crate.");
	}
};

Register_Crate(DAStealthCrateClass,"Stealth",DACrateType::INFANTRY);

/********************************************************************************************************************************/

class DATiberiumMutantCrateObserverClass : public DAGameObjObserverClass {
	virtual const char *Get_Name() { 
		return "DATiberiumMutantCrateObserverClass"; 
	}

	virtual void Init() {
		const_cast<Matrix3D&>(((SoldierGameObj*)Get_Owner())->Get_Transform()).Adjust_Z_Translation(1.0f); //Fix stuck in ground.
		WeaponBagClass *Bag = ((SoldierGameObj*)Get_Owner())->Get_Weapon_Bag();
		Bag->Clear_Weapons();
		Bag->Select_Weapon(Bag->Add_Weapon("CNC_Weapon_ChemSprayer_Player",999)); //Can only use chem sprayer.
		Start_Timer(1,1.0f);
	}

	virtual void Timer_Expired(GameObject *obj,int Number) {
		Start_Timer(1,1.0f);
		DefenseObjectClass *Defense = ((SoldierGameObj*)Get_Owner())->Get_Defense_Object(); //Small health regen.
		if (Defense->Get_Health_Max() == Defense->Get_Health()) {
			Defense->Add_Shield_Strength(1.0f);
		}
		else {
			Defense->Add_Health(1.0f);
		}
		Set_Bullets(Get_Owner(),"CNC_Weapon_ChemSprayer_Player",100); //Infinite ammo, no reload.
		Vector3 Position;
		Get_Owner()->Get_Position(&Position);
		for (SLNode<SoldierGameObj> *x = GameObjManager::SoldierGameObjList.Head();x;x = x->Next()) { //AOE damage.
			if (x->Data() != Get_Owner() && Commands->Get_Distance(Position,x->Data()->Get_Position()) < 5.0f) {
				Commands->Apply_Damage(x->Data(),1.0f,"TiberiumRaw",Get_Owner());
			}
		}
	}

	virtual bool Vehicle_Entry_Request(VehicleGameObj *Vehicle,int &Seat) {
		return false; //Can't enter vehicles.
	}

	virtual bool Add_Weapon_Request(const WeaponDefinitionClass *Weapon) {
		return !_stricmp(Weapon->Get_Name(),"CNC_Weapon_ChemSprayer_Player"); //Can only use chem sprayer.
	}
	
	virtual bool Damage_Received_Request(OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone) {
		if (Type == DADamageType::SQUISH || Type == DADamageType::FALL) {
			return false; //Can't be squished and doesn't take fall damage.
		}
		else if (Offense->Get_Damage() > 0) {
			Offense->Scale_Damage(0.5f); //Only takes half damage.
		}
		return true;
	}

	virtual void Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
		Get_Owner()->Set_Delete_Pending(); //Fix bug where the player won't respawn when they die because the visceroid doesn't have a death animation.
	}
};

class DATiberiumMutantCrateClass : public DACrateClass {
	virtual void Settings_Loaded() {
		DACrateClass::Settings_Loaded();

		//Setup Visceroid preset. The already existing presets can't be used because they don't have hands.
		SoldierGameObjDef *Mutant = (SoldierGameObjDef*)Find_Named_Definition("CnC_GDI_Mutant_2SF_Templar");
		SoldierGameObjDef *Visceroid = (SoldierGameObjDef*)Find_Named_Definition("CnC_Visceroid");
		if (Mutant && Visceroid) {
			Mutant->WeaponDefID = 0;
			Mutant->ScriptNameList.Delete_All();
			Mutant->ScriptParameterList.Delete_All();
			DefenseObjectDefClass *Defense = &const_cast<DefenseObjectDefClass&>(Mutant->Get_DefenseObjectDef());
			int Skin = ArmorWarheadManager::Get_Armor_Type("SkinMutant");
			Defense->Skin = Skin;
			Defense->ShieldType = Skin;
			Defense->HealthMax = 500.0f;
			Defense->Health = 500.0f;
			Defense->ShieldStrengthMax = 0.0f;
			Defense->DamagePoints = 0.05f;
			Defense->DeathPoints = 100.0f;
			Mutant->PhysDefID = Visceroid->PhysDefID;
		}
	}

	virtual bool Can_Activate(cPlayer *Player) { //Prevent crate from triggering if already in effect.
		return !Player->Get_GameObj()->Has_Observer("DATiberiumMutantCrateObserverClass");
	}

	virtual void Activate(cPlayer *Player) {
		SoldierGameObjDef *Mutant = (SoldierGameObjDef*)Find_Named_Definition("CnC_GDI_Mutant_2SF_Templar");
		if (Mutant) {
			Player->Get_GameObj()->Re_Init(*Mutant);
			Player->Get_GameObj()->Post_Re_Init();
		}
		Player->Get_GameObj()->Add_Observer(new DATiberiumMutantCrateObserverClass);
		DA::Host_Message("A %ls player has been mutated by the Tiberium Mutant Crate!",Get_Wide_Team_Name(Player->Get_Team()));
	}
};

Register_Crate(DATiberiumMutantCrateClass,"Tiberium Mutant",DACrateType::INFANTRY);

/********************************************************************************************************************************/

class DAHumanSiloCrateObserverClass : public DAGameObjObserverClass {
	virtual const char *Get_Name() { 
		return "DAHumanSiloCrateObserverClass"; 
	}

	virtual void Init() {
		Total = 0;
	}

	virtual void Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
		if (Type == DADamageType::TIBERIUM || Warhead == 9 || Warhead == 10 || Warhead == 11) { //Tiberium based warheads.
			Give_Credits_Team(((SoldierGameObj*)Get_Owner())->Get_Player_Type(),Damage/2);
			Total += Damage;
			DefenseObjectClass *Defense = ((SoldierGameObj*)Get_Owner())->Get_Defense_Object();
			if (Total >= (Defense->Get_Health_Max()+Defense->Get_Shield_Strength_Max())) { //Wear off after taking tiberium damage equal to character's health.
				Set_Delete_Pending();
			}
		}
	}

	float Total;
};

class DAHumanSiloCrateClass : public DACrateClass {
	virtual bool Can_Activate(cPlayer *Player) { //Prevent crate from triggering if already in effect.
		return !Player->Get_GameObj()->Has_Observer("DAHumanSiloCrateObserverClass");
	}
	virtual void Activate(cPlayer *Player) {
		Player->Get_GameObj()->Add_Observer(new DAHumanSiloCrateObserverClass);
		DA::Page_Player(Player,"You picked up the Human Silo Crate. Any tiberium damage you take will be converted into credits for your team.");
	}
};

Register_Crate(DAHumanSiloCrateClass,"Human Silo",DACrateType::INFANTRY);

/********************************************************************************************************************************/