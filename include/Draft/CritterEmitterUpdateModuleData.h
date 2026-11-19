// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5EA10).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does.
// CritterEmitterUpdateModuleData.h: the INI data of CritterEmitterUpdate. SIZE 0x24.
// Target facts: ModuleFactory registers the module with operator new 0x24 and
// constructor RVA 0x004C8EFF; buildFieldParse RVA 0x004C8FD7 adds VA 0x00C5EA10 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_CRITTER_EMITTER_UPDATE_MODULE_DATA_H
#define BFME2_CRITTER_EMITTER_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include <vector>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class FXList;
class ObjectCreationList;
class MultiIniFieldParse;

class CritterEmitterUpdateModuleData : public ModuleData
{
public:
	CritterEmitterUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	// s02: two pointer vectors. onCollide 0x004C8E83 plays a random entry of +0x8 through
	// FXList::doFXObj and creates a random entry of +0x14 through ObjectCreationList::create;
	// the ctor 0x004C8EFF builds both, the implicit dtor 0x004C8F49 frees both.
	_STL::vector<const FXList *> m_fx;                       // +0x8 FX (iniParseFXListVec)
	_STL::vector<const ObjectCreationList *> m_ocl;          // +0x14
	UnsignedInt m_reloadTime;                              // +0x20 ReloadTime; duration, frames
};
// SIZE 0x24
typedef char CritterEmitterUpdateModuleData_size[(sizeof(CritterEmitterUpdateModuleData) == 0x24) ? 1 : -1];
typedef char CritterEmitterUpdateModuleData_off_m_fx[(offsetof(CritterEmitterUpdateModuleData, m_fx) == 0x8) ? 1 : -1];
typedef char CritterEmitterUpdateModuleData_off_m_reloadTime[(offsetof(CritterEmitterUpdateModuleData, m_reloadTime) == 0x20) ? 1 : -1];

#endif
