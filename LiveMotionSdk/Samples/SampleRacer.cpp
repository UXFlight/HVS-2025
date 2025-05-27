#include <cstdio>
#include <conio.h>
#include "LiveMotion/dboxLiveMotion.h"
#include <Windows.h>

#pragma comment(lib, DBOX_LIVEMOTION_LIB)

// TODO: Set APP_KEY that will be unique to your D-BOX Integrated Project, no special characters except underscore.
#define APP_KEY         "SampleRacer"
// TODO: Set APP_BUILD to an integer representing your build number.
#define APP_BUILD       1001

// CONFIG for switching configurations (weapon, vehicle, level, track, etc.)
// TELEMETRY for information that gets updated at every frame (RPM, accelerations, rotations etc.)
// INFO for information received with an event trigger (picked up item, explosion intensity, etc.)

namespace {
	/// Sample configuration structure for CONFIG_UPDATE event.
	// TODO: Unused fields can be removed from the structure
	// and new relevant fields can be added to the structure.
	// However the STRUCTINFO section must ALWAYS be matched.
	struct RaceConfig {
	
		//Mandatory
		dbox::I32 EngineMaxRpm;	
		dbox::I32 EngineIdleRpm;	
		dbox::QuadFloat32 SuspensionMaxTravel;
	
		// Recommended
		dbox::I32 EngineCylinders;	
		dbox::I32 VehicleType;
	
		// Optional
		dbox::I32 EngineRedRpm;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(RaceConfig, EngineMaxRpm, dbox::FT_INT32, dbox::FM_ENGINE_RPM_MAX)	
		DBOX_STRUCTINFO_FIELD(RaceConfig, EngineIdleRpm, dbox::FT_INT32, dbox::FM_ENGINE_RPM_IDLE)
		DBOX_STRUCTINFO_FIELD(RaceConfig, SuspensionMaxTravel, dbox::FT_QUAD_FLOAT32, dbox::FM_SUSPENSION_TRAVEL_MAX_QUAD)
		DBOX_STRUCTINFO_FIELD(RaceConfig, EngineCylinders, dbox::FT_INT32, dbox::FM_ENGINE_CYLINDERS)	
		DBOX_STRUCTINFO_FIELD(RaceConfig, VehicleType, dbox::FT_INT32, dbox::FM_VEHICLE_TYPE)	
		DBOX_STRUCTINFO_FIELD(RaceConfig, EngineRedRpm, dbox::FT_INT32, dbox::FM_ENGINE_RPM_RED)
		DBOX_STRUCTINFO_END()
	};

	/// Sample real-time frame structure for FRAME_UPDATE event.
	// TODO: Unused fields can be removed from the structure
	// and new relevant fields can be added to the structure.
	// However the STRUCTINFO section must ALWAYS be matched.
	struct RaceTelemetry {
	
		// Mandatory
		dbox::XyzFloat32 Acceleration;	
		dbox::XyzFloat32 Velocity;
		dbox::I32 EngineRpm;
		dbox::QuadFloat32 SuspensionTravel;
		dbox::QuadFloat32 SurfaceTypeId;
		dbox::F32 PitchRad;
		dbox::F32 RollRad;		
	
		// Recommended
		dbox::XyzFloat32 AngularVelocity;  
		dbox::I32 TransmissionGear;
		dbox::QuadFloat32 TireSlipRatio;
		dbox::QuadFloat32 TireSlipAngleRadians;

		// Optional
		dbox::QuadFloat32 TireNormalizedLoad;
		dbox::QuadFloat32 TireRotationSpeed;
		dbox::QuadFloat32 TireSlipVelocity;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, Acceleration, dbox::FT_XYZ_FLOAT32, dbox::FM_ACCELERATION_XYZ)	
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, Velocity, dbox::FT_XYZ_FLOAT32, dbox::FM_VELOCITY_XYZ)
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, EngineRpm, dbox::FT_INT32, dbox::FM_ENGINE_RPM)
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, SuspensionTravel, dbox::FT_QUAD_FLOAT32, dbox::FM_SUSPENSION_TRAVEL_QUAD)
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, SurfaceTypeId, dbox::FT_QUAD_INT32, dbox::FM_SURFACE_TYPE_ID)
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, PitchRad, dbox::FT_FLOAT32, dbox::FM_PITCH_RAD)
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, RollRad, dbox::FT_FLOAT32, dbox::FM_ROLL_RAD)
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, AngularVelocity, dbox::FT_XYZ_FLOAT32, dbox::FM_ANGULAR_VELOCITY_XYZ)
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, TransmissionGear, dbox::FT_INT32, dbox::FM_ENGINE_TRANSMISSION_GEAR)
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, TireSlipRatio, dbox::FT_QUAD_FLOAT32, dbox::FM_TIRE_SLIP_RATIO_QUAD)
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, TireSlipAngleRadians, dbox::FT_QUAD_FLOAT32, dbox::FM_TIRE_SLIP_ANGLE_RAD_QUAD)
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, TireNormalizedLoad, dbox::FT_QUAD_FLOAT32, dbox::FM_TIRE_LOAD_NORM_QUAD)
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, TireRotationSpeed, dbox::FT_QUAD_FLOAT32, dbox::FM_TIRE_ROTATION_SPEED_QUAD)
		DBOX_STRUCTINFO_FIELD(RaceTelemetry, TireSlipVelocity, dbox::FT_QUAD_FLOAT32, dbox::FM_TIRE_SLIP_VELOCITY_QUAD)
		DBOX_STRUCTINFO_END()
	};

	/// Sample attributes structure for EVENT with information.
	// TODO: Unused fields can be removed from the structure
	// and new relevant fields can be added to the structure.
	// However the STRUCTINFO section must ALWAYS be matched.
	struct NitrousInfo {
		dbox::F32 Intensity;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(NitrousInfo, Intensity, dbox::FT_FLOAT32, dbox::FM_EVENT_INTENSITY)
		DBOX_STRUCTINFO_END()

		NitrousInfo(dbox::F32 dIntensity) :
			Intensity(dIntensity) {
		}
	};

	/// Sample attributes structure for EVENT with information.
	struct ImpactInfo {
		dbox::F32 Intensity;
		dbox::F32 Direction;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(ImpactInfo, Intensity, dbox::FT_FLOAT32, dbox::FM_EVENT_INTENSITY)
		DBOX_STRUCTINFO_FIELD(ImpactInfo, Direction, dbox::FT_FLOAT32, dbox::FM_EVENT_YAW_RAD)
		DBOX_STRUCTINFO_END()

		ImpactInfo(dbox::F32 dIntensity, dbox::F32 dDirection) :
			Intensity(dIntensity),
			Direction(dDirection) {
		}
	};

	/// These are your unique event ids that you'll use when calling PostEvent.
	/// You can use the numbers that are the most appropriate for your application.
	/// These values should be unique and not change in future releases.
	enum AppEvents {
		RACE_CONFIG = 1000,
		RACE_TELEMETRY,
		ENGINE_STARTED = 2000,
		ENGINE_STOPPED,
		NITRO_STARTED = 3000,
		NITRO_FINISHED,
		CAR_BANG = 4000,
	};
}

int main(int, char* []) {

	// Global initialization and registers must be called once at start-up. Initialize/Terminate calls must be balanced.
	dbox::LiveMotion::Initialize(APP_KEY, APP_BUILD);
	dbox::LiveMotion::RegisterEvent(RACE_CONFIG, dbox::EM_CONFIG_UPDATE, RaceConfig::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(RACE_TELEMETRY, dbox::EM_FRAME_UPDATE, RaceTelemetry::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(ENGINE_STARTED, dbox::EM_ENGINE_START);
	dbox::LiveMotion::RegisterEvent(ENGINE_STOPPED, dbox::EM_ENGINE_STOP);
	dbox::LiveMotion::RegisterEvent(NITRO_STARTED, dbox::EM_ENGINE_BOOST_START, NitrousInfo::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(NITRO_FINISHED, dbox::EM_ENGINE_BOOST_STOP);
	dbox::LiveMotion::RegisterEvent(CAR_BANG, dbox::EM_IMPACT, ImpactInfo::GetStructInfo());

	// Registration completed, open motion output device.
	// This will prepare the motion platform for output. Open/Close calls must be balanced.
	dbox::LiveMotion::Open();
	{
		// Race Start
		// It is always good to ResetState before each race.
		dbox::LiveMotion::ResetState();
		
		//------------  CONFIG ------------		
		// Configuration fields can be posted when required or when a change occurs.
		// In this case we are selecting a new vehicle. Their state is global and will be maintained 
		// by the Motion Code between updates. Even if a same CONFIGURATION FIELD is registered in 
		// different event structures, it will update the same global state in the Motion Code.	
		RaceConfig oConfig;
		oConfig.EngineCylinders = 4;
		oConfig.EngineIdleRpm = 1100;
		oConfig.EngineMaxRpm = 9000;
		oConfig.EngineRedRpm = 7500;
		oConfig.SuspensionMaxTravel.BackLeft = 6 * 0.0254f;
		oConfig.SuspensionMaxTravel.BackRight = 6 * 0.0254f;
		oConfig.SuspensionMaxTravel.FrontLeft = 6 * 0.0254f;
		oConfig.SuspensionMaxTravel.FrontRight = 6 * 0.0254f;		
		oConfig.VehicleType = 17;
		dbox::LiveMotion::PostEvent(RACE_CONFIG, oConfig);

		// for sample application only, do not sleep in real application
		Sleep(1000);
		//---------  END OF CONFIG ---------				

		// Gently starts motion activity based on PostEvent calls and internal state of Motion
        // Code logic. A fade-in will be applied for smooth transition. It should be called as
        // soon as the end - user can interact with the virtual environment, like start of
        // simulation/level, resume from pause, etc.
		dbox::LiveMotion::Start();
		
		//------------  FRAME ------------	
		// Telemetry fields should be posted at every simulation frame or game loop frame.
        // Their state is global and will be maintained by the Motion Code between frames.Even if
        // a same Telemetry field is registered in different event structures, it will update the same
        // global state in the Motion Code.
		RaceTelemetry oTelemetry;
		
		// First Frame
		oTelemetry.Acceleration.X = 0.4f;
		oTelemetry.Acceleration.Y = 0.2f;
		oTelemetry.Acceleration.Z = 1.0f;
		// Keep filling the oTelemetry Object, then send it with PostEvent() function.
		dbox::LiveMotion::PostEvent(RACE_TELEMETRY, oTelemetry);

		// Second Frame		
		oTelemetry.Acceleration.X = 0.8f;
		oTelemetry.Acceleration.Y = 0.1f;
		oTelemetry.Acceleration.Z = 2.7f;
		// Keep filling the oTelemetry Object, then send it with PostEvent() function.
		dbox::LiveMotion::PostEvent(RACE_TELEMETRY, oTelemetry);	
		
		// Engine started, this is a sample event with no attributes.
		dbox::LiveMotion::PostEvent(ENGINE_STARTED);

		// Nitro event, this is a sample event with level attribute.
		dbox::LiveMotion::PostEvent(NITRO_STARTED, NitrousInfo(0.75f));  // 75% intensity		
		// Nitro finished, this is a sample event with no attributes.
		dbox::LiveMotion::PostEvent(NITRO_FINISHED);	
	
		// Impact, this is a sample event with direction and level attributes.
		dbox::LiveMotion::PostEvent(CAR_BANG, ImpactInfo(0.4f, 1.57f));  // 40% intensity, 90 degrees.
		
		// for sample application only, do not sleep in real application
		Sleep(1000);
		
		// Gently stops motion activity. A fade-out will be applied for smooth transition. It
        // should be called as soon as the end - user can no longer interact with the virtual
        // environment, like end of simulation/level, pause, etc.
		dbox::LiveMotion::Stop();

		// Resume from pause, this will fade-in actual motion.
		dbox::LiveMotion::Start();
		
		// ​New Sim Frame, keep filling the telemetry struct with updated values
		dbox::LiveMotion::PostEvent(RACE_TELEMETRY, oTelemetry); 
	    // ​New Sim Frame	 	 
	    dbox::LiveMotion::PostEvent(RACE_TELEMETRY, oTelemetry);
	    // ​...
		
		// Engine cut off, this is a sample event with no attributes.
		dbox::LiveMotion::PostEvent(ENGINE_STOPPED);

		// End of level, this will fade-out motion.
		dbox::LiveMotion::Stop();

		// for sample application only, do not sleep in real application
		Sleep(1000);

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