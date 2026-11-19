#pragma once
// Native GameMakeColor4145E packs four byte channels as ARGB.
inline int GameMakeColor(unsigned char r,unsigned char g,unsigned char b,unsigned char a)
{
    return (a << 24) | (r << 16) | (g << 8) | b;
}
void GameGetColorComponents(int color, unsigned char *red, unsigned char *green, unsigned char *blue, unsigned char *alpha);	// 0x002D2A9F
