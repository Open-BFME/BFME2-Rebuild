// RadiusDecal.h: one decal a shroud-clearing module keeps. SIZE 0x10.
// Target facts: the array in DynamicShroudClearingRangeUpdate starts at +0x50 and
// killGridDecals 0x0048B03E steps it 0x10 at a time; clear is 0x00330DBA,
// setPosition 0x00330DFD, setOpacity is 0x00330DDB and the ctor/dtor are the
// 0x00330DA4/0x00330DD6 pair the behaviour constructor uses.
//
// The decal at +0x04 is a Shadow (see WW3D2/Shadow.h): createRadiusDecal
// 0x00331459 stores the manager's slot-2 result there and calls
// Shadow::setColor on it, clear calls its slot 2 to release it, and setPosition
// copies three dwords into its +0x08 Coord3D.
#pragma once
#ifndef BFME2_RADIUSDECAL_H
#define BFME2_RADIUSDECAL_H

#include "Common/GameType.h"

class Coord3D;
class Shadow;
class Xfer;

class RadiusDecal
{
public:
	RadiusDecal();
	~RadiusDecal();

	void clear();
	void update();					// 0x00330F98
	void setPosition(const Coord3D &pos);
	void setOpacity(Real opacity);
	void xferRadiusDecal(Xfer *xfer);	// 0x00330F7D (Zero Hour's name: RadiusDecalTemplate::xferRadiusDecalTemplate 0x00330EE0 sits just before it)

	const void *m_template;			// 0x0
	Shadow *m_decal;				// 0x4
	Byte m_empty;					// 0x8
	Byte m_pad0x09[3];				// 0x9
	Real m_unk0x0C;					// 0xC
};
// SIZE 0x10

#endif
