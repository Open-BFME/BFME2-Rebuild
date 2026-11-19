// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// pk.h: WWLib's public key (PKey): a modulus, an exponent and the modulus's bit count.
// Target facts (retail game.dat): PKey is SIZE 0x204 and has no vptr: Modulus +0 and
// Exponent +0x100 (each 64 words, 0x100 bytes, the template instance Int<64> whose
// constructor from a value is 0x00615C70 and DER encoder 0x00615C90), BitPrecision +0x200.
// The constructor 0x0061DF40 takes (exponent, modulus), decodes both and stores the
// modulus's bit count less one; Plain_Block_Size 0x0061DC80 is (BitPrecision - 1) / 8 and
// Crypt_Block_Size 0x0061DCA0 is one more.
#pragma once
#ifndef BFME2_PK_H
#define BFME2_PK_H

#include "WWLib/mpmath.h"

class Straw;

// A 64-word integer for the XMP routines (retail's Int<64>).
template<int PRECISION>
class Int
{
public:
	Int(unsigned int value = 0) { XMP_Init(reg, value, PRECISION); }		// 0x00615C70
	int DEREncode(unsigned char *output) const { return XMP_DER_Encode(reg, output, PRECISION); }	// 0x00615C90

	// Zero Hour's INT.H: the word array through conversions, the modular exponentiation, and the shared error word
	// that every arithmetic operator stores (PKey::Encrypt 0x0061E210 stores XMP_Exponent_Mod's result in it).
	operator unsigned int *() { return &reg[0]; }
	operator const unsigned int *() const { return &reg[0]; }
	Int exp_b_mod_c(const Int &e, const Int &m) const
	{
		Int result;
		Error = XMP_Exponent_Mod(result, &reg[0], e, m, PRECISION);
		return result;
	}

	static int Error;

	unsigned int reg[PRECISION];
};

template<int PRECISION>
int Int<PRECISION>::Error = 0;

class PKey
{
public:
	PKey() : BitPrecision(0) {}						// inline: both integers cleared through XMP_Init
	PKey(const void *exponent, const void *modulus);			// 0x0061DF40

	int Plain_Block_Size() const;						// 0x0061DC80
	int Crypt_Block_Size() const;						// 0x0061DCA0
	int Encode_Modulus(void *buffer) const;					// 0x0061DFA0
	int Encode_Exponent(void *buffer) const;				// 0x0061DFC0
	void Decode_Modulus(void *buffer);					// 0x0061DFE0
	void Decode_Exponent(void *buffer);					// 0x0061E010
	int Encrypt(const void *source, int slen, void *dest) const;		// 0x0061E210
	int Decrypt(const void *source, int slen, void *dest) const;		// 0x0061E340
	static void Generate(Straw &random, int bits, PKey &fastkey, PKey &slowkey);	// 0x0061E7C0

	enum { MAX_UNIT_PRECISION = 64 };

private:
	Int<MAX_UNIT_PRECISION> Modulus;		// +0x000
	Int<MAX_UNIT_PRECISION> Exponent;		// +0x100
	int BitPrecision;				// +0x200
};
// SIZE 0x204

#endif
