// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// hrawanim.h: WW3D2's uncompressed hierarchical animation (WWVegas/WW3D2/hrawanim.h).
// Target facts (retail game.dat), from the Has_* predicates, Free 0x0018E890 and add_bit_channel 0x0018D520:
//  - HRawAnimClass is a HAnimClass: the frame count is at +0x44, the pivot count at +0x48, the frame rate at +0x4C
//    (Get_Frame_Rate 0x0018D340, Get_Total_Time 0x0018D350 is frames / rate) and the per-pivot motion records at +0x50.
//  - A record (NodeMotionStruct, SIZE 0x24; Free and the destructor release the array through the vector destructor
//    iterator with that stride) holds nine pointers: the X, Y, Z, XR, YR, ZR and Q motion channels, the BFME fade
//    channel at +0x1C and the visibility bit channel at +0x20.
#pragma once
#ifndef BFME2_HRAWANIM_H
#define BFME2_HRAWANIM_H

#include "WW3D2/hanim.h"
#include "WW3D2/motchan.h"

struct NodeMotionStruct
{
	NodeMotionStruct();
	~NodeMotionStruct();
	int Get_Memory_Usage();		// 0x0018D220

	MotionChannelClass *X;		// +0x00
	MotionChannelClass *Y;		// +0x04
	MotionChannelClass *Z;		// +0x08
	MotionChannelClass *XR;		// +0x0C
	MotionChannelClass *YR;		// +0x10
	MotionChannelClass *ZR;		// +0x14
	MotionChannelClass *Q;		// +0x18
	MotionChannelClass *Fade;	// +0x1C
	BitChannelClass *Vis;		// +0x20
};
// SIZE 0x24

class HRawAnimClass : public HAnimClass
{
public:
	HRawAnimClass();						// 0x0018D2A0
	virtual ~HRawAnimClass();					// 0x0018EBC0 (slot 1)
	virtual float Get_VisibilityF(int pivot, float frame);		// 0x0018D540 (slot 12)
	virtual void Get_Translation(Vector3 &translation, int pivot, float frame) const;	// 0x0018D730 (slot 8)
	virtual bool Get_Orientation(Quaternion &orientation, int pivot, float frame) const;	// 0x0018DD60 (slot 9)
	virtual void Get_Transform(Matrix3D &transform, int pivot, float frame) const;	// 0x0018DF20 (slot 10)
	virtual int Get_Name_Key() const { return NameKey; }		// 0x00661FD0 (slot 3)
	virtual const char *Get_HName() const { return HTreeName; }	// 0x0066D7D0 (slot 4)
	virtual int Get_Num_Frames() { return NumFrames; }		// 0x0066D730 (slot 5)
	virtual const char *Get_Name() const { return Name; }		// 0x0066E430 (slot 2; the shared `lea eax,[ecx+0x10]` stub)
	virtual int Get_Num_Pivots() const { return NumNodes; }		// 0x00143290 (slot 13)
	virtual int Get_Class_ID() const { return 0; }			// 0x0065CE90 (slot 21)
	virtual int Get_Memory_Usage();					// 0x0018E6F0 (slot 22)
	virtual float Get_Frame_Rate() const;				// 0x0018D340 (slot 6)
	virtual float Get_Total_Time();					// 0x0018D350 (slot 7)
	virtual bool Get_Visibility(int pivot, float frame);		// 0x0018E570 (slot 11)
	virtual bool Is_Node_Motion_Present(int pivot);			// 0x0018E5E0 (slot 14)
	virtual bool Has_X_Translation(int pivot);			// 0x0018E630
	virtual bool Has_Y_Translation(int pivot);			// 0x0018E650
	virtual bool Has_Z_Translation(int pivot);			// 0x0018E670
	virtual bool Has_Rotation(int pivot);				// 0x0018E690
	virtual bool Has_Visibility(int pivot);				// 0x0018E6D0
	virtual bool Has_VisibilityF(int pivot);				// 0x0018E6B0

private:
	bool read_channel(ChunkLoadClass &cload, MotionChannelClass **newchan, bool pre30);	// 0x0018D360
	void add_channel(MotionChannelClass *newchan);			// 0x0018D3E0
	bool read_bit_channel(ChunkLoadClass &cload, BitChannelClass **newchan, bool pre30);	// 0x0018D4A0
	void add_bit_channel(BitChannelClass *newchan);			// 0x0018D520
	void Free(void);						// 0x0018E890

	char Name[16];				// +0x10 (the constructor clears it and takes the name key of it)
	unsigned char Unrecovered20[0x10];	// +0x20
	char HTreeName[16];			// +0x30 (cleared by the constructor)
	int NameKey;				// +0x40 (TheNameKeyGenerator->nameToKey(Name))
	int NumFrames;				// +0x44
	int NumNodes;				// +0x48
	float FrameRate;			// +0x4C
	NodeMotionStruct *NodeMotion;		// +0x50
};

#endif
