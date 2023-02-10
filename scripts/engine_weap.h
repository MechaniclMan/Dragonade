/*	Renegade Scripts.dll
	Copyright 2011 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef SCRIPTS_INCLUDE__ENGINE_WEAP_H
#define SCRIPTS_INCLUDE__ENGINE_WEAP_H

class AmmoDefinitionClass;
class WeaponDefinitionClass;
class ExplosionDefinitionClass;
class BeaconGameObj;
SCRIPTS_API int Get_Current_Bullets(GameObject *obj); //Get loaded bullets for an objects current gun
SCRIPTS_API int Get_Current_Clip_Bullets(GameObject *obj); //Get clip/backpack bullets for an objects current gun
SCRIPTS_API int Get_Current_Total_Bullets(GameObject *obj); //Get total bullets for an objects current gun
SCRIPTS_API int Get_Total_Bullets(GameObject *obj,const char *weapon); //Get total bullets for a specific gun (if the object doesnt have the gun, return is zero)
SCRIPTS_API int Get_Clip_Bullets(GameObject *obj,const char *weapon); //Get clip/backpack bullets for a specific gun (if the object doesnt have the gun, return is zero)
SCRIPTS_API int Get_Bullets(GameObject *obj,const char *weapon); //Get loaded bullets for a specific gun (if the object doesnt have the gun, return is zero)
SCRIPTS_API int Get_Current_Max_Bullets(GameObject *obj); //Get max loaded bullets for an objects current gun
SCRIPTS_API int Get_Current_Clip_Max_Bullets(GameObject *obj); //Get max clip/backpack bullets for an objects current gun
SCRIPTS_API int Get_Current_Total_Max_Bullets(GameObject *obj); //Get total bullets for an objects current gun
SCRIPTS_API int Get_Max_Total_Bullets(GameObject *obj,const char *weapon); //Get max total bullets for a specific gun (if the object doesnt have the gun, return is zero)
SCRIPTS_API int Get_Max_Clip_Bullets(GameObject *obj,const char *weapon); //Get max clip/backpack bullets for a specific gun (if the object doesnt have the gun, return is zero)
SCRIPTS_API int Get_Max_Bullets(GameObject *obj,const char *weapon); //Get max loaded bullets for a specific gun (if the object doesnt have the gun, return is zero)
SCRIPTS_API int Get_Position_Total_Bullets(GameObject *obj, int position); //Get total bullets for an objects gun at a specific position
SCRIPTS_API int Get_Position_Bullets(GameObject *obj,int position); //Get loaded bullets for an objects gun at a specific position
SCRIPTS_API int Get_Position_Clip_Bullets(GameObject *obj,int position); //Get clip/backpack bullets for an objects gun at a specific position
SCRIPTS_API int Get_Position_Total_Max_Bullets(GameObject *obj, int position); //Get total bullets for an objects gun at a specific position
SCRIPTS_API int Get_Position_Max_Bullets(GameObject *obj,int position); //Get loaded bullets for an objects gun at a specific position
SCRIPTS_API int Get_Position_Clip_Max_Bullets(GameObject *obj,int position); //Get clip/backpack bullets for an objects gun at a specific position
//For all of these, -1 means "infinate ammo"
SCRIPTS_API const char *Get_Powerup_Weapon(const char *Powerup); //Get the weapon name that a powerup will grant if collected
SCRIPTS_API const AmmoDefinitionClass *Get_Weapon_Ammo_Definition(const char *weapon,bool PrimaryFire); //Get the AmmoDefinitionClass of a weapon given its preset name
SCRIPTS_API const WeaponDefinitionClass *Get_Weapon_Definition(const char *weapon); //Get the WeaponDefinitionClass of a weapon given its preset name
SCRIPTS_API const WeaponDefinitionClass *Get_Current_Weapon_Definition(GameObject *obj); //Get the WeaponDefinitionClass of an objects current weapon
SCRIPTS_API const WeaponDefinitionClass *Get_Position_Weapon_Definition(GameObject *obj,int position); //Get the WeaponDefinitionClass of an objects weapon at the specified position
SCRIPTS_API const ExplosionDefinitionClass *Get_Explosion(const char *explosion); //Get the ExplosionDefinitionClass of an explosion given its preset name
SCRIPTS_API const char *Get_Powerup_Weapon_By_Obj(GameObject *Powerup); //Get the name of a powerup weapon given a PowerUpGameObj
SCRIPTS_API int Get_Current_Weapon_Style(GameObject *obj); //Get weapon style for an objects current gun
SCRIPTS_API int Get_Position_Weapon_Style(GameObject *obj,int position); //Get weapon style for an objects gun at a specific position
SCRIPTS_API int Get_Weapon_Style(GameObject *obj,const char *weapon); //Get weapon style for a specific gun (if the object doesnt have the gun, return is zero)
SCRIPTS_API void Disarm_Beacon(GameObject *obj); //Disarm a beacon
SCRIPTS_API void Disarm_Beacons(int PlayerID); //Disarm all beacons for a player
SCRIPTS_API void Disarm_C4(GameObject *obj); //Disarm a C4 object
SCRIPTS_API void Disarm_All_C4_Beacons(int PlayerID); //Disarm all C4 and beacons owned by a player
SCRIPTS_API const char *Get_Current_Weapon(GameObject *obj); //gets the current weapon of an object
SCRIPTS_API int Get_Weapon_Count(GameObject *obj); //gets the weapon count for an object
SCRIPTS_API const char *Get_Weapon(GameObject *obj,int position); //get the specified position in the weapon bag
SCRIPTS_API bool Has_Weapon(GameObject *obj,const char *weapon); //does the object have the weapon
SCRIPTS_API GameObject *Find_Beacon(int Number,int Team); //find a beacon

enum C4Type
{
	C4_REMOTE = 1,
	C4_TIMED = 2,
	C4_PROXY = 3
};

SCRIPTS_API int Get_C4_Count(int team, int type = 0); // get C4 count by type
SCRIPTS_API void Defuse_Owned_C4(int player_id, int type = 0); // defuse all C4 owned by a player of a certain type (or 0 for all types)
SCRIPTS_API void Defuse_Attached_C4(GameObject* attached_object); // defuse all C4 attached to an object or (use nullptr for all unattached C4)


SCRIPTS_API int Get_Beacon_Count(int Team); //get the beacon count
SCRIPTS_API int Get_C4_Mode(GameObject *obj); //get the C4 mode for a C4GameObj
SCRIPTS_API GameObject *Get_C4_Planter(GameObject *obj); //Gets the planter of a C4GameObj
SCRIPTS_API GameObject *Get_C4_Attached(GameObject *obj); //Gets the object a C4GameObj is attached to (if any)
SCRIPTS_API GameObject *Get_Beacon_Planter(GameObject *obj); //Gets the planter of a BeaconGameObj
SCRIPTS_API BeaconGameObj* Find_Nearest_Beacon(const Vector3& position, int team); //Find the nearest beacon to a position
#endif
