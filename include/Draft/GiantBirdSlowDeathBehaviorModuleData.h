// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C42D08).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// GiantBirdSlowDeathBehaviorModuleData.h: the INI data of GiantBirdSlowDeathBehavior. SIZE 0x1C4.
// Target facts: ModuleFactory registers the module with operator new 0x1C4 and
// constructor RVA 0x00461F7F; buildFieldParse RVA 0x00461E3D calls SlowDeathBehaviorModuleData::buildFieldParse, then adds VA 0x00C42D08 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_GIANT_BIRD_SLOW_DEATH_BEHAVIOR_MODULE_DATA_H
#define BFME2_GIANT_BIRD_SLOW_DEATH_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/SlowDeathBehaviorModuleData.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;
class FXList;
class ObjectCreationList;

// Base SlowDeathBehaviorModuleData: the constructor calls its constructor on this. It ends at 0x190 (its own operator-new size);
// this class's members follow it.
typedef char GiantBirdSlowDeathBehaviorModuleData_base_size[(sizeof(SlowDeathBehaviorModuleData) == 0x190) ? 1 : -1];

class GiantBirdSlowDeathBehaviorModuleData : public SlowDeathBehaviorModuleData
{
public:
	GiantBirdSlowDeathBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	const FXList * m_fxHitGround;                          // +0x190 FXHitGround
	const ObjectCreationList * m_oclHitGround;             // +0x194 OCLHitGround
	Real m_delayFromGroundToFinalDeath;                    // +0x198 DelayFromGroundToFinalDeath; duration
	KindOfMaskType m_crashAvoidKindOfs;                    // +0x19C CrashAvoidKindOfs; BitFlags<218>
	Real m_crashAvoidRadius;                               // +0x1B8 CrashAvoidRadius
	Real m_crashAvoidStrength;                             // +0x1BC CrashAvoidStrength
	Bool m_needToMaintainFlailingHeight;                   // +0x1C0 NeedToMaintainFlailingHeight
};
// SIZE 0x1C4
typedef char GiantBirdSlowDeathBehaviorModuleData_size[(sizeof(GiantBirdSlowDeathBehaviorModuleData) == 0x1C4) ? 1 : -1];
typedef char GiantBirdSlowDeathBehaviorModuleData_off_m_fxHitGround[(offsetof(GiantBirdSlowDeathBehaviorModuleData, m_fxHitGround) == 0x190) ? 1 : -1];
typedef char GiantBirdSlowDeathBehaviorModuleData_off_m_oclHitGround[(offsetof(GiantBirdSlowDeathBehaviorModuleData, m_oclHitGround) == 0x194) ? 1 : -1];
typedef char GiantBirdSlowDeathBehaviorModuleData_off_m_delayFromGroundToFinalDeath[(offsetof(GiantBirdSlowDeathBehaviorModuleData, m_delayFromGroundToFinalDeath) == 0x198) ? 1 : -1];
typedef char GiantBirdSlowDeathBehaviorModuleData_off_m_crashAvoidKindOfs[(offsetof(GiantBirdSlowDeathBehaviorModuleData, m_crashAvoidKindOfs) == 0x19C) ? 1 : -1];
typedef char GiantBirdSlowDeathBehaviorModuleData_off_m_crashAvoidRadius[(offsetof(GiantBirdSlowDeathBehaviorModuleData, m_crashAvoidRadius) == 0x1B8) ? 1 : -1];
typedef char GiantBirdSlowDeathBehaviorModuleData_off_m_crashAvoidStrength[(offsetof(GiantBirdSlowDeathBehaviorModuleData, m_crashAvoidStrength) == 0x1BC) ? 1 : -1];
typedef char GiantBirdSlowDeathBehaviorModuleData_off_m_needToMaintainFlailingHeight[(offsetof(GiantBirdSlowDeathBehaviorModuleData, m_needToMaintainFlailingHeight) == 0x1C0) ? 1 : -1];

#endif
