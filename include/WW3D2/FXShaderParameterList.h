// FXShaderParameterList.h: the list an FXShader's parameters are appended to. Only Append is recovered:
// 0x00082EB8 (61 bytes) copies a 0x24-byte FXShaderParameter onto the end; TextureMapperClass::Convert_To_FXShader
// (0x00187690, 0x00187850, 0x00187AD0) and FXShaderAsset::Load (0x00151915) call it. The members are not recovered, so
// no code may construct or size one; it is only ever used through a pointer or reference.
#pragma once
#ifndef BFME2_FXSHADERPARAMETERLIST_H
#define BFME2_FXSHADERPARAMETERLIST_H

struct FXShaderParameter;

class FXShaderParameterList
{
public:
	void Append(const FXShaderParameter &parameter);		// 0x00082EB8
};

#endif
