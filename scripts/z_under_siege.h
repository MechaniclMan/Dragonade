/*	Renegade Scripts.dll
	Copyright 2012-2013 http://www.UltraAOW.COM by zunnie
	For the map C&C_Under_Siege

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#pragma once

class z_us_building : public ScriptImpClass {
	void Damaged(GameObject *obj, GameObject *damager, float damage);
};

class z_us_core_nod : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Timer_Expired(GameObject *obj, int number);
	GameObject *center_node;
	bool play_heal_core;
	bool play_under_attack;
	bool play_destroyed;
	bool play_heavily_damaged;
	bool play_critical_damaged;
};

class z_us_core_gdi : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Timer_Expired(GameObject *obj, int number);
	GameObject *center_node;
	bool play_heal_core;
	bool play_under_attack;
	bool play_destroyed;
	bool play_heavily_damaged;
	bool play_critical_damaged;
};

class z_us_center_node : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	bool play_defend_node;
};

class z_us_orb_carrier_nod : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Destroyed(GameObject *obj);
	void Detach(GameObject *obj);
	int orb_id;
};

class z_us_orb_carrier_gdi : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Destroyed(GameObject *obj);
	void Detach(GameObject *obj);
	int orb_id;
};

class z_us_get_orb : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	int team;
	bool allowpoke;
};

class z_us_enemy_orb_incoming_zone : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj, GameObject *enter);
	void Timer_Expired(GameObject *obj, int number);
	int team;
	GameObject *center_node;
	bool trigger;
};

class z_us_limited_character : public ScriptImpClass {
	void Created(GameObject *obj);
};

class z_us_destroy_orb : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};
