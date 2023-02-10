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
#ifndef TT_INCLUDE__SPHERECLASS_H
#define TT_INCLUDE__SPHERECLASS_H

#include "Vector3.h"

class SphereClass
{

public:

   Vector3 Center; // 0000
   float   Radius; // 000C

   SphereClass() {}
   SphereClass(const Vector3& _center, float _radius) : Center(_center), Radius(_radius) {}
	inline void Init(const Vector3 & pos,float radius);

}; // 0010
inline void SphereClass::Init(const Vector3 & pos,float radius)
{
	Center = pos;
	Radius = radius;
}

#endif
