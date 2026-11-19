// LaserUpdateModuleData.h: the laser update module's INI data. SIZE 0x1C.
// Target facts: ctor 0x00363147 stores vtable 0x00C17120, nulls the three
// AsciiStrings at +0x08/+0x0C/+0x14, clears the bool at +0x10 and zeroes the
// real at +0x18. Destructor 0x003631E8 releases +0x14, +0x0C and +0x08 and
// restores the base vtable. buildFieldParse 0x00363166 registers the retail
// table at 0x00C17208 with offset 0. The factory news 0x1C bytes.
#pragma once
#ifndef BFME2_LASERUPDATEMODULEDATA_H
#define BFME2_LASERUPDATEMODULEDATA_H

#include "Common/AsciiString.h"
#include "GameLogic/Module/ModuleData.h"

class MultiIniFieldParse;

class LaserUpdateModuleData : public ModuleData
{
public:
	LaserUpdateModuleData();


	static void buildFieldParse(MultiIniFieldParse &parse);

	AsciiString m_muzzleParticleSystemName;			// 0x08
	AsciiString m_parentFireBoneName;				// 0x0C
	Bool m_parentFireBoneOnTurret;					// 0x10
	UnsignedByte m_unk0x11[3];
	AsciiString m_targetParticleSystemName;			// 0x14
	Real m_lifetimeMsec;							// 0x18
};
// SIZE 0x1C

#endif
