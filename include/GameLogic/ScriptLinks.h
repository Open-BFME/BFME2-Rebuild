// ScriptLinks.h: the link nodes that name an entry of a ScriptList's tables (see ScriptTable.h).
// Target facts (code RVAs): a link is 0xC bytes (ScriptList 0x003B8337 allocates one): +0x00 the next link of
// its chain, +0x04 the entry number, +0x08 the depth the entry had when the link was taken.  The group chain's
// destructor is 0x003B31DF and the script chain's 0x003B3204 (each deletes the rest of its chain; the two bodies
// are identical but not folded, so the two types are separate), their copy constructors 0x003B3221 / 0x003B3277.
#pragma once
#ifndef BFME2_SCRIPTLINKS_H
#define BFME2_SCRIPTLINKS_H

#include "Common/GameType.h"

struct ScriptGroupLink
{
	ScriptGroupLink *m_next;			// +0x00
	Int m_index;					// +0x04 the entry number in the group table
	Int m_depth;					// +0x08
	~ScriptGroupLink() { delete m_next; }		// 0x003B31DF
};

struct ScriptLink
{
	ScriptLink *m_next;				// +0x00
	Int m_index;					// +0x04 the entry number in the script table
	Int m_depth;					// +0x08
	~ScriptLink() { delete m_next; }		// 0x003B3204
};

// The pair of chain heads a ScriptList (at +0x04) and a ScriptGroup (at +0x04) both start with.  One class serves both:
// the prune walks (0x003B48E4 / 0x003B4982) recurse from a list's base into each group's by passing the group converted
// to this type (null-checked pointer arithmetic), and the destructor is one function, 0x003B4071 (script chain first,
// then the group chain).  The constructor zeroes both words before the owner stores its table; the copy constructor
// 0x003B3FCD copies both chains node by node.
class ScriptListBase
{
public:
	ScriptListBase() : m_firstGroup(0), m_firstScript(0) {}
	ScriptListBase(const ScriptListBase &that);	// 0x003B3FCD
	~ScriptListBase();							// 0x003B4071 (out of line: retail's ~ScriptGroup / ~ScriptList call it; defined in Scripts.cpp)
	void swap(ScriptListBase *other);				// 0x003B33FA: exchanges the two chains

	ScriptGroupLink *m_firstGroup;			// +0x00 the group link chain (WriteGroupDataChunk 0x003B6975)
	ScriptLink *m_firstScript;			// +0x04 the script link chain (WriteScriptDataChunk 0x003B5994)
};

#endif
