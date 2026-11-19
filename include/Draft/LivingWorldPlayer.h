// LivingWorldPlayer.h: one player's War of the Ring state. Only the upgrade mask and the
// revival-unit list are recovered.
//
// Target facts (retail RVAs):
//  - AddUpgrade 0x002E1588 and RemoveUpgrade 0x002E15BC take an upgrade template as
//    void *. Only a player-kind template (m_type == 0) is considered. AddUpgrade
//    with its flag set also needs m_persistsInCampaign (+0x78).
//  - The mask is a BitFlags set at +0x1D8, indexed by the template's mask index
//    (+0x38, UpgradeTemplate::getMaskIndex).
//  - The revival-unit vector is at +0x1A8 (begin +0x1A8, end +0x1AC). Its records are
//    0xD8 bytes each with an int id at +0xAC (the search 0x002E12F3 reads it).
//  - Still unrecovered: the base class, the total size, the record type's name, the
//    mask width (BitFlags<32> here is the minimum the two rows need; set and clear do
//    no bounds check, so their bytes do not depend on it) and the members between
//    +0x00 and +0x1A8 and between +0x1B4 and +0x1D8.
#pragma once
#ifndef BFME2_LIVINGWORLDPLAYER_H
#define BFME2_LIVINGWORLDPLAYER_H

#include "Common/AsciiString.h"
#include "Common/BitFlags.h"
#include "Common/Vector.h"

// One revival-unit record: 0xD8 bytes, the id at +0xAC. Its name is not recovered.
struct RevivalUnitRecord
{
	unsigned char m_unk0x00[0xAC];
	int m_id;				// +0xAC
	unsigned char m_unk0xB0[0xD4 - 0xB0];
	AsciiString m_name;			// +0xD4 (compared by 0x002E199E)
};

// The key of 0x002E199E: the name sits at +0x64 of the argument.
struct RevivalLookupKey
{
	unsigned char m_unk0x00[0x64];
	AsciiString m_name;			// +0x64
};

class LivingWorldArmy;
class LivingWorldPlayer
{
public:
	void AddUpgrade(void *upgrade, bool persistentOnly);	// 0x002E1588
	int rva002E0C2B();	// 0x002E0C2B: the player's money-scaling level in percent (Player::ScaleMoney 0x002A9E36)
	void RemoveUpgrade(void *upgrade);			// 0x002E15BC
	bool rva002E12F3(int id);				// 0x002E12F3: true when a revival record carries the id
	int GetNumRegionsOwned();				// 0x002E1001: regions whose owner id is this player's id
	int rva002E199E(const void *key);			// 0x002E199E: id of the first record with the key's name
	bool rva002E0B30();					// 0x002E0B30 (LivingWorldLogic::rva002B4B83 calls it for each player)
	LivingWorldArmy *findArmy(int id);			// 0x002E0A9F: the m_armies entry whose +0x20 id matches (LivingWorldLogic::findPlayerArmy 0x002B488E asks each player)
	void removeArmy(LivingWorldArmy *army);		// 0x002E10BD: erase it from m_armies, then GameLogic::rva0023D007(army +0x78)
	void rva002E10BD(LivingWorldArmy *army);		// 0x002E10BD (LivingWorldLogic::destroyArmy 0x002B5B55 passes the army being destroyed)

private:
	unsigned char m_unk0x00[0x14];				// +0x00 not yet recovered
	int m_id;						// +0x14 (the player id)
	friend class LivingWorldLogic;
	friend class LivingWorldArmy;
	friend class LivingWorldBattle;
	unsigned char m_unk0x18[0x20 - 0x18];		// +0x18 not yet recovered
	AsciiString m_name;					// +0x20: LivingWorldLogic::findPlayer(const AsciiString &) 0x002B6AEC compares it
	unsigned char m_unk0x24[0x34 - 0x24];		// +0x24 not yet recovered
	int m_team;						// +0x34: the key LivingWorldLogic collects into a set (0x002B693F, 0x002B69A6)
	unsigned char m_unk0x38[0x44 - 0x38];		// +0x38 not yet recovered
	int m_kind;						// +0x44: 0 for a human player (LivingWorldLogic::getNumHumanPlayers 0x002B5256 counts those)
	unsigned char m_unk0x48[0x1A8 - 0x48];		// +0x48 not yet recovered
	_STL::vector<RevivalUnitRecord> m_revivalUnits;		// +0x1A8
	unsigned char m_unk0x1B4[0x1B8 - 0x1B4];		// +0x1B4 not yet recovered
	_STL::vector<LivingWorldArmy *> m_armies;		// +0x1B8 (findArmy 0x002E0A9F, rva002E0B30 0x002E0B30, removeArmy 0x002E10BD)
	unsigned char m_unk0x1C4[0x1D8 - 0x1C4];		// +0x1C4 not yet recovered
	BitFlags<32> m_upgradeMask;				// +0x1D8 (width unrecovered)
	unsigned char m_unk0x1DC[0x3C4 - 0x1DC];		// +0x1DC not yet recovered
	bool m_unk0x3C4;					// +0x3C4: 0x002B693F skips a player whose flag is set (ctor 0x002E28CF clears it)
};

#endif
