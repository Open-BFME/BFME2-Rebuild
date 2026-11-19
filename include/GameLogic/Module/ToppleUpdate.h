// ToppleUpdate.h: promoted from include/Draft by h05 (round 10). Proven by 2 rows that MATCH against it on main (0x004A8125, 0x004A812E);
// name, size, bases and virtual order are retail facts given below.
// ToppleUpdate.h: the ToppleUpdate module. SIZE 0x58 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A8201 calls UpdateModule first; it stores the
// tables 0x00C40818 at +0x20, 0x00C5386C at +0x0, 0x00C5A260 at +0xC, 0x00C53860 at +0x10, 0x00C53848 at +0x20. Members are what xfer (0x004A86E7) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x20 slot 0 -> 0x0003B810 __purecall
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004A80B1 ?name@Rva004A80B1Named@@QBEPBDXZ
//   +0x0 slot 14 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 16 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 17 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 27 -> 0x000B69A1 ?slot14@ModuleData@@UBE?AVAsciiString@@H@Z
//   +0x0 slot 29 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 43 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0xC slot 1 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0xC slot 49 -> 0x004BBEFD ?name@AODCrushCollide@@QBEPBDXZ
//   +0x10 slot 0 -> 0x004A878F ?update@ToppleUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 0 -> 0x004A82F5 ?rva004A82F5@Rva004A82F5@@QAEXPAVObject@@HH@Z
//   +0x20 slot 1 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x20 slot 7 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 8 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x20 slot 18 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x20 slot 21 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?, XferRawBytes +0x38.
// Interfaces after the base (own table -> class): +0x20 CollideModuleInterface.
#pragma once
#ifndef BFME2_TOPPLE_UPDATE_H
#define BFME2_TOPPLE_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/CollideModule.h"
#include "Common/Coord3D.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char ToppleUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class ToppleUpdate : public UpdateModule, public CollideModuleInterface
{
public:
	ToppleUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	bool isAbleToBeToppled() const; // 0x004A8125
	void applyTopplingForce(const Coord3D *toppleDirection, Real toppleSpeed, UnsignedInt options); // 0x004A83D7 (Object::topple 0x0028CF37)
	virtual UpdateSleepTime update();	// UpdateModuleInterface slot 0, 0x004A878F
	virtual const char *GetSnapshotName() const;

protected:
	virtual void xfer(Xfer *xfer);

public:
	Real m_angularVelocity;                        // +0x24; xfer Real
	Real m_angularAcceleration;                    // +0x28; xfer Real
	Coord3D m_toppleDirection;                     // +0x2C; xfer Coord3D
	Int m_toppleState;                             // +0x38 (isAbleToBeToppled 0x004A8125 compares it with 0 = upright; update 0x004A878F sleeps for 0 and 2 = down)
	Real m_angularAccumulation;                    // +0x3C; xfer Real
	Real m_angleDeltaX;                            // +0x40; xfer Real
	Int m_numAngleDeltaX;                          // +0x44; xfer Int
	Bool m_doBounceFX;                             // +0x48; xfer Bool
	UnsignedInt m_options;                         // +0x4C; xfer UnsignedInt
	ObjectID m_stumpID;                            // +0x50; xfer ObjectID
	Real m_bounceDamping;                          // +0x54; xfer Real; update 0x004A878F subtracts it from the module data's bounce velocity, then adds 0.025
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x58
typedef char ToppleUpdate_size[(sizeof(ToppleUpdate) == 0x58) ? 1 : -1];
typedef char ToppleUpdate_off_m_angularVelocity[(offsetof(ToppleUpdate, m_angularVelocity) == 0x24) ? 1 : -1];
typedef char ToppleUpdate_off_m_angularAcceleration[(offsetof(ToppleUpdate, m_angularAcceleration) == 0x28) ? 1 : -1];
typedef char ToppleUpdate_off_m_toppleDirection[(offsetof(ToppleUpdate, m_toppleDirection) == 0x2C) ? 1 : -1];
typedef char ToppleUpdate_off_m_angularAccumulation[(offsetof(ToppleUpdate, m_angularAccumulation) == 0x3C) ? 1 : -1];
typedef char ToppleUpdate_off_m_angleDeltaX[(offsetof(ToppleUpdate, m_angleDeltaX) == 0x40) ? 1 : -1];
typedef char ToppleUpdate_off_m_numAngleDeltaX[(offsetof(ToppleUpdate, m_numAngleDeltaX) == 0x44) ? 1 : -1];
typedef char ToppleUpdate_off_m_doBounceFX[(offsetof(ToppleUpdate, m_doBounceFX) == 0x48) ? 1 : -1];
typedef char ToppleUpdate_off_m_options[(offsetof(ToppleUpdate, m_options) == 0x4C) ? 1 : -1];
typedef char ToppleUpdate_off_m_stumpID[(offsetof(ToppleUpdate, m_stumpID) == 0x50) ? 1 : -1];
typedef char ToppleUpdate_off_m_bounceDamping[(offsetof(ToppleUpdate, m_bounceDamping) == 0x54) ? 1 : -1];

#endif
