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

class z_Sell_Zone : public JFW_Key_Hook_Base {
	int ID;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void KeyHook();
	public: void Register_Auto_Save_Variables();
};

class z_Aircraft_Refill : public ScriptImpClass {
	bool active;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class z_Repair_Zone_No_Boats : public JFW_Key_Hook_Base {
	int ID;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Timer_Expired(GameObject *obj,int number);
	void KeyHook();
	public: void Register_Auto_Save_Variables();
};
class z_Helipad_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};
class z_Ore_Delivery_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
class z_Repair_Zone_Boats : public JFW_Key_Hook_Base {
	int ID;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Timer_Expired(GameObject *obj,int number);
	void KeyHook();
	public: void Register_Auto_Save_Variables();
};
class z_Zone_Steal_Tech : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
	bool trigger;
	int team;
	int enterguy;
};
