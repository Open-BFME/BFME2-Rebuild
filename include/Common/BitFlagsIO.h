// BitFlagsIO.h: the INI side of BitFlags, template members defined once for every
// instantiation. WorldBuilder 1.06's assert text names this header
// (`..\GameEngine\Source\Common/BitFlagsIO.h`, bf1 2026-11-17); a file that emits one of
// these instantiations includes it and instantiates the members retail placed there.
//
// Target facts (one copy per instantiation, same bytes apart from the name table):
// - parse (234 bytes): BitFlags<101> 0x003B1017 (SpecialPower.cpp), <218> 0x00256499
//   (ModuleFactory.cpp), <104> 0x002C8754 (WeaponSet.cpp), <15> 0x0049B8A0. Each calls
//   INI::rva0002DFE2 for the next token, the shared append 0x0049B6AE and its own copy of
//   the token applier below.
// - rva003339CE (308 bytes, the token applier; the map's name for the <101> copy):
//   <101> 0x003339CE, <218> 0x00255D58, <104> 0x002C82C8, <15> 0x0049B750. NONE clears
//   the set and ends the line (returns false); +NAME and -NAME set and clear one bit; a
//   bare NAME clears the set before the first one. Mixing the two forms throws
//   INIException(2, "you may not mix normal and +- ops in bitstring lists") (VA 0x00BBE0DC).
//   Names are looked up through 0x0002BCAB with the instantiation's s_bitNameList
//   (<101>: VA 0x00DA5F30), throwing when a name is missing.
#pragma once
#ifndef BFME2_BITFLAGSIO_H
#define BFME2_BITFLAGSIO_H

#include "Common/BitFlags.h"
#include "Common/AsciiString.h"
#include "Common/INI.h"
#include "Common/INIException.h"
#include "Common/Xfer.h"
#include "Common/XferException.h"
#include <string.h>

// 0x0002BCAB (INI.cpp): index of token in a NULL-terminated name list; *found says whether
// it was there, and a missing name throws INIException(3, ...) when throwIfMissing is set.
Int scanIndexListChecked(const char *token, const char *const *nameList, Bool *found, Bool throwIfMissing);

template <int NUMBITS>
Bool BitFlags<NUMBITS>::rva003339CE(const char *token, Bool *sawName, Bool *sawSign)
{
	if (_strcmpi(token, "NONE") == 0)
	{
		if (*sawName || *sawSign)
			throw INIException(2, "you may not mix normal and +- ops in bitstring lists");
		clear();
		return false;
	}
	Bool found;
	if (token[0] == '+')
	{
		if (*sawName)
			throw INIException(2, "you may not mix normal and +- ops in bitstring lists");
		set(scanIndexListChecked(token + 1, s_bitNameList, &found, true));
		*sawSign = true;
	}
	else if (token[0] == '-')
	{
		if (*sawName)
			throw INIException(2, "you may not mix normal and +- ops in bitstring lists");
		clear(scanIndexListChecked(token + 1, s_bitNameList, &found, true));
		*sawSign = true;
	}
	else
	{
		if (*sawSign)
			throw INIException(2, "you may not mix normal and +- ops in bitstring lists");
		if (!*sawName)
			clear();
		set(scanIndexListChecked(token, s_bitNameList, &found, true));
		*sawName = true;
	}
	return true;
}

// Every token on the line is applied in order, and the raw text is collected into *str when
// the caller wants it. A token that carries a quoted list is split at its separators first.
template <int NUMBITS>
void BitFlags<NUMBITS>::parse(INI *ini, AsciiString *str)
{
	if (str != 0)
		str->clear();

	Bool sawName = false;
	Bool sawSign = false;
	Bool quotedList = false;

	const char *token;
	while ((token = ini->rva0002DFE2(0, &quotedList)) != 0)
	{
		if (quotedList)
		{
			AsciiString rest(token);
			AsciiString piece;
			while (rest.nextToken(&piece, 0))
			{
				const char *text = piece.str();
				append(text, str);
				if (!rva003339CE(text, &sawName, &sawSign))
					break;
			}
			quotedList = false;
		}
		else
		{
			append(token, str);
			if (!rva003339CE(token, &sawName, &sawSign))
				break;
		}
	}
}

// getSingleBitFromName: Zero Hour's lookup, a case-blind compare against each name in turn
// (<218> 0x002AA2F9 in Player.cpp, <101> 0x0028C819 in Object.cpp, <591> 0x000B42CA in
// W3DScriptedModelDraw.cpp: 54 bytes each, `_strcmpi(*name, token)`, -1 when absent).
template <int NUMBITS>
Int BitFlags<NUMBITS>::getSingleBitFromName(const char *token)
{
	Int i = 0;
	for (const char **name = s_bitNameList; *name != 0; ++name, ++i)
	{
		if (_strcmpi(*name, token) == 0)
			return i;
	}
	return -1;
}

// testSetAndClear: every bit of mustBeSet present and none of mustBeClear, word by word
// (<218> 0x0030A146, emitted in Thing.cpp, where Thing::isKindOfMulti 0x0030AD7D tail-jumps to it).
template <int NUMBITS>
Bool BitFlags<NUMBITS>::testSetAndClear(const BitFlags &mustBeSet, const BitFlags &mustBeClear) const
{
	for (UnsignedInt i = 0; i < NUMWORDS; ++i)
	{
		if (m_bits[i] & mustBeClear.m_bits[i])
			return false;
		if ((m_bits[i] & mustBeSet.m_bits[i]) != mustBeSet.m_bits[i])
			return false;
	}
	return true;
}

// getBitNameIfSet: the name of a set bit, else NULL (<21> 0x004BE0C7, <218> 0x002AA2CF, <104> 0x0028C725).
template <int NUMBITS>
const char *BitFlags<NUMBITS>::getBitNameIfSet(UnsignedInt bit) const
{
	return test(bit) ? s_bitNameList[bit] : NULL;
}

// xferCRCBytes: the set packed into (N + 7) / 8 bytes for the light CRC (<21> 0x004BE722,
// <218> 0x002ABBBE, <104> 0x0028F808).
template <int NUMBITS>
void BitFlags<NUMBITS>::xferCRCBytes(Xfer *xfer)
{
	UnsignedByte bytes[(NUMBITS + 7) / 8];
	memset(bytes, 0, sizeof(bytes));
	for (Int i = 0; i < NUMBITS; ++i)
	{
		if (test(i))
			bytes[i / 8] |= (UnsignedByte)(1 << (i % 8));
	}
	xfer->XferRawBytes(bytes, sizeof(bytes));
}

// count: the set bits, a parallel bit count per word (<218> 0x002AACDB, <104> 0x0028F6EB;
// probe k07 p11: the first step reads the word twice, the nibble sum adds the shifted value first).
template <int NUMBITS>
Int BitFlags<NUMBITS>::count() const
{
	Int c = 0;
	for (UnsignedInt i = 0; i < NUMWORDS; ++i)
	{
		UnsignedInt w = m_bits[i] - ((m_bits[i] >> 1) & 0x55555555);
		w = (w & 0x33333333) + ((w >> 2) & 0x33333333);
		c += ((((w >> 4) + w) & 0x0F0F0F0F) * 0x01010101) >> 24;
	}
	return c;
}

// setBitByName: false when the name is not in the table (<218> 0x002AAD23, <104> 0x0028F733).
template <int NUMBITS>
Bool BitFlags<NUMBITS>::setBitByName(const char *token)
{
	Int bit = getSingleBitFromName(token);
	if (bit >= 0)
	{
		set(bit);
		return true;
	}
	return false;
}

// countIntersection / countInverseIntersection: the bits set in both, and the bits of `that`
// not set here (<104> 0x0033ADFC / 0x0033AE53, emitted in ThingTemplate.cpp for the weapon-set
// SparseMatchFinder; probe k07 p12, same popcount as count()).
template <int NUMBITS>
Int BitFlags<NUMBITS>::countIntersection(const BitFlags &that) const
{
	Int c = 0;
	for (UnsignedInt i = 0; i < NUMWORDS; ++i)
	{
		UnsignedInt v = that.m_bits[i] & m_bits[i];
		UnsignedInt w = v - ((v >> 1) & 0x55555555);
		w = (w & 0x33333333) + ((w >> 2) & 0x33333333);
		c += ((((w >> 4) + w) & 0x0F0F0F0F) * 0x01010101) >> 24;
	}
	return c;
}

template <int NUMBITS>
Int BitFlags<NUMBITS>::countInverseIntersection(const BitFlags &that) const
{
	Int c = 0;
	for (UnsignedInt i = 0; i < NUMWORDS; ++i)
	{
		UnsignedInt v = ~m_bits[i] & that.m_bits[i];
		UnsignedInt w = v - ((v >> 1) & 0x55555555);
		w = (w & 0x33333333) + ((w >> 2) & 0x33333333);
		c += ((((w >> 4) + w) & 0x0F0F0F0F) * 0x01010101) >> 24;
	}
	return c;
}

// xfer: Zero Hour's version 1 (the count, then the name of each set bit), with the light CRC
// first (<218> 0x002AC06A in Player.cpp, <104> 0x002914AF in Object.cpp, <21> 0x004BE781 in
// ActiveBody.cpp). A name the table lacks throws XferException(0, NULL).
template <int NUMBITS>
void BitFlags<NUMBITS>::xfer(Xfer *xfer)
{
	Xfer::Version version(1, 1);
	*xfer == version;
	if (xfer->IsLightCRC())
	{
		xferCRCBytes(xfer);
	}
	else if (xfer->IsStoring())
	{
		Int c = count();
		*xfer == c;
		for (Int i = 0; i < NUMBITS; ++i)
		{
			const char *bitName = getBitNameIfSet(i);
			if (bitName == NULL)
				continue;
			AsciiString bitNameA(bitName);
			*xfer == bitNameA;
			--c;
		}
	}
	else
	{
		clear();
		Int c;
		*xfer == c;
		AsciiString string;
		for (Int i = 0; i < c; ++i)
		{
			*xfer == string;
			Bool valid = setBitByName(string.str());
			if (!valid)
			{
				throw XferException(0, NULL);
			}
		}
	}
}

#endif
