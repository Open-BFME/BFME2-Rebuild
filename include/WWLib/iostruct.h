// iostruct.h: the plain float records the W3D chunk files store.
// Target facts: ChunkSaveClass's three Write overloads (0x00615400, 0x00615410,
// 0x00615420) forward to Write(const void *, unsigned long) 0x00615390 with
// sizes 8, 0xC and 0x10, so the records are two, three and four floats with no
// padding. They are declared `struct` in retail: the overloads mangle them
// with U (ABUIOVector3Struct@@).
#pragma once
#ifndef BFME2_IOSTRUCT_H
#define BFME2_IOSTRUCT_H

struct IOVector2Struct
{
	float X;	// +0x0
	float Y;	// +0x4
};

struct IOVector3Struct
{
	float X;	// +0x0
	float Y;	// +0x4
	float Z;	// +0x8
};

struct IOVector4Struct
{
	float X;	// +0x0
	float Y;	// +0x4
	float Z;	// +0x8
	float W;	// +0xC
};

typedef char IOVector2StructSize[(sizeof(IOVector2Struct) == 0x8) ? 1 : -1];
typedef char IOVector3StructSize[(sizeof(IOVector3Struct) == 0xC) ? 1 : -1];
typedef char IOVector4StructSize[(sizeof(IOVector4Struct) == 0x10) ? 1 : -1];

#endif
