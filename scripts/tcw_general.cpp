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
#include "engine.h"
#include "tcw_TCWSoldierObj.h"
#include "GameObjManager.h"

cTCWGameData FullTCWGameData;

void cTCWGameData::TCWLoadLevel()
{
	

	//TechInfo.Take_AllTech();
	//Enable_Snipers();
	//Enable_Commandos();
	//Enable_Shadows();
	//Set_CurrentSnipers(0);
}

void cTCWGameData::TCWObjectCreate(void *data, GameObject *obj)
{
	if(Commands->Is_A_Star(obj))
		Commands->Attach_Script(obj, "TCWSoldierObj", "");
}

cTCWGameData *TheTCWGame()
{
	return &FullTCWGameData;
}

/*
void cTCWGameData::Enable_Snipers()
{
	Hide_Preset_By_Name(1,"TCW_GDI_Sniper",false);
	Hide_Preset_By_Name(1,"TCW_GDI_Sniper_Low",false);
	SnipersAvailable = true;
}

void cTCWGameData::Disable_Snipers()
{
	Hide_Preset_By_Name(1,"TCW_GDI_Sniper",true);
	Hide_Preset_By_Name(1,"TCW_GDI_Sniper_Low",true);
	SnipersAvailable = false;
}

void cTCWGameData::Enable_Commandos()
{
	Hide_Preset_By_Name(1,"TCW_GDI_Commando",false);
	Hide_Preset_By_Name(1,"TCW_GDI_Commando_Low",false);
	Hide_Preset_By_Name(0,"TCW_Nod_Commando",false);
	Hide_Preset_By_Name(0,"TCW_Nod_Commando_Low",false);
	CommandoAvailable = true;
}

void cTCWGameData::Disable_Commandos()
{
	Hide_Preset_By_Name(1,"TCW_GDI_Commando",true);
	Hide_Preset_By_Name(1,"TCW_GDI_Commando_Low",true);
	Hide_Preset_By_Name(0,"TCW_Nod_Commando",true);
	Hide_Preset_By_Name(0,"TCW_Nod_Commando_Low",true);
	CommandoAvailable = false;
}

void cTCWGameData::Enable_Shadows()
{
	Hide_Preset_By_Name(0,"TCW_Nod_Shadow",false);
	Hide_Preset_By_Name(0,"TCW_Nod_Shadow_Low",false);
	ShadowsAvailable = true;
}

void cTCWGameData::Disable_Shadows()
{
	Hide_Preset_By_Name(0,"TCW_Nod_Shadow",true);
	Hide_Preset_By_Name(0,"TCW_Nod_Shadow_Low",true);
	ShadowsAvailable = false;
}
*/

/*
bool TechClass::Have_InfantryTech(int Team)
{
	if(Team == 0)
		return NOD_GotInfantryTech;
	else if(Team == 1)
		return GDI_GotInfantryTech;
	else
		return false;
}

bool TechClass::Have_VehicleTech(int Team)
{
	if(Team == 0)
		return NOD_GotVehicleTech;
	else if(Team == 1)
		return GDI_GotVehicleTech;
	else
		return false;
}

void TechClass::Give_InfantryTech(int Team)
{
	if(Team == 0)
	{
		Hide_Preset_By_Name(0,"TCW_GDI_Rifle_Squad",false);
		Hide_Preset_By_Name(0,"TCW_GDI_Engineer",false);
		Hide_Preset_By_Name(0,"TCW_GDI_Missile_Squad",false);
		Hide_Preset_By_Name(0,"TCW_GDI_Grenadier",false);
		Hide_Preset_By_Name(0,"TCW_GDI_Sniper",false);
		Hide_Preset_By_Name(0,"TCW_GDI_Zonetrooper",false);
		Hide_Preset_By_Name(0,"TCW_GDI_Commando",false);
		NOD_GotInfantryTech = true;
	}
	else if(Team == 1)
	{
		Hide_Preset_By_Name(1,"TCW_Nod_Militant_Infantry",false);
		Hide_Preset_By_Name(1,"TCW_Nod_Saboteur",false);
		Hide_Preset_By_Name(1,"TCW_Nod_RocketGuy",false);
		Hide_Preset_By_Name(1,"TCW_Nod_Shadow",false);
		Hide_Preset_By_Name(1,"TCW_Nod_Black_Hand_Flame",false);
		Hide_Preset_By_Name(1,"TCW_Nod_Commando",false);
		GDI_GotInfantryTech = true;
	}
}

void TechClass::Take_InfantryTech(int Team)
{
	if(Team == 0)
	{
		Hide_Preset_By_Name(0,"TCW_GDI_Rifle_Squad",true);
		Hide_Preset_By_Name(0,"TCW_GDI_Engineer",true);
		Hide_Preset_By_Name(0,"TCW_GDI_Missile_Squad",true);
		Hide_Preset_By_Name(0,"TCW_GDI_Grenadier",true);
		Hide_Preset_By_Name(0,"TCW_GDI_Sniper",true);
		Hide_Preset_By_Name(0,"TCW_GDI_Zonetrooper",true);
		Hide_Preset_By_Name(0,"TCW_GDI_Commando",true);
		NOD_GotInfantryTech = false;
	}
	else if(Team == 1)
	{
		Hide_Preset_By_Name(1,"TCW_Nod_Militant_Infantry",true);
		Hide_Preset_By_Name(1,"TCW_Nod_Saboteur",true);
		Hide_Preset_By_Name(1,"TCW_Nod_RocketGuy",true);
		Hide_Preset_By_Name(1,"TCW_Nod_Shadow",true);
		Hide_Preset_By_Name(1,"TCW_Nod_Black_Hand_Flame",true);
		Hide_Preset_By_Name(1,"TCW_Nod_Commando",true);
		GDI_GotInfantryTech = false;
	}
}

void TechClass::Give_VehicleTech(int Team)
{
	if(Team == 0)
	{
		Hide_Preset_By_Name(0,"TCW_Nod_Predator_Tank",false);
		Hide_Preset_By_Name(0,"TCW_Nod_Pitbull",false);
		Hide_Preset_By_Name(0,"TCW_Nod_GDI_Harvester_Player",false);
		Hide_Preset_By_Name(0,"TCW_Nod_APC",false);
		Hide_Preset_By_Name(0,"TCW_Nod_Mammoth_Tank",false);
		NOD_GotVehicleTech = true;
	}
	else if(Team == 1)
	{
		Hide_Preset_By_Name(1,"TCW_GDI_Attack_Bike",false);
		Hide_Preset_By_Name(1,"TCW_GDI_Flame_Tank",false);
		Hide_Preset_By_Name(1,"TCW_GDI_Nod_Harvester_Player",false);
		Hide_Preset_By_Name(1,"TCW_GDI_Raider_Buggy",false);
		Hide_Preset_By_Name(1,"TCW_GDI_Scorpion_Tank",false);
		Hide_Preset_By_Name(1,"TCW_GDI_Stealth_Tank",false);
		GDI_GotVehicleTech = true;
	}
}

void TechClass::Take_VehicleTech(int Team)
{
	if(Team == 0)
	{
		Hide_Preset_By_Name(0,"TCW_Nod_Predator_Tank",true);
		Hide_Preset_By_Name(0,"TCW_Nod_Pitbull",true);
		Hide_Preset_By_Name(0,"TCW_Nod_GDI_Harvester_Player",true);
		Hide_Preset_By_Name(0,"TCW_Nod_APC",true);
		Hide_Preset_By_Name(0,"TCW_Nod_Mammoth_Tank",true);
		NOD_GotVehicleTech = false;
	}
	else if(Team == 1)
	{
		Hide_Preset_By_Name(1,"TCW_GDI_Attack_Bike",true);
		Hide_Preset_By_Name(1,"TCW_GDI_Flame_Tank",true);
		Hide_Preset_By_Name(1,"TCW_GDI_Nod_Harvester_Player",true);
		Hide_Preset_By_Name(1,"TCW_GDI_Raider_Buggy",true);
		Hide_Preset_By_Name(1,"TCW_GDI_Scorpion_Tank",true);
		Hide_Preset_By_Name(1,"TCW_GDI_Stealth_Tank",true);
		GDI_GotVehicleTech = false;
	}
}

void TechClass::Take_AllTech()
{
	Take_InfantryTech(0);
	Take_InfantryTech(1);
	Take_VehicleTech(0);
	Take_VehicleTech(1);
}

void TechClass::Take_AllTech(int Team)
{
	Take_InfantryTech(Team);
	Take_VehicleTech(Team);
}
*/

void Console_InputF(const char *Format, ...)
{
	char buffer[256];
    va_list va;
    _crt_va_start(va, Format);
    vsnprintf(buffer, 256, Format, va);
    va_end(va);
    Console_Input(buffer);
}

const char *PackArg(const char *Format, ...)
{
	char buffer[256];
    va_list va;
    _crt_va_start(va, Format);
    vsnprintf(buffer, 256, Format, va);
    va_end(va);
	const char *Tmp = (const char *)buffer;
    return Tmp;
}