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
#include "z_m01.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"

void z_m01_missionstart::Created(GameObject *obj)//we want a Created event so timers for message briefing is started
{
	Commands->Start_Timer(obj,this,5.0f,100);//initialize briefing :)
	Commands->Start_Timer(obj,this,9.0f,101);
	Commands->Start_Timer(obj,this,13.0f,102);
	Commands->Start_Timer(obj,this,20.0f,103);
	Commands->Start_Timer(obj,this,25.0f,104);
	Commands->Start_Timer(obj,this,30.0f,105);
}
void z_m01_missionstart::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Create_2D_WAV_Sound("m01_locatedetentioncenter.wav");
		Send_Message(255,204,0,"Now locate the Nod detention center. The civilians are being held there.");
	}
	else if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("newmissionobjective.wav");
		Send_Message(255,204,0,"New Mission Objective");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,204,0,"Locate Capt. Duncan and obtain an Ion Cannon Beacon.");
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("m01_warningcy.wav");
		Send_Message(255,204,0,"Warning GDI Construction Yard destroyed.");
	}
	else if (number == 104)
	{
		Commands->Create_2D_WAV_Sound("m01_warningpp.wav");
		Send_Message(255,204,0,"Warning GDI Power Plant destroyed.");
	}
	else if (number == 105)
	{
		Commands->Create_2D_WAV_Sound("m01_allpersevac.wav");
		Send_Message(255,204,0,"All personell evacuate immidiately.");
	}
}

void z_m01_captduncan::Created(GameObject *obj)
{
	allowpoke = true;
	Commands->Enable_HUD_Pokable_Indicator(obj, true);//indicators for pressing e, the arrow like on vehs
}
void z_m01_captduncan::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		allowpoke = false;
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		Commands->Start_Timer(obj,this,1.0f,100);//good timing...
		Commands->Start_Timer(obj,this,4.0f,101);//ive been protect...
		Commands->Start_Timer(obj,this,7.0f,102);//its a prototype...
		Commands->Start_Timer(obj,this,13.0f,103);//great for taking...
		Commands->Start_Timer(obj,this,16.0f,104);//once you place...
		Commands->Start_Timer(obj,this,19.0f,105);//ive gotto go...
		Commands->Start_Timer(obj,this,23.0f,106);//enable ion spawner
	}
}
void z_m01_captduncan::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Create_2D_WAV_Sound("m01_duncan1.wav");
		Send_Message(255,204,0,"Good timing. Nod was closing in.");
	}
	else if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("m01_duncan2.wav");
		Send_Message(255,204,0,"I've been protecting something special.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("m01_duncan3.wav");
		Send_Message(255,204,0,"It's a prototype homing beacon for GDI's new space based Ion Cannon.");
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("m01_duncan4.wav");
		Send_Message(255,204,0,"Great for taking out large structures.");
	}
	else if (number == 104)
	{
		Commands->Create_2D_WAV_Sound("m01_duncan5.wav");
		Send_Message(255,204,0,"Once you place the beacon you have 10 seconds to clear the area.");
	}
	else if (number == 105)
	{
		Commands->Create_2D_WAV_Sound("m01_duncan6.wav");
		Send_Message(255,204,0,"I've gotto go see to my men. Good luck.");
	}
	else if (number == 106)
	{
		Commands->Enable_Spawner(Get_Int_Parameter("IonSpawnerID"),true);
		Destroy_Script();
	}
}


void z_m01_obj1_announce::Created(GameObject *obj)
{
	domsg = true;//when the scriptzone is created we want the players to be able to trigger it
}
void z_m01_obj1_announce::Entered(GameObject *obj, GameObject *enter)//the standard line for ::Entered event...
{
	if (Commands->Is_A_Star(enter))//if it is a player then proceed...
	{
		if (domsg)//if domsg then proceed...
		{
			Commands->Start_Timer(obj,this,1.0f,101);//first message runs after 1 second with number 101
			Commands->Start_Timer(obj,this,5.0f,102);//second message runs after 5 seconds with number 102
			domsg = false;
		}
	}
}
void z_m01_obj1_announce::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("newmissionobjective.wav");
		Send_Message(255,204,0,"New Mission Objective");//ok this will display the text obviously for players to see.. we're done here
	}
	else if (number == 102)// or is it our second?
	{
		Send_Message(255,204,0,"Objective: Destroy all enemy cargo trucks in the area.");//we dont have a sound for this one :)
	}
}

void z_m01_accessdeniedzone::Created(GameObject *obj)
{
	domsg = true;
}
void z_m01_accessdeniedzone::Entered(GameObject *obj, GameObject *enter)
{
	if (domsg)
	{
		domsg = false;
		Commands->Start_Timer(obj,this,15.0f,100);

		Create_2D_WAV_Sound_Player(enter,"accessdenied.wav");
		Send_Message_Player(enter,255,204,0,"Access Denied");
	}
}
void z_m01_accessdeniedzone::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		domsg = true;
	}
}

void z_m01_commcontrol::Created(GameObject *obj)
{
	allowcommdeath = false;
}
void z_m01_commcontrol::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 2500)
	{
		if (param == 1)
		{
			allowcommdeath = true;//computer was haxored...
			Commands->Start_Timer(obj,this,1.0f,100);//hmm
		}
	}
	else if (message == 3000)//comm died
	{
		if (param == 1)
		{
			if (allowcommdeath)
			{
				Commands->Start_Timer(obj,this,4.0f,301);//secondary complete
			}
			else
			{
				Commands->Start_Timer(obj,this,1.0f,200);
				Commands->Start_Timer(obj,this,4.0f,201);
				Commands->Start_Timer(obj,this,9.0f,202);
			}
		}
	}
}
void z_m01_commcontrol::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Create_2D_WAV_Sound("objective1new.wav");
		Send_Message(255,204,0,"New Primary Mission Objective");
		Commands->Start_Timer(obj,this,3.0f,101);
	}
	else if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,204,0,"Obtain the Red Key Card to access lowerlevels of the Hand of Nod.");
		Commands->Start_Timer(obj,this,3.0f,102);
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("objective2new.wav");
		Send_Message(255,204,0,"New Secondary Mission Objective");
		Commands->Start_Timer(obj,this,3.0f,103);
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("m01eval_dsgn0011i1eval_snd.wav");
		Send_Message(255,204,0,"Concider destroying the Nod Communications Center to hamper communications.");
	}
	
	else if (number == 200)
	{
		Commands->Create_2D_WAV_Sound("missionfailed.wav");
		Send_Message(255,204,0,"Your mission is a failure.");
	}
	else if (number == 201)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,204,0,"Retrieving the Yellow Security Key is not possible when the Communications Center is dead.");
	}
	else if (number == 202)
	{
		Console_Input("win 0");
	}

	else if (number == 301)
	{
		Commands->Create_2D_WAV_Sound("objective2complete.wav");
		Send_Message(255,204,0,"Secondary Objective Complete");
		Commands->Start_Timer(obj,this,3.0f,302);
	}
	else if (number == 302)
	{
		Commands->Create_2D_WAV_Sound("m01eval_dsgn0015r1eval_snd.wav");
		Send_Message(255,204,0,"Push the button next to the gate to open the detention center.");
	}

	
}


void z_m01_commpc::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	allowpoke = true;
	pokerguy = 0;
}
void z_m01_commpc::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		pokerguy = poker;
		allowpoke = false;
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Commands->Start_Timer(obj,this,1.0f,101);//accessing
		Commands->Start_Timer(obj,this,3.0f,102);//plz standby
		Commands->Start_Timer(obj,this,6.0f,103);////check key
	}
}
void z_m01_commpc::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Send_Message(255,204,0,"Accessing.");
		Commands->Create_2D_WAV_Sound("accessing.wav");
	}
	else if (number == 102)
	{		
		Send_Message(255,204,0,"Please Standby.");
		Commands->Create_2D_WAV_Sound("pleasestandby.wav");
	}
	else if (number == 103)
	{
		if (Commands->Has_Key(pokerguy,1))//if he got green key then allow access
		{
			Send_Message(255,204,0,"Access Granted.");
			Commands->Create_2D_WAV_Sound("accessgranted.wav");
			Commands->Start_Timer(obj,this,3.0f,150);//enable yellow key spawn + inform control commcenter may be killed...
		}
		else 
		{
			Send_Message(255,204,0,"Access Denied.");
			Commands->Create_2D_WAV_Sound("accessdenied.wav");
			allowpoke = true;
			Commands->Enable_HUD_Pokable_Indicator(obj,true);
		}
	}
	else if (number == 150)
	{
		Commands->Enable_Spawner(126391,true);
		Commands->Send_Custom_Event(obj,Commands->Find_Object(126392),2500,1,0);
		Destroy_Script();//nothing to do anymore lol
	}
}

void z_m01_missionend::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,1.0f,101);
			Commands->Start_Timer(obj,this,5.0f,102);
			Commands->Start_Timer(obj,this,7.5f,103);
		}
	}
}
void z_m01_missionend::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("m01eval_dsgn0016i1eval_snd.wav");
		Send_Message(255,204,0,"Good job Havoc. There's hope for you yet.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("missionaccomplished.wav");
		Send_Message(255,204,0,"Mission Accomplished");
	}
	else if (number == 103)
	{
		Console_Input("gameover");
	}
}

void z_m01_civbarn_rec::Created(GameObject *obj)
{
	civ1 = 0;
	civ2 = 0;
	civ3 = 0;
	civ4 = 0;
	civ5 = 0;
	res1 = false;
	res2 = false;
	res3 = false;
	res4 = false;
	res5 = false;
	civdeaths = 0;
	civres = 0;
	reqres = Get_Int_Parameter("RequiredRescues");
	maxdeaths = Get_Int_Parameter("MaxDeaths");
	Commands->Start_Timer(obj,this,5.0f,600);//check pos civs
}
void z_m01_civbarn_rec::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1001)//civilian 1 created
	{
		civ1 = param;//param is obj id
	}
	else if (message == 1002)//civilian 2 created
	{
		civ2 = param;//param is obj id
	}
	else if (message == 1003)//civilian 3 created
	{
		civ3 = param;//param is obj id
	}
	else if (message == 1004)//civilian 4 created
	{
		civ4 = param;//param is obj id
	}
	else if (message == 1005)//civilian 5 created
	{
		civ5 = param;//param is obj id
	}

	else if (message == 3000)//civ died
	{
		if (param == 1)
		{
			civdeaths++;
			if (civdeaths == maxdeaths)
			{
				Commands->Start_Timer(obj,this,4.5f,100);
				Commands->Start_Timer(obj,this,8.0f,101);
			}
		}
	}
	else if (message == 4000)//civ made it into the rescue zone
	{
		if (param == 1)
		{
			civres++;
			if (civres == reqres)//if we got enough rescues call in the chopper
			{
				Commands->Start_Timer(obj,this,1.0f,199);//first call the chopper
				Commands->Start_Timer(obj,this,1.0f,200);//round up civs
				Commands->Start_Timer(obj,this,3.5f,201);//trans enroute
				Commands->Start_Timer(obj,this,6.0f,210);//obj2 complete
				Commands->Start_Timer(obj,this,8.5f,211);//obj2 new
				Commands->Start_Timer(obj,this,10.5f,212);//locate detention
			}
			Commands->Create_2D_WAV_Sound("incmsg.wav");
			Send_Message(255,255,255,"A Civilian was rescued.");
			Remove_Script(sender,"z_m01_civilian");
		}
	}
	else if (message == 5000)//chopper flies away destroy the civilian
	{
		if (param == 1)
		{
			Commands->Destroy_Object(Commands->Find_Object(civ1));
			Commands->Destroy_Object(Commands->Find_Object(civ2));
			Commands->Destroy_Object(Commands->Find_Object(civ3));
			Commands->Destroy_Object(Commands->Find_Object(civ4));
			Commands->Destroy_Object(Commands->Find_Object(civ5));
			Commands->Destroy_Object(Commands->Find_Object(126417));//sigflare
		}
	}
}
void z_m01_civbarn_rec::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)//mission fail
	{
		Commands->Create_2D_WAV_Sound("missionfailed.wav");
		Send_Message(255,204,0,"Your mission is a failure.");
	}
	else if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,204,0,"Too many civilians were killed.");
		Commands->Start_Timer(obj,this,6.0f,666);
	}
	else if (number == 666)
	{
		Console_Input("win 0");
	}


	else if (number == 199)
	{
		const char *scriptname;
		Vector3 position;
		float facing;
		GameObject *object;
		scriptname = Get_Parameter("Script_Name");
		position = Get_Vector3_Parameter("location");
		facing = Get_Float_Parameter("Facing");
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
	}
	else if (number == 200)
	{
		Commands->Create_2D_WAV_Sound("roundupcivsforevac.wav");
		Send_Message(255,204,0,"Round up the civilians for evacuation.");
	}
	else if (number == 201)
	{
		Commands->Create_2D_WAV_Sound("transportheli_enroute.wav");
		Send_Message(255,204,0,"A transport helicopter is enroute.");
	}
	else if (number == 210)//obj complete
	{
		Commands->Create_2D_WAV_Sound("objective2complete.wav");
		Send_Message(255,204,0,"Secondary Objective Complete.");
	}
	else if (number == 211)//new obj
	{
		Commands->Create_2D_WAV_Sound("objective2new.wav");
		Send_Message(255,204,0,"New Secondary Mission Objective.");
	}
	else if (number == 212)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,204,0,"Acces Lower Levels of Hand of Nod and Ion the Pedestal.");
	}

	else if (number == 600)
	{
		Vector3 mypos = Commands->Get_Position(obj);
		Vector3 target1pos = Commands->Get_Position(Find_Object_By_Preset(2,"M01_Civilian_Barn_1"));
		Vector3 target2pos = Commands->Get_Position(Find_Object_By_Preset(2,"M01_Civ3_Female"));
		Vector3 target3pos = Commands->Get_Position(Find_Object_By_Preset(2,"M01_Civ4_Male1"));
		Vector3 target4pos = Commands->Get_Position(Find_Object_By_Preset(2,"M01_Civ5_Male2"));
		Vector3 target5pos = Commands->Get_Position(Find_Object_By_Preset(2,"M01_Cook_Civ2"));
		float distance1 = Commands->Get_Distance(target1pos,mypos);
		float distance2 = Commands->Get_Distance(target2pos,mypos);
		float distance3 = Commands->Get_Distance(target3pos,mypos);
		float distance4 = Commands->Get_Distance(target4pos,mypos);
		float distance5 = Commands->Get_Distance(target5pos,mypos);
		if (!res1)
		{
			if (distance1 < 25.0f)
			{
				res1 = true;
				Commands->Send_Custom_Event(obj,Find_Object_By_Preset(2,"M01_Civilian_Barn_1"),2000,1,0);//tell civ to inform controller he was rescued
			}
		}
		if (!res2)
		{
			if (distance2 < 25.0f)
			{
				res2 = true;
				Commands->Send_Custom_Event(obj,Find_Object_By_Preset(2,"M01_Civ3_Female"),2000,1,0);//tell civ to inform controller he was rescued
			}
		}
		if (!res3)
		{
			if (distance3 < 25.0f)
			{
				res3 = true;
				Commands->Send_Custom_Event(obj,Find_Object_By_Preset(2,"M01_Civ4_Male1"),2000,1,0);//tell civ to inform controller he was rescued
			}
		}
		if (!res4)
		{
			if (distance4 < 25.0f)
			{
				res4 = true;
				Commands->Send_Custom_Event(obj,Find_Object_By_Preset(2,"M01_Civ5_Male2"),2000,1,0);//tell civ to inform controller he was rescued
			}
		}
		if (!res5)
		{
			if (distance5 < 25.0f)
			{
				res5 = true;
				Commands->Send_Custom_Event(obj,Find_Object_By_Preset(2,"M01_Cook_Civ2"),2000,1,0);//tell civ to inform controller he was rescued
			}
		}
		Commands->Start_Timer(obj,this,5.0f,600);
	}
}

void z_m01_civilian::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	FollowID = 0;
	pokerguy = 0;
	allowpoke = true;
	sendmsg = true;
	cmsg = Get_Int_Parameter("CreatedMessage");
	kmsg = Get_Int_Parameter("KilledMessage");
	cid = Get_Int_Parameter("ControllerID");
	Commands->Send_Custom_Event(obj,Commands->Find_Object(cid),cmsg,Commands->Get_ID(obj),0);//send creation id
}
void z_m01_civilian::Poked(GameObject *obj, GameObject *poker)
{
	if (Commands->Is_A_Star(poker))
	{
		if (allowpoke)
		{
			allowpoke = false;
			Commands->Enable_HUD_Pokable_Indicator(obj, false);
			Commands->Set_Player_Type(obj,1);


			int waypathid;
			float speed;
			waypathid = Get_Int_Parameter("Waypathid");
			speed = 1.0f;
			ActionParamsStruct params;
			params.MoveArrivedDistance = 100;
			params.Set_Basic(this,100,777);
			params.Set_Movement(0,speed,10);
			params.WaypathID = waypathid;
			params.WaypathSplined = true;
			params.AttackActive = false;
			params.AttackCheckBlocked = false;
			Commands->Action_Goto(obj,params);
			Commands->Action_Goto(obj,params);//SIGH THE FUCKING RESCUE ZONE BLOCKS THEM LOLOLOLOL
		}
	}
}
void z_m01_civilian::Timer_Expired(GameObject *obj, int number)
{
	if (number == 789)//some old code here from when i made them follow players instead, not gonna clean it up lol....
	{
		if (pokerguy)
		{
			GameObject *GotoObject = Commands->Find_Object(FollowID);
			ActionParamsStruct params;
			params.Set_Basic(this,100,100);
			params.Set_Movement(GotoObject,1.0f,50.0f);
			Commands->Action_Goto(obj,params);
			Commands->Start_Timer(obj,this,1.0f,789);
		}
		else 
		{
			pokerguy = 0;
			Commands->Enable_HUD_Pokable_Indicator(obj, true);
			allowpoke = true;
		}
	}
}
void z_m01_civilian::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(cid),kmsg,1,0);//increment deaths
	Commands->Create_2D_WAV_Sound("civkilled.wav");
	Send_Message(255,255,255,"Civilian Killed");	
}
void z_m01_civilian::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	/*if (message == 1000)//enter in cinematic txt file
	{
		if (param == 1)
		{
			Commands->Destroy_Object(obj);//rescued.. chopper flies away 
		}
	}*/
	if (message == 2000)//i entred the rescue zone, inform controller
	{
		if (param == 1)
		{
			if (sendmsg)
			{
				FollowID = 0;
				pokerguy = 0;
				sendmsg = false;
				Commands->Send_Custom_Event(obj,Commands->Find_Object(cid),4000,1,0);
			}
		}
	}
}

void z_m01_civreszonebarn::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,5.0f,100);
}
void z_m01_civreszonebarn::Entered(GameObject *obj, GameObject *enter)
{
	if (
	(strcmp(Commands->Get_Preset_Name(enter),"M01_Civilian_Barn_1") == 0) || 
	(strcmp(Commands->Get_Preset_Name(enter),"M01_Civ3_Female") == 0) || 
	(strcmp(Commands->Get_Preset_Name(enter),"M01_Civ4_Male1") == 0) || 
	(strcmp(Commands->Get_Preset_Name(enter),"M01_Civ5_Male2") == 0) || 
	(strcmp(Commands->Get_Preset_Name(enter),"M01_Cook_Civ2") == 0)
	)
	{
		Commands->Send_Custom_Event(obj,enter,2000,1,0);//tell civ to inform controller he was rescued
	}
}
void z_m01_civreszonebarn::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		//check
	}
}

void z_m01_church_evac_control::Created(GameObject *obj)
{
	civ1 = 0;
	civ2 = 0;
	res1 = false;
	res2 = false;
	civdeaths = 0;
	civres = 0;
	reqres = Get_Int_Parameter("RequiredRescues");
	maxdeaths = Get_Int_Parameter("MaxDeaths");
	Commands->Start_Timer(obj,this,5.0f,600);//check pos civs
}
void z_m01_church_evac_control::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1001)//civilian 1 created
	{
		civ1 = param;//param is obj id
	}
	else if (message == 1002)//civilian 2 created
	{
		civ2 = param;//param is obj id
	}

	else if (message == 3000)//civ died
	{
		if (param == 1)
		{
			civdeaths++;
			if (civdeaths == maxdeaths)
			{
				Commands->Start_Timer(obj,this,4.5f,100);
				Commands->Start_Timer(obj,this,8.0f,101);
			}
		}
	}

	else if (message == 4000)//civ made it into the rescue zone
	{
		if (param == 1)
		{
			civres++;
			if (civres == reqres)//if we got enough rescues call in the chopper
			{
				Commands->Start_Timer(obj,this,1.0f,199);//first call the chopper
				Commands->Start_Timer(obj,this,1.0f,200);//round up civs
				Commands->Start_Timer(obj,this,3.5f,201);//trans enroute
				Commands->Start_Timer(obj,this,6.0f,210);//obj2 complete
				Commands->Start_Timer(obj,this,8.5f,211);//obj2 new
				Commands->Start_Timer(obj,this,10.5f,212);//locate detention
			}
			Commands->Create_2D_WAV_Sound("incmsg.wav");
			Send_Message(255,255,255,"A Civilian was rescued.");
		}
	}
	else if (message == 5000)//chopper flies away destroy the civilian
	{
		if (param == 1)
		{
			Commands->Destroy_Object(Commands->Find_Object(civ1));
			Commands->Destroy_Object(Commands->Find_Object(civ2));
			Commands->Destroy_Object(Commands->Find_Object(126533));//sigflare
			Commands->Send_Custom_Event(obj,Commands->Find_Object(126536),1000,1,0);
		}
	}

}
void z_m01_church_evac_control::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)//mission fail
	{
		Commands->Create_2D_WAV_Sound("missionfailed.wav");
		Send_Message(255,204,0,"Your mission is a failure.");
	}
	else if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("incmsg.wav");
		Send_Message(255,204,0,"Too many civilians were killed.");
		Commands->Start_Timer(obj,this,6.0f,666);
	}
	else if (number == 666)
	{
		Console_Input("win 0");
	}


	else if (number == 199)
	{
		const char *scriptname;
		Vector3 position;
		float facing;
		GameObject *object;
		scriptname = Get_Parameter("Script_Name");
		position = Get_Vector3_Parameter("location");
		facing = Get_Float_Parameter("Facing");
		object = Commands->Create_Object("Invisible_Object",position);
		Commands->Set_Facing(object,facing);
		Commands->Attach_Script(object,"Test_Cinematic",scriptname);
	}
	else if (number == 200)
	{
		Commands->Create_2D_WAV_Sound("roundupcivsforevac.wav");
		Send_Message(255,204,0,"Round up the civilians for evacuation.");
	}
	else if (number == 201)
	{
		Commands->Create_2D_WAV_Sound("transportheli_enroute.wav");
		Send_Message(255,204,0,"A transport helicopter is enroute.");
	}
	else if (number == 210)//obj complete
	{
		Commands->Create_2D_WAV_Sound("objective2complete.wav");
		Send_Message(255,204,0,"Secondary Objective Complete.");
	}
	else if (number == 211)//new obj
	{
		Commands->Create_2D_WAV_Sound("objective2new.wav");
		Send_Message(255,204,0,"New Secondary Mission Objective.");
	}
	else if (number == 212)
	{
		Commands->Create_2D_WAV_Sound("m01eval_dsgn0001i1eval_snd.wav");
		Send_Message(255,204,0,"Now locate the Nod detention center. The civilians are being held there.");
	}

	else if (number == 600)
	{
		Vector3 mypos = Commands->Get_Position(obj);
		Vector3 target1pos = Commands->Get_Position(Find_Object_By_Preset(2,"M01_Church_Civ1"));
		Vector3 target2pos = Commands->Get_Position(Find_Object_By_Preset(2,"M01_Church_Civ2"));
		float distance1 = Commands->Get_Distance(target1pos,mypos);
		float distance2 = Commands->Get_Distance(target2pos,mypos);
		if (!res1)
		{
			if (distance1 < 17.0f)
			{
				res1 = true;
				Commands->Send_Custom_Event(obj,Find_Object_By_Preset(2,"M01_Church_Civ1"),2000,1,0);//tell civ to inform controller he was rescued
			}
		}
		if (!res2)
		{
			if (distance2 < 17.0f)
			{
				res2 = true;
				Commands->Send_Custom_Event(obj,Find_Object_By_Preset(2,"M01_Church_Civ2"),2000,1,0);//tell civ to inform controller he was rescued
			}
		}
		Commands->Start_Timer(obj,this,5.0f,600);
	}
}

void z_m01_civilian_church::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	allowpoke = true;
	sendmsg = true;
	cmsg = Get_Int_Parameter("CreatedMessage");
	kmsg = Get_Int_Parameter("KilledMessage");
	cid = Get_Int_Parameter("ControllerID");
	Commands->Send_Custom_Event(obj,Commands->Find_Object(cid),cmsg,Commands->Get_ID(obj),0);//send creation id
}
void z_m01_civilian_church::Killed(GameObject *obj, GameObject *shooter)
{
	Commands->Send_Custom_Event(obj,Commands->Find_Object(cid),kmsg,1,0);//increment deaths
	Commands->Create_2D_WAV_Sound("civkilled.wav");
	Send_Message(255,255,255,"Civilian Killed");	
}
void z_m01_civilian_church::Poked(GameObject *obj, GameObject *poker)
{
	if (Commands->Is_A_Star(poker))
	{
		if (allowpoke)
		{
			allowpoke = false;
			Commands->Enable_HUD_Pokable_Indicator(obj, false);
			Commands->Set_Player_Type(obj,1);
			int waypathid;
			float speed;
			waypathid = Get_Int_Parameter("Waypathid");
			speed = 1.0f;
			ActionParamsStruct params;
			params.MoveArrivedDistance = 5;
			params.Set_Basic(this,100,777);
			params.Set_Movement(0,speed,10);
			params.WaypathID = waypathid;
			params.WaypathSplined = true;
			params.AttackActive = false;
			params.AttackCheckBlocked = false;
			Commands->Action_Goto(obj,params);//SIGH THE FUCKING RESCUE ZONE BLOCKS THEM LOLOLOLOL
		}
	}
}
void z_m01_civilian_church::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 2000)//i entred the rescue zone, inform controller
	{
		if (param == 1)
		{
			if (sendmsg)
			{
				sendmsg = false;
				Commands->Send_Custom_Event(obj,Commands->Find_Object(cid),4000,1,0);
			}
		}
	}
}

void z_m01_checkpoint_1::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	locked = true;
	allowpoke = true;
	gotoid1 = Get_Int_Parameter("GotoID1");
	gotoid2 = Get_Int_Parameter("GotoID2");
	gotoid3 = Get_Int_Parameter("GotoID3");
	gotoid4 = Get_Int_Parameter("GotoID4");
	gotoid5 = Get_Int_Parameter("GotoID5");
}
void z_m01_checkpoint_1::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (locked)
		{
			Create_2D_WAV_Sound_Player(poker,"accessdenied.wav");
			Send_Message_Player(poker,255,255,255,"Access Denied");
			Send_Message_Player(poker,255,204,0,"Checkpoint Teleporter to Barn area locked. Complete Objectives to Unlock it.");
		}
		else
		{
			int random = Commands->Get_Random_Int(1,5);
			if (random == 1)
			{
				Vector3 gotoloc1 = Commands->Get_Position(Commands->Find_Object(gotoid1));
				Commands->Set_Position(poker,gotoloc1);
			}
			else if (random == 2)
			{
				Vector3 gotoloc2 = Commands->Get_Position(Commands->Find_Object(gotoid2));
				Commands->Set_Position(poker,gotoloc2);
			}
			else if (random == 3)
			{
				Vector3 gotoloc3 = Commands->Get_Position(Commands->Find_Object(gotoid3));
				Commands->Set_Position(poker,gotoloc3);
			}
			else if (random == 4)
			{
				Vector3 gotoloc4 = Commands->Get_Position(Commands->Find_Object(gotoid4));
				Commands->Set_Position(poker,gotoloc4);
			}
			else if (random == 5)
			{
				Vector3 gotoloc5 = Commands->Get_Position(Commands->Find_Object(gotoid5));
				Commands->Set_Position(poker,gotoloc5);
			}
		}
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		allowpoke = false;
		Commands->Start_Timer(obj,this,5.0f,200);//allowpoke true
	}
}
void z_m01_checkpoint_1::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == Get_Int_Parameter("Open_Checkpoint_Msg"))
	{ 
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,5.5f,100);//long timer cos of obj announcemetns etc...
		}
	}
}
void z_m01_checkpoint_1::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Send_Message(255,204,0,"Checkpoint secure! Keep up the good work Havoc.");
		Commands->Create_2D_WAV_Sound("checkpointsecure.wav");
		Commands->Start_Timer(obj,this,3.5f,101);
	}
	else if (number == 101)
	{
		locked = false;
		Send_Message(255,204,0,"Checkpoint Teleporter to Barn area unlocked.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
	}
	else if (number == 200)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}

void z_m01_checkpoint_2::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	locked = true;
	allowpoke = true;
	gotoid1 = Get_Int_Parameter("GotoID1");
	gotoid2 = Get_Int_Parameter("GotoID2");
	gotoid3 = Get_Int_Parameter("GotoID3");
	gotoid4 = Get_Int_Parameter("GotoID4");
	gotoid5 = Get_Int_Parameter("GotoID5");
}
void z_m01_checkpoint_2::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (locked)
		{
			Create_2D_WAV_Sound_Player(poker,"accessdenied.wav");
			Send_Message_Player(poker,255,255,255,"Access Denied");
			Send_Message_Player(poker,255,204,0,"Checkpoint Teleporter to Church area locked. Complete Objectives to Unlock it.");
		}
		else
		{
			int random = Commands->Get_Random_Int(1,5);
			if (random == 1)
			{
				Vector3 gotoloc1 = Commands->Get_Position(Commands->Find_Object(gotoid1));
				Commands->Set_Position(poker,gotoloc1);
			}
			else if (random == 2)
			{
				Vector3 gotoloc2 = Commands->Get_Position(Commands->Find_Object(gotoid2));
				Commands->Set_Position(poker,gotoloc2);
			}
			else if (random == 3)
			{
				Vector3 gotoloc3 = Commands->Get_Position(Commands->Find_Object(gotoid3));
				Commands->Set_Position(poker,gotoloc3);
			}
			else if (random == 4)
			{
				Vector3 gotoloc4 = Commands->Get_Position(Commands->Find_Object(gotoid4));
				Commands->Set_Position(poker,gotoloc4);
			}
			else if (random == 5)
			{
				Vector3 gotoloc5 = Commands->Get_Position(Commands->Find_Object(gotoid5));
				Commands->Set_Position(poker,gotoloc5);
			}
		}
		Commands->Enable_HUD_Pokable_Indicator(obj, false);
		allowpoke = false;
		Commands->Start_Timer(obj,this,5.0f,200);//allowpoke true
	}
}
void z_m01_checkpoint_2::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == Get_Int_Parameter("Open_Checkpoint_Msg"))
	{ 
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,5.5f,100);//long timer cos of obj announcemetns etc...
		}
	}
}
void z_m01_checkpoint_2::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Send_Message(255,204,0,"Checkpoint secure! Keep up the good work Havoc.");
		Commands->Create_2D_WAV_Sound("checkpointsecure.wav");
		Commands->Start_Timer(obj,this,3.5f,101);
	}
	else if (number == 101)
	{
		locked = false;
		Send_Message(255,204,0,"Checkpoint Teleporter to Church area unlocked.");
		Commands->Create_2D_WAV_Sound("incmsg.wav");
	}
	else if (number == 200)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
}

void z_m01_disable_honzone::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Commands->Enable_Spawner(126778,false);
			Commands->Enable_Spawner(126779,false);
			Commands->Enable_Spawner(126780,false);
			Commands->Enable_Spawner(126781,false);
			Commands->Enable_Spawner(126789,false);
			Commands->Enable_Spawner(126790,false);
			Commands->Enable_Spawner(126795,false);
			Commands->Enable_Spawner(126796,false);
			Commands->Enable_Spawner(126797,false);
			Commands->Enable_Spawner(126801,false);
			Commands->Enable_Spawner(126807,false);
		}
	}
}

void z_m01_disable_commzone::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Commands->Enable_Spawner(126738,false);
			Commands->Enable_Spawner(126737,false);
			Commands->Enable_Spawner(126736,false);
			Commands->Enable_Spawner(126739,false);
			Commands->Enable_Spawner(126740,false);
			Commands->Enable_Spawner(126741,false);
			Commands->Enable_Spawner(126742,false);
			Commands->Enable_Spawner(126743,false);
			Commands->Enable_Spawner(126744,false);
			Commands->Enable_Spawner(126745,false);
			Commands->Enable_Spawner(126756,false);
			Commands->Enable_Spawner(126755,false);
			Commands->Enable_Spawner(126760,false);
			Commands->Enable_Spawner(126761,false);
			Commands->Enable_Spawner(126764,false);
			Commands->Enable_Spawner(126765,false);
			Commands->Enable_Spawner(126766,false);
			Commands->Enable_Spawner(126767,false);
		}
	}
}

ScriptRegistrant<z_m01_disable_commzone> z_m01_disable_commzone_Registrant("z_m01_disable_commzone","");
ScriptRegistrant<z_m01_disable_honzone> z_m01_disable_honzone_Registrant("z_m01_disable_honzone","");
ScriptRegistrant<z_m01_checkpoint_2> z_m01_checkpoint_2_Registrant("z_m01_checkpoint_2","Open_Checkpoint_Msg=1000:int,GotoID1=1:int,GotoID2=2:int,GotoID3=3:int,GotoID4=4:int,GotoID5=5:int");
ScriptRegistrant<z_m01_checkpoint_1> z_m01_checkpoint_1_Registrant("z_m01_checkpoint_1","Open_Checkpoint_Msg=1000:int,GotoID1=1:int,GotoID2=2:int,GotoID3=3:int,GotoID4=4:int,GotoID5=5:int");
ScriptRegistrant<z_m01_civilian_church> z_m01_civilian_church_Registrant("z_m01_civilian_church","ControllerID=1:int,CreatedMessage=1001:int,KilledMessage=3000:int,Waypathid=1:int");
ScriptRegistrant<z_m01_church_evac_control> z_m01_church_evac_control_Registrant("z_m01_church_evac_control","RequiredRescues=3:int,MaxDeaths=3:int,Script_Name=asdf:string,location:vector3,Facing=5:float");
ScriptRegistrant<z_m01_civreszonebarn> z_m01_civreszonebarn_Registrant("z_m01_civreszonebarn","");
ScriptRegistrant<z_m01_civilian> z_m01_civilian_Registrant("z_m01_civilian","ControllerID=1:int,CreatedMessage=1001:int,KilledMessage=3000:int,Waypathid=1:int");
ScriptRegistrant<z_m01_civbarn_rec> z_m01_civbarn_rec_Registrant("z_m01_civbarn_rec","RequiredRescues=1:int,MaxDeaths=1:int,Script_Name=asdf:string,location:vector3,Facing=5:float");
ScriptRegistrant<z_m01_missionend> z_m01_missionend_Registrant("z_m01_missionend","");
ScriptRegistrant<z_m01_commpc> z_m01_commpc_Registrant("z_m01_commpc","");
ScriptRegistrant<z_m01_commcontrol> z_m01_commcontrol_Registrant("z_m01_commcontrol","");
ScriptRegistrant<z_m01_accessdeniedzone> z_m01_accessdeniedzone_Registrant("z_m01_accessdeniedzone","");
ScriptRegistrant<z_m01_obj1_announce> z_m01_obj1_announce_Registrant("z_m01_obj1_announce","");
ScriptRegistrant<z_m01_captduncan> z_m01_captduncan_Registrant("z_m01_captduncan","IonSpawnerID=1:int");
ScriptRegistrant<z_m01_missionstart> z_m01_missionstart_Registrant("z_m01_missionstart","");