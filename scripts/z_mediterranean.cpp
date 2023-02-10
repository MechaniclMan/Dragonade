/*	Renegade Scripts.dll
	Copyright 2013 UltraAOW.COM

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
#include "z_mediterranean.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"
#include "BuildingGameObj.h"
#ifdef SSGM
#include "gmgame.h"
#endif

void z_Med_Vehicle::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,3.0f,102030);//check Z position
}
void z_Med_Vehicle::Timer_Expired(GameObject *obj, int number)
{
	if (number == 102030)
	{
		Vector3 currpos = Commands->Get_Position(obj);
		if (currpos.Z < -7.821f)
		{
			Commands->Apply_Damage(obj,99999.0f,"Death",0);//destroy vehicle cos it dropped below the permitted Z height ie: it is now driving on the water plane which is bad ;)
		}
		else
		{
			Commands->Start_Timer(obj,this,3.0f,102030);//vehicle did not drop below Z Height so we leave it alone, for now...
		}
	}
}
void z_Med_Vehicle::Killed(GameObject *obj, GameObject *shooter)
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

void z_Med_Disable_Transitions_Zone::Entered(GameObject *obj, GameObject *enter)
{
	Commands->Enable_Vehicle_Transitions(enter, false);
}
void z_Med_Disable_Transitions_Zone::Exited(GameObject *obj, GameObject *exit)
{
	Commands->Enable_Vehicle_Transitions(exit, true);
}

void z_Med_Credit_Trickle::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,3.0f,150);
}
void z_Med_Credit_Trickle::Timer_Expired(GameObject *obj, int number)
{
	if (number == 150)
	{
		Commands->Give_Money(obj,5.0f,true);
		Commands->Start_Timer(obj,this,3.0f,150);
	}
}

void z_Med_TibSilo::Created(GameObject *obj)
{
	owner = Get_Int_Parameter("Owner");
	Commands->Set_Player_Type(obj,owner);
	Attach_Script_Once(obj,"z_Med_Credit_Trickle","");
	playdmg = true;
	playkilled = true;
}
void z_Med_TibSilo::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (damage >= Commands->Get_Health(obj))
	{
		Commands->Set_Health(obj,1.0f);//prevent destruction
		Commands->Set_Player_Type(obj,-2);//unteamed
		Remove_Script(obj,"z_Med_Credit_Trickle");//no more income from this silo
		if (playkilled)
		{
			if (owner == 0)//nod silo killed
			{
				Create_2D_WAV_Sound_Team("m00bnts_kill0001i1evan_snd.wav",0);
				Create_2D_WAV_Sound_Team("m00bnts_kill0002i1evag_snd.wav",1);
				Send_Message(255,255,255,"Nod Tiberium Silo destroyed");
			}
			else if (owner == 1)//gdi silo killed
			{
				Create_2D_WAV_Sound_Team("m00bgts_kill0002i1evan_snd.wav",0);
				Create_2D_WAV_Sound_Team("m00bgts_kill0001i1evag_snd.wav",1);
				Send_Message(255,255,255,"GDI Tiberium Silo destroyed");
			}
			Set_Damage_Points(obj,0.0f);
			Commands->Give_Points(damager,150.0f,false);
		}
		owner = -2;
		playdmg = false;
		playkilled = false;
	}

	if (owner == -2)//neutral silo
	{
		if (damage < 0.0) //its repaired
		{
			if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))//fully repaired
			{
				owner = Commands->Get_Player_Type(damager);//whoever repaired it owns it now
				Commands->Set_Player_Type(obj,owner);
				Attach_Script_Once(obj,"z_Med_Credit_Trickle","");
				if (owner == 0)
				{
					Create_2D_WAV_Sound_Team("m00bnts_dsgn0002i1evan_snd.wav",0);
					Send_Message_Team(0,255,0,0,"Nod Tiberium Silo repaired");
				}
				else if (owner == 1)
				{
					Create_2D_WAV_Sound_Team("m00bgts_dsgn0001i1evag_snd.wav",1);
					Send_Message_Team(1,255,204,0,"GDI Tiberium Silo repaired");
				}
				playdmg = true;
				playkilled = true;
				Set_Damage_Points(obj,0.300f);
			}
		}
	}
	else if (owner == 0)//nod silo
	{
		if (damage > 0.1f)
		{
			if (playdmg)
			{
				Create_2D_WAV_Sound_Team("m00bnts_tdfe0001i1evan_snd.wav",0);
				Send_Message_Team(0,255,255,255,"Warning Nod Tiberium Silo under attack");
				Create_2D_WAV_Sound_Team("m00bnts_tdfe0002i1evag_snd.wav",1);
				Send_Message_Team(1,255,255,255,"Nod Tiberium Silo under attack");
				playdmg = false; Commands->Start_Timer(obj,this,25.0f,155);
			}
		}
	}
	else if (owner == 1)//gdi silo
	{
		if (damage > 0.1f)
		{
			if (playdmg)
			{
				Create_2D_WAV_Sound_Team("m00bgts_tdfe0002i1evan_snd.wav",0);
				Send_Message_Team(0,255,255,255,"GDI Tiberium Silo under attack");
				Create_2D_WAV_Sound_Team("m00bgts_tdfe0001i1evag_snd.wav",1);
				Send_Message_Team(1,255,255,255,"Warning GDI Tiberium Silo under attack");
				playdmg = false; Commands->Start_Timer(obj,this,25.0f,155);
			}
		}
	}
}
void z_Med_TibSilo::Timer_Expired(GameObject *obj, int number)
{
	if (number == 155)
	{
		playdmg = true;
	}
}
void z_Med_TibSilo::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&playdmg,sizeof(playdmg),1);
	Auto_Save_Variable(&playkilled,sizeof(playkilled),2);
}

void z_Med_Helipad::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	playdmg = true;
	playkilled = true;
	disablerepair = false;
}
void z_Med_Helipad::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (damage > Commands->Get_Health(obj))
	{
		if (playkilled)
		{
			if (team == 0)//it is nod helipad
			{
				Send_Message(255,255,255,"Nod Helicopter Pad destroyed");
				Create_2D_WAV_Sound_Team("m00bnhp_kill0001i1evan_snd.wav",0);
				Create_2D_WAV_Sound_Team("m00bnhp_kill0002i1evag_snd.wav",1);
				GameObject *targetnod = Find_Object_With_Script("z_Med_Helipad_Count_Nod");
				Commands->Send_Custom_Event(obj,targetnod,2500,1,0);
			}
			else if (team == 1)//it is gdi helipad
			{
				Send_Message(255,255,255,"GDI Helicopter Pad destroyed");
				Create_2D_WAV_Sound_Team("m00bghp_kill0002i1evan_snd.wav",0);
				Create_2D_WAV_Sound_Team("m00bghp_kill0001i1evag_snd.wav",1);
				GameObject *targetgdi = Find_Object_With_Script("z_Med_Helipad_Count_GDI");
				Commands->Send_Custom_Event(obj,targetgdi,2500,1,0);
			}
			Commands->Destroy_Object(Commands->Find_Object(Get_Int_Parameter("RefillZoneID")));//stop refills :D
			Commands->Give_Points(damager,150.0f,false);
		}
		Commands->Set_Health(obj,1.0f);
		Commands->Set_Player_Type(obj,-2);
		Set_Damage_Points(obj,0.0f);
		playkilled = false;
		playdmg = false;
		disablerepair = true;
	}
	if (damage > 0.1f)
	{
		if (playdmg)
		{
			if (team == 0)//it is nod helipad
			{
				Send_Message_Team(0,255,255,255,"Warning Nod Helicopter Pad under attack");
				Send_Message_Team(1,255,255,255,"Nod Helicopter Pad under attack");
				Create_2D_WAV_Sound_Team("m00bnhp_tdfe0001i1evan_snd.wav",0);
				Create_2D_WAV_Sound_Team("m00bnhp_tdfe0002i1evag_snd.wav",1);
			}
			else if (team == 1)//it is gdi helipad
			{
				Send_Message_Team(0,255,255,255,"GDI Helicopter Pad under attack");
				Send_Message_Team(1,255,255,255,"Warning GDI Helicopter Pad under attack");
				Create_2D_WAV_Sound_Team("m00bghp_tdfe0002i1evan_snd.wav",0);
				Create_2D_WAV_Sound_Team("m00bghp_tdfe0001i1evag_snd.wav",1);
			}
			playdmg = false; Commands->Start_Timer(obj,this,25.0f,150);
		}
	}
	if (disablerepair)
	{
		Commands->Set_Health(obj,1.0f);
	}
}
void z_Med_Helipad::Timer_Expired(GameObject *obj, int number)
{
	if (number == 150)
	{
		if (playkilled)
		{
			playdmg = true;
		}
	}
}
void z_Med_Helipad::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&playdmg,sizeof(playdmg),1);
	Auto_Save_Variable(&playkilled,sizeof(playkilled),2);
	Auto_Save_Variable(&team,sizeof(team),3);
	Auto_Save_Variable(&disablerepair,sizeof(disablerepair),4);
}

void z_Med_Helipad_Zone::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	vehid = 0;
	busy = false;
}
void z_Med_Helipad_Zone::Entered(GameObject *obj, GameObject *enter)
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
void z_Med_Helipad_Zone::Exited(GameObject *obj, GameObject *exit)
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
void z_Med_Helipad_Zone::Timer_Expired(GameObject *obj, int number)
{
	if (number == vehid)
	{
		if (Commands->Find_Object(vehid))
		{
			Commands->Give_PowerUp(Commands->Find_Object(vehid),"Med_VTOL_Ammo_Powerup",false);
			Commands->Start_Timer(obj,this,1.0f,vehid);
		}
		else 
		{
			vehid = 0;
			busy = false;
		}
	}
}
void z_Med_Helipad_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&team,sizeof(team),1);
	Auto_Save_Variable(&vehid,sizeof(vehid),2);
	Auto_Save_Variable(&busy,sizeof(busy),3);
}

void z_Med_Helipad_Count_Nod::Created(GameObject *obj)
{
	deaths = 0;
}
void z_Med_Helipad_Count_Nod::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 2500)
	{
		if (param == 1)
		{
			deaths++;
			if (deaths == Get_Int_Parameter("Helipad_Count"))
			{
				Commands->Apply_Damage(Commands->Find_Object(Get_Int_Parameter("Helipad_Building_Controller")),99999.0f,"Death",0);
			}
		}
	}
}
void z_Med_Helipad_Count_Nod::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&deaths,sizeof(deaths),1);
}

void z_Med_Helipad_Count_GDI::Created(GameObject *obj)
{
	deaths = 0;
}
void z_Med_Helipad_Count_GDI::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 2500)
	{
		if (param == 1)
		{
			deaths++;
			if (deaths == Get_Int_Parameter("Helipad_Count"))
			{
				Commands->Apply_Damage(Commands->Find_Object(Get_Int_Parameter("Helipad_Building_Controller")),99999.0f,"Death",0);
			}
		}
	}
}
void z_Med_Helipad_Count_GDI::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&deaths,sizeof(deaths),1);
}



void z_Med_Repairpad::Created(GameObject *obj)
{
	owner = Get_Int_Parameter("Owner");
	zoneid = Get_Int_Parameter("RepairZoneID");
	Commands->Set_Player_Type(obj,owner);
	Commands->Send_Custom_Event(obj,Commands->Find_Object(zoneid),2500,owner,1);//tell zone what units to repair or none at all
	playdmg = true;
	playkilled = true;
}
void z_Med_Repairpad::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (damage >= Commands->Get_Health(obj))
	{
		Commands->Set_Health(obj,1.0f);//prevent destruction
		Commands->Set_Player_Type(obj,-2);//unteamed
		if (playkilled)
		{
			if (owner == 0)//nod silo killed
			{
				Create_2D_WAV_Sound_Team("m00bnrf_kill0001i1evan_snd.wav",0);
				Create_2D_WAV_Sound_Team("m00bnrf_kill0002i1evag_snd.wav",1);
				Send_Message(255,255,255,"Nod Repair Facility destroyed");
			}
			else if (owner == 1)//gdi silo killed
			{
				Create_2D_WAV_Sound_Team("m00bgrf_kill0002i1evan_snd.wav",0);
				Create_2D_WAV_Sound_Team("m00bgrf_kill0001i1evag_snd.wav",1);
				Send_Message(255,255,255,"GDI Repair Facility destroyed");
			}
			Set_Damage_Points(obj,0.0f);
			Commands->Give_Points(damager,150.0f,false);
			Commands->Send_Custom_Event(obj,Commands->Find_Object(zoneid),2500,owner,1);//tell zone what units to repair or none at all
		}
		owner = -2;
		playdmg = false;
		playkilled = false;
	}

	if (owner == -2)//neutral silo
	{
		if (damage < 0.0) //its repaired
		{
			if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))//fully repaired
			{
				owner = Commands->Get_Player_Type(damager);//whoever repaired it owns it now
				Commands->Set_Player_Type(obj,owner);
				if (owner == 0)
				{
					Create_2D_WAV_Sound_Team("m00bnrf_dsgn0002i1evan_snd.wav",0);
					Send_Message_Team(0,255,0,0,"Nod Repair Facility repaired");
					Commands->Send_Custom_Event(obj,Commands->Find_Object(zoneid),2500,owner,1);//tell zone what units to repair or none at all
				}
				else if (owner == 1)
				{
					Create_2D_WAV_Sound_Team("m00bgrf_dsgn0001i1evag_snd.wav",1);
					Send_Message_Team(1,255,204,0,"GDI Repair Facility repaired");
					Commands->Send_Custom_Event(obj,Commands->Find_Object(zoneid),2500,owner,1);//tell zone what units to repair or none at all
				}
				playdmg = true;
				playkilled = true;
				Set_Damage_Points(obj,0.300f);
			}
		}
	}
	else if (owner == 0)//nod pad
	{
		if (damage > 0.1f)
		{
			if (playdmg)
			{
				Create_2D_WAV_Sound_Team("m00bnrf_tdfe0001i1evan_snd.wav",0);
				Send_Message_Team(0,255,255,255,"Warning Nod Repair Facility under attack");
				Create_2D_WAV_Sound_Team("m00bnrf_tdfe0002i1evag_snd.wav",1);
				Send_Message_Team(1,255,255,255,"Nod Repair Facility under attack");
				playdmg = false; Commands->Start_Timer(obj,this,25.0f,155);
			}
		}
	}
	else if (owner == 1)//gdi pad
	{
		if (damage > 0.1f)
		{
			if (playdmg)
			{
				Create_2D_WAV_Sound_Team("m00bgrf_tdfe0002i1evan_snd.wav",0);
				Send_Message_Team(0,255,255,255,"GDI Repair Facility under attack");
				Create_2D_WAV_Sound_Team("m00bgrf_tdfe0001i1evag_snd.wav",1);
				Send_Message_Team(1,255,255,255,"Warning GDI Repair Facility under attack");
				playdmg = false; Commands->Start_Timer(obj,this,25.0f,155);
			}
		}
	}
}
void z_Med_Repairpad::Timer_Expired(GameObject *obj, int number)
{
	if (number == 155)
	{
		playdmg = true;
	}
}
void z_Med_Repairpad::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&playdmg,sizeof(playdmg),1);
	Auto_Save_Variable(&playkilled,sizeof(playkilled),2);
}

void z_Med_Repairzone::Created(GameObject *obj)
{
	padid = Get_Int_Parameter("RepairPadID");
	playerobjid = 0;
	owner = -2;
	vehid = 0;
	busy = false;
}
void z_Med_Repairzone::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 2500)
	{
		if (param == 0)
		{
			owner = 0;
		}
		else if (param == 1)
		{
			owner = 1;
		}
		else if (param == -2)
		{
			owner = -2;
		}
	}
}
void z_Med_Repairzone::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		if (Get_Vehicle(enter))
		{
			if (Commands->Get_Player_Type(enter) == owner)//the guy that entered belongs to the owning team
			{
				if (Commands->Get_Health(Commands->Find_Object(padid)) > 2.0f)//the pad is alive n kicking (2.0f because 1.0f=dead)
				{
					if (!busy)
					{
						if (owner == 0)
						{
							Send_Message_Player(enter,255,0,0,"Repairing");
							Create_2D_WAV_Sound_Player(enter,"m00evan_dsgn0015i1evan_snd.wav");
						}
						else if (owner == 1)
						{
							Send_Message_Player(enter,255,204,0,"Repairing");
							Create_2D_WAV_Sound_Player(enter,"m00evag_dsgn0018i1evag_snd.wav");
						}
						vehid = Commands->Get_ID(Get_Vehicle(enter));
						Commands->Start_Timer(obj,this,1.0f,vehid);
						busy = true;
						playerobjid = Commands->Get_ID(Get_Vehicle_Driver(enter));
					}
				}
			}
		}
	}
}
void z_Med_Repairzone::Exited(GameObject *obj, GameObject *exit)
{
	if (Commands->Is_A_Star(exit))
	{
		if (Get_Vehicle(exit))
		{
			if (Commands->Get_Player_Type(exit) == owner)//the guy that exited belongs to the owning team
			{
				if (Commands->Get_ID(Get_Vehicle(exit)) == vehid)
				{
					vehid = 0;
					busy = false;
					playerobjid = 0;
				}
			}
		}
	}
}
void z_Med_Repairzone::Timer_Expired(GameObject *obj, int number)
{
	if (number == vehid)
	{
		if (Commands->Find_Object(vehid))
		{
			float maxhp = Commands->Get_Max_Health(Commands->Find_Object(vehid));
			float curhp = Commands->Get_Health(Commands->Find_Object(vehid));
			float maxap = Commands->Get_Max_Shield_Strength(Commands->Find_Object(vehid));
			float curap = Commands->Get_Shield_Strength(Commands->Find_Object(vehid));
			if (curhp < maxhp)
			{
				Commands->Set_Health(Commands->Find_Object(vehid),curhp + 10.0f);
			}
			else if (curap < maxap)
			{
				Commands->Set_Shield_Strength(Commands->Find_Object(vehid),curap + 10.0f);
			}
			Commands->Start_Timer(obj,this,1.0,vehid);
		}
		else
		{
			busy = false;
			vehid = 0;
			playerobjid = 0;
		}
	}
}
void z_Med_Repairzone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&owner,sizeof(owner),1);
	Auto_Save_Variable(&vehid,sizeof(vehid),2);
	Auto_Save_Variable(&busy,sizeof(busy),3);
	Auto_Save_Variable(&playerobjid,sizeof(playerobjid),4);
}

void z_Med_Building::Created(GameObject *obj)
{
	trigger75 = true;
	trigger50 = true;
	trigger25 = true;
	if ((obj->As_BuildingGameObj() && obj->As_BuildingGameObj()->As_SoldierFactoryGameObj()) && (Get_Object_Type(obj) == 1))
	{
		Commands->Start_Timer(obj,this,30.0f,5004999);//set vehlimit and creator msg
	}
}
void z_Med_Building::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	int team = Get_Object_Type(obj);
	float hp75a = Commands->Get_Max_Health(obj)/100*70;
	float hp75b = Commands->Get_Max_Health(obj)/100*80;
	float hp50a = Commands->Get_Max_Health(obj)/100*45;
	float hp50b = Commands->Get_Max_Health(obj)/100*55;
	float hp25a = Commands->Get_Max_Health(obj)/100*20;
	float hp25b = Commands->Get_Max_Health(obj)/100*30;
	if (Commands->Get_Health(obj) >hp75a && Commands->Get_Health(obj) <hp75b)
	{
		if (trigger75)
		{
			if (team == 0)
			{
#ifdef SSGM
				char hpmsg[512];
				sprintf(hpmsg,"%s Health at 75 percent",Get_Translated_Preset_Name_Ex(obj));
				Send_Message_Team(0,255,0,0,hpmsg);
#endif
				Create_2D_WAV_Sound_Team("m00evan_dsgn0070i1evan_snd.wav",0);
			}
			if (team == 1)
			{
#ifdef SSGM
				char hpmsg[512];
				sprintf(hpmsg,"%s Health at 75 percent",Get_Translated_Preset_Name_Ex(obj));
				Send_Message_Team(1,255,204,0,hpmsg);
#endif
				Create_2D_WAV_Sound_Team("m00evag_dsgn0066i1evag_snd.wav",1);
			}
			trigger75 = false; Commands->Start_Timer(obj,this,30.0f,500500075);
		}
	}
	else if (Commands->Get_Health(obj) >hp50a && Commands->Get_Health(obj) <hp50b)
	{
		if (trigger50)
		{
			if (team == 0)
			{
#ifdef SSGM
				char hpmsg[512];
				sprintf(hpmsg,"%s Health at 50 percent",Get_Translated_Preset_Name_Ex(obj));
				Send_Message_Team(0,255,0,0,hpmsg);
#endif
				Create_2D_WAV_Sound_Team("m00evan_dsgn0071i1evan_snd.wav",0);
			}
			if (team == 1)
			{
#ifdef SSGM
				char hpmsg[512];
				sprintf(hpmsg,"%s Health at 50 percent",Get_Translated_Preset_Name_Ex(obj));
				Send_Message_Team(1,255,204,0,hpmsg);
#endif
				Create_2D_WAV_Sound_Team("m00evag_dsgn0067i1evag_snd.wav",1);
			}
			trigger50 = false; Commands->Start_Timer(obj,this,30.0f,500500050);
		}
	}
	else if (Commands->Get_Health(obj) >hp25a && Commands->Get_Health(obj) <hp25b)
	{
		if (trigger25)
		{
			if (team == 0)
			{
#ifdef SSGM
				char hpmsg[512];
				sprintf(hpmsg,"%s Health at 25 percent",Get_Translated_Preset_Name_Ex(obj));
				Send_Message_Team(0,255,0,0,hpmsg);
#endif
				Create_2D_WAV_Sound_Team("m00evan_dsgn0072i1evan_snd.wav",0);
			}
			if (team == 1)
			{
#ifdef SSGM
				char hpmsg[512];
				sprintf(hpmsg,"%s Health at 25 percent",Get_Translated_Preset_Name_Ex(obj));
				Send_Message_Team(1,255,204,0,hpmsg);
#endif
				Create_2D_WAV_Sound_Team("m00evag_dsgn0068i1evag_snd.wav",1);
			}
			trigger25 = false; Commands->Start_Timer(obj,this,30.0f,500500025);
		}
	}
}
void z_Med_Building::Timer_Expired(GameObject *obj, int number)
{
	if (number == 5004999)//spam version
	{
		Console_Input("msg Running Tropical or Mediterranean by zunnie and Mauler");
		Console_Input("msg Vehicle Limit set to: 20 Vehicles per Team");
		Console_Input("vlimit 20");
	}	
	//building health status timers
	else if (number == 500500075)//trigger75
	{
		trigger75 = true;
	}
	else if (number == 500500050)//trigger50
	{
		trigger50 = true;
	}
	else if (number == 500500025)//trigger25
	{
		trigger25 = true;
	}
	//building health status timers
}
void z_Med_Building::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&trigger75,sizeof(trigger75),1);
	Auto_Save_Variable(&trigger50,sizeof(trigger50),2);
	Auto_Save_Variable(&trigger25,sizeof(trigger25),3);
}

void z_Med_Teleporter::Created(GameObject *obj)
{
	tele = true;
}
void z_Med_Teleporter::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Get_Player_Type(enter) == Get_Int_Parameter("Team"))
	{
		if (!Get_Vehicle(enter))
		{
			if (tele)
			{
				int id1 = Get_Int_Parameter("TeleID1");				
				int id2 = Get_Int_Parameter("TeleID2");				
				int id3 = Get_Int_Parameter("TeleID3");
				int random = Commands->Get_Random_Int(1,3);
				if (random == 1)
				{
					Vector3 pos1 = Commands->Get_Position(Commands->Find_Object(id1));
					Commands->Set_Position(enter,pos1);
				}
				else if (random == 2)
				{
					Vector3 pos2 = Commands->Get_Position(Commands->Find_Object(id2));
					Commands->Set_Position(enter,pos2);
				}
				else if (random == 3)
				{
					Vector3 pos3 = Commands->Get_Position(Commands->Find_Object(id3));
					Commands->Set_Position(enter,pos3);
				}
				tele = false; Commands->Start_Timer(obj,this,2.0f,100);//tele true
			}
		}
	}
	else
	{
		Send_Message_Player(enter,255,255,255,"Gowai nub");
	}
}
void z_Med_Teleporter::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		tele = true;
	}
}
void z_Med_Teleporter::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&tele,sizeof(tele),1);
}

void z_Med_Buy_Heli::Created(GameObject *obj)
{
	hpadid = Get_Int_Parameter("HelipadID");
	type = Get_Parameter("Type");
	cost = Get_Float_Parameter("Cost");
	team = Get_Int_Parameter("Team");
	buyerid = 0;
	allowpoke = true; Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void z_Med_Buy_Heli::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&hpadid,sizeof(hpadid),1);
	Auto_Save_Variable(&type,sizeof(type),2);
	Auto_Save_Variable(&cost,sizeof(cost),3);
	Auto_Save_Variable(&team,sizeof(team),4);
	Auto_Save_Variable(&buyerid,sizeof(buyerid),5);
	Auto_Save_Variable(&allowpoke,sizeof(allowpoke),6);
}
void z_Med_Buy_Heli::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (Commands->Get_Health(Commands->Find_Object(hpadid)) >= 2.0f)
		{
			if (Commands->Get_Player_Type(poker) == team)
			{
				if (Is_Base_Powered(team))
				{
					if (Commands->Get_Money(poker) >= cost)
					{
						Commands->Give_Money(poker,-cost,false);
						Commands->Start_Timer(obj,this,1.0f,100);
						allowpoke = false; Commands->Enable_HUD_Pokable_Indicator(obj,false); Commands->Start_Timer(obj,this,10.0f,99);//allowpoke true
						buyerid = Commands->Get_ID(poker);
						if (team == 0)
						{
							Send_Message_Player(poker,255,0,0,"Building");
							Create_2D_WAV_Sound_Player(poker,"m00evan_dsgn0002i1evan_snd.wav");
						}
						else if (team == 1)
						{
							Send_Message_Player(poker,255,204,0,"Building");
							Create_2D_WAV_Sound_Player(poker,"m00evag_dsgn0004i1evag_snd.wav");
						}
					}
					else
					{
						if (team == 0)
						{
							Send_Message_Player(poker,255,0,0,"Insufficient funds");
							Create_2D_WAV_Sound_Player(poker,"m00evan_dsgn0024i1evan_snd.wav");
						}
						else if (team == 1)
						{
							Send_Message_Player(poker,255,204,0,"Insufficient funds");
							Create_2D_WAV_Sound_Player(poker,"m00evag_dsgn0028i1evag_snd.wav");
						}
					}
				}
				else
				{
					if (Commands->Get_Money(poker) >= cost*2)
					{
						Commands->Give_Money(poker,-cost,false);
						Commands->Give_Money(poker,-cost,false);
						Commands->Start_Timer(obj,this,1.0f,100);
						allowpoke = false; Commands->Enable_HUD_Pokable_Indicator(obj,false); Commands->Start_Timer(obj,this,10.0f,99);//allowpoke true
						buyerid = Commands->Get_ID(poker);
					}
					else
					{
						if (team == 0)
						{
							Send_Message_Player(poker,255,0,0,"Insufficient funds: No Base Power = Double Costs");
							Create_2D_WAV_Sound_Player(poker,"m00evan_dsgn0024i1evan_snd.wav");
						}
						else if (team == 1)
						{
							Send_Message_Player(poker,255,204,0,"Insufficient funds: No Base Power = Double Costs");
							Create_2D_WAV_Sound_Player(poker,"m00evag_dsgn0028i1evag_snd.wav");
						}
					}
				}
			}
			else
			{
				Send_Message_Player(poker,255,255,255,"Access Denied: go away n00b");
				Create_2D_WAV_Sound_Player(poker,"m00evag_dsgn0039i1evag_snd.wav");
			}
		}
	}
}
void z_Med_Buy_Heli::Timer_Expired(GameObject *obj, int number)
{
	if (number == 99)
	{
		allowpoke = true; Commands->Enable_HUD_Pokable_Indicator(obj,true);
	}
	else if (number == 100)
	{
		Vector3 createpos = Get_Vector3_Parameter("Location");
		float facing = Get_Float_Parameter("Facing");
		GameObject *veh = Commands->Create_Object(type,createpos);
		Commands->Set_Facing(veh,facing);
#ifdef SSGM
		if (team == 0)
		{
			char buymsg[512];
			sprintf(buymsg,"PURCHASE: %s purchased a %s",Get_Player_Name(Commands->Find_Object(buyerid)),Get_Translated_Preset_Name_Ex(veh));
			Send_Message_Team(0,255,0,0,buymsg);
		}
		else if (team == 1)
		{
			char buymsg[512];
			sprintf(buymsg,"PURCHASE: %s purchased a %s",Get_Player_Name(Commands->Find_Object(buyerid)),Get_Translated_Preset_Name_Ex(veh));
			Send_Message_Team(1,255,204,0,buymsg);
		}
#endif
	}
}

void z_Med_Supply_Truck::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (Commands->Get_ID(Get_Vehicle_Driver(obj)) == Commands->Get_ID(sender))
		{
			Send_Message_Player(sender,255,255,255,"Supply Truck: You refill everyone that enters your Truck");
		}
		else
		{
			if (Get_Vehicle_Driver(obj))
			{
				Commands->Give_PowerUp(sender,"CnC_Ammo_Crate",false);
				Commands->Set_Health(sender,Commands->Get_Max_Health(sender));
				Commands->Set_Shield_Strength(sender,Commands->Get_Max_Shield_Strength(sender));
				Create_2D_WAV_Sound_Player(sender,"m00puar_aqob0002i1evag_snd.wav");
				Send_Message_Player(sender,255,255,255,"Your ammo, health and armor have been refilled");
			}
		}
	}
}

void z_Med_Tiberium_Harvester_Player::Created(GameObject *obj)
{
	isharvesting = false;
	hastiberium = false;
	driverid = 0;
	team = 0;
	plid = 0;
}
void z_Med_Tiberium_Harvester_Player::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			if (!isharvesting)
			{
				Commands->Start_Timer(obj,this,10.0f,100);
				isharvesting = true;
				Send_Message_Player(Commands->Find_Object(driverid),104,234,40,"You are now harvesting Tiberium. Please wait...");
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
						Commands->Give_Money(obj,1000.0f,true);
						Commands->Give_Points(Get_GameObj(plid),100.0f,false);
						Create_2D_WAV_Sound_Team("m00pc$$_aqob0002i1evag_snd.wav",0);
						char msg[250]; sprintf(msg,"%s deposited Tiberium funding Nod with $%g",Get_Player_Name(Commands->Find_Object(driverid)),1000.0f);
						Send_Message_Team(0,255,0,0,msg);
						hastiberium = false;
						isharvesting = false;
					}
				}
				else if (Commands->Get_Player_Type(obj) == 1)
				{
					if (Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY) && Commands->Get_Health(Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY)) > 1.0f)
					{
						Commands->Give_Money(obj,1000.0f,true);
						Commands->Give_Points(Get_GameObj(plid),100.0f,false);
						Create_2D_WAV_Sound_Team("m00pc$$_aqob0002i1evag_snd.wav",1);
						char msg2[250]; sprintf(msg2,"%s deposited Tiberium funding GDI with $%g",Get_Player_Name(Commands->Find_Object(driverid)),1000.0f);
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
		Send_Message_Player(Commands->Find_Object(driverid),104,234,40,"Goto the Tiberium Field to harvest Tiberium, then return to the Refinery to deposit.");
	}
	else if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		team = 0;
		driverid = 0;
		plid = 0;
	}
}

void z_Med_Tiberium_Harvester_Player::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		if (isharvesting)
		{
			hastiberium = true;
			isharvesting = false;
			Create_2D_WAV_Sound_Player(Commands->Find_Object(driverid),"00-n006e.wav");
			Send_Message_Player(Commands->Find_Object(driverid),104,234,40,"HARVESTING COMPLETE: Return to the Refinery to deposit.");
		}
	}
}
void z_Med_Tiberium_Harvester_Player::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&team,sizeof(team),1);
	Auto_Save_Variable(&isharvesting,sizeof(isharvesting),2);
	Auto_Save_Variable(&hastiberium,sizeof(hastiberium),3);
	Auto_Save_Variable(&driverid,sizeof(driverid),4);
	Auto_Save_Variable(&plid,sizeof(plid),5);
}

void z_Med_Tiberium_Field::Entered(GameObject *obj, GameObject *enter)
{
	if (Is_Script_Attached(enter,"z_Med_Tiberium_Harvester_Player"))
	{
		Commands->Send_Custom_Event(obj,enter,1000,1,0);//harvesting started
	}
}
void z_Med_Tiberium_Field::Exited(GameObject *obj, GameObject *exit)
{
	if (Is_Script_Attached(exit,"z_Med_Tiberium_Harvester_Player"))
	{
		Commands->Send_Custom_Event(obj,exit,1001,1,0);//not harvesting anymore
	}
}

void z_Med_Tiberium_Deposit::Entered(GameObject *obj, GameObject *enter)
{
	if (Is_Script_Attached(enter,"z_Med_Tiberium_Harvester_Player"))
	{
		Commands->Send_Custom_Event(obj,enter,1002,1,0);//dump
	}	
}

ScriptRegistrant<z_Med_Tiberium_Harvester_Player> z_Med_Tiberium_Harvester_Player_Registrant("z_Med_Tiberium_Harvester_Player","");
ScriptRegistrant<z_Med_Tiberium_Field> z_Med_Tiberium_Field_Registrant("z_Med_Tiberium_Field","");
ScriptRegistrant<z_Med_Tiberium_Deposit> z_Med_Tiberium_Deposit_Registrant("z_Med_Tiberium_Deposit","");
ScriptRegistrant<z_Med_Supply_Truck> z_Med_Supply_Truck_Registrant("z_Med_Supply_Truck","");
ScriptRegistrant<z_Med_Buy_Heli> z_Med_Buy_Heli_Registrant("z_Med_Buy_Heli","HelipadID=0:int,Type=Med_GDI_Orca:string,Team=1:int,Cost=75000:float,Location=0:vector3,Facing=0:float");
ScriptRegistrant<z_Med_Teleporter> z_Med_Teleporter_Registrant("z_Med_Teleporter","Team=0:int,TeleID1=0:int,TeleID2=0:int,TeleID3=0:int");
ScriptRegistrant<z_Med_Building> z_Med_Building_Registrant("z_Med_Building","");
ScriptRegistrant<z_Med_Repairzone> z_Med_Repairzone_Registrant("z_Med_Repairzone","RepairPadID=0:int");
ScriptRegistrant<z_Med_Repairpad> z_Med_Repairpad_Registrant("z_Med_Repairpad","Owner=0:int,RepairZoneID=0:int");
ScriptRegistrant<z_Med_Helipad_Count_GDI> z_Med_Helipad_Count_GDI_Registrant("z_Med_Helipad_Count_GDI","Helipad_Count=3:int,Helipad_Building_Controller=0:int");
ScriptRegistrant<z_Med_Helipad_Count_Nod> z_Med_Helipad_Count_Nod_Registrant("z_Med_Helipad_Count_Nod","Helipad_Count=3:int,Helipad_Building_Controller=0:int");
ScriptRegistrant<z_Med_Helipad_Zone> z_Med_Helipad_Zone_Registrant("z_Med_Helipad_Zone","Team=0:int");
ScriptRegistrant<z_Med_Helipad> z_Med_Helipad_Registrant("z_Med_Helipad","Team=0:int,RefillZoneID=0:int");
ScriptRegistrant<z_Med_TibSilo> z_Med_TibSilo_Registrant("z_Med_TibSilo","Owner=0:int");
ScriptRegistrant<z_Med_Credit_Trickle> z_Med_Credit_Trickle_Registrant("z_Med_Credit_Trickle","");
ScriptRegistrant<z_Med_Disable_Transitions_Zone> z_Med_Disable_Transitions_Zone_Registrant("z_Med_Disable_Transitions_Zone","");
ScriptRegistrant<z_Med_Vehicle> z_Med_Vehicle_Registrant("z_Med_Vehicle","");