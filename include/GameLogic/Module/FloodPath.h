#pragma once
#include "Common/Array.h"
#include "Common/Coord3D.h"
#include <stdlib.h>
struct FloodPathAllocator { FloodPathAllocator() {} };
class FloodPath : public Array<Coord3D>
{
public:
 FloodPath(const FloodPathAllocator & = FloodPathAllocator()) throw();
 ~FloodPath() { if(m_begin) free(m_begin); }
};
// SIZE 0x0C
