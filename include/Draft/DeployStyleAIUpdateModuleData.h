// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BEECB0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// DeployStyleAIUpdateModuleData.h: the INI data of DeployStyleAIUpdate. SIZE 0x74.
// Target facts: ModuleFactory registers the module with operator new 0x74 and
// constructor RVA 0x00255154; buildFieldParse RVA 0x0024D1CB calls AIUpdateModuleData::buildFieldParse, then adds VA 0x00BEECB0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DEPLOY_STYLE_A_I_UPDATE_MODULE_DATA_H
#define BFME2_DEPLOY_STYLE_A_I_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/AIUpdate.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base AIUpdateModuleData: the constructor calls its constructor on this. It ends at 0x64 (its own operator-new size);
// this class's members follow it.
typedef char DeployStyleAIUpdateModuleData_base_size[(sizeof(AIUpdateModuleData) == 0x64) ? 1 : -1];

class DeployStyleAIUpdateModuleData : public AIUpdateModuleData
{
public:
	static ModuleData *friend_newModuleData(INI *ini);
	DeployStyleAIUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedInt m_unpackTime;                              // +0x64 UnpackTime; duration, frames
	UnsignedInt m_packTime;                                // +0x68 PackTime; duration, frames
	Bool m_resetTurretBeforePacking;                       // +0x6C ResetTurretBeforePacking
	Bool m_turretsFunctionOnlyWhenDeployed;                // +0x6D TurretsFunctionOnlyWhenDeployed
	Bool m_turretsMustCenterBeforePacking;                 // +0x6E TurretsMustCenterBeforePacking
	Bool m_mustDeployToAttack;                             // +0x6F MustDeployToAttack
	AsciiString m_deployedAttributeModifier;               // +0x70 DeployedAttributeModifier
};
// SIZE 0x74
typedef char DeployStyleAIUpdateModuleData_size[(sizeof(DeployStyleAIUpdateModuleData) == 0x74) ? 1 : -1];
typedef char DeployStyleAIUpdateModuleData_off_m_unpackTime[(offsetof(DeployStyleAIUpdateModuleData, m_unpackTime) == 0x64) ? 1 : -1];
typedef char DeployStyleAIUpdateModuleData_off_m_packTime[(offsetof(DeployStyleAIUpdateModuleData, m_packTime) == 0x68) ? 1 : -1];
typedef char DeployStyleAIUpdateModuleData_off_m_resetTurretBeforePacking[(offsetof(DeployStyleAIUpdateModuleData, m_resetTurretBeforePacking) == 0x6C) ? 1 : -1];
typedef char DeployStyleAIUpdateModuleData_off_m_turretsFunctionOnlyWhenDeployed[(offsetof(DeployStyleAIUpdateModuleData, m_turretsFunctionOnlyWhenDeployed) == 0x6D) ? 1 : -1];
typedef char DeployStyleAIUpdateModuleData_off_m_turretsMustCenterBeforePacking[(offsetof(DeployStyleAIUpdateModuleData, m_turretsMustCenterBeforePacking) == 0x6E) ? 1 : -1];
typedef char DeployStyleAIUpdateModuleData_off_m_mustDeployToAttack[(offsetof(DeployStyleAIUpdateModuleData, m_mustDeployToAttack) == 0x6F) ? 1 : -1];
typedef char DeployStyleAIUpdateModuleData_off_m_deployedAttributeModifier[(offsetof(DeployStyleAIUpdateModuleData, m_deployedAttributeModifier) == 0x70) ? 1 : -1];

#endif
