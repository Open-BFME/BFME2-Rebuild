// TransportStatus.h: the per-transport record ScriptConditions::evaluateUnitHasEmptied keeps
// (Zero Hour's TransportStatus, defined in its ScriptConditions.cpp).
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts:
//  - SIZE 0x14: evaluateUnitHasEmptied 0x003E4860 allocates 0x14 with operator new 0x0002FDA0.
//  - Constructor 0x003E3C22 (inline; the out-of-line copy stores the table 0x00C35B34, then zero
//    at +0x04, +0x08, +0x0C, +0x10 in that order).
//  - One-slot table 0x00C35B34: the deleting destructor 0x003E54F7 (calls 0x003E3C39, then
//    operator delete 0x0002FD60 when bit 0 of the flags is set).
//  - Destructor 0x003E3C39: stores the table back, then deletes m_nextStatus through its slot 0
//    when it is non-null (Zero Hour: m_nextStatus->deleteInstance(); BFME 2 has no memory pools,
//    so the delete is the global one).
//  - The list head is a file static of ScriptConditions.cpp (Zero Hour's s_transportStatuses,
//    VA 0x00E02E00); ScriptConditions::reset 0x003E3C66 deletes it.
#pragma once
#ifndef BFME2_TRANSPORTSTATUS_H
#define BFME2_TRANSPORTSTATUS_H

#include "Common/GameType.h"

class TransportStatus
{
public:
	TransportStatus *m_nextStatus;	// +0x04
	ObjectID m_objID;		// +0x08
	UnsignedInt m_frameNumber;	// +0x0C
	Int m_unitCount;		// +0x10

public:
	TransportStatus() : m_objID(INVALID_ID), m_frameNumber(0), m_unitCount(0), m_nextStatus(NULL) {}
	virtual ~TransportStatus();	// 0x003E3C39
};
// SIZE 0x14

#endif
