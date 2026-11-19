// straw.h: WWLib's pull-side data filters (WWVegas/WWLib/straw.h): the chain base
// and the three plain sources.
// Target facts (retail game.dat): Straw is vptr +0, ChainTo +4, ChainFrom +8 (SIZE 0xC)
// with three slots, deleting destructor, Get_From 0x00615510 and Get 0x00615560.
// BufferStraw (SIZE 0x1C, Get 0x00615590) holds a Buffer at +0xC and the read
// index at +0x18. FileStraw (SIZE 0x14, table 0x00C7C20C, Get 0x006155E0, destructor
// 0x00615640) holds the FileClass pointer at +0xC and a "we opened it" flag at +0x10.
// CacheStraw (Get 0x0061B430) holds a Buffer at +0xC, the read index at +0x18
// and the byte count at +0x1C.
// Donor facts: names follow Zero Hour's WWLib/straw.h (GPL), naming only.
#pragma once
#ifndef BFME2_STRAW_H
#define BFME2_STRAW_H

#include "WWLib/buff.h"
#include "WWLib/wwfile.h"

class Straw
{
public:
	Straw() : ChainTo(0), ChainFrom(0) {}
	virtual ~Straw();					// 0x006154D0

	virtual void Get_From(Straw *pipe);			// 0x00615510
	void Get_From(Straw &pipe) { Get_From(&pipe); }
	virtual int Get(void *buffer, int slen);		// 0x00615560

	Straw *ChainTo;		// +0x04
	Straw *ChainFrom;		// +0x08

private:
	Straw(const Straw &rvalue);
	Straw &operator=(const Straw &rvalue);
};
// SIZE 0x0C

class BufferStraw : public Straw
{
public:
	BufferStraw(const void *buffer, int length);		// 0x0013C880
	virtual ~BufferStraw();					// 0x0013C8E0
	virtual int Get(void *buffer, int slen);		// 0x00615590

private:
	Buffer BufferPtr;	// +0x0C
	int Index;			// +0x18
};
// SIZE 0x1C

class FileStraw : public Straw
{
public:
	FileStraw(FileClass &file) : File(&file), HasOpened(false) {}
	virtual ~FileStraw();					// 0x00615640
	virtual int Get(void *buffer, int slen);		// 0x006155E0

private:
	bool Valid_File() { return File != 0; }
	FileClass *File;		// +0x0C
	bool HasOpened;		// +0x10
};
// SIZE 0x14

class CacheStraw : public Straw
{
public:
	CacheStraw(int length = 4096);					// 0x00615940
	virtual int Get(void *buffer, int slen);		// 0x0061B430

private:
	bool Is_Valid();					// 0x0061B4B0
	Buffer BufferPtr;	// +0x0C
	int Index;			// +0x18
	int Length;			// +0x1C
};
// SIZE 0x20

#endif
