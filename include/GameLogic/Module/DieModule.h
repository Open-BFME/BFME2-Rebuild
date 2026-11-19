// DieModule.h: the concrete die-module base and its INI data.
// Target facts (retail game.dat):
// - DieModule is BehaviorModule (ctor 0x00253330) plus DieModuleInterface at
//   +0x10 and adds no member (size 0x14). Ctor 0x0045CEBD stores 0x00C41E78 at
//   +0, 0x00C4A650 at +0xC and 0x00C1C780 at +0x10 (the interface's own table:
//   DieModule leaves onDie pure); dtor 0x0045CE54 restores BehaviorModule's and
//   jumps to ~Module. getDie is the shared 0x002533D3 conversion.
// - DieModuleData is ModuleData plus DieMuxData at +8 (field table 0x0085FFA0,
//   DieMuxData::getFieldParse 0x004CE52E), 0x38 bytes: DestroyDie registers
//   it with operator new 0x38 and no table of its own.
#pragma once
#ifndef BFME2_DIEMODULE_H
#define BFME2_DIEMODULE_H

#include "Common/GameType.h"
#include "GameLogic/Module/BehaviorModule.h"
#include "Common/Xfer.h"
#include "GameLogic/Module/DieModuleInterface.h"
#include "GameLogic/Module/DieMuxData.h"
#include "GameLogic/Module/ModuleData.h"

class Thing;
class DamageInfo;

class DieModuleData : public ModuleData
{
public:
	DieMuxData m_dieMuxData;											// 0x08
};
// SIZE 0x38

class DieModule : public BehaviorModule, public DieModuleInterface
{
public:
	DieModule(Thing *thing, const ModuleData *moduleData);				// 0x0045CEBD

	virtual DieModuleInterface *getDie() { return this; }				// 0x002533D3

	const DieModuleData *getDieModuleData() const { return (const DieModuleData *)getModuleData(); }

protected:
	// Out of line in retail (0x0085CEA7, DieModule::isDieApplicable): whether the damage counts for this die module.
	bool isDieApplicable(const DamageInfo *damageInfo) const;

	virtual ~DieModule() {}											// 0x0045CE54: inline and empty (it restores BehaviorModule's tables and jumps to ~Module); a COMDAT in each file that destroys a die module

	virtual const char *GetSnapshotName() const;						// slot 2, 0x0045CEEE: `mov eax,"DieModule"; ret` (the string sits right after the table at 0x00841EA8)

	// Version 1, then BehaviorModule::xfer; ICF-folded with DamageModule::xfer at
	// 0x004CE56D, which RebuildHoleExposeDie::xfer 0x004866F3 calls.
	virtual void xfer(Xfer *xfer)
	{
		xfer->Version1();
		BehaviorModule::xfer(xfer);
	}
protected:
	virtual void loadPostProcess();	// out of line (a jmp to BehaviorModule's), ICF-folded with UpdateModule's at 0x0058B03E; inline, MSVC folds it into each subclass's forwarder and loses retail's second level (0x00497805) (h05)
};
// SIZE 0x14

#endif
