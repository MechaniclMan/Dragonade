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
#include "engine_tt.h"
#include "engine.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"
#include "z_reconnaissance.h"
#include "BaseControllerClass.h"
#include "GameObjManager.h"

void z_rec_CheckpointControl::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	PokerGuy = 0;
	AllowPoke = true;
	Check1Locked = true;
	Check2Locked = true;
	Check3Locked = true;
	Check4Locked = true;
	Check5Locked = true;
	Check6Locked = true;
}
void z_rec_CheckpointControl::Poked(GameObject *obj, GameObject *poker)
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
		else if (Check4Locked == false)
		{
			Commands->Start_Timer(obj,this,0.0f,104);//teleport the guy
		}
		else if (Check5Locked == false)
		{
			Commands->Start_Timer(obj,this,0.0f,105);//teleport the guy
		}
		else if (Check6Locked == false)
		{
			Commands->Start_Timer(obj,this,0.0f,106);//teleport the guy
		}
		else
		{
			Send_Message(104,234,40,"Access Denied");
			Commands->Create_2D_WAV_Sound("accessdenied.wav");
		}
		Commands->Start_Timer(obj,this,2.0f,107);//allowpoke true
	}
}
void z_rec_CheckpointControl::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Send_Message_Player(PokerGuy,104,234,40,"Access Granted");
		Create_2D_WAV_Sound_Player(PokerGuy,"accessgranted.wav");
		int random = Commands->Get_Random_Int(1,3);
		int cp1id1 = 1234;
		int cp1id2 = 1234;
		int cp1id3 = 1234;
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
		int cp2id1 = 1234;
		int cp2id2 = 1234;
		int cp2id3 = 1234;
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
		int cp3id1 = 1234;
		int cp3id2 = 1234;
		int cp3id3 = 1234;
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
		Send_Message_Player(PokerGuy,104,234,40,"Access Granted");
		Create_2D_WAV_Sound_Player(PokerGuy,"accessgranted.wav");
		int random = Commands->Get_Random_Int(1,3);
		int cp4id1 = 1234;
		int cp4id2 = 1234;
		int cp4id3 = 1234;
		GameObject *gotoObjectcp4id1 = Commands->Find_Object(cp4id1);	
		GameObject *gotoObjectcp4id2 = Commands->Find_Object(cp4id2);	
		GameObject *gotoObjectcp4id3 = Commands->Find_Object(cp4id3);
		Vector3 gotoLocationcp4id1 = Commands->Get_Position(gotoObjectcp4id1);	
		Vector3 gotoLocationcp4id2 = Commands->Get_Position(gotoObjectcp4id2);	
		Vector3 gotoLocationcp4id3 = Commands->Get_Position(gotoObjectcp4id3);
		if (random == 1)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp4id1);
		}
		else if (random == 2)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp4id2);
		}
		else if (random == 3)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp4id3);
		}
	}
	else if (number == 105)
	{
		Send_Message_Player(PokerGuy,104,234,40,"Access Granted");
		Create_2D_WAV_Sound_Player(PokerGuy,"accessgranted.wav");
		int random = Commands->Get_Random_Int(1,3);
		int cp5id1 = 1234;
		int cp5id2 = 1234;
		int cp5id3 = 1234;
		GameObject *gotoObjectcp5id1 = Commands->Find_Object(cp5id1);	
		GameObject *gotoObjectcp5id2 = Commands->Find_Object(cp5id2);	
		GameObject *gotoObjectcp5id3 = Commands->Find_Object(cp5id3);
		Vector3 gotoLocationcp5id1 = Commands->Get_Position(gotoObjectcp5id1);	
		Vector3 gotoLocationcp5id2 = Commands->Get_Position(gotoObjectcp5id2);	
		Vector3 gotoLocationcp5id3 = Commands->Get_Position(gotoObjectcp5id3);
		if (random == 1)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp5id1);
		}
		else if (random == 2)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp5id2);
		}
		else if (random == 3)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp5id3);
		}
	}
	else if (number == 106)
	{
		Send_Message_Player(PokerGuy,104,234,40,"Access Granted");
		Create_2D_WAV_Sound_Player(PokerGuy,"accessgranted.wav");
		int random = Commands->Get_Random_Int(1,3);
		int cp6id1 = 1234;
		int cp6id2 = 1234;
		int cp6id3 = 1234;
		GameObject *gotoObjectcp6id1 = Commands->Find_Object(cp6id1);	
		GameObject *gotoObjectcp6id2 = Commands->Find_Object(cp6id2);	
		GameObject *gotoObjectcp6id3 = Commands->Find_Object(cp6id3);
		Vector3 gotoLocationcp6id1 = Commands->Get_Position(gotoObjectcp6id1);	
		Vector3 gotoLocationcp6id2 = Commands->Get_Position(gotoObjectcp6id2);	
		Vector3 gotoLocationcp6id3 = Commands->Get_Position(gotoObjectcp6id3);
		if (random == 1)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp6id1);
		}
		else if (random == 2)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp6id2);
		}
		else if (random == 3)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp6id3);
		}
	}
	else if (number == 107)
	{
		AllowPoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}
void z_rec_CheckpointControl::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Check1Locked = false;
			Check2Locked = true;
			Check3Locked = true;
			Check4Locked = true;
			Check5Locked = true;
			Check6Locked = true;
			Send_Message(255,204,0,"Access Granted");
			Send_Message(255,204,0,"Checkpoint One is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint One is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint One is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		else if (param == 2)
		{
			Check1Locked = true;
			Check2Locked = false;
			Check3Locked = true;
			Check4Locked = true;
			Check5Locked = true;
			Check6Locked = true;
			Send_Message(255,204,0,"Access Granted");
			Send_Message(255,204,0,"Checkpoint Two is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Two is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Two is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		else if (param == 3)
		{
			Check1Locked = true;
			Check2Locked = true;
			Check3Locked = false;
			Check4Locked = true;
			Check5Locked = true;
			Check6Locked = true;
			Send_Message(255,204,0,"Access Granted");
			Send_Message(255,204,0,"Checkpoint Three is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Three is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Three is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		else if (param == 4)
		{
			Check1Locked = true;
			Check2Locked = true;
			Check3Locked = true;
			Check4Locked = false;
			Check5Locked = true;
			Check6Locked = true;
			Send_Message(255,204,0,"Access Granted");
			Send_Message(255,204,0,"Checkpoint Four is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Four is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Four is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		else if (param == 5)
		{
			Check1Locked = true;
			Check2Locked = true;
			Check3Locked = true;
			Check4Locked = true;
			Check5Locked = false;
			Check6Locked = true;
			Send_Message(255,204,0,"Access Granted");
			Send_Message(255,204,0,"Checkpoint Five is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Five is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Five is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		else if (param == 3)
		{
			Check1Locked = true;
			Check2Locked = true;
			Check3Locked = true;
			Check4Locked = true;
			Check5Locked = true;
			Check6Locked = false;
			Send_Message(255,204,0,"Access Granted");
			Send_Message(255,204,0,"Checkpoint Six is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Six is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Six is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
	}
}
void z_rec_CheckpointControl::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&Check1Locked,5,1);
	Auto_Save_Variable(&Check2Locked,5,1);
	Auto_Save_Variable(&Check3Locked,5,1);
	Auto_Save_Variable(&Check4Locked,5,1);
	Auto_Save_Variable(&Check5Locked,5,1);
	Auto_Save_Variable(&Check6Locked,5,1);
	Auto_Save_Variable(&AllowPoke,5,1);
	Auto_Save_Variable(&PokerGuy,5,1);
}


ScriptRegistrant<z_rec_CheckpointControl> z_rec_CheckpointControl_Registrant("z_rec_CheckpointControl","");