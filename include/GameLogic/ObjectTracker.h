// ObjectTracker.h: one entry of PropagandaTowerBehavior's list of objects in its area of influence.
// SIZE 0xC.
// Target facts (retail game.dat): PropagandaTowerBehavior::xfer 0x00481842 allocates 0xC bytes with
// the global operator new, stores 0 at +4 and +8 and the table 0x00C49174 at +0; that table has one
// slot, a deleting destructor (0x0048195F). update 0x00481C0C and removeAllInfluence 0x00481D4C delete
// entries through it. The ObjectID is at +4 (xfer's XferObjectID argument) and the next link at +8.
// Zero Hour spells this class the same way (EA, GPL-3.0-or-later).
#pragma once
#ifndef BFME2_OBJECTTRACKER_H
#define BFME2_OBJECTTRACKER_H

#include "Common/GameType.h"

class ObjectTracker
{
public:
	ObjectTracker() : objectID(INVALID_ID), next(NULL) {}
	virtual ~ObjectTracker() {}

	ObjectID objectID;			// +0x04
	ObjectTracker *next;		// +0x08
};
// SIZE 0xC
typedef char ObjectTracker_size[(sizeof(ObjectTracker) == 0xC) ? 1 : -1];

#endif
