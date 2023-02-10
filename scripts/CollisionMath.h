/*	Renegade Scripts.dll
	Copyright 2015 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef TT_INCLUDE__COLLISIONMATH_H
#define TT_INCLUDE__COLLISIONMATH_H
#include "CastResultStruct.h"
#include "aaboxclass.h"
#include "FrustumClass.h"

class Vector3;
class TriClass;
class MeshTriClass;
class FrustumClass;
class LineSegClass;
class SphereClass;
class OBBoxClass;
class PlaneClass;
class AAPlaneClass;
const float COLLISION_EPSILON = 0.001f;

inline void get_far_extent(const Vector3 & normal,const Vector3 & extent,Vector3 * posfarpt)
{
	if (WWMath::Fast_Is_Float_Positive(normal.X))
	{
		posfarpt->X = extent.X;
	}
	else
	{
		posfarpt->X = -extent.X;
	}
	if (WWMath::Fast_Is_Float_Positive(normal.Y))
	{
		posfarpt->Y = extent.Y;
	}
	else
	{
		posfarpt->Y = -extent.Y;
	}
	if (WWMath::Fast_Is_Float_Positive(normal.Z))
	{
		posfarpt->Z = extent.Z;
	}
	else
	{
		posfarpt->Z = -extent.Z;
	}
}

inline void get_far_extent_branchless(const Vector3& normal, const Vector3& extent, Vector3* posfarpt)
{
	*reinterpret_cast<int*>(&posfarpt->X) = (*(reinterpret_cast<const int*>(&extent.X))) ^ ((*(reinterpret_cast<const int*>(&normal.X))) & 0x80000000);
	*reinterpret_cast<int*>(&posfarpt->Y) = (*(reinterpret_cast<const int*>(&extent.Y))) ^ ((*(reinterpret_cast<const int*>(&normal.Y))) & 0x80000000);
	*reinterpret_cast<int*>(&posfarpt->Z) = (*(reinterpret_cast<const int*>(&extent.Z))) ^ ((*(reinterpret_cast<const int*>(&normal.Z))) & 0x80000000);
}

class SHARED_API CollisionMath
{
public:
	static bool		Intersection_Test(const Vector3& min, const Vector3& max, const LineSegClass& line);
	static bool		Intersection_Test(const AABoxClass & box,const TriClass & tri);	
	static bool		Intersection_Test(const AABoxClass & box,const AABoxClass & box2);
	static bool		Intersection_Test(const AABoxClass & box,const OBBoxClass & box2);	
	static bool		Intersection_Test(const OBBoxClass & box,const TriClass & tri);
	static bool		Intersection_Test(const OBBoxClass & box,const AABoxClass & box2);
	static bool		Intersection_Test(const OBBoxClass & box,const OBBoxClass & box2);
	static bool		Intersection_Test(const SphereClass & sphere,const AABoxClass & box);
	static bool		Intersection_Test(const SphereClass & sphere,const OBBoxClass & box);
	enum OverlapType
	{
		POS				= 0x01,
		NEG				= 0x02,
		ON					= 0x04,
		BOTH				= 0x08,
		OUTSIDE			= POS,
		INSIDE			= NEG,
		OVERLAPPED		= BOTH,
		FRONT				= POS,
		BACK				= NEG
	};
	static OverlapType	Overlap_Test(const AAPlaneClass & plane,const Vector3 & point);
	static OverlapType	Overlap_Test(const AAPlaneClass & plane,const LineSegClass & line);
	static OverlapType	Overlap_Test(const AAPlaneClass & plane,const TriClass & tri);
	static OverlapType	Overlap_Test(const AAPlaneClass & plane,const SphereClass & sphere);
	static OverlapType	Overlap_Test(const AAPlaneClass & plane,const AABoxClass & box);
	static OverlapType	Overlap_Test(const AAPlaneClass & plane,const OBBoxClass & box);
	static OverlapType	Overlap_Test(const PlaneClass & plane,const Vector3 & point)
	{
		float delta = Vector3::Dot_Product(point,plane.N) - plane.D;
		if (delta > COINCIDENCE_EPSILON)
		{
			return POS;
		}
		if (delta < -COINCIDENCE_EPSILON)
		{
			return NEG;
		}
		return ON;
	}
	static OverlapType	Overlap_Test(const PlaneClass & plane,const LineSegClass & line);
	static OverlapType	Overlap_Test(const PlaneClass & plane,const TriClass & tri);
	static OverlapType	Overlap_Test(const PlaneClass & plane,const SphereClass & sphere);
	static OverlapType	Overlap_Test(const PlaneClass & plane,const Vector3 & center,const Vector3 & extent);
	inline static OverlapType	Overlap_Test(const PlaneClass & plane,const AABoxClass & box)
	{
		Vector3 posfarpt;
		Vector3 negfarpt;
		get_far_extent_branchless(plane.N,box.Extent,&posfarpt);
		negfarpt = -posfarpt;
		posfarpt += box.Center;
		negfarpt += box.Center;
		if (Overlap_Test(plane,negfarpt) == POS)
		{
			return POS;
		}
		if (Overlap_Test(plane,posfarpt) == NEG)
		{
			return NEG;
		}
		return BOTH;
	}
	static OverlapType	Overlap_Test(const PlaneClass & plane,const OBBoxClass & box);
	static OverlapType	Overlap_Test(const SphereClass & sphere,const Vector3 & point);
	static OverlapType	Overlap_Test(const SphereClass & sphere,const LineSegClass & line);
	static OverlapType	Overlap_Test(const SphereClass & sphere,const TriClass & tri);
	static OverlapType	Overlap_Test(const SphereClass & sphere,const SphereClass & sphere2);
	static OverlapType	Overlap_Test(const SphereClass & sphere,const AABoxClass & aabox);
	static OverlapType	Overlap_Test(const SphereClass & sphere,const OBBoxClass & obbox);
	static OverlapType	Overlap_Test(const AABoxClass & box,const Vector3 & point)
	{
		if (WWMath::Fabs(point.X - box.Center.X) > box.Extent.X) return POS;
		if (WWMath::Fabs(point.Y - box.Center.Y) > box.Extent.Y) return POS;
		if (WWMath::Fabs(point.Z - box.Center.Z) > box.Extent.Z) return POS;
		return NEG;
	}
	static OverlapType	Overlap_Test(const AABoxClass & box,const LineSegClass & line);
	static OverlapType	Overlap_Test(const AABoxClass & box,const TriClass & tri);
	static OverlapType	Overlap_Test(const AABoxClass & box,const AABoxClass & box2)
	{
		Vector3 dc;
		Vector3::Subtract(box2.Center,box.Center,&dc);
		if (box.Extent.X + box2.Extent.X < WWMath::Fabs(dc.X)) return POS;
		if (box.Extent.Y + box2.Extent.Y < WWMath::Fabs(dc.Y)) return POS;
		if (box.Extent.Z + box2.Extent.Z < WWMath::Fabs(dc.Z)) return POS;
		if ((dc.X + box2.Extent.X <= box.Extent.X) && (dc.Y + box2.Extent.Y <= box.Extent.Y) && (dc.Z + box2.Extent.Z <= box.Extent.Z) && (dc.X - box2.Extent.X >= -box.Extent.X) && (dc.Y - box2.Extent.Y >= -box.Extent.Y) && (dc.Z - box2.Extent.Z >= -box.Extent.Z))
		{
			return NEG;
		}
		return BOTH;
	}
	static OverlapType	Overlap_Test(const AABoxClass & box,const OBBoxClass & obbox);
	static OverlapType	Overlap_Test(const AABoxClass & box,const SphereClass & sphere);
	static OverlapType	Overlap_Test(const OBBoxClass & box,const Vector3 & point);
	static OverlapType	Overlap_Test(const OBBoxClass & box,const LineSegClass & line);
	static OverlapType	Overlap_Test(const OBBoxClass & box,const TriClass & tri);
	static OverlapType	Overlap_Test(const OBBoxClass & box,const AABoxClass & box2);
	static OverlapType	Overlap_Test(const OBBoxClass & box,const OBBoxClass & box2);
	static OverlapType	Overlap_Test(const FrustumClass & frustum,const Vector3 & point);
	static OverlapType	Overlap_Test(const FrustumClass & frustum,const TriClass & tri);
	static OverlapType	Overlap_Test(const FrustumClass & frustum,const SphereClass & sphere);
	static OverlapType	Overlap_Test(const FrustumClass & frustum,const AABoxClass & box);
	static OverlapType	Overlap_Test(const FrustumClass & frustum,const OBBoxClass & box);
	static OverlapType	Overlap_Test(const FrustumClass & frustum,const AABoxClass & box,int & planes_passed);
	static OverlapType	Overlap_Test(const FrustumClass & frustum,const OBBoxClass & box,int & planes_passed);

	static OverlapType	Overlap_Test(const Vector3 & min,const Vector3 & max,const LineSegClass & line)
	{
		AABoxClass box;
		box.Init_Min_Max(min,max);
		return CollisionMath::Overlap_Test(box,line);
	}
	
	static bool		Collide(const LineSegClass & line,const AAPlaneClass & plane,CastResultStruct * result);
	static bool		Collide(const LineSegClass & line,const PlaneClass & plane,CastResultStruct * result);
	static bool		Collide(const LineSegClass & line,const TriClass & tri,CastResultStruct * result);
	static bool		Collide(const LineSegClass & line,const MeshTriClass & tri,CastResultStruct * result);
	static bool		CollideTwoSided(const LineSegClass& line, const MeshTriClass& tri, CastResultStruct* result);
	static bool		Collide(const LineSegClass & line,const SphereClass & sphere,CastResultStruct * result);
	static bool		Collide(const LineSegClass & line,const AABoxClass & box,CastResultStruct * result);
	static bool		Collide(const LineSegClass & line,const OBBoxClass & box,CastResultStruct * result);
	static bool		Collide(const AABoxClass & box,const Vector3 & move,const PlaneClass & plane,CastResultStruct * result);
	static bool		Collide(const AABoxClass & box,const Vector3 & move,const TriClass & tri,CastResultStruct * result);
	static bool		Collide(const AABoxClass & box,const Vector3 & move,const AABoxClass & box2,CastResultStruct * result);
	static bool		Collide(const AABoxClass & box,const Vector3 & move,const OBBoxClass & box2,const Vector3 & move2,CastResultStruct * result);
	static bool		Collide(const OBBoxClass & box,const Vector3 & move,const PlaneClass & plane,CastResultStruct * result);
	static bool		Collide(const OBBoxClass & box,const Vector3 & move,const TriClass & tri,const Vector3 & move2,CastResultStruct * result);
	static bool		Collide(const OBBoxClass & box,const Vector3 & move,const AABoxClass & box2,const Vector3 & move2,CastResultStruct * result);
	static bool		Collide(const OBBoxClass & box,const Vector3 & move,const OBBoxClass & box2,const Vector3 & move2,CastResultStruct * result);
private:
	static OverlapType eval_overlap_mask(int mask)
	{
		if (mask == ON)
		{
			return ON;
		}
		if ((mask & ~(POS | ON)) == 0)
		{
			return POS;
		}
		if ((mask & ~(NEG | ON)) == 0)
		{
			return NEG;
		}
		return BOTH;
	}
	static OverlapType eval_overlap_collision(const CastResultStruct & res)
	{
		if (res.Fraction < 1.0f)
		{
			return BOTH;
		}
		else
		{
			if (res.StartBad)
			{
				return NEG;
			}
			else
			{
				return POS;
			}
		}
	}
	static const float COINCIDENCE_EPSILON;
};

#endif
