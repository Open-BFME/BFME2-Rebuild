// AttributeModifierPoolUpdate.h: the AttributeModifierPoolUpdate module. SIZE 0xA8 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00403BEF calls UpdateModule first; it stores the
// tables 0x00C3856C at +0x0, 0x00BEFF90 at +0xC, 0x00C38560 at +0x10. Members are what xfer (0x0040426D) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 0 -> 0x00403CA8 ?update@AttributeModifierPoolUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x10 slot 1 -> 0x003908A7 ?rva003908A7@@YGPAXPAX@Z
// Not established: transfers xfer makes that this view cannot place: slot4 ?, Int ?, Int ?, UnsignedInt ?, ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_ATTRIBUTE_MODIFIER_POOL_UPDATE_H
#define BFME2_ATTRIBUTE_MODIFIER_POOL_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char AttributeModifierPoolUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class AttributeModifierPoolUpdate : public UpdateModule
{
public:
	AttributeModifierPoolUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00403CA8)

	Bool rva00403382(Int attrKind, Real *value, Int flag);				// 0x00403382 (Object::rva0028C149 forwards to it)
	Bool rva00403448(Int a, Real *value, Int b, Int c);				// 0x00403448 (Object::rva0028C15E forwards to it)
	Bool addModifierToPool(const AsciiString &name, Int strength);		// 0x00403E72 (Object::addAttributeModifierToPool 0x0028EA91)
	void rva00403744(const AsciiString &name);						// 0x00403744 (map placeholder; Object::removeAttributeModifierFromPool 0x0028EB42 calls it)
	void rva00403415(Int *flags, Int frame);							// 0x00403415 (map ?rva00403415@AttributeModifierPoolUpdate@@QAEXPAHH@Z; SpecialPowerModule::ApplyPowerToObject 0x00493EA7 passes the anti-category mask and the frame)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x20[0x4C];
	Int m_unk0x6C;                                 // +0x6C; xfer Int
	UnsignedByte m_gap0x70[0x38];
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0xA8
typedef char AttributeModifierPoolUpdate_size[(sizeof(AttributeModifierPoolUpdate) == 0xA8) ? 1 : -1];
typedef char AttributeModifierPoolUpdate_off_m_unk0x6C[(offsetof(AttributeModifierPoolUpdate, m_unk0x6C) == 0x6C) ? 1 : -1];

#endif
