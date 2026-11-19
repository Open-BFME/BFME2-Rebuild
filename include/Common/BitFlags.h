// BitFlags.h: the engine's fixed-size bit set, one instantiation per enumeration,
// each with a static name table for INI parsing (h05). SIZE (N+31)/32 words.
//
// Target facts:
// - Names: every member below except parse/parseFromINI is spelled by a mangled
//   ledger name (`?any@?$BitFlags@$0NK@@@QBE_NXZ` and so on), all COMDATs, so they
//   are out of line: declared here, defined where retail defined them. The bit
//   accessors (test/set/clear of one bit) are always inlined in retail bodies.
// - Storage: any/test/count loop over (N+31)/32 dwords (BitFlags<11> 1 word,
//   <104> 4, <218> 7). Retail keeps an STLport bitset<N> there (DieMuxData's
//   masks reset through `bitset<128>::reset`, 0x0024CA24, the fold of every
//   97..128-bit reset); the words are the same bytes.
// - parse/parseFromINI are h05's proposed names (the map holds placeholders):
//   each table entry for a mask is the 16-byte wrapper
//   `((BitFlags *)store)->parse(ini, NULL)` (SAGE's BitFlags::parseFromINI), and
//   parse is a 234-byte copy per instantiation that reads tokens until the line
//   ends and sets bits by name.
//
// Instantiations, identified by the name table each one's getSingleBitFromName
// or parse reads (VA of s_bitNameList, count, first names):
//   BitFlags<11>   0x00DC828C  11 DEFAULT, DISABLED_USER_PARALYZED, ...   (DisabledMaskType)
//   BitFlags<12>   0x00DBA9C8  12 TAUNT, CHEER, HERO_CHEER, POINT, ...
//   BitFlags<15>   0x00DBAA00  15 NONE, LEADERSHIP, FORMATION, SPELL, ... parse 0x0049B8A0, wrapper 0x0049BBA3 (AntiCategory)
//   BitFlags<21>   0x00DBAA40  21 VETERAN, ELITE, HERO, PLAYER_UPGRADE, ...
//   BitFlags<101>  0x00DA5F30 101 DESTROYED, CAN_ATTACK, ...   object status; parse 0x003B1017, wrapper 0x003B1327
//   BitFlags<104>  0x00DBC2C8 104 VETERAN, ELITE, HERO, ...    weapon-set flags; parse 0x002C8754, wrapper 0x002C8C06
//   BitFlags<154>  0x00DC0B40 154 SPECIAL_INVALID, ...         special-power types
//   BitFlags<218>  0x00DBBE18 218 OBSTACLE, SELECTABLE, ...    kind-of; parse 0x00256499, wrapper 0x002567EA
//   BitFlags<591>  0x00DBAA98 591 TOPPLED, FRONTCRUSHED, ...   model condition; parse 0x000B937E, wrapper 0x000B9468
// The map's BitFlags<69>, <116> and <117> names carry Zero Hour's counts (KINDOF
// 116, MODELCONDITION 117) and no BFME 2 table of that size exists; its
// BitFlags<304>::getSingleBitFromName reads the 591-name table (a fold).
#pragma once
#ifndef BFME2_BITFLAGS_H
#define BFME2_BITFLAGS_H

#include "Common/GameType.h"
#include <string.h>

class INI;
class Xfer;
class AsciiString;

template <int NUMBITS>
class BitFlags
{
public:
	enum { NUMWORDS = (NUMBITS + 31) / 32 };

	// Inline, in class. MSVC 7.1 /O1 inlines it for a local or a temporary and declines to
	// inline it for a member constructed by an enclosing constructor, which then calls the
	// COMDAT: WeaponSet::xfer 0x002C8872 memsets its local WeaponSetFlags in place, while 29
	// class constructors and DieMuxData's (0x004CE534) call 0x0024CA24 (memset 0x10, returns
	// this) for a 97..128-bit member, 0x0024C7B3 (memset 0x1C) for a 218-bit one. Compile
	// check found the member constructor out of line and the local and temporary constructors inline.
	BitFlags() { memset(m_bits, 0, sizeof(m_bits)); }
	// Retail passes a BitFlags<218> by value through one out-of-line copy constructor
	// (COMDAT 0x0004543D, `memcpy(this, src, 0x1C)`), e.g. Player::countObjects' callers.
	BitFlags(const BitFlags &that) { memcpy(m_bits, that.m_bits, sizeof(m_bits)); }
	// Zero Hour's one-bit constructor; the BitFlags<218> copy is emitted out of line at
	// 0x00045411 (memset 0x1C, then the bit; map: ??0Rva00045411BitSet, ScoreKeeper 0x0039CBCE).
	// It is nothrow: ScriptActions::doNamedUseCommandButtonOnNearestEnemyBuilding 0x003BDE5D calls the out-of-line copy with no
	// exception-state store before the call, which only a throw() constructor gives.
	enum BogusInitType { kInit = 0 };
	BitFlags(BogusInitType k, Int idx1) throw() { memset(m_bits, 0, sizeof(m_bits)); set(idx1); }
	// Zero Hour's two-bit form; emitted out of line for BitFlags<218> at 0x0006EE7A (28 bytes: memset 0x1C, two ors;
	// Drawable::setIndicatorColor 0x002741DE calls it).
	BitFlags(BogusInitType k, Int idx1, Int idx2) { memset(m_bits, 0, sizeof(m_bits)); set(idx1); set(idx2); }

	// One form for every instantiation (BitFlags11.h's too): Zero Hour's test is STLport's
	// bitset<N>::test(size_t), so the index is unsigned at the call. Judged on all 272
	// includers. Both plain-inline forms lose the two Lua rows below.
	// evidence: retail expands it at 0x00333031 and 0x003330A8 (variable index, plain inline declined there) and at 0x004BEE21 0x004BE056 0x004BDFCB (shr; test cl,1)
	// Ordinary inlining, even with /Ob2, loses LuaScriptEngine 0x00333031 and 0x003330A8 and StructureCollapseUpdate 0x004A4580.
	__forceinline Bool test(UnsignedInt bit) const { return (m_bits[bit >> 5] & (1u << (bit & 31))) != 0; }
	void set(Int bit) { m_bits[(UnsignedInt)bit >> 5] |= 1u << (bit & 31); }
	void clear(Int bit) { m_bits[(UnsignedInt)bit >> 5] &= ~(1u << (bit & 31)); }

	Bool any() const;
	Bool test(const void *other) const;
	Int count() const;
	Int countIntersection(const BitFlags &that) const;
	Int countInverseIntersection(const BitFlags &that) const;
	Bool testSetAndClear(const BitFlags &mustBeSet, const BitFlags &mustBeClear) const;
	// In-class, so inlined: WeaponTemplateSet::clear 0x002C752F memsets each set in place.
	void clear() { memset(m_bits, 0, sizeof(m_bits)); }
	void buildDescription(AsciiString *str, Int unused) const;
	Bool setBitByName(const char *token);
	// 0x0049B6AE (map: Rva0049B6AE::append): appends token to *str, a space first when it is non-empty.
	void append(const char *token, AsciiString *str);
	// 0x003339CE (map: Rva003339CE, BitFlags<101>): applies one token (NONE, +NAME, -NAME or NAME);
	// the two flags track a bare name and a signed name, and mixing them throws an INIException.
	Bool rva003339CE(const char *token, Bool *sawName, Bool *sawSign);
	void xfer(Xfer *xfer);
	// f14 (BitFlags<21>::xfer 0x004BE781): 0x004BE0C7 gives the name of a set bit, else NULL; 0x004BE722 writes the CRC image (ceil(N/8) bytes).
	const char *getBitNameIfSet(UnsignedInt bit) const;	// 0x004BE0C7
	void xferCRCBytes(Xfer *xfer);	// 0x004BE722

	static Int getSingleBitFromName(const char *token);
	static const char **getBitNamesList();

	void parse(INI *ini, AsciiString *str);
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData)
	{
		((BitFlags *)store)->parse(ini, 0);
	}

	static const char *s_bitNameList[];

	UnsignedInt m_bits[NUMWORDS];
};

// any(): retail's COMDAT (BitFlags<218> 0x002C7501, BitFlags<11> 0x0023C58B) is an
// unsigned word loop: `cmp [ecx+eax*4],0; jne; inc eax; cmp eax,NUMWORDS; jb`.
template <int NUMBITS>
Bool BitFlags<NUMBITS>::any() const
{
	for (UnsignedInt i = 0; i < NUMWORDS; ++i)
	{
		if (m_bits[i] != 0)
			return true;
	}
	return false;
}

typedef BitFlags<218> KindOfMaskType;			// SAGE spelling; retail symbols show only BitFlags<218>
typedef BitFlags<101> ObjectStatusMaskType;		// SAGE spelling; retail symbols show only BitFlags<101>
typedef BitFlags<104> WeaponSetFlags;			// SAGE spelling; retail symbols show only BitFlags<104>

#endif
