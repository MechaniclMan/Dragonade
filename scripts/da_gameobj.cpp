/*	Renegade Scripts.dll
    Dragonade Game Object Manager
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
#include "da_gameobj.h"

DynamicVectorClass<DAGameObjObserverClass*> DAGameObjManager::ObserversDeletePending;
DynamicVectorClass<GameObject*> DAGameObjManager::GameObjDeletePending;

void DAGameObjObserverClass::Attach(GameObject *obj) {
	int &NextID = *(int*)0x00812B1C;
	Set_ID(NextID);
	NextID++;
	_Owner = obj;
	Init();
}

void DAGameObjObserverClass::Detach(GameObject *obj) {
	delete this;
}

GameObject _declspec(naked) *DAGameObjObserverClass::Owner() {
	_asm {
		mov ecx, 1
		mov eax, 0
		retn
	}
}

void DAGameObjObserverClass::Start_Timer(int Number,float Duration) {
	Get_Owner()->Start_Observer_Timer(Get_ID(),Duration,Number);
}

void DAGameObjObserverClass::Stop_Timer(int Number) {
	Get_Owner()->Stop_Observer_Timer(Get_ID(),Number);
}

void DAGameObjObserverClass::Set_Delete_Pending() {
	DAGameObjManager::Set_Observer_Delete_Pending(this);
}

void DAGameObjManager::Init() {
	static DAGameObjManager Instance;
	Instance.Register_Event(DAEvent::THINK,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEENTRYREQUEST,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEENTER,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEEXIT,INT_MAX);
	Instance.Register_Event(DAEvent::VEHICLEFLIP,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DAMAGERECEIVEDREQUEST,DAObjectEvent::ALL,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DAMAGERECEIVED,DAObjectEvent::ALL,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::ALL,INT_MAX);
	Instance.Register_Object_Event(DAObjectEvent::DESTROYED,DAObjectEvent::ALL,INT_MAX);
}

bool DAGameObjManager::Is_DAGameObjObserverClass(GameObjObserverClass *Observer) {
	Observer->Owner();
	_asm {
		cmp ecx, 1
		jnz False
	}
	return true;

False:
	return false;
}

void DAGameObjManager::Set_Observer_Delete_Pending(DAGameObjObserverClass *Observer) {
	if (ObserversDeletePending.ID(Observer) == -1 && !Observer->Get_Owner()->Is_Delete_Pending()) {
		ObserversDeletePending.Add(Observer);
	}
}

//This is a workaround to a bug where you cannot destroy one object within the destroyed event of another.
void DAGameObjManager::Set_GameObj_Delete_Pending(GameObject *obj) {
	if (GameObjDeletePending.ID(obj) == -1 && !obj->Is_Delete_Pending()) {
		GameObjDeletePending.Add(obj);
	}
}

void DAGameObjManager::Think() {
	for (int i = 0;i < GameObjDeletePending.Count();i++) {
		GameObjDeletePending[i]->Set_Delete_Pending();
	}
	GameObjDeletePending.Delete_All();
	for (int i = 0;i < ObserversDeletePending.Count();i++) {
		ObserversDeletePending[i]->Get_Owner()->Remove_Observer(ObserversDeletePending[i]);
	}
	ObserversDeletePending.Delete_All();
}

bool DAGameObjManager::Vehicle_Entry_Request_Event(VehicleGameObj *Vehicle,SoldierGameObj *Soldier,int &Seat) {
	for (int i = 0;i < Vehicle->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Vehicle->Get_Observers()[i])) {
			if (!((DAGameObjObserverClass*)Vehicle->Get_Observers()[i])->Vehicle_Entry_Request(Soldier,Seat)) {
				return false;
			}
		}
	}
	for (int i = 0;i < Soldier->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Soldier->Get_Observers()[i])) {
			if (!((DAGameObjObserverClass*)Soldier->Get_Observers()[i])->Vehicle_Entry_Request(Vehicle,Seat)) {
				return false;
			}
		}
	}
	return true;
}

void DAGameObjManager::Vehicle_Enter_Event(VehicleGameObj *Vehicle,SoldierGameObj *Soldier,int Seat) {
	for (int i = 0;i < Vehicle->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Vehicle->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Vehicle->Get_Observers()[i])->Vehicle_Enter(Soldier,Seat);
		}
	}
	for (int i = 0;i < Soldier->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Soldier->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Soldier->Get_Observers()[i])->Vehicle_Enter(Vehicle,Seat);
		}
	}
}

void DAGameObjManager::Vehicle_Exit_Event(VehicleGameObj *Vehicle,SoldierGameObj *Soldier,int Seat) {
	for (int i = 0;i < Vehicle->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Vehicle->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Vehicle->Get_Observers()[i])->Vehicle_Exit(Soldier,Seat);
		}
	}
	for (int i = 0;i < Soldier->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Soldier->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Soldier->Get_Observers()[i])->Vehicle_Exit(Vehicle,Seat);
		}
	}
}

bool DAGameObjManager::Vehicle_Flip_Event(VehicleGameObj *Vehicle) {
	for (int i = 0;i < Vehicle->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Vehicle->Get_Observers()[i])) {
			if (!((DAGameObjObserverClass*)Vehicle->Get_Observers()[i])->Vehicle_Flip()) {
				return false;
			}
		}
	}
	return true;
}

bool DAGameObjManager::Damage_Request_Event(DamageableGameObj *Victim,OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone) {
	for (int i = 0;i < Victim->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Victim->Get_Observers()[i])) {
			if (!((DAGameObjObserverClass*)Victim->Get_Observers()[i])->Damage_Received_Request(Offense,Type,Bone)) {
				return false;
			}
		}
	}
	if (Offense->Get_Owner()) {
		for (int i = 0;i < Offense->Get_Owner()->Get_Observers().Count();i++) {
			if (Is_DAGameObjObserverClass(Offense->Get_Owner()->Get_Observers()[i])) {
				if (!((DAGameObjObserverClass*)Offense->Get_Owner()->Get_Observers()[i])->Damage_Dealt_Request(Victim,Offense,Type,Bone)) {
					return false;
				}
			}
		}
	}
	return true;
}

void DAGameObjManager::Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	for (int i = 0;i < Victim->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Victim->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Victim->Get_Observers()[i])->Damage_Received(Damager,Damage,Warhead,Type,Bone);
		}
	}
	if (Damager) {
		for (int i = 0;i < Damager->Get_Observers().Count();i++) {
			if (Is_DAGameObjObserverClass(Damager->Get_Observers()[i])) {
				((DAGameObjObserverClass*)Damager->Get_Observers()[i])->Damage_Dealt(Victim,Damage,Warhead,Type,Bone);
			}
		}
	}
}

void DAGameObjManager::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	for (int i = 0;i < Victim->Get_Observers().Count();i++) {
		if (Is_DAGameObjObserverClass(Victim->Get_Observers()[i])) {
			((DAGameObjObserverClass*)Victim->Get_Observers()[i])->Kill_Received(Killer,Damage,Warhead,Type,Bone);
		}
	}
	if (Killer) {
		for (int i = 0;i < Killer->Get_Observers().Count();i++) {
			if (Is_DAGameObjObserverClass(Killer->Get_Observers()[i])) {
				((DAGameObjObserverClass*)Killer->Get_Observers()[i])->Kill_Dealt(Victim,Damage,Warhead,Type,Bone);
			}
		}
	}
}

void DAGameObjManager::Object_Destroyed_Event(GameObject *obj) {
	for (int i = ObserversDeletePending.Count()-1;i >= 0;i--) {
		if (ObserversDeletePending[i]->Get_Owner() == obj) {
			ObserversDeletePending.Delete(i);
		}
	}
}