// HeroDieModuleData.h: the INI data of HeroDie. SIZE 0x3C (ModuleFactory operator new).
// Target facts: the field table entry at 0x007F0928 parses the token SpecialPowerTemplate into +0x38;
// everything below it is DieModuleData (0x38). HeroDie::onDie 0x004C236A hands the +0x38 pointer to
// its iterateObjects callback 0x004C2299, which asks each object for that power's module.
#pragma once
#ifndef BFME2_HERO_DIE_MODULE_DATA_H
#define BFME2_HERO_DIE_MODULE_DATA_H

#include <stddef.h>
#include "GameLogic/Module/DieModule.h"

class SpecialPowerTemplate;

class HeroDieModuleData : public DieModuleData
{
public:
	const SpecialPowerTemplate *m_specialPowerTemplate;	// +0x38 SpecialPowerTemplate
};
// SIZE 0x3C
typedef char HeroDieModuleData_size[(sizeof(HeroDieModuleData) == 0x3C) ? 1 : -1];
typedef char HeroDieModuleData_off_m_specialPowerTemplate[(offsetof(HeroDieModuleData, m_specialPowerTemplate) == 0x38) ? 1 : -1];

#endif
