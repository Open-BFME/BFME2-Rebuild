// CreateObjectDieModuleData.h: the INI data of CreateObjectDie. SIZE 0x4C.
// Target facts: ModuleFactory registers the module with operator new 0x4C and
// constructor RVA 0x00485D88; buildFieldParse RVA 0x00485A74 adds VA 0x00C4A928 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_CREATE_OBJECT_DIE_MODULE_DATA_H
#define BFME2_CREATE_OBJECT_DIE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DestroyDieModuleData.h"
#include "Common/AsciiString.h"
#include "Common/Vector.h"

class INI;
class MultiIniFieldParse;
class ObjectCreationList;

// Base DestroyDieModuleData: the constructor calls its constructor on this. It ends at 0x38 (its own operator-new size);
// this class's members follow it.
typedef char CreateObjectDieModuleData_base_size[(sizeof(DestroyDieModuleData) == 0x38) ? 1 : -1];

class CreateObjectDieModuleData : public DestroyDieModuleData
{
public:
	CreateObjectDieModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	ObjectCreationList * m_creationList;                    // +0x38 CreationList (created through; onDie 0x00485C86 calls its create overload)
	AsciiString m_debrisPortionOfSelf;                     // +0x3C DebrisPortionOfSelf
	_STL::vector<AsciiString> m_upgradeRequired;           // +0x40 UpgradeRequired
};
// SIZE 0x4C
typedef char CreateObjectDieModuleData_size[(sizeof(CreateObjectDieModuleData) == 0x4C) ? 1 : -1];
typedef char CreateObjectDieModuleData_off_m_creationList[(offsetof(CreateObjectDieModuleData, m_creationList) == 0x38) ? 1 : -1];
typedef char CreateObjectDieModuleData_off_m_debrisPortionOfSelf[(offsetof(CreateObjectDieModuleData, m_debrisPortionOfSelf) == 0x3C) ? 1 : -1];
typedef char CreateObjectDieModuleData_off_m_upgradeRequired[(offsetof(CreateObjectDieModuleData, m_upgradeRequired) == 0x40) ? 1 : -1];

#endif
