// DestroyEnvironmentUpdate.h: the DestroyEnvironmentUpdate module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004AC63A calls UpdateModule first; it stores the
// tables 0x00C54D64 at +0x0, 0x00BEFF90 at +0xC, 0x00C54D58 at +0x10. Members are what xfer (0x004AC6CB) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 13 -> 0x004ACBB3 ?update@PartTheHeavensUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x0 slot 14 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x0 slot 15 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x0 slot 17 -> 0x004ACBE8 ?loadPostProcess@PartTheHeavensUpdate@@MAEXXZ
//   +0x0 slot 21 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 25 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x0 slot 26 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 28 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x10 slot 0 -> 0x004AC84B ?update@DestroyEnvironmentUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_DESTROY_ENVIRONMENT_UPDATE_H
#define BFME2_DESTROY_ENVIRONMENT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char DestroyEnvironmentUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class DestroyEnvironmentUpdate : public UpdateModule
{
public:
	DestroyEnvironmentUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~DestroyEnvironmentUpdate();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004AC84B)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_unk0x20;                         // +0x20; xfer UnsignedInt
	ObjectID m_unk0x24;                            // +0x24; xfer ObjectID
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x28
typedef char DestroyEnvironmentUpdate_size[(sizeof(DestroyEnvironmentUpdate) == 0x28) ? 1 : -1];
typedef char DestroyEnvironmentUpdate_off_m_unk0x20[(offsetof(DestroyEnvironmentUpdate, m_unk0x20) == 0x20) ? 1 : -1];
typedef char DestroyEnvironmentUpdate_off_m_unk0x24[(offsetof(DestroyEnvironmentUpdate, m_unk0x24) == 0x24) ? 1 : -1];

#endif
