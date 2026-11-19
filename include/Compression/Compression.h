// Compression.h: the compression front end (Zero Hour's Compression/Compression.h, GPL-3.0-or-later).
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc.
// Target facts: getCompressionType 0x0068A1B0 returns 2 for "NOX\0", 3..11 for "ZL1\0".."ZL9\0", 12 for "EAB\0",
// 13 for "EAH\0" and 1 for "EAR\0" (the string literals sit at 0x00CE4AD8..); every caller treats 1..13 as compressed
// (getUncompressedSize 0x0068A320 reads the size word at +4 for those).
#pragma once
#ifndef BFME2_COMPRESSION_H
#define BFME2_COMPRESSION_H

#include "Common/GameType.h"

enum CompressionType
{
	COMPRESSION_NONE = 0,
	COMPRESSION_REFPACK,
	COMPRESSION_MAX = COMPRESSION_REFPACK,
	COMPRESSION_NOXLZH,
	COMPRESSION_ZLIB1,
	COMPRESSION_ZLIB2,
	COMPRESSION_ZLIB3,
	COMPRESSION_ZLIB4,
	COMPRESSION_ZLIB5,
	COMPRESSION_ZLIB6,
	COMPRESSION_ZLIB7,
	COMPRESSION_ZLIB8,
	COMPRESSION_ZLIB9,
	COMPRESSION_BTREE,
	COMPRESSION_HUFF,
};

class CompressionManager
{
public:
	static Bool isDataCompressed(const void *mem, Int len);							// 0x0068A5D0
	static CompressionType getCompressionType(const void *mem, Int len);				// 0x0068A1B0

	static Int getMaxCompressedSize(Int uncompressedLen, CompressionType compType);	// 0x0068A2A0
	static Int getUncompressedSize(const void *mem, Int len);							// 0x0068A320

	static Int compressData(CompressionType compType, void *src, Int srcLen, void *dest, Int destLen);	// 0x0068A360, 0 on error
	static Int decompressData(const void *src, Int srcLen, void *dest, Int destLen);			// 0x0068A4E0, 0 on error

	static const char *getCompressionNameByType(CompressionType compType);			// 0x0068A190
	static const char *getDecompressionNameByType(CompressionType compType);			// 0x0068A1A0
};

#endif
