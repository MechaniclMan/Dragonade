/*	Renegade Scripts.dll
    Dragonade Squads Game Feature
	Copyright 2013 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DASQUAD
#define INCLUDE_DASQUAD

#include "da_event.h"
#include "da_gamefeature.h"
#include "da_player.h"
#include "engine_game.h"

#define SQUADCOLOR GREEN

class DASquadClass;
class DASquadMemberClass;

struct DASquadInviteStruct {
	DASquadInviteStruct() {
		Leader = 0;
		Player = 0;
		Timeout = The_Game()->Get_Game_Duration_S();
	}
	DASquadInviteStruct(cPlayer *leader,cPlayer *player) {
		Leader = leader;
		Player = player;
		Timeout = The_Game()->Get_Game_Duration_S();
	}
	bool operator ==(const DASquadInviteStruct &Struct) {
		return false;
	}
	bool operator !=(const DASquadInviteStruct &Struct) {
		return false;
	}
	cPlayer *Leader;
	cPlayer *Player;
	int Timeout;
};

struct DASquadRemixStruct {
	DASquadRemixStruct() {
		Squad = 0;
		Team = -1;
	}
	DASquadRemixStruct(DASquadClass *s,int t) {
		Squad = s;
		Team = t;
	}
	bool operator ==(const DASquadRemixStruct &Struct) {
		return false;
	}
	bool operator !=(const DASquadRemixStruct &Struct) {
		return false;
	}
	DASquadClass *Squad;
	int Team;
};

class DA_API DASquadMemberClass : public DAPlayerObserverClass {
public:
	DASquadMemberClass(DASquadClass *squad) : DAPlayerObserverClass(), Squad(squad) { Set_Flags(DAPlayerFlags::PERSISTMAP); }
	virtual void Init();
	virtual void Name_Change();
	virtual ~DASquadMemberClass();
	virtual void Team_Change();
	virtual void Created();
	virtual void Player_Loaded();
	virtual void Timer_Expired(int Number,unsigned int Data);
	virtual void Vehicle_Enter(VehicleGameObj *Vehicle,int Seat);
	virtual void Vehicle_Exit(VehicleGameObj *Vehicle,int Seat);
	virtual void Character_Purchase(float Cost,const SoldierGameObjDef *Item);
	virtual void Vehicle_Purchase(float Cost,const VehicleGameObjDef *Item);
	virtual void PowerUp_Purchase(float Cost,const PowerUpGameObjDef *Item);

	bool Disband_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType);
	bool Promote_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType);
	bool Info_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType);
	bool SKick_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType);
	bool Leave_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType);
	bool Msg_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType);
	
	void Update_Radar(GameObject *Member = 0);
	void Set_Radar_Soldier(GameObject *Member = 0);
	void Set_Radar_Vehicle_Driver(GameObject *Member = 0);
	void Set_Radar_Vehicle_Passenger(GameObject *Member = 0);
	void Reset_Radar(GameObject *Member);

	virtual const char *Get_Observer_Name() {
		return "DASquadMemberClass";
	}
	inline DASquadClass *Get_Squad() {
		return Squad;
	}
	inline void Leave_Squad() {
		Set_Delete_Pending();
	}
	bool Is_Leader();

private:
	DASquadMemberClass();
	DASquadClass *Squad;
};

class DA_API DASquadClass {
public:
	DASquadClass(cPlayer *Player);
	bool Add(cPlayer *Player);
	bool Remove(cPlayer *Player);
	void Internal_Remove(DASquadMemberClass *Member);
	void Disband();
	
	void Squad_Message(const char *Format,...);
	void Squad_Message_Except(cPlayer *Player,const char *Format,...);
	void Leader_Message(const char *Format,...);

	inline bool Is_Member(cPlayer *Player) {
		for (int i = 0;i < Members.Count();i++) {
			if (Members[i]->Get_Owner() == Player) {
				return true;
			}
		}
		return false;
	}
	inline bool Is_Member(int ID) {
		for (int i = 0;i < Members.Count();i++) {
			if (Members[i]->Get_ID() == ID) {
				return true;
			}
		}
		return false;
	}
	inline bool Is_Member(GameObject *obj) {
		for (int i = 0;i < Members.Count();i++) {
			if (Members[i]->Get_GameObj() == obj) {
				return true;
			}
		}
		return false;
	}
	inline bool Is_Leader(cPlayer *Player) {
		return (Members.Count() && Members[0]->Get_Owner() == Player);
	}
	inline bool Is_Leader(int ID) {
		return (Members.Count() && Members[0]->Get_ID() == ID);
	}
	inline bool Is_Leader(GameObject *obj) {
		return (Members.Count() && Members[0]->Get_GameObj() == obj);
	}
	inline DASquadMemberClass *Find_Member(cPlayer *Player) {
		for (int i = 0;i < Members.Count();i++) {
			if (Members[i]->Get_Owner() == Player) {
				Members[i];
			}
		}
		return 0;
	}
	inline DASquadMemberClass *Find_Member(int ID) {
		for (int i = 0;i < Members.Count();i++) {
			if (Members[i]->Get_ID() == ID) {
				Members[i];
			}
		}
		return 0;
	}
	inline DASquadMemberClass *Find_Member(GameObject *obj) {
		for (int i = 0;i < Members.Count();i++) {
			if (Members[i]->Get_GameObj() == obj) {
				Members[i];
			}
		}
		return 0;
	}
	inline DASquadMemberClass *Get_Member(int Index) {
		return Members[Index];
	}
	inline DASquadMemberClass *Get_Leader() {
		return Members[0];
	}
	void Set_Leader(cPlayer *Player);

	inline int Get_Team() {
		return Get_Leader()->Get_Team();
	}
	void Set_Team(int team);
	void Check_Team();

	inline int Size() {
		return Members.Count();
	}
	bool Is_Full();
	inline void Destroy() {
		delete this;
	}
	~DASquadClass();
	
private:
	DynamicVectorClass<DASquadMemberClass*> Members;
	bool Disbanded;
};

class DA_API DASquadManagerClass : private DAEventClass, public DAGameFeatureClass {
private:
	virtual void Init();
	virtual ~DASquadManagerClass();
	virtual void Level_Loaded_Event();
	virtual void Settings_Loaded_Event();
	virtual void Player_Leave_Event(cPlayer *Player);
	virtual void Team_Change_Event(cPlayer *Player);
	virtual void Remix_Event();
	virtual void Rebalance_Event();
	virtual void Swap_Event();
	virtual void Think();

	bool List_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool Join_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool Invite_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool Accept_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool Decline_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);
	bool Leave_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType);

public:
	DASquadClass *Create_Squad(cPlayer *Leader);
	inline void Remove_Squad(DASquadClass *Squad) {
		SquadList.DeleteObj(Squad);
	}

	void Invite(cPlayer *Player,cPlayer *Leader);
	void Join_Accepted(int JoinIndex);
	void Invite_Accepted(int InviteIndex);

	bool Is_Join_Pending(cPlayer *Player,cPlayer *Leader = 0);
	bool Is_Invite_Pending(cPlayer *Player,cPlayer *Leader = 0);
	bool Is_WaitList_Pending(cPlayer *Player,cPlayer *Leader = 0);

	bool Remove_Join(cPlayer *Player,cPlayer *Leader = 0);
	bool Remove_Invite(cPlayer *Player,cPlayer *Leader = 0);
	bool Remove_WaitList(cPlayer *Player,cPlayer *Leader = 0);

	bool Clear_Joins(cPlayer *Player);
	bool Clear_Invites(cPlayer *Player);
	bool Clear_WaitList(cPlayer *Player);

	DASquadClass *Find_Squad(cPlayer *Player);
	DASquadClass *Find_Squad(int ID);
	DASquadClass *Find_Squad(GameObject *obj);

	inline DASquadClass *Get_Squad(int Index) {
		return SquadList[Index];
	}
	inline int Get_Squad_Count() {
		return SquadList.Count();
	}

	void Check_WaitList(cPlayer *Player);
	unsigned int Get_Max_Squad_Size();

private:
	DynamicVectorClass<DASquadInviteStruct> Invites;
	DynamicVectorClass<DASquadInviteStruct> Joins;
	DynamicVectorClass<DASquadInviteStruct> WaitList;
	DynamicVectorClass<DASquadClass*> SquadList;

	//Settings
	unsigned int MaxSquadSize;
	bool RemixSquads;
};

extern DA_API DAGameFeatureRegistrant<DASquadManagerClass> DASquadManagerClassRegistrant;
#define DASquadManager ((DASquadManagerClass*)DASquadManagerClassRegistrant.Get_Instance())

#endif