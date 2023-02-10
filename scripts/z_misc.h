/*	Renegade Scripts.dll
	Copyright 2012 UltraAOW.COM by zunnie

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#pragma once

class z_Building_Health_Notifications : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	public: void Register_Auto_Save_Variables();
	bool trigger75;
	bool trigger50;
	bool trigger25;
};

class z_Locked_Vehicle : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
}; 

class z_Simple_Tiberium_Silo : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool givemoney;
	bool playdmg;
	bool playdead;
}; 

class z_This_Cannot_Be_Repaired : public ScriptImpClass {
	void Damaged(GameObject *obj, GameObject *damager, float damage);
}; 

class z_Endgame_Custom_Count : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
	int nod_dead_msg;
	int gdi_dead_msg;
	int nod_alive_msg;
	int gdi_alive_msg;
	int nod_dead_count;
	int gdi_dead_count;
	int nod_max_deaths;
	int gdi_max_deaths;
}; 

class z_Endgame_Custom_Killed : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
}; 

class z_Endgame_Custom_Created : public ScriptImpClass {
	void Created(GameObject *obj);
}; 

class z_Enter_Teleport_Random_Team : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class z_Vehicle_Death_Notification : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class z_Credit_Trickle : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class z_Capturable_TibSilo : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj, int number);
	int owner;
	bool playdmg;
	bool playkilled;
};

class z_Capturable_Repairpad : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj, int number);
	int owner;
	int zoneid;
	bool playdmg;
	bool playkilled;
};

class z_Capturable_Repairzone : public ScriptImpClass {
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

class z_Set_Vehicle_Limit : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class z_Set_Time_Limit_Created : public ScriptImpClass {
	void Created(GameObject *obj);
};

class z_Set_Mine_Limit_Created : public ScriptImpClass {
	void Created(GameObject *obj);
};

class z_Min_Veh_Z_Pos : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

