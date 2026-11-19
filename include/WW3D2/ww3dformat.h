// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ww3dformat.h: WW3D2's surface format enum (Zero Hour's WWVegas/WW3D2/ww3dformat.h gives the names).
// Target facts (retail game.dat): the values are NOT Zero Hour's 0..25. DX8Wrapper::_Create_DX8_Surface 0x0011DC60 hands its
// WW3DFormat to the device's CreateOffscreenPlainSurface (slot 36) unchanged, and DX8Caps::Check_Texture_Format_Support
// 0x0012B210 walks a 123-entry table (SupportTextureFormat at +0x13E) asking the device about format i directly for i below
// 118, and for the five tail entries about the compressed formats, with Check_Render_To_Texture_Support 0x0012B2A0 and
// Check_Depth_Stencil_Support 0x0012B3A0 doing the same over 118 entries. So the enumerators carry Direct3D's own format
// numbers (the names below are Zero Hour's list of the ones the game uses); the compressed formats keep their FourCCs
// (Get_Bits_Per_Pixel 0x00131A9D switches on 0x31545844.. as well), and the support tables hold them in slots 118..122.
#pragma once
#ifndef BFME2_WW3DFORMAT_H
#define BFME2_WW3DFORMAT_H

enum WW3DFormat {
	WW3D_FORMAT_UNKNOWN = 0,
	WW3D_FORMAT_R8G8B8 = 20,
	WW3D_FORMAT_A8R8G8B8 = 21,
	WW3D_FORMAT_X8R8G8B8 = 22,
	WW3D_FORMAT_R5G6B5 = 23,
	WW3D_FORMAT_X1R5G5B5 = 24,
	WW3D_FORMAT_A1R5G5B5 = 25,
	WW3D_FORMAT_A4R4G4B4 = 26,
	WW3D_FORMAT_R3G3B2 = 27,
	WW3D_FORMAT_A8 = 28,
	WW3D_FORMAT_A8R3G3B2 = 29,
	WW3D_FORMAT_X4R4G4B4 = 30,
	WW3D_FORMAT_A8P8 = 40,
	WW3D_FORMAT_P8 = 41,
	WW3D_FORMAT_L8 = 50,
	WW3D_FORMAT_A8L8 = 51,
	WW3D_FORMAT_A4L4 = 52,
	WW3D_FORMAT_U8V8 = 60,			// Bumpmap
	WW3D_FORMAT_L6V5U5 = 61,		// Bumpmap
	WW3D_FORMAT_X8L8V8U8 = 62,		// Bumpmap
	WW3D_FORMAT_DXT1 = 0x31545844,	// the Direct3D FourCCs: Get_Bits_Per_Pixel 0x00131A9D compares them
	WW3D_FORMAT_DXT2 = 0x32545844,
	WW3D_FORMAT_DXT3 = 0x33545844,
	WW3D_FORMAT_DXT4 = 0x34545844,
	WW3D_FORMAT_DXT5 = 0x35545844
};

// Get_Bits_Per_Pixel (0x00131A9D, __fastcall): the format's size in bits, 0 for formats without one.
unsigned __fastcall Get_Bits_Per_Pixel(WW3DFormat format);

#endif
