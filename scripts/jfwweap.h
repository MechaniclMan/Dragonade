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
class JFW_Nod_Turret : public ScriptImpClass {
	unsigned int id1;
	unsigned int id2;
	unsigned int id3;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Nod_Obelisk_CnC : public ScriptImpClass {
	int weaponid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Obelisk_Weapon_CnC : public ScriptImpClass {
	bool EnemySeen;
	int attackid;
	int effectid;
	float range;
	bool attack;
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Advanced_Guard_Tower : public ScriptImpClass {
	int missileid;
	int gun1id;
	int gun2id;
	int gun3id;
	int gun4id;
	bool enable;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Advanced_Guard_Tower_2 : public ScriptImpClass {
	int missileid;
	int gun1id;
	int gun2id;
	int gun3id;
	int gun4id;
	bool enable;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Advanced_Guard_Tower_Missile : public ScriptImpClass {
	bool firing;
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Advanced_Guard_Tower_Gun : public ScriptImpClass {
	int id;
	bool disable;
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Guard_Duty_Helper : public ScriptImpClass {
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
};

class JFW_Guard_Duty : public ScriptImpClass {
	int helperid;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Hunt_Attack : public ScriptImpClass {
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
};

class JFW_Hunt_Blow_Up : public ScriptImpClass {
	bool firsttime;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	public: void Register_Auto_Save_Variables();
};

class JFW_Clear_Weapons : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Zone_Weapon : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Weapon_Change_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Weapon_Change_On_Custom_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Weapon_Change_On_Custom_Multiple : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Weapon_Change_On_Custom_Multiple_Sender : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Clear_Weapons_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Vehicle_Weapon_Preset : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Vehicle_Weapon_Weapon : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Clear_Weapons_Create : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Fire_Weapon_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Vehicle_Weapon_Switcher : public JFW_Key_Hook_Base {
	bool switching;
	bool newweap;
	int pilotID;
	const char *new_weapon;
	const char *old_weapon;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	void KeyHook();
};

class JFW_Char_Weapon_Switcher : public JFW_Key_Hook_Base {
	time_t LastSwitch;
	bool Weapon2;
	void Created( GameObject *obj );
	void KeyHook();
	void Timer_Expired( GameObject *obj,int number );
};
