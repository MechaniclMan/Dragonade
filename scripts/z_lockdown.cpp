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
#include "z_lockdown.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"

void z_Lockdown_Bar::Created(GameObject *obj)
{
	count = 0;
}
void z_Lockdown_Bar::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			count++;
			if (count == 2)
			{
				Send_Message(255,255,255,"The Allied Barracks is no longer internally repaired by Engineers");
				Destroy_Script();
			}
		}
	}
}
void z_Lockdown_Bar::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Get_Health(obj) <= 50.0f)
	{
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	}
}
void z_Lockdown_Bar::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Enable_Spawner(100178,false);
	Commands->Enable_Spawner(100179,false);
	Commands->Enable_Spawner(100180,false);
	Commands->Enable_Spawner(100181,false);
	Commands->Enable_Spawner(100182,false);
	Commands->Enable_Spawner(100183,false);
	Commands->Enable_Spawner(100184,false);
	Commands->Enable_Spawner(100185,false);
	Commands->Enable_Spawner(100186,false);
	Commands->Enable_Spawner(100187,false);
	Commands->Enable_Spawner(100188,false);
	Commands->Enable_Spawner(100189,false);
	Commands->Enable_Spawner(100512,false);
	Commands->Enable_Spawner(100513,false);
	Commands->Enable_Spawner(100520,false);
	Commands->Enable_Spawner(100521,false);
	Commands->Enable_Spawner(100522,false);
	Commands->Enable_Spawner(100532,false);
	Commands->Enable_Spawner(100534,false);
	Commands->Enable_Spawner(100535,false);
	Commands->Enable_Spawner(100536,false);
	Commands->Enable_Spawner(100547,false);
	Commands->Enable_Spawner(100549,false);
	Commands->Enable_Spawner(100551,false);
	Commands->Enable_Spawner(100552,false);
	Commands->Enable_Spawner(100585,false);
	Commands->Enable_Spawner(100596,false);
	Commands->Enable_Spawner(100514,false);
	Commands->Enable_Spawner(100515,false);
	Commands->Enable_Spawner(100516,false);
	Commands->Enable_Spawner(100523,false);
	Commands->Enable_Spawner(100524,false);
	Commands->Enable_Spawner(100537,false);
	Commands->Enable_Spawner(100539,false);
	Commands->Enable_Spawner(100540,false);
	Commands->Enable_Spawner(100553,false);
	Commands->Enable_Spawner(100555,false);
	Commands->Enable_Spawner(100556,false);
	Commands->Enable_Spawner(100557,false);
	Commands->Enable_Spawner(100601,false);
	Commands->Enable_Spawner(100602,false);
	Commands->Enable_Spawner(100597,false);
	Commands->Enable_Spawner(100598,false);
	Commands->Enable_Spawner(100599,false);
	Commands->Enable_Spawner(100600,false);
	Commands->Enable_Spawner(100586,false);
}
void z_Lockdown_Ref::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Enable_Spawner(100183,false);
	Commands->Enable_Spawner(100181,false);
	Commands->Enable_Spawner(100187,false);
	Commands->Enable_Spawner(100186,false);
	Commands->Enable_Spawner(100189,false);
	Commands->Enable_Spawner(100512,false);
	Commands->Enable_Spawner(100515,false);
	Commands->Enable_Spawner(100520,false);
	Commands->Enable_Spawner(100521,false);
	Commands->Enable_Spawner(100523,false);
	Commands->Enable_Spawner(100532,false);
	Commands->Enable_Spawner(100536,false);
	Commands->Enable_Spawner(100537,false);
	Commands->Enable_Spawner(100541,false);
	Commands->Enable_Spawner(100549,false);
	Commands->Enable_Spawner(100552,false);
	Commands->Enable_Spawner(100553,false);
	Commands->Enable_Spawner(100557,false);
	Commands->Enable_Spawner(100585,false);
	Commands->Enable_Spawner(100586,false);

	Commands->Enable_Spawner(100622,false);
	Commands->Enable_Spawner(100627,false);
	Commands->Enable_Spawner(100617,false);
	Commands->Enable_Spawner(100619,false);
	Commands->Enable_Spawner(100607,false);
	Commands->Enable_Spawner(100614,false);
}

void z_Death_Disable10Spawners::Killed(GameObject *obj, GameObject *shooter)
{
	int option = Get_Int_Parameter("Enable");
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner01"),option);
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner02"),option);
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner03"),option);
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner04"),option);
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner05"),option);
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner06"),option);
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner07"),option);
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner08"),option);
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner09"),option);
	Commands->Enable_Spawner(Get_Int_Parameter("Spawner10"),option);
}

void z_LockdownMissionStart::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,10.0f,101);
	Commands->Start_Timer(obj,this,15.0f,102);
	Commands->Start_Timer(obj,this,20.0f,103);
	Commands->Start_Timer(obj,this,25.0f,104);
	Commands->Start_Timer(obj,this,30.0f,105);
}
void z_LockdownMissionStart::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,0,0,"Welcome back commanders.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,0,0,"Since our last invasion failed we get one more chance of eliminating the Allied Forces.");
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,0,0,"We locked down the Allied Base with our Helipad outposts.");
	}
	else if (number == 104)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,0,0,"Co-ordinate a final crushing attack and wipe the Allies off the face of the earth.");
	}
	else if (number == 105)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,0,0,"Do not return home before the Allied Forces are crushed. Comrades!");
	}
}

void z_Lockdown_Endgame::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	int msg;
	const char *script1;
	const char *script2;
	const char *script3;
	const char *script4;
	const char *script5;
	Vector3 pos1;
	Vector3 pos2;
	Vector3 pos3;
	Vector3 pos4;
	Vector3 pos5;
	GameObject *object1;
	GameObject *object2;
	GameObject *object3;
	GameObject *object4;
	GameObject *object5;
	float facing1;
	float facing2;
	float facing3;
	float facing4;
	float facing5;
	msg = Get_Int_Parameter("Message");
	script1 = Get_Parameter("Script1");
	script2 = Get_Parameter("Script2");
	script3 = Get_Parameter("Script3");
	script4 = Get_Parameter("Script4");
	script5 = Get_Parameter("Script5");
	pos1 = Get_Vector3_Parameter("Loc1");
	pos2 = Get_Vector3_Parameter("Loc2");
	pos3 = Get_Vector3_Parameter("Loc3");
	pos4 = Get_Vector3_Parameter("Loc4");
	pos5 = Get_Vector3_Parameter("Loc5");
	facing1 = Get_Float_Parameter("Fac1");
	facing2 = Get_Float_Parameter("Fac2");
	facing3 = Get_Float_Parameter("Fac3");
	facing4 = Get_Float_Parameter("Fac4");
	facing5 = Get_Float_Parameter("Fac5");
	if (message == msg)
	{
		object1 = Commands->Create_Object("Invisible_Object",pos1);
		Commands->Set_Facing(object1,facing1);
		Commands->Attach_Script(object1,"Test_Cinematic",script1);

		object2 = Commands->Create_Object("Invisible_Object",pos2);
		Commands->Set_Facing(object2,facing2);
		Commands->Attach_Script(object2,"Test_Cinematic",script2);

		object3 = Commands->Create_Object("Invisible_Object",pos3);
		Commands->Set_Facing(object3,facing3);
		Commands->Attach_Script(object3,"Test_Cinematic",script3);

		object4 = Commands->Create_Object("Invisible_Object",pos4);
		Commands->Set_Facing(object4,facing4);
		Commands->Attach_Script(object4,"Test_Cinematic",script4);

		object5 = Commands->Create_Object("Invisible_Object",pos5);
		Commands->Set_Facing(object5,facing5);
		Commands->Attach_Script(object5,"Test_Cinematic",script5);

		Commands->Start_Timer(obj,this,120.0f,100);
		Commands->Start_Timer(obj,this,90.0f,102);

	}
}

void z_Lockdown_Endgame::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Console_Input("amsg Mission Accomplished! You may now return home comrades!");
		Console_Input("win 0");
		Commands->Start_Timer(obj,this,2.5f,101);
	}
	else if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("we_owned_them.wav");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("a_fallen.wav");
		Send_Message(255,0,0,"Allied Forces have fallen!");
	}
}


void z_Lockdown_WF::Created(GameObject *obj)
{
	count = 0;
}
void z_Lockdown_WF::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			count++;
			if (count == 2)
			{
				Send_Message(255,255,255,"The Allied War Factory is no longer internally repaired by Engineers");
				Destroy_Script();
			}
		}
	}
}
void z_Lockdown_WF::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Get_Health(obj) <= 50.0f)
	{
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	}
}

ScriptRegistrant<z_Lockdown_WF> z_Lockdown_WF_Registrant("z_Lockdown_WF","");
ScriptRegistrant<z_Lockdown_Endgame> z_Lockdown_Endgame_Registrant("z_Lockdown_Endgame","Message=1000:int,Script1=1:string,Script2=1:string,Script3=1:string,Script4=1:string,Script5=1:string,Loc1:vector3,Loc2:vector3,Loc3:vector3,Loc4:vector3,Loc5:vector3,Fac1:float,Fac2:float,Fac3:float,Fac4:float,Fac5:float");
ScriptRegistrant<z_LockdownMissionStart> z_LockdownMissionStart_Registrant("z_LockdownMissionStart","");
ScriptRegistrant<z_Death_Disable10Spawners> z_Death_Disable10Spawners_Registrant("z_Death_Disable10Spawners","Spawner01=1:int,Spawner02=1:int,Spawner03=1:int,Spawner04=1:int,Spawner05=1:int,Spawner06=1:int,Spawner07=1:int,Spawner08=1:int,Spawner09=1:int,Spawner10=1:int,Enable=1:int");
ScriptRegistrant<z_Lockdown_Bar> z_Lockdown_Bar_Registrant("z_Lockdown_Bar","");
ScriptRegistrant<z_Lockdown_Ref> z_Lockdown_Ref_Registrant("z_Lockdown_Ref","");
