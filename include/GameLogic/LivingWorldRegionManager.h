// The region manager's constructor 0x0021042D installs its Snapshot table
// at +0 (0x007E4358) and the turn observer at +4 (0x007E4350). It registers
// the observer in the LivingWorldLogic list. The allocation is 0x40 bytes.
#pragma once

#include "Common/Snapshot.h"
#include "Common/Vector.h"
#include "GameLogic/LivingWorldTurnObserver.h"

class LivingWorldLogic;
class LivingWorldRegion;
class LivingWorldBattle;

// The visitor that EnumeratePendingBattles (0x0020E7BD) calls for each pending battle until one returns false.
// Only slot 0 is recovered: the call site 0x0020E7CF calls [vtable+0] with the battle as its argument.
class PendingBattleVisitor {
public:
	virtual bool call(LivingWorldBattle *battle) = 0; // slot 0 (the call site 0x0020E7CF)
};
// The table at +0x8 of the manager; its region vector is at +0x2C. Its own name is not recovered.
struct RegionOwnershipTable {
	unsigned char m_unk0x00[0x2C];
	_STL::vector<LivingWorldRegion *> m_regions; // +0x2C
};
class LivingWorldRegionManager : public Snapshot, public LivingWorldTurnObserver
{
public:
	LivingWorldRegionManager(LivingWorldLogic *logic); // 0x0021042D
	virtual ~LivingWorldRegionManager(); // 0x00210B38
	virtual void loadPostProcess();
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void BeginTurn(Int, Int); // Observer slot 0, 0x0020EB07.
	virtual void OnTurnEnding(Int, Int); // Observer slot 1, 0x0020FB41.

	RegionOwnershipTable *m_table; // +0x8
	const _STL::vector<LivingWorldRegion *> *getRegions() const { return m_table ? &m_table->m_regions : NULL; }
	unsigned char m_unk0x0C[0x10 - 0x0C];
	int m_selectedBattleKey; // +0x10 (getSelectedBattle 0x0020E6C0 looks it up in both lists)
	_STL::vector<LivingWorldBattle *> m_pendingBattles; // +0x14 (SelectPendingBattle 0x0020E794, findPendingBattleByID 0x0020E501)
	_STL::vector<LivingWorldBattle *> m_completedBattles; // +0x20 (findCompletedBattleByKey 0x0020E5FD)
	LivingWorldBattle *findPendingBattleByID(int id); // 0x0020E501: battle +0x30
	LivingWorldBattle *findPendingBattleByRegionID(int regionID); // 0x0020E53D: battle region +0x12C
	LivingWorldBattle *findPendingBattleByRegion(LivingWorldRegion *region); // 0x0020E57F: battle +0x24
	LivingWorldBattle *findPendingBattleByKey(int key); // 0x0020E5BB: battle +0x34, 0 for key 0
	LivingWorldBattle *findCompletedBattleByKey(int key); // 0x0020E5FD: the same over +0x20
	LivingWorldBattle *getSelectedBattle(); // 0x0020E6C0: the +0x10 key, pending first
	LivingWorldBattle *SelectPendingBattle(); // The first pending battle whose rva003F4831 is positive
	LivingWorldRegion *rva0020EAF6(int index); // (LivingWorldLogic::getBattleRegion 0x002B2B2D passes the battle region index)
	void UpdateBattleMarkers();
	void SetRegionReinforcements(Int regionIndex, Int a, Int b, Int c, Int d, Int e, Int f); // 0x0020EEC8
	LivingWorldRegion *rva0020FAEA(float *pos, LivingWorldRegion *hint); // (LivingWorldArmy::getRegion 0x00318C32: the region at a position, given the last one)
	LivingWorldRegion *rva0020EEF4(int id); // (LivingWorldLogic::getRegionByID 0x002B2579 forwards a non-zero id)
	void EnumeratePendingBattles(PendingBattleVisitor &visitor) const;
	void EnumerateCompletedBattles(PendingBattleVisitor *visitor); // 0x0020E7E8: the completed battle list, stopping when the visitor returns false

private:
	Int m_unk0x2C;
	Int m_unk0x30;
	UnsignedByte m_unk0x34[0xC]; // Vector initialized by 0x00211E58, element type unresolved.
};

typedef char LivingWorldRegionManagerSize[(sizeof(LivingWorldRegionManager) == 0x40) ? 1 : -1];
