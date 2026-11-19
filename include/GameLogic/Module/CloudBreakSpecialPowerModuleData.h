// CloudBreakSpecialPowerModuleData.h: the INI data of CloudBreakSpecialPower. SIZE 0x8C.
// SpecialPowerModuleData (0x7C) plus the cloud-break radius, its FX list, the sunbeam
// object name and the object spacing. Layout from the constructor 0x004C479A and the
// field table; buildFieldParse is not written yet.
#pragma once
#ifndef BFME2_CLOUDBREAKSPECIALPOWERMODULEDATA_H
#define BFME2_CLOUDBREAKSPECIALPOWERMODULEDATA_H

#include "Common/AsciiString.h"
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"

class FXList;

class CloudBreakSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	CloudBreakSpecialPowerModuleData();								// 0x004C479A

	Real m_cloudBreakRadius;										// +0x7C CloudBreakRadius
	const FXList *m_cloudBreakFX;									// +0x80 CloudBreakFX
	AsciiString m_sunbeamObject;									// +0x84 SunbeamObject
	Real m_objectSpacing;											// +0x88 ObjectSpacing
};

typedef char CloudBreakSpecialPowerModuleDataSize[sizeof(CloudBreakSpecialPowerModuleData) == 0x8C ? 1 : -1];

#endif
