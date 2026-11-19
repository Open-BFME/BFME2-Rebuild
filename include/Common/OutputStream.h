// OutputStream: the write interface the map-file chunk writer sends its bytes to.
// Target facts: the stream vtable's slot 0 is the write call used by the chunk table
// writer (retail 0x0030715D calls [vtable+0] with a byte pointer and a count).
#pragma once
#ifndef BFME2_COMMON_OUTPUTSTREAM_H
#define BFME2_COMMON_OUTPUTSTREAM_H

#include "Common/GameType.h"

class OutputStream
{
public:
	virtual void write(const void *data, Int numBytes) = 0;	// slot 0
};

#endif
