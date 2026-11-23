#pragma once
#ifndef BFME2_NBENCH_EMFLOAT_H
#define BFME2_NBENCH_EMFLOAT_H

#include <stddef.h>

/* Precision in 16-bit words for BYTEmark / nbench emulated floating-point values. */
#define INTERNAL_FPF_PRECISION 4

/* InternalFPF::type classification constants. */
#define IFPF_IS_ZERO 0
#define IFPF_IS_SUBNORMAL 1
#define IFPF_IS_NORMAL 2
#define IFPF_IS_INFINITY 3
#define IFPF_IS_NAN 4
#define IFPF_TYPE_COUNT 5

/* Signed 16-bit exponent bounds used by InternalFPF normalization and rounding. */
#define MIN_EXP (-32767)
#define MAX_EXP 32767

typedef struct InternalFPF
{
	unsigned char type;
	unsigned char sign;
	short exp;
	unsigned short mantissa[INTERNAL_FPF_PRECISION];
} InternalFPF;

typedef char InternalFPF_size_check[(sizeof(InternalFPF) == 12) ? 1 : -1];
typedef char InternalFPF_off_type[(offsetof(InternalFPF, type) == 0) ? 1 : -1];
typedef char InternalFPF_off_sign[(offsetof(InternalFPF, sign) == 1) ? 1 : -1];
typedef char InternalFPF_off_exp[(offsetof(InternalFPF, exp) == 2) ? 1 : -1];
typedef char InternalFPF_off_mantissa[(offsetof(InternalFPF, mantissa) == 4) ? 1 : -1];

#endif
