#pragma once
#ifndef _DBOX_PLATFORMTYPES_H_INCLUDED_
#define _DBOX_PLATFORMTYPES_H_INCLUDED_
//------------------------------------------------------------------------
// D-BOX PUBLIC PLATFORM TYPES
// Copyright (C) 2008, D-BOX Technologies Inc.
// All rights reserved.
//------------------------------------------------------------------------
#if defined(__GNUC__)
	#include <stdint.h>
	#include <wchar.h>
#endif

//------------------------------------------------------------------------
// PLATFORM LIBRARY SELECTION MACRO
//------------------------------------------------------------------------
#ifdef _MSC_VER
	// See also: http://support.microsoft.com/kb/q140584/
	#ifdef _MT
		#ifdef _DLL
			#ifdef _DEBUG
				/// For projects compiled with Multi-threaded Dynamic Debug C run-time (/MDd)
				#define DBOX_PLATFORM_CRT_SUFFIX "MDd"
			#else
				/// For projects compiled with Multi-threaded Dynamic Release C run-time (/MD)
				#define DBOX_PLATFORM_CRT_SUFFIX "MD"
			#endif
		#else
			#ifdef _DEBUG
				/// For projects compiled with Multi-threaded Static Debug C run-time (/MTd)
				#define DBOX_PLATFORM_CRT_SUFFIX "MTd"
			#else
				/// For projects compiled with Multi-threaded Static Release C run-time (/MT)
				#define DBOX_PLATFORM_CRT_SUFFIX "MT"
			#endif
		#endif
	#else
		// Windows single-threaded C run-time library not supported.
		#define DBOX_PLATFORM_CRT_SUFFIX "[UnsupportedCRT]"
	#endif
	/// Microsoft Visual Studio Version
	#if _MSC_VER >= 1920
		/// Visual Studio 2019 library
		#define DBOX_VISUAL_STUDIO_SUFFIX "-vc142"
	#elif _MSC_VER >= 1910
		/// Visual Studio 2017 library
		#define DBOX_VISUAL_STUDIO_SUFFIX "-vc141"
	#elif _MSC_VER >= 1900
		/// Visual Studio 2015 library
		#define DBOX_VISUAL_STUDIO_SUFFIX "-vc140"
	#else
		/// Default library for previous versions of Visual Studio
		#define DBOX_VISUAL_STUDIO_SUFFIX ""
	#endif
#else
	#define DBOX_PLATFORM_CRT_SUFFIX "[UnknownCRT]"
#endif

#define DBOX_PLATFORM_LIB_SUFFIX    DBOX_PLATFORM_CRT_SUFFIX DBOX_VISUAL_STUDIO_SUFFIX ".lib"

namespace dbox {
	//------------------------------------------------------------------------
	// Types definitions for platform isolation
	//------------------------------------------------------------------------
	typedef bool BOOL;

	typedef unsigned char U8;
	typedef signed char I8;

	typedef unsigned short U16;
	typedef signed short I16;

#if defined(_MSC_VER)
	typedef unsigned long U32;
	typedef signed long I32;
#elif defined(__GNUC__)
	typedef uint32_t U32;
	typedef int32_t I32;
#else
	typedef unsigned long U32;
	typedef signed long I32;
#endif

	// No 64-bit in ISO C++98 Standard, so it is compiler based
#if defined(_MSC_VER)
	typedef unsigned __int64 U64;
	typedef signed __int64 I64;
#elif defined(__GNUC__)
	typedef uint64_t U64;
	typedef int64_t I64;
#else
	typedef unsigned long long U64;
	typedef signed long long I64;
#endif

	typedef struct I128 {
		I64 Int[2]; // Int[0] is the low part.
	} I128;

	typedef float F32;

	typedef double F64;

	typedef void* PVOID;
	typedef const void* PCVOID;

	typedef char CHAR;
	typedef char* PCHAR;
	typedef const char* PCCHAR;

#if defined(_WCHAR_T) || defined(_WCHAR_T_DEFINED) || defined(_NATIVE_WCHAR_T_DEFINED)
	typedef wchar_t WCHAR;
	typedef wchar_t* PWCHAR;
	typedef const wchar_t* PCWCHAR;
#else
	typedef unsigned short WCHAR;
	typedef unsigned short* PWCHAR;
	typedef const unsigned short* PCWCHAR;
#endif
}

#endif  // _DBOX_PLATFORMTYPES_H_INCLUDED_
