// Shared glyph rasterization state. The retail constructor at 0x001543F0
// creates and selects the bitmap. The destructor at 0x001544B0 restores the
// previous selection and releases the bitmap and DC. Size 0x14.
#pragma once
#ifndef BFME2_FONTCHARSCLASSGDISTATE_H
#define BFME2_FONTCHARSCLASSGDISTATE_H

#include <windows.h>

class FontCharsClassGdiState
{
public:
	FontCharsClassGdiState() throw();
	~FontCharsClassGdiState();

	int Refs;
	HGDIOBJ PreviousBitmap;
	HBITMAP Bitmap;
	void *Bits;
	HDC DeviceContext;
};

typedef char FontCharsGdiStateSize[(sizeof(FontCharsClassGdiState) == 0x14) ? 1 : -1];

#endif
