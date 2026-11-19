// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C4DC48).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_parentOwnerAttachmentEvaEvent, m_parentAllyAttachmentEvaEvent, m_parentEnemyAttachmentEvaEvent, m_parentOwnerDiedEvaEvent, m_parentAllyDiedEvaEvent, m_parentEnemyDiedtEvaEvent.
// AttachUpdateModuleData.h: the INI data of AttachUpdate. SIZE 0x40.
// Target facts: ModuleFactory registers the module with operator new 0x40 and
// constructor RVA 0x00491968; buildFieldParse RVA 0x00491957 adds VA 0x00C4DC48 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_parentOwnerAttachmentEvaEvent +0x24: Rva001DEE74Parse 0x001DEE74
//   m_parentAllyAttachmentEvaEvent +0x28: Rva001DEE74Parse 0x001DEE74
//   m_parentEnemyAttachmentEvaEvent +0x2C: Rva001DEE74Parse 0x001DEE74
//   m_parentOwnerDiedEvaEvent +0x34: Rva001DEE74Parse 0x001DEE74
//   m_parentAllyDiedEvaEvent +0x38: Rva001DEE74Parse 0x001DEE74
//   m_parentEnemyDiedtEvaEvent +0x3C: Rva001DEE74Parse 0x001DEE74
#pragma once
#ifndef BFME2_ATTACH_UPDATE_MODULE_DATA_H
#define BFME2_ATTACH_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;
class FXList;

class AttachUpdateModuleData : public ModuleData
{
public:
	AttachUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	ObjectFilter m_objectFilter;                           // +0x8 ObjectFilter
	ObjectStatusMaskType m_parentStatus;                   // +0xC ParentStatus; BitFlags<101>
	Real m_scanRange;                                      // +0x1C ScanRange
	Bool m_alwaysTeleport;                                 // +0x20 AlwaysTeleport
	Bool m_anchorToTopOfGeometry;                          // +0x21 AnchorToTopOfGeometry
	UnsignedByte m_unk0x22[0x2];
	UnsignedByte m_parentOwnerAttachmentEvaEvent[0x4];     // +0x24 ParentOwnerAttachmentEvaEvent; unresolved: Rva001DEE74Parse
	UnsignedByte m_parentAllyAttachmentEvaEvent[0x4];      // +0x28 ParentAllyAttachmentEvaEvent; unresolved: Rva001DEE74Parse
	UnsignedByte m_parentEnemyAttachmentEvaEvent[0x4];     // +0x2C ParentEnemyAttachmentEvaEvent; unresolved: Rva001DEE74Parse
	const FXList * m_attachFX;                             // +0x30 AttachFX
	UnsignedByte m_parentOwnerDiedEvaEvent[0x4];           // +0x34 ParentOwnerDiedEvaEvent; unresolved: Rva001DEE74Parse
	UnsignedByte m_parentAllyDiedEvaEvent[0x4];            // +0x38 ParentAllyDiedEvaEvent; unresolved: Rva001DEE74Parse
	UnsignedByte m_parentEnemyDiedtEvaEvent[0x4];          // +0x3C ParentEnemyDiedtEvaEvent; unresolved: Rva001DEE74Parse
};
// SIZE 0x40
typedef char AttachUpdateModuleData_size[(sizeof(AttachUpdateModuleData) == 0x40) ? 1 : -1];
typedef char AttachUpdateModuleData_off_m_objectFilter[(offsetof(AttachUpdateModuleData, m_objectFilter) == 0x8) ? 1 : -1];
typedef char AttachUpdateModuleData_off_m_parentStatus[(offsetof(AttachUpdateModuleData, m_parentStatus) == 0xC) ? 1 : -1];
typedef char AttachUpdateModuleData_off_m_scanRange[(offsetof(AttachUpdateModuleData, m_scanRange) == 0x1C) ? 1 : -1];
typedef char AttachUpdateModuleData_off_m_alwaysTeleport[(offsetof(AttachUpdateModuleData, m_alwaysTeleport) == 0x20) ? 1 : -1];
typedef char AttachUpdateModuleData_off_m_anchorToTopOfGeometry[(offsetof(AttachUpdateModuleData, m_anchorToTopOfGeometry) == 0x21) ? 1 : -1];
typedef char AttachUpdateModuleData_off_m_parentOwnerAttachmentEvaEvent[(offsetof(AttachUpdateModuleData, m_parentOwnerAttachmentEvaEvent) == 0x24) ? 1 : -1];
typedef char AttachUpdateModuleData_off_m_parentAllyAttachmentEvaEvent[(offsetof(AttachUpdateModuleData, m_parentAllyAttachmentEvaEvent) == 0x28) ? 1 : -1];
typedef char AttachUpdateModuleData_off_m_parentEnemyAttachmentEvaEvent[(offsetof(AttachUpdateModuleData, m_parentEnemyAttachmentEvaEvent) == 0x2C) ? 1 : -1];
typedef char AttachUpdateModuleData_off_m_attachFX[(offsetof(AttachUpdateModuleData, m_attachFX) == 0x30) ? 1 : -1];
typedef char AttachUpdateModuleData_off_m_parentOwnerDiedEvaEvent[(offsetof(AttachUpdateModuleData, m_parentOwnerDiedEvaEvent) == 0x34) ? 1 : -1];
typedef char AttachUpdateModuleData_off_m_parentAllyDiedEvaEvent[(offsetof(AttachUpdateModuleData, m_parentAllyDiedEvaEvent) == 0x38) ? 1 : -1];
typedef char AttachUpdateModuleData_off_m_parentEnemyDiedtEvaEvent[(offsetof(AttachUpdateModuleData, m_parentEnemyDiedtEvaEvent) == 0x3C) ? 1 : -1];

#endif
