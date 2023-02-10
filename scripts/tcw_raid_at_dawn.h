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

class tcw_raid_steal_tech_zone_nwf : public ScriptImpClass {
	void Entered(GameObject *obj, GameObject *enter);
	void Exited(GameObject *obj, GameObject *exit);
};

class tcw_raid_i_am_stealing_tech : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class tcw_raid_missionfail : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *shooter);
};
