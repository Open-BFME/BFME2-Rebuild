// W3DProjectileStreamDraw.h: the projectile stream draw module (a trail of line objects behind a
// projectile) and its INI data.
// Target facts (retail game.dat 1.06):
// - Constructor 0x000D1370 calls DrawModule's, stores table 0x007CE010, loads the texture the data
//   names (+0x0C, a texture handle) and clears the twenty line slots (+0x10..+0x5F) and the count
//   (+0x60). SIZE 0x64.
// - The table overrides 2 (0x000D1401, the class name), 3 (xfer 0x000D11A6), 4 (module key
//   0x000D140A), 11 (doDrawModule 0x000D14F9), 12-14, 38 and 39 (empty bodies) and 35
//   (setFullyObscuredByShroud 0x000D1124: takes every line out of the scene or puts it back).
// - W3DProjectileStreamDrawModuleData (table 0x007CDF18, constructor 0x000D11D2, destructor
//   0x000D1226, buildFieldParse 0x000D125C -> table 0x007CDFB0) is declared in
//   GameLogic/Module/W3DProjectileStreamDrawModuleData.h.
// Donor facts: Zero Hour's W3DProjectileStreamDraw has m_texture, m_allMyLines[MAX_PROJECTILE_STREAM]
//   and m_linesValid with the same INI tokens; BFME 2 keeps the line pointers and their count.
#pragma once
#ifndef BFME2_W3DPROJECTILESTREAMDRAW_H
#define BFME2_W3DPROJECTILESTREAMDRAW_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "GameClient/DrawModule.h"
#include "GameLogic/Module/W3DProjectileStreamDrawModuleData.h"

class Matrix3D;
class RenderObjClass;
class Xfer;
class Coord3D;

class W3DProjectileStreamDraw : public DrawModule
{
public:
	W3DProjectileStreamDraw(Thing *thing, const ModuleData *moduleData);		// 0x000D1370

	virtual NameKeyType getModuleNameKey() const;								// 0x000D140A
	virtual void doDrawModule(const Matrix3D *transformMtx);					// 0x000D14F9
	virtual void setShadowsEnabled(Bool enable) {}								// slot 12, folded `ret 4`
	virtual void releaseShadows() {}											// slot 13, folded `ret`
	virtual void allocateShadows() {}											// slot 14, folded `ret`
	virtual void setFullyObscuredByShroud(Bool fullyObscured);					// 0x000D1124
	virtual void reactToTransformChange(const Matrix3D *oldMtx, const Coord3D *oldPos, Real oldAngle) {}	// slot 38, folded `ret 0xC`
	virtual void reactToGeometryChange() {}										// slot 39, folded `ret`

protected:
	virtual ~W3DProjectileStreamDraw();										// 0x000D146B
	virtual void loadPostProcess() { DrawModule::loadPostProcess(); }	// slot 1: retail table 0x007CE010 holds 0x0058B03E (`jmp 0x000B3FD0`, DrawModule's empty body), ICF-folded with the module bases' (h05)
	virtual const char *GetSnapshotName() const;								// 0x000D1401 (the map names it W3DProjectileStreamDraw::name)
	virtual void xfer(Xfer *xfer);												// 0x000D11A6

public:
	UnsignedByte m_texture[0x4];												// 0x0C a texture handle (the constructor loads it from the data's Texture)
	RenderObjClass *m_lines[20];												// 0x10
	Int m_lineCount;															// 0x60
};
// SIZE 0x64
typedef char W3DProjectileStreamDraw_size[(sizeof(W3DProjectileStreamDraw) == 0x64) ? 1 : -1];
typedef char W3DProjectileStreamDraw_off_m_lines[(offsetof(W3DProjectileStreamDraw, m_lines) == 0x10) ? 1 : -1];
typedef char W3DProjectileStreamDraw_off_m_lineCount[(offsetof(W3DProjectileStreamDraw, m_lineCount) == 0x60) ? 1 : -1];

#endif
