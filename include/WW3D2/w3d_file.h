// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// w3d_file.h: records of the W3D model file format that the loaders read.
// Target facts: VertexMaterialClass::Parse_W3dVertexMaterialStruct 0x0013D030
// reads a dword of attribute bits at +0, four byte-RGB colours at +4, +8,
// +0xC and +0x10 (ambient, diffuse, specular, emissive; the fourth byte of
// each is unused) and floats at +0x14 (shininess) and +0x18 (opacity); the
// record is 0x20 bytes (a translucency float closes it).
#pragma once
#ifndef BFME2_W3D_FILE_H
#define BFME2_W3D_FILE_H

struct W3dRGBStruct
{
	unsigned char R;	// +0x0
	unsigned char G;	// +0x1
	unsigned char B;	// +0x2
	unsigned char pad;	// +0x3
};

// The 4-byte colour record with an alpha byte (the per-vertex DCG/DIG records); Zero Hour's W3dRGBAStruct.
struct W3dRGBAStruct
{
	unsigned char R;	// +0x0
	unsigned char G;	// +0x1
	unsigned char B;	// +0x2
	unsigned char A;	// +0x3
};

// Chunk ids inside a vertex material chunk (VertexMaterialClass::Load_W3D
// 0x0013E8B0 switches on 0x2C..0x2F).
enum
{
	W3D_CHUNK_VERTEX_MATERIAL_NAME = 0x2C,
	W3D_CHUNK_VERTEX_MATERIAL_INFO = 0x2D,
	W3D_CHUNK_VERTEX_MAPPER_ARGS0 = 0x2E,
	W3D_CHUNK_VERTEX_MAPPER_ARGS1 = 0x2F
};

// W3dVertexMaterialStruct::Attributes bits used by the loader.
enum
{
	W3DVERTMAT_USE_DEPTH_CUE = 0x00000001,
	W3DVERTMAT_COPY_SPECULAR_TO_DIFFUSE = 0x00000004
};

// Chunk ids inside a mesh chunk (MeshGeometryClass::read_chunks 0x0016D340
// switches on them: 0x03 and 0x04 share the normals reader).
enum
{
	W3D_CHUNK_VERTICES = 0x02,
	W3D_CHUNK_VERTEX_NORMALS = 0x03,
	W3D_CHUNK_SURRENDER_NORMALS = 0x04,
	W3D_CHUNK_MESH_USER_TEXT = 0x0C,
	W3D_CHUNK_VERTEX_COLORS = 0x0D,
	W3D_CHUNK_VERTEX_INFLUENCES = 0x0E,
	W3D_CHUNK_MATERIALS3 = 0x15,
	W3D_CHUNK_TRIANGLES = 0x20,
	W3D_CHUNK_PER_TRI_MATERIALS = 0x21,
	W3D_CHUNK_VERTEX_SHADE_INDICES = 0x22,
	// BFME 2's own additions, as MeshModelClass::read_chunks 0x0018B790 switches on them.
	W3D_CHUNK_FX_SHADERS = 0x50,
	W3D_CHUNK_BFME_60 = 0x60,
	W3D_CHUNK_BFME_61 = 0x61,
	W3D_CHUNK_VERTICES_SECOND_SET = 0xC00,
	W3D_CHUNK_VERTEX_NORMALS_SECOND_SET = 0xC01,
	W3D_CHUNK_AABTREE = 0x90,
	W3D_CHUNK_AABTREE_HEADER = 0x91,
	W3D_CHUNK_AABTREE_POLYINDICES = 0x92,
	W3D_CHUNK_AABTREE_NODES = 0x93
};

// Chunk ids of a mesh's material description (Zero Hour's w3d_file.h; MeshModelClass::read_chunks 0x0018B790 and the material
// readers 0x00188760..0x0018B4F0 switch on them).
enum
{
	W3D_CHUNK_TEXCOORDS = 0x05,		// MeshModelClass::read_texture_stage 0x0018B080's table starts here
	W3D_CHUNK_MESH_HEADER3 = 0x1F,
	W3D_CHUNK_PRELIT_UNLIT = 0x23,
	W3D_CHUNK_PRELIT_VERTEX = 0x24,
	W3D_CHUNK_PRELIT_LIGHTMAP_MULTI_PASS = 0x25,
	W3D_CHUNK_PRELIT_LIGHTMAP_MULTI_TEXTURE = 0x26,
	W3D_CHUNK_MATERIAL_INFO = 0x28,
	W3D_CHUNK_SHADERS = 0x29,
	W3D_CHUNK_VERTEX_MATERIALS = 0x2A,
	W3D_CHUNK_VERTEX_MATERIAL = 0x2B,
	W3D_CHUNK_TEXTURES = 0x30,
	W3D_CHUNK_TEXTURE = 0x31,
	W3D_CHUNK_TEXTURE_NAME = 0x32,
	W3D_CHUNK_TEXTURE_INFO = 0x33,
	W3D_CHUNK_MATERIAL_PASS = 0x38,
	W3D_CHUNK_VERTEX_MATERIAL_IDS = 0x39,
	W3D_CHUNK_SHADER_IDS = 0x3A,
	W3D_CHUNK_DCG = 0x3B,
	W3D_CHUNK_DIG = 0x3C,
	W3D_CHUNK_SCG = 0x3E,
	W3D_CHUNK_FX_SHADER_IDS = 0x3F,		// BFME 2: MeshModelClass::read_material_pass 0x0018B400 hands it to 0x00188AF0
	W3D_CHUNK_TEXTURE_STAGE = 0x48,
	W3D_CHUNK_TEXTURE_IDS = 0x49,
	W3D_CHUNK_STAGE_TEXCOORDS = 0x4A,
	W3D_CHUNK_PER_FACE_TEXCOORD_IDS = 0x4B
};

struct W3dVertexMaterialStruct
{
	unsigned long Attributes;	// +0x00
	W3dRGBStruct Ambient;		// +0x04
	W3dRGBStruct Diffuse;		// +0x08
	W3dRGBStruct Specular;		// +0x0C
	W3dRGBStruct Emissive;		// +0x10
	float Shininess;		// +0x14
	float Opacity;			// +0x18
	float Translucency;		// +0x1C
};

// The older material record. Target facts: VertexMaterialClass::
// Init_From_Material3 0x0013CEA0 multiplies the colours at +4 and +8 by the
// coefficients at +0x14 and +0x18 (diffuse, specular), takes the emissive and
// ambient coefficients at +0xC and +0x10 directly, and reads the shininess at
// +0x1C and the opacity at +0x20.
struct W3dMaterial3Struct
{
	unsigned long Attributes;		// +0x00
	W3dRGBStruct DiffuseColor;		// +0x04
	W3dRGBStruct SpecularColor;		// +0x08
	W3dRGBStruct EmissiveCoefficients;	// +0x0C
	W3dRGBStruct AmbientCoefficients;	// +0x10
	W3dRGBStruct DiffuseCoefficients;	// +0x14
	W3dRGBStruct SpecularCoefficients;	// +0x18
	float Shininess;			// +0x1C
	float Opacity;				// +0x20
};

struct W3dVectorStruct
{
	float X;		// +0x0
	float Y;		// +0x4
	float Z;		// +0x8
};

// The AABTree chunks inside a mesh (AABTreeClass::Load_W3D 0x00198090): Zero Hour's w3d_file.h layouts.
struct W3dMeshAABTreeHeader
{
	unsigned long NodeCount;
	unsigned long PolyCount;
	unsigned long Padding[6];
};

struct W3dMeshAABTreeNode
{
	W3dVectorStruct Min;
	W3dVectorStruct Max;
	unsigned long FrontOrPoly0;			// MSB set: a leaf (the first polygon index); clear: the front child
	unsigned long BackOrPolyCount;
};

// One polygon of a mesh. Target facts: MeshGeometryClass::read_triangles
// 0x0016B020 reads 0x20 bytes per polygon, keeps the low words of +0x0, +0x4
// and +0x8 as the vertex indices, the byte +0xC as the surface type, the
// floats +0x10..+0x18 as the plane normal and the negated float +0x1C as the
// plane's fourth term.
struct W3dTriStruct
{
	unsigned long Vindex[3];	// +0x00
	unsigned long Attributes;	// +0x0C
	W3dVectorStruct Normal;		// +0x10
	float Dist;			// +0x1C
};

// One four-word influence record per vertex. The loader transposes these words
// into four planes and groups consecutive vertices whose four words agree.
struct W3dVertInfStruct
{
	unsigned short Value[4];
};

// Hierarchy (skeleton) records. Target facts: HTreeClass::Load_W3D 0x00166800 opens chunk 0x101 and
// reads a 0x24-byte header (a version dword at +0 compared unsigned with 0x30000, the 16-byte name at
// +4, the pivot count at +0x14; the rest is the centre); a version below 3.0 gets one more pivot, an
// implicit root. Chunk 0x102 holds the pivots, which HTreeClass::read_pivots 0x001614F0 reads 0x3C bytes
// at a time: name +0, parent index +0x10 (-1 for none), translation +0x14, Euler angles +0x20 (unused
// by the reader), rotation quaternion +0x2C.
enum
{
	W3D_CHUNK_HIERARCHY_HEADER = 0x101,
	W3D_CHUNK_PIVOTS = 0x102
};

#define W3D_HTREE_VERSION_3_0 0x30000

struct W3dQuaternionStruct
{
	float Q[4];		// +0x0
};

struct W3dHierarchyStruct
{
	unsigned long Version;		// +0x00
	char Name[16];			// +0x04
	int NumPivots;			// +0x14
	W3dVectorStruct Center;		// +0x18
};

struct W3dPivotStruct
{
	char Name[16];			// +0x00
	int ParentIdx;			// +0x10
	W3dVectorStruct Translation;	// +0x14
	W3dVectorStruct EulerAngles;	// +0x20
	W3dQuaternionStruct Rotation;	// +0x2C
};

// One sub-object of an HLOD record (ProxyRecordClass::Init 0x0019B120 reads the bone index at +0 and
// up to 32 name bytes at +4).
struct W3dHLodSubObjectStruct
{
	unsigned long BoneIndex;	// +0x00
	char Name[32];			// +0x04
};

typedef char W3dHLodSubObjectStructSize[(sizeof(W3dHLodSubObjectStruct) == 0x24) ? 1 : -1];

// HLOD chunk ids (HLodDefClass::Save_Header 0x0019B140, SubObjectArrayClass::Save_W3D 0x0019B370,
// Save_Aggregate_Array 0x0019C060 and Load_W3D 0x0019B270 open them).
enum
{
	W3D_CHUNK_HLOD = 0x700,
	W3D_CHUNK_HLOD_HEADER = 0x701,
	W3D_CHUNK_HLOD_LOD_ARRAY = 0x702,
	W3D_CHUNK_HLOD_SUB_OBJECT_ARRAY_HEADER = 0x703,
	W3D_CHUNK_HLOD_SUB_OBJECT = 0x704,
	W3D_CHUNK_HLOD_AGGREGATE_ARRAY = 0x705,
	W3D_CHUNK_HLOD_PROXY_ARRAY = 0x706
};

#define W3D_CURRENT_HLOD_VERSION 0x10000

struct W3dHLodHeaderStruct
{
	unsigned long Version;		// +0x00
	unsigned long LodCount;		// +0x04
	char Name[16];			// +0x08
	char HierarchyName[16];		// +0x18
};

struct W3dHLodArrayHeaderStruct
{
	unsigned long ModelCount;	// +0x00
	float MaxScreenSize;		// +0x04
};
typedef char W3dHierarchyStructSize[(sizeof(W3dHierarchyStruct) == 0x24) ? 1 : -1];
typedef char W3dPivotStructSize[(sizeof(W3dPivotStruct) == 0x3C) ? 1 : -1];
typedef char W3dRGBStructSize[(sizeof(W3dRGBStruct) == 0x4) ? 1 : -1];
typedef char W3dVertexMaterialStructSize[(sizeof(W3dVertexMaterialStruct) == 0x20) ? 1 : -1];
typedef char W3dTriStructSize[(sizeof(W3dTriStruct) == 0x20) ? 1 : -1];

// Animation channel chunk records, from the field offsets the BFME 2 channel loaders read (motchan.cpp). Each
// loader reads the whole fixed part with one Read and keeps the first data element from it.
struct W3dAnimChannelStruct
{
	unsigned short FirstFrame;		// +0x00
	unsigned short LastFrame;		// +0x02
	unsigned short VectorLen;		// +0x04
	unsigned short Flags;			// +0x06
	unsigned short Pivot;			// +0x08
	unsigned short Pad;			// +0x0A
	float Data[1];				// +0x0C
};

struct W3dBitChannelStruct
{
	unsigned short FirstFrame;		// +0x00
	unsigned short LastFrame;		// +0x02
	unsigned short Flags;			// +0x04
	unsigned short Pivot;			// +0x06
	unsigned char DefaultVal;		// +0x08
	unsigned char Data[1];			// +0x09
};

struct W3dTimeCodedAnimChannelStruct
{
	unsigned long NumTimeCodes;		// +0x00
	unsigned short Pivot;			// +0x04
	unsigned char VectorLen;		// +0x06
	unsigned char Flags;			// +0x07
	unsigned long Data[1];			// +0x08
};

struct W3dTimeCodedBitChannelStruct
{
	unsigned long NumTimeCodes;		// +0x00
	unsigned short Pivot;			// +0x04
	unsigned char Flags;			// +0x06
	unsigned char DefaultVal;		// +0x07
	unsigned long Data[1];			// +0x08
};

struct W3dAdaptiveDeltaAnimChannelStruct
{
	unsigned long NumFrames;		// +0x00
	unsigned short Pivot;			// +0x04
	unsigned char VectorLen;		// +0x06
	unsigned char Flags;			// +0x07
	float Scale;				// +0x08
	unsigned long Data[1];			// +0x0C
};

typedef char W3dAnimChannelStructSize[(sizeof(W3dAnimChannelStruct) == 0x10) ? 1 : -1];
typedef char W3dBitChannelStructSize[(sizeof(W3dBitChannelStruct) == 0x0A) ? 1 : -1];
typedef char W3dTimeCodedAnimChannelStructSize[(sizeof(W3dTimeCodedAnimChannelStruct) == 0x0C) ? 1 : -1];
typedef char W3dTimeCodedBitChannelStructSize[(sizeof(W3dTimeCodedBitChannelStruct) == 0x0C) ? 1 : -1];
typedef char W3dAdaptiveDeltaAnimChannelStructSize[(sizeof(W3dAdaptiveDeltaAnimChannelStruct) == 0x10) ? 1 : -1];

// Chunk ids of an uncompressed hierarchical animation (HRawAnimClass::Load_W3D 0x0018E8D0): the header comes first, then one
// chunk per motion or bit channel.
enum
{
	W3D_CHUNK_ANIMATION_HEADER = 0x201,
	W3D_CHUNK_ANIMATION_CHANNEL = 0x202,
	W3D_CHUNK_BIT_CHANNEL = 0x203
};

#define W3D_ANIMATION_VERSION_3_0 0x30000

// The animation header record (0x2C bytes; Load_W3D reads it with one Read): the version dword, the animation's own name, the
// hierarchy it animates, the frame count and the frame rate.
struct W3dAnimHeaderStruct
{
	unsigned int Version;			// +0x00
	char Name[16];				// +0x04
	char HierarchyName[16];			// +0x14
	unsigned int NumFrames;			// +0x24
	unsigned int FrameRate;			// +0x28
};

typedef char W3dAnimHeaderStructSize[(sizeof(W3dAnimHeaderStruct) == 0x2C) ? 1 : -1];

// The mesh chunk records the loader reads (Zero Hour's w3d_file.h): the header (MeshModelClass::Load_W3D 0x0018BA10
// reads 0x74 bytes of it), the material counts and a texture coordinate.
struct W3dMeshHeader3Struct
{
	unsigned int Version;			// +0x00
	unsigned int Attributes;		// +0x04
	char MeshName[16];			// +0x08
	char ContainerName[16];			// +0x18
	unsigned int NumTris;			// +0x28
	unsigned int NumVertices;		// +0x2C
	unsigned int NumMaterials;		// +0x30
	unsigned int NumDamageStages;		// +0x34
	int SortLevel;				// +0x38
	unsigned int PrelitVersion;		// +0x3C
	unsigned int FutureCounts[1];		// +0x40
	unsigned int VertexChannels;		// +0x44
	unsigned int FaceChannels;		// +0x48
	float Min[3];				// +0x4C
	float Max[3];				// +0x58
	float SphCenter[3];			// +0x64
	float SphRadius;			// +0x70
};
typedef char W3dMeshHeader3StructSize[(sizeof(W3dMeshHeader3Struct) == 0x74) ? 1 : -1];

struct W3dMaterialInfoStruct
{
	unsigned int PassCount;			// +0x00
	unsigned int VertexMaterialCount;	// +0x04
	unsigned int ShaderCount;		// +0x08
	unsigned int TextureCount;		// +0x0C
};

struct W3dTexCoordStruct
{
	float U;				// +0x00
	float V;				// +0x04
};

// Mesh attribute bits (Zero Hour's w3d_file.h; MeshClass::Load_W3D 0x00149CC0 reads them from the model).
#define W3D_MESH_FLAG_TWO_SIDED	0x00002000
#define W3D_MESH_FLAG_CAST_SHADOW	0x00008000
#define W3D_MESH_FLAG_GEOMETRY_TYPE_MASK	0x00FF0000
#define W3D_MESH_FLAG_GEOMETRY_TYPE_NORMAL	0x00000000
#define W3D_MESH_FLAG_GEOMETRY_TYPE_CAMERA_ALIGNED	0x00010000
#define W3D_MESH_FLAG_GEOMETRY_TYPE_SKIN	0x00020000
#define W3D_MESH_FLAG_GEOMETRY_TYPE_CAMERA_ORIENTED	0x00060000

#define W3D_MESH_FLAG_COLLISION_TYPE_MASK	0x00000FF0
#define W3D_MESH_FLAG_COLLISION_TYPE_SHIFT	4
#define W3D_MESH_FLAG_HIDDEN				0x00001000

// The 16-byte shader record of a W3D file (Zero Hour's w3d_file.h layout; the W3d_Shader_Get/Set accessors of
// w3d_util.cpp 0x001A4459..0x001A4520 read and write these bytes: depth compare +0, depth mask +1, destination
// blend +3, primary gradient +5, secondary gradient +6, source blend +7, texturing +8, detail colour +9, detail
// alpha +0xA, alpha test +0xC, post-detail colour +0xD and alpha +0xE).
struct W3dShaderStruct
{
	unsigned char DepthCompare;		// +0x00
	unsigned char DepthMask;		// +0x01
	unsigned char ColorMask;		// +0x02 obsolete and ignored
	unsigned char DestBlend;		// +0x03
	unsigned char FogFunc;			// +0x04 obsolete and ignored
	unsigned char PriGradient;		// +0x05
	unsigned char SecGradient;		// +0x06
	unsigned char SrcBlend;			// +0x07
	unsigned char Texturing;		// +0x08
	unsigned char DetailColorFunc;	// +0x09
	unsigned char DetailAlphaFunc;	// +0x0A
	unsigned char ShaderPreset;		// +0x0B obsolete and ignored
	unsigned char AlphaTest;		// +0x0C
	unsigned char PostDetailColorFunc;	// +0x0D
	unsigned char PostDetailAlphaFunc;	// +0x0E
	unsigned char pad[1];			// +0x0F
};

// W3dShaderStruct defaults (Zero Hour's W3DSHADER_*_DEFAULT values; W3d_Shader_Reset stores them).
enum
{
	W3DSHADER_DEPTHCOMPARE_DEFAULT = 3,	// pass if less or equal
	W3DSHADER_DEPTHMASK_DEFAULT = 1,	// write enable
	W3DSHADER_PRIGRADIENT_DEFAULT = 1,	// modulate
	W3DSHADER_SRCBLENDFUNC_DEFAULT = 1	// one
};

// A W3D box (Zero Hour's w3d_file.h; BoxRenderObjClass's constructor 0x00175A40 reads Name, Color, Center, Extent and the
// collision bits of Attributes).
#define W3D_NAME_LEN 16
#define W3D_BOX_ATTRIBUTE_ORIENTED 0x00000001
#define W3D_BOX_ATTRIBUTE_ALIGNED 0x00000002
#define W3D_BOX_ATTRIBUTE_COLLISION_TYPE_MASK 0x00000FF0
#define W3D_BOX_ATTRIBUTE_COLLISION_TYPE_SHIFT 4

struct W3dBoxStruct
{
	unsigned int Version;
	unsigned int Attributes;
	char Name[2 * W3D_NAME_LEN];
	W3dRGBStruct Color;
	W3dVectorStruct Center;
	W3dVectorStruct Extent;
};

// Surface types a ray cast stores in CastResultStruct::SurfaceType (Zero Hour's w3d_file.h SurfaceType, in its order; retail's
// StreakLineClass::Cast_Ray 0x00742C50 and SegmentedLineClass::Cast_Ray 0x0015F2C0 store SURFACE_TYPE_DEFAULT as 13).
enum
{
	SURFACE_TYPE_LIGHT_METAL = 0,
	SURFACE_TYPE_HEAVY_METAL,
	SURFACE_TYPE_WATER,
	SURFACE_TYPE_SAND,
	SURFACE_TYPE_DIRT,
	SURFACE_TYPE_MUD,
	SURFACE_TYPE_GRASS,
	SURFACE_TYPE_WOOD,
	SURFACE_TYPE_CONCRETE,
	SURFACE_TYPE_FLESH,
	SURFACE_TYPE_ROCK,
	SURFACE_TYPE_SNOW,
	SURFACE_TYPE_ICE,
	SURFACE_TYPE_DEFAULT
};

#endif
