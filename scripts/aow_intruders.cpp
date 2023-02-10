/*	Renegade Scripts.dll
	Copyright 2012 UltraAOW.COM

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.

	Useful information for the AOW_Building_Camera scripts usage:
	------------------------------------------------------------------------------------------------------------------------
	Building Infiltrated Sounds:
	------------------------------------------------------------------------------------------------------------------------
	AGT Intruder: 		m00bgat_dsgn0005i1evag_snd.wav		Warning! Intruder detected in GDI Advanced Guard Tower
	AGT Infiltrated: 	m00bgat_dsgn0006i1evan_snd.wav		GDI Advanced Guard Tower infiltrated
	BAR Intruder:		m00bgib_dsgn0006i1evag_snd.wav		Warning! Intruder detected in GDI Infantry Barracks
	BAR Infiltrated:	m00bgib_dsgn0007i1evan_snd.wav		GDI Infantry Barracks infiltrated
	GPP Intruder:		m00bgpp_dsgn0007i1evag_snd.wav		Warning! Intruder detected in GDI Power Plant
	GPP Infiltrated:	m00bgpp_dsgn0008i1evan_snd.wav		GDI Power Plant infiltrated
	GRF Intruder:		m00bgtr_dsgn0007i1evag_snd.wav		Warning! Intruder detected in GDI Tiberium Refinery
	GRF Infiltrated:	m00bgtr_dsgn0008i1evan_snd.wav		GDI Tiberium Refinery infiltrated
	GWF Intruder:		m00bgwf_dsgn0007i1evag_snd.wav		Warning! Intruder detected in GDI Weapons Factory
	GWF Infiltrated:	m00bgwf_dsgn0008i1evan_snd.wav		GDI Weapons Factory infiltrated
	------------------------------------------------------------------------------------------------------------------------
	AIR Intruder:		m00bnaf_dsgn0008i1evag_snd.wav		Nod Airstrip Control Tower infiltrated
	AIR Infiltrated:	m00bnaf_dsgn0008i1evag_snd.wav		Nod Airstrip Control Tower infiltrated
	HON Intruder:		m00bnhn_dsgn0013i1evan_snd.wav		Warning! Intruder detected in Hand of Nod
	HON Infiltrated:	m00bnhn_dsgn0014i1evag_snd.wav		Hand of Nod infiltrated
	OBI Intruder:		m00bnol_dsgn0005i1evan_snd.wav		Warning! Intruder detected in Nod Obelisk
	OBI Infiltrated:	m00bnol_dsgn0006i1evag_snd.wav		Nod Obelisk infiltrated
	NPP Intruder:		m00bnpp_dsgn0007i1evan_snd.wav		Warning! Intruder detected in Nod Power Plant
	NPP Infiltrated:	m00bnpp_dsgn0008i1evag_snd.wav		Nod Power Plant infiltrated
	NRF Intruder:		m00bntr_dsgn0007i1evan_snd.wav		Warning! Intruder detected in Nod Tiberium Refinery
	NRF Infiltrated:	m00bntr_dsgn0008i1evag_snd.wav		Nod Tiberium Refinery infiltrated
	------------------------------------------------------------------------------------------------------------------------
	------------------------------------------------------------------------------------------------------------------------
*/

#include "General.h"
#include "scripts.h"
#include "engine.h"
#include "BuildingGameObj.h"
#include "SmartGameObj.h"
#include "aow_intruders.h"
#ifdef SSGM
#include "gmgame.h"
#endif

void AOW_Building_Camera::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	Seen = false;
	Commands->Enable_Enemy_Seen(obj, true);
}
void AOW_Building_Camera::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if(!Seen && obj->As_DamageableGameObj()->Is_Enemy(enemy->As_DamageableGameObj()) && Commands->Get_Distance(Commands->Get_Position(obj), Commands->Get_Position(enemy)) <= Get_Float_Parameter("Range"))
	{
		if (strcmp(Commands->Get_Preset_Name(enemy),"CnC_Nod_FlameThrower_2SF") != 0)
		{
			if (team == 0)
			{
				char infiltrated[250];		sprintf(infiltrated,"%s",Get_Parameter("Infiltrated_Message"));		Send_Message_Team(1,255,204,0,infiltrated);
				char infsnd[250];			sprintf(infsnd,"%s",Get_Parameter("Infiltrated_Sound"));			Create_2D_WAV_Sound_Team(infsnd,1);
				char intruder[250];			sprintf(intruder,"%s",Get_Parameter("Intruder_Message"));			Send_Message_Team(0,255,0,0,intruder);
				char intsnd[250];			sprintf(intsnd,"%s",Get_Parameter("Intruder_Sound"));				Create_2D_WAV_Sound_Team(intsnd,0);
			}
			else if (team == 1)
			{
				char infiltrated[250];		sprintf(infiltrated,"%s",Get_Parameter("Infiltrated_Message"));		Send_Message_Team(0,255,0,0,infiltrated);
				char infsnd[250];			sprintf(infsnd,"%s",Get_Parameter("Infiltrated_Sound"));			Create_2D_WAV_Sound_Team(infsnd,0);
				char intruder[250];			sprintf(intruder,"%s",Get_Parameter("Intruder_Message"));			Send_Message_Team(1,255,204,0,intruder);
				char intsnd[250];			sprintf(intsnd,"%s",Get_Parameter("Intruder_Sound"));				Create_2D_WAV_Sound_Team(intsnd,1);			
			}

			Seen = true;
			Commands->Start_Timer(obj,this,30.0f,12341);
		}
	}
}
void AOW_Building_Camera::Timer_Expired(GameObject *obj, int number)
{
	if(number == 12341)
		Seen = false;
}
void AOW_Building_Camera::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&team,sizeof(team),1);
	Auto_Save_Variable(&Seen,sizeof(Seen),2);
}


void AOW_Set_Max_Health_Armor_Created::Created(GameObject *obj)
{
	const char *newhp = Get_Parameter("Health_Type");
	const char *newap = Get_Parameter("Armor_Type");
	float maxhp = Get_Float_Parameter("Max_Health");
	float maxap = Get_Float_Parameter("Max_Armor");
	Set_Skin(obj,newhp);
	Commands->Set_Shield_Type(obj,newap);
	Set_Max_Shield_Strength(obj,maxap);
	Set_Max_Health(obj,maxhp);
	Commands->Set_Shield_Strength(obj,maxap);
	Commands->Set_Health(obj,maxhp);
}

void AOW_Set_Team::Created(GameObject *obj)
{
	int team = Get_Int_Parameter("Team");
	Commands->Set_Player_Type(obj,team);
}


ScriptRegistrant<AOW_Set_Team> AOW_Set_Team_Created_Registrant("AOW_Set_Team","Team=0:int");
ScriptRegistrant<AOW_Set_Max_Health_Armor_Created> AOW_Set_Max_Health_Armor_Created_Registrant("AOW_Set_Max_Health_Armor_Created","Health_Type=SkinFlesh:string,Armor_Type=ShieldKevlar:string,Max_Health=25000:float,Max_Armor=10000:float");
ScriptRegistrant<AOW_Building_Camera> AOW_Building_Camera_Registrant("AOW_Building_Camera","Team=0:int,Infiltrated_Message=0:string,Infiltrated_Sound=0:string,Intruder_Message=0:string,Intruder_Sound=0:string,Range=5000:float");

