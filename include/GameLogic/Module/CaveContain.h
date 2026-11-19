// CaveContain.h: a contain whose occupants live in a shared cave (tunnel
// network) owned by TheCaveSystem. SIZE 0x10C.
//
// Target facts: the constructor 0x00466630 calls OpenContain's 0x004649F8,
// stores CaveInterface's own table 0x00C6FFFC (two __purecall slots) at +0xFC,
// zeroes +0x104/+0x108, installs CaveContain's tables (+0xFC 0x00C43A54) and
// sets the byte +0x100 to 1; the factory 0x0024B797 allocates 0x10C.
// xfer 0x00466577 writes the bool +0x100, the int +0x104 and the team ID of
// the Team* at +0x108 (TeamFactory::findTeamByID on load). The CaveInterface
// table holds tryToSetCaveIndex 0x004667D1 (slot 0) and a folded body.
// shouldDoOnBuildComplete 0x00466467 returns +0x100; onCreate 0x00466780 copies
// the module data's CaveIndex (+0x98) to +0x104; every tunnel query reaches
// TheCaveSystem->getTunnelTrackerForCaveIndex(+0x104).
// Donor facts: Zero Hour's CaveContain/CaveInterface names (m_needToRunOnBuildComplete,
// m_caveIndex, m_originalTeam, tryToSetCaveIndex, setOriginalTeam).
#pragma once
#ifndef BFME2_CAVECONTAIN_H
#define BFME2_CAVECONTAIN_H

#include "GameLogic/Module/OpenContain.h"
#include "GameLogic/Module/CaveContainModuleData.h"

class Team;

class CaveInterface
{
public:
	virtual void tryToSetCaveIndex(Int newIndex) = 0;					// 0
	virtual void setOriginalTeam(Team *oldTeam) = 0;					// 1
};

class CaveContain : public OpenContain, public CaveInterface		// CaveInterface at +0xFC
{
public:
	CaveContain(Thing *thing, const ModuleData *moduleData);			// 0x00466630

	const CaveContainModuleData *getCaveContainModuleData() const
	{
		return (const CaveContainModuleData *)getModuleData();
	}

	virtual NameKeyType getModuleNameKey() const;						// 0x004663FF
	const char *name() const;									// 0x00466444

	virtual CaveInterface *getCaveInterface();

	// ContainModuleInterface
	virtual void addToContainList(Object *obj);							// 0x0046646E
	virtual void iterateContained(ContainIterateFunc func, void *userData, UnsignedInt flags);	// 0x00466486
	virtual void onContaining(Object *obj, Bool wasSelected);			// 0x004664BB
	virtual Bool isValidContainerFor(Object *obj, Bool checkCapacity, Bool flag);	// 0x004664E1
	virtual Bool containSlot52() { return false; }
	virtual Int getContainMax() const;									// 0x0046651C
	virtual ContainmentListView getList();								// 0x00466534 (ContainModuleInterface slot 70)
	virtual void removeFromContain(Object *obj, Bool exposeStealthUnits);	// 0x004666C9
	virtual void onRemoving(Object *obj);								// 0x004668DC
	virtual void recalcApparentControllingPlayer();						// 0x00466977
	virtual void removeAllContained(Bool exposeStealthUnits);			// 0x00466A50

	// DieModuleInterface
	virtual void onDie(const DamageInfo *damageInfo);					// 0x00466724

	// CreateModuleInterface
	virtual void onCreate();											// 0x00466780
	virtual void onBuildComplete();										// 0x00466790
	virtual Bool shouldDoOnBuildComplete() const { return m_needToRunOnBuildComplete; }	// 0x00466467

	// CaveInterface
	virtual void tryToSetCaveIndex(Int newIndex);						// 0x004667D1
	virtual void setOriginalTeam(Team *oldTeam) { m_originalTeam = oldTeam; }	// slot 1 of the CaveInterface table: the shared setter 0x005F69C4

	// 0x00466848 (map placeholder name): onRemoving calls it with m_originalTeam
	// and false when the last object leaves a cave whose occupant had a team.
	void rva00466848(Team *team, Bool flag);

protected:
	virtual ~CaveContain();												// 0x004663B2
	virtual const char *GetSnapshotName() const;						// 0x00466444
	virtual void xfer(Xfer *xfer);										// 0x00466577

public:
	Bool m_needToRunOnBuildComplete;									// 0x100
	Int m_caveIndex;													// 0x104
	Team *m_originalTeam;												// 0x108
protected:
	virtual void loadPostProcess() { OpenContain::loadPostProcess(); }	// retail slot 0x004783D7: a jmp to OpenContain's, ICF-folded across classes (h05)
};
// SIZE 0x10C

typedef char CaveContain_off_m_needToRunOnBuildComplete[(offsetof(CaveContain, m_needToRunOnBuildComplete) == 0x100) ? 1 : -1];
typedef char CaveContain_off_m_caveIndex[(offsetof(CaveContain, m_caveIndex) == 0x104) ? 1 : -1];
typedef char CaveContain_size[(sizeof(CaveContain) == 0x10C) ? 1 : -1];

#endif
