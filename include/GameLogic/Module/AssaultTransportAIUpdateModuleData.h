// AssaultTransportAIUpdateModuleData.h: the INI data of AssaultTransportAIUpdate. SIZE 0x6C.
// Target facts: ModuleFactory registers the module with operator new 0x6C and
// constructor RVA 0x0024D221; buildFieldParse RVA 0x0024D248 calls AIUpdateModuleData::buildFieldParse, then adds VA 0x00BEED74 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_ASSAULT_TRANSPORT_A_I_UPDATE_MODULE_DATA_H
#define BFME2_ASSAULT_TRANSPORT_A_I_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/AIUpdate.h"

class INI;
class MultiIniFieldParse;

// Base AIUpdateModuleData: the constructor calls its constructor on this. It ends at 0x64 (its own operator-new size);
// this class's members follow it.
typedef char AssaultTransportAIUpdateModuleData_base_size[(sizeof(AIUpdateModuleData) == 0x64) ? 1 : -1];

class AssaultTransportAIUpdateModuleData : public AIUpdateModuleData
{
public:
	AssaultTransportAIUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_membersGetHealedAtLifeRatio;                    // +0x64 MembersGetHealedAtLifeRatio
	Real m_clearRangeRequiredToContinueAttackMove;         // +0x68 ClearRangeRequiredToContinueAttackMove
};
// SIZE 0x6C
typedef char AssaultTransportAIUpdateModuleData_size[(sizeof(AssaultTransportAIUpdateModuleData) == 0x6C) ? 1 : -1];
typedef char AssaultTransportAIUpdateModuleData_off_m_membersGetHealedAtLifeRatio[(offsetof(AssaultTransportAIUpdateModuleData, m_membersGetHealedAtLifeRatio) == 0x64) ? 1 : -1];
typedef char AssaultTransportAIUpdateModuleData_off_m_clearRangeRequiredToContinueAttackMove[(offsetof(AssaultTransportAIUpdateModuleData, m_clearRangeRequiredToContinueAttackMove) == 0x68) ? 1 : -1];

#endif
