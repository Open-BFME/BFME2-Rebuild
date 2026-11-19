// SupplyCenterDockUpdateModuleData.h: the INI data of SupplyCenterDockUpdate. SIZE 0x1C.
// Target facts: DockUpdateModuleData (0x10) is the base. The field table at retail 0x00C51A80
// has three entries and the zero sentinel: ValueMultiplier (INI::parseReal) at +0x10,
// BonusScience (INI::parseScience) at +0x14 and BonusScienceMultiplier (INI::parsePercentToReal)
// at +0x18. buildFieldParse 0x004A0DAC calls the base's buildFieldParse, then adds that table.
#pragma once
#ifndef BFME2_SUPPLYCENTERDOCKUPDATEMODULEDATA_H
#define BFME2_SUPPLYCENTERDOCKUPDATEMODULEDATA_H

#include "Common/GameType.h"
#include "Common/Science.h"
#include "GameLogic/Module/DockUpdate.h"

class SupplyCenterDockUpdateModuleData : public DockUpdateModuleData
{
public:
	SupplyCenterDockUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);	// 0x004A0DAC

	Real m_valueMultiplier;				// 0x10
	ScienceType m_bonusScience;			// 0x14
	Real m_bonusScienceMultiplier;		// 0x18
};
// SIZE 0x1C

// Retail VA 0x00C51A80: three fields and the zero sentinel.
extern const FieldParse BfmeSupplyCenterDockUpdateFields[];

#endif
