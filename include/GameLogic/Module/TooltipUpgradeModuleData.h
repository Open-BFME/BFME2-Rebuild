// TooltipUpgradeModuleData.h: the INI data of TooltipUpgrade. SIZE 0x120.
// UpgradeModuleData (0x118, which holds the trigger mask data at +0x8) plus two
// strings. DisplayName is at +0x118 and Description at +0x11C.
// Target facts: friend_newModuleData 0x002558E9 allocates 0x120 bytes, runs the
// constructor 0x0025588E and then INI::initFromINIMultiProc with buildFieldParse
// 0x004B7A2E. The constructor's base call 0x00653487 is UpgradeModuleData's, which
// has no declared constructor here yet.
#pragma once
#ifndef BFME2_TOOLTIPUPGRADEMODULEDATA_H
#define BFME2_TOOLTIPUPGRADEMODULEDATA_H

#include "Common/AsciiString.h"
#include "GameLogic/Module/UpgradeModule.h"

class INI;
class MultiIniFieldParse;
class ModuleData;

class TooltipUpgradeModuleData : public UpgradeModuleData
{
public:
	TooltipUpgradeModuleData();										// 0x0025588E
	static void buildFieldParse(MultiIniFieldParse &parse);			// 0x004B7A2E
	static ModuleData *friend_newModuleData(INI *ini);				// 0x002558E9

	AsciiString m_displayName;										// +0x118 DisplayName
	AsciiString m_description;										// +0x11C Description
};

typedef char TooltipUpgradeModuleDataSize[sizeof(TooltipUpgradeModuleData) == 0x120 ? 1 : -1];

#endif
