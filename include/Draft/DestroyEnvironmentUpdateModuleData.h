// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C54D28).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// DestroyEnvironmentUpdateModuleData.h: the INI data of DestroyEnvironmentUpdate. SIZE 0x10.
// Target facts: ModuleFactory registers the module with operator new 0x10 and
// constructor RVA 0x004AC615; buildFieldParse RVA 0x004AC629 adds VA 0x00C54D28 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DESTROY_ENVIRONMENT_UPDATE_MODULE_DATA_H
#define BFME2_DESTROY_ENVIRONMENT_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class DestroyEnvironmentUpdateModuleData : public ModuleData
{
public:
	DestroyEnvironmentUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedInt m_startTime;                               // +0x8 StartTime; duration, frames
	UnsignedInt m_destructionTime;                         // +0xC DestructionTime; duration, frames
};
// SIZE 0x10
typedef char DestroyEnvironmentUpdateModuleData_size[(sizeof(DestroyEnvironmentUpdateModuleData) == 0x10) ? 1 : -1];
typedef char DestroyEnvironmentUpdateModuleData_off_m_startTime[(offsetof(DestroyEnvironmentUpdateModuleData, m_startTime) == 0x8) ? 1 : -1];
typedef char DestroyEnvironmentUpdateModuleData_off_m_destructionTime[(offsetof(DestroyEnvironmentUpdateModuleData, m_destructionTime) == 0xC) ? 1 : -1];

#endif
