// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Render2DClass.h: WW3D2's 2D quad renderer (Zero Hour WW3D2/render2d.h), which BFME 2 rebuilt around
// per-texture draw batches.
// Target facts (retail game.dat):
// - No virtual table and no base: the constructor 0x0011C0F0 stores no table pointer (its first store is
//   the integer 2 at +0x00), and W3DDisplay's destructor 0x00049FCB deletes the object with a direct call
//   to ~Render2DClass 0x00119D00 followed by the free, so the destructor is not virtual (Zero Hour's
//   W3DMPO base and virtual destructor are gone). SIZE 0x4C: both allocation sites (W3DDisplay::init
//   0x00046A50 and the Render2DSentenceClass row at 0x00155D7D) ask operator new for 0x4C bytes.
// - The constructor stores 2 at +0x00, (1, 1) at +0x04 and (0, 0) at +0x0C, builds the two arrays at
//   +0x14 and +0x24 (data, capacity and count zeroed, growth step 16) and the vector at +0x34, clears the
//   +0x40 texture, stores -1 at +0x44 and true at +0x48, then calls Reset 0x00119F00.
// - Set_Coordinate_Range 0x001188C0 writes the scale at +0x04 and the offset at +0x0C with Zero Hour's
//   formulas; Convert_Vert 0x0004268C/0x000426CA read them back (out = in * scale + offset).
// - The destructor releases the +0x40 texture handle (AssetFactoryBase::ReleaseRef 0x0061ED10 when its
//   factory is set), destroys the +0x34 vector (vector<BfmeRender2DBatch>::~vector 0x00119C00) and frees
//   the data pointers of the +0x24 and +0x14 arrays with the CRT free.
// - Render 0x0011A040 copies Count * 0x2C bytes of +0x14 into the dynamic vertex buffer and the 16-bit
//   entries of +0x24 into the index buffer, and walks +0x34 in 0x74-byte steps.
// - Set_Texture 0x000456C9 (emitted next to W3DDisplay) compares the incoming handle's factory with +0x40,
//   assigns through the handle's operator= copy 0x000424D0 and stores -1 at +0x44 when a texture is set,
//   0 otherwise; Reset ends with the same store. It is not declared here: the ledger types its parameter
//   as RefCountPtr<TextureClass>, while the bytes are TextureClass handle operations.
// Method signatures are the ledger's; the Add_* bodies live at 0x00042719..0x00042C80 and 0x00045708
// (emitted next to W3DDisplay), Reset, Render and Add_Outline in this class's own object.
#pragma once
#ifndef BFME2_RENDER2DCLASS_H
#define BFME2_RENDER2DCLASS_H

#include <stdlib.h>
#include <vector>
#include "WWMath/Vector2.h"
#include "WW3D2/TextureBaseClass.h"

class RectClass;
class Vector3;
struct BfmeRenderVertex;

// The growable array Render2DClass keeps its vertices and indices in: a CRT buffer grown with realloc
// by a fixed step. Two copies of each routine exist, one per element size (0x2C-byte vertices,
// 16-bit indices): the grow routines 0x00118CC0 and 0x00118D80, the constructor 0x00118D60. The
// owner's constructor 0x0011C0F0 and destructor 0x00119D00 expand the constructor and destructor in
// place.
template <class T>
class Render2DRawArray
{
public:
	Render2DRawArray() : Data(0), Capacity(0), Count(0), GrowthStep(16) {}
	~Render2DRawArray() { free(Data); }

private:
	T *Data;									// +0x00
	unsigned int Capacity;						// +0x04
	unsigned int Count;							// +0x08
	unsigned int GrowthStep;					// +0x0C (16)
};

// One entry of Render2DClass's batch list (0x74 bytes; Render divides the vector's byte span by 0x74).
// The copy constructor 0x00119A80 adds a reference to the +0x00 texture and copies four arrays of
// seven ints; ~vector 0x00119C00 releases each element's texture. Reset 0x00119F00 appends one with
// no texture, the first three arrays at -1 and the last at 0.
class BfmeRender2DBatch
{
public:
	TextureClass Texture;						// +0x00 (allocateGeometry compares it with the owner's texture)
	int m_unk0x04[7];							// +0x04
	int m_unk0x20[7];							// +0x20
	int m_unk0x3C[7];							// +0x3C
	int m_unk0x58[7];							// +0x58
};
typedef char BfmeRender2DBatchSizeCheck[sizeof(BfmeRender2DBatch) == 0x74 ? 1 : -1];

class Render2DClass
{
public:
	Render2DClass();
	~Render2DClass();

	void Reset();
	void Render();

	void Set_Coordinate_Range(const RectClass &range);

	// Retail's vertices carry a depth: Z comes from a renderer-wide value, not from a member.
	void Convert_Vert(Vector3 &vert_out, const Vector2 &vert_in);
	void Convert_Vert(Vector3 &vert_out, float x_in, float y_in);

	void Add_Quad(const RectClass &screen, const RectClass &uv, unsigned long color0, unsigned long color1, unsigned long color2, unsigned long color3);
	void Add_Quad(const RectClass &screen, unsigned long color);
	void Add_Tri(const Vector2 &v0, const Vector2 &v1, const Vector2 &v2, const Vector2 &uv0, const Vector2 &uv1, const Vector2 &uv2, unsigned long color);
	void Add_Line(const Vector2 &a, const Vector2 &b, float width, unsigned long color);
	void Add_Line(const Vector2 &a, const Vector2 &b, float width, unsigned long color, unsigned long color2);
	void Add_Outline(const RectClass &rect, float width, unsigned long color);

protected:
	int m_unk0x00;									// +0x00 (2 after construction)
	Vector2 CoordinateScale;						// +0x04 (Zero Hour's name; Set_Coordinate_Range writes it)
	Vector2 CoordinateOffset;						// +0x0C (Zero Hour's name; the screen UV bias is folded in here)
	Render2DRawArray<BfmeRenderVertex> Vertices;	// +0x14 (Zero Hour's name; Render copies it into the dynamic vertex buffer)
	Render2DRawArray<unsigned short> Indices;		// +0x24 (Zero Hour's name; Render copies it into the index buffer)
	std::vector<BfmeRender2DBatch> m_unk0x34;		// +0x34
	TextureClass Texture;							// +0x40 (Zero Hour's name; a TextureClass handle, one factory pointer)
	int m_unk0x44;									// +0x44 (-1 or 0, see Set_Texture above; allocateGeometry 0x0011BD80 searches the batches while it is negative)
	bool m_unk0x48;									// +0x48 (true after construction; allocateGeometry tests it with the texture; W3DDisplay::drawLine 0x000448CF clears it where Zero Hour calls Enable_Texturing(FALSE))
};
// SIZE 0x4C

typedef char Render2DClassSizeCheck[sizeof(Render2DClass) == 0x4C ? 1 : -1];

#endif
