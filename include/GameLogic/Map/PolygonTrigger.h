// PolygonTrigger.h: the named map trigger area. Only the member the script conditions call is
// declared here; the layout is not recovered yet. Retail row 0x002E3A13 is PolygonTrigger's
// pointInTrigger: a non-virtual member taking a const ICoord3D reference.
#pragma once
#ifndef BFME2_POLYGONTRIGGER_H
#define BFME2_POLYGONTRIGGER_H

#include "Common/AreaPolygon.h"
#include "Common/GameType.h"
#include "Common/ICoord2D.h"
#include "Common/ICoord3D.h"
#include "Common/IRegion2D.h"
#include "Common/Region2D.h"
#include "Common/AsciiString.h"

class Coord3D;

struct Rva002E3A8DPair	// one vertex copied out by PolygonTrigger::get
{
	Real first;
	Real second;
};

Bool rva00285B66(const void *point, const void *polygon);	// whether the integer point (x, y) lies inside the polygon

class PolygonTrigger
{
public:
	static void addPolygonTrigger(PolygonTrigger *trigger);
	virtual ~PolygonTrigger();						// PathfindLayer::reset 0x00366DEC deletes one through vtable slot 0
	Rva0030B72CPoint getCenterPoint2D() const;		// the centre of the polygon
	Bool rva002E3A39(const Coord3D &position);		// pointInTrigger for a float position
	Region2D rva0007E03A();							// the polygon's bounds
	Bool rva002E37B1(const Rva0030B7C2Point *point);	// whether the point lies inside the polygon
	Rva002E3A8DPair *get(Rva002E3A8DPair *out, Int index);	// the vertex at the index
	void set(Int callback, Int userData);			// stores the enter/exit callback and its user data
	void notifyObjectEntered(Int object);			// calls the callback with true
	void notifyObjectExited(Int object);			// calls the callback with false
	Bool pointInTrigger(const ICoord3D &position);	// 0x002E3A13
	void getCenterPoint(Coord3D *center) const;		// 0x002E38F3 (AIUpdateInterface::privateGuardAreaFromPosition 0x00264DF8 asks it when no position is given)
	void getBounds(Region2D *bounds);				// 0x002E3954 (the bounding rectangle in floats)
	void getBounds(IRegion2D *bounds);				// 0x002E3978 (PathfindLayer::DoBoundsOverlap 0x003665CB asks each area)

	UnsignedByte m_unk0x04[4];						// +0x04 (virtual-base pointer)
	ElevatedAreaPolygon m_polygon;						// +0x08..+0x33
	UnsignedByte m_unk0x34[4];						// +0x34
	Int m_unk0x38;									// +0x38 (ScriptEngine::doNamedMapReveal 0x003577DA hands its address to PartitionManager::doShroudRevealRegion)
	PolygonTrigger *m_nextLayerArea;				// +0x3C, a pathfind layer's area list: AddArea 0x003665BB pushes onto it, DoBoundsOverlap 0x003665CB walks it
	AsciiString m_triggerName;						// +0x40 (Zero Hour name): AIGuardMachine::xfer 0x00542FC5 copies it to save the guarded area by name
	Int m_triggerID;								// +0x44 (a counter value taken at construction, or passed in)
	Bool m_unk0x48;									// +0x48
	AsciiString m_unk0x4C;							// +0x4C (a second name string, written next to the trigger name)
	Bool m_unk0x50;									// +0x50
	UnsignedInt m_enterExitCallback;				// +0x54 (a cdecl function address)
	Int m_enterExitUserData;						// +0x58
};

#endif
