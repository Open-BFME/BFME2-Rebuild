// DX8FVFCategoryContainer.h: the texture categories of one vertex format,
// with the per-pass lists of those that have something to draw this frame
// (dx8renderer.cpp).
//
// Target facts:
// - The constructor 0x00144E90 (FVF, sorting) clears MultiListObjectClass's
//   node (+4), installs vftable 0x007D350C (the deleting destructor 0x001461E0
//   and six pure slots), builds two arrays of four 0x18-byte
//   MultiListClass<DX8TextureCategoryClass> at +8 and +0x68 (vector
//   constructor), clears +0xC8..+0xD4, stores the FVF at +0xD8, the pass count
//   4 at +0xDC, the sorting flag +0xE4, clears the bytes +0xE5/+0xE6 and sets
//   +0xE0 to the number of texture coordinate sets the FVF's D3DFVF_TEXn
//   field (bits 8..11) names. SIZE 0xE8.
// - Add_Visible_Texture_Category 0x001441A0 links into +0x68[pass] (Internal_Add) and sets
//   +0xE5; Anything_To_Render 0x00143450 / Any_Delayed_Passes_To_Render
//   0x00143460 return +0xE5 / +0xE6. DX8TextureCategoryClass::Add_Render_Task
//   0x00144BD0 expands Add_Visible_Texture_Category.
// Names of +0xC8..+0xD4 are role names.
#pragma once
#ifndef BFME2_DX8FVFCATEGORYCONTAINER_H
#define BFME2_DX8FVFCATEGORYCONTAINER_H

#include "WWLib/MultiListObjectClass.h"
#include "WWLib/multilist.h"

class DX8TextureCategoryClass;
class MaterialPassClass;
class MeshClass;
class MeshModelClass;
class MatPassTaskClass;
class DX8PolygonRendererClass;
class VertexMaterialClass;
class IndexBufferClass;
class Vertex_Split_Table;

class DX8FVFCategoryContainer : public MultiListObjectClass
{
public:
	enum { MAX_PASSES = 4 };

	DX8FVFCategoryContainer(unsigned int FVF, bool sorting);			// 0x00144E90
	virtual ~DX8FVFCategoryContainer(void);						// 0x00145000

	// Slot order from DX8RigidFVFCategoryContainer's table 0x007D3528; slot 3
	// is the folded empty one-argument body in both derived tables (the debug
	// log in Westwood's design).
	virtual void Render(void) = 0;							// 1
	virtual void Add_Mesh(MeshModelClass *model) = 0;				// 2
	virtual void Log(bool only_visible) = 0;					// 3
	virtual bool Check_If_Mesh_Fits(MeshModelClass *model) = 0;			// 4
	virtual void Add_Delayed_Visible_Material_Pass(MaterialPassClass *pass, MeshClass *mesh) = 0;	// 5
	virtual void Render_Delayed_Procedural_Material_Passes(void) = 0;		// 6

	void Add_Visible_Texture_Category(DX8TextureCategoryClass *category, int pass);	// 0x001441A0
	void Remove_Texture_Category(DX8TextureCategoryClass *category);		// 0x00144C30
	void Add_Visible_Material_Pass(MaterialPassClass *pass, MeshClass *mesh);	// 0x00144CB0
	// Emitted as a comdat in dx8renderer.cpp; MeshModelClass::Replace_VertexMaterial
	// 0x00172D40 calls it.
	void Change_Polygon_Renderer_Material(MultiListClass<DX8PolygonRendererClass> &renderers,
		VertexMaterialClass *vmat, VertexMaterialClass *new_vmat, unsigned int pass);	// 0x001465C0

	unsigned int Get_FVF(void) const { return FVF; }

protected:
	bool Render_Procedural_Material_Passes(void);				// 0x00146130
	void Generate_Texture_Categories(Vertex_Split_Table &split_table, unsigned int vertex_offset);	// 0x00146BA0
	bool Anything_To_Render(void);							// 0x00143450
	bool Any_Delayed_Passes_To_Render(void);					// 0x00143460

	MultiListClass<DX8TextureCategoryClass> TextureCategoryList[MAX_PASSES];	// +0x08
	MultiListClass<DX8TextureCategoryClass> VisibleTextureCategoryList[MAX_PASSES];	// +0x68
	MatPassTaskClass *VisibleMatPassHead;				// +0xC8
	MatPassTaskClass *VisibleMatPassTail;				// +0xCC
	IndexBufferClass *IndexBuffer;					// +0xD0
	int UsedIndices;						// +0xD4
	unsigned int FVF;						// +0xD8
	unsigned int Passes;						// +0xDC
	int UVCoordinateChannels;					// +0xE0
	bool Sorting;							// +0xE4
	bool AnythingToRender;						// +0xE5
	bool AnyDelayedPassesToRender;					// +0xE6
};

typedef char DX8FVFCategoryContainerSize[(sizeof(DX8FVFCategoryContainer) == 0xE8) ? 1 : -1];

#endif
