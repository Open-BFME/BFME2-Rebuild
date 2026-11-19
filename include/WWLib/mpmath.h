// mpmath.h: WWLib's multiple-precision integer primitives (the XMP_* family).
// Target facts (retail game.dat): plain C functions in the 0x0061BD70 object, cdecl, taking
// little-endian arrays of 32-bit words and the word count (the "precision"). XMP_Init
// 0x0061BD70 clears the array and stores the value in word 0; XMP_Count_Bits 0x0061BD90
// returns the number of significant bits; XMP_DER_Encode 0x0061C580 and XMP_DER_Decode
// 0x0061C5E0 convert between a word array and a DER byte string.
#pragma once
#ifndef BFME2_MPMATH_H
#define BFME2_MPMATH_H

extern "C" {
void XMP_Init(unsigned int *number, unsigned int value, int precision);		// 0x0061BD70
int XMP_Count_Bits(const unsigned int *number, int precision);			// 0x0061BD90
int XMP_DER_Encode(const unsigned int *number, unsigned char *output, int precision);	// 0x0061C580
void XMP_DER_Decode(unsigned int *number, const unsigned char *input, int precision);	// 0x0061C5E0
int XMP_Exponent_Mod(unsigned int *expout, const unsigned int *expin, const unsigned int *exponent_ptr, const unsigned int *modulus, int precision);	// 0x0061D4C0
}

#endif
