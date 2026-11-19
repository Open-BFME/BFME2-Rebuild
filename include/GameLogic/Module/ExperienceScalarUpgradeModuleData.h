// ExperienceScalarUpgradeModuleData.h: the INI data of ExperienceScalarUpgrade. SIZE 0x11C.
// Target facts: the data class derives from UpgradeModuleData (0x118 bytes, the upgrade trigger
// data at +0x8) and adds AddXPScalar as a Real at +0x118. Its constructor (0x004B6187) calls the
// base constructor, installs its own vtable and zeroes AddXPScalar.
// buildFieldParse (0x004B61A4) adds the table at VA 0x00C58524; the parsers are not declared here.
#pragma once
#ifndef BFME2_EXPERIENCE_SCALAR_UPGRADE_MODULE_DATA_H
#define BFME2_EXPERIENCE_SCALAR_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"

class INI;
class MultiIniFieldParse;

class ExperienceScalarUpgradeModuleData : public UpgradeModuleData
{
public:
	ExperienceScalarUpgradeModuleData();								// 0x004B6187
	virtual ~ExperienceScalarUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);				// 0x004B61A4

	Real m_addXPScalar;													// +0x118 AddXPScalar
};
// SIZE 0x11C
typedef char ExperienceScalarUpgradeModuleData_size[(sizeof(ExperienceScalarUpgradeModuleData) == 0x11C) ? 1 : -1];
typedef char ExperienceScalarUpgradeModuleData_off_m_addXPScalar[(offsetof(ExperienceScalarUpgradeModuleData, m_addXPScalar) == 0x118) ? 1 : -1];

#endif
