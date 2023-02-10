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
#include "tcw_frost.h"
#include "GameObjManager.h"
#include "SoldierGameObj.h"
#include "VehicleGameObj.h"

//0=nod 1=gdi 2=neu current animations for spawnpads

void tcw_frost_node::Created(GameObject *obj)
{
	spawnpad1 = Get_Int_Parameter("SpawnPad1ID");
	spawnpad2 = Get_Int_Parameter("SpawnPad2ID");
	Commands->Start_Timer(obj,this,60.0f,100100);//start spawncheck for vehicles for nod
	Commands->Start_Timer(obj,this,60.0f,100101);//start spawncheck for vehicles for gdi
	int nodeteam = Get_Int_Parameter("PreTeam");
	if (nodeteam == 0)
	{
		Attach_Script_Once(obj,"tcw_domination_pointgen","0");
		owner = 0;
		Commands->Set_Player_Type(obj,0);
		Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,0,0,false);
		nodename = Get_Parameter("Node_Name");
		owner = 0;
		nodveh1id = 0;	nodveh2id = 0;
		gdiveh1id = 0;	gdiveh2id = 0;
		trigger_neutral_damage = true;
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
		if (Commands->Find_Object(spawnpad1))
		{
			Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false);
			Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),0);
		}
		if (Commands->Find_Object(spawnpad2))
		{
			Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false);
			Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),0);
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
	}
	else if (nodeteam == 1)
	{
		Attach_Script_Once(obj,"tcw_domination_pointgen","1");
		owner = 1;
		Commands->Set_Player_Type(obj,1);
		Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,1,1,false);
		nodename = Get_Parameter("Node_Name");
		owner = 1;
		nodveh1id = 0;	nodveh2id = 0;
		gdiveh1id = 0;	gdiveh2id = 0;
		trigger_neutral_damage = true;
		Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
		if (Commands->Find_Object(spawnpad1))
		{
			Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false);
			Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),1);
		}
		if (Commands->Find_Object(spawnpad2))
		{
			Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false);
			Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),1);
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
	}
	else
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
		nodename = Get_Parameter("Node_Name");
		owner = -2;
		nodveh1id = 0;	nodveh2id = 0;
		gdiveh1id = 0;	gdiveh2id = 0;
		trigger_neutral_damage = true;
	}
}
void tcw_frost_node::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if ((damage < 0.0f) && (Commands->Get_Player_Type(obj) == -2))
	{
		if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))
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
					Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),0);
					Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false);
				}
				if (Commands->Find_Object(spawnpad2))
				{
					Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false);
					Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),0);
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
					Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),1);
				}
				if (Commands->Find_Object(spawnpad2))
				{
					Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false);
					Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),1);
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

				char capturemsg[250];	sprintf(capturemsg,"GDI captured the %s Control Point.",nodename);	Send_Message(255,255,255,capturemsg);
				Create_2D_WAV_Sound_Team("ControlPointCaptured_GDI.wav",1); 

				if (Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
				{
					Commands->Give_Money(damager,500.0f,true);
				}
			}
		}
	}
	else if (damage > 0.0f)
	{
		if (damage > Commands->Get_Health(obj))
		{
			Commands->Set_Health(obj,1.1f);
			Commands->Set_Player_Type(obj,-2);
			if (trigger_neutral_damage)
			{
				trigger_neutral_damage = false;
				Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),-2);
				Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),-2);
				Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
				Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);
				Remove_Script(obj,"tcw_domination_pointgen");
				Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);

				if (owner == 0)
				{
					Create_2D_WAV_Sound_Team("ControlPointLost_Nod.wav",0);
					
					char lostcp[512];
					sprintf(lostcp,"Control Point Lost: %s",nodename);
					Send_Message_Team(0,255,255,255,lostcp);
					if ( (Commands->Find_Object(nodveh1id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh1id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh1id),99999.0f,"Steel",obj);		nodveh1id = 0;
					}
					if ( (Commands->Find_Object(nodveh2id)) && (!Get_Vehicle_Driver(Commands->Find_Object(nodveh2id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(nodveh2id),99999.0f,"Steel",obj);		nodveh2id = 0;
					}
				}
				else if (owner == 1)
				{
					Create_2D_WAV_Sound_Team("ControlPointLost_GDI.wav",1);
					char lostcp[512];
					sprintf(lostcp,"Control Point Lost: %s",nodename);
					Send_Message_Team(1,255,255,255,lostcp);
					if ( (Commands->Find_Object(gdiveh1id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh1id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh1id),99999.0f,"Steel",obj);		gdiveh1id = 0;
					}
					if ( (Commands->Find_Object(gdiveh2id)) && (!Get_Vehicle_Driver(Commands->Find_Object(gdiveh2id))) )
					{
						Commands->Apply_Damage(Commands->Find_Object(gdiveh2id),99999.0f,"Steel",obj);		gdiveh2id = 0;
					}
				}
				owner = -2;
			}
		}
	}
}
void tcw_frost_node::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100100) // spawn vehicles check nod
	{
		if (owner == 0)//if this point is still controlled by nod proceed
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
		}
		Commands->Start_Timer(obj,this,60.0f,100100);//loop nod check
	}
	else if (number == 100101) // spawn vehicles check gdi
	{
		if (owner == 1)//if this point is still controlled by gdi proceed
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
		}
		Commands->Start_Timer(obj,this,60.0f,100101);//loop gdi check
	}
}
void tcw_frost_node::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&nodename,sizeof(nodename),1);
	Auto_Save_Variable(&owner,sizeof(owner),2);
	Auto_Save_Variable(&nodveh1id,sizeof(nodveh1id),3);
	Auto_Save_Variable(&nodveh2id,sizeof(nodveh2id),4);
	Auto_Save_Variable(&gdiveh1id,sizeof(gdiveh1id),5);
	Auto_Save_Variable(&gdiveh2id,sizeof(gdiveh2id),6);
	Auto_Save_Variable(&spawnpad1,sizeof(spawnpad1),7);
	Auto_Save_Variable(&spawnpad2,sizeof(spawnpad2),8);
}

ScriptRegistrant<tcw_frost_node> tcw_frost_node_Registrant("tcw_frost_node","PreTeam=-2:int,Node_Name=North:string,NodVeh1=bla:string,NodVeh1Pos=0:vector3,NodVeh1Facing=0:float,NodVeh2=bla:string,NodVeh2Pos=0:vector3,NodVeh2Facing=0:float,GDIVeh1=bla:string,GDIVeh1Pos=0:vector3,GDIVeh1Facing=0:float,GDIVeh2=bla:string,GDIVeh2Pos=0:vector3,GDIVeh2Facing=0:float,SpawnPad1ID=0:int,SpawnPad2ID=0:int");