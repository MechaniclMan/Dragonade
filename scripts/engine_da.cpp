/*	Renegade Scripts.dll
    Dragonade Engine Functions
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
#include "engine_DA.h"
#include "da.h"
#include "da_hook.h"
#include "da_player.h"
#include "da_translation.h"
#include "TeamPurchaseSettingsDefClass.h"
#include "PurchaseSettingsDefClass.h"
#include "WeaponBagClass.h"
#include "WeaponClass.h"
#include "datasafe.h"
#include "GameObjManager.h"
#include "VehicleFactoryGameObj.h"
#include "VehicleFactoryGameObjDef.h"
#include "VehicleGameObj.h"
#include "VehicleGameObjDef.h"
#include "SoldierGameObj.h"
#include "SoldierGameObjDef.h"
#include "RefineryGameObj.h"
#include "RefineryGameObjDef.h"
#include "CollisionMath.h"
#include "PowerupGameObj.h"
#include "PowerupGameObjDef.h"
#include "DefinitionMgrClass.h"
#include "TranslateDBClass.h"
#include "TDBObjClass.h"
#include "SmartGameObj.h"
#include "SmartGameObjDef.h"
#include "ScriptZoneGameObj.h"
#include "ScriptZoneGameObjDef.h"
#include "MoveablePhysClass.h"
#include "C4GameObj.h"
#include "C4GameObjDef.h"
#include "weaponmgr.h"
#include "PhysicsSceneClass.h"
#include "SoldierFactoryGameObj.h"
#include "SoldierFactoryGameObjDef.h"
#include "RawFileClass.h"
#include "BeaconGameObj.h"
#include "BeaconGameObjDef.h"
#include "OffenseObjectClass.h"
#include "TransitionInstanceClass.h"
#include "cScTextObj.h"
#include "SCAnnouncement.h"
#include "SpawnerClass.h"
#include "cGameOptionsEvent.h"
#include "cPurchaseResponseEvent.h"
#include "CombatManager.h"

RENEGADE_FUNCTION
uint Send_Object_Update(NetworkObjectClass *object, int remoteHostId)
AT2(0x00461820,0x004612F0);

RENEGADE_FUNCTION
void VehicleGameObj::Add_Occupant(SoldierGameObj *occupant,int seat_id)
AT2(0x0067AB70,0x0067AB70);
RENEGADE_FUNCTION
void VehicleGameObj::Remove_Occupant(SoldierGameObj *occupant)
AT2(0x0067AD00,0x0067AD00);
RENEGADE_FUNCTION
void VehicleGameObj::Create_And_Destroy_Transitions()
AT2(0x00678710,0x00678710);

RENEGADE_FUNCTION
void TransitionInstanceClass::Start(SoldierGameObj *Soldier)
AT2(0x006D4570,0x006D4570);

REF_DEF2(SpawnerList, DynamicVectorClass<SpawnerClass*>, 0x008564A8, 0x008564A8);

RENEGADE_FUNCTION
WeaponClass *WeaponBagClass::Add_Weapon(const WeaponDefinitionClass *, int, bool) 
AT2(0x006F3000,0x006F3000);
RENEGADE_FUNCTION
WeaponClass *WeaponBagClass::Add_Weapon(const char *, int, bool) 
AT2(0x006F31C0,0x006F31C0);
RENEGADE_FUNCTION
void WeaponBagClass::Remove_Weapon(int) 
AT2(0x006F2EF0,0x006F2EF0);
RENEGADE_FUNCTION
void WeaponBagClass::Clear_Weapons() 
AT2(0x006F2F50,0x6F2F50);
RENEGADE_FUNCTION
void WeaponBagClass::Select_Next() 
AT2(0x006F3260,0x006F3260);
RENEGADE_FUNCTION
void WeaponBagClass::Select_Weapon_Name(const char *)
AT2(0x006F3520,0x006F3520);

RENEGADE_FUNCTION
void PhysClass::Set_Model_By_Name(const char *) 
AT2(0x00640350,0x00640350);
RENEGADE_FUNCTION
void PhysClass::Set_Facing(float) 
AT2(0x00640E80,0x00640E80);
RENEGADE_FUNCTION
void PhysicalGameObj::Attach_To_Object_Bone(PhysicalGameObj *,const char *)
AT2(0x006A5270,0x006A5270);

RENEGADE_FUNCTION
void BuildingGameObj::Find_Closest_Poly(const Vector3&,float*) 
AT2(0x00684ED0,0x00684ED0)

REF_DEF2(CombatManager::FriendlyFirePermitted,bool,0x008550E4,0x008550E4);
REF_DEF2(CombatManager::BeaconPlacementEndsGame,bool,0x008550E5,0x008550E5);
REF_DEF2(BuildingGameObj::CanRepairBuildings,bool,0x00810474,0x00810474);

void Destroy_All_Objects_With_Script(const char *Script) {
	for (SLNode<BaseGameObj> *z = GameObjManager::GameObjList.Head();z;z = z->Next()) {
		if (z->Data()->As_ScriptableGameObj()) {
			if (Is_Script_Attached((ScriptableGameObj*)z->Data(),Script)) {
				z->Data()->Set_Delete_Pending();
			}
		}
	}
}

bool Has_Beacon(ArmedGameObj *obj) {
	WeaponBagClass *Bag = obj->Get_Weapon_Bag();
	for (int i = 1;i < Bag->Get_Count();i++) {
		if (Bag->Peek_Weapon(i)->Get_Definition()->Style == STYLE_BEACON && Bag->Peek_Weapon(i)->Get_Total_Rounds()) {
			return true;
		}
	}
	return false;
}

float Get_Distance_To_Closest_Building(const Vector3 &Position,int Team) {
	float ClosestDist = FLT_MAX;
	for (SLNode<BuildingGameObj> *z = GameObjManager::BuildingGameObjList.Head();z;z = z->Next()) {
		if ((z->Data()->Get_Player_Type() == Team || Team == 2) && !z->Data()->Is_Destroyed()) {
			float Dist = 0.0f;
			z->Data()->Find_Closest_Poly(Position,&Dist);
			if (Dist < ClosestDist) {
				ClosestDist = Dist;
			}
		}
	}
	return ClosestDist;
}

float Get_Distance_To_Closest_Beacon(const Vector3 &Position,int Team) {
	float ClosestDist = FLT_MAX;
	for (SLNode<BeaconGameObj> *z = GameObjManager::BeaconGameObjList.Head();z;z = z->Next()) {
		if (z->Data()->Get_Player_Type() == Team || Team == 2) {
			float Dist = Commands->Get_Distance(Position,Commands->Get_Position(z->Data()));
			if (Dist < ClosestDist) {
				ClosestDist = Dist;
			}
		}
	}
	return ClosestDist;
}

float Get_Distance_To_Closest_PCT(const Vector3 &Position,int Team) {
	float ClosestDist = FLT_MAX;
	for (SLNode<BaseGameObj> *z = GameObjManager::GameObjList.Head();z;z = z->Next()) {
		if (z->Data()->As_PhysicalGameObj() && ((PhysicalGameObj*)z->Data())->As_SimpleGameObj()) {
			if ((Team == 0 && ((SimpleGameObj*)z->Data())->Get_Definition().Get_Player_Terminal_Type() == 1) || (Team == 1 && ((SimpleGameObj*)z->Data())->Get_Definition().Get_Player_Terminal_Type() == 0) || Team == 2) { //Stupid backwards teams.
				float Dist = Commands->Get_Distance(Position,((PhysicalGameObj*)z->Data())->Get_Position());
				if (Dist < ClosestDist) {
					ClosestDist = Dist;
				}
			}
		}
	}
	return ClosestDist;
}

BuildingGameObj *Get_Closest_Building(const Vector3 &Position,int Team) {
	float ClosestDist = FLT_MAX;
	BuildingGameObj *Closest = 0;
	for (SLNode<BuildingGameObj> *z = GameObjManager::BuildingGameObjList.Head();z;z = z->Next()) {
		if ((z->Data()->Get_Player_Type() == Team || Team == 2) && !z->Data()->Is_Destroyed()) {
			float Dist = 0.0f;
			z->Data()->Find_Closest_Poly(Position,&Dist);
			if (Dist < ClosestDist) {
				ClosestDist = Dist;
				Closest = z->Data();
			}
		}
	}
	return Closest;
}

BuildingGameObj *Get_Random_Building(int Team) {
	BaseControllerClass *Base = BaseControllerClass::Find_Base(Team);
	if (!Base || !Base->Get_Building_List().Count() || Base->Is_Base_Destroyed()) {
		return 0;
	}
	BuildingGameObj *Return = Base->Get_Building_List()[Get_Random_Int(0,Base->Get_Building_List().Count())];
	if (Return->Is_Destroyed()) {
		if (Get_Random_Int(0,2)) {
			for (int i = 0;i < Base->Get_Building_List().Count();i++) {
				if (!Base->Get_Building_List()[i]->Is_Destroyed()) {
					return Base->Get_Building_List()[i];
				}
			}
		}
		else {
			for (int i = Base->Get_Building_List().Count()-1;i >= 0;i--) {
				if (!Base->Get_Building_List()[i]->Is_Destroyed()) {
					return Base->Get_Building_List()[i];
				}
			}
		}
		return 0;
	}
	return Return;
}

void Send_Custom_Event_Vehicle_Occupants(GameObject *obj,GameObject *Sender,int Message,int Param,float Delay,bool Driver) {
	VehicleGameObj *Vehicle = obj->As_VehicleGameObj();
	if (!Vehicle) {
		return;
	}

	int x = Vehicle->Get_Definition().Get_Seat_Count();
	for (int i = Driver?0:1;i < x;i++) {
		if (Vehicle->Get_Occupant(i)) {
			Commands->Send_Custom_Event(Sender,Vehicle->Get_Occupant(i),Message,Param,Delay);
		}
	}
}

void Disarm_Closest_Beacon(const Vector3 &Position,int Team) {
	float ClosestDist = FLT_MAX;
	BeaconGameObj *Closest = 0;
	for (SLNode<BeaconGameObj> *z = GameObjManager::BeaconGameObjList.Head();z;z = z->Next()) {
		if (z->Data()->Get_Player_Type() == Team || Team == 2) {
			float Dist = Commands->Get_Distance(Position,Commands->Get_Position(z->Data()));
			if (Dist < ClosestDist) {
				ClosestDist = Dist;
				Closest = z->Data();
			}
		}
	}
	if (Closest) {
		OffenseObjectClass offense(0);
		Closest->Completely_Damaged(offense);
	}
}

void Disarm_All_Beacons_Team(int Team) {
	for (SLNode<BeaconGameObj> *z = GameObjManager::BeaconGameObjList.Head();z;z = z->Next()) {
		if (z->Data()->Get_Player_Type() == Team || Team == 2) {
			OffenseObjectClass offense(0);
			z->Data()->Completely_Damaged(offense);
		}
	}
}

void Disarm_All_Beacons_Player(GameObject *Player) {
	for (SLNode<BeaconGameObj> *z = GameObjManager::BeaconGameObjList.Head();z;z = z->Next()) {
		if (z->Data()->Get_Owner() == Player) {
			OffenseObjectClass offense(0);
			z->Data()->Completely_Damaged(offense);
		}
	}
}

void Play_Animation_All_Players(const char *Anim) {
	for (SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next()) {
		Commands->Set_Animation(z->Data(),Anim,false,0,0.0f,-1.0f,false);
	}
}

StringClass Get_String_Player_Name(GameObject *obj) {
	if (!obj || !obj->As_SoldierGameObj()) {
		return "None";
	}
	if (!((SoldierGameObj*)obj)->Get_Player()) {
		return "None";
	}
	return ((SoldierGameObj*)obj)->Get_Player()->Get_Name().Peek_Buffer();
}

StringClass Get_String_Player_Name_By_ID(int ID) {
	cPlayer *x = Find_Player(ID);
	if (!x) {
		return "None";
	}
	return x->Get_Name().Peek_Buffer();
}

const wchar_t *Get_Wide_Player_Name_By_ID(int ID) {
	cPlayer *x = Find_Player(ID);
	if (!x) {
		return L"None";
	}
	return x->PlayerName;
}

StringClass Get_IP_Address_String(int PlayerID) {
	const sockaddr_in *addr = Get_IP(PlayerID);
	int ip1 = addr->sin_addr.s_addr&0x000000FF;
	int ip2 = (addr->sin_addr.s_addr&0x0000FF00)>>8;
	int ip3 = (addr->sin_addr.s_addr&0x00FF0000)>>16;
	int ip4 = (addr->sin_addr.s_addr&0xFF000000)>>24;
	return StringFormat("%d.%d.%d.%d",ip1,ip2,ip3,ip4);
}

const wchar_t *Get_Wide_Team_Name(int Team) {
	if (!Team) {
		return Get_Wide_Translated_String(7198);
	}
	else if (Team == 1) {
		return Get_Wide_Translated_String(7199);
	}
	return L"Neutral";
}

int Find_Empty_Vehicle_Seat(VehicleGameObj *obj,bool AllowDriver) {
	int x = obj->Get_Definition().Get_Seat_Count();
	for (int i = 0;i < x;i++) {
		if (!obj->Get_Occupant(i)) {
			if (i == 0) {
				if (AllowDriver) {
					return i;
				}
			}
			else {
				return i;
			}
		}
	}
	return -1;
}

ScriptImpClass *Get_Script_By_Name(GameObject *obj,const char *Script) {
	if (!obj->As_ScriptableGameObj()) {
		return 0;
	}
	const SimpleDynVecClass<GameObjObserverClass *> *observers = &obj->Get_Observers();
	for (int i = 0;i < observers->Count();i++) {
		if (!_stricmp((*observers)[i]->Get_Name(),Script)) {
			return (ScriptImpClass*)(*observers)[i];
		}
	}
	return 0;
}

void Change_Team_3(cPlayer *Player,int Team) {
	if (Player->Get_Money() > (float)The_Cnc_Game()->StartingCredits) { //Allow players to keep their starting credits when switching teams.
		Give_Credits_Team(Player->Get_Player_Type(),(Player->Get_Money()-(float)The_Cnc_Game()->StartingCredits)/(Get_Team_Player_Count(Player->Get_Team())-1)); //Distribute anything above the starting credits to their teammates.
		Player->Set_Money((float)The_Cnc_Game()->StartingCredits);
	}
	Disarm_All_C4_Beacons(Player->Get_ID());
	int OldTeam = Player->Get_Player_Type();
	Player->Set_Player_Type(Team);
	if (Player->Destroy_GameObj()) {
		Player->Set_Deaths(Player->Get_Deaths()-1);
		cTeam *Team = Find_Team(OldTeam);
		if (Team) {
			Team->Dec_Deaths();
		}
	}
}

void Change_Team_4(cPlayer *Player,int Team) {
	Player->Set_Money((float)The_Cnc_Game()->StartingCredits);
	Disarm_All_C4_Beacons(Player->Get_ID());
	int OldTeam = Player->Get_Player_Type();
	Player->Set_Player_Type(Team);
	if (Player->Destroy_GameObj()) {
		Player->Set_Deaths(Player->Get_Deaths()-1);
		cTeam *Team = Find_Team(OldTeam);
		if (Team) {
			Team->Dec_Deaths();
		}
	}
}

void Change_Team_5(cPlayer *Player,int Team) {
	Player->Set_Score(0);
	Player->Set_Kills(0);
	Player->Set_Money((float)The_Cnc_Game()->StartingCredits);
	Disarm_All_C4_Beacons(Player->Get_ID());
	int OldTeam = Player->Get_Player_Type();
	Player->Set_Player_Type(Team);
	if (Player->Destroy_GameObj()) {
		Player->Set_Deaths(-1);
		cTeam *Team = Find_Team(OldTeam);
		if (Team) {
			Team->Dec_Deaths();
		}
	}
	else {
		Player->Set_Deaths(0);
	}
}

cPlayer *Match_Player(cPlayer *Player,const StringClass &Nick,bool TeamOnly,bool AllowSelf) {
	cPlayer *Return = 0;
	int Team = Player->Get_Team();
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data()->Is_Active() && (AllowSelf || Player != z->Data())) {
			if (!TeamOnly || Team == z->Data()->Get_Player_Type()) {
				if (stristr(StringClass(z->Data()->Get_Name().Peek_Buffer()),Nick)) {
					if (Return) {
						if (Player) {
							DA::Page_Player(Player,"Multiple players were found matching \"%s.\" Please supply a more unique wildcard.",Nick);
						}
						return 0;
					}
					else {
						Return = z->Data();
					}
				}
			}
		}
	}
	if (!Return) {
		if (Player) {
			DA::Page_Player(Player,"No players were found matching \"%s.\"",Nick);
		}
		return 0;
	}
	return Return;
}

void Damage_Objects_Half_No_Kill() {
	for (SLNode<SmartGameObj> *z = GameObjManager::SmartGameObjList.Head();z;z = z->Next()) {
		DefenseObjectClass *Defense = z->Data()->Get_Defense_Object();
		float MaxHealth = Defense->Get_Health_Max() + Defense->Get_Shield_Strength_Max();
		float Health = Defense->Get_Health() + Defense->Get_Shield_Strength();
		if ((MaxHealth / 2) >= Health) {
			Commands->Apply_Damage(z->Data(),Health-1.0f,"None",0);
		}
		else {
			Commands->Apply_Damage(z->Data(),(MaxHealth / 2),"None",0);
		}
	}
}

void Attach_Script_To_Definition(const char *Preset,const char *Script,const char *Params) {
	ScriptableGameObjDef *Def = (ScriptableGameObjDef*)Find_Named_Definition(Preset);
	if (Def) {
		Def->ScriptNameList.Add(Script);
		Def->ScriptParameterList.Add(Params);
	}
}

void Attach_Script_To_Definitions(unsigned long CID,const char *Script,const char *Params) {
	for (ScriptableGameObjDef *Def = (ScriptableGameObjDef*)DefinitionMgrClass::Get_First(CID,DefinitionMgrClass::ID_CLASS);Def;Def = (ScriptableGameObjDef*)DefinitionMgrClass::Get_Next(Def,CID,DefinitionMgrClass::ID_CLASS)) {
		Def->ScriptNameList.Add(Script);
		Def->ScriptParameterList.Add(Params);
	}
}

void TDBObjClass::Set_English_String (const char *string) {
	EnglishString = string;
}

void Grant_Weapon(GameObject *obj,const char *Weapon,int Rounds) {
	if (obj && obj->As_PhysicalGameObj() && ((PhysicalGameObj*)obj)->As_ArmedGameObj()) {
		((ArmedGameObj*)obj)->Get_Weapon_Bag()->Add_Weapon(Weapon,Rounds,true);
	}
}

void Set_Object_Type_Preset(const char *Preset,int Team) {
	for (SLNode<BaseGameObj> *z = GameObjManager::GameObjList.Head();z;z = z->Next()) {
		if (!_stricmp(Commands->Get_Preset_Name(z->Data()->As_ScriptableGameObj()),Preset)) {
			Set_Object_Type(z->Data()->As_ScriptableGameObj(),Team);
		}
	}
} 

GameObject *Get_Closest_Vehicle(GameObject *obj,float MaxDistance) {
	if (!obj || !Commands->Get_ID(obj)) {
		return 0;
	}
	GameObject *Closest = 0;
	float ClosestDistance = 99999.0f;
	Vector3 Pos = Commands->Get_Position(obj);
	for (SLNode<SmartGameObj> *z = GameObjManager::SmartGameObjList.Head();z;z = z->Next()) {
		GameObject *o = z->Data();
		if (o && o->As_VehicleGameObj()) {
			float Distance = Commands->Get_Distance(Pos,Commands->Get_Position(o));
			if (Distance < ClosestDistance) {
				ClosestDistance = Distance;
				Closest = o;
			}
		}
	}
	if (ClosestDistance > MaxDistance) {
		return 0;
	}
	return Closest;
}

float Get_Distance_To_Closest_Player(const Vector3 &Position,int Team) {
	float Return = 9999.0f;
	for (SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next()) {
		float Dis = Commands->Get_Distance(Commands->Get_Position(z->Data()),Position);
		if (Dis < Return) {
			Return = Dis;
		}
	}
	return Return;
} 

void Give_Credits_Team(int Team,float Credits) {
	for (SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next()) {
		GameObject *obj = z->Data();
		if (Get_Object_Type(obj) == Team) {
			Commands->Give_Money(obj,Credits,false);
		}
	}
}

const char *PhysClass::Get_Name() {
	return Name;
}

void Give_Points_Players_In_Range(const Vector3 &Position,float Range,float Points,bool GiveMoney) {
	for (SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next()) {
		GameObject *obj = z->Data();
		if (Commands->Get_Distance(Commands->Get_Position(obj),Position) <= Range) {
			Commands->Give_Points(obj,Points,false);
			if (!GiveMoney) {
				Commands->Give_Points(obj,Points*-1.0f,false);
			}
		}
	}
}

void Give_Points_Players_In_Range_Team(int Team,const Vector3 &Position,float Range,float Points,bool GiveMoney) {
	for (SLNode<SoldierGameObj> *z = GameObjManager::StarGameObjList.Head();z;z = z->Next()) {
		GameObject *obj = z->Data();
		if (Get_Object_Type(obj) == Team && Commands->Get_Distance(Commands->Get_Position(obj),Position) <= Range) {
			Commands->Give_Points(obj,Points,false);
			if (!GiveMoney) {
				Commands->Give_Points(obj,Points*-1.0f,false);
			}
		}
	}
}

StringClass Encrypt_Apgar(const char *Password) {
	int Len = strlen(Password);
	char Lookup[70] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";
	StringClass Return;
	for (int i = 0;i < Len;i++) {
		char Left = Password[i];
		char Right = ((i > 0) ? Password[Len - i] : 0);
		char x = ((Left & 1) > 0 ? (Left << 1) & Right : Left ^ Right);
		Return += Lookup[x & 63]; 
	}
	return Return;
}

void Set_Facing_Player(GameObject *obj,float Facing) {
	float Angle = Facing*WWMATH_PI/180.0f;
	Vector3 Position = Commands->Get_Position(obj);
	Position.X += cos(Angle)*2.0f;
	Position.Y += sin(Angle)*2.0f;
	Position.Z += 4.0f;
	Force_Camera_Look_Player(obj,Position);
}

bool Overlap_Test(GameObject *obj,const Vector3 &Position,bool CheckDynOnly) {
	if (!obj || !obj->As_PhysicalGameObj()) {
		return false;
	}
	MoveablePhysClass *Phys = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
	if (!Phys || (!Phys->As_RigidBodyClass() && !Phys->As_Phys3Class())) {
		return false;
	}
	Matrix3D Transform = Phys->Get_Transform();
	Transform.Set_Translation(Position);
	return !Phys->Can_Teleport(Transform,CheckDynOnly);
}

void Console_InputF(const char *Format,...) {
	va_list arg_list;
	va_start(arg_list,Format);
	char Buffer[256];
	vsnprintf(Buffer,256,Format,arg_list);
	Console_Input(Buffer);
	va_end(arg_list);
}

PUSH_MEMORY_MACROS
#undef new
void Update_Game_Settings(int ID) {
	if (ID) {
		cGameOptionsEvent *Event = (cGameOptionsEvent*)operator new(sizeof(cGameOptionsEvent));
		Event->Constructor();
		Event->Init(ID);
	}
	else {
		cGameOptionsEvent *Event = (cGameOptionsEvent*)operator new(sizeof(cGameOptionsEvent));
		Event->Constructor();
		Event->Init();
	}
}
POP_MEMORY_MACROS

bool Is_Stealth_Unit(GameObject *obj) {
	if (!obj || !obj->As_SmartGameObj()) {
		return false;
	}
	return obj->As_SmartGameObj()->Get_Definition().Is_Stealthed();
}

//The client VehicleFactoryGameObj::Get_Team_Vehicle_Count is bugged and counts the harvester towards the vehicle limit.
//The server VehicleFactoryGameObj::Get_Team_Vehicle_Count doesn't suffer from this bug, thus the following function is used instead so that the server and client vehicle counts match up.
unsigned int Get_Ground_Vehicle_Count(int Team) {
	int Count = 0;
	if (Find_Building_By_Type(Team,BuildingConstants::TYPE_HELIPAD)) {
		for (SLNode<VehicleGameObj> *z = GameObjManager::VehicleGameObjList.Head();z;z = z->Next()) {
			VehicleGameObj *Vehicle = z->Data();
			const VehicleGameObjDef *VehicleDef = &Vehicle->Get_Definition();
			if (VehicleDef->Get_Type() != VEHICLE_TYPE_TURRET && Vehicle->Peek_Physical_Object()->As_MoveablePhysClass() && VehicleDef->Get_Build_At_Ground() && VehicleDef->Get_Default_Player_Type() == Team) {
				Count++;
			}
		}
	}
	else {
		for (SLNode<VehicleGameObj> *z = GameObjManager::VehicleGameObjList.Head();z;z = z->Next()) {
			VehicleGameObj *Vehicle = z->Data();
			const VehicleGameObjDef *VehicleDef = &Vehicle->Get_Definition();
			if (VehicleDef->Get_Type() != VEHICLE_TYPE_TURRET && Vehicle->Peek_Physical_Object()->As_MoveablePhysClass() && VehicleDef->Get_Default_Player_Type() == Team) {
				Count++;
			}
		}
	}
	return Count;
}

unsigned int Get_Air_Vehicle_Count(int Team) {
	int Count = 0;
	for (SLNode<VehicleGameObj> *z = GameObjManager::VehicleGameObjList.Head();z;z = z->Next()) {
		VehicleGameObj *Vehicle = z->Data();
		const VehicleGameObjDef *VehicleDef = &Vehicle->Get_Definition();
		if (!VehicleDef->Get_Build_At_Ground() && VehicleDef->Get_Type() == VEHICLE_TYPE_FLYING && VehicleDef->Get_Default_Player_Type() == Team) {
			Count++;
		}
	} 
	return Count;
}

unsigned int Get_Naval_Vehicle_Count(int Team) {
	int Count = 0;
	for (SLNode<VehicleGameObj> *z = GameObjManager::VehicleGameObjList.Head();z;z = z->Next()) {
		VehicleGameObj *Vehicle = z->Data();
		const VehicleGameObjDef *VehicleDef = &Vehicle->Get_Definition();
		if (!VehicleDef->Get_Build_At_Ground() && (VehicleDef->Get_Type() == VEHICLE_TYPE_BOAT || VehicleDef->Get_Type() == VEHICLE_TYPE_SUB) && VehicleDef->Get_Default_Player_Type() == Team) {
			Count++;
		}
	} 
	return Count;
}

void Set_Object_Dirty_Bit_For_Version_Greater_Than(NetworkObjectClass *obj,float Version,NetworkObjectClass::DIRTY_BIT Bit,bool OnOff) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data()->Get_DA_Player()->Get_Version() >= Version) {
			obj->Set_Object_Dirty_Bit(z->Data()->Get_ID(),Bit,OnOff);
		}
	}
}

void Set_Object_Dirty_Bit_For_Version_Less_Than(NetworkObjectClass *obj,float Version,NetworkObjectClass::DIRTY_BIT Bit,bool OnOff) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data()->Get_DA_Player()->Get_Version() < Version) {
			obj->Set_Object_Dirty_Bit(z->Data()->Get_ID(),Bit,OnOff);
		}
	}
}

void Set_Object_Dirty_Bit_For_Team_Version_Greater_Than(NetworkObjectClass *obj,int Team,float Version,NetworkObjectClass::DIRTY_BIT Bit,bool OnOff) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data()->Get_DA_Player()->Get_Version() >= Version && z->Data()->Get_Team() == Team) {
			obj->Set_Object_Dirty_Bit(z->Data()->Get_ID(),Bit,OnOff);
		}
	}
}

void Set_Object_Dirty_Bit_For_Team_Version_Less_Than(NetworkObjectClass *obj,int Team,float Version,NetworkObjectClass::DIRTY_BIT Bit,bool OnOff) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		if (z->Data()->Get_DA_Player()->Get_Version() < Version && z->Data()->Get_Team() == Team) {
			obj->Set_Object_Dirty_Bit(z->Data()->Get_ID(),Bit,OnOff);
		}
	}
}

static WideStringClass NickSave;
static cPlayer *NickSavePlayer = 0;

int Setup_Send_Message_Fake(const char *NewNick,int ID) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Is_Active()) {
			NickSave = Player->Get_Name();
			NickSavePlayer = Player;
			Player->PlayerName = NewNick;
			if (ID) {
				Player->Set_Object_Dirty_Bit(ID,NetworkObjectClass::BIT_CREATION,true);
				//Send_Object_Update(Player,ID);
				Update_Network_Object(Player);
			}
			else {
				//Player->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_CREATION,true);
				Set_Object_Dirty_Bit_For_Version_Less_Than(Player,2.6f,NetworkObjectClass::BIT_CREATION,true);
				Update_Network_Object(Player);
			}
			return Player->Get_ID();
		}
	}
	return 0;
}

int Setup_Send_Message_Team_Fake(const char *NewNick,int Team,int ID) {
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Is_Active() && Player->Get_Team() == Team) {
			NickSave = Player->Get_Name();
			NickSavePlayer = Player;
			Player->PlayerName = NewNick;
			if (ID) {
				Player->Set_Object_Dirty_Bit(ID,NetworkObjectClass::BIT_CREATION,true);
				//Send_Object_Update(Player,ID);
				Update_Network_Object(Player);
			}
			else {
				//Player->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_CREATION,true);
				Set_Object_Dirty_Bit_For_Version_Less_Than(Player,2.6f,NetworkObjectClass::BIT_CREATION,true);
				Update_Network_Object(Player);
			}
			return Player->Get_ID();
		}
	}
	return 0;
}

void Restore_Send_Message_Fake() {
	if (NickSavePlayer) {
		NickSavePlayer->PlayerName = NickSave;
		//NickSavePlayer->Set_Object_Dirty_Bit(NetworkObjectClass::BIT_CREATION,true);
		Set_Object_Dirty_Bit_For_Version_Less_Than(NickSavePlayer,2.6f,NetworkObjectClass::BIT_CREATION,true);
		Update_Network_Object(NickSavePlayer);
		DAEventManager::Name_Change_Event(NickSavePlayer);
		NickSavePlayer = 0;
	}
}

void Send_Announcement(const char *StringID) {
	TDBObjClass *TDBO = TranslateDBClass::Find_Object(StringID);
	if (TDBO) {
		Send_Client_Announcement(0,-1,TDBO->Get_ID(),ANNOUNCE_PUBLIC,0,true,true);
	}
}

void Send_Announcement_Version_Less_Than(const char *StringID,float Version) {
	TDBObjClass *TDBO = TranslateDBClass::Find_Object(StringID);
	if (TDBO) {
		SCAnnouncement *RadioEvent = Send_Client_Announcement(0,-1,TDBO->Get_ID(),ANNOUNCE_PUBLIC,0,false,false);
		for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
			cPlayer *Player = z->Data();
			if (Player->Is_Active() && Player->Get_DA_Player()->Get_Version() < Version) {
				RadioEvent->Set_Object_Dirty_Bits(Player->Get_ID(),NetworkObjectClass::BIT_CREATION);
			}
		}
	}
}

void Send_Announcement_Player(int ID,const char *StringID) {
	TDBObjClass *TDBO = TranslateDBClass::Find_Object(StringID);
	if (TDBO) {
		SCAnnouncement *RadioEvent = Send_Client_Announcement(0,-1,TDBO->Get_ID(),ANNOUNCE_PUBLIC,0,false,false);
		RadioEvent->Set_Object_Dirty_Bits(ID,NetworkObjectClass::BIT_CREATION);
	}
}

void Send_Announcement_Player_Version_Less_Than(int ID,const char *StringID,float Version) {
	if (Get_Client_Version(ID) < Version) {
		TDBObjClass *TDBO = TranslateDBClass::Find_Object(StringID);
		if (TDBO) {
			SCAnnouncement *RadioEvent = Send_Client_Announcement(0,-1,TDBO->Get_ID(),ANNOUNCE_PUBLIC,0,false,false);
			RadioEvent->Set_Object_Dirty_Bits(ID,NetworkObjectClass::BIT_CREATION);
		}
	}
}

void Send_Announcement_Team(int Team,const char *StringID) {
	TDBObjClass *TDBO = TranslateDBClass::Find_Object(StringID);
	if (TDBO) {
		SCAnnouncement *RadioEvent = Send_Client_Announcement(0,-1,TDBO->Get_ID(),ANNOUNCE_PUBLIC,0,false,false);
		for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
			cPlayer *Player = z->Data();
			if (Player->Is_Active() && Player->Get_Player_Type() == Team) {
				RadioEvent->Set_Object_Dirty_Bits(Player->Get_ID(),NetworkObjectClass::BIT_CREATION);
			}
		}
	}
}

void Send_Announcement_Team_Version_Less_Than(int Team,const char *StringID,float Version) {
	TDBObjClass *TDBO = TranslateDBClass::Find_Object(StringID);
	if (TDBO) {
		SCAnnouncement *RadioEvent = Send_Client_Announcement(0,-1,TDBO->Get_ID(),ANNOUNCE_PUBLIC,0,false,false);
		for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
			cPlayer *Player = z->Data();
			if (Player->Is_Active() && Player->Get_Player_Type() == Team && Player->Get_DA_Player()->Get_Version() < Version) {
				RadioEvent->Set_Object_Dirty_Bits(Player->Get_ID(),NetworkObjectClass::BIT_CREATION);
			}
		}
	}
}

void Send_Message_Team_With_Team_Color(int Team,const char *Msg) {
	unsigned int Red = 0,Blue = 0,Green = 0;
	Get_Team_Color(Team,&Red,&Green,&Blue);
	Send_Message_Team(Team,Red,Green,Blue,Msg);
}

void Send_Message_Player_By_ID(int ID,unsigned int Red,int unsigned Green,int unsigned Blue,const char *Message) {
	WideStringClass Send;
	Send.Format(L"j\n44\n%u\n%u\n%u\n%hs\n,",Red,Green,Blue,Message);
	Send_Client_Text(Send,TEXT_MESSAGE_PRIVATE,false,-2,ID,true,true);
}

void Create_2D_WAV_Sound_Player_By_ID(int ID,const char *Sound) {
	WideStringClass Send;
	Send.Format(L"j\n10\n%hs\n,",Sound);
	Send_Client_Text(Send,TEXT_MESSAGE_PRIVATE,false,-2,ID,true,true);
}

void Set_Obj_Radar_Blip_Shape_Player_By_ID(int ID,GameObject *obj,int Shape) {
	WideStringClass Send;
	Send.Format(L"j\n39\n%d\n%d\n",Commands->Get_ID(obj),Shape);
	Send_Client_Text(Send,TEXT_MESSAGE_PRIVATE,false,-2,ID,true,true);
}

void Set_Obj_Radar_Blip_Color_Player_By_ID(int ID,GameObject *obj,int Color) {
	WideStringClass Send;
	Send.Format(L"j\n40\n%d\n%d\n",Commands->Get_ID(obj),Color);
	Send_Client_Text(Send,TEXT_MESSAGE_PRIVATE,false,-2,ID,true,true);
}

PhysicalGameObj *Create_Object(const char *Preset,const Vector3 &Position) {
	DefinitionClass *Def = Find_Named_Definition(Preset);
	if (Def) {
		PhysicalGameObj *obj = (PhysicalGameObj*)Def->Create();
		if (obj->As_PhysicalGameObj()) {
			obj->Set_Position(Position);
			obj->Start_Observers();
			return obj;
		}
		else {
			obj->Set_Delete_Pending();
			return 0;
		}
	}
	return 0;
}

PhysicalGameObj *Create_Object(const char *Preset,const Matrix3D &Transform) {
	DefinitionClass *Def = Find_Named_Definition(Preset);
	if (Def) {
		PhysicalGameObj *obj = (PhysicalGameObj*)Def->Create();
		if (obj->As_PhysicalGameObj()) {
			obj->Set_Transform(Transform);
			obj->Start_Observers();
			return obj;
		}
		else {
			obj->Set_Delete_Pending();
			return 0;
		}
	}
	return 0;
}

PhysicalGameObj *Create_Object(int Preset,const Vector3 &Position) {
	DefinitionClass *Def = Find_Definition(Preset);
	if (Def) {
		PhysicalGameObj *obj = (PhysicalGameObj*)Def->Create();
		if (obj->As_PhysicalGameObj()) {
			obj->Set_Position(Position);
			obj->Start_Observers();
			return obj;
		}
		else {
			obj->Set_Delete_Pending();
			return 0;
		}
	}
	return 0;
}

PhysicalGameObj *Create_Object(int Preset,const Matrix3D &Transform) {
	DefinitionClass *Def = Find_Definition(Preset);
	if (Def) {
		PhysicalGameObj *obj = (PhysicalGameObj*)Def->Create();
		if (obj->As_PhysicalGameObj()) {
			obj->Set_Transform(Transform);
			obj->Start_Observers();
			return obj;
		}
		else {
			obj->Set_Delete_Pending();
			return 0;
		}
	}
	return 0;
}

PhysicalGameObj *Create_Object(const DefinitionClass *Def,const Vector3 &Position) {
	PhysicalGameObj *obj = (PhysicalGameObj*)Def->Create();
	if (obj->As_PhysicalGameObj()) {
		obj->Set_Position(Position);
		obj->Start_Observers();
		return obj;
	}
	else {
		obj->Set_Delete_Pending();
		return 0;
	}
}

PhysicalGameObj *Create_Object(const DefinitionClass *Def,const Matrix3D &Transform) {
	PhysicalGameObj *obj = (PhysicalGameObj*)Def->Create();
	if (obj->As_PhysicalGameObj()) {
		obj->Set_Transform(Transform);
		obj->Start_Observers();
		return obj;
	}
	else {
		obj->Set_Delete_Pending();
		return 0;
	}
}

//This function should only be called from a pre level loaded event. Calling it anywhere else will cause clients to crash.
void Disable_Building_Type(int Team,BuildingConstants::BuildingType Type) {
	BaseControllerClass *Base = BaseControllerClass::Find_Base(Team);
	for (int i = Base->Get_Building_List().Count()-1;i >= 0;i--) {
		if (Base->Get_Building_List()[i]->Get_Definition().Get_Type() == Type) {
			Base->Get_Building_List()[i]->Set_Delete_Pending();
			Base->Get_Building_List()[i]->Clear_Object_Dirty_Bits();
			Base->Get_Building_List().Delete(i);
		}
	}
}

unsigned int Get_Hash(const char *String) {
	int Len = strlen(String);
	if (Len >= 8) {
		return HashTemplateKeyClass<uint>::Get_Hash_Value((uint&)String[Len - 8]);
	}
	else {
		unsigned int Hash = 0;
		for (int i = 0; i < Len; i++) {
			Hash += String[i] + Hash * 37;
		}
		return Hash;
	}
}

bool Is_Starting_Weapon(const SoldierGameObjDef *Soldier,const PowerUpGameObjDef *PowerUp) {
	if (!PowerUp || !Soldier) {
		return false;
	}
	if (Soldier->WeaponDefID == PowerUp->GrantWeaponID || Soldier->SecondaryWeaponDefID == PowerUp->GrantWeaponID) {
		return true;
	}
	for (int i = 0;i < Soldier->ScriptNameList.Count();i++) {
		if (Soldier->ScriptNameList[i] == "M00_GrantPowerup_Created") {
			if (Soldier->ScriptParameterList[i] == PowerUp->Get_Name()) {
				return true;
			}
		}
	}
	return false;
}

bool Is_Starting_Weapon(SoldierGameObj *Soldier,const PowerUpGameObjDef *PowerUp) {
	if (!Soldier) {
		return false;
	}
	return Is_Starting_Weapon(&Soldier->Get_Definition(),PowerUp);
}

void WeaponBagClass::Remove_Weapon(const WeaponDefinitionClass *WeaponDef) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (WeaponList[x]->Get_Definition() == WeaponDef) {
			Remove_Weapon(x);
			return;
		}
	}
}

void WeaponBagClass::Remove_Weapon(const char *Weapon) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (!_stricmp(WeaponList[x]->Get_Definition()->Get_Name(),Weapon)) {
			Remove_Weapon(x);
			return;
		}
	}
}

void WeaponBagClass::Remove_Weapon(unsigned int WeaponDefID) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (WeaponList[x]->Get_Definition()->Get_ID() == WeaponDefID) {
			Remove_Weapon(x);
			return;
		}
	}
}

WeaponClass *WeaponBagClass::Find_Weapon(const WeaponDefinitionClass *WeaponDef) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (WeaponList[x]->Get_Definition() == WeaponDef) {
			return WeaponList[x];
		}
	}
	return 0;
}

WeaponClass *WeaponBagClass::Find_Weapon(const char *Weapon) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (!_stricmp(WeaponList[x]->Get_Definition()->Get_Name(),Weapon)) {
			return WeaponList[x];
		}
	}
	return 0;
}

WeaponClass *WeaponBagClass::Find_Weapon(unsigned int WeaponDefID) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (WeaponList[x]->Get_Definition()->Get_ID() == WeaponDefID) {
			return WeaponList[x];
		}
	}
	return 0;
}

int WeaponBagClass::Get_Weapon_Position(const WeaponDefinitionClass *WeaponDef) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (WeaponList[x]->Get_Definition() == WeaponDef) {
			return x;
		}
	}
	return 0;
}

int WeaponBagClass::Get_Weapon_Position(const char *Weapon) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (!_stricmp(WeaponList[x]->Get_Definition()->Get_Name(),Weapon)) {
			return x;
		}
	}
	return 0;
}

int WeaponBagClass::Get_Weapon_Position(unsigned int WeaponDefID) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (WeaponList[x]->Get_Definition()->Get_ID() == WeaponDefID) {
			return x;
		}
	}
	return 0;
}

int WeaponBagClass::Get_Weapon_Position(WeaponClass *Weapon) {
	for (int x = 1;x < WeaponList.Count();x++) {
		if (WeaponList[x] == Weapon) {
			return x;
		}
	}
	return 0;
}

void ScriptableGameObj::Start_Observer_Timer(int ObserverID,float Duration,int Number) {
	GameObjObserverTimerClass *Timer = new GameObjObserverTimerClass;
	Timer->ObserverID = ObserverID;
	Timer->Duration = Duration;
	Timer->Number = Number;
	ObserverTimerList.Add(Timer);
}

void ScriptableGameObj::Start_Custom_Timer(ScriptableGameObj *Sender,float Delay,int Message,int Param) {
	GameObjCustomTimerClass *Timer = new GameObjCustomTimerClass;
	Timer->Sender = Sender;
	Timer->Delay = Delay;
	Timer->Message = Message;
	Timer->Param = Param;
	CustomTimerList.Add(Timer);
}

void ScriptableGameObj::Stop_Observer_Timer(int ObserverID,int Number) {
	for (int i = ObserverTimerList.Count()-1;i >= 0;i--) {
		if (ObserverTimerList[i]->ObserverID == ObserverID && (!Number || ObserverTimerList[i]->Number == Number)) {
			ObserverTimerList[i]->ObserverID = -1;
			ObserverTimerList[i]->Number = -1;
		}
	}
}

void ScriptableGameObj::Stop_Custom_Timer(ScriptableGameObj *Sender,int Message) {
	for (int i = CustomTimerList.Count()-1;i >= 0;i--) {
		if (CustomTimerList[i]->Sender == Sender && (!Message || CustomTimerList[i]->Message == Message)) {
			delete CustomTimerList[i];
			CustomTimerList.Delete(i);
		}
	}
}

bool ScriptableGameObj::Is_Observer_Timer(int ObserverID,int Number) {
	for (int i = ObserverTimerList.Count()-1;i >= 0;i--) {
		if (ObserverTimerList[i]->ObserverID == ObserverID && (!Number || ObserverTimerList[i]->Number == Number)) {
			return true;
		}
	}
	return false;
}

bool ScriptableGameObj::Is_Custom_Timer(ScriptableGameObj *Sender,int Message) {
	for (int i = CustomTimerList.Count()-1;i >= 0;i--) {
		if (CustomTimerList[i]->Sender == Sender && (!Message || CustomTimerList[i]->Message == Message)) {
			return true;
		}
	}
	return false;
}

void Fix_Stuck_Objects(const Vector3 &Position,float Range) {
	for (SLNode<SmartGameObj> *x = GameObjManager::SmartGameObjList.Head();x;x = x->Next()) {
		PhysicalGameObj *obj = x->Data();
		if (obj->Peek_Physical_Object()->As_MoveablePhysClass() && Commands->Get_Distance(Position,obj->Get_Position()) <= Range && !((MoveablePhysClass*)obj->Peek_Physical_Object())->Can_Teleport(obj->Get_Transform(),true)) {
			if (!Fix_Stuck_Object(obj,Range)) {
				Commands->Apply_Damage(obj,99999.0f,"None",0);
			}
		}
	}
}

bool Fix_Stuck_Object(PhysicalGameObj *obj,float Range) {
	MoveablePhysClass *Phys = (MoveablePhysClass*)obj->Peek_Physical_Object();
	Matrix3D Transform = Phys->Get_Transform();
	Vector3 Position;
	Transform.Get_Translation(&Position);
	for (int i = 0;i < 50;i++) { //First try to find a new position without changing the Z axis.
		Transform = Phys->Get_Transform();
		Transform.Set_X_Translation(Transform.Get_X_Translation()+WWMath::Random_Float(Range*-1.0f,Range));
		Transform.Set_Y_Translation(Transform.Get_Y_Translation()+WWMath::Random_Float(Range*-1.0f,Range));
		if (Phys->Can_Teleport(Transform)) { //Don't use this position if it collides with something.
			CastResultStruct CastResult;
			PhysRayCollisionTestClass CollisionTest(LineSegClass(Transform.Get_Translation(),Position),&CastResult,BULLET_COLLISION_GROUP);
			PhysicsSceneClass::Get_Instance()->Cast_Ray(CollisionTest,false);
			if (CollisionTest.CollidedPhysObj == Phys) { //Only use this position if it can see the original position. Prevents teleporting through walls.
				obj->Set_Transform(Transform);
				return true;
			}
		}
	}
	for (int i = 0;i < 50;i++) { //Shit's getting serious. Start changing the Z axis too.
		Transform = Phys->Get_Transform();
		Transform.Set_X_Translation(Transform.Get_X_Translation()+WWMath::Random_Float(Range*-1.0f,Range));
		Transform.Set_Y_Translation(Transform.Get_Y_Translation()+WWMath::Random_Float(Range*-1.0f,Range));
		Transform.Set_Z_Translation(Transform.Get_Z_Translation()+WWMath::Random_Float(0.0f,Range));
		if (Phys->Can_Teleport(Transform)) {
			CastResultStruct CastResult;
			PhysRayCollisionTestClass CollisionTest(LineSegClass(Transform.Get_Translation(),Position),&CastResult,BULLET_COLLISION_GROUP);
			PhysicsSceneClass::Get_Instance()->Cast_Ray(CollisionTest,false);
			if (CollisionTest.CollidedPhysObj == Phys) {
				obj->Set_Transform(Transform);
				return true;
			}
		}
	}
	return false;
}

PUSH_MEMORY_MACROS
#undef new
void Send_Purchase_Response(int ID,int Type) {
	cPurchaseResponseEvent *Event = (cPurchaseResponseEvent*)operator new(sizeof(cPurchaseResponseEvent));
	Event->Constructor();
	Event->Init(Type,ID);
}
POP_MEMORY_MACROS

StringClass Clean_Model_Name(StringClass Model) {
	Model.cropTo(Model.Get_Length()-4);
	for (int i = Model.Get_Length();i >= 0;i--) {
		if (Model[i] == '\\') {
			Model.cropFrom(i+1);
			break;
		}
	}
	return Model;
}

StringClass Get_Weapon_PowerUp_Model(const WeaponDefinitionClass *Weapon) {
	StringClass Model = Clean_Model_Name(Weapon->Model);
	if (!Model.Is_Empty()) {
		Model[0] = 'p';
	}
	return Model;
}

void Add_Console_Function(ConsoleFunctionClass *Func) {
	if (Func->Get_Name()) {
		Delete_Console_Function(Func->Get_Name());
	}
	if (Func->Get_Alias()) {
		Delete_Console_Function(Func->Get_Alias());
	}
	ConsoleFunctionList.Add(Func);
	Sort_Function_List();
	Verbose_Help_File();
}

