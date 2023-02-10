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
#include "z_ship.h"
#include "BaseControllerClass.h"

void z_Ship_TorpedoRack::Created(GameObject *obj)
{
	sabotaged = false;
}
void z_Ship_TorpedoRack::Poked(GameObject *obj, GameObject *poker)
{
	if (!sabotaged)
	{
		sabotaged = true;
		Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0013i1evag_snd.wav");
		Send_Message(255,255,255,"Rewirering complete. Missilerack Sabotaged.");
		Commands->Send_Custom_Event(obj,Commands->Find_Object(105916),1500,1,0);
	}
}
void z_Ship_TorpedoRack::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&sabotaged,5,1);
}

void z_Ship_TorpedoRackCount::Created(GameObject *obj)
{
	count = 0;
}
void z_Ship_TorpedoRackCount::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1500)
	{
		count++;
		if (count == 4)
		{
			Commands->Start_Timer(obj,this,1.0f,100);
			Commands->Start_Timer(obj,this,4.0f,101);
			Commands->Start_Timer(obj,this,8.0f,102);

		}
	}
}
void z_Ship_TorpedoRackCount::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Create_2D_WAV_Sound("objective_primary_accomplished.wav");
		Send_Message(255,255,255,"Primary Objective Accomplished");
	}
	else if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("objective_primary_new.wav");
		Send_Message(255,255,255,"New primary mission objective:");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0032i1evag_snd.wav");
		Send_Message(255,255,255,"Objective: Locate and destroy vessel defense systems.");
	}
}
void z_Ship_TorpedoRackCount::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&count,5,1);
}

void z_Ship_TorpedoRackHelp::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0014i1evag_snd.wav");
		Send_Message(255,255,255,"Use the Action Key (E) on the Missileracks to sabotage them.");
		Destroy_Script();
	}
}

void z_Ship_Intro::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,10.0f,100);
	Commands->Start_Timer(obj,this,15.0f,101);
	Commands->Start_Timer(obj,this,20.0f,102);
}
void z_Ship_Intro::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,255,255,"Welcome aboard the Ship commanders.");
	}
	else if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0001i1evag_snd.wav");
		Send_Message(255,255,255,"Main Mission Objective: Locate and rescue GDI prisoners.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,255,255,"Objective: Find and sabotage the four torpedo racks.");
	}
}

void z_Ship_BigSam::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(105961),1000,1,0);
}

void z_Ship_BigSamDead_Custom::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{ 
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,1.0f,101);
		}
	}
}
void z_Ship_BigSamDead_Custom::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0034i1evag_snd.wav");
		Send_Message(255,255,255,"Vessel defense systems.. Destroyed..");
		Commands->Destroy_Object(Commands->Find_Object(105917));
		Commands->Destroy_Object(Commands->Find_Object(105960));
	}
}

void z_Ship_BigSamAnnouncezone::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		Send_Message(255,255,255,"Vessel Air Defense systems detected in this room. Locate and destroy.");
		Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0036i1evag_snd.wav");
		Destroy_Script();
	}
}

void z_Ship_EnginesAnnounceZone::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Send_Message(255,255,255,"Engine critical systems detected. Locate and destroy.");
			Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0038i1evag_snd.wav");
			Destroy_Script();
		}
	}
}

void z_Ship_EngineCount::Created(GameObject *obj)
{
	count = 4;
}
void z_Ship_EngineCount::Custom(GameObject *obj, int message, int param, GameObject *sender)
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
				Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0040i1evag_snd.wav");
				Send_Message(255,255,255,"Engine Critical System Destroyed.");
				Commands->Start_Timer(obj,this,5.0f,100);
			}
		}
	}
}
void z_Ship_EngineCount::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Send_Message(255,255,255,"Engines Offline!");
		Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0044i1evag_snd.wav");
		Set_Skin(Commands->Find_Object(150001),"SkinFlesh");
		Commands->Apply_Damage(Commands->Find_Object(150001),2500.0f,"blamo",obj);//killkillkill the engine :)
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100424),0,530,0);
	}
}
void z_Ship_EngineCount::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&count,5,1);
}

void z_Ship_EngineCriticalSystem::Created(GameObject *obj)
{
	allowpoke = true;
	pokerguy = 0;
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
}
void z_Ship_EngineCriticalSystem::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (strcmp(Commands->Get_Preset_Name(poker),"CnC_GDI_Engineer_2SF") == 0)
		{
			Commands->Start_Timer(obj,this,1.0f,100);
			Commands->Enable_HUD_Pokable_Indicator(obj, false);
			allowpoke = false;
			pokerguy = poker;
		}
		else
		{
			Send_Message(255,255,255,"Only a Hotwire can hack this Engine Critical System.");
			allowpoke = false;
			Commands->Start_Timer(obj,this,10.0f,105);//allowpoke true
			Commands->Enable_HUD_Pokable_Indicator(obj, false);
			Commands->Create_2D_WAV_Sound("accessdenied.wav");
		}
	}
}
void z_Ship_EngineCriticalSystem::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		if (Commands->Get_Health(pokerguy) >= 1.0f)
		{
			Send_Message(255,255,255,"Accessing...");
			Commands->Create_2D_WAV_Sound("accessing.wav");
			Commands->Start_Timer(obj,this,3.0f,101);
		}
		else
		{
			allowpoke = true;
			Commands->Create_2D_WAV_Sound("accessdenied.wav");
			Commands->Enable_HUD_Pokable_Indicator(obj, true);
		}
	}
	else if (number == 101)
	{
		if (Commands->Get_Health(pokerguy) >= 1.0f)
		{
			Send_Message(255,255,255,"Please stand by...");
			Commands->Create_2D_WAV_Sound("plsstandby.wav");
			Commands->Start_Timer(obj,this,3.0f,102);
		}
		else
		{
			allowpoke = true;
			Commands->Create_2D_WAV_Sound("accessdenied.wav");
			Commands->Enable_HUD_Pokable_Indicator(obj, true);
		}
	}
	else if (number == 102)
	{
		if (Commands->Get_Health(pokerguy) >= 1.0f)
		{
			Send_Message(255,255,255,"Uplink estabalished...");
			Commands->Create_2D_WAV_Sound("uplinkest.wav");
			Commands->Start_Timer(obj,this,3.0f,103);
		}
		else
		{
			allowpoke = true;
			Commands->Create_2D_WAV_Sound("accessdenied.wav");
			Commands->Enable_HUD_Pokable_Indicator(obj, true);
		}
	}
	else if (number == 103)
	{
		if (Commands->Get_Health(pokerguy) >= 1.0f)
		{
			Commands->Create_2D_WAV_Sound("updatecomplete.wav");
			Send_Message(255,255,255,"Update complete...");
			Commands->Send_Custom_Event(obj,Commands->Find_Object(105979),1000,1,3);//tell control to play sound and/or kill the engine
			Destroy_Script();
		}
		else
		{
			allowpoke = true;
			Commands->Enable_HUD_Pokable_Indicator(obj, true);
		}
	}
	else if (number == 105)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}
void z_Ship_EngineCriticalSystem::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowpoke,5,1);
	Auto_Save_Variable(&pokerguy,5,1);
}

void z_Ship_LocateGreenKeyZone::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		Commands->Enable_Spawner(106047,true);
		Send_Message(255,255,255,"Objective: Locate the Nod Officer holding the Green Key Card.");
		Destroy_Script();
	}
}

void z_Ship_ObtainPrisonKeyAnnounceZone::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0003i1evag_snd");
		Send_Message(255,255,255,"Objective: Obtain the prison security card.");
		Destroy_Script();
	}
}

void z_Ship_PrisonersEnableSubmarine::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Commands->Enable_Spawner(106219, true);//a
			Commands->Enable_Spawner(106220, true);//b
			Commands->Enable_Spawner(106221, true);//c
			Commands->Enable_Spawner(106222, true);//d
			Commands->Enable_Spawner(106223, true);//e
		}
	}
}

void z_Ship_TorpedoRacks_EnableRedKey::Created(GameObject *obj)
{
	count = 0;
}
void z_Ship_TorpedoRacks_EnableRedKey::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			count++;
			if (count == 2)
			{
				Commands->Enable_Spawner(106142, true);
				Destroy_Script();
			}
		}
	}
}
void z_Ship_TorpedoRacks_EnableRedKey::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&count,5,1);
}

void z_Ship_TorpedoRacks_AnnounceZone::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		Send_Message(255,255,255,"Objective: Sabotage the 2 Nod Torpedo Racks.");
		Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0015i1evag_snd.wav");
		Destroy_Script();
	}
}

void z_Ship_TorpedoRack2::Poked(GameObject *obj, GameObject *poker)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(106141),1000,1,0);
	Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0017i1evag_snd.wav");
	Send_Message(255,255,255,"Rewirering complete. Torpedo Rack Sabotaged.");
	Destroy_Script();
}

void z_Ship_Prisoner_Waypath_Poke::Created(GameObject *obj)
{
	allowpoke = true;
	followid = 0;
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
}
void z_Ship_Prisoner_Waypath_Poke::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		int waypathid;
		float speed;
		waypathid = Get_Int_Parameter("Waypathid");
		speed = Get_Float_Parameter("Speed");
		ActionParamsStruct params;
		params.MoveArrivedDistance = 100;
		params.Set_Basic(this,100,777);
		params.Set_Movement(0,speed,10);
		params.WaypathID = waypathid;
		params.WaypathSplined = true;
		params.AttackActive = false;
		params.AttackCheckBlocked = false;
		Commands->Action_Goto(obj,params);

		/*followid = Commands->Get_ID(poker);
		Commands->Innate_Disable(obj);
		Commands->Set_Innate_Is_Stationary(obj, false);
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		allowpoke = false;
		GameObject *GotoObject = Commands->Find_Object(followid);
		ActionParamsStruct params;
		params.Set_Basic(this,100,100);
		params.Set_Movement(GotoObject,1.0f,50.0f,false);
		Commands->Action_Goto(obj,params);
		Commands->Start_Timer(obj,this,0.5f,100);*/
	}
}
void z_Ship_Prisoner_Waypath_Poke::Timer_Expired(GameObject *obj, int number)
{
	/*if (number == 100)
	{
		if (Commands->Find_Object(followid))
		{
			GameObject *GotoObject = Commands->Find_Object(followid);
			ActionParamsStruct params;
			params.Set_Basic(this,100,100);
			params.Set_Movement(GotoObject,1.0f,50.0f,false);
			Commands->Action_Goto(obj,params);
			Commands->Start_Timer(obj,this,0.5f,100);
		}
		else
		{
			followid = 0;
			Commands->Enable_HUD_Pokable_Indicator(obj, true);
			allowpoke = true;
		}
	}*/
}
void z_Ship_Prisoner_Waypath_Poke::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Get_Player_Type(damager) == 0)
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(106126),1002,1,0);
	}
}
void z_Ship_Prisoner_Waypath_Poke::Killed(GameObject *obj, GameObject *shooter)
{
	/*Console_Input("amsg Mission Failed! A Prisoner was terminated.");
	Console_Input("win 0");*/
}
void z_Ship_Prisoner_Waypath_Poke::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowpoke,5,1);
	Auto_Save_Variable(&followid,5,1);
}

void z_Ship_Prisoner_Enter_Sub_Zone::Created(GameObject *obj)
{
	res1 = false;
	res2 = false;
	res3 = false;
	res4 = false;
	res5 = false;
	Commands->Start_Timer(obj,this,2.0f,100);
}
void z_Ship_Prisoner_Enter_Sub_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&res1,5,1);
	Auto_Save_Variable(&res2,5,1);
	Auto_Save_Variable(&res3,5,1);
	Auto_Save_Variable(&res4,5,1);
	Auto_Save_Variable(&res5,5,1);
}
void z_Ship_Prisoner_Enter_Sub_Zone::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Vector3 mypos = Commands->Get_Position(obj);
		Vector3 target1pos = Commands->Get_Position(Find_Object_By_Preset(2,"Ship_Prisoner_a"));
		Vector3 target2pos = Commands->Get_Position(Find_Object_By_Preset(2,"Ship_Prisoner_b"));
		Vector3 target3pos = Commands->Get_Position(Find_Object_By_Preset(2,"Ship_Prisoner_c"));
		Vector3 target4pos = Commands->Get_Position(Find_Object_By_Preset(2,"Ship_Prisoner_d"));
		Vector3 target5pos = Commands->Get_Position(Find_Object_By_Preset(2,"Ship_Prisoner_e"));
		float distance1 = Commands->Get_Distance(target1pos,mypos);
		float distance2 = Commands->Get_Distance(target2pos,mypos);
		float distance3 = Commands->Get_Distance(target3pos,mypos);
		float distance4 = Commands->Get_Distance(target4pos,mypos);
		float distance5 = Commands->Get_Distance(target5pos,mypos);
		if (!res1)
		{
			if (distance1 < 5.0f)
			{
				res1 = true;
				Commands->Send_Custom_Event(obj,Commands->Find_Object(106126),1001,1,0);//rescued
				Commands->Destroy_Object(Find_Object_By_Preset(2,"Ship_Prisoner_a"));
			}
		}
		if (!res2)
		{
			if (distance2 < 5.0f)
			{
				res2 = true;
				Commands->Send_Custom_Event(obj,Commands->Find_Object(106126),1001,1,0);//rescued
				Commands->Destroy_Object(Find_Object_By_Preset(2,"Ship_Prisoner_b"));
			}
		}
		if (!res3)
		{
			if (distance3 < 5.0f)
			{
				res3 = true;
				Commands->Send_Custom_Event(obj,Commands->Find_Object(106126),1001,1,0);//rescued
				Commands->Destroy_Object(Find_Object_By_Preset(2,"Ship_Prisoner_c"));
			}
		}
		if (!res4)
		{
			if (distance4 < 5.0f)
			{
				res4 = true;
				Commands->Send_Custom_Event(obj,Commands->Find_Object(106126),1001,1,0);//rescued
				Commands->Destroy_Object(Find_Object_By_Preset(2,"Ship_Prisoner_d"));
			}
		}
		if (!res5)
		{
			if (distance5 < 5.0f)
			{
				res5 = true;
				Commands->Send_Custom_Event(obj,Commands->Find_Object(106126),1001,1,0);//rescued
				Commands->Destroy_Object(Find_Object_By_Preset(2,"Ship_Prisoner_e"));
			}
		}
		Commands->Start_Timer(obj,this,1.0f,100);
	}
}
void z_Ship_Prisoner_Enter_Sub_Zone::Entered(GameObject *obj, GameObject *enter)
{
	//not used anymore...fucking bots dont entering a zone :(
}

void z_Ship_Prisoner_Control::Created(GameObject *obj)
{
	playwarning = true;
	deaths = 0;
	reqres = 5;
	saves = 0;
}
void z_Ship_Prisoner_Control::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			deaths++;
			reqres--;
			Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0030i1evag_snd.wav");
			Send_Message(255,255,255,"Prisoner Terminated.");
			if (deaths == 5)
			{
				Console_Input("amsg Mission Failed. All Prisoners were terminated.");
				Console_Input("win 0");
			}
		}
	}
	else if (message == 1001)
	{
		if (param == 1)
		{
			saves++;
			if (saves == reqres)
			{
				Send_Message(255,255,255,"Objective Complete");
				Send_Message(255,255,255,"The prisoners were rescued.");
				Commands->Create_2D_WAV_Sound("objective_primary_accomplished.wav");
				Commands->Start_Timer(obj,this,5.0f,100);
			}
		}
	}
	else if (message == 1002)
	{
		if (param == 1)
		{
			if (playwarning)
			{
				playwarning = false;
				Commands->Create_2D_WAV_Sound("m04dsgn_dsgn0025i1evag_snd.wav");
				Send_Message(255,255,255,"Warning: Prisoners under attack in submarine bay. Protect them at all costs.");
				Commands->Start_Timer(obj,this,10.0f,102);
			}
		}
	}
}
void z_Ship_Prisoner_Control::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Send_Message(255,255,255,"New Primary Mission Objective!");
		Commands->Create_2D_WAV_Sound("objective_primary_new.wav");
		Commands->Start_Timer(obj,this,3.5f,103);
	}
	else if (number == 101)
	{
		Console_Input("amsg Mission Failed! The prisoners were terminated.");
		Console_Input("win 0");
	}
	else if (number == 102)
	{
		playwarning = true;
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("locateshipcpt.wav");
		Send_Message(255,255,255,"Locate the Ship Captain and ion the Pedestal");
	}
}
void z_Ship_Prisoner_Control::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&deaths,5,1);
	Auto_Save_Variable(&reqres,5,1);
	Auto_Save_Variable(&saves,5,1);
	Auto_Save_Variable(&playwarning,5,1);
}

void z_Ship_End::Killed(GameObject *obj, GameObject *shooter)
{
	if (Commands->Is_A_Star(shooter))
	{
		char killmsg[250];
		sprintf(killmsg,"amsg %s Ion-ed the Pedestal ending the game. Mission Accomplished",Get_Player_Name(shooter));
		Console_Input(killmsg);
		Commands->Start_Timer(obj,this,5.0f,100);
	}
}


ScriptRegistrant<z_Ship_End> z_Ship_End_Registrant("z_Ship_End","");
ScriptRegistrant<z_Ship_Prisoner_Enter_Sub_Zone> z_Ship_Prisoner_Enter_Sub_Zone_Registrant("z_Ship_Prisoner_Enter_Sub_Zone","");
ScriptRegistrant<z_Ship_Prisoner_Waypath_Poke> z_Ship_Prisoner_Waypath_Poke_Registrant("z_Ship_Prisoner_Waypath_Poke","Waypathid=1:int,Speed=1:float");
ScriptRegistrant<z_Ship_TorpedoRacks_AnnounceZone> z_Ship_TorpedoRacks_AnnounceZone_Registrant("z_Ship_TorpedoRacks_AnnounceZone","");
ScriptRegistrant<z_Ship_TorpedoRack2> z_Ship_TorpedoRack2_Registrant("z_Ship_TorpedoRack2","");
ScriptRegistrant<z_Ship_TorpedoRacks_EnableRedKey> z_Ship_TorpedoRacks_EnableRedKey_Registrant("z_Ship_TorpedoRacks_EnableRedKey","");
ScriptRegistrant<z_Ship_Prisoner_Control> z_Ship_Prisoner_Control_Registrant("z_Ship_Prisoner_Control","");
ScriptRegistrant<z_Ship_PrisonersEnableSubmarine> z_Ship_PrisonersEnableSubmarine_Registrant("z_Ship_PrisonersEnableSubmarine","");
ScriptRegistrant<z_Ship_ObtainPrisonKeyAnnounceZone> z_Ship_ObtainPrisonKeyAnnounceZone_Registrant("z_Ship_ObtainPrisonKeyAnnounceZone","");
ScriptRegistrant<z_Ship_LocateGreenKeyZone> z_Ship_LocateGreenKeyZone_Registrant("z_Ship_LocateGreenKeyZone","");
ScriptRegistrant<z_Ship_EngineCriticalSystem> z_Ship_EngineCriticalSystem_Registrant("z_Ship_EngineCriticalSystem","");
ScriptRegistrant<z_Ship_EngineCount> z_Ship_EngineCount_Registrant("z_Ship_EngineCount","");
ScriptRegistrant<z_Ship_EnginesAnnounceZone> z_Ship_EnginesAnnounceZone_Registrant("z_Ship_EnginesAnnounceZone","");
ScriptRegistrant<z_Ship_BigSamAnnouncezone> z_Ship_BigSamAnnouncezone_Registrant("z_Ship_BigSamAnnouncezone","");
ScriptRegistrant<z_Ship_BigSamDead_Custom> z_Ship_BigSamDead_Custom_Registrant("z_Ship_BigSamDead_Custom","");
ScriptRegistrant<z_Ship_BigSam> z_Ship_BigSam_Registrant("z_Ship_BigSam","");
ScriptRegistrant<z_Ship_TorpedoRackHelp> z_Ship_TorpedoRackHelp_Registrant("z_Ship_TorpedoRackHelp","");
ScriptRegistrant<z_Ship_TorpedoRackCount> z_Ship_TorpedoRackCount_Registrant("z_Ship_TorpedoRackCount","");
ScriptRegistrant<z_Ship_TorpedoRack> z_Ship_TorpedoRack_Registrant("z_Ship_TorpedoRack","");