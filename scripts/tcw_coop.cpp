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
#include "General.h"
#include "scripts.h"
#include "engine.h"
#include "tcw_coop.h"

//The_Game()->Get_Game_Duration_S() to get elapsed time in seconds for some missions maybe

void tcw_i_am_a_bot::Created(GameObject *obj)
{
	//lol
}
void tcw_i_am_a_bot::Destroyed(GameObject *obj)
{
	int random = Commands->Get_Random_Int(1,11);
	if (random == 1)
	{
		Vector3 place = Commands->Get_Position(obj);	place.Z += 0.25f;
		GameObject *pup = Commands->Create_Object("TCW_POW_Armor_025",place);
		Commands->Set_Facing(pup,Commands->Get_Facing(obj));
		Attach_Script_Once(pup,"M00_Powerup_Destroy","");
	}
	else if (random == 2)
	{
		Vector3 place = Commands->Get_Position(obj);	place.Z += 0.25f;
		GameObject *pup = Commands->Create_Object("TCW_POW_Health_025",place);
		Commands->Set_Facing(pup,Commands->Get_Facing(obj));
		Attach_Script_Once(pup,"M00_Powerup_Destroy","");
	}
	else if (random == 3)
	{
		Vector3 place = Commands->Get_Position(obj);	place.Z += 0.25f;
		GameObject *pup = Commands->Create_Object("TCW_POW_Armor_050",place);
		Commands->Set_Facing(pup,Commands->Get_Facing(obj));
		Attach_Script_Once(pup,"M00_Powerup_Destroy","");
	}
	else if (random == 4)
	{
		Vector3 place = Commands->Get_Position(obj);	place.Z += 0.25f;
		GameObject *pup = Commands->Create_Object("TCW_POW_Health_050",place);
		Commands->Set_Facing(pup,Commands->Get_Facing(obj));
		Attach_Script_Once(pup,"M00_Powerup_Destroy","");
	}
	else if (random == 5)
	{
		Vector3 place = Commands->Get_Position(obj);	place.Z += 0.25f;
		GameObject *pup = Commands->Create_Object("TCW_POW_Armor_100",place);
		Commands->Set_Facing(pup,Commands->Get_Facing(obj));
		Attach_Script_Once(pup,"M00_Powerup_Destroy","");
	}
	else if (random == 6)
	{
		Vector3 place = Commands->Get_Position(obj);	place.Z += 0.25f;
		GameObject *pup = Commands->Create_Object("TCW_POW_Health_100",place);
		Commands->Set_Facing(pup,Commands->Get_Facing(obj));
		Attach_Script_Once(pup,"M00_Powerup_Destroy","");
	}
	else
	{
		Destroy_Script();
	}
}

void tcw_attach_script_simple_timer::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("TimerID"));
}
void tcw_attach_script_simple_timer::Timer_Expired(GameObject *obj, int number)
{
	if (number == Get_Int_Parameter("TimerID"))
	{
		Attach_Script_Once(obj,Get_Parameter("Scriptname"),"");
	}
}

void tcw_enable_spawners_killed::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner01"),Get_Int_Parameter("Enable"));
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner02"),Get_Int_Parameter("Enable"));
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner03"),Get_Int_Parameter("Enable"));
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner04"),Get_Int_Parameter("Enable"));
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner05"),Get_Int_Parameter("Enable"));
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner06"),Get_Int_Parameter("Enable"));
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner07"),Get_Int_Parameter("Enable"));
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner08"),Get_Int_Parameter("Enable"));
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner09"),Get_Int_Parameter("Enable"));
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner10"),Get_Int_Parameter("Enable"));
}

void tcw_midlight_eva1::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,17.0f,123123123);//skip over the map intro
}
void tcw_midlight_eva1::Timer_Expired(GameObject *obj, int number)
{
	if (number == 123123123)
	{
		Send_Message(255,204,0,"Lt. Sandra Telfair: Remember Commanders that by destroying the enemy Hand of Nod you stop infantry reinforcements.");
		Commands->Create_2D_WAV_Sound("midlight_destroy_hand_stop_reinf.wav");
	}
}

void tcw_goto_player_attack::Created(GameObject *obj)
{
    Commands->Start_Timer(obj, this, 3.500000, 20000);
} 
void tcw_goto_player_attack::Timer_Expired(GameObject *obj, int number)
{
    if(number == 20000)
    {
		GameObject *player = Commands->Get_A_Star(Commands->Get_Position(obj));
		if (player)
		{
			ActionParamsStruct Params;
			Params.Set_Basic(this,100,20000);
			Params.Set_Movement(player,0.800000f,2.500000f);
			Params.Set_Attack(player,50.000000f,1.000000f,true);
			Commands->Action_Attack(obj,Params);
			Commands->Start_Timer(obj,this,3.0f,20000);
		}
		else
		{
			player = 0;	Commands->Start_Timer(obj,this,3.0f,20000);
		}
	}
}
 
void tcw_goto_player::Created(GameObject *obj)
{
        Commands->Enable_Hibernation(obj,false);
        Commands->Start_Timer(obj,this,5.0f,65);
        ActionParamsStruct Params;
        Params.Set_Basic(this,45,63);
        Params.Set_Movement(Commands->Get_A_Star(Commands->Get_Position(obj)),1,0.8f);
        Commands->Action_Goto(obj,Params);
}
void tcw_goto_player::Timer_Expired(GameObject *obj, int number)
{
        if (number == 65 && obj)
        {
                ActionParamsStruct Params;
                Params.Set_Basic(this,45,63);
                Params.Set_Movement(Commands->Get_A_Star(Commands->Get_Position(obj)),1,0.8f);
                Commands->Action_Goto(obj,Params);
                Commands->Start_Timer(obj,this,5.0f,65);
        }
}

ScriptRegistrant<tcw_goto_player> tcw_goto_player_Registrant("tcw_goto_player","");
ScriptRegistrant<tcw_goto_player_attack> tcw_goto_player_attack_Registrant("tcw_goto_player_attack","");
ScriptRegistrant<tcw_midlight_eva1> tcw_midlight_eva1_Registrant("tcw_midlight_eva1","");
ScriptRegistrant<tcw_enable_spawners_killed> tcw_enable_spawners_killed_Registrant("tcw_enable_spawners_killed","Enable=0:int,Spawner01=0:int,Spawner02=0:int,Spawner03=0:int,Spawner04=0:int,Spawner05=0:int,Spawner06=0:int,Spawner07=0:int,Spawner08=0:int,Spawner09=0:int,Spawner10=0:int,");
ScriptRegistrant<tcw_attach_script_simple_timer> tcw_attach_script_simple_timer_Registrant("tcw_attach_script_simple_timer","Time=0:float,TimerID=100:int,Scriptname=bla:string");
ScriptRegistrant<tcw_i_am_a_bot> tcw_i_am_a_bot_Registrant("tcw_i_am_a_bot","");

