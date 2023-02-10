/*	Renegade Scripts.dll
	Copyright 2012-2013 http://www.UltraAOW.COM by zunnie
	For the map C&C_SnowWarfare by Mauler

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
#include "z_snowwarfare.h"
#ifdef SSGM
#include "gmgame.h"
#endif

void z_sw_randomchar::Created(GameObject *obj)
{
	if ( (strcmp(Commands->Get_Preset_Name(obj),"GDI_Spawn") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"NOD_Spawn") == 0) )
	{
		int random = Commands->Get_Random_Int(1,20);
		int team = Commands->Get_Player_Type(obj);
		if (team == 0)
		{
			if (random == 2) Change_Character(obj,"MAU_Nod_Engineer_0");
			else if (random == 3) Change_Character(obj,"MAU_Nod_FlameThrower_2SF");
			else if (random == 4) Change_Character(obj,"MAU_Nod_FlameThrower_1Off");
			else if (random == 5) Change_Character(obj,"MAU_Nod_FlameThrower_2SF");
			else if (random == 6) Change_Character(obj,"MAU_Nod_FlameThrower_3Boss");
			else if (random == 7) Change_Character(obj,"MAU_Nod_Minigunner_1Off");
			else if (random == 8) Change_Character(obj,"MAU_Nod_Minigunner_1Off");
			else if (random == 9) Change_Character(obj,"MAU_Nod_Minigunner_2SF");
			else if (random == 10) Change_Character(obj,"MAU_Nod_Minigunner_3Boss");
			else if (random == 11) Change_Character(obj,"MAU_Nod_RocketSoldier_1Off");
			else if (random == 12) Change_Character(obj,"MAU_Nod_RocketSoldier_1Off");
			else if (random == 13) Change_Character(obj,"MAU_Nod_RocketSoldier_2SF");
			else if (random == 14) Change_Character(obj,"MAU_Nod_RocketSoldier_3Boss");
			else if (random == 15) Change_Character(obj,"MAU_Nod_FlameThrower_2SF");
			else if (random == 16) Change_Character(obj,"MAU_Nod_Minigunner_1Off");
			else if (random == 17) Change_Character(obj,"MAU_Nod_RocketSoldier_1Off");
			else if (random == 18) Change_Character(obj,"MAU_Nod_RocketSoldier_2SF");
			else Change_Character(obj,"MAU_Nod_Technician_0");
		}
		else if (team == 1)
		{
			if (random == 2) Change_Character(obj,"MAU_GDI_Engineer_0");
			else if (random == 3) Change_Character(obj,"MAU_Sydney");
			else if (random == 4) Change_Character(obj,"MAU_GDI_Grenadier_2SF");
			else if (random == 5) Change_Character(obj,"MAU_Ignatio_Mobius");
			else if (random == 6) Change_Character(obj,"MAU_GDI_MiniGunner_1Off");
			else if (random == 7) Change_Character(obj,"MAU_GDI_MiniGunner_1Off");
			else if (random == 8) Change_Character(obj,"MAU_GDI_MiniGunner_2SF");
			else if (random == 9) Change_Character(obj,"MAU_GDI_MiniGunner_3Boss");
			else if (random == 10) Change_Character(obj,"MAU_GDI_RocketSoldier_1Off");
			else if (random == 11) Change_Character(obj,"MAU_GDI_RocketSoldier_1Off");
			else if (random == 12) Change_Character(obj,"MAU_GDI_RocketSoldier_2SF");
			else if (random == 13) Change_Character(obj,"MAU_Sydney_PowerSuit_ALT2");
			else if (random == 14) Change_Character(obj,"MAU_Sydney");
			else if (random == 15) Change_Character(obj,"MAU_GDI_MiniGunner_1Off");
			else if (random == 16) Change_Character(obj,"MAU_GDI_MiniGunner_2SF");
			else if (random == 17) Change_Character(obj,"MAU_GDI_RocketSoldier_2SF");
			else if (random == 18) Change_Character(obj,"MAU_GDI_Grenadier_2SF");
			else Change_Character(obj,"MAU_GDI_Engineer_2SF");
		}
	}
}

void z_sw_dom_node::Created(GameObject *obj)
{
	Commands->Set_Animation(obj,"MAU_CP.MAU_CP",false,0,2,2,false);
	nodename = Get_Parameter("Node_Name");
	owner = -2;
	trigger_neutral_damage = true;
}
void z_sw_dom_node::Damaged(GameObject *obj, GameObject *damager, float damage)
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

				Commands->Set_Animation(obj,"MAU_CP.MAU_CP",false,0,0,0,false);

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

				Commands->Set_Animation(obj,"MAU_CP.MAU_CP",false,0,1,1,false);

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
				Commands->Set_Animation(obj,"MAU_CP.MAU_CP",false,0,2,2,false);

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

ScriptRegistrant<z_sw_dom_node> z_sw_dom_node_Registrant("z_sw_dom_node","Node_Name=North:string");
ScriptRegistrant<z_sw_randomchar> z_sw_randomchar_Registrant("z_sw_randomchar","");