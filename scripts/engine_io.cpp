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
#pragma warning(disable: 4073) //warning C4073: initializers put in library initialization area - That's EXACTLY why I put that pragma in...
#pragma init_seg(lib) // Move this files static initializers up a level
#pragma warning(default: 4073)
#include "engine_io.h"
#include "straw.h"
#include "BufferedFileClass.h"
#include "Crc32.h"
#pragma warning (disable:6387)
#include "shlobj.h"
#pragma warning (default:6387)
void SCRIPTS_API Strip_Path_From_Filename(StringClass& target, const char* fileName)
{
	if (strchr(fileName,'\\'))
	{
		const char *c = strrchr(fileName,'\\');
		c++;
		target = c;
	}
	else
	{
		target = fileName;
	}
}
#ifndef TTLE_EXPORTS
#ifndef DDBEDIT
SCRIPTS_API REF_DEF2(_TheFileFactory, FileFactoryClass *, 0x00809E74, 0x0080904C);
FileClass SCRIPTS_API *Get_Data_File(const char *file)
{
	return _TheFileFactory->Get_File(file);
}

void SCRIPTS_API Close_Data_File(FileClass *file)
{
	_TheFileFactory->Return_File(file);
}
char FilePath[MAX_PATH];
char AppDataPath[MAX_PATH];
char RegPath[MAX_PATH];
bool PathsInit = false;
void Read_Paths()
{
	const bool isClient = (Exe == 0);

	char RegBase[MAX_PATH];
	char RegClient[MAX_PATH];
	char RegFDS[MAX_PATH];
	char FileBase[MAX_PATH];
	char FileClient[MAX_PATH];
	char FileFDS[MAX_PATH];

	PathsInit = true;
	bool UseRenFolder = !isClient;
	INIClass *ini = Get_INI("paths.ini");
	if (ini)
	{
		ini->Get_String("paths","RegBase","Westwood",RegBase,MAX_PATH);
		ini->Get_String("paths","RegClient","Renegade",RegClient,MAX_PATH);
		ini->Get_String("paths","RegFDS","RenegadeFDS",RegFDS,MAX_PATH);
		ini->Get_String("paths","FileBase","Renegade",FileBase,MAX_PATH);
		ini->Get_String("paths","FileClient","Client",FileClient,MAX_PATH);
		ini->Get_String("paths","FileFDS","FDS",FileFDS,MAX_PATH);
		UseRenFolder = ini->Get_Bool("paths","UseRenFolder",UseRenFolder);
		Release_INI(ini);
	}
	else
	{
		strcpy(RegBase,"Westwood");
		strcpy(RegClient,"Renegade");
		strcpy(RegFDS,"RenegadeFDS");
		strcpy(FileBase,"Renegade");
		strcpy(FileClient,"Client");
		strcpy(FileFDS,"FDS");
	}
	FilePath[0] = 0;
	if (!UseRenFolder)
	{
		wchar_t fpath[MAX_PATH];
		SHGetSpecialFolderPathW(NULL, fpath, CSIDL_MYDOCUMENTS, FALSE);
		bool unicode = false;
		for (int i = 0;fpath[i] != 0;i++)
		{
			unsigned short value = fpath[i];
			if (value > 255)
			{
				unicode = true;
				break;
			}
		}
		if (unicode)
		{
			GetModuleFileName(NULL,FilePath,MAX_PATH);
			strrchr(FilePath,'\\')[0] = 0;
		}
		else
		{
			SHGetSpecialFolderPath(NULL,  FilePath, CSIDL_MYDOCUMENTS, FALSE);
		}
	}
	else
	{
		GetModuleFileName(NULL,FilePath,MAX_PATH);
		strrchr(FilePath,'\\')[0] = 0;
	}
	strcat(FilePath,"\\");
	strcat(FilePath,FileBase);
	strcat(FilePath,"\\");
	strcat(FilePath,isClient ? FileClient : FileFDS);
	strcat(FilePath,"\\");

	AppDataPath[0] = 0;
	if (!UseRenFolder)
	{
		wchar_t fpath[MAX_PATH];
		SHGetSpecialFolderPathW(NULL, fpath, CSIDL_MYDOCUMENTS, FALSE);
		bool unicode = false;
		for (int i = 0;fpath[i] != 0;i++)
		{
			unsigned short value = fpath[i];
			if (value > 255)
			{
				unicode = true;
				break;
			}
		}
		if (unicode)
		{
			GetModuleFileName(NULL,AppDataPath,MAX_PATH);
			strrchr(AppDataPath,'\\')[0] = 0;
		}
		else
		{
			SHGetSpecialFolderPath(NULL, AppDataPath, CSIDL_APPDATA, FALSE);
		}
	}
	else
	{
		GetModuleFileName(NULL,AppDataPath,MAX_PATH);
		strrchr(AppDataPath,'\\')[0] = 0;
	}
	strcat(AppDataPath,"\\");
	strcat(AppDataPath,FileBase);
	strcat(AppDataPath,"\\");
	strcat(AppDataPath,isClient ? FileClient : FileFDS);
	strcat(AppDataPath,"\\");

	strcpy(RegPath,"Software\\");
	strcat(RegPath,RegBase);
	strcat(RegPath,"\\");
	strcat(RegPath,isClient ? RegClient : RegFDS);
}
const char SCRIPTS_API *Get_File_Path()
{
	if (!PathsInit)
	{
		Read_Paths();
	}
	return FilePath;
}
const char SCRIPTS_API *Get_App_Data_Path()
{
	if (!PathsInit)
	{
		Read_Paths();
	}
	return AppDataPath;
}
const char SCRIPTS_API *Get_Registry_Path()
{
	if (!PathsInit)
	{
		Read_Paths();
	}
	return RegPath;
}
#endif
#endif
unsigned long ChunkSaveClass2::Write(const StringClass& string)
{
	uint16 length = (uint16)string.Get_Length();
	int result = SimpleWrite(length);
	result += Write((char*)string.Peek_Buffer(), length);
	TT_ASSERT(result == (int)sizeof(uint16) + (int)length);
	return result;
}
#ifndef TTLE_EXPORTS
#ifndef DDBEDIT
INIClass SCRIPTS_API *Get_INI(char const *filename)
{
	INIClass *ini = 0;
	FileClass *f = _TheFileFactory->Get_File(filename);
	if (f)
	{
		if (f->Is_Available())
		{
			ini = new INIClass(*f);
		}
		_TheFileFactory->Return_File(f);
	}
	return ini;
}

void SCRIPTS_API Release_INI(INIClass *ini)
{
	if (ini)
	{
		delete ini;
	}
}

int INISection::Count() const {
	return EntryIndex.Count();
}

INIEntry *INISection::Peek_Entry(int Index) {
	return EntryIndex[Index];
}

INIEntry *INISection::Find_Entry(const char *Entry) {
	for (int i = 0;i < Count();i++) {
		if (!_stricmp(Peek_Entry(i)->Entry,Entry)) {
			return Peek_Entry(i);
		}
	}
	return 0;
}

int INIClass::Load(FileClass& file)
{
	FileStraw straw(file);
	if (Filename)
	{
		delete[] Filename;
	}
	Filename = newstr(file.File_Name());
	return Load(straw);
}

int INIClass::Section_Count() const {
	return SectionIndex->Count();
}

const char *INIClass::Get_File_Name() const {
	return Filename;
}
	
INISection *INIClass::Get_Section(const char *Section) const {
	for (int i = 0;i < SectionIndex->Count();i++) {
		if (!_stricmp((*SectionIndex)[i]->Section,Section)) {
			return (*SectionIndex)[i];
		}
	}
	return 0;
}

INIEntry *INIClass::Find_Entry(const char* section,const char* entry) const {
	INISection *S = Get_Section(section);
	if (S) {
		return S->Find_Entry(entry);
	}
	return 0;
}

int INIClass::Get_Int(char const *section,char const *entry,int defaultvalue) const {
	INIEntry *E = Find_Entry(section,entry);
	if (E) {
		return atoi(E->Value);
	}
	return defaultvalue;
}

float INIClass::Get_Float(char const *section,char const *entry,float defaultvalue) const {
	INIEntry *E = Find_Entry(section,entry);
	if (E) {
		return (float)atof(E->Value);
	}
	return defaultvalue;
}

bool INIClass::Get_Bool(char const *section,char const *entry,bool defaultvalue) const {
	INIEntry *E = Find_Entry(section,entry);
	if (E) {
		return (!_stricmp(E->Value,"1") || !_stricmp(E->Value,"true") || !_stricmp(E->Value,"yes"));
	}
	return defaultvalue;
}

int INIClass::Get_String(char const *section,char const *entry,char const *defaultvalue,char *result,int size) const {
	INIEntry *E = Find_Entry(section,entry);
	if (E) {
		strncpy(result,E->Value,size);
		return strlen(result);
	}
	strncpy(result,defaultvalue,size);
	return strlen(result);
}

StringClass &INIClass::Get_String(StringClass &string,const char *section,const char *entry,const char *defaultvalue) const {
	INIEntry *E = Find_Entry(section,entry);
	if (E) {
		string = E->Value;
		return string;
	}
	string = defaultvalue;
	return string;
}

WideStringClass &INIClass::Get_Wide_String(WideStringClass &string,const char *section,const char *entry,const wchar_t *defaultvalue) const {
	INIEntry *E = Find_Entry(section,entry);
	if (E) {
		string = E->Value;
		return string;
	}
	string = defaultvalue;
	return string;
}

RENEGADE_FUNCTION
void INIClass::Initialize()
AT2(0x005DE9E0,0x005DE280);
RENEGADE_FUNCTION
void INIClass::Shutdown()
AT2(0x005DEAA0,0x005DE340);
RENEGADE_FUNCTION
bool INIClass::Clear(char* section,char* entry)
AT2(0x005DEF00,0x005DE7A0);
RENEGADE_FUNCTION
int INIClass::Load(Straw& ffile)
AT2(0x005DF260,0x005DEB00);
RENEGADE_FUNCTION
int INIClass::Entry_Count(char const *section) const
AT2(0x005DFE30,0x005DF6D0);
RENEGADE_FUNCTION
const char *INIClass::Get_Entry(char const *section,int index) const
AT2(0x005E0040,0x005DF8E0);
RENEGADE_FUNCTION
bool INIClass::Put_Wide_String(const char* section, const char* entry, const wchar_t* string)
AT2(0x005E08B0,0x005E0150);
RENEGADE_FUNCTION
bool INIClass::Put_String(const char* section, const char* entry, const char* string)
AT2(0x005E1A10,0x005E12B0);
RENEGADE_FUNCTION
int INIClass::Save(FileClass& file)
AT2(0x005DFA40,0x005DF2E0);
RENEGADE_FUNCTION
bool INIClass::Put_Int(const char* section, const char* entry, int value, int format)
AT2(0x005E1030,0x005E08D0);
RENEGADE_FUNCTION
bool INIClass::Put_Bool(const char* section, const char* entry, bool value)
AT2(0x005E2760,0x005E2000);
RENEGADE_FUNCTION
bool INIClass::Put_Float(const char* section, const char* entry, float value)
AT2(0x005E17F0,0x005E1090);

int INIClass::CRC(char *string)
{
	return CRC_String(string,0);
}

INIClass::INIClass()
{
	Filename = 0;
	Initialize();
}

INIClass::INIClass(FileClass &file)
{
	Filename = 0;
	Initialize();
	Load(file);
}

INIClass::~INIClass()
{
	Clear(0,0);
	Shutdown();
}

uint INIClass::Get_Color_UInt(char const *section, char const *entry, uint defaultvalue) const
{
	char buf[256], hex[10];

	sprintf(buf,"%sHex",entry);
	this->Get_String(section, buf, "0xNotValid", hex, 10); 
	if (strcmp(hex, "0xNotValid") != 0) // We've got us a supposedly valid hex value
	{
		uint color;
		int res = sscanf(hex, "%lx", &color);
		if (res == 1) return color; // Yay, we've got a color. Party!
	};

	uint a, r, g, b;
	sprintf(buf,"%sAlpha",entry);
	a = this->Get_Int(section, buf, (defaultvalue >> 24) & 0xFF);
	sprintf(buf,"%sRed",entry);
	r = this->Get_Int(section, buf, (defaultvalue >> 16) & 0xFF);
	sprintf(buf,"%sGreen",entry);
	g = this->Get_Int(section, buf, (defaultvalue >> 8) & 0xFF);
	sprintf(buf,"%sBlue",entry);
	b = this->Get_Int(section, buf, defaultvalue & 0xFF);

	return ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
};

unsigned int SCRIPTS_API Get_Registry_Int(const char *entry,int defaultvalue)
{
	unsigned int value;
	HKEY key;
	LONG error = RegOpenKeyEx(HKEY_CURRENT_USER,Get_Registry_Path(),0,KEY_READ,&key);
	value = 0;
	unsigned long size = 4;
	unsigned long type;
	error = RegQueryValueEx(key,entry,0,&type,(BYTE *)&value,&size);
	if (error == ERROR_FILE_NOT_FOUND)
	{
		value = defaultvalue;
	}
	RegCloseKey(key);
	return value;
}

file_auto_ptr::file_auto_ptr(FileFactoryClass* fac, const char* filename)
{
	_Ptr = 0;
	_Fac = fac;
	_Ptr = fac->Get_File(filename);
	if (!_Ptr)
	{
		_Ptr = new BufferedFileClass();
	}
}

file_auto_ptr::~file_auto_ptr()
{
	_Fac->Return_File(_Ptr);
}
#endif
#endif