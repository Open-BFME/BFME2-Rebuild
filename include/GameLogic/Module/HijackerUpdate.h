// HijackerUpdate.h: promoted from include/Draft by h05 (round 10). Proven by 4 rows that MATCH against it on main (0x004A3F81, 0x004A4010, 0x004A40BB, 0x004A40DB);
// name, size, bases and virtual order are retail facts given below.
// HijackerUpdate.h: the HijackerUpdate module. SIZE 0x34 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A4055 calls UpdateModule first; it stores the
// tables 0x00C525E4 at +0x0, 0x00BEFF90 at +0xC, 0x00C525D8 at +0x10. Members are what xfer (0x004A3FAA) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004A40BB ?loadPostProcess@HijackerUpdate@@MAEXXZ
//   +0x0 slot 2 -> 0x004A3FA4 ?name@Rva004A3FA4Named@@QBEPBDXZ
//   +0x0 slot 4 -> 0x004A4010 ?rva0004A4010@HijackerUpdate@@SA?AW4NameKeyType@@XZ
//   +0x10 slot 0 -> 0x004A40DB ?update@HijackerUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_HIJACKER_UPDATE_H
#define BFME2_HIJACKER_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "Common/Coord3D.h"

class Object;
class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char HijackerUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class HijackerUpdate : public UpdateModule
{
public:
	HijackerUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	void setIsInVehicle(Bool inVehicle);			// 0x004A3F81
	void setTargetObject(const Object *target);		// 0x003A4341 (public QAE in the map)
	Object *getTargetObject() const;				// 0x003A445C (public QBE; update 0x004A40DB asks it for the object to follow)

protected:
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess();   // 0x004A40BB: UpdateModule::loadPostProcess, then the target from the ID at +0x20

public:
	ObjectID m_targetID;                            // +0x20; xfer ObjectID
	Coord3D m_ejectPos;                             // +0x24; xfer Coord3D
	Bool m_update;                                // +0x30; xfer Bool
	Bool m_isInVehicle;                                // +0x31; xfer Bool; setIsInVehicle 0x004A3F81 stores its Bool here (bytes equal)
	Bool m_wasTargetAirborne;                                // +0x32; xfer Bool
public:
	virtual UpdateSleepTime update();	// 0x004A40DB, UpdateModuleInterface slot 0; retail's table has this class's own (h05, gen3 vtable-content list)
};
// SIZE 0x34
typedef char HijackerUpdate_size[(sizeof(HijackerUpdate) == 0x34) ? 1 : -1];
typedef char HijackerUpdate_off_m_targetID[(offsetof(HijackerUpdate, m_targetID) == 0x20) ? 1 : -1];
typedef char HijackerUpdate_off_m_ejectPos[(offsetof(HijackerUpdate, m_ejectPos) == 0x24) ? 1 : -1];
typedef char HijackerUpdate_off_m_update[(offsetof(HijackerUpdate, m_update) == 0x30) ? 1 : -1];
typedef char HijackerUpdate_off_m_isInVehicle[(offsetof(HijackerUpdate, m_isInVehicle) == 0x31) ? 1 : -1];
typedef char HijackerUpdate_off_m_wasTargetAirborne[(offsetof(HijackerUpdate, m_wasTargetAirborne) == 0x32) ? 1 : -1];

#endif
