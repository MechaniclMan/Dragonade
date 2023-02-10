/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
//Changes made in DA:
//Added Stop_Observer_Timer, Stop_Custom_Timer, Is_Observer_Timer, and Is_Custom_Timer.
#ifndef TT_INCLUDE_SCRIPTABLEGAMEOBJ_H
#define TT_INCLUDE_SCRIPTABLEGAMEOBJ_H
class ScriptableGameObj;
#include "audiocallback.h"
#include "scripts.h"
#include "ReferencerClass.h"
#include "BaseGameObj.h"
#include "engine_vector.h"
class SoundSceneObjClass;
class LogicalListenerClass;
class LogicalSoundClass;
class DamageableGameObj;
class BuildingGameObj;
class SoldierGameObj;
class ScriptZoneGameObj;

class GameObjObserverTimerClass {
public:
	int ObserverID;
	float Duration;
	int Number;
};

class GameObjCustomTimerClass {
public:
	float Delay;
	ReferencerClass Sender;
	int Message;
	int Param;
};

class ScriptableGameObjDef;
class ScriptableGameObj : public BaseGameObj, public ReferenceableGameObj, public AudioCallbackClass
{
public:
	ScriptableGameObj( void );
	virtual	~ScriptableGameObj( void );
	void	Init( const ScriptableGameObjDef & definition );	
	void	Copy_Settings( const ScriptableGameObjDef & definition );
	void	Re_Init( const ScriptableGameObjDef & definition );
	virtual void	Post_Re_Init( void );
	const ScriptableGameObjDef SCRIPTS_API & Get_Definition( void ) const ;
	virtual void	Set_Delete_Pending (void);
	virtual	bool	Save( ChunkSaveClass & csave );
	virtual	bool	Load( ChunkLoadClass & cload );
	virtual	void	On_Post_Load( void );
	virtual	void	Think();
	virtual	void	Post_Think();
	virtual	void		Get_Position(Vector3 * set_pos) const		= 0;
	SCRIPTS_API void Add_Observer( GameObjObserverClass * observer );
	SCRIPTS_API void Remove_Observer( GameObjObserverClass * observer );
	SCRIPTS_API void Remove_Observer(const char *Name);
	SCRIPTS_API GameObjObserverClass *Find_Observer(const char *Name);
	void Remove_All_Observers(void);
	void Start_Observers( void );
	const SimpleDynVecClass<GameObjObserverClass *> & Get_Observers( void )	{ return Observers; }
	SCRIPTS_API void Insert_Observer( GameObjObserverClass * observer );
	SCRIPTS_API void Start_Observer_Timer(int ObserverID,float Duration,int Number);
	SCRIPTS_API void Start_Custom_Timer(ScriptableGameObj *Sender,float Delay,int Message,int Param);
	SCRIPTS_API void Stop_Observer_Timer(int ObserverID,int Number);
	SCRIPTS_API void Stop_Custom_Timer(ScriptableGameObj *Sender,int Message);
	SCRIPTS_API bool Is_Observer_Timer(int ObserverID,int Number);
	SCRIPTS_API bool Is_Custom_Timer(ScriptableGameObj *Sender,int Message);
	virtual	ScriptableGameObj	*As_ScriptableGameObj( void )	{ return this; };
	virtual	DamageableGameObj	*As_DamageableGameObj( void )	{ return NULL; };
	virtual	BuildingGameObj	*As_BuildingGameObj( void )	{ return NULL; };
	virtual	SoldierGameObj		*As_SoldierGameObj( void )		{ return NULL; };
	virtual	ScriptZoneGameObj	*As_ScriptZoneGameObj( void )	{ return NULL; };
	ReferenceableGameObj *	As_ReferenceableGameObj( void ) { return this; }
	virtual	void	Get_Information( StringClass & string );
	virtual void	On_Sound_Ended( SoundSceneObjClass *sound_obj );
	virtual bool	Is_Always_Dirty( void )				{ return true; }
	virtual void	Export_Creation( BitStreamClass &packet );
	virtual void	Import_Creation( BitStreamClass &packet );

	bool hasObservers() const { return Observers.Count() != 0; }
protected:
	bool															ObserverCreatedPending; // 06E8
	bool															ObserversStarted; //06E9
	SimpleDynVecClass<GameObjObserverClass *>		Observers; // 06EC
	DynamicVectorClass<GameObjObserverTimerClass *>	ObserverTimerList; // 06FC
	DynamicVectorClass<GameObjCustomTimerClass *>	CustomTimerList; // 0714
}; // 072C

#endif