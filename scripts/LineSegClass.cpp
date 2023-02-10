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

#include "LineSegClass.h"



RENEGADE_FUNCTION
void LineSegClass::Set(const LineSegClass& object, const Matrix3D& transform)
AT3(0x00601560,0x00600E00,0x00676DB0);

RENEGADE_FUNCTION
Vector3 LineSegClass::Find_Point_Closest_To(const Vector3 &pos) const
AT3(0x00601840,0x006010E0,0x006770C0);
