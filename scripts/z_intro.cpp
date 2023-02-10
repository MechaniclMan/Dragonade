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
#include "z_intro.h"
#include "BuildingGameObjDef.h"
#include "VehicleGameObjDef.h"

void z_intro_end::Created(GameObject *obj)
{
	//Console_Input("msg Endgame cinematic created");
}
void z_intro_end::Timer_Expired(GameObject *obj, int number)
{
	//
}

void z_intro_lolzone::Entered(GameObject *obj, GameObject *enter)
{
	if (Commands->Is_A_Star(enter))
	{
		Commands->Apply_Damage(enter,20000.0f,"Death",enter);
	}
}
void z_intro_lolzone::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Destroy_Script();
		}
	}
}

void z_intro_start::Created(GameObject *obj)
{
	//255,204,0 gdi yellow
	option = Get_Int_Parameter("Option");

	if (option == 1)
	{
		Commands->Create_2D_WAV_Sound("intro_1.wav");
		Send_Message(255,204,0,"Man if Nod doesn't kill us this heat will.");
	}
	else if (option == 2)
	{
		Commands->Create_2D_WAV_Sound("intro_2.wav");
		Send_Message(255,204,0,"What are we looking for again?");
	}
	else if (option == 3)
	{
		Commands->Create_2D_WAV_Sound("intro_3.wav");
		Send_Message(255,204,0,"An imaginary base.");
	}
	else if (option == 4)
	{
		Commands->Create_2D_WAV_Sound("intro_4.wav");
		Send_Message(255,204,0,"Quiet...");
	}
	else if (option == 5)
	{
		Commands->Create_2D_WAV_Sound("intro_5.wav");
		Send_Message(255,204,0,"Exactly. It's too quiet.");
	}
	else if (option == 6)
	{
		Commands->Create_2D_WAV_Sound("intro_6.wav");
		Send_Message(255,204,0,"Ugh. My radar is acting up.");
	}
	else if (option == 7)
	{
		Commands->Create_2D_WAV_Sound("intro_7.wav");
		Send_Message(255,204,0,"It's this terrain.");
	}
	else if (option == 8)
	{
		Commands->Create_2D_WAV_Sound("intro_8.wav");
		Send_Message(255,204,0,"Stay loose.");
		Send_Message(255,204,0,"Keep your eyes open.");
		Commands->Create_2D_WAV_Sound("intro_8b.wav");
	}
	else if (option == 9)
	{
		Commands->Create_2D_WAV_Sound("intro_9.wav");
		Send_Message(255,204,0,"Recon One this is Eagle Base. What's the delay up there?");
	}
	else if (option == 10)
	{
		Commands->Create_2D_WAV_Sound("intro_10.wav");
		Send_Message(255,204,0,"Copy Eagle Base. Still no sign of any Nod Installation.");
	}
	else if (option == 11)
	{
		Commands->Create_2D_WAV_Sound("intro_11.wav");
		Send_Message(255,204,0,"Take cover! Move! Move! Move!");
	}
	else if (option == 12)
	{
		Commands->Create_2D_WAV_Sound("intro_12.wav");
		Send_Message(255,204,0,"Ambush!");
	}
	else if (option == 13)
	{
		Commands->Create_2D_WAV_Sound("intro_13.wav");
		Send_Message(255,204,0,"It's a trap!");
	}
	else if (option == 14)
	{
		Commands->Create_2D_WAV_Sound("intro_14.wav");
		Send_Message(255,204,0,"Get down!!");
	}
	else if (option == 15)
	{
		Commands->Create_2D_WAV_Sound("intro_15.wav");
		Send_Message(255,204,0,"Medic!");
	}
	else if (option == 16)
	{
		Commands->Create_2D_WAV_Sound("intro_16.wav");
		Send_Message(255,204,0,"Eagle Base. Under heavy fire. Requesting immediate reinforcements.");
	}
	else if (option == 17)
	{
		Commands->Create_2D_WAV_Sound("intro_17+18.wav");
		Send_Message(255,204,0,"This is Recon One we're taking fire. We're taking...AAAAAaaahhh!!!");
	}
	else if (option == 19)
	{
		Commands->Create_2D_WAV_Sound("intro_19.wav");
		Send_Message(255,204,0,"Hang in there Recon One. We're sending a Commando!");
	}
}

void z_intro_humm::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0f,101);
	Commands->Start_Timer(obj,this,5.0f,102);
	Commands->Start_Timer(obj,this,7.0f,103);
}
void z_intro_humm::Timer_Expired(GameObject *obj, int number)
{
	//255,204,0 gdi yellow
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("intro_hum1.wav");
		Send_Message(255,204,0,"Eagle Base. We're on the ground.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("intro_hum2.wav");
		Send_Message(255,204,0,"Eagle Base. We must be close.");
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("intro_hum3.wav");
		Send_Message(255,204,0,"There's a Nod Harvester here.");
	}
}

void z_intro_ioncountdown::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			Commands->Create_2D_WAV_Sound("ioncountdown.wav");
			Send_Message(255,204,0,"You have 10 seconds to reach minimum safe distance");
			Commands->Start_Timer(obj,this,5.6f,109);
			Commands->Start_Timer(obj,this,6.6f,108);
			Commands->Start_Timer(obj,this,7.6f,107);
			Commands->Start_Timer(obj,this,8.6f,106);
			Commands->Start_Timer(obj,this,9.6f,105);
			Commands->Start_Timer(obj,this,10.6f,104);
			Commands->Start_Timer(obj,this,11.6f,103);
			Commands->Start_Timer(obj,this,12.6f,102);
			Commands->Start_Timer(obj,this,13.6f,101);
			Commands->Start_Timer(obj,this,14.6f,100);
		}
	}
}

void z_intro_ioncountdown::Timer_Expired(GameObject *obj, int number)
{
	if (number == 109)
	{
		Send_Message(255,204,0,"Nine");
	}
	else if (number == 108)
	{
		Send_Message(255,204,0,"Eight");
	}
	else if (number == 107)
	{
		Send_Message(255,204,0,"Seven");
	}
	else if (number == 106)
	{
		Send_Message(255,204,0,"Six");
	}
	else if (number == 105)
	{
		Send_Message(255,204,0,"Five");
	}
	else if (number == 104)
	{
		Send_Message(255,204,0,"Four");
	}
	else if (number == 103)
	{
		Send_Message(255,204,0,"Three");
	}
	else if (number == 102)
	{
		Send_Message(255,204,0,"Two");
	}
	else if (number == 101)
	{
		Send_Message(255,204,0,"One");
	}
	else if (number == 100)
	{
		Send_Message(255,204,0,"Zero");
	}
}

void z_intro_a10crash::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 2000)
	{
		if (param == 1)
		{
			Commands->Start_Timer(obj,this,1.0f,201);
			Commands->Start_Timer(obj,this,4.0f,202);
		}
	}
}
void z_intro_a10crash::Timer_Expired(GameObject *obj, int number)
{
	if (number == 201)
	{
		Commands->Create_2D_WAV_Sound("mx0_gdieaglebase_168.wav ");
		Send_Message(255,204,0,"This is Eagle Base. I'm not risking any more pilots.");
	}
	else if (number == 202)
	{
		Commands->Create_2D_WAV_Sound("mx0_gdieaglebase_169.wav");
		Send_Message(255,204,0,"We have a GPS lock on the Nod installation. Prepare for Ion Cannon Strike.");
	}
}

void z_intro_finale::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0f,101);
	Commands->Start_Timer(obj,this,4.0f,102);
	Commands->Start_Timer(obj,this,8.0f,103);
	Commands->Start_Timer(obj,this,12.0f,104);
}
void z_intro_finale::Timer_Expired(GameObject *obj, int number)
{//255,204,0 gdi yellow
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("mx0_gdieaglebase_176.wav");
		Send_Message(255,204,0,"The Nod Base is down. Good job Havoc.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("mx0_gdieaglebase_177.wav");
		Send_Message(255,204,0,"The Base computer should contain a wealth of intelligence data.");
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("mx0_gdieaglebase_178.wav");
		Send_Message(255,204,0,"Mission Accomplished");
	}
	else if (number == 104)
	{
		Console_Input("gameover");
	}
}

void z_intro_orcaharv::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0f,101);
	Commands->Start_Timer(obj,this,4.0f,102);
	Commands->Start_Timer(obj,this,7.0f,103);
	Commands->Start_Timer(obj,this,8.5f,104);
	Commands->Start_Timer(obj,this,10.0f,105);
}
void z_intro_orcaharv::Timer_Expired(GameObject *obj, int number)
{
	if (number == 101)
	{
		Commands->Create_2D_WAV_Sound("mx0_orca6_138.wav");
		Send_Message(255,204,0,"Orca6 to Eagle Base. I have a visual on the Harvester.");
	}
	else if (number == 102)
	{
		Commands->Create_2D_WAV_Sound("mx0_orca6_139.wav");
		Send_Message(255,204,0,"Starting my run now...");
	}
	else if (number == 103)
	{
		Commands->Create_2D_WAV_Sound("mx0_orca6_141.wav");
		Send_Message(255,204,0,"This is Orca6. Bingo Fuel. Returning to base.");
	}
	else if (number == 104)
	{
		Commands->Create_2D_WAV_Sound("mx0_gdieaglebase_142.wav");
		Send_Message(255,204,0,"Copy Orca6. Your ball Havoc.");
	}
	else if (number == 105)
	{
		Commands->Create_2D_WAV_Sound("mx0_gdieaglebase_143.wav");
		Send_Message(255,204,0,"Finish off that harvester.");
	}
}


ScriptRegistrant<z_intro_orcaharv> z_intro_orcaharv_Registrant("z_intro_orcaharv","");
ScriptRegistrant<z_intro_finale> z_intro_finale_Registrant("z_intro_finale","");
ScriptRegistrant<z_intro_a10crash> z_intro_a10crash_Registrant("z_intro_a10crash","");
ScriptRegistrant<z_intro_ioncountdown> z_intro_ioncountdown_Registrant("z_intro_ioncountdown","");
ScriptRegistrant<z_intro_humm> z_intro_humm_Registrant("z_intro_humm","");
ScriptRegistrant<z_intro_start> z_intro_start_Registrant("z_intro_start","Option=1:int");
ScriptRegistrant<z_intro_lolzone> z_intro_lolzone_Registrant("z_intro_lolzone","");
ScriptRegistrant<z_intro_end> z_intro_end_Registrant("z_intro_end","");