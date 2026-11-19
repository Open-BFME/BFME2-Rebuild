// AttackPriorityInfo.h: a named table of attack priorities per thing template, used by
// the script engine's attack-priority sets. SIZE 0x10.
// Target facts (retail game.dat):
//  - vtable 0x0087D... (RVA of the table stored by the ScriptEngine's creator): a Snapshot
//    subclass: slot 0 deleting destructor 0x00358317, slot 1 loadPostProcess (empty,
//    folded), slot 2 the name getter 0x0035701F, slot 3 xfer 0x003583A7.
//  - +4 the set's name, +8 the default priority (an int), +0xC a pointer to a
//    map keyed by the template's final override (as an unsigned) with an int priority,
//    allocated by the first setPriority (0x00358333) and read by getPriority (0x00357CDE).
//  - xfer writes version, name, default, the map's size as an unsigned short, then one
//    (template name, priority) pair per entry; loading looks each name up in
//    TheThingFactory and calls setPriority.
#pragma once
#ifndef BFME2_ATTACKPRIORITYINFO_H
#define BFME2_ATTACKPRIORITYINFO_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Snapshot.h"
#include "Common/StlMap.h"

class Object;
class ThingTemplate;
class Xfer;

class AttackPriorityInfo : public Snapshot
{
public:
	Real getPriority(const Object *source, const Object *target, Bool applyCombatChain) const;	// 0x00357CDE
	void setPriority(const ThingTemplate *tmpl, Int priority);		// 0x00358333
	// 0x00357025: adds the combat-chain bonus; its helpers are not named yet.
	Real doModPriorityByCombatChain(Real priority, const Object *source, const Object *target) const;

	const AsciiString &getName() const { return m_name; }	// the set's name (+0x04): ScriptActions::doTeamApplyAttackPrioritySet 0x003BEDD1 reads it
	friend class ScriptEngine;	// findAttackInfo 0x00205D5D claims a slot (+0x04) and setPriorityDefault 0x002061D7 writes +0x08

protected:
	virtual void loadPostProcess() {}								// slot 1, empty (folded)
	virtual const char *GetSnapshotName() const;					// slot 2, 0x0035701F
	virtual void xfer(Xfer *xfer);									// slot 3, 0x003583A7

private:
	AsciiString m_name;												// 0x04
	Int m_defaultPriority;											// 0x08
	_STL::map<UnsignedInt, Int> *m_priorityMap;						// 0x0C
};
// SIZE 0x10

#endif
