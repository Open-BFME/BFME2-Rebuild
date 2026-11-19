// W3DDebrisDraw.h: the debris draw module (a DrawModule that plays a model through its fall).
// Target facts (retail game.dat 1.06):
// - Constructor 0x000B1C41 calls DrawModule's (0x000B19A1), stores the primary table
//   0x007C97A8 at +0, the debris-interface table 0x007C97A0 at +0x0C (after the interface's
//   own pure table 0x00C6FFFC), zeroes the strings at +0x10, +0x18..+0x20, sets +0x14 to -1,
//   clears the render object +0x24, the three animations +0x28..+0x30 and +0x34..+0x44.
//   Allocated 0x48 bytes by the factory.
// - The primary table overrides slots 11-14 (doDrawModule, the shadow pair, one empty), 35
//   (fully-obscured flag), 38 (reactToTransformChange) and 39 (empty), and the two debris
//   interface getters 43/44 return this+0x0C. The shadow flag setters 0x000B19C3/0x000B19D4
//   write Shadow +4 and +5 when the shadow at +0x44 exists.
// - xfer 0x000B1DC3 transfers the three names, the colour, +0x38, +0x3C and the stop flag, and
//   on load re-issues setModelName and setAnimNames through the interface at +0x0C.
// - The destructor 0x000B1CE2 releases the shadow (+0x44), removes the render object (+0x24)
//   from the scene and drops the three animation references.
// Donor facts: Zero Hour's W3DDebrisDraw names the animation slots INITIAL/FLYING/FINAL and
// the members m_modelName, m_anims, m_fxFinal, m_state, m_frames and m_finalStop.
#pragma once
#ifndef BFME2_W3DDEBRISDRAW_H
#define BFME2_W3DDEBRISDRAW_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "GameClient/DrawModule.h"

class FXList;
class HAnimClass;
class RenderObjClass;
class Shadow;
class Xfer;
class Matrix3D;
class Coord3D;
enum ShadowType;	// values not recovered

// The debris draw interface (W3DDebrisDraw's +0x0C base). Pure table 0x00C6FFFC.
class DebrisDrawInterface
{
public:
	virtual void setModelName(AsciiString name, Int color, ShadowType shadowType) = 0;	// 0 (W3DDebrisDraw 0x000B1EEB)
	virtual void setAnimNames(AsciiString initial, AsciiString flying, AsciiString final,
		const FXList *finalFX) = 0;													// 1 (W3DDebrisDraw 0x000B207F)
};

class W3DDebrisDraw : public DrawModule, public DebrisDrawInterface
{
public:
	W3DDebrisDraw(Thing *thing, const ModuleData *moduleData);				// 0x000B1C41

	virtual NameKeyType getModuleNameKey() const;							// 0x000B1C97
	virtual void doDrawModule(const Matrix3D *transformMtx);				// 0x000B1A37
	// Slots 12 and 35: bodies 0x000B19C3 / 0x000B19D4 (`if (m_shadow) m_shadow->m_enabled / m_invisible = flag`) are owned by the map's
	// placeholder classes Gen_00750550 / Gen_00750570 and wait for their rename; the declarations keep the table right.
	virtual void setShadowsEnabled(Bool enable);							// 0x000B19C3 (map: Gen_00750550::bfmeSet)
	virtual void releaseShadows() {}										// slot 13, folded `ret`
	virtual void allocateShadows() {}										// slot 14, folded `ret`
	virtual void setFullyObscuredByShroud(Bool fullyObscured);				// 0x000B19D4 (map: Gen_00750570::bfmeSet)
	virtual void reactToTransformChange(const Matrix3D *oldMtx, const Coord3D *oldPos, Real oldAngle);	// 0x000B1A14
	virtual void reactToGeometryChange() {}									// slot 39, folded `ret`
	virtual DebrisDrawInterface *getDebrisDrawInterface() { return this; }	// slot 44 (0x000CA841)
	virtual const DebrisDrawInterface *getDebrisDrawInterface() const { return this; }	// slot 43
	virtual void setModelName(AsciiString name, Int color, ShadowType shadowType);	// 0x000B1EEB
	virtual void setAnimNames(AsciiString initial, AsciiString flying, AsciiString final,
		const FXList *finalFX);												// 0x000B207F

protected:
	virtual ~W3DDebrisDraw();												// 0x000B1CE2
	virtual void loadPostProcess() { DrawModule::loadPostProcess(); }	// slot 1: retail table 0x007C97A8 holds 0x0058B03E (`jmp 0x000B3FD0`, DrawModule's empty body), ICF-folded with the module bases' (h05)
	virtual const char *GetSnapshotName() const;							// 0x000B1CDC
	virtual void xfer(Xfer *xfer);											// 0x000B1DC3

public:
	enum AnimStateType { INITIAL, FLYING, FINAL, STATECOUNT };

	AsciiString m_modelName;												// 0x10
	Int m_modelColor;														// 0x14 = -1
	AsciiString m_animInitial;												// 0x18
	AsciiString m_animFlying;												// 0x1C
	AsciiString m_animFinal;												// 0x20
	RenderObjClass *m_renderObject;											// 0x24
	HAnimClass *m_anims[STATECOUNT];										// 0x28
	const FXList *m_fxFinal;												// 0x34
	Int m_state;															// 0x38
	Int m_frames;															// 0x3C
	Bool m_finalStop;														// 0x40
	Shadow *m_shadow;														// 0x44
};
// SIZE 0x48
typedef char W3DDebrisDraw_size[(sizeof(W3DDebrisDraw) == 0x48) ? 1 : -1];
typedef char W3DDebrisDraw_off_m_renderObject[(offsetof(W3DDebrisDraw, m_renderObject) == 0x24) ? 1 : -1];
typedef char W3DDebrisDraw_off_m_shadow[(offsetof(W3DDebrisDraw, m_shadow) == 0x44) ? 1 : -1];

#endif
