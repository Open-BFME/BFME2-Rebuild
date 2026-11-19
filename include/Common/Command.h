// Command.h: a player's command-point account (Player +0x60). The class name is Command: WorldBuilder's
// Common/Rts/Command.cpp asserts "Command::Init" (a local `lwPlayer`), "Command::RemoveCommandPointsFromBonus"
// (`m_commandPointBonus>=0`), "Command::AddUpgradableCommandPointsToBonus" and
// "Command::RemoveUpgradableCommandPointsFromBonus" (messages "bonus already stored" / "unable to find bonus"),
// and an earlier function there asserts `m_commandPointsTotal && m_commandPointsMax`. The map's ledger names
// (rows 0x002A71BC, 0x002A77C1, 0x002A7611) agree, and the snapshot literal at VA 0x00BFDDC4 is "Command".
// Member names marked (WB) are the assert strings'; the others are descriptive, from the bodies.
// Target facts (code RVAs, VA 0x00BFDDB4 table):
//  - Snapshot-derived, SIZE 0x30 (Player's next member, Money, starts at +0x60+0x30).
//    Constructor 0x002AE5EB (COMDAT): table at +0, +4 = 100, +8 = 0, +0x14..+0x1C
//    = 0, the vector at +0x20 (_Vector_base ctor 0x00611E58), byte +0x2C = 0;
//    +0x0C and +0x10 are not written. Table: 0 deleting dtor 0x002AF53E (~ 0x002AE627),
//    1 empty 0x000B3FD0, 2 name 0x002AE621 ("Command"), 3 xfer 0x002A7818.
//  - xfer 0x002A7818, version 7: v1 +4, +8, +0x14; v2 +0x18; v3 +0x1C; v4 +0xC;
//    v5 the vector at +0x20 as a count followed by (Int, ObjectID, ObjectFilter)
//    entries; v6 +0x10; v7 the byte +0x2C.
//  - Init 0x002A71BC stores the player index at +0x14. rva002A7461 (0x002A7461)
//    is the cap: +0xC + +4 plus the points of every entry whose filter accepts the
//    player (ObjectFilter::isValid 0x00360CED, Player::rva002AB2D9 0x002AB2D9),
//    clamped to +0x10. rva002A7548 (0x002A7548) is that cap less the points in
//    use (+8); its Int argument is ignored. Player::addCommandPoints 0x002A9B35
//    forwards to this.
#pragma once
#ifndef BFME2_COMMAND_H
#define BFME2_COMMAND_H

#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "Common/Vector.h"
#include "GameLogic/ObjectFilter.h"

class Xfer;

// One bonus entry (12 bytes; its constructor is 0x002A7400).
struct CommandPointsBonus
{
	Int m_points;				// +0x00
	ObjectID m_source;			// +0x04
	ObjectFilter m_filter;		// +0x08
};
// SIZE 0xC

class Command : public Snapshot
{
public:
	Command();					// 0x002AE5EB
	virtual ~Command();			// 0x002AE627 (deleting 0x002AF53E)

	void Init(Int playerIndex, Bool flag);					// 0x002A71BC
	Int rva002A7461();							// 0x002A7461: the cap
	Int getCommandPointsInUse(Int unused);	// 0x002A7389: returns m_pointsInUse (+8) and ignores its argument (f07, ScriptActions 0x003C2CD8; accepted batch 32)
	Int rva002A7548(Int);							// 0x002A7548: cap less the points in use
	void RemoveUpgradableCommandPointsFromBonus(Int points, ObjectID source);	// 0x002A7611
	void AddUpgradableCommandPointsToBonus(Int points, ObjectID source, const void *filter);	// 0x002A77C1

protected:
	virtual void loadPostProcess() {}		// slot 1, the shared empty body 0x000B3FD0
	virtual const char *GetSnapshotName() const;	// slot 2, 0x002AE621
	virtual void xfer(Xfer *xfer);			// slot 3, 0x002A7818

public:
	Int m_commandPointsTotal;			// +0x04 (WB; 100 from the constructor)
	Int m_pointsInUse;				// +0x08
	Int m_commandPointBonus;			// +0x0C (WB; xfer version 4)
	Int m_commandPointsMax;			// +0x10 (WB; xfer version 6)
	Int m_playerIndex;				// +0x14 (Init)
	Int m_unk0x18;					// +0x18 (xfer version 2)
	Int m_unk0x1C;					// +0x1C (xfer version 3)
	_STL::vector<CommandPointsBonus> m_bonuses;	// +0x20
	Bool m_unk0x2C;					// +0x2C (xfer version 7)
};
// SIZE 0x30

#endif
