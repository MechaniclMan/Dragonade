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


class z_Eve_Start : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class z_Eve_Count_Sams : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int count;
};

class z_Eve_DropRockets : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int count;
};

class z_Eve_Death_Drop_Rockets : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class z_Eve_SAMsDead_DropWeps : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
	const char *scriptname;
	int id1;
	int id2;
	int id3;
	bool loop;
	bool startnow;
	int msg;
	float delay;
	float startnowdelay;
	int loops;
	int loopsmax;
};

class z_Eve_Hostage : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Killed(GameObject *obj, GameObject *shooter);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	public: void Register_Auto_Save_Variables();
	bool allowpoke;
};

class z_Eve_Make_1P_Engineer : public ScriptImpClass {
	void Created(GameObject *obj);
};

class z_Eve_Drop_Engineers : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class z_Eve_Enter_SetHosties : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class z_Eve_Hostage_Count : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int saves;
};

class z_Eve_End_Mission : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

















