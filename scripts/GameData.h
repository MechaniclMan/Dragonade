/*	Renegade Scripts.dll
	Copyright 2014 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef TT_INCLUDE_GAMEDATA_H
#define TT_INCLUDE_GAMEDATA_H

#include "BaseControllerClass.h"
#include "cPlayer.h"
class cGameDataSkirmish;
class cGameDataCnc;
class cGameDataSinglePlayer;
class cPacket;
class WOLGameInfo;

template<typename A, typename B> class TypedEventPair {
public:
	TypedEventPair(A itemA, B itemB) : mItemA(itemA),mItemB(itemB)
	{
	}
	A GetItemA(void)
	{
		return mItemA;
	}
	B GetItemB(void)
	{
		return mItemB;
	}
protected:
	A mItemA;
	B mItemB;
};

template <class T> class Signaler {
public:
	Signaler *Forward; // 0004
	Signaler() : Forward(0)
	{
	}
	void SendSignal(T signal)
	{
		ReceiveSignal(signal);
	}
	virtual void ReceiveSignal(T &signal)
	{
	}
	virtual void SignalDropped(Signaler &sig)
	{
		Forward = 0;
	}
	virtual ~Signaler()
	{
		if (Forward)
			Forward->SignalDropped(*this);
		Forward = 0;
	}
	void SignalMe(Signaler &sig)
	{
		if (Forward != &sig)
		{
			if (Forward)
				Forward->SignalDropped(*this);
			Forward = &sig;
			sig.Forward = this;
		}
	}
}; // 0008



class Render2DSentenceClass;
class cGameData :
	public Signaler<TypedEventPair<bool, int>>
{
public:
	static REF_DECL(bool, IsManualExit);
	static REF_DECL(int, HostedGameNumber);
	static REF_DECL(WideStringClass, WinText);
	static REF_DECL(Render2DSentenceClass *, PTextRenderer);
	bool IsIntermission; // 0008
	bool IsDedicated; // 0009
	bool IsAutoRestart; // 000A
	bool IsFriendlyFirePermitted; // 000B
	bool IsTeamChangingAllowed; // 000C
	bool IsPassworded; // 000D
	bool GrantWeapons; // 000E
	bool IsLaddered; // 000F
	bool IsClanGame; // 0010
	bool UseLagReduction; // 0011
	bool RemixTeams; // 0012
	bool CanRepairBuildings; // 0013
	bool DriverIsAlwaysGunner; // 0014
	bool SpawnWeapons; // 0015
	bool DoMapsLoop; // 0016
	bool MapCycleOver; // 0017
	unsigned int MapNumber; // 0018
	WideStringClass GameTitle; // 001C
	WideStringClass Motd; // 0020
	WideStringClass Password; // 0024
	StringClass MapName; // 0028
	StringClass ModName; // 002C
	StringClass MapList[100]; // 0030
	WideStringClass Owner; // 01C0
	DynamicVectorClass<WideStringClass> BottomText; // 01C4
	DynamicVectorClass<WideStringClass> SettingsLimits; // 01DC
	StringClass SettingsDescription; // 01F4; this is a WideStringClass on LFDS
	int MaxPlayers; // 01F8
	unsigned int TimeLimit_Minutes; // 01FC
	unsigned int IntermissionTime_Seconds; // 0200
	unsigned int ExeKey; // 0204
	bool ExeVersionsMatch; // 0208
	bool StringVersionsMatch; // 0209
	unsigned int IP; // 020C
	unsigned short Port; // 0210
	StringClass INI; // 0214
	unsigned int RadarMode; // 0218
	unsigned int CFGModTime; // 021C
	int CurrentPlayers; // 0220
	float IntermissionTimeLeft; // 0224
	float TimeRemaining_Seconds; // 0228
	float MaxWorldDistance; // 022C
	unsigned int MinQualifyingTime_Minutes; // 0230
	int WinnerID; // 0234
	unsigned int WinType; // 0238
	SYSTEMTIME GameStartTime2; // 023C  023C
	unsigned int GameStartTime; // 024C  0254
	unsigned long FrameCount; // 0250  0258
	WideStringClass MVPName; // 0254  025C
	unsigned int MVPCount; // 0258  0260
	unsigned int GameDuration_Seconds; // 025C  0264
	bool IsQuickMatch; // 0260  0268
	unsigned long Clan1; // 0264  026C
	unsigned long Clan2; // 0268  0270
public:
	static REF_DECL(bool, IsManualRestart);
	virtual void ReceiveSignal(TypedEventPair<bool,int> &signal);
	virtual ~cGameData();
	virtual void On_Game_Begin();
	virtual void On_Game_End();
	virtual void Think();
	virtual void Render();
	virtual void Reset_Game(bool skipPlayerReset);
	virtual const wchar_t*  Get_Game_Name() = 0;
	virtual int Get_Game_Type() = 0;
	virtual int Choose_Player_Type(cPlayer *Player,int type,bool SkipAutoTeam);
	virtual void Load_From_Server_Config();
	virtual void Save_To_Server_Config();
	virtual bool Is_Editable_Teaming();
	virtual bool Is_Editable_Clan_Game();
	virtual bool Is_Editable_Friendly_Fire();
	virtual void Soldier_Added(GameObject *obj);
	virtual void Show_Game_Settings_Limits();
	virtual bool Is_Single_Player();
	virtual bool Is_Skirmish();
	virtual bool Is_Cnc();
	virtual cGameDataSinglePlayer *As_Single_Player();
	virtual cGameDataSkirmish *As_Skirmish();
	virtual cGameDataCnc *As_Cnc();
	virtual unsigned int Get_Min_Players();
	virtual bool Is_Game_Over();
	virtual bool Is_Limited();
	virtual bool Is_Valid_Settings(WideStringClass &w,bool b);
	virtual void Export_Tier_1_Data(cPacket &packet);
	virtual void Import_Tier_1_Data(cPacket &packet);
	virtual void Import_Tier_1_Data(const WOLGameInfo &packet);
	virtual void Export_Tier_2_Data(cPacket &packet);
	virtual void Import_Tier_2_Data(cPacket &packet);
	virtual bool Is_Gameplay_Permitted();
	virtual bool Remember_Inventory();
	virtual void Get_Description(WideStringClass &w);
	void Begin_Intermission();
	void Game_Over_Processing();
	bool Has_Config_File_Changed();
	static cGameData* Create_Game_Of_Type(int type);
	TT_DEPRECATED("Use the TT game manager instead.") void Rotate_Map();
	void Set_Game_Duration_S(uint32 uGameDuration);
	void Increment_Mvp_Count();
	void Set_Mvp_Count(uint32 uMvpCount);
	void Set_Mvp_Name(WideStringClass oMvpName);
	bool Set_Max_Players(int maxplayers);
	bool Is_Map_Valid(char** _fileName) const;
	const wchar_t* Get_Team_Word();
	void Filter_Spawners();
	void Swap_Team_Sides();
	void Remix_Team_Sides();
	void Rebalance_Team_Sides();
	int Choose_Available_Team(int type);
	int Choose_Smallest_Team();

	void Reset_Time_Remaining_Seconds() { TimeRemaining_Seconds = (float)(TimeLimit_Minutes * 60); }
	void Set_Time_Remaining_Seconds(float time) {TimeRemaining_Seconds = time;}
	void Set_Time_Limit_Minutes(unsigned int time) {TimeLimit_Minutes = time;}
	bool Is_Map_Cycle_Over() {return MapCycleOver;}
	int Get_Winner_ID() {return WinnerID;}
	static void Set_Win_Text(const WideStringClass& _winText) { WinText = _winText; }
	static bool Is_Manual_Exit() { return IsManualExit; }
	static void Set_Manual_Exit(bool b) {IsManualExit = b;}
	static bool Is_Manual_Restart() { return IsManualRestart; }
	static void Set_Manual_Restart(bool b) {IsManualRestart = b;}
	static void Onetime_Init();
	static void Onetime_Shutdown();
	void Set_Motd(const wchar_t *s) {Motd = s;}
	void Set_Map_Name(const StringClass &map) {MapName = map;}
	void Set_Mod_Name(const StringClass &mod);
	void Set_Time_Limit_Minutes(int time) {TimeLimit_Minutes = time;}
	void Set_Radar_Mode(int mode) {RadarMode = mode;}
	unsigned int Get_Config_File_Mod_Time();
	void Load_From_Server_Config(const char *);
	void Save_To_Server_Config(const char *);
	StringClass &Get_Map_Name() {return MapName;}
	WideStringClass &Get_MVP_Name() {return MVPName;}
	unsigned int Get_MVP_Count() {return MVPCount;}
	unsigned int Get_Time_Limit_Minutes() {return TimeLimit_Minutes;}
	float Get_Time_Remaining_Seconds() {return TimeRemaining_Seconds;}
	unsigned int Get_Radar_Mode() {return RadarMode;}
	int Get_Current_Players() {return CurrentPlayers;}
	bool Is_Friendly_Fire_Permitted() {return IsFriendlyFirePermitted;}
	unsigned int Get_Game_Duration_S() {return GameDuration_Seconds;}
	StringClass Get_Map_Cycle(int map) {return MapList[map];}
	void Set_Map_Cycle(int map,const StringClass &name) {MapList[map] = name;}
	StringClass &Get_Mod_Name() {return ModName;}
	unsigned int Get_Map_Number() {return MapNumber;}
	bool Is_Dedicated() {return IsDedicated;}
	int Get_Max_Players() {return MaxPlayers;}
	bool Is_Laddered() {return IsLaddered;}
	bool Is_Intermission() {return IsIntermission;}
	bool Is_Team_Changing_Allowed() {return IsTeamChangingAllowed;}
	static int Get_Hosted_Game_Number() {return HostedGameNumber;}
	static void Set_Hosted_Game_Number(int num) {HostedGameNumber = num;}
	void Set_Time_Rmaining_Seconds(float time) {TimeRemaining_Seconds = time;}
	void Set_Password(const wchar_t *pw) {Password = pw;}
	wchar_t *Get_Password() {return Password.Peek_Buffer();}
	unsigned int Get_Ip_Address() {return IP;}
	unsigned short Get_Port() {return Port;}
	void Set_Is_Intermission(bool b) {IsIntermission = b;}
	WideStringClass &Get_Motd() {return Motd;}
	float Get_Maximum_World_Distance() {return MaxWorldDistance;}
	bool Is_Passworded() {return IsPassworded;}
	float Get_Intermission_Time_Remaining() {return IntermissionTimeLeft;}
	bool Is_Clan_Game() {return IsClanGame;}
	void Set_Winner_ID(int id) {WinnerID = id;}
	void Set_Win_Type(unsigned int type) {WinType = type;}
	unsigned int Get_Win_Type() {return WinType;}
	void Set_Is_Map_Cycle_Over(bool b) {MapCycleOver = b;}
	void Set_MVP_Name(WideStringClass w) {MVPName = w;}
	void Set_MVP_Count(int count) {MVPCount = count;}
	void Set_QuickMatch_Server(bool b) {IsQuickMatch = b;}
	WideStringClass &Get_Owner() {return Owner;}
	WideStringClass &Get_Game_Title() {return GameTitle;}
	uint32 Get_Version_Number() {return ExeKey;}
	bool Do_Exe_Versions_Match() {return ExeVersionsMatch;}
	bool Do_String_Versions_Match() {return StringVersionsMatch;}
	void Set_Is_Passworded(bool b) {IsPassworded = b;}
	void Set_Game_Title(const wchar_t * w) {GameTitle = w;}
	bool Is_Remix_Teams() {return RemixTeams;}
	bool Is_Auto_Restart() {return IsAutoRestart;}
	bool Is_QuickMatch_Server() {return IsQuickMatch;}
	bool Do_Maps_Loop() {return DoMapsLoop;}
	void Set_Do_Maps_Loop(bool b) {DoMapsLoop = b;}
	void Clear_Map_Cycle();
	bool Can_Repair_Buildings() {return CanRepairBuildings;}
	bool Driver_Is_Always_Gunner() {return DriverIsAlwaysGunner;}
	bool Spawn_Weapons() {return SpawnWeapons;}
	void Set_Maximum_World_Distance(float distance);
	void Set_Ini_Filename(StringClass const &ini)
	{
		INI = ini;
	}
	StringClass Get_Settings_Description()
	{
		return SettingsDescription;
	}
	void Set_Settings_Description(StringClass w)
	{
		SettingsDescription = w;
	}
	static int Get_Mission_Number_From_Map_Name(const char *mapname)
	{
		int mission = 90;
		if (_strnicmp(mapname,"M00_T",5))
		{
			if (strlen(mapname) == 1)
			{
				mission = 0;
			}
			else
			{
				mission = atoi(&(mapname[1]));
			}
		}
		return mission;
	}
	bool Is_Valid_Player_Type(int playertype)
	{
		return Is_Single_Player() || playertype != -1;
	}
	int Get_MinQualifyingTime_Minutes()
	{
		return MinQualifyingTime_Minutes;
	}
	bool Does_Map_Exist()
	{
		return !ModName.Is_Empty() || !MapName.Is_Empty();
	}
	StringClass getMapBaseName()
	{
		if (MapName[MapName.Get_Length() - 4] != '.')
			return MapName;
		else
		{
			StringClass baseName = MapName;
			baseName.TruncateRight(4);
			return baseName;
		}
	}
	void setCurrentPlayers(int value) { CurrentPlayers = value; }
}; // 026C  0274

class cGameDataCnc : public cGameData {
public:
	bool BaseDestructionEndsGame;
	bool BeaconPlacementEndsGame;
	unsigned int StartingCredits;
	BaseControllerClass GDI;
	BaseControllerClass Nod;
	bool BasesInitalized;
	~cGameDataCnc();
	void On_Game_Begin();
	void On_Game_End();
	void Think();
	void Reset_Game();
	const wchar_t*  Get_Game_Name();
	int Get_Game_Type();
	void Load_From_Server_Config();
	void Save_To_Server_Config();
	bool Is_Editable_Teaming();
	bool Is_Editable_Clan_Game();
	bool Is_Editable_Friendly_Fire();
	void Soldier_Added(GameObject *obj);
	void Show_Game_Settings_Limits();
	bool Is_Cnc();
	cGameDataCnc *As_Cnc();
	bool Is_Game_Over();
	bool Is_Limited();
	bool Is_Valid_Settings(WideStringClass &w,bool b);
	void Export_Tier_2_Data(cPacket &packet);
	void Import_Tier_2_Data(cPacket &packet);
	bool Is_Gameplay_Permitted();
	void Get_Description(WideStringClass &w);
	void Base_Destruction_Score_Tweaking();
};

class cGameDataSkirmish : public cGameData {
public:
	bool BaseDestructionEndsGame;
	bool BeaconPlacementEndsGame;
	unsigned int StartingCredits;
	BaseControllerClass GDI;
	BaseControllerClass Nod;
	bool BasesInitalized;
	~cGameDataSkirmish();
	void On_Game_Begin();
	void On_Game_End();
	void Think();
	void Reset_Game();
	const wchar_t* Get_Game_Name();
	int Get_Game_Type();
	void Load_From_Server_Config();
	void Save_To_Server_Config();
	bool Is_Editable_Teaming();
	bool Is_Editable_Clan_Game();
	bool Is_Editable_Friendly_Fire();
	void Soldier_Added(GameObject *obj);
	void Show_Game_Settings_Limits();
	bool Is_Skirmish();
	cGameDataSkirmish *As_Skirmish();
	bool Is_Game_Over();
	bool Is_Limited();
	void Get_Description(WideStringClass &w);
};

class cGameDataSinglePlayer : public cGameData {
public:
	~cGameDataSinglePlayer();
	const wchar_t* Get_Game_Name();
	int Get_Game_Type();
	int Choose_Player_Type(cPlayer *Player,int type,bool SkipAutoTeam);
	bool Is_Single_Player();
	cGameDataSinglePlayer *As_Single_Player();
	bool Is_Limited();
	bool Remember_Inventory();
};

#endif