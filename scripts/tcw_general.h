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

#pragma once

#include "tcw_functions.h"

enum
{
	CUSTOM_EVENT_GREENTIB_ENTERED = 10500,
	CUSTOM_EVENT_BLUETIB_ENTERED = 10501,
	CUSTOM_EVENT_TIB_ENTERED = 10502,
	CUSTOM_EVENT_HARVY_LEFT_TIBFIELD = 10503,
	CUSTOM_EVENT_HARVY_DEPOSITING = 10504,

	CUSTOM_EVENT_CAPTURE_SPIKE = 10200,
	CUSTOM_EVENT_SPIKE_DIED = 10201,
	CUSTOM_EVENT_CAPTURE_SPIKE_START_NOD = 10202,
	CUSTOM_EVENT_CAPTURE_SPIKE_START_GDI = 10203,
	
};

enum
{
	TIBTYPE_GREEN = 10100,
	TIBTYPE_BLUE = 10101,
};

class TCW_Sound
{
public:
	TCW_Sound():Length(0),SoundName(""){}
	TCW_Sound(float SLength, const char *SName):Length(SLength),SoundName(SName){}
	float Length;
	const char *SoundName;
};

class TechClass
{
public:
	TechClass(){}//:GDI_GotInfantryTech(false),GDI_GotVehicleTech(false),NOD_GotInfantryTech(false),NOD_GotVehicleTech(false)
	//{
		//if (!Exe || Exe == 1)
		//	Take_AllTech();
	//}


	/*
	bool Have_InfantryTech(int Team);
	bool Have_VehicleTech(int Team);
	void Give_InfantryTech(int Team);
	void Take_InfantryTech(int Team);
	void Give_VehicleTech(int Team);
	void Take_VehicleTech(int Team);
	void Take_AllTech();
	void Take_AllTech(int Team);
	//void Give_CompositeArmour()
	*/
private:
	//bool GDI_GotInfantryTech;
	//bool GDI_GotVehicleTech;
	//bool NOD_GotInfantryTech;
	//bool NOD_GotVehicleTech;
	//bool CompositArmour;
};

class cTCWGameData
{
public:
	cTCWGameData():GiveCredSpikeCapture(true){};//:SnipersAvailable(true),CommandoAvailable(true),ShadowsAvailable(true),CurrentSnipers(0)
	~cTCWGameData(){}
	void TCWLoadLevel();
	void TCWObjectCreate(void *data, GameObject *obj);
	TechClass *Get_TechInfo(){return &TechInfo;}

	void Set_GiveCaptureCredits(bool Set){GiveCredSpikeCapture=Set;}
	bool Get_GiveCaptureCredits(){return GiveCredSpikeCapture;}

	/*
	void Enable_Snipers();
	void Enable_Commandos();
	void Enable_Shadows();
	void Disable_Commandos();
	void Disable_Snipers();
	void Disable_Shadows();
	bool IsCommandoAvailable(){return CommandoAvailable;}
	bool IsSnipersAvailable(){return SnipersAvailable;}
	bool IsShadowsAvailable(){return ShadowsAvailable;}
	void Register_Sniper(){CurrentSnipers++;}
	void Unregister_Sniper(){CurrentSnipers--;}
	int Get_CurrentSnipers(){return CurrentSnipers;}
	void Set_CurrentSnipers(int Set){CurrentSnipers=Set;}
	*/
private:
	TechClass TechInfo;
	bool GiveCredSpikeCapture;

	//bool SnipersAvailable;
	//bool CommandoAvailable;
//	bool ShadowsAvailable;
	//int CurrentSnipers;
};

cTCWGameData *TheTCWGame();

void Console_InputF(const char *Format, ...);
const char *PackArg(const char *Format, ...);