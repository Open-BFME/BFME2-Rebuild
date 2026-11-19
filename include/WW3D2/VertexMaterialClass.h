// VertexMaterialClass.h: WW3D's per-vertex lighting material (a D3D material
// plus colour sources, texture mappers and UV-array assignments).
//
// Target facts (vertmaterial.cpp, O0013C970):
// - The constructor 0x0013D420 sets NumRefs 1 and vftable 0x007D32A0 (two
//   slots: RefCountClass's Delete_This and the deleting destructor 0x0013D550),
//   zeroes +8..+0x18, builds the StringClass at +0x1C, zeroes the eight mapper
//   pointers +0x20..+0x3C and sets the UV sources +0x40..+0x5C to 0..7, zeroes
//   +0x60, sets the bytes +0x68 = 1 and +0x69 = 0, then allocates the 0x44-byte
//   D3DMATERIAL8 at +8, clears it and sets the diffuse and ambient RGB and the
//   diffuse alpha to 1.0f.
// - The destructor 0x0013C990 releases each non-null mapper (inline
//   Release_Ref) and clears it, frees the material, then destroys the name.
// - Compute_CRC 0x0013CA20 runs CRC_Memory over the material (0x44), +0xC,
//   +0x18, +0x10, +0x14, the UV sources (0x20), the byte +0x69, +0x60 and each
//   non-null mapper pointer.
// - The colour setters 0x0013CAE0.. write the material through +8 and set the
//   byte +0x68 first (the CRC is stale); Get_Shininess 0x0013CD20 reads the
//   material's power (+0x40), Get_Opacity 0x0013CD50 its diffuse alpha (+0xC).
// - Set_*_Color_Source 0x0013CD80.. store 1 for COLOR1, 2 for COLOR2 and 0
//   otherwise (D3DMCS_* values) at +0x10 (ambient), +0x14 (emissive), +0x18
//   (diffuse); the getters map them back the same way.
// - Make_Unique 0x0013C970 takes the next value of the counter at 0x009B62D8
//   into +0x60 and marks the CRC stale.
// SIZE 0x6C.
#pragma once
#ifndef BFME2_VERTEXMATERIALCLASS_H
#define BFME2_VERTEXMATERIALCLASS_H

#include "WWLib/RefCountClass.h"
#include "WWLib/wwstring.h"
#include "WWMath/Vector3.h"
#include "WW3D2/TextureMapperClass.h"

struct _D3DMATERIAL8;
struct W3dMaterial3Struct;
struct W3dVertexMaterialStruct;
class TextureMapperClass;
class ChunkLoadClass;

class VertexMaterialClass : public RefCountClass
{
public:
	enum { MAX_STAGES = 8 };

	enum ColorSourceType
	{
		MATERIAL = 0,
		COLOR1,
		COLOR2
	};

	// Flags bits (Parse_W3dVertexMaterialStruct 0x0013D030 sets 1 and 4 from
	// the file's attribute bits of the same values).
	enum FlagsType
	{
		DEPTH_CUE = 0x01,
		COPY_SPECULAR_TO_DIFFUSE = 0x04
	};

	enum PresetType
	{
		PRELIT_DIFFUSE = 0,
		PRELIT_NODIFFUSE,
		PRESET_COUNT
	};

	VertexMaterialClass(void);							// 0x0013D420
	VertexMaterialClass(const VertexMaterialClass &src);				// 0x0013DAE0
	virtual ~VertexMaterialClass(void);						// 0x0013C990
	VertexMaterialClass *Clone(void);						// 0x0016EAF0
	void Reset_Mappers(void);							// 0x001820C0
	bool Are_Mappers_Time_Variant(void) const;					// 0x001820F0

	VertexMaterialClass &operator=(const VertexMaterialClass &src);		// 0x0013DC00

	void Make_Unique(void);								// 0x0013C970
	int Compute_Ram_Size(void) const;						// 0x0013D250 (the object, its D3D material when allocated, and one mapper object per stage)

	void Set_Flag(FlagsType flag, bool onoff)
	{
		if (onoff)
			Flags |= flag;
		else
			Flags &= ~flag;
		CRCDirty = true;
	}

	void Get_Ambient(Vector3 *set_color) const;					// 0x0013CAB0
	void Set_Ambient(const Vector3 &color);						// 0x0013CAE0
	void Set_Ambient(float r, float g, float b);					// 0x0013CB10
	void Get_Diffuse(Vector3 *set_color) const;					// 0x0013CB50
	void Set_Diffuse(const Vector3 &color);						// 0x0013CB80
	void Set_Diffuse(float r, float g, float b);					// 0x0013CBB0
	void Get_Specular(Vector3 *set_color) const;					// 0x0013CBE0
	void Set_Specular(const Vector3 &color);					// 0x0013CC10
	void Set_Specular(float r, float g, float b);					// 0x0013CC40
	void Get_Emissive(Vector3 *set_color) const;					// 0x0013CC80
	void Set_Emissive(const Vector3 &color);					// 0x0013CCB0
	void Set_Emissive(float r, float g, float b);					// 0x0013CCE0
	float Get_Shininess(void) const;						// 0x0013CD20
	void Set_Shininess(float shin);							// 0x0013CD30
	float Get_Opacity(void) const;							// 0x0013CD50
	void Set_Opacity(float opacity);						// 0x0013CD60

	void Set_Ambient_Color_Source(ColorSourceType src);				// 0x0013CD80
	void Set_Emissive_Color_Source(ColorSourceType src);				// 0x0013CDB0
	void Set_Diffuse_Color_Source(ColorSourceType src);				// 0x0013CDE0
	ColorSourceType Get_Ambient_Color_Source(void);					// 0x0013CE10
	ColorSourceType Get_Emissive_Color_Source(void);				// 0x0013CE30
	ColorSourceType Get_Diffuse_Color_Source(void);					// 0x0013CE50

	// Copy 0x0006E18D (map: Set_Lighting@VertexMaterialClass); Init 0x0013D9B0
	// expands it (CRC-dirty byte, then +0x69).
	void Set_Lighting(bool lighting) { CRCDirty = true; UseLighting = lighting; }
	unsigned long Get_CRC(void) const { if (CRCDirty) { CRC = Compute_CRC(); CRCDirty = false; } return CRC; }	// out-of-line copy 0x001432B0; Equal_Material 0x00143910 expands it

	// Inline: operator= 0x0013DC00 expands it (CRC-dirty byte, add-ref the new
	// mapper, release the old one); 0x0013C7B0 is its out-of-line copy.
	void Set_Mapper(TextureMapperClass *mapper, int stage = 0)
	{
		CRCDirty = true;
		if (mapper != 0)
			mapper->Add_Ref();
		if (Mapper[stage] != 0)
			Mapper[stage]->Release_Ref();
		Mapper[stage] = mapper;
	}
	TextureMapperClass *Peek_Mapper(int stage = 0) { return Mapper[stage]; }	// 0x000633CA
	// Zero Hour's member; MeshMatDescClass::Do_Mappers_Need_Normals 0x0015A9F0 expands it in place.
	bool Do_Mappers_Need_Normals(void)
	{
		for (int stage = 0; stage < MAX_STAGES; stage++) {
			if (Mapper[stage] != 0 && Mapper[stage]->Needs_Normals()) return true;
		}
		return false;
	}

	void Set_UV_Source(int stage, int array_index);					// 0x0013CE70
	int Get_UV_Source(int stage);							// 0x0013CE90

	void Init_From_Material3(const W3dMaterial3Struct &mat3);			// 0x0013CEA0
	void Parse_W3dVertexMaterialStruct(const W3dVertexMaterialStruct &vmat);	// 0x0013D030
	void Parse_Mapping_Args(const W3dVertexMaterialStruct &vmat, char *mapping0_arg_buffer, char *mapping1_arg_buffer);	// 0x0013DD10
	bool Load_W3D(ChunkLoadClass &cload);						// 0x0013E8B0

	static void Init(void);								// 0x0013D9B0
	static void Shutdown(void);							// 0x0013D200
	static VertexMaterialClass *Get_Preset(PresetType type);			// 0x0013D230

private:
	unsigned long Compute_CRC(void) const;						// 0x0013CA20
	void Apply(void) const;								// 0x0013D570
	static void Apply_Null(void);							// 0x0013D180

	_D3DMATERIAL8 *Material;			// +0x08
	unsigned int Flags;				// +0x0C
	int AmbientColorSource;				// +0x10 a D3DMATERIALCOLORSOURCE
	int EmissiveColorSource;			// +0x14
	int DiffuseColorSource;				// +0x18
	StringClass Name;				// +0x1C
	TextureMapperClass *Mapper[MAX_STAGES];		// +0x20
	int UVSource[MAX_STAGES];			// +0x40
	unsigned long UniqueID;				// +0x60
	mutable unsigned long CRC;			// +0x64
	mutable bool CRCDirty;				// +0x68
	bool UseLighting;				// +0x69 (in the CRC; false after construction; Set_Lighting)

	// Init 0x0013D9B0 fills these; Get_Preset 0x0013D230 hands one out with a
	// reference added; Shutdown 0x0013D200 releases both (0x009F3040, two slots).
	static VertexMaterialClass *Presets[PRESET_COUNT];
};

typedef char VertexMaterialClassSize[(sizeof(VertexMaterialClass) == 0x6C) ? 1 : -1];

#endif
