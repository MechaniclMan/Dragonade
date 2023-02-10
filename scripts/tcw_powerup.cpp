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
#include "tcw_powerup.h"

void tcw_POW_PickupSounds::Custom(GameObject *obj, int message, int param, GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_RemoteC4"))
		{
			Send_Message_Player(sender,104,234,40,"Remote C4 acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pac4_aqob0004i1evag_snd.wav");
		}
		else if ( (strstr(Commands->Get_Preset_Name(obj),"POW_Chaingun_Player")) || (strstr(Commands->Get_Preset_Name(obj),"POW_Chaingun_Player_Nod")) )
		{
			Send_Message_Player(sender,104,234,40,"Chaingun ammunition acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pacg_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Mines"))
		{
			Send_Message_Player(sender,104,234,40,"Proximity C4 acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pacp_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_Key_Green"))
		{
			char pickupmsg[250];
			const char *str = Get_Player_Name(sender);
			sprintf(pickupmsg,"%s acquired a Green Key Card.",str);
			delete[] str;
			Send_Message(104,234,40,pickupmsg);
			Commands->Create_2D_WAV_Sound("greenkeyacquired.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_Key_Yellow"))
		{
			char pickupmsg[250];
			const char *str = Get_Player_Name(sender);
			sprintf(pickupmsg,"%s acquired a Yellow Security Card.",str);
			delete[] str;
			Send_Message(104,234,40,pickupmsg);
			Commands->Create_2D_WAV_Sound("yellowkeyacquired.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_Key_Red"))
		{
			char pickupmsg[250];
			const char *str = Get_Player_Name(sender);
			sprintf(pickupmsg,"%s acquired a Red Key Card.",str);
			delete[] str;
			Send_Message(104,234,40,pickupmsg);
			Commands->Create_2D_WAV_Sound("redkeyacquired.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Chemsprayer"))
		{
			Send_Message_Player(sender,104,234,40,"Chemsprayer fluid acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pacs_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_TimedC4_Commando"))
		{
			Send_Message_Player(sender,104,234,40,"Timed C4 acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pact_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Flamethrower"))
		{
			Send_Message_Player(sender,104,234,40,"Flamethrower fuel acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00paft_aqob0001i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Grenades"))
		{
			Send_Message_Player(sender,104,234,40,"Grenades acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pagn_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Beacon_IonCannon_10Sec"))
		{
			Send_Message_Player(sender,104,234,40,"10 Second Superweapon Beacon acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00paib_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Beacon_Nuke_10Sec"))
		{
			Send_Message_Player(sender,104,234,40,"Nuclear Weapon Available.");
			Create_2D_WAV_Sound_Player(sender,"nukeavail.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Beacon_Nuke_60Sec"))
		{
			Send_Message_Player(sender,104,234,40,"Nuclear Weapon Available.");
			Create_2D_WAV_Sound_Player(sender,"nukeavail.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Beacon_IonCannon_60Sec"))
		{
			Send_Message_Player(sender,104,234,40,"Superweapon Beacon acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00paib_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"POW_LaserChaingun_Player"))//upgraded laser weapon?
		{
			Send_Message_Player(sender,104,234,40,"Laser Chaingun battery acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00palc_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Laser_Shadow"))
		{
			Send_Message_Player(sender,104,234,40,"Laser Rifle battery acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00palr_aqob0004i1evag_snd.wav");
		}
		else if ((strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Missiles")) || 
				 (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Rockets")) )
		{
			Send_Message_Player(sender,104,234,40,"Rockets acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00park_aqob0004i1evag_snd.wav");
		}
		else if ((strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Repairgun_Advanced")) || (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Repairgun_Building")))
		{
			Send_Message_Player(sender,104,234,40,"Maintenance Tool Battery acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00parp_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Sniper"))
		{
			Send_Message_Player(sender,104,234,40,"Sniper Rifle ammunition acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pasr_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Shotgun"))
		{
			Send_Message_Player(sender,104,234,40,"Shotgun ammunition acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00pass_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Armor_100"))
		{
			Send_Message_Player(sender,104,234,40,"Full Body Armor acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00prba_aqob0004i1evag_snd.wav");
		}
		else if (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Armor_050"))
		{
			Send_Message_Player(sender,104,234,40,"Breast Plate Armor acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00prbp_aqob0004i1evag_snd.wav");
		}
		else if ( (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Health_025")) || (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Health_050")) || (strstr(Commands->Get_Preset_Name(obj),"TCW_POW_Health_100")) )
		{
			Send_Message_Player(sender,104,234,40,"Health Supplement acquired.");
			Create_2D_WAV_Sound_Player(sender,"m00ph25_aqob0004i1evag_snd.wav");
		}
	}
}


ScriptRegistrant<tcw_POW_PickupSounds> tcw_POW_PickupSounds_Registrant("tcw_POW_PickupSounds","");