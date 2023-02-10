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
#include "tcw_technology.h"

void tcw_Research_Composit_Armor::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	allowpokegdi = true;
	researching_composit_armor_upgrade = false;
	researched_composit_armor_upgrade = false;
	research_composit_armor_cost = 2000.0f;
	research_composit_armor_fund = 0.0f;
}
void tcw_Research_Composit_Armor::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpokegdi)
	{
		if (Commands->Get_Player_Type(poker) == 1)
		{
			if (!Is_Building_Dead(Find_Building_By_Preset(1,"TCW_GDI_Armory_Building")))
			{
				if (!researching_composit_armor_upgrade)
				{
					if (!researched_composit_armor_upgrade)
					{
						if (research_composit_armor_fund < 2000.0f)
						{
							if (Commands->Get_Money(poker) >= 250.0f)
							{
								research_composit_armor_fund += 250.0f;
								Commands->Give_Money(poker,-250.0f,false);
								char addedfunds[250];
								sprintf(addedfunds,"%s added $250 to the Composite Armor Upgrade. Current Funds: %g/%g",Get_Player_Name(poker),research_composit_armor_fund,research_composit_armor_cost);
								Send_Message_Team(1,255,204,0,addedfunds);
								
								if (research_composit_armor_fund == 2000.0f)
								{
									researching_composit_armor_upgrade = true;
									Commands->Start_Timer(obj,this,20.0f,10001);
									Create_2D_WAV_Sound_Team("gdi_tech_upgrade_in_progress.wav",1);
									Send_Message_Team(1,5,220,220,"Upgrade in progress: Composite Armor.");
									allowpokegdi = false;
								}
							}
							else
							{
								Send_Message_Player(poker,255,204,0,"Insufficient funds.");
								Create_2D_WAV_Sound_Player(poker,"gdi_insufficient_funds.wav");
							}
						}
					}
					else
					{
						Create_2D_WAV_Sound_Player(poker,"incmsg.wav");
						Send_Message_Player(poker,255,204,0,"Upgrade already performed.");						
					}
				}
				else
				{
					Create_2D_WAV_Sound_Player(poker,"gdi_tech_upgrade_in_progress.wav");
					Send_Message_Player(poker,255,204,0,"Upgrade in progress.");
				}
			}
			else
			{
				Create_2D_WAV_Sound_Player(poker,"incmsg.wav");
				Send_Message_Player(poker,255,204,0,"Sorry, your Armory structure has been destroyed.");
			}
		}
	}
}
void tcw_Research_Composit_Armor::Timer_Expired(GameObject *obj, int number)
{
	if (number == 10001)
	{
		if (!Is_Building_Dead(Find_Building_By_Preset(1,"TCW_GDI_Armory_Building")))
		{
			Vector3 pos; pos.Z = 999.0f; pos.Y = 999.0f; pos.X = 999.0f;
			GameObject *compositarmor = Commands->Create_Object("TCW_Tech_Composit_Armor_Available",pos);
			Commands->Set_Facing(compositarmor,0.0f);
			Send_Message_Team(1,5,220,220,"Upgrade Complete: Composite Armor.");
			Create_2D_WAV_Sound_Team("gdi_tech_upgrade_complete.wav",1);
			researched_composit_armor_upgrade = true;
			researching_composit_armor_upgrade = false;
			Commands->Enable_HUD_Pokable_Indicator(obj, false);
			Send_Custom_Event_To_Object(obj,"TCW_GDI_Rifle_Squad",1005,1,2);
			Send_Custom_Event_To_Object(obj,"TCW_GDI_Rifle_Squad_Low",1005,1,2);
			Send_Custom_Event_To_Object(obj,"TCW_GDI_Grenadier",1005,1,2);
			Send_Custom_Event_To_Object(obj,"TCW_GDI_Grenadier_Low",1005,1,2);
			Send_Custom_Event_To_Object(obj,"TCW_GDI_Missile_Squad",1005,1,2);
			Send_Custom_Event_To_Object(obj,"TCW_GDI_Missile_Squad_Low",1005,1,2);
		}
		else
		{
			Send_Message_Team(1,255,204,0,"Upgrading of Composite Armor has failed. Structure lost.");
			Create_2D_WAV_Sound_Team("incmsg.wav",1);
		}
	}
	else if (number == 10002)
	{
		allowpokegdi = true;
	}
}
void tcw_Research_Composit_Armor::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&researched_composit_armor_upgrade,sizeof(researched_composit_armor_upgrade),1);
	Auto_Save_Variable(&research_composit_armor_cost,sizeof(research_composit_armor_cost),2);
	Auto_Save_Variable(&research_composit_armor_fund,sizeof(research_composit_armor_fund),3);
	Auto_Save_Variable(&researching_composit_armor_upgrade,sizeof(researching_composit_armor_upgrade),4);	
	Auto_Save_Variable(&allowpokegdi,sizeof(allowpokegdi),5);
}

void tcw_hide_show_tech::Created(GameObject *obj)
{
	//gdi wf steal-able tech:
	Hide_Preset_By_Name(0,"TCW_Nod_Predator_Tank",true);
	Hide_Preset_By_Name(0,"TCW_Nod_Pitbull",true);
	Hide_Preset_By_Name(0,"TCW_Nod_GDI_Harvester_Player",true);
	Hide_Preset_By_Name(0,"TCW_Nod_APC",true);
	Hide_Preset_By_Name(0,"TCW_Nod_Mammoth_Tank",true);

	//gdi bar steal-able tech:
	Hide_Preset_By_Name(0,"TCW_GDI_Rifle_Squad",true);
	Hide_Preset_By_Name(0,"TCW_GDI_Engineer",true);
	Hide_Preset_By_Name(0,"TCW_GDI_Missile_Squad",true);
	Hide_Preset_By_Name(0,"TCW_GDI_Grenadier",true);
	Hide_Preset_By_Name(0,"TCW_GDI_Sniper",true);
	Hide_Preset_By_Name(0,"TCW_GDI_Zonetrooper",true);
	Hide_Preset_By_Name(0,"TCW_GDI_Commando",true);

	//nod wf steal-able tech:
	Hide_Preset_By_Name(1,"TCW_GDI_Attack_Bike",true);
	Hide_Preset_By_Name(1,"TCW_GDI_Flame_Tank",true);
	Hide_Preset_By_Name(1,"TCW_GDI_Nod_Harvester_Player",true);
	Hide_Preset_By_Name(1,"TCW_GDI_Raider_Buggy",true);
	Hide_Preset_By_Name(1,"TCW_GDI_Scorpion_Tank",true);
	Hide_Preset_By_Name(1,"TCW_GDI_Stealth_Tank",true);

	//nod hand steal-able tech:
	Hide_Preset_By_Name(1,"TCW_Nod_Militant_Infantry",true);
	Hide_Preset_By_Name(1,"TCW_Nod_Saboteur",true);
	Hide_Preset_By_Name(1,"TCW_Nod_RocketGuy",true);
	Hide_Preset_By_Name(1,"TCW_Nod_Shadow",true);
	Hide_Preset_By_Name(1,"TCW_Nod_Black_Hand_Flame",true);
	Hide_Preset_By_Name(1,"TCW_Nod_Commando",true);
}


////Start tech stealing zones

void tcw_steal_tech_zone_gwf::Entered(GameObject *obj, GameObject *enter)
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
					Attach_Script_Once(enter,"tcw_i_am_stealing_tech","1");
				}
			}
		}
	}
}
void tcw_steal_tech_zone_gwf::Exited(GameObject *obj, GameObject *exit)
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

void tcw_steal_tech_zone_nwf::Entered(GameObject *obj, GameObject *enter)
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
					Attach_Script_Once(enter,"tcw_i_am_stealing_tech","2");
				}
			}
		}
	}
}
void tcw_steal_tech_zone_nwf::Exited(GameObject *obj, GameObject *exit)
{
	if ( (Commands->Is_A_Star(exit)) && (Commands->Get_Player_Type(exit) == 1) )
	{
		if ( (strcmp(Commands->Get_Preset_Name(exit),"TCW_GDI_Engineer") == 0) || (strcmp(Commands->Get_Preset_Name(exit),"TCW_GDI_Engineer_Low") == 0) || 
			(strcmp(Commands->Get_Preset_Name(exit),"TCW_Nod_Saboteur") == 0) || (strcmp(Commands->Get_Preset_Name(exit),"TCW_Nod_Saboteur_Low") == 0) )
		{
			Remove_Script(exit,"tcw_i_am_stealing_tech");
		}
	}
}

void tcw_steal_tech_zone_nhon::Entered(GameObject *obj, GameObject *enter)
{
	if (!Is_Building_Dead(Find_Building_By_Preset(0,"TCW_Hand_Of_Nod")))
	{
		if ( (Commands->Is_A_Star(enter)) && (Commands->Get_Player_Type(enter) == 1) )
		{
			if ( (strcmp(Commands->Get_Preset_Name(enter),"TCW_GDI_Engineer") == 0) || (strcmp(Commands->Get_Preset_Name(enter),"TCW_GDI_Engineer_Low") == 0) || 
				(strcmp(Commands->Get_Preset_Name(enter),"TCW_Nod_Saboteur") == 0) || (strcmp(Commands->Get_Preset_Name(enter),"TCW_Nod_Saboteur_Low") == 0) )
			{
				if (!Get_Vehicle(enter))
				{
					Attach_Script_Once(enter,"tcw_i_am_stealing_tech","3");
				}
			}
		}
	}
}
void tcw_steal_tech_zone_nhon::Exited(GameObject *obj, GameObject *exit)
{
	if ( (Commands->Is_A_Star(exit)) && (Commands->Get_Player_Type(exit) == 1) )
	{
		if ( (strcmp(Commands->Get_Preset_Name(exit),"TCW_GDI_Engineer") == 0) || (strcmp(Commands->Get_Preset_Name(exit),"TCW_GDI_Engineer_Low") == 0) || 
			(strcmp(Commands->Get_Preset_Name(exit),"TCW_Nod_Saboteur") == 0) || (strcmp(Commands->Get_Preset_Name(exit),"TCW_Nod_Saboteur_Low") == 0) )
		{
			Remove_Script(exit,"tcw_i_am_stealing_tech");
		}
	}
}

void tcw_steal_tech_zone_gbar::Entered(GameObject *obj, GameObject *enter)
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
					Attach_Script_Once(enter,"tcw_i_am_stealing_tech","4");
				}
			}
		}
	}
}
void tcw_steal_tech_zone_gbar::Exited(GameObject *obj, GameObject *exit)
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

////End tech stealing zones

void tcw_i_am_stealing_tech::Created(GameObject *obj)
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
	else if (building == 2)//gdi stealing nod wf
	{
		Commands->Start_Timer(obj,this,15.0f,779902);//start stealing from the gdi wf
		char stealermsg[250];
		sprintf(stealermsg,"%s is stealing technology from the Nod War Factory.",Get_Player_Name(obj));
		Send_Message_Team(1,255,204,0,stealermsg);
		Create_2D_WAV_Sound_Team("building_capture.wav",1);
	}
	else if (building == 3)//gdi stealing nod hand of nod
	{
		Commands->Start_Timer(obj,this,15.0f,779903);//start stealing from the nod hand of nod
		char stealermsg[250];
		sprintf(stealermsg,"%s is stealing technology from the Hand of Nod.",Get_Player_Name(obj));
		Send_Message_Team(1,255,204,0,stealermsg);
		Create_2D_WAV_Sound_Team("building_capture.wav",1);
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
void tcw_i_am_stealing_tech::Timer_Expired(GameObject *obj, int number)
{
	if (number == 779901)//nod stole tech from gdi wf
	{
		if (!Is_Building_Dead(Find_Building_By_Preset(1,"TCW_GDI_War_Factory")))
		{
			if (Get_Total_Bullets(obj,"TCW_Weapon_Repairgun_Advanced") == 5)//full repairgun = successful stealing
			{
				Commands->Select_Weapon(obj,"TCW_Weapon_Pistol");
				Remove_Weapon(obj,"TCW_Weapon_Repairgun_Advanced");
					
				Hide_Preset_By_Name(0,"TCW_Nod_Predator_Tank",false);
				Hide_Preset_By_Name(0,"TCW_Nod_Pitbull",false);
				Hide_Preset_By_Name(0,"TCW_Nod_GDI_Harvester_Player",false);
				Hide_Preset_By_Name(0,"TCW_Nod_APC",false);
				Hide_Preset_By_Name(0,"TCW_Nod_Mammoth_Tank",false);

				char stealmsg[250];
				sprintf(stealmsg,"%s has stolen technology from the GDI War Factory",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,stealmsg);
				Send_Message_Team(0,255,0,0,"New technology acquired.");
				Create_2D_WAV_Sound_Team("nod_new_tech_acquired.wav",0);
			}
			else
			{
				Send_Message_Player(obj,255,0,0,"To steal technology you need a FULL AMMO Repair Gun.");
				Destroy_Script();
			}
		}
	}
	else if (number == 779902)//gdi stole tech from nod wf
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
				sprintf(stealmsg,"%s has stolen technology from the Nod War Factory",Get_Player_Name(obj));
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
	else if (number == 779903)//gdi stole tech from nod hand
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
				Hide_Preset_By_Name(0,"TCW_GDI_Zonetrooper",false);
				Hide_Preset_By_Name(0,"TCW_GDI_Commando",false);

				char stealmsg[250];
				sprintf(stealmsg,"%s has stolen technology from the GDI Barracks",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,stealmsg);
				Send_Message_Team(0,255,0,0,"New technology acquired.");
				Create_2D_WAV_Sound_Team("nod_new_tech_acquired.wav",0);
			}
			else
			{
				Send_Message_Player(obj,255,0,0,"To steal technology you need a FULL AMMO Repair Gun.");
				Destroy_Script();
			}
		}
	}
}

void tcw_Research_Powerpacks::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	allowpokegdi = true;
	researching_powerpacks_upgrade = false;
	researched_powerpacks_upgrade = false;
	research_powerpacks_cost = 2000.0f;
	research_powerpacks_fund = 0.0f;
}
void tcw_Research_Powerpacks::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpokegdi)
	{
		if (Commands->Get_Player_Type(poker) == 1)
		{
			if (!Is_Building_Dead(Find_Building_By_Preset(1,"TCW_GDI_Armory_Building")))
			{
				if (!researching_powerpacks_upgrade)
				{
					if (!researched_powerpacks_upgrade)
					{
						if (research_powerpacks_fund < 2000.0f)
						{
							if (Commands->Get_Money(poker) >= 250.0f)
							{
								research_powerpacks_fund += 250.0f;
								Commands->Give_Money(poker,-250.0f,false);
								char addedfunds[250];
								sprintf(addedfunds,"%s added $250 to the Power Packs Upgrade. Current Funds: %g/%g",Get_Player_Name(poker),research_powerpacks_fund,research_powerpacks_cost);
								Send_Message_Team(1,255,204,0,addedfunds);
								
								if (research_powerpacks_fund == 2000.0f)
								{
									researching_powerpacks_upgrade = true;
									Commands->Start_Timer(obj,this,60.0f,10001);
									Create_2D_WAV_Sound_Team("gdi_tech_upgrade_in_progress.wav",1);
									Send_Message_Team(1,5,220,220,"Upgrade in progress: Power Packs.");
									allowpokegdi = false;
								}
							}
							else
							{
								Send_Message_Player(poker,255,204,0,"Insufficient funds.");
								Create_2D_WAV_Sound_Player(poker,"gdi_insufficient_funds.wav");
							}
						}
					}
					else
					{
						Create_2D_WAV_Sound_Player(poker,"incmsg.wav");
						Send_Message_Player(poker,255,204,0,"Upgrade already performed.");						
					}
				}
				else
				{
					Create_2D_WAV_Sound_Player(poker,"gdi_tech_upgrade_in_progress.wav");
					Send_Message_Player(poker,255,204,0,"Upgrade in progress.");
				}
			}
			else
			{
				Create_2D_WAV_Sound_Player(poker,"incmsg.wav");
				Send_Message_Player(poker,255,204,0,"Sorry, your Armory structure has been destroyed.");
			}
		}
	}
}
void tcw_Research_Powerpacks::Timer_Expired(GameObject *obj, int number)
{
	if (number == 10001)
	{
		if (!Is_Building_Dead(Find_Building_By_Preset(1,"TCW_GDI_Armory_Building")))
		{
			Vector3 pos; pos.Z = 999.0f; pos.Y = 999.0f; pos.X = 999.0f;
			GameObject *powerpacks = Commands->Create_Object("TCW_Tech_Power_Packs_Available",pos);
			Commands->Set_Facing(powerpacks,0.0f);
			Send_Message_Team(1,5,220,220,"Upgrade Complete: Power Packs.");
			Create_2D_WAV_Sound_Team("gdi_tech_upgrade_complete.wav",1);
			researched_powerpacks_upgrade = true;
			researching_powerpacks_upgrade = false;
			Commands->Enable_HUD_Pokable_Indicator(obj, false);
			Send_Custom_Event_To_Object(obj,"TCW_GDI_Zonetrooper",1006,1,2);
			Send_Custom_Event_To_Object(obj,"TCW_GDI_Zonetrooper_Low",1006,1,2);
		}
		else
		{
			Send_Message_Team(1,255,204,0,"Upgrading of Power Packs has failed. Structure lost.");
			Create_2D_WAV_Sound_Team("incmsg.wav",1);
		}
	}
	else if (number == 10002)
	{
		allowpokegdi = true;
	}
}
void tcw_Research_Powerpacks::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&researched_powerpacks_upgrade,sizeof(researched_powerpacks_upgrade),1);
	Auto_Save_Variable(&research_powerpacks_cost,sizeof(research_powerpacks_cost),2);
	Auto_Save_Variable(&research_powerpacks_fund,sizeof(research_powerpacks_fund),3);
	Auto_Save_Variable(&researching_powerpacks_upgrade,sizeof(researching_powerpacks_upgrade),4);	
	Auto_Save_Variable(&allowpokegdi,sizeof(allowpokegdi),5);
}

ScriptRegistrant<tcw_Research_Powerpacks> tcw_Research_Powerpacks_Registrant("tcw_Research_Powerpacks","");
ScriptRegistrant<tcw_steal_tech_zone_gbar> tcw_steal_tech_zone_gbar_Registrant("tcw_steal_tech_zone_gbar","");
ScriptRegistrant<tcw_steal_tech_zone_nhon> tcw_steal_tech_zone_nhon_Registrant("tcw_steal_tech_zone_nhon","");
ScriptRegistrant<tcw_steal_tech_zone_nwf> tcw_steal_tech_zone_nwf_Registrant("tcw_steal_tech_zone_nwf","");
ScriptRegistrant<tcw_steal_tech_zone_gwf> tcw_steal_tech_zone_gwf_Registrant("tcw_steal_tech_zone_gwf","");
ScriptRegistrant<tcw_i_am_stealing_tech> tcw_i_am_stealing_tech_Registrant("tcw_i_am_stealing_tech","Building=1:int");
ScriptRegistrant<tcw_hide_show_tech> tcw_hide_show_tech_Registrant("tcw_hide_show_tech","");
ScriptRegistrant<tcw_Research_Composit_Armor> tcw_Research_Composit_Armor_Registrant("tcw_Research_Composit_Armor","");



