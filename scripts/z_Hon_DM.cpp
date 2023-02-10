/*	Renegade Scripts.dll
	Copyright 2012-2013 http://www.UltraAOW.COM by zunnie

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
#include "z_Hon_DM.h"
#ifdef SSGM
#include "gmgame.h"
#endif

void z_hon_randomchar::Created(GameObject *obj)
{
	if ( (strcmp(Commands->Get_Preset_Name(obj),"GDI_Spawn") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"NOD_Spawn") == 0) )
	{
		int random = Commands->Get_Random_Int(1,20);
		int team = Commands->Get_Player_Type(obj);
		if (team == 0)
		{
			if (random == 2) Change_Character(obj,"CnC_Nod_Engineer_0");
			else if (random == 3) Change_Character(obj,"CnC_Nod_FlameThrower_2SF");
			else if (random == 4) Change_Character(obj,"CnC_Nod_FlameThrower_1Off");
			else if (random == 5) Change_Character(obj,"CnC_Nod_FlameThrower_2SF");
			else if (random == 6) Change_Character(obj,"CnC_Nod_FlameThrower_3Boss");
			else if (random == 7) Change_Character(obj,"CnC_Nod_Minigunner_1Off");
			else if (random == 8) Change_Character(obj,"CnC_Nod_Minigunner_1Off");
			else if (random == 9) Change_Character(obj,"CnC_Nod_Minigunner_2SF");
			else if (random == 10) Change_Character(obj,"CnC_Nod_Minigunner_3Boss");
			else if (random == 11) Change_Character(obj,"CnC_Nod_RocketSoldier_1Off");
			else if (random == 12) Change_Character(obj,"CnC_Nod_RocketSoldier_1Off");
			else if (random == 13) Change_Character(obj,"CnC_Nod_RocketSoldier_2SF");
			else if (random == 14) Change_Character(obj,"CnC_Nod_RocketSoldier_3Boss");
			else if (random == 15) Change_Character(obj,"CnC_Nod_FlameThrower_2SF");
			else if (random == 16) Change_Character(obj,"CnC_Nod_Minigunner_1Off");
			else if (random == 17) Change_Character(obj,"CnC_Nod_RocketSoldier_1Off");
			else if (random == 18) Change_Character(obj,"CnC_Nod_RocketSoldier_2SF");
			else Change_Character(obj,"CnC_Nod_Technician_0");
		}
		else if (team == 1)
		{
			if (random == 2) Change_Character(obj,"CnC_GDI_Engineer_0");
			else if (random == 3) Change_Character(obj,"CnC_Sydney");
			else if (random == 4) Change_Character(obj,"CnC_GDI_Grenadier_2SF");
			else if (random == 5) Change_Character(obj,"CnC_Ignatio_Mobius");
			else if (random == 6) Change_Character(obj,"CnC_GDI_MiniGunner_1Off");
			else if (random == 7) Change_Character(obj,"CnC_GDI_MiniGunner_1Off");
			else if (random == 8) Change_Character(obj,"CnC_GDI_MiniGunner_2SF");
			else if (random == 9) Change_Character(obj,"CnC_GDI_MiniGunner_3Boss");
			else if (random == 10) Change_Character(obj,"CnC_GDI_RocketSoldier_1Off");
			else if (random == 11) Change_Character(obj,"CnC_GDI_RocketSoldier_1Off");
			else if (random == 12) Change_Character(obj,"CnC_GDI_RocketSoldier_2SF");
			else if (random == 13) Change_Character(obj,"CnC_Sydney_PowerSuit_ALT2");
			else if (random == 14) Change_Character(obj,"CnC_Sydney");
			else if (random == 15) Change_Character(obj,"CnC_GDI_MiniGunner_1Off");
			else if (random == 16) Change_Character(obj,"CnC_GDI_MiniGunner_2SF");
			else if (random == 17) Change_Character(obj,"CnC_GDI_RocketSoldier_2SF");
			else if (random == 18) Change_Character(obj,"CnC_GDI_Grenadier_2SF");
			else Change_Character(obj,"CnC_GDI_Engineer_2SF");
		}
	}
}

//Domination Type Scripts Below:
void z_hon_dom_control::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,40.0f,123439348);//spam score
	Commands->Start_Timer(obj,this,30.0f,123123123);//spam todo
	triggerendmsg = true;
	points_gdi = 0;		points_nod = 0;
	points_max = Get_Int_Parameter("Points_Max");
}
void z_hon_dom_control::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 0)
		{
			
			if (triggerendmsg)
			{
				points_nod += 1;
				if (points_nod >= points_max)
				{
					triggerendmsg = false;
					char nodwins[250];
					sprintf(nodwins,"amsg Nod has won the game with a %d point lead (%d/%d)",points_nod-points_gdi,points_nod,points_gdi);
					Console_Input(nodwins);		Console_Input("win 0");
				}
			}
		}
		if (param == 1)
		{
			if (triggerendmsg)
			{
				points_gdi += 1;
				if (points_gdi >= points_max)
				{
					triggerendmsg = false;
					char gdiwins[250];
					sprintf(gdiwins,"amsg GDI has won the game with a %d point lead (%d/%d)",points_gdi-points_nod,points_gdi,points_nod);
					Console_Input(gdiwins);		Console_Input("win 1");
				}
			}
		}
	}
}
void z_hon_dom_control::Timer_Expired(GameObject *obj, int number)
{
	if (number == 123439348)
	{
		if (points_gdi > points_nod)
		{
			char status[250]; char lead[250];
			sprintf(status,"msg GDI: %d points -- Nod: %d points -- Required: %d points",points_gdi,points_nod,points_max);
			sprintf(lead,"msg GDI leads the game with a %d points lead",points_gdi-points_nod);
			Console_Input(status);
			Console_Input(lead);
		}
		else if (points_nod > points_gdi)
		{
			char status[250]; char lead[250];
			sprintf(status,"msg GDI: %d points -- Nod: %d points -- Required: %d points",points_gdi,points_nod,points_max);
			sprintf(lead,"msg Nod leads the game with a %d points lead",points_nod-points_gdi);
			Console_Input(status);
			Console_Input(lead);
		}
		else
		{
			char status[250];
			sprintf(status,"msg GDI: %d points ----- Nod: %d points",points_gdi,points_nod);
			Console_Input(status);
		}
		Commands->Start_Timer(obj,this,60.0f,123439348);
	}
	else if (number == 123123123)
	{
		Console_Input("msg Capture Control Points by Repairing them and gain 1 Domination Point per second per Control Point.");
		char required[512];		sprintf(required,"msg Control Point Capture Points Required to Win: %d",points_max);		Console_Input(required);
		Commands->Start_Timer(obj,this,72.0f,123123123);
	}
}

void z_hon_dom_pointgen::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	Commands->Start_Timer(obj,this,1.0f,303);
}
void z_hon_dom_pointgen::Timer_Expired(GameObject *obj, int number)
{
	if (number == 303)
	{
		if (team == 0)
		{
			Commands->Send_Custom_Event(obj,Find_Object_With_Script("z_hon_dom_control"),1000,0,0);
		}
		else if (team == 1)
		{
			Commands->Send_Custom_Event(obj,Find_Object_With_Script("z_hon_dom_control"),1000,1,0);
		}
		Commands->Start_Timer(obj,this,1.0f,303);
	}
}

void z_hon_dom_node::Created(GameObject *obj)
{
	Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);
	nodename = Get_Parameter("Node_Name");
	owner = -2;
	trigger_neutral_damage = true;
}
void z_hon_dom_node::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if ((damage < 0.0f) && (Commands->Get_Player_Type(obj) == -2))
	{
		if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))//if node fully repaired
		{
			trigger_neutral_damage = true;
			int team = Commands->Get_Player_Type(damager);
			if (team == 0)
			{
				char params[250]; sprintf(params,"%d",team);
				Attach_Script_Once(obj,"z_hon_dom_pointgen",params);
				owner = 0;
				Commands->Set_Player_Type(obj,0);

				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,0,0,false);

				char capturemsg[250];	sprintf(capturemsg,"Nod captured the %s Control Point.",nodename);	Send_Message(255,255,255,capturemsg);
				Create_2D_WAV_Sound_Team("ControlPointCaptured_Nod.wav",0); 
				if (Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
				{
					Commands->Give_Points(damager,500.0f,false);
				}
			}
			else if (team == 1)
			{
				char params[250]; sprintf(params,"%d",team);
				Attach_Script_Once(obj,"z_hon_dom_pointgen",params);
				owner = 1;
				Commands->Set_Player_Type(obj,1);

				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,1,1,false);

				char capturemsg[250];	sprintf(capturemsg,"GDI captured the %s Control Point.",nodename);	Send_Message(255,255,255,capturemsg);
				Create_2D_WAV_Sound_Team("ControlPointCaptured_GDI.wav",1); 
				if (Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
				{
					Commands->Give_Points(damager,500.0f,false);
				}
			}
		}
	}
	else if (damage > 0.0f)//it is being attacked by enemy
	{
		if (damage > Commands->Get_Health(obj))//if node fully destroyed
		{
			Commands->Set_Health(obj,1.1f);
			Commands->Set_Player_Type(obj,-2);
			if (trigger_neutral_damage)
			{
				trigger_neutral_damage = false;
				Remove_Script(obj,"z_hon_dom_pointgen");
				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);

				if (owner == 0)
				{
					Create_2D_WAV_Sound_Team("ControlPointLost_Nod.wav",0);
					char lostmsg[250];	sprintf(lostmsg,"Control Point %s lost.",nodename);	Send_Message_Team(0,255,255,255,lostmsg);
				}
				else if (owner == 1)
				{
					Create_2D_WAV_Sound_Team("ControlPointLost_GDI.wav",1);
					char lostmsg[250];	sprintf(lostmsg,"Control Point %s lost.",nodename);	Send_Message_Team(1,255,255,255,lostmsg);
				}
				owner = -2;
			}
		}
	}
}

ScriptRegistrant<z_hon_dom_node> z_hon_dom_node_Registrant("z_hon_dom_node","Node_Name=North:string");
ScriptRegistrant<z_hon_dom_pointgen> z_hon_dom_pointgen_Registrant("z_hon_dom_pointgen","Team=0:int");
ScriptRegistrant<z_hon_dom_control> z_hon_dom_control_Registrant("z_hon_dom_control","Points_Max=10:int");
ScriptRegistrant<z_hon_randomchar> z_hon_randomchar_Registrant("z_hon_randomchar","");