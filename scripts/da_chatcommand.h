/*	Renegade Scripts.dll
    Dragonade Chat Command Manager
	Copyright 2012 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLIDE_DACHATCOMMAND
#define INCLIDE_DACHATCOMMAND

#include "da_event.h"
#include "da_token.h"

class DAChatCommandClass abstract {
public:
	DynamicVectorClass<unsigned int> Triggers;
	DAAccessLevel::Level AccessLevel;
	DAChatType::Type ChatType;
	int Parameters;
	virtual bool Activate(cPlayer *Player,const DATokenClass &Text,TextMessageEnum ChatType) = 0;
};

struct DAEventChatCommandStruct {
	DynamicVectorClass<unsigned int> Triggers;
	DAAccessLevel::Level AccessLevel;
	DAChatType::Type ChatType;
	int Parameters;
	DAEventClass *Base;
	DAECC Func;
};

class DAKeyHookClass abstract {
public:
	DynamicVectorClass<unsigned int> Triggers;
	virtual void Activate(cPlayer *Player) = 0;
};

struct DAEventKeyHookStruct {
	DynamicVectorClass<unsigned int> Triggers;
	DAEventClass *Base;
	DAEKH Func;
};

class DA_API DAChatCommandManager : private DAEventClass {
public:
	static void Init();
	virtual bool Chat_Command_Event(cPlayer *Player,TextMessageEnum Type,const StringClass &Command,const DATokenClass &Text,int ReceiverID);
	virtual bool Key_Hook_Event(cPlayer *Player,const StringClass &Key);
	template<class T> static void Register_Chat_Command(const char *Triggers,int Parameters = 0,DAAccessLevel::Level AccessLevel = DAAccessLevel::NONE,DAChatType::Type ChatType = DAChatType::ALL) {
		DAChatCommandClass *Base = new T;
		Base->AccessLevel = AccessLevel;
		Base->ChatType = ChatType;
		Base->Parameters = Parameters;
		DATokenParserClass Parser(Triggers,'|');
		while (char *Token = Parser.Get_String()) {
			_strlwr(Token);
			Base->Triggers.Add(Get_Hash(Token));
		}
		ChatCommands.Add(Base);
	}
	static void Register_Event_Chat_Command(DAEventClass *Base,DAECC Func,const char *Triggers,int Parameters = 0,DAAccessLevel::Level AccessLevel = DAAccessLevel::NONE,DAChatType::Type ChatType = DAChatType::ALL) {
		DAEventChatCommandStruct *Struct = new DAEventChatCommandStruct;
		Struct->AccessLevel = AccessLevel;
		Struct->ChatType = ChatType;
		Struct->Parameters = Parameters;
		DATokenParserClass Parser(Triggers,'|');
		while (char *Token = Parser.Get_String()) {
			_strlwr(Token);
			Struct->Triggers.Add(Get_Hash(Token));
		}
		Struct->Base = Base;
		Struct->Func = Func;
		EventChatCommands.Add(Struct);
	}
	template<class T> static void Register_Key_Hook(const char *Triggers) {
		DAKeyHookClass *Base = new T;
		DATokenParserClass Parser(Triggers,'|');
		while (char *Token = Parser.Get_String()) {
			_strlwr(Token);
			Base->Triggers.Add(Get_Hash(Token));
		}
		KeyHooks.Add(Base);
	}
	static void Register_Event_Key_Hook(DAEventClass *Base,DAEKH Func,const char *Triggers) {
		DAEventKeyHookStruct *Struct = new DAEventKeyHookStruct;
		DATokenParserClass Parser(Triggers,'|');
		while (char *Token = Parser.Get_String()) {
			_strlwr(Token);
			Struct->Triggers.Add(Get_Hash(Token));
		}
		Struct->Base = Base;
		Struct->Func = Func;
		EventKeyHooks.Add(Struct);
	}
	static void Unregister_Chat_Command(const char *Trigger);
	static void Clear_Event_Chat_Commands(DAEventClass *Base);
	static void Unregister_Key_Hook(const char *Trigger);
	static void Clear_Event_Key_Hooks(DAEventClass *Base);

private:
	static DynamicVectorClass<DAChatCommandClass*> ChatCommands;
	static DynamicVectorClass<DAEventChatCommandStruct*> EventChatCommands;
	static DynamicVectorClass<DAKeyHookClass*> KeyHooks;
	static DynamicVectorClass<DAEventKeyHookStruct*> EventKeyHooks;
	static DATokenClass NullToken;
};

template <class T> class DAChatCommandRegistrant {
public:
	DAChatCommandRegistrant(const char *Triggers,int Parameters = 0,DAAccessLevel::Level AccessLevel = DAAccessLevel::NONE,DAChatType::Type ChatType = DAChatType::ALL) {
		DAChatCommandManager::Register_Chat_Command<T>(Triggers,Parameters,AccessLevel,ChatType);
	}
};

template <class T> class DAKeyHookRegistrant {
public:
	DAKeyHookRegistrant(const char *Triggers) {
		DAChatCommandManager::Register_Key_Hook<T>(Triggers);
	}
};

#endif