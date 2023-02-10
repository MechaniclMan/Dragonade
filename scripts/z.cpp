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
#include "z.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"
#include "ScriptableGameObj.h"
#ifdef SSGM
#include "gmgame.h"
#endif


void z_I_May_Not_Place_Defenses::Created(GameObject *obj)
{
	if ( (strcmp(The_Game()->Get_Map_Name(),"RA_Soviet_Siege_Coop.mix") == 0) || 
		(strcmp(The_Game()->Get_Map_Name(),"RA_Allied_M02.mix") == 0) || 
		 (strcmp(The_Game()->Get_Map_Name(),"RA_Operation_Endgame.mix") == 0) )
	{
		Destroy_Script();
	}
	else
	{
		//no placing
	}
}

void z_No_PlaceDefense_Zone_Enter::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		Attach_Script_Once(enter,"z_I_May_Not_Place_Defenses","");
	}
}

void z_No_PlaceDefense_Zone_Exit::Exited(GameObject *obj, GameObject *exit)
{
	if (Commands->Is_A_Star(exit))
	{
		Remove_Script(exit,"z_I_May_Not_Place_Defenses");
	}
}



void z_Console_Input_Killed::Killed(GameObject *obj,GameObject *shooter)
{
	const char *Input;
	Input = Get_Parameter("Input");
	Console_Input(Get_Parameter("Input"));
}

void z_Console_Input_Killed_2::Killed(GameObject *obj,GameObject *shooter)
{
	const char *Input;
	Input = Get_Parameter("Input");
	Console_Input(Get_Parameter("Input"));
	const char *Input2;
	Input2 = Get_Parameter("Input2");
	Console_Input(Get_Parameter("Input2"));
}

void z_Console_Input_Custom_2::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		Console_Input(Get_Parameter("Input"));
		Console_Input(Get_Parameter("Input2"));
	}
}

void z_Play_Three_Cinematics_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	int msg;
	const char *Cinematic1;
	const char *Cinematic2;
	const char *Cinematic3;
	const char *Console_Command;
	Vector3 position1;
	Vector3 position2;
	Vector3 position3;
	GameObject *object1;
	GameObject *object2;
	GameObject *object3;
	float facing1;
	float facing2;
	float facing3;
	msg = Get_Int_Parameter("Message");
	Cinematic1 = Get_Parameter("Cinematic1");
	Cinematic2 = Get_Parameter("Cinematic2");
	Cinematic3 = Get_Parameter("Cinematic3");
	position1 = Get_Vector3_Parameter("Location1");
	position2 = Get_Vector3_Parameter("Location2");
	position3 = Get_Vector3_Parameter("Location3");
	facing1 = Get_Float_Parameter("Facing1");
	facing2 = Get_Float_Parameter("Facing2");
	facing3 = Get_Float_Parameter("Facing3");
	Console_Command = Get_Parameter("Console_Command");
	if (message == msg)
	{
		object1 = Commands->Create_Object("Invisible_Object",position1);
		Commands->Set_Facing(object1,facing1);
		Commands->Attach_Script(object1,"Test_Cinematic",Cinematic1);
		object2 = Commands->Create_Object("Invisible_Object",position2);
		Commands->Set_Facing(object2,facing2);
		Commands->Attach_Script(object2,"Test_Cinematic",Cinematic2);
		object3 = Commands->Create_Object("Invisible_Object",position3);
		Commands->Set_Facing(object3,facing3);
		Commands->Attach_Script(object3,"Test_Cinematic",Cinematic3);
		Console_Input(Get_Parameter("Console_Command"));
	}
}

void z_Play_Cinematic_Console_Input_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	int msg;
	const char *scriptname;
	const char *Console_Command;
	Vector3 position;
	GameObject *object;
	float facing;
	msg = Get_Int_Parameter("Message");
	scriptname = Get_Parameter("Script_Name");
	position = Get_Vector3_Parameter("Location");
	facing = Get_Float_Parameter("Facing");
	Console_Command = Get_Parameter("Console_Command");
	if (message == msg)
	{
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
		Console_Input(Get_Parameter("Console_Command"));
	}
}

void z_Destroy_Three_Objects_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	int msg;
	int id1;
	int id2;
	int id3;
	GameObject *object;
	msg = Get_Int_Parameter("Message");
	id1 = Get_Int_Parameter("ID1");
	id2 = Get_Int_Parameter("ID2");
	id3 = Get_Int_Parameter("ID3");
	if (msg == message)
	{
		object = Commands->Find_Object(id1);
		Commands->Destroy_Object(object);
		object = Commands->Find_Object(id2);
		Commands->Destroy_Object(object);
		object = Commands->Find_Object(id3);
		Commands->Destroy_Object(object);
	}
}

void z_Enable_Multiple_Spawners_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		Commands->Enable_Spawner(Get_Int_Parameter("ID1"),Get_Int_Parameter("Enable"));
		Commands->Enable_Spawner(Get_Int_Parameter("ID2"),Get_Int_Parameter("Enable"));
		Commands->Enable_Spawner(Get_Int_Parameter("ID3"),Get_Int_Parameter("Enable"));
		Commands->Enable_Spawner(Get_Int_Parameter("ID4"),Get_Int_Parameter("Enable"));
		Commands->Enable_Spawner(Get_Int_Parameter("ID5"),Get_Int_Parameter("Enable"));
	}
}

void z_Console_Input_Poke_2::Poked(GameObject *obj,GameObject *poker)
{
	const char *Console_Command1;
	const char *Console_Command2;
	Console_Command1 = Get_Parameter("Console_Command1");
	Console_Command2 = Get_Parameter("Console_Command2");
	{
		Console_Input(Get_Parameter("Console_Command1"));
		Console_Input(Get_Parameter("Console_Command2"));
	}
}


void z_Console_Input_Custom_Delay_1::Custom(GameObject *obj,int message,int timernumber,GameObject *sender)
{
	if (Get_Int_Parameter("Message") == message)
	{
		float timertime,timemin,timemax;
		timemin = Get_Float_Parameter("Time_Min");
		timemax = Get_Float_Parameter("Time_Max");
		timernumber = Get_Int_Parameter("TimerNum");
		timertime = Commands->Get_Random(timemin,timemax);
		if (Get_Float_Parameter("Time_Min"),Get_Float_Parameter("Time_Max") > 0)
			Commands->Start_Timer(obj,this,timertime,timernumber);
	}
}
void z_Console_Input_Custom_Delay_1::Timer_Expired(GameObject *obj,int number)
{
	float timertime,timemin,timemax;
	int timernumber;
	int repeat;
	const char *Input1;
	timemin = Get_Float_Parameter("Time_Min");
	timemax = Get_Float_Parameter("Time_Max");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	if (number == timernumber)
	{
		Input1 = Get_Parameter("Input1");
		Console_Input(Input1);
		if (repeat == 1)
		{
			timertime = Commands->Get_Random(timemin,timemax);
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void z_Console_Input_Custom_Delay_2::Custom(GameObject *obj,int message,int timernumber,GameObject *sender)
{
	if (Get_Int_Parameter("Message") == message)
	{
		float timertime,timemin,timemax;
		timemin = Get_Float_Parameter("Time_Min");
		timemax = Get_Float_Parameter("Time_Max");
		timernumber = Get_Int_Parameter("TimerNum");
		timertime = Commands->Get_Random(timemin,timemax);
		if (Get_Float_Parameter("Time_Min"),Get_Float_Parameter("Time_Max") > 0)
			Commands->Start_Timer(obj,this,timertime,timernumber);
	}
}
void z_Console_Input_Custom_Delay_2::Timer_Expired(GameObject *obj,int number)
{
	float timertime,timemin,timemax;
	int timernumber;
	int repeat;
	const char *Input1;
	const char *Input2;
	timemin = Get_Float_Parameter("Time_Min");
	timemax = Get_Float_Parameter("Time_Max");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	if (number == timernumber)
	{
		Input1 = Get_Parameter("Input1");
		Input2 = Get_Parameter("Input2");
		Console_Input(Input1);
		Console_Input(Input2);
		if (repeat == 1)
		{
			timertime = Commands->Get_Random(timemin,timemax);
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void z_RemoveWeaponModel::Created(GameObject *obj)
{
	const char *Weapon;
	Weapon = Get_Parameter("Weapon_Model");
	Remove_Weapon(obj,Weapon);
}

void z_rwk::Killed(GameObject *obj, GameObject *shooter)
{
	if (Commands->Is_A_Star(shooter))
	Commands->Create_Object(Commands->Get_Preset_Name(obj),Commands->Get_Position(obj));
}

void z_GameoverCreated::Created(GameObject *obj)
{
	char Msg[100]; char Msg2[100];
	Console_Input("gameover");
	sprintf(Msg,"amsg Mission Accomplished!");
	sprintf(Msg2,"snda m00evag_dsgn0005i1evag_snd.wav");
	Console_Input(Msg);
	Console_Input(Msg2);
}

void z_Created_Send_Custom_Param::Created(GameObject *obj)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),Get_Int_Parameter("param"),0);
}

void z_NoDamageMoneyPoints::Created(GameObject *obj)
{
	Set_Damage_Points(obj,0.0f);
}
void z_NoDamageMoneyPoints::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
}

void z_Set_Team::Created(GameObject *obj)
{
	Commands->Set_Player_Type(obj,Get_Int_Parameter("team"));
}

void z_Set_Skin_Created::Created(GameObject *obj)
{
	const char *skin;
	float amm;
	{
		skin = Get_Parameter("SkinType_Created");
		amm = Get_Float_Parameter("Ammount");
		Set_Skin(obj,skin);
		Commands->Set_Health(obj,amm);
	}
}
void z_Set_Skin_Created::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	const char *skin;
	const char *rmvs;
	if (message == Get_Int_Parameter("Message"))
	{
		rmvs = Get_Parameter("Remove_Script");
		skin = Get_Parameter("NewSkinType");
		Set_Skin(obj,skin);
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
		Remove_Script(obj,rmvs);
	}
}

void z_Set_Skin::Created(GameObject *obj)
{
	const char *skin;
	float amm;
	{
		skin = Get_Parameter("SkinType");
		amm = Get_Float_Parameter("Ammount");
		Set_Skin(obj,skin);
		Set_Max_Health(obj,amm);//added so if amount if larger than the objects current settings it will reset the max health else it would not work properly
		Commands->Set_Health(obj,amm);
	}
}

void z_Death_Enable_Spawner::Killed(GameObject *obj,GameObject *shooter) 
{
	Commands->Enable_Spawner(Get_Int_Parameter("ID"),Get_Int_Parameter("Enable"));
}

void z_Death_Enable_3Spawners::Killed(GameObject *obj,GameObject *shooter)
{
	Commands->Enable_Spawner(Get_Int_Parameter("ID1"),Get_Int_Parameter("Enable1"));
	Commands->Enable_Spawner(Get_Int_Parameter("ID2"),Get_Int_Parameter("Enable2"));
	Commands->Enable_Spawner(Get_Int_Parameter("ID3"),Get_Int_Parameter("Enable3"));
}

void z_UnkillableUntilEntered::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
	Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
}
void z_UnkillableUntilEntered::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Destroy_Script();
	}
}


void z_DestroyVeh259::Created(GameObject *obj)
{ 
	Commands->Start_Timer(obj,this,259,1);
}
void z_DestroyVeh259::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Destroy_Script();
	}
}
void z_DestroyVeh259::Timer_Expired(GameObject *obj, int number)
{ 
	if (number == 1)
	{
		Commands->Apply_Damage(obj,2500,"Death",0);
	}
}

void z_VehExit::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Attach_Script(obj,"z_DestroyVeh259","");
	}
}

void z_Spawn_When_Killed::Killed(GameObject *obj,GameObject *shooter)//TFX_Spawn_When_Killed, except this script will allow you to set the skintype, Health and Armor ammount instead of a percentage.
{
	float facing = Commands->Get_Facing(obj);
	float Health;
	float Armor;
	const char *skin;
	Vector3 position = Commands->Get_Position(obj);
	GameObject *newobj;
	position.Z += Get_Float_Parameter("DropHeight");
	newobj = Commands->Create_Object(Get_Parameter("PresetName"),position);
	skin = Get_Parameter("SkinType");
	Health = Get_Float_Parameter("Health");
	Armor = Get_Float_Parameter("Armor");
	Set_Skin(newobj,skin);
	Commands->Set_Health(newobj,Health);
	Commands->Set_Shield_Strength(newobj,Armor);
	if (!Get_Int_Parameter("SameFacing")) return;
	Commands->Set_Facing(newobj,facing);
}

void z_Remove_Script_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	const char *script;
	script = Get_Parameter("Script");
	if (message == Get_Int_Parameter("Message"))
	{
		Remove_Script(obj,script);
	}
}

void z_Teleport_Powerup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Vector3 spawn_position;
		spawn_position = Get_Vector3_Parameter("Location");
		int x = Get_Int_Parameter("Object_ID");
		if (x)
		{
			GameObject *gotoObject = Commands->Find_Object(x);
			Vector3 gotoLocation = Commands->Get_Position(gotoObject);
			Commands->Set_Position(sender,gotoLocation);
		}
		else
		{
			Commands->Set_Position(sender,spawn_position);
		}
	}
}

void z_blamo4sec::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,4.0f,111);
	{
		Commands->Set_Shield_Type(obj,"blamo");
	}
}
void z_blamo4sec::Timer_Expired(GameObject *obj, int number) 
{ 
	if (number == 111) 
	{
		Commands->Set_Shield_Type(obj,"ShieldKevlar");
		Destroy_Script();
	}
}

void z_ChangeTeamPowerup::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		char pick[256];
		char cht[200];
		if (Commands->Get_Player_Type(sender) == 1)
		{
			sprintf(cht,"team2 %d 0",Get_Player_ID(sender));
			const char *c = Get_Player_Name(sender);
			sprintf(pick,"msg %s team was changed to NOD by a powerup.",c);
			delete[] c;
		}
		else if (Commands->Get_Player_Type(sender) == 0)
		{
			sprintf(cht,"team2 %d 1",Get_Player_ID(sender));
			const char *c = Get_Player_Name(sender);
			sprintf(pick,"msg %s team was changed to GDI by a powerup.",c);
			delete[] c;
		}
		Console_Input(pick); Console_Input(cht);
	}
}



/* A PATH BEYOND RELATED SCRIPTS */



/*********************************************************************************************/

void z_Hostage::Created(GameObject *obj)
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
	PokerTeam = Get_Int_Parameter("PokerTeam");
	HostageID = Commands->Get_ID(obj);
	HostageHealth = Get_Float_Parameter("Hostage_Health");	
	HostageShield = Get_Float_Parameter("Hostage_Shield");
	Set_Max_Health(obj,HostageHealth);
	Set_Max_Shield_Strength(obj,HostageShield);
	Commands->Set_Shield_Strength(obj,HostageShield);	
	Commands->Set_Health(obj,HostageHealth);
}

void z_Hostage::Poked(GameObject *obj,GameObject *poker)
{
	if ((AllowPoke) && (Commands->Get_Player_Type(poker) == PokerTeam))
	{
		Pokerguy = poker;
		FollowID = Commands->Get_ID(poker);
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Commands->Set_Player_Type(obj,Get_Int_Parameter("Hostage_Team"));
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

void z_Hostage::Timer_Expired(GameObject *obj, int number)
{
	if (number == 789)//this is the number we have been waiting for
	{
		if (Commands->Find_Object(FollowID))//w00t we found our hero to the rescue
		{
			GameObject *GotoObject = Commands->Find_Object(FollowID);
			ActionParamsStruct params;
			params.Set_Basic(this,100,100);
			params.Set_Movement(GotoObject,1.0f,1.5f);
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
	else if (number == 788)
	{
		float hlth = Commands->Get_Health(obj);
		hlth += Get_Float_Parameter("Health_Regen_Ammount");
		float armr = Commands->Get_Shield_Strength(obj);
		armr += Get_Float_Parameter("Health_Regen_Ammount");
		if (hlth > Commands->Get_Max_Health(obj))
		{
			hlth = Commands->Get_Max_Health(obj);
		}
		if (armr > Commands->Get_Max_Shield_Strength(obj))
		{
			armr = Commands->Get_Max_Shield_Strength(obj);
		}
		Commands->Set_Health(obj,hlth);
		Commands->Set_Shield_Strength(obj,armr);
		Commands->Start_Timer(obj,this,1.0f,788);
	}
}


void z_Hostage::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Rescue_Message"))
	{
		if (param == 1)
		{
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("Controller_ID")),Get_Int_Parameter("Rescue_Message"),1,0.0f);//lets tell the controller a hostage was rescued
			Commands->Give_Money(Pokerguy,2500.0f,0);
			Commands->Give_Points(Pokerguy,1000.0f,0);
			Commands->Destroy_Object(obj);//destroy hostage, rescued, good work :P
			char resmsg[250];
			const char *str = Get_Player_Name(Pokerguy);
			sprintf(resmsg,"msg %s rescued a hostage.",str);
			delete[] str;
			Console_Input(resmsg);
		}
	}
}

void z_Hostage::Killed(GameObject *obj,GameObject *shooter)
{
	if (Commands->Find_Object(Get_Int_Parameter("Controller_ID")))
	{
		if (Commands->Is_A_Star(shooter))
		{
			Commands->Give_Money(shooter,-1000.0f,0);
			Commands->Give_Points(shooter,-1000.0f,0);
			char killmsg[250];
			const char *str = Get_Player_Name(shooter);
			const char *str2 = Get_Player_Name(shooter);
			sprintf(killmsg,"msg %s killed a hostage. %s try not to kill hostages, it may get you kicked.",str,str2);
			delete[] str2;
			delete[] str;
			Console_Input(killmsg);
		}
		Commands->Send_Custom_Event(obj, Commands->Find_Object(Get_Int_Parameter("Controller_ID")),Get_Int_Parameter("HostageDead_Message"),1,0.0);//hosty died, tell controller to increment deaths
		Destroy_Script();
	}
}

/*********************************************************************************************/

void z_Hostage_Controller::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,10.0f,800);
	Commands->Set_Player_Type(obj,-2);
	Set_Damage_Points(obj,0.0f);
	Attach_Script_Once(obj,"M00_Damage_Modifier_DME","0.00,1,1,0,0");
	Commands->Enable_Vehicle_Transitions(obj,false);
	Commands->Enable_HUD_Pokable_Indicator(obj,false);
	hostagerescues = 0;
	hostagedeaths = 0;
}

void z_Hostage_Controller::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Rescue_Message"))
	{
		if (param == 1)
		{
			hostagerescues++;
			Commands->Create_2D_WAV_Sound("rescued.wav");
			Send_Message(255,255,255,"A hostage has been rescued!");
		}
	}
	else if (message == Get_Int_Parameter("HostageDead_Message"))
	{
		if (param == 1)
		{
			hostagedeaths++;
			Commands->Create_2D_WAV_Sound("hosdown.wav");
			Send_Message(255,255,255,"Hostage down!");
		}
	}
	if (hostagerescues == Get_Int_Parameter("Required_Rescues"))
	{
		hostagerescues = 0;
		Commands->Create_2D_WAV_Sound("rescued.wav");
		int hostyteam = Get_Int_Parameter("Hostage_Team");
		if (hostyteam == 0)
		{
			Commands->Start_Timer(obj,this,3.0f,787);
			hostagerescues = 0;
		}
		else if (hostyteam == 1)
		{
			Commands->Start_Timer(obj,this,3.0f,786);
			hostagerescues = 0;
		}
	}
	if (hostagedeaths == Get_Int_Parameter("Max_Hosty_Deaths"))
	{
		int hostyteam = Get_Int_Parameter("Hostage_Team");
		if (hostyteam == 0)
		{
			Commands->Start_Timer(obj,this,3.0f,785);
			hostagedeaths = 0;
		}
		else if (hostyteam == 1)
		{
			Commands->Start_Timer(obj,this,3.0f,784);
			hostagedeaths = 0;
		}
	}
}

void z_Hostage_Controller::Timer_Expired(GameObject *obj, int number)
{
	if (number == 787)
	{
		Console_Input("win 0");
		Console_Input("win 0");
		Console_Input("win 0");
		Console_Input("amsg Good work Soviets: Mission Accomplished! All hostages were rescued!");
	}
	else if (number == 786)
	{
		Console_Input("win 1");
		Console_Input("win 1");
		Console_Input("win 1");
		Console_Input("amsg Good work Allies: Mission Accomplished! All hostages were rescued!");
	}
	if (number == 785)
	{
		Console_Input("win 1");
		Console_Input("win 1");
		Console_Input("win 1");
		Console_Input("amsg All hostages were terminated, your mission failed Soviets!");
	}
	else if (number == 784)
	{
		Console_Input("win 0");
		Console_Input("win 0");
		Console_Input("win 0");
		Console_Input("amsg All hostages were terminated, your mission failed Allies!");
	}
	else if (number == 800)
	{
		char msg[200]; char msg2[200]; char msg3[200];
		sprintf(msg2,"msg This map requires %d number of rescues or %d number of hostage deaths.",Get_Int_Parameter("Required_Rescues"),Get_Int_Parameter("Max_Hosty_Deaths"));
		sprintf(msg,"msg Number of hostage rescues so far: %d.",hostagerescues);
		sprintf(msg3,"msg Number of hostage deaths so far: %d.",hostagedeaths);
		Console_Input(msg2);Console_Input(msg);Console_Input(msg3);
		Commands->Start_Timer(obj,this,61.0f,800);
	}
}

/*********************************************************************************************/

void z_Hostage_Rescue_Zone::Entered(GameObject *obj,GameObject *enter)
{
	int team = Get_Int_Parameter("Hostage_Team");
	if ((Commands->Get_Player_Type(enter) == team) && (!Commands->Is_A_Star(enter)))
	{
		if ((strcmp(Commands->Get_Preset_Name(enter),(Get_Parameter("Hostage_Preset"))) == 0) ||
		(strcmp(Commands->Get_Preset_Name(enter),(Get_Parameter("Hostage_Preset2"))) == 0) ||
		(strcmp(Commands->Get_Preset_Name(enter),(Get_Parameter("Hostage_Preset3"))) == 0) ||
		(strcmp(Commands->Get_Preset_Name(enter),(Get_Parameter("Hostage_Preset4"))) == 0) ||
		(strcmp(Commands->Get_Preset_Name(enter),(Get_Parameter("Hostage_Preset5"))) == 0))
		{
			Commands->Send_Custom_Event(obj,enter,(Get_Int_Parameter("Rescue_Message")),1,0.0f);
		}
	}
}

/******************************************************************************************/

void z_Created_Timer_Enable_Spawner::Created(GameObject *obj)
{
	time = Get_Float_Parameter("Send_After");
	num = Get_Int_Parameter("TimerNumber");
	Commands->Start_Timer(obj,this,time,num);
}

void z_Created_Timer_Enable_Spawner::Timer_Expired(GameObject *obj, int number)
{
	if (number == num)
	{
		char snd[100];
		sprintf(snd,"snda %s",Get_Parameter("Play_Sound"));
		Console_Input(snd);
		Commands->Enable_Spawner(Get_Int_Parameter("ID1"),Get_Int_Parameter("Enable"));
		Commands->Enable_Spawner(Get_Int_Parameter("ID2"),Get_Int_Parameter("Enable"));
		Commands->Enable_Spawner(Get_Int_Parameter("ID3"),Get_Int_Parameter("Enable"));
	}
}


/******************************************************************************************/

void z_Goto_Star_Custom::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		if (param == 1)
		{
			Commands->Attach_Script(obj,"M03_Goto_Star","0");
		}
	}
}

/******************************************************************************************/

void z_Enable_Spawner_Custom::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		Commands->Enable_Spawner(Get_Int_Parameter("ID"),Get_Int_Parameter("Enable"));
	}
}

/******************************************************************************************/

void z_Enter_Enable_Spawner::Entered(GameObject *obj,GameObject *enter)
{
	int team = Get_Int_Parameter("Trigger_Team");
	if ((Commands->Get_Player_Type(enter) == team) && (!Commands->Is_A_Star(enter)))
	{
		Commands->Enable_Spawner(Get_Int_Parameter("ID1"),Get_Int_Parameter("Enable"));
		Commands->Enable_Spawner(Get_Int_Parameter("ID2"),Get_Int_Parameter("Enable"));
		Commands->Enable_Spawner(Get_Int_Parameter("ID3"),Get_Int_Parameter("Enable"));
	}
}

/******************************************************************************************/

void z_VehExit_DestroyVeh::Created(GameObject *obj)
{ 
	ID = 0;
	ownerisincar = false;
}
void z_VehExit_DestroyVeh::Custom(GameObject *obj, int message, int param, GameObject *sender){

	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		static bool entered;
		if(entered == false)
		{
			ID = Get_Player_ID(sender);
		}
		entered = true;
		if(Get_Player_ID(sender) == ID)
		{
			ownerisincar = true;
		}
	}
	else if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if(Get_Player_ID(sender) == ID)
		{
			Commands->Start_Timer(obj,this,180.0f,333);
			ownerisincar = false;
		}
	}
}

void z_VehExit_DestroyVeh::Timer_Expired(GameObject *obj, int number){ 
	if (number == 333){
		if (ownerisincar == true)
		{
			//nothing
		}
		else if (ownerisincar == false)
		{
			Commands->Destroy_Object(obj);
		}
	}
}

/******************************************************************************************/
void z_Death_Send_Custom::Killed(GameObject *obj,GameObject *shooter)//clone from jfw
{
	GameObject *object;
	int message;
	int ID;
	int param;
	ID = Get_Int_Parameter("ID");
	message = Get_Int_Parameter("Message");
	param = Get_Int_Parameter("Param");
	object = Commands->Find_Object(ID);
	Commands->Send_Custom_Event(obj,object,message,param,0);
}
/******************************************************************************************/
void z_Death_Send_Custom_Delay::Killed(GameObject *obj,GameObject *shooter)//clone from jfw
{
	GameObject *object;
	int message;
	int ID;
	int param;
	float delay = Get_Float_Parameter("Delay");
	ID = Get_Int_Parameter("ID");
	message = Get_Int_Parameter("Message");
	param = Get_Int_Parameter("Param");
	object = Commands->Find_Object(ID);
	Commands->Send_Custom_Event(obj,object,message,param,delay);
}

/******************************************************************************************/

void z_Custom_Send_Custom_Delay::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	int ID = Get_Int_Parameter("SendID");
	int sendmsg = Get_Int_Parameter("SendMessage");
	int sendparam = Get_Int_Parameter("SendParam");
	float time = Get_Float_Parameter("SendAfter");
	if (message == Get_Int_Parameter("Message"))
	{
		if (param == 1)
		{
			Commands->Send_Custom_Event(obj,Commands->Find_Object(ID),sendmsg,sendparam,time);
		}
	}
}

/******************************************************************************************/

void z_Start_Music_Delay::Created(GameObject *obj)
{
	time = Get_Float_Parameter("StartAfter");
	duration = Get_Float_Parameter("Duration");
	num = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,time,num);
}

void z_Start_Music_Delay::Timer_Expired(GameObject *obj, int number)
{
	if (number == num)
	{
		char mp3[200];
		sprintf(mp3,"snda %s",Get_Parameter("MP3"));
		Console_Input(mp3);
		Commands->Start_Timer(obj,this,duration,832);
	}
	else if (number == 832)
	{
		char mp32[200];
		sprintf(mp32,"snda %s",Get_Parameter("MP3"));
		Console_Input(mp32);
		Commands->Start_Timer(obj,this,duration,832);
	}

}


void z_Engineer_Repair::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0f,943);
}

void z_Engineer_Repair::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	int msg = Get_Int_Parameter("Message");
	int prm = Get_Int_Parameter("Param");
	if (message == msg)
	{
		if (param == prm)
		{
			char norepair[250];
			sprintf(norepair,"%s",Get_Parameter("No_Repair_MSG"));
			Send_Message(255,255,255,norepair);
			Destroy_Script();
		}
	}
}

void z_Engineer_Repair::Timer_Expired(GameObject *obj, int number)
{
	if (number == 943)
	{
		if (Commands->Get_Health(obj) < 100.0f)
		{
			Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
			Commands->Start_Timer(obj,this,1.0f,943);
		}
		else
		{
			Commands->Start_Timer(obj,this,1.0f,943);
		}
	}
}
		
/******************************************************************************************/
void z_Custom_Multiple_Send_Custom::Created(GameObject *obj)
{
	times_recieved = 0;
	enabled = true;
}

void z_Custom_Multiple_Send_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (enabled)
	{
		if (message == Get_Int_Parameter("RecieveMessage"))
		{
			times_recieved++;
		}
		if (times_recieved == Get_Int_Parameter("RecieveCount"))
		{
			enabled = false;
			Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendID")),Get_Int_Parameter("SendMessage"),Get_Int_Parameter("SendParam"),0);
		}
	}
}

/******************************************************************************************/
void z_Buy_Longbow::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	longbows = 0;
	allowbuy = true;
}

void z_Buy_Longbow::Poked(GameObject *obj, GameObject *poker)
{
	if (allowbuy)
	{ 
		if (longbows < Get_Int_Parameter("Max"))
		{
			if (Commands->Get_Money(poker) >= Get_Float_Parameter("Cost"))
			{
				Vector3 pos = Get_Vector3_Parameter("Loc");
				float facing = Get_Float_Parameter("Facing");
				GameObject *lb = Commands->Create_Object("CnC_Allied_Apache",pos);
				Commands->Set_Facing(lb,facing);

				if (strcmp(The_Game()->Get_Map_Name(),"RA_Soviet_Siege_Coop.mix") == 0)
				{
					Attach_Script_Once(lb,"z_Kill_LB_Exit","");
				}

				char prms[100];
				sprintf(prms,"%d,%d,%d",Commands->Get_ID(obj),1000,1);
				Commands->Attach_Script(lb,"z_Death_Send_Custom",prms);
				
				Commands->Give_Money(poker, -1200.0f,0);
				
				char pmsg[250];
				const char *str = Get_Player_Name(poker);
				sprintf(pmsg,"msg %s bought an Apache Longbow.",str);
				delete[] str;
				Console_Input(pmsg);

				longbows++;
			}
			else if (Commands->Get_Money(poker) < Get_Float_Parameter("Cost"))
			{
				char nomoney[250];
				sprintf(nomoney,"ppage %d You have insufficient funds. An Apache Longbow costs 1200.",Get_Player_ID(poker));
				Console_Input(nomoney);
			}
		}
		else 
		{
			char max[250];
			sprintf(max,"ppage %d Sorry! The maximum number of Apache Longbows is %d",Get_Player_ID(poker),Get_Int_Parameter("Max"));
			Console_Input(max);
		}
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		Commands->Start_Timer(obj,this,15.0f,123);
		allowbuy = false;
	}
}

void z_Buy_Longbow::Timer_Expired(GameObject *obj, int number)
{
	if (number == 123)
	{
		allowbuy = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}

void z_Buy_Longbow::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			longbows--;
		}
	}
}

/******************************************************************************************/

//START COOP POWERUPS
void z_Pup_Allied_Tanya::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	int RandomNum = Commands->Get_Random_Int(1,3);
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		char snd[100];
		Change_Character(sender,"Allied_Tanya");
		if (RandomNum == 1)
		{
			sprintf(snd,"snda tanya-yes.wav");
		}
		else if (RandomNum == 2)
		{
			sprintf(snd,"snda tanya-yeah.wav");
		}
		else if (RandomNum == 3)
		{
			sprintf(snd,"snda tanya-yo.wav");
		}
		Console_Input(snd);
	}
}

void z_Pup_Allied_RocketGuy::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"Allied_Rocket_Soldier");
	}
}

void z_Pup_Allied_Medic::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"Allied_Medic");
	}
}

void z_Pup_Allied_Medic_Coop::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"Allied_Medic_Coop");
	}
}

void z_Pup_Allied_Sniper::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"Allied_Sniper");
	}
}
void z_Pup_Allied_Spy::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"Allied_Spy_Coop");
	}
}
void z_Pup_Allied_Officer::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"Allied_Captain");
	}
}

void z_Pup_Allied_Mechanic::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"Allied_Mechanic");
	}
}

void z_Pup_Allied_Sniper_Coop::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Change_Character(sender,"Allied_Sniper_Coop");
	}
}

void z_TanyaVolkovAnnounce::Created(GameObject *obj)
{
	if ((strcmp(Commands->Get_Preset_Name(obj),"Allied_Tanya") == 0) ||
	(strcmp(Commands->Get_Preset_Name(obj),"Allied_Tanya_Forest") == 0) ||
	(strcmp(Commands->Get_Preset_Name(obj),"Allied_Tanya_Snow") == 0))
	{
		Console_Input("snda tanya3.wav");
		Console_Input("msg playing tanya");
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"Soviet_Volkov") == 0)
	{
		Console_Input("snda sov_ready1.wav");
		Console_Input("msg playing volkov");
	}
}

	
//END COOP POWERUPS
/******************************************************************************************/

void z_Purchase_Pillbox::Created(GameObject *obj)
{
	funds = 0;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}

void z_Purchase_Pillbox::Poked(GameObject *obj, GameObject *poker)
{
	if (funds < Get_Float_Parameter("Cost"))
	{
		if (Commands->Get_Money(poker) >= 500.0f)
		{
			funds = funds + 500.0f;
			char fundadded[250];
			const char *str = Get_Player_Name(poker);
			sprintf(fundadded,"%s added $500 to the %s Pillbox fundpool. (Current: %g / %g)",str,Get_Parameter("Direction"),funds,Get_Float_Parameter("Cost"));
			delete[] str;
			Send_Message(255,255,255,fundadded);
			Commands->Give_Money(poker,-500.0f,false);
		}
		else if (Commands->Get_Money(poker) < 500.0f)
		{
			char nomoney[100]; 
			sprintf(nomoney,"ppage %d Sorry you do not have enough money ($500) to add to the fundpool.",Get_Player_ID(poker));
			Console_Input(nomoney);
		}

		if (funds == Get_Float_Parameter("Cost"))
		{
			Commands->Create_2D_WAV_Sound("building.wav");
			Commands->Start_Timer(obj,this,3.0f,166);
		}
	}
}

void z_Purchase_Pillbox::Timer_Expired(GameObject *obj, int number)
{
	if (number == 166)
	{
		Vector3 pos = Get_Vector3_Parameter("Loc");
		float facing = Get_Float_Parameter("Facing");
		GameObject *pb = Commands->Create_Object(Get_Parameter("Preset"),pos);
		Commands->Set_Facing(pb,facing);
		char msg[250];
		sprintf(msg,"The %s Pillbox is now operational.",Get_Parameter("Direction"));
		Send_Message(255,255,255,msg);
		Commands->Create_2D_WAV_Sound("buildingcomplete.wav");
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100193),100,1,0);
		Commands->Destroy_Object(obj);
	}
}

void z_Purchase_Turret::Created(GameObject *obj)
{
	funds = 0;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}

void z_Purchase_Turret::Poked(GameObject *obj, GameObject *poker)
{
	if (funds < Get_Float_Parameter("Cost"))
	{
		if (Commands->Get_Money(poker) >= 500.0f)
		{
			funds = funds + 500.0f;
			char fundadded[250];
			const char *str = Get_Player_Name(poker);
			sprintf(fundadded,"%s added $500 to the %s Turret fundpool. (Current: %g / %g)",str,Get_Parameter("Direction"),funds,Get_Float_Parameter("Cost"));
			delete[] str;
			Send_Message(255,255,255,fundadded);
			Commands->Give_Money(poker,-500.0f,false);
		}
		else if (Commands->Get_Money(poker) < 500.0f)
		{
			char nomoney[100]; 
			sprintf(nomoney,"ppage %d Sorry you do not have enough money ($500) to add to the fundpool.",Get_Player_ID(poker));
			Console_Input(nomoney);
		}

		if (funds == Get_Float_Parameter("Cost"))
		{
			Commands->Create_2D_WAV_Sound("building.wav");
			Commands->Start_Timer(obj,this,3.0f,166);
		}
	}
}

void z_Purchase_Turret::Timer_Expired(GameObject *obj, int number)
{
	if (number == 166)
	{
		Vector3 pos = Get_Vector3_Parameter("Loc");
		float facing = Get_Float_Parameter("Facing");
		GameObject *pb = Commands->Create_Object(Get_Parameter("Preset"),pos);
		Commands->Set_Facing(pb,facing);
		char msg[250];
		sprintf(msg,"The %s Turret is now operational.",Get_Parameter("Direction"));
		Send_Message(255,255,255,msg);
		Commands->Create_2D_WAV_Sound("buildingcomplete.wav");
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100193),100,1,0);
		Commands->Destroy_Object(obj);
	}
}

/****************************************************************************************************/
void z_Cinematic_Attack::Created(GameObject *obj)
{
	ActionParamsStruct params;
	int priority = Get_Int_Parameter("Priority");
	float range = Get_Float_Parameter("Range");
	float deviation = Get_Float_Parameter("Deviation");
	int primary = Get_Int_Parameter("Primary");
	GameObject *GotoObject = Commands->Find_Object(Get_Int_Parameter("ID"));
	params.Set_Movement(GotoObject,10.0f,50.0f);
	params.MoveFollow = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,(float)priority,40016);
	params.Set_Attack(Commands->Find_Object(Get_Int_Parameter("ID")),range,deviation,primary);
	params.AttackCheckBlocked = false;
	Commands->Action_Attack(obj,params);
}

void z_Cinematic_Attack::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	Commands->Action_Reset(obj,100);
}

void z_Attack_MCV_SIC::Created(GameObject *obj)
{
	Attach_Script_Once(obj,"z_Cinematic_Attack","100088,100,1000.00,0.00,1");
}

/****************************************************************************************************/

void z_Airraid_Custom::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		if (param == 1)
		{
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");

			Commands->Start_Timer(obj,this,3.0f,100);
			Commands->Start_Timer(obj,this,5.0f,101);
			Commands->Start_Timer(obj,this,7.0f,102);
			Commands->Start_Timer(obj,this,8.5f,103);
		}
	}
}

void z_Airraid_Custom::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	}
	else if (number == 101)
	{
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	}
	else if (number == 102)
	{
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	}
	else if (number == 103)
	{
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
			Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	}
}

/****************************************************************************************************/

void z_Created_Timer_EnableSpawners::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("StartAfter"),Get_Int_Parameter("TimerNumber"));
}

void z_Created_Timer_EnableSpawners::Timer_Expired(GameObject *obj, int number)
{
	if (number == Get_Int_Parameter("TimerNumber"))
	{
		Commands->Enable_Spawner(Get_Int_Parameter("ID1"),Get_Int_Parameter("Enable"));
		Commands->Enable_Spawner(Get_Int_Parameter("ID2"),Get_Int_Parameter("Enable"));
		Commands->Enable_Spawner(Get_Int_Parameter("ID3"),Get_Int_Parameter("Enable"));
		Commands->Enable_Spawner(Get_Int_Parameter("ID4"),Get_Int_Parameter("Enable"));
		Commands->Enable_Spawner(Get_Int_Parameter("ID5"),Get_Int_Parameter("Enable"));
	}
}
/****************************************************************************************************/

void z_Cinematic_Attack2::Created(GameObject *obj)
{
	ActionParamsStruct params;
	int priority = 100;
	float range = 1000.0f;
	float deviation = 0.0f;
	int primary = 1;
	GameObject *GotoObject = Commands->Find_Object(Get_Int_Parameter("ID"));
	params.Set_Movement(GotoObject,10.0f,50.0f);
	params.MoveFollow = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,(float)priority,40016);
	params.Set_Attack(Commands->Find_Object(Get_Int_Parameter("ID")),range,deviation,primary);
	params.AttackCheckBlocked = false;
	Commands->Action_Attack(obj,params);
	Commands->Start_Timer(obj,this,3.0f,250);
}

void z_Cinematic_Attack2::Timer_Expired(GameObject *obj, int number)
{
	if (number == 250)
	{
		ActionParamsStruct params;
		int priority = 100;
		float range = 1000.0f;
		float deviation = 0.0f;
		int primary = 1;
		GameObject *GotoObject = Commands->Find_Object(Get_Int_Parameter("ID"));
		params.Set_Movement(GotoObject,10.0f,50.0f);
		params.MoveFollow = true;
		Commands->Action_Goto(obj,params);
		params.Set_Basic(this,(float)priority,40016);
		params.Set_Attack(Commands->Find_Object(Get_Int_Parameter("ID")),range,deviation,primary);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
		Commands->Start_Timer(obj,this,3.0f,250);
	}
}

void z_Cinematic_Attack2::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	Commands->Action_Reset(obj,100);
}




void z_Cinematic_Attack3::Created(GameObject *obj)
{
	ActionParamsStruct params;
	int priority = 100;
	float range = 1000.0f;
	float deviation = 0.0f;
	int primary = 1;
	GameObject *GotoObject = Commands->Find_Object(Get_Int_Parameter("Move_ID"));
	params.Set_Movement(GotoObject,10.0f,50.0f);
	params.MoveFollow = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,(float)priority,40016);
	params.Set_Attack(Commands->Find_Object(Get_Int_Parameter("Attack_ID")),range,deviation,primary);
	params.AttackCheckBlocked = false;
	Commands->Action_Attack(obj,params);
	Commands->Start_Timer(obj,this,3.0f,250);
}

void z_Cinematic_Attack3::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	Commands->Action_Reset(obj,100);
}



void z_MissionStart_SIC::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,3060.0f,101);//start last invasion//bigass tank and infantry rush to destroy the allied base
	Commands->Start_Timer(obj,this,3000.0f,102);//1 min remaining//mcv deploys
	Commands->Start_Timer(obj,this,2760.0f,103);//5 min remaining//enable some more infantry "goto star"
	Commands->Start_Timer(obj,this,2160.0f,104);//10 min remaining//second attack tanks
	Commands->Start_Timer(obj,this,1560.0f,105);//20 min remaining//enable bots in the area + attack their oretruck
	Commands->Start_Timer(obj,this,960.0f,106);//30 min remaining//first attack tanks
	Commands->Start_Timer(obj,this,60.0f,107);//40 min remaining
	Commands->Start_Timer(obj,this,30.0f,100);//initialize timer msg, msg to build defenses
}

void z_MissionStart_SIC::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)//initailize mission and msg build defenses
	{
		//
		Send_Message(255,255,255,"Mission timer initialized.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Commands->Create_2D_WAV_Sound("missiontimer.wav");
		Send_Message(255,255,255,"You have ten minutes to build your base defenses.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
	}
	//timers and their msg/sound
	else if (number == 101)//start last invasion
	{
		Send_Message(255,255,255,"Soviet forces approaching.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Commands->Create_2D_WAV_Sound("sovforcesapp.wav");
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100086),1000,1,0);
	}
	else if (number == 102)//1min rem
	{
		Send_Message(255,255,255,"Warning, one minute remaining.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Commands->Create_2D_WAV_Sound("1mr.wav");
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100086),1000,1,0);//tell mcv to deploy

		Commands->Enable_Spawner(100228,1);
		Commands->Enable_Spawner(100230,1);
		Commands->Enable_Spawner(100231,1);
		//Commands->Enable_Spawner(100229,1);
		Commands->Enable_Spawner(100231,1);
		//Commands->Enable_Spawner(100230,1);
		Commands->Enable_Spawner(100232,1);
		//Commands->Enable_Spawner(100233,1);
		Commands->Enable_Spawner(100234,1);
		
		Commands->Start_Timer(obj,this,60.0f,855);//end the game

	}
	else if (number == 855)
	{
		Commands->Start_Timer(obj,this,4.0f,856);
		Console_Input("amsg Mission Accomplished.");
		Console_Input("win 1");
		Console_Input("win 1");
	}
	else if (number == 856)
	{
		Commands->Create_2D_WAV_Sound("we_owned_them.wav");
	}
	else if (number == 103)//5min
	{
		Send_Message(255,255,255,"Warning, five minutes remaining.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Commands->Create_2D_WAV_Sound("5mr.wav");

		//enable 4 more tanks
		Commands->Enable_Spawner(100249,1);
		//Commands->Enable_Spawner(100250,1);
		//Commands->Enable_Spawner(100251,1);
		//Commands->Enable_Spawner(100252,1);

	}
	else if (number == 104)//10min
	{
		Send_Message(255,255,255,"Ten minutes remaining.");
		Commands->Create_2D_WAV_Sound("snda incmsg.wav");
		Commands->Create_2D_WAV_Sound("snda 10mr.wav");
		Commands->Enable_Spawner(100199,1);
		Commands->Enable_Spawner(100200,1);
		Commands->Enable_Spawner(100201,1);
		Commands->Enable_Spawner(100203,1);
	}
	else if (number == 105)//20min//enable bots in the area + attack their oretruck
	{
		Send_Message(255,255,255,"Twenty minutes remaining.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Commands->Create_2D_WAV_Sound("20mr.wav");
		Commands->Enable_Spawner(100236,1);
		//Commands->Enable_Spawner(100237,1);
		Commands->Enable_Spawner(100238,1);
		Commands->Enable_Spawner(100239,1);
		//Commands->Enable_Spawner(100241,1);
		Commands->Enable_Spawner(100242,1);
		Commands->Enable_Spawner(100243,1);
		Commands->Enable_Spawner(100245,1);
		//Commands->Enable_Spawner(100244,1);
		Commands->Enable_Spawner(100248,1);
	}
	else if (number == 106)//30min//first attack tanks
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100225),1000,1,0);
		Commands->Send_Custom_Event(obj,Commands->Find_Object(100259),1000,1,0);
		Send_Message(255,255,255,"Thirty minutes remaining.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Commands->Create_2D_WAV_Sound("30mr.wav");
		Commands->Enable_Spawner(100111,1);
		Commands->Enable_Spawner(100114,1);
		Commands->Enable_Spawner(100113,1);
		Commands->Enable_Spawner(100162,1);
		Commands->Enable_Spawner(100115,1);
		Commands->Enable_Spawner(100165,1);
		Commands->Enable_Spawner(100166,1);
		Commands->Enable_Spawner(100169,1);
		Commands->Enable_Spawner(100180,1);
		Commands->Enable_Spawner(100181,1);
		Commands->Enable_Spawner(100184,1);
		Commands->Enable_Spawner(100185,1);
		Commands->Enable_Spawner(100188,1);
		Commands->Enable_Spawner(100191,1);
	}
	else if (number == 107)//40min//enable some botspawners attacking players and the ore truck
	{
		Send_Message(255,255,255,"Forty minutes remaining.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Commands->Create_2D_WAV_Sound("40mr.wav");

		Commands->Enable_Spawner(100110,1);

		
	}
	else if (number == 108)//final invasion started
	{
		//set model arrow WF
		//new construction options
		//players can buy tanks to counter last attack
	}
}

/****************************************************************************************************/

void z_Allied_MCV_Killed::Created(GameObject *obj)
{
	Attach_Script_Once(obj,"z_MCV_Points","4.0");
}

void z_Allied_MCV_Killed::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Create_2D_WAV_Sound("missionfailed.wav");
	Send_Message(255,255,255,"Your mission was a failure.");
	Send_Message(255,255,255,"Your mission was a failure.");
	Send_Message(255,255,255,"Your mission was a failure.");
	Commands->Send_Custom_Event(obj,Commands->Find_Object(100201),1000,1,0); //Start Mammoth invasion destroying all allied buildings

	
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");

	Commands->Start_Timer(obj,this,3.0f,100);
	Commands->Start_Timer(obj,this,5.0f,101);
	Commands->Start_Timer(obj,this,7.0f,102);
	Commands->Start_Timer(obj,this,8.5f,103);
	Commands->Start_Timer(obj,this,10.0f,104);
	Commands->Start_Timer(obj,this,12.5f,105);
	Commands->Start_Timer(obj,this,15.0f,106);


	//invulnerable mammies to finish off allied base
	Commands->Enable_Spawner(100204,1);
	Commands->Enable_Spawner(100205,1);
	Commands->Enable_Spawner(100206,1);
	Commands->Enable_Spawner(100213,1);
	Commands->Enable_Spawner(100215,1);
	Commands->Enable_Spawner(100214,1);
	Commands->Enable_Spawner(100210,1);
	Commands->Enable_Spawner(100212,1);
	Commands->Enable_Spawner(100211,1);
	Commands->Enable_Spawner(100222,1);
	Commands->Enable_Spawner(100224,1);
	Commands->Enable_Spawner(100223,1);
}

void z_Allied_MCV_Killed::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	}
	else if (number == 101)
	{
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	}
	else if (number == 102)
	{
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	}
	else if (number == 103)
	{
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	}
	else if (number == 104)
	{
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	}
	else if (number == 105)
	{
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	}
	else if (number == 106)
	{
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	Commands->Create_2D_WAV_Sound("amb_airraid.wav");
	}
}

/****************************************************************************************************/

void z_MCV_Deploy_SIC::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			ActionParamsStruct params;
			params.MoveArrivedDistance = 100;
			params.Set_Basic(this,100,777);
			params.Set_Movement(0,100.0,10);
			params.WaypathID = 100170;
			params.WaypathSplined = true;
			params.AttackActive = false;
			params.AttackCheckBlocked = false;
			Commands->Action_Goto(obj,params);
			Commands->Start_Timer(obj,this,7.5f,100);
		}
	}
}

void z_MCV_Deploy_SIC::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Create_2D_WAV_Sound("buildingcomplete.wav");
		Send_Message(255,255,255,"New construction options.");
		Commands->Create_2D_WAV_Sound("snda newco.wav");
		Commands->Set_Model(Commands->Find_Object(100225),"allied_cy");
		Commands->Set_Facing(Commands->Find_Object(100225),-180.0f);
		Commands->Destroy_Object(obj);
	}
}

void z_Soviet_Paradrop::Created(GameObject *obj)
{
	float timertime;
	int timernumber;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	Commands->Start_Timer(obj,this,timertime,timernumber);
}

void z_Soviet_Paradrop::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	int repeat;
	float timertime;
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	scriptname = Get_Parameter("Script_Name");
	position = Get_Vector3_Parameter("location");
	facing = Get_Float_Parameter("Facing");
	if (number == timernumber)
	{
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
		Console_Input("snda amb_airraid.wav");
		if (repeat == 1)
		{	
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}


void z_OT_Deliver_SIC::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			float timertime;
			int timernumber;
			timertime = Get_Float_Parameter("Time");
			timernumber = Get_Int_Parameter("TimerNum");
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}

void z_OT_Deliver_SIC::Timer_Expired(GameObject *obj,int number)
{
	int timernumber;
	int repeat;
	float timertime;
	const char *scriptname;
	Vector3 position;
	GameObject *object;
	float facing;
	timertime = Get_Float_Parameter("Time");
	timernumber = Get_Int_Parameter("TimerNum");
	repeat = Get_Int_Parameter("Repeat");
	scriptname = Get_Parameter("Script_Name");
	position = Get_Vector3_Parameter("location");
	facing = Get_Float_Parameter("Facing");
	if (number == timernumber)
	{
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
		Console_Input("msg Allied Ore Truck delivery boy is on its way.");
		if (repeat == 1)
		{	
			Commands->Start_Timer(obj,this,timertime,timernumber);
		}
	}
}




void z_Prototype_AllowEnter::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{ 
		if (Commands->Get_Player_Type(sender) == Get_Int_Parameter("Allow_Team"))
		{
			//nothing
		}
		else if (Commands->Get_Player_Type(sender) != Get_Int_Parameter("Allow_Team"))
		{
			Commands->Start_Timer(obj,this,0.1f,100);
			char pm[200];
			sprintf(pm,"pamsg %d You are not allowed to enter the Prototype Vehicle.",Get_Player_ID(sender));
			Console_Input(pm);
		}
	}
}

void z_Prototype_AllowEnter::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Force_Occupant_Exit(obj,0);
	}
}
		

void z_Prototype::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{ 
		if (Commands->Get_Player_Type(sender) == Get_Int_Parameter("Allow_Team"))
		{
			Commands->Start_Timer(obj,this,1.0f,100);
			char stealmsg[250];
			const char *str = Get_Player_Name(sender);
			sprintf(stealmsg,"%s has stolen the prototype.",str);
			delete[] str;
			Send_Message(243,10,246,stealmsg);
		}
		if (Commands->Get_Player_Type(sender) != Get_Int_Parameter("Allow_Team"))
		{
			Commands->Start_Timer(obj,this,0.5f,101);
			char pm[200];
			sprintf(pm,"pamsg %d You are not allowed to enter the Prototype Vehicle.",Get_Player_ID(sender));
			Console_Input(pm);
		}
	}
}

void z_Prototype::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Force_Occupant_Exit(obj,0);
	}
}


void z_Prototype_Zone::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Timelimit"),200);
	proto = 0;
}

void z_Prototype_Zone::Entered(GameObject *obj, GameObject *enter)
{
	if (strcmp(Commands->Get_Preset_Name(enter),(Get_Parameter("Prototype"))) == 0)
	{
		proto = Commands->Get_ID(enter);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("ResearchTime"),100);
		char res[250];
		sprintf(res,"The %s are now reverse engineering the %s prototype.",Get_Parameter("ResearchTeamName"),Get_Parameter("Prototype_Friendly_Name"));
		Send_Message(243,10,246,res);
	}
}
void z_Prototype_Zone::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		if (Commands->Find_Object(proto))
		{
			Commands->Enable_Spawner(Get_Int_Parameter("Spawner_1"),1);
			Commands->Enable_Spawner(Get_Int_Parameter("Spawner_2"),1);
			Commands->Enable_Spawner(Get_Int_Parameter("Spawner_3"),1);
			Commands->Enable_Spawner(Get_Int_Parameter("Spawner_4"),1);
			Commands->Enable_Spawner(Get_Int_Parameter("Spawner_5"),1);
			Commands->Enable_Spawner(Get_Int_Parameter("Spawner_6"),1);
			Commands->Enable_Spawner(Get_Int_Parameter("Spawner_7"),1);
			Commands->Enable_Spawner(Get_Int_Parameter("Spawner_8"),1);
			Commands->Enable_Spawner(Get_Int_Parameter("Spawner_9"),1);
			Commands->Enable_Spawner(Get_Int_Parameter("Spawner_10"),1);
			char rescomp[250];
			sprintf(rescomp,"The %s have successfully reverse engineered the %s",Get_Parameter("ResearchTeamName"),Get_Parameter("Prototype_Friendly_Name"));
			Send_Message(243,10,246,rescomp);
			Commands->Enable_Spawner(Get_Int_Parameter("ProtoSpawnerID"),0);
			Destroy_Script();
		}
		else
		{
			char fail[250];
			sprintf(fail,"The %s failed to reverse engineer the %s.",Get_Parameter("ResearchTeamName"),Get_Parameter("Prototype_Friendly_Name"));
			Send_Message(243,10,246,fail);
		}
	}
	else if (number == 200)
	{
		Kill_All_Buildings_By_Team(Get_Int_Parameter("LosingTeamTimeExpired"));
	}
}

void z_Pickup_Send_Custom::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	}
}

void z_MCV_Points::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Is_A_Star(damager)) //Ignore if not a player
	{
		int objtype1 = Commands->Get_Player_Type(obj);
		int objtype2 = Commands->Get_Player_Type(damager);
		if (!objtype1 || objtype1 == 1) //Ignore if not soviet or allied
		{
			if (objtype1 != objtype2) //Test if on different teams
			{
				if (damage > 0) //If 'damage' was harmful
				{
					float divisor = Get_Float_Parameter("Divisor");
					Commands->Give_Points(damager, damage/divisor, false);
				}
			}
			else if (objtype1 == objtype2) //Just to be sure...
			{
				if (damage < 0) //If 'damage' was helpful
				{
					float divisor = Get_Float_Parameter("Divisor");
					Commands->Give_Points(damager, (-damage)/divisor, false);
				}
			}
		}
	}
}

void z_Defense_Owner::Created(GameObject *obj)
{
	owner = Get_Int_Parameter("Owner_ID");
}

void z_Defense_Owner::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 2002)
	{
		if (param == 1)
		{
			//killmsg
		}
	}
}

void z_Powerdown_Crate::Created(GameObject *obj)
{
	Power_Base(Get_Int_Parameter("Team"),false);
	Commands->Start_Timer(obj,this,60.0f,100);
}
void z_Powerdown_Crate::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Power_Base(Get_Int_Parameter("Team"),true);
		Send_Message(25,255,255,"Base power was restored");
		Destroy_Script();
	}
}

void z_S_HP1_Zone::Entered(GameObject *obj, GameObject *enter)
{
	GameObject *hp = Commands->Find_Object(Get_Int_Parameter("S_HP1_ID"));
	if (Commands->Get_Health(hp) >= 1.0f)
	{
		if (Commands->Get_Player_Type(enter) == 0)
		{
			Commands->Start_Timer(obj,this,1.0,Commands->Get_ID(enter));
		}
	}
}

void z_S_HP1_Zone::Timer_Expired(GameObject *obj, int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	GameObject *object = Commands->Find_Object(number);
	if (!object)
	{
		return;
	}
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(object);
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= 10.0)
	{
		if (object->As_VehicleGameObj())
		{
			if (Get_Vehicle_Mode(object) == VEHICLE_TYPE_FLYING)
			{
				Commands->Give_PowerUp(object,"Hind_Reload",0);
				Commands->Start_Timer(obj, this, 2.0f, number);
			}
		}
	}
}

void z_S_HP2_Zone::Entered(GameObject *obj, GameObject *enter)
{
	GameObject *hp = Commands->Find_Object(Get_Int_Parameter("S_HP2_ID"));
	if (Commands->Get_Health(hp) >= 1.0f)
	{
		if (Commands->Get_Player_Type(enter) == 0)
		{
			Commands->Start_Timer(obj,this,1.0,Commands->Get_ID(enter));
		}
	}
}

void z_S_HP2_Zone::Timer_Expired(GameObject *obj, int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	GameObject *object = Commands->Find_Object(number);
	if (!object)
	{
		return;
	}
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(object);
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= 10.0)
	{
		if (object->As_VehicleGameObj())
		{
			if (Get_Vehicle_Mode(object) == VEHICLE_TYPE_FLYING)
			{
				Commands->Give_PowerUp(object,"Hind_Reload",0);
				Commands->Start_Timer(obj, this, 2.0f, number);
			}
		}
	}
}


void z_Vehicle_AI::Created(GameObject *obj)
{
	//Variable Initializing
	targetID = 0;
	position.X = 0;
	position.Y = 0;
	position.Z = 0;
	rampaging = false;
	guarding = false;
	moving = false;
	hunting = false;
//	move_priority = 90;
//	attack_priority = 100;
	face_priority = 1;
	current_priority = 10;
	guard_timer = 2;
	attack_timer = 2;
	mypos = Commands->Get_Position(obj);
	move_count = 0;

	//Standard AI Startup Junk
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
}

void z_Vehicle_AI::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	/***********************************************/
	//				Message List
	/***********************************************/
	//				Incoming
	//	127127: Order to hunt at location of sender
	//	781781: Order to hunt a new player (sender)
	//	329329: Order to guard location of sender
	//	983983: Order to guard a new player (sender)
	//	param will be used to determine order priority
	/***********************************************/

	if (!rampaging) //Ignore orders if beserk
	{
		//Maybe a Switch statement would be better?
		if (message == 127127)
		{
			position = Commands->Get_Position(sender);
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority,10);
			params.Set_Movement(position,1,2,false);
			params.MoveBackup = Set_Direction(obj);
			Commands->Action_Goto(obj,params);
			moving = true;

			//Offense code to be added here?
		}
		else if (message == 781781)
		{
			Hunt(obj,Set_Target(sender));
			//Offense code to be added here?
		}
		else if (message == 329329)
		{
			position = Commands->Get_Position(sender);
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority,10);
			params.Set_Movement(position,1,2,false);
			params.MoveBackup = Set_Direction(obj);
			Commands->Action_Goto(obj,params);

			moving = true;
			guarding = true;
			hunting = false;
		}
		else if (message == 983983)
		{
			Guard(obj,Set_Target(sender));
			//Might need more here...
		}
	}
}

void z_Vehicle_AI::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	/**********************************************/
	//				Action IDs
	/**********************************************/
	//	10: Goto action
	//	20: Attack action
	//	30: Facing action
	/**********************************************/

	if (action == 10)
	{
		moving = false;
		move_count = 0; //move completed
	}
}

void z_Vehicle_AI::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	//This function needs to be rewritten from scratch

	int testID = Commands->Get_Player_Type(damager);
	int myID = Commands->Get_Player_Type(obj);

	if (testID != myID) //Did the enemy hit me?
	{
		if (!hunting && !guarding) //Not doing anything?
		{
			Hunt(obj,Set_Target(damager));
		}
		else if (hunting) //On a hunt?
		{
			//Might not need the Hunt function here...
			Hunt(obj,Select_Target(obj,damager));
			//This determines if damager bigger threat than target
		}
	}
	else if (!hunting && !guarding) //Am I doing anything?
	{
		Guard(obj,Set_Target(damager)); //must be a call for help
	}
}

void z_Vehicle_AI::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (guarding)
	{
		Vector3 pos = Commands->Get_Position(seen);
		Vector3 mypos = Commands->Get_Position(obj);
		float distance = Commands->Get_Distance(pos,mypos);

		current_priority += 1; //override last command

		if (distance > Get_Float_Parameter("MaxRange"))
		{
			if (!moving)
			{
				ActionParamsStruct params;
				params.Set_Basic(this,(float)face_priority,30);
				params.Set_Face_Location(pos,5);
				Commands->Action_Face_Location(obj,params);
			}
		}
		else
		{
			//May need a 'Check_Target' that won't touch targetID
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority,20);
			params.Set_Attack(seen,distance,0,Set_Ammo(seen));
			params.AttackCheckBlocked = true;
			Commands->Action_Attack(obj,params);
		}

		return; // we're done here
	}

	//If I'm not guarding,might as well go hunting

	Hunt(obj,Select_Target(obj,seen));

}

void z_Vehicle_AI::Timer_Expired(GameObject *obj,int number)
{
	/***********************************************/
	//				Timer List
	/***********************************************/
	//	11: Guard Timer
	//	22: Hunt Timer
	/***********************************************/
	if (number == 11)
	{
		if (guarding) //in case guard was forced off
		{
			guarding = false; //guard will only continue if Guard called
			GameObject *target = Commands->Find_Object(targetID);

			if (target) //Check if the target lives
			{
				Guard(obj,Set_Target(target));
			}
			else //target dead
			{
				targetID = 0;
				current_priority = 10; //drop priority back down
			}
		}
	}
	else if (number == 22)
	{
		if (hunting) //In case hunt was forced off
		{
			hunting = false; //hunt will only continue if Hunt called
			GameObject *target = Commands->Find_Object(targetID);

			if (target) //Check if the target lives
				Hunt(obj,Set_Target(target));//Hunt starts a new timer by itself
			else
			{
				targetID = 0; //target dead,forget him
				current_priority = 10; //drop priority back down
			}
		}
	}
}

void z_Vehicle_AI::Sound_Heard(GameObject *obj,const CombatSound &sound)
{
	if (guarding)
	{
		if (!moving)
		{
			if (Commands->Is_A_Star(sound.Creator))
			{
				ActionParamsStruct params;
				params.Set_Basic(this,(float)face_priority,30);
				params.Set_Face_Location(sound.Position,5);
				Commands->Action_Face_Location(obj,params);
			}
		}
	}
	else
	{
		if (!rampaging)
		{
			if (!moving)
			{
				if (Commands->Is_A_Star(sound.Creator))
				{
					ActionParamsStruct params;
					params.Set_Basic(this,(float)face_priority,30);
					params.Set_Face_Location(sound.Position,5);
					Commands->Action_Face_Location(obj,params);
				}
			}
		}
	}
}

void z_Vehicle_AI::Destroyed(GameObject *obj)
{
	//clean up things,send off messages
}

void z_Vehicle_AI::Hunt(GameObject *obj,GameObject *target)
{
	if (target) //Can't do anything without a target
	{
		hunting = true; //The hunt has begun!
		guarding = false; //W00T! no more guard duty!

		Vector3 pos = Commands->Get_Position(target);
		Vector3 mypos = Commands->Get_Position(obj);
		float distance = Commands->Get_Distance(pos,mypos);
		float max = Get_Float_Parameter("MaxRange");

		current_priority += 1; //override last hunt command

		if (distance > max)
		{
			float deviation;

			if (max > 10)
				deviation = max - 10;
			else
				deviation = max;

			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority,10);
			params.Set_Movement(position,1,2,false);
			params.MoveBackup = Set_Direction(obj);
			Commands->Action_Goto(obj,params);
			moving = true;
		}
		else
		{
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority,20);
			params.Set_Attack(target,distance,0,Set_Ammo(target));
			params.AttackCheckBlocked = true;
			Commands->Action_Attack(obj,params);
		}

		Commands->Start_Timer(obj,this,attack_timer,22);
	}
}

void z_Vehicle_AI::Guard(GameObject *obj,GameObject *target)
{
	if (target) //Check for a target
	{
		guarding = true; //time for guard duty
		hunting = false; //any hunts have ended

		Vector3 pos = Commands->Get_Position(target);
		Vector3 mypos = Commands->Get_Position(obj);
		float distance = Commands->Get_Distance(pos,mypos);

		current_priority += 1; //override last command

		if (distance > 10)
		{
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority,10);
			params.Set_Movement(position,1,2,false);
			params.MoveBackup = Set_Direction(obj);
			Commands->Action_Goto(obj,params);
			moving = true;
		}

		Commands->Start_Timer(obj,this,guard_timer,11);
	}
}

GameObject *z_Vehicle_AI::Select_Target(GameObject *obj,GameObject *target)
{
	//This function determines threats between old and new targets
	GameObject *selected = 0; //if threat rating skips,this stays 0
							//if threat keeps old,this stays 0

	if (target) //First see if there is a valid target
	{
		GameObject *old = Commands->Find_Object(targetID);
		if (old) //is there a current target?
		{
			unsigned int newID = Commands->Get_ID(target);

			/******************Threat Rating System**********************/

			if (newID != targetID) //is this a new target
			{
				//requires threat assessment
				if (Commands->Is_A_Star(target)) //Is new one a player?
				{
					GameObject *vehicle = Get_Vehicle(target);

					if (vehicle) //is player in a tank?
					{
						if (Commands->Is_A_Star(old))
						{
							//Both are players,do a vehicle test
							vehicle = Get_Vehicle(old);
							if (vehicle) //is the old in a vehicle?
							{
								//both in vehicle,closer is priority
								selected = Get_Closest(obj,target,old);
							}
							else //old not in a vehicle
							{
								//specify distance to change on
								float distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(old));
								if (distance > Get_Float_Parameter("MaxRange"))
								{
									distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(target));
									if (distance < Get_Float_Parameter("MaxRange"))
										selected = target;
								}
							}
						}
						else
						{
							//new is a player(in tank),old is not
							if (old->As_SoldierGameObj())
							{
								//old is a soldier
								//switch to new target,old not worth it
								selected = target;
							}
							else if (old->As_VehicleGameObj())
							{
								//old is a vehicle,new is a player in tank
								//switch based on distance
								selected = Get_Closest(obj,target,old);
							}
						}
					}
					else //new player is on foot
					{
						if (Commands->Is_A_Star(old))
						{
							//Both are players,do a vehicle test
							vehicle = Get_Vehicle(old);
							if (vehicle) //is the old in a vehicle?
							{
								//new on foot,old in vehicle
								//specify distance to change on
								float distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(old));
								if (distance > Get_Float_Parameter("MaxRange"))
								{
									distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(target));
									if (distance < Get_Float_Parameter("MaxRange"))
										selected = target;
								}
							}
							else //old not in a vehicle
							{
								//both on foot,choose one closer
								selected = Get_Closest(obj,target,old);
							}
						}
						else
						{
							//new is a player on foot,old is not a player
							if (old->As_SoldierGameObj())
							{
								//old is a soldier
								//switch to new target,old not worth it
								selected = target;
							}
							else if (old->As_VehicleGameObj())
							{
								//old is a vehicle
								//switch if old out of range and new is close
								float distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(old));
								if (distance > Get_Float_Parameter("MaxRange"))
								{
									distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(target));
									if (distance < Get_Float_Parameter("MaxRange"))
										selected = target;
								}
							}
						}
					}
				}
				else if (Commands->Is_A_Star(old)) //Is the old guy a player?
				{
					//old a player,new not a player
					GameObject *vehicle = Get_Vehicle(old);
					if (vehicle) //is the old in a vehicle?
					{
						//old in vehicle
						if (target->As_SoldierGameObj())
						{
							//new is a soldier,old is player in a tank
							//Perhaps an out of range check here?
							selected = 0; //keep old					
						}
						else if (target->As_VehicleGameObj())
						{
							//new is a vehicle,old is player in a tank
							//switch if new closer
							selected = Get_Closest(obj,target,old);
						}
					}
					else //old on foot
					{
						if (target->As_SoldierGameObj())
						{
							//new is a soldier
							selected = Get_Closest(obj,target,old);
						}
						else if (target->As_VehicleGameObj())
						{
							//new is a vehicle,old just a player
							selected = target;
						}
					}
				}
			}
		}
		else	//old target was invalid
			targetID = 0; //forget ID
	}

	/***************Determine Specific Target*******************/

	if (selected) //Did we determine a new target?
	{
		targetID = Commands->Get_ID(selected); //remember target
		
		if (!hunting && !guarding) //only initiate if not doing anything
		{
			if (Commands->Is_A_Star(selected)) //Is it a player?
			{
				GameObject *vehicle = Get_Vehicle(selected); 

				if (vehicle) //Are they in a vehicle
					return vehicle; //if so,return vehicle
				else
					return selected; //if not,return player
			}
			return selected; //nothing left but the target
		}
	}
	else //No new target was picked,only change if not doing anything
	{
		if (!hunting && !guarding) //only initiate if not doing anything
		{
			targetID = Commands->Get_ID(target); //remember target
			if (Commands->Is_A_Star(target)) //Is it a player?
			{
				GameObject *vehicle = Get_Vehicle(target); 

				if (vehicle) //Are they in a vehicle
					return vehicle; //if so,return vehicle
				else
					return target; //if not,return player
			}
			return target; //nothing left but the target
		}
	}

	return 0; //don't start new job if already working
}

GameObject *z_Vehicle_AI::Set_Target(GameObject *target)
{
	//This function sets a new target even if one exists
	targetID = Commands->Get_ID(target); //remember target

	if (!hunting && !guarding) //only initiate if not doing anything
	{
		if (Commands->Is_A_Star(target)) //Is it a player?
		{
			GameObject *vehicle = Get_Vehicle(target); 

			if (vehicle) //Are they in a vehicle
				return vehicle; //if so,return vehicle
			else
				return target; //if not,return player
		}

		return target; //nothing left but the target
	}

	return 0; //don't start new job if already working
}

GameObject *z_Vehicle_AI::Get_Closest(GameObject *obj,GameObject *new_target,GameObject *old_target)
{
	Vector3 obj_pos = Commands->Get_Position(obj);
	Vector3 target1_pos = Commands->Get_Position(new_target);
	Vector3 target2_pos = Commands->Get_Position(old_target);

	float dist1 = Commands->Get_Distance(obj_pos,target1_pos);
	float dist2 = Commands->Get_Distance(obj_pos,target2_pos);

	if (dist1 < dist2)
		return new_target; //new is closer,inform of change
	else
		return 0; //old is closer,no change needed
}

bool z_Vehicle_AI::Set_Ammo(GameObject *target)
{
	//This function sets ammo type to use on a target

	if (target)//Just to be on the safe side
	{
		if (target->As_SoldierGameObj()) //is the target a soldier?
		{
			if (Get_Int_Parameter("vsSoldier")) 
				return true; //Use Primary
			else
				return false; //Use Secondary
		}

		if (target->As_VehicleGameObj())
		{
			if (Get_Vehicle_Mode(target) != VEHICLE_TYPE_FLYING)
			{
				if (Get_Int_Parameter("vsAircraft")) 
					return true; //Use Primary
				else
					return false; //Use Secondary
			}
			else
			{
				if (Get_Int_Parameter("vsVehicle")) 
					return true; //Use Primary
				else
					return false; //Use Secondary
			}
		}
	}
	return true; //If all else fails,use Primary Ammo
}

bool z_Vehicle_AI::Set_Direction(GameObject *obj)
{
	Vector3 current = Commands->Get_Position(obj);
	float distance = Commands->Get_Distance(mypos,current);

	if (distance < 1) //did I barely move?
	{
		if (move_count > 2)
		{
			move_count = 0;
			return true; //tried at least 3 times,reverse direction
		}
		move_count++; //haven't hit the limit yet
	}

	mypos = current; //remember current position
	return false; //Don't drive backwards
}

void z_A_HP1_Zone::Entered(GameObject *obj, GameObject *enter)
{
	GameObject *hp = Commands->Find_Object(Get_Int_Parameter("A_HP1_ID"));
	if (Commands->Get_Health(hp) >= 1.0f)
	{
		if (Commands->Get_Player_Type(enter) == 1)
		{
			Commands->Start_Timer(obj,this,1.0,Commands->Get_ID(enter));
		}
	}
}

void z_A_HP1_Zone::Timer_Expired(GameObject *obj, int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	GameObject *object = Commands->Find_Object(number);
	if (!object)
	{
		return;
	}
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(object);
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= 10.0)
	{
		if (object->As_VehicleGameObj())
		{
			if (Get_Vehicle_Mode(object) == VEHICLE_TYPE_FLYING)
			{
				Commands->Give_PowerUp(object,"Apache_Reload",0);
				Commands->Start_Timer(obj, this, 2.0f, number);
			}
		}
	}
}

void z_GDI_OfficerDropGreenKey::Destroyed(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	pos.Z += 0.25f;
	GameObject *pup = Commands->Create_Object("Level_01_Keycard",pos);
	Commands->Set_Facing(pup,Commands->Get_Facing(obj));
}

void z_BeaconNukeReady::Created(GameObject *obj)
{
	Console_Input("snda nukeavail.wav");
	Send_Message(255,255,255,"Nuclear Weapon Available");	
}

void z_BeaconIonReady::Created(GameObject *obj)
{
	Commands->Create_2D_WAV_Sound("m00evag_dsgn0070i1evag_snd.wav");
	Send_Message(255,255,255,"Ion Cannon Ready");	
}


void z_Delayed_Cinematic_Custom::Created(GameObject *obj)
{
	loop = Get_Int_Parameter("Loop");
	msg = Get_Int_Parameter("Message");
	time = Get_Float_Parameter("Delay");
	loops = 0;
}
void z_Delayed_Cinematic_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == msg)
	{
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,time,100);
		}
	}
}
void z_Delayed_Cinematic_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 100)
	{
		const char *scriptname;
		Vector3 position;
		GameObject *object;
		float facing;
		scriptname = Get_Parameter("Script_Name");
		position = Get_Vector3_Parameter("Location");
		facing = Get_Float_Parameter("Facing");
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
		if (loop)
		{
			loops++;
			if (loops <= 100)//lets set a max loops of 100 to avoid the circumstance for an empty server continuesly calling a dropoff when htere are no players ending up with a huge amount of whatever the cinematic drops off....and possibly crashing the server after a while....
			{
				Commands->Start_Timer(obj,this,time,100);
			}
			else
			{
				Destroy_Script();
			}
		}
	}
}

void z_Building_GDI::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Start_Timer(obj,this,3.5f,500);
}
void z_Building_GDI::Timer_Expired(GameObject *obj, int number)
{
	if (number == 500)
	{
		Commands->Create_2D_WAV_Sound("gdistructdestr.wav");
		Send_Message(255,0,0,"GDI Structure Destroyed.");
	}
}

void z_Building_NOD::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Start_Timer(obj,this,3.5f,500);
}
void z_Building_NOD::Timer_Expired(GameObject *obj, int number)
{
	if (number == 500)
	{
		Commands->Create_2D_WAV_Sound("nodstructdestr.wav");
		Send_Message(255,204,0,"Nod Structure Destroyed.");
	}
}

void z_Created_PlaySound::Created(GameObject *obj)
{
	const char *sound = Get_Parameter("Sound");
	const char *mesg = Get_Parameter("Message");
	int red = Get_Int_Parameter("Red");
	int green = Get_Int_Parameter("Green");
	int blue = Get_Int_Parameter("Blue");
	Commands->Create_2D_WAV_Sound(sound);
	Send_Message(red,green,blue,mesg);
}

void z_POW_PickupSounds::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	/* This code was moved to the ssgm.ini under [General] as "PlayPowerupSounds"

	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_MineRemote_02"))
		{
			Send_Message_Player(sender,104,234,40,"Remote C4 acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pac4_aqob0004i1evag_snd.wav");
		}
		else if ( (strstr(Commands->Get_Preset_Name(obj),"POW_Chaingun_Player")) || (strstr(Commands->Get_Preset_Name(obj),"POW_Chaingun_Player_Nod")) )
		{
			Send_Message_Player(sender,104,234,40,"Chaingun ammunition acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pacg_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"CnC_MineProximity_05"))
		{
			Send_Message_Player(sender,104,234,40,"Proximity C4 acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pacp_aqob0004i1evag_snd.wav");
		}
		else if ( (strstr(Commands->Get_Preset_Name(obj),"Level_01_Keycard")) || (strstr(Commands->Get_Preset_Name(obj),"Green_Key")) )
		{
			char pickupmsg[250];
			const char *str = Get_Player_Name(sender);
			sprintf(pickupmsg,"%s acquired a Green Key Card.",str);
			delete[] str;
			Send_Message(104,234,40,pickupmsg);
			Commands->Create_2D_WAV_Sound("greenkeyacquired.wav");
		}
		else if ((strstr(Commands->Get_Preset_Name(obj),"Level_02_Keycard")) || (strstr(Commands->Get_Preset_Name(obj),"Yellow_Key")) )
		{
			char pickupmsg[250];
			const char *str = Get_Player_Name(sender);
			sprintf(pickupmsg,"%s acquired a Yellow Security Card.",str);
			delete[] str;
			Send_Message(104,234,40,pickupmsg);
			Commands->Create_2D_WAV_Sound("yellowkeyacquired.wav");
		}
		else if ( (strstr(Commands->Get_Preset_Name(obj),"Level_03_Keycard")) || (strstr(Commands->Get_Preset_Name(obj),"Red_Key")) )
		{
			char pickupmsg[250];
			const char *str = Get_Player_Name(sender);
			sprintf(pickupmsg,"%s acquired a Red Key Card.",str);
			delete[] str;
			Send_Message(104,234,40,pickupmsg);
			Commands->Create_2D_WAV_Sound("redkeyacquired.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_ChemSprayer_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Chemsprayer fluid acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pacs_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_MineTimed_Player_02"))
		{
			Send_Message_Player(sender,104,234,40,"Timed C4 acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pact_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_Flamethrower_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Flamethrower fuel acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00paft_aqob0001i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_GrenadeLauncher_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Grenades acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pagn_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_IonCannonBeacon_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Ion Cannon Beacon acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00paib_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_IonCannonBeacon_Player"))
		{
			Send_Message_Player(sender,104,234,40,"10 Second Superweapon Beacon acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00paib_aqob0004i1evag_snd.wav");
		}

		else if (strstr(Commands->Get_Preset_Name(obj),"POW_Nuclear_Missle_Beacon"))
		{
			Send_Message_Player(sender,104,234,40,"10 Second Superweapon Beacon acquired.");
			Create_2D_WAV_Sound_Player(sender,"nukeavail.wav");
		}

		else if ((strstr(Commands->Get_Preset_Name(obj),"CnC_POW_IonCannonBeacon_Player")) || (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_Nuclear_Missle_Beacon")))
		{
			Send_Message_Player(sender,104,234,40,"Superweapon Beacon acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00paib_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_LaserChaingun_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Laser Chaingun battery acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00palc_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_LaserRifle_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Laser Rifle battery acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00palr_aqob0004i1evag_snd.wav");
		}
		else if ((strstr(Commands->Get_Preset_Name(obj),"POW_VoltAutoRifle_Player")) || (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_VoltAutoRifle_Player_Nod")))
		{
			Send_Message_Player(sender,104,234,40,"Volt Auto Rifle battery acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pavr_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_Railgun_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Railgun ammunition acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00parg_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_RamjetRifle_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Ramjet Rifle ammunition acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00parj_aqob0004i1evag_snd.wav");
		}
		else if ((strstr(Commands->Get_Preset_Name(obj),"POW_RocketLauncher_Player")) || 
			     (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_RocketLauncher_Player")) || 
				 (strstr(Commands->Get_Preset_Name(obj),"M02_POW_Rocket_FullAmmo")) )
		{
			Send_Message_Player(sender,104,234,40,"Rockets acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00park_aqob0004i1evag_snd.wav");
		}
		else if ((strstr(Commands->Get_Preset_Name(obj),"POW_RepairGun_Player")) || (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_RepairGun_Player")))
		{
			Send_Message_Player(sender,104,234,40,"Maintenance Tool Battery acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00parp_aqob0004i1evag_snd.wav");
		}
		else if ((strstr(Commands->Get_Preset_Name(obj),"POW_SniperRifle_Player")) || (strstr(Commands->Get_Preset_Name(obj),"POW_SniperRifle_Player_Nod")))
		{
			Send_Message_Player(sender,104,234,40,"Sniper Rifle ammunition acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pasr_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_Shotgun_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Shotgun ammunition acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pass_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_TiberiumFlechetteGun_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Tiberium Flechette ammunition acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00patf_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_TiberiumAutoRifle_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Tiberium Auto Rifle ammunition acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00patr_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_TiberiumAutoRifle_Player"))
		{
			Send_Message_Player(sender,104,234,40,"Tiberium Auto Rifle ammunition acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00patr_aqob0004i1evag_snd.wav");
		}
		else if ( (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_Armor_Max")) || (strstr(Commands->Get_Preset_Name(obj),"POW_Armor_100")) )
		{
			Send_Message_Player(sender,104,234,40,"Full Body Armor acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00prba_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_Armor_050"))
		{
			Send_Message_Player(sender,104,234,40,"Breast Plate Armor acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00prbp_aqob0004i1evag_snd.wav");
		}
		else if ( (strstr(Commands->Get_Preset_Name(obj),"CnC_POW_Health_Max")) || (strstr(Commands->Get_Preset_Name(obj),"POW_Health_025")) || (strstr(Commands->Get_Preset_Name(obj),"POW_Health_050")) || (strstr(Commands->Get_Preset_Name(obj),"POW_Health_100")) )
		{
			Send_Message_Player(sender,104,234,40,"Health Supplement acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00ph25_aqob0004i1evag_snd.wav");
		}
	}
	*/
}

void z_Created_Send_Custom_Delay::Created(GameObject *obj)
{
	float delay = Get_Float_Parameter("Delay");
	int mesg = Get_Int_Parameter("Message");
	int prm = Get_Int_Parameter("Param");
	int targetid = Get_Int_Parameter("TargetID");
	Commands->Send_Custom_Event(obj,Commands->Find_Object(targetid),mesg,prm,delay);
}

void z_MP_GDI_MP::Created(GameObject *obj)//attached to minigunners
{
	if (Commands->Is_A_Star(obj))
	{
		if (strcmp(The_Game()->Get_Map_Name(),"C&C_Tutorial.mix") == 0)
		{
			Change_Character(obj,"M00_MP_GDI_MP");
		}
		else if (strcmp(The_Game()->Get_Map_Name(),"C&C_M03.mix") == 0)
		{
			Change_Character(obj,"M03_MP_GDI_MP");
		}
	}
}

void z_zone_gdienteringenemyref::Created(GameObject *obj)
{
	play = true;
}
void z_zone_gdienteringenemyref::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		if (play)
		{
			Commands->Create_2D_WAV_Sound("enteringref.wav");
			Send_Message(255,204,0,"Entering enemy Refinery.");
			play = false;
			Destroy_Script();
		}
	}
}

void z_zone_gdienteringenemycc::Created(GameObject *obj)
{
	play = true;
}
void z_zone_gdienteringenemycc::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		if (play)
		{
			Commands->Create_2D_WAV_Sound("enteringcomm.wav");
			Send_Message(255,204,0,"Entering enemy Communications Center.");
			play = false;
			Destroy_Script();
		}
	}
}

void z_poke_enable_spawner::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void z_poke_enable_spawner::Poked(GameObject *obj, GameObject *poker)
{
	Commands->Enable_Spawner(Get_Int_Parameter("SpawnerID"),true);
	Commands->Enable_HUD_Pokable_Indicator(obj,false);
	Destroy_Script();
}

void z_Delayed_Cinematic_Custom_ID::Created(GameObject *obj)
{
	startnow = Get_Int_Parameter("StartNow");
	startnowdelay = Get_Float_Parameter("StartNowDelay");
	id = Get_Int_Parameter("LocationID");
	
	loop = Get_Int_Parameter("Loop");
	msg = Get_Int_Parameter("Message");
	delay = Get_Float_Parameter("Delay");
	loopsmax = Get_Int_Parameter("LoopMax");
	scriptname = Get_Parameter("Script_Name");
	loops = 0;
}
void z_Delayed_Cinematic_Custom_ID::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == msg)
	{
		if (param == 1)
		{
			if (startnow)
			{
				Commands->Start_Timer(obj,this,startnowdelay,100);
			}
			if (loop)
			{
				Commands->Start_Timer(obj,this,delay,101);
			}
		}
	}
}
void z_Delayed_Cinematic_Custom_ID::Timer_Expired(GameObject *obj,int number)
{
	if (number == 100)
	{
		Vector3 position;
		GameObject *object;
		float facing;
		position = Commands->Get_Position(Commands->Find_Object(id));
		facing = 0.0f;
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
	}
	else if (number == 101)
	{
		Vector3 position;
		GameObject *object;
		float facing;
		position = Commands->Get_Position(Commands->Find_Object(id));
		facing = 0.0f;
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
		if (loop)
		{
			loops++;
			if (loops <= loopsmax)//if number of current loops is less/equal to the max number of loops then do a dropoff else do destroy script and stop doing dropoffs...
			{
				Commands->Start_Timer(obj,this,delay,101);
			}
			else
			{
				Destroy_Script();
			}
		}
	}
}

void z_Enter_Send_Custom::Created(GameObject *obj)
{
	domsg = true;
	id = Get_Int_Parameter("ID");
	msg = Get_Int_Parameter("Message");
	delay = Get_Float_Parameter("Delay");
}
void z_Enter_Send_Custom::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		domsg = false;
		Commands->Start_Timer(obj,this,0.1f,250);
	}
}
void z_Enter_Send_Custom::Timer_Expired(GameObject *obj, int number)
{
	if (number == 250)
	{
		Commands->Send_Custom_Event(obj,Commands->Find_Object(id),msg,1,0);
		Destroy_Script();
	}
}

void z_Poke_Grant_Weapons::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	allowpoke = true;
	weapon1 = Get_Parameter("Weapon1");
	weapon2 = Get_Parameter("Weapon2");
	weapon3 = Get_Parameter("Weapon3");
	cost = Get_Float_Parameter("Cost");
}
void z_Poke_Grant_Weapons::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (Commands->Get_Money(poker) >= cost)
		{
			allowpoke = false;
			Commands->Enable_HUD_Pokable_Indicator(obj, false);
			Commands->Start_Timer(obj,this,1.0f,100);
			Commands->Give_PowerUp(poker,weapon1,true);
			Commands->Give_PowerUp(poker,weapon2,true);
			Commands->Give_PowerUp(poker,weapon3,true);
			Commands->Give_Money(poker,-cost,false);
		}
	}
}
void z_Poke_Grant_Weapons::Timer_Expired(GameObject *obj,int number)
{
	if (number == 100)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}

void z_Console_Input_Created::Created(GameObject *obj)
{
	Console_Input(Get_Parameter("Input"));
}

void z_Created_Play_Sound_Msg::Created(GameObject *obj)
{
	const char *snd = Get_Parameter("Sound");
	const char *msg = Get_Parameter("Message");
	int red = Get_Int_Parameter("Red"); 
	int green = Get_Int_Parameter("Green"); 
	int blue = Get_Int_Parameter("Blue");

	Commands->Create_2D_WAV_Sound(snd);
	Send_Message(red,green,blue,msg);
}

void z_Custom_Play_Sound_Msg_Delay::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	snd = Get_Parameter("Sound");
	msg = Get_Parameter("Message");
	red = Get_Int_Parameter("Red"); 
	green = Get_Int_Parameter("Green"); 
	blue = Get_Int_Parameter("Blue");
	delay = Get_Float_Parameter("Delay");
}
void z_Custom_Play_Sound_Msg_Delay::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == custom)
	{
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,delay,100);
		}
	}
}
void z_Custom_Play_Sound_Msg_Delay::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Create_2D_WAV_Sound(snd);
		Send_Message(red,green,blue,msg);
	}
}


//we need a script that sends a message when a player ENTERS it, but we only want to send the message once...
//so we destroy our script once the first player entered it and the message was sent...
//what we use is the ::Entered event like this:

void z_Enter_Send_Custom_Once::Entered(GameObject *obj, GameObject *enter)
{
	//here goes our code... we need to ask the user for input for the ID and Message to send, its also possible to ask for a param
	//but i never do that, i always send param 1 anyways, and we dont need a delay...
	int msg = Get_Int_Parameter("Message");
	int id = Get_Int_Parameter("ID");
	Commands->Send_Custom_Event(obj,Commands->Find_Object(id),msg,1,0);//this will send the msg to the id with param 1 and 0 delay...
	//we destroy our script now soit doesnt trigger again on another player
	Destroy_Script();
}

void z_Drop_Key_Guy::Killed(GameObject *obj, GameObject *shooter)
{
	int key = Get_Int_Parameter("KeyNo");
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *powerup = 0;
	pos.Z += 0.25f;

	if (key == 1)//green
	{
		powerup = Commands->Create_Object("Level_01_Keycard",pos);
	}
	else if (key == 2)//yellow
	{
		powerup = Commands->Create_Object("Level_02_Keycard",pos);
	}
	else if (key == 3)//red
	{
		powerup = Commands->Create_Object("Level_03_Keycard",pos);
	}
	Commands->Set_Facing(powerup,Commands->Get_Facing(obj));
}

void z_Poke_Teleport_Simple::Poked(GameObject *obj, GameObject *poker)
{
	int id = Get_Int_Parameter("GotoID");
	GameObject *gotoObject = Commands->Find_Object(id);
	Vector3 gotoLocation = Commands->Get_Position(gotoObject);
	Commands->Set_Position(poker,gotoLocation);
}

void z_Custom_Enable_Spawner::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		if (param == 1)
		{
			Commands->Enable_Spawner(Get_Int_Parameter("ID"),Get_Int_Parameter("Enable"));
		}
	}
}

void z_Custom_Goto_Star::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		if (param == 1)
		{
			Attach_Script_Once(obj,"M03_Goto_Star","");
			Destroy_Script();
		}
	}
}

void z_Poke_PPAGE3::Created(GameObject *obj)
{
	dopage = true;
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
}
void z_Poke_PPAGE3::Poked(GameObject *obj, GameObject *poker)
{
	if (dopage)
	{
		if (_stricmp(Get_Parameter("PPage1"),"0") != 0)
		{
			char p1[250];
			sprintf(p1,"ppage %d %s",Get_Player_ID(poker),Get_Parameter("PPage1"));
			Console_Input(p1);
		}
		if (_stricmp(Get_Parameter("PPage2"),"0") != 0)
		{
			char p2[250];
			sprintf(p2,"ppage %d %s",Get_Player_ID(poker),Get_Parameter("PPage2"));
			Console_Input(p2);
		}
		if (_stricmp(Get_Parameter("PPage3"),"0") != 0)
		{
			char p3[250];
			sprintf(p3,"ppage %d %s",Get_Player_ID(poker),Get_Parameter("PPage3"));
			Console_Input(p3);
		}
		dopage = false;
		Commands->Start_Timer(obj,this,5.0f,150);
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
	}
}
void z_Poke_PPAGE3::Timer_Expired(GameObject *obj, int number)
{
	if (number == 150)
	{
		dopage = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}
void z_Poke_PPAGE3::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&dopage,5,1);
}

void z_Poke_PAMSG3::Created(GameObject *obj)
{
	dopage = true;
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
}
void z_Poke_PAMSG3::Poked(GameObject *obj, GameObject *poker)
{
	if (dopage)
	{
		if (_stricmp(Get_Parameter("PPage1"),"0") != 0)
		{
			char p1[250];
			sprintf(p1,"pamsg %d %s",Get_Player_ID(poker),Get_Parameter("PPage1"));
			Console_Input(p1);
		}
		if (_stricmp(Get_Parameter("PPage2"),"0") != 0)
		{
			char p2[250];
			sprintf(p2,"pamsg %d %s",Get_Player_ID(poker),Get_Parameter("PPage2"));
			Console_Input(p2);
		}
		if (_stricmp(Get_Parameter("PPage3"),"0") != 0)
		{
			char p3[250];
			sprintf(p3,"pamsg %d %s",Get_Player_ID(poker),Get_Parameter("PPage3"));
			Console_Input(p3);
		}
		dopage = false;
		Commands->Start_Timer(obj,this,5.0f,150);
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
	}
}
void z_Poke_PAMSG3::Timer_Expired(GameObject *obj, int number)
{
	if (number == 150)
	{
		dopage = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}
void z_Poke_PAMSG3::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&dopage,5,1);
}

void z_PT_Terminal::Poked(GameObject *obj,GameObject *poker)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	int team;
	team = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(poker,team))
	{
		return;
	}

	Vector3 mypos;
	mypos = Commands->Get_Position(obj);
	Vector3 targetpos;
	targetpos = Commands->Get_Position(poker);
	float distance = Commands->Get_Distance(mypos,targetpos);
	
	if (distance <= 50.0f)
	{
		if (!team)
		{
			Display_NOD_Player_Terminal_Player(poker);
		}
		if (team == 1)
		{
			Display_GDI_Player_Terminal_Player(poker);
		}
		if (team == 2)
		{
			if (!Commands->Get_Player_Type(poker))
			{
				Display_NOD_Player_Terminal_Player(poker);
			}
			if (Commands->Get_Player_Type(poker) == 1)
			{
				Display_GDI_Player_Terminal_Player(poker);
			}
		}
	}
	else 
	{
		Send_Message(255,255,255,"Access denied.");
	}
}

void z_Enter_Change_Team::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Get_Player_Type(enter) == 0)
	{
		Change_Team(enter,1);
	}
	else
	{
		Change_Team(enter,0);
	}
}

void z_Enter_Teleport_Random::Entered(GameObject *obj, GameObject *enter)
{

	int random = Commands->Get_Random_Int(1,5);
	{
	
		if (random == 1)
		{
			int x = Get_Int_Parameter("Object_ID1");
			if (x)
			{
				GameObject *gotoObject = Commands->Find_Object(x);
				Vector3 gotoLocation = Commands->Get_Position(gotoObject);
				Commands->Set_Position(enter,gotoLocation);
			}
		}
		else if (random == 2)
		{
			int x = Get_Int_Parameter("Object_ID2");
			if (x)
			{
				GameObject *gotoObject = Commands->Find_Object(x);
				Vector3 gotoLocation = Commands->Get_Position(gotoObject);
				Commands->Set_Position(enter,gotoLocation);
			}
		}
		else if (random == 3)
		{
			int x = Get_Int_Parameter("Object_ID3");
			if (x)
			{
				GameObject *gotoObject = Commands->Find_Object(x);
				Vector3 gotoLocation = Commands->Get_Position(gotoObject);
				Commands->Set_Position(enter,gotoLocation);
			}
		}
		else if (random == 4)
		{
			int x = Get_Int_Parameter("Object_ID4");
			if (x)
			{
				GameObject *gotoObject = Commands->Find_Object(x);
				Vector3 gotoLocation = Commands->Get_Position(gotoObject);
				Commands->Set_Position(enter,gotoLocation);
			}
		}
		else
		{
			int x = Get_Int_Parameter("Object_ID5");
			if (x)
			{
				GameObject *gotoObject = Commands->Find_Object(x);
				Vector3 gotoLocation = Commands->Get_Position(gotoObject);
				Commands->Set_Position(enter,gotoLocation);
			}
		}
	}
}

void z_Vehicle_Buy_Poke_Cinematic::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	allowpoke = true;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void z_Vehicle_Buy_Poke_Cinematic::Poked(GameObject *obj, GameObject *poker)
{
	if (team == Get_Int_Parameter("Team"))
	{
		if (allowpoke)
		{
			if (Commands->Get_Money(poker) < Get_Float_Parameter("Cost"))
			{
				Send_Message_Player(poker,255,255,255,"Access Denied. You have insufficient funds.");
			}
			else
			{
				allowpoke = false;
				const char *scriptname = Get_Parameter("Cinematic");
				Vector3 position = Get_Vector3_Parameter("Position");
				float facing = Get_Float_Parameter("Facing");
				GameObject *object = Commands->Create_Object("Invisible_Object",position);
				Commands->Set_Facing(object,facing);
				Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
				Commands->Create_2D_WAV_Sound(Get_Parameter("Purchase_Sound"));
				Commands->Start_Timer(obj,this,5.0f,857673);
				Commands->Give_Money(poker,-Get_Float_Parameter("Cost"),false);
				Commands->Enable_HUD_Pokable_Indicator(obj,false);
			}
		}
	}
	else
	{
		Send_Message_Player(poker,255,255,255,"Go away n00b!");
	}
}
void z_Vehicle_Buy_Poke_Cinematic::Timer_Expired(GameObject *obj, int number)
{
	if (number == 857673)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj,true);
	}
}
void z_Vehicle_Buy_Poke_Cinematic::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowpoke,5,1);
	Auto_Save_Variable(&team,5,1);
}

void z_Poke_Change_Team::Poked(GameObject *obj, GameObject *poker)
{
	if (Commands->Get_Money(poker) >= Get_Float_Parameter("Cost"))
	{
		Change_Team(poker,Get_Int_Parameter("Team"));
	}
	else
	{
		Send_Message_Player(poker,255,255,255,"Insufficiend funds for a teamchange. Sorry.");
	}
}

void z_Timer_Set_Team::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),Get_Int_Parameter("Number"));
}
void z_Timer_Set_Team::Timer_Expired(GameObject *obj, int number)
{
	if (number == Get_Int_Parameter("Number"))
	{
		Commands->Set_Player_Type(obj,Get_Int_Parameter("Type"));
	}
}

void z_Enter_Kill_Star::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		Commands->Apply_Damage(enter,99999.0f,"Steel",enter);//killed himself lol :)
	}
}

void z_Killed_Play_Sound_Message::Killed(GameObject *obj, GameObject *shooter)
{
	char msg[250];
	sprintf(msg,"%s",Get_Parameter("Message"));
	Send_Message(Get_Int_Parameter("Red"),Get_Int_Parameter("Green"),Get_Int_Parameter("Blue"),msg);
	Commands->Create_2D_WAV_Sound(Get_Parameter("WavSound"));
}

void z_Powerup_Expire::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,11.0f,100);
}
void z_Powerup_Expire::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Expire_Powerup(obj);
		Commands->Destroy_Object(obj);
	}	
}

void z_Map_Time_Limit::Created(GameObject *obj)
{
	int time = Get_Int_Parameter("Time_In_Seconds");
	char settime[250];
	sprintf(settime,"time %d",time);
	Console_Input(settime);
}

void z_Zone_Aircraft_Refill::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	if ((enterer->As_VehicleGameObj()) && (Is_VTOL(enterer)))
	{
		Commands->Start_Timer(obj,this,1.0,Commands->Get_ID(enterer));
	}
}

void z_Zone_Aircraft_Refill::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Refill_Distance"))
	{
		const char *pup = Get_Parameter("Powerup_Preset");
		Commands->Give_PowerUp(Commands->Find_Object(number),pup,false);
		Commands->Start_Timer(obj,this,1.0,number);
	}
}

ScriptRegistrant<z_Zone_Aircraft_Refill> z_Zone_Aircraft_Refill_Registrant("z_Zone_Aircraft_Refill","Powerup_Preset=asdf:string,Player_Type=1:int,Refill_Distance=5000:float");
ScriptRegistrant<z_Map_Time_Limit> z_Map_Time_Limit_Registrant("z_Map_Time_Limit","Time_In_Seconds=50000:int");
ScriptRegistrant<z_Powerup_Expire> z_Powerup_Expire_Registrant("z_Powerup_Expire","");
ScriptRegistrant<z_Killed_Play_Sound_Message> z_Killed_Play_Sound_Message_Registrant("z_Killed_Play_Sound_Message","Message=bla:string,WavSound=bla:string,Red=255:int,Green=0:int,Blue=0:int");
ScriptRegistrant<z_Enter_Kill_Star> z_Enter_Kill_Star_Registrant("z_Enter_Kill_Star","");
ScriptRegistrant<z_Timer_Set_Team> z_Timer_Set_Team_Registrant("z_Timer_Set_Team","Time=6000:float,Number=500:int,Type=1:int");
ScriptRegistrant<z_Poke_Change_Team> z_Poke_Change_Team_Registrant("z_Poke_Change_Team","Team=3:int,Cost=10000:float");
ScriptRegistrant<z_Vehicle_Buy_Poke_Cinematic> z_Vehicle_Buy_Poke_Cinematic_Registrant("z_Vehicle_Buy_Poke_Cinematic","Team=1:int,Cost=10000:float,Cinematic=bla:string,Position=88:vector3,Facing=90:float,Purchase_Sound=asdf:string");
ScriptRegistrant<z_Enter_Teleport_Random> z_Enter_Teleport_Random_Registrant("z_Enter_Teleport_Random","Object_ID1=1:int,Object_ID2=1:int,Object_ID3=1:int,Object_ID4=1:int,Object_ID5=1:int");
ScriptRegistrant<z_Enter_Change_Team> z_Enter_Change_Team_Registrant("z_Enter_Change_Team","");
ScriptRegistrant<z_PT_Terminal> z_PT_Terminal_Registrant("z_PT_Terminal","Player_Type=0:int");
ScriptRegistrant<z_Poke_PAMSG3> z_Poke_PAMSG3_Registrant("z_Poke_PAMSG3","PPage1=bla:string,PPage2=bla:string,PPage3=bla:string");
ScriptRegistrant<z_Poke_PPAGE3> z_Poke_PPAGE3_Registrant("z_Poke_PPAGE3","PPage1=bla:string,PPage2=bla:string,PPage3=bla:string");
ScriptRegistrant<z_Custom_Goto_Star> z_Custom_Goto_Star_Registrant("z_Custom_Goto_Star","Message=1000:int");
ScriptRegistrant<z_Custom_Enable_Spawner> z_Custom_Enable_Spawner_Registrant("z_Custom_Enable_Spawner","Message=1:int,ID=1:int,Enable=1:int");
ScriptRegistrant<z_Poke_Teleport_Simple> z_Poke_Teleport_Simple_Registrant("z_Poke_Teleport_Simple","GotoID=1:int");
ScriptRegistrant<z_Drop_Key_Guy> z_Drop_Key_Guy_Registrant("z_Drop_Key_Guy","KeyNo=1:int");
ScriptRegistrant<z_Enter_Send_Custom_Once> z_Enter_Send_Custom_Once_Registrant("z_Enter_Send_Custom_Once","Message=1000:int,ID=123:int");
ScriptRegistrant<z_Custom_Play_Sound_Msg_Delay> z_Custom_Play_Sound_Msg_Delay_Registrant("z_Custom_Play_Sound_Msg_Delay","Custom=1000:int,Sound=asdf:string,Message=bla:string,Red=1:int,Green=1:int,Blue=1:int,Delay=5:float");
ScriptRegistrant<z_Created_Play_Sound_Msg> z_Created_Play_Sound_Msg_Registrant("z_Created_Play_Sound_Msg","Sound=asdf:string,Message=bla:string,Red=1:int,Green=1:int,Blue=1:int");
ScriptRegistrant<z_Console_Input_Created> z_Console_Input_Created_Registrant("z_Console_Input_Created","Input=msg:string");
ScriptRegistrant<z_Poke_Grant_Weapons> z_Poke_Grant_Weapons_Registrant("z_Poke_Grant_Weapons","Cost=1000:float,Weapon1=asdf:string,Weapon2=asdf:string,Weapon3=asdf:string");
ScriptRegistrant<z_Enter_Send_Custom> z_Enter_Send_Custom_Registrant("z_Enter_Send_Custom","ID=1:int,Message=1000:int,Delay=5:float");
ScriptRegistrant<z_Delayed_Cinematic_Custom_ID> z_Delayed_Cinematic_Custom_ID_Registrant("z_Delayed_Cinematic_Custom_ID","Message=1000:int,LocationID=1:int,StartNow=1:int,StartNowDelay=5:float,Loop=1:int,LoopMax=100:int,Delay=5:float,Script_Name=asdf:string");
ScriptRegistrant<z_poke_enable_spawner> z_poke_enable_spawner_Registrant("z_poke_enable_spawner","SpawnerID=1:int");
ScriptRegistrant<z_zone_gdienteringenemycc> z_zone_gdienteringenemycc_Registrant("z_zone_gdienteringenemycc","");
ScriptRegistrant<z_zone_gdienteringenemyref> z_zone_gdienteringenemyref_Registrant("z_zone_gdienteringenemyref","");
ScriptRegistrant<z_MP_GDI_MP> z_MP_GDI_MP_Registrant("z_MP_GDI_MP","");
ScriptRegistrant<z_Created_Send_Custom_Delay> z_Created_Send_Custom_Delay_Registrant("z_Created_Send_Custom_Delay","");
ScriptRegistrant<z_POW_PickupSounds> z_POW_PickupSounds_Registrant("z_POW_PickupSounds","");
ScriptRegistrant<z_Created_PlaySound> z_Created_PlaySound_Registrant("z_Created_PlaySound","Sound=a:string,Message=b:string,Red=1:int,Green=1:int,Blue=1:int");
ScriptRegistrant<z_Building_NOD> z_Building_NOD_Registrant("z_Building_NOD","");
ScriptRegistrant<z_Building_GDI> z_Building_GDI_Registrant("z_Building_GDI","");
ScriptRegistrant<z_Delayed_Cinematic_Custom> z_Delayed_Cinematic_Custom_Registrant("z_Delayed_Cinematic_Custom","Delay:float,Message:int,Script_Name:string,Location:vector3,Facing:float,Loop:int");
ScriptRegistrant<z_BeaconIonReady> z_BeaconIonReady_Registrant("z_BeaconIonReady","");
ScriptRegistrant<z_BeaconNukeReady> z_BeaconNukeReady_Registrant("z_BeaconNukeReady","");
ScriptRegistrant<z_GDI_OfficerDropGreenKey> z_GDI_OfficerDropGreenKey_Registrant("z_GDI_OfficerDropGreenKey","");
ScriptRegistrant<z_Vehicle_AI> z_Vehicle_AI_AI_Registrant("z_Vehicle_AI","MaxRange=0.0:float,vsSoldier=1:int,vsVehicle=1:int,vsAircraft=1:int");
ScriptRegistrant<z_Powerdown_Crate> z_Powerdown_Crate_Registrant("z_Powerdown_Crate","Team=0:int");
ScriptRegistrant<z_I_May_Not_Place_Defenses> z_I_May_Not_Place_Defenses_Registrant("z_I_May_Not_Place_Defenses","");
ScriptRegistrant<z_No_PlaceDefense_Zone_Enter> z_No_PlaceDefense_Zone_Enter_Registrant("z_No_PlaceDefense_Zone_Enter","");
ScriptRegistrant<z_No_PlaceDefense_Zone_Exit> z_No_PlaceDefense_Zone_Exit_Registrant("z_No_PlaceDefense_Zone_Exit","");
ScriptRegistrant<z_Pickup_Send_Custom> z_Pickup_Send_Custom_Registrant("z_Pickup_Send_Custom","ID=1:int,Message=1000:int,Param=1:int,Delay=100:float");
ScriptRegistrant<z_Prototype> z_Prototype_Registrant("z_Prototype","Allow_Team=1:int");
ScriptRegistrant<z_Prototype_AllowEnter> z_Prototype_AllowEnter_Registrant("z_Prototype_AllowEnter","Allow_Team=1:int");
ScriptRegistrant<z_Prototype_Zone> z_Prototype_Zone_Registrant("z_Prototype_Zone","Prototype=CnC_Soviet_MAD_Tank:string,Prototype_Friendly_Name=MAD:string,ResearchTeam=1:int,ResearchTeamName=Allies:string,ResearchTime=10:float,Spawner_1=1:int,Spawner_2=1:int,Spawner_3=1:int,Spawner_4=1:int,Spawner_5=1:int,Spawner_6=1:int,Spawner_7=1:int,Spawner_8=1:int,Spawner_9=1:int,Spawner_10=1:int,Timelimit=100:float,LosingTeamTimeExpired=1:int,ProtoSpawnerID=100:int");
ScriptRegistrant<z_OT_Deliver_SIC> z_OT_Deliver_SIC_Registrant("z_OT_Deliver_SIC","Time:float,TimerNum:int,Repeat:int,Script_Name:string,Location:vector3,Facing:float");
ScriptRegistrant<z_Soviet_Paradrop> z_Soviet_Paradrop_Registrant("z_Soviet_Paradrop","Time:float,TimerNum:int,Repeat:int,Script_Name:string,Location:vector3,Facing:float");
ScriptRegistrant<z_Cinematic_Attack3> z_Cinematic_Attack3_Registrant("z_Cinematic_Attack3","Move_ID=1:int,Attack_ID=1:int");
ScriptRegistrant<z_MCV_Deploy_SIC> z_MCV_Deploy_SIC_Registrant("z_MCV_Deploy_SIC","Loc=0:vector3,Facing=180000:float,Preset=allied_cy:string");
ScriptRegistrant<z_MissionStart_SIC> z_MissionStart_SIC_Registrant("z_MissionStart_SIC","");
ScriptRegistrant<z_Allied_MCV_Killed> z_Allied_MCV_Killed_Registrant("z_Allied_MCV_Killed","");
ScriptRegistrant<z_Cinematic_Attack2> z_Cinematic_Attack2_Registrant("z_Cinematic_Attack2","ID=1:int");
ScriptRegistrant<z_Created_Timer_EnableSpawners> z_Created_Timer_EnableSpawners_Registrant("z_Created_Timer_EnableSpawners","StartAfter=10:float,TimerNumber=870:int,ID1=1:int,ID2=1:int,ID3=1:int,ID4=1:int,ID5=1:int,Enable=1:int");
ScriptRegistrant<z_Airraid_Custom> z_Airraid_Custom_Registrant("z_Airraid_Custom","Message=1000:int");
ScriptRegistrant<z_Attack_MCV_SIC> z_Attack_MCV_SIC_Registrant("z_Attack_MCV_SIC","");
ScriptRegistrant<z_Cinematic_Attack> z_Cinematic_Attack_Registrant("z_Cinematic_Attack","ID=1:int,Priority=100:int,Range=100:float,Deviation=0:float,Primary=1:int");
ScriptRegistrant<z_Purchase_Turret> z_Purchase_Turret_Registrant("z_Purchase_Turret","Preset=pb:string,Cost=250000:float,Loc=0:vector3,Facing=0:float,Direction=South:string");
ScriptRegistrant<z_Purchase_Pillbox> z_Purchase_Pillbox_Registrant("z_Purchase_Pillbox","Preset=pb:string,Cost=250000:float,Loc=0:vector3,Facing=0:float,Direction=South:string");
ScriptRegistrant<z_TanyaVolkovAnnounce> z_TanyaVolkovAnnounce_Registrant("z_TanyaVolkovAnnounce","");
ScriptRegistrant<z_Pup_Allied_Officer> z_Pup_Allied_Officer_Registrant("z_Pup_Allied_Officer","");
ScriptRegistrant<z_Pup_Allied_Tanya> z_Pup_Allied_Tanya_Registrant("z_Pup_Allied_Tanya","");
ScriptRegistrant<z_Pup_Allied_RocketGuy> z_Pup_Allied_RocketGuy_Registrant("z_Pup_Allied_RocketGuy","");
ScriptRegistrant<z_Pup_Allied_Medic> z_Pup_Allied_Medic_Registrant("z_Pup_Allied_Medic","");
ScriptRegistrant<z_Pup_Allied_Medic_Coop> z_Pup_Allied_Medic_Coop_Registrant("z_Pup_Allied_Medic_Coop","");
ScriptRegistrant<z_Pup_Allied_Sniper> z_Pup_Allied_Sniper_Registrant("z_Pup_Allied_Sniper","");
ScriptRegistrant<z_Pup_Allied_Mechanic> z_Pup_Allied_Mechanic_Registrant("z_Pup_Allied_Mechanic","");
ScriptRegistrant<z_Pup_Allied_Sniper_Coop> z_Pup_Allied_Sniper_Coop_Registrant("z_Pup_Allied_Sniper_Coop","");
ScriptRegistrant<z_Pup_Allied_Spy> z_Pup_Allied_Spy_Registrant("z_Pup_Allied_Spy","");
ScriptRegistrant<z_Buy_Longbow> z_Buy_Longbow_Registrant("z_Buy_Longbow","Max=5:int,Cost=12000:float,Loc=0:vector3,Facing=0:float");
ScriptRegistrant<z_Engineer_Repair> z_Engineer_Repair_Registrant("z_Engineer_Repair","Message=1000:int,Param=1:int,No_Repair_MSG=bla:string");
ScriptRegistrant<z_Start_Music_Delay> z_Start_Music_Delay_Registrant("z_Start_Music_Delay","MP3=bla.mp3:string,StartAfter=0:float,Duration=200:float,TimerNum=0:int");
ScriptRegistrant<z_Custom_Multiple_Send_Custom> z_Custom_Multiple_Send_Custom_Registrant("z_Custom_Multiple_Send_Custom","RecieveMessage:int,RecieveCount:int,SendMessage:int,SendParam:int,SendID:int");
ScriptRegistrant<z_Death_Send_Custom_Delay> z_Death_Send_Custom_Delay_Registrant("z_Death_Send_Custom_Delay","ID:int,Message:int,Param:int,Delay=10:float");
ScriptRegistrant<z_Defense_Owner> z_Defense_Owner_Registrant("z_Defense_Owner","Owner_ID=1:int");
ScriptRegistrant<z_Custom_Send_Custom_Delay> z_Custom_Send_Custom_Delay_Registrant("z_Custom_Send_Custom_Delay","Message=0:int,SendID=1:int,SendMessage=1:int,SendParam=1:int,SendAfter=0:float");
ScriptRegistrant<z_Death_Send_Custom> z_Death_Send_Custom_Registrant("z_Death_Send_Custom","ID:int,Message:int,Param:int");
ScriptRegistrant<z_VehExit_DestroyVeh> z_VehExit_DestroyVeh_Registrant("z_VehExit_DestroyVeh","");
ScriptRegistrant<z_Enter_Enable_Spawner> z_Enter_Enable_Spawner_Registrant("z_Enter_Enable_Spawner","Trigger_Team=1:int,ID1=0:int,ID2=0:int,ID3=0:int,Enable=1:int");
ScriptRegistrant<z_Enable_Spawner_Custom> z_Enable_Spawner_Custom_Registrant("z_Enable_Spawner_Custom","Enable=1:int");
ScriptRegistrant<z_Goto_Star_Custom> z_Goto_Star_Custom_Registrant("z_Goto_Star_Custom","Message=0:int");
ScriptRegistrant<z_Hostage> z_Hostage_Registrant("z_Hostage","PokerTeam=1:int,Hostage_Team=0:int,Hostage_Health=100:float,Hostage_Shield=100:float,Health_Regen_Ammount=1:float,Rescue_Message=1000:int,Controller_ID=1010:int,HostageDead_Message=2020:int");
ScriptRegistrant<z_Hostage_Rescue_Zone> z_Hostage_Rescue_Zone_Registrant("z_Hostage_Rescue_Zone","Hostage_Team=1:int,Hostage_Preset=asdf:string,Hostage_Preset2=asdf:string,Hostage_Preset3=asdf:string,Hostage_Preset4=asdf:string,Hostage_Preset5=asdf:string,Rescue_Message=1000:int");
ScriptRegistrant<z_Hostage_Controller> z_Hostage_Controller_Registrant("z_Hostage_Controller","Rescue_Message:int,HostageDead_Message:int,Required_Rescues:int,Max_Hosty_Deaths:int,Hostage_Team:int");
ScriptRegistrant<z_Created_Timer_Enable_Spawner> z_Created_Timer_Enable_Spawner_Registrant("z_Created_Timer_Enable_Spawner","Send_After:float,TimerNumber:int,ID1:int,ID2:int,ID3:int,Enable:int,Play_Sound:string");
ScriptRegistrant<z_Console_Input_Poke_2> z_Console_Input_Poke_2_Registrant("z_Console_Input_Poke_2","Console_Command1:string,Console_Command2:string");
ScriptRegistrant<z_Console_Input_Killed> z_Console_Input_Killed_Registrant("z_Console_Input_Killed","Input:string");
ScriptRegistrant<z_Console_Input_Killed_2> z_Console_Input_Killed_2_Registrant("z_Console_Input_Killed_2","Input=msg:string,Input2=snda:string");
ScriptRegistrant<z_Console_Input_Custom_2> z_Console_Input_Custom_2_Registrant("z_Console_Input_Custom_2","Message:int,Input:string,Input2:string");
ScriptRegistrant<z_Enable_Multiple_Spawners_Custom> z_Enable_Multiple_Spawners_Custom_Registrant("z_Enable_Multiple_Spawners_Custom","ID1:int,ID2:int,ID3:int,ID4:int,ID5:int,Enable:int,Message:int");
ScriptRegistrant<z_Play_Three_Cinematics_Custom> z_Play_Three_Cinematics_Custom_Registrant("z_Play_Three_Cinematics_Custom","Message:int,Cinematic1:string,Location1:vector3,Facing1:float,Cinematic2:string,Location2:vector3,Facing2:float,Cinematic3:string,Location3:vector3,Facing3:float,Console_Command:string");
ScriptRegistrant<z_Play_Cinematic_Console_Input_Custom> z_Play_Cinematic_Console_Input_Custom_Registrant("z_Play_Cinematic_Console_Input_Custom","Message:int,Script_Name:string,Location:vector3,Facing:float,Console_Command:string");
ScriptRegistrant<z_Destroy_Three_Objects_Custom> z_Destroy_Three_Objects_Custom_Registrant("z_Destroy_Three_Objects_Custom","Message:int,ID1:int,ID2:int,ID3:int");
ScriptRegistrant<z_RemoveWeaponModel> z_RemoveWeaponModel_Registrant("z_RemoveWeaponModel","Weapon_Model:string");
ScriptRegistrant<z_rwk> z_rwk_Registrant("z_rwk","");
ScriptRegistrant<z_GameoverCreated> z_GameoverCreated_Registrant("z_GameoverCreated","");
ScriptRegistrant<z_Console_Input_Custom_Delay_1> z_Console_Input_Custom_Delay_1_Registrant("z_Console_Input_Custom_Delay_1","Message:int,Input1:string,TimerNum:int,Time_Min:float,Time_Max:float,Repeat:int");
ScriptRegistrant<z_Console_Input_Custom_Delay_2> z_Console_Input_Custom_Delay_2_Registrant("z_Console_Input_Custom_Delay_2","Message:int,Input1:string,Input2:string,TimerNum:int,Time_Min:float,Time_Max:float,Repeat:int");
ScriptRegistrant<z_Created_Send_Custom_Param> z_Created_Send_Custom_Param_Registrant("z_Created_Send_Custom_Param","ID:int,Message:int,param:int");
ScriptRegistrant<z_NoDamageMoneyPoints> z_NoDamageMoneyPoints_Registrant("z_NoDamageMoneyPoints","");
ScriptRegistrant<z_Set_Team> z_Set_Team_Registrant("z_Set_Team","team:int");
ScriptRegistrant<z_Set_Skin_Created> z_Set_Skin_Created_Registrant("z_Set_Skin_Created","SkinType_Created=Blamo:string,Ammount=2500:float,Message=0:int,NewSkinType=CNCStructureHeavy:string,Remove_Script=z_NoDamageMoneyPoints:string");
ScriptRegistrant<z_Set_Skin> z_Set_Skin_Registrant("z_Set_Skin","SkinType=Blamo:string,Ammount=2500:float");
ScriptRegistrant<z_Death_Enable_Spawner> z_Death_Enable_Spawner_Registrant("z_Death_Enable_Spawner","ID:int,Enable:int");
ScriptRegistrant<z_Death_Enable_3Spawners> z_Death_Enable_3Spawners_Registrant("z_Death_Enable_3Spawners","ID1:int,Enable1:int,ID2:int,Enable2:int,ID3:int,Enable3:int");
ScriptRegistrant<z_UnkillableUntilEntered> z_UnkillableUntilEntered_Registrant("z_UnkillableUntilEntered","");
ScriptRegistrant<z_DestroyVeh259> z_DestroyVeh259_Registrant("z_DestroyVeh259","");
ScriptRegistrant<z_VehExit> z_VehExit_Registrant("z_VehExit","");
ScriptRegistrant<z_Spawn_When_Killed> z_Spawn_When_Killed_Registrant("z_Spawn_When_Killed","PresetName:string,DropHeight=0:float,SameFacing=1:int,SkinType=Blamo:string,Health=2500:float,Armor=2500:float");
ScriptRegistrant<z_Remove_Script_Custom> z_Remove_Script_Custom_Registrant("z_Remove_Script_Custom","Script=scriptname:string,Message=100:int");
ScriptRegistrant<z_Teleport_Powerup> z_Teleport_Powerup_Registrant("z_Teleport_Powerup","Location:vector3,Object_ID:int");
ScriptRegistrant<z_blamo4sec> z_blamo4sec_Registrant("z_blamo4sec","");
ScriptRegistrant<z_ChangeTeamPowerup> z_ChangeTeamPowerup_Registrant("z_ChangeTeamPowerup","");
ScriptRegistrant<z_MCV_Points> z_MCV_Points_Registrant("z_MCV_Points","Divisor=4.0:float");
ScriptRegistrant<z_S_HP1_Zone> z_S_HP1_Zone_Registrant("z_S_HP1_Zone","S_HP1_ID=1:int");
ScriptRegistrant<z_S_HP2_Zone> z_S_HP2_Zone_Registrant("z_S_HP2_Zone","S_HP2_ID=1:int");
ScriptRegistrant<z_A_HP1_Zone> z_A_HP1_Zone_Registrant("z_A_HP1_Zone","A_HP1_ID=1:int");
