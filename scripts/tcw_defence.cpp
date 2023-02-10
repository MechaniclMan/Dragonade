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
#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "tcw_defence.h"
#include "ScriptableGameObj.h"
#include "DamageableGameObj.h"
#include "WeaponClass.h"
#include "ScriptZoneGameObj.h"
#include "SmartGameObj.h"
#include "VehicleGameObjDef.h"
#define PTTEAM(t) (t?0:1)

void tcw_Base_Defense::Created(GameObject *obj)
{
	targetID = 0;
	attacking = false;
	current_priority = 10;
	attack_timer = 1.0f;
	loop_count = 0;
	Commands->Enable_Enemy_Seen(obj, true);
	Commands->Enable_Vehicle_Transitions(obj, false);
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Start_Timer(obj, this, attack_timer, 11);
}

void tcw_Base_Defense::Killed(GameObject *obj, GameObject *shooter)
{
	//placeholder	
}

void tcw_Base_Defense::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (Is_Base_Powered(Get_Int_Parameter("Team")))
	{
		if (!damager || amount <= 0)
		{
			return;
		}
		if (Get_Vehicle_Mode(damager) == VEHICLE_TYPE_FLYING)
		{
			if (!(Get_Int_Parameter("AntiAir")))
			{
				return; //Ignore the target
			}
		}
		else
		{
			if (!(Get_Int_Parameter("AntiGround")))
			{
				return; //Ignore the target
			}
		}
		if (targetID) //check for current target
		{
			unsigned long newID = Commands->Get_ID(damager);
			if (targetID == newID)
			{
				return; //already firing at target
			}
		}
		int objID = Commands->Get_Player_Type(obj);
		int damagerID = Commands->Get_Player_Type(damager);
		if (objID != damagerID) //Make sure an enemy attacked
		{
			if (Commands->Get_Health(damager))
			{
				Attack(obj, damager);
			}
		}
	}
}

void tcw_Base_Defense::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if (Is_Base_Powered(Get_Int_Parameter("Team")))
	{
		if (Get_Vehicle_Mode(enemy) == VEHICLE_TYPE_FLYING)
		{
			if (!(Get_Int_Parameter("AntiAir")))
			{
				return; //Ignore the target
			}
		}
		else
		{
			if (!(Get_Int_Parameter("AntiGround")))
			{
				return; //Ignore the target
			}
		}
		if (targetID) //check for current target
		{
			unsigned long newID = Commands->Get_ID(enemy);
			if (targetID == newID)
			{
				return; //already firing at target
			}
		}
		if (Commands->Get_Health(enemy))
		{
			Attack(obj,enemy);
		}
	}
}

void tcw_Base_Defense::Timer_Expired(GameObject *obj, int number)
{
	if (number != 11)
	{
		return; //If its not the timer we're interested in, end now
	}
	if (Is_Base_Powered(Get_Int_Parameter("Team")))
	{
		bool call_attack = true;
		if (!attacking)	//skip facing action if attacking
		{
			if (loop_count > 4) //only face new direction every 5th loop
			{
				Vector3 pos = Commands->Get_Position(obj); 
				switch (Commands->Get_Random_Int(0,8))
				{
					case 0:
						pos.X += 10;
						pos.Y -= 10;
						break;
					case 1:
						pos.X -= 10;
						pos.Y -= 10;
						break;
					case 2:
						pos.X += 10;
						pos.Y += 10;
						break;
					case 3:
						pos.X -= 10;
						pos.Y += 10;
						break;
					case 4:
						pos.X += 10;
						break;
					case 5:
						pos.X -= 10;
						break;
					case 6:
						pos.Y += 10;
						break;
					case 7:
						pos.Y -= 10;
						break;
					default: 
						pos.X += 10;
						break;
				}
				ActionParamsStruct params;
				params.Set_Basic(this, 1, 30); //priority low
				params.Set_Face_Location(pos, 4);
				Commands->Action_Face_Location(obj, params);
			}
		}
		else
		{
			//force ceasefire before checking to continue (helps fight client glitches... I hope...)
			Commands->Action_Reset(obj, 100); //I have no idea what this number does...
			attacking = false;
			current_priority = 10;
			GameObject *target = Commands->Find_Object(targetID);
			if (!(target)) //If target invalid, end now
			{
				targetID = 0;
				call_attack = false;
			}
			else
			{
				int TargetType = Commands->Get_Player_Type(target);
				int myType = Commands->Get_Player_Type(obj);
				if (myType == TargetType) //Check old target for team change
				{
					targetID = 0; //old target a friend now
					call_attack = false;
				}
				//Check Range
				Vector3 objPos = Commands->Get_Position(obj);
				Vector3 targetPos = Commands->Get_Position(target);
				float range = Commands->Get_Distance(objPos, targetPos);
				float max = Get_Float_Parameter("MaxRange");
				float min = Get_Float_Parameter("MinRange");
				if (range < min || range > max)
				{
					targetID = 0; //old target out of range
					call_attack = false;
				}
				//Check Settings
				if ( (Get_Vehicle_Mode(target) == VEHICLE_TYPE_FLYING) || (Get_Vehicle_Mode(Get_Vehicle(target)) == VEHICLE_TYPE_FLYING) )
				{
					if (!(Get_Int_Parameter("AntiAir")))
					{
						targetID = 0; //old target not targetable
						call_attack = false;
					}
				}
				else
				{
					if (!(Get_Int_Parameter("AntiGround")))
					{
						targetID = 0; //old target not targetable
						call_attack = false;
					}
				}
			}
			if (call_attack) //if attack and if call_attack was not turned false
			{
				if (Commands->Get_Health(target))
				{
					Attack(obj, target);
				}
			}
		}
		if (loop_count > 4)
		{
			loop_count = 0; //reset counter
		}
		loop_count++;
		Commands->Start_Timer(obj, this, attack_timer, 11);
	}
}

void tcw_Base_Defense::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 100) //Attack completed
	{
		attacking = false;
		targetID = 0;
		current_priority = 10;
	}
}

void tcw_Base_Defense::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&targetID,4,1);
	Auto_Save_Variable(&attacking,4,2);
	Auto_Save_Variable(&current_priority,4,3);
	Auto_Save_Variable(&attack_timer,4,4);
	Auto_Save_Variable(&loop_count,4,5);
}

void tcw_Base_Defense::Attack(GameObject *obj, GameObject *target)
{
	/**************Evaluate Target Threat****************/
	GameObject *FinalTarget = Set_Target(target);
	if (FinalTarget) //Was there a non-NULL return from Set_Target?
	{
		FinalTarget = Select_Target(obj, FinalTarget); //Was Rate_Target
	}
	/****************Execute Final Attack****************/
	if (FinalTarget && Commands->Get_Player_Type(FinalTarget) == PTTEAM(Commands->Get_Player_Type(obj))) //Check if I can still fire
	{
		targetID = Commands->Get_ID(FinalTarget); //remember new target, or re-remember old target
		Vector3 objPos = Commands->Get_Position(obj);
		Vector3 targetPos = Commands->Get_Position(FinalTarget);
		float range = Commands->Get_Distance(objPos, targetPos);
		float max = Get_Float_Parameter("MaxRange");
		float min = Get_Float_Parameter("MinRange");
		if (min < range && range < max && (obj->As_SmartGameObj()->Is_Obj_Visible(FinalTarget->As_PhysicalGameObj())))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,(float)current_priority, 100);
			if (Adjust_For_Infantry(FinalTarget))
			{
				params.Set_Attack(targetPos, range, 0, true);
				params.AttackCheckBlocked = false;
			}
			else
			{
				params.Set_Attack(FinalTarget, range, 0, true);
				params.AttackCheckBlocked = false;
			}
			Commands->Action_Attack(obj, params);
			current_priority++;
			attacking = true;
		}
		else
		{
			attacking = false;
			targetID = 0;
			current_priority = 10;
		}
	}
}

GameObject* tcw_Base_Defense::Select_Target(GameObject *obj, GameObject *target)
{
	//Targets passed into Select_Target have already been
	//checked for "infantry in vehicle" by Set_Target
	if (!(target)) //First see if there is a valid target
	{
		return Commands->Find_Object(targetID); //no need to test an invalid target
	}
	GameObject *old = Commands->Find_Object(targetID);
	if (!(old)) //is there a current target?
	{
		return target; //no need to test an invalid target
	}
	int toggle = Get_Int_Parameter("TargetMode");
	switch(toggle)
	{
	case 0: //Case 0 prioritizes vehicles over infantry
		//if (Is_Soldier(old) && !(Is_Soldier(target)))
		if ((old && old->As_SoldierGameObj()) && (target && !target->As_SoldierGameObj()))
		{
			return target; //new target is a vehicle
		}
		//else if (!(Is_Soldier(old)) && Is_Soldier(target))
		else if (!(old && old->As_SoldierGameObj()) && (target && target->As_SoldierGameObj()))
		{
			return old; //old target is a vehicle
		}
		else
		{
			return Tie_Breaker(obj, target, old); //both targets the same, let tie breaker decide
		}
		break;//just to be safe
	case 1: //Case 1 prioritizes infantry over vehicles
		//if (Is_Soldier(old) && !(Is_Soldier(target)))
		if ((old && old->As_SoldierGameObj()) && !(target && target->As_SoldierGameObj()))
		{
			if (Get_Vehicle(old))// Did the old target get in a vehicle?
			{
				return Tie_Breaker(obj, target, old); //both vehicle
			}
			else
			{
				return old; //old soldier
			}
		}
		else if (!(old && old->As_SoldierGameObj()) && (target && target->As_SoldierGameObj()))
		{
			if (Get_Vehicle(target))// Did the new target get in a vehicle? 
			{
				return Tie_Breaker(obj, target, old); //both vehicle 
			}
			//Needs to pass in the vehicle instead of old?
			else
			{
				return target; //target soldier
			}
		}
		else
		{
			return Tie_Breaker(obj, target, old); //both targets the same, let tie breaker decide
		}
		break; //just to be safe
	default: //Default doesn't prioritize target type
		return Tie_Breaker(obj, target, old); //all people are equal in my gun sights
		break; //yes, being safe still
	}
}

GameObject* tcw_Base_Defense::Set_Target(GameObject *target)
{
	if (target) //confirm non-NULL pointer
	{
		if (Commands->Is_A_Star(target)) //Is it a player?
		{
			GameObject *vehicle = Get_Vehicle(target); 
			if (vehicle) //Are they in a vehicle
			{
				if (Get_Vehicle_Mode(target) == VEHICLE_TYPE_FLYING)
				{
					if (!(Get_Int_Parameter("AntiAir")))
					{
						return 0; //Ignore the target
					}
				}
				else
				{
					if (!(Get_Int_Parameter("AntiGround")))
					{
						return 0; //Ignore the target
					}
				}
				return vehicle; //if so, return vehicle
			}
			else
			{
				if (Get_Vehicle_Mode(target) == VEHICLE_TYPE_FLYING)
				{
					if (!(Get_Int_Parameter("AntiAir")))
					{
						return 0; //Ignore the target
					}
				}
				else
				{
					if (!(Get_Int_Parameter("AntiGround")))
					{
						return 0; //Ignore the target
					}
				}
				return target; //if not, return player
			}
		}
		return target; //nothing left but the target
	}
	return 0;
}

GameObject* tcw_Base_Defense::Tie_Breaker(GameObject *obj, GameObject *new_target, GameObject *old_target)
{
	//This function compiles several methods of breaking ties
	//-lower health gives higher rating
	//-closer distance gives higher rating
	//-being player gives higher rating
	float old_rating = 0;
	float new_rating = 0;
	//Player Evaluation
	if (Commands->Is_A_Star(old_target))
	{
		old_rating += 100; //automatic +100 for being human
	}
	if (Commands->Is_A_Star(new_target))
	{
		new_rating += 100; //automatic +100 for being human
	}
	//Health Evaluation
	GameObject *old = old_target;
	GameObject *target = new_target;
	//Health needs to be tested for vehicle being driven, if it exists
	if (Get_Vehicle(old_target))
	{
		old = Get_Vehicle(old_target);
	}
	if (Get_Vehicle(new_target))
	{
		target = Get_Vehicle(old_target);
	}
	float new_max = Commands->Get_Max_Health(target);
	float old_max = Commands->Get_Max_Health(old);
	float new_percent = 0; //set to minimum
	float old_percent = 0; //set to minimum
	if (new_max > 0) //just to be sure
	{
		new_percent = ((100 * Commands->Get_Health(target)) / (new_max));
	}
	else
	{
		new_percent += 100; //If maximum health is 0, always at 100% health
	}
	if (old_max > 0) //just to be sure
	{
		old_percent = ((100 * Commands->Get_Health(old)) / (old_max));
	}
	else
	{
		old_percent += 100; //If maximum health is 0, always at 100% health
	}
	new_max = Commands->Get_Max_Shield_Strength(target);
	old_max = Commands->Get_Max_Shield_Strength(old);
	if (new_max > 0) //just to be sure
	{
		new_percent += ((100 * Commands->Get_Health(target)) / (new_max)); //Add in shield percent
	}
	else
	{
		new_percent += 100; //If maximum shield is 0, always at 100% shield
	}
	if (old_max > 0) //just to be sure
	{
		old_percent += ((100 * Commands->Get_Health(old)) / (old_max)); //Add in shield percent
	}
	else
	{
		old_percent += 100; //If maximum shield is 0, always at 100% shield
	}
	old_rating += 200 - old_percent;
	new_rating += 200 - new_percent;
	//Range Evaluation
	float max_range = Get_Float_Parameter("MaxRange");
	float min_range = Get_Float_Parameter("MinRange");
	Vector3 obj_pos = Commands->Get_Position(obj);
	Vector3 new_pos = Commands->Get_Position(new_target);
	Vector3 old_pos = Commands->Get_Position(old_target);
	float new_range = Commands->Get_Distance(obj_pos, new_pos);
	float old_range = Commands->Get_Distance(obj_pos, old_pos);
	if (new_range < max_range && new_range > min_range)
	{
		new_rating += max_range - (new_range - min_range); //not sure if this is good
	}
	if (old_range < max_range && old_range > min_range)
	{
		old_rating += max_range - (old_range - min_range);
	}
	//Final Evaluation
	float max_potential = 300 + max_range; //300 = 100 from Player vs AI + 200 from health
	if (old_rating > new_rating)
	{
		return old_target;
	}
	else if (old_rating < new_rating)
	{
		float test = new_rating - old_rating;
		if (test > (max_potential * 0.1)) //new target must be at least 10% greater priority
		{
			return new_target;
		}
		else
		{
			return old_target;
		}
	}
	else
	{
		return old_target; //rating tie, don't change targets
	}
}

bool tcw_Base_Defense::Adjust_For_Infantry(GameObject *target)
{
	if (Get_Int_Parameter("AdjustAim"))
	{
		if (target && target->As_SoldierGameObj())
		{
			return true;
		}
	}
	return false;
}

ScriptRegistrant<tcw_Base_Defense> tcw_Base_Defense_Registrant("tcw_Base_Defense","MinRange=0.0:float,MaxRange=100.0:float,AntiAir=0:int,AntiGround=1:int,AdjustAim=0:int,TargetMode=0:int,Team=0:int");