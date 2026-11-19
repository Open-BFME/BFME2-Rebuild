// PartitionManager.h: the spatial index singleton (ThePartitionManager, VA 0x00DFE748).
// Target facts (code RVAs, vtable VAs):
//  - SIZE 0x14: GameLogic::init 0x00243EE7 allocates 0x14 and calls the
//    constructor 0x006253F0, which builds SubsystemInterface at +0, stores
//    SubsystemInterface's table 0x00C7C6B0 (14 slots) at +0 and a Snapshot
//    table 0x00C7C6A0 (4 slots) at +0xC, then news the 0x124-byte
//    PartitionManagerImpl (ctor 0x00627FB0) into +0x10. The destructor
//    0x006254B0 deletes the implementation (0x00627DE0) and runs
//    ~SubsystemInterface.
//  - Every query is a forwarder: `mov ecx,[ecx+0x10]; jmp/call Impl::...`
//    (getClosestObject 0x00625360, iterateObjectsInRange 0x00625610, ...).
//  - SubsystemInterface slots overridden: 0 deleting dtor 0x006255B0,
//    1 init 0x00625560 (an Impl call with a zeroed six-word state block),
//    9 reset 0x006158B0 (jumps through slot 1), 10 update 0x006252F0
//    (Impl::handle 0x00627770). Snapshot slots: 1 empty 0x0069E440, 2 the name
//    "PartitionManager" (0x006254A0, string at 0x00C7C6E8), 3 xfer 0x00625530
//    (version byte 1, then Impl::xfer). Access of the Snapshot overrides is not
//    proven (the map has no decorated names for them); protected, as Snapshot.
#pragma once
#include "GameLogic/ObjectIterator.h"
#include "Common/GameType.h"
#include "Common/SubsystemInterface.h"
#include "Common/Snapshot.h"
class PartitionFilter;
class PartitionManagerImpl;
class Xfer;
class Object;
class Coord3D;
class Coord2D;
struct Region2D;

enum CellShroudStatus
{
	CELLSHROUD_CLEAR,
	CELLSHROUD_FOGGED,
	CELLSHROUD_SHROUDED
};

// Zero Hour's PartitionManager::FindPositionOptions. Retail's findPositionAround
// (0x00285202) reads flags +0, minRadius +4, maxRadius +8, startAngle +0xC,
// maxZDelta +0x10, ignoreObject +0x14, sourceToPathToDest +0x18 and
// relationshipObject +0x1C; the defaults are Zero Hour's.
struct FindPositionOptions
{
	Int flags;							// 0x00
	Real minRadius;						// 0x04
	Real maxRadius;						// 0x08
	Real startAngle;					// 0x0C
	Real maxZDelta;						// 0x10
	const Object *ignoreObject;			// 0x14
	const Object *sourceToPathToDest;	// 0x18
	const Object *relationshipObject;	// 0x1C

	FindPositionOptions()
	{
		flags = 0;
		minRadius = 0.0f;
		maxRadius = 0.0f;
		startAngle = -99999.9f;
		maxZDelta = 1e10f;
		ignoreObject = 0;
		sourceToPathToDest = 0;
		relationshipObject = 0;
	}
};
// SIZE 0x20

// Zero Hour's DistanceCalculationType (the fourth argument of getClosestObject and
// iterateObjectsInRange). Retail's findClosestRepulsor and TunnelNetworkScan call
// getClosestObject with 1 and 0; Zero Hour spells 0 FROM_CENTER_2D, but the
// old repo's matched bodies named 0 FROM_CENTER_3D; the values are the fact.
enum DistanceCalculationType
{
	FROM_CENTER_3D = 0,
	FROM_CENTER_2D = 1
};

class PartitionManager : public SubsystemInterface, public Snapshot
{
public:
	// The per-object data (WorldBuilder's PartitionManager::Data; Object holds one at +0x4C8). Only what game
	// code calls is declared. makeDirty 0x00625840 (map placeholder ?rva00625840@Rva00625840) links the data
	// into its manager's dirty list once (+0x18 / +0x1C against +0x120), the shape of the shroud data's
	// makeDirty 0x0073A0F0; Object::setTeam 0x00298AE4 calls it.
	class Data
	{
	public:
		void makeDirty();
	};

 PartitionManager();					// 0x006253F0
 virtual ~PartitionManager();			// 0x006254B0 (deleting 0x006255B0)
 virtual void init();					// slot 1, 0x00625560
 virtual void reset();					// slot 9, 0x006158B0
 virtual void update();					// slot 10, 0x006252F0
protected:
 virtual void loadPostProcess() {}		// Snapshot slot 1, the shared empty body 0x0069E440
 virtual const char *GetSnapshotName() const;	// Snapshot slot 2, 0x006254A0
 virtual void xfer(Xfer *xfer);			// Snapshot slot 3, 0x00625530
public:
 Object *getClosestObject(const Coord3D *,float,int,PartitionFilter *);
 ObjectIterator iterateObjectsInRange(const Coord3D *,float,int,PartitionFilter *,int);
 ObjectIterator rva006255D0(const Coord3D *,float,int,int);	// 0x006255D0 (iterateObjectsInRange without a filter list; DemoTrapUpdate::update 0x00495ABA)
 // 0x00285202 takes no receiver: its callers push the three arguments and call
 // straight through (TeleportToCasterSpecialPower 0x004CD5EF), so it is static.
 static Bool findPositionAround(const Coord3D *pos, const FindPositionOptions *options, Coord3D *result);
 // 0x007397F0 (mangled ?getShroudStatusForPlayer@PartitionManager@@QBE?AW4CellShroudStatus@@HPBUCoord3D@@@Z).
 CellShroudStatus getShroudStatusForPlayer(Int playerIndex, const Coord3D *pos) const;

 // 0x00739730 (map: ?rva00739730@Rva00739730@@QAEXHH@Z, an 8-byte forwarder): PlayerList::setLocalPlayer 0x002A7B34 calls it on the
 // shroud manager with the new local player index and the callback 0x002D7AE0 (a function pointer; the map types it int).
 void rva00739730(Int playerIndex, void (*callback)(Int, Int, Int));
 // 0x00739780 / 0x007397C0 (map: rva00739780@Rva00739780, rva007397C0@Rva007397C0, both QAEXH@Z forwarders on the shroud manager): ScriptActions::doRevealMapEntire
 // 0x003BB94C and doShroudMapEntire 0x003BBA59 call them with a player index (Zero Hour's revealMapForPlayer / shroudMapForPlayer).
 // 0x00739830 (map: ?rva00739830@Rva00739830@@QBEHPBUBfmePointFD@@HI@Z, an 8-byte forwarder to the impl's cell sum 0x0073BD70, which adds the 16-bit
 // per-player counts of one cell over the players set in the mask): EmotionTrackerUpdate::selectNugget 0x004B13D8 calls it twice with a mask of
 // players related to the owner. The map types the position as a point struct.
 Int sumCellValueForPlayers(const Coord3D *position, Int layer, UnsignedInt playerMask) const;
 void rva00739780(Int playerIndex);
 void rva007397C0(Int playerIndex);
 void revealMapForPlayerPermanently(Int playerIndex);	// 0x00739790
 void undoRevealMapForPlayerPermanently(Int playerIndex);	// 0x007397B0
 void doShroudReveal(const Coord2D *center, Real radius, Int playerMask);	// 0x00739A30 (decl_queue f02 15:4x, ACCEPTED k10; ScriptEngine::doNamedMapReveal 0x003577DA)
 void undoShroudReveal(const Coord2D *center, Real radius, Int playerMask);	// 0x00739BE0
 void doShroudRevealRegion(const Region2D *region, Int extra, UnsignedInt playerMask);	// 0x00739AF0
 void undoShroudRevealRegion(const Region2D *region, Int extra, UnsignedInt playerMask);	// 0x00739CA0

 // 0x007397D0 (map: ?rva007397D0@Rva007397D0@@QAEXXZ): PlayerList::refreshObservedPlayerView 0x002A7E42 calls it on the shroud manager (when set) before it refreshes the control bar.
 void rva007397D0();

 PartitionManagerImpl *m_impl;			// 0x10
};
// SIZE 0x14
extern PartitionManager *ThePartitionManager;
// The shroud manager is declared as the same PartitionManager type by the map
// (?TheShroudManager@@3PAVPartitionManager@@A). Its real class has the same shape
// (SubsystemInterface + Snapshot + an implementation pointer at +0x10, tables
// 0x008F1250/0x008F1240, impl ShroudManagerImpl) and forwards 0x007397F0 to
// ShroudManagerImpl::GetShroudStatusForPlayer; the forwarders are shared by name here.
extern PartitionManager *TheShroudManager;
