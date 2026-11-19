// DrawableInterpolationHistory.h: the 0x90-byte block of a Drawable that keeps the object's last
// transforms and positions for the client's frame interpolation (Drawable +0x3AC).
// Target facts: its constructor 0x002701F4 (45 bytes, map name ??0Rva002701F4@@QAE@XZ, a COMDAT)
// runs the EH vector constructor iterator over two 0x30-byte Matrix3D (element constructor 0x000423C5)
// and then over four 0xC-byte Coord3D at +0x60 (constructor fold 0x0047A6A9, destructor 0x000B3FD0), so
// the layout is Matrix3D[2] then Coord3D[4]: 0x60 + 0x30 = 0x90. Drawable::rva002747F9 0x002747F9 shifts
// the object's 12-word transform (+0x08..+0x37) through the two matrices and its position (+0x38) through
// the four positions (Drawable +0x430, +0x424, +0x418, +0x40C); Drawable::getPosition 0x002763E6 hands
// the four positions and TheGameEngine's clientFrameRatio to D3DXVec3CatmullRom.
// The class name is a provisional role name (the map has only the placeholder class Rva002701F4).
#pragma once
#ifndef BFME2_DRAWABLEINTERPOLATIONHISTORY_H
#define BFME2_DRAWABLEINTERPOLATIONHISTORY_H

#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "WWMath/Matrix3D.h"

class DrawableInterpolationHistory
{
public:
	DrawableInterpolationHistory() {}	// 0x002701F4 (out of line in retail)

	Matrix3D m_xform[2];		// +0x00, +0x30 the previous transforms
	Coord3D m_position[4];		// +0x60, +0x6C, +0x78, +0x84 the previous positions, oldest first (Drawable +0x40C..+0x430)
};
typedef char DrawableInterpolationHistorySize[(sizeof(DrawableInterpolationHistory) == 0x90) ? 1 : -1];

#endif
