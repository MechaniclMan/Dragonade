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

#include "FileFactoryListClass.h"

FileClass* FileFactoryListClass::Get_File
   (const char* fileName)
{
   FileClass* file = 0;
   if (this->tempFactory)
   {
      file = this->tempFactory->Get_File (fileName);
      if (file)
      {
         if (file->Is_Available (0))
            return file;

         this->tempFactory->Return_File (file);
      }
   }

   for (sint32 u = 0; u < this->factories.Count(); ++u)
   {
      FileFactoryClass* factory = this->factories[u];

      file = factory->Get_File (fileName);
      if (!file)
         continue;

      if (file->Is_Available (0))
         return file;

      factory->Return_File (file);
   }

   if (this->factories.Count())
      return this->factories[0]->Get_File (fileName);

   return 0;
}

void FileFactoryListClass::Return_File
   (FileClass* file)
{
   this->factories[0]->Return_File (file);
}

void FileFactoryListClass::Add_FileFactory
   (FileFactoryClass* factory, const char* name)
{
	factories.Add(factory);
	fileNames.Add(name);
	searchStart = 0;
}

void FileFactoryListClass::Remove_FileFactory(FileFactoryClass *factory)
{
	for (int i = 0;i < factories.Count();i++)
	{
		if (factories[i] == factory)
		{
			factories.Delete(i);
			fileNames.Delete(i);
			searchStart = 0;
		}
	}
}

#if !defined(WWCONFIG) && !defined(PACKAGEEDITOR)
#pragma warning(disable: 4073) //warning C4073: initializers put in library initialization area - That's EXACTLY why I put that pragma in...
#pragma init_seg(lib) // Move this files static initializers up a level
#pragma warning(default: 4073)
REF_DEF3(FileFactoryListClass::Instance, FileFactoryListClass *, 0x0085DB70, 0x0085CD58, 0x0085CE48);
#else
FileFactoryListClass *FileFactoryListClass::Instance = 0;
#endif

FileFactoryClass *FileFactoryListClass::Remove_Temp_FileFactory()
{
	FileFactoryClass *f = tempFactory;
	tempFactory = 0;
	return f;
}
void FileFactoryListClass::Add_Temp_FileFactory(FileFactoryClass *factory)
{
	tempFactory = factory;
}



void FileFactoryListClass::Set_Search_Start(const char* factoryName)
{
	for (int factoryIndex = 0; factoryIndex < fileNames.Length(); ++factoryIndex)
	{
		if (_stricmp(fileNames[factoryIndex], factoryName) == 0)
		{
			searchStart = factoryIndex;
			return;
		}
	}
	
	searchStart = 0;
}

FileFactoryClass *FileFactoryListClass::Remove_FileFactory()
{
	FileFactoryClass *factory = 0;
	if (factories.Count())
	{
		factory = factories[0];
		factories.Delete(0);
		fileNames.Delete(0);
	}
	return factory;
}

FileFactoryListClass::FileFactoryListClass() : tempFactory(0), searchStart(0)
{
	Instance = this;
}

FileFactoryListClass::~FileFactoryListClass()
{
	Instance = 0;
}
