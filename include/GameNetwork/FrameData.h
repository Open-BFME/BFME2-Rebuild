// FrameData.h: the commands of one network frame (Zero Hour's FrameData by role; offsets are retail's).
// Target facts (code RVAs):
//  - SIZE 0x14: the constructor 0x005DA4C8 writes the frame command count +0 = -1 and zeroes the command count +4, the
//    command list pointer +8 and the two "last failed" counters +0xC/+0x10 (init 0x005DA4F9 sets those two to -2).
//    FrameDataManager keeps FRAME_DATA_LENGTH of them in an array (stride 0x14).
//  - init 0x005DA4F9 allocates the NetCommandList (0x10 bytes, constructor 0x0058B06A) when there is none and resets it,
//    then restores the counters. allCommandsReady 0x005DA563 compares +0 with +4: equal -> READY (2) and both "last failed"
//    counters back to -2; with debugging on it records the pair in +0x10 (frame count) / +0xC (count) when it changed; more commands than expected ->
//    walks the list to its end, re-inits the frame and answers RESEND (1); otherwise NOTREADY (0).
//    addCommand 0x005DA5AE adds a message unless the list already holds it and counts it. getAckCommandCount 0x005DA621
//    counts the commands whose type needs an acknowledgement (CommandRequiresAck 0x00581229).
//  - The one-instruction accessors: setFrameCommandCount 0x0009070B, getFrameCommandCount 0x00421795 (folded), the folded
//    getters of +4 (0x005801F2) and +8 (0x0030F45F), all called out of line.
#pragma once
#ifndef BFME2_FRAMEDATA_H
#define BFME2_FRAMEDATA_H

#include "Common/GameType.h"

class NetCommandList;
class NetCommandRef;
class NetCommandMsg;

enum FrameDataReturnType
{
	FRAMEDATA_NOTREADY = 0,
	FRAMEDATA_RESEND = 1,
	FRAMEDATA_READY = 2
};

class FrameData
{
public:
	FrameData();											// 0x005DA4C8
	~FrameData();											// 0x005DA4DC

	void init();											// 0x005DA4F9
	void reset();											// 0x005DA55E: a five-byte `jmp` to init
	FrameDataReturnType allCommandsReady(Bool debugSpewage);	// 0x005DA563
	NetCommandRef *addCommand(NetCommandMsg *msg);			// 0x005DA5AE
	void zeroFrame();										// 0x005DA5E3
	void destroyGameMessages();								// 0x005DA5EB
	Int countCommandsOfType(Int type) const;				// 0x005DA600 (walks the list at +8 counting messages whose command type equals the argument; the map holds it as BfmeThingYN::bfmeCountYN)
	UnsignedInt getAckCommandCount();						// 0x005DA621
	UnsignedInt getCommandCount();							// folded getter of +4
	void setFrameCommandCount(UnsignedInt count);			// 0x0009070B
	UnsignedInt getFrameCommandCount();						// 0x00421795 (folded getter of +0)
	NetCommandList *getCommandList();						// folded getter of +8

	Int m_frameCommandCount;								// +0x00
	Int m_commandCount;										// +0x04
	NetCommandList *m_commandList;							// +0x08
	Int m_lastFailedCC;										// +0x0C
	Int m_lastFailedFrameCC;								// +0x10
};
// SIZE 0x14
typedef char FrameDataSize[(sizeof(FrameData) == 0x14) ? 1 : -1];

#endif
