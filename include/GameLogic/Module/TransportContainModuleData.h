// TransportContainModuleData.h: the INI data of TransportContain. SIZE 0x18C.
// Target facts: ModuleFactory registers the module with operator new 0x18C and
// constructor RVA 0x00468301; buildFieldParse RVA 0x004689DF calls OpenContainModuleData::buildFieldParse, then adds VA 0x00C44648 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_TRANSPORT_CONTAIN_MODULE_DATA_H
#define BFME2_TRANSPORT_CONTAIN_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/OpenContainModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base OpenContainModuleData: the constructor calls its constructor on this. It ends at 0x98 (its own operator-new size);
// this class's members follow it.
typedef char TransportContainModuleData_base_size[(sizeof(OpenContainModuleData) == 0x98) ? 1 : -1];

class TransportContainModuleData : public OpenContainModuleData
{
public:
	TransportContainModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Int m_slots;                                           // +0x98 Slots
	Real m_exitPitchRate;                                  // +0x9C ExitPitchRate; angular velocity
	AsciiString m_exitBone;                                // +0xA0 ExitBone
	UnsignedByte m_unk0xA4[0x98];
	Bool m_forceOrientationContainer;	// INI ForceOrientationContainer
	UnsignedByte m_unk0x13D;
	Bool m_scatterNearbyOnExit;                            // +0x13E ScatterNearbyOnExit
	Bool m_orientLikeContainerOnExit;                      // +0x13F OrientLikeContainerOnExit
	Bool m_goAggressiveOnExit;                             // +0x140 GoAggressiveOnExit
	Bool m_resetMoodCheckTimeOnExit;                       // +0x141 ResetMoodCheckTimeOnExit
	Bool m_destroyRidersWhoAreNotFreeToExit;               // +0x142 DestroyRidersWhoAreNotFreeToExit
	UnsignedByte m_unk0x143[0x49];
};
// SIZE 0x18C
typedef char TransportContainModuleData_size[(sizeof(TransportContainModuleData) == 0x18C) ? 1 : -1];
typedef char TransportContainModuleData_off_m_slots[(offsetof(TransportContainModuleData, m_slots) == 0x98) ? 1 : -1];
typedef char TransportContainModuleData_off_m_exitPitchRate[(offsetof(TransportContainModuleData, m_exitPitchRate) == 0x9C) ? 1 : -1];
typedef char TransportContainModuleData_off_m_exitBone[(offsetof(TransportContainModuleData, m_exitBone) == 0xA0) ? 1 : -1];
typedef char TransportContainModuleData_off_m_scatterNearbyOnExit[(offsetof(TransportContainModuleData, m_scatterNearbyOnExit) == 0x13E) ? 1 : -1];
typedef char TransportContainModuleData_off_m_orientLikeContainerOnExit[(offsetof(TransportContainModuleData, m_orientLikeContainerOnExit) == 0x13F) ? 1 : -1];
typedef char TransportContainModuleData_off_m_goAggressiveOnExit[(offsetof(TransportContainModuleData, m_goAggressiveOnExit) == 0x140) ? 1 : -1];
typedef char TransportContainModuleData_off_m_resetMoodCheckTimeOnExit[(offsetof(TransportContainModuleData, m_resetMoodCheckTimeOnExit) == 0x141) ? 1 : -1];
typedef char TransportContainModuleData_off_m_destroyRidersWhoAreNotFreeToExit[(offsetof(TransportContainModuleData, m_destroyRidersWhoAreNotFreeToExit) == 0x142) ? 1 : -1];

#endif
