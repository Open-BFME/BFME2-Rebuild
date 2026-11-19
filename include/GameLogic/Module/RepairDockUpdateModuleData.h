// RepairDockUpdateModuleData.h: the INI data of RepairDockUpdate. SIZE 0x14.
// Target facts: DockUpdateModuleData (0x10) is the base. The field table at retail 0x00C51C10 has one
// entry and the zero sentinel: TimeForFullHeal (INI::parseDurationReal) at +0x10, a Real.
#pragma once
#ifndef BFME2_REPAIRDOCKUPDATEMODULEDATA_H
#define BFME2_REPAIRDOCKUPDATEMODULEDATA_H

#include "Common/GameType.h"
#include "GameLogic/Module/DockUpdate.h"

class RepairDockUpdateModuleData : public DockUpdateModuleData
{
public:
	static void buildFieldParse(MultiIniFieldParse &parse);	// 0x004A1170

	Real m_timeForFullHeal;						// 0x10
};
// SIZE 0x14

#endif
