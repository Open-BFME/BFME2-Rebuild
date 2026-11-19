// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// hcanim.h: WW3D2's compressed (time-coded) hierarchical animation (WWVegas/WW3D2/hcanim.h).
// Target facts (retail game.dat), from the Has_* predicates and the two channel records' constructors:
//  - HCompressedAnimClass is a HAnimClass: its frame count is at +0x44 and the frame rate at +0x50
//    (Get_Total_Time 0x0018F190 is frames / rate), the per-pivot channel records live in two arrays whose
//    pointers are at +0x54 (NodeCompressedMotionStruct, stride 0x1C) and +0x58 (BFME2CompressedMotionChannels,
//    stride 0x18); every Has_* predicate reads the second array when it exists and the first otherwise,
//    asking whether the channel pointer is non-null.
//  - NodeCompressedMotionStruct (SIZE 0x1C): the pivot index +0 (the constructor 0x0018F680 leaves it alone) and
//    six channel pointers +4 X, +8 Y, +0xC Z, +0x10 rotation, +0x14 the fade channel, +0x18 visibility.
//    BFME2CompressedMotionChannels (SIZE 0x18) has the same six pointers from +0.
//  The channel pointers' types (TimeCoded, AdaptiveDelta, bit channels) are not recovered per slot yet.
#pragma once
#ifndef BFME2_HCANIM_H
#define BFME2_HCANIM_H

#include "WW3D2/hanim.h"

class TimeCodedBitChannelClass;
class TimeCodedMotionChannelClass;
class AdaptiveDeltaMotionChannelClass;

struct NodeCompressedMotionStruct
{
	// The flavour selects which channel class the five pointers hold (SetFlavor 0x0066C940 stores it, the destructor
	// 0x0018F6A0 and Get_Channel_Memory_Usage 0x0018F7B0 switch on it: 0 time-coded, 1 adaptive delta).
	enum { ANIM_FLAVOR_TIMECODED = 0, ANIM_FLAVOR_ADAPTIVE_DELTA = 1 };

	NodeCompressedMotionStruct();			// 0x0018F680
	~NodeCompressedMotionStruct();			// 0x0018F6A0
	int Get_Channel_Memory_Usage();			// 0x0018F7B0

	int Flavor;					// +0x00
	union
	{
		struct
		{
			TimeCodedMotionChannelClass *X;		// +0x04
			TimeCodedMotionChannelClass *Y;		// +0x08
			TimeCodedMotionChannelClass *Z;		// +0x0C
			TimeCodedMotionChannelClass *Q;		// +0x10
			TimeCodedMotionChannelClass *Fade;	// +0x14
		} tc;
		struct
		{
			AdaptiveDeltaMotionChannelClass *X;
			AdaptiveDeltaMotionChannelClass *Y;
			AdaptiveDeltaMotionChannelClass *Z;
			AdaptiveDeltaMotionChannelClass *Q;
			AdaptiveDeltaMotionChannelClass *Fade;
		} ad;
	};
	TimeCodedBitChannelClass *Visibility;		// +0x18
};
// SIZE 0x1C

struct BFME2CompressedMotionChannels
{
	BFME2CompressedMotionChannels();		// 0x0018F850
	~BFME2CompressedMotionChannels();		// 0x0018F870

	// X, Y, Z, rotation and fade: Is_Node_Motion_Present walks the first five as an array.
	void *Motion[5];				// +0x00
	TimeCodedBitChannelClass *Visibility;		// +0x14
};
// SIZE 0x18

class TimeCodedBitChannelClass;
class TimeCodedMotionChannelClass;
class AdaptiveDeltaMotionChannelClass;
class ChunkLoadClass;

class HCompressedAnimClass : public HAnimClass
{
public:
	HCompressedAnimClass();				// 0x0018F0E0
	virtual ~HCompressedAnimClass();		// 0x00190F10 (slot 1)
	virtual int Get_Name_Key() const { return NameKey; }		// 0x00661FD0 (slot 3)
	virtual const char *Get_HName() const { return HTreeName; }	// 0x0066D7D0 (slot 4)
	virtual int Get_Num_Frames() { return NumFrames; }		// 0x0066D730 (slot 5)
	virtual const char *Get_Name() const { return Name; }		// 0x0066E430 (slot 2; the shared `lea eax,[ecx+0x10]` stub)
	virtual int Get_Num_Pivots() const { return NumNodes; }		// 0x00143290 (slot 13)
	virtual int Get_Class_ID() const { return 1; }			// 0x00756B00 (slot 21)
	virtual int Get_Memory_Usage();					// 0x0018FB70 (slot 22)
	virtual float Get_VisibilityF(int pivot, float frame);		// 0x001903C0 (slot 12)
	virtual float Get_Frame_Rate() const;		// 0x0018F180 (slot 6)
	virtual float Get_Total_Time();			// 0x0018F190 (slot 7)
	virtual bool Has_X_Translation(int pivot);	// 0x0018F500
	virtual bool Has_Y_Translation(int pivot);	// 0x0018F540
	virtual bool Has_Z_Translation(int pivot);	// 0x0018F580
	virtual bool Has_Rotation(int pivot);		// 0x0018F5C0
	virtual bool Has_Visibility(int pivot);		// 0x0018F600
	virtual bool Has_VisibilityF(int pivot);		// 0x0018F640
	virtual bool Is_Node_Motion_Present(int pivot);	// 0x0018F490 (slot 14)
	virtual bool Get_Visibility(int pivot, float frame);	// 0x0018F440 (slot 11)
	virtual void Get_Translation(Vector3 &translation, int pivot, float frame) const;	// 0x00190610 (slot 8)
	virtual bool Get_Orientation(Quaternion &orientation, int pivot, float frame) const;	// 0x00190C60 (slot 9)
	virtual void Get_Transform(Matrix3D &transform, int pivot, float frame) const;	// 0x0018F3F0 (slot 10)

private:
	void add_bit_channel(TimeCodedBitChannelClass *channel);	// 0x0018F3C0
	bool read_channel(ChunkLoadClass &cload, TimeCodedMotionChannelClass **newchan);		// 0x0018F1C0
	bool read_channel(ChunkLoadClass &cload, AdaptiveDeltaMotionChannelClass **newchan);	// 0x0018F230
	bool read_bit_channel(ChunkLoadClass &cload, TimeCodedBitChannelClass **newchan);	// 0x0018F350
	void Free(void);										// 0x0018FE30

	char Name[16];					// +0x10 (the constructor clears it and takes the name key of it)
	unsigned char Unrecovered20[0x10];		// +0x20
	char HTreeName[16];				// +0x30 (cleared by the constructor)
	int NameKey;					// +0x40 (TheNameKeyGenerator->nameToKey(Name))
	int NumFrames;					// +0x44
	int NumNodes;					// +0x48
	int Flavor;					// +0x4C (the constructor stores 2)
	float FrameRate;				// +0x50
	NodeCompressedMotionStruct *NodeMotion;		// +0x54
	BFME2CompressedMotionChannels *Channels;	// +0x58
};

#endif
