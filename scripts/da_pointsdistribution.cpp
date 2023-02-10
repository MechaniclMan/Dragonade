/*	Renegade Scripts.dll
    Dragonade Points Distribution Game Feature
	Copyright 2013 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "engine_da.h"
#include "da.h"
#include "da_pointsdistribution.h"
#include "da_damagelog.h"
#include "da_vehicle.h"

void DAPointsDistributionGameFeatureClass::Init() {
	Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::ALL);
}

void DAPointsDistributionGameFeatureClass::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	if (!Is_Beacon(GetExplosionObj())) { //Give all points to the killer if killed by a beacon.
		float Points = Get_Death_Points(Victim);
		if (Points) {
			int Team = Victim->Get_Player_Type();
			if (Is_Player(Killer)) {
				if (Victim->As_VehicleGameObj()) {
					Team = DAVehicleManager::Get_Team(Victim);
					if ((Victim->Get_Player_Type() != -2 && Victim->Get_Player_Type() != Killer->Get_Player_Type()) || (((VehicleGameObj*)Victim)->Get_Last_Team() != -2 && ((VehicleGameObj*)Victim)->Get_Last_Team() != Killer->Get_Player_Type())) {
						((SoldierGameObj*)Killer)->Get_Player()->Increment_Score(Points*-1.0f); //Remove points the killer got for getting the last shot.
						((SoldierGameObj*)Killer)->Get_Player()->Increment_Money(Points*-1.0f);
					}
				}
				else if (Victim->Get_Player_Type() != Killer->Get_Player_Type()) {
					((SoldierGameObj*)Killer)->Get_Player()->Increment_Score(Points*-1.0f);
					((SoldierGameObj*)Killer)->Get_Player()->Increment_Money(Points*-1.0f);
				}
			}
			DynamicVectorClass<DADamageTableStruct> Damagers;
			DADamageLog::Get_Damagers_By_Percent_Other_Team(Damagers,Victim,Team,0.0f,0.0f);
			for (int i = 0;i < Damagers.Count();i++) {
				Damagers[i].Player->Increment_Score(Damagers[i].Damage*Points); //Distribute points to all damagers.
			}
		}
	}
}

Register_Game_Feature(DAPointsDistributionGameFeatureClass,"Points Distribution","EnablePointsDistribution",0);