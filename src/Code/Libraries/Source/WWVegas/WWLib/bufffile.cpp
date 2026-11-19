// cl: /O2
#include <stdio.h>
#include <string.h>
#include "WWLib/bufffile.h"
// Tier B; object=O0061ADE0 evidence=Z flags=align=A
// Extent is a hypothesis; only Object VERIFIED establishes recovery.
// Edges: left=unknown-rows right=unknown-rows
// FUNCTION: BFME2 0x0061ADE0 size=32 name=??0BufferedFileClass@@QAE@XZ evidence=N:ledger

int BufferedFileClass::_DesiredBufferSize = 1024 * 16;

BufferedFileClass::BufferedFileClass() :
	RawFileClass(),
	Buffer(0),
	BufferSize(0),
	BufferAvailable(0),
	BufferOffset(0)
{
}

// FUNCTION: BFME2 0x0061AE00 size=39 name=??0BufferedFileClass@@QAE@PBD@Z evidence=N:ledger

BufferedFileClass::BufferedFileClass(const char *filename) :
	RawFileClass(filename),
	Buffer(0),
	BufferSize(0),
	BufferAvailable(0),
	BufferOffset(0)
{
}

// FUNCTION: BFME2 0x0061AE30 size=239 name=?Read@BufferedFileClass@@UAEHPAXH@Z evidence=N:ledger

int BufferedFileClass::Read(void *buffer, int size)
{
	int delivered = 0;

	if (BufferAvailable > 0)
	{
		int chunk = size < BufferAvailable ? size : BufferAvailable;
		::memcpy(buffer, &Buffer[BufferOffset], chunk);
		BufferAvailable -= chunk;
		BufferOffset += chunk;
		size -= chunk;
		buffer = (char *)buffer + chunk;
		delivered += chunk;
	}

	if (size == 0)
		return delivered;

	int wanted = _DesiredBufferSize;

	int chunk = BufferSize;
	if (chunk == 0)
		chunk = wanted;
	if (size > chunk)
		return RawFileClass::Read(buffer, size) + delivered;

	if (BufferSize == 0)
	{
		BufferSize = wanted;
		Buffer = (unsigned char *)::operator new(BufferSize);
		BufferAvailable = 0;
		BufferOffset = 0;
	}

	if (BufferAvailable == 0)
	{
		BufferAvailable = RawFileClass::Read(Buffer, BufferSize);
		BufferOffset = 0;
	}

	if (BufferAvailable > 0)
	{
		int chunk = size < BufferAvailable ? size : BufferAvailable;
		::memcpy(buffer, &Buffer[BufferOffset], chunk);
		BufferAvailable -= chunk;
		BufferOffset += chunk;
		delivered += chunk;
	}

	return delivered;
}

int BufferedFileClass::Write(const void *buffer, int size)
{
	return RawFileClass::Write(buffer, size);
}

// FUNCTION: BFME2 0x0061AF30 size=37 name=?Reset_Buffer@BufferedFileClass@@IAEXXZ evidence=N:ledger

void BufferedFileClass::Reset_Buffer()
{
	if (Buffer != 0)
	{
		delete Buffer;
		Buffer = 0;
		BufferSize = 0;
		BufferAvailable = 0;
		BufferOffset = 0;
	}
}

// FUNCTION: BFME2 0x0061AF60 size=102 name=??1BufferedFileClass@@UAE@XZ evidence=N:ledger

BufferedFileClass::~BufferedFileClass()
{
	Reset_Buffer();
}

// FUNCTION: BFME2 0x0061AFD0 size=42 name=?Close@BufferedFileClass@@UAEXXZ evidence=N:ledger

void BufferedFileClass::Close()
{
	RawFileClass::Close();

	Reset_Buffer();
}

// FUNCTION: BFME2 0x0061B000 size=113 name=?Seek@BufferedFileClass@@UAEHHH@Z evidence=N:ledger

int BufferedFileClass::Seek(int pos, int dir)
{
	if ((dir != SEEK_CUR) || (pos < 0))
		Reset_Buffer();

	if (BufferAvailable == 0)
		return RawFileClass::Seek(pos, dir);

	int chunk = pos < BufferAvailable ? pos : BufferAvailable;
	pos -= chunk;
	BufferAvailable -= chunk;
	BufferOffset += chunk;

	return RawFileClass::Seek(pos, dir) - BufferAvailable;
}
