// PathfindCell.h: one pathfinding grid cell (WorldBuilder: pathfinder\pathfinder_cell.h,
// unit Pathfinder/pathfinder_cell.cpp).
// Target facts (retail game.dat):
// - SIZE 0x10: PathfindLayer::getCell 0x00366626 indexes a column as y*0x10, and
//   PathfindLayer's reset 0x003667D4 runs `delete []` (vector deleting dtor
//   0x002E70E8) on the cell block, so the class has a destructor.
// - +0x00 the per-search info block (SetParentCell 0x0052DAC4, SetOpen 0x0052DAE9,
//   StartPathfind 0x0052DA95 and IsObstaclePresent 0x002E6C23 all go through it).
// - +0x04 a word zeroed and +0x08 a 16-bit field set to 0xFFFF by 0x0052DED3.
// - Ctor 0x0052DFF2: no info block, then 0x0052DED3.
// - +0x0C a 32-bit bitfield: the cell type in bits 0..3 (getCell rejects type 5;
//   IsObstaclePresent tests type 4), the layer in bits 4..9, bit 16 pinched
//   (setPinched 0x0052E0FC), the top byte kept by 0x0052DED3. Bits 10..15 and
//   17..22 are not named yet.
#pragma once
#ifndef BFME2_PATHFINDCELL_H
#define BFME2_PATHFINDCELL_H

#include <stddef.h>
#include "Common/GameType.h"

class Xfer;

// The per-search block a cell borrows (Zero Hour's PathfindCellInfo). Only the
// obstacle ID is placed so far: IsObstaclePresent 0x002E6C23 and the zone-change
// walk callback 0x002E7261 read +0x28, which is Zero Hour's m_obstacleID offset.
class PathfindCellInfo
{
public:
	UnsignedByte m_unk0x00[0x28];					// +0x00 (Zero Hour: open-list links, path parent, cell, costs, position, goal and position unit IDs)
	ObjectID m_obstacleID;							// +0x28
	// The flag word after the obstacle ID: SetOpen sets and clears bit 3; PutOnClosedList tests and
	// sets bit 4 and RemoveFromClosedList clears it. Zero Hour has the same pair one bit higher.
	UnsignedInt m_unkBits0 : 3;						// +0x2C bits 0..2
	UnsignedInt m_open : 1;							// +0x2C bit 3
	UnsignedInt m_closed : 1;						// +0x2C bit 4
	UnsignedInt m_unkBits5 : 27;					// +0x2C bits 5..31
};

class PathfindCell
{
public:
	// Cell types (Zero Hour's values; IS_IMPASSABLE 0x002E7569 takes the enum,
	// IsObstaclePresent 0x002E6C23 tests CELL_OBSTACLE, the fling walk 0x002E85AD
	// stops on CELL_CLIFF, CELL_OBSTACLE and CELL_BRIDGE_IMPASSABLE).
	enum CellType
	{
		CELL_CLEAR = 0x00,
		CELL_WATER = 0x01,
		CELL_CLIFF = 0x02,
		CELL_RUBBLE = 0x03,
		CELL_OBSTACLE = 0x04,
		CELL_BRIDGE_IMPASSABLE = 0x05,
		CELL_IMPASSABLE = 0x06,
		CELL_WATER_7 = 0x07			// BFME 2 only: IsWaterCell 0x002E9897 counts it with CELL_WATER (no name yet)
	};

	~PathfindCell();

	UnsignedInt getType() const { return m_type; }
	UnsignedInt getLayer() const { return m_layer; }
	Bool IsObstaclePresent(ObjectID objID) const;			// 0x002E6C23 (Zero Hour's isObstaclePresent)
	void setPinched(Bool pinched);							// 0x0052E0FC
	Bool setFlag23(Bool set);								// 0x0052E0CE (provisional name): false when bit 23 already equals `set`
	PathfindCell();											// 0x0052DFF2
	void rva0052DED3();										// 0x0052DED3, resets the cell then ReleaseInfo
	void SetOpen(Bool open);
	void ReleaseInfo();										// 0x0052DE5B
	void DoXfer(Xfer *xfer);								// 0x0052D84C

	PathfindCellInfo *m_info;						// +0x00
	UnsignedInt m_unk0x04;							// +0x04
	UnsignedShort m_unk0x08;						// +0x08
	UnsignedShort m_unk0x0A;						// +0x0A
	UnsignedInt m_type : 4;							// +0x0C bits 0..3
	UnsignedInt m_layer : 6;						// +0x0C bits 4..9, the cell's PathfindLayerEnum: 0x0052DED3 resets it to 1 (ground), 0x002E8251 compares (word & 0x3F0) with 0x10, 0x002E7ED6 and 0x002ED01E read (word >> 4) & 0x3F
	UnsignedInt m_unkBits10 : 6;					// +0x0C bits 10..15 (0x0052DED3 clears them with the layer in one dword op)
	UnsignedInt m_pinched : 1;						// +0x0C bit 16
	UnsignedInt m_unkBits17 : 6;					// +0x0C bits 17..22
	UnsignedInt m_flag23 : 1;						// +0x0C bit 23 (setFlag23 0x0052E0CE)
	UnsignedInt m_unkBits24 : 8;					// +0x0C bits 24..31
};

typedef char PathfindCellSize[(sizeof(PathfindCell) == 0x10) ? 1 : -1];

#endif
