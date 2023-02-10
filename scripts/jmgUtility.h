
#pragma once
#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "WeaponClass.h"
#include "WeaponBagClass.h"
#include "SoldierGameObj.h"
#include "PhysicsSceneClass.h"
#include "MoveablePhysClass.h"

/*!
* \brief Displays a message if the script named isn't in the scripts build on the server.
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Check_If_Script_Is_In_Library : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sends a custom when customs are sent in a certain order (kind of like a combo lock)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Send_Custom_When_Custom_Sequence_Matched : public ScriptImpClass {
	int depth;
	int failCount;
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Send_Custom(GameObject *obj,int custom,int param);
};

/*!
* \brief Changes a objects w3d model on a timer
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Change_Model_On_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Turns an object into an object that emulates the damange system of DamageableStaticPhysics tiles
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Emulate_DamageableStaticPhys : public ScriptImpClass {
	int team;
	bool alive;
	bool playingTransition;
	bool playingTwitch;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Animation_Complete(GameObject *obj,const char *anim);
	void Play_Animation(GameObject *obj,bool loop,float start,float end);
};
/*!
* \brief Displays a hud message to all players on custom
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Display_HUD_Info_Text_To_All_Players_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
/*!
* \brief Displays a hud message to a player on custom
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Display_HUD_Info_Text_To_Sender_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
/*!
* \brief Soldier well enter the nearest vehicle on custom
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Soldier_Transition_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
/*!
* \brief On Poke sends custom to self
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Poke_Send_Self_Custom : public ScriptImpClass {
	bool poked;
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};
/*!
* \brief Basic turret attach script, turrets match team of vehicle attached to, turrets are destroyed by destroy event
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Turret_Spawn : public ScriptImpClass
{
  int turretId;
  bool hasDriver;
  void Created(GameObject *obj);
  void Custom(GameObject *obj,int type,int param,GameObject *sender);
  void Destroyed(GameObject *obj);
};

class JmgUtility
{
public:
	static void RotateZoneBox(float Angle,Matrix3 Basis)
	{
		Angle = Angle*3.14159265f/180.0f;
		float Sin = sin(Angle);
		float Cos = cos(Angle);
		Basis[0][0] = Cos;Basis[0][1] = Sin;Basis[0][2] = 0.0f;
		Basis[1][0] = Sin;Basis[1][1] = Cos;Basis[1][2] = 0.0f;
		Basis[2][0] = 0.0f;Basis[2][1] = 0.0f;Basis[2][2] = 1.0f;
	}
	static float rotationClamp(float startRotation,float addRotation)
	{
		startRotation += addRotation;
		while (startRotation > 180)
			startRotation -= 360;
		while (startRotation < -180)
			startRotation += 360;
		return startRotation;
	}
	static char *JMG_Get_Compass_Directions(float compassDegree)
	{
		static char name[4];
		sprintf(name,"%s",compassDegree <= -168.75 ? "W":
			compassDegree <= -146.25 ? "WSW":
			compassDegree <= -123.75 ? "SW":
			compassDegree <= -101.25 ? "SSW":
			compassDegree <= -78.75 ? "S":
			compassDegree <= -56.25 ? "SSE":
			compassDegree <= -33.75 ? "SE":
			compassDegree <= -11.25 ? "ESE":
			compassDegree <= 11.25 ? "E":
			compassDegree <= 33.75 ? "ENE":
			compassDegree <= 56.25 ? "NE":
			compassDegree <= 78.75 ? "NNE":
			compassDegree <= 101.25 ? "N":
			compassDegree <= 123.75 ? "NNW":
			compassDegree <= 146.25 ? "NW":
			compassDegree <= 168.75 ? "WNW":
			compassDegree <= 180 ? "W" : "ERROR");
		return name;
	}
	static char *formatDigitGrouping(double money)
	{
		static char groupedMoney[43];
		if (abs(money) > 100000000000000000000000000.0)
		{
			sprintf(groupedMoney,"%.2lf",money);
			return groupedMoney;
		}
		char sMoney[40],tMoney[53],fMoney[53];
		sprintf(sMoney,"%.0lf",abs(money));
		int length = strlen(sMoney),count = -1,pos;
		pos = length+1+(length-1)/3;
		tMoney[pos] = '\0';
		for (int x = length;x >= 0;x--)
		{
			pos--;
			tMoney[pos] = sMoney[x];
			count++;
			if (count == 3 && pos > 0)
			{
				count = 0;
				pos--;
				tMoney[pos] = ',';
			}
		}
		if (money < 0)
			sprintf(fMoney,"-%s",tMoney);
		else
			sprintf(fMoney,"%s",tMoney);

		sprintf(sMoney,"%.2lf",abs(money));
		int nLength = strlen(sMoney);
		pos = strlen(fMoney);
		for (int x = length;x < nLength;x++)
		{
			fMoney[pos] = sMoney[x];
			pos++;
		}
		fMoney[pos] = '\0';
		sprintf(groupedMoney,"%s",fMoney);
		return groupedMoney;
	}
	static char *formatDigitGrouping(unsigned long value)
	{
		static char groupedMoney[43];
		if (value > 10000000000000000000)
		{
			sprintf(groupedMoney,"%lu",value);
			return groupedMoney;
		}
		char sValue[40],tValue[53],fValue[53];
		sprintf(sValue,"%lu",value);
		int length = strlen(sValue),count = -1,pos;
		pos = length+1+(length-1)/3;
		tValue[pos] = '\0';
		for (int x = length;x >= 0;x--)
		{
			pos--;
			tValue[pos] = sValue[x];
			count++;
			if (count == 3 && pos > 0)
			{
				count = 0;
				pos--;
				tValue[pos] = ',';
			}
		}
		sprintf(fValue,"%s",tValue);
	
		sprintf(groupedMoney,"%s",fValue);
		return groupedMoney;
	}
	static char JMG_Get_Sex(GameObject *obj)
	{
		int TestID = Commands->Get_ID(obj);
		if (!obj || !TestID)
			return '\0';
		if (!Get_Model(obj) || !_stricmp(Get_Model(obj),"null"))
			return '\0';
		if (!Get_Sex(obj))
			return '\0';
		return Get_Sex(obj);
	}
	static const char *JMG_Get_Units_Sex(GameObject *Unit)
	{
		if (JMG_Get_Sex(Unit) == 'A')
			return "himself";
		else if (JMG_Get_Sex(Unit) == 'B')
			return "herself";
		else
			return "itself";
	}
	static const char *JMG_Get_Units_Sex2(GameObject *Unit)
	{
		if (JMG_Get_Sex(Unit) == 'A')
			return "his";
		else if (JMG_Get_Sex(Unit) == 'B')
			return "her";
		else
			return "its";
	}
	static const char *JMG_Get_Units_Sex3(GameObject *Unit)
	{
		if (JMG_Get_Sex(Unit) == 'A')
			return "he";
		else if (JMG_Get_Sex(Unit) == 'B')
			return "she";
		else
			return "it";
	}
	static char *Get_The_Units_Name(GameObject *Unit)
	{
		static char RetChar[512];
		if (Commands->Is_A_Star(Unit))
			sprintf(RetChar,"%s",Get_Player_Name(Unit));
		else
			sprintf(RetChar,"A %s",Get_Translated_Preset_Name(Unit));
		return RetChar;
	}
	static char *Get_The_Units_Name2(GameObject *Unit)
	{
		static char RetChar[512];
		if (Commands->Is_A_Star(Unit))
			sprintf(RetChar,"%s",Get_Player_Name(Unit));
		else
			sprintf(RetChar,"a %s",Get_Translated_Preset_Name(Unit));
		return RetChar;
	}
	static long JMG_Get_Player_ID(GameObject *obj)
	{
		if (!obj)
			return 0;
		SoldierGameObj *o = obj->As_SoldierGameObj();
		if (!o)
			return 0;
		if (!((cPlayer *)o->Get_Player_Data()))
			return 0;
		return ((cPlayer *)o->Get_Player_Data())->Get_Id();
	}
	static int JMG_Get_Current_Weapon_ID(GameObject *obj)
	{
		if (!obj)
			return -1;
		PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
		if (!o2)
			return -1;
		ArmedGameObj *o3 = o2->As_ArmedGameObj();
		if (!o3)
			return -1;
		WeaponBagClass *w = o3->Get_Weapon_Bag();
		if ((w->Get_Index()) && (w->Get_Index() < w->Get_Count()))
			return w->Peek_Weapon(w->Get_Index())->Get_ID();
		return -1;
	}
	static float SimpleDistance(const Vector3 &Pos1,const Vector3 &Pos2)
	{
		float DiffX = Pos1.X-Pos2.X;
		float DiffY = Pos1.Y-Pos2.Y;
		float DiffZ = Pos1.Z-Pos2.Z;
		return (DiffX*DiffX+DiffY*DiffY+DiffZ*DiffZ);
	}
	static float SimpleFlatDistance(const Vector3 &Pos1,const Vector3 &Pos2)
	{
		float DiffX = Pos1.X-Pos2.X;
		float DiffY = Pos1.Y-Pos2.Y;
		return (DiffX*DiffX+DiffY*DiffY);
	}
	static float MathClamp(float Value,float Min,float Max)
	{
		if (Value > Max)
			return Max;
		if (Value < Min)
			return Min;
		return Value;
	}
	static double MathClampDouble(double Value,double Min,double Max)
	{
		if (Value > Max)
			return Max;
		if (Value < Min)
			return Min;
		return Value;
	}
	static int MathClampInt(int Value,int Min,int Max)
	{
		if (Value > Max)
			return Max;
		if (Value < Min)
			return Min;
		return Value;
	}
	static void DisplayChatMessage(GameObject *obj,int Red,int Green,int Blue,const char *Message)
	{
		if (!obj)
			return;
		int Strlen = strlen(Message);
		if (Strlen < 220)
		{
			Send_Message_Player(obj,Red,Green,Blue,Message);
			return;
		}
		int x;
		char msg[230];
		for (x = 0;x < 220 && x < Strlen;x++)
			msg[x] = Message[x];
		msg[x] = '\0';
		Send_Message_Player(obj,255,0,200,msg);
	}
	static void MessageAllPlayers(int Red,int Green,int Blue,const char *MSG)
	{
		for (int x = 0;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player)
				continue;
			DisplayChatMessage(Player,Red,Green,Blue,MSG);
		}
	}
	static void MessageTeamPlayers(int Red,int Green,int Blue,int Team,const char *MSG)
	{
		for (int x = 0;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player || Get_Team(x) != Team)
				continue;
			DisplayChatMessage(Player,Red,Green,Blue,MSG);
		}
	}
	static void MessageTeamPlayersType(int Red,int Green,int Blue,int Team,const char *MSG)
	{
		for (int x = 0;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player || Commands->Get_Player_Type(Player) != Team)
				continue;
			DisplayChatMessage(Player,Red,Green,Blue,MSG);
		}
	}
	static char *Rp2Encrypt(const char *String,int EncryptionFactor,int Start)
	{
		static char FinalString[2000];
		sprintf(FinalString," ");
		int loops = strlen(String),x = 0;
		int CurrentAdd = Start;
		while (x < loops)
		{
			if (String[x] > 31 && String[x] < 127)
			{
				int Temp = (String[x]+CurrentAdd);
				while (Temp > 126)
					Temp -= 95;
				FinalString[x] = (char)Temp;
			}
			else
				FinalString[x] = String[x];
			x++;
			CurrentAdd++;
			if (CurrentAdd > EncryptionFactor)
				CurrentAdd = 1;
			FinalString[x] = '\0';
		}
		return FinalString;
	}
	static char *Rp2Encrypt2(const char *String,int EncryptionFactor,int Start)
	{
		static char FinalString[2000];
		sprintf(FinalString," ");
		int loops = strlen(String),x = 0;
		int CurrentAdd = Start;
		while (x < loops)
		{
			if (String[x] > 31 && String[x] < 127)
			{
				int Temp = (String[x]+CurrentAdd);
				while (Temp > 126)
					Temp -= 95;
				FinalString[x] = (char)Temp;
			}
			else
				FinalString[x] = String[x];
			x++;
			CurrentAdd++;
			if (CurrentAdd > EncryptionFactor)
				CurrentAdd = 1;
			FinalString[x] = '\0';
		}
		return FinalString;
	}
	static char *Rp2Decrypt(const char *String,int EncryptionFactor,int Start)
	{
		static char FinalString[2000];
		int loops = strlen(String),x = 0;
		int CurrentAdd = Start;
		while (x < loops)
		{
			if (String[x] > 31 && String[x] < 127)
			{
				int Temp = String[x]-CurrentAdd;
				while (!(Temp > 31))
					Temp += 95;
				FinalString[x] = (char)Temp;
			}
			else
				if (String[x] == '\n')
					FinalString[x] = '\0';
				else
					FinalString[x] = String[x];
			x++;
			CurrentAdd++;
			if (CurrentAdd > EncryptionFactor)
				CurrentAdd = 1;
			FinalString[x] = '\0';
		}
		return FinalString;
	}
	static bool IsTruePhysicalObject(GameObject *obj)
	{
		if (obj->As_ScriptableGameObj() && obj->As_PhysicalGameObj())
		{
			if (obj->As_SoldierGameObj())
				return true;
			if (obj->As_VehicleGameObj())
			{
				//TODO: asdfasdf
				/*if (Is_DecorationPhys(obj))
					return false;
				if (Is_DynamicAnimPhys(obj))
					return false;
				if (Is_TimedDecorationPhys(obj))
					return false;*/
				return true;
			}
		}
		return false;
	}
	static float MathClampDegrees(float Value)
	{
		while (Value < -180.0f)
			return Value+360.0f;
		while (Value > 180.0f)
			return Value-360.0f;
		return Value;
	}
	static bool hasStatedDeathMessage[128];
	static void setStatedDeathMessage(int playerId,bool value)
	{
		hasStatedDeathMessage[playerId] = value;
	}
	static void JMG_Player_Death_Message(GameObject *obj,GameObject *killer,const char *overrideMessage)
	{
		int playerId = JMG_Get_Player_ID(obj);
		if (hasStatedDeathMessage[playerId])
			return;
		char deathMsg[500];
		if (overrideMessage)
			sprintf(deathMsg,"%s",overrideMessage);
		else if (!killer)
			sprintf(deathMsg,"%s died",JmgUtility::Get_The_Units_Name(obj));
		else if (killer == obj)
			sprintf(deathMsg,"%s killed %s",JmgUtility::Get_The_Units_Name(obj),JmgUtility::JMG_Get_Units_Sex(obj));
		else
			sprintf(deathMsg,"%s killed %s",JmgUtility::Get_The_Units_Name(killer),JmgUtility::Get_The_Units_Name2(obj));
		if (Commands->Is_A_Star(obj))
			MessageAllPlayers(255,255,0,deathMsg);
		hasStatedDeathMessage[playerId] = true;
	}
	static void SetHUDHelpText(unsigned long stringId,Vector3 color)
	{
		for (int x = 0;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player)
				continue;
			Set_HUD_Help_Text_Player(Player,stringId,color);
		}
	}
};


class NewObjectiveSystem
{
	bool objectiveStringIdsLoaded;
	const char *objectiveNewString;
	const char *objectiveCancelledString;
	const char *objectiveStatusChangedString;
	const char *objectivePrioritieStrings[12];
	const char *objectiveStatusStrings[4];
	const char *objectiveListString;
	const char *objectiveUpdateObjectiveString;
	const char *objectiveCancelledStringNumbered;
	const char *objectiveStatusChangedStringNumbered;
	const char *objectiveUpdateObjectiveStringNumbered;
	char primaryObjectiveModel[32];
	char secondaryObjectiveModel[32];
	char tertiaryObjectiveModel[32];
	bool showRadarStars;
public:
	enum Priority{Undefined=-1,Unknown,Primary,Secondary,Tertiary,Quaternary,Quinary,Senary,Septenary,Octonary,Nonary,Denary,Bonus};
	enum Status{Removed=-2,NotDefined=-1,Pending,Accomplished,Failed,Hidden};
private:
	struct ObjectiveNode
	{
		int id;
		Priority priority;
		Status status;
		unsigned long nameId;
		char *soundFilename;
		unsigned long descriptionId;
		int radarMarkerId;
		bool active;
		int objectiveNumber;
		ObjectiveNode *next;
		ObjectiveNode(int id, Priority priority, Status status, unsigned long nameId, char *soundFilename, unsigned long descriptionId,int radarMarkerId,int objectiveNumber)
		{
			this->id = id;
			this->priority = priority;
			this->status = status;
			this->nameId = nameId;
			this->soundFilename = soundFilename;
			this->descriptionId = descriptionId;
			this->radarMarkerId = radarMarkerId;
			this->active = true;
			this->objectiveNumber = objectiveNumber;
			next = NULL;
		}
	};
	ObjectiveNode *objectiveNodeList;
	int objectiveCounts;
	int team;
	void initilizeStringIds()
	{
		objectiveNewString = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_NEW_OBJ"));
		objectiveCancelledString = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_CANCELLED"));
		objectiveStatusChangedString = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_STATUS_CHANGED"));
		objectiveListString = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_LIST"));
		objectiveUpdateObjectiveString = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_UPDATED"));
		char descriptionString[512];
		for (int x = 0;x < 12;x++)
		{
			sprintf(descriptionString,"IDS_OBJ2_PRIORITY_%0d",x);
			objectivePrioritieStrings[x] = Get_Translated_String(Get_String_ID_By_Desc(descriptionString));
		}
		for (int x = 0;x < 4;x++)
		{
			sprintf(descriptionString,"IDS_OBJ2_STATE_%0d",x);
			objectiveStatusStrings[x] = Get_Translated_String(Get_String_ID_By_Desc(descriptionString));
		}
		objectiveCancelledStringNumbered = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_CANCELLED_NUMBERED"));
		objectiveStatusChangedStringNumbered = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_STATUS_CHANGED_NUMBERED"));
		objectiveUpdateObjectiveStringNumbered = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_UPDATED_NUMBERED"));
		objectiveStringIdsLoaded = true;
	}
	void selectMessageAndColor(const char *format,Priority priority)
	{
		switch (priority)
		{
		case Primary: JmgUtility::MessageTeamPlayersType(50,255,50,team,format); break;
		case Secondary: JmgUtility::MessageTeamPlayersType(50,150,250,team,format); break;
		case Tertiary:case Unknown: JmgUtility::MessageTeamPlayersType(150,50,150,team,format); break;
		default: JmgUtility::MessageTeamPlayersType(125,150,150,team,format); break;
		}
	}
	void messagePlayerAndColor(GameObject *player,const char *format,Priority priority)
	{
		switch (priority)
		{
		case Primary: JmgUtility::DisplayChatMessage(player,50,255,50,format); break;
		case Secondary: JmgUtility::DisplayChatMessage(player,50,150,250,format); break;
		case Tertiary:case Unknown: JmgUtility::DisplayChatMessage(player,150,50,150,format); break;
		default: JmgUtility::DisplayChatMessage(player,125,150,150,format); break;
		}
	}
	char *formatObjectiveString(const char *format,...)
	{
		static char displayMsg[256];
		va_list args;
		va_start(args,format);
		vsprintf(displayMsg,format,args);
		va_end(args);
		return displayMsg;
	}
	bool addObjective(int id, Priority priority, Status status, unsigned long nameId, char *soundFilename, unsigned long descriptionId,int radarMarkerId,int objectiveNumber)
	{
		if (priority == Undefined)
			return false;
		if (!objectiveStringIdsLoaded)
			initilizeStringIds();
		ObjectiveNode *current = objectiveNodeList;
		if (!objectiveNodeList)
			objectiveNodeList = new ObjectiveNode(id,priority,status,nameId,soundFilename,descriptionId,radarMarkerId,objectiveNumber);
		while (current)
		{
			if (current->id == id)
				if (current->active)
				{
					Destroy_Radar_Marker(Commands->Find_Object(radarMarkerId));
					return false;
				}
				else
				{
					Destroy_Radar_Marker(Commands->Find_Object(current->radarMarkerId));
					current->id = id;
					current->priority = priority;
					current->status = status;
					current->nameId = nameId;
					current->soundFilename = soundFilename;
					current->descriptionId = descriptionId;
					current->radarMarkerId = radarMarkerId;
					current->objectiveNumber = objectiveNumber;
					current->active = true;
					break;
				}
			if (!current->next)
			{
				current->next = new ObjectiveNode(id,priority,status,nameId,soundFilename,descriptionId,radarMarkerId,objectiveNumber);
				break;
			}
			current = current->next;
		}
		objectiveCounts++;
		if (status != Hidden && descriptionId)
		{
			selectMessageAndColor(formatObjectiveString(objectiveNewString,objectivePrioritieStrings[priority]),priority);
			if (objectiveNumber)
				selectMessageAndColor(formatObjectiveString(Get_Translated_String(descriptionId),objectiveNumber),priority);
			else
				selectMessageAndColor(Get_Translated_String(descriptionId),priority);
		}
		return true;
	}
	void Destroy_Radar_Marker(GameObject *marker)
	{
		if (!marker)
			return;
		Commands->Destroy_Object(marker);
	}
	GameObject *Create_Radar_Marker(Vector3 pos, Priority priority,const char *modelOverride)
	{
		GameObject *radarMarker = Commands->Create_Object("Daves Arrow",pos);
		Commands->Set_Player_Type(radarMarker,team);
		Commands->Set_Is_Visible(radarMarker,false);
		if (modelOverride)
			Commands->Set_Model(radarMarker,modelOverride);
		else
			switch (priority)
			{
				case Priority::Primary:Commands->Set_Model(radarMarker,primaryObjectiveModel);break;
				case Priority::Secondary:Commands->Set_Model(radarMarker,secondaryObjectiveModel);break;
				case Priority::Tertiary:Commands->Set_Model(radarMarker,tertiaryObjectiveModel);break;
				default:Commands->Set_Model(radarMarker,"null");break;
			}
		if (showRadarStars)
		{
			Commands->Set_Obj_Radar_Blip_Shape(radarMarker,RADAR_BLIP_SHAPE_OBJECTIVE);
			Commands->Set_Obj_Radar_Blip_Color(radarMarker,priority == Primary ? RADAR_BLIP_COLOR_PRIMARY_OBJECTIVE : priority == Secondary ? RADAR_BLIP_COLOR_SECONDARY_OBJECTIVE : RADAR_BLIP_COLOR_TERTIARY_OBJECTIVE);
		}
		return radarMarker;
	}
public:
	NewObjectiveSystem(int team,bool showRadarStars = true,const char *primaryObjectiveModel = "null",const char *secondaryObjectiveModel = "null",const char *tertiaryObjectiveModel = "null")
	{
		this->team = team;
		objectiveStringIdsLoaded = false;
		objectiveCounts = 0;
		sprintf(this->primaryObjectiveModel,"%s",primaryObjectiveModel);
		sprintf(this->secondaryObjectiveModel,"%s",secondaryObjectiveModel);
		sprintf(this->tertiaryObjectiveModel,"%s",tertiaryObjectiveModel);
		this->showRadarStars = showRadarStars;
		objectiveNodeList = NULL;
	}
	~NewObjectiveSystem()
	{
		objectiveStringIdsLoaded = false;
		objectiveCounts = 0;
		ObjectiveNode *temp = objectiveNodeList,*die;
		while (temp)
		{
			die = temp;
			temp = temp->next;
			delete die;
		}
		objectiveNodeList = NULL;
	}
	bool Add_Objective(int objectiveId, Priority priority, Status status, unsigned long nameId, char *soundFilename, unsigned long descriptionId,GameObject *blipUnit,const char *modelOverride = NULL,int objectiveNumber = 0)
	{
		if (!blipUnit)
			return false;
		GameObject *radarMarker = Create_Radar_Marker(Vector3(),priority,modelOverride);
		if (!radarMarker)
			return false;
		Commands->Attach_To_Object_Bone(radarMarker,blipUnit,"origin");
		return addObjective(objectiveId,priority,status,nameId,soundFilename,descriptionId,Commands->Get_ID(radarMarker),objectiveNumber);
	}
	bool Add_Objective(int objectiveId, Priority priority, Status status, unsigned long nameId, char *soundFilename, unsigned long descriptionId,Vector3 blipPosition,const char *modelOverride = NULL,int objectiveNumber = 0)
	{
		GameObject *radarMarker = Create_Radar_Marker(blipPosition,priority,modelOverride);
		if (!radarMarker)
			return false;
		return addObjective(objectiveId,priority,status,nameId,soundFilename,descriptionId,Commands->Get_ID(radarMarker),objectiveNumber);
	}
	bool Get_Radar_Blip_Position(int objectiveId,Vector3 *position)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
			{
				GameObject *objectiveMarker = Commands->Find_Object(current->radarMarkerId);
				if (objectiveMarker)
				{
					*position = Commands->Get_Position(objectiveMarker);
					return true;
				}
				return false;
			}
			current = current->next;
		}
		return false;
	}
	void Set_Radar_Blip(int objectiveId,GameObject *blipUnit,const char *modelOverride = NULL)
	{
		if (!blipUnit)
			return;
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
			{
				Destroy_Radar_Marker(Commands->Find_Object(current->radarMarkerId));
				GameObject *radarMarker = Create_Radar_Marker(Vector3(),current->priority,modelOverride);
				if (!radarMarker)
					return;
				Commands->Attach_To_Object_Bone(radarMarker,blipUnit,"origin");
				current->radarMarkerId = Commands->Get_ID(radarMarker);
				return;
			}
			current = current->next;
		}
	}
	void Set_Radar_Blip(int objectiveId,Vector3 blipPosition,const char *modelOverride = NULL)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
			{
				Destroy_Radar_Marker(Commands->Find_Object(current->radarMarkerId));
				GameObject *radarMarker = Create_Radar_Marker(blipPosition,current->priority,modelOverride);
				if (!radarMarker)
					return;
				current->radarMarkerId = Commands->Get_ID(radarMarker);
				return;
			}
			current = current->next;
		}
	}
	bool Add_Objective(int objectiveId, Priority priority, Status status, unsigned long nameId, char *soundFilename, unsigned long descriptionId,int objectiveNumber = 0)
	{
		return addObjective(objectiveId,priority,status,nameId,soundFilename,descriptionId,0,objectiveNumber);
	}
	long Get_Mission_Text_Id(int objectiveId)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
				return current->nameId;
			current = current->next;
		}
		return 0;
	}
	bool Remove_Objective(int objectiveId)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
				if (current->active)
				{
					objectiveCounts--;
					if (current->status == Pending && current->nameId)
						if (current->objectiveNumber)
							selectMessageAndColor(formatObjectiveString(objectiveCancelledStringNumbered,objectivePrioritieStrings[current->priority],current->objectiveNumber),current->priority);
						else
							selectMessageAndColor(formatObjectiveString(objectiveCancelledString,objectivePrioritieStrings[current->priority]),current->priority);
					Destroy_Radar_Marker(Commands->Find_Object(current->radarMarkerId));
					current->active = false;
					return true;
				}
				else
					return false;
			current = current->next;
		}
		return true;
	}
	bool Set_Objective_Status(int objectiveId,Status status)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
				if (current->status != status)
				{
					if (status != Hidden && current->status != Hidden && current->nameId)
						if (current->objectiveNumber)
							selectMessageAndColor(formatObjectiveString(objectiveStatusChangedStringNumbered,objectivePrioritieStrings[current->priority],current->objectiveNumber,objectiveStatusStrings[status]),current->priority);
						else
							selectMessageAndColor(formatObjectiveString(objectiveStatusChangedString,objectivePrioritieStrings[current->priority],objectiveStatusStrings[status]),current->priority);
					GameObject *marker = Commands->Find_Object(current->radarMarkerId);
					if (marker)
					{
						if (status == Pending)
						{
							Commands->Set_Is_Rendered(marker,true);
							if (showRadarStars)
								Commands->Set_Obj_Radar_Blip_Shape(marker,RADAR_BLIP_SHAPE_OBJECTIVE);
						}
						else
						{
							Commands->Set_Is_Rendered(marker,false);
							if (showRadarStars)
								Commands->Set_Obj_Radar_Blip_Shape(marker,RADAR_BLIP_SHAPE_NONE);
						}
						Commands->Set_Player_Type(marker,status == Pending);
					}
					current->status = status;
					return true;
				}
				else
					return false;
			current = current->next;
		}
		return true;
	}
	bool Set_Objective_Mission(int objectiveId,unsigned int nameStringId,unsigned int descriptionStringId)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
				if (current->nameId != nameStringId || current->descriptionId != descriptionStringId)
				{
					current->nameId = nameStringId;
					current->descriptionId = descriptionStringId;
					if (current->status != Hidden && descriptionStringId)
					{
						if (current->objectiveNumber)
						{
							selectMessageAndColor(formatObjectiveString(objectiveUpdateObjectiveStringNumbered,objectivePrioritieStrings[current->priority],current->objectiveNumber),current->priority);
							selectMessageAndColor(formatObjectiveString(Get_Translated_String(descriptionStringId),current->objectiveNumber),current->priority);
						}
						else
						{
							selectMessageAndColor(formatObjectiveString(objectiveUpdateObjectiveString,objectivePrioritieStrings[current->priority]),current->priority);
							selectMessageAndColor(Get_Translated_String(descriptionStringId),current->priority);
						}
					}
					return true;
				}
				else
					return false;
			current = current->next;
		}
		return true;
	}
	Status Get_Objective_Status(int objectiveId)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
				if (current->active)
					return current->status;
				else
					return Removed;
			current = current->next;
		}
		return NotDefined;
	}
	int Get_Objective_Status_Count(Status status,Priority requiredPriority = Undefined)
	{
		int count = 0;
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->active && current->status == status && (requiredPriority == Undefined || current->priority == requiredPriority))
				count++;
			current = current->next;
		}
		return count;
	}
	int Get_Objective_Priority_Count(Priority requiredPriority,Status status = NotDefined)
	{
		int count = 0;
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->active && current->priority == requiredPriority && (status == NotDefined || current->status == status))
				count++;
			current = current->next;
		}
		return count;
	}
	void Display_Current_Objectives(GameObject *player,Priority priority)
	{
		messagePlayerAndColor(player,formatObjectiveString(objectiveListString,objectivePrioritieStrings[priority]),priority);
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->active && current->status == Pending && current->priority == priority && current->nameId)
			{
				char objectiveMsg[220];
				if (current->objectiveNumber)
					sprintf(objectiveMsg,"*%s",formatObjectiveString(Get_Translated_String(current->nameId),current->objectiveNumber));
				else
					sprintf(objectiveMsg,"*%s",Get_Translated_String(current->nameId));
				messagePlayerAndColor(player,objectiveMsg,priority);
			}
			current = current->next;
		}
	}
	void Display_All_Objectives(GameObject *player)
	{
		int counts[12];
		for (int x = 0;x < 12;x++)
			counts[x] = Get_Objective_Status_Count(Pending,(Priority)x);
		for (int x = 0;x < 12;x++)
			if (counts[x])
				Display_Current_Objectives(player,(Priority)x);
	}
	int Get_First_Pending_Objective_Of_Priority(Priority priority)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->active && current->status == Pending && current->priority == priority && current->nameId)
				return current->id;
			current = current->next;
		}
		return 0;
	}

};

class ClientNetworkObjectPositionSync
{
public:
	struct SyncObjectNode
	{
		int id;
		GameObject *obj;
		float facing;
		Vector3 position;
		struct SyncObjectNode *next;
		SyncObjectNode(GameObject *obj)
		{
			this->id = Commands->Get_ID(obj);
			this->obj = obj;
			this->facing = Commands->Get_Facing(obj);
			this->position = Commands->Get_Position(obj);
			this->next = NULL;
		}

	};
private:
	SyncObjectNode *syncObjectNodeList;
	SyncObjectNode *currentSyncNodeListPosition;
	struct SyncControl
	{
		bool playersSynced;
		bool syncedPlayers[128];
		SyncObjectNode *lastSyncNode[128];
		SyncControl()
		{
			playersSynced = false;
			for (int x = 0;x < 128;x++)
			{
				syncedPlayers[x] = false;
				lastSyncNode[x] = NULL;
			}
		}
		bool isSynced(int playerId,SyncObjectNode *currentSyncNode)
		{
			if (syncedPlayers[playerId])
				return true;
			if (!syncedPlayers[playerId] && !lastSyncNode[playerId])
			{
				lastSyncNode[playerId] = currentSyncNode;
				return false;
			}
			if (!syncedPlayers[playerId] && lastSyncNode[playerId] == currentSyncNode)
			{
				syncedPlayers[playerId] = true;
				return true;
			}
			return false;
		}
		void clientNoLongerSynced(int playerId)
		{
			syncedPlayers[playerId] = false;
			lastSyncNode[playerId] = NULL;
		}
	};
	SyncControl syncControl;
	void updateSyncStates()
	{
		bool allPlayersSynced = true;
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			bool synced = syncControl.isSynced(x,currentSyncNodeListPosition);
			if (!player && synced)
				syncControl.clientNoLongerSynced(x);
			if (player && !synced)
				allPlayersSynced = false;
		}
		syncControl.playersSynced = allPlayersSynced;
	}
public:
	ClientNetworkObjectPositionSync()
	{
		currentSyncNodeListPosition = NULL;
		syncObjectNodeList = NULL;
		syncControl = SyncControl();
	}
	SyncObjectNode *addNode(GameObject *obj)
	{
		int id = Commands->Get_ID(obj);
		SyncObjectNode *current = syncObjectNodeList;
		if (!syncObjectNodeList)
			syncObjectNodeList = new SyncObjectNode(obj);
		while (current)
		{
			if (!current->id)
			{
				current->id = id;
				current->obj = obj;
				current->facing = Commands->Get_Facing(obj);
				current->position = Commands->Get_Position(obj);
				return current;
			}
			if (current->id == id)
				return current;
			if (!current->next)
			{
				current->next = new SyncObjectNode(obj);
				return current->next;
			}
			current = current->next;
		}
		return NULL;
	};
	void checkForPlayersThatLeftTheGame()
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player && syncControl.lastSyncNode)
				syncControl.clientNoLongerSynced(x);
		}
	}
	void triggerSingleNetworkSync()
	{
		updateSyncStates();
		if (syncControl.playersSynced)
			return;
		if (!currentSyncNodeListPosition)
			currentSyncNodeListPosition = syncObjectNodeList;
		else
			currentSyncNodeListPosition = currentSyncNodeListPosition->next;
		if (!currentSyncNodeListPosition)
			return;
		Force_Position_Update(currentSyncNodeListPosition->obj);
	}
	void Empty_List()
	{
		SyncObjectNode *temp = currentSyncNodeListPosition,*die;
		while (temp)
		{
			die = temp;
			temp = temp->next;
			delete die;
		}
		currentSyncNodeListPosition = NULL;
		syncObjectNodeList = NULL;
		syncControl = SyncControl();
	}
};

/*!
* \brief An object that will have its position synced by JMG_Utility_Sync_System_Controller
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Sync_System_Object : public ScriptImpClass
{
	ClientNetworkObjectPositionSync::SyncObjectNode *syncNode;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
public:
	JMG_Utility_Sync_System_Object()
	{
		syncNode = NULL;
	}
};

/*!
* \brief Controls all objects that have the script JMG_Utility_Sync_System_Object
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Sync_System_Controller : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
public:
	JMG_Utility_Sync_System_Controller();
};

/*!
* \brief Syncs object positions between the client and the server
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Sync_Object_Periodically : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Turns a placed object into a renstyle spawner that can be sent customs like a normal object
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Basic_Spawner : public ScriptImpClass
{
	int spawnLimit;
	int spawnedId;
	bool enabled;
	float respawnTime;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void CalculateRespawnTime();
};

class JMG_Utility_Basic_Spawner_Attach : public ScriptImpClass
{
	void Destroyed(GameObject *obj);
};
