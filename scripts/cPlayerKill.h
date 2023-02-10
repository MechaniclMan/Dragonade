/*	Renegade Scripts.dll
	Copyright 2015 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_CPLAYERKILL
#define INCLUDE_CPLAYERKILL

#include "cNetEvent.h"

class cPlayerKill : public cNetEvent {
public:
	inline cPlayerKill *Constructor();
	inline void Init(int Killer,int Victim) {
		this->Killer = Killer;
		this->Victim = Victim;
		Set_Object_Dirty_Bit(BIT_CREATION,true);
	}
	
private:
	int Killer;
	int Victim;
};

inline _declspec(naked) cPlayerKill *cPlayerKill::Constructor() AT2(0x004B7CD0,0x004B7CD0);

#endif