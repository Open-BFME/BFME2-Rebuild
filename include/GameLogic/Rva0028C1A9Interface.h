// Rva0028C1A9Interface.h: the interface Object::rva0028C1A9 (0x0028C1A9) returns,
// used by BloodthirstyUpdate to claim a sacrifice. Original name not recovered.
// Target facts (old repo BloodthirstyUpdate.cpp): BloodthirstyUpdate calls slot 5
// with an ObjectID (or 0 to release), slot 6 for a state (0 when free) and slot 7
// for the claiming owner's ObjectID.
#pragma once
#ifndef BFME2_RVA0028C1A9INTERFACE_H
#define BFME2_RVA0028C1A9INTERFACE_H

#include "Common/GameType.h"

class Rva0028C1A9Interface
{
public:
	virtual void slot00();						// 0
	virtual void slot01();						// 1
	virtual void slot02();						// 2
	virtual void slot03();						// 3
	virtual void slot04();						// 4
	virtual void release(Int ownerID);			// 5
	virtual Int getState();						// 6
	virtual ObjectID getOwnerID();				// 7
};

#endif
