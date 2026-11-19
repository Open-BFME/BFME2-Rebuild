#pragma once
#include "Common/SubsystemInterface.h"
#include "GameClient/TerrainRoadType.h"
// Target ctor2DAC34/table803D64,14 slots; listsC/10 walked by dtor2DAC58.
// ZH TerrainRoads.h names; BFME2 uses the expanded SubsystemInterface base.
class TerrainRoadCollection : public SubsystemInterface {
public:
    TerrainRoadCollection();
    virtual ~TerrainRoadCollection();
    virtual void init() {}
    virtual void reset() {}
    virtual void update() {}
    TerrainRoadType *findRoad(AsciiString);
    TerrainRoadType *findBridge(AsciiString);
    TerrainRoadType *findRoadOrBridge(AsciiString);
    TerrainRoadType *newRoad(AsciiString);
    TerrainRoadType *newBridge(AsciiString);
    TerrainRoadType *nextRoad(TerrainRoadType *);
private:
    TerrainRoadType *m_roadList,*m_bridgeList;
    static unsigned int m_idCounter;
};
typedef char TerrainRoadCollectionSizeCheck[sizeof(TerrainRoadCollection)==0x14 ? 1 : -1];
extern TerrainRoadCollection *TheTerrainRoads;