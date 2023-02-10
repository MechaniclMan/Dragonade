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
#include "z_cargo.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"


void z_Pup_Soviet_Shocky::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"Soviet_Shock_Trooper");
	}
}

void z_Pup_Soviet_Officer::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"Soviet_Rifle_Infantry_Officer");
	}
}

void z_Pup_Soviet_Sniper::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"Soviet_Sniper_Coop");
	}
}

void z_Pup_Soviet_Flamer::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"Soviet_Flamethrower_Infantry");
	}
}

void z_Pup_Soviet_Volkov::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"Soviet_Volkov");
	}
}

void z_BotDeadCount_DoMSG::Created(GameObject *obj)
{
	count = 0;
	check = Get_Int_Parameter("HowManyDeathsToTrigger");
}
void z_BotDeadCount_DoMSG::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			count++;
			if (count == check)
			{
				const char *inputmsg = Get_Parameter("Message");
				char prms[250];
				sprintf(prms,"%s",inputmsg);
				Send_Message(47,190,36,prms);
				Destroy_Script();
			}
		}
	}
}

void z_Cargo_Enter_MissionComplete::Entered(GameObject *obj, GameObject *enter)
{
	if (strcmp(The_Game()->Get_Map_Name(),"RA_Cargo_Express_Coop.mix") == 0)
	{
		if (strcmp(Commands->Get_Preset_Name(enter),"TheCargoTruck") == 0)
		{
			Send_Message(47,190,36,"Primary mission objective met.");
			Commands->Create_2D_WAV_Sound("1objmet1.wav");
			Commands->Start_Timer(obj,this,5.0f,101);
		}
	}
}
void z_Cargo_Enter_MissionComplete::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Console_Input("amsg Mission Accomplished");
		Console_Input("win 0");
	}
}

void z_CargoTruckDeathCounter::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,90.0f,501);
	count = 0;
	check = Get_Int_Parameter("MaxDeaths");
}
void z_CargoTruckDeathCounter::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		if (param == 1)
		{
			count++;
			if (count == check)
			{
				Commands->Create_2D_WAV_Sound("missionfailed.wav");
				Console_Input("amsg Mission Failed!");
				Commands->Start_Timer(obj,this,2.0f,500);
			}
		}
	}
}
void z_CargoTruckDeathCounter::Timer_Expired(GameObject *obj, int number)
{
	if (number == 500)
	{
		Console_Input("win 1");
	}
	else if (number == 501)
	{
		char deads[250];
		sprintf(deads,"The Cargo Truck has died %d/%d times.",count,check);
		Send_Message(255,255,255,deads);
		Commands->Start_Timer(obj,this,90.0f,501);
	}
}


ScriptRegistrant<z_CargoTruckDeathCounter> z_CargoTruckDeathCounter_Registrant("z_CargoTruckDeathCounter","MaxDeaths=3:int,Message=500:int");
ScriptRegistrant<z_Cargo_Enter_MissionComplete> z_Cargo_Enter_MissionComplete_Registrant("z_Cargo_Enter_MissionComplete","");
ScriptRegistrant<z_BotDeadCount_DoMSG> z_BotDeadCount_DoMSG_Registrant("z_BotDeadCount_DoMSG","HowManyDeathsToTrigger=10:int,Message=WhatToShow:string");
ScriptRegistrant<z_Pup_Soviet_Volkov> z_Pup_Soviet_Volkov_Registrant("z_Pup_Soviet_Volkov","");
ScriptRegistrant<z_Pup_Soviet_Flamer> z_Pup_Soviet_Flamer_Registrant("z_Pup_Soviet_Flamer","");
ScriptRegistrant<z_Pup_Soviet_Sniper> z_Pup_Soviet_Sniper_Registrant("z_Pup_Soviet_Sniper","");
ScriptRegistrant<z_Pup_Soviet_Officer> z_Pup_Soviet_Officer_Registrant("z_Pup_Soviet_Officer","");
ScriptRegistrant<z_Pup_Soviet_Shocky> z_Pup_Soviet_Shocky_Registrant("z_Pup_Soviet_Shocky","");