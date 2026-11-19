// RenderObjClass.h: WW3D2's base of every render object (rendobj.h).
//
// Bases (binary): RefCountClass at +0 is the primary base (vftable 0x007D2F68
// starts with RefCountClass::Delete_This 0x0006CE57 and the deleting destructor
// 0x0013C010); MultiListObjectClass at +8 (the copy constructor 0x0013B430 stores
// its table 0x00BC5C6C at +8, zeroes +0xC, then installs RenderObjClass's
// one-slot secondary table 0x007D2F60). Zero Hour's PersistClass base is gone:
// no slot holds its members.
//
// Virtual order (binary): 129 slots; slots 2..128 are declared below in order,
// each with its retail target. Westwood's rendobj.h gives the names; BFME 2 adds
// slots Zero Hour lacks, kept under the map's spellings (_bfme_*) until their
// meaning is recovered. MSVC 7.1 lays overloads out in reverse declaration
// order (Set_Animation, Add_Sub_Object_To_Bone, Remove_Sub_Objects_From_Bone,
// Get_Sub_Object_Bone_Index, Get_Bone_Transform, Scale). Get_Container is not
// virtual in BFME 2. The alignment of this list with retail was first worked
// out in the old repo's rendobj.h; every slot here was re-checked against the
// target's body (arity from its `ret N`, folded empty/zero/false bodies).
//
// Inline against out of line: members whose retail body sits in rendobj.cpp's
// run (0x0013B430-0x0013C5xx) are out of line. Folded defaults (bare ret,
// return 0/false/true) are inline. The small getters/setters in the
// 0x0006CE6B-0x0006D07F run are COMDAT copies of header inlines, defined here
// from those bodies (Bits values below). Get_Name (0x0006CE6B returns a literal)
// and Get_Bone_Transform (0x0006D06D) stay declared only.
//
// Layout (copy constructor 0x0013B430, default constructor 0x0013BF00): 0xC4.
#pragma once
#ifndef BFME2_RENDEROBJCLASS_H
#define BFME2_RENDEROBJCLASS_H

#include "WWLib/RefCountClass.h"
#include "WWLib/MultiListObjectClass.h"
#include "WWMath/Matrix3D.h"
#include "WWMath/SphereClass.h"
#include "WWMath/AABoxClass.h"

class Vector3;
class SceneClass;
class MeshClass;
class CameraClass;
class RenderInfoClass;
class HAnimClass;
class HAnimComboClass;
class HTreeClass;
class MaterialInfoClass;
class RayCollisionTestClass;
class AABoxIntersectionTestClass;
class TextureAsset;
class RecolorColor;
class IntersectionClass;
class IntersectionResultClass;

class RenderObjClass : public RefCountClass, public MultiListObjectClass
{
public:
	// Values from W3DDebrisDraw::doDrawModule 0x000B1A37's table {2, 1, 2} (loop = 1, once = 2) and Set_Animation's int mode.
	enum AnimMode { ANIM_MODE_MANUAL = 0, ANIM_MODE_LOOP = 1, ANIM_MODE_ONCE = 2 };
	enum
	{
		IS_NOT_HIDDEN = 0x00002000,				// Is_Hidden 0x0006CF00 (shr 0Dh), Set_Hidden 0x0006CF0C
		IS_NOT_ANIMATION_HIDDEN = 0x00004000,	// 0x0006CF20 / 0x0006CF2C
		IS_FORCE_VISIBLE = 0x00008000,			// 0x0006CF40
		BOUNDING_VOLUMES_VALID = 0x00020000,	// Get_Bounding_Sphere 0x0006D028 tests byte +0x12 bit 2
		IS_TRANSLUCENT = 0x00040000,			// 0x0006CF66 / 0x0006CF6F (slots 108/109)
		SUBOBJS_MATCH_LOD = 0x00100000,			// HLodClass::Render 0x0019C950 tests byte +0x12 bit 4
		SUBOBJ_TRANSFORMS_DIRTY = 0x00200000,	// HLodClass's setters 0x0019B760..0x0019B7F0 set it (or byte +0x12, 20h)
		IS_ALPHA = 0x00400000,					// 0x0006CF83 / 0x0006CF8C (slots 110/111)
		IS_ADDITIVE = 0x01000000,				// 0x0006CFF7 / 0x0006D000 (slots 118/119)
		BFME_FLAG_02000000 = 0x02000000,		// slots 112/113
		BFME_FLAG_04000000 = 0x04000000,		// slots 114/115
		BFME_FLAG_08000000 = 0x08000000,		// slots 116/117
		BFME_FLAG_10000000 = 0x10000000,		// 0x0006CF49 / 0x0006CF52 (slots 106/107); meaning not recovered
		IS_NOT_HIDDEN_AT_ALL = IS_NOT_HIDDEN | IS_NOT_ANIMATION_HIDDEN	// 0x6000: Is_Not_Hidden_At_All 0x0006CEEE
	};

	// The LOD value bounds (rendobj.cpp's statics; HLodClass::Calculate_Cost_Value_Arrays 0x0019B860 reads
	// them from 0x007D2F58 and 0x007D2F5C: FLT_MAX and -1).
	// Class_ID values BFME 2's HLodClass tests (Set_Hidden 0x0019E520, Is_NULL_Lod_Included 0x0019C700,
	// Render 0x0019C950, Class_ID 0x0019F020).
	enum
	{
		CLASSID_PARTICLEEMITTER = 0x0F,
		CLASSID_NULL = 0x16,
		CLASSID_HLOD = 0x19,
		CLASSID_OBBOX = 0x1B
	};

	static const float AT_MIN_LOD;
	static const float AT_MAX_LOD;

	RenderObjClass();											// 0x0013BF00
	RenderObjClass(const RenderObjClass &src);					// 0x0013B430
	RenderObjClass &operator=(const RenderObjClass &that);		// 0x0013B5F0
	virtual ~RenderObjClass();									// slot 1: ??_G 0x0013C010, dtor 0x0013BE20

	virtual RenderObjClass *Clone() const = 0;													// 2
	virtual int Class_ID() const;																// 3: 0x0013B5E0
	// 4, 5: only MeshClass overrides them (both slots 0x00181450, `return this`);
	// W3DTreeBuffer and mapper.cpp's callers use slot 5 to reach a MeshClass from
	// a RenderObjClass. No retail spelling found, so the old views' names stay.
	virtual MeshClass *_bfme_ro_v2() const { return 0; }												// 4
	virtual MeshClass *_bfme_ro_v3() const { return 0; }												// 5
	virtual const char *Get_Name() const;														// 6: 0x0006CE6B
	virtual void Set_Name(const char *name) {}													// 7
	virtual const char *Get_Base_Model_Name() const { return 0; }								// 8
	virtual void Set_Base_Model_Name(const char *name) {}										// 9
	virtual int Get_Num_Polys() const { return 0; }												// 10
	virtual int Get_Num_Verts() const { return 0; }												// 11: WorldBuilder asserts name MeshClass::Get_Num_Verts and CompositeRenderObjClass::Get_Num_Verts (0x001A5F00, also HLodClass's slot 11)
	virtual void Render(RenderInfoClass &rinfo) = 0;											// 12
	virtual void On_Frame_Update() {}															// 13
	virtual void Restart() {}																	// 14
	virtual void Add(SceneClass *scene);														// 15: 0x0013BC80
	virtual void Remove();																		// 16: 0x0013BCA0
	virtual SceneClass *Get_Scene();															// 17: 0x0013B770
	virtual SceneClass *Peek_Scene() { return Scene; }											// 18
	virtual void Set_Container(RenderObjClass *con);											// 19: 0x0013B780
	virtual void Validate_Transform() const;													// 20: 0x0013B850
	virtual void Set_Transform(const Matrix3D &m);												// 21: 0x0013B790
	virtual void Set_Position(const Vector3 &v);												// 22: 0x0013B810
	virtual void _bfme_set_indexed_factor(int index, float value) { _bfme_indexed_factors[index] = value; }								// 23: 0x0006CE71 (+0x90)
	virtual float _bfme_get_indexed_factor(int index) const { return _bfme_indexed_factors[index]; }									// 24: 0x0006CE87
	virtual float _bfme_get_factor_product() const;												// 25: 0x0013BC60
	virtual void Notify_Added(SceneClass *scene);												// 26: 0x0013BCD0
	virtual void Notify_Removed(SceneClass *scene);												// 27: 0x0013BCE0
	virtual int Get_Num_Sub_Objects() const { return 0; }										// 28
	virtual int _bfme_ro_v28() const { return Get_Num_Sub_Objects(); }															// 29: 0x0006CE95 (forwards to slot 28)
	virtual RenderObjClass *Get_Sub_Object(int index) const { return 0; }						// 30
	virtual int Remove_Sub_Object(RenderObjClass *robj) { return 0; }							// 31
	virtual RenderObjClass *Get_Sub_Object_By_Name(const char *name, int *index = 0) const;		// 32: 0x0013B8D0
	virtual int Get_Num_Sub_Objects_On_Bone(int boneindex) const { return 0; }					// 33
	virtual RenderObjClass *Get_Sub_Object_On_Bone(int index, int boneindex) const { return 0; }	// 34
	virtual int Get_Sub_Object_Bone_Index(RenderObjClass *subobj) const { return 0; }			// 36
	virtual int Get_Sub_Object_Bone_Index(int lodIndex, int modelIndex) const { return 0; }	// 35
	virtual int Add_Sub_Object_To_Bone(RenderObjClass *subobj, int bone_index, const Vector3 *offset = 0) { return 0; }	// 38
	virtual int Add_Sub_Object_To_Bone(RenderObjClass *subobj, const char *bname, const Vector3 *offset = 0);	// 37: 0x0013B9E0
	virtual int Remove_Sub_Objects_From_Bone(int boneindex);									// 40: 0x0013BA10
	virtual int Remove_Sub_Objects_From_Bone(const char *bname);								// 39: 0x0013BA80
	virtual void Removed_Added_Sub_Objects_From_Bones() {}										// 41: HLodClass 0x0019CEA0 (WorldBuilder names it)
	virtual void Update_Sub_Object_Transforms();												// 42: folded empty body 0x0069E440
	virtual void Set_Animation() {}																// 46
	virtual void Set_Animation(HAnimClass *motion, float frame, int anim_mode = 0) {}			// 45
	virtual void Set_Animation(HAnimClass *motion0, float frame0, HAnimClass *motion1, float frame1, float percentage) {}	// 44
	virtual void Set_Animation(HAnimComboClass *anim_combo) {}									// 43
	virtual HAnimClass *Peek_Animation() { return 0; }											// 47
	virtual int Get_Num_Bones() { return 0; }													// 48
	virtual const char *Get_Bone_Name(int bone_index) { return 0; }							// 49
	virtual int Get_Bone_Index(const char *bonename) { return 0; }								// 50
	virtual Matrix3D Get_Bone_Transform(const char *bonename);									// 52: 0x0006D06D
	virtual Matrix3D Get_Bone_Transform(int boneindex);										// 51: 0x0006D06D (same body)
	virtual bool _bfme_ro_v50(int unk) { return true; }										// 53
	virtual void Capture_Bone(int bindex) {}													// 54
	virtual void Release_Bone(int bindex) {}													// 55
	virtual bool Is_Bone_Captured(int bindex) const { return false; }							// 56
	virtual void Control_Bone(int bindex, const Matrix3D &objtm, bool world_space_translation = false) {}	// 57
	virtual const HTreeClass *Get_HTree() const { return 0; }									// 58
	virtual void _bfme_ro_v56(RenderObjClass *unk) {}														// 59
	virtual bool Cast_Ray(RayCollisionTestClass &raytest) { return false; }					// 60
	// 61: MeshClass's 0x0014B5E0 builds an OBBox test from the argument's AABox
	// (center +4, extent +0x10; constructor 0x0014A3F0) before calling
	// MeshGeometryClass::Intersect_OBBox; WorldBuilder asserts name
	// MeshClass::Intersect_AABox and CompositeRenderObjClass::Intersect_AABox.
	virtual bool Intersect_AABox(AABoxIntersectionTestClass &boxtest) { return false; }		// 61
	virtual bool Intersect(IntersectionClass *intersection, IntersectionResultClass *finalResult);	// 62: 0x0013C3B0
	virtual bool Intersect_Sphere(IntersectionClass *intersection, IntersectionResultClass *finalResult);	// 63: 0x0013BD20
	virtual bool Intersect_Sphere_Quick(IntersectionClass *intersection, IntersectionResultClass *finalResult);	// 64: 0x0013BD70
	virtual const SphereClass &Get_Bounding_Sphere() const { if (!(Bits & BOUNDING_VOLUMES_VALID)) Update_Cached_Bounding_Volumes(); return CachedBoundingSphere; }									// 65: 0x0006D028
	virtual const AABoxClass &Get_Bounding_Box() const { if (!(Bits & BOUNDING_VOLUMES_VALID)) Update_Cached_Bounding_Volumes(); return CachedBoundingBox; }										// 66: 0x0006D03E
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;						// 67: 0x0013BCF0
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const;							// 68: 0x001A1420
	virtual void Update_Obj_Space_Bounding_Volumes() {}										// 69
	virtual void _bfme_set_float_84(float v) { _bfme_float_84 = v; }													// 70: 0x0006CE9A (+0x84)
	virtual float _bfme_get_float_84() const { return _bfme_float_84; }													// 71: 0x0006CEAB
	virtual void Prepare_LOD(CameraClass &camera);												// 72: folded empty 0x00180FD0
	virtual void Recalculate_Static_LOD_Factors() {}											// 73
	virtual void Increment_LOD() {}															// 74
	virtual void Decrement_LOD() {}															// 75
	virtual float Get_Cost() const;															// 76: 0x0013BAA0
	virtual float Get_Value() const;															// 77: 0x00167F59
	virtual float Get_Post_Increment_Value() const;											// 78: 0x00167F60
	virtual void Set_LOD_Level(int lod) {}														// 79
	virtual int Get_LOD_Level() const { return 0; }											// 80
	virtual int Get_LOD_Count() const { return 1; }											// 81
	virtual void Set_LOD_Bias(float bias) {}													// 82
	virtual int Calculate_Cost_Value_Arrays(float screen_area, float *values, float *costs) const;	// 83: 0x0013BAC0
	virtual RenderObjClass *Get_Current_LOD();													// 84: 0x0007E4AF
	virtual MaterialInfoClass *Get_Material_Info() { return 0; }								// 85
	virtual void Set_User_Data(void *value, bool recursive = false) { User_Data = value; }							// 86: 0x0006CEB2
	virtual void *Get_User_Data();																// 87: 0x0030C8A7
	virtual int Get_Num_Snap_Points() { return 0; }											// 88
	virtual void Get_Snap_Point(int index, Vector3 *set) {}									// 89
	virtual float Get_Screen_Size(CameraClass &camera);										// 90: 0x0013C030
	virtual void Scale(float scale) {}															// 92
	virtual void Scale(float scalex, float scaley, float scalez) {}							// 91
	virtual void Set_ObjectScale(float scale) { ObjectScale = scale; }													// 93: 0x0006CEBF
	virtual int Get_Sort_Level() const { return 0; }											// 94
	virtual void Set_Sort_Level(int level) {}													// 95
	virtual int Is_Really_Visible() { return ((Bits & IS_NOT_HIDDEN_AT_ALL) == IS_NOT_HIDDEN_AT_ALL) && rva0013B6D0(); }															// 96: 0x0006CECD
	virtual int Is_Not_Hidden_At_All() { return (Bits & IS_NOT_HIDDEN_AT_ALL) == IS_NOT_HIDDEN_AT_ALL; }														// 97: 0x0006CEEE
	virtual int rva0013B6D0();																	// 98: 0x0013B6D0 (forced visible, or the scene's slot 17 equals VisCheckId)
	// 99: 0x0013B710 stores the second argument at +0x88; WorldBuilder's copy
	// asserts `!visCheckId||scene==Scene` under RenderObjClass::SetVisible.
	// ParticleEmitterClass 0x001A1480 calls it and then its own slot 132.
	virtual void SetVisible(SceneClass *scene, int visCheckId);
	virtual int Is_Hidden() const { return !(Bits & IS_NOT_HIDDEN); }																// 100: 0x0006CF00
	virtual void Set_Hidden(int onoff) { if (onoff) Bits &= ~IS_NOT_HIDDEN; else Bits |= IS_NOT_HIDDEN; }														// 101: 0x0006CF0C
	virtual int Is_Animation_Hidden() const { return !(Bits & IS_NOT_ANIMATION_HIDDEN); }													// 102: 0x0006CF20
	virtual void Set_Animation_Hidden(int onoff) { if (onoff) Bits &= ~IS_NOT_ANIMATION_HIDDEN; else Bits |= IS_NOT_ANIMATION_HIDDEN; }												// 103: 0x0006CF2C
	virtual int Is_Force_Visible() const { return Bits & IS_FORCE_VISIBLE; }														// 104: 0x0006CF40
	virtual void Set_Force_Visible(int onoff);													// 105: 0x0013B720
	virtual int _bfme_ro_flag101() const { return Bits & BFME_FLAG_10000000; }														// 106: 0x0006CF49
	virtual void _bfme_ro_flag102(int onoff) { if (onoff) Bits |= BFME_FLAG_10000000; else Bits &= ~BFME_FLAG_10000000; }													// 107: 0x0006CF52
	virtual int Is_Translucent() const { return Bits & IS_TRANSLUCENT; }																// 108: 0x0006CF66
	virtual void Set_Translucent(int onoff) { if (onoff) Bits |= IS_TRANSLUCENT; else Bits &= ~IS_TRANSLUCENT; }															// 109: 0x0006CF6F
	virtual int Is_Alpha() const { return Bits & IS_ALPHA; }															// 110: 0x0006CF83
	virtual void Set_Alpha(int onoff) { if (onoff) Bits |= IS_ALPHA; else Bits &= ~IS_ALPHA; }														// 111: 0x0006CF8C
	virtual int _bfme_ro_flag109() const { return Bits & BFME_FLAG_02000000; }														// 112: 0x0006CFA0
	virtual void _bfme_ro_flag110(int onoff) { if (onoff) Bits |= BFME_FLAG_02000000; else Bits &= ~BFME_FLAG_02000000; }													// 113: 0x0006CFA9
	virtual int _bfme_ro_flag111() const { return Bits & BFME_FLAG_04000000; }														// 114: 0x0006CFBD
	virtual void _bfme_ro_flag112(int onoff) { if (onoff) Bits |= BFME_FLAG_04000000; else Bits &= ~BFME_FLAG_04000000; }													// 115: 0x0006CFC6
	virtual int _bfme_ro_flag113() const { return Bits & BFME_FLAG_08000000; }														// 116: 0x0006CFDA
	virtual void _bfme_ro_flag114(int onoff) { if (onoff) Bits |= BFME_FLAG_08000000; else Bits &= ~BFME_FLAG_08000000; }													// 117: 0x0006CFE3
	virtual int Is_Additive() const { return Bits & IS_ADDITIVE; }														// 118: 0x0006CFF7
	virtual void Set_Additive(int onoff) { if (onoff) Bits |= IS_ADDITIVE; else Bits &= ~IS_ADDITIVE; }													// 119: 0x0006D000
	virtual int Get_Collision_Type() const { return (Bits & 0xFFE) | 1; }													// 120: 0x0006D014
	virtual void Set_Collision_Type(int type, bool recurse = false);							// 121: 0x0013BAF0
	virtual bool Is_Complete() { return false; }												// 122
	virtual bool Is_In_Scene() { return Scene != 0; }																	// 123: 0x0006D01F
	// 124-126: overridden by MeshClass and HLodClass only (HLodClass's bodies
	// forward to every sub-object). MeshClass 124 0x0014B800 compares each of its
	// material info's textures with the first handle and calls
	// MeshModelClass::Replace_Texture 0x00172BF0; 125 and 126 are the bodies
	// WorldBuilder asserts name MeshClass::Recolor_Prototype and
	// MeshClass::RecolorHouseColor (RecolorColor.h asserts follow them).
	virtual void Replace_Texture(const TextureAsset &texture, const TextureAsset &new_texture) {}	// 124
	virtual void Recolor_Prototype(const RecolorColor &color) {}									// 125
	virtual void RecolorHouseColor(const RecolorColor &color, const char *name) {}				// 126

	RenderObjClass *Get_Container() const { return Container; }
	// Zero Hour's inline: Get_Screen_Size 0x0013C030 expands it, 0x0013B8A0 is the COMDAT copy.
	Vector3 Get_Position() const;															// 0x0013B8A0 (out of line in retail: ParticleEmitterClass::Update_Cached_Bounding_Volumes 0x001A1D70 calls it)
	const Matrix3D &Get_Transform() const { Validate_Transform(); return Transform; }
	int Is_Sub_Objects_Match_LOD_Enabled() const { return Bits & SUBOBJS_MATCH_LOD; }
	void Set_Sub_Objects_Match_LOD(int onoff) { if (onoff) Bits |= SUBOBJS_MATCH_LOD; else Bits &= ~SUBOBJS_MATCH_LOD; }	// out-of-line copy 0x001A30F0
	bool Are_Sub_Object_Transforms_Dirty() { return (Bits & SUBOBJ_TRANSFORMS_DIRTY) != 0; }
	void Set_Sub_Object_Transforms_Dirty(bool onoff) { if (onoff) Bits |= SUBOBJ_TRANSFORMS_DIRTY; else Bits &= ~SUBOBJ_TRANSFORMS_DIRTY; }
	const Matrix3D &Get_Transform_No_Validity_Check() const { return Transform; }
	bool Is_Transform_Identity() const { Validate_Transform(); return IsTransformIdentity; }
	float Get_ObjectScale() const { return ObjectScale; }

	// The scene grid cell code SimpleSceneClass's spatial index keeps here
	// (BFME2SceneSpatialIndex::Insert 0x001415B0 stores it, Remove 0x00141640
	// decodes it and stores -1).
	int Get_Spatial_Cell() const { return SpatialCell; }
	void Set_Spatial_Cell(int cell) { SpatialCell = cell; }

protected:
	virtual void Update_Cached_Bounding_Volumes() const;										// 127: 0x0013C190
	virtual void Update_Sub_Object_Bits();														// 128: 0x0013BB60

	// Bits: the copy constructor sets or clears 0x2000 from the source's
	// Is_Hidden (slot 100); SegmentedLineClass's setters clear 0x20000 before
	// changing anything the bounds depend on (`and byte [this+0x12], 0xFD`).
	// Other bit values are not yet checked here.

	bool Bounding_Volumes_Valid() const { return (Bits & BOUNDING_VOLUMES_VALID) != 0; }
	void Invalidate_Cached_Bounding_Volumes() const { Bits &= ~BOUNDING_VOLUMES_VALID; }
	void Validate_Cached_Bounding_Volumes() const { Bits |= BOUNDING_VOLUMES_VALID; }

	mutable unsigned long Bits;					// +0x10
	unsigned long _bfme_unk_14;					// +0x14 collision input mask (old repo note)
	Matrix3D Transform;							// +0x18
	float ObjectScale;							// +0x48 1.0f after construction
	mutable SphereClass CachedBoundingSphere;	// +0x4C
	mutable AABoxClass CachedBoundingBox;		// +0x5C
	mutable bool IsTransformIdentity;			// +0x74
	SceneClass *Scene;							// +0x78 not copied (zeroed)
	RenderObjClass *Container;					// +0x7C not copied
	void *User_Data;							// +0x80 not copied
	float _bfme_float_84;						// +0x84 slots 70/71
	int VisCheckId;								// +0x88 SetVisible's second argument; not copied
	int SpatialCell;							// +0x8C scene grid cell (x<<20|y<<10|z), -1 when not in one; -1 after copy
	float _bfme_indexed_factors[2];				// +0x90 slots 23/24, copied last
	int _bfme_unk_98;							// +0x98
	char _bfme_opaque_9c[32];					// +0x9C copied byte by byte
	bool _bfme_unk_bc;							// +0xBC
	bool _bfme_unk_bd;							// +0xBD
	void *_bfme_unk_c0;							// +0xC0

	friend class SceneClass;
};
// SIZE 0xC4
typedef char RenderObjClassSize[(sizeof(RenderObjClass) == 0xC4) ? 1 : -1];

#endif
