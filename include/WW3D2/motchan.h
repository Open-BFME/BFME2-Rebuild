// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// motchan.h: WW3D2's keyframe channels (WWVegas/WW3D2/motchan.h): plain-value, bit, time-coded and
// adaptive-delta motion channels that HRawAnimClass and HCompressedAnimClass play back.
// Target facts (retail game.dat), from the constructors, Free and memory-usage rows in motchan.cpp:
//  - MotionChannelClass (SIZE 0x20): PivotIdx +0, Type +4, VectorLen +8 (all cleared), two floats +0xC/+0x10
//    (0.0), the sample array +0x14 (freed with the array delete by Free 0x001960C0 and cleared), and the first
//    and last frame +0x18/+0x1C (-1; Load_W3D 0x00195AE0 stores the chunk's frame range there).
//  - BitChannelClass (SIZE 0x18): PivotIdx +0, Type +4, default value +8, first and last frame +0xC/+0x10
//    (-1), the bit array +0x14.
//  - TimeCodedMotionChannelClass (SIZE 0x1C): PivotIdx +0, Type +4, VectorLen +8, PacketSize +0xC, the number
//    of time codes +0x10, the last time-code index +0x14 (the search cache), the packet array +0x18 (Free
//    0x001960E0). Get_Channel_Memory_Usage is NumTimeCodes * PacketSize * 4 + 0x1C; set_identity zeroes the
//    first result word and, for a quaternion channel (Type 6), writes (0, 0, 0, 1.0) into the result.
//  - TimeCodedBitChannelClass (SIZE 0x18): six words all cleared by the constructor; its memory usage is the
//    time-code count (+0xC) times 4 plus 0x18. Each entry of the array at +0x14 packs a frame number in the low
//    31 bits and the bit's value in the top bit; Get_Bit searches forward from the cached index (+0x10).
#pragma once
#ifndef BFME2_MOTCHAN_H
#define BFME2_MOTCHAN_H

#include "WWMath/Quaternion.h"

class ChunkLoadClass;

class MotionChannelClass
{
public:
	MotionChannelClass();						// 0x00195AB0
	bool Load_W3D(ChunkLoadClass &cload);				// 0x00195AE0
	int Get_Channel_Memory_Usage();					// 0x00195BC0
	int Get_Pivot() const { return PivotIdx; }
	void Set_Pivot(int pivot) { PivotIdx = pivot; }
	int Get_Type() const { return Type; }
	// Zero Hour's WWINLINE Get_Vector and set_identity (motchan.h); evidence: HRawAnimClass's Get_VisibilityF 0x0018D540,
	// Get_Translation 0x0018D730, Get_Orientation 0x0018DD60 and Get_Transform 0x0018DF20 expand them in place.
	__forceinline void Get_Vector(int frame, float *setvec) const
	{
		if ((frame < FirstFrame) || (frame > LastFrame)) {
			set_identity(setvec);
		} else {
			int vframe = frame - FirstFrame;
			for (int i = 0; i < VectorLen; i++) {
				setvec[i] = Data[vframe * VectorLen + i];
			}
		}
	}

	// Zero Hour's SPECIAL_GETVEC_AS_QUAT form of Get_Vector for a quaternion channel (HRawAnimClass::Get_Orientation 0x0018DD60).
	__forceinline void Get_Vector_As_Quat(int frame, Quaternion &quat) const
	{
		if ((frame < FirstFrame) || (frame > LastFrame)) {
			quat.Set(0.0f, 0.0f, 0.0f, 1.0f);
		} else {
			const float *d = &Data[(frame - FirstFrame) * VectorLen];
			quat.Set(d[0], d[1], d[2], d[3]);
		}
	}

private:
	__forceinline void set_identity(float *setvec) const
	{
		// BFME's version of Zero Hour's set_identity: retail's expanded copies test the fade type (15) first, then the quaternion
		// type (6) (0x0018D540 stores 1.0 for type 15; 0x0018DD60 writes (0, 0, 0, 1) for type 6).
		if (Type == 15) {
			setvec[0] = 1.0f;
		} else {
			setvec[0] = 0.0f;
			if (Type == 6) {
				setvec[1] = 0.0f;
				setvec[2] = 0.0f;
				setvec[3] = 1.0f;
			}
		}
	}
	void Free();							// 0x001960C0

	int PivotIdx;							// +0x00
	int Type;							// +0x04
	int VectorLen;							// +0x08
	float CachedFrame;						// +0x0C: cleared with its neighbour, not read by the rows recovered so far
	float CachedValue;						// +0x10
	float *Data;							// +0x14
	int FirstFrame;							// +0x18: the chunk's first and last frame, -1 until loaded
	int LastFrame;							// +0x1C
};
// SIZE 0x20

class BitChannelClass
{
public:
	BitChannelClass();						// 0x00195BE0
	bool Load_W3D(ChunkLoadClass &cload);				// 0x00195C00
	int Get_Channel_Memory_Usage();					// 0x00195CC0
	int Get_Pivot() const { return PivotIdx; }
	void Set_Pivot(int pivot) { PivotIdx = pivot; }
	int Get_Type() const { return Type; }
	void Free() { delete[] Bits; Bits = 0; }
	int Get_Bit(int frame);

	// The out-of-line overload and the inline reader share this predicate.
	__forceinline int Get_Bit(int frame) const
	{
		if ((frame < FirstFrame) || (frame > LastFrame)) {
			return DefaultValue;
		} else {
			int bit = frame - FirstFrame;
			unsigned char mask = (unsigned char)(1 << (bit & 7));
			return ((*(Bits + (bit >> 3)) & mask) != 0);
		}
	}

private:
	int PivotIdx;							// +0x00
	int Type;							// +0x04
	int DefaultValue;						// +0x08
	int FirstFrame;							// +0x0C
	int LastFrame;							// +0x10
	unsigned char *Bits;						// +0x14
};
// SIZE 0x18

class TimeCodedMotionChannelClass
{
public:
	TimeCodedMotionChannelClass();					// 0x00195CE0
	bool Load_W3D(ChunkLoadClass &cload);				// 0x00195D00
	int Get_Channel_Memory_Usage();					// 0x00195DC0

private:
	void set_identity(float *setvec);				// 0x00195DD0
	void Free();							// 0x001960E0

	int PivotIdx;							// +0x00
	int Type;							// +0x04
	int VectorLen;							// +0x08
	int PacketSize;							// +0x0C
	int NumTimeCodes;						// +0x10
	int LastTimeCodeIdx;						// +0x14
	unsigned long *Data;						// +0x18
};
// SIZE 0x1C

class TimeCodedBitChannelClass
{
	friend class HCompressedAnimClass;
public:
	TimeCodedBitChannelClass();					// 0x00195E00
	bool Load_W3D(ChunkLoadClass &cload);				// 0x00195E20
	void Free() { delete[] Bits; Bits = 0; }
	int Get_Channel_Memory_Usage();					// 0x00195EE0
	int Get_Bit(int frame);						// 0x00195EF0

private:
	int PivotIdx;							// +0x00
	int Type;							// +0x04
	int DefaultVal;							// +0x08
	int NumTimeCodes;						// +0x0C
	int CachedIdx;							// +0x10
	unsigned long *Bits;						// +0x14
};
// SIZE 0x18

// The adaptive-delta channel (SIZE 0x1C): its constructor 0x00195F60 clears +0 to +0x10 and +0x18 and stores
// 0.0 at +0x14, then fills a shared 240-entry table once (VA 0x00DB6818, flag VA 0x00DFCEC0: 1 - sin of
// 0.375-degree steps; retail spills the sine's argument and result through the stack, which an expression
// written with sin or sinf does not reproduce, so the constructor is not written). Get_Channel_Memory_Usage 0x001960B0 adds eight bytes
// per vector component (VectorLen, +8) and the packed delta byte count (+0x10) to the object's own size.
class AdaptiveDeltaMotionChannelClass
{
public:
	AdaptiveDeltaMotionChannelClass();				// 0x00195F60
	bool Load_W3D(ChunkLoadClass &cload);				// 0x00195FF0
	void Free() { delete[] Data; Data = 0; }
	int Get_Channel_Memory_Usage();					// 0x001960B0

private:
	int PivotIdx;							// +0x00
	int Type;							// +0x04
	int VectorLen;							// +0x08
	int NumFrames;							// +0x0C
	int DeltaBytes;							// +0x10
	float Scale;							// +0x14
	unsigned char *Data;						// +0x18
};
// SIZE 0x1C

#endif
