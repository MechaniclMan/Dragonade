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
#ifndef TT_INCLUDE_BASEGAMEOBJ_H
#define TT_INCLUDE_BASEGAMEOBJ_H
#include "NetworkObjectClass.h"
#include "DataSafe.h"
#include "Persist.h"
class PhysicalGameObj;
class VehicleGameObj;
class SmartGameObj;
class ScriptableGameObj;
class DefinitionClass;
class BaseGameObjDef;
class BaseGameObj : public PersistClass, public NetworkObjectClass {
public:
	BaseGameObj( void );
	virtual	~BaseGameObj( void );
	virtual	void					Init( void )											= 0;
	void								Init( const BaseGameObjDef & definition );
	const BaseGameObjDef SCRIPTS_API &		Get_Definition( void ) const ;
	virtual	bool					Save( ChunkSaveClass & csave );
	virtual	bool					Load( ChunkLoadClass & cload );
	virtual	void					Think()				{ IsPostThinkAllowed = true; }
	virtual	void					Post_Think()		{};
	void	Set_ID( int id )				{ Set_Network_ID (id); }
	int	Get_ID( void )	const			{ return Get_Network_ID (); }
	virtual	bool	Is_Hibernating( void )	{ return false; }
	virtual	PhysicalGameObj	*As_PhysicalGameObj( void )	{ return (PhysicalGameObj*)NULL; };
	virtual  VehicleGameObj		*As_VehicleGameObj( void )		{ return (VehicleGameObj *)NULL; }
	virtual	SmartGameObj		*As_SmartGameObj( void )		{ return (SmartGameObj*)NULL; };
	virtual	ScriptableGameObj	*As_ScriptableGameObj( void )	{ return (ScriptableGameObj*)NULL; };
	virtual uint32					Get_Network_Class_ID( void ) const		{ return 0x3E8; }
	virtual void					Delete (void)									{ delete this; }
	bool								Is_Post_Think_Allowed( void )				{ return IsPostThinkAllowed; }
	void								Enable_Cinematic_Freeze( bool enable )	{ EnableCinematicFreeze = enable; }
	bool								Is_Cinematic_Freeze_Enabled( void )		{ return EnableCinematicFreeze; }
private:
	const BaseGameObjDef *	Definition;	// 06BC
	bool							IsPostThinkAllowed; // 06C0
	bool							EnableCinematicFreeze; // 06C1
};// 06C2

#endif