// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// MaterialRemapperClass.h: maps the vertex materials and textures of one MaterialInfoClass onto those of another (Zero Hour's
// matinfo.h): MeshModelClass::clone_materials 0x001881C0 builds one over the source and the cloned info and remaps the mesh's
// material description through it.
//
// Target facts (retail game.dat): the constructor 0x0016EF20 takes the two infos, the destructor 0x0016F170 releases both
// infos' references, destroys the texture remap array through the vector iterator (8-byte elements whose destructor releases
// each handle) and frees the vertex material remap array. Layout from Remap_Vertex_Material 0x0016ECD0 and Remap_Texture
// 0x0016EC00: SrcMatInfo +0, DestMatInfo +4, TextureCount +8, TextureRemaps +0xC (8-byte records), VertexMaterialCount +0x10,
// VertexMaterialRemaps +0x14 (8-byte records), the last vertex material pair +0x18/+0x1C and the last texture handle pair
// +0x20/+0x24. SIZE 0x28.
#pragma once
#ifndef BFME2_MATERIALREMAPPERCLASS_H
#define BFME2_MATERIALREMAPPERCLASS_H

#include "WW3D2/TextureAsset.h"

class MaterialInfoClass;
class MeshMatDescClass;
class VertexMaterialClass;

class MaterialRemapperClass
{
public:
	MaterialRemapperClass(MaterialInfoClass *src, MaterialInfoClass *dest);	// 0x0016EF20
	~MaterialRemapperClass(void);							// 0x0016F170

	VertexMaterialClass *Remap_Vertex_Material(VertexMaterialClass *src);		// 0x0016ECD0
	void Remap_Mesh(const MeshMatDescClass *srcmeshmatdesc, MeshMatDescClass *destmeshmatdesc);	// 0x0016F220

private:
	struct VmatRemapStruct
	{
		VertexMaterialClass *Src;
		VertexMaterialClass *Dest;
	};

	struct TextureRemapStruct
	{
		TextureAsset Src;
		TextureAsset Dest;
	};

	MaterialInfoClass *SrcMatInfo;				// +0x00
	MaterialInfoClass *DestMatInfo;				// +0x04
	int TextureCount;					// +0x08
	TextureRemapStruct *TextureRemaps;			// +0x0C
	int VertexMaterialCount;				// +0x10
	VmatRemapStruct *VertexMaterialRemaps;			// +0x14
	VertexMaterialClass *LastSrcVmat;			// +0x18
	VertexMaterialClass *LastDestVmat;			// +0x1C
	TextureAsset LastSrcTex;				// +0x20
	TextureAsset LastDestTex;				// +0x24
};

typedef char MaterialRemapperClassSize[(sizeof(MaterialRemapperClass) == 0x28) ? 1 : -1];

#endif
