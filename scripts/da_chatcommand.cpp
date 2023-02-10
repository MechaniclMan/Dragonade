/*	Renegade Scripts.dll
    Dragonade Chat Command Manager
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
#include "engine_da.h"
#include "cScTextObj.h"
#include "da.h"
#include "da_chatcommand.h"
#include "da_player.h"
#include "da_settings.h"

#pragma warning(disable: 4073)
#pragma init_seg(lib)

DynamicVectorClass<DAChatCommandClass*> DAChatCommandManager::ChatCommands;
DynamicVectorClass<DAEventChatCommandStruct*> DAChatCommandManager::EventChatCommands;
DynamicVectorClass<DAKeyHookClass*> DAChatCommandManager::KeyHooks;
DynamicVectorClass<DAEventKeyHookStruct*> DAChatCommandManager::EventKeyHooks;
DATokenClass DAChatCommandManager::NullToken; //For passing to chat commands that get triggered by key hooks.

void DAChatCommandManager::Init() {
	static DAChatCommandManager Instance;
	Instance.Register_Event(DAEvent::SETTINGSLOADED,INT_MAX);
	Instance.Register_Event(DAEvent::CHATCOMMAND,INT_MIN);
	Instance.Register_Event(DAEvent::KEYHOOK,INT_MIN);
}

void DAChatCommandManager::Shutdown() {
	for (int i = 0;i < EventChatCommands.Count();i++) {
		delete EventChatCommands[i];
	}
	EventChatCommands.Delete_All();

	for (int i = 0;i < ChatCommands.Count();i++) {
		delete ChatCommands[i];
	}
	ChatCommands.Delete_All();

	for (int i = 0;i < EventKeyHooks.Count();i++) {
		delete EventKeyHooks[i];
	}
	EventKeyHooks.Delete_All();

	for (int i = 0;i < KeyHooks.Count();i++) {
		delete KeyHooks[i];
	}
	KeyHooks.Delete_All();
}

bool DAChatCommandManager::Chat_Command_Event(cPlayer *Player,TextMessageEnum Type,const StringClass &Command,const DATokenClass &Text,int ReceiverID) {
	unsigned int CommandHash = Command.GetHash();
	const DynamicVectorClass<DAPlayerObserverClass*> &Observers = Player->Get_DA_Player()->Get_Observers();
	for (int i = Observers.Count()-1;i >= 0;i--) {
		const DynamicVectorClass<DAPlayerObserverChatCommandStruct*> &Commands = Observers[i]->Get_Chat_Commands();
		for (int x = Commands.Count()-1;x >= 0;x--) {
			if (Commands[x]->Triggers.ID(CommandHash) != -1) {
				if (Player->Get_DA_Player()->Get_Access_Level() < Commands[x]->AccessLevel) {
					DA::Page_Player(Player->Get_ID(),"You do not have access to use this command.");
				}
				else if (Text.Size() < Commands[x]->Parameters) {
					DA::Page_Player(Player->Get_ID(),"Insufficient parameters. This command requires atleast %d parameter(s), you only supplied %d.",Commands[x]->Parameters,Text.Size());
				}
				else if ((Commands[x]->ChatType & DAChatType::ALL) == DAChatType::ALL || (Type == TEXT_MESSAGE_PUBLIC && (Commands[x]->ChatType & DAChatType::PUBLIC) == DAChatType::PUBLIC) || (Type == TEXT_MESSAGE_TEAM && (Commands[x]->ChatType & DAChatType::TEAM) == DAChatType::TEAM) || (Type == TEXT_MESSAGE_PRIVATE && (Commands[x]->ChatType & DAChatType::PRIVATE) == DAChatType::PRIVATE) || (Type == TEXT_MESSAGE_TMSG && (Commands[x]->ChatType & DAChatType::TMSG) == DAChatType::TMSG)) {
					if (!(Observers[i]->*Commands[x]->Func)(Text,Type)) {
						return false;
					}
				}
				break;
			}
		}
	}
		
	for (int i = EventChatCommands.Count()-1;i >= 0;i--) {
		if (EventChatCommands[i]->Triggers.ID(CommandHash) != -1) {
			if (Player->Get_DA_Player()->Get_Access_Level() < EventChatCommands[i]->AccessLevel) {
				DA::Page_Player(Player->Get_ID(),"You do not have access to use this command.");
			}
			else if (Text.Size() < EventChatCommands[i]->Parameters) {
				DA::Page_Player(Player->Get_ID(),"Insufficient parameters. This command requires atleast %d parameter(s), you only supplied %d.",EventChatCommands[i]->Parameters,Text.Size());
			}
			else if ((EventChatCommands[i]->ChatType & DAChatType::ALL) == DAChatType::ALL || (Type == TEXT_MESSAGE_PUBLIC && (EventChatCommands[i]->ChatType & DAChatType::PUBLIC) == DAChatType::PUBLIC) || (Type == TEXT_MESSAGE_TEAM && (EventChatCommands[i]->ChatType & DAChatType::TEAM) == DAChatType::TEAM) || (Type == TEXT_MESSAGE_PRIVATE && (EventChatCommands[i]->ChatType & DAChatType::PRIVATE) == DAChatType::PRIVATE) || (Type == TEXT_MESSAGE_TMSG && (EventChatCommands[i]->ChatType & DAChatType::TMSG) == DAChatType::TMSG)) {
				if (!(EventChatCommands[i]->Base->*EventChatCommands[i]->Func)(Player,Text,Type)) {
					return false;
				}
			}
			break;
		}
	}

	for (int i = ChatCommands.Count()-1;i >= 0;i--) {
		if (ChatCommands[i]->Triggers.ID(CommandHash) != -1) {
			if (Player->Get_DA_Player()->Get_Access_Level() < ChatCommands[i]->AccessLevel) {
				DA::Page_Player(Player->Get_ID(),"You do not have access to use this command.");
			}
			else if (Text.Size() < ChatCommands[i]->Parameters) {
				DA::Page_Player(Player->Get_ID(),"Insufficient parameters. This command requires atleast %d parameter(s), you only supplied %d.",ChatCommands[i]->Parameters,Text.Size());
			}
			else if ((ChatCommands[i]->ChatType & DAChatType::ALL) == DAChatType::ALL || (Type == TEXT_MESSAGE_PUBLIC && (ChatCommands[i]->ChatType & DAChatType::PUBLIC) == DAChatType::PUBLIC) || (Type == TEXT_MESSAGE_TEAM && (ChatCommands[i]->ChatType & DAChatType::TEAM) == DAChatType::TEAM) || (Type == TEXT_MESSAGE_PRIVATE && (ChatCommands[i]->ChatType & DAChatType::PRIVATE) == DAChatType::PRIVATE) || (Type == TEXT_MESSAGE_TMSG && (ChatCommands[i]->ChatType & DAChatType::TMSG) == DAChatType::TMSG)) {
				if (!ChatCommands[i]->Activate(Player,Text,Type)) {
					return false;
				}
			}
			break;
		}
	}
	return true;
}

bool DAChatCommandManager::Key_Hook_Event(cPlayer *Player,const StringClass &Key) {
	unsigned int KeyHash = Key.GetHash();
	const DynamicVectorClass<DAPlayerObserverClass*> &Observers = Player->Get_DA_Player()->Get_Observers();
	for (int i = Observers.Count()-1;i >= 0;i--) {
		const DynamicVectorClass<DAPlayerObserverChatCommandStruct*> &Commands = Observers[i]->Get_Chat_Commands();
		for (int x = Commands.Count()-1;x >= 0;x--) {
			if (Commands[x]->Triggers.ID(KeyHash) != -1) {
				if (Player->Get_DA_Player()->Get_Access_Level() < Commands[x]->AccessLevel) {
					DA::Page_Player(Player->Get_ID(),"You do not have access to use this command.");
				}
				else {
					(Observers[i]->*Commands[x]->Func)(NullToken,TEXT_MESSAGE_KEYHOOK);
				}
				return false;
			}
		}
		const DynamicVectorClass<DAPlayerObserverKeyHookStruct*> &KeyHooks = Observers[i]->Get_Key_Hooks();
		for (int x = KeyHooks.Count()-1;x >= 0;x--) {
			if (KeyHooks[x]->Triggers.ID(KeyHash) != -1) {
				(Observers[i]->*KeyHooks[x]->Func)();
				return false;
			}
		}
	}
	
	for (int i = EventChatCommands.Count()-1;i >= 0;i--) {
		if (EventChatCommands[i]->Triggers.ID(KeyHash) != -1) {
			if (Player->Get_DA_Player()->Get_Access_Level() < EventChatCommands[i]->AccessLevel) {
				DA::Page_Player(Player->Get_ID(),"You do not have access to use this command.");
			}
			else {
				(EventChatCommands[i]->Base->*EventChatCommands[i]->Func)(Player,NullToken,TEXT_MESSAGE_KEYHOOK);
			}
			return false;
		}
	}
	for (int i = EventKeyHooks.Count()-1;i >= 0;i--) {
		if (EventKeyHooks[i]->Triggers.ID(KeyHash) != -1) {
			(EventKeyHooks[i]->Base->*EventKeyHooks[i]->Func)(Player);
			return false;
		}
	}
	
	for (int i = ChatCommands.Count()-1;i >= 0;i--) {
		if (ChatCommands[i]->Triggers.ID(KeyHash) != -1) {
			if (Player->Get_DA_Player()->Get_Access_Level() < ChatCommands[i]->AccessLevel) {
				DA::Page_Player(Player->Get_ID(),"You do not have access to use this command.");
			}
			else {
				ChatCommands[i]->Activate(Player,NullToken,TEXT_MESSAGE_KEYHOOK);
			}
			return false;
		}
	}
	for (int i = KeyHooks.Count()-1;i >= 0;i--) {
		if (KeyHooks[i]->Triggers.ID(KeyHash) != -1) {
			KeyHooks[i]->Activate(Player);
			return false;
		}
	}

	return true;
}

void DAChatCommandManager::Unregister_Chat_Command(const char *Trigger) {
	unsigned int Hash = StringClass(Trigger).GetHash();
	for (int i = ChatCommands.Count()-1;i >= 0;i--) {
		if (ChatCommands[i]->Triggers.ID(Hash) != -1) {
			delete ChatCommands[i];
			ChatCommands.Delete(i);
		}
	}
	for (int i = EventChatCommands.Count()-1;i >= 0;i--) {
		if (EventChatCommands[i]->Triggers.ID(Hash) != -1) {
			delete EventChatCommands[i];
			EventChatCommands.Delete(i);
		}
	}
}

void DAChatCommandManager::Clear_Event_Chat_Commands(DAEventClass *Base) {
	for (int i = EventChatCommands.Count()-1;i >= 0;i--) {
		if (EventChatCommands[i]->Base == Base) {
			delete EventChatCommands[i];
			EventChatCommands.Delete(i);
		}
	}
}

void DAChatCommandManager::Unregister_Key_Hook(const char *Trigger) {
	unsigned int Hash = StringClass(Trigger).GetHash();
	for (int i = KeyHooks.Count()-1;i >= 0;i--) {
		if (KeyHooks[i]->Triggers.ID(Hash) != -1) {
			delete KeyHooks[i];
			KeyHooks.Delete(i);
		}
	}
	for (int i = EventKeyHooks.Count()-1;i >= 0;i--) {
		if (EventKeyHooks[i]->Triggers.ID(Hash) != -1) {
			delete EventKeyHooks[i];
			EventKeyHooks.Delete(i);
		}
	}
}

void DAChatCommandManager::Clear_Event_Key_Hooks(DAEventClass *Base) {
	for (int i = EventKeyHooks.Count()-1;i >= 0;i--) {
		if (EventKeyHooks[i]->Base == Base) {
			delete EventKeyHooks[i];
			EventKeyHooks.Delete(i);
		}
	}
}

