/*	Renegade Scripts.dll
	Scripts by from - http://www.multiplayerforums.com
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

class z_air_Tiberium_Silo : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool givemoney;
	bool playdmg;
	bool playdead;
	bool disablerepair;
}; 

class z_air_Repairpad : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj, int number);
	int owner;
	int zoneid;
	bool playdmg;
	bool playkilled;
	bool disablerepair;
};

class z_air_Repairzone : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj, GameObject *enter);
	void Exited(GameObject *obj, GameObject *exit);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int playerobjid;
	int padid;
	int owner;
	int vehid;
	bool busy;
};

class z_air_Carrier_Control_Point : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	public: void Register_Auto_Save_Variables();
	int owner;
	bool trigger_neutral_damage;
	const char *nodename;
}; 

class z_air_Helipad_Zone : public ScriptImpClass {
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

class z_air_Helipad : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int team;
	bool playdmg;
	bool playkilled;
	bool disablerepair;
};

class z_air_Buy_Heli : public ScriptImpClass
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

class z_air_ApacheOrca : public ScriptImpClass
{
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};

class z_air_SniperLimit : public ScriptImpClass
{
	void Created(GameObject *obj);
};