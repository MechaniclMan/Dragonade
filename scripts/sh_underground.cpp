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
#include "general.h"

#include "scripts.h"
#include "engine.h"
#include "ScriptableGameObj.h"
#include "PhysicalGameObj.h"
#include "MoveablePhysClass.h"
#include "VehicleGameObj.h"

// customs that are used by the underground script
enum
{
	SH_UNDERGROUND_LOCKSTATE = 420001, // set topmost lock state to param
	SH_UNDERGROUND_PUSHLOCKSTATE, // push param on new topmost lock state
	SH_UNDERGROUND_POPLOCKSTATE, // pop to old lock state
	SH_UNDERGROUND_SETSTATE, // attempt to set underground state to param
	SH_UNDERGROUND_TOGGLESTATE, // attempt to toggle underground state
	SH_UNDERGROUND_DETACH,
};

// UndergroundDigZone - Objects inside a zone with this script attached can enter and exit the underground state
class SH_UndergroundDigZone: public ScriptImpClass
{
public:

	void Entered(GameObject *zone, GameObject *object)
	{
		// push an "unlocked" state on the underground state stack
		Commands->Send_Custom_Event(zone, object, SH_UNDERGROUND_PUSHLOCKSTATE, 0, 0.0f);
	};

	void Exited(GameObject *zone, GameObject *object)
	{
		// pop what we pushed on the state stack
		Commands->Send_Custom_Event(zone, object, SH_UNDERGROUND_POPLOCKSTATE, 0, 0.0f);
	};
};

// UndergroundNoDigZone - Objects inside a zone with this script attached cannot enter and exit the underground state
class SH_UndergroundNoDigZone: public ScriptImpClass
{
public:

	void Entered(GameObject *zone, GameObject *object)
	{
		// push a "locked" state on the underground state stack
		Commands->Send_Custom_Event(zone, object, SH_UNDERGROUND_PUSHLOCKSTATE, 1, 0.0f);
	};

	void Exited(GameObject *zone, GameObject *object)
	{
		// pop what we pushed on the state stack
		Commands->Send_Custom_Event(zone, object, SH_UNDERGROUND_POPLOCKSTATE, 0, 0.0f);
	};
};

// UndergroundVehicle - Vehicles with this script attached can enter and exit the underground state
class SH_UndergroundVehicle : public ScriptImpClass 
{
protected:

	Collision_Group_Type	original_group;		// needed to undo underground state
	ArmorType				original_armor;		// ^
	bool					underground;		// "am I currently underground?"
	bool					could_fire;			// "could I fire before I went underground?"
	bool					dig_pending;		// "is there a dig animation pending completion?"
	unsigned int			lock_state;			// stack of "can I set underground?"
	int						lock_state_index;	// index of topmost ^that

	bool IsStateLocked()
	{
		// is the top-most state locked
		return (lock_state & (1 << lock_state_index)) != 0;
	};

	void SetStateLocked(bool onoff)
	{
		// set the top-most state
		if (onoff) lock_state |= (1 << lock_state_index);
		else lock_state &= ~(1 << lock_state_index);
	};

	void PushStateLocked(bool onoff)
	{
		// push a new state on
		++lock_state_index;

		// and set it to onoff
		SetStateLocked(onoff);
	};

	void PopStateLocked()
	{
		// pop off the topmost state
		--lock_state_index;

		// paranoia check
		assert("LockState popped too many times" && lock_state_index > 0);
	};

	void EnterUndergroundMode(bool force = false)
	{
		if (underground || (IsStateLocked() && !force)) return; // already entered or state locked and not forced

		// stash away the owner
		GameObject* owner = Owner();

		// pull out the physical object
		PhysicalGameObj* physical_object = owner->As_PhysicalGameObj();

		// only physical objects can play
		if (!physical_object) return;

		// pull out the movable phys
		MoveablePhysClass* movable_phys = physical_object->Peek_Physical_Object()->As_MoveablePhysClass();

		// only movable objects can play
		if (!movable_phys) return;

		if (movable_phys->Is_Immovable()) return;

		// no weapon firing is allowed
		{
			VehicleGameObj* vehicle = owner->As_VehicleGameObj();
			if (vehicle)
			{
				could_fire = vehicle->Get_Scripts_Can_Fire();
				vehicle->Set_Scripts_Can_Fire(false);
			};
		};

		// nobody gets in or out...
		Commands->Enable_Vehicle_Transitions(owner, false);

		// defuse attached C4
		Defuse_Attached_C4(owner);

		// start digging
		const char* animation = Get_Parameter("DigAnimation");
		
		if (*animation)
		{
			dig_pending = true;
			Commands->Set_Animation(owner, animation, false, 0, 0, -1, false);
		}
		else
		{
			CompleteDig(owner);
		}
	};

	void CompleteDig(GameObject* obj)
	{
		PhysicalGameObj* physical_object = obj->As_PhysicalGameObj();
		MoveablePhysClass* movable_phys = physical_object->Peek_Physical_Object()->As_MoveablePhysClass();

		// we aren't visible to AI
		Set_Vehicle_Is_Visible(obj, false);

		// become invulnerable
		original_armor = physical_object->Get_Defense_Object()->Get_Shield_Type();
		Commands->Set_Shield_Type(obj, "Blamo");

		// backup the original group for when we undig
		original_group = movable_phys->Get_Collision_Group();

		// switch to the underground collision group
		movable_phys->Set_Collision_Group(UNDERGROUND_COLLISION_GROUP);

		// inform netcode of the dirty things we've done with the object
		physical_object->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE, true);

		// you are now underground, congrats
		underground = true;
	};

	void ExitUndergroundMode(bool force = false)
	{
		if (!underground || (IsStateLocked() && !force)) return; // already left or state locked and not forced

		// stash away the owner
		GameObject* owner = Owner();

		// pull out the physical object
		PhysicalGameObj* physical_object = owner->As_PhysicalGameObj();

		// only physical objects can play
		if (!physical_object) return;

		// pull out the movable phys
		MoveablePhysClass* movable_phys = physical_object->Peek_Physical_Object()->As_MoveablePhysClass();

		// only movable objects can play
		if (!movable_phys) return;

		// restore the original collision group
		movable_phys->Set_Collision_Group(original_group);

		// check to see if we can surface
		const Matrix3D& new_tm = movable_phys->Get_Transform();
		if (!movable_phys->Can_Teleport(new_tm, true) && !force)
		{
			// we can't, so back to underground we go
			movable_phys->Set_Collision_Group(UNDERGROUND_COLLISION_GROUP);
			return;
		};

		// weapon firing is now allowed
		{
			VehicleGameObj* vehicle = owner->As_VehicleGameObj();
			if (vehicle) vehicle->Set_Scripts_Can_Fire(could_fire);
		};

		// you are now free the move about the cabin
		Commands->Enable_Vehicle_Transitions(owner, true);

		// but the AI can see everything you do
		Set_Vehicle_Is_Visible(owner, true);
		Commands->Set_Animation(owner,Get_Parameter("SurfaceAnimation"),false,0,0,-1,false);

		// and are no longer invulnerable
		physical_object->Get_Defense_Object()->Set_Shield_Type(original_armor);

		// inform netcode of the dirty things we've done with the physical object
		physical_object->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE, true);

		// no longer mark us as underground
		underground = false;
	};

public:

	void Attach(GameObject* obj)
	{
		// base implementation
		ScriptImpClass::Attach(obj);

		// make sure we are in the default state for our attached object
		underground = false;
		lock_state = 0;
		lock_state_index = 0;

		// hook up any customizations
		SetStateLocked(Get_Int_Parameter("DefaultLockState") != 0);
	};

	void Detach(GameObject* obj)
	{
		if (Exe != 4) //make sure we dont try things in LE that may cause issues
		{
			// force exit of underground (don't want any objects stuck underground)
			ExitUndergroundMode(true);
		}

		// base implementation
		ScriptImpClass::Detach(obj);
	};

	void Custom(GameObject *obj, int type, int param, GameObject *sender)
	{
		switch(type)
		{
		case SH_UNDERGROUND_LOCKSTATE:
			// set the top-most lock state
			SetStateLocked(param != 0);
			break;
		case SH_UNDERGROUND_PUSHLOCKSTATE:
			// push a new top-most lock state
			PushStateLocked(param != 0);
			break;
		case SH_UNDERGROUND_POPLOCKSTATE:
			// pop to the old top-most lock state
			PopStateLocked();
			break;
		case SH_UNDERGROUND_SETSTATE:
			// attempt to set the current underground state
			if (param) EnterUndergroundMode();
			else ExitUndergroundMode();
			break;
		case SH_UNDERGROUND_TOGGLESTATE:
			// toggle states
			if (!underground) EnterUndergroundMode();
			else ExitUndergroundMode();
			break;
		case SH_UNDERGROUND_DETACH:
			Destroy_Script();
			break;
		};
	};

	void Animation_Complete(GameObject *obj, const char *animation_name)
	{
		VehicleGameObj* vehicle = obj ? obj->As_VehicleGameObj() : NULL;
		if (vehicle && dig_pending && !_stricmp(animation_name,Get_Parameter("DigAnimation")))
		{
			dig_pending = false;

			CompleteDig(obj);
		}
	};
};

class SH_UndergroundKey: public JFW_Key_Hook_Base
{
public:
	void Created(GameObject *obj)
	{
		InstallHook(Get_Parameter("Key"),obj);
	}
	void KeyHook()
	{
		GameObject *owner = Owner();
		if (Get_Vehicle(owner) && Get_Vehicle_Driver(Get_Vehicle(owner)) == owner)
		{
			Commands->Send_Custom_Event(owner,Get_Vehicle(owner),SH_UNDERGROUND_TOGGLESTATE,0,0);
		}
	}
};

#define REGISTER_SCRIPT(name, params) ScriptRegistrant<name> name##Registrant(#name, params);

REGISTER_SCRIPT(SH_UndergroundDigZone, "");
REGISTER_SCRIPT(SH_UndergroundNoDigZone, "");
REGISTER_SCRIPT(SH_UndergroundVehicle, "DefaultLockState=0:int,DigAnimation:string,SurfaceAnimation:string");
REGISTER_SCRIPT(SH_UndergroundKey, "Key=Key:string");
