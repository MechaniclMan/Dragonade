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
#include "General.h"
#include "scripts.h"
#include "engine.h"
#include "z_coop.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"
#include "ScriptableGameObj.h"
//Just having a script thats attached to all player type characters
//in case i think of something that i want to do to players globally...

void z_I_Am_A_Player::Created(GameObject *obj)
{
	if (_stricmp(The_Game()->Get_Map_Name(),"RA_DarkDawn.mix") == 0)
	{
		if (Commands->Get_Player_Type(obj) == 0)
		{
			Commands->Start_Timer(obj,this,1.0f,1001);//darkdawn teamchange
		}

		if (Get_Object_Count(1,"Allied_Tanya") >= 2)
		{
			char gfd[250];
			sprintf(gfd,"ppage %d Tanya limit reached",Get_Player_ID(obj));
			Console_Input(gfd);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
		else if (Get_Object_Count(1,"Allied_Mechanic") >= 2)
		{
			char gfd[250];
			sprintf(gfd,"ppage %d Mechanic limit reached",Get_Player_ID(obj));
			Console_Input(gfd);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
		else if (Get_Object_Count(1,"Allied_Sniper_Coop") >= 3)
		{
			char gfd[250];
			sprintf(gfd,"ppage %d Sniper limit reached",Get_Player_ID(obj));
			Console_Input(gfd);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
		else if (Get_Object_Count(1,"Allied_Rocket_Soldier") >= 5)
		{
			char gfd[250];
			sprintf(gfd,"ppage %d Rocket Soldier limit reached",Get_Player_ID(obj));
			Console_Input(gfd);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
		else if (Get_Object_Count(1,"Allied_Captain") >= 7)
		{
			char gfd[250];
			sprintf(gfd,"ppage %d Captain limit reached",Get_Player_ID(obj));
			Console_Input(gfd);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
	}
	else if (_stricmp(The_Game()->Get_Map_Name(),"RA_Midnight_Fever.mix") == 0)
	{
		if (Commands->Get_Player_Type(obj) == 0)
		{
			Commands->Start_Timer(obj,this,1.0f,1001);//midnight fever teamchange
		}

		if (Get_Object_Count(1,"Allied_Tanya") >= 2)
		{
			char gfd[250];
			sprintf(gfd,"ppage %d Tanya limit reached",Get_Player_ID(obj));
			Console_Input(gfd);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
		else if (Get_Object_Count(1,"Allied_Mechanic") >= 2)
		{
			char gfd[250];
			sprintf(gfd,"ppage %d Mechanic limit reached",Get_Player_ID(obj));
			Console_Input(gfd);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
		else if (Get_Object_Count(1,"Allied_Sniper_Coop") >= 3)
		{
			char gfd[250];
			sprintf(gfd,"ppage %d Sniper limit reached",Get_Player_ID(obj));
			Console_Input(gfd);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
		else if (Get_Object_Count(1,"Allied_Rocket_Soldier") >= 5)
		{
			char gfd[250];
			sprintf(gfd,"ppage %d Rocket Soldier limit reached",Get_Player_ID(obj));
			Console_Input(gfd);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
		else if (Get_Object_Count(1,"Allied_Captain") >= 7)
		{
			char gfd[250];
			sprintf(gfd,"ppage %d Captain limit reached",Get_Player_ID(obj));
			Console_Input(gfd);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
	}
	else if (_stricmp(The_Game()->Get_Map_Name(),"RA_MCV_Escort.mix") == 0)
	{
		if (Commands->Get_Player_Type(obj) == 0)
		{
			Commands->Start_Timer(obj,this,1.0f,1001);
		}
		else if (Get_Object_Count(1,"Allied_Medic_Coop") >= 5)
		{
			char gfd[250];
			sprintf(gfd,"ppage %d Medic limit reached",Get_Player_ID(obj));
			Console_Input(gfd);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
		else if (Get_Object_Count(1,"Allied_Rocket_Soldier") >= 5)
		{
			char gfd[250];
			sprintf(gfd,"ppage %d Rocket Soldier limit reached",Get_Player_ID(obj));
			Console_Input(gfd);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
		else if (Get_Object_Count(1,"Allied_Captain") >= 7)
		{
			char gfd[250];
			sprintf(gfd,"ppage %d Captain limit reached",Get_Player_ID(obj));
			Console_Input(gfd);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
	}
}
void z_I_Am_A_Player::Killed(GameObject *obj, GameObject *shooter)
{
/*#ifdef SSGM
	char killmsg[200];
	sprintf(killmsg,"The %s terminated %s",Get_Translated_Preset_Name_Ex(shooter),Get_Player_Name(obj));
	Send_Message(255,96,9,killmsg);
#endif*/
}
void z_I_Am_A_Player::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1000)//change to soviets
	{
		Change_Team(obj,0);
	}
	else if (number == 1001)//change to allies
	{
		Change_Team(obj,1);
	}
}

void z_I_Can_Steal_Tech::Created(GameObject *obj)//use on presets that can steal tech from bar or wf or whatever building
{
	//dummy script
}

void z_AI_Bot_Attach_Dropweapons::ObjectCreateHook(GameObject *obj)
{
	if (!Commands->Is_A_Star(obj))
	{
		if (obj->As_SoldierGameObj())
		{
			Attach_Script_Once(obj,"z_AI_Bot_Dropweapons","");
		}
		Commands->Start_Timer(obj,this,45.0f,123454321);
	}
}
void z_AI_Bot_Attach_Dropweapons::Timer_Expired(GameObject *obj, int number)
{
	if (number == 123454321)
	{
		if (!Commands->Is_A_Star(obj))
		{
			if (obj->As_SoldierGameObj())
			{
				Attach_Script_Once(obj,"z_AI_Bot_Dropweapons","");
				Commands->Start_Timer(obj,this,45.0f,123454321);
			}
		}
	}
}

void z_AI_Bot_Dropweapons::Destroyed(GameObject *obj)
{
	int random = Commands->Get_Random_Int(1,15);
	if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Engineer") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Engineer") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Engineer_Stationary") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M01_NodEngiTurret") == 0) )
	{
		if (random == 1)
		{
			GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_MineRemote_Player",pos);
			Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
			GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("CnC_POW_MineRemote_02",pos);
			Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
			GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_RepairGun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
			GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Flamethrower") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Flamethrower_Star") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M01_FlamerGuy") == 0) )
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_Flamethrower_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Flamethrower_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Flamethrower_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if (strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Grenadier") == 0)
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_GrenadeLauncher_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_GrenadeLauncher_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_GrenadeLauncher_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ((strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Hotwire") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Hotwire_Stationary") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Joi_Hotwire_Repair_PP") == 0))
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("CnC_POW_RepairGun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("CnC_POW_MineRemote_02",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("CnC_MineProximity_05",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Chemwarrior") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Chemwarrior_Star") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M00_Nod_Chemwarr") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M01_Chemwarr") == 0) )
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_ChemSprayer_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_ChemSprayer_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_ChemSprayer_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_SBH") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_SBH_Star") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M00_Nod_SBH") == 0) )
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_LaserRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_LaserRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_LaserRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if (strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Patch") == 0)
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_TiberiumFlechetteGun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_TiberiumFlechetteGun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_TiberiumFlechetteGun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if (strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Minigunner") == 0)
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_AutoRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_AutoRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_AutoRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if (strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Officer") == 0)
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_Chaingun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Chaingun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Chaingun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ((strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Sniper") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Sniper_Stationary") == 0))
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_SniperRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_SniperRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_SniperRifle_Player_Nod",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if (strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Shotgunner") == 0)
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_Shotgun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Shotgun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Shotgun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ((strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Rocketsoldier") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Rocketsoldier_Stationary") == 0))
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_RocketLauncher_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_RocketLauncher_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_RocketLauncher_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ((strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Gunner") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Gunner_Stationary") == 0))
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("CnC_POW_RocketLauncher_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("CnC_POW_RocketLauncher_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("CnC_POW_RocketLauncher_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if (strcmp(Commands->Get_Preset_Name(obj),"Bot_GDI_Sydney") == 0)
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_TiberiumAutoRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_TiberiumAutoRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_TiberiumAutoRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Minigunner") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Minigunner_Star") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M01_Minigunr") == 0) )
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_AutoRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_AutoRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_AutoRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Officer") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Officer_Star") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M00_NodOfficer") == 0) || 
		      (strcmp(Commands->Get_Preset_Name(obj),"M01_NodOfficer") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M01_NodOfficerStat") == 0)  )
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_Chaingun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Chaingun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Chaingun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Sniper_Stationary") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Sniper") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M01_Nod_SniperStat") == 0) )
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_SniperRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_SniperRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_SniperRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Shotgunner") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Shotgunner_Star") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M01_Nod_Shotty") == 0) )
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_Shotgun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Shotgun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Shotgun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Sakura") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Sakura_Stationary") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M01_SakuraBot") == 0) )
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_RamjetRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_RamjetRifle_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Rocketguy") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Rocketguy_Star") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Rocketguy_Stationary") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M00_Nod_RocketGuy") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M01_NodRocketGuy") == 0) )
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_RocketLauncher_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_RocketLauncher_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("CnC_POW_RocketLauncher_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Laserchaingunner_Star") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Laserchaingunner") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M00_Nod_LCG") == 0) )
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_LaserChaingun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_LaserChaingun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_LaserChaingun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}
	
	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Tech_Repair") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Tech") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Tech_Stationary") == 0) ||
		(strcmp(Commands->Get_Preset_Name(obj),"M01_Nod_Techie") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M01_Nod_Techie_Repair") == 0) )
	{
		if (random == 1)
		{
			GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("CnC_POW_RepairGun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
			GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("CnC_POW_MineRemote_02",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
			GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_MineRemote_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 4)
		{
			GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("CnC_MineProximity_05",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 5)
		{
			GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 6)
		{
			GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Armor_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 7)
		{
			GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_025",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 8)
		{
			GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_050",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 9)
		{
			GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Health_100",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else
		{
			Destroy_Script();
		}
	}
}

void z_Building_Hand_Of_Nod::Created(GameObject *obj)
{
	playdmg = true;
	alive = true;
}
void z_Building_Hand_Of_Nod::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (alive)
	{
		if (playdmg)
		{
			if (damage > 1.0f)
			{
				playdmg = false;
				Commands->Create_2D_WAV_Sound("m00bnhn_tdfe0002i1evag_snd.wav");
				Send_Message(255,255,255,"Hand of Nod under attack.");
				Commands->Start_Timer(obj,this,20.0f,163);//playdmg true
			}
		}
	}
}
void z_Building_Hand_Of_Nod::Timer_Expired(GameObject *obj, int number)
{
	if (number == 163)
	{
		if (alive)
		{
			playdmg = true;
		}
	}
}
void z_Building_Hand_Of_Nod::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Create_2D_WAV_Sound("m00bnhn_kill0002i1evag_snd.wav");
	Send_Message(255,255,255,"Hand of Nod destroyed.");
	alive = false;
	playdmg = false;
}
void z_Building_Hand_Of_Nod::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&alive,5,1);
	Auto_Save_Variable(&playdmg,5,1);
}

ScriptRegistrant<z_Building_Hand_Of_Nod> z_Building_Hand_Of_Nod_Registrant("z_Building_Hand_Of_Nod","");
ScriptRegistrant<z_AI_Bot_Attach_Dropweapons> z_AI_Bot_Attach_Dropweapons_Registrant("z_AI_Bot_Attach_Dropweapons","");
ScriptRegistrant<z_AI_Bot_Dropweapons> z_AI_Bot_Dropweapons_Registrant("z_AI_Bot_Dropweapons","");
ScriptRegistrant<z_I_Can_Steal_Tech> z_I_Can_Steal_Tech_Registrant("z_I_Can_Steal_Tech","");
ScriptRegistrant<z_I_Am_A_Player> z_I_Am_A_Player_Registrant("z_I_Am_A_Player","");

