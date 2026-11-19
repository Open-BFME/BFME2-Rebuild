// AutoFindHealingUpdate.h: the object scans for damaged friends and heals them.
// Target facts (AutoFindHealingUpdate object O00495087, read off the bodies):
// - the module data is 0x18 bytes: ModuleData (0x08) then the scan rate +0x08,
//   the scan range +0x0C, and the two heal thresholds +0x10 / +0x14. Its
//   constructor zeroes the rate and range and seeds the thresholds with 0.95
//   (0x00BC4DD4) and 0.25 (0x00BBB8D4).
// - the field table at 0x00C4EC68 is ScanRate (parseDurationUnsignedInt, +0x08),
//   ScanRange (parseReal, +0x0C), NeverHeal (parseReal, +0x10) and AlwaysHeal
//   (parseReal, +0x14), then the null terminator.
// - the update is UpdateModule (0x20) plus one int at +0x20, which xfer carries
//   after the version and the base class.
#pragma once
#ifndef BFME2_AUTOFINDHEALINGUPDATE_H
#define BFME2_AUTOFINDHEALINGUPDATE_H

#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/INI/FieldParse.h"

class AutoFindHealingUpdateModuleData : public ModuleData
{
public:
	AutoFindHealingUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedInt m_scanRate;			// +0x08
	Real m_scanRange;				// +0x0C
	Real m_neverHeal;				// +0x10
	Real m_alwaysHeal;				// +0x14
};
// SIZE 0x18

class AutoFindHealingUpdate : public UpdateModule
{
public:
	AutoFindHealingUpdate(Thing *thing, const ModuleData *moduleData);

	virtual NameKeyType getModuleNameKey() const;

protected:
	virtual void xfer(Xfer *xfer);
	virtual const char *GetSnapshotName() const;
	Object *scanClosestTarget();		// 0x00495183 (Zero Hour's name): the closest healer in range

	Int m_nextScanFrame;			// +0x20
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
public:
	virtual UpdateSleepTime update();	// 0x0049520B (UpdateModuleInterface slot 0)
};
// SIZE 0x24

#endif
