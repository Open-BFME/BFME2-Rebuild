// W3DDisplayString.h: the Direct3D implementation of DisplayString (GameClient/DisplayString.h):
// one string of UI text, drawn by two Render2DSentenceClass passes (text, then the hot-key
// underline). W3DDisplayStringManager allocates them.
//
// Evidence (retail game.dat): the constructor 0x00106088 runs DisplayString's constructor
// (0x0035897D), stores table 0x007CF900 (24 slots), builds the renderers at +0x14 and +0xD8
// (0x00157E80, 0xC4 bytes each) and clears the members behind them; the manager allocates
// 0x1F8 bytes (0x000905F0 pushes 0x1F8). Members from the setters: the changed flags at
// +0x1A0..+0x1A3 (text, font, colors, use-hot-key; the setters raise +0x1A2 when a color or scale
// changes and call slot +0x10, notifyTextChanged, when a layout setting changes), the hot-key
// position and text position, hot-key color +0x1B4, four text colors from +0x1B8 and four drop
// colors from +0x1C8 (setColors 0x00105DC0 fills each four-word group; setTextColor/setDropColor
// copy four ints), size +0x1DC/+0x1E0 (getSize 0x00105E79), the clip region +0x1E4 and the frame the
// strings were last used on +0x1F4 (W3DDisplayStringManager::update resets strings idle for more
// than 60 frames). Slots 1..3 and 21..23 are DisplayString's own. Names are Zero Hour's.
#pragma once
#ifndef BFME2_W3DDISPLAYSTRING_H
#define BFME2_W3DDISPLAYSTRING_H

#include "GameClient/DisplayString.h"
#include "WW3D2/Render2DSentenceClass.h"
#include "Common/ICoord2D.h"
#include "Common/IRegion2D.h"

class W3DDisplayString : public DisplayString
{
public:
	W3DDisplayString();															// 0x00106088
	virtual ~W3DDisplayString();												// 0x00106162

	virtual void notifyTextChanged();											// 0x00106663
	virtual void reset();														// 0x001061BE
	virtual void setFont(GameFont *font);										// 0x001066B3
	virtual void setWordWrap(int wordWrap);										// 0x00105F6E
	virtual void setWordWrapCentered(bool isCentered);							// 0x00105FC6
	virtual void setColors(int textColor, int dropColor);						// 0x00105DC0
	virtual void setTextColor(int *colors);										// 0x00105E05
	virtual void setDropColor(int *colors);										// 0x00105E35
	virtual void draw(int x, int y, int textColor, int dropColor);				// 0x00106405
	virtual void draw(int x, int y);											// 0x00105E65
	virtual void getSize(int *width, int *height);								// 0x00105E79
	virtual int getWidth(int charPos = -1);										// 0x0010657E
	virtual void setScale(float x, float y);									// 0x00105FE4
	virtual void setTargetSize(float width, float height);						// 0x00106036
	virtual void setUseHotkey(bool useHotKey, int hotKeyColor);					// 0x00105F94
	virtual void setClipRegion(IRegion2D *region);								// 0x00105E9C

protected:
	void computeExtents();														// 0x001065D9

public:

	Render2DSentenceClass m_textRenderer;										// +0x14
	Render2DSentenceClass m_hotKeyRenderer;										// +0xD8
	UnicodeString m_hotKeyString;												// +0x19C
	bool m_textChanged;															// +0x1A0
	bool m_fontChanged;															// +0x1A1
	bool m_colorsChanged;														// +0x1A2
	bool m_useHotKey;															// +0x1A3
	ICoord2D m_hotKeyPos;														// +0x1A4
	ICoord2D m_textPos;															// +0x1AC
	int m_hotKeyColor;															// +0x1B4
	int m_textColors[4];														// +0x1B8
	int m_dropColors[4];														// +0x1C8
	int m_colorKey;																// +0x1D8
	int m_width;																// +0x1DC
	int m_height;																// +0x1E0
	IRegion2D m_clipRegion;														// +0x1E4
	unsigned int m_lastResourceFrame;											// +0x1F4
};
// SIZE 0x1F8

typedef char W3DDisplayStringSizeCheck[sizeof(W3DDisplayString) == 0x1F8 ? 1 : -1];

#endif
