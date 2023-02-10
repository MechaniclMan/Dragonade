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
class JFW_User_Settable_Parameters : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Debug_Text_File : public ScriptImpClass {
	const char *filename;
	const char *description;
	FILE *f;
	time_t t2;
	time_t t;
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Sound_Heard(GameObject *obj,const CombatSound & sound);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void Poked(GameObject *obj,GameObject *poker);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JFW_Power_Off : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Follow_Waypath : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Object_Draw_In_Order : public ScriptImpClass {
	int currentmodelnumber;
	int currentmodelid;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Object_Draw_In_Order_2 : public ScriptImpClass {
	int currentmodelnumber;
	int currentmodelid;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Object_Draw_Random : public ScriptImpClass {
	int currentmodelid;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Play_Animation_Destroy_Object : public ScriptImpClass {
	void Created(GameObject *obj);
	void Animation_Complete(GameObject *obj,const char *animation_name);
};

class JFW_Model_Animation : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Animated_Effect : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Animation_Complete(GameObject *obj,const char *animation_name);
};

class JFW_Animated_Effect_2 : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Animation_Complete(GameObject *obj,const char *animation_name);
};

class JFW_Random_Animated_Effect : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Animation_Complete(GameObject *obj,const char *animation_name);
};

class JFW_Random_Animated_Effect_2 : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Animation_Complete(GameObject *obj,const char *animation_name);
};

class JFW_Fog_Create : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_War_Blitz_Create : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Goto_Object_On_Startup : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_BHS_DLL : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Scope : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_HUD_INI : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Screen_Fade_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Screen_Fade_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Screen_Fade_On_Exit : public ScriptImpClass {
	void Exited(GameObject *obj,GameObject *exiter);
};

class JFW_Screen_Fade_Custom_Timer : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Stealthable_Object : public ScriptImpClass {
	bool enabled;
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};

class JFW_Object_Counter : public ScriptImpClass {
	int count;
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	public: void Register_Auto_Save_Variables();
};

class JFW_Change_Spawn_Character : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Show_Info_Texture : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Wireframe_Mode : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Change_Radar_Map : public ScriptImpClass {
	void Created(GameObject *obj);
};
class JFW_Goto_Player_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
};

class JFW_Credit_Grant : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_PT_Hide_Death : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_PT_Hide : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_PT_Hide_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_PT_Hide_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_PT_Show_Death : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_PT_Show : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_PT_Show_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_PT_Show_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Follow_Waypath_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Set_Info_Texture : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Clear_Info_Texture : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Time_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Set_Time_Limit_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Radar_Low_Power : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Game_Hint_Soldier : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Game_Hint_Vehicle : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Game_Hint_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Game_Hint_Image_Soldier : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Game_Hint_Image_Vehicle : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Game_Hint_Image_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

class JFW_Moon_Is_Earth : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Points_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Display_String_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Preset_Disable : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	unsigned int count;
};

class JFW_Power_Disable : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	unsigned int count;
};

class JFW_Radar_Disable : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	unsigned int count;
};

class JFW_Conyard_Disable : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	unsigned int count;
};

class JFW_Refinery_Disable : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	unsigned int count;
};

class JFW_VehicleFactory_Disable : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	unsigned int count;
};

class JFW_AirFactory_Disable : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	unsigned int count;
};

class JFW_NavalFactory_Disable : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	unsigned int count;
};

class JFW_Engineer_Hack : public ScriptImpClass {
	unsigned int Warhead;
	int IndcatorID;
	float HackHealth;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *killer);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Timer_Expired(GameObject *obj,int number);
	void Apply_Damage(GameObject *obj,float damage);
	void Apply_Repair(GameObject *obj,float repair);
};

class JFW_Double_Animation : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_EMP : public ScriptImpClass {
	unsigned int Warhead;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_EMP_Mine : public ScriptImpClass {
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *enemy);
	void Destroyed(GameObject *obj);
};

class JFW_EMP_Mine_Manager : public ScriptImpClass {
	int mines;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_EMP_Mine_Layer : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float amount);
};

class JFW_Radar_Disable_Death : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Cyborg_Reaper : public ScriptImpClass {
	int driverid;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Limpet_Drone : public ScriptImpClass {
	bool stealth;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Forward_Custom_Object : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Death_Send_Custom_Self : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

class JFW_Hunter_Seeker : public JFW_Key_Hook_Base {
	int driverid;
	void Created(GameObject *obj);
	void KeyHook();
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Ion_Storm : public ScriptImpClass {
	bool storm;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Tech_Level_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class JFW_Tech_Level_Startup : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Tech_Level_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JFW_Change_Character_Created : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Change_Model_Created : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Spawn_Object_Created : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JFW_Killed_String_Sound : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};
