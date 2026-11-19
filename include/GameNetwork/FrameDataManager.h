// FrameDataManager.h: a ring of FrameData, one per frame (Zero Hour's FrameDataManager by role; offsets are retail's).
// Target facts (code RVAs, VA 0x00C70A0C table):
//  - SIZE 0x10. The constructor 0x0058B567 stores the table, the argument byte +8, allocates FRAME_DATA_LENGTH
//    FrameData (0x14 bytes each, with the usual four-byte count in front) and clears +0xC and +9. The only slot is the
//    scalar deleting destructor 0x0058B86E; the destructor 0x0058B623 re-stores the table, inits every frame and frees the
//    array. +9 is the quitting flag set by setQuitFrame 0x0058B818 (which stores the frame at +0xC).
//  - Every frame accessor takes a frame number and indexes the ring with frame % FRAME_DATA_LENGTH.
#pragma once
#ifndef BFME2_FRAMEDATAMANAGER_H
#define BFME2_FRAMEDATAMANAGER_H

#include "Common/GameType.h"
#include "GameNetwork/FrameData.h"
#include "GameNetwork/NetworkDefs.h"

class FrameDataManager
{
public:
	FrameDataManager(Bool isLocal);							// 0x0058B567
	virtual ~FrameDataManager();							// slot 0, 0x0058B623 (deleting 0x0058B86E)

	void reset();											// 0x0058B666
	NetCommandRef *addNetCommandMsg(NetCommandMsg *msg);	// 0x0058B6AB
	FrameDataReturnType allCommandsReady(UnsignedInt frame, Bool debugSpewage);	// 0x0058B6D1
	NetCommandList *getFrameCommandList(UnsignedInt frame);	// 0x0058B6F5
	void resetFrame(UnsignedInt frame, Bool isAdvancing);	// 0x0058B715
	UnsignedInt getCommandCount(UnsignedInt frame);			// 0x0058B74B
	void setFrameCommandCount(UnsignedInt frame, UnsignedInt count);	// 0x0058B76B
	UnsignedInt getFrameCommandCount(UnsignedInt frame);	// 0x0058B78F
	void zeroFrames(UnsignedInt startingFrame, UnsignedInt numFrames);	// 0x0058B7AF
	void destroyGameMessages();								// 0x0058B7ED
	void setQuitFrame(UnsignedInt frame);					// 0x0058B818
	Bool getIsQuitting();									// 0x0058B826
	Int countCommandsOfType(UnsignedInt frame, Int type);	// 0x0058B82A (the frame's ring entry counts the messages of that command type; the map holds it as rva00670890 on this class)
	UnsignedInt getAckCommandCount(UnsignedInt frame);		// 0x0058B84E

	FrameData *m_frameData;									// +0x04
	Bool m_isLocal;											// +0x08
	Bool m_isQuitting;										// +0x09
	unsigned char m_pad0A[2];								// +0x0A
	UnsignedInt m_quitFrame;								// +0x0C
};
// SIZE 0x10
typedef char FrameDataManagerSize[(sizeof(FrameDataManager) == 0x10) ? 1 : -1];

#endif
