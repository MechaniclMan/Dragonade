/*	Renegade Scripts.dll
	Copyright 2011 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
//Changes made in DA:
//Moved various DefenseObjectClass and PhysicalGameObj functions to their respective header files
//Added dirty bit to Set_Collision_Group.
#include "general.h"
#include "PowerUpGameObjDef.h"
#include "PowerUpGameObj.h"
#include "scripts.h"
#include "BuildingGameObj.h"
#include "GameObjManager.h"
#include "wwmath.h"
#include "VehicleGameObj.h"
#include "SoldierGameObj.h"
#include "engine_def.h"
#include "RefineryGameObj.h"
#include "ScriptZoneGameObj.h"
#include "MoveablePhysClass.h"
#include "SmartGameObjDef.h"
#include "VehicleFactoryGameObj.h"
#include "VehicleFactoryGameObjDef.h"
#include "TransitionGameObjDef.h"
#include "AirFactoryGameObjDef.h"
#include "AirFactoryGameObj.h"
#include "NavalFactoryGameObjDef.h"
#include "NavalFactoryGameObj.h"
#include "AirStripGameObjDef.h"
#include "AirStripGameObj.h"
#include "BeaconGameObjDef.h"
#include "BeaconGameObj.h"
#include "C4GameObjDef.h"
#include "C4GameObj.h"
#include "CinematicGameObjDef.h"
#include "CinematicGameObj.h"
#include "ComCenterGameObjDef.h"
#include "ComCenterGameObj.h"
#include "ConstructionYardGameObjDef.h"
#include "ConstructionYardGameObj.h"
#include "DamageZoneGameObjDef.h"
#include "DamageZoneGameObj.h"
#include "PowerPlantGameObjDef.h"
#include "PowerPlantGameObj.h"
#include "RepairBayGameObjDef.h"
#include "RepairBayGameObj.h"
#include "SamSiteGameObjDef.h"
#include "SamSiteGameObj.h"
#include "SoldierFactoryGameObjDef.h"
#include "SoldierFactoryGameObj.h"
#include "SpecialEffectsGameObjDef.h"
#include "SpecialEffectsGameObj.h"
#include "TransitionGameObjDef.h"
#include "TransitionGameObj.h"
#include "WarFactoryGameObjDef.h"
#include "WarFactoryGameObj.h"

SCRIPTS_API const AirFactoryGameObjDef &
AirFactoryGameObj::Get_Definition (void) const
{
	return (const AirFactoryGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API  const NavalFactoryGameObjDef &
NavalFactoryGameObj::Get_Definition (void) const
{
	return (const NavalFactoryGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API  const AirStripGameObjDef & AirStripGameObj::Get_Definition( void ) const
{
	return (const AirStripGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API  const ArmedGameObjDef & ArmedGameObj::Get_Definition( void ) const
{
	return (const ArmedGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const BeaconGameObjDef & BeaconGameObj::Get_Definition( void ) const
{
	return (const BeaconGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const C4GameObjDef & C4GameObj::Get_Definition( void ) const
{
	return (const C4GameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const CinematicGameObjDef & CinematicGameObj::Get_Definition( void ) const
{
	return (const CinematicGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const ComCenterGameObjDef &
ComCenterGameObj::Get_Definition (void) const
{
	return (const ComCenterGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API const ConstructionYardGameObjDef &
ConstructionYardGameObj::Get_Definition (void) const
{
	return (const ConstructionYardGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API const DamageZoneGameObjDef & DamageZoneGameObj::Get_Definition( void ) const
{
	return (const DamageZoneGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const PowerPlantGameObjDef &
PowerPlantGameObj::Get_Definition (void) const
{
	return (const PowerPlantGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API const RepairBayGameObjDef &
RepairBayGameObj::Get_Definition (void) const
{
	return (const RepairBayGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API const SamSiteGameObjDef & SamSiteGameObj::Get_Definition( void ) const
{
	return (const SamSiteGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const SoldierGameObjDef & SoldierGameObj::Get_Definition( void ) const
{
	return (const SoldierGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const SoldierFactoryGameObjDef &
SoldierFactoryGameObj::Get_Definition (void) const
{
	return (const SoldierFactoryGameObjDef &)BaseGameObj::Get_Definition ();
}
SCRIPTS_API const SpecialEffectsGameObjDef & SpecialEffectsGameObj::Get_Definition( void ) const
{
	return (const SpecialEffectsGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const TransitionGameObjDef & TransitionGameObj::Get_Definition( void ) const
{
	return (const TransitionGameObjDef &)BaseGameObj::Get_Definition();
}
SCRIPTS_API const WarFactoryGameObjDef &
WarFactoryGameObj::Get_Definition (void) const
{
	return (const WarFactoryGameObjDef &)BaseGameObj::Get_Definition ();
}

SCRIPTS_API const VehicleFactoryGameObjDef &
VehicleFactoryGameObj::Get_Definition (void) const
{
	return (const VehicleFactoryGameObjDef &)BaseGameObj::Get_Definition ();
}

SCRIPTS_API const ScriptZoneGameObjDef & ScriptZoneGameObj::Get_Definition( void ) const
{
	return (const ScriptZoneGameObjDef &)BaseGameObj::Get_Definition();
}

unsigned char NetworkObjectClass::Get_Object_Dirty_Bits(int clientId)
{
	return ClientStatus[clientId];
}

bool NetworkObjectClass::Get_Object_Dirty_Bit(int clientId, DIRTY_BIT level)
{
	return (ClientStatus[clientId] & level) == level;
}

void DefenseObjectClass::Set_Health(float health) {
	if (health > HealthMax) {
		Health = HealthMax;
	}
	else {
		Health = health;
	}
	Mark_Owner_Dirty();
}

void DefenseObjectClass::Add_Health(float add_health) {
	Health += add_health;
	if (Health > HealthMax) {
		Health = HealthMax;
	}
	Mark_Owner_Dirty();
}

void DefenseObjectClass::Set_Health_Max(float health)
{
	HealthMax = WWMath::Clamp(health, 0, MAX_MAX_HEALTH);
	if (HealthMax < Health)
	{
		Health = HealthMax;
	}
	Mark_Owner_Dirty();
}

void DefenseObjectClass::Set_Shield_Type( ArmorType type )	
{ 
	ShieldType = type; 
	Mark_Owner_Dirty();
}

float DefenseObjectClass::Get_Health() const
{
	return this->Health;
}

float DefenseObjectClass::Get_Health_Max() const
{
	return this->HealthMax;
}

void DefenseObjectClass::Set_Shield_Strength(float str) {
	if (str > ShieldStrengthMax) {
		ShieldStrength = ShieldStrengthMax;
	}
	else {
		ShieldStrength = str;
	}
	Mark_Owner_Dirty();
}

void DefenseObjectClass::Add_Shield_Strength(float str) {
	ShieldStrength += str;
	if (ShieldStrength > ShieldStrengthMax) {
		ShieldStrength = ShieldStrengthMax;
	}
	Mark_Owner_Dirty();
}

float DefenseObjectClass::Get_Shield_Strength() const
{
	return this->ShieldStrength;
}

float DefenseObjectClass::Get_Shield_Strength_Max() const
{
	return this->ShieldStrengthMax;
}

void DefenseObjectClass::Set_Shield_Strength_Max(float str)
{
	ShieldStrengthMax = WWMath::Clamp(str, 0, MAX_MAX_SHIELD_STRENGTH);
	if (ShieldStrengthMax < ShieldStrength)
	{
		ShieldStrength = ShieldStrengthMax;
	}
	Mark_Owner_Dirty();
}

void DefenseObjectClass::Mark_Owner_Dirty()
{
	if (Get_Owner())
	{
		Get_Owner()->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_OCCASIONAL, true);
	}
}

SCRIPTS_API REF_DEF3(GameObjManager::BuildingGameObjList, SList<BuildingGameObj>, 0x00856FE8, 0x008561D0, 0x0085BED0);
SCRIPTS_API REF_DEF3(GameObjManager::GameObjList, SList<BaseGameObj>, 0x00856FF8, 0x008561E0, 0x0085BEE0);
SCRIPTS_API REF_DEF3(GameObjManager::StarGameObjList, SList<SoldierGameObj>, 0x00856FC8, 0x008561B0, 0x0085BEB0);
SCRIPTS_API REF_DEF3(GameObjManager::SmartGameObjList, SList<SmartGameObj>, 0x00856FD8, 0x008561C0, 0x0085BEC0);

#ifdef SHADERS_EXPORTS
SHADERS_API SList<C4GameObj> GameObjManager::C4GameObjList;
SHADERS_API SList<BeaconGameObj> GameObjManager::BeaconGameObjList;
SHADERS_API SList<SoldierGameObj> GameObjManager::SoldierGameObjList;
SHADERS_API SList<VehicleGameObj> GameObjManager::VehicleGameObjList;
SHADERS_API SList<ScriptZoneGameObj> GameObjManager::ScriptZoneGameObjList;
#endif

SCRIPTS_API const BaseGameObjDef &	BaseGameObj::Get_Definition( void ) const 
{
	return *Definition;
};

SCRIPTS_API const DamageableGameObjDef & DamageableGameObj::Get_Definition( void ) const 
{
	return (const DamageableGameObjDef &)BaseGameObj::Get_Definition();
}

SCRIPTS_API void ScriptableGameObj::Insert_Observer( GameObjObserverClass * observer )
{
	observer->Attach( this );
	Observers.Add( observer );
}

SCRIPTS_API void ScriptableGameObj::Remove_Observer( GameObjObserverClass * observer )
{
	Observers.Delete( observer );
	observer->Detach( this );
}

SCRIPTS_API void ScriptableGameObj::Remove_Observer(const char *Name)
{
	for (int i = Observers.Count()-1;i >= 0;i--) {
		if (!_stricmp(Observers[i]->Get_Name(),Name)) {
			Observers[i]->Detach(this);
			Observers.Delete(i,true);
		}
	}
}

SCRIPTS_API bool ScriptableGameObj::Has_Observer(const char *Name)
{
	for (int i = 0;i < Observers.Count();i++) {
		if (!_stricmp(Observers[i]->Get_Name(),Name)) {
			return true;
		}
	}
	return false;
}

SCRIPTS_API const ScriptableGameObjDef & ScriptableGameObj::Get_Definition( void ) const
{
	return (const ScriptableGameObjDef &)BaseGameObj::Get_Definition();
}

SCRIPTS_API const BuildingGameObjDef & BuildingGameObj::Get_Definition( void ) const
{
	return (const BuildingGameObjDef &)BaseGameObj::Get_Definition();
}

SCRIPTS_API const RefineryGameObjDef & RefineryGameObj::Get_Definition( void ) const
{
	return (const RefineryGameObjDef &)BaseGameObj::Get_Definition();
}

#ifndef TTLE_EXPORTS
#ifndef TT_EXPORTS
RENEGADE_FUNCTION
bool PowerUpGameObjDef::Grant(SmartGameObj *, PowerUpGameObj *, bool) const
AT2(0x006F09F0,0x006EFFB0);
#endif

RENEGADE_FUNCTION
Vector3 SCRIPTS_API Get_Color_For_Team(int teamId)
AT2(0x006D99E0,0x006D9280);
Vector3 DamageableGameObj::Get_Team_Color(void)
{
	return Get_Color_For_Team(PlayerType);
}

int SCRIPTS_API Get_Object_Type(GameObject *obj)
{
	if (!obj)
	{
		return 0;
	}
	DamageableGameObj *o2 = obj->As_DamageableGameObj();
	if (!o2)
	{
		return 0;
	}
	return o2->Get_Player_Type();
}

void SCRIPTS_API Set_Object_Type(GameObject *obj,int type)
{
	if (!obj)
	{
		return;
	}
	DamageableGameObj *o2 = obj->As_DamageableGameObj();
	if (!o2)
	{
		return;
	}
	o2->Set_Player_Type(type);
}

void SCRIPTS_API Get_Object_Color(GameObject *obj, unsigned int *red, unsigned int *green, unsigned int *blue)
{
	if (!obj)
	{
		return;
	}
	DamageableGameObj *o = obj->As_DamageableGameObj();
	if (!o)
	{
		return;
	}
	Vector3 v = o->Get_Team_Color();
	*red = (unsigned int)(v.X*255);
	*green = (unsigned int)(v.Y*255);
	*blue = (unsigned int)(v.Z*255);
}

GameObject SCRIPTS_API *Find_Smart_Object_By_Team(int Team)
{
	SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if (Get_Object_Type(o) == Team)
			{
				return o;
			}
		}
		x = x->Next();
	}
	return 0;
}

GameObject SCRIPTS_API *Find_Object_By_Team(int Team)
{
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if (Get_Object_Type(o2) == Team)
			{
				return o2;
			}
		}
		x = x->Next();
	}
	return 0;
}

GameObject SCRIPTS_API *Find_Non_Player_Object_By_Team(int Team)
{
	SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if ((Get_Object_Type(o) == Team) && (!Commands->Is_A_Star(o)))
			{
				return o;
			}
		}
		x = x->Next();
	}
	return 0;
}

GameObject SCRIPTS_API *Find_Object_By_Preset(int Team,const char *Preset_Name)
{
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			const DefinitionClass *d = &o2->Get_Definition();
			if (!_stricmp(d->Get_Name(),Preset_Name))
			{
				if ((Get_Object_Type(o2) == Team) || (Team == 2))
				{
					return o2;
				}
			}
		}
		x = x->Next();
	}
	return 0;
}

GameObject SCRIPTS_API *Find_Closest_Non_Building_Object_By_Team(int Team,Vector3 position)
{
	float closestdist = FLT_MAX;
	GameObject *closest = 0;
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ((Get_Object_Type(o2) == Team) && !o2->As_BuildingGameObj())
			{
				Vector3 pos = Commands->Get_Position(o2);
				float dist = Commands->Get_Distance(pos,position);
				if (dist < closestdist)
				{
					closestdist = dist;
					closest = o2;
				}
			}
		}
		x = x->Next();
	}
	return closest;
}

GameObject SCRIPTS_API *Find_Closest_Preset_By_Team(int Team,const Vector3 &pos,const char *Preset)
{
	float closestdist = 9999.0f;
	GameObject *closest = 0;
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ((Get_Object_Type(o2) == Team || Team == 2) && !_stricmp(Commands->Get_Preset_Name(o2),Preset))
			{
				float dist = Commands->Get_Distance(Commands->Get_Position(o2),pos);
				if (dist < closestdist)
				{
					closestdist = dist;
					closest = o2;
				}
			}
		}
		x = x->Next();
	}
	return closest;
}

GameObject SCRIPTS_API *Find_Random_Preset_By_Team(int Team,const char *Preset)
{
	SimpleDynVecClass<GameObject*> ObjList(0);
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ((Get_Object_Type(o2) == Team || Team == 2) && !_stricmp(Commands->Get_Preset_Name(o2),Preset))
			{
				ObjList.Add(o2);
			}
		}
		x = x->Next();
	}
	int len = ObjList.Count();
	if (len > 1)
	{
		return ObjList[Commands->Get_Random_Int(0,len)];
	}
	else if (!len)
	{
		return 0;
	}
	return ObjList[0];
}

int SCRIPTS_API Get_Object_Count(int Team,const char *Preset)
{
	int count = 0;
	SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if ((Get_Object_Type(o) == Team || Team == 2) && !_stricmp(Commands->Get_Preset_Name(o),Preset))
			{
				count++;
			}
		}
		x = x->Next();
	}
	return count;
}

SimpleDynVecClass<GameObject*> SCRIPTS_API *Get_All_Objects_By_Preset(int Team,const char *Preset)
{
	SimpleDynVecClass<GameObject*> *ObjList = new SimpleDynVecClass<GameObject*>;
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ((Get_Object_Type(o2) == Team || Team == 2) && !_stricmp(Commands->Get_Preset_Name(o2),Preset))
			{
				ObjList->Add(o2);
			}
		}
		x = x->Next();
	}
	return ObjList;
}

GameObject SCRIPTS_API *Find_Nearest_Preset(Vector3 position, const char *preset)
{
	GameObject *object = 0;
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if (!_stricmp(Commands->Get_Preset_Name(o2),preset))
			{
				if (object)
				{
					Vector3 obj_pos = Commands->Get_Position(object);
					Vector3 o_pos = Commands->Get_Position(o2);
					if (Commands->Get_Distance(position, o_pos) < Commands->Get_Distance(position, obj_pos))
					{
						object = o2;
					}
				}
				else
				{
					object = o2;
				}
			}
		}
		x = x->Next();
	}
	return object;
}

GameObject SCRIPTS_API *Find_Random_Preset(const char* preset, int min, int max)
{
	int count = Commands->Get_Random_Int(min, max);
	GameObject *object = 0;
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (count > 0)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if (!_stricmp(Commands->Get_Preset_Name(o2),preset))
			{
				object = o2;
				count--;
			}
		}
		x = x->Next();
		if (!x)
		{
			if (object)
			{
				x = GameObjManager::GameObjList.Head();
			}
			else
			{
				return object;
			}
		}
	}
	return object;
}

void SCRIPTS_API Send_Custom_To_Team_Buildings(int Team,GameObject *sender,int type,int param,float delay)
{
	if (!sender)
	{
		return;
	}
	SLNode<BuildingGameObj> *x = GameObjManager::BuildingGameObjList.Head();
	while (x)
	{
		GameObject *o = x->Data();
		if (o)
		{
			if (Get_Object_Type(o) == Team)
			{
				int ReceiverID = Commands->Get_ID(o);
				int SenderID = Commands->Get_ID(sender);
				if (SenderID != ReceiverID)
				{
					Commands->Send_Custom_Event(sender,o,type,param,delay);
				}
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Send_Custom_To_Team_Preset(int Team,const char *PresetName,GameObject *sender,int type,int param,float delay)
{
	if (!sender)
	{
		return;
	}
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if (Get_Object_Type(o2) == Team)
			{
				if (!_stricmp(Commands->Get_Preset_Name(o2),PresetName))
				{
					Commands->Send_Custom_Event(sender,o2,type,param,delay);
				}
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Send_Custom_All_Objects_Area(int type,const Vector3 &Position,float Distance,GameObject *sender,int team)
{
	if (!sender)
	{
		return;
	}
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x) 
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			Vector3 ObjPosition = Commands->Get_Position(o2);
			Vector3 TestPosition = Position;
			ObjPosition.Z = 0;
			TestPosition.Z = 0;
			if ((Commands->Get_Distance(ObjPosition,TestPosition) <= Distance))
			{
				if ((Get_Object_Type(o2) == team) || (team == 2))
				{
					Commands->Send_Custom_Event(sender,o2,type,0,0);
				}
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Send_Custom_All_Objects(int type,GameObject *sender,int team)
{
	if (!sender)
	{
		return;
	}
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x) 
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ((Get_Object_Type(o2) == team) || (team == 2))
			{
				Commands->Send_Custom_Event(sender,o2,type,0,0);
			}
		}
		x = x->Next();
	}
}

void SCRIPTS_API Send_Custom_Event_To_Object(GameObject *sender,const char *Preset,int type,int param,float delay)
{
	if (!sender)
	{
		return;
	}
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			const DefinitionClass *d = &o2->Get_Definition();
			if (!_stricmp(d->Get_Name(),Preset))
			{
				Commands->Send_Custom_Event(sender,o2,type,param,delay);
			}
		}
		x = x->Next();
	}
}

bool SCRIPTS_API Is_Unit_In_Range(const char *preset,float range,Vector3 location,int team,bool empty)
{
	SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if (Get_Object_Type(o2) == team && !_stricmp(Commands->Get_Preset_Name(o2),preset))
			{
				Vector3 pos = Commands->Get_Position(o2);
				pos.Z = 0;
				location.Z = 0;
				if (Commands->Get_Distance(pos,location) <= range)
				{
					if (o2->As_VehicleGameObj() && !empty)
					{
						if (o2->As_VehicleGameObj()->Get_Occupant_Count() == 0)
						{
							return false;
						}
						else
						{
							return true;
						}
					}
					else
					{
						return true;
					}
				}
			}
		}
		x = x->Next();
	}
	return false;
}

bool SCRIPTS_API Get_Is_Powerup_Persistant(GameObject *obj)
{
	if (obj->As_PhysicalGameObj())
	{
		PowerUpGameObj *o = obj->As_PhysicalGameObj()->As_PowerUpGameObj();
		if (!o)
		{
			return 0;
		}
		return o->Get_Definition().Is_Persistant();
	}
	return 0;
}

bool SCRIPTS_API Get_Powerup_Always_Allow_Grant(GameObject *obj)
{
	if (obj->As_PhysicalGameObj())
	{
		PowerUpGameObj *o = obj->As_PhysicalGameObj()->As_PowerUpGameObj();
		if (!o)
		{
			return 0;
		}
		return o->Get_Definition().Is_Always_Allow_Grant();
	}
	return 0;
}

int SCRIPTS_API Get_Powerup_Grant_Sound(GameObject *obj)
{
	if (obj->As_PhysicalGameObj())
	{
		PowerUpGameObj *o = obj->As_PhysicalGameObj()->As_PowerUpGameObj();
		if (!o)
		{
			return 0;
		}
		return o->Get_Definition().Get_Grant_Sound();
	}
	return 0;
}

void SCRIPTS_API Grant_Powerup(GameObject *obj,const char *Preset_Name)
{
	if (!obj)
	{
		return;
	}
	SmartGameObj *o = obj->As_SmartGameObj();
	if (o)
	{
		PowerUpGameObjDef *def = (PowerUpGameObjDef *)Find_Named_Definition(Preset_Name);
		def->Grant(o,0,true);
	}
}


Vector3 SCRIPTS_API Get_Velocity( GameObject* obj )
{
	Vector3 velocity ( 0.0f, 0.0f, 0.0f );

	if ( obj && obj->As_PhysicalGameObj() )
	{
		MoveablePhysClass* moveable = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
		if (moveable)
			moveable->Get_Velocity(&velocity);
	}

	return velocity;
}


void SCRIPTS_API Set_Velocity( GameObject* obj, Vector3 velocity )
{
	if ( obj && obj->As_PhysicalGameObj() )
	{
		MoveablePhysClass* moveable = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
		if (moveable)
			moveable->Set_Velocity(velocity);
	}
}


Matrix3D SCRIPTS_API Get_Transform( GameObject* obj )
{
	Matrix3D transform(true);

	if ( obj && obj->As_PhysicalGameObj() )
	{
		MoveablePhysClass* moveable = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
		if (moveable)
			transform = moveable->Get_Transform();
	}

	return transform;
}


void SCRIPTS_API Set_Transform( GameObject* obj, Matrix3D transform )
{
	if ( obj && obj->As_PhysicalGameObj() )
	{
		MoveablePhysClass* moveable = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
		if (moveable)
			moveable->Set_Transform(transform);
	}
}


GameObject SCRIPTS_API *Get_Closest_Armed_Object_To_Object( GameObject* obj, int team )
{
	float closestdist = FLT_MAX;
	GameObject *closest = 0;
	Vector3 position = Commands->Get_Position(obj);
	SLNode<BaseGameObj> *x = GameObjManager::GameObjList.Head();
	while (x)
	{
		BaseGameObj *o = x->Data();
		ScriptableGameObj *o2 = 0;
		if (o && o != obj)
		{
			o2 = o->As_ScriptableGameObj();
		}
		if (o2)
		{
			if ((team == 2 || Get_Object_Type(o2) == team) && o2->As_PhysicalGameObj() && o2->As_PhysicalGameObj()->As_ArmedGameObj())
			{
				Vector3 pos = Commands->Get_Position(o2);
				float dist = Commands->Get_Distance(pos,position);
				if (dist < closestdist)
				{
					closestdist = dist;
					closest = o2;
				}
			}
		}
		x = x->Next();
	}
	return closest;
}

SCRIPTS_API bool DamageableGameObj::Is_Teammate(DamageableGameObj * p_obj)
{
	return ((p_obj == this) || (Is_Team_Player() && Get_Player_Type() == p_obj->Get_Player_Type()));
}

int SCRIPTS_API VehicleGameObj::Get_Occupant_Count(void)
{
	int count = 0;
	for ( int i = 0; i < SeatOccupants.Length(); i++ ) {
		if ( SeatOccupants[i] != NULL ) {
			count++;
		}
	}
	return count;
}

VehicleGameObj* SoldierGameObj::Get_Profile_Vehicle( void )
{
	if (( Get_State() == HumanStateClass::IN_VEHICLE ) && Vehicle )
	{
		return Vehicle;
	}
	if (( Get_State() == HumanStateClass::TRANSITION ) && ( TransitionCompletionData != NULL ))
	{
		if ( TransitionCompletionData->Type == TransitionDataClass::VEHICLE_ENTER )
		{
			return (VehicleGameObj *)TransitionCompletionData->Vehicle.Get_Ptr();
		}
	}
	return NULL;
}

SCRIPTS_API bool DamageableGameObj::Is_Team_Player(void) 
{
	int player_type = Get_Player_Type();
	return player_type == PLAYERTYPE_NOD || player_type == PLAYERTYPE_GDI;
}

SCRIPTS_API bool DamageableGameObj::Is_Enemy(DamageableGameObj *obj)
{
	bool enemy = false;
	if (obj != this)
	{
		int sTeamID1 = this->Get_Player_Type();
		int sTeamID2 = obj->Get_Player_Type();
		if ( sTeamID1 != -2 && sTeamID2 != -2 && sTeamID1 != -4 && sTeamID2 != -4 )
		{
			if ( sTeamID1 != -1 && sTeamID2 != -1 )
			{
				enemy = sTeamID1 != sTeamID2;
			}
			else
			{
				enemy = true;
			}
		}
		else
		{
			enemy = false;
		}
	}
	return enemy;
}

SCRIPTS_API AnimControlClass *PhysicalGameObj::Get_Anim_Control()
{
	return AnimControl;
}

SCRIPTS_API  const PowerUpGameObjDef & PowerUpGameObj::Get_Definition( void ) const
{
	return (const PowerUpGameObjDef &)BaseGameObj::Get_Definition();
}

SCRIPTS_API  const PhysicalGameObjDef & PhysicalGameObj::Get_Definition( void ) const
{
	return (const PhysicalGameObjDef &)BaseGameObj::Get_Definition();
}

SCRIPTS_API bool Verify_Preset_Limit(int team, int instance_count, uint32* preset_ids, int preset_id_count, GameObject* ignore)
{
	for (SLNode<SmartGameObj>* node = GameObjManager::SmartGameObjList.Head(); node; node = node->Next())
	{
		SmartGameObj* object = node->Data();
		if (object != ignore && (object->Get_Player_Type() == team || team == 2))
		{
			for (int i = 0; i < preset_id_count; ++i)
			{
				if ((uint32)object->Get_Definition().Get_ID() == preset_ids[i] && --instance_count == 0) return true;
			}
		}
	}
	return false;
}

void SCRIPTS_API Set_Powerup_Always_Allow_Grant(GameObject *obj,bool Grant)
{
	if (!obj)
	{
		return;
	}
	PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
	if (!o2)
	{
		return;
	}
	PowerUpGameObj *o3 = o2->As_PowerUpGameObj();
	if (!o3)
	{
		return;
	}
	((PowerUpGameObjDef&)o3->Get_Definition()).AlwaysAllowGrant = Grant;
}

void SCRIPTS_API PhysicalGameObj::Set_Collision_Group(int group)
{
	PhysObj->Set_Collision_Group((Collision_Group_Type)group);
	Set_Object_Dirty_Bit(BIT_RARE,true);
}
#endif
