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

/*********************************************************************************************/
class z_aM01_Einstein : public ScriptImpClass {
	unsigned int FollowID;
	GameObject *Pokerguy;
	bool Following;
	bool AllowPoke;
	float HealthRegen;
	int HostageID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Poked(GameObject *obj,GameObject *poker);
	void Killed(GameObject *obj,GameObject *shooter);
	void Timer_Expired(GameObject *obj,int number);
};
/*********************************************************************************************/
class z_aM01_Einstein_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};
/*********************************************************************************************/
class z_aM01_MissionStart : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};
class z_aM01_SovBarracks : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Damaged(GameObject *obj,	GameObject *damager, float damage);
	int count;
};