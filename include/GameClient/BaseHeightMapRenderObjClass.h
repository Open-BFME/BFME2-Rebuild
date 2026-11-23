// BaseHeightMapRenderObjClass.h: the terrain render object that W3DTerrainLogic forwards its height, cliff and
// line-of-sight queries to (TheTerrainRenderObject, VA 0x00DE1EAC). Only the members W3DTerrainLogic calls are
// declared; the layout is not recovered (a generated vtable draft lives in Draft/BaseHeightMapRenderObjClass.h and
// is not included anywhere). Target facts: each W3DTerrainLogic wrapper loads the pointer and calls or tail-jumps to
// the member named in its comment.
#pragma once
#ifndef BFME2_BASEHEIGHTMAPRENDEROBJCLASS_H
#define BFME2_BASEHEIGHTMAPRENDEROBJCLASS_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "Common/IRegion2D.h"
#include "Common/Snapshot.h"
#include "WW3D2/RenderObjClass.h"
#include "WW3D2/DX8CleanupHook.h"

class W3DTerrainLogic;
class WorldHeightMap;
class W3DShroud;
class W3DTaint;
class W3DBibBuffer;
class RefRenderObjListIterator;

class BaseHeightMapRenderObjClass : public RenderObjClass, public DX8_CleanupHook, public Snapshot
{
public:
	BaseHeightMapRenderObjClass();
	virtual ~BaseHeightMapRenderObjClass();

	virtual void Notify_Added(SceneClass *scene);
	virtual bool Cast_Ray(RayCollisionTestClass &raytest);
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const;

	virtual void ReleaseResources(void);
	virtual void ReAcquireResources(void);

	virtual void vslot129();
	virtual void vslot130();
	virtual Int initHeightData(Int x, Int y, WorldHeightMap *pMap, RefRenderObjListIterator *pLightsIterator);
	virtual void vslot132() = 0;
	virtual Int vslot133();
	virtual void vslot134(const void *bounds, Int mode);
	virtual void adjustTerrainLOD(Int adj);
	virtual void doPartialUpdate(const IRegion2D &partialRange, WorldHeightMap *htMap, RefRenderObjListIterator *pLightsIterator) = 0;
	virtual void vslot137(Int value);
	virtual void reset();
	virtual void vslot139(Bool flag);
	virtual void vslot140(void *quad) = 0;
	virtual void vslot141(const void *view, AABoxClass *box) = 0;
	virtual Bool vslot142(Int value);
	virtual void vslot143();
	virtual Int vslot144();
	virtual Real getHeightMapHeight(Real x, Real y, Coord3D *normal) const;
	virtual Real getMaxCellHeight(Real x, Real y) const;
	virtual Int vslot147() = 0;

	Bool isClearLineOfSight(const Coord3D &start, const Coord3D &end) const;
	Bool isCliffCell(Real x, Real y);
	UnsignedByte rva0006B187(Real x, Real y);
	UnsignedByte rva00067225(Real x, Real y);
	Bool rva00067298(Real x, Real y);
	UnsignedByte rva0006730B(Real x, Real y);
	void loadRoadsAndBridges(W3DTerrainLogic *terrain, Bool saveGame);
	void addTerrainBib(Vector3 corners[4], ObjectID id, Bool highlight);
	void addTerrainBibDrawable(Vector3 corners[4], DrawableID id, Bool highlight);
	void removeTerrainBib(ObjectID id);
	void removeTerrainBibDrawable(DrawableID id);
	void removeAllTerrainBibs();
	void removeTerrainBibHighlighting();

	WorldHeightMap *getMap() const { return m_map; }
	Real getTransparentWaterDepth() const { return m_transparentWaterDepth; }
	Real getMinWaterOpacity() const { return m_minWaterOpacity; }

protected:
	virtual void loadPostProcess() {}
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_unk0xCC[0x36F4];
	WorldHeightMap *m_map;
	UnsignedByte m_unk0x37C4[0x20];
	Real m_transparentWaterDepth;
	Real m_minWaterOpacity;
	UnsignedByte m_unk0x37EC[0x70];
	W3DBibBuffer *m_bibBuffer;
	UnsignedByte m_unk0x3860[0x18];
	W3DShroud *m_shroud;
	W3DTaint *m_taint;
	Bool m_needFullUpdate;
};

typedef char BaseHeightMapSize[(sizeof(BaseHeightMapRenderObjClass) == 0x3884) ? 1 : -1];
typedef char BaseHeightMapMapOffset[(offsetof(BaseHeightMapRenderObjClass, m_map) == 0x37C0) ? 1 : -1];
typedef char BaseHeightMapWaterDepthOffset[(offsetof(BaseHeightMapRenderObjClass, m_transparentWaterDepth) == 0x37E4) ? 1 : -1];
typedef char BaseHeightMapWaterOpacityOffset[(offsetof(BaseHeightMapRenderObjClass, m_minWaterOpacity) == 0x37E8) ? 1 : -1];
typedef char BaseHeightMapBibBufferOffset[(offsetof(BaseHeightMapRenderObjClass, m_bibBuffer) == 0x385C) ? 1 : -1];
typedef char BaseHeightMapShroudOffset[(offsetof(BaseHeightMapRenderObjClass, m_shroud) == 0x3878) ? 1 : -1];
typedef char BaseHeightMapNeedFullUpdateOffset[(offsetof(BaseHeightMapRenderObjClass, m_needFullUpdate) == 0x3880) ? 1 : -1];

extern BaseHeightMapRenderObjClass *TheTerrainRenderObject;

#endif
