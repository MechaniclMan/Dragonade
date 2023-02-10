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
#include "tcw_nod_incursion.h"
#include "GameObjManager.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"

void tcw_i_have_ion_beacon_incursion::Destroyed(GameObject *obj)
{
	Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_pt_ion_incursion"),1000,1,0);//someone has ion is false
}

void tcw_pt_ion_incursion::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	allowpoke = true;
	ionready = false;
	Create_2D_WAV_Sound_Team("gdi_ion_cannon_charging.wav",0);
	Send_Message_Team(0,255,255,255,"Ion Cannon charging");
	Commands->Start_Timer(obj,this,900.0f,100100);
}
void tcw_pt_ion_incursion::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 2)//successful beacon placed, start recharging
		{
			ionready = false;
			Commands->Start_Timer(obj,this,900.0f,100100);//recharge the ion
		}
	}
}
void tcw_pt_ion_incursion::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100100)
	{
		ionready = true;
		Create_2D_WAV_Sound_Team("gdi_IonCannReady.wav",0);
		Send_Message_Team(0,255,255,255,"Ion Cannon ready");	
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_gdi_ion_cannon_building_incursion"),1000,1,0);
	}
	else if (number == 100101)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}
void tcw_pt_ion_incursion::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if ( (Commands->Get_Player_Type(poker) == 0) && (strcmp(Commands->Get_Preset_Name(poker),"TCW_GDI_Engineer") == 0) )
		{
			if ( (strcmp(Commands->Get_Preset_Name(poker),"TCW_Nod_Commando") == 0) || (strcmp(Commands->Get_Preset_Name(poker),"TCW_Nod_Commando_Low") == 0) )
			{
				Send_Message_Player(poker,255,255,255,"Commando's are not allowed to carry the Ion Cannon Beacon");
			}
			else
			{
				if (Is_Building_Dead(Find_Building_By_Preset(1,"TCW_GDI_Ion_Center")))
				{
					Send_Message_Player(poker,255,255,255,"Your Ion Cannon Center is destroyed. The Ion Cannon is unavailable.");
				}
				else
				{
					allowpoke = false;
					Commands->Enable_HUD_Pokable_Indicator(obj, false);
					Commands->Start_Timer(obj,this,5.0f,100101);//alowpoke true
					if (Find_Object_With_Script("tcw_i_have_ion_beacon_incursion"))//someone has ion already
					{
						char currholdermsg[250];
						sprintf(currholdermsg,"Sorry, %s currently has the Ion Cannon Beacon.",Get_Player_Name(Find_Object_With_Script("tcw_i_have_ion_beacon")));
						Send_Message_Player(poker,255,255,255,currholdermsg);
					}
					else
					{
						if (ionready)
						{
							Attach_Script_Once(poker,"tcw_i_have_ion_beacon_incursion","");
							Commands->Give_PowerUp(poker,"TCW_POW_Beacon_IonCannon_60Sec",false);
							char pickupmsg[250];
							sprintf(pickupmsg,"%s picked up the Ion Cannon Beacon",Get_Player_Name(poker));
							Send_Message_Team(0,255,204,0,pickupmsg);
							Send_Message_Player(poker,255,0,0,"Select Target");
							Create_2D_WAV_Sound_Player(poker,"gdi_selecttarget.wav");
						}
						else
						{
							char notreadymsg[250];
							sprintf(notreadymsg,"Sorry %s, the Ion Cannon is still charging up.",Get_Player_Name(poker));
							Send_Message_Player(poker,255,255,255,notreadymsg);
						}
					}
				}
			}
		}
		else
		{
			char errorwrongchar[250];
			sprintf(errorwrongchar, "Sorry %s, You can only pick up the GDI Ion cannon beacon with a GDI Engineer!", Get_Player_Name(poker));
			Send_Message_Player(poker,255,255,255,errorwrongchar);
		}
	}
}

void tcw_gdi_ion_cannon_building_incursion::Created(GameObject *obj)
{
	killable = false;
}
void tcw_gdi_ion_cannon_building_incursion::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 0)
		{
			Commands->Set_Animation(obj,"V_GDI_IONCTR.V_GDI_IONCTR",false,0,40,90,false);//go down
		}
		else if (param == 1)
		{
			Commands->Set_Animation(obj,"V_GDI_IONCTR.V_GDI_IONCTR",false,0,0,40,false);//go up
			killable = true;
		}
	}
}
void tcw_gdi_ion_cannon_building_incursion::Killed(GameObject *obj, GameObject *shooter)
{
	if (!killable) 
	{
		Console_Input("amsg The Ion Cannon Control center was destroyed before ion strike was possible! Mission is a failure!");
		Console_Input("win 1");
	}
	else
	{
		int carrier = Commands->Get_ID(Find_Object_With_Script("tcw_i_have_ion_beacon_incursion"));
		if (carrier)
		{
			Commands->Select_Weapon(Commands->Find_Object(carrier),"TCW_Weapon_Knife");
			Remove_Weapon(Commands->Find_Object(carrier),"TCW_Weapon_IonCannon_60Sec");
		}
		Create_2D_WAV_Sound_Team("gdi_cancelled.wav",1);
		Send_Message_Team(0,255,255,255,"Ion Cannon Cancelled");
	}
}

void tcw_inc_steal_tech_zone_gbar::Entered(GameObject *obj, GameObject *enter)
{
	if (!Is_Building_Dead(Find_Building_By_Preset(1,"TCW_GDI_Barracks")))
	{
		if ( (Commands->Is_A_Star(enter)) && (Commands->Get_Player_Type(enter) == 0) )
		{
			if ( (strcmp(Commands->Get_Preset_Name(enter),"TCW_GDI_Engineer") == 0) || (strcmp(Commands->Get_Preset_Name(enter),"TCW_GDI_Engineer_Low") == 0) || 
				(strcmp(Commands->Get_Preset_Name(enter),"TCW_Nod_Saboteur") == 0) || (strcmp(Commands->Get_Preset_Name(enter),"TCW_Nod_Saboteur_Low") == 0) )
			{
				if (!Get_Vehicle(enter))
				{
					Attach_Script_Once(enter,"tcw_inc_i_am_stealing_tech","4");
				}
			}
		}
	}
}
void tcw_inc_steal_tech_zone_gbar::Exited(GameObject *obj, GameObject *exit)
{
	if ( (Commands->Is_A_Star(exit)) && (Commands->Get_Player_Type(exit) == 0) )
	{
		if ( (strcmp(Commands->Get_Preset_Name(exit),"TCW_GDI_Engineer") == 0) || (strcmp(Commands->Get_Preset_Name(exit),"TCW_GDI_Engineer_Low") == 0) || 
			(strcmp(Commands->Get_Preset_Name(exit),"TCW_Nod_Saboteur") == 0) || (strcmp(Commands->Get_Preset_Name(exit),"TCW_Nod_Saboteur_Low") == 0) )
		{
			Remove_Script(exit,"tcw_i_am_stealing_tech");
		}
	}
}

void tcw_inc_i_am_stealing_tech::Created(GameObject *obj)
{
	int building = Get_Int_Parameter("Building");
	if (building == 1)//nod stealing gdi wf
	{
		Commands->Start_Timer(obj,this,15.0f,779901);//start stealing from the gdi wf
		char stealermsg[250];
		sprintf(stealermsg,"%s is stealing technology from the GDI War Factory.",Get_Player_Name(obj));
		Send_Message_Team(0,255,0,0,stealermsg);
		Create_2D_WAV_Sound_Team("building_capture.wav",0);
	}
	else if (building == 4)//nod stealing gdi barracks
	{
		Commands->Start_Timer(obj,this,15.0f,779904);//start stealing from the gdi barracks
		char stealermsg[250];
		sprintf(stealermsg,"%s is stealing technology from the GDI Barracks.",Get_Player_Name(obj));
		Send_Message_Team(0,255,0,0,stealermsg);
		Create_2D_WAV_Sound_Team("building_capture.wav",0);
	}
}
void tcw_inc_i_am_stealing_tech::Timer_Expired(GameObject *obj, int number)
{
	if (number == 779901)//nod stole tech from gdi wf
	{
		if (!Is_Building_Dead(Find_Building_By_Preset(1,"TCW_GDI_War_Factory")))
		{
			if (Get_Total_Bullets(obj,"TCW_Weapon_Repairgun_Advanced") == 5)//full repairgun = successful stealing
			{
				Commands->Select_Weapon(obj,"TCW_Weapon_Pistol");
				Remove_Weapon(obj,"TCW_Weapon_Repairgun_Advanced");

				char stealmsg[250];
				sprintf(stealmsg,"%s has stolen technology from the GDI War Factory",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,stealmsg);
				Send_Message_Team(0,255,0,0,"New technology acquired.");
				Create_2D_WAV_Sound_Team("nod_new_tech_acquired.wav",0);
				Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_inc_war_factory"),12345,1,0);
			}
			else
			{
				Send_Message_Player(obj,255,0,0,"To steal technology you need a FULL AMMO Repair Gun.");
				Destroy_Script();
			}
		}
	}
	else if (number == 779904)//nod stole tech from gdi barracks
	{
		if (!Is_Building_Dead(Find_Building_By_Preset(1,"TCW_GDI_Barracks")))
		{
			if (Get_Total_Bullets(obj,"TCW_Weapon_Repairgun_Advanced") == 5)//full repairgun = successful stealing
			{
				Commands->Select_Weapon(obj,"TCW_Weapon_Pistol");
				Remove_Weapon(obj,"TCW_Weapon_Repairgun_Advanced");
					
				Hide_Preset_By_Name(0,"TCW_GDI_Rifle_Squad",false);
				Hide_Preset_By_Name(0,"TCW_GDI_Engineer",false);
				Hide_Preset_By_Name(0,"TCW_GDI_Missile_Squad",false);
				Hide_Preset_By_Name(0,"TCW_GDI_Grenadier",false);
				Hide_Preset_By_Name(0,"TCW_GDI_Sniper",false);

				char stealmsg[512];
				sprintf(stealmsg,"%s has stolen technology from the GDI Barracks",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,stealmsg);
				Send_Message_Team(0,255,0,0,"New technology acquired.");
				Create_2D_WAV_Sound_Team("nod_new_tech_acquired.wav",0);

				Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_inc_barracks"),12345,1,0);
			}
			else
			{
				Send_Message_Player(obj,255,0,0,"To steal technology you need a FULL AMMO Repair Gun.");
				Destroy_Script();
			}
		}
	}
}

void tcw_inc_barracks::Created(GameObject *obj)
{
	AllowKill = false;
}
void tcw_inc_barracks::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 12345)
	{
		if (param == 1)
		{
			AllowKill = true;
			char stealmsg[512];
			sprintf(stealmsg,"Good job! Now finish off the barracks to stop advanced reinforcements from pushing trough!");
			Send_Message_Team(0,255,255,255,stealmsg);
		}
	}
}
void tcw_inc_barracks::Killed(GameObject *obj, GameObject *shooter)
{
	if (!AllowKill)
	{
		if (Commands->Is_A_Star(shooter))
		{
			char lolmsg[512]; 
			sprintf(lolmsg,"amsg %s Failed the Mission by prematurely killing the Barracks.",Get_Player_Name(shooter));
			Console_Input(lolmsg);
			Console_Input("win 1");
		}
		else
		{
			Console_Input("amsg Mission Failed: The Barracks was killed before technology was stolen.");
			Console_Input("win 1");
		}
	}
	else
	{
		Commands->Enable_Spawner(1234,false);//disable all advanced ifnantry spawners
	}
}
void tcw_inc_war_factory::Created(GameObject *obj)
{
		AllowKill = false;
}
void tcw_inc_war_factory::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 12345)
	{
		if (param == 1)
		{
			AllowKill = true;
			char stealmsg[512];
			GameObject *controller = Commands->Create_Object("Invisible_Object",Vector3(0.0f,0.0f,0.0f));
				// Drop 1
			Vector3 pos = Commands->Get_Position(Find_Object_With_Script("tcw_inc_vehicle_pos"));
			GameObject *drop1 = Commands->Create_Object("Invisible_Object", pos);
			Commands->Attach_Script(drop1,"Test_Cinematic","tcw_inc_mammy.txt");
			Commands->Set_Facing(drop1, 45.0f);
			Commands->Attach_Script(controller, "tcw_inc_veh_drop2", "");
			Commands->Attach_Script(controller, "tcw_inc_veh_drop3", "");
			Commands->Attach_Script(controller, "tcw_inc_veh_drop4", "");
			Commands->Attach_Script(controller, "tcw_inc_bonus_complete", "");
			Send_Message_Team(0,255,255,255,stealmsg);
			
		}
	}
}
void tcw_inc_war_factory::Killed(GameObject *obj, GameObject *shooter)
{
	if (!AllowKill) 
	{
			char stealmsg[512];
			GameObject *controller = Commands->Create_Object("Invisible_Object",Vector3(0.0f,0.0f,0.0f));
			Commands->Attach_Script(controller, "tcw_inc_bonus_failed", "");
			Send_Message_Team(0,255,255,255,stealmsg);
			char stealmsg1[512];
			sprintf(stealmsg,"GDI Vehicle reinforcements stopped.");
			Send_Message_Team(0,255,255,255,stealmsg1);
	}
	else {
			char stealmsg[512];
			sprintf(stealmsg,"GDI Vehicle reinforcements stopped.");
			Send_Message_Team(0,255,255,255,stealmsg);
	}
}
void tcw_inc_vehicle_pos::Created(GameObject *obj)
{
	//
}
void tcw_inc_vehicle_pos2::Created(GameObject *obj)
{
	//
}
void tcw_inc_vehicle_pos3::Created(GameObject *obj)
{
	//
}
void tcw_inc_vehicle_pos4::Created(GameObject *obj)
{
	//
}

void tcw_inc_steal_tech_zone_gwf::Entered(GameObject *obj, GameObject *enter)
{
	if (!Is_Building_Dead(Find_Building_By_Preset(1,"TCW_GDI_War_Factory")))
	{
		if ( (Commands->Is_A_Star(enter)) && (Commands->Get_Player_Type(enter) == 0) )
		{
			if ( (strcmp(Commands->Get_Preset_Name(enter),"TCW_GDI_Engineer") == 0) || (strcmp(Commands->Get_Preset_Name(enter),"TCW_GDI_Engineer_Low") == 0) || 
				(strcmp(Commands->Get_Preset_Name(enter),"TCW_Nod_Saboteur") == 0) || (strcmp(Commands->Get_Preset_Name(enter),"TCW_Nod_Saboteur_Low") == 0) )
			{
				if (!Get_Vehicle(enter))
				{
					Attach_Script_Once(enter,"tcw_inc_i_am_stealing_tech","1");
				}
			}
		}
	}
}
void tcw_inc_steal_tech_zone_gwf::Exited(GameObject *obj, GameObject *exit)
{
	if ( (Commands->Is_A_Star(exit)) && (Commands->Get_Player_Type(exit) == 0) )
	{
		if ( (strcmp(Commands->Get_Preset_Name(exit),"TCW_GDI_Engineer") == 0) || (strcmp(Commands->Get_Preset_Name(exit),"TCW_GDI_Engineer_Low") == 0) || 
			(strcmp(Commands->Get_Preset_Name(exit),"TCW_Nod_Saboteur") == 0) || (strcmp(Commands->Get_Preset_Name(exit),"TCW_Nod_Saboteur_Low") == 0) )
		{
			Remove_Script(exit,"tcw_inc_i_am_stealing_tech");
		}
	}
}

void tcw_inc_veh_drop1::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 1.0f, 1);
}
void tcw_inc_veh_drop1::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1) {
	// Drop 1
		Vector3 pos = Commands->Get_Position(Find_Object_With_Script("tcw_inc_vehicle_pos"));
		GameObject *drop1 = Commands->Create_Object("Invisible_Object", pos);
		Commands->Attach_Script(drop1,"Test_Cinematic","tcw_inc_mammy.txt");
		Commands->Set_Facing(drop1, 45.0f);
	}
}
void tcw_inc_veh_drop2::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 2.0f, 1);
}
void tcw_inc_veh_drop2::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1) {
		// Drop 2
		Vector3 pos2 = Commands->Get_Position(Find_Object_With_Script("tcw_inc_vehicle_pos2"));
		GameObject *drop2 = Commands->Create_Object("Invisible_Object", pos2);
		Commands->Attach_Script(drop2,"Test_Cinematic","tcw_inc_pred.txt");
		Commands->Set_Facing(drop2, 90.0f);
	}
}
void tcw_inc_veh_drop3::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 3.0f, 1);
}
void tcw_inc_veh_drop3::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1) {
		// Drop 3
		Vector3 pos3 = Commands->Get_Position(Find_Object_With_Script("tcw_inc_vehicle_pos3"));
		GameObject *drop3 = Commands->Create_Object("Invisible_Object", pos3);
		Commands->Attach_Script(drop3,"Test_Cinematic","tcw_inc_apc.txt");
		Commands->Set_Facing(drop3, 180.0f);
	}
}
void tcw_inc_veh_drop4::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 4.0f, 1);
}
void tcw_inc_veh_drop4::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1) {
		// Drop 4
		Vector3 pos4 = Commands->Get_Position(Find_Object_With_Script("tcw_inc_vehicle_pos4"));
		GameObject *drop4 = Commands->Create_Object("Invisible_Object", pos4);
		Commands->Attach_Script(drop4,"Test_Cinematic","tcw_inc_pitbull.txt");
		Commands->Set_Facing(drop4, 280.0f);
	}
}
void tcw_inc_bonus_complete::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 5.0f, 1);
}
void tcw_inc_bonus_complete::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1) {
		Console_Input("cmsg public Bonus objective complete! Nod Reinforcements are inbound.");
		Console_Input("snda tcw_inc_bonus_complete.mp3");
	}
}
void tcw_inc_bonus_failed::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 5.0f, 1);
}
void tcw_inc_bonus_failed::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1) {
		Console_Input("cmsg public Bonus objective failed! Nod Reinforcements halted.");
		Console_Input("snda tcw_inc_bonus_failed.mp3");
	}
}
ScriptRegistrant<tcw_inc_bonus_failed> tcw_inc_bonus_failed_Registrant("tcw_inc_bonus_failed","");
ScriptRegistrant<tcw_inc_bonus_complete> tcw_inc_bonus_complete_Registrant("tcw_inc_bonus_complete","");
ScriptRegistrant<tcw_inc_veh_drop1> tcw_inc_veh_drop1_Registrant("tcw_inc_veh_drop1","");
ScriptRegistrant<tcw_inc_veh_drop2> tcw_inc_veh_drop2_Registrant("tcw_inc_veh_drop2","");
ScriptRegistrant<tcw_inc_veh_drop3> tcw_inc_veh_drop3_Registrant("tcw_inc_veh_drop3","");
ScriptRegistrant<tcw_inc_veh_drop4> tcw_inc_veh_drop4_Registrant("tcw_inc_veh_drop4","");
ScriptRegistrant<tcw_inc_steal_tech_zone_gwf> tcw_inc_steal_tech_zone_gwf_Registrant("tcw_inc_steal_tech_zone_gwf","");
ScriptRegistrant<tcw_inc_vehicle_pos> tcw_inc_vehicle_pos_Registrant("tcw_inc_vehicle_pos","");
ScriptRegistrant<tcw_inc_vehicle_pos2> tcw_inc_vehicle_pos2_Registrant("tcw_inc_vehicle_pos2","");
ScriptRegistrant<tcw_inc_vehicle_pos3> tcw_inc_vehicle_pos3_Registrant("tcw_inc_vehicle_pos3","");
ScriptRegistrant<tcw_inc_vehicle_pos4> tcw_inc_vehicle_pos4_Registrant("tcw_inc_vehicle_pos4","");
ScriptRegistrant<tcw_inc_war_factory> tcw_inc_war_factory_Registrant("tcw_inc_war_factory","");
ScriptRegistrant<tcw_inc_barracks> tcw_inc_barracks_Registrant("tcw_inc_barracks","");
ScriptRegistrant<tcw_inc_i_am_stealing_tech> tcw_inc_i_am_stealing_tech_Registrant("tcw_inc_i_am_stealing_tech","Building=1:int");
ScriptRegistrant<tcw_inc_steal_tech_zone_gbar> tcw_inc_steal_tech_zone_gbar_Registrant("tcw_inc_steal_tech_zone_gbar","");
ScriptRegistrant<tcw_gdi_ion_cannon_building_incursion> tcw_gdi_ion_cannon_building_incursion_Registrant("tcw_gdi_ion_cannon_building_incursion","");
ScriptRegistrant<tcw_pt_ion_incursion> tcw_pt_ion_incursion_Registrant("tcw_pt_ion_incursion","");
ScriptRegistrant<tcw_i_have_ion_beacon_incursion> tcw_i_have_ion_beacon_incursion_Registrant("tcw_i_have_ion_beacon_incursion","");
