// SubObjectsUpgradeModuleData.h: the INI data of SubObjectsUpgrade. SIZE 0x154.
// Target facts: ModuleFactory registers the module with operator new 0x154 and
// constructor RVA 0x00257768; buildFieldParse RVA 0x004B52EB adds the UpgradeMuxData at +0x8 and VA 0x00C57D38 at +0x0.
// Promoted from Draft by f03. The destructor 0x004B5214 destroys, in reverse order, an 8-byte-element vector at
// +0x13C (after clearing it: erase(begin, end)), three vectors of AsciiString at +0x130, +0x124 and +0x118, then
// stores ModuleData's table. UpgradeTexture's parse function (0x004B5293) grows the +0x13C vector by one element and
// reads three tokens into it: the first lands in the element's first string, the second is skipped, the third lands
// in its second string. Names are the INI tokens.
#pragma once
#ifndef BFME2_SUB_OBJECTS_UPGRADE_MODULE_DATA_H
#define BFME2_SUB_OBJECTS_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/AsciiString.h"
#include "Common/Vector.h"

class INI;
class MultiIniFieldParse;

// One UpgradeTexture entry: the texture to replace and the one that replaces it (0x004B5293).
struct SubObjectTexturePair
{
	AsciiString m_original;													// +0x0, first token
	AsciiString m_replacement;												// +0x4, third token
};
// SIZE 0x8

// Base UpgradeModuleData: the constructor 0x00257768 calls its constructor 0x00253487 first, and the
// table keeps its slot 10 (`mov al,1`, 0x0050B5C6) (h05 r9).
class SubObjectsUpgradeModuleData : public UpgradeModuleData
{
public:
	SubObjectsUpgradeModuleData();											// 0x00257768
	virtual ~SubObjectsUpgradeModuleData();									// 0x004B5214
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	_STL::vector<AsciiString> m_showSubObjects;            // +0x118 ShowSubObjects
	_STL::vector<AsciiString> m_hideSubObjects;            // +0x124 HideSubObjects
	_STL::vector<AsciiString> m_excludeSubobjects;         // +0x130 ExcludeSubobjects
	_STL::vector<SubObjectTexturePair> m_upgradeTexture;   // +0x13C UpgradeTexture
	Real m_fadeTimeInSeconds;                              // +0x148 FadeTimeInSeconds
	Real m_waitBeforeFadeInSeconds;                        // +0x14C WaitBeforeFadeInSeconds
	Bool m_recolorHouse;                                   // +0x150 RecolorHouse
	Bool m_skipFadeOnCreate;                               // +0x151 SkipFadeOnCreate
	Bool m_hideSubObjectsOnRemove;                         // +0x152 HideSubObjectsOnRemove
	Bool m_unHideSubObjectsOnRemove;                       // +0x153 UnHideSubObjectsOnRemove
};
// SIZE 0x154
typedef char SubObjectsUpgradeModuleData_size[(sizeof(SubObjectsUpgradeModuleData) == 0x154) ? 1 : -1];
typedef char SubObjectsUpgradeModuleData_off_m_upgradeMuxData[(offsetof(SubObjectsUpgradeModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char SubObjectsUpgradeModuleData_off_m_showSubObjects[(offsetof(SubObjectsUpgradeModuleData, m_showSubObjects) == 0x118) ? 1 : -1];
typedef char SubObjectsUpgradeModuleData_off_m_hideSubObjects[(offsetof(SubObjectsUpgradeModuleData, m_hideSubObjects) == 0x124) ? 1 : -1];
typedef char SubObjectsUpgradeModuleData_off_m_excludeSubobjects[(offsetof(SubObjectsUpgradeModuleData, m_excludeSubobjects) == 0x130) ? 1 : -1];
typedef char SubObjectsUpgradeModuleData_off_m_upgradeTexture[(offsetof(SubObjectsUpgradeModuleData, m_upgradeTexture) == 0x13C) ? 1 : -1];
typedef char SubObjectsUpgradeModuleData_off_m_fadeTimeInSeconds[(offsetof(SubObjectsUpgradeModuleData, m_fadeTimeInSeconds) == 0x148) ? 1 : -1];
typedef char SubObjectsUpgradeModuleData_off_m_waitBeforeFadeInSeconds[(offsetof(SubObjectsUpgradeModuleData, m_waitBeforeFadeInSeconds) == 0x14C) ? 1 : -1];
typedef char SubObjectsUpgradeModuleData_off_m_recolorHouse[(offsetof(SubObjectsUpgradeModuleData, m_recolorHouse) == 0x150) ? 1 : -1];
typedef char SubObjectsUpgradeModuleData_off_m_skipFadeOnCreate[(offsetof(SubObjectsUpgradeModuleData, m_skipFadeOnCreate) == 0x151) ? 1 : -1];
typedef char SubObjectsUpgradeModuleData_off_m_hideSubObjectsOnRemove[(offsetof(SubObjectsUpgradeModuleData, m_hideSubObjectsOnRemove) == 0x152) ? 1 : -1];
typedef char SubObjectsUpgradeModuleData_off_m_unHideSubObjectsOnRemove[(offsetof(SubObjectsUpgradeModuleData, m_unHideSubObjectsOnRemove) == 0x153) ? 1 : -1];

#endif
