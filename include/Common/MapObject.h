// MapObject.h: one object placed in a map file (Zero Hour Common/MapObject.h).
// Retail allocates it with the plain allocator in duplicate and frees it the
// same way in the deleting destructor, so there is no memory-pool glue. The
// virtual table holds only that deleting destructor.
// Retail moves the list link to the first field after the table pointer
// (Zero Hour keeps it after m_angle): fastAssignAllUniqueIDs walks the list
// through it. Each field names the access that places it.
#pragma once
#ifndef BFME2_MAPOBJECT_H
#define BFME2_MAPOBJECT_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"
#include "Common/Dict.h"

class RenderObjClass;
class ThingTemplate;
class Shadow;
enum BridgeTowerType;

class MapObject
{
public:
	virtual ~MapObject();

	Bool getFlag(Int flag) const { return (m_flags & flag) ? true : false; }

	const ThingTemplate *getThingTemplate(void) const;
	void setRenderObj(RenderObjClass *pObj);
	RenderObjClass *getBridgeRenderObject(BridgeTowerType type);
	void setBridgeRenderObject(BridgeTowerType type, RenderObjClass *renderObj);

	// validate calls verifyValidTeam, then verifyValidUniqueID.
	void validate(void);
	void verifyValidTeam(void);
	void verifyValidUniqueID(void);
	static void fastAssignAllUniqueIDs(void);
	static Int countMapObjectsWithOwner(const AsciiString &n);

private:
	MapObject *m_nextMapObject;				// fastAssignAllUniqueIDs follows it
	Coord3D m_location;						// duplicate copies it as the constructor's first argument
	AsciiString m_objectName;				// duplicate passes its address as the name
	const ThingTemplate *m_thingTemplate;	// duplicate passes it as the last constructor argument
	Real m_angle;							// duplicate loads it as the angle
	Int m_flags;							// getFlag
	Dict m_properties;						// verifyValidTeam and duplicate take its address
	Int m_color;							// duplicate copies it after construction
	RenderObjClass *m_renderObj;			// setRenderObj
	Shadow *m_shadowObj;					// Zero Hour's order; no retail access checked yet
	RenderObjClass *m_bridgeTowers[4];		// BRIDGE_MAX_TOWERS; set/getBridgeRenderObject
	Int m_runtimeFlags;						// duplicate copies it after construction
	UnsignedByte m_unknownTail[20];			// unresolved; completes the size duplicate allocates
};

#endif
