/*	Renegade Scripts.dll
	Copyright 2012-2013 zunnie
	Websites: http://www.MultiPlayerForums.com / http://www.UltraAOW.com
	ModDB Profile: http://www.moddb.com/mods/renegade-domination

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

/*
	This file contains dummy scripts which are required for the MPF Domination Plugin.
	They can be used in the Level Editor to setup new maps while using the MPF Domination Plugin as a base on the server.
*/

#pragma once

class DOM_Control_Point : public ScriptImpClass {
	void Created(GameObject *obj);
}; 

class DOM_Random_Spawn_Character : public ScriptImpClass {
	void Created(GameObject *obj);
}; 
