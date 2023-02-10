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

class tcw_m01_gdi_beachdrops : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	int deaths;
};

class tcw_m01_gdi_beachdrops_predators : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	int deaths;
};

class tcw_m01_gdi_beachdrop_bot : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

class tcw_m01_engi_hostage : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Destroyed(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Killed(GameObject *obj, GameObject *shooter);
	int FollowID;
	int Prisoner;
	bool PlayPain;
	bool PlayDead;
};

class tcw_m01_engi_hostage_rescuezone : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class tcw_m01_engi_hostage_control : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	int PrisonerWave;
	int PrisonerRescuesRequired;
	int PrisonerRescues;
	int PrisonerDeathsMax;
	int PrisonerDeathsTotal;
	int PrisonerDeaths;
	bool Prisoner1Alive;
	bool Prisoner2Alive;
	bool Prisoner3Alive;
	bool Prisoner1Rescued;
	bool Prisoner2Rescued;
	bool Prisoner3Rescued;
	bool TriggerChinook;
};
