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

#ifndef INCLUDE_DAVEHICLE
#define INCLUDE_DAVEHICLE

#include "da_event.h"
#include "da_gameobj.h"
#include "engine_game.h"

class DAVehicleObserverClass : public DAGameObjObserverClass {
public:
	virtual void Init();
	virtual void Timer_Expired(GameObject *obj,int Number);
	virtual void Vehicle_Enter(SoldierGameObj *Soldier,int Seat);
	virtual void Vehicle_Exit(SoldierGameObj *Soldier,int Seat);

	inline cPlayer *Get_Last_Driver() {
		return LastDriver;
	}
	inline cPlayer *Get_Last_Player() {
		return LastPlayer;
	}
	inline int Get_Team() {
		return Team;
	}
	inline void Set_Team(int team) {
		Team = team;
	}
	inline unsigned int Get_Time_Since_Last_Driver_Exit() {
		return The_Game()->Get_Game_Duration_S()-LastDriverExitTime;
	}
	inline unsigned int Get_Time_Since_Last_Exit() {
		return The_Game()->Get_Game_Duration_S()-LastExitTime;
	}
	virtual const char *Get_Name();
	inline VehicleGameObj *Get_Vehicle() {
		return (VehicleGameObj*)Get_Owner();
	}
	
private:
	cPlayer *LastDriver;
	cPlayer *LastPlayer;
	unsigned int LastDriverExitTime;
	unsigned int LastExitTime;
	int Team;
	bool GiveSteal;
};

class DAVehicleManager : public DAEventClass {
public:
	static void Init();
	DA_API static bool Check_Limit_For_Player(SoldierGameObj *Player);
	DA_API static DAVehicleObserverClass *Get_Vehicle_Data(GameObject *obj);
	DA_API static void Disable_Flip_Kill();
	DA_API static void Enable_Flip_Kill();
	inline static cPlayer *Get_Last_Driver(GameObject *obj) {
		DAVehicleObserverClass *Data = Get_Vehicle_Data(obj);
		if (Data) {
			return Data->Get_Last_Driver();
		}
		return 0;
	}
	inline static cPlayer *Get_Last_Player(GameObject *obj) {
		DAVehicleObserverClass *Data = Get_Vehicle_Data(obj);
		if (Data) {
			return Data->Get_Last_Player();
		}
		return 0;
	}
	inline static unsigned int Get_Time_Since_Last_Driver_Exit(GameObject *obj) {
		DAVehicleObserverClass *Data = Get_Vehicle_Data(obj);
		if (Data) {
			return Data->Get_Time_Since_Last_Driver_Exit();
		}
		return INT_MAX;
	}
	inline static unsigned int Get_Time_Since_Last_Exit(GameObject *obj) {
		DAVehicleObserverClass *Data = Get_Vehicle_Data(obj);
		if (Data) {
			return Data->Get_Time_Since_Last_Exit();
		}
		return INT_MAX;
	}
	inline static int Get_Team(GameObject *obj) {
		DAVehicleObserverClass *Data = Get_Vehicle_Data(obj);
		if (Data) {
			return Data->Get_Team();
		}
		return 2;
	}
	inline static void Set_Team(GameObject *obj,int Team) {
		DAVehicleObserverClass *Data = Get_Vehicle_Data(obj);
		if (Data) {
			Data->Set_Team(Team);
		}
	}
	
private:
	class DefaultPurchaseEvent : public DAEventClass {
		virtual int Vehicle_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const VehicleGameObjDef *Item);
	};
	virtual void Settings_Loaded_Event();
	virtual void Object_Created_Event(GameObject *obj);
	virtual int Vehicle_Purchase_Request_Event(BaseControllerClass *Base,SoldierGameObj *Purchaser,float &Cost,const VehicleGameObjDef *Item);
	virtual void Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone);
	virtual bool Vehicle_Flip_Event(VehicleGameObj *Vehicle);

	static DAVehicleManager Instance;
};

#endif