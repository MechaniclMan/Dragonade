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
#include "z_am02.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"

void z_aM02_TruckZone::Created(GameObject *obj)
{
	saves = 0;
	count = 0;
	deaths = 0;
	trigger = true;
	Commands->Start_Timer(obj,this,10.0f,200);
	Commands->Start_Timer(obj,this,30.0f,300);//initialize mission timer
	Commands->Start_Timer(obj,this,110.0f,200);
}
void z_aM02_TruckZone::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			deaths++;
			if (deaths == 6)//allow 6 trucks to die before mission fails...
			{
				Commands->Create_2D_WAV_Sound("missionfailed.wav");
				Send_Message(19,169,255,"Mission Failed!");
				Send_Message(19,169,255,"Mission Failed!");
				Send_Message(19,169,255,"Mission Failed!");
				Commands->Start_Timer(obj,this,2.5f,100);
			}
		}
	}
	else if (message == 1001)
	{
		if (param == 1)
		{
			count++;
			if (count == 3)
			{
				if (trigger)
				{
					Commands->Start_Timer(obj,this,120.0f,302);//new trucks
					Send_Message(19,169,255,"New convoy will arrive in two minutes.");
					count = 0;
					trigger = false;
				}
			}
		}
	}
}
void z_aM02_TruckZone::Entered(GameObject *obj, GameObject *enter)
{
	if (strcmp(Commands->Get_Preset_Name(enter),"The_Cargo_Truck") == 0)
	{
		saves++;
		char lolk[250];
		sprintf(lolk,"A truck was saved! %d/3.",saves);
		Send_Message(19,169,255,lolk);
		Set_Skin(enter,"Blamo");
		Commands->Set_Shield_Type(enter,"Blamo");
		Commands->Create_2D_WAV_Sound("objbeep.wav");
		Commands->Send_Custom_Event(obj,obj,1001,1,0);
		if (saves == 3)//completed  
		{
			Commands->Create_2D_WAV_Sound("missionaccomplished.wav");
			Send_Message(19,169,255,"Mission Accomplished!");
			Send_Message(19,169,255,"Mission Accomplished!");
			Commands->Start_Timer(obj,this,30.0f,101);
		}
	}
}
void z_aM02_TruckZone::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Create_2D_WAV_Sound("airraid.wav");
		Console_Input("amsg Mission Failed. Too many Trucks were killed.");
		Console_Input("win 0");
	}
	else if (number == 101)
	{
		Console_Input("amsg Mission Accomplished! Three Trucks were saved.");
		Console_Input("win 1");
	}
	else if (number == 200)
	{
		char lolk[250];
		sprintf(lolk,"%d/3 trucks were escorted to safety. %d/6 trucks were killed.",saves,deaths);
		Send_Message(19,169,255,lolk);
		Commands->Start_Timer(obj,this,100.0f,200);
	}
	else if (number == 300)
	{
		char timer[250];
		Commands->Create_2D_WAV_Sound("missiontimer.wav");
		sprintf(timer,"Mission Timer initialized.");
		Send_Message(19,169,255,timer);
		Commands->Start_Timer(obj,this,4.0f,301);
	}
	else if (number == 301)
	{
		/*Commands->Start_Timer(obj,this,1801.0f,302);//start truck deployment
		Commands->Start_Timer(obj,this,1741.0f,303);//start 1min
		Commands->Start_Timer(obj,this,1501.0f,304);//start 5min
		Commands->Start_Timer(obj,this,1201.0f,305);//start 10min
		Commands->Start_Timer(obj,this,601.0f,306);//start 20min
		Commands->Start_Timer(obj,this,1.0f,307);//start 30min*/

		//speed up the deployment to test otherwise we gotto wait a real half hour lol :D
		Commands->Start_Timer(obj,this,105.0f,302);//start truck deployment
		Commands->Start_Timer(obj,this,90.0f,303);//start 1min
		Commands->Start_Timer(obj,this,75.0f,304);//start 5min
		Commands->Start_Timer(obj,this,60.0f,305);//start 10min
		Commands->Start_Timer(obj,this,45.0f,306);//start 20min
		Commands->Start_Timer(obj,this,30.0f,307);//start 30min
	}
	else if (number == 302)
	{
		//spawn the trucks
		Commands->Enable_Spawner(100288,true);//truck 1
		Commands->Enable_Spawner(100289,true);//truck 2
		Commands->Enable_Spawner(100290,true);//truck 3

		//disable spawners after 1 second:
		Commands->Start_Timer(obj,this,1.0f,308);

		//enable additional rifle soldiers to compensate no adv infantry
		Commands->Enable_Spawner(100662,true);

	}
	else if (number == 303)
	{
		Commands->Create_2D_WAV_Sound("1mr.wav");//1min remaining
		Send_Message(19,169,255,"Warning, one minute remaining.");
	}
	else if (number == 304)
	{
		Commands->Create_2D_WAV_Sound("5mr.wav");//5min remaining
		Send_Message(19,169,255,"Warning, five minutes remaining.");
	}
	else if (number == 305)
	{
		Commands->Create_2D_WAV_Sound("10mr.wav");//10min remaining
		Send_Message(19,169,255,"Ten minutes remaining.");
	}
	else if (number == 306)
	{
		Commands->Create_2D_WAV_Sound("20mr.wav");//20min remaining
		Send_Message(19,169,255,"Twenty minutes remaining");
	}
	else if (number == 307)
	{
		Commands->Create_2D_WAV_Sound("30mr.wav");//30min remaining
		Send_Message(19,169,255,"Thirty minutes remaining.");
	}
	else if (number == 308)
	{
		//immidetaly disable spawners again...
		Commands->Enable_Spawner(100288,false);//truck 1
		Commands->Enable_Spawner(100289,false);//truck 2
		Commands->Enable_Spawner(100290,false);//truck 3
		trigger = true;
	}
}
void z_aM02_TruckZone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&trigger,5,1);
	Auto_Save_Variable(&count,5,1);
	Auto_Save_Variable(&deaths,5,1);
	Auto_Save_Variable(&saves,5,1);
}

void z_aM02_Truck::Created(GameObject *obj)
{
	Commands->Create_2D_WAV_Sound("a_reinf_north.wav");
	Send_Message(19,169,255,"Allied reinforcements have arrived from the North!");
}
void z_aM02_Truck::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(100287),1000,1,0);//deathcount
	Commands->Send_Custom_Event(obj,Commands->Find_Object(100287),1001,1,0);//count increase for new trucks when 3 deaths are registered
}


ScriptRegistrant<z_aM02_Truck> z_aM02_Truck_Registrant("z_aM02_Truck","");
ScriptRegistrant<z_aM02_TruckZone> z_aM02_TruckZone_Registrant("z_aM02_TruckZone","");