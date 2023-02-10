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
#include "tcw_zone.h"
#include "GameObjManager.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"

void tcw_GDI_Zone_Aircraft_Only::Entered(GameObject *obj,GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		if (Commands->Get_Player_Type(enter) == 1)
		{
			if (Is_VTOL(enter))
			{
				GameObject *vobj = 0;
				if (enter && enter->As_VehicleGameObj())
				{
					vobj = Get_Vehicle(enter);
				}
				if (Get_Vehicle(enter))
				{
					vobj = Get_Vehicle(enter);
				}
				if (vobj)
				{
					Commands->Start_Timer(obj,this,1.0f,Commands->Get_ID(vobj));
				}
			}
		}
	}
}
void tcw_GDI_Zone_Aircraft_Only::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;	
	Vector3 pos2;
	float distance;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= 10.0f)
	{
		float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
		float Health = Commands->Get_Health(Commands->Find_Object(number));
		float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
		float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
		if (Health < Max_Health)
		{
			Commands->Set_Health(Commands->Find_Object(number),Health + 25.0f);
		}
		if (Shield_Strength < Max_Shield_Strength)
		{
			Commands->Set_Shield_Strength(Commands->Find_Object(number),Shield_Strength + 25.0f);
		}

		//check if they are orca or firehawk and give powerups
		if ( (strcmp(Commands->Get_Preset_Name(Commands->Find_Object(number)),"TCW_GDI_Orca") == 0) || (strcmp(Commands->Get_Preset_Name(Commands->Find_Object(number)),"TCW_Paul_Orca") == 0) )
		{
			Commands->Give_PowerUp(Commands->Find_Object(number),"TCW_POW_Orca_Rocket",true);
		}
		else if (strcmp(Commands->Get_Preset_Name(Commands->Find_Object(number)),"TCW_GDI_Firehawk") == 0)
		{
			Commands->Give_PowerUp(Commands->Find_Object(number),"TCW_POW_Firehawk_Rocket",true);
		}

		Commands->Start_Timer(obj,this,1.0f,number);
	}
}

void tcw_zone_kill_soldier_stars::Entered(GameObject *obj, GameObject *enter)
{
	if (enter->As_SoldierGameObj() && Commands->Is_A_Star(enter))
	{
		Commands->Apply_Damage(enter,99999.0f,"Death",enter);
	}
}

ScriptRegistrant<tcw_zone_kill_soldier_stars> tcw_zone_kill_soldier_stars_Registrant("tcw_zone_kill_soldier_stars","");
ScriptRegistrant<tcw_GDI_Zone_Aircraft_Only> tcw_GDI_Zone_Aircraft_Only_Registrant("tcw_GDI_Zone_Aircraft_Only","");