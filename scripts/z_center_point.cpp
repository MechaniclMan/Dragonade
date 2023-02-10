/*	Renegade Scripts.dll
	Scripts by from - http://www.tiberiumcrystalwar.com
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
#include "z_center_point.h"
#include "GameObjManager.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"

//make building damageable when cruiser attacks
//0=nod 1=gdi 2=neu current animations for spawnpads

void z_cp_cruiser_countdown_point::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,30.0f,300300);//spam explanations
	Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);
	spawnpad1 = Get_Int_Parameter("SpawnPad1ID");
	if (Commands->Find_Object(spawnpad1))
	{
		Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
		Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),-2);
	}
	spawnpad2 = Get_Int_Parameter("SpawnPad2ID");
	if (Commands->Find_Object(spawnpad2))
	{
		Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
		Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),-2);
	}
	nod_bar_turret_id = 100129; nod_ref_turret_id = 100130; gdi_hand_turret_id = 100128; gdi_ref_turret_id = 100127;	
	nodveh1id = 0; gdiveh1id = 0;
	Commands->Set_Player_Type(obj,-2);
	Commands->Set_Health(obj,2.0f);
	create_cruiser_countdown_object = true;
}
void z_cp_cruiser_countdown_point::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (Commands->Is_A_Star(damager))//is it a player?
	{
		if ( (damage < 0.0f) && (Commands->Get_Player_Type(obj) == -2) )//if damage is less than 0 it must being repaired...
		{
			Commands->Give_Money(damager,(-damage)/5,false);
			if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))//if fully repaired
			{
				if (Commands->Get_Player_Type(damager) == 0)
				{
					Commands->Set_Player_Type(obj,0);
					Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,0,0,false);

					if (Commands->Find_Object(spawnpad1))
					{
						Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false);
						Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),0);
					}
					if (Commands->Find_Object(spawnpad2))
					{
						Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
						Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),-2);
					}

					if (create_cruiser_countdown_object)//if the cruisercountdown object may be created
					{
						Send_Message(255,255,255,"Nod has captured the Countdown Control Point. One Minute for Cruiser Attacks.");
						create_cruiser_countdown_object = false;//its being created here so we set it to false so it doesnt get made a 2nd time
						Vector3 countdown_object_pos;
						countdown_object_pos.X = 0.0f; countdown_object_pos.Y = 0.0f; countdown_object_pos.Z = 0.0f;
						GameObject *countdown_object = Commands->Create_Object("Invisible_Object",countdown_object_pos);

						char prms[250];
						sprintf(prms,"%d,%d,%d,%d,%d",0,nod_bar_turret_id,nod_ref_turret_id,gdi_hand_turret_id,gdi_ref_turret_id);
						Attach_Script_Once(countdown_object,"z_cp_cruiser_countdown",prms);

						Create_2D_WAV_Sound_Team("ControlPointCaptured_Nod.wav",0);
						Commands->Give_Money(damager,500.0f,true);

						if (!Commands->Find_Object(nodveh1id))
						{
							if (strcmp(Get_Parameter("NodVeh1"), "0") != 0)
							{
								Vector3 nodveh1pos = Get_Vector3_Parameter("NodVeh1Pos");
								float nodveh1fac = Get_Float_Parameter("NodVeh1Facing");
								GameObject *nodveh1 = Commands->Create_Object(Get_Parameter("NodVeh1"),nodveh1pos);
								Commands->Set_Facing(nodveh1,nodveh1fac);
								nodveh1id = Commands->Get_ID(nodveh1);
								Attach_Script_Once(nodveh1,"z_Locked_Vehicle","0");
							}
						}
					}
				}
				else if (Commands->Get_Player_Type(damager) == 1)
				{
					Commands->Set_Player_Type(obj,1);
					Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,1,1,false);

					if (Commands->Find_Object(spawnpad1))
					{
						Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
						Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),-2);
					}
					if (Commands->Find_Object(spawnpad2))
					{
						Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false);
						Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),1);
					}

					if (create_cruiser_countdown_object)
					{
						Send_Message(255,255,255,"GDI has captured the Countdown Control Point. One Minute for Cruiser Attacks.");
						create_cruiser_countdown_object = false;
						Vector3 countdown_object_pos;
						countdown_object_pos.X = 0.0f; countdown_object_pos.Y = 0.0f; countdown_object_pos.Z = 0.0f;
						GameObject *countdown_object = Commands->Create_Object("Invisible_Object",countdown_object_pos);
						
						char prms[250];
						sprintf(prms,"%d,%d,%d,%d,%d",1,nod_bar_turret_id,nod_ref_turret_id,gdi_hand_turret_id,gdi_ref_turret_id);
						Attach_Script_Once(countdown_object,"z_cp_cruiser_countdown",prms);

						Create_2D_WAV_Sound_Team("ControlPointCaptured_GDI.wav",1);
						Commands->Give_Money(damager,500.0f,true);

						if (!Commands->Find_Object(gdiveh1id))
						{
							if (strcmp(Get_Parameter("GDIVeh1"), "0") != 0)
							{
								Vector3 gdiveh1pos = Get_Vector3_Parameter("GDIVeh1Pos");
								float gdiveh1fac = Get_Float_Parameter("GDIVeh1Facing");
								GameObject *gdiveh1 = Commands->Create_Object(Get_Parameter("GDIVeh1"),gdiveh1pos);
								Commands->Set_Facing(gdiveh1,gdiveh1fac);
								gdiveh1id = Commands->Get_ID(gdiveh1);
								Attach_Script_Once(gdiveh1,"z_Locked_Vehicle","1");
							}
						}
					}
				}
			}
		}
		else if (damage > 0.0f)
		{
			if (Commands->Get_Player_Type(damager) == 1)//if its gdi attacking it
			{
				if (damage >= Commands->Get_Health(obj))//point is destroyed
				{
					if (Commands->Get_Player_Type(obj) == 0)
					{
						GameObject *target1 = Find_Building_By_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY);
						if (target1)
						{
							Commands->Send_Custom_Event(obj,target1,50505,0,0);//allow player damage false
						}
						GameObject *target2 = Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY);
						if (target2)
						{
							Commands->Send_Custom_Event(obj,target2,50505,0,0);//allow player damage false
						}
						Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);
						Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
						Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
						Send_Message(255,255,255,"Cruiser Attacks on GDI halted. Countdown Point destroyed.");
						Vector3 tur1pos = Commands->Get_Position(Commands->Find_Object(nod_bar_turret_id));//bar attacking turret for nod
						Vector3 tur2pos = Commands->Get_Position(Commands->Find_Object(nod_ref_turret_id));//ref attacking turret for nod
						float facingtur1 = Commands->Get_Facing(Commands->Find_Object(nod_bar_turret_id));
						float facingtur2 = Commands->Get_Facing(Commands->Find_Object(nod_ref_turret_id));
						Commands->Destroy_Object(Commands->Find_Object(nod_bar_turret_id));
						Commands->Destroy_Object(Commands->Find_Object(nod_ref_turret_id));
						GameObject *tur1 = Commands->Create_Object("cp_Carrier_Cannon",tur1pos);
						GameObject *tur2 = Commands->Create_Object("cp_Carrier_Cannon",tur2pos);
						Commands->Set_Facing(tur1,facingtur1); 
						Commands->Set_Player_Type(tur1,-2);
						Commands->Set_Facing(tur2,facingtur2); 
						Commands->Set_Player_Type(tur2,-2);
						nod_bar_turret_id = Commands->Get_ID(tur1);//this is the new bar attacking turret for nod
						nod_ref_turret_id = Commands->Get_ID(tur2);//this is the new ref attacking turret for nod
						create_cruiser_countdown_object = true;
						Create_2D_WAV_Sound_Team("ControlPointLost_Nod.wav",0);
						//destroy the spawned vehicles
						if ( (Commands->Find_Object(nodveh1id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh1id))) )
						{
							Commands->Apply_Damage(Commands->Find_Object(nodveh1id),999999.0f,"Death",obj);		nodveh1id = 0;
						}
						Commands->Set_Player_Type(obj,-2);//it now belongs to gdi because they fully destroyed it
						Commands->Set_Health(obj,2.0f);//we cant allow them to REALLY destroy it, otherwise the script halts and stuff....
					}
					else if (Commands->Get_Player_Type(obj) == -2)
					{
						Commands->Set_Health(obj,2.0f);//we cant allow them to REALLY destroy it, otherwise the script halts and stuff....
					}
				}
			}
			else if (Commands->Get_Player_Type(damager) == 0)//if its nod attacking it
			{
				if (damage >= Commands->Get_Health(obj))//point is destroyed
				{
					if (Commands->Get_Player_Type(obj) == 1)
					{
						GameObject *target1 = Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY);
						if (target1)
						{
							Commands->Send_Custom_Event(obj,target1,50505,0,0);//allow player damage false
						}
						GameObject *target2 = Find_Building_By_Type(0,BuildingConstants::TYPE_REFINERY);
						if (target2)
						{
							Commands->Send_Custom_Event(obj,target2,50505,0,0);//allow player damage false
						}
						Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);
						Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
						Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
						Send_Message(255,255,255,"Cruiser Attacks on Nod halted. Countdown Point destroyed.");
						Vector3 tur1pos = Commands->Get_Position(Commands->Find_Object(gdi_hand_turret_id));//hon attacking turret for gdi
						Vector3 tur2pos = Commands->Get_Position(Commands->Find_Object(gdi_ref_turret_id));//ref attacking turret for gdi
						float facingtur1 = Commands->Get_Facing(Commands->Find_Object(gdi_hand_turret_id));
						float facingtur2 = Commands->Get_Facing(Commands->Find_Object(gdi_ref_turret_id));	
						Commands->Destroy_Object(Commands->Find_Object(gdi_hand_turret_id));
						Commands->Destroy_Object(Commands->Find_Object(gdi_ref_turret_id));
						GameObject *tur1 = Commands->Create_Object("cp_Carrier_Cannon",tur1pos);
						GameObject *tur2 = Commands->Create_Object("cp_Carrier_Cannon",tur2pos);
						Commands->Set_Facing(tur1,facingtur1); 
						Commands->Set_Player_Type(tur1,-2);
						Commands->Set_Facing(tur2,facingtur2); 
						Commands->Set_Player_Type(tur2,-2);
						gdi_hand_turret_id = Commands->Get_ID(tur1);//this is the new hand attacking turret for gdi
						gdi_ref_turret_id = Commands->Get_ID(tur2);//this is the new ref attacking turret for gdi
						create_cruiser_countdown_object = true;
						Create_2D_WAV_Sound_Team("ControlPointLost_GDI.wav",1);
						//destroy the spawned vehicles
						if ( (Commands->Find_Object(gdiveh1id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh1id))) )
						{
							Commands->Apply_Damage(Commands->Find_Object(gdiveh1id),999999.0f,"Death",obj);		gdiveh1id = 0;
						}
						Commands->Set_Player_Type(obj,-2);//it now belongs to nod because they fully destroyed it
						Commands->Set_Health(obj,2.0f);//we cant allow them to REALLY destroy it, otherwise the script halts and stuff....
					}
					else if (Commands->Get_Player_Type(obj) == -2)
					{
						Commands->Set_Health(obj,2.0f);//we cant allow them to REALLY destroy it, otherwise the script halts and stuff....
					}
				}
			}
		}
	}
	else
	{
		Commands->Set_Health(obj,Commands->Get_Health(obj)+(damage));//we cant allow them to REALLY destroy it, otherwise the script halts and stuff....
	}
}
void z_cp_cruiser_countdown_point::Timer_Expired(GameObject *obj, int number)
{
	if (number == 300300)
	{
		Console_Input("msg Running Center_Point made by zunnie and Mauler");
		Commands->Start_Timer(obj,this,1.0f,300301);
	}
	else if (number == 300301)
	{
		Send_Message(255,255,255,"Repair the Control Point in the center of map");
		Send_Message(255,255,255,"Carrier will attack the enemy base after one minute");
		Send_Message(255,255,255,"Destroy the Control Point to stop attacks");
		Commands->Start_Timer(obj,this,120.0f,300301);//spam loop		
	}
}
void z_cp_cruiser_countdown_point::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&create_cruiser_countdown_object,sizeof(create_cruiser_countdown_object),1);
	Auto_Save_Variable(&nodveh1id,sizeof(nodveh1id),2);
	Auto_Save_Variable(&gdiveh1id,sizeof(gdiveh1id),4);
}

void z_cp_cruiser_countdown::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	turretbarnod = Get_Int_Parameter("NodTurretBar_ID");
	turretrefnod = Get_Int_Parameter("NodTurretRef_ID");
	turrethongdi = Get_Int_Parameter("GDITurretHon_ID");
	turretrefgdi = Get_Int_Parameter("GDITurretRef_ID");
	if (team == 0)
	{
		Commands->Start_Timer(obj,this,60.0f,100);
	}
	else if (team == 1)
	{
		Commands->Start_Timer(obj,this,60.0f,101);
	}
}
void z_cp_cruiser_countdown::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100) // nod timer hits so we need the turrets from nod which attack gdi base:
	{
		//100065 -> bar			100067 -> ref (turrets)
		//100062 -> barattack	100071 -> refattack (attackpoints)
		Attach_Script_Once(Commands->Find_Object(turretbarnod),"z_cp_cruiser_attack","100131");//turret 1 that attacks the barracks
		Attach_Script_Once(Commands->Find_Object(turretrefnod),"z_cp_cruiser_attack","100132");//turret 2 that attacks the refinery

		GameObject *target1 = Find_Building_By_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY);
		if (target1)
		{
			Commands->Send_Custom_Event(obj,target1,50505,1,0);//allow player damage true
		}
		GameObject *target2 = Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY);
		if (target2)
		{
			Commands->Send_Custom_Event(obj,target2,50505,1,0);//allow player damage true
		}
	}
	else if (number == 101)
	{
		//100068 -> hand			100070 -> ref (turrets)
		//100073 -> handattack		100072 -> refattack (attackpoints)
		Attach_Script_Once(Commands->Find_Object(turrethongdi),"z_cp_cruiser_attack","100133");//turret 1 that attacks the hand
		Attach_Script_Once(Commands->Find_Object(turretrefgdi),"z_cp_cruiser_attack","100134");//turret 2 that attacks the refinery

		GameObject *target1 = Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY);
		if (target1)
		{
			Commands->Send_Custom_Event(obj,target1,50505,1,0);//allow player damage true
		}
		GameObject *target2 = Find_Building_By_Type(0,BuildingConstants::TYPE_REFINERY);
		if (target2)
		{
			Commands->Send_Custom_Event(obj,target2,50505,1,0);//allow player damage true
		}
	}
}
void z_cp_cruiser_countdown::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&team,sizeof(team),1);
	Auto_Save_Variable(&turretbarnod,sizeof(turretbarnod),2);
	Auto_Save_Variable(&turretrefnod,sizeof(turretrefnod),3);
	Auto_Save_Variable(&turrethongdi,sizeof(turrethongdi),4);
	Auto_Save_Variable(&turretrefgdi,sizeof(turretrefgdi),5);
}

void z_cp_cruiser_attack::Created(GameObject *obj)//this script will immideately attack whatever is passed as the attack id
{
	Commands->Start_Timer(obj,this,1.0f,250);
	Commands->Create_2D_WAV_Sound("attackalert.wav");
}
void z_cp_cruiser_attack::Timer_Expired(GameObject *obj, int number)
{
	if (number == 250)
	{
		ActionParamsStruct params;
		int priority = 100;		float range = 1000.0f;		float deviation = 0.0f;		int primary = 1;
		params.Set_Basic(this,(float)priority,40016);
		params.Set_Attack(Commands->Find_Object(Get_Int_Parameter("Attack_ID")),range,deviation,primary);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
		Commands->Start_Timer(obj,this,1.0f,250);
	}
}
void z_cp_cruiser_attack::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	Commands->Action_Reset(obj,100);
}

void z_cp_Harvester::Created(GameObject *obj)
{
	Set_Max_Shield_Strength(obj,600.0f);
	Set_Max_Health(obj,600.0f);
	Commands->Set_Shield_Strength(obj,600.0f);
	Commands->Set_Health(obj,600.0f);
	Commands->Set_Player_Type(obj,-2);
	Set_Damage_Points(obj,0.250f);
	isharvesting = false;
	hastiberium = false;
	driverid = 0;
	team = 0;
	plid = 0;
}
void z_cp_Harvester::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 1)
		{
			if (!isharvesting)
			{
				Commands->Start_Timer(obj,this,10.0f,100);
				isharvesting = true;
				Send_Message_Player(Commands->Find_Object(driverid),104,234,40,"You are now harvesting Tiberium. Please wait...");
			}
		}
	}
	else if (message == 1001)
	{
		if (param == 1)
		{
			isharvesting = false;
		}
	}
	else if (message == 1002)
	{
		if (param == 1)
		{
			if (hastiberium)
			{
				if (Commands->Get_Player_Type(obj) == 0)
				{
					if (Find_Building_By_Type(0,BuildingConstants::TYPE_REFINERY) && Commands->Get_Health(Find_Building_By_Type(0,BuildingConstants::TYPE_REFINERY)) > 1.0f)
					{
						Commands->Give_Money(obj,500.0f,true);
						Commands->Give_Points(Get_GameObj(plid),50.0f,false);
						Create_2D_WAV_Sound_Team("m00pc$$_aqob0002i1evag_snd.wav",0);
						char msg[250]; sprintf(msg,"%s deposited Tiberium funding Nod with $%g",Get_Player_Name(Commands->Find_Object(driverid)),500.0f);
						Send_Message_Team(0,255,0,0,msg);
						hastiberium = false;
						isharvesting = false;
					}
				}
				else if (Commands->Get_Player_Type(obj) == 1)
				{
					if (Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY) && Commands->Get_Health(Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY)) > 1.0f)
					{
						Commands->Give_Money(obj,500.0f,true);
						Commands->Give_Points(Get_GameObj(plid),50.0f,false);
						Create_2D_WAV_Sound_Team("m00pc$$_aqob0002i1evag_snd.wav",1);
						char msg2[250]; sprintf(msg2,"%s deposited Tiberium funding GDI with $%g",Get_Player_Name(Commands->Find_Object(driverid)),500.0f);
						Send_Message_Team(1,255,204,0,msg2);
						hastiberium = false;
						isharvesting = false;
					}
				}
			}
		}
	}
	else if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		team = Commands->Get_Player_Type(sender);
		driverid = Commands->Get_ID(sender);
		plid = Get_Player_ID(sender);
		Send_Message_Player(Commands->Find_Object(driverid),104,234,40,"Goto the Tiberium Field to harvest Tiberium, then return to the Refinery to deposit.");
	}
	else if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		team = 0;
		driverid = 0;
		plid = 0;
	}
}

void z_cp_Harvester::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100)
	{
		if (isharvesting)
		{
			hastiberium = true;
			isharvesting = false;
			Create_2D_WAV_Sound_Player(Commands->Find_Object(driverid),"00-n006e.wav");
			Send_Message_Player(Commands->Find_Object(driverid),104,234,40,"HARVESTING COMPLETE: Return to the Refinery to deposit.");
		}
	}
}
void z_cp_Harvester::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&team,sizeof(team),1);
	Auto_Save_Variable(&isharvesting,sizeof(isharvesting),2);
	Auto_Save_Variable(&hastiberium,sizeof(hastiberium),3);
	Auto_Save_Variable(&driverid,sizeof(driverid),4);
	Auto_Save_Variable(&plid,sizeof(plid),5);
}

void z_cp_Tiberium_Field::Entered(GameObject *obj, GameObject *enter)
{
	if (Is_Script_Attached(enter,"z_cp_Harvester"))
	{
		Commands->Send_Custom_Event(obj,enter,1000,1,0);//harveting started
	}
}
void z_cp_Tiberium_Field::Exited(GameObject *obj, GameObject *exit)
{
	if (Is_Script_Attached(exit,"z_cp_Harvester"))
	{
		Commands->Send_Custom_Event(obj,exit,1001,1,0);//not harvesting anymore
	}
}
void z_cp_Tiberium_Deposit::Entered(GameObject *obj, GameObject *enter)
{
	if (Is_Script_Attached(enter,"z_cp_Harvester"))
	{
		Commands->Send_Custom_Event(obj,enter,1002,1,0);//dump
	}	

	if (strcmp(Commands->Get_Preset_Name(enter),"CnC_GDI_Harvester") == 0)
	{
		Send_Message_Team(1,255,204,0,"The GDI Harvester Tiberium deposit imminent");
	}
	else if (strcmp(Commands->Get_Preset_Name(enter),"CnC_Nod_Harvester") == 0)
	{
		Send_Message_Team(0,255,0,0,"The Nod Harvester Tiberium deposit imminent");
	}
}

void z_cp_Building::Created(GameObject *obj)
{
	allow_player_damage = false;
}
void z_cp_Building::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 50505)
	{
		if (param == 0)
		{
			allow_player_damage = false;
		}
		else if (param == 1)
		{
			allow_player_damage = true;
		}
	}	
}
void z_cp_Building::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (allow_player_damage)
	{
		if (damage > 0.0f)
		{
			Commands->Give_Money(damager,(damage)/5,false);
			Commands->Give_Points(damager,(damage)/5,false);
		}
	}
	else if (!allow_player_damage)
	{
		if (damage > 0.0f)
		{
			if (Commands->Is_A_Star(damager))
			{
				Commands->Set_Health(obj,Commands->Get_Health(obj)+(damage));
			}
		}
	}
	if (damage < 0.0f)
	{
		if (Commands->Is_A_Star(damager))
		{
			/* damage = damage + (damage - (damage));
			float curhp = Commands->Get_Health(obj);
			Commands->Set_Health(obj,curhp+damage);*/
			Commands->Give_Money(damager,(-damage)/5,false);
			Commands->Give_Points(damager,(-damage)/5,false);
		}
	}
}
void z_cp_Building::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&allow_player_damage,sizeof(allow_player_damage),1);
}

void z_cp_Limit_1000_Chars::Created(GameObject *obj)
{
	if (Commands->Get_Player_Type(obj) == 0)
	{
		if ( (strcmp(Commands->Get_Preset_Name(obj),"cp_Nod_Minigunner_3Boss") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"cp_Nod_MiniGunner_3Boss_ALT2") == 0) )
		{
			int sakuras = Get_Object_Count(0,"cp_Nod_Minigunner_3Boss");
			int sakuras2 = Get_Object_Count(0,"cp_Nod_MiniGunner_3Boss_ALT2");
			int sakurastotal = sakuras+sakuras2;
			if (sakurastotal >= 3)
			{
				Create_2D_WAV_Sound_Player(obj,"m00evag_dsgn0039i1evag_snd.wav");
				Send_Message_Player(obj,255,0,0,"Sorry the Sakura Limit has been reached (3) You have been refunded your $2000 credits");
				Commands->Give_Money(obj,2000.0f,false);
				Change_Character(obj,"CnC_Nod_Minigunner_0");
			}
		}
		else if ( (strcmp(Commands->Get_Preset_Name(obj),"cp_Nod_FlameThrower_3Boss") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"cp_Nod_FlameThrower_3Boss_ALT2") == 0) )
		{
			int mendozas = Get_Object_Count(0,"cp_Nod_FlameThrower_3Boss");
			int mendozas2 = Get_Object_Count(0,"cp_Nod_FlameThrower_3Boss_ALT2");
			int mendozastotal = mendozas+mendozas2;
			if (mendozastotal >= 3)
			{
				Create_2D_WAV_Sound_Player(obj,"m00evag_dsgn0039i1evag_snd.wav");
				Send_Message_Player(obj,255,0,0,"Sorry the Mendoza Limit has been reached (3) You have been refunded your $2000 credits");
				Commands->Give_Money(obj,2000.0f,false);
				Change_Character(obj,"CnC_Nod_Minigunner_0");
			}
		}
		else if ( (strcmp(Commands->Get_Preset_Name(obj),"cp_Nod_RocketSoldier_3Boss") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"cp_Nod_RocketSoldier_3Boss_ALT2") == 0) )
		{
			int raves = Get_Object_Count(0,"cp_Nod_RocketSoldier_3Boss");
			int raves2 = Get_Object_Count(0,"cp_Nod_RocketSoldier_3Boss_ALT2");
			int ravestotal = raves+raves2;
			if (ravestotal >= 3)
			{
				Create_2D_WAV_Sound_Player(obj,"m00evag_dsgn0039i1evag_snd.wav");
				Send_Message_Player(obj,255,0,0,"Sorry the Raveshaw Limit has been reached (3) You have been refunded your $2000 credits");
				Commands->Give_Money(obj,2000.0f,false);
				Change_Character(obj,"CnC_Nod_Minigunner_0");
			}
		}
	}
	else if (Commands->Get_Player_Type(obj) == 1)
	{
		if ( (strcmp(Commands->Get_Preset_Name(obj),"cp_GDI_MiniGunner_3Boss") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"cp_GDI_MiniGunner_3Boss_ALT2") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"cp_GDI_MiniGunner_3Boss_ALT3") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"cp_GDI_MiniGunner_3Boss_ALT4") == 0) )
		{
			int havocs = Get_Object_Count(1,"cp_GDI_MiniGunner_3Boss");
			int havocs2 = Get_Object_Count(1,"cp_GDI_MiniGunner_3Boss_ALT2");
			int havocs3 = Get_Object_Count(1,"cp_GDI_MiniGunner_3Boss_ALT3");
			int havocs4 = Get_Object_Count(1,"cp_GDI_MiniGunner_3Boss_ALT4");
			int havoctotal = havocs+havocs2+havocs3+havocs4;
			if (havoctotal >= 3)
			{
				Create_2D_WAV_Sound_Player(obj,"m00evag_dsgn0039i1evag_snd.wav");
				Send_Message_Player(obj,255,204,0,"Sorry the Havoc Limit has been reached (3) You have been refunded your $2000 credits");
				Commands->Give_Money(obj,2000.0f,false);
				Change_Character(obj,"CnC_GDI_MiniGunner_0");
			}
		}
		else if ( (strcmp(Commands->Get_Preset_Name(obj),"cp_Ignatio_Mobius") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"cp_Ignatio_Mobius_ALT2") == 0) )
		{
			int mobius = Get_Object_Count(1,"cp_Ignatio_Mobius");
			int mobius2 = Get_Object_Count(1,"cp_Ignatio_Mobius_ALT2");
			int mobiustotal = mobius+mobius2;
			if (mobiustotal >= 3)
			{
				Create_2D_WAV_Sound_Player(obj,"m00evag_dsgn0039i1evag_snd.wav");
				Send_Message_Player(obj,255,204,0,"Sorry the Mobius Limit has been reached (3) You have been refunded your $2000 credits");
				Commands->Give_Money(obj,2000.0f,false);
				Change_Character(obj,"CnC_GDI_MiniGunner_0");
			}
		}
		else if ( (strcmp(Commands->Get_Preset_Name(obj),"cp_Sydney_PowerSuit") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"cp_Sydney_PowerSuit_ALT2") == 0) )
		{
			int sydneys = Get_Object_Count(1,"cp_Sydney_PowerSuit");
			int sydneys2 = Get_Object_Count(1,"cp_Sydney_PowerSuit_ALT2");
			int sydneystotal = sydneys+sydneys2;
			if (sydneystotal >= 3)
			{
				Create_2D_WAV_Sound_Player(obj,"m00evag_dsgn0039i1evag_snd.wav");
				Send_Message_Player(obj,255,204,0,"Sorry the Sydney Powersuit Limit has been reached (3) You have been refunded your $2000 credits");
				Commands->Give_Money(obj,2000.0f,false);
				Change_Character(obj,"CnC_GDI_MiniGunner_0");
			}
		}
	}
}


ScriptRegistrant<z_cp_Limit_1000_Chars> z_cp_Limit_1000_Chars_Registrant("z_cp_Limit_1000_Chars","");
ScriptRegistrant<z_cp_Building> z_cp_Building_Registrant("z_cp_Building","");
ScriptRegistrant<z_cp_Harvester> z_cp_Harvester_Registrant("z_cp_Harvester","");
ScriptRegistrant<z_cp_Tiberium_Field> z_cp_Tiberium_Field_Registrant("z_cp_Tiberium_Field","");
ScriptRegistrant<z_cp_Tiberium_Deposit> z_cp_Tiberium_Deposit_Registrant("z_cp_Tiberium_Deposit","");
ScriptRegistrant<z_cp_cruiser_attack> z_cp_cruiser_attack_Registrant("z_cp_cruiser_attack","Attack_ID=1111:int,TurretID=0:int");
ScriptRegistrant<z_cp_cruiser_countdown> z_cp_cruiser_countdown_Registrant("z_cp_cruiser_countdown","Team=0:int,NodTurretBar_ID=1:int,NodTurretRef_ID=1:int,GDITurretHon_ID=1:int,GDITurretRef_ID=1:int");
ScriptRegistrant<z_cp_cruiser_countdown_point> z_cp_cruiser_countdown_point_Registrant("z_cp_cruiser_countdown_point","NodVeh1=asdf:string,NodVeh1Pos=0:vector3,NodVeh1Facing=0:float,GDIVeh1=asdf:string,GDIVeh1Pos=0:vector3,GDIVeh1Facing=0:float,SpawnPad1ID=0:int,SpawnPad2ID=0:int");