// HordeTransportContain.h: the transport a horde rides in. SIZE 0x128.
//
// Target facts: the constructor 0x00477003 calls TransportContain's
// 0x00468559, runs a one-byte member's constructor at +0x11D (the folded empty
// body the map names RealRange's), installs its tables (primary 0x00C45EB8,
// exit 0x00C45CFC, contain 0x00C45D30) and sets +0x120 = -1000 and the byte
// +0x124 = 0. xfer 0x0047714A writes its version, TransportContain::xfer, then
// xferInt(+0x120) and xferBool(+0x124). +0x11C..+0x11F are not established.
// Handed to h05's generator for the remaining members and overrides.
#pragma once
#ifndef BFME2_HORDETRANSPORTCONTAIN_H
#define BFME2_HORDETRANSPORTCONTAIN_H

#include "GameLogic/Module/TransportContain.h"

class HordeTransportContain : public TransportContain
{
public:
	HordeTransportContain(Thing *thing, const ModuleData *moduleData);	// 0x00477003

	virtual NameKeyType getModuleNameKey() const;						// 0x00477185
	virtual void orderAllPassengersToExit(CommandSourceType cmdSource);	// ContainModuleInterface 32, 0x004772B8 (forwards to the horde exit helper at +0xFD)

protected:
	virtual const char *GetSnapshotName() const;						// 0x00477098
	virtual void xfer(Xfer *xfer);										// 0x0047714A

public:
	UnsignedByte m_unk0x11C[4];											// 0x11C not established
	Int m_unk0x120;														// 0x120 = -1000, xferInt
	Bool m_unk0x124;													// 0x124 xferBool
};
// SIZE 0x128

typedef char HordeTransportContain_off_m_unk0x120[(offsetof(HordeTransportContain, m_unk0x120) == 0x120) ? 1 : -1];
typedef char HordeTransportContain_size[(sizeof(HordeTransportContain) == 0x128) ? 1 : -1];

#endif
