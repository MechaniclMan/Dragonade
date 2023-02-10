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
#pragma once

class tcw_Grant_Powerup_Created : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Killed(GameObject *obj, GameObject *shooter);
	public: void Register_Auto_Save_Variables();
	bool isInitialized;
	void Timer_Expired(GameObject *obj, int number);
	int kills;
	bool revealed;
	bool playdetected;
	bool upgrade_composit_armor_available;
	bool upgrade_powerpacks_available;
	bool map_low_grav;
	bool map_dm_tech1;
	bool map_dm_tech2;
	bool map_dm_tech3;
	bool map_coop_gdi;
	bool map_coop_nod;
	bool map_sniper_only;
	Vector3 commandocurrpos;
	Vector3 commandonewpos;
};

class tcw_fanatic : public JFW_Key_Hook_Base
{
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void KeyHook();
	float DetonateTime;
	float RunSpeed;
	const char *Explosion;
};