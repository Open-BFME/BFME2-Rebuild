// Rva002918E0Object.h: the EntEnragedUpdate module's state surface. Object's
// module lookup returns the module at 0x002918E0; FlammableUpdate notifies it
// when a burn starts or ends (rows 0x004B239A and 0x004B23B3). No source name
// for the type survives, so it keeps the address.
#pragma once
#ifndef BFME2_RVA002918E0OBJECT_H
#define BFME2_RVA002918E0OBJECT_H

#include "Common/GameType.h"

class Rva002918E0Object
{
public:
	void rva004B239A(UnsignedByte a, UnsignedByte b);	// 0x004B239A
	void set(UnsignedByte value);						// 0x004B23B3
};

#endif
