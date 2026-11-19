// buff.h: WWLib's sized memory block (WWVegas/WWLib/buff.h).
// SIZE 0x0C: Data +0, Size +4, IsAllocated +8. Target facts: BufferStraw (0x00615590)
// and BufferPipe (0x0061B4C0) read the pointer at their +0xC and the size at +0x10;
// the validity test is a non-null pointer. Constructors and destructor are not
// among the rows recovered so far and are only declared.
// The class is `Buffer` in this build (the rows are ??0Buffer@@QAE@PAXJ@Z and friends), so the pointer member is
// `Data`: a member cannot share the class's name.
#pragma once
#ifndef BFME2_BUFF_H
#define BFME2_BUFF_H

class Buffer
{
public:
	Buffer(void *buffer, long size);
	Buffer(long size = 0);
	Buffer(const void *buffer, long size);
	Buffer(const Buffer &rvalue);
	~Buffer();

	Buffer &operator=(const Buffer &buffer);
	void Reset();

	operator void *() const { return Data; }
	void *Get_Buffer() const { return Data; }
	long Get_Size() const { return Size; }
	bool Is_Valid() const { return Data != 0; }

protected:
	void *Data;			// +0x00
	long Size;			// +0x04
	bool IsAllocated;		// +0x08
};
// SIZE 0x0C

#endif
