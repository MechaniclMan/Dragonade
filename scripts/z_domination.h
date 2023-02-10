/*	Renegade Scripts.dll
	Copyright 2013 UltraAOW.COM by zunnie

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#pragma once

class AOW_Domination_Control : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj, int number);
	bool triggerendmsg;
	bool spam;
	int points_gdi;
	int points_nod;
	int points_max;
};

class AOW_Domination_Pointgen : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int team;
};

class AOW_Domination_Vehicle : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
}; 

class AOW_Nodewar_Node : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int owner;
	bool trigger_neutral_damage;
	int nodveh1id; int nodveh2id;
	int gdiveh1id; int gdiveh2id;
	int spawnpad1;	int spawnpad2;
	const char *nodename;
}; 

class AOW_Prevent_Destruction : public ScriptImpClass {
	void Damaged(GameObject *obj, GameObject *damager, float damage);
}; 