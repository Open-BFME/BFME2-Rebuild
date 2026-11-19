#pragma once
#include "Common/ICoord3D.h"
#include <stddef.h>
// SIZE 0x18, no base or virtual functions. Retail XferSaveAsText's
// transfer 0x0060DADA reads the six signed integer components at 0..0x14;
// old private views agree on two ICoord3D corners, lo followed by hi.
struct IRegion3D
{
    ICoord3D lo; // 0x0
    ICoord3D hi; // 0xC
};
typedef char IRegion3DSize[(sizeof(IRegion3D) == 0x18) ? 1 : -1];
typedef char IRegion3DHiOffset[(offsetof(IRegion3D,hi) == 0xC) ? 1 : -1];
