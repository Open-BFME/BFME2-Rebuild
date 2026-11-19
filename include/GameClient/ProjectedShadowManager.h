// ProjectedShadowManager: abstract decal factory, SIZE 4, no subsystem base.
// Native ctor 0x00108657 installs six-slot table RVA 0x007CF994. The
// concrete W3DProjectedShadowManager ctor 0x00108D33 replaces it with
// RVA 0x007CF9C4 (seven slots). W3DShadowManager ctor 0x0009A710 publishes
// this same object at VA 0x00DEC2D4 and 0x00DEC2D8.
// addDecal's role is established by 0x0010C065: resolve the descriptor's
// textures and allocate a projected Shadow; caller 0x00331459 uses slot 2.
#pragma once
#ifndef BFME2_PROJECTED_SHADOW_MANAGER_H
#define BFME2_PROJECTED_SHADOW_MANAGER_H
#include "WW3D2/Shadow.h"
class RenderObjClass;
class ProjectedShadowManager
{
public:
    virtual ~ProjectedShadowManager(); // 0, 0x00108475
    virtual void updateDecalFades() = 0; // 1, role inferred, 0x0010BE72
    virtual Shadow *addDecal(Shadow::ShadowTypeInfo *info) = 0; // 2
    virtual Shadow *addObjectDecal(RenderObjClass *object,
        Shadow::ShadowTypeInfo *info, bool, bool) = 0; // 3, role inferred
    virtual void forward(UnsignedInt first, UnsignedInt second, UnsignedInt third) = 0; // 4, concrete 0x00108790 (the ledger's spelling: UAEXIII)
    virtual UnsignedInt forwardIf(UnsignedInt first, void *second, UnsignedInt third,
        UnsignedInt fourth) = 0; // 5, concrete 0x001087A6 (the ledger's spelling: UAEIIPAXII)
};
typedef char ProjectedShadowManagerSize[(sizeof(ProjectedShadowManager) == 4) ? 1 : -1];
extern ProjectedShadowManager *TheProjectedShadowManager; // VA 0x00DEC2D4
#include "WWMath/Vector3.h"
class Shadow;
class W3DShadowManager
{
public:
	void removeShadow(Shadow *shadow);	// 0x000518E0 (map: the stdcall placeholder Rva000518E0Thunk; ecx is the manager)
	Vector3 &getLightPosWorld(int lightIndex);	// 0x0009A497: &m_lightPosWorld[lightIndex]

protected:
	unsigned char m_unk00[0x0C];		// +0x00 (a flag byte at +0 is tested by 0x0009A348 / 0x0009A361)
	Vector3 m_lightPosWorld[1];		// +0x0C one entry per light (the count is not read by the rows written)
};
extern W3DShadowManager *TheW3DShadowManager; // VA 0x00DE5DFC (W3DDebrisDraw dtor 0x000B1CE2 tests it)
#endif
