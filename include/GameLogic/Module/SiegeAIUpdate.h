// SiegeAIUpdate.h: the SiegeAIUpdate module. SIZE 0x3E4 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00490506 calls AIUpdateInterface first; it stores the
// tables 0x00C4D330 at +0x0, 0x00BFA3C0 at +0xC, 0x00C53A98 at +0x10, 0x00C47B88 at +0x20, 0x00C52F88 at +0x24. Members are what xfer (0x0049060D) transfers after AIUpdateInterface::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x0049A7EE ?loadPostProcess@WorkerAIUpdate@@MAEXXZ
//   +0x0 slot 2 -> 0x0049053E ?Rva0049053EGet@@YAHXZ
#pragma once
#ifndef BFME2_SIEGE_A_I_UPDATE_H
#define BFME2_SIEGE_A_I_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/AIUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// AIUpdateInterface ends at 0x3E4; this class's members follow it.
typedef char SiegeAIUpdate_base_size[(sizeof(AIUpdateInterface) == 0x3E4) ? 1 : -1];

class SiegeAIUpdate : public AIUpdateInterface
{
public:
	SiegeAIUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~SiegeAIUpdate();

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { AIUpdateInterface::loadPostProcess(); }	// retail slot 0x0049A7EE: a jmp to AIUpdateInterface's, ICF-folded across classes (h05)
};
// SIZE 0x3E4
typedef char SiegeAIUpdate_size[(sizeof(SiegeAIUpdate) == 0x3E4) ? 1 : -1];

#endif
