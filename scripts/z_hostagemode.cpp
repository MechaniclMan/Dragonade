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

/*********************************************************************************************/
#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "z_hostagemode.h"
/*********************************************************************************************/

/*********************************************************************************************/
void z_HostageFollowTest::Created(GameObject *obj)
{
	AllowPoke = true;
	FollowID = 0;
	Following = false;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}

void z_HostageFollowTest::Poked(GameObject *obj, GameObject *poker)
{
	if (AllowPoke)
	{
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		FollowID = Commands->Get_ID(poker);
		Commands->Start_Timer(obj,this,0.5,789);
	}
}
void z_HostageFollowTest::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("Stop_Following_Msg"))
	{
		Commands->Enable_HUD_Pokable_Indicator(obj,true);
		AllowPoke = true;
		Following = false;
		FollowID = 0;
	}
}
void z_HostageFollowTest::Timer_Expired(GameObject *obj,int number)//void Set_Goto_Location(const Vector3 & position,float speed,float arrivedistance,bool crouch);
{
	if (number == 789)
	{
		Commands->Innate_Disable(obj);
		GameObject *GotoObject = Commands->Find_Object(FollowID);
		if (GotoObject)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,100);
			params.Set_Movement(GotoObject,6.5f,1.5f);
			params.MoveFollow = true;
			Commands->Action_Goto(obj,params);
			Commands->Start_Timer(obj,this,1.0,789);
		}
	}
}
void z_HostageFollowTest::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&FollowID,4,1);
}

/*********************************************************************************************/

void z_GotoStar_Custom::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == Get_Int_Parameter("Message"))
	{
		Attach_Script_Once(obj,"M03_Goto_Star","");
	}
}

/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
//PVP hostage mode
/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/


void z_Hostage2::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	Commands->Set_Player_Type(obj,-2);
	Commands->Clear_Weapons(obj);
	Commands->Innate_Disable(obj);
	Commands->Start_Timer(obj,this,1.0f,788);
	AllowPoke = true;
	Following = false;
	FollowID = 0;
	Pokerguy = 0;
	HostageID = Commands->Get_ID(obj);
}

void z_Hostage2::Poked(GameObject *obj,GameObject *poker)
{
	if (AllowPoke)
	{
		Pokerguy = poker;
		FollowID = Commands->Get_ID(poker);
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Commands->Set_Player_Type(obj,Commands->Get_Player_Type(poker));
		Commands->Start_Timer(obj,this,0.5,789);
		AllowPoke = false;
		Following = true;
		Commands->Give_Money(Pokerguy,100.0f,0);
		int RandomNum = Commands->Get_Random_Int(1,5);
		if (RandomNum == 1 || RandomNum == 2)
		{
			Create_2D_WAV_Sound_Player(poker,"youlead.wav");
			Send_Message_Player(poker,243,0,0,"Hostage: Ok you lead the way.");
		}
		else
		{
			Create_2D_WAV_Sound_Player(poker,"getouttahere.wav");
			Send_Message_Player(poker,243,0,0,"Hostage: Let's get the hell outta here.");
		}
	}
}

void z_Hostage2::Timer_Expired(GameObject *obj, int number)
{
	if (number == 789)//this is the number we have been waiting for
	{
		if (Commands->Find_Object(FollowID))//w00t we found our hero to the rescue
		{
			GameObject *GotoObject = Commands->Find_Object(FollowID);
			ActionParamsStruct params;
			params.Set_Basic(this,100,100);
			params.Set_Movement(GotoObject,0.6f,1.5f);
			params.MoveFollow = true;
			Commands->Action_Goto(obj,params);
			Following = true;
			Commands->Start_Timer(obj,this,1.0f,789);//our hero is alive so we continue following
			Commands->Give_Money(Pokerguy,1.0f,0);
		}
		else//pokern00b died or left the game, stop following
		{
			Pokerguy = 0;
			FollowID = 0;//setting FollowID to 0
			Following = false;//not following anymore
			AllowPoke = true;//poking is allowed again because the person poking the hostage died or left
			Commands->Enable_HUD_Pokable_Indicator(obj,true);//can poke again so adjust the indicators
			Commands->Set_Player_Type(obj,-2);//unteam the hostage so they are not attacked by bots anymore
		}
	}
}


void z_Hostage2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("All_Rescue_Message"))
	{
		if (param == 1)
		{
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("Controller_ID")),Get_Int_Parameter("All_Rescue_Message"),1,0.0f);//lets tell the controller a hostage was rescued
			Commands->Give_Money(Pokerguy,250.0f,0);
			Commands->Give_Points(Pokerguy,100.0f,0);
			Commands->Destroy_Object(obj);//destroy hostage, rescued, good work :P
		}
	}
	else if (type == Get_Int_Parameter("Sov_Rescue_Message"))
	{
		if (param == 1)
		{
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("Controller_ID")),Get_Int_Parameter("Sov_Rescue_Message"),1,0.0f);//lets tell the controller a hostage was rescued
			Commands->Give_Money(Pokerguy,250.0f,0);
			Commands->Give_Points(Pokerguy,100.0f,0);
			Commands->Destroy_Object(obj);//destroy hostage, rescued, good work :P
		}
	}
}

void z_Hostage2::Killed(GameObject *obj,GameObject *killer)//hostage down
{
	if (Commands->Find_Object(Get_Int_Parameter("Controller_ID")))
	{
		if (Commands->Is_A_Star(killer))
		{
			Commands->Give_Money(killer,-100.0f,0);
			Commands->Give_Points(killer,-100.0f,0);
			if (Commands->Get_Player_Type(killer) == 0)
			{
				Commands->Send_Custom_Event(obj, Commands->Find_Object(Get_Int_Parameter("Controller_ID")),Get_Int_Parameter("Sov_HostageDead_Message"),1,0.0);//hosty died, tell controller to increment deaths
			}
			else if (Commands->Get_Player_Type(killer) == 1)
			{
				Commands->Send_Custom_Event(obj, Commands->Find_Object(Get_Int_Parameter("Controller_ID")),Get_Int_Parameter("All_HostageDead_Message"),1,0.0);//hosty died, tell controller to increment deaths
			}
		}
	}
	Commands->Destroy_Object(obj);
}

void z_Hostage2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&HostageID,4,1);
	Auto_Save_Variable(&FollowID,4,2);
}




void z_Hostage_Controller2::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,10.0f,800);
	Commands->Set_Player_Type(obj,-2);
	Set_Damage_Points(obj,0.0f);
	Attach_Script_Once(obj,"M00_Damage_Modifier_DME","0.00,1,1,0,0");
	Commands->Enable_Vehicle_Transitions(obj,false);
	Commands->Enable_HUD_Pokable_Indicator(obj,false);
	allhostagerescues = 0;
	sovhostagerescues = 0;
	allhostagedeaths = 0;
	sovhostagedeaths = 0;
}

void z_Hostage_Controller2::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == Get_Int_Parameter("All_Rescue_Message"))
	{
		if (param == 1)
		{
			allhostagerescues++;
			Commands->Create_2D_WAV_Sound("rescued.wav");
			Send_Message(255,255,255,"An Allied hostage has been rescued!");
		}
	}
	else if (type == Get_Int_Parameter("Sov_Rescue_Message"))
	{
		if (param == 1)
		{
			sovhostagerescues++;
			Commands->Create_2D_WAV_Sound("rescued.wav");
			Send_Message(255,255,255,"A Soviet hostage has been rescued!");
		}
	}

	else if (type == Get_Int_Parameter("All_HostageDead_Message"))
	{
		if (param == 1)
		{
			allhostagedeaths++;
			Commands->Create_2D_WAV_Sound("hosdown.wav");
			Send_Message(255,255,255,"Allied Hostage down!");
		}
	}
	else if (type == Get_Int_Parameter("Sov_HostageDead_Message"))
	{
		if (param == 1)
		{
			sovhostagedeaths++;
			Commands->Create_2D_WAV_Sound("hosdown.wav");
			Send_Message(255,255,255,"Soviet Hostage down!");
		}
	}

	if (sovhostagerescues == Get_Int_Parameter("Sov_Required_Rescues"))
	{
		sovhostagerescues = 0;
		Commands->Create_2D_WAV_Sound("rescued.wav");
		Commands->Start_Timer(obj,this,1.0f,787);
	}
	if (allhostagerescues == Get_Int_Parameter("All_Required_Rescues"))
	{
		sovhostagerescues = 0;
		Commands->Create_2D_WAV_Sound("rescued.wav");
		Commands->Start_Timer(obj,this,1.0f,786);
	}
	if (sovhostagedeaths == Get_Int_Parameter("Sov_Max_Hosty_Deaths"))
	{
		sovhostagedeaths = 0;
		Commands->Create_2D_WAV_Sound("hosdown.wav");
		Commands->Start_Timer(obj,this,1.0f,785);
	}
	if (allhostagedeaths == Get_Int_Parameter("All_Max_Hosty_Deaths"))
	{
		allhostagedeaths = 0;
		Commands->Create_2D_WAV_Sound("hosdown.wav");
		Commands->Start_Timer(obj,this,1.0f,784);
	}
}

void z_Hostage_Controller2::Timer_Expired(GameObject *obj, int number)
{
	if (number == 787)//soviet win by max recues for allies
	{
		Console_Input("win 0");
		Console_Input("amsg All Soviet hostages were rescued!");
	}
	else if (number == 786)//allies win by max rescues
	{
		Console_Input("win 1");
		Console_Input("amsg All Allied hostages were rescued!");
	}
	if (number == 785)//allies win by soviet hostage deaths
	{
		Console_Input("win 1");
		Console_Input("amsg All Soviet hostages were terminated, your mission failed Soviets!");
	}
	else if (number == 784)//soviet win by allied hostage deaths
	{
		Console_Input("win 0");
		Console_Input("amsg All Allied hostages were terminated, your mission failed Allies!");
	}
	else if (number == 800)
	{
		char msg[200]; char msg1[200]; char msg2[200];
		sprintf(msg,"msg This map requires %d number of rescues or %d number of hostage deaths.",Get_Int_Parameter("All_Required_Rescues"),Get_Int_Parameter("All_Max_Hosty_Deaths"));
		sprintf(msg1,"msg Allied hostage rescues/deaths so far: %d/%d.",allhostagerescues,allhostagedeaths);
		sprintf(msg2,"msg Soviet hostage rescues/deaths so far: %d/%d.",sovhostagerescues,sovhostagedeaths);
		Console_Input(msg);		Console_Input(msg1);	Console_Input(msg2);
		Commands->Start_Timer(obj,this,100.0f,800);
	}
}

void z_Hostage_Controller2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allhostagerescues,4,1);
	Auto_Save_Variable(&sovhostagerescues,4,2);
	Auto_Save_Variable(&allhostagedeaths,4,3);
	Auto_Save_Variable(&sovhostagedeaths,4,4);
}

void z_Hostage_Rescue_Zone2::Entered(GameObject *obj,GameObject *enterer)
{
	if (!Commands->Is_A_Star(enterer))
	{
		if ((strcmp(Commands->Get_Preset_Name(enterer),(Get_Parameter("Hostage_Preset"))) == 0) ||
		(strcmp(Commands->Get_Preset_Name(enterer),(Get_Parameter("Hostage_Preset2"))) == 0) ||
		(strcmp(Commands->Get_Preset_Name(enterer),(Get_Parameter("Hostage_Preset3"))) == 0) ||
		(strcmp(Commands->Get_Preset_Name(enterer),(Get_Parameter("Hostage_Preset4"))) == 0) ||
		(strcmp(Commands->Get_Preset_Name(enterer),(Get_Parameter("Hostage_Preset5"))) == 0))
		{
			if (Commands->Get_Player_Type(enterer) == 0)
			{
				Commands->Send_Custom_Event(obj,enterer,(Get_Int_Parameter("Sov_Rescue_Message")),1,0.0f);
			}
			else if (Commands->Get_Player_Type(enterer) == 1)
			{
				Commands->Send_Custom_Event(obj,enterer,(Get_Int_Parameter("All_Rescue_Message")),1,0.0f);
			}
		}
	}
}

ScriptRegistrant<z_HostageFollowTest> z_HostageFollowTest_Registrant("z_HostageFollowTest","Stop_Following_Msg=125:int");
ScriptRegistrant<z_GotoStar_Custom> z_GotoStar_Custom_Registrant("z_GotoStar_Custom","Message=125:int");


ScriptRegistrant<z_Hostage2> z_Hostage2_Registrant("z_Hostage2","All_Rescue_Message=1000:int,Sov_Rescue_Message=2000:int,Controller_ID=1010:int,All_HostageDead_Message=1010:int,Sov_HostageDead_Message=2020:int");
ScriptRegistrant<z_Hostage_Controller2> z_Hostage_Controller2_Registrant("z_Hostage_Controller2","All_Rescue_Message=1000:int,Sov_Rescue_Message=2000:int,All_HostageDead_Message=1001:int,Sov_HostageDead_Message=2001:int,All_Required_Rescues=10:int,Sov_Required_Rescues=10:int,All_Max_Hosty_Deaths=10:int,Sov_Max_Hosty_Deaths=10:int");
ScriptRegistrant<z_Hostage_Rescue_Zone2> z_Hostage_Rescue_Zone2_Registrant("z_Hostage_Rescue_Zone2","Hostage_Preset=Nun:string,Hostage_Preset2=Priest:string,Hostage_Preset3=civ_lab_tech_01:string,Hostage_Preset4=civ_lab_tech_02:string,Hostage_Preset5=civ_lab_tech_03:string,All_Rescue_Message=1000:int,Sov_Rescue_Message=2000:int");

//if you are actually reading these comments then you need a life