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
#include "engine_tt.h"
#include "engine.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"
#include "z_cold_fever.h"
#include "BaseControllerClass.h"
#include "GameObjManager.h"


void z_Cold_Closest_Star::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,5.0f,100);
}
void z_Cold_Closest_Star::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		if (!Find_Object_By_Preset(1,"CnC_GDI_Engineer_0"))
		{
			GameObject *engi = Commands->Get_A_Star(Commands->Get_Position(obj));
			{
				Change_Character(engi,"CnC_GDI_Engineer_0");
			}
		}
		else
		{
			Commands->Start_Timer(obj,this,5.0f,100);
		}
	}
}



ScriptRegistrant<z_Cold_Closest_Star> z_Cold_Closest_Star_Registrant("z_Cold_Closest_Star","");