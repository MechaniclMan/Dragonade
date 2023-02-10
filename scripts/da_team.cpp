/*	Renegade Scripts.dll
    Dragonade Team Manager
	Copyright 2012 Whitedragon, Tiberian Technologies

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
#include "da_team.h"
#include "da_settings.h"
#include "da_hook.h"

unsigned int DATeamManager::RebalanceTime = 10;
int DATeamManager::RemixFrequency = 1;
int DATeamManager::SwapChance = 50;
int DATeamManager::RemixCount = 0;
bool DATeamManager::EnableRebalance = true;
bool DATeamManager::EnableFreeTeamChanging = false;
int DATeamManager::ForceTeam = -1;

class RemixConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "remix"; }
	const char *Get_Help() { return "REMIX - Remix teams."; }
	void Activate(const char *ArgumentsString) {
		DATeamManager::Remix();
	}
};

class RebalanceConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "rebalance"; }
	const char *Get_Help() { return "REBALANCE - Rebalance teams."; }
	void Activate(const char *ArgumentsString) {
		DATeamManager::Rebalance();
	}
};

class SwapConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "swap"; }
	const char *Get_Help() { return "SWAP - Swap teams."; }
	void Activate(const char *ArgumentsString) {
		DATeamManager::Swap();
	}
};

class ForceTeamConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "forceteam"; }
	const char *Get_Help() { return "FORCETEAM <team> - Force all players to switch to the given team."; }
	void Activate(const char *ArgumentsString) {
		DATeamManager::Set_Force_Team(atoi(ArgumentsString));
	}
};

class Team3ConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "team3"; }
	const char *Get_Help() { return "TEAM3 <playerid> - Swap a player's team, allowing them to keep their score, kills, deaths, and starting credits. Any credits over the starting amount are distributed to their team."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Change_Team_3(Player,Player->Get_Player_Type()?0:1);
		}
	}
};

class Team4ConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "team4"; }
	const char *Get_Help() { return "TEAM4 <playerid> - Swap a player's team, allowing them to keep their score, kills, deaths, and reseting their credits to the starting amount."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Change_Team_4(Player,Player->Get_Player_Type()?0:1);
		}
	}
};

class Team5ConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "team5"; }
	const char *Get_Help() { return "TEAM5 <playerid> - Swap a player's team, reseting their score, kills and deaths to 0, and their credits to the starting amount."; }
	void Activate(const char *ArgumentsString) {
		cPlayer *Player = Find_Player(atoi(ArgumentsString));
		if (Player) {
			Change_Team_5(Player,Player->Get_Player_Type()?0:1);
		}
	}
};

void DATeamManager::Init() {
	static DATeamManager Instance;
	Instance.Register_Event(DAEvent::SETTINGSLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::LEVELLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::PLAYERJOIN,INT_MAX);
	Instance.Register_Event(DAEvent::TEAMCHANGEREQUEST,INT_MIN);

	ConsoleFunctionList.Add(new RemixConsoleCommand);
	ConsoleFunctionList.Add(new RebalanceConsoleCommand);
	ConsoleFunctionList.Add(new SwapConsoleCommand);
	ConsoleFunctionList.Add(new ForceTeamConsoleCommand);
	ConsoleFunctionList.Add(new Team3ConsoleCommand);
	ConsoleFunctionList.Add(new Team4ConsoleCommand);
	ConsoleFunctionList.Add(new Team5ConsoleCommand);
	Sort_Function_List();
	Verbose_Help_File();
}

void DATeamManager::Settings_Loaded_Event() {	
	RebalanceTime = (unsigned int)DASettingsManager::Get_Int("RebalanceTime",10);
	RemixFrequency = DASettingsManager::Get_Int("RemixFrequency",1);
	SwapChance = DASettingsManager::Get_Int("SwapChance",50);
	EnableFreeTeamChanging = DASettingsManager::Get_Bool("EnableFreeTeamChanging",false);
	EnableRebalance = DASettingsManager::Get_Bool("EnableRebalance",true);
	if (EnableRebalance) {
		Register_Event(DAEvent::PLAYERLEAVE,INT_MAX);
	}
	else {
		Unregister_Event(DAEvent::PLAYERLEAVE);
	}
}

void DATeamManager::Level_Loaded_Event() {
	RemixCount++;

	if (ForceTeam != -1) {
		Set_Force_Team(ForceTeam);
	}
	else {
		if (RemixFrequency && RemixCount >= RemixFrequency) {
			Remix();
			RemixCount = 0;
		}
		else if (Commands->Get_Random_Int(1,101) <= SwapChance) {
			Swap();
		}
		if (EnableRebalance) {
			Rebalance();
		}
	}
}

void DATeamManager::Player_Join_Event(cPlayer *Player) {
	if (ForceTeam != -1) {
		Player->Set_Player_Type(ForceTeam);
	}
	else if (Player->SessionTime < 10.0f) {
		if (!EnableFreeTeamChanging) {
			if (Tally_Team_Size(0)-Tally_Team_Size(1) > 1) { //Don't let a new player joining imbalance the teams.
				Player->Set_Player_Type(1);
			}
			else if (Tally_Team_Size(1)-Tally_Team_Size(0) > 1) {
				Player->Set_Player_Type(0);
			}
		}
	}
	if (The_Game()->IsTeamChangingAllowed) { //Disable team changing when a new player joins so it uses the "fighting for team x" join message.
		The_Game()->IsTeamChangingAllowed = false;
		if (The_Game()->Is_Laddered()) {
			The_Game()->IsLaddered = false; //Having team changing and laddered both enabled causes stock clients to crash.
			Update_Game_Settings();
			The_Game()->IsLaddered = true;
		}
		else {
			Update_Game_Settings();
		}
		Stop_Timer(2);
		Start_Timer(2,0.1f);
	}
}

void DATeamManager::Player_Leave_Event(cPlayer *Player) {
	if (The_Game()->Get_Game_Duration_S() <= RebalanceTime) {
		int Team = Player->Get_Team();
		if (Team == 0 || Team == 1) {
			int OtherTeam = Team?0:1;
			if (Tally_Team_Size(OtherTeam)-(Tally_Team_Size(Team)-1) > 1) {
				Stop_Timer(1);
				Start_Timer(1,10.0f);
			}
		}
	}
}

bool DATeamManager::Team_Change_Request_Event(cPlayer *Player) {
	if (Is_Free_Team_Changing_Enabled()) {
		Change_Team_3(Player,Player->Get_Team()?0:1);
		Send_Client_Text(WideStringFormat(L"%ls changed teams.",Player->Get_Name()),TEXT_MESSAGE_PUBLIC,false,-1,-1,true,true);
		return false;
	}
	return true;
}

void DATeamManager::Timer_Expired(int Number,unsigned int Data) {
	if (Number == 1) {
		Rebalance();
	}
	else if (Number == 2) {
		The_Game()->IsTeamChangingAllowed = true;
		Update_Game_Settings();
	}
}

void DATeamManager::Set_Force_Team(int Team) {
	if (Team == 0 || Team == 1) {
		ForceTeam = Team;
		for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
			Change_Team_4(z->Data(),Team);
		}
	}
	else {
		ForceTeam = -1;
		Remix();
	}
}

void DATeamManager::Remix() {
	if (ForceTeam != -1) {
		return;
	}

	DA::Host_Message("Teams have been remixed.");

	bool Winner[128];

	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Is_Active()) {
			if (Player->Get_Player_Type() == The_Game()->Get_Winner_ID()) {
				Winner[Player->Get_Id()] = true;
			}
			else {
				Winner[Player->Get_Id()] = false;
			}
			Player->Set_Player_Type(2); //Change team to 2 to mark for remixing.
		}
	}
	int TeamCount[2] = {0,0};

	DAEventManager::Remix_Event();

	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Is_Active()) {
			if (Player->Get_Player_Type() == 0) {
				TeamCount[0]++;
			}
			else if (Player->Get_Player_Type() == 1) {
				TeamCount[1]++;
			}
		}
	}

	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) { //Team winners first so that each team will have half the winners...
		cPlayer *Player = z->Data();
		if (Player->Is_Active()) {
			if (Player->Get_Player_Type() == 2 && Winner[Player->Get_Id()]) { //Only remix player if another event hasn't already handled them.
				int RandTeam = Get_Random_Int(1,101) > 50?1:0;
				if (TeamCount[RandTeam] <= TeamCount[RandTeam?0:1]) {
					Change_Team_4(Player,RandTeam);
					TeamCount[RandTeam]++;
				}
				else {
					Change_Team_4(Player,RandTeam?0:1);
					TeamCount[RandTeam?0:1]++;
				}
			}
		}
	}
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) { //And half the losers.
		cPlayer *Player = z->Data();
		if (Player->Is_Active()) {
			if (Player->Get_Player_Type() == 2 && !Winner[Player->Get_Id()]) {
				int RandTeam = Get_Random_Int(1,101) > 50?1:0;
				if (TeamCount[RandTeam] <= TeamCount[RandTeam?0:1]) {
					Change_Team_4(Player,RandTeam);
					TeamCount[RandTeam]++;
				}
				else {
					Change_Team_4(Player,RandTeam?0:1);
					TeamCount[RandTeam?0:1]++;
				}
			}
		}
	}
}

void DATeamManager::Rebalance() {
	if (ForceTeam != -1) {
		return;
	}

	int TeamCount[2];
	TeamCount[0] = Tally_Team_Size(0);
	TeamCount[1] = Tally_Team_Size(1);

	if (Diff(TeamCount[0],TeamCount[1]) > 1) {
		DA::Host_Message("Teams have been rebalanced.");

		DAEventManager::Rebalance_Event();

		TeamCount[0] = Tally_Team_Size(0);
		TeamCount[1] = Tally_Team_Size(1);

		if (Diff(TeamCount[0],TeamCount[1]) > 1) {

			int OldTeam = -1;
			int NewTeam = -1;
			if (TeamCount[0] > TeamCount[1]) {
				OldTeam = 0;
				NewTeam = 1;
			}
			else {
				OldTeam = 1;
				NewTeam = 0;
			}
			for (SLNode<cPlayer> *z = Get_Player_List()->Head();z && TeamCount[OldTeam]-TeamCount[NewTeam] > 1;z = z->Next()) {
				cPlayer *Player = z->Data();
				if (Player->Is_Active()) {
					if (Player->Get_Player_Type() == OldTeam) {
						Change_Team_4(Player,NewTeam);
						TeamCount[OldTeam]--;
						TeamCount[NewTeam]++;
					}
				}
			}
		}
	}
}

void DATeamManager::Swap() {
	if (ForceTeam != -1) {
		return;
	}

	DA::Host_Message("Teams have been swapped.");

	int TeamSave[128];
	
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Is_Active()) {
			TeamSave[Player->Get_Id()] = Player->Get_Player_Type(); //Save old team then change everyone to 2 to mark them for swapping.
			Player->Set_Player_Type(2);
		}
	}

	DAEventManager::Swap_Event();

	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
		cPlayer *Player = z->Data();
		if (Player->Is_Active() && Player->Get_Player_Type() == 2) { //Only swap if some other event hasn't handled this player already.
			Change_Team_4(Player,TeamSave[Player->Get_Id()]?0:1);
		}
	}
}
