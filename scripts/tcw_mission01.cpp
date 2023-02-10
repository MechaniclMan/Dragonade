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
#ifdef SSGM
#include "gmgame.h"
#endif
#include "tcw_mission01.h"
#include "GameObjManager.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"

void tcw_m01_gdi_beachdrops::Created(GameObject *obj)
{
	deaths = 0;
	Commands->Start_Timer(obj,this,10.0f,101);
	Commands->Start_Timer(obj,this,13.0f,102);
}
void tcw_m01_gdi_beachdrops::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 134135)
	{
		if (param == 1)
		{
			deaths++;
			if (deaths == 6)//all reinforcement bots are dead
			{
				Commands->Start_Timer(obj,this,27.0f,101);
				Commands->Start_Timer(obj,this,30.0f,102);
				deaths = 0;
			}
		}
	}
}
void tcw_m01_gdi_beachdrops::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Vector3 position1 = Commands->Get_Position(Commands->Find_Object(100182));
		GameObject *drop1 = Commands->Create_Object("Invisible_Object",position1);
		Commands->Set_Facing(drop1,45.000f);
		Commands->Attach_Script(drop1,"Test_Cinematic","gdi_beachdrop1.txt");
	}
	else if (number == 102)
	{
		Vector3 position2 = Commands->Get_Position(Commands->Find_Object(100183));
		GameObject *drop2 = Commands->Create_Object("Invisible_Object",position2);
		Commands->Set_Facing(drop2,95.000f);
		Commands->Attach_Script(drop2,"Test_Cinematic","gdi_beachdrop2.txt");
	}
}

void tcw_m01_gdi_beachdrops_predators::Created(GameObject *obj)
{
	deaths = 0;
}
void tcw_m01_gdi_beachdrops_predators::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 100100)
	{
		if (param == 100)
		{
			deaths++;
			if (deaths == 3)
			{
				Commands->Start_Timer(obj,this,20.0f,101);
				Commands->Start_Timer(obj,this,30.0f,102);
				Commands->Start_Timer(obj,this,40.0f,103);
				deaths = 0;
			}
		}
	}
}
void tcw_m01_gdi_beachdrops_predators::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Vector3 position1;		position1.X = -103.481f;	position1.Y = 79.523f;	position1.Z = -5.175f;
		GameObject *drop1 = Commands->Create_Object("Invisible_Object",position1);
		Commands->Set_Facing(drop1,43.854f);
		Commands->Attach_Script(drop1,"Test_Cinematic","gdi_beachdroppred.txt");
	}
	else if (number == 102)
	{
		Vector3 position2;		position2.X = -127.818f;	position2.Y = 112.567f;	position2.Z = -5.175f;
		GameObject *drop2 = Commands->Create_Object("Invisible_Object",position2);
		Commands->Set_Facing(drop2,85.854f);
		Commands->Attach_Script(drop2,"Test_Cinematic","gdi_beachdroppred.txt");
	}
	else if (number == 103)
	{
		Vector3 position3;		position3.X = -114.919f;	position3.Y = 95.872f;	position3.Z = -5.175f;
		GameObject *drop3 = Commands->Create_Object("Invisible_Object",position3);
		Commands->Set_Facing(drop3,102.854f);
		Commands->Attach_Script(drop3,"Test_Cinematic","gdi_beachdroppred.txt");
	}
}


void tcw_m01_gdi_beachdrop_bot::Destroyed(GameObject *obj)
{
	int target = Commands->Get_ID(Find_Object_With_Script("tcw_m01_gdi_beachdrops"));
	Commands->Send_Custom_Event(obj,Commands->Find_Object(target),134135,1,0);
}

void tcw_m01_engi_hostage::Created(GameObject *obj)
{
	PlayPain = true;
	PlayDead = true;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	FollowID = 0;
	Prisoner = Get_Int_Parameter("Prisoner");
}
void tcw_m01_engi_hostage::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (damage > 0.1f)
	{
		if (PlayPain)
		{
			PlayPain = false; Commands->Start_Timer(obj,this,1.0f,200);
			const char *painsnd1 = "tcw_m01_hpain1.wav"; const char *painsnd2 = "tcw_m01_hpain2.wav"; const char *painsnd3 = "tcw_m01_hpain3.wav"; const char *painsnd4 = "tcw_m01_hpain4.wav"; const char *painsnd5 = "tcw_m01_hpain5.wav"; const char *painsnd6 = "tcw_m01_hpain6.wav";
			int randompain = Commands->Get_Random_Int(1,6);
			if (randompain == 1) { Commands->Create_2D_WAV_Sound(painsnd1); }
			else if (randompain == 2) { Commands->Create_2D_WAV_Sound(painsnd2); }
			else if (randompain == 3) { Commands->Create_2D_WAV_Sound(painsnd3); }
			else if (randompain == 4) { Commands->Create_2D_WAV_Sound(painsnd4); }
			else if (randompain == 5) { Commands->Create_2D_WAV_Sound(painsnd5); }
			else if (randompain == 6) { Commands->Create_2D_WAV_Sound(painsnd6); }
		}
	}
}
void tcw_m01_engi_hostage::Poked(GameObject *obj, GameObject *poker)
{
	if (!Commands->Find_Object(FollowID))
	{
		const char *sound1 = "tcw_m01_getouttahere.wav"; const char *sound2 = "tcw_m01_illfollow.wav"; const char *sound3 = "tcw_m01_letsdoit.wav"; const char *sound4 = "tcw_m01_letsgo.wav"; const char *sound5 = "tcw_m01_letshurry.wav"; const char *sound6 = "tcw_m01_letsmove.wav"; const char *sound7 = "tcw_m01_okletsgo.wav"; const char *sound8 = "tcw_m01_youlead.wav";
		int randomsnd = Commands->Get_Random_Int(1,8);
		if (randomsnd == 1) { Commands->Create_2D_WAV_Sound(sound1); }
		else if (randomsnd == 2) { Commands->Create_2D_WAV_Sound(sound2); }
		else if (randomsnd == 3) { Commands->Create_2D_WAV_Sound(sound3); }
		else if (randomsnd == 4) { Commands->Create_2D_WAV_Sound(sound4); }
		else if (randomsnd == 5) { Commands->Create_2D_WAV_Sound(sound5); }
		else if (randomsnd == 6) { Commands->Create_2D_WAV_Sound(sound6); }
		else if (randomsnd == 7) { Commands->Create_2D_WAV_Sound(sound7); }
		else if (randomsnd == 8) { Commands->Create_2D_WAV_Sound(sound8); }
#ifdef SSGM
		char followmsg[512];
		sprintf(followmsg,"%s is following %s",Get_Translated_Preset_Name_Ex(obj),Get_Player_Name(poker));
		Send_Message(255,255,255,followmsg);
#endif
		FollowID = Commands->Get_ID(poker);
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Commands->Set_Player_Type(obj,1);
		Commands->Start_Timer(obj,this,0.1f,100);
	}
}
void tcw_m01_engi_hostage::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		if (Commands->Find_Object(FollowID))
		{
			GameObject *GotoObject = Commands->Find_Object(FollowID);
			ActionParamsStruct params;
			params.Set_Basic(this,100,100);
			params.Set_Movement(GotoObject,0.75f,1.5f);
			params.MoveFollow = true;
			Commands->Action_Goto(obj,params);
			Commands->Start_Timer(obj,this,1.0f,100);
		}
		else
		{
			FollowID = 0;
			Commands->Action_Reset(obj,100);
			Commands->Enable_HUD_Pokable_Indicator(obj,true);
			Commands->Set_Player_Type(obj,-2);
		}
	}
	else if (number == 200)
	{
		PlayPain = true;
	}
}
void tcw_m01_engi_hostage::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			FollowID = 0;
			Commands->Action_Reset(obj,100);
			Destroy_Script();
		}
	}
}
void tcw_m01_engi_hostage::Killed(GameObject *obj, GameObject *shooter)
{
	if (PlayDead)
	{
		PlayDead = false;
		int randomdead = Commands->Get_Random_Int(1,6);
		if (randomdead == 1) { Commands->Create_2D_WAV_Sound("tcw_m01_death1.wav"); }
		else if (randomdead == 2) { Commands->Create_2D_WAV_Sound("tcw_m01_death2.wav"); }
		else if (randomdead == 3) { Commands->Create_2D_WAV_Sound("tcw_m01_death3.wav"); }
		else if (randomdead == 4) { Commands->Create_2D_WAV_Sound("tcw_m01_death4.wav"); }
		else if (randomdead == 5) { Commands->Create_2D_WAV_Sound("tcw_m01_death5.wav"); }
		else if (randomdead == 6) { Commands->Create_2D_WAV_Sound("tcw_m01_death6.wav"); }
	}
}
void tcw_m01_engi_hostage::Destroyed(GameObject *obj)
{	
	if (Prisoner == 1)
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_m01_engi_hostage_control"),1000,1,0);
	}
	else if (Prisoner == 2)
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_m01_engi_hostage_control"),1000,2,0);
	}
	else if (Prisoner == 3)
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_m01_engi_hostage_control"),1000,3,0);
	}
	Commands->Create_2D_WAV_Sound("tcw_m01_hosdown.wav");
#ifdef SSGM
		char deadmsg[512];
		sprintf(deadmsg,"Hostage Down: %s!",Get_Translated_Preset_Name_Ex(obj));
		Send_Message(255,255,255,deadmsg);
#endif
}

void tcw_m01_engi_hostage_rescuezone::Entered(GameObject *obj, GameObject *enter)
{
	if (strcmp(Commands->Get_Preset_Name(enter),"TCW_M01_GDI_Engineer_Prisoner") == 0)
	{
		Set_Skin(enter,"Blamo");
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_m01_engi_hostage_control"),1000,11,0);
		Commands->Send_Custom_Event(obj,enter,1000,1,0);
		Commands->Create_2D_WAV_Sound("tcw_m01_rescued.wav");
		Send_Message(255,255,255,"A hostage has been rescued");
	}
	else if (strcmp(Commands->Get_Preset_Name(enter),"TCW_M01_GDI_Engineer_Prisoner2") == 0)
	{
		Set_Skin(enter,"Blamo");
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_m01_engi_hostage_control"),1000,22,0);
		Commands->Send_Custom_Event(obj,enter,1000,1,0);
		Commands->Create_2D_WAV_Sound("tcw_m01_rescued.wav");
		Send_Message(255,255,255,"A hostage has been rescued");
	}
	else if (strcmp(Commands->Get_Preset_Name(enter),"TCW_M01_GDI_Engineer_Prisoner3") == 0)
	{
		Set_Skin(enter,"Blamo");
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_m01_engi_hostage_control"),1000,33,0);
		Commands->Send_Custom_Event(obj,enter,1000,1,0);
		Commands->Create_2D_WAV_Sound("tcw_m01_rescued.wav");
		Send_Message(255,255,255,"A hostage has been rescued");
	}
}

void tcw_m01_engi_hostage_control::Created(GameObject *obj)
{
	PrisonerRescues = 0;
	PrisonerRescuesRequired = 3;
	PrisonerDeaths = 0;
	PrisonerDeathsMax = 9;
	PrisonerDeathsTotal = 0;
	Prisoner1Alive = true;
	Prisoner2Alive = true;
	Prisoner3Alive = true;
	TriggerChinook = true;
	Vector3 Prisoner1Pos = Commands->Get_Position(Commands->Find_Object(100232));
	Vector3 Prisoner2Pos = Commands->Get_Position(Commands->Find_Object(100233));
	Vector3 Prisoner3Pos = Commands->Get_Position(Commands->Find_Object(100234));
	GameObject *Prisoner1 = Commands->Create_Object("TCW_M01_GDI_Engineer_Prisoner",Prisoner1Pos);
	GameObject *Prisoner2 = Commands->Create_Object("TCW_M01_GDI_Engineer_Prisoner2",Prisoner2Pos);
	GameObject *Prisoner3 = Commands->Create_Object("TCW_M01_GDI_Engineer_Prisoner3",Prisoner3Pos);
	Attach_Script_Once(Prisoner1,"tcw_m01_engi_hostage","1");
	Attach_Script_Once(Prisoner2,"tcw_m01_engi_hostage","2");
	Attach_Script_Once(Prisoner3,"tcw_m01_engi_hostage","3");
}
void tcw_m01_engi_hostage_control::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)//prisoner1 died
		{
			PrisonerDeaths++;
			PrisonerDeathsTotal++;
			Prisoner1Alive = false;
		}
		else if (param == 2)//prisoner2 died
		{
			PrisonerDeaths++;
			PrisonerDeathsTotal++;
			Prisoner2Alive = false;
		}
		else if (param == 3)//prisoner3 died
		{
			PrisonerDeaths++;
			PrisonerDeathsTotal++;
			Prisoner3Alive = false;
		}

		else if (param == 11)//prisoner1 rescued
		{
			Prisoner1Rescued = true;
			PrisonerRescues++;
		}
		else if (param == 22)//prisoner2 rescued
		{
			Prisoner2Rescued = true;
			PrisonerRescues++;
		}
		else if (param == 33)//prisoner3 rescued
		{
			Prisoner3Rescued = true;
			PrisonerRescues++;
		}
		Commands->Start_Timer(obj,this,1.0f,100);
	}
	else if (message == 2000)//chinook reached destination, destroy the engi's and chinook will fly away
	{
		if (param == 1)
		{
			const char *prisoner1 = "TCW_M01_GDI_Engineer_Prisoner";			const char *prisoner2 = "TCW_M01_GDI_Engineer_Prisoner2";			const char *prisoner3 = "TCW_M01_GDI_Engineer_Prisoner3";
			Commands->Destroy_Object(Find_Object_By_Preset(2,prisoner1));		Commands->Destroy_Object(Find_Object_By_Preset(2,prisoner2));		Commands->Destroy_Object(Find_Object_By_Preset(2,prisoner3));
			Console_Input("msg Objective Completed: Bazil, Mauler and Goldy were rescued.");
		}
	}
}
void tcw_m01_engi_hostage_control::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		if (PrisonerRescues == 3)
		{
			if (TriggerChinook)
			{
				TriggerChinook = false;
				Vector3 ChinookPickupLocation = Commands->Get_Position(Commands->Find_Object(100231));
				GameObject *Pickup = Commands->Create_Object("Invisible_Object",ChinookPickupLocation);
				Commands->Set_Facing(Pickup,45.000f);
				Attach_Script_Once(Pickup,"Test_Cinematic","gdi_base_pickup_engineers.txt");
			}
		}
		else if (PrisonerDeaths + PrisonerRescues == 3)
		{
			PrisonerDeaths = 0;
			if (PrisonerDeathsTotal == PrisonerDeathsMax)
			{
				Console_Input("msg Game would end now as the prisoner max deaths is reached");
			}
			else if (PrisonerRescues < PrisonerRescuesRequired)
			{
				if (!Prisoner1Rescued && !Prisoner1Alive)
				{
					Vector3 Prisoner1Pos = Commands->Get_Position(Commands->Find_Object(100232));
					GameObject *Prisoner1 = Commands->Create_Object("TCW_M01_GDI_Engineer_Prisoner",Prisoner1Pos);
					Attach_Script_Once(Prisoner1,"tcw_m01_engi_hostage","1");
					Prisoner1Alive = true;
				}
				if (!Prisoner2Rescued && !Prisoner2Alive)
				{
					Vector3 Prisoner2Pos = Commands->Get_Position(Commands->Find_Object(100233));
					GameObject *Prisoner2 = Commands->Create_Object("TCW_M01_GDI_Engineer_Prisoner2",Prisoner2Pos);
					Attach_Script_Once(Prisoner2,"tcw_m01_engi_hostage","2");
					Prisoner2Alive = true;
				}
				if (!Prisoner3Rescued && !Prisoner3Alive)
				{
					Vector3 Prisoner3Pos = Commands->Get_Position(Commands->Find_Object(100234));
					GameObject *Prisoner3 = Commands->Create_Object("TCW_M01_GDI_Engineer_Prisoner3",Prisoner3Pos);
					Attach_Script_Once(Prisoner3,"tcw_m01_engi_hostage","3");
					Prisoner3Alive = true;
				}
			}
		}
	}
}

ScriptRegistrant<tcw_m01_engi_hostage_control> tcw_m01_engi_hostage_control_Registrant("tcw_m01_engi_hostage_control","");
ScriptRegistrant<tcw_m01_engi_hostage_rescuezone> tcw_m01_engi_hostage_rescuezone_Registrant("tcw_m01_engi_hostage_rescuezone","");
ScriptRegistrant<tcw_m01_engi_hostage> tcw_m01_engi_hostage_Registrant("tcw_m01_engi_hostage","Prisoner=0:int");
ScriptRegistrant<tcw_m01_gdi_beachdrops_predators> tcw_m01_gdi_beachdrops_predators_Registrant("tcw_m01_gdi_beachdrops_predators","");
ScriptRegistrant<tcw_m01_gdi_beachdrop_bot> tcw_m01_gdi_beachdrop_bot_Registrant("tcw_m01_gdi_beachdrop_bot","");
ScriptRegistrant<tcw_m01_gdi_beachdrops> tcw_m01_gdi_beachdrops_Registrant("tcw_m01_gdi_beachdrops","");