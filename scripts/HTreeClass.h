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
#ifndef TT_INCLUDE__HTREECLASS_H
#define TT_INCLUDE__HTREECLASS_H
#include "Matrix3D.h"
#include "engine_3dre.h"

struct PivotClass
{
	char Name[16]; //0
	int Index; //16
	PivotClass* Parent; //20
	Matrix3D BaseTransform; //24
	Matrix3D Transform; //72
	bool IsVisible; //120
	bool IsCaptured; //121
	Matrix3D CapTransform; //124
	bool WorldSpaceTranslation; //172

	PivotClass() : Index(0), Parent(0), BaseTransform(true), Transform(true), IsVisible(true), IsCaptured(false), CapTransform(true), WorldSpaceTranslation(false)
	{
	}

	void Capture_Update()
	{
		if (WorldSpaceTranslation)
		{
			Matrix3D CapOrientation = CapTransform;
			CapOrientation[0].W = 0;
			CapOrientation[1].W = 0;
			CapOrientation[2].W = 0;
			Matrix3D::Multiply(Transform,CapOrientation,&Transform);
			Transform[0].W += CapTransform[0].W;
			Transform[1].W += CapTransform[1].W;
			Transform[2].W += CapTransform[2].W;
		}
		else
		{
			Matrix3D::Multiply(Transform,CapTransform,&Transform);
		}
	}
};

class HTreeClass
{
private:
	char Name[16]; //0
	int NumPivots; //16
	PivotClass* Pivots; //20
	float ScaleFactor; //24

public:

	HTreeClass();
	HTreeClass(const HTreeClass& that);
	~HTreeClass();

	static HTreeClass* Create_Interpolated(const HTreeClass* base, const HTreeClass* a, const HTreeClass* b, float weightA, float weightB);

	void			Init_Default();

	WW3DErrorType	Load_W3D(ChunkLoadClass& cload);
	void			Free();


	const char* Get_Name() const;
	const Matrix3D& HTreeClass::Get_Transform(int pivot) const;
	void Get_Bone_Control(int, Matrix3D&) const;

	int Num_Pivots() const
	{
		return NumPivots;
	}

	int Get_Parent_Index(int boneidx) const
	{
		if (Pivots[boneidx].Parent)
		{
			return Pivots[boneidx].Parent->Index;
		}
		else
		{
			return 0;
		}
	}

	void Base_Update(Matrix3D& root)
	{
		Pivots->Transform = root;
		Pivots->IsVisible = true;
		for (int i = 1;i < NumPivots;i++)
		{
			Matrix3D::Multiply(Pivots[i].Parent->Transform,Pivots[i].BaseTransform,&Pivots[i].Transform);
			Pivots[i].IsVisible = true;
			if (Pivots[i].IsCaptured)
			{
				Pivots[i].Capture_Update();
			}
		}
	}

private:
	HTreeClass& operator = (const HTreeClass& that); // unimplemented

	WW3DErrorType read_pivots(ChunkLoadClass& cload, bool create_root);
};

#endif
