// DozerAIUpdateCallees.h: the map's placeholder classes for functions DozerAIUpdate's rows call.
// Each class here is the map's own spelling of a method that is really a member of another class
// (an ICF fold or an unrecovered class); they are declared only so the call mangles to the map's name.
// Remove an entry when the map renames its method.
#pragma once
#ifndef BFME2_DRAFT_DOZERAIUPDATECALLEES_H
#define BFME2_DRAFT_DOZERAIUPDATECALLEES_H
#include "Common/GameType.h"

// The skirmish AI's per-player builder record (0x004EC2D4 is its method; Draft/Rva002A8F24.h forward-declares
// the record type that TheSkirmishAIManager hands out).
class AIBuilder
{
public:
	void rva004EC2D4(Int objectID);	// 0x004EC2D4
};

// Player's method at 0x002ACECC (called with the controlling Player as `this`; Player.cpp carries it as
// Rva002ACECC::rva002ACECC).
class Rva002ACECC
{
public:
	void rva002ACECC(Int value);	// 0x002ACECC
};

#endif
