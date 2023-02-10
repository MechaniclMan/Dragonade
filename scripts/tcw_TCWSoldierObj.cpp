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

#include "General.h"
#include "tcw_TCWSoldierObj.h"
#include "tcw_functions.h"
#include "engine.h"

void TCWSoldierObj::Created(GameObject *obj)
{
	if(Is_TCW_Engineer(obj))
		CanCaptureSpikes = true;
}

TCWSoldierObj *Find_TCWSoldier(GameObject *Soldier)
{
	if(Soldier)
	{
		const SimpleDynVecClass<GameObjObserverClass *> *Observers = &((ScriptableGameObj *)Soldier)->Get_Observers();
		for (int i = 0;i < Observers->Count();i++)
		{
			ScriptImpClass *Script = (ScriptImpClass*)(*Observers)[i];
			if(Script && strcmp(Script->Get_Name(), "TCWSoldierObj") == 0)
				return (TCWSoldierObj *)Script;
		}
	}
	return 0;
}

TCWSoldierObj *Find_TCWSoldier(int ID)
{
	GameObject *Soldier = Commands->Find_Object(ID);
	if(Soldier)
	{
		const SimpleDynVecClass<GameObjObserverClass *> *Observers = &((ScriptableGameObj *)Soldier)->Get_Observers();
		for (int i = 0;i < Observers->Count();i++)
		{
			ScriptImpClass *Script = (ScriptImpClass*)(*Observers)[i];
			if(Script && strcmp(Script->Get_Name(), "TCWSoldierObj") == 0)
				return (TCWSoldierObj *)Script;
		}
	}
	return 0;
}

TCWSoldierObj *Find_TCWSoldier_NetworkID(int ID)
{
	GameObject *Soldier = Get_GameObj(ID);
	if(Soldier)
	{
		const SimpleDynVecClass<GameObjObserverClass *> *Observers = &((ScriptableGameObj *)Soldier)->Get_Observers();
		for (int i = 0;i < Observers->Count();i++)
		{
			ScriptImpClass *Script = (ScriptImpClass*)(*Observers)[i];
			if(Script && strcmp(Script->Get_Name(), "TCWSoldierObj") == 0)
				return (TCWSoldierObj *)Script;
		}
	}
	return 0;
}

ScriptRegistrant<TCWSoldierObj> TCWSoldierObj_Registrant("TCWSoldierObj","");