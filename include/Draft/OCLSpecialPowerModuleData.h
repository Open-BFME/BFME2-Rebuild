// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5CDC0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_upgradeOCL, m_upgradeName.
// OCLSpecialPowerModuleData.h: the INI data of OCLSpecialPower. SIZE 0xA0.
// Target facts: ModuleFactory registers the module with operator new 0xA0 and
// constructor RVA 0x004C32BC; buildFieldParse RVA 0x004C3898 calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5CDC0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_upgradeOCL +0x7C: INI::Rva004C381C_ParseUpgradeOCL 0x004C381C
//   m_upgradeName +0x90: INI::Rva004C3859_ParseUpgradeName 0x004C3859
#pragma once
#ifndef BFME2_O_C_L_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_O_C_L_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include <vector>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;
class ObjectCreationList;
enum ScienceType;

// One UpgradeOCL entry: the player's science that selects the list (OCLSpecialPower::findOCL 0x004C31A8 walks the vector in 8-byte steps).
struct OCLUpgradeEntry
{
	ScienceType m_science;							// +0x00
	const ObjectCreationList *m_ocl;				// +0x04
};

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char OCLSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class OCLSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	OCLSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	virtual ~OCLSpecialPowerModuleData();		// 0x004C37C0
	static void parseUpgradeName(INI *ini, void *instance, void *store, const void *userData);	// 0x004C3859 (map: placeholder INI::Rva004C3859_ParseUpgradeName)

	_STL::vector<OCLUpgradeEntry> m_upgradeOCL;            // +0x7C UpgradeOCL (INI::Rva004C381C_ParseUpgradeOCL); findOCL 0x004C31A8 reads begin +0x7C, end +0x80
	const ObjectCreationList * m_ocl;                      // +0x88 OCL
	Int m_createLocation;                                  // +0x8C CreateLocation; index into a name list (enum)
	_STL::vector<AsciiString> m_upgradeName;               // +0x90 UpgradeName (0x004C3859 pushes the next ascii token; ~OCLSpecialPowerModuleData 0x004C37C0 destroys it)
	ObjectFilter m_nearestSecondaryObjectFilter;           // +0x9C NearestSecondaryObjectFilter
};
// SIZE 0xA0
typedef char OCLSpecialPowerModuleData_size[(sizeof(OCLSpecialPowerModuleData) == 0xA0) ? 1 : -1];
typedef char OCLSpecialPowerModuleData_off_m_upgradeOCL[(offsetof(OCLSpecialPowerModuleData, m_upgradeOCL) == 0x7C) ? 1 : -1];
typedef char OCLSpecialPowerModuleData_off_m_ocl[(offsetof(OCLSpecialPowerModuleData, m_ocl) == 0x88) ? 1 : -1];
typedef char OCLSpecialPowerModuleData_off_m_createLocation[(offsetof(OCLSpecialPowerModuleData, m_createLocation) == 0x8C) ? 1 : -1];
typedef char OCLSpecialPowerModuleData_off_m_upgradeName[(offsetof(OCLSpecialPowerModuleData, m_upgradeName) == 0x90) ? 1 : -1];
typedef char OCLSpecialPowerModuleData_off_m_nearestSecondaryObjectFilter[(offsetof(OCLSpecialPowerModuleData, m_nearestSecondaryObjectFilter) == 0x9C) ? 1 : -1];

#endif
