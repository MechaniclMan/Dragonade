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
#include "z_escort.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"


void z_Escort_CheckPT::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	PokerGuy = 0;
	AllowPoke = true;
	Check1Locked = true;
	Check2Locked = true;
	Check3Locked = true;
}
void z_Escort_CheckPT::Poked(GameObject *obj, GameObject *poker)
{
	if (AllowPoke)
	{
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		AllowPoke = false;
		PokerGuy = poker;
		if (Check1Locked == false)
		{
			Commands->Start_Timer(obj,this,0.0f,101);//teleport the guy
		}
		else if (Check2Locked == false)
		{
			Commands->Start_Timer(obj,this,0.0f,102);//teleport the guy
		}
		else if (Check3Locked == false)
		{
			Commands->Start_Timer(obj,this,0.0f,103);//teleport the guy
		}
		else
		{
			Send_Message(104,234,40,"Access Denied");
			Commands->Create_2D_WAV_Sound("accessdenied.wav");
		}
		Commands->Start_Timer(obj,this,2.0f,104);//allowpoke true
	}
}
void z_Escort_CheckPT::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Send_Message_Player(PokerGuy,104,234,40,"Access Granted");
		Create_2D_WAV_Sound_Player(PokerGuy,"accessgranted.wav");
		int random = Commands->Get_Random_Int(1,3);
		int cp1id1 = Get_Int_Parameter("CP1_ID1");	
		int cp1id2 = Get_Int_Parameter("CP1_ID2");	
		int cp1id3 = Get_Int_Parameter("CP1_ID3");
		GameObject *gotoObjectcp1id1 = Commands->Find_Object(cp1id1);	
		GameObject *gotoObjectcp1id2 = Commands->Find_Object(cp1id2);	
		GameObject *gotoObjectcp1id3 = Commands->Find_Object(cp1id3);
		Vector3 gotoLocationcp1id1 = Commands->Get_Position(gotoObjectcp1id1);	
		Vector3 gotoLocationcp1id2 = Commands->Get_Position(gotoObjectcp1id2);	
		Vector3 gotoLocationcp1id3 = Commands->Get_Position(gotoObjectcp1id3);
		if (random == 1)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp1id1);
		}
		else if (random == 2)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp1id2);
		}
		else if (random == 3)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp1id3);
		}
	}
	else if (number == 102)
	{
		Send_Message_Player(PokerGuy,104,234,40,"Access Granted");
		Create_2D_WAV_Sound_Player(PokerGuy,"accessgranted.wav");
		int random = Commands->Get_Random_Int(1,3);
		int cp2id1 = Get_Int_Parameter("CP2_ID1");	
		int cp2id2 = Get_Int_Parameter("CP2_ID2");	
		int cp2id3 = Get_Int_Parameter("CP2_ID3");
		GameObject *gotoObjectcp2id1 = Commands->Find_Object(cp2id1);	
		GameObject *gotoObjectcp2id2 = Commands->Find_Object(cp2id2);	
		GameObject *gotoObjectcp2id3 = Commands->Find_Object(cp2id3);
		Vector3 gotoLocationcp2id1 = Commands->Get_Position(gotoObjectcp2id1);	
		Vector3 gotoLocationcp2id2 = Commands->Get_Position(gotoObjectcp2id2);	
		Vector3 gotoLocationcp2id3 = Commands->Get_Position(gotoObjectcp2id3);
		if (random == 1)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp2id1);
		}
		else if (random == 2)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp2id2);
		}
		else if (random == 3)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp2id3);
		}
	}
	else if (number == 103)
	{
		Send_Message_Player(PokerGuy,104,234,40,"Access Granted");
		Create_2D_WAV_Sound_Player(PokerGuy,"accessgranted.wav");
		int random = Commands->Get_Random_Int(1,3);
		int cp3id1 = Get_Int_Parameter("CP3_ID1");	
		int cp3id2 = Get_Int_Parameter("CP3_ID2");	
		int cp3id3 = Get_Int_Parameter("CP3_ID3");
		GameObject *gotoObjectcp3id1 = Commands->Find_Object(cp3id1);	
		GameObject *gotoObjectcp3id2 = Commands->Find_Object(cp3id2);	
		GameObject *gotoObjectcp3id3 = Commands->Find_Object(cp3id3);
		Vector3 gotoLocationcp3id1 = Commands->Get_Position(gotoObjectcp3id1);	
		Vector3 gotoLocationcp3id2 = Commands->Get_Position(gotoObjectcp3id2);	
		Vector3 gotoLocationcp3id3 = Commands->Get_Position(gotoObjectcp3id3);
		if (random == 1)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp3id1);
		}
		else if (random == 2)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp3id2);
		}
		else if (random == 3)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp3id3);
		}
	}
	else if (number == 104)
	{
		AllowPoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}
void z_Escort_CheckPT::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Check1Locked = false;
			Check2Locked = true;
			Check3Locked = true;
			Send_Message(19,169,255,"Access Granted");
			Send_Message(19,169,255,"Checkpoint One is now UNLOCKED.");
			Send_Message(19,169,255,"Checkpoint One is now UNLOCKED.");
			Send_Message(19,169,255,"Checkpoint One is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		else if (param == 2)
		{
			Check1Locked = true;
			Check2Locked = false;
			Check3Locked = true;
			Send_Message(19,169,255,"Access Granted");
			Send_Message(19,169,255,"Checkpoint Two is now UNLOCKED.");
			Send_Message(19,169,255,"Checkpoint Two is now UNLOCKED.");
			Send_Message(19,169,255,"Checkpoint Two is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		else if (param == 3)
		{
			Check1Locked = true;
			Check2Locked = true;
			Check3Locked = false;
			Send_Message(19,169,255,"Access Granted");
			Send_Message(19,169,255,"Checkpoint Three is now UNLOCKED.");
			Send_Message(19,169,255,"Checkpoint Three is now UNLOCKED.");
			Send_Message(19,169,255,"Checkpoint Three is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
	}
}

void z_Escort_MCV_CheckPT::Entered(GameObject *obj, GameObject *enter)
{
	if (_stricmp(Commands->Get_Preset_Name(enter),"Allied_MCV") == 0)
	{
		int msg = Get_Int_Parameter("Message");
		int prm = Get_Int_Parameter("Param");
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100117),msg,prm,0);//checkpt checker
		Destroy_Script();
	}
}

void z_Escort_MCV_Finish::Created(GameObject *obj)
{
	mcv = 0;
	trigger = true;
}
void z_Escort_MCV_Finish::Entered(GameObject *obj, GameObject *enter)
{
	if (_stricmp(Commands->Get_Preset_Name(enter),"Allied_MCV") == 0)
	{
		trigger = false;
		mcv = enter;
		Commands->Start_Timer(obj,this,2.5f,100);//deploytimer
		Commands->Start_Timer(obj,this,10.0f,101);//wintimer
	}
}
void z_Escort_MCV_Finish::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Send_Custom_Event(obj,mcv,1000,1,0);//send deploy message to mcv
	}
	else if (number == 101)
	{
		Console_Input("amsg Mission Accomplished");
		Console_Input("win 1");
	}
}

void z_Escort_MCV::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Create_2D_WAV_Sound("missionfailed.wav");
	Console_Input("amsg Mission Failed - The MCV died");
	Console_Input("win 0");
}

void z_Escort_Start::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,5430.0f,201);//start failed
	Commands->Start_Timer(obj,this,5370.0f,202);//start 1min
	Commands->Start_Timer(obj,this,5130.0,203);//start 5min//
	Commands->Start_Timer(obj,this,4830.0f,204);//start 10min
	Commands->Start_Timer(obj,this,4230.0f,205);//start 20min
	Commands->Start_Timer(obj,this,3630.0f,206);//start 30min
	Commands->Start_Timer(obj,this,2730.0,207);//start 45min
	Commands->Start_Timer(obj,this,1830.0f,208);//start 60min
	Commands->Start_Timer(obj,this,930.0f,209);//start 75min
	Commands->Start_Timer(obj,this,30.0f,210);//start 90min
	Commands->Start_Timer(obj,this,25.0f,211);//initialize timer
	Commands->Start_Timer(obj,this,20.0f,212);//announce objective
}
void z_Escort_Start::Timer_Expired(GameObject *obj, int number)
{
	if (number == 201)//failed
	{
		Console_Input("amsg Mission Failed. Time Expired");
		Commands->Create_2D_WAV_Sound("time_has_expired.wav");
		Console_Input("win 0");
	}
	else if (number == 202)//1min
	{
		Commands->Create_2D_WAV_Sound("1mr.wav");
		Send_Message(19,169,255,"1 minute remaining.");
	}
	else if (number == 203)//5min
	{
		Commands->Create_2D_WAV_Sound("5mr.wav");
		Send_Message(19,169,255,"5 minutes remaining.");
	}
	else if (number == 204)//10min
	{
		Commands->Create_2D_WAV_Sound("10mr.wav");
		Send_Message(19,169,255,"10 minutes remaining.");
	}
	else if (number == 205)//20min
	{
		Commands->Create_2D_WAV_Sound("20mr.wav");
		Send_Message(19,169,255,"20 minutes remaining.");
	}
	else if (number == 206)//30min
	{
		Commands->Create_2D_WAV_Sound("30mr.wav");
		Send_Message(19,169,255,"30 minutes remaining.");
	}
	else if (number == 207)//45min
	{
		Send_Message(19,169,255,"45 minutes remaining.");
	}
	else if (number == 208)//60min
	{
		Send_Message(19,169,255,"60 minutes remaining.");
	}
	else if (number == 209)//75min
	{
		Send_Message(19,169,255,"75 minutes remaining.");
	}
	else if (number == 210)//90min
	{
		Send_Message(19,169,255,"90 minutes remaining.");
	}
	else if (number == 211)//init timer
	{
		Commands->Create_2D_WAV_Sound("missiontimer.wav");
		Send_Message(19,169,255,"Mission timer initialized.");
	}
	else if (number == 212)//objective announce
	{
		Commands->Create_2D_WAV_Sound("objective1new.wav");
		Send_Message(19,169,255,"New Primary Mission Objective - Escort the MCV to our Naval Yard Area.");
	}
}

ScriptRegistrant<z_Escort_Start> z_Escort_Start_Registrant("z_Escort_Start","");
ScriptRegistrant<z_Escort_MCV> z_Escort_MCV_Registrant("z_Escort_MCV","");
ScriptRegistrant<z_Escort_MCV_Finish> z_Escort_MCV_Finish_Registrant("z_Escort_MCV_Finish","");
ScriptRegistrant<z_Escort_MCV_CheckPT> z_Escort_MCV_CheckPT_Registrant("z_Escort_MCV_CheckPT","Message=1000:int,Param=1:int");
ScriptRegistrant<z_Escort_CheckPT> z_Escort_CheckPT_Registrant("z_Escort_CheckPT","CP1_ID1=1:int,CP1_ID2=1:int,CP1_ID3=1:int,CP2_ID1=1:int,CP2_ID2=1:int,CP2_ID3=1:int,CP3_ID1=1:int,CP3_ID2=1:int,CP3_ID3=1:int");