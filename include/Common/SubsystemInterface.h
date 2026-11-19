// SubsystemInterface.h: the base of every engine subsystem (Zero Hour's
// Common/SubsystemInterface.h; BFME 2 builds it from
// Code\Libraries\Source\subsystem\SubsystemInterface.cpp, the path WorldBuilder's
// asserts carry).
//
// Layout (binary): ctor 0x001B4E63 stores vftable 0x007D77A0 at +0, nulls the
// AsciiString at +8, then clears the byte at +4; dtor 0x001B4E74 restores the
// vftable and releases +8. SubsystemInterfaceList 0x001B4E21 sets +4 to 1 around
// its attemptReload call and back to 0 afterwards. Allocation: 0xC.
//
// Virtual order (binary): vftable 0x007D77A0, 14 slots. Retail folds the empty
// default bodies (0x000B3FD0 bare ret, 0x005CB9FF `xor al,al; ret 4`, 0x0047A699
// `xor al,al; ret`, 0x000D43D0 `xor eax,eax; ret`, 0x0047A69C `ret 4`), so the map
// names beside those addresses belong to other classes. WorldBuilder (unfolded,
// incrementally linked) shows the split between header and .cpp:
//  - slots 3-8 and 11 are one run of COMDAT bodies (0x004A3800..0x004A3870) far
//    from SubsystemInterface.cpp: inline in this header;
//  - slots 2, 12, 13 sit beside the constructor in SubsystemInterface.cpp
//    (0x00675D00, 0x00674C00 "SubsystemInterface::draw called - unimplemented for
//    this class!", 0x00674C50 "SubsystemInterface::updatePhase called -
//    unimplemented for this class."): out of line;
//  - slots 1, 9, 10 are _purecall.
// The implicit copy constructor and assignment are emitted as COMDATs in other
// objects (0x00329F5E, 0x00329880), so neither is declared here.
#pragma once
#ifndef BFME2_SUBSYSTEMINTERFACE_H
#define BFME2_SUBSYSTEMINTERFACE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"

struct FieldParse;

class SubsystemInterface
{
public:
	SubsystemInterface();										// 0x001B4E63
	virtual ~SubsystemInterface();								// slot 0: ??_G 0x001B4F51, dtor 0x001B4E74

	virtual void init() = 0;									// slot 1 (0x04): _purecall; name: donor (ZH)
	// slot 2 (0x08): 0x001B5384, out of line (loads the subsystem's INI files from
	// its SubsystemLegend directive); SubsystemInterfaceList::initSubsystem
	// 0x001B5266 calls slots 1 and 2. Name: map identity of the unfolded body.
	virtual Bool loadIniFilesFromLegend();
	// slot 3 (0x0C): void(), inline empty. Name: donor (ZH); ThingFactory
	// 0x002CF5FF and WeaponStore 0x002CADFA override it, and
	// SubsystemInterfaceList::postProcessLoadAll 0x001B4E49 dispatches it.
	virtual void postProcessLoad() {}
	// slot 4 (0x10): Bool(Bool *), inline `return false`. WorldBuilder asserts name
	// the overrides "UpgradeCenter::attemptReload" and
	// "ExperienceLevelStore::attemptReload". SubsystemInterfaceList 0x001B4E21
	// passes the address of a Bool the Locomotor store sets when a map restart
	// is needed.
	virtual Bool attemptReload(Bool *needsRestart) { return false; }
	// slot 5 (0x14): Bool(), inline `return false`. WorldBuilder assert
	// "PlayerTemplateStore::attemptRefresh"; dispatched twice by 0x001B5140.
	virtual Bool attemptRefresh() { return false; }
	// slot 6 (0x18): pointer(), inline `return 0`. GlobalData's override 0x002376C6
	// returns its FieldParse table. Name: structural (ZH's spelling of that
	// accessor elsewhere); constness unknown.
	virtual const FieldParse *getFieldParse() const { return NULL; }
	// slot 7 (0x1C): void(Bool), inline empty. SubsystemInterfaceList 0x001B505F
	// calls it on every subsystem that reloaded, passing the restart Bool;
	// WeaponStore's override 0x002C941C re-runs postProcessLoad. Name: structural.
	virtual void postReload(Bool needsRestart) {}
	// slot 8 (0x20): void(), inline empty. SubsystemInterfaceList 0x001B4FE9 calls
	// it on each pending entry before clearing that list; ExperienceLevelSystem's
	// override 0x002886A2 clears its reload latch (+0x24). Name: structural.
	virtual void clearReloadState() {}

	virtual void reset() = 0;									// slot 9 (0x24): _purecall; name: donor (ZH)
	virtual void update() = 0;									// slot 10 (0x28): _purecall; name: donor (ZH)

	// slot 11 (0x2C): Bool(const char *), inline `return false`. 0x001B4EAB calls it
	// on each subsystem with a "%s_%02d.csv" file name built from m_name, when a
	// GlobalData switch (+0x9AC) is set (GameEngine 0x002304BB). Name: structural.
	virtual Bool dumpToCSV(const char *fileName) { return false; }
	// slot 12 (0x30): out of line; WorldBuilder assert "SubsystemInterface::draw".
	// Release body is empty and folds to 0x000B3FD0.
	virtual void draw();
	// slot 13 (0x34): out of line; WorldBuilder assert
	// "SubsystemInterface::updatePhase". Release body is empty and folds to
	// 0x0047A69C (ret 4); GameLogic's override is 0x0024555A.
	virtual void updatePhase(Int phase);

	const AsciiString &getName() const { return m_name; }		// name: donor (ZH), inline there
	void setName(AsciiString name);								// 0x0006F3CC, out of line, takes the string by value (FontLibrary ctor 0x00218942)

	// SubsystemInterfaceList reads and writes both fields directly (0x001B4E21, 0x001B4EAB).
	friend class SubsystemInterfaceList;
protected:
	Bool m_reloading;		// +0x04 set only around attemptReload (0x001B4E21); ctor clears it. Name: structural
	AsciiString m_name;		// +0x08 the subsystem name (formatted into the CSV name by 0x001B4EAB)
};
// SIZE 0xC

#endif
