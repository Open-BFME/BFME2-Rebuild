// AnimalAIUpdateModuleData.h: the INI data of AnimalAIUpdate. SIZE 0x80.
// Target facts: ModuleFactory registers the module with operator new 0x80 and
// constructor RVA 0x0024BE43; buildFieldParse RVA 0x0024BE7A calls AIUpdateModuleData::buildFieldParse, then adds VA 0x00BEEB88 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_ANIMAL_A_I_UPDATE_MODULE_DATA_H
#define BFME2_ANIMAL_A_I_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/AIUpdate.h"

class INI;
class MultiIniFieldParse;

// Base AIUpdateModuleData: the constructor calls its constructor on this. It ends at 0x64 (its own operator-new size);
// this class's members follow it.
typedef char AnimalAIUpdateModuleData_base_size[(sizeof(AIUpdateModuleData) == 0x64) ? 1 : -1];

class AnimalAIUpdateModuleData : public AIUpdateModuleData
{
public:
	AnimalAIUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Int m_fleeRange;                                       // +0x64 FleeRange
	Int m_fleeDistance;                                    // +0x68 FleeDistance
	Int m_wanderPercentage;                                // +0x6C WanderPercentage
	Int m_maxWanderDistance;                               // +0x70 MaxWanderDistance
	Int m_maxWanderRadius;                                 // +0x74 MaxWanderRadius
	Int m_updateTimer;                                     // +0x78 UpdateTimer
	Bool m_afraidOfCastles;                                // +0x7C AfraidOfCastles
};
// SIZE 0x80
typedef char AnimalAIUpdateModuleData_size[(sizeof(AnimalAIUpdateModuleData) == 0x80) ? 1 : -1];
typedef char AnimalAIUpdateModuleData_off_m_fleeRange[(offsetof(AnimalAIUpdateModuleData, m_fleeRange) == 0x64) ? 1 : -1];
typedef char AnimalAIUpdateModuleData_off_m_fleeDistance[(offsetof(AnimalAIUpdateModuleData, m_fleeDistance) == 0x68) ? 1 : -1];
typedef char AnimalAIUpdateModuleData_off_m_wanderPercentage[(offsetof(AnimalAIUpdateModuleData, m_wanderPercentage) == 0x6C) ? 1 : -1];
typedef char AnimalAIUpdateModuleData_off_m_maxWanderDistance[(offsetof(AnimalAIUpdateModuleData, m_maxWanderDistance) == 0x70) ? 1 : -1];
typedef char AnimalAIUpdateModuleData_off_m_maxWanderRadius[(offsetof(AnimalAIUpdateModuleData, m_maxWanderRadius) == 0x74) ? 1 : -1];
typedef char AnimalAIUpdateModuleData_off_m_updateTimer[(offsetof(AnimalAIUpdateModuleData, m_updateTimer) == 0x78) ? 1 : -1];
typedef char AnimalAIUpdateModuleData_off_m_afraidOfCastles[(offsetof(AnimalAIUpdateModuleData, m_afraidOfCastles) == 0x7C) ? 1 : -1];

#endif
