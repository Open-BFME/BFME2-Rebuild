// PolygonTrigger.h: the named map trigger area. Only the member the script conditions call is
// declared here; the layout is not recovered yet. Retail row 0x002E3A13 is PolygonTrigger's
// pointInTrigger: a non-virtual member taking a const ICoord3D reference.
#pragma once
#ifndef BFME2_POLYGONTRIGGER_H
#define BFME2_POLYGONTRIGGER_H

#include "Common/ICoord3D.h"
#include "Common/IRegion2D.h"
#include "Common/Region2D.h"
#include "Common/AsciiString.h"

class Coord3D;

class PolygonTrigger
{
public:
	virtual ~PolygonTrigger();						// PathfindLayer::reset 0x00366DEC deletes one through vtable slot 0
	Bool pointInTrigger(const ICoord3D &position);	// 0x002E3A13
	void getCenterPoint(Coord3D *center) const;		// 0x002E38F3 (AIUpdateInterface::privateGuardAreaFromPosition 0x00264DF8 asks it when no position is given)
	void getBounds(Region2D *bounds);				// 0x002E3954 (the bounding rectangle in floats)
	void getBounds(IRegion2D *bounds);				// 0x002E3978 (PathfindLayer::DoBoundsOverlap 0x003665CB asks each area)

	UnsignedByte m_unk0x04[0x34];					// +0x04..+0x37, not recovered
	Int m_unk0x38;									// +0x38 (ScriptEngine::doNamedMapReveal 0x003577DA hands its address to PartitionManager::doShroudRevealRegion)
	PolygonTrigger *m_nextLayerArea;				// +0x3C, a pathfind layer's area list: AddArea 0x003665BB pushes onto it, DoBoundsOverlap 0x003665CB walks it
	AsciiString m_triggerName;						// +0x40 (Zero Hour name): AIGuardMachine::xfer 0x00542FC5 copies it to save the guarded area by name
};

#endif
