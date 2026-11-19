// WorkOrder.h: one order in a TeamInQueue's list (AIPlayer's build queue). Layout from
// the rows that read it: TeamInQueue::isAllBuilt 0x004F0656, isMinimumBuilt 0x004F06A6,
// areBuildsComplete 0x004F06CD and includesADozer 0x004F10C2 walk the list at +0x0C.
// Snapshot's four slots (table 0x00C62E40, which the constructor 0x004F10E6 stores); the fields sit at the retail
// offsets. The destructor is not written yet. Size 0x30 (SIZE below).
#pragma once
#ifndef BFME2_WORKORDER_H
#define BFME2_WORKORDER_H

#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "Common/AsciiString.h"

class ThingTemplate;
class Player;
class Xfer;

class WorkOrder : public Snapshot
{
public:
	WorkOrder();									// 0x004F10E6 (table 0x00C62E40)
	virtual ~WorkOrder();							// slot 0 (deleting dtor, not yet written)

	void validateFactory(Player *player);			// 0x004F33BD

protected:
	virtual void loadPostProcess() {}				// slot 1, the shared empty body 0x000B3FD0
	virtual char const *GetSnapshotName() const;	// slot 2, 0x004F1116
	virtual void xfer(Xfer *xfer);					// slot 3, 0x004F0716

public:

	const ThingTemplate *m_thing;			// +0x04, KINDOF_DOZER is tested on it by includesADozer; xfer 0x004F0716 writes it through Rva003063A9XferThingTemplate
	ObjectID m_factoryID;								// +0x08, INVALID_ID until a factory takes the order (validateFactory 0x004F33BD resolves it)
	WorkOrder *m_next;								// +0x0C
	Int m_numCompleted;								// +0x10, number built (Zero Hour names, +0x04..+0x19 in Zero Hour's order)
	Int m_numRequired;								// +0x14, number needed (isAllBuilt 0x004F0656 compares it with +0x10)
	Bool m_required;								// +0x18, part of the minimum requirement (isMinimumBuilt 0x004F06A6)
	Bool m_isResourceGatherer;						// +0x19, includesADozer 0x004F10C2 skips gatherers
	unsigned char m_unk0x1A[0x2];					// +0x1A
	Int m_unk0x1C;									// +0x1C, xfer 0x004F0716 writes it
	AsciiString m_unk0x20;							// +0x20, xfer writes it through the AsciiString operator
	UnsignedInt m_unk0x24;							// +0x24, xfer writes it
	Bool m_unk0x28;								// +0x28, xfer writes it
	Bool m_unk0x29;								// +0x29, read by isAllBuilt
	unsigned char m_unk0x2A[0x2];				// +0x2A
	Int m_unk0x2C;								// +0x2C, xfer copies it to a local before writing
};
// SIZE 0x30

// The template-pointer transfer that WorkOrder::xfer calls (retail 0x003063A9, a placeholder name).
void Rva003063A9XferThingTemplate(Xfer *xfer, const ThingTemplate **templ);

#endif
