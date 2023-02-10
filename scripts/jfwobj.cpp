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
#include "general.h"

#include "scripts.h"
#include "jfwobj.h"

struct TimerParams {
	int number;
	float time;
	int repeat;
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
	z.repeat = Get_Int_Parameter("Repeat");
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
	z.repeat = Get_Int_Parameter("Repeat");
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
	z.repeat = Get_Int_Parameter("Repeat");
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
	z.repeat = Get_Int_Parameter("Repeat");
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
	z.repeat = Get_Int_Parameter("Repeat");
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
	Commands->Mission_Complete(x);
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
	Commands->Mission_Complete(x);
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
	Commands->Mission_Complete(x);
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
	Commands->Mission_Complete(x);
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
	Commands->Mission_Complete(x);
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
	Commands->Mission_Complete(x);
}

void JFW_Mission_Complete::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&repeat,4,1);
	Auto_Save_Variable(&timertime,4,2);
	Auto_Save_Variable(&timernumber,4,3);
}

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
