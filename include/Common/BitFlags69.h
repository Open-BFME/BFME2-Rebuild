// BitFlags69.h: the 69-kind flag set SAGE passes to Thing::isAnyKindOf.
// Target facts (old repo ThreatFinderUpdate.cpp): the set is seven 32-bit words
// (0x1C bytes), zeroed by its constructor and set bit-by-bit through set().
#pragma once
#ifndef BFME2_BITFLAGS69_H
#define BFME2_BITFLAGS69_H

#include <string.h>

template <int N> class BitFlags;

template <> class BitFlags<69>
{
public:
	BitFlags() { memset(m_words, 0, sizeof(m_words)); }

	void set(Int bit) { m_words[(unsigned int)bit >> 5] |= 1u << (bit & 31); }
	void clear(Int bit) { m_words[(unsigned int)bit >> 5] &= ~(1u << (bit & 31)); }
	Bool test(Int bit) const { return (m_words[(unsigned int)bit >> 5] & (1u << (bit & 31))) != 0; }

	unsigned int m_words[7];
};
// SIZE 0x1C

#endif
