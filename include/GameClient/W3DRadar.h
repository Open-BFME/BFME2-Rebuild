// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DRadar.h: the W3D radar (Zero Hour's W3DRadar.h, Mark Wilczynski), derived from Radar. Target facts (retail game.dat): Radar is
// 0x1464 bytes; the texture formats W3DRadar keeps start at +0x1468 (initializeTextureFormats 0x0004D855 stores the terrain, overlay
// and shroud formats at +0x1468 / +0x1478 / +0x1484). Only the two protected helpers that do not touch the layout are declared here;
// the virtual overrides (draw 0x0005053E, setShroudLevel 0x0004EB00, clearShroud 0x0004E3B2 and the unnamed slots 8, 11 and 12), the
// constructor and the members come with the rows that read them.
#pragma once
#ifndef BFME2_W3DRADAR_H
#define BFME2_W3DRADAR_H

#include "Common/Radar.h"
#include "Common/RGBColor.h"
#include "Common/ICoord2D.h"

class W3DRadar : public Radar
{
protected:
	// The pixel on screen of a radar cell: the radar window's upper left plus the cell scaled to its size (the grid is 128 x 128).
	void radarToPixel(const ICoord2D *radar, ICoord2D *pixel, Int radarUpperLeftX, Int radarUpperLeftY, Int radarWidth, Int radarHeight);	// 0x0004DB33

	// Lightens a cell colour above the middle height and darkens it below (hi / mid / lo are the map's height thirds).
	void interpolateColorForHeight(RGBColor *color, Real height, Real hiZ, Real midZ, Real loZ);	// 0x0004E1A8
};

#endif
