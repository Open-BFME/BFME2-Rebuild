// SpecialDisguiseUpdate.h: a special ability that disguises the unit as another template.
// Target facts (old repo Code/GameEngine/Source/GameLogic/Object/Update/SpecialDisguiseUpdate*.cpp):
//  - module data extends SpecialAbilityUpdateModuleData (0xC8): TriggerInstantlyOnCreate
//    +0xC8, OpacityTarget +0xCC, DisguiseAsTemplate +0xD0, DisguisedAsEnemy +0xD4,
//    DisguiseFX +0xD8, ForceMounted +0xDC;
//  - the module extends SpecialAbilityUpdate (0x88) with a one-slot interface at
//    +0x88 (0x004B0303 fires the ability when the data's +0xC8 flag is set), a
//    float at +0x8C and a bool at +0x90 (xfer 0x004B01F4: Version(1, 2), the bool,
//    the float, then SpecialAbilityUpdate::xfer from version 2).
#pragma once
#ifndef BFME2_SPECIALDISGUISEUPDATE_H
#define BFME2_SPECIALDISGUISEUPDATE_H

#include "GameLogic/Module/SpecialAbilityUpdate.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
#include "Common/AsciiString.h"

class FXList;

class SpecialDisguiseUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	SpecialDisguiseUpdateModuleData();

	Bool m_triggerInstantlyOnCreate;		// 0xC8
	Real m_opacityTarget;					// 0xCC
	AsciiString m_disguiseAsTemplate;		// 0xD0
	AsciiString m_disguisedAsEnemy;			// 0xD4
	const FXList *m_disguiseFX;				// 0xD8
	Bool m_forceMounted;					// 0xDC
};
// SIZE 0xE0

class SpecialDisguiseUpdateInterface
{
public:
	virtual void rva004B0303() = 0;			// 0
};

class SpecialDisguiseUpdate : public SpecialAbilityUpdate, public SpecialDisguiseUpdateInterface
{
public:
	SpecialDisguiseUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~SpecialDisguiseUpdate();
	virtual NameKeyType getModuleNameKey() const;
	virtual void triggerAbilityEffect();
	virtual void rva004B0303();
	virtual CreateModuleInterface *getCreate() { return reinterpret_cast<CreateModuleInterface *>(static_cast<SpecialDisguiseUpdateInterface *>(this)); }
	virtual bool continuePreparation();	// slot 16, 0x004B01E6 (the map spells it rva004B01E6; ICF-shared with the sibling classes)
	virtual bool handlePackingProcessing();

	void *rva004B031E();		// the DisguiseAsTemplate's ThingTemplate
	void *rva004B0333();		// the DisguisedAsEnemy ThingTemplate
	void rva004B04B2(Bool value);
	void rva004B05F5(Bool keep);

	const SpecialDisguiseUpdateModuleData *getSpecialDisguiseUpdateModuleData() const
	{
		return (const SpecialDisguiseUpdateModuleData *)m_moduleData;
	}

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void onExit(Bool a, Bool b);

	Real m_unk0x8C;				// 0x8C
	Bool m_unk0x90;				// 0x90
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialAbilityUpdate's (h05)
};
// SIZE 0x94

#endif
