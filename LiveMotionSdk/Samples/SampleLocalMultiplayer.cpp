#include <cstdio>
#include <conio.h>
#include "LiveMotion/dboxLiveMotion.h"
#include <Windows.h>

#pragma comment(lib, DBOX_LIVEMOTION_LIB)

// TODO: Set APP_KEY that will be unique to your D-BOX Integrated Project, no special characters except underscore.
#define APP_KEY         "SampleLocalMultiplayer"
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
	struct WeaponConfig {
		dbox::I32 WeaponID;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(WeaponConfig, WeaponID, dbox::FT_INT32, dbox::FM_WEAPON_ID)
		DBOX_STRUCTINFO_END()
	};

	/// Sample real-time frame structure for FRAME_UPDATE event.
	// TODO: Unused fields can be removed from the structure
	// and new relevant fields can be added to the structure.
	// However the STRUCTINFO section must ALWAYS be matched.
	struct GroundTelemetry {
		dbox::XyzFloat32 Acceleration;
		dbox::XyzFloat32 Velocity;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(GroundTelemetry, Acceleration, dbox::FT_XYZ_FLOAT32, dbox::FM_ACCELERATION_XYZ)	
		DBOX_STRUCTINFO_FIELD(GroundTelemetry, Velocity, dbox::FT_XYZ_FLOAT32, dbox::FM_VELOCITY_XYZ)
		DBOX_STRUCTINFO_END()
	};

	/// Sample attributes structure for EVENT with information.
	// TODO: Unused fields can be removed from the structure
	// and new relevant fields can be added to the structure.
	// However the STRUCTINFO section must ALWAYS be matched.
	struct PickupInfo {
		dbox::I32 ItemID;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(PickupInfo, ItemID, dbox::FT_INT32, dbox::FM_ITEM_ID)
		DBOX_STRUCTINFO_END()

		PickupInfo(dbox::I32 dItemID) : ItemID(dItemID) {}
	};

	/// These are your unique event ids that you'll use when calling PostEvent.
	/// You can use the numbers that are the most appropriate for your application.
	/// These values should be unique and not change in future releases.
	enum AppEvents {	
		PRIMARY_WEAPON_CONFIG = 1000,
		RECOIL_PRIMARY = 2000,
		GROUND_TELEMETRY = 3000,
		EARTHQUAKE = 4000
	};

	enum eWeapons {
		FISTS,
		PISTOL,
		GRENADE,
		EXPELLIARMUS
	};

	enum ePickupItems {
		HEALTH,
		AMMO,
		SHIELD
	};

	enum ePlayerIDs {
		P1,
		P2,
		P3,
		P4
	};
}

int main(int, char* []) {

	// Global initialization and registers must be called once at start-up. Initialize/Terminate calls must be balanced.
	dbox::LiveMotion::Initialize(APP_KEY, APP_BUILD);
	dbox::LiveMotion::RegisterEvent(PRIMARY_WEAPON_CONFIG, dbox::EM_CONFIG_UPDATE, WeaponConfig::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(RECOIL_PRIMARY, dbox::EM_RECOIL_PRIMARY);
	dbox::LiveMotion::RegisterEvent(GROUND_TELEMETRY, dbox::EM_FRAME_UPDATE, GroundTelemetry::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(EARTHQUAKE, dbox::EM_ACTION_TRIGGER_PULSE);

	// Registration completed, open motion output device.
	// This will prepare the motion platform for output. Open/Close calls must be balanced.
	// Pass the number of players as a parameter to the Open call
	// In other words, this is the number of D-BOX Systems connected to your machine
	dbox::LiveMotion::Open(2); {

		// Level start
		// It is always good to ResetState before each level.
		dbox::LiveMotion::ResetState();

		//------------CONFIG------------		
		// Configuration fields can be posted when required or when a change occurs.
		// In this case we are selecting a new weapon. Their state is global and will be maintained 
		// by the Motion Code between updates. Even if a same CONFIGURATION FIELD is registered in 
		// different event structures, it will update the same global state in the Motion Code.	
		// Player 1 selects his weapon
		WeaponConfig oWeaponConfigP1;
		oWeaponConfigP1.WeaponID = PISTOL;
		// Call PostEventToSession INSTEAD of PostEvent
		// Give ID of connected device. Same order as listed devices in D-BOX Control Panel
		dbox::LiveMotion::PostEventToSession(P1, PRIMARY_WEAPON_CONFIG, oWeaponConfigP1);

		// Player 2 selects his weapon
		WeaponConfig oWeaponConfigP2; 
		oWeaponConfigP2.WeaponID = EXPELLIARMUS;
		// Call PostEventToSession INSTEAD of PostEvent
		// Give ID of connected device. Same order as listed devices in D-BOX Control Panel
		dbox::LiveMotion::PostEventToSession(P2, PRIMARY_WEAPON_CONFIG, oWeaponConfigP1);
		//---------END OF CONFIG---------
		
		// Gently starts motion activity based on PostEvent calls and internal state of Motion
        // Code logic. A fade-in will be applied for smooth transition. It should be called as
        // soon as the end - user can interact with the virtual environment, like start of
        // simulation/level, resume from pause, etc.
		dbox::LiveMotion::Start();

		//------------FRAMEs------------
		// Send this info each game frame
		GroundTelemetry oTelemetryP1;
		GroundTelemetry oTelemetryP2;

		// First Frame for P1
		oTelemetryP1.Acceleration.X = 0.4f;
		oTelemetryP1.Acceleration.Y = 0.2f;
		oTelemetryP1.Acceleration.Z = 1.0f;
		// Keep filling the oTelemetryObject, then send it with PostEventToSession() function.
		dbox::LiveMotion::PostEventToSession(P1, GROUND_TELEMETRY, oTelemetryP1);

		// First Frame for P2
		oTelemetryP2.Acceleration.X = 0.4f;
		oTelemetryP2.Acceleration.Y = 0.2f;
		oTelemetryP2.Acceleration.Z = 1.0f;
		// Keep filling the oTelemetryObject, then send it with PostEventToSession() function.
		dbox::LiveMotion::PostEventToSession(P2, GROUND_TELEMETRY, oTelemetryP2);

		// Pause, this will fade-out motion.
		dbox::LiveMotion::Stop();

		// Resume from pause, this will fade-in actual motion.
		dbox::LiveMotion::Start();
		// ​New Game Frame, keep filling the telemetry struct with updated values
		dbox::LiveMotion::PostEventToSession(P1, GROUND_TELEMETRY, oTelemetryP1);
		dbox::LiveMotion::PostEventToSession(P2, GROUND_TELEMETRY, oTelemetryP2);
		// ​New Game Frame
		dbox::LiveMotion::PostEventToSession(P1, GROUND_TELEMETRY, oTelemetryP1);
		dbox::LiveMotion::PostEventToSession(P2, GROUND_TELEMETRY, oTelemetryP2);
		// ...

		// For an event that will be felt by all players call a regular PostEvent()
		// and it will play on all D-BOX systems connected to the game.
		// Example: An earthquake and both players are affected
		dbox::LiveMotion::PostEvent(EARTHQUAKE);

		// End of level, this will fade-out motion.
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