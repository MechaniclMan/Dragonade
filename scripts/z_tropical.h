/*	Renegade Scripts.dll
	Copyright 2012 UltraAOW.COM

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#pragma once

class z_Trop_Vehicle : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj, GameObject *shooter);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};

class z_Trop_Carrier_Control_Point : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int owner;
	bool trigger_neutral_damage;
	GameObject *nodveh1; GameObject *nodveh2; GameObject *nodveh3; GameObject *nodveh4;
	GameObject *gdiveh1; GameObject *gdiveh2; GameObject *gdiveh3; GameObject *gdiveh4; 
	Vector3 Spawn_Pos1; Vector3 Spawn_Pos2; Vector3 Spawn_Pos3; Vector3 Spawn_Pos4;
	float Spawn_Facing;
	int nodveh1id; int nodveh2id; int nodveh3id; int nodveh4id;
	int gdiveh1id; int gdiveh2id; int gdiveh3id; int gdiveh4id;
	int carrierid;
	const char *nodename;
	const char *gdi_orca;
	const char *nod_apache;
}; 

class z_Trop_Locked_Vehicle : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
}; 

class z_Trop_Aircraft_Carrier : public ScriptImpClass {
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Killed(GameObject *obj, GameObject *shooter);
}; 

class z_Trop_Helipad_Zone : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj, GameObject *enter);
	void Exited(GameObject *obj, GameObject *exit);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool busy;
	int vehid;
	int team;
};