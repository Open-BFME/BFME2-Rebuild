// GeometryUpgrade.h: the upgrade that swaps an object's geometry/sub-objects.
// Target facts: an UpgradeModule (table 0x00858978 fills mux slots 8 and 10 with
// upgradeRemovalImplementation 0x004B6B29 and upgradeImplementation 0x004B6C8A)
// with one AsciiString at +0x1C: xfer 0x004B6C02 transfers it after Version
// (1, 1) and before UpgradeModule::xfer, and ~GeometryUpgrade 0x004B6C55
// releases it before ~UpgradeModule. Its module data's fields start at 0x118.
#pragma once
#ifndef BFME2_GEOMETRYUPGRADE_H
#define BFME2_GEOMETRYUPGRADE_H

#include "Common/AsciiString.h"
#include "GameLogic/Module/UpgradeModule.h"

class GeometryUpgrade : public UpgradeModule
{
public:
	GeometryUpgrade(Thing *thing, const ModuleData *moduleData);
	virtual ~GeometryUpgrade();											// 0x004B6C55

	virtual NameKeyType getModuleNameKey() const;						// 0x004B6ABF
	virtual void upgradeRemovalImplementation();						// 0x004B6B29
	virtual void upgradeImplementation();								// 0x004B6C8A

protected:
	virtual void xfer(Xfer *xfer);										// 0x004B6C02

	AsciiString m_unk0x1C;												// 0x1C
protected:
	virtual const char *GetSnapshotName() const;	// slot 2: the class's own (`mov eax,"GeometryUpgrade"; ret`) (h05 r9)
protected:
	virtual void loadPostProcess() { UpgradeModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpgradeModule's (h05)
};

#endif
