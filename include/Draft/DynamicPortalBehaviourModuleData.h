// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C42A38).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_wayPoint, m_link.
// DynamicPortalBehaviourModuleData.h: the INI data of DynamicPortalBehaviour. SIZE 0x15C.
// Target facts: ModuleFactory registers the module with operator new 0x15C and
// constructor RVA 0x0046142A; buildFieldParse RVA 0x004617AD adds VA 0x00C42A38 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_wayPoint +0x120: DynamicPortalBehaviourData::parseWayPoint 0x0046133B
//   m_link +0x12C: DynamicPortalBehaviourData::parseLink 0x004616B1
#pragma once
#ifndef BFME2_DYNAMIC_PORTAL_BEHAVIOUR_MODULE_DATA_H
#define BFME2_DYNAMIC_PORTAL_BEHAVIOUR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

class DynamicPortalBehaviourModuleData : public ModuleData
{
public:
	DynamicPortalBehaviourModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	Int m_numberOfBones;                                   // +0x118 NumberOfBones
	AsciiString m_bonePrefix;                              // +0x11C BonePrefix
	UnsignedByte m_wayPoint[0xC];                          // +0x120 WayPoint; unresolved: DynamicPortalBehaviourData::parseWayPoint
	UnsignedByte m_link[0xC];                              // +0x12C Link; unresolved: DynamicPortalBehaviourData::parseLink
	AsciiString m_wallBoundsMesh;                          // +0x138 WallBoundsMesh
	Bool m_generateNow;                                    // +0x13C GenerateNow
	Bool m_allowEnemies;                                   // +0x13D AllowEnemies
	UnsignedInt m_aboveWall;                               // +0x140 AboveWall
	Coord3D m_topAttackPos;                                // +0x144 TopAttackPos
	Real m_topAttackRadius;                                // +0x150 TopAttackRadius
	Real m_activationDelaySeconds;                         // +0x154 ActivationDelaySeconds
	ObjectFilter m_objectFilter;                           // +0x158 ObjectFilter
};
// SIZE 0x15C
typedef char DynamicPortalBehaviourModuleData_size[(sizeof(DynamicPortalBehaviourModuleData) == 0x15C) ? 1 : -1];
typedef char DynamicPortalBehaviourModuleData_off_m_upgradeMuxData[(offsetof(DynamicPortalBehaviourModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char DynamicPortalBehaviourModuleData_off_m_numberOfBones[(offsetof(DynamicPortalBehaviourModuleData, m_numberOfBones) == 0x118) ? 1 : -1];
typedef char DynamicPortalBehaviourModuleData_off_m_bonePrefix[(offsetof(DynamicPortalBehaviourModuleData, m_bonePrefix) == 0x11C) ? 1 : -1];
typedef char DynamicPortalBehaviourModuleData_off_m_wayPoint[(offsetof(DynamicPortalBehaviourModuleData, m_wayPoint) == 0x120) ? 1 : -1];
typedef char DynamicPortalBehaviourModuleData_off_m_link[(offsetof(DynamicPortalBehaviourModuleData, m_link) == 0x12C) ? 1 : -1];
typedef char DynamicPortalBehaviourModuleData_off_m_wallBoundsMesh[(offsetof(DynamicPortalBehaviourModuleData, m_wallBoundsMesh) == 0x138) ? 1 : -1];
typedef char DynamicPortalBehaviourModuleData_off_m_generateNow[(offsetof(DynamicPortalBehaviourModuleData, m_generateNow) == 0x13C) ? 1 : -1];
typedef char DynamicPortalBehaviourModuleData_off_m_allowEnemies[(offsetof(DynamicPortalBehaviourModuleData, m_allowEnemies) == 0x13D) ? 1 : -1];
typedef char DynamicPortalBehaviourModuleData_off_m_aboveWall[(offsetof(DynamicPortalBehaviourModuleData, m_aboveWall) == 0x140) ? 1 : -1];
typedef char DynamicPortalBehaviourModuleData_off_m_topAttackPos[(offsetof(DynamicPortalBehaviourModuleData, m_topAttackPos) == 0x144) ? 1 : -1];
typedef char DynamicPortalBehaviourModuleData_off_m_topAttackRadius[(offsetof(DynamicPortalBehaviourModuleData, m_topAttackRadius) == 0x150) ? 1 : -1];
typedef char DynamicPortalBehaviourModuleData_off_m_activationDelaySeconds[(offsetof(DynamicPortalBehaviourModuleData, m_activationDelaySeconds) == 0x154) ? 1 : -1];
typedef char DynamicPortalBehaviourModuleData_off_m_objectFilter[(offsetof(DynamicPortalBehaviourModuleData, m_objectFilter) == 0x158) ? 1 : -1];

#endif
