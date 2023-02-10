/*	Renegade Scripts.dll
    Dragonade Misc. Game Object Observers
	Copyright 2012 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAGAMEOBJOBSERVERS
#define INCLUDE_DAGAMEOBJOBSERVERS

#include "da_gameobj.h"

class DATimedInvincibilityObserverClass : public DAGameObjObserverClass {
public:
	DATimedInvincibilityObserverClass(GameObject *obj,float ProtectTime);
	virtual const char *Get_Name() { return "DATimedInvincibilityObserverClass"; }
	virtual bool Damage_Received_Request(OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone);
	virtual void Timer_Expired(GameObject *obj,int Number);
};

#endif