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
#ifndef TT_INCLUDE__CONPUBLIC_H
#define TT_INCLUDE__CONPUBLIC_H

#include <stdio.h>
#include <Windows.h>
//#include "convar.h"

//Uncomment to enable new console system
#ifdef DEBUG
//#define NEW_CONSOLE
#endif //!DEBUG
class CStringBufferA;

#define CON_UNREACHABLE	__assume(false)

#ifdef CONSOLE_EXPORTS
#define CONSOLE_API	__declspec(dllexport)
#else
#define CONSOLE_API __declspec(dllimport)
#endif

CONSOLE_API void ConsoleOutputStr(const char* pStr);
CONSOLE_API void vConsoleOutput(const char* pFmt, va_list va);
CONSOLE_API void ConsoleOutput(const char* pFmt, ...);

CONSOLE_API void ParseConsoleInputStr(const char* pStr);
CONSOLE_API void vParseConsoleInput(const char* pFmt, va_list va);
CONSOLE_API void ParseConsoleInput(const char* pFmt, ...);

CONSOLE_API char* dynsprintf(const char* pFmt, ...);
CONSOLE_API char* dynvsprintf(const char* pFmt, va_list va);
CONSOLE_API void dynfreeprintf(char* pStr);

CONSOLE_API void __strfree(char* pStr);
CONSOLE_API void __strfree2(char* &pStr);
CONSOLE_API char* __strdup(const char* pStr);
CONSOLE_API char* __stralloc(int size);
CONSOLE_API char* __stralloc2(int size);

int TokenizeA(char *pSrc, int slen, char **dest_array, int max_count, const char *extra_ws, const char *extra_quotation, BOOL extended_whitespace, BOOL translate_slashes, BOOL leave_quotation, BOOL NoDefaultWhiteSpace);
int cTokenizeA(const char *pSrc, int slen, const char *extra_ws, const char *extra_quotation, BOOL extended_whitespace, BOOL translate_slashes, BOOL leave_quotation, BOOL NoDefaultWhiteSpace);

class CVar;
class CCommand;


enum ConsoleTextMessageEnum {
	TextMessage_Public,
	TextMessage_Team,
	TextMessage_Private,
	TextMessage_TMSG, //special extra value for the TMSG console command
};

enum {
	CVarCheck_False = 0,
	CVarCheck_True,
	CVarCheck_PrintHelp,
};

#define CON_ISSUER_FIRSTPLAYER		(0)
#define CON_ISSUER_LASTPLAYER		(127)
#define CON_ISSUER_HOSTPLAYER		(-1)
#define CON_ISSUER_SERVER			(-2)
#define CON_ISSUER_SYSTEM			(-3)
#define CON_ISSUER_CONSOLE			(-3)

interface IConsoleEngineInterface {
public:
	virtual bool IAmServer() const = 0;
	virtual int GetGameType() const = 0;
	virtual bool IsSinglePlayer() const = 0;
	virtual bool IsMultiPlayer() const = 0;
	virtual void SendTextSc(const char *string, ConsoleTextMessageEnum mtype, bool amsg, int sender, int target) const = 0;
	virtual void RenPrintf(const char* pFmt, ...) const = 0;
	virtual void RenRemPrintf(const char* pFmt, ...) const = 0;
	virtual void ConsoleOutput(const char* pString) const = 0;
	virtual void ConsoleOutputC(unsigned long color, const char* pString) const = 0;
	virtual void ConsoleOutput(const wchar_t* pString) const = 0;
	virtual void ConsoleOutputC(unsigned long color, const wchar_t* pString) const = 0;

	virtual void ConsoleOutputF(const char* pString, ...) const = 0;
	virtual void ConsoleOutputCF(unsigned long color, const char* pString, ...) const = 0;
	virtual void ConsoleOutputF(const wchar_t* pString, ...) const = 0;
	virtual void ConsoleOutputCF(unsigned long color, const wchar_t* pString, ...) const = 0;

	virtual void ConsoleAppend(const char* pString) const = 0;
	virtual void ConsoleAppend(const wchar_t* pString) const = 0;
	virtual void ConsoleAppendF(const char* pString, ...) const = 0;
	virtual void ConsoleAppendF(const wchar_t* pString, ...) const = 0;

	virtual bool LoadFileToBuffer(const char* pFilePath, CStringBufferA *pBuffer) const = 0;
	virtual bool ConIssuerIsMe(int issuer) const = 0;
	virtual bool ConIssuerHostIsMe(int issuer) const = 0;
	virtual void ReplicateForPlayer(int PlayerID) const = 0;
	virtual void ReplicateForAll() const = 0;
	virtual void ReplicateForPlayer(CVar* pCVar, int PlayerID) const = 0;
	virtual void ReplicateForAll(CVar* pCVar) const = 0;
	virtual void SendReplicatedCommand(const char* pName, const char* pValue) const = 0;
};

interface ICVarMgr {
public:
	virtual int CanChange(CVar* pVar) const = 0;
	virtual bool CanExec(CCommand* pCommand) const = 0;
	virtual void NotifyChanged(CVar* pVar) const = 0;
	virtual void SetConsoleEngineInterface(const IConsoleEngineInterface* pConsoleEngineInterface) const = 0;
	virtual const IConsoleEngineInterface* GetCEIF() const = 0;
	virtual void LevelLoading(const char* pLevelName) const = 0;
	virtual void LevelLoaded(const char* pLevelName) const = 0;
	virtual void LevelUnloading(const char* pLevelName) const = 0;
	virtual void SingleplayerStarted() const = 0;
	virtual void SingleplayerEnded() const = 0;
	virtual void MultiplayerStarted() const = 0;
	virtual void MultiplayerEnded() const = 0;
	virtual void PlayerConnected(int player) const = 0;
};

CONSOLE_API const ICVarMgr* GetCVarMgr();

#endif //!TT_INCLUDE__CONPUBLIC_H