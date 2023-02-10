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
#pragma once
#include "General.h"
#include "scripts.h"
#include "engine.h"
#include "z_fever.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"


class z_Escort_CheckPT : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Poked(GameObject *obj, GameObject *poker);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	GameObject *PokerGuy;
	bool AllowPoke;
	int CheckID;
	int Check1Locked;
	int Check2Locked;
	int Check3Locked;
};
class z_Escort_MCV_CheckPT : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};
class z_Escort_MCV_Finish : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Entered(GameObject *obj, GameObject *enter);
	bool trigger;
	GameObject *mcv;
};
class z_Escort_MCV : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};
class z_Escort_Start : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};
