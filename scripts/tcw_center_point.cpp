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
#include "scripts.h"
#include "engine.h"
#include "BuildingGameObj.h"
#include "tcw_center_point.h"

#define ISREPAIR(Damage) (Damage < 0.0f)
#define ISVALIDPLAYER(Team) (Team == 0 || Team == 1)
#define CUSTOM_EVENT_INITIALIZE_TURRET 873423
#define CUSTOM_EVENT_START_TURRET 978356
#define CUSTOM_EVENT_STOP_TURRET 957832

void TCW_CP_Cruiser_Countdown_Point::Created(GameObject *obj)
{
	Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);
	SpawnPadIDs[0] = Get_Int_Parameter("VehSpawnPadID1");
	SpawnPadIDs[1] = Get_Int_Parameter("VehSpawnPadID2");
	SpawnPadIDs[2] = Get_Int_Parameter("VehSpawnPadID3");
	SpawnPadIDs[3] = Get_Int_Parameter("VehSpawnPadID4");
	for(int Index = 0; Index < 4; Index++)
		Commands->Set_Animation(Commands->Find_Object(SpawnPadIDs[Index]),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
	NodTurretIDs[0] = Get_Int_Parameter("NodTurretID1");
	NodTurretIDs[1] = Get_Int_Parameter("NodTurretID2");
	GDITurretIDs[0] = Get_Int_Parameter("GDITurretID1");
	GDITurretIDs[1] = Get_Int_Parameter("GDITurretID2");
	Commands->Send_Custom_Event(obj, Commands->Find_Object(NodTurretIDs[0]), CUSTOM_EVENT_INITIALIZE_TURRET, 1, 0);
	Commands->Send_Custom_Event(obj, Commands->Find_Object(NodTurretIDs[1]), CUSTOM_EVENT_INITIALIZE_TURRET, 1, 0);
	Commands->Send_Custom_Event(obj, Commands->Find_Object(GDITurretIDs[0]), CUSTOM_EVENT_INITIALIZE_TURRET, 0, 0);
	Commands->Send_Custom_Event(obj, Commands->Find_Object(GDITurretIDs[1]), CUSTOM_EVENT_INITIALIZE_TURRET, 0, 0);
	Vehs[0] = new VehicleReinforcements(Get_Parameter("GDIVehicle1"), Get_Vector3_Parameter("GDIVehicle1Pos"), Get_Float_Parameter("GDIVehicle1Facing"), 1);
	Vehs[1] = new VehicleReinforcements(Get_Parameter("GDIVehicle2"), Get_Vector3_Parameter("GDIVehicle2Pos"), Get_Float_Parameter("GDIVehicle2Facing"), 1);
	Vehs[2] = new VehicleReinforcements(Get_Parameter("NodVehicle1"), Get_Vector3_Parameter("NodVehicle1Pos"), Get_Float_Parameter("NodVehicle1Facing"), 0);
	Vehs[3] = new VehicleReinforcements(Get_Parameter("NodVehicle2"), Get_Vector3_Parameter("NodVehicle2Pos"), Get_Float_Parameter("NodVehicle2Facing"), 0);
	Commands->Set_Player_Type(obj, -2);
	Commands->Set_Health(obj, 2);
	Captured = false;
	CaptureType = -2;
	Health = 2;
}

void TCW_CP_Cruiser_Countdown_Point::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if(Commands->Is_A_Star(damager)) // Only Stars may do something with me.
	{
		if(ISREPAIR(damage)) // Check if it's repair damage (damage < 0.0).
		{
			if(!ISVALIDPLAYER(Commands->Get_Player_Type(damager))) // When not a valid player ignore the repairing.
			{
				Commands->Set_Health(obj, Health);
				return;
			}
			Health = Commands->Get_Health(obj);
			if(!Captured && Commands->Get_Health(obj) == Commands->Get_Max_Health(obj)) // When I'm not captured and I've reached maximum health.
			{
				Health = Commands->Get_Max_Health(obj);
				Captured = true;
				CaptureType = Commands->Get_Player_Type(damager);
				Commands->Set_Player_Type(obj, CaptureType);
				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,0,0,false);
				for(int Index = 0; Index < 4; Index++)
					Commands->Set_Animation(Commands->Find_Object(Index),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false);
				Commands->Start_Timer(obj, this, 60, 1);
				Commands->Give_Money(damager, 500, true);
				if(CaptureType == 0)
				{
					Send_Message(255,255,255, "Nod has captured the Countdown Control Point. One Minute for Cruiser Attacks.");
					Create_2D_WAV_Sound_Team("ControlPointCaptured_Nod.wav", 0);
					Vehs[2] ? Vehs[2]->Create() : 0;
					Vehs[3] ? Vehs[3]->Create() : 0;
				}
				else if(CaptureType == 1)
				{
					Send_Message(255,255,255, "GDI has captured the Countdown Control Point. One Minute for Cruiser Attacks.");
					Create_2D_WAV_Sound_Team("ControlPointCaptured_GDI.wav", 1);
					Vehs[0] ? Vehs[0]->Create() : 0;
					Vehs[1] ? Vehs[1]->Create() : 0;
				}
			}
		}
		else
		{
			if(Captured)
			{
				if(!ISVALIDPLAYER(Commands->Get_Player_Type(damager))) // When not a valid player ignore the damage being done.
				{
					Commands->Set_Health(obj, Health);
					return;
				}
				Health = Commands->Get_Health(obj);
				if(damage >= Commands->Get_Health(obj)) // We are destroyed, point lost.
				{
					Health = 2;
					Captured = false;
					Commands->Set_Health(obj, 2);
					Commands->Set_Player_Type(obj, -2);
					Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);
					for(int Index = 0; Index < 4; Index++)
						Commands->Set_Animation(Commands->Find_Object(SpawnPadIDs[Index]),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
					Commands->Send_Custom_Event(obj, Commands->Find_Object(NodTurretIDs[0]), CUSTOM_EVENT_STOP_TURRET, 0, 0);
					Commands->Send_Custom_Event(obj, Commands->Find_Object(NodTurretIDs[1]), CUSTOM_EVENT_STOP_TURRET, 0, 0);
					Commands->Send_Custom_Event(obj, Commands->Find_Object(GDITurretIDs[0]), CUSTOM_EVENT_STOP_TURRET, 0, 0);
					Commands->Send_Custom_Event(obj, Commands->Find_Object(GDITurretIDs[1]), CUSTOM_EVENT_STOP_TURRET, 0, 0);
					if(CaptureType == 0)
					{
						Send_Message(255,255,255, "Cruiser Attacks on GDI halted. Countdown Point destroyed.");
						Create_2D_WAV_Sound_Team("ControlPointLost_Nod.wav", 0);
						Vehs[2] ? Commands->Apply_Damage(Vehs[2]->Is_Alive_And_Kicking(), 9999, "BlamoKiller", 0) : 0;
						Vehs[3] ? Commands->Apply_Damage(Vehs[3]->Is_Alive_And_Kicking(), 9999, "BlamoKiller", 0) : 0;
					}
					else if(CaptureType == 1)
					{
						Send_Message(255,255,255, "Cruiser Attacks on Nod halted. Countdown Point destroyed.");
						Create_2D_WAV_Sound_Team("ControlPointLost_GDI.wav", 1);
						Vehs[0] ? Commands->Apply_Damage(Vehs[0]->Is_Alive_And_Kicking(), 9999, "BlamoKiller", 0) : 0;
						Vehs[1] ? Commands->Apply_Damage(Vehs[1]->Is_Alive_And_Kicking(), 9999, "BlamoKiller", 0) : 0;
					}
					CaptureType = -2;
				}
			}
			else
				Commands->Set_Health(obj, 2);
		}
	}
}

void TCW_CP_Cruiser_Countdown_Point::Timer_Expired(GameObject *obj, int number)
{
	if(number == 1 && Captured)
	{
		Commands->Create_2D_WAV_Sound("attackalert.mp3");
		if(CaptureType == 0)
		{
			Commands->Send_Custom_Event(obj, Commands->Find_Object(NodTurretIDs[0]), CUSTOM_EVENT_START_TURRET, BuildingConstants::TYPE_REFINERY, 0);
			Commands->Send_Custom_Event(obj, Commands->Find_Object(NodTurretIDs[1]), CUSTOM_EVENT_START_TURRET, BuildingConstants::TYPE_SOLDIER_FACTORY, 0);
		}
		else if(CaptureType == 1)
		{
			Commands->Send_Custom_Event(obj, Commands->Find_Object(GDITurretIDs[0]), CUSTOM_EVENT_START_TURRET, BuildingConstants::TYPE_REFINERY, 0);
			Commands->Send_Custom_Event(obj, Commands->Find_Object(GDITurretIDs[1]), CUSTOM_EVENT_START_TURRET, BuildingConstants::TYPE_SOLDIER_FACTORY, 0);
		}
	}
}

void TCW_CP_Cruiser_Attack::Created(GameObject *obj)
{
	Set_Skin(obj, "Blamo");
	Commands->Set_Shield_Type(obj, "Blamo");
	Commands->Set_Player_Type(obj, -2);
	Commands->Innate_Disable(obj);
}

void TCW_CP_Cruiser_Attack::Action_Complete(GameObject *obj, int action, ActionCompleteReason reason)
{
	if(reason == ACTION_COMPLETE_NORMAL)
		Commands->Action_Reset(obj, 100);
}

void TCW_CP_Cruiser_Attack::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if(type == CUSTOM_EVENT_INITIALIZE_TURRET)
	{
		AttackTeam = param;
		if(AttackTeam == 0)
			Commands->Set_Player_Type(obj, 1);
		else if(AttackTeam == 1)
			Commands->Set_Player_Type(obj, 0);
		else
			Commands->Set_Player_Type(obj, -2);
	}
	else if(type == CUSTOM_EVENT_START_TURRET)
	{
		GameObject *Building = Find_Building_By_Type(AttackTeam, param);
		if(Building)
		{
			ActionParamsStruct Params;
			Params.Set_Basic(this, 100, 1);
			Params.Set_Attack(Commands->Get_Position(Building), 9999, 0, true);
			Params.AttackCheckBlocked = false;
			Params.AttackForceFire = true;
			Commands->Action_Attack(obj, Params);
		}
	}
	else if(type == CUSTOM_EVENT_STOP_TURRET)
	{
		ActionParamsStruct Params;
		Params.Set_Basic(this, 100, 1);
		Params.Set_Attack(0, 9999, 0, true);
		Commands->Action_Attack(obj, Params);
	}
}

ScriptRegistrant<TCW_CP_Cruiser_Attack> tcw_cp_cruiser_attack_Registrant("tcw_cp_cruiser_attack","");
ScriptRegistrant<TCW_CP_Cruiser_Countdown_Point> tcw_cp_cruiser_countdown_point_Registrant("tcw_cp_cruiser_countdown_point","VehSpawnPadID1=0:int,VehSpawnPadID2=0:int,VehSpawnPadID3=0:int,VehSpawnPadID4=0:int,NodTurretID1=0:int,NodTurretID2=0:int,GDITurretID1=0:int,GDITurretID2=0:int,GDIVehicle1:string,GDIVehicle1Pos:vector3,GDIVehicle1Facing=0:float,GDIVehicle2:string,GDIVehicle2Pos:vector3,GDIVehicle2Facing=0:float,NodVehicle1:string,NodVehicle1Pos:vector3,NodVehicle1Facing=0:float,NodVehicle2:string,NodVehicle2Pos:vector3,NodVehicle2Facing=0:float");