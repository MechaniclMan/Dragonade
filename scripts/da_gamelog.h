/*	Renegade Scripts.dll
    Dragonade Gamelog
	Copyright 2012 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAGAMELOG
#define INCLUDE_DAGAMELOG

#include "da_event.h"
#include "da_gamefeature.h"

class DAGameLogGameFeatureClass: public DAEventClass, public DAGameFeatureClass {
	virtual void Init();
	virtual void Level_Loaded_Event();
	virtual void Game_Over_Event();
	virtual void Vehicle_Enter_Event(VehicleGameObj *Vehicle,SoldierGameObj *Soldier,int Seat);
	virtual void Vehicle_Exit_Event(VehicleGameObj *Vehicle,SoldierGameObj *Soldier,int Seat);
	virtual void Object_Created_Event(GameObject *obj);
	virtual void Damage_Event(DamageableGameObj *Victim,ArmedGameObj *Damager,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone);
	virtual void Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone);
	virtual void Object_Destroyed_Event(GameObject *obj);
};

#endif