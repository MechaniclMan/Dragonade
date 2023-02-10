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
#include "z_prototype.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"
#ifdef SSGM
#include "gmgame.h"
#endif


void z_Pro_Mammy_Death_Counter::Created(GameObject *obj)
{
	Commands->Set_Animation(Commands->Find_Object(100003),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false);
	Commands->Set_Animation(Commands->Find_Object(100004),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false);
	Commands->Set_Animation(Commands->Find_Object(100005),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false);
	Commands->Start_Timer(obj,this,30.0f,99);//spam map creator msg
	Commands->Start_Timer(obj,this,119.0f,200);//spam what to do
	deaths = 0;
	dropcount = 0;
	dodrops = true;
}
void z_Pro_Mammy_Death_Counter::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			deaths++;
			if (deaths == 3)
			{
				Commands->Start_Timer(obj,this,1.0f,101);
			}
		}
	}
	else if (message == 3000)
	{
		if (param == 1)
		{
			if (dodrops)
			{
				dropcount++;
				if (dropcount == 3)	{	dodrops = false;	}
				const char *pos1cin = "gdi_infantry_1.txt";		
				const char *pos2cin = "gdi_infantry_2.txt";
				const char *pos3cin = "gdi_infantry_3.txt";
				Vector3 pos1 = Commands->Get_Position(Commands->Find_Object(100132)); float pos1fac = -57.634f;
				Vector3 pos2 = Commands->Get_Position(Commands->Find_Object(100133)); float pos2fac = 90.000f;
				Vector3 pos3 = Commands->Get_Position(Commands->Find_Object(100134)); float pos3fac = 180.000f;
				GameObject *dropos1 = Commands->Create_Object("Invisible_Object",pos1);
				GameObject *dropos2 = Commands->Create_Object("Invisible_Object",pos2);
				GameObject *dropos3 = Commands->Create_Object("Invisible_Object",pos3);
				Commands->Set_Facing(dropos1,pos1fac);	Commands->Set_Facing(dropos1,pos1fac);
				Commands->Set_Facing(dropos2,pos2fac);	Commands->Set_Facing(dropos2,pos2fac);
				Commands->Set_Facing(dropos3,pos3fac);	Commands->Set_Facing(dropos3,pos3fac);
				Commands->Attach_Script(dropos1,"Test_Cinematic",pos1cin);
				Commands->Attach_Script(dropos2,"Test_Cinematic",pos2cin);
				Commands->Attach_Script(dropos3,"Test_Cinematic",pos3cin);
			}
		}
	}
}
void z_Pro_Mammy_Death_Counter::Timer_Expired(GameObject *obj, int number)
{
	if (number == 99)
	{
		Console_Input("msg Running C&C_Prototype by zunnie, Mauler and Goldy58");
		Console_Input("msg Timer Initiated: 45 Minutes remaining");
		Commands->Start_Timer(obj,this,300.0f,201);//40mins remaining when expired
	}
	else if (number == 101)//nod lost, mammy stealing failed 3 times or time expired
	{
		Send_Message_Team(0,255,0,0,"Your mission is a failure");
		Create_2D_WAV_Sound_Team("00-n100e.wav",0);
		Commands->Start_Timer(obj,this,5.0f,102);//punish them for losing :D
		Console_Input("amsg GDI has successfully protected the Prototypes, GDI WINS!");
	}
	else if (number == 103)
	{
		Console_Input("win 1");
	}
	else if (number == 200)
	{
		Send_Message_Team(0,255,0,0,"Steal the GDI Prototype Mammoth 2 and drive it to the start of map");
		Send_Message_Team(0,255,0,0,"Steal the GDI Prototype Mammoth 2 and drive it to the start of map");
		Send_Message_Team(0,255,0,0,"Steal the GDI Prototype Mammoth 2 and drive it to the start of map");
		Send_Message_Team(1,255,204,0,"Prevent Nod from stealing one of the GDI Prototype Mammoth 2 tanks");
		Send_Message_Team(1,255,204,0,"Prevent Nod from stealing one of the GDI Prototype Mammoth 2 tanks");
		Send_Message_Team(1,255,204,0,"Prevent Nod from stealing one of the GDI Prototype Mammoth 2 tanks");
		Commands->Start_Timer(obj,this,119.0f,200);
	}
	else if (number == 201)
	{
		Console_Input("msg TIMER: 40 minutes remaining...");
		Commands->Start_Timer(obj,this,300.0f,202);//35mins remaining when expired
	}
	else if (number == 202)
	{
		Console_Input("msg TIMER: 35 minutes remaining...");
		Commands->Start_Timer(obj,this,300.0f,203);//30mins remaining when expired
	}
	else if (number == 203)
	{
		Console_Input("msg TIMER: 30 minutes remaining...");
		Commands->Start_Timer(obj,this,300.0f,204);//25mins remaining when expired
	}
	else if (number == 204)
	{
		Console_Input("msg TIMER: 25 minutes remaining...");
		Commands->Start_Timer(obj,this,300.0f,205);//20mins remaining when expired
	}
	else if (number == 205)
	{
		Console_Input("msg TIMER: 20 minutes remaining...");
		Commands->Start_Timer(obj,this,300.0f,206);//15mins remaining when expired
	}
	else if (number == 206)
	{
		Console_Input("msg TIMER: 15 minutes remaining...");
		Commands->Start_Timer(obj,this,300.0f,207);//10mins remaining when expired
	}
	else if (number == 207)
	{
		Console_Input("msg TIMER: 10 minutes remaining...");
		Commands->Start_Timer(obj,this,300.0f,208);//5mins remaining when expired
	}
	else if (number == 208)
	{
		Console_Input("msg TIMER: 5 minutes remaining...");
		Commands->Start_Timer(obj,this,300.0f,209);//1mins remaining when expired
	}
	else if (number == 209)
	{
		Console_Input("msg TIMER: 1 minute remaining...");
		Commands->Start_Timer(obj,this,60.0f,210);//1mins remaining when expired
	}
	else if (number == 210)
	{
		Console_Input("amsg GDI has successfully protected the Prototype Mammoth 2 Tanks, GDI WINS!");
		Send_Message(255,255,255,"All Nod players are cancelled for losing");
		Create_2D_WAV_Sound_Team("nod_cancelled.wav",0);

		Create_2D_WAV_Sound_Team("gdi_new_tech_acquired.wav",1);
		Send_Message_Team(1,255,204,0,"New technology acquired");
		for (SLNode<cPlayer>* PlayerIter = Get_Player_List()->Head(); (PlayerIter != NULL); PlayerIter = PlayerIter->Next())
		{
			cPlayer *p = PlayerIter->Data();
			if (p->IsActive)
			{		
				int id = p->PlayerId;
				if (Get_Team(id) == 0)
				{
					GameObject *player = Get_GameObj(id);
					if (Commands->Is_A_Star(player))
					{
						Commands->Apply_Damage(player,99999.0f,"Death",0);
					}
				}
			}
		}
		Commands->Start_Timer(obj,this,2.0f,211);//end the game
	}
	else if (number == 211)
	{
		Console_Input("win 1");
	}
}
void z_Pro_Mammy_Death_Counter::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&deaths,sizeof(deaths),1);
	Auto_Save_Variable(&dodrops,sizeof(dodrops),2);
	Auto_Save_Variable(&dropcount,sizeof(dropcount),3);
}


void z_Pro_Prototype_Mammoth::Created(GameObject *obj)
{
	spawnpadid = Get_Int_Parameter("SpawnPadID");
	stolen = false;
	send_drop_request = true;
	gdi_player_id = 0;
}
void z_Pro_Prototype_Mammoth::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Set_Animation(Commands->Find_Object(spawnpadid),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
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

	if (stolen)
	{
		GameObject *target = Find_Object_With_Script("z_Pro_Mammy_Death_Counter");
		Commands->Send_Custom_Event(obj,target,1000,1,0);
	}

	if ( (Commands->Get_Player_Type(obj) == 0) && (Get_Vehicle_Driver(obj)) )
	{
		if (Commands->Get_Player_Type(shooter) == 1)
		{
			char killmsg[512];
			sprintf(killmsg,"%s prevented %s from stealing the GDI Prototype Mammoth Tank 2",Get_Player_Name(shooter),Get_Player_Name(Get_Vehicle_Driver(obj)));
			Send_Message(255,255,255,killmsg);
		}
	}
}
void z_Pro_Prototype_Mammoth::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (!stolen)
	{
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
		Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
	}
}
void z_Pro_Prototype_Mammoth::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (Commands->Get_Player_Type(sender) == 0)//good
		{
			stolen = true;
			if (send_drop_request)
			{
				send_drop_request = false;
				GameObject *target = Find_Object_With_Script("z_Pro_Mammy_Death_Counter");
				Commands->Send_Custom_Event(obj,target,3000,1,0);
			}		
			Commands->Set_Animation(Commands->Find_Object(spawnpadid),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false);
#ifdef SSGM
			char stolenmsg[512];
			sprintf(stolenmsg,"%s has stolen a %s",Get_Player_Name(Get_Vehicle_Driver(obj)),Get_Translated_Preset_Name_Ex(obj));
			Send_Message(255,255,255,stolenmsg);
#endif
		}
		else if (Commands->Get_Player_Type(sender) == 1)//bad
		{
			gdi_player_id = Commands->Get_ID(sender);
			Commands->Start_Timer(obj,this,0.5f,250500);//eject and kill player
		}
	}
}
void z_Pro_Prototype_Mammoth::Timer_Expired(GameObject *obj, int number)
{
	if (number == 250500)
	{
		Force_Occupant_Exit(obj,0);//boot him from the tank
		Commands->Start_Timer(obj,this,0.5f,250501);
	}
	else if (number == 250501)
	{
		Commands->Apply_Damage(Commands->Find_Object(gdi_player_id),99999.0f,"Death",obj);
		Commands->Set_Player_Type(obj,1);
		gdi_player_id = 0;
	}
}
void z_Pro_Prototype_Mammoth::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&stolen,sizeof(stolen),1);
	Auto_Save_Variable(&gdi_player_id,sizeof(gdi_player_id),2);
	Auto_Save_Variable(&spawnpadid,sizeof(spawnpadid),3);
	Auto_Save_Variable(&send_drop_request,sizeof(send_drop_request),4);
}

void z_Pro_Vehicle::Killed(GameObject *obj, GameObject *shooter)
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

void z_Pro_Loop_Nod_Drops::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,299.0f,100);//first dropoff
}
void z_Pro_Loop_Nod_Drops::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		const char *artycin = "nod_arty_drop.txt";		const char *buggycin = "nod_buggy_drop.txt";
		Vector3 artypos; artypos.X = -113.934f; artypos.Y = -27.656f; artypos.Z = 3.966f; float artyfac = -37.634f;
		Vector3 buggypos; buggypos.X = -138.786f; buggypos.Y = -32.566f; buggypos.Z = 1.613f; float buggyfac = 0.000f;
		GameObject *droparty = Commands->Create_Object("Invisible_Object",artypos);
		GameObject *dropbuggy = Commands->Create_Object("Invisible_Object",buggypos);
		Commands->Set_Facing(droparty,artyfac);
		Commands->Set_Facing(dropbuggy,buggyfac);
		Commands->Attach_Script(droparty,"Test_Cinematic",artycin);
		Commands->Attach_Script(dropbuggy,"Test_Cinematic",buggycin);
		Send_Message_Team(0,255,0,0,"Reinforcements are enroute: Light Tank");
		Send_Message_Team(0,255,0,0,"Reinforcements are enroute: Buggy");
		Create_2D_WAV_Sound_Team("m00evan_dsgn0064i1evan_snd.wav",0);
	}
	else if (number == 101)
	{
		const char *artycin = "nod_arty_drop.txt";
		Vector3 artypos; artypos.X = -113.934f; artypos.Y = -27.656f; artypos.Z = 3.966f; float artyfac = -37.634f;
		GameObject *droparty = Commands->Create_Object("Invisible_Object",artypos);
		Commands->Set_Facing(droparty,artyfac);
		Commands->Attach_Script(droparty,"Test_Cinematic",artycin);
		Send_Message_Team(0,255,0,0,"Reinforcements are enroute: Light Tank");
		Create_2D_WAV_Sound_Team("m00evan_dsgn0064i1evan_snd.wav",0);
	}
	else if (number == 102)
	{
		const char *buggycin = "nod_buggy_drop.txt";
		Vector3 buggypos; buggypos.X = -138.786f; buggypos.Y = -32.566f; buggypos.Z = 1.613f; float buggyfac = 0.000f;
		GameObject *dropbuggy = Commands->Create_Object("Invisible_Object",buggypos);
		Commands->Set_Facing(dropbuggy,buggyfac);
		Commands->Attach_Script(dropbuggy,"Test_Cinematic",buggycin);
		Send_Message_Team(0,255,0,0,"Reinforcements are enroute: Buggy");
		Create_2D_WAV_Sound_Team("m00evan_dsgn0064i1evan_snd.wav",0);
	}
}
void z_Pro_Loop_Nod_Drops::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,60.0f,101);//drop arty
			Send_Message_Team(0,255,0,0,"Reinforcements are enroute: 60 seconds for Light Tank");
			Create_2D_WAV_Sound_Team("m00evan_dsgn0064i1evan_snd.wav",0);
		}
		else if (param == 2)
		{
			Commands->Start_Timer(obj,this,60.0f,102);//drop buggy
			Send_Message_Team(0,255,0,0,"Reinforcements are enroute: 60 seconds for Buggy");
			Create_2D_WAV_Sound_Team("m00evan_dsgn0064i1evan_snd.wav",0);
		}
	}
}

void z_Pro_GDI_Harvester::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0f,100);
	Set_Vehicle_Is_Visible(obj,false);//bots ignore it
}
void z_Pro_GDI_Harvester::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Start_Timer(obj,this,1.0f,100);
		Commands->Give_Money(obj,7.0f,true);
	}
}

void z_Pro_Mammoth_Steal_Zone::Created(GameObject *obj)
{
	trigger_stolen = true;
	stealerid = 0;
}
void z_Pro_Mammoth_Steal_Zone::Entered(GameObject *obj, GameObject *enter)
{
	if (trigger_stolen)
	{ 
		if (strcmp(Commands->Get_Preset_Name(Get_Vehicle(enter)),"CnC3_GDI_Mammoth_Tank") == 0)
		{
			trigger_stolen = false;
			stealerid = Commands->Get_ID(enter);
			Commands->Start_Timer(obj,this,1.0f,100);
			Set_Skin(enter,"Blamo");
			Commands->Set_Shield_Type(enter,"Blamo");
			Commands->Set_Health(enter,Commands->Get_Max_Health(enter));
			Commands->Set_Shield_Strength(enter,Commands->Get_Max_Shield_Strength(enter));
		}
	}
}
void z_Pro_Mammoth_Steal_Zone::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		if (Commands->Find_Object(stealerid))
		{
			char stealmsg[512];
			sprintf(stealmsg,"amsg %s has successfully stolen a GDI Prototype Mammoth Tank 2. Nod WINS",Get_Player_Name(Commands->Find_Object(stealerid)));
			Console_Input(stealmsg);
			Commands->Start_Timer(obj,this,5.0f,101);
		}
	}
	else if (number == 101)
	{
		Send_Message_Team(0,255,0,0,"New technology acquired");
		Create_2D_WAV_Sound_Team("nod_new_tech_acquired.wav",0);
		Commands->Start_Timer(obj,this,5.0f,102);
	}
	else if (number == 102)
	{
		for (SLNode<cPlayer>* PlayerIter = Get_Player_List()->Head(); (PlayerIter != NULL); PlayerIter = PlayerIter->Next())
		{
			cPlayer *p = PlayerIter->Data();
			if (p->IsActive)
			{		
				int id = p->PlayerId;
				if (Get_Team(id) == 1)
				{
					GameObject *player = Get_GameObj(id);
					if (Commands->Is_A_Star(player))
					{
						Commands->Apply_Damage(player,99999.0f,"Death",0);
					}
				}
			}
		}
		Commands->Start_Timer(obj,this,5.0f,103);
	}
	else if (number == 103)
	{
		Console_Input("win 0");
	}
}
void z_Pro_Mammoth_Steal_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&trigger_stolen,sizeof(trigger_stolen),1);
	Auto_Save_Variable(&stealerid,sizeof(stealerid),2);
}

ScriptRegistrant<z_Pro_Mammoth_Steal_Zone> z_Pro_Mammoth_Steal_Zone_Registrant("z_Pro_Mammoth_Steal_Zone","");
ScriptRegistrant<z_Pro_GDI_Harvester> z_Pro_GDI_Harvester_Registrant("z_Pro_GDI_Harvester","");
ScriptRegistrant<z_Pro_Loop_Nod_Drops> z_Pro_Loop_Nod_Drops_Registrant("z_Pro_Loop_Nod_Drops","");
ScriptRegistrant<z_Pro_Vehicle> z_Pro_Vehicle_Registrant("z_Pro_Vehicle","");
ScriptRegistrant<z_Pro_Prototype_Mammoth> z_Pro_Prototype_Mammoth_Registrant("z_Pro_Prototype_Mammoth","SpawnPadID=0:int");
ScriptRegistrant<z_Pro_Mammy_Death_Counter> z_Pro_Mammy_Death_Counter_Registrant("z_Pro_Mammy_Death_Counter","");