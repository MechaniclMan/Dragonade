/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#include "general.h"

#include "scripts.h"
#include "jfwobj.h"
#include "engine.h"
#include "ScriptZoneGameObj.h"
#include "ArmorWarheadManager.h"
#include "MoveablePhysClass.h"
#include "VehicleGameObj.h"
#include "SimpleGameObj.h"
#include "GameObjManager.h"
struct TimerParams {
	int number;
	float time;
	bool repeat;
};
typedef TimerParams *PTimerParams;

void JFW_Start_Timer::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 1)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	TimerParams z;
	int TimerObject;
	int TimerMessage;
	TimerObject = Get_Int_Parameter("Timer_Object");
	z.number = Get_Int_Parameter("Timer_Number");
	z.time = Get_Float_Parameter("Timer_Time");
	z.repeat = Get_Bool_Parameter("Repeat");
	TimerMessage = Get_Int_Parameter("Timer_Custom");
	GameObject *TimerObj = Commands->Find_Object(TimerObject);
	Commands->Send_Custom_Event(obj,TimerObj,TimerMessage,0,0);
}

void JFW_Start_Timer::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 4)
	{
		return;
	}
	TimerParams z;
	int TimerObject;
	int TimerMessage;
	TimerObject = Get_Int_Parameter("Timer_Object");
	z.number = Get_Int_Parameter("Timer_Number");
	z.time = Get_Float_Parameter("Timer_Time");
	z.repeat = Get_Bool_Parameter("Repeat");
	TimerMessage = Get_Int_Parameter("Timer_Custom");
	GameObject *TimerObj = Commands->Find_Object(TimerObject);
	Commands->Send_Custom_Event(obj,TimerObj,TimerMessage,0,0);
}

void JFW_Start_Timer::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 3)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != type)
	{
		return;
	}
	TimerParams z;
	int TimerObject;
	int TimerMessage;
	TimerObject = Get_Int_Parameter("Timer_Object");
	z.number = Get_Int_Parameter("Timer_Number");
	z.time = Get_Float_Parameter("Timer_Time");
	z.repeat = Get_Bool_Parameter("Repeat");
	TimerMessage = Get_Int_Parameter("Timer_Custom");
	GameObject *TimerObj = Commands->Find_Object(TimerObject);
	Commands->Send_Custom_Event(obj,TimerObj,TimerMessage,0,0);
}

void JFW_Start_Timer::Killed(GameObject *obj,GameObject *killer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 2)
	{
		return;
	}
	TimerParams z;
	int TimerObject;
	int TimerMessage;
	TimerObject = Get_Int_Parameter("Timer_Object");
	z.number = Get_Int_Parameter("Timer_Number");
	z.time = Get_Float_Parameter("Timer_Time");
	z.repeat = Get_Bool_Parameter("Repeat");
	TimerMessage = Get_Int_Parameter("Timer_Custom");
	GameObject *TimerObj = Commands->Find_Object(TimerObject);
	Commands->Send_Custom_Event(obj,TimerObj,TimerMessage,0,0);
}

void JFW_Start_Timer::Created(GameObject *obj)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x)
	{
		return;
	}
	TimerParams z;
	int TimerObject;
	int TimerMessage;
	TimerObject = Get_Int_Parameter("Timer_Object");
	z.number = Get_Int_Parameter("Timer_Number");
	z.time = Get_Float_Parameter("Timer_Time");
	z.repeat = Get_Bool_Parameter("Repeat");
	TimerMessage = Get_Int_Parameter("Timer_Custom");
	GameObject *TimerObj = Commands->Find_Object(TimerObject);
	Commands->Send_Custom_Event(obj,TimerObj,TimerMessage,0,0);
}

void JFW_Add_Objective::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 1)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	int objective_num;
	int objective_type;
	int title_id;
	int unknown;
	char *soundname;
	int description_id;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_type = Get_Int_Parameter("Objective_Type");
	title_id = Get_Int_Parameter("Title_ID");
	unknown = Get_Int_Parameter("Unknown");
	soundname = (char *)Get_Parameter("Sound_Name");
	description_id = Get_Int_Parameter("Description_ID");
	int trigger = Get_Int_Parameter("Trigger");
	Commands->Add_Objective(objective_num,objective_type,unknown,title_id,soundname,description_id);
	if (trigger)
	{
		Commands->Send_Custom_Event(obj,obj,trigger,0,0);
	}
}

void JFW_Add_Objective::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 4)
	{
		return;
	}
	int objective_num;
	int objective_type;
	int title_id;
	int unknown;
	char *soundname;
	int description_id;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_type = Get_Int_Parameter("Objective_Type");
	title_id = Get_Int_Parameter("Title_ID");
	unknown = Get_Int_Parameter("Unknown");
	soundname = (char *)Get_Parameter("Sound_Name");
	description_id = Get_Int_Parameter("Description_ID");
	int trigger = Get_Int_Parameter("Trigger");
	Commands->Add_Objective(objective_num,objective_type,unknown,title_id,soundname,description_id);
	if (trigger)
	{
		Commands->Send_Custom_Event(obj,obj,trigger,0,0);
	}
}

void JFW_Add_Objective::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int x;
	TimerParams t;
	x = Get_Int_Parameter("Timer_Custom");
	if (x == type)
	{
		t = *(PTimerParams)param;
		repeat = t.repeat;
		timertime = t.time;
		timernumber = t.number;
		Commands->Start_Timer(obj,this,t.time,t.number);
	}
	x = Get_Int_Parameter("Type");
	if (x != 3)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != type)
	{
		return;
	}
	int objective_num;
	int objective_type;
	int title_id;
	int unknown;
	char *soundname;
	int description_id;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_type = Get_Int_Parameter("Objective_Type");
	title_id = Get_Int_Parameter("Title_ID");
	unknown = Get_Int_Parameter("Unknown");
	soundname = (char *)Get_Parameter("Sound_Name");
	description_id = Get_Int_Parameter("Description_ID");
	int trigger = Get_Int_Parameter("Trigger");
	Commands->Add_Objective(objective_num,objective_type,unknown,title_id,soundname,description_id);
	if (trigger)
	{
		Commands->Send_Custom_Event(obj,obj,trigger,0,0);
	}
}

void JFW_Add_Objective::Killed(GameObject *obj,GameObject *killer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 2)
	{
		return;
	}
	int objective_num;
	int objective_type;
	int title_id;
	int unknown;
	char *soundname;
	int description_id;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_type = Get_Int_Parameter("Objective_Type");
	title_id = Get_Int_Parameter("Title_ID");
	unknown = Get_Int_Parameter("Unknown");
	soundname = (char *)Get_Parameter("Sound_Name");
	description_id = Get_Int_Parameter("Description_ID");
	int trigger = Get_Int_Parameter("Trigger");
	Commands->Add_Objective(objective_num,objective_type,unknown,title_id,soundname,description_id);
	if (trigger)
	{
		Commands->Send_Custom_Event(obj,obj,trigger,0,0);
	}
}

void JFW_Add_Objective::Created(GameObject *obj)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x)
	{
		return;
	}
	int objective_num;
	int objective_type;
	int title_id;
	int unknown;
	char *soundname;
	int description_id;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_type = Get_Int_Parameter("Objective_Type");
	title_id = Get_Int_Parameter("Title_ID");
	unknown = Get_Int_Parameter("Unknown");
	soundname = (char *)Get_Parameter("Sound_Name");
	description_id = Get_Int_Parameter("Description_ID");
	int trigger = Get_Int_Parameter("Trigger");
	Commands->Add_Objective(objective_num,objective_type,unknown,title_id,soundname,description_id);
	if (trigger)
	{
		Commands->Send_Custom_Event(obj,obj,trigger,0,0);
	}
}

void JFW_Add_Objective::Timer_Expired(GameObject *obj,int number)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 5)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != number)
	{
		return;
	}
	if (repeat)
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	int objective_num;
	int objective_type;
	int title_id;
	int unknown;
	char *soundname;
	int description_id;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_type = Get_Int_Parameter("Objective_Type");
	title_id = Get_Int_Parameter("Title_ID");
	unknown = Get_Int_Parameter("Unknown");
	soundname = (char *)Get_Parameter("Sound_Name");
	description_id = Get_Int_Parameter("Description_ID");
	int trigger = Get_Int_Parameter("Trigger");
	Commands->Add_Objective(objective_num,objective_type,unknown,title_id,soundname,description_id);
	if (trigger)
	{
		Commands->Send_Custom_Event(obj,obj,trigger,0,0);
	}
}

void JFW_Add_Objective::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&repeat,1,1);
	Auto_Save_Variable(&timertime,4,2);
	Auto_Save_Variable(&timernumber,4,3);
}

void JFW_Remove_Objective::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 1)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	x = Get_Int_Parameter("Objective_Num");
	Commands->Remove_Objective(x);
}

void JFW_Remove_Objective::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 4)
	{
		return;
	}
	x = Get_Int_Parameter("Objective_Num");
	Commands->Remove_Objective(x);
}

void JFW_Remove_Objective::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int x;
	TimerParams t;
	x = Get_Int_Parameter("Timer_Custom");
	if (x == type)
	{
		t = *(PTimerParams)param;
		repeat = t.repeat;
		timertime = t.time;
		timernumber = t.number;
		Commands->Start_Timer(obj,this,t.time,t.number);
	}
	x = Get_Int_Parameter("Type");
	if (x != 3)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != type)
	{
		return;
	}
	x = Get_Int_Parameter("Objective_Num");
	Commands->Remove_Objective(x);
}

void JFW_Remove_Objective::Killed(GameObject *obj,GameObject *killer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 2)
	{
		return;
	}
	x = Get_Int_Parameter("Objective_Num");
	Commands->Remove_Objective(x);
}

void JFW_Remove_Objective::Created(GameObject *obj)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x)
	{
		return;
	}
	x = Get_Int_Parameter("Objective_Num");
	Commands->Remove_Objective(x);
}

void JFW_Remove_Objective::Timer_Expired(GameObject *obj,int number)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 5)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != number)
	{
		return;
	}
	if (repeat)
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	x = Get_Int_Parameter("Objective_Num");
	Commands->Remove_Objective(x);
}

void JFW_Remove_Objective::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&repeat,1,1);
	Auto_Save_Variable(&timertime,4,2);
	Auto_Save_Variable(&timernumber,4,3);
}

void JFW_Set_Objective_Status::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 1)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	int objective_num;
	int objective_status;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_status = Get_Int_Parameter("Status");
	Commands->Set_Objective_Status(objective_num,objective_status);
}

void JFW_Set_Objective_Status::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 4)
	{
		return;
	}
	int objective_num;
	int objective_status;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_status = Get_Int_Parameter("Status");
	Commands->Set_Objective_Status(objective_num,objective_status);
}

void JFW_Set_Objective_Status::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int x;
	TimerParams t;
	x = Get_Int_Parameter("Timer_Custom");
	if (x == type)
	{
		t = *(PTimerParams)param;
		repeat = t.repeat;
		timertime = t.time;
		timernumber = t.number;
		Commands->Start_Timer(obj,this,t.time,t.number);
	}
	x = Get_Int_Parameter("Type");
	if (x != 3)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != type)
	{
		return;
	}
	int objective_num;
	int objective_status;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_status = Get_Int_Parameter("Status");
	Commands->Set_Objective_Status(objective_num,objective_status);
}

void JFW_Set_Objective_Status::Killed(GameObject *obj,GameObject *killer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 2)
	{
		return;
	}
	int objective_num;
	int objective_status;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_status = Get_Int_Parameter("Status");
	Commands->Set_Objective_Status(objective_num,objective_status);
}

void JFW_Set_Objective_Status::Created(GameObject *obj)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x)
	{
		return;
	}
	int objective_num;
	int objective_status;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_status = Get_Int_Parameter("Status");
	Commands->Set_Objective_Status(objective_num,objective_status);
}

void JFW_Set_Objective_Status::Timer_Expired(GameObject *obj,int number)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 5)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != number)
	{
		return;
	}
	if (repeat)
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	int objective_num;
	int objective_status;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_status = Get_Int_Parameter("Status");
	Commands->Set_Objective_Status(objective_num,objective_status);
}

void JFW_Set_Objective_Status::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&repeat,1,1);
	Auto_Save_Variable(&timertime,4,2);
	Auto_Save_Variable(&timernumber,4,3);
}

void JFW_Change_Objective_Type::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 1)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	int objective_num;
	int objective_type;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_type = Get_Int_Parameter("Objective_Type");
	Commands->Change_Objective_Type(objective_num,objective_type);
}

void JFW_Change_Objective_Type::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 4)
	{
		return;
	}
	int objective_num;
	int objective_type;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_type = Get_Int_Parameter("Objective_Type");
	Commands->Change_Objective_Type(objective_num,objective_type);
}

void JFW_Change_Objective_Type::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int x;
	TimerParams t;
	x = Get_Int_Parameter("Timer_Custom");
	if (x == type)
	{
		t = *(PTimerParams)param;
		repeat = t.repeat;
		timertime = t.time;
		timernumber = t.number;
		Commands->Start_Timer(obj,this,t.time,t.number);
	}
	x = Get_Int_Parameter("Type");
	if (x != 3)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != type)
	{
		return;
	}
	int objective_num;
	int objective_type;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_type = Get_Int_Parameter("Objective_Type");
	Commands->Change_Objective_Type(objective_num,objective_type);
}

void JFW_Change_Objective_Type::Killed(GameObject *obj,GameObject *killer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 2)
	{
		return;
	}
	int objective_num;
	int objective_type;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_type = Get_Int_Parameter("Objective_Type");
	Commands->Change_Objective_Type(objective_num,objective_type);
}

void JFW_Change_Objective_Type::Created(GameObject *obj)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x)
	{
		return;
	}
	int objective_num;
	int objective_type;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_type = Get_Int_Parameter("Objective_Type");
	Commands->Change_Objective_Type(objective_num,objective_type);
}

void JFW_Change_Objective_Type::Timer_Expired(GameObject *obj,int number)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 5)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != number)
	{
		return;
	}
	if (repeat)
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	int objective_num;
	int objective_type;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_type = Get_Int_Parameter("Objective_Type");
	Commands->Change_Objective_Type(objective_num,objective_type);
}

void JFW_Change_Objective_Type::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&repeat,1,1);
	Auto_Save_Variable(&timertime,4,2);
	Auto_Save_Variable(&timernumber,4,3);
}

void JFW_Set_Objective_Radar_Blip::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 1)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	int objective_num;
	Vector3 objective_position;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_position = Get_Vector3_Parameter("Pos");
	Commands->Set_Objective_Radar_Blip(objective_num,objective_position);
}

void JFW_Set_Objective_Radar_Blip::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 4)
	{
		return;
	}
	int objective_num;
	Vector3 objective_position;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_position = Get_Vector3_Parameter("Pos");
	Commands->Set_Objective_Radar_Blip(objective_num,objective_position);
}

void JFW_Set_Objective_Radar_Blip::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int x;
	TimerParams t;
	x = Get_Int_Parameter("Timer_Custom");
	if (x == type)
	{
		t = *(PTimerParams)param;
		repeat = t.repeat;
		timertime = t.time;
		timernumber = t.number;
		Commands->Start_Timer(obj,this,t.time,t.number);
	}
	x = Get_Int_Parameter("Type");
	if (x != 3)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != type)
	{
		return;
	}
	int objective_num;
	Vector3 objective_position;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_position = Get_Vector3_Parameter("Pos");
	Commands->Set_Objective_Radar_Blip(objective_num,objective_position);
}

void JFW_Set_Objective_Radar_Blip::Killed(GameObject *obj,GameObject *killer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 2)
	{
		return;
	}
	int objective_num;
	Vector3 objective_position;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_position = Get_Vector3_Parameter("Pos");
	Commands->Set_Objective_Radar_Blip(objective_num,objective_position);
}

void JFW_Set_Objective_Radar_Blip::Created(GameObject *obj)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x)
	{
		return;
	}
	int objective_num;
	Vector3 objective_position;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_position = Get_Vector3_Parameter("Pos");
	Commands->Set_Objective_Radar_Blip(objective_num,objective_position);
}

void JFW_Set_Objective_Radar_Blip::Timer_Expired(GameObject *obj,int number)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 5)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != number)
	{
		return;
	}
	if (repeat)
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	int objective_num;
	Vector3 objective_position;
	objective_num = Get_Int_Parameter("Objective_Num");
	objective_position = Get_Vector3_Parameter("Pos");
	Commands->Set_Objective_Radar_Blip(objective_num,objective_position);
}

void JFW_Set_Objective_Radar_Blip::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&repeat,1,1);
	Auto_Save_Variable(&timertime,4,2);
	Auto_Save_Variable(&timernumber,4,3);
}

void JFW_Set_Objective_Radar_Blip_Object::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 1)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	int objective_num;
	int object_num;
	objective_num = Get_Int_Parameter("Objective_Num");
	object_num = Get_Int_Parameter("Object");
	GameObject *a = Commands->Find_Object(object_num);
	Commands->Set_Objective_Radar_Blip_Object(objective_num,a);
}

void JFW_Set_Objective_Radar_Blip_Object::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 4)
	{
		return;
	}
	int objective_num;
	int object_num;
	objective_num = Get_Int_Parameter("Objective_Num");
	object_num = Get_Int_Parameter("Object");
	GameObject *a = Commands->Find_Object(object_num);
	Commands->Set_Objective_Radar_Blip_Object(objective_num,a);
}

void JFW_Set_Objective_Radar_Blip_Object::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int x;
	TimerParams t;
	x = Get_Int_Parameter("Timer_Custom");
	if (x == type)
	{
		t = *(PTimerParams)param;
		repeat = t.repeat;
		timertime = t.time;
		timernumber = t.number;
		Commands->Start_Timer(obj,this,t.time,t.number);
	}
	x = Get_Int_Parameter("Type");
	if (x != 3)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != type)
	{
		return;
	}
	int objective_num;
	int object_num;
	objective_num = Get_Int_Parameter("Objective_Num");
	object_num = Get_Int_Parameter("Object");
	GameObject *a = Commands->Find_Object(object_num);
	Commands->Set_Objective_Radar_Blip_Object(objective_num,a);
}

void JFW_Set_Objective_Radar_Blip_Object::Killed(GameObject *obj,GameObject *killer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 2)
	{
		return;
	}
	int objective_num;
	int object_num;
	objective_num = Get_Int_Parameter("Objective_Num");
	object_num = Get_Int_Parameter("Object");
	GameObject *a = Commands->Find_Object(object_num);
	Commands->Set_Objective_Radar_Blip_Object(objective_num,a);
}

void JFW_Set_Objective_Radar_Blip_Object::Created(GameObject *obj)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x)
	{
		return;
	}
	int objective_num;
	int object_num;
	objective_num = Get_Int_Parameter("Objective_Num");
	object_num = Get_Int_Parameter("Object");
	GameObject *a = Commands->Find_Object(object_num);
	Commands->Set_Objective_Radar_Blip_Object(objective_num,a);
}

void JFW_Set_Objective_Radar_Blip_Object::Timer_Expired(GameObject *obj,int number)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 5)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != number)
	{
		return;
	}
	if (repeat)
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	int objective_num;
	int object_num;
	objective_num = Get_Int_Parameter("Objective_Num");
	object_num = Get_Int_Parameter("Object");
	GameObject *a = Commands->Find_Object(object_num);
	Commands->Set_Objective_Radar_Blip_Object(objective_num,a);
}

void JFW_Set_Objective_Radar_Blip_Object::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&repeat,1,1);
	Auto_Save_Variable(&timertime,4,2);
	Auto_Save_Variable(&timernumber,4,3);
}

void JFW_Set_Objective_HUD_Info::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 1)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	int objective_num;
	float priority;
	const char *pog_file;
	int pog_title_id;
	objective_num = Get_Int_Parameter("Objective_Num");
	priority = Get_Float_Parameter("Priority");
	pog_file = Get_Parameter("Pog_File");
	pog_title_id = Get_Int_Parameter("Pog_Title_ID");
	Commands->Set_Objective_HUD_Info(objective_num,priority,pog_file,pog_title_id);
}

void JFW_Set_Objective_HUD_Info::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 4)
	{
		return;
	}
	int objective_num;
	float priority;
	const char *pog_file;
	int pog_title_id;
	objective_num = Get_Int_Parameter("Objective_Num");
	priority = Get_Float_Parameter("Priority");
	pog_file = Get_Parameter("Pog_File");
	pog_title_id = Get_Int_Parameter("Pog_Title_ID");
	Commands->Set_Objective_HUD_Info(objective_num,priority,pog_file,pog_title_id);
}

void JFW_Set_Objective_HUD_Info::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int x;
	TimerParams t;
	x = Get_Int_Parameter("Timer_Custom");
	if (x == type)
	{
		t = *(PTimerParams)param;
		repeat = t.repeat;
		timertime = t.time;
		timernumber = t.number;
		Commands->Start_Timer(obj,this,t.time,t.number);
	}
	x = Get_Int_Parameter("Type");
	if (x != 3)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != type)
	{
		return;
	}
	int objective_num;
	float priority;
	const char *pog_file;
	int pog_title_id;
	objective_num = Get_Int_Parameter("Objective_Num");
	priority = Get_Float_Parameter("Priority");
	pog_file = Get_Parameter("Pog_File");
	pog_title_id = Get_Int_Parameter("Pog_Title_ID");
	Commands->Set_Objective_HUD_Info(objective_num,priority,pog_file,pog_title_id);
}

void JFW_Set_Objective_HUD_Info::Killed(GameObject *obj,GameObject *killer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 2)
	{
		return;
	}
	int objective_num;
	float priority;
	const char *pog_file;
	int pog_title_id;
	objective_num = Get_Int_Parameter("Objective_Num");
	priority = Get_Float_Parameter("Priority");
	pog_file = Get_Parameter("Pog_File");
	pog_title_id = Get_Int_Parameter("Pog_Title_ID");
	Commands->Set_Objective_HUD_Info(objective_num,priority,pog_file,pog_title_id);
}

void JFW_Set_Objective_HUD_Info::Created(GameObject *obj)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x)
	{
		return;
	}
	int objective_num;
	float priority;
	const char *pog_file;
	int pog_title_id;
	objective_num = Get_Int_Parameter("Objective_Num");
	priority = Get_Float_Parameter("Priority");
	pog_file = Get_Parameter("Pog_File");
	pog_title_id = Get_Int_Parameter("Pog_Title_ID");
	Commands->Set_Objective_HUD_Info(objective_num,priority,pog_file,pog_title_id);
}

void JFW_Set_Objective_HUD_Info::Timer_Expired(GameObject *obj,int number)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 5)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != number)
	{
		return;
	}
	if (repeat == 1)
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	int objective_num;
	float priority;
	const char *pog_file;
	int pog_title_id;
	objective_num = Get_Int_Parameter("Objective_Num");
	priority = Get_Float_Parameter("Priority");
	pog_file = Get_Parameter("Pog_File");
	pog_title_id = Get_Int_Parameter("Pog_Title_ID");
	Commands->Set_Objective_HUD_Info(objective_num,priority,pog_file,pog_title_id);
}

void JFW_Set_Objective_HUD_Info::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&repeat,4,1);
	Auto_Save_Variable(&timertime,4,2);
	Auto_Save_Variable(&timernumber,4,3);
}

void JFW_Set_Objective_HUD_Info_Position::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 1)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	int objective_num;
	float priority;
	const char *pog_file;
	int pog_title_id;
	Vector3 objective_position;
	objective_num = Get_Int_Parameter("Objective_Num");
	priority = Get_Float_Parameter("Priority");
	pog_file = Get_Parameter("Pog_File");
	pog_title_id = Get_Int_Parameter("Pog_Title_ID");
	objective_position = Get_Vector3_Parameter("Position");
	Commands->Set_Objective_HUD_Info_Position(objective_num,priority,pog_file,pog_title_id,objective_position);
}

void JFW_Set_Objective_HUD_Info_Position::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 4)
	{
		return;
	}
	int objective_num;
	float priority;
	const char *pog_file;
	int pog_title_id;
	Vector3 objective_position;
	objective_num = Get_Int_Parameter("Objective_Num");
	priority = Get_Float_Parameter("Priority");
	pog_file = Get_Parameter("Pog_File");
	pog_title_id = Get_Int_Parameter("Pog_Title_ID");
	objective_position = Get_Vector3_Parameter("Position");
	Commands->Set_Objective_HUD_Info_Position(objective_num,priority,pog_file,pog_title_id,objective_position);
}

void JFW_Set_Objective_HUD_Info_Position::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int x;
	TimerParams t;
	x = Get_Int_Parameter("Timer_Custom");
	if (x == type)
	{
		t = *(PTimerParams)param;
		repeat = t.repeat;
		timertime = t.time;
		timernumber = t.number;
		Commands->Start_Timer(obj,this,t.time,t.number);
	}
	x = Get_Int_Parameter("Type");
	if (x != 3)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != type)
	{
		return;
	}
	int objective_num;
	float priority;
	const char *pog_file;
	int pog_title_id;
	Vector3 objective_position;
	objective_num = Get_Int_Parameter("Objective_Num");
	priority = Get_Float_Parameter("Priority");
	pog_file = Get_Parameter("Pog_File");
	pog_title_id = Get_Int_Parameter("Pog_Title_ID");
	objective_position = Get_Vector3_Parameter("Position");
	Commands->Set_Objective_HUD_Info_Position(objective_num,priority,pog_file,pog_title_id,objective_position);
}

void JFW_Set_Objective_HUD_Info_Position::Killed(GameObject *obj,GameObject *killer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 2)
	{
		return;
	}
	int objective_num;
	float priority;
	const char *pog_file;
	int pog_title_id;
	Vector3 objective_position;
	objective_num = Get_Int_Parameter("Objective_Num");
	priority = Get_Float_Parameter("Priority");
	pog_file = Get_Parameter("Pog_File");
	pog_title_id = Get_Int_Parameter("Pog_Title_ID");
	objective_position = Get_Vector3_Parameter("Position");
	Commands->Set_Objective_HUD_Info_Position(objective_num,priority,pog_file,pog_title_id,objective_position);
}

void JFW_Set_Objective_HUD_Info_Position::Created(GameObject *obj)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x)
	{
		return;
	}
	int objective_num;
	float priority;
	const char *pog_file;
	int pog_title_id;
	Vector3 objective_position;
	objective_num = Get_Int_Parameter("Objective_Num");
	priority = Get_Float_Parameter("Priority");
	pog_file = Get_Parameter("Pog_File");
	pog_title_id = Get_Int_Parameter("Pog_Title_ID");
	objective_position = Get_Vector3_Parameter("Position");
	Commands->Set_Objective_HUD_Info_Position(objective_num,priority,pog_file,pog_title_id,objective_position);
}

void JFW_Set_Objective_HUD_Info_Position::Timer_Expired(GameObject *obj,int number)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 5)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != number)
	{
		return;
	}
	if (repeat == 1)
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	int objective_num;
	float priority;
	const char *pog_file;
	int pog_title_id;
	Vector3 objective_position;
	objective_num = Get_Int_Parameter("Objective_Num");
	priority = Get_Float_Parameter("Priority");
	pog_file = Get_Parameter("Pog_File");
	pog_title_id = Get_Int_Parameter("Pog_Title_ID");
	objective_position = Get_Vector3_Parameter("Position");
	Commands->Set_Objective_HUD_Info_Position(objective_num,priority,pog_file,pog_title_id,objective_position);
}

void JFW_Set_Objective_HUD_Info_Position::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&repeat,4,1);
	Auto_Save_Variable(&timertime,4,2);
	Auto_Save_Variable(&timernumber,4,3);
}

void JFW_Set_Num_Tertary_Objectives::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 1)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	x = Get_Int_Parameter("Count");
	Commands->Set_Num_Tertiary_Objectives(x);
}

void JFW_Set_Num_Tertary_Objectives::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 4)
	{
		return;
	}
	x = Get_Int_Parameter("Count");
	Commands->Set_Num_Tertiary_Objectives(x);
}

void JFW_Set_Num_Tertary_Objectives::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int x;
	TimerParams t;
	x = Get_Int_Parameter("Timer_Custom");
	if (x == type)
	{
		t = *(PTimerParams)param;
		repeat = t.repeat;
		timertime = t.time;
		timernumber = t.number;
		Commands->Start_Timer(obj,this,t.time,t.number);
	}
	x = Get_Int_Parameter("Type");
	if (x != 3)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != type)
	{
		return;
	}
	x = Get_Int_Parameter("Count");
	Commands->Set_Num_Tertiary_Objectives(x);
}

void JFW_Set_Num_Tertary_Objectives::Killed(GameObject *obj,GameObject *killer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 2)
	{
		return;
	}
	x = Get_Int_Parameter("Count");
	Commands->Set_Num_Tertiary_Objectives(x);
}

void JFW_Set_Num_Tertary_Objectives::Created(GameObject *obj)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x)
	{
		return;
	}
	x = Get_Int_Parameter("Count");
	Commands->Set_Num_Tertiary_Objectives(x);
}

void JFW_Set_Num_Tertary_Objectives::Timer_Expired(GameObject *obj,int number)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 5)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != number)
	{
		return;
	}
	if (repeat == 1)
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	x = Get_Int_Parameter("Count");
	Commands->Set_Num_Tertiary_Objectives(x);
}

void JFW_Set_Num_Tertary_Objectives::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&repeat,4,1);
	Auto_Save_Variable(&timertime,4,2);
	Auto_Save_Variable(&timernumber,4,3);
}

void JFW_Mission_Complete::Entered(GameObject *obj,GameObject *enterer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 1)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (CheckPlayerType(enterer,x))
	{
		return;
	}
	x = Get_Int_Parameter("Won");
	Commands->Mission_Complete(x != 0);
}

void JFW_Mission_Complete::Poked(GameObject *obj,GameObject *poker)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 4)
	{
		return;
	}
	x = Get_Int_Parameter("Won");
	Commands->Mission_Complete(x != 0);
}

void JFW_Mission_Complete::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	int x;
	TimerParams t;
	x = Get_Int_Parameter("Timer_Custom");
	if (x == type)
	{
		t = *(PTimerParams)param;
		repeat = t.repeat;
		timertime = t.time;
		timernumber = t.number;
		Commands->Start_Timer(obj,this,t.time,t.number);
	}
	x = Get_Int_Parameter("Type");
	if (x != 3)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != type)
	{
		return;
	}
	x = Get_Int_Parameter("Won");
	Commands->Mission_Complete(x != 0);
}

void JFW_Mission_Complete::Killed(GameObject *obj,GameObject *killer)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 2)
	{
		return;
	}
	x = Get_Int_Parameter("Won");
	Commands->Mission_Complete(x != 0);
}

void JFW_Mission_Complete::Created(GameObject *obj)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x)
	{
		return;
	}
	x = Get_Int_Parameter("Won");
	Commands->Mission_Complete(x != 0);
}

void JFW_Mission_Complete::Timer_Expired(GameObject *obj,int number)
{
	int x;
	x = Get_Int_Parameter("Type");
	if (x != 5)
	{
		return;
	}
	x = Get_Int_Parameter("TypeVal");
	if (x != number)
	{
		return;
	}
	if (repeat == 1)
	{
		Commands->Start_Timer(obj,this,timertime,timernumber);
	}
	x = Get_Int_Parameter("Won");
	Commands->Mission_Complete(x != 0);
}

void JFW_Mission_Complete::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&repeat,4,1);
	Auto_Save_Variable(&timertime,4,2);
	Auto_Save_Variable(&timernumber,4,3);
}

#define REGISTER_SCRIPT(name, params) ScriptRegistrant<name> name##Registrant(#name, params);
#define STAR (Commands->Get_A_Star(Commands->Get_Position(Owner())))
class JFW_Mission_Complete_Custom : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Mission_Complete(Get_Bool_Parameter("Success"));
		}
	}
};

REGISTER_SCRIPT(JFW_Mission_Complete_Custom,"Message:int,Success:int");

class JFW_Tutorial_Conversation : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("StartMessage"))
		{
			if (Get_Int_Parameter("DisableControls") == 1)
			{
				Commands->Control_Enable(STAR,false);
			}
			Commands->Stop_All_Conversations();
			int conversation = Commands->Create_Conversation(Get_Parameter("Conversation"),100,300.0f,0);
			Commands->Join_Conversation(obj,conversation,false,true,true);
			Commands->Join_Conversation(STAR,conversation,false,false,false);
			Commands->Start_Conversation(conversation,Get_Int_Parameter("ActionID"));
			Commands->Monitor_Conversation(obj,conversation);
			int lookid = Get_Int_Parameter("LookID");
			if (lookid)
			{
				Commands->Force_Camera_Look(Commands->Get_Position(Commands->Find_Object(lookid)));
			}
		}
		else if (type == Get_Int_Parameter("StopMessage"))
		{
			Commands->Stop_All_Conversations();
		}
	}

	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
	{
		if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED || complete_reason == ACTION_COMPLETE_CONVERSATION_INTERRUPTED || complete_reason == ACTION_COMPLETE_CONVERSATION_UNABLE_TO_INIT)
		{
			if (Get_Int_Parameter("DisableControls") == 1)
			{
				Commands->Control_Enable(STAR,true);
			}
		}
		if (action_id == Get_Int_Parameter("ActionID") && complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
		{
			Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("CompleteMessage"),Get_Int_Parameter("CompleteParam"),0);
		}
	}
};
REGISTER_SCRIPT(JFW_Tutorial_Conversation,"Conversation:string,ActionID:int,StartMessage:int,StopMessage:int,CompleteMessage:int,CompleteParam:int,DisableControls:int,LookID:int");

class JFW_HUD_Help_Text : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			int red = Get_Int_Parameter("Red");
			int green = Get_Int_Parameter("Green");
			int blue = Get_Int_Parameter("Blue");
			Vector3 color((float)red / 255.0f,(float)green / 255.0f,(float)blue / 255.0f);
			Commands->Set_HUD_Help_Text(Get_Int_Parameter("StringID"),color);
		}
	}
};

REGISTER_SCRIPT(JFW_HUD_Help_Text,"Message:int,StringID:int,Red:int,Green:int,Blue:int");

class JFW_Add_Objective_Startup : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		Commands->Add_Objective(Get_Int_Parameter("ObjectiveID"),Get_Int_Parameter("ObjectiveType"),OBJECTIVE_STATUS_HIDDEN,Get_Int_Parameter("ShortDescription"),(char *)Get_Parameter("Sound"),Get_Int_Parameter("LongDescription"));
	}
};

REGISTER_SCRIPT(JFW_Add_Objective_Startup,"ObjectiveID:int,ObjectiveType:int,ShortDescription:int,Sound:string,LongDescription:int");

class JFW_Set_Objective_Status_Custom : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Set_Objective_Status(Get_Int_Parameter("ObjectiveID"),Get_Int_Parameter("Status"));
		}
	}
};

REGISTER_SCRIPT(JFW_Set_Objective_Status_Custom,"Message:int,ObjectiveID:int,Status:int");

class JFW_Set_Objective_Radar_Blip_Custom : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Set_Objective_Radar_Blip(Get_Int_Parameter("ObjectiveID"),Get_Vector3_Parameter("Position"));
		}
	}
};

REGISTER_SCRIPT(JFW_Set_Objective_Radar_Blip_Custom,"Message:int,ObjectiveID:int,Position:int");

class JFW_Set_Objective_Radar_Blip_Object_Custom : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Set_Objective_Radar_Blip_Object(Get_Int_Parameter("ObjectiveID"),Commands->Find_Object(Get_Int_Parameter("ID")));
		}
	}
};

REGISTER_SCRIPT(JFW_Set_Objective_Radar_Blip_Object_Custom,"Message:int,ObjectiveID:int,ID:int");

class JFW_Set_Objective_HUD_Info_Custom : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Set_Objective_HUD_Info(Get_Int_Parameter("ObjectiveID"),Get_Float_Parameter("Priority"),Get_Parameter("Texture"),Get_Int_Parameter("StringID"));
		}
	}
};

REGISTER_SCRIPT(JFW_Set_Objective_HUD_Info_Custom,"Message:int,ObjectiveID:int,Priority:float,Texture:string,StringID:int");

class JFW_Set_Objective_HUD_Info_Position_Custom : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Set_Objective_HUD_Info_Position(Get_Int_Parameter("ObjectiveID"),Get_Float_Parameter("Priority"),Get_Parameter("Texture"),Get_Int_Parameter("StringID"),Get_Vector3_Parameter("Position"));
		}
	}
};

REGISTER_SCRIPT(JFW_Set_Objective_HUD_Info_Position_Custom,"Message:int,ObjectiveID:int,Priority:float,Texture:string,StringID:int,Position:vector3");

class JFW_Add_Radar_Marker : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Add_Radar_Marker(Get_Int_Parameter("MarkerID"),Get_Vector3_Parameter("Position"),Get_Int_Parameter("BlipShape"),Get_Int_Parameter("BlipColor"));
		}
	}
};

REGISTER_SCRIPT(JFW_Add_Radar_Marker,"Message:int,MarkerID:int,Position:vector3,BlipShape:int,BlipColor:int");

class JFW_Clear_Radar_Marker : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Clear_Radar_Marker(Get_Int_Parameter("MarkerID"));
		}
	}
};

REGISTER_SCRIPT(JFW_Clear_Radar_Marker,"Message:int,MarkerID:int");

class JFW_Tutorial_Conversation_2 : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("StartMessage"))
		{
			if (Get_Int_Parameter("DisableControls") == 1)
			{
				Commands->Control_Enable(STAR,false);
			}
			Commands->Stop_All_Conversations();
			int conversation = Commands->Create_Conversation(Get_Parameter("Conversation"),100,300.0f,0);
			Commands->Join_Conversation(obj,conversation,false,false,false);
			Commands->Start_Conversation(conversation,Get_Int_Parameter("ActionID"));
			Commands->Monitor_Conversation(obj,conversation);
			int lookid = Get_Int_Parameter("LookID");
			if (lookid)
			{
				Commands->Force_Camera_Look(Commands->Get_Position(Commands->Find_Object(lookid)));
			}
		}
		else if (type == Get_Int_Parameter("StopMessage"))
		{
			Commands->Stop_All_Conversations();
		}
	}

	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
	{
		if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED || complete_reason == ACTION_COMPLETE_CONVERSATION_INTERRUPTED || complete_reason == ACTION_COMPLETE_CONVERSATION_UNABLE_TO_INIT)
		{
			if (Get_Int_Parameter("DisableControls") == 1)
			{
				Commands->Control_Enable(STAR,true);
			}
		}
		if (action_id == Get_Int_Parameter("ActionID") && complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
		{
			Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("CompleteMessage"),Get_Int_Parameter("CompleteParam"),0);
		}
	}
};
REGISTER_SCRIPT(JFW_Tutorial_Conversation_2,"Conversation:string,ActionID:int,StartMessage:int,StopMessage:int,CompleteMessage:int,CompleteParam:int,DisableControls:int,LookID:int");

class JFW_Reveal_Map : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		Commands->Reveal_Map();
	}
};

REGISTER_SCRIPT(JFW_Reveal_Map,"");

class JFW_Goto_Object : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,Get_Int_Parameter("ActionID"));
			params.Set_Movement(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Float_Parameter("Speed"),Get_Float_Parameter("ArriveDistance"));
			params.WaypathID = Get_Int_Parameter("WaypathID");
			Commands->Action_Goto(obj,params);
		}
	}

	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
	{
		if (action_id == Get_Int_Parameter("ActionID"))
		{
			if (Get_Int_Parameter("CompleteMessage") != 0)
			{
				Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("CompleteMessage"),Get_Int_Parameter("CompleteParam"),0);
			}
		}
	}
};

REGISTER_SCRIPT(JFW_Goto_Object,"Message:int,ActionID:int,ID:int,Speed:float,ArriveDistance:float,WaypathID:int,CompleteMessage:int,CompleteParam:int");

class JFW_Goto_Location : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,Get_Int_Parameter("ActionID"));
			params.Set_Movement(Get_Vector3_Parameter("Location"),Get_Float_Parameter("Speed"),Get_Float_Parameter("ArriveDistance"));
			params.WaypathID = Get_Int_Parameter("WaypathID");
			Commands->Action_Goto(obj,params);
		}
	}

	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
	{
		if (action_id == Get_Int_Parameter("ActionID"))
		{
			if (Get_Int_Parameter("CompleteMessage") != 0)
			{
				Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("CompleteMessage"),Get_Int_Parameter("CompleteParam"),0);
			}
		}
	}
};

REGISTER_SCRIPT(JFW_Goto_Location,"Message:int,ActionID:int,Location:vector3,Speed:float,ArriveDistance:float,WaypathID:int,CompleteMessage:int,CompleteParam:int");

class JFW_Destroy_Object : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Destroy_Object(Commands->Find_Object(Get_Int_Parameter("ID")));
		}
	}
};

REGISTER_SCRIPT(JFW_Destroy_Object,"Message:int,ID:int");

class JFW_Innate_AI_Flags : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			int hibernate = Get_Int_Parameter("HibernateEnable");
			if (hibernate == 0 || hibernate == 1)
			{
				Commands->Enable_Hibernation(obj,hibernate != 0);
			}
			int seen = Get_Int_Parameter("EnemySeenEnable");
			if (seen == 0 || seen == 1)
			{
				Commands->Enable_Enemy_Seen(obj,seen != 0);
			}
			int innate = Get_Int_Parameter("InnateEnable");
			if (innate == 0 || innate == 1)
			{
				if (innate == 0)
				{
					Commands->Innate_Disable(obj);
				}
				if (innate == 1)
				{
					Commands->Innate_Enable(obj);
				}
			}
			int loiters = Get_Int_Parameter("LoitersAllowed");
			if (loiters == 0 || loiters == 1)
			{
				Commands->Set_Loiters_Allowed(obj,loiters != 0);
			}
			int stationary = Get_Int_Parameter("InnateIsStationary");
			if (stationary == 0 || stationary == 1)
			{
				Commands->Set_Innate_Is_Stationary(obj,stationary != 0);
			}
		}
	}
};

REGISTER_SCRIPT(JFW_Innate_AI_Flags,"Message:int,HibernateEnable:int,EnemySeenEnable:int,InnateEnable:int,LoitersAllowed:int,InnateIsStationary:int");

class JFW_Innate_AI_Aggressiveness : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Set_Innate_Aggressiveness(obj,Get_Float_Parameter("Aggressiveness"));
		}
	}
};

REGISTER_SCRIPT(JFW_Innate_AI_Aggressiveness,"Message:int,Aggressiveness:float");

class JFW_Innate_AI_Cover_Probability : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Set_Innate_Take_Cover_Probability(obj,Get_Float_Parameter("CoverProbability"));
		}
	}
};

REGISTER_SCRIPT(JFW_Innate_AI_Cover_Probability,"Message:int,CoverProbability:float");

class JFW_Innate_AI_Home_Location : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Set_Innate_Take_Cover_Probability(obj,Get_Float_Parameter("HomeLocation"));
		}
	}
};

REGISTER_SCRIPT(JFW_Innate_AI_Home_Location,"Message:int,HomeLocation:vector3");

class JFW_Stop_Action : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Action_Reset(obj,100);
		}
	}
};

REGISTER_SCRIPT(JFW_Stop_Action,"Message:int");

class JFW_Attack_Object : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			ActionParamsStruct params;
			if (Get_Float_Parameter("Speed") == 0)
			{
				VehicleGameObj *o = obj->As_VehicleGameObj();
				if (NULL != o)
				{
				  o->Set_Immovable(true);
				}
			}
			params.Set_Basic(this,100,Get_Int_Parameter("ActionID"));
			params.Set_Movement(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Float_Parameter("Speed"),Get_Float_Parameter("ArriveDistance"));
			params.Set_Attack(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Float_Parameter("Range"),Get_Float_Parameter("Error"),Get_Bool_Parameter("Primary"));
			params.WaypathID = Get_Int_Parameter("WaypathID");
			params.AttackCheckBlocked = false;
			params.AttackErrorOverride = true;
			Commands->Action_Attack(obj,params);
		}
	}

	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
	{
		if (action_id == Get_Int_Parameter("ActionID"))
		{
			if (Get_Float_Parameter("Speed") == 0)
			{
  			VehicleGameObj *o = obj->As_VehicleGameObj();
				if (NULL != o)
				{
  			  o->Set_Immovable(false);
				}
			}
			
			if (Get_Int_Parameter("CompleteMessage") != 0)
			{
				Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("CompleteMessage"),Get_Int_Parameter("CompleteParam"),0);
			}
		}
	}
};

REGISTER_SCRIPT(JFW_Attack_Object,"Message:int,ActionID:int,ID:int,Speed:float,ArriveDistance:float,WaypathID:int,CompleteMessage:int,CompleteParam:int,Range:float,Error:float,Primary:int");

class JFW_Attack_Location : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			ActionParamsStruct params;
			if (Get_Float_Parameter("Speed") == 0)
			{
				VehicleGameObj *o = obj->As_VehicleGameObj();
				if (NULL != o)
				{
				  o->Set_Immovable(true);
				}
			}
			params.Set_Basic(this,100,Get_Int_Parameter("ActionID"));
			params.Set_Movement(Get_Vector3_Parameter("Location"),Get_Float_Parameter("Speed"),Get_Float_Parameter("ArriveDistance"));
			params.Set_Attack(Get_Vector3_Parameter("Location"),Get_Float_Parameter("Range"),Get_Float_Parameter("Error"),Get_Bool_Parameter("Primary"));
			params.WaypathID = Get_Int_Parameter("WaypathID");
			params.AttackCheckBlocked = false;
			params.AttackErrorOverride = true;
			Commands->Action_Attack(obj,params);
		}
	}

	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason)
	{
		if (action_id == Get_Int_Parameter("ActionID"))
		{
			if (Get_Float_Parameter("Speed") == 0)
			{
  			VehicleGameObj *o = obj->As_VehicleGameObj();
				if (NULL != o)
				{
  			  o->Set_Immovable(false);
				}
			}
			
			if (Get_Int_Parameter("CompleteMessage") != 0)
			{
				Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("CompleteMessage"),Get_Int_Parameter("CompleteParam"),0);
			}
		}
	}
};

REGISTER_SCRIPT(JFW_Attack_Location,"Message:int,ActionID:int,Location:vector3,Speed:float,ArriveDistance:float,WaypathID:int,CompleteMessage:int,CompleteParam:int,Range:float,Error:float,Primary:int");

class JFW_Set_Radar_Blip_State : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Set_Obj_Radar_Blip_Color(obj,Get_Int_Parameter("BlipColor"));
			Commands->Set_Obj_Radar_Blip_Shape(obj,Get_Int_Parameter("BlipShape"));
		}
	}
};

REGISTER_SCRIPT(JFW_Set_Radar_Blip_State,"Message:int,BlipColor:int,BlipShape:int");

class JFW_Give_PowerUp : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Give_PowerUp(obj,Get_Parameter("PowerUp"),Get_Bool_Parameter("DisplayOnHud"));
			if (Get_Bool_Parameter("SelectWeapon"))
			{
				Commands->Select_Weapon(obj,Get_Powerup_Weapon(Get_Parameter("PowerUp")));
			}
		}
	}
};

REGISTER_SCRIPT(JFW_Give_PowerUp,"Message:int,PowerUp:string,DisplayOnHud:int,SelectWeapon:int");

class JFW_Star_Trigger_Zone : public ScriptImpClass
{
	bool enabled;
	void Created(GameObject *obj)
	{
		enabled = false;
	}

	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("EnableMessage"))
		{
			enabled = true;
		}
	}

	void Entered(GameObject *obj,GameObject *enterer)
	{
		if (obj == STAR && enabled == true)
		{
			Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("ZoneMessage"),Get_Int_Parameter("ZoneParam"),0);
		}
	}
};

REGISTER_SCRIPT(JFW_Star_Trigger_Zone,"EnableMessage:int,ZoneMessage:int,ZoneParam:int");

class JFW_Encyclopedia_Reveal : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			int enctype = Get_Int_Parameter("Type");
			int id = Get_Int_Parameter("ID");
			bool ret = false;
			switch (enctype)
			{
			case 0:
				ret = Commands->Reveal_Encyclopedia_Character(id);
				break;
			case 1:
				ret = Commands->Reveal_Encyclopedia_Weapon(id);
				break;
			case 2:
				ret = Commands->Reveal_Encyclopedia_Vehicle(id);
				break;
			case 3:
				ret = Commands->Reveal_Encyclopedia_Building(id);
				break;
			}
			if (ret && Get_Bool_Parameter("DisplayUI"))
			{
				Commands->Display_Encyclopedia_Event_UI();
			}
		}
	}
};

REGISTER_SCRIPT(JFW_Encyclopedia_Reveal,"Message:int,Type:int,ID:int,DisplayUI:int");

class JFW_Poke_Send_Custom_Self : public ScriptImpClass
{
	void Poked(GameObject *obj,GameObject *poker)
	{
		Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),0);
	}
};

REGISTER_SCRIPT(JFW_Poke_Send_Custom_Self,"Message:int,Type:int");

class JFW_Poke_Send_Custom_Poker : public ScriptImpClass
{
	void Poked(GameObject *obj,GameObject *poker)
	{
		Commands->Send_Custom_Event(obj,poker,Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),0);
	}
};

REGISTER_SCRIPT(JFW_Poke_Send_Custom_Poker,"Message:int,Type:int");

class JFW_Apply_Damage_Custom : public ScriptImpClass
{
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Commands->Apply_Damage(Commands->Find_Object(Get_Int_Parameter("ID")),Get_Float_Parameter("Amount"),Get_Parameter("Warhead"),0);
		}
	}
};
REGISTER_SCRIPT(JFW_Apply_Damage_Custom,"Message:int,ID:int,Warhead:string,Amount:float");

class JFW_Deployable_Object : public ScriptImpClass
{
	void Damaged(GameObject *obj, GameObject *damager, float amount)
	{
		unsigned int warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Warhead"));
		if (warhead == Get_Damage_Warhead() && damager == obj)
		{
			Vector3 v = Commands->Get_Position(Owner());
			v.Z += Get_Float_Parameter("Z_Offset");
			float len = Get_Float_Parameter("Distance");
			float angle = DEG_TO_RADF(Commands->Get_Facing(Owner()));
			v.X += cos(angle) * len;
			v.Y += sin(angle) * len;
			GameObject *obj = Commands->Create_Object(Get_Parameter("Preset"),v);
			Commands->Set_Player_Type(obj,Commands->Get_Player_Type(Owner()));
			PhysicalGameObj *p = obj->As_PhysicalGameObj();
			p->Peek_Physical_Object()->Set_Collision_Group(TERRAIN_AND_BULLET_COLLISION_GROUP);
		}
	}
};
REGISTER_SCRIPT(JFW_Deployable_Object,"Key:string,Z_Offset:float,Distance:float,Preset:string");

class JFW_Damage_All_Objects_Area : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
	}
	void Timer_Expired(GameObject *obj,int number)
	{
		if (number == 1)
		{
			Damage_All_Objects_Area_By_Team(Get_Float_Parameter("Damage"),Get_Parameter("Warhead"),Commands->Get_Position(obj),Get_Float_Parameter("Distance"),obj,Get_Bool_Parameter("Soldiers"),Get_Bool_Parameter("Vehicles"),Get_Int_Parameter("Team"));
			Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
		}
	}
};
REGISTER_SCRIPT(JFW_Damage_All_Objects_Area,"Time:float,Damage:float,Warhead:string,Distance:float,Soldiers:int,Vehicles:int,Team:int");

class JFW_Set_HUD_Help_Text_Player : public ScriptImpClass
{
	void Custom(GameObject *obj, int type, int param, GameObject *sender)
	{
		if (type == Get_Int_Parameter("Message"))
		{
			Set_HUD_Help_Text_Player(obj,Get_Int_Parameter("String"),Get_Vector3_Parameter("Color"));
			if (Get_Bool_Parameter("OnceOnly"))
			{
				Destroy_Script();
			}
		}
	}
};
REGISTER_SCRIPT(JFW_Set_HUD_Help_Text_Player, "Message:int,String:int,Color:vector3,OnceOnly:int");

class JFW_Spawn_Projectile_Death : public ScriptImpClass
{
	void Killed(GameObject *obj,GameObject *killer)
	{
		Vector3 spawn_location;
		float facing;
		const char *c;
		GameObject *object;
		spawn_location = Commands->Get_Position(obj);
		facing = Commands->Get_Facing(obj);
		c = Get_Parameter("Projectile");
		object = Commands->Create_Object (c,spawn_location);
		Commands->Set_Facing(object,facing);
		PhysicalGameObj *p = obj->As_PhysicalGameObj();
		if (p)
		{
			PhysClass *ph = p->Peek_Physical_Object();
			if (ph)
			{
				MoveablePhysClass *m = ph->As_MoveablePhysClass();
				if (m)
				{
					PhysicalGameObj *p2 = object->As_PhysicalGameObj();
					if (p2)
					{
						SimpleGameObj *s = p2->As_SimpleGameObj();
						if (s)
						{
							PhysClass *ph2 = s->Peek_Physical_Object();
							if (ph2)
							{
								MoveablePhysClass *m2 = ph2->As_MoveablePhysClass();
								if (m2)
								{
									Vector3 velocity;
									m->Get_Velocity(&velocity);
									m2->Set_Velocity(velocity);
									object->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
								}
							}
						}
					}
				}
			}
		}
		Destroy_Script();
	}
};
REGISTER_SCRIPT(JFW_Spawn_Projectile_Death, "Projectile:string");

class JFW_Projectile_Sync : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
	}
	void Timer_Expired(GameObject *obj,int number)
	{
		obj->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
	}
};
REGISTER_SCRIPT(JFW_Projectile_Sync, "Time:float");

class JFW_Medic_Beacon : public ScriptImpClass
{
	bool fire;
	void Created(GameObject *obj)
	{
		fire = true;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
		Commands->Start_Timer(obj,this,1,3);
	}
	void Timer_Expired(GameObject *obj,int number)
	{
		if (number == 1)
		{
			Commands->Destroy_Object(obj);
		}
		if (number == 2)
		{
			fire = true;
		}
		if (number == 3)
		{
			Commands->Start_Timer(obj,this,1,3);
			if (!fire)
			{
				return;
			}
			SmartGameObj *o = obj->As_SmartGameObj();
			if (!o)
			{
				return;
			}
			for (SLNode<SoldierGameObj>* node = GameObjManager::SoldierGameObjList.Head(); node; node = node->Next())
			{
				SoldierGameObj* object = node->Data();
				if ((object->Get_Defense_Object()->Get_Health() == object->Get_Defense_Object()->Get_Health_Max()) && (object->Get_Defense_Object()->Get_Shield_Strength() == object->Get_Defense_Object()->Get_Shield_Strength()))
				{
					continue;
				}
				if (!o->Is_Teammate(object))
				{
					continue;
				}
				if (!object->Is_Visible())
				{
					continue;
				}
				if (!o->Is_Obj_Visible(object))
				{
					continue;
				}
				fire = false;
				Commands->Apply_Damage(object,Get_Float_Parameter("Amount"),Get_Parameter("Warhead"),0);
				Commands->Start_Timer(obj,this,Get_Float_Parameter("ROF"),2);
			}
		}
	}
};
REGISTER_SCRIPT(JFW_Medic_Beacon, "Time:float,Amount:float,Warhead:string,ROF:float");

class JFW_Medic_Beacon_Layer : public ScriptImpClass
{
	void Damaged(GameObject *obj, GameObject *damager, float amount)
	{
		unsigned int warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Warhead"));
		if (warhead == Get_Damage_Warhead() && damager == obj)
		{
			SoldierGameObj *s = damager->As_SoldierGameObj();
			if (s)
			{
				Vector3 v = Commands->Get_Position(damager);
				GameObject *mine = Commands->Create_Object(Get_Parameter("Beacon_Preset"),v);
				Commands->Set_Player_Type(mine,Commands->Get_Player_Type(damager));
				PhysicalGameObj *p = mine->As_PhysicalGameObj();
				p->Peek_Physical_Object()->Set_Collision_Group(TERRAIN_AND_BULLET_COLLISION_GROUP);
				ArmedGameObj *a = p->As_ArmedGameObj();
				Matrix3D muzzle = a->Get_Muzzle();
				Matrix3D transform(true);
				transform.Rotate_Z(muzzle.Get_Z_Rotation());
				transform.Set_Translation(muzzle.Get_Translation());
				p->Peek_Physical_Object()->Set_Transform(transform);
				MoveablePhysClass *m = p->Peek_Physical_Object()->As_MoveablePhysClass();
				Vector3 velocity = muzzle.Get_X_Vector() * Get_Float_Parameter("Throw_Velocity");
				m->Set_Velocity(velocity);
				p->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_RARE,true);
			}
		}
	}
};
REGISTER_SCRIPT(JFW_Medic_Beacon_Layer, "Beacon_Preset:string,Throw_Velocity:float,Warhead:string");

class RMV_Test_Big_Gun_Turning : public ScriptImpClass
{
private:
	int DestroyedSamCount;
	bool SoundHeardTrigger;
	bool DestroyCinematicStarted;
public:
	void Register_Auto_Save_Variables()
	{
		Auto_Save_Variable(&DestroyedSamCount,4,1);
		Auto_Save_Variable(&SoundHeardTrigger,1,2);
		Auto_Save_Variable(&DestroyCinematicStarted,1,3);
	}
	void Created(GameObject *obj)
	{
		DestroyCinematicStarted = false;
		DestroyedSamCount = 0;
		SoundHeardTrigger = false;
		Vector3 pos = Commands->Get_Position(obj);
		Vector3 target_direction = obj->As_VehicleGameObj()->Get_Muzzle(0).Get_X_Vector();
		obj->As_VehicleGameObj()->Set_Targeting(pos + target_direction * 100);
	}
	void Killed(GameObject *obj,GameObject *killer)
	{
		Commands->Attach_Script(Commands->Create_Object("Invisible_Object",Vector3(0,0,0)),"Test_Cinematic","X3C_Bigguns2.txt");
		Commands->Send_Custom_Event(obj,Commands->Find_Object(1100004),306,3,0);
		Commands->Send_Custom_Event(obj,Commands->Find_Object(1100004),306,1,0);
		Commands->Send_Custom_Event(obj,Commands->Find_Object(1100003),40027,0,0);
	}
	void Sound_Heard(GameObject *obj,const CombatSound & sound)
	{
		if (sound.sound == SOUND_TYPE_DESIGNER04)
		{
			Commands->Shake_Camera(sound.Position,40.0f,0.3f,1.0f);
			if ( !SoundHeardTrigger && Commands->Get_Random(1.0f,5.0f) < 2.0f)
			{
				SoundHeardTrigger = true;
			}
		}
	}
	void Damaged(GameObject *obj, GameObject *damager, float amount)
	{
		if (DestroyCinematicStarted)
		{
			if (Commands->Get_Health(obj) < 1.0f)
			{
				Commands->Set_Health(obj,Commands->Get_Health(obj) + 1.0f);
			}
		}
	}
	void Timer_Expired(GameObject *obj,int number)
	{
		if (number == 40024)
		{
			if (Commands->Find_Object(1100002))
			{
				Commands->Attach_Script(Commands->Create_Object("Invisible_Object",Vector3(0,0,0)),"Test_Cinematic","X3C_Bigguns.txt");
				DestroyCinematicStarted = true;
			}
		}
	}
	void Custom(GameObject *obj,int type,int param,GameObject *sender)
	{
		if (type == 40026)
		{
			Commands->Action_Reset(obj,100.0);
			Commands->Enable_Enemy_Seen(obj,0);
		}
		if (type == 1000 && param == 1000)
		{
			ActionParamsStruct params;
			params.Set_Basic(Get_ID(),99,2);
			params.Set_Attack(Commands->Find_Object(1100003),1000.0f,2.0f,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
		}
		if (type == 2000 && param == 2000)
		{
			DestroyedSamCount++;
			if (DestroyedSamCount == 2)
			{
				if (Commands->Find_Object(1100003))
				{
					Commands->Start_Timer(obj,this,2.0f,40024);
				}
			}
		}
		if (type == 2450 && param == 2450)
		{
			GameObject *o = Commands->Find_Object(1100003);
			if (o)
			{
				Commands->Send_Custom_Event(obj,o,8000,8000,0);
			}
			Commands->Destroy_Object(obj);
		}
	}
};
REGISTER_SCRIPT(RMV_Test_Big_Gun_Turning, "");

class M08_Nod_Turret : public ScriptImpClass
{
	bool Attacking;
public:
	void Register_Auto_Save_Variables()
	{
		Auto_Save_Variable(&Attacking,1,1);
	}
	void Created(GameObject *obj)
	{
		Commands->Set_Player_Type(obj,0);
		Commands->Enable_Enemy_Seen(obj,true);
		Attacking = false;
		Vector3 pos = Commands->Get_Position(obj);
		Vector3 target_direction = obj->As_VehicleGameObj()->Get_Muzzle(0).Get_X_Vector();
		obj->As_VehicleGameObj()->Set_Targeting(pos + target_direction * 100);
	}
	void Enemy_Seen(GameObject *obj,GameObject *enemy)
	{
		if (!Attacking)
		{
			Attacking = true;
			ActionParamsStruct params;
			params.Set_Basic(Get_ID(),90,1);
			params.Set_Attack(enemy,250,0,true);
			params.AttackCheckBlocked = false;
			Commands->Action_Attack(obj,params);
			Commands->Start_Timer(obj,this,6.0f,0);
		}
	}
	void Damaged(GameObject *obj, GameObject *damager, float amount)
	{
		if (!Attacking)
		{
			if (Commands->Get_Player_Type(damager))
			{
				Attacking = true;
				ActionParamsStruct params;
				params.Set_Basic(Get_ID(),90,1);
				params.Set_Attack(damager,250,0,true);
				params.AttackCheckBlocked = false;
				Commands->Action_Attack(obj,params);
				Commands->Start_Timer(obj,this,6.0f,0);
			}
		}
	}
	void Timer_Expired(GameObject *obj,int number)
	{
		if (!number)
		{
			Attacking = false;
		}
	}
};
REGISTER_SCRIPT(M08_Nod_Turret, "");

class M10_Turret : public ScriptImpClass
{
	void Created(GameObject *obj)
	{
		Commands->Enable_Enemy_Seen(obj,true);
		Vector3 pos = Commands->Get_Position(obj);
		Vector3 target_direction = obj->As_VehicleGameObj()->Get_Muzzle(0).Get_X_Vector();
		obj->As_VehicleGameObj()->Set_Targeting(pos + target_direction * 100);
	}
	void Enemy_Seen(GameObject *obj,GameObject *enemy)
	{
		ActionParamsStruct params;
		params.Set_Basic(Get_ID(),90,0);
		params.Set_Attack(enemy,100,4,true);
		Commands->Action_Attack(obj,params);
		Commands->Start_Timer(obj,this,10.0f,0);
	}
	void Killed(GameObject *obj,GameObject *killer)
	{
		Vector3 v = Commands->Get_Position(obj);
		float facing = Commands->Get_Facing(obj);
		GameObject *o = Commands->Create_Object("Nod_Turret_Destroyed",v);
		Commands->Set_Facing(o,facing);
		Commands->Attach_Script(o,"M10_Destroyed_Turret","");
	}
	void Timer_Expired(GameObject *obj,int number)
	{
		Commands->Action_Reset(obj,99.0f);
	}
};
REGISTER_SCRIPT(M10_Turret, "");

ScriptRegistrant<JFW_Add_Objective> JFW_Add_Objective_Registrant("JFW_Add_Objective","Type:int,TypeVal:int,Objective_Num:int,Objective_Type:int,Title_ID:int,Unknown:int,Sound_Name:string,Description_ID:int,Timer_Custom:int,Trigger:int");
ScriptRegistrant<JFW_Remove_Objective> JFW_Remove_Objective_Registrant("JFW_Remove_Objective","Type:int,TypeVal:int,Objective_Num:int,Timer_Custom:int");
ScriptRegistrant<JFW_Set_Objective_Status> JFW_Set_Objective_Status_Registrant("JFW_Set_Objective_Status","Type:int,TypeVal:int,Objective_Num:int,Status:int,Timer_Custom:int");
ScriptRegistrant<JFW_Change_Objective_Type> JFW_Change_Objective_Type_Registrant("JFW_Change_Objective_Type","Type:int,TypeVal:int,Objective_Num:int,Objective_Type:int,Timer_Custom:int");
ScriptRegistrant<JFW_Set_Objective_Radar_Blip> JFW_Set_Objective_Radar_Blip_Registrant("JFW_Set_Objective_Radar_Blip","Type:int,TypeVal:int,Objective_Num:int,Pos:vector3,Timer_Custom:int");
ScriptRegistrant<JFW_Set_Objective_Radar_Blip_Object> JFW_Set_Objective_Radar_Blip_Object_Registrant("JFW_Set_Objective_Radar_Blip_Object","Type:int,TypeVal:int,Objective_Num:int,Object:int,Timer_Custom:int");
ScriptRegistrant<JFW_Set_Objective_HUD_Info> JFW_Set_Objective_HUD_Info_Registrant("JFW_Set_Objective_HUD_Info","Type:int,TypeVal:int,Objective_Num:int,Priority:float,Pog_File:string,Pog_Title_ID:int,Timer_Custom:int");
ScriptRegistrant<JFW_Set_Objective_HUD_Info_Position> JFW_Set_Objective_HUD_Info_Position_Registrant("JFW_Set_Objective_HUD_Info_Position","Type:int,TypeVal:int,Objective_Num:int,Priority:float,Pog_File:string,Pog_Title_ID:int,Position:vector3,Timer_Custom:int");
ScriptRegistrant<JFW_Set_Num_Tertary_Objectives> JFW_Set_Num_Tertary_Objectives_Registrant("JFW_Set_Num_Tertary_Objectives","Type:int,TypeVal:int,Count:int,Timer_Custom:int");
ScriptRegistrant<JFW_Mission_Complete> JFW_Mission_Complete_Registrant("JFW_Mission_Complete","Type:int,TypeVal:int,Won:int,Timer_Custom:int");
ScriptRegistrant<JFW_Start_Timer> JFW_Start_Timer_Registrant("JFW_Start_Timer","Type:int,TypeVal:int,Timer_Object:int,Timer_Number:int,Timer_Time:float,Repeat:int,Timer_Custom:int");
