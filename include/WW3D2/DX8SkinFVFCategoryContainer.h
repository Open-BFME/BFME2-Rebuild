// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// DX8SkinFVFCategoryContainer.h: the FVF container for skinned meshes (dx8renderer.cpp).
// Target facts: Add_Visible_Skin 0x00143CB0 links a mesh at the head of the list at +0xF0 through the mesh's next
// pointer (+0x304), sets the tail +0xF4 when the list was empty and adds the mesh model's vertex count (+0x28) to the
// dword at +0xE8; clearVisibleSkinList 0x00143C60 empties the list (clearing every next pointer) and zeroes +0xE8
// and +0xEC; Reset 0x001444B0 clears it, deletes every texture category of each pass and releases the index
// buffer. Check_If_Mesh_Fits 0x00143C20 compares the index buffer's free count. SIZE 0xF8.
#pragma once
#ifndef BFME2_DX8SKINFVFCATEGORYCONTAINER_H
#define BFME2_DX8SKINFVFCATEGORYCONTAINER_H

#include "WW3D2/DX8FVFCategoryContainer.h"

class DX8SkinFVFCategoryContainer : public DX8FVFCategoryContainer
{
public:
	virtual void Render(void);							// 1: 0x00148520
	virtual void Add_Mesh(MeshModelClass *mmc);					// 2: 0x00146EE0
	virtual void Log(bool only_visible);						// 3: folded empty body
	virtual bool Check_If_Mesh_Fits(MeshModelClass *mmc);				// 4: 0x00143C20
	virtual void Add_Delayed_Visible_Material_Pass(MaterialPassClass *pass, MeshClass *mesh)	// 5: 0x001460B0
	{
		Add_Visible_Material_Pass(pass, mesh);
	}
	virtual void Render_Delayed_Procedural_Material_Passes(void) {}			// 6: folded empty body

	void Add_Visible_Skin(MeshClass *mesh);						// 0x00143CB0

private:
	void Reset(void);								// 0x001444B0
	void clearVisibleSkinList(void);						// 0x00143C60

	unsigned int VisibleVertexCount;		// +0xE8
	int Field_EC;					// +0xEC cleared with the list; role not seen yet
	MeshClass *VisibleSkinHead;			// +0xF0
	MeshClass *VisibleSkinTail;			// +0xF4
};

typedef char DX8SkinFVFCategoryContainerSize[(sizeof(DX8SkinFVFCategoryContainer) == 0xF8) ? 1 : -1];

#endif
