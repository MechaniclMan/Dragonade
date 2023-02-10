/*	Renegade Scripts.dll
    Dragonade Game Manager
	Copyright 2012 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAGAME
#define INCLUDE_DAGAME

#include "da_event.h"
#include "da_gamemode.h"
#include "da_gamefeature.h"

class DA_API DAGameManager : private DAEventClass {
public:
	static void Init();
	static void Shutdown();
	static inline DAGameModeFactoryClass *Get_Game_Mode() {
		return GameMode;
	}
	static inline const char *Get_Game_Mode_Long_Name() {
		if (GameMode) {
			return GameMode->Get_Long_Name();
		}
		return "Unknown";
	}
	static inline const char *Get_Game_Mode_Short_Name() {
		if (GameMode) {
			return GameMode->Get_Short_Name();
		}
		return "Unknown";
	}
	static inline bool Is_Game_Mode(const char *Name) {
		return (!_stricmp(Get_Game_Mode_Long_Name(),Name) || !_stricmp(Get_Game_Mode_Short_Name(),Name));
	}
	static void Add_Game_Mode(DAGameModeFactoryClass *Factory);
	static void Add_Game_Feature(DAGameFeatureFactoryClass* Factory);
	static inline const DynamicVectorClass<DAGameFeatureFactoryClass*> &Get_Game_Features() {
		return GameFeatures;
	}
	static inline bool Is_Shutdown_Pending() {
		return ShutdownPending;
	}
	static inline const StringClass &Get_Map() {
		return Map;
	}

private:
	virtual void Game_Over_Event();
	virtual void Level_Loaded_Event();
	virtual void Settings_Loaded_Event();
	static DAGameModeFactoryClass *Find_Game_Mode(const char *Name);
	static DAGameModeFactoryClass *GameMode;
	static DynamicVectorClass<DAGameModeFactoryClass*> GameModes;
	static bool ShowGameModeTitle;
	static DynamicVectorClass<DAGameFeatureFactoryClass*> GameFeatures;
	static bool FirstMap;
	static bool ShutdownPending;
	static StringClass Map;
};


#endif