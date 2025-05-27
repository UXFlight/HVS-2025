#pragma once
#ifndef _DBOX_LIVEMOTIONTYPES_H_INCLUDED_
#define _DBOX_LIVEMOTIONTYPES_H_INCLUDED_
//------------------------------------------------------------------------
// D-BOX LIVE MOTION API
// Copyright (C) 2008, D-BOX Technologies Inc.
// All rights reserved.
//------------------------------------------------------------------------
#include "dboxPlatformTypes.h"
#include "dboxEEventMeaning.h"
#include "dboxEFieldMeaning.h"

#pragma pack(push, 8)   // Guarantees proper alignment of structures for native types members up to 8 bytes.

namespace dbox {

	//------------------------------------------------------------------------
	/// Live Motion API return values.
	//------------------------------------------------------------------------
	enum ELiveMotionResult {
		LMR_SUCCESS = 0,                        //< Operation successful.
		LMR_ERROR = -1,                         //< Unknown error.
		LMR_ERROR_NOT_INITIALIZED = -2,         //< Error when calling a method that requires Initialize.
		LMR_ERROR_NOT_OPENED = -3,              //< Error when calling a method that requires Open.
		LMR_ERROR_NOT_STARTED = -4,             //< Error when calling a method that requires Start.
		LMR_ERROR_REGISTER_LOCKED = -5,         //< Once Open is called, Register cannot be called again.
		LMR_ERROR_NO_EVENT_HANDLER = -6,        //< Initialize did not find an appropriate IEventHandler.
		LMR_ERROR_DLL_NOT_FOUND = -7,           //< LoadLibrary failed.
		LMR_ERROR_DLL_ENTRY_NOT_FOUND = -8,     //< GetProcAddress failed.
		LMR_ERROR_DLL_DISPOSE = -9,             //< FreeLibrary failed.
		LMR_ERROR_NO_DEVICE_FOUND = 10,         //< No motion device found.
	};

	//------------------------------------------------------------------------
	/// This enum is used to define the type of a field in an event struct.
	//------------------------------------------------------------------------
	enum EFieldType {
		/// Invalid or undefined field type.
		FT_INVALID          = 0x00,

		//------------------------------------------------------------------------
		// NUMERIC TYPES
		//------------------------------------------------------------------------

		/// 32-bit signed integer numeric field (long).
		FT_INT32            = 0x17,

		/// 64-bit signed integer numeric field (long long).
		FT_INT64            = 0x18,

		/// 32-bit floating point decimal field (float).
		FT_FLOAT32          = 0x19,

		/// 64-bit floating point decimal field (double).
		FT_FLOAT64          = 0x1a,

		//------------------------------------------------------------------------
		// STRING TYPES
		//------------------------------------------------------------------------

		/// 32-bit pointer to null terminated string field (const char *).
		FT_CHAR_PTR32       = 0x60,

		/// 32-bit pointer to null terminated UNICODE string field (const wchar_t *).
		//FT_WCHAR_PTR32 SHOULD NOT BE USED FOR SDK INTEGRATION, PLEASE USE FT_CHAR_PTR32 IF REQUIRED
		FT_WCHAR_PTR32      = 0x61,

		//------------------------------------------------------------------------
		// CUSTOM TYPES
		//------------------------------------------------------------------------

		/// 3 x 32-bit floating point structure.  See XyzFloat32 for member definitions.
		FT_XYZ_FLOAT32      = 0x89,

		/// 3 x 64-bit floating point structure.  See XyzFloat64 for member definitions.
		FT_XYZ_FLOAT64      = 0x8a,

		/// 4 x 32-bit signed integer structure.  See QuadInt32 for member definitions.
		FT_QUAD_INT32       = 0x97,

		/// 4 x 32-bit floating point structure.  See QuadFloat32 for member definitions.
		FT_QUAD_FLOAT32     = 0x99,

		/// 4 x 64-bit floating point structure.  See QuadFloat64 for member definitions.
		FT_QUAD_FLOAT64     = 0x9a,

	};

	//------------------------------------------------------------------------
	/// Spare
	//------------------------------------------------------------------------
	enum EFieldFlag {
		/// Normal field.
		FF_NORMAL = 0
	};

	//------------------------------------------------------------------------
	/// 6 bytes field definition structure (2 byte alignment).
	//------------------------------------------------------------------------
	struct FieldDef {
		U8 FieldType;                   //< Field value type (1 byte).
		U8 FieldFlags;                  //< Field flags, spare (1 byte).
		U16 FieldMeaning;               //< D-BOX field meaning (2 bytes).
		U16 FieldOffset;                //< Field byte offset in structure (2 bytes).
	};

	//------------------------------------------------------------------------
	/// Structure used by D-BOX LiveMotion API to submit list
	/// of FieldDef defining structure layout registration.
	//------------------------------------------------------------------------
	struct StructInfo {
		FieldDef* StaticFields;         //< Pointer to static field definition structure (4 bytes).
		U16 FieldCount;                 //< Number of fields in event (2 bytes, last to prevent padding).
	};

	//------------------------------------------------------------------------
	/// Helper template structures for XYZ fields.
	/// This represents a left-handed 3D vector.
	/// See individual fields for axis meaning.
	//------------------------------------------------------------------------
	template <class TNumeric>
	struct Xyz {
		/// Positive toward right-hand side (or East), negative toward left-hand side (or West).
		TNumeric X;
		/// Positive toward up or head (or Sky), negative toward down or feet (or Ground).
		TNumeric Y;
		/// Positive toward front or looking direction (or North), negative toward back (or South)
		TNumeric Z;

		Xyz() :
			X(TNumeric(0)),
			Y(TNumeric(0)),
			Z(TNumeric(0)) {
		}

		Xyz(TNumeric dX, TNumeric dY, TNumeric dZ) :
			X(dX),
			Y(dY),
			Z(dZ) {
		}
	};

	/// 3 x 32-bit floating point structure.  See Xyz template for member definitions.
	typedef Xyz<F32> XyzFloat32;
	/// 3 x 64-bit floating point structure.  See Xyz template for member definitions.
	typedef Xyz<F64> XyzFloat64;

	//------------------------------------------------------------------------
	/// Helper template structures for QUAD fields.
	/// This represents a four corners structure (like the wheels of a vehicle).
	/// See individual fields for meaning.
	//------------------------------------------------------------------------
	template <class TNumeric>
	struct Quad {
		/// Numeric value related to front-left corner.
		TNumeric FrontLeft;
		/// Numeric value related to front-right corner.
		TNumeric FrontRight;
		/// Numeric value related to back-left corner.
		TNumeric BackLeft;
		/// Numeric value related to back-right corner.
		TNumeric BackRight;

		Quad() :
			FrontLeft(TNumeric(0)),
			FrontRight(TNumeric(0)),
			BackLeft(TNumeric(0)),
			BackRight(TNumeric(0)) {
		}

		Quad(TNumeric dFrontLeft, TNumeric dFrontRight, TNumeric dBackLeft, TNumeric dBackRight) :
			FrontLeft(dFrontLeft),
			FrontRight(dFrontRight),
			BackLeft(dBackLeft),
			BackRight(dBackRight) {
		}

	};

	/// 4 x 32-bit signed integer structure.  See Quad template for member definitions.
	typedef Quad<I32> QuadInt32;
	/// 4 x 32-bit floating point structure.  See Quad template for member definitions..
	typedef Quad<F32> QuadFloat32;
	/// 4 x 64-bit floating point structure.  See Quad template for member definitions..
	typedef Quad<F64> QuadFloat64;
}

#pragma pack(pop)       // Restores user struct packing.

#endif  // _DBOX_LIVEMOTIONTYPES_H_INCLUDED_