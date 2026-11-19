// HealContain.h: a contain that heals its occupants over time. SIZE 0xFC.
//
// Target facts: the constructor 0x00466AEB calls OpenContain's 0x004649F8 and
// only re-installs tables (primary 0x00C43ED0, update 0x00C43EC0, contain
// 0x00C43D48), so the class adds no members; xfer 0x00466BE9 writes its
// version and defers to OpenContain::xfer. Its module data is
// HealContainModuleData (TimeForFullHeal at +0x98).
#pragma once
#ifndef BFME2_HEALCONTAIN_H
#define BFME2_HEALCONTAIN_H

#include "GameLogic/Module/OpenContain.h"
#include "GameLogic/Module/HealContainModuleData.h"

class HealContain : public OpenContain
{
public:
	HealContain(Thing *thing, const ModuleData *moduleData);			// 0x00466AEB

	const HealContainModuleData *getHealContainModuleData() const
	{
		return (const HealContainModuleData *)getModuleData();
	}

	virtual Bool containSlot06() { return true; }
	virtual NameKeyType getModuleNameKey() const;						// 0x00466B88
	virtual UpdateSleepTime update();

protected:
	virtual ~HealContain();
	virtual const char *GetSnapshotName() const;						// 0x00466B3F
	virtual void xfer(Xfer *xfer);										// 0x00466BE9
protected:
	virtual void loadPostProcess() { OpenContain::loadPostProcess(); }	// retail slot 0x004783D7: a jmp to OpenContain's, ICF-folded across classes (h05)
};
// SIZE 0xFC

typedef char HealContain_size[(sizeof(HealContain) == 0xFC) ? 1 : -1];

#endif
