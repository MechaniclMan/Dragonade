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

class z_Ship_TorpedoRack : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	public: void Register_Auto_Save_Variables();
	bool sabotaged;
};

class z_Ship_TorpedoRackCount : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int count;
};

class z_Ship_TorpedoRackHelp : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class z_Ship_Intro : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class z_Ship_BigSam : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class z_Ship_BigSamDead_Custom : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};

class z_Ship_BigSamAnnouncezone : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class z_Ship_EnginesAnnounceZone : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class z_Ship_EngineCount : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int count;
};

class z_Ship_EngineCriticalSystem : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj, int number);
	bool allowpoke;
	GameObject *pokerguy;
};

class z_Ship_LocateGreenKeyZone : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class z_Ship_ObtainPrisonKeyAnnounceZone : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class z_Ship_PrisonersEnableSubmarine : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class z_Ship_Prisoner_Control : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj, int number);
	int deaths;
	int reqres;
	int saves;
	bool playwarning;
};

class z_Ship_TorpedoRacks_EnableRedKey : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
	int count;
};

class z_Ship_TorpedoRacks_AnnounceZone : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class z_Ship_TorpedoRack2 : public ScriptImpClass {
	void Poked(GameObject *obj, GameObject *poker);
};

class z_Ship_Prisoner_Waypath_Poke : public ScriptImpClass {
	void Poked(GameObject *obj, GameObject *poker);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Created(GameObject *obj);
	public: void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj, int number);
	void Killed(GameObject *obj, GameObject *shooter);
	bool allowpoke;
	int followid;
};

class z_Ship_Prisoner_Enter_Sub_Zone : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool res1;
	bool res2;
	bool res3;
	bool res4;
	bool res5;
};

class z_Ship_End : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};












