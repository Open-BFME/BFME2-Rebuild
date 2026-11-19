// GlowMaterial.h: the glow material that BuffInstance embeds at +0x30 (size 0x14).
// Layout, proven by the rows below: the constructor 0x0036247C writes the vptr at +0 and zeroes
// +4, +8, +0xC and +0x10; IsActive 0x00362499 reads the two pointers +8 and +4; DoXfer 0x003624C4
// transfers the version pair then +0xC and +0x10 as unsigned ints.
// Still a Draft: the target at +4 (LoadPostProcess 0x003624FE writes +0x40 of it) has no type yet,
// and the destructor 0x00362CF2 is not in this file.
#pragma once
#ifndef BFME2_GLOW_MATERIAL_H
#define BFME2_GLOW_MATERIAL_H

#include <stddef.h>
#include "Common/GameType.h"

class Xfer;
class W3DGlowOutlineMaterialPassClass;

class GlowMaterial
{
	friend class BuffInstance;								// BuffInstance::DoXfer 0x003628D3 calls it directly
public:
	GlowMaterial();											// 0x0036247C
	virtual ~GlowMaterial();
	unsigned char IsActive();								// 0x00362499

protected:
	virtual void LoadPostProcess();							// 0x003624FE (slot 1)
	virtual const char *GetSnapshotName() const;			// 0x00362493 (slot 2)
	virtual void DoXfer(Xfer *xfer);						// 0x003624C4 (slot 3)

public:
	W3DGlowOutlineMaterialPassClass *m_04;					// +0x04 per-buff material (state 0 at creation); its +0x40 state word receives m_10 in LoadPostProcess
	W3DGlowOutlineMaterialPassClass *m_08;					// +0x08 shared material (state 1 at creation)
	UnsignedInt m_0C;										// +0x0C (DoXfer)
	UnsignedInt m_10;										// +0x10 (DoXfer)
};
// SIZE 0x14
typedef char GlowMaterial_size[(sizeof(GlowMaterial) == 0x14) ? 1 : -1];

#endif
