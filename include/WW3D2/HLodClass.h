// HLodClass.h: a hierarchical level-of-detail render object: per-level arrays of sub-objects attached
// to bones of the inherited hierarchy tree, plus models attached at every level.
//
// Target facts (retail game.dat):
// - The protected default constructor 0x0019F8C0 runs Animatable3DObjClass(0), installs vftable 0x007D6780
//   (146 slots: Animatable3DObjClass's 134 and twelve more) and the secondary 0x007D677C, then clears
//   LodCount +0x120, CurLod +0x124 and Lod +0x128, sets BoundingBoxIndex +0x12C to -1, clears the cost and
//   value arrays +0x130/+0x134, builds AdditionalModels +0x138 (a ModelArrayClass: the vector's
//   constructor 0x0019BD10, growth step 10, MaxScreenSize 0x007BB8E0's value, three zero floats) and clears
//   SnapPoints +0x160 and ProxyArray +0x164; LODBias +0x168 is 1.0. Clone 0x001A0500 allocates 0x16C bytes.
// - A level is a ModelArrayClass (0x28 bytes: the DynamicVectorClass of 0x14-byte ModelNodeClass records,
//   then four floats); Get_Max_Screen_Size 0x0019B6A0 reads +0x18 of the level, Get_Lod_Model_Count
//   0x0019C610 its count +0x10, Peek_Lod_Model 0x0019C640 node +0 and Get_Lod_Model_Bone 0x0019C6C0 node +4.
//   Add_Lod_Model 0x0019F2B0 adds a node {model, bone, zero offset}.
// - Free 0x0019C230 detaches (slot 19) and releases every model of every level and of AdditionalModels,
//   array-deletes the levels and the cost and value arrays, and releases SnapPoints (its reference count
//   lives at +0x18) and ProxyArray (at +0x10).
// - Get_Cost/Get_Value/Get_Post_Increment_Value 0x0019B810..0x0019B830 read Cost[CurLod], Value[CurLod]
//   and Value[CurLod + 1]; Class_ID 0x0019F020 returns 0x19.
// SIZE 0x16C.
#pragma once
#ifndef BFME2_HLODCLASS_H
#define BFME2_HLODCLASS_H

#include <float.h>
#include "WW3D2/Animatable3DObjClass.h"
#include "WWLib/vector.h"
#include "WWMath/Vector3.h"
#include "WW3D2/w3d_file.h"

class HLodDefClass;
class ChunkLoadClass;
class ChunkSaveClass;
class ProxyClass;


// The snap points: a growable Vector3 array that is also reference counted (Free 0x0019C230 releases it
// through the count at +0x1C; Get_Snap_Point 0x0019D5F0 reads the points at +4 and the count at +0x10).
class SnapPointsClass : public DynamicVectorClass<Vector3>, public RefCountClass
{
};

// One proxy of an HLOD: the bone it sits on and the name of the object to create there. The constructor
// 0x0019B0F0 clears the index and the 32 name bytes; Init 0x0019B120 copies the index and strncpy's the
// name (at most 32 bytes) from the W3D record.
class ProxyRecordClass
{
public:
	ProxyRecordClass(void);					// 0x0019B0F0
	void Init(const W3dHLodSubObjectStruct &w3d_data);	// 0x0019B120

	bool operator==(const ProxyRecordClass &that) const { return BoneIndex == that.BoneIndex; }
	bool operator!=(const ProxyRecordClass &that) const { return !(*this == that); }

	int BoneIndex;				// +0x00
	char Name[32];				// +0x04
};
// SIZE 0x24

// The proxies: a fixed array of records that is also reference counted (the count at +0x14;
// Get_Proxy_Count 0x0019C730 returns the length at +8; Get_Proxy 0x0019C750 reads 0x24-byte records).
class ProxyArrayClass : public VectorClass<ProxyRecordClass>, public RefCountClass
{
public:
	ProxyArrayClass(int size) : VectorClass<ProxyRecordClass>(size) {}
};

// Retail creates render objects by name through this free function (renderasset.cpp, 0x00136175).
RenderObjClass *Create_Render_Obj(const char *name);

class HLodClass : public Animatable3DObjClass
{
public:
	// (Nested: the map names the vector's constructor ??0?$VectorClass@VModelNodeClass@HLodClass@@@@.)
	// One sub-object of a level: the model, the bone it follows and an offset from that bone.
	class ModelNodeClass
	{
	public:
		bool operator==(const ModelNodeClass &that) const { return (Model == that.Model) && (BoneIndex == that.BoneIndex); }
		bool operator!=(const ModelNodeClass &that) const { return !(*this == that); }

		RenderObjClass *Model;		// +0x00
		int BoneIndex;			// +0x04
		Vector3 Offset;			// +0x08: Add_Sub_Object_To_Bone's optional position
	};
	// SIZE 0x14

	class ModelArrayClass : public DynamicVectorClass<ModelNodeClass>
	{
	public:
		// The level's constructor (HLodClass 0x0019F8C0 stores it for AdditionalModels): no screen size limit, zero costs.
		ModelArrayClass(void) : MaxScreenSize(FLT_MAX), NonPixelCost(0.0f), PixelCostPerArea(0.0f), BenefitFactor(0.0f) {}

		float MaxScreenSize;		// +0x18
		float NonPixelCost;		// +0x1C
		float PixelCostPerArea;		// +0x20
		float BenefitFactor;		// +0x24
	};
	// SIZE 0x28

	HLodClass(const HLodClass &src);							// 0x001A0410
	HLodClass(const HLodDefClass &def);							// 0x0019FC80
	HLodClass(const char *name, RenderObjClass **lods, int count);				// 0x0019F9B0
	HLodClass &operator=(const HLodClass &that);						// 0x0019FF50
	virtual ~HLodClass(void);								// 1: 0x0019EF70

	virtual RenderObjClass *Clone(void) const;						// 2: 0x001A0500
	virtual int Class_ID(void) const;							// 3: 0x0019F020
	virtual int Get_Num_Polys(void) const;							// 10: 0x0019C890
	virtual void Render(RenderInfoClass &rinfo);						// 12: 0x0019C950
	virtual void Set_Transform(const Matrix3D &m);						// 21: 0x0019B6F0
	virtual void Set_Position(const Vector3 &v);						// 22: 0x0019B760
	virtual void Notify_Added(SceneClass *scene);						// 26: 0x0019CA80
	virtual void Notify_Removed(SceneClass *scene);						// 27: 0x0019CB00
	virtual int Get_Num_Sub_Objects(void) const;						// 28: 0x0019CB90
	virtual int _bfme_ro_v28(void) const;							// 29: 0x0019CBA0 (models over all levels)
	virtual RenderObjClass *Get_Sub_Object(int index) const;				// 30: 0x0019CBC0
	virtual int Remove_Sub_Object(RenderObjClass *subobj);					// 31: 0x0019F040
	virtual int Get_Num_Sub_Objects_On_Bone(int boneindex) const;				// 33: 0x0019CC50
	virtual RenderObjClass *Get_Sub_Object_On_Bone(int index, int boneindex) const;	// 34: 0x0019CCD0
	virtual int Get_Sub_Object_Bone_Index(int lod_index, int model_index) const;		// 35: 0x0019CE80
	virtual int Get_Sub_Object_Bone_Index(RenderObjClass *subobj) const;			// 36: 0x0019CDD0
	virtual int Add_Sub_Object_To_Bone(RenderObjClass *subobj, int bone_index, const Vector3 *offset);	// 38: 0x0019F190
	virtual void Removed_Added_Sub_Objects_From_Bones(void);				// 41: 0x0019CEA0
	virtual void Set_Animation(void);							// 46: 0x0019B780
	virtual void Set_Animation(HAnimClass *motion, float frame, int anim_mode = 0);	// 45: 0x0019B790
	virtual void Set_Animation(HAnimClass *first, float first_frame, HAnimClass *second, float second_frame, float blend);	// 44: 0x0019B7C0
	virtual void Set_Animation(HAnimComboClass *combo);					// 43: 0x0019B7F0
	virtual bool Cast_Ray(RayCollisionTestClass &raytest);					// 60: 0x0019CF50
	virtual bool Intersect_AABox(AABoxIntersectionTestClass &boxtest);			// 61: 0x0019C180
	virtual const SphereClass &Get_Bounding_Sphere(void) const;				// 65: 0x0019B4E0
	virtual const AABoxClass &Get_Bounding_Box(void) const;					// 66: 0x0019B5D0
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;			// 67: 0x0019C520
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const;				// 68: 0x0019C3F0
	virtual void Prepare_LOD(CameraClass &camera);						// 72: 0x0019D0A0
	virtual void Recalculate_Static_LOD_Factors(void);					// 73: 0x0019D150
	virtual void Increment_LOD(void);							// 74: 0x0019D250
	virtual void Decrement_LOD(void);							// 75: 0x0019D320
	virtual float Get_Cost(void) const;							// 76: 0x0019B810
	virtual float Get_Value(void) const;							// 77: 0x0019B820
	virtual float Get_Post_Increment_Value(void) const;					// 78: 0x0019B830
	virtual void Set_LOD_Level(int lod);							// 79: 0x0019D3F0
	virtual int Get_LOD_Level(void) const;							// 80: 0x0019B850
	virtual int Get_LOD_Count(void) const;							// 81: 0x0019B6E0
	virtual void Set_LOD_Bias(float bias);							// 82: 0x0019C5B0
	virtual int Calculate_Cost_Value_Arrays(float screen_area, float *values, float *costs) const;	// 83: 0x0019B860
	virtual RenderObjClass *Get_Current_LOD(void);						// 84: 0x0019B990
	virtual int Get_Num_Snap_Points(void);							// 88: 0x0019D5D0
	virtual void Get_Snap_Point(int index, Vector3 *set);					// 89: 0x0019D5F0
	virtual void Scale(float scale);							// 92: 0x0019D4E0
	virtual void Scale(float scalex, float scaley, float scalez) {}				// 91: folded empty 0x00619FE0
	virtual void Set_Hidden(int onoff);							// 101: 0x0019E520
	virtual void Replace_Texture(const TextureAsset &texture, const TextureAsset &new_texture);	// 124: 0x0019B9C0
	virtual void Recolor_Prototype(const RecolorColor &color);				// 125: 0x0019BA20
	virtual void RecolorHouseColor(const RecolorColor &color, const char *name);		// 126: 0x0019BA70

	// 134..145: the slots this class adds.
	virtual void Set_Max_Screen_Size(int lod_index, float size);				// 134: 0x0019B630
	virtual float Get_Max_Screen_Size(int lod_index) const;					// 135: 0x0019B6A0
	virtual int Get_Lod_Count(void) const;							// 136: 0x0019B6E0 (Get_LOD_Count's body)
	virtual int Get_Lod_Model_Count(int lod_index) const;					// 137: 0x0019C610
	virtual RenderObjClass *Peek_Lod_Model(int lod_index, int model_index) const;		// 138: 0x0019C640
	virtual RenderObjClass *Get_Lod_Model(int lod_index, int model_index) const;		// 139: 0x0019C680
	virtual int Get_Lod_Model_Bone(int lod_index, int model_index) const;			// 140: 0x0019C6C0
	virtual void Add_Lod_Model(int lod, RenderObjClass *robj, int boneindex);		// 141: 0x0019F2B0
	virtual bool Is_NULL_Lod_Included(void) const;						// 142: 0x0019C700
	virtual void Include_NULL_Lod(bool include);						// 143: 0x001A0560
	virtual int Get_Proxy_Count(void) const;						// 144: 0x0019C730
	virtual bool Get_Proxy(int index, ProxyClass &proxy) const;				// 145: 0x0019C750

protected:
	HLodClass(void);									// 0x0019F8C0
	void Free(void);									// 0x0019C230
	virtual void Update_Sub_Object_Transforms(void);					// 42: 0x0019D640
	virtual void Update_Obj_Space_Bounding_Volumes(void);					// 69: 0x0019DB50

	int LodCount;					// +0x120
	int CurLod;					// +0x124
	ModelArrayClass *Lod;				// +0x128: LodCount levels, array-new'd
	int BoundingBoxIndex;				// +0x12C: -1 when none
	float *Cost;					// +0x130
	float *Value;					// +0x134
	ModelArrayClass AdditionalModels;		// +0x138
	SnapPointsClass *SnapPoints;			// +0x160
	ProxyArrayClass *ProxyArray;			// +0x164
	float LODBias;					// +0x168
};

// The description read from a W3D HLOD chunk. Target facts: the constructor 0x0019EC00 clears Name +0,
// HierarchyTreeName +4, LodCount +8, Lod +0xC, builds Aggregates +0x10 (MaxScreenSize FLT_MAX, then zero
// count, names and bones) and clears ProxyArray +0x20; no vftable. Free 0x0019F5B0 frees the two names
// with free, destroys the level array with the vector destructor iterator (0x10-byte elements,
// ~SubObjectArrayClass 0x0019C170) and releases ProxyArray; the destructor 0x001A03C0 is Free then the
// aggregates' Reset 0x0019B210.
class HLodDefClass
{
public:
	HLodDefClass(void);						// 0x0019EC00
	HLodDefClass(HLodClass &src_lod);				// 0x001A0350
	~HLodDefClass(void);						// 0x001A03C0

	bool Load_W3D(ChunkLoadClass &cload);				// 0x0019F800
	bool Save(ChunkSaveClass &csave);				// 0x0019EC90
	const char *Get_Name(void) const { return Name; }
	void Initialize(HLodClass &src_lod);				// 0x0019F620

protected:
	bool Save_Header(ChunkSaveClass &csave);			// 0x0019B140
	bool Save_Lod_Array(ChunkSaveClass &csave);			// 0x0019C020
	bool Save_Aggregate_Array(ChunkSaveClass &csave);		// 0x0019C060

public:
	// One level of the description (0x10 bytes).
	class SubObjectArrayClass
	{
	public:
		SubObjectArrayClass(void);				// 0x0019B1F0
		~SubObjectArrayClass(void) { Reset(); }			// 0x0019C170
		void Reset(void);					// 0x0019B210
		int Compute_Ram_Size(void) const;			// 0x0019B4A0 (role name: 0x10 plus eight bytes and the name per model)

		bool Load_W3D(ChunkLoadClass &cload);			// 0x0019B270
		bool Save_W3D(ChunkSaveClass &csave);			// 0x0019B370

		float MaxScreenSize;		// +0x0
		int ModelCount;			// +0x4
		char **ModelName;		// +0x8: array of model names
		int *BoneIndex;			// +0xC: array of bone indices
	};

private:
	char *Name;					// +0x00
	char *HierarchyTreeName;			// +0x04
	int LodCount;					// +0x08
	SubObjectArrayClass *Lod;			// +0x0C
	SubObjectArrayClass Aggregates;			// +0x10
	ProxyArrayClass *ProxyArray;			// +0x20

	void Free(void);				// 0x0019F5B0
	bool read_header(ChunkLoadClass &cload);	// 0x0019ECE0
	bool read_proxy_array(ChunkLoadClass &cload);	// 0x0019EDD0

	friend class HLodClass;
};

typedef char HLodDefClassSize[(sizeof(HLodDefClass) == 0x24) ? 1 : -1];

typedef char HLodClassSize[(sizeof(HLodClass) == 0x16C) ? 1 : -1];

#endif
