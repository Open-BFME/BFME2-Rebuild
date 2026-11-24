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
// and the target size +0xBC/+0xC0 (0x00154370). Reset_Sentence_Data 0x00154F10
// uses the 0x24-byte sentence records, Release_Pending_Surfaces 0x00154F70
// uses 0x1C-byte pending records, and Reset 0x00155A20 frees renderers from
// the third vector. The 0x00157E80 constructor builds the surface at +0x7C
// through the owning COM wrapper 0x00116600. The destructor passes the
// non-null +0xB0 member to AssetFactoryBase::ReleaseRef (0x0061ED10).
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#ifndef BFME2_RENDER2DSENTENCECLASS_H
#define BFME2_RENDER2DSENTENCECLASS_H

#include <stddef.h>
#include "WWMath/Vector2.h"
#include "WWMath/RectClass.h"
#include "WWLib/vector.h"
#include "WW3D2/SurfaceClass.h"

class AssetFactoryBase;
class FontCharsClass;
class Render2DClass;

class Render2DSentenceClass
{
public:
	Render2DSentenceClass();													// 0x00157E80
	~Render2DSentenceClass();													// 0x00157C70

	virtual void Reset();

	bool Set_Wrapping_Width(float width);
	bool Set_Word_Wrap_Centered(bool centered);
	bool Set_Default_Scale(float x, float y);									// 0x00154320 ("name: inferred")
	bool Set_Target_Size(float width, float height);							// 0x00154370 ("name: inferred")
	void Set_Font(FontCharsClass *font);
	void Set_Clipping_Rect(const RectClass &rect);
	Vector2 Get_Formatted_Text_Extents(const unsigned short *text);
	void Draw_Sentence(unsigned long color);									// 0x00104D97: all four colours the same
	void Draw_Sentence(unsigned long color, unsigned long shadowColor, unsigned long hotKeyColor, unsigned long hotKeyShadowColor);	// 0x00155B00
	void Set_Location(const Vector2 &location);

	// The sentence builder the extents query calls (0x00159700): the Draw_Sentence helper that
	// lays the text out left to right; hotKey/lineEnd are out parameters (may be null).
	Vector2 Build_Sentence_Not_Centered(const unsigned short *text, int *hotKey, int *lineEnd, bool measureOnly);

private:
	struct SentenceDataStruct
	{
		SurfaceClass Surface;
		RectClass ScreenRect;
		RectClass UVRect;
		bool operator==(const SentenceDataStruct &other) const;
	};

	struct PendingSurfaceStruct
	{
		SurfaceClass Surface;
		DynamicVectorClass<Render2DClass *> Renderers;
		bool operator==(const PendingSurfaceStruct &other) const;
	};

	struct RendererDataStruct
	{
		Render2DClass *Renderer;
		SurfaceClass Surface;
		bool operator==(const RendererDataStruct &other) const;
	};

	typedef char SentenceDataSize[(sizeof(SentenceDataStruct) == 0x24) ? 1 : -1];
	typedef char PendingSurfaceSize[(sizeof(PendingSurfaceStruct) == 0x1C) ? 1 : -1];
	typedef char RendererDataSize[(sizeof(RendererDataStruct) == 8) ? 1 : -1];

	DynamicVectorClass<SentenceDataStruct> m_sentenceData;		// +0x04
	DynamicVectorClass<PendingSurfaceStruct> m_pendingSurfaces;	// +0x1C
	DynamicVectorClass<RendererDataStruct> m_renderers;			// +0x34

public:
	FontCharsClass *m_font;					// +0x4C
	Vector2 m_baseLocation;					// +0x50
	Vector2 m_location;						// +0x58
	Vector2 m_cursor;						// +0x60
	int m_textureOffsetX;					// +0x68
	int m_textureOffsetY;					// +0x6C
	int m_textureStartX;					// +0x70
	int m_currentTextureSize;				// +0x74
	int m_textureSizeHint;					// +0x78
	SurfaceClass m_currentSurface;			// +0x7C
	bool m_monoSpaced;						// +0x80
	float m_wrapWidth;						// +0x84
	bool m_centered;						// +0x88
	RectClass m_clipRect;					// +0x8C
	RectClass m_drawExtents;				// +0x9C
	bool m_isClippedEnabled;				// +0xAC
	bool m_parseHotKey;						// +0xAD
	bool m_useHardWordWrap;					// +0xAE
	AssetFactoryBase *m_assetFactory;			// +0xB0
	float m_defaultScaleX;														// +0xB4
	float m_defaultScaleY;														// +0xB8
	float m_targetWidth;														// +0xBC
	float m_targetHeight;														// +0xC0
};
// SIZE 0xC4

typedef char Render2DSentenceClassSizeCheck[sizeof(Render2DSentenceClass) == 0xC4 ? 1 : -1];
typedef char Render2DSentenceClassParseHotKeyCheck[offsetof(Render2DSentenceClass, m_parseHotKey) == 0xAD ? 1 : -1];

#endif
