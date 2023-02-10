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
#include "SoldierGameObj.h"
#include "tcw_player.h"
#ifdef SSGM
#include "gmgame.h"
#endif

#pragma warning(disable: 4239)

void tcw_Grant_Powerup_Created::Created(GameObject *obj)//with this script we can do to players what we want easily
{
	map_low_grav = false;	map_dm_tech1 = false;	map_dm_tech2 = false;	map_dm_tech3 = false;	map_coop_gdi = false;	map_coop_nod = false;	map_sniper_only = false;
	if (Find_Object_By_Preset(2,"TCW_Low_Grav_Map")) { map_low_grav = true; }
	if (Find_Object_By_Preset(2,"TCW_DM_Tech1")) { map_dm_tech1 = true; }//no snipers, shadows and commando's
	if (Find_Object_By_Preset(2,"TCW_DM_Tech2")) { map_dm_tech2 = true; }//no commando's
	if (Find_Object_By_Preset(2,"TCW_DM_Tech3")) { map_dm_tech3 = true; }//no limitations
	if (Find_Object_By_Preset(2,"TCW_Coop_Map_GDI")) { map_coop_gdi = true; }
	if (Find_Object_By_Preset(2,"TCW_Coop_Map_Nod")) { map_coop_nod = true; }
	if (Find_Object_By_Preset(2,"TCW_Sniper_Only_Map")) { map_sniper_only = true; }

	if (map_coop_nod)
	{
		if (Commands->Get_Player_Type(obj) == 1)	{ Change_Team(obj,0); }
	}
	else if (map_coop_gdi)
	{
		if (Commands->Get_Player_Type(obj) == 0)	{ Change_Team(obj,1); }
	}
	else if (map_sniper_only)
	{
		if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Sniper") != 0)
		{
			Change_Character(obj,"TCW_GDI_Sniper");
		}
	}
	if (map_dm_tech1)
	{
		int randomchargdi = Commands->Get_Random_Int(1,5);		int randomcharnod = Commands->Get_Random_Int(1,5);
		if (map_low_grav)
		{
			if (Commands->Get_Player_Type(obj) == 1)
			{
				if (strcmp(Commands->Get_Preset_Name(obj),"CnC_GDI_MiniGunner_0") == 0)
				{
					if (randomchargdi == 1)				{ Change_Character(obj,"TCW_GDI_Rifle_Squad_Low"); }
					else if (randomchargdi == 2)		{ Change_Character(obj,"TCW_GDI_Engineer_Low"); }
					else if (randomchargdi == 3)		{ Change_Character(obj,"TCW_GDI_Grenadier_Low"); }
					else if (randomchargdi == 4)		{ Change_Character(obj,"TCW_GDI_Missile_Squad_Low"); }
					else								{ Change_Character(obj,"TCW_GDI_Zonetrooper_Low"); }
				}
			}
			else if (Commands->Get_Player_Type(obj) == 0)
			{
				if (strcmp(Commands->Get_Preset_Name(obj),"CnC_Nod_Minigunner_0") == 0)
				{
					if (randomcharnod == 1)				{ Change_Character(obj,"TCW_Nod_Militant_Infantry_Low"); }
					else if (randomcharnod == 2)		{ Change_Character(obj,"TCW_Black_Hand_Low"); }
					else if (randomcharnod == 3)		{ Change_Character(obj,"TCW_Nod_RocketGuy_Low"); }
					else if (randomcharnod == 4)		{ Change_Character(obj,"TCW_Nod_Saboteur_Low"); }
					else								{ Change_Character(obj,"TCW_Nod_Militant_Infantry_Low"); }
				}
			}
		}
		else
		{
			if (Commands->Get_Player_Type(obj) == 1)
			{
				if (strcmp(Commands->Get_Preset_Name(obj),"CnC_GDI_MiniGunner_0") == 0)
				{
					if (randomchargdi == 1)				{ Change_Character(obj,"TCW_GDI_Rifle_Squad"); }
					else if (randomchargdi == 2)		{ Change_Character(obj,"TCW_GDI_Engineer"); }
					else if (randomchargdi == 3)		{ Change_Character(obj,"TCW_GDI_Grenadier"); }
					else if (randomchargdi == 4)		{ Change_Character(obj,"TCW_GDI_Missile_Squad"); }
					else								{ Change_Character(obj,"TCW_GDI_Zonetrooper"); }
				}
			}
			else if (Commands->Get_Player_Type(obj) == 0)
			{
				if (strcmp(Commands->Get_Preset_Name(obj),"CnC_Nod_Minigunner_0") == 0)
				{
					if (randomchargdi == 1)				{ Change_Character(obj,"TCW_Nod_Militant_Infantry"); }
					else if (randomchargdi == 2)		{ Change_Character(obj,"TCW_Nod_Black_Hand_Flame"); }
					else if (randomchargdi == 3)		{ Change_Character(obj,"TCW_Nod_RocketGuy"); }
					else if (randomchargdi == 4)		{ Change_Character(obj,"TCW_Nod_Saboteur"); }
					else								{ Change_Character(obj,"TCW_Nod_Militant_Infantry"); }
				}
			}
		}
	}
	else if (map_dm_tech2)
	{
		int randomchargdi = Commands->Get_Random_Int(1,6);		int randomcharnod = Commands->Get_Random_Int(1,6);
		if (map_low_grav)
		{
			if (Commands->Get_Player_Type(obj) == 1)
			{
				if (strcmp(Commands->Get_Preset_Name(obj),"CnC_GDI_MiniGunner_0") == 0)
				{
					if (randomchargdi == 1)				{ Change_Character(obj,"TCW_GDI_Rifle_Squad_Low"); }
					else if (randomchargdi == 2)		{ Change_Character(obj,"TCW_GDI_Engineer_Low"); }
					else if (randomchargdi == 3)		{ Change_Character(obj,"TCW_GDI_Grenadier_Low"); }
					else if (randomchargdi == 4)		{ Change_Character(obj,"TCW_GDI_Missile_Squad_Low"); }
					else if (randomchargdi == 5)		{ Change_Character(obj,"TCW_GDI_Sniper_Low"); }
					else								{ Change_Character(obj,"TCW_GDI_Zonetrooper_Low"); }
				}
			}
			else if (Commands->Get_Player_Type(obj) == 0)
			{
				if (strcmp(Commands->Get_Preset_Name(obj),"CnC_Nod_Minigunner_0") == 0)
				{
					if (randomcharnod == 1)				{ Change_Character(obj,"TCW_Nod_Militant_Infantry_Low"); }
					else if (randomcharnod == 2)		{ Change_Character(obj,"TCW_Black_Hand_Low"); }
					else if (randomcharnod == 3)		{ Change_Character(obj,"TCW_Nod_RocketGuy_Low"); }
					else if (randomcharnod == 4)		{ Change_Character(obj,"TCW_Nod_Saboteur_Low"); }
					else if (randomcharnod == 5)		{ Change_Character(obj,"TCW_Nod_Shadow_Low"); }
					else								{ Change_Character(obj,"TCW_Nod_Militant_Infantry_Low"); }
				}
			}
		}
		else
		{
			if (Commands->Get_Player_Type(obj) == 1)
			{
				if (strcmp(Commands->Get_Preset_Name(obj),"CnC_GDI_MiniGunner_0") == 0)
				{
					if (randomchargdi == 1)				{ Change_Character(obj,"TCW_GDI_Rifle_Squad"); }
					else if (randomchargdi == 2)		{ Change_Character(obj,"TCW_GDI_Engineer"); }
					else if (randomchargdi == 3)		{ Change_Character(obj,"TCW_GDI_Grenadier"); }
					else if (randomchargdi == 4)		{ Change_Character(obj,"TCW_GDI_Missile_Squad"); }
					else if (randomchargdi == 5)		{ Change_Character(obj,"TCW_GDI_Sniper"); }
					else								{ Change_Character(obj,"TCW_GDI_Zonetrooper"); }
				}
			}
			else if (Commands->Get_Player_Type(obj) == 0)
			{
				if (strcmp(Commands->Get_Preset_Name(obj),"CnC_Nod_Minigunner_0") == 0)
				{
					if (randomchargdi == 1)				{ Change_Character(obj,"TCW_Nod_Militant_Infantry"); }
					else if (randomchargdi == 2)		{ Change_Character(obj,"TCW_Nod_Black_Hand_Flame"); }
					else if (randomchargdi == 3)		{ Change_Character(obj,"TCW_Nod_RocketGuy"); }
					else if (randomchargdi == 4)		{ Change_Character(obj,"TCW_Nod_Saboteur"); }
					else if (randomchargdi == 5)		{ Change_Character(obj,"TCW_Nod_Shadow"); }
					else								{ Change_Character(obj,"TCW_Nod_Militant_Infantry"); }
				}
			}
		}
	}
	else if (map_dm_tech3)
	{
		int randomchargdi = Commands->Get_Random_Int(1,7);		int randomcharnod = Commands->Get_Random_Int(1,7);
		if (map_low_grav)
		{
			if (Commands->Get_Player_Type(obj) == 1)
			{
				if (strcmp(Commands->Get_Preset_Name(obj),"CnC_GDI_MiniGunner_0") == 0)
				{
					if (randomchargdi == 1)				{ Change_Character(obj,"TCW_GDI_Rifle_Squad_Low"); }
					else if (randomchargdi == 2)		{ Change_Character(obj,"TCW_GDI_Engineer_Low"); }
					else if (randomchargdi == 3)		{ Change_Character(obj,"TCW_GDI_Grenadier_Low"); }
					else if (randomchargdi == 4)		{ Change_Character(obj,"TCW_GDI_Missile_Squad_Low"); }
					else if (randomchargdi == 5)		{ Change_Character(obj,"TCW_GDI_Sniper_Low"); }
					else if (randomchargdi == 6)		{ Change_Character(obj,"TCW_GDI_Commando_Low"); }
					else								{ Change_Character(obj,"TCW_GDI_Zonetrooper_Low"); }
				}
			}
			else if (Commands->Get_Player_Type(obj) == 0)
			{
				if (strcmp(Commands->Get_Preset_Name(obj),"CnC_Nod_Minigunner_0") == 0)
				{
					if (randomcharnod == 1)				{ Change_Character(obj,"TCW_Nod_Militant_Infantry_Low"); }
					else if (randomcharnod == 2)		{ Change_Character(obj,"TCW_Black_Hand_Low"); }
					else if (randomcharnod == 3)		{ Change_Character(obj,"TCW_Nod_RocketGuy_Low"); }
					else if (randomcharnod == 4)		{ Change_Character(obj,"TCW_Nod_Saboteur_Low"); }
					else if (randomcharnod == 5)		{ Change_Character(obj,"TCW_Nod_Shadow_Low"); }
					else if (randomcharnod == 6)		{ Change_Character(obj,"TCW_Nod_Commando_Low"); }
					else								{ Change_Character(obj,"TCW_Nod_Militant_Infantry_Low"); }
				}
			}
		}
		else
		{
			if (Commands->Get_Player_Type(obj) == 1)
			{
				if (strcmp(Commands->Get_Preset_Name(obj),"CnC_GDI_MiniGunner_0") == 0)
				{
					if (randomchargdi == 1)				{ Change_Character(obj,"TCW_GDI_Rifle_Squad"); }
					else if (randomchargdi == 2)		{ Change_Character(obj,"TCW_GDI_Engineer"); }
					else if (randomchargdi == 3)		{ Change_Character(obj,"TCW_GDI_Grenadier"); }
					else if (randomchargdi == 4)		{ Change_Character(obj,"TCW_GDI_Missile_Squad"); }
					else if (randomchargdi == 5)		{ Change_Character(obj,"TCW_GDI_Sniper"); }
					else if (randomchargdi == 6)		{ Change_Character(obj,"TCW_GDI_Commando"); }
					else								{ Change_Character(obj,"TCW_GDI_Zonetrooper"); }
				}
			}
			else if (Commands->Get_Player_Type(obj) == 0)
			{
				if (strcmp(Commands->Get_Preset_Name(obj),"CnC_Nod_Minigunner_0") == 0)
				{
					if (randomchargdi == 1)				{ Change_Character(obj,"TCW_Nod_Militant_Infantry"); }
					else if (randomchargdi == 2)		{ Change_Character(obj,"TCW_Nod_Black_Hand_Flame"); }
					else if (randomchargdi == 3)		{ Change_Character(obj,"TCW_Nod_RocketGuy"); }
					else if (randomchargdi == 4)		{ Change_Character(obj,"TCW_Nod_Saboteur"); }
					else if (randomchargdi == 5)		{ Change_Character(obj,"TCW_Nod_Shadow"); }
					else if (randomchargdi == 6)		{ Change_Character(obj,"TCW_Nod_Commando"); }
					else								{ Change_Character(obj,"TCW_Nod_Militant_Infantry"); }
				}
			}
		}
	}

	upgrade_composit_armor_available = false; upgrade_powerpacks_available = false;
	if (Find_Object_By_Preset(2,"TCW_Tech_Composit_Armor_Available")) {	upgrade_composit_armor_available = true; }
	if (Find_Object_By_Preset(2,"TCW_Tech_Power_Packs_Available")) { upgrade_powerpacks_available = true; }

	int sniperlimit;
	if (map_sniper_only) 
	{ 
		sniperlimit = 100; 
	}
	else sniperlimit = 2;

	commandocurrpos.X = 0;	commandocurrpos.Y = 0;	commandocurrpos.Z = 0;
	commandonewpos.X = 0;	commandonewpos.Y = 0;	commandonewpos.Z = 0;
	revealed = false;
	playdetected = true;
	kills = 0;

	if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Commando") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Commando_Low") == 0) )
	{
		Commands->Enable_Stealth(obj,true);
		commandocurrpos = Commands->Get_Position(obj);
		Commands->Start_Timer(obj,this,1.0f,1006);//check move enable stealth
	}
	else if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Sniper") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Sniper_Low") == 0) )
	{
		if (!map_sniper_only)
		{
			Commands->Enable_Stealth(obj,true);
			commandocurrpos = Commands->Get_Position(obj);
			Commands->Start_Timer(obj,this,1.0f,1006);//check move enable stealth
		}
	}

	//grant weapon stuff when they spawn
	const char *weapon1 = Get_Parameter("Weapon1");	const char *weapon2 = Get_Parameter("Weapon2");	const char *weapon3 = Get_Parameter("Weapon3");	const char *weapon4 = Get_Parameter("Weapon4");	const char *weapon5 = Get_Parameter("Weapon5");
	if (_stricmp(weapon1,"0") != 0)	{ Commands->Give_PowerUp(obj,weapon1,false); }
	if (_stricmp(weapon2,"0") != 0)	{ Commands->Give_PowerUp(obj,weapon2,false); }
	if (_stricmp(weapon3,"0") != 0)	{ Commands->Give_PowerUp(obj,weapon3,false); }
	if (_stricmp(weapon4,"0") != 0)	{ Commands->Give_PowerUp(obj,weapon4,false); }
	if (_stricmp(weapon5,"0") != 0)	{ Commands->Give_PowerUp(obj,weapon5,false); }

	//unit purchase sounds
	int random = Commands->Get_Random_Int(1,6);//used by char purchase playsound
	if ( (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Commando") == 0) || (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Commando_Low") == 0) )
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			if ( (Get_Object_Count(1,"TCW_GDI_Commando") > 1) || (Get_Object_Count(1,"TCW_GDI_Commando_Low") > 1) || (Get_Object_Count(1,"TCW_Nod_Commando") > 1) || (Get_Object_Count(1,"TCW_Nod_Commando_Low") > 1) )
			{
				Commands->Give_Money(obj,2000.0f,false);
				Change_Character(obj,"CnC_GDI_MiniGunner_0");
			}
			else
			{
				Create_2D_WAV_Sound_Team("nod_commandodetected.wav",0);
				Send_Message_Team(0,255,0,0,"Warning, enemy commando detected.");

				int rnd = Commands->Get_Random_Int(1,5);
				if (rnd == 1)
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_commando_1.wav",1);
					char player[250]; sprintf(player,"%s: Got a present for ya.",Get_Player_Name(obj));
					Send_Message_Team(1,255,204,0,player);
				}
				else if (rnd == 2)
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_commando_2.wav",1);
					char player[250]; sprintf(player,"%s: Okay boss let's hear the plan.",Get_Player_Name(obj));
					Send_Message_Team(1,255,204,0,player);
				}
				else if (rnd == 3)
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_commando_5.wav",1);
					char player[250]; sprintf(player,"%s: Let's load and lock and roll and rock.",Get_Player_Name(obj));
					Send_Message_Team(1,255,204,0,player);
				}
				else if (rnd == 4)
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_commando_4.wav",1);
					char player[250]; sprintf(player,"%s: Didn't miss anything, did i?.",Get_Player_Name(obj));
					Send_Message_Team(1,255,204,0,player);
				}
				else
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_commando_5.wav",1);
					char player[250]; sprintf(player,"%s: Let's load and lock and roll and rock.",Get_Player_Name(obj));
					Send_Message_Team(1,255,204,0,player);
				}
			}
		}
		else if (Commands->Get_Player_Type(obj) == 0)
		{
			if ( (Get_Object_Count(0,"TCW_GDI_Commando") > 1) || (Get_Object_Count(0,"TCW_GDI_Commando_Low") > 1) || (Get_Object_Count(0,"TCW_Nod_Commando") > 1) || (Get_Object_Count(0,"TCW_Nod_Commando_Low") > 1) )
			{
				Commands->Give_Money(obj,2000.0f,false);
				Change_Character(obj,"CnC_Nod_Minigunner_0");
			}
			else
			{
				Create_2D_WAV_Sound_Team("gdi_commandodetected.wav",1);
				Send_Message_Team(1,255,204,0,"Warning, enemy commando detected.");

				int rnd = Commands->Get_Random_Int(1,5);
				if (rnd == 1)
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_commando_1.wav",0);
					char player[250]; sprintf(player,"%s: Got a present for ya.",Get_Player_Name(obj));
					Send_Message_Team(0,255,0,0,player);
				}
				else if (rnd == 2)
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_commando_2.wav",0);
					char player[250]; sprintf(player,"%s: Okay boss let's hear the plan.",Get_Player_Name(obj));
					Send_Message_Team(0,255,0,0,player);
				}
				else if (rnd == 3)
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_commando_5.wav",0);
					char player[250]; sprintf(player,"%s: Let's load and lock and roll and rock.",Get_Player_Name(obj));
					Send_Message_Team(0,255,0,0,player);
				}
				else if (rnd == 4)
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_commando_4.wav",0);
					char player[250]; sprintf(player,"%s: Didn't miss anything, did i?.",Get_Player_Name(obj));
					Send_Message_Team(0,255,0,0,player);
				}
				else
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_commando_5.wav",0);
					char player[250]; sprintf(player,"%s: Let's load and lock and roll and rock.",Get_Player_Name(obj));
					Send_Message_Team(0,255,0,0,player);
				}
			}
		}
	}

	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Rifle_Squad") == 0)
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_rifle_squad_1.wav",1);
				char player[250]; sprintf(player,"%s: Keep your weapon ready.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_rifle_squad_2.wav",1);
				char player[250]; sprintf(player,"%s: Reporting in.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_rifle_squad_3.wav",1);
				char player[250]; sprintf(player,"%s: Rifleman awaiting orders.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			if (upgrade_composit_armor_available)
			{
				Set_Max_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj)+75.0f);
				Commands->Set_Shield_Strength(obj,Commands->Get_Health(obj)+75.0f);
			}
		}
		else if (Commands->Get_Player_Type(obj) == 0)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_rifle_squad_1.wav",0);
				char player[250]; sprintf(player,"%s: Keep your weapon ready.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_rifle_squad_2.wav",0);
				char player[250]; sprintf(player,"%s: Rifle squad ready.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_rifle_squad_3.wav",0);
				char player[250]; sprintf(player,"%s: Rifleman awaiting orders.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
		}
	}

	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Rifle_Squad_Low") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_rifle_squad_1.wav",1);
			char player[250]; sprintf(player,"%s: Keep your weapon ready.",Get_Player_Name(obj));
			Send_Message_Team(1,255,204,0,player);
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_rifle_squad_2.wav",1);
			char player[250]; sprintf(player,"%s: Rifle squad ready.",Get_Player_Name(obj));
			Send_Message_Team(1,255,204,0,player);
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_rifle_squad_3.wav",1);
			char player[250]; sprintf(player,"%s: Rifleman awaiting orders.",Get_Player_Name(obj));
			Send_Message_Team(1,255,204,0,player);
		}
		if (upgrade_composit_armor_available)
		{
			Set_Max_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj)+75.0f);
			Commands->Set_Shield_Strength(obj,Commands->Get_Health(obj)+75.0f);
		}
	}

	else if ( (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Engineer") == 0) || (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Engineer_Low") == 0) || (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Engineer_Coop") == 0) )
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_engineer_1.wav",1);
				char player[250]; sprintf(player,"%s: I got their codes.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_engineer_2.wav",1);
				char player[250]; sprintf(player,"%s: Engineer clocking in.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_engineer_3.wav",1);
				char player[250]; sprintf(player,"%s: Engineer reporting in.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
		}
		else if (Commands->Get_Player_Type(obj) == 0)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_engineer_1.wav",0);
				char player[250]; sprintf(player,"%s: I got their codes.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_engineer_2.wav",0);
				char player[250]; sprintf(player,"%s: Engineer clocking in.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_engineer_3.wav",0);
				char player[250]; sprintf(player,"%s: Engineer reporting in.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Grenadier") == 0)
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_grenadier_1.wav",1);
				char player[250]; sprintf(player,"%s: They won't be coming back out.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_grenadier_2.wav",1);
				char player[250]; sprintf(player,"%s: Somebody call for some blasting?",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_grenadier_3.wav",1);
				char player[250]; sprintf(player,"%s: Pull your pins.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			if (upgrade_composit_armor_available)
			{
				Set_Max_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj)+75.0f);
				Commands->Set_Shield_Strength(obj,Commands->Get_Health(obj)+75.0f);
			}
		}
		else if (Commands->Get_Player_Type(obj) == 0)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_grenadier_1.wav",0);
				char player[250]; sprintf(player,"%s: They won't be coming back out.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_grenadier_2.wav",0);
				char player[250]; sprintf(player,"%s: Somebody call for some blasting?",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_grenadier_3.wav",0);
				char player[250]; sprintf(player,"%s: Pull your pins.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Grenadier_Low") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_grenadier_1.wav",1);
			char player[250]; sprintf(player,"%s: They won't be coming back out.",Get_Player_Name(obj));
			Send_Message_Team(1,255,204,0,player);
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_grenadier_2.wav",1);
			char player[250]; sprintf(player,"%s: Somebody call for some blasting?",Get_Player_Name(obj));
			Send_Message_Team(1,255,204,0,player);
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_grenadier_3.wav",1);
			char player[250]; sprintf(player,"%s: Pull your pins.",Get_Player_Name(obj));
			Send_Message_Team(1,255,204,0,player);
		}
		if (upgrade_composit_armor_available)
		{
			Set_Max_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj)+75.0f);
			Commands->Set_Shield_Strength(obj,Commands->Get_Health(obj)+75.0f);
		}
	}		
		
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Missile_Squad") == 0)
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_missile_squad_1.wav",1);
				char player[250]; sprintf(player,"%s: Launcher's ready.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_missile_squad_1.wav",1);
				char player[250]; sprintf(player,"%s: Launcher's ready.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_missile_squad_1.wav",1);
				char player[250]; sprintf(player,"%s: Launcher's ready.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			if (upgrade_composit_armor_available)
			{
				Set_Max_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj)+75.0f);
				Commands->Set_Shield_Strength(obj,Commands->Get_Health(obj)+75.0f);
			}
		}
		else if (Commands->Get_Player_Type(obj) == 0)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_missile_squad_1.wav",0);
				char player[250]; sprintf(player,"%s: Launcher's ready.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_missile_squad_1.wav",0);
				char player[250]; sprintf(player,"%s: Launcher's ready.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_missile_squad_1.wav",0);
				char player[250]; sprintf(player,"%s: Launcher's ready.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Missile_Squad_Low") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_missile_squad_1.wav",1);
			char player[250]; sprintf(player,"%s: Launcher's ready.",Get_Player_Name(obj));
			Send_Message_Team(1,255,204,0,player);
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_missile_squad_1.wav",1);
			char player[250]; sprintf(player,"%s: Launcher's ready.",Get_Player_Name(obj));
			Send_Message_Team(1,255,204,0,player);
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_missile_squad_1.wav",1);
			char player[250]; sprintf(player,"%s: Launcher's ready.",Get_Player_Name(obj));
			Send_Message_Team(1,255,204,0,player);
		}
		if (upgrade_composit_armor_available)
		{
			Set_Max_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj)+75.0f);
			Commands->Set_Shield_Strength(obj,Commands->Get_Health(obj)+75.0f);
		}
	}

	else if ( (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Sniper") == 0) || (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Sniper_Low") == 0) )
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			if ( (Get_Object_Count(1,"TCW_GDI_Sniper") > sniperlimit) || (Get_Object_Count(1,"TCW_GDI_Sniper_Low") > sniperlimit) )
			{
				Commands->Give_Money(obj,1000.0f,false);
				Send_Message_Player(obj,255,204,0,"Sniper limit reached, try again later.");
				Create_2D_WAV_Sound_Player(obj,"incmsg.wav");
				Change_Character(obj,"CnC_GDI_MiniGunner_0");
			}
			else
			{
				if (random == 1)
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_sniper_1.wav",1);
					char player[250]; sprintf(player,"%s: Take 'em out one by one.",Get_Player_Name(obj));
					Send_Message_Team(1,255,204,0,player);
				}
				else if (random == 2)
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_sniper_2.wav",1);
					char player[250]; sprintf(player,"%s: I got'm in my scope.",Get_Player_Name(obj));
					Send_Message_Team(1,255,204,0,player);
				}
				else
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_sniper_3.wav",1);
					char player[250]; sprintf(player,"%s: Need somebody taken out?",Get_Player_Name(obj));
					Send_Message_Team(1,255,204,0,player);
				}
			}
		}
		else if (Commands->Get_Player_Type(obj) == 0)
		{
			if ( (Get_Object_Count(0,"TCW_GDI_Sniper") > sniperlimit) || (Get_Object_Count(1,"TCW_GDI_Sniper_Low") > sniperlimit) )
			{
				Commands->Give_Money(obj,1000.0f,false);
				Send_Message_Player(obj,255,0,0,"Sniper limit reached, try again later.");
				Create_2D_WAV_Sound_Player(obj,"incmsg.wav");
				Change_Character(obj,"CnC_Nod_Minigunner_0");
			}
			else
			{
				if (random == 1)
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_sniper_1.wav",0);
					char player[250]; sprintf(player,"%s: Take 'em out one by one.",Get_Player_Name(obj));
					Send_Message_Team(0,255,0,0,player);
				}
				else if (random == 2)
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_sniper_2.wav",0);
					char player[250]; sprintf(player,"%s: Sniper team ready.",Get_Player_Name(obj));
					Send_Message_Team(0,255,0,0,player);
				}
				else
				{
					Create_2D_WAV_Sound_Team("purchase_gdi_sniper_3.wav",0);
					char player[250]; sprintf(player,"%s: Need somebody taken out?",Get_Player_Name(obj));
					Send_Message_Team(0,255,0,0,player);
				}
			}
		}
	}
	else if ( (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Zonetrooper") == 0) || (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Zonetrooper_Low") == 0) )
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			if (upgrade_powerpacks_available) 
			{ 
				Commands->Start_Timer(obj,this,3.0f,1007); //powerpacks check move
				commandocurrpos = Commands->Get_Position(obj);//using this for checking the distance travel on zonetrooper as well
			}
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_zonetrooper_1.wav",1);
				char player[250]; sprintf(player,"%s: Keep 'em coming.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_zonetrooper_2.wav",1);
				char player[250]; sprintf(player,"%s: Shut this party down.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else if (random == 3)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_zonetrooper_3.wav",1);
				char player[250]; sprintf(player,"%s: Troopers in the field.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_zonetrooper_4.wav",1);
				char player[250]; sprintf(player,"%s: Sit tight.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
		}
		else if (Commands->Get_Player_Type(obj) == 0)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_zonetrooper_1.wav",0);
				char player[250]; sprintf(player,"%s: Keep 'em coming.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_zonetrooper_2.wav",0);
				char player[250]; sprintf(player,"%s: Troopers ready for action.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else if (random == 3)
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_zonetrooper_3.wav",0);
				char player[250]; sprintf(player,"%s: Troopers in the field.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_gdi_zonetrooper_4.wav",0);
				char player[250]; sprintf(player,"%s: Sit tight.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
		}
	}
	else if ( (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Commando") == 0) || (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Commando_Low") == 0) )
	{
		if (Commands->Get_Player_Type(obj) == 0)
		{
			if ( (Get_Object_Count(0,"TCW_Nod_Commando") > 1) || (Get_Object_Count(0,"TCW_Nod_Commando_Low") > 1) || (Get_Object_Count(0,"TCW_GDI_Commando") > 1) || (Get_Object_Count(0,"TCW_GDI_Commando_Low") > 1) )
			{
				Commands->Give_Money(obj,2000.0f,false);
				Change_Character(obj,"CnC_Nod_Minigunner_0");
			}
			else
			{
				Create_2D_WAV_Sound_Team("gdi_commandodetected.wav",1);
				Send_Message_Team(1,255,204,0,"Warning, enemy commando detected.");

				if (random == 1)
				{
					Create_2D_WAV_Sound_Team("purchase_nod_commando_1.wav",0);
					char player[250]; sprintf(player,"%s: Nice day for a kill.",Get_Player_Name(obj));
					Send_Message_Team(0,255,0,0,player);
				}
				else if (random == 2)
				{
					Create_2D_WAV_Sound_Team("purchase_nod_commando_2.wav",0);
					char player[250]; sprintf(player,"%s: It's play time.",Get_Player_Name(obj));
					Send_Message_Team(0,255,0,0,player);
				}
				else
				{
					Create_2D_WAV_Sound_Team("purchase_nod_commando_3.wav",0);
					char player[250]; sprintf(player,"%s: Sorry about the mess.",Get_Player_Name(obj));
					Send_Message_Team(0,255,0,0,player);
				}
			}
		}
		else if (Commands->Get_Player_Type(obj) == 1)
		{
			if ( (Get_Object_Count(1,"TCW_Nod_Commando") > 1) || (Get_Object_Count(1,"TCW_Nod_Commando_Low") > 1) || (Get_Object_Count(1,"TCW_GDI_Commando") > 1) || (Get_Object_Count(1,"TCW_GDI_Commando_Low") > 1) )
			{
				Commands->Give_Money(obj,2000.0f,false);
				Change_Character(obj,"CnC_GDI_MiniGunner_0");
			}
			else
			{
				Create_2D_WAV_Sound_Team("nod_commandodetected.wav",0);
				Send_Message_Team(0,255,0,0,"Warning, enemy commando detected.");

				if (random == 1)
				{
					Create_2D_WAV_Sound_Team("purchase_nod_commando_1.wav",1);
					char player[250]; sprintf(player,"%s: Nice day for a kill.",Get_Player_Name(obj));
					Send_Message_Team(1,255,204,0,player);
				}
				else if (random == 2)
				{
					Create_2D_WAV_Sound_Team("purchase_nod_commando_2.wav",1);
					char player[250]; sprintf(player,"%s: It's play time.",Get_Player_Name(obj));
					Send_Message_Team(1,255,204,0,player);
				}
				else
				{
					Create_2D_WAV_Sound_Team("purchase_nod_commando_3.wav",1);
					char player[250]; sprintf(player,"%s: Sorry about the mess.",Get_Player_Name(obj));
					Send_Message_Team(1,255,204,0,player);
				}
			}
		}
	}
	else if ( (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Militant_Infantry") == 0) || (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Militant_Infantry_Low") == 0) )
	{
		if (Commands->Get_Player_Type(obj) == 0)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_squad_1.wav",0);
				char player[250]; sprintf(player,"%s: Rise up.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_squad_2.wav",0);
				char player[250]; sprintf(player,"%s: The time has come warrior of Nod.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else if (random == 3)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_squad_3.wav",0);
				char player[250]; sprintf(player,"%s: Fighters arm up.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_squad_4.wav",0);
				char player[250]; sprintf(player,"%s: Fight for Nod.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
		}
		else if (Commands->Get_Player_Type(obj) == 1)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_squad_1.wav",1);
				char player[250]; sprintf(player,"%s: Rise up.",Get_Player_Name(obj));
				Send_Message_Team(0,255,204,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_squad_2.wav",1);
				char player[250]; sprintf(player,"%s: The time has come warrior of Nod.",Get_Player_Name(obj));
				Send_Message_Team(0,255,204,0,player);
			}
			else if (random == 3)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_squad_3.wav",1);
				char player[250]; sprintf(player,"%s: Fighters arm up.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_squad_4.wav",1);
				char player[250]; sprintf(player,"%s: Fight for Nod.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
		}
	}
	else if ( (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Black_Hand_Chem") == 0) || (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Black_Hand_Chem_Low") == 0) )
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_blackhand_chem_1.wav",0);
			char player[250]; sprintf(player,"%s: Strenght in tiberium.",Get_Player_Name(obj));
			Send_Message_Team(0,255,0,0,player);
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_blackhand_chem_2.wav",0);
			char player[250]; sprintf(player,"%s: The Black Hand is in control.",Get_Player_Name(obj));
			Send_Message_Team(0,255,0,0,player);
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_blackhand_chem_3.wav",0);
			char player[250]; sprintf(player,"%s: The Black Hand has arrived.",Get_Player_Name(obj));
			Send_Message_Team(0,255,0,0,player);
		}
	}
	else if ( (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Black_Hand_Flame") == 0) || (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Black_Hand_Low") == 0) )
	{
		if (Commands->Get_Player_Type(obj) == 0)
		{
			int rnd = Commands->Get_Random_Int(1,5);
			if (rnd == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_blackhand_flame_1.wav",0);
				char player[250]; sprintf(player,"%s: One mission. One purpose.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else if (rnd == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_blackhand_flame_2.wav",0);
				char player[250]; sprintf(player,"%s: Exterminate the weak.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else if (rnd == 3)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_blackhand_flame_3.wav",0);
				char player[250]; sprintf(player,"%s: They cannot hide from the Black Hand.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_nod_blackhand_flame_4.wav",0);
				char player[250]; sprintf(player,"%s: Spare no one.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
		}
		else if (Commands->Get_Player_Type(obj) == 1)
		{
			int rnd = Commands->Get_Random_Int(1,5);
			if (rnd == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_blackhand_flame_1.wav",1);
				char player[250]; sprintf(player,"%s: One mission. One purpose.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else if (rnd == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_blackhand_flame_2.wav",1);
				char player[250]; sprintf(player,"%s: Purifier ready.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else if (rnd == 3)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_blackhand_flame_3.wav",1);
				char player[250]; sprintf(player,"%s: The flame shall purify.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_nod_blackhand_flame_4.wav",1);
				char player[250]; sprintf(player,"%s: Spare no one.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
		}
	}
	else if ( (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_RocketGuy") == 0) || (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_RocketGuy_Low") == 0) )
	{
		if (Commands->Get_Player_Type(obj) == 0)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_rocket_squad_1.wav",0);
				char player[250]; sprintf(player,"%s: If it moves: Kill it.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_rocket_squad_2.wav",0);
				char player[250]; sprintf(player,"%s: Rockets ready.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_rocket_squad_3.wav",0);
				char player[250]; sprintf(player,"%s: We got the rockets.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
		}
		else if (Commands->Get_Player_Type(obj) == 1)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_rocket_squad_1.wav",1);
				char player[250]; sprintf(player,"%s: If it moves: Kill it.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_rocket_squad_2.wav",1);
				char player[250]; sprintf(player,"%s: Rockets ready.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_nod_militant_rocket_squad_3.wav",1);
				char player[250]; sprintf(player,"%s: We got the rockets.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
		}
	}
	else if ( (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Saboteur") == 0) || (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Saboteur_Low") == 0) )
	{
		if (Commands->Get_Player_Type(obj) == 0)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_saboteur_1.wav",0);
				char player[250]; sprintf(player,"%s: The Brotherhood has entrusted me.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_saboteur_2.wav",0);
				char player[250]; sprintf(player,"%s: Need help with anything?",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_nod_saboteur_3.wav",0);
				char player[250]; sprintf(player,"%s: You can trust me.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
		}
		if (Commands->Get_Player_Type(obj) == 1)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_saboteur_1.wav",1);
				char player[250]; sprintf(player,"%s: The Brotherhood has entrusted me.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_saboteur_2.wav",1);
				char player[250]; sprintf(player,"%s: Need help with anything?",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_nod_saboteur_3.wav",1);
				char player[250]; sprintf(player,"%s: You can trust me.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
		}
	}
	else if ( (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Shadow") == 0) || (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Shadow_Low") == 0) )
	{
		if (Commands->Get_Player_Type(obj) == 0)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_shadow_1.wav",0);
				char player[250]; sprintf(player,"%s: Quietly.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_shadow_2.wav",0);
				char player[250]; sprintf(player,"%s: The wings of Nod.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_nod_shadow_2.wav",0);
				char player[250]; sprintf(player,"%s: The wings of Nod.",Get_Player_Name(obj));
				Send_Message_Team(0,255,0,0,player);
			}
		}
		else if (Commands->Get_Player_Type(obj) == 1)
		{
			if (random == 1)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_shadow_1.wav",1);
				char player[250]; sprintf(player,"%s: Quietly.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else if (random == 2)
			{
				Create_2D_WAV_Sound_Team("purchase_nod_shadow_2.wav",1);
				char player[250]; sprintf(player,"%s: The wings of Nod.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
			else
			{
				Create_2D_WAV_Sound_Team("purchase_nod_shadow_2.wav",1);
				char player[250]; sprintf(player,"%s: The wings of Nod.",Get_Player_Name(obj));
				Send_Message_Team(1,255,204,0,player);
			}
		}
	}
}
void tcw_Grant_Powerup_Created::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1004)//enable stealth again after distance check 
	{
		Commands->Enable_Stealth(obj, true);
		revealed = false;
	}
	else if (number == 1005)
	{
		playdetected = true;
	}
	else if (number == 1006)//used for stealthing Nod Commando and GDI Sniper
	{
		commandonewpos = Commands->Get_Position(obj);
		float commandotraveldistance = Commands->Get_Distance(commandocurrpos,commandonewpos);
		if (commandotraveldistance <= 0.1f)	// not moved...
		{
			Commands->Start_Timer(obj,this,1.0f,1006);
			Commands->Enable_Stealth(obj, true);
		}
		else 
		{
			commandocurrpos = Commands->Get_Position(obj);//new current position, moved..
			Commands->Enable_Stealth(obj, false);
			Commands->Start_Timer(obj,this,1.0f,1006);
		}
	}
	else if (number == 1007)//used for zonetrooper repair powerpacks
	{
		if (!Get_Vehicle(obj))
		{
			commandonewpos = Commands->Get_Position(obj);
			float zonetroopertraveldistance = Commands->Get_Distance(commandocurrpos,commandonewpos);
			if (zonetroopertraveldistance <= 0.1f)	// not moved...
			{
				if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Zonetrooper") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Zonetrooper_Low") == 0) && (Commands->Get_Player_Type(obj) == 1))
				{
					float maxhp = Commands->Get_Max_Health(obj);
					float maxap = Commands->Get_Max_Shield_Strength(obj);
					float curhp = Commands->Get_Health(obj);
					float curap = Commands->Get_Shield_Strength(obj);
					if (curhp < maxhp)
					{
						Commands->Set_Health(obj,curhp+5.0f);
					}
					if (curap < maxap)
					{
						Commands->Set_Shield_Strength(obj,curap+5.0f);
					}
					Commands->Start_Timer(obj,this,3.0f,1007);
				}
			}
			else
			{
				commandocurrpos = Commands->Get_Position(obj);//new current position, moved..
				Commands->Start_Timer(obj,this,3.0f,1007);
			}
		}
		else
		{
			Commands->Start_Timer(obj,this,3.0f,1007);
		}
	}
}
void tcw_Grant_Powerup_Created::Killed(GameObject *obj, GameObject *shooter)
{
	//placeholder
}
void tcw_Grant_Powerup_Created::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1003)
	{
		if (param == 1)
		{
			if (Commands->Get_ID(sender) != Commands->Get_ID(obj))
			{
				kills++;
				if (kills == 2)
				{
					//double kill
					Create_2D_WAV_Sound_Player(obj,"quake_doublekill.wav");
					Send_Message_Player(obj,104,234,40,"Achieved Double Kill!");
				}
				if (kills == 3)
				{
					//multikill
					Create_2D_WAV_Sound_Player(obj,"quake_multikill.wav");
					Send_Message_Player(obj,104,234,40,"Achieved Multikill!");
				}
				else if (kills == 5)
				{
					//killspree
					char msg2[250];
					sprintf(msg2,"%s achieved Killing Spree (5 kills)",Get_Player_Name(obj));
					Commands->Create_2D_WAV_Sound("quake_killingspree.wav");
					Send_Message(255,255,255,msg2);
				}
				else if (kills == 7)
				{
					//rampage
					Create_2D_WAV_Sound_Player(obj,"quake_rampage.wav");
					Send_Message_Player(obj,104,234,40,"Achieved Rampage!");
				}
				else if (kills == 9)
				{
					//ultra
					Create_2D_WAV_Sound_Player(obj,"quake_ultrakill.wav");
					Send_Message_Player(obj,104,234,40,"Achieved UltraKill!");
				}
				else if (kills == 11)
				{
					//holyshit
					char msg[250];
					sprintf(msg,"%s achieved Holy Shit!!! (11 kills)",Get_Player_Name(obj));
					Commands->Create_2D_WAV_Sound("quake_holyshit.wav");
					Send_Message(255,255,255,msg);
				}
				else if (kills == 13)
				{
					//monster
					char msg2[250];
					sprintf(msg2,"%s achieved M-M-M-M-Monster Kill!!! (13 kills)",Get_Player_Name(obj));
					Commands->Create_2D_WAV_Sound("quake_monsterkill.wav");
					Send_Message(255,255,255,msg2);
				}
				else if (kills == 15)
				{
					//godlike
					char msg3[250];
					sprintf(msg3,"%s achieved GODLIKE!!! (15 kills)",Get_Player_Name(obj));
					Commands->Create_2D_WAV_Sound("quake_godlike.wav");
					Send_Message(255,255,255,msg3);
				}
			}
		}
	}
	else if (message == 1004)//destealth unit
	{ 
		if (param == 1)
		{
			if (strcmp(Commands->Get_Preset_Name(sender),"TCW_GDI_Command_Post") == 0)
			{
				if (Commands->Get_Player_Type(obj) == 0)//only check nod units when the command post checks
				{
					if (!revealed)
					{
						Commands->Enable_Stealth(obj, false);
						Commands->Start_Timer(obj,this,2.0f,1004);//enable stealh again
						if (playdetected)
						{
							if (Commands->Get_Player_Type(obj) == 0)
							{
								Create_2D_WAV_Sound_Team("stealth_unit_detected_gdi.wav",1);
								Send_Message_Team(1,255,204,0,"Stealth unit detected.");
							}
							else if (Commands->Get_Player_Type(obj) == 1)
							{
								Create_2D_WAV_Sound_Team("stealth_unit_detected_nod.wav",0);
								Send_Message_Team(0,255,0,0,"Stealth unit detected.");
							}
							playdetected = false;
							Commands->Start_Timer(obj,this,10.0f,1005);//playdetected true
						}	
						revealed = true;
					}
				}
			}
			else
			{
				if (Commands->Get_Player_Type(sender) != Commands->Get_Player_Type(obj))
				{
					Vector3 mypos = Commands->Get_Position(obj);
					Vector3 target = Commands->Get_Position(sender);
					float distance = Commands->Get_Distance(mypos,target);

					if (distance < 51.0f)
					{
						if (!revealed)
						{
							Commands->Enable_Stealth(obj, false);
							Commands->Start_Timer(obj,this,2.0f,1004);//enable stealh again
							if (playdetected)
							{
								if (Commands->Get_Player_Type(obj) == 0)
								{
									Create_2D_WAV_Sound_Team("stealth_unit_detected_gdi.wav",1);
									Send_Message_Team(1,255,204,0,"Stealth unit detected.");
								}
								else if (Commands->Get_Player_Type(obj) == 1)
								{
									Create_2D_WAV_Sound_Team("stealth_unit_detected_nod.wav",0);
									Send_Message_Team(0,255,0,0,"Stealth unit detected.");
								}
								playdetected = false;
								Commands->Start_Timer(obj,this,10.0f,1005);//playdetected true
							}	
							revealed = true;
						}
					}
				}
			}
		}
	}
	else if (message == 1005)//Tech Composit Armor Upgrade complete
	{
		if (param == 1)
		{
			float maxhp = Commands->Get_Max_Health(obj);
			float maxap = Commands->Get_Max_Shield_Strength(obj);
			float curhp = Commands->Get_Health(obj);
			float curap = Commands->Get_Shield_Strength(obj);
			float maxhpnew = maxhp + 30.0f; Set_Max_Health(obj, maxhpnew);
			float maxapnew = maxap + 30.0f; Set_Max_Shield_Strength(obj, maxapnew);
			float newhp = curhp + 30.0f; Commands->Set_Health(obj, newhp);
			float newap = curap + 30.0f; Commands->Set_Shield_Strength(obj, newap);
			Create_2D_WAV_Sound_Player(obj,"gdi_tech_unit_upgraded.wav");
			Send_Message_Player(obj,5,220,220,"Unit Upgraded: Composite Armor.");
		}
	}
	else if (message == 1006)//Tech Powerpacks Upgrade complete
	{
		if (param == 1)
		{
			commandocurrpos = Commands->Get_Position(obj);
			Commands->Start_Timer(obj,this,1.0f,1007);//start regeneration if not moved
			Create_2D_WAV_Sound_Player(obj,"gdi_tech_unit_upgraded.wav");
			Send_Message_Player(obj,5,220,220,"Unit Upgraded: Power Packs.");
		}
	}
}
void tcw_Grant_Powerup_Created::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&kills,sizeof(kills),1);
	Auto_Save_Variable(&revealed,sizeof(revealed),2);
	Auto_Save_Variable(&playdetected,sizeof(playdetected),3);
	Auto_Save_Variable(&commandocurrpos,sizeof(commandocurrpos),4);
	Auto_Save_Variable(&commandonewpos,sizeof(commandonewpos),5);
	Auto_Save_Variable(&map_low_grav,sizeof(map_low_grav),6);
	Auto_Save_Variable(&map_dm_tech1,sizeof(map_dm_tech1),7);
	Auto_Save_Variable(&map_dm_tech2,sizeof(map_dm_tech2),8);
	Auto_Save_Variable(&map_dm_tech3,sizeof(map_dm_tech3),9);
	Auto_Save_Variable(&map_coop_gdi,sizeof(map_coop_gdi),10);
	Auto_Save_Variable(&map_coop_nod,sizeof(map_coop_nod),11);
}

void tcw_fanatic::Created(GameObject *obj)
{
	if (!Commands->Is_A_Star(obj))
	{
		Destroy_Script();
		return;
	}
	DetonateTime = Get_Float_Parameter("DetonateTime");
	RunSpeed = Get_Float_Parameter("RunSpeed");
	Explosion = Get_Parameter("Explosion");
	InstallHook("FanaticExplode",obj);
}

void tcw_fanatic::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		Create_Explosion_Extended(Explosion,Commands->Get_Position(obj),obj);
	}
}

void tcw_fanatic::KeyHook()
{
	RemoveHook();
	Owner()->As_SoldierGameObj()->Set_Max_Speed(RunSpeed);
	Owner()->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
	Commands->Start_Timer(Owner(),this,DetonateTime,1);
}

ScriptRegistrant<tcw_fanatic> tcw_fanatic_Registrant("tcw_fanatic","DetonateTime=30:float,RunSpeed=5:float,Explosion:string");
ScriptRegistrant<tcw_Grant_Powerup_Created> tcw_Grant_Powerup_Created_Registrant("tcw_Grant_Powerup_Created","Weapon1=TCW_POW_Knife:string,Weapon2=0:string,Weapon3=0:string,Weapon4=0:string,Weapon5=0:string");