/*	Renegade Scripts.dll
	Scripts by from - http://www.multiplayerforums.com
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
#include "engine.h"
#include "BuildingGameObj.h"
#include "z_airraid.h"
#include "GameObjManager.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"
#ifdef SSGM
#include "gmgame.h"
#endif

void z_air_Tiberium_Silo::Created(GameObject *obj)
{
	playdead = true;
	playdmg = true;
	givemoney = true;
	disablerepair = false;
	Commands->Start_Timer(obj,this,1.0f,205060);
}
void z_air_Tiberium_Silo::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Get_Health(obj) <= 2.0f)//its destroyed
	{
		Commands->Set_Health(obj,2.0f);
		Set_Damage_Points(obj,0.0f);
		Set_Death_Points(obj,0.0f);
		if (playdead)
		{
			disablerepair = true;
			playdead = false;
			if (Commands->Get_Player_Type(obj) == 1)
			{
				Send_Message(255,255,255,"GDI Tiberium Silo destroyed");
				Create_2D_WAV_Sound_Team("m00bgts_kill0001i1evag_snd.wav",1);
				Create_2D_WAV_Sound_Team("m00bgts_kill0002i1evan_snd.wav",0);
				GameObject *targetgdi = Find_Object_With_Script("z_Endgame_Custom_Count");
				Commands->Send_Custom_Event(obj,targetgdi,2000,1,0);
			}
			else if (Commands->Get_Player_Type(obj) == 0)
			{
				Send_Message(255,255,255,"Nod Tiberium Silo destroyed");
				Create_2D_WAV_Sound_Team("m00bnts_kill0002i1evag_snd.wav",1);
				Create_2D_WAV_Sound_Team("m00bnts_kill0001i1evan_snd.wav",0);
				GameObject *targetnod = Find_Object_With_Script("z_Endgame_Custom_Count");
				Commands->Send_Custom_Event(obj,targetnod,1000,1,0);
			}
			Commands->Give_Points(damager,250.0f,false);
		}
		Commands->Set_Player_Type(obj,-2);
		givemoney = false;
		playdmg = false;
	}

	if (damage < 0.0f)
	{
		if (disablerepair)
		{
			Commands->Set_Health(obj,1.0f);
		}
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
void z_air_Tiberium_Silo::Timer_Expired(GameObject *obj, int number)
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
void z_air_Tiberium_Silo::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&givemoney,sizeof(givemoney),1);
	Auto_Save_Variable(&playdmg,sizeof(playdmg),2);
	Auto_Save_Variable(&playdead,sizeof(playdead),3);
	Auto_Save_Variable(&disablerepair,sizeof(disablerepair),4);
}

void z_air_Repairpad::Created(GameObject *obj)
{
	owner = Get_Int_Parameter("Owner");
	zoneid = Get_Int_Parameter("RepairZoneID");
	playdmg = true;
	playkilled = true;
	disablerepair = false;
}
void z_air_Repairpad::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (damage >= Commands->Get_Health(obj))
	{
		Commands->Set_Health(obj,1.0f);//prevent destruction
		Commands->Set_Player_Type(obj,-2);//unteamed
		if (playkilled)
		{
			disablerepair = true;
			if (owner == 0)//nod pad killed
			{
				Create_2D_WAV_Sound_Team("m00bnrf_kill0001i1evan_snd.wav",0);
				Create_2D_WAV_Sound_Team("m00bnrf_kill0002i1evag_snd.wav",1);
				Send_Message(255,255,255,"Nod Repair Facility destroyed");
				GameObject *targetnod = Find_Object_With_Script("z_Endgame_Custom_Count");
				Commands->Send_Custom_Event(obj,targetnod,1000,1,0);
			}
			else if (owner == 1)//gdi pad killed
			{
				Create_2D_WAV_Sound_Team("m00bgrf_kill0002i1evan_snd.wav",0);
				Create_2D_WAV_Sound_Team("m00bgrf_kill0001i1evag_snd.wav",1);
				Send_Message(255,255,255,"GDI Repair Facility destroyed");
				GameObject *targetgdi = Find_Object_With_Script("z_Endgame_Custom_Count");
				Commands->Send_Custom_Event(obj,targetgdi,2000,1,0);
			}
			Set_Damage_Points(obj,0.0f);
			Commands->Give_Points(damager,250.0f,false);
			Commands->Send_Custom_Event(obj,Commands->Find_Object(zoneid),2500,owner,1);//tell zone to activate
		}
		playdmg = false;
		playkilled = false;
	}

	if (damage < 0.0f)
	{
		if (disablerepair)
		{
			Commands->Set_Health(obj,1.0f);
		}
	}

	if (owner == 0)//nod pad
	{
		if (damage > 0.2f)
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
		if (damage > 0.2f)
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
void z_air_Repairpad::Timer_Expired(GameObject *obj, int number)
{
	if (number == 155)
	{
		playdmg = true;
	}
}
void z_air_Repairpad::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&playdmg,sizeof(playdmg),1);
	Auto_Save_Variable(&playkilled,sizeof(playkilled),2);
	Auto_Save_Variable(&owner,sizeof(owner),3);
	Auto_Save_Variable(&zoneid,sizeof(zoneid),4);
	Auto_Save_Variable(&disablerepair,sizeof(disablerepair),5);
}

void z_air_Repairzone::Created(GameObject *obj)
{
	padid = Get_Int_Parameter("RepairPadID");
	playerobjid = 0;
	owner = Get_Int_Parameter("Owner");
	vehid = 0;
	busy = false;
}
void z_air_Repairzone::Entered(GameObject *obj, GameObject *enter)
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
						if ( (Commands->Get_Health(Get_Vehicle(enter)) < Commands->Get_Max_Health(Get_Vehicle(enter))) || (Commands->Get_Shield_Strength(Get_Vehicle(enter)) < Commands->Get_Max_Shield_Strength(Get_Vehicle(enter))) )
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
}
void z_air_Repairzone::Exited(GameObject *obj, GameObject *exit)
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
void z_air_Repairzone::Timer_Expired(GameObject *obj, int number)
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
			if (Commands->Get_Max_Shield_Strength(Commands->Find_Object(vehid)) == Commands->Get_Shield_Strength(Commands->Find_Object(vehid)))
			{
				if (owner == 0)
				{
					if (Commands->Find_Object(playerobjid))
					{
						Send_Message_Player(Commands->Find_Object(playerobjid),255,0,0,"Vehicle Repaired");
						Create_2D_WAV_Sound_Player(Commands->Find_Object(playerobjid),"m00evag_dsgn0019i1evag_snd.wav");
					}
				}
				else if (owner == 1)
				{
					if (Commands->Find_Object(playerobjid))
					{
						Send_Message_Player(Commands->Find_Object(playerobjid),255,204,0,"Vehicle Repaired");
						Create_2D_WAV_Sound_Player(Commands->Find_Object(playerobjid),"m00evag_dsgn0019i1evag_snd.wav");
					}
				}
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
void z_air_Repairzone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&owner,sizeof(owner),1);
	Auto_Save_Variable(&vehid,sizeof(vehid),2);
	Auto_Save_Variable(&busy,sizeof(busy),3);
	Auto_Save_Variable(&playerobjid,sizeof(playerobjid),4);
	Auto_Save_Variable(&padid,sizeof(padid),5);
}

void z_air_Carrier_Control_Point::Created(GameObject *obj)
{
	Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);
	nodename = Get_Parameter("Node_Name");
	owner = -2;
	trigger_neutral_damage = true;
}
void z_air_Carrier_Control_Point::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (damage < 0.0f)//being repaired
	{
		if (Commands->Get_Player_Type(obj) == -2)
		{
			trigger_neutral_damage = true;
			int team = Commands->Get_Player_Type(damager);
			if (team == 0)
			{
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100191),1000,0,0);//refilzones reset
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100192),1000,0,0);
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100193),1000,0,0);
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100194),1000,0,0);

				owner = 0;
				Commands->Set_Player_Type(obj,0);
				Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,0,0,false);

				char capturemsg[250];	sprintf(capturemsg,"Nod captured the %s Control Point.",nodename);	Send_Message(255,255,255,capturemsg);
				Create_2D_WAV_Sound_Team("ControlPointCaptured_Nod.wav",0); 
				Send_Message_Team(0,255,0,0,"Your team can now refill helicopters on the Aircraft Carrier deck");
			}
			else if (team == 1)
			{
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100191),1000,1,0);//refilzones reset
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100192),1000,1,0);
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100193),1000,1,0);
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100194),1000,1,0);

				owner = 1;
				Commands->Set_Player_Type(obj,1);
				Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,1,1,false);

				char capturemsg[250];	sprintf(capturemsg,"GDI captured the %s Control Point.",nodename);	Send_Message(255,255,255,capturemsg);
				Create_2D_WAV_Sound_Team("ControlPointCaptured_GDI.wav",1);
				Send_Message_Team(1,255,204,0,"Your team can now refill helicopters on the Aircraft Carrier deck");
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
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100191),1000,2,0);//refilzones reset
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100192),1000,2,0);
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100193),1000,2,0);
				Commands->Send_Custom_Event(obj,Commands->Find_Object(100194),1000,2,0);

				if (owner == 0)
				{
					Create_2D_WAV_Sound_Team("ControlPointLost_Nod.wav",0);
					Send_Message_Team(0,255,0,0,"Aircraft Carrier Control Point Lost");
				}
				else if (owner == 1)
				{
					Create_2D_WAV_Sound_Team("ControlPointLost_GDI.wav",1);
					Send_Message_Team(1,255,204,0,"Aircraft Carrier Control Point Lost");
				}
				owner = -2;
			}
		}
	}
}
void z_air_Carrier_Control_Point::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&nodename,sizeof(nodename),1);
	Auto_Save_Variable(&owner,sizeof(owner),2);
	Auto_Save_Variable(&trigger_neutral_damage,sizeof(trigger_neutral_damage),3);
}

void z_air_Helipad_Zone::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	vehid = 0;
	busy = false;
}
void z_air_Helipad_Zone::Entered(GameObject *obj, GameObject *enter)
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
void z_air_Helipad_Zone::Exited(GameObject *obj, GameObject *exit)
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
void z_air_Helipad_Zone::Timer_Expired(GameObject *obj, int number)
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
void z_air_Helipad_Zone::Custom(GameObject *obj, int message, int param, GameObject *sender)
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
void z_air_Helipad_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&team,sizeof(team),1);
	Auto_Save_Variable(&vehid,sizeof(vehid),2);
	Auto_Save_Variable(&busy,sizeof(busy),3);
}

void z_air_Helipad::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	playdmg = true;
	playkilled = true;
	disablerepair = false;
}
void z_air_Helipad::Damaged(GameObject *obj, GameObject *damager, float damage)
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
				GameObject *targetnod = Find_Object_With_Script("z_Endgame_Custom_Count");
				Commands->Send_Custom_Event(obj,targetnod,1000,1,0);
			}
			else if (team == 1)//it is gdi helipad
			{
				Send_Message(255,255,255,"GDI Helicopter Pad destroyed");
				Create_2D_WAV_Sound_Team("m00bghp_kill0002i1evan_snd.wav",0);
				Create_2D_WAV_Sound_Team("m00bghp_kill0001i1evag_snd.wav",1);
				GameObject *targetgdi = Find_Object_With_Script("z_Endgame_Custom_Count");
				Commands->Send_Custom_Event(obj,targetgdi,2000,1,0);
			}
			Commands->Destroy_Object(Commands->Find_Object(Get_Int_Parameter("RefillZoneID")));//stop refills :D
			Commands->Give_Points(damager,250.0f,false);
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
void z_air_Helipad::Timer_Expired(GameObject *obj, int number)
{
	if (number == 150)
	{
		playdmg = true;
	}
}
void z_air_Helipad::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&playdmg,sizeof(playdmg),1);
	Auto_Save_Variable(&playkilled,sizeof(playkilled),2);
	Auto_Save_Variable(&team,sizeof(team),3);
	Auto_Save_Variable(&disablerepair,sizeof(disablerepair),4);
}

void z_air_Buy_Heli::Created(GameObject *obj)
{
	hpadid = Get_Int_Parameter("HelipadID");
	type = Get_Parameter("Type");
	cost = Get_Float_Parameter("Cost");
	team = Get_Int_Parameter("Team");
	buyerid = 0;
	allowpoke = true; Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void z_air_Buy_Heli::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&hpadid,sizeof(hpadid),1);
	Auto_Save_Variable(&type,sizeof(type),2);
	Auto_Save_Variable(&cost,sizeof(cost),3);
	Auto_Save_Variable(&team,sizeof(team),4);
	Auto_Save_Variable(&buyerid,sizeof(buyerid),5);
	Auto_Save_Variable(&allowpoke,sizeof(allowpoke),6);
}
void z_air_Buy_Heli::Poked(GameObject *obj, GameObject *poker)
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
		else
		{
			if (team == 0)
			{
				Send_Message_Player(poker,255,0,0,"Access Denied: This Helicopter Pad is dead");
				Create_2D_WAV_Sound_Player(poker,"m00evag_dsgn0039i1evag_snd.wav");
			}
			else if (team == 1)
			{
				Send_Message_Player(poker,255,204,0,"Access Denied: This Helicopter Pad is dead");
				Create_2D_WAV_Sound_Player(poker,"m00evag_dsgn0039i1evag_snd.wav");
			}
		}
	}
}
void z_air_Buy_Heli::Timer_Expired(GameObject *obj, int number)
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

void z_air_ApacheOrca::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if ( (strcmp(Commands->Get_Preset_Name(sender),"air_GDI_Sniper") == 0) || (strcmp(Commands->Get_Preset_Name(sender),"air_Nod_Sniper") == 0) )
		{
			Send_Message_Player(sender,255,255,255,"Snipers are not allowed to enter Apache's or Orca's");
			Commands->Start_Timer(obj,this,1.0f,235235);
		}
	}
}
void z_air_ApacheOrca::Timer_Expired(GameObject *obj, int number)
{
	if (number == 235235)
	{
		Force_Occupant_Exit(obj,0);
	}
}

void z_air_SniperLimit::Created(GameObject *obj)
{
	int limit = 5;
	if (strcmp(Commands->Get_Preset_Name(obj),"air_GDI_Sniper") == 0)
	{
		if (Get_Object_Count(1,"air_GDI_Sniper") > limit)
		{
			Send_Message_Player(obj,255,204,0,"Sniper Limit of 5 reached, try again later");
			Commands->Give_Money(obj,500.0f,false);
			Change_Character(obj,"CnC_GDI_MiniGunner_0");
		}
	}
	else if (strcmp(Commands->Get_Preset_Name(obj),"air_Nod_Sniper") == 0)
	{
		if (Get_Object_Count(0,"air_Nod_Sniper") > limit)
		{
			Send_Message_Player(obj,255,0,0,"Sniper Limit of 5 reached, try again later");
			Commands->Give_Money(obj,500.0f,false);
			Change_Character(obj,"CnC_Nod_Minigunner_0");
		}
	}
}

ScriptRegistrant<z_air_SniperLimit> z_air_SniperLimit_Registrant("z_air_SniperLimit","");
ScriptRegistrant<z_air_ApacheOrca> z_air_ApacheOrca_Registrant("z_air_ApacheOrca","");
ScriptRegistrant<z_air_Buy_Heli> z_air_Buy_Heli_Registrant("z_air_Buy_Heli","HelipadID=0:int,Type=air_Nod_Apache:string,Team=0:int,Cost=70000:float,Location=0:vector3,Facing=0:float");
ScriptRegistrant<z_air_Helipad> z_air_Helipad_Registrant("z_air_Helipad","Team=0:int,RefillZoneID=0:int");
ScriptRegistrant<z_air_Helipad_Zone> z_air_Helipad_Zone_Registrant("z_air_Helipad_Zone","Team=0:int");
ScriptRegistrant<z_air_Carrier_Control_Point> z_air_Carrier_Control_Point_Registrant("z_air_Carrier_Control_Point","Node_Name=Carrier:string");
ScriptRegistrant<z_air_Repairzone> z_air_Repairzone_Registrant("z_air_Repairzone","Owner=0:int,RepairPadID=0:int");
ScriptRegistrant<z_air_Repairpad> z_air_Repairpad_Registrant("z_air_Repairpad","Owner=0:int,RepairZoneID=0:int");
ScriptRegistrant<z_air_Tiberium_Silo> z_air_Tiberium_Silo_Registrant("z_air_Tiberium_Silo","");