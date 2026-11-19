// AutoPickUpUpdate.h: object scans for and picks up nearby objects. SIZE 0x2C.
// Target facts (old repo Code/GameEngine/Source/GameLogic/Object/Update/AutoPickUpUpdate.cpp):
// the module is 0x2C bytes: UpdateModule (0x20), the AutoPickUpUpdateInterface
// vptr at +0x20, then the scan countdown +0x24 and two flags +0x28/+0x29.
// xfer (0x00495C77) is Version(1, 2), UpdateModule::xfer, +0x24, +0x28, and +0x29
// when the version's minimum is at least 2.
#pragma once
#ifndef BFME2_AUTOPICKUPUPDATE_H
#define BFME2_AUTOPICKUPUPDATE_H

#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/AutoPickUpEatObjectEntryVector.h"

class INI;

class AutoPickUpUpdateModuleData : public ModuleData
{
public:
	AutoPickUpUpdateModuleData();
	virtual ~AutoPickUpUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &p);
	static void iniParseEatObjectEntry(INI *ini, void *instance, void *store, const void *userData);

	Int m_scanDelayTime;					// 0x08
	ObjectFilter m_pickUpFilter;					// 0x0C ObjectFilter m_pickUpFilter
	Real m_scanDistance;					// 0x10
	AutoPickUpEatObjectEntryVector m_eatObjectEntries;			// 0x14 vector<AutoPickUpEatObjectEntry>
	Bool m_autoThrowObject;					// 0x20
	Bool m_runFromButton;					// 0x21
	Int m_runFromButtonNumber;				// 0x24
	Bool m_canScanWhileAttackingOrMoving;	// 0x28
};
// SIZE 0x2C

class AutoPickUpUpdateInterface
{
public:
	virtual int getRunFromButtonNumber() const = 0;			// 0
	virtual unsigned char isRunFromButton() const = 0;			// 1
	virtual void rva00495C6E() = 0;		// 2
	virtual Bool getFlag0x29() const = 0;	// 3: retail 0x0058B826, a byte getter folded with others (`mov al,[ecx+9]`: the +0x29 flag)
};

class AutoPickUpUpdate : public UpdateModule, public AutoPickUpUpdateInterface
{
public:
	AutoPickUpUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~AutoPickUpUpdate();
	virtual NameKeyType getModuleNameKey() const;
	virtual void rva00495C6E();
	virtual UpdateSleepTime update();
	virtual Bool getFlag0x29() const { return m_unk0x29; }	// interface slot 3
	virtual void *slot27() { return static_cast<AutoPickUpUpdateInterface *>(this); }	// BehaviorModuleInterface slot 27: retail 0x004543A9 shape, `lea eax,[ecx-0xC]; ret`
	Bool rva00495DD4();
 virtual int getRunFromButtonNumber() const;
 virtual unsigned char isRunFromButton() const;
 float getScanRange() const { const AutoPickUpUpdateModuleData *scanData=getAutoPickUpUpdateModuleData(); return scanData->m_scanDistance; }

	const AutoPickUpUpdateModuleData *getAutoPickUpUpdateModuleData() const
	{
		return (const AutoPickUpUpdateModuleData *)m_moduleData;
	}

protected:
	virtual const char *GetSnapshotName() const;
	// Own override: the table slot is the fold 0x00497805 of the per-module
	// `{ UpdateModule::loadPostProcess(); }` bodies, not UpdateModule's 0x0058B03E.
	virtual void loadPostProcess();
	virtual void xfer(Xfer *xfer);

private:
	UnsignedInt m_scanCountdown;		// 0x24
	Bool m_unk0x28;						// 0x28 set by rva00495C6E, cleared by update
	Bool m_unk0x29;						// 0x29
};
// SIZE 0x2C

#endif
