// A byte slot of an owner not yet recovered; the pad stands in for the owner.
#pragma once
#ifndef BFME2_RVA0048F358BYTESLOT_H
#define BFME2_RVA0048F358BYTESLOT_H

#include <stddef.h>

class Rva0048F358ByteSlot
{
public:
	void set(unsigned char value);

private:
	unsigned char m_unk0x000[0x3C1];
	unsigned char m_value;
};

#endif
