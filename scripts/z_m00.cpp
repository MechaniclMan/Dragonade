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
#include "z_m00.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"

void z_m00_missionstart::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,10.0f,101);
	Commands->Start_Timer(obj,this,14.0f,102);
	Commands->Start_Timer(obj,this,18.0f,103);
	Commands->Start_Timer(obj,this,22.0f,104);
	Commands->Start_Timer(obj,this,26.0f,105);
	Commands->Start_Timer(obj,this,30.0f,106);
}
void z_m00_missionstart::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,0,0,"Welcome back commanders.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,0,0,"We have started an assault on a small GDI Compound.");
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,0,0,"Take out the SamSites for air support.");
	}
	else if (number == 104)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,0,0,"Destroy the GDI base and beacon the pedestal in the barracks to finish them off.");
	}
	else if (number == 105)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,0,0,"Good luck commanders!");
	}
	else if (number == 106)
	{
		Commands->Create_2D_WAV_Sound("new2destroysamsites.wav");
		Send_Message(255,0,0,"New Secondary Mission Objective");
		Send_Message(255,0,0,"Destroy Enemy Sam Sites. Updating Radar Flags.");
	}
}


void z_m00_killsams::Created(GameObject *obj)
{
	sams = 5;
}
void z_m00_killsams::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		if (param == 1)
		{
			sams--;
			if (sams == 3)
			{
				Commands->Create_2D_WAV_Sound("sams3remain.wav");
				Send_Message(255,0,0,"Three Sam Sites remain.");
			}
			else if (sams == 2)
			{
				Commands->Create_2D_WAV_Sound("sams2remain.wav");
				Send_Message(255,0,0,"Two Sam Sites remain.");
			}
			else if (sams == 1)
			{
				Commands->Create_2D_WAV_Sound("sams1remain.wav");
				Send_Message(255,0,0,"One Sam Site remains.");
			}
			else if (sams == 0)
			{
				Commands->Create_2D_WAV_Sound("samsalldead.wav");
				Send_Message(255,0,0,"All Sam Sites have been destroyed.");
			}
		}
	}
}

void z_m00_custommissioncomplete::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,7.0f,100);
		}
	}
}
void z_m00_custommissioncomplete::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Console_Input("amsg Mission Accomplished!");
		Commands->Create_2D_WAV_Sound("m00evag_dsgn0005i1evag_snd.wav");
		Console_Input("win 0");
	}
}

void z_m00_killscientists::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == Get_Int_Parameter("EnableObjectiveMessage"))
	{
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,1.0f,100);
			Commands->Start_Timer(obj,this,5.0f,101);
		}
	}
	else if (message == Get_Int_Parameter("ScientistsDeadMsg"))
	{
		if (param == 1)
		{
			Commands->Create_2D_WAV_Sound("objective3complete.wav");
			Send_Message(255,0,0,"Tertiary mission objective complete.");
			Commands->Start_Timer(obj,this,4.0f,102);//init new objective
			Commands->Start_Timer(obj,this,8.0f,103);//eliminate gdi
		}
	}
}
void z_m00_killscientists::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Create_2D_WAV_Sound("objective3new.wav");
		Send_Message(255,0,0,"New tertiary mission objective.");
	}
	else if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,0,0,"Eliminate GDI Tiberium Scientists.");		
	}

	//scientistsdead new objective
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("nodcommanddirective.wav");
		Send_Message(255,0,0,"Nod Command Directive.");	
	}
	else if (number == 103)
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100070),1000,1,0);
		Commands->Create_2D_WAV_Sound("elimgdipresregion.wav");
		Send_Message(255,0,0,"Eliminate GDI presence in the region.");	
	}
}



ScriptRegistrant<z_m00_killscientists> z_m00_killscientists_Registrant("z_m00_killscientists","EnableObjectiveMessage=1:int,ScientistsDeadMsg=1:int");
ScriptRegistrant<z_m00_custommissioncomplete> z_m00_custommissioncomplete_Registrant("z_m00_custommissioncomplete","Message=1:int");
ScriptRegistrant<z_m00_killsams> z_m00_killsams_Registrant("z_m00_killsams","Message=1:int");
ScriptRegistrant<z_m00_missionstart> z_m00_missionstart_Registrant("z_m00_missionstart","");