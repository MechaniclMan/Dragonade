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
#include "engine.h"
#include "BuildingGameObj.h"
#include "tcw_domination.h"
#include "GameObjManager.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"

//0=nod 1=gdi 2=neu current animations for spawnpads

void tcw_domination_control::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,120.0f,123439348);//spam score
	triggerendmsg = true;
	points_gdi = 0;		points_nod = 0;
	points_max = Get_Int_Parameter("Points_Max");
}
void tcw_domination_control::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1000)
	{
		if (param == 0)
		{
			points_nod += 1;
			if (points_nod >= points_max)
			{
				if (triggerendmsg)
				{
					triggerendmsg = false;
					char nodwins[250];
					sprintf(nodwins,"amsg Nod has won the game with a %d point lead (%d/%d)",points_nod-points_gdi,points_nod,points_gdi);
					Console_Input(nodwins);		Console_Input("win 0");
				}
			}
		}
		if (param == 1)
		{
			if (triggerendmsg)
			{
				points_gdi += 1;
				if (points_gdi >= points_max)
				{
					triggerendmsg = false;
					char gdiwins[250];
					sprintf(gdiwins,"amsg GDI has won the game with a %d point lead (%d/%d)",points_gdi-points_nod,points_gdi,points_nod);
					Console_Input(gdiwins);		Console_Input("win 1");
				}
			}
		}
		else if (param == 2)
		{
			if (points_gdi > points_nod)
			{
				char status[250]; char lead[250];
				sprintf(status,"msg GDI: %d points -- Nod: %d points -- Required: %d points",points_gdi,points_nod,points_max);
				sprintf(lead,"msg GDI leads the game with a %d points lead",points_gdi-points_nod);
				Console_Input(status);
				Console_Input(lead);
			}
			else if (points_nod > points_gdi)
			{
				char status[250]; char lead[250];
				sprintf(status,"msg GDI: %d points -- Nod: %d points -- Required: %d points",points_gdi,points_nod,points_max);
				sprintf(lead,"msg Nod leads the game with a %d points lead",points_nod-points_gdi);
				Console_Input(status);
				Console_Input(lead);
			}
			else
			{
				char status[250];
				sprintf(status,"msg GDI: %d points ----- Nod: %d points",points_gdi,points_nod);
				Console_Input(status);
			}
		}
	}
}
void tcw_domination_control::Timer_Expired(GameObject *obj, int number)
{
	if (number == 123439348)
	{
		if (points_gdi > points_nod)
		{
			char status[250]; char lead[250];
			sprintf(status,"msg GDI: %d points -- Nod: %d points -- Required: %d points",points_gdi,points_nod,points_max);
			sprintf(lead,"msg GDI leads the game with a %d points lead",points_gdi-points_nod);
			Console_Input(status);
			Console_Input(lead);
		}
		else if (points_nod > points_gdi)
		{
			char status[250]; char lead[250];
			sprintf(status,"msg GDI: %d points -- Nod: %d points -- Required: %d points",points_gdi,points_nod,points_max);
			sprintf(lead,"msg Nod leads the game with a %d points lead",points_nod-points_gdi);
			Console_Input(status);
			Console_Input(lead);
		}
		else
		{
			char status[250];
			sprintf(status,"msg GDI: %d points ----- Nod: %d points",points_gdi,points_nod);
			Console_Input(status);
		}
		Commands->Start_Timer(obj,this,120.0f,123439348);
	}
}
void tcw_domination_control::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&points_gdi,sizeof(points_gdi),1);
	Auto_Save_Variable(&points_nod,sizeof(points_nod),2);
	Auto_Save_Variable(&points_max,sizeof(points_max),3);
	Auto_Save_Variable(&triggerendmsg,sizeof(triggerendmsg),4);
}

void tcw_domination_pointgen::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	Commands->Start_Timer(obj,this,1.0f,303);
}
void tcw_domination_pointgen::Timer_Expired(GameObject *obj, int number)
{
	if (number == 303)
	{
		if (team == 0)
		{
			Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_domination_control"),1000,0,0);
		}
		else if (team == 1)
		{
			Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_domination_control"),1000,1,0);
		}
		Commands->Start_Timer(obj,this,1.0f,303);
	}
}
void tcw_domination_pointgen::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&team,sizeof(team),1);
}

void tcw_domination_node::Created(GameObject *obj)
{
	Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);
	spawnpad1 = Get_Int_Parameter("SpawnPad1ID");
	if (Commands->Find_Object(spawnpad1))
	{
		Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
	}
	spawnpad2 = Get_Int_Parameter("SpawnPad2ID");
	if (Commands->Find_Object(spawnpad2))
	{
		Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
	}
	spawnpad3 = Get_Int_Parameter("SpawnPad3ID");
	if (Commands->Find_Object(spawnpad3))
	{
		Commands->Set_Animation(Commands->Find_Object(spawnpad3),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
	}
	spawnpad4 = Get_Int_Parameter("SpawnPad4ID");
	if (Commands->Find_Object(spawnpad4))
	{
		Commands->Set_Animation(Commands->Find_Object(spawnpad4),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
	}
	nodename = Get_Parameter("Node_Name");
	owner = -2;
	nodveh1id = 0;	nodveh2id = 0; nodveh3id = 0;	nodveh4id = 0; nodveh5id = 0;
	gdiveh1id = 0;	gdiveh2id = 0; gdiveh3id = 0;	gdiveh4id = 0; gdiveh5id = 0;
	trigger_neutral_damage = true;
	Commands->Start_Timer(obj,this,60.0f,200200);//recheck spawns for nod
	Commands->Start_Timer(obj,this,60.0f,200201);//recheck spawns for gdi
}
void tcw_domination_node::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if ((damage < 0.0f) && (Commands->Get_Player_Type(obj) == -2))
	{
		if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))//if node fully repaired
		{
			trigger_neutral_damage = true;
			int team = Commands->Get_Player_Type(damager);
			if (team == 0)
			{
				char params[250]; sprintf(params,"%d",team);
				Attach_Script_Once(obj,"tcw_domination_pointgen",params);
				owner = 0;
				Commands->Set_Player_Type(obj,0);

				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,0,0,false);
				if (Commands->Find_Object(spawnpad1))
				{
					Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false);
				}
				if (Commands->Find_Object(spawnpad2))
				{
					Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false);
				}
				if (Commands->Find_Object(spawnpad3))
				{
					Commands->Set_Animation(Commands->Find_Object(spawnpad3),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false);
				}
				if (Commands->Find_Object(spawnpad4))
				{
					Commands->Set_Animation(Commands->Find_Object(spawnpad4),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false);
				}

				if (!Commands->Find_Object(nodveh1id))
				{
					if (strcmp(Get_Parameter("NodVeh1"), "0") != 0)
					{
						Vector3 nodveh1pos = Get_Vector3_Parameter("NodVeh1Pos");
						float nodveh1fac = Get_Float_Parameter("NodVeh1Facing");
						GameObject *nodveh1 = Commands->Create_Object(Get_Parameter("NodVeh1"),nodveh1pos);
						Commands->Set_Facing(nodveh1,nodveh1fac);
						nodveh1id = Commands->Get_ID(nodveh1);
						Attach_Script_Once(nodveh1,"tcw_domination_vehicle","0");
						Commands->Set_Player_Type(nodveh1,-2);
					}
				}
				if (!Commands->Find_Object(nodveh2id))
				{
					if (strcmp(Get_Parameter("NodVeh2"), "0") != 0)
					{
						Vector3 nodveh2pos = Get_Vector3_Parameter("NodVeh2Pos");
						float nodveh2fac = Get_Float_Parameter("NodVeh2Facing");
						GameObject *nodveh2 = Commands->Create_Object(Get_Parameter("NodVeh2"),nodveh2pos);
						Commands->Set_Facing(nodveh2,nodveh2fac);
						nodveh2id = Commands->Get_ID(nodveh2);
						Attach_Script_Once(nodveh2,"tcw_domination_vehicle","0");
						Commands->Set_Player_Type(nodveh2,-2);
					}
				}
				if (!Commands->Find_Object(nodveh3id))
				{
					if (strcmp(Get_Parameter("NodVeh3"), "0") != 0)
					{
						Vector3 nodveh3pos = Get_Vector3_Parameter("NodVeh3Pos");
						float nodveh3fac = Get_Float_Parameter("NodVeh3Facing");
						GameObject *nodveh3 = Commands->Create_Object(Get_Parameter("NodVeh3"),nodveh3pos);
						Commands->Set_Facing(nodveh3,nodveh3fac);
						nodveh3id = Commands->Get_ID(nodveh3);
						Attach_Script_Once(nodveh3,"tcw_domination_vehicle","0");
						Commands->Set_Player_Type(nodveh3,-2);
					}
				}
				if (!Commands->Find_Object(nodveh4id))
				{
					if (strcmp(Get_Parameter("NodVeh4"), "0") != 0)
					{
						Vector3 nodveh4pos = Get_Vector3_Parameter("NodVeh4Pos");
						float nodveh4fac = Get_Float_Parameter("NodVeh4Facing");
						GameObject *nodveh4 = Commands->Create_Object(Get_Parameter("NodVeh4"),nodveh4pos);
						Commands->Set_Facing(nodveh4,nodveh4fac);
						nodveh4id = Commands->Get_ID(nodveh4);
						Attach_Script_Once(nodveh4,"tcw_domination_vehicle","0");
						Commands->Set_Player_Type(nodveh4,-2);
					}
				}
				if (!Commands->Find_Object(nodveh5id))
				{
					if (strcmp(Get_Parameter("NodVeh5"), "0") != 0)
					{
						Vector3 nodveh5pos = Get_Vector3_Parameter("NodVeh5Pos");
						float nodveh5fac = Get_Float_Parameter("NodVeh5Facing");
						GameObject *nodveh5 = Commands->Create_Object(Get_Parameter("NodVeh5"),nodveh5pos);
						Commands->Set_Facing(nodveh5,nodveh5fac);
						nodveh5id = Commands->Get_ID(nodveh5);
						Attach_Script_Once(nodveh5,"tcw_domination_vehicle","0");
						Commands->Set_Player_Type(nodveh5,-2);
					}
				}

				char capturemsg[250];	sprintf(capturemsg,"Nod captured the %s Control Point.",nodename);	Send_Message(255,255,255,capturemsg);
				Create_2D_WAV_Sound_Team("ControlPointCaptured_Nod.wav",0); 
				if (Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
				{
					Commands->Give_Money(damager,500.0f,true);
				}
			}
			else if (team == 1)
			{
				char params[250]; sprintf(params,"%d",team);
				Attach_Script_Once(obj,"tcw_domination_pointgen",params);
				owner = 1;
				Commands->Set_Player_Type(obj,1);

				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,1,1,false);
				if (Commands->Find_Object(spawnpad1))
				{
					Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false);
				}
				if (Commands->Find_Object(spawnpad2))
				{
					Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false);
				}
				if (Commands->Find_Object(spawnpad3))
				{
					Commands->Set_Animation(Commands->Find_Object(spawnpad3),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false);
				}
				if (Commands->Find_Object(spawnpad4))
				{
					Commands->Set_Animation(Commands->Find_Object(spawnpad4),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false);
				}

				if (!Commands->Find_Object(gdiveh1id))
				{
					if (strcmp(Get_Parameter("GDIVeh1"), "0") != 0)
					{
						Vector3 gdiveh1pos = Get_Vector3_Parameter("GDIVeh1Pos");
						float gdiveh1fac = Get_Float_Parameter("GDIVeh1Facing");
						GameObject *gdiveh1 = Commands->Create_Object(Get_Parameter("GDIVeh1"),gdiveh1pos);
						Commands->Set_Facing(gdiveh1,gdiveh1fac);
						gdiveh1id = Commands->Get_ID(gdiveh1);
						Attach_Script_Once(gdiveh1,"tcw_domination_vehicle","1");
						Commands->Set_Player_Type(gdiveh1,-2);
					}
				}
				if (!Commands->Find_Object(gdiveh2id))
				{
					if (strcmp(Get_Parameter("GDIVeh2"), "0") != 0)
					{
						Vector3 gdiveh2pos = Get_Vector3_Parameter("GDIVeh2Pos");
						float gdiveh2fac = Get_Float_Parameter("GDIVeh2Facing");
						GameObject *gdiveh2 = Commands->Create_Object(Get_Parameter("GDIVeh2"),gdiveh2pos);
						Commands->Set_Facing(gdiveh2,gdiveh2fac);
						gdiveh2id = Commands->Get_ID(gdiveh2);
						Attach_Script_Once(gdiveh2,"tcw_domination_vehicle","1");
						Commands->Set_Player_Type(gdiveh2,-2);
					}
				}
				if (!Commands->Find_Object(gdiveh3id))
				{
					if (strcmp(Get_Parameter("GDIVeh3"), "0") != 0)
					{
						Vector3 gdiveh3pos = Get_Vector3_Parameter("GDIVeh3Pos");
						float gdiveh3fac = Get_Float_Parameter("GDIVeh3Facing");
						GameObject *gdiveh3 = Commands->Create_Object(Get_Parameter("GDIVeh3"),gdiveh3pos);
						Commands->Set_Facing(gdiveh3,gdiveh3fac);
						gdiveh3id = Commands->Get_ID(gdiveh3);
						Attach_Script_Once(gdiveh3,"tcw_domination_vehicle","1");
						Commands->Set_Player_Type(gdiveh3,-2);
					}
				}
				if (!Commands->Find_Object(gdiveh4id))
				{
					if (strcmp(Get_Parameter("GDIVeh4"), "0") != 0)
					{
						Vector3 gdiveh4pos = Get_Vector3_Parameter("GDIVeh4Pos");
						float gdiveh4fac = Get_Float_Parameter("GDIVeh4Facing");
						GameObject *gdiveh4 = Commands->Create_Object(Get_Parameter("GDIVeh4"),gdiveh4pos);
						Commands->Set_Facing(gdiveh4,gdiveh4fac);
						gdiveh4id = Commands->Get_ID(gdiveh4);
						Attach_Script_Once(gdiveh4,"tcw_domination_vehicle","1");
						Commands->Set_Player_Type(gdiveh4,-2);
					}
				}
				if (!Commands->Find_Object(gdiveh5id))
				{
					if (strcmp(Get_Parameter("GDIVeh5"), "0") != 0)
					{
						Vector3 gdiveh5pos = Get_Vector3_Parameter("GDIVeh5Pos");
						float gdiveh5fac = Get_Float_Parameter("GDIVeh5Facing");
						GameObject *gdiveh5 = Commands->Create_Object(Get_Parameter("GDIVeh5"),gdiveh5pos);
						Commands->Set_Facing(gdiveh5,gdiveh5fac);
						gdiveh5id = Commands->Get_ID(gdiveh5);
						Attach_Script_Once(gdiveh5,"tcw_domination_vehicle","1");
						Commands->Set_Player_Type(gdiveh5,-2);
					}
				}

				char capturemsg[250];	sprintf(capturemsg,"GDI captured the %s Control Point.",nodename);	Send_Message(255,255,255,capturemsg);
				Create_2D_WAV_Sound_Team("ControlPointCaptured_GDI.wav",1); 
				if (Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
				{
					Commands->Give_Money(damager,500.0f,true);
				}
			}
		}
	}
	else if (damage > 0.0f)//it is being attacked by enemy
	{
		if (damage > Commands->Get_Health(obj))//if node fully destroyed
		{
			Commands->Set_Health(obj,1.1f);
			Commands->Set_Player_Type(obj,-2);
			if (trigger_neutral_damage)
			{
				trigger_neutral_damage = false;
				Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
				Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
				Commands->Set_Animation(Commands->Find_Object(spawnpad3),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
				Commands->Set_Animation(Commands->Find_Object(spawnpad4),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
				Remove_Script(obj,"tcw_domination_pointgen");
				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);

				if (owner == 0)
				{
					Create_2D_WAV_Sound_Team("ControlPointLost_Nod.wav",0);
					if ( (Commands->Find_Object(nodveh1id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh1id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh1id),99999.0f,"Steel",obj);		nodveh1id = 0;
					}
					if ( (Commands->Find_Object(nodveh2id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh2id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh2id),99999.0f,"Steel",obj);		nodveh2id = 0;
					}
					if ( (Commands->Find_Object(nodveh3id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh3id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh3id),99999.0f,"Steel",obj);		nodveh3id = 0;
					}
					if ( (Commands->Find_Object(nodveh4id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh4id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh4id),99999.0f,"Steel",obj);		nodveh4id = 0;
					}
					if ( (Commands->Find_Object(nodveh5id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh5id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh5id),99999.0f,"Steel",obj);		nodveh5id = 0;
					}
				}
				else if (owner == 1)
				{
					Create_2D_WAV_Sound_Team("ControlPointLost_GDI.wav",1);
					if ( (Commands->Find_Object(gdiveh1id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh1id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh1id),99999.0f,"Steel",obj);		gdiveh1id = 0;
					}
					if ( (Commands->Find_Object(gdiveh2id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh2id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh2id),99999.0f,"Steel",obj);		gdiveh2id = 0;
					}
					if ( (Commands->Find_Object(gdiveh3id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh3id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh3id),99999.0f,"Steel",obj);		gdiveh3id = 0;
					}
					if ( (Commands->Find_Object(gdiveh4id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh4id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh4id),99999.0f,"Steel",obj);		gdiveh4id = 0;
					}
					if ( (Commands->Find_Object(gdiveh5id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh5id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh5id),99999.0f,"Steel",obj);		gdiveh5id = 0;
					}
				}
				owner = -2;
			}
		}
	}
}
void tcw_domination_node::Timer_Expired(GameObject *obj, int number)
{
	if (number == 200200)
	{
		if (owner == 0)
		{
			if (!Commands->Find_Object(nodveh1id))
			{
				if (strcmp(Get_Parameter("NodVeh1"), "0") != 0)
				{
					Vector3 nodveh1pos = Get_Vector3_Parameter("NodVeh1Pos");
					float nodveh1fac = Get_Float_Parameter("NodVeh1Facing");
					GameObject *nodveh1 = Commands->Create_Object(Get_Parameter("NodVeh1"),nodveh1pos);
					Commands->Set_Facing(nodveh1,nodveh1fac);
					nodveh1id = Commands->Get_ID(nodveh1);
					Attach_Script_Once(nodveh1,"tcw_domination_vehicle","0");
					Commands->Set_Player_Type(nodveh1,-2);
				}
			}
			if (!Commands->Find_Object(nodveh2id))
			{
				if (strcmp(Get_Parameter("NodVeh2"), "0") != 0)
				{
					Vector3 nodveh2pos = Get_Vector3_Parameter("NodVeh2Pos");
					float nodveh2fac = Get_Float_Parameter("NodVeh2Facing");
					GameObject *nodveh2 = Commands->Create_Object(Get_Parameter("NodVeh2"),nodveh2pos);
					Commands->Set_Facing(nodveh2,nodveh2fac);
					nodveh2id = Commands->Get_ID(nodveh2);
					Attach_Script_Once(nodveh2,"tcw_domination_vehicle","0");
					Commands->Set_Player_Type(nodveh2,-2);
				}
			}
			if (!Commands->Find_Object(nodveh3id))
			{
				if (strcmp(Get_Parameter("NodVeh3"), "0") != 0)
				{
					Vector3 nodveh3pos = Get_Vector3_Parameter("NodVeh3Pos");
					float nodveh3fac = Get_Float_Parameter("NodVeh3Facing");
					GameObject *nodveh3 = Commands->Create_Object(Get_Parameter("NodVeh3"),nodveh3pos);
					Commands->Set_Facing(nodveh3,nodveh3fac);
					nodveh3id = Commands->Get_ID(nodveh3);
					Attach_Script_Once(nodveh3,"tcw_domination_vehicle","0");
					Commands->Set_Player_Type(nodveh3,-2);
				}
			}
			if (!Commands->Find_Object(nodveh4id))
			{
				if (strcmp(Get_Parameter("NodVeh4"), "0") != 0)
				{
					Vector3 nodveh4pos = Get_Vector3_Parameter("NodVeh4Pos");
					float nodveh4fac = Get_Float_Parameter("NodVeh4Facing");
					GameObject *nodveh4 = Commands->Create_Object(Get_Parameter("NodVeh4"),nodveh4pos);
					Commands->Set_Facing(nodveh4,nodveh4fac);
					nodveh4id = Commands->Get_ID(nodveh4);
					Attach_Script_Once(nodveh4,"tcw_domination_vehicle","0");
					Commands->Set_Player_Type(nodveh4,-2);
				}
			}
			if (!Commands->Find_Object(nodveh5id))
			{
				if (strcmp(Get_Parameter("NodVeh5"), "0") != 0)
				{
					Vector3 nodveh5pos = Get_Vector3_Parameter("NodVeh5Pos");
					float nodveh5fac = Get_Float_Parameter("NodVeh5Facing");
					GameObject *nodveh5 = Commands->Create_Object(Get_Parameter("NodVeh5"),nodveh5pos);
					Commands->Set_Facing(nodveh5,nodveh5fac);
					nodveh5id = Commands->Get_ID(nodveh5);
					Attach_Script_Once(nodveh5,"tcw_domination_vehicle","0");
					Commands->Set_Player_Type(nodveh5,-2);
				}
			}
		}
		Commands->Start_Timer(obj,this,60.0f,200200);//recheck spawns
	}
	else if (number == 200201)
	{
		if (owner == 1)
		{
			if (!Commands->Find_Object(gdiveh1id))
			{
				if (strcmp(Get_Parameter("GDIVeh1"), "0") != 0)
				{
					Vector3 gdiveh1pos = Get_Vector3_Parameter("GDIVeh1Pos");
					float gdiveh1fac = Get_Float_Parameter("GDIVeh1Facing");
					GameObject *gdiveh1 = Commands->Create_Object(Get_Parameter("GDIVeh1"),gdiveh1pos);
					Commands->Set_Facing(gdiveh1,gdiveh1fac);
					gdiveh1id = Commands->Get_ID(gdiveh1);
					Attach_Script_Once(gdiveh1,"tcw_domination_vehicle","1");
					Commands->Set_Player_Type(gdiveh1,-2);
				}
			}
			if (!Commands->Find_Object(gdiveh2id))
			{
				if (strcmp(Get_Parameter("GDIVeh2"), "0") != 0)
				{
					Vector3 gdiveh2pos = Get_Vector3_Parameter("GDIVeh2Pos");
					float gdiveh2fac = Get_Float_Parameter("GDIVeh2Facing");
					GameObject *gdiveh2 = Commands->Create_Object(Get_Parameter("GDIVeh2"),gdiveh2pos);
					Commands->Set_Facing(gdiveh2,gdiveh2fac);
					gdiveh2id = Commands->Get_ID(gdiveh2);
					Attach_Script_Once(gdiveh2,"tcw_domination_vehicle","1");
					Commands->Set_Player_Type(gdiveh2,-2);
				}
			}
			if (!Commands->Find_Object(gdiveh3id))
			{
				if (strcmp(Get_Parameter("GDIVeh3"), "0") != 0)
				{
					Vector3 gdiveh3pos = Get_Vector3_Parameter("GDIVeh3Pos");
					float gdiveh3fac = Get_Float_Parameter("GDIVeh3Facing");
					GameObject *gdiveh3 = Commands->Create_Object(Get_Parameter("GDIVeh3"),gdiveh3pos);
					Commands->Set_Facing(gdiveh3,gdiveh3fac);
					gdiveh3id = Commands->Get_ID(gdiveh3);
					Attach_Script_Once(gdiveh3,"tcw_domination_vehicle","1");
					Commands->Set_Player_Type(gdiveh3,-2);
				}
			}
			if (!Commands->Find_Object(gdiveh4id))
			{
				if (strcmp(Get_Parameter("GDIVeh4"), "0") != 0)
				{
					Vector3 gdiveh4pos = Get_Vector3_Parameter("GDIVeh4Pos");
					float gdiveh4fac = Get_Float_Parameter("GDIVeh4Facing");
					GameObject *gdiveh4 = Commands->Create_Object(Get_Parameter("GDIVeh4"),gdiveh4pos);
					Commands->Set_Facing(gdiveh4,gdiveh4fac);
					gdiveh4id = Commands->Get_ID(gdiveh4);
					Attach_Script_Once(gdiveh4,"tcw_domination_vehicle","1");
					Commands->Set_Player_Type(gdiveh4,-2);
				}
			}
			if (!Commands->Find_Object(gdiveh5id))
			{
				if (strcmp(Get_Parameter("GDIVeh5"), "0") != 0)
				{
					Vector3 gdiveh5pos = Get_Vector3_Parameter("GDIVeh5Pos");
					float gdiveh5fac = Get_Float_Parameter("GDIVeh5Facing");
					GameObject *gdiveh5 = Commands->Create_Object(Get_Parameter("GDIVeh5"),gdiveh5pos);
					Commands->Set_Facing(gdiveh5,gdiveh5fac);
					gdiveh5id = Commands->Get_ID(gdiveh5);
					Attach_Script_Once(gdiveh5,"tcw_domination_vehicle","1");
					Commands->Set_Player_Type(gdiveh5,-2);
				}
			}
		}
		Commands->Start_Timer(obj,this,60.0f,200201);//recheck spawns
	}
}
void tcw_domination_node::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&nodename,sizeof(nodename),1);
	Auto_Save_Variable(&owner,sizeof(owner),2);
	Auto_Save_Variable(&nodveh1id,sizeof(nodveh1id),3);
	Auto_Save_Variable(&nodveh2id,sizeof(nodveh2id),4);
	Auto_Save_Variable(&nodveh3id,sizeof(nodveh3id),5);
	Auto_Save_Variable(&nodveh4id,sizeof(nodveh4id),6);
	Auto_Save_Variable(&nodveh5id,sizeof(nodveh5id),7);
	Auto_Save_Variable(&gdiveh1id,sizeof(gdiveh1id),8);
	Auto_Save_Variable(&gdiveh2id,sizeof(gdiveh2id),9);
	Auto_Save_Variable(&gdiveh3id,sizeof(gdiveh3id),10);
	Auto_Save_Variable(&gdiveh4id,sizeof(gdiveh4id),11);
	Auto_Save_Variable(&gdiveh5id,sizeof(gdiveh5id),12);
	Auto_Save_Variable(&spawnpad1,sizeof(spawnpad1),13);
	Auto_Save_Variable(&spawnpad2,sizeof(spawnpad2),14);
	Auto_Save_Variable(&spawnpad3,sizeof(spawnpad3),15);
	Auto_Save_Variable(&spawnpad4,sizeof(spawnpad4),16);
}

void tcw_domination_vehicle::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if ( (Commands->Get_Player_Type(sender) != Get_Int_Parameter("Team")) )
		{
			Commands->Start_Timer(obj,this,0.5,509482);
		}
		else if ( (Commands->Get_Player_Type(sender) == Get_Int_Parameter("Team")) )
		{
			Destroy_Script();
		}
	}
}
void tcw_domination_vehicle::Timer_Expired(GameObject *obj, int number)
{
	if (number == 509482)
	{
		Force_Occupant_Exit(obj,0);
	}
}

void tcw_wmd_countdown_node::Created(GameObject *obj)
{
	allowcap = true;
	nodename = Get_Parameter("Node_Name");
	owner = -2;
	nodveh1id = 0;	nodveh2id = 0; nodveh3id = 0;	nodveh4id = 0; nodveh5id = 0;
	gdiveh1id = 0;	gdiveh2id = 0; gdiveh3id = 0;	gdiveh4id = 0; gdiveh5id = 0;
}
void tcw_wmd_countdown_node::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 1111)
	{
		if (param == 1)
		{
			allowcap = false; Commands->Start_Timer(obj,this,15.5,2050);
			owner = -2;
			Commands->Set_Player_Type(obj,-2);
			Commands->Set_Health(obj,2.0f);
		}
	}
}
void tcw_wmd_countdown_node::Timer_Expired(GameObject *obj, int number)
{
	if (number == 2050)
	{
		allowcap = true;
	}
}
void tcw_wmd_countdown_node::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (damage < 0.0f)
	{
		if (allowcap)
		{
			if (owner == -2)
			{
				owner = Commands->Get_Player_Type(damager);
				Commands->Set_Player_Type(obj,Commands->Get_Player_Type(damager));
			}
			else if (owner == 0)
			{
				if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))//if node fully repaired
				{

					if (!Find_Object_With_Script("tcw_wmd_strike"))//if there is no existing strike
					{
						//gdi buildings attacks:
						if ( (!Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY))) && (Find_Building_By_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY)) )//if bar is alive, target it....
						{
							Vector3 ionarrowpos = Commands->Get_Position(Find_Building_By_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY));
							GameObject *ionarrow = Commands->Create_Object("Invisible_Object",ionarrowpos);
							Attach_Script_Once(ionarrow,"tcw_wmd_strike","4");//pass 4 for bar
						}
						else if ( (!Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_VEHICLE_FACTORY))) && (Find_Building_By_Type(1,BuildingConstants::TYPE_VEHICLE_FACTORY)) )//if gwf is alive, target it....
						{
							Vector3 ionarrowpos = Commands->Get_Position(Find_Building_By_Type(1,BuildingConstants::TYPE_VEHICLE_FACTORY));
							GameObject *ionarrow = Commands->Create_Object("Invisible_Object",ionarrowpos);
							Attach_Script_Once(ionarrow,"tcw_wmd_strike","5");//pass 5 for gwf
						}
						else if ( (!Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY))) && (Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY)) )//if grf is alive, target it....
						{
							Vector3 ionarrowpos = Commands->Get_Position(Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY));
							GameObject *ionarrow = Commands->Create_Object("Invisible_Object",ionarrowpos);
							Attach_Script_Once(ionarrow,"tcw_wmd_strike","6");//pass 6 for grf
						}
						else if ( (!Is_Building_Dead(Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT))) && (Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT)) )//if gpp is alive, target it....
						{
							Vector3 ionarrowpos = Commands->Get_Position(Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT));
							GameObject *ionarrow = Commands->Create_Object("Invisible_Object",ionarrowpos);
							Attach_Script_Once(ionarrow,"tcw_wmd_strike","7");//pass 7 for gpp
						}
					}

					if (!Commands->Find_Object(nodveh1id))
					{
						if (strcmp(Get_Parameter("NodVeh1"), "0") != 0)
						{
							Vector3 nodveh1pos = Get_Vector3_Parameter("NodVeh1Pos");
							float nodveh1fac = Get_Float_Parameter("NodVeh1Facing");
							GameObject *nodveh1 = Commands->Create_Object(Get_Parameter("NodVeh1"),nodveh1pos);
							Commands->Set_Facing(nodveh1,nodveh1fac);
							nodveh1id = Commands->Get_ID(nodveh1);
							Attach_Script_Once(nodveh1,"tcw_domination_vehicle","0");
						}
					}
					if (!Commands->Find_Object(nodveh2id))
					{
						if (strcmp(Get_Parameter("NodVeh2"), "0") != 0)
						{
							Vector3 nodveh2pos = Get_Vector3_Parameter("NodVeh2Pos");
							float nodveh2fac = Get_Float_Parameter("NodVeh2Facing");
							GameObject *nodveh2 = Commands->Create_Object(Get_Parameter("NodVeh2"),nodveh2pos);
							Commands->Set_Facing(nodveh2,nodveh2fac);
							nodveh2id = Commands->Get_ID(nodveh2);
							Attach_Script_Once(nodveh2,"tcw_domination_vehicle","0");
						}
					}
					if (!Commands->Find_Object(nodveh3id))
					{
						if (strcmp(Get_Parameter("NodVeh3"), "0") != 0)
						{
							Vector3 nodveh3pos = Get_Vector3_Parameter("NodVeh3Pos");
							float nodveh3fac = Get_Float_Parameter("NodVeh3Facing");
							GameObject *nodveh3 = Commands->Create_Object(Get_Parameter("NodVeh3"),nodveh3pos);
							Commands->Set_Facing(nodveh3,nodveh3fac);
							nodveh3id = Commands->Get_ID(nodveh3);
							Attach_Script_Once(nodveh3,"tcw_domination_vehicle","0");
						}
					}
					if (!Commands->Find_Object(nodveh4id))
					{
						if (strcmp(Get_Parameter("NodVeh4"), "0") != 0)
						{
							Vector3 nodveh4pos = Get_Vector3_Parameter("NodVeh4Pos");
							float nodveh4fac = Get_Float_Parameter("NodVeh4Facing");
							GameObject *nodveh4 = Commands->Create_Object(Get_Parameter("NodVeh4"),nodveh4pos);
							Commands->Set_Facing(nodveh4,nodveh4fac);
							nodveh4id = Commands->Get_ID(nodveh4);
							Attach_Script_Once(nodveh4,"tcw_domination_vehicle","0");
						}
					}
					if (!Commands->Find_Object(nodveh5id))
					{
						if (strcmp(Get_Parameter("NodVeh5"), "0") != 0)
						{
							Vector3 nodveh5pos = Get_Vector3_Parameter("NodVeh5Pos");
							float nodveh5fac = Get_Float_Parameter("NodVeh5Facing");
							GameObject *nodveh5 = Commands->Create_Object(Get_Parameter("NodVeh5"),nodveh5pos);
							Commands->Set_Facing(nodveh5,nodveh5fac);
							nodveh5id = Commands->Get_ID(nodveh5);
							Attach_Script_Once(nodveh5,"tcw_domination_vehicle","0");
						}
					}
				}
			}
			else if (owner == 1)
			{
				if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))//if node fully repaired
				{
				
					if (!Find_Object_With_Script("tcw_wmd_strike"))//if there is no existing strike
					{
						//nod buildings attacks:
						if ( (!Is_Building_Dead(Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY))) && (Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY)) )//if hand is alive, target it....
						{
							Vector3 ionarrowpos = Commands->Get_Position(Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY));
							GameObject *ionarrow = Commands->Create_Object("Invisible_Object",ionarrowpos);
							Attach_Script_Once(ionarrow,"tcw_wmd_strike","0");//pass 0 for hand
						}
						else if ( (!Is_Building_Dead(Find_Building_By_Type(0,BuildingConstants::TYPE_VEHICLE_FACTORY))) && (Find_Building_By_Type(0,BuildingConstants::TYPE_VEHICLE_FACTORY)) )//if nwf is alive, target it....
						{
							Vector3 ionarrowpos = Commands->Get_Position(Find_Building_By_Type(0,BuildingConstants::TYPE_VEHICLE_FACTORY));
							GameObject *ionarrow = Commands->Create_Object("Invisible_Object",ionarrowpos);
							Attach_Script_Once(ionarrow,"tcw_wmd_strike","1");//pass 1 for nwf
						}
						else if ( (!Is_Building_Dead(Find_Building_By_Type(0,BuildingConstants::TYPE_REFINERY))) && (Find_Building_By_Type(0,BuildingConstants::TYPE_REFINERY)) )//if nrf is alive, target it....
						{
							Vector3 ionarrowpos = Commands->Get_Position(Find_Building_By_Type(0,BuildingConstants::TYPE_REFINERY));
							GameObject *ionarrow = Commands->Create_Object("Invisible_Object",ionarrowpos);
							Attach_Script_Once(ionarrow,"tcw_wmd_strike","2");//pass 2 for nrf
						}
						else if ( (!Is_Building_Dead(Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT))) && (Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT)) )//if npp is alive, target it....
						{
							Vector3 ionarrowpos = Commands->Get_Position(Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT));
							GameObject *ionarrow = Commands->Create_Object("Invisible_Object",ionarrowpos);
							Attach_Script_Once(ionarrow,"tcw_wmd_strike","3");//pass 3 for npp
						}
					}

					if (!Commands->Find_Object(gdiveh1id))
					{
						if (strcmp(Get_Parameter("GDIVeh1"), "0") != 0)
						{
							Vector3 gdiveh1pos = Get_Vector3_Parameter("GDIVeh1Pos");
							float gdiveh1fac = Get_Float_Parameter("GDIVeh1Facing");
							GameObject *gdiveh1 = Commands->Create_Object(Get_Parameter("GDIVeh1"),gdiveh1pos);
							Commands->Set_Facing(gdiveh1,gdiveh1fac);
							gdiveh1id = Commands->Get_ID(gdiveh1);
							Attach_Script_Once(gdiveh1,"tcw_domination_vehicle","1");
						}
					}
					if (!Commands->Find_Object(gdiveh2id))
					{
						if (strcmp(Get_Parameter("GDIVeh2"), "0") != 0)
						{
							Vector3 gdiveh2pos = Get_Vector3_Parameter("GDIVeh2Pos");
							float gdiveh2fac = Get_Float_Parameter("GDIVeh2Facing");
							GameObject *gdiveh2 = Commands->Create_Object(Get_Parameter("GDIVeh2"),gdiveh2pos);
							Commands->Set_Facing(gdiveh2,gdiveh2fac);
							gdiveh2id = Commands->Get_ID(gdiveh2);
							Attach_Script_Once(gdiveh2,"tcw_domination_vehicle","1");
						}
					}
					if (!Commands->Find_Object(gdiveh3id))
					{
						if (strcmp(Get_Parameter("GDIVeh3"), "0") != 0)
						{
							Vector3 gdiveh3pos = Get_Vector3_Parameter("GDIVeh3Pos");
							float gdiveh3fac = Get_Float_Parameter("GDIVeh3Facing");
							GameObject *gdiveh3 = Commands->Create_Object(Get_Parameter("GDIVeh3"),gdiveh3pos);
							Commands->Set_Facing(gdiveh3,gdiveh3fac);
							gdiveh3id = Commands->Get_ID(gdiveh3);
							Attach_Script_Once(gdiveh3,"tcw_domination_vehicle","1");
						}
					}
					if (!Commands->Find_Object(gdiveh4id))
					{
						if (strcmp(Get_Parameter("GDIVeh4"), "0") != 0)
						{
							Vector3 gdiveh4pos = Get_Vector3_Parameter("GDIVeh4Pos");
							float gdiveh4fac = Get_Float_Parameter("GDIVeh4Facing");
							GameObject *gdiveh4 = Commands->Create_Object(Get_Parameter("GDIVeh4"),gdiveh4pos);
							Commands->Set_Facing(gdiveh4,gdiveh4fac);
							gdiveh4id = Commands->Get_ID(gdiveh4);
							Attach_Script_Once(gdiveh4,"tcw_domination_vehicle","1");
						}
					}
					if (!Commands->Find_Object(gdiveh5id))
					{
						if (strcmp(Get_Parameter("GDIVeh5"), "0") != 0)
						{
							Vector3 gdiveh5pos = Get_Vector3_Parameter("GDIVeh5Pos");
							float gdiveh5fac = Get_Float_Parameter("GDIVeh5Facing");
							GameObject *gdiveh5 = Commands->Create_Object(Get_Parameter("GDIVeh5"),gdiveh5pos);
							Commands->Set_Facing(gdiveh5,gdiveh5fac);
							gdiveh5id = Commands->Get_ID(gdiveh5);
							Attach_Script_Once(gdiveh5,"tcw_domination_vehicle","1");
						}
					}
				}
			}
		}
		else if (!allowcap)
		{
			Commands->Set_Health(obj,2.0f);
		}
	}
	else if (damage > 0.0f)
	{
		if (damage > Commands->Get_Health(obj))//if node fully destroyed
		{
			
			if (owner == -2)
			{
				Commands->Set_Player_Type(obj,-2);
				Commands->Set_Health(obj,1.1f);
			}
			else
			{
				Commands->Set_Health(obj,1.1f);
				Commands->Set_Player_Type(obj,Commands->Get_Player_Type(damager));

				if (owner == 0)
				{
					//if the node was nod, then now it is not anymore so we disable the cruiser attack gdi
					//Commands->Send_Custom_Event(obj,Commands->Find_Object(100067),1000,2,0);
					//Commands->Send_Custom_Event(obj,Commands->Find_Object(100065),1000,2,0);

					Commands->Destroy_Object(Find_Object_With_Script("tcw_wmd_strike"));
					Send_Message(255,255,255,"The Nuclear Strike was cancelled.");

					if ( (Commands->Find_Object(nodveh1id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh1id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh1id),99999.0f,"Steel",obj);		nodveh1id = 0;
					}
					if ( (Commands->Find_Object(nodveh2id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh2id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh2id),99999.0f,"Steel",obj);		nodveh2id = 0;
					}
					if ( (Commands->Find_Object(nodveh3id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh3id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh3id),99999.0f,"Steel",obj);		nodveh3id = 0;
					}
					if ( (Commands->Find_Object(nodveh4id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh4id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh4id),99999.0f,"Steel",obj);		nodveh4id = 0;
					}
					if ( (Commands->Find_Object(nodveh5id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh5id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh5id),99999.0f,"Steel",obj);		nodveh5id = 0;
					}
				}
				else if (owner == 1)
				{
					Commands->Destroy_Object(Find_Object_With_Script("tcw_wmd_strike"));
					Send_Message(255,255,255,"The Ion Cannon Strike was cancelled.");
					if ( (Commands->Find_Object(gdiveh1id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh1id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh1id),99999.0f,"Steel",obj);		gdiveh1id = 0;
					}
					if ( (Commands->Find_Object(gdiveh2id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh2id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh2id),99999.0f,"Steel",obj);		gdiveh2id = 0;
					}
					if ( (Commands->Find_Object(gdiveh3id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh3id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh3id),99999.0f,"Steel",obj);		gdiveh3id = 0;
					}
					if ( (Commands->Find_Object(gdiveh4id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh4id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh4id),99999.0f,"Steel",obj);		gdiveh4id = 0;
					}
					if ( (Commands->Find_Object(gdiveh5id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh5id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh5id),99999.0f,"Steel",obj);		gdiveh5id = 0;
					}
				}

				owner = Commands->Get_Player_Type(damager);
			}
		}
	}
}
void tcw_wmd_countdown_node::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&nodename,sizeof(nodename),1);
	Auto_Save_Variable(&owner,sizeof(owner),2);
	Auto_Save_Variable(&nodveh1id,sizeof(nodveh1id),3);
	Auto_Save_Variable(&nodveh2id,sizeof(nodveh2id),4);
	Auto_Save_Variable(&nodveh3id,sizeof(nodveh3id),5);
	Auto_Save_Variable(&nodveh4id,sizeof(nodveh4id),6);
	Auto_Save_Variable(&nodveh5id,sizeof(nodveh5id),7);
	Auto_Save_Variable(&gdiveh1id,sizeof(gdiveh1id),8);
	Auto_Save_Variable(&gdiveh2id,sizeof(gdiveh2id),9);
	Auto_Save_Variable(&gdiveh3id,sizeof(gdiveh3id),10);
	Auto_Save_Variable(&gdiveh4id,sizeof(gdiveh4id),11);
	Auto_Save_Variable(&gdiveh5id,sizeof(gdiveh5id),12);
	Auto_Save_Variable(&allowcap,sizeof(allowcap),13);
}

void tcw_wmd_strike::Created(GameObject *obj)
{
	int strike = Get_Int_Parameter("StrikeAgainst");

	//nod buildings strikes
	if (strike == 0)
	{
		Commands->Start_Timer(obj,this,60.0f,200);//target nod hand
		Send_Message(255,255,255,"ALERT: Ion Cannon Satelite moving in position targetting the Hand of Nod.");
		Commands->Create_2D_WAV_Sound("attackalert.wav");
	}
	else if (strike == 1)
	{
		Commands->Start_Timer(obj,this,60.0f,210);//target nod nwf
		Send_Message(255,255,255,"ALERT: Ion Cannon Satelite moving in position targetting the Nod Weapons Factory.");
		Commands->Create_2D_WAV_Sound("attackalert.wav");
	}
	else if (strike == 2)
	{
		Commands->Start_Timer(obj,this,60.0f,220);//target nod nrf
		Send_Message(255,255,255,"ALERT: Ion Cannon Satelite moving in position targetting the Nod Tiberium Refinery.");
		Commands->Create_2D_WAV_Sound("attackalert.wav");
	}
	else if (strike == 3)
	{
		Commands->Start_Timer(obj,this,60.0f,230);//target nod npp
		Send_Message(255,255,255,"ALERT: Ion Cannon Satelite moving in position targetting the Nod Power Plant.");
		Commands->Create_2D_WAV_Sound("attackalert.wav");
	}

	//gdi buildings strikes
	else if (strike == 4)
	{
		Commands->Start_Timer(obj,this,60.0f,240);//target gdi bar
		Send_Message(255,255,255,"ALERT: Nuclear Strike incoming at the GDI Infantry Barracks.");
		Commands->Create_2D_WAV_Sound("attackalert.wav");
	}
	else if (strike == 5)
	{
		Commands->Start_Timer(obj,this,60.0f,250);//target gdi wf
		Send_Message(255,255,255,"ALERT: Nuclear Strike incoming at the GDI War Factory.");
		Commands->Create_2D_WAV_Sound("attackalert.wav");
	}
	else if (strike == 6)
	{
		Commands->Start_Timer(obj,this,60.0f,260);//target gdi grf
		Send_Message(255,255,255,"ALERT: Nuclear Strike incoming at the GDI Tiberium Refinery.");
		Commands->Create_2D_WAV_Sound("attackalert.wav");
	}
	else if (strike == 7)
	{
		Commands->Start_Timer(obj,this,60.0f,270);//target gdi gpp
		Send_Message(255,255,255,"ALERT: Nuclear Strike incoming at the GDI Power Plant.");
		Commands->Create_2D_WAV_Sound("attackalert.wav");
	}
}
void tcw_wmd_strike::Timer_Expired(GameObject *obj, int number)
{
	//initiate strikes nod buildings
	if (number == 200)//init ion hand
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_wmd_countdown_node"),1111,1,0);//reset the capture point
		Commands->Start_Timer(obj,this,10.5f,201);//initate after party
		Vector3 nukepos; Commands->Get_Position(Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY));
		GameObject *nuke = Commands->Create_Object("Pre Ion Strike",nukepos);
		Commands->Set_Facing(nuke,0.0f);
	}
	else if (number == 201)//kill hand and play afterparty
	{
		Commands->Apply_Damage(Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY),99999.0f,"Steel",obj);//kill the hand
		Vector3 nukepos; Commands->Get_Position(Find_Building_By_Type(0,BuildingConstants::TYPE_SOLDIER_FACTORY));
		GameObject *nuke = Commands->Create_Object("Post Ion Strike",nukepos);
		Commands->Set_Facing(nuke,0.0f);
		Destroy_Script();
	}

	else if (number == 210)//init ion nod wf
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_wmd_countdown_node"),1111,1,0);//reset the capture point
		Commands->Start_Timer(obj,this,10.5f,211);//initate after party
		Vector3 nukepos = Commands->Get_Position(Find_Building_By_Type(0,BuildingConstants::TYPE_VEHICLE_FACTORY));
		GameObject *nuke = Commands->Create_Object("Pre Ion Strike",nukepos);
		Commands->Set_Facing(nuke,0.0f);
	}
	else if (number == 211)//kill hand and play afterparty
	{
		Commands->Apply_Damage(Find_Building_By_Type(0,BuildingConstants::TYPE_VEHICLE_FACTORY),999999.0f,"Steel",obj);//kill the hand
		Vector3 nukepos = Commands->Get_Position(Find_Building_By_Type(0,BuildingConstants::TYPE_VEHICLE_FACTORY));
		GameObject *nuke = Commands->Create_Object("Post Ion Strike",nukepos);
		Commands->Set_Facing(nuke,0.0f);
		Destroy_Script();
	}

	else if (number == 220)//init ion nod nrf
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_wmd_countdown_node"),1111,1,0);//reset the capture point
		Commands->Start_Timer(obj,this,10.5f,221);//initate after party
		Vector3 nukepos = Commands->Get_Position(Find_Building_By_Type(0,BuildingConstants::TYPE_REFINERY));
		GameObject *nuke = Commands->Create_Object("Pre Ion Strike",nukepos);
		Commands->Set_Facing(nuke,0.0f);
	}
	else if (number == 221)//kill nrf and play afterparty
	{
		Commands->Apply_Damage(Find_Building_By_Type(0,BuildingConstants::TYPE_REFINERY),999999.0f,"Steel",obj);//kill the nrf
		Vector3 nukepos = Commands->Get_Position(Find_Building_By_Type(0,BuildingConstants::TYPE_REFINERY));
		GameObject *nuke = Commands->Create_Object("Post Ion Strike",nukepos);
		Commands->Set_Facing(nuke,0.0f);
		Destroy_Script();
	}

	else if (number == 230)//init ion nod nrf
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_wmd_countdown_node"),1111,1,0);//reset the capture point
		Commands->Start_Timer(obj,this,10.5f,231);//initate after party
		Vector3 nukepos = Commands->Get_Position(Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT));
		GameObject *nuke = Commands->Create_Object("Pre Ion Strike",nukepos);
		Commands->Set_Facing(nuke,0.0f);
	}
	else if (number == 231)//kill npp and play afterparty
	{
		Commands->Apply_Damage(Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT),99999.0f,"Steel",obj);//kill the npp
		Vector3 nukepos = Commands->Get_Position(Find_Building_By_Type(0,BuildingConstants::TYPE_POWER_PLANT));
		GameObject *nuke = Commands->Create_Object("Post Ion Strike",nukepos);
		Commands->Set_Facing(nuke,0.0f);
		Destroy_Script();
	}

	//initiate strikes gdi buildings
	if (number == 240)//init nuke bar
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_wmd_countdown_node"),1111,1,0);//reset the capture point
		Commands->Start_Timer(obj,this,10.5f,241);//initate after party
		Vector3 nukepos; Commands->Get_Position(Find_Building_By_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY));
		GameObject *nuke = Commands->Create_Object("Nuke_Strike_Pre",nukepos);
		Commands->Set_Facing(nuke,0.0f);
	}
	else if (number == 241)//kill bar and play afterparty
	{
		Commands->Apply_Damage(Find_Building_By_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY),99999.0f,"Steel",obj);//kill the bar
		Vector3 nukepos; Commands->Get_Position(Find_Building_By_Type(1,BuildingConstants::TYPE_SOLDIER_FACTORY));
		GameObject *nuke = Commands->Create_Object("Nuke_Strike_Post",nukepos);
		Commands->Set_Facing(nuke,0.0f);
		Commands->Start_Timer(obj,this,5.0f,300);
	}

	else if (number == 250)//init nuke gdi wf
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_wmd_countdown_node"),1111,1,0);//reset the capture point
		Commands->Start_Timer(obj,this,10.5f,251);//initate after party
		Vector3 nukepos = Commands->Get_Position(Find_Building_By_Type(1,BuildingConstants::TYPE_VEHICLE_FACTORY));
		GameObject *nuke = Commands->Create_Object("Nuke_Strike_Pre",nukepos);
		Commands->Set_Facing(nuke,0.0f);
	}
	else if (number == 251)//kill hand and play afterparty
	{
		Commands->Apply_Damage(Find_Building_By_Type(1,BuildingConstants::TYPE_VEHICLE_FACTORY),999999.0f,"Steel",obj);//kill the wf
		Vector3 nukepos = Commands->Get_Position(Find_Building_By_Type(1,BuildingConstants::TYPE_VEHICLE_FACTORY));
		GameObject *nuke = Commands->Create_Object("Nuke_Strike_Post",nukepos);
		Commands->Set_Facing(nuke,0.0f);
		Commands->Start_Timer(obj,this,5.0f,300);
	}

	else if (number == 260)//init nuke gdi ref
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_wmd_countdown_node"),1111,1,0);//reset the capture point
		Commands->Start_Timer(obj,this,10.5f,261);//initate after party
		Vector3 nukepos = Commands->Get_Position(Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY));
		GameObject *nuke = Commands->Create_Object("Nuke_Strike_Pre",nukepos);
		Commands->Set_Facing(nuke,0.0f);
	}
	else if (number == 261)//kill gref and play afterparty
	{
		Commands->Apply_Damage(Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY),999999.0f,"Steel",obj);//kill the gref
		Vector3 nukepos = Commands->Get_Position(Find_Building_By_Type(1,BuildingConstants::TYPE_REFINERY));
		GameObject *nuke = Commands->Create_Object("Nuke_Strike_Post",nukepos);
		Commands->Set_Facing(nuke,0.0f);
		Commands->Start_Timer(obj,this,5.0f,300);
	}

	else if (number == 270)//init nuke gdi pp
	{
		Commands->Send_Custom_Event(obj,Find_Object_With_Script("tcw_wmd_countdown_node"),1111,1,0);//reset the capture point
		Commands->Start_Timer(obj,this,10.5f,271);//initate after party
		Vector3 nukepos = Commands->Get_Position(Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT));
		GameObject *nuke = Commands->Create_Object("Nuke_Strike_Pre",nukepos);
		Commands->Set_Facing(nuke,0.0f);
	}
	else if (number == 271)//kill gpp and play afterparty
	{
		Commands->Apply_Damage(Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT),99999.0f,"Steel",obj);//kill the gpp
		Vector3 nukepos = Commands->Get_Position(Find_Building_By_Type(1,BuildingConstants::TYPE_POWER_PLANT));
		GameObject *nuke = Commands->Create_Object("Nuke_Strike_Post",nukepos);
		Commands->Set_Facing(nuke,0.0f);
		Commands->Start_Timer(obj,this,5.0f,300);
	}
	else if (number == 300)
	{
		Commands->Destroy_Object(Find_Object_By_Preset(2,"Nuke_Strike_Post"));
		Destroy_Script();
	}
}



ScriptRegistrant<tcw_wmd_strike> tcw_wmd_strike_Registrant("tcw_wmd_strike","StrikeAgainst=0:int");
ScriptRegistrant<tcw_wmd_countdown_node> tcw_wmd_countdown_node_Registrant("tcw_wmd_countdown_node","Node_Name=North:string,NodVeh1=bla:string,NodVeh1Pos=0:vector3,NodVeh1Facing=0:float,NodVeh2=bla:string,NodVeh2Pos=0:vector3,NodVeh2Facing=0:float,NodVeh3=bla:string,NodVeh3Pos=0:vector3,NodVeh3Facing=0:float,NodVeh4=bla:string,NodVeh4Pos=0:vector3,NodVeh4Facing=0:float,NodVeh5=bla:string,NodVeh5Pos=0:vector3,NodVeh5Facing=0:float,GDIVeh1=bla:string,GDIVeh1Pos=0:vector3,GDIVeh1Facing=0:float,GDIVeh2=bla:string,GDIVeh2Pos=0:vector3,GDIVeh2Facing=0:float,GDIVeh3=bla:string,GDIVeh3Pos=0:vector3,GDIVeh3Facing=0:float,GDIVeh4=bla:string,GDIVeh4Pos=0:vector3,GDIVeh4Facing=0:float,GDIVeh5=bla:string,GDIVeh5Pos=0:vector3,GDIVeh5Facing=0:float");
ScriptRegistrant<tcw_domination_vehicle> tcw_domination_vehicle_Registrant("tcw_domination_vehicle","Team=0:int");
ScriptRegistrant<tcw_domination_node> tcw_domination_node_Registrant("tcw_domination_node","Node_Name=North:string,NodVeh1=bla:string,NodVeh1Pos=0:vector3,NodVeh1Facing=0:float,NodVeh2=bla:string,NodVeh2Pos=0:vector3,NodVeh2Facing=0:float,NodVeh3=bla:string,NodVeh3Pos=0:vector3,NodVeh3Facing=0:float,NodVeh4=bla:string,NodVeh4Pos=0:vector3,NodVeh4Facing=0:float,NodVeh5=bla:string,NodVeh5Pos=0:vector3,NodVeh5Facing=0:float,GDIVeh1=bla:string,GDIVeh1Pos=0:vector3,GDIVeh1Facing=0:float,GDIVeh2=bla:string,GDIVeh2Pos=0:vector3,GDIVeh2Facing=0:float,GDIVeh3=bla:string,GDIVeh3Pos=0:vector3,GDIVeh3Facing=0:float,GDIVeh4=bla:string,GDIVeh4Pos=0:vector3,GDIVeh4Facing=0:float,GDIVeh5=bla:string,GDIVeh5Pos=0:vector3,GDIVeh5Facing=0:float,SpawnPad1ID=0:int,SpawnPad2ID=0:int,SpawnPad3ID=0:int,SpawnPad4ID=0:int");
ScriptRegistrant<tcw_domination_pointgen> tcw_domination_pointgen_Registrant("tcw_domination_pointgen","Team=0:int");
ScriptRegistrant<tcw_domination_control> tcw_domination_control_Registrant("tcw_domination_control","Points_Max=10:int");