// XferLoad.h: the save-game reader (an Xfer over a File). Vtable VA 0x00C7AF18 (RVA 0x0087AF18, 39 slots); the
// constructor 0x0060C5FA (map placeholder Rva0060C5FA) takes three pointers and stores them at +4, +0xC and +8.
// Target facts (retail game.dat; f04, reading the table and the five rows below):
//  - slots 5/6/7/8 are BeginBlock 0x0060C629, EndBlock 0x0060C469, SkipBlock 0x0060C6F6 and SkipBadBlock 0x0060C4EA;
//    slot 38 XferData 0x0060C7AD; slot 12 0x0060C549 is its own operator==(Snapshot &) (map placeholder).
//  - +0x08 a callback the bad-block skip calls with (snapshot, +0x0C, +0x1C); +0x10 Bool: the block names are
//    length-prefixed strings in the stream (the string-table mode XferSave::Open's third argument writes);
//    +0x14 File *, read through its slot 3 and moved through its slot 5 (seek); +0x18 the open block count;
//    +0x1C the stream position BeginBlock stored (-1 after the block closes). SIZE 0x20.
//  - Stream tags: "KOLB" 0x424C4F4B opens a block and "KLBE" 0x45424C4B closes it; every failure throws XferException.
#pragma once
#ifndef BFME2_XFERLOAD_H
#define BFME2_XFERLOAD_H

#include "Common/GameType.h"
#include "Common/Xfer.h"

class File;
class Snapshot;

class XferLoad : public Xfer
{
public:
	virtual Int BeginBlock(const char *name);									// slot 5, 0x0060C629
	virtual void EndBlock();													// slot 6, 0x0060C469
	virtual void SkipBlock(const char *name);									// slot 7, 0x0060C6F6
	virtual void SkipBadBlock(Snapshot &snapshot, UnsignedInt size);			// slot 8, 0x0060C4EA
	virtual Xfer &operator==(Snapshot &snapshot);								// slot 12, 0x0060C549
	void rva0060C59D();												// 0x0060C59D (map name): skips the length-prefixed block name (length byte, 0xFF means four bytes); BeginBlock, SkipBlock and XferData call it

protected:
	virtual void XferData(UnsignedInt type, void *data, UnsignedInt size);		// slot 38, 0x0060C7AD

private:
	void (*m_snapshotLoadedCallback)(Snapshot &snapshot, void *userData, Int position);	// +0x04 (constructor's first argument): called after each snapshot with +0x0C and the file position (0x0060C549)
	void (*m_skipBadBlockCallback)(Snapshot &snapshot, void *userData, Int blockPos);	// +0x08
	void *m_userData;															// +0x0C
	Bool m_stringTableMode;														// +0x10
	File *m_file;																// +0x14
	Int m_blockDepth;															// +0x18
	Int m_blockPos;																// +0x1C
};
// SIZE 0x20

#endif
