// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DTerrainBackground.h: background terrain patch renderer owned by FlatHeightMapRenderObjClass.
#pragma once
#ifndef BFME2_W3DTERRAINBACKGROUND_H
#define BFME2_W3DTERRAINBACKGROUND_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/ICoord2D.h"
#include "Common/IRegion2D.h"

class DX8VertexBufferClass;
class DX8IndexBufferClass;
class WorldHeightMap;
class RayCollisionTestClass;
class RenderInfoClass;
class Vector3;

class W3DTerrainBackground
{
public:
	void freeTerrainBuffers();
	Bool updateTexture();
	Bool isEdgeAligned(Int x, Int y, Int step);
	Bool advanceYThenX(ICoord2D &pos, Int xMin, Int yMin, Int xMax, Int yMax);
	Bool advanceScan(ICoord2D &pos, Int xMin, Int yMin, Int xMax, Int yMax);
	void setFlipRecursive(Int x, Int y, Int step);
	void setFlipRecursive(Int x, Int y, Int step, Real threshold);
	Int countUnsetCorners(Int x, Int y, Int step, Bool *c0, Bool *c1, Bool *c2, Bool *c3);
	void checkUnsetEdges(Int x, Int y, Int step, Bool *e0, Bool *e1, Bool *e2, Bool *e3);
	Bool castTriangle(RayCollisionTestClass &raytest, const Vector3 &p0, const Vector3 &p1, const Vector3 &p2);
	void getTriangleIntersection(Vector3 *out, Int x, Int y, Int step, Int corner, const Vector3 &p0, const Vector3 &p1, const Vector3 &p2);
	void doTesselatedUpdate(const IRegion2D &partialRange, WorldHeightMap *htMap, Bool doTextures);
	void doPartialUpdate(const IRegion2D &partialRange, WorldHeightMap *htMap, Bool doTextures, Bool force);
	void calcBlendHeight(Real t, Int count, Real *out);
	void rva00115DD2();

	UnsignedByte m_unk0x00[0x24];
	DX8VertexBufferClass *m_vbTerrain;
	UnsignedInt m_unk0x28;
	DX8IndexBufferClass *m_ibTerrain;
	UnsignedByte m_unk0x30[0x18];
	Int m_numVertices;
	Int m_numIndices;
	Int m_xOrigin;
	Int m_yOrigin;
	Int m_width;
	WorldHeightMap *m_map;
	Bool m_dirty;
	Bool m_flipFlag;
	UnsignedByte m_unk0x62[0x06];
	DX8VertexBufferClass *m_vbCliff;
	UnsignedInt m_unk0x6C;
	DX8IndexBufferClass *m_ibCliff;
	UnsignedByte m_unk0x74[0x14];
	void *m_activeFilter;
	void *m_prevFilter;
	Int m_state;
	UnsignedByte m_unk0x94[0x14];
	Int m_borderA;
	Int m_unk0xAC;
	Int m_borderB;
	Int m_unk0xB4;
	Int m_borderC;
	Int m_unk0xBC;
	Int m_borderD;
	UnsignedByte m_unk0xC4[0x0C];
	UnsignedByte *m_scratchBuffer;
};

typedef char W3DTerrainBackgroundSize[(sizeof(W3DTerrainBackground) == 0xD4) ? 1 : -1];
typedef char W3DTerrainBackgroundOriginOffset[(offsetof(W3DTerrainBackground, m_xOrigin) == 0x50) ? 1 : -1];
typedef char W3DTerrainBackgroundMapOffset[(offsetof(W3DTerrainBackground, m_map) == 0x5C) ? 1 : -1];
typedef char W3DTerrainBackgroundScratchOffset[(offsetof(W3DTerrainBackground, m_scratchBuffer) == 0xD0) ? 1 : -1];

#endif
