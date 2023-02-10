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
#include "tcw_paul.h"
#include "tcw_general.h"
#include "tcw_functions.h"
#include "VehicleGameObj.h"
#include "engine.h"
#include "GameObjManager.h"
#include "BuildingGameObj.h"


/////////////////////////////////////////
//		USS Enviseration Script		  //
////////////////////////////////////////
void tcw_USS_Enviseration_carrier::Created(GameObject *obj)
{
	trigger = true;
}
void tcw_USS_Enviseration_carrier::Killed(GameObject *obj, GameObject *killer)
{
		Console_Input("cmsg public USS Enviseration: Armor's gone, this is it!");
		Console_Input("snda tcw_paul_enviseration_death.mp3");
		GameObject *reply = Commands->Create_Object("Invisible_Object",Vector3(0.0f,0.0f,0.0f));
		Commands->Attach_Script(reply, "tcw_paul_johnson_reply", "");
}
void tcw_USS_Enviseration_carrier::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Get_Health(obj) <= 200.0f)
	{
		if (trigger)
		{
			trigger = false;
			Console_Input("cmsg public USS Enviseration: Side hull breaches! Deck 1 to 14! Damage controls crews are locking them down now.");
			Console_Input("snda tcw_enviseration_hit.mp3");
		}
	}
}
ScriptRegistrant<tcw_USS_Enviseration_carrier> tcw_USS_Enviseration_carrier_Registrant("tcw_USS_Enviseration_carrier","");

/////////////////////////////////////////
//		USS Defiled Script		      //
////////////////////////////////////////
void tcw_USS_Defiled_carrier::Created(GameObject *obj)
{
	trigger = true;
}
void tcw_USS_Defiled_carrier::Killed(GameObject *obj, GameObject *killer)
{
		Console_Input("cmsg public USS Enviseration: We need help out here! Do you understand?");
		Console_Input("snda tcw_paul_defiled_death.mp3");
		GameObject *reply = Commands->Create_Object("Invisible_Object",Vector3(0.0f,0.0f,0.0f));
		GameObject *reply2 = Commands->Create_Object("Invisible_Object",Vector3(0.0f,0.0f,0.0f));
		Commands->Attach_Script(reply, "tcw_paul_johnson_reply2", "");
		Commands->Attach_Script(reply2, "tcw_paul_cyno", "");
}
void tcw_USS_Defiled_carrier::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Get_Health(obj) <= 500.0f)
	{
		if (trigger)
		{
			trigger = false;
			Console_Input("cmsg public USS Defiled: We're hit! Damage report, get me a sitrep!");
			Console_Input("snda tcw_defiled_hit.mp3");
		}
	}
}

ScriptRegistrant<tcw_USS_Defiled_carrier> tcw_USS_Defiled_carrier_Registrant("tcw_USS_Defiled_carrier","");

/////////////////////////////////////////
//		USS Paul Johnson Script		   //
////////////////////////////////////////
void tcw_paul_carrier::Created(GameObject *obj)
{
	trigger = true;
}
void tcw_paul_carrier::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Get_Health(obj) <= 120.0f)
	{
		if (trigger)
		{
			trigger = false;
			Commands->Set_Animation(obj,"CARRIER.CARRIER",false,0,1,1000,false);
			Console_Input("cmsg public USS Paul Johnson: We're hit, We're hit! The USS Paul Johnson is going down!");
			Console_Input("snda tcw_paul_destroyed.mp3");
			Console_Input("snda tessa_paul_hit.mp3");
			Set_Skin(obj, "Blamo");
			Vector3 exp1;
			Vector3 exp2;
			Vector3 exp3;
			Vector3 exp4;
			exp1 = Commands->Get_Bone_Position(obj, "HullExp1");
			exp2 = Commands->Get_Bone_Position(obj, "HullExp02");
			exp3 = Commands->Get_Bone_Position(obj, "HullExp03");
			exp4 = Commands->Get_Bone_Position(obj, "CockExp");
			exp1.Z += 1.5;
			exp2.Z += 1.5;
			exp3.Z += 1.5;
			exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp1, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp2, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp3, obj);
			exp1.Z += 1.5;
			exp2.Z += 1.5;
			exp3.Z += 1.5;
		    exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp1, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp2, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp3, obj);
			exp1.Z += 1.5;
			exp2.Z += 1.5;
			exp3.Z += 1.5;
			exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp1, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp2, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp3, obj);
			exp1.Z += 1.5;
			exp2.Z += 1.5;
			exp3.Z += 1.5;
			exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp1, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp2, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp3, obj);
			exp1.Z += 1.5;
			exp2.Z += 1.5;
			exp3.Z += 1.5;
			exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp1, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp2, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp3, obj);
			exp1.Z += 1.5;
			exp2.Z += 1.5;
			exp3.Z += 1.5;
			exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp1, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp2, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp3, obj);
			exp1.Z += 1.5;
			exp2.Z += 1.5;
			exp3.Z += 1.5;
			exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp1, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp2, obj);
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp3, obj);
			exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			exp4.Z += 1.5;
			Commands->Create_Explosion("Explosion_Rocket_SSM", exp4, obj);
			Commands->Destroy_Object(Commands->Find_Object(Get_Int_Parameter("Turret1")));
			Commands->Destroy_Object(Commands->Find_Object(Get_Int_Parameter("Turret2")));
			GameObject *reply = Commands->Create_Object("Invisible_Object",Vector3(0.0f,0.0f,0.0f));
		   Commands->Attach_Script(reply, "tcw_paul_sinking", "");
		}
	}
}



ScriptRegistrant<tcw_paul_carrier> tcw_paul_carrier_Registrant("tcw_paul_carrier","Turret1=0:int,Turret2=0:int");

/////////////////////////////////////////
//		USS Paul Johnson Reply Script  //
////////////////////////////////////////
void tcw_paul_johnson_reply::Created(GameObject *obj) {
	Commands->Start_Timer(obj,this,7.0f,1);
}
void tcw_paul_johnson_reply::Timer_Expired(GameObject *obj,int number) {
	if (number == 1) {
		Console_Input("cmsg nod USS Paul Johnson: Yes! Well done. Focus fire on the second carrier.");
		Destroy_Script();
	}
}

ScriptRegistrant<tcw_paul_johnson_reply> tcw_paul_johnson_reply_Registrant("tcw_paul_johnson_reply","");

/////////////////////////////////////////
//	USS Paul Johnson Reply Script 2   //
////////////////////////////////////////
void tcw_paul_johnson_reply2::Created(GameObject *obj) {
	Commands->Start_Timer(obj,this,5.0f,1);
}
void tcw_paul_johnson_reply2::Timer_Expired(GameObject *obj,int number) {
	if (number == 1) {
		Console_Input("cmsg nod USS Paul Johnson: Yes! Well done. Focus fire on the second carrier.");
		Console_Input("snda tcw_paul_second.mp3");
		Destroy_Script();
	}
}

ScriptRegistrant<tcw_paul_johnson_reply2> tcw_paul_johnson_reply2_Registrant("tcw_paul_johnson_reply2","");


/////////////////////////////////////////
//USS Paul Johnson  Sinking song      //
////////////////////////////////////////
void tcw_paul_sinking::Created(GameObject *obj) {
	Commands->Start_Timer(obj,this,4.5f,1);
}
void tcw_paul_sinking::Timer_Expired(GameObject *obj,int number) {
	if (number == 1) {
		Console_Input("snda tcw_paul_destroyed.mp3");
		Destroy_Script();
	}
}

ScriptRegistrant<tcw_paul_sinking> tcw_paul_sinking_Registrant("tcw_paul_sinking","");



/////////////////////////////////////////
//     Orca Purchase Terminal         //
////////////////////////////////////////
void tcw_paul_buy_orca::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	allowpoke = true;
	orca_buyer = 0;
	orca_cost = 1100.0f;
	orca_max = 12;
	orca_spawn = Get_Int_Parameter("Orca_Spawn_ID");
}
void tcw_paul_buy_orca::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (Get_Object_Count(2,"TCW_Paul_Orca") < 12)
		{
			if (Commands->Get_Money(poker) >= orca_cost)
			{
				orca_buyer = Commands->Get_ID(poker);
				Commands->Start_Timer(obj,this,1.0f,102);//create orca after success but disallow poking for 15 seconds in next line of code to prevent ppl buying orca's that get stuck in each other
				allowpoke = false; Commands->Enable_HUD_Pokable_Indicator(obj, false) ;Commands->Start_Timer(obj,this,15.0f,100);//15 seconds to get in the orca and fly away
				Commands->Give_Money(poker,-orca_cost,0);//take money :D
				char buymsg[250]; sprintf(buymsg,"%s purchased a GDI Orca.",Get_Player_Name(Commands->Find_Object(orca_buyer))); Send_Message(255,255,255,buymsg);
			}
			else
			{
				allowpoke = false; Commands->Enable_HUD_Pokable_Indicator(obj, false); Commands->Start_Timer(obj,this,5.0f,101);//5 second delay for pressing e again so you cant spam-press-e on it
			}
		}
		else if (Get_Object_Count(2,"TCW_Paul_Orca") >= 12)
		{
			Send_Message_Player(poker,255,255,255,"Sorry, the Orca limit of 12 has been reached. Try again later.");
		}
	}
}
void tcw_paul_buy_orca::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)//succesful purchase enable poking
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
	else if (number == 101)//unsuccesful purchase
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj, true);
	}
	else if (number == 102)//succesful purchase create orca
	{
		Vector3 orca_spawnpos = Commands->Get_Position(Commands->Find_Object(orca_spawn));
		GameObject *orca = Commands->Create_Object("TCW_Paul_Orca",orca_spawnpos);
		Attach_Script_Once(orca,"tcw_vehexit_killtimer","");//put script that destroys orca when it is exitted so players dont fuckup the game by leaving empty orcas everywhere.
	}
}
void tcw_paul_buy_orca::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allowpoke,sizeof(allowpoke),1);
	Auto_Save_Variable(&orca_buyer,sizeof(orca_buyer),2);
	Auto_Save_Variable(&orca_cost,sizeof(orca_cost),3);
	Auto_Save_Variable(&orca_spawn,sizeof(orca_spawn),4);
}
ScriptRegistrant<tcw_paul_buy_orca> tcw_paul_buy_orca_Registrant("tcw_paul_buy_orca","Orca_Spawn_ID=123:int");


/////////////////////////////////////////
//       USS Defiled death 2          //
////////////////////////////////////////
void tcw_paul_cyno::Created(GameObject *obj) {
	Commands->Start_Timer(obj,this,2.5f,1);
}
void tcw_paul_cyno::Timer_Expired(GameObject *obj,int number) {
	if (number == 1) {
		Console_Input("cmsg public USS Defiled: CYNOOOOooo... *static*");
		Destroy_Script();
	}
}

ScriptRegistrant<tcw_paul_cyno> tcw_paul_cyno_Registrant("tcw_paul_cyno","");

/////////////////////////////////////////
//       Gameover                      //
////////////////////////////////////////
void tcw_paul_gameover::Created(GameObject *obj) {
	Commands->Start_Timer(obj,this,67.0f,1);
}
void tcw_paul_gameover::Timer_Expired(GameObject *obj,int number) {
	if (number == 1) {
		Console_Input("amsg USS Paul Johson destroyed, Good job GDI! Command is proud of you! * Misson Acomplished *");
		Console_Input("win 1");
	}
}

ScriptRegistrant<tcw_paul_gameover> tcw_paul_gameover_Registrant("tcw_paul_gameover","");