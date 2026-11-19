// WorkerAIUpdateCallees.h: the map's placeholder classes for functions WorkerAIUpdate's rows call.
#pragma once
#ifndef BFME2_DRAFT_WORKERAIUPDATECALLEES_H
#define BFME2_DRAFT_WORKERAIUPDATECALLEES_H
#include "Common/GameType.h"

// Drawable's supply-box broadcast at 0x0027164E (the map spells the class Rva002716Holder; Drawable.cpp carries the
// row): WorkerAIUpdate::aiDoCommand 0x004AB09D calls it on the worker's drawable with the module data's maximum box
// count and the carried count.
class Rva002716Holder
{
public:
	void Rva0027164EBroadcast(Int maxBoxes, Int boxes);	// 0x0027164E
};

#endif
