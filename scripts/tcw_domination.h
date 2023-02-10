/*	Renegade Scripts.dll
	Scripts from - http://www.tiberiumcrystalwar.com
	Copyright 2011-2012 MPF

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it 
	under the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code 
	with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#pragma once

class tcw_domination_control : public ScriptImpClass {
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

class tcw_domination_pointgen : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int team;
}; 

class tcw_domination_node : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int owner;
	bool trigger_neutral_damage;
	int nodveh1id; int nodveh2id; int nodveh3id; int nodveh4id; int nodveh5id;
	int gdiveh1id; int gdiveh2id; int gdiveh3id; int gdiveh4id; int gdiveh5id; 
	int spawnpad1;	int spawnpad2;	int spawnpad3;	int spawnpad4;
	const char *nodename;
}; 

class tcw_domination_vehicle : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
}; 

class tcw_wmd_countdown_node : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool allowcap;
	int owner;
	int nodveh1id; int nodveh2id; int nodveh3id; int nodveh4id; int nodveh5id;
	int gdiveh1id; int gdiveh2id; int gdiveh3id; int gdiveh4id; int gdiveh5id; 
	const char *nodename;
}; 

class tcw_wmd_strike : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
}; 


