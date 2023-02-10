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

class tcw_Credit_Trickle : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class tcw_Custom_Win_Game_Credits : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};

class tcw_Map_Intro : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class tcw_There_Must_Be_Engineer : public ScriptImpClass {
	void Created(GameObject *obj);
}; 

class tcw_Spikemap_Get_CaptureGuy : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	bool allowpoke;
	int type;
	int target;
	int targetlow;
}; 

class tcw_Death_Blowup_Building : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
}; 

class tcw_zone_teleport_random_id : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj, GameObject *enter);
	void Timer_Expired(GameObject *obj, int number);
	public: void Register_Auto_Save_Variables();
	int targetid1;
	int targetid2;
	int targetid3;
	bool tele;
}; 

class tcw_dm_map_tech3 : public ScriptImpClass {
	void Created(GameObject *obj);
}; 
class tcw_dm_map_tech2 : public ScriptImpClass {
	void Created(GameObject *obj);
}; 
class tcw_dm_map_tech1 : public ScriptImpClass {
	void Created(GameObject *obj);
}; 

class tcw_destroyed_send_custom : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

class tcw_Spawner_Enable_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class tcw_snipe_only_building : public ScriptImpClass {
	void Created(GameObject *obj);
};





