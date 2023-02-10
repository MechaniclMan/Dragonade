/*	Renegade Scripts.dll
	Copyright 2013 UltraAOW.COM by zunnie

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
#include "z_domination.h"

void AOW_Domination_Control::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,120.0f,123439348);//spam score
	triggerendmsg = true;
	points_gdi = 0;		points_nod = 0;
	points_max = Get_Int_Parameter("Points_Max");
}
void AOW_Domination_Control::Custom(GameObject *obj, int message, int param, GameObject *sender)
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
				sprintf(status,"msg GDI: %d points -- Nod: %d points -- Required: %d points",points_gdi,points_nod,points_max);
				Console_Input(status);
			}
		}
	}
}
void AOW_Domination_Control::Timer_Expired(GameObject *obj, int number)
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
void AOW_Domination_Control::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&points_gdi,sizeof(points_gdi),1);
	Auto_Save_Variable(&points_nod,sizeof(points_nod),2);
	Auto_Save_Variable(&points_max,sizeof(points_max),3);
	Auto_Save_Variable(&triggerendmsg,sizeof(triggerendmsg),4);
}

void AOW_Domination_Pointgen::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	Commands->Start_Timer(obj,this,1.0f,303);
}
void AOW_Domination_Pointgen::Timer_Expired(GameObject *obj, int number)
{
	if (number == 303)
	{
		if (team == 0)
		{
			Commands->Send_Custom_Event(obj,Find_Object_With_Script("AOW_Domination_Control"),1000,0,0);
		}
		else if (team == 1)
		{
			Commands->Send_Custom_Event(obj,Find_Object_With_Script("AOW_Domination_Control"),1000,1,0);
		}
		Commands->Start_Timer(obj,this,1.0f,303);
	}
}
void AOW_Domination_Pointgen::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&team,sizeof(team),1);
}


void AOW_Domination_Vehicle::Custom(GameObject *obj, int message, int param, GameObject *sender)
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
void AOW_Domination_Vehicle::Timer_Expired(GameObject *obj, int number)
{
	if (number == 509482)
	{
		Force_Occupant_Exit(obj,0);
	}
}

//0=nod 1=gdi 2=neu current animations for spawnpads
void AOW_Nodewar_Node::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,60.0f,100100);//start spawncheck for vehicles for nod
	Commands->Start_Timer(obj,this,60.0f,100101);//start spawncheck for vehicles for gdi
	int nodeteam = Get_Int_Parameter("PreTeam");
	if (nodeteam == 0)
	{
		Attach_Script_Once(obj,"AOW_Domination_Pointgen","0");
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
			Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false); Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),0);
		}
		if (Commands->Find_Object(spawnpad2))
		{
			Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false); Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),0);
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
				Attach_Script_Once(nodveh1,"AOW_Domination_Vehicle","0");
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
				Attach_Script_Once(nodveh2,"AOW_Domination_Vehicle","0");
			}
		}
	}
	else if (nodeteam == 1)
	{
		Attach_Script_Once(obj,"AOW_Domination_Pointgen","1");
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
			Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false); Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),1);
		}
		if (Commands->Find_Object(spawnpad2))
		{
			Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false); Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),1);
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
				Attach_Script_Once(gdiveh1,"AOW_Domination_Vehicle","1");
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
				Attach_Script_Once(gdiveh2,"AOW_Domination_Vehicle","1");
			}
		}
	}
	else
	{
		Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,2,2,false);
		spawnpad1 = Get_Int_Parameter("SpawnPad1ID");
		if (Commands->Find_Object(spawnpad1))
		{
			Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);  Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),-2);
		}
		spawnpad2 = Get_Int_Parameter("SpawnPad2ID");
		if (Commands->Find_Object(spawnpad2))
		{
			Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false);  Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),-2);
		}
		nodename = Get_Parameter("Node_Name");
		owner = -2;
		nodveh1id = 0;	nodveh2id = 0;
		gdiveh1id = 0;	gdiveh2id = 0;
		trigger_neutral_damage = true;
	}
}
void AOW_Nodewar_Node::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if (damage < 0.0f)
	{
		if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))
		{
			trigger_neutral_damage = true;
			int team = Commands->Get_Player_Type(damager);
			if (team == 0)
			{
				if (Commands->Get_Player_Type(obj) == -2)
				{
					char params[250]; sprintf(params,"%d",team);
					Attach_Script_Once(obj,"AOW_Domination_Pointgen",params);
					owner = 0;
					Commands->Set_Player_Type(obj,0);

					Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,0,0,false);
					if (Commands->Find_Object(spawnpad1))
					{
						Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false); Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),0);
					}
					if (Commands->Find_Object(spawnpad2))
					{
						Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,0,0,false); Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),0);
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
							Attach_Script_Once(nodveh1,"AOW_Domination_Vehicle","0");
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
							Attach_Script_Once(nodveh2,"AOW_Domination_Vehicle","0");
						}
					}

					char capturemsg[250];	sprintf(capturemsg,"Nod captured the %s Control Point.",nodename);	Send_Message(255,255,255,capturemsg);
					Create_2D_WAV_Sound_Team("ControlPointCaptured_Nod.wav",0); 

					if (Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
					{
						Commands->Give_Money(damager,500.0f,true);
					}
				}
			}
			else if (team == 1)
			{
				if (Commands->Get_Player_Type(obj) == -2)
				{
					char params[250]; sprintf(params,"%d",team);
					Attach_Script_Once(obj,"AOW_Domination_Pointgen",params);
					owner = 1;
					Commands->Set_Player_Type(obj,1);

					Commands->Set_Animation(obj,"DOM_NODE.DOM_NODE",false,0,1,1,false);
					if (Commands->Find_Object(spawnpad1))
					{
						Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false); Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),1);
					}
					if (Commands->Find_Object(spawnpad2))
					{
						Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,1,1,false); Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),1);
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
							Attach_Script_Once(gdiveh1,"AOW_Domination_Vehicle","1");
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
							Attach_Script_Once(gdiveh2,"AOW_Domination_Vehicle","1");
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
				Commands->Set_Animation(Commands->Find_Object(spawnpad1),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false); Commands->Set_Player_Type(Commands->Find_Object(spawnpad1),-2);
				Commands->Set_Animation(Commands->Find_Object(spawnpad2),"NEU_PLATFRM_2.NEU_PLATFRM_2",false,0,2,2,false); Commands->Set_Player_Type(Commands->Find_Object(spawnpad2),-2);
				Remove_Script(obj,"AOW_Domination_Pointgen");
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
				}
				owner = -2;
			}
		}
	}
}
void AOW_Nodewar_Node::Timer_Expired(GameObject *obj, int number)
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
					Attach_Script_Once(nodveh1,"AOW_Domination_Vehicle","0");
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
					Attach_Script_Once(nodveh2,"AOW_Domination_Vehicle","0");
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
					Attach_Script_Once(gdiveh1,"AOW_Domination_Vehicle","1");
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
					Attach_Script_Once(gdiveh2,"AOW_Domination_Vehicle","1");
				}
			}
		}
		Commands->Start_Timer(obj,this,60.0f,100101);//loop gdi check
	}
}
void AOW_Nodewar_Node::Register_Auto_Save_Variables()
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

void AOW_Prevent_Destruction::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
}


ScriptRegistrant<AOW_Prevent_Destruction> AOW_Prevent_Destruction_Registrant("AOW_Prevent_Destruction","");
ScriptRegistrant<AOW_Nodewar_Node> AOW_Nodewar_Node_Registrant("AOW_Nodewar_Node","PreTeam=-2:int,Node_Name=North:string,NodVeh1=bla:string,NodVeh1Pos=0:vector3,NodVeh1Facing=0:float,NodVeh2=bla:string,NodVeh2Pos=0:vector3,NodVeh2Facing=0:float,GDIVeh1=bla:string,GDIVeh1Pos=0:vector3,GDIVeh1Facing=0:float,GDIVeh2=bla:string,GDIVeh2Pos=0:vector3,GDIVeh2Facing=0:float,SpawnPad1ID=0:int,SpawnPad2ID=0:int");
ScriptRegistrant<AOW_Domination_Vehicle> AOW_Domination_Vehicle_Registrant("AOW_Domination_Vehicle","Team=0:int");
ScriptRegistrant<AOW_Domination_Pointgen> AOW_Domination_Pointgen_Registrant("AOW_Domination_Pointgen","Team=0:int");
ScriptRegistrant<AOW_Domination_Control> AOW_Domination_Control_Registrant("AOW_Domination_Control","Points_Max=10:int");