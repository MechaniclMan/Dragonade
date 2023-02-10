/*	Renegade Scripts.dll
	Scripts from - http://www.tiberiumcrystalwar.com
	Copyright 2011-2012 MPF

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it 
	under the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code 
	with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "BuildingGameObj.h"
#include "tcw_coop_cruising.h"

void tcw_pt_buy_orca_crs::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	allowpoke = true;
	orca_buyer = 0;
	orca_cost = 1100.0f;
	orca_max = 12;
	orca_spawn = Get_Int_Parameter("Orca_Spawn_ID");
}
void tcw_pt_buy_orca_crs::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (Get_Object_Count(2,"TCW_GDI_Orca") < 12)
		{
			if (Commands->Get_Money(poker) >= orca_cost)
			{
				orca_buyer = Commands->Get_ID(poker);
				Commands->Start_Timer(obj,this,1.0f,102);//create orca after success but disallow poking for 15 seconds in next line of code to prevent ppl buying orca's that get stuck in each other
				allowpoke = false; Commands->Enable_HUD_Pokable_Indicator(obj, false) ;Commands->Start_Timer(obj,this,15.0f,100);//15 seconds to get in the orca and fly away
				Commands->Give_Money(poker,-orca_cost,0);//take money :D
				char buymsg[250]; sprintf(buymsg,"%s purchased a GDI Orca.",Get_Player_Name(Commands->Find_Object(orca_buyer))); Send_Message(255,255,255,buymsg);
			}
			else
			{
				allowpoke = false; Commands->Enable_HUD_Pokable_Indicator(obj, false); Commands->Start_Timer(obj,this,5.0f,101);//5 second delay for pressing e again so you cant spam-press-e on it
			}
		}
		else if (Get_Object_Count(2,"TCW_GDI_Orca") >= 12)
		{
			Send_Message_Player(poker,255,255,255,"Sorry, the Orca limit of 12 has been reached. Try again later.");
		}
	}
}
void tcw_pt_buy_orca_crs::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)//succesful purchase enable poking
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
	else if (number == 101)//unsuccesful purchase
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
	else if (number == 102)//succesful purchase create orca
	{
		Vector3 orca_spawnpos = Commands->Get_Position(Commands->Find_Object(orca_spawn));
		GameObject *orca = Commands->Create_Object("TCW_GDI_Orca",orca_spawnpos);
		Attach_Script_Once(orca,"tcw_vehexit_killtimer","");//put script that destroys orca when it is exitted so players dont fuckup the game by leaving empty orcas everywhere.
	}
}
void tcw_pt_buy_orca_crs::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowpoke,sizeof(allowpoke),1);
	Auto_Save_Variable(&orca_buyer,sizeof(orca_buyer),2);
	Auto_Save_Variable(&orca_cost,sizeof(orca_cost),3);
	Auto_Save_Variable(&orca_spawn,sizeof(orca_spawn),4);
}

void tcw_crs_nod_building::Created(GameObject *obj)
{
	sendmsg = true;
}
void tcw_crs_nod_building::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&sendmsg,sizeof(sendmsg),1);
}
void tcw_crs_nod_building::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Refinery") == 0)
	{
		if ( (strcmp(Commands->Get_Preset_Name(damager),"TCW_GDI_Sniper") == 0) && !Get_Vehicle(damager) )
		{
			if (sendmsg)
			{
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100250),1000,1,0);//enable front turret firing at ref
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100251),1000,1,0);//enable back turret firing at ref
				Attach_Script_Once(damager,"tcw_crs_sniper_attack","0");//when this script is attached with param 0 when this sniper (damager) dies it will stop the turret fire
			}
		}
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_Hand_Of_Nod") == 0)
	{
		if ( (strcmp(Commands->Get_Preset_Name(damager),"TCW_GDI_Sniper") == 0) && !Get_Vehicle(damager) )
		{
			if (sendmsg)
			{
				sendmsg = false; //only allow once
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100250),1000,1,0);//enable front turret firing at ref
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100251),1000,1,0);//enable back turret firing at ref
				Attach_Script_Once(damager,"tcw_crs_sniper_attack","0");//when this script is attached with param 0 when this sniper (damager) dies it will stop the turret fire
			}
		}
	}
}
void tcw_crs_nod_building::Killed(GameObject *obj, GameObject *shooter)
{
	int type = Get_Int_Parameter("Type");
	if (type == 1) //hand
	{
		Send_Message(255,255,255,"All Nod advanced infantry production stopped.");
		//disable all advanced inf spawners
		Commands->Enable_Spawner(100155,false);//rocketguy stationary
		Commands->Enable_Spawner(100156,false);//rocketguy stationary
		Commands->Enable_Spawner(100157,false);//rocketguy stationary
		Commands->Enable_Spawner(100158,false);//rocketguy stationary
		Commands->Enable_Spawner(100159,false);//rocketguy stationary
		Commands->Enable_Spawner(100160,false);//rocketguy stationary
		Commands->Enable_Spawner(100161,false);//rocketguy stationary
		Commands->Enable_Spawner(100162,false);//rocketguy stationary
		Commands->Enable_Spawner(100206,false);//rocketguy stationary
		Commands->Enable_Spawner(100207,false);//rocketguy stationary
		Commands->Enable_Spawner(100208,false);//rocketguy stationary
		Commands->Enable_Spawner(100209,false);//rocketguy stationary
		Commands->Enable_Spawner(100210,false);//rocketguy stationary
		Commands->Enable_Spawner(100211,false);//rocketguy stationary
		Commands->Enable_Spawner(100212,false);//rocketguy stationary
		Commands->Enable_Spawner(100213,false);//rocketguy stationary
		Commands->Enable_Spawner(100214,false);//rocketguy stationary

		Commands->Enable_Spawner(100215,false);//rocketguy
		Commands->Enable_Spawner(100216,false);//rocketguy
		Commands->Enable_Spawner(100217,false);//rocketguy
		Commands->Enable_Spawner(100218,false);//rocketguy
		Commands->Enable_Spawner(100219,false);//rocketguy
		Commands->Enable_Spawner(100220,false);//rocketguy
		Commands->Enable_Spawner(100221,false);//rocketguy
		Commands->Enable_Spawner(100222,false);//rocketguy

		Commands->Enable_Spawner(100223,false);
		Commands->Enable_Spawner(100224,false);
		Commands->Enable_Spawner(100225,false);
		Commands->Enable_Spawner(100226,false);
		Commands->Enable_Spawner(100227,false);
		Commands->Enable_Spawner(100228,false);
		Commands->Enable_Spawner(100229,false);
		Commands->Enable_Spawner(100230,false);
		Commands->Enable_Spawner(100231,false);
		Commands->Enable_Spawner(100232,false);
		Commands->Enable_Spawner(100233,false);
		Commands->Enable_Spawner(100234,false); //commando
		Commands->Enable_Spawner(100235,false);
		Commands->Enable_Spawner(100236,false);
		Commands->Enable_Spawner(100237,false);
		Commands->Enable_Spawner(100238,false);
		Commands->Enable_Spawner(100239,false);
		Commands->Enable_Spawner(100240,false);
		Commands->Enable_Spawner(100241,false);
		Commands->Enable_Spawner(100242,false); //commando
		Commands->Enable_Spawner(100243,false);
		Commands->Enable_Spawner(100244,false);
		Commands->Enable_Spawner(100245,false);
		Commands->Enable_Spawner(100246,false);
		Commands->Enable_Spawner(100247,false);
		Commands->Enable_Spawner(100248,false);
		Commands->Enable_Spawner(100249,false);
	}
	else if (type == 2) //nwf
	{
		//disable all nod vehicle spawners
	}
	else if (type == 3) //nref
	{
		Send_Message(255,255,255,"Some Nod advanced infantry production stopped.");
		//disable some random vehicle and inf spawners
		Commands->Enable_Spawner(100242,false); //commando
		Commands->Enable_Spawner(100156,false);//rocketguy stationary
		Commands->Enable_Spawner(100160,false);//rocketguy stationary
		Commands->Enable_Spawner(100208,false);//rocketguy stationary
		Commands->Enable_Spawner(100212,false);//rocketguy stationary
		Commands->Enable_Spawner(100217,false);//rocketguy
		Commands->Enable_Spawner(100219,false);//rocketguy
		Commands->Enable_Spawner(100222,false);//rocketguy
		Commands->Enable_Spawner(100225,false);
		Commands->Enable_Spawner(100231,false);
		Commands->Enable_Spawner(100226,false);
		Commands->Enable_Spawner(100235,false);
		Commands->Enable_Spawner(100239,false);
		Commands->Enable_Spawner(100243,false);
		Commands->Enable_Spawner(100246,false);
		Commands->Enable_Spawner(100249,false);
	}
}

void tcw_crs_attack_ref::Created(GameObject *obj)
{
	attack = false;
	ref_front_turret_id = 100250;
	ref_back_turret_id = 100251;
}
void tcw_crs_attack_ref::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)//ref
	{
		if (param == 0)
		{
			attack = false;
			Commands->Action_Reset(obj,100);//stop current attack
		}
		else if (param == 1)
		{
			attack = true;
			Commands->Start_Timer(obj,this,1.0f,101);//start attacking
		}
	}
}
void tcw_crs_attack_ref::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		if (attack)
		{
			if (Commands->Get_Health(Find_Building_By_Type(0,BuildingConstants::TYPE_REFINERY)) > 1.0f)//if the refinery is alive
			{
				ActionParamsStruct params;
				int priority = 100;		float range = 1500.0f;		float deviation = 0.0f;		int primary = 1;
				params.Set_Basic(this,(float)priority,40016);
				params.Set_Attack(Commands->Find_Object(100252),range,deviation,primary);//attack ref point
				params.AttackCheckBlocked = false;
				Commands->Action_Attack(obj,params);
				if (attack)
				{
					Commands->Start_Timer(obj,this,1.0f,101);
				}
				else if (!attack)//if not in attackmode ie: ref or sniper are dead
				{
					Commands->Action_Reset(obj,100);//stop attacking mode
				}
			}
			else if (Commands->Get_Health(Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY)) > 1.0f)//if the hand is alive
			{
				ActionParamsStruct params;
				int priority = 100;		float range = 1500.0f;		float deviation = 0.0f;		int primary = 1;
				params.Set_Basic(this,(float)priority,40016);
				params.Set_Attack(Commands->Find_Object(100254),range,deviation,primary);//attack hand point
				params.AttackCheckBlocked = false;
				Commands->Action_Attack(obj,params);
				if (attack)
				{
					Commands->Start_Timer(obj,this,1.0f,101);
				}
				else if (!attack)//if not in attackmode ie: ref or sniper are dead
				{
					Commands->Action_Reset(obj,100);//stop attacking mode
				}
			}
			else
			{
				attack = false;
				Commands->Action_Reset(obj,100);//stop attacking mode
			}
		}
	}
}
void tcw_crs_attack_ref::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	Commands->Action_Reset(obj,100);
}
void tcw_crs_attack_ref::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ref_front_turret_id,sizeof(ref_front_turret_id),1);
	Auto_Save_Variable(&ref_back_turret_id,sizeof(ref_back_turret_id),2);
	Auto_Save_Variable(&attack,sizeof(attack),3);
}

void tcw_crs_sniper_attack::Destroyed(GameObject *obj)
{
	int defense = Get_Int_Parameter("Defense_ID");
	if (defense == 0)//sniper targetted the ref so we disable the ref turret attacks
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100250),1000,0,0);//disable front turret firing at ref
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100251),1000,0,0);//disable back turret firing at ref
	}
}

ScriptRegistrant<tcw_crs_sniper_attack> tcw_crs_sniper_attack_Registrant("tcw_crs_sniper_attack","Defense_ID=0:int");
ScriptRegistrant<tcw_crs_attack_ref> tcw_crs_attack_ref_Registrant("tcw_crs_attack_ref","");
ScriptRegistrant<tcw_crs_nod_building> tcw_crs_nod_building_Registrant("tcw_crs_nod_building","Type=1:int");
ScriptRegistrant<tcw_pt_buy_orca_crs> tcw_pt_buy_orca_crs_Registrant("tcw_pt_buy_orca_crs","Orca_Spawn_ID=123:int");