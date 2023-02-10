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


class z_Lockdown_Bar : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj, GameObject *shooter);
	int count;
};
class z_Lockdown_Ref : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};
class z_Death_Disable10Spawners : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};
class z_LockdownMissionStart : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};
class z_Lockdown_Endgame : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};
class z_Lockdown_WF : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	int count;
};
