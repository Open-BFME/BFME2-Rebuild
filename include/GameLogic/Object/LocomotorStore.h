// LocomotorStore.h: the locomotor template registry (TheLocomotorStore, VA 0x00DFDC5C).
// Target facts (retail game.dat):
// - SubsystemInterface (0xC) then an STLport map<NameKeyType, LocomotorTemplate *>
//   at +0xC: findLocomotorTemplate(NameKeyType) 0x001E6FEA runs _M_find on this+0xC
//   and returns the node's value (+0x14); reset 0x001E681D (virtual) walks the same
//   map, calls deleteOverrides on each template and erases the entries it returns
//   null for.
// - findLocomotorTemplate(const AsciiString &) 0x001E7010 goes through the global
//   TheLocomotorStore, not this: nameToKey, then the NameKeyType overload.
// - newLocomotor 0x001E4A81 allocates 0xB0 and runs Locomotor's ctor; newOverride
//   0x001E520C; parseLocomotorTemplateDefinition 0x001E8A94 allocates 0x154-byte
//   templates.
// - The map's ??0LocomotorStore 0x0022078E (no vptr store) is not this class's ctor;
//   no allocation site is seen, so SIZE 0x18 is SubsystemInterface plus the map.
// WorldBuilder: "LocomotorStore::reset", "LocomotorStore::parseLocomotorTemplateDefinition".
#pragma once
#ifndef BFME2_LOCOMOTORSTORE_H
#define BFME2_LOCOMOTORSTORE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/SubsystemInterface.h"
#include "Common/StlMap.h"

class AsciiString;
class INI;
class Locomotor;
class LocomotorTemplate;

class LocomotorStore : public SubsystemInterface
{
public:
	virtual void reset();													// 0x001E681D

	LocomotorTemplate *findLocomotorTemplate(Int namekey);					// 0x001E6FEA; the map row takes an int
	LocomotorTemplate *findLocomotorTemplate(const AsciiString &name);		// 0x001E7010
	Locomotor *newLocomotor(const LocomotorTemplate *tmpl, Bool flag);		// 0x001E4A81
	LocomotorTemplate *newOverride(LocomotorTemplate *tmpl);				// 0x001E520C
	static void parseLocomotorTemplateDefinition(INI *ini);					// 0x001E8A94

	// Keyed by int: findLocomotorTemplate 0x001E6FEA (map row `H`) hands its own
	// argument's address to _M_find, so the key type is the parameter type.
	typedef _STL::map<Int, LocomotorTemplate *> LocomotorTemplateMap;
	LocomotorTemplateMap m_locomotorTemplates;								// +0x0C
};

extern LocomotorStore *TheLocomotorStore;

#endif
