// StructureCollapseUpdateModuleData.h: the INI data of StructureCollapseUpdate. SIZE 0xFC.
// Target facts: ModuleFactory registers the module with operator new 0xFC and
// constructor RVA 0x00256ACC; buildFieldParse RVA 0x004A4BF7 adds VA 0x00C52790 at +0x0 and DieMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_STRUCTURE_COLLAPSE_UPDATE_MODULE_DATA_H
#define BFME2_STRUCTURE_COLLAPSE_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/DieMuxData.h"
#include <vector>

class INI;
class MultiIniFieldParse;
class FXList;
class ObjectCreationList;

// Constructor 0x00256ACC and destructor 0x00256B94 build and destroy five
// 0xC STLport vectors of each kind, which fixes the arrays at +0x54/+0x90
// and the count arrays at +0xCC/+0xE0.
enum { COLLAPSE_PHASE_COUNT = 5 };
typedef std::vector<const ObjectCreationList *> OCLVec;
typedef std::vector<const FXList *> FXVec;

class StructureCollapseUpdateModuleData : public ModuleData
{
public:
	StructureCollapseUpdateModuleData();
	// 0x00256B94
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);	// 0x00256BEA
	static void parseOCL(INI *ini, void *instance, void *store, const void *userData);
	static void parseFXList(INI *ini, void *instance, void *store, const void *userData);

	DieMuxData m_dieMuxData;                               // +0x8; added by buildFieldParse at +0x8
	UnsignedInt m_minCollapseDelay;                        // +0x38 MinCollapseDelay; duration, frames
	UnsignedInt m_maxCollapseDelay;                        // +0x3C MaxCollapseDelay; duration, frames
	UnsignedInt m_minBurstDelay;                           // +0x40 MinBurstDelay; duration, frames
	UnsignedInt m_maxBurstDelay;                           // +0x44 MaxBurstDelay; duration, frames
	Int m_bigBurstFrequency;                               // +0x48 BigBurstFrequency
	Real m_collapseDamping;                                // +0x4C CollapseDamping
	Real m_maxShudder;                                     // +0x50 MaxShudder
	OCLVec m_ocls[COLLAPSE_PHASE_COUNT];                   // +0x54
	FXVec m_fxs[COLLAPSE_PHASE_COUNT];                     // +0x90
	Int m_oclCount[COLLAPSE_PHASE_COUNT];                  // +0xCC
	Int m_fxCount[COLLAPSE_PHASE_COUNT];                   // +0xE0
	Bool m_destroyObjectWhenDone;                          // +0xF4 DestroyObjectWhenDone
	Real m_collapseHeight;                                 // +0xF8 CollapseHeight
};
// SIZE 0xFC
typedef char StructureCollapseUpdateModuleData_size[(sizeof(StructureCollapseUpdateModuleData) == 0xFC) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_dieMuxData[(offsetof(StructureCollapseUpdateModuleData, m_dieMuxData) == 0x8) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_minCollapseDelay[(offsetof(StructureCollapseUpdateModuleData, m_minCollapseDelay) == 0x38) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_maxCollapseDelay[(offsetof(StructureCollapseUpdateModuleData, m_maxCollapseDelay) == 0x3C) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_minBurstDelay[(offsetof(StructureCollapseUpdateModuleData, m_minBurstDelay) == 0x40) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_maxBurstDelay[(offsetof(StructureCollapseUpdateModuleData, m_maxBurstDelay) == 0x44) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_bigBurstFrequency[(offsetof(StructureCollapseUpdateModuleData, m_bigBurstFrequency) == 0x48) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_collapseDamping[(offsetof(StructureCollapseUpdateModuleData, m_collapseDamping) == 0x4C) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_maxShudder[(offsetof(StructureCollapseUpdateModuleData, m_maxShudder) == 0x50) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_ocls[(offsetof(StructureCollapseUpdateModuleData, m_ocls) == 0x54) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_fxs[(offsetof(StructureCollapseUpdateModuleData, m_fxs) == 0x90) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_oclCount[(offsetof(StructureCollapseUpdateModuleData, m_oclCount) == 0xCC) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_fxCount[(offsetof(StructureCollapseUpdateModuleData, m_fxCount) == 0xE0) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_destroyObjectWhenDone[(offsetof(StructureCollapseUpdateModuleData, m_destroyObjectWhenDone) == 0xF4) ? 1 : -1];
typedef char StructureCollapseUpdateModuleData_off_m_collapseHeight[(offsetof(StructureCollapseUpdateModuleData, m_collapseHeight) == 0xF8) ? 1 : -1];

#endif
