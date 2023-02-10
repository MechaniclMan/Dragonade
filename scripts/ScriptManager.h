/*	Renegade Scripts.dll
	Copyright 2015 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef TT_INCLUDE_SCRIPTMANAGER_H
#define TT_INCLUDE_SCRIPTMANAGER_H
#include "engine_vector.h"
#include "scripts.h"
#include "engine_io.h"
typedef void (*srdf) (void (*function)(ScriptClass*));
typedef void (*ds) (ScriptClass *);
typedef ScriptClass *(*cs) (const char *);
typedef int (*gsc) ();
typedef char *(*gsn) (int);
typedef char *(*gspd) (int);
typedef bool (*ssc) (ScriptCommandsClass *);
class ScriptManager {
public:
	static cs ScriptCreateFunct;
	static ds ScriptDestroyFunct;
	static bool EnableScriptCreation;
	static HMODULE hDLL;
	static SimpleDynVecClass<ScriptClass *> PendingDestroyList;
	static SimpleDynVecClass<ScriptClass *> ActiveScriptList;
	static ScriptCommandsClass EngineCommands;
	static SHARED_API void Init();
	static SHARED_API void Shutdown();
	static SHARED_API void Destroy_Pending();
	static void Load_Scripts(char const *name);
	static SHARED_API ScriptClass *Create_Script(char const *name);
	static void Request_Destroy_Script(ScriptClass *script);
	static SHARED_API bool Save(ChunkSaveClass &saver);
	static SHARED_API bool Load(ChunkLoadClass &loader);
	static ScriptCommandsClass Get_Script_Commands();
};
#endif