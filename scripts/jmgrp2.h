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

// Back to Jeradcode

struct AntiMissileSystemControl
{
	int DishID;
	struct AntiMissileSystemControl *next;
};

AntiMissileSystemControl *AntiMissileSystemNode = NULL;

struct PlayerCharacterControl
{
	char PresetName[128];
	char PlayerName[128];
	struct PlayerCharacterControl *next;
};

PlayerCharacterControl *PlayerCharacterNode = NULL;

struct ObjectLimitControl
{
	int TurretID;
	ObjectLimitControl *next;
};

ObjectLimitControl *ObjectLimitNode[128] = {NULL};

struct SCUD_Missile_Launcher_Control
{
	int LauncherID;
	int ScudID;
	int TargetingSystemID;
	struct SCUD_Missile_Launcher_Control *next;
};

SCUD_Missile_Launcher_Control *SCUD_Missile_Launcher_Node = NULL;

//Banking
struct RP2PlayerBankAccountControl
{
	char PlayerName[256];
	float AccountBalance;
	float InterestRate;
	struct RP2PlayerBankAccountControl *next;
};

RP2PlayerBankAccountControl *RP2PlayerBankAccountNode = NULL;

struct RP2RandomTeleportPositionControl
{
	int SpotNumber;
	Vector3 Telepos;
	struct RP2RandomTeleportPositionControl *next;
};

RP2RandomTeleportPositionControl *RP2RandomTeleportPositionNode = NULL;

// New Missions System
struct RP2GiveObjectiveByCreationLocationControl
{
	Vector3 CreatePos;
	int ObjectiveID;
	struct RP2GiveObjectiveByCreationLocationControl *next;
};

RP2GiveObjectiveByCreationLocationControl *RP2GiveObjectiveByCreationLocationNode = NULL;

struct RP2PlayerVehicleFailMissionControl
{
	int MissionType;
	struct RP2PlayerVehicleFailMissionControl *next;
};

struct RP2PlayerVehicleFailMissionExceptionControl
{
	int MissionType;
	char VehiclePreset[256];
	struct RP2PlayerVehicleFailMissionExceptionControl *next;
};

struct RP2PlayerMissionConflicts
{
	int MissionType;
	int ConflictingMission;
	struct RP2PlayerMissionConflicts *next;
};

struct RP2MissionObjectiveTypeControl
{
	int MissionType;
	int ObjectiveCount;
	char MissionStartText[2000];
	char MissionFailedText[2000];
	char MissionSuccessfulText[2000];
	int CompleteMissionRewardMoney;
	char CompleteMissionRewardWeapon[256];
	int CompleteMissionRewardInventory;
	int CompleteMissionRewardMaxHealth;
	int CompleteMissionRewardMaxArmor;
	int MissionRewardCustomMessage;
	int ResetOnDeath;
	int ResetOnSuccess;
	int RegrantOnNewgame;
	int RegrantOnSpawn;
	int RegrantOnFirstSpawn;
	int ResetOnTime;
	int ObjectivesInOrder;
	int ResetOnSpawn;
	int OneAtATime;
	int StartObjective;
	int RandomChoice;
	int MissionMessageSuccess;
	int MissionMessageFailed;
	int MissionMessageStart;
	int InUse;
	int DisplayMissionInfo;
	int IsAchievement;
	char MissionRewardW3D[256];
	int FailOnTrackedVehicle;
	int FailOnWheeledVehicle;
	int FailOnMotorcycle;
	int FailOnAircraft;
	int FailOnTurret;
	int FailOnBoat;
	int AllVehicles;
	int VehicleFails;
	RP2PlayerVehicleFailMissionExceptionControl *VehicleExceptions;
	RP2PlayerMissionConflicts *MissionConflicts;
	struct RP2MissionObjectiveTypeControl *next;
};

RP2MissionObjectiveTypeControl *RP2MissionObjectiveTypeNode = NULL;

struct MissionObjectives
{
	int ObjectiveType;
	struct MissionObjectives *next;
};

struct MissionTypes
{
	int MissionType;
	int MissionCompleted; // Objectives in each mission that have been thus completed
	int CompletedMissionObjectives; // Objectives in each mission that have been thus completed
	int StartedMission;// Which missions have been completed
	struct MissionObjectives *MissionDataPoints;// Objectives that have been completed for this mission
	int ResetOnDeath;
	int ResetOnSuccess;
	int ResetOnSpawn;
	int RegrantOnNewgame;
	int RegrantOnSpawn;
	int RegrantOnFirstSpawn;
	int HasGranted;
	int ResetOnTime;
	int ObjectivesInOrder;
	struct MissionTypes *next;
};

struct RP2PlayerMissionsControl
{
	char PlayerName[256];
	struct MissionTypes *Mission;// Different types of missions this player is doing
	RP2PlayerVehicleFailMissionControl *AllVehicles;
	RP2PlayerVehicleFailMissionControl *FailOnTrackedVehicle;
	RP2PlayerVehicleFailMissionControl *FailOnWheeledVehicle;
	RP2PlayerVehicleFailMissionControl *FailOnMotorcycle;
	RP2PlayerVehicleFailMissionControl *FailOnAircraft;
	RP2PlayerVehicleFailMissionControl *FailOnTurret;
	RP2PlayerVehicleFailMissionControl *FailOnBoat;
	//RP2PlayerVehicleFailMissionExceptionControl *VehicleExceptions;
	RP2PlayerMissionConflicts *MissionConflicts;
	struct RP2PlayerMissionsControl *next;
};

RP2PlayerMissionsControl *RP2PlayerMissionsNode = NULL;

struct RP2PhoneMissionControl
{
	int PhoneMissionNumber;
	int MissionType;
	struct RP2PhoneMissionControl *next;
};

RP2PhoneMissionControl *RP2PhoneMissionNode = NULL;

struct RP2PhoneTextControl
{
	int ScriptID;
	char Greeting[2000];
	char Confirmation[2000];
	char Goodbye[2000];
	char Buy[2000];
	char Sorry[2000];
	struct RP2PhoneTextControl *next;
};

RP2PhoneTextControl *RP2PhoneTextNode = NULL;

struct PlayerSpecificPNControl
{
	int AttachedID;
	char PlayerName[236];
	char Message[236];
	struct PlayerSpecificPNControl *next;
};

PlayerSpecificPNControl *PlayerSpecificPNNode = NULL;

// Weapons System
struct RP2WeaponListControl
{
	char WeaponName[256];
	char PowerUpName[256];
	char FakePowerUpName[256];
	bool CanBeDropped;
	float WeaponSize;
	int WeaponCost;
	int BulletCost;
	char AltWeaponName[256];
	float DropHeight;
	float RespawnDistance;
	int DropWhenEmpty;
	int WeaponClass;
	int DropMessage;
	int WeaponTier;
	int TierNumber;
	float AmmoSize;
	int CanFavorite;
	int CookTime;
	struct RP2WeaponListControl *next;
};

RP2WeaponListControl *RP2WeaponListNode = NULL;

struct RP2PlayerSAWTControl
{
	char WeaponName[256];
	int LastAmmoCount;
	struct RP2PlayerSAWTControl *next;
};

RP2PlayerSAWTControl *RP2PlayerSAWTArray[128] = {NULL};

struct DroppedWeaponControl
{
	int PowerupID;
	char WeaponName[256];
	int Bullets;
	DroppedWeaponControl *next;
};

DroppedWeaponControl *DroppedWeaponNode = NULL;

struct RP2PickedUpWeaponControl
{
	struct RP2WeaponListControl *PickedUpNode;
	int WeaponTotalAmmo;
	struct RP2PickedUpWeaponControl *next;
};

RP2PickedUpWeaponControl *RP2PickedUpWeaponNode = NULL;

struct RP2PUWCPlayerListControl
{
	char PlayerName[128];
	struct RP2PickedUpWeaponControl *List;
	struct RP2PUWCPlayerListControl *next;
};

RP2PUWCPlayerListControl *RP2PUWCPlayerListNode = NULL;

// New Fog/Weatch Control node is this
struct RP2WeatherStruct
{
	int AreaID;
	//Time it takes to switch to this weather node
	float TransitionTime;
	//Fog Stuff
	bool FogEnabled;
	float FogMinDist;
	float FogMaxDist;
	//Screen Color
	Vector3 ScreenColor;
	float ScreenOpacity;
	//Cloud Cover
	float CloudCover;
	float CloudGloominess;
	// Lightning
	float LightningIntensity;
	float LightningStart;
	float LightningEnd;
	float LightningHeading;
	float LightningDistribution;
	// War_Blitz
	float WarBlitzIntensity;
	float WarBlitzStart;
	float WarBlitzEnd;
	float WarBlitzHeading;
	float WarBlitzDistribution;
	// Wind
	float WindHeading;
	float WindSpeed;
	float WindVariability;
	// Particles
	float RainDensity;
	float SnowDensity;
	float AshDensity;
};

RP2WeatherStruct *DefaultRP2WeatherNode = NULL;
RP2WeatherStruct *ArcticRP2WeatherNode = NULL;
RP2WeatherStruct *RenhaloRP2WeatherNode = NULL;
RP2WeatherStruct *MarsRP2WeatherNode = NULL;
RP2WeatherStruct *RCRP2WeatherNode = NULL;
RP2WeatherStruct *CanadaRP2WeatherNode = NULL;
RP2WeatherStruct *PlayerWeatherNodes[128] = {NULL};

int JRP2SSBDGrid[128][25];
int JRP2SSBDGridTC[128];

int JRP2SSHDGrid[128][25];
int JRP2SSHDGridTC[128];

void JeradTrace(const char *Description)
{
	FILE *Trace;
	char JTrace[200];
	sprintf_s(JTrace,"JTrace.txt");
	fopen_s(&Trace,JTrace,"a+");
	if (Trace)
	{
		sprintf_s(JTrace,"%s\n",Description);
		fprintf_s(Trace,JTrace);
		fflush(Trace);
		fclose(Trace);
	}
}

class JMG_Slot_Machine_On_Poke : public ScriptImpClass {
	int didwin;
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Poked(GameObject *obj,GameObject *poker);
	void Animation_Complete(GameObject *obj,const char *anim);
};

class JMG_Visible_Infantry_In_Vehicle : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Attach_Turret_And_Send_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Flash_Light_Toggle : public ScriptImpClass {
	int FlashLightID;
	int LightOn;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj, GameObject *damager);
	void Destroyed(GameObject *obj);
	void CreateLight(GameObject *obj);
	void DestroyLight();
};

class JMG_Dual_Weapon_Script : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Free_For_All_Script : public ScriptImpClass {
	int FireTruckID;
	int Overweight;
	int CupboardID;
	int SeeIfVisible;
	int HealthRegen;
	RP2PlayerMissionsControl *MyMissionNode;
	int SellZoneID;
	int PlayerID;
	int PTUpdateWeaponsListDelay;
	int Ambulances[5];
	int MyHealthRegenTime;
	float MyHealthRegenAmount;
	float MyRehealCap;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj, GameObject *damager);
	void Destroyed(GameObject *obj);
	int Drop_Weapon(GameObject *obj,RP2WeaponListControl *Weapon);
	int Can_Drop_Weapon(GameObject *obj,RP2WeaponListControl *Weapon,RP2WeaponListControl *Weapon2);
	void Add_Mission_Node(GameObject *obj);
	void Vehicle_Mission_Conflict_Check(GameObject *obj,GameObject *sender);
	void Scan_For_Vehicle_Conflicts(GameObject *obj,RP2PlayerVehicleFailMissionControl *Node);
	int Sell_Weapon(GameObject *obj,RP2WeaponListControl *Weapon);
	float Weapon_Cost_Multiplier(int Cost);
	int Drop_Weapon_Single(GameObject *obj,RP2WeaponListControl *Weapon);
	int Sell_Weapon_Single(GameObject *obj,RP2WeaponListControl *Weapon);
	void Drop_Credit_Card_Info(GameObject *obj);
	void Security_Check(GameObject *obj);
	void Security_Check_Failed(GameObject *obj,const char *Preset,const char *PlayerName,const char *TestName);
	void Player_Character_Node(GameObject *obj);
	void Death_Drop_Weapon(GameObject *obj);
	void PTUpdateWeaponsList(GameObject *obj);
};

class JMG_Buy_Soldier_On_Preset_Entry : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Block_Refill_RP2 : public ScriptImpClass {
	int enabled;
	float currenthealth;
	float currentshield;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Druggy_Script : public ScriptImpClass {
	int CurrentDrugLevel;
	float DrugOverdoseLevel;
	float DrugDeathLevel;
	float WithdrawalLevel;
	int AllowHealthIncrease;
	int StillOnDrugs;
	int CanDisplayedDFTMessage;
	int ScreenEffectsEnabled;
	int CanDisplayedWithdrawalMessage;
	int CanDisplayedOverdoseMessage;
	int CanDisplayedDeathWarnMessage;
	int AllowHighHurt;
	int CanDisplayedNDLMessage;
	int WithdrawalDamageEnabled;
	float Red;
	float Green;
	float Blue;
	float AddedHealth;
	int PillsPickedUp;
	int PillsPickedUpSpeedBonus;
	float LastMax;
	float LastHealth;
	int AddedWeight;
	int SubtractedWeight;
	float DefaultWithdrawalLevel;
	float DefaultDrugOverdoseLevel;
	float DefaultDrugDeathLevel;
	int DropDrugs;
	float DrugLevelDropTime;
	int CantExitVehicle;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Injure_When_Withdrawal(GameObject *obj,int number3);
	void Hurt_While_High(GameObject *obj,int number4);
	void Effect_Wear_Off(GameObject *obj);
	void Detox_Used(GameObject *obj);
	void Overdose(GameObject *obj);
	void Overdosing_Kills(GameObject *obj);
	void Begin_Suffering_From_Withdrawal(GameObject *obj);
	void End_Suffering_From_Withdrawal(GameObject *obj);
	void Screen_Effects(GameObject *obj);
	void Reset_Drug_Script(GameObject *obj);
	void Take_Drugs(GameObject *obj,int param);
};

class JMG_Get_Drugs_Power_Up : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_No_Heal_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JMG_Parachute_Script : public ScriptImpClass {
	int create;
	int CE_ID[6];
	int Para_ID;
	int fallenough;
	Vector3 lastpos;
	int ChuteEnabled;
	int haspowerup;
	const char *armor;
	const char *health;
	int In_Water;
	int lastzmove;
	int SpaceBarPressed;
	int messagedisplayed;
	int Is_Attached;
	float Speed;
	float OldFacing;
	int SpeedUp;
	int SpeedUpCount;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Killed(GameObject *obj, GameObject *damager);
	void Destroy_Main_Chute_Object(GameObject *obj);
};

class JMG_Parachute_Power_Up : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Police_Car_Script : public ScriptImpClass {
	int lightson;
	int SirenID;
	int siren;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Water_Kill_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *enter);
};

class JMG_Boat_To_High : public ScriptImpClass {
	float Create_Height;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class Reset_Screen_Color_When_Created : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Plecos_Script : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Detox_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Drug_Health_Increase_Powerup_Monitor : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Prostitute_Script : public ScriptImpClass {
	float cost;
	int cost2;
	float moneygathered;
	int Current_ID;
	int hasacustomer;
	int haschosen;
	int hasclient;
	int iwta;
	int resetinprogress;
	const char *name;
	int count;
	int tofaraway;
	int resetcount;
	int resetpending;
	int DelayOn;
	int secondattempt;
	int choicetime;
	int hasbeenshot;
	int shotresetcount;
	int shotcount;
	float percentage;
	float myoriginalmaxshield;
	float hisoriginalmaxshield;
	int cmcn;
	int camcn;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj, GameObject *damager);
public:
	JMG_Prostitute_Script()
	{
		name = 0;
	}
	~JMG_Prostitute_Script()
	{
		if (name)
		{
			delete[] name;
		}
	}
};

class JMG_Keyboard_Input_Custom : public JFW_Key_Hook_Base {
	void Created(GameObject *obj);
	void KeyHook();
};

class JMG_Prostitute_Attach_Script : public ScriptImpClass {
	int hassentcustom;
	int prostitute;
	const char *name;
	int cost;
	int dip;
	int ttcyon;
	int Prostitute_ID;
	float percentage;
	int scriptopperational;
	float originalshield;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj, GameObject *damager);
};

class JMG_Sword_Script : public ScriptImpClass {
	float armor;
	float health;
	int canblock;
	int cnumber;
	int newtotal;
	bool enabled;
	int playingblockanimation;
	float sworddamageamount;
	int ObjectID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject *obj,const char *anim);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_Dual_P90_Script : public ScriptImpClass {
	int count;
	int resetcountcount;
	int Nnewtotal;
	int	Nclipbullets;
	int isdoingthing;
	int modelnum;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_RP2_Dual_Wield_Script : public ScriptImpClass {
	const char *originalmodel;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Swimming_Script : public ScriptImpClass {
	int isswimming;
	const char *mymodel;
	int ObjectID;
	const char *myweapon;
	int HeadID;
	int enabled;
	int lastwaterzone;
	int allowemptyhands;
	int drowntime;
	Vector3 lastpos;
	float speed;
	int scubamax;
	int scubacurrent; 
	int ScubaSoundID;
	int underwater;
	int allowupgrade;
	int uniquetankcheck[10];
	int HeightAdjustID;
	float EnterZoneHeight;
	int IsHeadSubmerged;
	char HeightAdjustAnimation[32];
	int CDT;
	int EnableEEnter;
	int canswim;
	int EffectControlID;
	float submergetime;
	Vector3 LastWaterZone;
	int High_Jump_Stopper;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
	void High_Jump_Stop(GameObject *obj);
	void Remove_Scuba_Tank_Sound();
	void Create_Swimmer(GameObject *obj);
	void Scuba_Tank_Upgrade_Tracker(GameObject *obj,int param,int message);
	void Exit_Water_Human(GameObject *obj);
	void Exit_Water_Into_Vehicle(GameObject *obj);
	void Exit_Vehicle_Into_Water(GameObject *obj);
	void Enter_Water_Human(GameObject *obj,GameObject *sender);
	void Incorrect_Model_Glitch_Test(GameObject *obj,GameObject *sender);
	void Air_Control(GameObject *obj,int param,int message,GameObject *sender);
	void Reset_Screen(GameObject *obj);
	void In_Water(GameObject *obj);
	void Double_Check_Swimming(GameObject *obj);
	void Retrive_Model_Glitch_Check(GameObject *obj);
	void Special_Water_Effects_Control(GameObject *obj,int message);
	void Key_Press_Swimming_Restart(GameObject* obj);
	void Height_Adjuster_Control(GameObject *obj);
	void Create_Scuba_Tank_Refil_Sound(GameObject *obj);
};

class JMG_Swimable_Water_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Swimming_Attach_Script : public ScriptImpClass {
	Vector3 pos;
	int forward;
	int backward;
	int StandingStill;
	int ASFBTRLUD;
	float oldfacing;
	int truningleft;
	int truningright;
	int up;
	int down;
	int HeadInWater;
	int underwater;
	int enabled;
	int OldCodeDIR;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Swimming_Head_Attach_Script : public ScriptImpClass {
	int tolong;
	int inwater;
	float screenred[2];
	float screengreen[2];
	float screenblue[2];
	float addredamount;
	float addgreenamount;
	float addblueamount;
	int Watchcolor[4];
	float watermurkyness[2];
	float watercolorthickness[2];
	int scubatankair;
	int UWBSID;
	int scubatankairmax;
	int canrepeat;
	int HeartBeatID;
	int CountDown;
	int CountDow3n;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Remove_Repeating_Sound_Object(int *ID);
};

class JMG_Remove_Self_On_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Freezer_Burn : public ScriptImpClass {
	int aco;
	int unfreeze;
	int FreezeID;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *damager);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Random_Splatters : public ScriptImpClass {
	int aco2;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *damager);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Vehicle_Entry_Control : public ScriptImpClass {
	int isnotdefault;
	int MarkerID;
	int enabled;
	int RemoveTime;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *damager);
	void Destroyed(GameObject *obj);
	int Warthog_Special(GameObject *Side1,GameObject *Side2,GameObject *Side3);
	int Warthog_Special2(GameObject *obj,GameObject *Side1,GameObject *Side2,GameObject *Side3);
};

class JMG_Real_Plane_Physics : public ScriptImpClass {
	float speed;
	int occupied;
	int FWID;
	int Rotation;
	float oldfacing;
	Vector3 oldpos;
	float distance;
	int alreadydieing;
	int OnGroundID;
	int changeog;
	int change;
	int OnGround;
	int PropellerID;
	int safetime;
	int FAID;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Advanced_Poke_Send_Custom : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_Play_Animation_On_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Weapon_Give_Powerup : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_War_Blitz_Create : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Keyboard_Input_Custom_ID_Veh : public JFW_Key_Hook_Base {
	void Created(GameObject *obj);
	void KeyHook();
};

class JMG_Keyboard_Vehicle_Input_Custom : public ScriptImpClass {
	int passenger[10];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_Play_Reload_Sound_On_Last_Shot : public ScriptImpClass {
	int isplayingsound;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_RP2_Teleport_Zone_Script : public ScriptImpClass {
	Vector3 Teleport_Pos;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_RP2_Teleport_Zone_Script_Attach : public ScriptImpClass {
	int Set_Facing_ID;
	int Movement_Object;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_RP2_Renhalo_Vortex : public ScriptImpClass {
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Find_And_Write_Positions : public ScriptImpClass {
	int count;
	float X[999];
	float Y[999];
	float Z[999];
	float time;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Send_Message_Until_Message_Reccived : public ScriptImpClass {
	int recived;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Random_Teleport_Zone_File : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Security_Printout_For_Teleport_Zone : public ScriptImpClass {
	float X[999];
	float Y[999];
	float Z[999];
	void Created(GameObject *obj);
};

class JMG_Set_Rotation_To_Nearest_Soldier : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Network_Link_Beacon : public ScriptImpClass {
	int PositionObject;
	int Portal;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Destroyed(GameObject *obj);
};

class JMG_Set_Ammo_On_Enter_Reset_On_Exit : public ScriptImpClass {
	int OriginalBullets[125];
	int OriginalClipBullets[125];
	int IDOfSoldier[125];
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
};

class JMG_Send_Custom_With_ID_Of_Enter_Or_Exiter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
};

class JMG_Change_Or_Reset_Sender_Ammo_On_Custom : public ScriptImpClass {
	int OriginalBullets[125];
	int OriginalClipBullets[125];
	int IDOfSoldier[125];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Play_Animation_On_Create : public ScriptImpClass {
	void Created(GameObject *obj);
};

// Moved to new location for the new swimming code
/*class JMG_RP2_Vehicle : public ScriptImpClass {
	int idstore[10];
	int enginedisabled;
	int Is_In_Water;
	float colornfog[5];
	int DamageEffectIDs[2];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *damager);
	void JMG_Force_Occupants_Exit(GameObject *obj);
};*/

class JMG_Set_Random_Rotation : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Random_Explosive_Barrel : public ScriptImpClass {
	int hasbeenhit[3];
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject *obj,const char *anim);
};

class JMG_Dont_Be_A_Douche : public ScriptImpClass {
	int MarkerID;
	int bulletcount;
	int clipcount;
	int AttackerID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
	void Destroyed(GameObject *obj);
	int JMG_Dont_Be_A_Douche::Special_Scud_System(GameObject *obj);
};

class JMG_Level_Fog_And_Screen_Color_Defaults : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Drown_Time_Modifier : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Grant_Powerup_To_Vehicle_Owner : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Remove_Object_When_Creator_Destroyed : public ScriptImpClass {
	const char *ownerspreset;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Enable_Flash_Light_On_Pickup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Nod_PT_Zone : public ScriptImpClass {
	int ZoneOccupants;
	int PlayerID[128];
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_GDI_PT_Zone : public ScriptImpClass {
	int ZoneOccupants;
	int PlayerID[128];
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Remove_Object_When_Timer_Expired : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Swimming_Water_Effects_Script : public ScriptImpClass {
	int ObjectID;
	int underwater;
	int hastriggered;
	int SwimmerID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Do_Not_Drive_In_Water : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Scuba_Tank_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Send_Custom_To_Self_On_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Ferriswheel_Script : public ScriptImpClass {
	float CurrentFrame;
	int forward;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_RP2_Death_Match_PT_Assister : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Random_Facing_On_Create : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Move_And_Then_Remove_When_Creator_Removed : public ScriptImpClass {
	const char *ownerspreset;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Create_Object_Attach_Script_Random_Facing : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Helicopter_Drop_Off_AI : public ScriptImpClass {
	int IsHeadingOut;
	Vector3 Target_Pos;
	Vector3 Temp_Target_Pos;
	int IHTTP;
	Vector3 Last_Position;
	int ChangeObjectiveCount;
	Vector3 Start_Position;
	int VehicleID;
	int HarnessID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_Helicopter_AI_Beacon : public ScriptImpClass {
	GameObject *object;
	void Created(GameObject *obj);
};

class JMG_Buy_Weapon_On_Poke_Advanced : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_Buy_Armor_On_Poke_Advanced : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_Visible_Infantry_In_Vehicle2 : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Advanced_Visible_Driver : public ScriptImpClass {
	int idstore[100];
	int idstore2[100];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_RP2_Random_Teleport_Zone : public ScriptImpClass {
	int Object_ID[7];
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void RTeleportCall(GameObject *obj,GameObject *TargetSpot);
};

class JMG_RP2_Cop_Arrest_Script : public ScriptImpClass {
	int Anti_Douche_Active;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Jail_Cell_Control : public ScriptImpClass {
	int Object_ID[6];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Renhalo_Dart_Gun_Script : public ScriptImpClass {
	float TimerTime;
	float Damage;
	int DartLauncherID;
	int DamagerCount;
	int Gun;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Jerad_Energy_Shield : public ScriptImpClass {
	int canchargelevel;
	float oldhealth;
	float oldshield;
	int ChargeObjectID;
	int CriticalBeepID;
	int WarningBeepID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
	void Destroyed(GameObject *obj);
};

class JMG_RP2_Jerad : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Keyboard_Input_Custom_ID_Veh2 : public JFW_Key_Hook_Base {
	void Created(GameObject *obj);
	void KeyHook();
};

class JMG_Vehicle_Key_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Switch_Weapon : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Health_Regen_Fixed : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Check_Model_Match_Or_Change : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Throw_Script : public ScriptImpClass {
	int ObjectID;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_RP2_Teleport_On_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_RP2_Elevator_Control : public ScriptImpClass {
	int enabled;
	int isup;
	int SwitchArray[10];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Create_Elevator_Switch_Attach_To_Bone : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Mindless_Zombie_Hunt : public ScriptImpClass {
	int CurrentTarget;
	int huntorattack;
	int waitcount;
	Vector3 homelocation;
	float speed;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Attach_Script_To_All_Players : public ScriptImpClass {
	char *params;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

struct PlayDataNode
{
	const char *Name;
	int Team;
	int Spawns;
	int Count;
	int Kills;
	int Betrails;
	int Idle;
	int Suicide;
	int PlayerID;
	int IsSpecial;
	float Score;
	struct PlayDataNode *next;
};

class JMG_Renegade_Mutation_Mode : public ScriptImpClass {
	PlayDataNode *Players;
	int Count;
	int Mutants;
	int StartingTeam;
	int HasDoneStartCheck;
	const char *MutantStartingCharacter;
	const char *MutantStartingCharacterLevel2;
	const char *MutantStartingCharacterLevel3;
	const char *MutantStartingCharacterSpecial1;
	const char *MutantStartingCharacterSpecial2;
	const char *HumanStartingCharacter;
	int MinPlayers;
	int Start_Override_Time;
	int ControllerID;
	int IsLastManStanding;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
	void Creation_Message_Reccived(int SenderID);
	void Suicide_Message_Reccived(int SenderID);
	void Killed_Message_Reccived(int SenderID,int KillerID);
	void Betrayed_Message_Reccived(int SenderID,int BetrayerID);
	void Change_Player_Data_By_Count_ID(int CountID,int NewTeam,int NewSpawns,int NewKills,int NewBetrails,int NewSuicide,int NewISSpecial);
	void PrintNamesandIDs();
	void CheckForRelogGlitching();
	void Start_Game();
	void Empty_Players_List();
	void End_Game_Successful();
	void Default_Spawn_Setup(PlayDataNode *Current,int AddCount);
	void Disable_Purchase_Terminals(GameObject *obj);
	void Base_Invulnerable();
	void Kill_Beacon_Zones(GameObject *obj);
	void Remove_Harvesters();
	int Last_Man_Standing();
	int End_Last_Man_Standing(int XLastManID);
	GameObject *FindPlayerByCount(int CountID);
};

class JMG_Renegade_Mutation_Mode_Infantry_Controller : public ScriptImpClass {
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_Renegade_Mutation_Mode_Start_Game_Key : public JFW_Key_Hook_Base {
	void Created(GameObject *obj);
	void KeyHook();
};

class JMG_Simple_Timed_Heal_Regen : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Player_Path_Creator : public ScriptImpClass {
	FILE *file;
	int lines;
	int canwrite;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_AI_Path_Controller : public ScriptImpClass {
	float X[8][180];
	float Y[8][180];
	float Z[8][180];
	int AIID[10];
	int AIPath[10];
	int maxdifferent;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Follow_User_Made_Path_AI : public ScriptImpClass {
	int CurrentTarget;
	int huntorattack;
	int waitcount;
	Vector3 GoToLocation;
	int spot;
	float lastdistance;
	float lastdistancenochagewait;
	int waitingnextpos;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Destroyed(GameObject *obj);
};

class JMG_Single_Player_Only_Script : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Set_Position_On_Create : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Chinook_Door_Control : public ScriptImpClass {
	//char Animation[32];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_RP2_Vehicle_Animate_While_Firing : public ScriptImpClass {
	char Animation[32];
	int oldbullets;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_RP2_Air_Craft_Cloud_Script : public ScriptImpClass {
	int NumberOfClouds;
	int CloudID[10];
	int EngineEnabled;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Killed(GameObject *obj,GameObject *damager);
	void Destroyed(GameObject *obj);
};

class JMG_Swimming_Under_Water_Effects_Control : public ScriptImpClass {
	Vector3 oldpos;
	int PlantID;
	int enabled;
	char Animation[34];
	char Animation2[34];
	int toggle;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Change_Armor_Type_On_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Simple_Prone_On_Custom : public ScriptImpClass {
	int IsProne;
	int animationcomplete;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Animation_Complete(GameObject *obj,const char *anim);
};

class JMG_Enter_Console_Command_On_Create : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Swimming_Underwater_Effect_Script : public ScriptImpClass {
	int EffectID;
	int isvisible;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Visible_Infantry_In_Vehicle_Attach_Script : public ScriptImpClass {
	float turninganimation[6];
	int WheelMonitorID;
	int AnimationState;
	float LastDamage;
	int Last_Damager_ID;
	int DamagerDelay;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject *obj,const char *anim);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *damager);
	void SetupAnimation(GameObject *obj);
};

class JMG_Vehicle_Occupant_Visible_And_Damageable : public ScriptImpClass {
	int VehicleID;
	int idstore[100];
	int idstore2[100];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *damager);
	void Destroyed(GameObject *obj);
	GameObject *Choose_Connection_Object(GameObject *obj);
};

class JMG_RP2_Set_Map_Version : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Object_Toggle_On_Vehicle_Enter_Exit : public ScriptImpClass {
	int ObjectID[10];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_Object_Toggle_On_Vehicle_Enter_Exit_Controller : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Repair_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
};

class JMG_Repair_Zone_Attach_Script : public ScriptImpClass {
	int repair;
	int delay;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Vehicle_Sell_Price : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Attach_Script_To_All_Specified_Presets : public ScriptImpClass {
	char *params;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Random_Civilian_Model : public ScriptImpClass {
	void Created(GameObject *obj);
	int Select_Player_Character(GameObject *obj);
};

class JMG_Sell_Vehicle_On_Custom : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	float Vehicle_Price_Calculation(GameObject *obj,float Cost,GameObject *VehicleDriver);
	void JMG_Force_Occupants_Exit(GameObject *obj);
	int Warthog_Special(GameObject *Side1,GameObject *Side2,GameObject *Side3);
};

class JMG_Vehicle_Sell_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
};

class JMG_Buy_Preset_On_Poke : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Vehicle_Door_Script : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Simple_Vehicle_Fire_Animation_Script : public ScriptImpClass {
	int hasbullet;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_Kill_All_In_Range_On_Create : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Kill_All_In_Range_On_Create_If_Above : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Kill_Nearest_Unit_Type_When_Stuck : public ScriptImpClass {	
	Vector3 LastPos;
	int StuckCount;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Return_Fire_On_Damager : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_Return_Fire_On_Damager_2 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	GameObject *JMG_Find_Closest_Non_Building_Object_By_Team(GameObject *obj,int Team,float MaxRange,Vector3 position);
};

class JMG_Motion_Mine_Script : public ScriptImpClass {
	int CurrentTargets[10];
	Vector3 TargetPositions[10];
	int OwnerID;
	float DetectRange;
	float DetectSpeed;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	//void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Get_Target(GameObject *obj);
	int Usable_Target(GameObject *obj);
	float Lazy_Distance_Comp_Script(Vector3 MyPos,Vector3 Pos);//Created to reduce CPU load
};

class JMG_Attach_Script_Collector_Once : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

struct PhoneNumberControl
{
	int PlayerID;
	int PhoneNumberLength;
	char PhoneNumber[200];
	char PhonePound[200];
	int PhonePoundLength;
	int StarPressed;
	struct PhoneNumberControl *next;
};

struct CellPhoneNumberControl
{
	int PlayerID;
	int NumberLength;
	char PlayerName[256];
	char MyPhoneNumber[200];
	int TargetID;
	struct CellPhoneNumberControl *next;
};

struct PlayerRedialNumberControl
{
	char PlayerName[256];
	int NumberLength[10];
	char PhoneNumber0[200];
	char PhoneNumber1[200];
	char PhoneNumber2[200];
	char PhoneNumber3[200];
	char PhoneNumber4[200];
	char PhoneNumber5[200];
	char PhoneNumber6[200];
	char PhoneNumber7[200];
	char PhoneNumber8[200];
	char PhoneNumber9[200];
	struct PlayerRedialNumberControl *next;
};

struct PlayerCellPhoneIDControl
{
	int PlayerID;
	int DialToneID;
	int BusySoundID;
	int RingSoundID;
	int OffHookSoundID;
	int CallWaitingID;
	int PhoneDisconnectID;
	struct PlayerCellPhoneIDControl *next;
};

PhoneNumberControl *PhoneNumbers = NULL;
CellPhoneNumberControl *CellPhoneNumberNode = NULL;
PlayerRedialNumberControl *PlayerRedialNumberNode = NULL;
PlayerCellPhoneIDControl *PlayerCellPhoneIDNode = NULL;

enum {PHONESpecial1Key = 40860504,PHONESpecial2Key = 40860505,PHONEUserDied = 40860596,PHONELostSignal = 40860590,PHONEAquiredSignal = 40860591,PHONEOtherGuyHungUp = 40860593,
PHONEResetCustom = 40860516,PHONECallRingDelay = 40860595,PHONEBombCallRingDelay = 40860598,PHONEBombPhoneRing = 40860546,PHONEBombDetonate = 40860545,
PHONETriggerBomb = 40860597,PHONEFoundOtherPlayerStandBy = 40860594,PHONECallReccived = 40860592,PHONENumberRequires1Or2Entry = 408605,PHONEStopRingingNumberReached = 408605,
PHONENumberRequires1Or2EntryNoDiscard = 408606,PHONEOtherEndHangUp = 4086051,PHONELineBusy = 4086052,PHONECallWaiting = 4086053,PHONENumberRequiresAdditionalNumberAndPound = 4086054,
PHONEHousingKeyTransferCustom = 40860508,PHONECheckForRedialNumberMatchReturn = 11911413,PHONERequestKnownPhoneNumberList = 40860501,PHONESendSenderTheDisconnectSignal = 40860589,
PHONECallStartCustom = 40860500,PHONESearchForMatchingNumbersCustom = 11911412,PHONENumberCallTimeMonitor = 40860506,PHONENumberEndCallMessage = 40860503,PHONENumberRequestSingleDigitPress = 40860577};

class JMG_RP2_Player_Phone_Control_Script : public ScriptImpClass {
	char cnumber[200];
	int Number;
	int ReachedNumber;
	int CallInProgress;
	int RedialFast[2];
	char BackupNumber[200];
	int PhoneBookID;
	int PhoneOut;
	int PhoneEnabled;
	int RingThrough;
	int SpecialKeys;
	int CallComplete;
	int StarPress;
	int WaitingForPound;
	int MyIDPC;
	int TraceDepthPC;
	CellPhoneNumberControl *PersonalPhoneNumber;
	int CallerID;
	int PhoneLineID;
	Vector3 LastPos[2];
	int RingUp;
	int ClearLines;
	int FirstTimeOut;
	int PoundPress;
	PlayerRedialNumberControl *PlayerRedialNumbers;
	int Powered;
	int HousingKeyTransfer;
	int HoldSpecial;
	int InVehicle;
	char MyPhoneNumber[200];
	int IsSpamming;
	int MissedCalls;
	int TurnedOff;
	int FoundMatch;
	int WaitingForDescription;
	PhoneNumberControl *ShortTermUsage;
	PlayerCellPhoneIDControl *PlayerIDNode;
	int SpecialID;
	float SpecialCount;
	float SpecialCountMax;
	int NewSpecialNumberPress;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
	void PlayerNumberControl(GameObject *sender);
	void ClearNumbers();
	void DisplayCurrentNumber(GameObject *obj);
	void PressNumber(GameObject *obj,char Number1);
	int Repeating_Sound_Object(GameObject *obj,const char *Model);
	void Remove_Repeating_Sound_Object(int *ID);
	void Reset_Phone(GameObject *obj);
	void Redial_Sound(GameObject *obj,char Number1);
	void PlayerPoundNumberControl(GameObject *obj);
	void Phone_Trace(const char *Description,int num,int ClearFile);
	void Phone_Ring_Timeout(GameObject *obj);
	void Phone_Position_Control(GameObject *obj);
	void Phone_Redial_Sound_Trigger(GameObject *obj);
	void Call_Complete(GameObject *obj);
	void LostSignal(GameObject *obj);
	void SignalDetected(GameObject *obj);
	void NumberKeyPressed(GameObject *obj,int message);
	void ClearKeyPressed(GameObject *obj);
	void StarKeyPressed(GameObject *obj);
	void PoundKeyPressed(GameObject *obj);
	void ListPhoneNumbers(GameObject *obj,int channel);
	void DisplayCurrentlyEnteredNumber(GameObject *obj);
	void DeleteKeyPressed(GameObject *obj);
	void SendKeyPressed(GameObject *obj);
	void WaitingForAdditionalNumber(GameObject *obj,int param);
	void CallWaitingBeep(GameObject *obj);
	void PhoneLineBusy(GameObject *obj);
	void PhoneCallPickedUp(GameObject *obj,int param);
	void PhoneCallPickedUpSimple(GameObject *obj);
	void CallCompleteMessage(GameObject *obj);
	void AddPersonalNumberNode(GameObject *obj);
	void PhoneIdleAndCalled(GameObject *obj,int param);
	void PhoneInUseAndCalled(GameObject *obj,int param);
	void PhoneCallDelayRelay(GameObject *obj);
	void HumanCallObjectFound(GameObject *obj,int param);
	void ContactKilledRS(GameObject *obj);
	void Phone_Busy_Startup(GameObject *obj);
	void Phone_Disconnect(GameObject *obj);
	void RedialNodeSetup(GameObject *obj);
	void NumberKeyPressedStorage(GameObject *obj,int message);
	void NumberKeyPressedRedial(GameObject *obj,int message,int PoundKeyNeeded);
	void ListStoredPhoneNumbers(GameObject *obj);
	void DisplayMyPhoneNumber(GameObject *obj);
	void PowerButton(GameObject *obj);
	void SendPhoneNumberToRecciver(GameObject *obj);
	char *JMG_RP2_Player_Phone_Control_Script::FormatPhoneNumber(char PhoneNumber[]);
	void DefinePhoneNumberNode(GameObject *obj);
	void PhoneDefineNumberReturn(GameObject *obj,int param);
	void ExtractRedialNumber(const char *PNumber,int Number);
	void SoundIDSetupNode(GameObject *obj);
	void Phone_Called_Select_State(GameObject *obj,int param);
	void Phone_OffHook(GameObject *obj);
	void PressNumberSpecial(GameObject *obj,char Number1);
	int PressAnyKey(int message);
};

class JMG_RP2_Phone_Number_Message : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Attach_Script_To_Poker : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_Phone_Create_Cinematic_At_Custom_Sender_Position : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Phone_Enable_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_RP2_Phone_Disable_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_RP2_Phone_Disabled_While_In_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JMG_RP2_Phone_Advanced_Phone_Number_Control_System : public ScriptImpClass {
	RP2PhoneTextControl *MyTextNode;
	char PhoneNumber[237];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void AutoCancelCall(GameObject *obj,GameObject *player);
	void ChargeForCall(GameObject *obj);
	RP2PhoneTextControl *MyNode(int ScriptID);
public:
	char *FormatPhoneNumber(const char *PN);
};

class JMG_RP2_Phone_Create_Preset_At_Sender_Player : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Phone_Play_Sound_At_Sender_Player : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Phone_Hidden_Number_Allow_Display : public ScriptImpClass {
	int StoredID[128];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Phone_Add_Number_To_List_Zone : public ScriptImpClass {
	int PhoneBookID;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_Custom_Multiplier : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Grant_Weapon_On_Weapon : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

struct SwimmingWaterZoneTypeControl
{
	int WaterZoneType;
	float ScreenRed;
	float ScreenGreen;
	float ScreenBlue;
	float AddRedAmount;
	float AddGreenAmount;
	float AddBlueAmount;
	float WaterMurkyness;
	float WaterColorThickness;
	float Depth_Before_Water_Darkens;
	const char *Small_Splash;
	const char *Medium_Splash;
	const char *Large_Splash;
	const char *Huge_Splash;
	const char *Underwater_Effect_Controller;
	const char *Surface_Water_Effect;
	const char *Underwater_Effect;
	const char *Water_Surface_Ambient_Sound;
	const char *Underwater_Ambient_Sound;
	const char *Scuba_Mask_Emitter;
	float VehicleScreenRed;
	float VehicleScreenGreen;
	float VehicleScreenBlue;
	float VehicleWaterMurkyness;
	float VehicleWaterColorThickness;
	float VehicleWaterDamage;
	int SwimmingAnimationTypeOverride;
	float InfantryWaterDamage;
	int InfantryWaterDamageDelay;
	struct SwimmingWaterZoneTypeControl *next;
};

struct SwimmingAnimationTypeControl
{
	int SwimmingAnimationType;
	const char *Skeleton;
	const char *Animation_Name;
	const char *Not_Moving;
	const char *Forward;
	const char *Backward;
	const char *Turn_Left;
	const char *Turn_Right;
	const char *Up;
	const char *Down;
	const char *Tilt_Up;
	const char *Tilt_Down;
	const char *Tilt_Up_Backward;
	const char *Tilt_Down_Backward;
	const char *Swim_Left;
	const char *Swim_Left_Tilt_Up;
	const char *Swim_Left_Tilt_Down;
	const char *Swim_Right;
	const char *Swim_Right_Tilt_Up;
	const char *Swim_Right_Tilt_Down;
	const char *Not_Moving_Surface;
	const char *Forward_Surface;
	const char *Backward_Surface;
	const char *Turn_Left_Surface;
	const char *Turn_Right_Surface;
	const char *Left_Surface;
	const char *Right_Surface;
	//const char *Water_Jump; Wasn't ever used after version 0.7 of the original swimming script, it has been removed
	struct SwimmingAnimationTypeControl *next;
};

struct RP2VehicleControl
{
	int VehicleID;
	int AirSupply;
	int ArraySpot;
	float mps[10];
	float MPS[10];
	float MPH;
	int OnCarryAll;
	int IsWarthog;
	int EMPTime;
	struct RP2VehicleControl *next;
};

struct SwimmingScubaGearControl
{
	int ScubaGearType;
	int HoldAirAmount;
	int EmitsBubbles;
	int PurchaseCost;
	int UpgradeMaxAir;
	float TankHealth;
	float TankArmor;
	const char *TankSkin;
	const char *TankShield;
	const char *SurfaceExplosion;
	const char *UnderwaterExplosion;
	const char *TankModel;
	const char *AttachBone;
	struct SwimmingScubaGearControl *next;
};

struct RP2SwimmingZoneTypeIndexControl
{
	int ZoneID;
	int ZoneType;
	RP2SwimmingZoneTypeIndexControl *next;
};

SwimmingWaterZoneTypeControl *SwimmingWaterZoneTypeNode = NULL;
SwimmingAnimationTypeControl *SwimmingAnimationTypeNode = NULL;
RP2VehicleControl *RP2VehicleNode = NULL;
SwimmingScubaGearControl *SwimmingScubaGearNode = NULL;
RP2SwimmingZoneTypeIndexControl *RP2SwimmingZoneTypeIndexNode = NULL;

class JMG_New_Swimming_Water_Type : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_New_Swimming_Animation_Type : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_New_Swimming_Script : public ScriptImpClass {
	int isswimming;
	int ObjectID;
	char myweapon[2000];
	int HeadID;
	int InVehicle;
	int CurrentWaterType;
	int allowemptyhands;
	int drowntime;
	Vector3 lastpos;
	float speed;
	int ScubaMax;
	int scubacurrent; 
	int ScubaSoundID;
	int underwater;
	int allowupgrade;
	int uniquetankcheck[10];
	int HeightAdjustID;
	float EnterZoneHeight;
	char HeightAdjustAnimation[32];
	int CDT;
	int EnableEEnter;
	int CanSwim;
	float submergetime;
	Vector3 LastWaterZone;
	// Head Mix ins
	float screenred[2];
	float screengreen[2];
	float screenblue[2];
	float addredamount;
	float addgreenamount;
	float addblueamount;
	float watermurkyness[2];
	float watercolorthickness[2];
	int UWBSID;
	int HeartBeatID;
	int CountDown;
	int CountDow3n;
	// Back to swimming 
	int MyIDNS;
	int TraceDepth;
	int EEnter;
	int NoSwimID;
	int HJROWP;
	int HeadPos;
	SwimmingWaterZoneTypeControl *WaterType;
	RP2VehicleControl *MyVehicle;
	SwimmingScubaGearControl *MyScubaTank;
	float ScubaTankHealth;
	float ScubaTankArmor;
	int VisibleTankID;
	int MaskEmitBubbles;
	int CanSwimToggleProcess[5];
	int SwimmingToggleOverFlowBuffer[100];
	int ArrayPosition;
	int CantExitVehicle;
	int InWaterTime;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
	void High_Jump_Stop(GameObject *obj);
	void Create_Swimmer(GameObject *obj);
	void Scuba_Tank_Upgrade_Tracker(GameObject *obj,int param,int message,GameObject *sender);
	void Exit_Water_Human(GameObject *obj,GameObject *sender);
	void Enter_Vehicle(GameObject *obj,int param,GameObject *sender);
	void Exit_Vehicle(GameObject *obj);
	void Enter_Water_Human(GameObject *obj,int param,GameObject *sender);
	void Air_Control(GameObject *obj,int param,int message,GameObject *sender);
	void Reset_Screen(GameObject *obj);
	void In_Water(GameObject *obj);
	void Double_Check_Swimming(GameObject *obj);
	void Underwater_Tracking_Control(GameObject *obj,int message);
	void Key_Press_Swimming_Restart(GameObject* obj);
	void Height_Adjuster_Control(GameObject *obj);
	void Create_Scuba_Tank_Refil_Sound(GameObject *obj);
	void Remove_Repeating_Sound_Object(int *ID);
	void Scuba_Tank_In_Water_Control(GameObject *obj);
	void Screen_Effects(GameObject *obj);
	void Swimming_Trace(const char *Description,int num);
	void Attempt_Vehicle_Enter(GameObject *obj);
	void Kill_Fake_Swimmer(GameObject *obj);
	void Create_Head(GameObject *obj,GameObject *attach,const char *attachbone,int HeadPosition);
	void Reload_Screen_Effects();
	void Additional_High_Jump_Check(GameObject *obj,int param);
	const char *Scuba_Tank_Air_Time(char text[]);
	void Action_Key_Pressed(GameObject *obj);
	void Swimming_Switch_Weapons(GameObject *obj);
	void Reset_Weapon_On_Water_Exit(GameObject *obj);
	int Vehicle_Air_Supply(GameObject *obj);
	void Create_Scuba_Tank(GameObject *obj);
	void Stop_Water_Flyout(GameObject *obj);
	void EnableActionKeyEnter(GameObject *obj);
	void DrownDamageSystem(GameObject *obj);
	void DrownDamageSystemHealthOnly(GameObject *obj);
	void Create_Scuba_Tank(GameObject *Player,GameObject *obj);
	void Scuba_Tank_Killed(GameObject *Player,GameObject *obj);
	void Remove_Scuba_Tank();
	void Swimming_Toggle_Control(GameObject *obj,int param);
	void Enable_Swimming_Toggle();
	void Toggle_Swimming();
	void WaterZoneHeight(GameObject *Zone);
	void DrownedText(GameObject *obj);
	void RefindSwimmingZone(GameObject *obj);
	void CheckForUnderwaterZoneMiscount(GameObject *obj,GameObject *Head);
	void StartSwimmingUnderwater(GameObject *obj);
	void StartSwimmingOnSurface(GameObject *obj);
	void TakeWaterDamage(GameObject *obj);
	GameObject *FindCurrentWaterZone(GameObject *obj);
};

class JMG_New_Swimming_Head_Attach_Script : public ScriptImpClass {
	int inwater;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_New_Swimming_Attach_Script : public ScriptImpClass {
	Vector3 pos;
	int forward;
	int backward;
	int StandingStill;
	int ASFBTRLUD;
	float oldfacing;
	int truningleft;
	int truningright;
	int up;
	int down;
	int HeadInWater;
	int underwater;
	int enabled;
	int OldCodeDIR;
	int hastriggered;
	int ObjectID;
	int SoundID;
	int left;
	int right;
	SwimmingAnimationTypeControl *AnimationType;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *damager);
	int Repeating_Sound_Object(GameObject *obj,const char *Sound);
	void Remove_Repeating_Sound_Object(int *ID);
};

class JMG_New_Swimming_Zone : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
};

class JMG_New_Swimming_Scuba_Tank_Type : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_New_Swimming_Scuba_Tank_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_New_Swimming_Scuba_Tank_Attach_Script : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_RP2_Vehicle : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Vehicle_Advanced : public ScriptImpClass {
	int idstore[10];
	int enginedisabled;
	int Is_In_Water;
	int DamageEffectIDs[2];
	int UnderWaterAirSupply;
	SwimmingWaterZoneTypeControl *WaterType;
	RP2VehicleControl *MyNode;
	Vector3 LPosition;
	bool CanBurn;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *damager);
	void JMG_Force_Occupants_Exit(GameObject *obj);
	void AddNode(GameObject *obj);
	void TakeDamageOnExit(GameObject *obj,GameObject *sender);
};

class JMG_RP2_Submarine_Script : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Submarine_Script_Advanced : public ScriptImpClass {
	int Sub_Block_ID;
	int cancollide;
	int idstore[10];
	int Is_In_Water;
	int Pinging;
	int UnderWaterEffectsID;
	int SubAlarmID;
	int PingTime;
	int PingReboundStart;
	int PingReboundEnd;
	int MaxAir;
	Vector3 LPosition;
	SwimmingWaterZoneTypeControl *WaterType;
	RP2VehicleControl *MyNode;
	int Sub_Block_ID2;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
	void AddNode(GameObject *obj);
	void PingSound(GameObject *Sub);
};

class JMG_RP2_Special_Death_Control : public ScriptImpClass {
	int aco2;
	int HoldID;
	int aco2reset;
	int unfreeze;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *damager);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Phone_Kill_Sender_Player : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

struct RP2NewHouseSystemControl
{
	int HouseAddress;
	int ControllerID;
	int SpawnObjectID;
	int IsOwned;
	char PlayerName[256];
	int NumberOfPrePays;
	int RentTime;
	int HouseYear;
	int HouseDay;
	int RentPrice;
	struct RP2NewHouseSystemControl *next;
};

RP2NewHouseSystemControl *RP2NewHouseSystemNode = NULL;

struct WeaponCupboardControl
{
	int ID;
	char Weapon[256];
	struct WeaponCupboardControl *next;
};

WeaponCupboardControl *WeaponCupboardNode = NULL;

struct PlayerHouseVehicleControl
{
	int House_Address;
	char VehiclePreset[256];
	Vector3 VehiclePosition;
	float VehicleFacing;
	int ZoneID;
	float HPPercent;
	int IsSetup;
	struct PlayerHouseVehicleControl *next;
};

PlayerHouseVehicleControl *PlayerHouseVehicleNode = NULL;

class JMG_RP2_House_Key_Control : public ScriptImpClass {
	int OwnedHouses[100];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void AddUpdateHouseNode(const char *OwnerName,int HouseNumber,int NumberOfPrePays,int RentTime,int HouseYear,int HouseDay);
};

class JMG_RP2_Grant_House_Keys_On_Spawn : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Open_Door_On_Poke_And_Key : public ScriptImpClass {
	int enabled;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Poked(GameObject *obj,GameObject *poker);
	void Animation_Complete(GameObject *obj,const char *anim);
};

class JMG_RP2_House_Address_Controller : public ScriptImpClass {
	RP2NewHouseSystemControl *HouseNode;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Phone_House_Phone_Number_Control_System : public ScriptImpClass {
	char PhoneNumberMessage[256];
	int HouseAddress;
	RP2NewHouseSystemControl *CurHouse;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void AutoCancelCall(GameObject *obj,GameObject *player);
	void ChargeForCall(GameObject *obj);
};

class JMG_RP2_RailRoad_Crossing_Control_Zone : public ScriptImpClass {
	int GateState;
	int TrainsInZone;
	int TrainID[128];
	char GateAnimation[256];
	int SoundID;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Street_Sign_Crushable : public ScriptImpClass {
	int SignControllerID;
	int SignID;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_RP2_Phone_Create_Test_Object : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Node_Cleanup : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	char *Encrypt(const char *String,int EncryptionFactor,int Start);
};

class JMG_RP2_Phone_Teleport_Sender_Player : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Remove_Object_When_Timer_Expired2 : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Turret_Attach : public ScriptImpClass {
	int ObjectID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_Attach_Script_To_All_AI_Infantry : public ScriptImpClass {
	char *params;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Remove_All_AI_Infantry : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Remove_All_Infantry : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Record_Vehicles_At_End_Of_Game : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Do_Not_Record_Vehicle : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Please_Record_Vehicle : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Create_Vehicles_At_Start_Of_Game : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Checked_If_Recorded_Vehicle_Stuck : public ScriptImpClass {
	int MoveCount;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_RP2_Map_Vehicle_Control : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Main_Map_Game_Control : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
public:
	void Server_Version();
};

class JMG_RP2_Phone_Player_Phone_Number : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Grant_Powerup_Weapon_Limit : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void DropGun(GameObject *obj,int Bullets,float SpawnTime,int Add);
	float SpawnTime();
};

class JMG_RP2_Decoy_Recreate_Powerup : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Zone_Remove_Weapon_On_Entry : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_RP2_WLS_Add_Weapon_Type : public ScriptImpClass {
	void Created(GameObject *obj);
	void Add_Node(GameObject *obj);
};

class JMG_RP2_WLS_Buy_Weapon_Poker : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_RP2_WLS_Weapon_Spawner_Control : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_WLS_Grant_Weapon_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_RP2_WLS_Grant_Weapon_Poke_Special : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_RP2_WLS_Grant_Weapon : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_WLS_Grant_Weapon_Special : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_WLS_Buy_Weapon_Vehicle_Owner : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_WLS_Zone_Remove_Weapon : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_RP2_Set_Up_Weapons_List : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_WLS_New_Grant_Powerup_Weapon_Limit : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void DropGun(GameObject *obj,int Bullets,float SpawnTime,int Add);
	void DropGunSpecial(GameObject *obj,int Bullets,float SpawnTime,int Add);
	float SpawnTime();
	void FailPickup(GameObject *obj,GameObject *sender,int PlayerID,RP2WeaponListControl *TheWeapon);
	void FillTheGun(GameObject *obj,int GunBullets,int *DropWeapon,GameObject *sender,RP2WeaponListControl *TheWeapon);
	void FillTheObject(GameObject *obj,int GunBullets,int LeftOvers,int *DropWeapon,GameObject *sender,RP2WeaponListControl *TheWeapon);
};

class JMG_RP2_WLS_New_Decoy_Recreate_Powerup : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Housing_Weapons_Cupboard_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JMG_RP2_Housing_Weapons_Cupboard_WLS : public ScriptImpClass {
	RP2WeaponListControl *Weapon;
	RP2NewHouseSystemControl *HouseNode;
	int MagazineSize;
	float AmmoPercent;
	int GiveOutWeapon;
	int DownTime;
	WeaponCupboardControl *WeaponNode;
	int CanState;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Poked(GameObject *obj,GameObject *poker);
	void Find_House_Node();
	int Is_Owned();
	void DataNode(GameObject *obj);
	unsigned long Get_Translated_Weapon_ID(GameObject *obj,int position);
	char *Get_Weapon_Name();
	int Grant_Ammo_Amount();
};

class JMG_RP2_Weapons_Cupboard_File_Control : public ScriptImpClass {
	void Created(GameObject *obj);
	void DataNode(int ID,const char *Weapon);
	char *Decrypt(const char *String,int EncryptionFactor,int Start);
};

class JMG_RP2_Screen_Fade_Red_On_Damage : public ScriptImpClass {
	float ScreenOpacicity;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void ScreenFade(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Phone_Load_Redial_Numbers : public ScriptImpClass {
	void Created(GameObject *obj);
	PlayerRedialNumberControl *JMG_RP2_Phone_Load_Redial_Numbers::DataNode(char PlayerName[]);
};

class JMG_RP2_Phone_Attach_Script_To_Sender_Player : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Display_Message_On_Pickup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Simple_Smart_Base_Defense : public ScriptImpClass {
	int AGTargetID;
	int EnemyID;
	int TargetVisible;
	int Checking;
	float TargetDistance;
	int KillTargetID;
	int SameTarget;
	int MyGrid;
	int AGAttackTime;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Timer_Expired(GameObject *obj,int number);
	void StopShootingAndReload(GameObject *obj);
	void AttackHoldFunction(GameObject *obj,GameObject *Enemy);
	void VisibilityCheck(GameObject *obj);
	void GiveToGrid(GameObject *Enemy);
	int GrabIDFromGrid();
	void RemoveGridTarget(int ID);
	void SelectNetworkTarget(GameObject *obj);
};

class JMG_RP2_Simple_Smart_House_Defense : public ScriptImpClass {
	int AGTargetID;
	int EnemyID;
	int TargetVisible;
	int Checking;
	float TargetDistance;
	int KillTargetID;
	int SameTarget;
	int MyGrid;
	int AGAttackTime;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Timer_Expired(GameObject *obj,int number);
	void StopShootingAndReload(GameObject *obj);
	void AttackHoldFunction(GameObject *obj,GameObject *Enemy);
	void VisibilityCheck(GameObject *obj);
	void GiveToGrid(GameObject *Enemy);
	int GrabIDFromGrid();
	void RemoveGridTarget(int ID);
	void SelectNetworkTarget(GameObject *obj);
};

class JMG_RP2_Phone_House_Key_Grant_Access_System : public ScriptImpClass {
	char PhoneNumberMessage[256];
	int HouseAddress;
	RP2NewHouseSystemControl *CurHouse;
	int TargetID;
	PhoneNumberControl *CallerNode;
	int IsOwned;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void AutoCancelCall(GameObject *obj,GameObject *player);
	void ChargeForCall(GameObject *obj);
};

class JMG_RP2_Jerads_Strike_Craft : public ScriptImpClass {
	int EnemyID;
	int TargetVisible;
	int Checking;
	float TargetDistance;
	int KillTargetID;
	int SameTarget;
	int ADS;
	bool Primary;
	int DriverID;
	int IsMoving;
	Vector3 MyLastPos;
	int TryReverse;
	bool Backward;
	bool LongRange;
	int JumpCharge;
	int Hover_Plate_ID;
	int TempID;
	int TempID2;
	int JetTime;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj, GameObject *damager);
	void StopShootingAndReload(GameObject *obj);
	void AttackHoldFunction(GameObject *obj,GameObject *Enemy);
	void VisibilityCheck(GameObject *obj);
	void ScreenFade(Vector3 HCPos);
	int OnIsland(GameObject *obj);
	GameObject *Get_A_Star2(GameObject *obj,Vector3 pos);
	RP2VehicleControl *Get_Vehicle_Node2(GameObject *obj);
};

class JMG_RP2_Map_Boundry : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_RP2_SCUD_Launcher_Script : public ScriptImpClass {
	int Deploy;
	int State;
	int EngineEnabled;
	int PlayerID;
	int IsArmed;
	char Animation[40];
	Vector3 TargetSpot;
	int DriverID;
	int CanToggle;
	SCUD_Missile_Launcher_Control *SCUDLauncherNode;
	RP2VehicleControl *MyNode;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj, GameObject *damager);
	void CreateTargetObject(GameObject *obj);
	void RemoveTargetObject(GameObject *obj);
	void Add_Node(GameObject *obj);
	void Remove_Node(GameObject *obj);
};

class JMG_RP2_New_Soldier_Spawn_System : public ScriptImpClass {
	Vector3 MyPos;
	int CanSpawnWhenEver;
	int CanBeSeen;
	int EnteredWater;
	int HasLeftClicked;
	Vector3 LastPos[5];
	int HouseID;
	Vector3 SpawnLocation;
	int RightClick;
	int PlayerID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	int IsNotTooCloseToStar(GameObject *obj);
	void SeeIfVisible(GameObject *obj);
	void SpawnPlayer(GameObject *obj);
	int Can_Spawn_Messages(GameObject *obj);
	void Relocate(GameObject *obj);
	GameObject *Get_A_Star2(GameObject *obj,Vector3 pos);
};

class JMG_RP2_New_Soldier_Spawn_Allowed_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_RP2_Simple_Smart_AA_Base_Defense : public ScriptImpClass {
	int AGTargetID;
	int EnemyID;
	int TargetVisible;
	int Checking;
	float TargetDistance;
	int KillTargetID;
	int SameTarget;
	int MyGrid;
	float MinFlightHeight;
	int AGAttackTime;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Timer_Expired(GameObject *obj,int number);
	void StopShootingAndReload(GameObject *obj);
	void AttackHoldFunction(GameObject *obj,GameObject *Enemy);
	void VisibilityCheck(GameObject *obj);
	void GiveToGrid(GameObject *Enemy);
	int GrabIDFromGrid();
	void RemoveGridTarget(int ID);
	void SelectNetworkTarget(GameObject *obj);
	bool Get_Vehicle_Is_Visible(GameObject *obj);
};

class JMG_RP2_Simple_Smart_AG_Base_Defense : public ScriptImpClass {
	int AGTargetID;
	int EnemyID;
	int TargetVisible;
	int Checking;
	float TargetDistance;
	int KillTargetID;
	int SameTarget;
	int MyGrid;
	float MinZ;
	float MaxZ;
	int AGAttackTime;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Timer_Expired(GameObject *obj,int number);
	void StopShootingAndReload(GameObject *obj);
	void AttackHoldFunction(GameObject *obj,GameObject *Enemy);
	void VisibilityCheck(GameObject *obj);
	void GiveToGrid(GameObject *Enemy);
	int GrabIDFromGrid();
	void RemoveGridTarget(int ID);
	void SelectNetworkTarget(GameObject *obj);
	bool Get_Vehicle_Is_Visible(GameObject *obj);
};

class JMG_Grant_Powerup_To_Beacon_Planter : public ScriptImpClass {
	void Created(GameObject *obj);
	int Find_Nearest_Beacon_ID(GameObject *obj);
};

class JMG_Send_Custom_To_Beacon_Planter : public ScriptImpClass {
	void Created(GameObject *obj);
	int Find_Nearest_Beacon_ID(GameObject *obj);
};

class JMG_RP2_Mission_System_New_Mission_Type : public ScriptImpClass {
	void Created(GameObject *obj);
	void Add_Phone_Node(int MissionType);
};

class JMG_RP2_Mission_System_Global_Mission_Controller : public ScriptImpClass {
	RP2MissionObjectiveTypeControl *MissionControlNode;
	RP2PlayerMissionsControl *MyMissionNode;
	MissionTypes *MyMissions;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	int Find_Mission_Type(int Message);
	int Find_Player_Mission_Node(GameObject *obj);
	int Find_Matching_Player_Mission_Type();
	int Check_Player_Mission_Objectives(int Objective);
	void MissionUpdate(GameObject *obj,GameObject *sender,int AchivementUpdate);
	void RewardWeapon(GameObject *obj);
	MissionTypes *Restart_Target_Mission(MissionTypes *TargetDelete);
	MissionTypes *Mission_Failed(GameObject *obj,MissionTypes *Mission);
	RP2PlayerVehicleFailMissionControl *Add_In_Special_Mission_Settings(RP2PlayerVehicleFailMissionControl *Node,int Enable);
	RP2PlayerVehicleFailMissionControl *Remove_Vehicle_Fail_From_Player_Listing(RP2PlayerVehicleFailMissionControl *Node,int Enable);
	int Check_For_Mission_Conflicts(int MissionType);
	void Copy_Conflicts_To_Player_Conflicts();
	void Remove_Conflicts_From_Player_List();
	int Check_If_Vehicle_Is_Excluded(GameObject *sender);
};

class JMG_RP2_Mission_System_Mission_Objective_Poke : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_RP2_Mission_System_Mission_Objective_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exit(GameObject *obj,GameObject *exiter);
};

class JMG_RP2_Mission_System_Mission_Objective_Kill : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_RP2_Mission_System_Mission_Objective_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Mission_System_Mission_Objective_Kill_OAG : public ScriptImpClass {
	int ObjectiveID;
	void Created(GameObject *obj);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_RP2_Mission_System_Load_Completed_Player_Missions : public ScriptImpClass {
	RP2PlayerMissionsControl *MyMissionNode;
	MissionTypes *MyMissions;
	RP2MissionObjectiveTypeControl *MissionControlNode;
	void Created(GameObject *obj);
	int Find_Mission_Type(int MissionType);
	void Add_Players(char PlayerName[]);
	int AddCompletedMissionToPlayer(int MissionType);
};

class JMG_RP2_Mission_System_Mission_Objective_Custom_Param : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Mission_System_Mission_Objective_Enter_Preset : public ScriptImpClass {
	int ObjectiveCount;
	void Entered(GameObject *obj,GameObject *enter);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void JMG_Force_Occupants_Exit(GameObject *obj);
};

class JMG_RP2_Mission_System_Mission_Objective_Enter_Weapon : public ScriptImpClass {
	int ObjectiveCount;
	void Entered(GameObject *obj,GameObject *enter);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Mission_System_Mission_Objective_Vehicle : public ScriptImpClass {
	int DriverID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_RP2_Mission_System_Phone_Grant_Mission : public ScriptImpClass {
	int PlayerMissions[128];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void AutoCancelCall(GameObject *obj,GameObject *player);
};

class JMG_RP2_Custom_Create_Object_Attach_Script : public ScriptImpClass {
	int ObjectID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};


class JMG_RP2_Phone_Main_Map_Runtime_Save : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void AutoCancelCall(GameObject *obj,GameObject *player);
	void SaveStructs();
	void MessageToPlayers(const char *Message);
	char *Encrypt(const char *String,int EncryptionFactor,int Start);
public:
	void SaveStructsSilent();
};

class JMG_RP2_MS_Mission_Objective_Enter_Attach_Script : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Load_Random_Teleport_File : public ScriptImpClass {
	void Created(GameObject *obj);
	void Add_Teleport_Node(float X,float Y,float Z);
};

class JMG_RP2_Mission_System_Mission_Objective_Damage : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

class JMG_RP2_Phone_Banking_System : public ScriptImpClass {
	char PhoneNumberMessage[256];
	float BankCredits[128];
	int DepositWithdraw[128];
	RP2PlayerBankAccountControl *PlayerAccount[128];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void AutoCancelCall(GameObject *obj,GameObject *player);
	void Account_Node(GameObject *obj);
	void ChargeForCall(GameObject *obj);
	float InterestRate(GameObject *obj);
};

class JMG_RP2_Load_Player_Bank_Accounts : public ScriptImpClass {
	void Created(GameObject *obj);
	void Account_Node(char PlayerName[],float Amount);
};

class JMG_RP2_Interest_Calculator : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Phone_Transfer_Funds : public ScriptImpClass {
	char PhoneNumberMessage[256];
	float Credits;
	int CheckingSavings;
	int TargetID;
	RP2PlayerBankAccountControl *PlayerAccount;
	PhoneNumberControl *CallerNode;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void AutoCancelCall(GameObject *obj,GameObject *player);
	void Account_Node(GameObject *obj);
	void ChargeForCall(GameObject *obj);
};

class JMG_RP2_SCUD_Launcher_Targeting_System : public ScriptImpClass {
	bool TransitionsEnabled;
	Vector3 SCUDLauncherLocation;
	int PlayerID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
	void ReturnToSL(GameObject *obj);
};

class JMG_RP2_SCUD_Launcher_SCUD_Missile_Up : public ScriptImpClass {
	Vector3 SCUDLauncherLocation;
	SCUD_Missile_Launcher_Control *ScudLauncherNode;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_RP2_SCUD_Launcher_SCUD_Missile_Down : public ScriptImpClass {
	Vector3 DownStartPos;
	int SendMessage;
	SCUD_Missile_Launcher_Control *ScudLauncherNode;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_RP2_SCUD_Launcher_Targeting_System_Camera_Facing : public ScriptImpClass {
	int DavesArrowID;
	int UpperBlockerID;
	int Collide;
	Vector3 LastPos;
	float DropSpeed;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_RP2_Phone_Message_Player_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Phone_Load_Phone_Number_Strings : public ScriptImpClass {
	void Created(GameObject *obj);
	RP2PhoneTextControl *DataNode(int ScriptID);
	char *RemoveReturn(const char *String);
	char *RemoveReturnAndE(const char *String,int EncryptionFactor,int Start);
};

class JMG_RP2_Mission_System_Strings_Loader : public ScriptImpClass {
	void Created(GameObject *obj);
	RP2MissionObjectiveTypeControl *DataNode(int MissionType);
	char *RemoveReturn(const char *String);
	char *RemoveReturnAndE(const char *String,int EncryptionFactor,int Start);
};

class JMG_RP2_Mission_System_Mission_Define_Mission_Conflicts : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void AddExcludedMission(int MissionType,int ExcludeMission);
};

class JMG_RP2_Mission_System_Mission_Vehicle_Exception : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void AddExcludedMission(int MissionType);
};

class JMG_RP2_Character_Info : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	float Weapon_Cost_Multiplier(int Cost);
	RP2VehicleControl *Get_Vehicle_Node(GameObject *obj);
	void House_Ownership_Details(GameObject *obj);
};

class JMG_RP2_Force_Vehicle_Exit_And_Kill : public ScriptImpClass {
	int Attempts;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Carryall : public ScriptImpClass {
	int HasVehicleID;
	char WheelName[32];
	float DropZ;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj, GameObject *damager);
	void Vehicle_Wheel_Bone(GameObject *CarryV);
	GameObject *Find_A_Vehicle(Vector3 Pos);
	RP2VehicleControl *Get_Vehicle_Node(GameObject *obj);
};

class JMG_RP2_Housing_Vehicle_Key_Press : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Housing_Vehicle_Zone : public ScriptImpClass {
	PlayerHouseVehicleControl *PHVCNode;
	RP2NewHouseSystemControl *HouseOwner;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
	void Control_Node(GameObject *obj,GameObject *TVehicle);
};

class JMG_RP2_Housing_Vehicle_PT : public ScriptImpClass {
	PlayerHouseVehicleControl *PHVCNode;
	RP2NewHouseSystemControl *HouseOwner;
	int CanState;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
	void Control_Node(GameObject *obj);
};

class JMG_RP2_Mission_System_Set_Vehicle_Mission_Fails : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Housing_Vehicle_Load : public ScriptImpClass {
	void Created(GameObject *obj);
	PlayerHouseVehicleControl *DataNode(int ID);
	char *Decrypt(const char *String,int EncryptionFactor,int Start);
};

class JMG_RP2_House_Spawn_Location : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Mission_System_Mission_Objective_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_WLS_Random_Weapon_Spawner_Control : public ScriptImpClass {
	int SpawnCount;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void SpawnWeapon(GameObject *obj);
};

class JMG_RP2_Send_Custom_To_ID_On_Pickup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Mission_Drop_Weapon_If_Mission_Is_Not_Started : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Remove_Weapon_Delay : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Move_To_Random_Location_On_Create : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_WLS_New_Decoy_Recreate_Powerup_Special : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_CarryAll_Vehicle_Height_Adjust : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Enable_Collisions_On_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_New_Parachute_Script : public ScriptImpClass {
	int ParachuteID;
	int create;
	int fallenough;
	Vector3 lastpos;
	int lastzmove;
	int go;
	int messagedisplayed;
	int UpperBlockerID;
	int Collide;
	float DropSpeed;
	Vector3 LastDrop;
	int CollisionDetectorID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj, int number);
	void Killed(GameObject *obj, GameObject *damager);
};

class JMG_Force_Enter_Vehicle : public ScriptImpClass {
	int TimeOut;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Move_While_In_Zone : public ScriptImpClass {
	char ScriptName[256];
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
};

class JMG_RP2_Move_While_In_Zone_Attach : public ScriptImpClass {
	int MovementAllowed;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Move_While_In_Zone_2 : public ScriptImpClass {
	char ScriptName[256];
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
};

class JMG_RP2_Move_While_In_Zone_2_Attach : public ScriptImpClass {
	int MovementAllowed;
	Vector3 ZoneCenter;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	float Calculations(float ZoneCenter,float Position,float Speed);
};

class JMG_RP2_Simple_Smart_ID_Base_Defense : public ScriptImpClass {
	int AGTargetID;
	int EnemyID;
	int TargetVisible;
	int Checking;
	float TargetDistance;
	int KillTargetID;
	int SameTarget;
	int MyGrid;
	int AGAttackTime;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void StopShootingAndReload(GameObject *obj);
	void AttackHoldFunction(GameObject *obj,GameObject *Enemy);
	void VisibilityCheck(GameObject *obj);
	void GiveToGrid(GameObject *Enemy);
	int GrabIDFromGrid();
	void RemoveGridTarget(int ID);
	void SelectNetworkTarget(GameObject *obj);
};

class JMG_RP2_Move_While_In_Zone_Allow_Movement : public ScriptImpClass {
	int x;
	int MovementEnabled[128];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Count_Objects_On_Map_And_Print : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_WLS_Weapon_Sell_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
};

class JMG_RP2_WLS_Weapon_Sell_On_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_RP2_Disable_Engine_On_Create : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Player_Anti_Missile_System_Limit : public ScriptImpClass {
	int PlayerID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Add_Node(GameObject *obj);
};

class JMG_RP2_Phone_Player_Anti_Missile_System_Set : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Change_HP_Shields_On_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_RP2_Phone_Sell_House_Phone_Number_Control_System : public ScriptImpClass {
	char PhoneNumberMessage[256];
	int HouseAddress;
	RP2NewHouseSystemControl *CurHouse;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void AutoCancelCall(GameObject *obj,GameObject *player);
	void ChargeForCall(GameObject *obj);
};

class JMG_RP2_Flame_Thrower_Script : public ScriptImpClass {
	int Heat;
	int CoolDownDelay;
	int NeedsToCoolDown;
	int LastCount;
	int CoolDownRate;
	int EXPGrowth;
	int HeatEXPGrowth;
	int HeatUpRate;
	int GiveTempWarnings[5];
	int FireID;
	int FlameThrowerID;
	float TankHealth;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void HeatWarningMessages(int IgnoreNumber);
	void Reset_Script();
};

struct FishTypeControl
{
	int FishID;
	int Size;
	int Value;
	float Probability;
	int OriginalProbability;
	float Damage;
	char Model[256];
	char Name[256];
	struct FishTypeControl *next;
};

FishTypeControl *FishTypeNode = NULL;

struct FishSchoolControl
{
	int SchoolID;
	int ZoneHealth;
	struct FishTypeControl *FishType;
	struct FishSchoolControl *next;
};

FishSchoolControl *FishSchoolNode = NULL;

class JMG_RP2_Fishing_Define_Fish : public ScriptImpClass {
	FishTypeControl *FishType;
	void Created(GameObject *obj);
	void New_Node();
};

class JMG_RP2_Fishing_School : public ScriptImpClass {
	FishSchoolControl *FishSchool;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Send_Custom_To_Boats(GameObject *obj,Vector3 Pos,int message);
	FishTypeControl *Find_Node(int Probability);
	void New_Node(GameObject *obj);
};

class JMG_RP2_Fishing_Boat : public ScriptImpClass {
	int FishPayLoad;
	float FishMarketValue;
	int FullLoadMessage;
	FishSchoolControl *CurrentSchool;
	int MessageDelay;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	FishSchoolControl *Find_School(int SchoolID);
};

#define NUMBEROFSCHOOLS 100

class JMG_RP2_Fishing_Random_School_Generator : public ScriptImpClass {
	int SchoolIDs[NUMBEROFSCHOOLS];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	Vector3 Find_Zone_For_Fish();
	Vector3 CreateSpot(GameObject *TempZone);
	int Check_For_Zones();
};

class JMG_RP2_Fishing_No_Fish_Zone : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Fishing_Unload_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_RP2_Fishing_Fish_Zone_50Percent : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Fishing_Fish_Zone_25Percent : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Phone_Player_Call_Tactical_Nuke : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Tactical_Nuke_Phone_Triggered : public ScriptImpClass {
	int Animating;
	int RingSoundID;
	int OwnerID;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj, GameObject *damager);
	int Find_Nearest_Beacon_ID(GameObject *obj);
	int Repeating_Sound_Object(GameObject *obj,const char *Model);
	void Remove_Repeating_Sound_Object(int *ID);
	void Trigger_Vehicle_EMP(GameObject *obj);
};

class JMG_RP2_Tactical_Nuke_Phone_Beacon : public ScriptImpClass {
	void Created(GameObject *obj);
	int Find_Nearest_Beacon_ID(GameObject *obj);
};

class JMG_RP2_Phone_Player_Special_Phone_Number : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Search_List(GameObject *obj);
public:
	void Delete_Node(int DeleteID);
	PlayerSpecificPNControl *Add_Node(int ID);
};

struct PlayerFavoriteVehicleControl
{
	char PlayerName[256];
	char VehicleName[256];
	char VehiclePreset[256];
	int VehicleCost;
	struct PlayerFavoriteVehicleControl *next;
};

PlayerFavoriteVehicleControl *PlayerFavoriteVehicleNode = NULL;

class JMG_RP2_Favorite_Vehicle_Main_Control_System : public ScriptImpClass {
	void Created(GameObject *obj);
	char *Decrypt(const char *String,int EncryptionFactor,int Start);
public:
	PlayerFavoriteVehicleControl *Add_Node(const char *PlayerName);
	PlayerFavoriteVehicleControl *Find_Node(const char *PlayerName);
};

class JMG_RP2_Soldier_Set_Favorite_Vehicle : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Phone_Buy_Favorite_Vehicle : public ScriptImpClass {
	char PhoneNumber[237];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void AutoCancelCall(GameObject *obj,GameObject *player);
	void ChargeForCall(GameObject *obj);
	void CreateFakeCinematic(GameObject *obj);
	GameObject *CreateAObject(const char *Preset,const char *Model,const char *Animation,Vector3 Pos,float Facing,GameObject *GAttachTo,const char *Bone,float DeathFrame,float LastFrame);
};

class JMG_RP2_Disallow_Favorite_Vehicle : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Tactical_Nuke_Radiation : public ScriptImpClass {
	int RadiationTime;
	int RadLevel;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	int Repeating_Sound_Object(GameObject *obj,const char *Model);
	void Remove_Repeating_Sound_Object(int *ID);
};

class JMG_RP2_Set_Object_Team_On_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Train_Kill_Zone_Control : public ScriptImpClass {
	bool AtTrainStation;
	int VisitedStations[3];
	float MaxPayout;
	RP2VehicleControl *MyNode;
	int CollisionDetectorID;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj, GameObject *damager);
	RP2VehicleControl *Find_Node(GameObject *obj);
};

class JMG_RP2_Set_Physical_Collisions_On_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Set_Engine_On_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

struct PlayerFavoriteWeaponControl
{
	char PlayerName[256];
	char WeaponName[256];
	char WeaponPreset[256];
	int WeaponCost;
	struct PlayerFavoriteWeaponControl *next;
};

PlayerFavoriteWeaponControl *PlayerFavoriteWeaponNode = NULL;

class JMG_RP2_Favorite_Weapon_Main_Control_System : public ScriptImpClass {
	void Created(GameObject *obj);
	char *Decrypt(const char *String,int EncryptionFactor,int Start);
public:
	PlayerFavoriteWeaponControl *Add_Node(const char *PlayerName);
	PlayerFavoriteWeaponControl *Find_Node(const char *PlayerName);
};

class JMG_RP2_Soldier_Set_Favorite_Weapon : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	char *Get_Weapon_Name(RP2WeaponListControl *WeaponNode);
};

class JMG_RP2_Phone_Buy_Favorite_Weapon : public ScriptImpClass {
	char PhoneNumber[237];
	Vector3 CreateLocation;
	float Facing;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void AutoCancelCall(GameObject *obj,GameObject *player);
	void ChargeForCall(GameObject *obj);
	void CreateFakeCinematic(GameObject *obj,GameObject *Player);
	GameObject *CreateAObject(GameObject *obj,const char *Preset,const char *Model,const char *Animation,Vector3 Pos,float Facing,GameObject *GAttachTo,const char *Bone,float DeathFrame,float LastFrame,float CreateDelay,int DelayTimer);
	void SetAObject(GameObject *Object,const char *Model,const char *Animation,GameObject *GAttachTo,const char *Bone,float DeathFrame,float LastFrame);
	GameObject *CreateRealWeapon(GameObject *obj,RP2WeaponListControl *TheWeapon);
};

class JMG_RP2_Phone_Request_Display_Server_Version : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Phone_Request_Object_Count : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void DisplayText(const char *Text,int RecordedMax,int Current);
};

class JMG_RP2_Renhalo_Blast_Doors : public ScriptImpClass {
	int CAMode;
	int SwitchIDs[4];
	bool Enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Animation_Complete(GameObject *obj,const char *anim);
	void Add_Switch_ID(int SenderID);
	void Send_Custom_To_Switches(GameObject *obj,int Custom);
};

class JMG_RP2_Renhalo_Blast_Door_Switch : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_RP2_Renhalo_Blast_Door_Wall_Switch : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Send_Custom_On_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_RP2_Renhalo_Blast_Door_Keycard : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Renhalo_Blast_Door_Wall_Switch_Key : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Send_Custom_On_Poke_Key : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

struct WarthogControl
{
	int BodyID;
	int TurretID;
	int GunnerID;
	struct WarthogControl *next;
};

WarthogControl *WarthogNode = NULL;

class JMG_RP2_Renhalo_Warthog : public ScriptImpClass {
	WarthogControl *MyNode;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj, GameObject *damager);
	void Set_HP(GameObject *obj,float Health,float Armor);
	WarthogControl *Add_Node(int ID);
};

class JMG_RP2_Renhalo_Vehicle_Passenger : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Select_Empty_Hands(GameObject *obj);
	void Remove_Empty_Hands(GameObject *obj);
};

class JMG_RP2_Create_Vehicle_At_Safe_Location : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Place_Vehicle_At_Safe_Location_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Get_Unstuck_Key : public ScriptImpClass {
	int Stuck;
	int Moving;
	int HolderID;
	Vector3 LastPosition;
	Vector3 OldestPosition;
	Vector3 LastUpdatePos;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Grill : public ScriptImpClass {
	int FireOn;
	int FlameID;
	int FoodTime;
	RP2WeaponListControl *FoodNode;
	int GrillTime;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
	void Grant_Food(GameObject *obj);
};

class JMG_RP2_Renhalo_Phone_Exit_Fix : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_RP2_Dropped_Credit_Card_Info_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Dropped_Credit_Card_Info_Powerup_Control : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_Renhalo_Mindless_Hunt_And_Attack : public ScriptImpClass {
	int Attacking;
	int CurrentTarget;
	float speed;
	Vector3 HomeLocation;
	int StuckCount;
	int GiveUp;
	Vector3 LastPos;
	int IAmGroupingUp;
	int Hunting;
	Vector3 RandomMoveSpot;
	int AttackMoveT;
	int JumpyAttack;
	Vector3 LastEnemyPos;
	char PrimaryWeapon[256];
	int EnterVehicleTargetID;
	int BackwardCount;
	int Backward;
	int GotoSpot;
	int MovingToVehicle;
	Vector3 LastTargetLocation;
	int VehicleStuckCount;
	int NewVehicleAction;
	int CanHearDelay;
	int AutoEnter;
	int EnterVehicleMoveUpdate;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Destroyed(GameObject *obj);
	void Killed(GameObject *obj, GameObject *damager);
	void Sound_Heard(GameObject *obj,const CombatSound &sound);
	void Attack_Target(GameObject *obj,int TargetID,bool Pathfind,float AttackRange);
	void Goto_Location(GameObject *obj,Vector3 TargetLocation,bool Pathfind,float ArriveDistance);
	void Hunt_Star(GameObject *obj);
	void Swarm_Group_Up(GameObject *obj);
	void Goto_Random_Spot(GameObject *obj,float MaxDistance);
	void Restart_Timer(GameObject *obj,Vector3 Pos,float Time);
	void AttackMove(GameObject *obj,float AttackRange,Vector3 Pos);
	void Action_Reset(GameObject *obj);
	void FaceTarget(GameObject *obj,Vector3 pos);
	void UpdateAction(GameObject *obj);
	void ChooseARandomAction(GameObject *obj,float *Time);
	void UnGroupedStuckActions(GameObject *obj,float *Time);
	void GroupedStuckActions(GameObject *obj,float *Time);
	void VehicleRandomAction(GameObject *obj,float *Time,GameObject *MyVehicle);
	void Attack_Logic(GameObject *obj,float *Time,Vector3 Pos,GameObject *MyVehicle);
	void Vehicle_Enter_Logic(GameObject *obj,Vector3 Pos,float *Time,GameObject *MyVehicle);
};

class JMG_RP2_Phone_Create_AI_Swarm : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Create_The_Swarm(GameObject *sender,Vector3 pos);
	void Create_AI(GameObject *obj,GameObject *sender,Vector3 Location);
	GameObject *Soldier_Twiddler(Vector3 Pos);
};

class Spawn_Zombie_When_Killed_By_Zombie : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *damager);
};

class JMG_Bring_Up_Text_Box_On_Poke : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Phone_Create_AI_Swarm_One : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Create_AI(GameObject *obj,GameObject *sender,Vector3 Location);
	GameObject *Soldier_Twiddler(Vector3 Pos);
	Vector3 Spawn_Pos_Twiddler();
};

class JMG_RP2_Tutorial_Door_Teleport : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_RP2_Tutorial_Message_Zone : public ScriptImpClass {
	int EnterID;
	int ExitID;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Exit(GameObject *obj,GameObject *exiter);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Zone_Debug_Script : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Load_Player_Characters : public ScriptImpClass {
	void Created(GameObject *obj);
	void Player_Character_Node(const char *PlayerName,const char *PresetName);
};

struct HomeSecuritySystemControl
{
	int HouseAddress;
	int NumberOfPrePays;
	int RentYear;
	int RentDay;
	int RentPrice;
	bool DetectIntruders;
	bool DetectVandals;
	bool CeilingGuns;
	bool MissileDefenseSystem;
	bool AutoRepair;
	int Intruding[128];
	bool SoundAlarm;
	int HouseSecurityAlarmIDs[5];
	struct HomeSecuritySystemControl *next;
};

HomeSecuritySystemControl *HomeSecuritySystemNode = NULL;

enum {HSODoor = 1,HSOCeilingGun = 2,HSOAntiMissileSystem = 3};
struct HomeSecurityObjectControl
{
	int HouseAddress;
	int ObjectID;
	char PresetName[128];
	Vector3 Position;
	float Rotation;
	float HealRate;
	float MaxHP;
	int RespawnTime;
	int DefenseType;
	int AllowRepair;
	struct HomeSecurityObjectControl *next;
};

HomeSecurityObjectControl *HomeSecurityObjectNode = NULL;

class JMG_RP2_Phone_House_Security_System_Config_Number : public ScriptImpClass {
	char PhoneNumberMessage[256];
	int HouseAddress;
	HomeSecuritySystemControl *HouseSecurity;
	float Cost;
	int SelectedOption;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void AutoCancelCall(GameObject *obj,GameObject *player);
	void ChargeForCall(GameObject *obj);
	char *ReturnEnabled(bool value);
	float SecurityPrice(float Price);
	void TogglePlanSettings(GameObject *obj,GameObject *sender,bool Owned,float Price,int Choice);
	HomeSecuritySystemControl *Find_Security_Node(int Address);
	void EnableObjectSpawn(int SecurityObjectType);
	void DisableObjectSpawn(int SecurityObjectType);
};

class JMG_RP2_House_Security_Object : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	HomeSecurityObjectControl *Create_Security_Object_Node(int Address);
	void Update_Security_Object(GameObject *obj,HomeSecuritySystemControl *ANode1);
};

class JMG_RP2_Load_House_Security_Setup : public ScriptImpClass {
	void Created(GameObject *obj);
	HomeSecuritySystemControl *Find_Security_Node(int Address);
	char *Decrypt(const char *String,int EncryptionFactor,int Start);
	int HouseOwnedCheck(int Address);
};

class JMG_RP2_House_Security_System_Object_Control : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void NodeCheckup(HomeSecurityObjectControl *TheNode);
	void RecreateObject(HomeSecurityObjectControl *TheNode);
	void CheckHomeAlarms(HomeSecuritySystemControl *CurrentSN);
	void CheckAndRepairSecuritySystems(HomeSecuritySystemControl *CurrentSN);
};

class JMG_RP2_House_Security_Object_Repair_Control : public ScriptImpClass {
	int RepairDelay;
	HomeSecurityObjectControl *MyNode;
	HomeSecuritySystemControl *MyHouseSecurityNode;
	RP2NewHouseSystemControl *MyHouseNode;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	HomeSecurityObjectControl *Find_Object_Security_Node(int ID);
};

struct HomeSecurityZoneControl
{
	int HouseAddress;
	int ZoneID;
	struct HomeSecurityZoneControl *next;
};

HomeSecurityZoneControl *HomeSecurityZoneNode = NULL;

class JMG_RP2_House_Zone : public ScriptImpClass {
	HomeSecuritySystemControl *MyHouseSecurityNode;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
	void CreateSecurityZoneNode(GameObject *obj);
};

class JMG_RP2_House_Security_Alarm_Object : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_House_Repair_PT : public ScriptImpClass {
	int DoublePressTime;
	HomeSecuritySystemControl *MyHouseSecurityNode;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
	void PriceCalculation(float *Damage,float *Price,int *DamagedObjects,int *DestroyedObjects);
	void RepairAllSecurityObjects();
	int CheckIfCanRepair(HomeSecurityObjectControl *Current);
};

class JMG_RP2_House_GDI_PT_Zone : public ScriptImpClass {
	int ZoneOccupants;
	int PlayerID[128];
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exit);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_House_Missile_Defense_System : public ScriptImpClass {
	int AGTargetID;
	int EnemyID;
	int TargetVisible;
	int Checking;
	float TargetDistance;
	int KillTargetID;
	int SameTarget;
	int MyGrid;
	int AGAttackTime;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void StopShootingAndReload(GameObject *obj);
	void AttackHoldFunction(GameObject *obj,GameObject *Enemy);
	void VisibilityCheck(GameObject *obj);
	void GiveToGrid(GameObject *Enemy);
	int GrabIDFromGrid();
	void RemoveGridTarget(int ID);
	void SelectNetworkTarget(GameObject *obj);
};

class JMG_RP2_Object_Send_Text_Message_On_Death : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *killer);
};

class JMG_RP2_Teleport_On_Frame_Range : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_RP2_Anti_Missile_System_Dish : public ScriptImpClass {
	AntiMissileSystemControl *MyDishNode;
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
	void Killed(GameObject *obj, GameObject *killer);
	AntiMissileSystemControl *Create_Dish_Node(int ID);
};

class JMG_Tornado_Throw : public ScriptImpClass {
	float LastSafeFrame;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject *obj,const char *anim);
};

class JMG_RP2_Firetruck : public ScriptImpClass {
	OBBoxClass *MyZoneBox;
	Vector3 BucketPos;
	bool Deployed;
	bool EngineEnabled;
	int ReEnterZoneID;
	int LastClickerID;
	int SirenID;
	int siren;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
	void JumpToLadder(GameObject *obj,GameObject *sender);
	Vector3 Find_Zone_Area(GameObject *TempZone);
};

class JMG_RP2_Vehicle_Speedometer : public ScriptImpClass {
	RP2VehicleControl *MyNode;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	RP2VehicleControl *FindNode(GameObject *obj);
};

class JMG_RP2_Hot_Air_Balloon : public ScriptImpClass {
	int MoverID;
	int MoverID2;
	int MovementDirection;
	int AirTemp;
	int FireOn;
	bool EngineEnabled;
	int UpNeitherDown;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *damager);
};

class JMG_RP2_Fire_Truck_ReEnter_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exiter);
};

struct Mech_Voice_Files
{
	char TextMessage[128];
	char EVAMessage[128];
	float VoiceTime;
	int SetState;
	struct Mech_Voice_Files *next;
};

class JMG_RP2_Mech_MadCat : public ScriptImpClass {
	enum MechStates{ShutDown,PoweringUp,OnLine,OverHeatShuttingDown,OverHeatShuttingDown2,ShuttingDown,OverHeatShutDown,SelfDestruct};
	enum MechMovement{MForward,MBackward,MIdle,MJump,MCrouch};
	MechStates MechState;
	MechMovement WalkState;
	Mech_Voice_Files *VoiceFiles;
	bool PlayingVoice;
	int ReactorTemp;
	bool HeatOverride;
	bool GaveHeatWarning;
	bool AttempttedAutoShutDownSequence;
	bool HeatOverrideEngaged;
	int CoolDownRate;
	int CoolDownSpeedUp;
	Vector3 LastPos;
	float OldFacing;
	MechStates SetState;
	bool AutoStarting;
	bool ChangingState;
	char LastMoveAnimation[127];
	int Explosives;
	bool IAEDE;
	bool JumpJetsOn;
	int JetFuel;
	int ReFuelStartDelay;
	int JetCoolDownTime;
	int Hover_Plate_ID;
	int TempID;
	int TempID2;
	int Is_In_Water;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Animation_Complete(GameObject *obj,const char *anim);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *damager);
	void Eva_Talk(GameObject *obj,const char *VoiceFile,float PlayTime,MechStates GoToState,const char *TextMessage);
	bool Is_OnLine();
	void Mech_ShutDown(GameObject *obj);
	void Mech_StartUp(GameObject *obj);
	bool Eject_Pilot(GameObject *obj);
	void Play_UI_SFX(GameObject *obj,const char *SoundFile);
	void Mech_Animation(GameObject *obj);
};

class JMG_Play_Animation_On_Create_The_Destroy : public ScriptImpClass {
	void Created(GameObject *obj);
	void Animation_Complete(GameObject *obj,const char *anim);
};

class JMG_RP2_Ambulance_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JMG_RP2_Ambulance : public ScriptImpClass {
	OBBoxClass *MyZoneBox1;
	OBBoxClass *MyZoneBox2;
	int ReEnterZoneID1;
	int ReEnterZoneID2;
	Vector3 LastPos;
	int SirenID;
	int siren;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *damager);
	OBBoxClass *SetupAZoneBox(GameObject *obj,OBBoxClass *TheBox,int ZoneRipID,int *ReEnterZoneID);
};

class JMG_Buy_Preset_On_Poke_Advanced : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
	void RTeleportCall(GameObject *obj,GameObject *TargetSpot);
};

class JMG_RP2_PT_Poke_Grant_Powerup_Death : public ScriptImpClass {
	void Poked(GameObject *obj,GameObject *poker);
};

class JMG_RP2_Vehicle_Fire_Override : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Invulnerable_Until_Custom_Received : public ScriptImpClass {
	char HealthType[128];
	char ArmorType[128];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_Send_Self_Custom_On_Create : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Enable_Collisions_When_Safe : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Plasma_Torpedo_Launcher : public ScriptImpClass {
	int ClickedTimes;
	float ResetTime;
	int LastAmmoCount;
	int HasLeftClickedBefore;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Console_Command_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Phone_Grant_Random_Weapon : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

class JMG_RP2_Vehicle_Ignore_EMP : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_RP2_Jet : public ScriptImpClass {
	bool InZone;
	int MoverID;
	RP2VehicleControl *MyNode;
	int Warning;
	bool InFlight;
	bool EngineEnabled;
	bool AlarmPlaying;
	bool EngineFailed;
	bool AllowCrash;
	bool AllowCrash2;
	bool BlockOn;
	bool Collides;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void CalculateAnimation(GameObject *obj);
};

class JMG_RP2_Jet_Reload_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JMG_Create_Powerup_At_Vehicle_Owner_Position : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
};

class JMG_RP2_Server_Remove_PTs_FPS_Improvement_Test : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

struct BaseDefenseNode
{
	long int ID;
	int Time;
};

class JMG_RP2_Simple_Smart_Base_Defense_Alarm : public ScriptImpClass {
	bool AlarmPlaying;
	BaseDefenseNode IDs[2];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	long int GrabIDFromGrid(int Grid);
	void RemoveIDFromGrid(long int ID);
	bool UpdateRemoveIDTime(int WGID,long int ID);
};

class JMG_RP2_Ammo_Crate_Control : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void TransferAmmoBetweenWeapons(GameObject *obj,const char *Weapon);
};

class JMG_RP2_Train_Station : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JMG_Buy_Preset_On_Poke_Hot_Air_Balloon : public ScriptImpClass {
	bool enabled;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};

class JMG_RP2_Change_Models_On_Custom : public ScriptImpClass {
	int ModelState;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Change_Model(GameObject *obj);
};

class JMG_RP2_Pull_Out_Phone_On_Keypress : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

struct AIPlayerStandingNode
{
	char PlayerName[128];
	int Standing;
};

enum AIStates{Wait,Wander,EnterVehicle,AttackVehicle,RequestEntry,OfferARide,FollowingFriend,AvengeFriend,WaitForFriend,HuntAStar,AttackTarget,InvestigateSound,MoveToLastSeen};

class JMG_RP2_Smart_AI : public ScriptImpClass {
	AIStates MyState;
	AIPlayerStandingNode *AIPlayerStanding[128];
	int GlobalAnger;
	int EnemyTargetID;
	bool Backward;
	int StuckCount;
	float Speed;
	int GiveUpTime;
	Vector3 LastTargetLocation;
	Vector3 RandomMoveSpot;
	Vector3 TargetLocation;
	Vector3 LastEnemyPos;
	Vector3 LastPos;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj, GameObject *damager);
	void Sound_Heard(GameObject *obj,const CombatSound &sound);
	void Attack_Target(GameObject *obj,int TargetID,bool Pathfind,float AttackRange);
	void Goto_Location(GameObject *obj,Vector3 TargetLocation,bool Pathfind,float ArriveDistance);
	void Hunt_Star(GameObject *obj);
	void Goto_Random_Spot(GameObject *obj,float MaxDistance);
	void Restart_Timer(GameObject *obj,Vector3 Pos,float Time);
	void AttackMove(GameObject *obj,float AttackRange,Vector3 Pos);
	void Action_Reset(GameObject *obj);
	void FaceTarget(GameObject *obj,Vector3 pos);
	void UpdateAction(GameObject *obj);
	void GroupedStuckActions(GameObject *obj,float *Time);
	void VehicleRandomAction(GameObject *obj,float *Time,GameObject *MyVehicle);
	void Attack_Logic(GameObject *obj,float *Time,Vector3 Pos,GameObject *MyVehicle);
	void Vehicle_Enter_Logic(GameObject *obj,Vector3 Pos,float *Time,GameObject *MyVehicle);
	bool ComparePriority(AIStates NewState);
	int GetPriority(AIStates State);
	bool MovingStates();
};

class JMG_RP2_Hovercraft_Hover_Control : public ScriptImpClass {
	bool HoverOn;
	unsigned int Hover_Plate_ID;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	void Killed(GameObject *obj, GameObject *damager);
};

class JMG_RP2_Initilize_Area_Weather : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Setup_Area_Node();
};

class JMG_RP2_Change_Weather_Area_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

class JMG_RP2_Change_Weather_When_In_Range : public ScriptImpClass {
	float MinDistance;
	bool PlayersInRange[128];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Change_Player_Area(GameObject *PlayerObj,int PlayerID,int Area);
};

class JMG_RP2_Checkstand_Cashier : public ScriptImpClass {
	int CheckerID;
	float CheckerGain;
	float MaxPayout;
	int PayoutTime;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exiter);
};

class JMG_RP2_Checkstand_Customer : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};


class JMG_RP2_UFO_Script : public ScriptImpClass {
	int JumpCharge;
	int JetTime;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void ScreenFade(Vector3 HCPos);
	GameObject *Get_A_Star2(GameObject *obj,Vector3 pos);
	RP2VehicleControl *Get_Vehicle_Node2(GameObject *obj);
};
