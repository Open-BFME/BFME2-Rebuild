// Module.h: SAGE common base of object and drawable modules. SIZE 0x8.
// The object-module chain, as the ctors install it:
//   0x00 Snapshot's vptr (Module adds no subobject of its own)
//   0x04 m_moduleData
//   0x08 m_object        (ObjectModule, sizeof 0x0C)
//   0x0C BehaviorModuleInterface (BehaviorModule, sizeof 0x10)
//   0x10 UpdateModuleInterface   (UpdateModule, sizeof 0x20)
// so a class with all three bases has its vptrs at 0x00/0x0C/0x10 and its own
// fields at 0x14+. ??0UpdateModule (0x00253390) stores exactly those three.
// Target facts (old repo Code/GameEngine/Include/Common/Module.h): no Module
// vftable survives; ObjectModule's 0x00C07E1C has 10 slots, DrawableModule's
// 11, so Module has 9: Snapshot's 0-3, 4 getModuleNameKey (pure; compared by
// Object::findModule 0x0028B6D6, which is `mov eax,[ecx]; call [eax+0x10]`),
// 5-8 empty `ret` (0x000B3FD0): onObjectCreated, onDrawableBoundToObject,
// preloadAssets (no argument in BFME2), onDelete. The constructors store the
// ModuleData at +4. Snapshot's own table 0x00BBB554 is a deleting destructor and
// three __purecall slots, so it contributes 4 slots and there is no crc slot.
// Destructors: Module's is inline; ObjectModule's and DrawableModule's are out of
// line (Zero Hour defines both in Module.cpp), so each body is the 7-byte
// `mov [this],0x00BBB554; ret` ICF folds into 0x0049B47C. A behavior-module chain
// therefore stores BehaviorModule's two tables ([+0] 0x00BEEA7C, then [+0xC]
// 0x00BEE9C0) and calls or jumps there (~SpecialPowerModule 0x00493DEF,
// ~UpgradeModule 0x0046089D, ~DieModule 0x0045CE54), and a drawable-module chain
// stores its own table and calls there (~LaserUpdate 0x00362F64). Module's own
// vptr store is always dead, and Module is abstract, which is why no Module
// vftable survives.
#pragma once
#ifndef BFME2_MODULE_H
#define BFME2_MODULE_H

#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/Xfer.h"

class Xfer;

class Module : public Snapshot
{
public:
	Module(const ModuleData *moduleData) : m_moduleData(moduleData) {}
	virtual ~Module() {}													// slot 0

	virtual NameKeyType getModuleNameKey() const = 0;						// slot 4
	virtual const char *GetSnapshotName() const { return "Module"; }
	virtual void onObjectCreated() {}										// slot 5
	virtual void onDrawableBoundToObject() {}								// slot 6
	virtual void preloadAssets() {}										// slot 7
	virtual void onDelete() {}												// slot 8

	// Zero Hour Module.h: the INI tag's name key. Object::rva0028F2C4 0x0028F2C4 compares
	// [[module+4]+4] (m_moduleData->m_moduleTagNameKey) against its argument (f04 decl block 22:20).
	NameKeyType getModuleTagNameKey() const { return getModuleData()->getModuleTagNameKey(); }

protected:
	// Snapshot slot 1: Zero Hour's empty out-of-line Module::loadPostProcess. Retail's ObjectModule
	// table 0x00807E1C holds the shared empty `ret` 0x000B3FD0 there, not __purecall (m08 P13 aliases
	// ?loadPostProcess@Module@@MAEXXZ onto 0x000B3FD0). Defined with ObjectModule's constructor.
	virtual void loadPostProcess();
	// Snapshot slot 3: Zero Hour's Module::xfer is the version stamp alone; ObjectModule's and DrawableModule's
	// xfer inline it after their own (retail's shared fold 0x00560AE1: two Version1 calls).
	virtual void xfer(Xfer *xfer) { xfer->Version1(); }
	const ModuleData *getModuleData() const { return m_moduleData; }

	const ModuleData *m_moduleData;										// 0x04
};
// SIZE 0x8

class Drawable;
class Thing;

// The drawable-side base: Module plus the Drawable at +8 (sizeof 0xC).
class DrawableModule : public Module
{
public:
	DrawableModule(Thing *thing, const ModuleData *moduleData);
	virtual ~DrawableModule();												// Module.cpp; folds into 0x0049B47C

	Drawable *getDrawable() const { return m_drawable; }

	// Slots 9 and 10: DrawableModule's table 0x00807E54 (11 slots) adds two
	// getters whose base bodies are the shared `xor eax,eax; ret` 0x000D43D0.
	// W3DFloorDraw and W3DModelDraw override both with 0x000CF155 (this+0x10, a
	// one-slot interface base). The Drawable constructor registers what slot 10
	// returns for every draw, client-update and third-list module it creates.
	// Names unknown.
	virtual void *vslot09() { return 0; }									// slot 9
	virtual void *vslot10() { return 0; }									// slot 10

protected:
	// Snapshot slots 1-3 (table 0x00807E54): 1 the shared empty `ret` 0x000B3FD0;
	// 2 GetSnapshotName 0x00252B84 (the literal class name); 3 xfer 0x00560AE1
	// (two Version1 calls; ICF-folded with ObjectModule's), which DrawModule::xfer
	// 0x004CBF58 and LaserUpdate's xfer call directly.
	virtual void loadPostProcess() {}										// slot 1
	virtual const char *GetSnapshotName() const;							// slot 2
	virtual void xfer(Xfer *xfer);											// slot 3

protected:
	Drawable *m_drawable;													// 0x08
};
// SIZE 0xC

#endif
