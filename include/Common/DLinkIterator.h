// DLinkIterator.h: the engine's intrusive doubly-linked lists. A class that owns a
// list keeps a head pointer; each member keeps prev/next links and the
// dlink_next_<LIST>/dlink_prependTo_<LIST>/dlink_removeFrom_<LIST>/
// dlink_isInList_<LIST> accessors (Object's for TeamMemberList: 0x0046F7F8,
// 0x0028A6E7, 0x0028A704, 0x0028A6C7; Team's for TeamInstanceList: 0x005C4AF5,
// 0x0039D429, 0x0039D440, 0x0039D40F); the owner's iterate_<LIST> returns a
// DLINK_ITERATOR built from its head and &OBJ::dlink_next_<LIST>.
// Target facts (code RVAs):
//  - DLINK_ITERATOR<Object>: ctor 0x0026350D (cur, member pointer by value)
//    stores cur at +0x00 and the member pointer at +0x08; advance 0x00263526
//    calls through it when cur is not null. +0x04 is never written. Object's
//    member pointers are 12 bytes (fn, adjustor, vbtable index: Object has a
//    virtual base, vbptr at +0x68), so the iterator is 0x14 bytes.
//  - DLINK_ITERATOR<Team>: advance 0x00203C93 uses the 8-byte (fn, adjustor)
//    form, Team's multiple-inheritance shape.
//  - Team::iterate_TeamMemberList 0x00263864 (COMDAT) returns the iterator over
//    the head at Team +0x38.
//  - The ctor, advance and iterate_ bodies are COMDATs: callers either call
//    them or expand them; done() and cur() are read inline by the callers
//    (`cmp [it],0`, `mov reg,[it]`).
// The iterator only matches retail's size once OBJ's real inheritance is
// declared (Object's virtual base, Team's second base).
#pragma once
#ifndef BFME2_DLINKITERATOR_H
#define BFME2_DLINKITERATOR_H

#include "Common/GameType.h"

template <class OBJ>
class DLINK_ITERATOR
{
public:
	typedef OBJ *(OBJ::*GetNextFunc)() const;

	// Inline with out-of-line COMDAT copies (Object: ctor 0x0026350D, advance
	// 0x00263526; Team: advance 0x00203C93). Team.cpp's TeamPrototype loops
	// (countTeamInstances 0x0039D954, findTeamByID 0x0039D92A, xfer 0x003A3E74)
	// expand Team's ctor and advance in place, while Team::healAllObjects
	// 0x0039DCE9 in the same unit calls Object's advance out of line.
	DLINK_ITERATOR() {}	// retail's team loops declare the iterator first and assign it from iterate_TeamMemberList (ScriptActions 0x003C9A80: one call, then `rep movsd` x6)
	DLINK_ITERATOR(OBJ *cur, GetNextFunc getNextFunc) : m_cur(cur), m_getNextFunc(getNextFunc) {}
	void advance()
	{
		if (m_cur)
			m_cur = (m_cur->*m_getNextFunc)();
	}
	Bool done() const { return m_cur == 0; }
	OBJ *cur() const { return m_cur; }

private:
	OBJ *m_cur;					// +0x00
	void *m_unk0x04;			// +0x04 never written by the ctor or iterate_
	GetNextFunc m_getNextFunc;	// +0x08
};

// Retail instantiated DLINK_ITERATOR<Object> while Object was only forward-declared (Team.h): MSVC then gives the
// member pointer its unspecified-inheritance 16-byte form and the iterator 24 bytes (ScriptActions 0x003C5C93 copies it with
// `push 6; pop ecx; rep movsd`). Measured: sizeof == 24 here, 20 once Object is complete (f07 probe).
class Object;
typedef char DLinkIteratorObjectPin[sizeof(DLINK_ITERATOR<Object>) == 24 ? 1 : -1];

#endif
