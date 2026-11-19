// DRAFT (h07): PathNode, one node of a Path. Target facts:
// - Ctor 0x0026212A (const Coord3D *pos, PathfindLayerEnum layer): zero +0/+4/+8,
//   the position at +0xC, the layer at +0x18, false at +0x1C, 0x7FFFFFFF at +0x20.
// - 0x00262161 (map placeholder Rva0026E4A0::set) is Zero Hour's prependToList:
//   +8 = list, +0 = list, list->+4 = this, returns this.
// - Nodes come from the pool object at 0x00E01E94 (pop 0x002635C2), not operator new;
//   the cell size is not recovered, so SIZE is at least 0x24.
// - Path::DoXfer 0x003661FC transfers each node's position (+0xC), layer (+0x18),
//   Bool +0x1C and the IDs of the +8 / +0x20 neighbours.
// Zero Hour PathNode names; +0 and +8 roles follow prependToList.
#pragma once
#ifndef BFME2_DRAFT_PATHNODE_H
#define BFME2_DRAFT_PATHNODE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Coord3D.h"

enum PathfindLayerEnum;

class PathNode
{
public:
  const Coord3D *getPosition() const { return &m_pos; } // inline (Zero Hour's): the move-away states copy +0x0C
	PathNode(const Coord3D *pos, PathfindLayerEnum layer);	// 0x0026212A
	PathNode *prependToList(PathNode *list);				// 0x00262161

	PathNode *m_nextOpti;						// +0x00
	PathNode *m_prev;							// +0x04
	PathNode *m_next;							// +0x08
	Coord3D m_pos;								// +0x0C
	PathfindLayerEnum m_layer;					// +0x18
	Bool m_canOptimize;							// +0x1C
	Int m_id;									// +0x20, 0x7FFFFFFF
};

typedef char PathNode_off_m_id[(offsetof(PathNode, m_id) == 0x20) ? 1 : -1];

#endif
