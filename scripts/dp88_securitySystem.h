/*	Renegade Scripts.dll
	Copyright 2014 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#pragma once


#define SECURITYSYSTEM_TIMER_POSTINITIALISE 1450001
#define SECURITYSYSTEM_TIMER_DEACTIVATE 1450002
#define SECURITYSYSTEM_TIMER_ROTATECAMERA 1450003


/* Struct template for linked list nodes */
template <class T> struct listNode
{
	T* data;
	listNode<T>* next;
	listNode<T>* prev;

	listNode() { data = 0; next = 0; prev = 0; };
	~listNode() { if (data != 0) {delete data;} data = 0; };
};


// Forward declaration so we can use dp88_securitySystem inside SecuritySystem */
class dp88_securitySystem;

/* SecuritySystem class contains all data for a specific security system */
class SecuritySystem
{
	public:
		SecuritySystem( int _systemID, int _activeTime = 60 );
		~SecuritySystem () {};

		SecuritySystem* addSecurityDevice( class dp88_securitySystem* securityDevice );
		void removeSecurityDevice( class dp88_securitySystem* securityDevice );
		void trigger();

	private:
		friend class dp88_securitySystem;

		int systemID;
		int activeTime;
		listNode<dp88_securitySystem>* firstSecurityDevice;
		listNode<dp88_securitySystem>* lastSecurityDevice;
};



/* Central class with static data members to contain all required game data
and process input from various sources */
class dp88_securitySystem : public ScriptImpClass
{
	protected:
		friend class SecuritySystem;

		SecuritySystem* registerSecurityDevice ( int systemID, dp88_securitySystem* securityDevice );
		void deregisterSecurityDevice ()
		{
			if ( securitySystem )
				securitySystem->removeSecurityDevice(this);
			securitySystem = 0;
		};
		
		void Created ( GameObject* obj )
		{
			securitySystem = registerSecurityDevice ( Get_Int_Parameter ( "systemID" ), this );
			myObjId = Commands->Get_ID ( obj );
			activated = false;
			Initiated( obj );
		}

		virtual void Initiated ( GameObject* obj ) {};

		void Trigger ( int activeTime )
		{
			Commands->Start_Timer ( Commands->Find_Object(myObjId), this, (float)activeTime, SECURITYSYSTEM_TIMER_DEACTIVATE );
			activated = true;
			Triggered( Commands->Find_Object(myObjId) );
		}

		virtual void Triggered ( GameObject* obj ) {};
		
		void Timer_Expired ( GameObject *obj, int number )
		{
			if ( number == SECURITYSYSTEM_TIMER_DEACTIVATE )
			{
				activated = false;
				Deactivated( obj );
			}
			else
				_Timer_Expired ( obj, number );
		}

		virtual void Deactivated ( GameObject* obj ) {};

		virtual void _Timer_Expired ( GameObject *obj, int number ) {};

		void Killed ( GameObject* obj, GameObject* killer )
		{
			deregisterSecurityDevice();
			_Killed ( obj, killer );
		};
		void Destroyed ( GameObject* obj ) { deregisterSecurityDevice(); };

		virtual void _Killed ( GameObject *obj, GameObject* killer ) {};


		// ID for this GameObject
		int myObjId;

		// Pointer to the security system we belong to
		SecuritySystem* securitySystem;

		// Activation state
		bool activated;

		// Static data members shared by all objects of this class
		static listNode<SecuritySystem>* firstSecuritySystem;
		static listNode<SecuritySystem>* lastSecuritySystem;
};




/* Script for a camera that spots enemies and triggers security systems */
class dp88_securitySystem_camera : public dp88_securitySystem
{
	void Initiated ( GameObject* obj );
	void _Timer_Expired ( GameObject* obj, int number );
	void Enemy_Seen ( GameObject* obj, GameObject* enemy );
	void _Killed ( GameObject *obj, GameObject* killer );

	// Set random facing
	void Set_Random_Facing ( GameObject* obj );

	/* Fake targets used to make camera face the right direction, and
	optionally rotate reguarly to view a different area */
	int fakeTargetId;

	// Starting facing
	float startFacing;
};



/* Script for a turret that attacks enemies when the security system is
triggered */
class dp88_securitySystem_defense : public dp88_securitySystem
{
	void Initiated ( GameObject* obj );
	void Triggered ( GameObject* obj );
	void Deactivated ( GameObject* obj );

	void _Timer_Expired ( GameObject* obj, int number );
};