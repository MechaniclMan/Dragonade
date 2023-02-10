/*	Renegade Scripts.dll
	Copyright 2015 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#pragma once


class JMG_Create_Ship_On_Poke : public ScriptImpClass {
public:
	int ShipID;
	int YourID;
	const char *armor;
	int BotID;
	int cango;
	int ViewingBoxID;
	const char *Your3DModel;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	//void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Register_Auto_Save_Variables();
};

class JMG_Advanced_Bounce_Zone : public ScriptImpClass {
public:
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Ship_Random_Hyperspace : public ScriptImpClass {
public:
	const char *OriginalModel;
	int Has_Recived_Message;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Register_Auto_Save_Variables();
};

class JMG_Random_Music : public ScriptImpClass {
public:
	int songnumber;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Register_Auto_Save_Variables();
};

class JMG_Ship_Weapon : public ScriptImpClass {
public:
	int newtotal;
	int Ammo;
	int cangainammo;
	int Currentammo;
	int ammocheck;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Register_Auto_Save_Variables();
};

class JMG_Return_Custom_When_Shot : public ScriptImpClass {
public:
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Cronie_AI : public ScriptImpClass {
public:
	int target;
	void Created(GameObject *obj);
	//void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Register_Auto_Save_Variables();
};

class JMG_Kill_Self_And_Sender_On_Custom : public ScriptImpClass {
public:
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Ship_Shield_Control_Script : public ScriptImpClass {
public:
	int ison;
	int ObjID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj, GameObject *damager);
	//void Timer_Expired(GameObject *obj,int number);
	void Register_Auto_Save_Variables();
};

class JMG_Kill_Self_And_Sender_On_Custom_Ship : public ScriptImpClass {
public:
	int canbekilled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Register_Auto_Save_Variables();
};

class JMG_Ship_Shield_Script : public ScriptImpClass {
public:
	const char *ShieldModel;
	int LastSenderID;
	int ReccentlyDamaged;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Register_Auto_Save_Variables();
};

class JMG_Renetbusters_Object : public ScriptImpClass {
public:
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Ship_Control_Script : public ScriptImpClass {
public:
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_New_Ship_Weapon_Control_System : public ScriptImpClass {
public:
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Comet_Control_Script : public ScriptImpClass {
public:
	void Created(GameObject *obj);
	void Killed(GameObject *obj, GameObject *damager);
};

class JMG_Attach_And_Remove : public ScriptImpClass {
public:
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};
