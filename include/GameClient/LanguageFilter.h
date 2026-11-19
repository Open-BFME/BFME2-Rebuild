// LanguageFilter.h: the profanity filter subsystem (TheLanguageFilter). SIZE 0x24.
//
// Target facts (retail RVAs; table addresses are VAs): createLanguageFilter 0x0038866D
// allocates 0x24 bytes and runs the constructor 0x0038860E, which runs the
// SubsystemInterface constructor, stores the 14-slot table VA 0x00C19774 and builds two
// red-black maps at +0x0C and +0x18 (the out-of-line map constructor 0x0033C432). The
// destructor 0x00388598 clears the +0x0C map (0x00387F28) and then destroys +0x18 and
// +0x0C (0x00388266). Slot 0 is the deleting destructor 0x00388651, slot 1 init
// 0x0038873F (clears +0x0C, opens the word list through TheFileSystem, reads words with
// readWord 0x00387AC3, XORs every character with 0x5555 and files the un-leeted word
// (unHaxor 0x00387B58) into +0x0C), slot 9 is `reset() { init(); }` (0x005CB260: a tail
// jump through slot 1) and slot 10 the folded empty body. The two maps are keyed by
// UnicodeString with the case-insensitive order UnicodeStringLessThan 0x00387D1E.
#pragma once
#ifndef BFME2_LANGUAGEFILTER_H
#define BFME2_LANGUAGEFILTER_H

#include "Common/StlMap.h"
#include "Common/SubsystemInterface.h"
#include "Common/UnicodeString.h"

class File;

// Retail takes both strings by value (0x00387D1E copies them and destroys the copies).
struct UnicodeStringLessThan
{
	bool operator()(UnicodeString a, UnicodeString b) const;				// 0x00387D1E
};

typedef _STL::map<UnicodeString, bool, UnicodeStringLessThan> LanguageFilterWordMap;

class LanguageFilter : public SubsystemInterface
{
public:
	LanguageFilter();														// 0x0038860E
	virtual ~LanguageFilter();												// 0x00388598
	virtual void init();													// slot 1: 0x0038873F
	virtual void reset() { init(); }										// slot 9: 0x005CB260
	virtual void update() {}												// slot 10: folded empty body
	void filterLine(UnicodeString &line);									// 0x00388102

protected:
	bool readWord(File *file, unsigned short *word);						// 0x00387AC3
	void unHaxor(UnicodeString &word);										// 0x00387B58

	LanguageFilterWordMap m_wordList;										// +0x0C
	LanguageFilterWordMap m_subWordList;									// +0x18
};
typedef char LanguageFilterSizeCheck[sizeof(LanguageFilter) == 0x24 ? 1 : -1];

extern LanguageFilter *TheLanguageFilter;									// VA 0x00E0233C
LanguageFilter *createLanguageFilter();										// 0x0038866D

#endif
