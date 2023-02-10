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
#include "General.h"
#include "DataSafe.h"
#include "Random2Class.h"
SCRIPTS_API REF_DEF2(GenericDataSafeClass::SimpleKey,unsigned long,0x00829478,0x00828660);
SCRIPTS_API REF_DEF2(GenericDataSafeClass::HandleKey,unsigned long,0x00820DD0,0x0081FFB8);
SCRIPTS_API REF_DEF2(GenericDataSafeClass::Checksum,unsigned long,0x008294D8,0x008286C0);
SCRIPTS_API REF_DEF2(GenericDataSafeClass::ShuffleDelay,unsigned long,0x00829470,0x00828658);
SCRIPTS_API REF_DEF2(GenericDataSafeClass::SecurityCheckDelay,unsigned long,0x00829468,0x00828650);
SCRIPTS_API REF_DEF2(GenericDataSafeClass::SentinelOne,DataSafeHandleClass,0x00829464,0x0082864C);
SCRIPTS_API REF_DEF2(GenericDataSafeClass::NumLists,int,0x008294E8,0x008286D0);
SCRIPTS_API REF_DEF2(GenericDataSafeClass::Safe,TT_NOOP(RefArray<DataSafeEntryListClass *,MAX_DATASAFE_LISTS>),0x00820E44,0x0082002C);
SCRIPTS_API REF_DEF2(GenericDataSafeClass::TypeList,TT_NOOP(RefArray<DataSafeEntryTypeClass,MAX_DATASAFE_TYPES>),0x00828E60,0x00828048);
SCRIPTS_API REF_DEF2(GenericDataSafeClass::TypeListCount,int,0x008294EC,0x008286D4);
SCRIPTS_API REF_DEF2(GenericDataSafeClass::SentinelTwo,DataSafeHandleClass,0x008294B0,0x00828698);
SCRIPTS_API REF_DEF2(GenericDataSafeClass::CRCErrors,int,0x008294F0,0x008286D8);
SCRIPTS_API REF_DEF2(GenericDataSafeClass::PreferredThread,unsigned int,0x00829480,0x00828668);

#ifndef TT_EXPORTS
RENEGADE_FUNCTION
SCRIPTS_API void GenericDataSafeClass::Reset_Timers(void)
AT2(0x0045B5C0,0x0045B050);

RENEGADE_FUNCTION
SCRIPTS_API void GenericDataSafeClass::Reset(void)
AT2(0x0045B640,0x0045B0D0)

RENEGADE_FUNCTION
SCRIPTS_API void GenericDataSafeClass::Shutdown(void)
AT2(0x0045C050,0x0045BAE0)

RENEGADE_FUNCTION
SCRIPTS_API DataSafeEntryClass *GenericDataSafeClass::Get_Entry(DataSafeHandleClass handle)
AT2(0x0045C0B0,0x0045BB40)

RENEGADE_FUNCTION
SCRIPTS_API int GenericDataSafeClass::Get_Entry_Type(DataSafeHandleClass handle)
AT2(0x0045C4E0,0x0045BF70)

RENEGADE_FUNCTION
SCRIPTS_API DataSafeEntryClass *GenericDataSafeClass::Get_Entry_By_Index(int list, int index)
AT2(0x0045CAF0,0x0045C580)

RENEGADE_FUNCTION
SCRIPTS_API void GenericDataSafeClass::Mem_Copy_Encrypt(void *dest, void *src, int size, bool do_checksum)
AT2(0x0045CB20,0x0045C5B0)

RENEGADE_FUNCTION
SCRIPTS_API void GenericDataSafeClass::Mem_Copy_Decrypt(void *dest, void *src, int size, bool do_checksum)
AT2(0x0045CBA0,0x0045C630)

RENEGADE_FUNCTION
SCRIPTS_API void GenericDataSafeClass::Encrypt(void *data, int size, unsigned long key, bool do_checksum)
AT2(0x0045CC20,0x0045C6B0)

RENEGADE_FUNCTION
SCRIPTS_API void GenericDataSafeClass::Decrypt(void *data, int size, unsigned long key, bool do_checksum)
AT2(0x0045CC80,0x0045C710)

RENEGADE_FUNCTION
SCRIPTS_API int GenericDataSafeClass::Get_Random_List_For_Insertion(int type)
AT2(0x0045CCF0,0x0045C780)

RENEGADE_FUNCTION
SCRIPTS_API int GenericDataSafeClass::Create_Safe_List(int type)
AT2(0x0045CE60,0x0045C8F0)

RENEGADE_FUNCTION
SCRIPTS_API void GenericDataSafeClass::Random_Insertion(DataSafeEntryClass *entry_ptr, int list, int type, bool is_slop)
AT2(0x0045CED0,0x0045C960)

RENEGADE_FUNCTION
SCRIPTS_API void GenericDataSafeClass::Swap_Entries(DataSafeEntryClass *first, DataSafeEntryClass *second, int type)
AT2(0x0045CF90,0x0045CA20)

RENEGADE_FUNCTION
SCRIPTS_API void GenericDataSafeClass::Remove_From_List(int list, DataSafeEntryClass *entry_ptr)
AT2(0x0045D050,0x0045CAE0)

RENEGADE_FUNCTION
SCRIPTS_API void GenericDataSafeClass::Shuffle(bool forced)
AT2(0x0045D120,0x0045CBB0)

RENEGADE_FUNCTION
SCRIPTS_API int GenericDataSafeClass::Get_Handle_ID(int list)
AT2(0x0045DC50,0x0045D6E0)

RENEGADE_FUNCTION
SCRIPTS_API void GenericDataSafeClass::Free_Handle_ID(int list, int id)
AT2(0x0045DC80,0x0045D710)
#endif

SCRIPTS_API void GenericDataSafeClass::Say_Security_Fault(void)
{
	__debugbreak();
}

char ErrorVal[1024] = {0,0,0,0};

SCRIPTS_API REF_DEF2(DataSafeClass<int>::ReturnIndex,int,0x00829488,0x00828670);
SCRIPTS_API REF_DEF2(DataSafeClass<int>::ReturnList,TT_NOOP(RefArray<RefArray<char, sizeof(int)>,MAX_OBJECT_COPIES>),0x0082948C,0x00828674);
SCRIPTS_API REF_DEF2(DataSafeClass<int>::MinSlop,int,0x00828E44,0x0082802C);
SCRIPTS_API REF_DEF2(DataSafeClass<int>::Type,int,0x00820DDC,0x0081FFC4);

SCRIPTS_API REF_DEF2(DataSafeClass<float>::ReturnIndex,int,0x008294DC,0x008286C4);
SCRIPTS_API REF_DEF2(DataSafeClass<float>::ReturnList,TT_NOOP(RefArray<RefArray<char, sizeof(float)>,MAX_OBJECT_COPIES>),0x008294B4,0x0082869C);
SCRIPTS_API REF_DEF2(DataSafeClass<float>::MinSlop,int,0x0082946C,0x00828654);
SCRIPTS_API REF_DEF2(DataSafeClass<float>::Type,int,0x008294E0,0x008286C8);

SCRIPTS_API REF_DEF2(DataSafeClass<unsigned int>::ReturnIndex,int,0x00829474,0x0082865C);
SCRIPTS_API REF_DEF2(DataSafeClass<unsigned int>::ReturnList,TT_NOOP(RefArray<RefArray<char, sizeof(unsigned int)>,MAX_OBJECT_COPIES>),0x00820DE0,0x0081FFC8);
SCRIPTS_API REF_DEF2(DataSafeClass<unsigned int>::MinSlop,int,0x00820DD4,0x0081FFBC);
SCRIPTS_API REF_DEF2(DataSafeClass<unsigned int>::Type,int,0x0082947C,0x00828664);
