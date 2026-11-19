// PolyRenderTaskClass.h: one queued draw of a polygon renderer for a mesh,
// chained per texture category (dx8renderer.cpp).
//
// Target facts: the constructor 0x00143890 stores the renderer at +0 and the
// mesh at +4, clears the next link at +8 and adds a reference to the mesh;
// the destructor 0x001438B0 releases the mesh. DX8TextureCategoryClass::
// Add_Render_Task 0x00144BD0 allocates one from a pool and pushes it on the
// category's list (+0x38). SIZE 0xC.
#pragma once
#ifndef BFME2_POLYRENDERTASKCLASS_H
#define BFME2_POLYRENDERTASKCLASS_H

class DX8PolygonRendererClass;
class MeshClass;

class PolyRenderTaskClass
{
public:
	PolyRenderTaskClass(DX8PolygonRendererClass *renderer, MeshClass *mesh);	// 0x00143890
	~PolyRenderTaskClass(void);							// 0x001438B0

	DX8PolygonRendererClass *Peek_Polygon_Renderer(void) { return Renderer; }
	MeshClass *Peek_Mesh(void) { return Mesh; }
	PolyRenderTaskClass *Get_Next_Visible(void) { return NextVisible; }
	void Set_Next_Visible(PolyRenderTaskClass *task) { NextVisible = task; }

protected:
	DX8PolygonRendererClass *Renderer;	// +0x0
	MeshClass *Mesh;			// +0x4
	PolyRenderTaskClass *NextVisible;	// +0x8
};

typedef char PolyRenderTaskClassSize[(sizeof(PolyRenderTaskClass) == 0xC) ? 1 : -1];

#endif
