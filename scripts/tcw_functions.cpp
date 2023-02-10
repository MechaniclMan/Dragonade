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
#include "tcw_functions.h"

bool Is_TCW_Harvester(GameObject *Obj)
{
	if ( strcmp(Commands->Get_Preset_Name(Obj),"TCW_GDI_Harvester_Player") == 0 || 
		 strcmp(Commands->Get_Preset_Name(Obj),"TCW_Nod_GDI_Harvester_Player") == 0 || 
		 strcmp(Commands->Get_Preset_Name(Obj),"TCW_GDI_Nod_Harvester_Player") == 0 || 
		 strcmp(Commands->Get_Preset_Name(Obj),"TCW_Nod_Harvester_Player") == 0 ||
		 strcmp(Commands->Get_Preset_Name(Obj),"TCW_Nod_Harvester") == 0 ||
		 strcmp(Commands->Get_Preset_Name(Obj),"TCW_GDI_Harvester") == 0 ) 
		return true;
	else
		return false;
}

bool Is_TCW_Engineer(GameObject *Obj)
{
	if ( strcmp(Commands->Get_Preset_Name(Obj),"TCW_Nod_Saboteur") == 0 || strcmp(Commands->Get_Preset_Name(Obj),"TCW_Nod_Saboteur_Low") == 0 || strcmp(Commands->Get_Preset_Name(Obj),"TCW_GDI_Engineer") == 0 || strcmp(Commands->Get_Preset_Name(Obj),"TCW_GDI_Engineer_Low") == 0 || strcmp(Commands->Get_Preset_Name(Obj),"TCW_GDI_Engineer_Coop") == 0 || strcmp(Commands->Get_Preset_Name(Obj),"TCW_Nod_Saboteur_Coop") == 0)
		return true;
	else
		return false;
}