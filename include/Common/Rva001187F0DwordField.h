// A dword field of an owner not yet recovered; the pad stands in for the owner.
#pragma once
#ifndef BFME2_RVA001187F0DWORDFIELD_H
#define BFME2_RVA001187F0DWORDFIELD_H

#include <stddef.h>

class Rva001187F0DwordField
{
public:
	int get() const;

private:
	unsigned char m_unk0x000[0x2B0];
	int m_value;
};

#endif
