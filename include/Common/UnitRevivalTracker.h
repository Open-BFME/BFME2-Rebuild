// UnitRevivalTracker.h: a player's record of units that can be revived. SIZE 0x14.
// Target facts (code RVAs, tables VAs):
//  - ctor 0x0037F27B (UnitRevivalTracker(Player *)) stores its table
//    0x00C18DF0 at +0x00, builds a vector at +0x04 (_Vector_base ctor), stores
//    the Player at +0x10 and calls 0x0037F26D. Player embeds one at +0x738 and
//    its dtor destroys the vector at +0x73C directly, so the tracker's own
//    destructor is implicit.
//  - The table has one slot, xfer 0x0037F39E (protected in the map). It is
//    not Snapshot's shape (Snapshot's slot 0 is the destructor).
//  - Size 0x14: the span Player gives it (+0x738..+0x74C).
#pragma once
#ifndef BFME2_UNITREVIVALTRACKER_H
#define BFME2_UNITREVIVALTRACKER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Vector.h"
#include "Common/UnitRevivalEntry.h"

class Player;
class Object;
class Image;
class ThingTemplate;
class Xfer;
class ArmySummarySystem;
struct Coord3D;

class UnitRevivalTracker
{
public:
	UnitRevivalTracker(Player *player);		// 0x0037F27B

	void clearEntries();					// 0x0037F26D (map: rva0037F26D)
	UnitRevivalEntry *getEntryByIndex(Int index);	// 0x0037E421: the entry at index, or null (map class: Rva0037E421)
	UnitRevivalEntry *getEntryById(Int id);			// 0x0037E451: the entry whose +0xA4 equals id, or null
	void *getEntryThingTemplate(Int index);			// 0x0037E7A5: the entry's template through getThingTemplate, null when there is no entry
	UnsignedByte isEntryUnqueued(Int index);					// 0x0037E7BC: the entry exists and its +0x98 is -1
	UnsignedByte productionSystemCancelUnitCreate(Int id);		// 0x0037E7DA: un-queues the entry whose id matches
	void setEntryProgress(Int index, Real value);		// 0x0037E87A: stores +0xCC (1.0 after the constructor, 0.998 when queued)
	Int rva0037F2C0(Object *object, Bool flag);	// 0x0037F2C0 (map: ?rva0037F2C0@Rva0037F2C0@@QAEHPAVObject@@_N@Z; RespawnUpdate::triggerDeathBeforeRespawn 0x004AF63E calls it on Player +0x738 and passes the entry index to 0x0037E421)
	Bool productionSystemQueueCreateUnit(Int index, Int id, const Image **outImage);	// 0x0037EDE0
	void removeBuildUnit(const ThingTemplate *unit);	// 0x0037EEB9
	void removeBuildUnitByIndex(Int index);	// 0x0037EEFC
	void rva0037EF2D(UnsignedInt id);		// 0x0037EF2D: drops the entry whose +0xA4 int equals id
	Int getTimeToBuild(Int index, Object *object);						// 0x0037E787
	Int getDiscountedCostToBuild(Int index, Object *object);			// 0x0037E649
	Int getDiscountedTimeToBuildById(Int id, Object *object);			// 0x0037E6E8
	Real getBuildProgress(Int index, Object *object);					// 0x0037E815
	Real getBuildProgressAndCost(Int index, Int *outCost, Object *object);	// 0x0037E898
	void *getThingTemplateByIndex(Int index);							// 0x0037E915
	const Image *getButtonImage(Int index);								// 0x0037EDC6
	Int findEntryIndex(const ThingTemplate *tmpl, Int id, Int skip);	// 0x0037EE4C
	void retrieveHeroesForArmySummary(ArmySummarySystem *summary);	// 0x0037EB3C
	Object *productionSystemNewObject(UnsignedInt productionId, const Coord3D *position);	// 0x0037EF5D

protected:
	virtual void xfer(Xfer *xfer);			// slot 0, 0x0037F39E

public:
	_STL::vector<UnitRevivalEntry> m_entries;	// +0x04 (ctor 0x0037F27B builds it, 0x0037F26D empties it)
	Player *m_player;						// +0x10
};
// SIZE 0x14
typedef char UnitRevivalTrackerSize[(sizeof(UnitRevivalTracker) == 0x14) ? 1 : -1];

#endif
