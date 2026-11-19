// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Vertex_Split_Table.h: the per-mesh view DX8's category containers read when they sort a model's polygons into
// texture categories (private to dx8renderer.cpp in Zero Hour).
// Target facts: the constructor 0x001454D0 stores the model at +0, clears the N-patch flag +4 and the allocation flag
// +0x10, sets the flag when the caps byte at +0x13B, MeshModelClass::Needs_Vertex_Normals 0x00171850 and the model's
// flag bit 0x10000 (+0x1A) agree, stores the polygon count at +8, allocates count * 6 bytes with operator new[]
// into +0xC and copies the model's polygon array into it; the destructor 0x00143A70 frees it when +0x10 is set.
// BFME 2 has no gap filler. SIZE 0x14.
#pragma once
#ifndef BFME2_VERTEX_SPLIT_TABLE_H
#define BFME2_VERTEX_SPLIT_TABLE_H

#include "WWMath/Vector3.h"
#include "WWMath/Vector3i.h"
#include "WW3D2/ShaderClass.h"

class MeshModelClass;
class VertexMaterialClass;
class Vector2;

class Vertex_Split_Table
{
public:
	Vertex_Split_Table(MeshModelClass *mmc);					// 0x001454D0
	~Vertex_Split_Table(void);							// 0x00143A70

	const Vector3 *Get_Vertex_Array(void) const;					// 0x00144330
	const Vector3 *Get_Vertex_Normal_Array(void) const;				// 0x00143A90
	const unsigned int *Get_Color_Array(unsigned int index) const;			// 0x00144340
	const Vector2 *Get_UV_Array(unsigned int uv_array_index) const;		// 0x00144360
	unsigned int Get_Vertex_Count(void) const;					// 0x00143AA0
	unsigned int Get_Pass_Count(void) const;					// 0x00143AB0
	VertexMaterialClass *Peek_Material(unsigned int idx, unsigned int pass);		// 0x00145560
	ShaderClass Peek_Shader(unsigned int idx, unsigned int pass);			// 0x001443D0
	unsigned short *Get_Polygon_Array(unsigned int pass);				// 0x00143AC0

private:
	MeshModelClass *Model;				// +0x00
	bool NPatchEnable;				// +0x04
	unsigned int PolygonCount;			// +0x08
	TriIndex *PolygonArray;				// +0x0C
	bool AllocatedPolygonArray;			// +0x10
};

typedef char Vertex_Split_TableSize[(sizeof(Vertex_Split_Table) == 0x14) ? 1 : -1];

#endif
