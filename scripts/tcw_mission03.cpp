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
#include "BuildingGameObj.h"
#include "tcw_mission03.h"
#include "GameObjManager.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"
#include "tcw_general.h"
#include "tcw_functions.h"

void tcw_m03_controller::Created(GameObject *obj)
{
	Hide_Preset_By_Name(1,"TCW_GDI_Saboteur_M03",true);
}

void tcw_m03_steal_hon_zone::Entered(GameObject *obj, GameObject *enter)
{
	if (!Is_Building_Dead(Find_Building_By_Preset(0,"TCW_Hand_Of_Nod")))
	{
		if ( (Commands->Is_A_Star(enter)) && (Commands->Get_Player_Type(enter) == 1) )
		{
			if ( (strcmp(Commands->Get_Preset_Name(enter),"TCW_GDI_Engineer_Coop") == 0) || (strcmp(Commands->Get_Preset_Name(enter),"TCW_GDI_Engineer_Low") == 0) || 
				(strcmp(Commands->Get_Preset_Name(enter),"TCW_Nod_Saboteur") == 0) || (strcmp(Commands->Get_Preset_Name(enter),"TCW_Nod_Saboteur_Low") == 0) )
			{
				if (!Get_Vehicle(enter))
				{
					Attach_Script_Once(enter,"tcw_m03_stealing_hon_tech","");
				}
			}
		}
	}
}
void tcw_m03_steal_hon_zone::Exited(GameObject *obj, GameObject *exit)
{
	if ( (Commands->Is_A_Star(exit)) && (Commands->Get_Player_Type(exit) == 1) )
	{
		if ( (strcmp(Commands->Get_Preset_Name(exit),"TCW_GDI_Engineer_Coop") == 0) || (strcmp(Commands->Get_Preset_Name(exit),"TCW_GDI_Engineer_Low") == 0) || 
			(strcmp(Commands->Get_Preset_Name(exit),"TCW_Nod_Saboteur") == 0) || (strcmp(Commands->Get_Preset_Name(exit),"TCW_Nod_Saboteur_Low") == 0) )
		{
			Remove_Script(exit,"tcw_m03_stealing_hon_tech");
		}
	}
}

void tcw_m03_stealing_hon_tech::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,15.0f,779903);//start stealing from the nod hand of nod
	char stealermsg[250];
	sprintf(stealermsg,"%s is stealing technology from the Hand of Nod.",Get_Player_Name(obj));
	Send_Message_Team(1,255,204,0,stealermsg);
	Create_2D_WAV_Sound_Team("building_capture.wav",1);
}
void tcw_m03_stealing_hon_tech::Timer_Expired(GameObject *obj, int number)
{
	if (number == 779903)//gdi stole tech from nod hand
	{
		if (!Is_Building_Dead(Find_Building_By_Preset(0,"TCW_Hand_Of_Nod")))
		{
			Hide_Preset_By_Name(1,"TCW_GDI_Saboteur_M03",false);
			char stealmsg[250];
			sprintf(stealmsg,"%s has stolen technology from the Hand of Nod",Get_Player_Name(obj));
			Send_Message_Team(1,255,204,0,stealmsg);
			Send_Message_Team(1,255,204,0,"New technology acquired.");
			Create_2D_WAV_Sound_Team("gdi_new_tech_acquired.wav",1);
			Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_m03_hon"),1212,12,0);
		}
	}
}

void tcw_m03_gdi_win_zone::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		if (strcmp(Commands->Get_Preset_Name(enter),"TCW_GDI_Saboteur_M03") == 0)
		{
			Console_Input("amsg Mission Accomplished");
			Console_Input("win 1");
		}
	}
}

void tcw_m03_gdi_saboteur_death_count::Created(GameObject *obj)
{
	trigger = true;
	deaths = 0;
	deathsmax = 5;
}
void tcw_m03_gdi_saboteur_death_count::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1111)
	{
		if (param == 1)
		{
			deaths++;
			if (deaths >= deathsmax)
			{
				if (trigger)
				{
					Console_Input("amsg Mission Failed: 5 Saboteurs have died");
					Console_Input("win 0");
					trigger = false;
				}
			}
			else if (deaths == 1)
			{
				Send_Message(255,255,255,"Careful, you may only lose 4 more stolen Saboteurs");
			}
			else if (deaths == 2)
			{
				Send_Message(255,255,255,"Careful, you may only lose 3 more stolen Saboteurs");
			}
			else if (deaths == 3)
			{
				Send_Message(255,255,255,"Careful, you may only lose 2 more stolen Saboteurs");
			}
			else if (deaths == 4)
			{
				Send_Message(255,255,255,"Careful, you may only lose 1 more stolen Saboteur");
			}
		}
	}
}

void tcw_m03_hon::Created(GameObject *obj)
{
	allowkill = false;
	dowarning = true;
}
void tcw_m03_hon::Killed(GameObject *obj, GameObject *shooter)
{
	if (!allowkill)
	{
		Console_Input("amsg Mission Failed: The required technology was not stolen");
		Console_Input("win 0");
	}
	Send_Message(255,255,255,"All Advanced Infantry reinforcements halted.");
	Commands->Enable_Spawner(100054,false);
	Commands->Enable_Spawner(100063,false);
	Commands->Enable_Spawner(100072,false);
	Commands->Enable_Spawner(100091,false);
	Commands->Enable_Spawner(100092,false);
	Commands->Enable_Spawner(100132,false);
	Commands->Enable_Spawner(100140,false);
	Commands->Enable_Spawner(100187,false);
	Commands->Enable_Spawner(100052,false);
	Commands->Enable_Spawner(100068,false);
	Commands->Enable_Spawner(100089,false);
	Commands->Enable_Spawner(100133,false);
	Commands->Enable_Spawner(100043,false);
	Commands->Enable_Spawner(100044,false);
	Commands->Enable_Spawner(100046,false);
	Commands->Enable_Spawner(100056,false);
	Commands->Enable_Spawner(100057,false);
	Commands->Enable_Spawner(100059,false);
	Commands->Enable_Spawner(100060,false);
	Commands->Enable_Spawner(100069,false);
	Commands->Enable_Spawner(100073,false);
	Commands->Enable_Spawner(100079,false);
	Commands->Enable_Spawner(100080,false);
	Commands->Enable_Spawner(100090,false);
	Commands->Enable_Spawner(100096,false);
	Commands->Enable_Spawner(100134,false);
	Commands->Enable_Spawner(100135,false);
	Commands->Enable_Spawner(100184,false);
	Commands->Enable_Spawner(100185,false);
	Commands->Enable_Spawner(100188,false);
	Commands->Enable_Spawner(100047,false);
	Commands->Enable_Spawner(100048,false);
	Commands->Enable_Spawner(100049,false);
	Commands->Enable_Spawner(100061,false);
	Commands->Enable_Spawner(100062,false);
	Commands->Enable_Spawner(100071,false);
	Commands->Enable_Spawner(100084,false);
	Commands->Enable_Spawner(100085,false);
	Commands->Enable_Spawner(100106,false);
	Commands->Enable_Spawner(100107,false);
	Commands->Enable_Spawner(100108,false);
	Commands->Enable_Spawner(100109,false);
	Commands->Enable_Spawner(100138,false);
	Commands->Enable_Spawner(100139,false);
	Commands->Enable_Spawner(100064,false);
	Commands->Enable_Spawner(100086,false);
	Commands->Enable_Spawner(100078,false);
	Commands->Enable_Spawner(100100,false);
	Commands->Enable_Spawner(100103,false);
	Commands->Enable_Spawner(100051,false);
	Commands->Enable_Spawner(100066,false);
	Commands->Enable_Spawner(100067,false);
	Commands->Enable_Spawner(100074,false);
	Commands->Enable_Spawner(100081,false);
	Commands->Enable_Spawner(100082,false);
	Commands->Enable_Spawner(100088,false);
	Commands->Enable_Spawner(100093,false);
	Commands->Enable_Spawner(100094,false);
	Commands->Enable_Spawner(100136,false);
	Commands->Enable_Spawner(100137,false);
	Commands->Enable_Spawner(100142,false);
}
void tcw_m03_hon::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1212)
	{
		if (param == 12)
		{
			allowkill = true;	
			dowarning = false;
			Send_Message(255,255,255,"Objective Accomplished: Steal Technology from Hand of Nod");
			Send_Message(255,255,255,"Proceed and Finish Off that Hand of Nod to stop Advanced Nod Infantry Reinforcements");
		}
	}
}
void tcw_m03_hon::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (damage > 0.1)
	{
		if (!allowkill)
		{
			if (dowarning)
			{
				dowarning = false; Commands->Start_Timer(obj,this,15.0f,1050);
				Send_Message(255,255,255,"WARNING: You must first steal technology from the Hand of Nod or you will lose");
			}
		}
	}
}
void tcw_m03_hon::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1050)
	{
		if (!allowkill)
		{
			dowarning = true;
		}
	}
}

void tcw_m03_saboteur::Created(GameObject *obj)
{
	if (Get_Object_Count(1,"TCW_GDI_Saboteur_M03") > 2)
	{
		Commands->Give_Money(obj,500.0f,false);
		Send_Message_Player(obj,255,255,255,"There are 2 Saboteurs already. Please get another character and protect the existing ones.");
		Change_Character(obj,"CnC_GDI_MiniGunner_0");
	}
	else
	{
		char createmsg[512];
		sprintf(createmsg,"%s has a stolen technology Saboteur. Protect them at all cost and escort to the Docks.",Get_Player_Name(obj));
		Send_Message(255,255,255,createmsg);
	}
}
void tcw_m03_saboteur::Destroyed(GameObject *obj)
{
	Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_m03_gdi_saboteur_death_count"),1111,1,0);
}

void tcw_m03_war_factory::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Enable_Spawner(100199,false);
	Commands->Enable_Spawner(100200,false);
	Commands->Enable_Spawner(100201,false);
	Send_Message(255,255,255,"All Nod Vehicular Reinforcements halted.");
}

void tcw_m03_nod_truck::Created(GameObject *obj)
{
	triggernuke = true;
}
void tcw_m03_nod_truck::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (strcmp(Commands->Get_Preset_Name(sender),"TCW_GDI_Saboteur_M03") == 0)
		{
			Console_Input("amsg Mission Accomplished");
			Console_Input("win 1");
		}
		else 
		{
			Commands->Start_Timer(obj,this,5.0f,300);
		}
	}
}
void tcw_m03_nod_truck::Timer_Expired(GameObject *obj, int number)
{
	if (number == 300)
	{
		Console_Input("amsg Mission Failed: A non-saboteur player entered the Nod Truck");
		Console_Input("win 0");
	}
}
void tcw_m03_nod_truck::Killed(GameObject *obj, GameObject *shooter)
{
	if (Commands->Is_A_Star(shooter))
	{
		char deadmsg[512];
		sprintf(deadmsg,"%s failed the mission by destroying the Nod Truck",Get_Player_Name(shooter));
		Send_Message(255,255,255,deadmsg);
	}
	else
	{
		Console_Input("amsg Mission Failed: The Nod Truck was destroyed.");
	}
	Console_Input("win 0");
}

void tcw_m03_gdi_beachdrop_bot::Destroyed(GameObject *obj)
{
	int target = Commands->Get_ID(Find_Object_With_Script("tcw_m03_gdi_beachdrop"));
	Commands->Send_Custom_Event(obj,Commands->Find_Object(target),134135,1,0);
}

void tcw_m03_gdi_beachdrop::Created(GameObject *obj)
{
	deaths = 0;
	Commands->Start_Timer(obj,this,5.0f,101);
	Commands->Start_Timer(obj,this,7.5f,102);
	Commands->Start_Timer(obj,this,10.5f,103);
}
void tcw_m03_gdi_beachdrop::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 134135)
	{
		if (param == 1)
		{
			deaths++;
			if (deaths == 9)
			{
				Commands->Start_Timer(obj,this,5.0f,101);
				Commands->Start_Timer(obj,this,7.5f,102);
				Commands->Start_Timer(obj,this,10.5f,103);
				deaths = 0;
			}
		}
	}
}
void tcw_m03_gdi_beachdrop::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Vector3 position1;
		position1.X = -76.868f; position1.Y = -103.012f; position1.Z = 0.151f;
		GameObject *drop1 = Commands->Create_Object("Invisible_Object",position1);
		Commands->Set_Facing(drop1,45.000f);
		Commands->Attach_Script(drop1,"Test_Cinematic","gdi_reinforce1.txt");
	}
	else if (number == 102)
	{
		Vector3 position2;
		position2.X = -90.965f; position2.Y = -109.145f; position2.Z = 0.151f;
		GameObject *drop2 = Commands->Create_Object("Invisible_Object",position2);
		Commands->Set_Facing(drop2,95.000f);
		Commands->Attach_Script(drop2,"Test_Cinematic","gdi_reinforce1.txt");
	}
	else if (number == 103)
	{
		Vector3 position3;
		position3.X = -102.645f; position3.Y = -115.202f; position3.Z = 0.151f;
		GameObject *drop3 = Commands->Create_Object("Invisible_Object",position3);
		Commands->Set_Facing(drop3,15.000f);
		Commands->Attach_Script(drop3,"Test_Cinematic","gdi_reinforce1.txt");
	}
}

void tcw_m03_nod_reinforce_zone::Created(GameObject *obj)
{
	triggerdrop = true;
}
void tcw_m03_nod_reinforce_zone::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		if (triggerdrop)
		{
			triggerdrop = false; Commands->Start_Timer(obj,this,250.0f,100);//triggerdrop true
			Commands->Start_Timer(obj,this,5.0f,101);
			Commands->Start_Timer(obj,this,8.0f,102);
			Commands->Start_Timer(obj,this,11.0f,103);
		}
	}
}
void tcw_m03_nod_reinforce_zone::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		triggerdrop = true;
	}
	else if (number == 101)
	{
		Vector3 position1;
		position1.X = 75.139f; position1.Y = -76.753f; position1.Z = 0.410f;
		GameObject *drop1 = Commands->Create_Object("Invisible_Object",position1);
		Commands->Set_Facing(drop1,105.000f);
		Commands->Attach_Script(drop1,"Test_Cinematic","nod_reinforce1.txt");
	}
	else if (number == 102)
	{
		Vector3 position1;
		position1.X = 83.242f; position1.Y = -39.874f; position1.Z = 1.700f;
		GameObject *drop1 = Commands->Create_Object("Invisible_Object",position1);
		Commands->Set_Facing(drop1,45.000f);
		Commands->Attach_Script(drop1,"Test_Cinematic","nod_reinforce2.txt");
	}
	else if (number == 103)
	{
		Vector3 position1;
		position1.X = 42.817f; position1.Y = -60.138f; position1.Z = 0.801f;
		GameObject *drop1 = Commands->Create_Object("Invisible_Object",position1);
		Commands->Set_Facing(drop1,15.000f);
		Commands->Attach_Script(drop1,"Test_Cinematic","nod_reinforce3.txt");
	}
}

ScriptRegistrant<tcw_m03_nod_reinforce_zone> tcw_m03_nod_reinforce_zone_Registrant("tcw_m03_nod_reinforce_zone","");
ScriptRegistrant<tcw_m03_gdi_beachdrop> tcw_m03_gdi_beachdrop_Registrant("tcw_m03_gdi_beachdrop","");
ScriptRegistrant<tcw_m03_gdi_beachdrop_bot> tcw_m03_gdi_beachdrop_bot_Registrant("tcw_m03_gdi_beachdrop_bot","ID=0:int");
ScriptRegistrant<tcw_m03_nod_truck> tcw_m03_nod_truck_Registrant("tcw_m03_nod_truck","");
ScriptRegistrant<tcw_m03_war_factory> tcw_m03_war_factory_Registrant("tcw_m03_war_factory","");
ScriptRegistrant<tcw_m03_saboteur> tcw_m03_saboteur_Registrant("tcw_m03_saboteur","");
ScriptRegistrant<tcw_m03_hon> tcw_m03_hon_Registrant("tcw_m03_hon","");
ScriptRegistrant<tcw_m03_gdi_saboteur_death_count> tcw_m03_gdi_saboteur_death_count_Registrant("tcw_m03_gdi_saboteur_death_count","");
ScriptRegistrant<tcw_m03_gdi_win_zone> tcw_m03_gdi_win_zone_Registrant("tcw_m03_gdi_win_zone","");
ScriptRegistrant<tcw_m03_steal_hon_zone> tcw_m03_steal_hon_zone_Registrant("tcw_m03_steal_hon_zone","");
ScriptRegistrant<tcw_m03_stealing_hon_tech> tcw_m03_stealing_hon_tech_Registrant("tcw_m03_stealing_hon_tech","");
ScriptRegistrant<tcw_m03_controller> tcw_m03_controller_Registrant("tcw_m03_controller","");