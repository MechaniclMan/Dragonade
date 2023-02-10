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
#include "z_zones.h"
#include "BaseControllerClass.h"
#include "ScriptableGameObj.h"

void z_Sell_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	//first check if its a coop map that must have selling disabled always
	if (_stricmp(The_Game()->Get_Map_Name(),"RA_MCV_Escort.mix") == 0)
	{
		//do nothing cos lolnotallowed
	}
	else
	{
		
		int Player_Type = Get_Int_Parameter("Player_Type");
		if (Find_Building_By_Type(Player_Type,BuildingConstants::TYPE_REPAIR_BAY) && !Is_Building_Dead(Find_Building_By_Type(Player_Type,BuildingConstants::TYPE_REPAIR_BAY)))
		{
			if (CheckPlayerType(enterer,Player_Type))
			{
				return;
			}
			if (!Commands->Find_Object(ID))
			{
				RemoveHook();
				ID = 0;
			}
			if ((ID) && (ID != Commands->Get_ID(enterer)))
			{
				return;
			}
			if (enterer->As_VehicleGameObj())
			{
				Create_2D_Sound_Player(Get_Vehicle_Driver(enterer),Get_Parameter("Sound"));
				InstallHook("Sell",Get_Vehicle_Driver(enterer));
				ID = Commands->Get_ID(enterer);
			}
		}
		else
		{
			//do nothing
		}
	}
}
void z_Sell_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if (ID == Commands->Get_ID(exiter))
	{
		RemoveHook();
		ID = 0;
	}
}
void z_Sell_Zone::KeyHook()
{
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if (Commands->Find_Object(ID)->As_VehicleGameObj())
	{
		if (Get_Vehicle_Driver(Commands->Find_Object(ID)))
		{
			Create_2D_Sound_Player(Get_Vehicle_Driver(Commands->Find_Object(ID)),Get_Parameter("SellSound"));
			const char *preset = Commands->Get_Preset_Name(Commands->Find_Object(ID));
			float cost = ((float)Get_Cost(preset) / 2);
			Commands->Give_Money(Get_Vehicle_Driver(Commands->Find_Object(ID)),cost,false);
			Force_Occupants_Exit(Commands->Find_Object(ID));
			Commands->Destroy_Object(Commands->Find_Object(ID));
			ID = 0;
			RemoveHook();
		}
	}
}
void z_Sell_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ID,4,1);
}
void z_Sell_Zone::Created(GameObject *obj)
{
	ID = 0;
}


void z_Aircraft_Refill::Created(GameObject *obj)
{
	active = false;
}

void z_Aircraft_Refill::Entered(GameObject *obj, GameObject *enterer)
{
	if (Find_Building_By_Type(Get_Int_Parameter("TeamSelection"),BuildingConstants::TYPE_HELIPAD) && !Is_Building_Dead(Find_Building_By_Type(Get_Int_Parameter("TeamSelection"),BuildingConstants::TYPE_HELIPAD)))
	{
		if (Commands->Get_Player_Type(enterer) == Get_Int_Parameter("TeamSelection"))
		{
			if (!active)
			{
				GameObject *vobj = 0;
				if (enterer->As_VehicleGameObj())
				{
					vobj = enterer;
				}
				if (Get_Vehicle(enterer))
				{
					vobj = Get_Vehicle(enterer);
				}
				if (vobj)
				{
					if (Get_Vehicle_Mode(vobj) == VEHICLE_TYPE_FLYING)
					{
						Commands->Start_Timer(obj,this,Get_Float_Parameter("Reload_Interval"),Commands->Get_ID(vobj));
						active = true;
					}
				}
			}
		}
	}
	else
	{
		//do nothing lol
	}
}
void z_Aircraft_Refill::Timer_Expired(GameObject *obj, int number)
{
	active = false;
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
			if (Get_Vehicle_Mode(object) == VEHICLE_TYPE_FLYING && Get_Vehicle_Driver(object))
			{
				Commands->Give_PowerUp(object,Get_Parameter("Reload_Powerup"),false);
				Commands->Start_Timer(obj, this, Get_Float_Parameter("Reload_Interval"), number);
			}
		}
	}
}


void z_Repair_Zone_No_Boats::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	//if (!Is_Building_Dead(Find_Building_By_Type(Get_Int_Parameter("TeamSelection"),BuildingConstants::TYPE_HELIPAD)))
	if (Find_Building_By_Preset(Player_Type,"mp_Allied_Naval_Yard") && !Is_Building_Dead(Find_Building_By_Preset(Player_Type,"mp_Allied_Naval_Yard")))
	{
		if (CheckPlayerType(enterer,Player_Type))
		{
			return;
		}
		if (!Commands->Find_Object(ID))
		{
			RemoveHook();
			ID = 0;
		}
		if ((ID) && (ID != Commands->Get_ID(enterer)))
		{
			return;
		}
		if (enterer->As_VehicleGameObj() && !Is_Script_Attached(enterer,"JFW_Boat"))
		{
			Create_2D_Sound_Player(Get_Vehicle_Driver(enterer),Get_Parameter("Sound"));
			InstallHook("Repair",Get_Vehicle_Driver(enterer));
			ID = Commands->Get_ID(enterer);
		}
	}
	else
	{
		//do nothing
	}
}
void z_Repair_Zone_No_Boats::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	if (!Commands->Find_Object(ID))
	{
		ID = 0;
		return;
	}
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Repair_Distance"))
	{
		if (Get_Vehicle_Driver(Commands->Find_Object(number)))
		{
			float cost = (Get_Cost(Commands->Get_Preset_Name(Commands->Find_Object(number))) / 2) / Get_Float_Parameter("Credits");
			if (Commands->Get_Money(Get_Vehicle_Driver(Commands->Find_Object(number))) >= cost)
			{
				float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
				float Health = Commands->Get_Health(Commands->Find_Object(number));
				float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
				float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
				bool repair = false;
				if (Health < Max_Health)
				{
					Commands->Set_Health(Commands->Find_Object(number),Health + Max_Health / Get_Float_Parameter("Repair_Health"));
					repair = true;
				}
				if (Shield_Strength < Max_Shield_Strength)
				{
					Commands->Set_Shield_Strength(Commands->Find_Object(number),Shield_Strength + Max_Shield_Strength / Get_Float_Parameter("Repair_Health"));
					repair = true;
				}
				if (repair)
				{
					Commands->Give_Money(Get_Vehicle_Driver(Commands->Find_Object(number)),-cost,false);
					Commands->Start_Timer(obj,this,1.0,number);
					return;
				}
			}
		}
	}
	Create_2D_Sound_Player(Get_Vehicle_Driver(Commands->Find_Object(ID)),Get_Parameter("RepairStopSound"));
	ID = 0;
}
void z_Repair_Zone_No_Boats::Exited(GameObject *obj,GameObject *exiter)
{
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if (ID == Commands->Get_ID(exiter))
	{
		RemoveHook();
		ID = 0;
	}
}
void z_Repair_Zone_No_Boats::KeyHook()
{
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if (Get_Vehicle_Driver(Commands->Find_Object(ID)))
	{
		Commands->Start_Timer(Owner(),this,1.0,ID);
		Create_2D_Sound_Player(Get_Vehicle_Driver(Commands->Find_Object(ID)),Get_Parameter("RepairSound"));
		RemoveHook();
	}
}
void z_Repair_Zone_No_Boats::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ID,4,1);
}
void z_Repair_Zone_No_Boats::Created(GameObject *obj)
{
	ID = 0;
}


void z_Helipad_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	if (Find_Building_By_Type(Get_Int_Parameter("TeamSelection"),BuildingConstants::TYPE_HELIPAD) && !Is_Building_Dead(Find_Building_By_Type(Get_Int_Parameter("TeamSelection"),BuildingConstants::TYPE_HELIPAD)))
	{
		if (Commands->Get_Player_Type(enterer) == Get_Int_Parameter("TeamSelection"))
		{
			Commands->Start_Timer(obj, this, Get_Float_Parameter("Reload_Interval"), Commands->Get_ID(enterer));
		}
	}
	else
	{
		//nothing
	}
}
void z_Helipad_Zone::Timer_Expired(GameObject *obj, int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if ((distance <= 10.0) && (Get_Vehicle_Driver(Commands->Find_Object(number))))
	{
		if (Commands->Get_Preset_ID(Commands->Find_Object(number)) == Get_Int_Parameter("Apache_Preset_ID"))
		{
			Commands->Give_PowerUp(Commands->Find_Object(number),Get_Parameter("Apache_Reload_Powerup"),false);
		}
		else if (Commands->Get_Preset_ID(Commands->Find_Object(number)) == Get_Int_Parameter("Hind_Preset_ID"))
		{
			Commands->Give_PowerUp(Commands->Find_Object(number),Get_Parameter("Hind_Reload_Powerup"),false);
		}
		Commands->Start_Timer(obj, this, Get_Float_Parameter("Reload_Interval"), number);
	}
}


void z_Ore_Delivery_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	if (Is_Script_Attached(enterer, "RA_Ore_Truck")) //Is this an Ore Truck?
	{
		if (Find_Building_By_Type(Get_Int_Parameter("Team"),BuildingConstants::TYPE_REFINERY) && !Is_Building_Dead(Find_Building_By_Type(Get_Int_Parameter("Team"),BuildingConstants::TYPE_REFINERY)))
		{
			if (Commands->Get_Player_Type(enterer) == Get_Int_Parameter("Team")) //Is it a friendly truck?
			{
				Commands->Send_Custom_Event(obj, enterer, Get_Int_Parameter("Enter"), 0, 0);
			}
		}
	}
}


void z_Repair_Zone_Boats::Entered(GameObject *obj,GameObject *enterer)
{
	int Player_Type = Get_Int_Parameter("Player_Type");
	if (CheckPlayerType(enterer,Player_Type))
	{
		return;
	}
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if ((ID) && (ID != Commands->Get_ID(enterer)))
	{
		return;
	}
	if (enterer->As_VehicleGameObj() && Is_Script_Attached(enterer,"JFW_Boat"))
	{
		Create_2D_Sound_Player(Get_Vehicle_Driver(enterer),Get_Parameter("Sound"));
		InstallHook("Repair",Get_Vehicle_Driver(enterer));
		ID = Commands->Get_ID(enterer);
	}
}

void z_Repair_Zone_Boats::Timer_Expired(GameObject *obj,int number)
{
	Vector3 pos1;
	Vector3 pos2;
	float distance;
	if (!Commands->Find_Object(ID))
	{
		ID = 0;
		return;
	}
	pos1 = Commands->Get_Position(obj);
	pos2 = Commands->Get_Position(Commands->Find_Object(number));
	distance = Commands->Get_Distance(pos1,pos2);
	if (distance <= Get_Float_Parameter("Repair_Distance"))
	{
		if (Get_Vehicle_Driver(Commands->Find_Object(number)))
		{
			float cost = (Get_Cost(Commands->Get_Preset_Name(Commands->Find_Object(number))) / 2) / Get_Float_Parameter("Credits");
			if (Commands->Get_Money(Get_Vehicle_Driver(Commands->Find_Object(number))) >= cost)
			{
				float Max_Health = Commands->Get_Max_Health(Commands->Find_Object(number));
				float Health = Commands->Get_Health(Commands->Find_Object(number));
				float Max_Shield_Strength = Commands->Get_Max_Shield_Strength(Commands->Find_Object(number));
				float Shield_Strength = Commands->Get_Shield_Strength(Commands->Find_Object(number));
				bool repair = false;
				if (Health < Max_Health)
				{
					Commands->Set_Health(Commands->Find_Object(number),Health + Max_Health / Get_Float_Parameter("Repair_Health"));
					repair = true;
				}
				if (Shield_Strength < Max_Shield_Strength)
				{
					Commands->Set_Shield_Strength(Commands->Find_Object(number),Shield_Strength + Max_Shield_Strength / Get_Float_Parameter("Repair_Health"));
					repair = true;
				}
				if (repair)
				{
					Commands->Give_Money(Get_Vehicle_Driver(Commands->Find_Object(number)),-cost,false);
					Commands->Start_Timer(obj,this,1.0,number);
					return;
				}
			}
		}
	}
	Create_2D_Sound_Player(Get_Vehicle_Driver(Commands->Find_Object(ID)),Get_Parameter("RepairStopSound"));
	ID = 0;
}

void z_Repair_Zone_Boats::Exited(GameObject *obj,GameObject *exiter)
{
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if (ID == Commands->Get_ID(exiter))
	{
		RemoveHook();
		ID = 0;
	}
}

void z_Repair_Zone_Boats::KeyHook()
{
	if (!Commands->Find_Object(ID))
	{
		RemoveHook();
		ID = 0;
	}
	if (Get_Vehicle_Driver(Commands->Find_Object(ID)))
	{
		Commands->Start_Timer(Owner(),this,1.0,ID);
		Create_2D_Sound_Player(Get_Vehicle_Driver(Commands->Find_Object(ID)),Get_Parameter("RepairSound"));
		RemoveHook();
	}
}

void z_Repair_Zone_Boats::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ID,4,1);
}

void z_Repair_Zone_Boats::Created(GameObject *obj)
{
	ID = 0;
}

void z_Zone_Steal_Tech::Created(GameObject *obj)
{
	trigger = true;
	enterguy = 0;
	team = Get_Int_Parameter("Player_Type");

	char* presets = newstr(Get_Parameter("Hide_Presets_Team"));
	char* token = strtok(presets, ";");
	while (token != NULL)
	{
		Hide_Preset_By_Name(team, token, true);
		token = strtok (NULL, ";");
	}
	delete[] presets;
}
void z_Zone_Steal_Tech::Entered(GameObject *obj, GameObject *enterer)
{
	if (Commands->Is_A_Star(enterer))
	{
		if (Commands->Get_Player_Type(enterer) == team)
		{
			if (trigger)
			{
				if (Is_Script_Attached(enterer,"z_I_Can_Steal_Tech"))
				{
					enterguy = Commands->Get_ID(enterer);
					trigger = false;
					char themsg[220];
					sprintf(themsg,"%s",Get_Parameter("Steal_Begin_Message"));
					Send_Message(Get_Int_Parameter("StealBeginMSG_Red"),Get_Int_Parameter("StealBeginMSG_Green"),Get_Int_Parameter("StealBeginMSG_Blue"),themsg);
					Commands->Create_2D_WAV_Sound(Get_Parameter("Steal_Begin_Sound"));
					Commands->Start_Timer(obj,this,Get_Float_Parameter("Steal_Time"),Get_Int_Parameter("Timernumber"));
				}
			}
		}
	}
}
void z_Zone_Steal_Tech::Timer_Expired(GameObject *obj, int number)
{
	if (number == Get_Int_Parameter("Timernumber"))
	{
		if (Commands->Find_Object(enterguy))
		{
			char themsg[220];
			sprintf(themsg,"%s",Get_Parameter("Steal_Complete_Message"));
			Send_Message(Get_Int_Parameter("StealCompleteMSG_Red"),Get_Int_Parameter("StealCompleteMSG_Green"),Get_Int_Parameter("StealCompleteMSG_Blue"),themsg);
			Commands->Create_2D_WAV_Sound(Get_Parameter("Steal_Complete_Sound"));

			char* presets = newstr(Get_Parameter("Stolen_Tech_Presets"));
			char* token = strtok(presets, ";");
			while (token != NULL)
			{
				Hide_Preset_By_Name(team, token, false);
				token = strtok (NULL, ";");
			}
			delete[] presets;
			Destroy_Script();
		}
		trigger = true;
	}
}
void z_Zone_Steal_Tech::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&trigger,5,1);
	Auto_Save_Variable(&enterguy,5,1);
	Auto_Save_Variable(&team,5,1);
}



ScriptRegistrant<z_Zone_Steal_Tech> z_Zone_Steal_Tech_Registrant("z_Zone_Steal_Tech","Player_Type=1:int,Hide_Presets=asdf:string,Steal_Begin_Message=StealingInProgress:string,StealBeginMSG_Red=255:int,StealBeginMSG_Green=255:int,StealBeginMSG_Blue=255:int,Steal_Begin_Sound=bla:string,Steal_Time=0:float,Timernumber=677:int,Steal_Complete_Message=StealingCompleted:string,StealCompleteMSG_Red=255:int,StealCompleteMSG_Green=255:int,StealCompleteMSG_Blue=255:int,Steal_Complete_Sound=bla:string,Stolen_Tech_Presets=blabla:string");
ScriptRegistrant<z_Repair_Zone_Boats> z_Repair_Zone_Boats_Registrant("z_Repair_Zone_Boats","Player_Type:int,Repair_Distance:float,Repair_Health:float,Sound:string,Credits:float,RepairSound:string,RepairStopSound:string");
ScriptRegistrant<z_Ore_Delivery_Zone> z_Ore_Delivery_Zone_Registrant("z_Ore_Delivery_Zone", "Team=0:int,Enter=321654");
ScriptRegistrant<z_Helipad_Zone> z_Helipad_Zone_Registrant("z_Helipad_Zone", "TeamSelection=0:int,Reload_Interval=2.00:float,Apache_Preset_ID=0:int,Apache_Reload_Powerup=Blank:string,Hind_Preset_ID=0:int,Hind_Reload_Powerup=Blank:string");
ScriptRegistrant<z_Repair_Zone_No_Boats> z_Repair_Zone_No_Boats_Registrant("z_Repair_Zone_No_Boats","Player_Type:int,Repair_Distance:float,Repair_Health:float,Sound:string,Credits:float,RepairSound:string,RepairStopSound:string");
ScriptRegistrant<z_Aircraft_Refill> z_Aircraft_Refill_Registrant("z_Aircraft_Refill","TeamSelection:int,Reload_Interval:float,Reload_Powerup:string");
ScriptRegistrant<z_Sell_Zone> z_Sell_Zone_Registrant("z_Sell_Zone","Player_Type:int,Sound:string,SellSound:string");