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

#include "engine_math.h"
#include "Vector3.h"

int SCRIPTS_API Find_POT_LT(int i)
{
	int x;
	for (x = 1;x <= i;x *= 2)
	{
	}
	return x/2;
}

Vector3 rayPlaneIntersect(const Vector3& rayDirection, const Vector3& planeOrigin)
{
	Vector3 result;
	const float planeDistanceTimesCosAngle = Vector3::Dot_Product(rayDirection, planeOrigin);
	if (planeDistanceTimesCosAngle >= 1.e-6f) // If there is a unique intersection.
	{
		result = rayDirection * (planeOrigin.Length2() / planeDistanceTimesCosAngle);
	}
	else
	{
		result = Vector3(0.f, 0.f, 0.f);
	}
	return result;
}

Vector3 rayPlaneIntersect(const Vector3& rayDirection, const Vector3& planeNormal, const float planeDistance)
{
	Vector3 result;
	const float cosAngle = Vector3::Dot_Product(rayDirection, planeNormal);
	if (cosAngle >= 1.e-6f) // If there is a unique intersection.
	{
		result = rayDirection * (planeDistance / cosAngle);
	}
	else
	{
		result = Vector3(0.f, 0.f, 0.f);
	}
	return result;
}
