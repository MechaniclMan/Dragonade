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

class tcw_i_have_ion_beacon_incursion : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

class tcw_pt_ion_incursion : public ScriptImpClass {
void Created(GameObject *obj);
void Timer_Expired(GameObject *obj, int number);
void Poked(GameObject *obj, GameObject *poker);
void Custom(GameObject *obj, int message, int param, GameObject *sender);
bool ionready;
bool allowpoke;
};

class tcw_gdi_ion_cannon_building_incursion : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Killed(GameObject *obj, GameObject *shooter);
	bool killable;
};

class tcw_inc_steal_tech_zone_gbar : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
	void Exited(GameObject *obj, GameObject *exit);
};

class tcw_inc_i_am_stealing_tech : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class tcw_inc_barracks : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Killed(GameObject *obj, GameObject *shooter);
	bool AllowKill;
};
class tcw_inc_war_factory : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Killed(GameObject *obj, GameObject *shooter);
	bool AllowKill;
};
class tcw_inc_vehicle_pos : public ScriptImpClass {
	void Created(GameObject *obj);
};
class tcw_inc_vehicle_pos2 : public ScriptImpClass {
	void Created(GameObject *obj);
};
class tcw_inc_vehicle_pos3 : public ScriptImpClass {
	void Created(GameObject *obj);
};
class tcw_inc_vehicle_pos4 : public ScriptImpClass {
	void Created(GameObject *obj);
};
class tcw_inc_steal_tech_zone_gwf : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
	void Exited(GameObject *obj, GameObject *exit);
};
class tcw_inc_veh_drop1 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};
class tcw_inc_veh_drop2 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};
class tcw_inc_veh_drop3 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};
class tcw_inc_veh_drop4 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};
class tcw_inc_bonus_complete : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};
class tcw_inc_bonus_failed : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};