// RadiusDecalTemplate.h: the INI template a radius decal is stamped from. SIZE 0x34.
//
// Target facts: DynamicShroudClearingRangeUpdateModuleData holds it at +0x28 and
// its constructor 0x0048B5C6 builds it through 0x00330E5D while the destructor
// 0x0048B634 tears it down through 0x000B6CF1; createRadiusDecal is 0x00331459
// (__thiscall, pos, radius, owning player, out decal).
//
// Member layout read from the constructor's stores (0x00330E5D) and the field
// table at 0x00C0DC20: two AsciiStrings at +0x00/+0x04 (both copied from
// AsciiString::TheEmptyString), a 32-bit style flag word at +0x08 (default 0x20,
// parsed through parseBitString32), percent real opacity bounds at +0x0C/+0x10
// (both 1.0), the throb time at +0x14 (1000.0), a colour int at +0x18, the
// owning-player flag at +0x1C (true), the radius bounds at +0x20/+0x24, the
// selected-unit cap at +0x28, the spin rate at +0x2C and one more real at +0x30.
#pragma once
#ifndef BFME2_RADIUSDECALTEMPLATE_H
#define BFME2_RADIUSDECALTEMPLATE_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"

class Coord3D;
class Player;
class RadiusDecal;
class INI;
class Xfer;

class RadiusDecalTemplate
{
public:
	static void parseRadiusDecalTemplate(INI *ini, void *formal, void *store, const void *userData);
	RadiusDecalTemplate();
	~RadiusDecalTemplate();

	void xferRadiusDecalTemplate(Xfer *xfer);
	void createRadiusDecal(const Coord3D &pos, Real radius, const Player *owningPlayer, RadiusDecal &result) const;

	AsciiString m_texture;					// +0x00
	AsciiString m_texture2;					// +0x04
	Int m_style;							// +0x08
	Real m_opacityMin;						// +0x0C
	Real m_opacityMax;						// +0x10
	Real m_opacityThrobTime;				// +0x14
	Int m_color;							// +0x18
	Bool m_onlyVisibleToOwningPlayer;		// +0x1C
	Real m_minRadius;						// +0x20
	Real m_maxRadius;						// +0x24
	UnsignedInt m_maxSelectedUnits;			// +0x28
	Real m_rotationsPerMinute;				// +0x2C
	Real m_spiralAcceleration;				// +0x30 (INI token SpiralAcceleration, parse table 0x0080DC20 entry 12)
};
// SIZE 0x34

#endif
