// FXShaderParameter.h: one entry of an FXShader parameter list (the 0x24-byte record the texture mappers' FXShader
// conversion builds, FXShaderAsset::Load stores and the list's Append copies).
// Evidence (retail game.dat): 0x0007C2D7 destroys one with 0x0007BB16 and steps 0x24 bytes; 0x00151E4D assigns with
// 0x00151336 at the same stride; the constructor 0x001512F1 (name, type) and 0x0007BB16 touch no vptr (no virtuals, no
// base): 0x0007BB16 releases the strings at +8 then +0 through StringBase::releaseBuffer 0x00036410; 0x001527D9 reads
// the float at +0x0C, 0x001527F5..0x0015282E move up to four floats from +0x0C, 0x0015283B reads the int at +0x1C and
// 0x0015284E the bool at +0x20. The type codes the mapper conversions pass are 6 (an integer, value at +0x1C) and
// 5 (a four-float vector, value at +0x0C). Member names are structural (role from the readers above).
#pragma once
#ifndef BFME2_FXSHADERPARAMETER_H
#define BFME2_FXSHADERPARAMETER_H

#include "Common/AsciiString.h"
#include "WWMath/Vector4.h"

struct FXShaderParameter
{
	FXShaderParameter(const char *name, int type);		// 0x001512F1
	~FXShaderParameter();								// 0x0007BB16

	AsciiString Name;			// +0x00
	int Type;					// +0x04
	AsciiString StringValue;	// +0x08
	Vector4 Value;				// +0x0C
	int IntValue;				// +0x1C
	bool BoolValue;				// +0x20
};

typedef char FXShaderParameterSize[(sizeof(FXShaderParameter) == 0x24) ? 1 : -1];

#endif
