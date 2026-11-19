// SciencePurchasePointTracker.h: a player's rank, skill points and science purchase
// points. SIZE 0x2C. Player carries one as its base at +0x08 (Player.h).
// WorldBuilder's path strings name the unit Common/System/SciencePurchasePointTracker.cpp,
// and its assertions spell getSciencePurchasePoints(), getSkillPoints(), getRankLevel()
// and SciencePurchasePointTracker::setRankLevel.
// Target facts (code RVAs, data VAs):
//  - Constructor 0x00380523 (Player's ctor calls it on +0x08): table 0x00BFDC30 at +0;
//    zeroes +0x04, +0x08, +0x1C, +0x24, +0x28; +0x0C = 0.0f; +0x10 = the float constant at
//    VA 0x00BBB8D8; +0x14 = +0x18 = 1; +0x20 = 0x7FFFFFFF; then, when
//    TheExperienceLevelSystem is set, +0x08 = FindExperienceScalarTableByName (0x00288AF2)
//    of a name literal; then resetRank().
//  - Table 0x00BFDC30, six slots: deleting dtor 0x002A9844; 0x0038037C setRankLevel (clamps
//    the level to 1..the rank count and to the limit, calls slot 3 when it drops, slot 4 for
//    each rank gained, adds each rank's +0x34 to +0x1C); 0x0038027B adds its argument to +0x1C
//    and clamps at 0; 0x0038028B resetRank (+0x28 = 0, level 1, +0x0C = 0, +0x1C = the
//    intrinsic points from +0x04's +4 plus rank 1's grant); slot 4 0x0047A69C (`ret 4`,
//    empty) and slot 5 0x0047A699 (returns false), which Player overrides.
//  - Player's overrides (compiled with this at Player+0x08): setRankLevel 0x002A9FA0,
//    addSciencePurchasePoints 0x002A9F5E, resetRank 0x002AE318, slot 4 0x002AD7E9 (adds
//    the rank's +0x34 to the score keeper and grants every science in its +0x38 vector),
//    slot 5 0x002A9F53.
//  - Slots 4 and 5 carry role names: no string names them.
#pragma once
#ifndef BFME2_SCIENCEPURCHASEPOINTTRACKER_H
#define BFME2_SCIENCEPURCHASEPOINTTRACKER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Science.h"

class ExperienceScalarTable;
class RankInfo;

class SciencePurchasePointTracker
{
public:
	SciencePurchasePointTracker();						// 0x00380523
	virtual ~SciencePurchasePointTracker();				// slot 0 (deleting 0x002A9844)

	virtual Bool setRankLevel(Int rankLevel);			// slot 1, 0x0038037C
	virtual void addSciencePurchasePoints(Int delta);	// slot 2, 0x0038027B
	virtual void resetRank();							// slot 3, 0x0038028B
	virtual void grantRankBonuses(const RankInfo *rank) {}	// slot 4, 0x0047A69C (role name)
	virtual Bool usesRankLevelLimitOverride() const { return false; }	// slot 5, 0x0047A699 (role name)

	Int getSciencePurchasePoints() const { return m_sciencePurchasePoints; }
	Real getSkillPoints() const { return m_skillPoints; }
	Int getRankLevel() const { return m_rankLevel; }
	Int getRankLevelLimit();	// 0x003802DF (the map spells it ?getRankLevelLimit@SciencePurchasePointTracker@@QAEHXZ, non-const; ScriptConditions 0x003E4519 compares the rank level with it)

	void *m_unk0x04;							// +0x04 resetRank takes the intrinsic points from its +4
	ExperienceScalarTable *m_skillPointsScalarTable;	// +0x08 found by name in the ctor
	Real m_skillPoints;							// +0x0C
	Real m_skillPointsModifier;					// +0x10 (the ctor's constant)
	Int m_rankLevel;							// +0x14
	Int m_unk0x18;								// +0x18 (1 after the ctor and resetRank)
	Int m_sciencePurchasePoints;				// +0x1C
	Int m_rankLevelLimit;						// +0x20 (0x7FFFFFFF)
	Int m_unk0x24;								// +0x24
	Int m_unk0x28;								// +0x28 (resetRank clears it)
};
// SIZE 0x2C
typedef char SciencePurchasePointTrackerSize[(sizeof(SciencePurchasePointTracker) == 0x2C) ? 1 : -1];
typedef char SciencePurchasePointTrackerPoints[(offsetof(SciencePurchasePointTracker, m_sciencePurchasePoints) == 0x1C) ? 1 : -1];

// The interface ScienceStore asks about a player's sciences
// (ScienceStore::playerHasPrereqsForScience 0x001FF47D takes it). Two classes implement it:
// Player at +0x04 (table 0x00BFDF30) and SpellStore's pending-purchase view (Rva0043D16F,
// table 0x00C3D7E8), which answers for its player plus the purchases not yet sent
// ("cost <= m_player->getSciencePurchasePoints() - m_pointsSpent"). Its three pure slots
// (folded table 0x00BE2B78) answer "has this science", "purchase points available" and
// "is this science hidden". Class and method names are role names: no string names them.
class ScienceQueryInterface
{
public:
	virtual Bool queryHasScience(ScienceType science) const = 0;			// slot 0
	virtual Int queryPurchasePoints() const = 0;							// slot 1
	virtual void queryScienceHidden(ScienceType science, Bool *hidden) const = 0;	// slot 2
};

#endif
