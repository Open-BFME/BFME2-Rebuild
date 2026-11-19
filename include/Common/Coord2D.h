// Class-key class: retail operator==(Coord2D&) 0x0060DB1A uses AAVCoord2D;
// old canonical Lib/Coord2D.h agrees. The 8-byte layout is unchanged.
// Coord2D.h: SAGE two-float coordinate. SIZE 0x8.
#pragma once
#ifndef BFME2_COORD2D_H
#define BFME2_COORD2D_H

class Coord2D
{
public:
	// Retail's Coord2D is a non-trivial class: user-written empty default constructor
	// (out-of-line copy 0x0047A6A9), memberwise copy constructor (0x005CFBC2), (x, y)
	// constructor (0x00003F3A) and an empty destructor (0x000B3FD0, the folded bare ret).
	// That is why a by-value Coord2D argument is copied through a stack temporary whose
	// address is stored into the argument slot (ControlBarSchemeManager::drawForeground
	// 0x0031FA9A) and why one in a by-value return stays on the unwind path.
	Coord2D() {}
	Coord2D(const Coord2D &other) : x(other.x), y(other.y) {}
	Coord2D(float xValue, float yValue) : x(xValue), y(yValue) {}
	~Coord2D() {}

	float Normalize();	// 0x000059D3 (returns the length; no zero test)
	void normalize();	// 0x0000378A (AISkirmishPlayer::computeSuperweaponTarget 0x004EFDDC; h07)
	float length() const;	// 0x00003755 (?length@Coord2D@@QBEMXZ, out of line: Coord2D::toAngle 0x00005923 calls it)
	float toAngle() const;	// 0x00005923 (StructureToppleUpdate::beginStructureTopple 0x004A5C8C, applyCrushingDamage 0x004A5EAC)

	float x;	// 0x0
	float y;	// 0x4
};
// SIZE 0x8

#endif
