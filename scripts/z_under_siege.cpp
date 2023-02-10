/*	Renegade Scripts.dll
	Copyright 2012-2013 http://www.UltraAOW.COM by zunnie
	For the map C&C_Under_Siege

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
#include "BuildingGameObj.h"
#include "z_under_siege.h"
#ifdef SSGM
#include "gmgame.h"
#endif

void z_us_building::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	Commands->Set_Health(obj,Commands->Get_Max_Health(obj)); 
	Set_Damage_Points(obj,0.0f);
}

void z_us_core_nod::Created(GameObject *obj)
{
	play_under_attack = true;
	Commands->Set_Player_Type(obj,0);
	center_node = Find_Object_With_Script("z_us_center_node");
	play_heal_core = true;
	play_destroyed = true;
	play_heavily_damaged = true;
	play_critical_damaged = true;
	Commands->Set_Animation(obj,"DOM_NODE_H.DOM_NODE_H",false,0,0,0,false);
}
void z_us_core_nod::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Get_Player_Type(center_node) == 1)
	{
		if (damage < 0.0f)
		{
			damage = damage + (damage - (damage));
			float curhp = Commands->Get_Health(obj);
			Commands->Set_Health(obj,curhp+damage);
			if (play_heal_core)
			{
				if (Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
				{
					play_heal_core = false; Commands->Start_Timer(obj,this,15.0f,500);
					Create_2D_WAV_Sound_Team("ut3_dom_you_cannot_heal_the_core.mp3",0);
					Send_Message_Team(0,255,0,0,"You cannot heal the Core!");
				}
			}
		}
		else if (damage > Commands->Get_Health(obj))
		{
			 Commands->Set_Health(obj,0.1f);
			 if (play_destroyed)
			 {
				play_destroyed = false;
				Create_2D_WAV_Sound_Team("ut3_dom_core_destroyed.mp3",0);
				Create_2D_WAV_Sound_Team("ut3_dom_enemy_core_destroyed.mp3",1);
				Send_Message_Team(0,255,0,0,"Core destroyed!");
				Send_Message_Team(1,255,0,0,"Enemy Core destroyed!");
				Console_Input("amsg GDI wins the game by destroying the enemy Core!");
				Console_Input("win 1");
			 }
		}
		else if (damage > 0.0f)
		{
			 if (play_heavily_damaged)
			 {
				 if (Commands->Get_Health(obj) > 240 && Commands->Get_Health(obj) < 260)
				 {
					 play_heavily_damaged = false;
					 Create_2D_WAV_Sound_Team("ut3_dom_core_is_heavily_damaged.mp3",0);
					 Create_2D_WAV_Sound_Team("ut3_dom_enemy_core_is_heavily_damaged.mp3",1);
					 Send_Message_Team(0,255,0,0,"Core is heavily damaged!");
					 Send_Message_Team(1,255,204,0,"Enemy Core is heavily damaged!");
				 }
			 }
			 if (play_critical_damaged)
			 {
				 if (Commands->Get_Health(obj) > 40 && Commands->Get_Health(obj) < 60)
				 {
					 play_critical_damaged = false;
					 Create_2D_WAV_Sound_Team("ut3_dom_core_is_critical.mp3",0);
					 Create_2D_WAV_Sound_Team("ut3_dom_enemy_core_is_critical.mp3",1);
					 Send_Message_Team(0,255,0,0,"Core is critical!");
					 Send_Message_Team(1,255,204,0,"Enemy Core is critical!");
				 }
			 }
			 if (play_under_attack)
			 {
				play_under_attack = false; Commands->Start_Timer(obj,this,20.0f,501);
				Create_2D_WAV_Sound_Team("ut3_dom_core_is_under_attack.mp3",0);
				Create_2D_WAV_Sound_Team("ut3_dom_enemy_core_is_under_attack.mp3",1);
				Send_Message_Team(0,255,0,0,"Core is under attack!");
				Send_Message_Team(1,255,204,0,"Enemy Core is under attack!");
			 }
		}
	}
	else
	{
		if (damage < 0.0f)
		{
			damage = damage + (damage - (damage));
			float curhp = Commands->Get_Health(obj);
			Commands->Set_Health(obj,curhp+damage);
			if (play_heal_core)
			{
				play_heal_core = false; Commands->Start_Timer(obj,this,15.0f,500);
				Create_2D_WAV_Sound_Team("ut3_dom_you_cannot_heal_the_core.mp3",0);
				Send_Message_Team(0,255,0,0,"You cannot heal the Core!");
			}
		}
		else
		{
			Commands->Set_Health(obj,Commands->Get_Health(obj) + damage);
		}
	}
}
void z_us_core_nod::Timer_Expired(GameObject *obj, int number)
{
	if (number == 500)
	{
		play_heal_core = true;
	}
	else if (number == 501)
	{
		play_under_attack = true;
	}
}

void z_us_core_gdi::Created(GameObject *obj)
{
	play_under_attack = true;
	Commands->Set_Player_Type(obj,1);
	center_node = Find_Object_With_Script("z_us_center_node");
	play_heal_core = true;
	play_destroyed = true;
	play_heavily_damaged = true;
	play_critical_damaged = true;
	Commands->Set_Animation(obj,"DOM_NODE_H.DOM_NODE_H",false,0,1,1,false);
}
void z_us_core_gdi::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Get_Player_Type(center_node) == 0)
	{
		if (damage < 0.0f)
		{
			damage = damage + (damage - (damage));
			float curhp = Commands->Get_Health(obj);
			Commands->Set_Health(obj,curhp+damage);
			if (play_heal_core)
			{
				if (Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
				{
					play_heal_core = false; Commands->Start_Timer(obj,this,15.0f,500);
					Create_2D_WAV_Sound_Team("ut3_dom_you_cannot_heal_the_core.mp3",1);
					Send_Message_Team(1,255,204,0,"You cannot heal the Core!");
				}
			}
		}
		else if (damage > Commands->Get_Health(obj))
		{
			 Commands->Set_Health(obj,0.1f);
			 if (play_destroyed)
			 {
				play_destroyed = false;
				Create_2D_WAV_Sound_Team("ut3_dom_core_destroyed.mp3",1);
				Create_2D_WAV_Sound_Team("ut3_dom_enemy_core_destroyed.mp3",0);
				Send_Message_Team(1,255,204,0,"Core destroyed!");
				Send_Message_Team(0,255,0,0,"Enemy Core destroyed!");
				Console_Input("amsg Nod wins the game by destroying the enemy Core!");
				Console_Input("win 0");
			 }
		}
		else if (damage > 0.0f)
		{
			 if (play_heavily_damaged)
			 {
				 if (Commands->Get_Health(obj) > 240 && Commands->Get_Health(obj) < 260)
				 {
					 play_heavily_damaged = false;
					 Create_2D_WAV_Sound_Team("ut3_dom_core_is_heavily_damaged.mp3",1);
					 Create_2D_WAV_Sound_Team("ut3_dom_enemy_core_is_heavily_damaged.mp3",0);
					 Send_Message_Team(1,255,204,0,"Core is heavily damaged!");
					 Send_Message_Team(0,255,0,0,"Enemy Core is heavily damaged!");
				 }
			 }
			 if (play_critical_damaged)
			 {
				 if (Commands->Get_Health(obj) > 40 && Commands->Get_Health(obj) < 60)
				 {
					 play_critical_damaged = false;
					 Create_2D_WAV_Sound_Team("ut3_dom_core_is_critical.mp3",1);
					 Create_2D_WAV_Sound_Team("ut3_dom_enemy_core_is_critical.mp3",0);
					 Send_Message_Team(1,255,204,0,"Core is critical!");
					 Send_Message_Team(0,255,0,0,"Enemy Core is critical!");
				 }
			 }
			 if (play_under_attack)
			 {
				play_under_attack = false; Commands->Start_Timer(obj,this,20.0f,501);
				Create_2D_WAV_Sound_Team("ut3_dom_core_is_under_attack.mp3",1);
				Create_2D_WAV_Sound_Team("ut3_dom_enemy_core_is_under_attack.mp3",0);
				Send_Message_Team(1,255,204,0,"Core is under attack!");
				Send_Message_Team(0,255,0,0,"Enemy Core is under attack!");
			 }
		}
	}
	else
	{
		if (damage < 0.0f)
		{
			damage = damage + (damage - (damage));
			float curhp = Commands->Get_Health(obj);
			Commands->Set_Health(obj,curhp+damage);
			if (play_heal_core)
			{
				play_heal_core = false; Commands->Start_Timer(obj,this,15.0f,500);
				Create_2D_WAV_Sound_Team("ut3_dom_you_cannot_heal_the_core.mp3",1);
				Send_Message_Team(1,255,204,0,"You cannot heal the Core!");
			}
		}
		else
		{
			Commands->Set_Health(obj,Commands->Get_Health(obj) + damage);
		}
	}
}
void z_us_core_gdi::Timer_Expired(GameObject *obj, int number)
{
	if (number == 500)
	{
		play_heal_core = true;
	}
	else if (number == 501)
	{
		play_under_attack = true;
	}
}

void z_us_center_node::Created(GameObject *obj)
{
	play_defend_node = true;
	Commands->Set_Player_Type(obj,-2);
	Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);
}
void z_us_center_node::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	int team = Commands->Get_Player_Type(damager);

	if ( (strcmp(Get_Current_Weapon(damager),"weapon_orb") == 0) && Commands->Get_Player_Type(obj) != Commands->Get_Player_Type(damager) )
	{
			if (Commands->Get_Player_Type(obj) == 0)
			{
				Create_2D_WAV_Sound_Team("ut3_dom_core_is_secure.mp3",1);
				Send_Message_Team(1,255,204,0,"Core is secure!");
			}
			else if (Commands->Get_Player_Type(obj) == 1)
			{
				Create_2D_WAV_Sound_Team("ut3_dom_core_is_secure.mp3",0);
				Send_Message_Team(0,255,0,0,"Core is secure!");
			}
			Commands->Set_Health(obj,300.0f);
			Commands->Set_Player_Type(obj,Commands->Get_Player_Type(damager));
			if (Commands->Get_Player_Type(damager) == 0)
			{
				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,0,0,false);
				Commands->Start_Timer(obj,this,2.0f,200);
			}
			else if (Commands->Get_Player_Type(damager) == 1)
			{
				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,1,1,false);
				Commands->Start_Timer(obj,this,2.0f,300);
			}
	}
	else if (damage < 0.0f)
	{
		if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj) && Commands->Get_Player_Type(obj) == -2)
		{
			Commands->Set_Player_Type(obj,Commands->Get_Player_Type(damager));
			if (team == 0)
			{
				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,0,0,false);
				Commands->Start_Timer(obj,this,2.0f,200);
			}
			else if (team == 1)
			{
				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,1,1,false);
				Commands->Start_Timer(obj,this,2.0f,300);
			}
		}
	}

	else if (damage > Commands->Get_Health(obj))
	{
		if (Commands->Get_Player_Type(obj) == 0)
		{
			Create_2D_WAV_Sound_Team("ut3_dom_core_is_secure.mp3",1);
			Send_Message_Team(1,255,204,0,"Core is secure!");
		}
		else if (Commands->Get_Player_Type(obj) == 1)
		{
			Create_2D_WAV_Sound_Team("ut3_dom_core_is_secure.mp3",0);
			Send_Message_Team(0,255,0,0,"Core is secure!");
		}
		Commands->Set_Health(obj,0.1f);
		Commands->Set_Player_Type(obj,-2);
		Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);
	}

	if (damage > 0.0f)
	{
		if (Commands->Get_Player_Type(obj) != -2)
		{
			if (play_defend_node && Commands->Get_Player_Type(damager) != Commands->Get_Player_Type(obj))
			{
				play_defend_node = false; Commands->Start_Timer(obj,this,20.0f,400);
				if (Commands->Get_Player_Type(obj) == 0)
				{
					Create_2D_WAV_Sound_Team("ut3_dom_defend_the_center_node.mp3",0);
					Send_Message_Team(0,255,0,0,"Defend the Center Node!");
					Create_2D_WAV_Sound_Team("ut3_dom_attack_the_center_node.mp3",1);
					Send_Message_Team(1,255,204,0,"Attack the Center Node!");
				}
				else if (Commands->Get_Player_Type(obj) == 1)
				{
					Create_2D_WAV_Sound_Team("ut3_dom_defend_the_center_node.mp3",1);
					Send_Message_Team(1,255,204,0,"Defend the Center Node!");
					Create_2D_WAV_Sound_Team("ut3_dom_attack_the_center_node.mp3",0);
					Send_Message_Team(0,255,0,0,"Attack the Center Node!");
				}
			}
		}
	}
}
void z_us_center_node::Timer_Expired(GameObject *obj, int number)
{
	if (number == 200)
	{
		if (Commands->Get_Player_Type(obj) == 0)
		{
			Create_2D_WAV_Sound_Team("ut3_dom_attack_the_center_node.mp3",1);
			Create_2D_WAV_Sound_Team("ut3_dom_enemy_core_is_vulnerable.mp3",0);
			Send_Message_Team(1,255,204,0,"Attack the Center Node!");
			Send_Message_Team(0,255,0,0,"Enemy Core is vulnerable!");
			Commands->Start_Timer(obj,this,2.0f,201);
		}
	}
	else if (number == 201)
	{
		if (Commands->Get_Player_Type(obj) == 0)
		{
			Create_2D_WAV_Sound_Team("ut3_dom_core_is_vulnerable.mp3",1);
			Create_2D_WAV_Sound_Team("ut3_dom_attack_the_enemy_core.mp3",0);
			Send_Message_Team(0,255,0,0,"Attack the enemy Core!");
			Send_Message_Team(1,255,204,0,"Core is vulnerable!");
			Commands->Start_Timer(obj,this,2.0f,202);
		}
	}
	else if (number == 202)
	{
		if (Commands->Get_Player_Type(obj) == 0)
		{
			Create_2D_WAV_Sound_Team("ut3_dom_defend_your_core.mp3",1);
			Send_Message_Team(1,255,204,0,"Defend your Core!");
		}
	}
	
	else if (number == 300)
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			Create_2D_WAV_Sound_Team("ut3_dom_attack_the_center_node.mp3",0);
			Create_2D_WAV_Sound_Team("ut3_dom_enemy_core_is_vulnerable.mp3",1);
			Send_Message_Team(0,255,0,0,"Attack the Center Node!");
			Send_Message_Team(1,255,204,0,"Enemy Core is vulnerable!");
			Commands->Start_Timer(obj,this,2.0f,301);
		}
	}
	else if (number == 301)
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			Create_2D_WAV_Sound_Team("ut3_dom_core_is_vulnerable.mp3",0);
			Send_Message_Team(0,255,0,0,"Core is vulnerable!");
			Create_2D_WAV_Sound_Team("ut3_dom_attack_the_enemy_core.mp3",1);
			Send_Message_Team(1,255,204,0,"Attack the enemy Core!");
			Commands->Start_Timer(obj,this,2.0f,302);
		}
	}
	else if (number == 302)
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			Create_2D_WAV_Sound_Team("ut3_dom_defend_your_core.mp3",0);
			Send_Message_Team(0,255,0,0,"Defend your Core!");
		}
	}

	else if (number == 400)
	{
		play_defend_node = true;
	}
}

void z_us_destroy_orb::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,100);
}
void z_us_destroy_orb::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		Commands->Destroy_Object(obj);
	}
}

void z_us_orb_carrier_nod::Created(GameObject *obj)
{
	GameObject *icon = Commands->Create_Object_At_Bone(obj,"Invisible_Object","c head");
	Vector3 v = Commands->Get_Position(icon);
	v.Z += 1.5f;
	Commands->Set_Position(icon,v);
	orb_id = Commands->Get_ID(icon);
	Commands->Set_Model(icon,"p_keycrd_red");
	Commands->Attach_To_Object_Bone(icon,obj,"c head");
	Commands->Start_Timer(obj,this,2.0f,437634);
	Commands->Give_PowerUp(obj,"POW_Orb",false);
}
void z_us_orb_carrier_nod::Timer_Expired(GameObject *obj, int number)
{
	if (number == 437634)
	{
		if (Get_Bullets(obj,"weapon_orb") < 1)
		{
			Attach_Script_Once(Commands->Find_Object(orb_id),"z_us_destroy_orb","");
			Destroy_Script();
		}
		else
		{
			Commands->Start_Timer(obj,this,2.0f,437634);
		}
	}
}
void z_us_orb_carrier_nod::Destroyed(GameObject *obj)
{
	Attach_Script_Once(Commands->Find_Object(orb_id),"z_us_destroy_orb","");
	Destroy_Script();
}
void z_us_orb_carrier_nod::Detach(GameObject *obj)
{
	ScriptImpClass::Detach(obj);
	Commands->Destroy_Object(Commands->Find_Object(orb_id));
}

void z_us_orb_carrier_gdi::Created(GameObject *obj)
{
	GameObject *icon = Commands->Create_Object_At_Bone(obj,"Invisible_Object","c head");
	Vector3 v = Commands->Get_Position(icon);
	v.Z += 1.5f;
	Commands->Set_Position(icon,v);
	orb_id = Commands->Get_ID(icon);
	Commands->Set_Model(icon,"p_keycrd_yel");
	Commands->Attach_To_Object_Bone(icon,obj,"c head");
	Commands->Start_Timer(obj,this,2.0f,437634);
	Commands->Give_PowerUp(obj,"POW_Orb",true);
}
void z_us_orb_carrier_gdi::Timer_Expired(GameObject *obj, int number)
{
	if (number == 437634)
	{
		if (Get_Bullets(obj,"weapon_orb") < 1)
		{
			Attach_Script_Once(Commands->Find_Object(orb_id),"z_us_destroy_orb","");
			Destroy_Script();
		}
		else
		{
			Commands->Start_Timer(obj,this,2.0f,437634);
		}
	}
}
void z_us_orb_carrier_gdi::Destroyed(GameObject *obj)
{
	Attach_Script_Once(Commands->Find_Object(orb_id),"z_us_destroy_orb","");
	Destroy_Script();
}
void z_us_orb_carrier_gdi::Detach(GameObject *obj)
{
	ScriptImpClass::Detach(obj);
	Commands->Destroy_Object(Commands->Find_Object(orb_id));
}

void z_us_get_orb::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	allowpoke = true;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void z_us_get_orb::Poked(GameObject *obj, GameObject *poker)
{
	if (allowpoke)
	{
		if (Commands->Get_Player_Type(poker) == team)
		{
			if (!Is_Stealth_Enabled(poker))
			{
				if (team == 0)
				{
					if (!Find_Object_With_Script("z_us_orb_carrier_nod"))
					{
						Attach_Script_Once(poker,"z_us_orb_carrier_nod","");
						char orbmsg[512];
						sprintf(orbmsg,"Orb picked up by %s!",Get_Player_Name(poker));
						Send_Message_Team(0,255,0,0,orbmsg);
						Create_2D_WAV_Sound_Team("ut3_dom_orb_picked_up.mp3",0);
						Create_2D_WAV_Sound_Team("ut3_dom_kill_the_enemy_orb_carrier.mp3",1);
						char enemyorb[512]; sprintf(enemyorb,"Kill the enemy Orb carrier: %s",Get_Player_Name(poker));
						Send_Message_Team(1,255,204,0,enemyorb);
					}
					else
					{
						char orbinuse[512];
						sprintf(orbinuse,"%s already has the Orb!",Get_Player_Name(Find_Object_With_Script("z_us_orb_carrier_nod")));
						Send_Message_Player(poker,255,0,0,orbinuse);
					}
				}
				else if (team == 1)
				{
					if (!Find_Object_With_Script("z_us_orb_carrier_gdi"))
					{
						Attach_Script_Once(poker,"z_us_orb_carrier_gdi","");
						char orbmsg[512];
						sprintf(orbmsg,"Orb picked up by %s!",Get_Player_Name(poker));
						Send_Message_Team(1,255,204,0,orbmsg);
						Create_2D_WAV_Sound_Team("ut3_dom_orb_picked_up.mp3",1);
						Create_2D_WAV_Sound_Team("ut3_dom_kill_the_enemy_orb_carrier.mp3",0);
						char enemyorb[512]; sprintf(enemyorb,"Kill the enemy Orb carrier: %s",Get_Player_Name(poker));
						Send_Message_Team(0,255,0,0,enemyorb);
					}
					else
					{
						char orbinuse[512];
						sprintf(orbinuse,"%s already has the Orb!",Get_Player_Name(Find_Object_With_Script("z_us_orb_carrier_gdi")));
						Send_Message_Player(poker,255,204,0,orbinuse);
					}
				}
				allowpoke = false; Commands->Start_Timer(obj,this,5.0f,100);
				Commands->Enable_HUD_Pokable_Indicator(obj,false);
			}
		}
	}
}
void z_us_get_orb::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		allowpoke = true;
		Commands->Enable_HUD_Pokable_Indicator(obj,true);
	}
}

void z_us_enemy_orb_incoming_zone::Created(GameObject *obj)
{
	trigger = true;
	team = Get_Int_Parameter("Team");
	center_node = Find_Object_With_Script("z_us_center_node");
}
void z_us_enemy_orb_incoming_zone::Entered(GameObject *obj, GameObject *enter)
{
	if (trigger)
	{
		if (team == 0)
		{
			if (Commands->Get_Player_Type(center_node) == 1)
			{
				if (Is_Script_Attached(enter,"z_us_orb_carrier_nod"))
				{
					char enemyorb[512]; sprintf(enemyorb,"Enemy Orb carrier incoming: %s",Get_Player_Name(enter));
					Send_Message_Team(1,255,204,0,enemyorb);
					Create_2D_WAV_Sound_Team("ut3_dom_enemy_orb_carrier_incoming.mp3",1);

					char friendorb[512]; sprintf(friendorb,"Orb carrier incoming: %s",Get_Player_Name(enter));
					Send_Message_Team(0,255,0,0,friendorb);
					Create_2D_WAV_Sound_Team("ut3_dom_orb_carrier_incoming.mp3",0);
				}

				else if (Is_Script_Attached(enter,"z_us_orb_carrier_gdi"))
				{
					char friendorb[512]; sprintf(friendorb,"Orb carrier incoming: %s",Get_Player_Name(enter));
					Send_Message_Team(1,255,204,0,friendorb);
					Create_2D_WAV_Sound_Team("ut3_dom_orb_carrier_incoming.mp3",1);
				}
			}
		}
		if (team == 1)
		{
			if (Commands->Get_Player_Type(center_node) == 0)
			{
				if (Is_Script_Attached(enter,"z_us_orb_carrier_gdi"))
				{
					char enemyorb[512]; sprintf(enemyorb,"Enemy Orb carrier incoming: %s",Get_Player_Name(enter));
					Send_Message_Team(0,255,0,0,enemyorb);
					Create_2D_WAV_Sound_Team("ut3_dom_enemy_orb_carrier_incoming.mp3",0);

					char friendorb[512]; sprintf(friendorb,"Orb carrier incoming: %s",Get_Player_Name(enter));
					Send_Message_Team(1,255,204,0,friendorb);
					Create_2D_WAV_Sound_Team("ut3_dom_orb_carrier_incoming.mp3",1);
				}

				else if (Is_Script_Attached(enter,"z_us_orb_carrier_nod"))
				{
					char friendorb[512]; sprintf(friendorb,"Orb carrier incoming: %s",Get_Player_Name(enter));
					Send_Message_Team(0,255,0,0,friendorb);
					Create_2D_WAV_Sound_Team("ut3_dom_orb_carrier_incoming.mp3",0);
				}
			}
		}
		trigger = false; Commands->Start_Timer(obj,this,20.0f,100);
	}
}
void z_us_enemy_orb_incoming_zone::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		trigger = true;
	}
}

void z_us_limited_character::Created(GameObject *obj)
{
	int team = Commands->Get_Player_Type(obj);
	int limit = 7;
	int current = 
		Get_Object_Count(team,"GDI_Havoc_1") + Get_Object_Count(team,"GDI_Havoc_2") + Get_Object_Count(team,"GDI_Havoc_3") + Get_Object_Count(team,"GDI_Havoc_4") +
		Get_Object_Count(team,"GDI_Powersuit_1") + Get_Object_Count(team,"GDI_Powersuit_1") + Get_Object_Count(team,"GDI_Logan") +
		Get_Object_Count(team,"Nod_Sakura_1") + Get_Object_Count(team,"Nod_Sakura_1") + Get_Object_Count(team,"Nod_Raveshaw_1") + Get_Object_Count(team,"Nod_Raveshaw_1") + Get_Object_Count(team,"Nod_Sniper");

	if (current >= limit)
	{
		if ( (strcmp(Commands->Get_Preset_Name(obj),"GDI_Logan") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"Nod_Sniper") == 0) )	Commands->Give_Money(obj,500.0f,false);
		else Commands->Give_Money(obj,1000.0f,false);

		Create_2D_WAV_Sound_Player(obj,"accessdenied.mp3");
		Send_Message_Player(obj,255,255,255,"Access Denied: Sniper Character limit of 7 max has been reached.");
		
		if (team == 0) Change_Character(obj,"CnC_Nod_Minigunner_0");
		else Change_Character(obj,"CnC_GDI_MiniGunner_0");
	}
	else Destroy_Script();
}


ScriptRegistrant<z_us_destroy_orb> z_us_destroy_orb_Registrant("z_us_destroy_orb","");
ScriptRegistrant<z_us_limited_character> z_us_limited_character_Registrant("z_us_limited_character","");
ScriptRegistrant<z_us_enemy_orb_incoming_zone> z_us_enemy_orb_incoming_zone_Registrant("z_us_enemy_orb_incoming_zone","Team=0:int");
ScriptRegistrant<z_us_get_orb> z_us_get_orb_Registrant("z_us_get_orb","Team=0:int");
ScriptRegistrant<z_us_orb_carrier_nod> z_us_orb_carrier_nod_Registrant("z_us_orb_carrier_nod","");
ScriptRegistrant<z_us_orb_carrier_gdi> z_us_orb_carrier_gdi_Registrant("z_us_orb_carrier_gdi","");
ScriptRegistrant<z_us_center_node> z_us_center_node_Registrant("z_us_center_node","");
ScriptRegistrant<z_us_core_nod> z_us_core_nod_Registrant("z_us_core_nod","");
ScriptRegistrant<z_us_core_gdi> z_us_core_gdi_Registrant("z_us_core_gdi","");
ScriptRegistrant<z_us_building> z_us_building_Registrant("z_us_building","");