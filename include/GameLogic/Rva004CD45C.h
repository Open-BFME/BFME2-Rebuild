// Rva004CD45C.h: the "StoreObjectsSpecialPower" lookup 0x004CD45C walks, as the
// teleport-to-caster ability reaches it. Original names not recovered.
// Target facts: 0x004CD45C keeps the module list at +0x08 and scans it for the
// entry whose key matches the interned "StoreObjectsSpecialPower" name; an entry
// answers getKey through slot 4 and keeps its ObjectID vector at +0x88.
#pragma once
#ifndef BFME2_RVA004CD45C_H
#define BFME2_RVA004CD45C_H

#include "Common/GameType.h"

class Rva004CD45CItem
{
public:
	virtual void slot00();						// 0
	virtual void slot01();						// 1
	virtual void slot02();						// 2
	virtual void slot03();						// 3
	virtual NameKeyType getKey();				// 4
	unsigned char m_pad[0x88 - 4];				// 0x04
	UnsignedInt *m_vecBeg;						// 0x88
	UnsignedInt *m_vecEnd;						// 0x8C
};

struct Rva004CD45CMid
{
	unsigned char m_pad[0x244];					// 0x000
	Rva004CD45CItem **m_list;					// 0x244
};

class Rva004CD45C
{
public:
	unsigned char m_pad0[8];					// 0x00
	Rva004CD45CMid *m_mid;						// 0x08

	void *rva004CD45C();
};

#endif
