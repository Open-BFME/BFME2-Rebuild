#pragma once
#include <stddef.h>
// SIZE 0x8, no base or virtual functions. Retail buildRegion 0x0030F8A2,
// appendPixelArgument 0x0030F9D8 and text transfer 0x0060DB58 read two ints.
// Old private views and Lib/ICoord2D.h agree on the component names.
struct ICoord2D
{
    int x; // 0x0
    int y; // 0x4
};
typedef char ICoord2DSize[(sizeof(ICoord2D) == 8) ? 1 : -1];
typedef char ICoord2DYOffset[(offsetof(ICoord2D,y) == 4) ? 1 : -1];
