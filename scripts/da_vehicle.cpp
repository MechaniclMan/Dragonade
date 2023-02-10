/*	Renegade Scripts.dll
    Dragonade Vehicle Manager
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
#include "engine_da.h"
#include "da.h"
#include "da_vehicle.h"
#include "da_translation.h"
#include "da_log.h"
#include "da_settings.h"
#include "da_damagelog.h"
#include "VehicleFactoryGameObj.h"
#include "AirFactoryGameObj.h"
#include "NavalFactoryGameObj.h"
#include "MoveablePhysDefClass.h"
#include "RefineryGameObj.h"
#include "GameObjManager.h"

DAVehicleManager DAVehicleManager::Instance;
static const char *ObserverName = "DAVehicleObserverClass";

void DAVehicleObserverClass::Init() {
	LastDriver = 0;
	LastPlayer = 0;
	LastDriverExitTime = 0;
	LastExitTime = 0;
	Team = Get_Vehicle()->Get_Definition().Get_Default_Player_Type();
	GiveSteal = true;
}

void DAVehicleObserverClass::Timer_Expired(GameObject *obj,int Number) {
	if (Number == 1) { //This is done in a timer instead of the Vehicle_Enter event to make sure that everything has a chance to check for vehicle stealing before the new team is set.
		Team = Get_Vehicle()->Get_Player_Type();
	}
	else {
		GiveSteal = true;
	}
}

void DAVehicleObserverClass::Vehicle_Enter(SoldierGameObj *Soldier,int Seat) {
	if (GiveSteal && Team != -2 && Team != Soldier->Get_Player_Type()) { //Reward players for stealing vehicles. 20 second timer to prevent exploits.
		Soldier->Get_Player()->Increment_Score(Get_Vehicle()->Get_Defense_Object()->Get_Death_Points());
		GiveSteal = false;
		Start_Timer(2,20.0f);
	}
	Start_Timer(1,0.1f);
}

void DAVehicleObserverClass::Vehicle_Exit(SoldierGameObj *Soldier,int Seat) {
	if (Seat == 0) {
		LastDriver = Soldier->Get_Player();
		LastDriverExitTime = The_Game()->Get_Game_Duration_S();
	}
	LastPlayer = Soldier->Get_Player();
	LastExitTime = The_Game()->Get_Game_Duration_S();
}

const char *DAVehicleObserverClass::Get_Name() {
	return ObserverName;
}

void DAVehicleManager::Init() {
	Instance.Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::VEHICLE,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::VEHICLE | DAObjectEvent::BUILDING,INT_MAX);
	Instance.Register_Event(DAEvent::SETTINGSLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEPURCHASEREQUEST,INT_MAX);

	static DefaultPurchaseEvent Instance2;
	Instance2.Register_Event(DAEvent::VEHICLEPURCHASEREQUEST,INT_MIN);
}

bool DAVehicleManager::Check_Limit_For_Player(SoldierGameObj *Player) {
	return !((unsigned int)Get_Ground_Vehicle_Count(Player->Get_Player_Type()) >= Get_Vehicle_Limit());
}

DAVehicleObserverClass *DAVehicleManager::Get_Vehicle_Data(GameObject *obj) {
	const SimpleDynVecClass<GameObjObserverClass*> &Observers = obj->Get_Observers();
	for (int i = 0;i < Observers.Count();i++) {
		if (Observers[i]->Get_Name() == ObserverName) {
			return (DAVehicleObserverClass*)Observers[i];
		}
	}
	return 0;
}

void DAVehicleManager::Disable_Flip_Kill() {
	Instance.Register_Event(DAEvent::VEHICLEFLIP);
}

void DAVehicleManager::Enable_Flip_Kill() {
	Instance.Unregister_Event(DAEvent::VEHICLEFLIP);
}

void DAVehicleManager::Settings_Loaded_Event() {
	Set_Vehicle_Limit((unsigned int)DASettingsManager::Get_Int("VehicleLimit",8));
	if (DASettingsManager::Get_Bool("DisableVehicleFlipKill",false)) {
		Disable_Flip_Kill();
	}
	else {
		Enable_Flip_Kill();
	}
}

void DAVehicleManager::Object_Created_Event(GameObject *obj) {
	obj->Add_Observer(new DAVehicleObserverClass);
	int Team = ((VehicleGameObj*)obj)->Get_Definition().Get_Default_Player_Type();
	if (Team == 0 || Team == 1) {
		RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_REFINERY);
		if (Ref) {
			if ((unsigned int)Ref->Get_Harvester_Def_ID() == obj->Get_Definition().Get_ID()) {
				if (Ref->Is_Destroyed()) { //Fix bug where the GDI Harvester can still exist if it is building when the Refinery is killed.
					obj->Set_Delete_Pending();
				}
				else {
					((VehicleGameObj*)obj)->Set_Is_Scripts_Visible(false); //Fix defenses shooting at harvester on some maps.
				}
			}
		}
	}
}

//Limit checking is done here for per-player and per-team limits(NYI)
int DAVehicleManager::Vehicle_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const VehicleGameObjDef *Item) {
	VehicleFactoryGameObj *VF = (VehicleFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
	AirFactoryGameObj *AF = (AirFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_HELIPAD);
	NavalFactoryGameObj *NF = (NavalFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_NAVAL_FACTORY);
	if (AF && Item->Get_Type() == VEHICLE_TYPE_FLYING) { //Flying vehicle
		if (!AF->Is_Available()) {
			return 3;
		}
		else if ((unsigned int)Get_Air_Vehicle_Count(Base->Get_Player_Type()) >= Get_Air_Vehicle_Limit()) {
			return 4;
		}
		else {
			return -1;
		}
	}
	else if (NF && Item->Get_Type() == VEHICLE_TYPE_BOAT || Item->Get_Type() == VEHICLE_TYPE_SUB) { //Naval vehicle
		if (!NF || !NF->Is_Available() || !NF->Can_Spawn(Item->Get_ID())) {
			return 3;
		}
		else if ((unsigned int)Get_Naval_Vehicle_Count(Base->Get_Player_Type()) >= Get_Naval_Vehicle_Limit()) {
			return 4;
		}
		else {
			return -1;
		}
	}
	else if (VF) { //Ground vehicle
		if (!VF->Is_Available()) {
			return 3;
		}
		else if (!Check_Limit_For_Player(Purchaser)) {
			return 4;
		}
		else {
			return -1;
		}
	}
	return 3;
}

//Default vehicle purchase handler.
//Seperate from the above so the vehicle purchase event can be overloaded.
int DAVehicleManager::DefaultPurchaseEvent::Vehicle_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const VehicleGameObjDef *Item) {
	VehicleFactoryGameObj *VF = (VehicleFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
	AirFactoryGameObj *AF = (AirFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_HELIPAD);
	NavalFactoryGameObj *NF = (NavalFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_NAVAL_FACTORY);
	if (AF && Item->Get_Type() == VEHICLE_TYPE_FLYING) { //Flying vehicle
		if (Purchaser->Get_Player()->Purchase_Item((int)Cost)) {
			AF->Create_Vehicle(Item->Get_ID(),Purchaser);
			return 0;
		}
		return 2;
	}
	else if (NF && Item->Get_Type() == VEHICLE_TYPE_BOAT || Item->Get_Type() == VEHICLE_TYPE_SUB) { //Naval vehicle
		if (Purchaser->Get_Player()->Purchase_Item((int)Cost)) {
			NF->Create_Vehicle(Item->Get_ID(),Purchaser);
			return 0;
		}
		return 2;
	}
	else if (VF) { //Ground vehicle
		if (Purchaser->Get_Player()->Purchase_Item((int)Cost)) {
			float Delay = 5.0f;
			if (!Base->Is_Base_Powered()) {
				Delay *= Get_Build_Time_Multiplier(Base->Get_Player_Type());
			}
			VF->Request_Vehicle(Item->Get_ID(),Delay,Purchaser);
			return 0;
		}
		return 2;
	}
	return 3;
}

void DAVehicleManager::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	if (Victim->As_VehicleGameObj()) {
		StringClass Message;
		if (!((VehicleGameObj*)Victim)->Are_Transitions_Enabled() || !((VehicleGameObj*)Victim)->Get_Definition().Get_Seat_Count()) { //Harvesters/Defenses/AI Vehicles
			if (Victim->Get_Player_Type() == 0 || Victim->Get_Player_Type() == 1) {
				StringClass VictimName;
				if (Is_Harvester_Preset(Victim)) {
					VictimName = "the " + DATranslationManager::Translate_With_Team_Name(Victim);
				}
				else if (((VehicleGameObj*)Victim)->Is_Turret()) {
					VictimName = a_or_an_Prepend(DATranslationManager::Translate_With_Team_Name(Victim));
				}
				else {
					VictimName = a_or_an_Prepend(DATranslationManager::Translate(Victim));
				}
				if (!Killer) { //No killer
					VictimName[0] = (char)toupper(VictimName[0]);
					Message.Format("%d %s was destroyed.",Victim->Get_Player_Type(),VictimName);
				}
				else {
					if (Killer->As_SoldierGameObj()) { //Killed by player
						if (((SoldierGameObj*)Killer)->Get_Player()) {
							if (Type == DADamageType::EXPLOSION) {
								Message.Format("%d %ls destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),VictimName,DATranslationManager::Translate(GetExplosionObj()),DATranslationManager::Translate(Victim));
							}
							else {
								Message.Format("%d %ls destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),VictimName,DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate(Victim));
							}
						}
						else { //Killed by bot
							Message.Format("%d %s destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate(Killer)),VictimName,DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate(Victim));
						}
					}
					else if (Killer->As_VehicleGameObj()) { //Killed by vehicle. Could be defense or AI vehicle.
						if (Killer == Victim) { //Suicide
							VictimName[0] = (char)toupper(VictimName[0]);
							Message.Format("%d %s destroyed itself.",Victim->Get_Player_Type(),VictimName);
						}
						else if (Killer->Get_Defense_Object()->Get_Shield_Type() == 1) {
							Message.Format("%d The %s destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),DATranslationManager::Translate_With_Team_Name(Killer),VictimName,DATranslationManager::Translate(Killer),DATranslationManager::Translate(Victim));
						}
						else if (((VehicleGameObj*)Killer)->Is_Turret()) {
							Message.Format("%d %s destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate_With_Team_Name(Killer)),VictimName,DATranslationManager::Translate(Killer),DATranslationManager::Translate(Victim));
						}
						else {
							Message.Format("%d %s destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate(Killer)),VictimName,DATranslationManager::Translate(Killer),DATranslationManager::Translate(Victim));
						}
					}
				}
				DALogManager::Write_Log("_VEHKILL","%s",Message);
			}
		}
		else {
			DAVehicleObserverClass *Data = Get_Vehicle_Data(Victim);
			if (!Killer || (Data->Get_Last_Driver()?Data->Get_Last_Driver()->Get_GameObj():0) != Killer || DADamageLog::Get_Percent_Other_Team_Damage(Victim,Data->Get_Team())) {
				//If this vehicle was killed entirely by its own team then don't display a kill message. This is done so the other team can't tell you're massing by the kill messages.
				cPlayer *Owner = 0;
				if (((VehicleGameObj*)Victim)->Get_Occupant(0)) {
					Owner = ((VehicleGameObj*)Victim)->Get_Occupant(0)->Get_Player();
				}
				else if (cPlayer *Player = Get_Last_Driver(Victim)) {
					if (Player->Is_Active()) {
						Owner = Player;
					}
				}
				else if (((VehicleGameObj*)Victim)->Get_Owner()) {
					Owner = ((VehicleGameObj*)Victim)->Get_Owner()->Get_Player();
				}
				else if (((VehicleGameObj*)Victim)->Get_Lock_Owner()) {
					Owner = ((SoldierGameObj*)((VehicleGameObj*)Victim)->Get_Lock_Owner())->Get_Player();
				}
				StringClass VictimName;
				if (Owner) {
					if (Owner->Get_GameObj() == Killer) {
						VictimName = "their own ";
						VictimName += DATranslationManager::Translate(Victim);
					}
					else {
						VictimName = Make_Possessive(Owner->Get_Name());
						VictimName += " ";
						VictimName += DATranslationManager::Translate(Victim);
					}
				}
				else {
					VictimName = a_or_an_Prepend(DATranslationManager::Translate(Victim));
				}
				if (!Killer) { //No killer
					if (!Owner) {
						VictimName[0] = (char)toupper(VictimName[0]);
						Message.Format("2 %s was destroyed.",VictimName);
					}
					else {
						Message.Format("%d %s was destroyed.",Owner->Get_Player_Type(),VictimName);
					}
				}
				else if (Killer->As_SoldierGameObj()) { //Killed by soldier
					if (((SoldierGameObj*)Killer)->Get_Player()) { //Killed by player
						if (Type == DADamageType::EXPLOSION) {
							Message.Format("%d %ls destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),VictimName,DATranslationManager::Translate(GetExplosionObj()),DATranslationManager::Translate(Victim));
						}
						else {
							Message.Format("%d %ls destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),Get_Wide_Player_Name(Killer),VictimName,DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate(Victim));
						}
					}
					else {
						Message.Format("%d %s destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate(Killer)),VictimName,DATranslationManager::Translate_Soldier(Killer),DATranslationManager::Translate(Victim));
					}
				}
				else if (Killer->As_VehicleGameObj()) { //Killed by vehicle. Could be defense or AI vehicle.
					if (Victim == Killer) {
						if (!Owner) {
							VictimName[0] = (char)toupper(VictimName[0]);
							DA::Color_Message(WHITE,"2 %s destroyed itself.",VictimName);
						}
						else {
							Message.Format("%d %s destroyed itself.",Owner->Get_Player_Type(),VictimName);
						}
					}
					else if (Killer->Get_Defense_Object()->Get_Shield_Type() == 1) {
						Message.Format("%d The %s destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),DATranslationManager::Translate_With_Team_Name(Killer),VictimName,DATranslationManager::Translate(Killer),DATranslationManager::Translate(Victim));
					}
					else if (((VehicleGameObj*)Killer)->Is_Turret()) {
						Message.Format("%d %s destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate_With_Team_Name(Killer)),VictimName,DATranslationManager::Translate(Killer),DATranslationManager::Translate(Victim));
					}
					else {
						Message.Format("%d %s destroyed %s (%s VS. %s)",Killer->Get_Player_Type(),A_Or_An_Prepend(DATranslationManager::Translate(Killer)),VictimName,DATranslationManager::Translate(Killer),DATranslationManager::Translate(Victim));
					}
				}
				DALogManager::Write_Log("_VEHKILL","%s",Message);
			}
		}
	}
	else if (((BuildingGameObj*)Victim)->As_RefineryGameObj()) { //Fix bug where the Nod Harvester can still exist if it is building when the Refinery is killed.
		for (SLNode<VehicleGameObj> *x = GameObjManager::VehicleGameObjList.Head();x;x = x->Next()) {
			if (x->Data()->Get_Definition().Get_ID() == (unsigned int)((RefineryGameObj*)Victim)->Get_Definition().Get_Harvester_ID()) {
				x->Data()->Set_Delete_Pending();
			}
		}
	}
}

bool DAVehicleManager::Vehicle_Flip_Event(VehicleGameObj *Vehicle) {
	return false;
}

