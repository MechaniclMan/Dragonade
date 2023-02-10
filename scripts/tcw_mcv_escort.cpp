/*	Renegade Scripts.dll
	Scripts by zunnie from - http://www.tiberiumcrystalwar.com
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
#include "tcw_mcv_escort.h"
#include "tcw_general.h"
#include "tcw_functions.h"
#include "VehicleGameObj.h"
#include "engine.h"
#include "GameObjManager.h"
#include "BuildingGameObj.h"

void tcw_mcv_spawnpad::Created(GameObject *obj)
{
	Commands->Attach_Script(obj,"M00_Damage_Modifier_DME","0.00,1,1,0,0");
	Commands->Set_Animation(obj,"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
}
void tcw_mcv_spawnpad::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Commands->Set_Animation(obj,"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false);
			Console_Input("amsg The MCV was successfully escorted to the GDI Base.");
			Commands->Start_Timer(obj,this,5.5f,100);
			Remove_Script(sender,"tcw_mcv_escort_mcv");//sender is wrong, should be the mcv not the sender
			//set skin of mcv to blamo
		}
	}
}
void tcw_mcv_spawnpad::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Console_Input("win 1");
	}
}

void tcw_mcv_endzone::Entered(GameObject *obj, GameObject *enter)
{
	if (Is_Script_Attached(enter,"tcw_mcv_escort_mcv"))
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_mcv_spawnpad"),1000,1,0);
		Commands->Destroy_Object(obj);
	}
}

void tcw_mcv_escort_mcv::Destroyed(GameObject *obj)
{
	Console_Input("amsg MCV destroyed. Nod wins.");
	Console_Input("win 0");
}

ScriptRegistrant<tcw_mcv_escort_mcv> tcw_mcv_escort_mcv_Registrant("tcw_mcv_escort_mcv","");
ScriptRegistrant<tcw_mcv_endzone> tcw_mcv_endzone_Registrant("tcw_mcv_endzone","");
ScriptRegistrant<tcw_mcv_spawnpad> tcw_mcv_spawnpad_Registrant("tcw_mcv_spawnpad","");