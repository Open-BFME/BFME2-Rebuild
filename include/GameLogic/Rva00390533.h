// Rva00390533.h: the Object+0x25C physics receiver's condition query. SIZE 0xC.
// Target facts: rva00390533 0x00390533 reads its Object at +0x08, tests
// condition 3*32+31 (Object+0x118 bit 31) and then 2*32+8 (Object+0x114 bit 8),
// answering true when either is set. Callers obtain it from Object+0x25C.
#pragma once
#ifndef BFME2_RVA00390533_H
#define BFME2_RVA00390533_H

class Object;

class Rva00390533
{
public:
	Bool rva00390533();

	UnsignedByte m_unk0x00[8];
	Object *m_object;									// 0x08
};
// SIZE 0xC

#endif
