/*	Renegade Scripts.dll
    Dragonade Soldier Manager
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
#include "da_soldier.h"
#include "da_settings.h"
#include "da_translation.h"
#include "da_log.h"
#include "TeamPurchaseSettingsDefClass.h"

void DASoldierManager::Init() {
	static DASoldierManager Instance;
	Instance.Register_Event(DAEvent::SETTINGSLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::CHARACTERPURCHASEREQUEST,INT_MIN);
	Instance.Register_Event(DAEvent::ADDWEAPONREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::ADDWEAPON,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::PLAYER,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DESTROYED,DAObjectEvent::PLAYER,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::SOLDIER,INT_MAX);
}

void DASoldierManager::Settings_Loaded_Event() {
	//Spawn characters
	StringClass Buffer;
	Set_Nod_Soldier_Name(DASettingsManager::Get_String(Buffer,"NodSpawnCharacter",Get_Definition_Name(TeamPurchaseSettingsDefClass::Get_Definition(TeamPurchaseSettingsDefClass::TEAM_NOD)->Get_Enlisted_Definition(0))));
	Set_GDI_Soldier_Name(DASettingsManager::Get_String(Buffer,"GDISpawnCharacter",Get_Definition_Name(TeamPurchaseSettingsDefClass::Get_Definition(TeamPurchaseSettingsDefClass::TEAM_GDI)->Get_Enlisted_Definition(0))));

	//Grant weapons
	GrantWeapons.Remove_All();
	INISection *Section = DASettingsManager::Get_Section("Grant_Weapons");
	if (Section) {
		for (int i = 0;i < Section->Count();i++) {
			INIEntry *Entry = Section->Peek_Entry(i);
			DefinitionClass *Def = Find_Named_Definition(Entry->Entry);
			if (Def && Def->Get_Class_ID() == CID_Soldier) {
				DynamicVectorClass<const WeaponDefinitionClass*> Weapons;
				DATokenParserClass Parser(Entry->Value,'|');
				while (const char *Token = Parser.Get_String()) {
					DefinitionClass *Def2 = Find_Named_Definition(Token);
					if (Def2 && Def2->Get_Class_ID() == CID_Weapon) {
						Weapons.Add((const WeaponDefinitionClass*)Def2);
					}
				}
				if (Weapons.Count()) {
					GrantWeapons.Insert((unsigned int)Def,Weapons);
				}
			}
		}
	}

	//Exclusive weapons
	ExclusiveWeapons.Remove_All();
	Section = DASettingsManager::Get_Section("Exclusive_Weapons");
	if (Section) {
		for (int i = 0;i < Section->Count();i++) {
			INIEntry *Entry = Section->Peek_Entry(i);
			DefinitionClass *Def = Find_Named_Definition(Entry->Entry);
			if (Def && Def->Get_Class_ID() == CID_Weapon) {
				DynamicVectorClass<const WeaponDefinitionClass*> Weapons;
				DATokenParserClass Parser(Entry->Value,'|');
				while (const char *Token = Parser.Get_String()) {
					DefinitionClass *Def2 = Find_Named_Definition(Token);
					if (Def2 && Def2->Get_Class_ID() == CID_Weapon) {
						Weapons.Add((const WeaponDefinitionClass*)Def2);
					}
				}
				if (Weapons.Count()) {
					ExclusiveWeapons.Insert((unsigned int)Def,Weapons);
				}
			}
		}
	}

	//Replace weapons
	Section = DASettingsManager::Get_Section("Replace_Weapons");
	if (Section) {
		for (int i = 0;i < Section->Count();i++) {
			INIEntry *Entry = Section->Peek_Entry(i);
			DefinitionClass *OldWeaponDef = Find_Named_Definition(Entry->Entry);
			DefinitionClass *NewWeaponDef = Find_Named_Definition(Entry->Value);
			if (OldWeaponDef && OldWeaponDef->Get_Class_ID() == CID_Weapon && NewWeaponDef && NewWeaponDef->Get_Class_ID() == CID_Weapon) {
				for (PowerUpGameObjDef *PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_First(CID_PowerUp);PowerUpDef;PowerUpDef = (PowerUpGameObjDef*)DefinitionMgrClass::Get_Next(PowerUpDef,CID_PowerUp)) {
					if ((unsigned int)PowerUpDef->GrantWeaponID == OldWeaponDef->Get_ID()) {
						PowerUpDef->GrantWeaponID = NewWeaponDef->Get_ID();
					}
				}
				for (SoldierGameObjDef *SoldierDef = (SoldierGameObjDef*)DefinitionMgrClass::Get_First(CID_Soldier);SoldierDef;SoldierDef = (SoldierGameObjDef*)DefinitionMgrClass::Get_Next(SoldierDef,CID_Soldier)) {
					if ((unsigned int)SoldierDef->WeaponDefID == OldWeaponDef->Get_ID()) {
						SoldierDef->WeaponDefID = NewWeaponDef->Get_ID();
					}
					if ((unsigned int)SoldierDef->SecondaryWeaponDefID == OldWeaponDef->Get_ID()) {
						SoldierDef->SecondaryWeaponDefID = NewWeaponDef->Get_ID();
					}
				}
			}
		}
	}

	//Remove weapons
	RemoveWeapons.Remove_All();
	Section = DASettingsManager::Get_Section("Remove_Weapons");
	if (Section) {
		for (int i = 0;i < Section->Count();i++) {
			INIEntry *Entry = Section->Peek_Entry(i);
			DefinitionClass *Def = Find_Named_Definition(Entry->Entry);
			if (Def && Def->Get_Class_ID() == CID_Weapon) {
				DynamicVectorClass<const WeaponDefinitionClass*> Weapons;
				DATokenParserClass Parser(Entry->Value,'|');
				while (const char *Token = Parser.Get_String()) {
					DefinitionClass *Def2 = Find_Named_Definition(Token);
					if (Def2 && Def2->Get_Class_ID() == CID_Weapon) {
						Weapons.Add((const WeaponDefinitionClass*)Def2);
					}
				}
				if (Weapons.Count()) {
					RemoveWeapons.Insert((unsigned int)Def,Weapons);
				}
			}
		}
	}
}

void DASoldierManager::Object_Created_Event(GameObject *obj) {
	DynamicVectorClass<const WeaponDefinitionClass*> *Weapons = GrantWeapons.Get((unsigned int)&obj->Get_Definition());
	if (Weapons) {
		WeaponBagClass *Bag = ((SoldierGameObj*)obj)->Get_Weapon_Bag();
		for (int i = 0;i < Weapons->Count();i++) {
			Bag->Add_Weapon(Weapons->operator[](i),999,true);
		}
	}
}

void DASoldierManager::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,float Scale,DADamageType::Type Type) {
	StringClass Message;
	if (((SoldierGameObj*)Victim)->Get_Player()) { //Player
		if (!Killer) { //No killer
			if (Type == DADamageType::FALL) {
				Message.Format("%d %ls had a great fall (%s)",Victim->Get_Player_Type(),Get_Wide_Player_Name(Victim),DATranslationManager::Translate_Soldier(Victim));
			}
			else if (Type == DADamageType::TIBERIUM) {
				Message.Format("%d %ls took a stroll through the Tiberium (%s)",Victim->Get_Player_Type(),Get_Wide_Player_Name(Victim),DATranslationManager::Translate_Soldier(Victim));
			}
			else {
				Message.Format("%d %ls was killed (%s)",Victim->Get_Player_Type(),Get_Wide_Player_Name(Victim),DATranslationManager::Translate_Soldier(Victim));
			}
		}
		else if (Killer->As_SoldierGameObj()) { //Killed by soldier
			if (((SoldierGameObj*)Killer)->Get_Player()) { //Killed by player
				if (Victim == Killer) { //Suicide
					if (Type == DADamageType::EXPLOSION) {
						Message.Format("%d %ls killed themself (%s)",Victim->Get_Player_Type(),Get_Wide_Player_Name(Victim),DATranslationManager::Translate(GetExplosionObj()));
					}
					else {
						Message.Format("%d %ls killed themself (%s)",Victim->Get_Player_Type(),Get_Wide_Player_Name(Victim),DATranslationManager::Translate_Soldier(Victim));
					}
				}
				else {
					Create_2D_WAV_Sound_Player(Killer,"correction_3.wav");
					if (Type == DADamageType::SQUISH) {
						((SoldierGameObj*)Killer)->Get_Player()->Squishes++;
						Message.Format("%d %ls killed %ls SQUISH! (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),Get_Wide_Player_Name(Victim),DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate_Soldier(Victim));
					}
					else if (Type == DADamageType::HEADSHOT || Type == DADamageType::NECKSHOT) {
						Message.Format("%d %ls killed %ls HEADSHOT! (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),Get_Wide_Player_Name(Victim),DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate_Soldier(Victim));
					}
					else if (Type == DADamageType::EXPLOSION) {
						Message.Format("%d %ls killed %ls (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),Get_Wide_Player_Name(Victim),DATranslationManager::Translate(GetExplosionObj()),DATranslationManager::Translate_Soldier(Victim));
					}
					else {
						Message.Format("%d %ls killed %ls (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),Get_Wide_Player_Name(Victim),DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate_Soldier(Victim));
					}
				}
			}
			else { //Killed by bot
				if (Type == DADamageType::HEADSHOT || Type == DADamageType::NECKSHOT) {
					Message.Format("%d %s killed %ls HEADSHOT! (%s VS. %s)",Killer->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate(Killer)),Get_Wide_Player_Name(Victim),DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate_Soldier(Victim));
				}
				else {
					Message.Format("%d %s killed %ls (%s VS. %s)",Killer->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate(Killer)),Get_Wide_Player_Name(Victim),DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate_Soldier(Victim));
				}
			}
		}
		else if (Killer->As_VehicleGameObj()) { //Killed by vehicle. Could be harvester, defense, or AI vehicle.
			StringClass KillerTranslation;
			if (Killer->Get_Defense_Object()->Get_Shield_Type() == 1 || Is_Harvester_Preset(Killer)) {
				KillerTranslation = "The " + DATranslationManager::Translate_With_Team_Name(Killer);
			}
			else if (((VehicleGameObj*)Killer)->Is_Turret()) {
				KillerTranslation = A_Or_An_Prepend(DATranslationManager::Translate_With_Team_Name(Killer));
			}
			else {
				KillerTranslation = A_Or_An_Prepend(DATranslationManager::Translate(Killer));
			}
			if (Type == DADamageType::SQUISH) {
				Message.Format("%d %s killed %ls SQUISH! (%s %VS. %s)",Killer->Get_Player_Type(),KillerTranslation,Get_Wide_Player_Name(Victim),DATranslationManager::Translate(Killer),DATranslationManager::Translate_Soldier(Victim));
			}
			else if (Type == DADamageType::HEADSHOT || Type == DADamageType::NECKSHOT) {
				Message.Format("%d %s killed %ls HEADSHOT! (%s VS. %s)",Killer->Get_Player_Type(),KillerTranslation,Get_Wide_Player_Name(Victim),DATranslationManager::Translate(Killer),DATranslationManager::Translate_Soldier(Victim));
			}
			else {
				Message.Format("%d %s killed %ls (%s VS. %s)",Killer->Get_Player_Type(),KillerTranslation,Get_Wide_Player_Name(Victim),DATranslationManager::Translate(Killer),DATranslationManager::Translate_Soldier(Victim));
			}
		}
		else { //Killed by something else
			Message.Format("%d %ls was killed (%s)",Victim->Get_Player_Type(),Get_Wide_Player_Name(Victim),DATranslationManager::Translate_Soldier(Victim));
		}
		DALogManager::Write_Log("_PLAYERKILL","%s",Message);
	}
	else { //Bot
		if (!Killer) { //No killer
			Message.Format("%d %s was killed (%s)",Victim->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate(Victim)),DATranslationManager::Translate_Soldier(Victim));
		}
		else if (Killer->As_SoldierGameObj()) { //Killed by soldier
			if (((SoldierGameObj*)Killer)->Get_Player()) { //Killed by player
				if (Type == DADamageType::SQUISH) {
					((SoldierGameObj*)Killer)->Get_Player()->Squishes++;
					Message.Format("%d %ls killed %s SQUISH! (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),a_or_an_Prepend(DATranslationManager::Translate(Victim)),DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate_Soldier(Victim));
				}
				else if (Type == DADamageType::HEADSHOT || Type == DADamageType::NECKSHOT) {
					Message.Format("%d %ls killed %s HEADSHOT! (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),a_or_an_Prepend(DATranslationManager::Translate(Victim)),DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate_Soldier(Victim));
				}
				else if (Type == DADamageType::EXPLOSION) {
					Message.Format("%d %ls killed %s (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),a_or_an_Prepend(DATranslationManager::Translate(Victim)),DATranslationManager::Translate(GetExplosionObj()),DATranslationManager::Translate_Soldier(Victim));
				}
				else {
					Message.Format("%d %ls killed %s (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),a_or_an_Prepend(DATranslationManager::Translate(Victim)),DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate_Soldier(Victim));
				}
			}
			else if (Victim == Killer) { //Suicide
				Message.Format("%d %s killed itself (%s)",Killer->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate(Victim)),DATranslationManager::Translate_Soldier(Victim));
			}
			else { //Killed by bot
				if (Type == DADamageType::HEADSHOT || Type == DADamageType::NECKSHOT) {
					Message.Format("%d %s killed %s HEADSHOT! (%s VS. %s)",Killer->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate(Killer)),a_or_an_Prepend(DATranslationManager::Translate(Victim)),DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate_Soldier(Victim));
				}
				else {
					Message.Format("%d %s killed %s (%s VS. %s)",Killer->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate(Killer)),a_or_an_Prepend(DATranslationManager::Translate(Victim)),DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate_Soldier(Victim));
				}
			}
		}
		else if (Killer->As_VehicleGameObj()) { //Killed by vehicle. Could be harvester, defense, or AI vehicle.
			StringClass KillerTranslation;
			if (Killer->Get_Defense_Object()->Get_Shield_Type() == 1 || Is_Harvester_Preset(Killer)) {
				KillerTranslation = "The " + DATranslationManager::Translate_With_Team_Name(Killer);
			}
			else if (((VehicleGameObj*)Killer)->Is_Turret()) {
				KillerTranslation = A_Or_An_Prepend(DATranslationManager::Translate_With_Team_Name(Killer));
			}
			else {
				KillerTranslation = A_Or_An_Prepend(DATranslationManager::Translate(Killer));
			}
			if (Type == DADamageType::SQUISH) {
				Message.Format("%d %s killed %s SQUISH! (%s VS. %s)",Killer->Get_Player_Type(),KillerTranslation,a_or_an_Prepend(DATranslationManager::Translate(Victim)),DATranslationManager::Translate(Killer),DATranslationManager::Translate_Soldier(Victim));
			}
			else if (Type == DADamageType::HEADSHOT || Type == DADamageType::NECKSHOT) {
				Message.Format("%d %s killed %s HEADSHOT! (%s VS. %s)",Killer->Get_Player_Type(),KillerTranslation,a_or_an_Prepend(DATranslationManager::Translate(Victim)),DATranslationManager::Translate(Killer),DATranslationManager::Translate_Soldier(Victim));
			}
			else {
				Message.Format("%d %s killed %s (%s VS. %s)",Killer->Get_Player_Type(),KillerTranslation,a_or_an_Prepend(DATranslationManager::Translate(Victim)),DATranslationManager::Translate(Killer),DATranslationManager::Translate_Soldier(Victim));
			}
		}
		DALogManager::Write_Log("_BOTKILL","%s",Message);
	}
}

//Default character purchase handler.
int DASoldierManager::Character_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const SoldierGameObjDef *Item) {
	if (Base->Can_Generate_Soldiers() || !Cost) {
		if (Player->Purchase_Item((int)Cost)) {
			Player->Get_GameObj()->Re_Init(*Item);
			Player->Get_GameObj()->Post_Re_Init();
			return 0;
		}
		return 2;
	}
	return 3;
}

bool DASoldierManager::Add_Weapon_Request_Event(cPlayer *Player,const WeaponDefinitionClass *Weapon) {
	DynamicVectorClass<const WeaponDefinitionClass*> *ExWeapons = ExclusiveWeapons.Get((unsigned int)Weapon);
	if (ExWeapons) {
		WeaponBagClass *Bag = Player->Get_GameObj()->Get_Weapon_Bag();
		for (int i = 0;i < ExWeapons->Count();i++) {
			if (Bag->Find_Weapon(ExWeapons->operator[](i))) { //Block grant if they have any of these weapons.
				return false;
			}
		}
	}
	return true;
}

void DASoldierManager::Add_Weapon_Event(cPlayer *Player,WeaponClass *Weapon) {
	DynamicVectorClass<const WeaponDefinitionClass*> *RemWeapons = RemoveWeapons.Get((unsigned int)Weapon->Get_Definition());
	if (RemWeapons) {
		WeaponBagClass *Bag = Player->Get_GameObj()->Get_Weapon_Bag();
		bool Select = false;
		for (int i = 0;i < RemWeapons->Count();i++) { //Remove these weapons.
			int Index = Bag->Get_Weapon_Position(RemWeapons->operator[](i));
			if (Index) {
				if (Index == Bag->Get_Index()) {
					Select = true;
				}
				Bag->Remove_Weapon(Index);
			}
		}
		if (Select) { //Select new weapon if current weapon was removed.
			Bag->Select_Weapon(Weapon);
		}
	}
}
