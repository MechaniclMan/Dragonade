/*	Renegade Scripts.dll
	Copyright 2012-2013 zunnie
	Websites: http://www.MultiPlayerForums.com / http://www.UltraAOW.com
	ModDB Profile: http://www.moddb.com/mods/renegade-domination

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

/*
	This file contains dummy scripts which are required for the MPF Domination Plugin.
	They can be used in the Level Editor to setup new maps while using the MPF Domination Plugin as a base on the server.
*/

#include "General.h"
#include "scripts.h"
#include "engine.h"
#include "mpf_domination.h"

void DOM_Control_Point::Created(GameObject *obj)
{
	//Dummy Script needed in Level Editor to setup Control Points for use on servers with the MPF Domination Plugin enabled.
}

void DOM_Random_Spawn_Character::Created(GameObject *obj)
{
	//
}

ScriptRegistrant<DOM_Random_Spawn_Character> DOM_Random_Spawn_Character_Registrant("DOM_Random_Spawn_Character","");
ScriptRegistrant<DOM_Control_Point> DOM_Control_Point_Registrant("DOM_Control_Point","Pre_Team=-2:int,CP_Name=CP:string,Nod_Spawn1=0:int,Nod_Spawn2=0:int,Nod_Spawn3=0:int,Nod_Spawn4=0:int,GDI_Spawn1=0:int,GDI_Spawn2=0:int,GDI_Spawn3=0:int,GDI_Spawn4=0:int");
