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
#include "tcw_raid_at_dawn.h"
#include "tcw_general.h"
#include "tcw_functions.h"
#include "VehicleGameObj.h"
#include "engine.h"
#include "GameObjManager.h"
#include "BuildingGameObj.h"

void tcw_raid_steal_tech_zone_nwf::Entered(GameObject *obj, GameObject *enter)
{
	if (!Is_Building_Dead(Find_Building_By_Preset(0,"TCW_Nod_War_Factory")))
	{
		if ( (Commands->Is_A_Star(enter)) && (Commands->Get_Player_Type(enter) == 1) )
		{
			if ( (strcmp(Commands->Get_Preset_Name(enter),"TCW_GDI_Engineer") == 0) || (strcmp(Commands->Get_Preset_Name(enter),"TCW_GDI_Engineer_Low") == 0) || 
				(strcmp(Commands->Get_Preset_Name(enter),"TCW_Nod_Saboteur") == 0) || (strcmp(Commands->Get_Preset_Name(enter),"TCW_Nod_Saboteur_Low") == 0) )
			{
				if (!Get_Vehicle(enter))
				{
					Attach_Script_Once(enter,"tcw_raid_i_am_stealing_tech","1");
				}
			}
		}
	}
}
void tcw_raid_steal_tech_zone_nwf::Exited(GameObject *obj, GameObject *exit)
{
	if ( (Commands->Is_A_Star(exit)) && (Commands->Get_Player_Type(exit) == 1) )
	{
		if ( (strcmp(Commands->Get_Preset_Name(exit),"TCW_GDI_Engineer") == 0) || (strcmp(Commands->Get_Preset_Name(exit),"TCW_GDI_Engineer_Low") == 0) || 
			(strcmp(Commands->Get_Preset_Name(exit),"TCW_Nod_Saboteur") == 0) || (strcmp(Commands->Get_Preset_Name(exit),"TCW_Nod_Saboteur_Low") == 0) )
		{
			Remove_Script(exit,"tcw_raid_i_am_stealing_tech");
		}
	}
}


void tcw_raid_i_am_stealing_tech::Created(GameObject *obj)
{
	int building = Get_Int_Parameter("Building");
	if (building == 1)//gdi stealing nod wf
	{
		Commands->Start_Timer(obj,this,15.0f,155156);//start stealing from the nod wf
		char stealermsg[250];
		sprintf(stealermsg,"%s is stealing technology from the Nod Weapons Factory.",Get_Player_Name(obj));
		Send_Message_Team(1,255,204,0,stealermsg);
		Create_2D_WAV_Sound_Team("building_capture.wav",1);
	}
	else if (building == 2)//gdi stealing nod hand of nod
	{
		Commands->Start_Timer(obj,this,15.0f,155155);//start stealing from the nod hand of nod
		char stealermsg[250];
		sprintf(stealermsg,"%s is stealing technology from the Hand of Nod.",Get_Player_Name(obj));
		Send_Message_Team(1,255,204,0,stealermsg);
		Create_2D_WAV_Sound_Team("building_capture.wav",1);
	}
}
void tcw_raid_i_am_stealing_tech::Timer_Expired(GameObject *obj, int number)
{
	if (number == 155155)//gdi stole tech from nod hand
	{
		if (!Is_Building_Dead(Find_Building_By_Preset(0,"TCW_Hand_Of_Nod")))
		{
			if (Get_Total_Bullets(obj,"TCW_Weapon_Repairgun_Advanced") == 5)//full repairgun = successful stealing
			{
				Commands->Select_Weapon(obj,"TCW_Weapon_Pistol");
				Remove_Weapon(obj,"TCW_Weapon_Repairgun_Advanced");
					
				Hide_Preset_By_Name(1,"TCW_Nod_Militant_Infantry",false);
				Hide_Preset_By_Name(1,"TCW_Nod_Saboteur",false);
				Hide_Preset_By_Name(1,"TCW_Nod_RocketGuy",false);
				Hide_Preset_By_Name(1,"TCW_Nod_Shadow",false);
				Hide_Preset_By_Name(1,"TCW_Nod_Black_Hand_Flame",false);
				Hide_Preset_By_Name(1,"TCW_Nod_Commando",false);

				char stealmsg[250];
				sprintf(stealmsg,"%s has stolen technology from the Hand of Nod",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,stealmsg);
				Send_Message_Team(1,255,204,0,"New technology acquired.");
				Create_2D_WAV_Sound_Team("gdi_new_tech_acquired.wav",1);
			}
			else
			{
				Send_Message_Player(obj,255,204,0,"To steal technology you need a FULL AMMO Repair Gun.");
				Destroy_Script();
			}
		}
	}
	else if (number == 155156)//gdi stole tech from nod wf
	{
		if (!Is_Building_Dead(Find_Building_By_Preset(0,"TCW_Nod_War_Factory")))
		{
			if (Get_Total_Bullets(obj,"TCW_Weapon_Repairgun_Advanced") == 5)//full repairgun = successful stealing
			{
				Commands->Select_Weapon(obj,"TCW_Weapon_Pistol");
				Remove_Weapon(obj,"TCW_Weapon_Repairgun_Advanced");
					
				Hide_Preset_By_Name(1,"TCW_GDI_Attack_Bike",false);
				Hide_Preset_By_Name(1,"TCW_GDI_Flame_Tank",false);
				Hide_Preset_By_Name(1,"TCW_GDI_Nod_Harvester_Player",false);
				Hide_Preset_By_Name(1,"TCW_GDI_Raider_Buggy",false);
				Hide_Preset_By_Name(1,"TCW_GDI_Scorpion_Tank",false);
				Hide_Preset_By_Name(1,"TCW_GDI_Stealth_Tank",false);

				char stealmsg[250];
				sprintf(stealmsg,"%s has stolen technology from the Nod Weapons Factory",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,stealmsg);
				Send_Message_Team(1,255,204,0,"New technology acquired.");
				Create_2D_WAV_Sound_Team("gdi_new_tech_acquired.wav",1);
				Commands->Start_Timer(obj,this,3.0f,7070);//primary objective complete
				GameObject *nwf = Find_Building_By_Type(0,BuildingConstants::TYPE_VEHICLE_FACTORY);
				if (nwf)
				{
					Remove_Script(nwf,"tcw_raid_missionfail");
					nwf = 0;
				}
			}
			else
			{
				Send_Message_Player(obj,255,204,0,"To steal technology you need a FULL AMMO Repair Gun.");
				Destroy_Script();
			}
		}
	}
	else if (number == 7070)
	{
		Send_Message_Team(1,255,204,0,"Well done Commanders. Now proceed and eliminate the remaining Nod resistance.");
		Destroy_Script();
	}
}

void tcw_raid_missionfail::Killed(GameObject *obj, GameObject *shooter)
{
	Console_Input("amsg Mission Failed: Technology was not stolen from the Nod Weapons Factory");
	Console_Input("win 0");
}

ScriptRegistrant<tcw_raid_missionfail> tcw_raid_missionfail_Registrant("tcw_raid_missionfail","");
ScriptRegistrant<tcw_raid_i_am_stealing_tech> tcw_raid_i_am_stealing_tech_Registrant("tcw_raid_i_am_stealing_tech","Building=1:int");
ScriptRegistrant<tcw_raid_steal_tech_zone_nwf> tcw_raid_steal_tech_zone_nwf_Registrant("tcw_raid_steal_tech_zone_nwf","");