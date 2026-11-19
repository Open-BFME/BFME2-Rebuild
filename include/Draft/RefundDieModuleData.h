// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF0700).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// RefundDieModuleData.h: the INI data of RefundDie. SIZE 0x44.
// Target facts: ModuleFactory registers the module with operator new 0x44 and
// constructor RVA 0x002538DB; buildFieldParse RVA 0x0025391E adds VA 0x00BF0700 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_REFUND_DIE_MODULE_DATA_H
#define BFME2_REFUND_DIE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DestroyDieModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;
class UpgradeTemplate;

// Base DestroyDieModuleData: the constructor calls its constructor on this. It ends at 0x38 (its own operator-new size);
// this class's members follow it.
typedef char RefundDieModuleData_base_size[(sizeof(DestroyDieModuleData) == 0x38) ? 1 : -1];

class RefundDieModuleData : public DestroyDieModuleData
{
public:
	RefundDieModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	const UpgradeTemplate * m_upgradeRequired;             // +0x38 UpgradeRequired
	Real m_refundPercent;                                  // +0x3C RefundPercent; percent
	ObjectFilter m_buildingRequired;                       // +0x40 BuildingRequired
};
// SIZE 0x44
typedef char RefundDieModuleData_size[(sizeof(RefundDieModuleData) == 0x44) ? 1 : -1];
typedef char RefundDieModuleData_off_m_upgradeRequired[(offsetof(RefundDieModuleData, m_upgradeRequired) == 0x38) ? 1 : -1];
typedef char RefundDieModuleData_off_m_refundPercent[(offsetof(RefundDieModuleData, m_refundPercent) == 0x3C) ? 1 : -1];
typedef char RefundDieModuleData_off_m_buildingRequired[(offsetof(RefundDieModuleData, m_buildingRequired) == 0x40) ? 1 : -1];

#endif
