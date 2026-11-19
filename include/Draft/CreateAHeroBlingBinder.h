// DRAFT (h05 fpgen): CreateAHeroBlingBinder from retail's field-parse table (VA 0x00DB9F68+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x14; bases and vptr, if any, are the leading gap.
// Unresolved members: none.
#pragma once
#ifndef BFME2_CREATE_A_HERO_BLING_BINDER_H
#define BFME2_CREATE_A_HERO_BLING_BINDER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"


class CreateAHeroBlingBinder
{
public:

	NameKeyType m_groupName;                               // +0x0 GroupName
	AsciiString m_labelTag;                                // +0x4 LabelTag
	AsciiString m_descriptionTag;                          // +0x8 DescriptionTag
	UnsignedInt m_uiSlot;                                  // +0xC UISlot
	Int m_blingType;                                       // +0x10 BlingType; index into a name list (enum)
};
typedef char CreateAHeroBlingBinder_off_m_groupName[(offsetof(CreateAHeroBlingBinder, m_groupName) == 0x0) ? 1 : -1];
typedef char CreateAHeroBlingBinder_off_m_labelTag[(offsetof(CreateAHeroBlingBinder, m_labelTag) == 0x4) ? 1 : -1];
typedef char CreateAHeroBlingBinder_off_m_descriptionTag[(offsetof(CreateAHeroBlingBinder, m_descriptionTag) == 0x8) ? 1 : -1];
typedef char CreateAHeroBlingBinder_off_m_uiSlot[(offsetof(CreateAHeroBlingBinder, m_uiSlot) == 0xC) ? 1 : -1];
typedef char CreateAHeroBlingBinder_off_m_blingType[(offsetof(CreateAHeroBlingBinder, m_blingType) == 0x10) ? 1 : -1];

#endif
