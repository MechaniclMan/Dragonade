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

class z_Custom_Restore_Building : public ScriptImpClass {
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
};

class z_Poke_Send_Custom : public ScriptImpClass {
	void Poked(GameObject *obj, GameObject *poker);
};

class z_APB_Building_Allied_Barracks_Buy : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj, int number);

	bool allowpoke;
	float buildingcost;
	float currentamount;
	float pokecost;
};

class z_Poke_Restore_Building : public ScriptImpClass {
	void Poked(GameObject *obj, GameObject *poker);
};
