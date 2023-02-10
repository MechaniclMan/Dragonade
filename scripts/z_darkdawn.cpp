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
#include "z_darkdawn.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"

void z_MechUnlockSovTank::Created(GameObject *obj)
{
	Locked = true;//locked at creation
}
void z_MechUnlockSovTank::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (Locked)
		{
			if (strcmp(Commands->Get_Preset_Name(sender),"Allied_Mechanic") == 0)//only mech can enter 
			{
				Send_Message(255,255,255,"Vehicle Unlocked by Mechanic.");
				Destroy_Script();
			}
			else
			{
				Commands->Start_Timer(obj,this,0.5f,100);
				Locked = true;
			}
		}
		else if (!Locked)
		{
			Destroy_Script();
		}
	}
}
void z_MechUnlockSovTank::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Force_Occupant_Exit(obj,0);
		Locked = true;
	}
}

void z_DarkDawkMissionStart::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,5.0f,101);//Intel operators have discovered plans for a secret Soviet assault on our base in Frankfurt, Germany.
	Commands->Start_Timer(obj,this,10.0f,102);//Due to a majority of our funds going to defenses for this base, we can only supply you with a small
	Commands->Start_Timer(obj,this,15.0f,103);//amount of units. Special Agent Tanya will lead the attack on the Soviet outpost. Wipe it off the map
	Commands->Start_Timer(obj,this,20.0f,104);//before sunrise and the Soviet assault will be greatly weakened.
	Commands->Start_Timer(obj,this,25.0f,105);//First Objective: Destroy the Construction Yard with Tanya's C4.
	Commands->Start_Timer(obj,this,30.0f,106);//Secondary Objective: Destroy the Soviet Base and Forces.
	Commands->Start_Timer(obj,this,35.0f,107);//Mission Timer Initializing...
	Commands->Start_Timer(obj,this,40.0f,108);//30 Minutes Remaining... Start a timer of 30 minutes
}
void z_DarkDawkMissionStart::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(19,169,255,"Intel operators have discovered plans for a secret Soviet assault on our base in Frankfurt, Germany.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(19,169,255,"Due to a majority of our funds going to defenses for this base, we can only supply you with a small");
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(19,169,255,"amount of units. Special Agent Tanya will lead the attack on the Soviet outpost. Wipe it off the map");
	}
	else if (number == 104)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(19,169,255,"before sunrise and the Soviet assault will be greatly weakened.");
	}
	else if (number == 105)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(19,169,255,"First Objective: Steal a V2.");
	}
	else if (number == 106)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(19,169,255,"Secondary Objective: Destroy the Soviet Coil and Base.");
	}
	else if (number == 107)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(19,169,255,"Mission Timer Initializing...");
	}
	else if (number == 108)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Commands->Start_Timer(obj,this,1801.0f,201);//start failed
		Commands->Start_Timer(obj,this,1741.0f,202);//start 1min
		Commands->Start_Timer(obj,this,1501.0f,203);//start 5min
		Commands->Start_Timer(obj,this,1201.0f,204);//start 10min
		Commands->Start_Timer(obj,this,601.0f,205);//start 20min
		Commands->Start_Timer(obj,this,1.0f,206);//start 30min
	}
	else if (number == 201)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Console_Input("msg Mission Failed");
		Console_Input("win 0");
	}
	else if (number == 202)
	{
		Commands->Create_2D_WAV_Sound("1mr.wav");//1min remaining
		Send_Message(19,169,255,"Warning, one minute remaining.");
	}
	else if (number == 203)
	{
		Commands->Create_2D_WAV_Sound("5mr.wav");//5min remaining
		Send_Message(19,169,255,"Warning, five minutes remaining.");
	}
	else if (number == 204)
	{
		Commands->Create_2D_WAV_Sound("10mr.wav");//10min remaining
		Send_Message(19,169,255,"Ten minutes remaining.");
	}
	else if (number == 205)
	{
		Commands->Create_2D_WAV_Sound("20mr.wav");//20min remaining
		Send_Message(19,169,255,"Twenty minutes remaining");
	}
	else if (number == 206)
	{
		Commands->Create_2D_WAV_Sound("30mr.wav");//30min remaining
		Send_Message(19,169,255,"Thirty minutes remaining.");
	}
}

void z_DarkDawnBarRepair::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Get_Health(obj) <= 50.0f)
	{
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	}
}

void z_DarkDawnBarRepair::Created(GameObject *obj)
{
	count = 0;
}
void z_DarkDawnBarRepair::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			count++;
			if (count == 1)
			{
				Send_Message(255,255,255,"The Soviet Barracks is no longer internally repaired by Engineers.");
				Destroy_Script();
			}
		}
	}
}





ScriptRegistrant<z_DarkDawnBarRepair> z_DarkDawnBarRepair_Registrant("z_DarkDawnBarRepair","");
ScriptRegistrant<z_DarkDawkMissionStart> z_DarkDawkMissionStart_Registrant("z_DarkDawkMissionStart","");
ScriptRegistrant<z_MechUnlockSovTank> z_MechUnlockSovTank_Registrant("z_MechUnlockSovTank","");
