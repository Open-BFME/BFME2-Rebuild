// PathfindLayer.h: one pathfinding layer (ground or a bridge), unit
// Pathfinder/pathfinder_path.cpp (WorldBuilder: pathfinder_layer.cpp holds
// ClassifyBridgeCells).
// Target facts (retail game.dat):
// - Ctor 0x003666C6 (no vtable): zero +0x00..+0x14, -1 at +0x18..+0x24, zero +0x28,
//   -1 +0x2C, false +0x30, zero +0x34/+0x38, -1 +0x3C.
// - getCell 0x00366626 (x, y): +0x04 is the column array (PathfindCell *[width]),
//   +0x08/+0x0C the width and height, +0x10/+0x14 the origin; a cell of type 5 reads
//   as no cell.
// - 0x00366DEC (map placeholder; ~PathfindLayer 0x00367369 jumps to it, so it is the
//   reset): zero +0x34, release the cells (0x003667D4), +0x28 = 1, `delete` the +0x38
//   trigger (virtual dtor), then -1 in +0x18..+0x24, +0x2C and +0x3C.
// - xfer 0x003666FD (map placeholder; version 1): every cell's DoXfer, then Ints
//   +0x08..+0x14, ICoord2Ds +0x18/+0x20, a copy of +0x28 (read, not stored back), Int +0x2C,
//   Bool +0x30, Int +0x3C.
// - 0x003667D4 (map placeholder; releaseCells, provisional): resets every cell (0x0052DED3), frees
//   the column array (delete []) and the cell block at +0x00 (vector deleting dtor),
//   then -1 at +0x2C and zero +0x00/+0x08..+0x14.
// - +0x38 heads a PolygonTrigger list linked through the trigger's +0x3C
//   (AddArea 0x003665BB, DoBoundsOverlap 0x003665CB).
// - SIZE 0x40: Pathfinder's ctor 0x002F6E22 builds PathfindLayer[16] at +0x60 with the
//   eh vector ctor (element size 0x40), ctor 0x003666C6 and dtor 0x00367369.
#pragma once
#ifndef BFME2_PATHFINDLAYER_H
#define BFME2_PATHFINDLAYER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/ICoord2D.h"
#include "Common/IRegion2D.h"

class PathfindCell;
class PolygonTrigger;
class Xfer;

class PathfindLayer
{
public:
	PathfindLayer();												// 0x003666C6
	PathfindCell *getCell(Int x, Int y);							// 0x00366626
	void releaseCells();											// 0x003667D4 (provisional name)
	void reset();													// 0x00366DEC
	void xfer(Xfer *xfer);											// 0x003666FD
	void AddArea(void *area);										// 0x003665BB (the map types the trigger as void *)
	Bool DoBoundsOverlap(const IRegion2D *region);					// 0x003665CB

	PathfindCell *m_blockOfMapCells;			// +0x00
	PathfindCell **m_layerCells;				// +0x04, columns
	Int m_width;								// +0x08
	Int m_height;								// +0x0C
	Int m_xOrigin;								// +0x10
	Int m_yOrigin;								// +0x14
	ICoord2D m_unk0x18;							// +0x18, (-1, -1)
	ICoord2D m_unk0x20;							// +0x20, (-1, -1)
	Int m_unk0x28;								// +0x28
	Int m_unk0x2C;								// +0x2C, -1
	Bool m_unk0x30;								// +0x30
	Int m_unk0x34;								// +0x34
	PolygonTrigger *m_areas;					// +0x38
	Int m_unk0x3C;								// +0x3C, -1
};

typedef char PathfindLayer_off_m_areas[(offsetof(PathfindLayer, m_areas) == 0x38) ? 1 : -1];
typedef char PathfindLayerSize[(sizeof(PathfindLayer) == 0x40) ? 1 : -1];

#endif
