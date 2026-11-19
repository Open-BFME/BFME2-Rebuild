// PhysicsBehaviorModuleData.h: the physics behaviour's INI data. SIZE 0x5C.
// Target facts: ctor 0x00390119 (factory 0x0024E774 news 0x5C) and the retail
// INI table at 0x00C19DE8, which names every field and offset below.
// buildFieldParse 0x003901C9 registers that table with offset 0.
#pragma once
#ifndef BFME2_PHYSICSBEHAVIORMODULEDATA_H
#define BFME2_PHYSICSBEHAVIORMODULEDATA_H

#include "GameLogic/Module/ModuleData.h"

class MultiIniFieldParse;

class PhysicsBehaviorModuleData : public ModuleData
{
public:
	PhysicsBehaviorModuleData();

	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_firstHeight;						// 0x08
	Real m_secondHeight;					// 0x0C
	Real m_firstPercentIndent;				// 0x10
	Real m_secondPercentIndent;				// 0x14
	Int m_shockStunnedTimeLow;				// 0x18
	Int m_shockStunnedTimeHigh;				// 0x1C
	Int m_shockStandingTime;				// 0x20
	Int m_bounceCount;						// 0x24
	Real m_unk0x28;							// 0x28, no INI field
	Real m_bounceFirstHeight;				// 0x2C
	Real m_bounceSecondHeight;				// 0x30
	Real m_bounceFirstPercentIndent;		// 0x34
	Real m_bounceSecondPercentIndent;		// 0x38
	Real m_curveFlattenMinDist;				// 0x3C
	Bool m_tumbleRandomly;					// 0x40
	Bool m_orientToFlightPath;				// 0x41
	Bool m_ignoreTerrainHeight;				// 0x42
	UnsignedByte m_unk0x43;
	Real m_firstPercentHeight;				// 0x44
	Real m_secondPercentHeight;				// 0x48
	Real m_gravityMult;						// 0x4C
	void *m_groundHitFX;					// 0x50
	void *m_groundBounceFX;					// 0x54
	Bool m_allowBouncing;					// 0x58
	Bool m_killWhenRestingOnGround;			// 0x59
};
// SIZE 0x5C

#endif
