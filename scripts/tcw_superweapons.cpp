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
#include "General.h"
#include "scripts.h"
#include "engine.h"
#include "BuildingGameObj.h"
#include "tcw_superweapons.h"
#include "GameObjManager.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"


//ION SCRIPTS HERE
void tcw_i_have_ion_beacon::Destroyed(GameObject *obj)
{
	Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_pt_ion"),1000,1,0);//someone has ion is false
}

void tcw_pt_ion::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	allowpoke = true;
	ionready = false;
	Create_2D_WAV_Sound_Team("gdi_ion_cannon_charging.wav",1);
	Send_Message_Team(1,255,255,255,"Ion Cannon charging");
	Commands->Start_Timer(obj,this,240.0f,100100);//420=7min
}
void tcw_pt_ion::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 2)//successful beacon placed, start recharging
		{
			ionready = false;
			Commands->Start_Timer(obj,this,240.0f,100100);//recharge the ion
		}
	}
}
void tcw_pt_ion::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100100)
	{
		ionready = true;
		Create_2D_WAV_Sound_Team("gdi_IonCannReady.wav",1);
		Send_Message_Team(1,255,255,255,"Ion Cannon ready");	
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_gdi_ion_cannon_building"),1000,1,0);
	}
	else if (number == 100101)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}
void tcw_pt_ion::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (Commands->Get_Player_Type(poker) == 1)
		{
			if ( (strcmp(Commands->Get_Preset_Name(poker),"TCW_GDI_Commando") == 0) || (strcmp(Commands->Get_Preset_Name(poker),"TCW_GDI_Commando_Low") == 0) )
			{
				Send_Message_Player(poker,255,255,255,"Commando's are not allowed to carry the Ion Cannon Beacon");
			}
			else
			{
				if (Is_Building_Dead(Find_Building_By_Preset(1,"TCW_GDI_Ion_Center")))
				{
					Send_Message_Player(poker,255,255,255,"Your Ion Cannon Center is destroyed. The Ion Cannon is unavailable.");
				}
				else
				{
					allowpoke = false;
					Commands->Enable_HUD_Pokable_Indicator(obj, false);
					Commands->Start_Timer(obj,this,5.0f,100101);//alowpoke true
					if (Find_Object_With_Script("tcw_i_have_ion_beacon"))//someone has ion already
					{
						char currholdermsg[250];
						sprintf(currholdermsg,"Sorry, %s currently has the Ion Cannon Beacon.",Get_Player_Name(Find_Object_With_Script("tcw_i_have_ion_beacon")));
						Send_Message_Player(poker,255,255,255,currholdermsg);
					}
					else
					{
						if (ionready)
						{
							Attach_Script_Once(poker,"tcw_i_have_ion_beacon","");
							Commands->Give_PowerUp(poker,"TCW_POW_Beacon_IonCannon_60Sec",false);
							char pickupmsg[250];
							sprintf(pickupmsg,"%s picked up the Ion Cannon Beacon",Get_Player_Name(poker));
							Send_Message_Team(1,255,204,0,pickupmsg);
							Send_Message_Player(poker,255,205,0,"Select Target");
							Create_2D_WAV_Sound_Player(poker,"gdi_selecttarget.wav");
						}
						else
						{
							char notreadymsg[250];
							sprintf(notreadymsg,"Sorry %s, the Ion Cannon is still charging up.",Get_Player_Name(poker));
							Send_Message_Player(poker,255,255,255,notreadymsg);
						}
					}
				}
			}
		}
	}
}
void tcw_pt_ion::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowpoke,sizeof(allowpoke),1);
	Auto_Save_Variable(&ionready,sizeof(ionready),2);
}

void tcw_gdi_ion_cannon_building::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 0)
		{
			Commands->Set_Animation(obj,"V_GDI_IONCTR.V_GDI_IONCTR",false,0,40,90,false);//go down
		}
		else if (param == 1)
		{
			Commands->Set_Animation(obj,"V_GDI_IONCTR.V_GDI_IONCTR",false,0,0,40,false);//go up
		}
	}
}
void tcw_gdi_ion_cannon_building::Killed(GameObject *obj, GameObject *shooter)
{
	int carrier = Commands->Get_ID(Find_Object_With_Script("tcw_i_have_ion_beacon"));
	if (carrier)
	{
		Commands->Select_Weapon(Commands->Find_Object(carrier),"TCW_Weapon_Knife");
		Remove_Weapon(Commands->Find_Object(carrier),"TCW_Weapon_IonCannon_60Sec");
	}
	Create_2D_WAV_Sound_Team("gdi_cancelled.wav",1);
	Send_Message_Team(1,255,255,255,"Ion Cannon Cancelled");
}


//NUKE SCRIPTS HERE
void tcw_i_have_nuke_beacon::Destroyed(GameObject *obj)
{
	Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_pt_nuke"),1000,1,0);//someone has nuke is false
}

void tcw_pt_nuke::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	allowpoke = true;
	ionready = false;
	Create_2D_WAV_Sound_Team("nod_nuke_charging.wav",0);
	Send_Message_Team(0,255,255,255,"Nuclear Missile charging");
	Commands->Start_Timer(obj,this,240.0f,100100);
}
void tcw_pt_nuke::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 2)//successful beacon placed, start recharging
		{
			ionready = false;
			Commands->Start_Timer(obj,this,240.0f,100100);//recharge the nuke
		}
	}
}
void tcw_pt_nuke::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100100)
	{
		ionready = true;
		Create_2D_WAV_Sound_Team("nod_NuclearMissReady.wav",0);
		Send_Message_Team(0,255,255,255,"Nuclear Missile ready");	
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_nod_nuke_building"),1000,1,0);
	}
	else if (number == 100101)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}
void tcw_pt_nuke::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (Commands->Get_Player_Type(poker) == 0)
		{
			if ( (strcmp(Commands->Get_Preset_Name(poker),"TCW_Nod_Commando") == 0) || (strcmp(Commands->Get_Preset_Name(poker),"TCW_Nod_Commando_Low") == 0) )
			{
				Send_Message_Player(poker,255,255,255,"Commando's are not allowed to carry the Nuclear Missile Beacon");
			}
			else 
			{
				if (Is_Building_Dead(Find_Building_By_Preset(0,"TCW_Temple_of_Nod")))
				{
					Send_Message_Player(poker,255,255,255,"Your Temple of Nod is destroyed. The Nuclear Missile is unavailable.");
				}
				else
				{
					allowpoke = false;
					Commands->Enable_HUD_Pokable_Indicator(obj, false);
					Commands->Start_Timer(obj,this,5.0f,100101);//alowpoke true
					if (Find_Object_With_Script("tcw_i_have_nuke_beacon"))
					{
						char currholdermsg[250];
						sprintf(currholdermsg,"Sorry, %s currently has the Nuclear Missile Beacon.",Get_Player_Name(Find_Object_With_Script("tcw_i_have_nuke_beacon")));
						Send_Message_Player(poker,255,255,255,currholdermsg);
					}
					else
					{
						if (ionready)
						{
							Attach_Script_Once(poker,"tcw_i_have_nuke_beacon","");
							Commands->Give_PowerUp(poker,"TCW_POW_Beacon_Nuke_60Sec",false);
							char pickupmsg[250];
							sprintf(pickupmsg,"%s picked up the Nuclear Missile Beacon",Get_Player_Name(poker));
							Send_Message_Team(0,255,0,0,pickupmsg);
							Send_Message_Player(poker,255,0,0,"Select Target");
							Create_2D_WAV_Sound_Player(poker,"nod_selecttarget.wav");
						}
						else
						{
							char notreadymsg[250];
							sprintf(notreadymsg,"Sorry %s, the Nuclear Missile is still charging up.",Get_Player_Name(poker));
							Send_Message_Player(poker,255,255,255,notreadymsg);
						}
					}
				}
			}
		}
	}
}
void tcw_pt_nuke::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowpoke,sizeof(allowpoke),1);
	Auto_Save_Variable(&ionready,sizeof(ionready),2);
}

void tcw_nod_nuke_building::Created(GameObject *obj)
{
	launchready = 0;
	launchnuke = 0;
}
void tcw_nod_nuke_building::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 0)
		{
			float facing = Commands->Get_Facing(obj);
			Vector3 launchpos = Commands->Get_Position(obj);
			GameObject *nukefire = Commands->Create_Object("TCW_Nuclear_Missile_Launch",launchpos);
			launchnuke = Commands->Get_ID(nukefire);
			Commands->Set_Facing(nukefire,facing);
			Commands->Set_Animation(nukefire,"V_MISSLE_FIRE.V_MISSLE_FIRE",false,0,0,184,false);//fire missile animation starts
			Commands->Destroy_Object(Commands->Find_Object(launchready));//remove the nuke idle animation
			Commands->Start_Timer(obj,this,10.0f,100100);//remove nuke fire thingy	
		}
		else if (param == 1)
		{
			float facing = Commands->Get_Facing(obj);
			Vector3 launchpos = Commands->Get_Position(obj);
			GameObject *nukeready = Commands->Create_Object("TCW_Nuclear_Missile_Ready",launchpos);
			launchready = Commands->Get_ID(nukeready);
			Commands->Set_Facing(nukeready,facing);
			Commands->Set_Animation(nukeready,"V_MISSLE_RELOAD.V_MISSLE_RELOAD",false,0,0,40,false);//nuke ready idle animation
		}
	}
}
void tcw_nod_nuke_building::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100100)
	{
		Commands->Destroy_Object(Commands->Find_Object(launchnuke));//destroy the nuke firing animation after a few seconds
	}
}
void tcw_nod_nuke_building::Killed(GameObject *obj, GameObject *shooter)
{
	int carrier = Commands->Get_ID(Find_Object_With_Script("tcw_i_have_nuke_beacon"));
	if (carrier)
	{
		Commands->Select_Weapon(Commands->Find_Object(carrier),"TCW_Weapon_Knife");
		Remove_Weapon(Commands->Find_Object(carrier),"TCW_Weapon_NukeStrike_60Sec");
	}

	if (launchready)
	{
		Commands->Destroy_Object(Commands->Find_Object(launchready));
	}
	Create_2D_WAV_Sound_Team("nod_cancelled.wav",0);
	Send_Message_Team(0,255,255,255,"Nuclear Missile Cancelled");
}
void tcw_nod_nuke_building::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&launchready,sizeof(launchready),1);
	Auto_Save_Variable(&launchnuke,sizeof(launchnuke),2);
}

ScriptRegistrant<tcw_nod_nuke_building> tcw_nod_nuke_building_Registrant("tcw_nod_nuke_building","");
ScriptRegistrant<tcw_pt_nuke> tcw_pt_nuke_Registrant("tcw_pt_nuke","");
ScriptRegistrant<tcw_i_have_nuke_beacon> tcw_i_have_nuke_beacon_Registrant("tcw_i_have_nuke_beacon","");

ScriptRegistrant<tcw_gdi_ion_cannon_building> tcw_gdi_ion_cannon_building_Registrant("tcw_gdi_ion_cannon_building","");
ScriptRegistrant<tcw_pt_ion> tcw_pt_ion_Registrant("tcw_pt_ion","");
ScriptRegistrant<tcw_i_have_ion_beacon> tcw_i_have_ion_beacon_Registrant("tcw_i_have_ion_beacon","");