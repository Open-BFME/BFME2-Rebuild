// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DProjectedShadowManager: the concrete decal factory (Zero Hour's W3DProjectedShadowManager). Target facts: the constructor 0x00108D33 stores the
// seven-slot table VA 0x007CF9C4 and clears +0x04..+0x20, +0x24C and +0x250..+0x270; W3DShadowManager's constructor 0x0009A710 allocates 0x274 bytes for
// it. Slots: 0 the deleting destructor 0x0010C825, 1 updateDecalFades 0x0010BE72, 2 addDecal 0x0010C065, 3 addObjectDecal 0x0010C145, 4 forward 0x00108790,
// 5 forwardIf 0x001087A6, 6 0x0010C2D5 (the four-argument creator both forwarders call). The members are not recovered beyond that.
#pragma once
#ifndef BFME2_W3DPROJECTEDSHADOWMANAGER_H
#define BFME2_W3DPROJECTEDSHADOWMANAGER_H

#include <stddef.h>
#include "GameClient/ProjectedShadowManager.h"

class W3DProjectedShadow;
class DX8VertexBufferClass;
class DX8IndexBufferClass;

class W3DProjectedShadowManager : public ProjectedShadowManager
{
public:
	virtual ~W3DProjectedShadowManager();															// slot 0, 0x0010C825
	virtual void updateDecalFades();																// slot 1, 0x0010BE72
	virtual Shadow *addDecal(Shadow::ShadowTypeInfo *info);											// slot 2, 0x0010C065
	virtual Shadow *addObjectDecal(RenderObjClass *object, Shadow::ShadowTypeInfo *info, bool, bool);	// slot 3, 0x0010C145
	virtual void forward(UnsignedInt first, UnsignedInt second, UnsignedInt third);					// slot 4, 0x00108790
	virtual UnsignedInt forwardIf(UnsignedInt first, void *second, UnsignedInt third, UnsignedInt fourth);	// slot 5, 0x001087A6
	virtual W3DProjectedShadow *rva0010C2D5(int, RenderObjClass *, Shadow::ShadowTypeInfo *, Shadow::ShadowTypeInfo *);	// slot 6, 0x0010C2D5

	Bool ReAcquireResources();
	void ReleaseResources();

	unsigned char m_unk0x04[0x250];
	DX8VertexBufferClass *m_shadowVertexBuffer;
	DX8IndexBufferClass *m_shadowIndexBuffer;
	Int m_shadowVertexBufferOffset;
	Int m_shadowVertexBufferBase;
	Int m_shadowIndexBufferOffset;
	Int m_shadowIndexBufferBase;
	Int m_numShadowVertices;
	Int m_numShadowIndices;
};
typedef char W3DProjectedShadowManagerSize[(sizeof(W3DProjectedShadowManager) == 0x274) ? 1 : -1];
typedef char W3DProjectedShadowManagerVbOffset[(offsetof(W3DProjectedShadowManager, m_shadowVertexBuffer) == 0x254) ? 1 : -1];
typedef char W3DProjectedShadowManagerIndicesOffset[(offsetof(W3DProjectedShadowManager, m_numShadowIndices) == 0x270) ? 1 : -1];

#endif
