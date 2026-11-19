// UpgradeMuxData.h: the upgrade trigger data every upgrade-driven module data
// carries (h05 fpgen). SIZE 0x110. No base, no virtual functions.
// Target facts:
// - RVA 0x004CE29D is `mov eax,0x00C5FEA0; ret`: the getter that 35 module
//   datas' buildFieldParse add at the member's offset (AutoHeal +0x8,
//   RadiateFear +0x20, AttributeModifierAura +0x28, SpawnBehavior +0x5C, ...);
//   UpgradeModuleData is ModuleData + this at +0x8 and ends at 0x118 (k01).
// - The table at VA 0x00C5FEA0: TriggeredBy +0x0 and ConflictsWith +0x80 (both
//   RVA 0x0026F28B), CustomAnimAndDuration +0x100 (INI::iniParseAnimAndDuration),
//   RequiresAllTriggers +0x10C, RequiresAllConflictingTriggers +0x10D and
//   Permanent +0x10E (INI::parseBool).
// - 0x0026F28B memsets 0x80 bytes at its store, then for every upgrade name
//   ORs bit UpgradeTemplate[+0x38] into that 32-word array: a 1024-bit upgrade
//   mask. The constructor (RVA 0x0025342C) builds both masks through RVA
//   0x001EAE6F, which clears 0x80 bytes and returns this, so the mask is a class
//   of its own: UpgradeMaskType (Common/UpgradeMaskType.h).
// - The constructor sets +0x100 to -1 and +0x104/+0x108 to 0, the
//   CustomAnimAndDuration reset, and zeroes the three Bools.
#pragma once
#ifndef BFME2_UPGRADE_MUX_DATA_H
#define BFME2_UPGRADE_MUX_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/INI/FieldParse.h"
#include "Common/UpgradeMaskType.h"
#include "GameLogic/Module/CustomAnimAndDuration.h"

class Object;

class UpgradeMuxData
{
public:
	UpgradeMuxData();								// RVA 0x0025342C
	static const FieldParse *getFieldParse();		// RVA 0x004CE29D
	void rva0045F2D5(UpgradeMaskType &activation, UpgradeMaskType &conflicting) const;	// RVA 0x0045F2D5 (35 B, map: ?rva0045F2D5@UpgradeMuxData@@QBEXAAUUpgradeMaskType@@0@Z): copies both masks out
	void performUpgradeFX(Object *object) const;	// RVA 0x0047A69C, ?performUpgradeFX@UpgradeMuxData@@QBEXPAVObject@@@Z: a `ret 4` body folded with others (f03; ObjectCreationUpgrade 0x004B4075 and RadiateFearUpdate 0x0049C049 call it with ecx = the data's mux and the object pushed)
	void muxDataProcessUpgradeRemoval(Object *object) const;	// RVA 0x0047A69C as well: the upgrade modules' slot-15 bodies (processUpgradeRemoval) call the same `ret 4` fold (Zero Hour's name; publish 3c lists it there)

	UpgradeMaskType m_triggeredBy;					// +0x0 TriggeredBy
	UpgradeMaskType m_conflictsWith;				// +0x80 ConflictsWith
	CustomAnimAndDuration m_customAnimAndDuration;	// +0x100 CustomAnimAndDuration
	Bool m_requiresAllTriggers;						// +0x10C RequiresAllTriggers
	Bool m_requiresAllConflictingTriggers;			// +0x10D RequiresAllConflictingTriggers
	Bool m_permanent;								// +0x10E Permanent
};
// SIZE 0x110
typedef char UpgradeMuxData_size[(sizeof(UpgradeMuxData) == 0x110) ? 1 : -1];
typedef char UpgradeMuxData_off_m_triggeredBy[(offsetof(UpgradeMuxData, m_triggeredBy) == 0x0) ? 1 : -1];
typedef char UpgradeMuxData_off_m_conflictsWith[(offsetof(UpgradeMuxData, m_conflictsWith) == 0x80) ? 1 : -1];
typedef char UpgradeMuxData_off_m_customAnimAndDuration[(offsetof(UpgradeMuxData, m_customAnimAndDuration) == 0x100) ? 1 : -1];
typedef char UpgradeMuxData_off_m_requiresAllTriggers[(offsetof(UpgradeMuxData, m_requiresAllTriggers) == 0x10C) ? 1 : -1];
typedef char UpgradeMuxData_off_m_requiresAllConflictingTriggers[(offsetof(UpgradeMuxData, m_requiresAllConflictingTriggers) == 0x10D) ? 1 : -1];
typedef char UpgradeMuxData_off_m_permanent[(offsetof(UpgradeMuxData, m_permanent) == 0x10E) ? 1 : -1];

#endif
