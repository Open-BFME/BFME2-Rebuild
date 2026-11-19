// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C6BB18).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// DelayedLuaEventUpdateModuleData.h: the INI data of DelayedLuaEventUpdate. SIZE 0x8.
// Target facts: ModuleFactory registers the module with operator new 0x8 and
// constructor RVA 0x000B28BF; buildFieldParse RVA 0x004A8D06 adds VA 0x00C6BB18 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DELAYED_LUA_EVENT_UPDATE_MODULE_DATA_H
#define BFME2_DELAYED_LUA_EVENT_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class DelayedLuaEventUpdateModuleData : public ModuleData
{
public:
	DelayedLuaEventUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

};
// SIZE 0x8
typedef char DelayedLuaEventUpdateModuleData_size[(sizeof(DelayedLuaEventUpdateModuleData) == 0x8) ? 1 : -1];

#endif
