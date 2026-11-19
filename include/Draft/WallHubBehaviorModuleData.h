// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C3FED8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_segmentTemplateName.
// WallHubBehaviorModuleData.h: the INI data of WallHubBehavior. SIZE 0x3C.
// Target facts: ModuleFactory registers the module with operator new 0x3C and
// constructor RVA 0x00453020; buildFieldParse RVA 0x00452DEF adds VA 0x00C3FED8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_segmentTemplateName +0x8: INI::parseAsciiStringVectorAppend 0x0002E896
#pragma once
#ifndef BFME2_WALL_HUB_BEHAVIOR_MODULE_DATA_H
#define BFME2_WALL_HUB_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class WallHubBehaviorModuleData : public ModuleData
{
public:
	WallHubBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedByte m_segmentTemplateName[0xC];               // +0x8 SegmentTemplateName; unresolved: INI::parseAsciiStringVectorAppend
	AsciiString m_hubCapTemplateName;                      // +0x14 HubCapTemplateName
	AsciiString m_defaultSegmentTemplateName;              // +0x18 DefaultSegmentTemplateName
	AsciiString m_cliffCapTemplateName;                    // +0x1C CliffCapTemplateName
	AsciiString m_shoreCapTemplateName;                    // +0x20 ShoreCapTemplateName
	AsciiString m_borderCapTemplateName;                   // +0x24 BorderCapTemplateName
	AsciiString m_elevatedSegmentTemplateName;             // +0x28 ElevatedSegmentTemplateName
	Real m_builderRadius;                                  // +0x2C BuilderRadius
	Real m_maxBuildoutDistance;                            // +0x30 MaxBuildoutDistance
	UnsignedInt m_options;                                 // +0x34 Options; bit string
	Int m_staggeredBuildFactor;                            // +0x38 StaggeredBuildFactor
};
// SIZE 0x3C
typedef char WallHubBehaviorModuleData_size[(sizeof(WallHubBehaviorModuleData) == 0x3C) ? 1 : -1];
typedef char WallHubBehaviorModuleData_off_m_segmentTemplateName[(offsetof(WallHubBehaviorModuleData, m_segmentTemplateName) == 0x8) ? 1 : -1];
typedef char WallHubBehaviorModuleData_off_m_hubCapTemplateName[(offsetof(WallHubBehaviorModuleData, m_hubCapTemplateName) == 0x14) ? 1 : -1];
typedef char WallHubBehaviorModuleData_off_m_defaultSegmentTemplateName[(offsetof(WallHubBehaviorModuleData, m_defaultSegmentTemplateName) == 0x18) ? 1 : -1];
typedef char WallHubBehaviorModuleData_off_m_cliffCapTemplateName[(offsetof(WallHubBehaviorModuleData, m_cliffCapTemplateName) == 0x1C) ? 1 : -1];
typedef char WallHubBehaviorModuleData_off_m_shoreCapTemplateName[(offsetof(WallHubBehaviorModuleData, m_shoreCapTemplateName) == 0x20) ? 1 : -1];
typedef char WallHubBehaviorModuleData_off_m_borderCapTemplateName[(offsetof(WallHubBehaviorModuleData, m_borderCapTemplateName) == 0x24) ? 1 : -1];
typedef char WallHubBehaviorModuleData_off_m_elevatedSegmentTemplateName[(offsetof(WallHubBehaviorModuleData, m_elevatedSegmentTemplateName) == 0x28) ? 1 : -1];
typedef char WallHubBehaviorModuleData_off_m_builderRadius[(offsetof(WallHubBehaviorModuleData, m_builderRadius) == 0x2C) ? 1 : -1];
typedef char WallHubBehaviorModuleData_off_m_maxBuildoutDistance[(offsetof(WallHubBehaviorModuleData, m_maxBuildoutDistance) == 0x30) ? 1 : -1];
typedef char WallHubBehaviorModuleData_off_m_options[(offsetof(WallHubBehaviorModuleData, m_options) == 0x34) ? 1 : -1];
typedef char WallHubBehaviorModuleData_off_m_staggeredBuildFactor[(offsetof(WallHubBehaviorModuleData, m_staggeredBuildFactor) == 0x38) ? 1 : -1];

#endif
