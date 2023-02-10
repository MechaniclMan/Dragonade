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

class z_Poke_Play_Cinematic : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool allowpoke;
};

class z_Invasion_Hostage : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	void Destroyed(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	public: void Register_Auto_Save_Variables();
	bool playpoke;
	bool playpain;
	const char *hostyname;
	const char *hostypokesound;
	int FollowID;
};

class z_Invasion_Hostage_Count : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int deaths;
};

class z_Invasion_Bot_Counter : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int botcount;
	bool endgame;
};

class z_Invasion_Enemy : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
};

class z_Invasion_Drop_Harvester : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class z_Invasion_Loop_Badgerdrop : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int count;
};

class z_Invasion_Drop_Hummvee1 : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class z_Invasion_Drop_Hummvee2 : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class z_Invasion_Loop_Chinooks_GDIBase : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int count;
};

class z_Invasion_Buy_Guntower_North : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	float funds;
	float totalcost;
	Vector3 pos;
	Vector3 pos2;
	bool allowpoke;
};

class z_Invasion_Buy_Guntower_West : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	float funds;
	float totalcost;
	Vector3 pos;
	Vector3 pos2;
	bool allowpoke;
};

class z_Invasion_Buy_Guntower_South : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	float funds;
	float totalcost;
	Vector3 pos;
	Vector3 pos2;
	bool allowpoke;
};

class z_Invasion_Buy_Guntower_East : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	float funds;
	float totalcost;
	Vector3 pos;
	Vector3 pos2;
	bool allowpoke;
};