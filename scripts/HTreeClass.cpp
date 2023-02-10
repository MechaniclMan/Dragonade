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
#include "Quaternion.h"

HTreeClass::HTreeClass()
{
	Name[0] = '\0';
	NumPivots = 0;
	Pivots = 0;
	ScaleFactor = 1.0f;
}

HTreeClass::HTreeClass(const HTreeClass& that)
{
	memcpy(Name, that.Name, sizeof(Name));
	NumPivots = that.NumPivots;

	Pivots = new PivotClass[NumPivots];
	for (int i = 0; i < NumPivots; ++i)
	{
		PivotClass& pivot = Pivots[i];
		pivot = that.Pivots[i];

		if (pivot.Parent) pivot.Parent = &Pivots[pivot.Parent->Index];
	}

	ScaleFactor = that.ScaleFactor;
}

HTreeClass::~HTreeClass()
{
	Free();
}

HTreeClass* HTreeClass::Create_Interpolated(const HTreeClass* tree_base, const HTreeClass* tree_a, const HTreeClass* tree_b, float weight_a, float weight_b)
{
	assert(tree_base->NumPivots == tree_a->NumPivots);
	assert(tree_base->NumPivots == tree_b->NumPivots);

	HTreeClass* tree_out = new HTreeClass(*tree_base);

	float abs_weight_a = WWMath::Fabs(weight_a);
	float abs_weight_b = WWMath::Fabs(weight_b);
	float abs_weight_base = abs_weight_b + abs_weight_a;

	if (abs_weight_base <= 0.0f || tree_out->NumPivots == 0)
	{
		return tree_out;
	}

	for (int i = 0; i < tree_out->NumPivots; ++i)
	{
		Vector3 pos;
		tree_base->Pivots[i].BaseTransform.Get_Translation(&pos);

		Vector3 pos_a;
		tree_a->Pivots[i].BaseTransform.Get_Translation(&pos_a);

		Vector3 pos_b;
		tree_b->Pivots[i].BaseTransform.Get_Translation(&pos_b);

		Vector3 contrib_a;
		Vector3::Lerp(pos, pos_a, weight_a, &contrib_a);

		Vector3 contrib_b;
		Vector3::Lerp(pos, pos_b, weight_b, &contrib_b);

		pos = contrib_a * abs_weight_a + contrib_b * abs_weight_b;
		
		tree_out->Pivots[i].BaseTransform.Set_Translation(pos / abs_weight_base);
	}

	return tree_out;
}

void HTreeClass::Init_Default()
{
	Free();

	Name[0] = '\0';
	NumPivots = 1;
	Pivots = new PivotClass();
	strcpy_s(Pivots->Name, "RootTransform");
}

WW3DErrorType HTreeClass::Load_W3D(ChunkLoadClass& cload)
{
	Free();

	W3dHierarchyStruct header;

	if (!cload.Open_Chunk()) goto error;
	if (cload.Cur_Chunk_ID() != 257) goto error;
	if (cload.SimpleRead(header) != sizeof(header)) goto error;
	cload.Close_Chunk();

	bool create_root = false;
	if (header.Version < 0x2FFFF)
	{
		create_root = true;
		++header.NumPivots;
	}

	memcpy(Name, header.Name, W3D_NAME_LEN);
	NumPivots = header.NumPivots;

	if (NumPivots > 0)
	{
		Pivots = new PivotClass[NumPivots];
	}

	for (; cload.Open_Chunk(); cload.Close_Chunk())
	{
		if (cload.Cur_Chunk_ID() == 258 && read_pivots(cload, create_root) != WW3D_ERROR_OK)
		{
			Free();
			goto error;
		}
	}

	return WW3D_ERROR_OK;

error:
	return WW3D_ERROR_GENERIC;
}

WW3DErrorType HTreeClass::read_pivots(ChunkLoadClass& cload, bool create_root)
{
	int pidx = 0;
	if (create_root)
	{
		Pivots[0].Index = 0;
		Pivots[0].Parent = nullptr;
		Pivots[0].BaseTransform.Make_Identity();
		Pivots[0].IsVisible = true;
		pidx = 1;
	}

	while (pidx < NumPivots)
	{
		W3dPivotStruct pivot_data;
		if (cload.SimpleRead(pivot_data) != sizeof(pivot_data))
		{
			return WW3D_ERROR_GENERIC;
		}

		PivotClass& pivot = Pivots[pidx];
		memcpy(pivot.Name, pivot_data.Name, W3D_NAME_LEN);
		pivot.Index = pidx;

		Vector3 translation;
		translation.X = pivot_data.Translation.X;
		translation.Y = pivot_data.Translation.Y;
		translation.Z = pivot_data.Translation.Z;
		pivot.BaseTransform.Set_Translation(translation);

		Quaternion rotation;
		rotation.X = pivot_data.Rotation.Q[0];
		rotation.Y = pivot_data.Rotation.Q[1];
		rotation.Z = pivot_data.Rotation.Q[2];
		rotation.W = pivot_data.Rotation.Q[3];

		Matrix3 rotation_matrix = Build_Matrix3(rotation);
		pivot.BaseTransform.Set_Rotation(rotation_matrix);

		if (pivot_data.ParentIdx == -1)
		{
			pivot.Parent = nullptr;
			assert(pidx == 0);
		}
		else
		{
			if (create_root) ++pivot_data.ParentIdx;
			pivot.Parent = &Pivots[pivot_data.ParentIdx];
		}

		++pidx;
	}

	Pivots[0].Transform.Make_Identity();
	Pivots[0].IsVisible = true;

	return WW3D_ERROR_OK;
}

void HTreeClass::Free()
{
	delete[] Pivots;
	Pivots = nullptr;
	NumPivots = 0;
	ScaleFactor = 1.0f;
}

const char* HTreeClass::Get_Name() const
{
	return this->Name;
}

const Matrix3D& HTreeClass::Get_Transform(int pivot) const
{
	return this->Pivots[pivot].Transform;
}

RENEGADE_FUNCTION
void HTreeClass::Get_Bone_Control(int, Matrix3D&) const
AT2(0x005CB180,0x005CAA20);
