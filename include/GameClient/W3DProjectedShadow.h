// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DProjectedShadow.h: projected shadow and dual decal shadow instances.
#pragma once
#ifndef BFME2_W3DPROJECTEDSHADOW_H
#define BFME2_W3DPROJECTEDSHADOW_H

#include <stddef.h>
#include "WW3D2/Shadow.h"
#include "WWMath/Vector3.h"

class RenderObjClass;
class W3DShadowTexture;

class W3DProjectedShadow : public Shadow
{
public:
	W3DProjectedShadow();
	virtual ~W3DProjectedShadow();
	virtual void release();

	void init();
	void update();
	void updateTexture(Vector3 &lightPos);

	W3DShadowTexture *m_shadowTexture[2];
	RenderObjClass *m_robj;
	Bool m_allowWorldAlign;
	Real m_offsetX;
	Real m_offsetY;
	Int m_unk0x80;
	Bool m_isDirectional;
	Bool m_allowUpdates;
	Vector3 m_lastLightPosition;
	UnsignedInt m_unk0x94[16];
	UnsignedInt m_unk0xD4[16];
	W3DProjectedShadow *m_next;
};

class W3DDualDecalShadow : public Shadow
{
public:
	W3DDualDecalShadow();
	virtual ~W3DDualDecalShadow();

	UnsignedInt m_unk0x68;
	Real m_decalOffsetX;
	Real m_decalOffsetY;
};

class W3DDualProjectedShadow : public ShadowBase
{
public:
	W3DDualProjectedShadow();
	virtual ~W3DDualProjectedShadow();
	virtual void release();

	void getDecalOffset(Int index, Real *offsetX, Real *offsetY) const;

	W3DDualDecalShadow *m_shadows[2];
	RenderObjClass *m_robj;
	W3DDualProjectedShadow *m_next;
};

typedef char W3DProjectedShadowSize[(sizeof(W3DProjectedShadow) == 0x118) ? 1 : -1];
typedef char W3DProjectedShadowNextOffset[(offsetof(W3DProjectedShadow, m_next) == 0x114) ? 1 : -1];
typedef char W3DDualDecalShadowSize[(sizeof(W3DDualDecalShadow) == 0x74) ? 1 : -1];
typedef char W3DDualDecalShadowOffsetX[(offsetof(W3DDualDecalShadow, m_decalOffsetX) == 0x6C) ? 1 : -1];
typedef char W3DDualProjectedShadowSize[(sizeof(W3DDualProjectedShadow) == 0x68) ? 1 : -1];
typedef char W3DDualProjectedShadowPairOffset[(offsetof(W3DDualProjectedShadow, m_shadows) == 0x58) ? 1 : -1];

#endif
