// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// TextureFilterClass.h: WW3D's texture filter settings (WWVegas/WW3D2/texturefilter.h). Only the members with a
// verified caller are declared: WW3D::Set_Texture_Filter 0x00117280 clamps its argument to 0..2 and tail-jumps to
// _Init_Filters 0x0013EB60 (texturefilter.cpp), which fills the per-stage min/mag/mip filter tables.
#pragma once
#ifndef BFME2_TEXTUREFILTERCLASS_H
#define BFME2_TEXTUREFILTERCLASS_H

class TextureFilterClass
{
public:
	enum TextureFilterMode
	{
		TEXTURE_FILTER_BILINEAR,
		TEXTURE_FILTER_TRILINEAR,
		TEXTURE_FILTER_ANISOTROPIC
	};

	enum TxtAddrMode
	{
		TEXTURE_ADDRESS_REPEAT = 0,
		TEXTURE_ADDRESS_CLAMP
	};

	void Apply(unsigned int stage);								// 0x0013EA30

	TxtAddrMode Get_U_Addr_Mode(void) const { return UAddressMode; }
	TxtAddrMode Get_V_Addr_Mode(void) const { return VAddressMode; }

	static void _Init_Filters(TextureFilterMode texture_filter);		// 0x0013EB60

	// The per-stage filter tables (retail's `_MinTextureFilters`, `_MagTextureFilters`, `_MipMapFilters`, 0x009F3048, 0x009F3188
	// and 0x009F32C8: sixteen stages of five unsigned values, 0x14 bytes apart).
	enum FilterType
	{
		FILTER_TYPE_NONE,
		FILTER_TYPE_FAST,
		FILTER_TYPE_BEST,
		FILTER_TYPE_RESERVED,		// +0xC: _Init_Filters 0x0013EB60 stores into it; Zero Hour has no such slot
		FILTER_TYPE_DEFAULT,
		FILTER_TYPE_COUNT
	};
	enum { MAX_TEXTURE_STAGES = 16 };

	static void _Set_Default_Min_Filter(FilterType filter);			// 0x0013ED20
	static void _Set_Default_Mag_Filter(FilterType filter);			// 0x0013ED50
	static void _Set_Default_Mip_Filter(FilterType filter);			// 0x0013ED80

private:
	// Layout from Apply 0x0013EA30: min, mag and mip filter, then the U and V address modes (five words).
	FilterType TextureMinFilter;	// +0x00
	FilterType TextureMagFilter;	// +0x04
	FilterType MipMapFilter;		// +0x08
	TxtAddrMode UAddressMode;		// +0x0C
	TxtAddrMode VAddressMode;		// +0x10
};

#endif
