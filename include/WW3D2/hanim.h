// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// hanim.h: WW3D2's hierarchical animation base class and the animation blend classes
// (WWVegas/WW3D2/hanim.h, hanimcombo).
// Target facts (retail game.dat), from the hanim.cpp object's constructors, destructors and
// reference releases:
//  - HAnimClass is a RefCountClass (vptr +0, NumRefs +4) with 23 slots in its table: 0 Delete_This,
//    1 the destructor, 2 Get_Name (const), 3 to 6 four field getters the concrete classes answer
//    from their members, 7 Get_Total_Time, 8 Get_Translation, 9 Get_Orientation, 10 Get_Transform,
//    11 Get_Visibility, 12 Get_VisibilityF, 13 Get_Num_Pivots, 14 Is_Node_Motion_Present,
//    15..19 Has_X_Translation, Has_Y_Translation, Has_Z_Translation, Has_Rotation, Has_Visibility,
//    20 Has_VisibilityF, 21 a slot the base answers with 0 and 22 one more pure slot (names from
//    HRawAnimClass's table, include/WW3D2/hrawanim.h).
//  - PivotMapClass is a DynamicVectorClass<float> (0x18 bytes) followed by a RefCountClass at +0x18
//    (NumRefs +0x1C): SIZE 0x20, one added virtual As_Named_Pivot_Map (slot 6, null in the base).
//    A release of the map lands on the RefCountClass subobject (`lea ecx,[p+0x18]`).
//  - HAnimComboDataClass is SIZE 0x14: HAnim +0 (reference counted), Frame +4 (0), Weight +8 (1.0),
//    PivotMap +0xC (reference counted), and a flag byte +0x10 that is the constructor's argument;
//    HAnimComboClass's Clear and Reset leave entries with the flag set alone.
//  - HAnimComboClass is a DynamicVectorClass<HAnimComboDataClass *> and adds no members. Its
//    constructor 0x00197650 stores a single vftable (0x00BD5E88) in the middle of the vector
//    initialisation and no derived one afterwards, so the table is not a distinct derived table
//    that a user-declared override or destructor would produce; the map's HAnimComboClass::Clear
//    0x001965D0 (UAE would be an override) and constructor rows therefore stay unwritten.
#pragma once
#include <stdlib.h>
#ifndef BFME2_HANIM_H
#define BFME2_HANIM_H

#include "WWLib/mempool.h"

#include "WWLib/RefCountClass.h"
#include "WWLib/HashableClass.h"
#include "WWLib/vector.h"

class Vector3;
class Quaternion;
class Matrix3D;
class NamedPivotMapClass;

// A second base at +8: retail's HAnimClass objects are also hash-table entries (HAnimManagerClass::Peek_Anim 0x000F0B81
// turns the HashableClass pointer the table returns back with `lea ecx,[eax-8]`; the constructor of HRawAnimClass 0x0018D2A0
// stores the HashableClass vftable at +8 and a null chain link at +0xC).
class HAnimClass : public RefCountClass, public HashableClass
{
public:
	// Zero Hour's `virtual ~HAnimClass(void) { }`. Retail's derived destructors (HRawAnimClass 0x0018EBC0) call it out of line: 0x00196150 stores
	// the two secondary tables and the RefCountClass table and returns.
	virtual ~HAnimClass();
	// The HashableClass slot adjusts to the animation and forwards to Get_Name.
	virtual const char *Get_Key(void) { return Get_Name(); }
	virtual const char *Get_Name() const = 0;						// slot 2
	virtual int Get_Name_Key() const = 0;							// slot 3 (BFME's addition: HRawAnimClass's stub 0x00661FD0 reads the key at +0x40)
	virtual const char *Get_HName() const = 0;						// slot 4 (Zero Hour's order; 0x0066D7D0 returns the hierarchy name at +0x30)
	virtual int Get_Num_Frames() = 0;							// slot 5 (Zero Hour's order; 0x0066D730 reads the frame count at +0x44)
	virtual float Get_Frame_Rate() const = 0;						// slot 6 (n02: the slot-6 getter reads the frame rate member)
	virtual float Get_Total_Time() = 0;							// slot 7
	virtual void Get_Translation(Vector3 &translation, int pivot, float frame) const = 0;	// slot 8
	virtual bool Get_Orientation(Quaternion &orientation, int pivot, float frame) const = 0;	// slot 9
	virtual void Get_Transform(Matrix3D &transform, int pivot, float frame) const = 0;	// slot 10
	virtual bool Get_Visibility(int pivot, float frame) = 0;				// slot 11
	virtual float Get_VisibilityF(int pivot, float frame) = 0;				// slot 12
	virtual int Get_Num_Pivots() const = 0;							// slot 13
	virtual bool Is_Node_Motion_Present(int pivot) = 0;					// slot 14
	// Retail HAnimClass slots 15-20 share the return-true fold at 0x0018D1B0.
	virtual bool Has_X_Translation(int pivot) { return true; }				// slot 15: 0x0018D1B0
	virtual bool Has_Y_Translation(int pivot) { return true; }				// slot 16
	virtual bool Has_Z_Translation(int pivot) { return true; }				// slot 17
	virtual bool Has_Rotation(int pivot) { return true; }					// slot 18
	virtual bool Has_Visibility(int pivot) { return true; }				// slot 19
	virtual bool Has_VisibilityF(int pivot) { return true; }				// slot 20
	virtual int Get_Class_ID() const { return -1; }						// slot 21: the base answers CLASSID_UNKNOWN (0x0013B5E0); HRawAnimClass 0, HCompressedAnimClass 1
	virtual int Get_Memory_Usage() = 0;							// slot 22 (HRawAnimClass's 0x0018E6F0: the object plus every node's channels)
};

class PivotMapClass : public DynamicVectorClass<float>, public RefCountClass
{
public:
	virtual NamedPivotMapClass *As_Named_Pivot_Map() { return 0; }				// slot 6
};
// SIZE 0x20

// NamedPivotMapClass: a pivot map whose weights are set by bone name (Update_Pivot_Map 0x00196560 fills the float array
// from the named weights, Add 0x00196AA0 appends one; the weight list is a DynamicVectorClass at +0x20: vector +0x24,
// count +0x30). SIZE 0x38.
class HTreeClass;

class NamedPivotMapClass : public PivotMapClass
{
public:
	struct WeightInfoStruct
	{
		char *Name;
		float Weight;
		bool operator==(const WeightInfoStruct &) { return false; }
		bool operator!=(const WeightInfoStruct &) { return true; }
		~WeightInfoStruct() { if (Name) free(Name); }
	};

	void Add(const char *Name, float Weight);						// 0x00196AA0
	void Update_Pivot_Map(const HTreeClass *Tree);						// 0x00196560

private:
	DynamicVectorClass<WeightInfoStruct> WeightInfo;					// +0x20
};

class HAnimComboDataClass : public AutoPoolClass<HAnimComboDataClass, 256>
{
public:
	HAnimComboDataClass(bool shared = false);						// 0x001961B0
	HAnimComboDataClass(const HAnimComboDataClass &src);					// 0x001961E0
	~HAnimComboDataClass();									// 0x00196280
	void Build_Active_Pivot_Map();								// 0x001974D0

	HAnimClass *Get_HAnim() const;								// 0x00196120
	PivotMapClass *Get_Pivot_Map() const;							// 0x00196130
	void Copy(const HAnimComboDataClass *src);						// 0x00196220
	void Clear();										// 0x001962B0
	void Set_Pivot_Map(PivotMapClass *map);							// 0x00196310

	HAnimClass *HAnim;									// +0x00
	float Frame;										// +0x04
	float Weight;										// +0x08
	PivotMapClass *PivotMap;								// +0x0C
	bool Shared;										// +0x10
};
// SIZE 0x14

// Zero Hour's declaration: the combo owns a dynamic vector of data records (the vector sits at +0 and the class has no
// virtuals of its own, which is why its Clear is a plain member).
class HAnimComboClass
{
public:
	HAnimComboClass(void);									// 0x00197650
	HAnimComboClass(int num_animations);							// 0x00196B80
	void Clear(void);									// 0x001965D0
	void Reset(void);									// 0x00197680
	bool Normalize_Weights(void);								// 0x00196CA0
	int Get_Num_Anims(void) { return HAnimComboData.Count(); }
	void Set_Motion(int index, HAnimClass *motion);						// 0x00196640
	HAnimClass *Get_Motion(int index);							// 0x00196670
	HAnimClass *Peek_Motion(int index);							// 0x00196690
	void Set_Frame(int index, float frame);							// 0x001966A0
	float Get_Frame(int index);								// 0x001966C0
	void Set_Weight(int index, float weight);						// 0x001966D0
	float Get_Weight(int index);								// 0x001966F0
	void Set_Pivot_Weight_Map(int index, PivotMapClass *map);				// 0x00196700
	PivotMapClass *Get_Pivot_Weight_Map(int index);						// 0x00196740
	PivotMapClass *Peek_Pivot_Weight_Map(int index);					// 0x00196760
	void Append_Anim_Combo_Data(HAnimComboDataClass *data);					// 0x00197140
	void Remove_Anim_Combo_Data(HAnimComboDataClass *data);					// 0x00197190

protected:
	DynamicVectorClass<HAnimComboDataClass *> HAnimComboData;
};

#endif
