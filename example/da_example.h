/*	Renegade Scripts.dll
    Dragonade Example Plugin
	Copyright 2012 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "da_event.h"
#include "da_gamemode.h"
#include "da_gamefeature.h"
#include "da_player.h"
#include "da_gameobj.h"

//Game modes inherit from DAGameModeClass, and if you want any events you can inherit from DAEventClass too.
class DAExampleGameModeClass : public DAEventClass, public DAGameModeClass {
	virtual void Init(); //Called each time this game mode is loaded. Note that the game mode is unloaded and reloaded every map.

	//The events needed to implement this game mode.
	virtual int Character_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const SoldierGameObjDef *Item);
	virtual int PowerUp_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const PowerUpGameObjDef *Item);
	virtual bool Add_Weapon_Request_Event(SoldierGameObj *Soldier,const WeaponDefinitionClass *Weapon);
	virtual bool Damage_Request_Event(DamageableGameObj *Victim,OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone);
	virtual void Object_Created_Event(GameObject *obj);
};

//To use the player data manager you create a class that inherits from DAPlayerDataClass and use that class with DAPlayerDataManagerClass.
//See DAExampleGameFeatueClass.
class DAExamplePlayerDataClass : public DAPlayerDataClass {
public:
	virtual void Init() { //Called when the class is created for a new player.
		Purchased = false;
	}
	virtual void Clear_Level() { //Called when a new map is loaded.
		Purchased = false;
	}
	virtual void Clear_Session() { //Called when the player leaves the game.
	}
	bool Purchased;
};

//Game features inherit from DAGameFeatureClass, and if you want any events you can inherit from DAEventClass too.
class DAExampleGameFeatueClass : public DAEventClass, public DAGameFeatureClass, public DAPlayerDataManagerClass<DAExamplePlayerDataClass> {
	virtual void Init(); //Called when this game feature is first loaded.

	//The events needed to implement this game feature.
	virtual bool Team_Change_Request_Event(cPlayer *Player);
	virtual bool Damage_Request_Event(DamageableGameObj *Victim,OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone);
	virtual void Timer_Expired(int Number,unsigned int Data);
	bool Example1_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool Example2_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);

	float NodDamageScale;
	float GDIDamageScale;
};


class DAPlayerObserverExampleClass : public DAPlayerObserverClass {
	virtual void Init();
	bool Refill_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType);
	virtual const char *Get_Observer_Name() { return "DAPlayerObserverExampleClass"; } //This is required.
	virtual int Character_Purchase_Request(float &Cost,const SoldierGameObjDef *Item);
	virtual int Vehicle_Purchase_Request(float &Cost,const VehicleGameObjDef *Item);
	virtual int PowerUp_Purchase_Request(float &Cost,const PowerUpGameObjDef *Item);
	virtual bool Vehicle_Entry_Request(VehicleGameObj *Vehicle,int &Seat);
	virtual void Vehicle_Enter(VehicleGameObj *Vehicle,int Seat);
	virtual void Vehicle_Exit(VehicleGameObj *Vehicle,int Seat);
	virtual bool C4_Detonate_Request(C4GameObj *C4);
	virtual void Created();
	virtual bool Damage_Dealt_Request(DamageableGameObj *Victim,OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone);
	virtual bool Damage_Received_Request(OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone);
	virtual void Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone);
	virtual void Kill_Received(ArmedGameObj *Killer,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone);
};

class DAExampleObserverClass : public DAGameObjObserverClass {
	virtual void Init();
	~DAExampleObserverClass();
	virtual void Destroyed(GameObject *obj);
	virtual void Timer_Expired(GameObject *obj,int Number);
	virtual bool Vehicle_Entry_Request(SoldierGameObj *Soldier,int &Seat);
	virtual bool Damage_Received_Request(OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone);
	virtual void Kill_Dealt(DamageableGameObj *Victim,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone);
	const char *Get_Name() { return "DAExampleObserverClass"; } //This is required.
};