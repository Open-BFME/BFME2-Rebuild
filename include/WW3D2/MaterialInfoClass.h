// MaterialInfoClass.h: the vertex materials and textures a mesh model uses.
//
// Target facts:
// - The constructor 0x001708D0 sets NumRefs 1 and vftable 0x007D46B8 (two
//   slots: Delete_This, the deleting destructor 0x00170960), builds a
//   DynamicVectorClass of vertex material pointers at +8 inline (vftable
//   0x007D4658: Vector +0xC, VectorMax +0x10, IsValid +0x14 = 1, IsAllocated
//   +0x15, ActiveCount +0x18, GrowthStep +0x1C = 10) and a second one at +0x20
//   through the out-of-line VectorClass constructor 0x0016ED80 (vftable
//   0x007D4688, ActiveCount +0x30, GrowthStep +0x34 = 10). SIZE 0x38.
// - The +0x20 vector holds texture handles: MeshClass's slot 124 0x0014B800
//   walks its ActiveCount (+0x30) and data (+0x24), compares each handle's
//   factory pointer and stores a new one with the factory's 16-bit add-ref.
// - Peek_Vertex_Material 0x000636C9 / Get_Vertex_Material 0x000B49A8 index
//   the +8 vector; Add_Vertex_Material 0x00189670, Get_Texture_Index
//   0x0016FC10, Free 0x0016EE70, the copy constructor 0x0016FFD0 and the
//   destructor 0x00170130 are out of line.
#pragma once
#ifndef BFME2_MATERIALINFOCLASS_H
#define BFME2_MATERIALINFOCLASS_H

#include "WWLib/RefCountClass.h"
#include "WWLib/vector.h"
#include "WW3D2/TextureAsset.h"

class VertexMaterialClass;

class MaterialInfoClass : public RefCountClass
{
public:
	MaterialInfoClass(void);							// 0x001708D0
	MaterialInfoClass(const MaterialInfoClass &src);				// 0x0016FFD0
	virtual ~MaterialInfoClass(void);						// 1: 0x00170130

	int Add_Vertex_Material(VertexMaterialClass *vmat);				// 0x00189670
	int Vertex_Material_Count(void) const { return VertexMaterials.Count(); }
	int Texture_Count(void) const { return Textures.Count(); }
	VertexMaterialClass *Peek_Vertex_Material(int index) const;			// 0x000636C9
	VertexMaterialClass *Get_Vertex_Material(int index);				// 0x000B49A8
	TextureAsset Peek_Texture(int index);				// 0x00135F35
	int Get_Texture_Index(const char *name);					// 0x0016FC10

protected:
	DynamicVectorClass<VertexMaterialClass *> VertexMaterials;	// +0x08
	DynamicVectorClass<TextureAsset> Textures;			// +0x20

private:
	void Free(void);								// 0x0016EE70
};

typedef char MaterialInfoClassSize[(sizeof(MaterialInfoClass) == 0x38) ? 1 : -1];

#endif
