#include <cstdio>
#include <conio.h>
#include <Windows.h>
#include "LiveMotion/dboxLiveMotion.h"

#pragma comment(lib, DBOX_LIVEMOTION_LIB)

// This sample demonstrates how to integrate the proper functions for Haptic feedback on a Linear
// clip that is built inside an Executable and not a video file, a cutscene for example.

// TODO: Set APP_KEY that will be unique to your D-BOX Integrated Project, no special characters except underscore.
#define APP_KEY         "SampleLinearClipExe"
// TODO: Set APP_BUILD to an integer representing your build number.
#define APP_BUILD       1001

namespace {
	// ------------------------cinemotion player structures-------------------------
	// These structs are for the base layer of Motion, the aspect of your experience where movements, events and actions
	// are linear, so always the same everytime the experience is launched
	struct ID_CineMotionBase {
		dbox::I32 FileIndex;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(ID_CineMotionBase, FileIndex, dbox::FT_INT32, dbox::FM_CINEMOTION_CONTENT_ID)
		DBOX_STRUCTINFO_END()

		ID_CineMotionBase(dbox::I32 nFileIndex) :
			FileIndex(nFileIndex) {
		}
	};

	struct MotionBaseTimeCode {
		dbox::F64 PlaybackTimeSec;
		dbox::F64 MatchingCpuTimeSec;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(MotionBaseTimeCode, PlaybackTimeSec, dbox::FT_FLOAT64, dbox::FM_CINEMOTION_PLAYBACK_TIME_SEC)
		DBOX_STRUCTINFO_FIELD(MotionBaseTimeCode, MatchingCpuTimeSec, dbox::FT_FLOAT64, dbox::FM_CINEMOTION_MATCHING_CPU_TIME_SEC)
		DBOX_STRUCTINFO_END()

		MotionBaseTimeCode(dbox::F64 dPlaybackTimeSec, dbox::F64 dMatchingCpuTimeSec) :
			PlaybackTimeSec(dPlaybackTimeSec), MatchingCpuTimeSec(dMatchingCpuTimeSec)  {
		}
	};

	// If your experience has user interactions and a small pool of effects can be called
	// Ex: Base track of a moving vehicle that is linear (always the same duration, path, animations)
	// User can shoot enemies as he rides, those shots are Dynamic Events
	struct ID_PreloadFile {
		dbox::I32 Index;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(ID_PreloadFile, Index, dbox::FT_INT32, dbox::FM_CINEMOTION_CONTENT_ID)
		DBOX_STRUCTINFO_END()
	};

	struct ID_DynamicEvent {
		dbox::F32 GainDb;
		dbox::I32 Index;
		dbox::I32 IsLoop;
		dbox::I32 StoppableGroupId;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(ID_DynamicEvent, GainDb, dbox::FT_FLOAT32, dbox::FM_CINEMOTION_GAIN_DB)
		DBOX_STRUCTINFO_FIELD(ID_DynamicEvent, Index, dbox::FT_INT32, dbox::FM_CINEMOTION_CONTENT_ID)
		DBOX_STRUCTINFO_FIELD(ID_DynamicEvent, IsLoop, dbox::FT_INT32, dbox::FM_CINEMOTION_LOOP)
		DBOX_STRUCTINFO_FIELD(ID_DynamicEvent, StoppableGroupId, dbox::FT_INT32, dbox::FM_CINEMOTION_STOPPABLE_GROUP_ID)
		DBOX_STRUCTINFO_END()

		ID_DynamicEvent(dbox::F32 dGainDb, dbox::I32 nIndex, dbox::I32 bIsLoop, dbox::I32 nStoppableGroupId) :
			GainDb(dGainDb),
			Index(nIndex),
			IsLoop(bIsLoop),
			StoppableGroupId(nStoppableGroupId) {
		}
	};

	struct StopDynamicEvent {
		dbox::I32 StoppableGroupId;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(StopDynamicEvent, StoppableGroupId, dbox::FT_INT32, dbox::FM_CINEMOTION_STOPPABLE_GROUP_ID)
		DBOX_STRUCTINFO_END()

		StopDynamicEvent(dbox::I32 nStoppableGroupId) :
			StoppableGroupId(nStoppableGroupId) {
		}
	};

	/// These are your unique event ids that you'll use when calling PostEvent.
	/// You can use the numbers that are the most appropriate for your application.
	/// These values should be unique and not change in future releases.
	enum AppEvents {
		// Base Layer Events
		ID_CINEMOTION_START = 1000,
		CINEMOTION_STOP = 1001,
		CINEMOTION_TIMECODE = 2000,

		// Short Dynamic Events
		ID_DYNAMIC_EVENT_PRELOAD_FILE = 3000,
		ID_DYNAMIC_EVENT_START = 4000,
		DYNAMIC_EVENT_STOP = 4001,
	};

	double g_dTickFactor = 0;
	LARGE_INTEGER g_nCurrentTick;

	// Function used to get time
	double GetCpuTimeS() {
		QueryPerformanceCounter(&g_nCurrentTick);
		return static_cast<double>(g_nCurrentTick.QuadPart) * g_dTickFactor;
	}
}

int main(int, char* []) {

	// This can be retrieved only once per application run-time as this is based off current CPU.
	LARGE_INTEGER nTickFreq;
	QueryPerformanceFrequency(&nTickFreq);
	if (0 != nTickFreq.QuadPart) {
		// Pre-calculate the inverse of tick frequency to do multiplications instead
		// of divisions in GetCpuTimeS which will be called a lot.
		g_dTickFactor = 1 / static_cast<double>(nTickFreq.QuadPart);
	} else {
		// Error...
	}

	// Global initialization and registers must be called once at start-up. Initialize/Terminate calls must be balanced.
	dbox::LiveMotion::Initialize(APP_KEY, APP_BUILD);

	dbox::LiveMotion::RegisterEvent(ID_CINEMOTION_START, dbox::EM_CINEMOTION_START, ID_CineMotionBase::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(CINEMOTION_STOP, dbox::EM_CINEMOTION_STOP);
	dbox::LiveMotion::RegisterEvent(CINEMOTION_TIMECODE, dbox::EM_CINEMOTION_UPDATE, MotionBaseTimeCode::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(ID_DYNAMIC_EVENT_PRELOAD_FILE, dbox::EM_CINEMOTION_PRELOAD_FILE,ID_PreloadFile::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(ID_DYNAMIC_EVENT_START, dbox::EM_CINEMOTION_START, ID_DynamicEvent::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(DYNAMIC_EVENT_STOP, dbox::EM_CINEMOTION_STOP, StopDynamicEvent::GetStructInfo());

	// for sample application only, do not sleep in real application
	Sleep(1000);

	// Registration completed, open motion output device.
	// This will prepare the motion platform for output. Open/Close calls must be balanced.
	dbox::LiveMotion::Open(); {

		const dbox::I32 GUN_GROUP_ID = 1;

		// It is always good to ResetState before each start.
		dbox::LiveMotion::ResetState();

		// Start level, this will fade-in actual motion.
		dbox::LiveMotion::Start();
		double dTimeCode = 0.0;
		
		// Preload files that will get called dynamically on user input
		// Give time for file to preload before calling start on it
		// Indexes are very short Motion impulses (GUN_GROUP)
		ID_PreloadFile oPreloadFile;
		oPreloadFile.Index = 2;
		dbox::LiveMotion::PostEvent(ID_DYNAMIC_EVENT_PRELOAD_FILE, oPreloadFile);

		// for sample application only, do not sleep in real application
		Sleep(500);

		oPreloadFile.Index = 3;
		dbox::LiveMotion::PostEvent(ID_DYNAMIC_EVENT_PRELOAD_FILE, oPreloadFile);
		
		// for sample application only, do not sleep in real application
		Sleep(500);

		oPreloadFile.Index = 4;
		dbox::LiveMotion::PostEvent(ID_DYNAMIC_EVENT_PRELOAD_FILE, oPreloadFile);
		
		// for sample application only, do not sleep in real application
		Sleep(500);

		// Play file ID #1 which is the Base Layer Motion Track
		int motionBaseLayerID = 1;
		dbox::LiveMotion::PostEvent(ID_CINEMOTION_START, ID_CineMotionBase(motionBaseLayerID));

		// for sample application only, do not sleep in real application
		Sleep(1000);

		// Post Time Code updates at every frame
		dbox::LiveMotion::PostEvent(CINEMOTION_TIMECODE, MotionBaseTimeCode(dTimeCode, GetCpuTimeS()));
		dTimeCode++;
		// New frame
		dbox::LiveMotion::PostEvent(CINEMOTION_TIMECODE, MotionBaseTimeCode(dTimeCode, GetCpuTimeS()));
		dTimeCode++;
		// ...

		for (int i = 0; i < 100; i++) {
			dbox::LiveMotion::PostEvent(CINEMOTION_TIMECODE, MotionBaseTimeCode(dTimeCode, GetCpuTimeS()));
			dTimeCode++;
		}

		// for sample application only, do not sleep in real application
		Sleep(1000);

		// User fires his weapon
		dbox::LiveMotion::PostEvent(ID_DYNAMIC_EVENT_START, ID_DynamicEvent(/*GainDb*/ 15.0f,
		/*index*/ 2, /*isLoop*/ 0, /*stoppable group id*/ GUN_GROUP_ID));

		// New frame
		dbox::LiveMotion::PostEvent(CINEMOTION_TIMECODE, MotionBaseTimeCode(dTimeCode, GetCpuTimeS()));
		dTimeCode++;
		// ...

		// User fires a different weapons
		dbox::LiveMotion::PostEvent(ID_DYNAMIC_EVENT_START, ID_DynamicEvent(/*GainDb*/ 10.0f,
		/*index*/ 3, /*isLoop*/ 0, /*stoppable group id*/ GUN_GROUP_ID));

		// Stop Motion Base Layer so file ID #1
		dbox::LiveMotion::PostEvent(CINEMOTION_STOP);

		// To Stop Dynamic Events
		dbox::LiveMotion::PostEvent(DYNAMIC_EVENT_STOP, StopDynamicEvent(GUN_GROUP_ID));

		// for sample application only, do not sleep in real application
		Sleep(1000);

		// Level End
		dbox::LiveMotion::Stop();
	}
	// Closes the motion output device. This will normally release the motion platform and
    // immediately stop any pending output.
	dbox::LiveMotion::Close();
	// Global termination. It must be called at the end and it is very important to do so.
	dbox::LiveMotion::Terminate();

	// for sample application only, do not sleep in real application
	Sleep(5000);

	printf("\nEnded, press any key...\n");
	_getch();

	return 0;
}