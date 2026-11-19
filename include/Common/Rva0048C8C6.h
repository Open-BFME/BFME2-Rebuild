// Rva0048C8C6.h: the 8-byte record FlammableUpdate's neighbour 0x0048C8C6
// builds. Its constructor stores 0 at +0 and an empty AsciiString at +4
// (0x0048C8C6, 23 bytes, caller 0x0048D671).
#pragma once
#ifndef BFME2_RVA0048C8C6_H
#define BFME2_RVA0048C8C6_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"

class FXList;

class Rva0048C8C6
{
public:
	Rva0048C8C6();

	const FXList *m_00;				// 0x0
	AsciiString m_04;				// 0x4
};
// SIZE 0x8

#endif
