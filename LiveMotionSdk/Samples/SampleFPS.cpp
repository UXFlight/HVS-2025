#include <cstdio>
#include <conio.h>
#include "LiveMotion/dboxLiveMotion.h"
#include <Windows.h>

#pragma comment(lib, DBOX_LIVEMOTION_LIB)

// TODO: Set APP_KEY that will be unique to your D-BOX Integrated Project, no special characters except underscore.
#define APP_KEY         "SampleFPS"
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
		dbox::XyzFloat32 Position;
		dbox::F32		 Heading;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(GroundTelemetry, Acceleration, dbox::FT_XYZ_FLOAT32, dbox::FM_ACCELERATION_XYZ)	
		DBOX_STRUCTINFO_FIELD(GroundTelemetry, Velocity, dbox::FT_XYZ_FLOAT32, dbox::FM_VELOCITY_XYZ)
		DBOX_STRUCTINFO_FIELD(GroundTelemetry, Position, dbox::FT_XYZ_FLOAT32, dbox::FM_POSITION_XYZ)
		DBOX_STRUCTINFO_FIELD(GroundTelemetry, Heading, dbox::FT_FLOAT32, dbox::FM_YAW_RAD)
		DBOX_STRUCTINFO_END()
	};

	/// Sample attributes structure for EVENT with information.
	// TODO: Unused fields can be removed from the structure
	// and new relevant fields can be added to the structure.
	// However the STRUCTINFO section must ALWAYS be matched.
	struct HitInfo {
		dbox::F32 Intensity;
		dbox::I32 WeaponID;
		dbox::I32 MaterialID;
		dbox::F32 OrientationRad;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(HitInfo, Intensity, dbox::FT_FLOAT32, dbox::FM_EVENT_INTENSITY)
		DBOX_STRUCTINFO_FIELD(HitInfo, WeaponID, dbox::FT_INT32, dbox::FM_WEAPON_ID)
		DBOX_STRUCTINFO_FIELD(HitInfo, MaterialID, dbox::FT_INT32, dbox::FM_MATERIAL_ID)
		DBOX_STRUCTINFO_FIELD(HitInfo, OrientationRad, dbox::FT_FLOAT32, dbox::FM_EVENT_YAW_RAD)
		DBOX_STRUCTINFO_END()

		HitInfo(dbox::F32 dIntensity, dbox::I32 dWeaponID, dbox::I32 dMaterialID, dbox::F32 dOrientationRad) :
			Intensity(dIntensity), WeaponID(dWeaponID), MaterialID(dMaterialID), OrientationRad(dOrientationRad) {
		}
	};

	/// Sample attributes structure for EVENT with information.
	struct ExplosionInfo {
		dbox::XyzFloat32 Position;
		dbox::F32 Intensity;
		dbox::F32 Radius;
		dbox::F32 DebrisQty;

		DBOX_STRUCTINFO_BEGIN()
		DBOX_STRUCTINFO_FIELD(ExplosionInfo, Position, dbox::FT_XYZ_FLOAT32, dbox::FM_POSITION_XYZ)
		DBOX_STRUCTINFO_FIELD(ExplosionInfo, Intensity, dbox::FT_FLOAT32, dbox::FM_EVENT_INTENSITY)
		DBOX_STRUCTINFO_FIELD(ExplosionInfo, Radius, dbox::FT_FLOAT32, dbox::FM_RADIUS_M)
		DBOX_STRUCTINFO_FIELD(ExplosionInfo, DebrisQty, dbox::FT_FLOAT32, dbox::FM_DEBRIS_QTY)
		DBOX_STRUCTINFO_END()

		ExplosionInfo(dbox::XyzFloat32 dPosition, dbox::F32 dIntensity, dbox::F32 dRadius, dbox::F32 dDebrisQty)
		: Position(dPosition), Intensity(dIntensity), Radius(dRadius), DebrisQty(dDebrisQty) {}
	};

	/// Sample attributes structure for EVENT with information.
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
		EXPLOSION = 4000,
		HIT = 5000,
		HURT = 6000,
		PICKUP = 7000
	};

	enum eWeapons {
		FISTS,
		PISTOL,
		GRENADE,
		EXPELLIARMUS
	};

	enum eMaterials {
		WOOD,
		METAL,
		FLESH
	};

	enum ePickupItems {
		HEALTH,
		AMMO,
		SHIELD
	};
}

int main(int, char* []) {

	// Global initialization and registers must be called once at start-up. Initialize/Terminate calls must be balanced.
	dbox::LiveMotion::Initialize(APP_KEY, APP_BUILD);
	dbox::LiveMotion::RegisterEvent(PRIMARY_WEAPON_CONFIG, dbox::EM_CONFIG_UPDATE, WeaponConfig::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(RECOIL_PRIMARY, dbox::EM_RECOIL_PRIMARY);
	dbox::LiveMotion::RegisterEvent(GROUND_TELEMETRY, dbox::EM_FRAME_UPDATE, GroundTelemetry::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(EXPLOSION, dbox::EM_FRAME_UPDATE, ExplosionInfo::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(HIT, dbox::EM_IMPACT, HitInfo::GetStructInfo());
	dbox::LiveMotion::RegisterEvent(HURT, dbox::EM_IS_DAMAGED);
	dbox::LiveMotion::RegisterEvent(PICKUP, dbox::EM_PICKUP, PickupInfo::GetStructInfo());

	// for sample application only, do not sleep in real application
	Sleep(1000);

	// Registration completed, open motion output device.
	// This will prepare the motion platform for output. Open/Close calls must be balanced.
	dbox::LiveMotion::Open(); {

		// Level Start
		// It is always good to ResetState before each start.
		dbox::LiveMotion::ResetState();

		// for sample application only, do not sleep in real application
		Sleep(1000);

		//------------  CONFIG ------------		
		// Configuration fields can be posted when required or when a change occurs.
		// In this case we are selecting a new weapon. Their state is global and will be maintained 
		// by the Motion Code between updates. Even if a same CONFIGURATION FIELD is registered in 
		// different event structures, it will update the same global state in the Motion Code.	
		WeaponConfig oWeaponConfig;
		oWeaponConfig.WeaponID = PISTOL;
		dbox::LiveMotion::PostEvent(PRIMARY_WEAPON_CONFIG, oWeaponConfig);
		//---------  END OF CONFIG ---------

		// for sample application only, do not sleep in real application
		Sleep(1000);

		// Gently starts motion activity based on PostEvent calls and internal state of Motion
        // Code logic. A fade-in will be applied for smooth transition. It should be called as
        // soon as the end-user can interact with the virtual environment, like start of
        // simulation/level, resume from pause, etc.
		dbox::LiveMotion::Start();

		//------------  FRAME ------------
		// Send this info each game frame
		GroundTelemetry oTelemetry;
		oTelemetry.Acceleration.X = 0.4f;		// can add ROLL while strafing
		oTelemetry.Acceleration.Y = 0.2f;		// will add HEAVE effects for jumping and landing
		oTelemetry.Velocity.Z = 1.0f;			// will allow for player bobbing effects, slow or rapid depending on velocity
		// Keep filling the oTelemetryObject, then send it with PostEvent() function.
		dbox::LiveMotion::PostEvent(GROUND_TELEMETRY, oTelemetry);

		// Filing and posting at every game loop
		for (int i = 0; i < 100; i++) {
			dbox::LiveMotion::PostEvent(GROUND_TELEMETRY, oTelemetry);
		}

		// for sample application only, do not sleep in real application
		Sleep(1000);

		// The player shoots at an enemy, since we posted a Weapon Config earlier, we know what weapon is being used
		// Not all Events require information, only one parameter is needed in this case
		dbox::LiveMotion::PostEvent(RECOIL_PRIMARY);

		// for sample application only, do not sleep in real application
		Sleep(1000);

		// Pause, this will fade-out motion.
		dbox::LiveMotion::Stop();

		// Resume from pause, this will fade-in actual motion.
		dbox::LiveMotion::Start();
		
		// The enemy throws a grenade to the player and it explodes
		dbox::XyzFloat32	exploPosition;
		exploPosition.X = 5.0f;
		exploPosition.Y = 0.f;
		exploPosition.Z = 2.0f;
		dbox::LiveMotion::PostEvent(EXPLOSION, ExplosionInfo(/*Position*/ exploPosition, /*Intensity*/ 1.4f, /*Radius*/ 0.9f, /*DebrisQty*/ 0.1f));

		// for sample application only, do not sleep in real application
		Sleep(1000);

		// While the explosion is happening, we can send player position and orientation updates
		// To have the effect change direction and intensity as the player continues to move
		// Very useful if the effect is to last for some time (ex: a collapsing building)
		oTelemetry.Acceleration.X = 0.4f;
		oTelemetry.Acceleration.Y = 0.2f;
		oTelemetry.Velocity.Z = 1.0f;
		oTelemetry.Position.X = -1.0f;
		oTelemetry.Position.Y = 0.1f;
		oTelemetry.Position.Z = -0.5f;
		oTelemetry.Heading = -2.1f;
		// Keep filling the oTelemetryObject, then send it with PostEvent() function.
		dbox::LiveMotion::PostEvent(GROUND_TELEMETRY, oTelemetry);

		// Filing and posting at every game loop
		for (int i = 0; i < 100; i++) {
			dbox::LiveMotion::PostEvent(GROUND_TELEMETRY, oTelemetry);
		}

		// for sample application only, do not sleep in real application
		Sleep(1000);

		// A piece of shrapnel hits the player
		// Material of where the player got hit = FLESH
		// Enemy weapon used to attack player = GRENADE
		// Send info to D-BOX
		dbox::LiveMotion::PostEvent(HIT, HitInfo(/*Intensity*/ 0.1f, /*WeaponID*/ GRENADE, 
		/*MaterialID*/ FLESH, /*Orientation*/ 1.45f));

		// Player also got hurt by the shrapnel
		dbox::LiveMotion::PostEvent(HURT);

		// Player picks up a health pack
		dbox::LiveMotion::PostEvent(PICKUP, PickupInfo(HEALTH));

		// for sample application only, do not sleep in real application
		Sleep(1000);

		// Frame
		oTelemetry.Acceleration.X = 0.4f;		// can add ROLL while strafing
		oTelemetry.Acceleration.Y = 0.2f;		// will add HEAVE effects for jumping and landing
		oTelemetry.Velocity.Z = 1.0f;			// will allow for player bobbing effects, slow or rapid depending on velocity
		// New Game Frame, keep filling the telemetry struct with updated values
		dbox::LiveMotion::PostEvent(GROUND_TELEMETRY, oTelemetry);
		// New Game Frame
		dbox::LiveMotion::PostEvent(GROUND_TELEMETRY, oTelemetry);
		// New Game Frame
		dbox::LiveMotion::PostEvent(GROUND_TELEMETRY, oTelemetry);
		// ...

		// for sample application only, do not sleep in real application
		Sleep(1000);

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