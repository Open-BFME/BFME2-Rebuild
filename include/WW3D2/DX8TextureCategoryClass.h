// DX8TextureCategoryClass.h: the polygon renderers of one FVF category that
// share textures, shader, vertex material and pass (dx8renderer.cpp).
//
// Target facts:
// - The constructor 0x00144A00 clears MultiListObjectClass's node (+4),
//   installs vftable 0x007D3508 (one slot, the deleting destructor 0x001460C0),
//   stores the pass at +8, builds two texture handles at +0xC (vector
//   constructor with the handle's constructor/destructor) and assigns them from
//   the argument array, stores the shader +0x14 and the vertex material +0x18
//   (adding a reference), builds the renderer list at +0x1C (MultiListClass
//   vftable 0x007D34E0), stores the container at +0x34 and clears the render
//   task head +0x38. SIZE 0x3C.
// - Add_Polygon_Renderer 0x001441D0 links a renderer (after a given one, or at
//   the head) and points it back at this category; Remove_Polygon_Renderer
//   0x001460E0 unlinks it and, when the list is empty, takes the category out
//   of its container and queues it on the mesh renderer's delete list;
//   Clear_Render_List 0x001433F0 clears +0x38; Get_Shader 0x00143440 returns
//   +0x14.
#pragma once
#ifndef BFME2_DX8TEXTURECATEGORYCLASS_H
#define BFME2_DX8TEXTURECATEGORYCLASS_H

#include "WWLib/MultiListObjectClass.h"
#include "WWLib/multilist.h"
#include "WW3D2/ShaderClass.h"
#include "WW3D2/TextureAsset.h"
#include "WW3D2/DX8PolygonRendererClass.h"

class DX8FVFCategoryContainer;
class VertexMaterialClass;
class PolyRenderTaskClass;
class MeshClass;

class DX8TextureCategoryClass : public MultiListObjectClass
{
public:
	virtual ~DX8TextureCategoryClass(void);						// 0x00144AF0

	void Add_Polygon_Renderer(DX8PolygonRendererClass *renderer, DX8PolygonRendererClass *predecessor = 0);	// 0x001441D0
	void Remove_Polygon_Renderer(DX8PolygonRendererClass *renderer);		// 0x001460E0
	void Add_Render_Task(DX8PolygonRendererClass *renderer, MeshClass *mesh);	// 0x00144BD0
	void Render(void);								// 0x00146F50
	void Clear_Render_List(void);							// 0x001433F0
	ShaderClass Get_Shader(void);							// 0x00143440

	DX8FVFCategoryContainer *Get_Container(void) { return Container; }
	int Get_Pass(void) const { return Pass; }

protected:
	int Pass;							// +0x08
	TextureAsset Textures[2];					// +0x0C
	ShaderClass Shader;						// +0x14
	VertexMaterialClass *Material;					// +0x18
	MultiListClass<DX8PolygonRendererClass> PolygonRendererList;	// +0x1C
	DX8FVFCategoryContainer *Container;				// +0x34
	PolyRenderTaskClass *RenderTaskHead;				// +0x38
};

typedef char DX8TextureCategoryClassSize[(sizeof(DX8TextureCategoryClass) == 0x3C) ? 1 : -1];

#endif
