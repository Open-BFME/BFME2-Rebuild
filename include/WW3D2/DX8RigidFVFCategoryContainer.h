// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// DX8RigidFVFCategoryContainer.h: the FVF container for rigid (non-skin) meshes (dx8renderer.cpp).
// Target facts: the constructor 0x00145420 (FVF, sorting) runs DX8FVFCategoryContainer's 0x00144E90, clears the four
// dwords at +0xE8..+0xF4 and installs vftable 0x00BD3528 (slot 0 the deleting destructor 0x00146930, then Render
// 0x00148100, Add_Mesh 0x00148180, Log, Check_If_Mesh_Fits 0x00143A20, Add_Delayed_Visible_Material_Pass 0x00144D70,
// Render_Delayed_Procedural_Material_Passes 0x00144DF0); the destructor 0x00145460 releases the vertex buffer at +0xE8
// before the base's. The fit test 0x00143A20 reads the vertex buffer's count (word +0xC), the used count +0xEC and
// the index buffer's. SIZE 0xF8.
#pragma once
#ifndef BFME2_DX8RIGIDFVFCATEGORYCONTAINER_H
#define BFME2_DX8RIGIDFVFCATEGORYCONTAINER_H

#include "WW3D2/DX8FVFCategoryContainer.h"

class VertexBufferClass;

class DX8RigidFVFCategoryContainer : public DX8FVFCategoryContainer
{
public:
	DX8RigidFVFCategoryContainer(unsigned int FVF, bool sorting);			// 0x00145420
	virtual ~DX8RigidFVFCategoryContainer(void);					// 0x00145460

	virtual void Render(void);							// 1: 0x00148100
	virtual void Add_Mesh(MeshModelClass *mmc);					// 2: 0x00148180
	virtual void Log(bool only_visible);						// 3: folded empty body
	virtual bool Check_If_Mesh_Fits(MeshModelClass *mmc);				// 4: 0x00143A20
	virtual void Add_Delayed_Visible_Material_Pass(MaterialPassClass *pass, MeshClass *mesh);	// 5: 0x00144D70
	virtual void Render_Delayed_Procedural_Material_Passes(void);			// 6: 0x00144DF0

protected:
	VertexBufferClass *VertexBuffer;		// +0xE8
	int UsedVertices;				// +0xEC
	MatPassTaskClass *DelayedMatPassHead;		// +0xF0
	MatPassTaskClass *DelayedMatPassTail;		// +0xF4
};

typedef char DX8RigidFVFCategoryContainerSize[(sizeof(DX8RigidFVFCategoryContainer) == 0xF8) ? 1 : -1];

#endif
