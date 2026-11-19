// MeshMatDescClass.h: the per-pass material description of a mesh model (vertex
// materials, shaders, textures and their per-vertex/per-polygon arrays).
//
// Target facts:
// - The constructor 0x0015B480 sets the pass count +0 to 1 and clears the
//   vertex and polygon counts (+4, +8) and +0xC; clears the eight UV buffer
//   pointers +0x10..+0x2C; builds eight texture handles at +0x78 (vector
//   constructor, element constructor 0x00726BE6 / destructor 0x0057098D) and
//   four shaders at +0x98 (ShaderClass default bits 0x0010441B); then per pass
//   sets both UV sources (+0x30, pass*2+stage) to -1, releases and clears both
//   texture handles, clears both texture arrays (+0xC8) and the per-pass slots
//   +0x58, +0x68, +0x98, +0xA8, +0xB8, +0xE8, +0xF8 and +0x108. SIZE 0x118.
// - The destructor 0x0015DC40 calls Reset(0, 0, 0) 0x0015D140 and then the
//   vector destructor over the eight handles at +0x78.
// - Get_UV_Array_Count 0x00143340 counts the non-null +0x10 pointers (at most
//   eight); Get_UV_Array 0x000D1F1A reads the source +0x30 then that buffer's
//   data (+0xC); Set_UV_Source 0x0015A040 writes +0x30.
// - Get_Color_Array 0x000D1FBD creates +0x50[array] on demand as a 0x18-byte
//   share buffer of the vertex count.
// - Set_Single_Shader 0x0015A7E0 / Get_Single_Shader 0x00143370 use +0x98;
//   Set_Single_Material 0x0015A760 swaps a referenced VertexMaterialClass at
//   +0xA8; Peek_Material 0x0015A8E0 reads the material array +0xE8 (data +8)
//   before the single material; Has_Shader_Array 0x001433B0 tests +0xF8.
// - +0xB8 is a second per-pass referenced pointer (setter 0x0015A800, same
//   shape as Set_Single_Material) and +0x108 its per-polygon array (0x0015B7A0
//   creates it from the polygon count); both are BFME additions whose names
//   are not recovered.
// - The pass count +0 is what MeshClass::Get_Num_Polys 0x00149A50 multiplies
//   the polygon count by.
#pragma once
#ifndef BFME2_MESHMATDESCCLASS_H
#define BFME2_MESHMATDESCCLASS_H

#include "WW3D2/ShaderClass.h"
#include "WW3D2/TextureAsset.h"
#include "WW3D2/ShareBufferClass.h"

class UVBufferClass;
class Vector2;
class VertexMaterialClass;
class MatBufferClass;
class OpaqueRefBuffer;
class TexBufferClass;
class RefCountClass;
class MeshModelClass;

class MeshMatDescClass
{
public:
	enum
	{
		MAX_PASSES = 4,
		MAX_TEX_STAGES = 2,
		MAX_COLOR_ARRAYS = 2,
		MAX_UV_ARRAYS = 8
	};

	MeshMatDescClass(void);								// 0x0015B480
	MeshMatDescClass(const MeshMatDescClass &that);					// 0x0015DD10
	~MeshMatDescClass(void);							// 0x0015DC40
	MeshMatDescClass &operator=(const MeshMatDescClass &that);			// 0x0015D800

	bool Is_Empty(void);								// 0x0015A6C0
	void Init_Alternate(MeshMatDescClass &default_materials, MeshMatDescClass &alternate_materials);	// 0x0015A1B0
	void Disable_Backface_Culling(void);						// 0x00188670
	void Reset(int polycount, int vertcount, int passcount);			// 0x0015D140

	int Get_Pass_Count(void) const { return PassCount; }
	void Set_Pass_Count(int passes) { PassCount = passes; }
	int Get_Vertex_Count(void) const { return VertexCount; }
	int Get_Polygon_Count(void) const { return PolyCount; }

	void Make_UV_Array_Unique(int pass, int stage);					// 0x0015D510
	unsigned int *Get_Color_Array(int array, bool create = true);			// 0x000D1FBD
	Vector2 *Get_UV_Array_By_Index(int index, bool create = true);			// 0x000D1F40
	void Make_Color_Array_Unique(int array);					// 0x0015B840
	bool Do_Mappers_Need_Normals(void);						// 0x0015A9F0

	// Per-vertex materials (+0xE8) override the single material (+0xA8).
	bool Has_Material_Array(int pass) const { return MaterialArray[pass] != 0; }
	VertexMaterialClass *Peek_Single_Material(int pass = 0) const { return Material[pass]; }
	VertexMaterialClass *Peek_Material(int vidx, int pass = 0) const;		// 0x0015A8E0
	void Set_Material(int vidx, VertexMaterialClass *vmat, int pass = 0);		// 0x0015D2A0
	void Set_Single_Material(VertexMaterialClass *vmat, int pass = 0);		// 0x0015A760

	// MeshClass::Load_W3D 0x00149CC0 expands the single-shader getter and the array tests in place and calls the
	// per-polygon getter out of line.
	ShaderClass Get_Shader(int pidx, int pass = 0) const;				// 0x0015B6C0
	ShaderClass Get_Single_Shader(int pass = 0) const { return Shader[pass]; }
	bool Has_Shader_Array(int pass = 0) const { return ShaderArray[pass] != 0; }
	// BFME's per-pass object (+0xB8) or its per-polygon array (+0x108).
	bool Has_Pass_Buffer(int pass = 0) const { return Field_B8[pass] != 0 || Field_108[pass] != 0; }
	// Zero Hour mesh model loader helpers (meshmdlio.cpp): a pass already holding data of this kind sends the next chunk to the alternate description.
	bool Has_Material_Data(int pass) const { return Material[pass] != 0 || MaterialArray[pass] != 0; }
	bool Has_Shader_Data(int pass) const { return Shader[pass] != NullShader || ShaderArray[pass] != 0; }
	bool Has_UV(int pass, int stage) const { return UVSource[pass][stage] != -1; }					// 0x00187F50
	// Out-of-line copies 0x00187F70 and 0x00188020 (src/unk); callers expand them. The colour-source values are
	// VertexMaterialClass::ColorSourceType's.
	bool Has_Color_Array(int array) const { return ColorArray[array] != 0; }					// 0x00187F70
	int Get_DCG_Source(int pass) const { return DCGSource[pass]; }									// 0x00188020
	void Set_DCG_Source(int pass, int source) { DCGSource[pass] = source; }
	void Set_Single_Shader(ShaderClass shader, int pass = 0);							// 0x0015A7E0
	void Set_Shader(int pidx, ShaderClass shader, int pass = 0);							// 0x0015DCA0
	void Install_UV_Array(int pass, int stage, Vector2 *uvs, int count);						// 0x0015B960
	void Set_UV_Source(int pass, int stage, int source);								// 0x0015A040
	Vector2 *Get_UV_Array(int pass, int stage);									// 0x000D1F1A
	// Counts the non-null UV buffers (0x00143340 is the out-of-line copy).
	int Get_UV_Array_Count(void)
	{
		int count = 0;
		while ((UV[count] != NULL) && (count < MAX_UV_ARRAYS)) {
			count++;
		}
		return count;
	}
	MatBufferClass *Get_Material_Array(int pass, bool create = true);						// 0x0015B700
	// +0x0B8 / +0x108: BFME's per-pass FX shader object and per-polygon array of them (the retail array literal is "MeshMatDescClass::FXShaderArray") (roles by shape: Set_Single_Material, Get_Material_Array and Set_Material over RefCountClass).
	VertexMaterialClass *Get_Material(int vidx, int pass = 0) const;						// 0x0015A840 (the reference is added)
	RefCountClass *Get_FXShader(int pidx, int pass = 0) const;							// 0x0015A890 (the reference is added)
	RefCountClass *Peek_FXShader(int pidx, int pass = 0) const;							// 0x0015A960
	void Set_Single_FXShader(RefCountClass *object, int pass = 0);							// 0x0015A800
	OpaqueRefBuffer *Get_FXShader_Array(int pass, bool create = true);					// 0x0015B7A0
	void Set_FXShader(int pidx, RefCountClass *object, int pass = 0);						// 0x0015D300
	TexBufferClass *Get_Texture_Array(int pass, int stage, bool create = true);						// 0x0015D360
	ShaderClass *Get_Shader_Array(int pass, bool create = true);							// 0x0015D450

protected:
	void Configure_Material(VertexMaterialClass *mtl, int pass, bool lighting_enabled);				// 0x0015A990
	// MeshModelClass::get_sort_flags 0x00189270 reads the shaders directly.
	friend class MeshModelClass;

	int PassCount;							// +0x000
	int VertexCount;						// +0x004
	int PolyCount;							// +0x008
	int Field_0C;							// +0x00C cleared by the constructor; role not seen yet
	UVBufferClass *UV[MAX_UV_ARRAYS];				// +0x010
	int UVSource[MAX_PASSES][MAX_TEX_STAGES];			// +0x030
	ShareBufferClass<unsigned int> *ColorArray[MAX_COLOR_ARRAYS];	// +0x050
	int DCGSource[MAX_PASSES];					// +0x058 per pass: the diffuse color source Configure_Material 0x0015A990 applies
	int DIGSource[MAX_PASSES];					// +0x068 per pass: the emissive color source it applies
	TextureAsset Texture[MAX_PASSES][MAX_TEX_STAGES];		// +0x078
	ShaderClass Shader[MAX_PASSES];					// +0x098
	VertexMaterialClass *Material[MAX_PASSES];			// +0x0A8
	RefCountClass *Field_B8[MAX_PASSES];				// +0x0B8 per-pass referenced object (0x0015A800)
	TexBufferClass *TextureArray[MAX_PASSES][MAX_TEX_STAGES];	// +0x0C8
	MatBufferClass *MaterialArray[MAX_PASSES];			// +0x0E8
	ShareBufferClass<ShaderClass> *ShaderArray[MAX_PASSES];		// +0x0F8 per-polygon shaders (count +0x10, data +0xC)
	OpaqueRefBuffer *Field_108[MAX_PASSES];					// +0x108 per-polygon array of the +0xB8 objects (0x0015B7A0)

public:
	static ShaderClass NullShader;						// 0x00DF6F2C: marks no shader data (Has_Shader_Data compares against it)
};

typedef char MeshMatDescClassSize[(sizeof(MeshMatDescClass) == 0x118) ? 1 : -1];

#endif
