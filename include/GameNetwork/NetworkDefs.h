// NetworkDefs.h: the two frame-ring sizes the network code reads (Zero Hour's NetworkDefs names them by role; the
// retail ledger names the data FRAME_DATA_LENGTH and FRAMES_TO_KEEP, both `int`).
// Target facts: every FrameDataManager row divides a frame number by the global at VA 0x00DD2DB8 (the ring length) and
// ConnectionManager::getFrameCommandList 0x004D0027 compares a frame against the global at VA 0x00DD2DBC before
// resetting the frames it no longer keeps.
#pragma once
#ifndef BFME2_NETWORKDEFS_H
#define BFME2_NETWORKDEFS_H

extern int FRAME_DATA_LENGTH;		// VA 0x00DD2DB8
extern int FRAMES_TO_KEEP;			// VA 0x00DD2DBC

#endif
