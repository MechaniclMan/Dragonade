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

class tcw_pt_buy_orca_crs : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	void Poked(GameObject *obj, GameObject *poker);
	float orca_cost;
	bool allowpoke;
	int orca_max;
	int orca_buyer;
	int orca_spawn;
};

class tcw_crs_nod_building : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Killed(GameObject *obj, GameObject *shooter);
	public: void Register_Auto_Save_Variables();
	bool sendmsg;
};

class tcw_crs_attack_ref : public ScriptImpClass {
	void Created(GameObject *obj);
	void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
	void Timer_Expired(GameObject *obj, int number);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
	bool attack;
	int ref_front_turret_id;
	int ref_back_turret_id;
};

class tcw_crs_sniper_attack : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};
