// BfmeConditionMasks.h: the 76-byte condition-mask builders Object's condition
// setters take. Identities are not recovered; each name is its address.
// Target facts (old repo DockUpdate.cpp onExitReached 0x0058977F / onEnterReached
// 0x00589DDD): Object::rva0028CFB2 (0x0028CFB2) takes two 19-word masks, one
// built by the 0x001E4912 builder (two bits) and one by the 0x0028F59A
// constructor (one bit); the setters pass 0x53/0x51 with 0x54 and 0x52/0x51
// with 0x54.
#pragma once
#ifndef BFME2_BFMECONDITIONMASKS_H
#define BFME2_BFMECONDITIONMASKS_H

#include "Common/GameType.h"
#include <string.h>

// The one-, four- and eight-condition builders (0x0028F59A, 0x0028F5C6, 0x002652AC) were placeholder structs here;
// they are ModelConditionFlags' (kInit, ...) constructors now (Common/ModelConditionFlags.h).

// 0x001E4912: 19 words holding two bits, built in place.
class Rva001E4912
{
public:
	Rva001E4912() {}
	Rva001E4912 *rva001E4912(Int unused, UnsignedInt first, UnsignedInt second);
	UnsignedInt m_bits[19];		// 0x00
};
// SIZE 0x4C

// cancelDock's clear mask: zeroed then four bits set. The memset stays a call to
// msvcr71's memset (old repo DockUpdate.cpp cancels through it).
class RvaDockClearMask
{
public:
	RvaDockClearMask() { memset(this, 0, sizeof(*this)); }
	void set(Int bit) { m_words[bit >> 5] |= 1U << (bit & 31); }
	UnsignedInt m_words[19];	// 0x00
};
// SIZE 0x4C

#endif
