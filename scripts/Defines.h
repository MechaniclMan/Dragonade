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
#ifndef TT_INCLUDE__DEFINES_H
#define TT_INCLUDE__DEFINES_H

// put global defines here

#include "Types.h"
#include "stdarg.h"
#if (WWCONFIG) || (TDBEDIT) || (W3DSHADER) || (W3DLIB_EXPORTS) || (W3DMESHMENDER) || (W3DDEPENDS) || (W3DMAPPER) || (ACHASH) || (PACKAGEEDITOR) || (FIXPLANES) || (MERGELOD) || (MIXCHECK) || (DDBEDIT) || (MAKEMIX)
#define EXTERNAL 1
#endif

#pragma warning(disable: 4100) // unreferenced formal parameter
#pragma warning(disable: 6011) // Dereferencing NULL pointer (used by renegade variable access functions)
#pragma warning(disable: 4275) // non dll-interface class '<class>' used as base for dll-interface class '<class>'
#pragma warning(disable: 4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable: 6290) // Bitwise operation on logical result: ! has higher precedence than &. Use && or (!(x & y)) instead
#pragma warning(disable: 6211) // Leaking memory 'newEntries' due to an exception. Consider using a local catch block to clean up memory
#pragma warning(disable: 4505) // unreferenced local function has been removed

//class needs to have dll-interface to used by clients of class. Except that it doesn't. 
//If it did, the linker would complain.
#pragma warning(disable:4251)

#define WIN32_LEAN_AND_MEAN
#define _USE_MATH_DEFINES

// breakpoint
#define TT_INTERRUPT  { __asm { __asm int 3 } }

// assert
#ifdef DEBUG
#	define TT_ASSERT(expression) \
   {                             \
      if (!(expression))         \
         TT_INTERRUPT;            \
   }
#else
#	define TT_ASSERT(expression)  {}
#endif

// assumption optimization
#ifdef DEBUG
#	define TT_ASSUME(x) TT_ASSERT(x)
#elif defined(NDEBUG) && defined(_MSC_VER)
#	define TT_ASSUME(x) __assume(x)
#else
#	define TT_ASSUME(x)
#endif

// unreachable code
#	ifdef DEBUG
#		define TT_UNREACHABLE TT_INTERRUPT
#	else
#		define TT_UNREACHABLE __assume(false);
#	endif

#define TT_UNIMPLEMENTED TT_INTERRUPT
#define TT_UNTESTED TT_INTERRUPT

// deprecated code
#define TT_DEPRECATED(x) __declspec(deprecated(x))

// inlined function calls
#if defined(NDEBUG) && defined(_MSC_VER)
#	define TT_INLINE __forceinline
#else
#	define TT_INLINE inline
#endif

// countof for static array
template <typename T, size_t N>
char ( &_ArraySizeHelper( T (&array)[N] ))[N];

#define countof( array ) (sizeof( _ArraySizeHelper( array ) ))

// This can be used as a workaround for when a template type in a class is needed.
// something like MACRO(Class<Param1, Param2>) will make the preprocessor think two
// parameters are supplied ("Class<Param1" and "Param2>").
#define TT_NOOP(...) __VA_ARGS__

// unknown type
#define UNK TT_DEPRECATED("Unknown type") uint32
#define UNKB TT_DEPRECATED("Unknown type") uint8
#define UNKADDRESS 0x00000000

// to communicate with Renegade
#ifndef EXTERNAL
template <typename T> T& ResolveGameReference(const int client, const int server, const int leveledit)
{
	if (Exe == 6) InitEngine();
	return *((T*)((Exe == 0) ? client : ((Exe == 1) ? server : ((Exe == 4) ? leveledit : 0))));
};

template <typename T, const int size> class RefArrayHelper
{
protected:
	char _dummy[size * sizeof(T)];
public:
	operator T* ()
	{
		return (T*) this;
	};

	operator const T* () const
	{
		return (T*) this;
	};

	T* operator & ()
	{
		return (T*) this;
	};

	const T* operator & () const
	{
		return (T*) this;
	};
};


#ifndef TTLE_EXPORTS
#define REF_DECL1(name, type) type & name
#define REF_DEF1(name, type, client) type & name = *(( type *) client )
#define REF_DECL2(name, type) type & name
#define REF_DEF2(name, type, client, server) type & name = ResolveGameReference<type>(client, server, 0)
#else //!TTLE_EXPORTS
/*#define REF_DECL1(name, type)
#define REF_DEF1(name, type, client)
#define REF_DECL2(name, type)
#define REF_DEF2(name, type, client, server)*/
#endif //TTLE_EXPORTS



#define REF_DECL3(name, type) type & name
#define REF_DEF3(name, type, client, server, leveledit) type & name = ResolveGameReference<type>(client, server, leveledit)

#ifndef TTLE_EXPORTS
#define REF_ARR_DECL1(name, type, size) RefArrayHelper<type, size> & name
#define REF_ARR_DEF1(name, type, size, client) RefArrayHelper <type, size> & name = *((RefArrayHelper< type, size >*) client)
#define REF_ARR_DECL2(name, type, size) RefArrayHelper<type, size> & name
#define REF_ARR_DEF2(name, type, size, client, server) RefArrayHelper<type, size> & name = ResolveGameReference<RefArrayHelper< type, size >> (client, server, 0)
#else //!TTLE_EXPORTS
/*#define REF_ARR_DECL1(name, type, size)
#define REF_ARR_DEF1(name, type, size, client)
#define REF_ARR_DECL2(name, type, size)
#define REF_ARR_DEF2(name, type, size, client, server)*/
#endif //TTLE_EXPORTS


#define REF_ARR_DECL3(name, type, size) RefArrayHelper<type, size> & name
#define REF_ARR_DEF3(name, type, size, client, server, leveledit) RefArrayHelper<type, size> & name = ResolveGameReference<RefArrayHelper< type, size >> ( client, server, leveledit)

#define REF_2DARR_DECL1(name, type, size_inner, size_outer) RefArrayHelper<RefArrayHelper<type, size_outer>, size_inner> & name
#define REF_2DARR_DEF1(name, type, size_inner, size_outer, client) RefArrayHelper<RefArrayHelper<type, size_outer>, size_inner> & name = *((RefArrayHelper<RefArrayHelper<type, size_outer>, size_inner>*) client)
#define REF_2DARR_DECL2(name, type, size_inner, size_outer) REF_2DARR_DECL1 ( name, type, size_inner, size_outer ) 
#define REF_2DARR_DEF2(name, type, size_inner, size_outer, client, server) RefArrayHelper<RefArrayHelper<type, size_outer>, size_inner> & name = ResolveGameReference<RefArrayHelper<RefArrayHelper<type, size_outer>, size_inner>> (client, server, 0)
#define REF_2DARR_DECL3(name, type, size_inner, size_outer) REF_2DARR_DECL1 ( name, type, size_inner, size_outer ) 
#define REF_2DARR_DEF3(name, type, size_inner, size_outer, client, server, leveledit) RefArrayHelper<RefArrayHelper<type, size_outer>, size_inner> & name = ResolveGameReference<RefArrayHelper<RefArrayHelper<type, size_outer>, size_inner>> (client, server, leveledit)

// Q: So what are these ATx things anyways?
// A: The ATx macros are are designed for transparently calling functions that may have 
// multiple implementations depending on the current system configuration. The most 
// common use of them that you'll see is calling functions inside Renegade. They work 
// by dynamically rewriting code in order to call the proper function with little
// to no overhead, so do not modify them unless you *really* know what you are doing. 
//
// Q: And I use them how exactly?
// A: For ATx, make sure the function is declared with RENEGADE_FUNCTION and 
// then use ATx as the function implemention. 
//
// Examples:
// RENEGADE_FUNCTION void ExampleFunction()		AT1(0xfeedface);
// RENEGADE_FUNCTION void ExampleFunction2()	AT2(0xdeadbeef, 0xbaadf00d);
// RENEGADE_FUNCTION void ExampleFunction2()	AT2(0xdeadbeef, 0xbaadf00d, 0xdeadc0de);
//
// ATxc is a bit more complicated. You need to provide a "version selector" function 
// and pass that to ATxc as the final parameter. If the version selector function 
// returns 0, then A will be used for all calls to the function. If it returns 1, B will be
// used. If it returns 2, C will be used. You should use ATxc when you want to provide 
// multiple versions of a function which may require different configurations or hardware
// capabilities and you don't want to manage function pointers manually.
//
// Example:
// // declared somewhere in current scope
// int ExampleAT2VersionSelector();
// void ExampleFunction_A();
// void ExampleFunction_B(); 
// // the actual function
// RENEGADE_FUNCTION void ExampleFunction() AT2c(ExampleFunction_A, ExampleFunction_B, ExampleAT2VersionSelector);

#ifdef SHADERS_EXPORTS
#define SHADERS_API __declspec(dllexport)
#else
#ifdef EXTERNAL
#define SHADERS_API
#else
#define SHADERS_API __declspec(dllimport)
#endif
#endif

#if (DAPLUGIN) || (SSGMPLUGIN)
#define SCRIPTS_API __declspec(dllimport)
#define DA_API __declspec(dllimport)
#else
#define SCRIPTS_API __declspec(dllexport)
#define DA_API __declspec(dllexport)
#endif

SCRIPTS_API void InitEngine();
SCRIPTS_API int		RenegadeAT3VersionSelector(); // Default implementation, used by AT1/AT2
SCRIPTS_API void*	HookupAT3(void* a, void* b, void* c, void* patch_start, void* patch_end, int (*function_selector)());
void* HookupAT3x(void* a, void* b, void* c, void* patch_start, void* patch_end, int (*version_selector)());
#define RENEGADE_FUNCTION  __declspec(naked)
#ifndef TTLE_EXPORTS
#define AT1(client)						AT2(client, 0)
#define AT2(client, server)				AT2c(client, server, RenegadeAT3VersionSelector)	
#endif
#define AT3(client, server, leveledit)	AT3c(client, server, leveledit, RenegadeAT3VersionSelector)
#ifndef TTLE_EXPORTS
#define AT2c(a, b, ver_selector)		AT3c(a, b, 0, ver_selector)
#endif
#define AT3c(a, b, c, ver_selector)	\
{									\
	__asm AT3PatchStart:			\
	__asm push ecx					\
	__asm push edx					\
	__asm push ver_selector			\
	__asm push offset AT3PatchEnd	\
	__asm push offset AT3PatchStart	\
	__asm push c					\
	__asm push b					\
	__asm push a					\
	__asm call HookupAT3x			\
	__asm AT3PatchEnd:				\
	__asm add esp, 24				\
	__asm pop edx					\
	__asm pop ecx					\
	__asm jmp eax					\
}

SCRIPTS_API extern int Exe;  // used by Vx()/ATx() macros
#else
#define SCRIPTS_API
#define SHADERS_API
#endif
template<int stackBufferLength, typename Char> class FormattedString;

template<int stackBufferLength> class FormattedString<stackBufferLength, char>
{

public:

	char stackBuffer[stackBufferLength+1];
	char* heapBuffer;
	const char* value;
	int length;

	FormattedString(const char* format, ...)
	{
		va_list arguments;
		va_start(arguments, format);
		length = vsnprintf(stackBuffer, stackBufferLength, format, arguments);
		if (length >= 0)
		{
			// The formatted string fit on the stack. Use the stack buffer.
			stackBuffer[length] = '\0'; // Fix terminator. Only necessary if length == stackBufferLength.
			heapBuffer = 0;
			value = stackBuffer;
		}
		else
		{
			// The formatted string did not fit on the stack. Allocate a heap buffer.
			length = _vscprintf(format, arguments);
			heapBuffer = new char[length + 1];
			vsprintf(heapBuffer, format, arguments);
			value = heapBuffer;
		}
		va_end(arguments);
	}

	~FormattedString()
	{
		delete[] heapBuffer;
	}
	
	const char* getValue() const { return value; }
	const int getLength() const { return length; }
};

template<int stackBufferLength> class FormattedString<stackBufferLength, wchar_t>
{

public:

	wchar_t stackBuffer[stackBufferLength+1];
	wchar_t* heapBuffer;
	const wchar_t* value;
	int length;

	FormattedString(const wchar_t* format, ...)
	{
		va_list arguments;
		va_start(arguments, format);
		length = _vsnwprintf(stackBuffer, stackBufferLength, format, arguments);
		if (length >= 0)
		{
			// The formatted string fit on the stack. Use the stack buffer.
			stackBuffer[length] = '\0'; // Fix terminator. Only necessary if length == stackBufferLength.
			heapBuffer = 0;
			value = stackBuffer;
		}
		else
		{
			// The formatted string did not fit on the stack. Allocate a heap buffer.
			length = _vscwprintf(format, arguments);
			heapBuffer = new wchar_t[length + 1];
			_vsnwprintf(heapBuffer, length + 1, format, arguments);
			value = heapBuffer;
		}
		va_end(arguments);
	}

	~FormattedString()
	{
		delete[] heapBuffer;
	}
	
	const wchar_t* getValue() const { return value; }
	const int getLength() const { return length; }
};

// Format a string (sprintf style) using a stack buffer of a given size if possible, or a heap buffer otherwise.
#define TT_FORMAT(maxFormattedLength, format, ...) FormattedString<maxFormattedLength, char>(format, __VA_ARGS__).getValue()
#define TT_FORMAT_WIDE(maxFormattedLength, format, ...) FormattedString<maxFormattedLength, wchar_t>(format, __VA_ARGS__).getValue()

#endif
