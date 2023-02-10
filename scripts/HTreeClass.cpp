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
#include "HTreeClass.h"

const char* HTreeClass::Get_Name() const
{
   return this->Name;
}

const Matrix3D& HTreeClass::Get_Transform(int pivot) const
{
	return this->Pivot[pivot].Transform;
}

RENEGADE_FUNCTION
void HTreeClass::Get_Bone_Control(int, Matrix3D&) const
AT2(0x005CB180,0x005CAA20);
