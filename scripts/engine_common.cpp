/*	Renegade Scripts.dll
	Copyright 2011 Tiberian Technologies

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
#include "engine_string.h"
//Don't include console stuff
#ifndef TTLE_EXPORTS
#include "ConPublic.h"
#ifdef NEW_CONSOLE
#include "../console/convarmgr.h"
#include "../Shaders/ConsoleWindow.h"
#endif
#endif //!TTLE_EXPORTS
SCRIPTS_API int Exe = 6;
#ifndef TTLE_EXPORTS

#ifndef NEW_CONSOLE

RENEGADE_FUNCTION
void SCRIPTS_API Console_Output(const char *Output,...)
AT2(0x00428B50,0x00428CD0);

RENEGADE_FUNCTION
void Parse_Console_Input(const char *)
AT2(0x004287E0,0x00428960);

void SCRIPTS_API Console_Input(const char *Input)
{
	if (strlen(Input) > 255)
	{
		return;
	}
	Parse_Console_Input(Input);
}

void ConsoleOutputDirect(const char *Output){
	Console_Output("%s", Output);
}


void Console_OutputV(const char *Output, va_list va){
	StringClass buf;
	buf.Format_Args(Output, va);
	Console_Output("%s", buf.Peek_Buffer());
}


#else //!NEW_CONSOLE

REF_DEF1(ClientDS, bool, 0x0081E33C);

//Use render console on client?

bool UseRenderConsole(){
	return (Exe == 0 && !ClientDS);
}

//Original console output, used by FDS and ClientDS

RENEGADE_FUNCTION
void RenegadeConsoleOutput(const char *Output,...)
AT2(0x00428B50,0x00428CD0);

void RenegadeConsoleOutputV(const char *Output,va_list va){
	CStringBufferA buf;
	buf.vAppendF(Output, va);
	RenegadeConsoleOutput("%s", buf.GetString());
}


//Output to rendered console

void ClientOutputV(const char *Output, va_list va){
	CConsoleWindow::vAppendF(Output, va);
}

//Do nothing

void EmptyOutputV(const char *Output, va_list va){
}

//Initializer forward

void OutputInitializerV(const char *Output, va_list va);

//Console output

void (*Console_OutputV)(const char *Output, va_list va) = OutputInitializerV;


//Output to rendered console

void ClientOutput(const char *Output,...){
	va_list va;
	va_start(va, Output);
	CConsoleWindow::vAppendF(Output, va);
}

//Do nothing

void EmptyOutput(const char *Output,...){
}

//Initializer forward

void OutputInitializer(const char *Output,...);

//Console output

void (*Console_Output)(const char *Output,...) = OutputInitializer;

//Sets the function ptr

void InitializeOutput(){
	if (Exe == 1){
		Console_Output = RenegadeConsoleOutput;
		Console_OutputV = RenegadeConsoleOutputV;
	} else if (UseRenderConsole()){
		Console_Output = ClientOutput;
		Console_OutputV = ClientOutputV;
	} else {
		Console_Output = EmptyOutput;
		Console_OutputV = EmptyOutputV;
	}
}

//Initial function

RENEGADE_FUNCTION
void OutputInitializer(const char *Output,...){
	__asm {
		call InitializeOutput;
		jmp Console_Output;
	}
}

RENEGADE_FUNCTION
void OutputInitializerV(const char *Output, va_list va){
	__asm {
		call InitializeOutput;
		jmp Console_OutputV;
	}
}


//Run this command in console parser

void SCRIPTS_API Console_Input(const char *Input){
	ConsoleManager::ParseInput(Input, CON_ISSUER_SYSTEM);	
}

#endif //NEW_CONSOLE


#endif
SCRIPTS_API int RenegadeAT3VersionSelector()
{
	if (Exe == 6) InitEngine();
	TT_ASSERT(Exe == 0 || Exe == 1 || Exe == 4);
	if (Exe == 4) return 2;
	return Exe;
};

void* HookupAT3x(void* a, void* b, void* c, void* patch_start, void* patch_end, int (*version_selector)())
{
	return HookupAT3(a,b,c,patch_start,patch_end,version_selector);
}

SCRIPTS_API void* HookupAT3(void* a, void* b, void* c, void* patch_start, void* patch_end, int (*version_selector)())
{
	static const char relJumpOp = '\xE9';					
	static const char breakpointOp = '\xCC';

	int versel = version_selector(); 
	void* function = (versel == 0) ? a : ((versel == 1) ? b : ((versel == 2) ? c : 0));
	TT_ASSERT(function != 0);

	char* function_rel = (char*)((char *)function - (char *)patch_start - 5);
	size_t patch_size = (char*) patch_end - (char*) patch_start;
	char* current_addr = (char*) patch_start;

	DWORD oldProtection;
	HANDLE process = GetCurrentProcess();
	VirtualProtectEx(process, patch_start, patch_size, PAGE_EXECUTE_READWRITE, &oldProtection);

	WriteProcessMemory(process, current_addr, &relJumpOp, sizeof(relJumpOp), NULL);
	current_addr += sizeof(relJumpOp);
	WriteProcessMemory(process, current_addr, &function_rel, sizeof(function_rel), NULL);
	current_addr += sizeof(function_rel);
	
	for (; current_addr < patch_end; current_addr += sizeof(breakpointOp))
	{
		WriteProcessMemory(process, current_addr, &breakpointOp, sizeof(breakpointOp), NULL);
	};

	VirtualProtectEx(process, patch_start, patch_size, oldProtection, &oldProtection);
	FlushInstructionCache(process, patch_start, patch_size);

	return function;
};

const unsigned char Code[19] = {0x6A,0x40,0xFF,0x74,0x24,0x0C,0xFF,0x74,0x24,0x0C,0xE8,0xC0,0xFF,0xFF,0xFF,0x83,0xC4,0x0C,0xC3};

#if (_MSC_VER >= 1400)
void AppInvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line,	uintptr_t pReserved )
{
	DebugBreak();
}
#endif
SCRIPTS_API void InitEngine()
{
#ifdef TTINIT_EXPORTS
#if (_MSC_VER >= 1400)
	_set_invalid_parameter_handler(AppInvalidParameterHandler);
#endif
#endif
	if (!memcmp((void *)0x0078CE49, Code, sizeof(Code))) Exe = 0;
	if (!memcmp((void *)0x0078C6E9, Code, sizeof(Code))) Exe = 1;
	if (!memcmp((void *)0x0078790C, Code, sizeof(Code))) Exe = 4;
}
