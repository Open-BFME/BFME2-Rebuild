// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once

#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "Common/Vector.h"
#include "GameLogic/Map/AreaSet.h"
#include "WW3D2/RenderObjClass.h"
#include "WW3D2/ShaderClass.h"
#include "WW3D2/TextureAsset.h"
#include "WWLib/RefCountPtr.h"
#include "WWMath/Vector2.h"

class CameraClass;
class RenderableRiverArea;
class RenderableStandingWaterArea;
class RiverArea;
class StandingWaveArea;
class WaterTracksRenderSystem;
class WaterWaveArea;

// The ctor 0x00082329 installs Snapshot at +0, RenderObjClass at +4
// and the three area observers at +0xC8, +0xCC and +0xD0. The 0x00083A7A
// registration call takes the three sets in that order.
class WaterRenderObjClass :
	public Snapshot,
	public RenderObjClass,
	public AreaSetObserver<StandingWaveArea>,
	public AreaSetObserver<RiverArea>,
	public AreaSetObserver<WaterWaveArea>
{
public:
	enum WaterType
	{
		WATER_TYPE_0_TRANSLUCENT = 0,
		WATER_TYPE_1_FB_REFLECTION,
		WATER_TYPE_2_PVSHADER,
		WATER_TYPE_3_GRIDMESH,
		WATER_TYPE_MAX
	};

	WaterRenderObjClass();
	virtual ~WaterRenderObjClass();

	const char *name() const;

	virtual RenderObjClass *Clone() const;
	virtual int Class_ID() const;
	virtual void Render(RenderInfoClass &renderInfo);
	virtual void Get_Obj_Space_Bounding_Sphere(SphereClass &sphere) const;
	virtual void Get_Obj_Space_Bounding_Box(AABoxClass &box) const;

	virtual void onDestroyingAreaSet(AreaSet<StandingWaveArea> *set);
	virtual void onAreaAddedToSet(Int id, StandingWaveArea *area);
	virtual void onRemovingAreaFromSet(Int id, StandingWaveArea *area);
	virtual void onDestroyingAreaSet(AreaSet<RiverArea> *set);
	virtual void onAreaAddedToSet(Int id, RiverArea *area);
	virtual void onRemovingAreaFromSet(Int id, RiverArea *area);
	virtual void onDestroyingAreaSet(AreaSet<WaterWaveArea> *set);

protected:
	virtual void loadPostProcess();
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

	// 0x0007EF35 loads two texture handles, six packed colors and the
	// scroll rates. The constructor and destructor walk six 0x30-byte settings.
	struct Setting
	{
		TextureAsset skyTexture;
		TextureAsset waterTexture;
		Int waterRepeatCount;
		Real skyTexelsPerUnit;
		UnsignedInt vertex00Diffuse;
		UnsignedInt vertex10Diffuse;
		UnsignedInt vertex11Diffuse;
		UnsignedInt vertex01Diffuse;
		UnsignedInt waterDiffuse;
		UnsignedInt transparentWaterDiffuse;
		Real uScrollPerMs;
		Real vScrollPerMs;
	};

private:
	SceneClass *m_parentScene;									// +0xD4
	ShaderClass m_shaderClass;									// +0xD8
	Vector2 m_textureOffset;									// +0xDC
	Vector2 m_textureScrollPerMs;								// +0xE4
	UnsignedInt m_lastUpdateTime;								// +0xEC
	Bool m_unk0xF0;												// +0xF0
	WaterType m_waterType;										// +0xF4
	TextureAsset m_reflectionTexture;							// +0xF8
	CameraClass *m_reflectionCamera;								// +0xFC
	WaterTracksRenderSystem *m_waterTrackSystem;					// +0x100
	AreaSet<StandingWaveArea> *m_standingWaveAreas;				// +0x104
	_STL::vector<RefCountPtr<RenderableStandingWaterArea> > m_renderableStandingWater;	// +0x108
	AreaSet<RiverArea> *m_riverAreas;								// +0x114
	_STL::vector<RefCountPtr<RenderableRiverArea> > m_renderableRivers;				// +0x118
	AreaSet<WaterWaveArea> *m_waterWaveAreas;						// +0x124
	RefCountClass *m_unk0x128;									// +0x128, released by 0x00082140
	Real m_unk0x12C;											// +0x12C
	Real m_unk0x130;											// +0x130
	Bool m_unk0x134;												// +0x134
	Bool m_unk0x135;												// +0x135
	Setting m_settings[TIME_OF_DAY_COUNT];						// +0x138
	TimeOfDay m_timeOfDay;										// +0x258
	Bool m_renderEnabled;										// +0x25C
	StandingWaveArea *m_currentStandingWave;					// +0x260, 0x00101374
};

// W3DTerrainVisual allocates this extent at 0x00091BA9.
typedef char WaterRenderObjClassSize[(sizeof(WaterRenderObjClass) == 0x264) ? 1 : -1];
