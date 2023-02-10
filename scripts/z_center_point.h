/*	Renegade Scripts.dll
	Scripts by from - http://www.tiberiumcrystalwar.com
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

class z_cp_cruiser_countdown_point : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool create_cruiser_countdown_object;
	int gdiveh1id; int nodveh1id;
	int nod_bar_turret_id; 	int nod_ref_turret_id; 	int gdi_hand_turret_id; int gdi_ref_turret_id;
	int spawnpad1;	int spawnpad2;
}; 

class z_cp_cruiser_countdown : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int team;
	int turretbarnod;
	int turretrefnod;
	int turrethongdi;
	int turretrefgdi;
}; 

class z_cp_cruiser_attack : public ScriptImpClass {
	void Created(GameObject *obj);
	void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
	void Timer_Expired(GameObject *obj, int number);
};

class z_cp_Harvester : public ScriptImpClass {
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

class z_cp_Tiberium_Field : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
	void Exited(GameObject *obj, GameObject *exit);
};

class z_cp_Tiberium_Deposit : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class z_cp_Building : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
	bool allow_player_damage;
};

class z_cp_Limit_1000_Chars : public ScriptImpClass {
	void Created(GameObject *obj);
};

