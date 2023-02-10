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

#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "engine_DA.h"
#include "da.h"
#include "da_squad.h"
#include "da_settings.h"
#include "da_translation.h"

bool DASquadMemberClass::Is_Leader() {
	return Squad->Get_Leader() == this;
}

void DASquadMemberClass::Init() {
	Register_Chat_Command((DAPOCC)&DASquadMemberClass::Disband_Chat_Command,"!disband");
	Register_Chat_Command((DAPOCC)&DASquadMemberClass::Promote_Chat_Command,"!promote",1);
	Register_Chat_Command((DAPOCC)&DASquadMemberClass::Info_Chat_Command,"!sinfo");
	Register_Chat_Command((DAPOCC)&DASquadMemberClass::SKick_Chat_Command,"!skick",1);
	Register_Chat_Command((DAPOCC)&DASquadMemberClass::Leave_Chat_Command,"!leave");
	Register_Chat_Command((DAPOCC)&DASquadMemberClass::Msg_Chat_Command,"!sm|!ms|!sc|!cs",1);
	Name_Change();
}

void DASquadMemberClass::Name_Change() {
	/*WideStringClass NameSave = Get_Name();
	Get_Owner()->PlayerName = "#" + NameSave;
	for (int i = 0;i < Squad->Size();i++) {
		cPlayer *Member = Squad->Get_Member(i)->Get_Owner();
		Get_Owner()->Set_Object_Dirty_Bit(Member->Get_Id(),NetworkObjectClass::BIT_CREATION);
		//Send_Object_Update(Get_Owner(),Member->Get_Id()); //Send new member's updated name to squadmates

		if (Member != Get_Owner()) {
			WideStringClass NameSave2 = Member->Get_Name();
			Member->PlayerName = "#" + Member->PlayerName;
			Member->Set_Object_Dirty_Bit(Get_ID(),NetworkObjectClass::BIT_CREATION);
			//Send_Object_Update(Member,Get_ID()); //Send updated squadmate's names to new member
			Update_Network_Object(Member);
			Member->PlayerName = NameSave2;
		}
	}
	Update_Network_Object(Get_Owner());
	Get_Owner()->PlayerName = NameSave; //Restore old name on server*/
}

DASquadMemberClass::~DASquadMemberClass() {
	for (int i = 0;i < Squad->Size();i++) {
		cPlayer *Member = Squad->Get_Member(i)->Get_Owner();

		Reset_Radar(Member->Get_GameObj()); //Reset radar for other players
		//Get_Owner()->Set_Object_Dirty_Bit(Member->Get_ID(),NetworkObjectClass::BIT_CREATION); //Reset name for other players

		if (Get_Owner()->Is_Active()) {
			Squad->Get_Member(i)->Reset_Radar(Get_GameObj()); //Reset other player's radar
			//Member->Set_Object_Dirty_Bit(Get_ID(),NetworkObjectClass::BIT_CREATION); //Reset other player's names
		}
	}
	Squad->Internal_Remove(this); //Remove this observer from the squad's list of members
}

void DASquadMemberClass::Team_Change() {
	if (Get_Team() != Squad->Get_Team()) {
		Leave_Squad(); //Remove from squad if changing teams
	}
}

void DASquadMemberClass::Created() {
	Start_Timer(2,0.5f);
}

void DASquadMemberClass::Player_Loaded() {
	Start_Timer(1,0.5f);
}

void DASquadMemberClass::Timer_Expired(int Number,unsigned int Data) {
	if (Number == 1) {
		for (int i = 0;i < Squad->Size();i++) {
			Squad->Get_Member(i)->Update_Radar(Get_GameObj());
		}
	}
	else if (Number == 2) {
		Update_Radar();
	}
}

void DASquadMemberClass::Vehicle_Enter(VehicleGameObj *Vehicle,int Seat) {
	if (Seat == 0) {
		Set_Radar_Vehicle_Driver();
	}
	else {
		Set_Radar_Vehicle_Passenger();
	}
}

void DASquadMemberClass::Vehicle_Exit(VehicleGameObj *Vehicle,int Seat) {
	Set_Radar_Soldier();
}

void DASquadMemberClass::Character_Purchase(float Cost,const SoldierGameObjDef *Item) {
	Squad->Squad_Message_Except(Get_Owner(),"Purchase: %ls - %s",Get_Name(),DATranslationManager::Translate(Item));
}

void DASquadMemberClass::Vehicle_Purchase(float Cost,const VehicleGameObjDef *Item) {
	Squad->Squad_Message_Except(Get_Owner(),"Purchase: %ls - %s",Get_Name(),DATranslationManager::Translate(Item));
}

void DASquadMemberClass::PowerUp_Purchase(float Cost,const PowerUpGameObjDef *Item) {
	Squad->Squad_Message_Except(Get_Owner(),"Purchase: %ls - %s",Get_Name(),DATranslationManager::Translate(Item));
}

bool DASquadMemberClass::Disband_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType) {
	if (!Is_Leader()) {
		DA::Private_Color_Message(Get_ID(),SQUADCOLOR,"You must be the squad leader to use this command.");
	}
	else {
		Squad->Disband();
	}
	return true;
}

bool DASquadMemberClass::Promote_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType) {
	if (!Is_Leader()) {
		DA::Private_Color_Message(Get_ID(),SQUADCOLOR,"You must be the squad leader to use this command.");
	}
	else {
		cPlayer *MatchPlayer = Match_Player(Get_Owner(),Text[1],false);
		if (MatchPlayer) {
			if (!Squad->Is_Member(MatchPlayer)) {
				DA::Private_Color_Message(Get_ID(),SQUADCOLOR,"%ls is not in your squad.",MatchPlayer->Get_Name());
			}
			else {
				Squad->Set_Leader(MatchPlayer);
			}
		}
	}
	return true;
}

bool DASquadMemberClass::Info_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType) {
	StringClass Str = "Leader: " + StringClass(Squad->Get_Leader()->Get_Name()) + "(" + DATranslationManager::Translate_Soldier(Squad->Get_Leader()->Get_GameObj()) + ")";
	Str += " - Members: ";
	for (int i = 1;i < Squad->Size();i++) {
		if (i > 1) {
			Str += ", " + StringClass(Squad->Get_Member(i)->Get_Name()) + "(" + DATranslationManager::Translate_Soldier(Squad->Get_Member(i)->Get_GameObj()) + ")";
		}
		else {
			Str += StringClass(Squad->Get_Member(i)->Get_Name()) + "(" + DATranslationManager::Translate_Soldier(Squad->Get_Member(i)->Get_GameObj()) + ")";
		}
	}
	DA::Private_Color_Message(Get_ID(),SQUADCOLOR,"%s",Str);
	return true;
}

bool DASquadMemberClass::SKick_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType) {
	if (!Is_Leader()) {
		DA::Private_Color_Message(Get_ID(),SQUADCOLOR,"You must be the squad leader to use this command.");
	}
	else {
		for (int i = 1;i <= Text.Size();i++) {
			cPlayer *MatchPlayer = Match_Player(Get_Owner(),Text[i],false);
			if (MatchPlayer) {
				if (Squad->Remove(MatchPlayer)) {
					DA::Private_Color_Message(MatchPlayer,SQUADCOLOR,"%ls has removed you from the squad.",Get_Name());
					Squad->Squad_Message_Except(MatchPlayer,"%ls was removed from the squad.",MatchPlayer->Get_Name());
				}
				else if (DASquadManager->Remove_WaitList(MatchPlayer,Get_Owner())) {
					DA::Private_Color_Message(MatchPlayer,SQUADCOLOR,"%ls has removed you from the wait list.",Get_Name());
					Squad->Leader_Message("%ls was removed from the wait list.",MatchPlayer->Get_Name());
				}
				else if (DASquadManager->Remove_Invite(MatchPlayer,Get_Owner())) {
					DA::Private_Color_Message(MatchPlayer,SQUADCOLOR,"%ls has rescinded your squad invitation.",Get_Name());
					Squad->Leader_Message("%ls squad invitation was rescinded.",Make_Possessive(MatchPlayer->Get_Name()));
				}
			}
		}
	}
	return true;
}

bool DASquadMemberClass::Leave_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType) {
	Leave_Squad();
	return true;
}

bool DASquadMemberClass::Msg_Chat_Command(const DATokenClass &Text,TextMessageEnum ChatType) {
	Squad->Squad_Message("%ls: %s",Get_Name(),Text[0]);
	return false;
}

void DASquadMemberClass::Update_Radar(GameObject *Member) {
	GameObject *Veh = Get_Vehicle(Get_GameObj());
	if (Veh) {
		if (Get_GameObj() == Get_Vehicle_Occupant(Veh,0)) {
			Set_Radar_Vehicle_Driver(Member);

		}
		else {
			Set_Radar_Vehicle_Passenger(Member);
		}
	}
	else {
		Set_Radar_Soldier(Member);
	}
}

void DASquadMemberClass::Set_Radar_Soldier(GameObject *Member) {
	if (Member) {
		Set_Obj_Radar_Blip_Shape_Player(Member,Get_GameObj(),RADAR_BLIP_SHAPE_OBJECTIVE);
		Set_Obj_Radar_Blip_Color_Player(Member,Get_GameObj(),RADAR_BLIP_COLOR_PRIMARY_OBJECTIVE);
	}
	else {
		for (int i = 0;i < Squad->Size();i++) {
			Member = Squad->Get_Member(i)->Get_GameObj();
			Set_Obj_Radar_Blip_Shape_Player(Member,Get_GameObj(),RADAR_BLIP_SHAPE_OBJECTIVE);
			Set_Obj_Radar_Blip_Color_Player(Member,Get_GameObj(),RADAR_BLIP_COLOR_PRIMARY_OBJECTIVE);
		}
	}
}

void DASquadMemberClass::Set_Radar_Vehicle_Driver(GameObject *Member) {
	if (Member) {
		Set_Obj_Radar_Blip_Shape_Player(Member,Get_GameObj(),RADAR_BLIP_SHAPE_OBJECTIVE);
		Set_Obj_Radar_Blip_Color_Player(Member,Get_GameObj(),RADAR_BLIP_COLOR_SECONDARY_OBJECTIVE);
	}
	else {
		for (int i = 0;i < Squad->Size();i++) {
			Member = Squad->Get_Member(i)->Get_GameObj();
			Set_Obj_Radar_Blip_Shape_Player(Member,Get_GameObj(),RADAR_BLIP_SHAPE_OBJECTIVE);
			Set_Obj_Radar_Blip_Color_Player(Member,Get_GameObj(),RADAR_BLIP_COLOR_SECONDARY_OBJECTIVE);
		}
	}
}

void DASquadMemberClass::Set_Radar_Vehicle_Passenger(GameObject *Member) {
	if (Member) {
		Set_Obj_Radar_Blip_Shape_Player(Member,Get_GameObj(),RADAR_BLIP_SHAPE_OBJECTIVE);
		Set_Obj_Radar_Blip_Color_Player(Member,Get_GameObj(),RADAR_BLIP_COLOR_RENEGADE);
	}
	else {
		for (int i = 0;i < Squad->Size();i++) {
			Member = Squad->Get_Member(i)->Get_GameObj();
			Set_Obj_Radar_Blip_Shape_Player(Member,Get_GameObj(),RADAR_BLIP_SHAPE_OBJECTIVE);
			Set_Obj_Radar_Blip_Color_Player(Member,Get_GameObj(),RADAR_BLIP_COLOR_RENEGADE);
		}
	}
}

void DASquadMemberClass::Reset_Radar(GameObject *Member) {
	Set_Obj_Radar_Blip_Shape_Player(Member,Get_GameObj(),RADAR_BLIP_SHAPE_HUMAN);
	Set_Obj_Radar_Blip_Color_Player(Member,Get_GameObj(),Get_Team());
}




DASquadClass::DASquadClass(cPlayer *Player) {
	Disbanded = false;
	if (!Player->Get_DA_Player()->Find_Observer("DASquadMemberClass")) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"You have created a squad.");
		DASquadMemberClass *NewMember = new DASquadMemberClass(this);
		Members.Add(NewMember);
		Player->Get_DA_Player()->Add_Observer(NewMember);
	}
}

bool DASquadClass::Add(cPlayer *Player) {
	if (!Is_Full() && (Player->Get_Team() == Get_Team() || Player->Get_Team() == 2) && !Player->Get_DA_Player()->Find_Observer("DASquadMemberClass")) {
		Squad_Message("%ls has joined the squad.",Player->Get_Name(),Player->Get_ID());
		DA::Private_Color_Message(Player,SQUADCOLOR,"You have joined %ls squad.",Make_Possessive(Get_Leader()->Get_Name()));
		DASquadMemberClass *NewMember = new DASquadMemberClass(this);
		Members.Add(NewMember);
		Player->Get_DA_Player()->Add_Observer(NewMember);
		return true;
	}
	return false;
}

bool DASquadClass::Remove(cPlayer *Player) {
	for (int i = 0;i < Members.Count();i++) {
		if (Members[i]->Get_Owner() == Player) {
			Members[i]->Leave_Squad();
			return true;
		}
	}
	return false;
}

void DASquadClass::Internal_Remove(DASquadMemberClass *Member) {
	if (!Disbanded) {
		for (int i = 0;i < Members.Count();i++) {
			if (Members[i] == Member) {
				Members.Delete(i);
				Squad_Message("%ls has left the squad.",Member->Get_Name());
				if (Member->Get_Owner()->Is_Active()) {
					DA::Private_Color_Message(Member->Get_ID(),SQUADCOLOR,"You have left the squad.");
				}
				if (i == 0 && Members.Count() > 1) {
					Squad_Message("%ls is now the squad leader.",Members[0]->Get_Name());
				}
				break;
			}
		}
		if (Size() == 1) {
			Disband();
		}
	}
	else {
		for (int i = 0;i < Members.Count();i++) {
			if (Members[i] == Member) {
				Members.Delete(i);
			}
		}
	}
	if (!Size()) {
		Destroy();
	}
}

void DASquadClass::Disband() {
	Squad_Message("The squad has been disbanded.");
	for (int i = 0;i < Members.Count();i++) {
		Members[i]->Leave_Squad();
	}
	Disbanded = true;
}

void DASquadClass::Squad_Message(const char *Format,...) {
	char Message[512];
	Format_String(Message);
	for (int i = 0;i < Members.Count();i++) {
		if (Members[i]->Get_Owner()->Is_Active()) {
			DA::Private_Color_Message(Members[i]->Get_ID(),SQUADCOLOR,"%s",Message);
		}
	}
}

void DASquadClass::Squad_Message_Except(cPlayer *Player,const char *Format,...) {
	char Message[512];
	Format_String(Message);
	for (int i = 0;i < Members.Count();i++) {
		if (Members[i]->Get_Owner() != Player && Members[i]->Get_Owner()->Is_Active()) {
			DA::Private_Color_Message(Members[i]->Get_ID(),SQUADCOLOR,"%s",Message);
		}
	}
}

void DASquadClass::Leader_Message(const char *Format,...) {
	if (Get_Leader()->Get_Owner()->Is_Active()) {
		char Message[512];
		Format_String(Message);
		DA::Private_Color_Message(Get_Leader()->Get_ID(),SQUADCOLOR,"%s",Message);
	}
}

void DASquadClass::Set_Leader(cPlayer *Player) {
	for (int i = 0;i < Members.Count();i++) {
		if (Members[i]->Get_Owner() == Player) {
			DASquadMemberClass *Temp = Members[i];
			Members.Delete(i);
			Members.Add_Head(Temp);
			Squad_Message("%ls has been promoted to squad leader.",Temp->Get_Name());
			break;
		}
	}
}

void DASquadClass::Set_Team(int Team) {
	for (int i = 0;i < Members.Count();i++) {
		Change_Team_4(Members[i]->Get_Owner(),Team);
	}
}

void DASquadClass::Check_Team() {
	int Team = Get_Team();
	for (int i = 1;i < Size();i++) {
		if (Members[i]->Get_Team() != Team) {
			Members[i]->Leave_Squad();
		}
	}
}

bool DASquadClass::Is_Full() {
	if ((unsigned int)Members.Count() >= DASquadManager->Get_Max_Squad_Size()) {
		return true;
	}
	return false;
}

DASquadClass::~DASquadClass() {
	if (DASquadManager) {
		DASquadManager->Remove_Squad(this);
	}
}



void DASquadManagerClass::Init() {
	Register_Event(DAEvent::LEVELLOADED);
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Event(DAEvent::REMIX);
	Register_Event(DAEvent::REBALANCE);
	Register_Event(DAEvent::SWAP);
	Register_Event(DAEvent::TEAMCHANGE);
	Register_Event(DAEvent::THINK);
	Register_Chat_Command((DAECC)&DASquadManagerClass::List_Chat_Command,"!squads|!squad");
	Register_Chat_Command((DAECC)&DASquadManagerClass::Join_Chat_Command,"!join",1);
	Register_Chat_Command((DAECC)&DASquadManagerClass::Invite_Chat_Command,"!invite",1);
	Register_Chat_Command((DAECC)&DASquadManagerClass::Accept_Chat_Command,"!accept");
	Register_Chat_Command((DAECC)&DASquadManagerClass::Decline_Chat_Command,"!decline");
	Register_Chat_Command((DAECC)&DASquadManagerClass::Leave_Chat_Command,"!leave");
}

DASquadManagerClass::~DASquadManagerClass() {
	for (int i = 0;i < SquadList.Count();i++) {
		SquadList[i]->Disband();
	}
}

void DASquadManagerClass::Level_Loaded_Event() {
	if (!RemixSquads) {
		for (int i = 0;i < SquadList.Count();i++) {
			SquadList[i]->Check_Team();
		}
	}
}

void DASquadManagerClass::Settings_Loaded_Event() {
	RemixSquads = DASettingsManager::Get_Bool("RemixSquads",true);
	MaxSquadSize = (unsigned int)DASettingsManager::Get_Int("MaxSquadSize",5);
}

void DASquadManagerClass::Player_Leave_Event(cPlayer *Player) {
	Remove_Join(Player);
	Remove_Invite(Player);
	Remove_WaitList(Player);
	Clear_Joins(Player);
	Clear_Invites(Player);
	Clear_WaitList(Player);
}

void DASquadManagerClass::Team_Change_Event(cPlayer *Player) {
	if (RemixSquads) {
		Check_WaitList(Player);
	}
	else {
		Remove_Join(Player);
		Remove_Invite(Player);
		Remove_WaitList(Player);
		Clear_Joins(Player);
		Clear_Invites(Player);
		Clear_WaitList(Player);
	}
}

void DASquadManagerClass::Remix_Event() {
	if (RemixSquads) {
		for (int i = 0;i < WaitList.Count();i++) {
			DASquadClass *Squad = Find_Squad(WaitList[i].Leader);
			if (!Squad) {
				DASquadClass *NewSquad = Create_Squad(WaitList[i].Leader);
				NewSquad->Add(WaitList[i].Player);
			}
			else if (Squad->Is_Leader(WaitList[i].Leader)) {
				Squad->Add(WaitList[i].Player);
			}
			WaitList.Delete(i);
			i--;
		}
	}
	
	for (int i = 0;i < SquadList.Count();i++) {
		DASquadClass *Squad = SquadList[i];
		for (int i = Squad->Size()-Get_Max_Squad_Size();i > 0;i--) {
			DA::Private_Color_Message(Squad->Get_Member(Squad->Size()-i)->Get_ID(),SQUADCOLOR,"You have been removed from your squad to balance the teams.");
			Squad->Get_Member(Squad->Size()-i)->Leave_Squad();
		}
	}

	if (RemixSquads) {
		if (SquadList.Count() == 1) { //If theres only one squad just put it on a random team.
			int RandTeam = Get_Random_Bool();
			SquadList[0]->Set_Team(RandTeam);	
		}
		else if (SquadList.Count() == 2) { //If there are two squads they go on opposite teams.
			int RandTeam = Get_Random_Bool();
			SquadList[0]->Set_Team(RandTeam);
			SquadList[1]->Set_Team(RandTeam?0:1);
		}
		else if (SquadList.Count() >= 3) { //If there are three or more we make many possible team combinations and choose the one that puts the most balanced amount of squad members on each team.
			int LeastDiff = 127;
			DynamicVectorClass<DASquadRemixStruct> LeastDiffSquadTeams;
			for (int loops = 0;loops < 100;loops++) {
				int LoopTeamCount[2] = {0,0};
				DynamicVectorClass<DASquadRemixStruct> SquadTeams;
				for (int i = 0;i < SquadList.Count();i++) {
					int RandTeam = Get_Random_Bool();
					LoopTeamCount[RandTeam] += SquadList[i]->Size();
					SquadTeams.Add(DASquadRemixStruct(SquadList[i],RandTeam));
				}
				if (Diff(LoopTeamCount[0],LoopTeamCount[1]) < 2) { //Not going to get any better than this.
					LeastDiffSquadTeams = SquadTeams;
					LeastDiff = Diff(LoopTeamCount[0],LoopTeamCount[1]);
					break;
				}
				else if (Diff(LoopTeamCount[0],LoopTeamCount[1]) < LeastDiff) {
					LeastDiffSquadTeams = SquadTeams;
					LeastDiff = Diff(LoopTeamCount[0],LoopTeamCount[1]);
				}
			}
			for (int i = 0;i < LeastDiffSquadTeams.Count();i++) {
				DASquadClass *Squad = LeastDiffSquadTeams[i].Squad;
				Squad->Set_Team(LeastDiffSquadTeams[i].Team);
			}
		}
	}
}

void DASquadManagerClass::Rebalance_Event() {
	int TeamCount[2] = {0,0};
	int NoSquadTeamCount[2] = {0,0};
	for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) { 
		cPlayer *Player = z->Data(); 
		if (Player->Is_Active()) { 
			if (Player->Get_Player_Type() == 0) { 
				TeamCount[0]++;
				if (!Find_Squad(Player)) {
					NoSquadTeamCount[0]++;
				}
			} 
			else if (Player->Get_Player_Type() == 1) {
				TeamCount[1]++;
				if (!Find_Squad(Player)) {
					NoSquadTeamCount[1]++;
				}
			} 
		}
	}

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
	//Teams are uneven, try to balance them without touching the squads.
	while ((TeamCount[OldTeam]-TeamCount[NewTeam]) > 1 && NoSquadTeamCount[OldTeam]) { //Go until teams are balanced or we run out of players not in a squad.
		int Rand = Get_Random_Int(0,NoSquadTeamCount[OldTeam]); //Select random player on team to change.
		int Count = 0;
		for (SLNode<cPlayer> *z = Get_Player_List()->Head();z;z = z->Next()) {
			cPlayer *Player = z->Data();
			if (Player->Is_Active() && Player->Get_Player_Type() == OldTeam && !Find_Squad(Player)) {
				if (Count == Rand) { //Loop until we find that player.
					Change_Team_3(Player,NewTeam);
					TeamCount[OldTeam]--;
					TeamCount[NewTeam]++;
					NoSquadTeamCount[OldTeam]--;
					NoSquadTeamCount[NewTeam]++;
					break;
				}
				Count++;
			}
		}
	}
	if ((TeamCount[OldTeam]-TeamCount[NewTeam]) > 1) { //Teams are still uneven, going to have to start removing people from squads.
		for (int i = SquadList.Count()-1;i >= 0 && TeamCount[OldTeam]-TeamCount[NewTeam] > 1;i--) { //Go until the teams are even or we run out of squads.
			DASquadClass *Squad = SquadList[i];
			for (int j = Squad->Size()-1;j >= 0 && TeamCount[OldTeam]-TeamCount[NewTeam] > 1;j--) { //Start with the last member of the last squad and work backwards.
				if (Squad->Get_Team() == OldTeam) {
					DASquadMemberClass *Member = Squad->Get_Member(j);
					DA::Private_Color_Message(Member->Get_ID(),SQUADCOLOR,"You have been removed from your squad to balance the teams.");
					Change_Team_3(Member->Get_Owner(),NewTeam);
					TeamCount[OldTeam]--;
					TeamCount[NewTeam]++;
					Member->Leave_Squad();
				}
			}
		}
	}
}

void DASquadManagerClass::Swap_Event() {
	for (int i = 0;i < SquadList.Count();i++) {
		SquadList[i]->Set_Team(SquadList[i]->Get_Team()?0:1);
	}
}

void DASquadManagerClass::Think() {
	for (int i = Invites.Count()-1;i >= 0;i--) {
		if (The_Game()->Get_Game_Duration_S()-Invites[i].Timeout >= 30) {
			Invites.Delete(i);
		}
	}
	for (int i = Joins.Count()-1;i >= 0;i--) {
		if (The_Game()->Get_Game_Duration_S()-Joins[i].Timeout >= 30) {
			Joins.Delete(i);
		}
	}
}

bool DASquadManagerClass::List_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	if (SquadList.Count()) {
		for (int i = 0;i < SquadList.Count();i++) {
			DASquadClass *Squad = SquadList[i];
			if (Squad->Get_Team() == 0) {
				StringClass Str;
				Str.Format("Leader: %ls - Members: ",Squad->Get_Leader()->Get_Name());
				for (int x = 1;x < Squad->Size();x++) {
					Str += StringClass(Squad->Get_Member(x)->Get_Name()) + ", ";
				}
				Str[Str.Get_Length()-2] = '\0';
				DA::Private_Color_Message_With_Team_Color(Player,0,"%s",Str);
			}
		}

		for (int i = 0;i < SquadList.Count();i++) {
			DASquadClass *Squad = SquadList[i];
			if (Squad->Get_Team() == 1) {
				StringClass Str;
				Str.Format("Leader: %ls - Members: ",Squad->Get_Leader()->Get_Name());
				for (int x = 1;x < Squad->Size();x++) {
					Str += StringClass(Squad->Get_Member(x)->Get_Name()) + ", ";
				}
				Str[Str.Get_Length()-2] = '\0';
				DA::Private_Color_Message_With_Team_Color(Player,1,"%s",Str);
			}
		}
		DA::Private_Color_Message(Player,SQUADCOLOR,"You can request to join one of these squads by typing \"!join <player name>\" in chat. Or create your own with \"!invite <player name>\".");
	}
	else {
		DA::Private_Color_Message(Player,SQUADCOLOR,"There are currently no squads. You can create a squad by typing \"!invite <player name>\" in chat.");
	}
	
	/*Console_InputF("msg squad  max size %u squads %d",Get_Max_Squad_Size(),SquadList.Count());
	for (int i = 0;i < SquadList.Count();i++) {
	DASquadClass *Squad = SquadList[i];
	Console_InputF("msg squad");
		for (int i = 0;i < Squad->Size();i++) {
			Console_InputF("msg member %ls",Squad->Get_Member(i)->Get_Name());
		}
	}
	for (int i = 0;i < Joins.Count();i++) {
		Console_InputF("msg joins %ls %d %d",Get_Wide_Player_Name_By_ID(Joins[i].PlayerID),Joins[i].LeaderID,Joins[i],Joins[i].Timeout);
	}
	for (int i = 0;i < Invites.Count();i++) {
		Console_InputF("msg invites %ls %d %d",Get_Wide_Player_Name_By_ID(Invites[i].PlayerID),Invites[i].LeaderID,Invites[i],Invites[i].Timeout);
	}
	for (int i = 0;i < WaitList.Count();i++) {
		Console_InputF("msg wait list %ls %d %d",Get_Wide_Player_Name_By_ID(WaitList[i].PlayerID),WaitList[i].LeaderID,WaitList[i],WaitList[i].Timeout);
	}*/
	return true;
}

bool DASquadManagerClass::Join_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	if (Find_Squad(Player)) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"You are already in a squad.");
	}
	else if (The_Game()->Get_Current_Players() < 7) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"There are not enough players to create a squad.");
	}
	else {
		cPlayer *MatchPlayer = Match_Player(Player,Text[1],false);
		if (MatchPlayer) {
			DASquadClass *Squad = Find_Squad(MatchPlayer);
			if (!Squad) {
				for (int i = 0;i < Invites.Count();i++) {
					if (Invites[i].Player == MatchPlayer && Invites[i].Leader == Player) {
						Invite_Accepted(i);
						return true;
					}
				}
				DA::Private_Color_Message(Player,SQUADCOLOR,"%ls is not in a squad.",MatchPlayer->Get_Name());
			}
			else if (Squad->Is_Full()) {
				DA::Private_Color_Message(Player,SQUADCOLOR,"%ls squad is full.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
			}
			else if (Is_Join_Pending(Player,Squad->Get_Leader()->Get_Owner())) {
				DA::Private_Color_Message(Player,SQUADCOLOR,"You have already requested to join %ls squad.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
			}
			else if (!RemixSquads && Squad->Get_Team() != Player->Get_Team()) {
				DA::Private_Color_Message(Player,SQUADCOLOR,"You cannot join squads on the other team.");
			}
			else {
				for (int i = 0;i < Invites.Count();i++) {
					if (Invites[i].Player == Player && Squad->Is_Leader(Invites[i].Leader)) {
						Invite_Accepted(i);
						return true;
					}
				}
				Remove_Join(Player);
				Remove_WaitList(Player);
				Remove_Invite(Player);
				Clear_Joins(Player);
				Clear_Invites(Player);
				Clear_WaitList(Player);
				DASquadInviteStruct Struct(Squad->Get_Leader()->Get_Owner(),Player);
				Joins.Add(Struct);
				DA::Private_Color_Message(Player,SQUADCOLOR,"You have requested to join %ls squad.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
				Squad->Leader_Message("%ls has requested to join your squad. Type \"!accept\" to accept, or \"!decline\" to decline.",Player->Get_Name());
			}
		}
	}
	return true;
}

bool DASquadManagerClass::Invite_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DASquadClass *Squad = Find_Squad(Player);
	if (Squad) {
		if (!Squad->Is_Leader(Player)) {
			DA::Private_Color_Message(Player,SQUADCOLOR,"You must be the squad leader to use this command.");
		}
		else if (Squad->Is_Full()) {
			DA::Private_Color_Message(Player,SQUADCOLOR,"Your squad is full.");
		}
		else {
			for (int j = 1;j <= Text.Size();j++) {
				cPlayer *MatchPlayer = Match_Player(Player,Text[j],false);
				if (MatchPlayer) {
					for (int i = 0;i < Joins.Count();i++) {
						if (Joins[i].Leader == Player && Joins[i].Player == MatchPlayer) {
							Join_Accepted(i);
							return true;
						}
					}
					if (Is_Invite_Pending(MatchPlayer) || Is_WaitList_Pending(MatchPlayer)) {
						DA::Private_Color_Message(Player,SQUADCOLOR,"%ls has already been invited to a squad.",MatchPlayer->Get_Name());
					}
					else if (Find_Squad(MatchPlayer)) {
						DA::Private_Color_Message(Player,SQUADCOLOR,"%ls is already in a squad.",MatchPlayer->Get_Name());
					}
					else if (!RemixSquads && Squad->Get_Team() != MatchPlayer->Get_Team()) {
						DA::Private_Color_Message(Player,SQUADCOLOR,"You cannot invite players on the other team.");
					}
					else {
						Invite(MatchPlayer,Player);
					}
				}
			}
		}
	}
	else if (The_Game()->Get_Current_Players() < 7) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"There are not enough players to create a squad.");
	}
	else {
		for (int i = 1;i <= Text.Size();i++) {
			cPlayer *MatchPlayer = Match_Player(Player,Text[i],false);
			if (MatchPlayer) {
				if (Is_Invite_Pending(MatchPlayer) || Is_WaitList_Pending(MatchPlayer)) {
					DA::Private_Color_Message(Player,SQUADCOLOR,"%ls has already been invited to a squad.",MatchPlayer->Get_Name());
				}
				else if (Find_Squad(MatchPlayer)) {
					DA::Private_Color_Message(Player,SQUADCOLOR,"%ls is already in a squad.",MatchPlayer->Get_Name());
				}
				else if (!RemixSquads && Player->Get_Team() != MatchPlayer->Get_Team()) {
					DA::Private_Color_Message(Player,SQUADCOLOR,"You cannot invite players on the other team.");
				}
				else {
					Invite(MatchPlayer,Player);
				}
			}
		}
	}
	return true;
}

bool DASquadManagerClass::Accept_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DASquadClass *Squad = Find_Squad(Player);
	if (Squad) {
		if (Squad->Is_Leader(Player)) {
			for (int i = 0;i < Joins.Count();i++) {
				if (Joins[i].Leader == Player) {
					Join_Accepted(i);
					break;
				}
			}
		}
	}
	else {
		for (int i = 0;i < Invites.Count();i++) {
			if (Invites[i].Player == Player) {
				Invite_Accepted(i);
				break;
			}
		}
	}
	return true;
}

bool DASquadManagerClass::Decline_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	DASquadClass *Squad = Find_Squad(Player);
	if (Squad) { 
		if (Squad->Is_Leader(Player)) {
			for (int i = 0;i < Joins.Count();i++) {
				if (Joins[i].Leader == Player) {
					DA::Private_Color_Message(Player,SQUADCOLOR,"You have refused %ls join request.",Make_Possessive(Joins[i].Leader->Get_Name()));
					DA::Private_Color_Message(Joins[i].Leader,SQUADCOLOR,"%ls has refused your join request.",Player->Get_Name());
					Joins.Delete(i);
					break;
				}
			}
		}
	}
	else {
		for (int i = 0;i < Invites.Count();i++) {
			if (Invites[i].Player == Player) {
				DA::Private_Color_Message(Player,SQUADCOLOR,"You have declined %ls squad invitation.",Make_Possessive(Invites[i].Leader->Get_Name()));
				DA::Private_Color_Message(Invites[i].Leader,SQUADCOLOR,"%ls has declined your squad invitation.",Player->Get_Name());
				Invites.Delete(i);
				break;
			}
		}
	}
	return true;
}

//This gets overridden by the one in DASquadMemberClass while in a squad.
bool DASquadManagerClass::Leave_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	if (Remove_WaitList(Player)) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"You have left the squad wait list.");
	}
	else if (Remove_Join(Player)) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"You have cancelled your squad join request.");
	}
	else if (Clear_Invites(Player)) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"You have rescinded your squad invitations.");
	}
	return true;
}

DASquadClass *DASquadManagerClass::Create_Squad(cPlayer *Player) {
	DASquadClass *Squad = new DASquadClass(Player);
	SquadList.Add(Squad);
	return Squad;
}

void DASquadManagerClass::Invite(cPlayer *Player,cPlayer *Leader) {
	DASquadInviteStruct Struct(Leader,Player);
	Invites.Add(Struct);
	DA::Private_Color_Message(Player,SQUADCOLOR,"%ls has invited you to join their squad. Type \"!accept\" to accept, or \"!decline\" to decline.",Leader->Get_Name());
	DA::Private_Color_Message(Leader,SQUADCOLOR,"You have invited %ls to join your squad.",Player->Get_Name());
}

void DASquadManagerClass::Join_Accepted(int JoinIndex) {
	cPlayer *Player = Joins[JoinIndex].Player;
	DASquadClass *Squad = Find_Squad(Joins[JoinIndex].Leader);
	if (Squad && Squad->Is_Leader(Joins[JoinIndex].Leader)) {
		if (!Find_Squad(Player)) {
			if (Squad->Get_Team() != Player->Get_Team()) {
				DA::Private_Color_Message(Player,SQUADCOLOR,"%ls squad is on the other team. You have been added to the wait list and will be teamed with them next game.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
				Squad->Leader_Message("%ls is on the other team. They have been added to the wait list and will be teamed with you next game.",Player->Get_Name());
				WaitList.Add(Joins[JoinIndex]);
			}
			else if (Squad->Is_Full()) {
				DA::Private_Color_Message(Player,SQUADCOLOR,"%ls squad is full.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
				Squad->Leader_Message("Your squad is full.");
			}
			else if (Squad->Add(Player)) {
				DA::Private_Color_Message(Player,SQUADCOLOR,"%ls has accepted your request to join their squad.",Squad->Get_Leader()->Get_Name());
			}
		}
	}
	Joins.Delete(JoinIndex);
	Clear_Invites(Player);
	Clear_WaitList(Player);
}

void DASquadManagerClass::Invite_Accepted(int InviteIndex) {
	cPlayer *Player = Invites[InviteIndex].Player;
	DASquadClass *Squad = Find_Squad(Invites[InviteIndex].Leader);
	if (Squad) {
		if (Squad->Get_Team() != Player->Get_Team()) {
			DA::Private_Color_Message(Player,SQUADCOLOR,"%ls squad is on the other team. You have been added to the wait list and will be teamed with them next game.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
			Squad->Leader_Message("%ls is on the other team. They have been added to the wait list and will be teamed with you next game.",Player->Get_Name());
			WaitList.Add(Invites[InviteIndex]);
		}
		else if (Squad->Is_Full()) {
			DA::Private_Color_Message(Player,SQUADCOLOR,"%ls squad is full.",Make_Possessive(Squad->Get_Leader()->Get_Name()));
			Squad->Leader_Message("Your squad is full.");
		}
		else if (Squad->Add(Player)) {
			Squad->Leader_Message("%ls has accepted your squad invitation.",Player->Get_Name());
		}
	}
	else if (Invites[InviteIndex].Leader->Get_Team() != Player->Get_Team()) {
		DA::Private_Color_Message(Player,SQUADCOLOR,"%ls is on the other team. You have been added to the wait list and will be teamed with them next game.",Invites[InviteIndex].Leader->Get_Name());
		DA::Private_Color_Message(Invites[InviteIndex].Leader,SQUADCOLOR,"%ls is on the other team. They have been added to the wait list and will be teamed with you next game.",Player->Get_Name());
		WaitList.Add(Invites[InviteIndex]);
	}
	else {
		DA::Private_Color_Message(Invites[InviteIndex].Leader,SQUADCOLOR,"%ls has accepted your squad invitation.",Player->Get_Name());
		DASquadClass *NewSquad = Create_Squad(Invites[InviteIndex].Leader);
		NewSquad->Add(Player);
	}
	Invites.Delete(InviteIndex);
	Clear_Invites(Player);
	Clear_WaitList(Player);
}

bool DASquadManagerClass::Is_Join_Pending(cPlayer *Player,cPlayer *Leader) {
	for (int i = 0;i < Joins.Count();i++) {
		if (Joins[i].Player == Player && (!Leader || Joins[i].Leader == Leader)) {
			return true;
		}
	}
	return false;
}

bool DASquadManagerClass::Is_Invite_Pending(cPlayer *Player,cPlayer *Leader) {
	for (int i = 0;i < Invites.Count();i++) {
		if (Invites[i].Player == Player && (!Leader || Invites[i].Leader == Leader)) {
			return true;
		}
	}
	return false;
}

bool DASquadManagerClass::Is_WaitList_Pending(cPlayer *Player,cPlayer *Leader) {
	for (int i = 0;i < WaitList.Count();i++) {
		if (WaitList[i].Player == Player && (!Leader || WaitList[i].Leader == Leader)) {
			return true;
		}
	}
	return false;
}

bool DASquadManagerClass::Remove_Join(cPlayer *Player,cPlayer *Leader) {
	for (int i = 0;i < Joins.Count();i++) {
		if (Joins[i].Player == Player && (!Leader || Joins[i].Leader == Leader)) {
			Joins.Delete(i);
			return true;
		}
	}
	return false;
}

bool DASquadManagerClass::Remove_Invite(cPlayer *Player,cPlayer *Leader) {
	for (int i = 0;i < Invites.Count();i++) {
		if (Invites[i].Player == Player && (!Leader || Invites[i].Leader == Leader)) {
			Invites.Delete(i);
			return true;
		}
	}
	return false;
}

bool DASquadManagerClass::Remove_WaitList(cPlayer *Player,cPlayer *Leader) {
	for (int i = 0;i < WaitList.Count();i++) {
		if (WaitList[i].Player == Player && (!Leader || WaitList[i].Leader == Leader)) {
			WaitList.Delete(i);
			return true;
		}
	}
	return false;
}

bool DASquadManagerClass::Clear_Joins(cPlayer *Player) {
	bool Return = false;
	for (int i = Joins.Count()-1;i >= 0;i--) {
		if (Joins[i].Leader == Player) {
			Joins.Delete(i);
			Return = true;
		}
	}
	return Return;
}

bool DASquadManagerClass::Clear_Invites(cPlayer *Player) {
	bool Return = false;
	for (int i = Invites.Count()-1;i >= 0;i--) {
		if (Invites[i].Leader == Player) {
			Invites.Delete(i);
			Return = true;
		}
	}
	return Return;
}

bool DASquadManagerClass::Clear_WaitList(cPlayer *Player) {
	bool Return = false;
	for (int i = WaitList.Count()-1;i >= 0;i--) {
		if (WaitList[i].Leader == Player) {
			WaitList.Delete(i);
			Return = true;
		}
	}
	return Return;
}

DASquadClass *DASquadManagerClass::Find_Squad(cPlayer *Player) {
	for (int i = 0;i < SquadList.Count();i++) {
		if (SquadList[i]->Is_Member(Player)) {
			return SquadList[i];
		}
	}
	return 0;
}

DASquadClass *DASquadManagerClass::Find_Squad(int ID) {
	for (int i = 0;i < SquadList.Count();i++) {
		if (SquadList[i]->Is_Member(ID)) {
			return SquadList[i];
		}
	}
	return 0;
}

DASquadClass *DASquadManagerClass::Find_Squad(GameObject *obj) {
	for (int i = 0;i < SquadList.Count();i++) {
		if (SquadList[i]->Is_Member(obj)) {
			return SquadList[i];
		}
	}
	return 0;
}

void DASquadManagerClass::Check_WaitList(cPlayer *Player) {
	for (int i = 0;i < WaitList.Count();i++) {
		if (WaitList[i].Player == Player) {
			DASquadClass *Squad = Find_Squad(WaitList[i].Leader);
			if (Squad) {
				if (Squad->Get_Team() == Player->Get_Team() && Squad->Is_Leader(WaitList[i].Leader)) {
					Squad->Add(Player);
				}
			}
			else if (WaitList[i].Leader->Get_Team() == Player->Get_Team()) {
				DASquadClass *NewSquad = Create_Squad(WaitList[i].Leader);
				NewSquad->Add(Player);
			}
			WaitList.Delete(i);
			break;
		}
		else if (WaitList[i].Leader == Player) {
			DASquadClass *Squad = Find_Squad(WaitList[i].Leader);
			if (Squad) {
				if (Squad->Get_Team() == WaitList[i].Player->Get_Team() && Squad->Is_Leader(Player)) {
					Squad->Add(WaitList[i].Player);
				}
			}
			else if (WaitList[i].Leader->Get_Team() == Player->Get_Team()) {
				DASquadClass *NewSquad = Create_Squad(Player);
				NewSquad->Add(WaitList[i].Player);
			}
			WaitList.Delete(i);
			i--;
		}
	}
}

unsigned int DASquadManagerClass::Get_Max_Squad_Size() {
	unsigned int Size = (unsigned int)(The_Game()->Get_Current_Players()/2)-1;
	if (Size < MaxSquadSize) {
		return Size;
	}
	return MaxSquadSize;
}

DA_API Register_Game_Feature(DASquadManagerClass,"Squad System","EnableSquads",0);

