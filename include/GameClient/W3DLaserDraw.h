// W3DLaserDraw.h: the laser draw module (a ring of segmented lines that follows a LaserUpdate).
// Target facts (retail game.dat 1.06):
// - Constructor 0x000C9D38 calls DrawModule's, stores the laser-interface table 0x007CB9DC at
//   +0x0C (after the interface's own pure table 0x00C1C780) and the primary table 0x007CB9E0 at
//   +0, then builds the texture list (+0x14, a vector of asset references) from the data's
//   Texture strings, copies the data's 0x28-byte Envelope to +0x34, and allocates
//   NumBeams * Segments SegmentedLineClass pointers at +0x10. Allocated 0x5C bytes.
// - Table 0x007CB9E0 overrides slots 0 (destructor 0x000C9B1D), 1 (loadPostProcess 0x000C92E8),
//   2 (GetSnapshotName 0x000C9C11), 3 (xfer 0x000C92A0), 4 (module key 0x000C9BCC), 11
//   (doDrawModule 0x000C9360), 12-14, 35, 38 and 39 (empty), 40 (isLaser, true) and the two
//   laser-interface getters 47/48 (this+0x0C). The interface table 0x007CB9DC holds
//   getLaserTemplateWidth (0x000C9353: half of the data's OuterBeamWidth).
// - xfer: Version1, then DrawModule::xfer, then the floats at +0x2C and +0x30, the int at +0x28
//   and the float at +0x20. loadPostProcess calls DrawModule's (empty) and sets +0x24.
// Donor facts: Zero Hour's W3DLaserDraw holds m_line3D, m_texture, m_selfDirtyFrame, m_length
//   and the data's beam, colour and tiling fields; BFME 2's draws NumBeams x Segments lines.
#pragma once
#ifndef BFME2_W3DLASERDRAW_H
#define BFME2_W3DLASERDRAW_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameClient/DrawModule.h"

class Matrix3D;
class SegmentedLineClass;
class Xfer;
class Coord3D;

class W3DLaserDraw : public DrawModule, public LaserDrawInterface
{
public:
	W3DLaserDraw(Thing *thing, const ModuleData *moduleData);					// 0x000C9D38

	virtual NameKeyType getModuleNameKey() const;								// 0x000C9BCC
	virtual void doDrawModule(const Matrix3D *transformMtx);					// 0x000C9360
	virtual void setShadowsEnabled(Bool enable) {}								// slot 12, folded `ret 4`
	virtual void releaseShadows() {}											// slot 13, folded `ret`
	virtual void allocateShadows() {}											// slot 14, folded `ret`
	virtual void setFullyObscuredByShroud(Bool fullyObscured) {}				// slot 35, folded `ret 4`
	virtual void reactToTransformChange(const Matrix3D *oldMtx, const Coord3D *oldPos, Real oldAngle) {}	// slot 38, folded `ret 0xC`
	virtual void reactToGeometryChange() {}										// slot 39, folded `ret`
	virtual Bool isLaser() const { return true; }								// slot 40, folded `mov al,1`
	virtual LaserDrawInterface *getLaserDrawInterface() { return this; }		// slot 48 (0x000CA841)
	virtual const LaserDrawInterface *getLaserDrawInterface() const { return this; }	// slot 47
	virtual Real getLaserTemplateWidth() const;								// 0x000C9353 (interface slot 0)

protected:
	virtual ~W3DLaserDraw();													// 0x000C9B1D
	virtual void loadPostProcess();											// 0x000C92E8
	virtual const char *GetSnapshotName() const;								// 0x000C9C11 (map: the placeholder Rva000C9C11Named::name)
	virtual void xfer(Xfer *xfer);												// 0x000C92A0

public:
	SegmentedLineClass **m_lines;												// 0x10 NumBeams * Segments pointers (new[] in the constructor)
	UnsignedByte m_unk0x14[0xC];												// 0x14 a vector of asset references (the textures); element class not recovered
	Real m_unk0x20;															// 0x20 xfer float; the constructor stores 1.0 (or a texture's width over height)
	Bool m_needsRefresh;														// 0x24 loadPostProcess sets it; the constructor stores true
	Int m_unk0x28;															// 0x28 xfer int, 0
	Real m_unk0x2C;															// 0x2C xfer float, 1.0
	Real m_unk0x30;															// 0x30 xfer float; the constructor stores the data's FanWidth
	UnsignedByte m_envelope[0x28];												// 0x34 copy of the data's Envelope
};
// SIZE 0x5C
typedef char W3DLaserDraw_size[(sizeof(W3DLaserDraw) == 0x5C) ? 1 : -1];
typedef char W3DLaserDraw_off_m_lines[(offsetof(W3DLaserDraw, m_lines) == 0x10) ? 1 : -1];
typedef char W3DLaserDraw_off_m_envelope[(offsetof(W3DLaserDraw, m_envelope) == 0x34) ? 1 : -1];

#endif
