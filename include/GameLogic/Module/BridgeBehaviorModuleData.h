// BridgeBehaviorModuleData.h: the INI data of BridgeBehavior. SIZE 0x18.
// Target facts: ModuleFactory registers the module with operator new 0x18 and
// constructor RVA 0x00457FF1; buildFieldParse RVA 0x00458268 adds VA 0x00C40B20 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_bridgeDieFX +0x10: BridgeBehaviorModuleData::parseFX 0x004580A9
//   m_bridgeDieOCL +0x14: BridgeBehaviorModuleData::parseOCL 0x00458153
#pragma once
#ifndef BFME2_BRIDGE_BEHAVIOR_MODULE_DATA_H
#define BFME2_BRIDGE_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include <list>
#include "Common/Coord3D.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;
class FXList;
class ObjectCreationList;

// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// When and where (a bone of the bridge) a bridge effect plays: parseTimeAndLocationInfo 0x0045679B.
struct TimeAndLocationInfo
{
	UnsignedInt delay;										// +0x0
	AsciiString boneName;									// +0x4
};

// One BridgeDieFX entry: an FX list and its timing (parseFX 0x004580A9); one node of the list is 20 bytes.
struct BridgeFXInfo
{
	const FXList *fx;										// +0x0
	TimeAndLocationInfo timeAndLocationInfo;				// +0x4
};

// One BridgeDieOCL entry (parseOCL 0x00458153).
struct BridgeOCLInfo
{
	const ObjectCreationList *ocl;							// +0x0
	TimeAndLocationInfo timeAndLocationInfo;				// +0x4
};

typedef _STL::list<BridgeFXInfo> BridgeFXList;
typedef _STL::list<BridgeOCLInfo> BridgeOCLList;

class BridgeBehaviorModuleData : public ModuleData
{
public:
	BridgeBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);
	static void parseFX(INI *ini, void *instance, void *store, const void *userData);
	static void parseOCL(INI *ini, void *instance, void *store, const void *userData);
	virtual ~BridgeBehaviorModuleData();					// 0x0045804A

	Real m_lateralScaffoldSpeed;                           // +0x8 LateralScaffoldSpeed; velocity
	Real m_verticalScaffoldSpeed;                          // +0xC VerticalScaffoldSpeed; velocity
	BridgeFXList m_bridgeDieFX;                            // +0x10 BridgeDieFX (parseFX 0x004580A9)
	BridgeOCLList m_bridgeDieOCL;                          // +0x14 BridgeDieOCL (parseOCL 0x00458153)
};
// SIZE 0x18
typedef char BridgeBehaviorModuleData_size[(sizeof(BridgeBehaviorModuleData) == 0x18) ? 1 : -1];
typedef char BridgeBehaviorModuleData_off_m_lateralScaffoldSpeed[(offsetof(BridgeBehaviorModuleData, m_lateralScaffoldSpeed) == 0x8) ? 1 : -1];
typedef char BridgeBehaviorModuleData_off_m_verticalScaffoldSpeed[(offsetof(BridgeBehaviorModuleData, m_verticalScaffoldSpeed) == 0xC) ? 1 : -1];
typedef char BridgeBehaviorModuleData_off_m_bridgeDieFX[(offsetof(BridgeBehaviorModuleData, m_bridgeDieFX) == 0x10) ? 1 : -1];
typedef char BridgeBehaviorModuleData_off_m_bridgeDieOCL[(offsetof(BridgeBehaviorModuleData, m_bridgeDieOCL) == 0x14) ? 1 : -1];

#endif
