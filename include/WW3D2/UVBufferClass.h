// UVBufferClass.h: a shared array of texture coordinates with a content checksum (MeshMatDescClass +0x10..+0x2C).
//
// Target facts (retail game.dat): a ShareBufferClass<Vector2> (vftable 0x00BCE330) whose own vftable is 0x00BCE338, followed by
// the checksum at +0x18. Update_CRC 0x0015B460 checksums the array (count * 8 bytes) with CRC_Memory 0x006157D0 and a zero seed;
// operator== 0x0015A160 compares the checksums. MeshMatDescClass::Make_UV_Array_Unique 0x0015D510 expands the copy
// constructor (base copy 0x0015CD00, vftable, checksum copy).
// SIZE 0x1C.
#pragma once
#ifndef BFME2_UVBUFFERCLASS_H
#define BFME2_UVBUFFERCLASS_H

#include "WW3D2/ShareBufferClass.h"
#include "WWMath/Vector2.h"

class UVBufferClass : public ShareBufferClass<Vector2>
{
public:
	UVBufferClass(int count, const char *msg) : ShareBufferClass<Vector2>(count, msg, 0), CRC(0xFFFFFFFF) {}
	UVBufferClass(const UVBufferClass &that) : ShareBufferClass<Vector2>(that), CRC(that.CRC) {}

	bool operator==(const UVBufferClass &that);				// 0x0015A160 (out of line in retail)
	void Update_CRC(void);							// 0x0015B460
	unsigned int Get_CRC(void) { return CRC; }

private:
	unsigned int CRC;			// +0x18
};

#endif
