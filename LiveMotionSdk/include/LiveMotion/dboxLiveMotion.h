#pragma once
#ifndef _DBOX_LIVEMOTION_H_INCLUDED_
#define _DBOX_LIVEMOTION_H_INCLUDED_
//------------------------------------------------------------------------
// D-BOX LIVE MOTION API
// Copyright (C) 2008, D-BOX Technologies Inc.
// All rights reserved.
//------------------------------------------------------------------------
#include "dboxLiveMotionTypes.h"
#include <cstddef>                          //< For offsetof().

//------------------------------------------------------------------------
/// D-BOX LIVE MOTION API MACROS AND DEFINES
//------------------------------------------------------------------------

/// Library selection macro
#ifdef _M_X64
	#define DBOX_LIVEMOTION_LIB     "dbxLive64" DBOX_PLATFORM_LIB_SUFFIX
#else
	#define DBOX_LIVEMOTION_LIB     "dbxLive32" DBOX_PLATFORM_LIB_SUFFIX
#endif

// D-BOX LiveMotion API Calling Convention
#if defined(_MSC_VER)
	#define DBOX_LIVEMOTION_CC __cdecl
#else
	#define DBOX_LIVEMOTION_CC
#endif

// D-BOX Motion API Call Modifier
#define DBOX_LIVEMOTION_API extern "C"

#ifdef DBOX_LIVEMOTION_EXPORTS  /* DLL exports */
	#define DBOX_LIVEMOTION_API_DLL DBOX_LIVEMOTION_API DBOX_API_EXPORT
#else
	#define DBOX_LIVEMOTION_API_DLL DBOX_LIVEMOTION_API DBOX_API_IMPORT
#endif


/// Converts a 2 part version to U32 AppBuild
#define DBOX_APP_BUILD_FROM_2_PART(HighShort, LowShort) \
	(((static_cast<dbox::U32>(HighShort) << 16) & 0xffff0000) | \
	 (static_cast<dbox::U32>(LowShort) & 0x0000ffff))

/// Converts a 3 part version to U32 AppBuild
#define DBOX_APP_BUILD_FROM_3_PART(MajorByte, MinorByte, BuildShort) \
	(((static_cast<dbox::U32>(MajorByte) << 24) & 0xff000000) | \
	 ((static_cast<dbox::U32>(MinorByte) << 16) & 0x00ff0000) | \
	 (static_cast<dbox::U32>(BuildShort) & 0x0000ffff))

/// Converts a 4 part version to U32 AppBuild
#define DBOX_APP_BUILD_FROM_4_PART(MajorByte, MinorByte, RevisionByte, BuildByte) \
	(((static_cast<dbox::U32>(MajorByte) << 24) & 0xff000000) | \
	 ((static_cast<dbox::U32>(MinorByte) << 16) & 0x00ff0000) | \
	 ((static_cast<dbox::U32>(RevisionByte) << 8) & 0x0000ff00) | \
	 (static_cast<dbox::U32>(BuildByte) & 0x000000ff))

//------------------------------------------------------------------------
/// STRUCTURE LAYOUT REGISTRATION MACROS.
/// Use these macros to define the fields layout, type and meaning in a struct.
/// They will create a static GetStructInfo() method in your struct
/// suitable for registration with the RegisterEvent method.  Later on,
/// instance of registered struct can be supplied to the PostEvent method.
/// Sample usage: (note that there is no ";" at end of macro calls)
///
///         struct SampleConfig {
///             int EngineMaxRpm;
///             float EngineMaxPower;
///
///             DBOX_STRUCTINFO_BEGIN()
///                 DBOX_STRUCTINFO_FIELD(SampleConfig, EngineMaxRpm, dbox::FT_INT32, dbox::FM_ENGINE_RPM_MAX)
///                 DBOX_STRUCTINFO_FIELD(SampleConfig, EngineMaxPower, dbox::FT_FLOAT32, dbox::FM_ENGINE_POWER_MAX)
///             DBOX_STRUCTINFO_END()
///         };
///
/// Then SampleConfig::GetStructInfo() can be supplied to RegisterEvent method.
/// WARNING: Your StructInfo layout must match exactly the layout of your struct.
/// These macros facilitate this because they can be both maintained close to
/// each other in your source code.
//------------------------------------------------------------------------
#define DBOX_STRUCTINFO_BEGIN() \
	static dbox::StructInfo GetStructInfo() {   \
		static dbox::FieldDef g_aFieldDefs[] = {

#define DBOX_STRUCTINFO_FIELD(StructName, FieldName, eFieldType, eFieldMeaning) \
	{ dbox::U8(eFieldType), dbox::U8(dbox::FF_NORMAL), dbox::U16(eFieldMeaning), offsetof(StructName, FieldName)},

#define DBOX_STRUCTINFO_END()   \
	};  \
	dbox::StructInfo oStructInfo = {g_aFieldDefs, dbox::U16(sizeof(g_aFieldDefs) / sizeof(dbox::FieldDef))};    \
	return oStructInfo; \
	}

namespace dbox {

	//------------------------------------------------------------------------
	/// D-BOX LIVE MOTION API STATIC METHODS
	//------------------------------------------------------------------------
	class LiveMotion {
	private:
		LiveMotion();
	public:

		/// Global API initialization.  It should be called once at startup.
		/// Initialize/Terminate calls should be balanced.
		/// Parameter:
		/// - sAppKey: should be set to a constant char * that uniquely
		///   identifies your application (usually assigned by D-BOX).
		/// - nAppBuildVersion: should be set to an integer representing
		///   your build number.  This field may be used by D-BOX to track
		///   version changes or updates in your application.
		static ELiveMotionResult DBOX_LIVEMOTION_CC Initialize(PCCHAR sAppKey, U32 nAppBuildVersion);

		/// Global API termination.  It should be called at end.
		/// Initialize/Terminate calls should be balanced.
		static ELiveMotionResult DBOX_LIVEMOTION_CC Terminate();

		/// Opens the motion output device.  This will normally prepare
		/// the motion platform for output.
		/// Open/Close calls should be balanced.
		/// Parameter:
		/// - nSessionCount: number of sessions requested to be opened.
		///   For applications supporting 1 motion output, default value of 1
		///   should be used. For application supporting multiple different
		///   motion outputs, this parameter should be set to the supported
		///   number of motion outputs.
		static ELiveMotionResult DBOX_LIVEMOTION_CC Open(U8 nSessionCount = 1);

		/// Closes the motion output device.  This will normally release
		/// the motion platform and immediately stop any pending output.
		/// Open/Close calls should be balanced.
		static ELiveMotionResult DBOX_LIVEMOTION_CC Close();

		/// Gently starts motion activity based on PostEvent calls and internal
		/// state of motion code logic.  A fade-in will be applied for smooth
		/// transition.  It should be called as soon as the end-user can interact
		/// with the virtual environment, like start of simulation/level, resume from pause, etc.
		static ELiveMotionResult DBOX_LIVEMOTION_CC Start();

		/// Gently stops motion activity.  A fade-out will be applied for smooth
		/// transition.  It should be called as soon as the end-user can no longer
		/// interact with the virtual environment, like end of simulation/level, pause, etc.
		static ELiveMotionResult DBOX_LIVEMOTION_CC Stop();

		/// Resets the internal state of the motion code logic.  It should normally be called
		/// before Start or after Stop (except for pauses) when the environment or context changed.
		/// If IsStarted is true, then this equivalent sequence will be executed: Stop, ResetState, Start.
		static ELiveMotionResult DBOX_LIVEMOTION_CC ResetState();

		/// Registers an event or message with related data or attributes.  Events are used
		/// to communicate configuration or real-time data that can be used for motion generation.
		/// You can combine many data fields in a structure and post them at once in a single event.
		/// To do so you must create a structure and use the DBOX_STRUCTINFO_* macros to create
		/// structure layout information that will be used by this RegisterEvent method.
		/// This way, the D-BOX motion code logic will be able to interpret your structure data
		/// on subsequent PostEvent calls.
		/// To post these events you will need to use the 2-parameters PostEvent method.
		/// Parameters:
		/// - nPermanentEventKey: This is your unique reference for later posting this event.
		///   Once a Permanent Event Key is used in motion generation, it must not
		///   change with future releases of your application to prevent compatibility
		///   breaks.
		/// - eMeaning: This is the general meaning of your event.
		/// - oGetStructInfo: For a structure with appropriate DBOX_STRUCTINFO_* layout
		///   use [your struct name]::GetStructInfo().
		static ELiveMotionResult DBOX_LIVEMOTION_CC RegisterEvent(U32 nPermanentEventKey, EEventMeaning eMeaning, const StructInfo& oGetStructInfo);

		/// Registers a simple event or trigger without related data or attribute.
		/// To post these events you will need to use the 1-parameter PostEvent method.
		static ELiveMotionResult DBOX_LIVEMOTION_CC RegisterEvent(U32 nPermanentEventKey, EEventMeaning eMeaning);

		/// Sends an event and a related data structure from your application to all sessions.
		/// You must supply your registered Permanent Event Key and a related
		/// structure instance reference.
		template <class T>
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEvent(U32 nPermanentEventKey, const T& oData) {
			return PostEvent(nPermanentEventKey, &oData, sizeof(T));
		};
		
		/// Sends an event and a related data structure from your application to all sessions.
		/// You must supply your registered Permanent Event Key, a related structure instance pointer and
		/// the size of the structure.
		/// NOTE: This function should only be used when PostEvent (template) cannot be used.
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEventUnsafe(U32 nPermanentEventKey, PCVOID pData, size_t nSize) {
			return PostEvent(nPermanentEventKey, pData, nSize);
		};

		/// Sends an event from your application to all sessions, without related data or attributes.
		/// This is useful for small events using only your registered Permanent Event Key.
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEvent(U32 nPermanentEventKey);

		/// Sends an event and a related data structure from your application to a specific session.
		/// You must supply your registered Permanent Event Key and a related
		/// structure instance reference.
		template <class T>
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEventToSession(U8 nTargetSession, U32 nPermanentEventKey, const T& oData) {
			return PostEventToSession(nTargetSession, nPermanentEventKey, &oData, sizeof(T));
		};

		/// Sends an event and a related data structure from your application to a specific session.
		/// You must supply your registered Permanent Event Key, a related structure instance pointer and
		/// the size of the structure.
		/// NOTE: This function should only be used when PostEventToSession (template) cannot be used.
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEventToSessionUnsafe(U8 nTargetSession, U32 nPermanentEventKey, PCVOID pData, size_t nSize) {
			return PostEventToSession(nTargetSession, nPermanentEventKey, pData, nSize);
		}

		/// Sends an event from your application to a specific session, without related data or attributes.
		/// This is useful for small events using only your registered Permanent Event Key.
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEventToSession(U8 nTargetSession, U32 nPermanentEventKey);

		/// Sends continuous events and a related data structure from your application to all sessions.
		/// You must supply your registered Permanent Event Key and a related structure instance reference with its count.
		/// nBufferedCount is an output parameter that returns the current count of events in the buffer after adding the
		/// events from this call. Calling this function without events (pData = NULL, nDataCount = 0) will allow the retrieval
		/// of the current value in nBufferedCount.
		/// NOTE: This function should only be used for a streaming application, as identified by D-BOX.
		template <class T>
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEvents(U32 nPermanentEventKey, const T* pData, U32 nDataCount, U32& nBufferedCount) {
			return PostEvents(nPermanentEventKey, pData, sizeof(T), nDataCount, nBufferedCount);
		};

		/// Sends continuous events and a related data structure from your application to all sessions.
		/// You must supply your registered Permanent Event Key and a related structure instance reference with its count.
		/// nBufferedCount is an output parameter that returns the current count of events in the buffer after adding the
		/// events from this call. Calling this function without events (pData = NULL, nDataCount = 0) will allow the retrieval
		/// of the current value in nBufferedCount.
		/// NOTE: This function should only be used for a streaming application, as identified by D-BOX.
		///       This function should only be used when PostEvents (template) cannot be used.
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEventsUnsafe(U32 nPermanentEventKey, PCVOID pData, size_t nSize, U32 nDataCount, U32& nBufferedCount) {
			return PostEvents(nPermanentEventKey, pData, nSize, nDataCount, nBufferedCount);
		}

		/// Sends continuous events and a related data structure from your application to a specific session.
		/// You must supply your registered Permanent Event Key and a related structure instance reference with its count.
		/// nBufferedCount is an output parameter that returns the current count of events in the buffer after adding the
		/// events from this call. Calling this function without events (pData = NULL, nDataCount = 0) will allow the retrieval
		/// of the current value in nBufferedCount.
		/// NOTE: This function should only be used for a streaming application, as identified by D-BOX.
		template <class T>
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEventsToSession(U8 nTargetSession, U32 nPermanentEventKey, const T* pData, U32 nDataCount, U32& nBufferedCount) {
			return PostEventsToSession(nTargetSession, nPermanentEventKey, pData, sizeof(T), nDataCount, nBufferedCount);
		};

		/// Sends continuous events and a related data structure from your application to a specific session.
		/// You must supply your registered Permanent Event Key and a related structure instance reference with its count.
		/// nBufferedCount is an output parameter that returns the current count of events in the buffer after adding the
		/// events from this call. Calling this function without events (pData = NULL, nDataCount = 0) will allow the retrieval
		/// of the current value in nBufferedCount.
		/// NOTE: This function should only be used for a streaming application, as identified by D-BOX.
		///       This function should only be used when PostEventsToSession (template) cannot be used.
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEventsToSessionUnsafe(U8 nTargetSession, U32 nPermanentEventKey, PCVOID pData, size_t nSize, U32 nDataCount, U32& nBufferedCount) {
			return PostEventsToSession(nTargetSession, nPermanentEventKey, pData, nSize, nDataCount, nBufferedCount);
		}

		/// Returns true if the API is successfully initialized.
		static BOOL DBOX_LIVEMOTION_CC IsInitialized();

		/// Returns true if the API is initialized and successfully opened.
		static BOOL DBOX_LIVEMOTION_CC IsOpened();

		/// Returns true if the API is initialized, opened and started.
		static BOOL DBOX_LIVEMOTION_CC IsStarted();

	private:
		/// The size in parameter should match the size registered.
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEvent(U32 nPermanentEventKey, PCVOID pData, size_t nSize);
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEventToSession(U8 nTargetSession, U32 nPermanentEventKey, PCVOID pData, size_t nSize);
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEvents(U32 nPermanentEventKey, PCVOID pData, size_t nSize, U32 nCount, U32& nBufferedCount);
		static ELiveMotionResult DBOX_LIVEMOTION_CC PostEventsToSession(U8 nTargetSession, U32 nPermanentEventKey, PCVOID pData, size_t nSize, U32 nCount, U32& nBufferedCount);
	};
}

#endif  // _DBOX_LIVEMOTION_H_INCLUDED_
