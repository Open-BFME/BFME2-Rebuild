// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Native 0x000EF008 constructs 4000 prop records of stride 0x30, followed by
// culling state, 96 prop-type records of stride 0x18, and the material and light.
// The Snapshot table at 0x007CEECC has the empty loadPostProcess override,
// GetSnapshotName, and xfer at slots 1, 2, and 3.
#pragma once

#include <vector>
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"
#include "Common/Snapshot.h"
#include "WWMath/SphereClass.h"

class CameraClass;
class GeometryInfo;
class LightClass;
class RenderInfoClass;
class RenderObjClass;
class W3DShroudMaterialPassClass;

struct TProp
{
	RenderObjClass *m_robj;					// +0x00
	Int id;							// +0x04
	Coord3D location;					// +0x08
	Int propType;						// +0x14
	Int shroudStatus;					// +0x18
	Bool visible;						// +0x1C
	SphereClass bounds;					// +0x20
};

struct TPropType
{
	RenderObjClass *m_robj;					// +0x00
	AsciiString m_robjName;					// +0x04
	SphereClass m_bounds;					// +0x08
};

class W3DPropBuffer : private Snapshot
{
public:
	W3DPropBuffer();
	virtual ~W3DPropBuffer();

	void addProp(Int id, Coord3D location, Real angle, Real scale, const AsciiString &modelName, Bool flag);
	void removeProp(Int id);
	void notifyShroudChanged();
	void drawProps(RenderInfoClass &rinfo);
	void removePropsForConstruction(const Coord3D *pos, const GeometryInfo &geom, Real angle);

protected:
	virtual void loadPostProcess();
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

	Int addPropType(const AsciiString &modelName, Bool flag);	// 0x000EE822: loads the model and records its bounds; the type index, 0 when full, -1 when the model is missing

private:
	enum { MAX_PROPS = 4000, MAX_TYPES = 96 };

	TProp m_props[MAX_PROPS];				// +0x00004
	Int m_numProps;						// +0x2EE04
	Bool m_anythingChanged;					// +0x2EE08
	Bool m_initialized;					// +0x2EE09
	Bool m_doCull;						// +0x2EE0A
	std::vector<CameraClass *> m_cullCameras;		// +0x2EE0C
	TPropType m_propTypes[MAX_TYPES];			// +0x2EE18
	Int m_numPropTypes;					// +0x2F718
	W3DShroudMaterialPassClass *m_propShroudMaterialPass;	// +0x2F71C
	Int m_shroudUpdateIndex;				// +0x2F720
	LightClass *m_light;					// +0x2F724
};

typedef char TPropSize[(sizeof(TProp) == 0x30) ? 1 : -1];
typedef char TPropTypeSize[(sizeof(TPropType) == 0x18) ? 1 : -1];
typedef char W3DPropBufferSize[(sizeof(W3DPropBuffer) == 0x2F728) ? 1 : -1];
