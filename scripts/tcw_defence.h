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

class tcw_Base_Defense : public ScriptImpClass {
private:
	unsigned long targetID;
	bool attacking;
	int current_priority;
	float attack_timer;
	int loop_count;
	void Attack(GameObject *obj, GameObject *target); 
	GameObject* Select_Target(GameObject *obj, GameObject *target);
	GameObject* Set_Target(GameObject *target);
	GameObject* Tie_Breaker(GameObject *obj, GameObject *new_target, GameObject *old_target);
	bool Adjust_For_Infantry(GameObject *target);
public:
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj, GameObject *enemy);
	void Damaged(GameObject *obj, GameObject *damager, float amount);
	void Timer_Expired(GameObject *obj, int number);
	void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason);
	void Killed(GameObject *obj, GameObject *shooter);
	public: void Register_Auto_Save_Variables();
};