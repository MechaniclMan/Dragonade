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

/*********************************************************************************************/
class z_HostageFollowTest : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
	unsigned int FollowID;
	bool Following;
	bool AllowPoke;
};
/*********************************************************************************************/
class z_GotoStar_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
};
/*********************************************************************************************/
class z_Hostage2 : public ScriptImpClass {
	unsigned int FollowID;
	GameObject *Pokerguy;
	bool Following;
	bool AllowPoke;
	float HostageHealth;
	float HostageShield;
	float HealthRegen;
	int HostageID;
	int PokerTeam;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Poked(GameObject *obj,GameObject *poker);
	void Killed(GameObject *obj,GameObject *killer);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};
/*********************************************************************************************/
class z_Hostage_Rescue_Zone2 : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};
/*********************************************************************************************/
class z_Hostage_Controller2 : public ScriptImpClass {
	int allhostagedeaths;
	int sovhostagedeaths;
	int allhostagerescues;
	int sovhostagerescues;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	public: void Register_Auto_Save_Variables();
};