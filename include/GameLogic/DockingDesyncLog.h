// DockingDesyncLog.h: the desync-trace switches the dozer and worker task code test (retail leaves them
// in the shipped build: DozerAIUpdate::newTask 0x00489A07 and WorkerAIUpdate::newTask 0x004AADB1 write
// "DockingDesync(...)" lines to the logic random log when g_bfmeDockingDesyncLog is set).
// Target facts: the bool at VA 0x00DCBF44 (map ?g_bfmeDockingDesyncLog@@3_NA), the bool at VA 0x00E03CA8
// (map ?g_bfmeDockingTraceActive@@3_NA, set before and cleared after the task's dock position is chosen) and the
// logic random log at VA 0x00DFEFF0 (theLogicRandomLogFile, Common/CRCDebug.h): each trace line goes through
// the log object's addLine.
#pragma once
#ifndef BFME2_DOCKINGDESYNCLOG_H
#define BFME2_DOCKINGDESYNCLOG_H

#include "Common/CRCDebug.h"

extern bool g_bfmeDockingDesyncLog;
extern bool g_bfmeDockingTraceActive;
// The critter-desync switch: the byte at VA 0x00E03745 (map ?g_00E03745@@3EA, no name yet; role name). 69
// AIStates.cpp rows (the move states' computePath/onEnter/update, AIInternalMoveToState::getAdjustsDestination
// 0x00344138) test it before writing "CritterDesync: ..." lines to the logic random log.
extern bool g_bfmeCritterDesyncLog;

#endif
