// MeshClass.h: WW3D's mesh render object, an instance of a shared
// MeshModelClass with its own lighting environment.
//
// Target facts:
// - vftable 0x007D35A8 (129 slots, RenderObjClass's count; secondary
//   MultiListObjectClass table 0x007D35A0 at +8). Own targets: 1 ??_G
//   0x0014BB70 (destructor 0x0014A5B0), 2 Clone 0x001498E0, 3 Class_ID
//   0x0065CE90 (folded `return 0`), 4 and 5 0x00181450 (`return this`), 6/7
//   Get_Name/Set_Name, 10 Get_Num_Polys, 11 Get_Num_Verts, 12 Render
//   0x0014BB90, 25 0x00149670, 60 Cast_Ray 0x0014B240, 61 Intersect_AABox
//   0x0014B5E0, 67/68 the object-space bounds, 85 Get_Material_Info, 91/92
//   Scale 0x0014A6A0/0x0014A630, 94/95 sort level, 124-126 0x0014B800,
//   0x0014B8C0, 0x0014C470, 127 Update_Cached_Bounding_Volumes 0x0014B650.
// - The constructor 0x001495A0 runs RenderObjClass's, clears Model +0xC4 and
//   the lighting environment pointer +0xC8, constructs the embedded
//   LightEnvironmentClass at +0xCC (0x0013F410), sets the Vector3 +0x2F4 to
//   (1,1,1) (slot 25 multiplies the base product by its X), clears +0x300,
//   +0x304, the byte +0x30C, +0x310, the Vector3 +0x314 and the byte +0x320,
//   and takes +0x308 from the running counter at 0x009F36A8. Clone allocates
//   0x324 bytes. SIZE 0x324.
// - Set_Lighting_Environment 0x0014BB20 keeps a caller's environment whose
//   +0 byte is set, else copies it into +0xCC (LightEnvironmentClass's
//   implicit operator= 0x0014A230) and points +0xC8 at the copy.
// - Get_Num_Polys 0x00149A50 / Get_Num_Verts 0x00149A70 multiply the model's
//   current pass count by its polygon / vertex count; Get_Sort_Level and
//   Set_Sort_Level go through the model's +0x1C byte.
// Names of +0x2F4..+0x320 are not recovered.
#pragma once
#ifndef BFME2_MESHCLASS_H
#define BFME2_MESHCLASS_H

#include "WW3D2/RenderObjClass.h"
#include "WW3D2/LightEnvironmentClass.h"
#include "WWMath/Vector3.h"

class MeshModelClass;
class MaterialInfoClass;
class MaterialPassClass;
class IndexBufferClass;
class VertexMaterialClass;
class ChunkLoadClass;
class RenderInfoClass;
class RayCollisionTestClass;
class AABoxIntersectionTestClass;

class MeshClass : public RenderObjClass
{
public:
	MeshClass(void);								// 0x001495A0
	MeshClass(const MeshClass &src);						// 0x00149680
	virtual ~MeshClass(void);							// 1: 0x0014A5B0
	MeshClass &operator=(const MeshClass &that);					// 0x001497A0

	virtual RenderObjClass *Clone(void) const;					// 2: 0x001498E0
	virtual int Class_ID(void) const { return 0; }					// 3: folded 0x0065CE90
	virtual MeshClass *_bfme_ro_v2() const { return (MeshClass *)this; }		// 4: 0x00181450
	virtual MeshClass *_bfme_ro_v3() const { return (MeshClass *)this; }		// 5: 0x00181450
	virtual const char *Get_Name(void) const;					// 6: 0x00149940
	virtual void Set_Name(const char *name);					// 7: 0x00149950
	virtual int Get_Num_Polys(void) const;						// 10: 0x00149A50
	virtual int Get_Num_Verts(void) const;						// 11: 0x00149A70
	virtual void Render(RenderInfoClass &rinfo);					// 12: 0x0014BB90
	virtual float _bfme_get_factor_product() const;					// 25: 0x00149670
	virtual bool Cast_Ray(RayCollisionTestClass &raytest);				// 60: 0x0014B240
	virtual bool Intersect_AABox(AABoxIntersectionTestClass &boxtest);		// 61: 0x0014B5E0
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;		// 67: 0x00149F40
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const;			// 68: 0x00149F80
	virtual MaterialInfoClass *Get_Material_Info(void);				// 85: 0x00149980
	virtual void Scale(float scale);						// 92: 0x0014A630
	virtual void Scale(float scalex, float scaley, float scalez);			// 91: 0x0014A6A0
	virtual int Get_Sort_Level(void) const;						// 94: 0x0014A070
	virtual void Set_Sort_Level(int level);						// 95: 0x0014A090
	virtual void Replace_Texture(const TextureAsset &texture, const TextureAsset &new_texture);	// 124: 0x0014B800
	virtual void Recolor_Prototype(const RecolorColor &color);			// 125: 0x0014B8C0
	virtual void RecolorHouseColor(const RecolorColor &color, const char *name);	// 126: 0x0014C470

	const char *Get_User_Text(void) const;						// 0x00149970
	int Is_Skin(void) const;							// 0x0014A0B0 (role name: the model carries the skin flag)
	int Get_W3D_Flags(void);							// 0x00149960 (the model's attribute word)
	void Replace_VertexMaterial(VertexMaterialClass *vmat, VertexMaterialClass *new_vmat);	// 0x00149C00
	MeshModelClass *Get_Model(void);						// 0x001499B0
	MeshModelClass *Peek_Model(void) { return Model; }
	void Make_Unique(bool force = false);						// 0x00149C10
	void Get_Deformed_Vertices(Vector3 *dst_vert, Vector3 *dst_norm);		// 0x001499D0
	void Get_Deformed_Vertices(Vector3 *dst_vert);					// 0x00149A10
	bool Load_W3D(ChunkLoadClass &cload);						// 0x00149CC0
	void Generate_Culling_Tree(void);						// 0x00149FC0
	void Set_Lighting_Environment(LightEnvironmentClass *light_env);		// 0x0014BB20
	LightEnvironmentClass *Get_Lighting_Environment(void) { return LightEnvironment; }
	int Get_Draw_Call_Count(void) const;						// 0x0014B7C0
	void Render_Material_Pass(MaterialPassClass *pass, IndexBufferClass *index_buffer);	// 0x0014C000
	int Get_Base_Vertex_Offset(void) const { return BaseVertexOffset; }
	void Set_Next_Visible_Skin(MeshClass *next_visible) { NextVisibleSkin = next_visible; }
	MeshClass *Peek_Next_Visible_Skin(void) { return NextVisibleSkin; }

protected:
	void Free(void);								// 0x001498B0
	virtual void Update_Cached_Bounding_Volumes(void) const;			// 127: 0x0014B650

	MeshModelClass *Model;						// +0x0C4
	LightEnvironmentClass *LightEnvironment;			// +0x0C8
	LightEnvironmentClass LocalLightEnv;				// +0x0CC
	Vector3 Field_2F4;						// +0x2F4 (1,1,1); slot 25 uses X
	int BaseVertexOffset;						// +0x300 offset of the first vertex in its vertex buffer
	MeshClass *NextVisibleSkin;					// +0x304 the visible-skin list link (DX8SkinFVFCategoryContainer 0x00143C60, 0x00143CB0); operator= 0x001497A0 swaps references
	unsigned int MeshDebugId;					// +0x308
	bool Field_30C;							// +0x30C
	int Field_310;							// +0x310
	Vector3 Field_314;						// +0x314 (0,0,0)
	bool Field_320;							// +0x320
};

typedef char MeshClassSize[(sizeof(MeshClass) == 0x324) ? 1 : -1];

#endif
