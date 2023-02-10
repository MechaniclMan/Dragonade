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
#include "z_fever.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"

void z_Fever_PP::Created(GameObject *obj)
{
	repair = true;
}
void z_Fever_PP::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	/*if (repair)
	{
		if (Commands->Get_Health(obj) <= 50.0f)
		{
			Commands->Set_Health(obj,Commands->Get_Max_Health(obj));//simulating an engineer repairing it :V
		}
	}*/
}
void z_Fever_PP::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)//tanya died with intel so disable script from repairing and obstructing the Win command...
	{
		if (param == 1)
		{
			repair = false;
			Console_Input("win 0");
		}
	}
	else if (message == 2000)
	{
		if (param == 1)
		{
			repair = false;
			Commands->Start_Timer(obj,this,3.5,100);//ownedm
			Console_Input("win 1");
		}
	}
}
void z_Fever_PP::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Console_Input("snda we_owned_them.wav");
	}
}

void z_Fever_Bar::Created(GameObject *obj)
{
	repair = true;
}
void z_Fever_Bar::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)//engis are dead in bar, disable repairing
	{
		if (param == 1)
		{
			repair = false;
			Send_Message(255,255,255,"The Soviet Barracks is no longer internally repaired by engineers.");
		}
	}
}
void z_Fever_Bar::Killed(GameObject *obj, GameObject *shooter)
{
	Send_Message(19,169,255,"Soviet advanced infantry reinforcements disabled.");
	Commands->Enable_Spawner(100258,0);
	Commands->Enable_Spawner(100259,0);
	Commands->Enable_Spawner(100260,0);
	Commands->Enable_Spawner(100264,0);
	Commands->Enable_Spawner(100265,0);
	Commands->Enable_Spawner(100266,0);
	Commands->Enable_Spawner(100267,0);
	Commands->Enable_Spawner(100268,0);
	Commands->Enable_Spawner(100269,0);
	Commands->Enable_Spawner(100270,0);
	Commands->Enable_Spawner(100271,0);
	Commands->Enable_Spawner(100272,0);
	Commands->Enable_Spawner(100273,0);
	Commands->Enable_Spawner(100283,0);
	Commands->Enable_Spawner(100284,0);
	Commands->Enable_Spawner(100285,0);
	Commands->Enable_Spawner(100287,0);
	Commands->Enable_Spawner(100288,0);
	Commands->Enable_Spawner(100289,0);
	Commands->Enable_Spawner(100290,0);
	Commands->Enable_Spawner(100293,0);
	Commands->Enable_Spawner(100294,0);
	Commands->Enable_Spawner(100295,0);
	Commands->Enable_Spawner(100296,0);
	Commands->Enable_Spawner(100297,0);
	Commands->Enable_Spawner(100298,0);
	Commands->Enable_Spawner(100299,0);
	Commands->Enable_Spawner(100300,0);
	Commands->Enable_Spawner(100301,0);
	Commands->Enable_Spawner(100302,0);
	Commands->Enable_Spawner(100303,0);
	Commands->Enable_Spawner(100304,0);
	Commands->Enable_Spawner(100305,0);
	Commands->Enable_Spawner(100313,0);
	Commands->Enable_Spawner(100314,0);
	Commands->Enable_Spawner(100315,0);
	Commands->Enable_Spawner(100316,0);
	Commands->Enable_Spawner(100317,0);
	Commands->Enable_Spawner(100318,0);
	Commands->Enable_Spawner(100319,0);
	Commands->Enable_Spawner(100324,0);
	Commands->Enable_Spawner(100325,0);
	Commands->Enable_Spawner(100326,0);
	Commands->Enable_Spawner(100327,0);
	Commands->Enable_Spawner(100328,0);
	Commands->Enable_Spawner(100329,0);
	Commands->Enable_Spawner(100333,0);
	Commands->Enable_Spawner(100340,0);
	Commands->Enable_Spawner(100341,0);
	Commands->Enable_Spawner(100342,0);
	Commands->Enable_Spawner(100343,0);
	Commands->Enable_Spawner(100345,0);
	Commands->Enable_Spawner(100344,0);
	Commands->Enable_Spawner(100346,0);
	Commands->Enable_Spawner(100347,0);
	Commands->Enable_Spawner(100348,0);
}
void z_Fever_Bar::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Get_Health(obj) <= 50.0f)
	{
		if (repair)
		{
			Commands->Set_Health(obj,Commands->Get_Max_Health(obj));//simulating an engineer repairing it :V
		}
	}
}

void z_Fever_Start::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,30.0f,101);//good morning
	Commands->Start_Timer(obj,this,34.0f,102);//steal intel
	Commands->Start_Timer(obj,this,38.0f,103);//use mech
	Commands->Start_Timer(obj,this,42.0f,104);//destroy bar
	Commands->Start_Timer(obj,this,46.0f,105);//do what it takes
	Commands->Start_Timer(obj,this,50.0f,106);//i repeat
}
void z_Fever_Start::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(19,169,255,"Good morning Commanders. Welcome to Operation Midnight Fever.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(19,169,255,"Steal intelligence data from the Soviet Construction Yard via Chinook escape.");
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(19,169,255,"Use a Mechanic to unlock a Heavy tank. Then Spy the Powerplant and take out the Tesla Coil.");
	}
	else if (number == 104)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(19,169,255,"Destroy the Barracks to eliminate Advanced Soviet Infantry reinforcements.");
	}
	else if (number == 105)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(19,169,255,"Do what it takes Commanders but do not destroy the Construction Yard.");
	}
	else if (number == 106)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(19,169,255,"I repeat. Do not destroy the Construction Yard.");
	}
}

void z_Fever_IHazIntel::Created(GameObject *obj)
{
	char ann[250];
	sprintf(ann,"amsg %s has the Intel Documents.",Get_Player_Name(obj));
	Console_Input(ann);
}
void z_Fever_IHazIntel::Killed(GameObject *obj, GameObject *shooter)
{
	char failguy[250];
	sprintf(failguy,"amsg %s failed the mission. Tanya has died.",Get_Player_Name(obj));
	Console_Input(failguy);
	Commands->Send_Custom_Event(obj,Commands->Find_Object(150206),1000,1,1);//send endgame message to powerplant controller
}

void z_Fever_IntelPC::Created(GameObject *obj)
{
	tries = 0;
	pokerguy = 0;
	allowpoke = true;
	pcid = Commands->Get_ID(obj);
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
}
void z_Fever_IntelPC::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		pokerguy = poker;
		if (_stricmp(Commands->Get_Preset_Name(poker),"Allied_Tanya") == 0)
		{
			Commands->Start_Timer(obj,this,1.0f,100);//accessing
			Commands->Start_Timer(obj,this,5.0f,101);//please standbu
			Commands->Start_Timer(obj,this,10.0f,103);//granted
		}
		else
		{
			Commands->Start_Timer(obj,this,1.0f,100);//accessing
			Commands->Start_Timer(obj,this,5.0f,101);//please standbu
			Commands->Start_Timer(obj,this,10.0f,102);//denied	
		}
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		Commands->Start_Timer(obj,this,30.0f,104);
		allowpoke = false;
	}
}
void z_Fever_IntelPC::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Create_2D_WAV_Sound("accessing.wav");
		Send_Message(19,169,255,"Accessing... ... ... ... ... ...");
	}
	else if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("pleasestandby.wav");
		Send_Message(19,169,255,"Please Stand By... ... ... .... ...");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("accessdenied.wav");
		Send_Message(19,169,255,"Access Denied");
		tries++;
		if (tries == 4)
		{
			char failguy[250];
			sprintf(failguy,"amsg %s failed the mission. Trying to hack the PC without Tanya.",Get_Player_Name(pokerguy));
			Console_Input(failguy);
			Commands->Send_Custom_Event(obj,Commands->Find_Object(150206),1000,1,1);//send lose endgame message to powerplant controller
		}
		Send_Message(19,169,255,"Only Tanya can hack the PC.");
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("accessgranted.wav");
		Send_Message(19,169,255,"Access Granted");
		Attach_Script_Once(pokerguy,"z_Fever_IHazIntel","");
	}
	else if (number == 104)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}

void z_Fever_Chinook::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (Is_Script_Attached(sender,"z_Fever_IHazIntel"))
		{
			Commands->Send_Custom_Event(obj,Commands->Find_Object(150206),2000,1,1);//send win endgame message to powerplant controller
		}
		else 
		{
			Commands->Start_Timer(obj,this,0.5f,100);//boot player
		}
	}
}
void z_Fever_Chinook::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Force_Occupant_Exit(obj,0);
	}
}


ScriptRegistrant<z_Fever_Chinook> z_Fever_Chinook_Registrant("z_Fever_Chinook","");
ScriptRegistrant<z_Fever_IntelPC> z_Fever_IntelPC_Registrant("z_Fever_IntelPC","");
ScriptRegistrant<z_Fever_IHazIntel> z_Fever_IHazIntel_Registrant("z_Fever_IHazIntel","");
ScriptRegistrant<z_Fever_Start> z_Fever_Start_Registrant("z_Fever_Start","");
ScriptRegistrant<z_Fever_Bar> z_Fever_Bar_Registrant("z_Fever_Bar","");
ScriptRegistrant<z_Fever_PP> z_Fever_PP_Registrant("z_Fever_PP","");
