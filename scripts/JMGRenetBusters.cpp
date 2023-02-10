/*	Renegade Scripts.dll
	Copyright 2015 Tiberian Technologies

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
#include "JMGRenetBuster.h"

// Did a major overhaul on all these scripts, even though the mod they belonged to is dead :(
void JMG_Create_Ship_On_Poke::Created(GameObject *obj)
{
	ShipID = 0;
	YourID = 0;
	armor = "null";
	BotID = 0;
	ViewingBoxID = 0;
	cango = 0;
}

void JMG_Create_Ship_On_Poke::Poked(GameObject *obj,GameObject *poker)
{
	if (ShipID == 0)
	{
		Vector3 pos = Get_Vector3_Parameter("CreateSpot");
		Commands->Create_Explosion(Get_Parameter("SpawnSpotExplosion"),pos,0);
		Commands->Create_Explosion(Get_Parameter("SpawnSpotExplosion"),pos,0);
		Commands->Create_Explosion(Get_Parameter("SpawnSpotExplosion"),pos,0);
		Commands->Create_Explosion(Get_Parameter("SpawnSpotExplosion"),pos,0);
		YourID = Commands->Get_ID(poker);
		Your3DModel = Get_Model(poker);
		armor = Get_Shield_Type(poker);
		Commands->Set_Shield_Type(poker,"blamo");
		Commands->Set_Model(poker,"NULL");
		GameObject *bot = Commands->Create_Object(Commands->Get_Preset_Name(poker),Commands->Get_Position(poker));
		BotID = Commands->Get_ID(bot);
		GameObject *object = Commands->Create_Object(Get_Parameter("ShipPreset"),pos);
		ShipID = Commands->Get_ID(object);
		Commands->Set_Position(poker,Commands->Get_Position(object));
		Commands->Start_Timer(obj,this,0.1f,6546585);
		Commands->Start_Timer(obj,this,0.15f,6546587);
	}
}

/*void JMG_Create_Ship_On_Poke::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 54231684)
	{
	}
}*/

void JMG_Create_Ship_On_Poke::Timer_Expired(GameObject *obj,int number)
{
	if (number == 6546585)
	{
		GameObject *Ship = Commands->Find_Object(ShipID);
		if ((Commands->Get_Health(Ship) == 0) && (ShipID > 0))
		{
			GameObject *object = Commands->Find_Object(YourID);
			Vector3 pos = Commands->Get_Position(object);
			GameObject *object2 = Commands->Create_Object(Get_Parameter("InvisibleObject"),pos);
			ViewingBoxID = Commands->Get_ID(object2);
			cango = 1;
			Commands->Start_Timer(obj,this,0.00f,6546588);	
			Commands->Start_Timer(obj,this,5.0f,6546586);
			Commands->Start_Timer(obj,this,0.5f,6546589);
		}
		Commands->Start_Timer(obj,this,0.1f,6546585);
	}
	if (number == 6546586)
	{
		GameObject *object = Commands->Find_Object(YourID);
		GameObject *object2 = Commands->Find_Object(ViewingBoxID);
		GameObject *object3 = Commands->Find_Object(BotID);
		Vector3 pos = Commands->Get_Position(object3);
		Commands->Destroy_Object(object3);
		Commands->Apply_Damage(object2,9999999.00f,"BlamoKiller",0);
		Commands->Set_Position(object,pos);
		Commands->Set_Shield_Type(object,armor);
		Commands->Set_Model(object,Your3DModel);
		ShipID = 0;
	}
	if (number == 6546587)
	{
		GameObject *object = Commands->Find_Object(YourID);
		Soldier_Transition_Vehicle(object);
	}
	if ((number == 6546588) && (cango == 1))
	{
		GameObject *object = Commands->Find_Object(YourID);
		Soldier_Transition_Vehicle(object);
		Commands->Start_Timer(obj,this,0.10f,6546588);
	}
	if ((number == 6546589))
	{
		cango = 0;
	}
}

void JMG_Create_Ship_On_Poke::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ShipID,5,1);
	Auto_Save_Variable(&YourID,5,2);
	Auto_Save_Variable(&armor,5,3);
	Auto_Save_Variable(&BotID,5,4);
	Auto_Save_Variable(&ViewingBoxID,5,5);
	Auto_Save_Variable(&cango,5,6);
}

void JMG_Advanced_Bounce_Zone::Entered(GameObject *obj,GameObject *enter)
{
	Vector3 pos;
	pos = Commands->Get_Position(enter);
	if ((Get_Float_Parameter("ZAmount") > 0) || (Get_Float_Parameter("ZAmount") < 0))
	{
		pos.Z += Get_Float_Parameter("ZAmount");
	}
	if ((Get_Float_Parameter("YAmount") > 0) || (Get_Float_Parameter("YAmount") < 0))
	{
		pos.Y += Get_Float_Parameter("YAmount");
	}
	if ((Get_Float_Parameter("XAmount") > 0) || (Get_Float_Parameter("XAmount") < 0))
	{
		pos.X += Get_Float_Parameter("XAmount");
	}
	Commands->Set_Position(enter,pos);
}

void JMG_Ship_Random_Hyperspace::Created(GameObject *obj)
{
	Has_Recived_Message = 0;
	OriginalModel = Get_Model(obj);
	Commands->Set_Model(obj,OriginalModel);
}

void JMG_Ship_Random_Hyperspace::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	//Start Hyper Spacing
	if ((message == 48473851) && (Has_Recived_Message == 0))
	{
		Has_Recived_Message = 1;
		Commands->Set_Model(obj,Get_Parameter("HyperspacingModel"));
		Commands->Start_Timer(obj,this,2.0f,586733);
		Commands->Send_Custom_Event(obj,obj,4537822,1,0);
	}
	//go now
	if (message == 48473852)
	{
		Vector3 pos = Commands->Get_Position(obj);
		pos.X = Commands->Get_Random(-100,100);
		pos.Y = Commands->Get_Random(-100,100);
		pos.Z += 0.0f;
		Commands->Set_Position(obj,pos);
		Commands->Set_Model(obj,OriginalModel);
		Vector3 sound = Commands->Get_Position(obj);
		Commands->Create_Sound(Get_Parameter("HyperSpaceSound"),sound,obj);
		Commands->Start_Timer(obj,this,30.0f,586734);
		Commands->Send_Custom_Event(obj,obj,4537821,1,0);
	}
}

void JMG_Ship_Random_Hyperspace::Timer_Expired(GameObject *obj,int number)
{
	if (number == 586733)
	{
		Commands->Send_Custom_Event(obj,obj,48473852,1,0);
	}
	if (number == 586734)
	{
		Has_Recived_Message = 0;
	}
}

void JMG_Ship_Random_Hyperspace::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&OriginalModel,4,1);
	Auto_Save_Variable(&Has_Recived_Message,4,2);
}

void JMG_Random_Music::Created(GameObject *obj)
{
	songnumber = Commands->Get_Random_Int(0,2);
	if (songnumber == 0)
	{
		Commands->Set_Background_Music(Get_Parameter("Song1"));
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Song1Time"),48557224);
	}
	if (songnumber == 1)
	{
		Commands->Set_Background_Music(Get_Parameter("Song2"));
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Song2Time"),48557224);
	}
	if (songnumber == 2)
	{
		Commands->Set_Background_Music(Get_Parameter("Song3"));
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Song3Time"),48557224);
	}
}

void JMG_Random_Music::Timer_Expired(GameObject *obj,int number)
{
	if (number == 48557224)
	{
		if (songnumber == 0)
		{
			Commands->Set_Background_Music(Get_Parameter("Song1"));
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Song1Time"),48557224);
			int repeat = Commands->Get_Random_Int(0,1);
			if (repeat == 1)
			{
				songnumber = Commands->Get_Random_Int(0,2);
			}
			if (repeat == 0)
			{
				songnumber = Commands->Get_Random_Int(1,2);
			}
		}
		if (songnumber == 1)
		{
			Commands->Set_Background_Music(Get_Parameter("Song2"));
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Song2Time"),48557224);
			int repeat = Commands->Get_Random_Int(0,1);
			if (repeat == 1)
			{
				songnumber = Commands->Get_Random_Int(0,2);
			}
			if (repeat == 0)
			{
				int tempsongnumber = Commands->Get_Random_Int(0,2);
				if (tempsongnumber == 1)
				{
					tempsongnumber = Commands->Get_Random_Int(0,2);
					if (tempsongnumber == 1)
					{
						tempsongnumber = 2;
					}
				}
				if ((tempsongnumber == 0) || (tempsongnumber == 2))
				{
					songnumber = tempsongnumber;
				}
			}
		}
		if (songnumber == 2)
		{
			Commands->Set_Background_Music(Get_Parameter("Song3"));
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Song3Time"),48557224);
			int repeat = Commands->Get_Random_Int(0,1);
			if (repeat == 1)
			{
				songnumber = Commands->Get_Random_Int(0,2);
			}
			if (repeat == 0)
			{
				songnumber = Commands->Get_Random_Int(0,1);
			}
		}
	}
}

void JMG_Random_Music::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&songnumber,3,1);
}

void JMG_Ship_Weapon::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.001f,28557222);
	Commands->Start_Timer(obj,this,0.1f,28557223);
	Commands->Start_Timer(obj,this,10.0,28557224);
	Ammo = 4;
	ammocheck = 20;
	Currentammo = 1;
	cangainammo = 1;
}

void JMG_Ship_Weapon::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 4537824 && cangainammo == 1)
	{
		if (Ammo > 0)
		{
			Ammo -= 1;
			Commands->Start_Timer(obj,this,2.00f,28557221);
		}
	}
	if (message == 4537823 && cangainammo == 1)
	{
		if (Ammo < 4 && Get_Current_Bullets(obj) < 4)
		{
			Ammo += 1;
			Set_Current_Bullets(obj,Get_Current_Bullets(obj) + 1);
			Commands->Start_Timer(obj,this,2.00f,28557221);
		}
	}
	if (message == 4537822 && cangainammo == 1)
	{
		Currentammo = Get_Current_Bullets(obj);
		cangainammo = 0;
		Set_Current_Bullets(obj,0);
		Commands->Enable_Engine(obj,false);
	}
	if (message == 4537821 && cangainammo == 0)
	{
		cangainammo = 1;
		Set_Current_Bullets(obj,Currentammo);
		Commands->Enable_Engine(obj,true);
	}
}

void JMG_Ship_Weapon::Timer_Expired(GameObject *obj,int number)
{
	if (28557221 == number && Get_Current_Bullets(obj) < 4)
	{
		Ammo += 1;
		Set_Current_Bullets(obj,Get_Current_Bullets(obj) + 1);
	}
	if (28557222 == number)
	{
		if (Get_Current_Bullets(obj) < newtotal)
		{
			ammocheck = 0;
			Commands->Send_Custom_Event(obj,obj,4537824,1,0);
		}
		newtotal = Get_Current_Bullets(obj);
		Commands->Start_Timer(obj,this,0.001f,28557222);
	}
	if (28557223 == number)
	{
		if (ammocheck < 20)
		{
			ammocheck++;
		}
		if (ammocheck > 20 && Get_Current_Bullets(obj) < 4)
		{
			Set_Current_Bullets(obj,4);
			Ammo = 4;
		}
		Commands->Start_Timer(obj,this,0.1f,28557223);
	}
	if (28557224 == number)
	{
		Set_Current_Bullets(obj,Ammo);
		Commands->Start_Timer(obj,this,10.0,28557224);
	}
}

void JMG_Ship_Weapon::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&newtotal,4,1);
	Auto_Save_Variable(&Ammo,4,2);
	Auto_Save_Variable(&cangainammo,4,3);
	Auto_Save_Variable(&Currentammo,4,4);
	Auto_Save_Variable(&ammocheck,4,5);
}

void JMG_Return_Custom_When_Shot::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if ((CheckPlayerType(damager,0)) || (CheckPlayerType(damager,1)))
	{
		Commands->Send_Custom_Event(obj,damager,4537823,1,0);
	}
}

void JMG_Cronie_AI::Created(GameObject *obj)
{
	target = 0;
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Hibernation(obj,false);
	Commands->Start_Timer(obj,this,0.1f,4586442);
	Commands->Start_Timer(obj,this,5.0f,4586443);
}

void JMG_Cronie_AI::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (target == 0)
	{
		ActionParamsStruct params;
		params.MoveArrivedDistance = 0;
		params.Set_Basic(this,100,131524);
		params.Set_Movement(seen,2.0f,0.0f);
		params.MoveFollow = true;
		Commands->Action_Goto(obj,params);
		target = Commands->Get_ID(seen);
	}
}

/*void JMG_Cronie_AI::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	if (action == 131524)
	{
		currently_hunting = 0;
	}
}*/

void JMG_Cronie_AI::Timer_Expired(GameObject *obj,int number)
{
	if (number == 4586442)
	{
		if (target > 0)
		{
			GameObject *object = Commands->Find_Object(target);
			if (Commands->Get_Health(object) == 0)
			{
				target = 0;
			}
		}
		Commands->Start_Timer(obj,this,0.1f,4586442);
	}
	if (number == 4586443)
	{
		if (target == 0)
		{
			Vector3 pos = Commands->Get_Position(obj);
			pos.X += Commands->Get_Random_Int(-5,5);
			pos.Y += Commands->Get_Random_Int(-5,5);
			ActionParamsStruct params;
			params.Set_Basic(this,100,911);
			params.Set_Movement(pos,1.0f,0.0f);
			params.MoveFollow = false;
			Commands->Action_Goto(obj,params);
		}
		Commands->Start_Timer(obj,this,2.5f,4586443);
	}
}

void JMG_Cronie_AI::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&target,5,1);
}

void JMG_Kill_Self_And_Sender_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Message"))
	{
		Commands->Apply_Damage(obj,9999999.00f,"BlamoKiller",0);
		Commands->Apply_Damage(sender,9999999.00f,"BlamoKiller",0);
	}
}

void JMG_Ship_Shield_Control_Script::Created(GameObject *obj)
{
	ison = 0;
	ObjID = 0;
}

void JMG_Ship_Shield_Control_Script::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if ((message == 1239464) && (ison == 0))
	{
		ison = 1;
		Vector3 pos;
		GameObject *object = Commands->Create_Object(Get_Parameter("Ship_Shield"),pos);
		Commands->Attach_To_Object_Bone(object,obj,"origin");
		Commands->Set_Player_Type(object,Get_Player_Type(obj));
		ObjID = Commands->Get_ID(object);
		char params[512];
		sprintf(params,"%d",Commands->Get_ID(obj));
		Commands->Attach_Script(object,"JMG_Ship_Shield_Script",params);
		Commands->Send_Custom_Event(obj,obj,4537822,1,0);
		//Commands->Send_Custom_Event(obj,obj,45378221,1,0);
		char Text[512];
		sprintf(Text,"MESSAGE on");
		Console_Input(Text);
		//Commands->Start_Timer(obj,this,0.1f,543424420);
	}
	if ((message == 1239464) && (ison == 1))
	{
		ison = 0;
		GameObject *object = Commands->Find_Object(ObjID);
		Commands->Apply_Damage(object,9999999.00f,"BlamoKiller",0);
		ObjID = 0;
		Commands->Send_Custom_Event(obj,obj,4537821,1,0);
		Commands->Send_Custom_Event(obj,obj,45378211,1,0);
		char Text[512];
		sprintf(Text,"MESSAGE off");
		Console_Input(Text);
		//Commands->Start_Timer(obj,this,0.1f,543424421);
	}
}

void JMG_Ship_Shield_Control_Script::Killed(GameObject *obj, GameObject *damager)
{
	if (ison == 1)
	{
		ison = 0;
		GameObject *object = Commands->Find_Object(ObjID);
		Commands->Apply_Damage(object,9999999.00f,"BlamoKiller",0);
		ObjID = 0;
		char Text[512];
		sprintf(Text,"MESSAGE DEAD, %s","Nod");
		Console_Input(Text);
	}
}

/*void JMG_Ship_Shield_Control_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 543424421)
	{
		ison = 0;
	}
	if (number == 543424420)
	{
		ison = 1;
	}
}*/

void JMG_Ship_Shield_Control_Script::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ison,4,1);
	Auto_Save_Variable(&ObjID,4,2);
}

void JMG_Kill_Self_And_Sender_On_Custom_Ship::Created(GameObject *obj)
{
	canbekilled = 1;
}

void JMG_Kill_Self_And_Sender_On_Custom_Ship::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if ((message == Get_Int_Parameter("Message")) && (canbekilled == 1))
	{
		Commands->Apply_Damage(obj,9999999.00f,"BlamoKiller",0);
		Commands->Apply_Damage(sender,9999999.00f,"BlamoKiller",0);
	}
	if ((message == 45378221) && (canbekilled == 1))
	{
		canbekilled = 0;
	}
	if ((message == 45378211) && (canbekilled == 0))
	{
		canbekilled = 1;
	}
}

void JMG_Kill_Self_And_Sender_On_Custom_Ship::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&canbekilled,3,1);
}

void JMG_Ship_Shield_Script::Created(GameObject *obj)
{
	LastSenderID = 0;
	ReccentlyDamaged = 0;
	GameObject *object = Commands->Find_Object(Get_Int_Parameter("ID"));
	if (!_stricmp(Get_Model(object),"cometbsb")){ShieldModel = "shipshieldb";}
	else if (!_stricmp(Get_Model(object),"cometbsg")){ShieldModel = "shipshieldg";}
	else if (!_stricmp(Get_Model(object),"cometbsr")){ShieldModel = "shipshieldr";}
	else if (!_stricmp(Get_Model(object),"cometbsy")){ShieldModel = "shipshieldy";}
	Commands->Set_Model(obj,ShieldModel);
	Commands->Start_Timer(obj,this,0.01f,54342442);
	char Text[512];
	sprintf(Text,"MESSAGE Shield Should Exist");
	Console_Input(Text);
}

void JMG_Ship_Shield_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 54342442)
	{
		Send_Custom_All_Objects_Area(913,Commands->Get_Position(obj),6.53f,obj,2);
		Commands->Start_Timer(obj,this,0.01f,54342442);
	}
	if (number == 54342443)
	{
		Commands->Set_Model(obj,ShieldModel);
		if (ReccentlyDamaged == 1)
		{
			ReccentlyDamaged = 0;
		}
	}
}

void JMG_Ship_Shield_Script::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 90111315 && LastSenderID != Commands->Get_ID(sender))
	{
		LastSenderID = Commands->Get_ID(sender);
		GameObject *object = Commands->Find_Object(Get_Int_Parameter("ID"));
		Commands->Send_Custom_Event(obj,object,90111316,param,0);
		Commands->Set_Model(obj,"shipshieldw");
		Commands->Apply_Damage(sender,9999999.00f,"BlamoKiller",object);
		Commands->Start_Timer(obj,this,0.25f,54342443);
	}
}

void JMG_Ship_Shield_Script::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (ReccentlyDamaged == 0)
	{
		ReccentlyDamaged = 1;
		Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("ID")),90111316,15,0);
		Commands->Set_Model(obj,"shipshieldw");
		Commands->Start_Timer(obj,this,0.25f,54342443);
	}
}

void JMG_Ship_Shield_Script::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&ReccentlyDamaged,4,1);
	Auto_Save_Variable(&LastSenderID,4,2);
	Auto_Save_Variable(&ShieldModel,4,3);
}

void JMG_Renetbusters_Object::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 913)
	{
		float distance = Commands->Get_Distance(Commands->Get_Position(obj),Commands->Get_Position(sender));
		if (distance <= 6.53f - Get_Float_Parameter("size"))
		{
			Commands->Send_Custom_Event(obj,sender,90111315,Get_Int_Parameter("Power_Drain"),0);
		}
	}
}

void JMG_Ship_Control_Script::Created(GameObject *obj)
{
	Commands->Attach_Script(obj,"JMG_New_Ship_Weapon_Control_System","");
	Commands->Attach_Script(obj,"JMG_Kill_Self_And_Sender_On_Custom","911");
	Commands->Start_Timer(obj,this,1.0f,581);
}

void JMG_Ship_Control_Script::Timer_Expired(GameObject *obj,int number)
{
	if (number == 581)
	{
		Commands->Set_Model(obj,"PlyrShipBlue");
	}
}

void JMG_New_Ship_Weapon_Control_System::Damaged(GameObject *obj,GameObject *damager,float damage)
{		
	if (Commands->Get_ID(Get_Vehicle(damager)) == Commands->Get_ID(obj) && damage == 0.0f && Get_Bullets(obj,"Weapon_Ship_Plasma_Shot_Normal") < 4)
	{
		Set_Clip_Bullets(obj,"Weapon_Ship_Plasma_Shot_Normal",Get_Bullets(obj,"Weapon_Ship_Plasma_Shot_Normal")+1);
	}
}

void JMG_Comet_Control_Script::Created(GameObject *obj)
{
	char params[1000];
	sprintf(params,"%f,911,0.10,453,2",Get_Float_Parameter("Comet_Size"));
	Commands->Attach_Script(obj,"JFW_Send_Custom_Distance_Objects_Timer",params);
}

void JMG_Comet_Control_Script::Killed(GameObject *obj, GameObject *damager)
{
	if (Get_Int_Parameter("Spawn_Comets") == 1)
	{
		Commands->Set_Facing(obj,Commands->Get_Random(0.00,360.00));
		GameObject *object = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
		Commands->Set_Model(object,Get_Parameter("Comet_Holder"));
		Commands->Attach_Script(object,"JMG_Attach_And_Remove",Get_Parameter("Smaller_Comet"));
	}
}

void JMG_Attach_And_Remove::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,581);
}

void JMG_Attach_And_Remove::Timer_Expired(GameObject *obj,int number)
{
	if (number == 581)
	{
		Commands->Create_Object(Get_Parameter("Smaller_Comet"),Commands->Get_Bone_Position(obj,"SC1"));
		Commands->Create_Object(Get_Parameter("Smaller_Comet"),Commands->Get_Bone_Position(obj,"SC2"));
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<JMG_Create_Ship_On_Poke> JMG_Create_Ship_On_Poke("JMG_Create_Ship_On_Poke","CreateSpot:vector3,ShipPreset=Comet_Busters_Ship_Blue:string,SpawnSpotExplosion=Explosion_Clear_Spawn_Point:string,InvisibleObject=Invisible_Spectator_Box:string");
ScriptRegistrant<JMG_Advanced_Bounce_Zone> JMG_Advanced_Bounce_Zone("JMG_Advanced_Bounce_Zone","XAmount=0:float,YAmount=0:float,ZAmount=0:float");
ScriptRegistrant<JMG_Ship_Random_Hyperspace> JMG_Ship_Random_Hyperspace("JMG_Ship_Random_Hyperspace","HyperspacingModel=cometbshs:string,HyperSpaceSound=Ship_Teleport:string");
ScriptRegistrant<JMG_Random_Music> JMG_Random_Music("JMG_Random_Music","Song1:string,Song1Time:float,Song2:string,Song2Time:float,Song3:string,Song3Time:float");
ScriptRegistrant<JMG_Ship_Weapon> JMG_Ship_Weapon("JMG_Ship_Weapon","");
ScriptRegistrant<JMG_Return_Custom_When_Shot> JMG_Return_Custom_When_Shot("JMG_Return_Custom_When_Shot","");
ScriptRegistrant<JMG_Cronie_AI> JMG_Cronie_AI("JMG_Cronie_AI","");
ScriptRegistrant<JMG_Kill_Self_And_Sender_On_Custom> JMG_Kill_Self_And_Sender_On_Custom("JMG_Kill_Self_And_Sender_On_Custom","Message:int");
ScriptRegistrant<JMG_Ship_Shield_Control_Script> JMG_Ship_Shield_Control_Script("JMG_Ship_Shield_Control_Script","");
ScriptRegistrant<JMG_Kill_Self_And_Sender_On_Custom_Ship> JMG_Kill_Self_And_Sender_On_Custom_Ship("JMG_Kill_Self_And_Sender_On_Custom_Ship","Message:int");
ScriptRegistrant<JMG_Ship_Shield_Script> JMG_Ship_Shield_Script("JMG_Ship_Shield_Script","ID:int");
ScriptRegistrant<JMG_Renetbusters_Object> JMG_Renetbusters_Object("JMG_Renetbusters_Object","size:float,Power_Drain:int");
ScriptRegistrant<JMG_Ship_Control_Script> JMG_Ship_Control_Script("JMG_Ship_Control_Script","");
ScriptRegistrant<JMG_New_Ship_Weapon_Control_System> JMG_New_Ship_Weapon_Control_System("JMG_New_Ship_Weapon_Control_System","");
ScriptRegistrant<JMG_Comet_Control_Script> JMG_Comet_Control_Script("JMG_Comet_Control_Script","Comet_Size:float,Spawn_Comets=0:int,Comet_Holder=null:string,Smaller_Comet=null:string");
ScriptRegistrant<JMG_Attach_And_Remove> JMG_Attach_And_Remove("JMG_Attach_And_Remove","Smaller_Comet:string");
