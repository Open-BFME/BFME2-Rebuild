// ScriptTable.h: the two name-keyed tables of a ScriptList (its groups at +0x0C and its scripts at +0x2C).
// Target facts (code RVAs; every one of these is read off retail's table code, not off a donor):
//  - One table is 0x20 bytes: a vector of Int at +0x00 (the entry numbers sorted by name; lower_bound
//    over it is 0x003B573E, 80 bytes), a vector of 0x14-byte entries at +0x0C, the head of the free
//    entry list at +0x18 and the head of the live entry list at +0x1C (both -1 when empty; the
//    constructor is 0x003B761E).  swap 0x003B56A5 exchanges the two vectors and the two heads.
//  - An entry is: +0x00 the next entry in its list (the live list, or the free list when it is
//    free), +0x04 the previous entry in the live list, +0x08 the name, +0x0C a flag byte (set by
//    deallocateEntry 0x003B66D8 and by the table-wide marking passes 0x003B3ECB / 0x003B3EEA,
//    cleared when the name is claimed again by 0x003B7C47), +0x0E the depth (how many versions the
//    entry holds: 0x003B7C47 adds one when it hands the entry out, deallocateEntry removes one),
//    +0x10 the head of the entry's version list.
//  - A version is a node of 4 + sizeof(T) bytes: +0x00 the next (older) version, +0x04 the value
//    (the ScriptGroup copy constructor 0x003B410E runs at node +4 in 0x003B44AB / 0x003B44C7, the
//    ScriptGroup destructor in 0x003B448C; for scripts the Script destructor 0x003B35B1 in
//    0x003B3F5E).  versionAt 0x003B3F09 walks (depth - version) older nodes from the head.
//  - A link (ScriptLinks.h) names an entry by number and depth; it is current while the depth
//    still equals the entry's (ScriptList 0x003B483D / 0x003B489C).
#pragma once
#ifndef BFME2_SCRIPTTABLE_H
#define BFME2_SCRIPTTABLE_H

#include <vector>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "GameLogic/ScriptLinks.h"

// One version of an entry: the chain link every version starts with (the value follows it; see ScriptTable<T>::Version).
struct ScriptTableVersion
{
	ScriptTableVersion *m_older;			// +0x00
};

// One entry of a table's vector (0x14 bytes).  Both tables use the same entry type: retail has one copy of
// vector<entry>'s destructor (0x003B7057), swap (0x00567ECD) and push_back (0x003B76B8) for the two of them.
struct ScriptTableEntry
{
	Int m_next;					// +0x00
	Int m_prev;					// +0x04
	AsciiString m_name;				// +0x08
	Bool m_flagged;					// +0x0C
	Short m_depth;					// +0x0E
	ScriptTableVersion *m_versions;			// +0x10
};

template <class T>
struct ScriptTableNode : public ScriptTableVersion
{
	~ScriptTableNode();
	T m_value;				// +0x04
};

template <class T>
class ScriptTable
{
public:
	typedef ScriptTableNode<T> Version;

	typedef ScriptTableEntry Entry;

	ScriptTable() : m_freeHead(-1), m_liveHead(-1) {}	// 0x003B761E (inline in retail, shared by both tables)
	~ScriptTable() { destroyEntries(&m_entries); }		// 0x003B766B (groups) / 0x003B7167 (scripts)

	static void destroyVersions(Entry *entry);	// 0x003B675B (groups) / 0x003B578E (scripts): deletes every version of one entry
	static void destroyEntries(std::vector<Entry> *entries);	// 0x003B678A (groups) / 0x003B713E (scripts): destroys the entries from the back

	Int find(const AsciiString &name) const;	// 0x003B6633: the entry number of a name, or -1
	T *versionAt(Int index, Int version);		// 0x003B3F09: the value the entry held `depth - version` versions ago
	void flagAll();					// 0x003B3ECB: sets the flag byte of every live entry
	void unflagAll();				// 0x003B3EEA: clears it
	void swap(ScriptTable *other);			// 0x003B56A5
	void clear();					// 0x003B7C1E (groups) / 0x003B7647 (scripts): swaps with a fresh table and lets it go
	void deallocateEntry(Int index);		// 0x003B66D8: drops one version of depth; frees the entry when it holds none
	void popVersion(Int index);			// 0x003B7096 (groups) / 0x003B71B4 (scripts): destroys the newest version, then deallocateEntry
	void flattenAll();				// 0x003B6676 (groups) / 0x003B56DC (scripts): every live entry keeps only its newest version, at depth 1
	void settle();					// 0x003B70CA (groups) / 0x003B71E8 (scripts): flagged live entries lose their newest version, the others are flattened

	// 0x003B573E (80 bytes, shared by both tables): the position of `name` in m_order, or where it would go.
	Int lowerBound(const AsciiString &name) const
	{
		Int lo = 0;
		Int hi = (Int)m_order.size();
		while (hi > lo)
		{
			Int mid = (hi + lo) >> 1;
			Int order = name.compare(m_entries[m_order[mid]].m_name);
			if (order == 0)
				return mid;
			if (order < 0)
				hi = mid;
			else
				lo = mid + 1;
		}
		return lo;
	}

	// The newest value of an entry (the head of its version chain).
	T *newest(Int index) const { return &static_cast<Version *>(m_entries[index].m_versions)->m_value; }

	std::vector<Int> m_order;			// +0x00 entry numbers sorted by name
	std::vector<Entry> m_entries;			// +0x0C
	Int m_freeHead;					// +0x18
	Int m_liveHead;					// +0x1C
};

#endif
