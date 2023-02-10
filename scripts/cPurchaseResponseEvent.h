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

#ifndef INCLUDE_CPURCHASERESPONSEEVENT
#define INCLUDE_CPURCHASERESPONSEEVENT

#include "cNetEvent.h"

class cPurchaseResponseEvent : public cNetEvent {
public:
	inline cPurchaseResponseEvent *Constructor();
	inline void Init(int Type,int ID) {
		this->ID = ID;
		this->Type = Type;
		Set_Object_Dirty_Bits(ID,BIT_CREATION);
	}

private:
	int ID;
	int Type;
};

inline _declspec(naked) cPurchaseResponseEvent *cPurchaseResponseEvent::Constructor() AT2(0x004B85F0,0x004B85F0);

#endif