// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// SortingNodeStruct.h: one queued translucent draw (Zero Hour's SortingNodeStruct, declared there in
// sortingrenderer.cpp; a header here because a class body does not live in a .cpp).
//
// Target facts (retail):
// - operator new(0x2A8) in Get_Sorting_Struct 0x0012FD80; constructor 0x0012E840 (zeroes the three list words,
//   then the render state's shader/material/texture array and its buffer pointers), destructor 0x0012FCA0
//   (releases the render state's references, then DLNodeClass::Remove 0x0012DC00), scalar deleting
//   destructor 0x001303F0.
// - Three list words at +0x00 (following, preceding, owning list): the node is a DLNodeClass with no vtable.
// - The render state sits at +0x0C (shader dword +0x0C, material +0x10, textures +0x14, vertex buffers
//   +0x28C/+0x290, index buffer +0x294, index_base_offset +0x298).
// - +0x29C is one float, the view-space depth the queue is ordered by (Insert 0x0012FE00 computes it from the
//   product of world and view and the centre, then compares it against each queued node's +0x29C);
//   Zero Hour's sphere and transformed centre are not in this build's node.
// - The four ushorts +0x2A0..+0x2A6 are start_index, polygon_count, min_vertex_index, vertex_count
//   (Insert_To_Sorting_Pool 0x0012D480 sums +0x2A2 and +0x2A6).
#pragma once
#ifndef BFME2_SORTINGNODESTRUCT_H
#define BFME2_SORTINGNODESTRUCT_H

#include "WW3D2/dllist.h"
#include "WW3D2/DX8Wrapper.h"

class SortingNodeStruct : public DLNodeClass<SortingNodeStruct>
{
public:
	RenderStateStruct sorting_state;	// +0x0C

	float transformed_z;				// +0x29C
	unsigned short start_index;			// +0x2A0 first index used in the index buffer
	unsigned short polygon_count;		// +0x2A2 three indices make one polygon
	unsigned short min_vertex_index;	// +0x2A4 first vertex used in the vertex buffer
	unsigned short vertex_count;		// +0x2A6 number of vertices used

	SortingNodeStruct();				// 0x0012E840
	~SortingNodeStruct();				// 0x0012FCA0
};
// SIZE 0x2A8
typedef char SortingNodeStructSize[(sizeof(SortingNodeStruct) == 0x2A8) ? 1 : -1];

#endif
