// pipe.h: WWLib's push-side data filters (WWVegas/WWLib/pipe.h): the chain base and
// the two plain sinks.
// Target facts (retail game.dat): Pipe is vptr +0, ChainTo +4, ChainFrom +8 (SIZE 0xC),
// five slots: deleting destructor, Flush 0x0061B150, End 0x006158B0, Put_To 0x0061B0E0
// and Put. BufferPipe (Put 0x0061B4C0) holds a Buffer at +0xC and the write
// index at +0x18. FilePipe (table 0x00C7C270, destructor 0x0061B510, End 0x0061B580,
// Put 0x0061B5B0) holds the FileClass pointer at +0xC and an "opened by us" flag at +0x10.
// Donor facts: names follow Zero Hour's WWLib/pipe.h (GPL), naming only.
#pragma once
#ifndef BFME2_PIPE_H
#define BFME2_PIPE_H

#include "WWLib/buff.h"
#include "WWLib/wwfile.h"

class Pipe
{
public:
	Pipe() : ChainTo(0), ChainFrom(0) {}
	virtual ~Pipe();					// 0x0061B0A0

	virtual int Flush();					// 0x0061B150
	virtual int End() { return Flush(); }
	virtual void Put_To(Pipe *pipe);			// 0x0061B0E0
	void Put_To(Pipe &pipe) { Put_To(&pipe); }
	virtual int Put(const void *source, int slen);	// 0x0061B130

	Pipe *ChainTo;		// +0x04
	Pipe *ChainFrom;		// +0x08

private:
	Pipe(Pipe &rvalue);
	Pipe &operator=(const Pipe &pipe);
};
// SIZE 0x0C

class BufferPipe : public Pipe
{
public:
	BufferPipe(void *buffer, int length);		// 0x006159F0; the destructor 0x00615A50 is implicit
	virtual int Put(const void *source, int slen);	// 0x0061B4C0

private:
	Buffer BufferPtr;	// +0x0C
	int Index;			// +0x18
};
// SIZE 0x1C

class FilePipe : public Pipe
{
public:
	FilePipe(FileClass *file) : File(file), HasOpened(false) {}
	virtual ~FilePipe();					// 0x0061B510
	virtual int End();					// 0x0061B580
	virtual int Put(const void *source, int slen);	// 0x0061B5B0

private:
	bool Valid_File() { return File != 0; }
	FileClass *File;		// +0x0C
	bool HasOpened;		// +0x10
};
// SIZE 0x14

#endif
