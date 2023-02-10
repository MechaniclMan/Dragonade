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
#include "z_joipa.h"
#include "BaseControllerClass.h"
#include "GameObjManager.h"

void z_Joi_Briefing::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,15.0f,101);
	Commands->Start_Timer(obj,this,20.0f,102);
	Commands->Start_Timer(obj,this,25.0f,103);
	Commands->Start_Timer(obj,this,30.0f,104);
	Commands->Start_Timer(obj,this,35.0f,105);
	Commands->Start_Timer(obj,this,40.0f,106);
	Commands->Start_Timer(obj,this,45.0f,107);
}
void z_Joi_Briefing::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("joi_brief_01.wav");
		Send_Message(255,0,0,"Good morning Brothers.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("joi_brief_02.wav");
		Send_Message(255,0,0,"Kane wants us to take control of this Tiberium rich area.");
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("joi_brief_03.wav");
		Send_Message(255,0,0,"Unfortunately there is a GDI presence in this area.");
	}
	else if (number == 104)
	{
		Commands->Create_2D_WAV_Sound("joi_brief_04.wav");
		Send_Message(255,0,0,"Eliminate all GDI forces and take control of the tiberium fields.");
	}
	else if (number == 105)
	{
		Commands->Create_2D_WAV_Sound("joi_brief_05.wav");
		Send_Message(255,0,0,"Some buildings require keycards to be opened.");
	}
	else if (number == 106)
	{
		Commands->Create_2D_WAV_Sound("joi_brief_06.wav");
		Send_Message(255,0,0,"Eliminate GDI Officers who are likely to hold keys.");
	}
	else if (number == 107)
	{
		Commands->Create_2D_WAV_Sound("joi_brief_07.wav");
		Send_Message(255,0,0,"Move out Brothers. In the name of Kane!");
	}
}


void z_Joi_KeyDropSound::Killed(GameObject *obj, GameObject *shooter)
{
	if (Get_Int_Parameter("Key") == 1)
	{
		Commands->Create_2D_WAV_Sound("joi_acquiregreen.wav");
		Send_Message(255,0,0,"Well done Brothers, get the Green Key Card now and access the GDI Base Facilities!");
	}
	else if (Get_Int_Parameter("Key") == 2)
	{
		Commands->Create_2D_WAV_Sound("joi_acquireyellow.wav");
		Send_Message(255,0,0,"Well done Brothers, get the Yellow Key Card now and access the GDI Base Facilities!");
	}
	else if (Get_Int_Parameter("Key") == 3)
	{
		Commands->Create_2D_WAV_Sound("joi_acquirered.wav");
		Send_Message(255,0,0,"Well done Brothers, get the Red Key Card now and access the GDI Base Facilities!");
	}
}

void z_Joi_GDI_Harvester::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Create_2D_WAV_Sound("joi_gdiharvdead.wav");
	Send_Message(255,0,0,"Brothers, destroy the Refinery or Weapons Factory. GDI will rebuild harvesters as long as they are online.");
}

void z_Joi_DropNukes::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD) && Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
			{
				Commands->Start_Timer(obj,this,5.0f,100);
				Commands->Start_Timer(obj,this,10.0f,101);
			}
		}
	}
}
void z_Joi_DropNukes::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Create_2D_WAV_Sound("joi_dropnukes.wav");
		Send_Message(255,0,0,"Nice work. I'm airdropping you three Nuclear Strike Beacons.");
	}
	else if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("joi_usenukes.wav");
		Send_Message(255,0,0,"Use the nukes to take out the GDI facilities. Remember you need one for the Pedestal.");

		int id1 = Get_Int_Parameter("Loc_ID1");
		Vector3 position1;
		GameObject *object1;
		position1 = Commands->Get_Position(Commands->Find_Object(id1));
		object1 = Commands->Create_Object("Invisible_Object",position1);
		Commands->Set_Facing(object1,25.0f);
		Commands->Attach_Script(object1,"Test_Cinematic","z_joi_drop_nukes.txt");

		int id2 = Get_Int_Parameter("Loc_ID2");
		Vector3 position2;
		GameObject *object2;
		position2 = Commands->Get_Position(Commands->Find_Object(id2));
		object2 = Commands->Create_Object("Invisible_Object",position2);
		Commands->Set_Facing(object2,155.0f);
		Commands->Attach_Script(object2,"Test_Cinematic","z_joi_drop_nukes.txt");

		int id3 = Get_Int_Parameter("Loc_ID3");
		Vector3 position3;
		GameObject *object3;
		position3 = Commands->Get_Position(Commands->Find_Object(id3));
		object3 = Commands->Create_Object("Invisible_Object",position3);
		Commands->Set_Facing(object3,355.0f);
		Commands->Attach_Script(object3,"Test_Cinematic","z_joi_drop_nukes.txt");
		Commands->Start_Timer(obj,this,240.0f,101);
	}
}

void z_Joi_Endgame::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,1.0f,100);
			Commands->Start_Timer(obj,this,3.5f,101);
		}
	}
}
void z_Joi_Endgame::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Create_2D_WAV_Sound("joi_endgame_01.wav");
		Send_Message(255,0,0,"Excellent work Brothers.");
	}
	else if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("joi_endgame_02.wav");
		Send_Message(255,0,0,"The GDI presence has been eliminated successfully.");
	}
}

void z_Joi_Conyard::Created(GameObject *obj)
{
	alive = true;
	playdmg = true;
}
void z_Joi_Conyard::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (alive)
	{
		if (playdmg)
		{
			playdmg = false;
			Commands->Start_Timer(obj,this,15.0f,401);
			Send_Message(255,255,255,"GDI Construction Yard under attack.");
			Commands->Create_2D_WAV_Sound("gdi_cy_underattack.wav");
		}
	}
}
void z_Joi_Conyard::Killed(GameObject *obj, GameObject *shooter)
{
	alive = false;
	playdmg = false;
	Send_Message(255,255,255,"GDI Construction Yard destroyed.");
	Commands->Create_2D_WAV_Sound("gdi_cy_destroyed.wav");
}
void z_Joi_Conyard::Timer_Expired(GameObject *obj, int number)
{
	if (number == 401)
	{
		if (alive)
		{
			playdmg = true;
		}
	}
}
void z_Joi_Conyard::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&alive,5,1);
	Auto_Save_Variable(&playdmg,5,1);
}

void z_Joi_Rebuild_PP::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD) && !Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
			{
				Commands->Start_Timer(obj,this,5.0f,101);//building
				Commands->Start_Timer(obj,this,36.0f,102);//update
				Commands->Start_Timer(obj,this,66.0f,103);//restore
			}
			else
			{
				Commands->Create_2D_WAV_Sound("cancelled.wav");
				Send_Message(255,255,255,"Reconstruction of the GDI Power Plant has been CANCELLED.");
			}
		}
	}
}
void z_Joi_Rebuild_PP::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD) && !Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
		{
			Commands->Create_2D_WAV_Sound("building.wav");
			Send_Message(255,255,255,"Building of the GDI Power Plant has commenced.");
		}
		else
		{
			Commands->Create_2D_WAV_Sound("cancelled.wav");
			Send_Message(255,255,255,"Reconstruction of the GDI Power Plant has been CANCELLED.");
		}
	}
	else if (number == 102)
	{
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD) && !Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
		{
			Commands->Create_2D_WAV_Sound("updatecomplete.wav");
			Send_Message(255,255,255,"Update on the GDI Power Plant has been completed.");
		}
		else
		{
			Commands->Create_2D_WAV_Sound("cancelled.wav");
			Send_Message(255,255,255,"Reconstruction of the GDI Power Plant has been CANCELLED.");
		}
	}
	else if (number == 103)
	{
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD) && !Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
		{
			GameObject *pp = Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT);
			if (pp)
			{
				Commands->Create_2D_WAV_Sound("gdipponline.wav");
				Send_Message(255,255,255,"GDI Base Power is Online.");
				Restore_Building(pp);
				Commands->Set_Health(pp,Commands->Get_Max_Health(pp));
				Power_Base(1,true);
				Commands->Set_Building_Power(pp,true);
			}
		}
		else
		{
			Commands->Create_2D_WAV_Sound("cancelled.wav");
			Send_Message(255,255,255,"Reconstruction of the GDI Power Plant has been CANCELLED.");
		}
	}
}

void z_Joi_Rebuild_WF::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD) && !Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
			{
				Commands->Start_Timer(obj,this,6.0f,101);//building
				Commands->Start_Timer(obj,this,36.0f,102);//update
				Commands->Start_Timer(obj,this,66.0f,103);//restore
			}
			else
			{
				Commands->Create_2D_WAV_Sound("cancelled.wav");
				Send_Message(255,255,255,"Reconstruction of the GDI Weapons Factory has been CANCELLED.");
			}
		}
	}
}
void z_Joi_Rebuild_WF::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD) && !Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
		{
			Commands->Create_2D_WAV_Sound("building.wav");
			Send_Message(255,255,255,"Building of the GDI Weapons Factory has commenced.");
		}
		else
		{
			Commands->Create_2D_WAV_Sound("cancelled.wav");
			Send_Message(255,255,255,"Reconstruction of the GDI Weapons Factory has been CANCELLED.");
		}
	}
	else if (number == 102)
	{
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD) && !Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
		{
			Commands->Create_2D_WAV_Sound("updatecomplete.wav");
			Send_Message(255,255,255,"Update on the GDI Weapons Factory has been completed.");
		}
		else
		{
			Commands->Create_2D_WAV_Sound("cancelled.wav");
			Send_Message(255,255,255,"Reconstruction of the GDI Weapons Factory has been CANCELLED.");
		}
	}
	else if (number == 103)
	{
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD) && !Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
		{
			GameObject *wf = Find_Building_By_Type(1,BuildingConstants::TYPE_VEHICLE_FACTORY);
			if (wf)
			{
				Commands->Create_2D_WAV_Sound("newconstroptions.wav");
				Send_Message(255,255,255,"New construction options. GDI Weapons Factory is online.");
				Restore_Building(wf);
				Commands->Set_Health(wf,Commands->Get_Max_Health(wf));
				Set_Can_Generate_Vehicles(1,true);
				if (Is_Base_Powered(1))
				{
					Commands->Set_Building_Power(wf,true);
				}
				else
				{
					Commands->Set_Building_Power(wf,false);
				}
			}
		}
		else
		{
			Commands->Create_2D_WAV_Sound("cancelled.wav");
			Send_Message(255,255,255,"Reconstruction of the GDI Weapons Factory has been CANCELLED.");
		}
	}
}

void z_Joi_Rebuild_REF::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD) && !Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
			{
				Commands->Start_Timer(obj,this,6.0f,101);//building
				Commands->Start_Timer(obj,this,36.0f,102);//update
				Commands->Start_Timer(obj,this,66.0f,103);//restore
			}
			else
			{
				Commands->Create_2D_WAV_Sound("cancelled.wav");
				Send_Message(255,255,255,"Reconstruction of the GDI Tiberium Refinery has been CANCELLED.");
			}
		}
	}
}
void z_Joi_Rebuild_REF::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD) && !Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
		{
			Commands->Create_2D_WAV_Sound("building.wav");
			Send_Message(255,255,255,"Building of the GDI Tiberium Refinery has commenced.");
		}
		else
		{
			Commands->Create_2D_WAV_Sound("cancelled.wav");
			Send_Message(255,255,255,"Reconstruction of the GDI Tiberium Refinery has been CANCELLED.");
		}
	}
	else if (number == 102)
	{
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD) && !Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
		{
			Commands->Create_2D_WAV_Sound("updatecomplete.wav");
			Send_Message(255,255,255,"Update on the GDI Tiberium Refinery has been completed.");
		}
		else
		{
			Commands->Create_2D_WAV_Sound("cancelled.wav");
			Send_Message(255,255,255,"Reconstruction of the GDI Tiberium Refinery has been CANCELLED.");
		}
	}
	else if (number == 103)
	{
		if (Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD) && !Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_CONYARD)))
		{
			GameObject *refinery = Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY);
			if (refinery)
			{
				Commands->Create_2D_WAV_Sound("gdirefonline.wav");
				Send_Message(255,255,255,"Primary pressing center operating at optimal efficiency. GDI Tiberium Refinery is online.");
				Restore_Building(refinery);
				Commands->Set_Health(refinery,Commands->Get_Max_Health(refinery));
				if (Is_Base_Powered(1))
				{
					Commands->Set_Building_Power(refinery,true);
				}
				else
				{
					Commands->Set_Building_Power(refinery,false);
				}
			}
		}
		else
		{
			Commands->Create_2D_WAV_Sound("cancelled.wav");
			Send_Message(255,255,255,"Reconstruction of the GDI Tiberium Refinery has been CANCELLED.");
		}
	}
}

ScriptRegistrant<z_Joi_Rebuild_REF> z_Joi_Rebuild_REF_Registrant("z_Joi_Rebuild_REF","");
ScriptRegistrant<z_Joi_Rebuild_WF> z_Joi_Rebuild_WF_Registrant("z_Joi_Rebuild_WF","");
ScriptRegistrant<z_Joi_Rebuild_PP> z_Joi_Rebuild_PP_Registrant("z_Joi_Rebuild_PP","");
ScriptRegistrant<z_Joi_Conyard> z_Joi_Conyard_Registrant("z_Joi_Conyard","");
ScriptRegistrant<z_Joi_Endgame> z_Joi_Endgame_Registrant("z_Joi_Endgame","");
ScriptRegistrant<z_Joi_DropNukes> z_Joi_DropNukes_Registrant("z_Joi_DropNukes","Loc_ID1=1:int,Loc_ID2=2:int,Loc_ID3=3:int");
ScriptRegistrant<z_Joi_GDI_Harvester> z_Joi_GDI_Harvester_Registrant("z_Joi_GDI_Harvester","");
ScriptRegistrant<z_Joi_KeyDropSound> z_Joi_KeyDropSound_Registrant("z_Joi_KeyDropSound","Key=1:int");
ScriptRegistrant<z_Joi_Briefing> z_Joi_Briefing_Registrant("z_Joi_Briefing","");