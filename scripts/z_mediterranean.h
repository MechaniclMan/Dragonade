/*	Renegade Scripts.dll
	Copyright 2013 UltraAOW.COM

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#pragma once

class z_Med_Vehicle : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Killed(GameObject *obj, GameObject *shooter);
};

class z_Med_Credit_Trickle : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class z_Med_Disable_Transitions_Zone : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
	void Exited(GameObject *obj, GameObject *exit);
};

class z_Med_TibSilo : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj, int number);
	int owner;
	bool playdmg;
	bool playkilled;
};

class z_Med_Helipad : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int team;
	bool playdmg;
	bool playkilled;
	bool disablerepair;
};

class z_Med_Helipad_Zone : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj, GameObject *enter);
	void Exited(GameObject *obj, GameObject *exit);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool busy;
	int vehid;
	int team;
};

class z_Med_Helipad_Count_Nod : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
	int deaths;
};

class z_Med_Helipad_Count_GDI : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
	int deaths;
};

class z_Med_Repairpad : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj, int number);
	int owner;
	int zoneid;
	bool playdmg;
	bool playkilled;
};

class z_Med_Repairzone : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj, GameObject *enter);
	void Exited(GameObject *obj, GameObject *exit);
	void Timer_Expired(GameObject *obj, int number);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
	int playerobjid;
	int padid;
	int owner;
	int vehid;
	bool busy;
};

class z_Med_Building : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	public: void Register_Auto_Save_Variables();
	bool trigger75;
	bool trigger50;
	bool trigger25;
};

class z_Med_Teleporter : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Entered(GameObject *obj, GameObject *enter);
	public: void Register_Auto_Save_Variables();
	bool tele;
};

class z_Med_Buy_Heli : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Poked(GameObject *obj, GameObject *poker);
	public: void Register_Auto_Save_Variables();
	bool allowpoke;
	float cost;
	const char *type;
	int team;
	int hpadid;
	int buyerid;
};

class z_Med_Supply_Truck : public ScriptImpClass
{
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class z_Med_Tiberium_Harvester_Player : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool isharvesting;
	bool hastiberium;
	int driverid;
	int team;
	int plid;
};

class z_Med_Tiberium_Field : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
	void Exited(GameObject *obj, GameObject *exit);
};

class z_Med_Tiberium_Deposit : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};