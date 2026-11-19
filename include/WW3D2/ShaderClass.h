// ShaderClass.h: WW3D2's packed render-state word (shader.h).
// Target facts: four bytes (Line3DClass keeps one at +0xCC and assigns presets
// with a single dword move); the presets are static members (data symbols
// ?_Preset...@ShaderClass@@2V1@A, e.g. _PresetOpaqueSolidShader RVA 0x009B6258,
// _PresetAlphaSolidShader 0x009B6260). Line3DClass's constructor stores
// 0x0010441B before anything else touches the member, which is the default
// constructor's bits.
// The field layout comes from the out-of-line setters in shader.cpp
// (0x00065F66..0x00066073, each `ShaderBits = (ShaderBits & ~mask) | (v << shift)`):
// depth compare bits 0-2, depth mask 3, colour mask 4, destination blend 5-7,
// fog 8-9, primary gradient 10-12, secondary gradient 13, source blend 14-15,
// texturing 16, N-patch 17, alpha test 18-19, cull mode 20, post-detail colour
// function 21-24, post-detail alpha function 25-27. Reset 0x00066073 stores
// 0x0010441B. Enumerator names follow the field meanings; only the type names
// are fixed by the binary (they appear in the setters' mangled names).
#pragma once
#ifndef BFME2_SHADERCLASS_H
#define BFME2_SHADERCLASS_H

struct W3dMaterial3Struct;

class ShaderClass
{
public:
	enum
	{
		SHIFT_DEPTHCOMPARE = 0,
		SHIFT_DEPTHMASK = 3,
		SHIFT_COLORMASK = 4,
		SHIFT_DSTBLEND = 5,
		SHIFT_FOG = 8,
		SHIFT_PRIGRADIENT = 10,
		SHIFT_SECGRADIENT = 13,
		SHIFT_SRCBLEND = 14,
		SHIFT_TEXTURING = 16,
		SHIFT_NPATCHENABLE = 17,
		SHIFT_ALPHATEST = 18,
		SHIFT_CULLMODE = 20,
		SHIFT_POSTDETAILCOLORFUNC = 21,
		SHIFT_POSTDETAILALPHAFUNC = 25
	};

	enum
	{
		MASK_DEPTHCOMPARE = 0x7 << SHIFT_DEPTHCOMPARE,
		MASK_DEPTHMASK = 0x1 << SHIFT_DEPTHMASK,
		MASK_COLORMASK = 0x1 << SHIFT_COLORMASK,
		MASK_DSTBLEND = 0x7 << SHIFT_DSTBLEND,
		MASK_FOG = 0x3 << SHIFT_FOG,
		MASK_PRIGRADIENT = 0x7 << SHIFT_PRIGRADIENT,
		MASK_SECGRADIENT = 0x1 << SHIFT_SECGRADIENT,
		MASK_SRCBLEND = 0x3 << SHIFT_SRCBLEND,
		MASK_TEXTURING = 0x1 << SHIFT_TEXTURING,
		MASK_NPATCHENABLE = 0x1 << SHIFT_NPATCHENABLE,
		MASK_ALPHATEST = 0x3 << SHIFT_ALPHATEST,
		MASK_CULLMODE = 0x1 << SHIFT_CULLMODE,
		MASK_POSTDETAILCOLORFUNC = 0xF << SHIFT_POSTDETAILCOLORFUNC,
		MASK_POSTDETAILALPHAFUNC = 0x7 << SHIFT_POSTDETAILALPHAFUNC
	};

	enum DepthCompareType { PASS_NEVER = 0, PASS_LESS, PASS_EQUAL, PASS_LEQUAL, PASS_GREATER, PASS_NOTEQUAL, PASS_GEQUAL, PASS_ALWAYS };
	enum DepthMaskType { DEPTH_WRITE_DISABLE = 0, DEPTH_WRITE_ENABLE };
	enum ColorMaskType { COLOR_WRITE_DISABLE = 0, COLOR_WRITE_ENABLE };
	enum DstBlendFuncType { DSTBLEND_ZERO = 0, DSTBLEND_ONE, DSTBLEND_SRC_COLOR, DSTBLEND_ONE_MINUS_SRC_COLOR, DSTBLEND_SRC_ALPHA, DSTBLEND_ONE_MINUS_SRC_ALPHA };
	enum FogFuncType { FOG_DISABLE = 0, FOG_ENABLE, FOG_SCALE_FRAGMENT, FOG_WHITE };
	enum PriGradientType { GRADIENT_DISABLE = 0, GRADIENT_MODULATE, GRADIENT_ADD, GRADIENT_BUMPENVMAP };
	enum SecGradientType { SECONDARY_GRADIENT_DISABLE = 0, SECONDARY_GRADIENT_ENABLE };
	enum SrcBlendFuncType { SRCBLEND_ZERO = 0, SRCBLEND_ONE, SRCBLEND_SRC_ALPHA, SRCBLEND_ONE_MINUS_SRC_ALPHA };
	enum TexturingType { TEXTURING_DISABLE = 0, TEXTURING_ENABLE };
	enum NPatchEnableType { NPATCH_DISABLE = 0, NPATCH_ENABLE };
	enum AlphaTestType { ALPHATEST_DISABLE = 0, ALPHATEST_ENABLE };
	enum CullModeType { CULL_MODE_DISABLE = 0, CULL_MODE_ENABLE };
	enum DetailColorFuncType { DETAILCOLOR_DISABLE = 0 };
	enum DetailAlphaFuncType { DETAILALPHA_DISABLE = 0 };

	ShaderClass() : ShaderBits(0x0010441B) {}
	ShaderClass(const unsigned int bits) { ShaderBits = bits; }
	ShaderClass(const ShaderClass &s) : ShaderBits(s.ShaderBits) {}	// Zero Hour's copy constructor: retail builds by-value arguments through it (push, then store into the slot)

	int Guess_Sort_Level() const;								// 0x001381B0

	DepthCompareType Get_Depth_Compare() const { return (DepthCompareType)((ShaderBits & MASK_DEPTHCOMPARE) >> SHIFT_DEPTHCOMPARE); }
	DepthMaskType Get_Depth_Mask() const { return (DepthMaskType)((ShaderBits & MASK_DEPTHMASK) >> SHIFT_DEPTHMASK); }
	ColorMaskType Get_Color_Mask() const { return (ColorMaskType)((ShaderBits & MASK_COLORMASK) >> SHIFT_COLORMASK); }
	DstBlendFuncType Get_Dst_Blend_Func() const { return (DstBlendFuncType)((ShaderBits & MASK_DSTBLEND) >> SHIFT_DSTBLEND); }
	FogFuncType Get_Fog_Func() const { return (FogFuncType)((ShaderBits & MASK_FOG) >> SHIFT_FOG); }
	PriGradientType Get_Primary_Gradient() const { return (PriGradientType)((ShaderBits & MASK_PRIGRADIENT) >> SHIFT_PRIGRADIENT); }
	SecGradientType Get_Secondary_Gradient() const { return (SecGradientType)((ShaderBits & MASK_SECGRADIENT) >> SHIFT_SECGRADIENT); }
	SrcBlendFuncType Get_Src_Blend_Func() const { return (SrcBlendFuncType)((ShaderBits & MASK_SRCBLEND) >> SHIFT_SRCBLEND); }
	TexturingType Get_Texturing() const { return (TexturingType)((ShaderBits & MASK_TEXTURING) >> SHIFT_TEXTURING); }
	NPatchEnableType Get_NPatch_Enable() const { return (NPatchEnableType)((ShaderBits & MASK_NPATCHENABLE) >> SHIFT_NPATCHENABLE); }
	AlphaTestType Get_Alpha_Test() const { return (AlphaTestType)((ShaderBits & MASK_ALPHATEST) >> SHIFT_ALPHATEST); }
	CullModeType Get_Cull_Mode() const { return (CullModeType)((ShaderBits & MASK_CULLMODE) >> SHIFT_CULLMODE); }
	DetailColorFuncType Get_Post_Detail_Color_Func() const { return (DetailColorFuncType)((ShaderBits & MASK_POSTDETAILCOLORFUNC) >> SHIFT_POSTDETAILCOLORFUNC); }
	DetailAlphaFuncType Get_Post_Detail_Alpha_Func() const { return (DetailAlphaFuncType)((ShaderBits & MASK_POSTDETAILALPHAFUNC) >> SHIFT_POSTDETAILALPHAFUNC); }

	// Out of line in shader.cpp.
	void Set_Depth_Compare(DepthCompareType x);				// 0x00065F66
	void Set_Depth_Mask(DepthMaskType x);				// 0x00065F73
	void Set_Color_Mask(ColorMaskType x);				// 0x00065F85
	void Set_Dst_Blend_Func(DstBlendFuncType x);				// 0x00065FEB
	void Set_Fog_Func(FogFuncType x);				// 0x00065FFD
	void Set_Primary_Gradient(PriGradientType x);				// 0x00066010
	void Set_Secondary_Gradient(SecGradientType x);				// 0x00066023
	void Set_Src_Blend_Func(SrcBlendFuncType x);				// 0x00066036
	void Set_Texturing(TexturingType x);				// 0x00066049
	void Set_NPatch_Enable(NPatchEnableType x);				// 0x0006605E
	void Set_Alpha_Test(AlphaTestType x);				// 0x00065FC1
	void Set_Cull_Mode(CullModeType x);				// 0x00065FD6
	void Set_Post_Detail_Color_Func(DetailColorFuncType x);				// 0x00065FAC
	void Set_Post_Detail_Alpha_Func(DetailAlphaFuncType x);				// 0x00065F97
	void Reset();					// 0x00066073
	void Enable_Fog(const char *source);				// 0x00138200

	unsigned long Get_Bits() const { return ShaderBits; }

	// Static sort categories: MeshModelClass::get_sort_flags 0x00189270 turns each
	// into a bit and compute_static_sort_levels 0x00189320 tests 1, 3, 4 and 8.
	enum StaticSortCategoryType
	{
		SSCAT_OPAQUE = 0,
		SSCAT_ALPHA_TEST,
		SSCAT_ADDITIVE,
		SSCAT_SCREEN,	// Zero Hour's; Guess_Sort_Level 0x001381B0 switches over four categories (3 gives 15)
		SSCAT_OTHER
	};
	StaticSortCategoryType Get_SS_Category(void) const;				// 0x00138130
	void Init_From_Material3(const W3dMaterial3Struct &mat3);			// 0x00137550
	static void Invert_Backface_Culling(bool onoff);					// 0x00138110
	static bool Is_Backface_Culling_Inverted(void);					// 0x001381F0
	// Shaders compare by their bits (MaterialCollectorClass::Add_Shader 0x0016FCD0
	// compares the dword against the last and each stored shader).
	bool operator==(const ShaderClass &that) const { return ShaderBits == that.ShaderBits; }
	bool operator!=(const ShaderClass &that) const { return ShaderBits != that.ShaderBits; }

	// Zero Hour's inline: the next Apply reloads every render state (DX8Wrapper::Invalidate_Cached_Render_States 0x0011FD10 stores
	// 1 at 0x009B621C).
	static void Invalidate(void) { ShaderDirty = true; }

	static ShaderClass _PresetOpaqueSolidShader;		// RVA 0x009B6258
	static ShaderClass _PresetOpaqueShader;				// RVA 0x009B6228 (ParticleBufferClass::Get_Shader 0x001A7B10)
	static ShaderClass _PresetAlphaSolidShader;			// RVA 0x009B6260
	static ShaderClass _PresetAdditiveSolidShader;		// RVA 0x009B625C
	static ShaderClass _PresetAdditive2DShader;			// RVA 0x009B6244
	static ShaderClass _PresetAlpha2DShader;			// RVA 0x009B6248
	static ShaderClass _PresetAdditiveSpriteShader;		// RVA 0x009B624C
	static ShaderClass _PresetAlphaSpriteShader;		// RVA 0x009B6254
	static ShaderClass _PresetATestSpriteShader;		// RVA 0x009B6268
	static ShaderClass _PresetMultiplicativeSpriteShader;	// RVA 0x009B6280

private:
	friend class W3dUtilityClass;
	void Set_Field(unsigned long mask, unsigned int shift, unsigned int value)
	{
		ShaderBits &= ~mask;
		ShaderBits |= value << shift;
	}

	friend class DX8Wrapper;	// Set_Shader 0x000662E5 tests ShaderDirty
protected:
	unsigned long ShaderBits;		// +0x0
	static bool ShaderDirty;		// RVA 0x009B621C (.data)
};
// SIZE 0x4
typedef char ShaderClassSize[(sizeof(ShaderClass) == 4) ? 1 : -1];

#endif
