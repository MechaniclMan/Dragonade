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

class z_Player : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Killed(GameObject *obj, GameObject *shooter);
	void Timer_Expired(GameObject *obj, int number);
};

class z_Tiberium_Truck : public ScriptImpClass {
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

class z_Tiberium_Field : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
	void Exited(GameObject *obj, GameObject *exit);
};

class z_Tiberium_Deposit : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class z_TiberiumTruck_Buy : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool allowpoke;
	int currenttruck;
};

class z_POW_ObiGun : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};