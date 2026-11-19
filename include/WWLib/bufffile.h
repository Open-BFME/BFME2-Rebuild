// bufffile.h: WWLib's read-ahead file (WWVegas/WWLib/bufffile.h).
// SIZE 0x2C: RawFileClass (0x1C) then four words. Table 0x0087C3E0 overrides
// RawFileClass's slots 0 (deleting destructor 0x0061B050 area), Read 0x0061AE30,
// Write 0x0061AF20, Close 0x0061AFD0 and Seek 0x0061B000.
// Target facts: both constructors zero Buffer (+0x1C), BufferSize (+0x20),
// BufferAvailable (+0x24) and BufferOffset (+0x28); Reset_Buffer 0x0061AF30
// releases the buffer only when one exists and zeroes the four words; the
// destructor 0x0061AF60 calls Reset_Buffer; Close calls RawFileClass::Close and
// then Reset_Buffer. The default buffer size is a class static of 16 KiB.
// Donor facts: names follow Zero Hour's WWLib/bufffile.h (GPL), naming only.
#pragma once
#ifndef BFME2_BUFFFILE_H
#define BFME2_BUFFFILE_H

#include "WWLib/rawfile.h"

class BufferedFileClass : public RawFileClass
{
public:
	BufferedFileClass();							// 0x0061ADE0
	BufferedFileClass(const char *filename);				// 0x0061AE00
	virtual ~BufferedFileClass();						// 0x0061AF60

	virtual int Read(void *buffer, int size);				// 0x0061AE30
	virtual int Write(const void *buffer, int size);			// 0x0061AF20
	virtual int Seek(int pos, int dir = SEEK_CUR);				// 0x0061B000
	virtual void Close();							// 0x0061AFD0

protected:
	void Reset_Buffer();							// 0x0061AF30

public:
	unsigned char *Buffer;			// +0x1C
	int BufferSize;			// +0x20
	int BufferAvailable;			// +0x24
	int BufferOffset;			// +0x28

	static int _DesiredBufferSize;
};
// SIZE 0x2C

#endif
