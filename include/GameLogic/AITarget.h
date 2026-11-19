// AITarget.h: one target the skirmish AI's target chooser hands to its tactics. SIZE 0x3C, no vtable.
// Layout from the retail disassembly (the constructor's stores and the xfer's field order; not a donor declaration):
//  - the constructor stores its owner argument at +0x00 and the others at the offsets below; there are no vptr stores
//    and no base calls (ctor 0x002C5C1F, dtor 0x002C589B, which deletes the +0x24 pointee).
//  - AITactic reads +0x04 (initializeTeamTemplate 0x004ECE61 tests it against zero; the expansion test 0x004EDA60 against one).
//  - +0x08 holds a GameLogic frame (copied from the logic frame counter), +0x0C a Coord3D position, +0x18 and +0x19 are
//    bytes (preUpdate 0x004EDF03 tests the +0x19 flag), +0x24 is the threat finder's pointer (calls at 0x002C597B and 0x002C5D75).
// Methods the map names but this header does not declare yet: markApproachHazard 0x002C590F, checkThreatInArea 0x002C59CE,
// rva002C5CF7 0x002C5CF7, setTarget 0x002C5D8B.
#pragma once
#ifndef BFME2_AITARGET_H
#define BFME2_AITARGET_H

#include "Common/Coord3D.h"

class AITarget
{
public:
	void *m_owner;					// +0x00
	unsigned int m_state;			// +0x04
	unsigned int m_frame;			// +0x08
	Coord3D m_position;				// +0x0C
	unsigned char m_flag18;			// +0x18
	unsigned char m_flag19;			// +0x19
	int m_1C;						// +0x1C
	int m_20;						// +0x20
	void *m_threatFinder;			// +0x24 (the pointee is deleted by the destructor; its type is not named yet)
	float m_28;						// +0x28
	unsigned int m_2C;				// +0x2C
	int m_30;						// +0x30
	int m_34;						// +0x34
	unsigned int m_38;				// +0x38
};
// SIZE 0x3C
typedef char AITargetSizeCheck[(sizeof(AITarget) == 0x3C) ? 1 : -1];

#endif
