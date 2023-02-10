/*	Renegade Scripts.dll
	Copyright 2013 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_CGAMEOPTIONSEVENT
#define INCLUDE_CGAMEOPTIONSEVENT

#include "cNetEvent.h"
#include "GameData.h"
#include "engine_tt.h"

class cGameOptionsEvent : public cNetEvent {
public:
	inline cGameOptionsEvent *Constructor();
	inline void Init() {
		HostedGameNumber = cGameData::HostedGameNumber;
		Set_Object_Dirty_Bit(BIT_CREATION,true);
		Update_Network_Object(this);
	}
	inline void Init(int ID) {
		HostedGameNumber = cGameData::HostedGameNumber;
		Set_Object_Dirty_Bit(ID,BIT_CREATION,true);
		Update_Network_Object(this);
	}

private:
	int HostedGameNumber;
};

inline _declspec(naked) cGameOptionsEvent *cGameOptionsEvent::Constructor() AT2(0x004B68A0,0x004B68A0);

#endif