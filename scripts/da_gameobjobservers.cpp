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

#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "engine_DA.h"
#include "da.h"
#include "da_gameobjobservers.h"


DATimedInvincibilityObserverClass::DATimedInvincibilityObserverClass(GameObject *obj,float ProtectTime) {
	obj->Add_Observer(this);
	Start_Timer(1,ProtectTime);
}

bool DATimedInvincibilityObserverClass::Damage_Received_Request(OffenseObjectClass *Offense,DADamageType::Type Type,const char *Bone) {
	if (Offense->Get_Damage() > 0.0f) {
		return false;
	}
	return true;
}

void DATimedInvincibilityObserverClass::Timer_Expired(GameObject *obj,int Number) {
	Set_Delete_Pending();
}

