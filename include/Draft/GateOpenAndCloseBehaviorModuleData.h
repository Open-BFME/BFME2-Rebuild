// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF19F8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_soundOpeningGateLoop, m_soundFinishedOpeningGate, m_soundClosingGateLoop, m_soundFinishedClosingGate, m_geometryForOpen, m_geometryForClosed.
// GateOpenAndCloseBehaviorModuleData.h: the INI data of GateProxyBehavior. SIZE 0x4C.
// Target facts: ModuleFactory registers the module with operator new 0x4C and
// constructor RVA 0x00498E2E; buildFieldParse RVA 0x00253F0E adds VA 0x00BF19F8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_soundOpeningGateLoop +0x1C: INI::parseAudioEventRTS 0x00339900
//   m_soundFinishedOpeningGate +0x20: INI::parseAudioEventRTS 0x00339900
//   m_soundClosingGateLoop +0x24: INI::parseAudioEventRTS 0x00339900
//   m_soundFinishedClosingGate +0x28: INI::parseAudioEventRTS 0x00339900
//   m_geometryForOpen +0x2C: INI::parseAsciiStringVectorAppend 0x0002E896
//   m_geometryForClosed +0x38: INI::parseAsciiStringVectorAppend 0x0002E896
#pragma once
#ifndef BFME2_GATE_OPEN_AND_CLOSE_BEHAVIOR_MODULE_DATA_H
#define BFME2_GATE_OPEN_AND_CLOSE_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"
#include "Common/OpaqueRefElement4.h"
#include <vector>

class INI;
class MultiIniFieldParse;

class GateOpenAndCloseBehaviorModuleData : public ModuleData
{
public:
	GateOpenAndCloseBehaviorModuleData();										// 0x00498E2E
	virtual ~GateOpenAndCloseBehaviorModuleData();							// 0x00498D46
	static void buildFieldParse(MultiIniFieldParse &parse);
	Int getTimeBeforePlayingOpenSound() const;			// 0x00498707: +0x44, or +0xC when it holds the default sentinel
	Int getTimeBeforePlayingClosedSound() const;		// 0x00498716: +0x48, or +0xC when it holds the default sentinel

	Bool m_openByDefault;                                  // +0x8 OpenByDefault
	UnsignedInt m_resetTimeInMilliseconds;                 // +0xC ResetTimeInMilliseconds; duration, frames
	UnsignedInt m_percentOpenForPathing;                   // +0x10 PercentOpenForPathing
	AsciiString m_proxy;                                   // +0x14 Proxy
	Bool m_repelCollidingUnits;                            // +0x18 RepelCollidingUnits
	UnsignedByte m_unk0x19[0x3];
	OpaqueRefElement4 m_soundOpeningGateLoop;              // +0x1C SoundOpeningGateLoop; unresolved: INI::parseAudioEventRTS
	OpaqueRefElement4 m_soundFinishedOpeningGate;          // +0x20 SoundFinishedOpeningGate; unresolved: INI::parseAudioEventRTS
	OpaqueRefElement4 m_soundClosingGateLoop;              // +0x24 SoundClosingGateLoop; unresolved: INI::parseAudioEventRTS
	OpaqueRefElement4 m_soundFinishedClosingGate;          // +0x28 SoundFinishedClosingGate; unresolved: INI::parseAudioEventRTS
	_STL::vector<AsciiString> m_geometryForOpen;           // +0x2C GeometryForOpen: INI::parseAsciiStringVectorAppend; the constructor 0x00498E2E clears it and pushes "OpenLeft", "OpenRight"
	_STL::vector<AsciiString> m_geometryForClosed;         // +0x38 GeometryForClosed: INI::parseAsciiStringVectorAppend; the constructor pushes "Closed"
	UnsignedInt m_timeBeforePlayingOpenSound;              // +0x44 TimeBeforePlayingOpenSound; duration, frames
	UnsignedInt m_timeBeforePlayingClosedSound;            // +0x48 TimeBeforePlayingClosedSound; duration, frames
};
// SIZE 0x4C
typedef char GateOpenAndCloseBehaviorModuleData_size[(sizeof(GateOpenAndCloseBehaviorModuleData) == 0x4C) ? 1 : -1];
typedef char GateOpenAndCloseBehaviorModuleData_off_m_openByDefault[(offsetof(GateOpenAndCloseBehaviorModuleData, m_openByDefault) == 0x8) ? 1 : -1];
typedef char GateOpenAndCloseBehaviorModuleData_off_m_resetTimeInMilliseconds[(offsetof(GateOpenAndCloseBehaviorModuleData, m_resetTimeInMilliseconds) == 0xC) ? 1 : -1];
typedef char GateOpenAndCloseBehaviorModuleData_off_m_percentOpenForPathing[(offsetof(GateOpenAndCloseBehaviorModuleData, m_percentOpenForPathing) == 0x10) ? 1 : -1];
typedef char GateOpenAndCloseBehaviorModuleData_off_m_proxy[(offsetof(GateOpenAndCloseBehaviorModuleData, m_proxy) == 0x14) ? 1 : -1];
typedef char GateOpenAndCloseBehaviorModuleData_off_m_repelCollidingUnits[(offsetof(GateOpenAndCloseBehaviorModuleData, m_repelCollidingUnits) == 0x18) ? 1 : -1];
typedef char GateOpenAndCloseBehaviorModuleData_off_m_soundOpeningGateLoop[(offsetof(GateOpenAndCloseBehaviorModuleData, m_soundOpeningGateLoop) == 0x1C) ? 1 : -1];
typedef char GateOpenAndCloseBehaviorModuleData_off_m_soundFinishedOpeningGate[(offsetof(GateOpenAndCloseBehaviorModuleData, m_soundFinishedOpeningGate) == 0x20) ? 1 : -1];
typedef char GateOpenAndCloseBehaviorModuleData_off_m_soundClosingGateLoop[(offsetof(GateOpenAndCloseBehaviorModuleData, m_soundClosingGateLoop) == 0x24) ? 1 : -1];
typedef char GateOpenAndCloseBehaviorModuleData_off_m_soundFinishedClosingGate[(offsetof(GateOpenAndCloseBehaviorModuleData, m_soundFinishedClosingGate) == 0x28) ? 1 : -1];
typedef char GateOpenAndCloseBehaviorModuleData_off_m_geometryForOpen[(offsetof(GateOpenAndCloseBehaviorModuleData, m_geometryForOpen) == 0x2C) ? 1 : -1];
typedef char GateOpenAndCloseBehaviorModuleData_off_m_geometryForClosed[(offsetof(GateOpenAndCloseBehaviorModuleData, m_geometryForClosed) == 0x38) ? 1 : -1];
typedef char GateOpenAndCloseBehaviorModuleData_off_m_timeBeforePlayingOpenSound[(offsetof(GateOpenAndCloseBehaviorModuleData, m_timeBeforePlayingOpenSound) == 0x44) ? 1 : -1];
typedef char GateOpenAndCloseBehaviorModuleData_off_m_timeBeforePlayingClosedSound[(offsetof(GateOpenAndCloseBehaviorModuleData, m_timeBeforePlayingClosedSound) == 0x48) ? 1 : -1];

#endif
