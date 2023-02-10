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

class tcw_m03_controller : public ScriptImpClass {
	void Created(GameObject *obj);
};

class tcw_m03_steal_hon_zone : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
	void Exited(GameObject *obj, GameObject *exit);
};

class tcw_m03_stealing_hon_tech : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class tcw_m03_gdi_win_zone : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
};

class tcw_m03_gdi_saboteur_death_count : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	int deaths;
	int deathsmax;
	bool trigger;
};

class tcw_m03_hon : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Killed(GameObject *obj, GameObject *shooter);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Timer_Expired(GameObject *obj, int number);
	bool allowkill;
	bool dowarning;
};

class tcw_m03_saboteur : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
};

class tcw_m03_war_factory : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class tcw_m03_nod_truck : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Killed(GameObject *obj, GameObject *shooter);
	bool triggernuke;
};

class tcw_m03_gdi_beachdrop_bot : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

class tcw_m03_gdi_beachdrop : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	int deaths;
};

class tcw_m03_nod_reinforce_zone : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj, GameObject *enter);
	void Timer_Expired(GameObject *obj, int number);
	bool triggerdrop;
};
