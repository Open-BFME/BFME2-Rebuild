// htree.h: WW3D2's hierarchy tree (a skeleton): the bone array with names and parents.
// Target facts (retail game.dat), from the small HTreeClass members in htree.cpp:
//  - HTreeClass is SIZE 0x34: Name +0 (a 16-byte buffer, cleared by the constructor 0x00166270),
//    NumPivots +0x10, Pivot array +0x14, ScaleFactor +0x18 (1.0), then the capture list +0x1C, an
//    STLport vector of 0x24-byte records (the copy constructor 0x001662A0 builds it with the vector's
//    copy constructor 0x00166110, Capture_Bone 0x00166980 inserts through vector::insert 0x001665B0 and
//    push_back's overflow path 0x001663F0, the destructor 0x001667E0 frees its storage), then +0x28 and
//    two more words (+0x2C a block released with the array delete in Free, +0x30) that the constructors
//    clear. Free 0x00166510 releases the pivots with the array delete, resets the scale and empties the
//    capture list.
//  - A pivot is a PivotClass (pivot.h, 0x58 bytes): its name starts the record (Get_Bone_Name returns the
//    record's address), the parent pointer is at +0x10 and the parent's own index at +0x4C (Get_Parent_Index
//    loads parent->Index, or 0 for a root); the base translation at +0x24 is what Scale multiplies.
//  - The capture list is kept sorted by bone index: Is_Bone_Captured walks to the first record whose
//    index is not below the one asked for and compares.
#pragma once
#ifndef BFME2_HTREE_H
#define BFME2_HTREE_H

#include <vector>
#include "WWMath/Quaternion.h"
#include "WW3D2/pivot.h"

class Matrix3D;
class ChunkLoadClass;
class HAnimClass;
class HRawAnimClass;
class HAnimComboClass;

struct HTreeCapture
{
	int BoneIndex;					// +0x00
	PivotTransform Transform;			// +0x04: the controlling matrix as a rotation and a translation
	bool WorldSpaceTranslation;			// +0x20
};
// SIZE 0x24

class HTreeClass
{
public:
	HTreeClass();							// 0x00166270
	HTreeClass(const HTreeClass &src);				// 0x001662A0
	~HTreeClass();							// 0x001667E0

	int Load_W3D(ChunkLoadClass &loader);				// 0x00166800
	void Init_Default(void);					// 0x001666D0
	int Get_Bone_Index(const char *name) const;			// 0x00160B60
	const char *Get_Bone_Name(int index) const;			// 0x00160BB0
	int Get_Parent_Index(int index) const;				// 0x00160BD0
	void Scale(float factor);					// 0x00160C00
	bool Is_Bone_Captured(int boneindex) const;			// 0x00162030
	int Get_First_Captured_Bone(void) const;			// 0x00162290 (role name: the first record of the sorted capture list, -1 when empty)
	void Control_Bone(int boneindex, const Matrix3D &relative_tm, bool world_space_translation);	// 0x00162060
	void Release_Bone(int boneindex);				// 0x001661F0
	void Capture_Bone(int boneindex);				// 0x00166980
	void Slave_Update(const HTreeClass *src, const Matrix3D &root);	// 0x00160CB0 (Animatable3DObjClass::Update_Sub_Object_Transforms follows another object's tree through it)

	void Free(void);						// 0x00166510

	const char *Get_Name(void) const { return Name; }					// HLodDefClass::Initialize 0x0019F620 copies it
	const PivotClass &Peek_Pivot(int index) const { return Pivot[index]; }	// HLodClass::Get_Proxy 0x0019C750 copies a pivot's current transform
	int Num_Pivots(void) const { return NumPivots; }
	bool Get_Visibility(int pivot) const { return Pivot[pivot].IsVisible; }	// HLodClass::Add_Sub_Object_To_Bone 0x0019F190 reads +0x50		// Animatable3DObjClass::Get_Num_Bones reads +0x10

	// The pose updates Animatable3DObjClass forwards to (0x001A47F0..0x001A48D0).
	bool Simple_Evaluate_Pivot(int pivot_index, const Matrix3D &obj_tm, Matrix3D *end_tm) const;	// 0x001606E0
	bool Simple_Evaluate_Pivot(HAnimClass *motion, int pivot_index, float frame, const Matrix3D &obj_tm, Matrix3D *end_tm) const;	// 0x00161700
	void Base_Update(const Matrix3D &root);				// 0x001628A0
	void Anim_Update(const Matrix3D &root, HAnimClass *motion, float frame);	// 0x00162BB0
	void Anim_Update(const Matrix3D &root, HRawAnimClass *motion, float frame);	// 0x00163A80
	void Blend_Update(const Matrix3D &root, HAnimClass *motion0, float frame0, HAnimClass *motion1, float frame1, float percentage);	// 0x001645B0
	void Combo_Update(const Matrix3D &root, HAnimComboClass *anim);	// 0x00165820

	int Capture_Update(int pivot_index);				// 0x001622B0 (applies the capture of one pivot, returns the next captured pivot or -1)
private:
	bool read_pivots(ChunkLoadClass &loader, bool pre30);		// 0x001614F0

	char Name[16];							// +0x00
	int NumPivots;							// +0x10
	PivotClass *Pivot;						// +0x14
	float ScaleFactor;						// +0x18
	_STL::vector<HTreeCapture> CaptureList;				// +0x1C
	int Unrecovered28;						// +0x28
	unsigned char *Unrecovered2C;					// +0x2C
	int Unrecovered30;						// +0x30
};
// SIZE 0x34

HTreeClass *Get_HTree(const char *name);			// 0x0017FC7C (the named tree of the hierarchy asset, null when unknown)

#endif
