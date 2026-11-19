// UpgradeMaskType.h: the 1024-bit upgrade mask (h05). SIZE 0x80. Not a BitFlags<N>:
// no name table or getSingleBitFromName reaches it; bits are UpgradeTemplate indices.
// Target facts (map names are ledger N: evidence; all three rows sit in map object
// "unknown", so nothing in src witnesses them yet):
// - ?clear@UpgradeMaskType@@QAEXXZ (RVA 0x001EAE86) is memset(this, 0, 0x80).
// - ?set@UpgradeMaskType@@QAEXI@Z (RVA 0x0033A7C1) ORs bit (i & 31) into word i >> 5.
// - RVA 0x001EAE6F (placeholder `clear80`) is the same memset returning this: the
//   default constructor UpgradeMuxData's constructor (0x0025342C) calls on +0 and +0x80.
// - The mangled names spell it a struct (`AAUUpgradeMaskType@@`), as in
//   getUpgradeActivationMasks(UpgradeMaskType &, UpgradeMaskType &) on every upgrade module.
// - INI: TriggeredBy/ConflictsWith go through RVA 0x0026F28B, which clears the mask and
//   sets UpgradeTemplate[+0x38] for each name (UpgradeCenter::findUpgrade).
#pragma once
#ifndef BFME2_UPGRADE_MASK_TYPE_H
#define BFME2_UPGRADE_MASK_TYPE_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"

struct UpgradeMaskType
{
	UpgradeMaskType();							// RVA 0x001EAE6F
	void clear();								// RVA 0x001EAE86
	void set(UnsignedInt bit);					// RVA 0x0033A7C1
	Bool test(UnsignedInt bit) const { return (m_bits[bit >> 5] & (1u << (bit & 31))) != 0; }
	// 0x00406F9C, ?rva00406F9C@Rva00406F9C@@QAE_NPBX@Z (the map spells it on an address-named class): true when this mask and the argument share a bit. SubObjectsUpgrade 0x004B4E56 calls it on Object +0x284 and Player +0x13C (f03)
	// 0x0033A453 (map ?rva0033A453@Rva0033A453@@QBE_NPBX0@Z): true when this mask holds every bit of `required` and shares no bit with `forbidden`, word by word.
	Bool satisfies(const UpgradeMaskType *required, const UpgradeMaskType *forbidden) const;
	Bool rva00406F9C(const UpgradeMaskType *other) const;
	// 0x002AA292 (map ?rva002AA292@Rva002AA292@@QBE_NPBH@Z, evidence=?:placeholder): true when every bit set in `other` is also set in this mask; UpgradeMux::wouldUpgrade 0x004CE2B0 calls it on the key mask for the "requires all" case (f11)
	Bool hasAllBitsOf(const UpgradeMaskType *other) const;
	Bool any() const
	{
		for (UnsignedInt i = 0; i < 0x20; ++i)
		{
			if (m_bits[i] != 0)
				return true;
		}
		return false;
	}
	// 0x004CE3B9 (map: ?rva004CE3B9@Rva004CE3B9@@QAEABVAsciiString@@XZ): the name of the lowest upgrade in the mask (found through TheUpgradeCenter->findUpgradeByMaskIndex), the empty string when none is set; f03: UnpauseSpecialPowerUpgrade 0x004B555F tail-jumps to it with its TriggeredBy mask
	const AsciiString &getFirstUpgradeName() const;
	// 0x004B3FDC (map: ?notContains@Rva00406F9C@@QAEHPBX@Z): the negation of rva00406F9C on the same mask (ObjectCreationUpgrade.cpp's rows; f03)
	Int hasNoBitsInCommon(const UpgradeMaskType *other) const;

	UnsignedInt m_bits[0x20];					// +0x0
};
// SIZE 0x80
typedef char UpgradeMaskType_size[(sizeof(UpgradeMaskType) == 0x80) ? 1 : -1];

#endif
