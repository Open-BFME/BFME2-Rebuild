// OpaqueRefElement4.h: a four-byte owning handle to an OpaqueRefCounted.
// Target facts (old repo Code/GameEngine/Include/Common/OpaqueRefElement4.h):
// no virtual functions and no base; sizeof 4 (TerrainRoadType's ctor passes
// element size 4 to ??_L). The default ctor nulls +0 inline, the destructor
// releases a non-null pointee through Release_Ref 0x00050ED3 inline, and
// operator= 0x00239099 is out of line.
#pragma once
#ifndef BFME2_OPAQUEREFELEMENT4_H
#define BFME2_OPAQUEREFELEMENT4_H

#include "Common/AtomicRefCounted.h"

// Retail's pointee: the atomic reference-counted base (Release_Ref 0x00050ED3 is
// ?Release_Ref@AtomicRefCounted@@QAEXXZ in the map).
class OpaqueRefCounted : public AtomicRefCounted
{
};

struct OpaqueRefElement4
{
	OpaqueRefElement4() : referent(0) {}
	// Out of line in retail (0x000A8C7C, folded with other handles' copies): stores the pointer and
	// takes a reference through the import (`add eax,4; push eax; call ds:InterlockedIncrement`).
	OpaqueRefElement4(const OpaqueRefElement4 &other) : referent(other.referent) { if (referent) referent->Add_Ref(); }
	~OpaqueRefElement4() { if (referent) referent->Release_Ref(); }
	OpaqueRefElement4 &operator=(const OpaqueRefElement4 &other);
	OpaqueRefElement4 &rva00239057(const OpaqueRefElement4 *other);
	void clear();								// 0x000A8C9B: releases and nulls (Drawable::clearCustomSoundAmbient 0x00279354, GameLogic 0x0023DCCE)

	OpaqueRefCounted *referent;					// 0x0
};
// SIZE 0x4

#endif
