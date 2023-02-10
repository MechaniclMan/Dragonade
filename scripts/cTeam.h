/*	Renegade Scripts.dll
	Copyright 2012 Whitedragon, Tiberian Technologies

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
	inline void Set_Kills(int kills) {
		Kills = kills;
		Set_Object_Dirty_Bit(BIT_RARE,true);
	}
	inline void Inc_Kills() {
		Kills++;
		Set_Object_Dirty_Bit(BIT_RARE,true);
	}
	inline void Dec_Kills() {
		Kills--;
		Set_Object_Dirty_Bit(BIT_RARE,true);
	}
	inline int Get_Kills() {
		return Kills;
	}
	inline void Set_Deaths(int deaths) {
		Deaths = deaths;
		Set_Object_Dirty_Bit(BIT_RARE,true);
	}
	inline void Inc_Deaths() {
		Deaths++;
		Set_Object_Dirty_Bit(BIT_RARE,true);
	}
	inline void Dec_Deaths() {
		Deaths--;
		Set_Object_Dirty_Bit(BIT_RARE,true);
	}
	inline int Get_Deaths() {
		return Deaths;
	}
	inline void Set_Score(float score) {
		Score = score;
		Set_Object_Dirty_Bit(BIT_OCCASIONAL,true);
	}
	inline float Get_Score() {
		return Score;
	}
	inline float Get_Kill_To_Death_Ratio() {
		if (!Deaths) {
			return 0.0f;
		}
		return (float)Kills/(float)Deaths;
	}
	int Tally_Size();
	void Increment_Kills();
	void Increment_Deaths();
	void Increment_Score(float amount);
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