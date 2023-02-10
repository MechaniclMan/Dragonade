/*	Renegade Scripts.dll
    Dragonade Disable List
	Copyright 2013 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DADISABLE
#define INCLUDE_DADISABLE

#include "da_event.h"
#include "da_gamefeature.h"
#include "Definition.h"
#include "HashTemplateClass.h"

class DA_API DADisableListManager : private DAEventClass {
public:
	static void Init();
	static bool Is_Preset_Disabled(GameObject *obj);
	static bool Is_Preset_Disabled(const char *Preset);
	static bool Is_Preset_Disabled(const DefinitionClass *Def);
	static bool Is_Preset_Disabled(unsigned int DefID);
	static bool Is_Preset_Disabled_For_Character(GameObject *obj,const DefinitionClass *Def);
	static bool Is_Preset_Disabled_For_Character(GameObject *obj,const char *Preset);
	static bool Is_Preset_Disabled_For_Character(GameObject *obj,unsigned int DefID);
	
private:
	virtual void Level_Loaded_Event();
	virtual void Object_Created_Event(GameObject *obj);
	virtual int Character_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const SoldierGameObjDef *Item);
	virtual int Vehicle_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const VehicleGameObjDef *Item);
	virtual int PowerUp_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const PowerUpGameObjDef *Item);
	virtual bool PowerUp_Grant_Request_Event(SoldierGameObj *Grantee,const PowerUpGameObjDef *PowerUp,PowerUpGameObj *PowerUpObj);
	virtual bool Add_Weapon_Request_Event(cPlayer *Player,const WeaponDefinitionClass *Weapon);
	virtual bool Refill_Event(cPlayer *Player);
	
	static DynamicVectorClass<unsigned int> DisableList;
	static HashTemplateClass<unsigned int,DynamicVectorClass<unsigned int>> CharacterDisableList; //SoldierGameObjDef*,Definition ID
};

#endif