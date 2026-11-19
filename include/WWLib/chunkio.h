// chunkio.h: WWLib's chunked file writer and reader (the W3D container format).
//
// Target facts:
// - ChunkSaveClass(FileClass *) 0x00615280 stores the file at +0, zeroes the
//   depth at +4, runs the 256 eight-byte header constructors at +0x408, clears
//   the micro-chunk flag byte +0xC08 and position +0xC0C, then zero-fills the
//   256 start positions at +0x8 and the header array, and finally the two-byte
//   micro-chunk header at +0xC10.
// - ChunkLoadClass(BFMEChunkInput *) 0x00614EF0 is the same shape shifted by
//   one word: +0 is a FileClass pointer (zeroed), +4 the alternative input
//   source, +8 the depth, +0xC the start positions, +0x40C the headers, +0xC0C
//   the micro flag, +0xC10 the micro position, +0xC14 the micro header.
// - The getters address the innermost open header as Headers[Depth - 1]:
//   Cur_Chunk_ID 0x00615020 reads +0x404 + Depth*8; Cur_Chunk_Length
//   0x00615030 masks the size word with 0x7FFFFFFF and Contains_Chunks
//   0x00615040 with 0x80000000 (the sub-chunk flag lives in the size's top bit).
// - Micro-chunk headers are a type byte and a size byte: Cur_Micro_Chunk_ID
//   0x006150A0 / _Length 0x006150B0 zero-extend +0xC14 / +0xC15, and
//   MicroChunkHeader::Add_Size 0x00615270 is `add [ecx+1], al`.
// - The file is driven through FileClass's slots: Seek (+0x28), Write (+0x34),
//   Read (+0x24).
#pragma once
#ifndef BFME2_CHUNKIO_H
#define BFME2_CHUNKIO_H

#include "WWLib/iostruct.h"

class FileClass;

// The alternative read source of ChunkLoadClass (used when its FileClass pointer is null). Target facts from the
// loader's calls: slot 3 (+0xC) is Read(buffer, size) and slot 5 (+0x14) is Seek(offset, origin); a Seek(0, 1)
// reports the current position. The other slots are not reached by any row here.
class BFMEChunkInput
{
public:
	virtual ~BFMEChunkInput();							// slot 0
	virtual int vfunc_0x04();							// slot 1
	virtual int vfunc_0x08();							// slot 2
	virtual unsigned long Read(void *buffer, unsigned long size);			// slot 3
	virtual int vfunc_0x10();							// slot 4
	virtual long Seek(long offset, int origin);					// slot 5
};

struct ChunkHeader
{
	ChunkHeader(void) : ChunkType(0), ChunkSize(0) {}

	void Set_Type(unsigned long type) { ChunkType = type; }
	unsigned long Get_Type(void) { return ChunkType; }
	void Set_Size(unsigned long size)
	{
		ChunkSize &= 0x80000000;
		ChunkSize |= (size & 0x7FFFFFFF);
	}
	unsigned long Get_Size(void) { return ChunkSize & 0x7FFFFFFF; }
	void Set_Sub_Chunk_Flag(bool onoff)
	{
		if (onoff)
			ChunkSize |= 0x80000000;
		else
			ChunkSize &= 0x7FFFFFFF;
	}
	int Get_Sub_Chunk_Flag(void) { return ChunkSize & 0x80000000; }
	void Add_Size(unsigned long add) { Set_Size(Get_Size() + add); }		// 0x00615240

	unsigned long ChunkType;	// +0x0
	unsigned long ChunkSize;	// +0x4: low 31 bits the payload size, top bit "has sub-chunks"
};

struct MicroChunkHeader
{
	MicroChunkHeader(void) {}

	void Set_Type(unsigned char type) { ChunkType = type; }
	unsigned char Get_Type(void) { return ChunkType; }
	void Set_Size(unsigned char size) { ChunkSize = size; }
	unsigned char Get_Size(void) { return ChunkSize; }
	void Add_Size(unsigned char add) { ChunkSize += add; }		// 0x00615270

	unsigned char ChunkType;	// +0x0
	unsigned char ChunkSize;	// +0x1
};

typedef char ChunkHeaderSize[(sizeof(ChunkHeader) == 0x8) ? 1 : -1];
typedef char MicroChunkHeaderSize[(sizeof(MicroChunkHeader) == 0x2) ? 1 : -1];

class ChunkSaveClass
{
public:
	ChunkSaveClass(FileClass *file);			// 0x00615280

	bool Begin_Chunk(unsigned long id);			// 0x00614DB0
	bool End_Chunk(void);					// 0x006152E0
	bool Begin_Micro_Chunk(unsigned long id);		// 0x00615460
	bool End_Micro_Chunk(void);				// 0x00614E30

	unsigned long Write(const void *buf, unsigned long nbytes);	// 0x00615390
	unsigned long Write(const IOVector2Struct &v);		// 0x00615400
	unsigned long Write(const IOVector3Struct &v);		// 0x00615410
	unsigned long Write(const IOVector4Struct &v);		// 0x00615420

private:
	enum { MAX_DEPTH = 256 };

	FileClass *File;				// +0x000
	int Depth;					// +0x004
	unsigned long ChunkStart[MAX_DEPTH];		// +0x008: file offset of each open chunk's header
	ChunkHeader Headers[MAX_DEPTH];			// +0x408
	bool InMicroChunk;				// +0xC08
	int MicroChunkStart;				// +0xC0C
	MicroChunkHeader MicroHeader;			// +0xC10
};

typedef char ChunkSaveClassSize[(sizeof(ChunkSaveClass) == 0xC14) ? 1 : -1];

class ChunkLoadClass
{
public:
	ChunkLoadClass(BFMEChunkInput *input);		// 0x00614EF0

	bool Open_Chunk(void);					// 0x00614F50
	bool Close_Chunk(void);					// 0x00614FC0
	unsigned long Cur_Chunk_ID(void);			// 0x00615020
	unsigned long Cur_Chunk_Length(void);			// 0x00615030
	int Contains_Chunks(void);				// 0x00615040

	bool Open_Micro_Chunk(void);				// 0x00615430
	bool Close_Micro_Chunk(void);				// 0x00615050
	unsigned long Cur_Micro_Chunk_ID(void);			// 0x006150A0
	unsigned long Cur_Micro_Chunk_Length(void);		// 0x006150B0

	unsigned long Seek(unsigned long nbytes);		// 0x006150C0
	unsigned long Read(void *buf, unsigned long nbytes);	// 0x006151A0

private:
	enum { MAX_DEPTH = 256 };

	FileClass *File;				// +0x000
	BFMEChunkInput *Input;				// +0x004: read source used when File is null
	int Depth;					// +0x008
	unsigned long ChunkStart[MAX_DEPTH];		// +0x00C: bytes consumed in each open chunk
	ChunkHeader Headers[MAX_DEPTH];			// +0x40C
	bool InMicroChunk;				// +0xC0C
	int MicroChunkPos;				// +0xC10
	MicroChunkHeader MicroHeader;			// +0xC14
};

typedef char ChunkLoadClassSize[(sizeof(ChunkLoadClass) == 0xC18) ? 1 : -1];

#endif
