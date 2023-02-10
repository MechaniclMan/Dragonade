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

class tcw_i_am_a_bot : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
};

class tcw_attach_script_simple_timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class tcw_enable_spawners_killed : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};

class tcw_midlight_eva1 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class tcw_goto_player_attack : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class tcw_goto_player : public ScriptImpClass
{
        void Created(GameObject *obj);
        void Timer_Expired(GameObject *obj, int number);
};