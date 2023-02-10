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
#include "z_joipa_return.h"
#include "BaseControllerClass.h"
#include "GameObjManager.h"
//255,204,0 = gdi yellow

void z_jret_briefing::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,20.0f,101);
	Commands->Start_Timer(obj,this,25.0f,102);
	Commands->Start_Timer(obj,this,30.0f,103);
	Commands->Start_Timer(obj,this,35.0f,104);
	Commands->Start_Timer(obj,this,40.0f,105);
}
void z_jret_briefing::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Send_Message(255,204,0,"Bad morning Commanders.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
	}
	else if (number == 102)
	{
		Send_Message(255,204,0,"The Brotherhood of Nod has overrun our base facilities last week.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
	}
	else if (number == 103)
	{
		Send_Message(255,204,0,"We need this area back under GDI control.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
	}
	else if (number == 104)
	{
		Send_Message(255,204,0,"Vehicles can be dropped in via chopper on your location.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
	}
	else if (number == 105)
	{
		Send_Message(255,204,0,"Take back our territory! Good luck Commanders.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
	}
}

void z_jret_Disable_Obi::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Commands->Set_Building_Power(Commands->Find_Object(Get_Int_Parameter("ObiID")),false);
			Commands->Start_Timer(obj,this,5.0f,101);
		}
	}
}
void z_jret_Disable_Obi::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Send_Message(255,204,0,"Good work Commanders, the Nod Obelisk is down.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
	}
}

void z_jret_VehDrops::Created(GameObject *obj)
{
	allowpoke = false;
	pokerguy = 0;
	Commands->Enable_HUD_Pokable_Indicator(obj, false);
	Commands->Start_Timer(obj,this,45.0f,102);//initial dropoff
	Commands->Start_Timer(obj,this,75.0f,100);//allowpoke true, delayed allow
}
void z_jret_VehDrops::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		pokerguy = poker;
		allowpoke = false;
		Commands->Start_Timer(obj,this,1.0f,101);//check objects and dropoff if allowed
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
	}
}
void z_jret_VehDrops::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
	else if (number == 101)
	{
		if (Get_Object_Count(2,"GDI_APC_Player") < 1)
		{
			const char *scriptname = Get_Parameter("Cinematic");
			Vector3 position = Get_Vector3_Parameter("Position");
			float facing = Get_Float_Parameter("Facing");
			GameObject *object = Commands->Create_Object("Invisible_Object",position);
			Commands->Set_Facing(object,facing);
			Commands->Attach_Script(object,"JFW_Cinematic",scriptname);
			char msg[250];
			sprintf(msg,"Access Granted: %s ordered an APC.",Get_Player_Name(pokerguy));
			Send_Message(255,204,0,msg);
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		if (Get_Object_Count(2,"GDI_MedTank_Player") < 1)
		{
			const char *scriptname2 = Get_Parameter("Cinematic2");
			Vector3 position2 = Get_Vector3_Parameter("Position2");
			float facing2 = Get_Float_Parameter("Facing2");
			GameObject *object2 = Commands->Create_Object("Invisible_Object",position2);
			Commands->Set_Facing(object2,facing2);
			Commands->Attach_Script(object2,"JFW_Cinematic",scriptname2);
			char msg[250];
			sprintf(msg,"Access Granted: %s ordered a Medium Tank.",Get_Player_Name(pokerguy));
			Send_Message(255,204,0,msg);
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		if (Get_Object_Count(2,"GDI_Humm-vee_Player") < 1)
		{
			const char *scriptname3 = Get_Parameter("Cinematic3");
			Vector3 position3 = Get_Vector3_Parameter("Position3");
			float facing3 = Get_Float_Parameter("Facing3");
			GameObject *object3 = Commands->Create_Object("Invisible_Object",position3);
			Commands->Set_Facing(object3,facing3);
			Commands->Attach_Script(object3,"JFW_Cinematic",scriptname3);
			char msg[250];
			sprintf(msg,"Access Granted: %s ordered a Humm-vee.",Get_Player_Name(pokerguy));
			Send_Message(255,204,0,msg);
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		Commands->Start_Timer(obj,this,10.0f,100);//allowpoke true
	}
	else if (number == 102)
	{
			const char *scriptname = Get_Parameter("Cinematic");
			Vector3 position = Get_Vector3_Parameter("Position");
			float facing = Get_Float_Parameter("Facing");
			GameObject *object = Commands->Create_Object("Invisible_Object",position);
			Commands->Set_Facing(object,facing);
			Commands->Attach_Script(object,"JFW_Cinematic",scriptname);

			const char *scriptname2 = Get_Parameter("Cinematic2");
			Vector3 position2 = Get_Vector3_Parameter("Position2");
			float facing2 = Get_Float_Parameter("Facing2");
			GameObject *object2 = Commands->Create_Object("Invisible_Object",position2);
			Commands->Set_Facing(object2,facing2);
			Commands->Attach_Script(object2,"JFW_Cinematic",scriptname2);

			const char *scriptname3 = Get_Parameter("Cinematic3");
			Vector3 position3 = Get_Vector3_Parameter("Position3");
			float facing3 = Get_Float_Parameter("Facing3");
			GameObject *object3 = Commands->Create_Object("Invisible_Object",position3);
			Commands->Set_Facing(object3,facing3);
			Commands->Attach_Script(object3,"JFW_Cinematic",scriptname3);

			Send_Message(255,204,0,"General Locke ordered a Medium Tank, Humm-vee and an APC at your location.");
			Commands->Create_2D_WAV_Sound("incmsg.wav");
	}
}
void z_jret_VehDrops::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowpoke,5,1);
	Auto_Save_Variable(&pokerguy,5,1);
}

void z_jret_WepDrop_PP1::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			GameObject *dropobj = Commands->Find_Object(100178);
			Vector3 pos = Commands->Get_Position(dropobj);
			GameObject *object = Commands->Create_Object("Invisible_Object",pos);
			Commands->Set_Facing(object,90.000f);
			Commands->Attach_Script(object,"JFW_Cinematic","z_gdi_pp1drop.txt");
			Send_Message(255,204,0,"Good work. I'm airdropping additional supplies. Take out that Power Plant!");
		}
	}
}

void z_jret_Drops_Obi1::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			GameObject *dropobj1 = Commands->Find_Object(100190);
			Vector3 pos1 = Commands->Get_Position(dropobj1);
			GameObject *object1 = Commands->Create_Object("Invisible_Object",pos1);
			Commands->Set_Facing(object1,70.000f);
			Commands->Attach_Script(object1,"JFW_Cinematic","z_gdi_obi1drop1.txt");

			GameObject *dropobj2 = Commands->Find_Object(100191);
			Vector3 pos2 = Commands->Get_Position(dropobj2);
			GameObject *object2 = Commands->Create_Object("Invisible_Object",pos2);
			Commands->Set_Facing(object2,180.000f);
			Commands->Attach_Script(object2,"JFW_Cinematic","z_gdi_obi1drop2.txt");

			GameObject *dropobj3 = Commands->Find_Object(100192);
			Vector3 pos3 = Commands->Get_Position(dropobj3);
			GameObject *object3 = Commands->Create_Object("Invisible_Object",pos3);
			Commands->Set_Facing(object3,220.000f);
			Commands->Attach_Script(object3,"JFW_Cinematic","z_gdi_obi1drop3.txt");

			GameObject *dropobj4 = Commands->Find_Object(100193);
			Vector3 pos4 = Commands->Get_Position(dropobj4);
			GameObject *object4 = Commands->Create_Object("Invisible_Object",pos4);
			Commands->Set_Facing(object4,10.000f);
			Commands->Attach_Script(object4,"JFW_Cinematic","z_gdi_obi1dropmp1.txt");

			GameObject *dropobj5 = Commands->Find_Object(100194);
			Vector3 pos5 = Commands->Get_Position(dropobj5);
			GameObject *object5 = Commands->Create_Object("Invisible_Object",pos5);
			Commands->Set_Facing(object5,100.000f);
			Commands->Attach_Script(object5,"JFW_Cinematic","z_gdi_obi1dropmp2.txt");
		}
	}
}

void z_jret_WepDrop_PP2::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			GameObject *dropobj = Commands->Find_Object(100178);
			Vector3 pos = Commands->Get_Position(dropobj);
			GameObject *object = Commands->Create_Object("Invisible_Object",pos);
			Commands->Set_Facing(object,90.000f);
			Commands->Attach_Script(object,"JFW_Cinematic","z_gdi_pp2drop.txt");
			Send_Message(255,204,0,"Good work. I'm airdropping additional supplies. Take out that Power Plant and Refinery!");
		}
	}
}

void z_jret_VehDrops_Obi1::Created(GameObject *obj)
{
	allowpoke = false;
	pokerguy = 0;
	Commands->Enable_HUD_Pokable_Indicator(obj, false);
	Commands->Start_Timer(obj,this,45.0f,102);//initial dropoff
	Commands->Start_Timer(obj,this,75.0f,100);//allowpoke true, delayed allow
}
void z_jret_VehDrops_Obi1::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		pokerguy = poker;
		allowpoke = false;
		Commands->Start_Timer(obj,this,1.0f,101);//check objects and dropoff if allowed
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
	}
}
void z_jret_VehDrops_Obi1::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
	else if (number == 101)
	{
		if (Get_Object_Count(2,"GDI_Mammoth_Tank_Player") < 1)
		{
			Vector3 position = Commands->Get_Position(Commands->Find_Object(100195));				float facing = 0.0f;
			GameObject *object = Commands->Create_Object("Invisible_Object",position);
			Commands->Set_Facing(object,facing);
			Commands->Attach_Script(object,"JFW_Cinematic","z_gdi_drop_mammy.txt");
			char msg[250];
			sprintf(msg,"Access Granted: %s ordered a Mammoth Tank.",Get_Player_Name(pokerguy));
			Send_Message(255,204,0,msg);
			Commands->Create_2D_WAV_Sound("accessgranted.wav");

		}
		if (Get_Object_Count(2,"GDI_MedTank_Player") < 2)
		{
			Vector3 position2 = Commands->Get_Position(Commands->Find_Object(100197));			float facing2 = 0.0f;
			GameObject *object2 = Commands->Create_Object("Invisible_Object",position2);
			Commands->Set_Facing(object2,facing2);
			Commands->Attach_Script(object2,"JFW_Cinematic","z_gdi_drop_medtank.txt");
			char msg[250];
			sprintf(msg,"Access Granted: %s ordered a Medium Tank.",Get_Player_Name(pokerguy));
			Send_Message(255,204,0,msg);
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		if (Get_Object_Count(2,"GDI_APC_Player") < 2)
		{
			Vector3 position3 = Commands->Get_Position(Commands->Find_Object(100196));			float facing3 = 0.0f;
			GameObject *object3 = Commands->Create_Object("Invisible_Object",position3);
			Commands->Set_Facing(object3,facing3);
			Commands->Attach_Script(object3,"JFW_Cinematic","z_gdi_drop_apc.txt");
			char msg[250];
			sprintf(msg,"Access Granted: %s ordered an APC.",Get_Player_Name(pokerguy));
			Send_Message(255,204,0,msg);
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		Commands->Start_Timer(obj,this,10.0f,100);//allowpoke true
	}
	else if (number == 102)
	{
			Vector3 position = Commands->Get_Position(Commands->Find_Object(100195));				float facing = 0.0f;
			GameObject *object = Commands->Create_Object("Invisible_Object",position);
			Commands->Set_Facing(object,facing);
			Commands->Attach_Script(object,"JFW_Cinematic","z_gdi_drop_mammy.txt");

			Vector3 position2 = Commands->Get_Position(Commands->Find_Object(100197));			float facing2 = 0.0f;
			GameObject *object2 = Commands->Create_Object("Invisible_Object",position2);
			Commands->Set_Facing(object2,facing2);
			Commands->Attach_Script(object2,"JFW_Cinematic","z_gdi_drop_medtank.txt");

			Vector3 position3 = Commands->Get_Position(Commands->Find_Object(100196));			float facing3 = 0.0f;
			GameObject *object3 = Commands->Create_Object("Invisible_Object",position3);
			Commands->Set_Facing(object3,facing3);
			Commands->Attach_Script(object3,"JFW_Cinematic","z_gdi_drop_apc.txt");

			Send_Message(255,204,0,"General Locke ordered a Mammoth Tank, Medium Tank and an APC at your location.");
			Commands->Create_2D_WAV_Sound("incmsg.wav");
	}
}
void z_jret_VehDrops_Obi1::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowpoke,5,1);
	Auto_Save_Variable(&pokerguy,5,1);
}

void z_jret_Obi1::Created(GameObject *obj)
{
	playdamage = true;
	alive = true;
}
void z_jret_Obi1::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (alive)
	{
		if (playdamage)
		{
			if (damage > 1.0f)
			{
				playdamage = false;
				Commands->Start_Timer(obj,this,1.0f,101);
				Commands->Start_Timer(obj,this,4.5f,102);
			}
		}
	}
}
void z_jret_Obi1::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Send_Message(255,204,0,"The Nod Obelisk is being repaired internally.");
		Commands->Create_2D_WAV_Sound("internalrepair_obi.wav");
	}
	else if (number == 102)
	{
		Send_Message(255,204,0,"External destruction is not possible.");
		Commands->Create_2D_WAV_Sound("extdestructionnotpossible.wav");
		Commands->Start_Timer(obj,this,20.0f,103);
	}
	else if (number == 103)
	{
		if (alive)
		{
			playdamage = true;
		}
	}
}
void z_jret_Obi1::Killed(GameObject *obj, GameObject *shooter)
{
	Attach_Script_Once(Commands->Find_Object(100188),"z_jret_VehDrops_Obi1","");
	Attach_Script_Once(Commands->Find_Object(100189),"z_PT_Terminal","1");
	Commands->Enable_Spawner(100087,0);
	Commands->Enable_Spawner(100099,0);
	Commands->Enable_Spawner(100095,0);
	Commands->Enable_Spawner(100094,0);
	Commands->Enable_Spawner(100098,0);
	Commands->Enable_Spawner(100086,0);
	Commands->Enable_Spawner(100258,0);
	Commands->Enable_Spawner(100257,0);
	Commands->Enable_Spawner(100259,0);
	Commands->Enable_Spawner(100260,0);
	Commands->Enable_Spawner(100261,0);
	Commands->Enable_Spawner(100262,0);
	Commands->Enable_Spawner(100263,0);
	Commands->Enable_Spawner(100264,0);
	Commands->Enable_Spawner(100265,0);
	Commands->Enable_Spawner(100266,0);
	Commands->Enable_Spawner(100268,0);
	Commands->Enable_Spawner(100269,0);
	Commands->Enable_Spawner(100270,0);
	Commands->Enable_Spawner(100271,0);
	Commands->Enable_Spawner(100272,0);
	Commands->Enable_Spawner(100273,0);
	Commands->Enable_Spawner(100274,0);
	Commands->Enable_Spawner(100275,0);
	Commands->Enable_Spawner(100276,0);
	Commands->Enable_Spawner(100277,0);
	Commands->Enable_Spawner(100278,0);
	Commands->Enable_Spawner(100289,0);
	Commands->Enable_Spawner(100290,0);
	Commands->Enable_Spawner(100291,0);
	Commands->Enable_Spawner(100292,0);
	Commands->Enable_Spawner(100293,0);
	Commands->Enable_Spawner(100299,0);
	Commands->Enable_Spawner(100300,0);
	Commands->Enable_Spawner(100301,0);
	Commands->Enable_Spawner(100302,0);
	Commands->Enable_Spawner(100303,0);
	Commands->Enable_Spawner(100304,0);
	Commands->Enable_Spawner(100305,0);
	Commands->Enable_Spawner(100306,0);
	Commands->Enable_Spawner(100307,0);
	Commands->Enable_Spawner(100079,0);
	Commands->Enable_Spawner(100078,0);
	Commands->Enable_Spawner(100308,0);
	Commands->Enable_Spawner(100309,0);
	Commands->Enable_Spawner(100310,0);
	Commands->Enable_Spawner(100311,0);
	Commands->Enable_Spawner(100312,0);
	Commands->Enable_Spawner(100313,0);
	Commands->Enable_Spawner(100314,0);
	Commands->Enable_Spawner(100315,0);
	Commands->Enable_Spawner(100316,0);
	Commands->Enable_Spawner(100317,0);
	Commands->Enable_Spawner(100318,0);
	Commands->Enable_Spawner(100319,0);
	Commands->Enable_Spawner(100320,0);
	Commands->Enable_Spawner(100321,0);
	Commands->Enable_Spawner(100322,0);
	Commands->Enable_Spawner(100323,0);
	Commands->Enable_Spawner(100324,0);
	Commands->Enable_Spawner(100325,0);
	Commands->Enable_Spawner(100326,0);
	Commands->Enable_Spawner(100327,0);
	Commands->Enable_Spawner(100328,0);
	Commands->Enable_Spawner(100329,0);
	Commands->Enable_Spawner(100330,0);
	Commands->Enable_Spawner(100331,0);
	Commands->Enable_Spawner(100332,0);
	Commands->Enable_Spawner(100333,0);
	Commands->Enable_Spawner(100334,0);
}
void z_jret_Obi1::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&playdamage,5,1);
	Auto_Save_Variable(&alive,5,1);
}

void z_jret_CheckpointControl::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	PokerGuy = 0;
	AllowPoke = true;
	Check1Locked = true;
	Check2Locked = true;
	Check3Locked = true;
	Check4Locked = true;
	Check5Locked = true;
	Check6Locked = true;
}
void z_jret_CheckpointControl::Poked(GameObject *obj, GameObject *poker)
{
	if (AllowPoke)
	{
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		AllowPoke = false;
		PokerGuy = poker;
		if (Check1Locked == false)
		{
			Commands->Start_Timer(obj,this,0.0f,101);//teleport the guy
		}
		else if (Check2Locked == false)
		{
			Commands->Start_Timer(obj,this,0.0f,102);//teleport the guy
		}
		else if (Check3Locked == false)
		{
			Commands->Start_Timer(obj,this,0.0f,103);//teleport the guy
		}
		else if (Check4Locked == false)
		{
			Commands->Start_Timer(obj,this,0.0f,104);//teleport the guy
		}
		else if (Check5Locked == false)
		{
			Commands->Start_Timer(obj,this,0.0f,105);//teleport the guy
		}
		else if (Check6Locked == false)
		{
			Commands->Start_Timer(obj,this,0.0f,106);//teleport the guy
		}
		else
		{
			Send_Message(104,234,40,"Access Denied");
			Commands->Create_2D_WAV_Sound("accessdenied.wav");
		}
		Commands->Start_Timer(obj,this,2.0f,107);//allowpoke true
	}
}
void z_jret_CheckpointControl::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Send_Message_Player(PokerGuy,104,234,40,"Access Granted");
		Create_2D_WAV_Sound_Player(PokerGuy,"accessgranted.wav");
		int random = Commands->Get_Random_Int(1,3);
		int cp1id1 = 100198;
		int cp1id2 = 100199;
		int cp1id3 = 100200;
		GameObject *gotoObjectcp1id1 = Commands->Find_Object(cp1id1);	
		GameObject *gotoObjectcp1id2 = Commands->Find_Object(cp1id2);	
		GameObject *gotoObjectcp1id3 = Commands->Find_Object(cp1id3);
		Vector3 gotoLocationcp1id1 = Commands->Get_Position(gotoObjectcp1id1);	
		Vector3 gotoLocationcp1id2 = Commands->Get_Position(gotoObjectcp1id2);	
		Vector3 gotoLocationcp1id3 = Commands->Get_Position(gotoObjectcp1id3);
		if (random == 1)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp1id1);
		}
		else if (random == 2)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp1id2);
		}
		else if (random == 3)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp1id3);
		}
	}
	else if (number == 102)
	{
		Send_Message_Player(PokerGuy,104,234,40,"Access Granted");
		Create_2D_WAV_Sound_Player(PokerGuy,"accessgranted.wav");
		int random = Commands->Get_Random_Int(1,3);
		int cp2id1 = 100219;
		int cp2id2 = 100220;
		int cp2id3 = 100221;
		GameObject *gotoObjectcp2id1 = Commands->Find_Object(cp2id1);	
		GameObject *gotoObjectcp2id2 = Commands->Find_Object(cp2id2);	
		GameObject *gotoObjectcp2id3 = Commands->Find_Object(cp2id3);
		Vector3 gotoLocationcp2id1 = Commands->Get_Position(gotoObjectcp2id1);	
		Vector3 gotoLocationcp2id2 = Commands->Get_Position(gotoObjectcp2id2);	
		Vector3 gotoLocationcp2id3 = Commands->Get_Position(gotoObjectcp2id3);
		if (random == 1)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp2id1);
		}
		else if (random == 2)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp2id2);
		}
		else if (random == 3)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp2id3);
		}
	}
	else if (number == 103)
	{
		Send_Message_Player(PokerGuy,104,234,40,"Access Granted");
		Create_2D_WAV_Sound_Player(PokerGuy,"accessgranted.wav");
		int random = Commands->Get_Random_Int(1,3);
		int cp3id1 = 1234;
		int cp3id2 = 1234;
		int cp3id3 = 1234;
		GameObject *gotoObjectcp3id1 = Commands->Find_Object(cp3id1);	
		GameObject *gotoObjectcp3id2 = Commands->Find_Object(cp3id2);	
		GameObject *gotoObjectcp3id3 = Commands->Find_Object(cp3id3);
		Vector3 gotoLocationcp3id1 = Commands->Get_Position(gotoObjectcp3id1);	
		Vector3 gotoLocationcp3id2 = Commands->Get_Position(gotoObjectcp3id2);	
		Vector3 gotoLocationcp3id3 = Commands->Get_Position(gotoObjectcp3id3);
		if (random == 1)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp3id1);
		}
		else if (random == 2)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp3id2);
		}
		else if (random == 3)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp3id3);
		}
	}
	else if (number == 104)
	{
		Send_Message_Player(PokerGuy,104,234,40,"Access Granted");
		Create_2D_WAV_Sound_Player(PokerGuy,"accessgranted.wav");
		int random = Commands->Get_Random_Int(1,3);
		int cp4id1 = 1234;
		int cp4id2 = 1234;
		int cp4id3 = 1234;
		GameObject *gotoObjectcp4id1 = Commands->Find_Object(cp4id1);	
		GameObject *gotoObjectcp4id2 = Commands->Find_Object(cp4id2);	
		GameObject *gotoObjectcp4id3 = Commands->Find_Object(cp4id3);
		Vector3 gotoLocationcp4id1 = Commands->Get_Position(gotoObjectcp4id1);	
		Vector3 gotoLocationcp4id2 = Commands->Get_Position(gotoObjectcp4id2);	
		Vector3 gotoLocationcp4id3 = Commands->Get_Position(gotoObjectcp4id3);
		if (random == 1)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp4id1);
		}
		else if (random == 2)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp4id2);
		}
		else if (random == 3)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp4id3);
		}
	}
	else if (number == 105)
	{
		Send_Message_Player(PokerGuy,104,234,40,"Access Granted");
		Create_2D_WAV_Sound_Player(PokerGuy,"accessgranted.wav");
		int random = Commands->Get_Random_Int(1,3);
		int cp5id1 = 1234;
		int cp5id2 = 1234;
		int cp5id3 = 1234;
		GameObject *gotoObjectcp5id1 = Commands->Find_Object(cp5id1);	
		GameObject *gotoObjectcp5id2 = Commands->Find_Object(cp5id2);	
		GameObject *gotoObjectcp5id3 = Commands->Find_Object(cp5id3);
		Vector3 gotoLocationcp5id1 = Commands->Get_Position(gotoObjectcp5id1);	
		Vector3 gotoLocationcp5id2 = Commands->Get_Position(gotoObjectcp5id2);	
		Vector3 gotoLocationcp5id3 = Commands->Get_Position(gotoObjectcp5id3);
		if (random == 1)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp5id1);
		}
		else if (random == 2)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp5id2);
		}
		else if (random == 3)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp5id3);
		}
	}
	else if (number == 106)
	{
		Send_Message_Player(PokerGuy,104,234,40,"Access Granted");
		Create_2D_WAV_Sound_Player(PokerGuy,"accessgranted.wav");
		int random = Commands->Get_Random_Int(1,3);
		int cp6id1 = 1234;
		int cp6id2 = 1234;
		int cp6id3 = 1234;
		GameObject *gotoObjectcp6id1 = Commands->Find_Object(cp6id1);	
		GameObject *gotoObjectcp6id2 = Commands->Find_Object(cp6id2);	
		GameObject *gotoObjectcp6id3 = Commands->Find_Object(cp6id3);
		Vector3 gotoLocationcp6id1 = Commands->Get_Position(gotoObjectcp6id1);	
		Vector3 gotoLocationcp6id2 = Commands->Get_Position(gotoObjectcp6id2);	
		Vector3 gotoLocationcp6id3 = Commands->Get_Position(gotoObjectcp6id3);
		if (random == 1)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp6id1);
		}
		else if (random == 2)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp6id2);
		}
		else if (random == 3)
		{
			Commands->Set_Position(PokerGuy,gotoLocationcp6id3);
		}
	}
	else if (number == 107)
	{
		AllowPoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}
void z_jret_CheckpointControl::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Check1Locked = false;
			Check2Locked = true;
			Check3Locked = true;
			Check4Locked = true;
			Check5Locked = true;
			Check6Locked = true;
			Send_Message(255,204,0,"Access Granted");
			Send_Message(255,204,0,"Checkpoint One is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint One is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint One is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		else if (param == 2)
		{
			Check1Locked = true;
			Check2Locked = false;
			Check3Locked = true;
			Check4Locked = true;
			Check5Locked = true;
			Check6Locked = true;
			Send_Message(255,204,0,"Access Granted");
			Send_Message(255,204,0,"Checkpoint Two is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Two is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Two is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		else if (param == 3)
		{
			Check1Locked = true;
			Check2Locked = true;
			Check3Locked = false;
			Check4Locked = true;
			Check5Locked = true;
			Check6Locked = true;
			Send_Message(255,204,0,"Access Granted");
			Send_Message(255,204,0,"Checkpoint Three is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Three is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Three is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		else if (param == 4)
		{
			Check1Locked = true;
			Check2Locked = true;
			Check3Locked = true;
			Check4Locked = false;
			Check5Locked = true;
			Check6Locked = true;
			Send_Message(255,204,0,"Access Granted");
			Send_Message(255,204,0,"Checkpoint Four is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Four is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Four is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		else if (param == 5)
		{
			Check1Locked = true;
			Check2Locked = true;
			Check3Locked = true;
			Check4Locked = true;
			Check5Locked = false;
			Check6Locked = true;
			Send_Message(255,204,0,"Access Granted");
			Send_Message(255,204,0,"Checkpoint Five is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Five is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Five is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		else if (param == 3)
		{
			Check1Locked = true;
			Check2Locked = true;
			Check3Locked = true;
			Check4Locked = true;
			Check5Locked = true;
			Check6Locked = false;
			Send_Message(255,204,0,"Access Granted");
			Send_Message(255,204,0,"Checkpoint Six is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Six is now UNLOCKED.");
			Send_Message(255,204,0,"Checkpoint Six is now UNLOCKED.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
	}
}
void z_jret_CheckpointControl::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&Check1Locked,5,1);
	Auto_Save_Variable(&Check2Locked,5,1);
	Auto_Save_Variable(&Check3Locked,5,1);
	Auto_Save_Variable(&Check4Locked,5,1);
	Auto_Save_Variable(&Check5Locked,5,1);
	Auto_Save_Variable(&Check6Locked,5,1);
	Auto_Save_Variable(&AllowPoke,5,1);
	Auto_Save_Variable(&PokerGuy,5,1);
}

void z_jret_Obi2::Killed(GameObject *obj, GameObject *shooter)
{
	Attach_Script_Once(Commands->Find_Object(100211),"z_jret_VehDrops_Obi2","");
	Attach_Script_Once(Commands->Find_Object(100212),"z_PT_Terminal","1");
	Commands->Enable_Spawner(100375,0);
	Commands->Enable_Spawner(100376,0);
	Commands->Enable_Spawner(100377,0);
	Commands->Enable_Spawner(100378,0);
	Commands->Enable_Spawner(100379,0);
	Commands->Enable_Spawner(100380,0);
	Commands->Enable_Spawner(100381,0);
	Commands->Enable_Spawner(100382,0);
	Commands->Enable_Spawner(100383,0);
	Commands->Enable_Spawner(100384,0);
	Commands->Enable_Spawner(100385,0);
	Commands->Enable_Spawner(100386,0);
	Commands->Enable_Spawner(100387,0);
	Commands->Enable_Spawner(100388,0);
	Commands->Enable_Spawner(100389,0);
	Commands->Enable_Spawner(100390,0);
	Commands->Enable_Spawner(100396,0);
	Commands->Enable_Spawner(100397,0);
	Commands->Enable_Spawner(100398,0);
	Commands->Enable_Spawner(100399,0);
	Commands->Enable_Spawner(100400,0);
	Commands->Enable_Spawner(100401,0);
	Commands->Enable_Spawner(100402,0);
	Commands->Enable_Spawner(100403,0);
	Commands->Enable_Spawner(100404,0);
	Commands->Enable_Spawner(100405,0);
	Commands->Enable_Spawner(100406,0);
	Commands->Enable_Spawner(100407,0);
	Commands->Enable_Spawner(100408,0);
	Commands->Enable_Spawner(100409,0);
}

void z_jret_VehDrops_Obi2::Created(GameObject *obj)
{
	allowpoke = false;
	pokerguy = 0;
	Commands->Enable_HUD_Pokable_Indicator(obj, false);
	Commands->Start_Timer(obj,this,45.0f,102);//initial dropoff
	Commands->Start_Timer(obj,this,75.0f,100);//allowpoke true, delayed allow
}
void z_jret_VehDrops_Obi2::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		pokerguy = poker;
		allowpoke = false;
		Commands->Start_Timer(obj,this,1.0f,101);//check objects and dropoff if allowed
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
	}
}
void z_jret_VehDrops_Obi2::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
	else if (number == 101)
	{
		if (Get_Object_Count(2,"GDI_Mammoth_Tank_Player") < 2)
		{
			Vector3 position = Commands->Get_Position(Commands->Find_Object(100213));				float facing = 0.0f;
			GameObject *object = Commands->Create_Object("Invisible_Object",position);
			Commands->Set_Facing(object,facing);
			Commands->Attach_Script(object,"JFW_Cinematic","z_gdi_drop_mammy.txt");
			char msg[250];
			sprintf(msg,"Access Granted: %s ordered a Mammoth Tank.",Get_Player_Name(pokerguy));
			Send_Message(255,204,0,msg);
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		if  (Get_Object_Count(2,"GDI_MedTank_Player") < 2)
		{
			Vector3 position2 = Commands->Get_Position(Commands->Find_Object(100214));			float facing2 = 0.0f;
			GameObject *object2 = Commands->Create_Object("Invisible_Object",position2);
			Commands->Set_Facing(object2,facing2);
			Commands->Attach_Script(object2,"JFW_Cinematic","z_gdi_drop_medtank.txt");
			char msg[250];
			sprintf(msg,"Access Granted: %s ordered a Medium Tank.",Get_Player_Name(pokerguy));
			Send_Message(255,204,0,msg);
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		if (Get_Object_Count(2,"GDI_APC_Player") < 2)
		{
			Vector3 position3 = Commands->Get_Position(Commands->Find_Object(100215));			float facing3 = 0.0f;
			GameObject *object3 = Commands->Create_Object("Invisible_Object",position3);
			Commands->Set_Facing(object3,facing3);
			Commands->Attach_Script(object3,"JFW_Cinematic","z_gdi_drop_apc.txt");
			char msg[250];
			sprintf(msg,"Access Granted: %s ordered an APC.",Get_Player_Name(pokerguy));
			Send_Message(255,204,0,msg);
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
		}
		Commands->Start_Timer(obj,this,10.0f,100);//allowpoke true
	}
	else if (number == 102)
	{
			Vector3 position = Commands->Get_Position(Commands->Find_Object(100213));				float facing = 0.0f;
			GameObject *object = Commands->Create_Object("Invisible_Object",position);
			Commands->Set_Facing(object,facing);
			Commands->Attach_Script(object,"JFW_Cinematic","z_gdi_drop_mammy2.txt");

			Vector3 position2 = Commands->Get_Position(Commands->Find_Object(100214));			float facing2 = 0.0f;
			GameObject *object2 = Commands->Create_Object("Invisible_Object",position2);
			Commands->Set_Facing(object2,facing2);
			Commands->Attach_Script(object2,"JFW_Cinematic","z_gdi_drop_medtank.txt");

			Vector3 position3 = Commands->Get_Position(Commands->Find_Object(100215));			float facing3 = 0.0f;
			GameObject *object3 = Commands->Create_Object("Invisible_Object",position3);
			Commands->Set_Facing(object3,facing3);
			Commands->Attach_Script(object3,"JFW_Cinematic","z_gdi_drop_apc.txt");

			Send_Message(255,204,0,"General Locke ordered a Mammoth Tank, Medium Tank and an APC at your location.");
			Commands->Create_2D_WAV_Sound("incmsg.wav");
	}
}
void z_jret_VehDrops_Obi2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowpoke,5,1);
	Auto_Save_Variable(&pokerguy,5,1);
}

void z_jret_DropMoreMP::Created(GameObject *obj)
{
	Send_Message(255,204,0,"Excellent work commanders, i'm sending in more troops and supplies. Do whatever it takes to take out the Hand of Nod Pedestal.");
	Commands->Create_2D_WAV_Sound("incmsg.wav");

	GameObject *dropobj1 = Commands->Find_Object(100216);
	Vector3 pos1 = Commands->Get_Position(dropobj1);
	GameObject *object1 = Commands->Create_Object("Invisible_Object",pos1);
	Commands->Set_Facing(object1,90.000f);
	Commands->Attach_Script(object1,"JFW_Cinematic","z_gdi_obi2dropmp.txt");

	GameObject *dropobj2 = Commands->Find_Object(100217);
	Vector3 pos2 = Commands->Get_Position(dropobj2);
	GameObject *object2 = Commands->Create_Object("Invisible_Object",pos2);
	Commands->Set_Facing(object2,90.000f);
	Commands->Attach_Script(object2,"JFW_Cinematic","z_gdi_obi2dropmp.txt");

	GameObject *dropobj3 = Commands->Find_Object(100218);
	Vector3 pos3 = Commands->Get_Position(dropobj3);
	GameObject *object3 = Commands->Create_Object("Invisible_Object",pos3);
	Commands->Set_Facing(object3,90.000f);
	Commands->Attach_Script(object3,"JFW_Cinematic","z_gdi_obi2dropmp.txt");
}

void z_jret_DropIons_Obi2::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			GameObject *dropobj4 = Commands->Find_Object(100213);
			Vector3 pos4 = Commands->Get_Position(dropobj4);
			GameObject *object4 = Commands->Create_Object("Invisible_Object",pos4);
			Commands->Set_Facing(object4,90.000f);
			Commands->Attach_Script(object4,"JFW_Cinematic","z_gdi_drop_ions.txt");
			Commands->Start_Timer(obj,this,90.0f,101);
		}
	}
}
void z_jret_DropIons_Obi2::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		GameObject *dropobj4 = Commands->Find_Object(100213);
		Vector3 pos4 = Commands->Get_Position(dropobj4);
		GameObject *object4 = Commands->Create_Object("Invisible_Object",pos4);
		Commands->Set_Facing(object4,90.000f);
		Commands->Attach_Script(object4,"JFW_Cinematic","z_gdi_drop_ions.txt");
		Commands->Start_Timer(obj,this,90.0f,101);
	}
}

void z_jret_Airstrip_Vehicles::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,15.0f,101);
}
void z_jret_Airstrip_Vehicles::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Create_Vehicle("Nod_Light_Tank_Bot_Star",0.0f,0,0);
		Commands->Start_Timer(obj,this,150.0f,101);
	}
}
void z_jret_Airstrip_Vehicles::Killed(GameObject *obj, GameObject *shooter)
{
	Destroy_Script();
}

void z_jret_PP1::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Enable_Spawner(100065,0);
	Commands->Enable_Spawner(100066,0);
	Commands->Enable_Spawner(100064,0);
	Commands->Enable_Spawner(100067,0);
	Commands->Enable_Spawner(100225,0);
	Commands->Enable_Spawner(100224,0);
	Commands->Enable_Spawner(100223,0);
	Commands->Enable_Spawner(100226,0);
	Commands->Enable_Spawner(100227,0);
	Commands->Enable_Spawner(100228,0);
	Commands->Enable_Spawner(100230,0);
	Commands->Enable_Spawner(100231,0);
	Commands->Enable_Spawner(100232,0);
	Commands->Enable_Spawner(100233,0);
	Commands->Enable_Spawner(100234,0);
	Commands->Enable_Spawner(100235,0);
	Commands->Enable_Spawner(100236,0);
	Commands->Enable_Spawner(100237,0);
	Commands->Enable_Spawner(100238,0);
	Commands->Enable_Spawner(100239,0);
	Commands->Enable_Spawner(100240,0);
	Commands->Enable_Spawner(100241,0);
	Commands->Enable_Spawner(100242,0);
	Commands->Enable_Spawner(100243,0);
	Commands->Enable_Spawner(100244,0);
	Commands->Enable_Spawner(100245,0);
	Commands->Enable_Spawner(100252,0);
	Commands->Enable_Spawner(100253,0);
	Commands->Enable_Spawner(100254,0);
	Commands->Enable_Spawner(100255,0);
	Commands->Enable_Spawner(100256,0);
}

void z_jret_PP2::Created(GameObject *obj)
{
	playdamage = true;
	alive = true;
}
void z_jret_PP2::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (alive)
	{
		if (playdamage)
		{
			if (damage > 1.0f)
			{
				playdamage = false;
				Commands->Start_Timer(obj,this,1.0f,101);
				Commands->Start_Timer(obj,this,4.5f,102);
			}
		}
	}
}
void z_jret_PP2::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Send_Message(255,204,0,"The Nod Power Plant is being repaired internally.");
		Commands->Create_2D_WAV_Sound("internalrepair_pp.wav");
	}
	else if (number == 102)
	{
		Send_Message(255,204,0,"External destruction is not possible.");
		Commands->Create_2D_WAV_Sound("extdestructionnotpossible.wav");
		Commands->Start_Timer(obj,this,20.0f,103);
	}
	else if (number == 103)
	{
		if (alive)
		{
			playdamage = true;
		}
	}
}
void z_jret_PP2::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Enable_Spawner(100108,0);
	Commands->Enable_Spawner(100110,0);
	Commands->Enable_Spawner(100103,0);
	Commands->Enable_Spawner(100102,0);
	Commands->Enable_Spawner(100412,0);
	Commands->Enable_Spawner(100413,0);
	Commands->Enable_Spawner(100414,0);
	Commands->Enable_Spawner(100415,0);
	Commands->Enable_Spawner(100416,0);
	Commands->Enable_Spawner(100417,0);
	Commands->Enable_Spawner(100418,0);
	Commands->Enable_Spawner(100419,0);
	Commands->Enable_Spawner(100420,0);
	Commands->Enable_Spawner(100421,0);
	Commands->Enable_Spawner(100422,0);
	Commands->Enable_Spawner(100423,0);
	Commands->Enable_Spawner(100424,0);
	Commands->Enable_Spawner(100425,0);
	Commands->Enable_Spawner(100426,0);
	Commands->Enable_Spawner(100427,0);
	Commands->Enable_Spawner(100428,0);
	Commands->Enable_Spawner(100429,0);
	Commands->Enable_Spawner(100430,0);
	Commands->Enable_Spawner(100431,0);
	Commands->Enable_Spawner(100432,0);
	Commands->Enable_Spawner(100433,0);
	Commands->Enable_Spawner(100434,0);
}
void z_jret_PP2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&playdamage,5,1);
	Auto_Save_Variable(&alive,5,1);
}

ScriptRegistrant<z_jret_PP2> z_jret_PP2_Registrant("z_jret_PP2","");
ScriptRegistrant<z_jret_PP1> z_jret_PP1_Registrant("z_jret_PP1","");
ScriptRegistrant<z_jret_Airstrip_Vehicles> z_jret_Airstrip_Vehicles_Registrant("z_jret_Airstrip_Vehicles","");
ScriptRegistrant<z_jret_DropIons_Obi2> z_jret_DropIons_Obi2_Registrant("z_jret_DropIons_Obi2","");
ScriptRegistrant<z_jret_DropMoreMP> z_jret_DropMoreMP_Registrant("z_jret_DropMoreMP","");
ScriptRegistrant<z_jret_Obi2> z_jret_Obi2_Registrant("z_jret_Obi2","");
ScriptRegistrant<z_jret_CheckpointControl> z_jret_CheckpointControl_Registrant("z_jret_CheckpointControl","");
ScriptRegistrant<z_jret_Obi1> z_jret_Obi1_Registrant("z_jret_Obi1","");
ScriptRegistrant<z_jret_VehDrops_Obi2> z_jret_VehDrops_Obi2_Registrant("z_jret_VehDrops_Obi2","");
ScriptRegistrant<z_jret_VehDrops_Obi1> z_jret_VehDrops_Obi1_Registrant("z_jret_VehDrops_Obi1","");
ScriptRegistrant<z_jret_WepDrop_PP2> z_jret_WepDrop_PP2_Registrant("z_jret_WepDrop_PP2","");
ScriptRegistrant<z_jret_Drops_Obi1> z_jret_Drops_Obi1_Registrant("z_jret_Drops_Obi1","");
ScriptRegistrant<z_jret_WepDrop_PP1> z_jret_WepDrop_PP1_Registrant("z_jret_WepDrop_PP1","");
ScriptRegistrant<z_jret_VehDrops> z_jret_VehDrops_Registrant("z_jret_VehDrops","Cinematic=bla:string,Position:vector3,Facing=90000:float,Cinematic2=bla:string,Position2:vector3,Facing2=90000:float,Cinematic3=bla:string,Position3:vector3,Facing3=90000:float");
ScriptRegistrant<z_jret_Disable_Obi> z_jret_Disable_Obi_Registrant("z_jret_Disable_Obi","ObiID");
ScriptRegistrant<z_jret_briefing> z_jret_briefing_Registrant("z_jret_briefing","");