// DRAFT (h05 modgen): generated from constructor RVA 0x004C0951 and xfer RVA 0x004C0A16; no function
// matches against it yet, so it stays in Draft until one does.
// StructureBody.h: the StructureBody module. SIZE 0x104 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C0951 calls ActiveBody first; it stores the
// tables 0x00C5B660 at +0x0, 0x00C5AD78 at +0xC, 0x00C5B5B0 at +0x10. Members are what xfer (0x004C0A16) transfers after ActiveBody::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004C202F ?rva004C202F@Rva004C202F@@QAEXXZ
//   +0x0 slot 2 -> 0x004C0982 ?Rva004C0982Get@@YAHXZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_STRUCTURE_BODY_H
#define BFME2_STRUCTURE_BODY_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/ActiveBody.h"

class Thing;
class Object;
class ModuleData;
class Xfer;

// ActiveBody ends at 0x100; this class's members follow it.
typedef char StructureBody_base_size[(sizeof(ActiveBody) == 0x100) ? 1 : -1];

class StructureBody : public ActiveBody
{
public:
	StructureBody(Thing *thing, const ModuleData *moduleData);
	void setConstructorObject(Object *obj);					// 0x004C09A1 (Zero Hour's name)
	ObjectID getConstructorObjectID() { return m_constructorObjectID; }
	virtual NameKeyType getModuleNameKey() const;

protected:
	virtual void loadPostProcess() { ActiveBody::loadPostProcess(); }	// slot 1: retail 0x004C202F, a jmp to ActiveBody's (0x004BF40F), ICF-folded across the ActiveBody subclasses
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	ObjectID m_constructorObjectID;               // +0x100; xfer ObjectID; the object that built this structure (Zero Hour StructureBody::m_constructorObjectID)
};
// SIZE 0x104
typedef char StructureBody_size[(sizeof(StructureBody) == 0x104) ? 1 : -1];
typedef char StructureBody_off_m_constructorObjectID[(offsetof(StructureBody, m_constructorObjectID) == 0x100) ? 1 : -1];

#endif
