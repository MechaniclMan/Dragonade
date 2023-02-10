/*	Renegade Scripts.dll
    Dragonade Player Manager
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
#include "da_player.h"
#include "da_translation.h"
#include "da_settings.h"
#include "da_log.h"
#include "cScTextObj.h"
#include "SCAnnouncement.h"
#include "WeaponBagClass.h"
#include "WeaponClass.h"
#include "weaponmgr.h"

DynamicVectorClass<DAPlayerDataFactoryClass*> DAPlayerManager::DataFactories;
DynamicVectorClass<DAPlayerClass*> DAPlayerManager::Players;
WideStringClass DAPlayerManager::DisallowedNickFirstCharacters;
WideStringClass DAPlayerManager::DisallowedNickCharacters;
DynamicVectorClass<WideStringClass> DAPlayerManager::DisallowedNicks;


DAPlayerClass::DAPlayerClass(cPlayer *Player,const char *serial,float version) {
	Player->Set_DA_Player(this);
	Owner = Player;
	Serial = serial;
	Version = version;
	AccessLevel = DAAccessLevel::NONE;
	Muted = false;
	C4Locked = false;
	BeaconLocked = false;
	VehicleLimit = Get_Vehicle_Limit();
	C4Limit = Get_Mine_Limit();
	Loaded = false;
	LastTibDamageTime = 0;
}

DAPlayerClass::~DAPlayerClass() {
	for (int i = 0;i < Observers.Count();i++) {
		delete Observers[i];
	}
	Observers.Delete_All();
	for (int i = 0;i < Data.Count();i++) {
		delete Data[i];
	}
	Data.Delete_All();
	for (int i = 0;i < Tags.Count();i++) {
		delete Tags[i];
	}
	Tags.Delete_All();
}

void DAPlayerObserverClass::Start_Timer(int Number,float Duration,bool Repeat,unsigned int Data) {
	DAPlayerObserverTimerStruct *Timer = new DAPlayerObserverTimerStruct;
	Timer->Number = Number;
	Timer->Repeat = Repeat;
	Timer->Data = Data;
	Timer->Duration = (unsigned long)(Duration*1000);
	Timer->StartTime = GetTickCount();
	Timers.Add(Timer);
}

void DAPlayerObserverClass::Stop_Timer(int Number,unsigned int Data) {
	for (int i = Timers.Count()-1;i >= 0;i--) {
		if (Timers[i]->Number == Number && (!Data || Timers[i]->Data == Data)) {
			Timers[i]->Number = -1;
		}
	}
}

void DAPlayerObserverClass::Clear_Timers() {
	for (int i = Timers.Count()-1;i >= 0;i--) {
		delete Timers[i];
	}
	Timers.Delete_All();
}

void DAPlayerObserverClass::Register_Chat_Command(DAPOCC Func,const char *Triggers,int Parameters,DAAccessLevel::Level AccessLevel,DAChatType::Type ChatType) {
	DAPlayerObserverChatCommandStruct *Struct = new DAPlayerObserverChatCommandStruct;
	Struct->AccessLevel = AccessLevel;
	Struct->ChatType = ChatType;
	Struct->Parameters = Parameters;
	DATokenParserClass Parser(Triggers,'|');
	while (char *Token = Parser.Get_String()) {
		_strlwr(Token);
		Struct->Triggers.Add(Get_Hash(Token));
	}
	Struct->Func = Func;
	ChatCommands.Add(Struct);
}

void DAPlayerObserverClass::Unregister_Chat_Command(const char *Trigger) {
	unsigned int Hash = StringClass(Trigger).GetHash();
	for (int i = ChatCommands.Count()-1;i >= 0;i--) {
		if (ChatCommands[i]->Triggers.ID(Hash) != -1) {
			delete ChatCommands[i];
			ChatCommands.Delete(i);
		}
	}
}

void DAPlayerObserverClass::Clear_Chat_Commands() {
	for (int i = ChatCommands.Count()-1;i >= 0;i--) {
		delete ChatCommands[i];
	}
	ChatCommands.Delete_All();
}

void DAPlayerObserverClass::Register_Key_Hook(DAPOKH Func,const char *Triggers) {
	DAPlayerObserverKeyHookStruct *Struct = new DAPlayerObserverKeyHookStruct;
	DATokenParserClass Parser(Triggers,'|');
	while (char *Token = Parser.Get_String()) {
		_strlwr(Token);
		Struct->Triggers.Add(Get_Hash(Token));
	}
	Struct->Func = Func;
	KeyHooks.Add(Struct);
}

void DAPlayerObserverClass::Unregister_Key_Hook(const char *Trigger) {
	unsigned int Hash = StringClass(Trigger).GetHash();
	for (int i = KeyHooks.Count()-1;i >= 0;i--) {
		if (KeyHooks[i]->Triggers.ID(Hash) != -1) {
			delete KeyHooks[i];
			KeyHooks.Delete(i);
		}
	}
}

void DAPlayerObserverClass::Clear_Key_Hooks() {
	for (int i = KeyHooks.Count()-1;i >= 0;i--) {
		delete KeyHooks[i];
	}
	KeyHooks.Delete_All();
}




void DAPlayerClass::Add_Observer(DAPlayerObserverClass *Observer) {
	Observer->Set_Owner(Get_Owner());
	Observer->Init();
	if (Get_GameObj()) {
		Observer->Created();
	}
	Observers.Add(Observer);
}

void DAPlayerClass::Remove_Observer(const char *Name) {
	for (int i = 0;i < Observers.Count();i++) {
		if (!_stricmp(Observers[i]->Get_Observer_Name(),Name)) {
			Observers[i]->Set_Delete_Pending();
		}
	}
}

DAPlayerObserverClass *DAPlayerClass::Find_Observer(const char *Name) {
	for (int i = 0;i < Observers.Count();i++) {
		if (!_stricmp(Observers[i]->Get_Observer_Name(),Name)) {
			return Observers[i];
		}
	}
	return 0;
}

void DAPlayerClass::Add_Data(DAPlayerDataClass *data) {
	data->Set_Owner(Get_Owner());
	data->Init();
	Data.Add(data);
}

void DAPlayerClass::Remove_Data(const DAPlayerDataFactoryClass *Factory) {
	for (int i = Observers.Count()-1;i >= 0;i--) {
		if (Data[i]->Get_Factory() == Factory) {
			delete Data[i];
			Data.Delete(i);
		}
	}
}

DAPlayerDataClass *DAPlayerClass::Find_Data(const DAPlayerDataFactoryClass *Factory) {
	for (int i = 0;i < Data.Count();i++) {
		if (Data[i]->Get_Factory() == Factory) {
			return Data[i];
		}
	}
	return 0;
}

void DAPlayerClass::Update_Tag() {
	StringClass Tag;
	for (int i = 0;i < Tags.Count();i++) {
		Tag += Tags[i]->Tag;
		Tag += "\n";
	}
	Get_Owner()->customTag = Tag;
	Get_Owner()->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
}

void DAPlayerClass::Add_Tag(const char *Tag,int Position,DAPlayerFlags::Flag Flags) {
	DAPlayerTagStruct *NewTag = new DAPlayerTagStruct;
	NewTag->Tag = Tag;
	NewTag->Position = Position;
	NewTag->Flags = Flags;
	for (int i = 0;i < Tags.Count();i++) {
		if (Tags[i]->Position < Position) {
			Tags.Insert(i,NewTag);
			Update_Tag();
			return;
		}
	}
	Tags.Add(NewTag);
	Update_Tag();
}

void DAPlayerClass::Remove_Tag(const char *Tag) {
	for (int i = 0;i < Tags.Count();i++) {
		if (Tags[i]->Tag == Tag) {
			delete Tags[i];
			Tags.Delete(i);
			i--;
		}
	}
	Update_Tag();
}

void DAPlayerClass::Remove_Tags_With_Flag(DAPlayerFlags::Flag Flag) {
	for (int i = 0;i < Tags.Count();i++) {
		if (Tags[i]->Has_Flag(Flag)) {
			delete Tags[i];
			Tags.Delete(i);
			i--;
		}
	}
	Update_Tag();
}

void DAPlayerClass::Beacon_Lock(bool lock) {
	BeaconLocked = lock;
	if (lock) {
		SoldierGameObj *Soldier = (SoldierGameObj*)Get_GameObj();
		if (Soldier) {
			WeaponBagClass *Bag = Soldier->Get_Weapon_Bag();
			for (int i = Bag->Get_Count()-1;i >= 1;i--) {
				if (Bag->Peek_Weapon(i)->Get_Definition()->Style == STYLE_BEACON) {
					Bag->Remove_Weapon(i);
				}
			}
		}
		DA::Page_Player(Get_ID(),"You have been beacon locked. You can no longer use beacons.");
	}
	else {
		DA::Page_Player(Get_ID(),"You are no longer beacon locked.");
	}
}

void DAPlayerClass::C4_Lock(bool lock) {
	C4Locked = lock;
	if (lock) {
		SoldierGameObj *Soldier = (SoldierGameObj*)Get_GameObj();
		if (Soldier) {
			WeaponBagClass *Bag = Soldier->Get_Weapon_Bag();
			for (int i = Bag->Get_Count()-1;i >= 1;i--) {
				if (Bag->Peek_Weapon(i)->Get_Definition()->Style == STYLE_C4) {
					if (Bag->Peek_Weapon(i)->PrimaryAmmoDefinition->AmmoType != 2) {
						Bag->Remove_Weapon(i);
					}
				}
			}
		}
		DA::Page_Player(Get_ID(),"You have been C4 locked. You can no longer use Remote or Proximity C4");
	}
	else {
		DA::Page_Player(Get_ID(),"You are no longer C4 locked.");
	}
}

bool DAPlayerClass::Is_Flooding() {
	for (int i = FloodProtection.Count();i-- > 0;) {
		if (GetTickCount()-FloodProtection[i] >= 10000) {
			FloodProtection.Delete(i);
		}
	}
	FloodProtection.Add(GetTickCount());
	if (FloodProtection.Count() >= 7) {
		Send_Announcement_Player(Get_ID(),"IDS_FLOOD_MSG");
		return true;
	}
	return false;
}

void DAPlayerClass::Join() {
	Get_Owner()->Set_DA_Player(this);
	for (int i = 0;i < Observers.Count();i++) {
		if (Observers[i]->Has_Flag(DAPlayerFlags::PERSISTLEAVE)) {
			Observers[i]->Join();
		}
	}
	Loaded = false;
}

void DAPlayerClass::Leave() {
	AccessLevel = DAAccessLevel::NONE;
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->Has_Flag(DAPlayerFlags::PERSISTLEAVE)) {
			Observers[i]->Set_Delete_Pending();
		}
		else {
			Observers[i]->Leave();
		}
	}
	
	for (int i = 0;i < Data.Count();i++) {
		Data[i]->Clear_Session();
	}
	
	for (int i = 0;i < Tags.Count();i++) {
		if (!Tags[i]->Has_Flag(DAPlayerFlags::PERSISTLEAVE)) {
			delete Tags[i];
			Tags.Delete(i);
			i--;
		}
	}
	Update_Tag();
}

void DAPlayerClass::Level_Loaded() {
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->Has_Flag(DAPlayerFlags::PERSISTMAP)) {
			Observers[i]->Set_Delete_Pending();
		}
		else {
			Observers[i]->Level_Loaded();
		}
	}

	for (int i = 0;i < Data.Count();i++) {
		Data[i]->Clear_Level();
	}
	
	for (int i = 0;i < Tags.Count();i++) {
		if (!Tags[i]->Has_Flag(DAPlayerFlags::PERSISTMAP)) {
			delete Tags[i];
			Tags.Delete(i);
			i--;
		}
	}
	Update_Tag();
	Loaded = false;
}

void DAPlayerClass::Player_Loaded() {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Player_Loaded();
	}
}

void DAPlayerClass::Name_Change() {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Name_Change();
	}
}

bool DAPlayerClass::Chat(TextMessageEnum Type,const wchar_t *Message,int ReceiverID) {
	if (Is_Muted() && Type != TEXT_MESSAGE_PRIVATE) {
		cScTextObj *Text = Send_Client_Text(Message,Type,false,Get_ID(),ReceiverID,false,false);
		Text->Set_Object_Dirty_Bits(Get_ID(),NetworkObjectClass::BIT_CREATION);
		return false;
	}
	else if (Is_Flooding()) {
		return false;
	}
	return true;
}

bool DAPlayerClass::Radio(int PlayerType,int AnnouncementID,int IconID,AnnouncementEnum AnnouncementType) {
	if (Is_Muted()) {
		SCAnnouncement *Radio = Send_Client_Announcement(PlayerType,Get_ID(),AnnouncementID,AnnouncementType,IconID,false,false);
		Radio->Set_Object_Dirty_Bits(Get_ID(),NetworkObjectClass::BIT_CREATION);
		return false;
	}
	else if (Is_Flooding()) {
		return false;
	}
	return true;
}

void DAPlayerClass::Team_Change() {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Team_Change();
	}
}

int DAPlayerClass::Character_Purchase_Request(float &Cost,const SoldierGameObjDef *Item) {
	for (int i = 0;i < Observers.Count();i++) {
		int Return = Observers[i]->Character_Purchase_Request(Cost,Item);
		if (Return != -1) {
			return Return;
		}
	}
	return -1;
}

int DAPlayerClass::Vehicle_Purchase_Request(float &Cost,const VehicleGameObjDef *Item) {
	for (int i = 0;i < Observers.Count();i++) {
		int Return = Observers[i]->Vehicle_Purchase_Request(Cost,Item);
		if (Return != -1) {
			return Return;
		}
	}
	return -1;
}

int DAPlayerClass::PowerUp_Purchase_Request(float &Cost,const PowerUpGameObjDef *Item) {
	if (Item->GrantWeapon) {
		WeaponDefinitionClass *WeapDef = (WeaponDefinitionClass*)Find_Definition(Item->GrantWeaponID);
		if (WeapDef) {
			if (Is_C4_Locked() && WeapDef->Style == STYLE_C4) {
				if (((AmmoDefinitionClass*)Find_Definition(WeapDef->PrimaryAmmoDefID))->AmmoType != 2) {
					return 4;
				}
			}
			else if (Is_Beacon_Locked() && WeapDef->Style == STYLE_BEACON) {
				return 4;
			}
		}
	}
	for (int i = 0;i < Observers.Count();i++) {
		int Return = Observers[i]->PowerUp_Purchase_Request(Cost,Item);
		if (Return != -1) {
			return Return;
		}
	}
	return -1;
}

int DAPlayerClass::Custom_Purchase_Request(float &Cost,unsigned int ID) {
	for (int i = 0;i < Observers.Count();i++) {
		int Return = Observers[i]->Custom_Purchase_Request(Cost,ID);
		if (Return != -1) {
			return Return;
		}
	}
	return -1;
}

void DAPlayerClass::Character_Purchase(float Cost,const SoldierGameObjDef *Item) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Character_Purchase(Cost,Item);
	}
}

void DAPlayerClass::Vehicle_Purchase(float Cost,const VehicleGameObjDef *Item) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Vehicle_Purchase(Cost,Item);
	}
}

void DAPlayerClass::PowerUp_Purchase(float Cost,const PowerUpGameObjDef *Item) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->PowerUp_Purchase(Cost,Item);
	}
}

void DAPlayerClass::Custom_Purchase(float Cost,unsigned int ID) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Custom_Purchase(Cost,ID);
	}
}

bool DAPlayerClass::Vehicle_Entry_Request(VehicleGameObj *Vehicle,int &Seat) {
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->Vehicle_Entry_Request(Vehicle,Seat)) {
			return false;
		}
	}
	return true;
}

void DAPlayerClass::Vehicle_Enter(VehicleGameObj *Vehicle,int Seat) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Vehicle_Enter(Vehicle,Seat);
	}
}

void DAPlayerClass::Vehicle_Exit(VehicleGameObj *Vehicle,int Seat) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Vehicle_Exit(Vehicle,Seat);
	}
}

bool DAPlayerClass::PowerUp_Grant_Request(const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	if (PowerUp->GrantWeapon) {
		WeaponDefinitionClass *WeapDef = (WeaponDefinitionClass*)Find_Definition(PowerUp->GrantWeaponID);
		if (WeapDef) {
			if (Is_C4_Locked() && WeapDef->Style == STYLE_C4) {
				if (((AmmoDefinitionClass*)Find_Definition(WeapDef->PrimaryAmmoDefID))->AmmoType != 2) {
					return false;
				}
			}
			else if (Is_Beacon_Locked() && WeapDef->Style == STYLE_BEACON) {
				return false;
			}
		}
	}
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->PowerUp_Grant_Request(PowerUp,PowerUpObj)) {
			return false;
		}
	}
	return true;
}

void DAPlayerClass::PowerUp_Grant(const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->PowerUp_Grant(PowerUp,PowerUpObj);
	}
}

bool DAPlayerClass::Add_Weapon_Request(const WeaponDefinitionClass *Weapon) {
	if (Weapon) {
		if (Is_C4_Locked() && Weapon->Style == STYLE_C4) {
			if (((AmmoDefinitionClass*)Find_Definition(Weapon->PrimaryAmmoDefID))->AmmoType != 2) {
				return false;
			}
		}
		else if (Is_Beacon_Locked() && Weapon->Style == STYLE_BEACON) {
			return false;
		}
	}
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->Add_Weapon_Request(Weapon)) {
			return false;
		}
	}
	return true;
}

void DAPlayerClass::Add_Weapon(WeaponClass *Weapon) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Add_Weapon(Weapon);
	}
}

void DAPlayerClass::Remove_Weapon(WeaponClass *Weapon) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Remove_Weapon(Weapon);
	}
}

void DAPlayerClass::Clear_Weapons() {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Clear_Weapons();
	}
}

bool DAPlayerClass::C4_Detonate_Request(C4GameObj *C4) {
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->C4_Detonate_Request(C4)) {
			return false;
		}
	}
	return true;
}

void DAPlayerClass::C4_Detonate(C4GameObj *C4) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->C4_Detonate(C4);
	}
}

void DAPlayerClass::Change_Character(const SoldierGameObjDef *SoldierDef) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Change_Character(SoldierDef);
	}
}

void DAPlayerClass::Created() {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Created();
	}
}

void DAPlayerClass::Destroyed() {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Destroyed();
	}
}

bool DAPlayerClass::Damage_Dealt_Request(DamageableGameObj *Victim,OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone) {
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->Damage_Dealt_Request(Victim,Offense,Type,Bone)) {
			return false;
		}
	}
	return true;
}

bool DAPlayerClass::Damage_Received_Request(OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone) {
	for (int i = 0;i < Observers.Count();i++) {
		if (!Observers[i]->Damage_Received_Request(Offense,Type,Bone)) {
			return false;
		}
	}
	return true;
}

void DAPlayerClass::Damage_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Damage_Dealt(Victim,Damage,Warhead,Type,Bone);
	}
}

void DAPlayerClass::Damage_Received(ArmedGameObj *Damager,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Damage_Received(Damager,Damage,Warhead,Type,Bone);
	}
}

void DAPlayerClass::Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Kill_Dealt(Victim,Damage,Warhead,Type,Bone);
	}
}

void DAPlayerClass::Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Kill_Received(Killer,Damage,Warhead,Type,Bone);
	}
}

void DAPlayerClass::Custom(GameObject *Sender,int Type,int Param) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Custom(Sender,Type,Param);
	}
}

void DAPlayerClass::Poked(SoldierGameObj *Poker) {
	for (int i = 0;i < Observers.Count();i++) {
		Observers[i]->Poked(Poker);
	}
}

void DAPlayerClass::Think() {
	for (int j = 0;j < Observers.Count();j++) {
		if (Observers[j]->Is_Delete_Pending()) {
			delete Observers[j];
			Observers.Delete(j);
			j--;
		}
		else {
			if (Observers[j]->Has_Flag(DAPlayerFlags::THINK)) {
				Observers[j]->Think();
			}
			DynamicVectorClass<DAPlayerObserverTimerStruct*> &Timers = Observers[j]->Get_Timers();
			for (int i = 0;i < Timers.Count();i++) {
				if (Timers[i]->Number == -1) {
					delete Timers[i];
					Timers.Delete(i);
					i--;
				}
				else if (GetTickCount()-Timers[i]->StartTime >= Timers[i]->Duration) {
					Observers[j]->Timer_Expired(Timers[i]->Number,Timers[i]->Data);
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
	}
}

	


class LegacyTagConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "tag"; }
	const char *Get_Help() { return "TAG <playerid> <tag> - Legacy command to set the custom name tag on a player. Persists through leaving or multiple maps; defaults to position 0"; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		int ID = 0;
		Parser.Get_Int(ID);
		cPlayer *Player = Find_Player(ID);
		if (Player) {
			Player->Get_DA_Player()->Remove_Tags_With_Flag(DAPlayerFlags::LEGACYTAG);
			if (Parser.Get_Remaining_String()) {
				Player->Get_DA_Player()->Add_Tag(Parser.Get_Remaining_String(),0,DAPlayerFlags::LEGACYTAG | DAPlayerFlags::PERSISTLEAVE | DAPlayerFlags::PERSISTMAP);
			}
		}
	}
};

class AddTagConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "addtag"; }
	const char *Get_Help() { return "ADDTAG <playerid> <persist on leave> <persist on map> <position> <tag> - Add a custom name tag to a player. Persist on map/leave determines if this tag should be removed when the player leaves the game or a new map loads. Position determintes the tag's position relative to the player's other tags."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		int ID = 0;
		Parser.Get_Int(ID);
		const char *PersistLeave = Parser.Get_String();
		const char *PersistMap = Parser.Get_String();
		int Position;
		Parser.Get_Int(Position);
		const char *Tag = Parser.Get_Remaining_String();
		cPlayer *Player = Find_Player(ID);
		if (Player && Tag) {
			DAPlayerFlags::Flag Flags = (DAPlayerFlags::Flag)0;
			if (!_stricmp(PersistLeave,"1") || !_stricmp(PersistLeave,"true") || !_stricmp(PersistLeave,"yes")) {
				Flags |= DAPlayerFlags::PERSISTLEAVE;
			}
			if (!_stricmp(PersistMap,"1") || !_stricmp(PersistMap,"true") || !_stricmp(PersistMap,"yes")) {
				Flags |= DAPlayerFlags::PERSISTMAP;
			}
			Player->Get_DA_Player()->Add_Tag(Tag,Position,Flags);
		}
	}
};

class RemoveTagConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "removetag"; }
	const char *Get_Help() { return "REMOVETAG <playerid> <tag> - Removes the given tag from the player."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		int ID = 0;
		Parser.Get_Int(ID);
		const char *Tag = Parser.Get_Remaining_String();
		cPlayer *Player = Find_Player(ID);
		if (Player && Tag) {
			Player->Get_DA_Player()->Remove_Tag(Tag);
		}
	}
};

class SetAccessLevelConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "setaccesslevel"; }
	const char *Get_Alias() { return "sal"; }
	const char *Get_Help() { return "SETACCESSLEVEL <playerid> <0-6> - Sets the given player's access level."; }
	void Activate(const char *ArgumentsString) {
		DATokenParserClass Parser(ArgumentsString,' ');
		int ID = 0;
		Parser.Get_Int(ID);
		int Level = 0;
		Parser.Get_Int(Level);
		cPlayer *Player = Find_Player(ID);
		if (Player) {
			Player->Get_DA_Player()->Set_Access_Level((DAAccessLevel::Level)Level);
		}
	}
};

class MuteConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "mute"; }
	const char *Get_Help() { return "MUTE <playerid> - Mute a player, preventing them from speaking in chat or using radio commands."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Mute(true);
		}
	}
};

class UnMuteConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "unmute"; }
	const char *Get_Help() { return "UNMUTE <playerid> - Unmute a player."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Mute(false);
		}
	}
};

class MuteToggleConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "mutetoggle"; }
	const char *Get_Help() { return "MUTETOGGLE <playerid> - Mute or unmute a player, preventing them from speaking in chat or using radio commands."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Mute(!Player->Get_DA_Player()->Is_Muted());
		}
	}
};

class BeaconLockConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "beaconlock"; }
	const char *Get_Alias() { return "beaconmute"; }
	const char *Get_Help() { return "BEACONLOCK <playerid> - Prevent a player from deploying beacons."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Beacon_Lock(true);
		}
	}
};

class UnBeaconLockConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "unbeaconlock"; }
	const char *Get_Alias() { return "unbeaconmute"; }
	const char *Get_Help() { return "UNBEACONLOCK <playerid> - Re-allow a player to deploy beacons."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Beacon_Lock(false);
		}
	}
};

class BeaconLockToggleConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "beaconlocktoggle"; }
	const char *Get_Alias() { return "beaconmutetoggle"; }
	const char *Get_Help() { return "BEACONLOCKTOGGLE <playerid> - Prevent or re-allow a player from deploying beacons."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->Beacon_Lock(!Player->Get_DA_Player()->Is_Beacon_Locked());
		}
	}
};

class C4LockConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "c4lock"; }
	const char *Get_Alias() { return "c4mute"; }
	const char *Get_Help() { return "C4LOCK <playerid> - Prevent a player from using remote and proximity C4."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->C4_Lock(true);
		}
	}
};

class UnC4LockConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "unc4lock"; }
	const char *Get_Alias() { return "unc4mute"; }
	const char *Get_Help() { return "UNC4LOCK <playerid> - Re-allow a player to use remote and proximity C4."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->C4_Lock(false);
		}
	}
};

class C4LockToggleConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "c4locktoggle"; }
	const char *Get_Alias() { return "c4mutetoggle"; }
	const char *Get_Help() { return "C4LOCKTOGGLE <playerid> - Prevent or re-allow a player from using remote and proximity C4."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Player->Get_DA_Player()->C4_Lock(!Player->Get_DA_Player()->Is_C4_Locked());
		}
	}
};

void DAPlayerManager::Init() {
	static DAPlayerManager Instance;
	
	Instance.Register_Event(DAEvent::CHAT,INT_MAX);
	Instance.Register_Event(DAEvent::RADIO,INT_MAX);
	Instance.Register_Event(DAEvent::LEVELLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::SETTINGSLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::CONNECTIONREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::PLAYERJOIN,INT_MAX);
	Instance.Register_Event(DAEvent::PLAYERLEAVE,INT_MIN);
	Instance.Register_Event(DAEvent::PLAYERLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::NAMECHANGE,INT_MAX);
	Instance.Register_Event(DAEvent::CHARACTERPURCHASEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEPURCHASEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPPURCHASEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEPURCHASE,INT_MAX);
	Instance.Register_Event(DAEvent::CHARACTERPURCHASE,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPPURCHASE,INT_MAX);
	Instance.Register_Event(DAEvent::TEAMCHANGE,INT_MAX);
	Instance.Register_Event(DAEvent::SUICIDE,INT_MIN);
	Instance.Register_Event(DAEvent::VEHICLEENTRYREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEENTER,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEEXIT,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPGRANTREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::POWERUPGRANT,INT_MAX);
	Instance.Register_Event(DAEvent::ADDWEAPONREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::ADDWEAPON,INT_MAX);
	Instance.Register_Event(DAEvent::REMOVEWEAPON,INT_MAX);
	Instance.Register_Event(DAEvent::CLEARWEAPONS,INT_MAX);
	Instance.Register_Event(DAEvent::C4DETONATEREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::C4DETONATE,INT_MAX);
	Instance.Register_Event(DAEvent::THINK,INT_MAX);
	
	Instance.Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::PLAYER,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DESTROYED,DAObjectEvent::PLAYER,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DAMAGERECEIVEDREQUEST,DAObjectEvent::ALL,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DAMAGERECEIVED,DAObjectEvent::ALL,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::ALL,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::CUSTOM,DAObjectEvent::PLAYER,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::POKE,DAObjectEvent::PLAYER,INT_MAX);
	
	Delete_Console_Function("tag");
	ConsoleFunctionList.Add(new LegacyTagConsoleCommand);
	ConsoleFunctionList.Add(new AddTagConsoleCommand);
	ConsoleFunctionList.Add(new RemoveTagConsoleCommand);
	ConsoleFunctionList.Add(new SetAccessLevelConsoleCommand);
	ConsoleFunctionList.Add(new MuteConsoleCommand);
	ConsoleFunctionList.Add(new UnMuteConsoleCommand);
	ConsoleFunctionList.Add(new MuteToggleConsoleCommand);
	ConsoleFunctionList.Add(new BeaconLockConsoleCommand);
	ConsoleFunctionList.Add(new UnBeaconLockConsoleCommand);
	ConsoleFunctionList.Add(new BeaconLockToggleConsoleCommand);
	ConsoleFunctionList.Add(new C4LockConsoleCommand);
	ConsoleFunctionList.Add(new UnC4LockConsoleCommand);
	ConsoleFunctionList.Add(new C4LockToggleConsoleCommand);
	Sort_Function_List();
	Verbose_Help_File();
}

void DAPlayerManager::Add_Data_Factory(DAPlayerDataFactoryClass *Factory) {
	DataFactories.Add(Factory);
	for (int i = 0;i < Players.Count();i++) {
		Players[i]->Add_Data(Factory->Create_Data());
	}
}

void DAPlayerManager::Remove_Data_Factory(DAPlayerDataFactoryClass *Factory) {
	DataFactories.DeleteObj(Factory);
	for (int i = 0;i < Players.Count();i++) {
		Players[i]->Remove_Data(Factory);
	}
}

bool DAPlayerManager::Check_Player(DAPlayerClass *DAPlayer) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data() == DAPlayer->Get_Owner()) {
			return true;
		}
	}
	return false;
}

bool DAPlayerManager::Chat_Event(cPlayer *Player,TextMessageEnum Type,const wchar_t *Message,int ReceiverID) {
	return Player->Get_DA_Player()->Chat(Type,Message,ReceiverID);
}

bool DAPlayerManager::Radio_Event(cPlayer *Player,int PlayerType,int AnnouncementID,int IconID,AnnouncementEnum AnnouncementType) {
	return Player->Get_DA_Player()->Radio(PlayerType,AnnouncementID,IconID,AnnouncementType);
}

void DAPlayerManager::Level_Loaded_Event() {
	for (int i = 0;i < Players.Count();i++) {
		DAPlayerClass *DAPlayer = Players[i];
		if (Check_Player(DAPlayer)) {
			DAPlayer->Level_Loaded();
		}
		else {
			delete DAPlayer;
			Players.Delete(i);
			i--;
		}
	}
}

void DAPlayerManager::Settings_Loaded_Event() {
	StringClass Buffer;
	DASettingsManager::Get_String(Buffer,"DisallowedNickFirstCharacters"," ");
	DisallowedNickFirstCharacters = Buffer.Peek_Buffer();
	DASettingsManager::Get_String(Buffer,"DisallowedNickCharacters"," ");
	Buffer += ": ;";
	DisallowedNickCharacters = Buffer.Peek_Buffer();
	
	DisallowedNicks.Delete_All();
	DASettingsManager::Get_String(Buffer,"DisallowedNicks",0);
	DATokenParserClass Parser(Buffer,'|');
	while (const char *Token = Parser.Get_String()) {
		DisallowedNicks.Add(Token);
	}
	DisallowedNicks.Add(L"Host");
	DisallowedNicks.Add(The_Game()->Owner);
	
	WideStringClass MessagePrefix = DA::Get_Message_Prefix();
	if (!MessagePrefix.Is_Empty()) {
		for (int i = 0;i < MessagePrefix.Get_Length();i++) {
			if (MessagePrefix[i] == '(' || MessagePrefix[i] == ')' ||MessagePrefix[i] == '[' ||MessagePrefix[i] == ']' ||MessagePrefix[i] == ':') {
				MessagePrefix.RemoveSubstring(i,1);
				i--;
			}
		}
		DisallowedNicks.Add(MessagePrefix);
	}
}

ConnectionAcceptanceFilter::STATUS DAPlayerManager::Connection_Request_Event(ConnectionRequest &Request,WideStringClass &RefusalMessage) {
	cGameData *TheGame = The_Game();
	if (TheGame->Is_Passworded() && Request.password.Compare(TheGame->Get_Password()) != 0) {
		RefusalMessage = L"Invalid password.";
		return ConnectionAcceptanceFilter::STATUS_REFUSING;
	}
	else if (Request.clientExeKey != TheGame->Get_Version_Number()) {
		RefusalMessage = L"Version mismatch.";
		return ConnectionAcceptanceFilter::STATUS_REFUSING;
	}	
	else if (TheGame->Get_Current_Players() > TheGame->Get_Max_Players()) {
		RefusalMessage = L"Game is full.";
		return ConnectionAcceptanceFilter::STATUS_REFUSING;
	}
	else if (Request.clientSerialHash.Is_Empty()) {
		return ConnectionAcceptanceFilter::STATUS_INDETERMINATE;
	}
	else if (Request.clientSerialHash.Get_Length() != 32) {
		RefusalMessage = L"Invalid serial.";
		return ConnectionAcceptanceFilter::STATUS_REFUSING;
	}
	/*else if (Request.clientVersion < ScriptsVersion){
		RefusalMessage = L"TT Scripts 4.0 are required to play in this server. Go to http://scripts.black-cell.net to download them.";
		return ConnectionAcceptanceFilter::STATUS_REFUSING;
	}*/
	else {
		CheckFirst:
		for (wchar_t *DisallowedPtr = DisallowedNickFirstCharacters.Peek_Buffer();*DisallowedPtr != '\0';DisallowedPtr++) {
			if (Request.clientName[0] == *DisallowedPtr) {
				Request.clientName.TruncateLeft(1);
				goto CheckFirst;
			}
		}
		for (int i = 0;i < Request.clientName.Get_Length();i++) {
			if (Request.clientName[i] < 33 || Request.clientName[i] > 126) {
				Request.clientName.RemoveSubstring(i,1);
				i--;
			}
			else {
				for (wchar_t *DisallowedPtr = DisallowedNickCharacters.Peek_Buffer();*DisallowedPtr != '\0';DisallowedPtr++) {
					if (Request.clientName[i] == *DisallowedPtr) {
						Request.clientName.RemoveSubstring(i,1);
						i--;
					}
				}
			}
		}
		if (Request.clientName.Get_Length() > 20) {
			Request.clientName.Erase(20,Request.clientName.Get_Length()-20);
		}
		if (DisallowedNicks.ID(Request.clientName) != -1) {
			RefusalMessage = L"Disallowed name.";
			return ConnectionAcceptanceFilter::STATUS_REFUSING;
		}
		else if (Request.clientName.Get_Length() <= 2) {
			RefusalMessage = L"Name too short.";
			return ConnectionAcceptanceFilter::STATUS_REFUSING;
		}
		else if (Find_Player(Request.clientName)) {
			cPlayer *Player = Find_Player(Request.clientName);
			if (Request.clientAddress.sin_addr.s_addr == (unsigned int)Player->Get_Ip_Address() && Request.clientSerialHash == Player->Get_DA_Player()->Get_Serial()) { //Client reconnecting before their ghost has timed out.
				Evict_Client(Player->Get_ID(),L"Ghost");
			}
			else {
				RefusalMessage = L"Name already in use.";
				return ConnectionAcceptanceFilter::STATUS_REFUSING;
			}
		}
	}
	return ConnectionAcceptanceFilter::STATUS_ACCEPTING;
}

void DAPlayerManager::Player_Join_Event(cPlayer *Player) {
	for (int i = 0;i < Players.Count();i++) {
		DAPlayerClass *DAPlayer = Players[i];
		if (DAPlayer->Get_Owner() == Player) {
			DAPlayer->Set_Serial(Get_Client_Serial_Hash(Player->Get_ID()));
			DAPlayer->Set_Version(Get_Client_Version(Player->Get_ID()));
			DAPlayer->Join();
			return;
		}
	}
	DAPlayerClass *DAPlayer = new DAPlayerClass(Player,Get_Client_Serial_Hash(Player->Get_ID()),Get_Client_Version(Player->Get_ID()));
	for (int i = 0;i < DataFactories.Count();i++) {
		DAPlayer->Add_Data(DataFactories[i]->Create_Data());
	}
	Players.Add(DAPlayer);
}

void DAPlayerManager::Player_Leave_Event(cPlayer *Player) {
	Player->Get_DA_Player()->Leave();
}

void DAPlayerManager::Player_Loaded_Event(cPlayer *Player) {
	Player->Get_DA_Player()->Player_Loaded();
}

void DAPlayerManager::Name_Change_Event(cPlayer *Player) {
	Player->Get_DA_Player()->Name_Change();
}

int DAPlayerManager::Character_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const SoldierGameObjDef *Item) {
	if (!Purchaser->Get_Defense_Object()->Get_Health()) {
		return 3;
	}
	return Purchaser->Get_DA_Player()->Character_Purchase_Request(Cost,Item);
}

int DAPlayerManager::Vehicle_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const VehicleGameObjDef *Item) {
	if (!Purchaser->Get_Defense_Object()->Get_Health()) {
		return 3;
	}
	return Purchaser->Get_DA_Player()->Vehicle_Purchase_Request(Cost,Item);
}

int DAPlayerManager::PowerUp_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const PowerUpGameObjDef *Item) {
	if (!Purchaser->Get_Defense_Object()->Get_Health()) {
		return 3;
	}
	return Purchaser->Get_DA_Player()->PowerUp_Purchase_Request(Cost,Item);
}

int DAPlayerManager::Custom_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,unsigned int ID) {
	if (!Purchaser->Get_Defense_Object()->Get_Health()) {
		return 3;
	}
	return Purchaser->Get_DA_Player()->Custom_Purchase_Request(Cost,ID);
}

void DAPlayerManager::Character_Purchase_Event(SoldierGameObj *Purchaser,float Cost,const SoldierGameObjDef *Item) {
	DALogManager::Write_Log("_PURCHASE","Purchase: %ls - %s",Purchaser->Get_Player()->Get_Name(),DATranslationManager::Translate(Item));
	Purchaser->Get_DA_Player()->Character_Purchase(Cost,Item);
}

void DAPlayerManager::Vehicle_Purchase_Event(SoldierGameObj *Purchaser,float Cost,const VehicleGameObjDef *Item) {
	DALogManager::Write_Log("_PURCHASE","Purchase: %ls - %s",Purchaser->Get_Player()->Get_Name(),DATranslationManager::Translate(Item));
	Purchaser->Get_DA_Player()->Vehicle_Purchase(Cost,Item);
}

void DAPlayerManager::PowerUp_Purchase_Event(SoldierGameObj *Purchaser,float Cost,const PowerUpGameObjDef *Item) {
	DALogManager::Write_Log("_PURCHASE","Purchase: %ls - %s",Purchaser->Get_Player()->Get_Name(),DATranslationManager::Translate(Item));
	Purchaser->Get_DA_Player()->PowerUp_Purchase(Cost,Item);
}

void DAPlayerManager::Custom_Purchase_Event(SoldierGameObj *Purchaser,float Cost,unsigned int ID) {
	Purchaser->Get_DA_Player()->Custom_Purchase(Cost,ID);
}

void DAPlayerManager::Team_Change_Event(cPlayer *Player) {
	Player->Get_DA_Player()->Team_Change();
}

bool DAPlayerManager::Suicide_Event(cPlayer *Player) {
	Player->Set_Money(0.0f);
	Player->Mark_As_Modified();
	Player->Destroy_GameObj();
	Send_Client_Text(WideStringFormat(L"%s committed suicide.",Player->Get_Name()),TEXT_MESSAGE_PUBLIC,false,-1,-1,true,true);
	return false;
}

bool DAPlayerManager::Vehicle_Entry_Request_Event(VehicleGameObj *Vehicle,SoldierGameObj *Soldier,int &Seat) {
	return Soldier->Get_DA_Player()->Vehicle_Entry_Request(Vehicle,Seat);
}

void DAPlayerManager::Vehicle_Enter_Event(VehicleGameObj *Vehicle,SoldierGameObj *Soldier,int Seat) {
	Soldier->Get_DA_Player()->Vehicle_Enter(Vehicle,Seat);
}

void DAPlayerManager::Vehicle_Exit_Event(VehicleGameObj *Vehicle,SoldierGameObj *Soldier,int Seat) {
	Soldier->Get_DA_Player()->Vehicle_Exit(Vehicle,Seat);
}

bool DAPlayerManager::PowerUp_Grant_Request_Event(SoldierGameObj *Soldier,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	if (!Soldier->Get_Defense_Object()->Get_Health()) {
		return false;
	}
	return Soldier->Get_DA_Player()->PowerUp_Grant_Request(PowerUp,PowerUpObj);
}

void DAPlayerManager::PowerUp_Grant_Event(SoldierGameObj *Soldier,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj) {
	Soldier->Get_DA_Player()->PowerUp_Grant(PowerUp,PowerUpObj);
}

bool DAPlayerManager::Add_Weapon_Request_Event(SoldierGameObj *Soldier,const WeaponDefinitionClass *Weapon) {
	return Soldier->Get_DA_Player()->Add_Weapon_Request(Weapon);
}

void DAPlayerManager::Add_Weapon_Event(SoldierGameObj *Soldier,WeaponClass *Weapon) {
	Soldier->Get_DA_Player()->Add_Weapon(Weapon);
}

void DAPlayerManager::Remove_Weapon_Event(SoldierGameObj *Soldier,WeaponClass *Weapon) {
	Soldier->Get_DA_Player()->Remove_Weapon(Weapon);
}

void DAPlayerManager::Clear_Weapons_Event(SoldierGameObj *Soldier) {
	Soldier->Get_DA_Player()->Clear_Weapons();
}

bool DAPlayerManager::C4_Detonate_Request_Event(C4GameObj *C4,SmartGameObj *Triggerer) {
	if (Is_Player(Triggerer)) {
		return ((SoldierGameObj*)Triggerer)->Get_DA_Player()->C4_Detonate_Request(C4);
	}
	return true;
}

void DAPlayerManager::C4_Detonate_Event(C4GameObj *C4,SmartGameObj *Triggerer) {
	if (Is_Player(Triggerer)) {
		((SoldierGameObj*)Triggerer)->Get_DA_Player()->C4_Detonate(C4);
	}
}

void DAPlayerManager::Change_Character_Event(SoldierGameObj *Soldier,const SoldierGameObjDef *SoldierDef) {
	Soldier->Get_DA_Player()->Change_Character(SoldierDef);
}
	
void DAPlayerManager::Think() {
	for (int i = 0;i < Players.Count();i++) {
		Players[i]->Think();
	}
}

void DAPlayerManager::Object_Created_Event(GameObject *obj) {
	((SoldierGameObj*)obj)->Get_DA_Player()->Created();
}

void DAPlayerManager::Object_Destroyed_Event(GameObject *obj) {
	((SoldierGameObj*)obj)->Get_DA_Player()->Destroyed();
}

bool DAPlayerManager::Damage_Request_Event(DamageableGameObj *Victim,OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone) {
	if (Is_Player(Offense->Get_Owner())) {
		if (!((SoldierGameObj*)Offense->Get_Owner())->Get_DA_Player()->Damage_Dealt_Request(Victim,Offense,Type,Bone)) {
			return false;
		}
	}
	if (Is_Player(Victim)) {
		return ((SoldierGameObj*)Victim)->Get_DA_Player()->Damage_Received_Request(Offense,Type,Bone);
	}
	return true;
}

void DAPlayerManager::Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	if (Is_Player(Damager)) {
		((SoldierGameObj*)Damager)->Get_DA_Player()->Damage_Dealt(Victim,Damage,Warhead,Type,Bone);
	}
	if (Is_Player(Victim)) {
		((SoldierGameObj*)Victim)->Get_DA_Player()->Damage_Received(Damager,Damage,Warhead,Type,Bone);
	}
}

void DAPlayerManager::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	if (Is_Player(Killer)) {
		((SoldierGameObj*)Killer)->Get_DA_Player()->Kill_Dealt(Victim,Damage,Warhead,Type,Bone);
	}
	if (Is_Player(Victim)) {
		((SoldierGameObj*)Victim)->Get_DA_Player()->Kill_Received(Killer,Damage,Warhead,Type,Bone);
	}
}

void DAPlayerManager::Custom_Event(GameObject *obj,int Type,int Param,GameObject *Sender) {
	((SoldierGameObj*)obj)->Get_DA_Player()->Custom(Sender,Type,Param);
}

void DAPlayerManager::Poke_Event(PhysicalGameObj *obj,SoldierGameObj *Poker) {
	((SoldierGameObj*)obj)->Get_DA_Player()->Poked(Poker);
}
