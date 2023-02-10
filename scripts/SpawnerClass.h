/*	Renegade Scripts.dll
    Dragonade Engine Functions
	Copyright 2015 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_SPAWNERCLASS
#define INCLUDE_SPAWNERCLASS

#include "Definition.h"
#include "Persist.h"
#include "ScriptableGameObj.h"
#include "Matrix3D.h"
#include "engine_vector.h"

class SpawnerDefClass : public DefinitionClass {
public:
	DynamicVectorClass<int> SpawnDefinitionIDList;
	int PlayerType;
	int SpawnMax;
	float SpawnDelay;
	float SpawnDelayVariation;
	bool IsPrimary;
	bool IsSoldierStartup;
	bool GotoSpawnerPos;
	float GotoSpawnerPosPriority;
	bool TeleportFirstSpawn;
	int SpecEffectsObjID;
	float PostVisualSpawnDelay;
	bool StartsDisabled;
	bool KillHibernatingSpawn;
	bool ApplySpawnMaterialEffect;
	bool IsMultiplayWeaponSpawner;
	DynamicVectorClass<StringClass> ScriptNameList;
	DynamicVectorClass<StringClass> ScriptParameterList;
};
class SpawnerClass : public PersistClass {
public:
	int ID;
	Matrix3D Transform;
	Matrix3D SpawnEffectTransform;
	SpawnerDefClass *Definition;
	ReferencerClass CurrentObject;
	bool Enable;
	int SpawnCount;
	float SpawnDelay;
	DynamicVectorClass<Matrix3D> AlternateTransforms;
	DynamicVectorClass<StringClass> ScriptNameList;
	DynamicVectorClass<StringClass> ScriptParameterList;
};
extern DynamicVectorClass<SpawnerClass*> &SpawnerList;

#endif