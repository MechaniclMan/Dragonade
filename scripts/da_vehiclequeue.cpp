/*	Renegade Scripts.dll
    Dragonade Vehicle Purchase Queue Game Feature
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
#include "da_vehiclequeue.h"
#include "da_translation.h"
#include "da_player.h"
#include "da_game.h"
#include "VehicleFactoryGameObjDef.h"
#include "RefineryGameObj.h"

void DAVehicleQueueGameFeatureClass::Init() {
	Register_Event(DAEvent::LEVELLOADED);
	Register_Event(DAEvent::GAMEOVER);
	Register_Event(DAEvent::VEHICLEPURCHASEREQUEST);
	Register_Event(DAEvent::TEAMCHANGE);
	Register_Event(DAEvent::PLAYERLEAVE);
	Register_Object_Event(DAObjectEvent::CREATED,DAObjectEvent::VEHICLE);
	Register_Object_Event(DAObjectEvent::KILLRECEIVED,DAObjectEvent::BUILDING);
	Register_Object_Event(DAObjectEvent::DESTROYED,DAObjectEvent::VEHICLE);
	Register_Chat_Command((DAECC)&DAVehicleQueueGameFeatureClass::VQ_Chat_Command,"!vq|!queue|!q|!veh|!vehicle|!vehlimit|!vehiclelimit|!vlimit");
	Queue[0].Resize(8);
	Queue[1].Resize(8);
	Building[0] = 0;
	Building[1] = 0;

	//Take over spawning of the harvesters.
	BaseControllerClass *Base = BaseControllerClass::Find_Base(0);
	RefineryGameObj *Ref = (RefineryGameObj*)Base->Find_Building(BuildingConstants::TYPE_REFINERY);
	if (Ref && Base->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY)) {
		Ref->Block_Harvester_Spawn(); 
	}

	Base = BaseControllerClass::Find_Base(1);
	Ref = (RefineryGameObj*)Base->Find_Building(BuildingConstants::TYPE_REFINERY);
	if (Ref && Base->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY)) {
		Ref->Block_Harvester_Spawn();
	}
}

DAVehicleQueueGameFeatureClass::~DAVehicleQueueGameFeatureClass() {
	//Return control of spawning harvesters if unloaded mid-game.
	if (!DAGameManager::Is_Shutdown_Pending()) {
		BaseControllerClass *Base = BaseControllerClass::Find_Base(0);
		RefineryGameObj *Ref = (RefineryGameObj*)Base->Find_Building(BuildingConstants::TYPE_REFINERY);
		if (Ref) {
			Ref->Allow_Harvester_Spawn(); 
		}
		VehicleFactoryGameObj *VF = (VehicleFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
		if (VF && Is_Building(0)) {
			VF->Set_Busy(true);
		}

		Base = BaseControllerClass::Find_Base(1);
		Ref = (RefineryGameObj*)Base->Find_Building(BuildingConstants::TYPE_REFINERY);
		if (Ref) {
			Ref->Allow_Harvester_Spawn();
		}
		VF = (VehicleFactoryGameObj*)Base->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
		if (VF && Is_Building(1)) {
			VF->Set_Busy(true);
		}
	}

	Clear(0);
	Clear(1);
}

void DAVehicleQueueGameFeatureClass::Level_Loaded_Event() {
	Stop_Timer(1);
	Clear(0);
	Clear(1);

	//Take over spawning of the harvesters.
	BaseControllerClass *Base = BaseControllerClass::Find_Base(0);
	RefineryGameObj *Ref = (RefineryGameObj*)Base->Find_Building(BuildingConstants::TYPE_REFINERY);
	if (Ref && Ref->Get_Harvester_Def_ID() && Base->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY)) {
		Ref->Block_Harvester_Spawn();
		Ref->Destroy_Harvester();
		Spawn_Vehicle(0,0,(VehicleGameObjDef*)Find_Definition(Ref->Get_Harvester_Def_ID()),0);
	}

	Base = BaseControllerClass::Find_Base(1);
	Ref = (RefineryGameObj*)Base->Find_Building(BuildingConstants::TYPE_REFINERY);
	if (Ref && Ref->Get_Harvester_Def_ID() && Base->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY)) {
		Ref->Block_Harvester_Spawn();
		Ref->Destroy_Harvester();
		Spawn_Vehicle(1,0,(VehicleGameObjDef*)Find_Definition(Ref->Get_Harvester_Def_ID()),0);
	}
}

void DAVehicleQueueGameFeatureClass::Game_Over_Event() {
	Stop_Timer(1);
	Clear(0);
	Clear(1);
}

int DAVehicleQueueGameFeatureClass::Vehicle_Purchase_Request_Event(BaseControllerClass *Base,cPlayer *Player,float &Cost,const VehicleGameObjDef *Item) {
	int Team = Base->Get_Player_Type();
	if ((unsigned int)Player->Get_Money() < Cost) {
		return 2;
	}
	else if (!Is_Building(Team)) { //Build if VF is free.
		Player->Purchase_Item((int)Cost);
		Spawn_Vehicle(Team,Player,Item,Cost);
		return 0;
	}
	else if (Building[Team]->Purchaser == Player) {
		return 3;
	}
	else { //Add to queue if VF is busy.
		Add(Player,Item,Cost);
		return 1;
	}
}

void DAVehicleQueueGameFeatureClass::Team_Change_Event(cPlayer *Player) {
	Remove(0,Player);
	Remove(1,Player);
}

void DAVehicleQueueGameFeatureClass::Player_Leave_Event(cPlayer *Player) {
	Remove(0,Player);
	Remove(1,Player);
}

void DAVehicleQueueGameFeatureClass::Object_Created_Event(GameObject *obj) {
	int Team = ((VehicleGameObj*)obj)->Get_Definition().Get_Default_Player_Type();
	if (Team == 0 || Team == 1) {
		if (((VehicleGameObj*)obj)->Peek_Physical_Object()->As_MoveablePhysClass() && (unsigned int)Get_Ground_Vehicle_Count(Team) == Get_Vehicle_Limit()) {
			Clear(Team); //Clear the queue if the vehicle limit has been reached.
			DA::Team_Color_Message(Team,GRAY,"Vehicle limit reached.");
		}
	}
}

void DAVehicleQueueGameFeatureClass::Object_Destroyed_Event(GameObject *obj) {
	int Team = Get_Object_Type(obj);
	if (Team == 0 || Team == 1) {
		RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_REFINERY);
		if (Ref && !Ref->Is_Destroyed() && !Ref->Get_Allow_Harvester_Spawn()) {
			if ((unsigned int)Ref->Get_Harvester_Def_ID() == obj->Get_Definition().Get_ID()) {
				VehicleGameObjDef *HarvDef = (VehicleGameObjDef*)&obj->Get_Definition();
				if (Is_Building(Team)) { //Add harvester to top of queue if VF is busy.
					Queue[Team].Add_Head(new QueueStruct(0,HarvDef,0));
					Send_Positions(Team);
				}
				else { //Build harvester if VF is idle.
					Spawn_Vehicle(Team,0,HarvDef,0);
				}
			}
		}
	}
}

void DAVehicleQueueGameFeatureClass::Kill_Event(DamageableGameObj *Victim,ArmedGameObj *Killer,float Damage,unsigned int Warhead,DADamageType::Type Type,const char *Bone) {
	int Team = Victim->Get_Player_Type();
	if (Team == 0 || Team == 1) {
		if (((BuildingGameObj*)Victim)->As_PowerPlantGameObj()) { //Double cost of everything in the queue.
			for (int i = 0;i < Queue[Team].Count();i++) {
				Queue[Team][i]->Cost *= 2;
			}
		}
		else if (((BuildingGameObj*)Victim)->As_VehicleFactoryGameObj()) { //Clear queue.
			Clear(Team);
		}
		else if (((BuildingGameObj*)Victim)->As_RefineryGameObj()) { //Remove harvester from queue.
			if (Queue[Team].Count() && !Queue[Team][0]->Purchaser) {
				delete Queue[Team][0];
				Queue[Team].Delete(0);
				Send_Positions(Team);
			}
		}
	}
}

bool DAVehicleQueueGameFeatureClass::VQ_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	if (Player->Get_Team() == 0 || Player->Get_Team() == 1) {
		if (!Text.Size()) {
			Send_List(Player);
		}
		else if (Text[1] == "cancel" || Text[1] == "halt" || Text[1] == "stop" || Text[1] == "leave" || Text[1] == "end") {
			if (Remove(Player->Get_Team(),Player)) {
				DA::Private_Color_Message(Player,GRAY,"You have left the vehicle queue.");
			}
		}
	}
	return true;
}

void DAVehicleQueueGameFeatureClass::Spawn_Vehicle(int Team,QueueStruct *Q) {
	VehicleFactoryGameObj *VF = (VehicleFactoryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
	if (VF && !VF->Is_Destroyed()) {
		Building[Team] = Q;
		float Delay = 5.0f;
		if (!BaseControllerClass::Find_Base(Team)->Is_Base_Powered()) {
			Delay *= Get_Build_Time_Multiplier(Team);
		}
		VF->Request_Vehicle(Q->Vehicle->Get_ID(),Delay,Q->Purchaser?Q->Purchaser->Get_GameObj():0);
		if (Q->Purchaser && Q->Purchaser->Is_Alive_And_Kicking()) { //Gray out for the purchaser.
			Send_Object_Update(VF,Q->Purchaser->Get_ID());
			VF->Set_Busy(false); //Prevent from graying out for other players.
			VF->Set_Object_Dirty_Bit(Q->Purchaser->Get_ID(),NetworkObjectClass::BIT_RARE,false);
		}
		else {
			VF->Set_Busy(false);
		}
		Start_Timer(1,VF->Get_Definition().Get_Total_Building_Time()+0.1f,false,Team);
	}
	else {
		delete Q;
	}
}

void DAVehicleQueueGameFeatureClass::Spawn_Vehicle(int Team,cPlayer *Purchaser,const VehicleGameObjDef *Vehicle,float Cost) {
	Spawn_Vehicle(Team,new QueueStruct(Purchaser,Vehicle,Cost));
}

void DAVehicleQueueGameFeatureClass::Timer_Expired(int Number,unsigned int Team) {
	if (Building[Team]) {
		if (Building[Team]->Purchaser && Building[Team]->Purchaser->Is_Alive_And_Kicking()) {
			VehicleFactoryGameObj *VF = (VehicleFactoryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_VEHICLE_FACTORY);
			VF->Set_Object_Dirty_Bit(Building[Team]->Purchaser->Get_ID(),NetworkObjectClass::BIT_RARE,true);
		}
		delete Building[Team];
		Building[Team] = 0;
	}
	if (Queue[Team].Count()) {
		QueueStruct *Q = Queue[Team][0];
		Queue[Team].Delete(0);
		Send_Positions(Team);
		if (!Q->Purchaser) { //Harvester
			Spawn_Vehicle(Team,Q);
		}
		else if (Q->Purchaser->Purchase_Item((int)Q->Cost)) {
			Spawn_Vehicle(Team,Q);
			DAEventManager::Vehicle_Purchase_Event(Q->Purchaser,Q->Cost,Q->Vehicle); //Trigger the proper events.
			Send_Purchase_Response(Q->Purchaser->Get_ID(),0);
		}
		else {
			Timer_Expired(Number,Team);
			DA::Private_Color_Message(Q->Purchaser,GRAY,"Insufficient funds. You have been removed from the vehicle queue.");
			if (Team == 0) {
				DA::Create_2D_Sound_Player(Q->Purchaser,"M00EVAN_DSGN0024I1EVAN_snd.wav");
			}
			else {
				DA::Create_2D_Sound_Player(Q->Purchaser,"M00EVAG_DSGN0028I1EVAG_snd.wav");
			}
		}
	}
}

void DAVehicleQueueGameFeatureClass::Clear(int Team) {
	if (Building[Team]) {
		delete Building[Team];
		Building[Team] = 0;
	}
	for (int i = 0;i < Queue[Team].Count();i++) {
		delete Queue[Team][i];
	}
	Queue[Team].Delete_All();
}

void DAVehicleQueueGameFeatureClass::Add(cPlayer *Player,const VehicleGameObjDef *Vehicle,float Cost) {
	int Team = Player->Get_Team();
	if (Team == 0) {
		DA::Create_2D_Sound_Player(Player,"M00EVAN_DSGN0009I1EVAN_SND.wav");
	}
	else {
		DA::Create_2D_Sound_Player(Player,"M00EVAG_DSGN0012I1EVAG_SND.wav");
	}
	for (int i = 0;i < Queue[Team].Count();i++) {
		if (Queue[Team][i]->Purchaser == Player) {
			Queue[Team][i]->Vehicle = Vehicle;
			Queue[Team][i]->Cost = Cost;
			DA::Private_Color_Message(Player,GRAY,"You are in position %d/%d of the vehicle queue.",i+1,Queue[Team].Count());
			return;
		}
	}
	Queue[Team].Add(new QueueStruct(Player,Vehicle,Cost));
	DA::Private_Color_Message(Player,GRAY,"You are in position %d/%d of the vehicle queue.",Queue[Team].Count(),Queue[Team].Count());
}

bool DAVehicleQueueGameFeatureClass::Remove(int Team,cPlayer *Player) {
	for (int i = 0;i < Queue[Team].Count();i++) {
		if (Queue[Team][i]->Purchaser == Player) {
			delete Queue[Team][i];
			Queue[Team].Delete(i);
			Send_Positions(Team);
			return true;
		}
	}
	return false;
}

void DAVehicleQueueGameFeatureClass::Send_Positions(int Team) {
	for (int i = 0;i < Queue[Team].Count();i++) {
		if (Queue[Team][i]->Purchaser) {
			DA::Private_Color_Message(Queue[Team][i]->Purchaser,GRAY,"You are now in position %d/%d of the vehicle queue.",i+1,Queue[Team].Count());
		}
	}
}

void DAVehicleQueueGameFeatureClass::Send_List(cPlayer *Player) {
	int Team = Player->Get_Team();
	DA::Private_Color_Message(Player,GRAY,"Limit: %d/%d",Get_Ground_Vehicle_Count(Team),Get_Vehicle_Limit());
	if (Building[Team]) {
		if (Building[Team]->Purchaser) {
			DA::Private_Color_Message(Player,GRAY,"Building: %ls - %s",Building[Team]->Purchaser->Get_Name(),DATranslationManager::Translate(Building[Team]->Vehicle));
		}
		else {
			RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_REFINERY);
			DA::Private_Color_Message(Player,GRAY,"Building: %s - %s",DATranslationManager::Translate(Ref),DATranslationManager::Translate(Building[Team]->Vehicle));
		}
	}
	
	for (int i = 0;i < Queue[Team].Count();++i) {
		if (Queue[Team][i]->Purchaser) {
			DA::Private_Color_Message(Player,GRAY,"%d/%d: %ls - %s",i+1,Queue[Team].Count(),Queue[Team][i]->Purchaser->Get_Name(),DATranslationManager::Translate(Queue[Team][i]->Vehicle));
		}
		else {
			RefineryGameObj *Ref = (RefineryGameObj*)BaseControllerClass::Find_Base(Team)->Find_Building(BuildingConstants::TYPE_REFINERY);
			DA::Private_Color_Message(Player,GRAY,"%d/%d: %s - %s",i+1,Queue[Team].Count(),DATranslationManager::Translate(Ref),DATranslationManager::Translate(Queue[Team][i]->Vehicle));
		}
	}
}

Register_Game_Feature(DAVehicleQueueGameFeatureClass,"Vehicle Queue","EnableVehicleQueue",0);