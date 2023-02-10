/*	Renegade Scripts.dll
	Scripts from - http://www.tiberiumcrystalwar.com
	Copyright 2011-2012 MPF

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it 
	under the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code 
	with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#pragma once

#pragma warning(disable: 4239 4706)

class TCW_CP_Cruiser_Countdown_Point : public ScriptImpClass
{
	class VehicleReinforcements
	{
		public:
			VehicleReinforcements()
			{
				CreatePreset = "";
				CreatePos = Vector3(0,0,0);
				CreateFacing = 0;
				CreateTeam = -2;
				PresetID = 0;
			}
			VehicleReinforcements(const char *Preset, Vector3 &Pos, float Facing, int Team)
			{
				CreatePreset = Preset;
				CreatePos = Pos;
				CreateFacing = Facing;
				CreateTeam = Team;
				PresetID = 0;
			}
			GameObject *Is_Alive_And_Kicking()
			{
				GameObject *Preset = Commands->Find_Object(PresetID);
				if(!Preset || Commands->Get_Health(Preset) < 0)
					return 0;
				return Preset;
			}
			void Create()
			{
				GameObject *Preset = 0;
				if(Preset = Is_Alive_And_Kicking())
					Commands->Apply_Damage(Preset, 99999, "BlamoKiller", 0);
				Preset = Commands->Create_Object(CreatePreset.Peek_Buffer(), CreatePos);
				Commands->Set_Facing(Preset, CreateFacing);
				Commands->Set_Player_Type(Preset, CreateTeam);
				StringClass Str;
				Str.Format("%d", CreateTeam);
				Commands->Attach_Script(Preset, "tcw_domination_vehicle", Str.Peek_Buffer());
				PresetID = Commands->Get_ID(Preset);
			}
		protected:
			StringClass CreatePreset;
			Vector3 CreatePos;
			float CreateFacing;
			int CreateTeam;
			int PresetID;
	};
	VehicleReinforcements *Vehs[4];
	~TCW_CP_Cruiser_Countdown_Point()
	{
		for(int Index = 0; Index < 4; Index++)
		{
			delete Vehs[Index];
			Vehs[Index] = 0;
		}
	}
	void Created(GameObject *obj);
	void Damaged(GameObject *obj, GameObject *damager, float damage);
	void Timer_Expired(GameObject *obj, int number);
	int SpawnPadIDs[4];
	int NodTurretIDs[2];
	int GDITurretIDs[2];
	bool Captured;
	int CaptureType;
	float Health;
};

class TCW_CP_Cruiser_Attack : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Action_Complete(GameObject *obj, int action, ActionCompleteReason reason);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	int AttackTeam;
};
