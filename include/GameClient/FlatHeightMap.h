// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// FlatHeightMap.h: tiled terrain height-map renderer deriving from BaseHeightMapRenderObjClass.
#pragma once
#ifndef BFME2_FLATHEIGHTMAP_H
#define BFME2_FLATHEIGHTMAP_H

#include <stddef.h>
#include <vector>
#include "GameClient/BaseHeightMapRenderObjClass.h"

class W3DTerrainBackground;
class RefRenderObjListIterator;

class FlatHeightMapRenderObjClass : public BaseHeightMapRenderObjClass
{
public:
	FlatHeightMapRenderObjClass(Bool useBackground);
	virtual ~FlatHeightMapRenderObjClass();

	virtual void Render(RenderInfoClass &rinfo);
	virtual void On_Frame_Update();
	virtual Int initHeightData(Int x, Int y, WorldHeightMap *pMap, RefRenderObjListIterator *pLightsIterator);
	virtual void doPartialUpdate(const IRegion2D &partialRange, WorldHeightMap *htMap, RefRenderObjListIterator *pLightsIterator);

	void *m_unk0x3884;
	W3DTerrainBackground *m_terrainBackground;
	void *m_unk0x388C;
	Int m_bgWidth;
	Int m_bgHeight;
	UnsignedByte m_unk0x3898[0x10];
	Bool m_useBackground;
	UnsignedByte m_unk0x38A9[0x17];
	Bool m_updating;
	std::vector<Int> m_unk0x38C4;
	UnsignedByte m_unk0x38D0[0x64];
	Int m_updateState;
};

typedef char FlatHeightMapBgOffset[(offsetof(FlatHeightMapRenderObjClass, m_terrainBackground) == 0x3888) ? 1 : -1];
typedef char FlatHeightMapWidthOffset[(offsetof(FlatHeightMapRenderObjClass, m_bgWidth) == 0x3890) ? 1 : -1];
typedef char FlatHeightMapHeightOffset[(offsetof(FlatHeightMapRenderObjClass, m_bgHeight) == 0x3894) ? 1 : -1];
typedef char FlatHeightMapUpdatingOffset[(offsetof(FlatHeightMapRenderObjClass, m_updating) == 0x38C0) ? 1 : -1];
typedef char FlatHeightMapStateOffset[(offsetof(FlatHeightMapRenderObjClass, m_updateState) == 0x3934) ? 1 : -1];

#endif
