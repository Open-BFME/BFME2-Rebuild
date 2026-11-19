// FontLibrary.h: the font cache and its INI-driven font settings. SIZE 0x2C.
//
// Target facts (retail RVAs; table addresses are VAs): the constructor 0x00218942
// runs the SubsystemInterface constructor (0x001B4E63), stores the 16-slot table
// 0x007E5AD0, builds two red-black maps at +0x14 and +0x20 (the out-of-line map
// constructor 0x0033C432), clears the font list head +0x0C and the count +0x10, and
// names the subsystem "TheFontLibrary" (setName 0x0006F3CC). The destructor
// 0x00218415 deletes every font (0x00217449), then destroys the +0x20 map (0x00217FEA)
// and the +0x14 map (0x00217FB2). Slots 14/15 load and release a font's device data
// (the device-side W3D library overrides them).
//
// The two maps (both keyed by the font's name, ordered by the out-of-line
// operator< 0x0005598C):
//  * +0x14, the default-settings map: name -> AptRef<FontDefaultSettingsTable>.
//    Filled by ParseFontDefaultSettings 0x00218C82 ("FontDefaultSettings" INI block);
//    read by 0x002186A6 (name and point size -> settings record).
//  * +0x20, the substitution map: name -> heap-allocated vector of
//    FontSizeSubstitution records. Filled by parseFontSizeMapping 0x00218E96 (the
//    "FontSubstitution" INI block's field callback); read by 0x00218468, which maps a
//    requested size and weight through the table; reset 0x00218BEF deletes the vectors.
#pragma once
#ifndef BFME2_FONTLIBRARY_H
#define BFME2_FONTLIBRARY_H

#include "Common/StlMap.h"
#include "Common/Vector.h"
#include "Common/SubsystemInterface.h"
#include "Common/AsciiString.h"
#include "Apt/AptRef.h"
#include "GameClient/GameFont.h"

class INI;

bool operator<(const AsciiString &left, const AsciiString &right);			// 0x0005598C, the maps' key order

// One font's settings at one point size (or the font's default). SIZE 0xC.
// Target facts: vtable 0x007E5AA8 (sole slot: deleting destructor 0x005FAA31);
// the constructor is 0x00217497 (vptr, count 0); the INI field table 0x007E5B30
// holds the single token "Antialiased" (a Bool at +8, parser 0x0042E850);
// ParseFontDefaultSettings sets it to true before reading the block.
class FontDefaultSettings : public AptRefTarget
{
public:
	FontDefaultSettings() {}						// 0x00217497
	virtual ~FontDefaultSettings() {}				// slot 0: deleting dtor 0x005FAA31

	bool m_antialiased;								// +0x08
};
// SIZE 0xC

typedef _STL::map<int, AptRef<FontDefaultSettings> > FontSizeSettingsMap;

// All the settings of one font name: the default plus per-point-size overrides.
// SIZE 0x18. Target facts: vtable 0x007E5B10 (sole slot: deleting destructor
// 0x00218728), constructor 0x002186EB, destructor 0x00218744 (map, then the
// default handle, then the base).
class FontDefaultSettingsTable : public AptRefTarget
{
public:
	FontDefaultSettingsTable() {}					// 0x002186EB
	virtual ~FontDefaultSettingsTable() {}			// slot 0: deleting dtor 0x00218728, dtor 0x00218744

	AptRef<FontDefaultSettings> m_default;			// +0x08 the record used when no size matches
	FontSizeSettingsMap m_bySize;					// +0x0C int point size -> record
};
// SIZE 0x18

// One row of a font's substitution table. SIZE 0x10 (parseFontSizeMapping 0x00218E96
// builds it: input size, output size, weight, replacement name; 0x00218468 sorts by
// input size, interpolates the output size and swaps in the replacement name).
struct FontSizeSubstitution
{
	int inputSize;									// +0x00 requested point size the row starts at
	int outputSize;									// +0x04 point size to use instead
	int weight;										// +0x08 0 = keep, 1 = "+bold", 2 = "-bold" (the parser's tokens)
	AsciiString replacementName;					// +0x0C font to use instead (may be empty)
};
// SIZE 0x10

// upper_bound's predicate over the rows: the first row whose input size exceeds the request.
struct FontSizeLess
{
	bool operator()(int size, const FontSizeSubstitution &row) const { return size < row.inputSize; }
};

typedef _STL::vector<FontSizeSubstitution> FontSubstitutionList;
typedef _STL::map<AsciiString, AptRef<FontDefaultSettingsTable> > FontDefaultSettingsMap;
typedef _STL::map<AsciiString, FontSubstitutionList *> FontSubstitutionMap;

class FontLibrary : public SubsystemInterface
{
public:
	FontLibrary();
	virtual ~FontLibrary();
	virtual void init();
	virtual void reset();
	virtual void update() {}
	GameFont *getFont(const AsciiString *name, float size, bool bold);
	// The two styled helpers keep the map's placeholder spellings: the ledger names
	// 0x0021857E and 0x00218468 rva... and 0x002186A6 returns void *.
	GameFont *rva0021857E(const AsciiString *name, float size, bool bold, int style);
	void rva00218468(const AsciiString *name, float *size, bool *bold);
	void *rva002186A6(const AsciiString *name, float size);
	static void ParseFontDefaultSettings(INI *);
	static void parseFontSizeMapping(INI *, void *, void *, const void *);
protected:
	virtual bool loadFontData(GameFont *)=0;
	virtual void releaseFontData(GameFont *) {}
	void unlinkFont(GameFont *);
	void linkFont(GameFont *);						// 0x002173E3: prepends to the list (the map spells it rva002173E3@Rva002173E3)
	void deleteAllFonts();
	GameFont *m_fontList;							// +0x0C head of the font list
	int m_count;									// +0x10 fonts in the list
	FontDefaultSettingsMap m_defaultSettings;		// +0x14
	FontSubstitutionMap m_substitutions;			// +0x20
};
typedef char FontLibrarySizeCheck[sizeof(FontLibrary)==0x2C ? 1 : -1];
extern FontLibrary *TheFontLibrary;

#endif
