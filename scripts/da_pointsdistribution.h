/*	Renegade Scripts.dll
    Dragonade Points Distribution Game Feature
	Copyright 2012 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAPOINTSDISTRIBUTION
#define INCLUDE_DAPOINTSDISTRIBUTION

#include "da_event.h"
#include "da_gamefeature.h"

class DAPointsDistributionGameFeatureClass : public DAEventClass, public DAGameFeatureClass {
	virtual void Init();
	virtual void Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone);
};

#endif