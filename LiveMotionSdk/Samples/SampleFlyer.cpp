#include <cstdio>
#include <conio.h>
#include "LiveMotion/dboxLiveMotion.h"
#include <Windows.h>

#pragma comment(lib, DBOX_LIVEMOTION_LIB)

// TODO: Set APP_KEY that will be unique to your D-BOX Integrated Project, no special characters except underscore.
#define APP_KEY         "SampleFlyer"
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
	struct FlightConfig {	
	
		// Mandatory
		dbox::F32 CriticalAoa;
		dbox::F32 VelocityVs0;
		dbox::I32 EngineCount;	

		// Recommended
		dbox::I32 AircraftType;

		// Optional
		dbox::F32 VelocityVfe;
		dbox::F32 VelocityVs;	
		dbox::F32 WingSpan;

		DBOX_STRUCTINFO_BEGIN()	
		DBOX_STRUCTINFO_FIELD(FlightConfig, CriticalAoa, dbox::FT_FLOAT32, dbox::FM_AIRCRAFT_CRITICAL_AOA_DEG)
		DBOX_STRUCTINFO_FIELD(FlightConfig, VelocityVs0, dbox::FT_FLOAT32, dbox::FM_AIRCRAFT_VS0_KT)
		DBOX_STRUCTINFO_FIELD(FlightConfig, EngineCount, dbox::FT_INT32, dbox::FM_ENGINE_COUNT)
		DBOX_STRUCTINFO_FIELD(FlightConfig, AircraftType, dbox::FT_INT32, dbox::FM_VEHICLE_TYPE)
		DBOX_STRUCTINFO_FIELD(FlightConfig, VelocityVfe, dbox::FT_FLOAT32, dbox::FM_AIRCRAFT_VFE_KT)
		DBOX_STRUCTINFO_FIELD(FlightConfig, VelocityVs, dbox::FT_FLOAT32, dbox::FM_AIRCRAFT_VS_KT)
		DBOX_STRUCTINFO_FIELD(FlightConfig, WingSpan, dbox::FT_FLOAT32, dbox::FM_AIRCRAFT_WINGSPAN_FT)
		DBOX_STRUCTINFO_END()
	};

	/// Sample real-time frame structure for FRAME_UPDATE event.
	// TODO: Unused fields can be removed from the structure
	// and new relevant fields can be added to the structure.
	// However the STRUCTINFO section must ALWAYS be matched.
	struct FlightTelemetry {
	
		// Mandatory	
		dbox::XyzFloat32	Acceleration;          // Acceleration vector with origin at pilot position and same referential as pilot orientation
		dbox::XyzFloat32	AngularAcceleration;   // Angular Acceleration values with origin at pilot position and same referential as pilot orientation
		dbox::F32 PitchRad;
		dbox::F32 RollRad;	
		dbox::F32 Engine1N1;
		dbox::F32 Rotor1Rpm;						// For helicopter
		dbox::F32 AngleOfAttack;
		dbox::I32 IsOnGround;
		dbox::F32 SpoilerLeftDeployment;
		dbox::F32 SpoilerRightDeployment;
		dbox::F32 FlapLeftDeployment;
		dbox::F32 FlapRightDeployment;
	
		//Recommended
		dbox::XyzFloat32	Velocity;   
		dbox::F32 Engine2N1;			// If Multi Engine
		dbox::F32 Engine3N1;			// If Multi Engine
		dbox::F32 Engine4N1;			// If Multi Engine

		dbox::F32 Rotor2Rpm;			// Important for helicopter
		dbox::F32 Rotor3Rpm;			// Important for helicopter
		dbox::F32 Rotor4Rpm;			// Important for helicopter
		dbox::F32 HeadingRad;

		//Optional
		dbox::XyzFloat32 GForce; // G-Force vector with origin at pilot position and same referential as pilot orientation
		dbox::F32 SlatLeftDeployment;
		dbox::F32 SlatRightDeployment;	
		dbox::F32 AltAboveGround;	
		dbox::F32 GroundVelocity;
		dbox::F32 LandingGearDeployment;

		DBOX_STRUCTINFO_BEGIN()	
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, Acceleration, dbox::FT_XYZ_FLOAT32, dbox::FM_ACCELERATION_XYZ)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, AngularAcceleration, dbox::FT_XYZ_FLOAT32, dbox::FM_ANGULAR_ACCELERATION_XYZ)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, PitchRad, dbox::FT_FLOAT32, dbox::FM_PITCH_RAD)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, RollRad, dbox::FT_FLOAT32, dbox::FM_ROLL_RAD)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, Engine1N1, dbox::FT_FLOAT32, dbox::FM_ENGINE1_N1)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, Rotor1Rpm, dbox::FT_FLOAT32, dbox::FM_ROTOR1_RPM)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, AngleOfAttack, dbox::FT_FLOAT32, dbox::FM_AIRCRAFT_AOA_DEG)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, IsOnGround, dbox::FT_INT32, dbox::FM_AIRCRAFT_ON_GROUND)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, SpoilerLeftDeployment, dbox::FT_FLOAT32, dbox::FM_SPOILER_LEFT_DEPLOYMENT)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, SpoilerRightDeployment, dbox::FT_FLOAT32, dbox::FM_SPOILER_RIGHT_DEPLOYMENT)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, FlapLeftDeployment, dbox::FT_FLOAT32, dbox::FM_FLAP_LEFT_DEPLOYMENT)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, FlapRightDeployment, dbox::FT_FLOAT32, dbox::FM_FLAP_RIGHT_DEPLOYMENT)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, Velocity, dbox::FT_XYZ_FLOAT32, dbox::FM_VELOCITY_XYZ)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, Engine2N1, dbox::FT_FLOAT32, dbox::FM_ENGINE2_N1)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, Engine3N1, dbox::FT_FLOAT32, dbox::FM_ENGINE3_N1)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, Engine4N1, dbox::FT_FLOAT32, dbox::FM_ENGINE4_N1)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, Rotor2Rpm, dbox::FT_FLOAT32, dbox::FM_ROTOR2_RPM)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, Rotor3Rpm, dbox::FT_FLOAT32, dbox::FM_ROTOR3_RPM)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, Rotor4Rpm, dbox::FT_FLOAT32, dbox::FM_ROTOR4_RPM)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, HeadingRad, dbox::FT_FLOAT32, dbox::FM_YAW_RAD)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, GForce, dbox::FT_XYZ_FLOAT32, dbox::FM_ACTOR_GFORCE_XYZ)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, SlatLeftDeployment, dbox::FT_FLOAT32, dbox::FM_SLAT_LEFT_DEPLOYMENT)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, SlatRightDeployment, dbox::FT_FLOAT32, dbox::FM_SLAT_RIGHT_DEPLOYMENT)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, AltAboveGround, dbox::FT_FLOAT32, dbox::FM_AIRCRAFT_AGL_FT)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, GroundVelocity, dbox::FT_FLOAT32, dbox::FM_AIRCRAFT_GROUND_SPEED_KT)
		DBOX_STRUCTINFO_FIELD(FlightTelemetry, LandingGearDeployment, dbox::FT_FLOAT32, dbox::FM_LANDING_GEAR_GENERAL_DEPLOYMENT)
		DBOX_STRUCTINFO_END()
	};

	/// These are your unique event ids that you'll use when calling PostEvent.
	/// You can use the numbers that are the most appropriate for your application.
	/// These values should be unique and not change in future releases.
	enum AppEvents {
		FLIGHT_CONFIG = 1000,
		FLIGHT_TELEMETRY
	};
}

int main(int, char* []) {

	// Global initialization and registers must be called once at start-up. Initialize/Terminate calls must be balanced.
	dbox::LiveMotion::Initialize(APP_KEY, APP_BUILD);
	dbox::LiveMotion::RegisterEvent(FLIGHT_CONFIG, dbox::EM_CONFIG_UPDATE, FlightConfig::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(FLIGHT_TELEMETRY, dbox::EM_FRAME_UPDATE, FlightTelemetry::GetStructInfo());

	// for sample application only, do not sleep in real application
	Sleep(1000);

	// Registration completed, open motion output device.
	// This will prepare the motion platform for output. Open/Close calls must be balanced.
	dbox::LiveMotion::Open();
	{
		// for sample application only, do not sleep in real application
		Sleep(1000);

		// Simulation Start
		// It is always good to ResetState before each simulation.
		dbox::LiveMotion::ResetState();

		// for sample application only, do not sleep in real application
		Sleep(1000);

		//------------CONFIG------------
		// Configuration fields can be posted when required or when a change occurs.
		// In this case we are selecting a new vehicle. Their state is global and will be maintained 
		// by the Motion Code between updates. Even if a same CONFIGURATION FIELD is registered in 
		// different event structures, it will update the same global state in the Motion Code.	
		FlightConfig oConfig;
		oConfig.AircraftType = 4;
		oConfig.EngineCount = 4;
		oConfig.VelocityVfe = 108;
		oConfig.VelocityVs0 = 55;
		oConfig.VelocityVs = 62;
		oConfig.CriticalAoa = 15;
		oConfig.WingSpan = 45.3f;
		dbox::LiveMotion::PostEvent(FLIGHT_CONFIG, oConfig);
		//---------END OF CONFIG---------

		// for sample application only, do not sleep in real application
		Sleep(1000);

		// Gently starts motion activity based on PostEvent calls and internal state of Motion
        // Code logic. A fade-in will be applied for smooth transition. It should be called as
        // soon as the end - user can interact with the virtual environment, like start of
        // simulation/level, resume from pause, etc.
		dbox::LiveMotion::Start();

		// for sample application only, do not sleep in real application
		Sleep(1000);
		
		//------------FRAME------------
		// Telemetry fields should be posted at every simulation frame or game loop frame.
        // Their state is global and will be maintained by the Motion Code between frames.Even if
        // a same Telemetry field is registered in different event structures, it will update the same
        // global state in the Motion Code.
		// Airplane still on runway, about to take off.
		FlightTelemetry oTelemetry;
		oTelemetry.GForce.X = 0.0f;
		oTelemetry.GForce.Y = 1.0f;
		oTelemetry.GForce.Z = -0.2f;
		oTelemetry.HeadingRad = +1.2f;
		oTelemetry.PitchRad = -0.09f;
		oTelemetry.RollRad = +0.002f;
		oTelemetry.Engine1N1 = 0.23f;
		oTelemetry.FlapLeftDeployment = 0.9f;
		oTelemetry.FlapRightDeployment = 0.9f;
		oTelemetry.SlatLeftDeployment = 0.0f;
		oTelemetry.SlatRightDeployment = 0.0f;
		oTelemetry.SpoilerLeftDeployment = 0.0f;
		oTelemetry.SpoilerRightDeployment = 0.0f;
		oTelemetry.IsOnGround = 1;
		oTelemetry.LandingGearDeployment = 1.0f;
		//oTelemetry...  fill the rest of the fields here and send it with PostEvent() function.
		dbox::LiveMotion::PostEvent(FLIGHT_TELEMETRY, oTelemetry);
		// ...

		// for sample application only, do not sleep in real application
		Sleep(1000);
	
		// Airplane at cruising speed, heading toward the ground.
		oTelemetry.GForce.X = 0.0f;
		oTelemetry.GForce.Y = 0.5f;
		oTelemetry.GForce.Z = -0.3f;
		oTelemetry.HeadingRad = 3.1416f;
		oTelemetry.PitchRad = +0.14f;
		oTelemetry.RollRad = -0.008f;
		oTelemetry.Engine1N1 = 0.75f;
		oTelemetry.FlapLeftDeployment = 0.0f;
		oTelemetry.FlapRightDeployment = 0.0f;
		oTelemetry.SlatLeftDeployment = 0.0f;
		oTelemetry.SlatRightDeployment = 0.0f;
		oTelemetry.SpoilerLeftDeployment = 0.0f;
		oTelemetry.SpoilerRightDeployment = 0.0f;
		oTelemetry.IsOnGround = 0;
		oTelemetry.LandingGearDeployment = 0.0f;
		//oTelemetry...  fill the rest of the fields here.
		dbox::LiveMotion::PostEvent(FLIGHT_TELEMETRY, oTelemetry);

		// for sample application only, do not sleep in real application
		Sleep(1000);
		
		// Pause, this will fade-out motion.
		dbox::LiveMotion::Stop();

		// Resume from pause, this will fade-in actual motion.
		dbox::LiveMotion::Start();

		// for sample application only, do not sleep in real application
		Sleep(1000);

		// ​New Sim Frame
		dbox::LiveMotion::PostEvent(FLIGHT_TELEMETRY, oTelemetry);	 	 
	    // ​New Sim Frame, keep filling the telemetry struct with updated values
		dbox::LiveMotion::PostEvent(FLIGHT_TELEMETRY, oTelemetry);	 	 
	    // ​... 

		// End of simulation, this will fade-out motion.
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