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


/****************************
Define custom codes
****************************/
//#define CUSTOM_PROMOTED							10022551
//#define CUSTOM_ENTERED_ORE_FIELD					10022552
//#define CUSTOM_EXITED_ORE_FIELD					10022553
#define CUSTOM_PILOTED_VEHICLE_ID					10022554
//#define CUSTOM_DRIVER_PROMOTED					10022555
#define CUSTOM_CHEVRON_ID_CHECK						10022556
//#define CUSTOM_ENTERED_ORE_DROP_ZONE				10022557
#define CUSTOM_ENTERED_VTOL_LAND_ZONE				10022558
#define CUSTOM_EXITED_VTOL_LAND_ZONE				10022559
/*#define CUSTOM_TD_TARGET_ID							10022560
#define CUSTOM_TD_TARGET_DIED						10022561
#define CUSTOM_TD_TARGET_REPAIRED					10022562
#define CUSTOM_TD_TD_DIED							10022563
#define CUSTOM_TD_WAITINGFORPURCHASE				10022564
#define CUSTOM_TD_PURCHASEWAIT_OK					10022565
#define CUSTOM_TD_REPORTCREATION					10022566
#define CUSTOM_TD_CONSOLEID							10022567
#define CUSTOM_TD_DRIVEREXIT						10022568*/
#define CUSTOM_KILLSELF								10022569
#define CUSTOM_RESET_SHIELD							10022570

// Customs for veterancy system
#define CUSTOM_VETERANCY_PROMOTED					1144000001		// Sent to self to notify other scripts about a promotion
#define CUSTOM_VETERANCY_REMOVE_OLD_WEAPON			1144000002		// Remove infantry units previous weapon after a promotion upgraded it
#define CUSTOM_VETERANCY_GRANT_POINTS_DELAY			1144000003		// Sent by dp88_veterancyGrantPoints to itself to delay the points grant until after game init

// Customs for IFV scripts
#define CUSTOM_IFV_REQUESTDRIVERDATA				1144010001		// Request IFV anim frame and powerup data from driver
#define CUSTOM_IFV_DRIVER_ANIMFRAME					1144010002		// IFV deployed anim frame for driver
#define CUSTOM_IFV_DRIVER_ROOKIEPOWID				1144010003		// IFV deployed rookie weapon POW id for driver
#define CUSTOM_IFV_DRIVER_VETERANPOWID				1144010004		// IFV deployed veteran weapon POW id for driver
#define CUSTOM_IFV_DRIVER_ELITEPOWID				1144010005		// IFV deployed elite weapon POW id for driver

// Custom for indirect targetting
#define CUSTOM_INSTALL_IT_DECREMENT					1144020001		// Attach decrease hook script

// Customs for dp88_unitSounds
#define CUSTOM_UNITSOUNDS_HEALDECREMENT				1144030001

// Customs for building scripts
#define CUSTOM_BUILDINGSCRIPTS_REGISTERCHILD		1144040001
#define CUSTOM_BUILDINGSCRIPTS_UNREGISTERCHILD		1144040002
#define CUSTOM_BUILDINGSCRIPTS_CHILDDESTROYED		1144040003		// Sent to the controller when a child is destroyed (minor / major WP)
#define CUSTOM_BUILDINGSCRIPTS_CHILDREPAIRED		1144040004		// Sent to the controller when a child is repaired (minor WP)
#define CUSTOM_BUILDINGSCRIPTS_BUILDINGDESTROYED	1144040005		// Sent by the controller to children when the building is destroyed
#define CUSTOM_BUILDINGSCRIPTS_BUILDINGOFFLINE		1144040006		// Sent by the controller to children when the building is offline
#define CUSTOM_BUILDINGSCRIPTS_BUILDINGONLINE		1144040007		// Sent by the controller to children when the building is online
#define CUSTOM_BUILDINGSCRIPTS_BUILDINGCAPTURED		1144040008		// Sent by the controller to children when the building has been captured or made neutral, or from a capture point to a building when it has been captured or made neutral

// Customs for AR Miners
#define CUSTOM_MINER_ENTERED_ORE_FIELD				1144050001		// Sent by ore field zone to miners on entry
#define CUSTOM_MINER_EXITED_ORE_FIELD				1144050002		// Sent by ore field zone to miners on exit
#define CUSTOM_MINER_ENTERED_DUMP_ZONE				1144050003		// Sent by ore dump zone to miners on entry
#define CUSTOM_MINER_INCREASE_ORE_LOAD				1144050004		// Sent by miners to themselves on a delay to increase their ore load
#define CUSTOM_MINER_UNLOAD_ORE_COMPLETE			1144050005		// Sent by miners to themselves on a delay to finish unloading ore
#define CUSTOM_CHRONO_MINER_DO_CHRONOSHIFT			1144050006		// Sent by Chrono Miner to itself to trigger chronoshift
#define CUSTOM_CHRONO_MINER_RETRY_CHRONOSHIFT		1144050007		// Sent by Chrono Miner to itself to retry a failed chronoshift

// Customs for Prism Tower script
#define CUSTOM_PRISMTOWER_REQUEST_CHARGING			1144060001
#define CUSTOM_PRISMTOWER_STOP_CHARGING				1144060002

// Customs for Remote Control script
#define CUSTOM_REMOTECONTROL_CREATED				1144070001		// Sent by a remote controlled vehicle when it is created
#define CUSTOM_REMOTECONTROL_DRIVEREXIT				1144070002		// Sent by a remote controlled vehicle when it's driver exits
#define CUSTOM_REMOTECONTROL_DESTROYED				1144070003		// Sent by a remote controlled vehicle when it is destroyed
#define CUSTOM_REMOTECONTROL_CONSOLEID				1144070004
#define CUSTOM_REMOTECONTROL_CONSOLEACK				1144070005
#define CUSTOM_REMOTECONTROL_DISABLED				1144070006		// Send to a remote control console to disable it
#define CUSTOM_REMOTECONTROL_ENABLED				1144070007		// Send to a remote control console to enable it

// Customs for custom turret AI scripts
#define CUSTOM_TURRETAI_ENEMYSEEN                     1144080001  // Sent from dp88_AI_PopupTurret_Spotter to dp88_AI_PopupTurret
#define CUSTOM_TURRETAI_ANIMATIONCOMPLETE             1144080002  // Send from dp88_AI_ChargeTurret_Animation to dp88_AI_ChargeTurret





/****************************
Define timer numbers
****************************/
#define TIMER_CHECK_BASE_POWER_STATE			10055220		// Used by scripts which need to check if a base is still powered
#define TIMER_REMOVE_OLDWEAPON					10055221
#define TIMER_CLEG_PHASEDEATH					10055222
#define TIMER_CLEG_CHECKDROPOBJ					10055223
#define TIMER_CLEG_CHECKRELEASETARGET			10055224
//#define TIMER_EXPIRE_PROMOTION_CHEVRON		10055225
//#define TIMER_INCREASE_ORE_LOAD					10055226
//#define TIMER_CHRONOMINER_CHRONOSHIFT_COMPLETE	10055227
//#define TIMER_UNLOAD_ORE_COMPLETE				10055228
/*#define TIMER_TD_DO_DAMAGE						10055229
#define TIMER_TD_PURCHASEWAIT					10055230
#define	TIMER_TD_PURCHASETIMEEXPIRED			10055231
#define TIMER_TD_TRANSITIONS_ENABLE				10055232
#define TIMER_TD_DIED_DRIVERFREE				10055233*/
#define TIMER_MIRAGE_CHECKMOVEMENT				10055234
#define TIMER_LINKHEALTH						10055235
#define TIMER_LINKVETPOINTS						10055236
#define TIMER_SPAWNVEHICLEPART					10055237		// Used by dp88_spawnVehiclePart
#define TIMER_HEALTHARMOURREGENTICK				10055239

// Timers for veterancy system
#define TIMER_VETERANCY_EXPIRE_PROMOTION_CHEVRON	2244000001		// Timer to clear promotion chevron

// Timers for IFV scripts
#define TIMER_IFV_ALLOWDEPLOY						2244010001		// Timer to allow deployment again
#define TIMER_IFV_DEPLOYCOMPLETE					2244010002		// Timer to set new animation frame after deployment

// Timer for random weather script
#define TIMER_RANDOMWEATHER							2244020001		// Timer to do a weather update

// Timers for building scripts
#define TIMER_BUILDINGSCRIPTS_CAPTUREPOINT			2244030001		// Runs on a building capture point to constantly move the health towards equilibrium

// Timers for building effect scripts
#define TIMER_BUILDINGSCRIPTS_TRICKLEMONEY			2244030501		// Runs on a trickle money effect to grant money once per second

// Timers for prism towers and tesla coils
#define TIMER_CUSTOMAI_THINK                          2244040001  // Timer that fires every 1 second in my custom AI scripts to update target state etc
#define TIMER_TESLACOIL_DECREMENT_CHARGE			2244040002

// Timers for dp88_chronoTank
#define TIMER_CHRONOTANK_ADVANCE_STATE				2244060001		// Timer to advance to the next state
#define TIMER_CHRONOTANK_RANDOM_INTERFERENCE		2244060002		// Timer to randomly alter velocity and heading during shifted out phase (unstable vortex)

// Timers for Remote Control script
#define TIMER_REMOTECONTROL_TIMEOUT					2244070001		// Timer to time out a remote vehicle purchase if we don't get confirmation within the deadline
#define TIMER_REMOTECONTROL_DRIVERENTER				2244070002		// Timer to place driver into remove vehicle once transitions are enabled
#define TIMER_REMOTECONTROL_DRIVEREXIT				2244070003		// Timer to return driver from a dead remote vehicle
#define TIMER_REMOTECONTROL_CHARGETICK				2244070004		// Timer to count down charge time