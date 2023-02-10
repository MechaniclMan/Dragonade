/*	Renegade Scripts.dll
    Dragonade Game Feature Class
	Copyright 2013 Whitedragon, Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/

#ifndef INCLUDE_DAGAMEFEATURE
#define INCLUDE_DAGAMEFEATURE

class DAGameFeatureClass;

class DAGameFeatureFactoryClass abstract {
public:	
	inline const char *Get_Name() const {
		return Name;
	}
	inline const char *Get_Enable_Setting() const {
		return EnableSetting;
	}
	inline const char *Get_Required_Data() const {
		return RequiredData;
	}
	inline DAGameFeatureClass *Get_Instance() const {
		return Instance;
	}
	bool Check_Map_Support() const;
	bool Check_Enabled() const;
	virtual DAGameFeatureClass *Create_Instance() = 0;
	void Destroy_Instance();
	
protected:
	DA_API void Add_Game_Feature();
	const char *Name;
	const char *EnableSetting;
	const char *RequiredData;
	DAGameFeatureClass *Instance;
};

class DAGameFeatureClass abstract {
public:
	inline const char *Get_Name() {
		return Registrant->Get_Name();
	}
	virtual void Init() = 0;
	virtual ~DAGameFeatureClass() { }
	
protected:
	template <typename T> friend class DAGameFeatureRegistrant;
	const DAGameFeatureFactoryClass *Registrant;
};

template <class T> class DAGameFeatureRegistrant : public DAGameFeatureFactoryClass {
public:
	DAGameFeatureRegistrant(const char *Name,const char *EnableSetting,const char *RequiredData = 0) {
		this->Instance = 0;
		this->Name = Name;
		this->EnableSetting = EnableSetting;
		this->RequiredData = RequiredData;
		Add_Game_Feature();
	}
	
protected:
	virtual DAGameFeatureClass *Create_Instance() {
		if (!Instance) {
			Instance = new T;
			Instance->Registrant = this;
			Instance->Init();
		}
		return Instance;
	}
};

#define Register_Game_Feature(ClassName,Name,EnableSetting,RequiredData) DAGameFeatureRegistrant<ClassName> ClassName##Registrant(Name,EnableSetting,RequiredData);

#endif
