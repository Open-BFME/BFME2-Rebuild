// RadarUpgradeModuleData.h: the INI data of RadarUpgrade. SIZE 0x11C.
// UpgradeModuleData (0x118) plus one Bool: DisableProof at +0x118. buildFieldParse (0x004B476C)
// chains UpgradeMuxData's table at +0x8, then DisableProof (INI::parseBool).
#pragma once
#ifndef BFME2_RADARUPGRADEMODULEDATA_H
#define BFME2_RADARUPGRADEMODULEDATA_H

#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"

class MultiIniFieldParse;

class RadarUpgradeModuleData : public UpgradeModuleData
{
public:
	static void buildFieldParse(MultiIniFieldParse &parse);				// 0x004B476C

	Bool m_disableProof;												// 0x118 DisableProof
};

typedef char RadarUpgradeModuleDataSize[sizeof(RadarUpgradeModuleData) == 0x11C ? 1 : -1];

#endif
