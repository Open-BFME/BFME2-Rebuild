// TeleportToCasterSpecialPowerModuleData.h: the INI data of the teleport-to-caster
// special power. SIZE 0xDC.
// Target facts: constructor 0x004CD34E chains SpecialAbilityUpdateModuleData
// (0x0044EB54) and then zeroes +0xC8..+0xD8; buildFieldParse 0x004CD389 chains
// the base table and registers the five fields retail names Radius,
// MinDestinationRadius, MaxDestinationRadius, TriggerFX and TargetFX at those
// offsets; the factory news 0xDC.
#pragma once
#ifndef BFME2_TELEPORTTOCASTERSPECIALPOWERMODULEDATA_H
#define BFME2_TELEPORTTOCASTERSPECIALPOWERMODULEDATA_H

#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"

class FXList;

class TeleportToCasterSpecialPowerModuleData : public SpecialAbilityUpdateModuleData
{
public:
	TeleportToCasterSpecialPowerModuleData();
	virtual ~TeleportToCasterSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_radius;						// 0xC8
	Real m_minDestinationRadius;		// 0xCC
	Real m_maxDestinationRadius;		// 0xD0
	const FXList *m_triggerFX;			// 0xD4
	const FXList *m_targetFX;			// 0xD8
};
// SIZE 0xDC

#endif
