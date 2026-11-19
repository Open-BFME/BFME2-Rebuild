// BridgeTowerBehaviorInterface.h: the interface a bridge tower module offers its bridge.
//
// Target facts: BridgeTowerBehavior's own table sits at +0x10 of the module (the class
// derives from this interface there). Slot 0 is setBridge (retail 0x004586EE); slots 1 and 2
// (bridge id getter, tower type setter) are the table's other two entries, bodies retail
// folded into 0x001DB0A8 and 0x005F69CE (table 0x00C40C9C, three entries then the next table).
#pragma once
#ifndef BFME2_BRIDGE_TOWER_BEHAVIOR_INTERFACE_H
#define BFME2_BRIDGE_TOWER_BEHAVIOR_INTERFACE_H

#include "Common/GameType.h"

class Object;
enum BridgeTowerType;

class BridgeTowerBehaviorInterface
{
public:
	virtual void setBridge(Object *bridge) = 0;	// slot 0, retail 0x004586EE
	virtual ObjectID getBridgeID() = 0;			// slot 1
	virtual void setTowerType(BridgeTowerType type) = 0;	// slot 2
};

#endif
