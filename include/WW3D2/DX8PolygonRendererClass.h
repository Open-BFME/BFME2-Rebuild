// DX8PolygonRendererClass.h: one draw range of a mesh model (an index range
// in one texture category); the mesh model keeps its renderers in a multi-list.
//
// Target facts (dx8polygonrenderer.cpp):
// - The constructor 0x00187DE0 clears MultiListObjectClass's node (+4), stores
//   its seven arguments (index count +0x18, model +8, texture category +0xC,
//   vertex offset +0x14, index offset +0x10, the strip byte +0x2C, pass +0x30),
//   zeroes +0x1C..+0x28, installs vftable 0x007D5AEC (one slot, the deleting
//   destructor 0x00187E70) and appends itself to the model's renderer list
//   (+0x9C, GenericMultiListClass::Internal_Add_Tail 0x00610820).
// - The copy constructor 0x00187E90 takes the new model as its second argument
//   and copies +0xC..+0x30 from the source.
// - The destructor 0x00187CE0 removes itself from its texture category
//   (DX8TextureCategoryClass::Remove_Polygon_Renderer 0x001460E0) when it has
//   one, then runs ~MultiListObjectClass.
// - Set_Vertex_Index_Range 0x001435F0 stores +0x1C and +0x20.
// SIZE 0x34.
#pragma once
#ifndef BFME2_DX8POLYGONRENDERERCLASS_H
#define BFME2_DX8POLYGONRENDERERCLASS_H

#include "WWLib/MultiListObjectClass.h"

class MeshModelClass;
class DX8TextureCategoryClass;
class SphereClass;

class DX8PolygonRendererClass : public MultiListObjectClass
{
public:
	DX8PolygonRendererClass(unsigned int index_count, MeshModelClass *mmc, DX8TextureCategoryClass *tex_cat,
		unsigned int vertex_offset, unsigned int index_offset, bool strip, unsigned int pass);	// 0x00187DE0
	DX8PolygonRendererClass(const DX8PolygonRendererClass &src, MeshModelClass *mmc);		// 0x00187E90
	virtual ~DX8PolygonRendererClass();							// 0x00187CE0

	void Log();										// 0x00187D40
	void Render(int base_vertex_offset);							// 0x00143630
	void Render_Sorted(int base_vertex_offset, const SphereClass &bounding_sphere);		// 0x00143770
	void Set_Vertex_Index_Range(unsigned int min_vertex_index, unsigned int vertex_index_range);	// 0x001435F0
	void set(unsigned int field24, unsigned int field28);	// 0x00143610 (the map's name): stores +0x24 and +0x28, Set_Vertex_Index_Range's twin

	MeshModelClass *Get_Mesh_Model_Class() const { return Model; }
	DX8TextureCategoryClass *Get_Texture_Category() const { return TextureCategory; }
	void Set_Texture_Category(DX8TextureCategoryClass *tex_cat) { TextureCategory = tex_cat; }
	unsigned int Get_Pass() const { return Pass; }

protected:
	MeshModelClass *Model;					// +0x08
	DX8TextureCategoryClass *TextureCategory;		// +0x0C
	unsigned int IndexOffset;				// +0x10
	unsigned int VertexOffset;				// +0x14
	unsigned int IndexCount;				// +0x18
	unsigned int MinVertexIndex;				// +0x1C
	unsigned int VertexIndexRange;				// +0x20
	unsigned int Field_24;					// +0x24 zeroed by the constructor; role not seen yet
	unsigned int Field_28;					// +0x28 zeroed by the constructor; role not seen yet
	bool Strip;						// +0x2C
	unsigned int Pass;					// +0x30
};

typedef char DX8PolygonRendererClassSize[(sizeof(DX8PolygonRendererClass) == 0x34) ? 1 : -1];

#endif
