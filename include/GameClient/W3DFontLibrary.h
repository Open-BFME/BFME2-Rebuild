// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DFontLibrary: the font library whose fonts are W3D FontCharsClass objects (Zero Hour's W3DGameFont.h).
// Target facts: the factory W3DGameClient 0x0004C4F9 allocates 0x2C bytes (FontLibrary's size), calls FontLibrary's
// constructor 0x00218942 and stores the table VA 0x00BC4818 inline; the destructor is 0x0009023C, loadFontData
// 0x000902A6 and releaseFontData 0x00090000. It adds no data members.
#pragma once
#ifndef BFME2_W3DFONTLIBRARY_H
#define BFME2_W3DFONTLIBRARY_H

#include "GameClient/FontLibrary.h"

class W3DFontLibrary : public FontLibrary
{
public:
	virtual ~W3DFontLibrary();								// 0x0009023C
protected:
	virtual bool loadFontData(GameFont *font);				// 0x000902A6
	virtual void releaseFontData(GameFont *font);			// 0x00090000
};

typedef char W3DFontLibrarySize[(sizeof(W3DFontLibrary) == 0x2C) ? 1 : -1];

#endif
