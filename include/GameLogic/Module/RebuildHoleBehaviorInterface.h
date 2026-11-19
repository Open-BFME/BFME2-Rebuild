// RebuildHoleBehaviorInterface.h: the five-slot interface RebuildHoleBehavior places after its
// DieModuleInterface vptr (+0x24).
// Target facts (retail game.dat): RebuildHoleBehavior's constructor 0x00483271 stores the folded
// table of every five-slot pure interface (VA 0x00C078DC, all __purecall) at +0x24 before its own
// table (VA 0x00C49A50) goes in. Its slots there: 0 startRebuildProcess (0x004835B4, ledger name
// with this signature), 1 a `mov eax,[ecx+0xC]` getter and 2 a `mov eax,[ecx+8]` getter (folded
// with other classes' one-load getters, so the ledger carries other classes' names for them),
// 3 getRebuildTemplate (0x002A79A1, `mov eax,[ecx+0x18]`, ledger name RebuildHoleBehavior::
// getRebuildTemplate const), 4 setSlot4Flag (0x00210CD3, stores its byte at [ecx+0x20]).
// The interface itself has no member: the interface's `this` is module +0x24, so the getters
// read module +0x30 (the spawner ObjectID), +0x2C (the reconstructing ObjectID), +0x3C (the
// rebuild template) and the setter writes module +0x44.
// Structural inference: slots 1 and 2 are named getSpawnerID and getReconstructingID from the
// ObjectIDs they read (the constructor sets all three IDs; the ledger has no name for either).
#pragma once
#ifndef BFME2_REBUILD_HOLE_BEHAVIOR_INTERFACE_H
#define BFME2_REBUILD_HOLE_BEHAVIOR_INTERFACE_H

#include "Common/GameType.h"

class ThingTemplate;

class RebuildHoleBehaviorInterface
{
public:
	virtual void startRebuildProcess(const ThingTemplate *rebuildTemplate, ObjectID spawnerID) = 0;	// slot 0
	virtual ObjectID getSpawnerID() const = 0;															// slot 1 (PROVISIONAL name)
	virtual ObjectID getReconstructingID() const = 0;													// slot 2 (PROVISIONAL name)
	virtual const ThingTemplate *getRebuildTemplate() const = 0;										// slot 3
	virtual void setSlot4Flag(bool flag) = 0;															// slot 4
};

#endif
