// FloatToLong.h: the float-to-long conversion retail inlines as a bare x87 load and store.
// Target facts: 360 `fld DWORD [ebp+x]; fistp DWORD [ebp+y]` pairs in 139 retail functions,
// with the float read back from memory, a 32-bit store and no rounding-mode change (no
// fnstcw/fldcw near any of them), so it rounds with the FPU's current mode instead of
// truncating like a C cast (`_ftol`). The checker allowlists exactly this helper (batch 4).
#pragma once
#ifndef BFME2_FLOATTOLONG_H
#define BFME2_FLOATTOLONG_H

inline long Float_To_Long(float value)
{
	long result;
	__asm fld value
	__asm fistp result
	return result;
}

#endif
