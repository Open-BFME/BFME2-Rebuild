// DockingDesyncLog.h: the desync-trace switches the dozer and worker task code test (retail leaves them
// in the shipped build: DozerAIUpdate::newTask 0x00489A07 and WorkerAIUpdate::newTask 0x004AADB1 write
// "DockingDesync(...)" lines to the logic random log when g_bfmeDockingDesyncLog is set).
// Target facts: the bool at VA 0x00DCBF44 (map ?g_bfmeDockingDesyncLog@@3_NA), the bool at VA 0x00E03CA8
// (map ?g_bfmeDockingTraceActive@@3_NA, set before and cleared after the task's dock position is chosen) and the
// log file pointer at VA 0x00DFEFF0 (map _theLogicRandomLogFile). RandomValue.cpp still holds the file pointer as a
// file static: the data owner exports it.
#pragma once
#ifndef BFME2_DOCKINGDESYNCLOG_H
#define BFME2_DOCKINGDESYNCLOG_H

extern bool g_bfmeDockingDesyncLog;
extern bool g_bfmeDockingTraceActive;
// The critter-desync switch: the byte at VA 0x00E03745 (map ?g_00E03745@@3EA, no name yet; role name). 69
// AIStates.cpp rows (the move states' computePath/onEnter/update, AIInternalMoveToState::getAdjustsDestination
// 0x00344138) test it before writing "CritterDesync: ..." lines to the logic random log.
extern bool g_bfmeCritterDesyncLog;
extern "C" void *theLogicRandomLogFile;
extern "C" int __cdecl fprintf(void *stream, const char *format, ...);

#endif
