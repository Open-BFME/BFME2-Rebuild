// LargeGroupBonusUpdateModuleData.h: the INI data of LargeGroupBonusUpdate. SIZE 0x30.
// Target facts: ModuleFactory registers the module with operator new 0x30 and
// constructor RVA 0x004901BD; buildFieldParse RVA 0x004900A0 adds VA 0x00C4D1A8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_LARGE_GROUP_BONUS_UPDATE_MODULE_DATA_H
#define BFME2_LARGE_GROUP_BONUS_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"
#include "Common/Vector.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class LargeGroupBonusUpdateModuleData : public ModuleData
{
public:
	LargeGroupBonusUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_updateRate;                              // +0x8 UpdateRate; duration, frames
	ObjectFilter m_hordeMemberFilter;                      // +0xC HordeMemberFilter
	Int m_count;                                           // +0x10 Count
	Real m_radius;                                         // +0x14 Radius
	Bool m_alliesOnly;                                     // +0x18 AlliesOnly
	Real m_rubOffRadius;                                   // +0x1C RubOffRadius
	_STL::vector<AsciiString> m_flagSubObjectNames;        // +0x20 FlagSubObjectNames
	AsciiString m_attributeModifier;                       // +0x2C AttributeModifier
};
// SIZE 0x30
typedef char LargeGroupBonusUpdateModuleData_size[(sizeof(LargeGroupBonusUpdateModuleData) == 0x30) ? 1 : -1];
typedef char LargeGroupBonusUpdateModuleData_off_m_updateRate[(offsetof(LargeGroupBonusUpdateModuleData, m_updateRate) == 0x8) ? 1 : -1];
typedef char LargeGroupBonusUpdateModuleData_off_m_hordeMemberFilter[(offsetof(LargeGroupBonusUpdateModuleData, m_hordeMemberFilter) == 0xC) ? 1 : -1];
typedef char LargeGroupBonusUpdateModuleData_off_m_count[(offsetof(LargeGroupBonusUpdateModuleData, m_count) == 0x10) ? 1 : -1];
typedef char LargeGroupBonusUpdateModuleData_off_m_radius[(offsetof(LargeGroupBonusUpdateModuleData, m_radius) == 0x14) ? 1 : -1];
typedef char LargeGroupBonusUpdateModuleData_off_m_alliesOnly[(offsetof(LargeGroupBonusUpdateModuleData, m_alliesOnly) == 0x18) ? 1 : -1];
typedef char LargeGroupBonusUpdateModuleData_off_m_rubOffRadius[(offsetof(LargeGroupBonusUpdateModuleData, m_rubOffRadius) == 0x1C) ? 1 : -1];
typedef char LargeGroupBonusUpdateModuleData_off_m_flagSubObjectNames[(offsetof(LargeGroupBonusUpdateModuleData, m_flagSubObjectNames) == 0x20) ? 1 : -1];
typedef char LargeGroupBonusUpdateModuleData_off_m_attributeModifier[(offsetof(LargeGroupBonusUpdateModuleData, m_attributeModifier) == 0x2C) ? 1 : -1];

#endif
