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
#include "z_eveninglight.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"


void z_Eve_Start::Created(GameObject *obj)
{
	const char *scriptname;	Vector3 position;	GameObject *object;	float facing;
	scriptname = Get_Parameter("Script_Name");
	position = Get_Vector3_Parameter("Location");
	facing = Get_Float_Parameter("Facing");
	object = Commands->Create_Object("Invisible_Object",position);
	Commands->Set_Facing(object,facing);
	Commands->Attach_Script(object,"Test_Cinematic",scriptname);
	Commands->Start_Timer(obj,this,10.0f,101);
	Commands->Start_Timer(obj,this,14.0f,102);
	Commands->Start_Timer(obj,this,20.0f,103);
	Commands->Start_Timer(obj,this,23.0f,104);
	Commands->Start_Timer(obj,this,27.0f,105);
	Commands->Start_Timer(obj,this,31.0f,106);
	Commands->Start_Timer(obj,this,34.0f,107);
}
void z_Eve_Start::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("eve_briefing_01.wav");
		Send_Message(255,204,0,"Commanders, we have a hostage situation in the Hand of Nod.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("eve_briefing_02.wav");
		Send_Message(255,204,0,"We dropped you off as close as possible. We don't have much time before Nod start eliminating the hostages.");
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("eve_briefing_03.wav");
		Send_Message(255,204,0,"We have approximately 35 minutes.");
	}
	else if (number == 104)
	{
		Commands->Create_2D_WAV_Sound("eve_briefing_04.wav");
		Send_Message(255,204,0,"Primary Objective: Rescue all hostages.");
	}
	else if (number == 105)
	{
		Commands->Create_2D_WAV_Sound("eve_briefing_05.wav");
		Send_Message(255,204,0,"Secondary Objective: Destroy all enemy samsites.");
	}
	else if (number == 106)
	{
		Commands->Create_2D_WAV_Sound("eve_briefing_06.wav");
		Send_Message(255,204,0,"Mission Timer Initialized: 35 Minutes remaining.");
	}
	else if (number == 107)
	{
		Commands->Create_2D_WAV_Sound("eve_briefing_07.wav");
		Send_Message(255,204,0,"Good luck Commanders.");
	}
}

void z_Eve_Count_Sams::Created(GameObject *obj)
{
	count = 4;
}
void z_Eve_Count_Sams::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			count--;
			if (count == 3)
			{
				Send_Message(255,255,255,"3 remaining..");
				Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0041i1evag_snd.wav");
			}
			else if (count == 2)
			{
				Send_Message(255,255,255,"2 remaining..");
				Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0042i1evag_snd.wav");
			}
			else if (count == 1)
			{
				Send_Message(255,255,255,"1 remaining..");
				Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0043i1evag_snd.wav");
			}
			else if (count == 0)
			{
				Commands->Create_2D_WAV_Sound("objective2complete.wav");
				Send_Message(255,255,255,"Secondary Objective Complete");
				Commands->Start_Timer(obj,this,1.0f,101);
				Commands->Start_Timer(obj,this,4.0f,102);
				Commands->Start_Timer(obj,this,9.0f,103);
				Commands->Destroy_Object(Commands->Find_Object(100224));//frontdoorblocker
				Commands->Destroy_Object(Commands->Find_Object(100225));//bakcdoorblocker
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100238),1000,1,0);//start weapon dropoffs after sams are all dead
			}
		}
	}
}
void z_Eve_Count_Sams::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("eve_welldone.wav");
		Send_Message(255,204,0,"Well done Commander.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("eve_headintohand.wav");
		Send_Message(255,204,0,"Now head into the Hand of Nod and liberate those hostages.");
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("eve_sendingsupplies.wav");
		Send_Message(255,204,0,"I'm airdropping you additional supplies, go out and look for them.");
	}
}
void z_Eve_Count_Sams::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&count,5,1);
}


void z_Eve_DropRockets::Created(GameObject *obj)
{
	count = 0;
	Vector3 position1;	Vector3 position2;	Vector3 position3;
	GameObject *object1;	GameObject *object2;	GameObject *object3;
	position1 = Get_Vector3_Parameter("Location1"); position2 = Get_Vector3_Parameter("Location2");	position3 = Get_Vector3_Parameter("Location3");
	
	object1 = Commands->Create_Object("Invisible_Object",position1);
	Commands->Set_Facing(object1,90.0f);
	Commands->Attach_Script(object1,"Test_Cinematic","z_gdi_rocketdrop.txt");
	object2 = Commands->Create_Object("Invisible_Object",position2);
	Commands->Set_Facing(object2,90.0f);
	Commands->Attach_Script(object2,"Test_Cinematic","z_gdi_rocketdrop.txt");
	object3 = Commands->Create_Object("Invisible_Object",position3);
	Commands->Set_Facing(object3,90.0f);
	Commands->Attach_Script(object3,"Test_Cinematic","z_gdi_rocketdrop.txt");
	Commands->Start_Timer(obj,this,60.0f,100);
}

void z_Eve_DropRockets::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		if (count <= 50)
		{
			count++;
			Vector3 position1;	Vector3 position2;	Vector3 position3;
			GameObject *object1;	GameObject *object2;	GameObject *object3;
			position1 = Get_Vector3_Parameter("Location1"); position2 = Get_Vector3_Parameter("Location2");	position3 = Get_Vector3_Parameter("Location3");
	
			object1 = Commands->Create_Object("Invisible_Object",position1);
			Commands->Set_Facing(object1,90.0f);
			Commands->Attach_Script(object1,"Test_Cinematic","z_gdi_rocketdrop.txt");
			object2 = Commands->Create_Object("Invisible_Object",position2);
			Commands->Set_Facing(object2,90.0f);
			Commands->Attach_Script(object2,"Test_Cinematic","z_gdi_rocketdrop.txt");
			object3 = Commands->Create_Object("Invisible_Object",position3);
			Commands->Set_Facing(object3,90.0f);
			Commands->Attach_Script(object3,"Test_Cinematic","z_gdi_rocketdrop.txt");
			Commands->Start_Timer(obj,this,60.0f,100);
		}
	}
	else
	{
		Destroy_Script();
	}
}
void z_Eve_DropRockets::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&count,5,1);
}

void z_Eve_Death_Drop_Rockets::Killed(GameObject *obj, GameObject *shooter)
{
	Vector3 position1;	Vector3 position2;	Vector3 position3;
	GameObject *object1;	GameObject *object2;	GameObject *object3;
	position1 = Get_Vector3_Parameter("Location1"); position2 = Get_Vector3_Parameter("Location2");	position3 = Get_Vector3_Parameter("Location3");
	
	object1 = Commands->Create_Object("Invisible_Object",position1);
	Commands->Set_Facing(object1,90.0f);
	Commands->Attach_Script(object1,"Test_Cinematic","z_gdi_rocketdrop.txt");

	object2 = Commands->Create_Object("Invisible_Object",position2);
	Commands->Set_Facing(object2,90.0f);
	Commands->Attach_Script(object2,"Test_Cinematic","z_gdi_rocketdrop.txt");

	object3 = Commands->Create_Object("Invisible_Object",position3);
	Commands->Set_Facing(object3,90.0f);
	Commands->Attach_Script(object3,"Test_Cinematic","z_gdi_rocketdrop.txt");

	Commands->Start_Timer(obj,this,60.0f,100);
}

void z_Eve_SAMsDead_DropWeps::Created(GameObject *obj)
{
	startnow = Get_Int_Parameter("StartNow");
	startnowdelay = Get_Float_Parameter("StartNowDelay");
	id1 = Get_Int_Parameter("LocationID1");
	id2 = Get_Int_Parameter("LocationID2");
	id3 = Get_Int_Parameter("LocationID3");
	loop = Get_Int_Parameter("Loop");
	msg = Get_Int_Parameter("Message");
	delay = Get_Float_Parameter("Delay");
	loopsmax = Get_Int_Parameter("LoopMax");
	scriptname = Get_Parameter("Script_Name");
	loops = 0;
}
void z_Eve_SAMsDead_DropWeps::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == msg)
	{
		if (param == 1)
		{
			if (startnow)
			{
				Commands->Start_Timer(obj,this,startnowdelay,100);
			}
			if (loop)
			{
				Commands->Start_Timer(obj,this,delay,101);
			}
		}
	}
}
void z_Eve_SAMsDead_DropWeps::Timer_Expired(GameObject *obj,int number)
{
	if (number == 100)
	{
		Vector3 position1;
		GameObject *object1;
		position1 = Commands->Get_Position(Commands->Find_Object(id1));
		object1 = Commands->Create_Object("Invisible_Object",position1);
		Commands->Set_Facing(object1,55.0f);
		Commands->Attach_Script(object1,"Test_Cinematic",scriptname);

		Vector3 position2;
		GameObject *object2;
		position2 = Commands->Get_Position(Commands->Find_Object(id2));
		object2 = Commands->Create_Object("Invisible_Object",position2);
		Commands->Set_Facing(object2,55.0f);
		Commands->Attach_Script(object2,"Test_Cinematic",scriptname);

		Vector3 position3;
		GameObject *object3;
		position3 = Commands->Get_Position(Commands->Find_Object(id3));
		object3 = Commands->Create_Object("Invisible_Object",position3);
		Commands->Set_Facing(object3,55.0f);
		Commands->Attach_Script(object3,"Test_Cinematic",scriptname);
	}
	else if (number == 101)
	{
		Vector3 position1;
		GameObject *object1;
		position1 = Commands->Get_Position(Commands->Find_Object(id1));
		object1 = Commands->Create_Object("Invisible_Object",position1);
		Commands->Set_Facing(object1,55.0f);
		Commands->Attach_Script(object1,"Test_Cinematic",scriptname);

		Vector3 position2;
		GameObject *object2;
		position2 = Commands->Get_Position(Commands->Find_Object(id2));
		object2 = Commands->Create_Object("Invisible_Object",position2);
		Commands->Set_Facing(object2,55.0f);
		Commands->Attach_Script(object2,"Test_Cinematic",scriptname);

		Vector3 position3;
		GameObject *object3;
		position3 = Commands->Get_Position(Commands->Find_Object(id3));
		object3 = Commands->Create_Object("Invisible_Object",position3);
		Commands->Set_Facing(object3,55.0f);
		Commands->Attach_Script(object3,"Test_Cinematic",scriptname);
		if (loop)
		{
			loops++;
			if (loops <= loopsmax)//if number of current loops is less/equal to the max number of loops then do a dropoff else do destroy script and stop doing dropoffs...
			{
				Commands->Start_Timer(obj,this,delay,101);
			}
			else
			{
				Destroy_Script();
			}
		}
	}
}
void z_Eve_SAMsDead_DropWeps::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&id1,5,1);
	Auto_Save_Variable(&id2,5,1);
	Auto_Save_Variable(&id3,5,1);
	Auto_Save_Variable(&loop,5,1);
	Auto_Save_Variable(&startnow,5,1);
	Auto_Save_Variable(&msg,5,1);
	Auto_Save_Variable(&delay,5,1);
	Auto_Save_Variable(&startnowdelay,5,1);
	Auto_Save_Variable(&loops,5,1);
	Auto_Save_Variable(&loopsmax,5,1);
}

void z_Eve_Hostage::Created(GameObject *obj)
{
	allowpoke = true;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	Commands->Innate_Disable(obj);
}
void z_Eve_Hostage::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		allowpoke = false;
		Commands->Set_Player_Type(obj,1);
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Commands->Give_PowerUp(obj,"POW_AutoRifle_Player",false);
		Commands->Select_Weapon(obj,"Weapon_AutoRifle_Player");
		Commands->Innate_Enable(obj);
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100312),1000,1,0);
	}
}
void z_Eve_Hostage::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Commands->Set_Player_Type(obj,1);
		}
	}
}
void z_Eve_Hostage::Killed(GameObject *obj, GameObject *shooter)
{
	Console_Input("amsg Mission Failed! A hostage was terminated.");
	Console_Input("win 0");
}
void z_Eve_Hostage::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowpoke,5,1);
}

void z_Eve_Drop_Engineers::Created(GameObject *obj)
{
	int id1 = Get_Int_Parameter("Loc_ID");
	Vector3 position1;
	GameObject *object1;
	position1 = Commands->Get_Position(Commands->Find_Object(id1));
	object1 = Commands->Create_Object("Invisible_Object",position1);
	Commands->Set_Facing(object1,75.0f);
	Commands->Attach_Script(object1,"Test_Cinematic","z_eve_engineer_drop.txt");
}
void z_Eve_Drop_Engineers::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			int id1 = Get_Int_Parameter("Loc_ID");
			Vector3 position1;
			GameObject *object1;
			position1 = Commands->Get_Position(Commands->Find_Object(id1));
			object1 = Commands->Create_Object("Invisible_Object",position1);
			Commands->Set_Facing(object1,75.0f);
			Commands->Attach_Script(object1,"Test_Cinematic","z_eve_engineer_drop.txt");
		}
	}
}

void z_Eve_Enter_SetHosties::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		Commands->Set_Player_Type(Commands->Find_Object(100241),1);
		Commands->Set_Player_Type(Commands->Find_Object(100242),1);
		Commands->Set_Player_Type(Commands->Find_Object(100243),1);
		Commands->Set_Player_Type(Commands->Find_Object(100244),1);
		Commands->Set_Player_Type(Commands->Find_Object(100245),1);
		Commands->Set_Player_Type(Commands->Find_Object(100246),1);
	}
}

void z_Eve_Hostage_Count::Created(GameObject *obj)
{
	saves = 0;
}
void z_Eve_Hostage_Count::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			saves++;
			if (saves == 8)
			{
				Commands->Start_Timer(obj,this,2.0f,101);
				Commands->Start_Timer(obj,this,7.0f,102);
			}
		}
	}
}
void z_Eve_Hostage_Count::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("eve_welldonedropion.wav");
		Send_Message(255,204,0,"Well done Commander. I'll be airdropping you an Ion Cannon.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("eve_ionpedend.wav");
		Send_Message(255,204,0,"Ion the Pedestal to end the mission.");

		int id1 = Get_Int_Parameter("Loc_ID");
		Vector3 position1;
		GameObject *object1;
		position1 = Commands->Get_Position(Commands->Find_Object(id1));
		object1 = Commands->Create_Object("Invisible_Object",position1);
		Commands->Set_Facing(object1,75.0f);
		Commands->Attach_Script(object1,"Test_Cinematic","z_eve_drop_ion.txt");
		Commands->Start_Timer(obj,this,60.0f,102);
	}
}
void z_Eve_Hostage_Count::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&saves,5,1);
}

void z_Eve_End_Mission::Killed(GameObject *obj, GameObject *shooter)
{
	Console_Input("amsg Mission Accomplished");
	Commands->Create_2D_WAV_Sound("missionaccomplished.wav");
}

ScriptRegistrant<z_Eve_End_Mission> z_Eve_End_Mission_Registrant("z_Eve_End_Mission","");
ScriptRegistrant<z_Eve_Hostage_Count> z_Eve_Hostage_Count_Registrant("z_Eve_Hostage_Count","Loc_ID");
ScriptRegistrant<z_Eve_Enter_SetHosties> z_Eve_Enter_SetHosties_Registrant("z_Eve_Enter_SetHosties","");
ScriptRegistrant<z_Eve_Drop_Engineers> z_Eve_Drop_Engineers_Registrant("z_Eve_Drop_Engineers","Loc_ID=1:int");
ScriptRegistrant<z_Eve_Hostage> z_Eve_Hostage_Registrant("z_Eve_Hostage","");
ScriptRegistrant<z_Eve_SAMsDead_DropWeps> z_Eve_SAMsDead_DropWeps_Registrant("z_Eve_SAMsDead_DropWeps","StartNow=0:int,StartNowDelay=0:float,LocationID1=1:int,LocationID2=1:int,LocationID3=1:int,Loop=1:int,Message=1000:int,Delay=180000:float,LoopMax=50:int,Script_Name=asdf:string");
ScriptRegistrant<z_Eve_Death_Drop_Rockets> z_Eve_Death_Drop_Rockets_Registrant("z_Eve_Death_Drop_Rockets","Location1=0:vector3,Location2=0:vector3,Location3=0:vector3");
ScriptRegistrant<z_Eve_DropRockets> z_Eve_DropRockets_Registrant("z_Eve_DropRockets","Location1=0:vector3,Location2=0:vector3,Location3=0:vector3");
ScriptRegistrant<z_Eve_Count_Sams> z_Eve_Count_Sams_Registrant("z_Eve_Count_Sams","");
ScriptRegistrant<z_Eve_Start> z_Eve_Start_Registrant("z_Eve_Start","Script_Name=z_gdi_dropoff:string,Location=0:vector3,Facing=90:float");





