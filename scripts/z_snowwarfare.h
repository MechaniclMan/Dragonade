/*	Renegade Scripts.dll
	Copyright 2012-2013 http://www.UltraAOW.COM by zunnie
	For the map C&C_SnowWarfare by Mauler

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#pragma once


class z_sw_randomchar : public ScriptImpClass
{
	void Created(GameObject *obj);
};

class z_sw_dom_node : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	int owner;
	bool trigger_neutral_damage;
	const char *nodename;
};