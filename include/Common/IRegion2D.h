#pragma once
#include "Common/ICoord2D.h"
#include <stddef.h>
// SIZE 0x10, no base or virtual functions. Retail buildRegion 0x0030F8A2
// writes lo.x/lo.y/hi.x/hi.y at +0/+4/+8/+C; appendPixelRegionArgument
// 0x0030F9F6 and text transfer 0x0060DBD8 read the same four ints.
struct IRegion2D
{
    ICoord2D lo; // 0x0
    ICoord2D hi; // 0x8
};
typedef char IRegion2DSize[(sizeof(IRegion2D) == 0x10) ? 1 : -1];
typedef char IRegion2DHiOffset[(offsetof(IRegion2D,hi) == 8) ? 1 : -1];
