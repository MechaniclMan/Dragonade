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

class tcw_i_have_ion_beacon : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

class tcw_pt_ion : public ScriptImpClass {
void Created(GameObject *obj);
void Timer_Expired(GameObject *obj, int number);
void Poked(GameObject *obj, GameObject *poker);
void Custom(GameObject *obj, int message, int param, GameObject *sender);
public: void Register_Auto_Save_Variables();
bool ionready;
bool allowpoke;
};

class tcw_gdi_ion_cannon_building : public ScriptImpClass {
void Custom(GameObject *obj, int message, int param, GameObject *sender);
void Killed(GameObject *obj, GameObject *shooter);
};


class tcw_i_have_nuke_beacon : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

class tcw_pt_nuke : public ScriptImpClass {
void Created(GameObject *obj);
void Timer_Expired(GameObject *obj, int number);
void Poked(GameObject *obj, GameObject *poker);
void Custom(GameObject *obj, int message, int param, GameObject *sender);
public: void Register_Auto_Save_Variables();
bool ionready;
bool allowpoke;
};

class tcw_nod_nuke_building : public ScriptImpClass {
void Created(GameObject *obj);
void Custom(GameObject *obj, int message, int param, GameObject *sender);
void Killed(GameObject *obj, GameObject *shooter);
void Timer_Expired(GameObject *obj, int number);
public: void Register_Auto_Save_Variables();
int launchready;
int launchnuke;
};

