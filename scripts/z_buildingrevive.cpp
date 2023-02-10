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
#include "z_buildingrevive.h"
#include "BaseControllerClass.h"




void z_Custom_Restore_Building::Custom(GameObject *obj, int message, int param, GameObject *sender)//needed a script to test it :)
{
	if (message == Get_Int_Parameter("Message"))
	{
		if (param == 1)
		{
			Restore_Building(obj);
			Commands->Set_Health(obj,Commands->Get_Max_Health(obj));//you must set health when a building is restored
		}
	}
}

void z_Poke_Send_Custom::Poked(GameObject *obj, GameObject *poker)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
}









void z_APB_Building_Allied_Barracks_Buy::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	allowpoke = true;
	buildingcost = Get_Float_Parameter("BuildingCost");
	pokecost = Get_Float_Parameter("Poke_Cost");
	currentamount = 0;
}
void z_APB_Building_Allied_Barracks_Buy::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD))
		{
			if (!Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
			{
				if (Find_Building_By_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY))
				{
					if (!Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY)))
					{
						//lolalive
					}
					else
					{
						float pokermoney = Commands->Get_Money(poker);
						if (pokermoney < pokecost)
						{
							Send_Message_Player(poker,255,255,255,"asdf");
						}
						else 
						{
							Commands->Give_Money(poker,-pokecost,false);
							currentamount = currentamount + pokecost;
							if (currentamount == buildingcost)
							{
								GameObject *abar = Find_Building_By_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY);
								Restore_Building(abar);
								Commands->Set_Health(abar,Commands->Get_Max_Health(obj));
								Commands->Create_2D_WAV_Sound("eva_newconst.wav");
								Send_Message_Team(1,255,255,255,"New Construction Options");
							}
						}
					}
				}
			}
			else
			{
				Send_Message_Player(poker,255,255,255,"Access Denied. Your Construction Yard is dead.");
				Create_2D_WAV_Sound_Player(poker,"accessdenied.wav");
			}
		}
		else
		{
			//no cy avail
		}
		allowpoke = false;
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Commands->Start_Timer(obj,this,2.0f,5000);
	}
}
void z_APB_Building_Allied_Barracks_Buy::Timer_Expired(GameObject *obj, int number)
{
	if (number == 4000)
	{
		//
	}
	else if (number == 5000)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj,true);
	}
}

void z_Poke_Restore_Building::Poked(GameObject *obj, GameObject *poker)
{
	int type = Get_Int_Parameter("Type");
	int team = Get_Int_Parameter("Team");
	if (type == 1)
	{
		GameObject *bing = Find_Building_By_Type(team,BuildingConstants::TYPE_BASE_DEFENSE);
		if (bing)
		{
			Restore_Building(bing);
			Commands->Set_Health(bing,Commands->Get_Max_Health(obj));
		}
	}
	else if (type == 2)
	{
		GameObject *bing = Find_Building_By_Type(team,BuildingConstants::TYPE_COM_CENTER);
		if (bing)
		{
			Restore_Building(bing);
			Commands->Set_Health(bing,Commands->Get_Max_Health(obj));
		}
	}
	else if (type == 3)
	{
		GameObject *bing = Find_Building_By_Type(team,BuildingConstants::TYPE_CONYARD);
		if (bing)
		{
			Restore_Building(bing);
			Commands->Set_Health(bing,Commands->Get_Max_Health(obj));
		}
	}
	else if (type == 4)
	{
		GameObject *bing = Find_Building_By_Type(team,BuildingConstants::TYPE_HELIPAD);
		if (bing)
		{
			Restore_Building(bing);
			Commands->Set_Health(bing,Commands->Get_Max_Health(obj));
		}
	}
	else if (type == 5)
	{
		GameObject *bing = Find_Building_By_Type(team,BuildingConstants::TYPE_POWER_PLANT);
		if (bing)
		{
			Restore_Building(bing);
			Commands->Set_Health(bing,Commands->Get_Max_Health(obj));
		}
	}
	else if (type == 6)
	{
		GameObject *bing = Find_Building_By_Type(team,BuildingConstants::TYPE_REFINERY);
		if (bing)
		{
			Restore_Building(bing);
			Commands->Set_Health(bing,Commands->Get_Max_Health(obj));
		}
	}
	else if (type == 7)
	{
		GameObject *bing = Find_Building_By_Type(team,BuildingConstants::TYPE_SOLDIER_FACTORY);
		if (bing)
		{
			Restore_Building(bing);
			Commands->Set_Health(bing,Commands->Get_Max_Health(obj));
		}
	}
	else if (type == 8)
	{
		GameObject *bing = Find_Building_By_Type(team,BuildingConstants::TYPE_VEHICLE_FACTORY);
		if (bing)
		{
			Restore_Building(bing);
			Commands->Set_Health(bing,Commands->Get_Max_Health(obj));
		}
	}
	else if (type == 9)
	{
		GameObject *bing = Find_Building_By_Preset(team,Get_Parameter("BuildingPreset"));
		if (bing)
		{
			Restore_Building(bing);
			Commands->Set_Health(bing,Commands->Get_Max_Health(obj));
		}
	}
}


ScriptRegistrant<z_Poke_Restore_Building> z_Poke_Restore_Building_Registrant("z_Poke_Restore_Building","Team=1:int,Type=1:int,BuildingPreset=bla:string");
ScriptRegistrant<z_Poke_Send_Custom> z_Poke_Send_Custom_Registrant("z_Poke_Send_Custom","ID=1:int,Message=1000:int,Param=1:int,Delay=1:float");
ScriptRegistrant<z_Custom_Restore_Building> z_Custom_Restore_Building_Registrant("z_Custom_Restore_Building","Message=1000:int");