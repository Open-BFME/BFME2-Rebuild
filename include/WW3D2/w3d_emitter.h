// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// w3d_emitter.h: the particle-emitter chunk records of Zero Hour's w3d_file.h (W3D_CHUNK_EMITTER and its children).
// Target facts: ParticleEmitterDefClass's constructor 0x001B1350 clears W3dEmitterInfoStruct as 0x53 dwords (0x14C bytes) at
// +0x18, W3dEmitterInfoStructV2 as 0x1F dwords (0x7C bytes) at +0x164, W3dEmitterExtraInfoStruct as ten dwords at +0x1E0 and
// W3dEmitterLinePropertiesStruct as 0x10 dwords at +0x208, so these records have Zero Hour's sizes.
#pragma once
#ifndef BFME2_W3D_EMITTER_H
#define BFME2_W3D_EMITTER_H

#include "WW3D2/w3d_file.h"

#ifndef W3D_NAME_LEN
#define W3D_NAME_LEN 16
#endif

#define W3D_CURRENT_EMITTER_VERSION				0x00020000

// Emitter chunk ids (ParticleEmitterDefClass::Load_W3D 0x001AF910 and Save_W3D 0x001B0A70 open them).
enum
{
	W3D_CHUNK_EMITTER = 0x00000500,					// description of a particle emitter
	W3D_CHUNK_EMITTER_HEADER = 0x00000501,			// general information such as name and version
	W3D_CHUNK_EMITTER_USER_DATA = 0x00000502,		// user-defined data that specific loaders can switch on
	W3D_CHUNK_EMITTER_INFO = 0x00000503,			// generic particle emitter definition
	W3D_CHUNK_EMITTER_INFOV2 = 0x00000504,			// generic particle emitter definition (version 2.0)
	W3D_CHUNK_EMITTER_PROPS = 0x00000505,			// key-frameable properties
	OBSOLETE_W3D_CHUNK_EMITTER_COLOR_KEYFRAME = 0x00000506,
	OBSOLETE_W3D_CHUNK_EMITTER_OPACITY_KEYFRAME = 0x00000507,
	OBSOLETE_W3D_CHUNK_EMITTER_SIZE_KEYFRAME = 0x00000508,
	W3D_CHUNK_EMITTER_LINE_PROPERTIES = 0x00000509,	// line properties, used by line rendering mode
	W3D_CHUNK_EMITTER_ROTATION_KEYFRAMES = 0x0000050A,	// rotation keys for the particles
	W3D_CHUNK_EMITTER_FRAME_KEYFRAMES = 0x0000050B,	// frame keys (u-v based frame animation)
	W3D_CHUNK_EMITTER_BLUR_TIME_KEYFRAMES = 0x0000050C,	// length of tail for line groups
	W3D_CHUNK_EMITTER_EXTRA_INFO = 0x0000050D		// extra info for the emitter
};

// The Type field of W3dEmitterUserInfoStruct.
enum
{
	EMITTER_TYPEID_DEFAULT = 0,
	EMITTER_TYPEID_COUNT
};

extern const char *EMITTER_TYPE_NAMES[EMITTER_TYPEID_COUNT];

struct W3dEmitterHeaderStruct
{
	unsigned int Version;
	char Name[W3D_NAME_LEN];
};

struct W3dEmitterUserInfoStruct
{
	unsigned int Type;					// One of the EMITTER_TYPEID_ enums
	unsigned int SizeofStringParam;		// Size (in bytes) of the following string data
	char StringParam[1];				// Array of bytes, count = SizeofStringParam
};

struct W3dEmitterInfoStruct
{
	char TextureFilename[260];
	float StartSize;
	float EndSize;
	float Lifetime;
	float EmissionRate;
	float MaxEmissions;
	float VelocityRandom;
	float PositionRandom;
	float FadeTime;
	float Gravity;
	float Elasticity;
	W3dVectorStruct Velocity;
	W3dVectorStruct Acceleration;
	W3dRGBAStruct StartColor;
	W3dRGBAStruct EndColor;
};

struct W3dEmitterExtraInfoStruct
{
	float FutureStartTime;				// Whether to start a particle in the future or not
	unsigned int Padding[9];
};

struct W3dVolumeRandomizerStruct
{
	unsigned int ClassID;
	float Value1;
	float Value2;
	float Value3;
	unsigned int reserved[4];
};

#define W3D_EMITTER_RENDER_MODE_TRI_PARTICLES		0
#define W3D_EMITTER_RENDER_MODE_QUAD_PARTICLES		1
#define W3D_EMITTER_RENDER_MODE_LINE				2
#define W3D_EMITTER_RENDER_MODE_LINEGRP_TETRA		3
#define W3D_EMITTER_RENDER_MODE_LINEGRP_PRISM		4

#define W3D_EMITTER_FRAME_MODE_1x1					0
#define W3D_EMITTER_FRAME_MODE_2x2					1
#define W3D_EMITTER_FRAME_MODE_4x4					2
#define W3D_EMITTER_FRAME_MODE_8x8					3
#define W3D_EMITTER_FRAME_MODE_16x16				4

struct W3dEmitterInfoStructV2
{
	unsigned int BurstSize;
	W3dVolumeRandomizerStruct CreationVolume;
	W3dVolumeRandomizerStruct VelRandom;
	float OutwardVel;
	float VelInherit;
	W3dShaderStruct Shader;
	unsigned int RenderMode;			// render as particles or lines?
	unsigned int FrameMode;				// chop the texture into a grid of smaller squares?
	unsigned int reserved[6];
};

// W3D_CHUNK_EMITTER_PROPS: a W3dEmitterPropertyStruct followed by colour, opacity and size keyframes.
struct W3dEmitterPropertyStruct
{
	unsigned int ColorKeyframes;
	unsigned int OpacityKeyframes;
	unsigned int SizeKeyframes;
	W3dRGBAStruct ColorRandom;
	float OpacityRandom;
	float SizeRandom;
	unsigned int reserved[4];
};

struct W3dEmitterColorKeyframeStruct
{
	float Time;
	W3dRGBAStruct Color;
};

struct W3dEmitterOpacityKeyframeStruct
{
	float Time;
	float Opacity;
};

struct W3dEmitterSizeKeyframeStruct
{
	float Time;
	float Size;
};

// W3D_CHUNK_EMITTER_ROTATION_KEYFRAMES
struct W3dEmitterRotationHeaderStruct
{
	unsigned int KeyframeCount;
	float Random;						// random initial rotational velocity (rotations/sec)
	float OrientationRandom;			// random initial orientation (rotations, 1.0 = 360 degrees)
	unsigned int Reserved[1];
};

struct W3dEmitterRotationKeyframeStruct
{
	float Time;
	float Rotation;						// rotational velocity in rotations/sec
};

// W3D_CHUNK_EMITTER_FRAME_KEYFRAMES
struct W3dEmitterFrameHeaderStruct
{
	unsigned int KeyframeCount;
	float Random;
	unsigned int Reserved[2];
};

struct W3dEmitterFrameKeyframeStruct
{
	float Time;
	float Frame;
};

// W3D_CHUNK_EMITTER_BLUR_TIME_KEYFRAMES
struct W3dEmitterBlurTimeHeaderStruct
{
	unsigned int KeyframeCount;
	float Random;
	unsigned int Reserved[1];
};

struct W3dEmitterBlurTimeKeyframeStruct
{
	float Time;
	float BlurTime;
};

// W3D_CHUNK_EMITTER_LINE_PROPERTIES: the Flags field of W3dEmitterLinePropertiesStruct.
#define W3D_ELINE_MERGE_INTERSECTIONS 				0x00000001	// Merge intersections
#define W3D_ELINE_FREEZE_RANDOM						0x00000002	// Freeze random (note: offsets are in camera space)
#define W3D_ELINE_DISABLE_SORTING					0x00000004	// Disable sorting (even if shader has alpha-blending)
#define W3D_ELINE_END_CAPS 							0x00000008	// Draw end caps on the line
#define W3D_ELINE_TEXTURE_MAP_MODE_MASK 			0xFF000000	// Must cover all possible TextureMapMode values

#define W3D_ELINE_TEXTURE_MAP_MODE_OFFSET 			24
#define W3D_ELINE_UNIFORM_WIDTH_TEXTURE_MAP			0x00000000	// Entire line uses one row of texture (constant V)
#define W3D_ELINE_UNIFORM_LENGTH_TEXTURE_MAP 		0x00000001	// Entire line uses one row of texture stretched length-wise
#define W3D_ELINE_TILED_TEXTURE_MAP					0x00000002	// Tiled continuously over line

#define W3D_ELINE_DEFAULT_BITS	(W3D_ELINE_MERGE_INTERSECTIONS | (W3D_ELINE_UNIFORM_WIDTH_TEXTURE_MAP << W3D_ELINE_TEXTURE_MAP_MODE_OFFSET))

struct W3dEmitterLinePropertiesStruct
{
	unsigned int Flags;
	unsigned int SubdivisionLevel;
	float NoiseAmplitude;
	float MergeAbortFactor;
	float TextureTileFactor;
	float UPerSec;
	float VPerSec;
	unsigned int Reserved[9];
};

typedef char W3dEmitterInfoStructSize[(sizeof(W3dEmitterInfoStruct) == 0x14C) ? 1 : -1];
typedef char W3dEmitterInfoStructV2Size[(sizeof(W3dEmitterInfoStructV2) == 0x7C) ? 1 : -1];
typedef char W3dEmitterExtraInfoStructSize[(sizeof(W3dEmitterExtraInfoStruct) == 0x28) ? 1 : -1];
typedef char W3dEmitterLinePropertiesStructSize[(sizeof(W3dEmitterLinePropertiesStruct) == 0x40) ? 1 : -1];

#endif
