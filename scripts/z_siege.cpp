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
#include "z_siege.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"

void z_Siege_Mission_Start::Created(GameObject *obj)
{
	Commands->Apply_Damage(Commands->Find_Object(150449),2500.0f,"Death",0);//killref
	//Commands->Apply_Damage(Commands->Find_Object(150448),2500.0f,"Death",0);//killsd
	Commands->Apply_Damage(Commands->Find_Object(150447),2500.0f,"Death",0);//kilbar

	Vector3 pos1;
	//allied refinery
	pos1.X = -19.686f;	pos1.Y = -668.997f; pos1.Z = 9.709f;
	GameObject *aref = Commands->Create_Object("Allied_Engineer",pos1);
	Commands->Set_Facing(aref,0.000f);
	Attach_Script_Once(aref,"z_Buy_Allied_Ref","");
	Attach_Script_Once(aref,"M00_Damage_Modifier_DME","0.00,1,1,0,0");

	Vector3 pos2;
	//allied bar
	pos2.X = -94.096f;	pos2.Y = -637.364f; pos2.Z = -4.102f;
	GameObject *abar = Commands->Create_Object("Allied_Engineer",pos2);
	Commands->Set_Facing(abar,16.785f);
	Attach_Script_Once(abar,"z_Buy_Allied_Bar","");
	Attach_Script_Once(abar,"M00_Damage_Modifier_DME","0.00,1,1,0,0");

	Vector3 pos3;
	//allied sd
	pos3.X = 99.085f;	pos3.Y = -541.053f; pos3.Z = -13.017f;
	GameObject *asd = Commands->Create_Object("Allied_Mechanic",pos3);
	Commands->Set_Facing(asd,-106.872f);
	Attach_Script_Once(asd,"z_Buy_Allied_SD","");
	Attach_Script_Once(asd,"M00_Damage_Modifier_DME","0.00,1,1,0,0");

	Commands->Start_Timer(obj,this,300.0f,101);

	Commands->Start_Timer(obj,this,10.0f,100);
	Commands->Start_Timer(obj,this,14.0f,102);
	Commands->Start_Timer(obj,this,18.0f,103);
}
void z_Siege_Mission_Start::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Send_Message(255,255,255,"Enemy forces are approaching...");
		Commands->Enable_Spawner(100730,true);//mammy1 wf
		Commands->Enable_Spawner(100731,true);//mammy2 wf
		Commands->Enable_Spawner(100732,true);//mammy3 cy

		Commands->Enable_Spawner(100736,true);
		Commands->Enable_Spawner(100742,true);
		Commands->Enable_Spawner(100737,true);
		Commands->Enable_Spawner(100741,true);
		Commands->Enable_Spawner(100739,true);
		Commands->Enable_Spawner(100740,true);
	}
	else if (number == 100)
	{
		Send_Message(255,255,255,"Welcome back commander.");
	}
	else if (number == 102)
	{
		Send_Message(255,255,255,"The Soviet forces have invaded our base in Afghanistan and crippled it. Restore the Refinery and the Barracks");
	}
	else if (number == 103)
	{
		Send_Message(255,255,255,"and keep the Construction Yard alive at all cost. Good luck!");
	}
}

void z_Siege_SovBaseActivateZone::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		if (Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY) && Commands->Get_Health(Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY)) >= 1.0f)//only enable if bar is alive
		{
			Commands->Enable_Spawner(100732,true);
			Commands->Enable_Spawner(100732,true);
			Commands->Enable_Spawner(100732,true);
			Commands->Enable_Spawner(100732,true);
		}
		if (Find_Building_By_Type(0,BuildingConstants::TYPE_VEHICLE_FACTORY) && Commands->Get_Health(Find_Building_By_Type(0,BuildingConstants::TYPE_VEHICLE_FACTORY)) >= 1.0f)//only enable if wf is alive
		{
			Commands->Enable_Spawner(100732,true);
			Commands->Enable_Spawner(100732,true);
			Commands->Enable_Spawner(100732,true);
			Commands->Enable_Spawner(100732,true);
		}
		Destroy_Script();
	}
}

void z_Siege_CoilActivationZone::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		if (Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY) && Commands->Get_Health(Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY)) >= 1.0f)//only enable if bar is alive
		{
			Commands->Enable_Spawner(100732,true);
			Commands->Enable_Spawner(100732,true);
			Commands->Enable_Spawner(100732,true);
			Commands->Enable_Spawner(100732,true);
		}
		if (Find_Building_By_Type(0,BuildingConstants::TYPE_VEHICLE_FACTORY) && Commands->Get_Health(Find_Building_By_Type(0,BuildingConstants::TYPE_VEHICLE_FACTORY)) >= 1.0f)//only enable if wf is alive
		{
			Commands->Enable_Spawner(100732,true);
			Commands->Enable_Spawner(100732,true);
			Commands->Enable_Spawner(100732,true);
			Commands->Enable_Spawner(100732,true);
		}
		Destroy_Script();
	}
}

void z_ZoneEnter_GotoStarAttach::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		int tank1 = Get_Int_Parameter("ID1");
		int tank2 = Get_Int_Parameter("ID2");
		int tank3 = Get_Int_Parameter("ID3");
		int tank4 = Get_Int_Parameter("ID4");
		int tank5 = Get_Int_Parameter("ID5");

		Attach_Script_Once(Commands->Find_Object(tank1),"M03_Goto_Star","");
		Attach_Script_Once(Commands->Find_Object(tank2),"M03_Goto_Star","");
		Attach_Script_Once(Commands->Find_Object(tank3),"M03_Goto_Star","");
		Attach_Script_Once(Commands->Find_Object(tank4),"M03_Goto_Star","");
		Attach_Script_Once(Commands->Find_Object(tank5),"M03_Goto_Star","");
	}
}

void z_Kill_LB_Exit::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Start_Timer(obj,this,30.0f,100);	
	}
}
void z_Kill_LB_Exit::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Apply_Damage(obj,2500.0f,"Death",0);//killkillkill
	}
}

ScriptRegistrant<z_Kill_LB_Exit> z_Kill_LB_Exit_Registrant("z_Kill_LB_Exit","");
ScriptRegistrant<z_ZoneEnter_GotoStarAttach> z_ZoneEnter_GotoStarAttach_Registrant("z_ZoneEnter_GotoStarAttach","ID1=1:int,ID2=1:int,ID3=1:int,ID4=1:int,ID5=1:int");
ScriptRegistrant<z_Siege_CoilActivationZone> z_Siege_CoilActivationZone_Registrant("z_Siege_CoilActivationZone","");
ScriptRegistrant<z_Siege_SovBaseActivateZone> z_Siege_SovBaseActivateZone_Registrant("z_Siege_SovBaseActivateZone","");
ScriptRegistrant<z_Siege_Mission_Start> z_Siege_Mission_Start_Registrant("z_Siege_Mission_Start","");