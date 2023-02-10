/*	Renegade Scripts.dll
	Copyright 2012 UltraAOW.COM

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
#include "z_nod_invasion.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"

bool Invasion_Botcount_Nod(int count)
{
	int Total_Flamerguys = Get_Object_Count(0,"Bot_Nod_Flamerguy");
	int Total_Chemwarriors = Get_Object_Count(0,"Bot_Nod_Chemwarrior");
	int Total_SBHs = Get_Object_Count(0,"Bot_Nod_SBH");
	int Total_Mendoza = Get_Object_Count(0,"Bot_Nod_Mendoza");
	int Total_Mendoza2 = Get_Object_Count(0,"Bot_Nod_Mendoza_ALT2");
	int Total_Minigunners = Get_Object_Count(0,"Bot_Nod_Minigunner");
	int Total_Officers = Get_Object_Count(0,"Bot_Nod_Officer");
	int Total_Snipers = Get_Object_Count(0,"Bot_Nod_Sniper");
	int Total_Sakuras = Get_Object_Count(0,"Bot_Nod_Sakura");
	int Total_Sakuras2 = Get_Object_Count(0,"Bot_Nod_Sakura_ALT2");
	int Total_Shotgunners = Get_Object_Count(0,"Bot_Nod_Shotgunner");
	int Total_Rocketguys = Get_Object_Count(0,"Bot_Nod_Rocket");
	int Total_LCGs = Get_Object_Count(0,"Bot_Nod_Laserchaingunner");
	int Total_Raves = Get_Object_Count(0,"Bot_Nod_Raveshaw");
	if(Total_Flamerguys+Total_Chemwarriors+Total_SBHs+Total_Mendoza+Total_Mendoza2+Total_Minigunners+Total_Officers+Total_Snipers+Total_Sakuras+Total_Sakuras2+Total_Shotgunners+Total_Rocketguys+Total_LCGs+Total_Raves <= count)
	{
		return true;
	}
	else return false;
}

void z_Poke_Play_Cinematic::Created(GameObject *obj)
{
	allowpoke = true;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void z_Poke_Play_Cinematic::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		allowpoke = false;
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Commands->Start_Timer(obj,this,2.0f,1000);//do cinematic
	}
}
void z_Poke_Play_Cinematic::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1000)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj,true);
		const char *Cinematic = Get_Parameter("Cinematic");
		int dropid = Get_Int_Parameter("DropID");
		Vector3 pos = Commands->Get_Position(Commands->Find_Object(dropid));
		float fac = Get_Float_Parameter("Facing");
		GameObject *dropoff = Commands->Create_Object("Invisible_Object",pos);
		Commands->Set_Facing(dropoff,fac);
		Commands->Attach_Script(dropoff,"Test_Cinematic",Cinematic);
		Commands->Create_2D_WAV_Sound("airraid.wav");
	}
}
void z_Poke_Play_Cinematic::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowpoke,sizeof(allowpoke),1);
}

void z_Invasion_Hostage::Created(GameObject *obj)
{
	FollowID = 0;
	playpoke = true;
	playpain = true;
	hostypokesound = 0;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	hostyname = Get_Parameter("Hostage_Name");

}
void z_Invasion_Hostage::Poked(GameObject *obj, GameObject *poker)
{
	if (playpoke)
	{
		playpoke = false;	Commands->Start_Timer(obj,this,10.0f,1000);
		char hostymsg[512];
		int random = Commands->Get_Random_Int(1,6);
		if (random == 1) {				sprintf(hostymsg,"Hostage %s: Let's get the hell outta here",hostyname);		hostypokesound = "hgetouttahere.wav";	}
		else if (random == 2) {			sprintf(hostymsg,"Hostage %s: Okay i'll follow you",hostyname);	hostypokesound = "hillfollow.wav";		}
		else if (random == 3) {			sprintf(hostymsg,"Hostage %s: Let's go",hostyname);	hostypokesound = "hletsgo.wav";		}
		else if (random == 4) {			sprintf(hostymsg,"Hostage %s: Okay let's hurry",hostyname);	hostypokesound = "hletshurry.wav";		}
		else if (random == 5) {			sprintf(hostymsg,"Hostage %s: Right let's move",hostyname);	hostypokesound = "hletsmove.wav";		}
		else {							sprintf(hostymsg,"Hostage %s: Okay let's go",hostyname);	hostypokesound = "hokletsgo.wav";		}
		Send_Message_Player(poker,255,204,0,hostymsg);
		Create_2D_WAV_Sound_Player(poker,hostypokesound);
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
	}
}
void z_Invasion_Hostage::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1000)
	{
		playpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj,true);
	}
	else if (number == 1001)
	{
		playpain = true;
	}
}
void z_Invasion_Hostage::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (damage > 0.0f)
	{
		if (playpain)
		{
			const char *painsnd = "0";
			playpain = false; Commands->Start_Timer(obj,this,5.0f,1001);
			int lol = Commands->Get_Random_Int(1,8);
			if (lol == 1) { painsnd = "hpain1.wav"; }
			else if (lol == 2)  { painsnd = "hpain2.wav"; }
			else if (lol == 3)  { painsnd = "hpain3.wav"; }
			else if (lol == 4)  { painsnd = "hpain4.wav"; }
			else if (lol == 5)  { painsnd = "hpain5.wav"; }
			else				{ painsnd = "hpain6.wav"; }
			Commands->Create_2D_WAV_Sound(painsnd);
		}
	}
}
void z_Invasion_Hostage::Destroyed(GameObject *obj)
{
	Commands->Create_2D_WAV_Sound("hostydown.wav");
	char killmsg[512];	sprintf(killmsg,"Hostage down: %s",hostyname);	Send_Message(255,204,0,killmsg);
	GameObject *receiver = Find_Object_With_Script("z_Invasion_Hostage_Count");
	Commands->Send_Custom_Event(obj,receiver,1000,1,0);
}
void z_Invasion_Hostage::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&playpoke,sizeof(playpoke),1);
	Auto_Save_Variable(&playpain,sizeof(playpain),2);
	Auto_Save_Variable(&hostyname,sizeof(hostyname),3);
	Auto_Save_Variable(&hostypokesound,sizeof(hostypokesound),4);
	Auto_Save_Variable(&FollowID,sizeof(FollowID),5);
}

void z_Invasion_Hostage_Count::Created(GameObject *obj)
{
	deaths = 0;
	Send_Message(255,204,0,"Protect the Hostages at all costs");
	Commands->Start_Timer(obj,this,90.0f,2003);
}
void z_Invasion_Hostage_Count::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			deaths++;
			if (deaths == 4)
			{
				Commands->Start_Timer(obj,this,3.0f,2000);
			}
		}
	}
}
void z_Invasion_Hostage_Count::Timer_Expired(GameObject *obj, int number)
{
	if (number == 2000)
	{
		Console_Input("amsg Mission Failed: All hostages were terminated");
		Commands->Start_Timer(obj,this,5.0f,2001);
	}
	else if (number == 2001)
	{
		Send_Message(255,204,0,"Your mission is a failure");
		Commands->Create_2D_WAV_Sound("00-n100e.wav");
		Commands->Start_Timer(obj,this,2.0f,2002);
		for (SLNode<cPlayer>* PlayerIter = Get_Player_List()->Head(); (PlayerIter != NULL); PlayerIter = PlayerIter->Next())
		{
			cPlayer *p = PlayerIter->Data();
			if (p->IsActive)
			{		
				int id = p->PlayerId;
				if (Get_Team(id) == 1)
				{
					GameObject *player = Get_GameObj(id);
					if (Commands->Is_A_Star(player))
					{
						Commands->Apply_Damage(player,99999.0f,"Death",0);
					}
				}
			}
		}
	}
	else if (number == 2002)
	{
		Console_Input("win 0");
	}
	else if (number == 2003)
	{
		Send_Message(255,204,0,"Protect the Hostages at all costs");
		Commands->Start_Timer(obj,this,90.0f,2003);
	}
}
void z_Invasion_Hostage_Count::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&deaths,sizeof(deaths),1);
}

void z_Invasion_Bot_Counter::Created(GameObject *obj)
{
	botcount = 0;
	endgame = false;
}
void z_Invasion_Bot_Counter::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)//bot created
	{
		if (param == 1)
		{
			botcount++;
		}
	}
	else if (message == 2000)//bot died
	{
		if (param == 1)
		{
			botcount--;
			if (botcount == 0)
			{
				if (endgame)
				{
					Commands->Start_Timer(obj,this,1.0f,1000);
				}
				else Commands->Start_Timer(obj,this,1.0f,1001);
			}
		}
	}
	else if (message == 3000)//planedrop done
	{
		if (param == 1)
		{
			endgame = true;
		}
	}
}
void z_Invasion_Bot_Counter::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1000)
	{
		Console_Input("amsg Mission accomplished");
		Console_Input("win 1");
	}
	else if (number == 1001)
	{
		if (botcount == 0)
		{
			if (endgame)
			{
				Commands->Start_Timer(obj,this,1.0f,1000);
			}
			else Commands->Start_Timer(obj,this,5.0f,1001);
		}
	}
}
void z_Invasion_Bot_Counter::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&botcount,sizeof(botcount),1);
	Auto_Save_Variable(&endgame,sizeof(endgame),2);
}

void z_Invasion_Enemy::Created(GameObject *obj)
{
	GameObject *recr = Find_Object_With_Script("z_Invasion_Bot_Counter");
	Commands->Send_Custom_Event(obj,recr,1000,1,0);
}
void z_Invasion_Enemy::Destroyed(GameObject *obj)
{
	GameObject *recvr = Find_Object_With_Script("z_Invasion_Bot_Counter");
	Commands->Send_Custom_Event(obj,recvr,2000,1,0);

	int random = Commands->Get_Random_Int(1,13);
	if (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Engineer_Stationary") == 0)
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

	else if (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Flamerguy") == 0)
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

	else if (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Chemwarrior") == 0)
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

	else if (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_SBH") == 0)
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

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Mendoza") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Mendoza_ALT2") == 0) )
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("CnC_POW_VoltAutoRifle_Player_Nod",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("CnC_POW_VoltAutoRifle_Player_Nod",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("CnC_POW_VoltAutoRifle_Player_Nod",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
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

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Sakura") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Sakura_ALT2") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M01_SakuraBot") == 0) )
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

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Rocket") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Rocketguy_Star") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Rocketguy_Stationary") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M00_Nod_RocketGuy") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"M01_NodRocketGuy") == 0) )
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
	
	else if (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Raveshaw") == 0)
	{
		if (random == 1)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			 powerup = Commands->Create_Object("POW_Railgun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 2)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Railgun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
		}
		else if (random == 3)
		{
						GameObject *powerup;
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.25f;
			powerup = Commands->Create_Object("POW_Railgun_Player",pos);	Attach_Script_Once(powerup,"z_POW_PickupSounds","");	Attach_Script_Once(powerup,"M00_Powerup_Destroy","");
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

	else if ( (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Tech_Repair") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Technician") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Bot_Nod_Tech_Stationary") == 0) ||
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

void z_Invasion_Drop_Harvester::Killed(GameObject *obj, GameObject *shooter)
{
	Vector3 loc; loc.X = 44.034f; loc.Y = -19.332f; loc.Z = 2.442f;	float facing = 63.327f;
	const char *Cinematic = "nod_inv_harvdrop.txt";
	GameObject *dropoff = Commands->Create_Object("Invisible_Object",loc);
	Commands->Set_Facing(dropoff,facing);
	Commands->Attach_Script(dropoff,"Test_Cinematic",Cinematic);
	Commands->Create_2D_WAV_Sound("m00evag_dsgn0060i1evag_snd.wav");
	Send_Message(255,204,0,"Reinforcements are enroute");
}

void z_Invasion_Loop_Badgerdrop::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,30.0f,999);
	Commands->Start_Timer(obj,this,60.0f,999);
	Commands->Start_Timer(obj,this,300.0f,1000);
	count = 0;
}
void z_Invasion_Loop_Badgerdrop::Timer_Expired(GameObject *obj, int number)
{
	if (number == 999)
	{
		Send_Message(255,204,0,"You have 5 minutes to build your defenses");
		Send_Message(255,204,0,"You have 5 minutes to build your defenses");
		Send_Message(255,204,0,"You have 5 minutes to build your defenses");
		Send_Message(255,204,0,"You have 5 minutes to build your defenses");
	}
	else if (number == 1000)
	{
		Commands->Start_Timer(obj,this,200.0f,1000);
		if (Invasion_Botcount_Nod(55))
		{
			const char *cinematic = "paradrop.txt";
			int dropid1 = 100016; int dropid2 = 100017; int dropid3 = 100018;
			Vector3 pos1 = Commands->Get_Position(Commands->Find_Object(dropid1));
			Vector3 pos2 = Commands->Get_Position(Commands->Find_Object(dropid2));
			Vector3 pos3 = Commands->Get_Position(Commands->Find_Object(dropid3));
			float fac1 = 0.0f; float fac2 = 90.0f; float fac3 = 180.0f;
			GameObject *dropoff1 = Commands->Create_Object("Invisible_Object",pos1);
			GameObject *dropoff2 = Commands->Create_Object("Invisible_Object",pos2);
			GameObject *dropoff3 = Commands->Create_Object("Invisible_Object",pos3);
			Commands->Set_Facing(dropoff1,fac1);
			Commands->Set_Facing(dropoff2,fac2);
			Commands->Set_Facing(dropoff3,fac3);
			Commands->Attach_Script(dropoff1,"Test_Cinematic",cinematic);
			Commands->Attach_Script(dropoff2,"Test_Cinematic",cinematic);
			Commands->Attach_Script(dropoff3,"Test_Cinematic",cinematic);
			Commands->Create_2D_WAV_Sound("airraid.wav");
			Commands->Create_2D_WAV_Sound("airraid.wav");
			count++;
			if (count > 1)
			{
				char countmsg[512];
				sprintf(countmsg,"Wave %d of 15",count);
				Send_Message(255,204,0,countmsg);
				Send_Message(255,204,0,countmsg);
				Send_Message(255,204,0,countmsg);
				Send_Message(255,204,0,countmsg);
			}
			if (count == 3)
			{
				Commands->Enable_Spawner(100102,true);
				Commands->Enable_Spawner(100105,true);
				Commands->Enable_Spawner(100157,true);
				Commands->Enable_Spawner(100158,true);
				Commands->Enable_Spawner(100159,true);
				Commands->Enable_Spawner(100160,true);
				Commands->Enable_Spawner(100161,true);
				Commands->Enable_Spawner(100162,true);
				Commands->Enable_Spawner(100163,true);
				Commands->Enable_Spawner(100164,true);
				Commands->Enable_Spawner(100165,true);
				Commands->Enable_Spawner(100166,true);
				Commands->Enable_Spawner(100167,true);
				Commands->Enable_Spawner(100168,true);
				Commands->Enable_Spawner(100169,true);
				Commands->Enable_Spawner(100170,true);
				Commands->Enable_Spawner(100171,true);
				Commands->Enable_Spawner(100172,true);
				Commands->Enable_Spawner(100173,true);
				Commands->Enable_Spawner(100174,true);
				Commands->Enable_Spawner(100175,true);
			}
			else if (count == 5)
			{
				Commands->Enable_Spawner(100108,true);
				Commands->Enable_Spawner(100139,true);
				Commands->Enable_Spawner(100140,true);
				Commands->Enable_Spawner(100141,true);
				Commands->Enable_Spawner(100142,true);
				Commands->Enable_Spawner(100143,true);
				Commands->Enable_Spawner(100144,true);
				Commands->Enable_Spawner(100145,true);
				Commands->Enable_Spawner(100146,true);
				Commands->Enable_Spawner(100147,true);
				Commands->Enable_Spawner(100148,true);
				Commands->Enable_Spawner(100149,true);
				Commands->Enable_Spawner(100150,true);
				Commands->Enable_Spawner(100151,true);
				Commands->Enable_Spawner(100152,true);
				Commands->Enable_Spawner(100153,true);
				Commands->Enable_Spawner(100154,true);
				Commands->Enable_Spawner(100155,true);
				Commands->Enable_Spawner(100156,true);			
			}
			else if (count == 7)
			{
				Commands->Enable_Spawner(100112,true);
				Commands->Enable_Spawner(100113,true);
				Commands->Enable_Spawner(100125,true);
				Commands->Enable_Spawner(100126,true);
				Commands->Enable_Spawner(100127,true);
				Commands->Enable_Spawner(100128,true);
				Commands->Enable_Spawner(100129,true);
				Commands->Enable_Spawner(100130,true);
				Commands->Enable_Spawner(100131,true);
				Commands->Enable_Spawner(100132,true);
				Commands->Enable_Spawner(100133,true);
				Commands->Enable_Spawner(100134,true);
				Commands->Enable_Spawner(100135,true);
				Commands->Enable_Spawner(100136,true);
				Commands->Enable_Spawner(100137,true);
				Commands->Enable_Spawner(100138,true);
			}
			else if (count == 9)
			{
				Commands->Enable_Spawner(100114,true);
				Commands->Enable_Spawner(100115,true);
				Commands->Enable_Spawner(100116,true);
				Commands->Enable_Spawner(100117,true);
				Commands->Enable_Spawner(100118,true);
				Commands->Enable_Spawner(100119,true);
				Commands->Enable_Spawner(100120,true);
				Commands->Enable_Spawner(100121,true);
				Commands->Enable_Spawner(100122,true);
				Commands->Enable_Spawner(100123,true);
				Commands->Enable_Spawner(100124,true);
			}
			else if (count == 15)
			{
				GameObject *ctrl = Find_Object_With_Script("z_Invasion_Bot_Counter");
				Commands->Send_Custom_Event(obj,ctrl,3000,1,0);//endgame true
				Commands->Destroy_Object(Commands->Find_Object(100099));//chinook loops
				Commands->Enable_Spawner(100102,false);
				Commands->Enable_Spawner(100105,false);
				Commands->Enable_Spawner(100114,false);
				Commands->Enable_Spawner(100115,false);
				Commands->Enable_Spawner(100116,false);
				Commands->Enable_Spawner(100117,false);
				Commands->Enable_Spawner(100118,false);
				Commands->Enable_Spawner(100119,false);
				Commands->Enable_Spawner(100120,false);
				Commands->Enable_Spawner(100121,false);
				Commands->Enable_Spawner(100122,false);
				Commands->Enable_Spawner(100123,false);
				Commands->Enable_Spawner(100124,false);
				Commands->Enable_Spawner(100112,false);
				Commands->Enable_Spawner(100113,false);
				Commands->Enable_Spawner(100125,false);
				Commands->Enable_Spawner(100126,false);
				Commands->Enable_Spawner(100127,false);
				Commands->Enable_Spawner(100128,false);
				Commands->Enable_Spawner(100129,false);
				Commands->Enable_Spawner(100130,false);
				Commands->Enable_Spawner(100131,false);
				Commands->Enable_Spawner(100132,false);
				Commands->Enable_Spawner(100133,false);
				Commands->Enable_Spawner(100134,false);
				Commands->Enable_Spawner(100135,false);
				Commands->Enable_Spawner(100136,false);
				Commands->Enable_Spawner(100137,false);
				Commands->Enable_Spawner(100138,false);
				Commands->Enable_Spawner(100108,false);
				Commands->Enable_Spawner(100139,false);
				Commands->Enable_Spawner(100140,false);
				Commands->Enable_Spawner(100141,false);
				Commands->Enable_Spawner(100142,false);
				Commands->Enable_Spawner(100143,false);
				Commands->Enable_Spawner(100144,false);
				Commands->Enable_Spawner(100145,false);
				Commands->Enable_Spawner(100146,false);
				Commands->Enable_Spawner(100147,false);
				Commands->Enable_Spawner(100148,false);
				Commands->Enable_Spawner(100149,false);
				Commands->Enable_Spawner(100150,false);
				Commands->Enable_Spawner(100151,false);
				Commands->Enable_Spawner(100152,false);
				Commands->Enable_Spawner(100153,false);
				Commands->Enable_Spawner(100154,false);
				Commands->Enable_Spawner(100155,false);
				Commands->Enable_Spawner(100156,false);
				Commands->Enable_Spawner(100157,false);
				Commands->Enable_Spawner(100158,false);
				Commands->Enable_Spawner(100159,false);
				Commands->Enable_Spawner(100160,false);
				Commands->Enable_Spawner(100161,false);
				Commands->Enable_Spawner(100162,false);
				Commands->Enable_Spawner(100163,false);
				Commands->Enable_Spawner(100164,false);
				Commands->Enable_Spawner(100165,false);
				Commands->Enable_Spawner(100166,false);
				Commands->Enable_Spawner(100167,false);
				Commands->Enable_Spawner(100168,false);
				Commands->Enable_Spawner(100169,false);
				Commands->Enable_Spawner(100170,false);
				Commands->Enable_Spawner(100171,false);
				Commands->Enable_Spawner(100172,false);
				Commands->Enable_Spawner(100173,false);
				Commands->Enable_Spawner(100174,false);
				Commands->Enable_Spawner(100175,false);	
				Commands->Destroy_Object(obj);
			}
		}
	}
}
void z_Invasion_Loop_Badgerdrop::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&count,sizeof(count),1);
}

void z_Invasion_Drop_Hummvee1::Killed(GameObject *obj, GameObject *shooter)
{
	Vector3 loc; loc.X = -46.938f; loc.Y = -64.767f; loc.Z = 2.018f;	float facing = 0.0f;
	const char *Cinematic = "nod_inv_hummdrop1.txt";
	GameObject *dropoff = Commands->Create_Object("Invisible_Object",loc);
	Commands->Set_Facing(dropoff,facing);
	Commands->Attach_Script(dropoff,"Test_Cinematic",Cinematic);
	Commands->Create_2D_WAV_Sound("m00evag_dsgn0060i1evag_snd.wav");
	Send_Message(255,204,0,"Reinforcements are enroute");
}

void z_Invasion_Drop_Hummvee2::Killed(GameObject *obj, GameObject *shooter)
{
	Vector3 loc; loc.X = 75.122f; loc.Y = 38.777f; loc.Z = 2.118f;	float facing = -180.0f;
	const char *Cinematic = "nod_inv_hummdrop2.txt";
	GameObject *dropoff = Commands->Create_Object("Invisible_Object",loc);
	Commands->Set_Facing(dropoff,facing);
	Commands->Attach_Script(dropoff,"Test_Cinematic",Cinematic);
	Commands->Create_2D_WAV_Sound("m00evag_dsgn0060i1evag_snd.wav");
	Send_Message(255,204,0,"Reinforcements are enroute");
}

void z_Invasion_Loop_Chinooks_GDIBase::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,311.0f,1001);
	Commands->Start_Timer(obj,this,314.0f,1002);
	Commands->Start_Timer(obj,this,318.0f,1003);
	Commands->Start_Timer(obj,this,320.0f,1004);
}
void z_Invasion_Loop_Chinooks_GDIBase::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1001)
	{
		const char *cinematic1 = "nod_inv_infantry1.txt";
		int dropid1 = 100095;
		Vector3 pos1 = Commands->Get_Position(Commands->Find_Object(dropid1));
		float fac1 = 0.000f;
		GameObject *dropoff1 = Commands->Create_Object("Invisible_Object",pos1);
		Commands->Set_Facing(dropoff1,fac1);
		Commands->Attach_Script(dropoff1,"Test_Cinematic",cinematic1);
		Commands->Start_Timer(obj,this,101.0f,1001);
	}
	else if (number == 1002)
	{
		const char *cinematic2 = "nod_inv_infantry2.txt";
		int dropid2 = 100096;
		Vector3 pos2 = Commands->Get_Position(Commands->Find_Object(dropid2));
		float fac2 = -90.000f;
		GameObject *dropoff2 = Commands->Create_Object("Invisible_Object",pos2);
		Commands->Set_Facing(dropoff2,fac2);
		Commands->Attach_Script(dropoff2,"Test_Cinematic",cinematic2);
		Commands->Start_Timer(obj,this,104.0f,1002);
	}
	else if (number == 1003)
	{
		const char *cinematic3 = "nod_inv_infantry3.txt";
		int dropid3 = 100097;
		Vector3 pos3 = Commands->Get_Position(Commands->Find_Object(dropid3));
		float fac3 = -180.000f;
		GameObject *dropoff3 = Commands->Create_Object("Invisible_Object",pos3);
		Commands->Set_Facing(dropoff3,fac3);
		Commands->Attach_Script(dropoff3,"Test_Cinematic",cinematic3);
		Commands->Start_Timer(obj,this,106.0f,1003);
		Commands->Create_2D_WAV_Sound("airraid.wav");
	}
	if (number == 1004)
	{
		const char *cinematic4 = "nod_inv_infantry4.txt";
		int dropid4 = 100098;
		Vector3 pos4 = Commands->Get_Position(Commands->Find_Object(dropid4));
		float fac4 = 90.000f;
		GameObject *dropoff4 = Commands->Create_Object("Invisible_Object",pos4);
		Commands->Set_Facing(dropoff4,fac4);
		Commands->Attach_Script(dropoff4,"Test_Cinematic",cinematic4);
		Commands->Start_Timer(obj,this,109.0f,1004);
		count++;
		if (count == 100)
		{
			Commands->Destroy_Object(obj);
		}
	}
}
void z_Invasion_Loop_Chinooks_GDIBase::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&count,sizeof(count),1);
}

void z_Invasion_Buy_Guntower_North::Created(GameObject *obj)
{
	pos.X = -7.462f;	pos.Y = 57.794f;	pos.Z = 7.042f;
	pos2.X = 23.309f;	pos2.Y = 57.112f;	pos2.Z = 7.070f;
	funds = 0;
	totalcost = 2500.0f;
	allowpoke = true;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void z_Invasion_Buy_Guntower_North::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (funds < totalcost)
		{
			if (Commands->Get_Money(poker) >= 500.0f)
			{
				Commands->Give_Money(poker,-500.0f,false);
				funds+=500.0f;
				char fundstatus[512];
				sprintf(fundstatus,"%s added $500 to the North Guntowers fund. Current: %g of %g",Get_Player_Name(poker),funds,totalcost);
				Send_Message(255,204,0,fundstatus);
				if (funds == totalcost)
				{
					Commands->Enable_HUD_Pokable_Indicator(obj,false);
					Commands->Start_Timer(obj,this,1.0f,20001);//please standby
					Commands->Start_Timer(obj,this,3.0f,20002);//access granted
					Commands->Start_Timer(obj,this,6.0f,20003);//building
					Commands->Start_Timer(obj,this,8.0f,20004);//make the guntower appear
				}
			}
			else
			{
				Create_2D_WAV_Sound_Player(poker,"m00evag_dsgn0028i1evag_snd.wav");
				Send_Message_Player(poker,255,204,0,"Insufficient funds. You do not have $500");
			}
		}
		else
		{
			Commands->Create_2D_WAV_Sound("m00evag_dsgn0012i1evag_snd.wav");
			Send_Message(255,204,0,"Unable to comply building in progress: North Guntowers");
			allowpoke = false;
		}
	}
}
void z_Invasion_Buy_Guntower_North::Timer_Expired(GameObject *obj, int number)
{
	if (number == 20001)
	{
		Commands->Create_2D_WAV_Sound("m00evag_dsgn0030i1evag_snd.wav");
		Send_Message(255,204,0,"Please standby");
	}
	else if (number == 20002)
	{
		Commands->Create_2D_WAV_Sound("m00evag_dsgn0045i1evag_snd.wav");
		Send_Message(255,204,0,"Access granted");
	}
	else if (number == 20003)
	{
		Commands->Create_2D_WAV_Sound("m00evag_dsgn0004i1evag_snd.wav");
		Send_Message(255,204,0,"Building: North Guntowers");
	}
	else if (number == 20004)
	{
		GameObject *guntower = Commands->Create_Object("GDI_Guntower",pos);
		GameObject *guntower2 = Commands->Create_Object("GDI_Guntower",pos2);
		Commands->Set_Facing(guntower,0.000f);
		Commands->Disable_Physical_Collisions(guntower);
		Commands->Set_Facing(guntower2,0.000f);
		Commands->Disable_Physical_Collisions(guntower2);
		Commands->Destroy_Object(obj);
	}
}
void z_Invasion_Buy_Guntower_North::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&pos,sizeof(pos),1);
	Auto_Save_Variable(&totalcost,sizeof(totalcost),2);
	Auto_Save_Variable(&funds,sizeof(funds),3);
	Auto_Save_Variable(&allowpoke,sizeof(allowpoke),4);
	Auto_Save_Variable(&pos2,sizeof(pos2),5);
}

void z_Invasion_Buy_Guntower_West::Created(GameObject *obj)
{
	pos.X = -64.574f;	pos.Y = -46.444f;	pos.Z = 7.042f;//west
	pos2.X = -64.051f;	pos2.Y = -11.443f;	pos2.Z = 7.010f;
	funds = 0;
	totalcost = 2500.0f;
	allowpoke = true;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void z_Invasion_Buy_Guntower_West::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (funds < totalcost)
		{
			if (Commands->Get_Money(poker) >= 500.0f)
			{
				Commands->Give_Money(poker,-500.0f,false);
				funds+=500.0f;
				char fundstatus[512];
				sprintf(fundstatus,"%s added $500 to the West Guntowers fund. Current: %g of %g",Get_Player_Name(poker),funds,totalcost);
				Send_Message(255,204,0,fundstatus);
				if (funds == totalcost)
				{
					Commands->Enable_HUD_Pokable_Indicator(obj,false);
					Commands->Start_Timer(obj,this,1.0f,20001);//please standby
					Commands->Start_Timer(obj,this,3.0f,20002);//access granted
					Commands->Start_Timer(obj,this,6.0f,20003);//building
					Commands->Start_Timer(obj,this,8.0f,20004);//make the guntower appear
				}
			}
			else
			{
				Create_2D_WAV_Sound_Player(poker,"m00evag_dsgn0028i1evag_snd.wav");
				Send_Message_Player(poker,255,204,0,"Insufficient funds. You do not have $500");
			}
		}
		else
		{
			Commands->Create_2D_WAV_Sound("m00evag_dsgn0012i1evag_snd.wav");
			Send_Message(255,204,0,"Unable to comply building in progress: West Guntowers");
			allowpoke = false;
		}
	}
}
void z_Invasion_Buy_Guntower_West::Timer_Expired(GameObject *obj, int number)
{
	if (number == 20001)
	{
		Commands->Create_2D_WAV_Sound("m00evag_dsgn0030i1evag_snd.wav");
		Send_Message(255,204,0,"Please standby");
	}
	else if (number == 20002)
	{
		Commands->Create_2D_WAV_Sound("m00evag_dsgn0045i1evag_snd.wav");
		Send_Message(255,204,0,"Access granted");
	}
	else if (number == 20003)
	{
		Commands->Create_2D_WAV_Sound("m00evag_dsgn0004i1evag_snd.wav");
		Send_Message(255,204,0,"Building: West Guntowers");
	}
	else if (number == 20004)
	{
		GameObject *guntower = Commands->Create_Object("GDI_Guntower",pos);
		GameObject *guntower2 = Commands->Create_Object("GDI_Guntower",pos2);
		Commands->Set_Facing(guntower,0.000f);
		Commands->Disable_Physical_Collisions(guntower);
		Commands->Set_Facing(guntower2,0.000f);
		Commands->Disable_Physical_Collisions(guntower2);
		Commands->Destroy_Object(obj);
	}
}
void z_Invasion_Buy_Guntower_West::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&pos,sizeof(pos),1);
	Auto_Save_Variable(&totalcost,sizeof(totalcost),2);
	Auto_Save_Variable(&funds,sizeof(funds),3);
	Auto_Save_Variable(&allowpoke,sizeof(allowpoke),4);
	Auto_Save_Variable(&pos2,sizeof(pos2),5);
}

void z_Invasion_Buy_Guntower_South::Created(GameObject *obj)
{
	pos.X = 21.602f;	pos.Y = -81.324f;	pos.Z = 7.042f;
	pos2.X = -20.775f;	pos2.Y = -80.544f;	pos2.Z = 7.027f;
	funds = 0;
	totalcost = 2500.0f;
	allowpoke = true;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void z_Invasion_Buy_Guntower_South::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (funds < totalcost)
		{
			if (Commands->Get_Money(poker) >= 500.0f)
			{
				Commands->Give_Money(poker,-500.0f,false);
				funds+=500.0f;
				char fundstatus[512];
				sprintf(fundstatus,"%s added $500 to the South Guntowers fund. Current: %g of %g",Get_Player_Name(poker),funds,totalcost);
				Send_Message(255,204,0,fundstatus);
				if (funds == totalcost)
				{
					Commands->Enable_HUD_Pokable_Indicator(obj,false);
					Commands->Start_Timer(obj,this,1.0f,20001);//please standby
					Commands->Start_Timer(obj,this,3.0f,20002);//access granted
					Commands->Start_Timer(obj,this,6.0f,20003);//building
					Commands->Start_Timer(obj,this,8.0f,20004);//make the guntower appear
				}
			}
			else
			{
				Create_2D_WAV_Sound_Player(poker,"m00evag_dsgn0028i1evag_snd.wav");
				Send_Message_Player(poker,255,204,0,"Insufficient funds. You do not have $500");
			}
		}
		else
		{
			Commands->Create_2D_WAV_Sound("m00evag_dsgn0012i1evag_snd.wav");
			Send_Message(255,204,0,"Unable to comply building in progress: South Guntowers");
			allowpoke = false;
		}
	}
}
void z_Invasion_Buy_Guntower_South::Timer_Expired(GameObject *obj, int number)
{
	if (number == 20001)
	{
		Commands->Create_2D_WAV_Sound("m00evag_dsgn0030i1evag_snd.wav");
		Send_Message(255,204,0,"Please standby");
	}
	else if (number == 20002)
	{
		Commands->Create_2D_WAV_Sound("m00evag_dsgn0045i1evag_snd.wav");
		Send_Message(255,204,0,"Access granted");
	}
	else if (number == 20003)
	{
		Commands->Create_2D_WAV_Sound("m00evag_dsgn0004i1evag_snd.wav");
		Send_Message(255,204,0,"Building: South Guntowers");
	}
	else if (number == 20004)
	{
		GameObject *guntower = Commands->Create_Object("GDI_Guntower",pos);
		GameObject *guntower2 = Commands->Create_Object("GDI_Guntower",pos2);
		Commands->Set_Facing(guntower,0.000f);
		Commands->Disable_Physical_Collisions(guntower);
		Commands->Set_Facing(guntower2,0.000f);
		Commands->Disable_Physical_Collisions(guntower2);
		Commands->Destroy_Object(obj);
	}
}
void z_Invasion_Buy_Guntower_South::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&pos,sizeof(pos),1);
	Auto_Save_Variable(&totalcost,sizeof(totalcost),2);
	Auto_Save_Variable(&funds,sizeof(funds),3);
	Auto_Save_Variable(&allowpoke,sizeof(allowpoke),4);
	Auto_Save_Variable(&pos2,sizeof(pos2),5);
}

void z_Invasion_Buy_Guntower_East::Created(GameObject *obj)
{
	pos.X = 91.753f;	pos.Y = 21.550f;	pos.Z = 7.042f;//east
	pos2.X = 90.502f;	pos2.Y = -12.639f;	pos2.Z = 6.933f;
	funds = 0;
	totalcost = 2500.0f;
	allowpoke = true;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void z_Invasion_Buy_Guntower_East::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (funds < totalcost)
		{
			if (Commands->Get_Money(poker) >= 500.0f)
			{
				Commands->Give_Money(poker,-500.0f,false);
				funds+=500.0f;
				char fundstatus[512];
				sprintf(fundstatus,"%s added $500 to the East Guntowers fund. Current: %g of %g",Get_Player_Name(poker),funds,totalcost);
				Send_Message(255,204,0,fundstatus);
				if (funds == totalcost)
				{
					Commands->Enable_HUD_Pokable_Indicator(obj,false);
					Commands->Start_Timer(obj,this,1.0f,20001);//please standby
					Commands->Start_Timer(obj,this,3.0f,20002);//access granted
					Commands->Start_Timer(obj,this,6.0f,20003);//building
					Commands->Start_Timer(obj,this,8.0f,20004);//make the guntower appear
				}
			}
			else
			{
				Create_2D_WAV_Sound_Player(poker,"m00evag_dsgn0028i1evag_snd.wav");
				Send_Message_Player(poker,255,204,0,"Insufficient funds. You do not have $500");
			}
		}
		else
		{
			Commands->Create_2D_WAV_Sound("m00evag_dsgn0012i1evag_snd.wav");
			Send_Message(255,204,0,"Unable to comply building in progress: East Guntowers");
			allowpoke = false;
		}
	}
}
void z_Invasion_Buy_Guntower_East::Timer_Expired(GameObject *obj, int number)
{
	if (number == 20001)
	{
		Commands->Create_2D_WAV_Sound("m00evag_dsgn0030i1evag_snd.wav");
		Send_Message(255,204,0,"Please standby");
	}
	else if (number == 20002)
	{
		Commands->Create_2D_WAV_Sound("m00evag_dsgn0045i1evag_snd.wav");
		Send_Message(255,204,0,"Access granted");
	}
	else if (number == 20003)
	{
		Commands->Create_2D_WAV_Sound("m00evag_dsgn0004i1evag_snd.wav");
		Send_Message(255,204,0,"Building: East Guntowers");
	}
	else if (number == 20004)
	{
		GameObject *guntower = Commands->Create_Object("GDI_Guntower",pos);
		GameObject *guntower2 = Commands->Create_Object("GDI_Guntower",pos2);
		Commands->Set_Facing(guntower,0.000f);
		Commands->Disable_Physical_Collisions(guntower);
		Commands->Set_Facing(guntower2,0.000f);
		Commands->Disable_Physical_Collisions(guntower2);
		Commands->Destroy_Object(obj);
	}
}
void z_Invasion_Buy_Guntower_East::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&pos,sizeof(pos),1);
	Auto_Save_Variable(&totalcost,sizeof(totalcost),2);
	Auto_Save_Variable(&funds,sizeof(funds),3);
	Auto_Save_Variable(&allowpoke,sizeof(allowpoke),4);
	Auto_Save_Variable(&pos2,sizeof(pos2),5);
}

ScriptRegistrant<z_Invasion_Buy_Guntower_East> z_Invasion_Buy_Guntower_East_Registrant("z_Invasion_Buy_Guntower_East","");
ScriptRegistrant<z_Invasion_Buy_Guntower_South> z_Invasion_Buy_Guntower_South_Registrant("z_Invasion_Buy_Guntower_South","");
ScriptRegistrant<z_Invasion_Buy_Guntower_West> z_Invasion_Buy_Guntower_West_Registrant("z_Invasion_Buy_Guntower_West","");
ScriptRegistrant<z_Invasion_Buy_Guntower_North> z_Invasion_Buy_Guntower_North_Registrant("z_Invasion_Buy_Guntower_North","");
ScriptRegistrant<z_Invasion_Loop_Chinooks_GDIBase> z_Invasion_Loop_Chinooks_GDIBase_Registrant("z_Invasion_Loop_Chinooks_GDIBase","");
ScriptRegistrant<z_Invasion_Drop_Hummvee2> z_Invasion_Drop_Hummvee2_Registrant("z_Invasion_Drop_Hummvee2","");
ScriptRegistrant<z_Invasion_Drop_Hummvee1> z_Invasion_Drop_Hummvee1_Registrant("z_Invasion_Drop_Hummvee1","");
ScriptRegistrant<z_Invasion_Loop_Badgerdrop> z_Invasion_Loop_Badgerdrop_Registrant("z_Invasion_Loop_Badgerdrop","");
ScriptRegistrant<z_Invasion_Drop_Harvester> z_Invasion_Drop_Harvester_Registrant("z_Invasion_Drop_Harvester","");
ScriptRegistrant<z_Invasion_Enemy> z_Invasion_Enemy_Registrant("z_Invasion_Enemy","");
ScriptRegistrant<z_Invasion_Bot_Counter> z_Invasion_Bot_Counter_Registrant("z_Invasion_Bot_Counter","");
ScriptRegistrant<z_Invasion_Hostage_Count> z_Invasion_Hostage_Count_Registrant("z_Invasion_Hostage_Count","");
ScriptRegistrant<z_Invasion_Hostage> z_Invasion_Hostage_Registrant("z_Invasion_Hostage","Hostage_Name=zunnie:string");
ScriptRegistrant<z_Poke_Play_Cinematic> z_Poke_Play_Cinematic_Registrant("z_Poke_Play_Cinematic","Cinematic=bla:string,DropID=0:int,Facing=0:float");