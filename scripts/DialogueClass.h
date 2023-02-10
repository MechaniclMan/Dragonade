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
#ifndef TT_INCLUDE__DIALOGUECLASS_H
#define TT_INCLUDE__DIALOGUECLASS_H
#include "engine_vector.h"
#include "engine_string.h"
class ChunkSaveClass;
class ChunkLoadClass;
class DialogueOptionClass
{
public:
	float unk1;
	int unk2;
	DialogueOptionClass() : unk1(1), unk2(0)
	{
	}
	virtual ~DialogueOptionClass()
	{
	}
	const DialogueOptionClass& operator=(const DialogueOptionClass& string)
	{
		unk1 = string.unk1;
		unk2 = string.unk2;
		return *this;
	}
	bool Load         (ChunkLoadClass& chunkLoader);
	bool Save         (ChunkSaveClass& chunkSaver);
};
class DialogueClass
{

private:

   DynamicVectorClass<DialogueOptionClass*> DialogueOptions;   // RH7
   float                                    SilenceWeight;


public:

            DialogueClass();
   virtual ~DialogueClass();

   void Free_Options ();
   bool Load         (ChunkLoadClass& chunkLoader);
   bool Save         (ChunkSaveClass& chunkSaver);
   const DialogueClass& operator=(const DialogueClass& string);

}; // size: 32


#endif

