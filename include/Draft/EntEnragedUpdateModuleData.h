// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C56A98).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// EntEnragedUpdateModuleData.h: the INI data of EntEnragedUpdate. SIZE 0x30.
// Target facts: ModuleFactory registers the module with operator new 0x30 and
// constructor RVA 0x004B249D; buildFieldParse RVA 0x004B2505 adds VA 0x00C56A98 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_ENT_ENRAGED_UPDATE_MODULE_DATA_H
#define BFME2_ENT_ENRAGED_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;
class FXList;

class EntEnragedUpdateModuleData : public ModuleData
{
public:
	EntEnragedUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedInt m_scanDelayTime;                           // +0x8 ScanDelayTime; duration, frames
	UnsignedInt m_enragedTime;                             // +0xC EnragedTime; duration, frames
	UnsignedInt m_timeUntilCanRageAgain;                   // +0x10 TimeUntilCanRageAgain; duration, frames
	Real m_scanDistance;                                   // +0x14 ScanDistance
	ObjectFilter m_friendlyDeadFilter;                     // +0x18 FriendlyDeadFilter
	ObjectFilter m_hatedObjectFilter;                      // +0x1C HatedObjectFilter
	UnsignedInt m_enragedTransitionTime;                   // +0x20 EnragedTransitionTime; duration, frames
	const FXList * m_enragedTransitionFX;                  // +0x24 EnragedTransitionFX
	const FXList * m_enragedOnBuffFX;                      // +0x28 EnragedOnBuffFX
	const FXList * m_enragedOffBuffFX;                     // +0x2C EnragedOffBuffFX
};
// SIZE 0x30
typedef char EntEnragedUpdateModuleData_size[(sizeof(EntEnragedUpdateModuleData) == 0x30) ? 1 : -1];
typedef char EntEnragedUpdateModuleData_off_m_scanDelayTime[(offsetof(EntEnragedUpdateModuleData, m_scanDelayTime) == 0x8) ? 1 : -1];
typedef char EntEnragedUpdateModuleData_off_m_enragedTime[(offsetof(EntEnragedUpdateModuleData, m_enragedTime) == 0xC) ? 1 : -1];
typedef char EntEnragedUpdateModuleData_off_m_timeUntilCanRageAgain[(offsetof(EntEnragedUpdateModuleData, m_timeUntilCanRageAgain) == 0x10) ? 1 : -1];
typedef char EntEnragedUpdateModuleData_off_m_scanDistance[(offsetof(EntEnragedUpdateModuleData, m_scanDistance) == 0x14) ? 1 : -1];
typedef char EntEnragedUpdateModuleData_off_m_friendlyDeadFilter[(offsetof(EntEnragedUpdateModuleData, m_friendlyDeadFilter) == 0x18) ? 1 : -1];
typedef char EntEnragedUpdateModuleData_off_m_hatedObjectFilter[(offsetof(EntEnragedUpdateModuleData, m_hatedObjectFilter) == 0x1C) ? 1 : -1];
typedef char EntEnragedUpdateModuleData_off_m_enragedTransitionTime[(offsetof(EntEnragedUpdateModuleData, m_enragedTransitionTime) == 0x20) ? 1 : -1];
typedef char EntEnragedUpdateModuleData_off_m_enragedTransitionFX[(offsetof(EntEnragedUpdateModuleData, m_enragedTransitionFX) == 0x24) ? 1 : -1];
typedef char EntEnragedUpdateModuleData_off_m_enragedOnBuffFX[(offsetof(EntEnragedUpdateModuleData, m_enragedOnBuffFX) == 0x28) ? 1 : -1];
typedef char EntEnragedUpdateModuleData_off_m_enragedOffBuffFX[(offsetof(EntEnragedUpdateModuleData, m_enragedOffBuffFX) == 0x2C) ? 1 : -1];

#endif
