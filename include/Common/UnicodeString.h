// UnicodeString.h: SAGE's reference-counted wide string, StringBase<unsigned
// short> behind an inline wrapper (wchar_t is unsigned short under VC7.1
// without /Zc:wchar_t, hence StringBase@G in every mangled name).
// Target facts (retail game.dat .text calls, StringBase.h): the wrapper copies
// of the copy and text constructors (0x00006618, 0x0000662A) and of
// operator=(UnicodeString) / operator=(const unsigned short *) (0x00005394,
// 0x0000667E) are never called; 783, 184, 411 and 57 calls reach
// StringBase<G>'s copy 0x00037050, text 0x00037E30, set 0x00037150 and
// set 0x0000565D instead, so those members are inline. The destructor is
// inline: 1806 direct calls reach releaseBuffer<G> 0x00036E70. The constructor
// from AsciiString is out of line (47 calls to 0x006CB6D0). TheEmptyString is
// const, as the export ?TheEmptyString@UnicodeString@@2V1@B spells it.
#pragma once
#ifndef BFME2_UNICODESTRING_H
#define BFME2_UNICODESTRING_H

#include "Common/StringBase.h"

class AsciiString;

class UnicodeString : public StringBase<unsigned short>
{
public:
	UnicodeString() {}
	UnicodeString(const unsigned short *s) : StringBase<unsigned short>(s) {}
	UnicodeString(const UnicodeString &that) : StringBase<unsigned short>(that) {}
	UnicodeString(const AsciiString &that);					// 0x006CB6D0
	~UnicodeString() {}

	UnicodeString &operator=(const UnicodeString &that) { set(that); return *this; }
	UnicodeString &operator=(const unsigned short *s) { set(s); return *this; }
	UnicodeString &operator+=(const UnicodeString &that) { concat(that); return *this; }
	UnicodeString &operator+=(const unsigned short *s) { concat(s); return *this; }

	void translate(const AsciiString &that);
	void translate(const char *s);
	// Target facts: exports ?format@UnicodeString@@QAAXPBGZZ (0x006CB5D0) and
	// ?format@UnicodeString@@QAAXPBV1@ZZ (0x006CB660); both cdecl, variadic. Declarations only.
	void __cdecl format(const unsigned short *fmt, ...);
	void __cdecl format(const UnicodeString *fmt, ...);

	static const UnicodeString TheEmptyString;
};

// Equality through compare(). Inline: list<UnicodeString>::remove 0x0043BF50 expands it to
// `compare(value) == 0` in place; the out-of-line copy ??8@YA_NABVUnicodeString@@0@Z
// 0x000AA5CE (19 bytes: compare 0x00006A7A, neg/sbb/inc) is its COMDAT.
inline bool operator==(const UnicodeString &left, const UnicodeString &right) { return left.compare(right) == 0; }
inline bool operator!=(const UnicodeString &left, const UnicodeString &right) { return left.compare(right) != 0; }
// SIZE 0x04

#endif
