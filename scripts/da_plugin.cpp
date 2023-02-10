/*	Renegade Scripts.dll
    Dragonade Plugin Manager
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
#include "da.h"
#include "da_plugin.h"
#include "da_settings.h"
#include "da_ssgm.h"
#include "da_log.h"

DynamicVectorClass<HINSTANCE> DAPluginManager::Plugins;

typedef void (*PluginInit)();
typedef void (*PluginShutdown)();

void DAPluginManager::Init() {
	DASSGMPluginManager::Init();
	INISection *Section = DASettingsManager::Get_Main_Settings()->Get_INI()->Get_Section("Plugins");
	if (Section) {
		bool NewLine = false;
		for (int i = 0;i < Section->Count();i++) {
			INIEntry *Entry = Section->Peek_Entry(i);
			if (!_stricmp(Entry->Value,"1") || !_stricmp(Entry ->Value,"true")) {
				NewLine = true;
				HINSTANCE Handle = LoadLibrary(Entry->Entry);
				if (Handle) {
					PluginInit Init = (PluginInit)GetProcAddress(Handle,"Plugin_Init");
					if (Init) {
						Init();
					}
					Plugins.Add(Handle);
					Console_Output("Plugin %s loaded\n",Entry->Entry);
					DALogManager::Write_Log("_PLUGIN","load %s",Entry->Entry);
				}
				else {
					Console_Output("Could not load plugin %s\n",Entry->Entry);
					DALogManager::Write_Log("_PLUGIN","failure %s",Entry->Entry);
				}
			}
		}
		if (NewLine) {
			Console_Output("\n");
		}
	}
}

void DAPluginManager::Shutdown() {
	for (int i = 0;i < Plugins.Count();i++) {
		PluginShutdown Shutdown = (PluginShutdown)GetProcAddress(Plugins[i],"Plugin_Shutdown");
		if (Shutdown) {
			Shutdown();
		}
		FreeLibrary(Plugins[i]);
	}
}
