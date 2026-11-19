// ChunkInputStream: the read interface the map-file chunk reader takes its bytes from.
// Target facts: the four slots of the stream vtable are read (slot 0), tell (slot 1),
// absoluteSeek (slot 2) and eof (slot 3). The chunk reader calls them through the
// stream pointer it was given; it never looks inside the stream.
#pragma once
#ifndef BFME2_COMMON_CHUNKINPUTSTREAM_H
#define BFME2_COMMON_CHUNKINPUTSTREAM_H

#include "Common/GameType.h"

class ChunkInputStream
{
public:
	virtual Int read(void *data, Int numBytes) = 0;		// slot 0
	virtual UnsignedInt tell() = 0;						// slot 1
	virtual Bool absoluteSeek(UnsignedInt pos) = 0;		// slot 2
	virtual Bool eof() = 0;								// slot 3
};

#endif
