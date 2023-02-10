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
#include "z_endgame.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"

void z_Endgame_AAPP::Created(GameObject *obj)
{
	count = 0;
}
void z_Endgame_AAPP::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			count++;
			if (count == 4)
			{
				Send_Message(255,255,255,"The Allied Power Plant is no longer being internally repaired by Engineers.");
				Destroy_Script();
			}
		}
	}
}
void z_Endgame_AAPP::Damaged(GameObject *obj,	GameObject *damager, float damage)
{
	if (Commands->Get_Health(obj) <= 50.0f)
	{
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	}
}

void z_Endgame_MissionTimer::Created(GameObject *obj)
{
	dodemomsg = true;
	docymsg = true;
	Commands->Start_Timer(obj,this,15.0f,101);
	Commands->Start_Timer(obj,this,35.0f,102);
	Commands->Start_Timer(obj,this,55.0f,103);
	Commands->Start_Timer(obj,this,70.0f,134);
	Attach_Script_Once(Commands->Find_Object(100405),"z_Death_Send_Custom","100525,1000,1");
}
void z_Endgame_MissionTimer::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Send_Message(255,255,255,"Retrieve the Allied Demolition Truck and drive it on the Service Depot.");
	}
	else if (number == 102)
	{
		Send_Message(255,255,255,"Retrieve the Allied Demolition Truck and drive it on the Service Depot.");
	}
	else if (number == 103)
	{
		Send_Message(255,255,255,"Retrieve the Allied Demolition Truck and drive it on the Service Depot.");
	}
	else if (number == 133)
	{
		Console_Input("amsg Mission Accomplished!");
		Console_Input("win 0");
	}
	else if (number == 134)
	{
		if (dodemomsg)
		{
			Send_Message(255,255,255,"Retrieve the Allied Demolition Truck and drive it on the Service Depot.");
			Commands->Start_Timer(obj,this,90.0f,134);
		}
	}
	else if (number == 135)
	{
		if (docymsg)
		{
			Commands->Start_Timer(obj,this,90.0f,135);
			Send_Message(255,255,255,"Destroy the Allied Construction Yard before it is too late!");
		}
	}
}
void z_Endgame_MissionTimer::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 133)
	{
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,300.0f,133);//timer ends mission complete
		}
	}
	else if (message == 134)
	{
		if (param == 1)
		{
			dodemomsg = false;
		}
	}
	else if (message == 135)
	{
		if (param == 1)
		{
			docymsg = true;
			Commands->Start_Timer(obj,this,90.0f,135);
		}
		else if (param == 2)
		{
			docymsg = false;
		}
	}
}

void z_Endgame_DemoZone::Entered(GameObject *obj, GameObject *enter)
{
	if (strcmp(Commands->Get_Preset_Name(enter),"CnC_Allied_Demolition_Truck") == 0)
	{
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_VEHICLE_FACTORY) && Commands->Get_Health(Find_Building_By_Type(1,BuildingConstants::TYPE_VEHICLE_FACTORY)) >= 1.0f)//only enable the extra tanks if wf is present and alive
		{
			Commands->Enable_Spawner(100528,1);
			Commands->Enable_Spawner(100529,1);
			Commands->Enable_Spawner(100530,1);
			Commands->Enable_Spawner(100531,1);
			Commands->Enable_Spawner(100532,1);
		}
		Console_Input("amsg Mission Objective Met The Allied Demolition Truck was captured.");
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100525),2000,1,0);//demotruck objective delete
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100526),134,1,0);//demotruck saved, disable objmsg
		Commands->Send_Custom_Event(obj,Commands->Find_Object(150667),1000,1,0);//acy objective enable
		Destroy_Script();
	}
}
void z_Endgame_DemoZone::Timer_Expired(GameObject *obj, int number)
{
	//
}

void z_Endgame_DemoCounter::Created(GameObject *obj)
{
	count = 0;
	Commands->Start_Timer(obj,this,30.0f,100);
}
void z_Endgame_DemoCounter::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			count++;
			if (count == 5)
			{
				char end[250];
				sprintf(end,"amsg Mission Failed! The Allied Demo Truck has died %d times.",count);
				Console_Input(end);
				Console_Input("win 1");
			}
			Vector3 pos;	pos.X = 193.181f;	pos.Y = 107.073f;	pos.Z = 1.771f;
			GameObject *truck = Commands->Create_Object("CnC_Allied_Demolition_Truck",pos);
			Commands->Set_Facing(truck,-83.288f);
			Attach_Script_Once(truck,"z_Death_Send_Custom","100525,1000,1");
		}
	}
	else if (message == 2000)
	{
		if (param == 1)
		{
			Destroy_Script();
		}
	}
}
void z_Endgame_DemoCounter::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		char deads[250];
		sprintf(deads,"msg The Allied Demolition Truck has died %d/5 times.",count);
		Console_Input(deads);
		Commands->Start_Timer(obj,this,180.0f,100);
	}
}

void z_Endgame_Demotruck::Killed(GameObject *obj, GameObject *shooter)
{
	Console_Input("amsg Mission Failed! The Allied Demolition Truck has been destroyed.");
	Console_Input("win 1");
}

void z_Endgame_ACY::Created(GameObject *obj)
{
	takedmg = false;
}
void z_Endgame_ACY::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)//enable objective
	{
		if (param == 1)
		{
			takedmg = true;
			Commands->Start_Timer(obj,this,1200.0f,100);
			Commands->Start_Timer(obj,this,1.0f,101);
			Send_Message(255,255,255,"Destroy the Allied Construction Yard within 20 minutes!");
		}
	}
	else if (message == 2000)
	{
		if (param == 1)
		{
			Destroy_Script();
		}
	}
}
void z_Endgame_ACY::Killed(GameObject *obj, GameObject *shooter)
{
	Console_Input("amsg This mission ends in 5 minutes.");
	Console_Input("amsg Objective Completed. The Construction Yard was destroyed.");
	Commands->Send_Custom_Event(obj,Commands->Find_Object(100526),133,1,0);//acy destroyed, 5min timer start to endgame
	Commands->Send_Custom_Event(obj,Commands->Find_Object(100526),135,2,0);//acy destroyed, disable msg
	Destroy_Script();
}
void z_Endgame_ACY::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Console_Input("amsg Mission Failed. The Construction Yard was not destroyed within the timelimit.");
		Console_Input("win 1");
	}
	else if (number == 101)
	{
		Commands->Start_Timer(obj,this,90.0f,101);
		Send_Message(255,255,255,"Destroy the Allied Construction Yard within 20 minutes!");
	}
}
void z_Endgame_ACY::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (takedmg)
	{
		//allow it
	}
	else
	{
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	}
}

void z_Endgame_ACYRepair::Created(GameObject *obj)
{
	count = 0;
}
void z_Endgame_ACYRepair::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 5000)
	{
		if (param == 1)
		{
			count++;
			if (count == 5)
			{
				Send_Message(255,255,255,"The Allied Construction Yard is no longer being internally repaired by Engineers.");
				Send_Message(255,255,255,"The Allied Construction Yard is no longer being internally repaired by Engineers.");
				Destroy_Script();
			}
		}
	}
}
void z_Endgame_ACYRepair::Damaged(GameObject *obj,	GameObject *damager, float damage)
{
	if (Commands->Get_Health(obj) <= 50.0f)
	{
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	}
}

	
ScriptRegistrant<z_Endgame_ACYRepair> z_Endgame_ACYRepair_Registrant("z_Endgame_ACYRepair","");
ScriptRegistrant<z_Endgame_ACY> z_Endgame_ACY_Registrant("z_Endgame_ACY","");
ScriptRegistrant<z_Endgame_Demotruck> z_Endgame_Demotruck_Registrant("z_Endgame_Demotruck","");
ScriptRegistrant<z_Endgame_MissionTimer> z_Endgame_MissionTimer_Registrant("z_Endgame_MissionTimer","");
ScriptRegistrant<z_Endgame_AAPP> z_Endgame_AAPP_Registrant("z_Endgame_AAPP","");
ScriptRegistrant<z_Endgame_DemoZone> z_Endgame_DemoZone_Registrant("z_Endgame_DemoZone","");
ScriptRegistrant<z_Endgame_DemoCounter> z_Endgame_DemoCounter_Registrant("z_Endgame_DemoCounter","");