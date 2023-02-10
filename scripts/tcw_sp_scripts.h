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
#pragma once

/*
	Usage: 
	Add this script to a GDI harvester.
	Add a 2 way waypath. The start node MUST be at the refinery and the end node MUST be at the tibfield.
	The waypath node count must be >= 3
	Add a zone in the tiberium, that will be the region of roaming in the tiberium.
	The bigger the zone the further it roams...
*/

class tcw_gdi_sp_harvester : public ScriptImpClass
{
public:
	enum State
	{
		NoState             = 0,
      	StateGoingToHarvest = 1,
      	StateHarvesting     = 2,
      	StateGoingToUnload  = 3,
      	StateUnloading      = 4,
	};
	void Register_Auto_Save_Variables();
	State Get_State() { return state; }
protected:
	void Created(GameObject *obj);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason complete_reason);
	void Timer_Expired(GameObject *obj,int number);
	void Detach(GameObject *obj);
	void Harvest();
	void Stop_Harvest();
	State state;
	OBBoxClass tiberiumRegion;
	int WaypathID;
	int WaypathBeginID;
	int WaypathEndID;
	float harvestTime;
	int HarvestIdleID;
	int HarvestFullID;
	int HarvestID;
};