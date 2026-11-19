// DRAFT (h05 fpgen): CreateAHeroBling from retail's field-parse table (VA 0x00DB9F18+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x10; bases and vptr, if any, are the leading gap.
// Unresolved members: none.
#pragma once
#ifndef BFME2_CREATE_A_HERO_BLING_H
#define BFME2_CREATE_A_HERO_BLING_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"


class CreateAHeroBling
{
public:

	AsciiString m_nameTag;                                 // +0x0 NameTag
	AsciiString m_descriptionTag;                          // +0x4 DescriptionTag
	AsciiString m_blingUpgradeName;                        // +0x8 BlingUpgradeName
	NameKeyType m_groupName;                               // +0xC GroupName
};
typedef char CreateAHeroBling_off_m_nameTag[(offsetof(CreateAHeroBling, m_nameTag) == 0x0) ? 1 : -1];
typedef char CreateAHeroBling_off_m_descriptionTag[(offsetof(CreateAHeroBling, m_descriptionTag) == 0x4) ? 1 : -1];
typedef char CreateAHeroBling_off_m_blingUpgradeName[(offsetof(CreateAHeroBling, m_blingUpgradeName) == 0x8) ? 1 : -1];
typedef char CreateAHeroBling_off_m_groupName[(offsetof(CreateAHeroBling, m_groupName) == 0xC) ? 1 : -1];

#endif
