// CreateAHeroData.h: a Create-a-Hero hero's saved state. SIZE 0x140.
//
// Target facts (retail RVAs): the object is built by 0x00409C3D (value) and
// 0x00409D4D (copy), and its vtable (VA 0x00C38D88) is installed there. The
// deleting destructor is 0x00409D31 and calls 0x00409285. The Snapshot base
// occupies +0x00. GetSnapshotName 0x00409353 returns the literal name
// "CreateAHeroData". The fifteen level records at +0x80 (12 bytes each) are
// constructed by 0x00409CFD and destroyed by 0x004092D5.
//
// Open: the vtable's slot 3 (xfer) and slot 4 (Update) name retail functions
// that the ledger spells as CreateAHeroHero members (0x00408C11, 0x004083FF),
// and CreateAHeroHero's bodies operate on this same object. Members other than
// +0x80 are not recovered yet; their spans are kept as padding, with the
// evidence noted where it is known.
#pragma once
class Image;
#ifndef BFME2_CREATEAHERODATA_H
#define BFME2_CREATEAHERODATA_H

#include <vector>
#include <map>
#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"
#include "Common/Snapshot.h"

enum { CAH_MAX_EXP_LEVELS = 15 };

// Named stat values (+0x50): the key is a stat's name, the payload its value.
// The payload's type name is the old decomp's; it is kept so the instantiated
// map symbols keep retail's identity.
struct TreeHintPayload001F8ACB
{
	unsigned int value;
};

typedef _STL::map<AsciiString, TreeHintPayload001F8ACB> StringPayloadMap;

// Bling ids by bling key: each value is the id list of one bling group (retail's
// tree node carries the list at node+0x14 and +0x18).
typedef _STL::map<Int, _STL::vector<UnsignedInt> > IntegerVectorMap;

// One experience level's button: a command button name and two scalar words.
// SIZE 0xC. Constructor 0x004071D7 takes (name, level, value).
class BfmeHeroElement005C39DE
{
public:
	BfmeHeroElement005C39DE() : text(), word4(0), word8(0) {}			// 0x004071F7
	BfmeHeroElement005C39DE(const AsciiString &name, UnsignedInt level, UnsignedInt value)
		: text(name), word4(level), word8(value) {}						// 0x004071D7

	AsciiString text;		// +0x0
	UnsignedInt word4;		// +0x4: the experience level it was set for
	UnsignedInt word8;		// +0x8: the value stored for that level
};

class CreateAHeroData : public Snapshot
{
public:
	virtual ~CreateAHeroData();							// 0x00409285
	virtual const char *GetSnapshotName() const;		// 0x00409353

	const UnicodeString &getName() const { return m_name; }
	Int getHeroClass() const { return m_heroClass; }
	Int getHeroSubclass() const { return m_heroSubclass; }
	Bool isDefault() const { return m_isDefault; }		// the +0x48 flag (0x005B5C02 and 0x005B5C70 read it inline)
	Bool rva00407A6A(const UnicodeString &name);			// 0x00407A6A: stores a changed name (flag bit 0x10)
	bool rva0040732C();									// 0x0040732C: resets the level records
	bool GetStat(const AsciiString &key, UnsignedInt *out);			// 0x004080DD
	UnsignedInt GetStatValue(const NameKeyType *nameKeyType);		// 0x00408109

	// The manager reads the class and sub-class indices directly (retail 0x0021B670 is a CreateAHeroManager member).
	friend class CreateAHeroManager;
	friend class HeroListSorter;						// the hero list's sort order 0x005B61B3 reads the name, class, sub-class and default flag
	friend class AptMpGameSetup;						// SetSlotHeroData 0x0043DD34 reads the class, sub-class and default flag

protected:
	ObjectID m_id;										// +0x04, the hero's object id (DoXfer's ID tag)
	UnicodeString m_name;								// +0x08: the hero's name (the value constructor 0x00409C3D copies its argument here; the create-a-hero name page reads it)
	Int m_heroClass;									// +0x0C
	Int m_heroSubclass;									// +0x10
	_STL::map<Int, Int> m_bling;						// +0x14, bling id lists by group
	_STL::map<Int, Int> m_priorBling;					// +0x20, the previous bling set
	Int m_red;											// +0x2C, colour word
	Int m_green;										// +0x30, colour word
	Int m_blue;											// +0x34, colour word
	UnsignedInt m_state38;								// +0x38, state flags (bit 2: bling changed)
	unsigned char m_pad3C[0x48 - 0x3C];					// +0x3C..+0x47, not yet recovered
	Bool m_isDefault;									// +0x48, set for the default hero of a faction (the ctor 0x00409C3D clears it; GetDefaultHero 0x0021A6C8 tests it)
	unsigned char m_pad49[0x50 - 0x49];					// +0x49..+0x4F, not yet recovered
	StringPayloadMap m_map50;							// +0x50, stat values by name
	unsigned char m_pad5C[0x70 - 0x5C];					// +0x5C..+0x6F, not yet recovered
	Bool m_flag70;										// +0x70
	Bool m_updateAwards;								// +0x71
	unsigned char m_pad72[2];							// +0x72..+0x73
	IntegerVectorMap m_blingIds;						// +0x74, bling id lists by key

public:
	BfmeHeroElement005C39DE m_buttons[CAH_MAX_EXP_LEVELS];	// +0x80
	unsigned char m_pad134[0x13C - 0x134];				// +0x134..+0x13B, not yet recovered
	const Image *m_buttonImage;						// +0x13C getButtonImage 0x0033B634 returns it when set (role name)
};
typedef char CreateAHeroData_off_m_buttons[(offsetof(CreateAHeroData, m_buttons) == 0x80) ? 1 : -1];
typedef char CreateAHeroData_size[(sizeof(CreateAHeroData) == 0x140) ? 1 : -1];

#endif
