// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C49CB0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_clearanceGeometry, m_clearanceMaxHeight, m_clearanceMaxHeightFraction, m_clearanceMinHeight, m_clearanceMinHeightFraction.
// ClearanceTestingSlowDeathBehaviorModuleData.h: the INI data of ClearanceTestingSlowDeathBehavior. SIZE 0x208.
// Target facts: ModuleFactory registers the module with operator new 0x208 and
// constructor RVA 0x00483E25; buildFieldParse RVA 0x00483C1A calls SlowDeathBehaviorModuleData::buildFieldParse, then adds VA 0x00C49CB0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_clearanceGeometry +0x190: GeometryInfo::parseGeometryType 0x006BFEE0
//   m_clearanceMaxHeight +0x1F8: INI::dup_002F02F 0x0002F02F
//   m_clearanceMaxHeightFraction +0x1FC: INI::dup_002F02F 0x0002F02F
//   m_clearanceMinHeight +0x200: INI::dup_002F082 0x0002F082
//   m_clearanceMinHeightFraction +0x204: INI::dup_002F02F 0x0002F02F
#pragma once
#ifndef BFME2_CLEARANCE_TESTING_SLOW_DEATH_BEHAVIOR_MODULE_DATA_H
#define BFME2_CLEARANCE_TESTING_SLOW_DEATH_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/SlowDeathBehaviorModuleData.h"
#include "Common/Coord3D.h"

class INI;
class MultiIniFieldParse;

// Base SlowDeathBehaviorModuleData: the constructor calls its constructor on this. It ends at 0x190 (its own operator-new size);
// this class's members follow it.
typedef char ClearanceTestingSlowDeathBehaviorModuleData_base_size[(sizeof(SlowDeathBehaviorModuleData) == 0x190) ? 1 : -1];

class ClearanceTestingSlowDeathBehaviorModuleData : public SlowDeathBehaviorModuleData
{
public:
	ClearanceTestingSlowDeathBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedByte m_clearanceGeometry[0x5C];                // +0x190 ClearanceGeometry (also ClearanceGeometryMajorRadius, ClearanceGeometryMinorRadius, ClearanceGeometryRotationAnchorOffset, ClearanceGeometryHeight, ClearanceGeometryIsSmall); unresolved: GeometryInfo::parseGeometryHeight, GeometryInfo::parseGeometryIsSmall, GeometryInfo::parseGeometryMajorRadius, GeometryInfo::parseGeometryMinorRadius, GeometryInfo::parseGeometryType, parseGeometryRotationAnchorOffset
	Coord3D m_clearanceGeometryOffset;                     // +0x1EC ClearanceGeometryOffset
	UnsignedByte m_clearanceMaxHeight[0x4];                // +0x1F8 ClearanceMaxHeight; unresolved: INI::dup_002F02F
	UnsignedByte m_clearanceMaxHeightFraction[0x4];        // +0x1FC ClearanceMaxHeightFraction; unresolved: INI::dup_002F02F
	UnsignedByte m_clearanceMinHeight[0x4];                // +0x200 ClearanceMinHeight; unresolved: INI::dup_002F082
	UnsignedByte m_clearanceMinHeightFraction[0x4];        // +0x204 ClearanceMinHeightFraction; unresolved: INI::dup_002F02F
};
// SIZE 0x208
typedef char ClearanceTestingSlowDeathBehaviorModuleData_size[(sizeof(ClearanceTestingSlowDeathBehaviorModuleData) == 0x208) ? 1 : -1];
typedef char ClearanceTestingSlowDeathBehaviorModuleData_off_m_clearanceGeometry[(offsetof(ClearanceTestingSlowDeathBehaviorModuleData, m_clearanceGeometry) == 0x190) ? 1 : -1];
typedef char ClearanceTestingSlowDeathBehaviorModuleData_off_m_clearanceGeometryOffset[(offsetof(ClearanceTestingSlowDeathBehaviorModuleData, m_clearanceGeometryOffset) == 0x1EC) ? 1 : -1];
typedef char ClearanceTestingSlowDeathBehaviorModuleData_off_m_clearanceMaxHeight[(offsetof(ClearanceTestingSlowDeathBehaviorModuleData, m_clearanceMaxHeight) == 0x1F8) ? 1 : -1];
typedef char ClearanceTestingSlowDeathBehaviorModuleData_off_m_clearanceMaxHeightFraction[(offsetof(ClearanceTestingSlowDeathBehaviorModuleData, m_clearanceMaxHeightFraction) == 0x1FC) ? 1 : -1];
typedef char ClearanceTestingSlowDeathBehaviorModuleData_off_m_clearanceMinHeight[(offsetof(ClearanceTestingSlowDeathBehaviorModuleData, m_clearanceMinHeight) == 0x200) ? 1 : -1];
typedef char ClearanceTestingSlowDeathBehaviorModuleData_off_m_clearanceMinHeightFraction[(offsetof(ClearanceTestingSlowDeathBehaviorModuleData, m_clearanceMinHeightFraction) == 0x204) ? 1 : -1];

#endif
