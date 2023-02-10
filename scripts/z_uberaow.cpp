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
#include "engine.h"
#include "BuildingGameObj.h"
#include "z_uberaow.h"

void z_Player::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000) //obigun pickup timer
	{
		Commands->Start_Timer(obj,this,300.0f,100);
	}
}

void z_Player::Killed(GameObject *obj, GameObject *shooter)
{
	if (strcmp(Commands->Get_Preset_Name(shooter), "CnC_Visceroid") == 0)
	{
		char mesg[250];
		sprintf(mesg,"%s got ownt by a Tiberium Monster",Get_Player_Name(obj));
		Send_Message(255,255,255,mesg);
	}
	if ( (strcmp(Commands->Get_Preset_Name(obj), "CnC_GDI_Engineer_2SF") == 0) || 
		 (strcmp(Commands->Get_Preset_Name(obj), "GDI_Engineer_2SF") == 0) || 
		 (strcmp(Commands->Get_Preset_Name(obj), "CnC_Sydney_PowerSuit") == 0) ||  
		 (strcmp(Commands->Get_Preset_Name(obj), "CnC_Sydney_PowerSuit_ALT2") == 0) ||  
		 (strcmp(Commands->Get_Preset_Name(obj), "CnC_Nod_Minigunner_3Boss") == 0) ||
		 (strcmp(Commands->Get_Preset_Name(obj), "CnC_Nod_MiniGunner_3Boss_ALT2") == 0) ||
		 (strcmp(Commands->Get_Preset_Name(obj), "CnC_Sydney") == 0) )
	{
		Commands->Create_2D_WAV_Sound("female_death01.wav");
	}
}

void z_Player::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Remove_Weapon(obj,"Weapon_Obelisk");
		Send_Message_Player(obj,104,234,40,"Sorry the Obelisk Weapon was removed due to the time restriction");
	}
}

void z_Tiberium_Truck::Created(GameObject *obj)
{
	isharvesting = false;
	hastiberium = false;
	driverid = 0;
	team = 0;
	plid = 0;
}
void z_Tiberium_Truck::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			if (!isharvesting)
			{
				Commands->Start_Timer(obj,this,10.0f,100);
				isharvesting = true;
			}
		}
	}
	else if (message == 1001)
	{
		if (param == 1)
		{
			isharvesting = false;
		}
	}
	else if (message == 1002)
	{
		if (param == 1)
		{
			if (hastiberium)
			{
				if (Commands->Get_Player_Type(obj) == 0)
				{
					if (Find_Building_By_Type(0,BuildingConstants::TYPE_REFINERY) && Commands->Get_Health(Find_Building_By_Type(0,BuildingConstants::TYPE_REFINERY)) > 1.0f)
					{
						Commands->Give_Money(obj,500.0f,true);
						Commands->Give_Points(Get_GameObj(plid),50.0f,false);
						Create_2D_WAV_Sound_Team("m00pc$$_aqob0002i1evag_snd.wav",0);
						char msg[250]; sprintf(msg,"%s unloaded Tiberium funding Nod with $%g",Get_Player_Name(Commands->Find_Object(driverid)),500.0f);
						Send_Message_Team(0,255,0,0,msg);
						hastiberium = false;
						isharvesting = false;
					}
				}
				else if (Commands->Get_Player_Type(obj) == 1)
				{
					if (Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY) && Commands->Get_Health(Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY)) > 1.0f)
					{
						Commands->Give_Money(obj,500.0f,true);
						Commands->Give_Points(Get_GameObj(plid),50.0f,false);
						Create_2D_WAV_Sound_Team("m00pc$$_aqob0002i1evag_snd.wav",1);
						char msg2[250]; sprintf(msg2,"%s unloaded Tiberium funding GDI with $%g",Get_Player_Name(Commands->Find_Object(driverid)),500.0f);
						Send_Message_Team(1,255,204,0,msg2);
						hastiberium = false;
						isharvesting = false;
					}
				}
			}
		}
	}
	else if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		team = Commands->Get_Player_Type(sender);
		driverid = Commands->Get_ID(sender);
		plid = Get_Player_ID(sender);
		Send_Message_Player(Commands->Find_Object(driverid),104,234,40,"Goto the Tiberium Field and harvest Tiberium, then return to the Refinery to unload.");
		Send_Message_Player(Commands->Find_Object(driverid),104,234,40,"You will be sent a message and sound when harvesting is complete.");
	}
	else if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		team = 0;
		driverid = 0;
		plid = 0;
	}
}

void z_Tiberium_Truck::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		if (isharvesting)
		{
			hastiberium = true;
			isharvesting = false;
			Create_2D_WAV_Sound_Player(Commands->Find_Object(driverid),"00-n006e.wav");
			Send_Message_Player(Commands->Find_Object(driverid),104,234,40,"HARVESTING COMPLETE: Return to the Refinery to unload.");
		}
	}
}
void z_Tiberium_Truck::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&team,sizeof(team),1);
	Auto_Save_Variable(&isharvesting,sizeof(isharvesting),2);
	Auto_Save_Variable(&hastiberium,sizeof(hastiberium),3);
	Auto_Save_Variable(&driverid,sizeof(driverid),4);
	Auto_Save_Variable(&plid,sizeof(plid),5);
}

void z_Tiberium_Field::Entered(GameObject *obj, GameObject *enter)
{
	if (Is_Script_Attached(enter,"z_Tiberium_Truck"))
	{
		Commands->Send_Custom_Event(obj,enter,1000,1,0);//harveting started
	}
}
void z_Tiberium_Field::Exited(GameObject *obj, GameObject *exit)
{
	if (Is_Script_Attached(exit,"z_Tiberium_Truck"))
	{
		Commands->Send_Custom_Event(obj,exit,1001,1,0);//not harvesting anymore
	}
}
void z_Tiberium_Deposit::Entered(GameObject *obj, GameObject *enter)
{
	if (Is_Script_Attached(enter,"z_Tiberium_Truck"))
	{
		Commands->Send_Custom_Event(obj,enter,1002,1,0);//dump
	}	
}

void z_TiberiumTruck_Buy::Created(GameObject *obj)
{
	Attach_Script_Once(obj,"M00_Damage_Modifier_DME","0.00f,1,1,0,0");
	currenttruck = 0;
	allowpoke = true;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void z_TiberiumTruck_Buy::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		allowpoke = false;
		Commands->Start_Timer(obj,this,5.0f,100);
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		if (!Commands->Find_Object(currenttruck))
		{
			if (Commands->Get_Money(poker) >= 250.0f)
			{
				GameObject *truck;
				Vector3 pos = Get_Vector3_Parameter("Location");
				truck = Commands->Create_Object("CnC_Nod_Truck_Player_Secret",pos);
				Commands->Set_Facing(truck,Get_Float_Parameter("Facing"));
				Attach_Script_Once(truck,"z_Tiberium_Truck","");
				currenttruck = Commands->Get_ID(truck);
				Commands->Give_Money(poker,-250.0f,false);
			}
			else if (Commands->Get_Money(poker) < 250.0f) 
			{
				Create_2D_WAV_Sound_Player(poker,"m00evag_dsgn0028i1evag_snd.wav");
				Send_Message_Player(poker,104,234,40,"Insufficient Funds.");
			}
		}
		else if (Commands->Find_Object(currenttruck))
		{
			Create_2D_WAV_Sound_Player(poker,"m00evag_dsgn0039i1evag_snd.wav");
			Send_Message_Player(poker,104,234,40,"Access denied.");
			Send_Message_Player(poker,104,234,40,"Someone else already has the Tiberium Truck, try again later.");
		}
	}
}
void z_TiberiumTruck_Buy::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Enable_HUD_Pokable_Indicator(obj,true);
		allowpoke = true;
	}
}
void z_TiberiumTruck_Buy::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&currenttruck,sizeof(currenttruck),1);
}

void z_POW_ObiGun::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message	== CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Send_Message_Player(sender,104,234,40,"You lucky ass you got the Obelisk Weapon!");
		Commands->Send_Custom_Event(obj,sender,1000,1,1);//start obi revoke timer
	}
}

ScriptRegistrant<z_POW_ObiGun> z_POW_ObiGun_Registrant("z_POW_ObiGun","");
ScriptRegistrant<z_Player> z_Player_Registrant("z_Player","");
ScriptRegistrant<z_Tiberium_Truck> z_Tiberium_Truck_Registrant("z_Tiberium_Truck","");
ScriptRegistrant<z_Tiberium_Field> z_Tiberium_Field_Registrant("z_Tiberium_Field","");
ScriptRegistrant<z_Tiberium_Deposit> z_Tiberium_Deposit_Registrant("z_Tiberium_Deposit","");
ScriptRegistrant<z_TiberiumTruck_Buy> z_TiberiumTruck_Buy_Registrant("z_TiberiumTruck_Buy","Location:vector3,Facing:float");