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


class z_I_Am_A_Player : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj, GameObject *shooter);
	void Timer_Expired(GameObject *obj, int number);
};

class z_I_Can_Steal_Tech : public ScriptImpClass {
	void Created(GameObject *obj);
};

class z_AI_Bot_Attach_Dropweapons : public JFW_Object_Created_Hook_Base {
	void ObjectCreateHook(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class z_AI_Bot_Dropweapons : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

class z_Building_Hand_Of_Nod : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Killed(GameObject *obj, GameObject *shooter);
	public: void Register_Auto_Save_Variables();
	bool alive;
	bool playdmg;
};











