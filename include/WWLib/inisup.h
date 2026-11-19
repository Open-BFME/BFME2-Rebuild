// inisup.h: the two records an INIClass keeps, INIEntry and INISection.
// Target facts (retail game.dat): both derive from Node<> so they can sit in a List; their own
// members start at +0x0C. INIEntry is SIZE 0x14: the entry text pointer +0x0C and the value text
// pointer +0x10; its destructor 0x00616530 frees both strings. INISection is SIZE 0x40: the
// section name pointer +0x0C, a List<INIEntry *> at +0x10 (0x1C bytes) and an IndexClass<int,
// INIEntry *> at +0x2C (0x14 bytes); its constructor is 0x006167E0 and destructor 0x00616830.
// Both are keyed in their index by the CRC-32 of their name (CRC::String).
#pragma once
#ifndef BFME2_INISUP_H
#define BFME2_INISUP_H

#include "WWLib/listnode.h"
#include "WWLib/index.h"
#include "WWLib/crc.h"

struct INIEntry : public Node<INIEntry *>
{
	INIEntry(char *entry = 0, char *value = 0) : Entry(entry), Value(value) {}	// 0x00616790
	~INIEntry();									// 0x00616530
	int Index_ID() const { return CRC::String(Entry); }

	char *Entry;			// +0x0C
	char *Value;			// +0x10
};
// SIZE 0x14

struct INISection : public Node<INISection *>
{
	INISection(char *section);							// 0x006167E0
	~INISection();									// 0x00616830
	INIEntry *Find_Entry(const char *entry) const;					// 0x00616C00
	int Index_ID() const { return CRC::String(Section); }

	char *Section;			// +0x0C
	List<INIEntry *> EntryList;	// +0x10
	IndexClass<int, INIEntry *> EntryIndex;	// +0x2C

private:
	INISection(const INISection &rvalue);
	INISection operator=(const INISection &rvalue);
};
// SIZE 0x40

#endif
