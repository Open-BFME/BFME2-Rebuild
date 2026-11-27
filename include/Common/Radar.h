// Radar.h: the radar subsystem (TheRadar): the object lists it draws, the
// queued radar events (pings) and the terrain refresh bookkeeping. SIZE 0x1464.
// W3DRadar derives it and implements the pure slots.
//
// Target facts (retail RVAs; table addresses are VAs). Two bases: Snapshot at +0
// and SubsystemInterface at +4 (the constructor 0x002D87B9 stores the primary table
// VA 0x00C0363C at +0 and the subsystem table VA 0x00C03604 at +4). The primary table
// has 13 slots: 0 the deleting destructor 0x002D8888, 1 loadPostProcess 0x002D7ABF,
// 2 GetSnapshotName (0x0004FA11, a folded `return "..."`), 3 xfer 0x002D85D7, then the
// Radar's own virtuals: 4 refreshTerrain 0x002D7AA6, 5 queueTerrainRefresh 0x002D7AB0,
// 6 0x002D7E68 (void(void *), 326 B), and six pure slots that W3DRadar fills with
// draw 0x0005053E (five ints), 0x0004F580, setShroudLevel 0x0004EB00, clearShroud
// 0x0004E3B2, 0x0004ED3D and 0x0004F84E. The subsystem table (14 slots) has the
// scalar deleting destructor thunk 0x002D7D4F, an empty init, reset 0x002D7DB9 and
// update 0x002D7DD5.
//
// Layout (constructor, clearAllEvents 0x002D7D57, xfer, update): +0x10 and +0x11 two
// Bools (xfer'd first), +0x14 and +0x18 two object lists (deleteListResources
// 0x002D75E5 empties +0x18 then +0x14, clearing each node's object back-pointer at
// Object+0x260), +0x1C..+0x28 four Reals, +0x2C the event table (64 RadarEvents of
// 0x50 bytes), +0x142C the index of the next free event, +0x1430 a handle newMap
// 0x002D7B6B stores, +0x1434..+0x1448 six Reals, +0x144C..+0x145B a 16-byte rectangle
// (set by 0x002D7BD3 to the empty box 0xFFFF0001/0xFFFF0001/0xFFFF/0xFFFF),
// +0x145C a Bool saying it is valid, +0x1460 the game frame a terrain refresh was
// queued at (0 = none).
#pragma once
#ifndef BFME2_RADAR_H
#define BFME2_RADAR_H

#include "Common/GameType.h"
#include "Common/Coord2D.h"
#include "Common/Coord3D.h"
#include "Common/ICoord2D.h"
#include "Common/IRegion2D.h"
#include "Common/Region3D.h"
#include "Common/RGBAColorInt.h"
#include "Common/Snapshot.h"
#include "Common/SubsystemInterface.h"
class Player;

class Object;
class TerrainLogic;
class GameWindow;

// One object on the radar (a node of Radar's two lists). Snapshot-derived (table with the four
// snapshot slots: GetSnapshotName 0x002D767C, xfer 0x002D8DC7); +4 is the object (deleteListResources
// 0x002D75E5 clears its Object+0x260 back-pointer), +8 the next node and +0xC a color (-1 until set);
// SIZE 0x10 (xferRadarObjectList 0x002D84F6 allocates 0x10 and builds the node inline: table VA
// 0x00C035D0, +4 and +8 cleared, +0xC = -1). RadarObject::xfer reads a version (1, 1), the object's id,
// re-finds the object when loading (throwing when it is gone), points Object+0x260 back at the node and
// transfers the color.
class RadarObject : public Snapshot
{
public:
	RadarObject() : m_object(0), m_next(0), m_color(-1) {}
	virtual ~RadarObject() {}

protected:
	virtual void loadPostProcess() {}
	virtual const char *GetSnapshotName() const;			// 0x002D767C
	virtual void xfer(Xfer *xfer);							// 0x002D8DC7

public:
	Object *m_object;										// +0x04
	RadarObject *m_next;									// +0x08
	int m_color;											// +0x0C
};
// SIZE 0x10

// The classification of an object on the radar; isPriorityVisible 0x002D7ACB is false for the first two.
enum RadarPriorityType { RADAR_PRIORITY_INVALID = 0, RADAR_PRIORITY_NOT_ON_RADAR = 1, RADAR_PRIORITY_STRUCTURE = 2, RADAR_PRIORITY_UNIT = 3 };

// A marker object radar events point at (AddReference 0x002D76B7, DeleteReference 0x002D76BB).
class RadarMarker
{
public:
	virtual void slot0();									// slot 0 (name not evidenced)
	virtual void deleteMarker();							// slot 1: what DeleteReference calls when the count reaches zero
	void AddReference();									// 0x002D76B7
	void DeleteReference();									// 0x002D76BB: drops the count (+8), deleting the marker at zero
	void releaseTarget();									// 0x002D335B (what the event slot and RadarEvent's destructor call)

	int m_unk04;											// +0x04
	int m_refCount;											// +0x08
};

// The pointer an event keeps to the marker it points at. clearRef 0x002D7CC6 releases
// a non-null target (0x002D335B) and nulls the slot.
struct RadarEventRefSlot
{
	void clearRef();										// 0x002D7CC6

	RadarMarker *m_target;									// +0x00
};

// One queued radar ping. SIZE 0x50. The constructor 0x002D7CD9 clears the slot at
// +0x4C, the destructor 0x002D7CE0 releases it; clearAllEvents resets the rest.
class RadarEvent
{
public:
	RadarEvent();											// 0x002D7CD9
	~RadarEvent();											// 0x002D7CE0

	int m_type;												// +0x00 (0xB = none, the value clearAllEvents stores)
	Bool m_active;											// +0x04
	unsigned char m_pad05[3];
	UnsignedInt m_createFrame;								// +0x08 the client frame the event was created at (internalCreateEvent 0x002D8395)
	UnsignedInt m_dieFrame;									// +0x0C createFrame + seconds-to-live in frames; update() deactivates the event after it
	UnsignedInt m_fadeFrame;								// +0x10 dieFrame minus the fade time in frames
	RGBAColorInt m_color1;									// +0x14 (xfer'd as an RGBAColorInt; internalCreateEvent copies its fourth argument here)
	RGBAColorInt m_color2;									// +0x24 (the fifth argument)
	Coord3D m_worldLoc;										// +0x34 the event's world position
	ICoord2D m_radarLoc;									// +0x40 its radar cell (worldToRadar of the position)
	Bool m_soundPlayed;										// +0x48 cleared on creation (role-inferred name)
	unsigned char m_pad49[3];
	RadarEventRefSlot m_ref;								// +0x4C
};
// SIZE 0x50

// The queue's event kinds are the indices of the colour table in Radar.cpp (0..10); 11 ends the table
// and is what an empty RadarEvent holds. The individual kinds' names are not recovered here.
enum RadarEventType { RADAR_EVENT_NONE = 11 };

// One row of the event colour table in Radar.cpp (retail data at VA 0x00DBCD08, 0x24 bytes per row).
struct RadarEventColors {
	int type;
	RGBAColorInt color1;
	RGBAColorInt color2;
};

class Radar : public Snapshot, public SubsystemInterface
{
public:
	void removeObject(Object *obj);	// 0x002D7714 (map spells the parameter PAURva002D76C6Owner; StealthUpdate::changeVisualDisguise 0x00374BC8 passes the object)
	void createPlayerEvent(const Player *player, const Coord3D *world, RadarEventType type, Real secondsToLive);	// 0x002D893B (map spells the first parameter PBURva002D893BColorSource; BattlePlanUpdate::setStatus 0x00497FF3 passes the controlling player)
	void addObject(Object *obj);	// 0x002D7FEC: KeepObjectDie::onDie adds a kept object to the radar
	void rva002D7FAE(Object *obj);	// 0x002D7FAE (map's provisional name): Object::setTeam 0x00298AE4 calls it after a team change
	Radar();												// 0x002D87B9
	virtual ~Radar();										// 0x002D7CED, deleting dtor 0x002D8888

	virtual void loadPostProcess();							// snapshot slot 1: 0x002D7ABF
	virtual const char *GetSnapshotName() const;			// snapshot slot 2: 0x0004FA11 (folded)
protected:
	virtual void xfer(Xfer *xfer);							// snapshot slot 3: 0x002D85D7 (protected in retail: MAE)
public:

	// Radar's own virtuals, in retail slot order.
	virtual void refreshTerrain(TerrainLogic *terrain);		// slot 4: 0x002D7AA6
	virtual void queueTerrainRefresh();						// slot 5: 0x002D7AB0
	virtual void slot6(void *arg);							// slot 6: 0x002D7E68 (name not evidenced)
	virtual void draw(int a, int b, int c, int d, int e) = 0;	// slot 7 (W3DRadar 0x0005053E)
	virtual void slot8() = 0;								// slot 8 (W3DRadar 0x0004F580; name not evidenced)
	virtual void setShroudLevel(int x, int y, int status) = 0;	// slot 9 (W3DRadar 0x0004EB00; status is a CellShroudStatus)
	virtual void clearShroud() = 0;							// slot 10 (W3DRadar 0x0004E3B2)
	virtual void slot11(int x, int y, int value) = 0;		// slot 11 (W3DRadar 0x0004ED3D; name not evidenced; rva002D7B18 passes it three dwords)
	virtual void slot12() = 0;								// slot 12 (W3DRadar 0x0004F84E; name not evidenced)

	// SubsystemInterface (table at +4): init is empty here.
	virtual void init() {}
	virtual void reset();									// 0x002D7DB9
	virtual void update();									// 0x002D7DD5

	bool worldToRadar(const Coord3D *world, ICoord2D *radar);	// 0x002D77C9
	bool radarToWorld(const ICoord2D *radar, Coord3D *world);	// 0x002D7744
	Object *searchListForRadarLocationMatch(RadarObject *listHead, ICoord2D *radarMatch);	// 0x002D78A4
	Object *objectUnderRadarPixel(const ICoord2D *pixel);	// 0x002D834E
	bool localPixelToRadar(const ICoord2D *pixel, ICoord2D *radar);	// 0x002D81EC
	bool screenPixelToWorld(const ICoord2D *pixel, Coord3D *world);	// 0x002D82E4
	void rva002D7E68(void *terrain);						// 0x002D7E68: the new-map sampler (the ledger spells it non-virtual; slot 6 of the table points here)
	bool rva002D782C(const Coord3D *world, Coord2D *cell);	// 0x002D782C: the world position as a fractional radar cell, clamped to 0..127
	void rva002D790A(Real *x, Real *y);						// 0x002D790A: the map extent's aspect (the longer side 1, the other its ratio)
	void findDrawPositions(int startX, int startY, int width, int height, ICoord2D *ul, ICoord2D *lr);	// 0x002D799C
	void newMap();											// 0x002D7B6B
	void createEvent(const Coord3D *world, RadarEventType type, Real secondsToLive);	// 0x002D88A4: looks the colours up by type in the table in Radar.cpp
	bool tryEvent(RadarEventType type, const Coord3D *world);	// 0x002D8AF7: no event of this type within 600 units for the last ten seconds, then creates one (4 s)
	void rva002D7BB7(const void *box);						// 0x002D7BB7: copies a 16-byte box into +0x144C and marks it valid
	void rva002D7BD3();										// 0x002D7BD3: resets the 16-byte rectangle to the empty box and clears its valid flag
	bool isPriorityVisible(RadarPriorityType priority) const;	// 0x002D7ACB
	// Zero Hour's inline setter: VictoryConditions::cachePlayerPtrs stores the flag directly, so retail has no out-of-line copy.
	void forceOn(Bool force) { m_radarForceOn = force; }

protected:
	void deleteListResources();								// 0x002D75E5
	void clearAllEvents();									// 0x002D7D57
	void internalCreateEvent(const Coord3D *world, RadarEventType type, Real secondsToLive, const RGBAColorInt *color1, const RGBAColorInt *color2);	// 0x002D8395

	Bool m_radarHidden;										// +0x10
	Bool m_radarForceOn;									// +0x11
	unsigned char m_pad12[2];
	RadarObject *m_localObjectList;							// +0x14
	RadarObject *m_objectList;								// +0x18
	Real m_terrainAverageZ;									// +0x1C (four Reals xfer'd together; names: Zero Hour's, values read from worldToRadar/radarToWorld)
	Real m_waterAverageZ;									// +0x20
	Real m_xSample;											// +0x24 world units per radar cell, x (the grid is 128 x 128)
	Real m_ySample;											// +0x28
	RadarEvent m_events[64];								// +0x2C
	int m_nextFreeRadarEvent;								// +0x142C
	GameWindow *m_radarWindow;								// +0x1430 the radar window newMap 0x002D7B6B looks up (screenPixelToWorld reads its screen position)
	Region3D m_mapExtent;									// +0x1434 the map's extent (lo x,y,z then hi x,y,z; findDrawPositions fits it into the radar window)
	IRegion2D m_box;										// +0x144C a 16-byte box (xfer'd as an IRegion2D); empty until set
	Bool m_rectValid;										// +0x145C
	unsigned char m_pad145D[3];
	UnsignedInt m_queuedRefreshFrame;						// +0x1460
};
// SIZE 0x1464

typedef char RadarSizeCheck[sizeof(Radar) == 0x1464 ? 1 : -1];

extern Radar *TheRadar;

#endif
