// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// w3d_aggregate.h: the aggregate chunk records of Zero Hour's w3d_file.h (W3D_CHUNK_AGGREGATE and its children). Target
// facts: AggregateDefClass's constructor 0x001A3ED0 clears W3dAggregateInfoStruct as nine dwords at +0x20 and
// W3dAggregateMiscInfo as five dwords at +0x44, so the records have Zero Hour's sizes (0x24 and 0x14).
#pragma once
#ifndef BFME2_W3D_AGGREGATE_H
#define BFME2_W3D_AGGREGATE_H

#ifndef W3D_NAME_LEN
#define W3D_NAME_LEN 16
#endif

enum
{
	W3D_CHUNK_AGGREGATE = 0x00000600,				// description of an aggregate object
	W3D_CHUNK_AGGREGATE_HEADER = 0x00000601,		// general information such as name and version
	W3D_CHUNK_AGGREGATE_INFO = 0x00000602,			// references to 'contained' models
	W3D_CHUNK_TEXTURE_REPLACER_INFO = 0x00000603,	// obsolete texture replacement info
	W3D_CHUNK_AGGREGATE_CLASS_INFO = 0x00000604		// information about the original class that created this aggregate
};

#define W3D_CURRENT_AGGREGATE_VERSION			0x00010003

const int MESH_PATH_ENTRIES = 15;
const int MESH_PATH_ENTRY_LEN = (W3D_NAME_LEN * 2);

struct W3dAggregateHeaderStruct
{
	unsigned int Version;
	char Name[W3D_NAME_LEN];
};

struct W3dAggregateInfoStruct
{
	char BaseModelName[W3D_NAME_LEN * 2];
	unsigned int SubobjectCount;
};

struct W3dAggregateSubobjectStruct
{
	char SubobjectName[W3D_NAME_LEN * 2];
	char BoneName[W3D_NAME_LEN * 2];
};

struct W3dTextureReplacerHeaderStruct
{
	unsigned int ReplacedTexturesCount;
};

// Flags used in the W3dAggregateMiscInfo structure
const int W3D_AGGREGATE_FORCE_SUB_OBJ_LOD = 0x00000001;

struct W3dAggregateMiscInfo
{
	unsigned int OriginalClassID;
	unsigned int Flags;
	unsigned int reserved[3];
};

typedef char W3dAggregateInfoStructSize[(sizeof(W3dAggregateInfoStruct) == 0x24) ? 1 : -1];
typedef char W3dAggregateMiscInfoSize[(sizeof(W3dAggregateMiscInfo) == 0x14) ? 1 : -1];
typedef char W3dAggregateSubobjectStructSize[(sizeof(W3dAggregateSubobjectStruct) == 0x40) ? 1 : -1];

#endif
