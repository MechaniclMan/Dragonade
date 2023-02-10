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

class AOW_Building_Camera : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
	bool Seen;
	int team;
};

class AOW_Set_Max_Health_Armor_Created : public ScriptImpClass {
	void Created(GameObject *obj);
};

class AOW_Set_Team : public ScriptImpClass {
	void Created(GameObject *obj);
};
