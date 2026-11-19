// Promoted from Draft by f10 (2026-11-17): generated from retail's field-parse table (VA 0x00C56858); constructor 0x004B115A and
// destructor 0x004B1285 are written against it. The 12 bytes at +0x34 are an STLport vector of EmotionTrackerUpdateEntry pointers
// (IniParseAddEmotion 0x004B2087 pushes through it at +0x34; the destructor deletes each entry, then the buffer).
// EmotionTrackerUpdateModuleData.h: the INI data of EmotionTrackerUpdate. SIZE 0x40.
// Target facts: ModuleFactory registers the module with operator new 0x40 and
// constructor RVA 0x004B115A; buildFieldParse RVA 0x004B235D adds VA 0x00C56858 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_EMOTION_TRACKER_UPDATE_MODULE_DATA_H
#define BFME2_EMOTION_TRACKER_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"
#include <vector>

class INI;
class EmotionTrackerUpdateEntry;
class MultiIniFieldParse;

class EmotionTrackerUpdateModuleData : public ModuleData
{
public:
	EmotionTrackerUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static void IniParseAddEmotion(INI *ini, void *instance, void *store, const void *userData);

	Real m_tauntAndPointDistance;                          // +0x8 TauntAndPointDistance
	UnsignedInt m_tauntAndPointUpdateDelay;                // +0xC TauntAndPointUpdateDelay; duration, frames
	ObjectFilter m_tauntAndPointExcluded;                  // +0x10 TauntAndPointExcluded
	ObjectFilter m_afraidOf;                               // +0x14 AfraidOf
	ObjectFilter m_alwaysAfraidOf;                         // +0x18 AlwaysAfraidOf
	ObjectFilter m_pointAt;                                // +0x1C PointAt
	Real m_heroScanDistance;                               // +0x20 HeroScanDistance
	Real m_fearScanDistance;                               // +0x24 FearScanDistance
	Real m_quarrelProbability;                             // +0x28 QuarrelProbability; percent
	Bool m_ignoreVeterancy;                                // +0x2C IgnoreVeterancy
	Int m_immuneToFearLevel;                               // +0x30 ImmuneToFearLevel
	_STL::vector<EmotionTrackerUpdateEntry *> m_entries;   // +0x34 (begin, end, end of storage)
	~EmotionTrackerUpdateModuleData();
};
// SIZE 0x40
typedef char EmotionTrackerUpdateModuleData_size[(sizeof(EmotionTrackerUpdateModuleData) == 0x40) ? 1 : -1];
typedef char EmotionTrackerUpdateModuleData_off_m_tauntAndPointDistance[(offsetof(EmotionTrackerUpdateModuleData, m_tauntAndPointDistance) == 0x8) ? 1 : -1];
typedef char EmotionTrackerUpdateModuleData_off_m_tauntAndPointUpdateDelay[(offsetof(EmotionTrackerUpdateModuleData, m_tauntAndPointUpdateDelay) == 0xC) ? 1 : -1];
typedef char EmotionTrackerUpdateModuleData_off_m_tauntAndPointExcluded[(offsetof(EmotionTrackerUpdateModuleData, m_tauntAndPointExcluded) == 0x10) ? 1 : -1];
typedef char EmotionTrackerUpdateModuleData_off_m_afraidOf[(offsetof(EmotionTrackerUpdateModuleData, m_afraidOf) == 0x14) ? 1 : -1];
typedef char EmotionTrackerUpdateModuleData_off_m_alwaysAfraidOf[(offsetof(EmotionTrackerUpdateModuleData, m_alwaysAfraidOf) == 0x18) ? 1 : -1];
typedef char EmotionTrackerUpdateModuleData_off_m_pointAt[(offsetof(EmotionTrackerUpdateModuleData, m_pointAt) == 0x1C) ? 1 : -1];
typedef char EmotionTrackerUpdateModuleData_off_m_heroScanDistance[(offsetof(EmotionTrackerUpdateModuleData, m_heroScanDistance) == 0x20) ? 1 : -1];
typedef char EmotionTrackerUpdateModuleData_off_m_fearScanDistance[(offsetof(EmotionTrackerUpdateModuleData, m_fearScanDistance) == 0x24) ? 1 : -1];
typedef char EmotionTrackerUpdateModuleData_off_m_quarrelProbability[(offsetof(EmotionTrackerUpdateModuleData, m_quarrelProbability) == 0x28) ? 1 : -1];
typedef char EmotionTrackerUpdateModuleData_off_m_ignoreVeterancy[(offsetof(EmotionTrackerUpdateModuleData, m_ignoreVeterancy) == 0x2C) ? 1 : -1];
typedef char EmotionTrackerUpdateModuleData_off_m_entries[(offsetof(EmotionTrackerUpdateModuleData, m_entries) == 0x34) ? 1 : -1];
typedef char EmotionTrackerUpdateModuleData_off_m_immuneToFearLevel[(offsetof(EmotionTrackerUpdateModuleData, m_immuneToFearLevel) == 0x30) ? 1 : -1];

#endif
