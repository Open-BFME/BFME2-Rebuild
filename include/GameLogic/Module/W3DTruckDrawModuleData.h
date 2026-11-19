// W3DTruckDrawModuleData.h: the INI data of W3DTruckDraw. SIZE 0x1F0.
// Target facts: ModuleFactory registers the module with operator new 0x1F0 and
// constructor RVA 0x000CB1C9; buildFieldParse RVA 0x000CB133 calls W3DModelDrawModuleData::buildFieldParse, then adds VA 0x00BCC358 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_W3_D_TRUCK_DRAW_MODULE_DATA_H
#define BFME2_W3_D_TRUCK_DRAW_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/W3DModelDrawModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base W3DModelDrawModuleData: the constructor calls its constructor on this. It ends at 0x188 (its own operator-new size);
// this class's members follow it.
typedef char W3DTruckDrawModuleData_base_size[(sizeof(W3DModelDrawModuleData) == 0x188) ? 1 : -1];

class W3DTruckDrawModuleData : public W3DModelDrawModuleData
{
public:
	W3DTruckDrawModuleData();
	virtual ~W3DTruckDrawModuleData();	// out of line in retail (ModuleFactory deletes through slot 0)
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_dust;                                    // +0x188 Dust
	AsciiString m_dirtSpray;                               // +0x18C DirtSpray
	AsciiString m_powerslideSpray;                         // +0x190 PowerslideSpray
	AsciiString m_leftFrontTireBone;                       // +0x194 LeftFrontTireBone
	AsciiString m_rightFrontTireBone;                      // +0x198 RightFrontTireBone
	AsciiString m_leftRearTireBone;                        // +0x19C LeftRearTireBone
	AsciiString m_rightRearTireBone;                       // +0x1A0 RightRearTireBone
	AsciiString m_midLeftFrontTireBone;                    // +0x1A4 MidLeftFrontTireBone
	AsciiString m_midRightFrontTireBone;                   // +0x1A8 MidRightFrontTireBone
	AsciiString m_midLeftRearTireBone;                     // +0x1AC MidLeftRearTireBone
	AsciiString m_midRightRearTireBone;                    // +0x1B0 MidRightRearTireBone
	AsciiString m_midLeftMidTireBone;                      // +0x1B4 MidLeftMidTireBone
	AsciiString m_midRightMidTireBone;                     // +0x1B8 MidRightMidTireBone
	AsciiString m_leftFrontTireBone2;                      // +0x1BC LeftFrontTireBone2
	AsciiString m_rightFrontTireBone2;                     // +0x1C0 RightFrontTireBone2
	AsciiString m_leftRearTireBone2;                       // +0x1C4 LeftRearTireBone2
	AsciiString m_rightRearTireBone2;                      // +0x1C8 RightRearTireBone2
	AsciiString m_midLeftMidTireBone2;                     // +0x1CC MidLeftMidTireBone2
	AsciiString m_midRightMidTireBone2;                    // +0x1D0 MidRightMidTireBone2
	AsciiString m_cabBone;                                 // +0x1D4 CabBone
	AsciiString m_trailerBone;                             // +0x1D8 TrailerBone
	Real m_cabRotationMultiplier;                          // +0x1DC CabRotationMultiplier
	Real m_trailerRotationMultiplier;                      // +0x1E0 TrailerRotationMultiplier
	Real m_rotationDamping;                                // +0x1E4 RotationDamping
	Real m_tireRotationMultiplier;                         // +0x1E8 TireRotationMultiplier
	Real m_powerslideRotationAddition;                     // +0x1EC PowerslideRotationAddition
};
// SIZE 0x1F0
typedef char W3DTruckDrawModuleData_size[(sizeof(W3DTruckDrawModuleData) == 0x1F0) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_dust[(offsetof(W3DTruckDrawModuleData, m_dust) == 0x188) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_dirtSpray[(offsetof(W3DTruckDrawModuleData, m_dirtSpray) == 0x18C) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_powerslideSpray[(offsetof(W3DTruckDrawModuleData, m_powerslideSpray) == 0x190) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_leftFrontTireBone[(offsetof(W3DTruckDrawModuleData, m_leftFrontTireBone) == 0x194) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_rightFrontTireBone[(offsetof(W3DTruckDrawModuleData, m_rightFrontTireBone) == 0x198) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_leftRearTireBone[(offsetof(W3DTruckDrawModuleData, m_leftRearTireBone) == 0x19C) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_rightRearTireBone[(offsetof(W3DTruckDrawModuleData, m_rightRearTireBone) == 0x1A0) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_midLeftFrontTireBone[(offsetof(W3DTruckDrawModuleData, m_midLeftFrontTireBone) == 0x1A4) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_midRightFrontTireBone[(offsetof(W3DTruckDrawModuleData, m_midRightFrontTireBone) == 0x1A8) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_midLeftRearTireBone[(offsetof(W3DTruckDrawModuleData, m_midLeftRearTireBone) == 0x1AC) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_midRightRearTireBone[(offsetof(W3DTruckDrawModuleData, m_midRightRearTireBone) == 0x1B0) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_midLeftMidTireBone[(offsetof(W3DTruckDrawModuleData, m_midLeftMidTireBone) == 0x1B4) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_midRightMidTireBone[(offsetof(W3DTruckDrawModuleData, m_midRightMidTireBone) == 0x1B8) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_leftFrontTireBone2[(offsetof(W3DTruckDrawModuleData, m_leftFrontTireBone2) == 0x1BC) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_rightFrontTireBone2[(offsetof(W3DTruckDrawModuleData, m_rightFrontTireBone2) == 0x1C0) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_leftRearTireBone2[(offsetof(W3DTruckDrawModuleData, m_leftRearTireBone2) == 0x1C4) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_rightRearTireBone2[(offsetof(W3DTruckDrawModuleData, m_rightRearTireBone2) == 0x1C8) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_midLeftMidTireBone2[(offsetof(W3DTruckDrawModuleData, m_midLeftMidTireBone2) == 0x1CC) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_midRightMidTireBone2[(offsetof(W3DTruckDrawModuleData, m_midRightMidTireBone2) == 0x1D0) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_cabBone[(offsetof(W3DTruckDrawModuleData, m_cabBone) == 0x1D4) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_trailerBone[(offsetof(W3DTruckDrawModuleData, m_trailerBone) == 0x1D8) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_cabRotationMultiplier[(offsetof(W3DTruckDrawModuleData, m_cabRotationMultiplier) == 0x1DC) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_trailerRotationMultiplier[(offsetof(W3DTruckDrawModuleData, m_trailerRotationMultiplier) == 0x1E0) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_rotationDamping[(offsetof(W3DTruckDrawModuleData, m_rotationDamping) == 0x1E4) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_tireRotationMultiplier[(offsetof(W3DTruckDrawModuleData, m_tireRotationMultiplier) == 0x1E8) ? 1 : -1];
typedef char W3DTruckDrawModuleData_off_m_powerslideRotationAddition[(offsetof(W3DTruckDrawModuleData, m_powerslideRotationAddition) == 0x1EC) ? 1 : -1];

#endif
