// AssaultTransportAIUpdate.h: SAGE assault transport AI (Zero Hour AssaultTransportAIUpdate.h names).
// Target facts:
// - Constructor 0x0048F5A3 calls AIUpdateInterface's (0x0026E9BD), stores the
//   class's tables 0x00C4CCA0 (+0), 0x00C4CC90 (+0x10), 0x00C4CC8C (+0x20), keeps
//   AIUpdateInterface's 0x00BFA3C0 (+0xC) and 0x00C52F88 (+0x24), and stores
//   0x00C4CC88 at +0x3E4: the one-slot AssaultTransportAIInterface sits right
//   after the 0x3E4-byte AIUpdateInterface. Destructor 0x0048F3E0 re-stores them
//   and jumps to ~AIUpdateInterface.
// - beginAssault 0x0048F45C (`const`, slot 0 of 0x00C4CC88) takes the interface
//   as this and stores the target's ID at this+0x4C, i.e. +0x430.
// - xfer 0x0048F4AC: AIUpdateInterface::xfer, Version1, member count +0x43C,
//   per member ObjectID +0x3E8[i] and Bool +0x410[i], Coord3D +0x424, ObjectID
//   +0x430, state +0x434, UnsignedInt +0x438, Bools +0x440/+0x441/+0x442.
//   The reset helper 0x0048F365 also clears the Bool array at +0x41A.
#pragma once
#ifndef BFME2_ASSAULTTRANSPORTAIUPDATE_H
#define BFME2_ASSAULTTRANSPORTAIUPDATE_H

#include "GameLogic/Module/AIUpdate.h"

enum
{
	MAX_TRANSPORT_SLOTS = 10	// 0x3E8..0x410 ObjectIDs, 0x410..0x41A Bools
};

// xfer'd through an Int temporary; values beyond IDLE are not recovered.
enum AssaultStateTypes
{
	IDLE
};

class AssaultTransportAIInterface
{
public:
	virtual void beginAssault(const Object *designatedTarget) const = 0;	// slot 0
};
// SIZE 0x4

class AssaultTransportAIUpdate : public AIUpdateInterface, public AssaultTransportAIInterface
{
public:
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
	AssaultTransportAIUpdate(Thing *thing, const ModuleData *moduleData);	// 0x0048F5A3
	virtual ~AssaultTransportAIUpdate();						// 0x0048F3E0

	virtual NameKeyType getModuleNameKey() const;				// 0x0048F411
	virtual void beginAssault(const Object *designatedTarget) const;	// 0x0048F45C
	virtual Bool isIdle() const;	// AIUpdateInterface slot 110: 0x0048F46D, a `jmp` to the base 0x002645FF (table 0x0084CCA0)
	virtual AssaultTransportAIInterface *getAssaultTransportAIInterface() { return this; }	// slot 102, 0x004A9B51 (shared)
	virtual const AssaultTransportAIInterface *getAssaultTransportAIInterface() const { return this; }	// slot 101
	virtual void aiDoCommand(const AICommandParms *parms);		// 0x0048F7A7
	virtual UpdateSleepTime update();							// 0x0048F828

	Bool isAttackPointless() const;								// 0x0048F61A
	Bool isMemberWounded(const Object *member) const;			// 0x0048F64E: health ratio below MembersGetHealedAtLifeRatio (Zero Hour's name)
	void retrieveMembers();										// 0x0048F690: orders every member outside back in (aiEnter) (Zero Hour's name)
	void reset();												// 0x0048F365, the constructor calls it after setting 10 members

protected:
	virtual void xfer(Xfer *xfer);								// 0x0048F4AC

	ObjectID m_memberIDs[MAX_TRANSPORT_SLOTS];			// +0x3E8
	Bool m_memberHealing[MAX_TRANSPORT_SLOTS];			// +0x410
	Bool m_newMember[MAX_TRANSPORT_SLOTS];				// +0x41A
	Coord3D m_attackMoveGoalPos;						// +0x424
	mutable ObjectID m_designatedTarget;				// +0x430
	AssaultStateTypes m_state;							// +0x434
	UnsignedInt m_framesRemaining;						// +0x438
	Int m_currentMembers;								// +0x43C
	Bool m_isAttackMove;								// +0x440
	Bool m_isAttackObject;								// +0x441
	Bool m_newOccupantsAreNewMembers;					// +0x442
protected:
	virtual void loadPostProcess() { AIUpdateInterface::loadPostProcess(); }	// retail slot 0x0049A7EE: a jmp to AIUpdateInterface's, ICF-folded across classes (h05)
protected:
	virtual const char *GetSnapshotName() const;	// slot 2 of the class table, 0x0048F456 (h05 r9)
};
// SIZE 0x444

#endif
