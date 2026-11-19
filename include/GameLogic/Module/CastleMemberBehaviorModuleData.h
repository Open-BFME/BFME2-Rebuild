// CastleMemberBehaviorModuleData.h: the INI data of CastleMemberBehavior. SIZE 0x1C.
// Target facts: ModuleFactory registers the module with operator new 0x1C and
// constructor RVA 0x00395B03; buildFieldParse RVA 0x00395AF2 adds VA 0x00C1A310.
// The constructor stores the Eva event defaults 9 / 0xA / 8, a null BeingBuiltSound
// and two false flags; the destructor releases the sound handle at +0x14 through
// Release_Ref (0x00050ED3), so it is the four-byte OpaqueRefElement4 handle.
#pragma once
#ifndef BFME2_CASTLE_MEMBER_BEHAVIOR_MODULE_DATA_H
#define BFME2_CASTLE_MEMBER_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/OpaqueRefElement4.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class CastleMemberBehaviorModuleData : public ModuleData
{
public:
	CastleMemberBehaviorModuleData();
	virtual ~CastleMemberBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Int m_campDestroyedOwnerEvaEvent;		// +0x08 CampDestroyedOwnerEvaEvent
	Int m_campDestroyedAllyEvaEvent;		// +0x0C CampDestroyedAllyEvaEvent
	Int m_campDestroyedAttackerEvaEvent;	// +0x10 CampDestroyedAttackerEvaEvent
	OpaqueRefElement4 m_beingBuiltSound;	// +0x14 BeingBuiltSound
	Bool m_storeUpgradePrice;				// +0x18 StoreUpgradePrice
	Bool m_countsForEvaCastleBreached;		// +0x19 CountsForEvaCastleBreached
};
// SIZE 0x1C
typedef char CastleMemberBehaviorModuleData_size[(sizeof(CastleMemberBehaviorModuleData) == 0x1C) ? 1 : -1];
typedef char CastleMemberBehaviorModuleData_off_owner[(offsetof(CastleMemberBehaviorModuleData, m_campDestroyedOwnerEvaEvent) == 0x8) ? 1 : -1];
typedef char CastleMemberBehaviorModuleData_off_sound[(offsetof(CastleMemberBehaviorModuleData, m_beingBuiltSound) == 0x14) ? 1 : -1];

#endif
