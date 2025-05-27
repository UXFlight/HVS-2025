#pragma once
#ifndef _DBOX_EFIELDMEANING_H_INCLUDED_
#define _DBOX_EFIELDMEANING_H_INCLUDED_
//------------------------------------------------------------------------
// D-BOX LIVE MOTION API
// Copyright (C) 2008, D-BOX Technologies Inc.
// All rights reserved.
//------------------------------------------------------------------------

namespace dbox {
	//------------------------------------------------------------------------
	/// This enum is the list of known meanings that can be associated to
	/// fields in a struct.  This association will be used during event registration.
	/// D-BOX may add meanings to this list over time, but should not remove any.
	/// Please see CONFIGURATION AND FRAME FIELDS section and EVENT FIELDS section
	/// below for specific details.
	//------------------------------------------------------------------------
	enum EFieldMeaning {
		FM_UNKNOWN = 0,

		//========================================================================
		// CONFIGURATION AND FRAME FIELDS
		//========================================================================
		// - FRAME FIELDS should be posted at every simulation frame or game loop frame.
		//   Their state is global and will be maintained by the motion code between frames.
		//   Even if a same FRAME FIELD is registered in different event structures,
		//   it will update the same global state in the motion code.
		// - CONFIGURATION FIELDS can be posted when required or when a change occurs.
		//   Their state is global and will be maintained by the motion code between updates.
		//   Even if a same CONFIGURATION FIELD is registered in different event structures,
		//   it will update the same global state in the motion code.

		//------------------------------------------------------------------------
		// Global Mixer
		//------------------------------------------------------------------------

		/// Master gain applied at the final output, in decibel.
		/// It is recommended to expose this setting in the application's user interface.
		/// 0dB is default level.
		/// CONFIGURATION FIELD
		FM_MASTER_GAIN_DB = 105,
		/// Motion/Vibration balance control, in decibel.
		/// It is recommended to expose this setting in the application's user interface.
		/// Value must be between -20dB and +20dB: -20dB being maximum vibration (high frequencies)
		/// attenuation, +20dB being maximum motion (low frequencies) attenuation and
		/// 0dB being normal signal (full spectrum).
		/// CONFIGURATION FIELD
		FM_MASTER_SPECTRUM_DB = 106,


		//------------------------------------------------------------------------
		// General Dynamics
		//------------------------------------------------------------------------

		/// Milliseconds elapsed since the last post of this field.
		/// Required when physics is computed ahead of time to synchronize all FRAME fields in time.
		/// FRAME FIELD
		FM_ELAPSED_TIME_MS = 93,

		/// Xyz vector of 3D felt G-Force vector in G relative to actor (i.e. driver/pilot) position and orientation.
		/// Positive X is right acceleration, Positive Y is up acceleration, Positive Z is forward acceleration.
		/// Normally standing on Earth: Y = +1.0.
		/// Note: ACTOR_GFORCE is always relative to actor and not [Reference] (see below).
		/// FRAME FIELD
		FM_ACTOR_GFORCE_XYZ = 68,

		/// [Reference]: 3D point in space where physics is computed.
		/// CASE [Reference] = Actor (i.e driver, pilot, soldier)
		///  - Reference positive X is right-hand side of actor body, positive Y is upward and positive Z is forward.
		///  - No ACTOR_OFFSET required but must be 0 if supplied.
		///  - i.e. If the [Reference] for given acceleration and velocity is the pilot then no ACTOR_OFFSET is needed.
		/// CASE [Reference] = Vehicle (i.e. car, plane, carrier)
		///  - Reference positive X is right side of vehicle looking forward, positive Y is upward and positive Z is forward.
		///  - ACTOR_OFFSET required to specify actor position and orientation relative to vehicle reference.
		///  - i.e. If the [Reference] for given acceleration and velocity is the vehicle center of gravity,
		///    ACTOR_OFFSET should represent the offset of the pilot from vehicle center of gravity.

		/// Xyz vector of actor (i.e. driver/pilot) position in meters, relative to the [Reference] used.
		/// Offset position must be specified in the case where [Reference] position is different than actor position.
		/// Positive X is right position, Positive Y is up position and Positive Z is forward position, relative to the [Reference] local X Y Z axis.
		/// CONFIGURATION FIELD
		FM_ACTOR_OFFSET_POSITION_XYZ = 77,
		/// Right yaw or heading of actor (i.e. driver/pilot), in radians, relative to [Reference] local Y axis.
		/// In the case where actor orientation is different than [Reference] orientation, that angle should be specified.
		/// Yaw expresses a rotation along [Reference] Y axis (vehicle up).
		/// (0 means actor facing front of the vehicle, pi/2 means actor facing right of the vehicle)
		/// CONFIGURATION FIELD
		FM_ACTOR_OFFSET_YAW_RAD = 78,
		/// Forward pitch of actor (i.e. driver/pilot), in radians, relative to [Reference] X-Z plane.
		/// In the case where actor orientation is different than [Reference] orientation, that angle should be specified.
		/// Pitch expresses a rotation along actor X axis, after Yaw is applied.
		/// (0 means actor same level as vehicle, pi/2 means actor facing vehicle floor)
		/// CONFIGURATION FIELD
		FM_ACTOR_OFFSET_PITCH_RAD = 79,
		/// Left roll of actor (i.e driver/pilot), in radians, relative to [Reference] pitch.
		/// In the case where actor orientation is different than [Reference] orientation, that angle should be specified.
		/// Roll expresses a rotation along actor Z axis, after Pitch and Yaw are applied.
		/// (0 means lateral level, pi/2 means actor rotated left)
		/// CONFIGURATION FIELD
		FM_ACTOR_OFFSET_ROLL_RAD = 80,
		/// Xyz vector of 3D acceleration in m/s2 relative to a [Reference] position and orientation.
		/// If actor position/orientation is different than [Reference] position/orientation, ACTOR_OFFSET fields must be specified.
		/// Positive X is right acceleration, Positive Y is up acceleration, Positive Z is forward acceleration.
		/// FRAME FIELD
		FM_ACCELERATION_XYZ = 8,
		/// Xyz vector of 3D velocity in m/s relative to a [Reference] position and orientation.
		/// If actor position/orientation is different than [Reference] position/orientation, ACTOR_OFFSET fields must be specified.
		/// Positive X is right velocity, Positive Y is up velocity, Positive Z is forward velocity.
		/// FRAME FIELD
		FM_VELOCITY_XYZ = 9,
		/// Xyz vector of position of a [Reference] in the world.
		/// Positive X is to the east, Positive Y is to the sky and Positive Z is to the north.
		/// FRAME FIELD
		FM_POSITION_XYZ = 110,
		/// Right yaw or heading of a [Reference], in radians, relative to world Y axis (0 means toward North, pi/2 means toward East).
		/// If actor orientation is different than [Reference] orientation, ACTOR_OFFSET fields must be specified.
		/// Yaw expresses a rotation along [Reference] Y axis.
		/// FRAME FIELD
		FM_YAW_RAD = 22,
		/// Forward pitch of a [Reference], in radians, relative to world X-Z plane (0 means forward level, pi/2 means toward ground).
		/// If actor orientation is different than [Reference] orientation, ACTOR_OFFSET fields must be specified.
		/// Pitch expresses a rotation along [Reference] X axis, after Yaw is applied.
		/// FRAME FIELD
		FM_PITCH_RAD = 21,
		/// Left roll of a [Reference], in radians, relative to pitch (0 means lateral level, pi/2 means body rotated left).
		/// If actor orientation is different than [Reference] orientation, ACTOR_OFFSET fields must be specified.
		/// Roll expresses a rotation along [Reference] Z axis, after Pitch and Yaw are applied.
		/// FRAME FIELD
		FM_ROLL_RAD = 23,
		/// Xyz structure of 3D angular acceleration in rad/s2 relative to a [Reference] position and orientation.
		/// If actor position/orientation is different than [Reference] position/orientation, ACTOR_OFFSET fields must be specified.
		/// Positive X is forward pitch acceleration, positive Y is right yaw acceleration, positive Z is left roll acceleration.
		/// FRAME FIELD
		FM_ANGULAR_ACCELERATION_XYZ = 34,
		/// Xyz structure of 3D angular velocity in rad/s relative to a [Reference] position and orientation.
		/// If actor position/orientation is different than [Reference] position/orientation, ACTOR_OFFSET fields must be specified.
		/// Positive X is forward pitch velocity, positive Y is right yaw velocity, positive Z is left roll velocity.
		/// FRAME FIELD
		FM_ANGULAR_VELOCITY_XYZ = 35,

		/// User configurable gain for left-right general dynamics motion effect.
		/// This setting can be exposed in the application's user interface.
		/// Value should be between -60dB and +20dB: 0dB being normal signal.
		/// Values lower than -60 means no effect, use -999 for explicit mute.
		/// CONFIGURATION FIELD
		FM_DYNAMIC_ROLL_GAIN_DB = 112,
		/// User configurable gain for front-back general dynamics motion effect.
		/// This setting can be exposed in the application's user interface.
		/// Value should be between -60dB and +20dB: 0dB being normal signal.
		/// Values lower than -60 means no effect, use -999 for explicit mute.
		/// CONFIGURATION FIELD
		FM_DYNAMIC_PITCH_GAIN_DB = 113,
		/// User configurable gain for up-down general dynamics motion effect.
		/// This setting can be exposed in the application's user interface.
		/// Value should be between -60dB and +20dB: 0dB being normal signal.
		/// Values lower than -60 means no effect, use -999 for explicit mute.
		/// CONFIGURATION FIELD
		FM_DYNAMIC_HEAVE_GAIN_DB = 114,

		//------------------------------------------------------------------------
		// General Context
		//------------------------------------------------------------------------

		/// Numeric identification of current vehicle.
		/// CONFIGURATION FIELD
		FM_VEHICLE = 27,
		/// String identification of current vehicle.
		/// CONFIGURATION FIELD
		FM_VEHICLE_STR = 28,
		/// Numeric identification of current vehicle type or class.
		/// CONFIGURATION FIELD
		FM_VEHICLE_TYPE = 29,
		/// String identification of current vehicle type or class.
		/// CONFIGURATION FIELD
		FM_VEHICLE_TYPE_STR = 30,

		/// Custom numeric identification of current map, track or level.
		/// CONFIGURATION FIELD
		FM_MAP = 31,
		/// Custom string identification of current map, track or level.
		/// CONFIGURATION FIELD
		FM_MAP_STR = 32,
		/// Custom numeric identification of the type or classification of current map, track or level.
		/// CONFIGURATION FIELD
		FM_MAP_TYPE = 14,
		/// Custom string identification of the type or classification of type of map, track or level.
		/// CONFIGURATION FIELD
		FM_MAP_TYPE_STR = 37,


		//------------------------------------------------------------------------
		// General Vehicle Engine
		//------------------------------------------------------------------------

		/// Absolute maximum RPM of vehicle engine.
		/// CONFIGURATION FIELD
		FM_ENGINE_RPM_MAX = 2,
		/// Common start of red RPM zone of vehicle engine.
		/// CONFIGURATION FIELD
		FM_ENGINE_RPM_RED = 3,
		/// Common idle RPM of vehicle engine.
		/// CONFIGURATION FIELD
		FM_ENGINE_RPM_IDLE = 4,
		/// Instantaneous engine rotation speed in RPM.
		/// FRAME FIELD
		FM_ENGINE_RPM = 1,
		/// Number of cylinders in main engine.
		/// CONFIGURATION FIELD
		FM_ENGINE_CYLINDERS = 7,
		/// Xyz vector of engine offset relative to [Reference]
		/// CONFIGURATION FIELD
		FM_ENGINE_OFFSET_XYZ = 119,

		/// Maximum engine power in Watts (1 HP = 745.699872 watts).
		/// CONFIGURATION FIELD
		FM_ENGINE_POWER_MAX = 15,
		/// Instantaneous engine power in Watts (1 HP = 745.699872 watts).
		/// FRAME FIELD
		FM_ENGINE_POWER = 17,

		/// Maximum engine torque in Newton meters (1 foot pound = 1.35581795 newtons meter).
		/// CONFIGURATION FIELD
		FM_ENGINE_TORQUE_MAX = 16,
		/// Instantaneous engine torque in Newton meters (1 foot pound = 1.35581795 newtons meter).
		/// Torque should be negative when breaking on engine compression.
		/// FRAME FIELD
		FM_ENGINE_TORQUE = 18,

		/// Current transmission gear number (Reverse = -1, Neutral = 0, 1st = 1, 2nd = 2...).
		/// FRAME FIELD
		FM_ENGINE_TRANSMISSION_GEAR = 5,
		/// Current transmission shaft rotation speed in RPM.
		/// FRAME FIELD
		FM_ENGINE_TRANSMISSION_RPM = 6,
		/// Engine current boost intensity from 0.0 to 1.0 (0.0 is no boost (normal), 1 is maximum boost effect).
		/// FRAME FIELD
		FM_ENGINE_BOOST = 33,

		/// User configurable gain for engine vibration effect.
		/// This setting can be exposed in the application's user interface.
		/// Value should be between -60dB and +20dB: 0dB being normal signal.
		/// Values lower than -60 means no effect, use -999 for explicit mute.
		/// CONFIGURATION FIELD
		FM_ENGINE_GAIN_DB = 115,
		/// User configurable gain for engine boost vibration effect (nitrous).
		/// This setting can be exposed in the application's user interface.
		/// Value should be between -60dB and +20dB: 0dB being normal signal.
		/// Values lower than -60 means no effect, use -999 for explicit mute.
		/// CONFIGURATION FIELD
		FM_ENGINE_BOOST_GAIN_DB = 116,

		//------------------------------------------------------------------------
		// Vehicle Multi-Engine (i.e. aircraft)
		//------------------------------------------------------------------------

		/// Engine count, up to 8.  If not specified then only one engine is assumed.
		/// CONFIGURATION FIELD
		FM_ENGINE_COUNT = 69,
		/// Instantaneous engine 1 rotation speed in RPM.
		/// FRAME FIELD
		FM_ENGINE1_RPM = FM_ENGINE_RPM,
		// Instantaneous engine 1 N1 power in percentage.
		// FRAME FIELD
		FM_ENGINE1_N1 = 155,
		// Instantaneous engine 1 rotor rotation in RPM.
		// FRAME FIELD
		FM_ROTOR1_RPM = 156,
		/// Xyz vector of engine 1 offset relative to [Reference].
		/// CONFIGURATION FIELD
		FM_ENGINE1_OFFSET_XYZ = FM_ENGINE_OFFSET_XYZ,
		/// Instantaneous engine 2 rotation speed in RPM.
		/// FRAME FIELD
		FM_ENGINE2_RPM = 70,
		// Instantaneous engine 2 N1 power in percentage.
		// FRAME FIELD
		FM_ENGINE2_N1 = 157,
		// Instantaneous engine 2 rotor rotation in RPM.
		// FRAME FIELD
		FM_ROTOR2_RPM = 158,
		/// Xyz vector of engine 2 offset relative to [Reference].
		/// CONFIGURATION FIELD
		FM_ENGINE2_OFFSET_XYZ = 120,
		/// Instantaneous engine 3 rotation speed in RPM.
		/// FRAME FIELD
		FM_ENGINE3_RPM = 71,
		// Instantaneous engine3 N1 power in percentage.
		// FRAME FIELD
		FM_ENGINE3_N1 = 159,
		// Instantaneous engine 3 rotor rotation in RPM.
		// FRAME FIELD
		FM_ROTOR3_RPM = 160,
		/// Xyz vector of engine 3 offset relative to [Reference].
		/// CONFIGURATION FIELD
		FM_ENGINE3_OFFSET_XYZ = 121,
		/// Instantaneous engine 4 rotation speed in RPM.
		/// FRAME FIELD
		FM_ENGINE4_RPM = 72,
		// Instantaneous engine 4 N1 power in percentage.
		// FRAME FIELD
		FM_ENGINE4_N1 = 161,
		// Instantaneous engine 4 rotor rotation in RPM.
		// FRAME FIELD
		FM_ROTOR4_RPM = 162,
		/// Xyz vector of engine 4 offset relative to [Reference].
		/// CONFIGURATION FIELD
		FM_ENGINE4_OFFSET_XYZ = 122,
		/// Instantaneous engine 5 rotation speed in RPM.
		/// FRAME FIELD
		FM_ENGINE5_RPM = 73,
		// Instantaneous engine 5 N1 power in percentage.
		// FRAME FIELD
		FM_ENGINE5_N1 = 163,
		// Instantaneous engine 5 rotor rotation in RPM.
		// FRAME FIELD
		FM_ROTOR5_RPM = 164,
		/// Xyz vector of engine 5 offset relative to [Reference].
		/// CONFIGURATION FIELD
		FM_ENGINE5_OFFSET_XYZ = 123,
		/// Instantaneous engine 6 rotation speed in RPM.
		/// FRAME FIELD
		FM_ENGINE6_RPM = 74,
		// Instantaneous engine 6 N1 power in percentage.
		// FRAME FIELD
		FM_ENGINE6_N1 = 165,
		// Instantaneous engine 6 rotor rotation in RPM.
		// FRAME FIELD
		FM_ROTOR6_RPM = 166,
		/// Xyz vector of engine 6 offset relative to [Reference].
		/// CONFIGURATION FIELD
		FM_ENGINE6_OFFSET_XYZ = 124,
		/// Instantaneous engine 7 rotation speed in RPM.
		/// FRAME FIELD
		FM_ENGINE7_RPM = 75,
		// Instantaneous engine 7 N1 power in percentage.
		// FRAME FIELD
		FM_ENGINE7_N1 = 167,
		// Instantaneous engine 7 rotor rotation in RPM.
		// FRAME FIELD
		FM_ROTOR7_RPM = 168,
		/// Xyz vector of engine 7 offset relative to [Reference].
		/// CONFIGURATION FIELD
		FM_ENGINE7_OFFSET_XYZ = 125,
		/// Instantaneous engine 8 rotation speed in RPM.
		/// FRAME FIELD
		FM_ENGINE8_RPM = 76,
		// Instantaneous engine 8 N1 power in percentage.
		// FRAME FIELD
		FM_ENGINE8_N1 = 169,
		// Instantaneous engine 8 rotor rotation in RPM.
		// FRAME FIELD
		FM_ROTOR8_RPM = 170,
		/// Xyz vector of engine 8 offset relative to [Reference].
		/// CONFIGURATION FIELD
		FM_ENGINE8_OFFSET_XYZ = 126,


		//------------------------------------------------------------------------
		// Vehicle Corners (independent wheel info)
		//------------------------------------------------------------------------

		/// Absolute maximum down suspension travel range in meters (1 in = 0.0254 meters).
		/// If applicable, this should also include frame torsion the vehicle can normally handle.
		/// CONFIGURATION FIELD
		FM_SUSPENSION_TRAVEL_MAX_QUAD = 19,
		/// Current suspension travel position relative to maximum travel range,
		/// from 0.0 (in air) to 1.0 (full down).
		/// FRAME FIELD
		FM_SUSPENSION_TRAVEL_NORM_QUAD = 36,
		/// Current suspension travel position, from 0.0 (in air) to down SUSPENSION_TRAVEL_MAX
		/// in meters (1 in = 0.0254 meters).
		/// FRAME FIELD
		FM_SUSPENSION_TRAVEL_QUAD = 13,
		/// User configurable gain for road texture motion effect (based on suspension or tire load).
		/// This setting can be exposed in the application's user interface.
		/// Value should be between -60dB and +20dB: 0dB being normal signal.
		/// Values lower than -60 means no effect, use -999 for explicit mute.
		/// CONFIGURATION FIELD
		FM_SUSPENSION_GAIN_DB = 117,

		/// Arbitrary integer value used to represent difference surface textures/types (e.g.: in air=1, gravel=2, sand=3, etc.).
		/// FRAME FIELD
		FM_SURFACE_TYPE_ID = 148,

		/// Load for each tire relative to vehicle weight, normalized to 1.0 at rest, 0.0 when in air.
		/// FRAME FIELD
		FM_TIRE_LOAD_NORM_QUAD = 10,
		/// Longitudinal slip velocity ratio, generally defined as ((A * R) - V) / V where
		/// A = tire angular velocity in rad/s, R = tire radius in meters and V = ground velocity in meters.
		/// Ratio should be 0 when V = 0 and A = 0.
		/// Ratio should be limited to a positive value (i.e. +5) when V = 0 and A != 0.
		/// Ratio = 0 means 100% grip and |ratio| > 1.0 means loss of grip.
		/// FRAME FIELD
		FM_TIRE_SLIP_RATIO_QUAD = 11,
		/// Angle between a rolling wheel's actual direction of travel and the direction towards which it is pointing.
		/// The slip angle is per tire and is relative to the tire itself and not the vehicle.
		/// FRAME FIELD
		FM_TIRE_SLIP_ANGLE_RAD_QUAD = 12,
		/// Current tangent slip velocity for each tire in m/s (1 mph = 0.44704 meters / second).
		/// This should represent the speed difference magnitude between tire and surface
		/// Can be expressed as (A * R) - V where A = tire angular velocity in rad/s,
		/// R = tire radius in meters and V = ground velocity in meters.
		/// FRAME FIELD
		FM_TIRE_SLIP_VELOCITY_QUAD = 20,
		/// User configurable gain for tire skid motion effect.
		/// This setting can be exposed in the application's user interface.
		/// Value should be between -60dB and +20dB: 0dB being normal signal.
		/// Values lower than -60 means no effect, use -999 for explicit mute.
		/// CONFIGURATION FIELD
		FM_TIRE_SLIP_GAIN_DB = 118,

		/// Speed in m/s at which each tire is rotating longitudinally.
		/// The direction should be parallel to the direction of motion of the vehicle.
		/// FRAME FIELD
		FM_TIRE_ROTATION_SPEED_QUAD = 154,

		//------------------------------------------------------------------------
		// Aircraft
		//------------------------------------------------------------------------

		/// True Airspeed, in knots (1 mph = 0.86898 knot).
		/// FRAME FIELD
		FM_AIRCRAFT_TAS_KT = 85,
		/// Maximum Flaps Extended Speed Vfe, in knots (1 mph = 0.86898 knot).
		/// CONFIGURATION FIELD
		FM_AIRCRAFT_VFE_KT = 86,
		/// Maximum Landing Gear operation speed, in knots (1 mph = 0.86898 knot)
		/// CONFIGURATION FIELD
		FM_AIRCRAFT_VLO_KT = 94,
		/// Stalling speed with flaps retracted Vs, in knots (1 mph = 0.86898 knot).
		/// CONFIGURATION FIELD
		FM_AIRCRAFT_VS_KT = 87,
		/// Stalling speed in landing configuration (flaps fully extended, landing gear deployed),
		/// in knots (1 mph = 0.86898 knot).
		/// CONFIGURATION FIELD
		FM_AIRCRAFT_VS0_KT = 88,
		/// Critical Angle of Attack (AOA, alpha), angle in degrees where aircraft is said to be in a stall.
		/// CONFIGURATION FIELD
		FM_AIRCRAFT_CRITICAL_AOA_DEG = 89,
		/// Angle of Attack (AOA, alpha), angle in degrees between the chord line of the wing and the direction of the relative wind.
		/// FRAME FIELD
		FM_AIRCRAFT_AOA_DEG = 90,
		/// Altitude above ground level in feet.
		/// FRAME FIELD
		FM_AIRCRAFT_AGL_FT = 98,
		/// Specifies whether aircraft is on ground.
		/// FRAME FIELD
		FM_AIRCRAFT_ON_GROUND = 99,
		/// Ground Velocity, in knots.
		/// FRAME FIELD
		FM_AIRCRAFT_GROUND_SPEED_KT = 100,
		/// Wingspan of the aircraft, in feet, measured in a straight line from wingtip to wingtip.
		/// CONFIGURATION FIELD
		FM_AIRCRAFT_WINGSPAN_FT = 102,

		/// Left Flap (trailing-edge) Deployment Level from 0.0 (fully retracted) to 1.0 (fully extended).
		/// FRAME FIELD
		FM_FLAP_LEFT_DEPLOYMENT = 81,
		/// Right Flap (trailing-edge) Deployment Level from 0.0 (fully retracted) to 1.0 (fully extended).
		/// FRAME FIELD
		FM_FLAP_RIGHT_DEPLOYMENT = 82,

		/// Left Slat (leading-edge flap) Deployment Level from 0.0 (fully retracted) to 1.0 (fully extended).
		/// FRAME FIELD
		FM_SLAT_LEFT_DEPLOYMENT = 83,
		/// Right Slat (leading-edge flap) Deployment Level from 0.0 (fully retracted) to 1.0 (fully extended).
		/// FRAME FIELD
		FM_SLAT_RIGHT_DEPLOYMENT = 84,

		/// Left Spoiler Deployment Level from 0.0 (fully retracted) to 1.0 (fully extended).
		/// FRAME FIELD
		FM_SPOILER_LEFT_DEPLOYMENT = 96,
		/// Right Spoiler Deployment Level from 0.0 (fully retracted) to 1.0 (fully extended).
		/// FRAME FIELD
		FM_SPOILER_RIGHT_DEPLOYMENT = 97,

		/// General landing gear deployment level from 0.0 (fully retracted) to 1.0 (fully deployed).
		/// FRAME FIELD
		FM_LANDING_GEAR_GENERAL_DEPLOYMENT = 92,
		/// Specifies if all landing gears are fixed.
		/// CONFIGURATION FIELD
		FM_LANDING_GEAR_ALL_FIXED = 91,


		//------------------------------------------------------------------------
		// Aircraft Detailed Landing Gears Information
		//------------------------------------------------------------------------

		/// Landing gear count, up to 5.
		/// CONFIGURATION FIELD
		FM_LANDING_GEAR_COUNT = 95,
		/// Landing Gear 1 Deployment Level from 0.0 (fully retracted) to 1.0 (fully deployed).
		/// FRAME FIELD
		FM_LANDING_GEAR1_DEPLOYMENT = 127,
		/// Specifies whether landing gear 1 is fixed.
		/// CONFIGURATION FIELD
		FM_LANDING_GEAR1_FIXED = 132,
		/// Xyz vector of landing gear 1 position in meters, relative to [Reference].
		/// CONFIGURATION FIELD
		FM_LANDING_GEAR1_OFFSET_XYZ = 137,
		/// Landing Gear 2 Deployment Level from 0.0 (fully retracted) to 1.0 (fully deployed).
		/// FRAME FIELD
		FM_LANDING_GEAR2_DEPLOYMENT = 128,
		/// Specifies whether landing gear 2 is fixed.
		/// CONFIGURATION FIELD
		FM_LANDING_GEAR2_FIXED = 133,
		/// Xyz vector of landing gear 2 position in meters, relative to [Reference].
		/// CONFIGURATION FIELD
		FM_LANDING_GEAR2_OFFSET_XYZ = 138,
		/// Landing Gear 3 Deployment Level from 0.0 (fully retracted) to 1.0 (fully deployed).
		/// FRAME FIELD
		FM_LANDING_GEAR3_DEPLOYMENT = 129,
		/// Specifies whether landing gear 3 is fixed.
		/// CONFIGURATION FIELD
		FM_LANDING_GEAR3_FIXED = 134,
		/// Xyz vector of landing gear 3 position in meters, relative to [Reference].
		/// CONFIGURATION FIELD
		FM_LANDING_GEAR3_OFFSET_XYZ = 139,
		/// Landing Gear 4 Deployment Level from 0.0 (fully retracted) to 1.0 (fully deployed).
		/// FRAME FIELD
		FM_LANDING_GEAR4_DEPLOYMENT = 130,
		/// Specifies whether landing gear 4 is fixed.
		/// CONFIGURATION FIELD
		FM_LANDING_GEAR4_FIXED = 135,
		/// Xyz vector of landing gear 4 position in meters, relative to [Reference].
		/// CONFIGURATION FIELD
		FM_LANDING_GEAR4_OFFSET_XYZ = 140,
		/// Landing Gear 5 Deployment Level from 0.0 (fully retracted) to 1.0 (fully deployed).
		/// FRAME FIELD
		FM_LANDING_GEAR5_DEPLOYMENT = 131,
		/// Specifies whether landing gear 5 is fixed.
		/// CONFIGURATION FIELD
		FM_LANDING_GEAR5_FIXED = 136,
		/// Xyz vector of landing gear 5 position in meters, relative to [Reference].
		/// CONFIGURATION FIELD
		FM_LANDING_GEAR5_OFFSET_XYZ = 141,


		//------------------------------------------------------------------------
		// Raw Motion Data (for advanced users only)
		//------------------------------------------------------------------------

		/// Left roll value transmitted directly to the Motion Output.
		/// Value should be between -1 and +1: -1 being max leaning to the right and
		/// +1 being max leaning to the left.
		/// FRAME FIELD
		FM_RAW_ROLL = 107,
		/// Forward pitch value transmitted directly to the Motion Output.
		/// Value should be between -1 and +1: -1 being max backward pitch and
		/// +1 being max forward pitch.
		/// FRAME FIELD
		FM_RAW_PITCH = 108,
		/// Up heave value transmitted directly to the Motion Output.
		/// Value should be between -1 and +1: -1 being bottommost position and
		/// +1 being topmost position.
		/// FRAME FIELD
		FM_RAW_HEAVE = 109,
		/// Horizontal Rotational Yaw value transmitted directly to the Motion Output.
		/// Value should be between -1 and +1: -1 with the front rotated leftmost and
		/// +1 with the front rotated rightmost.
		/// FRAME FIELD
		FM_RAW_YAW = 151,
		/// Horizontal Left Sway value transmitted directly to the Motion Output.
		/// Value should be between -1 and +1: -1 Leftmost position and
		/// +1 being rightmost position.
		/// FRAME FIELD
		FM_RAW_SWAY = 152,
		/// Horizontal Front Surge value transmitted directly to the Motion Output.
		/// Value should be between -1 and +1: -1 being the frontmost position and
		/// +1 being backmost position.
		/// FRAME FIELD
		FM_RAW_SURGE = 153,
		/// Front right corner value transmitted directly to the Motion Output.
		/// Value should be between -1 and +1: -1 being max low and +1 being max
		/// high.
		/// FRAME FIELD
		FM_RAW_FRONT_RIGHT = 180,
		/// Front left corner value transmitted directly to the Motion Output.
		/// Value should be between -1 and +1: -1 being max low and +1 being max
		/// high.
		/// FRAME FIELD
		FM_RAW_FRONT_LEFT = 181,
		/// Rear right corner value transmitted directly to the Motion Output.
		/// Value should be between -1 and +1: -1 being max low and +1 being max
		/// high.
		/// FRAME FIELD
		FM_RAW_REAR_RIGHT = 182,
		/// Rear left corner value transmitted directly to the Motion Output.
		/// Value should be between -1 and +1: -1 being max low and +1 being max
		/// high.
		/// FRAME FIELD
		FM_RAW_REAR_LEFT = 183,


		//------------------------------------------------------------------------
		// Custom states
		//------------------------------------------------------------------------

		/// Custom configuration fields when no generic meaning is suitable.
		/// When these are used, their actual meaning should be reported to D-BOX.
		/// CONFIGURATION FIELD
		FM_CONFIG_CUSTOM_01 = 48,
		FM_CONFIG_CUSTOM_02 = 49,
		FM_CONFIG_CUSTOM_03 = 50,
		FM_CONFIG_CUSTOM_04 = 51,
		FM_CONFIG_CUSTOM_05 = 52,
		FM_CONFIG_CUSTOM_06 = 53,
		FM_CONFIG_CUSTOM_07 = 54,
		FM_CONFIG_CUSTOM_08 = 55,
		FM_CONFIG_CUSTOM_09 = 56,
		FM_CONFIG_CUSTOM_10 = 57,

		/// Custom frame fields when no generic meaning is suitable.
		/// When these are used, their actual meaning should be reported to D-BOX.
		/// FRAME FIELD
		FM_FRAME_CUSTOM_01 = 38,
		FM_FRAME_CUSTOM_02 = 39,
		FM_FRAME_CUSTOM_03 = 40,
		FM_FRAME_CUSTOM_04 = 41,
		FM_FRAME_CUSTOM_05 = 42,
		FM_FRAME_CUSTOM_06 = 43,
		FM_FRAME_CUSTOM_07 = 44,
		FM_FRAME_CUSTOM_08 = 45,
		FM_FRAME_CUSTOM_09 = 46,
		FM_FRAME_CUSTOM_10 = 47,


		//========================================================================
		// ACTION FIELDS
		//========================================================================
		// - ACTION fields are used with ACTION events. The list built
		//   with REGISTER and UNREGISTER is global. The associations built with
		//   ASSIGN and UNASSIGN are also global.

		/// Unique numeric identification of action.
		/// Specific case for weapons: Each mode for a given weapon consists of a
		/// different resulting action therefore each of them requires a unique ACTION id
		/// that usually is computed by combining weapon id and mode id.
		/// ACTION FIELD
		FM_ACTION = 101,
		/// Unique string identification of action.
		/// ACTION FIELD
		FM_ACTION_STR = 103,
		/// Action Slot Index, usually 0-3 for 4 slots, 0-7 for 8 slots, etc.
		/// Action loaded in slot is ready to be used at any time (i.e. in-hand weapon).
		/// ACTION FIELD
		FM_ACTION_SLOT = 104,


		//========================================================================
		// EVENT FIELDS
		//========================================================================
		// - EVENT FIELDS are posted when events occur and are immediately processed
		//   by the motion code event handler.  Their state is only valid during
		//   the event local scope.  No global state is maintained.
		//   When a same EVENT FIELD is registered in different event structures,
		//   each posted event will be processed as distinct local values.

		/// Generic intensity attribute for events.
		/// EVENT FIELD
		FM_EVENT_INTENSITY = 24,
		/// Generic direction.
		/// EVENT FIELD
		FM_EVENT_YAW_RAD = 25,
		/// Generic orientation vector relative to body.
		/// EVENT FIELD
		FM_EVENT_ORIENTATION_XYZ = 26,
		/// Generic position coordinates
		/// EVENT FIELD
		FM_EVENT_POSITION_XYZ = 111,

		/// Custom frame fields when no generic meaning is suitable.
		/// When these are used, their actual meaning should be reported to D-BOX.
		/// EVENT FIELD
		FM_EVENT_CUSTOM_01 = 58,
		FM_EVENT_CUSTOM_02 = 59,
		FM_EVENT_CUSTOM_03 = 60,
		FM_EVENT_CUSTOM_04 = 61,
		FM_EVENT_CUSTOM_05 = 62,
		FM_EVENT_CUSTOM_06 = 63,
		FM_EVENT_CUSTOM_07 = 64,
		FM_EVENT_CUSTOM_08 = 65,
		FM_EVENT_CUSTOM_09 = 66,
		FM_EVENT_CUSTOM_10 = 67,

		//------------------------------------------------------------------------
		// Cine Motion Data
		//------------------------------------------------------------------------
		/// Time code (in seconds) used for synchronization.
		/// CINEMOTION FIELD
		FM_CINEMOTION_PLAYBACK_TIME_SEC  = 142,
		/// Obtained exactly the same moment as FM_PLAYBACK_TIME_SEC.
		/// Must be obtained from a high performance counter.
		/// CINEMOTION FIELD
		FM_CINEMOTION_MATCHING_CPU_TIME_SEC = 143,
		/// Path for the CineMotion file. The path may be absolute (eg C:\cinemotionfile)
		/// when the CineMotion files are not in the same location as the program or
		/// relative (eg .\cinemotionfile) when the files are in the same location as the program.
		/// This is a special case, it is recommended to use an ID instead of a path, see next field.
		/// CINEMOTION FIELD
		FM_CINEMOTION_FILE = 144,
		/// Recommended procedure is to use an ID instead of a path
		/// CINEMOTION FIELD
		FM_CINEMOTION_CONTENT_ID = 179,
		/// Configurable gain for CineMotion vibration effects.
		/// Value should be between -60dB and +20dB: 0dB being normal signal.
		/// Values lower than -60 means no effect, use -999 for explicit mute.
		/// CINEMOTION FIELD
		FM_CINEMOTION_GAIN_DB = 145,
		/// Used to identify if a CineMotion file will be played in a loop.
		/// CINEMOTION FIELD
		FM_CINEMOTION_LOOP = 146,
		/// Provided by the user in order to stop playing one or multiple files later.
		/// CINEMOTION FIELD
		FM_CINEMOTION_STOPPABLE_GROUP_ID = 147,
		/// Path for the CineMotion file to preload in memory for future use. The path may be
		/// absolute (eg C:\cinemotionfile) when the CineMotion files are not in the same location
		/// as the program or relative (eg .\cinemotionfile) when the files are in the same location.
		/// CINEMOTION FIELD
		FM_CINEMOTION_PRELOAD_FILE = 149,

		//------------------------------------------------------------------------
		// First Person Shooter Context
		//------------------------------------------------------------------------
		/// Weapon ID (int) when player switches weapon
		/// CONFIGURATION FIELD
		FM_WEAPON_ID = 171,
		/// Distance from player when something happens (explosion, event) in meters
		/// FRAME FIELD
		FM_DISTANCE_FROM_PLAYER_M = 172,
		/// For Intensity and Orientation of Explosion for example
		/// Use FM_EVENT_INTENSITY and FM_EVENT_YAW_RAD
		/// Radius of explosion in meters
		/// FRAME FIELD
		FM_RADIUS_M = 173,
		/// If an explosion has debris, enter the amount here between 0 and 1
		/// FRAME FIELD
		FM_DEBRIS_QTY = 174,
		/// When the player picks up or uses an item
		/// FRAME FIELD
		FM_ITEM_ID = 175,
		/// When the player gets hit, what material gets hit (cloth, metal armor, wooden shield etc...)
		/// FRAME FIELD
		FM_MATERIAL_ID = 176,
		/// When an event is triggered and we want information for the duration of said event
		/// FRAME FIELD
		FM_ISRUNNING = 177,
		/// Generic identifier
		/// FRAME FIELD
		FM_IDENTIFIER = 178,

		/// Internal value, do not use.
		FM_COUNT = 184
	};
}

#endif  // _DBOX_EFIELDMEANING_H_INCLUDED_