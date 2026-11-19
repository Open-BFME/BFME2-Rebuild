// CreateAHeroHero.h: the Create-a-Hero view of a hero object. It adds no data:
// its methods read and write CreateAHeroData's layout (SIZE 0x140), and retail
// calls them non-virtually on that same object. See CreateAHeroData.h for the
// vtable and the open xfer/Update question.
//
// Target facts: GetButtonForLevel 0x00406ED7 reads the name of level idx (idx
// below 15) from +0x80 and asks the control bar for its command button.
// SetButtonForLevel 0x0040737F stores a level record in +0x80. ClearSpecialPower
// 0x004072D2 assigns an empty record to a level below 15.
#pragma once
#ifndef BFME2_CREATEAHEROHERO_H
#define BFME2_CREATEAHEROHERO_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "GameLogic/CreateAHeroData.h"

class CommandButton;
class Object;
class UnicodeString;

// A running CRC of hero state: the CRC value at +0 and a set flag at +4, as
// retail's two hero CRC updaters (0x00406FBF, 0x00406FDC) keep them.
class CreateAHeroCrc
{
public:
	void update4(unsigned char *data);		// 0x00406FBF: four bytes
	void update1(unsigned char *data);		// 0x00406FDC: one byte
	void updateAscii(const AsciiString &asciiString);		// 0x0040710C: a name's CRC
	void updateUnicode(const UnicodeString &unicodeString);	// 0x00407137: the ASCII form of a name's CRC

public:
	unsigned long m_crc;					// +0x00
	Bool m_flag;							// +0x04
};

class CreateAHeroHero : public CreateAHeroData
{
public:
	Object *rva004076EE();									// 0x004076EE: the hero's object, or null when its id is unset
	void rva00407AE6(ModelConditionFlagType mc, UnsignedInt frames);	// 0x00407AE6: forwards a special model condition to that object

	const CommandButton *GetButtonForLevel(Int idx);								// 0x00406ED7
	Bool SetButtonForLevel(const AsciiString &button, UnsignedInt experienceLevel,
		UnsignedInt value);															// 0x0040737F
	Bool ClearSpecialPower(Int index);												// 0x004072D2
	void rva00407C6B();																// 0x00407C6B: clears both bling maps
	Int rva00407E28(Int key);														// 0x00407E28: the bling value, or -1 when absent
	Bool rva00407E53(Int key);														// 0x00407E53: adds a bling key at 0 when absent
	Bool rva00407DE0(Int key, Int value);											// 0x00407DE0: updates a present bling key
	Bool rva00406EFD(Int value);													// 0x00406EFD: sets the subclass (flag bit 1)
	Bool rva00407004(Int value);													// 0x00407004: sets the class, then the subclass path
	Bool rva00406F12(Int value);													// 0x00406F12: sets the red colour word (flag bit 3)
	Bool rva00406F27(Int value);													// 0x00406F27: sets the green colour word
	Bool rva00406F3C(Int value);													// 0x00406F3C: sets the blue colour word
	Bool SetDefaultColors();														// 0x00407020: the colours from the manager
	Bool rva00406E47(Int value);													// 0x00406E47: stores the hero's object id
	char rva004079D5(Int key, Int *out);											// 0x004079D5: finds a bling group; stores its node
	Int GetBlingCount(Int blingKey) const;											// 0x004079F4: ids in the bling group
	Int GetBlingId(Int blingKey, UnsignedInt index) const;							// 0x00407A29: one id of the bling group
};

#endif
