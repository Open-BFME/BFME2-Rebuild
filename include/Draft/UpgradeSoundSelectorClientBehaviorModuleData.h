// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5F2DC).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// UpgradeSoundSelectorClientBehaviorModuleData.h: the INI data of UpgradeSoundSelectorClientBehavior. SIZE 0x14.
// Target facts: ModuleFactory registers the module with operator new 0x14 and
// constructor RVA 0x004CB9F3; buildFieldParse RVA 0x004CBBB4 adds VA 0x00C5F2DC at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_UPGRADE_SOUND_SELECTOR_CLIENT_BEHAVIOR_MODULE_DATA_H
#define BFME2_UPGRADE_SOUND_SELECTOR_CLIENT_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class UpgradeSoundSelectorClientBehaviorModuleData : public ModuleData
{
public:
	UpgradeSoundSelectorClientBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static void parseSoundUpgrade(INI *ini, void *instance, void *store, const void *userData);

	UnsignedByte m_unk0x8[0xC];
};
// SIZE 0x14
typedef char UpgradeSoundSelectorClientBehaviorModuleData_size[(sizeof(UpgradeSoundSelectorClientBehaviorModuleData) == 0x14) ? 1 : -1];

#endif
