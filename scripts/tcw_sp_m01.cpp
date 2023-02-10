/*	Renegade Scripts.dll
	Scripts from - http://www.tiberiumcrystalwar.com
	Copyright 2011-2012 MPF

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it 
	under the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code 
	with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "General.h"
#include "engine.h"
#include "scripts.h"
#include "ScriptZoneGameObj.h"
#include "VehicleGameObj.h"
#include "tcw_sp_m01.h"

void tcw_m01_objective_controller::Created(GameObject *obj)
{
	Commands->Enable_Radar(true);
	spike_captured = false;
	Vector3 spike_position = Commands->Get_Position(Commands->Find_Object(100017));
	Commands->Set_Objective_HUD_Info_Position(1000,90,"POG_M04_1_01.tga",12778,spike_position );
	Commands->Add_Objective(1000,OBJECTIVE_TYPE_PRIMARY,OBJECTIVE_STATUS_PENDING,12776,NULL,12777);
	Commands->Add_Radar_Marker(1001,Commands->Get_Position(Commands->Find_Object(100017)),RADAR_BLIP_SHAPE_OBJECTIVE,RADAR_BLIP_COLOR_PRIMARY_OBJECTIVE);
}
void tcw_m01_objective_controller::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		spike_captured = true;
		Commands->Set_Objective_Status(1000,OBJECTIVE_STATUS_ACCOMPLISHED);
		Commands->Clear_Radar_Marker(1001);
		Commands->Start_Timer(obj,this,5.0f,1000);//complete mission
	}
}
void tcw_m01_objective_controller::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1000)
	{
		Commands->Mission_Complete(true);
	}
}
void tcw_m01_objective_controller::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&spike_captured,sizeof(spike_captured),1);
}

void tcw_m01_spike_zone::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		Commands->Create_2D_WAV_Sound("building_capture.wav");
		Attach_Script_Once(enter,"tcw_m01_star_capturing_spike","");
		Send_Message(255,255,255,"You are now capturing this Tiberium Spike.");
	}
}
void tcw_m01_spike_zone::Exited(GameObject *obj, GameObject *exit)
{
	Remove_Script(exit,"tcw_m01_star_capturing_spike");
	Send_Message(255,255,255,"You have stopped capturing this Tiberium Spike.");
	Commands->Create_2D_WAV_Sound("incmsg.wav");
}

void tcw_m01_star_capturing_spike::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,7.5f,1000);
}
void tcw_m01_star_capturing_spike::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1000)
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_m01_spike"),1000,0,0);//enable animation on spike
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_m01_objective_controller"),1000,0,0);//set objective accomplished
		Commands->Create_2D_WAV_Sound("gdi_building_captured.wav");
		Send_Message(255,255,255,"Building Captured");
		Commands->Destroy_Object(Find_Object_With_Script("tcw_m01_spike_zone"));
		Destroy_Script();
	}
}

void tcw_m01_spike::Created(GameObject *obj)
{
	Commands->Set_Animation(obj,"V_NEU_SPIKE.V_NEU_SPIKE",true,0,0,0,0);
}
void tcw_m01_spike::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		Commands->Set_Animation(obj,"V_NEU_SPIKE.V_NEU_SPIKE",true,0,1,-1,0);
		Commands->Set_Player_Type(obj,1);
	}
}

ScriptRegistrant<tcw_m01_spike> tcw_m01_spike_Registrant("tcw_m01_spike", "");
ScriptRegistrant<tcw_m01_star_capturing_spike> tcw_m01_star_capturing_spike_Registrant("tcw_m01_star_capturing_spike", "");
ScriptRegistrant<tcw_m01_spike_zone> tcw_m01_spike_zone_Registrant("tcw_m01_spike_zone", "");
ScriptRegistrant<tcw_m01_objective_controller> tcw_m01_objective_controller_Registrant("tcw_m01_objective_controller", "");