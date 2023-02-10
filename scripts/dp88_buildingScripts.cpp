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
// Include files
#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "dp88_custom_timer_defines.h"
#include "dp88_buildingScripts.h"
#include "definition.h"
#include "BaseControllerClass.h"
#include "WarFactoryGameObj.h"
#include "AirFactoryGameObj.h"
#include "NavalFactoryGameObj.h"
#include "RefineryGameObj.h"
#include "PurchaseSettingsDefClass.h"
#include "PhysicalGameObj.h"
#include "ArmorWarheadManager.h"

/*------------------------
Building Controller Script
--------------------------*/

void dp88_buildingScripts_buildingController::Created ( GameObject *obj )
{
	numChildren = 0;
	children = NULL;
	buildingOnline = true;

	team0_buildingOfflineSoundId = 0;
	team1_buildingOfflineSoundId = 0;
	team0_buildingOnlineSoundId = 0;
	team1_buildingOnlineSoundId = 0;

	/* Get string IDs and for each one determine if it has a valid sound
	attached to it */
	unsigned long soundId;
	team0_buildingOfflineStringId = (Is_Valid_String_ID(Get_Int_Parameter("team0_buildingOfflineStringId"))) ? Get_Int_Parameter("team0_buildingOfflineStringId") : 0;
	if ( team0_buildingOfflineStringId && (soundId = Get_String_Sound_ID(team0_buildingOfflineStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		team0_buildingOfflineSoundId = soundId;

	team1_buildingOfflineStringId = (Is_Valid_String_ID(Get_Int_Parameter("team1_buildingOfflineStringId"))) ? Get_Int_Parameter("team1_buildingOfflineStringId") : 0;
	if ( team1_buildingOfflineStringId && (soundId = Get_String_Sound_ID(team1_buildingOfflineStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		team1_buildingOfflineSoundId = soundId;

	team0_buildingOnlineStringId = (Is_Valid_String_ID(Get_Int_Parameter("team0_buildingOnlineStringId"))) ? Get_Int_Parameter("team0_buildingOnlineStringId") : 0;
	if ( team0_buildingOnlineStringId && (soundId = Get_String_Sound_ID(team0_buildingOnlineStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		team0_buildingOnlineSoundId = soundId;

	team1_buildingOnlineStringId = (Is_Valid_String_ID(Get_Int_Parameter("team1_buildingOnlineStringId"))) ? Get_Int_Parameter("team1_buildingOnlineStringId") : 0;
	if ( team1_buildingOnlineStringId && (soundId = Get_String_Sound_ID(team1_buildingOnlineStringId)) != 0 && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000 )
		team1_buildingOnlineSoundId = soundId;
}


void dp88_buildingScripts_buildingController::Destroyed ( GameObject *obj )
{
	// Clean up memory if it is still allocated
	if ( children != NULL )
	{
		delete [] children;
		children = NULL;
		numChildren = 0;
	}
}


void dp88_buildingScripts_buildingController::Killed ( GameObject* obj, GameObject* killer )
{
	// Do we have any children?
	if ( children != NULL )
	{
		// Yes? OK, send building destroyed custom to all children
		for ( int i = 0; i < numChildren; i++ )
		{
			GameObject* child = Commands->Find_Object ( children[i].objectId );

			if ( child != NULL )
				Commands->Send_Custom_Event ( obj, child, CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED, 0, 0.0f );
		}

		// At this point we no longer need to store information about our children so erase the array
		delete [] children;
		children = NULL;
		numChildren = 0;
	}
}


void dp88_buildingScripts_buildingController::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Child object registrations
	if ( type == CUSTOM_BUILDINGSCRIPTS_REGISTERCHILD && sender != NULL )
	{
		// Get ID of child
		int childId = Commands->Get_ID ( sender );
		if (findChild(childId) == -1)
		{
			// Does child array exist?
			if ( children != NULL )
			{
				// It does, add this to the end of the list
				ChildData* oldChildren = children;
				children = new ChildData[++numChildren];
	
				// Copy old data across
				for ( int i = 0; i < numChildren-1; i++ )
					children[i] = oldChildren[i];
	
				// Store data for new child
				children[numChildren-1].objectId = childId;
				children[numChildren-1].type = param;
	
				// Cleanup old array
				delete [] oldChildren;
			}
	
			// Ok, no child array, create one
			else
			{
				children = new ChildData[++numChildren];
				children[0].objectId = childId;
				children[0].type = param;
			}
		}
	}




	// Child object de-registrations
	else if ( type == CUSTOM_BUILDINGSCRIPTS_UNREGISTERCHILD && sender != NULL )
	{
		// Get ID of child and find them in the data array
		int childId = Commands->Get_ID ( sender );
		int childIndex = findChild( childId );

		// If we found the child in the array we need to remove it
		if ( childIndex != -1 )
		{
			// If there is only one child then simply erase the array
			if ( numChildren == 1 && childIndex == 0 )
			{
				delete [] children;
				children = NULL;
				numChildren = 0;
			}

			// Otherwise create a new, smaller, array and copy over data for all other children
			else
			{
				ChildData* oldChildren = children;
				children = new ChildData[--numChildren];

				// Copy data across
				for ( int i = 0, j = 0; i < numChildren; i++, j++ )
				{
					// If this is the index to skip decrement j so that subsequent items
					// are moved one position forwards and ignore this item
					if ( i == childIndex )
						j--;
					else
						children[j] = oldChildren[i];
				}
			}
		}
	}




	// Child object destruction report
	else if ( type == CUSTOM_BUILDINGSCRIPTS_CHILDDESTROYED && sender != NULL )
	{
		// Find the child in the array
		int childId = Commands->Get_ID ( sender );
		int childIndex = findChild( childId );

		// If this is a minor weakpoint check to see if all other minor weakpoints (if any) are
		// destroyed. If they are then the building is now offline (if it was not before)
		if ( children[childIndex].type == BUILDING_CHILD_TYPE_MINOR_WEAKPOINT )
		{
			children[childIndex].destroyed = true;

			// If the building is not already offline (one weakpoint may have been repaired and
			// destroyed again before building came online) and all weakpoints are destroyed then
			// we set the building to offline.
			if ( buildingOnline )
			{
				bool allWeakpointsDestroyed = true;
				for ( int i = 0; i < numChildren; i++ )
				{
					// If this is a live minor weakpoint then not all weakpoints are destroyed, set flag
					// to false and break (only need to find one to disprove hypothesis)
					if ( children[i].type == BUILDING_CHILD_TYPE_MINOR_WEAKPOINT && children[i].destroyed != true )
					{
						allWeakpointsDestroyed = false;
						break;
					}
				}

				// If all weakpoints are destroyed then the building is now offline. Send the building offline
				// custom to the child who caused us to go offline with param 1 (to apply the effect) and also
				// to other children to notify them of the change of state with param 0 (do not apply effects)
				if ( allWeakpointsDestroyed )
				{
					buildingOnline = false;
					
					// Notify ourselves to trigger building functions to take action on it
					Commands->Send_Custom_Event ( obj, obj, CUSTOM_BUILDINGSCRIPTS_BUILDINGOFFLINE, param, 0.0f );

					// Notify sender to perform their weakpoint action (this will eventually be subsumed by the
					// building effects mechanism)
					Commands->Send_Custom_Event ( obj, sender, CUSTOM_BUILDINGSCRIPTS_BUILDINGOFFLINE, 1, 0.0f );

					for ( int i = 0; i < numChildren; i++ )
					{
						if ( i != childIndex )
						{
							GameObject* child = Commands->Find_Object ( children[i].objectId );
							if ( child != NULL )
								Commands->Send_Custom_Event ( obj, sender, CUSTOM_BUILDINGSCRIPTS_BUILDINGOFFLINE, 0, 0.0f );
						}
					}

					/* Send messages to teams */
					if ( team0_buildingOfflineStringId )
					{
						const char *str = Get_Translated_String(team0_buildingOfflineStringId);
						Send_Message_Team(0,255,255,255,str);
						delete[] str;
						if ( team0_buildingOfflineSoundId )
							Create_2D_Sound_Team(Get_Definition_Name(team0_buildingOfflineSoundId),0);
					}

					if ( team1_buildingOfflineStringId )
					{
						const char *str = Get_Translated_String(team1_buildingOfflineStringId);
						Send_Message_Team(1,255,255,255,str);
						delete[] str;
						if ( team1_buildingOfflineSoundId )
							Create_2D_Sound_Team(Get_Definition_Name(team1_buildingOfflineSoundId),1);
					}
				}
			}
		}
		
		
		
		
		// If this was a major weakpoint then, if all other major weakpoints are also destroyed, the
		// entire building is destroyed
		else if ( children[childIndex].type == BUILDING_CHILD_TYPE_MAJOR_WEAKPOINT )
		{
			children[childIndex].destroyed = true;

			// If all other major weakpoints are destroyed then the building is destroyed
			bool allWeakpointsDestroyed = true;
			for ( int i = 0; i < numChildren; i++ )
			{
				// If this is a live major weakpoint then not all weakpoints are destroyed, set flag
				// to false and break (only need to find one to disprove hypothesis)
				if ( children[i].type == BUILDING_CHILD_TYPE_MAJOR_WEAKPOINT && children[i].destroyed != true )
				{
					allWeakpointsDestroyed = false;
					break;
				}
			}


			// If all major weakpoints are destroyed then kill the building (using the GameObject whose ID
			// matches param, to give credit to the appropriate killer)
			if ( allWeakpointsDestroyed )
			{
				// Inform all children that we are dead
				for ( int i = 0; i < numChildren; i++ )
				{
					GameObject* child = Commands->Find_Object ( children[i].objectId );
					if ( child != NULL )
						Commands->Send_Custom_Event ( obj, sender, CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED, 0, 0.0f );
				}

				// And now die...
				GameObject* killer = Commands->Find_Object ( param );
				if ( killer != NULL )
					Commands->Apply_Damage ( obj, 1000.0f, "Death", killer );
				else
					Commands->Apply_Damage ( obj, 1000.0f, "Death", obj );
			}
		}
		
		
		
		
		// If this is any other type of weakpoint then just mark them as destroyed
		else
		{
			children[childIndex].destroyed = true;
		}
	}




	// Child object repaired report
	else if ( type == CUSTOM_BUILDINGSCRIPTS_CHILDREPAIRED && sender != NULL )
	{
		// Find the child in the array
		int childId = Commands->Get_ID ( sender );
		int childIndex = findChild( childId );

		// If this is a minor weakpoint check to see if all other minor weakpoints (if any) are
		// destroyed. If they are then the building is now online (if it was not before)
		if ( children[childIndex].type == BUILDING_CHILD_TYPE_MINOR_WEAKPOINT )
		{
			children[childIndex].destroyed = false;

			// If the building is not already online (one weakpoint may have been destroyed and
			// repaired again without the building going offline) and all weakpoints are repaired
			// then we set the building to online.
			if ( buildingOnline != true )
			{
				bool allWeakpointsRepaired = true;
				for ( int i = 0; i < numChildren; i++ )
				{
					// If this is a dead minor weakpoint then not all weakpoints are repaired, set flag
					// to false and break (only need to find one to disprove hypothesis)
					if ( children[i].type == BUILDING_CHILD_TYPE_MINOR_WEAKPOINT && children[i].destroyed == true )
					{
						allWeakpointsRepaired = false;
						break;
					}
				}

				// If all weakpoints are repaired then the building is now online. Send the building online
				// custom to the child who caused us to go online with param 1 (to apply the effect) and also
				// to other children to notify them of the change of state with param 0 (do not apply effects)
				if ( allWeakpointsRepaired )
				{
					buildingOnline = true;

					// Notify ourselves to trigger building functions to take action on it
					Commands->Send_Custom_Event ( obj, obj, CUSTOM_BUILDINGSCRIPTS_BUILDINGONLINE, param, 0.0f );

					// Notify sender to perform their weakpoint action (this will eventually be subsumed by the
					// building effects mechanism)
					Commands->Send_Custom_Event ( obj, sender, CUSTOM_BUILDINGSCRIPTS_BUILDINGONLINE, 1, 0.0f );

					for ( int i = 0; i < numChildren; i++ )
					{
						if ( i != childIndex )
						{
							GameObject* child = Commands->Find_Object ( children[i].objectId );
							if ( child != NULL )
								Commands->Send_Custom_Event ( obj, sender, CUSTOM_BUILDINGSCRIPTS_BUILDINGONLINE, 0, 0.0f );
						}
					}

					/* Send messages to teams */
					if ( team0_buildingOnlineStringId )
					{
						const char *str = Get_Translated_String(team0_buildingOnlineStringId);
						Send_Message_Team(0,255,255,255,str);
						delete[] str;
						if ( team0_buildingOnlineSoundId )
							Create_2D_Sound_Team(Get_Definition_Name(team0_buildingOnlineSoundId),0);
					}

					if ( team1_buildingOnlineStringId )
					{
						const char *str = Get_Translated_String(team1_buildingOnlineStringId);
						Send_Message_Team(1,255,255,255,str);
						delete[] str;
						if ( team1_buildingOnlineSoundId )
							Create_2D_Sound_Team(Get_Definition_Name(team1_buildingOnlineSoundId),1);
					}
				}
			}
		}
		

		// If this is any other type of weakpoint then just mark them as not destroyed
		else
		{
			children[childIndex].destroyed = false;
		}
	}




	// Building captured notification sent by a capture point
	else if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED && sender != NULL )
	{
		// Check the sender is one of our children
		int childIndex = findChild( Commands->Get_ID (sender) );
		if ( childIndex < 0 )
			return;

		// The building has been captured by another team, set the new object type
		if ( Get_Object_Type(obj) != param )
		{
			Set_Object_Type(obj,param);

			// Mirror the message to ourselves to trigger building functions scripts
			// to take action on it (they ignore messages not from the the same object)
			Commands->Send_Custom_Event ( obj, obj, CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED, param, 0.0f );

			// Notify all children of the change of ownership
			for ( int i = 0; i < numChildren; i++ )
			{
				GameObject* child = Commands->Find_Object ( children[i].objectId );
				if ( child != NULL )
					Commands->Send_Custom_Event ( obj, sender, CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED, param, 0.0f );
			}
		}
	}
}


// Finds the position of the given object ID in the children array, otherwise returns -1 if it
// is not found
int dp88_buildingScripts_buildingController::findChild ( int objectId )
{
	// Do we have any children to search?
	if ( children != NULL )
	{
		// Does this object ID exist in the array?
		for ( int i = 0; i < numChildren; i++ )
		{
			// Is this the right child? If so then return this index as the result
			if ( children[i].objectId == objectId )
				return i;
		}
	}

	// Not found
	return -1;
}




/*------------------------
Building Repair Node
--------------------------*/

void dp88_buildingScripts_repairNode::Created ( GameObject *obj )
{
	// Find parent
	GameObject* parent = Find_Object_By_Preset ( Get_Object_Type ( obj ), Get_Parameter("buildingControllerPreset") );
	if ( parent != NULL )
	{
		parentId = Commands->Get_ID ( parent );

		// Send with 1 second delay to ensure parent has been initialized
		Commands->Send_Custom_Event ( obj, parent, CUSTOM_BUILDINGSCRIPTS_REGISTERCHILD, BUILDING_CHILD_TYPE_GENERIC, 1.0f );

		// Attach link health script - no point cloning its functionality here
		char scriptParams[20];
		sprintf_s ( scriptParams, sizeof(scriptParams), "%d", Commands->Get_ID(parent) );
		Commands->Attach_Script(obj,"dp88_linkHealth",scriptParams);
	}
	else
	{
		Console_Output ( "[%d:%s:%s] Critical Error: Building controller '%s' not found. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("buildingControllerPreset") );
		Destroy_Script();
	}
}


void dp88_buildingScripts_repairNode::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Parent building destroyed, spawn dead preset
	if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED && Commands->Get_ID(sender) == parentId )
	{
		// Spawn dead preset
		if ( Is_Valid_Preset ( Get_Parameter("dead_preset") ) )
		{
			GameObject* dead = Commands->Create_Object ( Get_Parameter("dead_preset"), Commands->Get_Position(obj) );
			Commands->Set_Facing ( dead, Commands->Get_Facing(obj) );
		}

		// And kill ourself
		Commands->Apply_Damage ( obj, 1000.0f, "Death", obj );
	}
}




/*------------------------
Weakpoint - Destroy Building Script
--------------------------*/

void dp88_buildingScripts_weakpoint_destroyBuilding::Created ( GameObject *obj )
{
	// Find parent
	GameObject* parent = Find_Object_By_Preset ( Get_Object_Type ( obj ), Get_Parameter("buildingControllerPreset") );
	if ( parent != NULL )
	{
		parentId = Commands->Get_ID ( parent );

		// Send with 1 second delay to ensure parent has been initialized
		Commands->Send_Custom_Event ( obj, parent, CUSTOM_BUILDINGSCRIPTS_REGISTERCHILD, BUILDING_CHILD_TYPE_MAJOR_WEAKPOINT, 1.0f );
	}
	else
	{
		Console_Output ( "[%d:%s:%s] Critical Error: Building controller '%s' not found. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("buildingControllerPreset") );
		Destroy_Script();
	}
}


void dp88_buildingScripts_weakpoint_destroyBuilding::Killed ( GameObject* obj, GameObject* killer )
{
	// This might not be the only major weakpoint so send a custom to the building controller to
	// let it know we are dead and it can decide what to do... include the killers ID so they are
	// credited with destroying the building
	GameObject* parent = Commands->Find_Object ( parentId );
	if ( parent != NULL )
		Commands->Send_Custom_Event ( obj, parent, CUSTOM_BUILDINGSCRIPTS_CHILDDESTROYED, Commands->Get_ID(killer), 0.0f );
	
	// Spawn dead preset
	if ( Is_Valid_Preset ( Get_Parameter("dead_preset") ) )
	{
		GameObject* dead = Commands->Create_Object ( Get_Parameter("dead_preset"), Commands->Get_Position(obj) );
		Commands->Set_Facing ( dead, Commands->Get_Facing(obj) );
	}
}

void dp88_buildingScripts_weakpoint_destroyBuilding::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Parent building now dead, set parentId to -1 to prevent Find_Object returning a non-NULL result and
	// potentially causing a pure virtual function call when we try to send the dead building a custom
	if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED && Commands->Get_ID(sender) == parentId )
		parentId = -1;
}




/*------------------------
Base Class For Generic Weakpoints
--------------------------*/

void MinorWeakpointScriptClass::Created ( GameObject *obj )
{
	// Set variables
	isDestroyed = false;
	m_pLoopedAnimCtrl = NULL;
	// Find parent
	GameObject* parent = Find_Object_By_Preset ( Get_Object_Type ( obj ), Get_Parameter("buildingControllerPreset") );
	if ( parent != NULL )
	{
		parentId = Commands->Get_ID ( parent );

		// Send with 1 second delay to ensure parent has been initialized
		Commands->Send_Custom_Event ( obj, parent, CUSTOM_BUILDINGSCRIPTS_REGISTERCHILD, BUILDING_CHILD_TYPE_MINOR_WEAKPOINT, 1.0f );
		m_pLoopedAnimCtrl = new LoopedAnimationController(obj);
		// Set animation
		if ( strlen(Get_Parameter("animName")) > 0 )
		{
			m_pLoopedAnimCtrl->PlayAnimation(Get_Parameter("animName"),Get_Int_Parameter("animStartFrame"),Get_Int_Parameter("animEndFrame"),!Get_Int_Parameter("animLoop"));
		}
	}
	else
	{
		Console_Output ( "[%d:%s:%s] Critical Error: Building controller '%s' not found. Destroying weakpoint script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("buildingControllerPreset") );
		Destroy_Script();
	}
}


void MinorWeakpointScriptClass::Damaged ( GameObject *obj, GameObject *damager, float amount )
{
	/* If we are repairable, not destroyed and have 0 shield strength then
	disable soldier purchases and set destroyed flag. */
	if ( !isDestroyed && Commands->Get_Shield_Strength(obj) == 0 )
	{
		isDestroyed = true;

		// Send message to game controller indicating we have been destroyed. If this is the
		// last minor weakpoint to be destroyed our parent will send us a custom indicating
		// the building is now offline and instructing us to apply our weakpoint effect
		GameObject* parent = Commands->Find_Object ( parentId );
		if ( parent != NULL )
			Commands->Send_Custom_Event ( obj, parent, CUSTOM_BUILDINGSCRIPTS_CHILDDESTROYED, 0, 0.0f );

		// Set animation
		if ( strlen(Get_Parameter("destroyedAnimName")) > 0 )
		{
			m_pLoopedAnimCtrl->PlayAnimation(Get_Parameter("destroyedAnimName"),Get_Int_Parameter("destroyedAnimStartFrame"),Get_Int_Parameter("destroyedAnimEndFrame"),!Get_Int_Parameter("destroyedAnimLoop"));
		}
		else
		{
			m_pLoopedAnimCtrl->StopAnimation();
		}
	}


	/* If we are repairable, destroyed and have been repaired to 100% health then
	re-enable soldier purchases and unset destroyed flag */
	if ( isDestroyed && amount <= 0.0f && Commands->Get_Shield_Strength(obj) == Commands->Get_Max_Shield_Strength(obj) )
	{
		isDestroyed = false;

		// Send message to game controller indicating we have been repaired. If this is the
		// last minor weakpoint to be repaired our parent will send us a custom indicating
		// the building is now online and instructing us to reverse our weakpoint effect
		GameObject* parent = Commands->Find_Object ( parentId );
		if ( parent != NULL )
			Commands->Send_Custom_Event ( obj, parent, CUSTOM_BUILDINGSCRIPTS_CHILDREPAIRED, 0, 0.0f );

		// Set animation
		if ( strlen(Get_Parameter("animName")) > 0 )
		{
			m_pLoopedAnimCtrl->PlayAnimation(Get_Parameter("animName"),Get_Int_Parameter("animStartFrame"),Get_Int_Parameter("animEndFrame"),!Get_Int_Parameter("animLoop"));
		}
		else
		{
			m_pLoopedAnimCtrl->StopAnimation();
		}
	}
}


void MinorWeakpointScriptClass::Killed ( GameObject* obj, GameObject* killer )
{
	// A minor weakpoint should never be killed... they should be repairable
	Console_Output ( "[%d:%s:%s] Script Error: A minor weakpoint has been killed...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
}


void MinorWeakpointScriptClass::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	GameObject* parent = Commands->Find_Object ( parentId );
	if ( parent == NULL )
		return;
	
	// Parent building now dead, set parentId to -1 to prevent Find_Object returning a non-NULL result and
	// potentially causing a pure virtual function call when we try to send the dead building a custom
	if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED && Commands->Get_ID(sender) == parentId )
		parentId = -1;
	
	// Parent building now offline, if param is 1 we need to apply our offline effect (param 1 means this
	// was the last weakpoint to die, hence this one is chosen to apply the effects)
	if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGOFFLINE && Commands->Get_ID(sender) == parentId && param == 1 )
		applyEffect ( obj, parent );

	// Parent building now online, if param is 1 we need to reverse our offline effect (param 1 means this
	// was the last weakpoint to die, hence this one is chosen to reverse the effects)
	if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGONLINE && Commands->Get_ID(sender) == parentId && param == 1 )
		reverseEffect ( obj, parent );
}

void MinorWeakpointScriptClass::Detach ( GameObject *obj )
{
	if (m_pLoopedAnimCtrl)
	{
		delete m_pLoopedAnimCtrl;
	}
}



/*------------------------
Weakpoint - Disable Soldier Purchase
--------------------------*/

void dp88_buildingScripts_weakpoint_disableSoldierPurchase::applyEffect ( GameObject* obj, GameObject* parent )
{
	Set_Can_Generate_Soldiers ( Get_Object_Type(parent), false );
}

void dp88_buildingScripts_weakpoint_disableSoldierPurchase::reverseEffect ( GameObject* obj, GameObject* parent )
{
	Set_Can_Generate_Soldiers ( Get_Object_Type(parent), true );
}




/*------------------------
Weakpoint - Disable Vehicle Purchase
--------------------------*/

void dp88_buildingScripts_weakpoint_disableVehiclePurchase::applyEffect ( GameObject* obj, GameObject* parent )
{
	if (parent->As_BuildingGameObj() && parent->As_BuildingGameObj()->As_VehicleFactoryGameObj())
	{
		parent->As_BuildingGameObj()->As_VehicleFactoryGameObj()->Set_Disabled(true);
		PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_VEHICLES,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(parent)))->Set_Page_Disabled(true);
		PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(parent)))->Set_Page_Disabled(true);
	}
}

void dp88_buildingScripts_weakpoint_disableVehiclePurchase::reverseEffect ( GameObject* obj, GameObject* parent )
{
	if (parent->As_BuildingGameObj() && parent->As_BuildingGameObj()->As_VehicleFactoryGameObj())
	{
		parent->As_BuildingGameObj()->As_VehicleFactoryGameObj()->Set_Disabled(false);
		PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_VEHICLES,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(parent)))->Set_Page_Disabled(false);
		PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_SECRET_VEHICLES,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(parent)))->Set_Page_Disabled(false);
	}
}


/*------------------------
Weakpoint - Disable Air Vehicle Purchase
--------------------------*/

void dp88_buildingScripts_weakpoint_disableAirVehiclePurchase::applyEffect ( GameObject* obj, GameObject* parent )
{
	if (parent->As_BuildingGameObj() && parent->As_BuildingGameObj()->As_AirFactoryGameObj())
	{
		parent->As_BuildingGameObj()->As_AirFactoryGameObj()->Set_Disabled(true);
		PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_AIR,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(parent)))->Set_Page_Disabled(true);
	}
}

void dp88_buildingScripts_weakpoint_disableAirVehiclePurchase::reverseEffect ( GameObject* obj, GameObject* parent )
{
	if (parent->As_BuildingGameObj() && parent->As_BuildingGameObj()->As_AirFactoryGameObj())
	{
		parent->As_BuildingGameObj()->As_AirFactoryGameObj()->Set_Disabled(false);
		PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_AIR,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(parent)))->Set_Page_Disabled(false);
	}
}

/*------------------------
Weakpoint - Disable Naval Vehicle Purchase
--------------------------*/

void dp88_buildingScripts_weakpoint_disableNavalVehiclePurchase::applyEffect ( GameObject* obj, GameObject* parent )
{
	if (parent->As_BuildingGameObj() && parent->As_BuildingGameObj()->As_NavalFactoryGameObj())
	{
		parent->As_BuildingGameObj()->As_NavalFactoryGameObj()->Set_Disabled(true);
		PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_NAVAL,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(parent)))->Set_Page_Disabled(true);
	}
}

void dp88_buildingScripts_weakpoint_disableNavalVehiclePurchase::reverseEffect ( GameObject* obj, GameObject* parent )
{
	if (parent->As_BuildingGameObj() && parent->As_BuildingGameObj()->As_NavalFactoryGameObj())
	{
		parent->As_BuildingGameObj()->As_NavalFactoryGameObj()->Set_Disabled(false);
		PurchaseSettingsDefClass::Find_Definition(PurchaseSettingsDefClass::TYPE_NAVAL,(PurchaseSettingsDefClass::TEAM)PTTEAM(Get_Object_Type(parent)))->Set_Page_Disabled(false);
	}
}

/*------------------------
Weakpoint - Disable Base Power
--------------------------*/

void dp88_buildingScripts_weakpoint_disableBasePower::applyEffect ( GameObject* obj, GameObject* parent )
{
	Power_Base ( Get_Object_Type(parent), false );
}

void dp88_buildingScripts_weakpoint_disableBasePower::reverseEffect ( GameObject* obj, GameObject* parent )
{
	Power_Base ( Get_Object_Type(parent), true );
}


/*------------------------
Weakpoint - Disable Radar
--------------------------*/

void dp88_buildingScripts_weakpoint_disableRadar::applyEffect ( GameObject* obj, GameObject* parent )
{
	Enable_Base_Radar ( Get_Object_Type(parent), false );
}

void dp88_buildingScripts_weakpoint_disableRadar::reverseEffect ( GameObject* obj, GameObject* parent )
{
	Enable_Base_Radar ( Get_Object_Type(parent), true );
}


/*------------------------
Weakpoint - Disable Refinery
--------------------------*/

void dp88_buildingScripts_weakpoint_disableRefinery::applyEffect ( GameObject* obj, GameObject* parent )
{
	if (parent->As_BuildingGameObj() && parent->As_BuildingGameObj()->As_RefineryGameObj())
	{
		parent->As_BuildingGameObj()->As_RefineryGameObj()->Block_Harvester_Spawn();
	}
}

void dp88_buildingScripts_weakpoint_disableRefinery::reverseEffect ( GameObject* obj, GameObject* parent )
{
	if (parent->As_BuildingGameObj() && parent->As_BuildingGameObj()->As_RefineryGameObj())
	{
		parent->As_BuildingGameObj()->As_RefineryGameObj()->Allow_Harvester_Spawn();
	}
}



/*------------------------
Building Effect Script
--------------------------*/

void dp88_BuildingEffectScript::Custom ( GameObject *obj, int type, int param, GameObject *sender )
{
	if ( obj == sender )
	{
		if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGOFFLINE )
			OnBuildingOffline(obj);
		else if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGONLINE )
			OnBuildingOnline(obj);
		else if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED )
			OnBuildingCaptured(obj, param);
		else
			OnCustom(obj,type,param,sender);
	}
	else
		OnCustom(obj,type,param,sender);
}




/*------------------------
Money Trickle Effect Timer
--------------------------*/

void dp88_buildingScripts_functionMoneyTrickle::Created ( GameObject *obj )
{
	isTimerRunning = false;
	if ( Get_Object_Type(obj) == 0 || Get_Object_Type(obj) == 1 )
	{
		isTimerRunning = true;
		Commands->Start_Timer(obj, this, 0.1f, TIMER_BUILDINGSCRIPTS_TRICKLEMONEY );
	}
}

void dp88_buildingScripts_functionMoneyTrickle::OnBuildingCaptured ( GameObject *obj, int team )
{
	if ( (team == 0 || team == 1) && !isTimerRunning )
	{
		isTimerRunning = true;
		Commands->Start_Timer(obj, this, 0.1f, TIMER_BUILDINGSCRIPTS_TRICKLEMONEY );
	}
}

void dp88_buildingScripts_functionMoneyTrickle::Timer_Expired ( GameObject* obj, int number )
{
	if ( number == TIMER_BUILDINGSCRIPTS_TRICKLEMONEY )
	{
		// Check team is still correct
		int team = Get_Object_Type(obj);
		if ( team == 0 || team == 1 )
		{
			// NB: Calling Give_Money with the building controller as a parameter doesn't seem
			// to give anyone any money, so get the first player on the team and use them as
			// the target for Give_Money (true as third param to give to whole team)
			GameObject* pFirstPlayer = Find_First_Player(team);
			if ( pFirstPlayer )
				Commands->Give_Money(pFirstPlayer,Get_Float_Parameter("creditsPerSec"),true);
			Commands->Start_Timer(obj, this, 1.0f, TIMER_BUILDINGSCRIPTS_TRICKLEMONEY );
		}
		else		
			isTimerRunning = false;
	}
}


void dp88_buildingScripts_functionMoneyGrant::Created ( GameObject *obj )
{
	if ( Get_Object_Type(obj) == 0 || Get_Object_Type(obj) == 1 )
	{
		int team = Get_Object_Type(obj);
		GameObject* pFirstPlayer = Find_First_Player(team);
		if ( pFirstPlayer )
		{
			Commands->Give_Money(pFirstPlayer,Get_Float_Parameter("credits"),true);
		}
		if (Get_Int_Parameter("onceOnly") == 1)
		{
			Destroy_Script();
		}
	}
}

void dp88_buildingScripts_functionMoneyGrant::OnBuildingCaptured ( GameObject *obj, int team )
{
	if (team == 0 || team == 1)
	{
		GameObject* pFirstPlayer = Find_First_Player(team);
		if ( pFirstPlayer )
		{
			Commands->Give_Money(pFirstPlayer,Get_Float_Parameter("credits"),true);
		}
		if (Get_Int_Parameter("onceOnly") == 1)
		{
			Destroy_Script();
		}
	}
}


/*------------------------
Building Function - Spawn Team Zone
--------------------------*/

void dp88_buildingScripts_functionSpawnTeamZone::Created ( GameObject *obj )
{
	// Set default values
	zoneId = NULL;

	// Create initial zone (if any)
	createZone(obj);
}


void dp88_buildingScripts_functionSpawnTeamZone::OnBuildingOffline( GameObject* obj )
{
	if ( Get_Int_Parameter("destroyZoneWhenDisabled") == 1 )
		destroyZone(obj);
}


void dp88_buildingScripts_functionSpawnTeamZone::OnBuildingOnline ( GameObject* obj )
{
	if ( Get_Int_Parameter("destroyZoneWhenDisabled") == 1 )
		createZone(obj);
}


void dp88_buildingScripts_functionSpawnTeamZone::OnBuildingCaptured ( GameObject* obj, int team )
{
	destroyZone(obj);
	createZone(obj);
}


void dp88_buildingScripts_functionSpawnTeamZone::Destroyed ( GameObject* obj )
{
	destroyZone(obj);
}


void dp88_buildingScripts_functionSpawnTeamZone::createZone(GameObject* obj)
{
	if ( zoneId == NULL )
	{
		// Decide which script zone to create based on current team type
		const char* zonePreset = "";
		if ( Get_Object_Type(obj) == 0 )
			zonePreset = Get_Parameter("scriptZonePreset_team0");
		else if ( Get_Object_Type(obj) == 1 )
			zonePreset = Get_Parameter("scriptZonePreset_team1");
		else
			zonePreset = Get_Parameter("scriptZonePreset_team2");


		if ( strlen(zonePreset) > 0 && Is_Valid_Preset(zonePreset) )
		{
			// Determine the size and rotation for the new zone
			Vector3 position = Commands->Get_Bone_Position(obj,Get_Parameter("scriptZoneBone"));
			Vector3 size = Get_Vector3_Parameter("scriptZoneSize");
			Matrix3 rotation(true);
			rotation.Rotate_Z(Commands->Get_Facing(obj));

			// Define the bounding box and create the zone
			OBBoxClass zoneBoundingBox ( position, size, rotation );
			GameObject* zone = Create_Zone(zonePreset,zoneBoundingBox);

			// Store the zone ID
			if ( zone )
				zoneId = Commands->Get_ID(zone);
		}
	}
}


void dp88_buildingScripts_functionSpawnTeamZone::destroyZone(GameObject* obj)
{
	// Check zoneId is not null before attempting to destroy the zone
	if ( zoneId != NULL )
	{
		// Destroy the zone if it exists
		GameObject* zone = Commands->Find_Object(zoneId);
		if ( zone )
			Commands->Destroy_Object(zone);
		zoneId = NULL;
	}
}




/*------------------------
Building Script Zone
--------------------------*/

void dp88_buildingScripts_scriptZone::Created ( GameObject *obj )
{
	// Set default values
	parentId = NULL;
	zoneId = NULL;

	// Find parent
	GameObject* parent = Find_Object_By_Preset ( Get_Object_Type ( obj ), Get_Parameter("buildingControllerPreset") );
	if ( parent != NULL )
	{
		parentId = Commands->Get_ID ( parent );

		// Send with 1 second delay to ensure parent has been initialized
		Commands->Send_Custom_Event ( obj, parent, CUSTOM_BUILDINGSCRIPTS_REGISTERCHILD, BUILDING_CHILD_TYPE_GENERIC, 1.0f );

		// Check the script zone preset is valid and, if it is, create the zone
		if ( Is_Valid_Preset(Get_Parameter("scriptZonePreset")) )
			createZone(obj);

		else
		{
			Console_Output ( "[%d:%s:%s] Critical Error: A script zone preset with the name '%s' was not found. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("scriptZonePreset") );
			Destroy_Script();
		}
	}
	else
	{
		Console_Output ( "[%d:%s:%s] Critical Error: Building controller '%s' not found. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("buildingControllerPreset") );
		Destroy_Script();
	}
}


void dp88_buildingScripts_scriptZone::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Ignore messages from anyone but our parent building
	if ( Commands->Get_ID(sender) == parentId )
	{
		// Parent building is now dead, destroy the script zone if applicable
		if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED && Get_Int_Parameter("destroyZoneWhenDestroyed") == 1 )
			destroyZone(obj);
	
		// Parent building now offline, destroy the script zone if applicable
		if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGOFFLINE && Get_Int_Parameter("destroyZoneWhenDisabled") == 1 )
			destroyZone(obj);

		// Parent building now online, recreate the script zone if applicable
		if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGONLINE && Get_Int_Parameter("destroyZoneWhenDisabled") == 1 )
			createZone(obj);
	}
}


void dp88_buildingScripts_scriptZone::createZone(GameObject* obj)
{
	// Check that zoneId is NULL before creating a new zone
	if ( zoneId == NULL )
	{
		// Determine the size and rotation for the new zone
		Vector3 position = Commands->Get_Position(obj);
		Vector3 size = Get_Vector3_Parameter("scriptZoneSize");
		Matrix3 rotation(true);
		rotation.Rotate_Z(Commands->Get_Facing(obj));

		// Define the bounding box and create the zone
		OBBoxClass zoneBoundingBox ( position, size, rotation );
		GameObject* zone = Create_Zone(Get_Parameter("scriptZonePreset"),zoneBoundingBox);

		// Store the zone ID
		if ( zone )
			zoneId = Commands->Get_ID(zone);
	}
}


void dp88_buildingScripts_scriptZone::destroyZone(GameObject* obj)
{
	// Check zoneId is not null before attempting to destroy the zone
	if ( zoneId != NULL )
	{
		// Destroy the zone if it exists
 		GameObject* zone = Commands->Find_Object(zoneId);
		if ( zone )
			Commands->Destroy_Object(zone);
		zoneId = NULL;
	}
}




/*------------------------
Building Capture Point
--------------------------*/

void dp88_buildingScripts_capturePoint::Created( GameObject *obj )
{
	// Set default values
	parentId = NULL;
	currentTeam = 0;

	// Find parent
	GameObject* parent = Find_Closest_Preset_By_Team ( Get_Object_Type ( obj ), Commands->Get_Position(obj), Get_Parameter("buildingControllerPreset") );
	if ( parent != NULL )
	{
		parentId = Commands->Get_ID ( parent );
		currentTeam = Get_Object_Type(parent);
		Set_Object_Type(obj,currentTeam);

		// Check parent is not a building (since changing the team of a BuildingGameObj results
		// in some very strange behaviour that we really don't want)
		if ( parent->As_BuildingGameObj() )
		{
			Console_Output ( "[%d:%s:%s] Critical Error: Building controller '%s' is of type BuildingGameObj, capturing is not possible. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name() );
			Destroy_Script();
		}

		else
		{
			// Send with 1 second delay to ensure parent has been initialized
			Commands->Send_Custom_Event ( obj, parent, CUSTOM_BUILDINGSCRIPTS_REGISTERCHILD, BUILDING_CHILD_TYPE_CAPTUREPOINT, 1.0f );

			// Set the current health state to match the current team (0 = Soviet, 1 = Allies, -2 = Neutral)
			if ( currentTeam == 0 )
				Commands->Set_Health(obj, 0.25f);
			else if ( currentTeam == 1 )
				Commands->Set_Health(obj, Commands->Get_Max_Health(obj));
			else
				Commands->Set_Health(obj, Commands->Get_Max_Health(obj)/2.0f);

			// Start timer to constantly move health towards equilibrium
			Commands->Start_Timer (obj, this, 1.0f, TIMER_BUILDINGSCRIPTS_CAPTUREPOINT );
		}
	}
	else
	{
		Console_Output ( "[%d:%s:%s] Critical Error: Building controller '%s' not found. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("buildingControllerPreset") );
		Destroy_Script();
	}
}


extern REF_DECL2(NodHouseColor, Vector3);
extern REF_DECL2(GDIHouseColor, Vector3);
extern REF_DECL2(PublicMessageColor, Vector3);
void dp88_buildingScripts_capturePoint::Damaged( GameObject *obj, GameObject *damager, float amount )
{
	float health = Commands->Get_Health(obj);
	float max = Commands->Get_Max_Health(obj);

	// If the damager was the soviet engineer then decrease the health by 1 and
	// check if they have now captured the building
	unsigned int warhead = ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("warhead"));
	if (warhead == Get_Damage_Warhead() && Commands->Get_Player_Type(damager) == 0)
	{
		// Decrease health by 1.00 (unless we are already below 1.25 in which
		// case we just set it to 0.25)
		health = ( health <= 1.25f ) ? 0.25f : health - 1.0f;
		Commands->Set_Health (obj, health );

		// Did we capture the structure?
		if ( health == 0.25f && currentTeam != 0 )
		{
			currentTeam = 0;
			int red = (int)(NodHouseColor.X * 255);
			int green = (int)(NodHouseColor.Y * 255);
			int blue = (int)(NodHouseColor.Z * 255);
			int stringall = Get_Int_Parameter("Sov_String_All");
			if (stringall && Is_Valid_String_ID(stringall))
			{
				const char *str = Get_Translated_String(stringall);
				Send_Message_Team(1,red,green,blue,str);
				delete[] str;
				int soundId = Get_String_Sound_ID(stringall);
				if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
				{
					Create_2D_Sound_Team(Get_Definition_Name(soundId),1);
				}
			}
			int stringsov = Get_Int_Parameter("Sov_String_Sov");
			if (stringsov && Is_Valid_String_ID(stringsov))
			{
				const char *str = Get_Translated_String(stringsov);
				Send_Message_Team(0,red,green,blue,str);
				delete[] str;
				int soundId = Get_String_Sound_ID(stringsov);
				if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
				{
					Create_2D_Sound_Team(Get_Definition_Name(soundId),0);
				}
			}
			Commands->Send_Custom_Event (obj, Commands->Find_Object(parentId), CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED, currentTeam, 0.0f );
			Set_Object_Type(obj,currentTeam);
		}

		// OK, we didn't capture it, but did we manage to make it neutral?
		else if ( currentTeam == 1 && health <= max/2.0f )
		{
			int red = (int)(PublicMessageColor.X * 255);
			int green = (int)(PublicMessageColor.Y * 255);
			int blue = (int)(PublicMessageColor.Z * 255);
			int stringall = Get_Int_Parameter("Neutral_String_All");
			if (stringall && Is_Valid_String_ID(stringall))
			{
				const char *str = Get_Translated_String(stringall);
				Send_Message_Team(1,red,green,blue,str);
				delete[] str;
				int soundId = Get_String_Sound_ID(stringall);
				if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
				{
					Create_2D_Sound_Team(Get_Definition_Name(soundId),1);
				}
			}
			int stringsov = Get_Int_Parameter("Neutral_String_Sov");
			if (stringsov && Is_Valid_String_ID(stringsov))
			{
				const char *str = Get_Translated_String(stringsov);
				Send_Message_Team(0,red,green,blue,str);
				delete[] str;
				int soundId = Get_String_Sound_ID(stringsov);
				if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
				{
					Create_2D_Sound_Team(Get_Definition_Name(soundId),0);
				}
			}
			currentTeam = -2;
			Commands->Send_Custom_Event (obj, Commands->Find_Object(parentId), CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED, currentTeam, 0.0f );
			Set_Object_Type(obj,currentTeam);
		}
	}


	// If the damager was the allied engineer then increase the health by 1 and
	// check if they have now captured the building
	else if (warhead == Get_Damage_Warhead() && Commands->Get_Player_Type(damager) == 1)
	{
		// Increase health by 1.00 (unless we are already less than 1.00 below the
		// maximum in which case we just set it to the maximum)
		health = ( health >= max-1.00f ) ? max : health + 1.0f;
		Commands->Set_Health (obj, health );

		// Did we capture the structure?
		if ( health == max && currentTeam != 1 )
		{
			currentTeam = 1;
			int red = (int)(GDIHouseColor.X * 255);
			int green = (int)(GDIHouseColor.Y * 255);
			int blue = (int)(GDIHouseColor.Z * 255);
			int stringall = Get_Int_Parameter("All_String_All");
			if (stringall && Is_Valid_String_ID(stringall))
			{
				const char *str = Get_Translated_String(stringall);
				Send_Message_Team(1,red,green,blue,str);
				delete[] str;
				int soundId = Get_String_Sound_ID(stringall);
				if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
				{
					Create_2D_Sound_Team(Get_Definition_Name(soundId),1);
				}
			}
			int stringsov = Get_Int_Parameter("All_String_Sov");
			if (stringsov && Is_Valid_String_ID(stringsov))
			{
				const char *str = Get_Translated_String(stringsov);
				Send_Message_Team(0,red,green,blue,str);
				delete[] str;
				int soundId = Get_String_Sound_ID(stringsov);
				if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
				{
					Create_2D_Sound_Team(Get_Definition_Name(soundId),0);
				}
			}
			Commands->Send_Custom_Event (obj, Commands->Find_Object(parentId), CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED, currentTeam, 0.0f );
			Set_Object_Type(obj,currentTeam);
		}

		// OK, we didn't capture it, but did we manage to make it neutral?
		else if ( currentTeam == 0 && health >= max/2.0f )
		{
			int red = (int)(PublicMessageColor.X * 255);
			int green = (int)(PublicMessageColor.Y * 255);
			int blue = (int)(PublicMessageColor.Z * 255);
			int stringall = Get_Int_Parameter("Neutral_String_All");
			if (stringall && Is_Valid_String_ID(stringall))
			{
				const char *str = Get_Translated_String(stringall);
				Send_Message_Team(1,red,green,blue,str);
				delete[] str;
				int soundId = Get_String_Sound_ID(stringall);
				if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
				{
					Create_2D_Sound_Team(Get_Definition_Name(soundId),1);
				}
			}
			int stringsov = Get_Int_Parameter("Neutral_String_Sov");
			if (stringsov && Is_Valid_String_ID(stringsov))
			{
				const char *str = Get_Translated_String(stringsov);
				Send_Message_Team(0,red,green,blue,str);
				delete[] str;
				int soundId = Get_String_Sound_ID(stringsov);
				if (soundId && Is_Valid_Preset_ID(soundId) && Find_Definition(soundId)->Get_Class_ID() == 0x5000)
				{
					Create_2D_Sound_Team(Get_Definition_Name(soundId),0);
				}
			}
			currentTeam = -2;
			Commands->Send_Custom_Event (obj, Commands->Find_Object(parentId), CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED, currentTeam, 0.0f );
			Set_Object_Type(obj,currentTeam);
		}
	}
} 


void dp88_buildingScripts_capturePoint::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	// Ignore messages from anyone but our parent building
	if ( Commands->Get_ID(sender) == parentId )
	{
		// If the parent building is dead we have no further purpose, spawn any appropriate 'dead' preset
		// and destroy ourselves
		if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED )
		{
			// Spawn dead preset
			if ( Is_Valid_Preset ( Get_Parameter("dead_preset") ) )
			{
				GameObject* dead = Commands->Create_Object ( Get_Parameter("dead_preset"), Commands->Get_Position(obj) );
				Commands->Set_Facing ( dead, Commands->Get_Facing(obj) );
			}

			// And kill ourself
			Commands->Apply_Damage ( obj, 1000.0f, "Death", obj );
		}
	}
}


void dp88_buildingScripts_capturePoint::Timer_Expired( GameObject *obj, int number )
{
	// Move health back towards equilibrium point for current owner
	if ( number == TIMER_BUILDINGSCRIPTS_CAPTUREPOINT )
	{
		float health = Commands->Get_Health(obj);

		if ( currentTeam == 0 && health != 0.25f )
		{
			// Decrease health by 0.25 (unless we are already below 0.50 in which
			// case we just set it to 0.25)
			health = ( health <= 0.50f ) ? 0.25f : health - 0.25f;

			// Apply new health
			Commands->Set_Health(obj, health);
		}

		else
		{
			float max = Commands->Get_Max_Health(obj);

			if ( currentTeam == 1 && health != max )
			{
				// Increase health by 0.25, unless we are within 0.25 of the maximum
				// health value, in which case we use that
				health = ( health+0.25f >= max ) ? max : health + 0.25f;

				// Apply new health
				Commands->Set_Health(obj, health);
			}

			else if ( health != max/2.0f )
			{
				// Are we above or below the halfway point?
				bool above = ( health > max/2.0f );

				// Are we within 0.25 points of the halfway point? If so set that as
				// our new health value (we have reached equilibrium)
				if ( (above && (health-0.25f <= max/2.0f)) || (!above &&  (health+0.25f >= max/2.0f)) )
					Commands->Set_Health(obj, max/2.0f);

				// Otherwise move 0.25 points closer to the halfway point
				else
					Commands->Set_Health(obj, (above) ? health-0.25f : health+0.25f );
			}
		}


		// Start timer for next iteration
		Commands->Start_Timer (obj, this, 1.0f, TIMER_BUILDINGSCRIPTS_CAPTUREPOINT );
	}
}










/*------------------------
Spawn Vehicle Part script
--------------------------*/

void dp88_buildingScripts_spawnPart::Created ( GameObject* obj )
{
	partObjectId = NULL;

	/* Start a timer to spawn the building part. We can't do this immediately because
	it seems to cause issues with double initializations of scripts... */
	Commands->Start_Timer ( obj, this, 0.01f, TIMER_SPAWNVEHICLEPART );
}


void dp88_buildingScripts_spawnPart::Timer_Expired( GameObject *obj, int number )
{
	/* Part spawn timer */
	if ( number == TIMER_SPAWNVEHICLEPART && partObjectId == NULL )
	{
		/* Spawn the vehicle part and get it's ID */
		GameObject* part = Commands->Create_Object_At_Bone ( obj, Get_Parameter("Building_Part_Preset"), Get_Parameter("Spawn_At_Bone") );
		Commands->Attach_To_Object_Bone ( part, obj, Get_Parameter ( "Spawn_At_Bone" ) );
		partObjectId = Commands->Get_ID(part);

		/* Unless this is a decorative part start the timer again to attach the health and
		veterancy link scripts. We don't do this immediately as it causes a lovely crash... */
		if ( Get_Int_Parameter("IsDecorative") == 0 )
		  Commands->Start_Timer ( obj, this, 0.01f, TIMER_SPAWNVEHICLEPART );
	}


	/* Link health & veterancy script attach timer */
	else if ( number == TIMER_SPAWNVEHICLEPART )
	{
		GameObject* part = Commands->Find_Object(partObjectId);
		if ( part )
		{
			char hostIdString[12];
			sprintf ( hostIdString, "%d", Commands->Get_ID(obj) );
			Attach_Script_Once ( part, "dp88_linkHealth", hostIdString );
			Attach_Script_Once ( part, "dp88_linkVetPoints", hostIdString );
		}
	}
}


void dp88_buildingScripts_spawnPart::OnBuildingCaptured ( GameObject* obj, int team )
{
	if ( partObjectId != NULL && Commands->Find_Object(partObjectId) )
		Set_Object_Type( Commands->Find_Object (partObjectId), team );
}


void dp88_buildingScripts_spawnPart::Destroyed ( GameObject* obj )
{
	if ( partObjectId != NULL && Commands->Find_Object(partObjectId) )
		Commands->Destroy_Object ( Commands->Find_Object (partObjectId) );
	partObjectId = NULL;
}

// -------------------------------------------------------------------------------------------------

void dp88_buildingScripts_PT_Script::Created( GameObject *obj )
{
  m_pLoopedAnimCtrl = NULL;

	// Find parent
	GameObject* parent = Find_Object_By_Preset ( Get_Object_Type ( obj ), Get_Parameter("buildingControllerPreset") );
	if ( parent != NULL )
	{
		parentId = Commands->Get_ID ( parent );

		// Send with 1 second delay to ensure parent has been initialized
		Commands->Send_Custom_Event ( obj, parent, CUSTOM_BUILDINGSCRIPTS_REGISTERCHILD, BUILDING_CHILD_TYPE_GENERIC, 1.0f );
		Commands->Start_Timer(obj, this, 1.0f, TIMER_CHECK_BASE_POWER_STATE );
		dead = false;
		basePowerState = Is_Base_Powered(Get_Object_Type(obj));
	    m_pLoopedAnimCtrl = new LoopedAnimationController(obj);
		SetDamageAnimation(obj);
	}
	else
	{
		Console_Output ( "[%d:%s:%s] Critical Error: Building controller '%s' not found. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("buildingControllerPreset") );
		Destroy_Script();
	}
}

void dp88_buildingScripts_PT_Script::Detach ( GameObject *obj )
{
	if (m_pLoopedAnimCtrl)
	{
		delete m_pLoopedAnimCtrl;
	}
}

void dp88_buildingScripts_PT_Script::Custom( GameObject *obj, int type, int param, GameObject *sender )
{
	if ( type == CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED && Commands->Get_ID(sender) == parentId )
	{
		parentId = -1;
		dead = true;
		SetDamageAnimation(obj);
	}
}

void dp88_buildingScripts_PT_Script::SetDamageAnimation ( GameObject* obj )
{
  int sFrame = 0;
  int eFrame = 0;

  if (basePowerState)
  {
    sFrame = (int)((dead) ? Get_Float_Parameter("DeadAnimationStartFrame") : Get_Float_Parameter("AliveAnimationStartFrame"));
    eFrame = (int)((dead) ? Get_Float_Parameter("DeadAnimationEndFrame") : Get_Float_Parameter("AliveAnimationEndFrame"));
  }
  else
  {
    sFrame = (int)((dead) ? Get_Float_Parameter("LPDeadAnimationStartFrame") : Get_Float_Parameter("LPAliveAnimationStartFrame"));
    eFrame = (int)((dead) ? Get_Float_Parameter("LPDeadAnimationEndFrame") : Get_Float_Parameter("LPAliveAnimationEndFrame"));
  }

  m_pLoopedAnimCtrl->PlayAnimation ( Get_Parameter("Animation"), sFrame, eFrame );
}

void dp88_buildingScripts_PT_Script::Timer_Expired ( GameObject* obj, int number )
{
	if ( number == TIMER_CHECK_BASE_POWER_STATE )
	{
		if ( Is_Base_Powered(Get_Object_Type(obj)) != basePowerState )
		{
			basePowerState = !basePowerState;
			SetDamageAnimation(obj);
		}

		// Restart timer
		Commands->Start_Timer(obj, this, 1.0f, TIMER_CHECK_BASE_POWER_STATE );
	}
}

ScriptRegistrant<dp88_buildingScripts_buildingController> dp88_buildingScripts_buildingController_Registrant( "dp88_buildingScripts_buildingController", "team0_buildingOfflineStringId=0:int,team1_buildingOfflineStringId=0:int,team0_buildingOnlineStringId=0:int,team1_buildingOnlineStringId=0:int" );

ScriptRegistrant<dp88_buildingScripts_repairNode> dp88_buildingScripts_repairNode_Registrant( "dp88_buildingScripts_repairNode", "buildingControllerPreset:string,dead_preset=null:string" );

ScriptRegistrant<dp88_buildingScripts_weakpoint_destroyBuilding> dp88_buildingScripts_weakpoint_destroyBuilding_Registrant( "dp88_buildingScripts_weakpoint_destroyBuilding", "buildingControllerPreset:string,dead_preset=null:string" );

ScriptRegistrant<dp88_buildingScripts_weakpoint_disableSoldierPurchase> dp88_buildingScripts_weakpoint_disableSoldierPurchase_Registrant( "dp88_buildingScripts_weakpoint_disableSoldierPurchase", "buildingControllerPreset:string,animName:string,animStartFrame=0:int,animEndFrame=0:int,animLoop=0:int,destroyedAnimName:string,destroyedAnimStartFrame=0:int,destroyedAnimEndFrame=0:int,destroyedAnimLoop=0:int" );
ScriptRegistrant<dp88_buildingScripts_weakpoint_disableVehiclePurchase> dp88_buildingScripts_weakpoint_disableVehiclePurchase_Registrant( "dp88_buildingScripts_weakpoint_disableVehiclePurchase", "buildingControllerPreset:string,animName:string,animStartFrame=0:int,animEndFrame=0:int,animLoop=0:int,destroyedAnimName:string,destroyedAnimStartFrame=0:int,destroyedAnimEndFrame=0:int,destroyedAnimLoop=0:int" );
ScriptRegistrant<dp88_buildingScripts_weakpoint_disableAirVehiclePurchase> dp88_buildingScripts_weakpoint_disableAirVehiclePurchase_Registrant( "dp88_buildingScripts_weakpoint_disableAirVehiclePurchase", "buildingControllerPreset:string,animName:string,animStartFrame=0:int,animEndFrame=0:int,animLoop=0:int,destroyedAnimName:string,destroyedAnimStartFrame=0:int,destroyedAnimEndFrame=0:int,destroyedAnimLoop=0:int" );
ScriptRegistrant<dp88_buildingScripts_weakpoint_disableNavalVehiclePurchase> dp88_buildingScripts_weakpoint_disableNavalVehiclePurchase_Registrant( "dp88_buildingScripts_weakpoint_disableNavalVehiclePurchase", "buildingControllerPreset:string,animName:string,animStartFrame=0:int,animEndFrame=0:int,animLoop=0:int,destroyedAnimName:string,destroyedAnimStartFrame=0:int,destroyedAnimEndFrame=0:int,destroyedAnimLoop=0:int" );
ScriptRegistrant<dp88_buildingScripts_weakpoint_disableBasePower> dp88_buildingScripts_weakpoint_disableBasePower_Registrant( "dp88_buildingScripts_weakpoint_disableBasePower", "buildingControllerPreset:string,animName:string,animStartFrame=0:int,animEndFrame=0:int,animLoop=0:int,destroyedAnimName:string,destroyedAnimStartFrame=0:int,destroyedAnimEndFrame=0:int,destroyedAnimLoop=0:int" );
ScriptRegistrant<dp88_buildingScripts_weakpoint_disableRadar> dp88_buildingScripts_weakpoint_disableRadar_Registrant( "dp88_buildingScripts_weakpoint_disableRadar", "buildingControllerPreset:string,animName:string,animStartFrame=0:int,animEndFrame=0:int,animLoop=0:int,destroyedAnimName:string,destroyedAnimStartFrame=0:int,destroyedAnimEndFrame=0:int,destroyedAnimLoop=0:int" );
ScriptRegistrant<dp88_buildingScripts_weakpoint_disableRefinery> dp88_buildingScripts_weakpoint_disableRefinery_Registrant( "dp88_buildingScripts_weakpoint_disableRefinery", "buildingControllerPreset:string,animName:string,animStartFrame=0:int,animEndFrame=0:int,animLoop=0:int,destroyedAnimName:string,destroyedAnimStartFrame=0:int,destroyedAnimEndFrame=0:int,destroyedAnimLoop=0:int" );

ScriptRegistrant<dp88_buildingScripts_functionMoneyTrickle> dp88_buildingScripts_functionMoneyTrickle_Registrant( "dp88_buildingScripts_functionMoneyTrickle", "creditsPerSec=3.0:float" );
ScriptRegistrant<dp88_buildingScripts_functionSpawnTeamZone> dp88_buildingScripts_functionSpawnTeamZone_Registrant( "dp88_buildingScripts_functionSpawnTeamZone", "scriptZonePreset_team0:string,scriptZonePreset_team1:string,scriptZonePreset_team2:string,scriptZoneBone:string,scriptZoneSize:vector3,destroyZoneWhenDisabled=1:int" );
ScriptRegistrant<dp88_buildingScripts_functionMoneyGrant> dp88_buildingScripts_functionMoneyGrant_Registrant( "dp88_buildingScripts_functionMoneyGrant", "credits:float,onceOnly=1:int");

ScriptRegistrant<dp88_buildingScripts_scriptZone> dp88_buildingScripts_scriptZone_Registrant( "dp88_buildingScripts_scriptZone", "buildingControllerPreset:string,scriptZonePreset:string,scriptZoneSize:vector3,destroyZoneWhenDisabled=1:int,destroyZoneWhenDestroyed=1:int" );

ScriptRegistrant<dp88_buildingScripts_capturePoint> dp88_buildingScripts_capturePoint_Registrant( "dp88_buildingScripts_capturePoint", "buildingControllerPreset:string,dead_preset=null:string,warhead:string,All_String_All:int,Sov_String_All:int,Neutral_String_All:int,All_String_Sov:int,Sov_String_Sov:int,Neutral_String_Sov:int");

ScriptRegistrant<dp88_buildingScripts_spawnPart> dp88_buildingScripts_spawnPart_Registrant("dp88_buildingScripts_spawnPart","Building_Part_Preset:string,Spawn_At_Bone:string,IsDecorative=0:int");
ScriptRegistrant<dp88_buildingScripts_PT_Script> dp88_buildingScripts_PT_Script_Registrant( "dp88_buildingScripts_PT_Script", "buildingControllerPreset:string,Animation:string,AliveAnimationStartFrame:float,AliveAnimationEndFrame:float,DeadAnimationStartFrame:float,DeadAnimationEndFrame:float,LPAliveAnimationStartFrame:float,LPAliveAnimationEndFrame:float,LPDeadAnimationStartFrame:float,LPDeadAnimationEndFrame:float");
