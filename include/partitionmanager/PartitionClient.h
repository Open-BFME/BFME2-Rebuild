// PartitionClient.h: an object as the partition manager sees it. SIZE 0x4.
// The class name is PROVISIONAL. Slot 7 carries WorldBuilder's name (the string
// Object::PartitionGetPlayerIndex is in WB 0x00CE1580, the last slot of WB's
// table 0x01E598EC); the other slot names are PROVISIONAL, from what Object's
// bodies do.
// Target facts (retail game.dat; functions by RVA, tables by VA):
//  - Its own table 0x00BFBC9C is eight __purecall (ICF-folded). Object's ctor
//    stores it at +0x6C inline, before its own.
//  - Object's table at +0x6C, 0x00BFC2B8: 0 0x005FBE57 &m_geometryInfo,
//    1 0x00299CBE Thing's position, 2 0x00299CC2 Thing's angle, 3 and 4
//    0x00299C74 the Object itself, 5 0x00299C7C stores its argument at +0x4C8,
//    6 0x00042272 returns it, 7 0x0028E7F8 PartitionGetPlayerIndex.
//  - Object 0x0028BAC0: if (+0x4C8) ThePartitionManager->...(this + 0x6C).
//    WorldBuilder's library has PartitionManager::Data (its ~Data string); the
//    data pointer stays void * until that class has a header.
#pragma once
#ifndef BFME2_PARTITIONCLIENT_H
#define BFME2_PARTITIONCLIENT_H

#include "Common/GameType.h"

class GeometryInfo;
class Object;
class Coord3D;

class PartitionClient
{
public:
	virtual const GeometryInfo *PartitionGetGeometryInfo() const = 0;	// slot 0
	virtual const Coord3D *PartitionGetPosition() const = 0;			// slot 1
	virtual Real PartitionGetOrientation() const = 0;					// slot 2
	// Slots 3 and 4 share one folded body; MSVC places an overload pair in
	// reverse declaration order.
	virtual Object *PartitionGetObject() = 0;							// slot 4
	virtual const Object *PartitionGetObject() const = 0;				// slot 3
	virtual void PartitionSetData(void *data) = 0;						// slot 5
	virtual void *PartitionGetData() const = 0;							// slot 6
	virtual Int PartitionGetPlayerIndex() const = 0;					// slot 7
};
// SIZE 0x4

#endif
