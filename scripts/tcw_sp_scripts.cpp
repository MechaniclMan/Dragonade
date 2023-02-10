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
#include "engine.h"
#include "scripts.h"
#include "ScriptZoneGameObj.h"
#include "PhysicalGameObj.h"
#include "tcw_sp_scripts.h"

void tcw_gdi_sp_harvester::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&state, 4, 1);
	Auto_Save_Variable(&tiberiumRegion, 60, 2);
	Auto_Save_Variable(&WaypathID, 4, 3);
	Auto_Save_Variable(&WaypathBeginID, 4, 4);
	Auto_Save_Variable(&WaypathEndID, 4, 5);
	Auto_Save_Variable(&harvestTime, 4, 6);
	Auto_Save_Variable(&HarvestIdleID, 4, 7);
	Auto_Save_Variable(&HarvestFullID, 4, 8);
	Auto_Save_Variable(&HarvestID, 4, 9);
}

void tcw_gdi_sp_harvester::Created(GameObject *obj)
{
	if(!obj || !obj->As_VehicleGameObj())
	{
		Destroy_Script();
		return;
	}	
	ScriptZoneGameObj *TiberiumZone = (ScriptZoneGameObj *)Commands->Find_Object(Get_Int_Parameter("TiberiumZone"));
	if(!TiberiumZone)
	{
		Destroy_Script();
		return;
	}
	tiberiumRegion = TiberiumZone->Get_Bounding_Box();
	state = StateGoingToHarvest;
	WaypathID = Get_Int_Parameter("WaypathID");
	WaypathBeginID = Get_Int_Parameter("WaypathBeginID");
	WaypathEndID = Get_Int_Parameter("WaypathEndID");
	harvestTime = 15.0f;
	ActionParamsStruct Params;
	Params.Set_Basic(this, 100, 1);
	Params.Set_Movement(0, 1, 1);
	Params.WaypathID = WaypathID;
	Params.MovePathfind = true;
	Commands->Action_Goto(obj, Params);
	Commands->Enable_Engine(obj, true);
	Commands->Innate_Disable(obj);
	GameObject *Tmp1 = Commands->Create_Object_At_Bone(obj, "TCW_GDI_Harv_Idle_FX", "FX");
	GameObject *Tmp2 = Commands->Create_Object_At_Bone(obj, "TCW_GDI_Harv_Full_FX", "FX");
	Commands->Attach_To_Object_Bone(Tmp1, obj,"FX");
	Commands->Attach_To_Object_Bone(Tmp2, obj, "FX");
	HarvestIdleID = Commands->Get_ID(Tmp1);
	HarvestFullID = Commands->Get_ID(Tmp2);
	HarvestID = 0;
	Commands->Set_Animation(Tmp1, "GDI_HARV_IDLE.GDI_HARV_IDLE",false,0,0,0,false);
	Commands->Set_Animation(Tmp2, "GDI_HARV_FULL.GDI_HARV_FULL",false,0,1,1,false);
}

void tcw_gdi_sp_harvester::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
{
	if(complete_reason != ACTION_COMPLETE_LOW_PRIORITY)
	{
		if(action_id == 1)
		{
			state = StateHarvesting;
			Commands->Enable_Engine(obj, false);
			obj->As_PhysicalGameObj()->Clear_Animation();
			Commands->Start_Timer(obj, this, ((((::rand() & 0xFFF) / 4095.0f) * 4.0f) + 3.0f), 2);
			Commands->Start_Timer(obj, this, harvestTime, 1);
			Commands->Set_Animation(Commands->Find_Object(HarvestIdleID), "GDI_HARV_IDLE.GDI_HARV_IDLE",false,0,1,1,false);
			Harvest();
		}
		else if(action_id == 2)
		{
			ActionParamsStruct Params;
			Params.Set_Basic(this, 100, 3);
			Params.Set_Movement(0, 1, 1);
			Params.WaypathID = WaypathID;
			Params.WaypointStartID = WaypathBeginID + 1;
			Params.WaypointEndID = WaypathBeginID;
			Params.MovePathfind = true;
			Params.MoveBackup = true;
			Commands->Action_Goto(obj, Params);
		}
		else if(action_id == 3)
		{
			state = StateUnloading;	
			obj->As_PhysicalGameObj()->Clear_Animation();
			Commands->Enable_Engine(obj, false);
			Commands->Start_Timer(obj, this, 10, 3);
		}
		else if(action_id == 1000)
		{
			state = StateHarvesting;
			Commands->Enable_Engine(obj, false);
			obj->As_PhysicalGameObj()->Clear_Animation();
			Commands->Start_Timer(obj, this, ((((::rand() & 0xFFF) / 4095.0f) * 4.0f) + 3.0f), 2);
			Commands->Set_Animation(Commands->Find_Object(HarvestIdleID), "GDI_HARV_IDLE.GDI_HARV_IDLE",false,0,1,1,false);
			Harvest();
		}
	}
}

void tcw_gdi_sp_harvester::Timer_Expired(GameObject *obj,int number)
{
	if(number == 1)
	{
		Stop_Harvest();
		state = StateGoingToUnload;
		Commands->Enable_Engine(obj, true);
		obj->As_PhysicalGameObj()->Clear_Animation();
		ActionParamsStruct Params;
		Params.Set_Basic(this, 100, 2);
		Params.Set_Movement(0, 1, 1);
		Params.WaypathID = WaypathID;
		Params.WaypointStartID = WaypathEndID;
		Params.WaypointEndID = WaypathBeginID + 1;
		Params.MovePathfind = true;
		Commands->Action_Goto(obj, Params);
		Commands->Set_Animation(Commands->Find_Object(HarvestIdleID), "GDI_HARV_IDLE.GDI_HARV_IDLE",false,0,0,0,false);
		Commands->Set_Animation(Commands->Find_Object(HarvestFullID), "GDI_HARV_FULL.GDI_HARV_FULL",false,0,0,0,false);
	}
	else if(number == 2)
	{
		if(state == StateHarvesting)
		{
			Stop_Harvest();
			state = StateGoingToHarvest;
			Commands->Enable_Engine(obj, true);
			float xrand = WWMath::Random_Float(0.0f,(tiberiumRegion.Extent.X - 5.0f));
			float yrand = WWMath::Random_Float(0.0f,(tiberiumRegion.Extent.Y - 5.0f));
			Vector3 loc = (tiberiumRegion.Basis * Vector3(xrand,yrand,0.0f)) + tiberiumRegion.Center;
			ActionParamsStruct Params;
			Params.Set_Basic(this, 100, 1000);
			Params.Set_Movement(loc, 1, 1);
			Commands->Action_Goto(obj, Params);
			Commands->Set_Animation(Commands->Find_Object(HarvestIdleID), "GDI_HARV_IDLE.GDI_HARV_IDLE",false,0,0,0,false);
		}
	}
	else if(number == 3)
	{
		state = StateGoingToHarvest;
		ActionParamsStruct Params;
		Params.Set_Basic(this, 100, 1);
		Params.Set_Movement(0, 1, 1);
		Params.WaypathID = WaypathID;
		Params.MovePathfind = true;
		Commands->Action_Goto(obj, Params);
		Commands->Enable_Engine(obj, true);
		Commands->Set_Animation(Commands->Find_Object(HarvestIdleID), "GDI_HARV_IDLE.GDI_HARV_IDLE",false,0,0,0,false);
		Commands->Set_Animation(Commands->Find_Object(HarvestFullID), "GDI_HARV_FULL.GDI_HARV_FULL",false,0,1,1,false);
	}
}

void tcw_gdi_sp_harvester::Detach(GameObject *obj)
{
	ScriptImpClass::Detach(obj);
	Commands->Destroy_Object(Commands->Find_Object(HarvestIdleID));
	Commands->Destroy_Object(Commands->Find_Object(HarvestFullID));
	Commands->Destroy_Object(Commands->Find_Object(HarvestID));
}

void tcw_gdi_sp_harvester::Harvest()
{
	Commands->Destroy_Object(Commands->Find_Object(HarvestID));
	HarvestID = 0;
	GameObject *Tmp = Commands->Create_Object_At_Bone(Owner(), "TCW_GDI_Harv_Harving_FX", "FX");
	Commands->Attach_To_Object_Bone(Tmp, Owner(), "FX");
	HarvestID = Commands->Get_ID(Tmp);
	Commands->Set_Animation(Tmp, "GDI_HARVEST.GDI_HARVEST",true,0,0,-1,false);
}

void tcw_gdi_sp_harvester::Stop_Harvest()
{
	Commands->Destroy_Object(Commands->Find_Object(HarvestID));
	HarvestID = 0;
}

ScriptRegistrant<tcw_gdi_sp_harvester> tcw_gdi_sp_harvester_Registrant("tcw_gdi_sp_harvester", "TiberiumZone=0:int,WaypathID=0:int,WaypathBeginID=0:int,WaypathEndID=0:int");