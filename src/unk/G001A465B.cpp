// cl: /O1
#include "WW3D2/BFME2MotionChannel.h"
#include "WWLib/chunkio.h"
#include <new>

// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G001A465B name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x001A465B size=17 name=??0BFME2MotionChannel@@QAE@XZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
BFME2MotionChannel::BFME2MotionChannel()
	: Type(-1), Pivot(-1)
{
}

// FUNCTION: BFME2 0x001A466C size=7 name=??1BFME2MotionChannel@@UAE@XZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
BFME2MotionChannel::~BFME2MotionChannel()
{
}

// FUNCTION: BFME2 0x001A46B4 size=264 name=?Load_BFME2MotionChannel@@YAPAVBFME2MotionChannel@@AAVChunkLoadClass@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit

BFME2MotionChannel *Load_BFME2MotionChannel(ChunkLoadClass &chunk)
{
	BFME2MotionChannelHeader header;
	if (chunk.Read(&header, sizeof(header)) != sizeof(header) || header.reserved != 0)
		return NULL;

	BFME2MotionChannel *channel = NULL;
	switch (header.encoding)
	{
	case 0:
		channel = new BFME2Encoding0MotionChannel;
		break;
	case 1:
		channel = new BFME2Encoding1MotionChannel;
		break;
	case 2:
		channel = new BFME2Encoding2MotionChannel;
		break;
	}
	if (channel != NULL)
	{
		channel->Type = header.type;
		channel->Pivot = header.pivot;
		channel->Count = header.count;
		channel->Components = header.components;
		if (!channel->Load(chunk))
		{
			channel->~BFME2MotionChannel();
			::operator delete(channel);
			return NULL;
		}
	}
	return channel;
}
