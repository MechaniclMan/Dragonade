/*	Renegade Scripts.dll
    Dragonade GameSpy Q&R
	Copyright 2012 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAGAMESPY
#define INCLUDE_DAGAMESPY

#include "da_event.h"
#include "da_gamefeature.h"

class DAGameSpyGameFeatureClass : public DAEventClass, public DAGameFeatureClass {
public:
	virtual void Init();
	virtual void Settings_Loaded_Event();
	virtual void Level_Loaded_Event();
	virtual void Think();
	virtual void Timer_Expired(int Number,unsigned int Data);
	virtual void Player_Join_Event(cPlayer *Player);
	virtual void Player_Leave_Event(cPlayer *Player);
	virtual ~DAGameSpyGameFeatureClass();

private:
	SOCKET ListenSocket;
	sockaddr_in ClientAddress, ServerAddress, MasterAddress1, MasterAddress2;
	int ClientAddressSize;
	unsigned int QueryID;
	WideStringClass Title;
	
	//Settings
	unsigned short Port;
	bool ShowPlayerCount;
	bool ShowGameFeatures;
	bool ShowTeamInfo;
	DynamicVectorClass<StringClass> CustomRules;
};

#endif