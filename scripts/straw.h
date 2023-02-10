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
#ifndef TT_INCLUDE__STRAW_H
#define TT_INCLUDE__STRAW_H

class SCRIPTS_API Straw {
private:
	Straw* ChainTo;
	Straw* ChainFrom;
public:
	Straw();
	virtual ~Straw();
	virtual void Get_From(Straw* straw);
	virtual int Get(void* source,int slen);
};
class SCRIPTS_API Buffer {
private:
	void* BufferPtr;
	long Size;
	bool IsAllocated;
public:
	Buffer(void* buffer,long size);
	void *Get_Buffer()
	{
		return BufferPtr;
	}
	long Get_Size()
	{
		return Size;
	}
	~Buffer();
};

class SCRIPTS_API BufferStraw : public Straw  {
private:
	Buffer BufferPtr;
	int Index;
public:
	BufferStraw(void* buffer, int size);
	~BufferStraw();
	int Get(void* source,int slen);
};

class FileClass;
class SCRIPTS_API FileStraw : public Straw {
private:
	FileClass* File;
	bool HasOpened;
public:
	FileStraw(class FileClass&);
	~FileStraw();
	int Get(void* source,int slen);
};

#endif;