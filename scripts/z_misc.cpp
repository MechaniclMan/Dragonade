/*	Renegade Scripts.dll
	Copyright 2013 UltraAOW.COM by zunnie

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
#include "z_misc.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"
#ifdef SSGM
#include "gmgame.h"
#endif

void z_Building_Health_Notifications::Created(GameObject *obj)
{
	trigger75 = true;
	trigger50 = true;
	trigger25 = true;
}
void z_Building_Health_Notifications::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	int team = Get_Object_Type(obj);
	float hp75a = Commands->Get_Max_Health(obj)/100*72;
	float hp75b = Commands->Get_Max_Health(obj)/100*78;
	float hp50a = Commands->Get_Max_Health(obj)/100*47;
	float hp50b = Commands->Get_Max_Health(obj)/100*53;
	float hp25a = Commands->Get_Max_Health(obj)/100*22;
	float hp25b = Commands->Get_Max_Health(obj)/100*28;
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
void z_Building_Health_Notifications::Timer_Expired(GameObject *obj, int number)
{
	//building health status timers
	if (number == 500500075)//trigger75
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
void z_Building_Health_Notifications::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&trigger75,sizeof(trigger75),1);
	Auto_Save_Variable(&trigger50,sizeof(trigger50),2);
	Auto_Save_Variable(&trigger25,sizeof(trigger25),3);
}

void z_Locked_Vehicle::Custom(GameObject *obj, int message, int param, GameObject *sender)
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
void z_Locked_Vehicle::Timer_Expired(GameObject *obj, int number)
{
	if (number == 509482)
	{
		Force_Occupant_Exit(obj,0);
	}
}

void z_Simple_Tiberium_Silo::Created(GameObject *obj)
{
	playdead = true;
	playdmg = true;
	givemoney = true;
	Commands->Start_Timer(obj,this,1.0f,205060);
}
void z_Simple_Tiberium_Silo::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Get_Health(obj) <= 2.0f)
	{
		Commands->Set_Health(obj,2.0f);
		Set_Damage_Points(obj,0.0f);
		Set_Death_Points(obj,0.0f);
		if (playdead)
		{
			playdead = false;
			if (Commands->Get_Player_Type(obj) == 1)
			{
				Send_Message(255,255,255,"GDI Tiberium Silo destroyed");
				Create_2D_WAV_Sound_Team("m00bgts_kill0001i1evag_snd.wav",1);
				Create_2D_WAV_Sound_Team("m00bgts_kill0002i1evan_snd.wav",0);
			}
			else if (Commands->Get_Player_Type(obj) == 0)
			{
				Send_Message(255,255,255,"Nod Tiberium Silo destroyed");
				Create_2D_WAV_Sound_Team("m00bnts_kill0002i1evag_snd.wav",1);
				Create_2D_WAV_Sound_Team("m00bnts_kill0001i1evan_snd.wav",0);
			}
		}
		Commands->Set_Player_Type(obj,-2);
		givemoney = false;
		playdmg = false;
	}

	if (playdmg)
	{
		playdmg = false;
		if ( (Commands->Get_Player_Type(obj) == 1) && (Commands->Get_Player_Type(damager) != 1) )
		{
			Send_Message_Team(1,255,255,255,"Warning GDI Tiberium Silo under attack");
			Send_Message_Team(0,255,255,255,"GDI Tiberium Silo under attack");
			Create_2D_WAV_Sound_Team("m00bgts_tdfe0001i1evag_snd.wav",1);
			Create_2D_WAV_Sound_Team("m00bgts_tdfe0002i1evan_snd.wav",0);
		}
		if ( (Commands->Get_Player_Type(obj) == 0) && (Commands->Get_Player_Type(damager) != 0) )
		{
			Send_Message_Team(0,255,255,255,"Warning Nod Tiberium Silo under attack");
			Send_Message_Team(1,255,255,255,"Nod Tiberium Silo under attack");
			Create_2D_WAV_Sound_Team("m00bnts_tdfe0002i1evag_snd.wav",1);
			Create_2D_WAV_Sound_Team("m00bnts_tdfe0001i1evan_snd.wav",0);
		}
		Commands->Start_Timer(obj,this,25.0f,205061);//playdmg true
	}
}
void z_Simple_Tiberium_Silo::Timer_Expired(GameObject *obj, int number)
{
	if (number == 205060)
	{
		if (givemoney)
		{
			Commands->Give_Money(obj,2.0f,true);
			Commands->Start_Timer(obj,this,1.0f,205060);
		}
	}
	else if (number == 205061)
	{
		if (playdead)//if its not dead
		{
			playdmg = true;
		}
	}
}
void z_Simple_Tiberium_Silo::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&givemoney,sizeof(givemoney),1);
	Auto_Save_Variable(&playdmg,sizeof(playdmg),2);
	Auto_Save_Variable(&playdead,sizeof(playdead),3);
}

void z_This_Cannot_Be_Repaired::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	 if (damage < 0.0f)
	 {
		 if (Commands->Is_A_Star(damager))
		 {
			damage = damage + (damage - (damage));
			float curhp = Commands->Get_Health(obj);
			Commands->Set_Health(obj,curhp+damage);
		 }
	 }	
}

void z_Endgame_Custom_Count::Created(GameObject *obj)
{
	nod_dead_msg = Get_Int_Parameter("Nod_Died_Message");
	gdi_dead_msg = Get_Int_Parameter("GDI_Died_Message");
	nod_alive_msg = Get_Int_Parameter("Nod_Alive_Message");
	gdi_alive_msg = Get_Int_Parameter("GDI_Alive_Message");
	nod_max_deaths = Get_Int_Parameter("Maximum_Nod_Deaths");
	gdi_max_deaths = Get_Int_Parameter("Maximum_GDI_Deaths");
	nod_dead_count = 0;
	gdi_dead_count = 0;
}
void z_Endgame_Custom_Count::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == nod_dead_msg)
	{
		if (param == 1)
		{
			nod_dead_count++;
			if (nod_dead_count == nod_max_deaths)
			{
				Console_Input("win 1");
			}
		}
	}
	else if (message == gdi_dead_msg)
	{
		if (param == 1)
		{
			gdi_dead_count++;
			if (gdi_dead_count == gdi_max_deaths)
			{
				Console_Input("win 0");
			}
		}
	}

	else if (message == nod_alive_msg)
	{
		if (param == 1)
		{
			nod_dead_count--;
		}
	}
	else if (message == gdi_alive_msg)
	{
		if (param == 1)
		{
			gdi_dead_count--;
		}
	}
}
void z_Endgame_Custom_Count::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&nod_dead_msg,sizeof(nod_dead_msg),1);
	Auto_Save_Variable(&gdi_dead_msg,sizeof(gdi_dead_msg),2);
	Auto_Save_Variable(&nod_dead_count,sizeof(nod_dead_count),3);
	Auto_Save_Variable(&gdi_dead_count,sizeof(gdi_dead_count),4);
	Auto_Save_Variable(&nod_alive_msg,sizeof(nod_alive_msg),5);
	Auto_Save_Variable(&gdi_alive_msg,sizeof(gdi_alive_msg),6);
	Auto_Save_Variable(&nod_max_deaths,sizeof(nod_max_deaths),7);
	Auto_Save_Variable(&gdi_max_deaths,sizeof(gdi_max_deaths),8);
}

void z_Endgame_Custom_Killed::Killed(GameObject *obj, GameObject *shooter)
{
	int counter = Get_Int_Parameter("Endgame_Custom_Count_ID");
	int msg = Get_Int_Parameter("Message");
	Commands->Send_Custom_Event(obj,Commands->Find_Object(counter),msg,1,0);
}

void z_Endgame_Custom_Created::Created(GameObject *obj)
{
	int counter = Get_Int_Parameter("Endgame_Custom_Count_ID");
	int msg = Get_Int_Parameter("Message");
	Commands->Send_Custom_Event(obj,Commands->Find_Object(counter),msg,1,0);
}

void z_Enter_Teleport_Random_Team::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		if (Commands->Get_Player_Type(enter) == Get_Int_Parameter("Team"))
		{
			if (!Get_Vehicle(enter))
			{
				int random = Commands->Get_Random_Int(1,5);
				{
					if (random == 1)
					{
						int x = Get_Int_Parameter("Object_ID1");
						if (x)
						{
							GameObject *gotoObject = Commands->Find_Object(x);
							Vector3 gotoLocation = Commands->Get_Position(gotoObject);
							Commands->Set_Position(enter,gotoLocation);
						}
					}
					else if (random == 2)
					{
						int x = Get_Int_Parameter("Object_ID2");
						if (x)
						{
							GameObject *gotoObject = Commands->Find_Object(x);
							Vector3 gotoLocation = Commands->Get_Position(gotoObject);
							Commands->Set_Position(enter,gotoLocation);
						}
					}
					else if (random == 3)
					{
						int x = Get_Int_Parameter("Object_ID3");
						if (x)
						{
							GameObject *gotoObject = Commands->Find_Object(x);
							Vector3 gotoLocation = Commands->Get_Position(gotoObject);
							Commands->Set_Position(enter,gotoLocation);
						}
					}
					else if (random == 4)
					{
						int x = Get_Int_Parameter("Object_ID4");
						if (x)
						{
							GameObject *gotoObject = Commands->Find_Object(x);
							Vector3 gotoLocation = Commands->Get_Position(gotoObject);
							Commands->Set_Position(enter,gotoLocation);
						}
					}
					else
					{
						int x = Get_Int_Parameter("Object_ID5");
						if (x)
						{
							GameObject *gotoObject = Commands->Find_Object(x);
							Vector3 gotoLocation = Commands->Get_Position(gotoObject);
							Commands->Set_Position(enter,gotoLocation);
						}
					}
				}
			}
		}
	}
}

void z_Vehicle_Death_Notification::Killed(GameObject *obj, GameObject *shooter)
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

void z_Credit_Trickle::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,3.0f,1501981);
}
void z_Credit_Trickle::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1501981)
	{
		Commands->Give_Money(obj,5.0f,true);
		Commands->Start_Timer(obj,this,3.0f,1501981);
	}
}

void z_Capturable_TibSilo::Created(GameObject *obj)
{
	owner = Get_Int_Parameter("Owner");
	Commands->Set_Player_Type(obj,owner);
	Attach_Script_Once(obj,"z_Credit_Trickle","");
	playdmg = true;
	playkilled = true;
}
void z_Capturable_TibSilo::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (damage >= Commands->Get_Health(obj))
	{
		Commands->Set_Health(obj,1.0f);//prevent destruction
		Commands->Set_Player_Type(obj,-2);//unteamed
		Remove_Script(obj,"z_Credit_Trickle");//no more income from this silo
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
				Attach_Script_Once(obj,"z_Credit_Trickle","");
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
		if (damage > 0.5f)
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
		if (damage > 0.5f)
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
void z_Capturable_TibSilo::Timer_Expired(GameObject *obj, int number)
{
	if (number == 155)
	{
		playdmg = true;
	}
}
void z_Capturable_TibSilo::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&playdmg,sizeof(playdmg),1);
	Auto_Save_Variable(&playkilled,sizeof(playkilled),2);
}

void z_Capturable_Repairpad::Created(GameObject *obj)
{
	owner = Get_Int_Parameter("Owner");
	zoneid = Get_Int_Parameter("RepairZoneID");
	Commands->Set_Player_Type(obj,owner);
	Commands->Send_Custom_Event(obj,Commands->Find_Object(zoneid),2500,owner,1);//tell zone what units to repair or none at all
	playdmg = true;
	playkilled = true;
}
void z_Capturable_Repairpad::Damaged(GameObject *obj, GameObject *damager, float damage)
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
			owner = -2;
			Set_Damage_Points(obj,0.0f);
			Commands->Give_Points(damager,150.0f,false);
			Commands->Send_Custom_Event(obj,Commands->Find_Object(zoneid),2500,owner,1);//tell zone what units to repair or none at all
		}
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
		if (damage > 0.5f)
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
		if (damage > 0.5f)
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
void z_Capturable_Repairpad::Timer_Expired(GameObject *obj, int number)
{
	if (number == 155)
	{
		playdmg = true;
	}
}
void z_Capturable_Repairpad::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&playdmg,sizeof(playdmg),1);
	Auto_Save_Variable(&playkilled,sizeof(playkilled),2);
}

void z_Capturable_Repairzone::Created(GameObject *obj)
{
	padid = Get_Int_Parameter("RepairPadID");
	playerobjid = 0;
	owner = -2;
	vehid = 0;
	busy = false;
}
void z_Capturable_Repairzone::Custom(GameObject *obj, int message, int param, GameObject *sender)
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
void z_Capturable_Repairzone::Entered(GameObject *obj, GameObject *enter)
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
void z_Capturable_Repairzone::Exited(GameObject *obj, GameObject *exit)
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
void z_Capturable_Repairzone::Timer_Expired(GameObject *obj, int number)
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
				Commands->Set_Health(Commands->Find_Object(vehid),curhp + 20.0f);
			}
			else if (curap < maxap)
			{
				Commands->Set_Shield_Strength(Commands->Find_Object(vehid),curap + 20.0f);
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
void z_Capturable_Repairzone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&owner,sizeof(owner),1);
	Auto_Save_Variable(&vehid,sizeof(vehid),2);
	Auto_Save_Variable(&busy,sizeof(busy),3);
	Auto_Save_Variable(&playerobjid,sizeof(playerobjid),4);
}

void z_Set_Vehicle_Limit::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,30.0f,3673846);
}
void z_Set_Vehicle_Limit::Timer_Expired(GameObject *obj, int number)
{
	if (number == 3673846)
	{
		char limit[256];		sprintf(limit,"vlimit %d",Get_Int_Parameter("Limit"));																Console_Input(limit);
		char limitmsg[512];		sprintf(limit,"msg Vehicle Limit for this map has been set to: %d vehicles per team",Get_Int_Parameter("Limit"));	Console_Input(limitmsg);
	}
}

void z_Set_Time_Limit_Created::Created(GameObject *obj)
{
	char limit[256];	sprintf(limit,"time %d",Get_Int_Parameter("Time_Seconds"));	Console_Input(limit);
}

void z_Set_Mine_Limit_Created::Created(GameObject *obj)
{
	char limit[256];	sprintf(limit,"mlimit %d",Get_Int_Parameter("Limit"));	Console_Input(limit);
}

void z_Min_Veh_Z_Pos::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,2.0f,27645352);
}
void z_Min_Veh_Z_Pos::Timer_Expired(GameObject *obj, int number)
{
	if (number == 27645352)
	{
		Vector3 curpos = Commands->Get_Position(obj);
		if (curpos.Z < Get_Float_Parameter("Min"))
		{
			Commands->Apply_Damage(obj,99999.0f,"Death",0);
		}
		else
		{
			Commands->Start_Timer(obj,this,2.0f,27645352);
		}
	}
}

ScriptRegistrant<z_Min_Veh_Z_Pos> z_Min_Veh_Z_Pos_Registrant("z_Min_Veh_Z_Pos","Min=0:float");
ScriptRegistrant<z_Set_Mine_Limit_Created> z_Set_Mine_Limit_Created_Registrant("z_Set_Mine_Limit_Created","Limit=36:int");
ScriptRegistrant<z_Set_Time_Limit_Created> z_Set_Time_Limit_Created_Registrant("z_Set_Time_Limit_Created","Time_Seconds=3600:int");
ScriptRegistrant<z_Set_Vehicle_Limit> z_Set_Vehicle_Limit_Registrant("z_Set_Vehicle_Limit","Limit=13:int");
ScriptRegistrant<z_Capturable_Repairzone> z_Capturable_Repairzone_Registrant("z_Capturable_Repairzone","RepairPadID=0:int");
ScriptRegistrant<z_Capturable_Repairpad> z_Capturable_Repairpad_Registrant("z_Capturable_Repairpad","Owner=0:int,RepairZoneID=0:int");
ScriptRegistrant<z_Capturable_TibSilo> z_Capturable_TibSilo_Registrant("z_Capturable_TibSilo","Owner=0:int");
ScriptRegistrant<z_Credit_Trickle> z_Credit_Trickle_Registrant("z_Credit_Trickle","");
ScriptRegistrant<z_Vehicle_Death_Notification> z_Vehicle_Death_Notification_Registrant("z_Vehicle_Death_Notification","");
ScriptRegistrant<z_Enter_Teleport_Random_Team> z_Enter_Teleport_Random_Team_Registrant("z_Enter_Teleport_Random_Team","Team=0:int,Object_ID1=0:int,Object_ID2=0:int,Object_ID3=0:int,Object_ID4=0:int,Object_ID5=0:int");
ScriptRegistrant<z_Endgame_Custom_Created> z_Endgame_Custom_Created_Registrant("z_Endgame_Custom_Created","Endgame_Custom_Count_ID=0:int,Message=1000:int");
ScriptRegistrant<z_Endgame_Custom_Killed> z_Endgame_Custom_Killed_Registrant("z_Endgame_Custom_Killed","Endgame_Custom_Count_ID=0:int,Message=1000:int");
ScriptRegistrant<z_Endgame_Custom_Count> z_Endgame_Custom_Count_Registrant("z_Endgame_Custom_Count","Nod_Died_Message=1000:int,Nod_Alive_Message=1500:int,Maximum_Nod_Deaths=8:int,GDI_Died_Message=2000:int,GDI_Alive_Message=2500:int,Maximum_GDI_Deaths=8:int");
ScriptRegistrant<z_This_Cannot_Be_Repaired> z_This_Cannot_Be_Repaired_Registrant("z_This_Cannot_Be_Repaired","");
ScriptRegistrant<z_Simple_Tiberium_Silo> z_Simple_Tiberium_Silo_Registrant("z_Simple_Tiberium_Silo","");
ScriptRegistrant<z_Locked_Vehicle> z_Locked_Vehicle_Registrant("z_Locked_Vehicle","Team=0:int");
ScriptRegistrant<z_Building_Health_Notifications> z_Building_Health_Notifications_Registrant("z_Building_Health_Notifications","");