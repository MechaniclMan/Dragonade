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
#include "scripts.h"
#include "engine.h"
#include "dp88_securitySystem.h"


// Initialise static variables
listNode<SecuritySystem>* dp88_securitySystem::firstSecuritySystem = 0;
listNode<SecuritySystem>* dp88_securitySystem::lastSecuritySystem = 0;



/************
SecuritySystem Class
*************/

// Constructor
SecuritySystem::SecuritySystem( int _systemID, int _activeTime )
{
	systemID = _systemID;
	activeTime = _activeTime;
	firstSecurityDevice = 0;
	lastSecurityDevice = 0;
}

// Add a new device to this system
SecuritySystem* SecuritySystem::addSecurityDevice( dp88_securitySystem* securityDevice )
{
	listNode<dp88_securitySystem>* newNode = new listNode<dp88_securitySystem>;
	newNode->data = securityDevice;

	if ( firstSecurityDevice )
	{
		lastSecurityDevice->next = newNode;
		lastSecurityDevice = newNode;
	}
	else
	{
		firstSecurityDevice = newNode;
		lastSecurityDevice = newNode;
	}

	return this;
}

// Remove a device from this system
void SecuritySystem::removeSecurityDevice( dp88_securitySystem* securityDevice )
{
	listNode<dp88_securitySystem>* thisNode = firstSecurityDevice;
	while ( thisNode )
	{
		listNode<dp88_securitySystem>* tempNode = firstSecurityDevice;
		thisNode = thisNode->next;

		if ( tempNode->data == securityDevice )
		{
			if ( tempNode->prev )
				tempNode->prev->next = tempNode->next;
			else
				firstSecurityDevice = tempNode->next;

			if ( tempNode->next )
				tempNode->next->prev = tempNode->prev;
			else
				lastSecurityDevice = tempNode->prev;
		}
	}
}

// Trigger all devices within this system
void SecuritySystem::trigger()
{
	listNode<dp88_securitySystem>* thisNode = firstSecurityDevice;
	while ( thisNode )
	{
		thisNode->data->Trigger( activeTime );
		thisNode = thisNode->next;
	}
}




/************
dp88_securitySystem Class
*************/

SecuritySystem* dp88_securitySystem::registerSecurityDevice ( int systemID, dp88_securitySystem* securityDevice )
{
	// Try to find the appropriate system within the already defined systems
	listNode<SecuritySystem>* thisNode = firstSecuritySystem;
	SecuritySystem* system = 0;

	while ( thisNode && !system )
	{
		if ( thisNode->data->systemID == systemID )
			system = thisNode->data;
		thisNode = thisNode->next;
	}

	if ( !system )
	{
		system = new SecuritySystem ( systemID );

		thisNode = new listNode<SecuritySystem>;
		thisNode->data = system;

		if ( firstSecuritySystem )
		{
			lastSecuritySystem->next = thisNode;
			lastSecuritySystem = thisNode;
		}
		else
		{
			firstSecuritySystem = thisNode;
			lastSecuritySystem = thisNode;
		}
	}

	system->addSecurityDevice ( securityDevice );
	return system;
}




/************
dp88_securitySystem_camera Class
*************/

void dp88_securitySystem_camera::Initiated ( GameObject* obj )
{
	Commands->Enable_Enemy_Seen ( obj, true );

	// Create fake target object
	GameObject* fakeTarget = Commands->Create_Object( "Invisible_Object", Commands->Get_Position(obj) );
	fakeTargetId = Commands->Get_ID(fakeTarget);

	startFacing = Commands->Get_Facing ( obj );
	Set_Random_Facing(obj);

	// Start timer to rotate camera
	Commands->Start_Timer ( obj, this, 5.0f, SECURITYSYSTEM_TIMER_ROTATECAMERA );
}

void dp88_securitySystem_camera::_Timer_Expired ( GameObject* obj, int number )
{
	if ( number == SECURITYSYSTEM_TIMER_ROTATECAMERA )
	{
		// Set random facing
		Set_Random_Facing(obj);

		// Start timer to rotate camera
		Commands->Start_Timer ( obj, this, 5.0f, SECURITYSYSTEM_TIMER_ROTATECAMERA );
	}
}

void dp88_securitySystem_camera::Enemy_Seen( GameObject *obj, GameObject *enemy )
{
	//Console_Output ( "Spotted %s!\n", Commands->Get_Preset_Name ( seen ) );
	if ( !activated )
		securitySystem->trigger();
}

void dp88_securitySystem_camera::_Killed ( GameObject *obj, GameObject* killer )
{
	Commands->Destroy_Object ( Commands->Find_Object ( fakeTargetId ) );
}


// Randomly look around between -30 and +30 degrees of base facing
void dp88_securitySystem_camera::Set_Random_Facing ( GameObject* obj )
{
	// Decide on our new facing
	float facing = startFacing + (float)Commands->Get_Random_Int ( -30, 31 );
	//Console_Output ( "Facing %.2f\n", facing );

	// Work out position for target box
	Vector3 pos = Commands->Get_Position( obj );
	pos.X = pos.X + 2 * cos (DEG_TO_RADF(facing));
	pos.Y = pos.Y + 2 * sin (DEG_TO_RADF(facing));

	// Move fake target
	GameObject* fakeTarget = Commands->Find_Object ( fakeTargetId );
	Commands->Set_Position ( fakeTarget, pos );

	// 'attack' the invisible object (Simulates the camera looking around)
	ActionParamsStruct var;
	var.Set_Basic( this, 70, 1 );
	var.Set_Attack( fakeTarget, 0.0, 0.0, true );
	Commands->Action_Attack( obj, var );
}



/************
dp88_securitySystem_defense Class
*************/

void dp88_securitySystem_defense::Initiated ( GameObject* obj )
{
	char ScriptParams[128];
	sprintf ( ScriptParams, "%.1f,%.1f,%.1f", Get_Float_Parameter( "MinAttackDistance" ), Get_Float_Parameter( "MaxAttackDistance" ), Get_Float_Parameter( "AttackTimer" ) );
	Commands->Attach_Script(obj, "JFW_Base_Defence", ScriptParams );

	/* Use a timer to deactivate the defense in .1 seconds ( otherwise
	JFW_Base_Defense will just activate it again automatically */
	Commands->Start_Timer ( obj, this, 0.1f, SECURITYSYSTEM_TIMER_POSTINITIALISE );
}

void dp88_securitySystem_defense::Triggered ( GameObject* obj )
{
	Commands->Enable_Enemy_Seen ( obj, true );
}

void dp88_securitySystem_defense::Deactivated ( GameObject* obj )
{
	Commands->Enable_Enemy_Seen ( obj, false );
}

void dp88_securitySystem_defense::_Timer_Expired ( GameObject* obj, int number )
{
	if ( number == SECURITYSYSTEM_TIMER_POSTINITIALISE )
		Deactivated( obj );
}





// Registrants
ScriptRegistrant<dp88_securitySystem_camera> dp88_securitySystem_camera_Registrant ( "dp88_securitySystem_camera", "systemID=100:int" );
ScriptRegistrant<dp88_securitySystem_defense> dp88_securitySystem_defense_Registrant ( "dp88_securitySystem_defense", "systemID=100:int,MinAttackDistance=0.0:float,MaxAttackDistance=300.0:float,AttackTimer=10.0:float" );