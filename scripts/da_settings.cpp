/*	Renegade Scripts.dll
    Dragonade Settings Manager
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
#include "da.h"
#include "da_settings.h"
#include "da_event.h"
#include "da_game.h"
#include "da_log.h"
#include "engine_game.h"
#include "engine_tt.h"

DASettingsClass *DASettingsManager::Main;
DASettingsClass *DASettingsManager::GameMode;
unsigned int DASettingsManager::LastMainModTime = 0;
unsigned int DASettingsManager::LastGameModeModTime = 0;

class ReloadConsoleCommand : public ConsoleFunctionClass {
public:
	const char *Get_Name() { return "reload"; }
	const char *Get_Alias() { return "rehash"; }
	const char *Get_Help() { return "RELOAD - Reload da.ini and gamemode.ini."; }
	void Activate(const char *ArgumentsString) {
		DASettingsManager::Reload();
		DASettingsManager::Post_Reload();
	}
};

void DASettingsManager::Init() {
	Main = new DASettingsClass("da.ini");
	GameMode = new DASettingsClass("da.ini");
	LastMainModTime = 0;
	LastGameModeModTime = 0;
	ConsoleFunctionList.Add(new ReloadConsoleCommand);
}

void DASettingsManager::Shutdown() {
	delete GameMode;
	delete Main;
}

void DASettingsManager::Reload() {
	Main->Reload();
	GameMode->Reload();
}

void DASettingsManager::Reload_GameMode() {
	StringClass GameModeFileName;
	GameModeFileName.Format("da_%s.ini",DAGameManager::Get_Game_Mode_Short_Name());
	GameModeFileName.ToLower();
	if (GameModeFileName != GameMode->Get_File_Name()) {
		GameMode->Set_INI(GameModeFileName);
		LastGameModeModTime = 1;
	}
}

void DASettingsManager::Post_Reload() {
	RawFileClass File;
	File.Open(Main->Get_File_Name(),1);
	if (File.Get_Date_Time() != LastMainModTime) {
		LastMainModTime = File.Get_Date_Time();
		Console_Output("New settings detected and loaded from \"%s!\"\n",Main->Get_File_Name());
		DALogManager::Write_Log("_GENERAL","New settings detected and loaded from \"%s!\"",Main->Get_File_Name());
	}
	File.Close();
	File.Open(GameMode->Get_File_Name(),1);
	if (File.Get_Date_Time() != LastGameModeModTime) {
		LastGameModeModTime = File.Get_Date_Time();
		Console_Output("New settings detected and loaded from \"%s!\"\n",GameMode->Get_File_Name());
		DALogManager::Write_Log("_GENERAL","New settings detected and loaded from \"%s!\"",GameMode->Get_File_Name());
	}
	File.Close();
	DAEventManager::Settings_Loaded_Event();
}

int DASettingsManager::Get_Int(const char *Entry,int Default) {
	return GameMode->Get_Int(Entry,Main->Get_Int(Entry,Default));
}

int DASettingsManager::Get_Int(const char *Section,const char *Entry,int Default) {
	return GameMode->Get_Int(Section,Entry,Main->Get_Int(Section,Entry,Default));
}

float DASettingsManager::Get_Float(const char *Entry,float Default) {
	return GameMode->Get_Float(Entry,Main->Get_Float(Entry,Default));
}

float DASettingsManager::Get_Float(const char *Section,const char *Entry,float Default) {
	return GameMode->Get_Float(Section,Entry,Main->Get_Float(Section,Entry,Default));
}

bool DASettingsManager::Get_Bool(const char *Entry,bool Default) {
	return GameMode->Get_Bool(Entry,Main->Get_Bool(Entry,Default));
}

bool DASettingsManager::Get_Bool(const char *Section,const char *Entry,bool Default) {
	return GameMode->Get_Bool(Section,Entry,Main->Get_Bool(Section,Entry,Default));
}

StringClass &DASettingsManager::Get_String(StringClass &Str,const char *Entry,const char *Default) {
	return GameMode->Get_String(Str,Entry,Main->Get_String(Str,Entry,Default));
}

StringClass &DASettingsManager::Get_String(StringClass &Str,const char *Section,const char *Entry,const char *Default) {
	return GameMode->Get_String(Str,Section,Main->Get_String(Str,Section,Entry,Default));
}

void DASettingsManager::Get_Vector3(Vector3 &Buffer,const char *Entry,const Vector3 &Default) {
	Main->Get_Vector3(Buffer,Entry,Default);
	GameMode->Get_Vector3(Buffer,Entry,Buffer);
}

void DASettingsManager::Get_Vector3(Vector3 &Buffer,const char *Section,const char *Entry,const Vector3 &Default) {
	Main->Get_Vector3(Buffer,Section,Entry,Default);
	GameMode->Get_Vector3(Buffer,Section,Entry,Buffer);
}

INISection *DASettingsManager::Get_Section(const char *Section) {
	StringClass MapSection;
	MapSection.Format("%s_%s",The_Game()->Get_Map_Name(),Section);
	if (INISection *Sect = GameMode->Get_Section(MapSection)) {
		return Sect;
	}
	else if (INISection *Sect = GameMode->Get_Section(Section)) {
		return Sect;
	}
	else if (INISection *Sect = Main->Get_Section(MapSection)) {
		return Sect;
	}
	return Main->Get_Section(Section);
}

INISection *DASettingsManager::Get_Section_No_Map(const char *Section) {
	if (INISection *Sect = GameMode->Get_Section(Section)) {
		return Sect;
	}
	return Main->Get_Section(Section);
}


int DASettingsClass::Get_Int(const char *Entry,int Default) const {
	return INI->Get_Int(The_Game()->Get_Map_Name(),Entry,INI->Get_Int("General",Entry,Default));
}

int DASettingsClass::Get_Int(const char *Section,const char *Entry,int Default) const {
	return INI->Get_Int(StringFormat("%s_%s",Section,The_Game()->Get_Map_Name()),Entry,INI->Get_Int(Section,Entry,Default));
}

float DASettingsClass::Get_Float(const char *Entry,float Default) const {
	return INI->Get_Float(The_Game()->Get_Map_Name(),Entry,INI->Get_Float("General",Entry,Default));
}

float DASettingsClass::Get_Float(const char *Section,const char *Entry,float Default) const {
	return INI->Get_Float(StringFormat("%s_%s",Section,The_Game()->Get_Map_Name()),Entry,INI->Get_Float(Section,Entry,Default));
}

bool DASettingsClass::Get_Bool(const char *Entry,bool Default) const {
	return INI->Get_Bool(The_Game()->Get_Map_Name(),Entry,INI->Get_Bool("General",Entry,Default));
}

bool DASettingsClass::Get_Bool(const char *Section,const char *Entry,bool Default) const {
	return INI->Get_Bool(StringFormat("%s_%s",Section,The_Game()->Get_Map_Name()),Entry,INI->Get_Bool(Section,Entry,Default));
}

StringClass &DASettingsClass::Get_String(StringClass &Str,const char *Entry,const char *Default) const {
	return INI->Get_String(Str,The_Game()->Get_Map_Name(),Entry,INI->Get_String(Str,"General",Entry,Default));
}

StringClass &DASettingsClass::Get_String(StringClass &Str,const char *Section,const char *Entry,const char *Default) const {
	return INI->Get_String(Str,StringFormat("%s_%s",Section,The_Game()->Get_Map_Name()),Entry,INI->Get_String(Str,Section,Entry,Default));
}

void DASettingsClass::Get_Vector3(Vector3 &Buffer,const char *Entry,const Vector3 &Default) const {
	StringClass String;
	StringClass StringBuffer;
	String.Format("%s_X",Entry);
	if (!INI->Get_String(StringBuffer,The_Game()->Get_Map_Name(),String,0).Is_Empty()) {
		Buffer.X = INI->Get_Float(The_Game()->Get_Map_Name(),String,Default.X);
		String.Format("%s_Y",Entry);
		Buffer.Y = INI->Get_Float(The_Game()->Get_Map_Name(),String,Default.Y);
		String.Format("%s_Z",Entry);
		Buffer.Z = INI->Get_Float(The_Game()->Get_Map_Name(),String,Default.Z);
	}
	else {
		Buffer.X = INI->Get_Float("General",String,Default.X);
		String.Format("%s_Y",Entry);
		Buffer.Y = INI->Get_Float("General",String,Default.Y);
		String.Format("%s_Z",Entry);
		Buffer.Z = INI->Get_Float("General",String,Default.Z);
	}
}

void DASettingsClass::Get_Vector3(Vector3 &Buffer,const char *Section,const char *Entry,const Vector3 &Default) const {
	StringClass String;
	StringClass StringBuffer;
	StringClass MapSection;
	MapSection.Format("%s_%s",The_Game()->Get_Map_Name(),Section);
	String.Format("%s_X",Entry);
	if (!INI->Get_String(StringBuffer,MapSection,String,0).Is_Empty()) {
		Buffer.X = INI->Get_Float(MapSection,String,Default.X);
		String.Format("%s_Y",Entry);
		Buffer.Y = INI->Get_Float(MapSection,String,Default.Y);
		String.Format("%s_Z",Entry);
		Buffer.Z = INI->Get_Float(MapSection,String,Default.Z);
	}
	else {
		Buffer.X = INI->Get_Float("General",String,Default.X);
		String.Format("%s_Y",Entry);
		Buffer.Y = INI->Get_Float("General",String,Default.Y);
		String.Format("%s_Z",Entry);
		Buffer.Z = INI->Get_Float("General",String,Default.Z);
	}
}

INISection *DASettingsClass::Get_Section(const char *Section) const {
	if (INISection *Sect = INI->Get_Section(StringFormat("%s_%s",Section,The_Game()->Get_Map_Name()))) {
		return Sect;
	}
	return INI->Get_Section(Section);
}


