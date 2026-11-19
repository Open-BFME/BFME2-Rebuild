// Render2DSentenceClass.h: one formatted line (or paragraph) of text drawn as 2D quads,
// the renderer W3DDisplayString embeds twice (the text pass and the hot-key pass).
//
// Evidence (retail game.dat): the constructor 0x00157E80 stores table 0x007D3C80 (one slot,
// Reset 0x00155A20: the class has no virtual destructor; ~Render2DSentenceClass is 0x00157C70)
// and the object is 0xC4 bytes (W3DDisplayString places the two copies at +0x14 and +0xD8 and
// its next member at +0x19C). The members the client UI touches are typed: WrapWidth +0x84
// (Set_Wrapping_Width 0x00105D3B skips the store when the value is unchanged), Centered +0x88
// (0x00105D63), IsClippedEnabled +0xAC, ParseHotKey +0xAD, UseHardWordWrap +0xAE (W3DDisplayString::notifyTextChanged 0x00106663 mirrors GlobalLanguage's switch into it) (W3DDisplayString::setUseHotkey
// 0x00105F94 writes the byte at W3DDisplayString+0xC1), the default scale +0xB4/+0xB8 (0x00154320)
// and the target size +0xBC/+0xC0 (0x00154370). The rest (the sentence and pending-surface vectors
// at +0x04/+0x1C, the renderer table at +0x34, the font at +0x4C, the locations and cursor from
// +0x50, the texture bookkeeping from +0x68, the clip and draw rectangles at +0x8C/+0x9C and the
// locked pointer at +0xB0) is held as opaque storage until a body needs it; the old private views
// agree on those offsets. Names are Westwood's (donor) where the map carries them.
#pragma once
#ifndef BFME2_RENDER2DSENTENCECLASS_H
#define BFME2_RENDER2DSENTENCECLASS_H

#include <stddef.h>
#include "WWMath/Vector2.h"
#include "WWMath/RectClass.h"

class FontCharsClass;

class Render2DSentenceClass
{
public:
	Render2DSentenceClass();													// 0x00157E80
	~Render2DSentenceClass();													// 0x00157C70

	virtual void Reset();														// 0x00155A20

	bool Set_Wrapping_Width(float width);										// 0x00105D3B
	bool Set_Word_Wrap_Centered(bool centered);									// 0x00105D63
	bool Set_Default_Scale(float x, float y);									// 0x00154320 ("name: inferred")
	bool Set_Target_Size(float width, float height);							// 0x00154370 ("name: inferred")
	void Set_Font(FontCharsClass *font);										// 0x001542D0
	void Set_Clipping_Rect(const RectClass &rect);								// 0x00105D96
	Vector2 Get_Formatted_Text_Extents(const unsigned short *text);				// 0x00159FF0
	void Set_Location(const Vector2 &location);									// 0x00154EF0

	// The sentence builder the extents query calls (0x00159700): the Draw_Sentence helper that
	// lays the text out left to right; hotKey/lineEnd are out parameters (may be null).
	Vector2 Build_Sentence_Not_Centered(const unsigned short *text, int *hotKey, int *lineEnd, bool measureOnly);	// 0x00159700

	unsigned char m_opaque04[0x48];												// +0x04 .. +0x4B
	FontCharsClass *m_font;														// +0x4C (Set_Font 0x001542D0)
	unsigned char m_opaque50[0x8];												// +0x50 .. +0x57
	Vector2 m_location;															// +0x58 (Set_Location 0x00154EF0)
	unsigned char m_opaque60[0x24];												// +0x60 .. +0x83
	float m_wrapWidth;															// +0x84
	bool m_centered;															// +0x88
	unsigned char m_opaque89[3];													// +0x89 .. +0x8B
	RectClass m_clipRect;													// +0x8C (Set_Clipping_Rect 0x00105D96 copies the argument through RectClass::operator= 0x0004254E)
	unsigned char m_opaque9C[0x10];												// +0x9C .. +0xAB
	bool m_isClippedEnabled;													// +0xAC
	bool m_parseHotKey;															// +0xAD
	bool m_useHardWordWrap;													// +0xAE
	unsigned char m_opaqueAF[5];												// +0xAF .. +0xB3
	float m_defaultScaleX;														// +0xB4
	float m_defaultScaleY;														// +0xB8
	float m_targetWidth;														// +0xBC
	float m_targetHeight;														// +0xC0
};
// SIZE 0xC4

typedef char Render2DSentenceClassSizeCheck[sizeof(Render2DSentenceClass) == 0xC4 ? 1 : -1];
typedef char Render2DSentenceClassParseHotKeyCheck[offsetof(Render2DSentenceClass, m_parseHotKey) == 0xAD ? 1 : -1];

#endif
