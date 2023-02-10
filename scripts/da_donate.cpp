/*	Renegade Scripts.dll
    Dragonade Donate Game Feature
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
#include "da_donate.h"
#include "da_settings.h"

void DADonateGameFeatureClass::Init() {
	Register_Event(DAEvent::SETTINGSLOADED);
	Register_Chat_Command((DAECC)&DADonateGameFeatureClass::Donate_Chat_Command,"!donate|!d|!tdonate|!teamdonate|!donateteam|!td");
}

void DADonateGameFeatureClass::Settings_Loaded_Event() {
	TimeLimit = DASettingsManager::Get_Int("DonateTimeLimit",120);
	Stop_Timer(1);
	int Timer = TimeLimit-The_Game()->Get_Game_Duration_S();
	if (Timer > 0) {
		Start_Timer(1,(float)Timer);
	}
}

void DADonateGameFeatureClass::Timer_Expired(int Number,unsigned int Data) {
	DA::Host_Message("Donations are now enabled.");
}

bool DADonateGameFeatureClass::Donate_Chat_Command(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) {
	if ((int)The_Game()->Get_Game_Duration_S() < TimeLimit) {
		DA::Page_Player(Player,"You cannot use donate during the first %d seconds of a game.",TimeLimit);
	}
	else if (Text.Size() < 2) { //Distributive donate
		int Amount = 0;
		if (!Text.As_Int(1,Amount)) {
			Amount = (int)Player->Get_Money();
		}
		if (Amount < 1) {
			return true;
		}
		int Count = Get_Team_Player_Count(Player->Get_Team())-1;
		int Amount2 = (int)Round((float)Amount/Count);
		if (Count < 1) {
			DA::Page_Player(Player,"There are not enough players on your team to distribute your credits among.");
		}
		else if (Amount2 < 10 || !Player->Purchase_Item(Amount)) {
			if (Player->Get_Team() == 0) {
				Send_Announcement_Player(Player->Get_ID(),"IDS_M00EVAN_DSGN0024I1EVAN_TXT");
			}
			else {
				Send_Announcement_Player(Player->Get_ID(),"IDS_M00EVAG_DSGN0028I1EVAG_TXT");
			}
		}
		else {
			DA::Page_Player(Player,"You have donated %d credit(s) to each of your teammates, %d in total.",Amount2,Amount);
			DA::Page_Team_Except(Player->Get_Team(),Player,"%ls has donated %d credit(s) to you.",Player->Get_Name(),Amount2);
			Give_Credits_Team(Player->Get_Team(),(float)Amount2);
			Player->Increment_Money(Amount2*-1.0f);
		}
	}
	else { //Player donate
		int Amount = 0;
		Text.As_Int(2,Amount);
		if (Amount < 1) {
			return true;
		}
		cPlayer *Receiver = Match_Player(Player,Text[1],true);
		if (Receiver) {
			if (!Player->Purchase_Item(Amount)) {
				if (Player->Get_Team() == 0) {
					Send_Announcement_Player(Player->Get_ID(),"IDS_M00EVAN_DSGN0024I1EVAN_TXT");
				}
				else {
					Send_Announcement_Player(Player->Get_ID(),"IDS_M00EVAG_DSGN0028I1EVAG_TXT");
				}
			}
			else {
				DA::Page_Player(Player,"You have donated %d credit(s) to %ls.",Amount,Receiver->Get_Name());
				DA::Page_Player(Receiver,"%ls has donated %d credit(s) to you.",Player->Get_Name(),Amount);
				Receiver->Increment_Money((float)Amount);
			}
		}
	}
	return true;
}

Register_Game_Feature(DADonateGameFeatureClass,"Donate","EnableDonate",0);