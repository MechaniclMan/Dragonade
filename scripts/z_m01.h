/*	Renegade Scripts.dll
	Copyright 2011 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#pragma once

class z_m01_missionstart : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};
class z_m01_captduncan : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	bool allowpoke;
};
class z_m01_obj1_announce : public ScriptImpClass {
	void Created(GameObject *obj);//ok we use this in our script
	void Timer_Expired(GameObject *obj, int number);//this too
	void Entered(GameObject *obj, GameObject *enter);
	bool domsg;//and our boolean....
};
class z_m01_accessdeniedzone : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Entered(GameObject *obj, GameObject *enter);
	bool domsg;
};
class z_m01_commpc : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);
	bool allowpoke;
	GameObject *pokerguy;
};
class z_m01_commcontrol : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	bool allowcommdeath;
};
class z_m01_missionend : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};
class z_m01_civbarn_rec : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	int civ1;
	int civ2;
	int civ3;
	int civ4;
	int civ5;
	int res1;
	int res2;
	int res3;
	int res4;
	int res5;
	int civdeaths;
	int maxdeaths;
	int civres;
	int reqres;
};
class z_m01_civilian : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Killed(GameObject *obj, GameObject *shooter);
	void Timer_Expired(GameObject *obj, int number);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	int cmsg;
	int kmsg;
	GameObject *pokerguy;
	int cid;
	int id;
	bool sendmsg;
	int FollowID;
	bool allowpoke;
};
class z_m01_civreszonebarn : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj, GameObject *enter);
	void Timer_Expired(GameObject *obj, int number);
};
class z_m01_church_evac_control : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	int civ1;
	int civ2;
	bool res1;
	bool res2;
	int civdeaths;
	int civres;
	int reqres;
	int maxdeaths;
};
class z_m01_civilian_church : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Killed(GameObject *obj, GameObject *shooter);
	bool allowpoke;
	bool sendmsg;
	int cmsg;
	int cid;
	int kmsg;
};
class z_m01_checkpoint_1 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	bool locked;
	bool allowpoke;
	int gotoid1;
	int gotoid2;
	int gotoid3;
	int gotoid4;
	int gotoid5;
};
class z_m01_checkpoint_2 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	bool locked;
	bool allowpoke;
	int gotoid1;
	int gotoid2;
	int gotoid3;
	int gotoid4;
	int gotoid5;
};
class z_m01_disable_honzone : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};
class z_m01_disable_commzone : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};