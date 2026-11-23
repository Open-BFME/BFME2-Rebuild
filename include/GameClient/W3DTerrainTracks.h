// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DTerrainTracks.h: terrain tracks rendering system.
#pragma once
#ifndef BFME2_W3DTERRAINTACKS_H
#define BFME2_W3DTERRAINTACKS_H

#include <stddef.h>
#include "Common/GameType.h"

class DX8VertexBufferClass;
class DX8IndexBufferClass;
class VertexMaterialClass;
class TerrainTracksRenderObjClass;
class RenderObjClass;

class TerrainTracksRenderObjClassSystem
{
public:
	TerrainTracksRenderObjClassSystem();
	~TerrainTracksRenderObjClassSystem();

	void ReAcquireResources();
	void ReleaseResources();
	void Reset();
	void update();
	void flush();

	DX8VertexBufferClass *m_vertexBuffer;
	DX8IndexBufferClass *m_indexBuffer;
	VertexMaterialClass *m_vertexMaterialClass;
	TerrainTracksRenderObjClass *m_usedModules;
	TerrainTracksRenderObjClass *m_freeModules;
	Int m_unk0x14;
	Int m_unk0x18;
	Int m_edgesToFlush;
	Int m_maxTankTrackOpaqueEdges;
	Int m_maxTankTrackFadeDelay;
};

extern TerrainTracksRenderObjClassSystem *TheTerrainTracksRenderObjClassSystem;

typedef char TerrainTracksSystemSize[(sizeof(TerrainTracksRenderObjClassSystem) == 0x28) ? 1 : -1];
typedef char TerrainTracksSystemUsedOffset[(offsetof(TerrainTracksRenderObjClassSystem, m_usedModules) == 0x0C) ? 1 : -1];
typedef char TerrainTracksSystemEdgesOffset[(offsetof(TerrainTracksRenderObjClassSystem, m_edgesToFlush) == 0x1C) ? 1 : -1];
typedef char TerrainTracksSystemFadeOffset[(offsetof(TerrainTracksRenderObjClassSystem, m_maxTankTrackFadeDelay) == 0x24) ? 1 : -1];

#endif
