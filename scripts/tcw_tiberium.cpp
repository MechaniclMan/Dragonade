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
#include "tcw_general.h"
#include "tcw_tiberium.h"
#include "VehicleGameObj.h"
#include "engine.h"
#include "GameObjManager.h"
#include "BuildingGameObj.h"
#include "tcw_TCWSoldierObj.h"

/////////////////////////////////////////////////////////
//					TCW_TIBERIUM_HARVESTER (PLAYER)
////////////////////////////////////////////////////////

float tcw_tiberium_harvester::Get_Cash()
{
	float Return = 0;
	for(int x = 0; x < 8; x++)
		Return += CashCollected[x];
	return Return;
}
 
void tcw_tiberium_harvester::Clear_Cash()
{
	for(int x = 0; x < 8; x++)
		CashCollected[x] = 0;
	CashIndex = 0;
}

void tcw_tiberium_harvester::Created(GameObject *obj)
{

	if (Is_TCW_Harvester(obj)) // When we are a harvester..
	{
		HaveGreen = false;
		HaveBlue = false;
		PlayerTeam = -5;
		DriverID = 0;
		Harvesting = false;
		TibType = 0;
		Clear_Cash();
		CashIndex = 0;
		VehicleGameObj *Harvester = obj->As_VehicleGameObj();
		if (!obj->As_VehicleGameObj()) 
			return;
		Harvester->Set_Allow_Stealth_While_Empty(false);
	}
	else
		Destroy_Script(); // No need to leave the mess...
}

void tcw_tiberium_harvester::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		PlayerTeam = Commands->Get_Player_Type(sender);
		DriverID = sender->Get_ID();
		if(CashIndex < 8)
		{
			if(TibType) // We are already in a tib field...
			{
				Harvesting = true;
				Commands->Start_Timer(obj, this, 1, CUSTOM_EVENT_TIB_ENTERED);
				if(PlayerTeam == 0)
					Send_Message_Player(Commands->Find_Object(DriverID), 255, 0, 0,"You are now harvesting tiberium.");
				else if(PlayerTeam == 1)
					Send_Message_Player(Commands->Find_Object(DriverID), 255, 204, 0,"You are now harvesting tiberium.");
			}
			else
			{	// Give instructions what to do..
				if (PlayerTeam == 0)
					Send_Message_Player(Commands->Find_Object(DriverID), 255, 0, 0,"Drive to a Tiberium Field to harvest Tiberium.");
				else if (PlayerTeam == 1)
					Send_Message_Player(Commands->Find_Object(DriverID), 255, 204, 0,"Drive to a Tiberium Field to harvest Tiberium.");
			}
			if(CashIndex > 0)
			{
				if(PlayerTeam == 0)
					Send_Message_Player(Commands->Find_Object(DriverID), 255, 0, 0, PackArg("Your harvester containts $%g credits.", Get_Cash()));
				else if(PlayerTeam == 1)
					Send_Message_Player(Commands->Find_Object(DriverID), 255, 204, 0, PackArg("Your harvester contains $%g credits.", Get_Cash()));
			}
			Create_2D_WAV_Sound_Player(Commands->Find_Object(DriverID), "incmsg.wav");
		}
	}
	else if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Harvesting = false;
		DriverID = 0;
		PlayerTeam = -5;
	}
	else if (message == CUSTOM_EVENT_GREENTIB_ENTERED && DriverID)
	{
		TibType = TIBTYPE_GREEN;
		HaveGreen = true;
		if(CashIndex < 8) // We're not full
		{
			Harvesting = true;
			if(PlayerTeam == 0)
				Send_Message_Player(Commands->Find_Object(DriverID), 255, 0, 0,"You are now harvesting tiberium.");
			else if(PlayerTeam == 1)
				Send_Message_Player(Commands->Find_Object(DriverID), 255, 204, 0,"You are now harvesting tiberium.");
			Commands->Start_Timer(obj, this, 1, CUSTOM_EVENT_TIB_ENTERED);
			Create_2D_WAV_Sound_Player(Commands->Find_Object(DriverID),"incmsg.wav");
		}
		else
		{
			if (PlayerTeam == 0)
				Send_Message_Player(Commands->Find_Object(DriverID),255, 0, 0,"Tiberium harvesting has completed, return to the Refinery.");
			else if (PlayerTeam == 1)
				Send_Message_Player(Commands->Find_Object(DriverID),255, 204, 0,"Tiberium harvesting has completed, return to the Refinery.");
			Create_2D_WAV_Sound_Player(Commands->Find_Object(DriverID),"incmsg.wav");
		}
	}
	else if (message == CUSTOM_EVENT_BLUETIB_ENTERED && DriverID)
	{
		TibType = TIBTYPE_BLUE;
		HaveBlue = true;
		if(CashIndex < 8) // We're not full
		{
			Harvesting = true;
			if(PlayerTeam == 0)
				Send_Message_Player(Commands->Find_Object(DriverID), 255, 0, 0,"You are now harvesting tiberium.");
			else if(PlayerTeam == 1)
				Send_Message_Player(Commands->Find_Object(DriverID), 255, 204, 0,"You are now harvesting tiberium.");
			Commands->Start_Timer(obj, this, 1, CUSTOM_EVENT_TIB_ENTERED);
			Create_2D_WAV_Sound_Player(Commands->Find_Object(DriverID),"incmsg.wav");
		}
		else
		{
			if (PlayerTeam == 0)
				Send_Message_Player(Commands->Find_Object(DriverID),255, 0, 0,"Tiberium harvesting has completed, return to the Refinery.");
			else if (PlayerTeam == 1)
				Send_Message_Player(Commands->Find_Object(DriverID),255, 204, 0,"Tiberium harvesting has completed, return to the Refinery.");
			Create_2D_WAV_Sound_Player(Commands->Find_Object(DriverID),"incmsg.wav");
		}
	}
	else if (message == CUSTOM_EVENT_HARVY_LEFT_TIBFIELD)
	{
		Harvesting = false;
		TibType = 0;
	}
	else if (message == CUSTOM_EVENT_HARVY_DEPOSITING && !Is_Building_Dead(Find_Building_By_Type(PlayerTeam, BuildingConstants::TYPE_REFINERY)) && Get_Cash() > 0)
	{
		Commands->Give_Money(Commands->Find_Object(DriverID), Get_Cash(),true);
		if (HaveGreen && !HaveBlue)
			Commands->Give_Points(Commands->Find_Object(DriverID), 50, false);
		else if(HaveBlue && !HaveGreen)
			Commands->Give_Points(Commands->Find_Object(DriverID), 75, false);
		else if(HaveGreen && HaveBlue)
			Commands->Give_Points(Commands->Find_Object(DriverID), 60, false);
		if (PlayerTeam == 0)
		{
			Send_Message_Team(PlayerTeam, 255, 0, 0, PackArg("%ls deposited Tiberium funding Nod with %g additional credits.", ((cPlayer *)(Commands->Find_Object(DriverID)->As_SoldierGameObj())->Get_Player_Data())->PlayerName.Peek_Buffer(), Get_Cash()));
			Create_2D_WAV_Sound_Team("creditsacquired_nod.wav", 0);
		}
		else if (PlayerTeam == 1)
		{
			Send_Message_Team(PlayerTeam, 255, 204, 0, PackArg("%ls deposited Tiberium funding GDI with %g additional credits.", ((cPlayer *)(Commands->Find_Object(DriverID)->As_SoldierGameObj())->Get_Player_Data())->PlayerName.Peek_Buffer(), Get_Cash()));
			Create_2D_WAV_Sound_Team("creditsacquired_gdi.wav", 1);
		}
		Clear_Cash();
		HaveGreen = false;
		HaveBlue = false;
	}
}

void tcw_tiberium_harvester::Timer_Expired(GameObject *obj, int number)
{
	if(number == CUSTOM_EVENT_TIB_ENTERED && Harvesting && CashIndex < 8) // When in a tib field timer started, harvesting and we're not full do...
	{	// Check what we need to distribute
		if(TibType == TIBTYPE_GREEN)
			CashCollected[CashIndex] = 100;
		else if(TibType == TIBTYPE_BLUE)
			CashCollected[CashIndex] = 200;
		if(PlayerTeam == 0)
			Send_Message_Player(Commands->Find_Object(DriverID), 255, 0, 0, PackArg("You harvested $%g worth of Tiberium Crystals.", Get_Cash()));
		else if(PlayerTeam == 1)
			Send_Message_Player(Commands->Find_Object(DriverID), 255, 204, 0, PackArg("You harvested $%g worth of Tiberium Crystals.", Get_Cash()));
		Commands->Start_Timer(obj, this, 1, CUSTOM_EVENT_TIB_ENTERED);
		CashIndex++;
	}
	else if(CashIndex >= 8) // Oops we're full...
	{
		Harvesting = false;
		if (PlayerTeam == 0)
			Send_Message_Player(Commands->Find_Object(DriverID),255, 0, 0,"Tiberium harvesting has completed, return to the Refinery.");
		else if (PlayerTeam == 1)
			Send_Message_Player(Commands->Find_Object(DriverID),255, 204, 0,"Tiberium harvesting has completed, return to the Refinery.");
		Create_2D_WAV_Sound_Player(Commands->Find_Object(DriverID),"incmsg.wav");
	}
}

/////////////////////////////////////////////////////////
//					TCW_TIBERIUM_FIELD
////////////////////////////////////////////////////////

void tcw_tiberium_field::Created(GameObject *obj)
{
	Zonetype = Get_Int_Parameter("Zonetype"); //0=green 1=blue
}

void tcw_tiberium_field::Entered(GameObject *obj, GameObject *enterer)
{
	if (Is_TCW_Harvester(enterer))
	{
		if (Zonetype == 0) //green
			Commands->Send_Custom_Event(obj, enterer, CUSTOM_EVENT_GREENTIB_ENTERED, 0, 0);//tell harv a green zone entered 
		else if (Zonetype == 1) //blue
			Commands->Send_Custom_Event(obj, enterer, CUSTOM_EVENT_BLUETIB_ENTERED, 0, 0);//tell harv a blue zone entered
	}
}

void tcw_tiberium_field::Exited(GameObject *obj, GameObject *exiter)
{
	if ( Is_TCW_Harvester(exiter))
		Commands->Send_Custom_Event(obj, exiter, CUSTOM_EVENT_HARVY_LEFT_TIBFIELD, 0, 0);//tell harv left zone
}

/////////////////////////////////////////////////////////
//					TCW_TIBERIUM_DEPOSIT
////////////////////////////////////////////////////////

void tcw_tiberium_deposit::Entered(GameObject *obj, GameObject *enterer)
{
	if(Is_TCW_Harvester(enterer))
		Commands->Send_Custom_Event(obj,enterer, CUSTOM_EVENT_HARVY_DEPOSITING, 0, 0);
	
	if (strcmp(Commands->Get_Preset_Name(enterer),"TCW_GDI_Harvester") == 0)
	{
		Commands->Send_Custom_Event(obj, enterer, 12500, 0, 0);//tell harv set idle on
		Commands->Send_Custom_Event(obj, enterer, 12505, 0, 0);//tell harv set full off
	}
}

/////////////////////////////////////////////////////////
//					TCW_TIBERIUM_SPIKE
////////////////////////////////////////////////////////

void tcw_tiberium_spike::Created(GameObject *obj)
{
	FirstCapture = true;
	int SpikeTeam = Commands->Get_Player_Type(obj);
	if(SpikeTeam == -2)
	{
		Commands->Set_Animation(obj, "V_NEU_SPIKE.V_NEU_SPIKE", true, 0, 0, 0, 0);
		Set_Skin(obj, "Blamo");
		Commands->Set_Health(obj, 1);
		alive = false;
	}
	else if(SpikeTeam == 0 || SpikeTeam == 1)
	{
		Commands->Set_Animation(obj, "V_NEU_SPIKE.V_NEU_SPIKE", true, 0, 1, -1, 0);
		Commands->Set_Health(obj, Commands->Get_Max_Health(obj));
		Set_Skin(obj, "CNCMCTSkin");
		Commands->Start_Timer(obj, this, 1, 1);
		alive = true;
	}
}

void tcw_tiberium_spike::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_CAPTURE_SPIKE)
	{
		Commands->Set_Health(obj, Commands->Get_Max_Health(obj));
		Commands->Set_Animation(obj, "V_NEU_SPIKE.V_NEU_SPIKE", true, 0, 1, -1, 0);
		Commands->Set_Player_Type(obj, Commands->Get_Player_Type(sender));
		Set_Skin(obj, "CNCMCTSkin");
		if(FirstCapture)
		{
			FirstCapture = false;
			if(TheTCWGame()->Get_GiveCaptureCredits())
				Commands->Give_Money(sender, 250, true);
		}
		Commands->Start_Timer(obj, this, 1, 1);
		alive = true;
	}
	else if(message == CUSTOM_EVENT_CAPTURE_SPIKE_START_NOD)
	{
		if(Commands->Get_Player_Type(obj) == -2)
			Commands->Start_Timer(obj, this, 1, 1);
		Commands->Set_Player_Type(obj, 0);
		Commands->Set_Animation(obj, "V_NEU_SPIKE.V_NEU_SPIKE", true, 0, 1, -1, 0);
		Commands->Set_Health(obj, Commands->Get_Max_Health(obj));
		Set_Skin(obj, "CNCMCTSkin");
		alive = true;
	}
	else if(message == CUSTOM_EVENT_CAPTURE_SPIKE_START_GDI)
	{
		if(Commands->Get_Player_Type(obj) == -2)
			Commands->Start_Timer(obj, this, 1, 1);
		Commands->Set_Player_Type(obj, 1);
		Commands->Set_Animation(obj, "V_NEU_SPIKE.V_NEU_SPIKE", true, 0, 1, -1, 0);
		Commands->Set_Health(obj, Commands->Get_Max_Health(obj));
		Set_Skin(obj, "CNCMCTSkin");
		alive = true;
	}
}

void tcw_tiberium_spike::Timer_Expired(GameObject *obj, int number)
{
	if(number == 1 && (Commands->Get_Player_Type(obj) == 0 || Commands->Get_Player_Type(obj) == 1))
	{
		Commands->Give_Money(obj, 5, true);
		Commands->Start_Timer(obj, this, 1, 1);
	}
}

void tcw_tiberium_spike::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if(damage > Commands->Get_Health(obj) && (Commands->Get_Player_Type(obj) == 0 || Commands->Get_Player_Type(obj) == 1))
	{
		Set_Skin(obj, "Blamo");
		Commands->Set_Health(obj, 1);
		Commands->Set_Animation(obj, "V_NEU_SPIKE.V_NEU_SPIKE", true, 0, 0, 0, 0);
		if (alive)
		{
			if (Commands->Get_Player_Type(obj) == 0)
			{
				Create_2D_WAV_Sound_Team("nod_spike_destroyed.wav",1);
				Create_2D_WAV_Sound_Team("nod_spike_lost.wav",0);
				Send_Message_Team(0,255,255,255,"Nod Tiberium Spike lost.");
				Send_Message_Team(1,255,255,255,"Nod Tiberium Spike destroyed.");
			}
			else if (Commands->Get_Player_Type(obj) == 1)
			{
				Create_2D_WAV_Sound_Team("gdi_spike_lost.wav",1);
				Create_2D_WAV_Sound_Team("gdi_spike_destroyed.wav",0);
				Send_Message_Team(0,255,255,255,"GDI Tiberium Spike destroyed.");
				Send_Message_Team(1,255,255,255,"GDI Tiberium Spike lost.");
			}
			alive = false;
		}
		Commands->Set_Player_Type(obj, -2);
	}
	if(damage < 0 && Commands->Get_Player_Type(obj) == -2)
		Commands->Set_Health(obj, 1);
}

/////////////////////////////////////////////////////////
//				TCW_TIBERIUM_SPIKE_CAPTURE_ZONE
////////////////////////////////////////////////////////

void tcw_tiberium_spike_capture::Created(GameObject *obj)
{
	Capturer = 0;
	SpikeID = Get_Int_Parameter("SpikeID");
	if(!SpikeID)
		Destroy_Script();
}

void tcw_tiberium_spike_capture::Entered(GameObject *obj, GameObject *enterer)
{
	if(Commands->Is_A_Star(enterer) && !Get_Vehicle(enterer) && !Capturer)
	{
		TCWSoldierObj *Soldier = Find_TCWSoldier(enterer);
		if(Soldier && Soldier->Can_CaptureSpikes())
		{
			if(Commands->Get_Player_Type(Commands->Find_Object(SpikeID)) == Commands->Get_Player_Type(enterer))
			{
				if(Commands->Get_Player_Type(enterer) == 0)
					Send_Message_Player(enterer,255,0,0,"Your team already controls this Tiberium Spike.");
				else if(Commands->Get_Player_Type(enterer) == 1)
					Send_Message_Player(enterer,255,204,0,"Your team already controls this Tiberium Spike.");
			}
			else
			{
				if(Capturer)
					Send_Message_Player(enterer,255,255,255,"Capturing in progress, please try again in 20 seconds.");
				else
				{
					Capturer = enterer->Get_ID();
					Commands->Start_Timer(obj, this, 15, 1);
					if(Commands->Get_Player_Type(enterer) == 0)
					{
						Send_Message_Team(0, 255, 0, 0, PackArg("%s is capturing a Tiberium Spike.", Get_Player_Name(enterer)));
						Create_2D_WAV_Sound_Team("building_capture.wav", 0);
					}
					else if(Commands->Get_Player_Type(enterer) == 1)
					{
						Send_Message_Team(1, 255, 204, 0, PackArg("%s is capturing a Tiberium Spike.", Get_Player_Name(enterer)));
						Create_2D_WAV_Sound_Team("building_capture.wav", 1);
					}
				}
			}
		}
	}
}

void tcw_tiberium_spike_capture::Exited(GameObject *obj, GameObject *exiter)
{
	if (Commands->Is_A_Star(exiter) && exiter->Get_ID() == Capturer)
			Capturer = 0;
}
void tcw_tiberium_spike_capture::Timer_Expired(GameObject *obj, int number)
{
	if(number == 1)
	{
		if (Capturer && Commands->Find_Object(Capturer)) // If our Capturer is still there..
		{
			TCWSoldierObj *Soldier = Find_TCWSoldier(Capturer);
			if(Soldier)
				Soldier->Set_CanCaptureSpikes(false);
			GameObject *Player = Commands->Find_Object(Capturer);
			Send_Message(255,255,255, PackArg("%s captured a Tiberium Spike.", Get_Player_Name(Player)));
			int SpikeTeam = Commands->Get_Player_Type(Commands->Find_Object(SpikeID));	
			if(SpikeTeam == 0)
			{
				Create_2D_WAV_Sound_Team("gdi_building_captured.wav",1);
				Send_Message_Team(1,255,204,0,"Building captured.");
				Create_2D_WAV_Sound_Team("nod_building_captured_our.wav",0);
				Send_Message_Team(0,255,0,0,"Our building has been captured.");
			}
			else if(SpikeTeam == 1)
			{
				Create_2D_WAV_Sound_Team("nod_building_captured.wav",0);
				Send_Message_Team(0,255,0,0,"Building captured.");
				Create_2D_WAV_Sound_Team("gdi_building_captured_our.wav",1);
				Send_Message_Team(1,255,204,0,"Our building has been captured.");
			}
			else if(SpikeTeam == -2)
			{
				if(Commands->Get_Player_Type(Player) == 0)
				{
					Create_2D_WAV_Sound_Team("nod_building_captured.wav",0);
					Send_Message_Team(0,255,0,0,"Building captured.");
				}
				else if(Commands->Get_Player_Type(Player) == 1)
				{
					Create_2D_WAV_Sound_Team("gdi_building_captured.wav",1);
					Send_Message_Team(1,255,204,0,"Building captured.");
				}
			}
			Commands->Send_Custom_Event(Player, Commands->Find_Object(SpikeID), CUSTOM_EVENT_CAPTURE_SPIKE, 0, 0);
			Commands->Give_Points(Player,250, 0);
			Capturer = 0;
		}
		else
		{
			Capturer = 0;
		}
	}
}

void tcw_set_spike_team_created::Created(GameObject *obj)
{
	int team = Get_Int_Parameter("Team");
	if (team == 0)
	{
		Commands->Send_Custom_Event(obj,obj,CUSTOM_EVENT_CAPTURE_SPIKE_START_NOD,0,1);
	}
	else if (team == 1)
	{
		Commands->Send_Custom_Event(obj,obj,CUSTOM_EVENT_CAPTURE_SPIKE_START_GDI,0,1);
	}
	else 
	{
		Destroy_Script();
	}
}


ScriptRegistrant<tcw_set_spike_team_created> tcw_set_spike_team_created_Registrant("tcw_set_spike_team_created","Team=0:int");

ScriptRegistrant<tcw_tiberium_spike> tcw_tiberium_spike_Registrant("tcw_tiberium_spike","");
ScriptRegistrant<tcw_tiberium_spike_capture> tcw_tiberium_spike_capture_Registrant("tcw_tiberium_spike_capture","SpikeID=0:int");

ScriptRegistrant<tcw_tiberium_harvester> tcw_tiberium_harvester_Registrant("tcw_tiberium_harvester","");
ScriptRegistrant<tcw_tiberium_field> tcw_tiberium_field_Registrant("tcw_tiberium_field","Zonetype=0:int");
ScriptRegistrant<tcw_tiberium_deposit> tcw_tiberium_deposit_Registrant("tcw_tiberium_deposit","");