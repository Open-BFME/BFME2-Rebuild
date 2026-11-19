// AsciiString.h: SAGE's reference-counted narrow string, an inline wrapper
// over StringBase<char> (StringBase.h). The default, text and copy
// constructors are inline and reach StringBase<char>'s out-of-line ones
// directly (0x00037BA0, 0x000365F0); MSVC kept a few wrapper copies out of line
// (0x0000654A, 0x001D8F56), which is the same inline definition, not a second
// shape. A temporary copied into an argument slot therefore records the slot
// address before setting ecx (INITerrainRoad 0x001FF288).
// Target facts: one pointer at +0; the default ctor nulls it inline, while the
// destructor is inline (see ~AsciiString). str,
// isEmpty, set, concat and compare are StringBase<char>'s (StringBase.h).
// Only the surface module units reach is declared here.
#pragma once
#ifndef BFME2_ASCIISTRING_H
#define BFME2_ASCIISTRING_H

#include "Common/GameType.h"
#include "Common/StringBase.h"

class UnicodeString;
class PooledString;

class AsciiString : public StringBase<char>
{
public:
	AsciiString() {}
	AsciiString(const char *s) : StringBase<char>(s) {}
	AsciiString(const AsciiString &that) : StringBase<char>(that) {}
	AsciiString(const UnicodeString &that);					// 0x00038250
	// Inline: ordinary call sites call releaseBuffer<char> 0x00036410 directly;
	// unwind funclets (6,403) call the out-of-line COMDAT ??1AsciiString, a
	// `jmp releaseBuffer` that ICF folds at 0x0048BA39 (fold group F0048BA39).
	~AsciiString() {}

	operator const char *() const { return str(); }

	// Inline: retail has no call to the ??4AsciiString@@QAEAAV0@ABV0@@Z copy
	// (0x00001733) and 1211 to StringBase<char>::set (0x000366F0).
	AsciiString &operator=(const AsciiString &that) { set(that); return *this; }
	// Out of line: 3053 calls reach its copy 0x000065B8, which calls set.
	AsciiString &operator=(const char *s);
	AsciiString &operator=(const UnicodeString &that);		// 0x000382B0
	// 0x006CD4A0 (the Apt.cpp row, ICF-folded with EAStringC::hasData): `[this] != 0`. Defined in Apt.cpp, where retail emits it.
	bool hasData() const;
	// Out of line beside operator=(const char *): 0x000065E8 and 0x000065FA,
	// one-call wrappers over concat. operator=(char) 0x000065CA is left out:
	// next to operator=(const char *) it would make `s = 0` ambiguous.
	AsciiString &operator+=(const char *s);
	AsciiString &operator+=(char c);
	AsciiString &operator+=(const PooledString &s);	// 0x0060BD19: concat of the pool entry's text (+8) (Xfer.cpp; decl_queue f09 05:50, ACCEPTED k08)

	void __cdecl format(const char *fmt, ...);				// 0x00038150 (?format@AsciiString@@QAAXPBDZZ), a cdecl member
	void __cdecl format(const AsciiString *fmt, ...);				// 0x000381E0 (?format@AsciiString@@QAAXPBV1@ZZ; decl_queue f09 06:15, ACCEPTED k08)
	void translate(const unsigned short *s);				// 0x00038170
	void translate(const UnicodeString &that);				// 0x00038220

	static const AsciiString TheEmptyString;
};
// SIZE 0x04

// Free comparison, out of line at 0x0005598C (`left.compare(right) < 0`). STLport's
// less<AsciiString> calls it (map/set trees keyed by AsciiString, e.g.
// _Rb_tree::_M_insert 0x00202CF8 and _M_find 0x001F8437); without it, less would
// compare the const char * conversions. throw(): like compare(), callers keep no
// EH state around it (OptionPreferences::usesSystemMapDir 0x002E4653 holds a
// temporary AsciiString across find() with no state store).
bool operator<(const AsciiString &left, const AsciiString &right) throw();

// Equality through compare(). Inline: STLport's equal_to<AsciiString> 0x000559B6
// expands it to `compare(right) == 0`; out-of-line copies 0x0002C7C1 (==),
// 0x0002C7D4 (!=) and 0x000559A3 (== const char *). Without these, == between
// AsciiStrings compared the two const char * conversions.
inline bool operator==(const AsciiString &left, const AsciiString &right) { return left.compare(right) == 0; }
inline bool operator!=(const AsciiString &left, const AsciiString &right) { return left.compare(right) != 0; }
inline bool operator==(const AsciiString &left, const char *right) { return left.compare(right) == 0; }

#endif
