// TerrainTex.h: the W3D terrain texture classes (TerrainTex.cpp).
// Target facts: TerrainTextureClass::setLOD 0x000EF2A0 passes `this` unchanged
// to TextureBaseClass::Peek_D3D_Base_Texture, so the texture handle is its
// first base at +0. The rest of the layout is not recovered yet.
#pragma once
#ifndef BFME2_TERRAINTEX_H
#define BFME2_TERRAINTEX_H

#include "WW3D2/TextureBaseClass.h"

class WorldHeightMap;
class TileData;

class TerrainTextureClass : public TextureClass
{
public:
	void setLOD(int LOD);																// 0x000EF2A0
	int updateFlat(WorldHeightMap *htMap, int xOrigin, int yOrigin, int pixelsPerCell, int cellWidth, int cellHeight);	// 0x000EF2C1
	int updateFlatDXT1(WorldHeightMap *htMap, int xOrigin, int yOrigin, int pixelsPerCell, int cellWidth, int cellHeight);	// 0x000EF4A3
	int updateCliff(TileData **tiles, int a, int b, int c, int d);						// 0x000EF63F
};

#endif
