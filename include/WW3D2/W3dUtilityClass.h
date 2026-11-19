// W3dUtilityClass.h: W3D's shader conversion helpers (Zero Hour's w3d_util.h W3dUtilityClass). Target facts: the two
// Convert_Shader overloads are static members at 0x001A4529 (W3D record to ShaderClass) and 0x001A45E3 (ShaderClass to
// W3D record).
#pragma once
#ifndef BFME2_W3DUTILITYCLASS_H
#define BFME2_W3DUTILITYCLASS_H

#include "WW3D2/w3d_file.h"
#include "WWMath/Vector3.h"

class ShaderClass;
struct W3dShaderStruct;

class W3dUtilityClass
{
public:
	// Zero Hour's w3d_util.cpp bodies; retail expands them in place (vertmaterial.cpp, boxrobj.cpp).
	static void Convert_Color(const W3dRGBStruct &rgb, Vector3 *set)
	{
		set->X = rgb.R * (1.0f / 255.0f);
		set->Y = rgb.G * (1.0f / 255.0f);
		set->Z = rgb.B * (1.0f / 255.0f);
	}
	static void Convert_Vector(const W3dVectorStruct &v, Vector3 *set)
	{
		set->X = v.X;
		set->Y = v.Y;
		set->Z = v.Z;
	}
	static void Convert_Shader(const W3dShaderStruct &w3d_shader, ShaderClass *shader);		// 0x001A4529
	static void Convert_Shader(const ShaderClass &shader, W3dShaderStruct *w3d_shader);		// 0x001A45E3
};

#endif
