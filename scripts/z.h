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

class z_Powerup_Expire : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class z_Custom_Enable_Spawner : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class z_I_May_Not_Place_Defenses : public ScriptImpClass {
	void Created(GameObject *obj);
};

class z_No_PlaceDefense_Zone_Enter : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};
class z_No_PlaceDefense_Zone_Exit : public ScriptImpClass {
	void Exited(GameObject *obj, GameObject *exit);
};

class z_Created_Timer_Enable_Spawner : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	float time;
	int num;
};
class z_S_HP1_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Timer_Expired(GameObject *obj,int number);
};
class z_S_HP2_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Timer_Expired(GameObject *obj,int number);
};
/*********************************************************************************************/
class z_Hostage : public ScriptImpClass {
	unsigned int FollowID;
	GameObject *Pokerguy;
	bool Following;
	bool AllowPoke;
	float HostageHealth;
	float HostageShield;
	float HealthRegen;
	int HostageID;
	int PokerTeam;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Poked(GameObject *obj,GameObject *poker);
	void Killed(GameObject *obj,GameObject *shooter);
	void Timer_Expired(GameObject *obj,int number);
};
/*********************************************************************************************/
class z_Hostage_Rescue_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};
/*********************************************************************************************/
class z_Hostage_Controller : public ScriptImpClass {
	int hostagedeaths;
	int hostagerescues;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};
/*********************************************************************************************/
class z_Goto_Star_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
/*********************************************************************************************/
class z_Enable_Spawner_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
/*********************************************************************************************/
class z_Enter_Enable_Spawner : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};
/*********************************************************************************************/
class z_VehExit_DestroyVeh : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	int ID;
	bool ownerisincar;
};
/*********************************************************************************************/
class z_Death_Send_Custom : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *shooter);
};
/*********************************************************************************************/
class z_Death_Send_Custom_Delay : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *shooter);
};
/*********************************************************************************************/
class z_Custom_Send_Custom_Delay : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
/*********************************************************************************************/
class z_Buy_Longbow : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	bool allowbuy;
	int longbows;
};
/*********************************************************************************************/
class z_Purchase_Pillbox : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	float funds;
};
class z_Purchase_Turret : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	float funds;
};
/*********************************************************************************************/
class z_Cinematic_Attack2 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
	void Timer_Expired(GameObject *obj, int number);
};
/*********************************************************************************************/

class z_Console_Input_Killed : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *shooter);
};

class z_Defense_Owner : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	int owner;
};

class z_Console_Input_Killed_2 : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *shooter);
};

class z_Console_Input_Custom_2 : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class z_Enable_Multiple_Spawners_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class z_Play_Three_Cinematics_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class z_Play_Cinematic_Console_Input_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class z_Destroy_Three_Objects_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class z_Console_Input_Poke_2 : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class z_Console_Input_Custom_Delay_1 : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class z_Console_Input_Custom_Delay_2 : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class z_RemoveWeaponModel : public ScriptImpClass {
	void Created(GameObject *obj);
};

class z_rwk : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class z_GameoverCreated : public ScriptImpClass {
	void Created(GameObject *obj);
};

class z_Created_Send_Custom_Param : public ScriptImpClass {
	void Created(GameObject *obj);
};

class z_NoDamageMoneyPoints : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
};

class z_Set_Team : public ScriptImpClass {
	void Created(GameObject *obj);
};

class z_Set_Skin_Created : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class z_Set_Skin : public ScriptImpClass {
	void Created(GameObject *obj);
};

class z_Death_Enable_Spawner : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *shooter);
};

class z_Death_Enable_3Spawners : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *shooter);
};

class z_UnkillableUntilEntered : public ScriptImpClass {
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class z_DestroyVeh259 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};

class z_VehExit : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class z_Spawn_When_Killed : public ScriptImpClass {
	void Killed (GameObject *obj,GameObject *shooter);
};

class z_Remove_Script_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class z_Teleport_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class z_blamo4sec : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class z_ChangeTeamPowerup : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};
class z_Start_Music_Delay : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	float duration;
	float time;
	int num;
};
class z_Engineer_Repair : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};
class z_Custom_Multiple_Send_Custom : public ScriptImpClass {
	int times_recieved;
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
class z_Pup_Allied_Tanya : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
class z_Pup_Allied_RocketGuy : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
class z_Pup_Allied_Medic : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
class z_Pup_Allied_Medic_Coop : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
class z_Pup_Allied_Sniper : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
class z_Pup_Allied_Mechanic : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
class z_Pup_Allied_Spy : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
class z_Pup_Allied_Officer : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
class z_Pup_Allied_Sniper_Coop : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
class z_TanyaVolkovAnnounce : public ScriptImpClass {
	void Created(GameObject *obj);
};
class z_Cinematic_Attack : public ScriptImpClass {
	void Created(GameObject *obj);
	void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
};
class z_Attack_MCV_SIC : public ScriptImpClass {
	void Created(GameObject *obj);
};
class z_Airraid_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};
class z_Created_Timer_EnableSpawners : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};
class z_Allied_MCV_Killed : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj, GameObject *shooter);
	void Timer_Expired(GameObject *obj,int number);
};

class z_MissionStart_SIC : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};
class z_MCV_Deploy_SIC : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};
class z_Cinematic_Attack3 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
};
class z_Soviet_Paradrop : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};
class z_OT_Deliver_SIC : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};
class z_Prototype : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};
class z_Prototype_Zone : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Timer_Expired(GameObject *obj,int number);
	int proto;
};
class z_Prototype_AllowEnter : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};
class z_Pickup_Send_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
class z_MCV_Points : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};
class z_Powerdown_Crate : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};
class z_Vehicle_AI : public ScriptImpClass {
	unsigned int targetID;
	Vector3 position;
	bool rampaging;
	bool guarding;
	bool moving;
	bool hunting;
//	int move_priority;
//	int attack_priority;
	int face_priority;
	int current_priority;
	float guard_timer;
	float attack_timer;
	Vector3 mypos;
	int move_count;

	void Created(GameObject *obj);
	void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Timer_Expired(GameObject *obj,int number);
	void Sound_Heard(GameObject *obj,const CombatSound & sound);

	void Hunt(GameObject *obj,GameObject *target);
	void Guard(GameObject *obj,GameObject *target);
	GameObject *Select_Target(GameObject *obj,GameObject *target);
	GameObject *Set_Target(GameObject *target);
	GameObject *Get_Closest(GameObject *obj,GameObject *new_target,GameObject *old_target);
	bool Set_Ammo(GameObject *target);
	bool Set_Direction(GameObject *obj);

	/*********************************************/
	//May need a Rampage Timer
	//May need 'if (obj)' error checks in the Is_xxx engine.cpp stuff
};
class z_A_HP1_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Timer_Expired(GameObject *obj,int number);
};
class z_GDI_OfficerDropGreenKey : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};
class z_BeaconNukeReady : public ScriptImpClass {
	void Created(GameObject *obj);
};
class z_BeaconIonReady : public ScriptImpClass {
	void Created(GameObject *obj);
};
class z_Delayed_Cinematic_Custom : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	bool loop;
	int msg;
	float time;
	int loops;
};
class z_Building_GDI : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
	void Timer_Expired(GameObject *obj, int number);
};
class z_Building_NOD : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
	void Timer_Expired(GameObject *obj, int number);
};
class z_Created_PlaySound : public ScriptImpClass {
	void Created(GameObject *obj);
};
class z_POW_PickupSounds : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};
class z_Created_Send_Custom_Delay : public ScriptImpClass {
	void Created(GameObject *obj);
};
class z_MP_GDI_MP : public ScriptImpClass {
	void Created(GameObject *obj);
};
class z_zone_gdienteringenemyref : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj, GameObject *enter);
	bool play;
};
class z_zone_gdienteringenemycc : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj, GameObject *enter);
	bool play;
};
class z_poke_enable_spawner : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
};
class z_Delayed_Cinematic_Custom_ID : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	const char *scriptname;
	int id;
	bool loop;
	bool startnow;
	int msg;
	float delay;
	float startnowdelay;
	int loops;
	int loopsmax;
};
class z_Enter_Send_Custom : public ScriptImpClass {//lol appearently i already made a script like that... LOL
	void Created(GameObject *obj);
	void Entered(GameObject *obj, GameObject *enter);
	void Timer_Expired(GameObject *obj, int number);
	bool domsg;
	float delay;
	int id;
	int msg;
};
class z_Poke_Grant_Weapons : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	bool allowpoke;
	float cost;
	const char *weapon1;
	const char *weapon2;
	const char *weapon3;
};
class z_Console_Input_Created : public ScriptImpClass {
	void Created(GameObject *obj);
};
class z_Created_Play_Sound_Msg : public ScriptImpClass {
	void Created(GameObject *obj);
};
class z_Custom_Play_Sound_Msg_Delay : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	const char *snd;
	const char *msg;
	int red;
	int green;
	int blue;
	int custom;
	float delay;
};

class z_Enter_Send_Custom_Once : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class z_Drop_Key_Guy : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class z_Poke_Teleport_Simple : public ScriptImpClass {
	void Poked(GameObject *obj, GameObject *poker);
};

class z_Custom_Goto_Star : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class z_Poke_PPAGE3 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool dopage;
};

class z_Poke_PAMSG3 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool dopage;
};

class z_PT_Terminal : public ScriptImpClass {
	void Poked(GameObject *obj, GameObject *poker);
};

class z_Enter_Change_Team : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class z_Enter_Teleport_Random : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class z_Vehicle_Buy_Poke_Cinematic : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool allowpoke;
	int team;
};

class z_Poke_Change_Team : public ScriptImpClass {
	void Poked(GameObject *obj, GameObject *poker);
};

class z_Timer_Set_Team : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class z_Enter_Kill_Star : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class z_Killed_Play_Sound_Message : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class z_Map_Time_Limit : public ScriptImpClass {
	void Created(GameObject *obj);
};

class z_Zone_Aircraft_Refill : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Timer_Expired(GameObject *obj,int number);
};



