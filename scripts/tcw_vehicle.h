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

class tcw_vehicle : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	void Killed(GameObject *obj, GameObject *shooter);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Destroyed(GameObject *obj);
	bool triggerstealthdetection;
	bool revealed;
	bool playdetected;
	bool playdamage;
	bool is_railgun_available;
	int venomfxid;
	int gharv_idle_fxid;
	int gharv_harving_fxid;
	int gharv_full_fxid;
};

class tcw_vehexit_killtimer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int timerid;
	int driverid;
};

class tcw_hijacked_nod : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
}; 
class tcw_hijacked_gdi: public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
}; 

class tcw_bunker : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	int ZoneID;
};

class tcw_bunker_zone : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	int ExpLoc1;
	int ExpLoc2;
	int ExpLoc3;
};

class tcw_apc_minefield_lay : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	bool Minefield_Available;
};

class tcw_apc_minefield : public ScriptImpClass
{
	void Entered(GameObject *obj, GameObject *enter);
};