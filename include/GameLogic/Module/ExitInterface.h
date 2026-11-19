// ExitInterface.h: the interface a module implements to let objects out of it.
//
// Target facts: OpenContain's constructor 0x004649F8 stores the interface's
// own table 0x00C430B4 at +0x30 (12 slots), then OpenContain's 0x00C43380:
// 2 exitObjectViaDoor 0x0046525F, 5 exitObjectInAHurry 0x004656DF and
// 9 getNaturalRallyPoint 0x0046363F are rowed there; HordeGarrisonContain's
// isExitBusy 0x00479BCA is slot 0 (const, returns bool). The interface's own
// table 0x00C430B4 is pure in slots 0-4, 7 and 8 and has defaults elsewhere:
// 5 an empty `ret 4`, 6 returns false, 9 ExitInterface::getNaturalRallyPoint
// 0x004624A7 (zeroes the point, returns false), 10 0x004624C1 (getExitPosition's
// default) and 11 an empty body. There is no virtual destructor (the twelve
// slots are all accounted for).
// Donor facts: Zero Hour's ExitInterface supplies the names and order, which
// BFME 2's twelve slots keep.
#pragma once
#ifndef BFME2_EXITINTERFACE_H
#define BFME2_EXITINTERFACE_H

#include "Common/GameType.h"

class Object;
class ThingTemplate;
class Coord3D;
// Door values evidenced by QueueProductionExitUpdate::reserveDoorForExit (0x004A0268): 0 for the
// one door a free exit uses, -1 when the exit is busy. Enumerator names are not evidenced beyond that.
enum ExitDoorType
{
	EXIT_DOOR_NONE = -1,
	EXIT_DOOR_FIRST = 0
};

class ExitInterface
{
public:
	virtual Bool isExitBusy() const = 0;														// 0
	virtual ExitDoorType reserveDoorForExit(const ThingTemplate *objType, Object *specificObject) = 0;	// 1
	virtual void exitObjectViaDoor(Object *newObj, ExitDoorType exitDoor) = 0;				// 2
	virtual void exitObjectByBudding(Object *newObj, Object *budHost) = 0;					// 3
	virtual void unreserveDoorForExit(ExitDoorType exitDoor) = 0;								// 4
	virtual void exitObjectInAHurry(Object *newObj) {}											// 5
	virtual Bool useSpawnRallyPoint() const { return false; }									// 6
	virtual void setRallyPoint(const Coord3D *pos) = 0;										// 7
	virtual const Coord3D *getRallyPoint() const = 0;											// 8
	virtual Bool getNaturalRallyPoint(Coord3D &rallyPoint, Bool offset) const;					// 9, 0x004624A7
	virtual Bool getExitPosition(Coord3D &exitPosition, Real &exitAngle) const;				// 10, 0x004624C1
	virtual void notifyProductionBurstFinished() {}												// 11
};

#endif
