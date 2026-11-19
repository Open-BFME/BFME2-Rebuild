// CastleBehaviorModuleData.h: the INI data of CastleBehavior. SIZE 0x78.
// Target facts: ModuleFactory registers the module with operator new 0x78 and
// constructor RVA 0x0039A027; buildFieldParse RVA 0x0039ABC5 adds VA 0x00BF00A0
// and 0x00C1ABA0. Offsets are retail's table entries'. +0x50, +0x5C and +0x68
// are three 12-byte STLport vectors (the parsers at 0x0039A4FC, 0x0039A565 and
// 0x0039981E fill them); +0x5C is the FactionDecal vector of
// (name, decal name, Real) records that getCampDiameter reads.
#pragma once
#ifndef BFME2_CASTLE_BEHAVIOR_MODULE_DATA_H
#define BFME2_CASTLE_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Vector.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"
#include <map>

class INI;
class MultiIniFieldParse;
class FXList;

// One FactionDecal entry: the faction's name, the decal it gets and its value.
struct CastleFactionDecal
{
	AsciiString m_factionName;		// 0x0
	AsciiString m_decalName;		// 0x4
	Real m_value;					// 0x8
};
// SIZE 0xC

// One CastleToUnpackForFaction entry: a template name and two counts.
struct CastleUnpackForFaction
{
	AsciiString m_templateName;		// 0x0
	UnsignedInt m_count1;			// 0x4
	UnsignedInt m_count2;			// 0x8
};
// SIZE 0xC

class CastleBehaviorModuleData : public ModuleData
{
public:
	CastleBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_repairHealthPercentPerSecond;					// +0x08 RepairHealthPercentPerSecond
	Int m_buildVariation;									// +0x0C BuildVariation
	AsciiString m_preBuiltPlyr;								// +0x10 PreBuiltPlyr
	AsciiString m_decalName;								// +0x14 DecalName
	Real m_decalSize;										// +0x18 DecalSize
	Real m_fadeTime;										// +0x1C FadeTime
	Real m_unpackDelayTime;									// +0x20 UnpackDelayTime (the constructor stores a float)
	Real m_buildTime;										// +0x24 BuildTime
	Real m_28;												// +0x28 no table entry; the constructor stores 100.0
	Real m_2C;												// +0x2C no table entry; the constructor stores 0.0
	ObjectFilter m_filter30;								// +0x30 filter built from a KindOf list
	ObjectFilter m_filter34;								// +0x34 filter over the no-KindOf masks
	Int m_38;												// +0x38 no table entry; the constructor stores 0x12
	Bool m_3C;												// +0x3C no table entry
	Bool m_3D;												// +0x3D no table entry
	Bool m_3E;												// +0x3E no table entry
	Int m_40;												// +0x40 no table entry; the constructor stores 0xA
	Int m_44;												// +0x44 no table entry
	Int m_48;												// +0x48 no table entry
	Int m_evaEnemyCastleSightedEvent;						// +0x4C EvaEnemyCastleSightedEvent
	_STL::vector<AsciiString> m_preBuiltList;				// +0x50 PreBuiltList
	_STL::vector<CastleFactionDecal> m_factionDecal;		// +0x5C FactionDecal
	_STL::map<Int, CastleUnpackForFaction> m_castleToUnpackForFaction;	// +0x68 CastleToUnpackForFaction
	Bool m_disableStructureRotation;						// +0x74 DisableStructureRotation
	Bool m_transferFoundationHealthToCastleUponUnpack;		// +0x75 TransferFoundationHealthToCastleUponUnpack
};
// SIZE 0x78
typedef char CastleBehaviorModuleData_size[(sizeof(CastleBehaviorModuleData) == 0x78) ? 1 : -1];
typedef char CastleBehaviorModuleData_off_preBuiltPlyr[(offsetof(CastleBehaviorModuleData, m_preBuiltPlyr) == 0x10) ? 1 : -1];
typedef char CastleBehaviorModuleData_off_factionDecal[(offsetof(CastleBehaviorModuleData, m_factionDecal) == 0x5C) ? 1 : -1];

#endif
