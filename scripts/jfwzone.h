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
class JFW_3D_Sound_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Bounce_Zone_Entry : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Grant_Key_Zone_Entry : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Zone_Send_Custom_Enable : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	public: void Register_Auto_Save_Variables();
};

class JFW_Zone_Send_Custom_Preset : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JFW_Zone_Send_Custom_Enable_Enter : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	public: void Register_Auto_Save_Variables();
};

class JFW_Zone_Send_Custom_Preset_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JFW_Send_Custom_Zone_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JFW_Gate_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Repair_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Repair_Zone_Aircraft_Only : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Repair_Zone_No_Aircraft : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Zone_Send_Custom_Not_Preset : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JFW_Zone_Send_Custom_Keycard : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JFW_Heal_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Heal_Zone_2 : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Disable_Transitions_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JFW_Bounce_Zone_Entry_All_Directions : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Zone_Send_Custom_Cost : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Vechicle_Damage_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Weapon_Buy : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Preset_Buy : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Zone_Character : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Zone_Character_Swap : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Character_Buy : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Teleport_Zone_Team : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Repair_Zone_VTOL_Only : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Repair_Zone_No_VTOL : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Refill_Buy : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Powerup_Buy : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Vehicle_Buy : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Zone_Vehicle : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Zone_Powerup : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Zone_Refill : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Zone_Send_Custom_Team: public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JFW_Zone_Animation : public ScriptImpClass {
	bool enabled;
	int ID;
	void Created(GameObject *obj);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	public: void Register_Auto_Save_Variables();
};

class JFW_Group_Purchase_Zone : public ScriptImpClass {
	float cash;
	bool enabled;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	public: void Register_Auto_Save_Variables();
};

class JFW_Zone_Timer : public ScriptImpClass {
	int count;
	bool enabled;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Gate_Zone_2 : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Blow_Up_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Blow_Up_On_Enter_Delay : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Apply_Damage_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Apply_Damage_On_Enter_Timer : public ScriptImpClass {
	int ID;
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Blow_Up_On_Enter_Random : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Create_Destroy_Object_On_Enter : public ScriptImpClass {
	int ObjectID;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	public: void Register_Auto_Save_Variables();
};

class JFW_Send_Driver_Custom_On_Enter : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Stealth_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JFW_Fog_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_War_Blitz_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Heavy_Vehicle_Damage_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Light_Vehicle_Damage_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_2D_Sound_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Zone_Send_Custom_Multiple : public ScriptImpClass {
	int count;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	public: void Register_Auto_Save_Variables();
};

class JFW_Zone_Send_Custom_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Zone_Send_Custom_Exit : public ScriptImpClass {
	void Exited(GameObject *obj,GameObject *exiter);
};

class JFW_Vehicle_Zone_Animation : public ScriptImpClass {
	int refcount;
	int ID;
	void Created(GameObject *obj);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	public: void Register_Auto_Save_Variables();
};

class JFW_Zone_Money_Preset : public ScriptImpClass {
	int ID;
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	public: void Register_Auto_Save_Variables();
};

class JFW_Look_At_Location_Entry : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Look_At_Object_Entry : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_3D_Sound_Team_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_3D_Sound_Player_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_2D_Sound_Team_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_2D_Sound_Player_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Zone_PCT : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Disable_Radar_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JFW_PPAGE_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_MSG_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_TMSG_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Teleport_Zone_Enable : public ScriptImpClass {
	bool enabled;
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	public: void Register_Auto_Save_Variables();
};

class JFW_Radar_Spy_Zone : public ScriptImpClass {
	bool reset;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Radar_Spy_Zone_New : public ScriptImpClass {
	bool reset;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_2D_Sound_Zone_Team : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Repair_Zone_2 : public JFW_Key_Hook_Base {
	int ID;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Timer_Expired(GameObject *obj,int number);
	void KeyHook();
	public: void Register_Auto_Save_Variables();
};

class JFW_Repair_Zone_No_Boats : public JFW_Key_Hook_Base {
	int ID;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Timer_Expired(GameObject *obj,int number);
	void KeyHook();
	public: void Register_Auto_Save_Variables();
};

class JFW_Repair_Zone_Boats : public JFW_Key_Hook_Base {
	int ID;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void Timer_Expired(GameObject *obj,int number);
	void KeyHook();
	public: void Register_Auto_Save_Variables();
};

class JFW_Sell_Zone : public JFW_Key_Hook_Base {
	int ID;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
	void KeyHook();
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Infantry_Force_Composition_Zone : public ScriptImpClass {
	unsigned int *IDs;
	unsigned int count;
	bool reset;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
public:
	JFW_Infantry_Force_Composition_Zone() {IDs = 0;}
	~JFW_Infantry_Force_Composition_Zone();
};

class JFW_Vehicle_Force_Composition_Zone : public ScriptImpClass {
	unsigned int *IDs;
	unsigned int count;
	bool reset;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
public:
	JFW_Vehicle_Force_Composition_Zone() {IDs = 0;}
	~JFW_Vehicle_Force_Composition_Zone();
};

class JFW_Infantry_Force_Composition_Zone_New : public ScriptImpClass {
	unsigned int *IDs;
	unsigned int count;
	bool reset;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
public:
	JFW_Infantry_Force_Composition_Zone_New() {IDs = 0;}
	~JFW_Infantry_Force_Composition_Zone_New();
};

class JFW_Vehicle_Force_Composition_Zone_New : public ScriptImpClass {
	unsigned int *IDs;
	unsigned int count;
	bool reset;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
public:
	JFW_Vehicle_Force_Composition_Zone_New() {IDs = 0;}
	~JFW_Vehicle_Force_Composition_Zone_New();
};

class JFW_Resize_Zone : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Cash_Spy_Zone : public ScriptImpClass {
	bool reset;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Power_Spy_Zone : public ScriptImpClass {
	bool reset;
	int team;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Cash_Spy_Zone_New : public ScriptImpClass {
	bool reset;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Power_Spy_Zone_New : public ScriptImpClass {
	bool reset;
	int team;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Conyard_Spy_Zone : public ScriptImpClass {
	bool reset;
	bool cydisabled;
	int team;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Conyard_Spy_Zone_2 : public ScriptImpClass {
	bool reset;
	bool cydisabled;
	int team;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Conyard_Spy_Zone_2_New : public ScriptImpClass {
	bool reset;
	bool cydisabled;
	int team;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Custom_Spawn_Zone : public ScriptImpClass {
	int zoneid;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Spawn_Zone_Created : public ScriptImpClass {
	int zoneid;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Boat : public ScriptImpClass {
};

class JFW_Destroy_Vehicle_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
