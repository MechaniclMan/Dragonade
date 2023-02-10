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
#ifndef SCRIPTS_INCLUDE__ENGINE_SCRIPT_H
#define SCRIPTS_INCLUDE__ENGINE_SCRIPT_H
#include "scripts.h"
#include "SList.h"

SCRIPTS_API void Remove_Script(GameObject *obj,const char *script); //removes all copies of <script> from an object
SCRIPTS_API void Remove_All_Scripts(GameObject *obj); //removes all scripts from an object
SCRIPTS_API void Attach_Script_Preset(const char *script,const char *params,const char *preset,int team); //attached <script> to all objects of <preset> in team <team>
SCRIPTS_API void Attach_Script_Type(const char *script,const char *params,unsigned long type,int team); //attaches <script> to all objects of <type> in team <team>
SCRIPTS_API void Remove_Script_Preset(const char *script,const char *preset,int team); //removes all copies of <script> from all objects of <preset> in team <team>
SCRIPTS_API void Remove_Script_Type(const char *script,unsigned long type,int team); //removes all copies of <script> from all objects of <type> in team <team>
SCRIPTS_API bool Is_Script_Attached(GameObject *obj,const char *script); //is the script attached
SCRIPTS_API void Attach_Script_Once(GameObject *obj,const char *script,const char *params); //attach a script if its not already attached
SCRIPTS_API void Attach_Script_Preset_Once(const char *script,const char *params,const char *preset,int team); //attach a script to all objects of preset if its not already attached
SCRIPTS_API void Attach_Script_Type_Once(const char *script,const char *params,unsigned long type,int team); //attach a script to all objects of type if its not already attached
SCRIPTS_API void Attach_Script_Building(const char *script,const char *params,int team); //attach a script to all buildings
SCRIPTS_API void Attach_Script_Is_Preset(GameObject *obj,const char *preset,const char *script,const char *params,int team); //attach the script if object is of preset
SCRIPTS_API void Attach_Script_Is_Type(GameObject *obj,unsigned long type,const char *script,const char *params,int team); //attach the script if object is of type
SCRIPTS_API void Attach_Script_Player_Once(const char *script,const char *params,int team); //attach a script to all players if its not already attached
SCRIPTS_API void Remove_Duplicate_Script(GameObject *obj, const char *script); //remove duplicate scripts from an object
SCRIPTS_API void Attach_Script_All_Buildings_Team(int Team,const char *Script,const char *Params,bool Once); //attach a script to all buildings by team
SCRIPTS_API void Attach_Script_All_Turrets_Team(int Team,const char *Script,const char *Params,bool Once); //attach a script to all static vehicles by team
SCRIPTS_API GameObject *Find_Building_With_Script(int Team,int Type,const char *Script,GameObject *Caller); //Find a building of this type with this script attached to it
SCRIPTS_API GameObject *Find_Object_With_Script(const char *script); //Find the first object with this script on it
SCRIPTS_API GameObject *Find_Closest_Object_With_Script(const char *script, Vector3 pos); //Find the closest object to pos with this script on it
SCRIPTS_API void Find_All_Objects_With_Script(const char *script, SList<GameObject>& objects); // Find all objects with this script on it
SCRIPTS_API void Send_Custom_Event_To_Objects_With_Script( GameObject *sender, const char *script, int message, int param, float delay ); // Script to send a custom to all objects with a specific script
SCRIPTS_API void Send_Custom_Event_To_Objects_With_Script_Ranged( GameObject *sender, const char *script, int message, int param, float delay, float range ); // Script to send a custom to all objects with a specific script in a specified range
SCRIPTS_API ScriptImpClass* Find_Script_On_Object(GameObject* obj, const char *script); // Returns a pointer to the first instance of the named script on the given object, or NULL if not found
SCRIPTS_API void Attach_Script_Occupants(GameObject *obj,const char *script,const char *params); //attaches a script to all occupants of a vehicle
#endif
