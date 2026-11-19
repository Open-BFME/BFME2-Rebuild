// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DShadowTexture: the texture-and-bounds record the projected shadows share per model (Zero Hour's W3DShadowTexture in
// W3DProjectedShadow.cpp). Target facts (retail game.dat): the constructor 0x00108BB4 stores the RefCountClass table VA 0x007CF9BC at +0 (count 1
// at +4) and the HashableClass table VA 0x007CF9B4 at +8 (NextHash +0xC); the hash table's key is the name buffer at +0x10 (Get_Key 0x005C4AD5 returns it);
// +0x30 is the RefCountPtr<TextureClass> (the destructor 0x00108CC4 releases it through 0x0061ED10), +0x34 the light position the shadow was last built for
// (setLightPosHistory 0x001085D7 copies three dwords), +0x40 a Matrix3x3 (three row constructors), +0x64 an AABoxClass (center, extent), +0x7C a SphereClass
// (getBoundingSphere 0x001085EF returns its address), +0x8C two Vector3 texture axes (setDecalUVAxis 0x00108603 copies both; the constructor seeds them).
// SIZE 0xA4.
#pragma once
#ifndef BFME2_W3DSHADOWTEXTURE_H
#define BFME2_W3DSHADOWTEXTURE_H

#include "WWLib/RefCountClass.h"
#include "WWLib/HashableClass.h"
#include "WWMath/Vector3.h"
#include "WWMath/Matrix3x3.h"
#include "WWMath/AABoxClass.h"
#include "WWMath/SphereClass.h"

class W3DShadowTexture : public RefCountClass, public HashableClass
{
public:
	W3DShadowTexture();														// 0x00108BB4
	virtual const char *Get_Key() { return m_namebuf; }						// 0x005C4AD5 (folded)

	void setLightPosHistory(Vector3 &pos);									// 0x001085D7
	Vector3 &getLightPosHistory() { return m_lastLightPosition; }
	SphereClass &getBoundingSphere();										// 0x001085EF
	void setDecalUVAxis(Vector3 &u, Vector3 &vec);							// 0x00108603

	char m_namebuf[32];														// +0x10 (two W3D_NAME_LEN)
	void *m_texture;														// +0x30 RefCountPtr<TextureClass>
	Vector3 m_lastLightPosition;											// +0x34
	Matrix3x3 m_lastObjectOrientation;										// +0x40
	AABoxClass m_areaEffectBox;												// +0x64
	SphereClass m_areaEffectSphere;											// +0x7C
	Vector3 m_shadowUV[2];													// +0x8C
};
typedef char W3DShadowTextureSize[(sizeof(W3DShadowTexture) == 0xA4) ? 1 : -1];

#endif
