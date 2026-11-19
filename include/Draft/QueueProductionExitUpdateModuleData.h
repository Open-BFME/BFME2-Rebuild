// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF1E48).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// QueueProductionExitUpdateModuleData.h: the INI data of QueueProductionExitUpdate. SIZE 0x34.
// Target facts: ModuleFactory registers the module with operator new 0x34 and
// constructor RVA 0x002541FA; buildFieldParse RVA 0x0025423E adds VA 0x00BF1E48 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_QUEUE_PRODUCTION_EXIT_UPDATE_MODULE_DATA_H
#define BFME2_QUEUE_PRODUCTION_EXIT_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/Coord3D.h"

class INI;
class MultiIniFieldParse;

class QueueProductionExitUpdateModuleData : public ModuleData
{
public:
	QueueProductionExitUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Coord3D m_unitCreatePoint;                             // +0x8 UnitCreatePoint
	Coord3D m_naturalRallyPoint;                           // +0x14 NaturalRallyPoint
	UnsignedInt m_exitDelay;                               // +0x20 ExitDelay; duration, frames
	Bool m_allowAirborneCreation;                          // +0x24 AllowAirborneCreation
	UnsignedInt m_initialBurst;                            // +0x28 InitialBurst
	Real m_placementViewAngle;                             // +0x2C PlacementViewAngle; angle
	Bool m_noExitPath;                                     // +0x30 NoExitPath
	Bool m_canRallyToSlaughter;                            // +0x31 CanRallyToSlaughter
	Bool m_useReturnToFormation;                           // +0x32 UseReturnToFormation
};
// SIZE 0x34
typedef char QueueProductionExitUpdateModuleData_size[(sizeof(QueueProductionExitUpdateModuleData) == 0x34) ? 1 : -1];
typedef char QueueProductionExitUpdateModuleData_off_m_unitCreatePoint[(offsetof(QueueProductionExitUpdateModuleData, m_unitCreatePoint) == 0x8) ? 1 : -1];
typedef char QueueProductionExitUpdateModuleData_off_m_naturalRallyPoint[(offsetof(QueueProductionExitUpdateModuleData, m_naturalRallyPoint) == 0x14) ? 1 : -1];
typedef char QueueProductionExitUpdateModuleData_off_m_exitDelay[(offsetof(QueueProductionExitUpdateModuleData, m_exitDelay) == 0x20) ? 1 : -1];
typedef char QueueProductionExitUpdateModuleData_off_m_allowAirborneCreation[(offsetof(QueueProductionExitUpdateModuleData, m_allowAirborneCreation) == 0x24) ? 1 : -1];
typedef char QueueProductionExitUpdateModuleData_off_m_initialBurst[(offsetof(QueueProductionExitUpdateModuleData, m_initialBurst) == 0x28) ? 1 : -1];
typedef char QueueProductionExitUpdateModuleData_off_m_placementViewAngle[(offsetof(QueueProductionExitUpdateModuleData, m_placementViewAngle) == 0x2C) ? 1 : -1];
typedef char QueueProductionExitUpdateModuleData_off_m_noExitPath[(offsetof(QueueProductionExitUpdateModuleData, m_noExitPath) == 0x30) ? 1 : -1];
typedef char QueueProductionExitUpdateModuleData_off_m_canRallyToSlaughter[(offsetof(QueueProductionExitUpdateModuleData, m_canRallyToSlaughter) == 0x31) ? 1 : -1];
typedef char QueueProductionExitUpdateModuleData_off_m_useReturnToFormation[(offsetof(QueueProductionExitUpdateModuleData, m_useReturnToFormation) == 0x32) ? 1 : -1];

#endif
