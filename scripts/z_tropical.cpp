/*	Renegade Scripts.dll
	Copyright 2012 UltraAOW.COM

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
#include "z_tropical.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"
#ifdef SSGM
#include "gmgame.h"
#endif

void z_Trop_Vehicle::Created(GameObject *obj)
{
	if (strcmp(Commands->Get_Preset_Name(obj),"Trop_GDI_Orca") == 0)
	{
		Send_Message_Team(1,255,204,0,"GDI Orca has spawned on the Aircraft Carrier");
		Commands->Start_Timer(obj,this,5.0f,250500);
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"Trop_Nod_Apache") == 0)
	{
		Send_Message_Team(0,255,0,0,"Nod Apache has spawned on the Aircraft Carrier");
		Commands->Start_Timer(obj,this,5.0f,250500);
	}
}
void z_Trop_Vehicle::Killed(GameObject *obj, GameObject *shooter)
{
	if (Commands->Get_Player_Type(obj) == 0)
	{
#ifdef SSGM
		char killmsg[250];
		sprintf(killmsg,"Nod unit destroyed. (%s)",Get_Translated_Preset_Name_Ex(obj));
		Send_Message_Team(0,255,0,0,killmsg);
#endif
		Create_2D_WAV_Sound_Team("noddead1.wav",0);
	}
	else if (Commands->Get_Player_Type(obj) == 1)
	{
#ifdef SSGM
		char killmsg[250];
		sprintf(killmsg,"GDI unit destroyed. (%s)",Get_Translated_Preset_Name_Ex(obj));
		Send_Message_Team(1,255,204,0,killmsg);
#endif
		Create_2D_WAV_Sound_Team("gdidead1.wav",1);
	}
}
void z_Trop_Vehicle::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if ( (strcmp(Commands->Get_Preset_Name(obj),"Trop_GDI_Orca") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Trop_Nod_Apache") == 0) )
		{
			if ( Has_Weapon(sender,"Weapon_IonCannonBeacon_Player") || Has_Weapon(sender,"CnC_Weapon_IonCannonBeacon_Player") || 
				 Has_Weapon(sender,"Weapon_NukeBeacon_Player") || Has_Weapon(sender,"CnC_Weapon_NukeBeacon_Player") )
			{
				Remove_Weapon(sender,"Weapon_IonCannonBeacon_Player"); Remove_Weapon(sender,"CnC_Weapon_IonCannonBeacon_Player");
				Remove_Weapon(sender,"Weapon_NukeBeacon_Player"); Remove_Weapon(sender,"CnC_Weapon_NukeBeacon_Player");
				Commands->Give_Money(sender,1000.0f,false);
			}
		}
	}
}
void z_Trop_Vehicle::Timer_Expired(GameObject *obj, int number)
{
	if (number == 250500)
	{
		Vector3 curpos = Commands->Get_Position(obj);
		if (curpos.Z <= -17.308f)
		{
			Commands->Apply_Damage(obj,99999.0f,"Death",0);//killkillkill
		}
		else
		{
			Commands->Start_Timer(obj,this,5.0f,250500);
		}
	}
}

//0=nod 1=gdi 2=neu current animations for spawnpads
void z_Trop_Carrier_Control_Point::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,180.0f,100100);//start spawncheck for vehicles for nod
	Commands->Start_Timer(obj,this,180.0f,100101);//start spawncheck for vehicles for gdi
	Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);
	nodename = Get_Parameter("Node_Name");
	owner = -2;
	carrierid = 152489;
	trigger_neutral_damage = true;
	gdi_orca = "Trop_GDI_Orca";	nod_apache = "Trop_Nod_Apache";
	Spawn_Facing = 0.0f;
	Spawn_Pos1.X = -523.352f;	Spawn_Pos1.Y = 39.289f;	Spawn_Pos1.Z = 20.649f;
	Spawn_Pos2.X = -492.604f;	Spawn_Pos2.Y = 39.289f;	Spawn_Pos2.Z = 20.649f;
	Spawn_Pos3.X = -523.352f;	Spawn_Pos3.Y = 11.659f;	Spawn_Pos3.Z = 20.649f;
	Spawn_Pos4.X = -492.604f;	Spawn_Pos4.Y = 11.659f;	Spawn_Pos4.Z = 20.649f;
	nodveh1id = 0;	nodveh2id = 0;	nodveh3id = 0;	nodveh4id = 0;
	gdiveh1id = 0;	gdiveh2id = 0;	gdiveh3id = 0;	gdiveh4id = 0;
	nodveh1 = 0;	nodveh2 = 0;	nodveh3 = 0;	nodveh4 = 0;
	gdiveh1 = 0;	gdiveh2 = 0;	gdiveh3 = 0;	gdiveh4 = 0;
}
void z_Trop_Carrier_Control_Point::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (damage < 0.0f)
	{
		if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))
		{
			trigger_neutral_damage = true;
			if (Commands->Get_Player_Type(obj) == -2)
			{
				int team = Commands->Get_Player_Type(damager);
				if (team == 0)
				{
					Commands->Send_Custom_Event(obj,Commands->Find_Object(100252),1000,0,0);//refilzones reset
					Commands->Send_Custom_Event(obj,Commands->Find_Object(100253),1000,0,0);
					Commands->Send_Custom_Event(obj,Commands->Find_Object(100254),1000,0,0);
					Commands->Send_Custom_Event(obj,Commands->Find_Object(100255),1000,0,0);

					owner = 0;
					Commands->Set_Player_Type(obj,0);
					Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,0,0,false);

					nodveh1 = Commands->Create_Object(nod_apache,Spawn_Pos1);
					Commands->Set_Facing(nodveh1,Spawn_Facing);
					nodveh1id = Commands->Get_ID(nodveh1);

					nodveh2 = Commands->Create_Object(nod_apache,Spawn_Pos2);
					Commands->Set_Facing(nodveh2,Spawn_Facing);
					nodveh2id = Commands->Get_ID(nodveh2);

					nodveh3 = Commands->Create_Object(nod_apache,Spawn_Pos3);
					Commands->Set_Facing(nodveh3,Spawn_Facing);
					nodveh3id = Commands->Get_ID(nodveh3);

					nodveh4 = Commands->Create_Object(nod_apache,Spawn_Pos4);
					Commands->Set_Facing(nodveh4,Spawn_Facing);
					nodveh4id = Commands->Get_ID(nodveh4);

					char capturemsg[250];	sprintf(capturemsg,"Nod captured the %s Control Point.",nodename);	Send_Message(255,255,255,capturemsg);
					Create_2D_WAV_Sound_Team("ControlPointCaptured_Nod.wav",0); 
				}
				else if (team == 1)
				{
					Commands->Send_Custom_Event(obj,Commands->Find_Object(100252),1000,1,0);//refilzones reset
					Commands->Send_Custom_Event(obj,Commands->Find_Object(100253),1000,1,0);
					Commands->Send_Custom_Event(obj,Commands->Find_Object(100254),1000,1,0);
					Commands->Send_Custom_Event(obj,Commands->Find_Object(100255),1000,1,0);

					owner = 1;
					Commands->Set_Player_Type(obj,1);
					Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,1,1,false);

					GameObject *gdiveh1 = Commands->Create_Object(gdi_orca,Spawn_Pos1);
					Commands->Set_Facing(gdiveh1,Spawn_Facing);
					gdiveh1id = Commands->Get_ID(gdiveh1);

					GameObject *gdiveh2 = Commands->Create_Object(gdi_orca,Spawn_Pos2);
					Commands->Set_Facing(gdiveh2,Spawn_Facing);
					gdiveh2id = Commands->Get_ID(gdiveh2);

					GameObject *gdiveh3 = Commands->Create_Object(gdi_orca,Spawn_Pos3);
					Commands->Set_Facing(gdiveh3,Spawn_Facing);
					gdiveh3id = Commands->Get_ID(gdiveh3);

					GameObject *gdiveh4 = Commands->Create_Object(gdi_orca,Spawn_Pos4);
					Commands->Set_Facing(gdiveh4,Spawn_Facing);
					gdiveh4id = Commands->Get_ID(gdiveh4);

					char capturemsg[250];	sprintf(capturemsg,"GDI captured the %s Control Point.",nodename);	Send_Message(255,255,255,capturemsg);
					Create_2D_WAV_Sound_Team("ControlPointCaptured_GDI.wav",1);
				}
			}
		}
	}
	else if (damage > 0.0f)
	{
		if (damage > Commands->Get_Health(obj))
		{
			Commands->Set_Health(obj,1.1f);
			Commands->Set_Player_Type(obj,-2);
			if (trigger_neutral_damage)
			{
				trigger_neutral_damage = false;
				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100252),1000,2,0);//refilzones reset
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100253),1000,2,0);
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100254),1000,2,0);
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100255),1000,2,0);

				if (owner == 0)
				{
					Create_2D_WAV_Sound_Team("ControlPointLost_Nod.wav",0);
					if ( (Commands->Find_Object(nodveh1id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh1id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh1id),99999.0f,"Steel",obj);		nodveh1id = 0;
					}
					if ( (Commands->Find_Object(nodveh2id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh2id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh2id),99999.0f,"Steel",obj);		nodveh2id = 0;
					}
					if ( (Commands->Find_Object(nodveh3id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh3id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh3id),99999.0f,"Steel",obj);		nodveh3id = 0;
					}
					if ( (Commands->Find_Object(nodveh4id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh4id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh4id),99999.0f,"Steel",obj);		nodveh4id = 0;
					}
				}
				else if (owner == 1)
				{
					Create_2D_WAV_Sound_Team("ControlPointLost_GDI.wav",1);
					if ( (Commands->Find_Object(gdiveh1id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh1id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh1id),99999.0f,"Steel",obj);		gdiveh1id = 0;
					}
					if ( (Commands->Find_Object(gdiveh2id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh2id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh2id),99999.0f,"Steel",obj);		gdiveh2id = 0;
					}
					if ( (Commands->Find_Object(gdiveh3id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh3id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh3id),99999.0f,"Steel",obj);		gdiveh3id = 0;
					}
					if ( (Commands->Find_Object(gdiveh4id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh4id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh4id),99999.0f,"Steel",obj);		gdiveh4id = 0;
					}
				}
				owner = -2;
			}
		}
	}
}
void z_Trop_Carrier_Control_Point::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100100) // spawn vehicles check nod
	{
		if (owner == 0)//if this point is still controlled by nod proceed
		{
			if (!Commands->Find_Object(nodveh1id))
			{
				nodveh1 = Commands->Create_Object(nod_apache,Spawn_Pos1);
				Commands->Set_Facing(nodveh1,Spawn_Facing);
				nodveh1id = Commands->Get_ID(nodveh1);
			}
			if (!Commands->Find_Object(nodveh2id))
			{
				nodveh2 = Commands->Create_Object(nod_apache,Spawn_Pos2);
				Commands->Set_Facing(nodveh2,Spawn_Facing);
				nodveh2id = Commands->Get_ID(nodveh2);
			}
			if (!Commands->Find_Object(nodveh3id))
			{
				nodveh3 = Commands->Create_Object(nod_apache,Spawn_Pos3);
				Commands->Set_Facing(nodveh3,Spawn_Facing);
				nodveh3id = Commands->Get_ID(nodveh3);
			}
			if (!Commands->Find_Object(nodveh4id))
			{
				nodveh4 = Commands->Create_Object(nod_apache,Spawn_Pos4);
				Commands->Set_Facing(nodveh4,Spawn_Facing);
				nodveh4id = Commands->Get_ID(nodveh4);
			}
		}
		Commands->Start_Timer(obj,this,180.0f,100100);//loop nod check
	}
	else if (number == 100101) // spawn vehicles check gdi
	{
		if (owner == 1)//if this point is still controlled by gdi proceed
		{
			if (!Commands->Find_Object(gdiveh1id))
			{
				GameObject *gdiveh1 = Commands->Create_Object(gdi_orca,Spawn_Pos1);
				Commands->Set_Facing(gdiveh1,Spawn_Facing);
				gdiveh1id = Commands->Get_ID(gdiveh1);
			}
			if (!Commands->Find_Object(gdiveh2id))
			{
				GameObject *gdiveh2 = Commands->Create_Object(gdi_orca,Spawn_Pos2);
				Commands->Set_Facing(gdiveh2,Spawn_Facing);
				gdiveh2id = Commands->Get_ID(gdiveh2);
			}
			if (!Commands->Find_Object(gdiveh3id))
			{
				GameObject *gdiveh3 = Commands->Create_Object(gdi_orca,Spawn_Pos3);
				Commands->Set_Facing(gdiveh3,Spawn_Facing);
				gdiveh3id = Commands->Get_ID(gdiveh3);
			}
			if (!Commands->Find_Object(gdiveh4id))
			{
				GameObject *gdiveh4 = Commands->Create_Object(gdi_orca,Spawn_Pos4);
				Commands->Set_Facing(gdiveh4,Spawn_Facing);
				gdiveh4id = Commands->Get_ID(gdiveh4);
			}
		}
		Commands->Start_Timer(obj,this,180.0f,100101);//loop gdi check
	}
}
void z_Trop_Carrier_Control_Point::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&nodename,sizeof(nodename),1);
	Auto_Save_Variable(&owner,sizeof(owner),2);
	Auto_Save_Variable(&trigger_neutral_damage,sizeof(trigger_neutral_damage),3);
	Auto_Save_Variable(&nodveh1,sizeof(nodveh1),4);
	Auto_Save_Variable(&nodveh2,sizeof(nodveh2),5);
	Auto_Save_Variable(&nodveh3,sizeof(nodveh3),6);
	Auto_Save_Variable(&nodveh4,sizeof(nodveh4),7);
	Auto_Save_Variable(&nodveh1id,sizeof(nodveh1id),8);
	Auto_Save_Variable(&nodveh2id,sizeof(nodveh2id),9);
	Auto_Save_Variable(&nodveh3id,sizeof(nodveh3id),10);
	Auto_Save_Variable(&nodveh4id,sizeof(nodveh4id),11);
	Auto_Save_Variable(&gdiveh1,sizeof(gdiveh1),12);
	Auto_Save_Variable(&gdiveh2,sizeof(gdiveh2),13);
	Auto_Save_Variable(&gdiveh3,sizeof(gdiveh3),14);
	Auto_Save_Variable(&gdiveh4,sizeof(gdiveh4),15);
	Auto_Save_Variable(&gdiveh1id,sizeof(gdiveh1id),16);
	Auto_Save_Variable(&gdiveh2id,sizeof(gdiveh2id),17);
	Auto_Save_Variable(&gdiveh3id,sizeof(gdiveh3id),18);
	Auto_Save_Variable(&gdiveh4id,sizeof(gdiveh4id),19);
	Auto_Save_Variable(&Spawn_Facing,sizeof(Spawn_Facing),20);
	Auto_Save_Variable(&Spawn_Pos1,sizeof(Spawn_Pos1),21);
	Auto_Save_Variable(&Spawn_Pos2,sizeof(Spawn_Pos2),22);
	Auto_Save_Variable(&Spawn_Pos3,sizeof(Spawn_Pos3),23);
	Auto_Save_Variable(&Spawn_Pos4,sizeof(Spawn_Pos4),24);
	Auto_Save_Variable(&carrierid,sizeof(carrierid),25);
	Auto_Save_Variable(&gdi_orca,sizeof(gdi_orca),26);
	Auto_Save_Variable(&nod_apache,sizeof(nod_apache),27);
}

void z_Trop_Locked_Vehicle::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if ( (Commands->Get_Player_Type(sender) != Get_Int_Parameter("Team")) )
		{
			Commands->Start_Timer(obj,this,0.5,509482);
		}
		else if ( (Commands->Get_Player_Type(sender) == Get_Int_Parameter("Team")) )
		{
			Destroy_Script();
		}
	}
}
void z_Trop_Locked_Vehicle::Timer_Expired(GameObject *obj, int number)
{
	if (number == 509482)
	{
		Force_Occupant_Exit(obj,0);
	}
}

void z_Trop_Aircraft_Carrier::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	Commands->Set_Health(obj,Commands->Get_Max_Health(obj));//decided to make the carrier invincible
}
void z_Trop_Aircraft_Carrier::Killed(GameObject *obj, GameObject *shooter)
{
	GameObject *CarrierCP = Commands->Find_Object(100251);
	Commands->Set_Animation(CarrierCP,"DOM_NODE.DOM_NODE",false,0,2,2,false);
	Commands->Set_Player_Type(CarrierCP,-2);
	Remove_Script(CarrierCP,"z_Trop_Carrier_Control_Point");
	Commands->Set_Shield_Type(CarrierCP,"Blamo");
	Set_Skin(CarrierCP,"Blamo");
	Commands->Set_Health(CarrierCP,Commands->Get_Max_Health(CarrierCP));
	Commands->Destroy_Object(Commands->Find_Object(100252));//kill refillzones
	Commands->Destroy_Object(Commands->Find_Object(100253));
	Commands->Destroy_Object(Commands->Find_Object(100254));
	Commands->Destroy_Object(Commands->Find_Object(100255));
}

void z_Trop_Helipad_Zone::Created(GameObject *obj)
{
	team = -2;
	vehid = 0;
	busy = false;
}
void z_Trop_Helipad_Zone::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		if (Commands->Get_Player_Type(enter) == team)
		{
			if (Get_Vehicle(enter) && Is_VTOL(enter))
			{
				if (!busy)
				{
					vehid = Commands->Get_ID(Get_Vehicle(enter));
					Commands->Start_Timer(obj,this,1.0f,vehid);
					busy = true;
				}
			}
		}
	}
}
void z_Trop_Helipad_Zone::Exited(GameObject *obj, GameObject *exit)
{
	if (Commands->Is_A_Star(exit))
	{
		if (Commands->Get_Player_Type(exit) == team)
		{
			if (Get_Vehicle(exit) && Is_VTOL(exit))
			{
				if (Commands->Get_ID(Get_Vehicle(exit)) == vehid)
				{
					vehid = 0;
					busy = false;
				}
			}
		}
	}
}
void z_Trop_Helipad_Zone::Timer_Expired(GameObject *obj, int number)
{
	if (number == vehid)
	{
		if (Commands->Find_Object(vehid))
		{
			Commands->Give_PowerUp(Commands->Find_Object(vehid),"POW_Heli_Ammo",false);
			Commands->Start_Timer(obj,this,1.0f,vehid);
		}
		else 
		{
			vehid = 0;
			busy = false;
		}
	}
}
void z_Trop_Helipad_Zone::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 0)
		{
			team = 0;
			vehid = 0;
			busy = false;
		}
		else if (param == 1)
		{
			team = 1;
			vehid = 0;
			busy = false;
		}
		else if (param == 2)
		{
			team = -2;
			vehid = 0;
			busy = false;
		}
	}
}
void z_Trop_Helipad_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&team,sizeof(team),1);
	Auto_Save_Variable(&vehid,sizeof(vehid),2);
	Auto_Save_Variable(&busy,sizeof(busy),3);
}

ScriptRegistrant<z_Trop_Helipad_Zone> z_Trop_Helipad_Zone_Registrant("z_Trop_Helipad_Zone","");
ScriptRegistrant<z_Trop_Aircraft_Carrier> z_Trop_Aircraft_Carrier_Registrant("z_Trop_Aircraft_Carrier","");
ScriptRegistrant<z_Trop_Locked_Vehicle> z_Trop_Locked_Vehicle_Registrant("z_Trop_Locked_Vehicle","Team=0:int");
ScriptRegistrant<z_Trop_Carrier_Control_Point> z_Trop_Carrier_Control_Point_Registrant("z_Trop_Carrier_Control_Point","Node_Name=Carrier:string");
ScriptRegistrant<z_Trop_Vehicle> z_Trop_Vehicle_Registrant("z_Trop_Vehicle","");