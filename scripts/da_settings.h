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

#ifndef INCLUDE_DASETTINGS
#define INCLUDE_DASETTINGS

#include "RawFileClass.h"
#include "engine_io.h"

class DA_API DASettingsClass {
public:
	inline DASettingsClass() {
		INI = 0;
	}
	inline DASettingsClass(const char *Name) {
		INI = 0;
		Set_INI(Name);
	}
	inline DASettingsClass(INIClass *That) {
		INI = 0;
		Set_INI(That->Get_File_Name());
	}
	inline DASettingsClass(const DASettingsClass &That) {
		INI = 0;
		Set_INI(That.INI->Get_File_Name());
	}
	inline DASettingsClass &operator=(const DASettingsClass &That) { 
		Set_INI(That.INI->Get_File_Name());
		return *this; 
	}
	inline DASettingsClass &operator=(INIClass *That) {
		Set_INI(That->Get_File_Name());
		return *this;
	}
	inline DASettingsClass &operator=(const char *Name) {
		Set_INI(Name);
		return *this;
	}
	inline void Set_INI(const char *ini) {
		Release();
		RawFileClass File(ini);
		if (File.Is_Available(0)) {
			INI = new INIClass(File);
		}
		else {
			File.Open("server.ini",0);
			INI = new INIClass(File);
		}
	}
	inline void Set_INI(INIClass *That) {
		Set_INI(That->Get_File_Name());
	}
	inline void Set_INI(const DASettingsClass &That) {
		Set_INI(That.Get_File_Name());
	}
	inline const INIClass *Get_INI() const {
		return INI;
	}
	inline const char *Get_File_Name() const {
		return INI->Get_File_Name();
	}
	inline void Release() {
		delete INI;
		INI = 0;
	}
	inline void Reload() {
		if (INI) {
			Set_INI(INI->Get_File_Name());
		}
	}
	~DASettingsClass() {
		Release();
	}

	int Get_Int(const char *Entry,int Default) const;
	int Get_Int(const char *Section,const char *Entry,int Default) const;
	float Get_Float(const char *Entry,float Default) const;
	float Get_Float(const char *Section,const char *Entry,float Default) const;
	bool Get_Bool(const char *Entry,bool Default) const;
	bool Get_Bool(const char *Section,const char *Entry,bool Default) const;
	StringClass &Get_String(StringClass &Str,const char *Entry,const char *Default) const;
	StringClass &Get_String(StringClass &Str,const char *Section,const char *Entry,const char *Default) const;
	void Get_Vector3(Vector3 &Buffer,const char *Entry,const Vector3 &Default) const;
	void Get_Vector3(Vector3 &Buffer,const char *Section,const char *Entry,const Vector3 &Default) const;
	INISection *Get_Section(const char *Section) const;

private:
	INIClass *INI;
};

class DA_API DASettingsManager {
public:
	static void Init();
	static void Shutdown();
	static void Reload(); //Reloads da.ini and gamemode.ini. Does not check for new game mode.
	static void Reload_GameMode(); //Sets gamemode.ini if the game mode has changed.
	static void Post_Reload(); //Triggers settings loaded event and displays new settings detected message.
	
	static const DASettingsClass *Get_Main_Settings() {
		return Main;
	}
	static const DASettingsClass *Get_GameMode_Settings() {
		return GameMode;
	}

	static int Get_Int(const char *Entry,int Default);
	static int Get_Int(const char *Section,const char *Entry,int Default);
	static float Get_Float(const char *Entry,float Default);
	static float Get_Float(const char *Section,const char *Entry,float Default);
	static bool Get_Bool(const char *Entry,bool Default);
	static bool Get_Bool(const char *Section,const char *Entry,bool Default);
	static StringClass &Get_String(StringClass &Str,const char *Entry,const char *Default);
	static StringClass &Get_String(StringClass &Str,const char *Section,const char *Entry,const char *Default);
	static void Get_Vector3(Vector3 &Buffer,const char *Entry,const Vector3 &Default);
	static void Get_Vector3(Vector3 &Buffer,const char *Section,const char *Entry,const Vector3 &Default);
	static INISection *Get_Section(const char *Section);
	static INISection *Get_Section_No_Map(const char *Section);
	
private:
	static DASettingsClass *Main;
	static DASettingsClass *GameMode;
	static unsigned int LastMainModTime;
	static unsigned int LastGameModeModTime;
};

#endif