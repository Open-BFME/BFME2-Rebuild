// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DDynamicLight.h: a light whose range and colour fade over a number of frames (Zero Hour's W3DDynamicLight.h).
// Target facts (retail game.dat): vftables 0x00BC6038 (+0) and 0x00BC6034 (+8, MultiListObjectClass); the object is 0x174 bytes.
// LightClass ends at +0x120. The constructor 0x0006DD6F clears everything from there on (Zero Hour leaves the integers
// uninitialised), sets the enabled byte +0x144 and leaves the rest of the fade state at zero: priorEnable +0x120, the previous
// and current cell bounds +0x124..+0x140, enabled +0x144, decayRange +0x145, decayColor +0x146, current decay count +0x148,
// current increase count +0x14C, decay count +0x150, increase count +0x154 (setFrameFade 0x0006DF81 stores them), target
// range +0x158, target ambient +0x15C, target diffuse +0x168. Zero Hour's processMe flag does not exist (+0x121 is padding).
#pragma once
#ifndef BFME2_W3DDYNAMICLIGHT_H
#define BFME2_W3DDYNAMICLIGHT_H

#include "Common/GameType.h"
#include "WW3D2/LightClass.h"
#include "WWMath/Vector3.h"

class BaseHeightMapRenderObjClass;

class W3DDynamicLight : public LightClass
{
	friend class BaseHeightMapRenderObjClass;

protected:
	// Values used by the height map renderer to update the terrain lighting.
	Bool m_priorEnable;				// +0x120
	Int m_prevMinX, m_prevMinY, m_prevMaxX, m_prevMaxY;	// +0x124
	Int m_minX, m_minY, m_maxX, m_maxY;	// +0x134

	Bool m_enabled;					// +0x144
	Bool m_decayRange;				// +0x145
	Bool m_decayColor;				// +0x146
	UnsignedInt m_curDecayFrameCount;		// +0x148
	UnsignedInt m_curIncreaseFrameCount;		// +0x14C
	UnsignedInt m_decayFrameCount;			// +0x150
	UnsignedInt m_increaseFrameCount;		// +0x154
	Real m_targetRange;				// +0x158
	Vector3 m_targetAmbient;			// +0x15C
	Vector3 m_targetDiffuse;			// +0x168

public:
	W3DDynamicLight();								// 0x0006DD6F
	virtual ~W3DDynamicLight();						// 1: 0x0006DD51, deleting 0x0006DE21
	virtual void On_Frame_Update(void);						// 13: 0x0006DE3D

	void setEnabled(Bool enabled) { m_enabled = enabled; m_decayRange = false; m_decayFrameCount = 0; m_decayColor = false; m_increaseFrameCount = 0; }
	Bool isEnabled(void) { return m_enabled; }

	// 0 frameIncreaseTime means it starts out full size/intensity, 0 decay time means it lasts forever.
	void setFrameFade(UnsignedInt frameIncreaseTime, UnsignedInt decayFrameTime);	// 0x0006DF81
	void setDecayRange(void) { m_decayRange = true; }
	void setDecayColor(void) { m_decayColor = true; }
	// Cull returns true if the terrain vertex at x,y is outside of the light's influence.
	Bool cull(Int x, Int y) { return (x < m_minX || y < m_minY || x > m_maxX || y > m_maxY); }
};
// SIZE 0x174
typedef char W3DDynamicLightSize[(sizeof(W3DDynamicLight) == 0x174) ? 1 : -1];

#endif
