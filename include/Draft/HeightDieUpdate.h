// HeightDieUpdate.h: the module that destroys a thing when it falls below a height.
//
// Target facts: the constructor 0x0048FCB2 calls UpdateModule's constructor
// (0x00253390) and then seeds three members — the two flags at +0x20/+0x21,
// the three -1.0f reals at +0x24 (transferred as one coordinate by xfer), and
// the word at +0x30 set to -1. xfer 0x0048FBD9 transfers UpdateModule first,
// bails on a light CRC, then xfer->Version1() and those four members in
// address order.
#pragma once
#ifndef BFME2_HEIGHTDIEUPDATE_H
#define BFME2_HEIGHTDIEUPDATE_H

#include "Common/Coord3D.h"
#include "GameLogic/Module/UpdateModule.h"

class Xfer;

class HeightDieUpdate : public UpdateModule
{
public:
	HeightDieUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~HeightDieUpdate();

	virtual void loadPostProcess(void);
	virtual NameKeyType getModuleNameKey(void) const;

protected:
	virtual const char *GetSnapshotName(void) const;
	virtual void xfer(Xfer *xfer);

public:
	Bool m_unk0x20;			// +0x20
	Bool m_unk0x21;			// +0x21
	Coord3DBase m_unk0x24;		// +0x24
	UnsignedInt m_unk0x30;		// +0x30
public:
	virtual UpdateSleepTime update();	// 0x0048FD1B, UpdateModuleInterface slot 0; retail's table has this class's own (h05, gen3 vtable-content list)
};

#endif
