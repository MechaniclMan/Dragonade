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

#include "SoldierGameObj.h"



class TCWSoldierObj : public ScriptImpClass
{
public:
	TCWSoldierObj():CanCaptureSpikes(false){}
	void Set_CanCaptureSpikes(bool Capture){CanCaptureSpikes=Capture;}
	bool Can_CaptureSpikes(){return CanCaptureSpikes;}
	//void Enable_Refill(){Refill=true;}
	//void Disable_Refill(){Refill=false;}
private:
	void Created(GameObject *obj);
	bool CanCaptureSpikes;
	//bool Refill;
};

TCWSoldierObj *Find_TCWSoldier(GameObject *Soldier);
TCWSoldierObj *Find_TCWSoldier(int ID);
TCWSoldierObj *Find_TCWSoldier_NetworkID(int ID);