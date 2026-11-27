// A byte field of an owner not yet recovered; the pad stands in for the owner.
#pragma once
#ifndef BFME2_RVA0054C7FEBYTEFIELD_H
#define BFME2_RVA0054C7FEBYTEFIELD_H

#include <stddef.h>

class Rva0054C7FEByteField
{
public:
	unsigned char get() const;

private:
	unsigned char m_unk0x000[0x312];
	unsigned char m_value;
};

#endif
