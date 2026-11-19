// AnimalAIUpdate.h: the animal AI update module.
// Target facts:
// - Constructor 0x0047ECA8 calls AIUpdateInterface's, stores the class tables
//   0x00C47B98 (+0), 0x00C47B8C (+0x10), 0x00C47B88 (+0x20), keeps
//   AIUpdateInterface's 0x00BFA3C0 (+0xC) and 0x00C52F88 (+0x24), and stores
//   0x00C47B84 at +0x3E4: a one-slot interface right after AIUpdateInterface.
//   Its slot 0x0047EC5F takes the interface as this and returns the module
//   data's AfraidOfCastles byte (+0x7C, h05's AnimalAIUpdateModuleData).
//   The interface's retail name is not recovered; AnimalAIInterface is a role name.
// - xfer 0x0047EBAF: version 1, XferObjectID +0x3E8, Bool +0x3F8, Coord3D +0x3EC,
//   Bool +0x3F9, then AIUpdateInterface::xfer.
// - makeStateMachine 0x0047EC69 returns a new 0x68-byte AIStateMachine
//   (owner, 0xD7F450E0).
#pragma once
#ifndef BFME2_ANIMALAIUPDATE_H
#define BFME2_ANIMALAIUPDATE_H

#include "GameLogic/Module/AIUpdate.h"

class AnimalAIInterface
{
public:
	virtual Bool isAfraidOfCastles() const = 0;	// slot 0
};
// SIZE 0x4

class AnimalAIUpdate : public AIUpdateInterface, public AnimalAIInterface
{
public:
	AnimalAIUpdate(Thing *thing, const ModuleData *moduleData);	// 0x0047ECA8

	virtual NameKeyType getModuleNameKey() const;			// 0x0047EC1A
	virtual Bool isAfraidOfCastles() const;					// 0x0047EC5F
	virtual AnimalAIInterface *getAnimalAIInterface() { return this; }	// slot 96, 0x004A9B51 (shared)

protected:
	virtual void xfer(Xfer *xfer);							// 0x0047EBAF
	virtual AIStateMachine *makeStateMachine();				// 0x0047EC69

	ObjectID m_unk0x3E8;									// +0x3E8
	Coord3D m_unk0x3EC;										// +0x3EC
	Bool m_unk0x3F8;										// +0x3F8
	Bool m_unk0x3F9;										// +0x3F9
protected:
	virtual const char *GetSnapshotName() const;	// slot 2 of the class table, 0x0047EBA9 (h05 r9)
};
// SIZE 0x3FC

#endif
