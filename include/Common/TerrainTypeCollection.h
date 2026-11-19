#pragma once
#include "Common/AsciiString.h"
#include "Common/SubsystemInterface.h"
class TerrainType;
// Target constructor 0x317BA5 chains SubsystemInterface and zeroes +0xC.
class TerrainTypeCollection : public SubsystemInterface
{
public:
    TerrainTypeCollection();
    virtual ~TerrainTypeCollection();
    virtual void init() {}
    virtual void reset() {}
    virtual void update() {}
    TerrainType *findTerrain(AsciiString name);
    TerrainType *newTerrain(AsciiString name);
private:
    TerrainType *m_head; // 0x0C
};
typedef char check_TerrainTypeCollection_size[sizeof(TerrainTypeCollection) == 0x10 ? 1 : -1];
