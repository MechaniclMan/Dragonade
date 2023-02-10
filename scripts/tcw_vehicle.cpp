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
#include "tcw_vehicle.h"
#include "VehicleGameObj.h"
#include "WeaponBagClass.h"
#include "WeaponClass.h"
#include "GameObjManager.h"

void tcw_vehicle::Created(GameObject *obj)
{
	revealed = false;
	playdetected = true;
	playdamage = true;
	triggerstealthdetection = true;
	venomfxid = 0;
	gharv_idle_fxid = 0;
	gharv_harving_fxid = 0;
	gharv_full_fxid = 0;
	int random = Commands->Get_Random_Int(1,5);

	if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_APC") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_apc_1.wav",1);
			Send_Message_Team(1,255,204,0,"APC ready for combat.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_apc_2.wav",1);
			Send_Message_Team(1,255,204,0,"APC what's the plan.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_apc_1.wav",1);
			Send_Message_Team(1,255,204,0,"APC ready for combat.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_apc_3.wav",1);
			Send_Message_Team(1,255,204,0,"APC standing by.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_APC") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_apc_1.wav",0);
			Send_Message_Team(0,255,0,0,"APC ready for combat.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_apc_2.wav",0);
			Send_Message_Team(0,255,0,0,"APC what's the plan.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_apc_1.wav",0);
			Send_Message_Team(0,255,0,0,"APC ready for combat.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_apc_3.wav",0);
			Send_Message_Team(0,255,0,0,"APC standing by.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Venom") == 0)
	{
		GameObject *venomfx = Commands->Create_Object("TCW_Nod_Venom_FX",Commands->Get_Position(obj));
		venomfxid = Commands->Get_ID(venomfx);
		Commands->Attach_To_Object_Bone(venomfx,obj,"FX");
		Commands->Set_Animation(venomfx,"V_NOD_VENOM_AG.V_NOD_VENOM_AG",false,0,2,2,false);//engine off
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_venom_1.wav",0);
			Send_Message_Team(0,255,0,0,"Venom craft ready for lift-off.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_venom_1.wav",0);
			Send_Message_Team(0,255,0,0,"Venom craft ready for lift-off.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Orca") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_orca_1.wav",1);
			Send_Message_Team(1,255,204,0,"Orca ready, sir.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_orca_2.wav",1);
			Send_Message_Team(1,255,204,0,"Orca ready for lift-off.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_orca_3.wav",1);
			Send_Message_Team(1,255,204,0,"Orca cleared for take off.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_orca_4.wav",1);
			Send_Message_Team(1,255,204,0,"Orca here.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Harvester") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_harvester_1.wav",1);
			Send_Message_Team(1,255,204,0,"Let's get to that tib-zone.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_harvester_1.wav",1);
			Send_Message_Team(1,255,204,0,"Let's get to that tib-zone.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_harvester_2.wav",1);
			Send_Message_Team(1,255,204,0,"Which way to tiberium?");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_harvester_3.wav",1);
			Send_Message_Team(1,255,204,0,"Harvesting operations.");
		}
		Commands->Send_Custom_Event(obj,obj,400400120,1,0);//set idle animation on create
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Harvester_Player") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_harvester_1.wav",1);
			Send_Message_Team(1,255,204,0,"Let's get to that tib-zone.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_harvester_1.wav",1);
			Send_Message_Team(1,255,204,0,"Let's get to that tib-zone.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_harvester_2.wav",1);
			Send_Message_Team(1,255,204,0,"Which way to tiberium?");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_harvester_3.wav",1);
			Send_Message_Team(1,255,204,0,"Harvesting operations.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_GDI_Harvester_Player") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_harvester_1.wav",0);
			Send_Message_Team(0,255,0,0,"Let's get to that tib-zone.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_harvester_1.wav",0);
			Send_Message_Team(0,255,0,0,"Let's get to that tib-zone.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_harvester_2.wav",0);
			Send_Message_Team(0,255,0,0,"Which way to tiberium?");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_harvester_3.wav",0);
			Send_Message_Team(0,255,0,0,"Harvesting operations.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Juggernaut") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_juggernaut_1.wav",1);
			Send_Message_Team(1,255,204,0,"Looking for the big guns?");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_juggernaut_2.wav",1);
			Send_Message_Team(1,255,204,0,"Juggernaut standing by.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_juggernaut_2.wav",1);
			Send_Message_Team(1,255,204,0,"Juggernaut standing by.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Mammoth_Tank") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_mammoth_1.wav",1);
			Send_Message_Team(1,255,204,0,"Mammoth Tank waiting orders.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_mammoth_2.wav",1);
			Send_Message_Team(1,255,204,0,"Mammoth Tank ready.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_mammoth_3.wav",1);
			Send_Message_Team(1,255,204,0,"Mammoth Tank assembled.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Mammoth_Tank") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_mammoth_1.wav",0);
			Send_Message_Team(0,255,0,0,"Mammoth Tank waiting orders.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_mammoth_2.wav",0);
			Send_Message_Team(0,255,0,0,"Mammoth Tank ready.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_mammoth_3.wav",0);
			Send_Message_Team(0,255,0,0,"Mammoth Tank assembled.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Pitbull") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_pitbull_1.wav",1);
			Send_Message_Team(1,255,204,0,"Pitbull ready for action.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_pitbull_1.wav",1);
			Send_Message_Team(1,255,204,0,"Pitbull ready for action.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_pitbull_2.wav",1);
			Send_Message_Team(1,255,204,0,"Pitbull looking for some action.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_pitbull_3.wav",1);
			Send_Message_Team(1,255,204,0,"Ready to tear it up!");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Pitbull") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_pitbull_1.wav",0);
			Send_Message_Team(0,255,0,0,"Pitbull ready for action.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_pitbull_1.wav",0);
			Send_Message_Team(0,255,0,0,"Pitbull ready for action.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_pitbull_2.wav",0);
			Send_Message_Team(0,255,0,0,"Pitbull looking for some action.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_pitbull_3.wav",0);
			Send_Message_Team(0,255,0,0,"Ready to tear it up!");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Predator_Tank") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_predator_1.wav",1);
			Send_Message_Team(1,255,204,0,"Predator rolling out.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_predator_1.wav",1);
			Send_Message_Team(1,255,204,0,"Predator rolling out.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_predator_2.wav",1);
			Send_Message_Team(1,255,204,0,"Predator crew ready to roll.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Predator_Tank") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_predator_1.wav",0);
			Send_Message_Team(0,255,0,0,"Predator rolling out.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_predator_1.wav",0);
			Send_Message_Team(0,255,0,0,"Predator rolling out.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_gdi_predator_2.wav",0);
			Send_Message_Team(0,255,0,0,"Predator crew ready to roll.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Attack_Bike") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_attackbike_1.wav",0);
			Send_Message_Team(0,255,0,0,"This bike's ready.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_attackbike_2.wav",0);
			Send_Message_Team(0,255,0,0,"Let's go for a hunt.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_attackbike_2.wav",0);
			Send_Message_Team(0,255,0,0,"Let's go for a hunt.");
		}
		else if (random == 4)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_attackbike_3.wav",0);
			Send_Message_Team(0,255,0,0,"Ya i'm ready.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_attackbike_4.wav",0);
			Send_Message_Team(0,255,0,0,"No one's gonna save you now.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Attack_Bike") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_attackbike_1.wav",1);
			Send_Message_Team(1,255,204,0,"This bike's ready.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_attackbike_2.wav",1);
			Send_Message_Team(1,255,204,0,"Let's go for a hunt.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_attackbike_2.wav",1);
			Send_Message_Team(1,204,0,0,"Let's go for a hunt.");
		}
		else if (random == 4)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_attackbike_3.wav",1);
			Send_Message_Team(1,204,0,0,"Ya i'm ready.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_attackbike_4.wav",1);
			Send_Message_Team(1,255,204,0,"No one's gonna save you now.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Raider_Buggy") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_raider_buggy_1.wav",0);
			Send_Message_Team(0,255,0,0,"Raider here.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_raider_buggy_1.wav",0);
			Send_Message_Team(0,255,0,0,"Raider here.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_raider_buggy_2.wav",0);
			Send_Message_Team(0,255,0,0,"Let's go on a raid.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_raider_buggy_3.wav",0);
			Send_Message_Team(0,255,0,0,"When's the next raid.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Raider_Buggy") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_raider_buggy_1.wav",1);
			Send_Message_Team(1,255,204,0,"Raider here.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_raider_buggy_1.wav",1);
			Send_Message_Team(1,255,204,0,"Raider here.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_raider_buggy_2.wav",1);
			Send_Message_Team(1,255,204,0,"Let's go on a raid.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_raider_buggy_3.wav",1);
			Send_Message_Team(1,255,204,0,"When's the next raid.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Scorpion_Tank") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_scorpion_1.wav",0);
			Send_Message_Team(0,255,0,0,"The scorpion's ready.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_scorpion_2.wav",0);
			Send_Message_Team(0,255,0,0,"Scorpion ready for battle.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_scorpion_2.wav",0);
			Send_Message_Team(0,255,0,0,"Scorpion ready for battle.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_scorpion_3.wav",0);
			Send_Message_Team(0,255,0,0,"Scorpion Tank.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Scorpion_Tank") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_scorpion_1.wav",1);
			Send_Message_Team(1,255,204,0,"The scorpion's ready.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_scorpion_2.wav",1);
			Send_Message_Team(1,255,204,0,"Scorpion ready for battle.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_scorpion_2.wav",1);
			Send_Message_Team(1,255,204,0,"Scorpion ready for battle.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_scorpion_3.wav",1);
			Send_Message_Team(1,255,204,0,"Scorpion Tank.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Stealth_Tank") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_stealth_tank_1.wav",0);
			Send_Message_Team(0,255,0,0,"Stealth tank clear for combat.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_stealth_tank_1.wav",0);
			Send_Message_Team(0,255,0,0,"Stealth tank clear for combat.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_stealth_tank_2.wav",0);
			Send_Message_Team(0,255,0,0,"Stay unseen.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Stealth_Tank") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_stealth_tank_1.wav",1);
			Send_Message_Team(1,255,204,0,"Stealth tank clear for combat.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_stealth_tank_1.wav",1);
			Send_Message_Team(1,255,204,0,"Stealth tank clear for combat.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_stealth_tank_2.wav",1);
			Send_Message_Team(1,255,204,0,"Stay unseen.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Flame_Tank") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_flame_tank_1.wav",0);
			Send_Message_Team(0,255,0,0,"The flames will rise.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_flame_tank_1.wav",0);
			Send_Message_Team(0,255,0,0,"The flames will rise.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_flame_tank_2.wav",0);
			Send_Message_Team(0,255,0,0,"Do not toy with the power of the flame.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_flame_tank_3.wav",0);
			Send_Message_Team(0,255,0,0,"Flame Tank.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Flame_Tank") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_flame_tank_1.wav",1);
			Send_Message_Team(1,255,204,0,"The flames will rise.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_flame_tank_1.wav",1);
			Send_Message_Team(1,255,204,0,"The flames will rise.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_flame_tank_2.wav",1);
			Send_Message_Team(1,255,204,0,"Do not toy with the power of the flame.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_flame_tank_3.wav",1);
			Send_Message_Team(1,255,204,0,"Flame Tank.");
		}
	}
	else if ( (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Harvester") == 0) || 
			  (_stricmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Harvester_Player") == 0) )
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_harvester_1.wav",0);
			Send_Message_Team(0,255,0,0,"It is time for a harvest.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_harvester_1.wav",0);
			Send_Message_Team(0,255,0,0,"It is time for a harvest.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_harvester_2.wav",0);
			Send_Message_Team(0,255,0,0,"Tiberium is power.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_harvester_3.wav",0);
			Send_Message_Team(0,255,0,0,"The crystals are calling.");
		}
	}
	else if (_stricmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Nod_Harvester_Player") == 0)
	{
		if (random == 1)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_harvester_1.wav",1);
			Send_Message_Team(1,255,204,0,"It is time for a harvest.");
		}
		else if (random == 2)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_harvester_1.wav",1);
			Send_Message_Team(1,255,204,0,"It is time for a harvest.");
		}
		else if (random == 3)
		{
			Create_2D_WAV_Sound_Team("purchase_nod_harvester_2.wav",1);
			Send_Message_Team(1,255,204,0,"Tiberium is power.");
		}
		else
		{
			Create_2D_WAV_Sound_Team("purchase_nod_harvester_3.wav",1);
			Send_Message_Team(1,255,204,0,"The crystals are calling.");
		}
	}
}
void tcw_vehicle::Destroyed(GameObject *obj)
{
	if (venomfxid)
	{
		Commands->Destroy_Object(Commands->Find_Object(venomfxid));
	}
}
void tcw_vehicle::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if ( (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Driver(obj)),"TCW_GDI_Commando") == 0) || (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Driver(obj)),"TCW_GDI_Commando_Low") == 0) || 
			 (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Driver(obj)),"TCW_Nod_Commando") == 0) || (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Driver(obj)),"TCW_Nod_Commando_Low") == 0) ||
			 (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Driver(obj)),"TCW_GDI_Zonetrooper") == 0) || (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Driver(obj)),"TCW_GDI_Zonetrooper_Low") == 0) )
		{
			Commands->Start_Timer(obj,this,0.5f,100100);
		}
		else if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Pitbull") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Pitbull") == 0)  || 
			 (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Attack_Bike") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Attack_Bike") == 0) )
		{
			Commands->Start_Timer(obj,this,5.0f,1004);
			triggerstealthdetection = true;
		}
		else if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_APC") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_APC") == 0) )
		{
			if (Get_Vehicle_Occupant_Count(obj) == 2)
			{
				Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(obj),"apc_full.wav");
				Create_2D_WAV_Sound_Player(Get_Vehicle_Occupant(obj,1),"apc_full.wav");
			}
			/*if (Get_Vehicle_Occupant(obj,1))//if there is a passenger
			{
				if ( (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Occupant(obj,1)),"TCW_GDI_Missile_Squad") == 0) || (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Occupant(obj,1)),"TCW_Nod_RocketGuy") == 0) || (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Occupant(obj,1)),"TCW_Nod_RocketGuy_Low") == 0) || (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Occupant(obj,1)),"TCW_GDI_Missile_Squad_Low") == 0) )
				{
					Commands->Give_PowerUp(obj,"TCW_POW_APC_Missile_Weapon",false);
					Commands->Select_Weapon(obj,"TCW_Weapon_APC_Missile");
				}
				else if ( (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Occupant(obj,1)),"TCW_GDI_Sniper") == 0) || (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Occupant(obj,1)),"TCW_GDI_Sniper_Low") == 0) || (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Occupant(obj,1)),"TCW_Nod_Sniper") == 0) )
				{
					Commands->Give_PowerUp(obj,"TCW_POW_APC_Sniper_Weapon",false);
					Commands->Select_Weapon(obj,"TCW_Weapon_APC_Sniper");
				}
				else if ( (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Occupant(obj,1)),"TCW_GDI_Zonetrooper") == 0) || (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Occupant(obj,1)),"TCW_GDI_Zonetrooper_Low") == 0) || (strcmp(Commands->Get_Preset_Name(Get_Vehicle_Occupant(obj,1)),"TCW_Nod_Zonetrooper") == 0) )
				{
					Commands->Give_PowerUp(obj,"TCW_POW_APC_Zonetrooper_Weapon",false);
					Commands->Select_Weapon(obj,"TCW_Weapon_APC_Zonetrooper");
				}
			}*/
		}
		else if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Chinook") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Chinook") == 0) )
		{
			if (Get_Vehicle_Occupant_Count(obj) == 5)
			{
				Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(obj),"apc_full.wav");
				Create_2D_WAV_Sound_Player(Get_Vehicle_Occupant(obj,1),"apc_full.wav");
				Create_2D_WAV_Sound_Player(Get_Vehicle_Occupant(obj,2),"apc_full.wav");
				Create_2D_WAV_Sound_Player(Get_Vehicle_Occupant(obj,3),"apc_full.wav");
				Create_2D_WAV_Sound_Player(Get_Vehicle_Occupant(obj,4),"apc_full.wav");
			}
		}
		else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Venom") == 0)
		{
			Commands->Set_Animation(Commands->Find_Object(venomfxid),"V_NOD_VENOM_AG.V_NOD_VENOM_AG",false,0,1,1,false);//engine on
		}
	}
	else if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (Commands->Get_Player_Type(sender) == 0)
		{
			Commands->Set_Player_Type(obj,0);//set it to exiter team to allow killing by enemy and still gaining vet points
			Commands->Start_Timer(obj,this,5.0f,1008);//set to neutral if there is no driver found
		}
		else if (Commands->Get_Player_Type(sender) == 1)
		{
			Commands->Set_Player_Type(obj,1);//set it to exiter team to allow killing by enemy and still gaining vet points
			Commands->Start_Timer(obj,this,5.0f,1008);//set to neutral if there is no driver found
		}

		if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Pitbull") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Pitbull") == 0) )
		{
			triggerstealthdetection = false;
		}
		else if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Harvester") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_GDI_Harvester") == 0) )
		{
			Commands->Set_Player_Type(obj,0);
		}
		else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Nod_Harvester") == 0)
		{
			Commands->Set_Player_Type(obj,1);
		}
		else if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_APC") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_APC") == 0) )
		{
			if (!Get_Vehicle_Occupant(obj,1))//if there is no passenger
			{
				Commands->Select_Weapon(obj,"TCW_Weapon_APC");//let's always select the normal weapon when someone exits the vehicle
			}
		}
		else if (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Venom") == 0)
		{
			Commands->Set_Animation(Commands->Find_Object(venomfxid),"V_NOD_VENOM_AG.V_NOD_VENOM_AG",false,0,2,2,false);//engine off
		}
	}
	else if (message == 1004)//destealth unit
	{ 
		if (param == 1)
		{
			if (strcmp(Commands->Get_Preset_Name(sender),"TCW_GDI_Command_Post") == 0)
			{
				if (!revealed)
				{
					Commands->Enable_Stealth(obj, false);
					Commands->Start_Timer(obj,this,2.0f,1005);//enable stealh again
					if (playdetected)
					{
						if (Commands->Get_Player_Type(obj) == 0)
						{
							Create_2D_WAV_Sound_Team("stealth_unit_detected_gdi.wav",1);
							Send_Message_Team(1,255,204,0,"Stealth unit detected.");
						}
						else if (Commands->Get_Player_Type(obj) == 1)
						{
							Create_2D_WAV_Sound_Team("stealth_unit_detected_nod.wav",0);
							Send_Message_Team(0,255,0,0,"Stealth unit detected.");
						}
						playdetected = false;
						Commands->Start_Timer(obj,this,10.0f,1006);//playdetected true
					}	
					revealed = true;
				}
			}
			else
			{
				if (Commands->Get_Player_Type(obj) != Commands->Get_Player_Type(sender))
				{
					Vector3 mypos = Commands->Get_Position(obj);
					Vector3 target = Commands->Get_Position(sender);
					float distance = Commands->Get_Distance(mypos,target);

					if (distance < 76.0f)
					{
						if (!revealed)
						{
							Commands->Enable_Stealth(obj, false);
							Commands->Start_Timer(obj,this,2.0f,1005);//enable stealh again
							if (playdetected)
							{
								if (Commands->Get_Player_Type(obj) == 0)
								{
									Create_2D_WAV_Sound_Team("stealth_unit_detected_gdi.wav",1);
									Send_Message_Team(1,255,204,0,"Stealth unit detected.");
								}
								else if (Commands->Get_Player_Type(obj) == 1)
								{
									Create_2D_WAV_Sound_Team("stealth_unit_detected_nod.wav",0);
									Send_Message_Team(0,255,0,0,"Stealth unit detected.");
								}
								playdetected = false;
								Commands->Start_Timer(obj,this,10.0f,1006);//playdetected true
							}	
							revealed = true;
						}
					}
				}
			}
		}
	}
}
void tcw_vehicle::Timer_Expired(GameObject *obj, int number)
{
	if (number == 100100)
	{
		Force_Occupant_Exit(obj,0);
	}
	else if (number == 1004)
	{
		if (Get_Vehicle_Occupant(obj,0))//fix empty pitbull stealthdetection errors
		{
			Send_Custom_Event_To_Object(obj,"TCW_Nod_Shadow",1004,1,0);
			Send_Custom_Event_To_Object(obj,"TCW_Nod_Shadow_Low",1004,1,0);
			Send_Custom_Event_To_Object(obj,"TCW_Nod_Stealth_Tank",1004,1,0);
			Send_Custom_Event_To_Object(obj,"TCW_GDI_Stealth_Tank",1004,1,0);
			Send_Custom_Event_To_Object(obj,"TCW_Nod_Harvester",1004,1,0);
			Send_Custom_Event_To_Object(obj,"TCW_Nod_Harvester_Player",1004,1,0);
			Send_Custom_Event_To_Object(obj,"TCW_GDI_Nod_Harvester_Player",1004,1,0);
			Send_Custom_Event_To_Object(obj,"TCW_GDI_Sniper",1004,1,0);
			Send_Custom_Event_To_Object(obj,"TCW_GDI_Sniper_Low",1004,1,0);
			Send_Custom_Event_To_Object(obj,"TCW_Nod_Commando",1004,1,0);
			Send_Custom_Event_To_Object(obj,"TCW_Nod_Commando_Low",1004,1,0);
			if (triggerstealthdetection)
			{
				if ( (Commands->Get_Player_Type(obj) == 0) || (Commands->Get_Player_Type(obj) == 1) )
				{
					Commands->Start_Timer(obj,this,5.0f,1004);
				}
			}
		}
	}
	else if (number == 1005)//enable stealth again after distance check 
	{
		Commands->Enable_Stealth(obj, true);
		revealed = false;
	}
	else if (number == 1006)
	{
		playdetected = true;
	}
	else if (number == 1007)
	{
		playdamage = true;
	}
	else if (number == 1008)
	{
		if (Get_Vehicle_Occupant_Count(obj) == 0)
		{
			Commands->Set_Player_Type(obj,-2);//unteam the tank after 5 seconds
		}
	}
}
void tcw_vehicle::Killed(GameObject *obj, GameObject *shooter)
{
	if (Commands->Get_Player_Type(obj) == 0)
	{
		if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Harvester") == 0) || (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Harvester_Player") == 0) ||  (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_GDI_Harvester_Player") == 0) )
		{
			Create_2D_WAV_Sound_Team("nod_harvester_lost.wav",0);
			Send_Message_Team(0,255,0,0,"Harvester lost.");
		}
		else
		{
#ifdef SSGM
			char killmsg[250];
			sprintf(killmsg,"Unit lost. (%s)",Get_Translated_Preset_Name_Ex(obj));
			Send_Message_Team(0,255,0,0,killmsg);
#endif
			Create_2D_WAV_Sound_Team("nod_unit_lost.wav",0);			
		}
	}
	else if (Commands->Get_Player_Type(obj) == 1)
	{
		if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Harvester") == 0) || 
			 (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Harvester_Player") == 0) || 
			 (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Nod_Harvester_Player") == 0) )
		{
			Create_2D_WAV_Sound_Team("gdi_harvester_lost.wav",1);
			Send_Message_Team(1,255,204,0,"Harvester lost.");
		}

		else
		{
#ifdef SSGM
			char killmsg[250];
			sprintf(killmsg,"Unit lost. (%s)",Get_Translated_Preset_Name_Ex(obj));
			Send_Message_Team(1,255,204,0,killmsg);
#endif
			Create_2D_WAV_Sound_Team("gdi_unit_lost.wav",1);
			if (venomfxid)
			{
				Commands->Destroy_Object(Commands->Find_Object(venomfxid));
			}
		}
	}
}
void tcw_vehicle::Damaged(GameObject *obj, GameObject *damager, float damage)
{
	if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Harvester") == 0) || 
		 (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Harvester_Player") == 0) || 
		 (strcmp(Commands->Get_Preset_Name(obj),"TCW_GDI_Nod_Harvester_Player") == 0) )
	{
		if (playdamage)
		{
			if (damage > 0.0)
			{
				playdamage = false;
				Create_2D_WAV_Sound_Team("gdi_harvester_under_attack.wav",1);
				Send_Message_Team(1,255,204,0,"Harvester under attack.");
				Commands->Start_Timer(obj,this,20.0f,1007);//playdamage true
			}
		}
	}
	else if ( (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Harvester") == 0) || 
			  (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_Harvester_Player") == 0)  || 
			  (strcmp(Commands->Get_Preset_Name(obj),"TCW_Nod_GDI_Harvester_Player") == 0) )
	{
		if (playdamage)
		{
			if (damage > 0.0)
			{
				playdamage = false;
				Create_2D_WAV_Sound_Team("nod_harvester_under_attack.wav",0);
				Send_Message_Team(0,255,0,0,"Harvester under attack.");
				Commands->Start_Timer(obj,this,20.0f,1007);//playdamage true
			}
		}
	}
}
void tcw_vehicle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&triggerstealthdetection,sizeof(triggerstealthdetection),1);
	Auto_Save_Variable(&playdetected,sizeof(playdetected),2);
	Auto_Save_Variable(&revealed,sizeof(revealed),3);
	Auto_Save_Variable(&playdamage,sizeof(playdamage),4);
}

void tcw_vehexit_killtimer::Created(GameObject *obj)
{
	timerid = Commands->Get_ID(obj);
	driverid = 0;
}
void tcw_vehexit_killtimer::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		driverid = Commands->Get_ID(Get_Vehicle_Driver(obj));
		//driverid = Commands->Get_ID(sender);
	}
	else if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Start_Timer(obj,this,30.0f,timerid);
	}
}
void tcw_vehexit_killtimer::Timer_Expired(GameObject *obj, int number)
{
	if (number == timerid)
	{
		if (!Commands->Find_Object(driverid))
		{
			Commands->Destroy_Object(obj);
		}
		else
		{
			Commands->Start_Timer(obj,this,30.0f,timerid);
		}
	}
}
void tcw_vehexit_killtimer::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&timerid,sizeof(timerid),1);
	Auto_Save_Variable(&driverid,sizeof(driverid),2);
}

void tcw_hijacked_nod::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (Commands->Get_Player_Type(sender) == 1)
		{
			Create_2D_WAV_Sound_Player(sender,"hijacked.wav");
			Send_Message_Player(sender,255,255,255,"You have hijacked an enemy vehicle, good job!");
			Destroy_Script();
		}
	}
}
void tcw_hijacked_gdi::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (Commands->Get_Player_Type(sender) == 0)
		{
			Create_2D_WAV_Sound_Player(sender,"hijacked.wav");
			Send_Message_Player(sender,255,255,255,"You have hijacked an enemy vehicle, good job!");
			Destroy_Script();
		}
	}
}

void tcw_bunker_zone::Created(GameObject *obj)
{
	ExpLoc1 = Get_Int_Parameter("ExpLoc1");	ExpLoc2 = Get_Int_Parameter("ExpLoc2");	ExpLoc3 = Get_Int_Parameter("ExpLoc3;");
}
void tcw_bunker_zone::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	/*
	if(type == 5089123)//probeer dit nog eens
	{
		GameObject *Damager = Commands->Find_Object(param);
		SLNode<SoldierGameObj> *Star_Node = GameObjManager::StarGameObjList.Head();
		for(; Star_Node; Star_Node = Star_Node->Next())
		{
			SoldierGameObj *Star = Star_Node->Data();
			if(Star && IsInsideZone(obj, Star))
				Commands->Apply_Damage(Star, 99999, "BlamoKiller", Damager); 
		}
		const char *Explosion = "TCW_Explosion_Shell_Mammoth";
		if(ExpLoc1)
			Commands->Create_Explosion(Explosion, Commands->Get_Position(Commands->Find_Object(ExpLoc1)), Damager);
		if(ExpLoc2)
			Commands->Create_Explosion(Explosion, Commands->Get_Position(Commands->Find_Object(ExpLoc2)), Damager);
		if(ExpLoc3)
			Commands->Create_Explosion(Explosion, Commands->Get_Position(Commands->Find_Object(ExpLoc3)), Damager);
	}
	*/
}

void tcw_bunker::Created(GameObject *obj)
{
	ZoneID = Get_Int_Parameter("ZoneID");
}
#pragma warning (disable: 4189)
void tcw_bunker::Damaged(GameObject *obj,GameObject *damager,float amount)
{
	const WeaponDefinitionClass *WepDef = Get_Current_Weapon_Definition(damager);
	/*if(WepDef)
	{
		const char *Name = WepDef->Get_Name();
		if(strcmp(Name, "TCW_Weapon_Grenades") == 0)
			Commands->Send_Custom_Event(obj, Commands->Find_Object(ZoneID), 5089123, Commands->Get_ID(damager), 0);
	}*/
}

void tcw_apc_minefield_lay::Created(GameObject *obj)
{
	Minefield_Available = false; Commands->Start_Timer(obj,this,100.0f,155155);
}
void tcw_apc_minefield_lay::Timer_Expired(GameObject *obj, int number)
{
	if (number == 155155)
	{
		Minefield_Available = true;
		if (Get_Vehicle_Driver(obj))
		{
			Send_Message_Player(Get_Vehicle_Driver(obj),255,255,255,"Minefield Available");
			Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(obj),"incmsg.wav");
		}
	}
	else if (number == 155161)
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			Send_Message_Player(Get_Vehicle_Driver(obj),255,255,255,"Laying Invisible Mine Section 1!");
			Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(obj),"gdi_minefield_deploy.wav");
			Vector3 Size = Vector3(10.0f,10.0f,4.0f); 
			Matrix3 Rotation(true);
			Rotation.Rotate_Z(Commands->Get_Facing(obj));
			OBBoxClass Box(Commands->Get_Position(obj),Size,Rotation);
			GameObject *Zone = Create_Zone("Zone_GDI_APC_Mine",Box);
			Commands->Set_Facing(Zone,Commands->Get_Facing(obj));
			Commands->Start_Timer(obj,this,0.2f,155162);
		}
		else if (Commands->Get_Player_Type(obj) == 0)
		{
			Send_Message_Player(Get_Vehicle_Driver(obj),255,255,255,"Laying Invisible Mine Section 1!");
			Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(obj),"gdi_minefield_deploy.wav");
			Vector3 Size = Vector3(10.0f,10.0f,4.0f); 
			Matrix3 Rotation(true);
			Rotation.Rotate_Z(Commands->Get_Facing(obj));
			OBBoxClass Box(Commands->Get_Position(obj),Size,Rotation);
			GameObject *Zone = Create_Zone("Zone_Nod_APC_Mine",Box);
			Commands->Set_Facing(Zone,Commands->Get_Facing(obj));
			Commands->Start_Timer(obj,this,0.2f,155162);
		}
	}
	else if (number == 155162)
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			Send_Message_Player(Get_Vehicle_Driver(obj),255,255,255,"Laying Invisible Mine Section 2!");
			Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(obj),"gdi_minefield_deploy.wav");
			Vector3 Size = Vector3(10.0f,10.0f,4.0f); 
			Matrix3 Rotation(true);
			Rotation.Rotate_Z(Commands->Get_Facing(obj));
			OBBoxClass Box(Commands->Get_Position(obj),Size,Rotation);
			GameObject *Zone = Create_Zone("Zone_GDI_APC_Mine",Box);
			Commands->Set_Facing(Zone,Commands->Get_Facing(obj));
			Commands->Start_Timer(obj,this,0.2f,155163);
		}
		else if (Commands->Get_Player_Type(obj) == 0)
		{
			Send_Message_Player(Get_Vehicle_Driver(obj),255,255,255,"Laying Invisible Mine Section 2!");
			Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(obj),"gdi_minefield_deploy.wav");
			Vector3 Size = Vector3(10.0f,10.0f,4.0f); 
			Matrix3 Rotation(true);
			Rotation.Rotate_Z(Commands->Get_Facing(obj));
			OBBoxClass Box(Commands->Get_Position(obj),Size,Rotation);
			GameObject *Zone = Create_Zone("Zone_Nod_APC_Mine",Box);
			Commands->Set_Facing(Zone,Commands->Get_Facing(obj));
			Commands->Start_Timer(obj,this,0.2f,155163);
		}
	}
	else if (number == 155163)
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			Send_Message_Player(Get_Vehicle_Driver(obj),255,255,255,"Laying Invisible Mine Section 3!");
			Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(obj),"gdi_minefield_deploy.wav");
			Vector3 Size = Vector3(10.0f,10.0f,4.0f); 
			Matrix3 Rotation(true);
			Rotation.Rotate_Z(Commands->Get_Facing(obj));
			OBBoxClass Box(Commands->Get_Position(obj),Size,Rotation);
			GameObject *Zone = Create_Zone("Zone_GDI_APC_Mine",Box);
			Commands->Set_Facing(Zone,Commands->Get_Facing(obj));
			Commands->Start_Timer(obj,this,0.2f,155164);
		}
		else if (Commands->Get_Player_Type(obj) == 0)
		{
			Send_Message_Player(Get_Vehicle_Driver(obj),255,255,255,"Laying Invisible Mine Section 3!");
			Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(obj),"gdi_minefield_deploy.wav");
			Vector3 Size = Vector3(10.0f,10.0f,4.0f); 
			Matrix3 Rotation(true);
			Rotation.Rotate_Z(Commands->Get_Facing(obj));
			OBBoxClass Box(Commands->Get_Position(obj),Size,Rotation);
			GameObject *Zone = Create_Zone("Zone_Nod_APC_Mine",Box);
			Commands->Set_Facing(Zone,Commands->Get_Facing(obj));
			Commands->Start_Timer(obj,this,0.2f,155164);
		}
	}
	else if (number == 155164)
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			Send_Message_Player(Get_Vehicle_Driver(obj),255,255,255,"Laying Invisible Mine Section 4!");
			Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(obj),"gdi_minefield_deploy.wav");
			Vector3 Size = Vector3(10.0f,10.0f,4.0f); 
			Matrix3 Rotation(true);
			Rotation.Rotate_Z(Commands->Get_Facing(obj));
			OBBoxClass Box(Commands->Get_Position(obj),Size,Rotation);
			GameObject *Zone = Create_Zone("Zone_GDI_APC_Mine",Box);
			Commands->Set_Facing(Zone,Commands->Get_Facing(obj));
			Commands->Start_Timer(obj,this,0.2f,155165);
		}
		else if (Commands->Get_Player_Type(obj) == 0)
		{
			Send_Message_Player(Get_Vehicle_Driver(obj),255,255,255,"Laying Invisible Mine Section 4!");
			Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(obj),"gdi_minefield_deploy.wav");
			Vector3 Size = Vector3(10.0f,10.0f,4.0f); 
			Matrix3 Rotation(true);
			Rotation.Rotate_Z(Commands->Get_Facing(obj));
			OBBoxClass Box(Commands->Get_Position(obj),Size,Rotation);
			GameObject *Zone = Create_Zone("Zone_Nod_APC_Mine",Box);
			Commands->Set_Facing(Zone,Commands->Get_Facing(obj));
			Commands->Start_Timer(obj,this,0.2f,155165);
		}
	}
	else if (number == 155165)
	{
		if (Commands->Get_Player_Type(obj) == 1)
		{
			Send_Message_Player(Get_Vehicle_Driver(obj),255,255,255,"Laying Invisible Mine Section 5!");
			Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(obj),"gdi_minefield_deploy.wav");
			Vector3 Size = Vector3(10.0f,10.0f,4.0f); 
			Matrix3 Rotation(true);
			Rotation.Rotate_Z(Commands->Get_Facing(obj));
			OBBoxClass Box(Commands->Get_Position(obj),Size,Rotation);
			GameObject *Zone = Create_Zone("Zone_GDI_APC_Mine",Box);
			Commands->Set_Facing(Zone,Commands->Get_Facing(obj));
			Commands->Start_Timer(obj,this,100.0f,155155);
		}
		else if (Commands->Get_Player_Type(obj) == 0)
		{
			Send_Message_Player(Get_Vehicle_Driver(obj),255,255,255,"Laying Invisible Mine Section 5!");
			Create_2D_WAV_Sound_Player(Get_Vehicle_Driver(obj),"gdi_minefield_deploy.wav");
			Vector3 Size = Vector3(10.0f,10.0f,4.0f); 
			Matrix3 Rotation(true);
			Rotation.Rotate_Z(Commands->Get_Facing(obj));
			OBBoxClass Box(Commands->Get_Position(obj),Size,Rotation);
			GameObject *Zone = Create_Zone("Zone_Nod_APC_Mine",Box);
			Commands->Set_Facing(Zone,Commands->Get_Facing(obj));
			Commands->Start_Timer(obj,this,100.0f,155155);
		}
	}
}
void tcw_apc_minefield_lay::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == 101055)
	{
		if (param == 1)
		{
			if (Minefield_Available)
			{
				/*int team = Get_Object_Type(obj);
				int mines = Get_APC_Mine_Count(team);
				if (mines < 30)
				{
					char minemsg[512]; sprintf(minemsg,"%d/30 mines placed.",mines);
					Send_Message_Player(Get_Vehicle_Driver(obj),255,255,255,minemsg);
					Minefield_Available = false;	Commands->Start_Timer(obj,this,0.1f,155161);
				}
				else
				{
					char minemsg[512]; sprintf(minemsg,"%d/30 mines placed. Maximum Reached. Try again later.",mines);
					Send_Message_Player(Get_Vehicle_Driver(obj),255,255,255,minemsg);					
				}*/
			}
		}
	}
}

void tcw_apc_minefield::Entered(GameObject *obj, GameObject *enter)
{
	int team = Get_Int_Parameter("Team");
	if (team == 0)
	{
		if (Commands->Get_Player_Type(enter) == 1)
		{
			const char *Explosion = "TCW_Explosion_Shell_Mammoth";
			Vector3 ExpLoc1 = Commands->Get_Position(enter);	ExpLoc1.X += 0.3f;	ExpLoc1.Y += 0.3f;	ExpLoc1.Z += 0.3f;
			Vector3 ExpLoc2 = Commands->Get_Position(enter);	ExpLoc2.X += 0.4f;	ExpLoc2.Y += 0.4f;	ExpLoc2.Z += 0.4f;
			Vector3 ExpLoc3 = Commands->Get_Position(enter);	ExpLoc3.X += 0.5f;	ExpLoc3.Y += 0.5f;	ExpLoc3.Z += 0.5f;
			Commands->Create_Explosion(Explosion,ExpLoc1,0);
			Commands->Create_Explosion(Explosion,ExpLoc2,0);
			Commands->Create_Explosion(Explosion,ExpLoc3,0);
			Commands->Apply_Damage(Get_Vehicle(enter),135.0f,"Ammo_Mammoth_Shell",0);
			Commands->Create_2D_WAV_Sound("gdi_minefield_trigger.wav");
			Commands->Destroy_Object(obj);
		}
	}
	else if (team == 1)
	{
		if (Commands->Get_Player_Type(enter) == 0)
		{
			const char *Explosion = "TCW_Explosion_Shell_Mammoth";
			Vector3 ExpLoc1 = Commands->Get_Position(enter);	ExpLoc1.X += 0.3f;	ExpLoc1.Y += 0.3f;	ExpLoc1.Z += 0.3f;
			Vector3 ExpLoc2 = Commands->Get_Position(enter);	ExpLoc2.X += 0.4f;	ExpLoc2.Y += 0.4f;	ExpLoc2.Z += 0.4f;
			Vector3 ExpLoc3 = Commands->Get_Position(enter);	ExpLoc3.X += 0.5f;	ExpLoc3.Y += 0.5f;	ExpLoc3.Z += 0.5f;
			Commands->Create_Explosion(Explosion,ExpLoc1,0);
			Commands->Create_Explosion(Explosion,ExpLoc2,0);
			Commands->Create_Explosion(Explosion,ExpLoc3,0);
			Commands->Apply_Damage(Get_Vehicle(enter),135.0f,"Ammo_Mammoth_Shell",0);
			Commands->Create_2D_WAV_Sound("gdi_minefield_trigger.wav");
			Commands->Destroy_Object(obj);
		}
	}
}

ScriptRegistrant<tcw_apc_minefield> tcw_apc_minefield_Registrant("tcw_apc_minefield", "Team=0:int");
ScriptRegistrant<tcw_apc_minefield_lay> tcw_apc_minefield_lay_Registrant("tcw_apc_minefield_lay", "");
ScriptRegistrant<tcw_bunker> tcw_bunker_Registrant("tcw_bunker", "ZoneID=0:int");
ScriptRegistrant<tcw_bunker_zone> tcw_bunker_zone_Registrant("tcw_bunker_zone", "ExpLoc1=0:int,ExpLoc2=0:int,ExpLoc3=0:int");
ScriptRegistrant<tcw_hijacked_gdi> tcw_hijacked_gdi_Registrant("tcw_hijacked_gdi","");
ScriptRegistrant<tcw_hijacked_nod> tcw_hijacked_nod_Registrant("tcw_hijacked_nod","");
ScriptRegistrant<tcw_vehexit_killtimer> tcw_vehexit_killtimer_Registrant("tcw_vehexit_killtimer","");
ScriptRegistrant<tcw_vehicle> tcw_vehicle_Registrant("tcw_vehicle","");