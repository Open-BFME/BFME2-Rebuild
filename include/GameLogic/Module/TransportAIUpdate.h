// TransportAIUpdate.h: the TransportAIUpdate module. SIZE 0x3E4 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A9042 calls AIUpdateInterface first; it stores the
// tables 0x00C53AA8 at +0x0, 0x00BFA3C0 at +0xC, 0x00C53A98 at +0x10, 0x00C47B88 at +0x20, 0x00C52F88 at +0x24. Members are what xfer (0x0049060D) transfers after AIUpdateInterface::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x0049A7EE ?loadPostProcess@WorkerAIUpdate@@MAEXXZ
//   +0x0 slot 2 -> 0x004A907A ?Rva004A907AGet@@YAHXZ
//   +0x0 slot 34 -> 0x004A9147 ?privateAttackObject@TransportAIUpdate@@MAEXPAVObject@@HW4CommandSourceType@@@Z
//   +0x0 slot 38 -> 0x004A91E0 ?privateForceAttackObject@TransportAIUpdate@@MAEXPAVObject@@HW4CommandSourceType@@@Z
//   +0x0 slot 40 -> 0x004A9279 ?privateAttackPosition@TransportAIUpdate@@MAEXPBUCoord3D@@HW4CommandSourceType@@@Z
#pragma once
#ifndef BFME2_TRANSPORT_A_I_UPDATE_H
#define BFME2_TRANSPORT_A_I_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/AIUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// AIUpdateInterface ends at 0x3E4; this class's members follow it.
typedef char TransportAIUpdate_base_size[(sizeof(AIUpdateInterface) == 0x3E4) ? 1 : -1];

class TransportAIUpdate : public AIUpdateInterface
{
public:
	TransportAIUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~TransportAIUpdate();

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual AIStateMachine *makeStateMachine();		// 0x004A90EC
	virtual void privateAttackObject(Object *, Int, CommandSourceType);	// override: privateAttackObject slot 34 at +0x0 (0x004A9147)
	virtual void privateForceAttackObject(Object *, Int, CommandSourceType);	// override: privateForceAttackObject slot 38 at +0x0 (0x004A91E0)
	virtual void privateAttackPosition(const Coord3D *, Int, CommandSourceType);	// override: privateAttackPosition slot 40 at +0x0 (0x004A9279)

public:
protected:
	virtual void loadPostProcess() { AIUpdateInterface::loadPostProcess(); }	// retail slot 0x0049A7EE: a jmp to AIUpdateInterface's, ICF-folded across classes (h05)
};
// SIZE 0x3E4
typedef char TransportAIUpdate_size[(sizeof(TransportAIUpdate) == 0x3E4) ? 1 : -1];

#endif
