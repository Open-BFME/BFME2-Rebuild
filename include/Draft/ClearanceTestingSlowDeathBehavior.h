// DRAFT (h05 modgen): generated from constructor RVA 0x00483D39 and xfer RVA 0x00483E0A; no function
// matches against it yet, so it stays in Draft until one does.
// ClearanceTestingSlowDeathBehavior.h: the ClearanceTestingSlowDeathBehavior module. SIZE 0x54 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00483D39 calls SlowDeathBehavior first; it stores the
// tables 0x00BE2B78 at +0x50, 0x00C49E4C at +0x0, 0x00C49D90 at +0xC, 0x00C42034 at +0x10, 0x00C42030 at +0x20, 0x00C49D7C at +0x24, 0x00C49D70 at +0x50. Members are what xfer (0x00483E0A) transfers after SlowDeathBehavior::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x50 slot 0 -> 0x0003B810 __purecall
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x00483C63 ?name@ClearanceTestingSlowDeathBehavior@@QBEPBDXZ
//   +0x0 slot 13 -> 0x00483C69 ?rva00483C69@ClearanceTestingSlowDeathBehavior@@UAEPAVRva00483C69Interface@@XZ
//   +0x0 slot 15 -> 0x00483C9F ?allow@Rva00483C9FFilter@@UAE_NPAVObject@@@Z
//   +0x0 slot 16 -> 0x0036CC7A ?Get_File_Handle@FileClass@@UAEPAXXZ
//   +0xC slot 24 -> 0x00483C75 ?get@Rva00483C75LeaNegSbbField@@QBEHXZ
//   +0x24 slot 3 -> 0x00483F19 ?rva00483F19@ClearanceTestingSlowDeathBehavior@@UAE_NXZ
//   +0x50 slot 0 -> 0x00483D9B ?get@Rva00483D9BAddImm32Field@@QBEHXZ
//   +0x50 slot 1 -> 0x00483DA4 ?getClearanceTestGeometryCenter@ClearanceTestingSlowDeathBehavior@@UAEXPAUCoord3D@@@Z
//   +0x50 slot 2 -> 0x00483DF8 ?get@Rva001F7270@@QBEMXZ
//   +0x50 slot 3 -> 0x0045DE65 ?beginSlowDeath@SlowDeathBehavior@@UAEXPBVDamageInfo@@@Z
//   +0x50 slot 4 -> 0x0045D5DA ?getProbabilityModifier@SlowDeathBehavior@@UBEHPBVDamageInfo@@@Z
//   +0x50 slot 5 -> 0x0045D49E ?isDieApplicable@SlowDeathBehavior@@UBE_NPBVDamageInfo@@@Z
// Not established: interface tables at +0x50 (their vptrs sit in the gaps there).
#pragma once
#ifndef BFME2_CLEARANCE_TESTING_SLOW_DEATH_BEHAVIOR_H
#define BFME2_CLEARANCE_TESTING_SLOW_DEATH_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/SlowDeathBehavior.h"

class Thing;
class ModuleData;
class Xfer;

// SlowDeathBehavior ends at 0x50; this class's members follow it.
typedef char ClearanceTestingSlowDeathBehavior_base_size[(sizeof(SlowDeathBehavior) == 0x50) ? 1 : -1];

class ClearanceTestingSlowDeathBehavior : public SlowDeathBehavior
{
public:
	char const * name() const;	// 0x00483C63
	ClearanceTestingSlowDeathBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x50[0x4];
protected:
	virtual void loadPostProcess() { SlowDeathBehavior::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SlowDeathBehavior's (h05)
};
// SIZE 0x54
typedef char ClearanceTestingSlowDeathBehavior_size[(sizeof(ClearanceTestingSlowDeathBehavior) == 0x54) ? 1 : -1];

#endif
