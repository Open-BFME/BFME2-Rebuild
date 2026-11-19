// GiantBirdAIUpdateModuleData.h: the INI data of GiantBirdAIUpdate. SIZE 0x84.
// Target facts: ModuleFactory registers the module with operator new 0x84 and
// constructor RVA 0x0036812E; buildFieldParse RVA 0x00368182 calls AIUpdateModuleData::buildFieldParse, then adds VA 0x00C178D8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_GIANT_BIRD_A_I_UPDATE_MODULE_DATA_H
#define BFME2_GIANT_BIRD_A_I_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/AIUpdate.h"

class INI;
class MultiIniFieldParse;
class FXList;

// Base AIUpdateModuleData: the constructor calls its constructor on this. It ends at 0x64 (its own operator-new size);
// this class's members follow it.
typedef char GiantBirdAIUpdateModuleData_base_size[(sizeof(AIUpdateModuleData) == 0x64) ? 1 : -1];

class GiantBirdAIUpdateModuleData : public AIUpdateModuleData
{
public:
	GiantBirdAIUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Int m_attackLocomotorType;                             // +0x64 AttackLocomotorType; index into a name list (enum)
	Int m_returnForAmmoLocomotorType;                      // +0x68 ReturnForAmmoLocomotorType; index into a name list (enum)
	Real m_grabTossTimeTrigger;                            // +0x6C GrabTossTimeTrigger
	Real m_grabTossHeightTrigger;                          // +0x70 GrabTossHeightTrigger
	Real m_followThroughDistance;                          // +0x74 FollowThroughDistance
	Real m_followThroughCheckStep;                         // +0x78 FollowThroughCheckStep
	Real m_followThroughGradient;                          // +0x7C FollowThroughGradient
	const FXList * m_tossFX;                               // +0x80 TossFX
};
// SIZE 0x84
typedef char GiantBirdAIUpdateModuleData_size[(sizeof(GiantBirdAIUpdateModuleData) == 0x84) ? 1 : -1];
typedef char GiantBirdAIUpdateModuleData_off_m_attackLocomotorType[(offsetof(GiantBirdAIUpdateModuleData, m_attackLocomotorType) == 0x64) ? 1 : -1];
typedef char GiantBirdAIUpdateModuleData_off_m_returnForAmmoLocomotorType[(offsetof(GiantBirdAIUpdateModuleData, m_returnForAmmoLocomotorType) == 0x68) ? 1 : -1];
typedef char GiantBirdAIUpdateModuleData_off_m_grabTossTimeTrigger[(offsetof(GiantBirdAIUpdateModuleData, m_grabTossTimeTrigger) == 0x6C) ? 1 : -1];
typedef char GiantBirdAIUpdateModuleData_off_m_grabTossHeightTrigger[(offsetof(GiantBirdAIUpdateModuleData, m_grabTossHeightTrigger) == 0x70) ? 1 : -1];
typedef char GiantBirdAIUpdateModuleData_off_m_followThroughDistance[(offsetof(GiantBirdAIUpdateModuleData, m_followThroughDistance) == 0x74) ? 1 : -1];
typedef char GiantBirdAIUpdateModuleData_off_m_followThroughCheckStep[(offsetof(GiantBirdAIUpdateModuleData, m_followThroughCheckStep) == 0x78) ? 1 : -1];
typedef char GiantBirdAIUpdateModuleData_off_m_followThroughGradient[(offsetof(GiantBirdAIUpdateModuleData, m_followThroughGradient) == 0x7C) ? 1 : -1];
typedef char GiantBirdAIUpdateModuleData_off_m_tossFX[(offsetof(GiantBirdAIUpdateModuleData, m_tossFX) == 0x80) ? 1 : -1];

#endif
