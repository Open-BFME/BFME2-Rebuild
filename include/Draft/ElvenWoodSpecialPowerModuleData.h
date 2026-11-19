// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5D018).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// ElvenWoodSpecialPowerModuleData.h: the INI data of ElvenWoodSpecialPower. SIZE 0x9C.
// Target facts: ModuleFactory registers the module with operator new 0x9C and
// constructor RVA 0x004C3DA9; buildFieldParse RVA 0x004C3FC2 calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5D018 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_ELVEN_WOOD_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_ELVEN_WOOD_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"
#include "Common/AsciiString.h"
#include "Common/Vector.h"

class INI;
class MultiIniFieldParse;
class FXList;
class ObjectCreationList;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char ElvenWoodSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

// The 8-byte element of the +0x7C vector: an AsciiString key and an Int (the map names the
// element RvaPair004C3D4C; retail's vector destructor 0x004C3D4C destroys each key through ~AsciiString).
struct ElvenWoodKeyCount
{
	AsciiString m_key;
	Real m_weight;	// +0x4: rva004C3C36 (0x004C3C36) adds it to a running int total with cvtsi2ss / addss / cvttss2si
};

class ElvenWoodSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	ElvenWoodSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	// +0x7C, 12 bytes: the vector's three words (begin, end, end of storage). The constructor
	// 0x004C3DA9 and the destructor 0x004C3EC2 both reach it through the vector's own code.
	_STL::vector<ElvenWoodKeyCount> m_keyCounts;
	AsciiString m_elvenGroveObject;                        // +0x88 ElvenGroveObject
	Int m_elvenNumObjects;                                 // +0x8C ElvenNumObjects
	Real m_elvenWoodRadius;                                // +0x90 ElvenWoodRadius
	const FXList * m_elvenWoodFX;                          // +0x94 ElvenWoodFX
	const ObjectCreationList * m_elvenWoodOCL;             // +0x98 ElvenWoodOCL
};
// SIZE 0x9C
typedef char ElvenWoodSpecialPowerModuleData_size[(sizeof(ElvenWoodSpecialPowerModuleData) == 0x9C) ? 1 : -1];
typedef char ElvenWoodSpecialPowerModuleData_off_m_elvenGroveObject[(offsetof(ElvenWoodSpecialPowerModuleData, m_elvenGroveObject) == 0x88) ? 1 : -1];
typedef char ElvenWoodSpecialPowerModuleData_off_m_elvenNumObjects[(offsetof(ElvenWoodSpecialPowerModuleData, m_elvenNumObjects) == 0x8C) ? 1 : -1];
typedef char ElvenWoodSpecialPowerModuleData_off_m_elvenWoodRadius[(offsetof(ElvenWoodSpecialPowerModuleData, m_elvenWoodRadius) == 0x90) ? 1 : -1];
typedef char ElvenWoodSpecialPowerModuleData_off_m_elvenWoodFX[(offsetof(ElvenWoodSpecialPowerModuleData, m_elvenWoodFX) == 0x94) ? 1 : -1];
typedef char ElvenWoodSpecialPowerModuleData_off_m_elvenWoodOCL[(offsetof(ElvenWoodSpecialPowerModuleData, m_elvenWoodOCL) == 0x98) ? 1 : -1];

#endif
