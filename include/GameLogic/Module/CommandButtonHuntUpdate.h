// CommandButtonHuntUpdate.h: the CommandButtonHuntUpdate module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004952CF calls UpdateModule first; it stores the
// tables 0x00C4ECFC at +0x0, 0x00BEFF90 at +0xC, 0x00C4ECF0 at +0x10. Members are what xfer (0x004953EE) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x0049533E ?Rva0049533EGet@@YAHXZ
//   +0x0 slot 13 -> 0x00495ABA ?update@DemoTrapUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x0 slot 14 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x0 slot 15 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x0 slot 18 -> 0x0049592F ?name@Rva0049592FNamed@@QBEPBDXZ
//   +0x0 slot 21 -> 0x004959FF ?onObjectCreated@DemoTrapUpdate@@UAEXXZ
//   +0x0 slot 22 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 25 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x0 slot 26 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 28 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x0 slot 32 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 33 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 43 -> 0x000B69A1 ?slot14@ModuleData@@UBE?AVAsciiString@@H@Z
//   +0x0 slot 59 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x10 slot 0 -> 0x004958AE ?update@Rva004958AE@@QAEHXZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?, slot4 +0x20.
#pragma once
#ifndef BFME2_COMMAND_BUTTON_HUNT_UPDATE_H
#define BFME2_COMMAND_BUTTON_HUNT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "Common/AsciiString.h"

class Thing;
class ModuleData;
class Xfer;
class CommandButton;
class Object;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char CommandButtonHuntUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class CommandButtonHuntUpdate : public UpdateModule
{
public:
	CommandButtonHuntUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004958AE)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual ~CommandButtonHuntUpdate();

protected:
	Object *scanClosestTarget();	// 0x00495489
public:
	AsciiString m_commandButtonName;               // +0x20; the constructor sets it to the empty string
	const CommandButton *m_commandButton;          // +0x24; the constructor clears it
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x28
typedef char CommandButtonHuntUpdate_size[(sizeof(CommandButtonHuntUpdate) == 0x28) ? 1 : -1];
typedef char CommandButtonHuntUpdate_off_m_commandButtonName[(offsetof(CommandButtonHuntUpdate, m_commandButtonName) == 0x20) ? 1 : -1];

#endif
