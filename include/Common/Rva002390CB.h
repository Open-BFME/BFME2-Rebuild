// Rva002390CB.h: an eight-byte sound/voice record held by module data
// (InitiateVoice, InitiateSound). SIZE 0x8. Original name not recovered.
// Target facts (old repo Code/GameEngine/Include/Common/Rva002390CB.h): the
// default constructor 0x004CEE6E writes -1 at +0 and zero at +4; the copy
// constructor is 0x002390CB; the reference at +4 is released through
// Release_Ref (0x00050ED3). BloodthirstyUpdateModuleData's destructor
// (0x0044E331) releases it inline.
#pragma once
#ifndef BFME2_RVA002390CB_H
#define BFME2_RVA002390CB_H

#include "Common/GameType.h"
#include "Common/OpaqueRefElement4.h"

class Rva002390CB
{
public:
	Rva002390CB();
	Rva002390CB(const Rva002390CB &other);
	~Rva002390CB()
	{
		if (m_unk0x04)
			m_unk0x04->Release_Ref();
	}

	// The reference at +4 viewed as the handle it is (OpaqueRefElement4 is one pointer): ActiveBody::onVeterancyLevelChanged 0x004BE47D
	// hands `&record + 4` to AudioEventRTS::assignSound 0x002D9C2F.
	const OpaqueRefElement4 &getSoundReference() const { return *(const OpaqueRefElement4 *)&m_unk0x04; }

	Int m_unk0x00;					// 0x0 index, -1 when unset
	OpaqueRefCounted *m_unk0x04;	// 0x4 released through OpaqueRefCounted::Release_Ref 0x00050ED3
};
// SIZE 0x8

#endif
