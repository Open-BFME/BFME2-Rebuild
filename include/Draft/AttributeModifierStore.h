// AttributeModifierStore.h: the table of attribute modifiers named by the INI (TheAttributeModifierStore, VA
// 0x00DFE1D4). Partial: only the two members SpecialPowerModule::rva0049402B (0x0049402B) calls are declared;
// the unit's own rows (AttributeModifierStore.cpp) still have no bodies.
#pragma once
#ifndef BFME2_ATTRIBUTEMODIFIERSTORE_H
#define BFME2_ATTRIBUTEMODIFIERSTORE_H

#include <vector>
#include "Common/GameType.h"

// One attribute modifier the store hands out by index: rva00214713 compares its +0x14 with a name key and
// getDuration 0x00214738 returns its +0x18 (SpecialPowerModule::rva0049402B reads it back as an int).
class Object;
struct AttributeModifierEntry
{
	UnsignedByte m_unk0x00[0x14];
	Int m_nameKey;													// +0x14
	Int m_duration;													// +0x18
	void *rva0040327B(Object *object);								// 0x0040327B (map: placeholder owner Rva0040327B): the FX started on an object, getFX 0x002147A1 forwards to it
	void *rva004032D3(Object *object);								// 0x004032D3: the end FX, getEndFX 0x002147D1 forwards to it
};

class AttributeModifierStore
{
public:
	Int rva00214713(Int nameKey);	// 0x00214713: the modifier's index for a name key
	void *getDuration(Int index);	// 0x00214738: the modifier's duration (retail reads it back as an int)
	void *getFX(Int index, Object *object);		// 0x002147A1
	void *getEndFX(Int index, Object *object);	// 0x002147D1

	UnsignedByte m_unk0x00[0xC];
	_STL::vector<AttributeModifierEntry *> m_modifiers;				// +0x0C the modifier table (rva00214713 and getDuration walk it)
};

extern AttributeModifierStore *TheAttributeModifierStore;	// VA 0x00DFE1D4

#endif
