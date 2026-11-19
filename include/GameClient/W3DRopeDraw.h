// W3DRopeDraw.h: the rope draw module (a chain of line segments that sways and slides).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts (retail game.dat 1.06):
// - Constructor 0x000CA752 calls DrawModule's (0x000B19A1), stores table 0x007CBD60 at +0 and the
//   RopeDrawInterface table 0x007CBD50 at +0xC (after the interface's own 0x007E2B78: three
//   __purecall slots), constructs the segment vector at +0x10 and sets the members below.
// - Table 0x007CBD60 (62 slots) overrides 0 (deleting destructor 0x000CA9D5), 2 GetSnapshotName
//   0x000CA83B, 3 xfer 0x000CA916, 4 the module key 0x000CA7F6, 11 doDrawModule 0x000CACA8, the
//   pure 12-14, 35, 38 and 39 with the folded empty bodies, and 45/46 getRopeDrawInterface (the fold
//   0x000CA841, `this ? this + 0xC : 0`). Slot 1 is 0x0058B03E as for every W3D draw module.
// - initRopeParms 0x000CAC25 (this = the interface) writes +0x1C..+0x30 and +0x40..+0x50 at these
//   offsets; setRopeSpeed 0x000CA72E writes +0x34/+0x38/+0x3C, setRopeCurLen (the fold 0x00101CDA) +0x1C;
//   xfer 0x000CA916 transfers +0x1C..+0x50 in member order (RGBColor at +0x28).
// Donor: Zero Hour's W3DRopeDraw.h has the same members in the same order; SIZE 0x54.
#pragma once
#ifndef BFME2_W3DROPEDRAW_H
#define BFME2_W3DROPEDRAW_H

#include <stddef.h>
#include <vector>
#include "Common/GameType.h"
#include "Common/RGBColor.h"
#include "GameClient/DrawModule.h"

class Line3DClass;
class Matrix3D;
class Xfer;
class Coord3D;

class RopeDrawInterface
{
public:
	virtual void initRopeParms(Real length, Real width, const RGBColor &color, Real wobbleLen, Real wobbleAmp, Real wobbleRate) = 0;	// 0
	virtual void setRopeCurLen(Real length) = 0;										// 1
	virtual void setRopeSpeed(Real curSpeed, Real maxSpeed, Real accel) = 0;			// 2
};

class W3DRopeDraw : public DrawModule, public RopeDrawInterface
{
public:
	W3DRopeDraw(Thing *thing, const ModuleData *moduleData);				// 0x000CA752
	virtual ~W3DRopeDraw();													// 0x000CA8BC

	virtual NameKeyType getModuleNameKey() const;							// 0x000CA7F6
	virtual void doDrawModule(const Matrix3D *transformMtx);				// 0x000CACA8
	virtual void setShadowsEnabled(Bool enable) {}							// slot 12, folded `ret 4`
	virtual void releaseShadows() {}										// slot 13, folded `ret`
	virtual void allocateShadows() {}										// slot 14, folded `ret`
	virtual void setFullyObscuredByShroud(Bool fullyObscured) {}			// slot 35, folded `ret 4`
	virtual void reactToTransformChange(const Matrix3D *oldMtx, const Coord3D *oldPos, Real oldAngle) {}	// slot 38, folded `ret 0Ch`
	virtual void reactToGeometryChange() {}									// slot 39, folded `ret`

	virtual void initRopeParms(Real length, Real width, const RGBColor &color, Real wobbleLen, Real wobbleAmp, Real wobbleRate);	// 0x000CAC25
	virtual void setRopeCurLen(Real length) { m_curLen = length; }			// the fold 0x00101CDA
	virtual void setRopeSpeed(Real curSpeed, Real maxSpeed, Real accel);	// 0x000CA72E

	virtual RopeDrawInterface *getRopeDrawInterface() { return this; }				// slot 46, the fold 0x000CA841
	virtual const RopeDrawInterface *getRopeDrawInterface() const { return this; }	// slot 45, the fold 0x000CA841

protected:
	virtual void loadPostProcess() { DrawModule::loadPostProcess(); }	// slot 1: retail table 0x007CBD60 holds 0x0058B03E (`jmp 0x000B3FD0`, DrawModule's empty body), ICF-folded with the module bases' (h05)
	virtual const char *GetSnapshotName() const;							// 0x000CA83B
	virtual void xfer(Xfer *xfer);											// 0x000CA916

public:
	struct SegInfo
	{
		Line3DClass *line;
		Line3DClass *softLine;
		Real wobbleAxisX;
		Real wobbleAxisY;
	};

	std::vector<SegInfo> m_segments;										// 0x10
	Real m_curLen;															// 0x1C
	Real m_maxLen;															// 0x20
	Real m_width;															// 0x24
	RGBColor m_color;														// 0x28
	Real m_curSpeed;														// 0x34
	Real m_maxSpeed;														// 0x38
	Real m_accel;															// 0x3C
	Real m_wobbleLen;														// 0x40
	Real m_wobbleAmp;														// 0x44
	Real m_wobbleRate;														// 0x48
	Real m_curWobblePhase;													// 0x4C
	Real m_curZOffset;														// 0x50

private:
	void tossSegments();													// 0x000CA84D
	void buildSegments();													// 0x000CA9F1
};
// SIZE 0x54
typedef char W3DRopeDraw_size[(sizeof(W3DRopeDraw) == 0x54) ? 1 : -1];
typedef char W3DRopeDraw_off_m_segments[(offsetof(W3DRopeDraw, m_segments) == 0x10) ? 1 : -1];
typedef char W3DRopeDraw_off_m_color[(offsetof(W3DRopeDraw, m_color) == 0x28) ? 1 : -1];
typedef char W3DRopeDraw_off_m_curZOffset[(offsetof(W3DRopeDraw, m_curZOffset) == 0x50) ? 1 : -1];

#endif
