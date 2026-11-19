// AIAttackPrioritySet.h: the AttackPriority INI block's sets, headed by TAiData +0xFC.
// Target facts: ScriptEngine::getAttackInfo 0x00205DDD walks the set list from [[TheAI+0x18]+0xFC]:
// compares [set+4] with the name, follows [set+0x10] to the next set; on a hit it copies the default
// priority [set+8] and walks the entry list at [set+0xC] (the list's sentinel pointer is the member itself,
// then node->next at +0, prev at +4, the element from +8: template name at node +8, priority at node +0xC).
// Decl_queue f02 (re-post of 15:1x), ACCEPTED k11.
#pragma once
#ifndef BFME2_AIATTACKPRIORITYSET_H
#define BFME2_AIATTACKPRIORITYSET_H

#include "Common/AsciiString.h"
#include "Common/GameType.h"
#include "Common/StlList.h"

struct AIAttackPriorityEntry
{
	AsciiString m_templateName;	// node +0x08
	Int m_priority;			// node +0x0C
};
typedef char AIAttackPriorityEntry_size[(sizeof(AIAttackPriorityEntry) == 8) ? 1 : -1];

struct AIAttackPrioritySet
{
	Int m_unk0x00;						// +0x00
	AsciiString m_name;					// +0x04
	Int m_defaultPriority;					// +0x08
	_STL::list<AIAttackPriorityEntry> m_entries;		// +0x0C (by value: one pointer to the sentinel node)
	AIAttackPrioritySet *m_next;				// +0x10
};
typedef char AIAttackPrioritySet_size[(sizeof(AIAttackPrioritySet) == 0x14) ? 1 : -1];

#endif
