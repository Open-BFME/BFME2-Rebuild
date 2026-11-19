// DefaultProductionExitUpdate.h: the default exit for produced units. SIZE 0x34.
//
// Target facts: the constructor 0x00487FC7 calls UpdateModule's, stores
// ExitInterface's own table 0x00C430B4 at +0x20 and then its own tables
// (0x00C4B2AC +0x00, 0x00C4B1F0 +0x0C, 0x00C4B1E0 +0x10, 0x00C4B1B0 +0x20); the
// factory allocates 0x34. setRallyPoint 0x00487F60 copies the point to +0x24
// and sets the byte +0x30; getRallyPoint 0x00487F75 returns +0x24 only while
// +0x30 is set. xfer 0x0048805B: UpdateModule::xfer, then (unless the Xfer is a
// light CRC) version 1, the Coord3D +0x24 and the Bool +0x30.
// (h05 modgen generated the first draft; h08 placed ExitInterface at +0x20.)
// Donor facts: Zero Hour's DefaultProductionExitUpdate names m_rallyPoint and
// m_rallyPointExists.
#pragma once
#ifndef BFME2_DEFAULT_PRODUCTION_EXIT_UPDATE_H
#define BFME2_DEFAULT_PRODUCTION_EXIT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/ExitInterface.h"

class Thing;
class ModuleData;
class Xfer;

class DefaultProductionExitUpdate : public UpdateModule, public ExitInterface	// +0x20
{
public:
	virtual ExitInterface *getUpdateExitInterface() { return this; }
	DefaultProductionExitUpdate(Thing *thing, const ModuleData *moduleData);	// 0x00487FC7
	virtual NameKeyType getModuleNameKey() const;						// 0x00487F82
	virtual const char *GetSnapshotName() const;						// 0x00487F5A
	virtual UpdateSleepTime update() { return UPDATE_SLEEP_FOREVER; }

	// ExitInterface
	virtual void exitObjectViaDoor(Object *newObj, ExitDoorType exitDoor);	// 0x00488099
	virtual void setRallyPoint(const Coord3D *pos);						// 0x00487F60
	virtual const Coord3D *getRallyPoint() const;						// 0x00487F75
	// Primary-table slot 13 (table 0x0084B2AC has 14 slots): 0x004A6642, shared with the other production exits; the
	// ledger's name, one Coord3D argument (not ExitInterface's two-argument getExitPosition).
	virtual Bool getExitPosition(Coord3D &exitPosition) const;
	// Retail's table at +0x20 holds this class's own for every slot (h05, gen3 vtable-content list):
	virtual Bool isExitBusy() const { return false; }					// 0: the shared `xor al,al; ret` 0x0047A699
	virtual ExitDoorType reserveDoorForExit(const ThingTemplate *objType, Object *specificObject) { return (ExitDoorType)0; }	// 1: the shared `xor eax,eax; ret 8` 0x005748B2
	virtual void exitObjectByBudding(Object *newObj, Object *budHost) {}	// 3: the shared `ret 8` 0x0050B238
	virtual void unreserveDoorForExit(ExitDoorType exitDoor) {}			// 4: the shared `ret 4` 0x0047A69C
	virtual Bool getNaturalRallyPoint(Coord3D &rallyPoint, Bool offset) const;	// 9: folded with QueueProductionExitUpdate's 0x004A027A

protected:
	virtual void xfer(Xfer *xfer);										// 0x0048805B

public:
	Coord3D m_rallyPoint;												// +0x24
	Bool m_rallyPointExists;											// +0x30
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x34
typedef char DefaultProductionExitUpdate_size[(sizeof(DefaultProductionExitUpdate) == 0x34) ? 1 : -1];
typedef char DefaultProductionExitUpdate_off_m_rallyPoint[(offsetof(DefaultProductionExitUpdate, m_rallyPoint) == 0x24) ? 1 : -1];
typedef char DefaultProductionExitUpdate_off_m_rallyPointExists[(offsetof(DefaultProductionExitUpdate, m_rallyPointExists) == 0x30) ? 1 : -1];

#endif
