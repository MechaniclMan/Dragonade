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
#ifndef SCRIPTS_INCLUDE__ENGINE_IO_H
#define SCRIPTS_INCLUDE__ENGINE_IO_H
#include "engine_vector.h"
#include "engine_string.h"
#include "chunkclass.h"
class FileClass {
public:
	virtual ~FileClass()
	{
	}
	virtual const char *File_Name() = 0;
	virtual const char *Set_Name(const char* name) = 0;
	virtual bool Create() = 0;
	virtual bool Delete() = 0;
	virtual bool Is_Available(int handle = 0) = 0;
	virtual bool Is_Open() = 0;
	virtual int Open(const char* name, int mode = 1) = 0;
	virtual int Open(int mode = 1) = 0;
	virtual int Read(void* buffer, int size) = 0;
	virtual int Seek(int offset, int origin) = 0;
	virtual int Tell()
	{
		return Seek(0,1);
	}
	virtual int Size() = 0;
	virtual int Write(void* buffer, int size) = 0;
	virtual void Close() = 0;
	virtual unsigned long Get_Date_Time()
	{
		return 0;
	}
	virtual bool Set_Date_Time(unsigned long time)
	{
		return false;
	}
	virtual void Error(int a, int b, const char *c) = 0;
	virtual HANDLE Get_File_Handle()
	{
		return (HANDLE)-1;
	}
	virtual void Bias(int start, int length) = 0;
	virtual bool Is_Hash_Checked() = 0;
};

class FileFactoryClass {
public:
	virtual ~FileFactoryClass() {};
	virtual FileClass* Get_File(const char* Filename) = 0;
	virtual void Return_File(FileClass* File) = 0;
}; // 0004

class Straw;
struct INIEntry : public Node<INIEntry *>
{
public:
	char* Entry;
	char* Value;
	~INIEntry();
};
struct INISection : public Node<INISection *>
{
public:
	char* Section;
	List<INIEntry *> EntryList;
	IndexClass<int,INIEntry *> EntryIndex;
	~INISection();
	inline int Count() {
		return EntryIndex.Count();
	}
	inline INIEntry *Peek_Entry(int Index) {
		return EntryIndex[Index];
	}
};
class SCRIPTS_API INIClass {
	List<INISection *> *SectionList;
	IndexClass<int,INISection *> *SectionIndex;
	char *Filename;
public:
	static int CRC(char *string);
	INIClass();
	INIClass(FileClass &file);
	void Initialize();
	void Shutdown();
	bool Clear(char* section,char* entry);
	int Get_Int(char const *section,char const *entry,int defaultvalue) const;
	uint Get_Color_UInt(char const *section, char const *entry, uint defaultvalue) const;
	float Get_Float(char const *section,char const *entry,float defaultvalue) const;
	bool Get_Bool(char const *section,char const *entry,bool defaultvalue) const;
	int Get_String(char const *section,char const *entry,char const *defaultvalue,char *result,int size) const;
	StringClass &Get_String(StringClass &str, const char* section, const char* entry, const char* default = 0) const;
	WideStringClass &Get_Wide_String(WideStringClass &,char  const*,char  const*,wchar_t  const*) const;
	bool Put_Wide_String(const char* section, const char* entry, const wchar_t* string);
	bool Put_String(const char* section, const char* entry, const char* string);
	bool Put_Int(const char* section, const char* entry, int value, int format);
	bool Put_Bool(const char* section, const char* entry, bool value);
	bool Put_Float(const char* section, const char* entry, float value);
	int Entry_Count(char const *section) const;
	const char *Get_Entry(char const *section,int index) const;
	INIEntry *Find_Entry(const char* section,const char* entry) const;
	virtual ~INIClass();
	int Load(Straw& ffile);
	int Load(FileClass& file);
	int Save(FileClass& file);
	int Section_Count() const
	{
		return SectionIndex->Count();
	}
	const char *Get_File_Name() const {
		return Filename;
	}
	INISection *Get_Section(const char *Section) const {
		for (int i = 0;i < SectionIndex->Count();i++) {
			if (!_stricmp((*SectionIndex)[i]->Section,Section)) {
				return (*SectionIndex)[i];
			}
		}
		return 0;
	}
};

class SCRIPTS_API file_auto_ptr
{

	FileClass* _Ptr;
	FileFactoryClass* _Fac;

public:

	file_auto_ptr(FileFactoryClass* fac, const char* filename);
	~file_auto_ptr();

	FileClass* operator ->() { return _Ptr; }
	operator FileClass *() { return _Ptr; }
	FileClass& operator*() { return *_Ptr; }
	FileClass* get() { return _Ptr; }
};

#define WRITE_MICRO_CHUNK(csave,id,value) \
csave.Begin_Micro_Chunk(id); \
csave.Write(&value,sizeof(value)); \
csave.End_Micro_Chunk();
#if (PARAM_EDITING_ON) || (DDBEDIT)
#define WRITE_SAFE_MICRO_CHUNK(csave,id,value,type) WRITE_MICRO_CHUNK(csave,id,value)
#else
#define WRITE_SAFE_MICRO_CHUNK(csave,id,value,type) \
csave.Begin_Micro_Chunk(id); \
csave.Write(value.Get_Ptr(),sizeof((type)value)); \
csave.End_Micro_Chunk();
#endif
#define WRITE_MICRO_CHUNK_STRING(cload,id,string) \
csave.Begin_Micro_Chunk(id); \
csave.Write((void *)string,strlen(string)+1); \
csave.End_Micro_Chunk();
#define WRITE_MICRO_CHUNK_WWSTRING(csave,id,string) \
csave.Begin_Micro_Chunk(id); \
csave.Write(string.Peek_Buffer(),sizeof(*string.Peek_Buffer())*(string.Get_Length() + 1)); \
csave.End_Micro_Chunk();
#define WRITE_WWSTRING_CHUNK(csave,id,string) \
csave.Begin_Chunk(id); \
csave.Write(string.Peek_Buffer(),string.Get_Length() + 1); \
csave.End_Chunk();
#define WRITE_WIDESTRING_CHUNK(csave,id,string) \
csave.Begin_Chunk(id); \
csave.Write(string.Peek_Buffer(),(string.Get_Length() + 1) * 2); \
csave.End_Chunk();
#define READ_MICRO_CHUNK(cload,id,value) \
case id: \
cload.Read(&value,sizeof(value)); \
break;
#if (PARAM_EDITING_ON) || (DDBEDIT)
#define READ_SAFE_MICRO_CHUNK(csave,id,value,type) READ_MICRO_CHUNK(csave,id,value)
#else
#define READ_SAFE_MICRO_CHUNK(cload,id,value,type) \
case id: \
{ \
type t; \
cload.Read(&t,sizeof(t)); \
value = t; \
} \
break;
#endif
#define READ_MICRO_CHUNK_STRING(cload,id,string,size) \
case id: \
cload.Read(string,cload.Cur_Micro_Chunk_Length()); \
break;
#define READ_MICRO_CHUNK_WWSTRING(cload,id,string) \
case id: \
cload.Read(string.Get_Buffer(cload.Cur_Micro_Chunk_Length()),cload.Cur_Micro_Chunk_Length()); \
break;
#define LOAD_MICRO_CHUNK_WWSTRING(cload,string) \
cload.Read(string.Get_Buffer(cload.Cur_Micro_Chunk_Length()),cload.Cur_Micro_Chunk_Length());
#define READ_WWSTRING_CHUNK(cload,id,string) \
case id: \
cload.Read(string.Get_Buffer(cload.Cur_Chunk_Length()),cload.Cur_Chunk_Length()); \
break;
#define LOAD_MICRO_CHUNK(cload,type) \
cload.Read(&type,sizeof(type));
FileClass SCRIPTS_API *Get_Data_File(const char *file); //Open a file using the mix file opening logic
void SCRIPTS_API Close_Data_File(FileClass *file); //Close a file that was opened with Get_Data_File
INIClass SCRIPTS_API *Get_INI(char const *filename); //Open an INI file (reading using the normal mix file opening logic) and read stuff from it
void SCRIPTS_API Release_INI(INIClass *ini); //Close an INI file opened with Get_INI
unsigned int SCRIPTS_API Get_Registry_Int(const char *entry,int defaultvalue); //Get an int value from the renegade registry key
#ifndef TTLE_EXPORTS
#ifndef WWCONFIG
#ifndef PACKAGEEDITOR
#ifndef EXTERNAL
extern SCRIPTS_API REF_DECL2(_TheFileFactory, FileFactoryClass*);
#endif
#endif
#endif
#endif
const char SCRIPTS_API *Get_File_Path();
const char SCRIPTS_API *Get_App_Data_Path();
const char SCRIPTS_API *Get_Registry_Path();
void SCRIPTS_API Strip_Path_From_Filename(StringClass& target, const char* fileName);
#endif
