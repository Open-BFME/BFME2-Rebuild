// HeightDieUpdateModuleData.h: the height-die module's INI data.
//
// Target facts: the field-parse table at 0x00C4CFC8 gives the members —
// TargetHeight (parseReal, +0x08), TargetHeightIncludesStructures (parseBool,
// +0x0C), OnlyWhenMovingDown (parseBool, +0x0D),
// DestroyAttachedParticlesAtHeight (parseReal, +0x10), SnapToGroundOnDeath
// (parseBool, +0x14) and InitialDelay (parseDurationUnsignedInt, +0x18) — and
// the constructor 0x0048FC75 seeds 0, false, false, -1.0f, false, 0.
#pragma once
#ifndef BFME2_HEIGHTDIEUPDATEMODULEDATA_H
#define BFME2_HEIGHTDIEUPDATEMODULEDATA_H

#include "GameLogic/Module/ModuleData.h"

class MultiIniFieldParse;

class HeightDieUpdateModuleData : public ModuleData
{
public:
	HeightDieUpdateModuleData();

	static void buildFieldParse(MultiIniFieldParse &p);

	Real TargetHeight;				// +0x08
	Bool TargetHeightIncludesStructures;		// +0x0C
	Bool OnlyWhenMovingDown;			// +0x0D
	Real DestroyAttachedParticlesAtHeight;		// +0x10
	Bool SnapToGroundOnDeath;			// +0x14
	UnsignedInt InitialDelay;			// +0x18
};

#endif
