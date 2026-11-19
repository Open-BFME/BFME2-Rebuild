// Font3D.h -- WW3D2 bitmap font data and its per-instance view.
//
// Peek_Texture on the instance forwards to the data object's Texture member:
// retail loads the FontData pointer from +0x08 (the first member after the
// RefCountClass base) and the texture from +0x0C of that object.
#pragma once

#include "WWLib/RefCountClass.h"

class TextureClass;

class Font3DDataClass : public RefCountClass
{
public:
	TextureClass * Peek_Texture(void) { return Texture; }

	char * Name;			// +0x08

private:
	TextureClass * Texture;	// +0x0C
};

class Font3DInstanceClass : public RefCountClass
{
public:
	TextureClass * Peek_Texture(void);

private:
	Font3DDataClass * FontData;	// +0x08
};
// SIZE 0x0C
