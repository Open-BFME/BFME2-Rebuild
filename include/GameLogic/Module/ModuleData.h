// ModuleData.h: SAGE base of every module's INI data. SIZE 0x8.
// Target facts (old repo Code/GameEngine/Include/Common/ModuleData.h):
// shared tables 0x00C4ED70 / 0x00C549F8 have 31 slots: 0 deleting destructor,
// 1 loadPostProcess (empty), 2 GetSnapshotName, 3 xfer (empty, ret 4),
// 4-13 and 15 return false, 14 is getBestModelNameForWB (0x000B69A1: the empty
// string by value; the five W3D draw module-data tables override it with
// 0x000B6E28, Zero Hour's WorldBuilder hook, m07 P10), 16-29 return null, 30 returns true. ThingTemplate tests slot 4
// (isAiModuleData), slot 18/19 the draw-data getters, 21 getHordeModuleData,
// 22 getMinimumRequiredGameLOD. The module tag key is at +4
// (ModuleFactory::newModuleDataFromINI 0x00257232). The destructor is inline
// and empty: retail's module-data destructor chains collapse it together with
// Snapshot's into a bare `mov [this],0x00BBB554` with no call
// (??1StrafeAreaUpdateModuleData 0x003A4ECF). Its COMDAT copy is the 7-byte
// 0x0049B47C, folded with ~Snapshot.
#pragma once
#ifndef BFME2_MODULEDATA_H
#define BFME2_MODULEDATA_H

#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "Common/AsciiString.h"
#include "Common/ModelConditionFlags.h"	// slot 14's argument (a typedef of BitFlags once k07's u1 lands, so no forward declaration)

class Xfer;
class HordeContainModuleData;
class HordeGarrisonContainModuleData;
class MultiIniFieldParse;
class W3DModelDrawModuleData;
class W3DPropDrawModuleData;

class ModuleData : public Snapshot
{
public:
	ModuleData() {}
	virtual ~ModuleData() {}											// slot 0; COMDAT 0x0049B47C
	virtual void loadPostProcess() {}									// slot 1
	virtual const char *GetSnapshotName() const { return "ModuleData"; }	// slot 2, 0x00065212
	virtual void xfer(Xfer *) {}										// slot 3

	virtual Bool isAiModuleData() const { return false; }				// slot 4
	virtual Bool slot05() const { return false; }
	virtual Bool slot06() const { return false; }
	virtual Bool slot07() const { return false; }
	virtual Bool slot08() const { return false; }
	virtual Bool slot09() const { return false; }
	virtual Bool slot10() const { return false; }
	virtual Bool slot11() const { return false; }
	virtual Bool slot12() const { return false; }
	virtual Bool slot13() const { return false; }
	virtual AsciiString getBestModelNameForWB(const ModelConditionFlags &) const;	// slot 14, 0x000B69A1
	virtual Bool slot15() const { return false; }
	virtual const void *slot16() const { return 0; }
	virtual const void *slot17() const { return 0; }
	virtual const W3DModelDrawModuleData *getAsW3DModelDrawModuleData() const { return 0; }	// slot 18
	virtual const W3DPropDrawModuleData *getAsW3DPropDrawModuleData() const { return 0; }	// slot 19
	virtual const void *slot20() const { return 0; }
	virtual const HordeContainModuleData *getHordeModuleData() const { return 0; }	// slot 21
	virtual Int getMinimumRequiredGameLOD() const { return 0; }		// slot 22
	virtual const void *slot23() const { return 0; }
	virtual const void *slot24() const { return 0; }
	virtual const void *slot25() const { return 0; }
	virtual const void *slot26() const { return 0; }
	virtual const void *slot27() const { return 0; }
	virtual const void *slot28() const { return 0; }
	virtual const HordeGarrisonContainModuleData *slot29() const { return 0; }
	virtual Bool slot30() const { return true; }						// slot 30

	void setModuleTagNameKey(NameKeyType key) { m_moduleTagNameKey = key; }
	NameKeyType getModuleTagNameKey() const { return m_moduleTagNameKey; }
	static void buildFieldParse(MultiIniFieldParse &) {}

private:
	NameKeyType m_moduleTagNameKey;	// 0x04
};
// SIZE 0x8

#endif
