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
#include "z_am01.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"


void z_aM01_Einstein::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	Commands->Set_Player_Type(obj,-2);
	Commands->Clear_Weapons(obj);
	Commands->Innate_Disable(obj);
	AllowPoke = true;
	Following = false;
	FollowID = 0;
	Pokerguy = 0;
	HostageID = Commands->Get_ID(obj);
}

void z_aM01_Einstein::Poked(GameObject *obj,GameObject *poker)
{
	if (AllowPoke)
	{
		Pokerguy = poker;
		FollowID = Commands->Get_ID(poker);
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Commands->Set_Player_Type(obj,Get_Player_Type(poker));
		Commands->Start_Timer(obj,this,0.5,789);
		AllowPoke = false;
		Following = true;
		Commands->Give_Money(Pokerguy,100.0f,0);
		int RandomNum = Commands->Get_Random_Int(1,3);
		if (RandomNum == 1 || RandomNum == 2)
		{
			Create_2D_WAV_Sound_Player(poker,"youlead.wav");
			Send_Message_Player(poker,243,0,0,"Einstein: Ok you lead the way.");
		}
		else if (RandomNum == 3)
		{
			Create_2D_WAV_Sound_Player(poker,"getouttahere.wav");
			Send_Message_Player(poker,243,0,0,"Einstein: Let's get the hell outta here.");
		}
	}
}

void z_aM01_Einstein::Timer_Expired(GameObject *obj, int number)
{
	if (number == 789)//this is the number we have been waiting for
	{
		if (Commands->Find_Object(FollowID))//w00t we found our hero to the rescue
		{
			GameObject *GotoObject = Commands->Find_Object(FollowID);
			ActionParamsStruct params;
			params.Set_Basic(this,100,100);
			params.Set_Movement(GotoObject,1.0f,1.5f);
			params.MoveFollow = true;
			Commands->Action_Goto(obj,params);			
			Following = true;
			Commands->Start_Timer(obj,this,0.1f,789);//our hero is alive so we continue following
		}
		else//pokern00b died or left the game, stop following
		{
			Pokerguy = 0;
			FollowID = 0;//setting FollowID to 0
			Following = false;//not following anymore
			AllowPoke = true;//poking is allowed again because the person poking the hostage died or left
			Commands->Enable_HUD_Pokable_Indicator(obj,true);//can poke again so adjust the indicators
			Commands->Set_Player_Type(obj,-2);//unteam the hostage so they are not attacked by bots anymore
		}
	}
}


void z_aM01_Einstein::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Commands->Create_2D_WAV_Sound("rescued.wav");
			Commands->Give_Points(Pokerguy,100.0f,0);
			char resmsg[250];
			const char *str = Get_Player_Name(Pokerguy);
			sprintf(resmsg,"%s rescued Einstein.",str);
			delete[] str;
			Send_Message(40,121,138,resmsg);
			Set_Skin(obj,"Blamo");
			Commands->Set_Shield_Type(obj,"Blamo");
			Commands->Send_Custom_Event(obj,Commands->Find_Object(100092),2000,1,0);
		}
	}
}

void z_aM01_Einstein::Killed(GameObject *obj,GameObject *shooter)
{
	Commands->Create_2D_WAV_Sound("hosdown.wav");
	Send_Message(255,64,64,"Einstein was killed!");
	if (Commands->Is_A_Star(shooter))
	{
		char fail[250];
		const char *str = Get_Player_Name(shooter);
		sprintf(fail,"%s failed the mission by terminating Einstein.",str);
		delete[] str;
		Send_Message(64,255,64,fail);
	}
	Commands->Send_Custom_Event(obj,Commands->Find_Object(100092),1000,1,0);
}


/*********************************************************************************************/

void z_aM01_Einstein_Zone::Entered(GameObject *obj,GameObject *enter)
{
	if (strcmp(Commands->Get_Preset_Name(enter),"Einstein") == 0)
	{
		Commands->Send_Custom_Event(obj,enter,1000,1,0.0f);
	}
}
void z_aM01_Einstein_Zone::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000) //fail
	{
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,1.0f,180);
			Commands->Start_Timer(obj,this,3.0f,181);
		}
	}
	else if (message == 2000)
	{
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,2.5f,182);
		}
	}
}
void z_aM01_Einstein_Zone::Timer_Expired(GameObject *obj, int number)
{
	if (number == 180)
	{
		Commands->Create_2D_WAV_Sound("missionfailed.wav");
		Send_Message(40,121,138,"Mission Failed!");
		Send_Message(40,121,138,"Mission Failed!");
		Send_Message(40,121,138,"Mission Failed!");
		Commands->Start_Timer(obj,this,2.5f,181);
	}
	else if (number == 181)
	{
		Console_Input("win 0");
		Console_Input("amsg Mission Failed. Einstein was killed.");
	}
	else if (number == 182)
	{
		Console_Input("amsg Mission Accomplished");
		Console_Input("win 1");
	}
}

void z_aM01_MissionStart::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,15.0f,100);
}
void z_aM01_MissionStart::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Send_Message(255,255,255,"Rescue Einstein from soviets hands. He is being held hostage at the Radar Dome.");
		Send_Message(255,255,255,"To rescue Einstein lead him to the Naval Transporter.");
		Send_Message(255,255,255,"Be careful. If Einstein dies your mission will fail. Keep medics close.");
		Commands->Start_Timer(obj,this,100.0f,100);
	}
}

void z_aM01_SovBarracks::Created(GameObject *obj)
{
	count = 0;
}
void z_aM01_SovBarracks::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			count++;
			if (count == 3)
			{
				Send_Message(255,255,255,"The Soviet Infantry Barracks is no longer being internally repaired by Engineers.");
				Destroy_Script();
			}
		}
	}
}
void z_aM01_SovBarracks::Damaged(GameObject *obj,	GameObject *damager, float damage)
{
	if (Commands->Get_Health(obj) <= 50.0f)
	{
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	}
}

ScriptRegistrant<z_aM01_SovBarracks> z_aM01_SovBarracks_Registrant("z_aM01_SovBarracks","");
ScriptRegistrant<z_aM01_MissionStart> z_aM01_MissionStart_Registrant("z_aM01_MissionStart","");
ScriptRegistrant<z_aM01_Einstein> z_aM01_Einstein_Registrant("z_aM01_Einstein","");
ScriptRegistrant<z_aM01_Einstein_Zone> z_aM01_Einstein_Zone_Registrant("z_aM01_Einstein_Zone","");