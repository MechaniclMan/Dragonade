/*	Renegade Scripts.dll
    Dragonade Event Manager
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
#include "da_event.h"
#include "da_gamemode.h"
#include "da_chatcommand.h"
#include "da_hook.h"
#include "da_player.h"
#include "da_log.h"
#include "TransitionInstanceClass.h"
#include "cScTextObj.h"
#include "GameObjManager.h"
#include "ArmorWarheadManager.h"

#pragma warning(disable: 4073)
#pragma init_seg(lib)

DynamicVectorClass<DAEventStruct*> DAEventManager::Events[DAEvent::MAX];
DynamicVectorClass<DAObjectEventStruct*> DAEventManager::ObjectEvents[DAObjectEvent::MAX];
DynamicVectorClass<DAEventTimerStruct*> DAEventManager::Timers;
bool DAEventManager::IsSoldierReInit = false;
int DAEventManager::AddOccupantSeat = 0;
DAEventManager::DADamageEventStruct DAEventManager::LastDamageEvent;
DAEventManager::DADamageTableStruct *DAEventManager::DamageTable = 0;

void Install_GameObject_Virtual_Hooks();

void DAEventManager::Register_Event(DAEventClass *Base,DAEvent::Type Type,int Priority) {
	for (int i = 0;i < Events[Type].Count();i++) {
		if (Events[Type][i]->Base == Base) {
			return;
		}
	}
	DAEventStruct *Event = new DAEventStruct;
	Event->Base = Base;
	Event->Priority = Priority;
	for (int i = 0;i < Events[Type].Count();i++) {
		if (Events[Type][i]->Priority < Priority) {
			Events[Type].Insert(i,Event);
			return;
		}
	}
	Events[Type].Add(Event);
}

void DAEventManager::Unregister_Event(DAEventClass *Base,DAEvent::Type Type) {
	for (int i = Events[Type].Count()-1;i >= 0;i--) {
		if (Events[Type][i]->Base == Base) {
			delete Events[Type][i];
			Events[Type].Delete(i);
		}
	}
}

void DAEventManager::Clear_Events(DAEventClass *Base) {
	for (int j = 0;j < DAEvent::MAX;j++) {
		for (int i = Events[j].Count()-1;i >= 0;i--) {
			if (Events[j][i]->Base == Base) {
				delete Events[j][i];
				Events[j].Delete(i);
			}
		}
	}
}

void DAEventManager::Register_Object_Event(DAEventClass *Base,DAObjectEvent::Type Type,DAObjectEvent::ObjectType ObjectType,int Priority) {
	for (int i = 0;i < ObjectEvents[Type].Count();i++) {
		if (ObjectEvents[Type][i]->Base == Base) {
			return;
		}
	}
	DAObjectEventStruct *Event = new DAObjectEventStruct;
	Event->Base = Base;
	Event->ObjectType = ObjectType;
	Event->Priority = Priority;
	for (int i = 0;i < ObjectEvents[Type].Count();i++) {
		if (ObjectEvents[Type][i]->Priority < Priority) {
			ObjectEvents[Type].Insert(i,Event);
			return;
		}
	}
	ObjectEvents[Type].Add(Event);
}

void DAEventManager::Unregister_Object_Event(DAEventClass *Base,DAObjectEvent::Type Type) {
	for (int i = ObjectEvents[Type].Count()-1;i >= 0;i--) {
		if (ObjectEvents[Type][i]->Base == Base) {
			delete ObjectEvents[Type][i];
			ObjectEvents[Type].Delete(i);
		}
	}
}

void DAEventManager::Clear_Object_Events(DAEventClass *Base) {
	for (int j = 0;j < DAObjectEvent::MAX;j++) {
		for (int i = ObjectEvents[j].Count()-1;i >= 0;i--) {
			if (ObjectEvents[j][i]->Base == Base) {
				delete ObjectEvents[j][i];
				ObjectEvents[j].Delete(i);
			}
		}
	}
}

void DAEventManager::Start_Timer(DAEventClass *Base,int Number,float Duration,bool Repeat,unsigned int Data) {
	DAEventTimerStruct *Timer = new DAEventTimerStruct;
	Timer->Base = Base;
	Timer->Number = Number;
	Timer->Repeat = Repeat;
	Timer->Data = Data;
	Timer->Duration = (unsigned long)(Duration*1000);
	Timer->StartTime = GetTickCount();
	Timers.Add(Timer);
}

void DAEventManager::Stop_Timer(DAEventClass *Base,int Number,unsigned int Data) {
	for (int i = Timers.Count()-1;i >= 0;i--) {
		if (Timers[i]->Base == Base && Timers[i]->Number == Number && (!Data || Timers[i]->Data == Data)) {
			Timers[i]->Number = -1;
		}
	}
}

void DAEventManager::Clear_Timers(DAEventClass *Base) {
	for (int i = Timers.Count()-1;i >= 0;i--) {
		if (Timers[i]->Base == Base) {
			delete Timers[i];
			Timers.Delete(i);
		}
	}
}

void DAEventManager::Think() {
	for (int i = 0;i < Events[DAEvent::THINK].Count();i++) {
		Events[DAEvent::THINK][i]->Base->Think();
	}
	for (int i = 0;i < Timers.Count();i++) {
		if (Timers[i]->Number == -1) {
			delete Timers[i];
			Timers.Delete(i);
			i--;
		}
		else if (GetTickCount()-Timers[i]->StartTime >= Timers[i]->Duration) {
			Timers[i]->Base->Timer_Expired(Timers[i]->Number,Timers[i]->Data);
			if (Timers[i]->Repeat) {
				Timers[i]->StartTime = GetTickCount();
			}
			else {
				delete Timers[i];
				Timers.Delete(i);
				i--;
			}
		}
	}
}

void DAEventManager::Settings_Loaded_Event() {
	for (int i = 0;i < Events[DAEvent::SETTINGSLOADED].Count();i++) {
		Events[DAEvent::SETTINGSLOADED][i]->Base->Settings_Loaded_Event();
	}
}

bool DAEventManager::Chat_Event(cPlayer *Player,TextMessageEnum Type,const wchar_t *Message,int ReceiverID) {
	if (Message[0] == '!') {
		if (!Player->Get_GameObj()) {
			return true;
		}
		DATokenParserClass Parser(Message,' ');
		StringClass Command = Parser.Get_String(); //Get the command from the first token.
		DATokenClass Text(Parser); //Build the token class from the remaining string.
		Command.ToLower();
		for (int i = 0;i < Events[DAEvent::CHATCOMMAND].Count();i++) {
			if (!Events[DAEvent::CHATCOMMAND][i]->Base->Chat_Command_Event(Player,Type,Command,Text,ReceiverID)) {
				return false;
			}
		}
	}
	for (int i = 0;i < Events[DAEvent::CHAT].Count();i++) {
		if (!Events[DAEvent::CHAT][i]->Base->Chat_Event(Player,Type,Message,ReceiverID)) {
			return false;
		}
	}
	return true;
}

bool DAEventManager::Key_Hook_Event(cPlayer *Player,const StringClass &Key) {
	if (!Player->Get_GameObj()) {
		return true;
	}
	for (int i = 0;i < Events[DAEvent::KEYHOOK].Count();i++) {
		if (!Events[DAEvent::KEYHOOK][i]->Base->Key_Hook_Event(Player,Key)) {
			return false;
		}
	}
	return true;
}

bool DAEventManager::Host_Chat_Event(int ID,TextMessageEnum Type,const char *Message) {
	for (int i = 0;i < Events[DAEvent::HOSTCHAT].Count();i++) {
		if (!Events[DAEvent::HOSTCHAT][i]->Base->Host_Chat_Event(ID,Type,Message)) {
			return false;
		}
	}
	return true;
}

bool DAEventManager::Radio_Event(int PlayerType,int ID,int AnnouncementID,int IconID,AnnouncementEnum AnnouncementType) {
	cPlayer *Player = Find_Player(ID);
	for (int i = 0;i < Events[DAEvent::RADIO].Count();i++) {
		if (!Events[DAEvent::RADIO][i]->Base->Radio_Event(Player,PlayerType,AnnouncementID,IconID,AnnouncementType)) {
			return false;
		}
	}
	return true;
}

void DAEventManager::Player_Join_Event(int ID,const char *Name) {
	cPlayer *Player = Find_Player(ID);
	if (Player) {
		for (int i = 0;i < Events[DAEvent::PLAYERJOIN].Count();i++) {
			Events[DAEvent::PLAYERJOIN][i]->Base->Player_Join_Event(Player);
		}
	}
}

void DAEventManager::Player_Leave_Event(int ID) {
	cPlayer *Player = Find_Player(ID);
	if (Player) {
		for (int i = 0;i < Events[DAEvent::PLAYERLEAVE].Count();i++) {
			Events[DAEvent::PLAYERLEAVE][i]->Base->Player_Leave_Event(Player);
		}
	}
}

void DAEventManager::Name_Change_Event(cPlayer *Player) {
	for (int i = 0;i < Events[DAEvent::NAMECHANGE].Count();i++) {
		Events[DAEvent::NAMECHANGE][i]->Base->Name_Change_Event(Player);
	}
}

void DAEventManager::Level_Loaded_Event() {
	for (int i = 0;i < Events[DAEvent::LEVELLOADED].Count();i++) {
		Events[DAEvent::LEVELLOADED][i]->Base->Level_Loaded_Event();
	}
	if (DamageTable) {
		delete DamageTable;
	}
	DamageTable = new DADamageTableStruct[ArmorWarheadManager::Get_Num_Warhead_Types()];
	for (AmmoDefinitionClass *Def = (AmmoDefinitionClass*)DefinitionMgrClass::Get_First(CID_Ammo);Def;Def = (AmmoDefinitionClass*)DefinitionMgrClass::Get_Next(Def,CID_Ammo)) {
		DamageTable[Def->Warhead].Normal.Add(Def->Damage);
		DamageTable[Def->Warhead].Neckshot.Add(Def->Damage*3.0f);
		DamageTable[Def->Warhead].Headshot.Add(Def->Damage*5.0f);
	}
	LastDamageEvent.Type = DADamageType::NONE;
	LastDamageEvent.ClientDamage = false;
}

void DAEventManager::Remix_Event() {
	for (int i = 0;i < Events[DAEvent::REMIX].Count();i++) {
		Events[DAEvent::REMIX][i]->Base->Remix_Event();
	}
}

void DAEventManager::Rebalance_Event() {
	for (int i = 0;i < Events[DAEvent::REBALANCE].Count();i++) {
		Events[DAEvent::REBALANCE][i]->Base->Rebalance_Event();
	}
}

void DAEventManager::Swap_Event() {
	for (int i = 0;i < Events[DAEvent::SWAP].Count();i++) {
		Events[DAEvent::SWAP][i]->Base->Swap_Event();
	}
}

void DAEventManager::Game_Over_Event() {
	for (int i = 0;i < Events[DAEvent::GAMEOVER].Count();i++) {
		Events[DAEvent::GAMEOVER][i]->Base->Game_Over_Event();
	}
}

void DAEventManager::Console_Output_Event(const char *Output) {
	for (int i = 0;i < Events[DAEvent::CONSOLEOUTPUT].Count();i++) {
		Events[DAEvent::CONSOLEOUTPUT][i]->Base->Console_Output_Event(Output);
	}
}

void DAEventManager::Ren_Log_Event(const char *Output) {
	for (int i = 0;i < Events[DAEvent::RENLOG].Count();i++) {
		Events[DAEvent::RENLOG][i]->Base->Ren_Log_Event(Output);
	}
}

void DAEventManager::DA_Log_Event(const char *Header,const char *Output) {
	for (int i = 0;i < Events[DAEvent::DALOG].Count();i++) {
		Events[DAEvent::DALOG][i]->Base->DA_Log_Event(Header,Output);
	}
}

int DAEventManager::Character_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,unsigned int Cost,unsigned int Preset,const char *Data) {
	DefinitionClass *Def = Find_Definition(Preset);
	if (Def) { //Check the class ID and pass to the proper purchase function. This allows any type of preset to be in any menu.
		if (Def->Get_Class_ID() == CID_Vehicle) {
			return Vehicle_Purchase_Request_Event(Base,Purchaser,Cost,Preset,0);
		}
		else if (Def->Get_Class_ID() == CID_PowerUp) {
			return PowerUp_Purchase_Request_Event(Base,Purchaser,Cost,Preset,0);
		}
		else if (Def->Get_Class_ID() == CID_Soldier) {
			float NewCost = (float)Cost;
			for (int i = 0;i < Events[DAEvent::CHARACTERPURCHASEREQUEST].Count();i++) {
				int Return = Events[DAEvent::CHARACTERPURCHASEREQUEST][i]->Base->Character_Purchase_Request_Event(Base,Purchaser,NewCost,(SoldierGameObjDef*)Def);
				NewCost = Round(NewCost);
				if (Return != -1) {
					if (!Return) {
						Character_Purchase_Event(Purchaser,NewCost,(SoldierGameObjDef*)Def);
					}
					return Return;
				}
			}
		}
		else {
			return Custom_Purchase_Request_Event(Base,Purchaser,Cost,Preset);
		}
	}
	return -1;
}

int DAEventManager::Vehicle_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,unsigned int Cost,unsigned int Preset,const char *Data) {
	DefinitionClass *Def = Find_Definition(Preset);
	if (Def) {
		if (Def->Get_Class_ID() == CID_Soldier) {
			return Character_Purchase_Request_Event(Base,Purchaser,Cost,Preset,0);
		}
		else if (Def->Get_Class_ID() == CID_PowerUp) {
			return PowerUp_Purchase_Request_Event(Base,Purchaser,Cost,Preset,0);
		}
		else if (Def->Get_Class_ID() == CID_Vehicle) {
			float NewCost = (float)Cost;
			for (int i = 0;i < Events[DAEvent::VEHICLEPURCHASEREQUEST].Count();i++) {
				int Return = Events[DAEvent::VEHICLEPURCHASEREQUEST][i]->Base->Vehicle_Purchase_Request_Event(Base,Purchaser,NewCost,(VehicleGameObjDef*)Def);
				NewCost = Round(NewCost);
				if (Return != -1) {
					if (!Return) {
						Vehicle_Purchase_Event(Purchaser,NewCost,(VehicleGameObjDef*)Def);
					}
					return Return;
				}
			}
		}
		else {
			return Custom_Purchase_Request_Event(Base,Purchaser,Cost,Preset);
		}
	}
	return -1;
}

int DAEventManager::PowerUp_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,unsigned int Cost,unsigned int Preset,const char *Data) {
	DefinitionClass *Def = Find_Definition(Preset);
	if (Def) {
		if (Def->Get_Class_ID() == CID_Vehicle) {
			return Vehicle_Purchase_Request_Event(Base,Purchaser,Cost,Preset,0);
		}
		else if (Def->Get_Class_ID() == CID_Soldier) {
			return Character_Purchase_Request_Event(Base,Purchaser,Cost,Preset,0);
		}
		else if (Def->Get_Class_ID() == CID_PowerUp) {
			float NewCost = (float)Cost;
			for (int i = 0;i < Events[DAEvent::POWERUPPURCHASEREQUEST].Count();i++) {
				int Return = Events[DAEvent::POWERUPPURCHASEREQUEST][i]->Base->PowerUp_Purchase_Request_Event(Base,Purchaser,NewCost,(PowerUpGameObjDef*)Def);
				NewCost = Round(NewCost);
				if (Return != -1) {
					if (!Return) {
						PowerUp_Purchase_Event(Purchaser,NewCost,(PowerUpGameObjDef*)Def);
					}
					return Return;
				}
			}
		}
		else {
			return Custom_Purchase_Request_Event(Base,Purchaser,Cost,Preset);
		}
	}
	return -1;
}

int DAEventManager::Custom_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,unsigned int Cost,unsigned int Preset) {
	float NewCost = (float)Cost;
	for (int i = 0;i < Events[DAEvent::CUSTOMPURCHASEREQUEST].Count();i++) {
		int Return = Events[DAEvent::CUSTOMPURCHASEREQUEST][i]->Base->Custom_Purchase_Request_Event(Base,Purchaser,NewCost,Preset);
		NewCost = Round(NewCost);
		if (Return != -1) {
			if (!Return) {
				Custom_Purchase_Event(Purchaser,NewCost,Preset);
			}
			return Return;
		}
	}
	return 3;
}

void DAEventManager::Character_Purchase_Event(SoldierGameObj *Purchaser,float Cost,const SoldierGameObjDef *Def) {
	for (int i = 0;i < Events[DAEvent::CHARACTERPURCHASE].Count();i++) {
		Events[DAEvent::CHARACTERPURCHASE][i]->Base->Character_Purchase_Event(Purchaser,Cost,Def);
	}
}

void DAEventManager::Vehicle_Purchase_Event(SoldierGameObj *Purchaser,float Cost,const VehicleGameObjDef *Def) {
	for (int i = 0;i < Events[DAEvent::VEHICLEPURCHASE].Count();i++) {
		Events[DAEvent::VEHICLEPURCHASE][i]->Base->Vehicle_Purchase_Event(Purchaser,Cost,Def);
	}
}

void DAEventManager::PowerUp_Purchase_Event(SoldierGameObj *Purchaser,float Cost,const PowerUpGameObjDef *Def) {
	for (int i = 0;i < Events[DAEvent::POWERUPPURCHASE].Count();i++) {
		Events[DAEvent::POWERUPPURCHASE][i]->Base->PowerUp_Purchase_Event(Purchaser,Cost,Def);
	}
}

void DAEventManager::Custom_Purchase_Event(SoldierGameObj *Purchaser,float Cost,unsigned int ID) {
	for (int i = 0;i < Events[DAEvent::CUSTOMPURCHASE].Count();i++) {
		Events[DAEvent::CUSTOMPURCHASE][i]->Base->Custom_Purchase_Event(Purchaser,Cost,ID);
	}
}

bool DAEventManager::Refill_Event(SoldierGameObj *Purchaser) {
	for (int i = 0;i < Events[DAEvent::REFILL].Count();i++) {
		if (!Events[DAEvent::REFILL][i]->Base->Refill_Event(Purchaser)) {
			return false;
		}
	}
	return true;
}

void DAEventManager::Suicide_Event(int ID) {
	cPlayer *Player = Find_Player(ID);
	if (Player) {
		for (int i = 0;i < Events[DAEvent::SUICIDE].Count();i++) {
			if (!Events[DAEvent::SUICIDE][i]->Base->Suicide_Event(Player)) {
				return;
			}
		}
	}
}

void DAEventManager::Team_Change_Request_Event(int ID) {
	cPlayer *Player = Find_Player(ID);
	if (Player) {
		for (int i = 0;i < Events[DAEvent::TEAMCHANGEREQUEST].Count();i++) {
			if (!Events[DAEvent::TEAMCHANGEREQUEST][i]->Base->Team_Change_Request_Event(Player)) {
				return;
			}
		}
	}
}

void DAEventManager::Transition_Check_Event(TransitionInstanceClass *Transition,SoldierGameObj *Soldier) {
	VehicleGameObj *Vehicle = (VehicleGameObj*)Transition->Get_Vehicle();
	if (!Vehicle || Soldier->Get_Vehicle() == Vehicle) { //Ladder or exiting vehicle.
		Transition->Start(Soldier);
		return;
	}
	int Seat = Find_Empty_Vehicle_Seat(Vehicle);
	int SeatCount = Vehicle->Get_Definition().Get_Seat_Count();
	if (Seat != -1) {
		int SeatRef = Seat;
		for (int i = 0;i < Events[DAEvent::VEHICLEENTRYREQUEST].Count();i++) {
			if (!Events[DAEvent::VEHICLEENTRYREQUEST][i]->Base->Vehicle_Entry_Request_Event(Vehicle,Soldier,SeatRef)) {
				return;
			}
		}
		if (Vehicle->Get_Occupant(SeatRef) || SeatRef >= SeatCount || SeatRef < 0) {
			AddOccupantSeat = Seat; //Use the seat we picked before if an event returned an invalid one.
		}
		else {
			AddOccupantSeat = SeatRef;
		}
		Transition->Start(Soldier);
	}
}

void DAEventManager::Add_Occupant_Event(VehicleGameObj *Vehicle,SoldierGameObj *Soldier) {
	Vehicle->Add_Occupant(Soldier,AddOccupantSeat);
	if (!Vehicle->Get_Driver()) {
		Commands->Enable_Engine(Vehicle,false);
	}
}

bool DAEventManager::PowerUp_Grant_Request_Event(SmartGameObj *Grantee,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	for (int i = 0;i < Events[DAEvent::POWERUPGRANTREQUEST].Count();i++) {
		if (!Events[DAEvent::POWERUPGRANTREQUEST][i]->Base->PowerUp_Grant_Request_Event((SoldierGameObj*)Grantee,PowerUp,PowerUpObj)) {
			return false;
		}
	}
	return true;
}

void DAEventManager::PowerUp_Grant_Event(SmartGameObj *Grantee,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	for (int i = 0;i < Events[DAEvent::POWERUPGRANT].Count();i++) {
		Events[DAEvent::POWERUPGRANT][i]->Base->PowerUp_Grant_Event((SoldierGameObj*)Grantee,PowerUp,PowerUpObj);
	}
}

bool DAEventManager::Add_Weapon_Request_Event(SoldierGameObj *Soldier,const WeaponDefinitionClass *Weapon) {
	for (int i = 0;i < Events[DAEvent::ADDWEAPONREQUEST].Count();i++) {
		if (!Events[DAEvent::ADDWEAPONREQUEST][i]->Base->Add_Weapon_Request_Event(Soldier,Weapon)) {
			return false;
		}
	}
	return true;
}

void DAEventManager::Add_Weapon_Event(SoldierGameObj *Soldier,WeaponClass *Weapon) {
	for (int i = 0;i < Events[DAEvent::ADDWEAPON].Count();i++) {
		Events[DAEvent::ADDWEAPON][i]->Base->Add_Weapon_Event(Soldier,Weapon);
	}
}

void DAEventManager::Remove_Weapon_Event(WeaponBagClass *Bag,int Index) {
	if (!IsSoldierReInit && Is_Player(Bag->Get_Owner()) && Index < Bag->Get_Count()) {
		WeaponClass *Weapon = Bag->Peek_Weapon(Index);
		for (int i = 0;i < Events[DAEvent::REMOVEWEAPON].Count();i++) {
			Events[DAEvent::REMOVEWEAPON][i]->Base->Remove_Weapon_Event((SoldierGameObj*)Bag->Get_Owner(),Weapon);
		}
	}
}

void DAEventManager::Clear_Weapons_Event(WeaponBagClass *Bag) {
	if (!IsSoldierReInit && Is_Player(Bag->Get_Owner())) {
		for (int i = 0;i < Events[DAEvent::CLEARWEAPONS].Count();i++) {
			Events[DAEvent::CLEARWEAPONS][i]->Base->Clear_Weapons_Event((SoldierGameObj*)Bag->Get_Owner());
		}
	}
}

void DAEventManager::Beacon_Set_State_Event(BeaconGameObj *Beacon) {
	if (Beacon->Get_State() == 2) {
		for (int i = 0;i < Events[DAEvent::BEACONDEPLOY].Count();i++) {
			Events[DAEvent::BEACONDEPLOY][i]->Base->Beacon_Deploy_Event(Beacon);
		}
	}
	else if (Beacon->Get_State() == 4) {
		for (int i = 0;i < Events[DAEvent::BEACONDETONATE].Count();i++) {
			Events[DAEvent::BEACONDETONATE][i]->Base->Beacon_Detonate_Event(Beacon);
		}
	}
}

bool DAEventManager::C4_Detonate_Request_Event(C4GameObj *C4) {
	if (C4->Get_Ammo_Def()->AmmoType == 3) {
		float TriggerRange = C4->Get_Ammo_Def()->C4TriggerRange1;
		Vector3 Position = Commands->Get_Position(C4);
		for (SLNode<SmartGameObj> *z = GameObjManager::SmartGameObjList.Head();z;z = z->Next()) { //Check all objects in range. Detonate if any one of them is allowed to trigger the C4.
			if (C4->Is_Enemy(z->Data()) && Commands->Get_Distance(Position,z->Data()->Get_Position()) <= TriggerRange) {
				bool Allow = true;
				for (int i = 0;i < Events[DAEvent::C4DETONATEREQUEST].Count();i++) {
					if (!Events[DAEvent::C4DETONATEREQUEST][i]->Base->C4_Detonate_Request_Event(C4,z->Data())) {
						Allow = false;
						break;
					}
				}
				if (Allow) {
					return true;
				}
			}
		}
		return false;
	}
	else {
		for (int i = 0;i < Events[DAEvent::C4DETONATEREQUEST].Count();i++) {
			if (!Events[DAEvent::C4DETONATEREQUEST][i]->Base->C4_Detonate_Request_Event(C4,0)) {
				return false;
			}
		}
	}
	return true;
}

void DAEventManager::C4_Detonate_Event(C4GameObj *C4) {
	for (int i = 0;i < Events[DAEvent::C4DETONATE].Count();i++) {
		Events[DAEvent::C4DETONATE][i]->Base->C4_Detonate_Event(C4);
	}
}

void DAEventManager::Soldier_Re_Init_Event(SoldierGameObj *Soldier,const SoldierGameObjDef *SoldierDef) {
	if (Soldier->Get_Player()) {
		IsSoldierReInit = true;
		for (int i = 0;i < Events[DAEvent::CHANGECHARACTER].Count();i++) {
			Events[DAEvent::CHANGECHARACTER][i]->Base->Change_Character_Event(Soldier,SoldierDef);
		}
	}
}

bool DAEventManager::Request_Vehicle_Event(VehicleFactoryGameObj *Factory,unsigned int Vehicle,SoldierGameObj *Owner,float Delay) {
	VehicleGameObjDef *VehicleDef = (VehicleGameObjDef*)Find_Definition(Vehicle);
	if (!VehicleDef || VehicleDef->Get_Class_ID() != CID_Vehicle) {
		return false;
	}
	for (int i = 0;i < Events[DAEvent::REQUESTVEHICLE].Count();i++) {
		if (!Events[DAEvent::REQUESTVEHICLE][i]->Base->Request_Vehicle_Event(Factory,VehicleDef,Owner,Delay)) {
			return false;
		}
	}
	return true;
}



void DAEventManager::Object_Created_Event(void *Data,GameObject *obj) {
	obj->Add_Observer(new DAEventObserverClass);
	for (int i = 0;i < ObjectEvents[DAObjectEvent::CREATED].Count() && !obj->Is_Delete_Pending();i++) {
		if (ObjectEvents[DAObjectEvent::CREATED][i]->Check_Object_Type(obj)) {
			ObjectEvents[DAObjectEvent::CREATED][i]->Base->Object_Created_Event(obj);
		}
	}
	if (obj->As_SoldierGameObj()) {
		cPlayer *Player = ((SoldierGameObj*)obj)->Get_Player();
		if (Player) {
			if (obj->Get_Object_Dirty_Bit_2(Player->Get_ID(),NetworkObjectClass::BIT_CREATION)) { //If the object was just created.
				if (!Player->Get_DA_Player()->Is_Loaded()) {
					Player->Get_DA_Player()->Set_Loaded(true);
					for (int i = 0;i < Events[DAEvent::PLAYERLOADED].Count();i++) {
						Events[DAEvent::PLAYERLOADED][i]->Base->Player_Loaded_Event(Player);
					}
				}
				WeaponBagClass *Bag = ((SoldierGameObj*)obj)->Get_Weapon_Bag();
				if (Bag->Get_Index()) { //When a player first spawns their SoldierGameObj is granted its primary weapon before being assigned to the player. This causes the add weapon events to not trigger. The following code fixes this.
					WeaponClass *Weapon = Bag->Get_Weapon();
					const WeaponDefinitionClass *WeaponDef = Weapon->Get_Definition();
					bool Allow = true;
					for (int i = 0;i < Events[DAEvent::ADDWEAPONREQUEST].Count();i++) {
						if (!Events[DAEvent::ADDWEAPONREQUEST][i]->Base->Add_Weapon_Request_Event((SoldierGameObj*)obj,WeaponDef)) {
							Bag->Remove_Weapon(Bag->Get_Index());
							Bag->Select_Next();
							Allow = false;
							break;
						}
					}
					if (Allow) {
						for (int i = 0;i < Events[DAEvent::ADDWEAPON].Count();i++) {
							Events[DAEvent::ADDWEAPON][i]->Base->Add_Weapon_Event((SoldierGameObj*)obj,Weapon);
						}
					}
				}
			}
			else {
				IsSoldierReInit = false;
			}
		}
	}
}

bool DAEventManager::Stock_Client_Damage_Request_Event(PhysicalGameObj *Damager,PhysicalGameObj *Target,float Damage, uint Warhead) {
	for (int i = 0;i < ObjectEvents[DAObjectEvent::STOCKCLIENTDAMAGEREQUEST].Count();i++) {
		if (ObjectEvents[DAObjectEvent::STOCKCLIENTDAMAGEREQUEST][i]->Check_Object_Type(Target)) {
			if (!ObjectEvents[DAObjectEvent::STOCKCLIENTDAMAGEREQUEST][i]->Base->Stock_Client_Damage_Request_Event(Target,(ArmedGameObj*)Damager,Damage,Warhead)) {
				return false;
			}
		}
	}
	LastDamageEvent.ClientDamage = true;
	if (Target->As_SoldierGameObj()) { //This calcuates, based on the warhead and damage done, what bone was hit.
		if (DamageTable[Warhead].Normal.ID(Damage) != -1) {
			LastDamageEvent.Bone = "K_CHEST";
			LastDamageEvent.Type = DADamageType::NORMAL;
		}
		else if (DamageTable[Warhead].Headshot.ID(Damage) != -1) {
			LastDamageEvent.Bone = "K_HEAD";
			LastDamageEvent.Type = DADamageType::HEADSHOT;
		}
		else if (DamageTable[Warhead].Neckshot.ID(Damage) != -1) {
			LastDamageEvent.Bone = "K_NECK";
			LastDamageEvent.Type = DADamageType::NECKSHOT;
		}
		else {
			LastDamageEvent.Bone = "None";
			LastDamageEvent.Type = DADamageType::NORMAL;
		}
		/*int NormalChance = DamageTable[Warhead].Get_Normal_Chance(Damage);
		int NeckshotChance = DamageTable[Warhead].Get_Neckshot_Chance(Damage);
		int HeadshotChance = DamageTable[Warhead].Get_Headshot_Chance(Damage);
		Console_InputF("msg %d %d %d",NormalChance,NeckshotChance,HeadshotChance);
		if (NormalChance >= NeckshotChance) {
			if (NormalChance >= HeadshotChance) {
				LastDamageEvent.Bone = "K_CHEST";
				LastDamageEvent.Type = DADamageType::NORMAL;
			}
			else {
				LastDamageEvent.Bone = "K_HEAD";
				LastDamageEvent.Type = DADamageType::HEADSHOT;
			}
		}
		else if (HeadshotChance >= NormalChance) {
			if (HeadshotChance >= NeckshotChance) {
				LastDamageEvent.Bone = "K_HEAD";
				LastDamageEvent.Type = DADamageType::HEADSHOT;
			}
			else {
				LastDamageEvent.Bone = "K_NECK";
				LastDamageEvent.Type = DADamageType::NECKSHOT;
			}
		}
		else if (NeckshotChance >= HeadshotChance) {
			if (NeckshotChance >= NormalChance) {
				LastDamageEvent.Bone = "K_NECK";
				LastDamageEvent.Type = DADamageType::NECKSHOT;
			}
			else {
				LastDamageEvent.Bone = "K_CHEST";
				LastDamageEvent.Type = DADamageType::NORMAL;
			}
		}
		else {
			LastDamageEvent.Bone = "None";
			LastDamageEvent.Type = DADamageType::NORMAL;
		}*/
	}
	else {
		LastDamageEvent.Bone = "None";
		LastDamageEvent.Type = DADamageType::NORMAL;
	}
	return true;
}

bool DAEventManager::TT_Client_Damage_Request_Event(PhysicalGameObj *Damager,PhysicalGameObj *Target,const AmmoDefinitionClass *Ammo,const char *Bone) {
	for (int i = 0;i < ObjectEvents[DAObjectEvent::TTCLIENTDAMAGEREQUEST].Count();i++) {
		if (ObjectEvents[DAObjectEvent::TTCLIENTDAMAGEREQUEST][i]->Check_Object_Type(Target)) {
			if (!ObjectEvents[DAObjectEvent::TTCLIENTDAMAGEREQUEST][i]->Base->TT_Client_Damage_Request_Event(Target,(ArmedGameObj*)Damager,Ammo,Bone)) {
				return false;
			}
		}
	}
	LastDamageEvent.ClientDamage = true;
	LastDamageEvent.Bone = Bone;
	if (strstr(Bone,"K_HEAD")) {
		LastDamageEvent.Type = DADamageType::HEADSHOT;
	}
	else if (strstr(Bone,"K_NECK")) {
		LastDamageEvent.Type = DADamageType::NECKSHOT;
	}
	else {
		LastDamageEvent.Type = DADamageType::NORMAL;
	}
	return true;
}

bool DAEventManager::Damage_Request_Event(DefenseObjectClass *Defense,OffenseObjectClass *Offense,float Scale) {
	DamageableGameObj *Victim = Defense->Get_Owner();
	ArmedGameObj *Damager = Offense->Get_Owner();
	//Console_InputF("msg damage request %s %s %f %u %d %d %d %f",Victim->Get_Definition().Get_Name(),Damager?Damager->Get_Definition().Get_Name():"None",Offense->Get_Damage(),Offense->Get_Warhead(),LastDamageEvent.ClientDamage,Offense->EnableClientDamage,Offense->ForceServerDamage);
	if (Offense->EnableClientDamage) { //Server damage request.
		if (Is_Player(Damager)) { //Server extrapolation of client damage.
			if (Victim->As_BuildingGameObj()) { //Only used for buildings.
				LastDamageEvent.Bone = "None";
				LastDamageEvent.Type = DADamageType::NORMAL;
			}
			else { //Ignore duplicate damage events generated by server.
				return false;
			}
		}
		else {
			LastDamageEvent.Bone = "None";
			LastDamageEvent.Type = DADamageType::NORMAL;
		}
	}
	else if (!LastDamageEvent.ClientDamage) { //Damage with no damager, splash/burn, and C4/beacon explosions.
		if (Damager) {
			if (Scale == 10000.0f && Offense->Get_Damage() == 10000.0f) { //Squish
				LastDamageEvent.Bone = "None";
				LastDamageEvent.Type = DADamageType::SQUISH;
			}
			else if (GetExplosionObj()) { //C4/Beacon explosion
				LastDamageEvent.Type = DADamageType::EXPLOSION;
			}
			else if (Offense->Get_Warhead() == 0) {
				if (Offense->Get_Damage() < 2.0f) { //Burn damage
					LastDamageEvent.Type = DADamageType::BURN;
				}
				else { //Probably Commands->Apply_Damage
					LastDamageEvent.Type = DADamageType::NORMAL;
				}
			}
			else { //Splash damage
				LastDamageEvent.Type = DADamageType::SPLASH;
			}
		}
		else if (Offense->Get_Warhead() == 9) { //Tiberium damage
			LastDamageEvent.Type = DADamageType::TIBERIUM;
			if (Is_Player(Victim)) {
				((SoldierGameObj*)Victim)->Get_DA_Player()->Set_Last_Tib_Damage_Time(GetTickCount());
			}
		}
		else if (Offense->Get_Warhead() == 0) {
			if (Is_Player(Victim)) {
				if (Offense->Get_Damage() < 2.0f) {
					if (GetTickCount()-((SoldierGameObj*)Victim)->Get_DA_Player()->Get_Last_Tib_Damage_Time() <= 3000) { //Tiberium burn damage
						LastDamageEvent.Type = DADamageType::TIBERIUM;
					}
					else { //Burn damage
						LastDamageEvent.Type = DADamageType::BURN;
					}
				}
				else { //Probably Commands->Apply_Damage
					LastDamageEvent.Type = DADamageType::NORMAL;
				}
			}
			else if (Offense->Get_Damage() < 2.0f) { //Burn damage
				LastDamageEvent.Type = DADamageType::BURN;
			}
			else { //Probably Commands->Apply_Damage
				LastDamageEvent.Type = DADamageType::NORMAL;
			}
		}
		else if (Offense->Get_Warhead() == 22) { //Fall damage
			LastDamageEvent.Type = DADamageType::FALL;
		}
		else { //Probably Commands->Apply_Damage
			LastDamageEvent.Type = DADamageType::NORMAL;
		}
		LastDamageEvent.Bone = "None";
	}
	LastDamageEvent.ClientDamage = false;
	//Console_InputF("msg damage request %s %s %f %u %d %d %d %s",Victim->Get_Definition().Get_Name(),Damager?Damager->Get_Definition().Get_Name():"None",Offense->Get_Damage(),Offense->Get_Warhead(),Offense->EnableClientDamage,Offense->ForceServerDamage,LastDamageEvent.Type,LastDamageEvent.Bone);
	for (int i = 0;i < ObjectEvents[DAObjectEvent::DAMAGEDEALTREQUEST].Count();i++) {
		if (ObjectEvents[DAObjectEvent::DAMAGEDEALTREQUEST][i]->Check_Object_Type(Damager)) {
			if (!ObjectEvents[DAObjectEvent::DAMAGEDEALTREQUEST][i]->Base->Damage_Request_Event(Victim,Offense,LastDamageEvent.Type,LastDamageEvent.Bone)) {
				return false;
			}
		}
	}
	for (int i = 0;i < ObjectEvents[DAObjectEvent::DAMAGERECEIVEDREQUEST].Count();i++) {
		if (ObjectEvents[DAObjectEvent::DAMAGERECEIVEDREQUEST][i]->Check_Object_Type(Victim)) {
			if (!ObjectEvents[DAObjectEvent::DAMAGERECEIVEDREQUEST][i]->Base->Damage_Request_Event(Victim,Offense,LastDamageEvent.Type,LastDamageEvent.Bone)) {
				return false;
			}
		}
	}
	return true;
}

void DAEventManager::Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead) {
	//Console_InputF("msg damage complete %s %s %f %u %d %s",Victim->Get_Definition().Get_Name(),Damager?Damager->Get_Definition().Get_Name():"None",Damage,Warhead,LastDamageEvent.Type,LastDamageEvent.Bone);
	for (int i = 0;i < ObjectEvents[DAObjectEvent::DAMAGEDEALT].Count();i++) {
		if (ObjectEvents[DAObjectEvent::DAMAGEDEALT][i]->Check_Object_Type(Damager)) {
			ObjectEvents[DAObjectEvent::DAMAGEDEALT][i]->Base->Damage_Event(Victim,Damager,Damage,Warhead,LastDamageEvent.Type,LastDamageEvent.Bone);
		}
	}
	for (int i = 0;i < ObjectEvents[DAObjectEvent::DAMAGERECEIVED].Count();i++) {
		if (ObjectEvents[DAObjectEvent::DAMAGERECEIVED][i]->Check_Object_Type(Victim)) {
			ObjectEvents[DAObjectEvent::DAMAGERECEIVED][i]->Base->Damage_Event(Victim,Damager,Damage,Warhead,LastDamageEvent.Type,LastDamageEvent.Bone);
		}
	}
	LastDamageEvent.Damage = Damage;
	LastDamageEvent.Warhead = Warhead;
}

bool DAEventManager::Vehicle_Flip_Event(VehicleGameObj *Vehicle) {
	for (int i = 0;i < Events[DAEvent::VEHICLEFLIP].Count();i++) {
		if (!Events[DAEvent::VEHICLEFLIP][i]->Base->Vehicle_Flip_Event(Vehicle)) {
			return false;
		}
	}
	LastDamageEvent.Type = DADamageType::FLIP;
	LastDamageEvent.Bone = "None";
	LastDamageEvent.Damage = 0.0f;
	LastDamageEvent.Warhead = 0;
	return true;
}

void DAEventManager::DAEventObserverClass::Killed(GameObject *obj,GameObject *Killer) {
	//Console_InputF("msg kill %s %s %f %u %d %s",obj->Get_Definition().Get_Name(),Killer?Killer->Get_Definition().Get_Name():"None",LastDamageEvent.Damage,LastDamageEvent.Warhead,LastDamageEvent.Type,LastDamageEvent.Bone);
	for (int i = 0;i < ObjectEvents[DAObjectEvent::KILLDEALT].Count();i++) {
		if (ObjectEvents[DAObjectEvent::KILLDEALT][i]->Check_Object_Type(Killer)) {
			ObjectEvents[DAObjectEvent::KILLDEALT][i]->Base->Kill_Event((DamageableGameObj*)obj,(ArmedGameObj*)Killer,LastDamageEvent.Damage,LastDamageEvent.Warhead,LastDamageEvent.Type,LastDamageEvent.Bone);
		}
	}
	for (int i = 0;i < ObjectEvents[DAObjectEvent::KILLRECEIVED].Count();i++) {
		if (ObjectEvents[DAObjectEvent::KILLRECEIVED][i]->Check_Object_Type(obj)) {
			ObjectEvents[DAObjectEvent::KILLRECEIVED][i]->Base->Kill_Event((DamageableGameObj*)obj,(ArmedGameObj*)Killer,LastDamageEvent.Damage,LastDamageEvent.Warhead,LastDamageEvent.Type,LastDamageEvent.Bone);
		}
	}
}

void DAEventManager::DAEventObserverClass::Custom(GameObject *obj,int Message,int Param,GameObject *Sender) {
	for (int i = 0;i < ObjectEvents[DAObjectEvent::CUSTOM].Count();i++) {
		if (ObjectEvents[DAObjectEvent::CUSTOM][i]->Check_Object_Type(obj)) {
			ObjectEvents[DAObjectEvent::CUSTOM][i]->Base->Custom_Event(obj,Message,Param,Sender);
		}
	}
	if (Message == CUSTOM_EVENT_VEHICLE_ENTERED) {
		for (int i = 0;i < Events[DAEvent::VEHICLEENTER].Count();i++) {
			Events[DAEvent::VEHICLEENTER][i]->Base->Vehicle_Enter_Event((VehicleGameObj*)obj,(SoldierGameObj*)Sender,Param);
		}
	}
	else if (Message == CUSTOM_EVENT_VEHICLE_EXITED) {
		for (int i = 0;i < Events[DAEvent::VEHICLEEXIT].Count();i++) {
			Events[DAEvent::VEHICLEEXIT][i]->Base->Vehicle_Exit_Event((VehicleGameObj*)obj,(SoldierGameObj*)Sender,Param);
		}
	}
}

void DAEventManager::DAEventObserverClass::Poked(GameObject *obj,GameObject *Poker) {
	for (int i = 0;i < ObjectEvents[DAObjectEvent::POKE].Count();i++) {
		if (ObjectEvents[DAObjectEvent::POKE][i]->Check_Object_Type(obj)) {
			ObjectEvents[DAObjectEvent::POKE][i]->Base->Poke_Event((PhysicalGameObj*)obj,(SoldierGameObj*)Poker);
		}
	}
}

void DAEventManager::DAEventObserverClass::Entered(GameObject *obj,GameObject *Enterer) {
	for (int i = 0;i < ObjectEvents[DAObjectEvent::ZONEENTER].Count();i++) {
		if (ObjectEvents[DAObjectEvent::ZONEENTER][i]->Check_Object_Type(Enterer)) {
			ObjectEvents[DAObjectEvent::ZONEENTER][i]->Base->Zone_Enter_Event((ScriptZoneGameObj*)obj,(PhysicalGameObj*)Enterer);
		}
	}
}

void DAEventManager::DAEventObserverClass::Exited(GameObject *obj,GameObject *Exiter) {
	for (int i = 0;i < ObjectEvents[DAObjectEvent::ZONEEXIT].Count();i++) {
		if (ObjectEvents[DAObjectEvent::ZONEEXIT][i]->Check_Object_Type(Exiter)) {
			ObjectEvents[DAObjectEvent::ZONEEXIT][i]->Base->Zone_Exit_Event((ScriptZoneGameObj*)obj,(PhysicalGameObj*)Exiter);
		}
	}
}

void DAEventManager::DAEventObserverClass::Destroyed(GameObject *obj) {
	SoldierGameObj *Soldier = obj->As_SoldierGameObj();
	if (Soldier) {
		cPlayer *Player = Soldier->Get_Player();
		if (Player) {
			//if (Soldier->Get_Player_Type() != Player->Get_Player_Type() && Soldier->Get_Definition().Get_Default_Player_Type() != Player->Get_Player_Type()) {
			if ((Soldier->Get_Player_Type() == 0 && Player->Get_Player_Type() == 1) || (Soldier->Get_Player_Type() == 1 && Player->Get_Player_Type() == 0)) {
				for (int i = 0;i < Events[DAEvent::TEAMCHANGE].Count();i++) {
					Events[DAEvent::TEAMCHANGE][i]->Base->Team_Change_Event(Player);
				}
			}
		}
	}
	VehicleGameObj *Vehicle = obj->As_VehicleGameObj();
	if (Vehicle) {
		int Seats = Vehicle->Get_Definition().Get_Seat_Count();
		for (int j = 0;j < Seats;j++) {
			if (Vehicle->Get_Occupant(j)) {
				for (int i = 0;i < Events[DAEvent::VEHICLEEXIT].Count();i++) {
					Events[DAEvent::VEHICLEEXIT][i]->Base->Vehicle_Exit_Event(Vehicle,Vehicle->Get_Occupant(j),j);
				}
			}
		}
	}
	for (int i = 0;i < ObjectEvents[DAObjectEvent::DESTROYED].Count();i++) {
		if (ObjectEvents[DAObjectEvent::DESTROYED][i]->Check_Object_Type(obj)) {
			ObjectEvents[DAObjectEvent::DESTROYED][i]->Base->Object_Destroyed_Event(obj);
		}
	}
}

void DAEventManager::DAEventConnectionAcceptanceFilterClass::handleInitiation(const ConnectionRequest &Request) {
	for (int i = 0;i < Events[DAEvent::PLAYERPREJOIN].Count();i++) {
		Events[DAEvent::PLAYERPREJOIN][i]->Base->Player_Pre_Join_Event(const_cast<ConnectionRequest&>(Request));
	}
}

ConnectionAcceptanceFilter::STATUS DAEventManager::DAEventConnectionAcceptanceFilterClass::getStatus(const ConnectionRequest &Request,WideStringClass &RefusalMessage) {
	for (int i = 0;i < Events[DAEvent::CONNECTIONREQUEST].Count();i++) {
		STATUS Return = Events[DAEvent::CONNECTIONREQUEST][i]->Base->Connection_Request_Event(const_cast<ConnectionRequest&>(Request),RefusalMessage);
		if (Return == ConnectionAcceptanceFilter::STATUS_REFUSING) {
			Console_Output("Refused connection from %ls(%s)(%s)(%.2f): %ls\n",Request.clientName,Long_To_IP(Request.clientAddress.sin_addr.s_addr),Request.clientSerialHash,Request.clientVersion,RefusalMessage);
			DALogManager::Write_Log("_CONNECTIONREFUSED","Refused connection from %ls(%s)(%s)(%.2f): %ls",Request.clientName,Long_To_IP(Request.clientAddress.sin_addr.s_addr),Request.clientSerialHash,Request.clientVersion,RefusalMessage);
			return ConnectionAcceptanceFilter::STATUS_REFUSING;
		}
		else if (Return == ConnectionAcceptanceFilter::STATUS_INDETERMINATE) {
			return ConnectionAcceptanceFilter::STATUS_INDETERMINATE;
		}
	}
	return ConnectionAcceptanceFilter::STATUS_ACCEPTING;
}

void DAEventManager::Init() {
	AddHostHook(Host_Chat_Event);
	AddPlayerJoinHook(Player_Join_Event);
	AddPlayerLeaveHook(Player_Leave_Event);
	AddPreLoadLevelHook(Level_Loaded_Event);
	AddGameOverHook(Game_Over_Event);
	AddConsoleOutputHook(Console_Output_Event);
	AddLogFileHook(Ren_Log_Event);
	AddCharacterPurchaseHook((PurchaseHook)Character_Purchase_Request_Event,0);
	AddVehiclePurchaseHook((PurchaseHook)Vehicle_Purchase_Request_Event,0);
	AddPowerupPurchaseHook((PurchaseHook)PowerUp_Purchase_Request_Event,0);
	AddThinkHook(Think);
	AddRefillHook((RefillHook)Refill_Event);
	AddRadioHook(Radio_Event);
	AddStockDamageHook(Stock_Client_Damage_Request_Event);
	AddTtDamageHook(TT_Client_Damage_Request_Event);

	static ObjectCreateHookStruct ObjectHook;
	ObjectHook.hook = Object_Created_Event;
	ObjectHook.data = 0;
	AddObjectCreateHook(&ObjectHook);

	static DAEventConnectionAcceptanceFilterClass ConnFilter;
	addConnectionAcceptanceFilter(&ConnFilter);
}



void DAEventClass::Register_Chat_Command(DAECC Func,const char *Triggers,int Parameters,DAAccessLevel::Level AccessLevel,DAChatType::Type ChatType) {
	DAChatCommandManager::Register_Event_Chat_Command(this,Func,Triggers,Parameters,AccessLevel,ChatType);
}
void DAEventClass::Unregister_Chat_Command(const char *Trigger) {
	DAChatCommandManager::Unregister_Chat_Command(Trigger);
}
void DAEventClass::Clear_Chat_Commands() {
	DAChatCommandManager::Clear_Event_Chat_Commands(this);
}

void DAEventClass::Register_Key_Hook(DAEKH Func,const char *Triggers) {
	DAChatCommandManager::Register_Event_Key_Hook(this,Func,Triggers);
}
void DAEventClass::Unregister_Key_Hook(const char *Trigger) {
	DAChatCommandManager::Unregister_Key_Hook(Trigger);
}
void DAEventClass::Clear_Key_Hooks() {
	DAChatCommandManager::Clear_Event_Key_Hooks(this);
}

