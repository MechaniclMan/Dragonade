/*	Renegade Scripts.dll
	Copyright 2011 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#pragma once

class z_jret_briefing : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class z_jret_Disable_Obi : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};

class z_jret_VehDrops : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	bool allowpoke;
	public: void Register_Auto_Save_Variables();
	GameObject *pokerguy;
};

class z_jret_WepDrop_PP1 : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class z_jret_Drops_Obi1 : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class z_jret_WepDrop_PP2 : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class z_jret_VehDrops_Obi1 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	bool allowpoke;
	public: void Register_Auto_Save_Variables();
	GameObject *pokerguy;
};

class z_jret_Obi1 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj, GameObject *shooter);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool playdamage;
	bool alive;
};

class z_jret_CheckpointControl : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Poked(GameObject *obj, GameObject *poker);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
	GameObject *PokerGuy;
	bool AllowPoke;
	int CheckID;
	int Check1Locked;
	int Check2Locked;
	int Check3Locked;
	int Check4Locked;
	int Check5Locked;
	int Check6Locked;
};

class z_jret_Obi2 : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class z_jret_VehDrops_Obi2 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	bool allowpoke;
	public: void Register_Auto_Save_Variables();
	GameObject *pokerguy;
};

class z_jret_DropMoreMP : public ScriptImpClass {
	void Created(GameObject *obj);
};

class z_jret_DropIons_Obi2 : public ScriptImpClass {
	void Timer_Expired(GameObject *obj, int number);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class z_jret_Airstrip_Vehicles : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Killed(GameObject *obj, GameObject *shooter);
};

class z_jret_PP1 : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class z_jret_PP2 : public ScriptImpClass {
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj, int number);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Killed(GameObject *obj, GameObject *shooter);
	bool playdamage;
	bool alive;
};
