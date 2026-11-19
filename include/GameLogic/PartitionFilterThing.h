#pragma once
#include "GameLogic/PartitionFilter.h"
class ThingTemplate;
// Zero Hour's PartitionFilterThing: accepts the objects whose template is equivalent to one given template (or,
// with m_match false, every other object). Target facts: vtable 0x0081FDEC's allow 0x00261750 (this+8's
// isEquivalentTo 0x0033BB04 asked about the candidate's template at Thing +4, compared with the byte at +0xC).
// The name is Zero Hour's: the constructor shape (template, match) is the one the rows ScriptActions 0x003C24F0 /
// 0x003C2662 / 0x003C4625 store inline.
class PartitionFilterThing : public PartitionFilter
{
public:
 PartitionFilterThing(const ThingTemplate *thing, bool match) : m_tThing(thing), m_match(match) {}
 virtual bool allow(Object *);	// 0x00261750
 const ThingTemplate *m_tThing;	// 0x08
 bool m_match;	// 0x0C
};
// SIZE 0x10
