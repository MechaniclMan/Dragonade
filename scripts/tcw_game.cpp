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
#include "tcw_game.h"
#include "tcw_general.h"
#include "tcw_functions.h"
#include "VehicleGameObj.h"
#include "engine.h"
#include "GameObjManager.h"
#include "BuildingGameObj.h"

void tcw_Credit_Trickle::Created(GameObject *obj)
{
	float delay;
	delay = Get_Float_Parameter("Delay");
	Commands->Start_Timer(obj,this,delay,667);
}

void tcw_Credit_Trickle::Timer_Expired(GameObject *obj,int number)
{
	if (number == 667)
	{
		Commands->Give_Money(obj,(float)Get_Int_Parameter("Credits"),true);
		float delay;
		delay = Get_Float_Parameter("Delay");
		Commands->Start_Timer(obj,this,delay,667);
	}
}

void tcw_Custom_Win_Game_Credits::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 0)
		{
			float npoints = Get_Team_Credits(0);
			float gpoints = Get_Team_Credits(1);
			float nlead = npoints-gpoints;
			char pts[250];
			sprintf(pts,"amsg Nod wins with a %g credits lead",nlead);
			Console_Input(pts);
			Console_Input("win 0");
		}
		else if (param == 1)
		{
			float npoints = Get_Team_Credits(0);
			float gpoints = Get_Team_Credits(1);
			float glead = gpoints-npoints;
			char pts[250];
			sprintf(pts,"amsg GDI wins with a %g credits lead",glead);
			Console_Input(pts);
			Console_Input("win 1");
		}
		Destroy_Script();
	}
}

void tcw_Map_Intro::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,25.0f,251);
	Commands->Start_Timer(obj,this,28.0f,252);
	Commands->Start_Timer(obj,this,31.0f,253);
	Commands->Start_Timer(obj,this,34.0f,254);
	Commands->Start_Timer(obj,this,37.0f,255);
}
void tcw_Map_Intro::Timer_Expired(GameObject *obj, int number)
{
	if (number == 251)
	{
		if (_stricmp(Get_Parameter("Intro1"), "0") != 0)
		{
			char intro1[250];
			sprintf(intro1,"%s",Get_Parameter("Intro1"));
			Send_Message(Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),intro1);
		}
	}
	else if (number == 252)
	{
		if (_stricmp(Get_Parameter("Intro2"), "0") != 0)
		{
			char intro2[250];
			sprintf(intro2,"%s",Get_Parameter("Intro2"));
			Send_Message(Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),intro2);
		}
	}
	else if (number == 253)
	{
		if (_stricmp(Get_Parameter("Intro3"), "0") != 0)
		{
			char intro3[250];
			sprintf(intro3,"%s",Get_Parameter("Intro3"));
			Send_Message(Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),intro3);
		}
	}
	else if (number == 254)
	{
		if (_stricmp(Get_Parameter("Intro4"), "0") != 0)
		{
			char intro4[250];
			sprintf(intro4,"%s",Get_Parameter("Intro4"));
			Send_Message(Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),intro4);
		}
	}
	else if (number == 255)
	{
		if (_stricmp(Get_Parameter("Intro5"), "0") != 0)
		{
			char intro5[250];
			sprintf(intro5,"%s",Get_Parameter("Intro5"));
			Send_Message(Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),intro5);
		}
	}
}

void tcw_There_Must_Be_Engineer::Created(GameObject *obj)
{
	SList<SoldierGameObj> *player_list = &GameObjManager::StarGameObjList;
	SLNode<SoldierGameObj> *objnode;
	for (objnode = player_list->Head(); objnode; objnode = objnode->Next()) 
	{
		SoldierGameObj *soldier = objnode->Data();
		if ( soldier != NULL ) 
		{
			if (Commands->Get_Player_Type(soldier) == 0)
			{
				if (!Is_Building_Dead(Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY)))
				{
					if (strcmp(Commands->Get_Preset_Name(soldier),"CnC_Nod_Minigunner_0") == 0)
					{
						Change_Character(soldier,"TCW_Nod_Saboteur");
						break;
					}
					else if (strcmp(Commands->Get_Preset_Name(soldier),"TCW_Nod_Spawn_Low") == 0)
					{
						Change_Character(soldier,"TCW_Nod_Saboteur_Low");
						break;
					}
				}
				else
				{
					break;
				}
			}
			else if (Commands->Get_Player_Type(soldier) == 1)
			{
				if (!Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY)))
				{
					if (strcmp(Commands->Get_Preset_Name(soldier),"CnC_GDI_MiniGunner_0") == 0)
					{
						Change_Character(soldier,"TCW_GDI_Engineer");
						break;
					}
					else if (strcmp(Commands->Get_Preset_Name(soldier),"TCW_GDI_Spawn_Low") == 0)
					{
						Change_Character(soldier,"TCW_GDI_Engineer_Low");
						break;
					}
				}
				else
				{
					break;
				}
			}
		}
	}
}

void tcw_Spikemap_Get_CaptureGuy::Created(GameObject *obj)
{
	// This script is for buying a Engineer or Saboteur for free on maps that have only spikes
	// It should be sticked on some pokable object
	allowpoke = true; Commands->Enable_HUD_Pokable_Indicator(obj, true);
	type = Get_Int_Parameter("Type");
}
void tcw_Spikemap_Get_CaptureGuy::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (type == 0)
		{
			if (!Is_Building_Dead(Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY)))
			{
				if (Commands->Get_Player_Type(poker) == type)
				{
					if (Get_Object_Count(0,"TCW_Nod_Saboteur") < 1)
					{
						Change_Character(poker,"TCW_Nod_Saboteur");
					}
				}
			}
		}
		else if (type == 1)
		{
			if (!Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY)))
			{
				if (Commands->Get_Player_Type(poker) == type)
				{
					if (Get_Object_Count(1,"TCW_GDI_Engineer") < 1)
					{
						Change_Character(poker,"TCW_GDI_Engineer");
					}
				}
			}
		}
		allowpoke = false; Commands->Enable_HUD_Pokable_Indicator(obj, false); Commands->Start_Timer(obj,this,5.0f,100100);
	}
}
void tcw_Spikemap_Get_CaptureGuy::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100100)
	{
		allowpoke = true; Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}
void tcw_Spikemap_Get_CaptureGuy::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowpoke,sizeof(allowpoke),1);
	Auto_Save_Variable(&type,sizeof(type),1);
}

void tcw_Death_Blowup_Building::Killed(GameObject *obj, GameObject *shooter)
{
	int type = Get_Int_Parameter("Type");
	const char *building = Get_Parameter("Building_Name");
	Commands->Apply_Damage(Find_Building_By_Preset(type,building),99999.0f,"C4",shooter);
}

void tcw_zone_teleport_random_id::Created(GameObject *obj)
{
	targetid1 = Get_Int_Parameter("ID1");
	targetid2 = Get_Int_Parameter("ID2");
	targetid3 = Get_Int_Parameter("ID3");
	tele = true;
}
void tcw_zone_teleport_random_id::Entered(GameObject *obj, GameObject *enter)
{
	if (tele)
	{
		if (!Get_Vehicle(enter))
		{
			tele = false; Commands->Start_Timer(obj,this,3.0f,100);
			int random = Commands->Get_Random_Int(1,3);
			if (random == 1)
			{
				GameObject *gotoObject;
				gotoObject = Commands->Find_Object(targetid1);
				Vector3 gotoLoc = Commands->Get_Position(gotoObject);
				Commands->Set_Position(enter,gotoLoc);
			}
			else if (random == 2)
			{
				GameObject *gotoObject;
				gotoObject = Commands->Find_Object(targetid2);
				Vector3 gotoLoc = Commands->Get_Position(gotoObject);
				Commands->Set_Position(enter,gotoLoc);
			}
			else if (random == 3)
			{
				GameObject *gotoObject;
				gotoObject = Commands->Find_Object(targetid3);
				Vector3 gotoLoc = Commands->Get_Position(gotoObject);
				Commands->Set_Position(enter,gotoLoc);
			}
		}
	}
}
void tcw_zone_teleport_random_id::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		tele = true;
	}
}
void tcw_zone_teleport_random_id::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&targetid1,sizeof(targetid1),1);
	Auto_Save_Variable(&targetid2,sizeof(targetid1),2);
	Auto_Save_Variable(&targetid3,sizeof(targetid1),3);
	Auto_Save_Variable(&tele,sizeof(tele),4);
}

void tcw_dm_map_tech3::Created(GameObject *obj)//no limits
{
	//no limitations on map
}
void tcw_dm_map_tech2::Created(GameObject *obj)//no commando's
{
	Hide_Preset_By_Name(1,"TCW_GDI_Commando",true);
	Hide_Preset_By_Name(1,"TCW_GDI_Commando_Low",true);
	Hide_Preset_By_Name(0,"TCW_Nod_Commando",true);
	Hide_Preset_By_Name(0,"TCW_Nod_Commando_Low",true);
}
void tcw_dm_map_tech1::Created(GameObject *obj)//no snipers, shadows and commando's
{
	Hide_Preset_By_Name(1,"TCW_GDI_Commando",true);
	Hide_Preset_By_Name(1,"TCW_GDI_Commando_Low",true);
	Hide_Preset_By_Name(1,"TCW_GDI_Sniper",true);
	Hide_Preset_By_Name(1,"TCW_GDI_Sniper_Low",true);
	Hide_Preset_By_Name(0,"TCW_Nod_Commando",true);
	Hide_Preset_By_Name(0,"TCW_Nod_Commando_Low",true);
	Hide_Preset_By_Name(0,"TCW_Nod_Shadow",true);
	Hide_Preset_By_Name(0,"TCW_Nod_Shadow_Low",true);
}

void tcw_destroyed_send_custom::Destroyed(GameObject *obj)
{
	int msg = Get_Int_Parameter("Message");		int id = Get_Int_Parameter("SendID");	int prm = Get_Int_Parameter("Param");
	Commands->Send_Custom_Event(obj,Commands->Find_Object(id),msg,prm,0);
}

void tcw_Spawner_Enable_Timer::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Enable_After_Seconds"),1001);
}
void tcw_Spawner_Enable_Timer::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1001)
	{
		Commands->Enable_Spawner(Get_Int_Parameter("SpawnerID"),true);
	}
}

void tcw_snipe_only_building::Created(GameObject *obj)
{
	Set_Damage_Points(obj,0.0f);
	Set_Skin(obj,"Blamo");
}

ScriptRegistrant<tcw_snipe_only_building> tcw_snipe_only_building_Registrant("tcw_snipe_only_building","");
ScriptRegistrant<tcw_Spawner_Enable_Timer> tcw_Spawner_Enable_Timer_Registrant("tcw_Spawner_Enable_Timer","Enable_After_Seconds=1000:float,SpawnerID=0:int");
ScriptRegistrant<tcw_destroyed_send_custom> tcw_destroyed_send_custom_Registrant("tcw_destroyed_send_custom","Message=1000:int,SendID=1000:int,Param=1:int");
ScriptRegistrant<tcw_dm_map_tech1> tcw_dm_map_tech1_Registrant("tcw_dm_map_tech1","");
ScriptRegistrant<tcw_dm_map_tech2> tcw_dm_map_tech2_Registrant("tcw_dm_map_tech2","");
ScriptRegistrant<tcw_dm_map_tech3> tcw_dm_map_tech3_Registrant("tcw_dm_map_tech3","");
ScriptRegistrant<tcw_zone_teleport_random_id> tcw_zone_teleport_random_id_Registrant("tcw_zone_teleport_random_id","ID1=1:int,ID2=2:int,ID3=3:int");
ScriptRegistrant<tcw_Death_Blowup_Building> tcw_Death_Blowup_Building_Registrant("tcw_Death_Blowup_Building","Type=0:int,Building_Name=TCW:string");
ScriptRegistrant<tcw_Spikemap_Get_CaptureGuy> tcw_Spikemap_Get_CaptureGuy_Registrant("tcw_Spikemap_Get_CaptureGuy","Type=0:int");
ScriptRegistrant<tcw_There_Must_Be_Engineer> tcw_There_Must_Be_Engineer_Registrant("tcw_There_Must_Be_Engineer","");
ScriptRegistrant<tcw_Map_Intro> tcw_Map_Intro_Registrant("tcw_Map_Intro","Red=1:int,Green=1:int,Blue=1:int,Intro1=Line1:string,Intro2=Line2:string,Intro3=Line3:string,Intro4=Line4:string,Intro5=Line5:string");
ScriptRegistrant<tcw_Custom_Win_Game_Credits> tcw_Custom_Win_Game_Credits_Registrant("tcw_Custom_Win_Game_Credits","");
ScriptRegistrant<tcw_Credit_Trickle> tcw_Credit_Trickle_Registrant("tcw_Credit_Trickle","Credits=1:int,Delay=2.0:float");
