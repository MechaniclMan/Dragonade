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
#include "buildinggameobj.h"
#include "tcw_building.h"

void tcw_building::Created(GameObject *obj)
{
	alive = true;
	playdamage = true;
	if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Command_Post") == 0)
	{
		Commands->Set_Animation(obj,"V_GDI_CPOST.V_GDI_CPOST",true,0,1,-1,0);
		Commands->Start_Timer(obj,this,300.0f,111222);//radar scan
	}
}
void tcw_building::Killed(GameObject *obj, GameObject *shooter)
{
	alive = false;
	if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Barracks") == 0)
	{
		Create_2D_WAV_Sound_Team("gdi_barracks_destroyed_gdi.wav",1);
		Create_2D_WAV_Sound_Team("gdi_barracks_destroyed_nod.wav",0);
		Send_Message(255,255,255,"GDI Barracks destroyed.");
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Airfield") == 0)
	{
		Create_2D_WAV_Sound_Team("gdi_airfield_destroyed_gdi.wav",1);
		Create_2D_WAV_Sound_Team("gdi_airfield_destroyed_nod.wav",0);
		Send_Message(255,255,255,"GDI Airfield destroyed.");
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Watchtower") == 0)
	{
		Create_2D_WAV_Sound_Team("gdi_watchtower_destroyed_gdi.wav",1);
		Create_2D_WAV_Sound_Team("gdi_watchtower_destroyed_nod.wav",0);
		Send_Message(255,255,255,"GDI Watch Tower destroyed.");
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_War_Factory") == 0)
	{
		Create_2D_WAV_Sound_Team("gdi_warfactory_destroyed_gdi.wav",1);
		Create_2D_WAV_Sound_Team("gdi_warfactory_destroyed_nod.wav",0);
		Send_Message(255,255,255,"GDI War Factory destroyed.");
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_War_Factory") == 0)
	{
		Create_2D_WAV_Sound_Team("nod_warfactory_destroyed_gdi.wav",1);
		Create_2D_WAV_Sound_Team("nod_warfactory_destroyed_nod.wav",0);
		Send_Message(255,255,255,"Nod War Factory destroyed.");
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_Hand_Of_Nod") == 0)
	{
		Create_2D_WAV_Sound_Team("nod_hand_of_nod_destroyed_gdi.wav",1);
		Create_2D_WAV_Sound_Team("nod_hand_of_nod_destroyed_nod.wav",0);
		Send_Message(255,255,255,"Hand Of Nod destroyed.");
	}
	else if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Refinery") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Refinery_Blue") == 0) )
	{
		Create_2D_WAV_Sound_Team("gdi_tiberium_refinery_destroyed.wav",1);
		Create_2D_WAV_Sound_Team("gdi_tiberium_refinery_destroyed_nod.wav",0);
		Send_Message(255,255,255,"GDI Tiberium Refinery destroyed.");
	}
	else if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Refinery") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Refinery_Blue") == 0) )
	{
		Create_2D_WAV_Sound_Team("nod_tiberium_refinery_destroyed_gdi.wav",1);
		Create_2D_WAV_Sound_Team("nod_tiberium_refinery_destroyed_nod.wav",0);
		Send_Message(255,255,255,"Nod Tiberium Refinery destroyed.");
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Power_Plant") == 0)
	{
		Create_2D_WAV_Sound_Team("nod_powerplant_destroyed_gdi.wav",1);
		Create_2D_WAV_Sound_Team("nod_powerplant_destroyed_nod.wav",0);
		Send_Message_Team(1,255,255,255,"Nod Power Plant destroyed.");
		Send_Message_Team(0,255,255,255,"Nod Power Plant destroyed.");
		if (Find_Building_By_Preset(0,"TCW_Nod_Oper_Centre"))
		{
			Enable_Base_Radar(0,false);
			Create_2D_WAV_Sound_Team("radar_off.wav",0);
		}
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Power_Plant") == 0)
	{
		Create_2D_WAV_Sound_Team("gdi_powerplant_destroyed_gdi.wav",1);
		Create_2D_WAV_Sound_Team("gdi_powerplant_destroyed_nod.wav",0);
		Send_Message_Team(1,255,255,255,"GDI Power Plant destroyed.");
		Send_Message_Team(0,255,255,255,"GDI Power Plant destroyed.");
		if (Find_Building_By_Preset(1,"TCW_GDI_Command_Post"))
		{
			Enable_Base_Radar(1,false);
			Create_2D_WAV_Sound_Team("radar_off.wav",1);
			Send_Message_Team(1,255,255,255,"Command Post Radar Scans cancelled");
		}
		Send_Message_Team(1,255,255,255,"Vehicle Factory Unit build times tripled");
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Airfield") == 0)
	{
		Create_2D_WAV_Sound_Team("nod_airfield_destroyed_gdi.wav",1);
		Create_2D_WAV_Sound_Team("nod_airfield_destroyed_nod.wav",0);
		Send_Message_Team(1,255,255,255,"Nod Airfield destroyed.");
		Send_Message_Team(0,255,255,255,"Nod Air Control Tower destroyed.");
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Command_Post") == 0)
	{
		Send_Message(255,255,255,"GDI Command Post destroyed.");
		Commands->Set_Animation(obj,"V_GDI_CPOST.V_GDI_CPOST",true,0,1,1,0);
		Enable_Base_Radar(1,false);
		Create_2D_WAV_Sound_Team("radar_off.wav",1);
		Create_2D_WAV_Sound_Team("gdi_command_post_destroyed_nod.wav",0);
		Create_2D_WAV_Sound_Team("gdi_command_post_destroyed_gdi.wav",1);
		Send_Message(255,255,255,"Command Post Radar Scans cancelled");
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_Nod_OpCtr") == 0)
	{
		Create_2D_WAV_Sound_Team("nod_operations_centre_destroyed_gdi.wav",1);
		Send_Message(255,255,255,"Nod Operations Centre destroyed.");
		Enable_Base_Radar(0,false);
		Create_2D_WAV_Sound_Team("radar_off.wav",0);
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_GDI_Armory") == 0)
	{
		Send_Message(255,255,255,"GDI Armory destroyed.");
		Create_2D_WAV_Sound_Team("gdi_armory_destroyed_gdi.wav",1);
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_Nod_Silo") == 0)
	{
		Create_2D_WAV_Sound_Team("nod_tiberium_silo_destroyed_nod.wav",0);
		Create_2D_WAV_Sound_Team("nod_tiberium_silo_destroyed_gdi.wav",1);
		Send_Message(255,255,255,"Nod Tiberium Silo destroyed.");
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_GDI_Silo") == 0)
	{
		Send_Message(255,255,255,"GDI Tiberium Silo destroyed.");
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_GDI_IonCtr") == 0)
	{
		Send_Message(255,255,255,"GDI Ion Cannon Control Center destroyed.");
		Create_2D_WAV_Sound_Team("gdi_ion_center_destroyed_gdi.wav",1);
		Create_2D_WAV_Sound_Team("gdi_ion_center_destroyed_nod.wav",0);
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_Nod_Tmpl") == 0)
	{
		Send_Message(255,255,255,"Temple of Nod destroyed.");
		Create_2D_WAV_Sound_Team("nod_temple_destroyed_nod.wav",0);
		Create_2D_WAV_Sound_Team("nod_temple_destroyed_gdi.wav",1);
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_Nod_Shrine") == 0)
	{
		Send_Message(255,255,255,"Shrine of Nod destroyed.");
	}
}
void tcw_building::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (alive)
	{
		if (playdamage)
		{
			if (damage > 0.5f)
			{
				playdamage = false;
				Commands->Start_Timer(obj,this,25.0f,100100);
				if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Refinery") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Refinery_Blue") == 0) )
				{
					Create_2D_WAV_Sound_Team("gdi_tiberium_refinery_under_attack_gdi.wav",1);
					Create_2D_WAV_Sound_Team("gdi_tiberium_refinery_under_attack_nod.wav",0);
					Send_Message_Team(0,255,255,255,"GDI Tiberium Refinery under attack.");
					Send_Message_Team(1,255,255,255,"Warning GDI Tiberium Refinery under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Watchtower") == 0)
				{
					Create_2D_WAV_Sound_Team("gdi_watchtower_under_attack_gdi.wav",1);
					Create_2D_WAV_Sound_Team("gdi_watchtower_under_attack_nod.wav",0);
					Send_Message_Team(1,255,255,255,"Warning: GDI Watch Tower under attack.");
					Send_Message_Team(0,255,255,255,"GDI Watch Tower under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Airfield") == 0)
				{
					Create_2D_WAV_Sound_Team("gdi_airfield_under_attack_gdi.wav",1);
					Create_2D_WAV_Sound_Team("gdi_airfield_under_attack_nod.wav",0);
					Send_Message_Team(0,255,255,255,"GDI Airfield under attack.");
					Send_Message_Team(1,255,255,255,"Warning GDI Airfield under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_War_Factory") == 0)
				{
					Create_2D_WAV_Sound_Team("gdi_warfactory_under_attack_gdi.wav",1);
					Create_2D_WAV_Sound_Team("gdi_warfactory_under_attack_nod.wav",0);
					Send_Message_Team(0,255,255,255,"GDI War Factory under attack.");
					Send_Message_Team(1,255,255,255,"Warning GDI War Factory under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_War_Factory") == 0)
				{
					Create_2D_WAV_Sound_Team("nod_warfactory_under_attack_gdi.wav",1);
					Create_2D_WAV_Sound_Team("nod_warfactory_under_attack_nod.wav",0);
					Send_Message_Team(0,255,255,255,"Warning Nod War Factory under attack.");
					Send_Message_Team(1,255,255,255,"Nod War Factory under attack.");
				}
				else if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Refinery") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Refinery_Blue") == 0) )
				{
					Create_2D_WAV_Sound_Team("nod_tiberium_refinery_under_attack_gdi.wav",1);
					Create_2D_WAV_Sound_Team("nod_tiberium_refinery_under_attack_nod.wav",0);
					Send_Message_Team(1,255,255,255,"Nod Tiberium Refinery under attack.");
					Send_Message_Team(0,255,255,255,"Warning Nod Tiberium Refinery under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Barracks") == 0)
				{
					Create_2D_WAV_Sound_Team("gdi_barracks_under_attack_gdi.wav",1);
					Create_2D_WAV_Sound_Team("gdi_barracks_under_attack_nod.wav",0);
					Send_Message_Team(1,255,255,255,"Warning GDI Barracks under attack.");
					Send_Message_Team(0,255,255,255,"GDI Barracks under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_Hand_Of_Nod") == 0)
				{
					Create_2D_WAV_Sound_Team("nod_hand_of_nod_under_attack_gdi.wav",1);
					Create_2D_WAV_Sound_Team("nod_hand_of_nod_under_attack_nod.wav",0);
					Send_Message_Team(0,255,255,255,"Warning Hand of Nod under attack.");
					Send_Message_Team(1,255,255,255,"Hand of Nod under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Power_Plant") == 0)
				{
					Create_2D_WAV_Sound_Team("nod_powerplant_under_attack_gdi.wav",1);
					Create_2D_WAV_Sound_Team("nod_powerplant_under_attack_nod.wav",0);
					Send_Message_Team(0,255,255,255,"Warning Nod Power Plant under attack.");
					Send_Message_Team(1,255,255,255,"Nod Power Plant under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Power_Plant") == 0)
				{
					Create_2D_WAV_Sound_Team("gdi_powerplant_under_attack_gdi.wav",1);
					Create_2D_WAV_Sound_Team("gdi_powerplant_under_attack_nod.wav",0);
					Send_Message_Team(1,255,255,255,"Warning GDI Power Plant under attack.");
					Send_Message_Team(0,255,255,255,"GDI Power Plant under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Obelisk") == 0)
				{
					Create_2D_WAV_Sound_Team("nod_obelisk_under_attack_gdi.wav",1);
					Create_2D_WAV_Sound_Team("nod_obelisk_under_attack_nod.wav",0);
					Send_Message_Team(0,255,255,255,"Warning Nod Obelisk under attack.");
					Send_Message_Team(1,255,255,255,"Nod Obelisk under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Airfield") == 0)
				{
					Create_2D_WAV_Sound_Team("nod_airfield_under_attack_gdi.wav",1);
					Create_2D_WAV_Sound_Team("nod_airfield_under_attack_nod.wav",0);
					Send_Message_Team(0,255,255,255,"Warning Nod Air Control Tower under attack.");
					Send_Message_Team(1,255,255,255,"Nod Airfield under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_GDI_Armory") == 0)
				{
					Create_2D_WAV_Sound_Team("gdi_armory_under_attack_gdi.wav",1);
					Send_Message_Team(1,255,255,255,"Warning GDI Armory under attack.");
					Send_Message_Team(0,255,255,255,"GDI Armory under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_GDI_Silo") == 0)
				{
					Create_2D_WAV_Sound_Team("gdi_our_base_under_attack.wav",1);
					Send_Message_Team(1,255,255,255,"Our Base is under attack. (Tiberium Silo)");
					Send_Message_Team(0,255,255,255,"GDI Tiberium Silo under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_GDI_CmdPost") == 0)
				{
					Create_2D_WAV_Sound_Team("gdi_command_post_under_attack_nod.wav",0);
					Send_Message_Team(0,255,255,255,"GDI Command Post under attack.");
					Create_2D_WAV_Sound_Team("gdi_command_post_under_attack_gdi.wav",1);
					Send_Message_Team(1,255,255,255,"Warning GDI Command Post under attack.");
					
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_Nod_OpCtr") == 0)
				{
					Create_2D_WAV_Sound_Team("nod_operations_centre_under_attack_gdi.wav",1);
					Send_Message_Team(1,255,255,255,"Nod Operations Centre under attack.");
					Create_2D_WAV_Sound_Team("nod_our_base_under_attack.wav",0);
					Send_Message_Team(0,255,255,255,"Our Base is under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_Nod_Shrine") == 0)
				{
					Create_2D_WAV_Sound_Team("nod_our_base_under_attack.wav",0);
					Send_Message_Team(0,255,255,255,"Our Base is under attack.");
					Create_2D_WAV_Sound_Team("nod_shrine_under_attack_gdi.wav",1);
					Send_Message_Team(1,255,255,255,"Shrine of Nod under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_GDI_IonCtr") == 0)
				{
					Create_2D_WAV_Sound_Team("gdi_ion_center_under_attack_nod.wav",0);
					Send_Message_Team(0,255,255,255,"GDI Ion Cannon Control Center under attack.");
					Create_2D_WAV_Sound_Team("gdi_ion_center_under_attack_gdi.wav",1);
					Send_Message_Team(1,255,255,255,"Warning GDI Ion Cannon Control Center under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_Nod_Tmpl") == 0)
				{
					Create_2D_WAV_Sound_Team("nod_temple_under_attack_nod.wav",0);
					Send_Message_Team(0,255,255,255,"Warning Temple of Nod under attack.");
					Create_2D_WAV_Sound_Team("nod_temple_under_attack_gdi.wav",1);
					Send_Message_Team(1,255,255,255,"Temple of Nod under attack.");
				}
				else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_VehBld_Nod_Silo") == 0)
				{
					Create_2D_WAV_Sound_Team("nod_tiberium_silo_under_attack_nod.wav",0);
					Send_Message_Team(0,255,255,255,"Warning Nod Tiberium Silo under attack.");
					Create_2D_WAV_Sound_Team("nod_tiberium_silo_under_attack_gdi.wav",1);
					Send_Message_Team(1,255,255,255,"Nod Tiberium Silo under attack.");
				}
			}
		}
	}
}
void tcw_building::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100100)
	{
		if (alive)
		{
			playdamage = true;
		}
	}
	else if (number == 111222)
	{
		if (alive)
		{
			if (Is_Base_Powered(1))
			{
				Send_Custom_Event_To_Object(obj,"TCW_Nod_Shadow",1004,1,0);
				Send_Custom_Event_To_Object(obj,"TCW_Nod_Shadow_Low",1004,1,0);
				Send_Custom_Event_To_Object(obj,"TCW_Nod_Stealth_Tank",1004,1,0);
				Send_Custom_Event_To_Object(obj,"TCW_GDI_Stealth_Tank",1004,1,0);
				Send_Custom_Event_To_Object(obj,"TCW_Nod_Harvester",1004,1,0);
				Send_Custom_Event_To_Object(obj,"TCW_Nod_Harvester_Player",1004,1,0);
				Send_Custom_Event_To_Object(obj,"TCW_GDI_Nod_Harvester_Player",1004,1,0);
				Send_Custom_Event_To_Object(obj,"TCW_GDI_Sniper",1004,1,0);
				Send_Custom_Event_To_Object(obj,"TCW_GDI_Sniper_Low",1004,1,0);
				Send_Custom_Event_To_Object(obj,"TCW_Nod_Commando",1004,1,0);
				Send_Custom_Event_To_Object(obj,"TCW_Nod_Commando_Low",1004,1,0);
				Commands->Start_Timer(obj,this,300.0f,111222);//radar scan repeat
				Send_Message(255,255,255,"GDI Command Post Radar Scan Initiated");
				Commands->Create_2D_WAV_Sound("radarscan.wav");
			}
		}
	}
}
void tcw_building::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 500500)
	{ 
		if (param == 1)
		{
			if (obj && obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_ComCenterGameObj())
			{
				Restore_Building(obj);
				Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
				Enable_Base_Radar(Get_Object_Type(obj),true);
				//enable pt options
			}
		}
	}
}
void tcw_building::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&alive,sizeof(alive),1);
	Auto_Save_Variable(&playdamage,sizeof(playdamage),2);
}

void tcw_watchtower::Created(GameObject *obj)
{
	play = true;
}
void tcw_watchtower::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (play)
	{
		if (damage > 0.1f)
		{
			play = false;
			Commands->Start_Timer(obj,this,25.0f,100100);
			Create_2D_WAV_Sound_Team("gdi_watchtower_under_attack_gdi.wav",1);
			Create_2D_WAV_Sound_Team("gdi_watchtower_under_attack_nod.wav",0);
			Send_Message_Team(1,255,255,255,"Warning: GDI Watch Tower under attack.");
			Send_Message_Team(0,255,255,255,"GDI Watch Tower under attack.");
		}
	}
}
void tcw_watchtower::Killed(GameObject *obj, GameObject *shooter)
{
	Create_2D_WAV_Sound_Team("gdi_watchtower_destroyed_gdi.wav",1);
	Create_2D_WAV_Sound_Team("gdi_watchtower_destroyed_nod.wav",0);
	Send_Message(255,255,255,"GDI Watch Tower destroyed.");
}
void tcw_watchtower::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100100)
	{
		play = true;
	}
}
void tcw_watchtower::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&play,sizeof(play),1);
}

ScriptRegistrant<tcw_watchtower> tcw_watchtower_Registrant("tcw_watchtower","");
ScriptRegistrant<tcw_building> tcw_building_Created_Registrant("tcw_building","");