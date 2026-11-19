// LocomotorTemplate.h: a locomotor's INI definition, from retail's field-parse table
// (VA 0x00BDE2F8; h05 fpgen). Types are what each entry's parse function stores; names are
// the INI tokens. Proved by validate 0x001E38B9 and the dtor 0x001E4DB0 (h07).
// h07: SIZE 0x154 (LocomotorStore::parseLocomotorTemplateDefinition 0x001E8A94 allocates
// 0x154); base Overridable (ctor 0x001E4ABE stores -1 at +0xC, 0 at +4/+8 and table
// 0x00BDE8B4, whose one slot is the deleting dtor; Locomotor's ctor and the store's reset
// call friend_getFinalOverride / deleteOverrides on it). +0x10 is a zeroed word (Zero Hour's
// AsciiString m_name sits there; not proven).
// m_canMoveBackwards (+0xD8) is an Int: parseCanMoveBackwards 0x001E38F9 stores atoi 1..4 or scanBool as a dword.
#pragma once
#ifndef BFME2_LOCOMOTOR_TEMPLATE_H
#define BFME2_LOCOMOTOR_TEMPLATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Overridable.h"
#include "Common/AsciiString.h"


class LocomotorTemplate : public Overridable
{
public:
	LocomotorTemplate();									// 0x001E4ABE
	LocomotorTemplate &operator=(const LocomotorTemplate &that);	// 0x001E3955
	virtual ~LocomotorTemplate();							// 0x001E4DB0 (deleting dtor 0x001E67A3)

	void validate();										// 0x001E38B9

	const AsciiString &getName() const { return m_name; }

	AsciiString m_name;                                     // +0x10; Locomotor::getTemplateName 0x001E4A63 copies it
	UnsignedInt m_surfaces;                                // +0x14 Surfaces; bit string
	Real m_lookAheadMult;                                  // +0x18 LookAheadMult
	Bool m_nonDirtyTransform;                              // +0x1C NonDirtyTransform
	Real m_speed;                                          // +0x20 Speed; velocity
	Real m_speedDamaged;                                   // +0x24 SpeedDamaged; percent
	Real m_minSpeed;                                       // +0x28 MinSpeed; percent
	UnsignedInt m_turnTime;                                // +0x2C TurnTime; duration, frames
	UnsignedInt m_turnTimeDamaged;                         // +0x30 TurnTimeDamaged; duration, frames
	Real m_slowTurnRadius;                                 // +0x34 SlowTurnRadius
	Real m_fastTurnRadius;                                 // +0x38 FastTurnRadius
	Real m_turnThreshold;                                  // +0x3C TurnThreshold; angle
	Real m_turnThresholdHS;                                // +0x40 TurnThresholdHS; angle
	UnsignedInt m_acceleration;                            // +0x44 Acceleration; duration, frames
	Real m_lift;                                           // +0x48 Lift; percent
	Real m_liftDamaged;                                    // +0x4C LiftDamaged; percent
	UnsignedInt m_braking;                                 // +0x50 Braking; duration, frames
	Real m_minTurnSpeed;                                   // +0x54 MinTurnSpeed; percent
	Real m_preferredHeight;                                // +0x58 PreferredHeight
	Real m_preferredAttackHeight;                          // +0x5C PreferredAttackHeight
	Real m_preferredHeightDamping;                         // +0x60 PreferredHeightDamping
	Real m_circlingRadius;                                 // +0x64 CirclingRadius
	Real m_speedLimitZ;                                    // +0x68 SpeedLimitZ; velocity
	Real m_maxThrustAngle;                                 // +0x6C MaxThrustAngle; angle
	Int m_zAxisBehavior;                                   // +0x70 ZAxisBehavior; index into a name list (enum)
	Int m_appearance;                                      // +0x74 Appearance; index into a name list (enum)
	Int m_formationPriority;                               // +0x78 FormationPriority; index into a name list (enum)
	Real m_accDecTrigger;                                  // +0x7C AccDecTrigger
	Real m_walkDistance;                                   // +0x80 WalkDistance
	Real m_maxTurnWithoutReform;                           // +0x84 MaxTurnWithoutReform; angle
	Real m_accelerationPitchLimit;                         // +0x88 AccelerationPitchLimit; angle
	Real m_bounceAmount;                                   // +0x8C BounceAmount; angular velocity
	Real m_pitchStiffness;                                 // +0x90 PitchStiffness
	Real m_rollStiffness;                                  // +0x94 RollStiffness
	Real m_pitchDamping;                                   // +0x98 PitchDamping
	Real m_rollDamping;                                    // +0x9C RollDamping
	Real m_pitchInDirectionOfZVelFactor;                   // +0xA0 PitchInDirectionOfZVelFactor
	Real m_maxOverlappedHeight;                            // +0xA4 MaxOverlappedHeight
	Real m_forwardVelocityPitchFactor;                     // +0xA8 ForwardVelocityPitchFactor
	Real m_lateralVelocityRollFactor;                      // +0xAC LateralVelocityRollFactor
	Real m_forwardAccelerationPitchFactor;                 // +0xB0 ForwardAccelerationPitchFactor
	Real m_lateralAccelerationRollFactor;                  // +0xB4 LateralAccelerationRollFactor
	Real m_uniformAxialDamping;                            // +0xB8 UniformAxialDamping
	Real m_turnPivotOffset;                                // +0xBC TurnPivotOffset
	Int m_airborneTargetingHeight;                         // +0xC0 AirborneTargetingHeight
	Real m_closeEnoughDist;                                // +0xC4 CloseEnoughDist
	Bool m_closeEnoughDist3D;                              // +0xC8 CloseEnoughDist3D
	Real m_slideIntoPlaceTime;                             // +0xCC SlideIntoPlaceTime; duration
	Bool m_locomotorWorksWhenDead;                         // +0xD0 LocomotorWorksWhenDead
	Bool m_allowAirborneMotiveForce;                       // +0xD1 AllowAirborneMotiveForce
	Bool m_apply2DFrictionWhenAirborne;                    // +0xD2 Apply2DFrictionWhenAirborne
	Bool m_downhillOnly;                                   // +0xD3 DownhillOnly
	Bool m_stickToGround;                                  // +0xD4 StickToGround
	UnsignedByte m_unk0xD5[0x3];
	Int m_canMoveBackwards;                                // +0xD8 CanMoveBackwards: 1..4 from atoi, else the Bool (parseCanMoveBackwards 0x001E38F9 stores a dword)
	Bool m_hasSuspension;                                  // +0xDC HasSuspension
	Real m_maximumWheelExtension;                          // +0xE0 MaximumWheelExtension
	Real m_maximumWheelCompression;                        // +0xE4 MaximumWheelCompression
	Real m_frontWheelTurnAngle;                            // +0xE8 FrontWheelTurnAngle; angle
	Bool m_crewPowered;                                    // +0xEC CrewPowered
	Bool m_useTerrainSmoothing;                            // +0xED UseTerrainSmoothing
	Real m_wanderWidthFactor;                              // +0xF0 WanderWidthFactor
	Real m_wanderLengthFactor;                             // +0xF4 WanderLengthFactor
	Real m_wanderAboutPointRadius;                         // +0xF8 WanderAboutPointRadius
	Real m_burningDeathRadius;                             // +0xFC BurningDeathRadius
	Bool m_burningDeathIsCavalry;                          // +0x100 BurningDeathIsCavalry
	Real m_chargeSpeed;                                    // +0x104 ChargeSpeed; percent
	Bool m_chargeAvailable;                                // +0x108 ChargeAvailable
	Bool m_chargeIgnoresCondition;                         // +0x109 ChargeIgnoresCondition
	Bool m_enableHighSpeedTurnModelconditions;             // +0x10A EnableHighSpeedTurnModelconditions
	Bool m_waitForFormation;                               // +0x10B WaitForFormation
	Real m_rudderCorrectionDegree;                         // +0x10C RudderCorrectionDegree
	Real m_rudderCorrectionRate;                           // +0x110 RudderCorrectionRate
	Real m_elevatorCorrectionDegree;                       // +0x114 ElevatorCorrectionDegree
	Real m_elevatorCorrectionRate;                         // +0x118 ElevatorCorrectionRate
	Real m_aeleronCorrectionDegree;                        // +0x11C AeleronCorrectionDegree
	Real m_aeleronCorrectionRate;                          // +0x120 AeleronCorrectionRate
	Real m_swoopStandoffRadius;                            // +0x124 SwoopStandoffRadius
	Real m_swoopStandoffHeight;                            // +0x128 SwoopStandoffHeight
	Real m_swoopTerminalVelocity;                          // +0x12C SwoopTerminalVelocity
	Real m_swoopAccelerationRate;                          // +0x130 SwoopAccelerationRate
	Real m_swoopSpeedTuningFactor;                         // +0x134 SwoopSpeedTuningFactor
	Real m_backingUpSpeed;                                 // +0x138 BackingUpSpeed; percent
	Bool m_backingUpStopWhenTurning;                       // +0x13C BackingUpStopWhenTurning
	Real m_backingUpDistanceMin;                           // +0x140 BackingUpDistanceMin
	Real m_backingUpDistanceMax;                           // +0x144 BackingUpDistanceMax
	Real m_backingUpAngle;                                 // +0x148 BackingUpAngle
	Real m_riverModifier;                                  // +0x14C RiverModifier; percent
	Bool m_scalesWalls;                                    // +0x150 ScalesWalls
	Bool m_turnWhileMoving;                                // +0x151 TurnWhileMoving
};
typedef char LocomotorTemplate_off_m_surfaces[(offsetof(LocomotorTemplate, m_surfaces) == 0x14) ? 1 : -1];
typedef char LocomotorTemplate_off_m_lookAheadMult[(offsetof(LocomotorTemplate, m_lookAheadMult) == 0x18) ? 1 : -1];
typedef char LocomotorTemplate_off_m_nonDirtyTransform[(offsetof(LocomotorTemplate, m_nonDirtyTransform) == 0x1C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_speed[(offsetof(LocomotorTemplate, m_speed) == 0x20) ? 1 : -1];
typedef char LocomotorTemplate_off_m_speedDamaged[(offsetof(LocomotorTemplate, m_speedDamaged) == 0x24) ? 1 : -1];
typedef char LocomotorTemplate_off_m_minSpeed[(offsetof(LocomotorTemplate, m_minSpeed) == 0x28) ? 1 : -1];
typedef char LocomotorTemplate_off_m_turnTime[(offsetof(LocomotorTemplate, m_turnTime) == 0x2C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_turnTimeDamaged[(offsetof(LocomotorTemplate, m_turnTimeDamaged) == 0x30) ? 1 : -1];
typedef char LocomotorTemplate_off_m_slowTurnRadius[(offsetof(LocomotorTemplate, m_slowTurnRadius) == 0x34) ? 1 : -1];
typedef char LocomotorTemplate_off_m_fastTurnRadius[(offsetof(LocomotorTemplate, m_fastTurnRadius) == 0x38) ? 1 : -1];
typedef char LocomotorTemplate_off_m_turnThreshold[(offsetof(LocomotorTemplate, m_turnThreshold) == 0x3C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_turnThresholdHS[(offsetof(LocomotorTemplate, m_turnThresholdHS) == 0x40) ? 1 : -1];
typedef char LocomotorTemplate_off_m_acceleration[(offsetof(LocomotorTemplate, m_acceleration) == 0x44) ? 1 : -1];
typedef char LocomotorTemplate_off_m_lift[(offsetof(LocomotorTemplate, m_lift) == 0x48) ? 1 : -1];
typedef char LocomotorTemplate_off_m_liftDamaged[(offsetof(LocomotorTemplate, m_liftDamaged) == 0x4C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_braking[(offsetof(LocomotorTemplate, m_braking) == 0x50) ? 1 : -1];
typedef char LocomotorTemplate_off_m_minTurnSpeed[(offsetof(LocomotorTemplate, m_minTurnSpeed) == 0x54) ? 1 : -1];
typedef char LocomotorTemplate_off_m_preferredHeight[(offsetof(LocomotorTemplate, m_preferredHeight) == 0x58) ? 1 : -1];
typedef char LocomotorTemplate_off_m_preferredAttackHeight[(offsetof(LocomotorTemplate, m_preferredAttackHeight) == 0x5C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_preferredHeightDamping[(offsetof(LocomotorTemplate, m_preferredHeightDamping) == 0x60) ? 1 : -1];
typedef char LocomotorTemplate_off_m_circlingRadius[(offsetof(LocomotorTemplate, m_circlingRadius) == 0x64) ? 1 : -1];
typedef char LocomotorTemplate_off_m_speedLimitZ[(offsetof(LocomotorTemplate, m_speedLimitZ) == 0x68) ? 1 : -1];
typedef char LocomotorTemplate_off_m_maxThrustAngle[(offsetof(LocomotorTemplate, m_maxThrustAngle) == 0x6C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_zAxisBehavior[(offsetof(LocomotorTemplate, m_zAxisBehavior) == 0x70) ? 1 : -1];
typedef char LocomotorTemplate_off_m_appearance[(offsetof(LocomotorTemplate, m_appearance) == 0x74) ? 1 : -1];
typedef char LocomotorTemplate_off_m_formationPriority[(offsetof(LocomotorTemplate, m_formationPriority) == 0x78) ? 1 : -1];
typedef char LocomotorTemplate_off_m_accDecTrigger[(offsetof(LocomotorTemplate, m_accDecTrigger) == 0x7C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_walkDistance[(offsetof(LocomotorTemplate, m_walkDistance) == 0x80) ? 1 : -1];
typedef char LocomotorTemplate_off_m_maxTurnWithoutReform[(offsetof(LocomotorTemplate, m_maxTurnWithoutReform) == 0x84) ? 1 : -1];
typedef char LocomotorTemplate_off_m_accelerationPitchLimit[(offsetof(LocomotorTemplate, m_accelerationPitchLimit) == 0x88) ? 1 : -1];
typedef char LocomotorTemplate_off_m_bounceAmount[(offsetof(LocomotorTemplate, m_bounceAmount) == 0x8C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_pitchStiffness[(offsetof(LocomotorTemplate, m_pitchStiffness) == 0x90) ? 1 : -1];
typedef char LocomotorTemplate_off_m_rollStiffness[(offsetof(LocomotorTemplate, m_rollStiffness) == 0x94) ? 1 : -1];
typedef char LocomotorTemplate_off_m_pitchDamping[(offsetof(LocomotorTemplate, m_pitchDamping) == 0x98) ? 1 : -1];
typedef char LocomotorTemplate_off_m_rollDamping[(offsetof(LocomotorTemplate, m_rollDamping) == 0x9C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_pitchInDirectionOfZVelFactor[(offsetof(LocomotorTemplate, m_pitchInDirectionOfZVelFactor) == 0xA0) ? 1 : -1];
typedef char LocomotorTemplate_off_m_maxOverlappedHeight[(offsetof(LocomotorTemplate, m_maxOverlappedHeight) == 0xA4) ? 1 : -1];
typedef char LocomotorTemplate_off_m_forwardVelocityPitchFactor[(offsetof(LocomotorTemplate, m_forwardVelocityPitchFactor) == 0xA8) ? 1 : -1];
typedef char LocomotorTemplate_off_m_lateralVelocityRollFactor[(offsetof(LocomotorTemplate, m_lateralVelocityRollFactor) == 0xAC) ? 1 : -1];
typedef char LocomotorTemplate_off_m_forwardAccelerationPitchFactor[(offsetof(LocomotorTemplate, m_forwardAccelerationPitchFactor) == 0xB0) ? 1 : -1];
typedef char LocomotorTemplate_off_m_lateralAccelerationRollFactor[(offsetof(LocomotorTemplate, m_lateralAccelerationRollFactor) == 0xB4) ? 1 : -1];
typedef char LocomotorTemplate_off_m_uniformAxialDamping[(offsetof(LocomotorTemplate, m_uniformAxialDamping) == 0xB8) ? 1 : -1];
typedef char LocomotorTemplate_off_m_turnPivotOffset[(offsetof(LocomotorTemplate, m_turnPivotOffset) == 0xBC) ? 1 : -1];
typedef char LocomotorTemplate_off_m_airborneTargetingHeight[(offsetof(LocomotorTemplate, m_airborneTargetingHeight) == 0xC0) ? 1 : -1];
typedef char LocomotorTemplate_off_m_closeEnoughDist[(offsetof(LocomotorTemplate, m_closeEnoughDist) == 0xC4) ? 1 : -1];
typedef char LocomotorTemplate_off_m_closeEnoughDist3D[(offsetof(LocomotorTemplate, m_closeEnoughDist3D) == 0xC8) ? 1 : -1];
typedef char LocomotorTemplate_off_m_slideIntoPlaceTime[(offsetof(LocomotorTemplate, m_slideIntoPlaceTime) == 0xCC) ? 1 : -1];
typedef char LocomotorTemplate_off_m_locomotorWorksWhenDead[(offsetof(LocomotorTemplate, m_locomotorWorksWhenDead) == 0xD0) ? 1 : -1];
typedef char LocomotorTemplate_off_m_allowAirborneMotiveForce[(offsetof(LocomotorTemplate, m_allowAirborneMotiveForce) == 0xD1) ? 1 : -1];
typedef char LocomotorTemplate_off_m_apply2DFrictionWhenAirborne[(offsetof(LocomotorTemplate, m_apply2DFrictionWhenAirborne) == 0xD2) ? 1 : -1];
typedef char LocomotorTemplate_off_m_downhillOnly[(offsetof(LocomotorTemplate, m_downhillOnly) == 0xD3) ? 1 : -1];
typedef char LocomotorTemplate_off_m_stickToGround[(offsetof(LocomotorTemplate, m_stickToGround) == 0xD4) ? 1 : -1];
typedef char LocomotorTemplate_off_m_canMoveBackwards[(offsetof(LocomotorTemplate, m_canMoveBackwards) == 0xD8) ? 1 : -1];
typedef char LocomotorTemplate_off_m_hasSuspension[(offsetof(LocomotorTemplate, m_hasSuspension) == 0xDC) ? 1 : -1];
typedef char LocomotorTemplate_off_m_maximumWheelExtension[(offsetof(LocomotorTemplate, m_maximumWheelExtension) == 0xE0) ? 1 : -1];
typedef char LocomotorTemplate_off_m_maximumWheelCompression[(offsetof(LocomotorTemplate, m_maximumWheelCompression) == 0xE4) ? 1 : -1];
typedef char LocomotorTemplate_off_m_frontWheelTurnAngle[(offsetof(LocomotorTemplate, m_frontWheelTurnAngle) == 0xE8) ? 1 : -1];
typedef char LocomotorTemplate_off_m_crewPowered[(offsetof(LocomotorTemplate, m_crewPowered) == 0xEC) ? 1 : -1];
typedef char LocomotorTemplate_off_m_useTerrainSmoothing[(offsetof(LocomotorTemplate, m_useTerrainSmoothing) == 0xED) ? 1 : -1];
typedef char LocomotorTemplate_off_m_wanderWidthFactor[(offsetof(LocomotorTemplate, m_wanderWidthFactor) == 0xF0) ? 1 : -1];
typedef char LocomotorTemplate_off_m_wanderLengthFactor[(offsetof(LocomotorTemplate, m_wanderLengthFactor) == 0xF4) ? 1 : -1];
typedef char LocomotorTemplate_off_m_wanderAboutPointRadius[(offsetof(LocomotorTemplate, m_wanderAboutPointRadius) == 0xF8) ? 1 : -1];
typedef char LocomotorTemplate_off_m_burningDeathRadius[(offsetof(LocomotorTemplate, m_burningDeathRadius) == 0xFC) ? 1 : -1];
typedef char LocomotorTemplate_off_m_burningDeathIsCavalry[(offsetof(LocomotorTemplate, m_burningDeathIsCavalry) == 0x100) ? 1 : -1];
typedef char LocomotorTemplate_off_m_chargeSpeed[(offsetof(LocomotorTemplate, m_chargeSpeed) == 0x104) ? 1 : -1];
typedef char LocomotorTemplate_off_m_chargeAvailable[(offsetof(LocomotorTemplate, m_chargeAvailable) == 0x108) ? 1 : -1];
typedef char LocomotorTemplate_off_m_chargeIgnoresCondition[(offsetof(LocomotorTemplate, m_chargeIgnoresCondition) == 0x109) ? 1 : -1];
typedef char LocomotorTemplate_off_m_enableHighSpeedTurnModelconditions[(offsetof(LocomotorTemplate, m_enableHighSpeedTurnModelconditions) == 0x10A) ? 1 : -1];
typedef char LocomotorTemplate_off_m_waitForFormation[(offsetof(LocomotorTemplate, m_waitForFormation) == 0x10B) ? 1 : -1];
typedef char LocomotorTemplate_off_m_rudderCorrectionDegree[(offsetof(LocomotorTemplate, m_rudderCorrectionDegree) == 0x10C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_rudderCorrectionRate[(offsetof(LocomotorTemplate, m_rudderCorrectionRate) == 0x110) ? 1 : -1];
typedef char LocomotorTemplate_off_m_elevatorCorrectionDegree[(offsetof(LocomotorTemplate, m_elevatorCorrectionDegree) == 0x114) ? 1 : -1];
typedef char LocomotorTemplate_off_m_elevatorCorrectionRate[(offsetof(LocomotorTemplate, m_elevatorCorrectionRate) == 0x118) ? 1 : -1];
typedef char LocomotorTemplate_off_m_aeleronCorrectionDegree[(offsetof(LocomotorTemplate, m_aeleronCorrectionDegree) == 0x11C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_aeleronCorrectionRate[(offsetof(LocomotorTemplate, m_aeleronCorrectionRate) == 0x120) ? 1 : -1];
typedef char LocomotorTemplate_off_m_swoopStandoffRadius[(offsetof(LocomotorTemplate, m_swoopStandoffRadius) == 0x124) ? 1 : -1];
typedef char LocomotorTemplate_off_m_swoopStandoffHeight[(offsetof(LocomotorTemplate, m_swoopStandoffHeight) == 0x128) ? 1 : -1];
typedef char LocomotorTemplate_off_m_swoopTerminalVelocity[(offsetof(LocomotorTemplate, m_swoopTerminalVelocity) == 0x12C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_swoopAccelerationRate[(offsetof(LocomotorTemplate, m_swoopAccelerationRate) == 0x130) ? 1 : -1];
typedef char LocomotorTemplate_off_m_swoopSpeedTuningFactor[(offsetof(LocomotorTemplate, m_swoopSpeedTuningFactor) == 0x134) ? 1 : -1];
typedef char LocomotorTemplate_off_m_backingUpSpeed[(offsetof(LocomotorTemplate, m_backingUpSpeed) == 0x138) ? 1 : -1];
typedef char LocomotorTemplate_off_m_backingUpStopWhenTurning[(offsetof(LocomotorTemplate, m_backingUpStopWhenTurning) == 0x13C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_backingUpDistanceMin[(offsetof(LocomotorTemplate, m_backingUpDistanceMin) == 0x140) ? 1 : -1];
typedef char LocomotorTemplate_off_m_backingUpDistanceMax[(offsetof(LocomotorTemplate, m_backingUpDistanceMax) == 0x144) ? 1 : -1];
typedef char LocomotorTemplate_off_m_backingUpAngle[(offsetof(LocomotorTemplate, m_backingUpAngle) == 0x148) ? 1 : -1];
typedef char LocomotorTemplate_off_m_riverModifier[(offsetof(LocomotorTemplate, m_riverModifier) == 0x14C) ? 1 : -1];
typedef char LocomotorTemplate_off_m_scalesWalls[(offsetof(LocomotorTemplate, m_scalesWalls) == 0x150) ? 1 : -1];
typedef char LocomotorTemplate_off_m_turnWhileMoving[(offsetof(LocomotorTemplate, m_turnWhileMoving) == 0x151) ? 1 : -1];

typedef char LocomotorTemplate_size[(sizeof(LocomotorTemplate) == 0x154) ? 1 : -1];

#endif
