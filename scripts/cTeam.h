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
//Changes made in DA:
//Added Inc and Dec functions.
//Fixed wrong dirty bit.
#ifndef INCLUDE_CTEAM
#define INCLUDE_CTEAM

#include "NetworkObjectClass.h"
#include "engine_string.h"

#define MAX_TEAMS 2

class cTeam : public NetworkObjectClass {
private:
	WideStringClass Name; // 06B4
	int Kills; // 06B8
	int Deaths; // 06BC
	float Score; // 06C0
	int unk06C4; // 06C4; Not sure about type, but this variable never seems to be used.
	int ID; // 06C8

public:
	cTeam();
	~cTeam();
	void Init(int _id);
	void Init_Team_Name();
	void Reset();
	SCRIPTS_API void Set_Kills(int kills);
	SCRIPTS_API void Increment_Kills();
	SCRIPTS_API void Decrement_Kills();
	SCRIPTS_API int Get_Kills();
	SCRIPTS_API void Set_Deaths(int deaths);
	SCRIPTS_API void Increment_Deaths();
	SCRIPTS_API void Decrement_Deaths();
	SCRIPTS_API int Get_Deaths();
	SCRIPTS_API void Set_Score(float score);
	SCRIPTS_API void Increment_Score(float score);
	SCRIPTS_API float Get_Score();
	SCRIPTS_API float Get_Kill_To_Death_Ratio();
	int Tally_Size();
	void Get_Team_String(int, WideStringClass&);
	Vector3 Get_Color();
	int Tally_Money();
	void Export_Creation(BitStreamClass& bitStream);
	void Import_Creation(BitStreamClass& bitStream);
	void Export_Rare(BitStreamClass& bitStream);
	void Import_Rare(BitStreamClass& bitStream);
	void Export_Occasional(BitStreamClass& bitStream);
	void Import_Occasional(BitStreamClass& bitStream);

	unsigned int Get_Network_Class_ID() const;
	void Delete() { delete this; }
	WideStringClass Get_Name() const { return Name; }
	int Get_Id() const { return ID; }

}; // 06CC

#endif